/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Utility routines, which are used in the skin/loft sub-system.  
// The below routines should not be called by any-one unless one 
// understands the routines behaviour completely and also willing to
// take the chance of possible un-documented side-effects.
/*******************************************************************/
#if !defined( ISGSKIN_HDR_DEF )
#define   ISGSKIN_HDR_DEF
#include "dcl_skin.h"
#include "logical.h"
#include "position.hxx"
#include "vlists.hxx"
#include "skin.hxx"
#include "lists.hxx"
#include "vers.hxx"
#include "param.hxx"
class spline;
class curve;
class intcurve;
class COEDGE;
class BODY;
class FACE;
class WIRE;
class plane;
class surface;
class ellipse;
class Mcurve_data;
/*******************************************************************/

// VERSIONED BREAKUP FUNCTIONS:
// (signatures in isgskin.hxx)
DECL_SKIN void 
sg_minimize_twist_wires(int no_wires, 
						BODY* inWires[], 
						logical all_closed,
						const skin_twist_preference &skin_twist_pref, 
						logical* fixedWireList = NULL,
						logical start_degenerate_wire = FALSE,
						logical end_degenerate_wire = FALSE,
						logical no_new_twist_vertices = FALSE,
						const ENTITY_LIST &vertices_to_skip = *(ENTITY_LIST *)NULL_REF,
						BODY* inPath = NULL,
						SPAposition* inCentroids = NULL,
						double* inPathparams = NULL, 
						law** inRaillaw  =  NULL
						);
						//AcisVersion* version = NULL);
DECL_SKIN void 
sg_breakup_wires(int no_wires, 
				 BODY* inWires[],
				 logical no_twist,  
				 logical all_closed, 
				 logical start_degenerate_wire = FALSE,
				 logical end_degenerate_wire = FALSE,
				 Mcurve_data* mcurve_data = NULL,
				 BODY* inPath = NULL,
				 SPAposition* inCentroids = NULL,
				 double* inPathparams = NULL,
				 law ** inRaillaw = NULL);//,
				 //AcisVersion* version = NULL);
DECL_SKIN void
sg_make_wires_compatible(int,			// Number of sections.
						 BODY *[],		// Input sections
						 BODY *[],		// output Compatible Sections.
						 logical twist = FALSE,
						 logical align = TRUE,
						 logical combine = FALSE,
						 logical no_new_twist_vertices = FALSE);//,
						 //AcisVersion* version = NULL);


// end: versioned breakup functions
/*******************************************************************/


// Routine which checks tangent continuity between the surfaces.  Simple
// sampler, this routine is to trap obvious non-tangencies rather than
// catching every possible case of tangency.  Returns TRUE if the surfaces
// are atleast G1.
DECL_SKIN logical
sg_check_g1(
	COEDGE *,
	COEDGE *
);


// Routine which computes the knots for the sections by calculating
// the average distance between them.
DECL_SKIN void
sg_compute_avg_knots( 
		int		no_knots,
		BODY	**wires,
		double	*knots
);

// Routine which reparametrizes the given coedge to lie, from 0.0 to 1.0
// range.  Modifies the coedge and the edge to have forward sense and
// makes a new curve which follows the coedge-direction and whose range
// is 0.0 to 1.0.
DECL_SKIN void
sg_reparam_coedge(
	COEDGE *
);

// Report the number of coedges in a wire
DECL_SKIN int
sg_no_coedges_in_wire( 
	const BODY *
);

DECL_SKIN int
sg_no_coedges_in_wire( 
	const WIRE *
);

DECL_SKIN int sg_no_vertices_in_wire(
	const BODY *
);

DECL_SKIN int sg_no_vertices_in_wire(
	const WIRE *
);

// Routine which fixes the wire-to-coedge pointer for open wires.  This
// basically makes the wire to point to the start-coedge of a open-wire
DECL_SKIN void
sg_fixup_wire(
	WIRE *
);

// Split the given coedge at the given SPAposition and the given SPAparameter
// value.  The SPAparameter value supplied is with respect to curve, not
// with respect to coedge.
DECL_SKIN VERTEX*
sg_split_coedge(
		COEDGE		*coed,
		const SPAposition	&split_pos,
		double		split_par
);

