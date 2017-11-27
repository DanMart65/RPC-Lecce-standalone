/** 
 * \class CalibrationTask 
 * abstract base class for an MDT Calibration Task
 *
 * The user can provide an application class based upon CalibrationTask
 * implementing non dummy methods taskInit,taskEnd,runInit,runEnd,evtProc. 
 *
 * \author D.Orestano
 * \date 20.01.00
 */
#include <string>


#ifndef CALIBRATIONTASK_H
#define CALIBRATIONTASK_H

class CalibrationTask {

   public:

      /**
       * task creator
       */
      CalibrationTask();

      /**
       * task destructor
       */
      virtual ~CalibrationTask();

      /** 
       * task initialization at each new interation over the data sample 
       * (put here histogram booking etc...)
       */
      virtual void taskInit()=0;

      /** 
       * task termination at the end of an iteration over the data sample 
       * (check here the results and decide wether a new iteration is needed:
       * return true when the task has completed its job)
       */
      virtual bool taskEnd()=0;

      /** 
       * initialization at each new run 
       * the update of geometry and calibrations when a run changes should
       * be handled by the CalibrationManager and not by the single tasks,
       * this method is therefore devoted to run dependent histograms 
       * initialization and similars 
       */
      virtual void runInit()=0;

      /** 
       * termination of a run 
       * the right method for task dependent run summaries
       */
      virtual void runEnd()=0;

      /** 
       * processing of the current event 
       * perform here event reconstruction
       */
      virtual void evtProc()=0;

      /** 
       * assignment of the position in the task list (calling sequence) 
       */
      void setOrder(int position);

      int getOrder() const; /**< access the task position in calling sequence */

      std::string Name() const; 
      
   private:

      /** 
       * the task position in calling sequence
       */
      int m_order;
   
   protected:
      /** 
       * the task position in calling sequence
       */
      std::string m_name;
};

#endif //CALIBRATIONTASK_H
