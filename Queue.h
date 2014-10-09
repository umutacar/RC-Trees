/******************************************************
 * queue.h
 *
 * Jorge Vittes, Umut Acar
 *
 * Code for dealing with a simple queue
 *
 *********************************************************/

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
