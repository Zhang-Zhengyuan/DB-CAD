/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( BOOLAPI_HXX )
#define BOOLAPI_HXX
/**
 * \defgroup BOOLAPI Booleans
 * \defgroup BOOLBASICAPI Basic Operations
 * \ingroup BOOLAPI
 * \defgroup BOOLADVAPI Advanced Operations
 * \ingroup BOOLAPI
 * \defgroup BOOLPROJAPI Projection Operations
 * \ingroup BOOLAPI
 * \defgroup BOOLOPTIONSAPI Options
 * \ingroup BOOLAPI
 * \defgroup BOOLADVIMPRINT Imprint
 * \ingroup BOOLADVAPI
 * \defgroup BOOLADVSLICE Planar Slice
 * \ingroup BOOLADVAPI
 * \defgroup BOOLADVREMOVALS Wire/Edge/Face Removals
 * \ingroup BOOLADVAPI
 * \defgroup BOOLADVEFINT Edge/Face Intersections
 * \ingroup BOOLADVAPI
 * \defgroup BOOLADVEMBED Embed Edges in Faces
 * \ingroup BOOLADVAPI
 * \defgroup BOOLADVSBOOL Selective Booleans
 * \ingroup BOOLADVAPI
 * \defgroup BOOLADVGLUE Glue
 * \ingroup BOOLADVAPI
 * \defgroup BOOLADVPARTIAL Partial Booleans
 * \ingroup BOOLADVAPI
 * \defgroup BOOLADVMERGE Clean and Merge
 * \ingroup BOOLADVAPI
 * \defgroup BOOLADVCHECKSREPAIRS Checks and Repairs
 * \ingroup BOOLADVAPI
 * \defgroup BOOLADVSTITCH Stitch
 * \ingroup BOOLADVAPI
 * \defgroup BOOLADVSPLITS Splits
 * \ingroup BOOLADVAPI
 * \defgroup BOOLADVOTHER Other Operations
 * \ingroup BOOLADVAPI
 *
 */

/**
* @file boolapi.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */



class BODY;
class FACE;
class EDGE;
class WIRE;
class ENTITY_LIST;

class SPAposition;
class SPAvector;
class SPAunit_vector;
class SPAtransf;
class insanity_list;
class glue_options;

#include "dcl_bool.h"
#include "logical.h"

#include "api.hxx"
#include "exchg_funcs.hxx"

class BoolOptions;
class AcisOptions;
class unstitch_nm_options;
class convert_to_spline_options;
class project_options;

/*! \addtogroup BOOLBASICAPI
 *  \brief Declared at <boolapi.hxx>, SPAbool
 *  @{
 */

/**
 * Initializes the Boolean library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_BOOL outcome api_initialize_booleans();
/**
 * Terminates the Boolean library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
**/
DECL_BOOL outcome api_terminate_booleans();


/**
 * Specifies the type of Boolean operation.
 * @param UNION
 * union.
 * @param INTERSECTION
 * intersection.
 * @param SUBTRACTION
 * subtraction.
 * @param NONREG_UNION
 * nonregularized union.
 * @param NONREG_INTERSECTION
 * nonregularized intersection.
 * @param NONREG_SUBTRACTION
 * nonregularized subtraction.
 * @param CHOP
 * chop.
 * @param NONREG_CHOP
 * nonregularized chop.
 **/
enum BOOL_TYPE {
	UNION,
	INTERSECTION,
	SUBTRACTION,
	NONREG_UNION,
	NONREG_INTERSECTION,
	NONREG_SUBTRACTION,
    CHOP,
    NONREG_CHOP
};

// ********************************************************************
// Enumeration:
// NDBOOL_KEEP
//
// Access:
// document
//
// ********************************************************************
// Purpose:
// This enum defines options for a non-destructive Boolean operation.
// See api_boolean documentation for further information.
//

/**
* For a non-destructive Boolean operation, this optional flag may be used to specify the preservation of either or both input bodies.
* <br>
* @param NDBOOL_KEEP_NEITHER
* preserve neither body.
* @param NDBOOL_KEEP_TOOL
* preserve the tool body.
* @param NDBOOL_KEEP_BLANK
* preserve the blank body.
* @param NDBOOL_KEEP_BOTH
* preserve the tool and blank bodies.
**/
enum NDBOOL_KEEP {
  NDBOOL_KEEP_NEITHER, // (default) preserve neither body
  NDBOOL_KEEP_TOOL,    // preserve the tool body
  NDBOOL_KEEP_BLANK,   // preserve the blank body
  NDBOOL_KEEP_BOTH     // preserve the tool and blank bodies
};

/**
 * Executes a general Boolean operation.
 * <br><b>Technical Article:</b> <i>[Booleans](http://doc.spatial.com/articles/b/o/o/Component~Booleans_da65.html)</i>
 * <br><br>
 * <b>Role:</b>
 * The two bodies are combined by way of union, subtraction, or intersection.
 * <br><br>
 * By default, the result is returned in a modified blank body and the tool body is deleted.
 * However, the non-destructive Boolean argument <tt>ndbool_keep</tt>, allows the caller to 
 * preserve the blank body, tool body, or both bodies.
 * If the blank body is to be preserved, a new body is created for the result and returned
 * via the pointer <tt>result_body</tt>. The caller is responsible for passing in a NULL
 * @href BODY pointer, through which the result body is returned. For more information refer
 * to the Technical Article <i>[Non-destructive Boolean Operations](http://doc.spatial.com/articles/n/o/n/Non-destructive_Boolean_Operations_edaa.html)</i>.
 * <br><br>
 * Note that it is possible for an empty blank body to be returned, for example,
 * when intersecting two solid bodies that do not overlap.
 * <br><br>
 * Note that the result of a regularized Boolean operation between two bodies of
 * the same dimension cannot have a lower dimension. Therefore, the
 * regularized intersection between two non-coincident 2D sheet bodies is an
 * empty body, whereas the non-regularized intersection is a wire body. 
 * For more information refer to the Technical Article <i>[Regularized and Non-regularized 
 * Boolean Operations](http://doc.spatial.com/articles/r/e/g/Regularized_and_Non-regularized_Boolean_Operations_a09b.html)</i>.
 * <br><br>
 * To perform a simultaneous intersection and subtraction, the API
 * @href api_boolean_chop_body is recommended.
 * For more information refer to the Technical Article <i>[chop 
 * Operations](http://doc.spatial.com/articles/b/a/s/Basic_Boolean_Operations_4145.html#Chop)</i>.
 * <br><br>
 * <b>Errors:</b> <tt>tool</tt> or <tt>blank</tt> is a NULL pointer or
 * does not point to a @href BODY.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
 * @param tool
 * (in) Tool body, deleted by the operation (default).
 * @param blank
 * (in/out) Blank body, modified by the operation (default).
 * @param op
 * (in) Type of Boolean operation. Valid values are <tt>UNION</tt>, <tt>INTERSECTION</tt>,
 * <tt>SUBTRACTION</tt>, <tt>NONREG_UNION</tt>, <tt>NONREG_INTERSECTION</tt>, and
 * <tt>NONREG_SUBTRACTION</tt>.
 * @param ndbool_keep
 * (in) Optonal flag for non-destructive Booleans.
 * Valid values are <tt>NDBOOL_KEEP_NEITHER</tt> (default), <tt>NDBOOL_KEEP_BLANK</tt>,
 * <tt>NDBOOL_KEEP_TOOL</tt>, and <tt>NDBOOL_KEEP_BOTH</tt>.
 * @param result_body
 * (out) Used to return the result body if performing a non-destructive Boolean.
 * Default value is <tt>*(BODY**)NULL_REF</tt> if not required.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_boolean(
        BODY *tool,
        BODY *blank,
        BOOL_TYPE op,
        NDBOOL_KEEP ndbool_keep =
          NDBOOL_KEEP_NEITHER,
        BODY *&result_body =
          *(BODY**) NULL_REF,
		AcisOptions* ao =
				NULL
    );

/**
 * 示例
 */
DECL_BOOL outcome gme_api_boolean(
		BODY* tool,
		BODY* blank,
		BOOL_TYPE op,
		NDBOOL_KEEP ndbool_keep =
		NDBOOL_KEEP_NEITHER,
		BODY*& result_body =
		*(BODY**)NULL_REF,
		AcisOptions* ao =
		NULL
);

