#include "control/CalibrationManager.h"
#include "event/RPCLETSEvent.h"
#include "event/SubEvent.h"
//#include "event/DCSBlock.h"
//#include "event/DCSLETSBlock.h"
//#include "event/DCSHandler.h"
#include "event/RPCLETSHeader.h"
#include "event/Handler.h"
#include "detector/RPCStripPlane.h"
#include "detector/RPCStrip.h"
#include "detector/Latch.h"
#include "database/GeometryHandler.h"
#include "database/EltxHandler.h"
#include "his/HisFile.h"
#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
//#include "Tbranch"
#define DATATYPE 0xffffffff
#define SLOWTYPE 0xfffffffe   
#define RUNHEADER 0x1234

RPCLETSEvent::RPCLETSEvent():EventHandler(),m_trigTime(0)
{ 
    if(m_EventHandler) abort(); 
    MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
    Memory->increase();   
    m_EventHandler = this; 
                          
}

RPCLETSEvent::~RPCLETSEvent()
{  
    MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
    Memory->decrease();   
    Handler<RPCLETSHeader>  &hd = Handler<RPCLETSHeader>::getHandle();
    // cancellare l'header!!!!!
}


bool RPCLETSEvent::loadEvent()
{
    if((*m_input)->eof()) return false;
// read the complete event from input file, returns list of latches found.
// In readEvent it also identifies DCS blocks and add these to DCSHandler  
   if(m_debugLevel>3) std::cout << " loadEvent() " << std::endl;
    std::vector<Latch*> latches=readEvent();
// decode event from list of latches
//    if(latches.size())return eventDecode(latches);
//       else return false;
    return eventDecode(latches);
}
/*******************************************************************************************
 *
 * load Run header
 *
 *******************************************************************************************/
bool RPCLETSEvent::loadRun()
{

  
// create, read and fill the run Header
  RPCLETSHeader *head = new RPCLETSHeader(m_input);
  
  head->print();
// add the header to handler  
  Handler<RPCLETSHeader>  &hd = Handler<RPCLETSHeader>::getHandle();
  hd.push_back(head);
// now create input/eltxfiles with the right configuration file 
  std::string local(head->PathConf());
  int start=local.find_last_of("\\")+1;
  int end=local.size();
  //std::string filnm="/afs/le.infn.it/user/d/daqatlas/rpcdaq/Daq/configuration/"+local.substr(start,end);
  std::string filnm="input/RPCLETS/"+local.substr(start,end);
  std::ofstream outeltx("input/eltxfiles");
  outeltx << filnm << std::endl;
  outeltx.close();
// now open 
  EltxHandler * eltx = EltxHandler::getEltxHandler();  
}


bool RPCLETSEvent::loadEnd()
{
   return true;
}
/*******************************************************************************************
 *
 * read the Event, includes the presence of a DCS block 
 *
 *******************************************************************************************/
