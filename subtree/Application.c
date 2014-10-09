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
// Application.c
//
// Jorge L. Vittes
//
// This code is for finding the largest edge in a subtree
// Algorithm by Guy Blelloch, and Jorge Vittes
//////////////////////////////////////////////////////////////////////

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

data_t* store;

///////////////////////////////////////////////////////////////////////////
// Print the data information
///////////////////////////////////////////////////////////////////////////
void printData(data_t* dat)
{
  deprintf("weight = %lf, n1 = %d, n2 = %d\n",dat->weight, dat->n1,dat->n2);
}

///////////////////////////////////////////////////
// a small class for storing results related to a
// specific endpoint
///////////////////////////////////////////////////
class result {
 public:
  int endp[2]; 
  data_t* val[2]; 
  
  result (data_t* d) {
    endp[0] = -1;
    endp[1] = -1; 
    val[0] = d;
    val[1] = d;
  };

  void remove (int ep) {    
    for (int i = 0; i < 2; ++i) 
      if (endp[i] == ep) 
	endp[i]=-1;
  };

  void insert (int ep, data_t* v) {
    for (int i = 0; i < 2; ++i) 
      if (endp[i]==-1) {
	endp[i]=ep; 
	val[i]=v;
	break;
      }
    
  };

  void print()
    {
      for(int i=0 ;i <2 ;i++)
	deprintf("ep %d val %p\n",endp[i],val[i]);
    }

  data_t* find (int ep) {
    for (int i = 0; i < 2; ++i) 
      if (endp[i]==ep) 
	return val[i];
    return NULL;
  };
  
 
};

///////////////////////////////////////////////
// compare the edge data for a with that for b
// and return the one with the larger weight
///////////////////////////////////////////////
data_t* compare (data_t* a, data_t* b) 
{
  if (a->weight > b->weight) 
    return a; 
  else
    return b; 
}

//////////////////////////////////////////////
// Get the edge data from the cluster
//////////////////////////////////////////////
data_t* getData(cluster* cl)
{
  PURIFY(&cl);
  switch(cl->endpoints) {
  case 0: {
    final_cluster* fcl = (final_cluster*) cl;
    return &(fcl->data.edge);
  }; break;
  case 1: {
    unary_cluster* ucl = (unary_cluster*) cl;
    return &(ucl->data.edge);
  }
  case 2: {
    bin_cluster* bcl = (bin_cluster*) cl;
    return &(bcl->data.edge);
  } 
  }
  return NULL;
}

////////////////////////////////////////////////////
// Combine the data for all the other children 
// clusters except cl
////////////////////////////////////////////////////
data_t* addAllOthers(cluster* cl, data_t* dat)
{
  cluster** clusters = cl->parent->getClusters();
  for(int i=0;i<MAX_DEGREE;i++)
    {
      if(clusters[i]) 
	{
	  cluster* cl2 = GET_CL(clusters[i]);
	  if(cl2 != cl)
	    dat = compare(dat,getData(cl2));
	}
    }
  free(clusters);
  return dat;
}

int met =0;

//////////////////////////////////////////////////////////////
// record if this is the meeting point between the root path 
/// and the vertex path to the root of the support tree
//////////////////////////////////////////////////////////////
int meeting(cluster* rcl,cluster* cl)
{
  if((rcl == cl->parent) && !met)
    {
      met = 1;
      return 1;
    }
  return 0;
}

/////////////////////////////////////////////////////////////
// Expand the subtree related to the vanishing endpoint of cl
//////////////////////////////////////////////////////////////
void grow(cluster* cl, result* r, cluster* rcl)
{
  cluster** siblings = cl->getSiblings (); 
  int vanishing = cl->getVanishing (); 
  data_t* res;
 
  //Get the data for the vanishing point and remove it;
  data_t* vanish = r->find(vanishing);
  if(vanish) {
    r->remove(vanishing);
    res = vanish;
     
    int insert =0;
    if(!meeting(rcl,cl)) {    
      for (int i = 0; i < cl -> nSiblings(); ++i) {
	bin_cluster* bcl = GET_BC(siblings[i]);
	res = addAllOthers(cl,vanish); 	
	r->insert(bcl->getNonVanishing(),res);
	insert =1;
      }
      if(!insert) {
	
	store = addAllOthers(cl,vanish);
      }
    }
   
  }
  else
    deprintf("no vanishing\n");
}

/////////////////////////////////////////////////////////
// If this is the first node do something different
////////////////////////////////////////////////////////
void firstNode(cluster* cl, result* r, cluster* rcl)
{
  cl = GET_CL(cl);
  cluster** siblings = cl->getSiblings (); 
  int vanishing = cl->getVanishing (); 
  data_t* res;

  //Get the data for the vanishing point and remove it;
  data_t* vanish = r->find(vanishing);
  r->remove(vanishing);
  res = vanish;
  
  deprintf("firstNode %d\n",cl->id);

  if(!meeting(rcl->parent,cl)) {    
    for (int i = 0; i < cl -> nSiblings(); ++i) {
      bin_cluster* bcl = GET_BC(siblings[i]);
      res = addAllOthers(bcl,vanish); 
      deprintf("res is ");printData(res);
      if(bcl->nSiblings()) {
	bin_cluster* bcl2 = GET_BC((bcl->getSiblings())[0]);
	r->insert(bcl2->getNonVanishing(),res);
      }
      else
	{	  
	  store = res;
	}
    }
  }
  else {
    int insert =0;
    deprintf("joining root\n");
    for (int i = 0; i < cl -> nSiblings(); ++i) {
      bin_cluster* bcl = GET_BC(siblings[i]);
      res = addAllOthers(rcl,vanish); 
      deprintf("res is ");printData(res);
      r->insert(bcl->getNonVanishing(),res);
      insert =1;
    }
    if(!insert) {
      deprintf("store now\n");
      store = addAllOthers(rcl,vanish);
    }
  }

}

data_t c_largestEdge(node* r, node* v)
{
  cluster *clv,*clvp;//cluster for v, and parent cluster for v
  cluster *clr,*clrp;//cluster for u, and parent cluster for u

  data_t emptyDat = {0.0,-1,-1};
  met =0;

  result rV(&emptyDat);
  rV.insert(v->nId,&emptyDat);
  
  clv = v->data;
  clr = r->data;

  clvp = clv->parent;
  clrp = clr->parent;
  while(clrp->height < clvp->height)
    {
      clr = clrp;
      clrp= clrp->parent;
    }

  firstNode(clv,&rV,clr);
  clv  = clvp;
  clvp = clvp->parent;
 

  while(clvp)  {      
      // "Recursive" (loopersive) case
      if(clvp->height <= clrp->height) {
	grow(clv,&rV,clrp);
	clv  = clvp;
	clvp = clvp->parent;
      }
      else {	
	clr  = clrp;
	clrp = clrp->parent;
      }
    }//while

  return *store;
}


data_t largestEdge(node* root, node* v)
{
  data_t edge;  
  edge = c_largestEdge(root,v); 
  return edge;
}
