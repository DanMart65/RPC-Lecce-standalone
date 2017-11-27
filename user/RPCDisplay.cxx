#include "user/RPCDisplay.h"
#include "control/CalibrationManager.h"
#include "user/RPCTBAnalize.h"
#include "event/EventHandler.h"
#include "event/Handler.h"
#include "event/RPCLETSHeader.h"
#include "event/RPCLETSEvent.h"
#include "detector/RPCStripPlane.h"
#include "detector/RPCChamber.h"
#include "detector/RPCCluster.h"
#include "recon/RPCTrack.h"
#include "database/GeometryHandler.h" 
#include "database/GeoRPCStripPlane.h"
#include "database/GeoRPCChamber.h"
#include "database/GeoRPCStrip.h"
#include "TVector3.h"
#include "TBRIK.h"
#include "TNode.h"
#include "TPad.h"
#include "TApplication.h"  
#include "tools/MapHandler.h"
#include "tools/Timer.h"
/***********************************************************************************
 *  Constructor, Reads the datacards if any
 ***********************************************************************************/
RPCDisplay::RPCDisplay():CalibrationTask()
{
   MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
   Memory->increase();

  // look for datacards if any
   std::ifstream optFile("input/RPCDisplay.datacards");
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
        if (strncmp(option,"NumEvents",9)==0 ){	  
            optFile >> n;
            int m_numevents=n;
        } else {
          std::cout << "RPCDisplay:: option not found !! ";
        }
      }
      userdef=true;
   }
   
// get info from CalibrationManager;   
   CalibrationManager *cm=CalibrationManager::getCalibrationManager();
// get debug level
   m_debug= cm->Debug();
   if(m_debug > 0 ) {
     std::cout << "RPCDisplay:: task initialized with ";
     if(!userdef) {
         std::cout << "DEFAULT ";
     } else {
         std::cout << "USER DEFINED " ;
     }
     std::cout << "options " << std::endl;
     
     std::cout << std::endl;
   }
}
/***********************************************************************************
 *  Destructor
 **********************************************************************************/
RPCDisplay::~RPCDisplay()
{
   MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
   Memory->decrease();
}
/***********************************************************************************
 *  Init: book Histograms and create RPCEffi objects
 ***********************************************************************************/
void RPCDisplay::taskInit() 
{
//
  if(m_debug>0) std::cout << "RPCDisplay: taskInit \n";
//   char *aa;
//   int ii;
//   TApplication theApp();
// event Handler and list of subEvents
  EventHandler * evh =EventHandler::getEventHandler();
  std::vector<SubEvent*> list = evh->subEvents();
// get geometry Handler  
  GeometryHandler* GeoPoi=GeometryHandler::getGeometryHandler();     

  MapHandler<Timer>  &htt = MapHandler<Timer>::getHandle();
  htt["RPCDisplay"]=new Timer();

// get Trigger Type   
  Handler<RPCLETSHeader>  &hdh = Handler<RPCLETSHeader>::getHandle(); 
  RPCLETSHeader * head=hdh[hdh.size()-1];
  int TrigType = head->TriggerType();
  if(m_debug>0) std::cout << "RPCDisplay: before new Tcanvas \n";
  if (!gApplication)TApplication *theApp=new TApplication("Application",0, 0);
  if(m_debug>0) std::cout << "RPCDisplay2: before new Tcanvas \n";
  theWindow = new TCanvas("theWindow","Lecce RPC Test Stand",0,0,900,900);
  if(m_debug>0) std::cout << "RPCDisplay: after new Tcanvas" << theWindow<< " \n"; 
  theWindow->SetFillColor(33);
}
/***********************************************************************************
 *  End
 ***********************************************************************************/
bool RPCDisplay::taskEnd() 
{
//  
  MapHandler<Timer>  &htt = MapHandler<Timer>::getHandle();     
  std::cout << std::endl << "RPCDisplay execution Time: " 
            << htt["RPCDisplay"]->Time() << std::endl;
            
  if(m_debug>0) std::cout << "RPCDisplay: taskEnd "<< std::endl;
      
// get Trigger Type
  Handler<RPCLETSHeader>  &hdh = Handler<RPCLETSHeader>::getHandle(); 
  RPCLETSHeader * head=hdh[hdh.size()-1];
  int TrigType = head->TriggerType();
  return true;
}
/***********************************************************************************
 *
 ***********************************************************************************/
void RPCDisplay::runInit() 
{
  Handler<RPCLETSHeader>  &hdh = Handler<RPCLETSHeader>::getHandle(); 
  RPCLETSHeader * head=hdh[hdh.size()-1];
  int TrigType = head->TriggerType();
   if(m_debug>0)std::cout <<"RPCDisplay: RunInit run number "<< head->runNumber()<< std::endl; 
   
}
/***********************************************************************************
 *
 ***********************************************************************************/
