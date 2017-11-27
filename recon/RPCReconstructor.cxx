#include "detector/RPCHit.h"
#include "recon/RPCTrack.h"
#include "recon/RPCReconstructor.h"
#include "his/HisFile.h" 
#include "event/EventHandler.h"
#include "detector/RPCCluster.h"
#include "user/RPCAnalysis.h"
#include "event/Handler.h"
#include "event/RPCLETSHeader.h"
#include "event/Header.h"
#include <map>
double RPCReconstructor::chisquamaZ=7;
double RPCReconstructor::chisquamaX=7;


RPCReconstructor::RPCReconstructor(std::vector<RPCCluster *> RPCRedList,int numpl, int numpc,
                                  int minhits,int maxhits) {

   MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
   Memory->increase();

   m_fullHitList = RPCRedList;
   m_numplnotr = numpl;
   m_numpc=numpc;
   m_minhits=minhits;
   m_maxhits=maxhits;
}

RPCReconstructor::RPCReconstructor(int numpl, int numpc,
                                  int minhits,int maxhits) {

   MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
   Memory->increase();

   m_numplnotr = numpl;
   m_numpc=numpc;
   m_minhits=minhits;
   m_maxhits=maxhits;
}


void RPCReconstructor::dump(){

   std::vector<RPCCluster *>::iterator ih; 
   EventHandler * eventHandler = EventHandler::getEventHandler();

   std::cout <<"RPCReconstructor: #hits " << m_fullHitList.size()<<std::endl; 
   for( ih=m_fullHitList.begin(); ih!=m_fullHitList.end(); ++ih) {
      std::cout << (*ih)->chamber() << " " << (*ih)->plane()   << " "
        << (*ih)->X() << " " << (*ih)->Y() << " " << (*ih)->Z() << " " << (*ih)->Err() << std::endl;
   }
}

RPCReconstructor::~RPCReconstructor() {
   MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
   Memory->decrease();
}


/**************************************************************************
 *
 * Make a pattern Recognition in the Lecce Test Stand.   First checks for the cleanest
 * planes and with those make a candidate track. Then try to add other chambers
 * makes only a single track.
 *
 ***************************************************************************/

