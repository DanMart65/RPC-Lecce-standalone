/* ************************************************************
 * Changed a lot E.G. 2002
 *
 * ************************************************************ */

#include "control/CalibrationManager.h"
#include "control/CalibrationTask.h"
#include "user/RPCTBAnalize.h"            // Efficiency computation
//#include "user/RPCSlowMonitor.h"          // SlowMonitor
#include "user/RPCMonitor.h"              // Monitor
#include "user/RPCDisplay.h"              // graphic Display
#include "event/EventHandler.h"
//#include "event/RPCX5Event.h"
#include "event/RPCLETSEvent.h"
//#include "event/DCSHandler.h"
#include "database/DeadHotHandler.h"
#include "database/GeometryHandler.h"
#include "database/EltxHandler.h"
#include "his/HisFile.h"
#include "tools/MapHandler.h"
#include "tools/Timer.h"
#include "TFile.h"
#include "TH1.h"
#include "TNtuple.h"

CalibrationManager* CalibrationManager::m_CalibrationManager = 0;

CalibrationManager* CalibrationManager::getCalibrationManager() 
{  
    if(!m_CalibrationManager) {
        m_CalibrationManager = new CalibrationManager;
    }
    return m_CalibrationManager; 
} 

CalibrationManager::CalibrationManager()
{ 
    if(m_CalibrationManager) abort(); 
    MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
    Memory->increase();   

    m_display_file=0;
    m_CalibrationManager = this; 
    m_skipEvt = 0;
    m_maxEvt = 999999999;
    m_lowTDC = 0;
    m_highTDC = 2000;
    m_T0offset = 0;
    m_format = 0;
    m_geometryformat = 0; 
    jobOpt("input/lets.datacards");
    initialize();
}

CalibrationManager::~CalibrationManager()
{  
    std::vector<CalibrationTask*>::iterator  it;
    for( it=m_tasklist.begin(); it!=m_tasklist.end(); ++it) {
         delete *it;
    }                                                                           
    while( !m_tasklist.empty() ) {
       m_tasklist.pop_back();
    }

    EventHandler * eventHandler = EventHandler::getEventHandler();
// X5 RPC
//    if(dataFormat()==6) delete (RPCX5Event *) eventHandler;
// LE TS RPC
    if(dataFormat()==7) delete (RPCLETSEvent *) eventHandler;    

    GeometryHandler * gg = GeometryHandler::getGeometryHandler();
    delete gg;
//
//    DCSHandler * dcs = DCSHandler::getDCSHandler();
//    delete dcs;
// 
    DeadHotHandler * dh = DeadHotHandler::getDeadHotHandler();
    delete dh;
//   
    EltxHandler * eltx = EltxHandler::getEltxHandler();
    delete eltx;
    
    MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
    Memory->decrease();   
}

void CalibrationManager::initialize()
{

// 
//    DCSHandler * dcs = DCSHandler::getDCSHandler();
// 
    DeadHotHandler * dh = DeadHotHandler::getDeadHotHandler();
// should be called first time after reading run header
//    GeometryHandler * gg = GeometryHandler::getGeometryHandler();

  //  if(dataFormat()==6) RPCX5Event * event = new RPCX5Event();
    if(dataFormat()==7) RPCLETSEvent * event = new RPCLETSEvent();
}

