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


/******************************************************
 * Tree.c
 *
 * Jorge L. Vittes and Umut Acar
 *
 * This code is for loading and editing tree data
 * marked nodes
 * Algorithm by Umut Acar, Guy Blelloch, and Jorge Vittes
*******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <assert.h> 
#include "FinalCluster.h"
#include "UnaryCluster.h"
#include "BinCluster.h"
#include "AMath.h"
#include "Tree.h"
extern char* HERE; 
int ucs=0;
int bcs=0;

/* Verify the entire tree */
int verifyTree (tree_t* tree) 
{  
  int i;
  deprintf("Tree->n is %d\n",tree->n);

  for (i = 0; i < tree->n; ++i) {
    deprintf("Verifying first vertex %d\n",i+1);
    assert (verifyVertex(tree->vertexArray+i+1)); 
   
  }
  return 1;
}

/* verify the tree, and all the levels below it for the contraction */
int verifyContractionTree (tree_t* tree) 
{  
  int i,depth; 
  node* v;
  for (i = 0; i < tree->n; ++i) {
    v = tree->vertexArray+i+1;
    while(!(v->deleted)) {
      if(!verifyVertex (v)) {
	deprintf("Depth is %d\n",depth);
	return 0;
      }
      v = v->descendant;
      depth++;
    }
     
  }
  return 1;
}


/*
  Add an edge from thisNode to the otherNode with
  data data
 */
int addEdge(node* v1, node* v2, bin_data data, tree_t* tree)
{
  scar* scar1;
  scar* scar2;

  deprintf("Adding edge now\n");
  assert (verifyTree(tree)); 

  int degree1 = v1->degree;
  int degree2 = v2->degree; 
  if ( degree1 == MAX_DEGREE || degree2 == MAX_DEGREE )    {
    deprintf("This node has maximum degree \n");
    return 0;
  }

  int i1 = findEmptyScar(v1); 
  int i2 = findEmptyScar(v2); 
  scar1 = v1->scars + i1;

  scar1->backscar = v2->scars+i2;
 
  scar1->cl = new (allocBlock(tree->BClusters)) bin_cluster(data); // must init all to null except for data
  bcs++;
  bin_cluster* cl1 = (bin_cluster*) scar1->cl;
  cl1->endp1 = v1->nId;
  cl1->endp2 = v2->nId;
  cl1->endpoints =2;

  // deprintf("data is ");data.print();

  scar2 = v2 ->scars + i2;
  scar2->backscar = scar1;
  
  scar2->cl = (cluster *)((intptr_t) (scar1->cl) + 1);

  deprintf("Added edge from %d to %d ",v1->nId, v2->nId);
  deprintf("EDGE Added on scars %d, and %d \n", i1,i2);

  makeCanonical (v1); 
  makeCanonical (v2); 

  assert (verifyTree(tree)); 
  return 1; 
}

/* Remove an edge */
int deleteEdge(node* n1, node* n2, tree_t* tree)
{
  int s1,s2;
 

  assert (verifyTree(tree)); 
  for (s1 = 0; s1 < MAX_DEGREE; ++s1) 
    if (GET_NEIGHBOR(n1->scars[s1].backscar) == n2) break; 

  for (s2 = 0; s2 < MAX_DEGREE; ++s2) 
    if (GET_NEIGHBOR(n2->scars[s2].backscar) == n1) break; 

  if (s1 == MAX_DEGREE || s2 == MAX_DEGREE) {
    deprintf("Can't find the edge\n"); 
    return 0;
  }

  for (int ss1 = 0; ss1 < MAX_DEGREE; ++ss1)
  {
      if (ss1 == s1) continue;
      node *ngb = GET_NEIGHBOR(n1->scars[ss1].backscar);
      if (ngb) ngb->affected = IS_AFFECTED;
  }
  for (int ss2 = 0; ss2 < MAX_DEGREE; ++ss2)
  {
      if (ss2 == s2) continue;
      node *ngb = GET_NEIGHBOR(n2->scars[ss2].backscar);
      if (ngb) ngb->affected = IS_AFFECTED;
  }

  cluster* cl = GET_CL(n1->scars[s1].cl);
  cl->affected = DELETED;
  if(!PUSHDOWN)
    freeBlock(tree->BClusters,(char*) cl);

  // update n1
  n1->degree--;
  n1->scars[s1].backscar = NULL;
  n1->scars[s1].cl = NULL;

  n2->degree--; 
  n2->scars[s2].backscar = NULL;
  n2->scars[s2].cl = NULL;

  deprintf("Removed on scars %d and %d \n",s1,s2);
  deprintf("Removed %d, and %d edge \n",n1->nId, n2->nId);

  makeCanonical (n1); 
  makeCanonical (n2); 

  assert (verifyTree(tree)); 

  return 1;
}