std::vector<RPCTrack *> RPCReconstructor::RPCPatRec(int deb, bool allchambers) {
   
   m_debug=deb;
   m_flag=0;
   EventHandler * evh =EventHandler::getEventHandler(); 
   if(m_debug)std::cout << std::endl << "RPCPatRec: event "<< evh->eventNumber() << std::endl;

   Handler<RPCLETSHeader>  &hdh = Handler<RPCLETSHeader>::getHandle(); 

   
   RPCLETSHeader * head=hdh[hdh.size()-1];

   std::vector<RPCTrack *> TrackList;   
   std::vector<RPCCluster *>::iterator ih;
   std::vector<RPCCluster *> cleanClusList,chamClusList,bestList;

   int neta=0,nphi=0;
// load only the hits belonging to trigger chambers
   std::vector<RPCChamber*> chambers = evh->GetRPCChambers();
   for(int jch=0; jch!=chambers.size();jch++) { 
     RPCChamber * theCham = chambers[jch];
     int ch=theCham->Chamber();
 // use measurement chambers for tracking only if allchambers flag is set (default is allchambers=false)
     if(!head->IsTriggerChamber( ch ) && !allchambers) continue; 
// loop on list of layers in this chamber
     for(int ic=0; ic!=theCham->nLayers();ic++) {   
// get the list of planes belonging to this layer    
       std::vector<RPCStripPlane *> thePlanes=theCham->LayerPlanes(ic+1);  
       int la=thePlanes[0]->RPCgeometry()->layer();      
// now we should require maximum one  hit per layer(eta and phi) is present
       std::vector<RPCStripPlane *>::iterator is;
       int totphi=0,toteta=0;
       for(is=thePlanes.begin(); is!=thePlanes.end();is++) {
         std::vector<RPCCluster*> cl = (*is)->clusList();
         if((*is)->RPCgeometry()->StripDir()==0)toteta+=cl.size();
         if((*is)->RPCgeometry()->StripDir()==2)totphi+=cl.size();
       }
// if clean planes put in cleanClusList and count clean  planes in eta  and phi
       if(toteta  == 1){
         neta++; 
         for(is=thePlanes.begin(); is!=thePlanes.end();is++) {
           if((*is)->RPCgeometry()->StripDir()==0){
             std::vector<RPCCluster*> cl = (*is)->clusList();
             for(ih=cl.begin(); ih!=cl.end();ih++)cleanClusList.push_back(*ih);
           }
         }
       //std::cout<<" gabrita:only one hit in eta"<< std::endl;
       } else {
// else put in rest of list       
         for(is=thePlanes.begin(); is!=thePlanes.end();is++) {
           if((*is)->RPCgeometry()->StripDir()==0){
             std::vector<RPCCluster*> cl = (*is)->clusList();
             for(ih=cl.begin(); ih!=cl.end();ih++)chamClusList.push_back(*ih);
           }
//          std::cout<< " gabrita:more then one hit in eta"<< std::endl;
         }
       }
       if(totphi  == 1){
         nphi++;
         for(is=thePlanes.begin(); is!=thePlanes.end();is++) {
           if((*is)->RPCgeometry()->StripDir()==2){
             std::vector<RPCCluster*> cl = (*is)->clusList();
             for(ih=cl.begin(); ih!=cl.end();ih++)cleanClusList.push_back(*ih);
           }
         }
       //std::cout<<" gabrita:only one hit in phi"<< std::endl;
       } else {
// else put in rest of list       
         for(is=thePlanes.begin(); is!=thePlanes.end();is++) {
           if((*is)->RPCgeometry()->StripDir()==2){
             std::vector<RPCCluster*> cl = (*is)->clusList();
             for(ih=cl.begin(); ih!=cl.end();ih++)chamClusList.push_back(*ih);
           }
         }
//       std::cout<<" gabrita:more then  one hit in phi"<< std::endl;
       }
     } // loop on layers
   } // loop on chambers   
//   std::cout << "gabrita: neta = "<< neta << std::endl;
//   std::cout << "gabrita: nphi = "<< nphi << std::endl; 
// gabrita.... esce se neta ed nphi sono <3 e incrementa eta phi solo in presenza di 1 hit sulla camera ? 
   if(neta < 3 ){
    if(m_debug)std::cout << "RPCReconstructor::RPCPatRec: neta = "<< neta << std::endl; 
    m_flag=1;
    return TrackList;
   }
   if(nphi < 3 ){
    if(m_debug)std::cout << "RPCReconstructor::RPCPatRec: nphi = "<< nphi << std::endl; 
    m_flag=2;
    return TrackList;
   }
// 
// builds the initial track starting from pivot (clean) chambers, at least 3 
// per projection
   RPCTrack  *TrigTraccia = new RPCTrack(cleanClusList); 
   TrigTraccia->fitTrack();
// cut on ChiSquare of trigger track.
//  std::cout << "gabritagio  Chi^2  MAX X" << chisquamaX << std::endl;
//  std::cout << "gabritagio  Chi^2  MAX z" << chisquamaZ << std::endl;
//  std::cout << "gabritagio  Chi^2  x " << TrigTraccia->chisquareX() << std::endl;
//  std::cout << "gabritagio  Chi^2  z " << TrigTraccia->chisquareZ() << std::endl;

   if(TrigTraccia->chisquareX()>chisquamaX || 
     TrigTraccia->chisquareZ()>chisquamaZ) {
	 if(m_debug){
	   std::cout << "RPCReconstructor::RPCPatRec: bad Chi^2 for pivot track ";
	   std::cout << "try to remove worst hits\n";
	   TrigTraccia->print();
	 }	   
// remove hits with pull greater than 3 and refit
//     std::vector<RPCCluster*> worsthits=TrigTraccia->RemoveWorstHits(3); 
     while(TrigTraccia->RemoveWorstHit(3)){
       if(m_debug){
	     std::cout << "removed worst cluster"<<std::endl;
       }	   
	 }
/*
     if(m_debug){
	   std::cout << "worst clusters::"<<std::endl;
       for(std::vector<RPCCluster*>::iterator ic=worsthits.begin(); ic!=worsthits.end();ic++)(*ic)->print();	
     }
*/
// we should add worst hits to ChamClusList
//    for(std::vector<RPCCluster*>::iterator ic=worsthits.begin(); ic!=worsthits.end();ic++)chamClusList.push_back((*ic));

     if(TrigTraccia->chisquareX()>chisquamaX || 
        TrigTraccia->chisquareZ()>chisquamaZ     ){
       if(m_debug){
         std::cout << "RPCReconstructor::RPCPatRec: bad Chi^2 for pivot track, ";
         std::cout << " remove failed \n";
         TrigTraccia->print();
       }
       m_flag=3;
       delete TrigTraccia; 
       return TrackList;	   
     }       
   }

   if(m_debug){
	 std::cout << "RPCReconstructor::RPCPatRec: pivot Track " << std::endl;
	 TrigTraccia->print();
   }   
   
   
// estrapolates trigger track to each remaining hit
// and make a list of the nearest hits for each chamber plane
   bestList=getBestList(chamClusList,TrigTraccia->aX(),TrigTraccia->bX(),TrigTraccia->aZ(),TrigTraccia->bZ());

   if(m_debug)std::cout << std::endl << " bestList size : " << bestList.size() << std::endl << std::endl;

// no hits added, return the trigtraccia if enough hits
   if(bestList.size()==0){
     if(TrigTraccia->nHits(0) >= m_minhits/2 &&
	    TrigTraccia->nHits(2) >= m_minhits/2    ){
       TrackList.push_back(TrigTraccia);
       if(m_debug){
         std::cout << "========================================================" << std::endl;
         std::cout << " !!!!!!!!!!!!!!!  Track !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<< std::endl;
         TrigTraccia->print();
         std::cout << "========================================================" << std::endl;
       } 
     } else {
      delete TrigTraccia;
	  m_flag=3;
     }
	 return TrackList;
   } 	      

//
// add trigger hits in bestList   
//
   for(ih=cleanClusList.begin(); ih!=cleanClusList.end(); ih++)bestList.push_back(*ih);  

// here we give the hit list to the tracking object         
   RPCTrack  *NuovaTraccia = new RPCTrack(bestList);
// and fit    
   NuovaTraccia->fitTrack();


// require good chi square and minhits m_minhits hits.. 	  
   if(NuovaTraccia->chisquareX()<chisquamaX && 
	  NuovaTraccia->chisquareZ()<chisquamaZ && 
	  NuovaTraccia->nHits(0) >= m_minhits/2  &&
	  NuovaTraccia->nHits(2) >= m_minhits/2            ) {
    
// it'ok  store it in TrackList
     TrackList.push_back(NuovaTraccia);
     if(m_debug){
	   std::cout << "========================================================" << std::endl;
	   std::cout << " !!!!!!!!!!!!!!!  Track final  !!!!!!!!!!!!!!!!!!!!!!!!!"<< std::endl;
	   NuovaTraccia->print();
	   std::cout << "========================================================" << std::endl;
      } 
     return TrackList; 
   }
// here if something wrong

   if(m_debug ){
	 std::cout << "RPCReconstructor::RPCPatRec: bad Chi^2 for track, try to remove hits" << std::endl;
	 std::cout << NuovaTraccia->chisquareX() << " " << NuovaTraccia->chisquareZ() << 
	 " " << NuovaTraccia->nHits(0) << " " << NuovaTraccia->nHits(2) << std::endl;
   } 
// try remove hits with pull greater than 3 and refit
   while(NuovaTraccia->RemoveWorstHit(3)){
	 if(m_debug){
		std::cout << "removed worst cluster"<<std::endl;
	 }	   
   }
// check again for chi2 and hits after hit removal	 
//   std::cout << "gabritagio  Chi^2  MAX X" << chisquamaX << std::endl;
//   std::cout << "gabritagio  Chi^2  MAX z" << chisquamaZ << std::endl;

   if(NuovaTraccia->chisquareX()<chisquamaX && 
	  NuovaTraccia->chisquareZ()<chisquamaZ &&
	  NuovaTraccia->nHits(0) >= m_minhits/2 && 
	  NuovaTraccia->nHits(2) >= m_minhits/2    ){   
// it'ok  store it in TrackList
	 TrackList.push_back(NuovaTraccia);

	 if(m_debug){
	   std::cout << "========================================================" << std::endl;
	   std::cout << " !!!!!!!!!!!!!!!  Track !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<< std::endl;
	   NuovaTraccia->print();
	   std::cout << "========================================================" << std::endl;
	 } 
	 return TrackList; 
   }
// here if hit removal unsuccessful, still checks if TrigTraccia was good 
   if(TrigTraccia->nHits(0) >= m_minhits/2 &&
	  TrigTraccia->nHits(2) >= m_minhits/2   ){
	  TrackList.push_back(TrigTraccia);
	  if(m_debug){
		std::cout << "========================================================" << std::endl;
		std::cout << " !!!!!!!!!!!!!!!  Track !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<< std::endl;
		TrigTraccia->print();
		std::cout << "========================================================" << std::endl;
	  } 
	  return TrackList;
    }
// no hope, set flags now	
   if(NuovaTraccia->chisquareX()>chisquamaX || 
	  NuovaTraccia->chisquareZ()>chisquamaZ)m_flag=4;
   if(NuovaTraccia->nHits(0) < m_minhits/2 || 
	  NuovaTraccia->nHits(2) < m_minhits/2 )m_flag=5;   
// here we should delete the track and return the empty List
   delete NuovaTraccia;
   delete TrigTraccia;
   return TrackList;
}

