/* $Id: chkmacro.hxx,v 1.4 2000/12/26 18:22:47 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef CHKMACRO_HXX
#define CHKMACRO_HXX

/* Avoid problems if this is included from C code (no classes of course). */

#ifdef __cplusplus

// Define a class used to detect programming errors where
// a return has been coded between API_BEGIN and API_END.
// Such an error will damage the exception handling because 
// the error_save object will not be restored to its contents
// before the API_BEGIN.

#include <stdio.h>
#include "dcl_base.h"
#include "rtchecks.err"

// kev. Nov 2007. Removed this block as it is no longer in use.
#if 0

//extern DECL_BASE unsigned long _global_api_level;
DECL_BASE unsigned long & _global_api_level();

class DECL_BASE apiMacroChecker : public ACIS_OBJECT {
private:
	unsigned long mLevel;
public:
	apiMacroChecker();
	~apiMacroChecker();
};

#endif

// Define macros which check that API_END and API_NOP_END 
// macros get properly executed.  If our helper class
// adeskApiMacroChecker detects that the xxx_END macro
// was bypassed, it calls sys_error.
//
// The curly braces control when the apiMacroChecker
// destructor is called.  It must be called before the 
// rest of the API_END macro restores the previous 
// error_mark data, so sys_error's longjmp transfers 
// control to the API_END which was missed.
// 

#define ERROR_MATCHUP_BEGIN \
{ \
	apiMacroChecker thisMacroIsChecked; \
	_global_api_level()++;

#define ERROR_MATCHUP_END \
	_global_api_level()--; \
}

#else

#define ERROR_MATCHUP_BEGIN
#define ERROR_MATCHUP_END

#endif /* #ifdef __cplusplus */
#endif

