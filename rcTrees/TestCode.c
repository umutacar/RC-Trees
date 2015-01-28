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


#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Globals.h"
#include "Interface.h"
#include "Contract.h"
#include "Application.h"

#include "TestCode.h"

#define SEED_FILE "seed"
int seed;  // this is used for debugging
#define MILLION 1000000

extern Queue *cQueue;
extern tree_t* cTree;

//////////////////////////////////////////////////////////////////////
// Some functions for testing
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Initializes the random number generator
// with the seed in file SEED
//////////////////////////////////////////////////////////////////////
void initTest () 
{
  FILE *file;

  // Initialize the random number generator with seed. 
  file = fopen(SEED_FILE,"r");
  if (!file) {printf ("Application must have a seed file"); exit (-1);}
  fscanf(file,"%i\n",&seed);
  srand(seed);
  fclose(file);
}

//////////////////////////////////////////////////////////////////////
// Write a new seed to the file SEED file
//////////////////////////////////////////////////////////////////////
void finishTest ()
{
  // Write seed to the 
  FILE *file;
  file = fopen(SEED_FILE,"w");
  fprintf(file,"%d\n",rand());
  fclose(file);
}

// Run tree contraction from scratch on the original tree, otree. 
final_data makeRegularRun(tree_t* otree)
{
  Queue* q =  initQueue (); 
  tree_t *coTree = initTree(otree->n); 
  cTree = coTree;
  copyTree(otree,coTree);
  
  // Do a tree contraction, from scratch
  initTreeContraction (q,coTree);

  cluster* root = initialrun(q,coTree);
  //synchronize (root);   

  final_data result = ((final_cluster*)root)->data; 

  destructTree(coTree);
  destructQueue (q); 
  return result;  
}



//////////////////////////////////////////////////////////////////////
// syncnAndCheck_
//  
// Does an update and if NDEBUG is not set (in the debugging mode,
// it verifies the update.  It will exit when incorrect. 
//////////////////////////////////////////////////////////////////////
final_data syncAndCheck_(tree_t *tree, Queue* Q)
{
  cluster *root1;
  final_data data1, data2; 

  deprintf("Rennuning\n\n");
      
  drun(assert (verifyContractionTree(tree)));
  deprintf ("--------------------------------------------------\n");
  drun(data2 = makeRegularRun(tree));

  deprintf("\n **************\nRegular run done \n\n");
  drun(assert (verifyContractionTree(tree)));
  root1 = update(Q,tree);
  deprintf("done with sync\n");
  drun(assert (verifyContractionTree(tree)));
  
  data1 = ((final_cluster*)root1)->data; 
  deprintf("root1's data is = ");drun(data1.print());
  deprintf("root2's data is = ");drun(data2.print());

  
  drun(
       if(!isEqual2(data1,data2))
  {
    printf("\n CHECK HERE Seed is %d!!! \n\n", seed);
    debug=1;
     deprintf("root1's data is = ");data1.print();
     deprintf("root2's data is = ");data2.print();
    CRASH;exit(0);
  }
       );
  deprintf("\n **************\nRerun done \n\n");
  
  deprintf("done\n");
  return data1; 
}




void printTimePerOp (clock_t start, clock_t finish, int nOps) 
{
  double total = ((double) (finish - start)) * (MILLION*1.0) / CLOCKS_PER_SEC;
  printf("\nTime taken is %.5f microseconds/operation \n",  total/(double) nOps);
}

//////////////////////////////////////////////////////////////////////
// propagateAndCheck
//  
// Does and update and if NDEBUG is not set (in the debugging mode,
// it verifies the update.  It exit when incorrect and print correct
// when so. 
//////////////////////////////////////////////////////////////////////
final_data propagateAndCheck (tree_t* tree, Queue* Q, clusterList* cList)
{
  cluster *root1;
  final_data data1, data2; 

  deprintf("Rennuning\n\n");
      
  drun(assert (verifyContractionTree(tree)));
  deprintf ("--------------------------------------------------\n");
  drun(data2= makeRegularRun(tree));

  deprintf("\n **************\nRegular run done \n\n");
  drun(assert (verifyContractionTree(tree)));
  root1 = propagate(cList);
  deprintf("Done with propagate\n");
  drun(assert (verifyContractionTree(tree)));
  
  data1 = ((final_cluster *)root1)->data; 
  //debug = 1;
  deprintf("root1's data is = "); data1.print();
  deprintf("root2's data is = "); data2.print();
  //debug = 0;
  drun(
       if(!isEqual2(data1,data2))  {         
	 printf("\n CHECK HERE Seed is %d!!! \n\n", seed);	
	 exit(0);
	 }
	 else     deprintf("\nCorrect!!!\n");
       );
  
  deprintf("\n **************\nRerun done \n\n");
  
  deprintf("done\n");
  return data1; 
}

