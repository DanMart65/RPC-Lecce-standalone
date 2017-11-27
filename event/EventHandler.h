/** 
 * \class EventHandler 
 * a base class for the handling of MDT test beam&sites events 
 * \author D.Orestano
 * \date 20.02.00
 */

#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <vector>
#include "string.h"
#include <fstream>
#include "tools/Utils.h"
#include "detector/RPCStripPlane.h"
#include "detector/RPCChamber.h"

class SubEvent;

class EventHandler {

   public:

      /** 
       * static method returning the singleton pointer to EventHandler 
       */
      static EventHandler* getEventHandler();    

      /**  
       * destructor: 
       * takes care of deleting subeventlist, inputList and loader 
       */ 
      ~EventHandler();

      bool firstEvent(); /**< open (rewind) the input file and read first evt */
      bool nextEvent();  /**< load the next event in current run */
      bool nextRun();    /**< switch to next run */
      std::vector<SubEvent*> subEvents(); /**< pointer to the subeventlist */
      std::string strippedName();
      int eventNumber() const; /**< return the actual eventNumber */ 

// get list of StripPlanes or RPCChambers
      std::vector<RPCStripPlane*> GetStripPlanes();
      std::vector<RPCChamber*> GetRPCChambers(); 

// all these method should pertain to an Header object        
      int runNumber() const;   /**< return the actual runNumber */ 
      int NumEvents() const;
      int phiOffset(int cham) const;
      float triggerOffset1() const;
      float triggerOffset2() const;
      int SourceStatus() const;
      int VtrhEta() const;
      int VtrhPhi() const;
      int TriggerType() const;
      std::string date() const; /**< data taking date */      
      
// togliere queto virtual      
      virtual double tdcScale(int tdcId=0) const=0; /**< return tdc tdcId scale factor*/

   protected:

      EventHandler(); /**< constructor */

      /**
       * the singleton pointer
       */
      static EventHandler* m_EventHandler; 

      /**
       * look for file input and read in the list of files to be processed 
       */
      bool openInputStream(); 

      /**
       * rewind input files and set input pointer to the first file 
       */
      void rewindInputStream();

      /**
       * clear the event and subevents contents 
       */
      void clearEvent();

      /**
       * load an event
       */
      virtual bool loadEvent()=0;

      /** 
       * load and decode the run record
       */
      virtual bool loadRun()=0;

      /**
       * load and check the end of event record 
       */
      virtual bool loadEnd()=0;

   protected:
      /**
       * the list of SubEvents active for this set up
       * it must be initialized making use of the geometry tree
       */
      std::vector<SubEvent*> m_subeventlist; 

      /**
       * the list of input files 
       */
      std::vector<std::ifstream*> m_inputList;

      /**
       * the pointer to the actual input file
       */
      std::vector<std::ifstream*>::iterator  m_input;

      int m_eventNumber; /**< event number */ 
      int m_debugLevel; /**< global debug level */
      std::string m_fileName; // name of actual input file 
      
// spstare in header object
      int m_runNumber;   /**< run number */ 
      int m_NumEvents;
      std::string m_date; /**< data taking date */
      int m_phiOffset[3];
      float m_triggerOffset1;
      float m_triggerOffset2;
      int m_SourceStatus;
      int m_VtrhEta;
      int m_VtrhPhi;
      int m_TriggerType;    	  
};

#endif // EVENTHANDLER_H


