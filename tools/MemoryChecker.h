/** 
 * \class MemoryChecker
 * an utility to check that objects are correctly deleted
 * \author D.Orestano
 * \date 23.01.00
 */

#ifndef MEMORYCHECKER_H
#define MEMORYCHECKER_H

class MemoryChecker {
		public: 
				static MemoryChecker* getMemoryChecker();
				MemoryChecker();
				~MemoryChecker();
				int objects();
				void increase();
				void decrease();
		private:
				static MemoryChecker* m_Memory;
				int m_objects;
};

#endif // MEMORYCHECKER_H
