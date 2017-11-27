/** 
 * \class RPCCluster 
 * class containing all the relevant information for an RPC hit strip 
 * \author E.Gorini, M. Primavera
 * \date 27.07.00
 */

#ifndef RPCCLUSTER_H
#define RPCCLUSTER_H

#include "tools/Utils.h"
#include <vector> 
#include "RPCHit.h"

class RPCStrip; 

class RPCCluster : public RPCHit {

 public:

  /** 
   * initialized constructor 
   */
  RPCCluster(std::vector<RPCStrip *> stripList); 


  ~RPCCluster();             /**< destructor */ 

  void print() ;    /**< print out the hit content */

  int firstStrip();      /**< return the number of the first strip*/ 
  int lastStrip();      /**< return the number of the last*/ 
  int size() ;      /**< return the number of strips */ 
  int plane() ;     /**< return the plane number */ 
  int chamber() ;   /**< return the chamber number */ 
  int stripDir() ;   /**< return the plane orientation */ 
  int idsp(){return m_idsp;} ;   /**< return id of StripPlane */ 
  int layer(){return m_layer;} ;   /**< returns layer */ 
  int gap(){return m_gap;} ;   /**< returns gap */   
  
  RPCCluster * ptr();    /**< return the pointer to the hit */ 
  std::vector<RPCStrip *> stripPtr(); /**< return the strip list */

  virtual double X();  /**< hit X coordinate */
  virtual double Y();  /**< hit X coordinate */
  virtual double Z();  /**< hit X coordinate */
  virtual double Err();/**< error on measured position */

 private:
  std::vector<RPCStrip *> m_list; /**< the list of strips in this cluster */
  int m_first;         /**< the first strip */
  int m_last;          /**< the first strip */
  int m_size;          /**< number of strips */
  int m_plane;         /**< the plane number */
  int m_chamber;       /**< the chamber number */
  int m_stripDir;       /**< the Plane orientation */
  int m_idsp;          /**< the StripPlane id */
  int m_layer;
  int m_gap;
  double m_x;/**< hit X coordinate */
  double m_y;/**< hit Y coordinate */
  double m_z;/**< hit Z coordinate */
  double m_err;          /**< error on the measured coordinate */
};
#endif //RPCCLUSTER_H
