/* ORIGINAL: acis2.1/sg_husk/vrbln/v_bl_spl.hxx */
// $Id: v_bl_spl.hxx,v 1.48 2002/08/20 15:20:48 jenglund Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef VAR_BLEND_SPL_SUR_HXX
#define VAR_BLEND_SPL_SUR_HXX

// Header file for a general blend surface.  Derived from spl_sur.

// Adele Modifications---
//
//  20-Jun-07 pvc   Added method get_section_cc to var_blend_spl_sur class.
//	27-Jun-06 btq	Made spring_deriv_calculator_srf_srf class a friend 
//					of var_blend_spl_sur.
//	15-Mar-05 syh	Added method to find discontinuities arising from 
// 					defining curve and radius function.
//	13-Oct-04 syh	Added method find_support_derived_discontinuities() 
// 					to find out discontinuities on the blend surface 
// 					derived from the support.
//  17-Jun-04 puc   Changes to identify variable radius specifications too large
//                  for supports.
//	04-Nov-02 bwc	Added code for evaluating on linear u extensions.
//	24-Oct-02 bwc	Added code for calculating 4th derivatives.
//

// Modification---
//	07-Jul-99 gct	Added new constructor which takes surfaces and bs2_curves.
//	08-Jun-99 bd	Add uncalibrate_radius().
//	10-Feb-99 bd	Add ratBez_section::debug().
//	02-Dec-98 bd	Pass args as const-ref to param_from_plane_point().
//	01-May-98 bd	Rename eval_spring(crv,) to int_spring(crv,).
//	22-Apr-98 bd	New methods blend_too_big().
//	07-Mar-98 bd	Add flag rb_envelope.
//  06 Jan 97 nvp   Add make_approx_surface() method.
//	02-Aug-96 bd	New method get_slice().
//	23-Jul-96 dap	Changed make_approximating_surface to old_make_...
//	29-Feb-96 bd	Add a param guess arg to param_from_plane_point().
//	28-Feb-96 bd	Add make_approximating_surface().
//	21-Feb-96 bd	Remove eval_springs(srf,...), make it a non-member, rename
//					to eval_springs_cap (because it calls intersectors -- for
//					micro-kernel).
//	07-Feb-96 bd	Replace var_rad_spl_sur with var_blend_spl_sur.
//	01-Feb-96 bd	Remove open_end arg from eval_springs and set_save_slice.
//	28-Jan-96 aed	Change null refs to NULL_REF;
//					also rename locals to be different from member data.
//	05-Jan-96 aed	Use dispatch tables for save/restore of derived types of
//					var_radius and forms of var_cross_section.
//	27-Sep-95 bd	New method get_slicing_plane(), for sliding-disc algorithm.

#include "acis.hxx"
#include "dcl_kern.h"

#include "logical.h"

#include "blnd_spl.hxx"

#include "var_rad.hxx"
#include "blnd_sec.hxx"
#include "bl_cntct.hxx"

#include "transf.hxx"
#include "bs2curve.hxx"

#include "debugmsc.hxx"
#include "fileio.hxx"

class SizeAccumulator;

class FACE;
class curve;
class pcurve;
class spline;

class extended_curve;		
class ratBez_section;		
class gsm_blend_problem;
class extension_info;

// Changed to SESSION_LOCAL_VAR extern DECL_KERN int var_blend_spl_sur_type;

class DECL_KERN var_blend_spl_sur: public blend_spl_sur 
{
// Extention fn declared as a friend.

#ifdef var_blend_spl_sur_FRIENDS
	var_blend_spl_sur_FRIENDS
#endif

public:

	// curve whose SPAposition and tangent define the slicing plane, as a
	// function of v. If the pointer is NULL, then use def_curve.

	curve *slicing_plane_crv;

	// We use an explicit convexity indicator.

	logical is_convex;

	// A flag to mark whether this surface was made with the true rolling-ball
	// envelope algorithm.
	// Note: this value is set in accordance with the value of the option
	// bl_envelope_surf at the time of construction. If the surface is created
	// before the blend is actually fixed, for example by a blend-query, the
	// value at the time of creation will be in effect.

	logical rb_envelope;

	// Maintain a permanent pointer to a slice.
	// Information can be cached in here without violating the const-ness of
	// eval. This is not saved or restored, but re-created when needed.

