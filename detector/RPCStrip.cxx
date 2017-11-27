#include "detector/RPCStrip.h"
#include "database/GeoRPCStrip.h"
#include <math.h>

RPCStrip::RPCStrip(GeoRPCStrip * geo, int time):RPCHit() {

  MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
  Memory->increase();   
  
  m_geo=geo;
// stripid is a unique identifier for the strip in the setup:
// 1000 * chamber_number + 100 * stripplane + strip
  int stripid=geo->idStrip(); 
  m_strip=stripid%100;
  m_plane=(stripid / 100) % 10;
  m_chamber=(stripid / 1000) ;
  m_time = time;
}

RPCStrip::~RPCStrip(){
   MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
   Memory->decrease();   
}

void RPCStrip::print(){
  std::cout << "RPCStrip: Chamber " << m_chamber << " Plane " << m_plane ; 
  std::cout << " strip " << m_strip  << " time " << m_time << std::endl;
}

int RPCStrip::strip()  {return m_strip;}
int RPCStrip::firstStrip()  {return m_strip;}
int RPCStrip::plane()  {return m_plane;}
int RPCStrip::chamber() {return m_chamber;}
int RPCStrip::size() {return 1;}
int RPCStrip::time(){return m_time;}

RPCStrip * RPCStrip::ptr() {
  return this; 
}


GeoRPCStrip * RPCStrip::geo() {
  return m_geo; 
}

double   RPCStrip::X() { geo()->xyzStrip().X(); }
double   RPCStrip::Y() { geo()->xyzStrip().Y(); }
double   RPCStrip::Z() { geo()->xyzStrip().Z(); }
double   RPCStrip::Err() { geo()->StripWidth()/sqrt(12.); }
int   RPCStrip::stripDir() { geo()->StripDir(); }
