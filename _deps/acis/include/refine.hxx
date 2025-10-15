/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _REFINE
#define _REFINE
#include <stdio.h>
#include "dcl_fct.h"
#include "usecount.hxx"
#include "logical.h"
#include "entity.hxx"
class MESH_MANAGER;

/**
 * @file refine.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup FCTAPI
 *  \brief Declared at <refine.hxx>
 *  @{
 */

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( REFINEMENT, FCT )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

/**
 * @nodoc
 */
ENTITY *LW_REFINEMENT_restore_data();
#ifdef OLD_FIELDS

/**
 * @nodoc
 */
DECL_FCT logical modify_refinement (
                     REFINEMENT*,
                     double, double, double, double, double, double,
                     int, int, int
                  );
#endif

//DOC
//
// A REFINEMENT is an ACIS ENTITY for numeric and enumerated
// values that control faceting.
//
// REFINEMENT entities may be attached a the body, lump, shell,
// and face level of the ACIS heierarchy.   The faceter itself
// maintains both a set fixed defaults and an additional
// set of application-specified defaults to be applied
// when no refinement is specfied on any part of the heierarchy.
//
// See also:
//	XREF(REFINEMENT member functions),
//	XREF(AF_SURF_MODE),
//	XREF(AF_GRID_MODE),
//	XREF(AF_TRIANG_MODE),
//	XREF(AF_ADJUST_MODE),
//
//END
//
//ENUM
// TOPIC(AF_GRID_MODE,REFINEMENT class;Faceter enumerated types)
//
// The AF_GRID_MODE enumerated value in a XREF(REFINEMENT) controls
//	what how gridding is used in the mesh process.

/**
 * Specifies the grid mode.
 * @param AF_GRID_NONE
 * no grids at all.
 * @param AF_GRID_INTERIOR
 * grids in interior.
 * @param AF_GRID_TO_EDGES
 * allow grid to divide edges.
 * @param AF_GRID_ONE_DIR
 * very much like AF_GRID_INTERIOR, but for spherical faces the nodes are evenly spaced only in the parametric 'U' direction.
 */
typedef enum {
	AF_GRID_NONE,		// No grids at all.
	AF_GRID_INTERIOR,	// Grids in interior.
	AF_GRID_TO_EDGES,	// Allow grid to divide model edges.
	AF_GRID_ONE_DIR		// Grid only in one direction, u or v
	} AF_GRID_MODE ;
//END

#include "af_enum.hxx"

//	surface_deviation - max surface-facet deviation in world space
//	normal_deviation - max deviation between adjacent facet normals
//	flatness_tol   : max surface-facet deviation in image space
//	pixel_area_tol : max area of a facet in image space
//	max_edge_length - maximum permitted edge length
//	grid_aspect_ratio - maximum aspect ratio due to gridding only.
//	max_grid_lines - maximum number of grid_lines in one direction.
//	min_u_grid_lines - min number of u grid_lines in var U direction.
//	min_v_grid_lines - min number of v grid_lines in var V direction.
//	grading - enables iterative mesh grading improvement.
//	postcheck - enables determining accuracy of facetted model.


//ENUM
// TOPIC(AF_TRIANG_MODE, REFINEMENT class;Faceter enumerated types)
//
// The AF_TRIANG_MODE enumerated value in a XREF(REFINEMENT) identifies
//	what portion of the mesh is to be triangulated.
/**
 * Specifies triangulation mode.
 * @param AF_TRIANG_NONE
 * no triangulation.
 * @param AF_TRIANG_ALL
 * triangulate everywhere.
 * @param AF_TRIANG_FRINGE_1
 * triangulate against the boundary.
 * @param AF_TRIANG_FRINGE_2
 * triangulate first grid level.
 * @param AF_TRIANG_FRINGE_3
 * triangulate 3 levels of fringe.
 * @param AF_TRIANG_FRINGE_4
 * triangulate 4 levels of fringe.
 */
typedef enum {
	AF_TRIANG_NONE,				// No triangulation
	AF_TRIANG_ALL,				// Triangulate everywhere
	AF_TRIANG_FRINGE_1,			// Triangulate against the boundary
	AF_TRIANG_FRINGE_2,			// Also triangulate first grid level
	AF_TRIANG_FRINGE_3,			// Triangulate to 3 levels of fringe
	AF_TRIANG_FRINGE_4,			// Triangulate to 4 levels of fringe
	AF_TRIANG_BRIDGES
} AF_TRIANG_MODE ;

