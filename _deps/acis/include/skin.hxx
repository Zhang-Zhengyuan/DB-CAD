/* ORIGINAL: acis2.1/sg_husk/skin/skin.hxx */
// $Id: skin.hxx,v 1.50 2002/08/09 17:28:57 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


#ifndef		SKIN_HEADER_FILE
#define		SKIN_HEADER_FILE

#include "dcl_skin.h"
#include "tophdr.hxx"
#include "interval.hxx"
#include "skinapi.hxx"
#include "skin_opts.hxx"

class law;
class outcome;
class SPAposition;
class curve;

/**
 * @file skin.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *
 * \addtogroup SKINLOFTAPI 
 * @{
 **/

/**
 * Loft input structure.
 * Each of these structure represents a section to be lofted. A section 
 * is described by a set of linearly connected coedges. A section may
 * be either open or closed, but not branched. The ending point of any coedge 
 * in the list should be the starting point of the next coedge in the list, 
 * first and last coedges excepted. In other words, the coedges are ordered in the list 
 * as they appear next to each other in model space. There cannot be any gaps 
 * between adjacent coedges. 
 * <br><br> 
 * If the coedges in every given section are tangential to each other, then the lofted surfaces 
 * in the resultant body will also be tangential to each other along the lateral (side) edges. 
 * This is however not true in certain special situations. Refer to the Technical Article 
 * <i>Lateral Edge Tangency in Skinning and Lofting</i> for more details. 
 * <br><br>
 * The information stored in this structure is used to create a single b-spline
 * curve-ribbon, i.e., one with tangency associated, if tangency information exists. 
 * Tangency information is (by default) obtained from the surface definition of the 
 * face associated with the coedge, if one exists.
 * <br><br>
 * The curve definition is generally extracted from the underlying edges of
 * the listed coedges. However, if the coedge has a face with a spline definition 
 * and the pcurve happens to be the boundary, then this information is utilized 
 * for extraction of the curve and tangency. If a law is supplied for a given
 * coedge in the section list, then the %law provides the tangency information of the skinned 
 * %surface, superseding any existing %surface tangency information. 
 * <br><br>
 * The orientation of the first coedge is imposed on the curve-ribbon 
 * resulting from merging the curves extracted from the coedges. The sense of 
 * the ribbon may be negated using <tt>coedge_orient</tt> field.
 * When the coedges in the list are associated with a %surface, the cross section orientation uses 
 * the <tt>REVBIT</tt> to set the tangent direction "out of" or "into" the starting %surface. 
 * The expression "out of" means that the lofted %surface is going out of the given ending %surface, 
 * while "into" means that the lofted %surface is going into the ending %surface. 
 * When lofting between two %surfaces, the starting %surface needs to have its <tt>REVBIT</tt> set to 0 
 * (for "out of the surface", the default) and the ending %surface needs to have its <tt>REVBIT</tt> 
 * set to 1 (for "into the surface"). When lofting between multiple surfaces, the first %surface 
 * has its <tt>REVBIT</tt> set for 0, while all others are set for 1. When lofting between a %surface and 
 * an edge, the <tt>REVBIT</tt> for both can be 0.
 * <br><br>
 * The default tangent direction associated with a face is away from the face. 
 * This default corresponds to a value of 1.0 for <tt>cross_tg_attr</tt>. The effect can also 
 * be scaled using <tt>cross_tg_attr</tt>. If a reversal of the tangent direction is desired, 
 * then a negative value should be specified for the field. A magnitude of 1.0 typically
 * gives good curvature. A value of 0.0 implies suppression of tangency. 
 * A negative value results in its reversal. Small values (less than 1.0) mean 
 * that the transition from the tangent to the lofted %surface happen abruptly. Large values 
 * mean that the transition from the tangent to the lofted %surface happen more gradually. 
 * Extremely high weight values could result in excessive whipping in the lofted %surface, 
 * or perhaps a self-intersecting %surface.
 * <br><br>
 * Argument, <tt>law_list</tt>, is an array of law pointers. Each coedge in the coedge 
 * list can have a %law as a constraint which describes the tangency of the edge. If a %law isn't 
 * used to control the tangency, the pointer should be <tt>NULL</tt>. 
 * <br><br>
 * Last two argumnets <tt>knot_value</tt> and <tt>knot_value_set</tt> have been introduced in R28.
 * These will be active from R28.
 * @param n_list
 * <tt>int</tt>, number of coedges in the profile
 * @param coedge_list
 * <tt>COEDGE**</tt>, ordered array of connected coedges forming the profile
 * @param coedge_orient
 * <tt>REVBIT</tt>, profile is reversed with respect to the direction of the first coedge 
 * @param cross_tg_attr
 * <tt>double</tt>, take-off vector scaling factor
 * @param law_list
 * <tt>law**</tt>, ordered array of laws defining the take-off vector associated with each coedge
 * @param knot_value
 * <tt>double</tt>, parmeter value of the intersection point of the path and the plane containing coedge_set.
 * @param knot_value_set
 * <tt>logical</tt>, Flag for knot value. If flag vlaue is true then knot vlaue has been calculated.
 */

