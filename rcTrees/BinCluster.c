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

////////////////////////////////////////////////////////////////////////////
// Bincluster.c
//
// Jorge Vittes
//
// Functions on binary clusters
///////////////////////////////////////////////////////////////////////////

#include <math.h>
#include <malloc.h>
#include <iostream>
#include "FreeList.h"
#include "Data.h"
#include "BaseCluster.h"
#include "BinCluster.h"
#include "UnaryCluster.h"
#include "FinalCluster.h"

#define AddCluster(a,b,c) AddData(&(a->data),&(b->data), &(c->data))

//////////////////////////////////////////////////////////////////
// Constructor for binary clusters
//////////////////////////////////////////////////////////////////
bin_cluster::bin_cluster(bin_data dat)
 {
    data = dat;
    parent = NULL;
    child = NULL;
    binaryCl[0] = binaryCl[1] = NULL;
    
    affected = 0;
    for(int i=0;i<MAX_DEGREE -2;i++)
      unaryCl[i]=NULL;
    endpoints =2;
  }

//////////////////////////////////////////////////////////////////
// Default constructor for binary Cluster
//////////////////////////////////////////////////////////////////
bin_cluster::bin_cluster()
 {
    parent = NULL;
    child = NULL;
    binaryCl[0] = binaryCl[1] = NULL;
   
    affected = 0;
    for(int i=0;i<MAX_DEGREE -2;i++)
      unaryCl[i]=NULL;
    endpoints =2;
  }

///////////////////////////////////////////////////////////////////
// function to pass the directionality bit with the data pointer 
///////////////////////////////////////////////////////////////////
bin_data* getBinData(bin_cluster* bc) 
{
  return BIN_DATA(bc);
}
 
///////////////////////////////////////////////////////////////////
// collect all the data on the unary clusters combine it, then 
// combine it with that of the binary clusters 
///////////////////////////////////////////////////////////////////
void bin_cluster::synchronize()
{
  int i;
  unary_cluster sumCl;

  //sum up all the clusters
  for(i=0;i<MAX_DEGREE-2;i++)
    {
      deprintf("for i %d\n",i);
      unary_cluster* curchild = unaryCl[i];
      if(curchild) {       
	addCluster(&sumCl,curchild,&sumCl);
      }
    }
  
  //take in to account the data on the node
  addCluster(&sumCl, ((unary_cluster*)child), &sumCl);
  
  //calculate the return value
  deprintf("Contract %d left=%d, right=%d\n",id,(GET_BC(binaryCl[0]))->id,(GET_BC(binaryCl[1]))->id);
  dataContract(getBinData(binaryCl[0]),getBinData(binaryCl[1]),&(sumCl.data),&data);
}

///////////////////////////////////////////////////////////////
// Reset the cluster by resetting the data and the pointers
///////////////////////////////////////////////////////////////
void bin_cluster::reset()
{
  int i;
  data.reset();
  binaryCl[0] = binaryCl[1] = NULL;
  for(i=0;i<MAX_DEGREE-2;i++)
    unaryCl[i] = 0;
}

///////////////////////////////////
// Return the vanishing endpoint 
///////////////////////////////////
int bin_cluster::getVanishing() 
{
  return parent -> id; 
}

///////////////////////////////////////
// return the non-vanishing endpoint 
///////////////////////////////////////
int bin_cluster::getNonVanishing() 
{
  if (endp1==parent->id) 
    return endp2;
  else 
    return endp1;
}

////////////////////////////////////////////
// return an array of all child clusters 
////////////////////////////////////////////
cluster** bin_cluster::getClusters()
{
  int i;
  cluster** arr = (cluster**)malloc(sizeof(cluster*)*MAX_DEGREE);
  for(i=0;i<2;i++)
    {
      arr[i] = binaryCl[i];
    }
  for(i=0;i<MAX_DEGREE-2;i++)
    {
      arr[i+2] = unaryCl[i];
    }
  return arr;
}

////////////////////////////////////////////////////////
// siblings is just the set of binary clusters that are
// your siblings 
////////////////////////////////////////////////////////
cluster**  bin_cluster::getSiblings()   {
  assert (parent); 
  cluster** p = parent->getBinaryClusters ();

  if (parent -> endpoints > 1) 
    if (GET_CL(p[0])==this)
      return p+1;
    else
      return p; 
  else
    return NULL; 
}

////////////////////////////////////////////////
// return the array of unary clusters 
////////////////////////////////////////////////
cluster** bin_cluster::getUnaryClusters()
{
  return (cluster**) unaryCl;
}

///////////////////////////////////////////////
// return the array of binary clusters 
///////////////////////////////////////////////
cluster** bin_cluster::getBinaryClusters()
{
  return (cluster**) binaryCl;
}

////////////////////////////////////////////////
// update the weight
////////////////////////////////////////////////
void  bin_cluster::updateWeight()
{
  updateDataWeight(&data); 
}


