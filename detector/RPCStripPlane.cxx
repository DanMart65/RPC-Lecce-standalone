#include "event/EventHandler.h"
#include "database/GeometryHandler.h" 
#include "database/GeoRPCStripPlane.h"
#include "detector/RPCStripPlane.h"
#include "detector/Latch.h"
//gabriele#include "algo.h"

RPCStripPlane::RPCStripPlane(int Id, GeoRPCStripPlane * ml):
    SubEvent(3,Id), m_geometry(ml)
{ 
    MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
    Memory->increase();
}

RPCStripPlane::~RPCStripPlane()
{  
    MemoryChecker* Memory = MemoryChecker::getMemoryChecker();
    Memory->decrease();
    clear();
}

void RPCStripPlane::clear()
{
   if(!m_fullhitList.empty()) {
       std::vector<RPCStrip*>::iterator  ih;
       for( ih=m_fullhitList.begin(); ih!=m_fullhitList.end(); ++ih) {
           delete *ih;
       }
   }
   m_fullhitList.clear();
   m_hitList.clear();
   
   if(!m_clusList.empty()) {
       std::vector<RPCCluster*>::iterator  ih;
       for( ih=m_clusList.begin(); ih!=m_clusList.end(); ++ih) {
           delete *ih;
       }
   }
   m_clusList.clear();
}
/*************************************************************************
 *
 * Add hit to the Strip List in StripPlane, 
 *
 *************************************************************************/
void RPCStripPlane::addHit(RPCStrip * hit)
{
         m_fullhitList.push_back(hit);
}

std::vector<RPCStrip*> RPCStripPlane::CompleteHitList() 
{
    return m_fullhitList;
}

std::vector<RPCStrip*> RPCStripPlane::hitList() 
{
    return m_hitList;
}

std::vector<RPCCluster *> RPCStripPlane::clusList() 
{
    return m_clusList;
}


void RPCStripPlane::printFull()
{
   if(!m_fullhitList.empty()) {
       std::cout << "RPCStripPlane::full strip list  for Plane: " << m_id << std::endl;  
       std::vector<RPCStrip*>::iterator  ih;
       for( ih=m_fullhitList.begin(); ih!=m_fullhitList.end(); ih++) {
           (*ih)->print();
       }
   } else {
       std::cout << "RPCStripPlane::full strip list  for Plane: " << m_id << " is empty" << std::endl;
   }
}

void RPCStripPlane::print()
{
   if(!m_hitList.empty()) {
       std::cout << "RPCStripPlane::strip list  for Plane: " << m_id << std::endl;  
       std::vector<RPCStrip*>::iterator  ih;
       for( ih=m_hitList.begin(); ih!=m_hitList.end(); ih++) {
           (*ih)->print();
       }
   } else {
       std::cout << "RPCStripPlane::strip list  for Plane: " << m_id << " is empty" << std::endl;
   }
}
GeoRPCStripPlane * RPCStripPlane::RPCgeometry()
{
   return m_geometry;
}

void RPCStripPlane::Consolidate(int timemin, int timemax){
   sortstrips();
   TimeClusterize();
   Clusterize(timemin,timemax);

}

void RPCStripPlane::sortstrips()
{
   StripCompare scomp;
   sort(m_fullhitList.begin(),m_fullhitList.end(),scomp);
}

// returns only first strip of a group in time. 
void RPCStripPlane::TimeClusterize()
{
       std::vector<RPCStrip*>::iterator  ih;
       int laststrip=0,lasttime=-1;
       for( ih=m_fullhitList.begin(); ih!=m_fullhitList.end(); ih++) {
          if(laststrip != (*ih)->strip() ){
            laststrip=(*ih)->strip();
            lasttime =(*ih)->time();
//            std::cout << "  new strip: " << laststrip << " " << lasttime << std::endl;
            m_hitList.push_back(*ih);
          } else {
 // same strip, check time         
             if(lasttime != ((*ih)->time()+CLOCK) ){
             // new time cluster
//               std::cout << " keep" << (*ih)->strip() << " " << (*ih)->time() << std::endl;
               m_hitList.push_back(*ih);               
             } else {
             // adjacent in time, remove from list
//                 std::cout << " remove " << (*ih)->strip() << " " << (*ih)->time() << std::endl;
             }
             lasttime =(*ih)->time();
          }
       }
}

/*********************************************************************
 * This method clusterize the strips, assuming they are ordered 
 * ordering is done in sortstrips. It Selects only strip between
 * timemin and timemax. Only one hit per time is taken and the time is 
 
 * 
 *********************************************************************/
void RPCStripPlane::Clusterize (int timemin=0, int timemax=480) {
 
     std::vector<RPCStrip*>::iterator is; 
     std::vector<RPCStrip*> strip_in_cluster; // list of strips in a cluster
      
 
	  int ilast=-1000;     // last strip used in the cluster

/* loop on the list of strip in that plane  */
          
          for(is=m_hitList.begin(); is!=m_hitList.end(); is++) {
           if( (*is)->time() >= timemin && (*is)->time() <= timemax ) {
            
/*            std::cout << " Makecl numero strip e piano" << (*is)->strip() << " " <<
	    (*is)->plane() << " " << (*is)->chamber() << std::endl;*/
            
            if( (*is)->strip() == ilast ){
              // same strip (repetition), exit loop
              continue;         
            }else if( (*is)->strip() != (ilast+1) ) {
              // found new cluster ! 
              // store the previous one in the list of clusters 
              // WARNING: each new cluster is created with the new keyword
              //          the code invoking the clusterization is responsible
              //          for deleting these clusters!!!!!
              if(ilast != -1000) {
                 m_clusList.push_back(new RPCCluster(strip_in_cluster));
		 
	      }	  
              // reset the list of strips
              while (!strip_in_cluster.empty()) strip_in_cluster.pop_back();
              strip_in_cluster.clear();
            } 
            // start the new one: put the strip in the list of strips
	    // and reset the ilast counter  
            strip_in_cluster.push_back(*is);
            ilast=(*is)->strip();
           }
          }
	  // this needed if only one strip in plane, create the single cluster
	  // with the single strip in plane
          if(!strip_in_cluster.empty()){
            m_clusList.push_back(new RPCCluster(strip_in_cluster));
	  }
	  // reset the list of strips 
          while (!strip_in_cluster.empty()) strip_in_cluster.pop_back();
          strip_in_cluster.clear();
}

int RPCStripPlane::NearestClusterAt(double estrap){
//  loop on clusters to search the nearest Cluster center to the estrapolation            
            int nearClus=-1;
            float residmin=999999.0,resid,pullmin,pull;
// get the cluster list for this plane and loop on that
            for(int i=0; i!=m_clusList.size();i++) {
// get cluster center and compute residual and pull 
              double cent=  m_clusList[i]->X();
              if(m_geometry->StripDir()==2)cent=m_clusList[i]->Z();
              resid=cent-estrap;
              pull=resid/m_clusList[i]->Err();
// search and store minimum distance                   
              if(fabs(resid) <= fabs(residmin)){
// set values and pointer for minimum distance from center
                residmin=resid;
                pullmin=pull;
                nearClus=i;
              }  // end resid  
            }// end for thislayer
       
            return nearClus;

}
