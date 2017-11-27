/**
 * \class RPCTBAnalize
 * example of user class for RPC task Analysis 
 *
 * \author E.Gorini, M. Primavera
 * \date 17.07.00
 */

#ifndef RPCTBANALIZE_H
#define RPCTBANALIZE_H

#include "control/CalibrationTask.h"
#include "user/RPCEffi.h"
#include "recon/RPCTrack.h"
#include <map>

class RPCTBAnalize:public CalibrationTask {
   public:
      RPCTBAnalize(std::string name);             /**< the constructor */
      virtual ~RPCTBAnalize();    /**< the distructor */
      virtual void taskInit();  /**< task initialization */
      virtual bool taskEnd();   /**< end the task if convergence reached */
      virtual void runInit();   /**< new run initialization */
      virtual void runEnd();    /**< run summary */
      virtual void evtProc();   /**< the event processing */
      std::vector<RPCTrack *> tracks(){return m_tracks;}
      
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
      int m_stripwindow;
	  int m_hitsforeffi;
	  bool m_allchambers;
      std::vector<RPCTrack *> m_tracks;
      int NumPlanesPerChamber(int ichamb);  
      
};
#endif //RPCTBANALIZE_H

