#include "database/GeoRPCStrip.h"
#include "detector/RPCStrip.h"
#include "detector/RPCCluster.h"
#include <math.h> 

  RPCCluster::RPCCluster(std::vector<RPCStrip *> stripList):RPCHit(){ 
     //TO BE PUT IN GLOBAL OR READ OUT FROM DATACARDS
	 //THIS IS VALID FOR <CLUSTER SIZE>=1.2
     float pitchsize2 = 0.2;
	 float realpitch=1.0-pitchsize2;

	 
	 
	 std::vector<RPCStrip *>::iterator it;
     int s,p,c;
     double x1,y1,z1,x2,y2,z2;
     double stripWidth;
     m_first=1000;
     m_last=-1000;
     m_plane=(*(stripList.begin()))->plane();
     m_chamber=(*(stripList.begin()))->chamber();
     m_stripDir=(*(stripList.begin()))->geo()->StripDir();
     m_size=stripList.size();
     m_idsp=m_chamber*10+m_plane;
// hardwired assume numerazione fissata in Camera RPC    
     m_gap=(m_plane-1)/2+1;
     m_layer=(m_gap-1)%2+1;
//     std::cout << " new cluster : chamber " << m_chamber <<" plane "  << m_plane 
//     << " gap " << m_gap << " layer " << m_layer << "\n";
     
     stripWidth=(*(stripList.begin()))->geo()->StripWidth();
//     std::cout << " new cluster : plane " << m_plane <<" chamber " 
//          << m_chamber << " size " << m_size << " ";
     for(it=stripList.begin(); it!=stripList.end(); ++it) {
        s = (*it)->strip();
        p = (*it)->plane();
        c = (*it)->chamber();
        int t = (*it)->time();
//        std::cout << s << " " ;
// check here if strip is repeated 
        if(s == m_first || s == m_last) {
          std::cout << " error equal strip" << s << " " << t << std::endl;
        }  
        if(s > m_last) { 
            m_last=s;
            x2=(*it)->geo()->xyzStrip().X();
            y2=(*it)->geo()->xyzStrip().Y();
            z2=(*it)->geo()->xyzStrip().Z();
        }
        if(s < m_first) {
            m_first=s;
            x1=(*it)->geo()->xyzStrip().X();
            y1=(*it)->geo()->xyzStrip().Y();
            z1=(*it)->geo()->xyzStrip().Z();
        }      
        if( (p != m_plane) || (c != m_chamber) ) {
          std::cout << "RPCCluster::RPCCluster ERROR (plane and chamber)" << std::endl;
          exit(1);
        }
     } 
//     std::cout << std::endl;
//     std::cout << " first : "<< m_first << " last : " << m_last << std::endl;
     if( (m_last-m_first+1) != m_size ) {
          std::cout << "RPCCluster::RPCCluster ERROR (cluster size)" << std::endl;
          exit(1);
     } 
     m_x = (x1+x2)/2.;
     m_y = (y1+y2)/2.;
     m_z = (z1+z2)/2.;
     if(m_stripDir == 0) { 
         m_err = (x2-x1);
     } else if( m_stripDir == 1) {
         m_err = (y2-y1);
     } else if( m_stripDir == 2) {
         m_err = (z2-z1);
     }
     m_err += stripWidth;
     m_err /=sqrt(12);

// Defined by realpitch 	 	 
	 if(m_chamber != 1){
	   if(m_size == 1)  m_err *= realpitch;
       else if(m_size == 2)  m_err *=pitchsize2;
     } else {
// for chamber 1 use a fixed resolution (dominated by m.s)
	   m_err=18;
	 }
     
     m_list = stripList;
     MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
     Memory->increase();

  }

  RPCCluster::~RPCCluster() {     
     MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
     Memory->decrease();
     m_list.clear();
  }          

  void RPCCluster::print() {        
  std::cout << "RPCcluster: plane " << m_plane << " chamber " << m_chamber << std::endl ;   
  std::cout << "RPCcluster: first,last strip " << m_first << " " << m_last; 
  std::cout << ",  size,dir " << m_size << " " << m_stripDir << std::endl;
  std::cout << std::endl;
}
  int   RPCCluster::firstStrip()  { return m_first; } 
  int   RPCCluster::lastStrip() { return m_last; }  
  int   RPCCluster::size()  { return m_size; }     
  int   RPCCluster::plane()  { return m_plane; }   
  int   RPCCluster::chamber()  { return m_chamber; } 
  int   RPCCluster::stripDir()  { return m_stripDir; } 
  
  RPCCluster *   RPCCluster::ptr() { return this; } 

  std::vector<RPCStrip *>   RPCCluster::stripPtr() { return m_list; }

  double   RPCCluster::X() { return m_x; }
  double   RPCCluster::Y() { return m_y; }
  double   RPCCluster::Z() { return m_z; }
  double   RPCCluster::Err() { return m_err; }
