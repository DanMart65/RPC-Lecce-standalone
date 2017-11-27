/** 
 * \class DetCreator 
 * a factory method for the construction of the SubEvents 
 * \author D.Orestano
 * \date 29.02.00
 */

#ifndef DETCREATOR_H
#define DETCREATOR_H

#include <vector>
#include "tools/Utils.h"
#include "event/EventHandler.h"

//class GeoMDTMultiLayer;
class GeoRPCStripPlane;
class GeoRPCChamber;
class SubEvent;

class DetCreator {

public:
    /**
     * DetCreator::detId
     * to define detector types
     */
    enum detId {MCINFO=0,MDTML,TRKML,RPC,RPCC};

protected:
    /**
     * allow only the EventHandler to create SubEvents 
     */
    friend bool EventHandler::firstEvent();

    /**
     * the constructor 
     */
    DetCreator();

    /**
     * the destructor
     */
    ~DetCreator();

    /**
     * construct the SubEvent list for the EventHandler
     * \warning will have to be motified as soon as detectors other than MDTs
     * will be defined
     */
     std::vector<SubEvent *> activeDetectors(); 

private:
    /**
     * the SubEvent creator
     * calls the correct constructor accordingly to objectType
     */
//    virtual SubEvent * Create(int objectType, int objectId,
//                              GeoMDTMultiLayer * ml);
    virtual SubEvent * Create(int objectType, int objectId,
                              GeoRPCStripPlane * ml);
   virtual SubEvent * Create(int objectType, int objectId,
                              GeoRPCChamber * ml);
};
 
#endif // DETCREATOR_H