/**************************************************************************
 *
 * Make a pattern Recognition in the X5 GIF Test. First checks for the cleanest
 * planes and with those make a track. Then try to add other chambers
 *
 ***************************************************************************/

std::vector<RPCTrack *> RPCReconstructor::RPCX5PatRec() {
/*
   std::vector<RPCTrack *> TrackList;
   
   std::vector<RPCCluster *>::iterator ih;
   std::vector<RPCCluster *> trig_clusList,chamClusList,bestList;
// create a map 
   std::map<int, int, std::less<int> > poin;
// scin
   poin[1]=0;poin[2]=1;poin[3]=2;
// phi   
   poin[11]=0;poin[14]=1;poin[21]=2;poin[24]=3;poin[31]=4;poin[34]=5;
// eta   
   poin[12]=0;poin[13]=1;poin[22]=2;poin[23]=3;poin[32]=4;poin[33]=5;
   
// check if there are enough hits in the list 
   if(m_fullHitList.size() < m_minhits ) {
//     std::cout << "RPCReconstructor::RPCX5PatRec: not enough hits in detector " << std::endl;
     return TrackList;
   }
// too many hits
   if(m_fullHitList.size() > m_maxhits ) {
//       std::cout << "RPCReconstructor::RPCX5PatRec: too many hits in detector " << std::endl;
       return TrackList;  
   }
//
// split the hitlist in phi eta and scin
//  and count clusters !plane
   std::vector<RPCCluster *> trig_List,phi_List,eta_List;
   int ncluphi[6],nclueta[6],nclutri[3];
   for(int ii=0;ii<6;ii++){ncluphi[ii]=0;nclueta[ii]=0;} 
   for(int ii=0;ii<3;ii++){nclutri[ii]=0;}                       
   
   for( ih=m_fullHitList.begin(); ih!=m_fullHitList.end(); ih++) {
      int id=(*ih)->chamber()*10+(*ih)->plane();
// eta      
      if((*ih)->stripDir() == 0){
        eta_List.push_back(*ih);
        nclueta[poin[id]]++;
      }
// scin      
      if((*ih)->stripDir() == 2 && (*ih)->chamber() == 0){
        trig_List.push_back(*ih);
        nclutri[poin[id]]++;
      }
// phi      
      if((*ih)->stripDir() == 2 && (*ih)->chamber() >  0){
        phi_List.push_back(*ih);
        ncluphi[poin[id]]++;
      }
    }  
//
// search for pivot planes (clean ones i.e. only one Cluster) in eta and phi
// makes a list with pivot planes and one for the others
   std::vector<RPCCluster *> address;      
//   
      for( ih=phi_List.begin(); ih!=phi_List.end(); ih++) {
        int id=(*ih)->chamber()*10+(*ih)->plane();
        if(ncluphi[poin[id]]==1){
          address.push_back(*ih);
        } 
        else chamClusList.push_back(*ih);    
      }  
      
      int phisize=address.size();

      if(address.size() >= 3){
//        std::cout << "found " << address.size() << " pivot planes" << std::endl;
      } else {
//        std::cout << "RPCReconstructor::RPCX5PatRec: not enough pivot planes on phi projection: " 
//        << address.size() << std::endl;
        return TrackList;
      }
      
      for( ih=eta_List.begin(); ih!=eta_List.end(); ih++) {
        int id=(*ih)->chamber()*10+(*ih)->plane();
        if(nclueta[poin[id]]==1){
          address.push_back(*ih);
        } 
        else chamClusList.push_back(*ih);     
      } 
      if((address.size()-phisize) >= 3){
//        std::cout << "found " << (address.size()-phisize) << " pivot planes" << std::endl;
      } else {
//        std::cout << "RPCReconstructor::RPCX5PatRec: not enough pivot planes on eta projection: " 
//        << (address.size()-phisize) << std::endl;
        return TrackList;
      }   
// 
// builds the initial track starting from pivot (clean) chambers at least 3 
// per projection
   RPCTrack  *TrigTraccia = new RPCTrack(address); 
   TrigTraccia->fitTrack();

   std::cout << "========================================================" << std::endl;
   std::cout << " !!!!!!!!!!!!!!! Pivot Track !!!!!!!!!!!!!!!!!!!!!!!!!!!"<< std::endl;
   TrigTraccia->print();
   std::cout << "========================================================" << std::endl;

// cut on ChiSquare of trigger track.
   if(TrigTraccia->chisquareX()>chisquamaX || 
     TrigTraccia->chisquareZ()>chisquamaZ) {
//     std::cout << "RPCReconstructor::RPCX5PatRec: bad Chi^2 for pivot track " << std::endl;
// should delete track 
     delete TrigTraccia;     
     return TrackList;	
   }
   
// estrapolates trigger track to each remaining hit
// and make a list of the nearest hits for each chamber plane
   double sestr;
   double mindis[m_numplnotr+3],errmin[m_numplnotr+3];

//   std::vector<RPCCluster *>::iterator iadd[m_numplnotr+3];
   int iadd[m_numplnotr+3];
   for (int ii=0;ii<m_numplnotr+3;ii++){
     iadd[ii]=0;
     mindis[ii]=99999.0;
     errmin[ii]=99999.0;
   }
   
   for(int ii=0; ii!=chamClusList.size(); ii++) {
      RPCCluster *cl=chamClusList[ii];      
      double estr = TrigTraccia->estrapPlane( cl->stripDir(), cl->Y(),sestr );
//      std::cout << "gabritagio  estrapplane condizione e prima del residuo" <<  cl->stripDir() << estr << std::endl;


//      std::cout << " coord, estrap coord, err";
//      if( (*ih)->stripDir() == 0)std::cout <<  (*ih)->X();      
//      if( (*ih)->stripDir() == 2)std::cout <<  (*ih)->Z();
//      std::cout << " " << estr << " " << sestr << std::endl;
      double resid;
      if( cl->stripDir() == 0)resid =fabs(cl->X() - estr);      
      if( cl->stripDir() == 2)resid =fabs(cl->Z() - estr);
//      std::cout << " Resid: " << resid << " Chamber,Plane " << 
//      (*ih)->chamber() << "," << (*ih)->plane() << std::endl;
// try 
      int Ch = cl->chamber();
      int Pl = cl->plane();

      int idpoi = (Ch-1)*m_numpc+Pl;
      if(Ch == 0 )idpoi=m_numplnotr+Pl-1;
         
      if(resid <= mindis[idpoi] ){
        mindis[idpoi]=resid;
// use error propagation (estrapolation and hit error)	
	errmin[idpoi]=sqrt(sestr*sestr+ cl->Err() * cl->Err() ); 
//        errmin[idpoi]=sestr; 
	iadd[idpoi]=ii;
      } 
      
   }
// get from input .....   
   float factor=3;	
// now store nearest hits in bestList
   for(int jh=0; jh!=m_numplnotr+3; jh++) {
     if(iadd[jh] != 0 && mindis[jh] < factor*errmin[jh]){
//      std::cout << "added: " << (chamClusList[iadd[jh]])->chamber() 
//                << "," << (chamClusList[iadd[jh]])->plane() << std::endl;
        bestList.push_back(chamClusList[iadd[jh]]);	
     }   
   }
//   std::cout << std::endl << " bestList size : " << bestList.size() << std::endl << std::endl;
   if(bestList.size()!=0){
// add trigger hits in bestList   
      for(ih=address.begin(); ih!=address.end(); ih++)bestList.push_back(*ih);  

      
// here we give the hit list to the tracking object         
      RPCTrack  *NuovaTraccia = new RPCTrack(bestList);
// and fit    
      NuovaTraccia->fitTrack();
//std::cout
//      NuovaTraccia->print();
//E.G. assume solo una traccia per evento.... e richiede almeno 8 hits.. cattivo...	  
      if(NuovaTraccia->chisquareX()>chisquamaX || 
         NuovaTraccia->chisquareZ()>chisquamaZ || 
         NuovaTraccia->nHits() < 6                ) {
//         std::cout << "RPCReconstructor::RPCX5PatRec: bad Chi^2 for track " << std::endl;
// here we should delete the track 
         delete NuovaTraccia;
         delete TrigTraccia;     
         return TrackList;
      }
// and store it in TrackList
      TrackList.push_back(NuovaTraccia);
// but delete the Trigger Track (hits include in Nuovatraccia)
      delete TrigTraccia;
// 
   } else if(TrigTraccia->nHits() >= 6){
      TrackList.push_back(TrigTraccia);
   } else {
      delete TrigTraccia;
      return TrackList;
   }	           
   
   
// clear all created lists   
   address.clear();   
   chamClusList.clear();
   bestList.clear();
   
   
   return TrackList;  
*/
}

