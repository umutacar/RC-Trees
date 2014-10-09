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

/////////////////////////////////////////////////////////////////////
// Data.h
//
// Jorge L. Vittes
//
// defines the interface for the data files, which
// define what operation the tree contraction does
///////////////////////////////////////////////////////////////////

#ifndef _DATA_H_
#define _DATA_H_ 1

#include "Globals.h"
#include <stdio.h>
#include <stdint.h>

#define GET_DT(c) (bin_data *)((intptr_t) c & 0xFFFFFFFFFFFFFFFCL)
#define GET_IN(c) (intptr_t) c & 1L
#define GET_OT(c) ~((intptr_t)c & 1L) & 1L
#define THIS_DATA(d) ((GET_DT(d))->data + (GET_IN(d)))
#define OTHER_DATA(d) ((GET_DT(d))->data + (GET_OT(d)))
//#define EMPTY_DATA ((data_t) {0.0,0.0})
#define EMPTY_UDATA ((unary_data){0})
#define EMPTY_BDATA ((bin_data){0})



class bin_data{
 public:
  bin_data(int u, int v, double w);
  bin_data();
  int count;
  void print(){};
  void reset();
};

class unary_data{
 public:
  unary_data(FILE* file);
  unary_data();
  int count;
  void print()
    {deprintf("count is %d\n",count);};
  void reset();
};

class final_data{
 public:  
  final_data();
  int count;
  void print()
    {deprintf("count is %d\n",count);};
  void reset();
};



//#define EMPTY_DATA ((data_t) {0,-1,-1})


void dataContract(bin_data* left,bin_data* right, unary_data* sumCl, bin_data* dest);
void rakeIn(bin_data* edgecl,unary_data* sumcl,unary_data* dest);
void addData(unary_data* clA, unary_data* clB, unary_data* dest);
void addNodeCluster(unary_data* cl, unary_data* ndata, unary_data* dest);
void finalizeData(unary_data* cl,final_data* dest);
void pushDownData(bin_data* parent, bin_data* child);

//int isEqual(data_t a, data_t b);
int isEqual2(final_data a, final_data b);
//data_t emptyData();
bin_data makeEdgeData (int u, int v, int w); 

// <MUST HAVE>
//   Read vertex data from a file
unary_data readVertexData(FILE* file);

// Read edge data from a file
bin_data readEdgeData (FILE* file,int v1, int v2);


void updateDataWeight(bin_data* bd);

//#define INFINITY 536870912
#endif
