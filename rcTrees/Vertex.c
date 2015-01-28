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


//////////////////////////////////////////////////////////////////////
// Vertex.c
//
// Umut A. Acar and Jorge Vittes
//
//////////////////////////////////////////////////////////////////////

#include "Vertex.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "AMath.h"
#include "Tree.h"
#include "FreeList.h"
#include "FinalCluster.h"

extern char*   HERE;

/* Return the maximum of two integers */
int max(int a, int b) 
{
  if(a > b) return a;
  return b;
}

vertex_t::vertex_t(node* nd)
{
  v = nd;
  cl = NULL;
}

vertex_t::vertex_t()
{
  v = NULL;
  cl = NULL;
}

/* print some of the vertex information */
void printVertex (node* v) 
{
  deprintf ("VERTEX:\n");
  deprintf ("\t id = %d\n", v->nId); 
  deprintf ("\t degree = %d\n", v->degree); 

}

/* copy the vertex into its decendant */
node* copyVertex (node* n, FreeList* flist)
{
  node* newVertex =  (node *) allocBlock(flist);
  int i;
  newVertex->id  = rand ();
  newVertex->nId = n->nId;
  //newVertex->cl  = n->cl;
  newVertex->height = n->height + 1;

  newVertex->right = -1;
  newVertex->left = -1;
  newVertex->mark = NOT_QUEUED;
  newVertex->affected = NOT_AFFECTED; 
  newVertex->deleted = 1;
  newVertex->descendant = NULL;
  newVertex->vertex = n->vertex;
  for(i=0;i<MAX_DEGREE; i++)
    {
      newVertex->scars[i].backscar = NULL;      
    }
 
  return newVertex; 
}

/* return whether the vertex is a leaf */
int isLeaf(node* v) 
{
  return (v->degree==1);
}


/*
  Initialize a vertex
 */
void initVertex(node* v, unary_data data, int id, void* tr)
{
  tree_t* tree = (tree_t*) tr;
  int i;
  
  //deprintf("cluster is %p\n",vertexDataClusters+id);
  deprintf("v is %p\n",v);
  v->data    =  new (allocBlock(tree->UClusters)) unary_cluster(data);
  v->vertex  =  new (allocBlock(tree->vertexList)) vertex_t(v);
  
  //memcpy((void*) (v->vertex), (void*) &dummyVertex, sizeof(vertex_t));
  deprintf("v->data is %p \n",v->data);
  
  //memcpy((void*) (v->data), (void*) &dummy, sizeof(unary_cluster));
  v->data->data = data;
  v->data->endpoints = 1;
  v->data->endp1 = id;

  v->nId     = id; 
  v->mark    = NOT_QUEUED;
  v->affected = NOT_AFFECTED; 
  v->height  = 0;
  v->id      = rand();
  v->degree  = 0;
  v->deleted = 0;
  v->left    = -1;
  v->right   = -1;

  
  //v->descendant = NULL;
  for(i=0;i<MAX_DEGREE;i++)
    {
      deprintf("i is %d\n",i);   
      v->scars[i].backscar = NULL;
      v->scars[i].cl       = NULL;
    }
}

// The canonical representation for a vertex includes.
void makeCanonical (node* v) 
{
  int degree,i; 
  int neigh[MAX_DEGREE]; 

  // only first two are used. 
  neigh[0] = -1; 
  neigh[1] = -1; 

  degree = 0; 
  for (i = 0; i < MAX_DEGREE; ++i) 
    if(v->scars[i].backscar)
      neigh[degree++] = i; 

  v->degree = degree; 
  v->left = neigh[0]; 
  v->right = neigh[1]; 

  deprintf("Made degree for %d %d\n",v->nId, degree);
}

/* find an empty scar on the vertex v */
int findEmptyScar (node* v) 
{
  int i; 
  for (i = 0; i < MAX_DEGREE; ++i) 
    if(v->scars[i].cl == NULL)
      return i; 

  return MAX_DEGREE;
}