struct Loft_Connected_Coedge_List : public ACIS_OBJECT
{
	int					n_list;
	COEDGE 				**coedge_list;
	REVBIT 				coedge_orient;
	double				cross_tg_attr;
	law					**law_list;
	double				knot_value;
	logical				knot_value_set;

	Loft_Connected_Coedge_List() 
		: knot_value_set( FALSE ) 
	{}

};
/** @} */

// New Code which make use of the skin-surface type for C1-Continuous
// skinning.

// Routine which does the actual skinning.
// Return from the function is TRUE, if the operation is successfull, other
// wise it will be FALSE.
//
// Return skin body in the variable result, the result will be NULL if the
// operation is unsuccessfull.
//
// The skin takes the sections in the form of wires, the number of
// sections is specified by the argument no_of_wires.
//
// The skin algorithm works as per the following flow chart:
//
// STEP1:
// -----
// If the sections do not have equal number of coedges, the skin algorithm
// splits the coedges as per the arc-length.
//
// STEP2:
// -----
// Than the corresponding coedges of the sections are used to construct the
// skin surface.
//
// STEP3:
// -----
// Construct the neccessary topology and the pcurves and join the individual
// pieces to form a connected sheet body.
//

DECL_SKIN outcome
sg_skin_wires(
		ENTITY_LIST	wireList,           // List of pointers to the wire bodies		
		BODY*& body,					// Skin body returned
		skin_options* opts);

DECL_SKIN outcome
sg_skin_wires_path(
	ENTITY_LIST wireList,			// List of pointers to the wire bodies	
	BODY*& body,			        // Skin body returned
	BODY* path,						// Path
	skin_options* opts);

DECL_SKIN outcome
sg_skin_wires_ruled(
	ENTITY_LIST wireList,			// List of pointers to the wire bodies		
	BODY*& body,					// Skin body returned
	skin_options* opts);

DECL_SKIN outcome
sg_skin_wires_normal(
	ENTITY_LIST wireList,					// List of pointers to the wire bodies	
	BODY*&				result,				// Skin body returned
	skinning_normals	in_normals,			// Wire tangent conditions
	skin_options* opts);	


DECL_SKIN outcome
sg_skin_wires_vectors(
	ENTITY_LIST wireList,					// List of pointers to the wire bodies	
	BODY*&				result,				// Skin body returned
	SPAvector*             vectors,			// Skining vectors
	int                 num_vectors,        // number of provided vectors
	double*             magnitudes,         // Magnitudes
	int                 num_magnitudes,     // number of provided magnitudes
	skin_options* opts);

DECL_SKIN outcome
sg_skin_wires_vectors_guides(
	ENTITY_LIST wireList,					// List of pointers to the wire bodies	
	BODY*&				result,				// Skin body returned
	SPAvector*             vectors,			// Skining vectors
	int                 num_vectors,        // Number of provided vectors
	EDGE**              guides,             // Guides
	int                 num_of_guides,      // Number of guides
	skin_options* opts);		

