///////////////////////////////////////////////////////////////
// Contract.c
//
// Jorge Vittes
//
// Code for dynamic parallel tree contraction (RC-Trees)
// This file handles the structural, and data propagation
// algorithm by Umut Acar, Guy Blelloch, and Jorge Vittes
//////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <assert.h>

#include "Globals.h"
#include "BinCluster.h"
#include "FinalCluster.h"
#include "UnaryCluster.h"
#include "Data.h"
#include "Interface.h"
#include "Queue.h"
#include "Tree.h"
#include "Vertex.h"
#include "Contract.h"


#define DO_RAKE 1
#define DO_COMPRESS 2
#define DO_LIVE 3
#define DO_END 4

#define CLUSTER_OFFSET_ZERO 0 
#define CLUSTER_OFFSET_ONE 1

/*DEBUG STUFF */
int delf;
node* glNode;

int currentrun = 0;
int morework=0;
Queue *currentQueue;
tree_t* currentTree;
clusterList oldRootList;
clusterList newRootList;

/* END DEGUG STUFF */

///////////////////////////////////////////////////////////
// Insert node nd into the queue if it isn't already there 
///////////////////////////////////////////////////////////
int insertQueue (node* nd, Queue *q) 
{
  // printVertex (nd); 
  deprintf("insertQueue: Queue = %p \n",q);

  //If the node is not already marked
  if(nd->mark != QUEUED)  {
    //then mark it and enqueue it
    nd->mark = QUEUED;
    deprintf("Enqueued %d\n",nd->nId);
    enqueue(q, nd);
    //return success
    return 1;
  }
  
  deprintf("Already Queued\n");
  return 0; //otherwise return failure
}

////////////////////////////////////////////
// return the end of the queue ie dequeue
////////////////////////////////////////////
node* removeQueue (Queue *q) 
{
  assert (!isEmpty (q)); 
  node* nd; 
  deprintf("removeQueue: Queue = %p \n",q);
  nd = (node *) dequeue(q);

  assert (nd); 
  nd -> mark = NOT_QUEUED;
  return nd; 
}

/* Code to empty the queue */
void emptyQueue(Queue* q)
{
  while(!isEmpty(q))
    removeQueue(q);
}


//////////////////////////////////////////////
//  Delete entire family tree
/////////////////////////////////////////////
void deleteFam(node* n)
{
  node *tmp; 
  int i;
  
  while (n) {
    tmp = n; 
    n = n->descendant; 

    tmp->deleted = 1;
    tmp->mark = NOT_QUEUED; 
    tmp->descendant = NULL;
    for(i=0;i<MAX_DEGREE;i++)
      {
	tmp->scars[i].backscar=NULL;
	tmp->scars[i].cl = NULL;
      }
    FREE_NODE(tmp);
  }

  
}

//////////////////////////////////////////////////////////////////////////
// Delete a node for the contraction by marking it deleted and freeing
// all of its decendants 
/////////////////////////////////////////////////////////////////////////
void deleteNode(node* thisNode)
{
  thisNode->descendant->deleted = 1;
  thisNode->descendant->mark = NOT_QUEUED;

  deleteFam (thisNode->descendant->descendant); 
  thisNode->descendant->descendant = NULL;
}

//////////////////////////////////////////////////////////////////////// 
// check what need to be done for the contraction here and return 
// the appropriate response 
////////////////////////////////////////////////////////////////////////
int contractCheck (node* thisNode) 
{
  node *left, *right;
  scar *lbscar, *rbscar;

  switch (thisNode->degree) {
  case 0:
    return DO_END;
  case 1: 
    //if the degree is one, and the others degree is not 1, or my address
    //is less than the neighbor then contract   
    lbscar = thisNode->scars[thisNode->left].backscar;
    left = GET_NEIGHBOR(lbscar);
    if((left < thisNode) || (left->degree !=1))
      return DO_RAKE;
    return DO_LIVE;
  case 2: 
    lbscar = thisNode->scars[thisNode->left].backscar;
    rbscar = thisNode->scars[thisNode->right].backscar;
    left  = GET_NEIGHBOR(lbscar);
    right = GET_NEIGHBOR(rbscar);

    /* And if there are the correct coin ids then contract */
     if ((left->degree != 1) && (thisNode->id > left->id) &&
	 (right->degree != 1) && (thisNode->id >right->id)
	)
       return DO_COMPRESS;
     return DO_LIVE;
  default: 
    return DO_LIVE; 
  }
}