//ENUM
// TOPIC(AF_ADJUST_MODE, REFINEMENT class;Faceter enumerated types)
//
// The AF_ADJUST_MODE enumerated value in a XREF(REFINEMENT) identifies
//	which facet nodes are to be adjusted (smoothed) to other than their
//	initial grid positions.
/**
 * Specifies the adjustment mode.
 * @param AF_ADJUST_NONE
 * no adjustments.
 * @param AF_ADJUST_NON_GRID
 * smooth vertices not on grid points.
 * @param AF_ADJUST_ALL
 * smooth all vertices.
 */
typedef enum {
	AF_ADJUST_NONE,			// Leave everything alone.
	AF_ADJUST_NON_GRID,		// Adjust points surrounded by triangles by moving nodes in the center of surrounding nodes.
	AF_ADJUST_ALL			// Also adjusts grid nodes.
} AF_ADJUST_MODE ;

//oldDOC
// TOPIC(REFINEMENT old member functions,REFINEMENT class;Faceter classes)
// OLD STUFF...............
// view-dependent controls:
//    flatness_tol   : max surface-facet deviation in image space
//    silhouette_tol : max surface-facet deviation in image space at
//                           silhouettes (if < flatness_tol, the
//                           flatness_tol will be used)
//    pixel_area_tol : max area of a facet in image space
//
//    aspect_ratio : max aspect ratio of a facet
//    mode         : max number of sides for a facet
//                      0 : use default for surface
//                      1 : 3-sided polygons (default for patch surfaces)
//                      2 : 4-sided polygons (default for quadric surfaces)
//                      3 : n-sided polygons (default for planar surfaces)

// The refinement structure is referenced from ATTRIB_EYE_REF_VT attributes
//	in (possibly many) entities.
//END
//DOC
// TOPIC(REFINEMENT member functions,REFINEMENT class;Faceter classes)
//
//END
extern DECL_FCT int REFINEMENT_TYPE ;


/**
 * @nodoc
 */
#define REFINEMENT_LEVEL 1

// These values don't have to be terribly accurate,
//	so pack the doubles internally as floats:
// STI dgh typedef float	REFINEMENT_IFLOAT;

/**
 * @nodoc
 */
typedef double	REFINEMENT_IFLOAT;

/**
 * @nodoc
 */
typedef double	REFINEMENT_FLOAT;

/**
 * @nodoc
 */
#define OLD_FIELDS


/**
 * Controls the accuracy and types of polygons generated in the faceter.
 * <br>
 * <b>Role:</b> Refinements control how entities are faceted. Refinements can be attached
 * to a specific entity; for instance, a body or face. In addition, there are default
 * refinements that control the faceting of all entities whose faceting is not controlled by 
 * a refinement on the entity or a refinement on a higher level topological entity.
 * <br><br>
 * Each refinement applies to a specific type of %surface. Available %surface types are:
 * <br><br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td valign = "top" width = 25%><tt>AF_SURF_ALL</tt></td>
 * <td valign = "top">all %surface types. This is the default value.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>AF_SURF_REGULAR</tt></td>
 * <td valign = "top">a %surface with planar cells.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>AF_SURF_IRREGULAR</tt></td>
 * <td valign = "top">a %surface with possibly nonplanar cells.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>AF_SURF_PLANE</tt></td>
 * <td valign = "top">a planar %surface.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>AF_SURF_CONE</tt></td>
 * <td valign = "top">a conical %surface (including cylinder).</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>AF_SURF_SPHERE</tt></td>
 * <td valign = "top">a spherical %surface.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>AF_SURF_TORUS</tt></td>
 * <td valign = "top">a toroidal %surface.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>AF_SURF_SPLINE</tt></td>
 * <td valign = "top">a %spline %surface.</td>
 * </tr>
 * </table>
 * <br><br>
 * These parameters can be used to control the faceting:
 * <br><br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td valign = "top" width = 25%><tt>adjust mode</tt></td>
 * <td valign = "top">specifies type of triangle smoothing to do. Specifies
 * if facet nodes should be adjusted by smoothing. Also
 * specifies if the grid points should be adjusted or not.
 * The default value is <tt>AF_ADJUST_NONE</tt>.</td>
 * </tr>
 * <tr>
 * <td valign = "top" width = 25%><tt>normal tolerance</tt></td>
 * <td valign = "top">specifies the maximum angle (in degrees) between any two normals of a facet.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>%surface tolerance</tt></td>
 * <td valign = "top">specifies the maximum distance between the facet and the nearest
 * point on the %surface. This can be either a specific value or a fraction of the
 * length of the diagonal of the bounding box of the body containing the face being faceted.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>maximum edge length</tt></td>
 * <td valign = "top">specifies the maximum size of an edge of a facet.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>maximum grid lines</tt></td>
 * <td valign = "top">specifies the maximum number of subdivisions on a face.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>minimum u grid lines</tt></td>
 * <td valign = "top">specifies the minimum number of u grid lines used to facet a face.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>minimum v grid lines</tt></td>
 * <td valign = "top">specifies the minimum number of v grid lines used to facet a face.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>grid aspect ratio</tt></td>
 * <td valign = "top">specifies the approximate aspect ratio of each cell in grid.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>grid mode</tt></td>
 * <td valign = "top">specifies whether a grid is used and whether the points
 * where the grid cuts the edge is inserted to the edge.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>triangulation control</tt></td>
 * <td valign = "top">specifies how much triangulation to do.</td>
 * </tr>
 * </table>
 * <br><br>
 * For more information on the <tt>REFINEMENT</tt> class refer to the Technical Article <i>Refinements</i>.
 */
