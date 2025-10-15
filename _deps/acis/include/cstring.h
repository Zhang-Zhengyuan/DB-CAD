/* ORIGINAL: 3dt2.1/scheme/elk/cstring.h */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/*
// $Id: cstring.h,v 1.7 2001/04/25 21:49:13 jeff Exp $
//----------------------------------------------------------------------
// purpose---
//    Declare Scheme String macros
//
//----------------------------------------------------------------------
*/
#ifndef CSTRING_H
#define CSTRING_H

/* These must be defined as macros, because they use Alloca(). */
#define Get_String_Stack(_from,_to) {\
    int _len;\
    Check_Type(_from, T_String);\
    _len = STRING(_from)->size;\
    (_to) = (char *)ACIS_ALLOCA(_len+1);\
    memcpy ((_to), STRING(_from)->data, _len);\
    (_to)[_len] = '\0';\
}

#define Get_Strsym_Stack(_from,_to) {\
    int _len;\
    if (TYPE(_from) == T_Symbol)\
	(_from) = SYMBOL(_from)->name;\
    else if (TYPE(_from) != T_String)\
	Wrong_Type_Combination ((_from), "string or symbol");\
    _len = STRING(_from)->size;\
    (_to) = (char *)ACIS_ALLOCA(_len+1);\
    memcpy ((_to), STRING(_from)->data,_len);\
    (_to)[_len] = '\0';\
}

#endif