std::vector<Latch*> RPCLETSEvent::readEvent() {

  std::vector<Latch*> latches;
  
  unsigned long int dummy,size;
  unsigned long int head;
  
  Handler<RPCLETSHeader>  &hdh = Handler<RPCLETSHeader>::getHandle();
  RPCLETSHeader * h=(RPCLETSHeader *)hdh[hdh.size()-1];

// read first 4 bytes (header) checks here also for end of file 
//  if(!(*m_input)->read( (char *) &dummy, sizeof(dummy)))return latches;
    if(m_debugLevel>3) std::cout << " RPCLETSEvent::readEvent()" << std::endl;
  
  //if(!(*m_input)->read( (char *) &dummy, sizeof(dummy)))return latches; 
  if(!(*m_input)->read( (char *) &dummy, 4))return latches;
  
  head=invertbytes(dummy);
  
   if(m_debugLevel>3) std::cout << std::hex << " dummy: " << head << std::endl;
  
  if(m_debugLevel>3)
    std::cout << "readevent: " << std::setbase(16) << head  << std::setbase(10) << std::endl;



// // checks first for presence of DCS block  
   if(head == SLOWTYPE) {
     std::cout << "SLOWTYPE. Now rejected" << std::endl;
//     Handler<DCSLETSBlock>  &dcs = Handler<DCSLETSBlock>::getHandle();
//     std::cout << "SLOWTYPE:end" << std::endl;
//     /*gabriele
//     DCSLETSBlock *bl=new DCSLETSBlock(h->SwapStatus(), (*m_input), (*m_input)->tellg()-4, h->NumRPCinDCS());
//     
//     dcs.push_back(bl);
// // print the first DCS block or all if debuglevel   
//     if(m_debugLevel || dcs.size()==1)bl->print();
//    
// // now should read again the "real" event" if not a DCS Trigger
//     if(h->TriggerType()!=12){
//      if(!(*m_input)->read( (char *) &dummy, sizeof(dummy)))return latches;     
//      head=invertbytes(dummy);
//      std::cout << "dopo block: " << std::setbase(16) << head  << std::setbase(10) << std::endl;
//     }
//     */

       if(!(*m_input)->read( (char *) &dummy, 4))return latches;
       head=invertbytes(dummy); 
       
       for(unsigned long int h=0; h!=head/4 ;h++){ 
        if(!(*m_input)->read( (char *) &dummy, 4))return latches;
        unsigned long int head_dummy=invertbytes(dummy);      
       }

       return latches;
   }
//     std::cout << "SLOWTYPE:end:end" << std::endl;



// here we got the "real" event
// checks event header  
  if(head == DATATYPE) {
     if(m_debugLevel>3)   std::cout <<  " DATATYPE " << head << std::endl; 
     if(m_debugLevel>3)
     std::cout << std::endl << "RPCLETSEvent::readevent: found EVENT HEADER " << std::endl;
// get # of Time Bins from Header
     Handler<RPCLETSHeader>  &hdh = Handler<RPCLETSHeader>::getHandle(); 
     RPCLETSHeader * hd=hdh[hdh.size()-1];                               
     int nbins=hd->TimeBins();
     if(m_debugLevel>3) std::cout <<  " nbins " << nbins<< std::endl; 
// read header (nbins long words * 4 bytes) (one is already read, is the header)
     //unsigned long int buffer[nbins];
     unsigned long int buffer[nbins];
     unsigned int buffer_dummy[nbins];
     (*m_input)->read((char *)  &buffer_dummy, nbins*4);
     for(int i=0;i!=nbins;i++)buffer[i]=buffer_dummy[i];
   
   
                               
     size=(int)invertbytes(buffer[0]);
     
     
      if(m_debugLevel>3)std::cout <<  " size " <<size << std::endl; 
     
     m_eventNumber=(int)invertbytes(buffer[1]);
     unsigned long int numberOfLatches=invertbytes(buffer[2]);
     unsigned long int numberOfWordsPerLatch=invertbytes(buffer[3])*0+32;
     if(m_debugLevel>3){
       std::cout << "eventNumber: " << m_eventNumber<< std::endl;
       std::cout << "event  size: " << size << std::endl;
       std::cout << "#latches and #words: " << numberOfLatches << " " << numberOfWordsPerLatch<< std::endl;
       std::cout << " Ev. Header: " << std::setbase(16); 
       for(int j=0; j!=nbins; j++)std::cout << invertbytes(buffer[j]) << " ";
       std::cout << std::setbase(16) << std::endl; 
     }
    if(numberOfLatches==0)return latches;
 // create a new latch object and put in Latches    
     for(int i=0; i!=numberOfLatches; i++) {
       unsigned long int linfo[numberOfWordsPerLatch];
       unsigned int linfo_dummy[numberOfWordsPerLatch];
       (*m_input)->read( (char *)  &linfo_dummy, numberOfWordsPerLatch*4);
       for(int i=0;i!=numberOfWordsPerLatch;i++)linfo[i]=linfo_dummy[i];
       
       if(m_debugLevel>3){
         std::cout <<  "Latch: " << i << ":" << std::setbase(16);   
         for(int j=0; j<numberOfWordsPerLatch; j++) 
                    std::cout << invertbytes(linfo[j]) << " ";
         std::cout << std::setbase(10) << std::endl;    
        }
// swap bytes if necessary
       for(int j=0; j<numberOfWordsPerLatch; j++)linfo[j]=invertbytes(linfo[j]);    
// necessario, cambiamo?      
       unsigned long int * bb = &linfo[0];       
       latches.push_back(new Latch(bb,numberOfWordsPerLatch));
     }
  } else {
    if(h->TriggerType()!=12)std::cout << "RPCLETSEvent:: event Header not found: " << head << std::endl;
  } 
  return latches;
} 
/*******************************************************************************************
 *
 * decode here the event
 *
 *******************************************************************************************/