/**************************************************************************
 *
 * Make a pattern Recognition in the X5 GIF Test. First search for two pivot
 * planes and with those make a track road. Then try to add other chambers
 *
 ***************************************************************************/

std::vector<RPCTrack *> RPCReconstructor::RPCX5PatRec2(int deb) {
/*   int m_debug=deb;
   EventHandler * evh =EventHandler::getEventHandler(); 
   if(m_debug)std::cout << std::endl << "RPCPatRec2: event "<< evh->eventNumber() << std::endl;
   
   
   std::vector<RPCTrack *> TrackList;
   
   std::vector<RPCCluster *>::iterator ih;
   std::vector<RPCCluster *> chamClusList;
   
// check if there are enough hits in the list 
   if(m_fullHitList.size() < m_minhits ) {
//     std::cout << "RPCReconstructor::RPCX5PatRec: not enough hits in detector " << std::endl;
     return TrackList;
   }
// too many hits
   if(m_fullHitList.size() > m_maxhits ) {
//       std::cout << "RPCReconstructor::RPCX5PatRec: too many hits in detector " << std::endl;
       return TrackList;  
   }
//
// split the hitlist in phi eta and scin and
// search for pivot planes in eta and phi, i.e. the farthest ones
// makes a list with pivot planes and one for the others
   std::vector<RPCCluster *> trig_List,phi_List,eta_List;
   float phiymin=999999;
   float phiymax=-999999; 
   int phiindmin=-1,phiindmax=-1;                   
   float etaymin=999999;
   float etaymax=-999999; 
   int etaindmin=-1,etaindmax=-1;   
   
   int maxsize=3;
   
   for(ih=m_fullHitList.begin(); ih!=m_fullHitList.end(); ih++) {
      int id=(*ih)->chamber()*10+(*ih)->plane();
// eta      
      if((*ih)->stripDir() == 0 && (*ih)->size()<=maxsize){
        eta_List.push_back(*ih);
        if((*ih)->Y() <= etaymin){
          etaymin= (*ih)->Y();
          etaindmin=eta_List.size()-1;
        } 
        if((*ih)->Y() >= etaymax){
          etaymax= (*ih)->Y();
          etaindmax=eta_List.size()-1;
        } 
      }
// scin      
      if((*ih)->stripDir() == 2 && (*ih)->chamber() == 0){
        trig_List.push_back(*ih);
      }
// phi      
      if((*ih)->stripDir() == 2 && (*ih)->chamber() >  0 && (*ih)->size()<=maxsize){
        phi_List.push_back(*ih);
        if((*ih)->Y() <= phiymin){
          phiymin= (*ih)->Y();
          phiindmin=phi_List.size()-1;
        } 
        if((*ih)->Y() >= phiymax){
          phiymax= (*ih)->Y();
          phiindmax=phi_List.size()-1;
        } 
      }
    }
    
    if(phiindmin!=-1)phiindmin=phi_List[phiindmin]->chamber()*10+phi_List[phiindmin]->plane();
    else return TrackList;
    if(phiindmax!=-1)phiindmax=phi_List[phiindmax]->chamber()*10+phi_List[phiindmax]->plane();
    else return TrackList;
    if(etaindmin!=-1)etaindmin=eta_List[etaindmin]->chamber()*10+eta_List[etaindmin]->plane();
    else return TrackList;
    if(etaindmax!=-1)etaindmax=eta_List[etaindmax]->chamber()*10+eta_List[etaindmax]->plane(); 
    else return TrackList;       
//
// makes a list with pivot planes and one for the others
    std::vector<RPCCluster *> lphimin,letamin,lphimax,letamax;           
      for(int i=0; i!=phi_List.size(); i++) {
//       int idsp=phi_List[i]->chamber()*10+phi_List[i]->plane();
//       std::cout << " idsp: " << idsp << "\n";
         if(phi_List[i]->Y() == phiymin){
           lphimin.push_back(phi_List[i]);
         }else if(phi_List[i]->Y() == phiymax){
           lphimax.push_back(phi_List[i]);
         } else {
          chamClusList.push_back(phi_List[i]);
         }
      } 
      int phisizemin=lphimin.size();
      int phisizemax=lphimax.size();
      if(m_debug)std::cout << "found " << phisizemin << "+" 
                           << phisizemax << " pivot clusters" << std::endl;
      if(m_debug)std::cout << "phiplanes " << phiindmin << "," 
                           << phiindmax << " are pivot planes" << std::endl;

 
      for(int i=0; i!=eta_List.size(); i++) {
        int idsp=eta_List[i]->chamber()*10+eta_List[i]->plane();
         if(eta_List[i]->Y() == etaymin){
           letamin.push_back(eta_List[i]);
         }else if(eta_List[i]->Y() == etaymax){
           letamax.push_back(eta_List[i]);
         } else {
          chamClusList.push_back(eta_List[i]);
         }
      } 
      int etasizemin=letamin.size();
      int etasizemax=letamax.size();
      if(m_debug)std::cout << "found " << etasizemin << "+" 
                           << etasizemax << " pivot clusters" << std::endl; 
      if(m_debug)std::cout << "etaplanes " << etaindmin << "," 
                           << etaindmax << "are pivot planes" << std::endl;



// molto cattivo, traccio solo se un solo cluster per piano pivot...
      if(phisizemin > 1 || phisizemax > 1 || 
         etasizemin > 1 || etasizemax > 1     )return TrackList;




// now loops on clusters in phi and build roads
      for(int i=0; i!=phisizemin; i++) {
        for(int j=0; j!=phisizemax; j++) {
          float phislope=(lphimax[j]->Z()-lphimin[i]->Z() )/
                         (lphimax[j]->Y()-lphimin[i]->Y() );
          float phiconst=-phislope*lphimax[j]->Y()+lphimax[j]->Z();
            if(m_debug){
          std::cout << "========================================================" << std::endl;
          std::cout << " !!!!!!!!!!!!!!! Pivot Candidate phi    !!!!!!!!!!!!!!!!!"<< std::endl;
          std::cout << phislope << " " << phiconst << std::endl;
          std::cout << lphimin[i]->chamber() << " " << lphimin[i]->plane() << " " ;
          std::cout << lphimin[i]->firstStrip() << " " << lphimin[i]->size() << std::endl;
          std::cout << lphimax[j]->chamber() << " " << lphimax[j]->plane() << " " ;
          std::cout << lphimax[j]->firstStrip() << " " << lphimax[j]->size() << std::endl; 
          std::cout << "========================================================" << std::endl;
             }   
// estrapolates  to each remaining hit
// and make a list of the nearest hits for each chamber plane
          std::vector<RPCCluster *> phiBest;
          phiBest=getBestList(chamClusList,phislope,phiconst,2);
          if(phiBest.size() < 2)continue;
          
          
          for(int ie=0; ie!=etasizemin; ie++) {
            for(int je=0; je!=etasizemax; je++) {
              float etaslope=(letamax[je]->X()-letamin[ie]->X() )/
                             (letamax[je]->Y()-letamin[ie]->Y() );
              float etaconst=-etaslope*letamax[je]->Y()+letamax[je]->X();
if(m_debug){
          std::cout << "========================================================" << std::endl;
          std::cout << " !!!!!!!!!!!!!!! Pivot Candidate eta    !!!!!!!!!!!!!!!!!"<< std::endl;
          std::cout << etaslope << " " << etaconst << std::endl;
          std::cout << letamin[ie]->chamber() << " " << letamin[ie]->plane() << " " ;
          std::cout << letamin[ie]->firstStrip() << " " << letamin[ie]->size() << std::endl;
          std::cout << letamax[je]->chamber() << " " << letamax[je]->plane() << " " ;
          std::cout << letamax[je]->firstStrip() << " " << letamax[je]->size() << std::endl; 
          std::cout << "========================================================" << std::endl;
             }  
// estrapolates  to each remaining hit
// and make a list of the nearest hits for each chamber plane
              std::vector<RPCCluster *> etaBest;
              etaBest=getBestList(chamClusList,etaslope,etaconst,0);
              if(etaBest.size() < 2)continue;


// add pivot and phi and eta clusters in bestList  
              std::vector<RPCCluster *> bestList; 
              for(ih=phiBest.begin(); ih!=phiBest.end(); ih++)bestList.push_back(*ih); 
              bestList.push_back(lphimin[i]);
              bestList.push_back(lphimax[j]);
              for(ih=etaBest.begin(); ih!=etaBest.end(); ih++)bestList.push_back(*ih);
              bestList.push_back(letamin[ie]);
              bestList.push_back(letamax[je]);
      
// here we give the hit list to the tracking object         
              RPCTrack  *NuovaTraccia = new RPCTrack(bestList);
// and fit    
              NuovaTraccia->fitTrack();
// make selelction on tracks  
              if(NuovaTraccia->chisquareX()>chisquamaX || 
                 NuovaTraccia->chisquareZ()>chisquamaZ || 
                 NuovaTraccia->nHits(0) < 4            ||
                 NuovaTraccia->nHits(2) < 4                 ) {
                if(m_debug){
                  std::cout << "RPCReconstructor::RPCX5PatRec: bad Chi^2 for track " << std::endl;
                  std::cout << NuovaTraccia->chisquareX() << " " ; 
                  std::cout << NuovaTraccia->chisquareZ() << " " ;
                  std::cout << NuovaTraccia->nHits(0)     << " " ;
                  std::cout << NuovaTraccia->nHits(2)     << std::endl;
                }
// here we should delete the track 
                delete NuovaTraccia;   
              } else {                         
// store it in TrackList and print
                if(m_debug)NuovaTraccia->print();
                TrackList.push_back(NuovaTraccia);
              }
           } // end loop on max eta               
         } // end loop min eta     
       } // end loop on max phi              
      } // end loop on min phi
      
   if(m_debug)std::cout << "RPCReconstructor: TrackList size= " << TrackList.size() << std::endl;   
   if(TrackList.size()>1){
     std::cout << std::endl << "RPCPatRec2: event "<< evh->eventNumber() << std::endl;
     std::cout << "RPCReconstructor: TrackList size= " << TrackList.size() << std::endl;
     std::vector<RPCTrack *>::iterator jt,Tbest;
     int nh=0;
     for (jt=TrackList.begin();jt!=TrackList.end();jt++){
       std::cout << (*jt)->chisquareX() << " " ; 
                  std::cout << (*jt)->chisquareZ() << " " ;
                  std::cout << (*jt)->nHits(0)     << " " ;
                  std::cout << (*jt)->nHits(2)     << std::endl;
       if(  (*jt)->nHits()  > nh){
         nh=(*jt)->nHits();
         Tbest=jt;
       }        
                  
     }
   }
// here we should choose best track if needed and return the list of tracks
   return TrackList;  
*/
}