/**
 * Executes a general Boolean operation.
 * <br><b>Technical Article:</b> <i>[Booleans](http://doc.spatial.com/articles/b/o/o/Component~Booleans_da65.html)</i>
 * <br><br>
 * <b>Role:</b> 
 * The two bodies are combined by way of union, subtraction, or intersection.
 * <br><br>
 * By default, the result is returned in a modified blank body and the tool body is deleted.
 * However, the non-destructive Boolean argument <tt>ndbool_keep</tt>, allows the caller 
 * to preserve the blank body, tool body, or both bodies.
 * If the blank body is to be preserved, a new body is created for the result and returned
 * via the pointer <tt>result_body</tt>. The caller is responsible for passing in a NULL
 * @href BODY pointer, through which the result body is returned. For more information refer
 * to the Technical Article <i>[Non-destructive Boolean Operations](http://doc.spatial.com/articles/n/o/n/Non-destructive_Boolean_Operations_edaa.html)</i>.
 * <br><br>
 * The Boolean options argument, <tt>boolopts</tt>, allows the caller to pass
 * additional information to the Boolean operation. Refer to @href BoolOptions
 * for details.
 * <br><br>
 * Note that it is possible for an empty blank body to be returned, for example,
 * when intersecting two solid bodies that do not overlap.
 * <br><br>
 * Note that the result of a regularized Boolean operation between two bodies of 
 * the same dimension cannot have a lower dimension. Therefore, the
 * regularized intersection between two non-coincident 2D sheet bodies is an
 * empty body, whereas the non-regularized intersection is a wire body.
 * For more information refer to the Technical Article <i>[Regularized and Non-regularized 
 * Boolean Operations](http://doc.spatial.com/articles/r/e/g/Regularized_and_Non-regularized_Boolean_Operations_a09b.html)</i>.
 * <br><br>
 * To perform a simultaneous intersection and subtraction, the API
 * @href api_boolean_chop_body is recommended.
 * For more information refer to the Technical Article <i>[chop 
 * Operations](http://doc.spatial.com/articles/b/a/s/Basic_Boolean_Operations_4145.html#Chop)</i>.
 * <br><br>
 * <b>Errors:</b> <tt>tool</tt> or <tt>blank</tt> is a NULL pointer or does 
 * not point to a @href BODY.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
 * @param tool
 * (in) Tool body, deleted by the Boolean operation (default).
 * @param blank
 * (in/out) Blank body, modified by the operation (default).
 * @param op
 * (in) Type of Boolean operation. Valid values are <tt>UNION</tt>, <tt>INTERSECTION</tt>, 
 * <tt>SUBTRACTION</tt>, <tt>NONREG_UNION</tt>, <tt>NONREG_INTERSECTION</tt>, and 
 * <tt>NONREG_SUBTRACTION</tt>.
 * @param ndbool_keep
 * (in) Flag for non-destructive Booleans. Valid values are <tt>NDBOOL_KEEP_NEITHER</tt> (default), 
 * <tt>NDBOOL_KEEP_BLANK</tt>, <tt>NDBOOL_KEEP_TOOL</tt>, and <tt>NDBOOL_KEEP_BOTH</tt>.
 * @param result_body
 * (out) Used to return the result body if performing a non-destructive Boolean.
 * Default value is <tt>*(BODY**)NULL_REF</tt> if not required.
 * @param boolopts
 * (in) Boolean options. Refer to @href BoolOptions for details.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_boolean(
        BODY *tool,					
        BODY *blank,				
        BOOL_TYPE op,				
        NDBOOL_KEEP ndbool_keep,		
        BODY *&result_body,		
        BoolOptions *boolopts,
		AcisOptions* ao =			
				NULL				
    );

/**
 * Intersects two bodies and imprints the intersection graph on both bodies.
 * <br><b>Technical Article:</b> <i>[Imprint](http://doc.spatial.com/articles/i/m/p/Imprint.html)</i>
 * <br><br>
 * <b>Role:</b> This API function computes the intersection graph of the tool body and
 * the blank body and imprints the intersection graph on both bodies.
 * If a closed loop of edges is created, a new face is made. An open loop of edges can be 
 * added as a spur to an existing loop on a face or as a slit in the face.
 * <br><br>
 * <b><i>Note:</i></b> As a side effect of the imprint operation the tool body is transformed
 * into the coordinate system of the blank body.
 * <br><br>
 * <b>Errors:</b> <tt>tool</tt> or <tt>blank</tt> is a NULL pointer or does not point 
 * to a @href BODY.
 * <br><br>
 * <b>Effect:</b> Changes models
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param tool
 * (in/out) Tool body.
 * @param blank
 * (in/out) Blank body.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_imprint(
			BODY *tool,						// first argument body
			BODY *blank,						// second argument body
			AcisOptions* ao =			// options. It contains aditional information
				NULL					//            and version and journaling data
);
DECL_BOOL outcome gme_api_imprint(
			BODY *tool,						// first argument body
			BODY *blank,						// second argument body
			AcisOptions* ao =			// options. It contains aditional information
				NULL					//            and version and journaling data
);

/**
 * Executes a Boolean intersect operation on two bodies.
 * <br><b>Technical Article:</b> <i>[Intersect](http://doc.spatial.com/articles/b/a/s/Basic_Boolean_Operations_4145.html#Intersect)</i>
 * <br><br>
 * <b>Role:</b> This API function intersects two bodies (<tt>blank</tt> and <tt>tool</tt>). If the
 * <tt>outcome</tt> is successful, the result is returned in a modified 
 * blank body, and the tool body is deleted. The intersection is performed 
 * even if the bodies do not overlap. In this case, a body with a <tt>NULL</tt> lump is returned.
 * <br><br>
 * <b>Errors:</b> <tt>tool</tt> or <tt>blank</tt> body is a NULL pointer or does
 * not point to a @href BODY.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
 * @param tool
 * (in) Tool body, deleted by the operation.
 * @param blank
 * (in/out) Blank body, modified by the operation.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_intersect(
			BODY *tool,						// first argument body, to be discarded
			BODY *blank,						// second argument body, returns result
			AcisOptions* ao =			// options. It contains aditional information
				NULL					//            and version and journaling data
);
DECL_BOOL outcome gme_api_intersect(
			BODY *tool,						// first argument body, to be discarded
			BODY *blank,						// second argument body, returns result
			AcisOptions* ao =			// options. It contains aditional information
				NULL					//            and version and journaling data
);

/**
 * Performs a slice operation between two bodies.
 * <br><b>Technical Article:</b> <i>[Slice](http://doc.spatial.com/articles/s/l/i/Slice.html)</i>
 * <br><br>
 * <b>Role:</b> This API function computes the intersection graph between two bodies.
 * It returns a special wire body where each edge has two coedges.
 * A normal vector can be given in the special case when the tool body is a plane,
 * causing this operation to sequence the resultant wires into
 * non-overlapping loops of conventional sense. Generally, the normal should be set
 * to <tt>*(SPAunit_vector*)NULL_REF</tt>.
 * <br><br>
 * <b><i>Note:</i></b> The slice graph is not designed to be used as a general wire body. To
 * generate a wire body suitable for subsequent general use, a call to @href api_clean_wire
 * is necessary after calling this API function.
 * However, if there are no intersections between the blank and tool bodies, then the returned wire 
 * body is <tt>NULL</tt> and subsequent calls to @href api_clean_wire are not necessary, nor recommended.
 * <br><br>
 * This function is optionally <i>Failsafe</i>. In <i>Failsafe</i> mode, the API function will return as 
 * much of the result as possible. If the result is incomplete, then the API function will return success, 
 * but the returned outcome will return true if <tt>encountered_errors()</tt> is called. 
 * The caller should examine the entities linked with these errors and take any action they 
 * require to resolve the incomplete slice graph. 
 * <br><br>
 * <i>Failsafe</i> mode is enabled using the option "fss", as well as ensuring that the 
 * "careful" option is set to <tt>FALSE</tt>. Setting the "careful" option
 * to <tt>TRUE</tt> will turn off all Failsafe behavior. In non-failsafe mode the model is completely 
 * rolled back on the occurrence of the first unhandled error. 
 * <br><br>
 * <b>Errors:</b><br>
 * <tt>tool</tt> or <tt>blank</tt> is a NULL pointer or does not point to a @href BODY.<br>
 * <tt>normal</tt> has zero length.
 * <br><br>
 * <b>Effect:</b> Read-only on input. A new body is created.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param tool
 * (in) Tool, or slicing body.
 * @param blank
 * (in) Blank body to be sliced.
 * @param normal
 * (in) Normal about which wire edges at a vertex are to be ordered.
 * @param graph
 * (out) Returned slice graph.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_slice(
			BODY *tool,				// slicing body
			BODY *blank,				// body to be sliced
			const SPAunit_vector &normal,           // (optional) normal about which
								// wire edges at a vertex are to be
								// ordered.
			BODY *&graph,				// intersection graph
			AcisOptions* ao = NULL
		);
DECL_BOOL outcome gme_api_slice(
			BODY *tool,				// slicing body
			BODY *blank,				// body to be sliced
			const SPAunit_vector &normal,           // (optional) normal about which
								// wire edges at a vertex are to be
								// ordered.
			BODY *&graph,				// intersection graph
			AcisOptions* ao = NULL
		);
/**
 * Performs a slice operation between two bodies.
 * <br><b>Technical Article:</b> <i>[Slice](http://doc.spatial.com/articles/s/l/i/Slice.html)</i>
 * <br><br>
 * <b>Role:</b> This API function computes the intersection graph between two bodies.
 * It returns a special wire body where each edge has two coedges.
 * A normal vector can be given in the special case when the tool body is a plane,
 * causing this operation to sequence the resultant wires into
 * non-overlapping loops of conventional sense. Generally, the normal should be set
 * to <tt>*(SPAunit_vector*)NULL_REF</tt>.
 * <br><br>
 * <b><i>Note:</i></b> The slice graph is not designed to be used as a general wire body. To
 * generate a wire body suitable for subsequent general use, a call to @href api_clean_wire
 * is necessary after calling this API function.
 * However, if there are no intersections between the blank and tool bodies, then the returned wire 
 * body is <tt>NULL</tt> and subsequent calls to @href api_clean_wire are not necessary, nor recommended.
 * <br><br>
 * The Boolean options argument, <tt>boolopts</tt>, allows the caller to pass
 * additional information to the slice operation. Refer to @href BoolOptions
 * for more details. Note that some options (for example, the merge type) do not affect the 
 * outcome of the slice operation.
 * <br><br>
 * This function is optionally <i>Failsafe</i>. In Failsafe mode, the API function will return as much of the 
 * result as possible. If the result is incomplete, then the API function will return success, but the 
 * returned outcome will return true if <tt>encountered_errors()</tt> is called. The caller 
 * should examine the entities linked with these errors and take any action they require to 
 * resolve the incomplete slice graph.
 * <br><br>
 * Failsafe mode is enabled using the option "fss", as well as ensuring that the "careful" 
 * option is set to <tt>FALSE</tt>. Setting the "careful" option to <tt>TRUE</tt> will turn off all 
 * Failsafe behavior. In non-failsafe mode the model is completely rolled back on the occurrence 
 * of the first unhandled error. 
 * <br><br>
 * <b>Errors:</b><br>
 * <tt>tool</tt> or <tt>blank</tt> is a NULL pointer or does not point to a @href BODY.<br>
 * <tt>normal</tt> has zero length.
 * <br><br>
 * <b>Effect:</b> Read-only on input. A new body is created.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param tool
 * (in) Tool, or slicing body.
 * @param blank
 * (in) Blank body to be sliced.
 * @param normal
 * (in) Normal about which wire edges at a vertex are to be ordered.
 * @param graph
 * (out) Returned slice graph.
 * @param boolopts
 * (in) Boolean options. Refer to @href BoolOptions for details.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/

DECL_BOOL outcome api_slice(
			BODY *tool,
			BODY *blank,
			const SPAunit_vector &normal,
			BODY *&graph,
			BoolOptions *boolopts,
			AcisOptions* ao = NULL
		);

/**
 * Executes a Boolean subtract operation.
 * <br><b>Technical Article:</b> <i>[Subtract](http://doc.spatial.com/articles/b/a/s/Basic_Boolean_Operations_4145.html#Subtract)</i>
 * <br><br>
 * <b>Role:</b> This API function subtracts one body from another. If the <tt>outcome</tt> is
 * successful, the result is returned in a modified blank body and the tool body
 * is deleted.
 * <br><br>
 * <b>Errors:</b> <tt>tool</tt> or <tt>blank</tt> is a NULL pointer or does not point to a @href BODY.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
 * @param tool
 * (in) Tool body to be subtracted, deleted by the operation.
 * @param blank
 * (in/out) Blank body to be subtracted from, modified by the operation.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_subtract(
			BODY *tool,						// first argument body, to be discarded
			BODY *blank,						// second argument body, returns result
			AcisOptions* ao =			// options. It contains aditional information
			NULL						//            and version and journaling data
);
DECL_BOOL outcome gme_api_subtract(
			BODY *tool,						// first argument body, to be discarded
			BODY *blank,						// second argument body, returns result
			AcisOptions* ao =			// options. It contains aditional information
			NULL						//            and version and journaling data
);

/**
 * Executes a Boolean unite operation.
 * <br><b>Technical Article:</b> <i>[Unite](http://doc.spatial.com/articles/b/a/s/Basic_Boolean_Operations_4145.html#Unite)</i>
 * <br><br>
 * <b>Role:</b> This API function unites two bodies. If the <tt>outcome</tt> is successful, the
 * result is returned in a modified blank body and the tool body is deleted.
 * <br><br>
 * <b>Errors:</b> <tt>tool</tt> or <tt>blank</tt> is a NULL pointer or does not point to a @href BODY.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
 * @param tool
 * (in) Tool body, deleted by the operation.
 * @param blank
 * (in) Blank body, modified by the operation.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_unite(
			BODY *tool,						// first argument body, to be discarded
			BODY *blank,						// second argument body, returns result
			AcisOptions* ao =			// options. It contains aditional information
				NULL					//            and version and journaling data
);
DECL_BOOL outcome gme_api_unite(
			BODY *tool,						// first argument body, to be discarded
			BODY *blank,						// second argument body, returns result
			AcisOptions* ao =			// options. It contains aditional information
				NULL					//            and version and journaling data
);

/**
 * Intersects two bodies and imprints the intersection graph on both bodies.
 * <br><b>Technical Article:</b> <i>[Imprint](http://doc.spatial.com/articles/i/m/p/Imprint.html)</i>
 * <br><br>
 * <b>Role:</b> This API function computes the intersection graph of the tool body and
 * the blank body and imprints the intersection graph on both
 * bodies. If a closed loop of
 * edges is created, a new face is made. An open loop of edges can be added as a
 * spur to an existing loop on a face or as a slit in the face.
 * <br><br>
 * <b><i>Note:</i></b> As a side effect of the imprint operation the tool body is transformed
 * into the coordinate system of the blank body.
 * <br><br>
 * <b>Errors:</b> <tt>tool</tt> or <tt>blank</tt> is a NULL pointer or does not point to a @href BODY.
 * <br><br>
 * <b>Effect:</b> Changes models
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param tool
 * (in/out) Tool body.
 * @param blank
 * (in/out) Blank body.
 * @param bool_opts
 * (in) Boolean options. Refer to @href BoolOptions for details.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/

DECL_BOOL outcome api_imprint(
			BODY *tool,						
			BODY *blank,					
			BoolOptions *bool_opts,
			AcisOptions* ao =			
				NULL		
);
/**
 * Executes a Boolean intersect operation on two bodies.
 * <br><b>Technical Article:</b> <i>[Intersect](http://doc.spatial.com/articles/b/a/s/Basic_Boolean_Operations_4145.html#Intersect)</i>
 * <br><br>
 * <b>Role:</b> This API function intersects two bodies (<tt>blank</tt> and <tt>tool</tt>). If the
 * <tt>outcome</tt> is successful,
 * the result is returned in a modified blank body, and the tool body
 * is deleted. The intersection is performed even if the bodies do not
 * overlap. In this case, a body with a <tt>NULL</tt> lump is returned. 
 * <br><br>
 * <b>Errors:</b> <tt>tool</tt> or <tt>blank</tt> body is a NULL pointer or does
 * not point to a @href BODY.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
 * @param tool
 * (in) Tool body, deleted by the operation.
 * @param blank
 * (in/out) Blank body, modified by the operation.
 * @param boolopts
 * (in) Boolean options. Refer to @href BoolOptions for details.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_intersect(
			BODY *tool,						
			BODY *blank,					
			BoolOptions *boolopts,
			AcisOptions* ao =			
				NULL				
);
/**
 * Executes a Boolean subtract operation.
 * <br><b>Technical Article:</b> <i>[Subtract](http://doc.spatial.com/articles/b/a/s/Basic_Boolean_Operations_4145.html#Subtract)</i>
 * <br><br>
 * <b>Role:</b> This API function subtracts one body from another. If the <tt>outcome</tt> is
 * successful, the result is returned in a modified blank body and the tool body
 * is deleted.
 * <br><br>
 * <b>Errors:</b> <tt>tool</tt> or <tt>blank</tt> is a NULL pointer or does not point to a @href BODY.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
 * @param tool
 * (in) Tool body to be subtracted, deleted by the operation.
 * @param blank
 * (in/out) Blank body to be subtracted from, modified by the operation.
 * @param boolopts
 * (in) Boolean options. Refer to @href BoolOptions for details.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_subtract(
			BODY *tool,						
			BODY *blank,					
			BoolOptions *boolopts,
			AcisOptions* ao =			
			NULL				
);
/**
 * Executes a Boolean unite operation.
 * <br><b>Technical Article:</b> <i>[Unite](http://doc.spatial.com/articles/b/a/s/Basic_Boolean_Operations_4145.html#Unite)</i>
 * <br><br>
 * <b>Role:</b> This API function unites two bodies. If the <tt>outcome</tt> is successful, the
 * result is returned in a modified blank body and the tool body is deleted.
 * <br><br>
 * <b>Errors:</b> <tt>tool</tt> or <tt>blank</tt> is a NULL pointer or does not point to a @href BODY.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
 * @param tool
 * (in) Tool body, deleted by the operation.
 * @param blank
 * (in) Blank body, modified by the operation.
 * @param bool_opts
 * (in) Boolean options. Refer to @href BoolOptions for details.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_unite(
			BODY *tool,						
			BODY *blank,						
			BoolOptions *bool_opts,
			AcisOptions* ao =			
				NULL
);

/**
* Executes simultaneous Boolean intersect and subtract operations on two bodies.
* <br><b>Technical Article:</b> <i>[Chop](http://doc.spatial.com/articles/b/a/s/Basic_Boolean_Operations_4145.html#Chop)</i>
* <br><br>
* <b>Role:</b> This API function "chops" the blank body with the tool body, returning the
* body formed by intersecting the tool with the blank, and the body formed by
* subtracting the tool from the blank, simultaneously. The intersection result
* is returned via a modified blank body. A new body is created for the subtraction
* result and is returned via the pointer <tt>outside</tt>. The caller is responsible
* for passing in a NULL pointer through which the subtraction result will be returned.
* <br><br>
* If the tool body is an incomplete solid, any lumps of the blank which are not
* intersected by the faces of the tool, and which therefore cannot be classified
* as either <i>inside</i> or <i>outside</i>, will be returned in a newly created body via the
* pointer <tt>leftovers</tt>, if
* a NULL pointer is supplied. If <tt>leftovers</tt> is not supplied, any unclassified lumps
* will be deleted. The operation will fail if the tool body does not extend far enough
* to cut completely through any lump of the blank body with which its faces do intersect.
* <br><br>
* <b>Errors:</b> <tt>tool</tt> or <tt>blank</tt> is a NULL pointer or does not point to a @href BODY.
* <br><br>
* <b>Effect:</b> Changes model.
* <br><br>
* <b>Journal:</b> Available
* <br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param tool
* (in) Tool body, deleted by the chop operation (default).
* @param blank
* (in/out) Blank body to be chopped, modified to return the intersection result (default).
* @param nonreg
* (in) Set to <tt>TRUE</tt> when nonregularized results are required, <tt>FALSE</tt> otherwise.
* @param outside
* (out) Used to return the subtraction result.
* @param leftovers
* (out) Returns any unclassified lumps from the blank, or <tt>NULL</tt> if none.
* @param ndbool_keep
* (in) Optional flag for non-destructive Booleans.
* Valid values are <tt>NDBOOL_KEEP_NEITHER</tt> (default), <tt>NDBOOL_KEEP_BLANK</tt>,
* <tt>NDBOOL_KEEP_TOOL</tt> and <tt>NDBOOL_KEEP_BOTH</tt>.
* @param result_body
* (out) Used to return the intersection body if performing a non-destructive Boolean.
* Default value is <tt>*(BODY**)NULL_REF</tt> if not required.
* @param ao
* (in) ACIS options such as versioning and journaling.
**/
DECL_BOOL outcome api_boolean_chop_body (
    BODY* tool,					// consumed by the operation
    BODY* blank,				// reused to return intersection of tool with blank
    logical nonreg,				// TRUE when nonregularized results are required
    BODY*& outside,				// created to return subtraction of tool from blank
    BODY*& leftovers =			// (optional) created to return any
      *(BODY**) NULL_REF,		// unclassified lumps from the blank,
								// or NULL if there are none
    NDBOOL_KEEP ndbool_keep=				// (optional) enum for non-destructive
      NDBOOL_KEEP_NEITHER,		// Booleans
    BODY*& result_body=					// (optional) resulting body, necessary
        *(BODY**) NULL_REF,     // necessary for
                                // non-destructive Booleans
    AcisOptions* ao			// contains journal and version
            = NULL              //  information
);

/**
* Executes simultaneous Boolean intersect and subtract operations on two bodies.
* <br><b>Technical Article:</b> <i>[Chop](http://doc.spatial.com/articles/b/a/s/Basic_Boolean_Operations_4145.html#Chop)</i>
* <br>
* <b>Role:</b> This API function "chops" the blank body with the tool body, returning the
* body formed by intersecting the tool with the blank, and the body formed by
* subtracting the tool from the blank, simultaneously. The intersection result
* is returned via a modified blank body. A new body is created for the subtraction
* result and is returned via the pointer <tt>outside</tt>. The caller is responsible
* for passing in a NULL pointer through which the subtraction result will be returned.
* <br><br>
* If the tool body is an incomplete solid, any lumps of the blank which are not
* intersected by the faces of the tool, and which therefore cannot be classified
* as either <i>inside</i> or <i>outside</i>, will be returned in a newly created body via the
* pointer <tt>leftovers</tt>, if a NULL pointer is supplied. 
* The operation will fail if the tool body does not extend far enough
* to cut completely through any lump of the blank body with which its faces do intersect.
* <br><br>
* <b>Errors:</b> <tt>tool</tt> or <tt>blank</tt> is a NULL pointer or does not point to a @href BODY.
* <br><br>
* <b>Effect:</b> Changes model.
* <br><br>
* <b>Journal:</b> Available
* <br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param tool
* (in) Tool body, deleted by the chop operation (default).
* @param blank
* (in/out) Blank body to be chopped, modified to return the intersection result (default).
* @param nonreg
* (in) Set to <tt>TRUE</tt> when nonregularized results are required, <tt>FALSE</tt> otherwise.
* @param outside
* (out) Used to return the subtraction result.
* @param leftovers
* (out) Returns any unclassified lumps from the blank, or <tt>NULL</tt> if none.
* Set it to <tt>*(BODY**)NULL_REF</tt> on input if not required.
* @param ndbool_keep
* (in) Flag for non-destructive Booleans.
* Valid values are <tt>NDBOOL_KEEP_NEITHER</tt>, <tt>NDBOOL_KEEP_BLANK</tt>,
* <tt>NDBOOL_KEEP_TOOL</tt> and <tt>NDBOOL_KEEP_BOTH</tt>.
* @param result_body
* (out) Used to return the intersection body if performing a non-destructive Boolean.
* Set it to <tt>*(BODY**)NULL_REF</tt> on input if not required.
* @param boolopts
* (in) Boolean options. Refer to @href BoolOptions for details.
* @param ao
* (in) ACIS options such as versioning and journaling.
**/
DECL_BOOL outcome api_boolean_chop_body ( 
    BODY* tool,               // consumed by the operation
    BODY* blank,              // reused to return intersection of tool with blank
    logical nonreg,           // TRUE when nonregularized results are required
    BODY*& outside,           // created to return subtraction of tool from blank
    BODY*& leftovers,         // created to return any 
                              //  unclassified lumps from the blank, 
                              //  or NULL if there are none
    NDBOOL_KEEP ndbool_keep,  // flag for non-destructive Booleans
    BODY*& result_body,       // result body, necessary for
                              // non-destructive Booleans
    BoolOptions *boolopts,
	AcisOptions* ao = NULL
    );


/*! @} */

/*! \addtogroup BOOLADVIMPRINT
 *  \brief Declared at <boolapi.hxx>, SPAbool
 *  @{
 */

