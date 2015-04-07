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
// FinalCluster.c
//
// Jorge Vittes
//
// functions on final clusters
////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <malloc.h>
#include "FreeList.h"
#include "Data.h"
#include "BaseCluster.h"
#include "UnaryCluster.h"
#include "FinalCluster.h"
#include "BinCluster.h"

////////////////////////////////////////////////////////
// Constructor for a final cluster given final data 
////////////////////////////////////////////////////////
final_cluster::final_cluster(final_data dat)
  {
      data = dat;
      parent = NULL;
      child = NULL;
     
      affected = 0;
      for(int i=0;i<MAX_DEGREE;i++)
	unaryCl[i]=NULL;
      endpoints = 0;
  }

////////////////////////////////////////////////////////
// Default constructor for a final cluster 
////////////////////////////////////////////////////////
final_cluster::final_cluster()
  {
      parent = NULL;
      child = NULL;
    
      affected = 0;
      for(int i=0;i<MAX_DEGREE;i++)
	unaryCl[i]=NULL;
      endpoints = 0;
  }

////////////////////////////////////////////////////////
// function to reset a final cluster
////////////////////////////////////////////////////////
void final_cluster::reset()
{
  data.reset();
  affected = 0;
  int i;
  for(i=0;i<MAX_DEGREE;i++)
    unaryCl[i] = NULL;
}

/////////////////////////////////////////////////////////
// Collect the information for all the unary clusters, 
// and copy the important stuff onto the final cluster 
////////////////////////////////////////////////////////
void final_cluster::synchronize() {
  int i;  
  unary_data dat;

  for(i=0;i<MAX_DEGREE;i++) {
      unary_cluster* child = unaryCl[i];
      if (child)
        addData (&dat, &(child->data),&dat); 
  }
  addData (&dat, &(((unary_cluster*) child)->data), &dat);
  finalizeData (&dat,&data);
}

////////////////////////////////////////////////////////
// Return the non non-vanishing endpoint 
////////////////////////////////////////////////////////
int final_cluster::getNonVanishing() 
{
  printf ("Fatal Moral Error: Unary clusters do not have non-vanishing end points (boundaries).\n"); 
  assert (0); 
}

/////////////////////////////////////////////////////////////
// Return the vanishing endpoint 
/////////////////////////////////////////////////////////////
int final_cluster::getVanishing() 
{
  printf ("Fatal Moral Error: Final clusters do not vanish.\n"); 
  assert (0); 
}

/////////////////////////////////////////////////////////////
// siblings is just the set of binary clusters that are
// your siblings 
/////////////////////////////////////////////////////////////
cluster**  final_cluster::getSiblings()   {
  printf ("Fatal Moral Error: Final clusters have no parents and therefore (binary) siblings\n"); 
  assert (0);
}

/////////////////////////////////////////////////////////////
// return an array of all child clusters 
/////////////////////////////////////////////////////////////
cluster** final_cluster::getClusters()
{
  cluster** arr = (cluster**)malloc(sizeof(cluster*)*MAX_DEGREE);
  for(int i=0;i<MAX_DEGREE;i++)
    {
      arr[i] = unaryCl[i];
    }
  return arr;
}

////////////////////////////////////////////////////////////
// return the array of unary clusters 
////////////////////////////////////////////////////////////
cluster** final_cluster::getUnaryClusters()
{
  return (cluster**) unaryCl;
}

///////////////////////////////////////////////////////////
// return the array of binary clusters 
///////////////////////////////////////////////////////////
cluster** final_cluster::getBinaryClusters()
{
  return (cluster**) NULL;
}


