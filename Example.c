/******************************************************
 * Example.c
 *
 * Jorge L. Vittes
 *
 * Code for testing dynamic parallel tree contraction
 * Code for finding the center of a tree using support trees. 
 *********************************************************/

#include "Application.h"
#include "Interface.h"
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
      F.link(F.vertex(i),F.vertex(i+1),1);
      printf("linking %d to %d\n",i,i+1);
    }
  //link them in batch
  //debug=1;
  F.contract();

  //get the root of the support tree
  cluster* rt = getRoot(F.vertex(10));

  //find the center
  int ctr = center(rt);
  printf("The center of the tree is %d\n",ctr);
  //now unlink 49-50 and link 1-100
  F.cut(F.vertex(49),F.vertex(50));
  F.link(F.vertex(1),F.vertex(100),1);
  F.contract();

  //get the root of the support tree
  rt = getRoot(F.vertex(10));

  ctr = center(rt);
  printf("The new center of the tree is %d\n",ctr);
}

