#include "control/CalibrationManager.h"
#include "database/EltxFile.h"
#include "database/mezzanine.h"
#include "event/RPCLETSHeader.h"
#include "event/Handler.h"

EltxFile::EltxFile(std::ifstream * inputFile) 
{
   MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
   Memory->increase(); 
   Handler<RPCLETSHeader>  &hd = Handler<RPCLETSHeader>::getHandle();
   RPCLETSHeader * head=hd[hd.size()-1];  

   char tag[100];
   int dettyp;
   int bank, identifier, ch, downup, phieta, chip, addbank, addlatch;
   (*inputFile) >> tag;
   inputFile->ignore(500,'\n');
   while( (*inputFile) >> bank) {                                                   
	 (*inputFile) >> std::setbase(16) >> identifier >> std::setbase(10) >> ch;
	 (*inputFile)>>downup>>phieta>>chip>>addbank>>addlatch;  
	 inputFile->ignore(500,'\n');      
// convert to my format
	 int pl,sign=-1,st;
	 dettyp=0;
	 if(phieta==1){
     // piani eta, (2,3 per left o 6,7 per right)
	   if(downup==1){
	     pl=3;sign=1;
	   } else { 
	     pl=2;sign=-1;
	   }
// se right inverti segno e aggiungi 4 al piano
	   if(  (chip==1) || (chip==3)  ){ // patch bmsa (5 connettori eta)
	      pl+=4;sign=-sign;
	   }
// per eta first strip e' sempre 1
	   st=1;
           if(chip>=2) st=33;  // patch bmsa (5 connettori eta)
	                
	 
	 
////////////////////////////////////////////////////////////////////// PATCH BOSA TRIGGER	       
	   if(head->runNumber() > 16265) if ( (ch==1 || ch==2 || ch==11 || ch==12) && chip==2) dettyp=98;
	 
         }

	 
	 else {
// piani phi, (1,4 per left o 5,8 per right)
	   if(downup==1){
	     pl=4;sign=1;
	   } else {
	     pl=1;sign=-1;
	   }
	   st=chip*32+1;
////////////////////////////////////////////////////////////////////// PATCH	       
	       if(head->runNumber() >= 948 && head->runNumber() <= 1142){    
                 if(chip>=2){
                   pl+=4;
                   st-=64;
                 }
	       } else if(head->runNumber() >= 2289 && head->runNumber() <= 2547){ 
			 if(ch>=3 && ch<=10){
		 // BMSE trigger chambers   
		 // chip 2 ha strip da 17 in poi dei piani 5 e 8
		       if(chip>1){
				 pl+=4;
				 st-=48;
			   }
		   
		 // caso tragico di chip 1 che e' mixed : ci sono le 
		 // strip 33-48 del piano left e le strip 1-16 di quello right
		 // ma fino ad ora ho le strip da 33 a 64
		 // mi invento sta' pippa.....
		       if(chip==1){
                     dettyp=99;                    
		       }
		     } else {
		 // BOSB trigger chambers    
			   if(chip>=2){
				 pl+=4;
				 st-=64;
			   }
			 }
		 } else {
// valido sempre
			   if(chip>=2){
				 pl+=4;
				 st-=64;
			   }
		 }	    
	 }
//	 if(upsidedown==1)pl=8-pl+1 ??????????????????????? NOOOOOOOOOOOOOOOOOOOO
	 int first_channel=sign*(ch*1000+pl*100+st);
// use mezzanine object to store info for rpc latch maps          
	 mezzanine * card = new mezzanine(dettyp,identifier,first_channel);
//	 card->print();
	 m_mezzanines[identifier]=card;
   }
   
   
   m_firstRun=0;
   m_lastRun=99999999;
   inputFile->close();
}