DECL_SKIN outcome
sg_skin_wires_draft_angle(
    ENTITY_LIST wireList,					    // List of pointers to the wire bodies	
	BODY*&,										// Skin body returned
	double,										// Draft angle on the starting wire profile
	double,										// Draft angle on the ending wire profile
	double,										// Magnitude of the starting draft angle
	double,										// Magnitude of the ending draft angle
	const SPAvector &start_normal = SPAvector(0.0,0.0,0.0),  // Plane normal of the first profile
	const SPAvector &end_normal = SPAvector(0.0,0.0,0.0),    // Plane normal of the last profile
	skin_options* opts = NULL);

outcome
sg_skin_wires_draft_angle(
		ENTITY_LIST wireList,			    // List of wire bodies	
		BODY*&		body,					// Skin body returned
		double		draft_start,			// Draft angle on the starting wire profile
		double		draft_end,				// Draft angle on the ending wire profile
		int         num_of_guides,          // Number of guides
        EDGE*       guides[],               // Guides
		skin_options* opts);                // Skinning options

DECL_SKIN logical 
sg_skin_wires_tree(
	int					num_wires,			// Number of sections in the trunk
	BODY*				wires[],			// Wire bodies that maket he trunk
	int					num_branches,		// Number of branches
	int*				count_list,			// List of wire count in each of the branches
	BODY***				branches,			// List of branches
	BODY*&				sheet_body,			// output
	skinning_normals	normals,			// Skinning normal condition
	skin_options* opts);


DECL_SKIN outcome 
sg_skin_wires_guide_curves(
		ENTITY_LIST  wireList,			    // List of pointers to the wire bodies	 
		int			 no_of_guides,			// Number of guides
		EDGE*		 guides[],				// Array of pointers to the guide wires
		BODY*&		 result,				// Skin body returned
		skin_options* opts);

// New Code which make use of the skin/loft-surface type for C1-Continuous
// Lofting.

// Routine which does the actual Lofting.
// Return from the function is  pointer to a BODY, this will be non-NULL in
// the case when loft succeeds.  Otherwise it will be NULL.
//
// The loft takes the input in the from of the link-list of structures,
// Loft_Connected_coedge_List.
//
// The Loft algorithm works as per the following flow chart:
//
// STEP1:
// -----
// If the sections do not have equal number of coedges, the loft algorithm
// splits the coedges as per the arc-length.
//
// STEP2:
// -----
// Than the corresponding coedges of the sections are used to construct the
// loft surface.
//
// STEP3:
// -----
// Construct the neccessary topology and the pcurves and join the individual
// peices to form a connected sheet body.
//

// Routine to construct the lofted body if only the coedge lists are provided
DECL_SKIN outcome
sg_loft_coedges(
    int ,           			        	// Number of sets
	Loft_Connected_Coedge_List *& sections,	// Set of lists of coedges.
	BODY*& out_body,
	skin_options* opts,
    double &oTol = *( double * )NULL_REF
    );

DECL_SKIN outcome  
sg_loft_coedges(
		int no_sections,					    // Number of sets
		Loft_Connected_Coedge_List *&sections,	// Set of lists of coedges
        int num_of_guides,
        EDGE* guides[],
		BODY*& out_body,
		skin_options* opts,
        double &oTol = *( double * )NULL_REF
        );

DECL_SKIN BODY*  
sg_skin_wires_with_laws(
		int no_sections,							// Number of sets
		Loft_Connected_Coedge_List *&sections,		// Set of lists of coedges.
		BODY **original_wires,
		BODY **breakup_wires,
		BODY *path = NULL,							// Path
		law** laws = NULL,
		int number_of_laws = 0,
		logical arc_length_option = FALSE,			// Arc length SPAparameter option
		logical twist_option = FALSE,				// If the co-edges are all closed
													// minimize the surface twist.
		logical align_direction = TRUE,				// Align the directions of the 
													// co-edges so they are the same.
		logical perpendicular_option = FALSE,		// Calculate the direction of the
													// loft perpendicular to coedges.
		logical simplify_option = FALSE,			// Simplify the surface,
													// if no surfaces.
		logical closed = FALSE,						// Means close the loft
		logical solid = FALSE,						// Make a solid out of the body
		logical allow_same_uv = FALSE,              // Allow surfaces with same U and V directions
		logical arc_length_u_option = FALSE,		// Arc-length paramertizes all input curves
		logical estimate_loft_tanfacs = FALSE,      // Make a surface with minimum curvature
		skin_gap_type gap_type = SKIN_GAP_EXTENDED, // Lateral gap filling type.
		logical self_int_test = FALSE				// test for self intersections..
		);

