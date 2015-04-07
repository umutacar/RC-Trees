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
