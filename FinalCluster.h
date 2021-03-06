//////////////////////////////////////////////////////////////////////////
// FinalCluster.h
//
// Jorge Vittes
//
// description of, and functions for final clusters
//////////////////////////////////////////////////////////////////////////

#ifndef _FINCLUSTER_H_
#define _FINCLUSTER_H_ 1
 
#include <math.h>
#include <malloc.h>
#include <assert.h>
#include "FreeList.h"
#include "Data.h"
#include "BaseCluster.h"
#include "BinCluster.h"
#include "UnaryCluster.h"

class final_cluster: public cluster {
 public:

  final_data data;
  unary_cluster* unaryCl[MAX_DEGREE];// unary Cluster  

  virtual int isEndPoint(int ep)  {
    printf ("FATAL ERROR: This is a final cluster and thus has no end points\n"); 
    assert (0);
  }

  final_cluster(final_data dat);  
  final_cluster();

  virtual void resetData() {
    data.reset();}

  virtual void reset();

  virtual void synchronize();

  virtual int getNonVanishing();     // The id of the non-vanishing vertex
  virtual int getVanishing();       // The id of the vanishing vertex
  virtual cluster** getSiblings();  // The "other" binary clusters that participates in this join. 
  virtual cluster** getClusters();
  virtual cluster** getUnaryClusters();
  virtual cluster** getBinaryClusters();

};

#endif
