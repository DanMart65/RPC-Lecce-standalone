/**
 * \class RPCAnalysis
 * example of user class for RPC task Analysis 
 *
 * \author E.Gorini, M. Primavera
 * \date 17.07.00
 */

#ifndef RPCANALYSIS_H
#define RPCANALYSIS_H

#include "control/CalibrationTask.h"
#include "user/RPCEffi.h"
#include "detector/RPCCluster.h"

class RPCAnalysis:public CalibrationTask {
   public:
      RPCAnalysis();             /**< the constructor */
      virtual ~RPCAnalysis();    /**< the distructor */
      virtual void taskInit();  /**< task initialization */
      virtual bool taskEnd();   /**< end the task if convergence reached */
      virtual void runInit();   /**< new run initialization */
      virtual void runEnd();    /**< run summary */
      virtual void evtProc();   /**< the event processing */
      void MakeClusterHistos(std::vector<RPCHit *> ClusList);
      void MakeHitHistos();
      
   private:
      int m_call;               /**< number of iterations reached */
      int m_debug;              /**< debug flag */
      int m_maxdebug;           /** number of events to debug */
      int m_maxdump;            /** number of events to dump track info */
      int numplnotr;     /**< number of RPC planes no trigger */
      int nchamtr;       /**< number of RPC trigger chambers */
      int m_minhits;
      int m_maxhits;
      int m_timemin;
      int m_timemax;
      std::vector<int> chatriglist; /** list of trigger chambers */
      std::vector<RPCEffi *> PlanEffList;
      int NumPlanesPerChamber(int ichamb);  
      bool IsTriggerChamber(int ichamb);    
      
};

#endif //RPCANALYSIS_H
