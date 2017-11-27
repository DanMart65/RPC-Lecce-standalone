/** 
 * \class CalibrationManager 
 * a class for the control of an MDT calibration job
 * modified to handle tasks in parallel, E.Gorini 13/4/2003
 * \author D.Orestano
 * \date 20.01.00
 */

#ifndef CALIBRATIONMANAGER_H
#define CALIBRATIONMANAGER_H

#include <vector>
#include <fstream> 
#include <string>
#include "tools/Utils.h"

class CalibrationTask;

class CalibrationManager {

   public:

      /** 
       * static method returning the singleton pointer to CalibrationManager
       */
      static CalibrationManager* getCalibrationManager();    

      /**  
       * destructor: 
       * takes care of deleting tasklist 
       */ 
      virtual ~CalibrationManager();

      /** 
       * job initialization method:
       * reads from an external file the job options 
       */ 
      virtual void jobOpt(char jobfile[255]);

      /** 
       * job execution method:
       * should take care of the loop over tasks and call eventLoop 
       * for each task 
       */ 
      virtual void execute();

	  /**
	   * return the pointer to the output file for event display
	   */
      std::ofstream * display_file(); 

	  /**
	   * return the lower bound for the TDC counts of physical MDT hits
	   */
	  int minTDC();

	  /**
	   * return the upper bound for the TDC counts of physical MDT hits
	   */
	  int maxTDC();

	  /**
	   * return the offset to be applyed to fitted T0 values 
	   */
	  int T0offset();
	  
	  /**
	   * return the Debug level
	   */
	  int Debug();
	  
	  /**
	   * format of the input data: 
	   * 0: intermediate ascii file form Paolo Bagnaia (defined for H8 in 1998)
	   * 1: DAQ-1 binary files from H8 2001
	   * 7: RPC Stazione di Lecce
	   */
	  int dataFormat();

	  /**
	   * format of the geometry: 
	   */
          int geometryFormat(); 
          
          int timeMin(); 
          int timeMax(); 
          std::string outputPath(); 
          CalibrationTask * getTaskPtr(std::string taskname);
   private:

      /** 
       * the singleton pointer
       */
      static CalibrationManager* m_CalibrationManager;
 
      /** 
       * constructor 
       */
      CalibrationManager();

      /**
       * job initilization method: called by the constructor
       * should take care of job initilization (job control datacards, 
       * set up definition ...). In particular it triggers the loading of
       * all the available database information
       */
      virtual void initialize();

      /** 
       * the event loop: the evtProc method of task is called for each event 
       * Old method, the sequence of task is done serially 
       */
      virtual void eventLoop(CalibrationTask* task);

      /** 
       * the event loop: the evtProc method of task is called for each event 
       * new method, the sequence of task is done in parallel 
       */
      virtual void eventLoop();
      /**
       * run initialization: the links from GeoMDTTube to the related 
       * RTRelations and T0s should be updated at the beginning of a new Run
       * \warning dummy until the geometry package will implement the method
       *          updateLinks
       */
      virtual void runInit();

      /** 
       * setting of the tasks to be run and of their calling sequence
       */ 
      virtual void setTask(CalibrationTask* task,int position);

   private:

      /** 
       * the list of tasks for this job
       */
      std::vector<CalibrationTask*> m_tasklist;
	  
	  /**
	   * the job options:
	   * number of events to be skipped in each run 
	   */
	  int m_skipEvt;
	  
	  /**
	   * the job options:
	   * number of events to be processed in each run 
	   */
	  int m_maxEvt;
	  
	  /**
	   * the job options:
	   * number of events to be displayed
	   */
      int m_num_display;
	  
	  /**
	   * the job options:
	   * input data format
	   */
	  int m_format;

	  /**
	   * the job options:
	   * geometry format
	   */
	  int m_geometryformat; 

	  /**
	   * the job options:
	   * the range in tdc counts for the physical MDT hits
	   * lower bound
	   */
	   int m_lowTDC; 
	   
	  /**
	   * the job options:
	   * the range in tdc counts for the physical MDT hits
	   * upper bound
	   */
	   int m_highTDC; 
	   
	  /**
	   * the job options:
	   * the quantity to be subtracted from the t0 values obtained from the
	   * fit to account for the real time associated to hits close to the wire
	   */
	   int m_T0offset;
	   
	   /*
	    * the job options:
	    * debug level for decoding and reconstruction classes, user tasks
	    * have it defined in their own datacards
	    */
	   int m_Debug; 
	   /*
	    * the job options:
	    * set the selection time window for hit selection in RPC
	    */
	   int m_timemin;
           int m_timemax;	   
           	   /*
	    * the job options:
	    * set the Output Relative Path
	    */
	   std::string m_OutputPath;
	  /** 
	   * pointer to the output file for event display
	   */
           std::ofstream * m_display_file;
	  
};

#endif // CALIBRATIONMANAGER_H
