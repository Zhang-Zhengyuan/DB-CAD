/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

/*******************************************************************/
// 10-Aug-09 ajr : Added api_entity_point_distance to take single 
//                 entity and point and an epd options object.
// 21-Nov-08 thm : Add api_ray_fire interface
// 27-May-08 DM  : Added grouping for documentation
// 05-Dec-07 DM  : Moved silhouette_options to new file
// 29-Nov-07 thm : Add silhouette_options object and a silhouette api 
//                 which takes this object.
// 31-May-07 DM  : Add new function api_entity_point_distance for a
//                 list of positions
// 01-May-07 DM  : Modify documentation to refer to new option 
//                 use_extrema_vertex_edge_tols
// 11-Jan-07 DM  : Modify documentation for api_get_entity_box
// 13-Sep-06 DM  : Modify documentation for api_get_entity_box
// 27-Jun-06 ajr : Moved api_get_entity_box (from kernapi.hxx)
// 21-Dec-05 nay : Moved api_edfa_int into boolean component.
/*******************************************************************/

// Header for api routines.
// For details of how to call each routine, consult the documentation
// for individual routines.
/*******************************************************************/

#if !defined( INTRAPI_HXX )
#define INTRAPI_HXX

#include "acis.hxx"
#include "box_opts.hxx"
#include "sil_opts.hxx"

/**
 * \defgroup INTRMAIN Intersectors
 *      \brief Details of Intersector functionality, supporting classes and enums
 *			\defgroup INTRAPICONVEXITY Convexity of a Curve or Edge
 *			\brief Determine the convexity of a curve or EDGE at a point or parameter
 *				\ingroup MODELANALYSIS
 *			\defgroup INTRAPISELFINT Self-Intersection of a CURVE or EDGE
 *			\brief Determine self-intersections of a CURVE or EDGE
 *				\ingroup INTRMAIN
 *			\defgroup INTRAPISILHOUETTE Silhouettes of a FACE
 *			\brief Determine the silhouettes of a FACE for a parallel or perspective projection
 *				\ingroup INTRMAIN
 *			\defgroup INTRAPIEXTREMA Entity Extrema
 *			\brief Determine the extremal position of one or more entities in a given direction
 *				\ingroup MODELANALYSIS
 *			\defgroup INTRAPIBOUNDARYFIELD Create a Boundary Vector Field
 *			\brief Create a vector field on a boundary COEDGE
 *				\ingroup MODELANALYSIS
 *			\defgroup INTRAPIISOLINES Create Isoparametric curves of a FACE
 *			\brief Create edges that lie on the isoparametric curves of a FACE
 *				\ingroup MODELANALYSIS
 *			\defgroup INTRAPIFACETCURVE Curve Faceting
 *			\brief Determine a linear approximation to a curve
 *				\ingroup MODELANALYSIS
 *			\defgroup INTRAPICCI Curve-curve or edge-edge Intersections
 *			\brief Determine curve-curve or edge-edge Intersections
 *				\ingroup INTRMAIN
 *			\defgroup INTRAPIPROJECT Project a Curve to a Surface
 *			\brief Project a curve to a surface
 *				\ingroup INTRMAIN
 *			\defgroup INTRAPIEDGETANG EDGE Tangency Conditions
 *			\brief Test an EDGE for tangency conditions
 *				\ingroup MODELANALYSIS
 *			\defgroup INTRDIRECTBS3CURVEINT Intersection and Projection Utility Functions for bs3_curve
 *			\brief Determine the intersection of a bs3_curve and a curve or surface, and projection of a bs3_curve onto a plane
 *				\ingroup INTRMAIN
 *			\defgroup INTRDIRECTSPLITTING Splitting Edges and Coedges
 *			\brief Split an EDGE at a convexity point or a VERTEX 
 *				\ingroup MODELANALYSIS
 */

/**
 * @file intrapi.hxx
 */

#include "dcl_intr.h"
#include "api.hxx"
#include "ptfcenum.hxx"
#include "bl_enum.hxx"
#include "param.hxx"
#include "coedfield.hxx"
#include "pattern_enum.hxx"

class BODY;
class WIRE;
class FACE;
class EDGE;
class APOINT;
class CURVE;
class ENTITY;
class ENTITY_LIST;
class SPAvector;
class curve;
class SPAposition;
class SPApar_pos;
class SPAtransf;
class SPAunit_vector;
class curve_curve_int;
class curve_irregularities;
class check_status_list;
class insanity_list;

// ------------------------------------------------------------------------------------------------

/** \addtogroup INTRMAIN
 *  \brief Declared at <intrapi.hxx>, SPAintr
 *  @{
 */

/**
 * Initializes the intersector library.
 * <br><br>
 * <b>Effect:</b> System routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
**/
DECL_INTR outcome api_initialize_intersectors();

/**
 * Terminates the intersector library.
 * <br><br>
 * <b>Effect:</b> System routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
**/
DECL_INTR outcome api_terminate_intersectors();

/** @} */

/** \addtogroup INTRAPICONTAINMENT
 *  \brief Declared at <queryapi.hxx>, SPAquery
 *  @{
 */

/**
 * Determines the relationship of a position within a given face's surface.
 * <br><br>
 * <b>Role:</b> This function returns the containment of a given point <b>on
 * the face's surface</b> for a given face. There are four possible outcomes:
 * <tt>point_inside_face</tt>, <tt>point_outside_face</tt>, <tt>point_boundary_face</tt>,
 * and <tt>point_unkown_face</tt>.
 * <br><br>
 * The transformation will be applied only if specified. Typically this is the
 * transformation from the body that contains the face.
 * <br><br>
 * The <tt>test_uv_guess</tt> argument provides additional help by providing a coordinate in
 * parametric space for an initial guess on where the position may lie. This may
 * help to improve the performance.
 * <br><br>
 * If a previous position is specified, a containment description must be provided
 * (<tt>point_inside_face</tt>, <tt>point_outside_face</tt>, <tt>point_boundary_face</tt> or
 * <tt>point_unkown_face</tt>). This position can help find the position containment on the
 * face at a faster rate. It is recomended to use a position that was previously
 * found using this function. If the previous position passed in is a <tt>NULL</tt> <tt>REF</tt> it
 * will call the other <tt>api_point_in_face</tt> function.
 * <br><br>
 * If <tt>api_point_in_face</tt> is called often on the same face, then setting <tt>use_cache</tt> to
 * <tt>TRUE</tt> will increase the performance. If <tt>api_point_in_face</tt> is called with the
 * <tt>use_cache</tt> set to <tt>TRUE</tt> an attribute will be attached to the face storing the
 * cached information. If <tt>api_point_in_face</tt> is called subsequently on a face with
 * the cached attribute information, but the <tt>use_cache</tt> flag is set to <tt>FALSE</tt>, the
 * information will be ignored.
 * <br><br>
 * Cache size refers to how many levels of cache are used to store and search
 * previously found data. Default is 10.
 * <br><br>
 * <b>Limitations:</b> If the input point is not on the face's surface, the behavior of this
 * function is undefined. For performance reasons this is not trapped by the function, so
 * the api will not fail in this situation, but the relationship returned is not meaningful.
 * In particular, the result <tt>point_outside_face</tt> is not guaranteed to be returned when this
 * function is called with a point not on the face's surface.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
* <b>Journal: </b> Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param test_point
 * point in question.
 * @param test_face
 * face to test point against.
 * @param face_trans
 * transformation of the face.
 * @param cont_answer
 * relationship found (<tt>point_inside_face</tt>, <tt>point_outside_face</tt>, <tt>point_boundary_face</tt>, and <tt>point_unkown_face</tt>).
 * @param test_uv_guess
 * Spline face  case, uv-coordinates guess of the testpoint use cached entries.
 * @param use_cache
 * use cached entries.
 * @param cache_size
 * size of array used to cache interior points.
 * @param ao
 * options such as journaling and versioning.
 **/
DECL_INTR outcome api_point_in_face(
            SPAposition const       &test_point,        // point in question
            FACE                    *test_face,         // face to test point against
            SPAtransf const         &face_trans,        // transformation of the face
            point_face_containment  &cont_answer,       // relationship found (point_inside_face,
                                                        // point_outside_face, point_boundary_face,
                                                        // and  point_unkown_face)
            SPApar_pos const        &test_uv_guess =    // spline face case, guess of uv-coordinates of the testpoint
                 *(SPApar_pos *)NULL_REF,
            logical                 use_cache   = FALSE,// use cached entries
            int                     cache_size  = 10,   // use cached entries
            AcisOptions             *ao         = NULL  // options such as journaling and versioning
         );

/**
 * Determines the relationship of a position within a given face's surface.
 * <br><br>
 * <b>Role:</b> This function returns the containment of a given point <b>on
 * the face's surface</b> for a given face. There are four possible outcomes:
 * <tt>point_inside_face</tt>, <tt>point_outside_face</tt>, <tt>point_boundary_face</tt>,
 * and <tt>point_unkown_face</tt>.
 * <br><br>
 * The transformation will be applied only if specified. Typically this is the
 * transformation from the body that contains the face.
 * <br><br>
 * The <tt>test_uv_guess</tt> argument provides additional help by providing a coordinate in
 * parametric space for an initial guess on where the position may lie. This may
 * help to improve the performance.
 * <br><br>
 * If a previous position is specified, a containment description must be provided
 * (<tt>point_inside_face</tt>, <tt>point_outside_face</tt>, <tt>point_boundary_face</tt> or
 * <tt>point_unkown_face</tt>). This position can help find the position containment on the
 * face at a faster rate. It is recomended to use a position that was previously
 * found using this function. If the previous position passed in is a <tt>NULL</tt> <tt>REF</tt> it
 * will call the other <tt>api_point_in_face</tt> function.
 * <br><br>
 * If <tt>api_point_in_face</tt> is called often on the same face, then setting <tt>use_cache</tt> to
 * <tt>TRUE</tt> will increase the performance. If <tt>api_point_in_face</tt> is called with the
 * <tt>use_cache</tt> set to <tt>TRUE</tt> an attribute will be attached to the face storing the
 * cached information. If <tt>api_point_in_face</tt> is called subsequently on a face with
 * the cached attribute information, but the <tt>use_cache</tt> flag is set to <tt>FALSE</tt>, the
 * information will be ignored.
 * <br><br>
 * Cache size refers to how many levels of cache are used to store and search
 * previously found data. Default is 10.
 * <br><br>
 * <b>Limitations:</b> If the input point is not on the face's surface, the behavior of this
 * function is undefined. For performance reasons this is not trapped by the function, so
 * the api will not fail in this situation, but the relationship returned is not meaningful.
 * In particular, the result <tt>point_outside_face</tt> is not guaranteed to be returned when this
 * function is called with a point not on the face's surface.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
* <b>Journal: </b> Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param test_point
 * point in question.
 * @param test_face
 * face to test point against.
 * @param face_trans
 * transformation of the face.
 * @param cont_answer
 * relationship found (<tt>point_inside_face</tt>, <tt>point_outside_face</tt>, <tt>point_boundary_face</tt>, and <tt>point_unkown_face</tt>).
 * @param test_uv_guess
 * spline facecase, guess of uv-coordinates of the testpoint.
 * @param prev_point_input
 * previous point.
 * @param prev_cont
 * previous point's containment.
 * @param use_cache
 * use cached entries.
 * @param cache_size
 * size of array used to cache interior points.
 * @param ao
 * options such as journaling and versioning.
 **/
