/** 
 * \class RPCStrip 
 * class containing all the relevant information for an RPC hit strip 
 * \author M. Primavera, E.Gorini
 * \date 27.07.00
 */

#ifndef RPCSTRIP_H
#define RPCSTRIP_H

#include "tools/Utils.h"
#include "detector/RPCHit.h"

class GeoRPCStrip; 

class RPCStrip : public RPCHit {

 public:

  RPCStrip(GeoRPCStrip * geo, int time); 
  
  ~RPCStrip();             /**< destructor */ 

  int strip();      /**< return the strip number */ 
  int plane();     /**< return the plane number */ 
  int chamber();   /**< return the chamber number */ 
  int size();   /**< return the size of the cluster (1 in this case)  */
  int stripDir();   /**< return the orientation of the strip  */ 
  RPCStrip * ptr();    /**< return the pointer to the hit */ 
  GeoRPCStrip * geo();/**< return the pointer to geometrical strip */

  double X();   /**< X coordinate */
  double Y();  /**< Y coordinate */
  double Z();  /**< Z coordinate */
  double Err();/**< Error on measured  coordinate */
  int firstStrip();
  int time();
  void print();
 private:

  int m_strip;           /**< the strip number */
  int m_plane;          /**< the plane number */
  int m_chamber;        /**< the chamber number */
// E.G. Added time 20/9/02
  int m_time;           /* latch time            */
  GeoRPCStrip * m_geo;   /**< the strip geometry */
};

// sort function : descending time
class TimeSort {
   public:
      bool operator () (RPCStrip * a, RPCStrip * b){
           if( a->time() > b->time())return true;
                                else return false;
      }
};

#endif //RPCSTRIP_H