// Create a new tree for nVertices. 
tree_t *initTree  (int nVertices)
{
  // Alloc a tree
  tree_t* tree = (tree_t*) malloc (sizeof (tree_t));
  assert(tree);

  tree->n = nVertices;
  
  // Get a freeList for binary clusters. 
  tree->BClusters = initPreAllocatedFreeList (nVertices+1,sizeof(bin_cluster));
  
  
  // Get a freeList for binary clusters. 
  tree->UClusters = initPreAllocatedFreeList (nVertices+1,sizeof(unary_cluster));

  // Allocate free list for vertices. 
  tree->vertexList = initPreAllocatedFreeList (nVertices+1, sizeof (LLNode)); 
  deprintf("VertexList %p\n",tree->vertexList);
   
  tree->nodeList = initPreAllocatedFreeList (nVertices+1, sizeof (node));

  // Dummy first vertex for getting the pointer to the vertex array. 
  tree->vertexArray = (node*) (allocBlock (tree->nodeList));
  
  tree->clusterList = (cluster**) (calloc(nVertices+1, sizeof(cluster*)));

  deprintf("Sizeof cluster and the others are u=%d, b=%d, and c=%d\n",sizeof(unary_cluster),sizeof(bin_cluster),sizeof(cluster));
  return tree; 
}

void destructTree (tree_t* tree) 
{
  free(tree->clusterList); 
  
  destructFreeList (tree->BClusters);
  destructFreeList (tree->UClusters); 
  destructFreeList (tree->vertexList); 
  destructFreeList (tree->nodeList);

  free(tree); 
}


/* This function loads the tree 
   Check input.dat for input information
*/
tree_t* loadtree(char* treeFile)
{
  FILE *file;
  char token[100];
  int i,id,v1,v2;
  
  int nVertices, nEdges;
  node* v;

  //  debug = 1; 
  // Open data file
  deprintf("here \n");
  file  = fopen(treeFile,"r");

  if (!file) { 
    printf ("Cannot open the tree file %s\n", treeFile); 
    exit (-1); 
  }

  deprintf("opened file \n");
  deprintf("eh? \n");

  while(1) {
    fscanf(file,"%s\n", token);
    if(strcasecmp(token,"-START-") ==0)
      break;
    //deprintf("%s\n",token);
  }
  deprintf("now here \n");

  // Read in the tree
  fscanf(file,"%s\n",token);
  deprintf("%s\n",token);
  fscanf(file,"%i",&nVertices);
  deprintf("v is %d \n",nVertices);

  tree_t *tree = initTree (nVertices); 
  
  // Read in vertices
  for (i = 1; i <= nVertices; ++i) {
    unary_data udat;
    deprintf("i is %d \n",i);
    v = (node*) allocBlock (tree->nodeList);
    deprintf("allocated node \n");
    fscanf(file,"%i",&id);  
    // Read vertex data.
    udat  = readVertexData (file); 
    initVertex (v,udat,i,tree); 
  }
     
  // Read in the edges. 
  //  fscanf(file,"%s\n",s);
  fscanf(file,"%s\n",token);
  fscanf(file,"%i",&nEdges);
  deprintf("EDGES %d \n",nEdges);

  
  node* vertexArray = tree->vertexArray;  
  for (i=0 ; i < nEdges; i++) {
    //deprintf("i = %d \n",i);
    fscanf(file,"%i %i",&v1,&v2);
    bin_data dat = readEdgeData (file,v1,v2);
    deprintf("reading edge %d from %d to %d\n",i,v1,v2);
    addEdge (vertexArray+v1,vertexArray+v2,dat,tree); 
  }


  assert (verifyTree(tree)); 

  return tree; 
}

/* This function loads the tree 
   Check input.dat for input information
*/
tree_t* loadtreeAndEdges(char* treeFile)
{
  FILE *file;
  char token[100];
  int i,id,v1,v2;
  
  int nVertices, nEdges;
  node* v;

  //  debug = 1; 
  // Open data file
  deprintf("here \n");
  file  = fopen(treeFile,"r");

  if (!file) { 
    printf ("Cannot open the tree file %s\n", treeFile); 
    exit (-1); 
  }

  deprintf("opened file \n");
  deprintf("eh? \n");

  while(1) {
    fscanf(file,"%s\n", token);
    if(strcasecmp(token,"-START-") ==0)
      break;
    //deprintf("%s\n",token);
  }
  deprintf("now here \n");

  // Read in the tree
  fscanf(file,"%s\n",token);
  deprintf("%s\n",token);
  fscanf(file,"%i",&nVertices);
  deprintf("v is %d \n",nVertices);

  tree_t *tree = initTree (nVertices); 
  
  // Read in vertices
  for (i = 1; i <= nVertices; ++i) {
    unary_data udat;
    deprintf("i is %d \n",i);
    v = (node*) allocBlock (tree->nodeList);
    deprintf("allocated node \n");
    fscanf(file,"%i",&id);  
    // Read vertex data.
    udat  = readVertexData (file); 
    initVertex (v,udat,i,tree); 
  }
     
  // Read in the edges. 
 
  fscanf(file,"%s\n",token);
  fscanf(file,"%i",&nEdges);
  deprintf("EDGES %d \n",nEdges);

  tree->edgeArray = (edge_t*) malloc(nEdges*(sizeof(edge_t)));
  
  node* vertexArray = tree->vertexArray;  
  for (i=0 ; i < nEdges; i++) {
   
    fscanf(file,"%i %i",&v1,&v2);
    bin_data dat = readEdgeData (file,v1,v2);
    deprintf("reading edge %d from %d to %d\n",i,v1,v2);
    addEdge (vertexArray+v1,vertexArray+v2,dat,tree); 
    tree->edgeArray[i].eps[0] = v1;
    tree->edgeArray[i].eps[1] = v2;
    tree->edgeArray[i].weight = 0.0;
  }

 
  assert (verifyTree(tree)); 

  return tree; 
}