/*
EltxFile::EltxFile(std::ifstream * inputFile) 
{
   MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
   Memory->increase();   

   char tag[11],datenam[10],timenam[5];
   char chComm[]   = "#";

   while( (*inputFile) >> tag) {                                               
        while (strncmp(tag,chComm,1)==0) {
          inputFile->ignore(500,'\n');
          (*inputFile) >> tag;
        }      
        if(strncmp(tag,"isValid",7)==0) {
           (*inputFile) >> m_firstRun >> m_lastRun ;
           inputFile->ignore(500,'\n');
//           std::cout << "isvalid " << m_firstRun << " " << m_lastRun << std::endl;
        } else if(strncmp(tag,"latch",5)==0) {
           if(m_lastRun <=817){
           int identifier, first_channel;
           (*inputFile) >> std::setbase(16) >> identifier >> std::setbase(10) >> first_channel;           
// use mezzanine object to store info for rpc latch maps 
//           std::cout << "LATCH: " << std::setbase(16) <<  identifier << std::setbase(10) << std::endl;         
             mezzanine * card = new mezzanine(0,identifier,first_channel);
             m_mezzanines.push_back(card);  
           } else {
           
           
           // new maps.
             int bank, identifier, ch, downup, phieta, chip, addbank, addlatch;
             (*inputFile) >> bank >> std::setbase(16) >> identifier >> std::setbase(10) >> ch;
             (*inputFile)>>downup>>phieta>>chip>>std::setbase(16)>>addbank>>addlatch>>std::setbase(10);  
             inputFile->ignore(500,'\n');
//             std::cout << bank << " " << ch << std::endl;      
             // convert to my format
             int pl,sign=-1,st;
             if(phieta==1){
               // piani eta, (2,3 per left o 6,7 per right)
               if(downup==1){
                 pl=2;
                 sign=1;
               }
               else pl=3; 
               if(chip==1)pl+=4;
               st=1;             
             } else {
               // piani phi, (1,4 per left o 5,8 per right)
               if(downup==1){
                 pl=1;
                 sign=1;
               }
               else pl=4;
               st=chip*32+1;
               if(chip>=2){
                 pl+=4;
                 st-=64;
               }
             }
             int first_channel=sign*(ch*1000+pl*100+st);
//             std::cout << "LATCH: " << bank << " " << std::setbase(16) <<  identifier << std::setbase(10);
//             std::cout << " " << first_channel << std::endl;
             // use mezzanine object to store info for rpc latch maps          
             mezzanine * card = new mezzanine(0,identifier,first_channel);
             m_mezzanines.push_back(card);
           }
        } 
   }
}

*/


EltxFile::~EltxFile() 
{
  MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
  Memory->decrease();   
  if(!m_mezzanines.empty()) {
     std::map<int,mezzanine *>::iterator  it;
     for( it=m_mezzanines.begin(); it!=m_mezzanines.end(); it++) {
         delete m_mezzanines[(*it).first];
     }
  }
  m_mezzanines.clear();
}


bool EltxFile::isValid(int run) const 
{
   if(run > m_lastRun || run < m_firstRun) {
      return false;
   }  
   return true;
}

int EltxFile::tubeId(int tdc_nb,int tdc_ch) 
{
/*
   if(!m_mezzanines.empty()) {
      std::vector<mezzanine*>::iterator  ir;
      for( ir=m_mezzanines.begin(); ir!=m_mezzanines.end(); ++ir) {
         if((*ir)->id() == tdc_nb) {
           int tubeId0=(*ir)->first();            
           if(tubeId0==0) return 0;
           // position of channel 0
           int layer0=tubeId0/100-(tubeId0/1000)*10;
           int mlayer0=tubeId0/1000-(tubeId0/10000)*10;
           int chamber0=tubeId0/10000;
           int channel0=tubeId0-tubeId0/100*100;
           int type=(*ir)->type();
           int tubesPerLayer;
           if(type==3) tubesPerLayer=8;
           if(type==41) tubesPerLayer=6;
           if(type==42) tubesPerLayer=6;
           // compute layer for actual channel
           int layer;
           if ( layer0 == 1) {
                layer =  tdc_ch/tubesPerLayer + 1; 
           } else if ( layer0==type) {
                layer = type - tdc_ch/tubesPerLayer ; 
           } else {
                std::cout << "EltxFile::tubeId error in first tube number for " 
                     << "mezzanine " << tdc_nb << std::endl; 
                abort(); 
           }
           // compute channel inside mezzanine
           int channel=tdc_ch%tubesPerLayer; 
           if( type == 3) {
              channel = m_map3layers[mlayer0-1][channel];
           } else if( type == 41) {
              channel = m_map4layersZ[channel];
           } else if( type == 42) {
              channel = m_map4layersS[channel];
           } else { 
                std::cout << "EltxFile::tubeId error in mezzanine type for " 
                     << "mezzanine " << tdc_nb << std::endl; 
                abort(); 
           }
           // compute channel within the layer
           int mezzanine_pos=((channel0-1)/tubesPerLayer) ;
           int offset = mezzanine_pos * tubesPerLayer; 
           channel += offset;
           int tubeId=chamber0*10000+mlayer0*1000+layer*100+channel;    
           return tubeId; 
         }	
      }
   }
   return 0;
*/
}

