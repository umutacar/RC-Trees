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

/////////////////////////////////////////////////////////////////////////////
// BaseCluster.h
//
// Jorge Vittes                  
//
// Basic structure for clusters
////////////////////////////////////////////////////////////////////////////

#ifndef _BASECLUSTER_H_
#define _BASECLUSTER_H_ 1
 
#include <math.h>
#include <malloc.h>
#include "FreeList.h"
#include "Data.h"
#include "GetMacros.h"

#define addCluster(a,b,c) addData(&((a)->data),&((b)->data),&((c)->data))

class cluster{
 public:
  int id;
  cluster* parent;
  cluster* child;
  char endpoints;
  char affected;
  char delOp;
  int height;

  virtual int isEndPoint(int ep)=0;
  virtual void synchronize()=0;

  virtual void resetData()=0;
  virtual void reset()=0;

  int nSiblings () {return (parent->endpoints - endpoints + 1);};   // The number of my siblings

  virtual int getNonVanishing()=0;       // The id of the non-vanishing vertex
  virtual int getVanishing()=0; 
  virtual cluster** getSiblings()=0;     // The "other" binarys cluster that participates in this join. 
  virtual cluster** getClusters()=0;
  virtual cluster** getUnaryClusters()=0;
  virtual cluster** getBinaryClusters()=0;
};

#endif