// This routine checks all of the WIRE BODYs in the skin and tries to make
// a cononical surface from them.  
DECL_SKIN logical
sg_skin_simplify_planar (
	int no_wires,			// Number of sections
	BODY* in_wires[],		// Array of pointers to the wire bodies	
	BODY*& result,			// Simplified Skin body returned
	logical align           // Align dirction the cross sections.
);

// This routine checks all of the curves in the skin and tries to make
// a cononical surface from them.
DECL_SKIN logical
sg_skin_simplify (
	int no_curves,          // Number of sections
	curve * in_curve[],     // Array of pointers to the curves
	spline * spl,           // Skin spline surface
	FACE*& result,          // Simplified Skin body returned
	logical no_twist        // Remove twist option
);
				 
DECL_SKIN logical
are_curves_planar(
	int				no_curves,
    curve**			curves,
    SPAposition&       centroid,
    SPAunit_vector&    normal
    );

DECL_SKIN logical 
sg_are_all_wires_periodic(int no_wires, BODY **wires);

DECL_SKIN logical 
is_periodic( curve const &curv );

// This routine aligns closed wires. This is done by reversing the wires.
DECL_SKIN logical
sg_skin_align_closed_wires (int no_wires,
							WIRE ** wires,
							SPAposition* centroids,
							SPAunit_vector*& normals,
							logical align_first_wire
);

// This routine aligns open wires. This is done by reversing the wires.
DECL_SKIN logical
sg_skin_align_open_wires (  int no_wires, 
							WIRE **wires,
							SPAposition* centroids,
							SPAunit_vector*& normals,
							logical align_first_wire
);

DECL_SKIN logical sg_align_wires_from_faces(int no_of_wires, 
						          BODY** in_wires);
/*
// This routine aligns closed wires. This is done by reversing the wires.
DECL_SKIN logical
sg_skin_align_closed_wires (
	int no_wires,
	WIRE ** wires,
	logical align_first_wire = FALSE
);

// This routine aligns open wires. This is done by reversing the wires.
DECL_SKIN logical
sg_skin_align_open_wires ( 
	int no_wires, 
	WIRE ** wires,
	logical start_degenerate = FALSE,
	logical align_first_wire = FALSE
);
*/

DECL_SKIN logical
get_exact_centroid(BODY* wire_body, 
				   SPAposition &centroid, 
				   SPAunit_vector &wire_normal);

DECL_SKIN logical
get_exact_centroid(WIRE *wire, 
				   SPAposition &centroid, 
				   SPAunit_vector &wire_normal,
               const double   iModelTolerance = SPAresabs);

// This routine finds the start and end points of a wire.
void sg_find_start_and_end_vertices_of_wire
  ( 
      WIRE const  *iWire, 
      VERTEX      *&oStartVertex, 
      VERTEX      *&oEndVertex
   );

// This routine finds the start and end points of a wire.
DECL_SKIN void 
sg_find_start_and_end_of_wire ( WIRE const *in_wire, 
						SPAvector &start, 
						SPAvector &end );

// This routine finds the start and end points of a wire.
DECL_SKIN void 
sg_find_start_and_end_of_wire ( WIRE const *in_wire, 
						SPAposition &start, 
						SPAposition &end );

// This routine checks to see if a list of wires are all closed.
DECL_SKIN logical 
sg_are_all_wires_closed ( int no_wires, WIRE **wires );

// This routine checks to see if a list of wires are all closed.
DECL_SKIN logical 
sg_are_all_wires_closed ( int no_wires, BODY **wires );

// This routine checks to see if the wire is a single point.
DECL_SKIN logical 
sg_degenerate_wire ( BODY *body_wire );

DECL_SKIN logical 
sg_degenerate_wire ( WIRE *wire );

// Construct a single face from a given plane and two WIRE's and two curves.
//  This will handle skin surfaces with singularities in both
// u and v directions.
DECL_SKIN FACE *
sg_make_face_plane(
		plane const 	&this_plane,
		WIRE const		&lo_wire,
		WIRE const		&hi_wire,
		curve const		&lt_cur,
		curve const		&rt_cur,
		logical			start_u_singular,
		logical 		end_u_singular,
		logical			start_v_singular,
		logical 		end_v_singular
);

