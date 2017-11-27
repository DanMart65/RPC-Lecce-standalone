/**
 * \class RPCStripPlane
 * the RPC SubEvent, the central object for RPC reconstruction
 * \author M. Primavera, E.Gorini
 * \date 18.07.00
 */


#ifndef RPCSTRIPPLANE_H
#define RPCSTRIPPLANE_H

#include <vector>
#include "event/SubEvent.h"
#include "tools/Utils.h"
#include "detector/RPCStrip.h"
#include "detector/RPCCluster.h"

class GeoRPCStripPlane;

class RPCStripPlane : public SubEvent {

   public:

      
      /** 
       * the constructor
       * needs the identifier and the pointer to the geometrical description
       */
      RPCStripPlane(int Id, GeoRPCStripPlane * ml);     

      virtual ~RPCStripPlane();  /**< the destructor */

      virtual void clear();      /**< clear after event processing*/

      void addHit(RPCStrip * hit); /**< add an hit to the hitList */
      
      std::vector<RPCStrip*> CompleteHitList(); /**< return the Complete hitList */

      std::vector<RPCStrip*> hitList(); /**< return the hitList clustered in time */
            
      void Consolidate (int timemin, int timemax);
      
      std::vector<RPCCluster*> clusList();
      
      int NearestClusterAt(double estrap);
      
      virtual void print();      /**< print out the subevent contents*/
      
      void printFull();
      
      GeoRPCStripPlane * RPCgeometry(); /**< pointer to the geom. description */
   
   private:    
      std::vector<RPCStrip*> m_fullhitList; /**< list of all RPCStrips  */
      std::vector<RPCStrip*> m_hitList; /**< list of time clusterized RPCStrips */
      std::vector<RPCCluster *> m_clusList; /**< list of all clusters */
      GeoRPCStripPlane * m_geometry; /**< where to find the geometry */
      void sortstrips(); /**< sort the hits */
      void TimeClusterize(); /**< clusterize strips in time  **/
      void Clusterize (int timemin, int timemax);

      friend class StripCompare;
};

class StripCompare {
   public:
      bool operator () (RPCStrip * a, RPCStrip * b){
         if(a->strip() < b->strip()){
            return true; // based on strip Number
         }
         else if(a->strip() == b->strip()){
// check time if equal
           if( a->time() > b->time()){
             return true;
           }
           else {
            return false;
           }
         }  
         else {
// bigger         
           return false;
         }
      }
};

#endif // RPCSTRIPPLANE_H