//////////////////////////////////////////////////////////////////////
// set goRandom to some positive number to do the random cut&linkHHHs. 
// there will be goRandom number of cut&links. 
// if goRandom is zero, then it will follow the trace, fName. 
//////////////////////////////////////////////////////////////////////
void testSynchronize (char* treeName, int goRandom, char* fName)
{
  cluster* root;
  clock_t start,finish; 

  int n,n1,n2; 
  int nOps; 
  
  initTest (); 
  // Initialize the queues
  Queue* Q  = initQueue();

  // Do a tree contraction
  tree_t* tree = loadtree(treeName);
  n = tree->n;
  node* vertexArray = tree->vertexArray;
  initTreeContraction (Q,tree);
  deprintf("done with loadtree\n");

  cTree = tree;
  cQueue = Q;


  root = initialrun(Q,tree);
  deprintf("done with the initial run\n");
  
  // Do goRandom number of random cut&links (same edge). 
  if (goRandom) {

    nOps = goRandom;

    start = clock (); 
    deprintf("going random, %d\n", goRandom);
     deprintf ("."); fflush (stdout); 
    
    while (goRandom > 0) {
      //debug =1;
      deprintf ("."); fflush (stdout); 
      // Randomly select edges and cut/link  
      n = tree->n; 
      n1 = (rand() % n)+1; //choose a random node;
      node* v = tree->vertexArray + n1;
      if(v->degree !=0)
	{
	  node* u = GET_NEIGHBOR(v->scars[v->left].backscar);
	  
	  // Cut and link the same age. 
	  cut (u,v,tree,Q); 
	  syncAndCheck_(tree,Q);
	  goRandom -= 1;
	  
	  link(u,v,rand()%MILLION,tree,Q); 
	

	  // Syncronize
	  syncAndCheck_(tree,Q); 
	  goRandom -= 1; 
	  
	}
    }
    finish = clock (); 
  }
  else {

    FILE *trace; 
    char str[200]; 

    trace = fopen(fName,"r"); 
    if (!trace) {printf ("Can't open trace file %s\n", fName); exit (-1);}

    start = clock();
    nOps=0;
    while((fscanf(trace,"%s\n",str) != EOF))  {
      int w; 
      deprintf ("."); 
      if(strcasecmp(str,"L") == 0) {
	fscanf(trace,"%i %i %i\n",&n1,&n2,&w);
	link(vertexArray+n1, vertexArray+n2, rand()%10, tree, Q); 
	//printf("Link %d, %d, with weight %d\n",n1,n2,w);
	syncAndCheck_(tree,Q);
	
	nOps++;
      }
      else if(strcasecmp(str,"C") == 0) {
	fscanf(trace,"%i %i\n",&n1,&n2);
	cut(vertexArray+n1,vertexArray+n2,tree,Q);
	deprintf("Cut %d, %d\n",n1,n2);
	syncAndCheck_(tree,Q);
	nOps++;
      }
    }
    finish = clock();
  }
  printTimePerOp (start,finish,nOps); 
  finishTest (); 
}


//////////////////////////////////////////////////////////////////////
// set goRandom to some positive number to do the random cut&linkHHHs. 
// there will be goRandom number of cut&links. 
// if goRandom is zero, then it will follow the trace, fName. 
//////////////////////////////////////////////////////////////////////
void testNCutsWithLC(char* treeName, int nCuts, char* fName)
{
  cluster* root;
  clock_t start,finish; 

  int n; 
  int nOps; 
  
  initTest (); 
  // Initialize the queues
  Queue* Q  = initQueue();

  // Do a tree contraction
  tree_t* tree = loadtreeAndEdges(treeName);
  n = tree->n;
  node* vertexArray = tree->vertexArray;
  initTreeContraction (Q,tree);
  deprintf("done with loadtree\n");

  cTree = tree;
  cQueue = Q;


  root = initialrun(Q,tree);
  deprintf("done with the initial run\n");
  
  if(fName)
    {
      FILE *trace; 
      char str[200]; 
      int n1,n2;
      
      trace = fopen(fName,"r"); 
      if (!trace) {printf ("Can't open trace file %s\n", fName); exit (-1);}
      
      start = clock();     
      nOps = 1;      
      while((fscanf(trace,"%s\n",str) != EOF))  {
	int w; 
	deprintf ("."); 
	if(strcasecmp(str,"L") == 0) {
	  fscanf(trace,"%i %i %i\n",&n1,&n2,&w);
	  link(vertexArray+n1, vertexArray+n2, w, tree, Q); 	  
	  rerun(Q,tree,0);	  	  
	}
	else if(strcasecmp(str,"C") == 0) {
	  fscanf(trace,"%i %i\n",&n1,&n2);
	  cut(vertexArray+n1,vertexArray+n2,tree,Q);
	  deprintf("Cut %d, %d\n",n1,n2);
	  rerun(Q,tree,0);	  
	}
      }
    }
  else 
    {
      nOps = 1;
      start = clock (); 
      int i;
      for(i = 0;i<nCuts;i++)
	{
	  node* u = vertexArray+tree->edgeArray[i].eps[0];
	  node* v = vertexArray+tree->edgeArray[i].eps[1];
	  
	  
	  // Cut and link the same age. 
	  cut (u,v,tree,Q); 
	  rerun(Q,tree,0);
	}
    }
      
  finish = clock (); 
  printTimePerOp (start,finish,nOps); 
  finishTest (); 
}


