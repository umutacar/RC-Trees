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


//////////////////////////////////////////////////////////////
// Application.c
//
// Jorge L. Vittes
//
// This code is for finding the median of a tree
// Algorithm by Guy Blelloch, and Jorge Vittes
//////////////////////////////////////////////////////////////

#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include "Data.h"
#include "UnaryCluster.h"
#include "BinCluster.h"
#include "FinalCluster.h"
#include "Vertex.h"

/////////////////////////////////////////////////////////////////////////////
// Find the maxum child, and the sum of all others put together
/////////////////////////////////////////////////////////////////////////////
void findMaxAndSum(cluster* cl, cluster** child, double* sum, double* max1,int ep1,double w1,int ep2,double w2)
{
  int i;
  double effMax;
  deprintf("findMaxAndSum\n");
  
  *max1 = *sum = 0.0;
  //purify the pointer
  cl = GET_CL(cl);
  /* For each of the binary clusters compensate for the outside clusters
     To the current cluster cl */
  cluster** bclusters = cl->getBinaryClusters();
  for(i = 0; i < cl->endpoints ; ++i) {
    bin_cluster* cbcl = GET_BC(bclusters[i]);
    if(cbcl->isEndPoint(ep1))
      effMax = w1 + cbcl->data.totalweight;
    else
      effMax = w2 + cbcl->data.totalweight;
    deprintf("i is %d, effMax is %lf, max1 is %lf, mindex is %d \n", i, effMax,*max1, 0);
    if(effMax > *max1) {*child = cbcl; *max1 = effMax;}
    (*sum)+= effMax;
  }
  cluster** uclusters = cl->getUnaryClusters();
  for(i=0; i < MAX_DEGREE-cl->endpoints; i++) {
    unary_cluster* ucl = (unary_cluster*) uclusters[i];
    if(ucl) {
      effMax = ucl->data.totalweight;
      deprintf("i is %d, effMax is %lf, max1 is %lf, mindex is %d \n", i, effMax,*max1, 0);
      if(effMax > *max1) {*child = ucl; *max1 = effMax;}
      (*sum)+= effMax;
    }
  }
    
  (*sum)+= ((unary_cluster*) cl->child)->data.totalweight;
}


//////////////////////////////////////////////////////////////////////
// Simply traverse down the tree until one gets to the median itself
//
//////////////////////////////////////////////////////////////////////
int traverse(cluster* curcl, int ep1, double w1, int ep2, double w2)
{
  double max1,sum;
  cluster* child=NULL;

  // cl = curcl->child;
  deprintf("Traverse: %d \n",curcl->id);
  findMaxAndSum(curcl, &child, &sum, &max1,ep1,w1,ep2,w2);
  deprintf("Return from find Max and sum\n");
  //child = GET_CL(child);
  switch(curcl->endpoints) {
  case 0:
    deprintf("End Event\n");
    if(child)
      return traverse(child,curcl->id,sum-max1, -1, -1.0);
    else
      return curcl->id;
    break;
  case 1:
    deprintf("Rake event\n");
    if(child->endpoints == 2) {
      if(child->child)
        return traverse(child,curcl->id,sum-max1,ep1,w1);
      else {
	deprintf("ep1 is %d \n",ep1);
	deprintf("sum is %lf \n",sum);
	deprintf("w1 is %lf \n",w1);
	if(w1 > (sum - max1))
	  return ep1;
	else
	  return curcl->id;
      }
    }
    else
      return traverse(child,curcl->id,sum-max1,-1,-1.0);
    break;
  case 2:
    deprintf("Compress Event\n");
    if(child->endpoints == 2)
      {
	if(child->child) { 
	  if(child->isEndPoint(ep1))
	    return traverse(child,ep1,w1,curcl->id,sum-max1);
	  else
	    return traverse(child,ep2,w2,curcl->id,sum-max1);
	}
	else
	  if(child->isEndPoint(ep1))
	    if(w1 > sum-max1)
	      return ep1;
	    else
	      return curcl->id;
	  else
	    if(w2 > sum-max1)
	      return ep2;
	    else
	      return curcl->id;
      }
    else
      {
	return traverse(child,curcl->id,sum-max1,-1,-1.0);
      }
    break;
  default: assert(0);
  }
}

//////////////////////////////////////////////////////////////////
// Find the median of the tree
//////////////////////////////////////////////////////////////////
int median(cluster* root)
{
  int r = traverse(root, -1, -1.0, -1, -1.0);

  return r; 
}

cluster* getRoot(node* v)
{
  cluster* cl = v->data;
  while(cl->parent)
    {
      cl = cl->parent;
    }
  return cl;
}