/* find the edge cluster for the edge (u,w) */
bin_cluster* findEdgeCluster (node* v, node* w) 
{
  int i; 
  for (i = 0; i < MAX_DEGREE; ++i) 
    if(GET_NEIGHBOR(v->scars[i].backscar) == w)
      return GET_BC(v->scars[i].cl); 
  return NULL;
}

/* verify the vertex v */
int verifyVertex (node* v) 
{ 
  int i, degree=0; 
  node* u; 
  for (i = 0; i < MAX_DEGREE; ++i) {
    if ((u = GET_NEIGHBOR(v->scars[i].backscar))) {
      ++degree;
      if (GET_NEIGHBOR(v->scars[i].backscar->backscar) != v) {
        printf ("verifyVertex: Error, inconsistent edge=(v=%d, u=%d)",v->nId,u->nId );
	drun(CRASH);
        return 0; 
      } 
    }
  }//for
  if (degree != v->degree) {
    printf ("verifyVertex: Error, inconsistent degree for vertex v=%d\n",v->nId);
    printf ("verifyVertex: degree=%d, i computed %d, address v=%p\n",v->degree, degree, v);
    return 0; 
  }
  if(v->left != -1)
    if(!v->scars[v->left].backscar) {
       deprintf ("verifyVertex: Error, inconsistent left index for vertex v=%d\n",v->nId);
       return 0; 
      
    }
  if(v->right != -1)
    if(!v->scars[v->right].backscar) {
       printf ("verifyVertex: Error, inconsistent right index for vertex v=%d",v->nId);
    return 0; 
    }
  return 1; 
}

int isRoot(cluster* cl)
{
  if(cl->parent) return 0;
  return 1;
}

void freeVertexCluster(cluster* cl,tree_t* tree)
{
  deprintf("freeCluster %p\n",cl);
  if(cl->endpoints == 2) {
    deprintf("free binary\n");
    bcs--;
    freeBlock(tree->BClusters,(char*) cl);
  }
  else {
    deprintf("free non-bnary\n");
    ucs--;
    freeBlock(tree->UClusters,(char*) cl);
  }
}

void setupBinVertexCluster (node* thisNode, int event,tree_t* tree, clusterList* rootList) 
{
  int i, j=0,k=0;
  cluster *cll;
  bin_cluster *cl;
 
  cluster* oldCluster = thisNode->vertex->cl;
  cl = new (allocBlock(tree->BClusters)) bin_cluster;
  drun(bcs++);

 
  if(oldCluster != NULL) {
    if(PUSHDOWN) {   
      oldCluster->affected = DELETED;
      if(isRoot(oldCluster))
	insertCluster(oldCluster,rootList);
    }
    else {
      freeVertexCluster(oldCluster,tree);
    }
  } 
  
  thisNode->vertex->cl = cl;
 
  deprintf("Setting up binary cluster %p\n",cl);
  cl->id = thisNode->nId;
  cl->height = thisNode->height;
  
  // Make children cluster points to thisNode
  for(i = 0; i < MAX_DEGREE; i++) {
    cll = (cluster *) GET_CL(thisNode->scars[i].cl);
    
    if(cll) {   
      
      cll->parent = cl;
      if(i == thisNode->left || i == thisNode->right) {
	cl->binaryCl[j] = (bin_cluster *) thisNode->scars[i].cl;
	j++;
      }
      else {
	cl->unaryCl[k] = (unary_cluster *) thisNode->scars[i].cl;
	k++;
      }
    }
  
  }
  for(k=k;k<MAX_DEGREE-2;k++) {
    cl->unaryCl[k] = NULL;
  }
  
  // Make node data cluster points to this node
  cll = thisNode->data;
  
  
  cll->parent = cl;
  cl->child = cll;
  
}