//////////////////////////////////////////////////////////////////////
// set goRandom to some positive number to do the random cut&linkHHHs. 
// there will be goRandom number of cut&links. 
// if goRandom is zero, then it will follow the trace, fName. 
//////////////////////////////////////////////////////////////////////
void testNCutsLC(char* treeName, int nCuts, char* fName)
{
  cluster* root;
  clock_t start,finish; 

  int n; 
  int nOps; 
  
  initTest (); 
  // Initialize the queues
  Queue* Q  = initQueue();

  // Do a tree contraction
  tree_t* tree = loadtreeAndEdges(treeName);
  n = tree->n;
  node* vertexArray = tree->vertexArray;
  initTreeContraction (Q,tree);
  deprintf("done with loadtree\n");

  cTree = tree;
  cQueue = Q;


  root = initialrun(Q,tree);
  deprintf("done with the initial run\n");
  
  if(fName)
    {
      FILE *trace; 
      char str[200]; 
      int n1,n2;
      
      trace = fopen(fName,"r"); 
      if (!trace) {printf ("Can't open trace file %s\n", fName); exit (-1);}
      
      start = clock();     
      nOps = 1;      
      while((fscanf(trace,"%s\n",str) != EOF))  {
	int w; 
	deprintf ("."); 
	if(strcasecmp(str,"L") == 0) {
	  fscanf(trace,"%i %i %i\n",&n1,&n2,&w);
	  link(vertexArray+n1, vertexArray+n2, w, tree, Q);
	}
	else if(strcasecmp(str,"C") == 0) {
	  fscanf(trace,"%i %i\n",&n1,&n2);
	  cut(vertexArray+n1,vertexArray+n2,tree,Q);
	  deprintf("Cut %d, %d\n",n1,n2);	 	  
	}
      }
      rerun(Q,tree,0);
    }
  else
    {
      nOps = 1;
      start = clock (); 
      int i;
      for(i = 0;i<nCuts;i++)
	{
	  node* u = vertexArray+tree->edgeArray[i].eps[0];
	  node* v = vertexArray+tree->edgeArray[i].eps[1];	  
	  // Cut and link the same age. 
	  cut (u,v,tree,Q);      
	}
      rerun(Q,tree,0);
    }
  
  finish = clock (); 
  printTimePerOp (start,finish,nOps); 
  finishTest (); 
}

