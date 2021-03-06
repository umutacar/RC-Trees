///////////////////////////////////////////////////////////////////////////
// ClusterList.c
//
// Jorge Vittes
//
// A structure to store a list of cluster to return from contraction
///////////////////////////////////////////////////////////////////////////

#include "FreeList.h"
#include "Vertex.h"
#include "ClusterList.h"

FreeList* theList;

///////////////////////////////////////////////////////////////////////////
// initialize the cluster list 
///////////////////////////////////////////////////////////////////////////
void initClusterList()
{
  theList = initFreeList(sizeof(clusterNode));
}

///////////////////////////////////////////////////////////////////////////
// Default Contstructor have the head be NULL
///////////////////////////////////////////////////////////////////////////
clusterList::clusterList()
{
  head = NULL;
}

///////////////////////////////////////////////////////////////////////////
// insert cluster cl into the list 
///////////////////////////////////////////////////////////////////////////
void insertCluster(cluster* cl, clusterList* list)
{
  clusterNode* newBlock;
  newBlock = (clusterNode *) allocBlock(theList);
  newBlock->cl = cl;
  newBlock->next = list->head;
  list->head = newBlock;
 
}

///////////////////////////////////////////////////////////////////////////
// remove the first cluster from the list 
///////////////////////////////////////////////////////////////////////////
cluster* removeCluster(clusterList* list)
{
  if(list->head == NULL) return NULL;
  else
    {
      cluster* ret = list->head->cl;
      clusterNode* prev = list->head;
      list->head = list->head->next;
      freeBlock(theList,(char*)prev);
      return ret;
    }
}
