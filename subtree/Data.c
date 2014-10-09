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

////////////////////////////////////////////////////////////////////////////////
// Data.c
//
// Jorge L. Vittes
//
// This code is for finding the
// Algorithm by Guy Blelloch, and Jorge Vittes
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <stdlib.h>
#include "Data.h"
#include "AMath.h"

/////////////////////////////////////////////////////////////////////////////////
// constructor for bin_data unsing edge information u,v, are endpoints, 
// w is a double.
/////////////////////////////////////////////////////////////////////////////////
bin_data::bin_data(int u, int v, double w)
{
  edge.weight = w;
  edge.n1 = u;
  edge.n2 = v;
}

/////////////////////////////////////////////////////////////////////////////////
// default constructor for binary data
/////////////////////////////////////////////////////////////////////////////////
bin_data::bin_data()
{
  edge.weight = 0.0;
  edge.n1 = -1;
  edge.n2 = -1;
}

/////////////////////////////////////////////////////////////////////////////////
// Reset function for binary data
/////////////////////////////////////////////////////////////////////////////////
void bin_data::reset()
{
  edge.weight = 0.0;
  edge.n1 = -1;
  edge.n2 = -1;
}

/////////////////////////////////////////////////////////////////////////////////
// constructor for unary data using vertex data to be extracted from a file
/////////////////////////////////////////////////////////////////////////////////
unary_data::unary_data(FILE* file)
{
  int val,mark;
  fscanf(file,"%i %i",&val,&mark);
  edge.weight = 0.0;
  edge.n1 = -1;
  edge.n2 = -1;
}

/////////////////////////////////////////////////////////////////////////////////
// Default constructor for unary data
/////////////////////////////////////////////////////////////////////////////////
unary_data::unary_data()
{
  edge.weight = 0.0;
  edge.n1 = -1;
  edge.n2 = -1;
}

////////////////////////////////////////////////////////////////////////////////
// Reset function for unary data
////////////////////////////////////////////////////////////////////////////////
void unary_data::reset()
{
  edge.weight = 0.0;
  edge.n1 = -1;
  edge.n2 = -1;
}

////////////////////////////////////////////////////////////////////////////////
// Default constructor for final data
////////////////////////////////////////////////////////////////////////////////
final_data::final_data()
{
  edge.weight = 0.0;
  edge.n1 = -1;
  edge.n2 = -1;
}

/////////////////////////////////////////////////////////////////////////////////
// Reset function for final data
/////////////////////////////////////////////////////////////////////////////////
void final_data::reset()
{
  edge.weight = 0.0;
  edge.n1 = -1;
  edge.n2 = -1;
}

////////////////////////////////////////////////////////////////////////////////////
// copy the edge data from one binary data onto another
////////////////////////////////////////////////////////////////////////////////////
void copyBData(bin_data* org, bin_data* dest)
{
  dest->edge.weight = org->edge.weight;
  dest->edge.n1 = org->edge.n1;
  dest->edge.n2 = org->edge.n2;
}

////////////////////////////////////////////////////////////////////////////////////
// copy the edge data from one unary data to another
////////////////////////////////////////////////////////////////////////////////////
void copyUData(unary_data* org, unary_data* dest)
{
  dest->edge.weight = org->edge.weight;
  dest->edge.n1 = org->edge.n1;
  dest->edge.n2 = org->edge.n2;
}

/////////////////////////////////////////////////////////////////////////////////////
// cpy the unary edge data from unary data org to binary data dest
/////////////////////////////////////////////////////////////////////////////////////
void copyUBData(unary_data* org, bin_data* dest)
{
  dest->edge.weight = org->edge.weight;
  dest->edge.n1 = org->edge.n1;
  dest->edge.n2 = org->edge.n2;
}

////////////////////////////////////////////////////////////////////////////////////
// using the collected information for the unary clusters, and the data from the 
// binary clusters combine it, and place it in dest for when a contract occured
////////////////////////////////////////////////////////////////////////////////////
void dataContract(bin_data* left,bin_data* right, unary_data* sumCl, bin_data* dest)
{
  left = GET_DT(left);
  right = GET_DT(right);
  if(left->edge.weight > sumCl->edge.weight)
    if(left->edge.weight > right->edge.weight)
      dest->edge = left->edge;
    else
      copyBData(right,dest);
  else
    if(sumCl->edge.weight > right->edge.weight)
      copyUBData(sumCl,dest);
    else
      copyBData(right,dest);
  
}

////////////////////////////////////////////////////////////////////////////////////
// using the collected information fro the unary clusters, and the data from the
// binary edge cluster combine it, and place it in dest for when a rake occurered
////////////////////////////////////////////////////////////////////////////////////
void rakeIn(bin_data* edgecl,unary_data* sumcl,unary_data* dest)
{
  edgecl = GET_DT(edgecl);  
  if(edgecl->edge.weight > sumcl->edge.weight)
    {
      dest->edge.weight = edgecl->edge.weight;
      dest->edge.n1 = edgecl->edge.n1;
      dest->edge.n2 = edgecl->edge.n2;
    }
  else
    {
      dest->edge.weight = sumcl->edge.weight;
      dest->edge.n1 = sumcl->edge.n1;
      dest->edge.n2 = sumcl->edge.n2;
    }
}

////////////////////////////////////////////////////////////////////////////////////
// combine the two unary datas clA, and clB and place that into dest
////////////////////////////////////////////////////////////////////////////////////
void addData(unary_data* clA, unary_data* clB, unary_data* dest)
{
  if(clA->edge.weight > clB->edge.weight)
    copyUData(clA,dest);
  else
    copyUData(clB,dest);
}

/////////////////////////////////////////////////////////////////////////////////
// copy the necessary data from unary data cl to the final destination dest
/////////////////////////////////////////////////////////////////////////////////
void finalizeData (unary_data *cl, final_data* dest) 
{
  dest->edge = cl->edge;
}

void pushDownData(bin_data* parent, bin_data* child)
{
}

/////////////////////////////////////////////////////////////////////////////////
// Return true if final_data a, and final_data b are the same as far as the 
// algorithm is concerned
/////////////////////////////////////////////////////////////////////////////////
int isEqual2(final_data a, final_data b)
{
  return ((a.edge.weight == b.edge.weight));
} 

/////////////////////////////////////////////////////////////////////////////////
// make the edge data given the edge information
/////////////////////////////////////////////////////////////////////////////////
bin_data makeEdgeData(int u, int v, int w)
{
  bin_data ret(u,v, (double) w);
  return ret;
}

//////////////////////////////////////////////////////////////////////////////////
// Read vertex data from a file
//////////////////////////////////////////////////////////////////////////////////
unary_data readVertexData(FILE* file)
{
  unary_data res(file); 
  return res;
}

/////////////////////////////////////////////////////////////////////////////////
// Read the edge data given endpoints v1 and v2
/////////////////////////////////////////////////////////////////////////////////
bin_data readEdgeData (FILE* file,int v1, int v2)
{
  int w;
  fscanf(file,"%i",&w);
  return makeEdgeData(v1,v2,w);
}

void updateDataWeight(bin_data* bd)
{
  double w = (double) (rand()%1000000);
  bd->edge.weight = (double) w;
   
}