////////////////////////////////////////////////////////////////////////////////////
//  Setup the scar on left's descendant to point to right's descendant 
////////////////////////////////////////////////////////////////////////////////////
void shortCut (node* thisNode,node* left,node* right,int lindex,int rindex, int dir)
{
  scar* lscar;

  //get the scar
  lscar = (scar* ) ((int) (left->descendant) + (int) (thisNode->scars[lindex].backscar) - (int) left);

  //update the backscar, and neighbor by making it point to the other neighbor
  lscar->backscar = (scar *) ((int) right->descendant + (((int)(thisNode->scars[rindex].backscar)) - (int)right));
  lscar->cl = (cluster*)((int) (thisNode->vertex->cl) + dir);
}

//////////////////////////////////////////////////////////////////////////////
// Update the neighbors descendant by making it point to the other neighbor's 
// descendant, neighbor being neigh, other neighbor is other
//////////////////////////////////////////////////////////////////////////////
void doCompress(node* thisNode, node* left, node* right,int lindex,int rindex) 
{
  deprintf("Contracting %d \n", thisNode->nId); 
  setupVertexCluster (thisNode, COMPRESS_EVENT, currentTree, &oldRootList); 

  shortCut (thisNode,left,right,lindex,rindex, CLUSTER_OFFSET_ZERO);
  shortCut (thisNode,right,left,rindex,lindex, CLUSTER_OFFSET_ONE);

  bin_cluster* cl = (bin_cluster*) thisNode->vertex->cl;

  cl->affected = IS_AFFECTED;
  //cl->endpoints = 2;
  cl->endp1 = left->nId;
  cl->endp2 = right->nId;
}

///////////////////////////////////////////////////////////////////
//  Do the raking 
///////////////////////////////////////////////////////////////////
void doRake(node* thisNode,node* left, int lindex)
{ 
  deprintf("Raking \n");
  scar* lscar;
  setupVertexCluster (thisNode,RAKE_EVENT, currentTree, &oldRootList);

  unary_cluster* cl= (unary_cluster*) thisNode->vertex->cl;
  deprintf("the cluster is %p\n",cl);
  
  //set the neighbors scars correctly

  lscar = (scar *)((int)(left->descendant) + (int)(thisNode->scars[lindex].backscar) -(int) left);
  lscar->cl = thisNode->vertex->cl;
  lscar->backscar = NULL;
  drun(deprintf("the neighbor is %d\n",((cluster*)((int) lscar - ((int) (lscar) % sizeof(node))))->id  ));

  cl->affected = IS_AFFECTED;
  //cl->endpoints = 1;
  cl->endp1 = left->nId;
}

///////////////////////////////////////////////////////////////////////////////
// Queue all the descendant's neighbors since they are affected
///////////////////////////////////////////////////////////////////////////////
void queueAllDescNeighbors(node* thisNode,Queue* q)
{
  int i;
  scar* bscar;
  node* neigh;
  node* nd= thisNode->descendant;

  for(i=0;i<MAX_DEGREE;i++)
    {
      bscar = nd->scars[i].backscar;
      if(bscar && nd->scars[i].cl)
	{
	  
	  if((bscar->backscar) == &(nd->scars[i])) {
	    neigh = GET_NEIGHBOR(bscar);
	    insertQueue(neigh,q);
	  }
	}
    }  
}