//////////////////////////////////////////////////////////////////////
// set goRandom to some positive number to do the random cut&links. 
// there will be goRandom number of cut&links. 
// if goRandom is zero, then it will follow the trace, fName. 
//////////////////////////////////////////////////////////////////////
void testNCutsAndSync(char* treeName, int nCuts, char* fName)
{
  cluster* root;
  clock_t start,finish; 

  int n; 
  int nOps; 
  
  initTest (); 
  // Initialize the queues
  Queue* Q  = initQueue();

  // Do a tree contraction
  tree_t* tree = loadtreeAndEdges(treeName);
  n = tree->n;
  node* vertexArray = tree->vertexArray;
  initTreeContraction (Q,tree);
  deprintf("done with loadtree\n");

  cTree = tree;
  cQueue = Q;


  root = initialrun(Q,tree);
  deprintf("done with the initial run\n");
  if(fName)
    {
      FILE *trace; 
      char str[200]; 
      int n1,n2;
      
      trace = fopen(fName,"r"); 
      if (!trace) {printf ("Can't open trace file %s\n", fName); exit (-1);}
      
      start = clock();     
      nOps = 1;      
      while((fscanf(trace,"%s\n",str) != EOF))  {
	int w; 
	deprintf ("."); 
	if(strcasecmp(str,"L") == 0) {
	  fscanf(trace,"%i %i %i\n",&n1,&n2,&w);
	  link(vertexArray+n1, vertexArray+n2, w, tree, Q);
	}
	else if(strcasecmp(str,"C") == 0) {
	  fscanf(trace,"%i %i\n",&n1,&n2);
	  cut(vertexArray+n1,vertexArray+n2,tree,Q);
	  deprintf("Cut %d, %d\n",n1,n2);	 	  
	}
      }
      syncAndCheck_(tree,Q);
      finish = clock (); 
    }
  else 
    {
      nOps = 1;
      start = clock (); 
      int i;
      //debug =1;
      
      for(i = 0;i<nCuts;i++)
	{
	  node* u = vertexArray+tree->edgeArray[i].eps[0];
	  node* v = vertexArray+tree->edgeArray[i].eps[1];
	  
	  
	  // Cut and link the same age. 
	  cut (u,v,tree,Q);      
	  deprintf("cut %d done between %d and %d\n",i,tree->edgeArray[i].eps[0],tree->edgeArray[i].eps[1]);
	  
	}
      syncAndCheck_(tree,Q);     
      finish = clock (); 
    }
  printTimePerOp (start,finish,nOps); 
  finishTest (); 
}


//////////////////////////////////////////////////////////////////////
// set goRandom to some positive number to do the random cut&links. 
// there will be goRandom number of cut&links. 
// if goRandom is zero, then it will follow the trace, fName. 
//////////////////////////////////////////////////////////////////////
void testNCutsWithSync(char* treeName, int nCuts, char* fName)
{
  cluster* root;
  clock_t start,finish; 

  int n; 
  int nOps; 
  
  initTest (); 
  // Initialize the queues
  Queue* Q  = initQueue();

  // Do a tree contraction
  tree_t* tree = loadtreeAndEdges(treeName);
  n = tree->n;
  node* vertexArray = tree->vertexArray;
  initTreeContraction (Q,tree);
  deprintf("done with loadtree\n");

  cTree = tree;
  cQueue = Q;


  root = initialrun(Q,tree);
  deprintf("done with the initial run\n");
  
  if(fName)
    {
      FILE *trace; 
      char str[200]; 
      int n1,n2;
      
      trace = fopen(fName,"r"); 
      if (!trace) {printf ("Can't open trace file %s\n", fName); exit (-1);}
      
      start = clock();     
      nOps = 1;      
      while((fscanf(trace,"%s\n",str) != EOF))  {
	int w; 
	deprintf ("."); 
	if(strcasecmp(str,"L") == 0) {
	  fscanf(trace,"%i %i %i\n",&n1,&n2,&w);
	  link(vertexArray+n1, vertexArray+n2, w, tree, Q);
	  syncAndCheck_(tree,Q);
	}
	else if(strcasecmp(str,"C") == 0) {
	  fscanf(trace,"%i %i\n",&n1,&n2);
	  cut(vertexArray+n1,vertexArray+n2,tree,Q);
	  deprintf("Cut %d, %d\n",n1,n2);	 	  
	  syncAndCheck_(tree,Q);
	}
      }      
    }
  else 
    {
      nOps = 1;
      start = clock (); 
      int i;
      //debug =1;
      for(i = 0;i<nCuts;i++)
	{
	  node* u = vertexArray+tree->edgeArray[i].eps[0];
	  node* v = vertexArray+tree->edgeArray[i].eps[1];	  	  
	  // Cut and link the same age. 
	  cut (u,v,tree,Q);      
	  deprintf("cut %d done between %d and %d\n",i,tree->edgeArray[i].eps[0],tree->edgeArray[i].eps[1]);
	  syncAndCheck_(tree,Q);
	}
    }
  
  finish = clock (); 
  printTimePerOp (start,finish,nOps); 
  finishTest (); 
}

    

