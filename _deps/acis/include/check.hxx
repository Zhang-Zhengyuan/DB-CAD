/* ORIGINAL: acis2.1/kernapi/api/check.hxx */
/* $Id: check.hxx,v 1.12 2002/08/09 17:15:16 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// API header file for argument checking routines

#if !defined( API_CHECKS )
#define API_CHECKS

#include <stdio.h>
#include "dcl_kern.h"
#include "logical.h"

class BODY;
class WIRE;
class FACE;
class EDGE;
class COEDGE;
class DELTA_STATE;
class HISTORY_STREAM;
class PLANE;
class TRANSFORM;
class ENTITY;
class SPAvector;
class VERTEX;
class LUMP;
class SHELL;
class LOOP;
class CURVE;
class PART;
class law;
class TEDGE;
class TCOEDGE;
class ENTITY_LIST;

/**
* \defgroup ACISAPICHECKING Checking
* \ingroup ACISAPIBEHAVIOR
*
*/
/**
* @file check.hxx
* @CAA2Level L1
* @CAA2Usage U1
* \addtogroup ACISAPICHECKING
*
* @{
*/


/**
* Checks if a array pointer is valid.
* <br><br>
* <b>Errors:</b><br>
* <tt>entity_array</tt> is a NULL pointer.
* <br><br>
* @param entity_array
* (in) Array pointer.
**/
DECL_KERN void check_array_exists(ENTITY** entity_array);

/**
* Checks if a array length is non-zero.
* <br><br>
* <b>Errors:</b><br>
* <tt>length</tt> is zero.
* <br><br>
* @param length
* (in) Array length.
**/
DECL_KERN void check_array_length(int length);

/**
* Checks if a body pointer is valid.
* <br><br>
* <b>Errors:</b><br>
* <tt>body</tt> is a NULL pointer or does not point to a @href BODY.<br>
* Functionality does not support polyhedral geometry.<br>
* Functionality does not support hybrid bodies.
* <br><br>
* @param body
* (in) Body pointer.
* @param poly_supported
* (in) Allow polyhedral bodies.
* @param hybrid_friendly
* (in) Allow hybrid bodies.
**/
DECL_KERN void check_body(BODY const* body, bool poly_supported = false, bool hybrid_friendly = false);

/**
* Checks if a coedge pointer is valid.
* <br><br>
* <b>Errors:</b><br>
* <tt>coedge</tt> is a NULL pointer or does not point to a @href COEDGE.<br>
* Functionality does not support polyhedral geometry.<br>
* Functionality does not support hybrid geometry.
* <br><br>
* @param coedge
* (in) Coedge pointer.
* @param poly_supported
* (in) Allow polyhedral geometry.
* @param hybrid_friendly
* (in) Allow hybrid geometry.
**/
DECL_KERN void check_coedge(COEDGE const* coedge, bool poly_supported = false, bool hybrid_friendly = false);

/**
* Checks if a delta state pointer is valid.
* <br><br>
* <b>Errors:</b><br>
* <tt>ds</tt> is a NULL pointer.
* <br><br>
* @param ds
* (in) Delta state pointer.
**/
DECL_KERN void check_delta(DELTA_STATE const* ds);

/**
* Checks if a history stream pointer is valid.
* <br><br>
* <b>Errors:</b><br>
* <tt>hs</tt> is a NULL pointer.
* <br><br>
* @param hs
* (in) History stream pointer.
**/
DECL_KERN void check_stream(HISTORY_STREAM const* hs);

/**
* Checks if an edge pointer is valid.
* <br><br>
* <b>Errors:</b><br>
* <tt>edge</tt> is a NULL pointer or does not point to an @href EDGE.<br>
* Functionality does not support polyhedral geometry.<br>
* Functionality does not support hybrid geometry.
* <br><br>
* @param edge
* (in) Edge pointer.
* @param poly_supported
* (in) Allow polyhedral geometry.
* @param hybrid_friendly
* (in) Allow hybrid geometry.
**/
DECL_KERN void check_edge(EDGE const* edge, bool poly_supported = false, bool hybrid_friendly = false);

/**
* Checks if an entity pointer is valid.
* <br><br>
* <b>Errors:</b><br>
* <tt>entity</tt> is a NULL pointer.<br>
* Functionality does not support polyhedral geometry.<br>
* Functionality does not support hybrid geometry.
* <br><br>
* @param entity
* (in) Entity pointer.
* @param poly_supported
* (in) Allow polyhedral geometry.
* @param hybrid_friendly
* (in) Allow hybrid geometry.
**/
DECL_KERN void check_entity(ENTITY const* ent, bool poly_supported = false, bool hybrid_friendly = false);