/*
int EltxFile::stripId(int latch,int ch) 
{
// uses mezzanine objects with type 0
   if(!m_mezzanines.empty()) {
      int reloff[4] = {7,15,23,31};
      std::vector<mezzanine*>::iterator  ir;
      for( ir=m_mezzanines.begin(); ir!=m_mezzanines.end(); ++ir) {
         if((*ir)->type() ==0 && (*ir)->id() == latch) {
           if((*ir)->first() >= 0){
             return (*ir)->first() + ch;
           } else {
// caso negativo piu' complicato...           
             int ifi= -(*ir)->first();
             int index= (ch/8);
             int rel=ch-index*8;
             return ifi+reloff[index]-rel;
           }          
         }
      }  
   }
}
*/
int EltxFile::stripId(int latch,int ch) 
{
// uses mezzanine objects with type 0
   if(!m_mezzanines.empty()) {
//      std::vector<mezzanine*>::iterator  ir;
//      for( ir=m_mezzanines.begin(); ir!=m_mezzanines.end(); ir++) {
//         if((*ir)->type() ==0 && (*ir)->id() == latch) return (*ir)->stripId(ch);
//      }  
      mezzanine *m=m_mezzanines[latch];
	  if(m)return m->stripId(ch);
	  else return 0;
   }
}


int EltxFile::stripIdX5(int tdc_nb,int tdc_ch) 
{
/*
// hardwired per ora vedere come fare per leggere eltx.....
// sequenza e' phi1 eta1 eta2 phi2
              int strip;
	      int chid;
              int plane;
              // get strip number from TDC/channel numbers strip from 1 to 32 ......
	      if (tdc_nb<3){ // phi strips 
                  chid=tdc_nb+1;
		  if ( tdc_ch<16 ){
                     strip = tdc_ch +1; // gas gap closest to the source 
		     plane = 1;
                  }
                  else {
                    strip = 32-tdc_ch;
	            plane = 4;
                 }
              }
	      else if(tdc_nb<9)      // eta strips 
	      {
            // tdc 3 % 4 chid =1 , 5 % 6 chid=2, 7 % 8, chid=3
                  chid = (tdc_nb-1)/2;
		  if ((tdc_nb%2)!=0) {
                     strip = tdc_ch+1; // gas gap closest to the source 
                     plane = 2;
                  }
                  else {
                    strip = 32-tdc_ch;
                    plane = 3;
                  }
	      }
              // treat scintillators
              if(tdc_nb==9){
                chid = 0 ;
// trigger channels.. ?                 
                if(tdc_ch==0){plane=0;strip=0;}
                if(tdc_ch==1){plane=0;strip=1;} 
                                
                if(tdc_ch==8){plane=1;strip=2;}
                if(tdc_ch==9){plane=1;strip=1;}                
                if(tdc_ch==10){plane=2;strip=3;} 
                if(tdc_ch==11){plane=2;strip=2;}
                if(tdc_ch==12){plane=2;strip=1;}
                if(tdc_ch==13){plane=3;strip=3;} 
                if(tdc_ch==14){plane=3;strip=2;}
                if(tdc_ch==15){plane=3;strip=1;} 
                               
              }
            int stripid = chid*1000 + plane*100 + strip;
              return stripid;
*/
}