DECL_INTR outcome api_point_in_face(
            SPAposition const       &test_point,        // point in question
            FACE                    *test_face,         // face to test point against
            SPAtransf   const       &face_trans,        // transformation of the face
            point_face_containment  &cont_answer,       // relationship found (point_inside_face,
                                                        // point_outside_face, point_boundary_face,
                                                        // and  point_unkown_face)
            SPApar_pos  const       &test_uv_guess,     // spline face case, guess of uv-coordinates of the testpoint
            SPAposition const       &prev_point_input,  // previous test point
            point_face_containment  prev_cont,          // previous test point containment
            logical                 use_cache  = FALSE, // use cached entries
            int                     cache_size = 10,    // use cached entries
            AcisOptions             *ao        = NULL   // options such as journaling and versioning
         );


/** @} */
/** \addtogroup INTRAPISELFINT
 *  \brief Declared at <intrapi.hxx>, SPAintr
 *  @{
 */

/**
 * Locates self-intersections of a CURVE.
 * <br><br>
 * <b>Role:</b> This API examines a CURVE for points or intervals of
 * self-intersection within the specified parameter range and results in the
 * structure representing the intersection of two curves.
 * <br><br>
 * Use the following guidelines to interpret <tt>curve_curve_int</tt> information:
 * <br><br>
 * First, because the <tt>curve_curve_int</tt> record is meant for intersection of two
 * curves, the information given for the second curve in the record is a
 * duplication of the information given for first/original curve.
 * <br><br>
 * Second, if the <tt>curve_curve_rel</tt> is <tt>cur_cur_unknown</tt>, the intersection is an
 * isolated point.
 * <br><br>
 * Third, if the <tt>curve_curve_rel</tt> is <tt>cur_cur_coin</tt>, the interval covered between this
 * record and the next record (ignore the relation in the next record) gives the
 * interval of overlap in the curve.
 * <br><br>
 * Except for the preceding relations, the <tt>cur_cur_int</tt> record for
 * self-intersections does not contain any other relations. Also, it does not
 * interpret the information in the record to obtain other relationships as it does
 * for intersections of different curves.
 * <br><br>
 * <b>Limitations:</b> If the start parameter equals the end parameter, the API returns
 * the point corresponding on the curve at given parameter in the self-intersection
 * record.
 * <br><br>
 * If the CURVE given is <tt>NULL</tt>, this API does not return the isolated vertex point
 * corresponding to an edge from which this <tt>NULL</tt> geometry might have come. To get
 * that point, call <tt>api_ed_self_inters</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param crv
 * CURVE which is interrogated.
 * @param start_par
 * start parameter.
 * @param end_par
 * end parameter.
 * @param self_intersection
 * information corresponding to self-intersection returned.
 * @param ao
 * ACIS options.
 **/
DECL_INTR outcome api_crv_self_inters(
			CURVE               *crv,			    // Curve which is interrogated
			double              start_par,		    // Start_parameter
			double              end_par,		    // End SPAparameter
			curve_curve_int     *&self_intersection,// Intersection record returned
		    AcisOptions         *ao = NULL);


/**
 * Determines if a curve that defines an edge self-intersects.
 * <br><br>
 * <b>Role:</b> This API returns a <tt>curve_curve_int</tt> record containing the
 * self-intersection information for the curve that defines the edge.
 * <br><br>
 * Use the following guidelines to interpret the <tt>curve_curve_int</tt> information.
 * <br><br>
 * Because the <tt>curve_curve_int</tt> record is meant for intersection of two curves, the
 * information given for the second curve in the record duplicates of the
 * information given for the first/original curve.
 * <br><br>
 * If the <tt>curve_curve_rel</tt> is <tt>cur_cur_unknown</tt>, the intersection is an isolated point.
 * <br><br>
 * If the <tt>curve_curve_rel</tt> is <tt>cur_cur_coin</tt>, the interval covered between this record
 * and the next record (ignore the relation in the next record) gives the interval
 * of overlap in the curve.
 * <br><br>
 * Except for above relations, the <tt>cur_cur_int</tt> record for self-intersections will
 * not contain any other relations. Also, it will not interpret the information in
 * the record to obtain other relationships as it will for intersections of two
 * different curves.
 * <br><br>
 * A side effect of this api is that it deletes the list of <tt>curve_curve_int</tt> objects
 * pointed to by <tt>self_intersection</tt> on entry. An important consequence of this is that
 * <tt>self_intersection</tt> <b>must</b> either point to a valid <tt>curve_curve_int</tt> list
 * or be NULL on entry.
 * <br><br>
 * <b>Errors:</b> <tt>NULL</tt> edge pointer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
* <b>Journal: </b> Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param edge
 * edge that is interrogated.
 * @param self_intersection
 * information corresponding to self-intersection returned (in/out - previous list is deleted).
 * @param ao
 * ACIS options.
 **/
DECL_INTR outcome api_ed_self_inters(
            EDGE            *edge,				    // Edge which is interrogated
			curve_curve_int *& self_intersection,   // Intersection record that is returned
		    AcisOptions     *ao = NULL);


/** @} */
/** \addtogroup INTRAPICONVEXITY
 *  \brief Declared at <intrapi.hxx>, SPAintr
 *  @{
 */

/**
 * Analyzes the convexity of an edge at a given parameter value.
 * <br><br>
 * <b>Role:</b> This API takes an edge and a parameter value as the arguments
 * and analyzes the convexity of the edge at that parameter value. The edge
 * convexity is reported as an enumerated type of type <tt>bl_ed_convexity</tt>.
 * <br><br>
 * A return value of <tt>bl_ed_convex_smooth</tt>, <tt>bl_ed_concave_smooth</tt>, or
 * <tt>bl_ed_smooth</tt> signifies a smooth edge.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
* <b>Journal: </b> Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param edge
 * given edge.
 * @param p
 * parameter value at which to interrogate.
 * @param cxty
 * returned convexity value.
 * @param ao
 * ACIS options.
 **/
DECL_INTR outcome api_edge_convexity_param(
            EDGE            *edge,          // The edge to be interrogated
            double          p,              // The param value to be tested at
            bl_ed_convexity &cxty,          // Returned value of the convexity
            AcisOptions     *ao = NULL);


/** @} */
/** \addtogroup INTRAPIISOLINES
 *  \brief Declared at <intrapi.hxx>, SPAintr
 *  @{
 */

/**
 * Generates a list of edges that lie on the isoparametric curves and are trimmed to the boundaries of the face.
 * <br><br>
 * <b>Role:</b> This API computes a <i>uv</i>-parameter box enclosing the face. The
 * <i>u</i>-range is then subdivided into <tt>nu</tt> equal intervals and a <i>u</i>-isoparametric curve
 * is calculated at the boundary of the intervals and trimmed to the boundaries of
 * the face to get the edges. The <i>v</i>-iso curves are handled similarly. Because the
 * parameter space box is not necessarily minimal, it is possible that the iso
 * curve does not lie in the face and no edges are generated for this parameter
 * value. The edges always have the associated transformation applied.
 * <br><br>
 * <b>Errors:</b> Pointer to face is <tt>NULL</tt> or not to a <tt>FACE</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param nu
 * number of u-curves.
 * @param nv
 * number of v-curves.
 * @param face
 * face on which edges are calculated.
 * @param ftrans
 * transformation positioning face.
 * @param edge_list
 * list of new edges returned.
 * @param ao
 * ACIS options.
 **/
DECL_INTR outcome api_face_nu_nv_isolines(
            int             nu,		    // number of U curves
			int             nv,		    // number of V curves
 			FACE            *face,	    // face on which to calculate iso-curves
 			const SPAtransf &ftrans,    // transformation positioning face
 			ENTITY_LIST     *edge_list, // edges are returned in list
		    AcisOptions     *ao = NULL);

/**
 * Generates a list of edges that lie on a <i>u</i>-parametric curve and are trimmed to the boundary of a face.
 * <br><br>
 * <b>Role:</b> This API computes the <i>u</i>-parametric curves on the surface of the
 * face and trims them to the boundaries of the face to get the edges. It uses the
 * face transformation to calculate the edges.
 * <br><br>
 * <b>Errors:</b> Pointer to face is <tt>NULL</tt> or not to a <tt>FACE</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param v
 * v parameter.
 * @param face
 * face on which edges are calculated.
 * @param ftrans
 * transformation positioning face.
 * @param edge_list
 * new edges returned in list.
 * @param ao
 * ACIS options.
 **/
DECL_INTR outcome api_face_u_iso(
            double          v,		    // U SPAparameter
 			FACE            *face,	    // face on which to calculate iso-curve
 			const SPAtransf &ftrans,    // transformation positioning face
 			ENTITY_LIST     *edge_list,	// edges are returned in list
 		    AcisOptions     *ao = NULL);

/**
 * Generates a list of edges that lie on a <i>v</i>-parametric curve and are trimmed to the boundary of a face.
 * <br><br>
 * <b>Role:</b> This API computes the <i>v</i>-parametric curves on the surface of the
 * face and trims them to the boundaries of the face to get the edges. It uses the
 * face transformation to calculate the edges.
 * <br><br>
 * <b>Errors:</b> Pointer to face is <tt>NULL</tt> or not to a <tt>FACE</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param u
 * u parameter.
 * @param face
 * face on which edges are calculated.
 * @param ftrans
 * transformation positioning face.
 * @param edge_list
 * new edges returned in list.
 * @param ao
 * ACIS options.
 **/
DECL_INTR outcome api_face_v_iso(
 			double          u,		    // U SPAparameter
 			FACE            *face,      // face on which to calculate iso-curve
 			const SPAtransf &ftrans,    // transformation positioning face
 			ENTITY_LIST     *edge_list, // edges are returned in list
 		    AcisOptions     *ao = NULL);


/** @} */
/** \addtogroup INTRAPIFACETCURVE
 *  \brief Declared at <intrapi.hxx>, SPAintr
 *  @{
 */

/**
 * Creates facets for a curve.
 * <br><br>
 * <b>Role:</b> This API calculates a list of points and parameter values that
 * approximate the given curve to within the specified tolerance.
 * <br><br>
 * <b>Errors:</b> Tolerance specified is not greater than zero.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param crv
 * curve.
 * @param a
 * starting parameter.
 * @param b
 * ending parameter.
 * @param tol
 * tolerance.
 * @param nmax
 * maximum number of points to generate.
 * @param npts
 * number of points generated (set to <i>nmax</i> +1 if <i>nmax</i> exceeded).
 * @param pts
 * points on curve returned.
 * @param t
 * parameter value at point returned.
 * @param ao
 * ACIS options.
 **/
DECL_INTR outcome api_facet_curve(
			const curve & crv,      // curve to approximate with linear polygons
			double      a,          // starting SPAparameter
			double      b,          // ending SPAparameter
			double      tol,        // tolerance
			int         nmax,       // max number of points to generate
			int         &npts,      // number of points generated
									// (set to nmax+1 if nmax exceeded)
			SPAposition pts[],      // points on curve
			double      t[],        // SPAparameter value at point
		    AcisOptions *ao = NULL);


/** @} */
/** \addtogroup INTRAPICCI
 *  \brief Declared at <intrapi.hxx>, SPAintr
 *  @{
 */

/**
 * Intersects two edges, producing a list of curve-curve intersection records.
 * <br><br>
 * <b>Role:</b> The records contain the locations of the intersection and the
 * nature of the intersection (coincident, etc.).
 * <br><br>
 * To turn the list of intersection records into the <tt>POINTs</tt> and <tt>EDGEs</tt> of the
 * intersection, use @href api_ed_inters_to_ents.
 * <br><br>
 * <b>Limitations:</b>
 * <ul>
 * <li>This API does not take body transforms into account.</li>
 * </ul>
 * <br>
 * <b>Errors:</b> Pointer to edge is <tt>NULL</tt> or not to an <tt>EDGE</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param e1
 * first edge.
 * @param e2
 * second edge.
 * @param inters
 * list of intersection records returned.
 * @param ao
 * ACIS options.
 **/
DECL_INTR outcome api_inter_ed_ed(
            EDGE            *e1,	    // first edge
			EDGE            *e2,	    // second edge
			curve_curve_int *&inters,   // list of intersection records
		    AcisOptions     *ao = NULL);

