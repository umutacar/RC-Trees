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
 * Data.c
 *
 * Jorge L. Vittes
 *
 * This code is for finding the maximum edged between two
 * marked nodes
 * Algorithm by Guy Blelloch, and Jorge Vittes
 *******************************************************/
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include "Data.h"
#include "AMath.h"

bin_data::bin_data(int u, int v, double w)
{
  len   = w;
  diam  = w;
  mpath1 = w;
  mpath2 = w;
}

bin_data::bin_data()
{
  len = 0.0;
  diam = 0.0;
  mpath1 = 0.0;
  mpath2 = 0.0;
}

void bin_data::reset()
{
  len = 0.0;
  diam = 0.0;
  mpath1 = 0.0;
  mpath2 = 0.0;
}

unary_data::unary_data(FILE* file)
{
  int val,mark;
  fscanf(file,"%i %i",&val,&mark); 
  diam  = 0.0;
  mpath = 0.0;
}

unary_data::unary_data()
{
  diam = 0.0;
  mpath = 0.0;
}

void unary_data::reset()
{  
  diam = 0.0;
  mpath = 0.0;
}

final_data::final_data()
{
  diam = 0.0;
}

void final_data::reset()
{  
  diam = 0.0; 
}

void dataContract(bin_data* left,bin_data* right, unary_data* sumCl, bin_data* dest)
{
  bin_data* left1 = GET_DT(left);
  int loff = GET_IN(left);

  bin_data* right1 = GET_DT(right);
  int roff = GET_IN(right);

  //Set the near max-path for the left data
  double nlmpath;
  double flmpath;
  if(loff == 1) {
    nlmpath = left1->mpath2;
    flmpath = left1->mpath1;
  }
  else {
    nlmpath = left1->mpath1;
    flmpath = left1->mpath2;
  }

  //Set the near max-path for the right data
  double nrmpath;
  double frmpath;
  if(roff == 1) {
    nrmpath = right1->mpath2;
    frmpath = right1->mpath1;
    }
  else {
    nrmpath = right1->mpath1;
    frmpath = right1->mpath2;
  }
  
  
  double diam = max3(left1->diam,right1->diam,nlmpath + nrmpath);
  diam = max3(diam,sumCl->diam,sumCl->mpath + dmax(nlmpath,nrmpath));
  
  double len  = right1->len + left1->len;

  double rmpath = max3(sumCl->mpath + right1->len, nlmpath + right1->len,frmpath);
  
  double lmpath = max3(sumCl->mpath + left1->len, nrmpath + left1->len, flmpath);

  dest->len   = len;
  dest->diam  = diam;
  dest->mpath1 = lmpath;
  dest->mpath2 = rmpath;
  

}
void rakeIn(bin_data* edgecl,unary_data* sumcl,unary_data* dest) 
{

  bin_data* edge = GET_DT(edgecl);
  int off = GET_IN(edgecl);
  
  double nmpath;
  double fmpath;
  if(off) {
    nmpath = edge->mpath2;
    fmpath = edge->mpath1;
  }
  else {
    nmpath = edge->mpath1;
    fmpath = edge->mpath2;
  }
     
  dest->diam = max3(edge->diam,sumcl->diam,nmpath+sumcl->mpath);
  dest->mpath = dmax(fmpath,sumcl->mpath + edge->len);

}
void addData(unary_data* clA, unary_data* clB, unary_data* dest)
{  
  dest->diam  = max3(clA->diam,clB->diam,clA->mpath+clB->mpath);
  dest->mpath = dmax(clA->mpath,clB->mpath);
  deprintf("Diameter is %lf\n",dest->diam);
}
void addNodeCluster(unary_data* cl, unary_data* ndata, unary_data* dest)
{
  addData(cl,ndata,dest);
}

void finalizeData(unary_data* cl,final_data* dest)
{
  dest->diam = cl->diam;
}

void pushDownData(bin_data* parent, bin_data* child)
{
}


int isEqual2(final_data a, final_data b) {
  return (a.diam == b.diam);
}

bin_data makeEdgeData (int u, int v, int w) 
{
  bin_data ret(u,v,(double) w);
 
  return ret;
  
}

// <MUST HAVE>
//   Read vertex data from a file
unary_data readVertexData(FILE* file) 
{
  unary_data res(file);
  return res;
}

// Read edge data from a file
bin_data readEdgeData (FILE* file,int v1, int v2) 
{
   int w;
  fscanf(file,"%i",&w);
  return makeEdgeData(v1,v2,w);
}


void updateDataWeight(bin_data* bd) 
{
  double w = rand()%10;
  deprintf("Changing to %lf \n",w);
  bd->len  = bd->mpath1 = 
    bd->diam = bd->mpath2 = w;
}