void testSynchronizeWithQuery (char* treeName, int goRandom, char* fName,void (*query)(tree_t*))
{
  cluster* root;
  clock_t start,finish; 

  int n,n1,n2; 
  int nOps; 
  
  initTest (); 
  // Initialize the queues
  Queue* Q  = initQueue();

  // Do a tree contraction
  tree_t* tree = loadtree(treeName);
  n = tree->n;
  node* vertexArray = tree->vertexArray;
  initTreeContraction (Q,tree);
  deprintf("done with loadtree\n");

  cTree = tree;
  cQueue = Q;


  root = initialrun(Q,tree);
  deprintf("done with the initial run\n");
  
  // Do goRandom number of random cut&links (same edge). 
  if (goRandom) {

    nOps = goRandom;

    start = clock (); 
    deprintf("going random, %d\n", goRandom);
    while (goRandom > 0) {
      // Randomly select edges and cut/link  
      n = tree->n; 
      n1 = (rand() % n)+1; //choose a random node;
      node* v = tree->vertexArray + n1;
      node* u = GET_NEIGHBOR(v->scars[v->left].backscar);
   
      // Cut and link the same age. 
      cut (u,v,tree,Q); 
      link(u,v,rand()%10,tree,Q); 
   
      // Syncronize
      syncAndCheck_(tree,Q); 
       
      // do a query
      (*query)(tree); 
      goRandom -= 1; 
    }
    finish = clock (); 
  }
  else {

    FILE *trace; 
    char str[200]; 

    trace = fopen(fName,"r"); 
    if (!trace) {printf ("Can't open trace file %s\n", fName); exit (-1);}

    start = clock();
    nOps=0;
    while((fscanf(trace,"%s\n",str) != EOF))  {
      int w; 
      deprintf ("."); 
      if(strcasecmp(str,"L") == 0) {
	fscanf(trace,"%i %i %i\n",&n1,&n2,&w);
	link(vertexArray+n1, vertexArray+n2, rand()%10, tree, Q); 
	//printf("Link %d, %d, with weight %d\n",n1,n2,w);
	syncAndCheck_(tree,Q);
       
        (*query)(tree); 
	nOps++;

      }
      else if(strcasecmp(str,"C") == 0) {
	fscanf(trace,"%i %i\n",&n1,&n2);
	cut(vertexArray+n1,vertexArray+n2,tree,Q);
	deprintf("Cut %d, %d\n",n1,n2);
      }
    }
    finish = clock();
  }
  printTimePerOp (start,finish,nOps); 
  finishTest (); 
}

void testSynchronizeWithQueryTrace (char* treeName, int goRandom, char* fName,void (*query)(tree_t*,FILE*))
{
  cluster* root;
  clock_t start,finish; 

  int n,n1,n2; 
  int nOps; 
  
  initTest (); 
  // Initialize the queues
  Queue* Q  = initQueue();

  // Do a tree contraction
  tree_t* tree = loadtree(treeName);
  n = tree->n;
  node* vertexArray = tree->vertexArray;
  initTreeContraction (Q,tree);
  deprintf("done with loadtree\n");

  cTree = tree;
  cQueue = Q;


  root = initialrun(Q,tree);
  deprintf("done with the initial run\n");
  
  // Do goRandom number of random cut&links (same edge). 
  if (goRandom) {

    nOps = goRandom;

    start = clock (); 
    deprintf("going random, %d\n", goRandom);
    while (goRandom > 0) {
      // Randomly select edges and cut/link  
      n = tree->n; 
      n1 = (rand() % n)+1; //choose a random node;
      node* v = tree->vertexArray + n1;
      node* u = GET_NEIGHBOR(v->scars[v->left].backscar);
   
      // Cut and link the same age. 
      cut (u,v,tree,Q); 
      if(goRandom != nOps)
	link(u,v,rand()%10,tree,Q); 
   
      // Syncronize
      syncAndCheck_(tree,Q); 
       
      // do a query
      (*query)(tree,NULL); 
      goRandom -= 1; 
    }
    finish = clock (); 
  }
  else {

    FILE *trace; 
    char str[200]; 

    trace = fopen(fName,"r"); 
    if (!trace) {printf ("Can't open trace file %s\n", fName); exit (-1);}

    start = clock();
    nOps=0;
    while((fscanf(trace,"%s\n",str) != EOF))  {
      int w; 
      deprintf ("."); 
      if(strcasecmp(str,"L") == 0) {
	fscanf(trace,"%i %i %i\n",&n1,&n2,&w);
	link(vertexArray+n1, vertexArray+n2, w, tree, Q); 
	//printf("Link %d, %d, with weight %d\n",n1,n2,w);
	syncAndCheck_(tree,Q);
       
        (*query)(tree,trace); 
	nOps++;

      }
      else if(strcasecmp(str,"C") == 0) {
	fscanf(trace,"%i %i\n",&n1,&n2);
	cut(vertexArray+n1,vertexArray+n2,tree,Q);
	deprintf("Cut %d, %d\n",n1,n2);
      }
    }
    finish = clock();
  }
  printTimePerOp (start,finish,nOps); 
  finishTest (); 
}