void CalibrationManager::jobOpt(char jobfile[255]) {

   MapHandler<Timer>  &htt = MapHandler<Timer>::getHandle();
   
   char option[14];
   char fileName[255];
   int n;
   char chComm[]   = "#"; 
   std::ifstream optFile(jobfile);
   if(!optFile) {
     std::cout << "Manager::jobOpt error in opening file "<<jobfile<<std::endl;
     std::cout << "                           default options assumed " << std::endl;
   } else {
     while( optFile >> option) {
      while (strncmp(option,chComm,1)==0) {
        optFile.ignore(500,'\n');
        optFile>>option ;
      }
      if (strncmp(option,"End",3)==0 ){
        break;
      } else if (strncmp(option,"nev",3)==0 ){
        optFile >> n;
        m_maxEvt=n;
      } else if (strncmp(option,"display",7)==0 ) {
        optFile >> n;
        m_num_display = n;
	    m_display_file=new std::ofstream("display.dump");
      } else if (strncmp(option,"TimeWindow",10)==0 ) {
        optFile >> m_timemin >> m_timemax;
      } else if (strncmp(option,"OutputRelativePath",18)==0 ) {
        optFile >> m_OutputPath;
       } else if (strncmp(option,"TDCrange",8)==0 ) {
        optFile >> n;
        m_lowTDC = n;
        optFile >> n;
        m_highTDC = n;
      } else if (strncmp(option,"T0offset",8)==0 ) {
        optFile >> n;
        m_T0offset=n;
      } else if (strncmp(option,"Debug",5)==0 ) {
        optFile >> n;
        m_Debug=n;
      } else if (strncmp(option,"DataFormat",10)==0 ) {
        optFile >> n;
        m_format=n;
      } else if (strncmp(option,"GeometryFormat",14)==0 ) {
        optFile >> n;
        m_geometryformat=n; 
      } else if (strncmp(option,"nsk",3)==0 ) {
        optFile >> n;
        m_skipEvt=n;
      } else if (strncmp(option,"RPCTBAnalize",12)==0) {
            optFile >> n;
            CalibrationTask *task=new RPCTBAnalize("RPCTBAnalize");
            std::cout <<"Manager: task "<< option <<" selected with calling seq. "<< n <<std::endl;
            setTask(task,n);  
      } else if (strncmp(option,"RPCSlowMonitor",14)==0) {
            optFile >> n;
            //gabriele CalibrationTask *task=new RPCSlowMonitor("RPCSlowMonitor");
            std::cout <<"Manager: task "<< option <<" selected with calling seq. "<< n <<std::endl;
            //gabriele setTask(task,n);  
      } else if (strncmp(option,"RPCMonitor",10)==0) {
            optFile >> n;
            CalibrationTask *task=new RPCMonitor("RPCMonitor");
            std::cout <<"Manager: task "<< option <<" selected with calling seq. "<< n <<std::endl;
            setTask(task,n);                               
      } else if (strncmp(option,"RPCDisplay",10)==0) {
            optFile >> n;
            CalibrationTask *task=new RPCDisplay();
            setTask(task,n);
            std::cout <<"Manager: task "<< option <<" selected with calling seq. "<< n <<std::endl;         
      } else if (strncmp(option,"Cal",3)==0) {
      // select the active tasks
         optFile >> n;
         std::cout <<"task "<< option <<" selected with calling seq. "<< n <<std::endl;
      }
     }
    } 
    std::cout << "Manager: processing " << m_maxEvt << " events " << std::endl; 
    std::cout << "Manager: skipping " << m_skipEvt<< " events " << std::endl << std::endl; 
}

void CalibrationManager::execute()
{
/** questo per eseguire i task in sequenza e non in parallelo
    std::vector<CalibrationTask*>::iterator  it;
    for( it=m_tasklist.begin(); it!=m_tasklist.end(); ++it) {
         eventLoop(*it);
    }
**/

    eventLoop();  
                                                                               
    return;
}

void CalibrationManager::setTask(CalibrationTask* task, int position)
{
   std::vector<CalibrationTask*>::iterator  it;
   task->setOrder(position);
   for( it=m_tasklist.begin(); it!=m_tasklist.end(); ++it) {
      if((*it)->getOrder() > position) {
          m_tasklist.insert(it,task);
          return; 
      }
   }                                                                           
   m_tasklist.push_back(task);
   return;
}                                                                               

CalibrationTask * CalibrationManager::getTaskPtr(std::string taskname)
{
   std::vector<CalibrationTask*>::iterator  it;
   for( it=m_tasklist.begin(); it!=m_tasklist.end(); ++it) 
      if( !(*it)->Name().compare(taskname) )return (*it); 
// ritorna cosa se non lo trova ?                                                                        
   return 0x0000;
}

// The event loop: called by CalibrationManager::execute once per task

