/** 
 * \class RPCHit 
 * abstract base class for all the types of RPCHits (Strips, Clusters...) 
 * \author D.Orestano, E.Gorini 
 * \date 25.01.01
 */

#ifndef RPCHIT_H
#define RPCHIT_H

#include "tools/Utils.h"

class RPCHit {
   public:
        RPCHit();
        virtual ~RPCHit();
	virtual double X()=0;
	virtual double Y()=0;
	virtual double Z()=0;
	virtual double Err()=0;
        virtual int size()=0 ;      /**< return the number of strips */ 
        virtual int plane()=0 ;     /**< return the plane number */ 
        virtual int chamber()=0 ;   /**< return the chamber number */ 
	virtual int stripDir()=0 ;   /**< return the plane orientation */
	virtual void print()=0;		
	virtual int  firstStrip()=0;	
};
#endif //RPCHIT_H