DECL_INTR outcome gme_api_inter_ed_ed(
            EDGE            *e1,	    // first edge
			EDGE            *e2,	    // second edge
			curve_curve_int *&inters,   // list of intersection records
		    AcisOptions     *ao = NULL);

// Compute the intersections of two curves
/**
 * Intersects two edges or two curves, producing a list of curve-curve intersection records.
 * <br><br>
 * <b>Role:</b> This API returns a list of all intersections between the specified edges 
 * or their underlying curves. If <tt>bounded</tt> is <tt>TRUE</tt>, the API returns only intersections 
 * within the parameter ranges of both the edges (<tt>crv1</tt> and <tt>crv2</tt>); otherwise, it 
 * returns intersections that occur on the extensions of the curves. However, extensions are only
 * considered when both the curves are either straight lines or circles.
 * <br><br>
 * This API uses analytic equations when both the curves are either straight lines or circles. For more
 * complex cases, it intersects the B-spline approximations associated with the two supplied curves. Thus,
 * the intersection results returned by this API are for the approximate B-spline curves, and not for the 
 * true curve geometries. For accurate intersections corresponding to true curve geometries,
 * use @href api_inter_ed_ed instead. 
 * <br><br>
 * <b>Limitations:</b>
 * <ul>
 * <li>If <tt>bounded</tt> is <tt>FALSE</tt>, the extensions of the input curves are not taken into account 
 * unless both the curves are either straight lines or circles.</li>
 * </ul>
 * <br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param crv1
 * first curve.
 * @param crv2
 * second curve.
 * @param bounded
 * find only intersections within edge bounds.
 * @param inters
 * list of intersection records returned.
 * @param ao
 * ACIS options.
 **/
DECL_INTR outcome api_intersect_curves(
            EDGE            * crv1,     // first curve
            EDGE            * crv2,     // second curve
            logical         bounded,    // find only intersections within edge bounds?
            curve_curve_int *& inters,  // list of intersection records
            AcisOptions     *ao = NULL);


/** @} */
/** \addtogroup INTRAPISILHOUETTE
 *  \brief Declared at <intrapi.hxx>, SPAintr
 *  @{
 */

/**
 * Generates a list of silhouette edges for a given face.
 * <br><br>
 * <b>Role:</b> This API computes the silhouette curves on the surface of the
 * face and trims them to the boundaries of the face.
 * <br><br>
 * The silhouette curves can be calculated for either a parallel or perspective
 * projection.
 * <br><br>
 * It creates a list of <tt>EDGEs</tt> representing the bounded portion of the silhouette
 * curves within the face.
 * <br><br>
 * <dl>
 * <b>Errors:</b> Pointer to face is <tt>NULL</tt> or not to a <tt>FACE</tt>.
 * <dd>&nbsp;From and to points are coincident.</dd>
 * </dl>
 * <br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param face
 * face to be examined.
 * @param ftrans
 * transform.
 * @param from_point
 * position of viewer.
 * @param to_point
 * position toward which the viewer looks.
 * @param projection_type
 * (0 = parallel, 1 = perspective).
 * @param edgelist
 * list of new silhouette edges returned.
 * @param ao
 * ACIS options.
 **/
DECL_INTR outcome api_silhouette_edges(
            FACE                *face,		    // face for which silhouette edges are calculated
			const SPAtransf     &ftrans,	    // transformation for face
			const SPAposition   &from_point,    // from point
			const SPAposition   &to_point,      // to point
			int projection_type,			    // projection type (0=>parallel, 1=>perspective)
			ENTITY_LIST         *edgelist,	    // list of silhouette edges
		    AcisOptions         *ao = NULL);


/**
 * Generates a list of silhouette edges for a given face.
 * <br><br>
 * <b>Role:</b> This API computes the silhouette curves on the surface of the
 * face and trims them to the boundaries of the face.
 * <br><br>
 * The silhouette curves can be calculated for either a parallel or perspective
 * projection.
 * <br><br>
 * It creates a list of <tt>EDGEs</tt> representing the bounded portion of the silhouette
 * curves within the face.
 * <br><br>
 * <dl>
 * <b>Errors:</b> Pointer to face is <tt>NULL</tt> or not to a <tt>FACE</tt>.
 * <dd>&nbsp;From and to points are coincident.</dd>
 * </dl>
 * <br>
 * <b>Effect:</b> Changes model
 * <br><br>
* <b>Journal: </b> Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param face
 * face to be examined.
 * @param ftrans
 * transform.
 * @param from_point
 * position of viewer.
 * @param to_point
 * position toward which the viewer looks.
 * @param projection_type
 * (0 = parallel, 1 = perspective).
 * @param edgelist
 * list of new silhouette edges returned.
 * @param sil_opts
 * silhouette_options object.
 * @param ao
 * ACIS options.
 **/

DECL_INTR outcome api_silhouette_edges(
            FACE                *face,		    // face for which silhouette edges are calculated
			const SPAtransf     &ftrans,	    // transformation for face
			const SPAposition   &from_point,    // from point
			const SPAposition   &to_point,      // to point
			int projection_type,			    // projection type (0=>parallel, 1=>perspective)
			ENTITY_LIST         *edgelist,	    // list of silhouette edges
			silhouette_options  *sil_opts,
		    AcisOptions         *ao = NULL);


/** @} */
/** \addtogroup ACISCHECKING
 *  \brief Declared at <intrapi.hxx>, SPAintr
 *  @{
 */

/**
 * Analyzes a curve for <tt>C1</tt> or <tt>G1</tt> discontinuities.
 * <br><br>
 * <b>Role:</b> Given an edge, this function examines the underlying curve and
 * returns a list of parametric space points where there are <tt>C1/G1</tt> discontinuities.
 * The points are returned in a class called <tt>curve_irregularities</tt>. <tt>C1</tt> 
 * discontinuities are discontinuities of tangent direction or magnitude.
 * <tt>G1</tt> discontinuities are discontinuities of tangent direction only. 
 * Therefore, if a discontinuity of tangent direction occurs, regardless of a
 * corresponding discontinuity of tangent magnitude, then a <tt>G1</tt> discontinuity exists. 
 * If a discontinuity of tangent magnitude occurs, it implies a <tt>C1</tt>
 * discontinuity exists. 
 * <br><br>
 * The discontinuities can be of the following types:
 * <ul>
 * <li>Tangents' magnitudes differ</li>
 * <li>Tangents' directions differ</li>
 * </ul>
 * <br>
 * <b>Errors:</b> The pointer to an edge is <tt>NULL</tt> or does not point to an <tt>EDGE</tt>.
 * <br><br>
 * <b>Limitations:</b> This API works only for <tt>intcurves</tt> because other curves are
 * always continuous.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param given_edge
 * edge to examine.
 * @param cirr
 * pointer to list of irregularities returned.
 * @param no_pts
 * number of irregularities returned.
 * @param ao
 * ACIS options.
 **/
DECL_INTR outcome api_check_cur_smoothness(
            EDGE                    *given_edge,
			curve_irregularities    *&cirr,
			int                     &no_pts,
		    AcisOptions             *ao = NULL);

/**
 * Calls the ACIS Checker and checks an entity's geometry, topology, and data structure
 * for bad data or invalidities. 
 * <b>Technical Article:</b> <i>[ACIS Checker](http://doc.spatial.com/articles/a/c/i/Component~ACIS_Checker_c03c.html)</i>, <i>[Checking Polyhedral Bodies](http://doc.spatial.com/articles/c/h/e/Checking_Polyhedral_Bodies_4d66.html)</i>
 * <br><br>
 * <b>Role:</b> This API traverses the given entity's topological tree structure,
 * checking for bad data or invalidities along the way.  Any problems found are added
 * to an @href insanity_list and returned to the caller via the given pointer.  A NULL
 * @href insanity_list indicates that no errors have been found. 
 * <br><br>
 * Each object in the @href insanity_list represents a single problem with the input entity
 * identified by the Checker, specifying the severity of the problem, the particular
 * problem identified, and the entity exhibiting the problem. Note that as most
 * entities own other sub-entities, the entities returned in the list are typically
 * not the input entity. For information on how to access the information from an
 * @href insanity_list , please refer to the documentation for @href insanity_list and
 * @href insanity_data . 
 * <br><br>
 * Each problem returned by <tt>api_check_entity</tt> is classified as being of one of four
 * different severities: ERROR, WARNING, NOTE, and INFO. Refer to "Table 1" and "Table 2" 
 * in the <i>Types of Checks</i> Technical Article for information on the checks performed.
 * <br><br>
 * The option <tt>check_level</tt> controls the level of computational effort performed by the
 * Checker. The range of effective check levels is any integer multiple of 10,
 * between 10 and 70 inclusive; the default value is 20. Any other value is invalid,
 * and the behavior of the Checker may be unpredictable with an invalid check level.
 * <br><br>
 * Each check in the Checker is allocated a check level at which it becomes
 * effective, although some checks may be forced at a lower level with the use of an
 * option. The Checker performs all checks allocated to levels less than or equal to
 * the set <tt>check_level</tt>.
 * Therefore, level 10 is the fastest and least thorough, and
 * level 70 is the slowest and most thorough. It is not the case that higher-level
 * check errors are less serious than lower-level check errors. Please refer to the
 * <i>Types of Checks</i> Technical Article for more information.
 * <br><br>
 * The option <tt>show_warning_msg</tt> controls whether or not WARNING-severity checks are performed.
 * By default, this option is set to FALSE in release builds and TRUE in debug builds.
 * <br><br>
 * The option <tt>get_aux_msg</tt> controls whether or not auxiliary information
 * should be included for selected Checker problems. By default, this option is set
 * to TRUE. Generally, when available, auxiliary information contains parametric or
 * position data specifically relating to the problem. This information is available
 * in string form only, via the @href insanity_data#print_message method.
 * <br><br>
 * The option <tt>check_abort</tt> controls whether or not the Checker aborts after
 * the first problem is found.  By default, this option is set to FALSE.   
 * <br><br>
 * The option <tt>check_fix</tt> controls whether or not the Checker should attempt
 * to repair selected problems. By default, this option is set to FALSE.  If the
 * <tt>check_fix</tt> option is switched on and a repairable problem is detected, the
 * repair is made and the problem is not reported.  Extreme caution is advised when
 * using this option. 
 * <br><br>
 * An example of the usage of this API, including the displaying of reported Checker
 * problems, can be found in <tt>scm/scmext/intr/chk_scm.cpp</tt>. Customers are
 * advised against including the ACIS checking functionality as part of their
 * modelling workflow.   Instead, the models should be checked on read-in, and
 * repaired before modelling commences.  Please refer to the section 
 * <i>ACIS Checker</i> for more information.
 * <br><br>
 * Each check problem has a unique integer ID, obtainable via the @href insanity_data#get_insane_id method . 
 * An internal description of the problem, in
 * the form of 
 * a string, is obtainable via the @href insanity_data#get_message method.  It is
 * not recommended that applications make use of the message strings, particularly
 * for determining workflow.  Instead, the unique integer IDs should be used.  For
 * ease of migration to future versions of ACIS, it is recommended that applications
 * use the corresponding ID macros, which can be found in <tt>insanity.err</tt>. 
 * <br><br>
 * Certain check problems may have further subclassification information, available
 * via the @href insanity_data#get_sub_category and @href
 * insanity_data#get_par_pos methods.  Currently, the only check problem that
 * supports such 
 * information is IRREG_SURF ("surface irregular").   For further information, please
 * refer to the documentation for @href insanity_sub_category . 
 * <br><br>
 * Tables of all checks performed, and the associated problems that may be reported,
 * can be found in the Technical Article on the ACIS Checker.
 * <br><br>
 * <b>Errors:</b> <tt>given_entity</tt> is a NULL pointer.
 * <br><br>
 * <b>Effect:</b> In normal use, does not change model, but derived data may be
 * computed and cached. If fixing options have been activated, model will be
 * modified, if and when fixes are applied. If unexpanded patterns exist, 
 * topological entities may be created.
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
 * @param given_entity
 * (in) Entity to be checked.
 * @param list
 * (out) List of problem entities with info.
 * @param ao
 * (in) ACIS options such as version and journal.
 **/
