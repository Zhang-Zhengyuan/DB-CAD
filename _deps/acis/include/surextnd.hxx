// $Id: surextnd.hxx,v 1.7 2002/08/09 17:18:49 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined(SUREXTND_HXX)
#define SUREXTND_HXX

#include "dcl_intr.h"
#include "logical.h"

#include "polyadic.hxx"
#include "param.hxx"

#include "acis_options.hxx"

/**
* @file surextnd.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

/** \addtogroup ACISGEOMETRY
 *  \brief Declared at <surextnd.hxx>, SPAintr
 *  @{
 */

class surface;
class extension_info;

typedef AcisOptions ExtendOptions;

typedef SPApar_box (*surface_extend_function)(
		surface &,
		SPApar_box const &,
		logical,
		extension_info &,
		extension_info &
	);
DECL_INTR surface_extend_function find_surface_extend_function( surface const & );

// Class for registering specific surface extension functions

class DECL_INTR surface_extend_calculator : public monadic_function_entry {
	surface_extend_function fn;
public:
	surface_extend_calculator(int, surface_extend_function);

	friend DECL_INTR surface_extend_function find_surface_extend_function(
			surface const &
		);
};

// Generic surface extension function
/**
* In place surface extension.
* <br><br>
* <b>Role:</b> Extend a surface in place, returning the extension that was actually achieved. 
* <br><br>
* If possible, the input surface is extended out to the requested parameter box. However, if the
* extended surface is found to be improper (e.g., self-intersecting), the requested extension will not be completed.
* In this case, if the logical flag partial_ok is TRUE, a smaller extension, which is proper, 
* will be performed.
* In either case, the achieved pamameter box will be returned. Clients should check the returned parameter box to determine the success of
* extend_surface.
* <br><br>
* <b>Limitations:</b> Surfaces cannot be extended in closed or periodic directions, nor beyond singularities. extend_surface will not shrink a parameter box.
* <br><br>
* <br><br>
* @param sur
* surface to be extended.
* @param requested_parameter_box
* requested parameter box of the extended surface.
* @param partial_ok
* if the requested parameter box cannot be achieved, controls whether a partial extension is performed; TRUE for yes, FALSE for no extension
* @param eiu
* Clients should not use.
* @param eiv
* Clients should not use.
**/
DECL_INTR SPApar_box
extend_surface(
		surface & sur,
		SPApar_box const & requested_parameter_box,
		logical partial_ok = TRUE,
		extension_info & eiu = *(extension_info*)NULL_REF,
		extension_info & eiv = *(extension_info*)NULL_REF	);



DECL_INTR SPApar_box
extend_surface_with_options(
		surface & sur,
		SPApar_box const & requested_parameter_box,
		logical partial_ok,
		extension_info & eiu,
		extension_info & eiv,
		ExtendOptions const & aeo	);

DECL_INTR SPApar_box 
extend_spline( 
		surface &sur, 
		const SPApar_box& new_range, 
		logical approx_ok,  
		extension_info& u_info, 
		extension_info& v_info 
		);
/*! @} */

/** @} */

#endif
