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
#include "Data.h"
#include "Vertex.h"

extern tree_t* currentTree;
extern int currentrun; 
//PQueue* pq;

  
void initTreeContraction (Queue* q, tree_t* tree);
cluster* rerun(Queue *q,tree_t* tree,int doSynch);
cluster* initialrun(Queue *q, tree_t* tree);
int insertQueue (node* nd, Queue *q);
#endif
