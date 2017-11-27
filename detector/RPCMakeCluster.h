/**
 * \class RPCCluster
 * takes the strips and returns a list of clusters
 * \author D.Orestano, E.Gorini
 * \date 25.01.01
 **/
#ifndef RPCMAKECLUSTER_H
#define RPCMAKECLUSTER_H

#include <vector>
#include "tools/Utils.h"
#include "detector/RPCCluster.h"

class RPCCluster;

class RPCMakeCluster {
    public:
       RPCMakeCluster(); /**< default constructor, uses all the strips */
       ~RPCMakeCluster(); /**< destructor */
       std::vector<RPCCluster *> clusterize(int timemin , int timemax); /**< clusterization */
    private:
       // put here cuts, parameters...
};
#endif
