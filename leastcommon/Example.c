////////////////////////////////////////////////////////////////
// Example.c
//
// Jorge L. Vittes
//
// Code for testing dynamic parallel tree contraction
// Code for finding the least common Ancestor of a tree using 
// support trees. 
////////////////////////////////////////////////////////////////

#include "Application.h"
#include "Interface.h"
#include "Contract.h"
#include <stdlib.h>
#include <sys/times.h>
#include <sys/param.h>
#include <stdint.h>

#define KILO 1000
#define MILLON 1000000


int main()
{
  int i,k,n,a,b,w;
  struct tms start,end;
  clock_t r1,r2;
  printf("enter n\n");
  scanf(" %d",&n);			// 543 works, 544 doesn't
  r1=times(&start);
  printf("start initial run\n");
  RC_Forest F(n);
  printf("enter k\n");
  while (scanf(" %d",&k),k>=0) {
    for (i=0;i<k;i++) {
      printf("enter a,b,w\n");
      scanf(" %d%d%d",&a,&b,&w);
      if (a<b) F.link(F.vertex(a+1),F.vertex(b+1),w);
    }
  }
  F.contract();
  r2=times(&end);
  printf("finished initial run\n");
  printf("process time: %.3lfs\n",((double)end.tms_utime-start.tms_utime)/HZ);
  printf("real time: %.3lfs\n",((double)r2-r1)/HZ);
  r1=times(&start);
  int mode;
  printf("enter mode (0 for cut, 1 for link)\n");
  while (scanf(" %d",&mode)!=EOF) {
    if (mode==0) {
      printf("enter a,b\n");
      scanf(" %d%d",&a,&b);
      F.cut(F.vertex(a+1),F.vertex(b+1));
    } else {
      printf("enter a,b,w\n");
      scanf(" %d%d%d",&a,&b,&w);
      F.link(F.vertex(a+1),F.vertex(b+1),w);
    }
    F.contract(); 
  }
  r2=times(&end);
  printf("finished change propagation\n");
  printf("process time: %.3lfs\n",((double)end.tms_utime-start.tms_utime)/HZ);
  printf("real time: %.3lfs\n",((double)r2-r1)/HZ);
  return 0;
}