/**
 * Creates and imprints silhouette edges tangent to shadow cast by draft parameters.
 * <br><b>Technical Article:</b> <i>[Imprint Shadow Edges](http://doc.spatial.com/articles/i/m/p/Imprint_Shadow_Edges_4e5f.html)</i>
 * <br><br>
 * <b>Role:</b> Given a list of faces, imprints (optionally) the silhouettes
 * defined by the shadow cast by the draft parameters.
 * The silhouette is defined by the tangential field between the face 
 * %surface, the draft direction, and the draft angle. The angle is measured
 * from the draft direction. The angle between the draft direction and face normal
 * at the silhouette point is the complement of the draft angle. 
 * The resulting edges are output in a list of entities which contain zero to many. 
 * The <tt>imprint_silhouettes</tt> flag, if set to <tt>FALSE</tt>, will return all 
 * the silhouette curves, embedded in independent edges and trimmed to the boundaries of the face.
 * <br><br>
 * Note that the face sense plays an important role in determining the
 * silhouette curves. Because there maybe ambiguous solutions on open bodies, the algorithm
 * picks the closest solution to a point of view computed with the up direction.
 * <br><br>
 * <b>Topology Changes:</b>
 * <br>
 * If the <tt>imprint_silhouettes</tt> flag is <tt>TRUE</tt> the operation will result in a topology change, 
 * as the new silhouette edges, if any, split the input faces.
 * If the <tt>imprint_silhouettes</tt> option is <tt>FALSE</tt>, the silhouette edges are computed and returned 
 * as a list of stand alone <tt>EDGE</tt> entities.
 * <br><br>
 * <b>Geometry Changes:</b>
 * <br>
 * The new edges will contain new curve geometry.
 * <br><br>
 * <b>Errors:</b> 
 * Silhouette edges cannot be created on planar faces. If no silhouette edges are created,
 * <tt>NO_SILHOUETTES</tt> error is generated.
 * <br><br>
 * <b>Effect:</b> Changes model if imprint flag is <tt>TRUE</tt>.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param input_faces
 * List of faces.
 * @param draft_dir
 * Draft direction.
 * @param draft_angle
 * Draft angle.
 * @param silhouette_edges
 * Output list of created silhouette edges.
 * @param imprint_silhouettes
 * Specify if the silhouette curves should be imprinted; default is <tt>TRUE</tt>.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/

DECL_BOOL outcome api_imprint_shadow_edges(ENTITY_LIST&			input_faces,
										const SPAunit_vector&	draft_dir,
										const double&			draft_angle,
										ENTITY_LIST&			silhouette_edges,
										const logical&			imprint_silhouettes = TRUE,
                                        AcisOptions*			ao = NULL);


/*! @} */

/*! \addtogroup BOOLADVSLICE
 *  \brief Declared at <boolapi.hxx>, SPAbool
 *  @{
 */

/**
 * Slices a body with a %plane.
 * <br><br>
 * <b>Role:</b> This API function creates a wire body suitable for general use,
 * corresponding to the intersection of the specified body with the
 * %plane defined by the specified position and
 * normal vector.
 * <br><br>
 * <b><i>Note:</i></b> If there is no intersection, the returned wire body will be <tt>NULL</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only. A new body is created.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param ent
 * (in) Body to slice.
 * @param pt
 * (in) Position on %plane.
 * @param normal
 * (in) Plane normal vector.
 * @param slice
 * (out) Resulting wire body.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_planar_slice(
    BODY* ent,                      // BODY to slice
    const SPAposition& pt,            // SPAposition on plane
    const SPAunit_vector& normal,         // plane normal SPAvector
    BODY*& slice,                     // WIRE BODY from slice
    AcisOptions* ao				// contains journal and version
        = NULL                  //  information
);

/**
 * Slices a body with a %plane.
 * <br><br>
 * <b>Role:</b> This API function creates a wire body suitable for general use,
 * corresponding to the intersection of the specified body with the
 * %plane defined by the specified position and normal vector.
 * <br><br>
 * <b><i>Note:</i></b> If there is no intersection, the returned wire body will be <tt>NULL</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only. A new body is created.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param ent
 * (in) Body to slice.
 * @param pt
 * (in) Position on %plane.
 * @param normal
 * (in) Plane normal vector.
 * @param slice
 * (out) Resulting wire body.
 * @param bool_opts
 * (in) Boolean options. Refer to @href BoolOptions for details.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_planar_slice(
    BODY* ent,                      
    const SPAposition& pt,            
    const SPAunit_vector& normal,        
    BODY*& slice,                    
	BoolOptions *bool_opts,
    AcisOptions* ao				
        = NULL           
);

/*! @} */

/*! \addtogroup BOOLADVEMBED
 *  \brief Declared at <boolapi.hxx>, SPAbool
 *  @{
 */


/**
 * Scribes an edge in the faces of another body or a wire body.
 * <br><b>Technical Article:</b> <i>[Embedding an Edge in One or More Faces ]
   (http://doc.spatial.com/articles/a/d/d/Additional_Boolean_Operations_b537.html#Embedding_an_Edge_in_One_or_More_Faces)</i>
 * <br><br>
 * <b>Role:</b> This API function scribes the given edge in the faces of another body or a wire body.
 * Note the caller is promising that the edge lies exactly in the faces of the other body
 * to within the specified tolerance. Thus, if the edge meets the interior of a face,
 * then the edge must be coincident with the %surface of that face to within the specified tolerance.
 * Any edges intended to split a face must intersect with the face edges to within the specified
 * tolerance.
 * <br><br>
 * If the edge is not coincident everywhere with the other body to within the specified
 * tolerance, the results are undefined. A tolerance of less than <tt>SPAresabs</tt> will be ignored
 * and treated as <tt>SPAresabs</tt>.
 * If a tolerance greater than <tt>SPAresabs</tt> is specified, tolerant geometry may be introduced.
 * <br><br>
 * This API function is designed to be faster than the general function @href api_boolean.
 * This is because it relies on the coincidence of the edge to perform
 * fewer intersections. It is therefore essential that this API function is used correctly and the input
 * is accurate.
 * <br><br>
 * If scribing an edge onto a wire where the edge meets end to end with the wire,
 * the argument <tt>split_on_intersection</tt>
 * may be set to <tt>FALSE</tt> and there will be a marked increase in performance. The argument
 * <tt>split_on_intersection</tt> has no effect on the scribe on faces.
 * <br><br>
 * <b>Errors:</b> <tt>entity</tt> or <tt>edge</tt> is a NULL pointer.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param entity
 * (in/out) Solid body, face or wire body.
 * @param edge
 * (in) Edge to scribe onto the body, face, or wire.
 * @param tol
 * (in) User-defined tolerance for coincidence between the edge and the body.
 * @param split_on_intersections
 * (in) Set to <tt>FALSE</tt> if wire edges do not need to be split.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/

DECL_BOOL outcome api_scribe(
                      ENTITY*	entity,
                      EDGE*		edge,
					  double tol = -1,
					  logical split_on_intersections = TRUE,
                      AcisOptions* ao = NULL
	);



/*! @} */

/*! \addtogroup BOOLADVREMOVALS
 *  \brief Declared at <boolapi.hxx>, SPAbool
 *  @{
 */


/**
 * Uncovers a face, leaving behind its edges.
 * <br><b>Technical Article:</b> <i>[Uncovering Faces] (http://doc.spatial.com/articles/a/d/d/Additional_Boolean_Operations_b537.html#Uncovering_Faces)</i>
 * <br><br>
 * <b>Role:</b> This API function uncovers a face, thereby changing a closed
 * set of faces into an open set, or diminishing an already open set
 * of faces. The function may leave a partly covered body or a wire body.
 * <br><br>
 * This API function differs from @href api_remove_face because it retains all
 * edges of the face, converting them
 * to wires if necessary. In that case, the sole coedge is retained
 * and its owner is changed to point to the newly created wire.
 * For this reason, the given face must be owned by a body.
 * <br><br>
 * <b>Errors:</b> <tt>face</tt> is a NULL pointer or does not point to a @href FACE.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param face
 * (in) Face to be uncovered, deleted by the operation.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_uncover_face(
			FACE *face,						// face to be uncovered
			AcisOptions* ao =			// options. It contains aditional information
				NULL					//            and version and journaling data
);

/**
 * Removes a wire edge from a body.
 * <br><b>Technical Article:</b> <i>[Removing Faces or Wire Edges] (http://doc.spatial.com/articles/a/d/d/Additional_Boolean_Operations_b537.html#Removing_Faces_or_Wire_Edges)</i>
 * <br><br>
 * <b>Role:</b> This API function removes a wire edge from its owning body and
 * repairs the topology of the owning body.
 * <br><br>
 * The wire edge is deleted. To retain a copy of the wire edge 
 * @href api_unhook_wire_edge is recommended.
 * <br><br>
 * <b>Errors:</b> <tt>given_edge</tt> is a NULL pointer or does not point to a wire edge.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param given_edge
 * Wire edge to be removed from owning body, deleted by the operation.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_remove_wire_edge(
			EDGE *given_edge,					// EDGE to be removed
			AcisOptions* ao = NULL
		);
/**
 * Removes a face from a body.
 * <br><b>Technical Article:</b> <i>[Removing Faces or Wire Edges](http://doc.spatial.com/articles/a/d/d/Additional_Boolean_Operations_b537.html#Removing_Faces_or_Wire_Edges)</i>
 * <br><br>
 * <b>Role:</b> This API function removes a face from its owning body. When edges and
 * vertices are no longer needed to support faces, they are removed.
 * This behavior differs from that of @href api_uncover_face, which converts redundant
 * edges to wire edges.
 * <br><br>
 * The removed face is deleted. To retain a copy of the face 
 * @href api_unhook_face is recommended.
 * <br><br>
 * <b>Errors:</b> <tt>given_face</tt> is a NULL pointer or does not point to a @href FACE.
 * <br><br>
 * <b>Limitations:</b> Works only on faces that belong to a body.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param given_face
 * (in) Face to be removed from owning body, deleted by the operation.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_remove_face(
			FACE *given_face,					// FACE to be removed
			AcisOptions* ao = NULL
		);
DECL_BOOL outcome gme_api_remove_face(
			FACE *given_face,					// FACE to be removed
			AcisOptions* ao = NULL
		);

// Unhooks a wire edge from a body, placing wire in a returned body.
/**
 * Removes a wire edge from a body and returns a copy in a new wire body.
 * <br><b>Technical Article:</b> <i>[Unhooking Faces or Wire Edges](http://doc.spatial.com/articles/a/d/d/Additional_Boolean_Operations_b537.html#Unhooking_Faces_or_Wire_Edges)</i>
 * <br><br>
 * <b>Role:</b> This API function removes a wire edge from its owning body and
 * repairs the topology in the owning body. A copy of the wire edge is
 * placed in a new wire body and returned via the
 * argument <tt>unhooked_wire_body</tt>. The original wire edge is
 * deleted.
 * <br><br>
 * If a new wire body is not required @href api_remove_wire_edge
 * is recommended.
 * <br><br>
 * <b>Errors:</b> <tt>given_edge</tt> is a NULL pointer or does not point to a wire edge.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param given_edge
 * (in) Edge to unhook, deleted by the operation.
 * @param unhooked_wire_body
 * (out) New body containing copied edge is returned.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_unhook_wire_edge(
			EDGE *given_edge,					// EDGE to unhook
			BODY *&unhooked_wire_body,					// BODY containing unhooked EDGE
			AcisOptions* ao = NULL
		);
/**
 * Removes a face from a body and returns a copy in a new body.
 * <br><b>Technical Article:</b> <i>[Unhooking Faces or Wire Edges](http://doc.spatial.com/articles/a/d/d/Additional_Boolean_Operations_b537.html#Unhooking_Faces_or_Wire_Edges)</i>
 * <br><br>
 * <b>Role:</b> This API function removes a face from its owning body and puts a copy
 * into a new body. When edges and vertices are no longer needed to support faces,
 * they are also removed. The original face is deleted.
 * <br><br>
 * If a new body is not required @href api_remove_face is recommended.
 * To unhook several faces simultaneously @href api_unhook_faces is recommended.
 * <br><br>
 * <b>Errors:</b> <tt>given_face</tt> is a NULL pointer or does not point to a <tt>FACE</tt>.
 * <br><br>
 * <b>Limitations:</b> Works only on faces that belong to a body.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
 * @param given_face
 * (in) Face to be unhooked, deleted by the operation.
 * @param unhooked_face_body
 * (out) New body containing copied face is returned.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_unhook_face(
			FACE *given_face,					// FACE to unhook
			BODY *&unhooked_face_body,				// BODY containing unhooked FACE
			AcisOptions* ao = NULL
		);

/**
 * Returns copies of selected faces in a new body and, optionally, removes them
 * from the original body.
 * <br><b>Technical Article:</b> <i>[Unhooking Faces or Wire Edges](http://doc.spatial.com/articles/a/d/d/Additional_Boolean_Operations_b537.html#Unhooking_Faces_or_Wire_Edges)</i>
 * <br><br>
 * <b>Role:</b> This API function creates copies of each face in the list <tt>given_faces</tt>,
 * and returns them in a new body, added to the list <tt>unhooked_bodies</tt>. Every
 * input face must belong to the same body.
 * <br><br>
 * If the argument <tt>copy</tt> is set to <tt>TRUE</tt>, the original model is not modified.
 * If <tt>copy</tt> is set to <tt>FALSE</tt>, then the input faces are removed from their owning body
 * and deleted.
 * Edges and vertices that are no longer needed to support faces also removed.
 * This behavior differs from that of @href api_uncover_face, which converts redundant
 * edges to wire edges.
 * <br><br>
 * <b>Limitations:</b> This API function works only on faces that belong to the same owning body.
 * <br><br>
 * <b>Effect:</b> Changes model, unless <tt>copy</tt> is set to <tt>TRUE</tt>.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
 * @param given_faces
 * (in) List of faces to be unhooked.
 * @param copy
 * (in) Set to <tt>TRUE</tt> to copy faces only. Set to <tt>FALSE</tt> to copy and remove faces.
 * @param unhooked_bodies
 * (in/out) New body containing copied faces is added.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_unhook_faces(
			ENTITY_LIST	given_faces,			// FACEs to unhook
			logical copy,						// Copy faces
			ENTITY_LIST	&unhooked_bodies,		// BODYies containing unhooked FACE
			AcisOptions* ao = NULL
		);

/*! @} */

/*! \addtogroup BOOLADVEFINT
 *  \brief Declared at <boolapi.hxx>, SPAbool
 *  @{
 */


/**
 * Determines the intersection between two faces.
 * <br><br>
 * <b>Role:</b> This API function computes the intersection between two faces and returns
 * the result as a wire body suitable for general use.
 * <br><br>
 * <b><i>Note:</i></b> Prior to ACIS 7.0 this function returned a specialized wire body 
 * (an intersection graph) containing extra coedges and Boolean attributes. Since ACIS 7.0 
 * this function has returned a "cleaned" wire body.
 * <br><br>
 * <b>Errors:</b> <tt>tool</tt> or <tt>blank</tt> is a NULL pointer or does not point to
 * a @href FACE.
 * <br><br>
 * <b>Effect:</b> Read-only on input. A new body is created.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param tool
 * (in) Tool face.
 * @param blank
 * (in) Blank face.
 * @param graph
 * (out) Returned intersection wire body.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_fafa_int(
			FACE *tool,                 // Tool face
			FACE *blank,                 // Blank face
			BODY *&graph,                 // Intersection graph returned
			AcisOptions* ao          // contains journal and version
				 = NULL                 //  information
);

/**
 * Computes the intersections between the given edge and the given face.
 * <br><br>
 * <b>Role:</b> This API function computes the intersections of a given edge with a given
 * face. Then, it determines the containment of the intersections and constructs the edges
 * or vertices, depending on whether the intersections are isolated or coincident.
 * It returns the result in an <tt>entity_list</tt> as edges and vertices.
 * <br><br>
 * <b>Errors:</b> Pointer to edge is <tt>NULL</tt> or not to an <tt>EDGE</tt>.<br>
 * Pointer to face is <tt>NULL</tt> or not to a <tt>FACE</tt>.
 * <br><br>
 * <b>Limitations:</b> Not Available
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param edge
 * The given edge.
 * @param face
 * The given face.
 * @param inter
 * The intersection returned as an entity list.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/

DECL_BOOL outcome api_edfa_int(
			EDGE *edge,					 // Tool edge
			FACE *face,					 // Blank face
			ENTITY_LIST 	*&inter,     // Intersection graph returned
			AcisOptions* ao              // contains journal and version
				 = NULL                  //  information
);
/*
 * @brief 求出给定EDGE和FACE的求交结果
 * @return API运行的结果
 * @param edge 
 * 给定的边
 * @param face 
 * 给定的面
 * @param inter 
 * 存储求交结果元素的链表
 * @param ao 
 * acis属性
 */