DECL_INTR outcome api_check_entity(
            const ENTITY    *given_entity, // entity to check
			insanity_list   *&list,
		    AcisOptions     *ao = NULL);


DECL_INTR outcome api_brep_health_report(
            const ENTITY    *given_entity, // health report for this entity
			insanity_list   *&list,
		    AcisOptions     *ao = NULL);

// This api will become obsolete. Use api_check_entity(const ENTITY*, insanity_list*&, FILE*).
/**
 * @nodoc 
 * <b>Obsolete:</b> use @href api_check_entity (that is, api_check_entity(ENTITY*, insanity_list*&, AcisOptions*)) instead.
 * <br><br>
 * Checks an entity's geometry, topology, and data structure for errors.
 * <br><br>
 * <b>Role:</b> A given entity, empty problem entity list, and, optionally, a file pointer are
 * passed to this function. The given entity is traversed and checked for
 * problems (see checks listed below) and, if any problem entities are found they
 * are added to the <tt>problem_ents</tt> list that is returned to the caller. The results
 * of the check are written to the file defined by <tt>file_ptr</tt>, if provided, otherwise
 * the results are written to the standard output device.
 * <br><br>
 * The option <tt>get_aux_msg</tt> can be used to control the level of detail for the
 * information to be added to the list.
 * <br><br>
 * The option <tt>show_warning_msg</tt> can be used to control whether less serious problems
 * that are classified as warnings rather than errors are returned or not. Note that the default
 * value for this option is different in debug and release builds. For release builds, it is
 * turned off by default (so only error-status problems are returned), but in debug builds it
 * is turned on by default (so all problems are returned).
 * <br><br>
 * The messages corresponding to the insanity information provided by this API
 * are available in the file <tt>insanity.err</tt>. The mechanism for this 
 * is similar to that of sys_error.
 * <br><br>
 * If the user wants to see the list of checks made when using either of the two
 * functions, add the following lines of code to your program:
 * <pre>
 * sanity_ctx& ctx = intersct_context()->sanity();
 * printf("\n");
 * printf( "checked:\n" );
 * printf( "%d lumps\n", ctx.lump_num );
 * printf( "%d shells\n", ctx.shell_num );
 * printf( "%d wires\n", ctx.wire_num );
 * printf( "%d faces\n", ctx.face_num );
 * printf( "%d loops\n", ctx.loop_num );
 * printf( "%d coedges\n", ctx.coedge_num );
 * printf( "%d edges\n", ctx.edge_num );
 * printf( "%d vertices\n", ctx.vertex_num );
 * if(ctx.law_num) printf( "%d law\n", ctx.law_num);</pre>
 * A sample output for the above code is as follows:
 * <pre>
 * checked:
 * 1 lumps
 * 1 shells
 * 0 wires
 * 3 faces
 * 3 loops
 * 6 coedges
 * 3 edges
 * 2 vertices</pre>
 * An example usage of this API can be found in <tt>intr/intr_scm/chk_scm.cxx</tt>.
 * <br><br>
 * The level of checking and the output reported are controlled by the options
 * <tt>check_level</tt> and <tt>check_output</tt>. If the <tt>check-level</tt> defined is not a multiple of
 * ten, it is treated as the next lowest valid number. Any number over 70 is equal
 * to 70. Entity checking is aborted after the first problem is found if the option
 * <tt>check_abort</tt> is on.
 * <br><br>
 * The valid check levels are:<br>
 * 10  = Fast error checks<br>
 * 20  = Level 10 checks plus slower error checks (Default)<br>
 * 30  = Level 20 checks plus curve and surface checks<br>
 * 40  = Level 30 checks plus fast warning checks<br>
 * 50  = Level 40 checks plus slower warning checks<br>
 * 60  = Level 50 checks plus slow edge convexity change point checks<br>
 * 70  = Level 60 checks plus face-face intersection checks
 * <br><br>
 * <i><b>Note:</b></i> Many of the errors with pcurves can be fixed by forcing the pcurves to be
 * regenerated.  The scheme command for doing this is: <tt>entity:reset-pcurves</tt>.
 * <br>
 * This scheme command executes the following functions:<br>
 * <ul>
 * <li><tt>sg_rm_pcurves_from_entity(ENTITY *ent, logical analytic_only=FALSE, logical do_tcoedges=FALSE)</tt><br>
 * (where <tt>analytic_only</tt> means only pcurves on analytic surfaces are to be removed
 * and <tt>do_tcoedges</tt> means that pcurves from tolerant coedges are to be removed. The
 * last parameter should never be set to <tt>TRUE</tt>)</li>
 * <li><tt>sg_add_pcurves_to_entity(ENTITY *ent)</tt>.</li>
 * </ul>
 * <br>
 * <b>Data Structure Checks</b>
 * <br><br>
 * <ul>
 * <li>The parent has an appropriate child-level entity; e.g., body has lump</li>
 * <br><br>
 * <li>Presence (non-<tt>NULL</tt>) and closure of back pointer from child to parent; e.g., body's lump points to body</li>
 * <br><br>
 * <li>The coedge on a spline surface has a pcurve</li>
 * <br><br>
 * <li>Pcurve indexing (0/+-1/+-2) is appropriate</li>
 * <br><br>
 * <li>The pcurve has a non-<tt>NULL</tt> <tt>2D</tt> <tt>B</tt>-spline curve</li>
 * <br><br>
 * <li>If edge has a non-<tt>NULL</tt> curve, then the curve must have an equation</li>
 * </ul>
 * <br>
 * <b>Topological Checks</b>
 * <br><br>
 * <ul>
 * <li>Loops are closed in both the next and previous directions</li>
 * <br><br>
 * <li>Apex edge loops are correct</li>
 * <br><br>
 * <li>Coedge has a partner, except an apex coedge</li>
 * <br><br>
 * <li>All coedge partners point to the same edge</li>
 * <br><br>
 * <li>Sequential coedges share a vertex</li>
 * <br><br>
 * <li>Edge is in exactly one of start and end vertex edge groups. For example,
 * edge can be reached for one value of <i>i </i>using <tt>start()->edge(i)->coedge()</tt> and
 * partner and next (or previous) pointers.</li>
 * </ul>
 * <br>
 * <b>Geometric Checks</b>
 * <br><br>
 * <ul>
 * <li>Entities with geometry, must have non-<tt>NULL</tt> geometry. For example, a face
 * points to a surface.</li>
 * <br><br>
 * <li>Analytic surfaces have valid definitions:</li>
 * <ol type = "a">
 * <li>plane must a unit vector normal</li>
 * <li>sphere must have a nonzero radius</li>
 * <li>cone has:</li>
 * <ol>
 * <li>unit vector normal</li>
 * <li>nonzero length major axis</li>
 * <li>normal and major axis are perpendicular</li>
 * <li>0 &lt; ratio &lt; 1</li>
 * <li>sin_angle^2 + cos_angle^2 = 1</li>
 * </ol>
 * <li>torus has:</li>
 * <ol>
 * <li>unit vector normal</li>
 * <li>major radius not equal to 0</li>
 * <li>minor radius not equal to 0</li>
 * <li>major radius &gt;= -fabs( minor radius)</li>
 * </ol> </ol>
 * <br>
 * <li>Pcurve surface matches face surface (warning only if not equal since
 * surface could be trimmed).</li>
 * <br><br>
 * <li>Pcurve form is agrees with curve form, for example, closed, open, periodic.</li>
 * <br><br>
 * <li>Pcurve parameter period agrees with curve period.</li>
 * <br><br>
 * <li>Pcurve at points 0, 1/3, 2/3, and one way along the curve must lie on
 * the edge, and tangent directions at these points must roughly agree, i.e., have
 * a positive dot product. This also tests the following:</li>
 * <ol type = "a">
 * <li>Pcurve and edge geometry direction agree (up to sense),</li>
 * <li>Start and end parameters of pcurve match those of coedge</li>
 * <li>Start and end locations of pcurve (wrt to surface) match those of coedge</li>
 * </ol>
 * <br>
 * <li>Spline surface form is set correctly, for example, surfaces closed in <i>u</i> report
 * this. Checks the underlying <tt>3D</tt> <tt>B</tt>-spline surface at 10 points along seam to
 * verify form.</li>
 * <br><br>
 * <li>Checks that coedge vertices do not lie on spline surface singularities.</li>
 * <br><br>
 * <li>Face normal is consistent with coedge direction, that is,</li>
 * <ol type = "a">
 * <li>Face area is greater than 0</li>
 * <li>Multi-loop faces have loops correctly oriented</li>
 * <li>All loops contain the others</li>
 * </ol>
 * <br>
 * <li>Surface checks (performed at check level 30 or higher, or if the option
 * <tt>d3_checks</tt> is turned on). Checks for a bad degeneracy
 * - adjacent degenerate boundaries. The remaining checks are just on spline
 * surfaces:</li>
 * <ol type = "a">
 * <li>surface irregular. It is possible for this to be further sub-categorized
 * as being due to high surface curvature, or evaluation problems constructing the
 * approximating surface</li>
 * <li>self intersection</li>
 * <li>closure wrong</li>
 * <li>no <tt>bs3_surface</tt></li>
 * <li>control point coincidence></li>
 * <li>not <tt>C1</tt>, <tt>G0</tt>, <tt>G1</tt>, or <tt>G2</tt></li>
 * </ol>
 * <br>
 * <li>Start and end vertices of coedge lie on face</li>
 * <br><br>
 * <li>Edge lies on face. Checks at 10 points along edge</li>
 * <br><br>
 * <li>Start and end vertices lie on edge geometry</li>
 * <br><br>
 * <li>Faces are ordered correctly around edge, according to sidedness</li>
 * <br><br>
 * <li>Coedges are ordered correctly around edge, according to face curvature</li>
 * <br><br>
 * <li>Edge has same sense as curve (taking reverse bit into account)</li>
 * <br><br>
 * <li>Checks curve has correct form:</li>
 * <ol type = "a">
 * <li>straight has unit vector direction and param scale > 0</li>
 * <li>ellipse has</li>
 * <ol>
 * <li>unit vector normal</li>
 * <li>axis with length &gt; 0</li>
 * <li>normal perpendicular to axis</li>
 * <li>radius &gt; 0</li>
 * <li>ratio &lt; 1</li>
 * </ol>
 * <li>intcurve is correctly labeled open, closed, or periodic</li>
 * <li>Edge parameter range is good and agrees with start and end points</li>
 * <li>Check edge for bad approximation direction</li>
 * <li><tt>D3</tt> checks on intcurve (option that can be turned on):</li>
 * <ol type = "a">
 * <li>has <tt>bs3_curve</tt></li>
 * closure is correctly reported
 * <li>continuity is <tt>C0</tt>, <tt>G1</tt>, or <tt>G2</tt></li>
 * <li>no coincident control points</li>
 * <li>curve is not irregular</li>
 * <li>no self intersections</li>
 * <li>no degeneracy</li>
 * <li>no untreatable singularity</li>
 * </ol>
 * <li>No two vertices have the same location</li>
 * <li>Optional face-face intersection checking (option <tt>check_ff_int</tt>):</li>
 * <ol type = "a">
 * <li>Two valid faces have proper intersection:</li>
 * <ol>
 * <li>adjacent faces intersect only along a common edge</li>
 * <li>nonadjacent faces do not intersect</li>
 * </ol>
 * <li>When a face-face Boolean fails, checks for proper edge-edge intersections on each face:</li>
 * <ol>
 * <li>adjacent edges intersection in common vertex only</li>
 * <li>nonadjacent edges do not intersect</li>
 * </ol>
 * <li>Two valid, nonintersecting shells in the same lump have
 *  proper containment; that is, each shell contains the other.
 *  A shell is valid if it contains no improperly
 *  intersecting faces.</li>
 * <li>Two valid, nonintersecting lumps have proper containment,
 *  that is, neither lump contains the other.
 *  A lump is valid when it contains no improperly
 *  intersecting faces and no shells with improper
 *  containment.</li>
 * </ol></ul>
 * <br>
 * When face-face problems are found, a body containing bad intersections is
 * created. This body is named <tt>check_error</tt>.
 * <br><br>
 * <b>Errors:</b> The pointer to an entity is NULL.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
* <b>Journal: </b> Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param given_entity
 * entity to be checked.
 * @param problem_ents
 * list of insane entities and their info.
 * @param file_ptr
 * file pointer.
 * @param ao
 * ACIS options.
 **/
