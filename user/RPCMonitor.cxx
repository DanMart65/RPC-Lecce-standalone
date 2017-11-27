#include "user/RPCMonitor.h"
#include "event/EventHandler.h"
#include "event/Handler.h"
#include "event/RPCLETSHeader.h"
#include "event/Header.h"
#include "tools/Utils.h"
#include "detector/RPCStripPlane.h"
#include "detector/RPCCluster.h"
#include "database/GeometryHandler.h" 
#include "database/GeoRPCStripPlane.h"
#include "his/HisFile.h" 
#include "control/CalibrationManager.h"
#include "tools/MapHandler.h"
#include "tools/Timer.h"
#include "TFile.h"
#include "TH1.h"
#include "TNtuple.h"
//gabriele#include <algo.h> // for sort
/***********************************************************************************
 *  Constructor, Reads the datacards if any
 ***********************************************************************************/
RPCMonitor::RPCMonitor(std::string name):CalibrationTask(),m_call(0),m_debug(0),m_maxdebug(0)
{
   MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
   Memory->increase();

  m_name=name;
  // look for datacards if any
   std::ifstream optFile("input/RPCMonitor.datacards");
   bool userdef=false;
   
   if(optFile) {
      char option[11];
      char chComm[]   = "#";
      int n,i;
      double f;             
      while( optFile >> option) {
        while (strncmp(option,chComm,1)==0) {
          optFile.ignore(500,'\n');
          optFile>>option ;
        }
        if (strncmp(option,"End",3)==0 ){
          break;
        } else if (strncmp(option,"ndebug",6)==0 ){   
            optFile >> n;
            m_maxdebug=n;
        } else {
          std::cout << "RPCMonitor:: option not found: " << option << "\n";
        }
        optFile.ignore(500,'\n');
      }
      userdef=true;
   }
   
   CalibrationManager *cm=CalibrationManager::getCalibrationManager();
// get debug level
   m_debug= cm->Debug();
   
   if(m_debug > 0 ) {
     std::cout << "RPCMonitor task initialized with ";
     if(!userdef) {
         std::cout << "DEFAULT ";
     } else {
         std::cout << "USER DEFINED " ;
     }
     std::cout << "options " << std::endl;
     
     std::cout << "Debug Level: " << m_debug <<std::endl;
     std::cout << "# of events to debug: " << m_maxdebug <<std::endl;
     std::cout << std::endl;
   }
}
/***********************************************************************************
 *  Destructor
 **********************************************************************************/
RPCMonitor::~RPCMonitor()
{
   MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
   Memory->decrease();
}
/***********************************************************************************
 *  Init: book Histograms
 ***********************************************************************************/
