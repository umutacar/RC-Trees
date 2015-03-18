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

/////////////////////////////////////////////////////////////////////////
// interface.c
// 
// Jorge Luis Vittes
//
// auxilary functions, and extra data structures for the tree-contraction
//
/////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Globals.h"
#include "Interface.h"
#include "Contract.h"
#include "Application.h"

/////////////////////////////////////////////////////////////////////
// Create RC Forest
/////////////////////////////////////////////////////////////////////
RC_Forest::RC_Forest(int n) {
  cQueue = initQueue();
  cTree = makeEmptyTree(n);
  initTreeContraction(cQueue,cTree);
}

RC_Forest::~RC_Forest() {
  destructTree(cTree);
  destructQueue(cQueue);
}

//////////////////////////////////////////////////////////////////////
// Structural Changes: link and cut
//////////////////////////////////////////////////////////////////////
int RC_Forest::link(node* thisNode, node* otherNode, int w)
{
  int isLeaf1, isLeaf2;
  
  isLeaf1 = isLeaf(thisNode);   
  isLeaf2 = isLeaf(otherNode);   

  bin_data data = makeEdgeData (thisNode->nId, otherNode->nId, w); 
  deprintf("Adding edge with weight %d from %d to %d\n",
	   w,thisNode->nId,otherNode->nId);
  if (addEdge(thisNode,otherNode,data,cTree)) {

    // Queue neighbors if leaf status changes
    if (isLeaf1) {
      insertQueue (GET_NEIGHBOR(thisNode->scars[thisNode->left].backscar),
		   cQueue);
      insertQueue (GET_NEIGHBOR(thisNode->scars[thisNode->right].backscar),
		   cQueue);
    }

    if (isLeaf2) {
      insertQueue (GET_NEIGHBOR(otherNode->scars[otherNode->left].backscar),
		   cQueue);
      insertQueue (GET_NEIGHBOR(otherNode->scars[otherNode->right].backscar),
		   cQueue);
    }
   
    insertQueue (thisNode,cQueue);
    insertQueue (otherNode,cQueue);
  
    assert (verifyTree(cTree)); 
    return 1;
  }
  deprintf("Insertion FAILED\n");
  // Insertion failed.
  return 0;
}

//////////////////////////////////////////////////////////////////////
// cut the edge between v1, and v2
//////////////////////////////////////////////////////////////////////
void RC_Forest::cut(node* v1, node* v2)
{
  int deg;
  deprintf("deleting edge from %d to %d\n",v1->nId,v2->nId);
  if(deleteEdge(v1,v2,cTree))   {
      deg = v1->degree;
      if(deg == 1)
	insertQueue(GET_NEIGHBOR(v1->scars[v1->left].backscar),cQueue);
      deg = v2->degree;
      if(deg == 1)
	insertQueue(GET_NEIGHBOR(v2->scars[v2->left].backscar),cQueue);
     
      insertQueue(v1,cQueue);
      insertQueue(v2,cQueue);
    }
  else
    {
      printf("ERROR NO SUCH EDGE\n");exit(-9);
    }
}

///////////////////////////////////////////////////////////////////
// return true (1) if there is an edge between veritices of id n1
// and n2 in the cached tree, otherwise, false (0)
///////////////////////////////////////////////////////////////////
int RC_Forest::isEdge(node* v1, node* v2) 
{
  int i;
  for(i=0;i<MAX_DEGREE;i++)
    {
      if((GET_NEIGHBOR(v1->scars[i].backscar)) == v2)
	return 1;
    }
  return 0;
}

//////////////////////////////////////////////////////////////////////
// Application-Specific Data Changes: changeVertexData and changeEdgeData
// The function f must be supplied by the application
//////////////////////////////////////////////////////////////////////

void RC_Forest::changeVertexData (node* v, void (*f)(unary_data* )) 
{
  unary_cluster* ucl = (unary_cluster*) v->data;
  (*f)(&(ucl->data)); 
  deprintf("changing data on %d\n",v->nId);
  deprintf("inserting into pq %p\n",affectedClusters);
  insertCluster(v->data,&affectedClusters);
}

void RC_Forest::changeEdgeData (node* v, node* u, void (*f)(bin_data*))
{
  bin_cluster* c = findEdgeCluster (v,u); 
  deprintf("changing edge from %d to %d \n",v->nId, u->nId);
  assert (c); 
  (*f)(&(c->data)); 
  deprintf("inserting into pq %p\n",affectedClusters);
  insertCluster(c,&affectedClusters);

}


//////////////////////////////////////////////////////////////////////
// Compute the value for the root cluster
//////////////////////////////////////////////////////////////////////

// A util function for updating the tag of a cluster
void updateCluster(cluster* cl)
{
  PURIFY(&cl);
  cl -> synchronize ();
  cl -> affected = NOT_AFFECTED;
 
}

//////////////////////////////////////////////////////////////////////
// Propagate changes in the priority queue
//////////////////////////////////////////////////////////////////////
cluster* RC_Forest::propagate()
{
  cluster* cl;
  cluster *root;

   while(affectedClusters.head)   {
    cl = removeCluster(&affectedClusters);
    cl = cl->parent;
    while (cl) {
      root = cl; 
      updateCluster(cl);
      cl = cl->parent; 
    }
  }
  return root; 
}

void RC_Forest::contract()
{
  rerun(cQueue,cTree,1);
}
