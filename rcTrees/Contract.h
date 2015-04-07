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

/////////////////////////////////////////////////////////////
// Contract.h
//
// Jorge L. Vittes
//
// Code for Structural, and Data Propagation on RC-Trees
// algorithm by Umut Acar, Guy Blelloch, and Jorge Vittes
/////////////////////////////////////////////////////////////
#ifndef _CONTRACT_H_
#define _CONTRACT_H_ 1

#include "Queue.h"
#include "Tree.h"
#include "Vertex.h"

extern tree_t* currentTree;
extern int currentrun; 
//PQueue* pq;

  
void initTreeContraction (Queue* q, tree_t* tree);
cluster* rerun(Queue *q,tree_t* tree,int doSynch);
cluster* initialrun(Queue *q, tree_t* tree);
int insertQueue (node* nd, Queue *q);
#endif