/**
* Checks if a face list is valid.
* <br><br>
* <b>Errors:</b><br>
* <tt>faces</tt> is empty or contains other than faces.<br>
* Functionality does not support polyhedral geometry.<br>
* Functionality does not support hybrid geometry.
* <br><br>
* @param faces
* (in) Face list.
* @param poly_supported
* (in) Allow polyhedral geometry.
* @param hybrid_friendly
* (in) Allow hybrid geometry.
**/
DECL_KERN void check_face_list(ENTITY_LIST const &faces, bool poly_supported = false, bool hybrid_friendly = false);

/**
* Checks if a face pointer is valid.
* <br><br>
* <b>Errors:</b><br>
* <tt>face</tt> is a NULL pointer or does not point to a @href FACE.<br>
* Functionality does not support polyhedral geometry.<br>
* Functionality does not support hybrid geometry.
* <br><br>
* @param face
* (in) Face pointer.
* @param poly_supported
* (in) Allow polyhedral geometry.
* @param hybrid_friendly
* (in) Allow hybrid geometry.
**/
DECL_KERN void check_face(FACE const* face, bool poly_supported = false, bool hybrid_friendly = false);

// Ouch!  The definition for the following function does not exist!
// DECL_KERN void check_graph(BODY const* body);

/**
* Checks if a %plane pointer is valid.
* <br><br>
* <b>Errors:</b><br>
* <tt>plane</tt> is a NULL pointer or does not point to a @href PLANE.
* <br><br>
* @param plane
* (in) Plane pointer.
**/
DECL_KERN void check_plane(PLANE const* plane);

// The following function current is not robust!
// error if null pointer
/**
* @nodoc
*/
DECL_KERN void check_sheet(BODY const* body, bool poly_supported = false, bool hybrid_friendly = false);

/**
* Checks if a wire pointer is valid.
* <br><br>
* <b>Errors:</b><br>
* <tt>wire</tt> is a NULL pointer or does not point to a @href WIRE.<br>
* Functionality does not support polyhedral geometry.<br>
* Functionality does not support hybrid geometry.
* <br><br>
* @param wire
* (in) Wire pointer.
* @param poly_supported
* (in) Allow polyhedral geometry.
* @param hybrid_friendly
* (in) Allow hybrid geometry.
**/
DECL_KERN void check_wire(WIRE const *wire, bool poly_supported = false, bool hybrid_friendly = false);

/**
* @nodoc
*/
DECL_KERN void check_body_lump_shell_or_face(ENTITY const *ent, bool poly_supported = false, bool hybrid_friendly = false);

/**
* Checks if a length value is non-negative.
* <b>Role:</b> Checks if the length is less than <tt>-SPAresabs</tt>. 
* Valid values for <tt>name</tt> are: <tt>"length"</tt> and <tt>"top"</tt>.
* <br><br>
* <b>Errors:</b><br>
* <tt>length</tt> is not positive or zero.
* <br><br>
* @param length
* (in) Length value.
* @param name
* (in) Parameter name.
**/
DECL_KERN void check_non_neg_length(double length, char const *name);

/**
* Checks if a length value is non-zero.
* <b>Role:</b> Checks if the absolute value of the length is less than <tt>SPAresabs</tt>.
* Valid values for <tt>name</tt> are: <tt>"length"</tt>, <tt>"distance"</tt> <tt>"normal"</tt> 
* and <tt>"radius"</tt>.
* <br><br>
* <b>Errors:</b><br>
* <tt>length</tt> is too close to zero.
* <br><br>
* @param length
* (in) Length value.
* @param name
* (in) Parameter name.
**/
DECL_KERN void check_non_zero_length(double length, char const *name);

/**
* Checks if a length value is positive.
* <b>Role:</b> Checks if the length is less than <tt>SPAresabs</tt>.
* Valid values for <tt>name</tt> are: <tt>"length"</tt>, <tt>"width"</tt>, <tt>"depth"</tt>, 
* <tt>"height"</tt>, <tt>"radius"</tt>, <tt>"rad1"</tt>, and <tt>"rad2"</tt>.
* <br><br>
* <b>Errors:</b><br>
* <tt>length</tt> is negative or zero.
* <br><br>
* @param length
* (in) Length value.
* @param name
* (in) Parameter name.
**/
DECL_KERN void check_pos_length(double length, char const *name);

