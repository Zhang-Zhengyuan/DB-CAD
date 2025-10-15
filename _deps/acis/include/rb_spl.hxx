/* ORIGINAL: acis2.1/kerngeom/splsur/rb_spl.hxx */
/* $Id: rb_spl.hxx,v 1.24 2002/08/20 15:20:22 jenglund Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header file for a rolling ball blend surface.  Derived from blend_spl_sur.

// This file resided formerly in the advanced blending husk, but the
// geometry definitions need to be generally available.
//
// Modification---
//
// 04-Nov-02 bwc  Added code for evaluating on u extensions of blend surface.
//		Added save function for saving as an approximation.


#include "acis.hxx"
#include "dcl_kern.h"
/**
* @file rb_spl.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISSURFACES
 *
 * @{
 */
#if !defined RB_BLEND_SPL_SUR_HXX
#define RB_BLEND_SPL_SUR_HXX

#include "logical.h"
#include "blnd_spl.hxx"

class SizeAccumulator;

/**
 * @nodoc
 */
DECL_KERN subtype_object *restore_rb_blend_spl_sur();

#if defined D3_STANDALONE || defined D3_DEBUG
/**
 * @nodoc
 */
DECL_KERN D3_istream& operator>>( D3_istream &, spl_sur *& );
#endif

// This class implements the constant radius rolling ball surface. It is a
// straightforward derivation of blend_spl_sur. The ball rolls on two support
// entities, which may be either curves, surfaces or points. The point-point
// case is not included because this is always a sphere. The surface-surface
// case is equivalent to the pipe surface.

/**
 * Implements the constant radius rolling ball blend surface.
 * <br>
 * <b>Role:</b> This is a straightforward derivation of <tt>blend_spl_sur</tt>. The ball rolls 
 * on two support entities, which may be either curves, surfaces or points. The point-point case 
 * is not included because this is always a sphere. The surface-surface case is equivalent to 
 * the pipe surface.
 */
