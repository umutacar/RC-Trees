////////////////////////////////////////////////////////////////////////
// Copyright (c) 2004
// Carnegie Mellon University:
// Guy Blelloch, Umut Acar, Jorge Vittes
//
// All Rights Reserved.
//
// Permission to use, copy, modify and distribute this software and its
// documentation is hereby granted, provided that both the copyright
// notice and this permission notice appear in all copies of the
// software, derivative works or modified versions, and any portions
// thereof.
//
// CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
// CONDITION.  CARNEGIE MELLON AND THE SCAL PROJECT DISCLAIMS ANY
// LIABILITY OF ANY KIND FOR ANY DAMAGES WHATSOEVER RESULTING FROM
// THE USE OF THIS SOFTWARE.
//
// The ALADDIN requests users of this software to return to
//
//  Guy Blelloch                         blelloch@cs.cmu.edu
//  School of Computer Science
//  Carnegie Mellon University
//  5000 Forbes Ave.
//  Pittsburgh PA 15213-3890
//
// any improvements or extensions that they make and grant Carnegie Mellon
// the rights to redistribute these changes.
///////////////////////////////////////////////////////////////////////

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
  if (theList == NULL) {
    theList = initFreeList(sizeof(clusterNode));
  }
}

///////////////////////////////////////////////////////////////////////////
// Default Contstructor have the head be NULL
///////////////////////////////////////////////////////////////////////////
clusterList::clusterList()
{
  head = NULL;
}

clusterList::~clusterList()
{
  while (head != NULL) {
    removeCluster(this);
  }
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