class DECL_FCT REFINEMENT : public ENTITY
{

/**
 * @nodoc
 */
	friend ENTITY *LW_REFINEMENT_restore_data();
	struct {

#ifdef mac
		unsigned 	grid_mode	 	:	 4;
		unsigned	triang_mode		:	 4;
		unsigned	adjust_mode		:	 4;
		unsigned	surf_mode		:	18;
#else
		AF_GRID_MODE 	grid_mode	 	:	 4;
		AF_TRIANG_MODE	triang_mode		:	 4;
		AF_ADJUST_MODE	adjust_mode		:	 4;
		AF_SURF_MODE	surf_mode		:	18;
#endif
		unsigned		grading_mode    :	1;
		unsigned		postcheck_mode	:	1;
		} tags;

	REFINEMENT_IFLOAT	surface_tol;
	REFINEMENT_IFLOAT	normal_tol;
	REFINEMENT_IFLOAT	silhouette_tol;
	REFINEMENT_IFLOAT	flatness_tol;
	REFINEMENT_IFLOAT	pixel_area_tol;
	REFINEMENT_IFLOAT	max_edge_length;
	REFINEMENT_IFLOAT	grid_aspect_ratio;
	REFINEMENT_IFLOAT	dynamic_surtol;
	int max_grid_lines;
	int min_u_grid_lines;
	int min_v_grid_lines;

#ifdef OLD_FIELDS
	int	min_level;			// Not supported
	int	max_level;			// Not supported
	float	max_aspect_ratio;			// Not supported
	int	max_sides_mode;			// Not supported
#endif

 // Callers that are going to update multiple fields
 //   should bracket the settings as a 'batch update'
 //   so that a backup occurs only before the first update:
   logical batch_update_mode ;


/**
 * @nodoc
 */
   void set_batch_update( logical on );

#ifdef OLD_FIELDS
 // Single entry to reset all externally visible fields:

/**
 * @nodoc
 */
 DECL_FCT friend logical modify_refinement 			// Not supported
                 (
                     REFINEMENT*,
                     double, double, double, double, double, double,
                     int, int, int
                  );
#endif

public :

/**
 * @nodoc
 */
   ENTITY_FUNCTIONS( REFINEMENT, FCT )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:

/**
 * @nodoc
 */
   LOOKUP_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

   // Create and delete