/**************************************************************************/   
void RPCReconstructor::getEquation(double z1, double z2, double y1, double y2,
                                   double * a, double * b) {
  // get equation of line passing through points z1,y1 & z2,y2
  // equation is z = a*y + b
  *a = (z1-z2)/(y1-y2);
  *b = z2-*a*y2;

}

/**************************************************************************/
std::vector<RPCTrack *> RPCReconstructor::RPCPatRec(std::vector<RPCCluster *> NoiseList) {

   std::vector<RPCTrack *> TrackList;
   std::vector<RPCCluster *>::iterator ih;   
   

   
// check if there are enough hits in the list 
   if(NoiseList.size() < m_minhits    ) return TrackList;
// too many hits
   if(NoiseList.size() > m_maxhits ) return TrackList;  
   
   std::cout << "Comincio Pattern Recognition con noise! " << std::endl;   

   RPCTrack  *NoiseTraccia = new RPCTrack(NoiseList); 
   NoiseTraccia->fitTrack();
   std::cout << "========================================================" << std::endl;
   std::cout << " !!!!!!!!!!!!!!!Noise track !!!!!!!!!!!!!!!!!!!!!!!!!!! " << std::endl;
   NoiseTraccia->print();
   std::cout << "========================================================" << std::endl;
   if(NoiseTraccia->chisquareX()>chisquamaX || 
   NoiseTraccia->chisquareZ()>chisquamaZ) {
        return TrackList;
   }	           
// and store it in TrackList
   TrackList.push_back(NoiseTraccia);
   NoiseTraccia->fill(5000); 
     std::cout << " !!Noise track inside tracklist !! " << TrackList.size() << std::endl;                 
   return TrackList;
}


