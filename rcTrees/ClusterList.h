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

//////////////////////////////////////////////////////////////////////////
// ClusterList.h
//
// Jorge Vittes
//
// A structure to store a list of clusters for returning after contraction
///////////////////////////////////////////////////////////////////////////

#include "Globals.h"
#include "Data.h"
#include "BinCluster.h"
#include "UnaryCluster.h"

#ifndef _CLUSTER_LIST_H_
#define _CLUSTER_LIST_H_ 1

typedef struct clusterNode{
  cluster* cl;
  struct clusterNode *next;
}clusterNode;

class clusterList{
 public:
  clusterList();
  ~clusterList();
  clusterNode* head;
};

void initClusterList();
void insertCluster(cluster* cl, clusterList* list);
cluster* removeCluster(clusterList* list);

#endif
