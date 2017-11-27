#include "detector/DetCreator.h"
//#include "detector/MDTMultiLayer.h"
//#include "detector/TRKMultiLayer.h"
#include "detector/RPCChamber.h"
#include "detector/RPCStripPlane.h"
//#include "detector/MCData.h"
#include "database/GeometryHandler.h"
//#include "database/GeoMDTMultiLayer.h"
#include "database/GeoRPCStripPlane.h"
#include "database/GeoRPCChamber.h"
DetCreator::DetCreator() 
{
    MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
    Memory->increase();
}

DetCreator::~DetCreator() 
{
    MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
    Memory->decrease();
}

std::vector<SubEvent *> DetCreator::activeDetectors()
{
    std::vector<SubEvent *> list;
    GeometryHandler * geo = GeometryHandler::getGeometryHandler();

//
/*    for(int j=0; j<geo->nMultiLayer(); j++) {
      GeoMDTMultiLayer * gmultiPtr = geo->multiLayerPtr(j);
      int id    = gmultiPtr->idMultiLayer();
      list.push_back(Create(MDTML,id,gmultiPtr));
    }
*/
// add RPCChambers to subevent list
    RPCChamber * chPtr;
    for(int j=0; j<geo->nRPCChamber(); j++) {
      GeoRPCChamber * gChamPtr = geo->RPCChamberPtr(j); 
      list.push_back(Create(RPCC,gChamPtr->Chamber(),gChamPtr));
    }    
// ???                    
//    GeoMDTMultiLayer * dummy;
//    list.push_back(Create(MCINFO,0,dummy));
    return list;
}

/*
SubEvent * DetCreator::Create(int objectType, int objectId, GeoMDTMultiLayer * ml) 
{
    if(objectType == MDTML) return new MDTMultiLayer(objectId,ml);
    if(objectType == TRKML) return new TRKMultiLayer(objectId,ml);
    if(objectType == MCINFO) return new MCData(MCINFO,objectId);
    return 0;
}
*/
// remove not needed anymore....
SubEvent * DetCreator::Create(int objectType, int objectId, GeoRPCStripPlane * ml) 
{
    if(objectType == RPC) return new RPCStripPlane(objectId,ml);
    return 0;
}

SubEvent * DetCreator::Create(int objectType, int objectId, GeoRPCChamber * gChamPtr) 
{
    if(objectType == RPCC){
        return new RPCChamber(objectId,gChamPtr);
    } else return 0;
}


