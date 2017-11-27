/**
 * \class mezzanine
 * base class to handle the mapping of channels in a MDT mezzanine card
 * \author D.Orestano
 * \date 27.09.01
 * modified by E.Gorini to handle RPC stripId in memory.
 **/

#ifndef MEZZANINE_H
#define MEZZANINE_H

#include "tools/Utils.h"
#include <vector>     

class mezzanine {

  public:
     mezzanine(int type, int identif, int channel);
     ~mezzanine();
	 int id() const;
	 int first() const;
	 int type() const;
     int stripId(int ch) const;
     void print();
     void printAll();
      
  private:
     int m_type;
     int m_id;
     int m_first;
     std::vector<int> m_stripId;
};

mezzanine::mezzanine(int type, int identif, int channel):
           m_type(type),m_id(identif),m_first(channel) {
  
  MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
  Memory->increase(); 

// for type =0 (i.e. RPC) store the stripId for each channel
if(m_type ==0) {
  int reloff[4] = {7,15,23,31};
  int sid,index;
  for(int ch=0; ch!=32; ch++) {
	if(m_first >= 0){
	  sid = m_first + ch;
	} else {
// caso negativo piu' complicato...           
	  index= (ch/8);
	  sid= -m_first+reloff[index]-ch+index*8;
	}          
    m_stripId.push_back(sid);
  }
  
} else if(m_type ==99) {
//PATCH  pippa speciale.....
  int reloff[4] = {7,15,23,31};
  int sid,index;
// primi 16 canali appartengono alla phi left  
// e corrispondono alle strip 33-48,
// nessuna modifica da fare
  for(int ch=0; ch!=16; ch++) {
	if(m_first >= 0){
	  sid = m_first+ ch;
	} else {
// caso negativo piu' complicato...           
	  index= (ch/8);
	  sid= -m_first+reloff[index]-ch+index*8;
	}          
    m_stripId.push_back(sid);
  }
// secondi 16 alla phi right 
// bisogna aggungere 4*100 a m_first
// inoltre in questo caso sarebbero le strip da 49 a 64
// da riportare a strip 1 a 16 cioe' bisogna sottrarre 48
// risultato=numero magico = 352
  for(int ch=16; ch!=32; ch++) {
	if(m_first >= 0){
	  sid = m_first+352 + ch;
	} else {
// caso negativo piu' complicato...           
	  index= (ch/8);
	  sid= -m_first+352+reloff[index]-ch+index*8;
	}          
    m_stripId.push_back(sid);
  }
} 

else if(m_type ==98) {
//PATCH  TRIGGER BOSA
  int reloff[4] = {7,15,23,31};
  int sid,index;

// primi 8 canali appartengono alla eta left up
// e corrispondono alle strip 33-40,
// nessuna modifica da fare
  for(int ch=0; ch!=8; ch++) {
	  sid = m_first + ch;
          m_stripId.push_back(sid);
  }

  for(int ch=0; ch!=8; ch++) {
	  sid = m_first + 7 - ch -100;
          m_stripId.push_back(sid);
  }

  for(int ch=0; ch!=8; ch++) {
	  sid = m_first + 7 - ch + 400;
          m_stripId.push_back(sid);
  }

  for(int ch=0; ch!=8; ch++) {
	  sid = m_first + ch + 300;
          m_stripId.push_back(sid);
  }


// primi 16 canali appartengono alla phi left  
// e corrispondono alle strip 33-48,
// nessuna modifica da fare
  for(int ch=0; ch!=16; ch++) {
	if(m_first >= 0){
	  sid = m_first+ ch;
	} else {
// caso negativo piu' complicato...           
	  index= (ch/8);
	  sid= -m_first+reloff[index]-ch+index*8;
	}          
    m_stripId.push_back(sid);
  }
// secondi 16 alla phi right 
// bisogna aggungere 4*100 a m_first
// inoltre in questo caso sarebbero le strip da 49 a 64
// da riportare a strip 1 a 16 cioe' bisogna sottrarre 48
// risultato=numero magico = 352
  for(int ch=16; ch!=32; ch++) {
	if(m_first >= 0){
	  sid = m_first+352 + ch;
	} else {
// caso negativo piu' complicato...           
	  index= (ch/8);
	  sid= -m_first+352+reloff[index]-ch+index*8;
	}          
    m_stripId.push_back(sid);
  }
}
	

}  
mezzanine::~mezzanine() {
  MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
  Memory->decrease(); 
}  
int mezzanine::id() const { return m_id; }
int mezzanine::type() const { return m_type; }
int mezzanine::first() const { return m_first; }
int mezzanine::stripId(int ch) const { return m_stripId[ch]; }
void mezzanine::print() { 
  std::cout << "Latch: " << m_type << " " << std::setbase(16) <<  m_id 
  << std::setbase(10) <<  " " << m_first << std::endl;
}
void mezzanine::printAll() { 
  std::cout << "Latch: " << m_type << " " << std::setbase(16) <<  m_id 
  << std::setbase(10) <<  " " << m_first << std::endl;
  for(int ch=0; ch!=m_stripId.size(); ch++)std::cout << m_stripId[ch] << " ";
  std::cout << std::endl;
}
#endif 





