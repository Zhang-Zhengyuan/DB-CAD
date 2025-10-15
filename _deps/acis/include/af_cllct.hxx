/* ORIGINAL: acis2.1/faceter/util/af_cllct.hxx */
// $Id: af_cllct.hxx,v 1.6 2000/12/26 18:35:39 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _AF_CLLCT
#define _AF_CLLCT



#include <stdio.h>
// Definitions common to collector data structures.

// AF_ADD_MODE -- enumerated type for qualifier argument
//	"add" functions for collections.
// Usage convention: Each collection implements an unqualified
//	add function
//		collection->add(void*p,AF_ADD_MODE)
// Where relevant, the collection supports specialized
//	additions as described by the qualifier.
typedef enum {
	AF_ADD_AT_TAIL,
	AF_ADD_AT_HEAD,
	AF_ADD_UNIQUE
    } AF_ADD_MODE;

// COMPARE_FUNC is the standard 3-way comparison as used by
//	qsort:
typedef int (*COMPARE_FUNC)(const void*,const void*);

#include <stdlib.h>




#endif