DECL_BOOL outcome gme_api_edfa_int(
	EDGE* edge,					 // Tool edge
	FACE* face,					 // Blank face
	ENTITY_LIST*& inter,     // Intersection graph returned
	AcisOptions* ao              // contains journal and version
	= NULL                  //  information
);
/**
 * Determines the intersection between two faces.
 * <br><br>
 * <b>Role:</b> This API function computes the intersection between two faces and returns
 * the result as a wire body suitable for general use.
 * <br><br>
 * <b><i>Note:</i></b> Prior to ACIS 7.0 this function returned a specialized wire body 
 * (an intersection graph) containing extra coedges and Boolean attributes. Since ACIS 7.0 
 * this function has returned a "cleaned" wire body.
 * <br><br>
 * <b>Errors:</b> <tt>tool</tt> or <tt>blank</tt> is a NULL pointer or does not point to
 * a @href FACE.
 * <br><br>
 * <b>Effect:</b> Read-only on input. A new body is created.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param tool
 * (in) Tool face.
 * @param blank
 * (in) Blank face.
 * @param graph
 * (out) Returned intersection wire body.
 * @param boolopts
 * (in) Boolean options. Refer to @href BoolOptions for details.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_fafa_int(
			FACE *tool,                
			FACE *blank,                 
			BODY *&graph,                
	        BoolOptions *bool_opts,
			AcisOptions* ao = NULL               
);
/**
 * Computes the intersections between the given edge and the given face.
 * <br><br>
 * <b>Role:</b> This API function computes the intersections of a given edge with a given
 * face. Then, it determines the containment of the intersections and constructs the edges
 * or vertices, depending on whether the intersections are isolated or coincident.
 * It returns the result in an <tt>entity_list</tt> as edges and vertices.
 * <br><br>
 * <b>Errors:</b> Pointer to edge is <tt>NULL</tt> or not to an <tt>EDGE</tt>.<br>
 * Pointer to face is <tt>NULL</tt> or not to a <tt>FACE</tt>.
 * <br><br>
 * <b>Limitations:</b> Not Available
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param edge
 * The given edge.
 * @param face
 * The given face.
 * @param inter
 * The intersection returned as an entity list.
 * @param boolopts
 * (in) Boolean options. Refer to @href BoolOptions for details.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/

DECL_BOOL outcome api_edfa_int(
			EDGE *edge,					 
			FACE *face,					 
			ENTITY_LIST 	*&inter,     
	        BoolOptions *bool_opts,
			AcisOptions* ao = NULL                 
);

/*! @} */

/*! \addtogroup BOOLADVEMBED
 *  \brief Declared at <boolapi.hxx>, SPAbool
 *  @{
 */

/**
 * Embeds a coincident wire in the faces of another body.
 * <br><b>Technical Article:</b> <i>[Embedding Wires in Faces](http://doc.spatial.com/articles/a/d/d/Additional_Boolean_Operations_b537.html#Embedding_Wires_in_Faces)</i>
 * <br><br>
 * <b>Role:</b> This API function embeds the edges of the given wire body in the faces of another body.
 * Note the caller is promising that the wire edges lies in the faces of the other body
 * to within the specified tolerance. Thus, if a wire edge meets the interior of a face,
 * then the edge must be coincident with the surface of that face to within the specified tolerance.
 * Edges intended to split a face must intersect with the face edges to within the specified
 * tolerance.
 * <br><br>
 * If the wire edge is not coincident everywhere with the other body to within the specified
 * tolerance, the results are undefined. A tolerance of less than <tt>SPAresabs</tt> will be ignored
 * and treated as <tt>SPAresabs</tt>.
 * If a tolerance greater than <tt>SPAresabs</tt> is specified, tolerant geometry may be introduced.
 * <br><br> 
 * A wire body consisting of a degenerate coedge/edge with a single vertex may not be 
 * imprinted nor embedded onto a sheet body. 
 * <br><br>
 * If <tt>face_list</tt> is <tt>NULL</tt>, then it is assumed that each edge of the wire is coincident with
 * <i>every</i> face that it contacts. If the wire does come into contact with
 * faces that should not be involved in the embedding, then the caller must supply a list excluding
 * those faces.
 * Particular care should be taken when a wire is coincident up to the boundary of one face but
 * it is not coincident with the adjacent face.
 * <br><br>
 * This API function is designed to be faster than the more general @href api_imprint.
 * This is because it relies on the coincidence of the wire to perform
 * fewer intersections. It is, therefore, essential that this API function is used correctly and the input
 * is accurate.
 * <br><br>
 * <b>Example:</b> This example embeds a "square" wire in (a face of) a block.
 * <pre>
 *  outcome result;
 *  BODY *wire;
 *  BODY *body;
 * 
 *  SPAposition p1(10, -30, 10);
 *  SPAposition p2(10, -30, -10);
 *  SPAposition p3(-10, -30, -10);
 *  SPAposition p4(-10, -30, 10);
 *  SPAposition points[5] = {p1,p2,p3,p4,p1};
 *  result = api_make_wire(NULL, 5, points, wire);
 * 
 *  SPAposition q1(-30, -30, -30);
 *  SPAposition q2(30, 30, 30);
 *  result = api_solid_block(q1, q2, body);
 * 
 *  result = api_embed_wire_in_faces(wire, body);
 * </pre>
 * <b>Errors:</b> <tt>wire</tt> or <tt>body</tt> is a NULL pointer or does not point to a @href BODY.
 * <br><br>
 * <b>Effect:</b> Changes models. Note that we actually imprint the intersection graph
 * on both entities, so the given wire might, for example, acquire extra vertices where
 * it crosses face boundaries. If a tolerance is used, tolerant geometry may be introduced.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param wire
 * (in/out) Wire body.
 * @param body
 * (in/out) Body in which to embed the wire.
 * @param face_list
 * (in) Set of faces in which to embed the wire, <tt>NULL</tt> implies all faces will be considered.
 * @param tol
 * (in) User-defined tolerance for coincidence between the wire and the body.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_embed_wire_in_faces(
                      BODY*	wire,
                      BODY*	body,
                      ENTITY_LIST *face_list = NULL,
                      double tol = -1,
                      AcisOptions* ao = NULL
	);

DECL_BOOL outcome gme_api_embed_wire_in_faces(
                      BODY*	wire,
                      BODY*	body,
                      ENTITY_LIST *face_list = NULL,
                      double tol = -1,
                      AcisOptions* ao = NULL
	);

/*! @} */

/*! \addtogroup BOOLADVGLUE
 *  \brief Declared at <boolapi.hxx>, SPAbool
 *  @{
 */

/**
 * Executes a performance-enhanced Boolean operation, where the intersection
 * graph is known to lie along a set of coincident faces.
 * <br><b>Technical Article:</b> <i>[Glue](http://doc.spatial.com/articles/g/l/u/Glue.html)</i>
 * <br><br>
 * <b>Role:</b> This API function performs a Boolean unite or subtract operation on two bodies
 * that do not penetrate each other; that is, the intersection graph of the bodies lies
 * precisely along a set of (overlapping) coincident faces. This API function is designed
 * to increase performance over the more general @href api_boolean.
 * <br><br>
 * Two faces are <i>coincident</i> if the intersection of their interior point sets is
 * non-empty and bounded by the edges of either face, and on this overlap their
 * %surface geometries are coincident.
 * <br><br>
 * The glue operation will perform only those face-face intersections deemed
 * necessary by the lists of pairwise coincident faces specified in the argument
 * <tt>glue_opts</tt>. There will be no verification that each pair of faces is
 * indeed coincident and it is, therefore, essential that these lists are accurate
 * and complete.
 * <br><br>
 * Refer to the documentation on @href glue_options for information on how to set up information
 * and options for glue. The options consist of flags that can be set to improve
 * performance. It is important that the information provided is accurate, because the
 * glue operation will rely heavily on this information.
 * <br><br>
 * By default, the result is returned in a modified blank body and the tool body is deleted.
 * However, the non-destructive Boolean
 * argument <tt>ndbool_keep</tt>, allows the caller to preserve the blank body, tool body,
 * or both bodies.
 * If the blank body is to be preserved, a new body is created for the result and returned
 * via the pointer <tt>result_body</tt>. The caller is responsible for passing in a NULL
 * @href BODY pointer, through which the resultant body is returned. For more information refer
 * to the Technical Article <i>[Non-destructive Boolean Operations](http://doc.spatial.com/articles/n/o/n/Non-destructive_Boolean_Operations_edaa.html)</i>.
 * <br><br>
 * <b>Errors:</b> <tt>tool</tt> or <tt>blank</tt> is a NULL pointer or does not point to a @href BODY.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param tool
 * (in) Tool body, deleted by the operation (default).
 * @param blank
 * (in/out) Blank body, used to return the result (default).
 * @param bool_type
 * (out) Type of glue operation. Valid values are <tt>UNION</tt>, <tt>SUBTRACTION</tt> and <tt>NONREG_UNION</tt>.
 * @param glue_opts
 * (in) Glue info and options.
 * @param ndbool_keep
 * (in) Optional flag for non-destructive Booleans.
 * Valid values are <tt>NDBOOL_KEEP_NEITHER</tt> (default), <tt>NDBOOL_KEEP_BLANK</tt>, 
 * <tt>NDBOOL_KEEP_TOOL</tt>, and <tt>NDBOOL_KEEP_BOTH</tt>.
 * @param result_body
 * (out) Used to return the result body if performing a non-destructive Boolean.
 * Default value is <tt>*(BODY**)NULL_REF</tt> if not required.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_boolean_glue( 
	BODY *tool,				// first argument body, to be discarded
    BODY *blank,			// second argument body, returns result
	BOOL_TYPE bool_type,	// UNION, SUBTRACTION or NONREG_UNION only
	const glue_options *glue_opts,
	NDBOOL_KEEP ndbool_keep = NDBOOL_KEEP_NEITHER,  // (optional) enum for non-destructive Booleans
	BODY* &result_body = *(BODY**) NULL_REF,     // (optional) resulting body, necessary for non-destructive Booleans
	AcisOptions *ao = NULL);


/**
 * Executes a performance-enhanced Boolean operation, where the intersection
 * graph is known to lie along a set of coincident faces.
 * <br><b>Technical Article:</b> <i>[Glue](http://doc.spatial.com/articles/g/l/u/Glue.html)</i>
 * <br><br>
 * <b>Role:</b> This API function performs a Boolean unite or subtract operation on two bodies
 * that do not penetrate each other; that is, the intersection graph of the bodies lies
 * precisely along a set of (overlapping) coincident faces. This API function is designed
 * to increase performance over the more general @href api_boolean.
 * <br><br>
 * Two faces are <i>coincident</i> if the intersection of their interior point sets is
 * non-empty and bounded by the edges of either face, and on this overlap their
 * %surface geometries are coincident.
 * <br><br>
 * The glue operation will perform only those face-face intersections deemed
 * necessary by the lists of pairwise coincident faces specified in the argument
 * <tt>glue_opts</tt>. There will be no verification that each pair of faces is
 * indeed coincident and it is, therefore, essential that these lists are accurate
 * and complete.
 * <br><br>
 * Refer to the documentation on @href glue_options for information on how to set up information
 * and options for glue. The options consist of flags that can be set to improve
 * performance. It is important that the information provided is accurate, because the
 * glue operation will rely heavily on this information.
 * <br><br>
 * The Boolean options argument, <tt>boolopts</tt>, allows the caller to pass
 * additional information to the Boolean operation. Refer to @href BoolOptions
 * for details.
 * <br><br>
 * By default, the result is returned in a modified blank body and the tool body is deleted.
 * However, the non-destructive Boolean
 * argument <tt>ndbool_keep</tt>, allows the caller to preserve the blank body, tool body,
 * or both bodies.
 * If the blank body is to be preserved, a new body is created for the result and returned
 * via the pointer <tt>result_body</tt>. The caller is responsible for passing in a NULL
 * @href BODY pointer, through which the resultant body is returned. For more information refer
 * to the Technical Article <i>[Non-destructive Boolean Operations](http://doc.spatial.com/articles/n/o/n/Non-destructive_Boolean_Operations_edaa.html)</i>.
 * <br><br>
 * <b>Errors:</b> <tt>tool</tt> or <tt>blank</tt> is a NULL pointer or does not point to a @href BODY.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param tool
 * (in) Tool body, deleted by the operation (default).
 * @param blank
 * (in/out) Blank body, used to return the result (default).
 * @param bool_type
 * (out) Type of glue operation. Valid values are <tt>UNION</tt>, <tt>SUBTRACTION</tt> and <tt>NONREG_UNION</tt>.
 * @param glue_opts
 * (in) Glue info and options.
 * @param boolopts
 * (in) Boolean options. Refer to @href BoolOptions for details.
 * @param ndbool_keep
 * (in) Optional flag for non-destructive Booleans.
 * Valid values are <tt>NDBOOL_KEEP_NEITHER</tt> (default), <tt>NDBOOL_KEEP_BLANK</tt>,
 * <tt>NDBOOL_KEEP_TOOL</tt>, and <tt>NDBOOL_KEEP_BOTH</tt>.
 * @param result_body
 * (out) Used to return the result body if performing a non-destructive Boolean.
 * Default value is <tt>*(BODY**)NULL_REF</tt> if not required.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
    DECL_BOOL outcome api_boolean_glue( 
	BODY *tool,				// first argument body, to be discarded
    BODY *blank,			// second argument body, returns result
	BOOL_TYPE bool_type,	// UNION, SUBTRACTION or NONREG_UNION only
	const glue_options *glue_opts,
    BoolOptions *boolopts,
	NDBOOL_KEEP ndbool_keep = NDBOOL_KEEP_NEITHER,  // (optional) enum for non-destructive Booleans
	BODY* &result_body = *(BODY**) NULL_REF,     // (optional) resulting body, necessary for non-destructive Booleans
	AcisOptions *ao = NULL);


/*! @} */

/*! \addtogroup BOOLADVPARTIAL
 *  \brief Declared at <boolapi.hxx>, SPAbool
 *  @{
 */


