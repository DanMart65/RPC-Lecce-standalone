/** 
 * \class Timer
 * 
 * \author E.Gorini
 * \date 8.5.03
 */

#ifndef TIMER_H
#define TIMER_H
#include <time.h>                
#include "tools/MapHandler.h"                   
#include <map>                  
#include <string>

class Timer {
		public: 
                                Timer();
				~Timer();
				float Time();
                                void Start();
                                void Stop();
		private:
				int m_Time;
                                int m_start;
};

#endif // TIMER_H
