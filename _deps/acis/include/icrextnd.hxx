// $Id: icrextnd.hxx,v 1.8 2002/08/09 17:18:48 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined(ICREXTND_HXX)
#define ICREXTND_HXX

#include "dcl_intr.h"
#include "logical.h"

#include "polyadic.hxx"
#include "interval.hxx"

class int_cur;
class extension_info;

typedef SPAinterval (*intcur_extend_function)(
		int_cur &,
		SPAinterval const &,
		logical,
		extension_info &,
		logical
	);
DECL_INTR intcur_extend_function find_intcur_extend_function( int_cur const & );

// Class for registering specific intcur extension functions

class DECL_INTR intcur_extend_calculator : public monadic_function_entry {
	intcur_extend_function fn;
public:
	intcur_extend_calculator(int, intcur_extend_function);

	friend DECL_INTR intcur_extend_function find_intcur_extend_function(
			int_cur const &
		);
};


// Generic intcur extension function

DECL_INTR SPAinterval
extend_int_cur(
		int_cur &,
		SPAinterval const &,
		logical = TRUE,
		extension_info & = *(extension_info*)NULL_REF,
		logical = TRUE
	);

DECL_INTR SPAinterval
extend_as_intersection(
		int_cur &,
		const SPAinterval &,
		logical,
		extension_info &,
		double = 0.0,
		double = 0.0
	);

DECL_INTR SPAinterval
extend_as_silhouette(
		int_cur &,
		const SPAinterval &,
		logical,
		extension_info &
	);

#endif