void RPCMonitor::taskInit() 
{
// create new timers
   MapHandler<Timer>  &htt = MapHandler<Timer>::getHandle();
   htt["RPCMonitor"]=new Timer();

// event Handler and list of subEvents, 
   EventHandler * evh =EventHandler::getEventHandler(); 
   std::vector<SubEvent*> list = evh->subEvents();  
// get geometry Handler  
   GeometryHandler* GeoPoi=GeometryHandler::getGeometryHandler();
// Histogram & Root File handler
//   HisFile* hf = HisFile::getHisFile();
   HisFile *rootFile = HisFile::getHisFile(3);
   TNtuple* ntuple;
    
// get Trigger Type 
   Handler<RPCLETSHeader>  &hdh = Handler<RPCLETSHeader>::getHandle(); 
   RPCLETSHeader * head=hdh[hdh.size()-1];
   int TrigType = head->TriggerType();
   
   // skip for DCSTrigger
   if(TrigType==12)return;
  
   if(m_debug>0) std::cout << "RPCMonitor::taskInit, Trigger Type "<< TrigType << std::endl;
       
// get Number of StripPlanes     
   int nsplanes= GeoPoi->nStripPlane();
     
 //  time global histogram     
//   hf->His2D(3333,"strip distance vs time diff. in cluster",6,0.,90.,10,0.,10.);
//   hf->His1D(2,"arrival time",32,0,480.);
//  time diff eta,phi
//   hf->His1D(3,"time diff",100,-200.,200.0);
//
// create radiography for noise Histograms, one per gap 
//
   std::vector<RPCChamber*> chambers = evh->GetRPCChambers();
   int  nphi,neta;
   RPCChamber *ch;
// get list of planes belonging to a gap     
   for(int jch=0; jch!=chambers.size();jch++) { 
     ch=chambers[jch];
     for(int ig=0; ig!=ch->nGaps();ig++) {          
       std::vector<RPCStripPlane *> gpl=ch->GapPlanes(ig+1);

       if(gpl[0]->RPCgeometry()->gap()==1 || gpl[0]->RPCgeometry()->gap()==3){
           nphi = gpl[0]->RPCgeometry()->Nstrips();
           neta = gpl[1]->RPCgeometry()->Nstrips();
       } else {
           nphi = gpl[1]->RPCgeometry()->Nstrips();
           neta = gpl[0]->RPCgeometry()->Nstrips(); 
       }

       std::string name=chambers[jch]->geo()->gapPtr(ig)->Name();

       // Histogram parameter (solo per le camere di trigger, per le camere sotto misura vengono raccolti i parametri
       // in RPCTBAnalize, perché sono necessari anche i limiti su phi ed eta)
       if(name.compare(7,1,"M")!=0) {
	       std::string tuplename = name+"-TUPLE";
	       std::string tupletitle = "Histogram Parameter for chamber: "+name;

	       ntuple = new TNtuple(tuplename.c_str(), tupletitle.c_str(), "nphi:neta");

	       ntuple->Fill(nphi,neta);
	       //ntuple->Scan();
	       //ntuple->Write();
       }

       // set histo address
       int hisadd=ch->Chamber()*10+ig+1;
       // Raw hitmap
       std::string hisname = name + " Raw Hitmap";
       char *htit=(char *)hisname.c_str();
//       hf->His2D(30000+hisadd,htit,nphi,1.0,(float)(nphi+1),neta,1.0,(float)(neta+1));    
       rootFile->createTree(name, "Raw Hitmap", m_debug);
     } // loop on gaps  
   } // loop on chambers

     
// book histos for time, hit multiplicity, profile and repetitions for hits
// cluster multiplicity, size and profile             
     for (int ns=0;ns<nsplanes;ns++){
// get the pointer to stripplane geometry
         GeoRPCStripPlane* gstptr=GeoPoi->stripPlanePtr(ns);
// number of strips
         int nums = gstptr->Nstrips();
// strip width   
         double sw = gstptr->StripWidth();
// plane Name
         std::string name = gstptr->Name();
// get idstripplane 
         int idsp = gstptr->idStripPlane();
// get plane limits
         double lowl=gstptr->lowlimit();
         double higl=gstptr->highlimit();
//
         int nlim=nums;
// cambiare a seconda del tipo di trigger >??         
         if(nlim > 16)nlim=nums/4;
//        
         std::string title=name+": hit time distribution";
         char *htitle={(char *)title.c_str()};
         // va bene per LETS...
//         hf->His1D(3000+idsp,htitle,32,0.0,480);
//         title=name+": hit multiplicity";
//         htitle=(char *)title.c_str();
//         hf->His1D(4000+idsp,htitle,nlim,-0.5,nlim-0.5);         
         

         title=name+": hit profile";
         htitle=(char *)title.c_str();
   rootFile->createTree(title, "Hit Profile", m_debug);      
//   hf->His1D(2000+idsp,htitle,nums,1.,nums+1);        
       

         title=name+": hit profile vs profile";
         htitle=(char *)title.c_str();
//   hf->His2D(100000+idsp,htitle,nums,1.,nums+1,nums,1.,nums+1);
         title=name+": hit repetitions";
         htitle=(char *)title.c_str();
//   hf->His1D(1600+idsp,htitle,16,0.5,16.5);
         title=name+": Time difference of hit repetitions";
         htitle=(char *)title.c_str();
//   hf->His1D(1400+idsp,htitle,32,0.0,480.0);
         title=name+": cluster multiplicity";
         htitle=(char *)title.c_str();        
//         hf->His1D(5000+idsp,htitle,nlim,-0.5,nlim-0.5);
         title=name+": cluster size";
         htitle=(char *)title.c_str();           
//         hf->His1D(6000+idsp,htitle,nlim,-0.5,nlim-0.5); 
     }
}
/***********************************************************************************
 *  End
 ***********************************************************************************/
