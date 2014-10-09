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
//#include "PQueue.h"

int link(node* thisNode, node* otherNode, int w, tree_t* tree, Queue* q);
int linkEdge(int n1, int n2,int w);
void cut(node* v1, node* v2, tree_t* tree, Queue* q);
void cutEdge(int n1, int n2);
int isEdge(int n1, int n2);

void setTreeAndQueue(tree_t* tree, Queue* q);
void contract();

void changeVertexData (node* v, Queue* q, clusterList* cList, void (*f)(unary_data* ));
void changeEdgeData (node* v, node* u, Queue* q, clusterList* cList, void (*f)(bin_data*));
cluster* propagate(clusterList* cList);
cluster* synchronize (node* v);
cluster* update(Queue *q,tree_t* tree);


final_data makeRegularRun(tree_t* otree);

void initTest ();
void finishTest ();
final_data propagateAndCheck (tree_t* tree, Queue* Q, clusterList* cList);
void printTimePerOp (clock_t start, clock_t finish, int nOps);


void testNCutsAndSync(char* treeName, int nCuts, char* fName);
void testNCutsWithSync(char* treeName, int nCuts, char* fName);
void testNCutsLC(char* treeName, int nCuts, char* fName);
void testNCutsWithLC(char* treeName, int nCuts, char* fName);
void testEdgeData(char* treeFile, int rounds, void (*updateWeight)(bin_data*));
void testVertexData(char* treeFile, int rounds, void (*updateWeight)(unary_data*));
void testVertexDataQuery(char* treeFile, int rounds, int opsPerRound, void (*changeMark)(unary_data*), unary_data (*readVertexData)(FILE*));
void testSynchronize (char* treeName, int goRandom, char* fName);
void testSynchronizeWithQuery (char* treeName, int goRandom, char* fName,void (*query)(tree_t*));
void testSynchronizeWithQueryTrace (char* treeName, int goRandom, char* fName,void (*query)(tree_t*,FILE*));

void testQuery(char* treeName, int rounds, void (*query)(tree_t*));
void testQuery_root (char* treeFile, int rounds, void (*query)(cluster *));
void testLinkCut (char* treeName, int goRandom, char* fName);
void testLinkCutChain (char* treeName, int goRandom);
void testLinkCutMSTChain (char* treeName, int goRandom,void (*query)(tree_t*));
void testLinkCutSyncChain (char* treeName, int goRandom);

#endif
