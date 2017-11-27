#include "tools/Utils.h"
#include "tools/MemoryChecker.h"


MemoryChecker* MemoryChecker::m_Memory = 0;

MemoryChecker* MemoryChecker::getMemoryChecker()
{
		if(!m_Memory) {
				m_Memory = new MemoryChecker;
		}	
		return m_Memory;
}

MemoryChecker::MemoryChecker():m_objects(0)
{
//		if(m_Memory) abort;
		m_Memory = this;
}

MemoryChecker::~MemoryChecker(){}

int MemoryChecker::objects(){return m_objects;} 

void MemoryChecker::increase(){m_objects++;} 

void MemoryChecker::decrease(){m_objects--;} 