tree_t* makeEmptyTree(int n)
{
  int i;
  node* v;
 
  tree_t *tree = initTree (n); 

  for (i = 1; i <= n; ++i) {
    unary_data udat;
    deprintf("i is %d \n",i);
    v = (node*) allocBlock (tree->nodeList);
  
    initVertex (v,udat,i,tree); 
  }
      
  assert (verifyTree(tree)); 
  //  debug = 0; 
  return tree; 
}


/* Load the tree using the passed function to read the vertex data off the file */
tree_t* loadtree_param_vertex(char* treeFile, 
			      unary_data (*readVertexData)(FILE* file))
{
  FILE *file;
  char token[100];
  int i, id,v1,v2;
  int nVertices, nEdges;
  node* v;

  //  debug = 1; 
  // Open data file
  deprintf("here \n");
  file  = fopen(treeFile,"r");

  if (!file) { 
    printf ("Cannot open the tree file %s\n", treeFile); 
    exit (-1); 
  }

  deprintf("opened file \n");
  deprintf("eh? \n");

  while(1) {
    fscanf(file,"%s\n", token);
    if(strcasecmp(token,"-START-") ==0)
      break;
    //deprintf("%s\n",token);
  }
  deprintf("now here \n");

  // Read in the tree
  fscanf(file,"%s\n",token);
  deprintf("%s\n",token);
  fscanf(file,"%i",&nVertices);
  deprintf("v is %d \n",nVertices);

  tree_t *tree = initTree (nVertices); 
  
  // Read in vertices
  for (i = 1; i <= nVertices; ++i) {
    deprintf("V: i is %d \n",i);
    v = (node*) allocBlock (tree->nodeList);
    deprintf("allocated node \n");
    fscanf(file,"%i",&id);  
    // Read vertex data.
    unary_data udat  = (*readVertexData) (file); 
    initVertex (v,udat,i,tree); 
  }
     
  // Read in the edges. 

  fscanf(file,"%s\n",token);
  fscanf(file,"%i",&nEdges);
  deprintf("EDGES %d \n",nEdges);

  
  node* vertexArray = tree->vertexArray;  
  for (i=0 ; i < nEdges; i++) {
    //deprintf("i = %d \n",i);
    fscanf(file,"%i %i",&v1,&v2);
    bin_data bdat = readEdgeData (file,v1,v2);
    addEdge (vertexArray+v1,vertexArray+v2,bdat,tree); 
  }

  assert (verifyTree(tree));
 
  return tree; 
}



/* copy the tree orTree into the tree coTree */
void copyTree(tree_t* orTree, tree_t* coTree)
{
  int nVertices = orTree->n;
  int i,j;
  bin_data dat;
  node *v, *v2;
  node* list;
  node* olst;
  node* vertexArray;
  bin_cluster* bcl;
  coTree->n = nVertices;
  
  // Make sure copy tree is good. 
  assert(coTree->UClusters);
  assert(coTree->BClusters);
  assert(coTree->clusterList);
  assert(coTree->nodeList);
  assert(coTree->vertexList);
  assert(coTree->vertexArray);



  vertexArray = coTree->vertexArray;
  olst = orTree->vertexArray;
  list = coTree->vertexArray;

    
  // Read in vertices
  for (i = 1; i <= nVertices; ++i) {
    v = (node*) allocBlock (coTree->nodeList);
    unary_data udat  = (olst+i)->data->data; 
    deprintf("Vertex %d has data ",i);
    //printData(&dat);
    initVertex (v,udat,i,coTree); 
  }
     
  // Read in the edges. 
  for (i=1 ; i <= nVertices; i++) {
    //deprintf("i = %d \n",i);
    v = (olst+i);
    for(j = 0; j < MAX_DEGREE; j++)
      if(v->scars[j].backscar) {
	  v2 = GET_NEIGHBOR(v->scars[j].backscar); 
	  bcl = GET_BC(v->scars[j].cl);
	  dat = bcl->data;
	  if(v->nId < v2->nId)
	    addEdge (vertexArray + v->nId, vertexArray+v2->nId, dat, coTree); 
	}
  }
  
}


/* Print the entire tree */
void printTree(tree_t* tree)
{
  int i;
  int j;
  node *nd, *v;
  for(i=1;i<=tree->n;i++)
    {
      for(j=0;j<MAX_DEGREE;j++)
	{
	  nd = tree->vertexArray+i;
	  if(nd->scars[j].cl){
	    
	    drun(v = GET_NEIGHBOR(nd->scars[j].backscar));
	    deprintf("neighbor of %d is %d \n", i, v->nId);
	  }
	}
    }
}