   // Create a default refinement

/**
  * C++ allocation constructor requests memory for this object but does not populate it.
  * <br><br>
  * <b>Role:</b> The allocation constructor is used primarily by restore. Applications
  * should call this constructor only with the overloaded new operator inherited
  * from the <tt>ENTITY</tt> class (for example, <tt>x=new REFINEMENT(...)</tt>), because this reserves
  * the memory on the heap, a requirement to support roll back and history management.
  * <br><br>
  * Creates a <tt>REFINEMENT</tt> with these settings:
  * <br><br>
  * <table width = 100%>
  * <tr width = 100%>
  * <td valign = "top" width = 25%>%surface type</td>
  * <td valign = "top">=</td>
  * <td valign = "top"><tt>AF_SURF_ALL</tt></td>
  * </tr>
  * <tr>
  * <td valign = "top">normal tolerance</td>
  * <td valign = "top">=</td>
  * <td valign = "top">30.0</td>
  * </tr>
  * <tr>
  * <td valign = "top">%surface tolerance</td>
  * <td valign = "top">=</td>
  * <td valign = "top">-1 (use bounding box diagonal)</td>
  * </tr>
  * <tr>
  * <td valign = "top">maximum edge length</td>
  * <td valign = "top">=</td>
  * <td valign = "top">0.0 (ignored)</td>
  * </tr>
  * <tr>
  * <td valign = "top">maximum grid lines</td>
  * <td valign = "top">=</td>
  * <td valign = "top">2048</td>
  * </tr>
  * <tr>
  * <td valign = "top">minimum u grid lines</td>
  * <td valign = "top">=</td>
  * <td valign = "top">0</td>
  * </tr>
  * <tr>
  * <td valign = "top">minimum v grid lines</td>
  * <td valign = "top">=</td>
  * <td valign = "top">0</td>
  * </tr>
  * <tr>
  * <td valign = "top">grid aspect ratio</td>
  * <td valign = "top">=</td>
  * <td valign = "top">0.0 (ignored)</td>
  * </tr>
  * <tr>
  * <td valign = "top">grid mode</td>
  * <td valign = "top">=</td>
  * <td valign = "top"><tt>AF_GRID_INTERIOR</tt></td>
  * </tr>
  * <tr>
  * <td valign = "top">triangulation control</td>
  * <td valign = "top">=</td>
  * <td valign = "top"><tt>AF_TRIANG_ALL</tt></td>
  * </tr>
  * <tr>
  * <td valign = "top">triangle smoothing</td>
  * <td valign = "top">=</td>
  * <td valign = "top"><tt>AF_ADJUST_NONE</tt></td>
  * </tr>
  * </table>
  */
  REFINEMENT();

/**
 * Indicates whether this entity is normally destroyed by <tt>lose</tt> (<tt>TRUE</tt>), 
 * or whether it is shared between multiple owners with a use count.
 * <br><br>
 * <b>Role:</b> If shared, it gets destroyed implicitly when every owner has
 * been lost (<tt>FALSE</tt>). The default is <tt>FALSE</tt>. This is an internal method that
 * should not be used by an application.
 */
   virtual logical deletable() const;

	//	Each of the following returns the respective enumerated
	//	control value in the REFINEMENT.

/**
 * Returns the grid mode.
 * <br><br>
 * <i><b>Note:</b> Refer to the</i> @href set_grid_mode <i>method</i>.
 */
	AF_GRID_MODE	get_grid_mode     () const;

/**
 * Returns the triangulation mode.
 * <br><br>
 * <i><b>Note:</b> Refer to the</i> @href set_triang_mode <i>method</i>.
 */
	AF_TRIANG_MODE	get_triang_mode   () const;

/**
 * Returns the surface mode.
 * <br><br>
 * <i><b>Note:</b> Refer to the</i> @href set_surf_mode <i>method</i>.
 */
	AF_SURF_MODE	get_surf_mode     () const;

/**
 * Returns the triangle smoothing mode.
 * <br><br>
 * <i><b>Note:</b> Refer to the</i> @href set_adjust_mode <i>method</i>.
 */
	AF_ADJUST_MODE	get_adjust_mode   () const;

/**
 * @nodoc
 */
	logical         get_grading_mode  () const;		// Not supported

/**
 * @nodoc
 */
	logical		    get_postcheck_mode() const;		// Not supported

