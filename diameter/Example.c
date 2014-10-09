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

////////////////////////////////////////////////////////////////
// Example.c
//
// Jorge L. Vittes
//
// Code for testing dynamic parallel tree contraction
// Code for finding the diamater of a tree using support trees. 
////////////////////////////////////////////////////////////////

#include "Application.h"
#include "Contract.h"
#include "Interface.h"

#define KILO 1000
#define MILLON 1000000


int main()
{
  RC_Forest F(100);
  
  //now just link consecutive vertices in a chain
  int i;
  for(i=1;i<=99;i++)
    {
      F.link(F.vertex(i),F.vertex(i+1),1);
      printf("linking %d to %d\n",i,i+1);
    }
  //link them in batch
  //debug=1;
  F.contract();

  //get the root of the support tree
 

 
  //find the diameter
  double diam = getDiameter(F.vertex(10));
  printf("The diameter of the tree is %lf\n",diam);
  //now unlink 49-50 and link 1-100
  F.cut(F.vertex(49),F.vertex(50));
 
  F.contract();


  diam = getDiameter(F.vertex(10));
  printf("The diameter of the tree is %lf\n",diam);
 
}



