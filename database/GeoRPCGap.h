/**
 * \class GeoRPCGap
 * Store pointers of StripPlanes belonging to a Chamber
 * \author E.Gorini
 * \version 0.01
 * \date 26.09.02
 **/
   
#ifndef GEORPCGAP_H
#define GEORPCGAP_H

#include <vector>
#include <string>
#include "tools/Utils.h"

class GeoRPCStripPlane;

class GeoRPCGap {
   public:

  /**
   * the constructor 
   */
      GeoRPCGap(std::vector<GeoRPCStripPlane *> gplanes, int gap, std::string name);

  /**
   * the destructor
   */
      ~GeoRPCGap();
       int    Gap();         /**< Get Gap number */
       int    nStripPlanes();    /**< Get number of strip Planes */
       GeoRPCStripPlane * stripplanePtr(int jplane); /**< Get pointer to the j-th Plane */
       void printOut();           /**< print out  */
       GeoRPCGap * Ptr();  /**< Get pointer to gap */
       std::string Name() const {return m_name;};
       void ReplaceName(std::string name);
   private:   
       int    m_gap;            
       std::string m_name; 
       std::vector<GeoRPCStripPlane *> m_stripplanePtr;  /**< pointer to the plane */
};

#endif // GEORPCGAP_H