class DECL_KERN rb_blend_spl_sur : public blend_spl_sur 
	{
	// Allow extensions to declare themselves as friends. USE WITH EXTREME CAUTION

#ifdef rb_blend_spl_sur_FRIENDS
	rb_blend_spl_sur_FRIENDS
#endif

private:

	// Constructors.

	// Construct a generally null blend surface.
	// This is to allow flexibility for constructors for derived classes
	// to set the common data members in whatever way is most convenient.

	rb_blend_spl_sur();

	// Copy constructor

	rb_blend_spl_sur( rb_blend_spl_sur const &orig );

	// An evaluator for the region of the blend outside the region enclosed by the
	// contact points of the blend.

	int eval_on_linear_extension(
					double u,
					SPAposition &pos,
					SPAvector **derivs,
					int n_derivs
					) const;

protected:

/**
* Check for any data errors in the surface, and correct the errors if possible.
* <br><br>
* <b>Role:</b> The various arguments provide control over which checks
* are made, which fixes can be applied and which fixes were actually
* applied.  The function returns a list of errors that remain in the
* surface on exit.
* See <tt>chk_stat.hxx</tt> for information on the argument types used here.
* At the moment we only fix non-G1 discontinuities by fixing
* them on the defining curves.
*/
	virtual	check_status_list *check(
  		        check_fix const &input =			// supplies a set of flags which 
					* (check_fix const *) NULL_REF, // say which fixes are allowable 
					                                // (the default is to fix nothing)						 						 
				check_fix &result = *(check_fix*) NULL_REF, // returns a set of flags 
															// which say which fixes
															// were applied
				check_status_list const * =				 // list of checks that are to 
					(check_status_list const *) NULL_REF // be made. If the	list is null, 
														 // then every possible check will
														 // be made; otherwise, the function 
														 // will only check for things in 
														 // the list.  The return value for 
														 // the function will then be a subset
														 // of this list.
				);

	// Calculate the discontinuity information along v

    virtual void calculate_disc_info_v( SPAinterval const &v_r );

public:

    // Construct an rb_blend_spl_sur given everything that is needed. Only certain
    // combinations of input are valid, and the constructors correspond exactly to
    // the valid combinations.
    
    // The constructor doesn't copy anything, but assumes ownership of the data that
    // is passed to it by pointer - namely the blend_supports, radius functions and
    // cross sections.

    // face-face blend

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Creates a face-face blend. The constructor doesn't copy anything, but assumes
 * ownership of the data that is passed to it by pointer-namely the <tt>blend_supports</tt>, <tt>radius
 * functions</tt> and <tt>cross sections</tt>.
 * <br><br>
 * @param left_srf
 * left surface.
 * @param right_srf
 * right surface.
 * @param left_bs2
 * left bs2.
 * @param right_bs2
 * right bs2.
 * @param def_crv
 * defining curve.
 * @param v_range
 * v range.
 * @param left_off
 * left off.
 * @param right_off
 * right off.
 * @param u_closure
 * u closure.
 * @param v_closure
 * v closure.
 */
    rb_blend_spl_sur( 
				surface const &left_srf,
				surface const &right_srf,
				bs2_curve left_bs2,
				bs2_curve right_bs2,
				curve const &def_crv,
				SPAinterval	const &v_range,
				double left_off,
				double right_off,
				closed_forms u_closure = OPEN,
				closed_forms v_closure = CLOSURE_UNSET
				);

	// face-edge blend
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Creates a face-edge blend. The constructor doesn't copy anything, but assumes
 * ownership of the data that is passed to it by pointer-namely the <tt>blend_supports</tt>, <tt>radius
 * functions</tt> and <tt>cross sections</tt>.
 * <br><br>
 * @param left_srf
 * left surface.
 * @param right_crv
 * right curve.
 * @param left_bs2
 * left bs2.
 * @param def_crv
 * defining curve.
 * @param v_range
 * v range.
 * @param left_off
 * left off.
 * @param right_off
 * right off.
 * @param u_closure
 * u closure.
 * @param v_closure
 * v closure.
 */
	rb_blend_spl_sur( 
				surface const &left_srf,
				curve const &right_crv,
				bs2_curve left_bs2,
				curve const &def_crv,
				SPAinterval	const &v_range,
				double left_off,
				double right_off,
				closed_forms u_closure = OPEN,
				closed_forms v_closure = CLOSURE_UNSET
				);

	// face-edge blend, with bs2 curve for the edge
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Creates an edge-vertex blend. The constructor doesn't copy anything, but assumes
 * ownership of the data that is passed to it by pointer-namely the <tt>blend_supports</tt>, <tt>radius
 * functions</tt> and <tt>cross sections</tt>.
 * <br><br>
 * @param left_srf
 * left surface.
 * @param right_crv
 * right curve.
 * @param left_bs2
 * defining curve.
 * @param right_bs2
 * defining curve.
 * @param def_crv
 * defining curve.
 * @param v_range
 * v range.
 * @param left_off
 * left off.
 * @param right_off
 * right off.
 * @param u_closure
 * u closure.
 * @param v_closure
 * v closure.
 */
	rb_blend_spl_sur( 
				surface const &left_srf,
				curve const &right_crv,
				bs2_curve left_bs2,
				bs2_curve right_bs2,
				curve const &def_crv,
				SPAinterval const &v_range,
				double left_off,
				double right_off,
				closed_forms u_closure = OPEN,
				closed_forms v_closure = CLOSURE_UNSET
				);


	// face-vertex blend
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Creates a face-vertex blend. The constructor doesn't copy anything, but assumes
 * ownership of the data that is passed to it by pointer-namely the <tt>blend_supports</tt>, <tt>radius
 * functions</tt> and <tt>cross sections</tt>.
 * <br><br>
 * @param left_srf
 * left surface.
 * @param right_pt
 * right point.
 * @param left_bs2
 * left bs2.
 * @param def_crv
 * defining curve.
 * @param v_range
 * v range.
 * @param left_off
 * left off.
 * @param right_off
 * right off.
 * @param u_closure
 * u closure.
 * @param v_closure
 * v closure.
 */
	rb_blend_spl_sur(	
				surface const &left_srf,
    		    SPAposition const &right_pt,
				bs2_curve left_bs2,
				curve const &def_crv,
				SPAinterval const &v_range,
    		    double left_off,
    		    double right_off,
				closed_forms u_closure = OPEN,
				closed_forms v_closure = CLOSURE_UNSET
    			);


	// edge-face blend
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Creates an edge-face blend. The constructor doesn't copy anything, but assumes
 * ownership of the data that is passed to it by pointer-namely the <tt>blend_supports</tt>, <tt>radius
 * functions</tt> and <tt>cross sections</tt>.
 * <br><br>
 * @param left_crv
 * left curve.
 * @param right_srf
 * right surface.
 * @param right_bs2
 * right bs2.
 * @param def_crv
 * defining curve.
 * @param v_range
 * v range.
 * @param left_off
 * left off.
 * @param right_off
 * right off.
 * @param u_closure
 * u closure.
 * @param v_closure
 * v closure.
 */
	rb_blend_spl_sur(	
				curve const &left_crv,
    		    surface const &right_srf,
			    bs2_curve right_bs2,
			    curve const &def_crv,
			    SPAinterval const &v_range,
    		    double left_off,
	    	    double right_off,
				closed_forms u_closure = OPEN,
				closed_forms v_closure = CLOSURE_UNSET
				);

	// edge-face blend, with bs2 curve for the edge.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Creates an edge-vertex blend. The constructor doesn't copy anything, but assumes
 * ownership of the data that is passed to it by pointer-namely the <tt>blend_supports</tt>, <tt>radius
 * functions</tt> and <tt>cross sections</tt>.
 * <br><br>
 * @param left_crv
 * left curve.
 * @param right_srf
 * right surface.
 * @param left_bs2
 * defining curve.
 * @param right_bs2
 * defining curve.
 * @param def_crv
 * defining curve.
 * @param v_range
 * v range.
 * @param left_off
 * left off.
 * @param right_off
 * right off.
 * @param u_closure
 * u closure.
 * @param v_closure
 * v closure.
 */
	rb_blend_spl_sur(	
				curve const &left_crv,
    		    surface const &right_srf,
				bs2_curve left_bs2,
				bs2_curve right_bs2,
				curve const &def_crv,
				SPAinterval	const &v_range,
    		    double left_off,
	    	    double right_off,
				closed_forms u_closure = OPEN,
				closed_forms v_closure = CLOSURE_UNSET
				);

	// edge-edge blend
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Creates an edge-edge blend. The constructor doesn't copy anything, but assumes
 * ownership of the data that is passed to it by pointer-namely the <tt>blend_supports</tt>, <tt>radius
 * functions</tt> and <tt>cross sections</tt>.
 * <br><br>
 * @param left_crv
 * left curve.
 * @param right_crv
 * right curve.
 * @param def_crv
 * defining curve.
 * @param v_range
 * v parameter range.
 * @param left_off
 * left offset.
 * @param right_off
 * right offset.
 * @param u_closure
 * u closure.
 * @param v_closure
 * v closure.
 */
	rb_blend_spl_sur(	
				curve const &left_crv,
    		    curve const &right_crv,
				curve const &def_crv,
				SPAinterval	const &v_range,
    		    double left_off,
    		    double right_off,
				closed_forms u_closure = OPEN,
				closed_forms v_closure = CLOSURE_UNSET
    			);

	// edge-edge blend, with bs2 curves.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied 
 * as arguments.
 * <br><br>
 * <b>Role:</b> Creates an edge-vertex blend. The constructor doesn't copy anything, but assumes
 * ownership of the data that is passed to it by pointer-namely the <tt>blend_supports</tt>, <tt>radius
 * functions</tt> and <tt>cross sections</tt>.
 * <br><br>
 * @param left_crv
 * left curve.
 * @param right_crv
 * right curve.
 * @param left_bs2
 * defining curve.
 * @param right_bs2
 * defining curve.
 * @param def_crv
 * defining curve.
 * @param v_range
 * v range.
 * @param left_off
 * left off.
 * @param right_off
 * right off.
 * @param u_closure
 * u closure.
 * @param v_closure
 * v closure.
 */
	rb_blend_spl_sur(	
				curve const &left_crv,
    		    curve const &right_crv,
				bs2_curve left_bs2,
				bs2_curve right_bs2,
				curve const &def_crv,
				SPAinterval	const &v_range,
    		    double left_off,
    		    double right_off,
				closed_forms u_closure = OPEN,
				closed_forms v_closure = CLOSURE_UNSET
    			);

	// edge-vertex blend
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as 
 * arguments.
 * <br><br>
 * <b>Role:</b> Creates an edge-vertex blend. The constructor doesn't copy anything, but assumes
 * ownership of the data that is passed to it by pointer-namely the <tt>blend_supports</tt>, <tt>radius
 * functions</tt> and <tt>cross sections</tt>.
 * <br><br>
 * @param left_crv
 * left curve.
 * @param right_pt
 * right point.
 * @param def_crv
 * defining curve.
 * @param v_range
 * v range.
 * @param left_off
 * left off.
 * @param right_off
 * right off.
 * @param u_closure
 * u closure.
 * @param v_closure
 * v closure.
 */
	rb_blend_spl_sur(	
				curve const &left_crv,
    		    SPAposition const &right_pt,
				curve const &def_crv,
				SPAinterval	const &v_range,
    		    double left_off,
    		    double right_off,
				closed_forms u_closure = OPEN,
				closed_forms v_closure = CLOSURE_UNSET
				);

	// vertex-face blend
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as 
 * arguments.
 * <br><br>
 * <b>Role:</b> Creates a vertex-face blend. The constructor doesn't copy anything, but assumes
 * ownership of the data that is passed to it by pointer-namely the <tt>blend_supports</tt>, <tt>radius
 * functions</tt> and <tt>cross sections</tt>.
 * <br><br>
 * @param left_pt
 * left point.
 * @param right_srf
 * right surface.
 * @param right_bs2
 * right bs2.
 * @param def_crv
 * defining curve.
 * @param v_range
 * v range.
 * @param left_off
 * left off.
 * @param right_off
 * right off.
 * @param u_closure
 * u closure.
 * @param v_closure
 * v closure.
 */
	rb_blend_spl_sur(	
				SPAposition const &left_pt,
	    	    surface const &right_srf,
				bs2_curve right_bs2,
				curve const &def_crv,
				SPAinterval	const &v_range,
    		    double left_off,
    		    double right_off,
				closed_forms u_closure = OPEN,
				closed_forms v_closure = CLOSURE_UNSET
				);

	// vertex-edge blend
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as 
 * arguments.
 * <br><br>
 * <b>Role:</b> Creates a vertex-edge blend. The constructor doesn't copy anything, but assumes
 * ownership of the data that is passed to it by pointer-namely the <tt>blend_supports</tt>, <tt>radius
 * functions</tt> and <tt>cross sections</tt>.
 * <br><br>
 * @param left_pt
 * left point.
 * @param right_crv
 * right curve.
 * @param def_crv
 * defining curve.
 * @param v_range
 * v range.
 * @param left_off
 * left off.
 * @param right_off
 * right off.
 * @param u_closure
 * u closure.
 * @param v_closure
 * v closure.
 */
	rb_blend_spl_sur(	
				SPAposition const &left_pt,
	    	    curve const &right_crv,
				curve const &def_crv,
				SPAinterval	const &v_range,
    		    double left_off,
    		    double right_off,
				closed_forms u_closure = OPEN,
				closed_forms v_closure = CLOSURE_UNSET
				);

	// vertex-vertex blend not required

	//  pipe
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as 
 * arguments.
 * <br><br>
 * <b>Role:</b> Creates a vertex-vertex blend. The constructor doesn't copy anything, but assumes
 * ownership of the data that is passed to it by pointer-namely the <tt>blend_supports</tt>, <tt>radius
 * functions</tt> and <tt>cross sections</tt>.
 * <br><br>
 * @param zero_crv
 * zero curve.
 * @param def_crv
 * defining curve.
 * @param v_range
 * v range.
 * @param offset
 * offset.
 * @param v_closure
 * v closure.
 */
	rb_blend_spl_sur(	
				curve const &zero_crv,
				curve const &def_crv,
				SPAinterval	const &v_range,
    		    double offset,
				closed_forms v_closure = CLOSURE_UNSET
    			);

	// Do-nothing destructor
/**
 * C++ destructor, deleting a <tt>rb_blend_spl_sur</tt>.
 */
	virtual ~rb_blend_spl_sur();

	// Almost all the virtual functions provided by blend_spl_sur are sufficient for
	// rb_blend_spl_sur. The only exceptions are evaluate and compute_section, and 
	// then mainly for reasons of efficiency.

	// Construct a duplicate in free store of this object but with
	// zero use count.
/**
 * Construct a duplicate in free store of this object but with zero use count.
 */
	virtual subtrans_object *copy() const;

	// Make a copy without sharing subdata.
/**
 * Creates a copy of an item that does not share any data with the original.
 * <br><br>
 * <b>Role:</b> Allocates new storage for all member data and any pointers.
 * Returns a pointer to the copied item.
 * <br><br>
 * @param pm
 * list of items within the entity that are already  deep copied.
 */
	virtual spl_sur *deep_copy( pointer_map *pm = NULL ) const;

	// General inquiry functions.

	// Query the parameter space range of the surface.

	// 	virtual SPApar_box  param_range( SPAbox const & = 
	//							*(SPAbox *)NULL_REF) const;
	// 	virtual SPAinterval param_range_u( SPAbox const & = 
	//							*(SPAbox *)NULL_REF );
	// 	virtual SPAinterval param_range_v( SPAbox const & = 
	//							*(SPAbox *)NULL_REF) const;

	// The u/v closure of the blend surface.

	// 	virtual logical periodic_u() const { return FALSE; }
	// 	virtual logical periodic_v() const;
	// 	virtual logical periodic_v();

	// 	virtual logical closed_u() const { return FALSE; }
	// 	virtual logical closed_v() const;
	// 	virtual logical closed_v();

	// u/v parameter period for a periodic blend surface.

	// 	virtual double param_period_u() const { return 0.0; }
	// 	virtual double param_period_v() const;

	// Test for equality. This does not guarantee that all
	// effectively equal surfaces are determined to be equal, but
	// does guarantee that different surfaces are correctly
	// identified as such.

	// logical operator==( subtype_object const & ) const;

	// Construct an iso-SPAparameter line on the surface. A u parameter 
	// line runs in the direction of increasing u SPAparameter, at constant 
	// v. A v SPAparameter line runs in the direction of increasing v, at
	// constant u. The parametrisation in the non-constant direction matches 
	// that of the surface, and has the range obtained by use of param_range_u() 
	// or param_range_v() appropriately.

	// curve *u_param_line( double v, spline const &owner ) const;

	// For v_param_line, we can't do any better than the parent.

	// curve *v_param_line( double u, spline const &owner ) const;

	// Bounding SPAbox. Normally the default is ok. Occasionally we make these
	// surfaces without bs3_surfaces just to support offset SSIs, so we can
	// do something more sensible here.

/**
 * Bounding box and normally the default is OK.
 * <br><br>
 * <b>Role:</b> Occasionally we make these surfaces without <tt>bs3_surfaces</tt> just to support offset
 * <tt>SSIs</tt>, so we can do something more sensible here.
 * <br><br>
 * @param box
 * bounding box.
 */
	virtual SPAbox bound( SPApar_box const &box );

/**
 * @nodoc
 */
	virtual void full_size( SizeAccumulator &, logical = TRUE ) const;

	// Blend specific inquiry functions.
/**
 * Returns the radius.
 */
	double radius() const { return left_offset; }

	// Same as the above non-virtual function.

	virtual double blend_radius( double ) const { return fabs( left_offset ); } 




	// General functions to modify the blend surface.

	// Transform this blend by the given SPAtransf.

	// virtual void operator*=( SPAtransf const &trf );

 	// Parameter shift: adjust the spline surface to have a SPAparameter
 	// range increased by the argument value (which may be negative).
 	// This is only used to move portions of a periodic surface by
 	// integral multiples of the period.

	// 	virtual void shift_u( double );
	// 	virtual void shift_v( double );

 	// Divide a surface into two pieces at a given SPAparameter value.
 	// If the split is at the end of the SPAparameter range, the spl_sur
 	// is just returned as the appropriate half (in increasing
 	// SPAparameter order), and the other is NULL. Otherwise a new spl_sur
 	// is used for one part, and the old one is modified for the other.

	// 	virtual void split_u( double, spl_sur *[ 2 ] );
	// 	virtual void split_v( double, spl_sur *[ 2 ] );

 	// Concatenate the contents of two surfaces into one. The surfaces
 	// are guaranteed to be the same base or derived type, and to have
 	// contiguous SPAparameter ranges ("this" is the beginning part of
 	// the combined surface (i.e. lower SPAparameter values), the
 	// argument gives the end part).

	// 	virtual void append_u( spl_sur & );
	// 	virtual void append_v( spl_sur & );




	// Functions for geometric evaluation of the blend surface.

	// The evaluate() function calculates derivatives, of any order
	// up to the number requested, and stores them in vectors provided
	// by the user. It returns the number it was able to calculate;
	// this will be equal to the number requested in all but the most
	// exceptional circumstances. A certain number will be evaluated
	// directly and (more or less) accurately; higher derivatives will
	// be automatically calculated by finite differencing; the accuracy
	// of these decreases with the order of the derivative, as the
	// cost increases.

/**
 * The <tt>evaluate</tt> function calculates derivatives, of any order up to the number requested, 
 * and stores them in vectors provided by the user.
 * <br><br>
 * <b>Role:</b> It returns the number it was able to calculate; this will be equal to the number
 * requested in all but the most exceptional circumstances. A certain number will be evaluated
 * directly and (more or less) accurately; higher derivatives will be automatically calculated by
 * finite differencing; the accuracy of these decreases with the order of the derivative, as the
 * cost increases.
 * <br><br>
 * @param val
 * given param value.
 * @param pos
 * evaluated position.
 * @param vec
 * derivative vectors.
 * @param no_der
 * number of derivatives requested.
 * @param eval_sur
 * eval surface quadrant.
 */
	virtual int evaluate(
				 SPApar_pos const &val,	 // uv parameter
				 SPAposition &pos,		 // Point on surface at given SPAparameter
				 SPAvector **vec = NULL, // Array of pointers to arrays of
										 // vectors, of size nd. Any of the
										 // pointers may be null, in which
										 // case the corresponding derivatives
										 // will not be returned. Otherwise
										 // they must point to arrays long
										 // enough for all the derivatives of
										 // that order - i.e. 2 for the first
										 // derivatives, 3 for the second, etc.
				 int num_der = 0,        // Number of derivatives required (nd)
				 evaluate_surface_quadrant eval_sur = 
					evaluate_surface_unknown
										 // The evaluation location - above,
										 // below for each SPAparameter direction,
										 // or don't care.
				 ) const;


	// Return the number of derivatives which evaluate() can find
	// "accurately" (and fairly directly), rather than by finite
	// differencing, over the given portion of the surface. If there
	// is no limit to the number of accurate derivatives, returns
	// the value ALL_SURFACE_DERIVATIVES, which is large enough to be
	// more than anyone could reasonably want.

/**
 * Returns the number of derivatives which evaluate can find precisely (and fairly directly), rather than by 
 * finite differencing, over the given portion of the surface.
 * <br><br>
 * <b>Role:</b> If there is no limit to the number of accurate derivatives, returns the value
 * <tt>ALL_SURFACE_DERIVATIVES</tt>, which is large enough to be more than anyone could reasonably want.
 * <br><br>
 * @param box
 * parameter box.
 */
	virtual int accurate_derivs(
						 SPApar_box const &box = 
							*(SPApar_box *)NULL_REF // Defaults to the whole surface
						 ) const;

 	// Find the SPAposition and first and second  derivatives of the
 	// surface at a given point.

	// 	virtual void eval(
	// 				SPApar_pos const &uv,
	// 				SPAposition &pos,
	// 				SPAvector *dpos,	// first derivatives - array of
	// 								// length 2 in order xu, xv
	// 				SPAvector *ddpos	// second derivatives - array of
	// 								// length 3 in order xuu, xuv, xvv
	// 				) const;

 	// Find the point on the surface with given SPAparameter values.
 	// The default uses eval().

	//	virtual SPAposition eval_position( SPApar_pos const & ) const;

  	// Find the normal to the surface at the point with given
 	// parameter values.

	// virtual SPAunit_vector eval_normal( SPApar_pos const & ) const;


	// Find the principal axes of curvature of the surface at a
	// point with given SPAparameter values, and the curvatures in those
	// directions.
/**
 * Finds the principal axes of curvature of the surface at a point with given parameter values, 
 * and the curvatures in those directions.
 * <br><br>
 * @param uv
 * uv parameter position.
 * @param u1
 * first axis direction.
 * @param c1
 * curvature in first direction.
 * @param u2
 * second axis direction.
 * @param c2
 * curvature in second direction.
 */
	virtual void eval_prin_curv(
					SPApar_pos const &uv,
					SPAunit_vector &u1,		// first axis direction
					double &c1,				// curvature in first direction
					SPAunit_vector &u2,		// second axis direction
					double &c2,				// curvature in second direction
					evaluate_surface_quadrant = evaluate_surface_unknown
					) const;

 	// Find the curvature of a cross-section curve of the surface at
 	// the point on the surface with given SPAparameter values.
 	// The cross-section curve is defined as the intersection of
 	// the surface with a plane passing through the point on the
 	// surface and normal to the given direction, which must lie in
 	// the surface.

	// virtual double eval_cross(
	//						SPApar_pos const &,
	//						SPAunit_vector const &
	//						) const;

    // A form of evaluation specific to blend_spl_surs (certain
    // numberical algorithms used by blending need this function).
	// Evaluates spine, defining curve, contact points and their
	// derivatives at the given v SPAparameter, according to the
	// blend_section class declaration as above. We may specify exactly
	// how may spine and spring curve derivatives we require. As the two
	// are typically connected you may get more than you asked for, but
	// you are guaranteed to get at least what you ask for.
	// Implementations of this should also ensure it does no more than
	// is necessary. Finally the logical flag indicates whether you
	// require the cross curve normal filled in; again this may (will)
	// have implications on the amount of other stuff you get back, but
	// if passed as TRUE then this is guaranteed to be returned. Note
	// that calling this with for example -1, -1 and TRUE is valid.

    // This is pretty trivial for constant rolling ball surfaces.

/**
 * A form of evaluation specific to <tt>blend_spl_surs</tt> (certain numerical algorithms used by blending need this function).
 * <br><br>
 * <b>Role:</b> Evaluates spine, defining curve, contact points and their derivatives at the given
 * v-parameter, according to the <tt>blend_section</tt> class declaration as above. We may specify exactly
 * how may spine and spring curve derivatives we require. As the two are typically connected you
 * may get more than you asked for, but you are guaranteed to get at least what you ask for.
 * Implementations of this should also ensure it does no more than is necessary. Finally the
 * logical flag indicates whether you require the cross curve normal filled in; again this may
 * (will) have implications on the amount of other stuff you get back, but if passed as <tt>TRUE</tt> then
 * this is guaranteed to be returned. Note that calling this with for example <tt>-1</tt>, <tt>-1</tt> and <tt>TRUE</tt> is
 * valid.
* <br><br>
 * @param v
 * v parameter.
 * @param spine_nder
 * number of required spine derivatives.
 * @param spring_nder
 * number of required spring derivs.
 * @param xcrv_norm
 * whether to fill in xcurve normal.
 * @param section
 * all output in here.
 * @param eval_loc
 * evaluation location 1 => above,-1 => below,0 => don't care.
 */
    virtual void compute_section(
						 double v,		// v SPAparameter
						 int spine_nder,	// number of required spine derivs
						 int spring_nder,	// no. of req'd spring derivs
						 logical xcrv_norm,	// whether to fill in xcurve normal
						 blend_section &section,	// all output in here
						 int eval_loc = 0			 // the evaluation location - 1 => above,
						                 // -1 => below, 0 => don't care.
						 ) const;

	// Find the angle between perps to supports at def_cvec, according to the rules
	// i) pipes return two pi
	// ii) singularities return 0
	// iii) 0 <= ang <= two pi
	// nb cvec provides Tangent to complete co-ordinate system

/**
 * Finds the angle between perpendiculars to supports at <tt>def_cvec</tt>, according to the rules.
 * <br><br>
 * <b>Role:</b>  Finds the angle between perpendiculars to supports at <tt>def_cvec</tt>, according
 * to the rules.
 * <pre>
 * <br>
 * 1.	pipes return two pi
 * 2.	singularities return 0
 * 3.	0 <= ang <= two pi
 * 4.	nb cvec provides tangent to complete coordinate system
 * </pre>
 * @param P
 * pipe position.
 * @param Tan
 * tangent vector.
 * @param R0
 * 1st radius vector.
 * @param R1
 * 2nd radius vector.
 * @param rr_sina
 * radius rule sin angle.
 * @param rr_cosa
 * radius rule cos angle.
 */
	//	double blend_total_angle( 
	//						SPAposition &P, 
	//						SPAunit_vector &Tan,
	//						SPAvector const &R0, 
	//						SPAvector const &R1,
	//						double &rr_sina = *( double * )NULL_REF,
	//						double &rr_cosa = *( double * ) NULL_REF 
	//						) const;

	// Find the angle between two radius vectors at def_cvec, according to 
	// the rule
	// 0 <= ang < two pi - SPAresnor
	// nb cvec provides Tangent to complete co-ordinate system
/**
 * Finds the angle between two radius vectors at def_cvec, according to the rule.
 * <br><br>
 * <b>Role:</b>  Finds the angle between two radius vectors at <tt>def_cvec</tt>, according to the rule.
 *<pre>
 * <br>
 * 1.	0 <= ang < two pi - SPAresnor
 * 2.	nb cvec provides tangent to complete coordinate system
 * </pre>
 * <br><br>
 * @param Tan
 * tangent vector.
 * @param R0
 * 1st radius vector.
 * @param R1
 * 2nd radius vector.
 * @param rr_sina
 * radius rule sin angle.
 * @param rr_cosa
 * radius rule cos angle.
 */
	// double blend_angle( 
	//				SPAunit_vector &Tan, 
	//				SPAvector const &R0, 
	//				SPAvector const &R1,
	//				double &rr_sina = *( double * )NULL_REF,
	//				double &rr_cosa = *( double * ) NULL_REF 
	//				) const;





	// Functions for inversion of object space points.

	// The inverting functions: point_normal(), point_prin_curve(),
	// point_cross(), point_outdir(), point_perp() and param(),
	// all eventually call point_perp() to do the actual inversion.
	// point_perp() can be optimized for our particular surface,
	// and then using the defaults for the other inverting functions
	// will be optimized as much as possible.

	// Find the point on the surface nearest to the given point,
	// iterating from the given SPAparameter values (if supplied).
	// Return the found point, the normal to the surface at that
	// point and the SPAparameter values at the found point.
//
/**
 * Finds the point on the surface nearest to the given point, iterating from the given parameter values (if supplied).
 * <br><br>
 * <b>Role:</b> Returns the found point, the normal to the surface at that point and the parameter
 * values at the found point.
 * <br><br>
 * @param point
 * point.
 * @param foot
 * foot.
 * @param norm
 * normal.
 * @param curv
 * curve.
 * @param uv_guess
 * uv guess.
 * @param uv_actual
 * uv actual.
 * @param f_weak
 * weak flag.
 * @param subset_range
 * subset range.
 */
	virtual void point_perp(
 					SPAposition const &point,
 					SPAposition &foot,
 					SPAunit_vector &norm,
 					surf_princurv &curv,
 					SPApar_pos const &uv_guess = 
						* (SPApar_pos *) NULL_REF,
 					SPApar_pos &uv_actual = 
						* (SPApar_pos *) NULL_REF,
					logical f_weak = FALSE,
					SPApar_box const &subset_range = 
						* (SPApar_box *) NULL_REF
				  ) const;


	// A version of point_perp to support SVEC::relax - it doesn't pull the
	// relaxed parameters back to range.
/**
 * A version of <tt>point_perp</tt> to support <tt>SVEC::relax</tt> - it doesn't pull 
 * the relaxed parameters back to range.
 * <br><br>
 * @param point
 * position.
 * @param sv
 * svec.
 */
	logical	relax( SPAposition const &pos, SVEC &sv );

	// Finds the SPAparameter values of a point on the surface.
/**
 * Returns the parameter.
 * <br><br>
 * @param pos
 * position.
 * @param param_pos
 * parameter position.
 */

	virtual SPApar_pos param(
						  SPAposition const &pos,
						  SPApar_pos const &param_pos = 
							*(SPApar_pos *)NULL_REF
						  ) const;

 	// Find the change in surface SPAparameter corresponding to a unit
 	// offset in a given direction at a given uv, the direction
 	// lying in the surface.

	// virtual SPApar_vec param_unitvec(
	//								SPAunit_vector const &,
	//								SPApar_pos const &
	//								) const;


	// Find the normal to the surface at a point on the surface
	// nearest to the given point.

	//	virtual SPAunit_vector point_normal(	
	//								SPAposition const &,
	//								SPApar_pos const & = 
	//									*(SPApar_pos *)NULL_REF
	//								) const;


	// Find the principal axes of curvature of the surface at a
	// given point, and the curvatures in those directions.

	//	virtual void point_prin_curv(
	//					SPAposition const &,
	//					SPAunit_vector &,	// first axis direction
	//					double &,			// curvature in first direction
	//					SPAunit_vector &,	// second axis direction
	//					double &,			// curvature in second direction
	//					SPApar_pos const & = *(SPApar_pos *)NULL_REF,
	//                  evaluate_surface_quadrant = evaluate_surface_unknown
	//					) const;

	// Find the curvature of a cross-section curve of the surface at
	// the given point.  The cross-section curve is given by the
	// intersection of the surface with a plane passing through the
	// given point and with given normal. The default using
	// point_prin_curv is fine.

	//	virtual double point_cross(
	//						SPAposition const &,
	//						SPAunit_vector const &,
	//						SPApar_pos const & = *(SPApar_pos *)NULL_REF
	//						) const;

	// Test whether a point lies on the surface.

	// virtual logical test_point_tol(
	//					SPAposition const &,
	//					double,
	//					SPApar_pos const & = *(SPApar_pos *)NULL_REF,
	//					SPApar_pos & = *(SPApar_pos *)NULL_REF
	//					) const;





	// Functions for generating the three-curve evaluator.
/**
 * @nodoc
 */
	virtual void enable_new_evaluator( logical enable = TRUE );

	virtual SPAcrvGeom *make_crvGeom_object( 
										curve *curve_in = NULL,
                                        int num_seeds = 0, 
										int *cont = NULL, 
										double *seeds = NULL 
										) const;

	virtual SPAcrvGeom *get_spine_crvGeom( 
										int nseeds = 0, 
										int *cont = NULL, 
										double *seeds = NULL
										);





	// Save and restore. Save is easy, as derived class switching goes
	// through the normal virtual function mechanism. Restore is more
	// complicated, because until it is invoked we do not have an
	// object of the right class. Instead we switch on a table defined
	// by static instances of the restore_ss_def class (see below), to
	// invoke a simple friend function which constructs an object of
	// the right (derived) type. Then it can call the appropriate
	// member function to do the actual work.

/**
 * Returns the ID for the <tt>rb_blend_spl_sur</tt> list.
 */
	static int id();

/**
 * Returns the type of <tt>rb_blend_spl_sur</tt>.
 */
	virtual int type() const;

/**
 * Returns the string <tt>"rbblnsur"</tt>.
 */
	virtual char const *type_name() const;

	// Whether the surface needs to be saved as a bspline approximation 
	// for the given bspline version.
/**
 * @nodoc
 */
	virtual logical need_save_as_approx( 
									int save_to_version, 
									logical check_progenitors
								  ) const;

/**
 * @nodoc
 */
	virtual void save() const;

	//	virtual void save_data() const;

/**
 * @nodoc
 */
	friend DECL_KERN subtype_object *restore_rb_blend_spl_sur();

	/**
	* @nodoc
	**/
	virtual void process( geometry_definition_processor& p ) const;




#if defined D3_STANDALONE || defined D3_DEBUG

	// D3 specific surface debug functions.

/**
 * @nodoc
 */
	friend DECL_KERN D3_istream &operator>>( D3_istream &, spl_sur *& );

/**
 * @nodoc
 */
	virtual void input( D3_istream & );

/**
 * @nodoc
 */
	virtual void print( D3_ostream & ) const;

#endif

	// Debug printout. As for save and restore we split the operation
	// into two parts: the virtual function "debug" prints a class-
	// specific identifying line, then calls the ordinary function
	// "debug_data" to put out the details. It is done this way so that
	// a derived class' debug_data can call its parent's version first,
	// to put out the common data. Indeed, if the derived class has no
	// additional data it need not define its own version of debug_data
	// and use its parent's instead. A string argument provides the
	// introduction to each displayed line after the first, and a
	// logical sets "brief" output (normally removing detailed
	// subsidiary curve and surface definitions).

/**
 * Debug printout.
 * <br><br>
 * <b>Role:</b> The virtual function <tt>debug</tt> prints a class-specific identifying line, then calls
 * the ordinary function <tt>debug_data</tt> to put out the details. It is done this way so that a derived
 * class' <tt>debug_data</tt> can call its parent's version first, to put out the common data. Indeed, if
 * the derived class has no additional data it need not define its own version of <tt>debug_data</tt> and
 * use its parent's instead. A string argument provides the introduction to each displayed line
 * after the first, and a logical sets brief output (normally removing detailed subsidiary curve
 * and surface definitions).
 * <br><br>
 * @param line
 * loader line.
 * @param op_flag
 * brief output flag.
 * @param file
 * file.
 */
	virtual void debug( char const *line, logical op_flag, FILE *file ) const;

	};

/** @} */
#endif
