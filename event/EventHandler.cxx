/* **************************************************************
 * mods: method firstEvent: removed loadEvent(). S.Z. 5/3/02
 *       method openInputStream: added error message in case file
 *              open fails  S.Z. 5/3/02
 *
 * ************************************************************** */

#include "event/EventHandler.h"
#include "event/SubEvent.h"
#include "detector/DetCreator.h"
#include "control/CalibrationManager.h"  

/* #ifdef SPYDATA
// to read from circular buffer
#include "spy_extended.h"
static SPY_ext_id spy_id;
#endif
*/        

EventHandler* EventHandler::m_EventHandler = 0;

EventHandler* EventHandler::getEventHandler() 
{  
    if(!m_EventHandler) {
       std::cout << "Abstract class EventHandler:"<<std::endl;
       std::cout << "getEventHandler called without instantiation of a concrete class!"<< std::endl;
       abort();
    }
    return m_EventHandler; 
} 

EventHandler::EventHandler()
{ 
    if(m_EventHandler) abort(); 
    MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
    Memory->increase();   
    m_eventNumber=0;
    m_runNumber=0;
    m_EventHandler = 0; 
    CalibrationManager * manager = CalibrationManager::getCalibrationManager();
    m_debugLevel= manager->Debug();
}

EventHandler::~EventHandler()
{  

    if(!m_subeventlist.empty()) {
       std::vector<SubEvent*>::iterator  is;
       for( is=m_subeventlist.begin(); is!=m_subeventlist.end(); ++is) {
           delete *is;
       }
    }
    while( !m_subeventlist.empty() ) {
       m_subeventlist.pop_back();
    }


    if(!m_inputList.empty()) {
       std::vector<std::ifstream*>::iterator it;
       for( it=m_inputList.begin(); it!=m_inputList.end(); ++it) {
           (*it)->close();
       }
    }

    while( !m_inputList.empty() ) {
       m_inputList.pop_back();
    }


    MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
    Memory->decrease();   
}

bool EventHandler::firstEvent()
{
    if( !m_inputList.empty() ) {
        rewindInputStream();
    } else {
        if(!openInputStream()) return false;
    }
    
   bool lr=loadRun();
   
// once the run header has been read, we can create
// the detectors, list is based on geometry
// is necessary that the first time we call the GeometryHandler is in 
// creator->activeDetectors(), because it access the Run Handler.
    DetCreator * creator = new DetCreator;
    m_subeventlist = creator->activeDetectors();
    delete creator;
	return lr;
}

bool EventHandler::nextEvent()
{  bool res;
   
    clearEvent();
// to debug event memory leaks
//    MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
//    std::cout << "memory " << Memory->objects() << std::endl;
    return loadEvent();
}

bool EventHandler::nextRun()
{
   clearEvent();
   ++m_input;
   if(m_input !=m_inputList.end()) {
      if(loadRun()) {
         return loadEvent();
      }
   }
   return false;
}

int EventHandler::eventNumber() const
{ 
    return m_eventNumber;
}

std::string EventHandler::strippedName()
{ 
// get only the filename stripped from rest of path and with .dat removed
   std::string local(m_fileName);
   int br;
   while((br = local.find(".dat")) != std::string::npos)local.erase(br, strlen(".dat"));
   int start=local.find_last_of("/")+1;
   int end=local.size();
   return local.substr(start,end);
}


std::vector<SubEvent*> EventHandler::subEvents() {
    return m_subeventlist;
}

//
//   private methods
//

bool EventHandler::openInputStream()
{
//#ifndef SPYDATA
    char * fileList = "input/inputfiles";
    char fileName[255];
    std::ifstream input(fileList);
    if(!input) {
        std::cout << "EventHandler::openInputStream could not open in file" << std::endl;
        std::cout << "              file not found " << fileList << std::endl;
        abort();
    } else {
        while(input >> fileName) {
	  std::ifstream *file = new std::ifstream(fileName);
          if (!(*file)) {
             std::cout << "EventHandler::openInputStream ERROR opening input file" << fileName << std::endl << std::endl; 
             std::cout << "============== INSERIRE IL NUMERO DEL RUN ALL'AVVIO! ==============\n" << std::endl;
             std::cout << "**************** USAGE: ./run.sh NUMERORUN ****************" << std::endl;
          }  
          else std::cout << "EventHandler: opened input file "<< fileName << std::endl; 
          m_fileName=(std::string)fileName;
          m_inputList.push_back(file);
 
        }
        m_input = m_inputList.begin();
	
        return true;
    }
    
//#else
//    int err; 
//    if ((err=spy_open_CB("/tmp/R_1",&spy_id))!=SPY_ERROR_OK){
//        std::cout<<"spy_open_local has failed\n"; 
//        return false;
//    }
//    return true;
//#endif
}

void EventHandler::rewindInputStream()
{
    std::ifstream * temporary;
    std::vector<std::ifstream*>::iterator it;
    for( it=m_inputList.begin(); it!=m_inputList.end(); ++it) {
       (*it)->clear();
       (*it)->seekg(0,std::ios::beg);
    }
    m_input = m_inputList.begin();
    return;
}

void EventHandler::clearEvent()
{
    if(!m_subeventlist.empty()) {
       std::vector<SubEvent*>::iterator  is;
       for( is=m_subeventlist.begin(); is!=m_subeventlist.end(); ++is) {
          if(m_debugLevel>4) {
             std::cout <<" clearing the contents of SubEvent:"<< std::endl;
             (*is)->print();
          }
          (*is)->clear();
       }
    }
}


std::vector<RPCStripPlane*> EventHandler::GetStripPlanes(){
     std::vector<SubEvent*>::iterator it;
     std::vector<RPCStripPlane*> splist;
     for(it=m_subeventlist.begin();it!=m_subeventlist.end(); it++) {
       if((*it)->type() == 4) {       
         RPCChamber * ch = (RPCChamber *)(*it);
         for(int j=0;j!=ch->nStripPlanes(); j++)splist.push_back(ch->stripplanePtr(j));
       }
     }  
     return splist;
}

std::vector<RPCChamber*> EventHandler::GetRPCChambers(){
     std::vector<SubEvent*>::iterator it;
     std::vector<RPCChamber*> splist;
     for(it=m_subeventlist.begin();it!=m_subeventlist.end(); it++) {
       if((*it)->type() == 4) {       
         RPCChamber * ch = (RPCChamber *)(*it);
         splist.push_back(ch);
       }
     }  
     return splist;
}

int EventHandler::runNumber() const
{ 
    return m_runNumber;
}

std::string EventHandler::date() const
{
    return m_date;
}

int EventHandler::NumEvents() const
{
    return m_NumEvents;
}

int EventHandler::phiOffset(int cham) const
{
    if(m_runNumber >  140){
      return m_phiOffset[cham-1];
    }
    else {
      return m_phiOffset[2];
    }
}

float EventHandler::triggerOffset1() const
{
    return m_triggerOffset1;
}

float EventHandler::triggerOffset2() const
{
    return m_triggerOffset2;
}

int EventHandler::SourceStatus() const
{
    return m_SourceStatus;
}

int EventHandler::VtrhEta() const
{
    return m_VtrhEta;
}

int EventHandler::VtrhPhi() const
{
    return m_VtrhPhi;
}

int EventHandler::TriggerType() const
{
    return m_TriggerType;
}

