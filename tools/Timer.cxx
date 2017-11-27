#include "tools/Utils.h"
#include "tools/Timer.h"
#include <unistd.h>
//long CLK_TCK= sysconf(_SC_CLK_TCK);

Timer::Timer():m_Time(0){
//std::cout << CLK_TCK << std::endl;
}

Timer::~Timer(){}

#ifdef MOTOROLA
float Timer::Time(){return (float)m_Time/100.;} 
#else
float Timer::Time(){return (float)m_Time/1000000.;} 
#endif
void Timer::Start(){m_start=clock();}

void Timer::Stop(){m_Time+=clock()-m_start;}