/**
 * This is a partial Boolean API function that completes a chop operation.
 * <br><b>Technical Articles:</b> <i>[Partial Booleans](http://doc.spatial.com/articles/p/a/r/Partial_Booleans_8b31.html)</i>, 
 *						      <i>[Chop](http://doc.spatial.com/articles/b/a/s/Basic_Boolean_Operations_4145.html#Chop)</i>
 * <br><br>
 * <b>Role:</b> This API function completes simultaneous Boolean intersect and subtract operations
 * on two bodies (blank and tool).
 * It is assumed that the intersection graph has already been computed using other
 * partial Boolean APIs. The intersection result
 * is returned via a modified blank body. A new body is created for the subtraction
 * result and is returned via the pointer <tt>outside</tt>. The caller is responsible
 * for passing in a <tt>NULL</tt> pointer through which the subtraction result will be returned.
 * <br><br>
 * If the tool body is an incomplete solid, any lumps of the blank which are not
 * intersected by the faces of the tool, and which therefore cannot be classified
 * as either <i>inside</i> or <i>outside</i>, will be returned in a newly created body
 * via the pointer <tt>leftovers</tt>, if a <tt>NULL</tt> pointer is supplied. 
 * If <tt>leftovers</tt> is not supplied, any unclassified
 * lumps will be deleted. The operation will fail if the tool body does not extend far
 * enough to cut completely through any lump of the blank body with which its faces do
 * intersect.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param nonreg
 * (in) Set to <tt>TRUE</tt> when nonregularized results are required, <tt>FALSE</tt> otherwise.
 * @param outside
 * (out) Used to return the subtraction of the tool body from the blank body.
 * @param leftovers
 * (out) Used to return any unclassified lumps from the blank body, or <tt>NULL</tt> if none.
 * @param ndbool_keep
 * (in) Optional flag for non-destructive Booleans.
 * Valid values are <tt>NDBOOL_KEEP_NEITHER</tt> (default), <tt>NDBOOL_KEEP_BLANK</tt>,
 * <tt>NDBOOL_KEEP_TOOL</tt>, and <tt>NDBOOL_KEEP_BOTH</tt>.
 * @param result_body
 * (out) Used to return the intersection of the tool body with the blank body if performing a
 * non-destructive Boolean.
 * Default value is <tt>*(BODY**)NULL_REF</tt> if not required.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/

DECL_BOOL outcome api_boolean_chop_complete (
    logical nonreg,           // TRUE when nonregularized results are required
    BODY*& outside,           // created to return subtraction of tool from blank
    BODY*& leftovers          // (optional) created to return any
        = *(BODY**) NULL_REF, //  unclassified lumps from the blank,
                              //  or NULL if there are none
    NDBOOL_KEEP ndbool_keep   // (optional) flag for non-destructive Booleans
		= NDBOOL_KEEP_NEITHER,
    BODY*& result_body        // (optional) result body, necessary for
		= *(BODY**) NULL_REF, // non-destructive Booleans
	AcisOptions* ao = NULL
    );

/**
 * This is a partial Boolean API function that completes a chop operation.
 * <br><b>Technical Articles:</b> <i>[Partial Booleans](http://doc.spatial.com/articles/p/a/r/Partial_Booleans_8b31.html)</i>,
 *							  <i>[Chop](http://doc.spatial.com/articles/b/a/s/Basic_Boolean_Operations_4145.html#Chop)</i>
 * <br><br>
 * <b>Role:</b> This API function completes simultaneous Boolean intersect and subtract operations
 * on two bodies (blank and tool).
 * It is assumed that the intersection graph has already been computed using other
 * partial Boolean APIs. The intersection result
 * is returned via a modified blank body. A new body is created for the subtraction
 * result and is returned via the pointer <tt>outside</tt>. The caller is responsible
 * for passing in a <tt>NULL</tt> pointer through which the subtraction result will be returned.
 * <br><br>
 * If the tool body is an incomplete solid, any lumps of the blank which are not
 * intersected by the faces of the tool, and which therefore cannot be classified
 * as either <i>inside</i> or <i>outside</i>, will be returned in a newly created body
 * via the pointer <tt>leftovers</tt>, if a <tt>NULL</tt> pointer is supplied. 
 * If <tt>leftovers</tt> is not supplied, any unclassified
 * lumps will be deleted. The operation will fail if the tool body does not extend far
 * enough to cut completely through any lump of the blank body with which its faces do
 * intersect.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param nonreg
 * (in) Set to <tt>TRUE</tt> when nonregularized results are required, <tt>FALSE</tt> otherwise.
 * @param outside
 * (out) Used to return the subtraction of the tool body from the blank body.
 * @param leftovers
 * (out) Used to return any unclassified lumps from the blank body, or <tt>NULL</tt> if none.
 * @param ndbool_keep
 * (in) Optional flag for non-destructive Booleans.
 * Valid values are <tt>NDBOOL_KEEP_NEITHER</tt> (default), <tt>NDBOOL_KEEP_BLANK</tt>,
 * <tt>NDBOOL_KEEP_TOOL</tt>, and <tt>NDBOOL_KEEP_BOTH</tt>.
 * @param result_body
 * (out) Used to return the intersection of the tool body with the blank body if performing a
 * non-destructive Boolean.
 * Default value is <tt>*(BODY**)NULL_REF</tt> if not required.
 * @param bool_opts
 * (in) Boolean options. Refer to @href BoolOptions for details.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_boolean_chop_complete (
    logical nonreg,           
    BODY*& outside,           
    BODY*& leftovers,
    NDBOOL_KEEP ndbool_keep,
    BODY*& result_body,
    BoolOptions *bool_opts,
	AcisOptions* ao = NULL
    );


// Initialize the boolean operation to the point where face/face
// intersections are performed to construct an intersection graph.
/**
 * This is a partial Boolean API function that initializes a Boolean operation.
 * <br><b>Technical Article:</b> <i>[Partial Booleans](http://doc.spatial.com/articles/p/a/r/Partial_Booleans_8b31.html)</i>
 * <br><br>
 * <b>Role:</b> This API function performs the opening stage of a Boolean operation,
 * initializing the operation to the point where face-face intersections can be
 * performed to construct an intersection graph.
 * <br><br>
 * <b>Errors:</b> <tt>tool</tt> or <tt>blank</tt> is a NULL pointer or does not point to a
 * @href BODY.
 * <br><br>
 * <b>Effect:</b> System routine.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param tool
 * (in) Tool body.
 * @param blank
 * (in) Blank body.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_boolean_start(
		BODY *				tool,
		BODY *				blank,
		AcisOptions *ao = NULL);

// This routine is passed a tool face and blank face together with an
// array of edges which represent the intersection of the two faces.
// A surf_surf_int structure is built using the geometry of these
// edges as the intersection of the two surfaces, thereby obviating
// the need to intersect the faces themselves. To record this, an
// ATTRIB_FACEINT is atttached to the tool face and no intersection
// will be performed when these faces are to be intersected. The final
// logical argument indicates whether to check the if the
// intersection edges really lie in the faces. This can be relatively
// expensive so may be avoided by passing FALSE, in which case the
// edge-face relationships are being guaranteed by the caller.
/**
 * This is a partial Boolean API function to create a @href surf_surf_int intersection structure to be
 * used in place of an actual intersection.
 * <br><b>Technical Article:</b> <i>[Partial Booleans](http://doc.spatial.com/articles/p/a/r/Partial_Booleans_8b31.html)</i>
 * <br><br>
 * <b>Role:</b> This routine is passed a tool face and blank face together with
 * an array of edges which represent the intersection of the two faces. A
 * @href surf_surf_int structure is built using the geometry of these edges as the
 * intersection of the two surfaces, thereby eliminating the need to intersect the
 * faces themselves. To record this, an @href ATTRIB_FACEINT attribute is
 * attached to the tool face and no intersection will be performed when these
 * faces are to be intersected.
 * <br><br>
 * The logical argument <tt>check_rels</tt> specifies whether or not to check
 * if the intersection edges really lie in the faces. This can be
 * relatively expensive so may be avoided by passing <tt>FALSE</tt>, in which
 * case the edge-face relationships are being guaranteed by the caller.
 * <br><br>
 * <i>Coordinate Systems and Transformations:</i> 
 * The intersection edges must be in the coordinate system of the blank body. 
 * For instance, if the blank body is in the world coordinate system 
 * (that is, the blank body does not have a transformation attached to it) 
 * then the intersection edges will also be specified in world coordinates. 
 * If the blank body does have a transformation attached to it, the intersection 
 * edges will be in the space of the untransformed blank body (that is, with respect 
 * to the world coordinate system the intersection edges will be transformed by 
 * the inverse of the transform on the blank body). 
 * The transformation associated with the tool face, <tt>ttrans</tt>, must be the 
 * transformation to convert from the coordinate system of the tool body into 
 * the coordinate system of the blank body. If T<sub>t</sub> is the transformation 
 * on the tool body and T<sub>b</sub> is the transformation on the blank body, 
 * then the transformation from tool coordinates to blank coordinates, is: 
 * T<sub>tb</sub> = T<sub>t</sub> * T<sub>b</sub><sup>-1</sup>. If both the tool 
 * and blank bodies are in the world coordinate system, then <tt>ttrans</tt> will be 
 * the identity transformation.
 * The transformation associated with the blank face, <tt>btrans</tt>, must be the 
 * identity transformation.
 * <br><br>
 * <b><i>Note:</i></b> A call to this API function should be followed by a call to
 * @href api_fixup_intersection when imprinting multiple edges on a face, 
 * but not during a typical Boolean operation.
 * <br><br>
 * <b>Errors:</b> NULL pointer to tool or blank face.
 * <br><br>
 * <b>Effect:</b> System routine.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param tool_face
 * (in) Tool face.
 * @param ttrans
 * (in) Tool body transform.
 * @param blank_face
 * (in) Blank face.
 * @param btrans
 * (in) Blank body transform.
 * @param number_edges
 * (in) Number of edges in <tt>ssi_edges</tt>.
 * @param ssi_edges
 * (in) Edges to make up the @href surf_surf_int structure.
 * @param check_rels
 * (in) Flag indicating whether or not to avoid checking the edge-face relationships.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_update_intersection(
		FACE*				tool_face,
		const SPAtransf&		ttrans,
		FACE*				blank_face,
		const SPAtransf&		btrans,
		const int 			number_edges,
		EDGE*				ssi_edges[],
		logical				check_rels = TRUE,
		AcisOptions *ao = NULL);

/**
 * Fixes intersection records created by @href api_update_intersection.
 * <br><b>Technical Article:</b> <i>[Partial Booleans](http://doc.spatial.com/articles/p/a/r/Partial_Booleans_8b31.html)</i>
 * <br><br>
 * <b>Role:</b> This API function should be called after calling @href api_update_intersection.
 * This function is needed only when imprinting multiple edges on a face, and not 
 * during a typical Boolean operation. 
 * <br><br>
 * <b>Errors:</b> <tt>tfaces[edge_knt]</tt> is a NULL pointer or does not point to a @href FACE.
 * <br><br>
 * <b>Effect:</b> System routine.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param edge_knt
 * (in) Array index of the relevant tool face.
 * @param edge_array
 * Obsolete argument.
 * @param tfaces
 * (in) Array of tool faces.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_fixup_intersection(
		int edge_knt, // Index of this intersection edge
		EDGE **edge_array,// Array of intersection edges
		FACE **tfaces,    // Array of tool faces
		AcisOptions *ao = NULL);


// This routine is given an array of faces on the tool body which will
// be intersected with the corresponding face in an array of faces on the
// blank body. The resulting intersections are appended to the current
// intersection graph.
/**
 * This is a partial Boolean API function to intersect an array of faces of one body 
 * with an array of faces of another body.
 * <br><b>Technical Article:</b> <i>[Partial Booleans](http://doc.spatial.com/articles/p/a/r/Partial_Booleans_8b31.html)</i>
 * <br><br>
 * <b>Role:</b> This API function intersects each face in array of faces from the
 * tool body with the corresponding face in an array of faces from the blank
 * body. Thus, both arrays must have the same length, equal to
 * <tt>number_faces</tt>. The resulting intersections are appended to the
 * current intersection graph.
 * <br><br>
 * <b><i>Note:</i></b> @href api_boolean_start must be called before
 * using this API function.
 * <br><br>
 * <b>Effect:</b> System routine.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param number_faces
 * (in) Size of face arrays.
 * @param tool_faces
 * (in) Array of tool body faces.
 * @param blank_faces
 * (in) Array of blank body faces.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_selectively_intersect(
		const int			number_faces,
		FACE*				tool_faces[],
		FACE*				blank_faces[],
	    AcisOptions* ao = NULL);

// This routine is given an array of faces on the tool body which will
// be intersected with the corresponding face in an array of faces on the
// blank body. The resulting intersections are appended to the current
// intersection graph.
/**
 * This is a partial Boolean API function to intersect an array of faces of one body 
 * with an array of faces of another body.
 * <br><b>Technical Article:</b> <i>[Partial Booleans](http://doc.spatial.com/articles/p/a/r/Partial_Booleans_8b31.html)</i>
 * <br><br>
 * <b>Role:</b> This API function intersects each face in array of faces from the
 * tool body with the corresponding face in an array of faces from the blank
 * body. Thus, both arrays must have the same length, equal to
 * <tt>number_faces</tt>. The resulting intersections are appended to the
 * current intersection graph.
 * <br><br>
 * <b><i>Note:</i></b> @href api_boolean_start must be called before
 * using this APIfunction.
 * <br><br>
 * <b>Effect:</b> System routine.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param number_faces
 * (in) Size of face arrays.
 * @param tool_faces
 * (in) Array of tool body faces.
 * @param blank_faces
 * (in) Array of blank body faces.
 * @param bool_opts
 * (in) Boolean options. Refer to @href BoolOptions for details.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_selectively_intersect(
		const int			number_faces,
		FACE*				tool_faces[],
		FACE*				blank_faces[],
	    BoolOptions *bool_opts,
	    AcisOptions* ao = NULL);

// Complete the first step of the Boolean operations. The current
// intersection graph contains its attributes.
/**
 * This is a partial Boolean operation for computing and building the intersection 
 * graph between two bodies.
 * <br><b>Technical Article:</b> <i>[Partial Booleans](http://doc.spatial.com/articles/p/a/r/Partial_Booleans_8b31.html)</i>
 * <br><br>
 * <b>Role:</b> This API function computes all intersections and builds a specialized wire body with 
 * Boolean attributes, 
 * representing the intersection graph between two bodies.
 * <br><br>
 * <b><i>Notes:</i></b>
 * <ul>
 * <li>Do not remove the Boolean attributes attached to the entities.
 * This API function is intended for use in conjunction with other partial Boolean API functions. Note that calling 
 * @href api_imprint_complete, @href api_boolean_complete, or @href api_boolean_chop_complete after 
 * this API function will delete the intersection graph; otherwise, the caller is responsible for deleting it, 
 * as normal.</li>For more information refer to the Technical Article
 * <i>[Non-destructive Boolean Operations](http://doc.spatial.com/articles/n/o/n/Non-destructive_Boolean_Operations_edaa.html)</i>.
 * <li>The intersection graph is not designed to be used as a general wire body.
 * To generate a wire body suitable for subsequent general use, the APIs
 * @href api_slice and @href api_clean_wire are recommended.</li>
 * <li>If no intersections exist between the two bodies, then an empty body with internal Boolean attributes would be returned.</li>
 * </ul>
 * <br><br>
 * <b>Errors:</b> <tt>tool</tt> or <tt>blank</tt> is a NULL pointer or does not point to a @href BODY.
 * <br><br>
 * <b>Effect:</b> Read-only on input. A new body is created.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param tool
 * (in) Tool or slicing body.
 * @param blank
 * (in) Blank body to be sliced.
 * @param graph
 * (out) Returned intersection graph.
 * @param type
 * (in) Type of Boolean operation (for glue only). Valid values are <tt>UNION</tt> 
 * and <tt>SUBTRACTION</tt>.
 * @param glue_opts
 * (in) Glue info and options.
 * @param ao
 * (in) ACIS options such as versioning or journaling.
 **/
DECL_BOOL outcome api_bool_make_intersection_graph (
    BODY*				tool,
    BODY*				blank,
	BODY*&				graph,
	BOOL_TYPE			type = UNION, // for glue purposes ONLY
	const glue_options *glue_opts = NULL,
	AcisOptions *ao		= NULL);

// Complete the first step of the Boolean operations. The current
// intersection graph contains its attributes.
/**
 * This is a partial Boolean operation to compute and build the intersection graph between two bodies.
 * <br><b>Technical Article:</b> <i>[Partial Booleans](http://doc.spatial.com/articles/p/a/r/Partial_Booleans_8b31.html)</i>
 * <br><br>
 * <b>Role:</b> This API function computes all intersections and builds a specialized wire body with Boolean 
 * attributes, representing the intersection graph between two bodies.
 * <br><br>
 * <b><i>Notes:</i></b>
 * <ul>
 * <li>Do not remove the Boolean attributes attached to the entities.
 * This API function is intended for use in conjunction with other partial Boolean API functions. Note that calling 
 * @href api_imprint_complete, @href api_boolean_complete, or @href api_boolean_chop_complete after 
 * this API function will delete the intersection graph, otherwise the caller is responsible for deleting it, 
 * as normal.</li>
 * <li>The intersection graph is not designed to be used as a general wire body.
 * To generate a wire body suitable for subsequent general use, the APIs
 * @href api_slice and @href api_clean_wire are recommended.</li>
 * <li>If no intersections exist between the two bodies, then an empty body with internal Boolean attributes would be returned.</li>
* </ul>
 * <br><br>
 * <b>Errors:</b> <tt>tool</tt> or <tt>blank</tt> is a NULL pointer or does not point to a @href BODY.
 * <br><br>
 * <b>Effect:</b> Read-only on input. A new body is created.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param tool
 * (in) Tool or slicing body.
 * @param blank
 * (in) Blank body to be sliced.
 * @param graph
 * (out) Returned intersection graph.
 * @param type
 * (in) Type of Boolean operation (for glue only). 
 * Valid values are <tt>UNION</tt> and <tt>SUBTRACTION</tt>.
 * @param glue_opts
 * (in) Glue info and options.
 * @param bool_opts
 * (in) Boolean options. Refer to @href BoolOptions for details.
 * @param ao
 * (in) ACIS options such as versioning or journaling.
 **/
DECL_BOOL outcome api_bool_make_intersection_graph (
    BODY*				tool,
    BODY*				blank,
	BODY*&				graph,
	BOOL_TYPE			type, // for glue purposes ONLY
	const glue_options *glue_opts,
	BoolOptions         *bool_opts,
	AcisOptions *ao		= NULL);


