#include <cmath> 
#include <iostream>
#include <iomanip>
#include "database/GeoRPCGap.h"
#include "database/GeoRPCStripPlane.h"

GeoRPCGap::GeoRPCGap(std::vector<GeoRPCStripPlane *> gplanes, int gap, std::string name):
m_gap(gap),m_name(name)    		       

{
  MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
  Memory->increase(); 

  for (int j=0; j<gplanes.size(); j++){m_stripplanePtr.push_back(gplanes[j]);}
}

GeoRPCGap::~GeoRPCGap() {
  MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
  Memory->decrease(); 
  if(!m_stripplanePtr.empty()) {
 
     std::vector<GeoRPCStripPlane*>::iterator  is;
     for( is=m_stripplanePtr.begin(); is!=m_stripplanePtr.end(); ++is) {
       delete *is;
     }
   }
   while( !m_stripplanePtr.empty() ) {
     m_stripplanePtr.pop_back();
   }
}


int    GeoRPCGap::Gap()       {return m_gap;}

int    GeoRPCGap::nStripPlanes()       {return m_stripplanePtr.size();}

GeoRPCStripPlane * GeoRPCGap::stripplanePtr(int j) {return m_stripplanePtr[j];}


void  GeoRPCGap::printOut(){
  std::cout <<  " Gap : " << m_gap << ", Name: " << m_name;
  std::cout << " # of strip planes: " << m_stripplanePtr.size()  << " " << std::endl;
// now prints info on the StripPlanes
  if(!m_stripplanePtr.empty()) {
    std::vector<GeoRPCStripPlane *>::iterator  is;
    for (is=m_stripplanePtr.begin(); is!=m_stripplanePtr.end(); ++is) {
      (*is)->printOut();     
    }
  }
  
}


GeoRPCGap* GeoRPCGap::Ptr() {return this;}


void    GeoRPCGap::ReplaceName(std::string name){
  m_name.replace(0,8,name);
  for(int k=0; k!=nStripPlanes(); k++)stripplanePtr(k)->ReplaceName(name);
}
