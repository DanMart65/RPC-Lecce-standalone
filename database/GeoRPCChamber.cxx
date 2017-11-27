#include <cmath> 
#include <iostream>
#include <iomanip>
#include "database/GeoRPCChamber.h"
#include "database/GeoRPCGap.h"

GeoRPCChamber::GeoRPCChamber(std::vector<GeoRPCGap *> gaps, int chamber, std::string name):
m_chamber(chamber),m_name(name)   		       

{
  MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
  Memory->increase();

  for (int j=0; j<gaps.size(); j++){m_gapPtr.push_back(gaps[j]);}
// set number of gaps
  m_ngaps=gaps.size();
// set number of stripplanes  
  m_nstripplanes=0;
  for(int k=0; k!=m_gapPtr.size(); k++)m_nstripplanes+=m_gapPtr[k]->nStripPlanes(); 
// set number of layers
  m_nlayers=2;   //m_ngaps/2;
}

GeoRPCChamber::~GeoRPCChamber() {
  MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
  Memory->decrease(); 
   if(!m_gapPtr.empty()) {
 
     std::vector<GeoRPCGap*>::iterator  is;
     for( is=m_gapPtr.begin(); is!=m_gapPtr.end(); ++is) {
       delete *is;
     }
   }
   while( !m_gapPtr.empty() ) {
     m_gapPtr.pop_back();
   }
}


int    GeoRPCChamber::Chamber()       {return m_chamber;}

int    GeoRPCChamber::nStripPlanes(){return m_nstripplanes;}

int    GeoRPCChamber::nGaps()       {return m_ngaps;}

int    GeoRPCChamber::nLayers()       {return m_nlayers;}

GeoRPCStripPlane * GeoRPCChamber::stripplanePtr(int j) {
  int l=0;
  for(int i=0; i!=m_gapPtr.size(); i++) {
    GeoRPCGap * gp = m_gapPtr[i];        
    for(int k=0; k!=gp->nStripPlanes(); k++) {
      GeoRPCStripPlane * pp=gp->stripplanePtr(k);
      if(j==l)return pp;
      l++;
    }  
  }
  return 0;
}

GeoRPCGap * GeoRPCChamber::gapPtr(int j) {return m_gapPtr[j];}

void  GeoRPCChamber::printOut(){
  std::cout <<  " Chamber : " << m_chamber << ", Name: " << m_name;
  std::cout << ", # of gaps: " << m_gapPtr.size() << std::endl;
// now prints info on the gaps
  if(!m_gapPtr.empty()) {
    std::vector<GeoRPCGap *>::iterator  is;
    for (is=m_gapPtr.begin(); is!=m_gapPtr.end(); ++is) {
      (*is)->printOut();     
    }
  }
  
}


GeoRPCChamber* GeoRPCChamber::Ptr() {return this;}

void    GeoRPCChamber::ReplaceName(std::string name){
  m_name.replace(0,8,name);
  for(int i=0; i!=m_gapPtr.size(); i++)m_gapPtr[i]->ReplaceName(name);
}
