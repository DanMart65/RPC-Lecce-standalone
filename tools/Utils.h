#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <iomanip>                                                              

#ifndef NULL
#ifdef __GNUG__
#define NULL (__null)
#else
#define NULL (0)
#endif
#endif                                                                          
// removed for gcc 3.1 !!! 
//#ifndef HAVE_NAMESPACE_STD
//    #define std
//#endif

#include "MemoryChecker.h"

#endif // UTILS_H
