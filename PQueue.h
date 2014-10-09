/******************************************************
 * PQueue.h
 *
 * Umut A. Acar and Jorge L. Vittes
 *
 * Interface for a priority queue
 *********************************************************/

#ifndef _PQUEUE_H_
#define _PQUEUE_H_ 1

#include "FreeList.h"
#define MAX_PQ_SIZE 10
typedef struct PQNode {
  int priority; 
  void* data; 
  struct PQNode* next; 
} PQNode; 

typedef struct PQueue  {
  PQNode* head; 
  FreeList* flist; 
} PQueue;

PQueue* initPQ (void); 
void insertPQ (PQueue* pq, int p, void* data); 
void* removeMin (PQueue* pq); 
int isEmptyPQ (PQueue* pq);

#endif
