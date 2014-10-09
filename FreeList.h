//////////////////////////////////////////////////////////////////////
// FreeList.h
//
// Jorge L. Vittes and Umut A. Acar
// Description: Interface for a free list of fixed size blocks
//////////////////////////////////////////////////////////////////////

#ifndef _FREE_LIST_H_
#define _FREE_LIST_H_ 1
#include "Globals.h"

typedef struct {
  int nAlloc;    // number of allocated  nodes
  int size;      // block size in bytes 

  char* head;      // head of the free list
  char* last_malloc;  // the address of the last malloc
  char* pages;     // list of all allocated pages
} FreeList;

FreeList* initFreeList(int bsize) ;
FreeList* initPreAllocatedFreeList(int nblocks, int bsize) ;

void* allocBlock(FreeList *flist);

void freeBlock (FreeList *flist, char* block);
void destructFreeList (FreeList *flist); 

void dumpFreeList (FreeList *flist);
void printMemUsage(FreeList *flist);


#endif
