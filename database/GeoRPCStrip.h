/**
 * \class GeoRPCStrip
 * It builds and provides the RPC strip positions
 * \author E. Gorini,M.Primavera
 * \version 0.0
 * \date 21.07.00
 **/

#ifndef GEORPCSTRIP_H
#define GEORPCSTRIP_H

#include <vector>
#include "TVector3.h"
#include "tools/Utils.h"


class GeoRPCStrip {
    public:

  /**
   * the constructor  
   * exists in two versions :
   * a) 
   */
       GeoRPCStrip(int idstrip, double xyzstrip[3], double striplen, 
             double stripwidth, int stripdir); // Initialization
  /** 
   * b)
   */
       GeoRPCStrip(int strip, int stripplane, int chamber,
             double xyzstrip[3], double striplen, 
             double stripwidth, int stripdir); // Initialization
  /**
   * the destructor
   * must take care of deleting the strip 
   */
       ~GeoRPCStrip();

       int    idStrip();              /**< Get the strip identifier */
       int    Strip();                /**< Get the strip number */
       int    StripPlane();               /**< Get the plane number */
       int    Chamber();             /**< Get the Chamber number */
       double StripLen();             /**< Get the stripLen (mm) */
       double StripWidth();             /**< Get the stripwidth (mm) */
       int StripDir();                /**< Get the strip dir. (mm) */
       TVector3 xyzStrip();         /**< Get the xyzstrip (mm) */

  
       void printOut();           /**< print out  */
       GeoRPCStrip * Ptr();        /**< Get pointer to strip */
// poco elegante perche modifica geometria.
       void modCoord(double offset[3]);
    private:
  /**
   * the real constructor,
   * called by the GeoRPCStrip(...)
   */
       void  fillRPCStrip(double xyzstrip[3], double striplen, 
                         double stripwidth, 
                         int stripdir);

    private:
       int    m_idstrip;     /**< strip identifier */
       int    m_strip;       /**< strip number */
       int    m_stripplane;  /**< plane number */
       int    m_chamber;     /**< chamber number */
       double m_striplen;    /**< strip length */
       double m_stripwidth;  /**< strip width */
       int    m_stripdir;    /**< strip dir. */
       TVector3  m_xyzstrip;    /**< position in global ref. frame */ 

};
 
#endif // GEORPCSTRIP_H