void RPCDisplay::runEnd() 
{
   if(m_debug>0) std::cout << "RPCDisplay: RunEnd "<< std::endl;
}
/***********************************************************************************
 * Event Processing
 ***********************************************************************************/
void RPCDisplay::evtProc()
{
  MapHandler<Timer>  &htt = MapHandler<Timer>::getHandle(); 
  htt["RPCDisplay"]->Start();
// get various Handlers
  GeometryHandler *GeoPoi=GeometryHandler::getGeometryHandler();
  EventHandler * evh =EventHandler::getEventHandler();   
  int event = evh->eventNumber();
  
  Handler<RPCLETSHeader>  &hdh = Handler<RPCLETSHeader>::getHandle(); 
  RPCLETSHeader * head=hdh[hdh.size()-1];
  int TrigType = head->TriggerType();
  int run = head->runNumber();
  if(m_debug>0) std::cout << std::endl << "RPCDisplay: EvtProc event "<< event << std::endl;                             
// Draw strips  
  DrawStrips();
  
  // stop Timer
  htt["RPCDisplay"]->Stop();
} 
/***********************************************************************************
 * Draw Strips
 ***********************************************************************************/

void RPCDisplay::DrawStrips(){

  theWindow->Clear();
  theWindow->Range(0, 0, 4000, 4000);
  
  TPad *phipad = new TPad("pr","pr",  0, 0 , 1, 0.5);
  phipad->Range(0, 0, 4000, 3500);
  phipad->Draw();  
  
  
  TPad *etapadl = new TPad("pr","pr", 0, 0.5 , 0.5, 1.0);
  etapadl->Range(0, 0, 1000, 3500);
  etapadl->Draw();
 
  TPad *etapadr = new TPad("pr","pr", 0.5, 0.5 , 1.0, 1.0);
  etapadr->Range(0, 0, 1000, 3500);
  etapadr->Draw();
    
    
    
  EventHandler * evh =EventHandler::getEventHandler();
  std::vector<RPCChamber*> chambers=evh->GetRPCChambers();   
  std::vector<RPCChamber*>::iterator it;
  
  TLine  *line;
          
  for(it=chambers.begin();it!=chambers.end(); it++) {    
    for(int ip=0;ip!=(*it)->nStripPlanes(); ip++) {
      RPCStripPlane * sp=(*it)->stripplanePtr(ip);
// get pointer to geometry      
      GeoRPCStripPlane * geo=sp->RPCgeometry();
      double high=geo->highlimit();
      double low=geo->lowlimit();
      int dir=geo->StripDir();
      int plane=geo->StripPlane();
      TVector3 r=geo->StripCoord(1);
      double y = r.Y();  
// draw the plane      
      if(dir==2){
            phipad->cd();
            line = new TLine(low,y,high,y);
            line->SetLineWidth(0.5); 
            line->SetLineColor(1); 
            line->Draw();
      }
      if(dir==0){
            if(plane <=4)etapadl->cd();
            else etapadr->cd();
            line = new TLine(low,y,high,y);
            line->SetLineWidth(0.5); 
            line->SetLineColor(1); 
            line->Draw();
      }
// get list of clusters in this plane
      std::vector<RPCCluster*> ClusList = sp->clusList();   
// loop on clusters   
      std::vector<RPCCluster *>::iterator  ic;
      for(ic=ClusList.begin(); ic!=ClusList.end();ic++){
 // get list of strip in this cluster 
        std::vector<RPCStrip *> stripList=(*ic)->stripPtr();          
// loop on the list of strip in that plane         
        std::vector<RPCStrip*>::iterator is;
        for(is=stripList.begin(); is!=stripList.end(); is++) {
// get pointer to geometry description of that strip
          GeoRPCStrip * g=(*is)->geo();
// get geometry description           
          double halfwid=g->StripWidth()/2.0;
          TVector3 r=g->xyzStrip();
          double x = r.X();
          double z = r.Z();     

          if(m_debug>0)g->printOut();      

// here draw for each strip
          if(dir==2){
            phipad->cd();
            line = new TLine(z-halfwid+4.0,y,z+halfwid-4.0,y);
            line->SetLineWidth(3); 
            line->SetLineColor(2); 
            line->Draw();
          }
          if(dir==0){
            if(plane <=4)etapadl->cd();
            else etapadr->cd();
            line = new TLine(x-halfwid+2.0,y,x+halfwid-2.0,y);
            line->SetLineWidth(3); 
            line->SetLineColor(2); 
            line->Draw();
          }
        }
      }
    }
  }
//
// draw the track
//  
// get CalibrationManager singleton;   
   CalibrationManager *cm=CalibrationManager::getCalibrationManager();
// get RPCTBAnalize pointer
   RPCTBAnalize *a= (RPCTBAnalize *)cm->getTaskPtr("RPCTBAnalize");
// get the tracks stored in RPCTBAnalize
   std::vector<RPCTrack *> tkCandidates=a->tracks();
   std::vector<RPCTrack *>::iterator ik;    
   if(m_debug)for(ik=tkCandidates.begin(); ik!=tkCandidates.end();ik++)  
    std::cout << "RPCDisplay: #hits in track: " << (*ik)->nHits(0) << " " << 
                                                                  (*ik)->nHits(2) << std::endl;  
  
   double sigestrap,z1,z2;  
   for(ik=tkCandidates.begin(); ik!=tkCandidates.end();ik++){
            phipad->cd();
            z1=(*ik)->estrapPlane(2,0.0, sigestrap);
            z2=(*ik)->estrapPlane(2,3500.0, sigestrap);
            line = new TLine(z1,0,z2,3500);
            line->SetLineWidth(1); 
            line->SetLineColor(4); 
            line->Draw();
          
            etapadl->cd();
            z1=(*ik)->estrapPlane(0,0.0, sigestrap);
            z2=(*ik)->estrapPlane(0,3500.0, sigestrap);
            line = new TLine(z1,0,z2,3500);
            line->SetLineWidth(1); 
            line->SetLineColor(4); 
            line->Draw();
            
            etapadr->cd();
            line->Draw();           
// now draw hits belonging to track
            std::vector<RPCCluster *>::iterator ih;
            std::vector<RPCCluster *> cluslist=(*ik)->hitList();
            for( ih=cluslist.begin(); ih!=cluslist.end(); ih++){
              int dir=(*ih)->stripDir();
               // get list of strip in this cluster 
              std::vector<RPCStrip *> stripList=(*ih)->stripPtr();          
// loop on the list of strip in that plane         
              std::vector<RPCStrip*>::iterator is;
              for(is=stripList.begin(); is!=stripList.end(); is++) {
              // get pointer to geometry description of that strip
                GeoRPCStrip * g=(*is)->geo();
// get geometry description           
                int plane=g->StripPlane();
                double halfwid=g->StripWidth()/2.0;
                TVector3 r=g->xyzStrip();
                double x = r.X();
                double y =r.Y();
                double z = r.Z();
              
                if(dir==2){
                  phipad->cd();
                  line = new TLine(z-halfwid+4.0,y,z+halfwid-4.0,y);
                  line->SetLineWidth(3); 
                  line->SetLineColor(6); 
                  line->Draw();
                }
                if(dir==0){
                  if(plane <=4)etapadl->cd();
                  else etapadr->cd();
                  line = new TLine(x-halfwid+2.0,y,x+halfwid-2.0,y);
                  line->SetLineWidth(3); 
                  line->SetLineColor(6); 
                  line->Draw();
                }
              }
            }
   }
  
  
// Draw this geometry in the current canvas
   theWindow->Update();
// 
  int num;
  std::cout << "enter:";
  std::cin >> num;    
}     

