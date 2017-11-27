#include "database/DeadHotHandler.h"
#include "database/DeadHotFile.h"


DeadHotHandler* DeadHotHandler::m_DeadHotHandler = 0;

DeadHotHandler * DeadHotHandler::getDeadHotHandler() {
    if(!m_DeadHotHandler) {
        m_DeadHotHandler = new DeadHotHandler;
    }
    return m_DeadHotHandler;
}

DeadHotHandler::DeadHotHandler(){
  MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
  Memory->increase();
// commentato perche' non compila... necessario ?
//  m_input=0;
  m_DeadHotHandler=this;
  load();
}

DeadHotHandler::~DeadHotHandler(){
  MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
  Memory->decrease();   
/*
  if(!m_DeadHotfiles.empty()) {
     std::vector<DeadHotFile*>::iterator  ir;
     for( ir=m_DeadHotfiles.begin(); ir!=m_DeadHotfiles.end(); ++ir) {
         delete *ir;
     }
  }
  while( !m_DeadHotfiles.empty() ) {
     m_DeadHotfiles.pop_back();
  }
  */
  if(!m_inputList.empty()) {
     std::vector<std::ifstream*>::iterator it;
     for( it=m_inputList.begin(); it!=m_inputList.end(); ++it) {
         (*it)->close();
     }
  }
 /* 
  while( !m_inputList.empty() ) {
     m_inputList.pop_back();
  }
*/
   m_DeadHotfiles.clear();
   m_inputList.clear();
}

std::vector <DeadHotFile*> DeadHotHandler::DeadHotfiles(){
  return m_DeadHotfiles; 
}

bool DeadHotHandler::isDead(int stripId,int run) {
  if(!m_DeadHotfiles.empty()) {
     std::vector<DeadHotFile*>::reverse_iterator  ir;
     for( ir=m_DeadHotfiles.rbegin(); ir!=m_DeadHotfiles.rend(); ++ir) {
         if((*ir)->isValid(run)) 
            return (*ir)->isDead(stripId);
     }
  }
  return false;
}

bool DeadHotHandler::isDeadWindow(int stripId,int run) {
  if(!m_DeadHotfiles.empty()) {
     std::vector<DeadHotFile*>::reverse_iterator  ir;
     for( ir=m_DeadHotfiles.rbegin(); ir!=m_DeadHotfiles.rend(); ++ir) {
         if((*ir)->isValid(run)) 
            return (*ir)->isDeadWindow(stripId);
     }
  }
  return false;
}

void DeadHotHandler::set(DeadHotFile * newDeadHot) {
   m_DeadHotfiles.push_back(newDeadHot);
}

void DeadHotHandler::load(){
   if(openInputStream()){  
      if(!m_inputList.empty()) {
         std::vector<std::ifstream*>::iterator it;
         for( it=m_inputList.begin(); it!=m_inputList.end(); ++it) {
            m_DeadHotfiles.push_back(new DeadHotFile(*it));
         }
      }
   }
}

bool DeadHotHandler::openInputStream() {
    char * fileList = "input/deadhotfiles";
    char fileName[255];
    std::ifstream input(fileList);
    if(!input) {
        std::cout << "DeadHotHandler::openInputStream could not open in file" << std::endl;
        std::cout << "              file not found " << fileList << std::endl;
        abort();
    } else {
//        while(input.scan("%s",&fileName[0])) {
        while(input >> fileName) {
            std::ifstream *file = new std::ifstream(fileName);
          if (!(*file))
             std::cout << "DeadHotHandler:: ERROR opening deadhot mapping file" << fileName 
                       << std::endl << std::endl; 
          else std::cout << "DeadHotHandler:: opened deadhot mapping file "<< fileName << std::endl;
            m_inputList.push_back(new std::ifstream(fileName));
        }
        m_input = m_inputList.begin();
        return true;
    }
}



























