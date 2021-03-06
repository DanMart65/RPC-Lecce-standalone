#include "recon/RPCReconstructor.h"
#include "user/RPCTBAnalize.h"
#include "event/EventHandler.h"
#include "event/Handler.h"
#include "event/RPCLETSHeader.h"
#include "event/Header.h"
#include "tools/Utils.h"
#include "detector/RPCStripPlane.h"
#include "detector/RPCCluster.h"
#include "recon/RPCTrack.h"
#include "database/DeadHotHandler.h"
#include "database/GeometryHandler.h" 
#include "database/GeoRPCStripPlane.h"
#include "database/GeoRPCChamber.h"
#include "database/GeoRPCStrip.h"
#include "his/HisFile.h" 
#include "control/CalibrationManager.h"
#include "TVector3.h"
#include "tools/MapHandler.h"
#include "tools/Timer.h"
#include <math.h> // for exp
#include <algorithm>
#include <sstream>
#include "TNtuple.h"
/***********************************************************************************
 *  Constructor, Reads the datacards if any
 ***********************************************************************************/
RPCTBAnalize::RPCTBAnalize(std::string name):CalibrationTask(),m_call(0),m_debug(0),m_maxdebug(0),
                                             m_minhits(6),m_maxhits(1000),m_maxdump(0),
                                             m_timemin(0),m_timemax(2048),m_stripwindow(0),
                     m_hitsforeffi(6),m_allchambers(false)
{
   MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
   Memory->increase();
 std::cout << " RPCTBAnalize::RPCTBAnalize " <<  "\n";
  m_name=name;
  // look for datacards if any
   std::ifstream optFile("input/RPCTBAnalize.datacards");
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
      std::cout << "ndebug: " << n << "\n";
        } else if (strncmp(option,"recodump",8)==0 ){   
            optFile >> n;
            m_maxdump=n;
      std::cout << "recodump: " << n << "\n";
        } else if (strncmp(option,"minhits",7)==0 ){    
            optFile >> n;
            m_minhits=n;
      std::cout << "minhits: " << n << "\n";
        } else if (strncmp(option,"minradhits",10)==0 ){    
            optFile >> n;
      std::cout << "minradhits " << n << "\n";
        } else if (strncmp(option,"maxhits",7)==0 ){    
            optFile >> n;
            m_maxhits=n;
      std::cout << "maxhits: " << n << "\n";
  } else if (strncmp(option,"hitsforeffi",11)==0 ){   
            optFile >> n;
      m_hitsforeffi=n;
      std::cout << "hitsforeffi: " << n << "\n";
  } else if (strncmp(option,"allchambers",11)==0 ){   
            m_allchambers=true; 
      std::cout << "allchambers: " << m_allchambers << "\n";      
        } else if (strncmp(option,"stripwindow",11)==0 ){   
            optFile >> n;
            m_stripwindow=n;   
        } else {
          std::cout << "RPCTBAnalize:: option not found: " << option << "\n";
        }
        optFile.ignore(500,'\n');
      }
      userdef=true;
   }
// check m_hitsforeffi >= minhits
   if( m_hitsforeffi < m_minhits ){
     std::cout << "\nRPCTBAnalize::  WARNING, WARNING  hitsforeffi < minhits\n\n";
   }   
   
// get info from CalibrationManager;   
   CalibrationManager *cm=CalibrationManager::getCalibrationManager();
   m_timemin=cm->timeMin();
   m_timemax=cm->timeMax();

// get debug level
   m_debug= cm->Debug();
   
   if(m_debug > 0 ) {
     std::cout << "RPCTBAnalize task initialized with ";
     if(!userdef) {
         std::cout << "DEFAULT ";
     } else {
         std::cout << "USER DEFINED " ;
     }
     std::cout << "options " << std::endl;
     
     std::cout << "Debug Level: " << m_debug <<std::endl;
     std::cout << "# of events to dump: " << m_maxdump <<std::endl;
     std::cout << "Time min: " << m_timemin << ", Time max: " << m_timemax << std::endl;
     std::cout << "Min Hits: " << m_minhits << ",  Max Hits: " << m_maxhits << std::endl;
     std::cout << "Efficiency strip Window: " << m_stripwindow  << std::endl;

     std::cout << std::endl;
   }
}
/***********************************************************************************
 *  Destructor
 **********************************************************************************/
