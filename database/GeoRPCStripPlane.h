/**
 * \class GeoRPCStripPlane
 * It builds and provides the geometry for a RPC chamber
 * \author M. Primavera, E.Gorini
 * \version 0.02
 * \date 18.07.00
 **/
   
#ifndef GEORPCSTRIPPLANE_H
#define GEORPCSTRIPPLANE_H

#include <vector>
#include <string>
#include "tools/Utils.h"
#include "TVector3.h"

class GeoRPCStrip;

class GeoRPCStripPlane {
   public:

  /**
   * the constructor 
   */
      GeoRPCStripPlane(int stripplane, int chamber, int nstrips,
                       std::vector<GeoRPCStrip *> gstriplist, double striplen, 
                       double stripwidth,int stripdir, std::string name);

  /**
   * the destructor
   */
      ~GeoRPCStripPlane();
       
       int    idStripPlane();    /**< Get Plane id (ch*10 + plane) */
       int    StripPlane();      /**< Get Plane number */       
       int    Chamber();         /**< Get Chamber number */
       int    Nstrips();         /**< Get number of strips */
       double StripLen();        /**< Get strip len */
       double StripWidth();      /**< Get strip width */
       int    StripDir();        /**< Get strip direction */
       int gap();
       int layer();
       std::string Name();            //get StripPlkane Name
       void ReplaceName(std::string name);
//       HepTransform3D transform();   /**< Get the ref.sys. transformation */

       GeoRPCStrip * stripPtr(int jstrip); /**< Get pointer to the j-th strip */
       GeoRPCStrip * findStrip(int stripId); /**< Get pointer to strip stripId */
       GeoRPCStrip * findStripAt(double coord); /**< Get pointer to strip containing coord */
       double lowlimit(int strip);
       double lowlimit();
       double highlimit(int strip);
       double highlimit();
       
       void setFirstInstrumentedStrip(int fis);
       void setLastInstrumentedStrip(int lis);
       bool InsideGeometry(double coord);      
       bool InsideInstrumented(double coord);  
       bool InsideFiducial(double coord);   
         

       TVector3 StripCoord(int strip);
       void printOut();           /**< print out  */
  /**
   * it updates the links -  
   * to be called for each run
   */
//       void  updateLinks(int run);

  /**
   * it transform into another ref. sys.
   */
//       void transform(int flag, HepPoint3D v1, HepPoint3D v2, HepPoint3D v3, 
//		      HepPoint3D v1p, HepPoint3D v2p, HepPoint3D v3p);

       GeoRPCStripPlane * Ptr();  /**< Get pointer to plane */
                                 /**< list of tubes on a track path */
   private:
       int    m_idstripplane;  
       int    m_stripplane;    
       int    m_chamber;       
       int    m_nstrips;       
       double m_striplen;      
       double m_stripwidth;        
       int    m_stripdir;
       int m_fis;
       int m_lis;    
	   double m_low;
	   double m_high;
       std::string m_name;   
//       HepTransform3D m_transform;           /**< rotation and translation */
       std::vector<GeoRPCStrip *> m_stripPtr;  /**< pointer to the plane */
};

#endif // GEORPCSTRIPPLANE_H