DECL_SKIN outcome
sg_skin_faces(
	FACE	*face1,				 // Input Face 1
	FACE    *face2,				 // Input Face 2
	BODY    *&skin,				 // Output body
	skin_options* opts);

DECL_SKIN outcome
sg_loft_faces(
	FACE	*face1,				 // Input Face 1
	double  fact1,				 // Input take off factor for face 1
	FACE    *face2,				 // Input Face 2
	double  fact2,				 // Input take off factor for face 2
    int     num_of_guides,       // Number of Guides
    EDGE*   guides[],            // Guides
	BODY    *&loft,				 // Output body
	skin_options* opts,
   const double iModelTolerance = SPAresabs);

DECL_SKIN logical 
sg_fix_topology_for_face_loft(BODY*                m_body,
							  Loft_Connected_Coedge_List* m_coedgeList,
							  FACE*                       m_face0,
							  FACE*                       m_face1,
							  logical                     m_face1_partner,
							  logical                     m_face2_partner,
                       const double                iModelTolerance = SPAresabs);

DECL_SKIN void
sg_make_lofting_wires(int in_no_sets,							// Input number of coed_sets
					  int &no_sets,								// Internal number of coed_sets
					  Loft_Connected_Coedge_List* in_coed_sets, // Input list of coed_sets
					  Loft_Connected_Coedge_List* &coed_sets,   // Internal list of coed_sets
					  BODY** &wires,							// Wires made by the routine after break-up
					  int &number_of_laws,						// number of laws associated with the wires
					  law** &laws,								// the laws associated with the routines
					  BODY** &sections,							// wires made by the routine before break-up
					  logical &closed,							// closed option is passed by reference  
					  logical& all_closed,
					  logical& degenerate_wire);

DECL_SKIN void 
sg_estimate_tangent_factors(int no_sets,							// Number of sets
							Loft_Connected_Coedge_List *coed_sets,	// Set of lists of coedges.
							BODY *path,								// Path
							BODY** breakup_wires,					// wires corresponding to the coedge_lists after break-up
							BODY** original_wires,					// wires corresponding to the coedge_lists before break-up
							int number_of_laws,						// total number of laws for all the wires
							law**	laws,							// law list associated with the wires
							logical arc_length_option,				// Arc length parameter option for the v direction
							logical twist_option,					// If the co-edges are all closed minimize the surface twist.
							logical perpendicular_option,			// Calculate the direction of the loft perpendicular to the coedges.
							logical closed,							// Closed in V direction option
							logical arcLengthInU,                   // Arc length parameter option for the u direction
							SPAinterval &tan_range,					// Range of tangent factors
							double &optimum,						// The best guy in the range
							double &minimum_radius);				// minimum_radius of the optimum range

DECL_SKIN void
sg_set_tangent_factors(int no_sets, Loft_Connected_Coedge_List *sections, double optimum);
	
DECL_SKIN logical 
sg_stitch_skinning_faces(int no_of_wires,			// Number of sections
		  				 BODY* wires[],				// Array of pointers to the wire bodies	after breakup
                         BODY* in_wires[],          // Array of pointers to the wire bodies	before breakup
						 ENTITY_LIST& face_list,    // List of already constructed faces
						 BODY*& result,				// Skin body returned		  
						 logical closed,			// Construct a closed solid body if TRUE
						 logical solid,				// End cap the first and last wires if TRUE
						 logical sliver_faces = 0,
						 logical*& degen_face_flag = *(logical**)NULL_REF);

