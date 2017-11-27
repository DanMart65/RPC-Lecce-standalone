#include <cmath> 
#include <iostream>
#include <iomanip>
#include "database/GeoRPCStripPlane.h"
#include "database/GeoRPCStrip.h"
#include "TVector3.h"
// TODO
// one should compute low and high limit for each strip and store them for later use

GeoRPCStripPlane::GeoRPCStripPlane(int stripplane, int chamber, int nstrips,
                       std::vector<GeoRPCStrip *> gstriplist, double striplen, 
                       double stripwidth,int stripdir, std::string name):
m_stripplane(stripplane),m_chamber(chamber),m_nstrips(nstrips),
m_striplen(striplen),m_stripwidth(stripwidth),m_stripdir(stripdir),m_name(name)   		       

{
  MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
  Memory->increase(); 
  m_idstripplane = chamber * 10 + stripplane;
  for (int j=0; j<nstrips; j++){m_stripPtr.push_back(gstriplist[j]);}
// set first and last instrumented strip to 1 and nstrips by default
// this can be changed in the Event Handler (at run time...)
  m_fis=1;
  m_lis=nstrips;
  m_low=-9999;
  m_high=-9999;
}

GeoRPCStripPlane::~GeoRPCStripPlane() {
  MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
  Memory->decrease(); 
  if(!m_stripPtr.empty()) {
    std::vector<GeoRPCStrip*>::iterator  is;
    for( is=m_stripPtr.begin(); is!=m_stripPtr.end(); ++is) {
      delete *is;
    }
  }
  while( !m_stripPtr.empty() ) {
    m_stripPtr.pop_back();
  }
}

int    GeoRPCStripPlane::idStripPlane()  {return m_idstripplane;}
int    GeoRPCStripPlane::StripPlane()    {return m_stripplane;}
int    GeoRPCStripPlane::Chamber()       {return m_chamber;}
int    GeoRPCStripPlane::Nstrips()       {return m_nstrips;}
double GeoRPCStripPlane::StripLen()      {return m_striplen;}
double GeoRPCStripPlane::StripWidth()    {return m_stripwidth;}
int    GeoRPCStripPlane::StripDir()      {return m_stripdir;}
int    GeoRPCStripPlane::gap()		 {return (m_stripplane-1)/2+1;}
int    GeoRPCStripPlane::layer()	 {return (gap()-1)%2+1;}

std::string    GeoRPCStripPlane::Name()          {return m_name;}

void    GeoRPCStripPlane::ReplaceName(std::string name){m_name.replace(0,8,name);}

//HepTransform3D GeoRPCStripPlane::transform() {return m_transform;}
GeoRPCStrip * GeoRPCStripPlane::stripPtr(int jstrip) {return m_stripPtr[jstrip];}

/*
GeoRPCStrip * GeoRPCStripPlane::findStrip(int stripId) 
{
  if(!m_stripPtr.empty()) {
    std::vector<GeoRPCStrip*>::iterator  is;
    for (is=m_stripPtr.begin(); is!=m_stripPtr.end(); ++is) {
      if((*is)->idStrip() == stripId) return (*is); 
    }
  }
  return 0;
}
*/

GeoRPCStrip * GeoRPCStripPlane::findStrip(int stripId) 
{
// this works because strips are ordered by construction
  if(!m_stripPtr.empty()) {
    int ind=stripId-(stripId/100)*100-1;
    if(ind<m_stripPtr.size() &&
       m_stripPtr[ind]->idStrip() == stripId) return m_stripPtr[ind]; 
  }
  return 0;
}


/*
GeoRPCStrip * GeoRPCStripPlane::findStripAt(double coord) 
{
  if(!m_stripPtr.empty()) {
    std::vector<GeoRPCStrip*>::iterator  is;
    for (is=m_stripPtr.begin(); is!=m_stripPtr.end(); ++is) {
// compute strip limits        
        HepPoint3D mcoord=(*is)->xyzStrip(); 
//  higher limit (aggiungere mezza strip....            
        double highlimit=mcoord.z()+0.5*(*is)->StripWidth();
        if((*is)->StripDir()==0)highlimit=mcoord.x()+0.5*(*is)->StripWidth();
//  lower limit             
        double lowlimit=mcoord.z()-0.5*(*is)->StripWidth();
        if((*is)->StripDir()==0)lowlimit=mcoord.x()-0.5*(*is)->StripWidth();        
        
        if( coord >= lowlimit && coord <= highlimit) return (*is); 
    }
  }
  return 0;
}
*/