/**
* Checks if a length value is positive.
* <b>Role:</b> Checks if the length is less than <tt>SPAresfit</tt>.
* Valid values for <tt>name</tt> are: <tt>"length"</tt>, <tt>"rad1"</tt>, and <tt>"rad2"</tt>.
* <br><br>
* <b>Errors:</b><br>
* <tt>length</tt> is negative or zero.
* <br><br>
* @param length
* (in) Length value.
* @param name
* (in) Parameter name.
**/
DECL_KERN void check_resfit_pos_length(double length, char const *name);

/**
* Checks if the number of sides is less than three.
* <br><br>
* <b>Errors:</b><br>
* Too few sides (<3) requested.
* <br><br>
* @param n_sides
* (in) Number of sides.
**/
DECL_KERN void check_3sides(int n_sides);

// The following function current is not robust!
// checks if body is a simple sheet body
/**
* @nodoc
*/
DECL_KERN logical check_sheet_body(BODY const* body, bool poly_supported = false, bool hybrid_friendly = false);

// The following function current is not robust!
// checks wire pointer not null
/**
* @nodoc
*/
DECL_KERN void check_wire_body(BODY const* body, bool poly_supported = false, bool hybrid_friendly = false);

/**
* Checks if a transform pointer is valid.
* <b>Role:</b> Allows a <tt>NULL</tt> pointer which signifies an identity transformation.
* <br><br>
* <b>Errors:</b><br>
* <tt>trans</tt> is not a pointer to a <tt>TRANSFORM</tt>.
* <br><br>
* @param trans
* (in) Transform pointer.
**/
DECL_KERN void check_transform(TRANSFORM const* trans);	

/**
* Checks if a vertex pointer is valid.
* <br><br>
* <b>Errors:</b><br>
* <tt>vertex</tt> is a NULL pointer or does not point to a @href VERTEX.<br>
* Functionality does not support polyhedral geometry.<br>
* Functionality does not support hybrid geometry.
* <br><br>
* @param vertex
* (in) Vertex pointer.
* @param poly_supported
* (in) Allow polyhedral geometry.
* @param hybrid_friendly
* (in) Allow hybrid geometry.
**/
DECL_KERN void check_vertex(VERTEX const* vertex, bool poly_supported = false, bool hybrid_friendly = false);

/**
* Checks if a lump pointer is valid.
* <br><br>
* <b>Errors:</b><br>
* <tt>lump</tt> is a NULL pointer or does not point to a @href LUMP.<br>
* Functionality does not support polyhedral geometry.<br>
* Functionality does not support hybrid geometry.
* <br><br>
* @param lump
* (in) Lump pointer.
* @param poly_supported
* (in) Allow polyhedral geometry.
* @param hybrid_friendly
* (in) Allow hybrid geometry.
**/
DECL_KERN void check_lump(LUMP const* lump, bool poly_supported = false, bool hybrid_friendly = false);

/**
* Checks if a shell pointer is valid.
* <br><br>
* <b>Errors:</b><br>
* <tt>shell</tt> is a NULL pointer or does not point to a @href SHELL.<br>
* Functionality does not support polyhedral geometry.<br>
* Functionality does not support hybrid geometry.
* <br><br>
* @param shell
* (in) Shell pointer.
* @param poly_supported
* (in) Allow polyhedral geometry.
* @param hybrid_friendly
* (in) Allow hybrid geometry.
**/
DECL_KERN void check_shell(SHELL const* shell, bool poly_supported = false, bool hybrid_friendly = false);

/**
* Checks if an entity pointer is valid and of the correct type.
* <br><br>
* <b>Errors:</b><br>
* <tt>ent</tt> is a NULL pointer or is of an incorrect type.<br>
* Functionality does not support polyhedral geometry.<br>
* Functionality does not support hybrid geometry.
* <br><br>
* @param ent
* (in) Entity pointer.
* @param level
* (in) Entity level.
* @param type
* (in) Entity type.
* @param poly_supported
* (in) Allow polyhedral geometry.
* @param hybrid_friendly
* (in) Allow hybrid geometry.
**/
DECL_KERN void check_entity_type(ENTITY const* ent, int level, int type, bool poly_supported = false, bool hybrid_friendly = false);

/**
* Checks if an entity pointer is valid and of a correct type.
* <b>Role:</b> Checks the identity of the given entity using the arrays of types and levels. 
* If the given entity is not one of the specfied types, throws an error.
* <br><br>
* <b>Errors:</b><br>
* <tt>ent</tt> is a NULL pointer or is of an incorrect type.<br>
* Functionality does not support polyhedral geometry.<br>
* Functionality does not support hybrid geometry.
* <br><br>
* @param ent
* (in) Entity pointer.
* @param n_types
* (in) Number of types.
* @param level
* (in) Entity level.
* @param type
* (in) Entity type.
* @param poly_supported
* (in) Allow polyhedral geometry.
* @param hybrid_friendly
* (in) Allow hybrid geometry.
**/
DECL_KERN void check_entity_types(ENTITY const* ent, int n_types, int* levels, int* types, bool poly_supported = false, bool hybrid_friendly = false);