    mutable v_bl_contacts *save_slice;

	// Constructors.

	// Construct a generally null blend surface.
	// This is to allow flexibility for constructors for derived classes to set
	// the common data members in whatever way is most convenient.

	var_blend_spl_sur();

	// Construct a curve-surface var_blend_spl_surf given all required
	// information, including the radius and section objects.
	//
	// Either the left_face_crv or the right_face_crv should be null. If they
	// are both non-null, then left face-curve will be used. If they are both
	// null, then there will be trouble. Neither of the FACEs may be null.
	//
	// Makes a copy of the face surfaces, but not of the given curve. (That's
	// because we don't know about transforming or reversing it.) The given
	// curve becomes our property and must not be deleted by the caller.
	//
	// The section object is also copied, because our local one is not a
	// pointer.  The radius objects are not copied, however; we just copy the
	// pointers and use the actual objects pointed to. They thus become our
	// property, and will be deleted when the surface is deleted.  The caller
	// must make copies if appropriate.
	//

	var_blend_spl_sur(
				FACE const *left_face,
				curve const *left_face_crv,
				FACE const *right_face,
				curve const *right_face_crv,
				logical convex,
				curve const *def_crv,
				SPAinterval	const &v_range,
				var_radius *radius1,
				var_radius *radius2,
				var_cross_section *x_sect,
				curve *loft_curve = NULL
				);

	// Create a surface-surface variable radius blend.

	var_blend_spl_sur(
				surface const *left_srf,
				surface	const *right_srf,
				bs2_curve left_bs2,
				bs2_curve right_bs2,
				logical	convex,
				curve const *def_crv,
				SPAinterval	const &v_r,
				var_radius *radius1,
				var_radius *radius2,
				var_cross_section *x_sect,
				curve *loft_curve = NULL,
				closed_forms u_closure = CLOSURE_UNSET,
				closed_forms v_closure = CLOSURE_UNSET,
				blend_support *center_sup = NULL
				);

	// Another version, for constructing two/three support blends.