// Construct a single face from a given plane and two WIRE's and two curves.
//  This will handle skin surfaces with singularities in both
// u and v directions.
DECL_SKIN FACE *
sg_make_face_plane(
		plane const     &this_plane,
		curve const     &lo_wire,
		curve const     &hi_wire,
		curve const     &lt_cur,
		curve const     &rt_cur,
		logical         start_u_singular,
		logical         end_u_singular,
		logical         start_v_singular,
		logical         end_v_singular
);

// Create curves from an array of WIREs.
DECL_SKIN int sg_create_curves_from_wires( int no_wires, 
						WIRE **wires, 
						curve ***curves );

// This routine will create an array of curves from a WIRE
// It will NOT handle degenerate (vertex) WIREs
DECL_SKIN int sg_create_curves_from_wire( WIRE *wire, curve ***curves );

// Check to see if we have a invalid surface.
// If the u and the v vectors on the surface are in the
// same or oppostite direction then we have a problem.
//DECL_SKIN logical check_for_bad_surface_u_v ( 
//	surface *surf
//);

// Routine which checks for corners at the start and end of the
// wires coedges.
DECL_SKIN logical
sg_wire_has_corners( 
		WIRE	*wire
);

DECL_SKIN logical
sg_wire_has_corners( 
		BODY	*wire_body
);

// Find all of the intersection points of the path with the profiles.
DECL_SKIN void
sg_find_profile_points(
	int no_of_wires,
	BODY **in_wires,
	WIRE *wire_path,
	SPAposition **points,
	SPAvector **directions,
	SPAunit_vector **normals
);

// Check to see if an array of doubles is cyclic and
// in ascending or descending order.
DECL_SKIN logical
sg_is_cyclic ( 
	int no_params, 
	double *params, 
	logical &up 
);

// Get the generating curves of a skin/loft surface as wires.
DECL_SKIN void 
get_curves_as_wire(
		FACE *face,
		int &no_wires,
		BODY ***wires
	);
DECL_SKIN void
get_net_curves_as_wires(
		FACE *face,
		int &no_wires_u,
		int &no_wires_v,
		BODY** &wires_u,
		BODY** &wires_v	);

//STI rr 05/18/99
// Get the generating laws of a skin/loft surface
DECL_SKIN logical
get_laws_from_skin_face(
		FACE* face, 
		int &no_laws, 
		law ** &laws
		);
//STI rr: end

void sg_make_laws_from_coedsets(
		int no, 
		Loft_Connected_Coedge_List *coed_list, 
		int &number_of_laws, 
		law ***laws,
        int inc_use_count = TRUE);

// Routine which checks the G1 continuity across the junction.
DECL_SKIN logical
is_coedge_tangent(
		COEDGE 	*coedge
);


// Check a loft surface for a small radius of curvature at the start and end
DECL_SKIN int check_for_bad_radius_of_curv ( 
	surface const& surf, double min_radius_tol, double rc[2]
);

// STI rr: constructs a solid body from an input sheet body constructed 
// by the lofting and skinning routines by either endcapping or by closing
// of the sheet body at the start and  end wires.
DECL_SKIN logical
sg_create_solid_loft(BODY **wires, 
					 FACE *first_face, 
					 int no_of_wires, 
					 logical closed, 
					 logical degenerate_faces_flag = FALSE, 
					 ENTITY_LIST* degenerate_face_list = NULL);

logical find_face_coedge(COEDGE* wire_coed,BODY* main_body,COEDGE*& face_coed);

//STI rr 05/24/99 r6171: we need a routine that will orient the 
// skin faces correctly. There have been problems due to the 
// non--unique manner in which the face normals of the skin faces 
// are returned by the skin code. This routine orients the face
// normals correctly for all cases -- end capping, and when sheet is
// made.
logical
sg_orient_skin_faces(BODY **wires, 
										 FACE *loft_face, 
										 int no_of_wires, 
										 int closed, 
										 FACE * &first_face, 
										 FACE * &last_face,										 
										 logical degenerate_faces_flag = FALSE, 
										 ENTITY_LIST* degenerate_face_list = NULL);
logical
sg_orient_skin_faces(BODY **wires, 
										 FACE *loft_face, 
										 int no_of_wires, 
										 int closed,										 
										 logical degenerate_faces_flag = FALSE, 
										 ENTITY_LIST* degenerate_face_list = NULL);

