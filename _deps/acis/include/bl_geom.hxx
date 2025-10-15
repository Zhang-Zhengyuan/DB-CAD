/* ORIGINAL: acis2.1/kernbool/blending/bl_geom.hxx */
/* $Id: bl_geom.hxx,v 1.16 2002/08/09 22:44:09 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


// Declarations of general blend geometry classes.

#if !defined BL_GEOM_HXX
#define BL_GEOM_HXX

// Adele Modification:
//  18-Apr-07 KPI   Added help point in project_pc to choose a solution 
//                  from multiple projections.
//  12-Mar-03 syh   New function int_from_cur_sur to handle intersection with
// 		coincidence. 

// Modification:
//	31-Mar-99 bd	New status "impossible", for point_cur and ffblend_geom.
//	17-Sep-98 dap	Finally added fn to return curve non-const. This was
//		driving me nuts!
//	19-Dec-97 bd	Added point_cur_surf::is_unset().
//  30-Jul-97 dap	Added use of value -1 to cc_x_found in cc_x_info
// 		class to mean "unset".
//

class ffblend_geom;
class vblend_geom;
class point_cur;
class point_cur_surf;

class SPApar_pos;
class curve;
class cc_x_info;
class SPAinterval;
class pcurve;
class SPAposition;
class sphere;
class surface;
class vsegend;

#include "dcl_blnd.h"

#include "bl_att.hxx"

#include "curdef.hxx"
#include "elldef.hxx"
#include "strdef.hxx"

#include "param.hxx"
#include "position.hxx"
#include "cusfint.hxx"

// Declare a class to hold a blend surface and details of spring 
// curves (or points) for left and right and spine curve (or point).  
// Record auxiliary surfaces (that contain the corresponding spring
// curve and whose normals are given by n X t where n is the normal
// to the blend surface and t the tangent along the spring curve).
// Auxiliary surfaces are required for blends i.e. when the blend
// surface meets its neighbours tangentially (i.e. can be omitted
// for chamfers).

// First define a class to record a spring point or a spring curve
// plus an optional auxiliary surface set when the blend is slope or
// curvature continuous.  If the blend surface is a spline, record a
// pcurve for the curve.  If the surface adjacent to the blend is a
// spline, record also a pcurve for the curve on that surface.

// The class will own the curve, pcurves and surface it references.

enum pc_type {pc_unset, pc_curve, pc_point, pc_impossible};

class DECL_BLND point_cur : public ACIS_OBJECT
{
private:
	pc_type		type_data;
	curve		*cur_ptr;
	SPAinterval	range_data;
	surface 	*aux_surf_ptr;
	surface 	*other_surf_ptr;
	pcurve		*bl_pcur_ptr;
	pcurve		*other_pcur_ptr;
	SPAposition	pos_data;
	SPApar_pos		*pos_uv_ptr;
	SPApar_pos		pos_uv_data;

public:
	// Constructors.

	point_cur();

	// Allow an extra argument for a SPAparameter value, to be kept as
	// a range SPAinterval with equal bounds, for the case of an edge
	// having setback but a blend of null size.
	// Allow a further argument so that a SPApar_pos can be recorded for
	// a point projected on to a spline surface.

	point_cur(SPAposition const &,
			  double = 0,
			  SPApar_pos const & = *(const class SPApar_pos *)NULL_REF
		);

	point_cur(curve const &);

	point_cur(curve const &,
			  SPAbox const &,
			  surface const & = *(surface const*)NULL_REF,	// aux surf
			  surface const & = *(surface const*)NULL_REF,	// other surf
			  pcurve const & = *(pcurve const*)NULL_REF,	// bl pcur
			  pcurve const & = *(pcurve const*)NULL_REF		// other pcur
		);

	point_cur(curve const &,
			  SPAinterval,
			  surface const & = *(surface const*)NULL_REF,	// aux surf
			  surface const & = *(surface const*)NULL_REF,	// other surf
			  pcurve const & = *(pcurve const*)NULL_REF,	// bl pcur
			  pcurve const & = *(pcurve const*)NULL_REF		// other pcur
		);

	point_cur(point_cur const &);

	// Destructor.

	~point_cur();

	// Assignment.

	point_cur &operator=(point_cur const &);

	// Negation.

	void negate();

	// Access to members.

	pc_type type() const { return type_data; }
	curve const *cur() const;
	curve *cur_for_update() const;
	SPAinterval range() const;
	SPAinterval &range_for_update();
	surface const *aux_surf() const;
	surface const *other_surf() const;
	pcurve const *bl_pcur() const;
	pcurve const *other_pcur() const;
	pcurve *bl_pcur_for_update() const;
	pcurve *other_pcur_for_update() const;
	SPAposition pos() const;
	SPApar_pos const &pos_uv() const;	// returns uv or null if unset
	logical is_unset() const { return type_data==pc_unset; }
	logical is_impossible() const;

	// Set values.

	void set_pos_uv(SPApar_pos const &);
	void set_cur(curve const &);
	void set_range(SPAinterval const &);
	void set_aux_surf(surface const &);
	void set_other_surf(surface const &);
	void set_bl_pcur(pcurve const &);
	void set_other_pcur(pcurve const &);
	void declare_impossible() { type_data = pc_impossible; }

	// Tests.

	logical is_straight() const {
		return
			type_data==pc_curve &&
			cur_ptr->type()==straight_type; }
	logical is_circle() const {
		return
			type_data==pc_curve &&
			cur_ptr->type()==ellipse_type &&
			((ellipse*)cur_ptr)->radius_ratio==1; }
	logical is_curve() const { return type_data==pc_curve; }
	logical is_point() const { return type_data==pc_point; }
	logical parallel(point_cur const&) const;	// (and not coincident)
	logical coaxial(point_cur const&) const;
	logical on_sphere(sphere const&) const;

	// Print out a point_cur.

	void debug( char const *, FILE * = debug_file_ptr ) const;
};

// Declare a class to hold a point, curve or surface.

enum pcs_type {pcs_unset, pcs_surface, pcs_point_curve, pcs_impossible};

class DECL_BLND point_cur_surf : public ACIS_OBJECT
{
private:
	pcs_type	type_data;
	point_cur	pc_data;
	surface 	*surf_ptr;

public:
	// Constructors.

	point_cur_surf();

	point_cur_surf(point_cur const &);

	point_cur_surf(surface const &);

	point_cur_surf(point_cur_surf const &);

	// Destructor.

	~point_cur_surf();

	// Assignment.

	point_cur_surf &operator=(point_cur_surf const &);

	// Access to members.

	pcs_type type() const { return type_data; }
	
	// STIBUGFIX erich r2948 begin
	// Don't create a temporary object when invoking this!  (Massive bug on NT)
	point_cur	const &pc() const;
	// STIBUGFIX erich r2948 end

    SPAposition	pos() const;
    curve const *cur() const;
	surface	const *surf() const;

	// Tests.

	logical is_point() const {
		return	type_data==pcs_point_curve && pc_data.is_point(); }
	logical is_curve() const {
		return	type_data==pcs_point_curve && pc_data.is_curve(); }
	logical is_point_curve() const { return	type_data==pcs_point_curve; }
	logical is_surface() const { return type_data==pcs_surface; }
	logical is_unset() const { return type_data==pcs_unset; }
	logical is_impossible() const { return type_data==pcs_impossible; }

	// Member-setting methods.

	void declare_impossible() { type_data = pcs_impossible; }

	void debug( char const *, FILE * = debug_file_ptr ) const;
};

DECL_BLND point_cur_surf 
offset_surface(
		surface const &,
		double,
		SPAbox const & =		// Bounds result when a curve
			*(SPAbox *)NULL_REF	
		);

// Project a point or curve on to a surface.

DECL_BLND point_cur_surf 
project_pc(
	point_cur const &,	// projectee
	surface const &,	// on surface
	surface const &,	// set non-null if projectee is an intcurve and
						// on_surf is an offset of this surface
	double,				// offset (negative for concave blend)
	plane const &,		// thru which result curve shd pass forwards
	SPAbox const &,		// confine result to SPAbox
	double,				// tol
	SPAposition const & =  // choosing a solution from multiple projection
			*(SPAposition const*)NULL_REF
	);

// Record the point of intersection of one spring curve of an edge
// with the spring curve of the cc adjacent edge at an end of the edge
// and the SPAparameter value for that point projected to the spine of
// the edge and its cc neighbour.

class DECL_BLND cc_x_info : public ACIS_OBJECT
{
public:
	// This is now an int where -1 means "spring curve intersection not
	// yet done", 0 means "done but no intersection found", 1 means
	// "done and intersection found and recorded".

	int			cc_x_found;

	SPAposition	cc_x_pos;
	SPApar_pos		cc_x_uv;
	SPAparameter	x_param;
	SPAparameter	cc_param;

	cc_x_info();	// construct empty cc_x_info

	// Note that an intersection is known to exist.

	void set_cc_x_info(
		SPAposition const &,
		SPAparameter,
		SPAparameter,
		SPApar_pos const & = *(SPApar_pos *)NULL_REF);

	// Note that there is known to be no intersection.

	void set_cc_x_info() { cc_x_found = 0; }

	logical known() const { return cc_x_found != -1; }
	logical found() const { return cc_x_found == 1; }

	// Print out a cc_x_info.

	void debug( FILE * = debug_file_ptr ) const;
};

// Record geometry of a face-face blend.

class DECL_BLND ffblend_geom : public ACIS_OBJECT
{
private:
	unsigned int use_count_data;	// number of references to this

public:
	surface		*blend_surf;
	point_cur	spine_pc;
	point_cur	left_pc;
	point_cur	right_pc;
	cc_x_info	start_cc_x;
	cc_x_info	end_cc_x;

	// Use count manipulation. Either add or subtract one use, and if 
	// subtraction causes the use count to fall to zero, then delete 
	// the record.

	ffblend_geom *add();
	void remove();

	// Constructors.

	ffblend_geom();

	ffblend_geom(
			point_cur const &left,
			point_cur const &right,
			point_cur const &spine,
			surface *blnd_sf = NULL
			);

	// Utilities

	logical is_impossible() {
		return spine_pc.is_impossible()
			||  left_pc.is_impossible()
			|| right_pc.is_impossible();
	}

	void declare_impossible();

	int ref_count() {return use_count_data; };

	// Print out a blend_geom.

	void debug( FILE * = debug_file_ptr ) const;

private:
	~ffblend_geom();	// [force deletion to be done with remove()]
};


// Define a class for returning vertex blend geometry.  Contains the
// blend surface and a chain of vsegends defining the vertex blend
// boundary which is ordered counterclockwise round the vertex.
 
class DECL_BLND vblend_geom : public ACIS_OBJECT
{
private:
	unsigned int use_count_data;	// number of references to this

public:
	surface		*blend_surf;
	vsegend		*vseg_chain;

	// Use count manipulation. Either add or subtract one use, and if 
	// subtraction causes the use count to fall to zero, then delete 
	// the record.

	void add();
	void remove();

	// Constructors.

	vblend_geom
	(
		surface* = NULL,	// blend surf
		vsegend* = NULL		// vsegend chain
	);

	// Print out a blend_geom.

	void debug( FILE * = debug_file_ptr ) const;

private:
	~vblend_geom();	// [force deletion to be done with remove()]
};

DECL_BLND SPAunit_vector perp(SPAunit_vector const &);


// Here are a few routines generally useful in blending.

// Check blend for too tight a curvature at n_pnts points.

DECL_BLND logical 
check_blgeom_cvtr_ok(
	curve const *,	// left_spring
	curve const *,	// right_spring
	curve const *,	// spine
	double,			// abs_rad
	int,			// n_pnts
	double,			// start_p
	double			// end_p
	);

// Find intersection at which a curve cuts a surface within SPAbox if any, near
// given point, and with specific sense (in-out or out_in) if given. Return true
// and curve_surf_int if successful.

DECL_BLND logical
int_from_cur_sur(
	curve const *, 			// curve to intersect.
	surface const *, 		// surface to be intersected with. 
	SPAbox const &, 		// region of interest
	SPAposition const &,	// find intersection nearest to this posistion.  
	logical const &,		// sense of intersection, 
	                        // 		TRUE  if out-in, 
	curve_surf_int *&		// result of intersection.
	);


// Find point in which a curve cuts a surface within SPAbox if any, near
// given point, and with specific sense (in-out or out_in) if given.  
// Return true if successful and SPAparameter on curve and SPAposition.

DECL_BLND logical
pos_from_cur_sur(
	curve const *,
	surface const *,
	SPAbox const &,
	SPAposition const &,
	logical const &,
	SPAparameter &,
	SPAposition &
	);

// Find points in which spring curves (or points) cut given plane.

DECL_BLND logical 
find_spring_points(
	ffblend_geom *, 
	plane const &, 
	SPAbox const &,
	SPAposition &, 
	SPAposition &, 
	SPAposition & = *(SPAposition *)NULL_REF
	);

// Find overlap of two intervals (and deal with intervals of periodic
// curves).  Make resulting SPAinterval "similar" (i.e. to within curve
// period) to range of projectee.

DECL_BLND SPAinterval 
interval_overlap(
	SPAinterval const &a,
	SPAinterval const &b,
	curve const &cu
	);

// Find whether the overlap between the two intervals is more than one
// connected regions.

logical
multiple_overlap(
	SPAinterval const &a,
	SPAinterval const &b,
	curve const &cu
	);

DECL_BLND logical
setback_too_large( 
	EDGE *this_ent, 
	double start_setback, 
	double end_setback
	);

#endif