	var_blend_spl_sur(
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

	// Copy constructor and copy() method. These should suffice for all
	// derivations of this class.

	var_blend_spl_sur( var_blend_spl_sur const &old );

	~var_blend_spl_sur();


	virtual subtrans_object *copy() const = 0;

	// Make a copy without sharing subdata.

	virtual spl_sur *deep_copy( pointer_map *pm = NULL ) const = 0;
	void deep_copy_elements_var_blend( 
                            var_blend_spl_sur const &old, 
                            pointer_map *pm = NULL
                            );


	// Data member retrieval.

	logical convex() const { return is_convex; }

	// Utilities.

	virtual logical operator==( subtype_object const &other ) const;
	virtual void operator*=( SPAtransf const &xfm );

	virtual void shift_v( double delta );
	virtual void split_v( double split_param, spl_sur *pieces[2] );

	// General inquiry functions.

	logical explicit_radius() const { return rad->explicit_var_radius(); }
	logical implicit_radius() const { return rad->implicit_var_radius(); }

	// Radius evaluation.

	virtual void radius(
		            double v_param,
		            double *rd  = NULL,
		            evaluate_curve_side side = 
                        evaluate_curve_unknown,
		            double *der1 = NULL,
		            double *der2 = NULL,
		            double *der3 = NULL 
                    ) const
	{
		this->left_radius( v_param, rd, side, der1, der2, der3 );
	}

	virtual void left_radius(
		            double v_param,
		            double *rad  = NULL,
		            evaluate_curve_side	side = 
                        evaluate_curve_unknown,
		            double *der1 = NULL,
		            double *der2 = NULL,
		            double *der3 = NULL,
		            double *der4 = NULL 
                    ) const;

	virtual void right_radius(
		            double v_param,
		            double *rad  = NULL,
		            evaluate_curve_side side = 
                        evaluate_curve_unknown,
		            double *der1 = NULL,
		            double *der2 = NULL,
		            double *der3 = NULL,
		            double *der4 = NULL 
                    ) const;

	// Find the minimum of both the left and the right radii.

	double minimum_radius(
		            SPAinterval const &vrange = 
                        *( SPAinterval * ) NULL_REF,
		            logical const &left_only  = 
                        *( logical * ) NULL_REF
		            ) const;

	// Find the maximum of both the left and the right radii.

	double maximum_radius(
		            SPAinterval const &vrange = 
                        *( SPAinterval * ) NULL_REF,
		            logical const &left_only = 
                        *( logical * ) NULL_REF
                    ) const;

	// Find the SPAparameter value (if any) at which either the left or the
	// right side has the supplied radius. If there is more than one root, the
	// root closest to the supplied SPAparameter is returned. If a point is not
	// found, which has the requested radius, return null.

	double find_radius(
		        double	requested_rad,
		        logical left_side,
		        double	closest_param,
		        SPAinterval &vrange = 
                    *( SPAinterval * ) NULL_REF 
                ) const;

	// Eventually we might need versions of these for the left & right radii:

	double end_radius( logical at_start ) const;
	double end_rad_slope( logical at_start ) const;

	// Calibrate (or uncalibrate) the radius function(s).

	void calibrate_radius( double t0, double t1 );
	void uncalibrate_radius();

	// Evaluate the thumbweights (for the cross section) at a v-SPAparameter.

	void eval_thumbweights( 
                        double v_param, 
                        int n_derivs,
						double left_tbw[ 4 ], 
                        double right_tbw[ 4 ] 
                        ) const;

	// Geometric evaluation.

	// The evaluate() function calculates derivatives, of any order up to the
	// number requested, and stores them in vectors provided by the user. It
	// returns the number it was able to calculate; this will be equal to the
	// number requested in all but the most exceptional circumstances. A certain
	// number will be evaluated directly and (more or less) accurately; higher
	// derivatives will be automatically calculated by finite differencing; the
	// accuracy of these decreases with the order of the derivative, as the cost
	// increases.

	virtual int evaluate(
		            SPApar_pos const &,	    // Parameter
		            SPAposition &,			// Point on surface at given SPAparameter
		            SPAvector ** = NULL,	// Array of pointers to arrays of
								            // vectors, of size nd. Any of the
								            // pointers may be null, in which
								            // case the corresponding derivatives
								            // will not be returned. Otherwise
								            // they must point to arrays long
								            // enough for all the derivatives of
								            // that order - i.e. 2 for the first
								            // derivatives, 3 for the second, etc.
		            int nd = 0,			    // Number of derivatives required (nd)
		            evaluate_surface_quadrant = evaluate_surface_unknown
								            // the evaluation location - above,
								            // below for each SPAparameter direction,
								            // or don't care.
		            ) const;


	// Return the number of derivatives which evaluate() can find "accurately"
	// (and fairly directly), rather than by finite differencing, over the given
	// portion of the surface. If there is no limit to the number of accurate
	// derivatives, returns the value ALL_SURFACE_DERIVATIVES, which is large
	// enough to be more than anyone could reasonably want.

    // Defaults to the whole surface

	virtual int accurate_derivs(
		                SPApar_box const &pbx = 
                            *(SPApar_box *)NULL_REF 
	                    ) const;

	// Implement (stubs for) new pure virtual functions: start: Find the
	// angle between perps to supports at def_cvec, according to the rules
	//	i) pipes return two pi
	// ii) singularities return 0
	// iii) 0 <= ang <= two pi

	virtual double blend_total_angle(
		                    SPAposition &P,
		                    SPAunit_vector &Tan,
		                    SPAvector const &R0,
		                    SPAvector const &R1,
		                    double &rr_sina = 
                                *(double*) NULL_REF,
		                    double &rr_cosa = 
                                *(double*) NULL_REF,
		                    double v_value = 0.
		                    ) const;

	// Find the angle between two radius vectors at def_cvec, according to the
	// rule 0 <= ang < two pi - SPAresnor

	virtual double blend_angle(
		                    SPAunit_vector &Tan,
		                    SPAvector const &R0,
		                    SPAvector const &R1,
		                    double &rr_sina = 
                                *(double*) NULL_REF,
		                    double &rr_cosa = 
                                *(double*) NULL_REF,
		                    double v_value  = 0.
		                    ) const;


	virtual void point_perp(
		                SPAposition const &point,		// object point
		                SPAposition &foot,		// resulting point on surface
		                SPAunit_vector &norm,		// unit normal there
		                surf_princurv &curv,		// principle curvatures there
		                SPApar_pos const &uv_guess,	// surface SPAparameter guess
		                SPApar_pos &uv_actual,	// resulting surface parameters
		                logical f_weak = FALSE,
		                SPApar_box const &subset_range = 
                            *(SPApar_box *)NULL_REF
		                ) const;

	// Find the SPAparameter values of a point on the surface.

	virtual SPApar_pos param(
		                   SPAposition const &pt,
		                   SPApar_pos const &guess
		                   ) const;

	// Evaluation specific to variable radius blend surface.

	// The following are newly defined at this level:
	// Some are pure virtual, some are defined here. None is defined both here
	// and below.

	//
	// eval_springs() takes a slice at a given v-SPAparameter; this is the basic
	// routine for stage 1 of the evaluator, and is also used in many other
	// places to find the contact points, spine point and v-derivs.

	// It calls the get_slice() method of the surface, which returns a pointer
	// to a slice owned by the surface, either save_slice or another slice in
	// its cache. If that succeeds, it creates the new slice object from the
	// save slice.  Our actual evaluator can just call get_slice(), and avoid
	// what amounts to lots of copying and memory activity.
	//
	// get_slice() checks the cache, and calls set_save_slice() if necessary.
	// set_save_slice() is specific to the type of blend (face-face, edge-face,
	// etc) and is pure virtual.  get_slice() just calls set_save_slice(), and
	// is generic, and so is defined here, and is not virtual.

	//
	// eval_springs() returns a new v_bl_contacts object. The number of
	// derivatives successfully calculated is set in result->num_derivs; compare
	// this to the input n_derivs to check its success. If the slice failed to
	// converge at all, the result will be null.
	//

	// Take a slice at a given v-SPAparameter.  The regular evaluator.

	v_bl_contacts *eval_springs(
		                double v_param,
		                int n_derivs,
		                v_bl_contacts *guess = NULL,
		                logical approx_ok = FALSE,
		                logical repeat_order = FALSE
		                ) const;

	// Specific evaluator that doesn't create a new v_bl_contacts object. It
	// returns a pointer to a slice, but the slice belongs to the spl_sur, and
	// must not be deleted or otherwise misused. The number of derivatives
	// successfully calculated is set in the num_derivs field of the resulting
	// slice; compare this to the input n_derivs to check its success.

	v_bl_contacts *get_slice(
		                double v_param,
		                int n_derivs,
		                evaluate_curve_side side = 
                            evaluate_curve_unknown,
		                v_bl_contacts *guess = NULL,
		                logical approx_ok = FALSE,
		                logical	repeat_order = FALSE
		                ) const;

	// Internal evaluator that doesn't create a new v_bl_contacts object.
	// This is specific to the type of blend surface, and is pure virtual. 

	virtual logical set_save_slice(
		                double v_param,
		                int n_derivs,
		                evaluate_curve_side	side,
		                v_bl_contacts *guess = NULL,
		                logical	approx_ok = FALSE,
		                logical	repeat_order = FALSE 
                        ) const;

	// Builds the 3 curves for the 3-curve evaluator and flips the switch so
	// that the blend surface uses the 3-curve evaluator.

    void instantiate_3_curve_evaluator();

	// Used internally by the eval_springs() and set_save_slice() methods.

	virtual void slice_derivs( int n_derivs, v_bl_contacts *result ) const = 0;

	// Find a slice where a spring curve intersects a given surface curve,
	// presumably an edge of the face.  Used during creation. This is specific
	// to the type of blend surface, and is pure virtual.

	virtual v_bl_contacts *int_spring(
		                    curve const &sfcrv,
		                    logical left_srf,
		                    int n_derivs,
		                    v_bl_contacts *guess = NULL,
		                    logical approx_ok = FALSE,
		                    logical	repeat_order = FALSE,
		                    SPAparameter &incpt_t = 
                                *( SPAparameter * ) NULL_REF
		                    ) const = 0;

	// Cross-section evaluation.
	// These are all the same for all variations of blend surfaces. They are
	// implemented in file vrbl_section.cxx.

	// Return a cross-section object between two given contact points.

	ratBez_section *get_section( 
                            v_bl_contacts *input_slice, 
                            int n_derivs 
                            ) const;

	// Return a cross-section object. Specific to thumbweights-type cross 
    // section.

	ratBez_section *get_section_tbw(
		                    v_bl_contacts *input_slice,
		                    int n_derivs
		                    ) const;

	// Return a cross-section object.
	// Specific to rounded-chamfer type cross section.

	ratBez_section *get_section_r_chamf(
		                    v_bl_contacts *input_slice,
		                    int n_derivs
		                    ) const;


	// Return a cross-section object.
	// Specific to curvature-continuous type cross section.

	ratBez_section *get_section_cc( 
                            v_bl_contacts *input_slice, 
                            int n_derivs 
                            ) const;


	// Evaluate a cross-section object at a given u-SPAparameter.

	int eval_section(
				ratBez_section *cross,	// input
				double u_param,
				int n_derivs,
				SPAposition &pos,			// output
				SPAvector dpos[ 2 ] = NULL,
				SPAvector ddpos[ 3 ] = NULL,
				SPAvector dddpos[ 4 ] = NULL,
				evaluate_curve_side side = evaluate_curve_unknown
				) const;


	// Evaluate a point on a blend surface that has been extended in the u
	// direction.

	int eval_on_linear_extension(
				ratBez_section *cross,	// input
				double u_param,
				int &n_derivs,
				SPAposition &pos,			// output
				SPAvector dpos[ 2 ],
				SPAvector ddpos[ 3 ],
				SPAvector dddpos[ 4 ],
				v_bl_contacts *slice
				) const;


	// Evaluate some information about a slice at a given v-SPAparameter. The
	// output data are placed in the blend_section argument.

	virtual void compute_section(
				double v,				// v SPAparameter
				int	spine_nder,			// number of required spine derivs
				int	spring_nder,		// no. of req'd spring derivs
				logical xcrv_norm,		// whether to calculate this
				blend_section &section_data, // all output in here
				int = 0	 // the evaluation location - 1 => above,
						 // -1 => below, 0 => don't care.
				) const;

	// Evaluate the slicing plane at a given v SPAparameter. The v-derivatives
	// are actual derivatives, so X_v is not a unit SPAvector. Implement two
	// versions, for convenience.

	void get_slicing_plane(
				double v_param,
				evaluate_curve_side	side,
				int n_derivs,
				SPAposition *oX,
				SPAvector *derivs[] 
				) const;

	void get_slicing_plane(
				double v_param,
				evaluate_curve_side side,
				SPAposition *oX,
				SPAvector *X_v = NULL,
				SPAvector *X_vv = NULL 
				) const;

	// Inverse of get_slicing_plane(): return the SPAparameter given a point.

	void param_from_plane_point(
				SPAposition	const &pt,
				const double &guess_param,	// may be NULL_REF.
				double *v_param				// result.
				) const;

	// Methods to return whether a blend appears to be too big at a location
	// along the blend.  One works from a given v_bl_contacts object, and the
	// other from a v-SPAparameter value. The last argument returns a measure of
	// just how much too big the blend would be here, suitable for use in a
	// root-finding iteration. Negative values are too big, positive are ok.

	logical blend_too_big( double v_param, double &value = *(double*)NULL_REF );

	// slice must have first derivatives set: slice->num_derivs >= 1

	logical blend_too_big(
					v_bl_contacts *slice,
					double &value = *(double*)NULL_REF 
					);

	// Make a bs3_curve from a ratBez_section

	bs3_curve make_cross_curve( ratBez_section *cross ) const;

	// Make a bs2_curve for a cross pcurve

	bs2_curve make_cross_pcurve( double v_param ) const;

	// Construct an iso-SPAparameter line on the surface. A u-SPAparameter line
	// runs in the direction of increasing u SPAparameter, at constant v. A
	// v-SPAparameter line runs in the direction of increasing v, at constant u.
	// The parametrisation in the non-constant direction matches that of the
	// surface, and has the range obtained by use of param_range_u() or
	// param_range_v() appropriately.

	 virtual curve *u_param_line( double v, spline const &owner ) const;

	// For v_param_line, we can't do any better than the parent.
	// curve *v_param_line( u, owner );

	// Create the approximating spline surface. There is a new and better
	// function provided now in the base class which should be upgraded for
	// var_blend_spl_surs and used as soon as possible.

	logical old_make_approximating_surface( double tol );

	// Save and restore. Save is easy, as derived class switching goes through
	// the normal virtual function mechanism. Restore is more complicated,
	// because until it is invoked we do not have an object of the right class.
	// Instead we switch on a table defined by static instances of the
	// restore_ss_def class (see below), to invoke a simple friend function
	// which constructs an object of the right (derived) type. Then it can call
	// the appropriate member function to do the actual work.

	// Pure virtual:

	virtual char const *type_name() const = 0;

	virtual void save() const;
	virtual void save_data() const;

	virtual logical need_save_as_approx(
                                int save_to_version, 
                                logical check_progenitors
                                ) const;

	// This one must be "pure virtual":
	//	friend subtype_object *restore_var_blend_spl_sur();

	void restore_data();

	// Debug printout. As for save and restore we split the operation into two
	// parts: the virtual function "debug" prints a class-specific identifying
	// line, then calls the ordinary function "debug_data" to put out the
	// details. It is done this way so that a derived class' debug_data can call
	// its parent's version first, to put out the common data. Indeed, if the
	// derived class has no additional data it need not define its own version
	// of debug_data and use its parent's instead. A string argument provides
	// the introduction to each displayed line after the first, and a logical
	// sets "brief" output (normally removing detailed subsidiary curve and
	// surface definitions).

	virtual void debug( char const *, logical, FILE *) const;
	void debug_data( char const *, logical, FILE *) const;

	virtual void full_size( 
                        SizeAccumulator &sz, 
                        logical count_self = TRUE 
                        ) const;

	virtual void minimize( minimize_helper *mh );

	virtual SPAcrvGeom *make_crvGeom_object( 
        curve* = NULL, int = 0, int* = NULL, double* = NULL ) const;

	virtual SPAcrvGeom *get_left_spring_crvGeom(
		int nseeds = 0, int *cont = NULL, double *seeds = NULL);
	virtual SPAcrvGeom *get_right_spring_crvGeom(
		int nseeds = 0, int *cont = NULL, double *seeds = NULL);
	virtual SPAcrvGeom *get_spine_crvGeom(
		int nseeds = 0, int *cont = NULL, double *seeds = NULL);

	virtual double blend_radius( double v_param ) const;

    // Is defcurve tangent is in the same/opposite direction as the cross 
    // product of left and right surface normals! We assume that this relation 
    // is maintained. Set the sense if not set. 
    // If it is set, check if the above mentioned relation is maintained for 
    // this slice. 

	virtual logical get_sense_wrt_defcurve( v_bl_contacts * ) { return TRUE; };

	/**
	* @nodoc
	**/
	virtual void process( geometry_definition_processor& p ) const;

protected:

	// Update the save slices after a blend surface definition change. 

	void reset_slice_cache();

	virtual void reconstruct_approximating_curves();

	virtual logical update_new_evaluator_curves();

	// Worker function to generate and return the "3-curves" for a given range.
	// The incoming bs3 curves is assumed to be NULL

	logical generate_new_evaluator_curves(
                                  SPAinterval const &required_range,
								  bs3_curve &spine_bs3,
								  bs3_curve &left_spring_bs3,
								  bs3_curve &right_spring_bs3
								  ) const;

	// Calculate the discontinuity information along u

	virtual void calculate_disc_info_u( SPAinterval const &range_of_interest );

	// Calculates the discontinuities along v, only within the range specified.

	virtual void calculate_disc_info_v( SPAinterval const &range_of_interest );

	// Calculates the derived discontinuities along v, only within the range specified.

	void find_support_derived_discontinuities( SPAinterval const &range_of_interest_v );
	void find_def_curve_derived_discontinuities( SPAinterval const &range_of_interest_v );
	void find_rad_function_derived_discontinuities( SPAinterval const &range_of_interest_v );

	// Internal function - Not guaranteed to be stable

	void limit_to_largest_continuous_v_range( int continuity_level );

	// Whether the surface primary data for stabilizing the evaluator like 
	// defining curve and support bs2 curves should be saved.

	virtual logical serialize_primary_evaldata() const;

	// Whether the center support of the blend should be saved even if it is
	// NULL.

	virtual logical serialize_center_support() const;

    // 
    // compute additional derivatives for curvature continuous sections
    // and (possibly) other forms; must be called at the end of the
    // virtual slice_derivs() implementation in order to ensure assumed
    // pre-existing data is available.
    //
    void slice_derivs_after( int n_derivs, v_bl_contacts *result ) const;

	// Internal friends

	friend class ATTRIB_VAR_BLEND;
	friend class ATT_BL_VR;
	friend class spring_deriv_calculator_srf_srf;

	// Enumeration which stores relationship of the blend-surface wrt its defining
	// curve
	
	typedef enum sense_wrt_defcrv_type { NOTSET, ALONG, BACKWARDS } sense_wrt_defcrv;
	sense_wrt_defcrv _sense_wrt_defcrv;

private:

	// The blend problem object. The blend spl_sur object is the 'explicit'
	// view of this object.

	gsm_blend_problem *_surf_p;
    
	// Use this variable to avoid recursion in spring_deriv_calculator_srf_srf class

	int _near_singularity;
    mutable v_bl_contacts *m_computation_slices;
};



//
//	A class to represent an extended curve.
//

//
// There are many ways in which a curve could be extended, but currently we'll
// only do straight extensions. If we need more, here's what we might do:
//
//	enum extension_type
//		{ EXTEND_STRAIGHT, EXTEND_NATURAL, EXTEND_CIRCULAR, EXTEND_REFLECT }
//
//	extension_type how_to_extend;
//
// Also, there's no apparent need to change, or even report, the range.

// So, why shouldn't we just save this, instead of a curve, with the surface?
// Because then we would have to do a full-blown implementation of it, complete
// with save/restore, split_param, etc.-- derive a new class.
//
// But won't we need it for every evaluator-method call?  (Evaluator methods are
// eval(), point_perp(), etc.)  In which case we would have to construct a new
// one on each call (or have the evaluator methods be non-const, which is
// impossible).
//
// Well, yes, we will have to cnostruct a new one each time, but guess what:
// it's really easy to do that. Maybe someday this (or someone else's version)
// can be properly incorporated.
//

class DECL_KERN extended_curve : public ACIS_OBJECT {
public:
	curve		*base_crv;

