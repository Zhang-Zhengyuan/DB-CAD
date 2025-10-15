/* ORIGINAL: 3dt2.1/scheme/elk/type.h */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/*
// $Id: type.h,v 1.6 2001/04/03 17:54:08 jeff Exp $
//----------------------------------------------------------------------
// purpose---
//	  Some type checking definitions
//
//----------------------------------------------------------------------
*/
/* Miscellaneous macros for type-checking Scheme objects.
 */
#ifndef TYPE_H

#define Check_Type(x,t) {\
    if (TYPE(x) != t) Wrong_Type (x, t);\
}

#define Check_List(x) {\
    if (TYPE(x) != T_Pair && !Nullp (x)) Wrong_Type_Combination (x, "list");\
}

#define Check_Number(x) {\
    register t = TYPE(x);\
    if (!Numeric (t)) Wrong_Type_Combination (x, "number");\
}

/* This should be renamed; it checks whether x is an *exact* integer.
 */
#define Check_Integer(x) {\
    register t = TYPE(x);\
    if (t != T_Fixnum && t != T_Bignum) Wrong_Type (x, T_Fixnum);\
}

#define Check_Mutable(x) {\
    if (ISCONST(x))\
	Primitive_Error ("attempt to modify constant");\
}

#endif
