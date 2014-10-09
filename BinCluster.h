/////////////////////////////////////////////////////////////////////////////
// BinCluster.h
// 
// Jorge Vittes
//
// The definition of binary clusters
/////////////////////////////////////////////////////////////////////////////
#ifndef _BINCLUSTER_H_
#define _BINCLUSTER_H_ 1
 

#include <math.h>
#include <malloc.h>
#include "FreeList.h"
#include "Data.h"
#include "BaseCluster.h"
#include "UnaryCluster.h"

class bin_cluster: public cluster {
 public:
  
  bin_data data;
  bin_cluster* binaryCl[2]; //binary clusters
  class unary_cluster* unaryCl[MAX_DEGREE-2];// unary Cluster
  int endp1;
  int endp2;

  bin_cluster(bin_data dat);
  bin_cluster();
 
  virtual int isEndPoint(int ep) {
    if (ep == endp1 | ep == endp2) return 1; 
    else return 0;}

  virtual void resetData() {data.reset();}
  void updateWeight();

  virtual void reset();

  virtual void synchronize();
  
  virtual int getNonVanishing();   // The id of the vanishing vertex
  virtual int getVanishing();      // The id of the vanishing vertex
  virtual cluster** getSiblings(); // The "other" binary clusters that participates in this join. 
  virtual cluster** getClusters();
  virtual cluster** getUnaryClusters();
  virtual cluster** getBinaryClusters();
  
};

// The pointer bc is not a pure pointer.  
bin_data* getBinData(bin_cluster* bc);



#endif
 