// STI rr: This routine is intentionally not DECL_SPEC.
int 
double_compare( const void *arg1, const void *arg2 );

// STI gto: returns true if the all coedges are identical
DECL_SKIN logical
is_coed_lists_equal(Loft_Connected_Coedge_List& coed_list1, 
					Loft_Connected_Coedge_List& coed_list2
					);

DECL_SKIN logical
sg_change_start_vertex_in_wire ( BODY *body, VERTEX *start );

void 
sg_copy_last_wire_from_first(BODY * wires[],
							 Loft_Connected_Coedge_List* coed_sets,
							 int no_sets
							 );

DECL_SKIN void sg_delete_coedge_list_contents(Loft_Connected_Coedge_List &coed_sets);

DECL_SKIN outcome modify_wire(COEDGE* coedge1, 
							  COEDGE* coedge2, 
							  const SPAposition &adjusted_point, 
							  WIRE* wire,
							  logical update_merge_attrib = FALSE);

DECL_SKIN outcome remove_vertex_wires(int, BODY**, WIRE*, const SPAposition&);
DECL_SKIN outcome remove_vertex_wires(int, BODY**, int);
DECL_SKIN outcome remove_vertex_wire(BODY* wire_body, int index);
DECL_SKIN outcome collapse_degenerate_wires(int, BODY**);
DECL_SKIN outcome collapse_degenerate_wires(ENTITY_LIST&);
DECL_SKIN logical sg_add_vertex_coedge(COEDGE* coed);

struct sg_stitchingInfoSt
{
	logical degenerate_face_flag;
	FACE* degenerate_face;
	logical* start_touch;
	logical* end_touch;
	logical* bottom_touch;
	logical* top_touch;
	sg_stitchingInfoSt(logical df, FACE* face, logical* st, logical* et, logical* bt, logical* tt)
	{
		degenerate_face_flag = df;
		degenerate_face = face;
		start_touch = st;
		end_touch = et;
		bottom_touch = bt;
		top_touch = tt;
	};
};

/* - old interface
void
sg_make_lofting_faces(
	  int no_sets,				                // Number of sets
	  Loft_Connected_Coedge_List* coed_sets,	// Set of lists of coedges.
	  ENTITY_LIST& face_list,					// List of returned faces
	  BODY* path,								// Path
	  BODY** breakup_wires,						// wires corresponding to the coedge_lists after break-up
	  BODY** original_wires,					// wires corresponding to the coedge_lists before break-up
	  int &number_of_laws,						// total number of laws for all the wires
	  law**&	laws,							// law list associated with the wires
	  logical arc_length_option,				// Arc length parameter option for the v direction
	  logical twist_option,						// If the co-edges are all closed minimize the surface twist.
	  logical perpendicular_option,				// Calculate the direction of the loft perpendicular to the coedges.
	  logical simplify_option,					// Simplify the surface if no surfaces.
	  logical closed,							// Construct a closed/open body for TRUE/FALSE	
	  logical allow_same_uv=FALSE,				// Allow surfaces with the same u & v direction
	  logical arcLengthInU=FALSE,			    // Arc-length paramertizes all input curves in the u direction
	  sg_stitchingInfoSt& stitchInfo = *(sg_stitchingInfoSt*)NULL_REF, // Stitching info
	  logical draft_normal_path = FALSE,        // Flag to state if we are doing draft, normal, or path
      logical use_loft_law = FALSE,
	  logical self_int_test = TRUE);
*/

void
sg_make_lofting_faces(
	  int no_sets,				                // Number of sets
	  Loft_Connected_Coedge_List* coed_sets,	// Set of lists of coedges.
	  ENTITY_LIST& face_list,					// List of returned faces
	  BODY* path,								// Path
	  BODY** breakup_wires,						// wires corresponding to the coedge_lists after break-up
	  BODY** original_wires,					// wires corresponding to the coedge_lists before break-up
	  int &number_of_laws,						// total number of laws for all the wires
	  law**&	laws,							// law list associated with the wires
	  const skin_options &opts,					// skin_options
	  sg_stitchingInfoSt& stitchInfo = *(sg_stitchingInfoSt*)NULL_REF, // Stitching info
	  logical draft_normal_path = FALSE,        // Flag to state if we are doing draft, normal, or path
      logical use_loft_law = FALSE,
	  logical self_int_test = TRUE);

