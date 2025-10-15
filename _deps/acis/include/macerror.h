/* $Id: macerror.h,v 1.6 2002/01/30 17:32:55 skenny Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifdef mac

#ifndef mac_error_hxx
#define mac_error_hxx

#ifdef __cplusplus
#include "carbonheader.h"
#endif


#ifdef __cplusplus
extern "C"	{
#endif

void OutOfMemory(void);
void deathAlert(void);

#ifdef __cplusplus
}
#endif

#endif

#endif
