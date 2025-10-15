/* $Id: static_types.hxx,v 1.5 2000/12/26 18:22:38 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef STATIC_TYPES_HXX
#define STATIC_TYPES_HXX

/* Indicate the scope of a variable, procedure, or class */
/* to a known file, component, or globally. This will be */
/* used to identify global and static variables for multithreading. */
/* Eventually, it may even indicate the interfaces to the components */
/* of the toolkit. */

/*	LOCAL_PROC			file scope; procedure	*/
/*	LOCAL_CONST			file scope; constant	*/
/*	LOCAL_PCONST		file scope; constant after initialization */
/*	LOCAL_VAR			file scope; variable	*/
/*	LOCAL_CLASS			file scope; class 	*/

/*	COMPONENT_PROC		component scope; procedure	*/
/*	COMPONENT_CONST		component scope; constant	*/
/*	COMPONENT_PCONST	component scope; constant after initialization	*/
/*	COMPONENT_VAR		component scope; variable	*/
/*	COMPONENT_CLASS		component scope; class	*/

/*	GLOBAL_PROC			global scope; procedure	*/
/*	GLOBAL_CONST		global scope; constant	*/
/*	GLOBAL_PCONST		global scope; constant after initialization	*/
/*	GLOBAL_VAR			global scope; variable	*/
/*	GLOBAL_CLASS		global scope; class	*/

/*	SESSION_LOCAL_CONST	file scope; session constant */
/*	SESSION_LOCAL_VAR	file scope; session variable */

#define LOCAL_PROC static
#define LOCAL_CONST static const
#define LOCAL_PCONST static
#define LOCAL_VAR static
#define LOCAL_CLASS static class

#define COMPONENT_PROC 
#define COMPONENT_CONST const
#define COMPONENT_PCONST
#define COMPONENT_VAR
#define COMPONENT_CLASS class

#define GLOBAL_PROC 
#define GLOBAL_CONST const
#define GLOBAL_PCONST
#define GLOBAL_VAR
#define GLOBAL_CLASS class

#define SESSION_GLOBAL_VAR
#define SESSION_LOCAL_CONST static const
#define SESSION_LOCAL_VAR static

#endif