void CalibrationManager::eventLoop(CalibrationTask* task)
{
// get the pointer to the first event in current input stream  
// gabrita- questo non viene chiamato

    EventHandler * eventHandler = EventHandler::getEventHandler();
    
    eventHandler->firstEvent();  // take care of positioning file pointer at
                                 // beginning of input files,....
    task->taskInit();    // task initialization 
 
      int displayed=0;
      int iall=0;
      do {               // loop over different runs/files 
        runInit();
        task->runInit();
        int iev=0;

// loop over events

        while(eventHandler->nextEvent() && iev < (m_maxEvt+m_skipEvt) ) {
          iev++;
          displayed++;
          iall++;
          if(displayed > m_num_display && m_display_file) {
            m_display_file->close();           
    	    m_display_file=0;
            }
          if(iev > m_skipEvt) task->evtProc();
	  }
        task->runEnd();
      } while(eventHandler->nextRun());
    
    std::cout << std::endl<<"Manager: Number of processed events " << iall << std::endl << std::endl;

    if(!task->taskEnd()) eventLoop(task);  // iterate same task once more...
}


void CalibrationManager::eventLoop()
{
// get the pointer to the first event in current input stream 
    EventHandler * eventHandler = EventHandler::getEventHandler();
 
    eventHandler->firstEvent();  // take care of positioning file pointer at
                                 // beginning of input files,....

// set and open here the Histogram File, one per job with name of first run...
// get stripped name of input file and add relative path and suffix
// std::string hname=m_OutputPath+"/his/"+eventHandler->strippedName()+".hbook";  
//   std::string hname="./output/his/"+eventHandler->strippedName()+".hbook"; 
   std::string rootname="./output/Tree"+eventHandler->strippedName()+".root";
// object should be deleted at end of job.... 
// gabrita: la riga seguente provoca un warning nel log del tipo: 
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// locf_() Warning: changing base from 0 to 7fff00000000!!!
// This may result in program crash or incorrect results
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//   HisFile* hf = HisFile::getHisFile(hname);
   HisFile* FileTree = HisFile::getHisFile(3, rootname);
// run Tasks init

    std::vector<CalibrationTask*>::iterator  it;
    for( it=m_tasklist.begin(); it!=m_tasklist.end(); ++it)(*it)->taskInit();


    int iall=0;

    do {               // loop over different runs/files 
      runInit();
// run Tasks RunInit

      std::cout<<"dopo RunInit "<<std::endl;
      for( it=m_tasklist.begin(); it!=m_tasklist.end(); ++it)(*it)->runInit();
      std::cout<<"ancora RunInit ? "<<std::endl;

      int iev=0;

// loop over events

      while(eventHandler->nextEvent() && iev < (m_maxEvt+m_skipEvt) ) {
         iev++;
         iall++;
         if(iev > m_skipEvt){ 
           // run Tasks event processing
           for( it=m_tasklist.begin(); it!=m_tasklist.end(); ++it)(*it)->evtProc();
         }
      }
 // run Tasks Run end
       for( it=m_tasklist.begin(); it!=m_tasklist.end(); ++it)(*it)->runEnd();
    } while(eventHandler->nextRun());
    

// run Tasks end
    for( it=m_tasklist.begin(); it!=m_tasklist.end(); ++it)(*it)->taskEnd();
    
// here update histogram file and delete object;
  //  hf->updateFile(hname);
  //  delete hf;

// scrittura dei valori nei diversi tree creati durante il programma nel file root, quando finisce distrugge il puntatore

	std::vector<RPCChamber*> chambers = eventHandler->GetRPCChambers();  
	RPCChamber *ch;

    std::vector<TTree*> vectorTree = FileTree->GetVectorTree();
    TFile* final = FileTree->GetRootFile();
    if (final->IsOpen()) std::cout << "\n========= File root aperto in scrittura =========\n" << std::endl;
    else std::cout << "\n========= File non aperto =========\n" << std::endl;

    final->cd();

    
    TDirectory *chamberDir;
    TDirectory *HistogramParam;
    TDirectory *RawHitmap;
    TDirectory *HitProfile;
    TDirectory *ExpectedHitmap;
    TDirectory *ObtainedHitmap;

    for(int jch=0; jch!=chambers.size();jch++)    
    {
      std::string chambername=chambers[jch]->geo()->Name();
      int numberGaps = chambers[jch]->nGaps();
      chamberDir = final->mkdir(chambername.c_str());
      RawHitmap = chamberDir->mkdir("Raw Hitmap");
      HistogramParam = chamberDir->mkdir("Histogram Parameter");
      if (chambername.compare(7,1,"T")!=0) ExpectedHitmap = chamberDir->mkdir("Expected Hitmap");
      if (chambername.compare(7,1,"T")!=0) ObtainedHitmap = chamberDir->mkdir("Obtained Hitmap");
      HitProfile = chamberDir->mkdir("Hit Profile");


    	for (int i=0; i < vectorTree.size(); i++)
    	{  
    	    	chamberDir->cd();
    	    	TTree* tree = FileTree->UseTree(i);
    	    	std::string treename = tree->GetName();
    		    std::string treetitle = tree->GetTitle();
    	    	if (treename.compare(0,8,chambername)==0)
    	    	{
    		    	if (m_Debug>0) tree->Scan();
    		    	//tree->Print();	
    		    	if (treetitle.compare(treetitle.size()-10,10, "Raw Hitmap")==0)
    		    	{
    		    		RawHitmap->cd();
    		    		tree->Write();
    		    	}
    		    	else if (treetitle.compare(treetitle.size()-11,11, "Hit Profile")==0)
    		    	{
    		    		HitProfile->cd();
    		    		tree->Write();
    		    	}
    			    else if (treetitle.compare(treetitle.size()-15,15, "Expected Hitmap")==0 && treename.compare(7,1,"T")!=0)
    		    	{
    		    		ExpectedHitmap->cd();
    		    		tree->Write();
    		    	}
    			    else if (treetitle.compare(treetitle.size()-15,15, "Obtained Hitmap")==0 && treename.compare(7,1,"T")!=0)
    		    	{
    		    		ObtainedHitmap->cd();
    		    		tree->Write();
    		    	}
    		    	
        	    	//	std::cout << "Numero di entrate: " << tree->GetEntries() << "\n" << std::endl;
        	    		chamberDir->cd();
        		}

    	}
    	
    	for (int i=0; i < numberGaps; i++)
    	{  
    		chamberDir->cd();
    		std::string gapname=chambers[jch]->geo()->gapPtr(i)->Name();
    		std::string tuplename = gapname+"-TUPLE";
    		TNtuple* tuple = (TNtuple*)final->Get(tuplename.c_str());
    		if(!tuple) std::cout << "tuple non trovato: " << tuplename << "\n" << std::endl;
    		else {
    		//	std::cout << "******** tupla trovata: " << tuple->GetName() << "\n" << "Descrizione: " << tuple->GetTitle() << std::endl;
    			HistogramParam->cd();
    			tuple->Write();
    		}	
      }
    	   
  	  final->cd();	
    }

/* DA INSERIRE SE SI VOGLIONO I TREE E GRAFICI "LATCHES" E "CHANNELS"
        for (int i=0; i < vectorTree.size(); i++)
        {  
        	TTree* tree = FileTree->UseTree(i);
        	std::string treename = tree->GetName();
    	if (treename.compare("latches")==0 || treename.compare("channels")==0)
        	{
        		if (m_Debug>0) tree->Scan();
    	    //	tree->Print();	
    	    	tree->Write();
           	        std::cout << "Numero di entrate: " << tree->GetEntries() << "\n" << std::endl;
        	}
        }

*/  	    
        //final->ls();
        final->Close();

        std::cout << "\n========= File root chiuso =========\n" << std::endl;


    delete FileTree;
    
    std::cout << "\nMalice: Number of processed events " << iall << std::endl;
}


void CalibrationManager::runInit()
{
    EventHandler * eventHandler = EventHandler::getEventHandler();
    GeometryHandler * gg = GeometryHandler::getGeometryHandler();
    gg->updateLinks(eventHandler->runNumber());
}

std::ofstream * CalibrationManager::display_file(){
  return m_display_file;
}

int CalibrationManager::minTDC() {
   return m_lowTDC;
}

int CalibrationManager::maxTDC() {
   return m_highTDC;
}

int CalibrationManager::T0offset() {
   return m_T0offset;
}

int CalibrationManager::Debug() {
   return m_Debug;
}

int CalibrationManager::dataFormat() {
   return m_format;
}

int CalibrationManager::geometryFormat() {  
   return m_geometryformat;
}                                                                                                                             

int CalibrationManager::timeMin() {  
   return m_timemin;
} 
int CalibrationManager::timeMax() {  
   return m_timemax;
} 
std::string CalibrationManager::outputPath() {  
   return m_OutputPath;
}
