/** 
 * \class Header, abstract base class 
 * a class to contain RUN Header info 
 * \author E.Gorini
 * \date 21.4.03
 */

#ifndef HEADER_H
#define HEADER_H
#include <string>
#include <fstream>
#include <vector>

class Header {

   public:
      Header();  /** constructor */
      ~Header(); /** destructor */
      virtual void print() const=0;
       
      int runNumber() const;   /**< return the actual runNumber */ 
      std::string date() const; /**< data taking date */ 
      int TriggerType() const;
      int NumEvents() const;

// X5 specific keep here for the moment      
      int phiOffset(int cham) const;

   
   protected:
      int m_runNumber;   /**< run number */ 
      std::string m_date; /**< data taking date */
      int m_NumEvents;
      int m_TriggerType;    
      unsigned long int invertbytes (unsigned long int dat);  

// X5 specific keep here for the moment            
      int m_phiOffset[3];   
};

#endif // HEADER_H
