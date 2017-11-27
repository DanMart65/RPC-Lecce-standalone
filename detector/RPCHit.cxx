#include "detector/RPCHit.h"

RPCHit::RPCHit() {

    MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
    Memory->increase();   
}

RPCHit::~RPCHit(){
   MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
   Memory->decrease();   
}