RPCTBAnalize::~RPCTBAnalize()
{
   MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
   Memory->decrease();
}
/***********************************************************************************
 *  Init: book Histograms
 ***********************************************************************************/
void RPCTBAnalize::taskInit() 
{
// create new timers
   MapHandler<Timer>  &htt = MapHandler<Timer>::getHandle();
   htt["RPCTBAnalize"]=new Timer();
   htt["RPCReconstructor"]=new Timer();
   htt["loop on tracks"]=new Timer(); 
   htt["search for hit"]=new Timer(); 
// event Handler and list of subEvents, 
  EventHandler * evh =EventHandler::getEventHandler(); 
  std::vector<SubEvent*> list = evh->subEvents();  
// get geometry Handler  
   GeometryHandler* GeoPoi=GeometryHandler::getGeometryHandler();
// Histogram File handler
//   HisFile* hf = HisFile::getHisFile();
   HisFile *rootFile = HisFile::getHisFile(3);
   TNtuple* ntuple;
    
// get Trigger Type 
  Handler<RPCLETSHeader>  &hdh = Handler<RPCLETSHeader>::getHandle(); 
  RPCLETSHeader * head=hdh[hdh.size()-1];
  int TrigType = head->TriggerType();
  int runnumber = head->runNumber();

// continue only for cosmic trigger 
//   if(TrigType!=11)return;    
   
   
   //if(m_debug>0) 
   std::cout << "RPCTBAnalize::taskInit, Trigger Type ---------------"<< TrigType << std::endl;

//
// set some defaults parameters 
//

// bin resolution: set to 1 for normal runs 
   float fact=1;

// special for radiography runs
   if(head->HVScan() && head->ThScan()){
// set bin resolution to 1/4 of a strip 
     fact=4;
// set allchambers = true
//     m_allchambers=true;
//     m_hitsforeffi=6;
       
   } else { 
// this is settign for all other runs. 
// set min number of hits  for tracks to be used for efficiency computation
// normally this parameter should is set to m_minhits if not a radiography run
// and allchambers is not set, if allchambers is set leave as the user defind in datacards
     if(!m_allchambers){
   m_hitsforeffi=m_minhits;
         std::cout << "hits for effi=" << m_hitsforeffi << std::endl;
     }
   }
// get Number of StripPlanes     
   int nsplanes= GeoPoi->nStripPlane();
//compute  number of trigger StripPlanes  
    if(m_debug>0)std::cout << "List of Trigger Chambers: ";  
    nchamtr=0;  
    for(int j=0;j!=GeoPoi->nRPCChamber();j++){
      if(head->IsTriggerChamber(GeoPoi->RPCChamberPtr(j)->Chamber())){
  if(m_debug>0)std::cout << GeoPoi->RPCChamberPtr(j)->Chamber()<< " ";  
  nchamtr++;
      }
    }
    if(m_debug>0)std::cout << std::endl;
    
    numplnotr = nsplanes - NumPlanesPerChamber(0)*nchamtr;
     
// E.G. test di RPCChamber
     //if(m_debug>0){
       std::cout << "Number of RPC Chambers: " << GeoPoi->nRPCChamber();      
       std::cout << ", # of Planes per RPC Chamber: " << NumPlanesPerChamber(0) << std::endl;
       std::cout << "Number of RPC StripPlanes: " << nsplanes << std::endl; 
       std::cout << "Number of trigger RPC StripPlanes : " << NumPlanesPerChamber(0)*nchamtr << std::endl;   
       std::cout << "Number of test RPC StripPlanes: " << numplnotr << std::endl << std::endl;           
     //}

//  track group should be done only for Cosmic trigger
     if(TrigType >= 11){
//       hf->His1D(10000,"pulls in RPC tracks",50,-5.,5.);
//       hf->His1D(10999,"#hits in  RPC tracks",50,-0.5,49.5);
//       hf->His1D(11000,"#hits in  RPC tracks (eta)",30,-0.5,29.5);
//       hf->His1D(11002,"#hits in RPC tracks (phi)",30,-0.5,29.5);
       
//       hf->His1D(10500,"present chambers",12,1.0,13.0);
//       hf->His1D(11003,"Flag",10,0.0,10.0); 
      
//       hf->His1D(11005,"Chi^2 (phi) ",100,0.0,10.0);  
//       hf->His1D(11006,"Chi^2 (eta)",100,0.0,10.0);   
//       hf->His2D(111006,"Chi^2 (eta) vs track impact (eta)",100,-500.0,1500.,100,0.0,10.0);    
//       hf->His1D(11007,"track slope (phi) ",100,-2.0,2.0);  
//       hf->His1D(11008,"track slope (eta)",100,-1.0,1.0);
//       hf->His1D(11017,"track impact (phi) ",120,-1000.0,5000.0);  
//       hf->His1D(11018,"track impact (eta)",100,-500.0,1500.);
//       hf->His1D(11110,"time diff in track eta",50,0.0,50.0);
//       hf->His1D(11112,"time diff in track",50,0.0,50.0);               
// plane efficiency
//       hf->His1D(1,"Raw Efficiency per plane",numplnotr,1.,numplnotr+1);
//       hf->His1D(4,"Efficiency per plane (tracking,acceptance corrected)",numplnotr,1.,numplnotr+1);             
     }
//
// create radiography Histograms, two per gap 
//
   std::vector<RPCChamber*> chambers = evh->GetRPCChambers();
   int  nphi,neta;
   double lowe,highe,lowp,highp;
   RPCChamber *ch;

   // file per salvare i parametri da utilizzare per costruire gli istogrammi nella macro radiography

 
     TFile* final = rootFile->GetRootFile();
     final->cd();
// get list of planes belonging to a gap     
   for(int jch=0; jch!=chambers.size();jch++) { 

   ch=chambers[jch];
     if(!head->IsTriggerChamber(ch->Chamber())){
     for(int ig=0; ig!=ch->nGaps();ig++) {         

       // set histo address
       int hisadd=ch->Chamber()*10+ig+1;
       // 
       std::string name=ch->geo()->gapPtr(ig)->Name();
       std::string hisname;
       std::string tuplename = name+"-TUPLE";
       std::string tupletitle = "Histogram Parameter for chamber: "+name;
       char *htit;

       ntuple = new TNtuple(tuplename.c_str(), tupletitle.c_str(), "nphi:neta:lowp:lowe:highp:highe");

 
       std::vector<RPCStripPlane *> gpl=ch->GapPlanes(ig+1);
       if(gpl[0]->RPCgeometry()->gap()==1 || gpl[0]->RPCgeometry()->gap()==3){
           nphi = gpl[0]->RPCgeometry()->Nstrips(); 
           neta = gpl[1]->RPCgeometry()->Nstrips(); 
           lowp = gpl[0]->RPCgeometry()->lowlimit();
           lowe = gpl[1]->RPCgeometry()->lowlimit();
           highp = gpl[0]->RPCgeometry()->highlimit();;
           highe = gpl[1]->RPCgeometry()->highlimit(); 
       } else {
           nphi = gpl[1]->RPCgeometry()->Nstrips();
           neta = gpl[0]->RPCgeometry()->Nstrips(); 
           lowp = gpl[1]->RPCgeometry()->lowlimit(); 
           lowe = gpl[0]->RPCgeometry()->lowlimit(); 
           highp = gpl[1]->RPCgeometry()->highlimit(); 
           highe = gpl[0]->RPCgeometry()->highlimit(); 
       }

  	ntuple->Fill(nphi,neta,lowp,lowe,highp,highe);
	     //ntuple->Scan();
        //ntuple->Write();

       


        // Expected hitmap
       hisname = name +" Expected Hitmap";
       htit=(char *)name.c_str();
//       hf->His2D(40000+hisadd,htit,nphi*fact,lowp,highp,neta*fact,lowe,highe);   
       rootFile->createTree(hisname, "Expected Hitmap", m_debug);      
       // Response hitmap
       hisname = name + " Obtained Hitmap";
       htit=(char *)name.c_str();
//       hf->His2D(50000+hisadd,htit,nphi*fact,lowp,highp,neta*fact,lowe,highe);      
       rootFile->createTree(hisname, "Obtained Hitmap", m_debug);
     }// loop on gaps  
     }// if on trigger chambers
   } // loop on chambers

 
   //final->Close();
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
//   if(upsidedown==1)idsp=8-idsp+1 ???????????????????????  
// get plane limits
         double lowl=gstptr->lowlimit();
         double higl=gstptr->highlimit();
//
         int nlim=nums;
// cambiare a seconda del tipo di trigger >??         
         if(nlim > 4)nlim=nums/4;
//        
 // they make sense only for Cosmic or AutoTrigger runs 
         std::string title;
     char *htitle;
         if(TrigType >= 11){
/*           title=name+": residuals in RPC tracks ";
           htitle=(char *)title.c_str();
           hf->His1D(10000+idsp,htitle,50,-50.,50.);        
           title=name+": pulls in RPC tracks ";
           htitle=(char *)title.c_str();
           hf->His1D(12000+idsp,htitle,50,-5.,5.); 
       title=name+": timing of cluster";
           htitle=(char *)title.c_str();
           float timediff=(float)m_timemax-(float)m_timemin;
           hf->His1D(20000+idsp,htitle,timediff/15,(float)m_timemin,(float)m_timemax);
*/
// done only for measurement chambers  
     if(!head->IsTriggerChamber(gstptr->Chamber()) || m_allchambers){
/*
           title=name+": size of efficient cluster ";
           htitle=(char *)title.c_str();           
           hf->His1D(9000+idsp,htitle,nlim,-0.5,nlim-0.5); 
           title=name+": residuals ";
           htitle=(char *)title.c_str();
           hf->His1D(13000+idsp,htitle,100,-200.,200.); 
           title=name+": residuals ";
           htitle=(char *)title.c_str();
           hf->His2D(23000+idsp,htitle,100,-200.,200.,10,-0.5,9.5); 
//           title=name+": residuals vs ch";
//           htitle=(char *)title.c_str();
//           hf->His2D(230000+idsp,htitle,100,-200.,200.,nums*fact,lowl,higl);  
//           title=name+": residuals vs perpendicular coordinate";
//           htitle=(char *)title.c_str();
//           hf->His2D(240000+idsp,htitle,100,-200.,200.,nums*fact,lowl,higl);
           title=name+": pulls ";
           htitle=(char *)title.c_str();
           hf->His1D(14000+idsp,htitle,25,-5.,5.);     
           title=name+": hit profile, tracking ";
           htitle=(char *)title.c_str();
           hf->His1D(7000+idsp,htitle,nums*fact,lowl,higl);
           title=name+": Efficiency per Strip";
           htitle=(char *)title.c_str();
           hf->His1D(8000+idsp,htitle,nums*fact,lowl,higl);   
           title=name+": Profile for noise clusters";
           htitle=(char *)title.c_str();
           hf->His1D(15000+idsp,htitle,nums,lowl,higl); 
           title=name+": size of noise clusters";
           htitle=(char *)title.c_str();
           hf->His1D(16000+idsp,htitle,nlim,-0.5,nlim-0.5);           
*/
         }
       }
     }
}
/***********************************************************************************
 *  End
 ***********************************************************************************/