/////////////////////////////////////////////////////////////////////
// doLive (for the initial run  in the inintial run if the node
// is going to live then copy down the neighbor's scars associated 
// with the node, and copy down the empty scars for the node
////////////////////////////////////////////////////////////////////
void doLive(node* thisNode)
{
  int i,new_degree;
  int neighs[MAX_DEGREE];
  node* neigh;
  node* desc;
  scar *neigh_scar, *desc_scar;

  drun(int j);
  drun(cluster* cl);

  neighs[0]=-1;neighs[1]=-1;

  deprintf("I live!!! I am %d\n",thisNode->nId);
  drun(deprintf("Degree is %d\n",thisNode->degree));
 
  // Compute degree, remember left and right
  new_degree = 0;
  for(i=0;i<MAX_DEGREE;i++) {
    neigh_scar = thisNode->scars[i].backscar;
    
    if (neigh_scar) { 
      neigh = GET_NEIGHBOR(neigh_scar);
      deprintf("i is %d\n",i);
      if(neigh->degree > 1) {
	neighs[new_degree] = i;
	++new_degree;
      }
    }
  }
  
  desc = thisNode->descendant;

  desc->degree = new_degree;
  desc->left   = neighs[0];
  desc->right  = neighs[1];

  // copy node data and your neighbors scars down to the next level
  desc->data = thisNode->data;

  for(i=0;i<MAX_DEGREE; i++) {
    neigh_scar = thisNode->scars[i].backscar;
  
    if (neigh_scar) { 
      neigh = GET_NEIGHBOR(neigh_scar);
      desc_scar= (scar *) ((int)neigh->descendant+ (int) neigh_scar - (int)neigh);

      desc_scar->cl       = neigh_scar->cl;
      desc_scar->backscar = desc->scars+i;     
      deprintf("Wrote %p at address %p\n",desc,GET_NEIGHBOR(desc_scar->backscar));
    }
    else {
      // no neighbor: just copy this scar down
      desc->scars[i].cl    = thisNode->scars[i].cl;
      desc->scars[i].backscar = NULL;

      drun(j= (int) thisNode->scars->cl & 3);
      drun(cl = GET_CL(thisNode->scars[i].cl));
      drun(deprintf("the cl is %p\n", cl));      
    }
  }
}
  
/////////////////////////////////////////////////////////////////////////
// Check if the leaf status has changed, ie. if it was degree one, but
// now it isn't, or visa-versa
////////////////////////////////////////////////////////////////////////
void checkLeafStatusChange(int old_degree, node* desc)
{
  int i;
  if (old_degree != desc->degree)
    if ((desc->degree==1) || (old_degree==1)) {
      node* neigh;
      scar* neigh_scar;

      for(i=0;i<MAX_DEGREE;i++)  {
	neigh_scar = desc->scars[i].backscar;
	if (neigh_scar) 
	  if(GET_NEIGHBOR(neigh_scar->backscar) == desc) {
	    neigh = GET_NEIGHBOR(neigh_scar);
	    insertQueue(neigh,currentQueue);
	  }
      }  
    }
}

//////////////////////////////////////////////////////////////////////
// check if the node has been undeleted
//////////////////////////////////////////////////////////////////////
void checkUndelete(node* thisNode, node* desc)
{
  node *neigh;
  scar* neigh_scar;
  if(desc->deleted) {
    int i;
    desc->deleted = 0;
    for(i=0;i<MAX_DEGREE; i++) {
      
      neigh_scar = thisNode->scars[i].backscar;
      if (neigh_scar) {
	neigh = GET_NEIGHBOR(neigh_scar);
	insertQueue(neigh->descendant,currentQueue);  
      }
    }
  }
}

