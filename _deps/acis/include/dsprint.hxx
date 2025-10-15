/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dsprint.hxx
// created  : March 15, 1999
// author   : GWCelniker
// contains : files to encapsulate printf and fprintf statements
//            DS_printf
//            DS_fprintf

#ifndef DS_PRINTF_H
#define DS_PRINTF_H

#include <stdio.h>                     // printf(), FILE

#define DS_printf1(a) printf(a)
#define DS_printf2(a,b) printf(a,b)
#define DS_printf3(a,b,c) printf(a,b,c)
#define DS_printf4(a,b,c,d) printf(a,b,c,d)
#define DS_printf5(a,b,c,d,e) printf(a,b,c,d,e)
#define DS_printf6(a,b,c,d,e,f) printf(a,b,c,d,e,f)
#define DS_printf7(a,b,c,d,e,f,g) printf(a,b,c,d,e,f,g)
#define DS_printf8(a,b,c,d,e,f,g,h) printf(a,b,c,d,e,f,g,h)
#define DS_printf9(a,b,c,d,e,f,g,h,i) printf(a,b,c,d,e,f,g,h,i)
#define DS_printf10(a,b,c,d,e,f,g,h,i,j) printf(a,b,c,d,e,f,g,h,i,j)
#define DS_printf11(a,b,c,d,e,f,g,h,i,j,k) printf(a,b,c,d,e,f,g,h,i,j,k)

#define DS_fprintf1(a) fprintf(a)
#define DS_fprintf2(a,b) fprintf(a,b)
#define DS_fprintf3(a,b,c) fprintf(a,b,c)
#define DS_fprintf4(a,b,c,d) fprintf(a,b,c,d)
#define DS_fprintf5(a,b,c,d,e) fprintf(a,b,c,d,e)
#define DS_fprintf6(a,b,c,d,e,f) fprintf(a,b,c,d,e,f)
#define DS_fprintf7(a,b,c,d,e,f,g) fprintf(a,b,c,d,e,f,g)
#define DS_fprintf8(a,b,c,d,e,f,g,h) fprintf(a,b,c,d,e,f,g,h)
#define DS_fprintf9(a,b,c,d,e,f,g,h,i) fprintf(a,b,c,d,e,f,g,h,i)
#define DS_fprintf10(a,b,c,d,e,f,g,h,i,j) fprintf(a,b,c,d,e,f,g,h,i,j)
#define DS_fprintf11(a,b,c,d,e,f,g,h,i,j,k) fprintf(a,b,c,d,e,f,g,h,i,j,k)

/*
int DS_printf              // eff: encapsulate printf for customers
 (const char *format,      // in : the printf char string
	 ...)  ;                  // in : all the other arguments
	
int DS_fprintf             // eff: encapsulate printf for customers
  (FILE *fp,               // in : file pointer
	  const char *format,     // in : the format string
	  ... ) ;                 // in : the argument list
*/

#endif // DS_PRINTF_H
