/*
 * \class MapHandler 
 * a Template Sigleton class, inheriting from std::map
 * for the handling of timers
 * 
 * \author E.Gorini
 * \date 8.5.03
 */

#ifndef MAPHANDLER_H
#define MAPHANDLER_H
#include <map>
#include <string>

template <class T>
class MapHandler : public std::map< std::string , T* , std::less< std::string> > {
   public:
      //gab static MapHandler &MapHandler::getHandle(){return Singleton;}
      static MapHandler &getHandle(){return Singleton;}
   private:
      static MapHandler Singleton;
      MapHandler(){};
};

#endif // HANDLER_H
