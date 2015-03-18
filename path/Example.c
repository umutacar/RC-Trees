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
// Code for doing path queries on a forest
///////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <time.h>
#include "Application.h"
#include "Contract.h"
#include "Interface.h"
//#incldue "Main.c"

#define KILO 1000
#define MILLON 1000000

int main()
{
	int experiment;
        for (int times = 0; times < 10; ++times) {
	for (experiment = 0; experiment <= 1; ++experiment) {
		int i;
		int n = 5000;
		int t0 = time(0);
		RC_Forest F(2*n);
		debug = 0;
		for(i=1;i<= 2*n-1 ;i++)
		{
			if(i == n)
				continue;
			else
				F.link(F.vertex(i), F.vertex(i+1),1);
		}
		printf("Before the first contract\n");
  	  	F.contract();
  	    printf("Initial links done\n");
		for(i=1;i<=n;i++)
		{
			int isFirstHalf = i <= n / 2;
			int source = experiment ? 1 : isFirstHalf ? i * 2 : i * 2 - n - 1;
			int target = n + source;
			if (source > n || source < 1) {
				printf("fail (i = %d source = %d)\n", i, source);
				fflush(stdout);
			}
			F.link(F.vertex(source), F.vertex(target),2);
			F.contract();
			bin_data dat = pathQuery(F.vertex(source), F.vertex(target));
			F.cut(F.vertex(source),F.vertex(target));
			F.contract();
		}
		printf("finish link and cut\n");
		printf("experiment %d ended in %ld seconds\n", experiment, time(0) - t0);
	}
}
}

