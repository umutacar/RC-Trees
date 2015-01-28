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

//////////////////////////////////////////////////////////////////////////////
// AMath.c
//
// Jorge Vittes
//
// Additional Math functions for doing things
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "AMath.h"

///////////////////////////////////////////////////
// Find the minimum of two doubles
//////////////////////////////////////////////////
double dmin(double a, double b)
{
  if (a > b) return b;
  else return a;
}

/////////////////////////////////////////////////
// Find the minimum of three doubles
/////////////////////////////////////////////////
double min3(double a, double b, double c)
{
  if(a > b) {
    if(b > c) return c;
    else return b;
  }
  else {
    if(a > c) return c;
    else return a;
  }
}

/////////////////////////////////////////////////
// return the max of two doubles
////////////////////////////////////////////////
double dmax(double a, double b)
{
  if(a > b) return a;
  else return b;
}

/////////////////////////////////////////////////
// Return the maximum of two ints 
/////////////////////////////////////////////////
int imax(int a, int b)
{
  if(a > b) return a;
  return b;
}

///////////////////////////////////////////////
// Find the maximum of three doubles 
///////////////////////////////////////////////
double max3(double a, double b, double c)
{
  if(a > b) {
    if(a > c) return a;
    else return c;
  }
  else {
    if(b > c) return b;
    else return c;
  }
}