DECL_INTR outcome api_check_entity(
            const ENTITY    *given_entity, // entity to check
			ENTITY_LIST     *problem_ents,  // list of questionable subentities
			FILE            *file_ptr = stdout,
		    AcisOptions     *ao = NULL);


/** @} */
/** \addtogroup ACISCHECKING
 *  \brief Declared at <intrapi.hxx>, SPAintr
 *  @{
 */

/**
 * Evaluates a WIRE for self intersections.
 * <br><br>
 * <b>Role:</b> Given a wire, this API examines the edges of the wire for
 * intersections. Each edge of the wire is examined for self-intersections and for
 * intersections with any of the other edges in the wire. As soon as any
 * intersection other than shared end points is found, the API exits with an error
 * outcome. If no such intersections are found, the API returns a successful
 * outcome.
 * <br><br>
 * This function is overloaded. Given a wire body, this API examines the edges of
 * the underlying wire for intersections.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
* <b>Journal: </b> Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param wire
 * wire to check.
 * @param ao
 * ACIS options.
 **/
DECL_INTR outcome api_check_wire_self_inters(
            WIRE        *wire,  // Wire to check
		    AcisOptions *ao = NULL);

/** @} */
/** \addtogroup ACISCHECKING
 *  \brief Declared at <intrapi.hxx>, SPAintr
 *  @{
 */

/**
 * Evaluates a WIRE for self intersections.
 * <br><br>
 * <b>Role:</b> Given a wire, this API examines the edges of the wire for
 * intersections. Each edge of the wire is examined for self-intersections and for
 * intersections with any of the other edges in the wire. Returned information is as follows:<br>
 *<ul>
 * <li>Case self-intersecting curve: position of self-intersection, self-intersecting edge and parameter of self-intersection.</li>
 * <li>Case two edges intersecting in a point: VERTEX at position of self-intersection, parameter at that position on both edges and Self-intersecting wire</li>
 * <li>Case two edges having a coincident region: EDGE representing coincident region, parameter-range of that region corresponiding to both edges and self-intersecting wire</li>
 *</ul>
 * Behavior of this api is affected by check_abort and checker_limit options. See <i>[Checker Options](https://doc.spatial.com/get_doc_page/articles/c/h/e/Checker_Options_6f13.html)</i> for details.
 * <br>
 * <b>Effect:</b> Read-only
 * <br><br>
* <b>Journal: </b> Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param wire
 * (in) wire to check.
 * @param insanities
 * (out) List of problem entities with information.
 * @param ao
 * (in) ACIS options such as version and journal.
 **/
DECL_INTR outcome api_check_wire_self_intersects(
            WIRE        *wire,  // Wire to check
			insanity_list* &insanities,
		    AcisOptions *ao = NULL);
/**
 * Evaluates a wire BODY for self intersections.
 * <br><br>
 * <b>Role:</b> Given a wire body, this API examines the edges of each wire for
 * intersections. Each edge of each wire is examined for self-intersections and for
 * intersections with any of the other edges in the wire. As soon as any
 * intersection other than shared end points is found, the API exits with an error
 * outcome. If no such intersections are found, the API returns a successful
 * outcome.
 * <br><br>
 * This function is overloaded. Given a wire body, this API examines the edges of
 * each underlying wire for intersections.
 * <br><br>
 * <b>Limitations:</b> This API checks for intersections between edges within each
 * wire of the body. It does not check for intersections between edges of different
 * wires in the body. That is, it will not find wire-wire intersections, only wire
 * self-intersections. 
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
* <b>Journal: </b> Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param body
 * wire body to check.
 * @param ao
 * ACIS options.
 **/
DECL_INTR outcome api_check_wire_self_inters(
			BODY        *body,          // Wire body to check
		    AcisOptions *ao = NULL);


/** @} */
/** \addtogroup ACISCHECKING
 *  \brief Declared at <intrapi.hxx>, SPAintr
 *  @{
 */

// Given an edge, this function returns a linked list of flags which indicate
// if an edge is self-intersecting, scrunched up, or contains other
// degeneracies which would cause problems in ACIS.  Descriptions of the
// flags are in chk.hxx
/**
 * Checks edge geometry for various conditions that could cause errors in other ACIS operations.
 * <br><br>
 * <b>Role:</b> This API checks whether a edge defined by a <tt>bs3_curve</tt> is
 * self-intersecting, is twisted, has too much oscillation, has degenerate segments,
 * or is not <tt>G0</tt>, <tt>G1</tt>, or <tt>G2</tt> continuous. The function <tt>list->status</tt> can be checked for
 * the results. If no errors are found, the <tt>NULL</tt> list is returned. For example:
 * <pre>
 * while(list != NULL){
 * switch(list->status()){
 *  case check_irregular:
 *   sys_error(CURVE_IRREGULAR);
 *   break;
 *  case check_self_intersects:
 *   sys_error(CURVE_SELF_INTER);
 *   break;
 *  case check_bad_closure:
 *   sys_error(CURVE_BAD_CLOSURE);
 *   break;
 *  case check_bs3_null:
 *   sys_error(BS3_CURVE_NULL);
 *   break;
 *  case check_bs3_coi_verts:
 *   sys_error(BS3_COI_VERTS);
 *   break;
 *  case check_bad_degeneracies:
 *   // This case never returned for curves
 *   break;
 *  case check_untreatable_singularity:
 *   // This case never returned for curves
 *   break;
 *  case check_non_G0:
 *   sys_error(CURVE_NON_G0);
 *   break;
 *  case check_non_G1:
 *   sys_error(CURVE_NON_G1);
 *   break;
 *  case check_non_G2:
 *   sys_error(CURVE_NON_G2);
 *   break;
 *  case check_non_C1;
 *   sys_error(CURVE_NON_C1);
 *   break;
 *  case check_inconsistent;
 *   printf("Data mismatch in the given edge");
 *   break;
 *  case check_unknown:
 *   ;
 * }
 * list = list->next();
 * }</pre>
 * <br><br>
 * Note: This signature of <tt>api_check_edge</tt> was deprecated in ACIS R12. 
 * It is recommended that you use the signature of <tt>api_check_edge</tt> that returns an 
 * <tt>insanity_list</tt>. For additional information refer to the ACIS R12 Release Notes.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param edge
 * edge defined by a bs3_curve.
 * @param list
 * linked list of errors in edge returned (NULL if edge is OK).
 * @param ao
 * ACIS options.
 **/
DECL_INTR outcome api_check_edge(
            EDGE                *edge,
            check_status_list   *&list,
            AcisOptions         *ao = NULL);


/**
 * Checks an edge's underlying curve for various conditions that could cause errors in other ACIS operations.
 * <br><br>
 * <b>Role:</b> This API checks the curve underlying the given edge for validitiy. No checking of
 * the edge's coedges or vertices is performed - for full checking of an EDGE entity, use
 * @href api_check_entity .
 * <br><br>
 * This API is expected always to succeed - it should never return an error outcome. A new @href insanity_list
 * instance is created by the API, and a reference to this instance is returned in the <tt>ilist</tt>
 * parameter. If no problems are found with the edge's curve, then the @href insanity_list instance will
 * contain no errors, but it will still be created. Destruction of the returned @href insanity_list is the
 * caller's responsibility.
 * <br><br>
 * In most cases, the insanity ids returned by <tt>api_check_edge</tt> have the same meaning as when they are returned
 * by @href api_check_entity . In the case of continuity problems (non-G1, non-G2 etc.), however, the meanings
 * are subtly different, because <tt>api_check_edge</tt> checks an edge's curve in isolation from the edge itself. It
 * may be that the curve underlying the edge is (say) non-G1, but all of the discontinuities lie outside of the
 * range of the edge. In this case, the edge is G1 continuous, and @href api_check_entity <b>will not</b>
 * report a non-G1 warning, but the edge's curve is not G1 continuous, so <tt>api_check_edge</tt> <b>will</b> report a
 * non-G1 warning.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param edge
 * The edge for which the curve geometry is to be checked.
 * @param ilist
 * Output: list of problems with edge's curve.
 * @param ao
 * ACIS options.
 * @see insanity_list
 **/
DECL_INTR outcome api_check_edge(
            EDGE            *edge, 
            insanity_list   *&ilist,
            AcisOptions     *ao = NULL);


// Given a face , this function returns a linked list of flags which indicate
// if a face is self-intersecting, scrunched up, or contains other
// degeneracies which would cause problems in ACIS.  Descriptions of the
// flags are in chk.hxx

/**
 * Checks face geometry for various conditions that could cause errors in other ACIS operations.
 * <br><br>
 * <b>Role:</b> This API checks whether a face defined by a <tt>bs3_surface</tt> is
 * self-intersecting, is twisted, has too much oscillation, has degenerate patches,
 * or is not <tt>G0</tt>, <tt>G1</tt>, or <tt>G2</tt>. The function <tt>list->status</tt> can be checked for the
 * results. If no errors are found, the <tt>NULL</tt> list is returned. For example:
 * <pre>
 * while(list != NULL){
 * switch(list->status()){
 *  case check_irregular:
 *   sys_error(FACE_IRREGULAR);
 *   break;
 *  case check_self_intersects:
 *   sys_error(FACE_SELF_INTER);
 *   break;
 *  case check_bad_closure:
 *   sys_error(SURF_BAD_CLOSURE);
 *   break;
 *  case check_bs3_null:
 *   sys_error(BS3_SURF_NULL);
 *   break;
 *  case check_bs3_coi_verts:
 *   sys_error(BS3_COI_VERTS);
 *   break;
 *  case check_bad_degeneracies:
 *   sys_error(BAD_DEGENERACIES);
 *   break;
 *  case check_untreatable_singularity:
 *   sys_error(UNTREAT_SING);
 *   break;
 *  case check_non_G0:
 *   sys_error(SURF_NON_G0);
 *   break;
 *  case check_non_G1:
 *   sys_error(SURF_NON_G1);
 *   break;
 *  case check_non_G2:
 *   sys_error(SURF_NON_G2);
 *   break;
 *  case check_non_C1:
 *   sys_error(SURF_NON_C1);
 *   break;
 *  case check_unknown:
 *   ;
 * }
 * list = list->next();
 * }</pre>
 * <br><br>
 * Note: This signature of <tt>api_check_face</tt> was deprecated in ACIS R12. 
 * It is recommended that you use the signature of <tt>api_check_face</tt> that returns an 
 * <tt>insanity_list</tt>. For additional information refer to the ACIS R12 Release Notes.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param face
 * face defined by a bs3_surface.
 * @param list
 * linked list of errors in face returned.
 * @param ao
 * ACIS options.
 **/
DECL_INTR outcome api_check_face(
            FACE                *face,
            check_status_list   *&list,
            AcisOptions         *ao = NULL);

