/**
 * \classRPCReconstructor
 * class reconstructing Tracks for a set of RPCHits
 * 
 * Class RPCReconstructor does
 *
 *   -triggers the PR. PR generates a vector of lists
 *    of RPCHits (one for each Track candidate)
 *
 *   -calls class RPCTrack for each each Track candidate
 *
 * Class RPCReconstructor contains
 *
 *   -list of RPCHit used to create Tracks
 *
 *   -list of Track candidates
 *
 * \date 26.07.00
 */

#ifndef RPCRECONSTRUCTOR_H
#define RPCRECONSTRUCTOR_H

#include <vector> 
#include "tools/Utils.h"
#include "his/HisFile.h" 

class RPCHit;
class RPCCluster;
class RPCTrack;


class RPCReconstructor {

public:
  /**
   ** the constructor
   ** takes the list of RPCHit to be reconstructed,
   ** this may be just one plane or many.
   */
  RPCReconstructor(std::vector<RPCCluster *> RPCRedList,int numpl,int numpc,
                  int minhits,int maxhits);
  RPCReconstructor(int numpl, int numpc,
                                  int minhits,int maxhits);
  /**
   ** the destructor 
   */
 ~RPCReconstructor();

  /**
   ** Method RPCPatRec 
   */ 
  std::vector<RPCTrack *> RPCPatRec(int deb, bool allchambers=false);
  std::vector<RPCTrack *> RPCPatRec(std::vector<RPCCluster *> NoiseList);
  std::vector<RPCTrack *> RPCX5PatRec();
  std::vector<RPCTrack *> RPCX5PatRec2(int deb);
  void dump();
  int flag(){return  m_flag;}
private:

  void getEquation(double y1, double y2, double z1, double z2, 
                   double * a, double * b); 

  double computeResidual(double y, double z, double a, double b); 

  std::vector<RPCCluster *> getBestList(std::vector<RPCCluster *> theList,float a,float b, int dir);
  std::vector<RPCCluster *> getBestList(std::vector<RPCCluster *> theList,float ax,float bx, float az, float bz);
  std::vector<RPCCluster *> m_fullHitList;
  int m_numplnotr;
  int m_numpc;
  int m_minhits;
  int m_maxhits;
  static double  chisquamaZ;
  static double  chisquamaX;
  int m_debug;
  int m_flag;
};
#endif //RPCRECONSTRUCTOR_H
