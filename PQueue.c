//////////////////////////////////////////////////////////////////////
// PQueue.c
//
// Umut A. Acar, Jorge Vittes
//
//Description: A max-priority queue implementation, smaller first
// This is not a priority queue anymore.  It is a list. 
//////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "PQueue.h"

/* Initialize the priority queue */
PQueue* initPQ ()
{
  PQueue* pq = (PQueue*) malloc (sizeof (PQueue)); 
  assert (pq); 
  pq -> flist = initPreAllocatedFreeList (MAX_PQ_SIZE,sizeof(PQNode)); 
  pq -> head = NULL; 

  return pq; 
}

/* insert data into the priority queue with priority p */
void insertPQ (PQueue* pq, int p, void* data) 
{
  assert (pq); 
  assert (data); 

  //  printf ("Inserting priority = %d\n", p); 
  PQNode *prev = NULL; 
  PQNode *head = pq->head;

  if (head) {
    //        if (head->priority > p) 
    //       break; 

    prev = head;     
    head = head->next; 
  }

  // Initialize the node
  PQNode* n = (PQNode*) allocBlock (pq->flist); 
  n->data = data; 
  n->next = head;     
  n->priority = p; 
  // Set the prev or the head to point to n
  if (prev) 
    prev->next = n; 
  else
    pq->head = n; 
}

/* remove the node with the minimum priority */
void* removeMin (PQueue* pq)
{
  assert (pq); 
  assert (pq->head); 
  void* data = pq->head->data; 
  pq->head = pq->head->next; 
  //  printf ("Removed priority = %d\n", pq->head->priority); 

  return data; 
}

/* return 1 if the pq is empty, 0 otherwise */
int isEmptyPQ (PQueue* pq)
{
  assert (pq); 
  return pq->head==NULL; 
}
