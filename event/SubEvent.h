/** 
 * \class SubEvent 
 * base class for subdetector dependent event information 
 * \author D.Orestano
 * \date 25.01.00
 */

#ifndef SUBEVENT_H
#define SUBEVENT_H


class SubEvent {

   public:
      /**
       * the constructor
       * takes two integer arguments: type, the subdetector type as defined
       * in DetCreator detId, and id, the subdetector identifier 
       */
      SubEvent(int type,int id);

      /**
       * the destructor 
       * has to be virtual since the detectors implementation will certainly
       * contain objects that will have to be deleted explicitely
       */
      virtual ~SubEvent();

      /**
       * clear the event dependent information for this subdetector
       * each implementation must provide this method
       */
      virtual void clear()=0;

      /**
       * print the event dependent information for this subdetector
       * each implementation must provide this method
       */
      virtual void print()=0;

      int id() const; /**< return the detector identifier */
      int type() const; /**< return the detector type */

   protected:    

      int m_type; /**< detector type */
      int m_id;  /**< detector identifier */ 
};

#endif // SUBEVENT_H
