/*
 * \class Handler 
 * a Template Sigleton class, inheriting from std::vector
 * for the handling of the Header, DCS, etc blocks...
 * 
 * \author E.Gorini
 * \date 22.4.03
 */

#ifndef HANDLER_H
#define HANDLER_H
#include <vector>

template <class T>
class Handler : public std::vector<T*> {
   public:
      //gab static Handler &Handler::getHandle(){return Singleton;}
      static Handler &getHandle(){return Singleton;}
   private:
      static Handler Singleton;
      Handler(){};
};

#endif // HANDLER_H
