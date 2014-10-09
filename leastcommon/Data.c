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

///////////////////////////////////////////////////////////////////////
// Data.c
//
// Jorge L. Vittes
//
// This file defines the functions that are done on the application 
// data
// Algorithm by Guy Blelloch, and Jorge Vittes
///////////////////////////////////////////////////////////////////////
#include "AMath.h"
#include <stdio.h>
#include "Data.h" 
#include <math.h>



void dataContract(bin_data* left,bin_data* right, unary_data* sumCl, bin_data* dest)
{  
}

void rakeIn(bin_data* edgecl,unary_data* sumcl,unary_data* dest)
{
}

void addData(unary_data* clA, unary_data* clB, unary_data* dest)
{
}

void addNodeCluster(unary_data* cl, unary_data* ndata, unary_data* dest)
{
}

void finalizeData(unary_data* cl, final_data* dest)
{
}

void pushDownData(bin_data* parent, bin_data* child)
{
}

int isEqual2(final_data a, final_data b)
{
  return 1;
}

bin_data::bin_data(int u, int v, double w)
{
  count = 0;
}

bin_data::bin_data()
{
  count = 0;
}

void bin_data::reset()
{
  count = 0;
}

unary_data::unary_data()
{
  count = 0;
}

unary_data::unary_data(FILE* file)
{
  int val,mark;
  fscanf(file,"%i %i",&val,&mark);
  count =0;
}

void unary_data::reset()
{
  count = 0;
}

final_data::final_data()
{
  count = 0;
}

void final_data::reset()
{
  count = 0;
}

bin_data makeEdgeData (int u, int v, int w)
{
  bin_data res(u,v,(double) w);
  return res;
}

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
  
}