/**
 * Checks a face's underlying surface for various conditions that could cause errors in other ACIS operations.
 * <br><br>
 * <b>Role:</b> This API checks the surface underlying the given face for validity. No checking of the face's
 * edges, coedges or loops is performed - for full checking of a FACE entity, use @href api_check_entity .
 * <br><br>
 * This API is expected always to succeed - it should never return an error outcome. A new @href insanity_list
 * instance is created by the API, and a reference to this instance is returned in the <tt>ilist</tt>
 * parameter. If no problems are found with the face's surface, then the @href insanity_list instance will
 * contain no errors, but it will still be created. Destruction of the returned @href insanity_list is the
 * caller's responsibility.
 * <br><br>
 * In most cases, the insanity ids returned by api_check_face have the same meaning as when they are returned
 * by @href api_check_entity . In the case of continuity problems (non-G1, non-G2 etc.) however, the meanings
 * are subtly different, because api_check_face checks a face's surface in isolation from the face itself. It
 * may be that the surface underlying the face is (say) non-G1, but all of the discontinuities lie outside the
 * face's boundary. In this case, the face is G1 continuous, and @href api_check_entity <b>will not</b> report
 * a non-G1 warning, but the face's surface is not G1 continuous, so api_check_face <b>will</b> report a non-G1
 * warning.
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param face
 * The face for which the surface geometry is to be checked.
 * @param ilist
 * Output: list of problems with face's surface.
 * @param ao
 * ACIS options.
 * <br>
 * @see insanity_list
 **/
DECL_INTR outcome api_check_face(
            FACE            *face,
            insanity_list   *&ilist,
            AcisOptions     *ao = NULL);


//  For identifying type of information returned
//  with following param_info class.

/*
// tbrv
*/

/**
 * @nodoc
 */

enum 	param_info_type
{
	ent_is_unknown=-1,
    ent_is_face,
    ent_is_edge,
    ent_is_vertex,
	ent_is_body,	// returned if inside body
	ent_is_lump		// returned if inside lump
};


/** @} */
/** \addtogroup INTRMAIN
 *  \brief Declared at <intrapi.hxx>, SPAintr
 *  @{
 */

// Class added for returning information about the positions
// of the entity distance APIs.
/**
 * Data representing type and, in some cases, parameter information about a point on an ENTITY.
 * <br>
 * <b>Role:</b> This class holds type and in some cases parameter data about point
 * on an entity. It is used by some query apis to return detailed information
 * about points they have identified on entities e.g. a point of minimum distance
 * to another entity.
 * <br><br>
 * <b>Limitations:</b> This is a data storage class only. It does not make any
 * attempt to maintain consistent data. It is the responsibility of the users
 * of this class to ensure that instances are valid.
 * @see SPApar_pos, SPAparameter, ENTITY
 */
class DECL_INTR param_info : public ACIS_OBJECT {

private:

	param_info_type		type;			// Face, Edge, or Vertex
	ENTITY*				entity_ptr;		// Entity SPAposition is on
	SPApar_pos		    uv_param;		// Face SPAparameter values
	SPAparameter	    t_param;		// Edge SPAparameter value

public:

	// Constructors

	// Default constructor
/*
// tbrv
*/
/**
 * @nodoc
 */
	param_info() {
			type = ent_is_vertex;
			entity_ptr = NULL;
			uv_param = SPApar_pos(0.0,0.0);
			t_param = 0.0;
		}

	// Inquiry functions.
/**
 * Returns the type of the entity referred to by this instance.
 */
	param_info_type	entity_type() const { return type; }
/**
 * Returns a pointer to the entity referred to by this instance.
 */
	ENTITY	*entity() const { return entity_ptr; }
/**
 * Returns the (u,v) parameters of the point represented by this instance.
 * <br><br>
 * <b>Role:</b> This will be meaningful only if the entity referred to is a face.
 */
	SPApar_pos uv() const { return uv_param; }
/**
 * Returns the curve parameter of the point represented by this instance.
 * <br><br>
 * <b>Role:</b> This will  be meaningful only if the entity referred to is an edge.
 */
	SPAparameter t() const { return t_param; }

/**
 * Sets the entity type for this instance.
 * <br><br>
 * @param ent_type
 * type of entity.
 */
	void set_entity_type( param_info_type ent_type ) { type = ent_type ; }
/**
 * Sets the entity referred to by this instance.
 * <br><br>
 * @param ent
 * entity.
 */
	void set_entity( ENTITY *ent ) { entity_ptr = ent; }
/**
 * Sets the (u,v) parameter position for this instance.
 * <br><br>
 * @param uv
 * position coordinate.
 */
	void set_uv(const SPApar_pos& uv ) { uv_param = uv; }
/**
 * Sets the curve parameter value for this instance.
 * <br><br>
 * @param t
 * parameter value.
 */
	void set_t(const SPAparameter& t ) { t_param = t; }

	// Assignment operator
/**
 * Assignment operator.
 * <br><br>
 * @param rhs
 * parameter information.
 */
	param_info const &operator=( param_info const & rhs)
	{
		type = rhs.type;
		entity_ptr = rhs.entity_ptr;
		uv_param = rhs.uv_param;
		t_param = rhs.t_param;
		return *this;
	}

};

class param_info_array;
class DECL_INTR param_info_vector: public ACIS_OBJECT
{
public:
	/**
	*@nodoc
	**/
	param_info_vector();

	/**
	*@nodoc
	**/
	~param_info_vector();

	/**
	*@nodoc
	**/
	int size() const;

	/**
	*@nodoc
	**/
	param_info const& operator[](int ii) const;

	/**
	*@nodoc
	**/
	param_info& operator[](int ii);

	/**
	*@nodoc
	**/
	void push_back( param_info const& pos_to_add );

	/**
	*@nodoc
	**/
	// changes allocated block size and vector size.
	void resize( int new_size );

	/**
	* erases all contents of vector.
	**/
	void clear();

	/**
	* copy contents into other vector
	**/
	void copy_to( param_info_vector& out_copy ) const;

	/**
	* @nodoc
	**/
	operator param_info const* const() const;

	/**
	* @nodoc
	**/
	operator param_info* const();

private:
	param_info_array* m_array;

	param_info_vector( param_info_vector const& );
	param_info_vector& operator=( param_info_vector const& );
};


/** @} */
/** \addtogroup INTRAPIEXTREMA
 *  \brief Declared at <intrapi.hxx>, SPAintr
 *  @{
 */

/**
 * Computes the position of the extremum of the entity in the given direction.
 * <br><br>
 * <b>Role:</b> You may specify between one and three extrema directions. The API 
 * computes the extrema of the entity in the first given direction. If there is 
 * more than one extremum, then the API attempts to obtain a unique extremum by 
 * determining the extrema of the first direction that are also extreme in the 
 * second direction (if given). If there is still more than one extremum, then the 
 * API selects the extremum that is also extreme in the third direction (if given). 
 * Optionally, the API will return the entity containing the extremum and the 
 * corresponding parameter value of the extremum position.
 * <br><br>
* <b>Effect:</b> Read-only
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param ent
 * entity on which to find extrema.
 * @param nvec
 * number of directions.
 * @param in_vec
 * directions.
 * @param on_pos
 * extrema position.
 * @param info
 * parameter info.
 * @param ao
 * ACIS options.
 **/

DECL_INTR outcome api_entity_extrema(
            ENTITY      *ent,
            int         nvec,
            SPAvector   *in_vec,
            SPAposition &on_pos,
            param_info  &info = *(param_info *) NULL_REF,
            AcisOptions *ao = NULL);

/**
 * Computes the position of the extremum of the entities in the given direction.
 * <br><br>
 * <b>Role:</b> You may specify between one and three extrema directions. The API 
 * computes the extrema of the entity in the first given direction. If there is 
 * more than one extremum, then the API attempts to obtain a unique extremum by 
 * determining the extrema of the first direction that are also extreme in the 
 * second direction (if given). If there is still more than one extremum, then the 
 * API selects the extremum that is also extreme in the third direction (if given). 
 * Optionally, the API will return the entity containing the extremum and the 
 * corresponding parameter value of the extremum position.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
* <b>Journal: </b> Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param ents
 * list of entities on which to find extrema.
 * @param nvec
 * number of directions.
 * @param in_vec
 * directions.
 * @param max_pos
 * extrema position.
 * @param out_info
 * parameter info.
 * @param ao
 * ACIS options.
 **/
DECL_INTR outcome api_entity_extrema(
            ENTITY_LIST &ents,
            int         nvec,
            SPAvector   *in_vec,
            SPAposition &max_pos,
            param_info  &out_info = *(param_info *) NULL_REF,
            AcisOptions *ao = NULL);


/** @} */
/** \addtogroup INTRAPIBOUNDARYFIELD
 *  \brief Declared at <intrapi.hxx>, SPAintr
 *  @{
 */

/**
 * Generates a vector field around the coedges of a face.
 * <br><br>
 * <b>Role:</b> This API generates vector fields on boundary coedges. These
 * vector fields are represented in the form of ACIS laws and can be controlled by
 * setting a target vector field type. The supported vector field types include:
 * <br><br>
 * <ul>
 * <li><tt>UNIFORM_VEC_FIELD</tt><br>
 * Generate a constant vector fields for all coedges. Applications may supply a
 * uniform vector. However, if no vector is supplied, an average wire normal vector
 * or an average face normal vector will be used to define the uniform vector.</li>
 * <br><br>
 * <li><tt>FACE_NORMAL_FIELD</tt><br>
 * Generated from the face normal that the coedge is on.</li>
 * <br><br>
 * <li><tt>FACE_TANGENT_FIELD</tt><br>
 * Generated from the cross product of coedge tangent of face normal.</li>
 * <br><br>
 * <li><tt>FACE_SIDE_FIELD</tt><br>
 * If a base face has adjacent faces, the vector fields is generated from the cross
 * product of the coedge tangent and the adjacent face normal.</li>
 * <br><br>
 * <li><tt>DRAFT_NORMAL_FIELD</tt><br>
 * Generate vector fields that form a constant draft angle to the vector fields
 * defined from either the <tt>UNIFORM_VEC_FIELD</tt> or the <tt>FACE_NORMAL_FIELD</tt> options.</li>
 * <br><br>
 * <li><tt>DRAFT_SIDE_FIELD</tt><br>
 * Generate vector fields that form a constant draft angle to the vector fields
 * generated from the <tt>FACE_SIDE_FIELD</tt> option.</li>
 * </ul>
 * <br>
 * The vector fields on <tt>G1</tt> discontinuous coedges are adjusted so the fields will be
 * continuous across vertices, i.e., no gaps between adjacent coedge vector fields.
 * This capability allows applications to generate valid vector fields for
 * subsequent operations such as lofting and sweeping.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
* <b>Journal: </b> Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param in_fas
 * create vector fields on coedges of this base face.
 * @param ftype
 * type of vector fields to create.
 * @param rev
 * reverse flag for face.
 * @param draft
 * draft angle from face, may be zero.
 * @param uniform_vec
 * reference vector to use (may be NULL_REF).
 * @param cons_eds
 * constraint edges to setup coedge field.
 * @param global
 * output vector fields in the global space.
 * @param out_law
 * output laws for vector fields.
 * @param ao
 * ACIS options.
 **/
DECL_INTR outcome api_create_boundary_field(
            FACE	    *in_fas,
            fieldtype   ftype,
            int		    rev,
            double      draft,
            SPAvector   &uniform_vec,
            ENTITY_LIST &cons_eds,
            int		    global,
            law         **&out_law,
            AcisOptions *ao = NULL);

