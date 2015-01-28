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
// queue.h
//
// Jorge Vittes, Umut Acar
//
// Code for dealing with a simple queue
//
////////////////////////////////////////////////////////////////////////

#ifndef _QUEUE_H_
#define _QUEUE_H_ 1

#include "FreeList.h"

typedef struct QNode {
  void* data;
  struct QNode* next;
} QNode;

typedef struct Queue {
  QNode* front;
  QNode* back;
  FreeList* flist;
} Queue;


//the total number ever placed in the queue.
extern int totalqueued;
Queue* initQueue();
void destructQueue(Queue* q);
void   enqueue(Queue* q, void* n);
void*  dequeue(Queue* q);
int    isEmpty(Queue* q);
#endif
