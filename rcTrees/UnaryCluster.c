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


/***************************************************************************
 * UnaryCluster.c
 *
 * Jorge Vittes
 *
 * functions on unary clusters
 ************************************************************************/
#include <math.h>
#include <malloc.h>
#include "FreeList.h"
#include "Data.h"
#include "BaseCluster.h"
#include "UnaryCluster.h"
#include "BinCluster.h"
#include "FinalCluster.h"

/* Constructor for a unary cluster given unary data */
unary_cluster::unary_cluster(unary_data dat)
  {
      data = dat;
      parent = NULL;
      child = NULL;
      binaryCl[0] = NULL;
      
      affected = 0;
      for(int i=0;i<MAX_DEGREE-1;i++)
	unaryCl[i]=NULL;
      endpoints = 1;
      endp1 = 0;
  }

/* Default constructor for a unary cluster */
unary_cluster::unary_cluster()
  {
      parent = NULL;
      child = NULL;
      binaryCl[0] = NULL;
      
      affected = 0;
      for(int i=0;i<MAX_DEGREE-1;i++)
	unaryCl[i]=NULL;
      endpoints = 1;
      endp1 = 0;
  }

/* function to reset a unary cluster*/
void unary_cluster::reset()
{
  data.reset();
  affected = 0;
  binaryCl[0] = NULL;
  int i;
  for(i=0;i<MAX_DEGREE-1;i++)
    unaryCl[i] = NULL;
}

/* collect all the data on the unary clusters combine it, then combine
   it with that of the binary cluster */ 
void unary_cluster::synchronize() {
  int i;
  
  unary_data dat;

  for(i=0;i<MAX_DEGREE-1;i++)
    {
      if(unaryCl[i]) {
	dat = ((unary_cluster*)unaryCl[i])->data;
	i++;
	break;
      }
    }
  
  for(i=i;i<MAX_DEGREE-1;i++) {   
    unary_cluster* ucl = (unary_cluster*) unaryCl[i];
    if(ucl)
      addData(&dat,&(ucl->data),&dat);
  }
  addData(&dat,&(((unary_cluster *)(child))->data),&dat);
  rakeIn(getBinData(binaryCl[0]),&dat,&data);
}

/* Return the non non-vanishing endpoint */
int unary_cluster::getNonVanishing() 
{
  printf ("Fatal Moral Error: Unary clusters do not have non-vanishing end points (boundaries).\n");
  assert (0); 
}

/* Return the vanishing endpoint */
int unary_cluster::getVanishing() 
{
  return parent -> id; 
}

/* siblings is just the set of binary clusters that are
   your siblings */
cluster**  unary_cluster::getSiblings()   {
  assert (parent);  
  return (parent->getBinaryClusters ());
}

/* return an array of all child clusters */
cluster** unary_cluster::getClusters()
{
  int i;
  cluster** arr = (cluster**)malloc(sizeof(cluster*)*MAX_DEGREE); 
  arr[0] = binaryCl[0];    
  for(i=0;i<MAX_DEGREE-1;i++)
    {
      arr[i+1] = unaryCl[i];
    }
  return arr;
}

/* returen the array of unary clusters */
cluster** unary_cluster::getUnaryClusters()
{
  return (cluster**) unaryCl;
}

/* return the array of binary clusters */
cluster** unary_cluster::getBinaryClusters()
{
  return (cluster**) binaryCl;
}



