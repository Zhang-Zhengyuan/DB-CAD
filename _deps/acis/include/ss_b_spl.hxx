/* ORIGINAL: acis2.1/sg_husk/vrbln/ss_b_spl.hxx */
// $Id: ss_b_spl.hxx,v 1.24 2002/08/16 19:28:37 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef SS_BLEND_SPL_SUR_HXX
#define SS_BLEND_SPL_SUR_HXX

// Header file for a variable-radius face-face blend surface.
// Derived from var_blend_spl_sur.

// Modification---
//
//	01-Jan-05 syh	Implmented eval_normal by handling cases where support
//					surfaces are parallel.
//	07-Jul-99 gct	Added new constructor which takes surfaces and bs2_curves.
//	01-May-98 bd	Rename eval_spring(crv,) to int_spring(crv,).
//	 8 Mar 96 nvp	Added function restore_old_var_spl() for restore
//                   of 1.7 or older files
//	28-Feb-96 bd	Add restore_srf_srf_v_bl_spl_sur().
//	07-Feb-96 bd	Provide defaults for c'tor args;
//		change incl of vrbl_spl to v_bl_spl.
//	02-Dec-95 bd	Pass loft curve to constructor.

#include "acis.hxx"
#include "dcl_kern.h"

#include "logical.h"
#include "v_bl_spl.hxx"

class SizeAccumulator;

DECL_KERN subtype_object *restore_srf_srf_v_bl_spl_sur();
DECL_KERN subtype_object *restore_old_var_rad_spl();

class DECL_KERN srf_srf_v_bl_spl_sur : public var_blend_spl_sur 
    {
	// Declare extensions as friends.

#ifdef srf_srf_v_bl_spl_sur_FRIENDS
	srf_srf_v_bl_spl_sur_FRIENDS
#endif

	// Constructors.

	// Construct a generally null blend surface.
	// This is to allow flexibility for constructors for derived classes
	// to set the common data members in whatever way is most convenient.

protected:

	srf_srf_v_bl_spl_sur();

public:

	// Construct a srf_srf_v_bl_spl_sur given everything that is needed.
	// Only certain combinations of input are valid, and the constructors
	// correspond exactly to the valid combinations. 
	
	// The constructor doesn't copy anything, but assumes ownership of
	// the data that is passed to it by pointer - namely the blend_supports,
	// radius functions and cross sections. 
    
    srf_srf_v_bl_spl_sur(
				FACE const *left_face,
				FACE const *right_face,
				logical convex,
				curve const	*def_crv,
				SPAinterval const &v_range,
				var_radius *radius1,
				var_radius *radius2 = NULL,	// same as radius1 if null
				var_cross_section *x_sect = NULL,	// CIRCULAR if null
				curve *loft_curve = NULL
				);

    srf_srf_v_bl_spl_sur(
				surface	const *left_srf,
				surface	const *right_srf,
				bs2_curve left_bs2,
				bs2_curve right_bs2,
				logical convex,
				curve const *def_crv,
				SPAinterval	const &v_r,
				var_radius *radius1,
				var_radius *radius2 = NULL,	// same as radius1 if null
				var_cross_section *x_sect = NULL,	// CIRCULAR if null
				curve *loft_curve = NULL,
				closed_forms u_closure = CLOSURE_UNSET,
				closed_forms v_closure = CLOSURE_UNSET,
				blend_support *center_sup = NULL
				);

	// Another version, for constructing two/three support blends.

	srf_srf_v_bl_spl_sur(
			logical two_sup,                        // The number of supports, two or three.
			blend_support *sup_arr[],               // The support array.
            logical convex,                         // Convexity of the blend.
			curve const &def_crv,                   // The defining curve of the blend.
			SPAinterval const &v_range,             // Blend surface parameter range.
			var_radius *left_rad,                   // The blend radius, left radius for chamfers.
			var_radius *right_rad = NULL,           // The right radius for chamfers.
			var_cross_section *x_sect = NULL,       // The cross-section, default is circular.
			closed_forms v_closure = CLOSURE_UNSET, // Closure of the blend.
			gsm_blend_problem *surf_p = NULL        // The blend surface problem.
			);

	srf_srf_v_bl_spl_sur( srf_srf_v_bl_spl_sur const &orig );

	// Copy method.
	// This could be defined in the parent and not here, but the parent
	// is an abstract class and can't construct an instance of itself.

	virtual subtrans_object * copy() const
	{
		return (subtrans_object *)( ACIS_NEW srf_srf_v_bl_spl_sur(*this));
	}

	// Make a copy without sharing subdata.

	virtual spl_sur *deep_copy( pointer_map *pm = NULL ) const;

	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
    
    virtual logical is_valid_blend_surface();

	// At pole ( radius goes to zero or surfaces are parallel, normal can be
	// better calculated by using definition of the surface. 

	virtual SPAunit_vector eval_normal(
		SPApar_pos const &
		) const;


	// Evaluation specific to variable radius blend surface.

	// For these, we just need to implement the internal evaluators
	// for finding the contact and spine points and their v-derivs
	// for this particular type of var_blend_spl_sur.
	// These don't create new v_bl_contacts objects.
	// See comments in the parent class declaration file.

	// Take a slice at a given v SPAparameter.

	virtual logical set_save_slice( double v_param,
									 int n_derivs,
									 evaluate_curve_side side,
									 v_bl_contacts *guess = NULL,
									 logical approx_ok    = FALSE,
									 logical repeat_order = FALSE
								) const;


	// Find the intersection of a spring curve with a given surface curve.

	virtual v_bl_contacts *int_spring( curve const &sfcrv,
										 logical left_srf,
										 int n_derivs,
										 v_bl_contacts *guess = NULL,
										 logical approx_ok    = FALSE,
										 logical repeat_order = FALSE,
                                         SPAparameter &incpt_t =
                                            * ( SPAparameter * ) NULL_REF
									) const;


	// Calculate the v-derivatives of the contact points.
	// This is virtual because of the different types of spring_deriv_calculator's.

	virtual void slice_derivs( int n_derivs, v_bl_contacts *result ) const;


	// Save and restore. 
	// We have no new data, so we don't need to do much.
	// There's only one thing we need: our parent cannot implement
	// restore_var_blend_spl_sur(), because it is an abstract class,
	// and that routine must make a "new" of itself.
	// So we do it here.

	static int id();
	virtual int type() const;

	virtual char const *type_name() const;

//	virtual void save() const;
//	virtual void save_data() const;

	friend DECL_KERN subtype_object *restore_srf_srf_v_bl_spl_sur();

//	void restore_data();

// Function to restore old data (var_rad_spl_sur) as a srf_srf_v_bl_spl_sur

	friend DECL_KERN subtype_object *restore_old_var_rad_spl();
	void restore_old_var_spl();

	// Is the def curve tangent in the same/opposite direction as the 
	// cross product of left and right surface normals! We assume that 
	// this relation is maintained. 
	// Set the sense if not set. If it is set, check if the above mentioned 
	// relation is maintained for this slice. 

	virtual logical get_sense_wrt_defcurve( v_bl_contacts *slice );	

	/**
	* @nodoc
	**/
	virtual void process( geometry_definition_processor& p ) const;

    };

#endif