/**
 * Generates a vector field around the coedges of a wire.
 * <br><br>
 * <b>Role:</b> This API generates vector fields on boundary coedges. These
 * vector fields are represented in the form of ACIS laws and can be controlled by
 * setting a target vector field type. The supported vector field types include:
 * <br><br>
 * <ul>
 * <li><tt>UNIFORM_VEC_FIELD</tt><br>
 * Generate a constant vector fields for all coedges. Applications may supply a
 * uniform vector. However, if no vector is supplied, an average wire normal vector
 * or an average face normal vector will be used to define the uniform vector.</li>
 * <br><br>
 * <li><tt>FACE_NORMAL_FIELD</tt><br>
 * Generated from the face normal that the coedge is on.</li>
 * <br><br>
 * <li><tt>FACE_TANGENT_FIELD</tt><br>
 * Generated from the cross product of coedge tangent of face normal.</li>
 * <br><br>
 * <li><tt>FACE_SIDE_FIELD</tt><br>
 * If a base face has adjacent faces, the vector fields is generated from the cross
 * product of the coedge tangent and the adjacent face normal.</li>
 * <br><br>
 * <li><tt>DRAFT_NORMAL_FIELD</tt><br>
 * Generate vector fields that form a constant draft angle to the vector fields
 * defined from either the <tt>UNIFORM_VEC_FIELD</tt> or the <tt>FACE_NORMAL_FIELD</tt> options.</li>
 * <br><br>
 * <li><tt>DRAFT_SIDE_FIELD</tt><br>
 * Generate vector fields that form a constant draft angle to the vector fields
 * generated from the <tt>FACE_SIDE_FIELD</tt> option.</li>
 * </ul>
 * <br>
 * The vector fields on <tt>G1</tt> discontinuous coedges are adjusted so the fields will be
 * continuous across vertices, i.e., no gaps between adjacent coedge vector fields.
 * This capability allows applications to generate valid vector fields for
 * subsequent operations such as lofting and sweeping.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
* <b>Journal: </b> Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param in_wire
 * the base wire on which to find coedges.
 * @param ftype
 * the type of vector fields to create.
 * @param rev
 * reverse flag for face.
 * @param draft
 * draft angle from the face (may be 0).
 * @param uniform_vec
 * reference vector to use (may be NULL_REF).
 * @param cons_eds
 * constraint edges to setup coedge field.
 * @param global
 * output vector fields in the global space.
 * @param out_law
 * output laws for vector fields.
 * @param ao
 * ACIS options.
 **/
DECL_INTR outcome api_create_boundary_field(
            WIRE	    *in_wire,
            fieldtype   ftype,
            int		    rev,
            double      draft,
            SPAvector   &uniform_vec,
            ENTITY_LIST &cons_eds,
            int		    global,
            law         **&out_law,
            AcisOptions *ao = NULL);


/**
 * Generates a vector field around the coedges of a wire or face.
 * <br><br>
 * <b>Role:</b> This API generates vector fields on boundary coedges. These
 * vector fields are represented in the form of ACIS laws and can be controlled by
 * setting a target vector field type. The supported vector field types include:
 * <br><br>
 * <ul>
 * <li><tt>UNIFORM_VEC_FIELD</tt><br>
 * Generate a constant vector fields for all coedges. Applications may supply a
 * uniform vector. However, if no vector is supplied, an average wire normal vector
 * or an average face normal vector will be used to define the uniform vector.</li>
 * <br><br>
 * <li><tt>FACE_NORMAL_FIELD</tt><br>
 * Generated from the face normal that the coedge is on.</li>
 * <br><br>
 * <li><tt>FACE_TANGENT_FIELD</tt><br>
 * Generated from the cross product of coedge tangent of face normal.</li>
 * <br><br>
 * <li><tt>FACE_SIDE_FIELD</tt><br>
 * If a base face has adjacent faces, the vector fields is generated from the cross
 * product of the coedge tangent and the adjacent face normal.</li>
 * <br><br>
 * <li><tt>DRAFT_NORMAL_FIELD</tt><br>
 * Generate vector fields that form a constant draft angle to the vector fields
 * defined from either the <tt>UNIFORM_VEC_FIELD</tt> or the <tt>FACE_NORMAL_FIELD</tt> options.</li>
 * <br><br>
 * <li><tt>DRAFT_SIDE_FIELD</tt><br>
 * Generate vector fields that form a constant draft angle to the vector fields
 * generated from the <tt>FACE_SIDE_FIELD</tt> option.</li>
 * </ul>
 * <br>
 * The vector fields on <tt>G1</tt> discontinuous coedges are adjusted so the fields will be
 * continuous across vertices, i.e., no gaps between adjacent coedge vector fields.
 * This capability allows applications to generate valid vector fields for
 * subsequent operations such as lofting and sweeping.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
* <b>Journal: </b> Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param coeds
 * a list of coedges.
 * @param ref_faces
 * reference faces for each edge, may be <tt>NULL</tt>.
 * @param ftype
 * the type of vector fields to create.
 * @param rev
 * reverse flag for face.
 * @param draft
 * draft angle from face, may be zero.
 * @param uniform_vec
 * reference vector to use (may be NULL_REF).
 * @param cons_eds
 * constraint edges to setup coedge field.
 * @param global
 * output vector fields in the global space.
 * @param out_law
 * output laws for vector fields.
 * @param ao
 * ACIS options.
 **/
DECL_INTR outcome api_create_boundary_field(
            ENTITY_LIST &coeds,
            FACE        **ref_faces,
            fieldtype   ftype,
            int		    rev,
            double      draft,
            SPAvector   &uniform_vec,
            ENTITY_LIST &cons_eds,
            int		    global,
            law         **&out_law,
            AcisOptions *ao = NULL);
/**
 * Generates a vector field around the coedges of a wire or face.
 * <br><br>
 * <b>Role:</b> This API generates vector fields on boundary coedges. These
 * vector fields are represented in the form of ACIS laws and can be controlled by
 * setting a target vector field type. The supported vector field types include:
 * <br><br>
 * <ul>
 * <li><tt>UNIFORM_VEC_FIELD</tt><br>
 * Generate a constant vector fields for all coedges. Applications may supply a
 * uniform vector. However, if no vector is supplied, an average wire normal vector
 * or an average face normal vector will be used to define the uniform vector.</li>
 * <br><br>
 * <li><tt>FACE_NORMAL_FIELD</tt><br>
 * Generated from the face normal that the coedge is on.</li>
 * <br><br>
 * <li><tt>FACE_TANGENT_FIELD</tt><br>
 * Generated from the cross product of coedge tangent of face normal.</li>
 * <br><br>
 * <li><tt>FACE_SIDE_FIELD</tt><br>
 * If a base face has adjacent faces, the vector fields is generated from the cross
 * product of the coedge tangent and the adjacent face normal.</li>
 * <br><br>
 * <li><tt>DRAFT_NORMAL_FIELD</tt><br>
 * Generate vector fields that form a constant draft angle to the vector fields
 * defined from either the <tt>UNIFORM_VEC_FIELD</tt> or the <tt>FACE_NORMAL_FIELD</tt> options.</li>
 * <br><br>
 * <li><tt>DRAFT_SIDE_FIELD</tt><br>
 * Generate vector fields that form a constant draft angle to the vector fields
 * generated from the <tt>FACE_SIDE_FIELD</tt> option.</li>
 * </ul>
 * <br>
 * The vector fields on <tt>G1</tt> discontinuous coedges are adjusted so the fields will be
 * continuous across vertices, i.e., no gaps between adjacent coedge vector fields.
 * This capability allows applications to generate valid vector fields for
 * subsequent operations such as lofting and sweeping.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
* <b>Journal: </b> Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param coeds
 * a list of coedges.
 * @param field_laws
 * reference laws to define basic fields, laws are in coedge local space.
 * @param draft
 * draft angle from face, may be zero.
 * @param cons_eds
 * constraint edges to setup coedge field.
 * @param global
 * output vector fields in the global space.
 * @param out_law
 * output laws for vector fields.
 * @param ao
 * ACIS options.
 **/
DECL_INTR outcome api_create_boundary_field(
            ENTITY_LIST &coeds,
            law         **field_laws,
            double      draft,
            ENTITY_LIST &cons_eds,
            int		    global,
            law         **& out_law,
            AcisOptions *ao = NULL);


/** @} */
/** \addtogroup INTRAPIEDGETANG
 *  \brief Declared at <intrapi.hxx>, SPAintr
 *  @{
 */

/**
 * Test an edge for tangency conditions.<br>
 * <b>Role:</b> Inquiries the EDGE to determine if it is tangent within
 * the given tolerance.<br>
 * <dl>
 * <b>Errors:</b> Pointer to <tt>EDGE</tt> is <tt>NULL</tt> or not to an <tt>EDGE</tt>.
 * <dd>&nbsp;Pointer to entity is <tt>NULL</tt>.</dd>
 * </dl>
 * <br><br>
 * <b>Effect:</b> Read-only 
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param edge
 * EDGE to test.
 * @param is_tangent
 * Returns TRUE if EDGE is tangent.
 * @param tolerance
 * Angle tolerance in radians
 * @param acis_options
 * AcisOptions
 */
DECL_INTR outcome api_edge_tangent(
			EDGE         *edge,
			logical      &is_tangent,
			double       tolerance     = SPAresnor,
			AcisOptions  *acis_options = NULL);


/** @} */
/** \addtogroup ACISBOXING
 *  \brief Declared at <intrapi.hxx>, SPAintr
 *  @{
 */

/**
 *  
 * Computes a bounding box containing the entities. 
 * <b>Technical Article:</b> <i>[Entity Boxing](http://doc.spatial.com/articles/e/n/t/Entity_Boxing_22f9.html)</i>
 * <br><br>
 * <b>Role:</b>
 * This API computes a bounding box for an <tt>ENTITY_LIST</tt>. If the <tt>SPAboxing_options</tt> argument  
 * specifies a <tt>WCS</tt>, then the computation of the bounding box is relative to that <tt>WCS</tt>. The
 * tightness of the box is specified by setting the <tt>SPAboxing_mode</tt> in the <tt>SPAboxing_options</tt>.
 * <br><br>
 * The positions returned, <tt>min_pt</tt> and <tt>max_pt</tt>, are the 
 * corners of the bounding box. These positions are always relative to the model space 
 * specified. Care must be taken during the interpretation of the results. 
 * <br><br>
 * By default this API uses the universal <tt>WCS</tt> for the orientation and location 
 * of the bounding box. If the user wants further control of the orientation, location or 
 * type of bounding box, then a pointer to a <tt>SPAboxing_options</tt> object must be provided. 
 * This object takes a <tt>WCS</tt> and a predefined enumerated type for the boxing mode 
 * (see @href SPAboxing_mode for further details).
 * <br><br>
 * Consider the following code example:
 * <pre>
 *   BODY*       block_0 = NULL;
 *   SPAposition pt_0(0,0,0);
 *   SPAposition pt_1(1,2,3);
 *
 *   outcome result = api_solid_block(pt_0, pt_1, block_0);
 *
 *   BODY*       block_1 = NULL;
 *   SPAposition pt_2(4,4,4);
 *   SPAposition pt_3(5,6,7);
 *
 *   result = api_solid_block(pt_2, pt_3, block_1);
 *
 *   ENTITY_LIST elist;
 *   elist.add( block_0 );
 *   elist.add( block_1 );
 *
 *   WCS*        wcs1 = NULL;
 *   SPAposition xpt(-1,0,0);
 *   SPAposition ypt(0,-1,0);
 *
 *   result = api_wcs_create(pt_0, xpt, ypt, wcs1);
 *      
 *   SPAposition        min_pt(0,0,0), max_pt(0,0,0);
 *   SPAboxing_options* box_opts = ACIS_NEW SPAboxing_options(wcs1, SPAboxing_tight);
 *
 *   result = api_get_entity_box(elist, min_pt, max_pt, box_opts);
 *
 *   ACIS_DELETE box_opts;
 * </pre>
 * 
 * This code example creates two blocks. One with corners at (0, 0, 0) and (1, 2, 3), 
 * and the other with corners at (4, 4, 4) and (5, 6, 7). Both blocks are added to an entity 
 * list to ensure that an overall bounding box is calculated.
 * <br><br>
 * Additionally, in order to have further control of the calculated bounding box, this code 
 * creates a pointer to a <tt>SPAboxing_options</tt> object. This object takes in a <tt>WCS</tt> 
 * (<tt>wcs1</tt>) that is model-space rotated about the <i>z</i>-axis by 180 degrees, and a 
 * <tt>SPAboxing_tight</tt> boxing mode. This pointer is subsequently passed in to the API.
 * <br><br>
 * The corners of the newly calculated bounding box are at (0, 0, 0) and (-5, -6, 7), which 
 * are relative to the specified <tt>wcs1</tt>. Furthermore, the extrema points are also 
 * relative to <tt>wcs1</tt>, and they are (-5, -6, 0) and (0, 0, 7). 
 * When these extrema points are mapped back to model space, they are (5, 6, 0) and (0, 0, 7).
 * <br><br>
 * Because the API computes the extrema relative to <tt>wcs1</tt> and returns the results
 * mapped back into model space, the returned positions are:
 *
 * <pre>
 *      min_pt = (5,6,0)
 *      max_pt = (0,0,7)
 * </pre>
 *
 * <b>Note:</b> This API is guaranteed to return the tightest bounding box on toroidal faces
 * by default. In order to obtain the tightest bounding box for spherical faces, this must be 
 * explicitly declared by setting the boxing mode to <tt>SPAboxing_tight</tt> in the 
 * <tt>SPAboxing_options</tt> argument (see @href get_face_box for further details). 
 * <br><br>
 * The <tt>SPAboxing_options</tt> object takes one of the several boxing modes available: 
 * <ul> 
 *      <li><tt>SPAboxing_tight</tt></li> 
 *      <li><tt>SPAboxing_loose</tt></li> 
 *      <li><tt>SPAboxing_quick</tt></li>
 *      <li><tt>SPAboxing_default_pre_R12</tt></li> 
 *      <li><tt>SPAboxing_default</tt></li>
 *      <li><tt>SPAboxing_exact</tt></li>
 * </ul>
 * <br>
 * For more information on this topic see @href SPAboxing_options and @href SPAboxing_mode.
 * 
 * <br><br>
 * <b>Effect:</b> Changes model's derived data
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
* @param ent_list
 * list of entities.
 * @param min_pt
 * minimum position of bounding box returned.
 * @param max_pt
 * maximum position of bounding box returned.
 * @param box_options
 * A pointer to a <tt>SPAboxing_options</tt> object. It requires a WCS and boxing mode.
 * @param ao
 * acis options.
 * @see SPAboxing_options, SPAboxing_mode
 **/
