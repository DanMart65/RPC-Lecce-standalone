/**
 * \class EltxFile
 * a Eltx File 
 *
 * \author D.Orestano, modified by E.Gorini for RPC decoding (14.10.02)
 * \date 26.09.01
 **/

#ifndef EltxFILE_H
#define EltxFILE_H

#include "tools/Utils.h"
#include <fstream> 
#include <vector>
#include <map> 

class mezzanine;

class EltxFile {

 public:
  /**
   * the constructor 
   * of a Eltxfile from a database inputFile 
   */
  EltxFile(std::ifstream * inputFile);
  /**
   * the destructor
   * must take care of deleting the Eltxfile (a list of Eltx) 
   */
  ~EltxFile();
  bool isValid(int run) const;/**<checks run number*/
  int tubeId(int tdc_id,int tdc_ch); /*< return tube id from tdc nb and chan*/
  int stripId(int tdc_id,int tdc_ch); /*< return strip id from tdc nb and chan*/
  int stripIdX5(int tdc_id,int tdc_ch); /*< return strip id from tdc nb and chan*/
 private:
  int m_firstRun;  /**< firstRun of validity */ 
  int m_lastRun;   /**< lastRun of validity */ 
  /** 
   * matrix 2x8 containing the list of tubes in first and second multilayer 
   * associated to the first 8 channels in a 3 layers mezzanine 
   */
  int m_map3layers[2][8]; 
  /** 
   * vector (6) containing the list of tubes in outer multilayer (Z-shaped) 
   * associated to the first 6 channels in a 4 layers mezzanine 
   */
  int m_map4layersZ[6]; 
  /** 
   * vector (6) containing the list of tubes in inner multilayer (S-shaped) 
   * associated to the first 6 channels in a 4 layers mezzanine 
   */
  int m_map4layersS[6]; 
  /**
   * list of mezzanines in the system
   */
  //std::vector<mezzanine *> m_mezzanines;
  std::map<int, mezzanine*, std::less<int> > m_mezzanines;
};
#endif //EltxFILE_H
