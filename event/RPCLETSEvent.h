/** 
 * \class RPCLETSEvent 
 * a class for the handling of the RPC LEcce Test Station events in binary format 
 * to select this format set data card DataFormat to 7  
 * \author E.Gorini
 * \date 9.02.03
 */

#ifndef RPCLETSEVENT_H
#define RPCLETSEVENT_H

#include "event/EventHandler.h"
#include "detector/Latch.h"
#include "detector/RPCStripPlane.h"
#include "detector/RPCChamber.h"

class RPCLETSEvent : public EventHandler {

   public:
       RPCLETSEvent();  // constructor
       ~RPCLETSEvent(); // destructor 
       int TriggerTime() const {return m_trigTime;}    
   private:
       bool loadEvent(); // load an event i.e. reads and decode it
       bool loadRun(); // load run Header reading from input*/
       bool loadEnd(); // dummy 
       std::vector<Latch*> readEvent();// read from binary file and put in a buffer
       bool eventDecode(std::vector<Latch*> latches);// decode the event buffer
       unsigned long int invertbytes (unsigned long int dat);
       unsigned short int invertbytes (unsigned short int dat);  
       std::string readascii(int size);         
       double tdcScale(int tdcNum) const;      
// put here all global relevent variables to be kept in the Event
       int m_trigTime; //
};

#endif // RPCLETSEVENT_H
