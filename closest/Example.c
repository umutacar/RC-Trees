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

///////////////////////////////////////////////////////////////
// Example.c
//
// Jorge L. Vittes
//
// Code for testing dynamic parallel tree contraction
// Code for finding the closest makred proint of a tree using 
// support trees.
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include "Interface.h"
#include "Contract.h"
#include "Application.h"

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

  for(i=5;i<=99;i+=5)
    {
      F.changeVertexData(F.vertex(i),&markNode);
      printf("Marking %d\n",i);
    }

  F.propagate();
  //find the center
 
  double clos = findClosest(F.vertex(48));
  
  printf("The closest marked node to 48 is at distance %lf\n",clos);
  //now unlink 49-50 and link 1-100
  F.cut(F.vertex(49),F.vertex(50));
  F.link(F.vertex(1),F.vertex(100),1);
  F.contract();

  clos = findClosest(F.vertex(48));
  printf("The closest marked node to 48 is at distance %lf\n",clos);
  
}