void
sg_make_lofting_surfaces(
	  int no_sets,						        // Number of sets							 
	  Loft_Connected_Coedge_List* coed_sets,	// Set of lists of coedges.	 
	  VOID_LIST& surface_list,					// List of returned faces				 
	  BODY* path,								// Path						 
	  BODY** breakup_wires,						// wires corresponding to the coedge_lists after break-up				 
	  BODY** original_wires,					// wires corresponding to the coedge_lists before break-up					 
	  int number_of_laws,						// total number of laws for all the wires					 
	  law** laws,								// law list associated with the wires							 
	  logical arc_length_option,				// Arc length SPAparameter option					 
	  logical twist_option,						// If the co-edges are all closed minimize the surface twist.				 
	  logical perpendicular_option,				// Calculate the direction of the loft perpendicular to the coedges.		 
	  logical closed,							// Construct a closed/open body for TRUE/FALSE	
	  logical arcLengthInU=FALSE,			    // Arc-length paramertizes all input curves in the u direction
	  logical draft_normal_path = FALSE);		// Flag to state if we are doing draft, normal, or path	
				 
BODY* sg_stitch_lofting_faces(int no_sets,		// Number of sections
	  ENTITY_LIST& face_list,					// List of returned faces 
	  BODY** breakup_wires,						// wires corresponding to the coedge_lists after break-up
	  logical closed,
	  logical solid,
	  sg_stitchingInfoSt& stitchInfo,
	  skin_gap_type gap_type);

logical make_skin_surface(int no_of_bodies, 
					int surface_number, 
					BODY** bodies, 
					spline*& result);

DECL_SKIN logical is_extreme_point(WIRE* wire, 
					VERTEX* vertex, 
					SPAunit_vector &extreme_dir,
                    SPAposition& centroid = *(SPAposition*)NULL_REF);

// Internal data type; do not doc ..... 
enum loft_surface_type {
	LOFT_USER_LAWS,
	LOFT_SURFACES,
    LOFT_NO_CONSTRAINT
};

loft_surface_type is_loft(int no_sets, 
					      BODY **wires, 
					      Loft_Connected_Coedge_List *coed_sets,
					      logical *oAllLoftFacesTwoSided = NULL);

// STI rr: 03/16/1999 This function is used to get the lateral edges of the skin face after the 
// stitch operation is completed. It assumes the user has taken care to put attributes on the
// lateral edges. DECL_SKIN has been intentionally left out for the following two routines
void
sg_get_lateral_coedges( COEDGE *c1, // First coedge
					   logical sense, // Sense of traversal of the coedges
					   ENTITY_LIST &coedge_list); // list of lateral coedges

logical 
is_wires_closed(int no_of_wires, 
				BODY **wires
				);

// not used
void 
make_degenerate_wire_law(logical start,
						 BODY *in_wire,
						 BODY*& out_wire,
						 law *inlaw
						 );

void make_degenerate_wire_law(logical start,
								BODY *wire,
								law *inlaw
								 );

void 
make_new_wire_from_degenerate_wire (BODY *wire,
									SPAposition const& original_start_position,
									law * &inlaw
									);
void 
make_new_wire_from_degenerate_wire (BODY *wire,
									SPAposition const& original_start_position
									);


DECL_SKIN void sg_copy_wires_attribs(int no_wires, 
							         BODY* in_wires[], 
							         BODY* out_wires[], 
							         logical combine, 
							         logical& all_closed = *(logical*)NULL_REF, 
							         logical& degenerate_wire = *(logical*)NULL_REF);

DECL_SKIN void sg_copy_wires(int no_wires, 
							 BODY* in_wires[], 
							 BODY* out_wires[], 
							 logical combine, 
							 logical& all_closed = *(logical*)NULL_REF, 
							 logical& degenerate_wire = *(logical*)NULL_REF);

DECL_SKIN void sg_copy_wires(int no_wires, 
							 const ENTITY_LIST &in_wires, 
							 BODY* out_wires[], 
							 logical combine, 
							 logical& all_closed = *(logical*)NULL_REF, 
							 logical& degenerate_wire = *(logical*)NULL_REF);

