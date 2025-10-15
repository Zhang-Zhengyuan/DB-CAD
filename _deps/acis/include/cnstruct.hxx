/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//            Construct functions for upper case GEOMETRY
//                 from lower case geometry.
/*******************************************************************/
#if !defined( CNSTRUCT_HDR_DEF )
#define  CNSTRUCT_HDR_DEF
#include "dcl_kern.h"
#include "polyadic.hxx"
class CURVE;
class curve;
class SURFACE;
class surface;
/*******************************************************************/
/**
* @file cnstruct.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup CSTRMAKEOTHERAPI
 *
 * @{
 */
/**
 * Creates a curve (upper case) for the given curve constant.
 * <br><br>
 * @param crv
 * Curve
 */
DECL_KERN CURVE *make_curve(curve const & crv);
/**
* Creates a surface for the given surface constant.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param this_surface
* Surface
**/
DECL_KERN SURFACE *make_surface(surface const & this_surface);
/**
 * @nodoc
 * Internal use.
 * Allows the definition of additional curve and surface types, and
 * provide suitable "constructors".
 */
typedef CURVE *(*CURVE_constr_function)(curve const &);
/**
 * @nodoc
 * Internal use.
 * Function to determine the "constructor" function.
 */
CURVE_constr_function find_CURVE_constr_function( curve const & );
/**
 * @nodoc
 * Class for allowing the definition of additional curve types, and
 * provide suitable "constructors".
 */
class DECL_KERN CURVE_constructor: public monadic_function_entry
{
	/**
	 * Constructor function.
	 */
	CURVE_constr_function fn;
public:
	/**
	 * Constructor
	 */
	CURVE_constructor( int, CURVE_constr_function );
	/**
	 * Function to find a suitable constructor function for the given
	 * curve type.
     */
	friend CURVE_constr_function find_CURVE_constr_function(
			curve const &
		);
};
/**
 * @nodoc
 * Internal use. Type of surface constructor  function.
 */
typedef SURFACE *(*SURFACE_constr_function)(
			surface const &
		);
/**
 * @nodoc
 * Internal use.
 * Used to find a  suitable constructor function according to the surface
 * type.
 */
SURFACE_constr_function find_SURFACE_constr_function( surface const & );
/**
 * @nodoc
 * Class for allowing the definition of additional surface types constructors.
 */
class DECL_KERN SURFACE_constructor: public monadic_function_entry {
	/**
	 * Surface constructor function.
	 */
	SURFACE_constr_function fn;
public:
	/**
	 * C Constructor
	 */
	SURFACE_constructor( int, SURFACE_constr_function );
   /**
	* Function to find a suitable constructor function for the given
	* surface type.
	*/
	friend SURFACE_constr_function find_SURFACE_constr_function(
			surface const &
		);
};

/** @} */
#endif