//////////////////////////////////////////////////////////////////////
// set goRandom to some positive number to do the random cut&links. 
// there will be goRandom number of cut&links. 
// if goRandom is zero, then it will follow the trace, fName. 
//////////////////////////////////////////////////////////////////////
void testLinkCut (char* treeName, int goRandom, char* fName)
{
  cluster* root;
  clock_t start,finish; 

  int n,n1,n2; 
  int nOps; 
  
  initTest (); 
  // Initialize the queues
  Queue* Q  = initQueue();

  // Do a tree contraction
  tree_t* tree = loadtree(treeName);
  n = tree->n;
  node* vertexArray = tree->vertexArray;
  initTreeContraction (Q,tree);
  deprintf("done with loadtree\n");

  cTree = tree;
  cQueue = Q;


  root = initialrun(Q,tree);
  deprintf("done with the initial run\n");
  
  // Do goRandom number of random cut&links (same edge). 
  if (goRandom) {

    nOps = goRandom; 

    start = clock (); 
    deprintf("going random, %d\n", goRandom);
    while (goRandom > 0) {
      // Randomly select edges and cut/link  
      n = tree->n; 
      n1 = (rand() % n)+1; //choose a random node;
      node* v = tree->vertexArray + n1;
      node* u = GET_NEIGHBOR(v->scars[v->left].backscar);
   
      // Cut and link the same age. 
      cut (u,v,tree,Q); 
      rerun(Q,tree,0);
      goRandom -= 1;

      link(u,v,rand()%10,tree,Q); 

      // propagate
      rerun(Q,tree,0);  //no synch

      goRandom -= 1; 
    }
    finish = clock (); 
  }
  else {

    FILE *trace; 
    char str[200]; 

    trace = fopen(fName,"r"); 
    if (!trace) {printf ("Can't open trace file %s\n", fName); exit (-1);}

    start = clock();
    nOps=0;
    while((fscanf(trace,"%s\n",str) != EOF))  {
      int w; 
      deprintf ("."); 
      if(strcasecmp(str,"L") == 0) {
	fscanf(trace,"%i %i %i\n",&n1,&n2,&w);
	link(vertexArray+n1, vertexArray+n2, rand()%MILLION, tree, Q); 
	// propagate
	rerun (Q,tree,0); //no synch
	nOps++;
      }
      else if(strcasecmp(str,"C") == 0) {
	fscanf(trace,"%i %i\n",&n1,&n2);
	cut(vertexArray+n1,vertexArray+n2,tree,Q);
	deprintf("Cut %d, %d\n",n1,n2);
      }
    }
    finish = clock();
  }
  printTimePerOp (start,finish,nOps); 
  finishTest (); 
}

void testLinkCutChain (char* treeName, int goRandom)
{
  cluster* root;
  clock_t start,finish; 

  int n,n1; 
  int nOps; 
  node *endp1, *endp2; 

  
  initTest (); 
  // Initialize the queues
  Queue* Q  = initQueue();

  // Do a tree contraction
  tree_t* tree = loadtree(treeName);
  n = tree->n;
  node* vertexArray = tree->vertexArray;

  cTree = tree;
  cQueue = Q;


  // Initial end points are always 1 and 2. 
  endp1 = vertexArray+1; endp2 = vertexArray+2; 

  initTreeContraction (Q,tree);
  deprintf("done with loadtree\n");

  root = initialrun(Q,tree);
  deprintf("done with the initial run\n");
  


  // Do goRandom number of random cut&links (same edge). 
  if (goRandom) {

    nOps = goRandom; 

    start = clock (); 
    deprintf("going random, %d\n", goRandom);

    while (--goRandom) {
      // Randomly select edges and cut/link  
      n = tree->n; 
      n1 = (rand() % n)+1; //choose a random node;
      node* v = tree->vertexArray + n1;
      node* u = GET_NEIGHBOR(v->scars[v->left].backscar);
   
      // Cut and link the same age. 
      cut (u,v,tree,Q); 
      link (endp1,endp2,rand()%MILLION,tree,Q); 

      // propagate
      rerun(Q,tree,0); 

      endp1 = u; 
      endp2 = v;
    }
    finish = clock (); 
  }

  printTimePerOp (start,finish,nOps); 
  finishTest (); 
}

