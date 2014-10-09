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

///////////////////////////////////////////////////////////////////////
// Example.c
//
// Jorge L. Vittes
//
// Code for testing dynamic parallel tree contraction
// Code for finding the center of a tree using support trees. 
///////////////////////////////////////////////////////////////////////

#include <stdlib.h>

#include "Application.h"
#include "Interface.h"
#include "Tree.h"
#include "Contract.h"

#define KILO 1000
#define MILLON 1000000



int main()
{
  RC_Forest F(100);

  //now just link consecutive vertices in a chain
  int i;
  for(i=1;i<=99;i++)
    {
      if(i == 50)
	{
	  F.link(F.vertex(i),F.vertex(i+1),2);
	}
      else
	F.link(F.vertex(i),F.vertex(i+1),1);
      printf("linking %d to %d\n",i,i+1);
    }
  //link them in batch
  //debrootg=1;
  F.contract();

  node* root = F.vertex(20);
  node* v = F.vertex(80);

  //find the center
  data_t dat = largestEdge(root,v);
  printf("The largest edge in the subtree 80 of the tree rooted at 20 is ");
  printf("weight = %lf, n1 = %d , n2 = %d\n",dat.weight,dat.n1,dat.n2);
  //now unlink 49-50 and link 1-100
  F.cut(F.vertex(49),F.vertex(50));
  F.link(F.vertex(1),F.vertex(100),4);
  printf(" unlink 49-50 and link 1-100\n");
  F.contract();

 
  dat = largestEdge(root,v);

  printf("The largest edge in the subtree 80 of the tree rooted at 20 is ");
  printf("weight = %lf, n1 = %d , n2 = %d\n",dat.weight,dat.n1,dat.n2);
}


