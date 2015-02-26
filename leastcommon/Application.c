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

/////////////////////////////////////////////////////////////
// Application.c
//
// Jorge L. Vittes
//
// This code is for finding the least common ancestor of 3 
// vertices in a tree
// Algorithm by Guy Blelloch, and Jorge Vittes
///////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <assert.h>
#include "Data.h"
#include "UnaryCluster.h"
#include "BinCluster.h"
#include "FinalCluster.h"
#include "Vertex.h"

/////////////////////////////////////////////
// returns the "ith" cluster of cl
/////////////////////////////////////////////
cluster* getCluster(cluster* cl, int i) 
{
  cluster** clusters = cl->getClusters();
  cluster* retcl = clusters[i];
  free(clusters);
  return retcl;
}

////////////////////////////////////////////////
// return the count on cl
///////////////////////////////////////////////
int getCount(cluster* cl)
{
  switch(cl->endpoints) {
  case 0: {
    final_cluster* fcl = (final_cluster*) cl;
    return fcl->data.count;
  }break;
  case 1: {
     unary_cluster* ucl = (unary_cluster*) cl;
     return ucl->data.count;
  }break;
  case 2: {
     bin_cluster* bcl = (bin_cluster*) cl;
     return bcl->data.count;
  }break;
  
  }
  printf("endpoints set incorrectly\n");
  return 0;
}

/////////////////////////////////////////
//
/////////////////////////////////////////
int traverse(cluster* cl, int nd)
{
  int i;
  int trav=0;
 
  int count;
  int ccount;


  deprintf("traverse: %d\n",cl->id);
  count = getCount(cl);

  switch(count){
  case 3: //There are there points here
    deprintf("3 marks\n");
    for (i = 0; i < MAX_DEGREE; ++i) {
      cluster* child = (cluster *) GET_CL(getCluster(cl,i));
      if(child) {
	ccount = getCount(child);
	if(ccount ==2 || ccount== 3)
	  { trav=1; return traverse(child,cl->id);}
      }
    }
    if(!trav) {
      deprintf("3 with no 2 cluster!\n");
      return cl->id;
    }
    break;
  case 2:
    deprintf("2 marks \n");
     for (i = 0; i < MAX_DEGREE; ++i) {
       cluster* child = (cluster *) GET_CL(getCluster(cl,i));      
       if (child) {
	 ccount = getCount(child);
	 if(ccount ==2)
	   { trav=1; return traverse(child,cl->id);}
	 if(ccount == 1) 
	   if(child->endpoints == 2) {      
	     if(child->isEndPoint(nd))
	       { trav=1; return traverse(child,cl->id);}
	   }
       }
     }
     
     deprintf("trav is %d\n",trav);
     if(!trav){
       return cl->id;
     }
     break;
  case 1:
    deprintf("1 mark \n");
    for (i = 0; i < MAX_DEGREE; ++i) {
      cluster* child = (cluster *) GET_CL(getCluster(cl,i));
      if (child) {
	ccount = getCount(child);
	if(ccount !=0)
	  if(child->endpoints == 2)
	    { trav=1; return traverse(child,cl->id);}
      }
    }
    if(!trav)
      return cl->id;
    break;
  default: printf("cl->data.count is %d, cl is %d\n", getCount(cl), cl->id); assert(0);
  }
  printf("Reach unreachable zone\n");
  exit(-9);
  return(0);
}

/////////////////////////////////////////////
// Increment the count by one
////////////////////////////////////////////
void incrementCount(cluster* cl)
{
  switch(cl->endpoints) {
  case 0:{
    final_cluster* fcl = (final_cluster*) cl;
    fcl->data.count++;
   
  }break;
  case 1:{
    unary_cluster* ucl = (unary_cluster*) cl;
    ucl->data.count++;
  }break;
  case 2:{
    bin_cluster* bcl   = (bin_cluster*) cl;
    bcl->data.count++;
  }break;
  }
  
}

////////////////////////////////////////////////
// reset the count
////////////////////////////////////////////////
void resetCount(cluster* cl)
{
  switch(cl->endpoints) {
  case 0:{
    final_cluster* fcl = (final_cluster*) cl;
    fcl->data.count=0;
  }break;
  case 1:{
    unary_cluster* ucl = (unary_cluster*) cl;
    ucl->data.count=0;
  }break;
  case 2: {
    bin_cluster* bcl   = (bin_cluster*) cl;
    bcl->data.count=0;
  }break;
  }
  
}

/////////////////////////////////////////////////////////
// find the least common ancestor of r, a and b
// by marking the path up to the root of the support
// tree, and then traversing down the tree from there
////////////////////////////////////////////////////////
int LeastCommonAncestor(node* r, node* a, node* b)
{
  int ret;
  cluster *cl, *clp, *root;

  deprintf("r is %d, a is %d, and b is %d\n",r->nId, a->nId, b->nId);
  cl = r->data;
  while(cl)
    {
      incrementCount(cl);
      cl = cl->parent;
    }
  cl = a->data;
  while(cl)
    {
      incrementCount(cl);
      cl = cl->parent;      
    }
  clp = b->data;
  while(clp)
    {
      cl = clp;
      incrementCount(cl);
      clp = clp->parent;
    }
  root = cl;
  ret = traverse(root,root->id);
  
  cl = r->data;
  while(cl)
    {
      resetCount(cl);
      cl = cl->parent;
    }
  cl = a->data;
  while(cl)
    {
      resetCount(cl);
      cl = cl->parent;      
    }
  cl = b->data;
  while(cl)
    {
      
      resetCount(cl);
      cl = cl->parent;
    }
  
  return ret;
}
