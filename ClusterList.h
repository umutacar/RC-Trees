/*************************************************************************
 * ClusterList.h
 *
 * Jorge Vittes
 *
 * A structure to store a list of clusters for returning after contraction
 ************************************************************************/
#include "Globals.h"
#include "Data.h"
#include "BinCluster.h"
#include "UnaryCluster.h"

#ifndef _CLUSTER_LIST_H_
#define _CLUSTER_LIST_H_ 1

typedef struct clusterNode{
  cluster* cl;
  struct clusterNode *next;
}clusterNode;

class clusterList{
 public:
  clusterList();
  clusterNode* head;
};

void initClusterList();
void insertCluster(cluster* cl, clusterList* list);
cluster* removeCluster(clusterList* list);

#endif
