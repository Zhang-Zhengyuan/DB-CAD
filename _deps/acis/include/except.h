/* ORIGINAL: acis2.1/kernutil/errorsys/except.h */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/* @(#)except.h	24.1	27 Sep 1995 */

/* STI aed: C interface to ACIS exception handling mechanism */

#if !defined( EXCEPTION_H )
#define EXCEPTION_H

#include "dcl_kern.h"
#include "logical.h"
#include "errorsys.hxx"
#include <setjmp.h>

typedef int err_mess_type;


/* Declare fatal error in ACIS */

//DECL_KERN void sys_error(err_mess_type);

/* Determine whether ACIS has been interrupted */

//DECL_KERN logical acis_interrupted();

/* Save exception tracking information from C */

DECL_KERN void c_exception_begin(jmp_buf);

/* Restore previous exception tracking info from C */

DECL_KERN void c_exception_end();


#define C_EXCEPTION_TRY \
	{ \
		err_mess_type resignal_no = 0; \
		jmp_buf save_buf; \
		err_mess_type error_no; \
		error_no = setjmp( save_buf ); \
		resignal_no = error_no; \
		if (error_no == 0) { \
			c_exception_begin(save_buf);

/* Normal processing code goes here		*/
/* DO NOT use a return statement here	*/
/* unless it is immediately preceded by	*/
/* a call to c_exception_end()			*/

#define C_EXCEPTION_CATCH(always_clean) \
		} \
		if ((always_clean) || error_no != 0) {

/* Interrupt/error cleanup code goes here	*/
/* DO NOT use a return statement here		*/

#define C_EXCEPTION_END \
		} \
		c_exception_end(); \
		if (resignal_no != 0 || acis_interrupted()) \
			sys_error(resignal_no ? resignal_no : error_no); \
	}

#endif
