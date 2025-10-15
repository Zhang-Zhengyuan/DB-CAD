/* ORIGINAL: acis2.1/kerndata/data/metharg.hxx */
/* $Id: metharg.hxx,v 1.7 2000/12/26 18:46:47 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// STI aed: New support for Runtime virtual methods

// Header for Runtime Virtual Method argument list classes

// Developers derive classes from METHOD_ARGS for each new method they
// define.

// Each class MUST define the virtual method "id" which must return a
// unique character string to allow runtime identification of overloaded
// methods.

#ifndef METHARG_HXX
#define METHARG_HXX

#include "dcl_kern.h"
#include "acis.hxx"
/**
* @file metharg.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup KERNAPI
 *
 * @{
 */

/**
 * Defines the virtual method <tt>ID</tt> that must return a unique character string to allow runtime identification of overloaded methods.
 * <br>
 * <b>Role:</b> Developers derive classes from <tt>METHOD_ARGS</tt> for each new defined method. Each class must
 * define the virtual method <tt>ID</tt> that must return a unique character string to allow runtime
 * identification of overloaded methods.
 */
class DECL_KERN METHOD_ARGS : public ACIS_OBJECT
{
public:

/**
 * Returns the <tt>ID</tt> for the <tt>METHOD_ARGS</tt> list.
 */
 virtual const char *id() const;
};

// Provide a void argument list for methods which require no arguments.
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN VOID_ARGS : public METHOD_ARGS
{
public:
	virtual const char *id() const;
};

/*! @} */
#endif
