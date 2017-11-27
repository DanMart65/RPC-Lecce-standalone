/**
 * \class GeometryHandler
 * a class for the handling of MDT & RPC Geometry (see EventHandler) 
 *
 * \author P.Bagnaia,E. Gorini,M. Primavera
 * \date 21.07.2000
 * \mods: changed length of array filename to 255. S.Z. 1/3/2002   
*/ 

#ifndef GEOMETRYHANDLER_H
#define GEOMETRYHANDLER_H

#include <vector>
#include <string>
#include "tools/Utils.h"
#include <TSQLServer.h>

//class GeoMDTMultiLayer;
//class GeoMDTTube;

class GeoRPCChamber;
class GeoRPCGap;
class GeoRPCStripPlane;
class GeoRPCStrip;

class GeometryHandler {

 public:    
  /**
   * static method returning the singleton pointer to GeometryHandler 
   */
  static GeometryHandler* getGeometryHandler();

  /**
   * destructor:
   * takes care of deleting the multiLayers
   */  
  ~GeometryHandler();
  /** 
   * returns the number of multiLayers 
   */ 
  int    nMultiLayer();

  /** 
   * returns the number of strip planes
   */ 
  int    nStripPlane();
  /** 
   * returns the number of strip planes
   */ 
  int    nGap();

  /** 
   * returns the number of strip planes
   */ 
  int    nRPCChamber();
  /** 
   * returns the pointers to all the multiLayers
   */ 
  //GeoMDTMultiLayer * multiLayerPtr(int jlayer);

  /** 
   * returns the pointers to all the strip planes 
   */ 
  GeoRPCStripPlane * stripPlanePtr(int jplane);
  /** 
   * returns the pointers to all the strip planes 
   */ 
  GeoRPCChamber * RPCChamberPtr(int jcham);
    /** 
   * returns the pointers to all the strip planes 
   */ 
  GeoRPCGap * gapPtr(int jgap);

  /** 
   * returns the pointer to the multiLayer MultiLayerId
   */   
  //GeoMDTMultiLayer * findMultiLayer(int MultiLayerId);

  /** 
   * returns the pointer to the Tube tubeId
   */   
  //GeoMDTTube * findTube(int tubeId);

  /** 
   * returns the pointer to the strip plane StripPlaneId
   */   
  GeoRPCStripPlane * findStripPlane(int StripPlaneId);
  /** 
   * returns the pointer to the strip plane StripPlaneId
   */   
  GeoRPCChamber * findRPCChamber(int ChamberId);
    /** 
   * returns the pointer to the strip plane StripPlaneId
   */   
  GeoRPCGap * findGap(int GapId);
 /**
   * the method allowing the setting of a new multiLayer
   */
 // void set(GeoMDTMultiLayer * ml);

 /**
   * the method allowing the setting of a new strip plane
   */
  void set(GeoRPCStripPlane * ml);
 /**
   * the method allowing the setting of a new strip plane
   */
  void set(GeoRPCChamber * ml);
 /**
   * the method allowing the setting of a new strip plane
   */
  void set(GeoRPCGap * ml);


 /**
   * to be called at each new run to update links 
   */
  void  updateLinks(int run);


 /**
   * the method allowing the printout of the geometry 
   */ 
  void print();
  
 protected:
 /**
   * the method allowing some simple checks and printouts on a tube 
   */
//  bool checkMDT(int jtube, GeoMDTTube * gtubePtr);

  GeometryHandler(); /**< constructor */

 /**
   * the method allowing some simple checks and printouts on a strip 
   */
  bool checkRPC(int jstrip, GeoRPCStrip * gstripPtr);

  
 private:

  /**
   * load all the multiLayers and tubes from the inputList files, 
   * load all the strip planes and strips from the inputList files 
   * (steering routine)
   */
  void load();

  /**
   * load a multiLayer and its tubes 
   * (called once, at the beginning of the program)
   */
  void loadMultiLayer(char fileName[255]);
  /**
   * load a strip plane and its strips 
   * (called once, at the beginning of the program)
   */
//  void loadStripPlane(char fileName[255]);
  /**
   * load an RPCChamber 
   * (called once, at the beginning of the program)
   */
  void loadRPCChamber(char fileName[255]);
  void DBloadRPCChamber(int idChamb, std::string ChamberName, int TestGroup);

  /**
   * load a translation + rotation
   * (called when necessary, both automatically or by user)
   */
  void loadTransform(char fileName[255]);

  /**
   * load modifications to the geom setup
   * (called by user)
   */
  void loadMods(char fileName[255]);
  
 private:

  /**
   * the singleton pointer
   */
  static GeometryHandler* m_GeometryHandler;


//  std::vector<GeoMDTMultiLayer *> m_multiPtr; /**< list of ALL the m.layers */
  std::vector<GeoRPCStripPlane *> m_planePtr; /**< list of ALL the s.planes */
  std::vector<GeoRPCChamber *> m_rpcchamberPtr; /**< list of ALL the RPC Chambers */
  std::vector<GeoRPCGap *> m_gapPtr; /**< list of RPC Gaps */
  int m_debugLevel;
  TSQLServer *db;
  std::vector<float> XYZOffset(std::string ChName, int tg) const;
  std::vector<float> PlaneOffset(std::string ChName, int tg) const;
  
  };

  
#endif // GEOMETRYHANDLER_H