DECL_SKIN logical sg_align_wires(   int no_of_wires, 
									BODY** in_wires, 
									logical all_closed, 
									logical align_first_wire = FALSE,
									SPAposition* centroids = NULL,
									SPAunit_vector* normals = NULL,
									int no_of_laws = 0,
									law** laws = NULL, 
									BODY * inPath = NULL,
									double* params = NULL
					            );

DECL_SKIN logical sg_align_wires(   int no_of_wires, 
									WIRE** in_wires, 
									logical all_closed, 
									logical align_first_wire = FALSE,
									SPAposition* centroids = NULL,
									SPAunit_vector* normals = NULL,
									int no_of_laws = 0,
									law** laws = NULL,
									WIRE * inPath = NULL,
									double* params = NULL	
								);

/*					  
DECL_SKIN logical sg_align_wires(int no_wires, 
					BODY* inWires[], 
					logical allClosed, 
					logical degenerateWire, // One may be able to take this one out. It prevents alignment in the case of closed wires.
					logical start_degenerate_law = FALSE,
					logical align_first_wire = FALSE);
*/
/*
DECL_SKIN void sg_align_lofting_wires(int no_wires, 
									BODY* inWires[], 
									int number_of_laws, 
									law** laws, 
									logical allClosed, 
									logical degenerateWire);
									*/

DECL_SKIN void sg_reverse_law(law *& law);

DECL_SKIN void sg_reverse_laws(int no_of_sections,Loft_Connected_Coedge_List*& sections,BODY** wires);

DECL_SKIN void sg_breakup_lofting_wires(int no_wires, 
							  Loft_Connected_Coedge_List *coed_sets,
							  BODY* inWires[], 
							  int number_of_laws, 
							  law** laws, 
							  logical no_twist, 
							  logical all_closed, 
							  logical degenerate_wire,
							  logical noBreakup = FALSE,
							  logical* fixedWireList = NULL,
							  Mcurve_data* mcurve_data = NULL);

DECL_SKIN logical sg_make_skinning_wires(int no_of_incoming_wires,	 
										 BODY* incoming_wires[],		 	
										 int& no_of_outgoing_wires,	 
										 BODY**& wires,					 
										 logical& closed,				 
										 logical& all_closed,			 
										 logical& degenerate_wire,
										 logical& is_periodic,
										 logical sweep_path = FALSE
                                         );	

DECL_SKIN logical sg_make_skinning_with_path_coedges(int no_of_wires, 
													 BODY** temp_wires,
													 BODY* path_copy,  
													 Loft_Connected_Coedge_List*&  sect_list,
													 logical make_path_law = TRUE,
													 double *knots = NULL
													 );

DECL_SKIN logical sg_make_skinning_with_normal_conditions(int no_of_wires, 
														  BODY** temp_wires,
														  skinning_normals in_normals,
														  Loft_Connected_Coedge_List*& sect_list);

logical sg_make_skinning_with_vectors(int no_of_wires,
									  BODY** temp_wires, 
									  SPAvector* in_vectors,
									  double* magnitudes,
									  Loft_Connected_Coedge_List*& sect_list);

DECL_SKIN logical sg_make_skinning_with_draft_angles(int no_of_wires, 
													 BODY** org_wires, 
													 BODY** temp_wires,
													 SPAposition* centroids,
													 SPAunit_vector* normals,
													 double draft_start, 
													 double draft_end, 
													 double draft_start_mag, 
													 double draft_end_mag, 
													 Loft_Connected_Coedge_List*& sect_list);

DECL_SKIN law** sg_generate_draft_laws(BODY* body, 
									   logical reverse, 
									   double draft, 
									   const SPAvector &in_normal);

law** sg_generate_draft_laws(BODY* body, double draft);
law** sg_generate_draft_laws(Loft_Connected_Coedge_List* clist, double draft);
law** sg_generate_loft_laws(BODY* body, logical orient);
law** sg_generate_loft_laws(int no_of_wires, BODY** wires, int curve_count, curve** cur_arr, surface** surf_arr, double* tan_factors, logical perpendicular);

void sg_copy_coedge_list_contents(Loft_Connected_Coedge_List &in_coed_list, Loft_Connected_Coedge_List &out_coed_list); 

void sg_order_coedges(int no, COEDGE **coed_list, const skin_options *skin_opts = NULL);

logical rotate_paramerisation (const WIRE *wire, double t);

logical reorder_coedges_in_wire ( WIRE *wire, int coedge_index );