void testLinkCutMSTChain (char* treeName, int goRandom,void (*query)(tree_t*))
{
  cluster* root;
  clock_t start,finish; 

  int n,n1; 
  int nOps; 
  node *endp1, *endp2; 

  
  initTest (); 
  // Initialize the queues
  Queue* Q  = initQueue();

  // Do a tree contraction
  tree_t* tree = loadtree(treeName);
  n = tree->n;
  node* vertexArray = tree->vertexArray;
  
  cTree = tree;
  cQueue = Q;


  // Initial end points are always 1 and 2. 
  endp1 = vertexArray+1; endp2 = vertexArray+2; 

  initTreeContraction (Q,tree);
  deprintf("done with loadtree\n");

  root = initialrun(Q,tree);
  deprintf("done with the initial run\n");
  


  // Do goRandom number of random cut&links (same edge). 
  if (goRandom) {

    nOps = goRandom; 

    start = clock (); 
    deprintf("going random, %d\n", goRandom);

    while (--goRandom) {
      // Randomly select edges and cut/link  
      n = tree->n; 
      n1 = (rand() % n)+1; //choose a random node;
      node* v = tree->vertexArray + n1;
      node* u = GET_NEIGHBOR(v->scars[v->left].backscar);
   
      // Cut and link the same age. 
      cut (u,v,tree,Q); 
      link (endp1,endp2,rand()%MILLION,tree,Q); 

      syncAndCheck_(tree,Q); 
      (*query)(tree);

      endp1 = u; 
      endp2 = v;
    }
    finish = clock (); 
  }

  printTimePerOp (start,finish,nOps); 
  finishTest (); 
}

void testLinkCutSyncChain (char* treeName, int goRandom)
{
  cluster* root;
  clock_t start,finish; 

  int n,n1; 
  int nOps; 
  node *endp1, *endp2; 

  
  initTest (); 
  // Initialize the queues
  Queue* Q  = initQueue();

  // Do a tree contraction
  tree_t* tree = loadtree(treeName);
  n = tree->n;
  node* vertexArray = tree->vertexArray;

  cTree = tree;
  cQueue = Q;


  // Initial end points are always 1 and 2. 
  endp1 = vertexArray+1; endp2 = vertexArray+2; 

  initTreeContraction (Q,tree);
  deprintf("done with loadtree\n");

  root = initialrun(Q,tree);
  deprintf("done with the initial run\n");
  
  // Do goRandom number of random cut&links (same edge). 
  if (goRandom) {

    nOps = goRandom; 

    start = clock (); 
    deprintf("going random, %d\n", goRandom);

    while (--goRandom) {
      // Randomly select edges and cut/link  
      n = tree->n; 
      n1 = (rand() % n)+1; //choose a random node;
      node* v = tree->vertexArray + n1;
      node* u = GET_NEIGHBOR(v->scars[v->left].backscar);
   
      // Cut and link the same age. 
      cut (u,v,tree,Q); 
      link (endp1,endp2,rand()%MILLION,tree,Q); 

      syncAndCheck_(tree,Q); 

      endp1 = u; 
      endp2 = v;
    }
    finish = clock (); 
  }

  printTimePerOp (start,finish,nOps); 
  finishTest (); 
}


//////////////////////////////////////////////////////////////////////
// test the edge data and do random update m times
//////////////////////////////////////////////////////////////////////
void testEdgeData(char* treeFile, int rounds, void (*updateWeight)(bin_data*))
{
  final_data edge; 
  node *v, *u;

  initTest (); 

  Queue* Q  = initQueue();

  clusterList changedClusters;
  //changeClusters.head = NULL;

  deprintf("done with mem_init\n");

  deprintf ("Loading the tree\n");  
  tree_t* tree = loadtree(treeFile);
  deprintf ("Loaded the tree\n"); 

  cTree = tree;
  cQueue = Q;


  initTreeContraction (Q,tree);
  deprintf ("Done with inittreecontract\n"); 

  deprintf("done with loadtree\n");
   
  cluster* root = initialrun(Q,tree);
 
 
  edge = ((final_cluster*)root)->data;
  int n = tree->n;

  

  totalqueued=0;
  clock_t start = clock();
  int i = 0; 
  while(i++ < rounds) {
  
   
    int n1 = (rand() % n)+1; //choose a random node;
    v = tree->vertexArray + n1;
    u = GET_NEIGHBOR(v->scars[v->left].backscar);
  
    changeEdgeData(v,u,Q,&changedClusters,updateWeight);
    edge = propagateAndCheck(tree,Q,&changedClusters);
  }
  clock_t finish = clock();
  finishTest (); 

  printTimePerOp (start,finish,rounds); 

}



