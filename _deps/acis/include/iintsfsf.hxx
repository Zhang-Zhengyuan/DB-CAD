/* ORIGINAL: acis2.1/kernint/intsfsf/iintsfsf.hxx */
// $Id: iintsfsf.hxx,v 1.18 2002/08/09 17:18:52 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Internal header file for surface-surface intersection.

// Intersect the surfaces of two faces, returning zero or more 
// curves. We may assume that every edge of each face has already 
// been intersected with the other surface, so the intersection 
// points may be used to assist us (for example if the surfaces 
// are parametric).

// In difference to the public interfaces those function do take 
// a tolerance which is used to perform position tests.

// For the general intersection mechanism it is possible to specify a
// parameter region for the two intersecting surfaces. This parameter box
// is only used in the general intersection mechanism.

#if !defined( IINTSFSF_HDR_DEF )
#define  IINTSFSF_HDR_DEF

#include "logical.h"

#include "dcl_intr.h"
#include "sfsfint.hxx"
#include "debugmsc.hxx"

class surface;
class FACE;
class SPAtransf;

class plane;
class cone;
class sphere;
class torus;
class spline;

class SPAbox;


// Specific surface-surface intersections.

// In intplpl.cxx

DECL_INTR surf_surf_int *int_plane_plane(
			plane const &,
			plane const &,
			double const,
			SPAbox const & = *(SPAbox *)NULL_REF,            
			SPApar_box const & = *(SPApar_box *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF
		);

DECL_INTR surf_surf_int *int_plane_plane_coi(
			plane const &,
			plane const &,
			double const,
			SPAbox const & = *(SPAbox *)NULL_REF,            
			SPApar_box const & = *(SPApar_box *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF
		);

// In intplco.cxx

DECL_INTR surf_surf_int *int_plane_cone(
			plane const &,
			cone const &,
			double const,
			logical = FALSE,
			SPAbox const & = *(SPAbox *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF
		);


// In intplsh.cxx

DECL_INTR surf_surf_int *int_plane_sphere(
			plane const &,
			sphere const &,
			double const,
			logical = FALSE,
			SPAbox const & = *(SPAbox *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF
		);


// In intplto.cxx

DECL_INTR surf_surf_int *int_plane_torus(
			plane const &,
			torus const &,
			double const,
			logical = FALSE,
			SPAbox const & = *(SPAbox *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF
		);


// In intplsp.cxx

DECL_INTR surf_surf_int *int_plane_spline(
			plane const &,
			spline const &,
			double const,
			logical = FALSE,
			SPAbox const & = *(SPAbox *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF
		);

// In intcoco.cxx

DECL_INTR surf_surf_int *int_cone_cone(
			cone const &,
			cone const &,
			double const,
			SPAbox const & = *(SPAbox *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF
		);

DECL_INTR surf_surf_int *int_cone_cone_coi(
			cone const &,
			cone const &,
			double const,
			SPAbox const & = *(SPAbox *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF
		);

// STI pmalrais 11/19/97
// 
// bug 4594
// make this a public function
// a utility used here and in utilcosf.cxx

DECL_INTR logical nearest_approach(
			SPAposition const &,
			SPAunit_vector const &,
			SPAposition const &,
			SPAunit_vector const &,
			SPAposition &,
			SPAposition &,
			double const
		);
// STI pmalrais 11/19/97 end

// In intcosh.cxx

DECL_INTR surf_surf_int *int_cone_sphere(
			cone const &,
			sphere const &,
			double const,
			logical = FALSE,
			SPAbox const & = *(SPAbox *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF
		);


// In intcoto.cxx

DECL_INTR surf_surf_int *int_cone_torus(
			cone const &,
			torus const &,
			double const,
			logical = FALSE,
			SPAbox const & = *(SPAbox *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF
		);


// In intcosp.cxx

DECL_INTR surf_surf_int *int_cone_spline(
			cone const &,
			spline const &,
			double const,
			logical = FALSE,
			SPAbox const & = *(SPAbox *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF
		);

// In intshsh.cxx

DECL_INTR surf_surf_int *int_sphere_sphere(
			sphere const &,
			sphere const &,
			double const,
			SPAbox const & = *(SPAbox *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF
		);


// In intshto.cxx

DECL_INTR surf_surf_int *int_sphere_torus(
			sphere const &,
			torus const &,
			double const,
			logical = FALSE,
			SPAbox const & = *(SPAbox *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF
		);


// In intshsp.cxx

DECL_INTR surf_surf_int *int_sphere_spline(
			sphere const &,
			spline const &,
			double const,
			logical = FALSE,
			SPAbox const & = *(SPAbox *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF
		);

// In inttoto.cxx

DECL_INTR surf_surf_int *int_torus_torus(
			torus const &,
			torus const &,
			double const,
			SPAbox const & = *(SPAbox *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF
		);


// In inttosp.cxx

DECL_INTR surf_surf_int *int_torus_spline(
			torus const &,
			spline const &,
			double const,
			logical = FALSE,
			SPAbox const & = *(SPAbox *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF
		);

// In intspsp.cxx

DECL_INTR surf_surf_int *int_spline_spline(
			spline const &,
			spline const &,
			double const,
			SPAbox const & = *(SPAbox *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF,
			SPApar_box const & = *(SPApar_box *)NULL_REF
		);


#include "position.hxx"
#include "unitvec.hxx"
#include "sfsfint.hxx"

// ywoo 08Jan01: added the class
class torus;
// ywoo: end
class surface;
class cone;
class surf_surf_int;
class surf_surf_term;
class SPAbox;
class SPApar_pos;
class FACE;
class SPAtransf;
class plane;
class sphere;
class spline;

// In utilsfsf.cc, useful for all surface pairs.

DECL_INTR surf_surf_int *construct_split_points(
			surf_surf_int *,
			SPAbox const &,
			double const epsilon = SPAresabs
		);
DECL_INTR SPAunit_vector point_int_tan(
			SPAposition const &,
			surface const &,
			surface const &,
			SPApar_pos const & = *(SPApar_pos *)NULL_REF,
			SPApar_pos const & = *(SPApar_pos *)NULL_REF
		);

// Cone-cone intersection enumeration of cases. The first division
// constructs the extrema of each cone with repect to the apex of the
// other ("silhouette lines"). Each may result in zero, one or two
// silhouettes, according to whether the apex is inside, on or outside
// the double-cone of the other. We switch according to the numbers
// of silhouettes.

// Each routine is in a separate file, intcocoxy.cc where x and y are
// 0, 1 or 2 as appropriate.

DECL_INTR surf_surf_int *int_cone_cone_22(
			cone const &,
			cone const &,
			surf_surf_int *,
			surf_surf_int *,
			SPAbox const &,
			double const
		);

DECL_INTR surf_surf_int *int_cone_cone_21(
			cone const &,
			cone const &,
			surf_surf_int *,
			surf_surf_int *,
			SPAbox const &,
			double const
		);

DECL_INTR surf_surf_int *int_cone_cone_20(
			cone const &,
			cone const &,
			surf_surf_int *,
			SPAbox const &,
			double const
		);

DECL_INTR surf_surf_int *int_cone_cone_11(
			cone const &,
			cone const &,
			surf_surf_int *,
			surf_surf_int *,
			SPAbox const &,
			double const
		);

DECL_INTR surf_surf_int *int_cone_cone_10(
			cone const &,
			cone const &,
			surf_surf_int *,
			SPAbox const &,
			double const
		);

DECL_INTR surf_surf_int *int_cone_cone_00(
			cone const &,
			cone const &,
			SPAbox const &,
			double const
		);

// Utilities used by most of the cone-cone and cone-sphere routines,
// in utilcosf.cc

// Class to contain an intersection point between a cone and another
// surface during processing. It contains the point and the direction
// of the intersection curve at that point, and the point's SPAparameter
// value with respect to the base curve of one cone. The intersections
// for a given intersection curve are held as a doubly-linked list,
// open-ended for an open curve (i.e. one which goes outside the
// region of interest) and circular for a closed curve.

enum span_status {
	span_unknown,
	span_in,
	span_out
};

class DECL_INTR cone_surf_int : public ACIS_OBJECT {
public:
	cone_surf_int *next;
	SPAposition int_point;
	SPAunit_vector int_dir;
	double param;
	logical tangency;		// True if this is a tangency between the
							// generator and the surface, FALSE
							// otherwise
	span_status set_type;	// Valid/invalid status in the current
							// round of bounding.
	span_status span_type;	// Cumulative valid/invalid status
	logical always_split;	// TRUE to force the span starting here
							// to be split at least once, regardless
							// of other considerations.

	cone_surf_int(
				cone_surf_int *,
				SPAposition const &,
				SPAunit_vector const &
			);

	logical span_valid() {
		// Return TRUE if the following span is to be processed.

		return span_type == span_in;
	}
};

// Now the utility routines. They support the general cone-surface
// procedures of constructing cone generators, intersecting them with
// the other surface, and constructing lists of these intersection
// points, in order round each intersection curve, or segment of it.
// Presently they rely on there being at most two intersections of each
// generator with the surface, with the intersection curve clockwise
// around the cone axis at one and anticlockwise at the other, so that
// which point lies on which curve may be easily determined. This is
// true when the second surface is a cone or a sphere - we may modify
// things later to handle other surfaces for which this is not true.

DECL_INTR void construct_int_lists(
			cone const &,
			surface const &,
			SPAbox const &,
			cone_surf_int* volatile&,
			cone_surf_int* volatile&,
			logical,
			logical,
			double const
		);

DECL_INTR void find_valid_spans(
			cone const &,
			surface const &,
			SPAbox const &,
			cone_surf_int* volatile&,
			cone_surf_int* volatile&,
			logical,
			logical,
			double const
		);
DECL_INTR void expand_int_lists(
			cone const &,
			surface const &,
			cone_surf_int*,
			cone_surf_int*,
			logical,
			logical,
			double const
		);

DECL_INTR cone_surf_int *intersect_generator(
			cone const &,
			SPAposition const &,
			surface const &
		);

DECL_INTR surf_surf_int *interpolate_curve(
			cone_surf_int* volatile&,
			surface const &,
			surface const &,
			SPAbox const &,
			double const,
			surf_surf_term * = NULL,
			surf_surf_term * = NULL
		);
DECL_INTR void debug_int_lists(
			cone_surf_int *,
			cone_surf_int *,
			FILE * = debug_file_ptr
		);

//  STILIMITS TAM:  begin - add this routine to the cone-surf utilities
class ellipse;
DECL_INTR ellipse compute_radial_bound_ellipse(
			cone const&,
			SPAbox const&,
			double
		);
//  STILIMITS TAM:  end


// STL pcp 23-Jul-1999 declarations of some torus utilities in
// utilsfsf moved from intplto

DECL_INTR void start_torus_int_lists(
			torus const &,
			surface const &,
			SPAunit_vector const &,
			logical,
			cone_surf_int *&,
			cone_surf_int *&,
			logical &,
			logical &,
			double const
		);

DECL_INTR void expand_torus_int_lists(
			torus const &,
			surface const &,
			SPAunit_vector const &,
			logical,
			cone_surf_int *,
			cone_surf_int *,
			logical,
			logical,
			double const
		);

DECL_INTR int get_torus_curv_directions(
			SPAposition const &,
			torus const &,
			double,
			SPAunit_vector &,
			SPAunit_vector &
		);

DECL_INTR int get_torus_apex_directions(
			torus const &,
			SPAposition const &,
			SPAunit_vector const &,
			SPAunit_vector &,
			SPAunit_vector &,
			double const
		);

// Delete a list of cone_surf_ints, allowing for the fact that it may be 
// circular. The head is always set to zero on exit. 

DECL_INTR void delete_cone_surf_ints( cone_surf_int* volatile &head );

#endif