	//  Each of the following returns the respective numeric control value from the refinement.
	//		                                         // STI bas - change to REFINEMENT_IFLOAT to make consistent

/**
 * Returns the normal tolerance, which is the maximum difference between any two normals of facet.
 * <br><br>
 * <i><b>Note:</b> Refer to the</i> @href set_normal_tol <i>method</i>.*/
	REFINEMENT_IFLOAT get_normal_tol       () const; //           with the type of the corresponding private

/**
 * Returns the surface tolerance.
 * <br><br>
 * <i><b>Note:</b> Refer to the</i> @href set_surface_tol <i>method</i>.*/
	REFINEMENT_IFLOAT get_surface_tol      () const; //           class variable

/**
 * @nodoc
 */
	REFINEMENT_IFLOAT get_silhouette_tol   () const;	// Not supported

/**
 * @nodoc
 */
	REFINEMENT_IFLOAT get_pixel_area_tol   () const;	// Not supported

/**
 * @nodoc
 */
	REFINEMENT_IFLOAT get_flatness_tol     () const;	// Not supported

/**
 * Returns the maximum allowable size of an edge of a facet.
 * <br><br>
 * <i><b>Note:</b> Refer to the</i> @href set_max_edge_length <i>method</i>.*/
	REFINEMENT_IFLOAT get_max_edge_length  () const;

/**
 * Returns the grid aspect ratio.
 * <br><br>
 * <b>Role:</b> The grid aspect ratio is the approximate ratio of the length in
 * the u direction to the length in the v direction of each uv cell in the grid.
 * <br><br>
 * <i><b>Note:</b> Refer to the</i> @href set_grid_aspect_ratio <i>method</i>.
 */
	REFINEMENT_IFLOAT get_grid_aspect_ratio() const;

/**
 * Returns the dynamic surface tolerance.
 * <br><br>
 * <b>Role:</b> If a refinement is attached to a body and a dynamic or relative 
 * surface tolerance is used (that is, the given value for the surface tolerance
 * is negative, causing the surface tolerance to be calculated as a fraction of
 * the length of the diagonal of the body's bounding box), then this method can
 * be called after faceting the body to obtain the surface tolerance value used.
 * This can also be used with a default refinement if a single body is being
 * faceted.
 */
	REFINEMENT_IFLOAT get_dynamic_surtol   () const;

/**
 * Returns the maximum allowable number of grid lines for a refinement.
 * <br><br>
 * <i><b>Note:</b> Refer to the</i> @href set_max_grid_lines <i>method</i>.
 */
	int get_max_grid_lines  () const;

/**
 * Returns the minimum number of <i>u</i> grid lines.
 * <br><br>
 * <i><b>Note:</b> Refer to the</i> @href set_min_u_grid_lines <i>method</i>. */
	int get_min_u_grid_lines() const;

/**
 * Returns the minimum number of <i>v</i> grid lines.
 * <br><br>
 * <i><b>Note:</b> Refer to the</i> @href set_min_v_grid_lines <i>method</i>. */
	int get_min_v_grid_lines() const;

	//END_MEMBER

	// secondary query ops

/**
 * @nodoc
 */
	logical			need_grid() const;

	// set ops
	//MEMBER
	// Each of the following sets the respective enumerated control
	// value in the REFINEMENT.

/**
 * The grid mode specifies whether a grid is used and whether the points where the grid cuts the edges should be inserted into the edge discretization.
 * <br><br>
 * <b>Role:</b> These are the allowable values:
 * <br><br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td valign = "top" width = 25%><tt>AF_GRID_NONE</tt></td>
 * <td valign = "top">do not subdivide face with a grid.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>AF_GRID_INTERIOR</tt></td>
 * <td valign = "top">use a grid but do not add model edge intersection points. (This is the default value.)</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>AF_GRID_ONE_DIR</tt></td>
 * <td valign = "top">similar to AF_GRID_INTERIOR, but spherical faces have nodes that are evenly spaced only in the parametric <i>u</i> direction, and the <i>v</i> direction spacing is variable.
 * </tr>
 * <tr>
 * <td valign = "top"><tt>AF_GRID_TO_EDGES</tt></td>
 * <td valign = "top">use a grid and insert edge intersections points.</td>
 * </tr>
 * </table>
 * <br>
 * @param gridmode
 * mode to set.
 */
	void set_grid_mode  (AF_GRID_MODE gridmode);

/**
 * This sets the triangulation control, which specifies how much triangulation to perform.
 * <br><br>
 * <b>Role:</b> If <tt>AF_GRID_INTERIOR</tt> or <tt>AF_GRID_ONE_DIR</tt> is specified, triangulation will be performed
 * at least at the fringe cells. Allowable values are:
 * <br><br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td valign = "top" width = 25%><tt>AF_TRIANG_NONE</tt></td>
 * <td valign = "top">no triangulation.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>AF_TRIANG_FRINGE_1</tt></td>
 * <td valign = "top">triangulate at the fringe layer.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>AF_TRIANG_FRINGE_2</tt></td>
 * <td valign = "top">triangulate 2 fringe layers.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>AF_TRIANG_FRINGE_3</tt></td>
 * <td valign = "top">triangulate 3 fringe layers.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>AF_TRIANG_FRINGE_4</tt></td>
 * <td valign = "top">triangulate 4 fringe layers.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>AF_TRIANG_ALL</tt></td>
 * <td valign = "top">triangulate all facets. (This is the default value.)</td>
 * </tr>
 * </table>
 * <br>
 * @param triangmode
 * mode to set
 */
	void set_triang_mode(AF_TRIANG_MODE triangmode);

/**
 * The adjustment mode is used for triangle smoothing.
 * <br><br>
 * <b>Role:</b> It specifies whether triangles should be smoothed. The nongrid mode
 * preserves the planarity of cells by avoiding points that are corners of a cell.
 * The allowable values are:
 * <br><br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td valign = "top" width = 25%><tt>AF_ADJUST_NONE</tt></td>
 * <td valign = "top">no smoothing. (This is the default value.)</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>AF_ADJUST_NON_GRID</tt></td>
 * <td valign = "top">applies to points that are not part of a cell.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>AF_ADJUST_ALL</tt></td>
 * <td valign = "top">adjust all points connected to triangles.</td>
 * </tr>
 * </table>
 * <br>
 * When triangle smoothing is performed grid based tolerances may not be satisified. 
 * <br><br>
 * @param adjustmode
 * mode to set.
 */
	void set_adjust_mode(AF_ADJUST_MODE adjustmode);

/**
 * @nodoc
 */
	void set_grading_mode(logical onoff);		// Not supported

/**
 * @nodoc
 */
	void set_postcheck_mode( logical );     // Not supported

/**
 * The %surface mode specifies the type of %surface to which the refinement is applicable.
 * <br><br>
 * <b>Role:</b> The allowable %surface modes are listed below. If more than one refinement
 * is applicable to a face, the more specific one overrides the less specific one.
 * <br><br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td valign = "top" width = 25%><tt>AF_SURF_ALL</tt></td>
 * <td valign = "top">all %surface types. (This is the default value.)</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>AF_SURF_REGULAR</tt></td>
 * <td valign = "top">a %surface with planar cells.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>AF_SURF_IRREGULAR</tt></td>
 * <td valign = "top">a %surface with possibly nonplanar cells.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>AF_SURF_PLANE</tt></td>
 * <td valign = "top">a planar %surface.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>AF_SURF_CONE</tt></td>
 * <td valign = "top">a conical %surface (including cylinder).</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>AF_SURF_SPHERE</tt></td>
 * <td valign = "top">a spherical %surface.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>AF_SURF_TORUS</tt></td>
 * <td valign = "top">a toroidal %surface.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>AF_SURF_SPLINE</tt></td>
 * <td valign = "top">a %spline %surface.</td>
 * </tr>
 * </table>
 * <br>
 * @param surfmode
 * surface mode value.
 */
	void set_surf_mode(AF_SURF_MODE surfmode);