	logical		start_set;
	SPAposition start_pt;
	SPAvector	start_tan;

	logical		end_set;
	SPAposition	end_pt;
	SPAvector	end_tan;

	extended_curve( curve *base );
	~extended_curve();

	// Local utilities.

	void set_start();
	void set_end();

	// Curve evaluation stuff.  Add more variants as needed.
	// Note that these methods are not const, to allow lazy evaluation.

	void eval( double param, int n_derivs, SPAposition &pt, SPAvector *derivs[] );

	void point_perp(
		SPAposition const &pt,
		SPAposition &perp_pt,
		SPAunit_vector &perp_tan,
		double const &param_guess = *(double *)NULL_REF,
		double &perp_param = *(double *)NULL_REF,
		logical f_weak = FALSE
				);

	void point_perp(
		SPAposition const &given_pt,
		SPAposition &perp_pt,
		double const &param_guess = *(double *)NULL_REF,
		double &perp_param = *(double *)NULL_REF,
		logical f_weak = FALSE
		)
		{
			this->point_perp(
				given_pt, perp_pt, *(SPAunit_vector *)NULL_REF,
				param_guess, perp_param, f_weak );
		}

};



//
//	A class to represent the geometry of a cross section of a blend surface. The
//	geometry is a rational Bezier segment, together with cross-derivatives,
//	which are blended to become the v-derivatives of the surface. This class is
//	used as an intermediate step in surface evaluation, and during construction
//	of the approximating surface.
//
//	Points (and v-derivative vectors) will be returned in homogeneous space.
//

class DECL_KERN ratBez_section : public ACIS_OBJECT {

public:
	int		npts;
	int		n_derivs;
	SPAvector	*pts;	// each point is three coords (xyz) in homogeneous space
	double	*wts;
	SPAvector	*pts_v;
	double	*wts_v;
	SPAvector	*pts_vv;
	double	*wts_vv;
	SPAvector	*pts_vvv;
	double	*wts_vvv;    

	// Would a rational flag be worthwile?

	ratBez_section( void );	// Null, so we can do an array, without knowing num_pts.
	ratBez_section( int num_pts, int num_derivs );

	~ratBez_section();

	SPAinterval const &param_range() const { return _p_range; }
	void set_param_range( SPAinterval const &p_r ) { _p_range = p_r; }

    ratBez_section *next() const { return _next; }
    void set_next( ratBez_section *n ) { _next = n; }

    void delete_chain();

	void debug();

private:

	// The parameter range of the bezier curve.

	SPAinterval _p_range;

    ratBez_section *_next;
};

#endif

