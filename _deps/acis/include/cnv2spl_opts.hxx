/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( CNV2SPL_OPTIONS_CLASS )
#define CNV2SPL_OPTIONS_CLASS			

#include "logical.h"
#include "logical_state.hxx"
#include "dcl_bool.h"           
#include "acis.hxx"

class ENTITY;
class error_info_list;

/**
* @file cnv2spl_opts.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */


/**
 * Defines a class for specifying options used in spline-conversion operations.
 * <br>
 * <b>Role:</b> This class is used to configure spline-conversion operations carried out
 * by @href api_convert_to_spline.  It allows users to specify whether the conversion
 * is performed directly on the input entity ("in place") or on a copy.  It also allows
 * toggling the conversion of faces and edges independently.  This
 * information is attached to a @href convert_to_spline_options object by means of
 * class methods before use by @href api_convert_to_spline.  Following conversion,
 * the @href get_return_entity method may be used to access the spline-converted entity.
 * <br><br>
 * @see api_convert_to_spline
 */
class DECL_BOOL convert_to_spline_options : public ACIS_OBJECT
{
	friend logical 
		convert_to_spline_failsafe(
		ENTITY*&					the_entity,
		convert_to_spline_options&	ctso,
		logical&					proceeded_after_errors,
		error_info_list&			eil);

	logical_state	_do_faces;
	logical_state	_do_edges;
	logical_state	_in_place;
	ENTITY*			_return_entity;

public:
/**
 * C++ allocation constructor requests memory for this object and sets the default values
 * of its member data.
 * <br>
 * <b>Role:</b> Unless subsequently modified by the @href set_in_place method, use of an object
 * created by this constructor in performing spline conversions results in the conversion
 * being performed on a copy of the supplied entity rather than on the supplied entity
 * itself.  Similarly, the default object directs that both faces and edges have their
 * geometry converted.
 */
    convert_to_spline_options();

 /**
 * Returns a flag indicating whether or not spline conversion is to be performed
 * on faces.
 */
    logical get_do_faces() const;
 /**
 * Returns a flag indicating whether or not spline conversion is to be performed
 * on edges.
 */
    logical get_do_edges() const;
/**
 * Returns a flag indicating whether or not spline conversion is to be performed on
 * the supplied entity (<tt>TRUE</tt>), or on a copy (<tt>FALSE</tt>).
 */
    logical get_in_place() const;

 /**
 * Returns a pointer to the entity returned by a call to @href api_convert_to_spline.
 * If in-place modification was specified, this is the same as the
 * input entity to the API.
 */
   ENTITY* get_return_entity() const;

/**
 * Sets a flag indicating whether or not spline conversion is to be performed on
 * faces. Note: if set_do_faces(TRUE) is set, and an edge is spline converted, 
 * all faces connected to that edge will also be converted.
 */
	void set_do_faces(logical do_faces);
/**
 * Sets a flag indicating whether or not spline conversion is to be performed on
 * edges.
 */
	void set_do_edges(logical do_edges);
/**
 * Sets a flag indicating whether or not spline conversion is to be performed on
 * the supplied entity (<tt>TRUE</tt>), or on a copy (<tt>FALSE</tt>).
 */
	void set_in_place(logical in_place);
};

#endif // CNV2SPL_OPTIONS_CLASS