std::vector<RPCCluster *> RPCReconstructor::getBestList(std::vector<RPCCluster *> theList,float a,float b, int dir){

   std::vector<RPCCluster *> bestList;
   double sestr,coord,resid,coordother;
   double mindis[100],errmin[100];
   int iadd[100];

   for (int ii=0;ii<100;ii++){
     iadd[ii]=-1;
     mindis[ii]=99999.0;
     errmin[ii]=99999.0;
   }   
// first search for nearest cluster to extrapolation  
   for(int ii=0; ii!=theList.size(); ii++) {
      RPCCluster *cl=theList[ii];  
      if( cl->stripDir() != dir)continue;   
// compute estrapolation for this cluster
      double estr = a*cl->Y()+b;
// compute cluster coordinate
      if(dir==0)coord=cl->X();
      else if(dir==2)coord=cl->Z();
      if(dir==0)coordother=cl->Z();
      else if(dir==2)coordother=cl->X();
// compute residual
     resid =fabs(coord - estr);
// now should make an index for each plane
      int Ch = cl->chamber();
      int Pl = cl->plane();
// vey bad ...... 
      int idpoi = (Ch-1)*m_numpc+Pl;
      if(Ch == 0 )idpoi=m_numplnotr+Pl-1;


      
//      std::cout << " coord, estrap coord, err "<<  coord << " " << estr << " " << sestr << std::endl;
//      std::cout << " Resid: " << resid << " Chamber,Plane " << Ch << "," << Pl << std::endl;

// here we find the nearest cluster for each plane ad store the index in the iadd array         
      if(resid <= mindis[idpoi] ){
        mindis[idpoi]=resid;
// use cluster error
	errmin[idpoi]=cl->Err();  
	iadd[idpoi]=ii;
      } 
      
   } // end loop on theList
// get from input .....   
   float factor=4;	
// now store nearest hits in bestList, if survives cut 
   for(int jh=0; jh!=100; jh++) {
     if(iadd[jh] != -1 && mindis[jh] < factor*errmin[jh]){
//       std::cout << "added: " << (theList[iadd[jh]])->chamber() 
//                  << "," << (theList[iadd[jh]])->plane() 
//                  << "," << (theList[iadd[jh]])->firstStrip()
//                  << "," << (theList[iadd[jh]])->size() << std::endl;
        bestList.push_back(theList[iadd[jh]]);	
     }   
   }
   return bestList;   
}
std::vector<RPCCluster *> RPCReconstructor::getBestList(std::vector<RPCCluster *> theList,float ax,float bx, float az, float bz){

   std::vector<RPCCluster *> bestList;
   double sestr,coord,resid,coordother;
   double mindis[200],errmin[200];
   int iadd[200];

   for (int ii=0;ii<200;ii++){
     iadd[ii]=-1;
     mindis[ii]=99999.0;
     errmin[ii]=99999.0;
   }   
// first search for nearest cluster to extrapolation  
   double estr;
   for(int ii=0; ii!=theList.size(); ii++) {
      RPCCluster *cl=theList[ii];  
      if(cl->stripDir() == 0) estr = ax*cl->Y()+bx;
      else estr = az*cl->Y()+bz;   
      if(cl->stripDir() == 0)coord=cl->X();
      else coord=cl->Z();
// compute residual
     resid =fabs(coord - estr);
// now should make an index for each plane
      int Ch = cl->chamber();
      int Pl = cl->plane();
// vey bad ...... 
      int idpoi = (Ch-1)*m_numpc+Pl;
      if(m_debug){
        std::cout << " coord, estrap coord, err "<<  coord << " " << estr << " " << sestr << std::endl;
        std::cout << " Resid: " << resid << " Chamber,Plane " << Ch << "," << Pl << std::endl;
      }
// here we find the nearest cluster for each plane ad store the index in the iadd array         
      if(resid <= mindis[idpoi] ){
        mindis[idpoi]=resid;
// use cluster error
	    errmin[idpoi]=cl->Err();  
	    iadd[idpoi]=ii;
      } 
      
   } // end loop on theList
// get from input .....   
   float factor=3;	
// now store nearest hits in bestList, if survives cut 
   for(int jh=0; jh!=200; jh++) {
     if(iadd[jh] != -1 && mindis[jh] < factor*errmin[jh]){
       if(m_debug){
         std::cout << "added cluster: " << std::endl; 
                   (theList[iadd[jh]])->print();
                
       }
       bestList.push_back(theList[iadd[jh]]);	
     }   
   }
   return bestList;   
}