// Complete an imprint operation. The current intersection graph is
// imprinted on both bodies.
/**
 * This is a partial Boolean API function to complete an imprint operation.
 * <br><b>Technical Article:</b> <i>[Partial Booleans](http://doc.spatial.com/articles/p/a/r/Partial_Booleans_8b31.html)</i>
 * <br><br>
 * <b>Role:</b> This API function imprints the current intersection graph on both bodies.
 * It is assumed that the intersection graph has already been computed using other
 * partial Boolean API functions.
 * Faces and edges may be split as a result.
 * <br><br>
 * <b>Errors:</b> <tt>tool</tt> or <tt>blank</tt> is a NULL pointer or does not point to a @href BODY.
 * <br><br>
 * <b>Effect:</b> Changes models.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param tool
 * (in/out) Tool body.
 * @param blank
 * (in/out) Blank body.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_imprint_complete(
		BODY *				tool,
		BODY *				blank,
		AcisOptions *ao = NULL);


// Complete a slice operation. The current intersection graph is
// returned. The edges are ordered about a vertex if a normal
// SPAvector is specified (that is, as in sectioning the blank body with
// a plane tool body).
/**
 * This is a partial Boolean API function to complete a slice operation.
 * <br><b>Technical Article:</b> <i>[Partial Booleans](http://doc.spatial.com/articles/p/a/r/Partial_Booleans_8b31.html)</i>
 * <br><br>
 * <b>Role:</b> This API function completes a slice operation. It is assumed
 * that the necessary intersections have already been performed using other partial Boolean
 * API functions. It returns a special wire body where each edge has two
 * coedges.
 * A normal vector can given in the special case when the tool body
 * is a plane, causing this operation to sequence the resultant wires into non-overlapping
 * loops of conventional sense. Generally, the normal vector should be set to
 * <tt>*(SPAunit_vector*)NULL_REF</tt>.
 * <br><br>
 * <b><i>Notes:</i></b>
 * <ul>
 * <li>The slice graph is not designed to be used as a general wire body. To
 * generate a wire body suitable for subsequent general use, a call to
 * @href api_clean_wire is necessary after calling this API.
 * However, if there are no intersections between the blank and tool bodies, then the returned wire 
 * body is <tt>NULL</tt> and subsequent calls to @href api_clean_wire are not necessary, nor recommended.</li>
 * <li>The slice graph is partially cleaned of its Boolean attributes.
 * To obtain a full intersection graph, the API @href api_complete_intersection_graph
 * is recommended.</li>
 * </ul>
 * <br><br>
 * <b>Errors:</b> <tt>tool</tt> or <tt>blank</tt> is a NULL pointer.
 * <br><br>
 * <b>Effect:</b> Read-only on input. A new body is created.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param tool
 * (in) Tool or slicing body.
 * @param blank
 * (in) Blank body to be sliced.
 * @param normal
 * (in) Normal about which wire edges at a vertex are to be ordered.
 * @param graph
 * (out) Returned slice graph.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_slice_complete(
		BODY *				tool,
		BODY *				blank,
		SPAunit_vector const &	normal,
		BODY*&				graph,
		AcisOptions *ao = NULL);

// Complete a slice operation without removing the attributes
// attached to the entities.
/**
 * This is a partial Boolean operation to build the intersection graph between two bodies.
 * <br><b>Technical Article:</b> <i>[Partial Booleans](http://doc.spatial.com/articles/p/a/r/Partial_Booleans_8b31.html)</i>
 * <br><br>
 * <b>Role:</b> This API function builds a specialized wire body containing extra coedges
 * and Boolean attributes, representing the intersection graph between two bodies.
 * It is assumed that the necessary intersections have already been performed using other
 * partial Boolean API functions.
 * To compute and build the intersection graph in one step, the API
 * @href api_bool_make_intersection_graph is recommended.
 * <br><br>
 * <b><i>Notes:</i></b>
 * <ul>
 * <li>Do not remove the Boolean attributes attached to the entities.
 * This API function is intended for use in conjunction with other partial Boolean API functions. Note that calling 
 * @href api_imprint_complete, @href api_boolean_complete, or @href api_boolean_chop_complete after 
 * this API will delete the intersection graph, otherwise the caller is responsible for deleting it, 
 * as normal. </li>
 * <li>The intersection graph is not designed to be used as a general wire body.
 * To generate a wire body suitable for subsequent general use, the APIs
 * @href api_slice_complete and @href api_clean_wire are recommended.</li>
 * </ul>
 * <br><br>
 * <b>Errors:</b> <tt>tool</tt> or <tt>blank</tt> is a NULL pointer or does not point to a @href BODY.
 * <br><br>
 * <b>Effect:</b> Read-only on input. A new body is created.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param tbody
 * (in) Tool body.
 * @param blank
 * (in) Blank body.
 * @param graph
 * (out) Resulting intersection graph.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_complete_intersection_graph(
		BODY *				tbody,
		BODY *				blank,
		BODY*&				graph,
		AcisOptions *ao = NULL);

/**
 * This is a partial Boolean API function to imprint two bodies and stitch them along their face-face 
 * intersection curves.
 * <br><b>Technical Article:</b> <i>[Partial Booleans](http://doc.spatial.com/articles/p/a/r/Partial_Booleans_8b31.html)</i>
 * <br><br>
 * <b>Role:</b> This API function imprints the current intersection graph on both bodies, and then
 * combines the bodies, stitching them along the imprinted edges and vertices.
 * It is assumed that the intersection graph has already been computed using other
 * partial Boolean API functions.
 * The result is returned in the body <tt>b1</tt> and the body <tt>b2</tt> is deleted.
 * <br><br>
 * Face normals and coedge senses must be compatible for
 * stitching of sheet edges. Stitching of incompatible edges will be ignored.
 * When vertices at the same location (within tolerance) are merged, they become
 * non-manifold and contain all surrounding face groups.
 * <br><br>
 * This API function is designed to be faster than performing separate
 * imprint and stitch operations because the imprinted edges are supplied directly to the
 * stitch operation, thus avoiding the need to detect compatible edges. However,
 * note that bodies that do not intersect or touch will be grouped into one body, whereas
 * calling @href api_imprint_complete followed by @href api_stitch on such bodies would leave
 * them separate.
 * <br><br>
 * <b>Errors:</b><br>
 * <tt>b1</tt> or <tt>b2</tt> is a NULL pointer or does not point to a @href BODY.<br>
 * <tt>b1</tt> and <tt>b2</tt> are the same body.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param b1
 * (in/out) First body, modified by the operation.
 * @param b2
 * (in) Second body, deleted by the operation.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_imprint_stitch_complete(
		BODY*		b1,
		BODY*		b2,
		AcisOptions *ao = NULL);


// Complete a regularized/non-regularized boolean operation.
 /**
  * This is a partial Boolean API function to finish a Boolean operation.
  * <br><b>Technical Article:</b> <i>[Partial Booleans](http://doc.spatial.com/articles/p/a/r/Partial_Booleans_8b31.html)</i>
  * <br><br>
  * <b>Role:</b> This API function completes a Boolean operation using information from the
  * current intersection graph. It is assumed that the intersection graph has already
  * been computed using other partial Boolean API functions.
  * <br><br>
  * By default, the result is returned in a modified blank body and the tool body is deleted.
  * However, the non-destructive Boolean
  * argument <tt>ndbool_keep</tt>, allows the caller to preserve the blank body, tool body,
  * or both bodies.
  * If the blank body is to be preserved, a new body is created for the result and returned
  * via the pointer <tt>result_body</tt>. The caller is responsible for passing in a NULL
  * @href BODY pointer, through which the result body is returned.
  * <br><br>
  * <b>Effect:</b> Changes model.
  * <br><br>
  * <b>Journal:</b> Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Modeler
  * <br><br>
  * @param op
  * (in) Type of Boolean operation. Valid values are <tt>UNION</tt>, <tt>INTERSECTION</tt>, 
  * <tt>SUBTRACTION</tt>, <tt>NONREG_UNION</tt>, <tt>NONREG_INTERSECTION</tt>, 
  * and <tt>NONREG_SUBTRACTION</tt>.
  * @param ndbool_keep
  * (in) Optional flag for non-destructive Booleans.
  * Valid values are <tt>NDBOOL_KEEP_NEITHER</tt> (default), <tt>NDBOOL_KEEP_BLANK</tt>, 
  * <tt>NDBOOL_KEEP_TOOL</tt>, and <tt>NDBOOL_KEEP_BOTH</tt>.
  * @param result_body
  * (out) Used to return the result body if performing a non-destructive Boolean.
  * Default value is <tt>*(BODY**)NULL_REF</tt> if not required.
  * @param ao
  * (in) ACIS options such as versioning and journaling.
  **/
DECL_BOOL outcome api_boolean_complete(
		BOOL_TYPE 			op,
		NDBOOL_KEEP			ndbool_keep = NDBOOL_KEEP_NEITHER,
		BODY*&				result_body = *(BODY**) NULL_REF,
		AcisOptions*		opts = NULL
		);

// Complete a regularized/non-regularized boolean operation.
 /**
  * This is a partial Boolean API function to finish a Boolean operation.
  * <br><b>Technical Article:</b> <i>[Partial Booleans](http://doc.spatial.com/articles/p/a/r/Partial_Booleans_8b31.html)</i>
  * <br><br>
  * <b>Role:</b> This API function completes a Boolean operation using information from the
  * current intersection graph. It is assumed that the intersection graph has already
  * been computed using other partial Boolean API functions.
  * <br><br>
  * By default, the result is returned in a modified blank body and the tool body is deleted.
  * However, the non-destructive Boolean argument <tt>ndbool_keep</tt>, allows the caller 
  * to preserve the blank body, tool body, or both bodies.
  * If the blank body is to be preserved, a new body is created for the result and returned
  * via the pointer <tt>result_body</tt>. The caller is responsible for passing in a NULL
  * @href BODY pointer, through which the result body is returned.
  * For more information refer to the Technical Article <i>[Non-destructive 
  * Boolean Operations](http://doc.spatial.com/articles/n/o/n/Non-destructive_Boolean_Operations_edaa.html)</i>.
  * <br><br>
  * <b>Effect:</b> Changes model.
  * <br><br>
  * <b>Journal:</b> Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Modeler
  * <br><br>
  * @param op
  * (in) Type of Boolean operation. Valid values are <tt>UNION</tt>, <tt>INTERSECTION</tt>, 
  * <tt>SUBTRACTION</tt>, <tt>NONREG_UNION</tt>, <tt>NONREG_INTERSECTION</tt>, 
  * and <tt>NONREG_SUBTRACTION</tt>.
  * @param ndbool_keep
  * (in) Optional flag for non-destructive Booleans.
  * Valid values are <tt>NDBOOL_KEEP_NEITHER</tt> (default), <tt>NDBOOL_KEEP_BLANK</tt>, 
  * <tt>NDBOOL_KEEP_TOOL</tt>, and <tt>NDBOOL_KEEP_BOTH</tt>.
  * @param result_body
  * (out) Used to return the result body if performing a non-destructive Boolean.
  * Default value is <tt>*(BODY**)NULL_REF</tt> if not required.
  * @param bool_opts
  * (in) Boolean options. Refer to @href BoolOptions for details.
  * @param ao
  * (in) ACIS options such as versioning and journaling.
**/
DECL_BOOL outcome api_boolean_complete(
		BOOL_TYPE 			op,
		NDBOOL_KEEP			ndbool_keep,
		BODY*&				result_body,
        BoolOptions         *bool_opts,
		AcisOptions*		opts
		);

/**
 * Performs an imprint operation using a subset of faces from the tool body and
 * a subset of faces from the blank body.
 * <br><b>Technical Article:</b> <i>[Partial Booleans](http://doc.spatial.com/articles/p/a/r/Partial_Booleans_8b31.html)</i>
 * <br><br>
 * <b>Role:</b> This API function takes a list of faces from the tool body and a list of faces from
 * the blank body. It intersects every face in <tt>tool_faces</tt> with every face in
 * <tt>blank_faces</tt> and imprints the resulting intersection graph on both bodies.
 * <br><br>
 * If the argument <tt>split_checking</tt> is set to <tt>TRUE</tt>, checking will be performed
 * to assure that all edges and vertices imprinted on the blank body contribute to
 * the splitting of blank body faces. If they do not (for example, there are dangling edges
 * imprinted on the faces), then an exception will be thrown.
 * If <tt>split_checking</tt> is set to <tt>FALSE</tt>, then all edges and
 * vertices of the intersection graph will be imprinted, regardless of their
 * contribution to face splitting.
 * <br><br>
 * Optionally, the API function will return a list of the edges imprinted on the blank
 * body via the argument <tt>intgraph_edges</tt>.
 * <br><br>
 * If annotations are turned on, <tt>SPLIT_ANNOTATION</tt>s and <tt>IMPRINT_ANNOTATION</tt>s
 * will be added to the entities of the <tt>blank</tt> and <tt>tool</tt> bodies during
 * the operation.
 * <br><br>
 * <b><i>Note:</i></b> In a standard imprint operation the tool body is transformed
 * into the coordinate system of the blank body as a side effect of the imprint operation.
 * This transformation does not occur with this API function.
 * <br><br>
 * For additional information on partial Boolean operations refer to the Technical Article <i>Partial Booleans</i>.
 * <br><br>
 * <b>Errors:</b>
 * <ul>
 * <li><tt>NO_INTSCT</tt>: No intersection edges were found between the specified subsets of faces.</li>
 * <li><tt>IMPROPER_SPLIT</tt>: Checking discovered that improper face splitting had occurred.</li>
 * </ul>
 * <br>
 * <b>Effect:</b> Changes models.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param tool
 * (in/out) Tool body.
 * @param tool_faces
 * (in) Tool faces selected for imprinting. A NULL reference implies all faces should be used.
 * @param blank
 * (in/out) Blank body.
 * @param blank_faces
 * (in) Blank faces selected for imprinting and/or splitting. 
 * A NULL reference implies all faces should be used.
 * @param split_checking
 * (in) Set to <tt>TRUE</tt> to check if all edges and vertices created by the imprint contribute to the 
 * splitting of blank faces.
 * @param intgraph_edges
 * (out) Returned list of edges imprinted on the blank body.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_selectively_imprint(
	BODY*			tool,			// The tool body
	ENTITY_LIST&	tool_faces,		// List of tool faces to be split
									// A NULL reference implies all faces
	BODY*			blank,			// The blank body
	ENTITY_LIST&	blank_faces,	// List of blank faces to be split
									// A NULL reference implies all faces
	logical			split_checking = TRUE,
									// Check if all edges and vertices
									// created by the imprint contribute
									// to face splitting.
	ENTITY_LIST&	intgraph_edges	= *(ENTITY_LIST*)NULL_REF,
									// List of intersection graph edges
									// Default is a NULL reference.
	AcisOptions*   ao = NULL
	);

/**
 * Performs an imprint operation using a subset of faces from the tool body and
 * a subset of faces from the blank body.
 * <br><b>Technical Article:</b> <i>[Partial Booleans](http://doc.spatial.com/articles/p/a/r/Partial_Booleans_8b31.html)</i>
 * <br><br>
 * <b>Role:</b> This API takes a list of faces from the tool body and a list of faces from
 * the blank body. It intersects every face in <tt>tool_faces</tt> with every face in
 * <tt>blank_faces</tt> and imprints the resulting intersection graph on both bodies.
 * <br><br>
 * If the argument <tt>split_checking</tt> is set to <tt>TRUE</tt>, checking will be performed
 * to assure that all edges and vertices imprinted on the blank body contribute to
 * the splitting of blank body faces. If they do not (e.g. there are dangling edges
 * imprinted on the faces) then an exception will be thrown.
 * If <tt>split_checking</tt> is set to <tt>FALSE</tt>, then all edges and
 * vertices of the intersection graph will be imprinted, regardless of their
 * contribution to face splitting.
 * <br><br>
 * Optionally, the API will return a list of the edges imprinted on the blank
 * body via the argument <tt>intgraph_edges</tt>.
 * <br><br>
 * If annotations are turned on, <tt>SPLIT_ANNOTATION</tt>s and <tt>IMPRINT_ANNOTATION</tt>s
 * will be added to the entities of the <tt>blank</tt> and <tt>tool</tt> bodies during
 * the operation.
 * <br><br>
 * <b><i>Note:</i></b> In a standard imprint operation the tool body is transformed
 * into the coordinate system of the blank body as a side effect of the imprint operation.
 * This transformation does not occur with this API function.
 * <br><br>
 * For additional information on partial Boolean operations refer to the Technical Article <i>Partial Booleans</i>.
 * <br><br>
 * <b>Errors:</b>
 * <ul>
 * <li><tt>NO_INTSCT</tt>: No intersection edges were found between the specified subsets of faces.</li>
 * <li><tt>IMPROPER_SPLIT</tt>: Checking discovered that improper face splitting had occurred.</li>
 * </ul>
 * <br>
 * <b>Effect:</b> Changes models.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param tool
 * (in/out) Tool body.
 * @param tool_faces
 * (in) Tool faces selected for imprinting. A NULL reference implies all faces should be used.
 * @param blank
 * (in/out) Blank body.
 * @param blank_faces
 * (in) Blank faces selected for imprinting and/or splitting. A NULL reference implies all faces 
 * should be used.
 * @param split_checking
 * (in) Set to <tt>TRUE</tt> to check if all edges and vertices created by the imprint contribute to the 
 * splitting of blank faces.
 * @param intgraph_edges
 * (out) Returned list of edges imprinted on the blank body.
 * @param bool_opts
 * (in) Boolean options. Refer to @href BoolOptions for details.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_selectively_imprint(
	BODY*			tool,		
	ENTITY_LIST&	tool_faces,		
	BODY*			blank,			
	ENTITY_LIST&	blank_faces,
	logical			split_checking,
	ENTITY_LIST&	intgraph_edges,
	BoolOptions     *bool_opts,
	AcisOptions*    ao = NULL
	);



/*! @} */

