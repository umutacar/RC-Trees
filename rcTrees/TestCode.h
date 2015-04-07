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


#include "Interface.h"

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
