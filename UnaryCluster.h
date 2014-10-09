/***************************************************************************
 * UnaryCluster.h
 *
 * Jorge Vittes
 *
 * description of, and functions for unary clusters
 ************************************************************************/

#ifndef _UNCLUSTER_H_
#define _UNCLUSTER_H_ 1
 
#include <math.h>
#include <malloc.h>
#include <assert.h>
#include "FreeList.h"
#include "Data.h"
#include "BaseCluster.h"
#include "BinCluster.h"

class unary_cluster: public cluster {
 public:

  unary_data data;
  class bin_cluster* binaryCl[1]; //binary clusters
  unary_cluster* unaryCl[MAX_DEGREE-1];// unary Cluster  
  int endp1; 

  virtual int isEndPoint(int ep)
    {return (ep == endp1);}

  unary_cluster(unary_data dat);  
  unary_cluster();

  virtual void resetData() {
    data.reset();}

  virtual void reset();

  virtual void synchronize();

  virtual int getNonVanishing();     // The id of the non-vanishing vertex
  virtual int getVanishing();       // The id of the vanishing vertex

  virtual cluster** getSiblings(); // The "other" binary clusters that participates in this join. 
  virtual cluster** getClusters();
  virtual cluster** getUnaryClusters();
  virtual cluster** getBinaryClusters();

};
#endif
