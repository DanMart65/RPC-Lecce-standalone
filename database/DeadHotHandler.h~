/**
 * \class DeadHotHandler
 * a singleton for the handling of RPC dead or hot channel map 
 *
 * \author E.Gorini 
 * \date 29.11.02
 */


#ifndef DeadHotHANDLER_H
#define DeadHotHANDLER_H

#include "tools/Utils.h"
#include <fstream>
#include <vector> 

class DeadHotFile;

class DeadHotHandler {

 public:

  /**
   * static method returning the singleton pointer to DeadHotHandler 
   */
  static DeadHotHandler* getDeadHotHandler();

  /**
   * destructor:
   * takes care of deleting inputList and DeadHotfiles   
   */
  ~DeadHotHandler();                   

  /** 
   * returns the complete list of DeadHot 
   */ 
  std::vector<DeadHotFile *> DeadHotfiles(); 

  //gabriele bool DeadHotHandler::isDead(int stripId, int run) ; 
  //gabriele bool DeadHotHandler::isDeadWindow(int stripId, int run) ; 
  bool isDead(int stripId, int run) ; 
  bool isDeadWindow(int stripId, int run) ; 


  /**
   * the method allowing the setting of a new DeadHotfile 
   */
  void set(DeadHotFile * newDeadHot); 

 protected:

  DeadHotHandler(); /**< constructor */ 

 private:

  /**
   * load all the DeadHotfile from the inputList files 
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
  static DeadHotHandler * m_DeadHotHandler;

  std::vector<DeadHotFile *> m_DeadHotfiles;  /**< list of ALL the DeadHotfile */
  std::vector<std::ifstream*> m_inputList; /**< list of input database files */
  std::vector<std::ifstream*>::iterator m_input;/**< pointer to actual file */

};
#endif //DeadHotHANDLER_H