bool RPCTBAnalize::taskEnd() 
{
  if(m_debug>0) std::cout << "RPCTBAnalize: taskEnd "<< std::endl;

// get Timer Handler 
  MapHandler<Timer>  &htt = MapHandler<Timer>::getHandle(); 
// Stop Timer
  htt["RPCTBAnalize"]->Stop();
//        
// print Timing counters     
//

  std::cout << "\nRPCTBAnalize execution Time: " 
            << htt["RPCTBAnalize"]->Time() << std::endl;
  return true;
}
/***********************************************************************************
 *
 ***********************************************************************************/
void RPCTBAnalize::runInit() 
{
  Handler<RPCLETSHeader>  &hdh = Handler<RPCLETSHeader>::getHandle(); 
  RPCLETSHeader * head=hdh[hdh.size()-1];
  int TrigType = head->TriggerType();
  if(m_debug>0) 
      std::cout <<"RPCTBAnalize: RunInit run number "<< head->runNumber() << std::endl;
}
/***********************************************************************************
 *
 ***********************************************************************************/
void RPCTBAnalize::runEnd() 
{
   if(m_debug>0) std::cout << "RPCTBAnalize: RunEnd "<< std::endl;
}
/***********************************************************************************
 * Event Processing
 ***********************************************************************************/
