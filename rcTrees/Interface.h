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


///////////////////////////////////////////////////////////////////////////
// Interface.c
// 
// Jorge Luis Vittes
//
// Convinient interfacing functions for RC-Trees
///////////////////////////////////////////////////////////////////////////
#ifndef _INTERFACE_H_
#define _INTERFACE_H_ 1

#include <time.h>
#include "BinCluster.h"
#include "UnaryCluster.h"
#include "Data.h"
#include "Vertex.h"
#include "Tree.h"
#include "Queue.h"

class RC_Forest {
 private:
  Queue *cQueue;
  tree_t* cTree;
  clusterList affectedClusters;

 public:

  node* vertex(int n) {return cTree->vertexArray+n;}

  int link(node* thisNode, node* otherNode, int w);

  void cut(node* v1, node* v2);

  int isEdge(node* v1, node* v2);

  RC_Forest(int n);

  ~RC_Forest();

  void contract();

  void changeVertexData (node* v, void (*f)(unary_data* ));

  void changeEdgeData (node* v, node* u, void (*f)(bin_data*));

  cluster* propagate();
};

#endif