bool RPCMonitor::taskEnd() 
{
  if(m_debug>0) std::cout << "RPCMonitor: taskEnd "<< std::endl;

// get Timer Handler 
  MapHandler<Timer>  &htt = MapHandler<Timer>::getHandle(); 
//        
// print Timing counters     
//
  std::cout << "\nRPCMonitor::taskEnd: execution Time: " 
            << htt["RPCMonitor"]->Time() << std::endl;      

    
  return true;
}
/***********************************************************************************
 *
 ***********************************************************************************/
void RPCMonitor::runInit() 
{
  Handler<RPCLETSHeader>  &hdh = Handler<RPCLETSHeader>::getHandle(); 
  RPCLETSHeader * head=hdh[hdh.size()-1];
  int TrigType = head->TriggerType();
  if(m_debug>0) 
      std::cout <<"RPCMonitor: RunInit run number "<< head->runNumber() << std::endl;
}
/***********************************************************************************
 *
 ***********************************************************************************/
void RPCMonitor::runEnd() 
{
   if(m_debug>0) std::cout << "RPCMonitor: RunEnd "<< std::endl;
}
/***********************************************************************************
 * Event Processing
 ***********************************************************************************/
void RPCMonitor::evtProc()
{
  MapHandler<Timer>  &htt = MapHandler<Timer>::getHandle(); 
  htt["RPCMonitor"]->Start();
// get various Handlers
  GeometryHandler *GeoPoi=GeometryHandler::getGeometryHandler();
  EventHandler * evh =EventHandler::getEventHandler(); 
  Handler<RPCLETSHeader>  &hdh = Handler<RPCLETSHeader>::getHandle(); 
  RPCLETSHeader * head=hdh[hdh.size()-1];
//  HisFile *hf = HisFile::getHisFile();
  HisFile *rootFile = HisFile::getHisFile(3);

  int event = evh->eventNumber();
  int TrigType = head->TriggerType();   
  
   // skip for DCSTrigger
   if(TrigType==12)return;
     
  int run = head->runNumber();
  
  if(m_debug>0) std::cout << std::endl << "RPCMonitor: EvtProc event "<< event << std::endl;                        

// fill some hit related histos (should be done with time cut ????
// no because these histos can be useful for single rate counting in random triggers
// yes but in random triggers one uses full window !!!!
  MakeHitHistos();
// make cluster histos
  MakeClusterHistos();
// stop Timer
  htt["RPCMonitor"]->Stop();
} 
/*************************************************************************
 * Makes some histograms for hits 
 *************************************************************************/