bool RPCLETSEvent::eventDecode(std::vector<Latch*> latches) {
// loop, decode and add to list the found strips....
if(m_debugLevel>3)std::cout << "loop, decode and add to list the found strips...." << std::endl;
  std::vector<RPCStrip*> striplist,minilist;   
  std::vector<RPCStrip*>::iterator is;
  std::vector<Latch*>::iterator la;
/*  
  HisFile *hf = HisFile::getHisFile();
  if(!hf->his(1000))hf->His1D(1000,"# of banks",50,0.,100.);
  if(!hf->his(1001))hf->His1D(1001,"# of fired channels per bank",30,0.,30.);
  if(!hf->his(1002))hf->His1D(1002,"# hits per event",50,0.,100.);
  if(!hf->his(9999))hf->His1D(9999,"prova",50,0.,100.);
  
  hf->his(1000)->fill(latches.size(),0.0,1.0);
*/
// crea il ttree -- per ora non ci serviamo di queste variabili, quindi i tree sono commentati
//  HisFile *rootFile = HisFile::getHisFile(3);

  double latchesSize = latches.size();
  
  int tot=0; 

//  rootFile->createTree("latches");
//  rootFile->createBranch("latches", &latchesSize, "banks", m_debugLevel);
//  rootFile->createBranch("latches", &tot, "hits_per_event", m_debugLevel);


 // return if empty event 
  if(latches.size()==0)
  { 
//  rootFile->fillTree("latches");
  return true;
  }
  
   
  for(la=latches.begin(); la!=latches.end(); la++){
    minilist=(*la)->StripList();
    double minilistSize = minilist.size();
  //hf->his(1001)->fill(minilist.size(),0.0,1.0);
  
  //  rootFile->createTree("channels");
  //  rootFile->createBranch("channels", &minilistSize, "fired_channels_per_bank", m_debugLevel);
  //  rootFile->fillTree("channels");

    for(is=minilist.begin(); is!=minilist.end(); is++)striplist.push_back((*is));
  tot+=minilist.size();
  }
  //hf->his(1002)->fill(tot,0.0,1.0);

// rootFile->fillTree("latches");

// I have used the latches, delete them: really we would like to add their address
// to the strip object !!!..... TODO 
    for(la=latches.begin(); la!=latches.end(); la++)delete *la;
    
// now loop on list of strip and add the RPCstrip to the right plane
  for(is=striplist.begin(); is!=striplist.end(); is++){
////////    std::cout << " new strip" << std::endl;
////////    (*is)->print();
// search the StripPlane in SubEvent
    RPCStripPlane * plane = 0;
    std::vector<SubEvent*> list = subEvents();     
    std::vector<SubEvent*>::iterator it;
    for(it=list.begin(); it!=list.end(); ++it) {
// check  chamber number    
      if( (*it)->type() == 4 &&  ( (*it)->id() == (*is)->chamber() ) ) {
// search stripplan
        if(m_debugLevel>2){
          std::cout << "RPCLETSEvent::eventDecode found Chamber " << (*it)->id() << 
                     " searching for plane " << (*is)->plane() << std::endl;
        }
        RPCChamber * ch=(RPCChamber *)(*it);
        plane = ch->findStripPlane( (*is)->plane() );

      }
    }
// if detector found      
    if(plane) {
// Add the hit to the hit list in plane
      plane->addHit((*is)); 
    } else {
      std::cout << "WARNING RPCLETSEvent::eventDecode could not find SubEvent, ";
      std::cout << "chamber " << (*it)->id() << " not found" << std::endl;

    }
  } // end loop on striplist

  CalibrationManager *cm=CalibrationManager::getCalibrationManager();
  Handler<RPCLETSHeader>  &hdh = Handler<RPCLETSHeader>::getHandle(); 
  RPCLETSHeader * hd=hdh[hdh.size()-1];
// set time Window according to Trigger Type. For random trigger open anyway to maximum window 
  int timemin,timemax;  
  if(hd->TriggerType() == 10){
    timemin=0;   
    timemax=465;    
  } else {
    timemin=cm->timeMin();
    timemax=cm->timeMax();
  }
  
   if(m_debugLevel>2)std::cout << "time min and max " << timemin << " " <<timemax <<std::endl;
  
// sort the strips, clusterize in time and make strip clusters.    
  std::vector<RPCStripPlane*> planes=GetStripPlanes();
  std::vector<RPCStripPlane*>::iterator it;
 // input il tempo ricordati...   
  for(it=planes.begin(); it!=planes.end(); ++it){
///////   std::cout << std::endl<< "RPCLETSEvent printout before" << std::endl;
///////   (*it)->printFull();
   (*it)->Consolidate(timemin,timemax);        
    if(m_debugLevel>2){
      std::cout << std::endl<< "RPCLETSEvent plane printout " << std::endl;
      (*it)->printFull();
    }
  } 
  return true;
}
/*******************************************************************************************
 *
 * invertbytes for treating big o little endian systems, long int
 *
 *******************************************************************************************/
