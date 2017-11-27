#include "event/SubEvent.h"
#include "tools/Utils.h"

SubEvent::SubEvent(int type,int id):m_type(type),m_id(id)
{ 
    MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
    Memory->increase();
}

SubEvent::~SubEvent()
{  
    MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
    Memory->decrease();
}

int SubEvent::id() const 
{
   return m_id;
}

int SubEvent::type() const 
{
   return m_type;
}