void make_wires_from_sections(Loft_Connected_Coedge_List *coed_sets, BODY ** bodies);

logical* calculateG1Status(WIRE *wire, SPAunit_vector *&vert_dirs = *(SPAunit_vector **)NULL_REF,
						   double dist_tol = -1.0, double direc_tol = -1.0);

DECL_SKIN outcome
sg_merge_coedges(
            BODY **wires,
			int no_wires,
			Loft_Connected_Coedge_List *coed_sets,
			law** laws,
			logical* simplified = NULL,
            logical iHandleAllCurves = FALSE,
			const skin_options* skin_opts = NULL
            );

DECL_SKIN logical 
sg_smooth_skin_wires (
                BODY **ioWires, 
                const int iNumWires, 
                const double iSmoothTolerance,
                double & oTol = *( double * )NULL_REF
                );

/*
DECL_SKIN logical sg_simplify_skin_wires( BODY **wires,
									   int no_wires,
									   Loft_Connected_Coedge_List *coed_sets,
									   int no_laws,
									   law** laws,
									   logical start_degenerate_law,
									   logical end_degenerate_law,
									   logical handleAllCurves = FALSE,
									   logical draft_normal_path = FALSE);
									   */
DECL_SKIN logical sg_simplify_skin_wires( BODY **wires,
									   int no_wires,
									   Loft_Connected_Coedge_List *coed_sets,
									   law** laws,
									   logical handleAllCurves = FALSE,
									   logical draft_normal_path = FALSE,
									   logical* simplified = NULL,
									   const skin_options *skin_opts = NULL);

DECL_SKIN void sg_map_coedge_to_wire( BODY** wires,
									  int j,
									  Loft_Connected_Coedge_List *coed_sets,
									  COEDGE* this_coed,
									  law** laws,
									  curve* &cur_arr,
									  surface* &surf_arr,
									  double &tan_factors, 
									  law* &law_arr,
									  logical replace_procedural_curve,
									  logical arcLengthInU,
									  logical draft_normal_path = FALSE,
									  const skin_options *skin_opts = NULL);

law * flip_draft_law(WIRE* wire,logical wire_was_reversed,law *draft_law,double angle);

void remove_zero_edges( BODY* wire_body,FACE* face );

logical remove_zero_edge(COEDGE* coedge,WIRE* wire);

logical skin_get_plane(BODY* wire_body,
					   SPAposition &centroid, 
					   SPAunit_vector   &normal,
					   logical  same_plane = FALSE,                // only get plane if all wire points are in the same plane
					   logical  apply_transf = TRUE,
					   double   &deviation = *(double*)NULL_REF,
					   logical  &zero_area = *(logical*)NULL_REF);

logical skin_get_plane(WIRE* wire,
					   SPAposition &centroid, 
					   SPAunit_vector   &normal,
					   logical  same_plane = FALSE,                // only get plane if all wire points are in the same plane
					   logical  apply_transf = TRUE,
					   double   &deviation = *(double*)NULL_REF,
					   logical  &zero_area = *(logical*)NULL_REF,
                  const double   iMaxTolerance = SPAresabs);

logical is_line(BODY* wire_body);

logical is_line(WIRE* wire);

logical is_in_periodic_range(double t,COEDGE* coedge);

FACE *make_Plane_face(spline *spl, int no_curves, curve **curves, plane *this_plane);

FACE *make_Open_Cylinder_Cone(int no_wires,ellipse **curves);

FACE *make_Cylinder_Cone( int no_wires, ellipse **curves );

logical all_curves_ellipses( int no_curves, curve **curves );
logical all_curves_straight( int no_curves, const curve **curves );

// For a given vertex pair this contains the values that 
// we use to compare the match values of two vertex pairs:
struct match_measurements
{
	logical matched; // TRUE if this pair is matched
	logical illegal; // TRUE if this match causes bad geometry
	double m;        // mean direction measurement
	double e;        // extreme direction measurement
	double d;        // distance between the two vertices
	logical both_coc;  // both concave or convex
    int i_index;
    int j_index;
};


struct index_range
{
	int start;
	int end;
};

struct vertex_match
{
	int i;
	int j;
};

logical remove_vertex_from_wire(BODY* wire_body,SPAposition*& its_position);