void RPCTBAnalize::evtProc()
{    
   
  MapHandler<Timer>  &htt = MapHandler<Timer>::getHandle();  
  htt["RPCTBAnalize"]->Start();
// get various Handlers
  GeometryHandler *GeoPoi=GeometryHandler::getGeometryHandler();
  EventHandler * evh =EventHandler::getEventHandler(); 
  DeadHotHandler *dh = DeadHotHandler::getDeadHotHandler();
  Handler<RPCLETSHeader>  &hdh = Handler<RPCLETSHeader>::getHandle(); 
  RPCLETSHeader * head=hdh[hdh.size()-1];
//  HisFile *hf = HisFile::getHisFile();
  HisFile *rootFile = HisFile::getHisFile(3);

  int event = evh->eventNumber();
  int TrigType = head->TriggerType(); 
  
  // continue only for cosmic trigger 
  if(TrigType!=11)return;
      
  int run = head->runNumber();
  
  if(m_debug>0) std::cout << std::endl << "RPCTBAnalize: EvtProc event "<< event << std::endl;
                          
  if(event%1000==0)std::cout << "RPCTBAnalize: EvtProc event "<< event << std::endl;

//# of RPC strip planes per chamber   
  int numpc=NumPlanesPerChamber(0);

// per il momento fare cosi', poi settare le ControlManager variblse appropriatamente,
  if(TrigType == 10){
    m_timemin=0;   
    m_timemax=465;    
  }
//
// from now on make tracking and compute efficiency only for Cosmic runs
//
  if(TrigType >= 11){
//    Histo *hden,*hnum;
 
    htt["RPCReconstructor"]->Start(); 

//    std::cout << "gabrita: parte il reconstructor " << std::endl;
// loop here on chambers or group of chambers
    
// create and initialize reconstructor object, with list of chambers to be used  
//    std::cout << "gabrita: Numero piani fuori trigger 8? " << numplnotr<< std::endl;
//    std::cout << "gabrita: Numero piani per camera trigger 8? " << NumPlanesPerChamber(0)<< std::endl;
//    std::cout << "gabrita: Numero min hits? " << m_minhits<< std::endl;
//    std::cout << "gabrita: Numero max hits? " << m_maxhits<< std::endl;

    RPCReconstructor *rec=new RPCReconstructor(numplnotr,
                                         NumPlanesPerChamber(0),m_minhits,m_maxhits);  
// make pattern recognition and get list of track candidates
    std::vector<RPCTrack *>::iterator ik;       
    std::vector<RPCTrack *> tkCandidates=rec->RPCPatRec(m_debug,m_allchambers); 
    htt["RPCReconstructor"]->Stop();

    if(m_debug)std::cout << "RPCTBAnalize: number of tracks found: " << tkCandidates.size() << std::endl;
// fill flag for this track (funziona solo per una traccia !!!!)
//    hf->his(11003)->fill(rec->flag(),0.0,1.0);
// delete the object;
    delete rec;       
// store tracks, but first delete old track objects in m_tracks
    if(!m_tracks.empty())for(ik=m_tracks.begin(); ik!=m_tracks.end();ik++)delete *ik;
    m_tracks=tkCandidates;
// fill some histos
    for(ik=tkCandidates.begin(); ik!=tkCandidates.end();ik++)(*ik)->fill(10000);
      
    htt["loop on tracks"]->Start();   
// now loop on tracks . There should be only one...    
    for(ik=tkCandidates.begin(); ik!=tkCandidates.end();ik++) {   
    if(m_debug)std::cout << "RPCTBAnalize: #hits in track: " << (*ik)->nHits(0) << " " << 
                                                                  (*ik)->nHits(2) << std::endl;
    
// use for efficiency computation only tracks with at least hitsforeffi hits
    if( (*ik)->nHits(0) < m_hitsforeffi/2 || (*ik)->nHits(2) < m_hitsforeffi/2)continue;
//************************************************************************   
// compute the efficiency for all test Chambers
//************************************************************************

// get and loop on list of chambers
    std::vector<RPCChamber*> chambers = evh->GetRPCChambers();
    for(int jch=0; jch!=chambers.size();jch++) { 
      RPCChamber * theCham = chambers[jch];
      int ch=theCham->Chamber();
// skip  trigger chambers
      if(head->IsTriggerChamber( ch ) ) continue; 

      if(m_debug)std::cout << " Analizing chamber=" << ch  << std::endl;
  
// loop on list of layers in this chamber
      for(int ic=0; ic!=theCham->nLayers();ic++) { 
       int la=ic+1;     
       if(m_debug)std::cout  << "   layer " << la << std::endl;
       
  

// get and loop on the gap numbers belonging to this layer (layers are numbered from 1) 
        std::vector<int> theGaps=theCham->GapNumbers(la);      
        for(int ig=0; ig!=theGaps.size();ig++) {
          int gap=theGaps[ig];   
// get the list of planes belonging to this gap    
          std::vector<RPCStripPlane *> thePlanes=theCham->GapPlanes(gap);  
// here we  get track extrapolation in both direction 
          double estrapeta,estrapphi,sigestrap;
          GeoRPCStripPlane *geoeta, *geophi;
    
          if(la==1){
            geoeta=thePlanes[1]->RPCgeometry();
            geophi=thePlanes[0]->RPCgeometry();  
          } else if(la==2){
            geoeta=thePlanes[0]->RPCgeometry();
            geophi=thePlanes[1]->RPCgeometry();
          }
        
          estrapeta=(*ik)->estrapPlane(0, (geoeta->StripCoord(1)).Y(), sigestrap);
//         std::cout << "gabrita: track estrap eta  " << estrapeta << " is in gap acceptance\n";
//        std::cout << "gabrita: error eta " << sigestrap<< " is in gap acceptance\n";

          estrapphi=(*ik)->estrapPlane(2, (geophi->StripCoord(1)).Y(), sigestrap); 
//         std::cout << "gabrita: track estrap phi  " << estrapphi << " is in gap acceptance\n";
//        std::cout << "gabrita: error phi " << sigestrap<< " is in gap acceptance\n";

          
// skip this gap if outside gap limits       
        if(!geoeta->InsideInstrumented(estrapeta) || 
           !geophi->InsideInstrumented(estrapphi)    )continue;
        if(m_debug)std::cout << "track " << gap << " is in gap acceptance\n";

//         std::cout << "gabrita: track estrap " << gap << " is in gap acceptance\n";

// skip this gap if outside gap fiducial volume 
//          if(!geoeta->InsideFiducial(estrapeta) || !geophi->InsideFiducial(estrapphi)    )continue;
//          if(m_debug)std::cout << "track " << gap << " is in gap fiducial volume\n"; 

          bool foundgap[2] = {false,false};                 
          int dir,idsp,pl,sw;     
          double estrap, estrapp;
// loop on the strip planes of this layer 
        
          for(int ns=0; ns!=thePlanes.size();ns++) {
// get info for the geometry of this plane         
            GeoRPCStripPlane* gstptr=thePlanes[ns]->RPCgeometry(); 
            idsp=gstptr->idStripPlane();
            dir=gstptr->StripDir();
            pl=gstptr->StripPlane();
      sw=gstptr->StripWidth();
      
// get track estrapolation at this plane            
            if(dir==0){estrap=estrapeta;estrapp=estrapphi;}
            else {estrap=estrapphi;estrapp=estrapeta;}
 
/* OBSOLETE, use coordinates... 
// get strip which includes the estrapolation, needed for check of dead window 
            GeoRPCStrip* theStrip=gstptr->findStripAt(estrap); 
            if(m_debug){
              std::cout << "track estrapolation at plane " << idsp;
              if(theStrip)std::cout << ": expected strip is: " << theStrip->idStrip() << std::endl;
              else std::cout << " but outside chamber boundary "  << std::endl;
            }
// check if track estrapolation is not in a dead strip (+/- 1 strip)      
            if(!theStrip || (dh->isDeadWindow( theStrip->idStrip(), run) ))continue;
            if(m_debug)std::cout << "valid strip, search for it"<< std::endl;
*/

            htt["search for hit"]->Start();
// fill normalization histogram at estrap                    
//            hden=hf->his(7000+idsp);     
//            hden->fill(estrap,0.0,1.0);     

// qui si puo' fare una funzione di RPCStripPlane int nearestCluster(estrap)

// now loop on thislayer hits to search the nearest Cluster center to the track extrapolation            
            int nearClus=-1,clusiz;
            float residmin=999999.0,resid;
            float pullmin,pull;
// get the cluster list for this plane and loop on that
            std::vector<RPCCluster*>  thislayer= thePlanes[ns]->clusList();
            for(int i=0; i!=thislayer.size();i++) {
// get cluster center and compute residual and pull 
              double cent=  thislayer[i]->X();
              if(dir==2)cent=thislayer[i]->Z();
              resid=cent-estrap;
        
              pull=resid/thislayer[i]->Err();
// search and store minimum distance                   
              if(fabs(resid) <= fabs(residmin)){
// set values and pointer for minimum distance from center
                residmin=resid;
                pullmin=pull;
                nearClus=i;
    clusiz=thislayer[i]->size();
              }  // end resid  
            }// end for thislayer
              
// here if cluster found
            if( nearClus!=-1 ){  
              if(m_debug)std::cout << "found a possible cluster"<< std::endl;
// fill residuals for nearest cluster                    
//              hf->his(13000+idsp)->fill(residmin,0.0,1.0);
//        hf->his(23000+idsp)->fill(residmin,clusiz,1.0); 
//        hf->his(230000+idsp)->fill(residmin,estrap,1.0); 
//        hf->his(240000+idsp)->fill(residmin,estrapp,1.0);
              

// fill pulls for nearest cluster         
//              hf->his(14000+idsp)->fill(pullmin,0.0,1.0);                                          
// apply cut for efficiency (again in strip size and not in sigma of residuals)
//              if(theStrip->Strip()+m_stripwindow >=thislayer[nearClus]->firstStrip() &&
//                 theStrip->Strip()-m_stripwindow <=thislayer[nearClus]->lastStrip()     ){
// apply cut for efficiency, require extrapolation is at maximum one strip away from end of cluster 
              if((estrap+m_stripwindow*sw) >= gstptr->lowlimit(thislayer[nearClus]->firstStrip()) &&
                 (estrap-m_stripwindow*sw) <= gstptr->highlimit(thislayer[nearClus]->lastStrip())     ){
      
                if(dir==0)foundgap[0]=true;
                else foundgap[1]=true;
          
                if(m_debug){
                  std::cout << "cluster is in window"<< std::endl;             
                  thislayer[nearClus]->print();
                }
// fill cluster size for efficient clusters                 
//                hnum=hf->his(9000+idsp);
//                hnum->fill(thislayer[nearClus]->size(),0.0,1.0);
// count efficient and fill profile for efficent clusters
//                hnum=hf->his(8000+idsp);    
//                hnum->fill(estrap,0.0,1.0);
              } else { 
                if(m_debug)std::cout << "cluster missing "<< std::endl;
    
    //gabriele  remove geometrical match
                if(dir==0)foundgap[0]=true;
                else foundgap[1]=true;
    
    
//        here fill histos for inefficient hits (all but nearClus)             
                for(int i=0; i!=thislayer.size();i++) { 
                  if(i != nearClus) {
// profile 
                    double coo=  thislayer[i]->X();
                    if(dir==2)coo=thislayer[i]->Z();               
//                    hnum=hf->his(15000+idsp);     
//                    hnum->fill(coo,0.0,1.0);     
// cluster size                    
//                    hnum=hf->his(16000+idsp);     
//                    hnum->fill(thislayer[i]->size(),0.0,1.0);                              
                  } 
                }  
                
                        
              } // end if(theStrip
            } // end !nearClus    
                    htt["search for hit"]->Stop(); 
        
          } //end loop on planes in this gap
// here we should have info for both planes of this gaps, make efficiencies per gap        
          if(m_debug){
            std::cout << "foundgap: " << foundgap[0] << " " << foundgap[1] << std::endl;          
          }
          
          int hisadd=ch*10+gap;
          
    // inizialmente la ricerca del gapPtr era fatta utilizzando il ig che fa da contatore per il ciclo, ma in realtà la vera gap è data dall'indicatore stesso della gap, che è proprio l'int gap che si ottiene poco più sopra. Dato che gap va da 1 a 4, e dato che il gapPtr accetta solo valori delle gap tra 0 e 3 allora è necessario decrementarlo di una unità
    std::string treename=theCham->geo()->gapPtr(gap-1)->Name();
          //std::cout << "treename: " << treename << "\ngap: " << gap << "\nig: " << ig << std::endl;
          std::string histreename;
          histreename = treename +" Expected Hitmap";
// fill normalization histo
          rootFile->createBranch(histreename, &estrapphi, "EstrapPhi", m_debug);
          rootFile->createBranch(histreename, &estrapeta, "EstrapEta", m_debug);
//          hf->his(40000+hisadd)->fill(estrapphi,estrapeta,1.0);
          rootFile->fillTree(histreename);
          // this case gap declared efficient 
//    std::cout<<" gabrita: APRIORI ESTRAP  " << estrapphi<<", " << estrapeta << std::endl;
//    std::cout<<" gabrita: dir  " << dir << " foundgap 0  " << foundgap[0] << " foundgap 1  " << foundgap[1] << " hisadd" << hisadd << std::endl;

          if(foundgap[0] || foundgap[1]){
//            std::cout<<" gabrita: LUNA estrap  " << estrapphi<<", " << estrapeta  << " hisadd" << hisadd << std::endl;
             histreename = treename +" Obtained Hitmap";
             rootFile->createBranch(histreename, &estrapphi, "EstrapPhi", m_debug);
             rootFile->createBranch(histreename, &estrapeta, "EstrapEta", m_debug);

//             hf->his(50000+hisadd)->fill(estrapphi,estrapeta,1.0);
             rootFile->fillTree(histreename);
          }
        
        
        } //end loop on gaps 
      } //end loop on layers            
    } // end loop on chambers
    } // end loop on tkcandidates  
    htt["loop on tracks"]->Stop();
    tkCandidates.clear(); 
    
    
    
  } // end of if(TrigType)  
// stop Timer
  htt["RPCTBAnalize"]->Stop();
} 
/*************************************************************************
 *  get number of Planes per Chamber
 *************************************************************************/
int RPCTBAnalize::NumPlanesPerChamber(int ichamb){
     GeometryHandler* GeoPoi=GeometryHandler::getGeometryHandler();
     GeoRPCChamber *     rpcc=GeoPoi->RPCChamberPtr(ichamb);
     return rpcc->nStripPlanes();
}