void RPCMonitor::MakeHitHistos(){
    EventHandler * evh =EventHandler::getEventHandler(); 
    Handler<RPCLETSHeader>  &hdh = Handler<RPCLETSHeader>::getHandle(); 
    RPCLETSHeader * head=hdh[hdh.size()-1];
    int TrigType = head->TriggerType();
 
     std::vector<RPCStripPlane*> plist = evh->GetStripPlanes();
     std::vector<RPCStripPlane*>::iterator it,it2; //Gabriele secondo loop su strip panel vs panel
     std::vector<RPCStrip*> stripList,stripList2; // list of all active strips in the plane//Gabriele secondo loop su strip panel vs panel
     std::vector<RPCStrip*>::iterator is,is2; //Gabriele secondo loop su strip panel vs panel
// 
//     HisFile *hf = HisFile::getHisFile();
     HisFile *rootFile = HisFile::getHisFile(3);
//     Histo *histo,*histo1;
//     histo1=hf->his(2);
    
     for(it=plist.begin();it!=plist.end(); it++) {
// get the strip list            
        stripList = (*it)->hitList();   
// get the StripPlane id
        GeoRPCStripPlane* gstptr=(*it)->RPCgeometry();
        int idsp = gstptr->idStripPlane();
        std::string stripname = gstptr->Name();
        std::string stripnameTree = stripname+": hit profile";
// number of hits per stripplane          
//        histo=hf->his(4000+idsp);

   //     if(stripList.size()>0 || TrigType==10) //histo->fill(stripList.size(),0.0,1.0);                        

// Make histo of number of duplicates           
        int olds=0;
        int numdup=1;
        int oldt=0;
// loop on the list of strip in that plane         
        for(is=stripList.begin(); is!=stripList.end(); is++) {
          int ss=(*is)->strip();
          int tt=(*is)->time();
// found a duplicated strip, count number of duplicates             
          if(olds == ss){
            numdup++;
//            histo=hf->his(1400+idsp);
//            histo->fill(oldt-tt,0.0,1.0);            
          } else if(olds !=0){
// Make histo of number of duplicates and reset           
//            histo=hf->his(1600+idsp);
//            histo->fill(numdup,0.0,1.0);
            numdup=1;
          }          
          olds=ss;
          oldt=tt;          
// correction for last strip
          if(is == (stripList.end()-1)){
//            histo=hf->his(1600+idsp);
//            histo->fill(numdup,0.0,1.0);
          }            
// Make histo for illumination one per plane...          
          rootFile->createBranch(stripnameTree, &ss, "hits", m_debug);
          rootFile->fillTree(stripnameTree);
//          histo=hf->his(2000+idsp);
//          histo->fill((*is)->strip(),0.0,1.0);             
// Make histo for illumination one per plane vs plane...            
//          histo=hf->his(100000+idsp);
          
    
//raw hit correlation plots   
//   for(it2=plist.begin();it2!=plist.end(); it2++) {
//           // get the strip list            
//           stripList2 = (*it2)->hitList();   
//           // get the StripPlane id
//           GeoRPCStripPlane* gstptr2=(*it2)->RPCgeometry();
//           int idsp2 = gstptr2->idStripPlane(); 
//    
//    // loop on the list of strip in that plane         
//           for(is2=stripList2.begin(); is2!=stripList2.end(); is2++) {
//    
//     //std::cout << "idsp idsp2 " << idsp << " " << idsp2 <<"  mod " <<  fmod(float(idsp),10) << " " <<fmod(float(idsp2),10) <<std::endl;
//     
//     //if(idsp>100&&(idsp2-idsp==10))histo->fill((*is)->strip(),(*is2)->strip(),1.0); 
//     //if(idsp<100&&(idsp2-idsp==100))histo->fill((*is)->strip(),(*is2)->strip(),1.0); 
//   
//     
//     //if(fmod(float(idsp),10)==1 && fmod(float(idsp2),10)==4 ) histo->fill((*is)->strip(),(*is2)->strip(),1.0);  
//     //if(fmod(float(idsp),10)==2 && fmod(float(idsp2),10)==3 ) histo->fill((*is)->strip(),(*is2)->strip(),1.0);    
//     //if(fmod(float(idsp),10)==5 && fmod(float(idsp2),10)==8 ) histo->fill((*is)->strip(),(*is2)->strip(),1.0);  
//     //if(fmod(float(idsp),10)==6 && fmod(float(idsp2),10)==7 ) histo->fill((*is)->strip(),(*is2)->strip(),1.0);   
//      
//     if(idsp==111 &&  idsp2==124 ) histo->fill((*is)->strip(),(*is2)->strip(),1.0); 
//     if(idsp==112 &&  idsp2==123 ) histo->fill((*is)->strip(),(*is2)->strip(),1.0); 
//     if(idsp==113 &&  idsp2==122 ) histo->fill((*is)->strip(),(*is2)->strip(),1.0); 
//     if(idsp==114 &&  idsp2==121 ) histo->fill((*is)->strip(),(*is2)->strip(),1.0);
//     
//     if(idsp==115 &&  idsp2==128 ) histo->fill((*is)->strip(),(*is2)->strip(),1.0); 
//     if(idsp==116 &&  idsp2==127 ) histo->fill((*is)->strip(),(*is2)->strip(),1.0); 
//     if(idsp==117 &&  idsp2==126 ) histo->fill((*is)->strip(),(*is2)->strip(),1.0); 
//     if(idsp==118 &&  idsp2==125 ) histo->fill((*is)->strip(),(*is2)->strip(),1.0);
//   
//   }}
    
    
    
    
// make histo for timing (single one).
//          histo1->fill((*is)->time(),0.0,1.0);
// time histos per stripplane            
//          histo=hf->his(3000+idsp);
//          histo->fill((*is)->time(),0.0,1.0);            
        }
       
     }
         
// now reloop on Stripplanes to plot the correlations between eta and phi 
// correlation is by gap!!!
    std::vector<RPCChamber*> chambers = evh->GetRPCChambers();
   GeometryHandler* GeoPoi=GeometryHandler::getGeometryHandler();   
    
//    Histo *histotime=hf->his(3);   
//    Histo *histoxy;
    RPCStripPlane *phi,*eta;
    RPCChamber *ch;
// get list of planes belonging to a gap     
    for(int jch=0; jch!=chambers.size();jch++) { 
      ch=chambers[jch];
      for(int ig=0; ig!=ch->nGaps();ig++) { 
         std::vector<RPCStripPlane *> gpl=ch->GapPlanes(ig+1);
         std::string name=chambers[jch]->geo()->gapPtr(ig)->Name();
         if(gpl[0]->RPCgeometry()->gap()==1 || gpl[0]->RPCgeometry()->gap()==3){
           phi = gpl[0];
           eta = gpl[1];
         } else {
           phi = gpl[1];
           eta = gpl[0];  
         }
         int hisadd=ch->Chamber()*10+ig+1;
//         histoxy=hf->his(30000+hisadd);
         // loop on strips of this planes
         std::vector<RPCStrip*> spphi=phi->hitList();
         std::vector<RPCStrip*> speta=eta->hitList();
         std::vector<RPCStrip*>::iterator is1,is2; 
         int st1;
         int st2;
         rootFile->createBranch(name, &st1, "phiRawHitmap", m_debug);
         rootFile->createBranch(name, &st2, "etaRawHitmap", m_debug);
         for(is1=spphi.begin(); is1!=spphi.end(); is1++) {
           st1 = (*is1)->strip();
           int tt1 = (*is1)->time();

           for(is2=speta.begin(); is2!=speta.end(); is2++) {
             st2 = (*is2)->strip();
             int tt2 = (*is2)->time();  
             int diff=tt1-tt2;             
//             histotime->fill(diff,0.0,1.0); 
// read this cut from cards, please                         
             if(diff >= -30 && diff <=30)
             {
//              histoxy->fill(st1,st2,1.0);
              rootFile->fillTree(name);
             }
           }
         }

      } // loop on gaps  
    } // loop on chambers


}
/*************************************************************************
 * Makes some histograms for clusters
 *************************************************************************/
