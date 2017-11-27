/**
 * \class RPCEffi
 * example of user class for RPC Single plane efficiency
 *
 * \author M.Primavera
 * \date 17.07.01
 */

#ifndef RPCEFFI_H
#define RPCEFFI_H
#include <string>
#include <iostream>

class RPCEffi {
   public:
      RPCEffi();             /**< the constructor */
      RPCEffi(std::string name);             /**< the constructor */
      ~RPCEffi();    /**< the destructor */
      double effic();
      double deffic();
      void IncDenom();
      void  IncNum(); 
      std::string Name();     
      void  RPCeffcomp();
      void print(); 
      void print(std::ostream &EffFile);

   private:
      int m_numer; 
      int m_denom;
      double m_effic;
      double m_deffic;
      std::string m_name;   

};

#endif //RPCEFFI
