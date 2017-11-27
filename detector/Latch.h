/** 
 * \class Latch 
 * a class for the handling of the Lecce RPC  
 * \author E.Gorini
 * \date 14.2.03
 */
#ifndef LATCH_H
#define LATCH_H
#include <vector>
#include "detector/RPCStrip.h"

#define CLOCK 15

class Latch {

   public:
       Latch(unsigned long int *buffer, unsigned long int size); 	 			        // constructor
       ~Latch();					 		// destructor
       std::vector<RPCStrip*> StripList();				// returns list of decoded strips
   private:
       int m_address;
       std::vector<unsigned long int> m_words;
};

#endif // LATCH_H