void RPCMonitor::MakeClusterHistos(){
 /* HisFile *hf = HisFile::getHisFile();
 Histo *histo,*histotim;
// timing histo
 histotim = hf->his(3333); 
// get list of stripplanes in event and loop on it
 EventHandler * evh =EventHandler::getEventHandler(); 
 std::vector<RPCStripPlane*> plist = evh->GetStripPlanes();         
 for(std::vector<RPCStripPlane*>::iterator it=plist.begin();it!=plist.end(); it++) { 
// get chamber id
   GeoRPCStripPlane* gstptr=(*it)->RPCgeometry();
   int idsp = gstptr->idStripPlane();  

// get list of clusters in this plane
   std::vector<RPCCluster*> ClusList = (*it)->clusList();   
   std::vector<RPCCluster *>::iterator  ic;

// fill cluster multiplicity per plane      
   histo = hf->his(5000+idsp);
   if(ClusList.size()>0)histo->fill(ClusList.size(),0.,1.);

// loop on clusters   
   for(ic=ClusList.begin(); ic!=ClusList.end();++ic){ 
// fill cluster size per plane     
     histo = hf->his(6000+idsp);
     histo->fill((*ic)->size(),0.,1.); 
   
// make some histos for timing
// get list of strip in this cluster 
     std::vector<RPCStrip *> striplist=(*ic)->stripPtr();
// sort strips decreasing with time 
     TimeSort ts;
     sort(striplist.begin(),striplist.end(),ts);   
     int start=striplist[0]->time();
     int st=striplist[0]->strip();
// fill timing histos     
     for(int is=1; is!=striplist.size();is++)
            histotim->fill(start-striplist[is]->time(),abs(striplist[is]->strip()-st),1.);
                        
   }
 }      
 
     */
}

