/**
 * main program for the application calib
 */


#include <time.h>
#include <map>
#include <string>
#include "TROOT.h"
#include "TApplication.h"


#include "control/CalibrationManager.h"  /**< the job control */
#include "tools/Utils.h"                 /**< for the MemoryChecker */
#include "his/HisFile.h"                 /**< for the histogram package */
#include "event/Handler.h"		/**< singleton Template handler */
#include "event/Header.h"               /**< for the Headers */    
#include "event/RPCLETSHeader.h"  
//#include "event/DCSLETSBlock.h"                    
#include "tools/MapHandler.h" 
#include "tools/Timer.h" 		/**< for timers */
#include <unistd.h>
#include <stdio.h>
#include <errno.h> // libreria standard per la gestione delle situazioni d'errore


// int Error; //left undefined by Motif
 // Initialize the ROOT system
//  extern void  InitGui();  // initializer for GUI needed for interactive
//  VoidFuncPtr_t initfuncs[] = { InitGui, 0 };
//  TROOT root("Test","Starting ROOT from a program", initfuncs);
// TROOT root("Test","Starting ROOT from a program");



//template<class Header>
//Handler<Header> Handler<Header>::Singleton;
//template<class RPCLETSHeader>
//Handler<RPCLETSHeader> Handler<RPCLETSHeader>::Singleton;
//template<class DCSLETSBlock>
//Handler<DCSLETSBlock> Handler<DCSLETSBlock>::Singleton;
template<class Timer>  
MapHandler<Timer> MapHandler<Timer>::Singleton;


int main(int argc, char** argv)
{

   time_t starttime=time(NULL);

   std::cout << std::endl<< "Malice: Start job at: " << ctime(&starttime) << std::endl;

   MapHandler<Timer>  &htt = MapHandler<Timer>::getHandle();

   htt["Main"]=new Timer();
  
   htt["Main"]->Start();
 

// construct the default CalibrationManager
   CalibrationManager* calMan = CalibrationManager::getCalibrationManager();
// job execution  


   calMan->execute();


// job termination
   delete calMan;
    
// check memory

    MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
    if(Memory->objects())std::cout << "Malice: memory leak: objects = " << Memory->objects() << std::endl;  
    
    htt["Main"]->Stop();

    std::cout << "Malice: Total execution Time: " << htt["Main"]->Time() << std::endl;
    starttime=time(NULL);

    std::cout << std::endl<< "Malice: End job at: " << ctime(&starttime) << std::endl; 

    return 0;
}
