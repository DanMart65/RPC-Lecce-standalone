/**
 * \class RPCChamber
 * Store pointers of StripPlanes belonging to a Chamber
 * \author E.Gorini
 * \version 0.01
 * \date 6.11.02
 **/
   
#ifndef RPCCHAMBER_H
#define RPCCHAMBER_H

#include <vector>
#include "event/SubEvent.h"
#include "tools/Utils.h"
#include "database/GeoRPCChamber.h"

class RPCStripPlane;

class RPCChamber : public SubEvent {
   
   public:

  /**
   * the constructor 
   */
      RPCChamber(int chamber, GeoRPCChamber * ml);

  /**
   * the destructor
   */
       ~RPCChamber();
       
       void clear();      					/**< clear after event processing*/
       void print();      					/**< print out the object contents*/

       int    Chamber(){return m_chamber;}         		 /**< Get Chamber number */
       int    nStripPlanes(){return m_stripplanePtr.size();}    /**< Get number of strip Planes */
       int    nGaps(){return m_geometry->nGaps();}		/**< Get number of Gaps */
       int    nLayers(){return m_geometry->nLayers();}      	/**< Get number of Layers */
       GeoRPCChamber * geo(){return m_geometry;} 		/**< Get pointer to geometry */
       
       RPCStripPlane * stripplanePtr(int jplane); /**< Get pointer to the j-th Plane */
       RPCStripPlane * findStripPlane( int k ); /**< Get pointer to plane k */
       RPCChamber * Ptr();  /**< Get pointer to chamber */
       
       std::vector<RPCStripPlane *> 	GapPlanes(int jgap);
       std::vector<RPCStripPlane *> 	LayerPlanes(int jlayer);        
       std::vector<int>  		GapNumbers(int layer);
   private:   
   // needed ? is in geometry !!!
       int m_chamber;
       GeoRPCChamber *    m_geometry;            
       std::vector<RPCStripPlane *> m_stripplanePtr;  /**< pointer to the planes */

};

#endif // RPCCHAMBER_H
