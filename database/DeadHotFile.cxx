#include "control/CalibrationManager.h"
#include "database/DeadHotFile.h"

DeadHotFile::DeadHotFile(std::ifstream * inputFile) 
{
   MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
   Memory->increase();   

   char tag[6],datenam[10],timenam[5];
   char chComm[]   = "#";
   int idStrip;
   while( (*inputFile) >> tag) {                                               
        while (strncmp(tag,chComm,1)==0) {
          inputFile->ignore(500,'\n');
          (*inputFile) >> tag;
        }      
        if(strncmp(tag,"valid",6)==0) {
           (*inputFile) >> m_firstRun >> m_lastRun ;
        } if(strncmp(tag,"dead",4)==0) {
            (*inputFile) >> idStrip;
            deadList.push_back(idStrip);            
        } 
   }
   inputFile->close();   
}

DeadHotFile::~DeadHotFile() 
{
  MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
  Memory->decrease();   
  if(!deadList.empty()) {
     std::vector<int>::iterator  it;
// ?? devo cancellarli, non sono puntatori... ?? 
//     for( it=deadList.begin(); it!=deadList.end(); ++it) {
//         delete it;
//     }
  }
  while(!deadList.empty()){
         deadList.pop_back();
  }
}


bool DeadHotFile::isValid(int run) const 
{
   if(run > m_lastRun || run < m_firstRun) {
      return false;
   }  
   return true;
}

bool DeadHotFile::isDead(int stripId) 
{
  if(!deadList.empty()) {
     std::vector<int>::iterator  it;
     for( it=deadList.begin(); it!=deadList.end(); it++){
//cout << "cerco " << *it << " confronto con " << stripId << endl;
       if( (*it)==stripId)return true;
     }
  }
  return false;
}
bool DeadHotFile::isDeadWindow(int stripId) 
{
  if(!deadList.empty()) {
     std::vector<int>::iterator  it;
     for( it=deadList.begin(); it!=deadList.end(); it++){
//cout << "cerco " << *it << " confronto con " << stripId << endl;
       if( (*it)==stripId || (*it)==(stripId+1) || ((*it)==stripId-1))return true;
     }
  }
  return false;
}