/*! \addtogroup BOOLADVMERGE
 *  \brief Declared at <boolapi.hxx>, SPAbool
 *  @{
 */

/**
 * Removes faces, edges, and vertices that are not necessary to support
 * the topology of the entity.
 * <br><b>Technical Article:</b> <i>[Regularization and Merging Outside of Boolean Operations ]
   (http://doc.spatial.com/articles/a/d/d/Additional_Boolean_Operations_-_Part_2_8c1f.html#Regularization_and_Merging_Outside_of_Boolean_Operations)</i>
 * <br><br>
 * <b>Role:</b> This API function removes all unnecessary faces, edges, and
 * vertices (and associated data). A face is unnecessary if it
 * is double-sided. An edge is unnecessary if it is manifold and its
 * two adjoining faces have the same underlying geometry. A vertex is
 * unnecessary if it is shared by two edges and both edges have the
 * same underlying geometry.
 * <br><br>
 * This API function will work on any topological entity as an input entity.
 * For other entity types, no action takes place.
 * <br><br>
 * <b><i>Note:</i></b> Vertices on spline edges will not be merged out
 * unless the option <tt>merge_spline_vertex</tt> is turned on.
 * <br><br>
 * To remove unnecessary edges and vertices only, @href api_clean_entity
 * is recommended.
 * <br><br>
 * <b>Errors:</b> <tt>ent</tt> is a NULL pointer.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param ent
 * (in) Entity to be regularized.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/


DECL_BOOL outcome api_regularise_entity(
			ENTITY *ent,					// entity from which unnecessary faces,
										// edges and vertices are to be removed.
			AcisOptions* ao =			// options. It contains aditional information
			NULL						//            and version and journaling data
);

/**
 * Removes edges and vertices that are not needed to support the topology of the entity.
 * <br><b>Technical Article:</b> <i>[Regularization and Merging Outside of Boolean Operations ]
   (http://doc.spatial.com/articles/a/d/d/Additional_Boolean_Operations_-_Part_2_8c1f.html#Regularization_and_Merging_Outside_of_Boolean_Operations)</i>
 * <br><br>
 * <b>Role:</b> This API function removes all unnecessary edges, vertices, and associated data
 * from the entity. An edge is unnecessary if it is manifold and its
 * two adjoining faces have the same underlying geometry. A vertex is
 * unnecessary if it is shared by two edges and both edges have the
 * same underlying geometry.
 * <br><br>
 * This API function will work on any topological entity as an input entity.
 * For non-topological entity types, no action takes place.
 * <br><br>
 * <b><i>Note:</i></b> Vertices on spline edges will not be merged out
 * unless the option <tt>merge_spline_vertex</tt> is turned on.
 * <br><br>
 * To remove unnecessary faces as well as unnecessary edges and vertices, 
 * @href api_regularise_entity is recommended.
 * To merge faces of a specified geometry type, @href api_merge_faces
 * is recommended.
 * <br><br>
 * <b>Errors:</b> <tt>ent</tt> is a NULL pointer.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param ent
 * (in) Entity to be cleaned.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_clean_entity(
			ENTITY *ent,			// entity from which unnecessary edges
								// and vertices are to be removed.
			AcisOptions* ao = NULL
		);


/**
 * Removes any unnecessary edges and vertices contained in a list of entities.
 * <br><b>Technical Article:</b> <i>[Regularization and Merging Outside of Boolean Operations ]
   (http://doc.spatial.com/articles/a/d/d/Additional_Boolean_Operations_-_Part_2_8c1f.html#Regularization_and_Merging_Outside_of_Boolean_Operations)</i>
 * <br><br>
 * <b>Role:</b> This API function builds a list of candidate edges and vertices from
 * the given list and then removes any unnecessary edges or vertices in that
 * list. Any bodies, lumps, shells, wires, faces, edges, or vertices in the
 * list are used to generate the list of candidates. Any edge or vertex in
 * the list simply becomes a candidate. If there are faces in the list, then
 * any edge which is shared by a pair of faces in the list becomes a
 * candidate. Any vertex which is shared by edges in the list, or which is on
 * the boundary of any face in the list, becomes a candidate. Any bodies,
 * lumps, or shells in the list are used to generate a list of faces, from
 * which a list of candidate edges is generated, and any wires in the list are
 * used to generate a list of candidate edges. 
 * <br><br>
 * The rules for when an edge or a vertex is unnecessary are exactly the same
 * as those applied by @href api_clean_entity.
 * <br><br>
 * Any entity in the list of a type not mentioned above is ignored.
 * <br><br>
 * The input list is unchanged by this API function.
 * <br><br>
 * <b><i>Note:</i></b> Vertices on spline edges will not be merged out
 * unless the option <tt>merge_spline_vertex</tt> is turned on.
 * <br><br>
 * To remove unnecessary faces as well as unnecessary edges and vertices, 
 * @href api_regularise_entity is recommended.
 * To merge faces of a specified geometry type, @href api_merge_faces
 * is recommended.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param ents
 * (in) List of entities to be cleaned.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_clean_list_of_entities(
			ENTITY_LIST& ents,	// entities from which unnecessary edges
								// and vertices are to be removed.
			AcisOptions* ao = NULL
		);


/**
 * Removes the attributes and extra coedges present on a wire body
 * generated by a slice operation.
 * <br><br>
 * <b>Role:</b> This API function is designed for use after calling @href api_slice or
 * @href api_slice_complete. It deletes the partner coedges and the Boolean
 * attributes on all coedges on all wires in the specialized wire body. The
 * result is a wire body that is suitable for subsequent general use.
 * <br><br>
 * <b>Errors:</b> <tt>wire</tt> is a NULL pointer or does not point to a wire body.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param wire
 * (in) Wire body to clean.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_clean_wire(
			BODY *wire,				// body containing slice wires to be
								// converted to standard form.
			AcisOptions* ao = NULL
		);


/*! @} */

/*! \addtogroup BOOLADVCHECKSREPAIRS
 *  \brief Declared at <boolapi.hxx>, SPAbool
 *  @{
 */


// for face/face checking
/**
 * Checks an entity for improper intersections and containments.
 * <br><b>Technical Article:</b> <i>[ACIS Checker](http://doc.spatial.com/articles/a/c/i/ACIS_Checker_e788.html)</i>
 * <br><br>
 * <b>Role:</b> This API function checks an entity for improper intersections and containments.
 * In particular,
 * <ul>
 * <li>Adjacent faces should intersect only in edges or vertices lying between the faces.</li>
 * <li>Faces that are not adjacent should not intersect.</li>
 * <li>Adjacent edges should intersect only at vertex lying between the edges.</li>
 * <li>Edges that are not adjacent should not intersect.</li>
 * <li>Two shells in the same lump should have a correct containment relation 
 * and they should not intersect.</li>
 * <li>No lump should be contained in another lump and they should not intersect.</li>
 * </ul>
 * <br>
 * Any faces in the entity that belong to entities in the list
 * <tt>insane_ents</tt> are removed from consideration. From the remaining
 * faces, each pair of distinct faces is intersected. Any
 * intersections not reflected in the topology of the model are deemed
 * improper.
 * <br><br>
 * By default, checks for improper edge-edge intersections are not
 * performed unless the owning face contains free edges (that is, coedges
 * without a partner). To force checks for improper edge-edge
 * intersections on all faces, the option <tt>check_ee_int_always</tt> should
 * be switched on.
 * <br><br>
 * This API function also checks valid shells and valid lumps for improper
 * containments. Containment tests are only performed when
 * <tt>insane_ents</tt> is non-NULL. A shell is valid if it contains
 * no bad faces (that is, it does not contain and is not contained by any
 * entity in <tt>insane_ents</tt>) and does not contain intersecting
 * faces. Similarly, lumps are valid when they contain no bad faces,
 * intersection faces, or shells with improper containment. Two shells
 * in the same lump have bad containment if either does not contain
 * the other. Two lumps have bad containment when one contains the
 * other.
 * <br><br>
 * If improper intersections or containments exist, then the logical
 * <tt>bad_ints</tt> is returned set to <tt>TRUE</tt>. For each pair of
 * improper entities, an @href ERROR_ENTITY object is
 * created and added to the list <tt>insane_ents</tt>, if it is
 * non-NULL. It is also added to the @href insanity_list <tt>list</tt> if
 * supplied, otherwise the error information is written to the file
 * <tt>file_ptr</tt>.
 * <br><br>
 * To check only a selected set of faces in an entity, 
 * @href api_check_list_ff_ints is recommended.
 * <br><br>
 * <b>Errors:</b> <tt>given_entity</tt> is a NULL pointer or does not point to an <tt>ENTITY</tt>.
 * <br><br>
 * <b>Effect:</b> Does not change model. Derived data may be cached.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param given_entity
 * (in) Entity to check.
 * @param insane_ents
 * (in/out) List of errors in the form of an entity list.
 * @param bad_ints
 * (out) Returns <tt>TRUE</tt> if problems are found, <tt>FALSE</tt> otherwise.
 * @param file_ptr
 * (in) Output file.
 * @param list
 * (in/out) List of errors as delivered by the ACIS Checker.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_check_entity_ff_ints(
		  const ENTITY* given_entity,      // entity to test
				ENTITY_LIST *insane_ents,  // error entities found
				logical &bad_ints,         // TRUE if bad intersections
                                           // or containment
				FILE *file_ptr = NULL,     // where to send output
				insanity_list*& list = *(insanity_list **) NULL_REF,
				AcisOptions* ao   = NULL
	);
/**
 * Checks selected faces in an entity for improper intersections.
 * <br><b>Technical Article:</b> <i>[ACIS Checker](http://doc.spatial.com/articles/a/c/i/ACIS_Checker_e788.html)</i>
 * <br><br>
 * <b>Role:</b>
 * This API function is a specialized version of @href api_check_entity_ff_ints, as
 * it checks only selected faces of an entity for improper intersections.
 * In this case, pairs of faces are intersected where one face belongs
 * to <tt>face_list1</tt> and one face belongs to <tt>face_list2</tt>.
 * If <tt>num_faces2</tt> is 0, then every face in the entity is used
 * instead of <tt>face_list2</tt>.
 * <br><br>
 * Refer to the documentation for @href api_check_entity_ff_ints
 * for more information. Note that the faces in both lists must belong
 * to the same entity.
 * <br><br>
 * <b>Effect:</b> Does not change model. Derived data may be cached.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param num_faces1
 * (in) Number of faces in first list.
 * @param face_list1
 * (in) First list of faces.
 * @param insane_ents
 * (in/out) List of errors in the form of an entity list.
 * @param bad_ints
 * (out) Returns <tt>TRUE</tt> if problems are found.
 * @param file_ptr
 * (in) Output file
 * @param num_faces2
 * (in) Number of faces in second list.
 * @param face_list2
 * (in) Second list of faces.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_check_list_ff_ints(
                  int num_faces1,            // number of faces in face_list1
                  FACE *face_list1[],        // face list1 to check
				  ENTITY_LIST *insane_ents,  // error entities found
				  logical &bad_ints,         // TRUE if bad intersections
                                             // or containment
				  FILE *file_ptr = NULL,     // where to send output
                  int num_faces2 = 0,        // number of faces in face_list2
                  FACE *face_list2[] = NULL,  // face list2 to check
				  AcisOptions* ao = NULL
                  );

/*! @} */

/*! \addtogroup BOOLADVMERGE
 *  \brief Declared at <boolapi.hxx>, SPAbool
 *  @{
 */

/**
 * Merges faces of a specified geometry type if they are not necessary to define the body.
 * <br><b>Technical Article:</b> <i>[Regularization and Merging Outside of Boolean Operations ]
 * (http://doc.spatial.com/articles/a/d/d/Additional_Boolean_Operations_-_Part_2_8c1f.html#Regularization_and_Merging_Outside_of_Boolean_Operations)</i>
 * <br><br>
 * <b>Role:</b> This API function merges faces of a specified geometry type if
 * they are not neccessary to define the body.
 * Specifically, it removes manifold edges that lie between
 * faces with the same underlying geometry of the specified type.
 * <br><br>
 * If <tt>geom_type</tt> is a NULL reference, then the surface geometry type
 * is not checked and all types are processed.
 * <br><br>
 * <b>Errors:</b> <tt>body</tt> is a NULL pointer or does not point to a @href BODY.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param body
 * (in/out) Body containing faces to merge.
 * @param geom_type
 * (in) Surface type identifier. Valid values are <tt>PLANE_TYPE</tt>, <tt>CONE_TYPE</tt>, 
 * <tt>SPHERE_TYPE</tt>, <tt>TORUS_TYPE</tt>, and <tt>SPLINE_TYPE</tt>.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_merge_faces(
			BODY *body,			// Body on which to merge planar faces
			int &geom_type,			// type of face to merge
			AcisOptions* ao  = NULL
		);

/**
 * Merges seam edges on a specified body.
 * <br><b>Technical Article:</b> <i>[Regularization and Merging Outside of Boolean Operations ]
 * (http://doc.spatial.com/articles/a/d/d/Additional_Boolean_Operations_-_Part_2_8c1f.html#Regularization_and_Merging_Outside_of_Boolean_Operations)</i>
 * <br><br>
 * <b>Role:</b> This API function merges (removes) any seam edges found on the specified body.
 * <br><br>
 * <b><i>Caution:</i></b> If the option <tt>periodic_no_seam</tt> is set to its 
 * non-default value of <tt>FALSE</tt>, the removal of such edges will result in a 
 * body that is considered invalid by @href api_check_entity.
 * <br><br>
 * <b>Errors:</b> <tt>body</tt> is a NULL pointer or does not point to a @href BODY.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param body
 * (in/out) Body whose seam edges are to be merged.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_merge_seam_edges(
			BODY *body,			
			AcisOptions* ao  = NULL
		);

/**
 * Attaches a @href NO_MERGE_ATTRIB attribute to each entity in a list.
 * <br><b>Technical Article:</b> <i>[Preventing Entities from Being Removed by Merging]
 * (http://doc.spatial.com/articles/a/d/d/Additional_Boolean_Operations_b537.html#Preventing_Entities_from_Being_Removed_by_Merging)</i>
 * <br><br>
 * <b>Role:</b> This API function attaches a NO_MERGE_ATTRIB attribute to
 * each double-sided face, edge, or vertex in the given list. 
 * The action of this attribute is to mark an entity as not mergeable 
 * when it would otherwise be merged out during a merge operation.
 * <br><br>
 * <b><i>Note:</i></b> Currently, the @href NO_MERGE_ATTRIB attribute
 * is only recognized when attached to double-sided faces, edges, or vertices.
 * <br><br>
 * <b>Effect:</b> Changes model's attribute data.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param list
 * (in) List of double-sided faces, edges, or vertices.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_set_no_merge_attrib(
			ENTITY_LIST& list,
			AcisOptions* ao = NULL
		);

DECL_BOOL outcome gme_api_set_no_merge_attrib(
			ENTITY_LIST& list,
			AcisOptions* ao = NULL
		);
/**
 * Removes @href NO_MERGE_ATTRIB attributes from each entity in a list.
 * <br><b>Technical Article:</b> <i>[Preventing Entities from Being Removed by Merging]
 * (http://doc.spatial.com/articles/a/d/d/Additional_Boolean_Operations_b537.html#Preventing_Entities_from_Being_Removed_by_Merging)</i>
 * <br><br>
 * <b>Role:</b> This API function removes the @href NO_MERGE_ATTRIB attribute,
 * if it exists, from each double-sided face, edge, or vertex in the given list.
 * <br><br>
 * <b>Effect:</b> Changes model's attribute data.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param list
 * List of double-sided faces, edges, or vertices.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_remove_no_merge_attrib(
			ENTITY_LIST& list,
			AcisOptions* ao = NULL
		);
DECL_BOOL outcome gme_api_remove_no_merge_attrib(
			ENTITY_LIST& list,
			AcisOptions* ao = NULL
		);
/*! @} */

