/*****************************************************************************
 * Globals.h
 *
 * Umut Acar, Jorge Vittes
 *
 * Global macros for use in the program
 ****************************************************************************/
#ifndef _GLOBALS_H_
#define _GLOBALS_H_ 1

#define MAX_DEGREE 4
#define VALID 1
#define INVALID 0
#define NOWHERE -1

#define RUN 0
#define COPY 1
#define GARBAGE -7777777

#define RAKE_EVENT 1
#define COMPRESS_EVENT 2
#define END_EVENT 3

#define QUEUED 1
#define NOT_QUEUED (-1)

#define IS_AFFECTED 1
#define NOT_AFFECTED (-1)
#define DELETED 3



#define ALLOC_NODE() (node *) allocBlock(currentTree->nodeList)
#define FREE_NODE(nd)  freeBlock(currentTree->nodeList,(char*) nd)

#define GET_CL(c) (cluster *)((int) c & 0xFFFFFFFC)
#define ND_CL(nd) (currentTree->clusterList[nd->nId])

//int* crash; 
//#define CRASH crash = NULL; *0 = 02;
#define CRASH *((int*) 0) = 02;

#define MILLION 1000000
#define CL_SIZE max(sizeof(bin_cluster),sizeof(unary_cluster))

extern int debug; 

#ifndef NDEBUG
  #define drun(args...) args;
#else
  #define drun(args...) ;
#endif

#ifndef NVERBOSE 
  #define deprintf(args...) if(debug) {printf(args);fflush(stdout);}
#else
  #define deprintf(args...) ;
#endif

#ifndef NPUSHDOWN
  #define PUSHDOWN 1
#else
  #define PUSHDOWN 0
#endif


#endif