///////////////////////////////////////////////////////////////////
// basic function that determines what to do with all nodes
//////////////////////////////////////////////////////////////////
void runNode(node* thisNode, Queue* q)
{
  int lindex,rindex;
  scar *lscar, *rscar;
  node *left=NULL,*right=NULL;

  deprintf("The degree is %d\n", thisNode->degree); 
  deprintf("The cluster is %p\n",thisNode->data);
  deprintf("The node is %p\n", thisNode);
  
  switch (contractCheck(thisNode)) {
  case DO_END:
    setupVertexCluster(thisNode,END_EVENT, currentTree, &oldRootList); //end event the event to end them all!
    thisNode->vertex->cl->affected = IS_AFFECTED;  
    deleteNode(thisNode);
    insertCluster(thisNode->vertex->cl,&newRootList);
    break;
  case DO_RAKE: 
    morework =1;
    lindex = thisNode->left;
    lscar = thisNode->scars[lindex].backscar;
    left  = GET_NEIGHBOR(lscar);

    doRake(thisNode,left,lindex);
    insertQueue(left->descendant,q);
    deleteNode(thisNode);
    break;

  case DO_COMPRESS: 
    morework =1;
    lindex = thisNode->left;
    rindex = thisNode->right;
    lscar = thisNode->scars[lindex].backscar;
    rscar = thisNode->scars[rindex].backscar;
    left  = GET_NEIGHBOR(lscar);
    right = GET_NEIGHBOR(rscar);

    doCompress(thisNode, left, right, lindex, rindex);    
    insertQueue(left->descendant,q);
    insertQueue(right->descendant,q);
    deleteNode(thisNode);    
    break;

  case DO_LIVE: {
    morework = 1;
    node *desc = thisNode->descendant;
    int old_degree = desc->degree;
    
    doLive(thisNode);
    
    // Queue all affected nodes. 
    insertQueue(desc,q);
    deprintf("Check Undeleted \n"); 
    // check if thisNode is un-deleted, queue neighbors if so
    checkUndelete(thisNode,desc);
    deprintf("Leaf status change \n"); 
      // Check if leaf status of the descendant changes and queue neighbors if so
    checkLeafStatusChange(old_degree,desc);
    
    //  Allocate a new node for the next round if needed
    if(!desc->descendant) {
      deprintf("Allocated new node \n");
      desc -> descendant = copyVertex (desc,currentTree->nodeList);
    }
    
  } break;
  default: printf ("Error in runNode"); exit (-9);
  }

}

//////////////////////////////////////////////////////////////
// Return whether the cluster has been deleted
//////////////////////////////////////////////////////////////
int isDeleted(cluster* cl)
{
  if(cl->affected == DELETED) return 1;    
  return 0;
}

/*
 */
void pushDownFun(cluster* parent, cluster* child)
{
  if(parent->endpoints == 2)
    if(child->endpoints == 2)
      {
	bin_cluster *bcp,*bc;
	bcp = (bin_cluster*) parent;
	bc  = (bin_cluster*) child;
	deprintf("Pushing down from %d, to %d \n",parent->id, child->id);
	pushDownData(&(bcp->data), &(bc->data));	
      }
}

/////////////////////////////////////////////////////////////
// To free the cluster, find out which pool it came from and
// put it back there
/////////////////////////////////////////////////////////////
void freeCluster(cluster* cl)
{
  deprintf("freeCluster %p\n",cl);
  if(cl->endpoints == 2) {
    drun(bcs--);
    deprintf("free binary\n");
    freeBlock(currentTree->BClusters,(char*) cl);
  }
  else {
    deprintf("free non-bnary\n");
    drun(ucs--);
    freeBlock(currentTree->UClusters,(char*) cl);
  }
}

////////////////////////////////////////////////////////////
// Push down any data from the old tree onto the new tree 
// by doing a traversal over the deleted clusters
//////////////////////////////////////////////////////////
void pushDown(cluster* cl)
{   
  cluster* ccl;
  int i;
  assert(cl);
 
  ccl = GET_CL(cl);  
  deprintf("pushDown: cl = %d\n",ccl->id);
 
  cluster** clusters = ccl->getClusters();
  for(i=0; i < MAX_DEGREE; ++i)
    {
      cluster* child = clusters[i];
      cluster* ccp = GET_CL(child);
      if(child) {
	pushDownFun(ccl,ccp);
	if(isDeleted(ccp)) {
	  if(ccp->child) {
	    deprintf("Child\n");
	    pushDown(child);
	    deprintf("ret\n");
	  }
	  freeCluster(ccp);
	}
      }
    }
  free(clusters);     
}