	//END_MEMBER

	//MEMBER
	// Each of the following sets the respective numeric control value
	//	in the REFINEMENT.

/**
 * The normal tolerance	specifies the maximum normal deviation (in degrees) allowed between two normals on two adjacent facet nodes.
 * <br><br>
 * <b>Role:</b> The value is independent of the model size. The default value is 30.0 degrees.
 * <br><br>
 * @param dev
 * normal tolerance value.
 */
	void set_normal_tol       ( REFINEMENT_IFLOAT dev); // STI bas - changed to REFINEMENT_IFLOAT to be consistent

/**
 * The %surface tolerance specifies the maximum distance between a facet and the true %surface.
 * <br><br>
 * <b>Role:</b> When this value is positive, it specifies the %surface tolerance value. In this case
 * the number of facets depends upon the model size. When this value is negative, it specifies a relative
 * %surface tolerance value. The %surface tolerance is the absolute value of this negative number 
 * multiplied by a fraction (1/500) of the length of the diagonal of the bounding box of the body
 * containing the face. The default value is -1.0, meaning the %surface tolerance is (1/500) of the 
 * length of the diagonal of the bounding box of the body containing the face. 
 * <br><br>
 * @param val
 * %surface tolerance value.
 */
	void set_surface_tol      ( REFINEMENT_IFLOAT val); //           with the type of the corresponding private

/**
 * Grid aspect ratio specifies the approximate aspect ratio of each cell in the grid.
 * <br><br>
 * <b>Role:</b> If the value is close to 1, then the cell is close to a square.
 * This does not guarantee the aspect ratio of the facet, which may consist of
 * only a part of a cell. The default value is 0.0, meaning this parameter is ignored.
 * <br><br>
 * @param rat
 * aspect ratio value.
 */
	void set_grid_aspect_ratio( REFINEMENT_IFLOAT rat); //           class variable

/**
 * The maximum edge length specifies the maximum length of a side of a cell in object space.
 * <br><br>
 * <b>Role:</b> Since a facet cannot be larger than the cell, this determines
 * the maximum size of the facet. The default value is 0.0, meaning this parameter is ignored.
 * <br><br>
 * @param len
 * maximum edge length.
 */
	void set_max_edge_length  ( REFINEMENT_IFLOAT len);

/**
 * @nodoc
 */
	void set_silhouette_tol   ( REFINEMENT_IFLOAT );

/**
 * @nodoc
 */
	void set_dynamic_surtol   ( REFINEMENT_IFLOAT val);

/**
 * @nodoc
 */
	void set_pixel_area_tol   ( REFINEMENT_IFLOAT );

/**
 * @nodoc
 */
	void set_flatness_tol     ( REFINEMENT_IFLOAT );

/**
 * The maximum grid lines specifies the maximum number of grid subdivisions.
 * <br><br>
 * <b>Role:</b> This prevents the facet data of a face from getting too big.
 * It can also be used to specify the exact number of divisions on a face by
 * using it in conjunction with another parameter; e.g., a very small
 * normal deviation. The default value is 2048. 
 * <br><br>
 * @param grid
 * maximum number of grid lines.
 */
	void set_max_grid_lines  (int grid);

/**
 * Sets the minimum number of <i>u</i> grid lines.
 * <br><br>
 * <b>Role:</b> When the minimum number of <i>u</i> grid lines is a non-zero value, 
 * the faceting algorithm inserts the minimum number of uniformly sized grid cells 
 * in the <i>u</i> direction before attempting to satisify any other constraints. 
 * This refinement parameter has no meaning when grid mode is set to <tt>AF_GRID_NONE</tt>. 
 * The default value is 0.
 * <br><br>
 * @param grid
 * at least this number of grid lines are generated in <i>u</i> direction.
 */
	void set_min_u_grid_lines(int grid);

/**
 * Sets the minimum number of <i>v</i> grid lines.
 * <br><br>
 * <b>Role:</b> When the minimum number of <i>v</i> grid lines is a non-zero value, 
 * the faceting algorithm inserts the minimum number of uniformly sized grid cells 
 * in the <i>v</i> direction before attempting to satisify any other constraints. 
 * This refinement parameter has no meaning when grid mode is set to <tt>AF_GRID_NONE</tt>. 
 * The default value is 0.
 * <br><br>
 * @param grid
 * at least this number of grid lines are generated in <i>v</i> direction.
 */
	void set_min_v_grid_lines(int grid);