/*! \addtogroup BOOLADVSTITCH
 *  \brief Declared at <boolapi.hxx>, SPAbool
 *  @{
 */

/**
 * Joins a list of edges into a single edge.
 * <br><br>
 * <b>Role:</b> This API function joins all edges in the supplied entity list into a
 * single edge (<tt>resulting_edge</tt>). If <tt>join_c1</tt> is set to <tt>TRUE</tt>, the resulting
 * edge is made C1-continuous at the join(s). Note that the resulting edge will be
 * one of the input edges. This API can be used to join together spline edges that do not
 * necessarily share the same geometry. If a composite curve is needed to represent the
 * underlying geometry, it will be constructed using a curve law. 
 * <br><br>
 * <b>Errors:</b> If the edges supplied are not end-to-end, then no merging takes place.
 * If the edges supplied are not G1-continuous at their common vertices, or if more than 
 * two edges meet at an interior vertex of the edge list, then this API function will not merge the edges.
 * <br><br>
 * <b>Limitations:</b> This API function does not handle cases where the edges are branched at the
 * "interior" vertices.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param edge_list
 * (in) List of edges to be joined.
 * @param resulting_edge
 * (out) Resulting edge.
 * @param join_c1
 * (in) Flag indicating whether or not to join as C1.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 * <br>
 **/
DECL_BOOL outcome api_join_edges(
	ENTITY_LIST &edge_list,
	EDGE * &resulting_edge,
	logical join_c1 = TRUE,
	AcisOptions* ao = NULL
	);

/**
 * Stitches faces along edges and vertices of identical geometry.
 * <br><b>Technical Article:</b> <i>[Stitching Faces](http://doc.spatial.com/articles/a/d/d/Additional_Boolean_Operations_b537.html#Stitching_Faces)</i>
 * <br><br>
 * <b>Role:</b> This API function joins two face bodies along edges and vertices that are
 * identical. Stitching operates only on faces, not on wires, and only stitches
 * faces to faces. If wires exist in one of the bodies being stitched, but do not
 * participate in the stitch (that is, they do not coincide with edges in the other
 * body), they will transfer to the resulting body.
 * <br><br>
 * The result is returned in the first body <tt>b1</tt>. The second body,
 * <tt>b2</tt>, is deleted unless it is the same as <tt>b1</tt>.
 * For example, a body might need internal stitching. You can stitch
 * <tt>b1</tt> to <tt>b1</tt>, and the body is not deleted.
 * <br><br>
 * When creating two-manifold edges on single-sided faces, stitching merges
 * geometry on coedges that have opposite edge sense and identical edge geometry
 * (within tolerance). The API will fail if coedges of incompatible orientation
 * (that is, same edge sense) are encountered. If the faces are double-sided, the
 * coedges need not be of opposite sense.
 * <br><br>
 * If the argument <tt>split</tt> is <tt>FALSE</tt>, the edges must be identical along their entire
 * length. If <tt>split</tt> is <tt>TRUE</tt>, this API function splits edges in order to match coincident
 * coedges. Coincident edges on single-sided faces and of incompatible orientation
 * (opposite coedge sense) are not split.
 * <br><br>
 * When creating non-manifold edges, the coedges are sorted and a "union" is
 * performed around the coedge, marking faces that are now <tt>BOTH_INSIDE</tt> as such. The
 * <tt>BOTH_INSIDE</tt> containment is then propagated to all faces not connected through a
 * stitched edge.
 * <br><br>
 * When vertices at the same location (within tolerance) are merged, they become
 * non-manifold and they contain all surrounding face groups. If the attempt to make
 * a two-manifold edge stitch fails, the vertices are not merged.
 * <br><br>
 * Unlike other Boolean Component API functions, this API function does <i>not</i> perform a Boolean operation.
 * Stitching is simpler than a Boolean operation because it avoids face-face intersections
 * and the evaluation of lump and shell containments.
 * <br><br>
 * For information on more comprehensive stitching functionality not located in the Boolean Component
 * refer to the Technical Article <i>Stitching</i>.
 * <br><br>
 * <b>Errors:</b>
 * <ul>
 * <li><tt>b1</tt> or <tt>b2</tt> is a NULL pointer or does not point to a @href BODY.</li>
 * <li><tt>NO_STITCHABLE_GEOM</tt>: No identical edges or vertices in stitch bodies.</li>
 * <li><tt>INCOMPATIBLE_COEDS</tt>: Incompatible coedges encountered.</li>
 * <li><tt>ILLEGAL_WIRE_STITCH</tt>: Illegal attempt to stitch to a wire edge.</li>
 * <li><tt>MISMATCHED_EDGES</tt>: Edge stitching problem. Duplicate edges may be present.</li>
 * </ul>
 * <br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param b1
 * (in/out) First body, modified by the operation.
 * @param b2
 * (in) Second body, deleted by the operation unless the same as <tt>b1</tt>.
 * @param split
 * (in) Flag indicating whether or not to split edges to match coincident coedges.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_stitch(
			BODY *b1, 					// Resultant body
			BODY *b2,					// Body to stitch
			logical split = FALSE,			// split option
			AcisOptions* ao = NULL
		);


/**
 * Combines two bodies, stitching them along their face-face intersection curves and at
 * coincident vertices.
 * <br><b>Technical Article:</b> <i>[Imprint-Stitch](http://doc.spatial.com/articles/i/m/p/Imprint-Stitch_8d0a.html)</i>
 * <br><br>
 * <b>Role:</b> This API function computes the intersection graph between two bodies, imprints the
 * intersection graph on both bodies, and then combines the bodies, stitching them along the
 * imprinted edges and vertices. The result is returned in the body <tt>b1</tt> and the
 * body <tt>b2</tt> is deleted.
 * <br><br>
 * Face normals and coedge senses must be compatible for
 * stitching of sheet edges. Stitching of incompatible edges will be ignored.
 * When vertices at the same location (within tolerance) are merged, they become
 * non-manifold and contain all surrounding face groups.
 * <br><br>
 * This API function is designed to be faster than performing separate
 * imprint and stitch operations because the imprinted edges are supplied directly to the
 * stitch operation, thus avoiding the need to detect compatible edges. However,
 * note that bodies that do not intersect or touch will be grouped into one body, whereas
 * calling @href api_imprint followed by @href api_stitch on such bodies would leave
 * them separate.
 * <br><br>
 * <b>Errors:</b><br>
 * <tt>b1</tt> or <tt>b2</tt> is a NULL pointer or does not point to a @href BODY.<br>
 * <tt>b1</tt> and <tt>b2</tt> are the same body.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param b1
 * (in/out) First body, modified by the operation.
 * @param b2
 * (in) Second body, deleted by the operation.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_imprint_stitch(
		BODY *b1,                      // Resultant body
		BODY *b2,                      // Body to stitch
		AcisOptions* ao				// contains journal and version
			 = NULL                 //  information
);

/**
 * Combines two bodies, stitching them along their face-face intersection curves and at
 * coincident vertices.
 * <br><b>Technical Article:</b> <i>[Imprint-Stitch](http://doc.spatial.com/articles/i/m/p/Imprint-Stitch_8d0a.html)</i>
 * <br><br>
 * <b>Role:</b> This API function computes the intersection graph between two bodies, imprints the
 * intersection graph on both bodies, and then combines the bodies, stitching them along the
 * imprinted edges and vertices. The result is returned in the body <tt>b1</tt> and the
 * body <tt>b2</tt> is deleted.
 * <br><br>
 * Face normals and coedge senses must be compatible for
 * stitching of sheet edges. Stitching of incompatible edges will be ignored.
 * When vertices at the same location (within tolerance) are merged, they become
 * non-manifold and contain all surrounding face groups.
 * <br><br>
 * This API function is designed to be faster than performing separate
 * imprint and stitch operations because the imprinted edges are supplied directly to the
 * stitch operation, thus avoiding the need to detect compatible edges. However,
 * note that bodies that do not intersect or touch will be grouped into one body, whereas
 * calling @href api_imprint followed by @href api_stitch on such bodies would leave
 * them separate.
 * <br><br>
 * <b>Errors:</b><br>
 * <tt>b1</tt> or <tt>b2</tt> is a NULL pointer or does not point to a @href BODY.<br>
 * <tt>b1</tt> and <tt>b2</tt> are the same body.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param b1
 * (in/out) First body, modified by the operation.
 * @param b2
 * (in) Second body, deleted by the operation.
 * @param boolopts
 * (in) Boolean options. Refer to @href BoolOptions for details.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_imprint_stitch(
		BODY *b1,                      
		BODY *b2,                     
	    BoolOptions *bool_opts,
		AcisOptions* ao				
			 = NULL                
);


/**
 * Joins two wire bodies at coincident vertices.
 * <br><b>Technical Article:</b> <i>[Stitching Edges](http://doc.spatial.com/articles/a/d/d/Additional_Boolean_Operations_b537.html#Stitching_Edges)</i>
 * <br><br>
 * <b>Role:</b> This API function combines two bodies, the blank and the tool, joining their disjoint
 * wire components at coincident vertices. The result is returned in the blank body and
 * the tool body is deleted.
 * <br><br>
 * <b>Limitations:</b>
 * <ul>
 * <li>This is a join operation, not a Boolean operation; in particular, no intersections are
 * performed. The bodies should not intersect anywhere other than at common wire vertices.
 * Otherwise, the result will be a self-intersecting body.</li>
 * <li>The wires to be joined must not be connected to any faces.</li>
 * </ul>
 * <br>
 * <b>Errors:</b><br>
 * <tt>tool</tt> or <tt>blank</tt> is a NULL pointer or does not point to a @href BODY.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param tool
 * (in) Tool body, deleted by the operation.
 * @param blank
 * (in/out) Blank body, modified by the operation.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_unite_wires(
			BODY *tool,
			BODY *blank,
			AcisOptions* ao = NULL
);

/*! @} */


/*! \addtogroup BOOLPROJAPI
 *  @{
 */


/**
 * Projects an edge onto a face.
 * <br><b>Technical Articles:</b> <i>[Projection](http://doc.spatial.com/articles/p/r/o/Projection.html)</i>,
 *                            <i>[HowTo:Project an edge onto a face](http://doc.spatial.com/articles/p/r/o/HowTo~Project_an_edge_onto_a_face_0c0c.html)</i>
 * <br><br>
 * <b>Role:</b>
 * This API function projects <tt>edge</tt> onto <tt>face</tt>, returning the result in <tt>elist</tt>.
 * The resulting edges and vertices in <tt>elist</tt> are not imprinted on the face. <br>
 * This API by default attempt to resolve self-intersections in projected edge, and improper intersections
 * between multiple projected edges. If unable to resolve, the API will fail by default. This behavior can be
 * controlled using @href project_options
 * <br><br>
 * Note that it is entirely possible
 * that no perpendicular projection exists for any part of the edge. In these
 * situations, the function will return a successful %outcome, but <tt>elist</tt>
 * will be empty. This allows an application to distinguish between a failed projection
 * and a non-existent projection.
 * <br><br>
 * For additional information refer to the Technical Article <i>Projection</i>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param edge
 * Edge to be projected.
 * @param face
 * Face to project edge onto.
 * @param elist
 * List of projected edges and vertices.
 * @param po
 * Projection options.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_project_edge_to_face(
            const EDGE * edge,
            const FACE * face,
            ENTITY_LIST * elist,
            project_options * po = NULL,
            AcisOptions * ao = NULL);

/**
 * Projects a wire onto a body.
 * <br><b>Technical Article:</b> <i>[Projection](http://doc.spatial.com/articles/p/r/o/Projection.html)</i>
 * <br><br>
 * <b>Role:</b>
 * This API function projects <tt>wire_body</tt> onto <tt>body</tt>, returning the result in <tt>body</tt>.
 * The projected edges and vertices are imprinted on the body by default although this can be 
 * controlled by the setting of an option in the project_options object. If imprinting is not 
 * done, then projected wire_body will be returned in @href project_options object.<br>
 * This API by default attempt to resolve self-intersections in projected edge, and improper intersections
 * between multiple projected edges. If unable to resolve, the API will fail by default. This behavior can be
 * controlled using @href project_options
 * <br><br>
 * Note that it is entirely possible that no perpendicular projection exists for 
 * any part of the wire. In these situations, the function will return a successful outcome, 
 * but the body will be unchanged. This allows an application to distinguish between a failed 
 * projection and a non-existent projection.
 * <br><br>
 * For additional information refer to the Technical Article <i>Projection</i>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param wire_body
 * Wire body to be projected.
 * @param body
 * Body to project and imprint onto.
 * @param po
 * Projection options.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_project_wire_to_body(
			const BODY * wire_body,
			BODY * body,
			project_options * po = NULL,
			AcisOptions * ao = NULL);


/*! \addtogroup BOOLADVOTHER
 *  \brief Declared at <boolapi.hxx>, SPAbool
 *  @{
 */

/**
 * Creates a new model by a slice based on a clipped copy of the model.
 * <br>
 * <b>Role:</b> This API function takes an entity list (<tt>model</tt>), a
 * position (<tt>eye</tt>), a non-zero vector (<tt>targ_eye</tt>), and
 * two doubles (<tt>hither</tt> and <tt>yon</tt>). It copies and then
 * trims those parts of the copied entity list that are outside the hither and yon
 * planes (these are orthogonal to the passed vector and the passed point is at a
 * distance hither along the passed vector from the hither %plane and a distance yon
 * along the passed vector from the yon %plane). It passes back the
 * trimmed copy via the argument <tt>clipped_copy</tt>.
 * <br><br>
 * You should check the value of the <tt>logical</tt> argument
 * <tt>MADE_COPY</tt> upon return to avoid accidental
 * deletion of the original model.
 * If the model does not need to be trimmed, that is, it already is completely
 * contained within the hither-yon slice, then the reference to the
 * original entity list is returned instead of making a copy, and
 * <tt>MADE_COPY</tt> is returned set to <tt>FALSE</tt>. Otherwise, the copy
 * of the model is trimmed as described above and <tt>MADE_COPY</tt>
 * is returned set to <tt>TRUE</tt>.
 * <br><br>
 * <b>Errors:</b> <tt>tool</tt> or <tt>blank</tt> is a NULL pointer or 
 * does not to point to a @href BODY.
 * <br><br>
 * <b>Effect:</b> Read-only on input. A new model may be created.
 * <br><br>
 * <b>Journal:</b> Available
 * <br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param model
 * (in) Model to be clipped.
 * @param clipped_copy
 * (out) Clipped model, or the original model if trimming was unnecessary.
 * @param eye
 * (in) Eye position.
 * @param targ_eye
 * (in) Non-zero target vector. If a zero vector is supplied, a random vector
 * is chosen.
 * @param hither
 * (in) Perp distance from hither %plane to eye.
 * @param yon
 * (in) Perp distance from yon %plane to eye.
 * @param MADE_COPY
 * (out) Logical flag indicating whether or not a copy of the model was made.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_slice_of_model(
		  const ENTITY_LIST& model,			//entity list to be clipped
				ENTITY_LIST& clipped_copy,	//ref to copy made
				SPAposition eye,				//eye
				SPAvector targ_eye,			//target - eye
				double hither,				//perp distance from hither plane
											// to eye
				double yon,					//perp distance from yon plane
											// to eye
				logical& MADE_COPY,			//flag saying copy was made
				AcisOptions* ao = NULL
	);

/** @} */

/**
 * @nodoc
 */
DECL_BOOL logical imprint_edge_array(
		int		num_edges,			// Input: number of edges to imprint
		EDGE	**edge_array,		// Input: array of size num_edges
		FACE	*blank_face			// Input: blank face
	);

DECL_BOOL logical gme_imprint_edge_array(
		int		num_edges,			// Input: number of edges to imprint
		EDGE	**edge_array,		// Input: array of size num_edges
		FACE	*blank_face			// Input: blank face
	);

/**
 * @nodoc
 * <b>Obsolete:</b> Use @href api_clean_entity instead.
 * <br><br>
 * Removes all edges (faces and associated data) that are not necessary to support 
 * the topology of the body.
 * <br><br>
 * <b>Role:</b> This API function removes all unnecessary edges (faces and associated
 * data) and vertices from the entity. An edge is not needed if the surface
 * defining the two faces of the edge are the same geometrically. For other
 * entity types no action takes place.
 * <br><br>
 * <b>Errors:</b> A NULL pointer to an entity is given.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param body
 * Body to be cleaned.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
[[deprecated("Will be deprecated soon : Please use api_clean_entity instead")]]
DECL_BOOL outcome api_clean_body(
			BODY *body,				// body from which unnecessary edges
								// and vertices are to be removed.
			AcisOptions* ao = NULL
		);

DECL_BOOL outcome gme_api_join_bodies(
			BODY* tool, 
			BODY* blank
		);

#endif