/**
* Checks if a double value is within the given range.
* <br><br>
* <b>Errors:</b><br>
* <tt>value</tt> is out of the given range.
* <br><br>
* @param min
* (in) Lower range limit.
* @param max
* (in) Upper range limit.
* @param val
* (in) Test value.
* @param name
* (in) Unused.
**/
DECL_KERN void check_range(double min, double max, double val, char const *name);

/**
* Checks if a double value is non-negative.
* <br><br>
* <b>Errors:</b><br>
* <tt>value</tt> is not positive or zero.
* <br><br>
* @param val
* (in) Test value.
* @param name
* (in) Unused.
* @param tol
* (in) Tolerance value.
**/
DECL_KERN void check_non_neg_value(double val, char const *name, double tol);

/**
* Checks if a double value is non-zero.
* <br><br>
* <b>Errors:</b><br>
* <tt>value</tt> too close to zero.
* <br><br>
* @param val
* (in) Test value.
* @param name
* (in) Unused.
* @param tol
* (in) Tolerance value.
**/
DECL_KERN void check_non_zero_value(double val, char const *name, double tol);

/**
* Checks if a double value is positive.
* <b>Role:</b> Checks if the value is less than the given tolerance.
* <br><br>
* <b>Errors:</b><br>
* <tt>value</tt> is not positive.
* <br><br>
* @param val
* (in) Double value.
* @param name
* (in) Unused.
* @param tol
* (in) Tolerance value.
**/
DECL_KERN void check_pos_value(double val, char const *name, double tol);

/**
* Checks if an integer value is within the given range.
* <br><br>
* <b>Errors:</b><br>
* <tt>value</tt> is out of the given range.
* <br><br>
* @param min
* (in) Lower range limit.
* @param max
* (in) Upper range limit.
* @param val
* (in) Test value.
* @param name
* (in) Unused.
**/
DECL_KERN void check_range(int min, int max, int val, char const *name);

/**
* Checks if an integer value is non-negative.
* <br><br>
* <b>Errors:</b><br>
* <tt>value</tt> is not positive or zero.
* <br><br>
* @param val
* (in) Test value.
* @param name
* (in) Unused.
**/
DECL_KERN void check_non_neg_value(int val, char const *name);

/**
* Checks if an integer value is non-zero.
* <br><br>
* <b>Errors:</b><br>
* <tt>value</tt> is zero.
* <br><br>
* @param val
* (in) Test value.
* @param name
* (in) Unused.
**/
DECL_KERN void check_non_zero_value(int val, char const *name);

/**
* Checks if an integer value is positive.
* <b>Role:</b> Checks if the value is less than or equal to zero.
* <br><br>
* <b>Errors:</b><br>
* <tt>value</tt> is not positive.
* <br><br>
* @param val
* (in) Test value.
* @param name
* (in) Unused.
**/
DECL_KERN void check_pos_value(int val, char const *name);

/**
* Checks if two vectors are perpendicular.
* <br><br>
* <b>Errors:</b><br>
* Vectors are not perpendicular.
* <br><br>
* @param v1
* (in) First vector.
* @param v2
* (in) Second vector.
**/
DECL_KERN void check_vector_perp(const SPAvector &v1, const SPAvector &v2);

/**
* Checks if a file pointer is valid</tt>.
* <br><br>
* <b>Errors:</b><br>
* NULL file pointer given.
* <br><br>
* @param fp
* (in) File pointer.
* @param name
* (in) Unused. For future use.
**/
DECL_KERN void check_file(FILE *fp, char const *name);

// Joel Longtine(cwv): 7.26.2005
// Name collision on Mac OS X 10.4(Tiger) with 'check_string'
// check_string is a defined macro in /usr/include/AssertMacros.h
// Changed to spa_check_string to resolve.

/**
* Checks if a string pointer is valid</tt>.
* <br><br>
* <b>Errors:</b><br>
* NULL string pointer given.
* <br><br>
* @param str
* (in) String pointer.
* @param name
* (in) Unused. For future use.
**/
DECL_KERN void spa_check_string(char const *str, char const *name);