/*
void RPCDisplay::DrawStrips(){

  theWindow->Clear();
  std::vector<TBRIK*> strips;
  std::vector<TNode*> nodes;
  TBRIK *system = new TBRIK("SYST","SYST","void",2000,2000,2000);
  TNode * origin= new TNode("SYST","SYST",system,0,0,0,0);
  origin->SetVisibility(0);
  origin->cd();

  EventHandler * evh =EventHandler::getEventHandler();
  std::vector<RPCChamber*> chambers=evh->GetRPCChambers();   
  std::vector<RPCChamber*>::iterator it;
  for(it=chambers.begin();it!=chambers.end(); it++) {    
    for(int ip=0;ip!=(*it)->nStripPlanes(); ip++) {
      RPCStripPlane * sp=(*it)->stripplanePtr(ip);
// print plane information
////////       sp->print();
      // list of all active strips in the plane
       std::vector<RPCStrip*> stripList=sp->hitList(); 
       std::vector<RPCStrip*>::iterator is;          
// loop on the list of strip in that plane         
       for(is=stripList.begin(); is!=stripList.end(); is++) {
// get pointer to geometry description of that strip
         GeoRPCStrip * g=(*is)->geo();
// get geometry description           
         double halflen=g->StripLen()/2.0;
         double halfwid=g->StripWidth()/2.0;
         HepPoint3D r=g->xyzStrip();
         int dir=g->StripDir();
         double x = r.x();
         double y = r.y();
         double z = r.z();     

         if(m_debug>0)g->printOut();      

// here define an object for each strip
         TBRIK *ob;
         if(dir==0)ob = new TBRIK("strip","strip","void",halfwid,0.1,halflen);
         else      ob = new TBRIK("strip","strip","void",halflen,0.1,halfwid);
//  Set shapes attributes
         ob->SetLineColor(2+dir);  
         strips.push_back(ob);
         TNode *no = new TNode("NODE","NODE",ob,x,y,z);
// necessario ?
         nodes.push_back(no);

       }
    }
  }
  
// Draw this geometry in the current canvas
   origin->cd();
   origin->Draw();
   
   theWindow->Update();
   theWindow->x3d();
//
  int num;
  std::cout << "enter:";
  std::cin >> num;  
  
}     
*/
