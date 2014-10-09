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

/////////////////////////////////////////////////////////////////////////////
// Application.c
//
// Jorge L. Vittes
//
// This code is for finding the maximum edge in a path between two vertices
// Algorithm by Guy Blelloch, and Jorge Vittes
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <assert.h>
#include "Data.h"
#include "BinCluster.h"
#include "UnaryCluster.h"
#include "FinalCluster.h"
#include "Vertex.h"
#include "AMath.h"

int doPrint =0;
bin_data emptyDat;

/////////////////////////////////////////////////
// a small class for storing results related to a
// specific endpoint
////////////////////////////////////////////////
class result {
 public:
  int endp[2]; 
  bin_data* val[2]; 
    
  result (bin_data* d) {
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

  void insert (int ep, bin_data* v) {
    for (int i = 0; i < 2; ++i) 
      if (endp[i]==-1) {
	endp[i]=ep; 
	val[i]=v;
	break;
      }
  };

  bin_data* find (int ep) {
    for (int i = 0; i < 2; ++i) 
      if (endp[i]==ep) 
	return val[i];
    return &emptyDat;
  };
};

///////////////////////////////////////////////////
// Update the weight on an edge 
///////////////////////////////////////////////////
void updateWeight(bin_cluster* cl)
{
  cl->data.weight = (double) (rand()%MILLION);
  cl->data.n1     = cl->endp1;
  cl->data.n2     = cl->endp2;
}

//////////////////////////////////////////////////////////////////
// return the bin data that is actually corresponding to the
// edge witht the larger weight
//////////////////////////////////////////////////////////////////
bin_data* compare (bin_data* a, bin_data* b) 
{
  a = GET_DT(a);
  b = GET_DT(b);
  if (a->weight > b->weight) 
    return a; 
  else
    return b; 
}

bin_cluster* getBinCluster(cluster* cl)
{
  return GET_BC(cl);
}

///////////////////////////////////////////////////////////////
// process the cluster on the way up by keeping track of the
// largest edge between the original endpoint and the
// endpoint of the cluster
///////////////////////////////////////////////////////////////
void processCluster(cluster* clp,cluster* cl, result* r)
{
  deprintf("process Cluster %p\n",cl);
  cluster** siblings = cl->getSiblings (); 
  int vanishing = cl->getVanishing (); 

  //Get the data for the vanishing point and remove it;
  bin_data* vanish = r->find(vanishing);
  r->remove(vanishing);
  
  int nsib = cl->nSiblings();

  for (int i = 0; i < nsib; ++i) {
    bin_cluster* bcl = GET_BC(siblings[i]);
    deprintf("bcl is %p\n",bcl);
    bin_data* d = compare (&(bcl->data), vanish); 
    r->insert(bcl->getNonVanishing(),d);
    deprintf("inserting %p to %d\n",d,bcl->getNonVanishing());
  } 

}

//////////////////////////////////////////////////////////////
// find the edge with the highest weight in the path between
// u and v
//////////////////////////////////////////////////////////////
bin_data pathQuery(node* v, node* u)
{
  cluster *clv,*clvp;//cluster for v, and parent cluster for v
  cluster *clu,*clup;//cluster for u, and parent cluster for u
  
  result rU(&emptyDat);
  result rV(&emptyDat);

  clv = v->data;
  clu = u->data;

  clvp = clv->parent;
  clup = clu->parent;

  while(clup != clvp)  {
      // Base case. 
      deprintf(".");
      deprintf("clvp = %d\n", clvp->id);
      deprintf("clup = %d\n", clup->id);

      // "Recursive" (loopersive) case
      if(clvp->height <= clup->height) {
	processCluster(clvp,clv,&rV);
	clv  = clvp;
	clvp = clvp->parent;      
	if(!clvp) return emptyDat;
      }
      else {
	processCluster(clup,clu,&rU);
	clu  = clup;
	clup = clup->parent;	
	if(!clup) return emptyDat;
      }
    }//while
 
  int van = clu -> getVanishing(); 

  return *(compare (rU.find (van), rV.find (van)));

}

///////////////////////////////////////////////
// return the root of the support tree v is in
///////////////////////////////////////////////
cluster* root(node* v)
{
  cluster* cl = v->data;
  while(cl->parent)
    {
      cl = cl->parent;
    }
  return cl;
}