outcome remove_vertex_from_wire(BODY* wire_body, int index);

outcome remove_cvty_pts_from_stitched_lateral_edge(ENTITY_LIST &coedges,logical* remove_vertex);

void make_new_start_vertex(WIRE* wire,VERTEX* start);

double sg_coedge_length(COEDGE *coed);

void sg_swap_double(double	*arr,int i,int j);

void sg_qsort(double *arr,int left,int right);

void sg_array_sort(double *arr,int size);

COEDGE *get_coedge(WIRE *wire,int coedge_index);

void get_wire_vertices_in_order(BODY *body, ENTITY_LIST &vert_list);

void get_wire_vertices_in_order(WIRE *wire, ENTITY_LIST &vert_list);

void skin_notify_merge_attrib_of_alignment(WIRE* wire);

logical check_point_intersection(SPAposition const& pos, 
								 BODY*         wire_body, 
								 int&          coedge_index = *(int*)NULL_REF, 
								 SPAparameter& par = *(SPAparameter*)NULL_REF,
								 double        guide_tol = SPAresfit);

logical check_point_intersection(SPAposition const& pos, 
								 WIRE*         wire, 
								 int&          coedge_index = *(int*)NULL_REF, 
								 SPAparameter& par = *(SPAparameter*)NULL_REF,
								 double        guide_tol = SPAresfit);

logical find_skin_input_max_tolerance(ENTITY *ent, double &max_error);
logical find_skin_input_max_tolerance(int num_ents, ENTITY **ents, double &max_error);
logical find_skin_input_max_tolerance(int num_1_ents_ents, int *num_2_ents_ents, 
									  ENTITY ***ents_ents, double &max_error);
logical find_skin_input_max_tolerance(ENTITY_LIST& elist, double &max_error);
logical find_skin_input_max_tolerance(int num_lcc, Loft_Connected_Coedge_List *lcclist, double &max_error);

logical find_obsolete_convexity_points(ENTITY_LIST &coedges,logical* skip_cvty_point=NULL);


logical is_same_wirebody(WIRE* wire1, WIRE* wire2);

logical isCircleOrStraight(curve const &this_cur);

logical sl_make_partners(COEDGE* c1, COEDGE* c2, BODY *b, logical = TRUE);

double skin_utl_get_vertices_tolerance
   (
      const VERTEX *iVertex1,
      const VERTEX *iVertex2 = NULL
   );
double skin_utl_get_edges_tolerance
   (
      const EDGE *iEdge1,
      const EDGE *iEdge2 = NULL
   );


//This operator assumes the input face is four sided.
//coedge on a particular side may not exist due to 
//1) The side is degeneated.
//2) The side is on a seam and the face is formed without seam.
class skin_face_border_extractor : public ACIS_OBJECT 
{
public:
   enum
   {
     eSkinFaceSideUnknown = 0,
     eSkinFaceSideBottom,
     eSkinFaceSideRight,
     eSkinFaceSideTop,
     eSkinFaceSideLeft
   };
   
   skin_face_border_extractor ();
   skin_face_border_extractor (FACE *iFace);
   ~skin_face_border_extractor ();

   void set_face (FACE *iFace);

   COEDGE *get_coedge (const int iWhichSide);
   curve  *get_curve  (const int iWhichSide);
   void get_coedges 
   (
      COEDGE *&oBottomCoedge,
      COEDGE *&oRightCoedge,
      COEDGE *&oTopCoedge,
      COEDGE *&oLeftCoedge
   );
   int edge_on_which_border (const EDGE *iEdge, const REVBIT iCoedgeSense);

private:
   SPAposition    m_corner_pos[4];
   COEDGE         *m_coedges[4];
   const surface  *m_surface;
   logical        m_start_u_singular;
   logical        m_end_u_singular;
   logical        m_start_v_singular;
   logical        m_end_v_singular;
   logical        m_u_closed;
   logical        m_v_closed;
   logical        m_face_degenerate;

   void Init ();
   void process_loop (LOOP *iLoop);
   void compute_corners (const surface &iSurface);
   int num_coedges (const LOOP *iLoop) const;
   int edge_on_which_border_degenerate (const EDGE *iEdge, const REVBIT iCoedgeSense);
   curve *get_seam_curve (const int iWhichSide, SPAinterval &oCurveRange);
};

#endif