	//END_MEMBER

#ifdef OLD_FIELDS
/**
 * @nodoc
 * <b>Obsolete:</b> used only in pre-1-7 Faceting - Creates a refinement.
 * <br><br>
 * @param min
 * minimum.
 * @param max
 * maximum.
 * @param ftol
 * f tolerance.
 * @param stol
 * s tolerance.
 * @param sdev
 * s deviation.
 * @param ndev
 * n deviation.
 * @param ptol
 * p tolerance.
 * @param ar
 * ar float.
 * @param m
 * m point.
 * @param edge_length
 * edge length.
 */
   REFINEMENT( int min, int max,
//             float ftol, float stol, float sdev, float ndev, float ptol,   // STI bas
               REFINEMENT_IFLOAT ftol, REFINEMENT_IFLOAT stol, REFINEMENT_IFLOAT sdev, REFINEMENT_IFLOAT ndev, REFINEMENT_IFLOAT ptol,
               float ar, // always float
               int m,
               REFINEMENT_IFLOAT edge_length // STI bas - was "float"
               );	// Not supported

/**
 * @nodoc
 */
   int   minimum_level() const     { return(min_level ); }	// Not supported
/**
 * @nodoc
 */
   int   maximum_level() const     { return(max_level ); }	// Not supported

   // STI bas - changed from "float" to REFINEMENT_IFLOAT to be consistent with the type of the corresponding private class variable
/**
 * @nodoc
 */
   REFINEMENT_IFLOAT flatness         () const { return flatness_tol;   }	// Not supported
/**
 * @nodoc
 */
   REFINEMENT_IFLOAT silhouette       () const { return silhouette_tol; }	// Not supported
/**
 * @nodoc
 */
   REFINEMENT_IFLOAT surface_deviation() const { return surface_tol;    }	// Not supported
/**
 * @nodoc
 */
   REFINEMENT_IFLOAT edge_length      () const { return max_edge_length;}	// Not supported
/**
 * @nodoc
 */
   REFINEMENT_IFLOAT normal_deviation () const { return normal_tol;     }	// Not supported
/**
 * @nodoc
 */
   REFINEMENT_IFLOAT pixel_area       () const { return pixel_area_tol; }	// Not supported
/**
 * @nodoc
 */
   float             aspect_ratio     () const { return max_aspect_ratio; }	// Not supported
/**
 * @nodoc
 */
   int               mode             () const { return max_sides_mode; }	// Not supported


