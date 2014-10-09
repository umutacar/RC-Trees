//////////////////////////////////////////////////////////////////////
// Queue.c
//
// Umut A. Acar
// Description: A queue implementation.  
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <assert.h>
#include "FreeList.h"
#include "Queue.h"
#include "Vertex.h"

int totalqueued=0; 

/* initialize a queue, return a pointer to the queue */
Queue *initQueue()
{
  Queue* queue = (Queue *) malloc (sizeof (Queue));
  queue->flist = initFreeList(sizeof(QNode));

  // Allocate a dummy item 
  queue->front = queue->back = (QNode*) allocBlock (queue->flist); 
  queue->front->next = queue->front;
  return queue; 
}

/* free the queue, and all its items and memory */
void destructQueue(Queue* q)
{
  destructFreeList(q->flist); 
  free (q); 
}


/* put something in the queue */
void enqueue(Queue* queue, void* d)
{
  deprintf("enqueded node = %d,%p  \n",((node *)d)->nId,d);
  totalqueued++;

  // Allocate a new node and init
  QNode *qn = (QNode *) allocBlock (queue->flist);
  qn->data = d;
  qn->next = queue->back->next;

  queue->back->next = qn;
  queue->back = qn ;

}

/* remove the front of the queue */
void* dequeue(Queue* queue)
{  
  assert (queue->front != queue->back); 

  QNode* tmp = queue->front->next;
  void* ret = tmp->data;
  
  queue->front->next = tmp->next;

  // Check if this was the last node.X
  if (tmp == queue->back) 
    queue->back = queue->front; 

  freeBlock(queue->flist, (char*) tmp);

  deprintf("\n\nDequeued %d ,%p \n",((node *) ret)->nId, ret);
  return ret;
}

/* This tell if the queue is empty*/
int isEmpty(Queue* queue)
{
  return queue->front == queue->back;
}

