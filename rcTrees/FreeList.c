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

//////////////////////////////////////////////////////////////////////
// FreeList.c
//
// Jorge L. Vittes and Umut A. Acar
// Description: A free list of fixed size blocks
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <assert.h>
#include <stdint.h>
#include "FreeList.h"

#define BUFFER 16
#define KILO 1024
#define PAGE_SIZE (8*KILO-BUFFER)

int nMalloc = 0;

/////////////////////////////////////////////////////////////////////
// Allocate a page or more from malloc and split it into blocks 
// aligned to the blocksize 
/////////////////////////////////////////////////////////////////////
void allocPage (FreeList* flist) 
{ 
  char **ptr, **endptr; 
  int bsize = flist->size; 
  int offset;

  deprintf("Allocating page \n");
  ptr = (char **) calloc (PAGE_SIZE,sizeof(char)); 
  int nBlocks = PAGE_SIZE/bsize; 
  endptr = (char **) (((char*) ptr) + (nBlocks-2)*bsize);

  if(!ptr) {printf("OUT OF MEMORY"); exit(-3);}
  nMalloc++; 

  // The first word is a pointer to the previously allocated page.
  *ptr = flist->pages; 
  flist->pages = (char *)ptr; 

  ptr = (char **) ((char*) ptr + bsize);
  offset = (int) ((intptr_t)ptr % bsize);
  if(offset > bsize-4)
    offset -= bsize;
  ptr = (char **) (((char*) ptr) - offset);
 
  // Add the remaining blocks to the free list. 
  flist->head = (char*) ptr; 

  char* next;
  while (ptr < endptr) {
    next = (char*) ptr + bsize;
    *ptr = next;
    ptr = (char **) next;
  }
  *ptr = NULL;
}

///////////////////////////////////////////////////////////////////
// Allocate a enough space to make nBlocks number of blocks and 
// partition it into nBlocks blocks aligned to the size 
///////////////////////////////////////////////////////////////////
void allocPageN (FreeList* flist, int nBlocks) 
{ 
  char **ptr; 
  int i, offset, bsize = flist->size; 
  
  ptr = (char **) calloc (bsize,nBlocks+2); 
  if(!ptr) {printf("OUT OF MEMORY"); exit(-3);}
  nMalloc++; 

  // The first word is a pointer to the previously allocated page.
  *ptr = flist->pages; 
  flist->pages = (char *)ptr; 
  ptr = (char **) ((char*) ptr + bsize);
  
  offset = (int) ((intptr_t)ptr % bsize);
  if(offset > bsize-4)
    offset -= bsize;
  ptr = (char **) (((char*) ptr) - offset);
 


  // Add the remaining blocks to the free list. 
  flist->head = (char*) ptr; 

  for(i = 0 ; i < nBlocks-1 ; i++) {
    char* next = (char*) ptr + bsize;
    *ptr = next;
    ptr = (char **) next;
  }
  *ptr = NULL;
}

///////////////////////////////////////////////////////////////////
// Create a freeList
///////////////////////////////////////////////////////////////////
FreeList* initFreeList(int bsize) {
  if (bsize < (int)sizeof(int*)) {
    printf ("Block size must be no less than 4\n"); exit (-9);
  }

  FreeList *flist = (FreeList*) calloc (1,sizeof (FreeList)); 
  flist->size = bsize; 

  // Allocate the first page. 
  allocPage (flist);

  return flist;
}

/////////////////////////////////////////////////////////////////
// Create a freeList with nBlocks already allocated in order
/////////////////////////////////////////////////////////////////
FreeList* initPreAllocatedFreeList(int nblocks, int bsize) {

  if (bsize < (int)sizeof(int*)) {
    printf ("Block size must be no less than 4\n"); exit (-9);
  }

  FreeList *flist = (FreeList*) calloc (1,sizeof (FreeList)); 
  flist->size = bsize; 

  // Allocate the first page. 
  allocPageN (flist,nblocks);

  return flist;
}

////////////////////////////////////////////////////////////////
// Allocate a Node from the stack 
////////////////////////////////////////////////////////////////
void* allocBlock(FreeList *flist)
{
  void* node = flist->head; 

  deprintf("allocBlock %p\n",flist);
  deprintf("node is %p\n",node);
  flist->head = (char *) (*((intptr_t *)flist->head));
  deprintf("flist->head is %p\n",flist->head);

  if (!(flist->head)) {
    allocPage (flist); 
    drun(flist->last_malloc = flist->head); 
  }

  drun(++(flist->nAlloc));
  return node; 
}

////////////////////////////////////////////////////////////////
// Free the block by adding it back to the stack 
////////////////////////////////////////////////////////////////
void freeBlock (FreeList *flist, char* block)
{
  char **ptr = (char**) block; 
  *ptr  = flist->head;
  flist->head = block;

  deprintf("free block %p\n",flist);
  deprintf("flist->head is %p\n",flist->head);
  drun(--(flist->nAlloc));
}

////////////////////////////////////////////////////////////////
// Free all the pages allocated by the freelist 
////////////////////////////////////////////////////////////////
void destructFreeList (FreeList *flist) 
{
  char** tmp;
  char* page = (char*) flist->pages; 
  
  while (page) {
    tmp = (char**) page; 
    page = *tmp; 
    free(tmp); 
    nMalloc--;
  }
  free(flist);
}

////////////////////////////////////////////////////////////////
// print memory usage 
////////////////////////////////////////////////////////////////
void printMemUsage(FreeList *flist)
{
  printf("Memory used is %d \n",nMalloc*PAGE_SIZE);
  printf("Number of nodes alloced %d \n",flist->nAlloc);
}

////////////////////////////////////////////////////////////////
// print out the information for the freelist
////////////////////////////////////////////////////////////////
void dumpFreeList (FreeList *flist) 
{
  if (flist) {
    printf ("dumpFreeList: head = %p\n", flist->head); 
    printf ("dumpFreeList: last_malloc = %p\n", flist->last_malloc); 
  }
  else 
    printf ("dumpFreeList: empty\n"); 
}

