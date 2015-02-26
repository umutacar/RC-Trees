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

/////////////////////////////////////////////////////////////
// Data.c
//
// Jorge L. Vittes
//
// This code is for defining the data operations for finding
// the closest marked vertex to a particular vertex
////////////////////////////////////////////////////////////

#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include "Data.h"
#include "AMath.h"

/////////////////////////////////////////////////
// constructor for binary data given edge
// endpoints u, v and weight w
/////////////////////////////////////////////////
bin_data::bin_data(int u, int v, double w)
{
  closest1 = INFINITY;
  closest2 = INFINITY;
  len = w;
}
 
////////////////////////////////////////////////
// defult constructor for binary data
////////////////////////////////////////////////
bin_data::bin_data()
{
  closest1 = INFINITY;
  closest2 = INFINITY;
  len = INFINITY;
}

/////////////////////////////////////////
// reset function for binary data
////////////////////////////////////////
void bin_data::reset()
{
  closest1 = INFINITY;
  closest2 = INFINITY;
  len = INFINITY;
}

/////////////////////////////////////////
// constructor for unary data using the
// input file data
////////////////////////////////////////
unary_data::unary_data(FILE* file)
{
  int val,mark;
  fscanf(file,"%i %i",&val,&mark);
  if(mark == 1) {
    closest = 0.0;   
  }
  else {
    closest = INFINITY;    
  }
}

/////////////////////////////////////////
// Default constructor for unary data
////////////////////////////////////////
unary_data::unary_data()
{
  closest = INFINITY; 
}

////////////////////////////////////////
// Reset function for unary data
////////////////////////////////////////
void unary_data::reset()
{
  closest = INFINITY;
}

/////////////////////////////////////////
// default constructor for final data
/////////////////////////////////////////
final_data::final_data()
{  
}

/////////////////////////////////////////
// Reset function for final data
/////////////////////////////////////////
void final_data::reset()
{
}

////////////////////////////////////////////
// get the distance for the closest marked
// vertex to the non-vanishing vertex
////////////////////////////////////////////
double getFarClosest(bin_data* bd)
{
  if(GET_OFF(bd)) {
    bd = GET_DT(bd);
    return bd->closest1;
  }
  return bd->closest2;
}

////////////////////////////////////////////
// get the distance for the closest marked
// vertex to the vanishing vertex
////////////////////////////////////////////
double getNearClosest(bin_data* bd)
{
  if(GET_OFF(bd)) {
    bd = GET_DT(bd);
    return bd->closest2;
  }
  return bd->closest1;
}

///////////////////////////////////////////////
// get the length from an impure binary data
///////////////////////////////////////////////
double getLen(bin_data* bd)
{
  bd = GET_DT(bd);
  return bd->len;
}

////////////////////////////////////////////////////////////////////////////////////
// combine the two unary datas clA, and clB and place that into dest
////////////////////////////////////////////////////////////////////////////////////
void addData(unary_data* clA, unary_data* clB, unary_data* dest)
{
  
  deprintf("\nAddCluster\n");
  deprintf("clA->closest is %lf, clB->closest is %lf\n",clA->closest, clB->closest);
  dest->closest = dmin(clA->closest, clB->closest);
 
}

////////////////////////////////////////////////////////////////////////////////////
// using the collected information for the unary clusters, and the data from the 
// binary clusters combine it, and place it in dest for when a contract occured
////////////////////////////////////////////////////////////////////////////////////
void dataContract(bin_data* left,bin_data* right, unary_data* sumCl, bin_data* dest)
{
  double lfar  = getFarClosest(left);
  double lnear = getNearClosest(left);
  
  double rfar  = getFarClosest(right);
  double rnear = getNearClosest(right);
  
  double llen = getLen(left);
  double rlen = getLen(right);

  dest->closest1 = min3(lfar,llen + rnear, llen + sumCl->closest);
  dest->closest2 = min3(rfar,rlen + lnear, rlen + sumCl->closest);
  dest->len = rlen + llen;
}

////////////////////////////////////////////////////////////////////////////////////
// using the collected information fro the unary clusters, and the data from the
// binary edge cluster combine it, and place it in dest for when a rake occurered
////////////////////////////////////////////////////////////////////////////////////
void rakeIn(bin_data* edgecl,unary_data* sumcl,unary_data* dest)
{
  double closest = getNearClosest(edgecl);
  double len = getLen(edgecl);
  dest->closest = dmin(closest,len + sumcl->closest);
 
}


/////////////////////////////////////////////////////////////////
// FinalizeData copies important information from the collected
// unary data on cl to the final data at dest
/////////////////////////////////////////////////////////////////
void finalizeData(unary_data* cl, final_data* dest)
{
  
}

///////////////////////////////////////////////////////////
// Return true if data_t a, and data_t b are the same as
// far as the algorithm is concerned
///////////////////////////////////////////////////////////
int isEqual2(final_data a, final_data b)
{
  return 1;
} 


/* 
<MUST HAVE>
*/

////////////////////////////////////////////
// make binary edge data from the actual
// edge enpoints and weight
////////////////////////////////////////////
bin_data makeEdgeData(int n1, int n2, int w)
{
  bin_data ret(n1,n2,(double) w);
  return ret;
}


/////////////////////////////////////////////////
// Read vertex data from a file
/////////////////////////////////////////////////
unary_data readVertexData(FILE* file)
{
  unary_data res(file);
  return res;
}

/////////////////////////////////////////////////////
//  Push down function is empty
/////////////////////////////////////////////////////
void pushDownData(bin_data* parent, bin_data* child) 
{
}

///////////////////////////////////////////////////
// Read Edge data from a file
//////////////////////////////////////////////////
bin_data readEdgeData (FILE* file,int v1, int v2)
{
  int w;
  fscanf(file,"%i",&w);
  return makeEdgeData(v1,v2,w);
}

//////////////////////////////////////////////////
// mark a vertex
//////////////////////////////////////////////////
void markNode(unary_data* dat)
{
  dat->closest = 0.0; 
}

/////////////////////////////////////
// unmark a vertex
////////////////////////////////////
void unMarkNode(unary_data* dat)
{
  dat->closest = INFINITY;
}

////////////////////////////////////////
// unmark a vertex if it's marked, mark
// it otherwise
////////////////////////////////////////
void markAndUnMark(unary_data* dat)
{
  if(dat->closest == 0.0)
    unMarkNode(dat);
  else
    markNode(dat);
}

/////////////////////////////////////////
// Update the weight on an edge 
/////////////////////////////////////////
void updateDataWeight(bin_data* bd)
{
  int w = rand()%MILLION;

  bd->closest1 = INFINITY;
  bd->closest2 = INFINITY;
  bd->len = (double) w;
}
