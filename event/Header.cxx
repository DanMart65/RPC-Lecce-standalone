#include "event/Header.h"
#include "tools/Utils.h"

Header::Header(){ }
Header::~Header(){ }

int Header::runNumber() const {return m_runNumber;}

std::string Header::date() const { return m_date;}

int Header::NumEvents() const {return m_NumEvents;}

int Header::TriggerType() const {return m_TriggerType;}

int Header::phiOffset(int cham) const
{
    if(m_runNumber >  140){
      return m_phiOffset[cham-1];
    }
    else {
      return m_phiOffset[2];
    }
}



///////////////////////////////////////////////////////////////////////
#ifdef MOTOROLA
unsigned long int Header::invertbytes (unsigned long int dat) {return dat;}
#else
unsigned long int Header::invertbytes (unsigned long int dat) {
unsigned long int temp[4]={0,0,0,0};
unsigned long int out=0;
unsigned long int nbyte[4]={0x000000ff,0x0000ff00,0x00ff0000,0xff000000};
int i;
 for(i=0; i<4; i++) { 
  if(i==0) temp[3-i] = (dat & nbyte[i])<<24;
  if(i==1) temp[3-i] = (dat & nbyte[i])<< 8;
  if(i==2) temp[3-i] = (dat & nbyte[i])>> 8;
  if(i==3) temp[3-i] = (dat & nbyte[i])>>24; 
} 
 for(i=0;i<4;i++) {out=out | temp[i];}
 return out;
}
#endif


