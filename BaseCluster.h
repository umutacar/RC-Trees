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

// Purify the cluster pointer pointer by setting the low bits of the cluster pointer to zero.
#define PURIFY(c) (*c = (cluster*) (((unsigned int) (*c)) & 0xFFFFFFFC))

#define GET_CL(c) (cluster *)((int) c & 0xFFFFFFFC)
#define GET_BC(c) (bin_cluster *)((int) c & 0xFFFFFFFC)
#define GET_OFF(c) (int) c & 1
#define GET_CLUSTER_DATA(cl) ((GET_BC(cl))->data[GET_IN(cl)])
#define GET_OTHER_DATA(cl) ((GET_CL(cl))->data[GET_OT(cl)])
#define BIN_DATA(bc) (bin_data*)((((int) &(GET_BC(bc))->data))| (GET_OFF(bc))) 
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