/**
* Checks if an edge has a valid geometry pointer.
* <br><br>
* <b>Errors:</b><br>
* <tt>edge</tt> has a NULL geometry pointer.<br>
* Functionality does not support polyhedral geometry.<br>
* Functionality does not support hybrid geometry.
* <br><br>
* @param edge
* (in) Edge pointer.
* @param poly_supported
* (in) Allow polyhedral geometry.
* @param hybrid_friendly
* (in) Allow hybrid geometry.
**/
DECL_KERN void check_edge_geom(EDGE const* edge, bool poly_supported = false, bool hybrid_friendly = false);

/**
* Checks if a face has a valid geometry pointer.
* <br><br>
* <b>Errors:</b><br>
* <tt>face</tt> has a NULL geometry pointer.<br>
* Functionality does not support polyhedral geometry.<br>
* Functionality does not support hybrid geometry.
* <br><br>
* @param face
* (in) Face pointer.
* @param poly_supported
* (in) Allow polyhedral geometry.
* @param hybrid_friendly
* (in) Allow hybrid geometry.
**/
DECL_KERN void check_face_geom(FACE const* face, bool poly_supported = false, bool hybrid_friendly = false);
			// error if null geometry pointer

/**
* Checks if a law pointer is valid</tt>.
* <br><br>
* <b>Errors:</b><br>
* NULL law pointer given.
* <br><br>
* @param in_law
* (in) Law pointer.
**/
DECL_KERN void check_law(law const* in_law);

/**
* Checks if a double pointer is valid</tt>.
* <br><br>
* <b>Errors:</b><br>
* NULL double pointer given.
* <br><br>
* @param d
* (in) Double pointer.
**/
DECL_KERN void check_double(double const* d);

/**
* Checks if a loop pointer is valid.
* <br><br>
* <b>Errors:</b><br>
* <tt>loop</tt> is a NULL pointer or does not point to a @href LOOP.<br>
* Functionality does not support polyhedral geometry.<br>
* Functionality does not support hybrid geometry.
* <br><br>
* @param loop
* (in) Loop pointer.
* @param poly_supported
* (in) Allow polyhedral geometry.
* @param hybrid_friendly
* (in) Allow hybrid geometry.
**/
DECL_KERN void check_loop(LOOP const* loop, bool poly_supported = false, bool hybrid_friendly = false);

/**
* Checks if a %curve pointer is valid</tt>.
* <br><br>
* <b>Errors:</b><br>
* NULL %curve pointer given.
* <br><br>
* @param crv
* (in) Curve pointer.
**/
DECL_KERN void check_curve(CURVE const* crv);

/**
* Checks if a part pointer is valid</tt>.
* <br><br>
* <b>Errors:</b><br>
* NULL part pointer given.
* <br><br>
* @param part
* (in) Part pointer.
**/
DECL_KERN void check_part(PART const* part);

/**
* Checks if a tedge pointer is valid.
* <br><br>
* <b>Errors:</b><br>
* <tt>tedge</tt> is a NULL pointer or does not point to a @href TEDGE.<br>
* Functionality does not support polyhedral geometry.<br>
* Functionality does not support hybrid geometry.
* <br><br>
* @param tedge
* (in) Tedge pointer.
* @param poly_supported
* (in) Allow polyhedral geometry.
* @param hybrid_friendly
* (in) Allow hybrid geometry.
**/
DECL_KERN void check_tedge(TEDGE const* tedge, bool poly_supported = false, bool hybrid_friendly = false);

/**
* Checks if a tcoedge pointer is valid.
* <br><br>
* <b>Errors:</b><br>
* <tt>tcoedge</tt> is a NULL pointer or does not point to a @href TCOEDGE.<br>
* Functionality does not support polyhedral geometry.<br>
* Functionality does not support hybrid geometry.
* <br><br>
* @param tcoedge
* (in) Tcoedge pointer.
* @param poly_supported
* (in) Allow polyhedral geometry.
* @param hybrid_friendly
* (in) Allow hybrid geometry.
**/
DECL_KERN void check_tcoedge(TCOEDGE const* tcoedge, bool poly_supported = false, bool hybrid_friendly = false);

/**
* Checks if a tcoedge has a valid geometry pointer.
* <br><br>
* <b>Errors:</b><br>
* <tt>tcoedge</tt> has a NULL geometry pointer.<br>
* Functionality does not support polyhedral geometry.<br>
* Functionality does not support hybrid geometry.
* <br><br>
* @param tcoedge
* (in) Tcoedge pointer.
* @param poly_supported
* (in) Allow polyhedral geometry.
* @param hybrid_friendly
* (in) Allow hybrid geometry.
**/
DECL_KERN void check_tcoedge_geom(TCOEDGE const* tcoedge, bool poly_supported = false, bool hybrid_friendly = false);

/** @} */
#endif
