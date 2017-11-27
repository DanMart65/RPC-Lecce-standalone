/**
 * \class RPCMonitor
 * example of user class for RPC task Analysis 
 *
 * \author E.Gorini
 * \date 21.12.03
 */

#ifndef RPCMONITOR_H
#define RPCMONITOR_H

#include "control/CalibrationTask.h"
#include <map>

class RPCMonitor:public CalibrationTask {
   public:
      RPCMonitor(std::string name);             /**< the constructor */
      virtual ~RPCMonitor();    /**< the distructor */
      virtual void taskInit();  /**< task initialization */
      virtual bool taskEnd();   /**< end the task if convergence reached */
      virtual void runInit();   /**< new run initialization */
      virtual void runEnd();    /**< run summary */
      virtual void evtProc();   /**< the event processing */
      void MakeClusterHistos();
      void MakeHitHistos();
      
   private:
      int m_call;               /**< number of iterations reached */
      int m_debug;              /**< debug flag */
      int m_maxdebug;           /** number of events to debug */
      
};
#endif //RPCMonitor_H

