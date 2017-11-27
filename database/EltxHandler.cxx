/* *****************************************************************
 * mods: changed array fileName to length 255 (was 50). S.Z. 1/3/02
 *       method openInputStream: added error message in case file
 *              open fails  S.Z. 5/3/02
 *
 * ***************************************************************** */



#include "database/EltxHandler.h"
#include "database/EltxFile.h"


EltxHandler* EltxHandler::m_EltxHandler = 0;

EltxHandler * EltxHandler::getEltxHandler() {
    if(!m_EltxHandler) {
        m_EltxHandler = new EltxHandler;
    }
    return m_EltxHandler;
}

EltxHandler::EltxHandler(){
  MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
  Memory->increase();
//CHECK
//  m_input=0;
  m_EltxHandler=this;
  load();
}

EltxHandler::~EltxHandler(){
  MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
  Memory->decrease();   
  if(!m_Eltxfiles.empty()) {
     std::vector<EltxFile*>::iterator  ir;
     for( ir=m_Eltxfiles.begin(); ir!=m_Eltxfiles.end(); ++ir) {
         delete *ir;
     }
  }
  while( !m_Eltxfiles.empty() ) {
     m_Eltxfiles.pop_back();
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
}

std::vector <EltxFile*> EltxHandler::Eltxfiles(){
  return m_Eltxfiles; 
}

int EltxHandler::tubeId(int tdc_id, int tdc_ch, int run) {
  if(!m_Eltxfiles.empty()) {
     std::vector<EltxFile*>::reverse_iterator  ir;
     for( ir=m_Eltxfiles.rbegin(); ir!=m_Eltxfiles.rend(); ++ir) {
         if((*ir)->isValid(run)) 
            return (*ir)->tubeId(tdc_id,tdc_ch);
     }
  }
  return 0;
}

int EltxHandler::stripId(int tdc_id, int tdc_ch, int run) {
  if(!m_Eltxfiles.empty()) {
     std::vector<EltxFile*>::reverse_iterator  ir;
     for( ir=m_Eltxfiles.rbegin(); ir!=m_Eltxfiles.rend(); ++ir) {
         if((*ir)->isValid(run)) 
            return (*ir)->stripId(tdc_id,tdc_ch);
     }
  }
  return 0;
}

int EltxHandler::stripIdX5(int tdc_id, int tdc_ch, int run) {
  if(!m_Eltxfiles.empty()) {
     std::vector<EltxFile*>::reverse_iterator  ir;
     for( ir=m_Eltxfiles.rbegin(); ir!=m_Eltxfiles.rend(); ++ir) {
         if((*ir)->isValid(run)) 
            return (*ir)->stripIdX5(tdc_id,tdc_ch);
     }
  }
  return 0;
}

void EltxHandler::set(EltxFile * newEltx) {
   m_Eltxfiles.push_back(newEltx);
}

void EltxHandler::load(){
   if(openInputStream()){  
      if(!m_inputList.empty()) {
         std::vector<std::ifstream*>::iterator it;
         for( it=m_inputList.begin(); it!=m_inputList.end(); ++it) {
            m_Eltxfiles.push_back(new EltxFile(*it));
         }
      }
   }
}

bool EltxHandler::openInputStream() {
    char * fileList = "input/eltxfiles";
    char fileName[255];
    std::ifstream input(fileList);
    if(!input) {
        std::cout << "EltxHandler::openInputStream could not open in file" << std::endl;
        std::cout << "              file not found " << fileList << std::endl;
        abort();
    } else {
        while(input >> fileName) {
            std::ifstream *file = new std::ifstream(fileName);
          if (!(*file))
             std::cout << "EltxHandler:: ERROR opening eltx mapping file: " << fileName << std::endl << std::endl; 
          else std::cout << "EltxHandler:: opened eltx mapping file "<< fileName << std::endl;
            m_inputList.push_back(new std::ifstream(fileName));
        }
        m_input = m_inputList.begin();
        return true;
    }
}



























