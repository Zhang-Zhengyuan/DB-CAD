/* ORIGINAL: acis2.1/acis.hxx */
/* $Id: acis.hxx,v 1.39 2000/12/26 18:46:18 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#include "base.hxx"

#if !defined( ACIS_OLD_HEADER_INCLUDED )
#define ACIS_OLD_HEADER_INCLUDED

#if defined ( __cplusplus )
#define POLL_FOR_CONTROL_C

#if !defined ( POLL_FOR_CONTROL_C )
#define ACIS_EXCEPTION_CHECK(t); 
#else
DECL_BASE void ACISExceptionCheck(const char*);
#define ACIS_EXCEPTION_CHECK(t); ACISExceptionCheck(t);
#endif
#endif /* __cplusplus */

#endif /* ACIS_OLD_HEADER_INCLUDED */

