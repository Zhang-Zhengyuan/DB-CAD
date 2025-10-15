/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef COVER_OPTIONS_HXX
#define COVER_OPTIONS_HXX
#include "dcl_covr.h"
#include "mmgr.hxx"

class cover_options_impl;
class surface;
class tolerize_entity_opts;
class ENTITY_LIST;
class outcome;
class AcisOptions;
class BODY;
class WIRE;

/**
 * @file cover_options.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup COVRAPI Covering APIs
 *
 * \ingroup ADVSURF Advanced Surfacing
 * @{
 */

/**
* The <tt>cover_options</tt> object is used to control behavior of the covering API functions, including: 
* @href api_cover_circuits, @href api_cover_sheet, and @href api_cover_wires.
* <br><br>
* <b>Role:</b> The user can specify an optional covering surface, an allowable gap tolerance, and options
* for making the edges and vertices tolerant after covering.  The user can also query the <tt>cover_options</tt> 
* object after a covering operation for the faces produced by the cover operation.
**/
class DECL_COVR cover_options: public ACIS_OBJECT
{
public:
/**
* Set the covering surface.
* If the user specifies the covering surface, the covering API function will use that surface to cover input loops;
* otherwise, the covering API will attempt to calculate a covering surface.
* @param surf
* desired covering surface
*/
    void set_covering_surface(const surface& surf);

/**
* Set the gap tolerance.
* If a gap tolerance is specified and is reasonable (i.e., it is greater than <tt>SPAresabs</tt>), the covering API function
* will fit a covering surface to the input loops (as long as gap is less than the gap tolerance) and tolerize the result.
* @param tol
* given gap tolerance
**/
    void set_gap_tol(double tol);

/**
* Returns the gap tolerance.  
* The default tolerance is <tt>SPAresabs</tt>. With the default tolerance the result is not tolerized.
**/
    double get_gap_tol() const;

/**
* Allows the user to customize tolerization behavior.
* The user retains ownership of the <tt>tolerize_entity_opts</tt> object: i.e.,
* the <tt>cover_options</tt> object will not delete the <tt>tolerize_entity_opts</tt>
* object it points to when it is destructed.
* @param te_opts
* the given <tt>tolerize_entity_opts</tt> object
* @see tolerize_entity_opts
**/
    void set_tolerize_entity_opts(tolerize_entity_opts* te_opts);

/**
* Returns a pointer to the <tt>tolerize_entity_opts</tt> object.
* Returns a <tt>NULL</tt> pointer unless the user has specified <tt>tolerize_entity_opts</tt>, 
* in which case a pointer to the user specified <tt>tolerize_entity_opts</tt> is returned.
**/
    tolerize_entity_opts const* get_tolerize_entity_opts() const;

/**
 * Adds pointers to the face(s) produced by the covering operation to the supplied list.  
 * Does not removing previously existing contents of the list (if any).
 * @param out_faces
 * faces produced by the covering operation
 * @see ENTITY_LIST
 **/
    void get_output_faces(ENTITY_LIST &out_faces) const;

/**
* Set the state of the allow rubber face option.
* If the user specifies that rubber faces are allowed in the output, faces with <tt>NULL</tt>
* geometry may be created if a covering surface could not be generated.
* @param allow_rubber_face
* given allow rubber face option
*/
    void set_allow_rubber_faces( logical allow_rubber_face );

/**
* Returns the state of the allow rubber face option. The default value is <tt>FALSE</tt>.
*/
    logical get_allow_rubber_faces() const;

/**
* Set the state of the fail safe mode.
* If the user specifies that the covering operation should be done in fail safe mode, then
* failures in covering connected loops of edges are ignored.
* @param failsafe
* given fail-safe option
*/
    void set_failsafe_mode( logical failsafe );

/**
* Returns the state of the fail-safe mode. The default value is <tt>TRUE</tt>.
*/
    logical get_failsafe_mode() const;

/**
* Creates a default <tt>cover_options</tt> object
**/
    cover_options();

/**
* @nodoc
**/
    cover_options_impl* get_impl();

/**
* Destroys a <tt>cover_options</tt> object
**/
    ~cover_options();

 private:
	cover_options_impl* _impl;
};
/** @} */
#endif
