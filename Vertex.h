//////////////////////////////////////////////////////////////////////
// Vertex.h
//
//
// Jorge L. Vittes, Umut A. Acar and Guy Blelloch
//
// Description: An interface for operations on vertices
//////////////////////////////////////////////////////////////////////

#ifndef _VERTEX_H_
#define _VERTEX_H_ 1

#include "Globals.h"
#include "Data.h"
#include "FreeList.h"
#include "BinCluster.h"
#include "UnaryCluster.h"
#include "ClusterList.h"


#define GET_NEIGHBOR(ptr) ((node *) ((int)(ptr) - ((int)(ptr) % sizeof(node))))

typedef struct scar { 
  //struct node* neighbor;
  cluster* cl;
  struct scar* backscar;
} scar;

typedef struct node { 
  unary_cluster* data;
  int nId;
  int height;
  int mark;
  char deleted;
  char left;
  char right;
  char degree;
  int affected;  
  int id; //id to determine contraction
  scar scars[MAX_DEGREE];
  struct node* descendant;
  class vertex_t* vertex;
} node;

class vertex_t {
 public:
  node* v;
  cluster* cl;
  vertex_t(node* nd);
  vertex_t();
};

class LLNode: vertex_t {
 public:
  LLNode* prev;
  LLNode* next;
};

void initVertex(node* v, unary_data data, int id, void* tr);
int isLeaf(node* v); 
void makeCanonical (node* v);
node* copyVertex (node* v, FreeList* flist); 
void printVertex (node* v);
int findEmptyScar (node* v);
bin_cluster* findEdgeCluster (node* v, node* w);
int verifyVertex (node* v);
void setupVertexCluster (node* thisNode, int event,void* tr,clusterList* rootList);
#endif
