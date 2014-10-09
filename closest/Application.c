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


////////////////////////////////////////////////////////////
// Application.c
//
// Jorge L. Vittes
//
// This code is for finding the closest marked vertex to 
// a particular marked vertex
// Algorithm by Guy Blelloch, and Jorge Vittes
////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include "Data.h"
#include "UnaryCluster.h"
#include "BinCluster.h"
#include "FinalCluster.h"
#include "Vertex.h"

/////////////////////////////////////////////////////////////
// return the ith binary cluster
/////////////////////////////////////////////////////////////
bin_cluster* getBinCl(cluster* cl, int i)
{  
  cluster** bclusters = cl->getBinaryClusters();
  return((bin_cluster*) bclusters[i]);     
}

/////////////////////////////////////////////////////////////
// return the ith unary cluster
/////////////////////////////////////////////////////////////
unary_cluster* getUnaryCl(cluster* cl, int i)
{
  cluster** uclusters = cl->getUnaryClusters();
  return((unary_cluster*) uclusters[i]);
    
}


/////////////////////////////////////////////////////////////
// This function takes the sum of all the scars
// and the value of the outgoing scar and returns 
// the value that the neighbor's scars gets 
/////////////////////////////////////////////////////////////
double getClosest(cluster* clp)
{
  int i;
  double closest;
  //cluster* cl;
  bin_data* bd;
  closest = INFINITY;
  double bclosest;

  for(i=0;i < clp->endpoints;i++)
    {
      bin_cluster* bcl = getBinCl(clp,i);
      if(bcl) {
	bd = BIN_DATA(bcl);
	bclosest = getNearClosest(bd);
	if(bclosest < closest) {
	  closest = bclosest;
	deprintf("closest is %lf \n",closest);
	}      
      }
    }
  for(i=0;i < MAX_DEGREE - clp->endpoints;i++)
    {
      unary_cluster* cl = getUnaryCl(clp,i);
      if(cl) {
	unary_data* dat = &(cl->data);
	if(dat->closest < closest) {
	  closest = dat->closest;
	  deprintf("closest is %lf \n",closest);
	}    
      }
    }
  unary_data* dat = &(((unary_cluster*)clp->child)->data);
  if(dat->closest < closest)
    closest = dat->closest;
   deprintf("closest is %lf \n",closest);
  return closest;
}


///////////////////////////////////////////////////////
// Given the id for one endpoint of a binary cluster 
// return the other endpoint id
////////////////////////////////////.//////////////////
int otherEndpoint(bin_cluster* cl, int id)
{
  cl = GET_BC(cl);
  if(cl->endp1 == id)
    return cl->endp2;
  else
    return cl->endp1;
}

/////////////////////////////////////////////////////
// Find the distance to the closest marked node to v
/////////////////////////////////////////////////////
double findClosest(node* v)
{
  cluster *cl,*clp;
  bin_cluster *leftCl, *rightCl;
  
  double ans;
  double closest;
  double bclosest;
  double dist1,dist2,dist,newDist1,newDist2; //distance from the endpoint to v;
  double llen,rlen;

  int ep1 = v->nId, ep2 = v->nId;
  int newEp1, newEp2;

  cl  = v->data;
  clp = cl->parent;
  dist1 = 0.0;
  dist2 = 0.0;
  ans = INFINITY;
  //debug=1;
  deprintf("\n");
  while(clp)
    {
      deprintf("clp is %d\n",clp->id);
      deprintf("dist1 = %lf, dist2 = %lf, ep1 = %d, ep2 = %d\n",dist1,dist2,ep1,ep2);
      switch(clp->endpoints) {
      case 1  : {
	//Get the closest node to the endpoint
	closest = getClosest(clp);
	deprintf("closest is %lf \n",closest);
	deprintf("Rake event \n");

	//Find the length across the binary cluster
	leftCl = GET_BC(getBinCl(clp,0));
	llen = leftCl->data.len;

	//Update the distance to the endpoint
	if(ep1 == clp->id) dist = dist1;
	else dist = dist2;

	//Update the result
	if(ans > closest + dist)
	  ans = closest + dist;


	//Update the endpoint
	newEp1 = ((unary_cluster*)clp)->endp1;

	if(newEp1 == ep1)      newDist1 = dist1;
	else if(newEp1 == ep1) newDist1 = dist2;
	else                   newDist1 = dist + llen;

	ep1   = newEp1;
	dist1 = newDist1;
      }
	break;

      case 2 : {
	bin_cluster* bcl = (bin_cluster*) clp;
	bin_cluster* bc = GET_BC(getBinCl(clp,0));
	bclosest = getNearClosest(BIN_DATA(bc));
	if(bclosest < closest) {
	  closest = bclosest;
	  deprintf("closest is %lf \n",closest);
	}    
	closest = getClosest(clp);
	deprintf("closest is %lf \n",closest);
	deprintf("Compress Event\n");
	bcl = GET_BC(bcl);
	
	//Find the length across the binary clusters
	leftCl  = GET_BC(bcl->binaryCl[0]);
	llen    = leftCl->data.len;

	rightCl = GET_BC(bcl->binaryCl[1]);
	rlen    = rightCl->data.len;

	
	//Update the distance to the endpoint
	if(ep1 == bcl->id) dist = dist1;
	else               dist = dist2;

	//Update the result
	if(ans > closest + dist)
	  ans = closest + dist;

	//Update the endpoint
	newEp1 = otherEndpoint(bcl->binaryCl[0],clp->id);
	newEp2 = otherEndpoint(bcl->binaryCl[1],clp->id);
	
	if(newEp1 == ep1)      newDist1 = dist1;
	else if(newEp1 == ep2) newDist1 = dist2;
	else                   newDist1 = dist + llen;

	if(newEp2 == ep1)      newDist2 = dist1;
	else if(newEp2 == ep2) newDist2 = dist2;
	else                   newDist2 = dist + rlen;
	
	deprintf("llen = %lf\n",llen);
	deprintf("rlen = %lf\n",rlen);
	ep1   = newEp1;
	ep2   = newEp2;
	dist1 = newDist1;
	dist2 = newDist2;
      }
	break;
      case 0 :
	{
	closest = getClosest(clp);
	

	//Update the distance to the endpoint
	if(ep1 == clp->id) dist = dist1;
	else              dist = dist2;

	//Update the result
	if(ans > closest + dist)
	  ans = closest + dist;
       
	}
	break;
      }
      deprintf("ans is %lf\n",ans);
      cl = clp;
      clp = cl->parent;
      
    }
  //printf("The closest to %d is at %lf distance\n",v->nId,ans);
  return ans;
}
