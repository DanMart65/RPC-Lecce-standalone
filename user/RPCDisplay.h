/**
 * \class RPCDisplay
 * example of user class for RPC task Analysis 
 *
 * \author E.Gorini
 * \date 13.4.2003
 */

#ifndef RPCDISPLAY_H
#define RPCDISPLAY_H

#include "control/CalibrationTask.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include <string>
#include <iostream>
#include <fstream>
#include <strstream>
#include <math.h>

#include <TROOT.h>
#include <TRandom.h>
#include <TDatime.h>
#include <TFile.h>
#include "TH3.h"
#include "TH2.h"
#include "TH1.h"
#include "TGraph.h"
#include "TGaxis.h"
#include "TLine.h"

#include "TSystem.h"
#include "TStyle.h"
#include "TMarker.h"
#include "TLatex.h"


#include "TCanvas.h"

class RPCDisplay:public CalibrationTask {
   public:
      RPCDisplay();             /**< the constructor */
      virtual ~RPCDisplay();    /**< the distructor */
      virtual void taskInit();  /**< task initialization */
      virtual bool taskEnd();   /**< end the task if convergence reached */
      virtual void runInit();   /**< new run initialization */
      virtual void runEnd();    /**< run summary */
      virtual void evtProc();   /**< the event processing */      
   private:   
      void DrawStrips();
      int m_debug;
      int m_call;
      TCanvas *theWindow;
};
#endif //RPCDISPLAY_H