///////////////////////////////////////////////////////
// Go over the list of old trees, and for each one of
// them push down their data onto the new trees
///////////////////////////////////////////////////////
void pushDownList(clusterList* rootList)
{
  while(rootList->head != NULL)
    {
      cluster* cl = removeCluster(rootList);
      pushDown(cl);
      freeBlock(currentTree->UClusters,(char*) cl);
      deprintf("done 1\n");
    }
  deprintf("done\n");
}



void updateClusterContract(cluster* cl)
{

  PURIFY (&cl); 
  cl->synchronize (); 
  cl->affected = NOT_AFFECTED;

}

cluster* synchronizeClusterContract(cluster* cl)
{

  int i;
  cluster** clusters = cl->getClusters();
  for(i=0; i < MAX_DEGREE; ++i)
    {
      if(clusters[i]) {
	cluster* ccp = GET_CL(clusters[i]);
	deprintf("Child->affected is %d\n",ccp->affected);
	if(ccp->affected == IS_AFFECTED) {      
	  synchronizeClusterContract(ccp);	 
	}
      }
    }
  free(clusters);
  updateClusterContract(cl);
  return cl;
  
}

/////////////////////////////////////////////////////////////////
// To do the data propagation travese each new tree, and do a 
// post-order update.
/////////////////////////////////////////////////////////////////
cluster* syncList(clusterList* rootList)
{
  cluster* cl=NULL;
  int odebug = debug;
  //debug =1;
  while(rootList->head != NULL)
    {
      deprintf("Synch \n");
      cl = removeCluster(rootList);
      synchronizeClusterContract(cl);
      deprintf("done 1\n");
    }
  debug = odebug;
  deprintf("done\n");
  return cl;
}



///////////////////////////////////////////////////////////////
// rerun is to do a rerun
// Takes the elements of the queue, if there is no more 
// work it breaks, if it is a copy event is calculates the
// degree puts it in the node and makes sure the node has 
// a child, and if it is a run event it runs the wake up 
// code
/////////////////////////////////////////////////////////////
cluster* rerun(Queue *q,tree_t* tree, int doSynch)
{
  cluster* root;
  node *nd = NULL;
  int curheight = -1;
  deprintf(" \n Rerunning \n"); 
  
  currentTree  = tree;
  currentQueue = q;
 
  morework=1;
  //debug=1;
  while (!isEmpty(q)) {  
    // Loop
    // Free the qnode 
    drun(assert(!isEmpty(q)));
    nd = removeQueue(q);
    
    if(!(nd->deleted)) {    
      //If it is the end of a round then check if all is done, otherwise continue, resetting
      //the morework flag
      if(nd->height != curheight)
	{
	  curheight = nd->height;
	  deprintf("Height is %d\n",curheight);
	  if(morework == 0) break;
	  morework = 0;
	  }
      deprintf("Running %d\n",nd->nId);
      runNode(nd,q);
      drun(assert(verifyVertex(nd)));
      
    }
  }//while
  
  // Empty the queue
  emptyQueue(q);
  if(PUSHDOWN)
    {
    deprintf("push down\n");
    pushDownList(&oldRootList);
    }
  if(doSynch) {
     root = syncList(&newRootList);
  }
  
  return root; 
 
  
}


///////////////////////////////////////////////////////////////////////
// Initial Run
////////////////////////////////////////////////////////////////////////
void initTreeContraction (Queue* q, tree_t* tree) {
  int i,n;
  node* v;


  n = tree->n;
  currentTree = tree;
  initClusterList();
  oldRootList.head = NULL;
  newRootList.head = NULL;

  for (i = 1; i <= n; ++i) {
    v = tree ->vertexArray + i;
    deprintf("Enqueueuing %d\n",i);
    insertQueue(v,q);
    deprintf("Making descendant copy\n");    
    v->descendant = copyVertex (v,currentTree->nodeList);
  }
}
  

//////////////////////////////////////////////////////
// This is the initial run, All it does now is call 
// rerun since that is what an initial run is, a rerun
// of all the code
//////////////////////////////////////////////////////
cluster* initialrun(Queue *q, tree_t* tree)
{

  cluster* root;
  root = rerun(q,tree,1);
  
  return(root);
}