   // Set criterion values
/**
 * @nodoc
 */
   void set_minimum_level( int level );	// Not supported
/**
 * @nodoc
 */
   void set_maximum_level( int level );	// Not supported

   // STI bas - changed from "float" to REFINEMENT_IFLOAT to be consistent with the type of the corresponding private class variable
/**
 * @nodoc
 */
   void set_flatness         ( REFINEMENT_IFLOAT tol );	// Not supported
/**
 * @nodoc
 */
   void set_silhouette       ( REFINEMENT_IFLOAT tol );	// Not supported
/**
 * @nodoc
 */
   void set_surface_deviation( REFINEMENT_IFLOAT tol );	// Not supported
/**
 * @nodoc
 */
   void set_edge_length      ( REFINEMENT_IFLOAT tol );	// Not supported
/**
 * @nodoc
 */
   void set_normal_deviation ( REFINEMENT_IFLOAT tol );	// Not supported
/**
 * @nodoc
 */
   void set_pixel_area       ( REFINEMENT_IFLOAT tol );	// Not supported
/**
 * @nodoc
 */
   void set_aspect_ratio( float ar );	// Not supported
/**
 * @nodoc
 */
   void set_mode( int m );	// Not supported
#endif

   // Test if any view dependent criteria are active:

/**
 * @nodoc
 */
   logical view_dependent() const		// Not supported
   {
      return(( flatness_tol > 0.0 ) ||
              (silhouette_tol > 0.0 ) ||
              (pixel_area_tol > 0.0 ) );
   }

// STI jmb: Standard use count implementation
//   REFINEMENT* share() { use(); return(this ); }
/**
 * Increments the use count if "this" and returns "this."
 */
	REFINEMENT* share();
	/**
	 * @nodoc
	 */
	USE_COUNTED_DECL
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// STI jmb: end

/**
 * Determines if two refinements have the same parameter values.
 * <br><br>
 * @param ref
 * refinement to test.
 */
   logical same( REFINEMENT* ref) const ;

/**
 * This creates and returns a new <tt>REFINEMENT</tt> that is a copy of this <tt>REFINEMENT</tt>.
 */
   REFINEMENT* copy() const;

/**
 * Copies the contents of this instance of <tt>REFINEMENT</tt> to the given <tt>REFINEMENT</tt>.
 * <br><br>
 * <b>Role:</b> In addition to copying the contents of this refinement to the given refinement,
 * the use count of the given refinement is reset to 0. This member function should be used with caution.
 * <br><br>
 * @param to
 * where to copy.
 */
   void copy_to( REFINEMENT *to ) const;

/**
 * Merges two refinements.
 * <br><br>
 * <b>Role:</b> The parameter values of this refinement are set to the tighter of the parameter values
 * of this refinement and the given refinement.
 * <br><br>
 * @param ref
 * second refinement.
 */
   void merge( REFINEMENT* ref);

/**
 * Notifies the <tt>REFINEMENT</tt> that its owning <tt>ENTITY</tt> is about to be merged with given entity.
 * <br><br>
 * <b>Role:</b> The application has the chance to delete or otherwise modify the
 * attribute. After the merge, this owner will be deleted if the logical deleting
 * owner is <tt>TRUE</tt>, otherwise it will be retained and other entity will be deleted.
 * The default action is to do nothing. This function is supplied by the
 * application whenever it defines a new attribute, and is called when a merge occurs.
 * <br><br>
 * @param other
 * given entity.
 * @param delete_other
 * deleting owner.
 */
   void merge_owner( ENTITY* other, logical delete_other );

/**
 * Notifies the <tt>REFINEMENT</tt> that its owner is about to be split into two parts.
 * <br><br>
 * <b>Role:</b> The application has the chance to duplicate or otherwise modify
 * the attribute. The default action is to do nothing. This function is
 * supplied by the application whenever it defines a new attribute, and is
 * called when a split occurs.
 * <br><br>
 * @param new_entity
 * new entity.
 */
   void split_owner( ENTITY* new_entity );

/**
 * Prints information about this refinement.
 * <br><br>
 * <b>Role:</b> MAC, NT platforms only.
 * <br><br>
 * @param fp
 * file.
 */
   void print( FILE* fp= stderr ) const ;
};


/**
 * @nodoc
 */
DECL_FCT int af_triang_depth(AF_TRIANG_MODE tmode);

/* @} */

#endif
