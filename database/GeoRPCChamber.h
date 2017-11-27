/**
 * \class GeoRPCChamber
 * Store pointers of StripPlanes belonging to a Chamber
 * \author E.Gorini
 * \version 0.01
 * \date 26.09.02
 **/
   
#ifndef GEORPCCHAMBER_H
#define GEORPCCHAMBER_H

#include <vector>
#include <string>
#include "tools/Utils.h"
#include "database/GeoRPCStripPlane.h"
#include "database/GeoRPCGap.h"

class GeoRPCChamber {
   public:

  /**
   * the constructor 
   */
      GeoRPCChamber(std::vector<GeoRPCGap *> gaps, int chamber, std::string name);

  /**
   * the destructor
   */
      ~GeoRPCChamber();
       int    Chamber();         /**< Get Chamber number */
       int    nStripPlanes();    /**< Get number of strip Planes */
       int    nGaps();    /**< Get number of Gaps */
       int    nLayers();    /**< Get number of Layers */
       GeoRPCStripPlane * stripplanePtr(int jplane); /**< Get pointer to the j-th Plane */
       GeoRPCGap * gapPtr(int jgap); /**< Get pointer to the j-th gap */
       void printOut();           /**< print out  */
       GeoRPCChamber * Ptr();  /**< Get pointer to chamber */
       std::string Name() const {return m_name;};
// fare protected o che...
       void ReplaceName(std::string name);
   private:   
       int    m_chamber;
       int    m_nstripplanes;
       int    m_ngaps;
       int    m_nlayers;   
       std::string m_name;         
       std::vector<GeoRPCGap *> m_gapPtr;  /**< list of pointers to gaps */
       
};

#endif // GEORPCCHAMBER_H