DECL_INTR outcome api_get_entity_box(
            const ENTITY_LIST   &ent_list,
            SPAposition         &min_pt,
            SPAposition         &max_pt,
            SPAboxing_options   *box_options = NULL,
            AcisOptions         *ao          = NULL);

/**
 * Computes a bounding box containing the entity.
 * <b>Technical Article:</b> <i>[Entity Boxing](http://doc.spatial.com/articles/e/n/t/Entity_Boxing_22f9.html)</i>
 * <br><br>
 * <b>Role:</b>
 * This API computes a bounding box of an <tt>ENTITY</tt>. If the <tt>SPAboxing_options</tt> argument 
 * specifies a <tt>WCS</tt>, then the computation of the bounding box is relative to that <tt>WCS</tt>. The
 * tightness of the box is specified by setting the <tt>SPAboxing_mode</tt> in the <tt>SPAboxing_options</tt>.
 * <br><br>
 * The positions returned, <tt>min_pt</tt> and <tt>max_pt</tt>, are the 
 * corners of the bounding box. These positions are always relative to the model space 
 * specified. Care must be taken during the interpretation of the results. 
 * <br><br>
 * By default this API uses the universal <tt>WCS</tt> for the orientation and location 
 * of the bounding box. If the user wants further control of the orientation, location or 
 * type of bounding box, then a pointer to a <tt>SPAboxing_options</tt> object must be provided. 
 * This object takes a <tt>WCS</tt> and a predefined enumerated type for the boxing mode 
 * (see @href SPAboxing_mode for further details).
 * <br><br>
 * Consider the following code example:
 * <pre>
 *   BODY*       block = NULL;
 *   SPAposition pt_0(0,0,0);
 *   SPAposition pt_1(1,2,3);
 *
 *   outcome result = api_solid_block(pt_0, pt_1, block);
 *
 *   WCS*        wcs1 = NULL;
 *   SPAposition xpt(-1,0,0);
 *   SPAposition ypt(0,-1,0);
 *
 *   result = api_wcs_create(pt_0, xpt, ypt, wcs1);
 *      
 *   SPAposition        min_pt(0,0,0), max_pt(0,0,0);
 *   SPAboxing_options* box_opts = ACIS_NEW SPAboxing_options(wcs1, SPAboxing_tight);
 *
 *   result = api_get_entity_box(block, min_pt, max_pt, box_opts);
 *
 *   ACIS_DELETE box_opts;
 * </pre>
 * 
 * This code example creates a block with corners at (0, 0, 0) and (1, 2, 3).
 * <br><br>
 * Additionally, in order to have further control of the calculated bounding box, this code 
 * creates a pointer to a <tt>SPAboxing_options</tt> object. This object takes in a <tt>WCS</tt> 
 * (<tt>wcs1</tt>) that is model-space rotated about the <i>z</i>-axis by 180 degrees, and a 
 * <tt>SPAboxing_tight</tt> boxing mode. This pointer is subsequently passed in to the API.
 * <br><br>
 * The corners of the newly calculated bounding box are at (0, 0, 0) and (-1, -2, 3), which 
 * are relative to the specified <tt>wcs1</tt>. Furthermore, the extrema points are also 
 * relative to <tt>wcs1</tt>, and they are (-1, -2, 0) and (0, 0, 3). 
 * When these extrema points are mapped back to model space, they are (1, 2, 0) and (0, 0, 3).
 * <br><br>
 * Because the API computes the extrema relative to <tt>wcs1</tt> and returns the results
 * mapped back into model space, the returned positions are:
 *
 * <pre>
 *      min_pt = (1,2,0)
 *      max_pt = (0,0,3)
 * </pre>
 *
 * <b>Note:</b> This API is guaranteed to return the tightest bounding box on toroidal faces
 * by default. In order to obtain the tightest bounding box for spherical faces, this must be 
 * explicitly declared by setting the boxing mode to <tt>SPAboxing_tight</tt> in the 
 * <tt>SPAboxing_options</tt> argument (see @href get_face_box for further details). 
 * <br><br>
 * The <tt>SPAboxing_options</tt> object takes one of the several boxing modes available: 
 * <ul> 
 *      <li><tt>SPAboxing_tight</tt></li> 
 *      <li><tt>SPAboxing_loose</tt></li> 
 *      <li><tt>SPAboxing_quick</tt></li>
 *      <li><tt>SPAboxing_default_pre_R12</tt></li> 
 *      <li><tt>SPAboxing_default</tt></li>
 *      <li><tt>SPAboxing_exact</tt></li>
 * </ul>
 * <br>
 * For more information on this topic see @href SPAboxing_options and @href SPAboxing_mode.
 * 
 * <br><br>
 * <b>Effect:</b> Changes model's derived data
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
* @param entity
 * entity of interest.
 * @param min_pt
 * minimum position of bounding box returned.
 * @param max_pt
 * maximum position of bounding box returned.
 * @param box_options
 * A pointer to a <tt>SPAboxing_options</tt> object. It requires a WCS and boxing mode.
 * @param ao
 * acis options.
 * @see SPAboxing_options, SPAboxing_mode
 **/

DECL_INTR outcome api_get_entity_box(
            ENTITY              *entity,
            SPAposition         &min_pt,
            SPAposition         &max_pt,
            SPAboxing_options   *box_options = NULL,
            AcisOptions         *ao          = NULL );


/**
 * Computes a bounding box containing the entities.
 * <b>Technical Article:</b> <i>[Entity Boxing](http://doc.spatial.com/articles/e/n/t/Entity_Boxing_22f9.html)</i>
 * <br><br>
 * <b>Role:</b>
 * This API computes a bounding box of an <tt>ENTITY_LIST</tt>. If a <tt>WCS</tt> is specified, 
 * then the computation of the bounding box is relative to that <tt>WCS</tt>.
 * <br><br>
 * The positions returned, <tt>min_pt</tt> and <tt>max_pt</tt>, are the 
 * corners of the bounding box. These positions are always relative to the model space 
 * specified. Care must be taken during the interpretation of the results. 
 * <br><br>
 * Consider the following code example:
 * <pre>
 *   BODY*       block_0 = NULL;
 *   SPAposition pt_0(0,0,0);
 *   SPAposition pt_1(1,2,3);
 *
 *   outcome result = api_solid_block(pt_0, pt_1, block_0);
 *
 *   BODY*       block_1 = NULL;
 *   SPAposition pt_2(4,4,4);
 *   SPAposition pt_3(5,6,7);
 *
 *   result = api_solid_block(pt_2, pt_3, block_1);
 *
 *   ENTITY_LIST elist;
 *   elist.add( block_0 );
 *   elist.add( block_1 );
 *
 *   WCS*        wcs1 = NULL;
 *   SPAposition xpt(-1,0,0);
 *   SPAposition ypt(0,-1,0);
 *
 *   result = api_wcs_create(pt_0, xpt, ypt, wcs1);
 *
 *   SPAposition min_pt(0,0,0), max_pt(0,0,0);
 *   result = api_get_entity_box(elist, wcs1, min_pt, max_pt);
 * </pre>
 * 
 * This code example creates two blocks. One with corners at (0, 0, 0) and (1, 2, 3), 
 * and the other with corners at (4, 4, 4) and (5, 6, 7). Both blocks are added to an entity 
 * list to ensure that an overall bounding box is calculated.
 * <br><br>
 * Additionally, it creates a <tt>WCS</tt> (<tt>wcs1</tt>) that is model-space rotated about the 
 * <i>z</i>-axis by 180 degrees.
 * <br><br>
 * The corners of the newly calculated bounding box are at (0, 0, 0) and (-5, -6, 7), which 
 * are relative to the specified <tt>wcs1</tt>. Furthermore, the extrema points are also 
 * relative to <tt>wcs1</tt>, and they are (-5, -6, 0) and (0, 0, 7). 
 * When these extrema points are mapped back to model space, they are (5, 6, 0) and (0, 0, 7).
 * <br><br>
 * Because the API computes the extrema relative to <tt>wcs1</tt> and returns the results
 * mapped back into model space, the returned positions are:
 *
 * <pre>
 *      min_pt = (5,6,0)
 *      max_pt = (0,0,7)
 * </pre>
 *
 * <b>Note:</b> This API is not guaranteed to return the tightest bounding box on toroidal
 * and spherical faces. A tighter face box may be explicitly requested by setting the flag 
 * <tt>tight_box</tt> to <tt>TRUE</tt>.
 * <br><br>
 * For more information on this topic see @href get_face_box.
 * 
 * <br><br>
 * <b>Effect:</b> Changes model's derived data
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param ent_list
 * list of entities.
 * @param min_pt
 * minimum position of bounding box returned.
 * @param max_pt
 * maximum position of bounding box returned.
 * @param tight_box
 * flag to specify a tighter face box.
 * @param ao
 * acis options.
 **/
DECL_INTR outcome api_get_entity_box(
            const ENTITY_LIST &ent_list,
            WCS               *wcs,
            SPAposition       &min_pt,
            SPAposition       &max_pt,
            logical           tight_box = FALSE,
            AcisOptions       *ao       = NULL);

/** @} */


class entity_hit_list;
class ray_fire_options;
class ray;


/** @} */
#endif
