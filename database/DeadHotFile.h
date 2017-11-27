/**
 * \class DeadHotFile
 * a DeadHot File 
 *
 * \author E.Gorini 
 * \date 29.11.02
 **/

#ifndef DeadHotFILE_H
#define DeadHotFILE_H

#include "tools/Utils.h"
#include <fstream> 
#include <vector> 
#include "string.h"

class DeadHotFile {

 public:
  /**
   * the constructor 
   * of a DeadHotfile from a database inputFile 
   */
  DeadHotFile(std::ifstream * inputFile);
  /**
   * the destructor
   * must take care of deleting the DeadHotfile (a list of DeadHot) 
   */
  ~DeadHotFile();
  bool isValid(int run) const;/**<checks run number*/
  bool isDead(int stripId); // returns true if Strip is dead
  bool isDeadWindow(int stripId); // returns true if Strip or strip+-1 is dead
 private:
  int m_firstRun;  /**< firstRun of validity */ 
  int m_lastRun;   /**< lastRun of validity */ 
  /**
   * list of mezzanines in the system
   */
  std::vector<int> deadList;
};
#endif //DeadHotFILE_H
