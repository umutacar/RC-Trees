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


///////////////////////////////////////////////////////////////
// Application.c
//
// Jorge L. Vittes
//
// This code for finding the center of a tree
// Algorithm by Guy Blelloch, and Jorge Vittes
///////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <assert.h>
#include "Data.h"
#include "AMath.h"
#include "FinalCluster.h"
#include "UnaryCluster.h"
#include "BinCluster.h"
#include "Vertex.h"
//#define MILLION 1000000


void changeNodeWeight(cluster* cl, int w) 
{
  printf ("Incorrect operation, this application has no node weights\n"); 
  exit (-1); 
}

/* Find the clusters with the two largest distances from the distinguished endpints, adjusting for being in a subcluster
 */
void find2max(cluster* curcl, cluster** child, double* max2,int ep1,double w1,int ep2,double w2)
{
  int i;
  double max1;
  double effMax; 
  bin_cluster** binaryCl;
  unary_cluster** unaryCl;
 
  max1 = *max2 = 0;
  binaryCl = (bin_cluster**) curcl->getBinaryClusters();
  unaryCl = (unary_cluster**) curcl->getUnaryClusters();
  
  for (i = 0; i < curcl->endpoints; ++i) {
    bin_cluster* cl = binaryCl[i];
    if(cl) {
      bin_data* d = getBinData(cl);
      double mpath = getThisData(d);
      double len = getLen(d);
      
      deprintf("Binary cluster \n");
      if((GET_CL(cl))->isEndPoint(ep1))
	effMax = dmax(mpath,w1+len);
      else 
	effMax = dmax(mpath,w2+len);
      deprintf("mpath is %lf, len is %lf \n",mpath, len);
      deprintf("w1 is %lf, w2 is %lf, ep1 is %d, ep2 is %d\n",w1,w2,ep1,ep2);
      deprintf("i is %d, effMax is %lf, max1 is %lf\n", i, effMax,max1);
      if(effMax > max1) {*max2 = max1; *child = cl; max1 = effMax;}
      else if(effMax > *max2) {*max2 = effMax;}
    }
  }
 

  for(i=0;i<MAX_DEGREE-curcl->endpoints;i++) {
    unary_cluster* cl = unaryCl[i];
    if(cl) {
      double mpath = cl->data.mpath;
      effMax = mpath;
      deprintf("i is %d, effMax is %lf, max1 is %lf\n", i, effMax,max1);
      if(effMax > max1) {*max2 = max1; *child = cl; max1 = effMax;}
      else if(effMax > *max2) {*max2 = effMax;}
    }
  }
  
    
}

/* traverse into the child with the largest "normalized" distance to 
another point going through that cluster
*/

int traverse(cluster* curcl, int ep1, double w1, int ep2, double w2)
{
  double max2;
  //data_t* dat;
  cluster* child=NULL;
  cluster* cll = NULL;
  cluster* cl2 = GET_CL(curcl);

  //  printf ("!");
  deprintf("Traverse: %d \n",cl2->id);
  find2max(curcl, &child, &max2,ep1,w1,ep2,w2);
  child = GET_CL(child);
  cll = GET_CL(child);
  switch(curcl->endpoints) {
  case 0:
    deprintf("End Event\n");
    if(child) {
      deprintf("max2 is %lf\n",max2);//dat= thisData(child);
      //deprintf("max is %lf\n",dat->mpath);
      cll= GET_CL(child);
      if(cll->child)
	return traverse(child,curcl->id,max2, -1, -1.0);
      else
	assert(0);
    }
    else
      return curcl->id;
    break;
  case 1:
    deprintf("Rake event\n");
    if(cll->endpoints == 2) {     
      if(cll->child)
	return traverse(child,curcl->id,max2,ep1,w1);
      else {
	deprintf("ep1 is %d \n",ep1);
	deprintf("max2 is %lf \n",max2);
	deprintf("w1 is %lf \n",w1);

	if(w1 > max2)
	  return ep1;
	else
	  return curcl->id;
      }
    }
    else
      if(child->child)
	return traverse(child,curcl->id,max2,-1,-1.0);
      else {
	deprintf("child->child is %p\n",child->child);
	deprintf("child is %p\n",child);
	drun(CRASH);//assert(0);
      }
  case 2:
    deprintf("Compress Event\n");

    if(child->endpoints == 2)
      {
	deprintf("Binary Child\n");
	if(child->child) { 
	  deprintf("with children\n");drun(fflush(stdout));
	  if(child->isEndPoint(ep1)) {
	    deprintf("Ep1 is an endpoint \n");
	    return traverse(child,ep1,w1,curcl->id,max2);
	  }
	  else {
	    deprintf("ep2 is an endpoint\n");
	    return traverse(child,ep2,w2,curcl->id,max2);
	  }
	}
	else {
	  deprintf("no children\n");
	  deprintf("child is %p\n",child);
	  if(child->isEndPoint(ep1)) {
	    deprintf("ep1 is an endpoint\n");drun(fflush(stdout));
	    if(w1 > max2)
	      return ep1;
	    else
	      return curcl->id;
	  }
	  else {
	    deprintf("ep2 is an endpoint\n");drun(fflush(stdout));
	    if(w2 > max2)
	      return ep2;
	    else
	      return curcl->id;
	  }
	}
      }
    else
      {
	if(child->child)
	  return traverse(child,curcl->id,max2,-1,-1.0);
	else
	  assert(0);
      }
 default: assert(0);
  }
}


int center(cluster* root)
{

  deprintf (".");
  int cntr = traverse(root, -1, -1.0, -1, -1.0);
  deprintf("center is %d\n\n",cntr);
  return cntr;
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

