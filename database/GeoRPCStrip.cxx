#include <iostream>
#include <iomanip>
#include <cmath>
#include "database/GeoRPCStrip.h"

GeoRPCStrip::GeoRPCStrip(int idstrip, double xyzstrip[3], double striplen, 
             double stripwidth, int stripdir){

  MemoryChecker* Memory = MemoryChecker::getMemoryChecker(); 
  Memory->increase();    
                                                                                
  m_idstrip     =  idstrip;
  m_strip       =  idstrip % 100;
  m_stripplane  = (idstrip / 100) % 10;
  m_chamber     = (idstrip / 1000);
//  std::cout << " GeoRPCStrip " << idstrip << " " << m_strip << " " 
//  << m_stripplane << " " << m_chamber << std::endl;
  GeoRPCStrip::fillRPCStrip(xyzstrip,striplen,stripwidth,stripdir); 
}

GeoRPCStrip::GeoRPCStrip(int strip, int stripplane, int chamber,
             double xyzstrip[3], double striplen, 
             double stripwidth, int stripdir){
  MemoryChecker* Memory = MemoryChecker::getMemoryChecker(); 
  Memory->increase();    
  m_idstrip     = chamber * 1000 + stripplane * 100 + strip;
  m_strip       = strip;
  m_stripplane  = stripplane;
  m_chamber     = chamber;
  GeoRPCStrip::fillRPCStrip(xyzstrip,striplen,stripwidth,stripdir);
}

GeoRPCStrip::~GeoRPCStrip(){
  MemoryChecker* Memory = MemoryChecker::getMemoryChecker(); 
  Memory->decrease();    
  
}

int    GeoRPCStrip::idStrip()        {return m_idstrip;}
int    GeoRPCStrip::Strip()          {return m_strip;}
int    GeoRPCStrip::StripPlane()     {return m_stripplane;}
int    GeoRPCStrip::Chamber()        {return m_chamber;}
double GeoRPCStrip::StripLen()       {return m_striplen;}
double GeoRPCStrip::StripWidth()     {return m_stripwidth;}
int    GeoRPCStrip::StripDir()       {return m_stripdir;}
TVector3 GeoRPCStrip::xyzStrip()   {return m_xyzstrip;}


void  GeoRPCStrip::printOut(){
  std::cout << "RPCStrip :"   << m_idstrip << " " << m_strip
  << " " << m_stripplane << " " << m_chamber
  << " - x,y,z : " ;
  std::cout << m_xyzstrip.X() << " " << m_xyzstrip.Y() << 
  " " <<  m_xyzstrip.Z() << std::endl;
  std::cout << "RPCStrip length, width,dir : " << m_striplen ;
  std::cout << " " << m_stripwidth  << " " << m_stripdir  << std::endl;
}  


GeoRPCStrip * GeoRPCStrip::Ptr() {return this;}

void GeoRPCStrip::fillRPCStrip(double xyzstrip[3], double striplen, 
                         double stripwidth, 
                         int stripdir){
  m_striplen   = striplen;
  m_stripwidth = stripwidth;
  m_stripdir   = stripdir;
  m_xyzstrip.SetX(xyzstrip[0]); 
  m_xyzstrip.SetY(xyzstrip[1]); 
  m_xyzstrip.SetZ(xyzstrip[2]); 
}


void GeoRPCStrip::modCoord(double offset[3]){
  m_xyzstrip.SetX(m_xyzstrip.X()+offset[0]); 
  m_xyzstrip.SetY(m_xyzstrip.Y()+offset[1]);
  m_xyzstrip.SetZ(m_xyzstrip.Z()+offset[2]);
}