void testVertexData(char* treeFile, int rounds, void (*updateWeight)(unary_data*))
{
  final_data edge; 
  node *v;

  initTest (); 
  Queue* Q  = initQueue();
 
  clusterList changedClusters;

  deprintf("done with mem_init\n");

  deprintf ("Loading the tree\n");  
  tree_t* tree = loadtree(treeFile);
  deprintf ("Loaded the tree\n"); 

  cTree = tree;
  cQueue = Q;


  initTreeContraction (Q,tree);
  deprintf ("Done with inittreecontract\n"); 

  deprintf("done with loadtree\n");
   
  cluster* root = initialrun(Q,tree);
 
 
  edge = ((final_cluster*)root)->data;
  int n = tree->n;

  
  totalqueued=0;
  clock_t start = clock();
  int i = 0; 
  while(i++ < rounds) {
    int n1 = (rand() % n)+1; //choose a random node;
    v = tree->vertexArray + n1;
  
    changeVertexData(v,Q,&changedClusters,updateWeight);
    propagateAndCheck(tree,Q,&changedClusters);
  }
  clock_t finish = clock();
  printTimePerOp (start,finish,rounds); 
  finishTest (); 
}

void testVertexDataQuery(char* treeFile, int rounds, int opsPerRound, void (*changeMark)(unary_data*), unary_data (*readVertexData)(FILE*))
{
  unary_data edge; 
  node *v;

  initTest (); 
  Queue* Q  = initQueue();
  clusterList changedClusters;

  deprintf("done with mem_init\n");

  deprintf ("Loading the tree\n");  
  tree_t* tree = loadtree_param_vertex(treeFile,readVertexData);
  deprintf ("Loaded the tree\n"); 

  cTree = tree;
  cQueue = Q;
  

  initTreeContraction (Q,tree);
  deprintf ("Done with inittreecontract\n"); 

  deprintf("done with loadtree\n");
  
  cluster* root = initialrun(Q,tree);
 
  
  edge = ((unary_cluster*)root)->data;
  int n = tree->n;

  int nd = (rand() % n)+1; //choose a random node;
  v = tree->vertexArray + nd; 
  changeVertexData(v,Q,&changedClusters,changeMark);


  propagateAndCheck(tree,Q,&changedClusters);
  totalqueued=0;
  clock_t start = clock();
  int i = 0; 
  while(i++ < rounds) {
    changeVertexData(v,Q,&changedClusters,changeMark); //unmark old node
    int n1 = (rand() % n)+1; //choose a random node;
    v = tree->vertexArray + n1;
   
    changeVertexData(v,Q,&changedClusters,changeMark);
    propagateAndCheck(tree,Q,&changedClusters);
  }
  clock_t finish = clock();
  rounds = rounds*opsPerRound;
  printTimePerOp (start,finish,rounds); 
  finishTest (); 
}


////////////////////////////////////////////////////////////////////
// test the function query by running it rounds number of times
// on the tree defined by treeFile
////////////////////////////////////////////////////////////////////
void testQuery(char* treeFile, int rounds, void (*query)(tree_t*))
{
  unary_data edge; 

  initTest (); 
  Queue* Q  = initQueue();

  deprintf("done with mem_init\n");

  deprintf ("Loading the tree\n");  
  tree_t* tree = loadtree(treeFile);
  
  cTree = tree;
  cQueue = Q;

  deprintf ("Loaded the tree\n"); 


  initTreeContraction (Q,tree);
  deprintf ("Done with inittreecontract\n"); 

  deprintf("done with loadtree\n");
   
  cluster* root = initialrun(Q,tree);
 
  edge = ((unary_cluster*)root)->data;
  

  totalqueued=0;
  clock_t start = clock();
  int i = 0; 
  while(i++ < rounds) {
    (*query)(tree);
  }
  clock_t finish = clock();
  printTimePerOp (start,finish,rounds); 
  finishTest (); 
}

/////////////////////////////////////////////////////////////////////////
// This is to test query functions that take the root of support trees
/////////////////////////////////////////////////////////////////////////
void testQuery_root (char* treeFile, int rounds, void (*query)(cluster *))
{
  initTest (); 
  Queue* Q  = initQueue();

  deprintf("done with mem_init\n");

  deprintf ("Loading the tree\n");  
  tree_t* tree = loadtree(treeFile);
  deprintf ("Loaded the tree\n"); 
  
  cTree = tree;
  cQueue = Q;

  
  initTreeContraction (Q,tree);
  deprintf ("Done with inittreecontract\n"); 

  deprintf("done with loadtree\n");
   
  cluster* root = initialrun(Q,tree);
 
  totalqueued=0;
  clock_t start = clock();
  int i = 0; 
  while(i++ < rounds) {
    (*query)(root);
  }
  clock_t finish = clock();
  printTimePerOp (start,finish,rounds); 
  finishTest (); 
}

