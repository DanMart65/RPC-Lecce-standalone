/**
 * \class EltxHandler
 * a singleton for the handling of MDT and RPC channel map 
 *
 * \author D.Orestano, Modified by E.Gorini to handle RPC (14.10.02)
 * \date 26.09.01
 */


#ifndef EltxHANDLER_H
#define EltxHANDLER_H

#include "tools/Utils.h"
#include <fstream>
#include <vector> 

class EltxFile;

class EltxHandler {

 public:

  /**
   * static method returning the singleton pointer to EltxHandler 
   */
  static EltxHandler* getEltxHandler();

  /**
   * destructor:
   * takes care of deleting inputList and Eltxfiles   
   */
  ~EltxHandler();                   

  /** 
   * returns the complete list of Eltx 
   */ 
  std::vector<EltxFile *> Eltxfiles(); 

  /** 
   * returns the tube id for a given electronics channels 
   */ 
   //gabriele int EltxHandler::tubeId(int tdc_id, int tdc_ch, int run) ;  
   int tubeId(int tdc_id, int tdc_ch, int run) ;      
  /** 
   * returns the strip id for a given electronics channels 
   */ 
   //gabriele int EltxHandler::stripId(int tdc_id, int tdc_ch, int run) ; 
   //gabriele int EltxHandler::stripIdX5(int tdc_id, int tdc_ch, int run) ; 
   int stripId(int tdc_id, int tdc_ch, int run) ; 
   int stripIdX5(int tdc_id, int tdc_ch, int run) ; 
  /**
   * the method allowing the setting of a new Eltxfile 
   */
  void set(EltxFile * newEltx); 

 protected:

  EltxHandler(); /**< constructor */ 

 private:

  /**
   * load all the Eltxfile from the inputList files 
   */
  void load();

  /**
   * look for file t0file and read in the list of files to be processed
   */
  bool openInputStream();

 private:

  /**
   * the singleton pointer
   */
  static EltxHandler * m_EltxHandler;

  std::vector<EltxFile *> m_Eltxfiles;  /**< list of ALL the Eltxfile */
  std::vector<std::ifstream*> m_inputList; /**< list of input database files */
  std::vector<std::ifstream*>::iterator m_input;/**< pointer to actual file */

};
#endif //EltxHANDLER_H
