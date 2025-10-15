/* ORIGINAL: acis2.1/kerngeom/splsur/exct_spl.hxx */
/* $Id: exct_spl.hxx,v 1.21 2002/08/09 17:15:25 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Class to represent an exact spline surface. This is derived from
// the base class spl_sur, which is used by the spline surface class
// to contain the surface description proper.

#if !defined( exact_spl_sur_CLASS )
#define exact_spl_sur_CLASS

#include <stdio.h>

#include "dcl_kern.h"
#include "spldef.hxx"

// STI ROLL
class SizeAccumulator;
// STI ROLL

DECL_KERN subtype_object *restore_exact_spl_sur();
#if defined D3_STANDALONE || defined D3_DEBUG
DECL_KERN D3_istream& operator>>( D3_istream &, spl_sur *& );
DECL_KERN D3_ostream& operator<<( D3_ostream &, spl_sur const & );
#endif

class DECL_KERN exact_spl_sur: public spl_sur {

// Allow extensions to declare themselves as friends. USE WITH EXTREME CAUTION
#ifdef exact_spl_sur_FRIENDS
exact_spl_sur_FRIENDS
#endif

protected:

	// Store the original (unextended) range of the surface, when the surface
	// gets extended. If the _unextended_range is empty, then it implies that
	// the surface has not been extended. 

	SPApar_box	_unextended_range;

    // Return whether this spl_sur is precisely defined by its bs3_surface
    // i.e. there is never a difference between evaluating the spl_sur and
    // evaluating the bs3_surface.
    virtual logical is_exactly_bs3() const;

    // Calculate the discontinuity information if it was never stored.  This
    // function is intended to support restore of old versions of splines. 

    virtual	void calculate_disc_info();


	// Constructors.

	// Construct a generally null object, preparatory to filling it
	// in.

	exact_spl_sur();

    // Override of spl_sur::make_approx. It is possible for an extended rational
    // exact spline to have a non-zero fit tolerance, but we must not generate
    // an approximation, which will then replace the 'real' spline and change
    // its shape. This override does nothing, even if the force flag is set.
    // (although we could make it compute a better approximation in the extended
    // regions of extended rationals)
    virtual void	make_approx(
							 double fit,
							 const spline& spl = *(spline*) NULL_REF,
						     logical force = FALSE
						 ) const;

public:

	// A constructor given all the basic data required. Used by the
	// corresponding spline constructor.

	exact_spl_sur( bs3_surface );

	// Copy constructor. 

	exact_spl_sur( const exact_spl_sur& );

	// Make a copy without sharing subdata.

	virtual spl_sur *deep_copy(pointer_map * pm = NULL) const;

	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL

protected:

	// The destructor eliminates all the dependent spline curve and
	// surface data. Each derived class must have a destructor if it
	// adds further dependent data. This one doesn't need one but it
	// is virtual, so we declare it anyway.

	virtual ~exact_spl_sur();

	// Duplication. Can't be done by constructor, as we want it
	// to be virtual.

	virtual subtrans_object *copy() const;


	// Test for equality. This does not guarantee that all
	// effectively equal surfaces are determined to be equal, but
	// does guarantee that different surfaces are correctly
	// identified as such. The base class (spl_sur) version is fine
	// for us here.

//	logical operator==( subtype_object const & ) const;


	// Transformation. The base class transforms the spline and fit
	// tolerance, and this is all we need.

//	virtual void operator*=( SPAtransf const & );

	// Parameter shift: adjust the spline surface to have a SPAparameter
	// range increased by the argument value (which may be negative).
	// This is only used to move portions of a periodic surface by
	// integral multiples of the period. The base class implementation
	// is fine.

//	virtual void shift_u( double );
//	virtual void shift_v( double );

	// Perform a linear transformation on the parametrisation, so that
	// it starts and ends at the given values (which must be in increasing
	// order).

	virtual void reparam_u(
				double,		// new start u SPAparameter
				double		// new end u SPAparameter
			);
	virtual void reparam_v(
				double,		// new start v SPAparameter
				double		// new end v SPAparameter
			);
	virtual void reparam(
				double,		// new start u SPAparameter
				double,		// new end u SPAparameter
				double,		// new start v SPAparameter
				double		// new end v SPAparameter
			);


	// Divide a surface into two pieces at a given SPAparameter value.
	// If the split is at the end of the SPAparameter range, the spl_sur
	// is just returned as the appropriate half (in increasing
	// SPAparameter order), and the other is NULL. Otherwise a new spl_sur
	// is used for one part, and the old one is modified for the other.

	virtual void split_u(
				double,
				spl_sur *[ 2 ]
			);
	virtual void split_v(
				double,
				spl_sur *[ 2 ]
			);


	// Concatenate the contents of two surfaces into one. The surfaces
	// are guaranteed to be the same base or derived type, and to have
	// contiguous SPAparameter ranges ("this" is the beginning part of
	// the combined surface (i.e. lower SPAparameter values), the
	// argument gives the end part). The base class implementation
	// is fine.

//	virtual void append_u( spl_sur & );
//	virtual void append_v( spl_sur & );


	// Geometric evaluation.

	// Evaluate the normal at the given point. The base class finds
	// the SPAparameter value, then evaluates the surface there. This is
	// fine for us.

//	virtual SPAunit_vector point_normal(
//				SPAposition const &,
//				SPApar_pos const & = *(SPApar_pos *)NULL_REF
//			) const;

	
	// Find the principal axes of curvature of the surface at a
	// given point, and the curvatures in those directions.

//	virtual void point_prin_curv(
//				SPAposition const &,
//				SPAunit_vector &,		// first axis direction
//				double &,			// curvature in first direction
//				SPAunit_vector &,		// second axis direction
//				double &,			// curvature in second direction
//				SPApar_pos const & = *(SPApar_pos *)NULL_REF,
//              evaluate_surface_quadrant = evaluate_surface_unknown
//			) const;


	// Find the curvature of a cross-section curve of the surface at
	// the point on the surface closest to the given point, iterating 
	// from the given SPAparameter values (if supplied).
	// The cross-section curve is determined by the intersection of 
	// the surface with a plane passing through the point on the
	// surface and with given normal.

//	virtual double point_cross(
//				SPAposition const &,
//				SPAunit_vector const &,
//				SPApar_pos const & = *(SPApar_pos *)NULL_REF
//			) const;


	// Find the point on the surface nearest to the given point, 
	// iterating from the given SPAparameter values (if supplied).
	// Return the found point, the normal to the surface at that 
	// point and the SPAparameter values at the found point.

//	virtual void point_perp(
//				SPAposition const &,
//				SPAposition &,
//				SPAunit_vector &,
//				SPApar_pos const & = *(SPApar_pos *)NULL_REF,
//				SPApar_pos & = *(SPApar_pos *)NULL_REF
//			) const;

	// Find the SPAparameter values of a point on the surface.

	virtual SPApar_pos param(
				SPAposition const &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF
			) const;


	// Find the change in surface SPAparameter corresponding to a unit
	// offset in a given direction at a given SPAposition, the SPAposition
	// and direction both lying in the surface.

//	virtual SPApar_vec param_unitvec(
//				SPAunit_vector const &,
//				SPApar_pos const &
//			) const;


	// Find the SPAposition and first and second  derivatives of the
	// surface at a given point.

	virtual void eval(
				SPApar_pos const &uv,
				SPAposition &pos,
				SPAvector *dpos,	// first derivatives - array of
								// length 2 in order xu, xv
				SPAvector *ddpos	// second derivatives - array of
								// length 3 in order xuu, xuv, xvv
			) const;


	// Find the point on the surface with given SPAparameter values.
	// The default uses eval().

//	virtual SPAposition eval_position(
//				SPApar_pos const &
//			) const;


 	// Find the normal to the surface at the point with given 
	// SPAparameter values. The default uses eval().

//	virtual SPAunit_vector eval_normal(
//				SPApar_pos const &
//			) const;


	// Find the principal axes of curvature of the surface at a
	// point with given SPAparameter values, and the curvatures in those
	// directions. The default uses eval().

//	virtual void eval_prin_curv(
//				SPApar_pos const &,
//				SPAunit_vector &,		// first axis direction
//				double &,			// curvature in first direction
//				SPAunit_vector &,		// second axis direction
//				double &,			// curvature in second direction
//              evaluate_surface_quadrant = evaluate_surface_unknown
//			) const;


	// Find the curvature of a cross-section curve of the surface at
	// the point on the surface with given SPAparameter values.
	// The cross-section curve is defined as the intersection of 
	// the surface with a plane passing through the point on the
	// surface and normal to the given direction, which must lie in
	// the surface. The default uses eval_prin_curv - here we prefer
	// to use eval() directly.

	virtual double eval_cross(
				SPApar_pos const &,
				SPAunit_vector const &
			) const;

	// The evaluate() function calculates derivatives, of any order
	// up to the number requested, and stores them in vectors provided
	// by the user. It returns the number it was able to calculate;
	// this will be equal to the number requested in all but the most
	// exceptional circumstances. A certain number will be evaluated
	// directly and (more or less) accurately; higher derivatives will
	// be automatically calculated by finite differencing; the accuracy
	// of these decreases with the order of the derivative, as the
	// cost increases.

	virtual int evaluate(
                SPApar_pos const &,	// Parameter
                SPAposition &,			// Point on surface at given SPAparameter
                SPAvector ** = NULL, 	// Array of pointers to arrays of
									// vectors, of size nd. Any of the
									// pointers may be null, in which
									// case the corresponding derivatives
									// will not be returned. Otherwise
									// they must point to arrays long
									// enough for all the derivatives of
									// that order - i.e. 2 for the first
									// derivatives, 3 for the second, etc.
                int = 0,       		// Number of derivatives required (nd) 
				evaluate_surface_quadrant = evaluate_surface_unknown
									// the evaluation location - above,
									// below for each SPAparameter direction,
									// or don't care.
            ) const;


	// Return the number of derivatives which evaluate() can find
	// "accurately" (and fairly directly), rather than by finite
	// differencing, over the given portion of the surface. If there
	// is no limit to the number of accurate derivatives, returns
	// the value ALL_SURFACE_DERIVATIVES, which is large enough to be
	// more than anyone could reasonably want.

	virtual int accurate_derivs( 
				SPApar_box const & = *(SPApar_box *)NULL_REF
								 	// Defaults to the whole surface
			) const;


	// Test whether a point lies on the surface, within a user-defined
	// tolerance. The base class implementation is as good as we could
	// do ourselves.

//	virtual logical test_point_tol(
//				SPAposition const &,
//				double,
//				SPApar_pos const & = *(SPApar_pos *)NULL_REF,
//				SPApar_pos & = *(SPApar_pos *)NULL_REF
//			) const;


	// Save and restore. Save is easy, as derived class switching goes
	// through the normal virtual function mechanism. Restore is more
	// complicated, because until it is invoked we do not have an
	// object of the right class. Instead we switch on a table defined
	// by static instances of the restore_ss_def class (see below), to
	// invoke a simple friend function which constructs an object of
	// the right (derived) type. Then it can call the appropriate
	// member function to do the actual work.

public:

	static int id();
	virtual int type() const;

	virtual char const *type_name() const;

	virtual void save_data() const;

	/**
	* @nodoc
	**/
	virtual void process( geometry_definition_processor& p ) const;
protected:

	friend DECL_KERN subtype_object *restore_exact_spl_sur();
	void restore_data();

	friend class ERROR_FUNCTION;

#if defined D3_STANDALONE || defined D3_DEBUG

	friend DECL_KERN D3_istream& operator>>(
				D3_istream &,
				spl_sur *&
			);

	friend DECL_KERN D3_ostream& operator<<(
				D3_ostream &,
				spl_sur const &
			);

	virtual void input(
				D3_istream &
			);

	virtual void print(
				D3_ostream &
			) const;

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

	virtual void debug(
				char const *,
				logical,
				FILE *
			) const;
	void debug_data(
				char const *,
				logical,
				FILE *
			) const;
};

#endif
