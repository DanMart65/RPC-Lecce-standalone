#include "detector/Latch.h"
#include "event/EventHandler.h"
#include "database/GeometryHandler.h"
#include "database/EltxHandler.h"
#include "database/GeoRPCStripPlane.h"
#include "database/GeoRPCStrip.h"
#include "event/RPCLETSHeader.h"
#include "event/Handler.h"

int errors=0;
/*******************************************************************************************
 * Store the data in private variables
 *******************************************************************************************/
Latch::Latch(unsigned long int *buffer, unsigned long int size)
{  
    MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
    Memory->increase();                  
// first word is Address
    m_address=buffer[0];
// rest are time slots...   
    for (int i=1;i!=size-1;i++)m_words.push_back(buffer[i]);
}
Latch::~Latch()
{  
    MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
    Memory->decrease();   

}
/*******************************************************************************************
 *
 * convert Latch Time Slots in Strip List
 *
 *******************************************************************************************/
std::vector<RPCStrip*> Latch::StripList(){

  GeometryHandler * geo=GeometryHandler::getGeometryHandler(); 
  EltxHandler * eltx = EltxHandler::getEltxHandler();
  Handler<RPCLETSHeader>  &hd = Handler<RPCLETSHeader>::getHandle();
  RPCLETSHeader * head=hd[hd.size()-1];
  unsigned long int FTB=head->FirstTimeBin();
  std::vector<RPCStrip*> striplist;
  int stripId,time;
// loop on latch words
  for(int iw=0;iw!=m_words.size();iw++){
// convert to (relative time) in ns
    time=(iw+FTB)*CLOCK;
    if(m_words[iw] == 0)continue;
//    std::cout << "address " << std::setbase(16) << m_address << " word " << m_words[iw]  
//    << std::setbase(10) << std::endl;    
// loop on bits
    for(int i=0;i!=32;i++){
// for each word check i-th bit
      if( (m_words[iw] & (1<<i)) ){
// found a signal, now map it, i=channel number
         stripId=eltx->stripId(m_address,i,head->runNumber());

 //        std::cout << " channel: " << setbase(10) << i << " strip " << stripId 
 //              << " time " << time << std::endl;
// find the right pointer in geometry..., first pointer to plane
         
	 //cout << "before stripId " << stripId<< "\n";
	 if (stripId > 13000)
	 {
	   char strip[6];
	   sprintf(strip,"%d",stripId);
	   if(strip[2]=='3'){
	    strip[2]='2';
	   } 
	   else if(strip[2]=='2'){
	    strip[2]='3';
	   } 
	 
	   stripId = atoi(strip);
	   //cout << " strip" << strip << "stripId " << stripId<< "\n"; 
	 
	 }
         //cout << "after stripId " << stripId<< "\n"; 


         if( (head->runNumber() > 15209) && (head->runNumber() < 15431)){
	  if(  (stripId>6224)&& (stripId<6233)  ) stripId = stripId-992;
	  if(  (stripId>6324)&& (stripId<6333)  ) stripId = stripId-992;
	  if(  (stripId>6624)&& (stripId<6633)  ) stripId = stripId-992; 
	  if(  (stripId>6724)&& (stripId<6733)  ) stripId = stripId-992; 
	 
	 }
    
    
    
    
    
     //  patch gruppo 48 e 49 6 camere BOSA in posizione 3/4/5/6/8/9  
     //  usiamo tutti i canali della posizione 7 ed i prini due connettori della posizione 10
          
         if( (head->runNumber() > 15985) && (head->runNumber() < 16532)){
 
   // lato left connettori down 	 	 
	 
	  if(  (stripId>7200)&& (stripId<7209)  ) stripId = stripId-3968;
	  if(  (stripId>7208)&& (stripId<7217)  ) stripId = stripId-2976;
	  if(  (stripId>7216)&& (stripId<7225)  ) stripId = stripId-1984; 
	  if(  (stripId>7224)&& (stripId<7233)  ) stripId = stripId-992; 
	  if(  (stripId>10200)&& (stripId<10209)  ) stripId = stripId-1968;
	  if(  (stripId>10208)&& (stripId<10217)  ) stripId = stripId-976;
	  
   // lato left connettori up

	  if(  (stripId>7300)&& (stripId<7309)  ) stripId = stripId-3968;
	  if(  (stripId>7308)&& (stripId<7317)  ) stripId = stripId-2976;
	  if(  (stripId>7316)&& (stripId<7325)  ) stripId = stripId-1984; 
	  if(  (stripId>7324)&& (stripId<7333)  ) stripId = stripId-992; 
	  if(  (stripId>10300)&& (stripId<10309)  ) stripId = stripId-1968;
	  if(  (stripId>10308)&& (stripId<10317)  ) stripId = stripId-976;

   // lato right connettori down 	 	 
	 
	  if(  (stripId>7600)&& (stripId<7609)  ) stripId = stripId-3968;
	  if(  (stripId>7608)&& (stripId<7617)  ) stripId = stripId-2976;
	  if(  (stripId>7616)&& (stripId<7625)  ) stripId = stripId-1984; 
	  if(  (stripId>7624)&& (stripId<7633)  ) stripId = stripId-992; 
	  if(  (stripId>10600)&& (stripId<10609)  ) stripId = stripId-1968;
	  if(  (stripId>10608)&& (stripId<10617)  ) stripId = stripId-976;

   // lato right connettori up

	  if(  (stripId>7700)&& (stripId<7709)  ) stripId = stripId-3968;
	  if(  (stripId>7708)&& (stripId<7717)  ) stripId = stripId-2976;
	  if(  (stripId>7716)&& (stripId<7725)  ) stripId = stripId-1984; 
	  if(  (stripId>7724)&& (stripId<7733)  ) stripId = stripId-992; 
	  if(  (stripId>10700)&& (stripId<10709)  ) stripId = stripId-1968;
	  if(  (stripId>10708)&& (stripId<10717)  ) stripId = stripId-976;
	
	 }

// fine  patch gruppo 48 49



//  patch gruppo 49camera BOSA012         
         if( head->runNumber() > 16266 &&   head->runNumber() < 16532) {
          //pannello eta left  camera 8
 
	  if  (stripId>8316 && stripId<8325) {stripId = 8225 - (stripId-8316)     ;} 
	  else if  (stripId>8216 && stripId<8225) {stripId = 8325 - (stripId-8216) ;}                  		       
          }


// patch gruppo 50 camera BOSA002 pos3 ultimo connettore  letto   da primo connetore camera in pos 2    
         if( head->runNumber() > 16532 &&   head->runNumber() < 16787) {
     
 
	  if  (stripId>4300 && stripId<4309) stripId = stripId+1032     ; 
	  if  (stripId>4200 && stripId<4209) stripId = stripId+1032     ; 
	  if  (stripId>4600 && stripId<4609) stripId = stripId+1032     ; 
	  if  (stripId>4700 && stripId<4709) stripId = stripId+1032     ; 
	  
	   }
	  


//  patch gruppo 56 camera BOSB082, BOSB013 and BOSB046       
         if( head->runNumber() > 17151 &&   head->runNumber() < 17269) {
	   //pannello eta right camera 5 c4
	   if  (stripId>5724 && stripId<5733) {stripId = 5633 - (stripId-5724)     ;} 
	   else if  (stripId>5624 && stripId<5633) {stripId = 5733 - (stripId-5624) ;}                  		       
	   //pannello eta right camera 10 c2
	   if  (stripId>10308 && stripId<10317) {stripId = 10217 - (stripId-10308)     ;} 
	   else if  (stripId>10208 && stripId<10217) {stripId = 10317 - (stripId-10208) ;}  	  
	  }
         if( head->runNumber() > 17151 &&   head->runNumber() < 17506) {
	   //pannello eta right camera 7 c4
	   if  (stripId>7724 && stripId<7733) {stripId = 7633 - (stripId-7724)     ;} 
	   else if  (stripId>7624 && stripId<7633) {stripId = 7733 - (stripId-7624) ;}                  		         
	  }




    
     //  patch gruppo 63 per le 4 camere BOSA in posizione 3/4/5/6
     //  usiamo tutti i canali della posizione 7 
         if( (head->runNumber() > 18811) && (head->runNumber() < 19160)){
 
   // lato left connettori down 	 	 
	 
	  if(  (stripId>7200)&& (stripId<7209)  ) stripId = stripId-3968;
	  if(  (stripId>7208)&& (stripId<7217)  ) stripId = stripId-2976;
	  if(  (stripId>7216)&& (stripId<7225)  ) stripId = stripId-1984; 
	  if(  (stripId>7224)&& (stripId<7233)  ) stripId = stripId-992; 
	 	  
   // lato left connettori up

	  if(  (stripId>7300)&& (stripId<7309)  ) stripId = stripId-3968;
	  if(  (stripId>7308)&& (stripId<7317)  ) stripId = stripId-2976;
	  if(  (stripId>7316)&& (stripId<7325)  ) stripId = stripId-1984; 
	  if(  (stripId>7324)&& (stripId<7333)  ) stripId = stripId-992; 

   // lato right connettori down 	 	 
	 
	  if(  (stripId>7600)&& (stripId<7609)  ) stripId = stripId-3968;
	  if(  (stripId>7608)&& (stripId<7617)  ) stripId = stripId-2976;
	  if(  (stripId>7616)&& (stripId<7625)  ) stripId = stripId-1984; 
	  if(  (stripId>7624)&& (stripId<7633)  ) stripId = stripId-992; 

   // lato right connettori up

	  if(  (stripId>7700)&& (stripId<7709)  ) stripId = stripId-3968;
	  if(  (stripId>7708)&& (stripId<7717)  ) stripId = stripId-2976;
	  if(  (stripId>7716)&& (stripId<7725)  ) stripId = stripId-1984; 
	  if(  (stripId>7724)&& (stripId<7733)  ) stripId = stripId-992; 
	
	 }

     //  patch gruppo 68 per le 2 camere BOSA in posizione 5/6
     //  usiamo tutti i canali della posizione 7 
         if( (head->runNumber() > 20392) && (head->runNumber() < 20685)){
 
   // lato left connettori down 	 	 
	 
	  if(  (stripId>7216)&& (stripId<7225)  ) stripId = stripId-1984; 
	  if(  (stripId>7224)&& (stripId<7233)  ) stripId = stripId-992; 
	 	  
   // lato left connettori up

	  if(  (stripId>7316)&& (stripId<7325)  ) stripId = stripId-1984; 
	  if(  (stripId>7324)&& (stripId<7333)  ) stripId = stripId-992; 

   // lato right connettori down 	 	 
	 
	  if(  (stripId>7616)&& (stripId<7625)  ) stripId = stripId-1984; 
	  if(  (stripId>7624)&& (stripId<7633)  ) stripId = stripId-992; 

   // lato right connettori up
        
	  if(  (stripId>7716)&& (stripId<7725)  ) stripId = stripId-1984; 
	  if(  (stripId>7724)&& (stripId<7733)  ) stripId = stripId-992; 
	
	 }



     //  patch gruppo 70 per la camere BOSA in posizione 6
     //  usiamo per gli ultimi connettori eta il quarto della posizione 5 
         if( (head->runNumber() > 20925) && (head->runNumber() < 21221)){
 
   // lato left connettori down 	 	 
	 
	  if(  (stripId>7224)&& (stripId<7233)  ) stripId = stripId+1000+8;
	 	  
   // lato left connettori up

	  if(  (stripId>7324)&& (stripId<7333)  ) stripId = stripId+1000+8;

   // lato right connettori down 	 	 
	 
	  if(  (stripId>7624)&& (stripId<7633)  ) stripId = stripId+1000+8;

   // lato right connettori up
 	 
	  if(  (stripId>7724)&& (stripId<7733)  ) stripId = stripId+1000+8;       
	  
	
	 }


     //  patch gruppo 78 
         if( (head->runNumber() > 23925) && (head->runNumber() < 24185)){
          // per la camere BOSB in posizione 3
          //  c1-c2 up phi right scambiato con il down 

          // lato right connettori c1	 	 
	 
	  if(  (stripId>3500) && (stripId<3509)  ) stripId = 3809 -(stripId-3500);
          else if(  (stripId>3800) && (stripId<3809)  ) stripId = 3509 -(stripId-3800);
        
	 // lato right connettori c2
	 
	  if(  (stripId>3508)&& (stripId<3517)  ) stripId  = 3817 -(stripId-3508);	  
	  else if(  (stripId>3808)&& (stripId<3817)  ) stripId = 3517 -(stripId-3808);
	  
	  // per la camere BOSE in posizione 8
          //  c8 down phi right scambiato con up

          // lato right connettori c8	 	 
	 
	  if(  (stripId>8156) && (stripId<8165)  )  {stripId = 8465 - (stripId-8156);}
          else if(  (stripId>8456) && (stripId<8465)  ) { stripId = 8165 -(stripId-8456);}
      
 	 
	 }




     //  patch gruppo 79 per la camere BOSA in posizione 6
     //  usiamo per gli ultimi connettori eta il quarto della posizione 8 
         if( (head->runNumber() > 24184) && (head->runNumber() < 24435)){
 
   // lato left connettori down 	 	 
	 
	  if(  (stripId>8224)&& (stripId<8233)  ) stripId = stripId-2000+8;
	 	  
   // lato left connettori up

	  if(  (stripId>8324)&& (stripId<8333)  ) stripId = stripId-2000+8;

   // lato right connettori down 	 	 
	 
	  if(  (stripId>8624)&& (stripId<8633)  ) stripId = stripId-2000+8;

   // lato right connettori up
 	 
	  if(  (stripId>8724)&& (stripId<8733)  ) stripId = stripId-2000+8;       
	  
	
	 }
	   

     //  patch gruppo 80 per le camere BOSB 122 123 cablato il connettore 5 dei
      // pannelli phi left sulle camere opposte
       
         if( (head->runNumber() > 24491) && (head->runNumber() < 24693)){
	 
  // da camera in posizione 5 BOSB122 a camera in posizione 6 BOSB123	 
	  if(  (stripId>7132)&& (stripId<7141)  ) stripId = stripId+10000;
	  if(  (stripId>7432)&& (stripId<7441)  ) stripId = stripId+10000;
	 	   
 
   // da camera in posizione 6 BOSB123 a camera in posizione 5 BOSB122	 	 
	 
	  if(  (stripId>8132)&& (stripId<8141)  ) stripId = stripId-1000;
	  if(  (stripId>8432)&& (stripId<8441)  ) stripId = stripId-1000;		 
	 
	  if(  (stripId>17132)&& (stripId<17141)  ) stripId = stripId-9000;
	  if(  (stripId>17432)&& (stripId<17441)  ) stripId = stripId-9000;
	  }
	  
	  
	  
     //  patch gruppo 87 per le camere BMLA in posizione 3,4,5
     //  usiamo per gli ultimi connettori eta il quarto della posizione 8 
         if( (head->runNumber() > 25935) && (head->runNumber() < 26235)){
    // pos 3
   // lato left connettori down 	 	 
	 
	  if(  (stripId>10224)&& (stripId<10233)  ) stripId = stripId-7000+8;
	 	  
   // lato left connettori up

	  if(  (stripId>10324)&& (stripId<10333)  ) stripId = stripId-7000+8;

   // lato right connettori down 	 	 
	 
	  if(  (stripId>7624)&& (stripId<7633)  ) stripId = stripId-4000+8;

   // lato right connettori up
 	 
	  if(  (stripId>7724)&& (stripId<7733)  ) stripId = stripId-4000+8;       
	  
   // pos 4
   // lato left connettori down 	 	 
	 
	  if(  (stripId>9224)&& (stripId<9233)  ) stripId = stripId-5000+8;
	 	  
   // lato left connettori up

	  if(  (stripId>9324)&& (stripId<9333)  ) stripId = stripId-5000+8;

   // lato right connettori down 	 	 
	 
	  if(  (stripId>9624)&& (stripId<9633)  ) stripId = stripId-5000+8;

   // lato right connettori up
 	 
	  if(  (stripId>9724)&& (stripId<9733)  ) stripId = stripId-5000+8;      
   // pos 5
   // lato left connettori down 	 	 
	 
	  if(  (stripId>7224)&& (stripId<7233)  ) stripId = stripId-2000+8;
	 	  
   // lato left connettori up

	  if(  (stripId>7324)&& (stripId<7333)  ) stripId = stripId-2000+8;

   // lato right connettori down 	 	 
	 
	  if(  (stripId>10624)&& (stripId<10633)  ) stripId = stripId-5000+8;

   // lato right connettori up
 	 
	  if(  (stripId>10724)&& (stripId<10733)  ) stripId = stripId-5000+8;       
          	 	  
	  if( head->runNumber() > 26025 ) {
	          
	  if(  (stripId>5524)&& (stripId<5533)  ) {stripId = stripId+1000;}   		  
	  else if(  (stripId>5540)&& (stripId<5549)  ) {stripId = stripId+1000;}   		  
	  else if(  (stripId>5824)&& (stripId<5833)  ) {stripId = stripId+1000;}    		  
	  else if(  (stripId>5840)&& (stripId<5849)  ) {stripId = stripId+1000;} 	  	          
	  else if(  (stripId>6524)&& (stripId<6533)  ) {stripId = stripId-1000;}   		  
	  else if(  (stripId>6540)&& (stripId<6549)  ) {stripId = stripId-1000;}	  	          
	  else if(  (stripId>6824)&& (stripId<6833)  ) {stripId = stripId-1000;}   		  
	  else if(  (stripId>6840)&& (stripId<6849)  ) {stripId = stripId-1000;}
	  }  		  		  
	 }
	 
	 
	 
	 
  //  patch gruppo 91 4 camere BMSA in posizione 3/4/5/6 
  //  usiamo tutti i canali della posizione 7   una BMLG in pos 7 
  //  sul lato left usiamo i connettori 3,4 della pos. 8 sul lato rigt i connettori 2,3 della pos. 8  
           
         if( (head->runNumber() > 26970) && (head->runNumber() < 269701)){
    //  patch per BMSA
   // lato left connettori down 	 	 
	 
	  if(  (stripId>7200)&& (stripId<7209)  ) stripId = stripId-3968;
	  if(  (stripId>7208)&& (stripId<7217)  ) stripId = stripId-2976;
	  if(  (stripId>7216)&& (stripId<7225)  ) stripId = stripId-1984; 
	  if(  (stripId>7224)&& (stripId<7233)  ) stripId = stripId-992; 
	  
   // lato left connettori up

	  if(  (stripId>7300)&& (stripId<7309)  ) stripId = stripId-3968;
	  if(  (stripId>7308)&& (stripId<7317)  ) stripId = stripId-2976;
	  if(  (stripId>7316)&& (stripId<7325)  ) stripId = stripId-1984; 
	  if(  (stripId>7324)&& (stripId<7333)  ) stripId = stripId-992; 

   // lato right connettori down 	 	 
	 
	  if(  (stripId>7600)&& (stripId<7609)  ) stripId = stripId-3968;
	  if(  (stripId>7608)&& (stripId<7617)  ) stripId = stripId-2976;
	  if(  (stripId>7616)&& (stripId<7625)  ) stripId = stripId-1984; 
	  if(  (stripId>7624)&& (stripId<7633)  ) stripId = stripId-992; 

        // lato right connettori up

	  if(  (stripId>7700)&& (stripId<7709)  ) stripId = stripId-3968;
	  if(  (stripId>7708)&& (stripId<7717)  ) stripId = stripId-2976;
	  if(  (stripId>7716)&& (stripId<7725)  ) stripId = stripId-1984; 
	  if(  (stripId>7724)&& (stripId<7733)  ) stripId = stripId-992; 
  
     // fine patch per bmsa
  
     //patch per BMLGS	
	 
	 
	   // lato left connettori down 	 	 
	 
	  if(  (stripId>8216)&& (stripId<8225)  ) stripId = stripId-1016;
	  if(  (stripId>8224)&& (stripId<8233)  ) stripId = stripId-1016;

	 // lato left connettori up	 
	 
	  if(  (stripId>8316)&& (stripId<8325)  ) stripId = stripId-1016; 
	  if(  (stripId>8324)&& (stripId<8333)  ) stripId = stripId-1016; 

         // lato right connettori down 	 	 
	  if(  (stripId>13216)&& (stripId<13225)  ) stripId = stripId-5616;
	  if(  (stripId>13224)&& (stripId<13233)  ) stripId = stripId-5616;

	 // lato left connettori up	 
	 
	  if(  (stripId>13316)&& (stripId<13325)  ) stripId = stripId-5616; 
	  if(  (stripId>13324)&& (stripId<13333)  ) stripId = stripId-5616; 

         // fine patch per  BMLGS	  
	 
	 
// fine patch per gruppo 91 	 
	 }
	 
	 
	 
	 
	 
	 
		 
	 
	 
	 
	 	   

         GeoRPCStripPlane * theplane=geo->findStripPlane(stripId/100);
//	           std::cout << "Latch::StripList: searching for stripid " 
//                       << stripId << std::endl;
// then strip pointer 
         if(theplane){
           GeoRPCStrip * thestrip=theplane->findStrip(stripId);
// create and add to striplist (these objects will be stored in RPCStripPlanes
// and deleted from the RPCStripPlanes destructors
           if(thestrip){
             striplist.push_back(new RPCStrip(thestrip,time));
           } else {
	     errors++;
	     if(errors<=100){
               std::cout << "Latch::StripList: stripId " 
               << stripId << " not found" << std::endl;
             }
	   }
         } else {
           std::cout << "Latch::StripList: plane with stripId " 
                       << stripId << " not found" << std::endl;
           std::cout << "addr,run " <<  std::setbase(16) << m_address << 
           std::setbase(10) << " " << head->runNumber() << std::endl;
         }
      }
    }
  }
  return striplist;
}