// Added the optional argument c_replaced (which defaults to NULL); users can supply this and check
// the result to determine if the returned curve is different from the input curve c - DO NOT compare pointer addresses,
// as c may have been deleted, and the new returned curve could have c's old address!
// If supplied, c_replaced is returned as TRUE when the returned curve is different than the input curve, 
// FALSE if they are the same.
DECL_SKIN curve*
sg_replace_procedural_curve( curve* c,                 // Curve to either arc-length parameterize or replace par_int_cur
						     logical arcLengthInU,     // Arc-length parameterize if TRUE,  The par_int_cur replacement variable is global.
							 logical* c_replace=NULL);

DECL_SKIN logical 
sg_make_skinning_faces(int no_of_wires,					// Number of sections
		  			   BODY* wires[],					// Array of pointers to the wire bodies	
					   ENTITY_LIST& face_list,			// Returned face list
					   ENTITY_LIST& original_coedge_list,	// Original coedge list
					   logical arc_length_flag,			// TRUE arc length parameterization
					   logical twist,					// Minimize the twist
					   logical simplify,				// Simplify the geometry
					   int closed,						// Indicate if the skin is closed in v
					   logical allow_same_uv,			// Allow surfaces with the same u & v direction to be made
					   logical arc_length_u,			// Arc-length paramertizes all input curves
					   logical* degen_face_flag=NULL,   // Array of logicals indicating what faces are degenerate
                       logical self_int_test = FALSE);

DECL_SKIN logical 
sg_make_ruled_skin(
				   int no_of_wires,					// Number of sections
				   BODY* wires[],					// Array of pointers to the wire bodies	after break-up
                   BODY* in_wires[],				// Array of pointers to the wire bodies	before break-up
				   BODY*& result,					// Skin body returned
				   ENTITY_LIST& org_coedges,		// Original coedges
				   logical arc_length_flag,			// TRUE arc length parameterization
				   logical twist,					// Minimize the twist
				   logical simplify,				// Simplify the geometry
				   int closed,					    // Construct a closed solid body if TRUE
				   logical solid,					// End cap the first and last wires if TRUE
				   logical allow_same_uv,			// Allow surfaces with the same U and V direction
				   logical arcLengthInU);			// Arc-length paramertizes all input curves

DECL_SKIN logical 
sg_stitch_ruled_skin_parts(int noWires,
						   BODY* parts[],
						   BODY*& result);

DECL_SKIN logical 
sg_make_ruled_faces(int no_of_wires,				// Number of profiles
		  			BODY* wires[],					// Array of pointers to the wire bodies	after break-up
					ENTITY_LIST& face_list,			// Skin body returned		  
					logical arc_length_flag,		// TRUE arc length parameterization
					logical twist,					// Minimize the twist
					logical simplify,				// Simplify the geometry
					int closed, 	     			// Construct a closed solid body if TRUE
					logical allow_same_uv, 			// Allow surfaces with the same U and V direction
					logical arc_length_u_option);   // Arc-length paramertizes all input curves in u
		

DECL_SKIN logical 
sg_make_skinning_guide_faces(int no_of_wires,				// Number of sections
		  					 BODY* wires[],				    // Array of pointers to the wire bodies	
							 int no_of_guides,			    // Number of guide curves
							 curve* guides[],				// Array of pointers to the wire guides 
							 ENTITY_LIST& face_list,		// Returned face list
                             int number_of_laws=0,          // number of laws
                             law** laws=NULL,               // laws
                             guide_curve_preference gf=FOLLOW_GUIDE_CONSTRAINT, // guide curve constraint type
							 logical arcLengthInU=FALSE,    // Arc-length paramertizes all input curves in the u direction
                             logical self_int_test=FALSE,   // Self intersection test 
							 logical tolerant_guides=FALSE);		// tolerant guides

DECL_SKIN logical 
sg_adjust_periodic_with_guide(int no_of_wires, 
							  BODY** wires, 
							  curve* guide_curve);

DECL_SKIN logical
sg_adjust_periodic_wire_with_guide(WIRE* wire,
							  curve* guide_curve);

DECL_SKIN ENTITY_LIST 
sg_make_vertices_with_guide(int no_of_wires, 
							  BODY** wires, 
							  curve* guide_curve,
							  int numberOfGuides,
							  logical* haveSimplified);


#endif
