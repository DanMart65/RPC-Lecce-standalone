#include <cmath> 
#include <iostream>
#include <iomanip>
#include "detector/RPCChamber.h"
#include "detector/RPCStripPlane.h"

RPCChamber::RPCChamber(int chamber, GeoRPCChamber * ml):SubEvent(4,chamber),m_chamber(chamber),m_geometry(ml)
{
  MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
  Memory->increase(); 

  for(int k=0; k!=m_geometry->nGaps(); k++) {
     GeoRPCGap * gapPtr = m_geometry->gapPtr(k);
     for(int l=0; l!=gapPtr->nStripPlanes(); l++) {
       GeoRPCStripPlane * gplanePtr=gapPtr->stripplanePtr(l);        
       m_stripplanePtr.push_back(new RPCStripPlane(gplanePtr->idStripPlane(),gplanePtr)); 
     }  
  }
// now fills some additonal information for chamber
  
}

RPCChamber::~RPCChamber() {
  MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
  Memory->decrease(); 
}


RPCStripPlane * RPCChamber::stripplanePtr(int j) {return m_stripplanePtr[j];}

void RPCChamber::clear(){
  if(!m_stripplanePtr.empty()) {
    std::vector<RPCStripPlane *>::iterator  is;
    for (is=m_stripplanePtr.begin(); is!=m_stripplanePtr.end(); is++) {
      (*is)->clear();     
    }
  }

}

void  RPCChamber::print(){
  std::cout <<  " Chamber : " << m_chamber;
  std::cout << " # of strip planes: " << m_stripplanePtr.size()  << " " << std::endl;
// now prints info the StripPlanes
  if(!m_stripplanePtr.empty()) {
    std::vector<RPCStripPlane *>::iterator  is;
    for (is=m_stripplanePtr.begin(); is!=m_stripplanePtr.end(); ++is) {
      (*is)->print();     
    }
  }
  
}

RPCStripPlane * RPCChamber::findStripPlane(int k){
  if(!m_stripplanePtr.empty()) {
    std::vector<RPCStripPlane *>::iterator  is;
    for (is=m_stripplanePtr.begin(); is!=m_stripplanePtr.end(); ++is) {
      if( (*is)->RPCgeometry()->StripPlane() == k)return (*is);
    }
  }  
  return 0;    
}

std::vector<RPCStripPlane *>  RPCChamber::GapPlanes(int jgap){
  std::vector<RPCStripPlane *> plist;
  if(!m_stripplanePtr.empty()) {
    std::vector<RPCStripPlane *>::iterator  is;    
    for (is=m_stripplanePtr.begin(); is!=m_stripplanePtr.end(); ++is) {
      if( (*is)->RPCgeometry()->gap()==jgap)plist.push_back(*is);
    }
  }  
  return plist;    
}

std::vector<int>  RPCChamber::GapNumbers(int layer){
  std::vector<int> plist;
  for(int k=0; k!=m_geometry->nGaps(); k++) {
     GeoRPCGap * gapPtr = m_geometry->gapPtr(k);
       GeoRPCStripPlane * geo=gapPtr->stripplanePtr(0);  
       if( geo->layer()==layer)plist.push_back(geo->gap());  
  }
  return plist;    
}



std::vector<RPCStripPlane *>  RPCChamber::LayerPlanes(int jlayer){
  std::vector<RPCStripPlane *> plist;
  if(!m_stripplanePtr.empty()) {
    std::vector<RPCStripPlane *>::iterator  is;    
    for (is=m_stripplanePtr.begin(); is!=m_stripplanePtr.end(); is++) {
      if( (*is)->RPCgeometry()->layer()==jlayer)plist.push_back(*is);
    }
  }  
  return plist;    
}
RPCChamber* RPCChamber::Ptr() {return this;}