void setupUnaryVertexCluster (node* thisNode, int event, tree_t* tree, clusterList* rootList) 
{
  int i, j=0,k=0;
  unary_cluster *cll;
  unary_cluster *cl = new (allocBlock(tree->UClusters)) unary_cluster;

  drun(ucs++);

  cluster* oldCluster = thisNode->vertex->cl;
  if(oldCluster != NULL) 
    {     
      if(PUSHDOWN) {
	oldCluster->affected = DELETED;
	if(isRoot(oldCluster)) {	 
	  insertCluster(oldCluster,rootList);
	}
      }
      else {
	freeVertexCluster(oldCluster,tree);     
      }
    }
    
  thisNode->vertex->cl = cl;
  
  deprintf("Setting up cluster for node %d\n",thisNode->nId);
  deprintf("Cluster %p\n",cl);

  cl->id = thisNode->nId;
  cl->height = thisNode->height;
 
  // Make children cluster points to thisNode
  for(i = 0; i < MAX_DEGREE; i++) {
    cll = (unary_cluster *) GET_CL(thisNode->scars[i].cl);
    deprintf("i is %d \n",i);
    if(cll) {
      deprintf("cll exists\n");      
      cll->parent = cl;
      if(i == thisNode->left || i == thisNode->right) {
	cl->binaryCl[j] = (bin_cluster *) thisNode->scars[i].cl;
	deprintf("Binary cluster %d is %p\n", j, cl->binaryCl[j]);
	j++;
      }
      else {
	cl->unaryCl[k] = (unary_cluster *) thisNode->scars[i].cl;
	deprintf("Unary cluster %d is %p\n", k, cl->unaryCl[k]);
	k++;
      }
    }
     
  }
  for(k=k;k<MAX_DEGREE-1;k++) {
    cl->unaryCl[k] = NULL;
  }

  // Make node data cluster points to this node
  cll = thisNode->data;
 
  cll->parent = cl;
  cl->child = cll;
 
  deprintf("wrote %p to %p\n",cll,&(cl->child));
  deprintf("Cl->id is %d\n",cl->id); 
}

void setupNullaryVertexCluster (node* thisNode, int event, tree_t* tree, clusterList* rootList) 
{
  int i, k=0;
  unary_cluster *cll;
  
  final_cluster *cl = new (allocBlock(tree->UClusters)) final_cluster;
 
  drun(ucs++);

  cluster* oldCluster = thisNode->vertex->cl;
  if(oldCluster != NULL) 
    {
     
      if(PUSHDOWN) {
	 oldCluster->affected = DELETED;
	if(isRoot(oldCluster))
	  insertCluster(oldCluster,rootList);
      }
      else {
	freeVertexCluster(oldCluster,tree);
      }
    }
    
  thisNode->vertex->cl = cl;
  

  deprintf("Setting up cluster for node %d\n",thisNode->nId);
  deprintf("Cluster %p\n",cl);

  cl->id = thisNode->nId;
  cl->height = thisNode->height;


  // Make children cluster points to thisNode
  for(i = 0; i < MAX_DEGREE; i++) {
    cll = (unary_cluster *) GET_CL(thisNode->scars[i].cl);
    deprintf("i is %d \n",i);
    if(cll) {
      deprintf("cll exists\n");      
      cll->parent = cl;
      
      cl->unaryCl[k] = (unary_cluster *) thisNode->scars[i].cl;
      deprintf("Unary cluster %d is %p\n", k, cl->unaryCl[k]);
      k++;
      
    }
  }
  for(k=k;k<MAX_DEGREE;k++) {
    cl->unaryCl[k] = NULL;
  }

  // Make node data cluster points to this node
  cll = thisNode->data;
  cll->parent = cl;
  cl->child = cll;


  deprintf("Cl->id is %d\n",cl->id);
}


/* setup the cluster associated with the vertex */
void setupVertexCluster (node* thisNode, int event, void* tr, clusterList* rootList) 
{
  tree_t* tree = (tree_t*) tr;
 
  switch (event) {
    case END_EVENT: { 
      setupNullaryVertexCluster(thisNode,event,tree,rootList);
      break;
    }

    case RAKE_EVENT: { 
      setupUnaryVertexCluster(thisNode,event,tree,rootList);
      break;
    }

    case COMPRESS_EVENT: { 
      setupBinVertexCluster (thisNode,event,tree,rootList);
      break;
    }
  }


}
