/******************************************************
 * Tree.c
 *
 * Jorge L. Vittes and Umut Acar
 *
 * This code is for loading and editing tree data
 * marked nodes
 * Algorithm by Umut Acar, Guy Blelloch, and Jorge Vittes
*******************************************************/

#ifndef _TREE_H_
#define _TREE_H_ 1

#include "Globals.h"
#include "UnaryCluster.h"
#include "BinCluster.h"
#include "FinalCluster.h"
#include "FreeList.h"
#include "Vertex.h"

typedef struct edge_t {
  int eps[2];
  double weight;
}edge_t;

typedef struct tree_t {
  int n; 
  //cluster* vertexClusters;
  //cluster* vertexDataClusters;
  FreeList* nodeList;
  FreeList* BClusters;
  FreeList* UClusters;
  FreeList* vertexList;
  cluster** clusterList;
  node* vertexArray; 
  edge_t* edgeArray;
} tree_t; 

extern int ucs;
extern int bcs;

tree_t *initTree  (int nVertices);
int verifyTree (tree_t* tree);
int verifyContractionTree (tree_t* tree);
int addEdge(node* v1, node* v2, bin_data data, tree_t* tree);
void destructTree(tree_t* tree);
int deleteEdge(node* n1, node* n2, tree_t* tree);
tree_t* loadtree(char* tree);
tree_t* loadtree_param_vertex(char* treeFile,unary_data (*readVertexData)(FILE* file)); 
tree_t* loadtreeAndEdges(char* treeFile);
tree_t* makeEmptyTree(int n);
void copyTree(tree_t* orTree, tree_t* coTree);
void printTree(tree_t* tree);
node* traverseTree(node* n1,int n, int* num, node* par);
void traverseTree2(node* n1, int* num, node* par);

#endif
