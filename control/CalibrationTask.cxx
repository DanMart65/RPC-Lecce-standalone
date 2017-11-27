#include "control/CalibrationTask.h"
#include "tools/Utils.h"


CalibrationTask::CalibrationTask():m_order(0),m_name("task")
{
   MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
   Memory->increase();                                                          
}

CalibrationTask::~CalibrationTask()
{
   MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
   Memory->decrease();                                                          
}

void CalibrationTask::setOrder(int position ) {m_order=position;}

int CalibrationTask::getOrder() const {return m_order;}

std::string CalibrationTask::Name() const {return m_name;}