GeoRPCStrip * GeoRPCStripPlane::findStripAt(double coord) 
{
  if(!m_stripPtr.empty()) {
// compute predicted strip..
    float low=lowlimit();
    float high=highlimit();
    if(coord < low || coord > high)return 0;
    int istheone=(coord-low)/m_stripwidth; 
    return m_stripPtr[istheone];   
  }
  return 0;
}


void  GeoRPCStripPlane::printOut(){
  std::cout << "Strip plane : " << m_name << std::endl;
  std::cout << "id : " << m_stripplane << " chamber : " << m_chamber;
  std::cout << " # of strips, strip length, width and dir. : " << m_nstrips  << " ";
  std::cout << m_striplen << " " << m_stripwidth << " " << m_stripdir << std::endl;
}


GeoRPCStripPlane * GeoRPCStripPlane::Ptr() {return this;}


/*************************************************************************
 * get strip lower limit
 *************************************************************************/
double GeoRPCStripPlane::lowlimit(int strip){           
// get pointer to strip
    GeoRPCStrip* theStrip=stripPtr(strip-1);
    double sw = StripWidth();
    double low;
// compute lower limit      
    TVector3 coord=theStrip->xyzStrip();   
    low=coord.Z();
    if(StripDir()==0)low=coord.X();
    low-=0.5*sw;
    return low;
}

/*************************************************************************
 * get strip higher limit 
 *************************************************************************/
double GeoRPCStripPlane::highlimit(int strip){           
// get pointer to strip
    GeoRPCStrip* theStrip=stripPtr(strip-1);
    double sw = StripWidth();
    double high;
// compute lower limit      
    TVector3 coord=theStrip->xyzStrip();                   
    high=coord.Z();
    if(StripDir()==0)high=coord.X();
    high+=0.5*sw;
    return high;
}

 /*************************************************************************
 * get plane lower limit 
 *************************************************************************/
double GeoRPCStripPlane::lowlimit(){           
// get pointer to first strip
  return lowlimit(1);
}
                                    
 /*************************************************************************
 * get plane lower limit 
 *************************************************************************/
double GeoRPCStripPlane::highlimit(){           
// get pointer to first strip
  return highlimit(Nstrips());
}

/*************************************************************************
 * get strip coordinates 
 *************************************************************************/
TVector3 GeoRPCStripPlane::StripCoord(int strip){           
// get pointer to strip
    GeoRPCStrip* theStrip=stripPtr(strip-1);   
    return theStrip->xyzStrip();                   
    
}

 /*************************************************************************
 * set First instrumented strip 
 *************************************************************************/
void GeoRPCStripPlane::setFirstInstrumentedStrip(int fis){m_fis=fis;}
 /*************************************************************************
 * set First instrumented strip 
 *************************************************************************/
void GeoRPCStripPlane::setLastInstrumentedStrip(int lis){m_lis=lis;}

 /*************************************************************************
 * Check if coordinate is inside the plane geometry 
 *************************************************************************/
bool GeoRPCStripPlane::InsideGeometry(double coord){           
  return ( coord >= lowlimit() && coord <= highlimit());
}

 /*************************************************************************
 * Check if coordinate is inside the instrumented strip (more restictive than Geometry)
 *************************************************************************/
bool GeoRPCStripPlane::InsideInstrumented(double coord){           
  return ( coord >= lowlimit(m_fis) && coord <= highlimit(m_lis));
}


 /*************************************************************************
 * Check if coordinate is inside the plane but with an enlargment due to 
 * track resolution
 *************************************************************************/
bool GeoRPCStripPlane::InsideFiducial(double coord){   
          
  return ( coord >= ( lowlimit(m_fis)+m_stripwidth) && 
           coord <= (highlimit(m_lis)-m_stripwidth)    );
}