#ifdef MOTOROLA
unsigned long int RPCLETSEvent::invertbytes (unsigned long int dat) {return dat;}
#else
unsigned long int RPCLETSEvent::invertbytes (unsigned long int dat) {
unsigned long int temp[4]={0,0,0,0};
unsigned long int out=0;
unsigned long int nbyte[4]={0x000000ff,0x0000ff00,0x00ff0000,0xff000000};
int i;
 for(i=0; i<4; i++) { 
  if(i==0) temp[3-i] = (dat & nbyte[i])<<24;
  if(i==1) temp[3-i] = (dat & nbyte[i])<< 8;
  if(i==2) temp[3-i] = (dat & nbyte[i])>> 8;
  if(i==3) temp[3-i] = (dat & nbyte[i])>>24; 
} 
 for(i=0;i<4;i++) {out=out | temp[i];}
 return out;
}
#endif
/*******************************************************************************************
 *
 * invertbytes for treating big o little endian systems, for short int
 *
 *******************************************************************************************/
#ifdef MOTOROLA
unsigned short int RPCLETSEvent::invertbytes (unsigned short int dat) {return dat;}
#else
unsigned short int RPCLETSEvent::invertbytes (unsigned short int dat) {
unsigned short int temp[2]={0,0};
unsigned short int out=0;
unsigned short int nbyte[2]={0x00ff,0xff00};

 for(int i=0; i<2; i++) { 
   if(i==0) temp[1-i] = (dat & nbyte[i])<<8;
   if(i==1) temp[1-i] = (dat & nbyte[i])>>8;
 } 
 for(int i=0;i<2;i++) {out=out | temp[i];}
 return out;
}
#endif

double RPCLETSEvent::tdcScale(int tdcNum=0) const
{
  return 0.78;    // ns per TDC count, AMT standard settings
}

