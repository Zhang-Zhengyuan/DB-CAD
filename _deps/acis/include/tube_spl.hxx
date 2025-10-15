/* ORIGINAL: acis2.1/kerngeom/splsur/tube_spl.hxx */
/* $Id: tube_spl.hxx,v 1.20 2002/08/20 15:20:22 jenglund Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for a tube, a class derived from spl_sur, that represents
// a surface which is the envelope of a fixed-radius circle centred
// on a point on a given curve, and normal to the curve at that
// point.

#if !defined( tube_spl_sur_CLASS )
#define tube_spl_sur_CLASS

#include "logical.h"

#include "acis.hxx"
#include "dcl_kern.h"

#include "spldef.hxx"

#include "debugmsc.hxx"

class curve;
class SPApar_pos;
class SPAposition;
class SPAvector;
class SPAunit_vector;
class SPAtransf;

// STI ROLL
class SizeAccumulator;
// STI ROLL

class SPAbox;
class SPApar_box;

DECL_KERN subtype_object *restore_tube_spl_sur();
#if defined D3_STANDALONE || defined D3_DEBUG
DECL_KERN D3_istream& operator>>( D3_istream &, spl_sur *& );
DECL_KERN D3_ostream& operator<<( D3_ostream &, spl_sur const & );
#endif

class DECL_KERN tube_spl_sur: public spl_sur {

protected:

	double radius;		// positive for a convex tube
	curve *spine;		// path traced out by centre of circle


	// Constructors.

	// Construct a generally null tube. This is to allow flexibility
	// for constructors for derived classes to set the common data
	// members in whatever way is most convenient.

	tube_spl_sur();

public:
	// Construct a tube with given radius and spine curve when the
	// interpolating surface has already been found.  Makes a copy
	// of the given curve.

	tube_spl_sur(
			double,				// radius of tube
			curve *,			// spine curve
			bs3_surface,		// approximating surface
			double				// fitol for surface

		);

	// Copy constructor. 

	tube_spl_sur( const tube_spl_sur& );

	// Make a copy without sharing subdata.

	virtual spl_sur *deep_copy(pointer_map * pm = NULL) const;

	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL
/**
 * @nodoc
 */
	virtual void minimize( minimize_helper*);

protected:
	// Destructor needs to delete the spine curve.

	virtual ~tube_spl_sur();

    // Function to determine if this spl_sur is a pipe or is a wrapper
	// around a pipe surface (eg offset or subset pipe). This function
	// is used to avoid using the bad parametrisation of pipe surfaces
	// if possible. A tube surface's parametrisation is as bad as a
	// pipe's, so we are going to cheat and return TRUE here.

    virtual logical contains_pipe() const
	    { return TRUE; }

	// Construct a duplicate in free store of this object but with
	// zero use count.

	virtual subtrans_object *copy() const;


	// Test for equality. This does not guarantee that all
	// effectively equal surfaces are determined to be equal, but
	// does guarantee that different surfaces are correctly
	// identified as such.

	logical operator==( subtype_object const & ) const;


	// Transform this tube by the given SPAtransf.

	virtual void operator*=( SPAtransf const &	);

	// Parameter shift: adjust the spline surface to have a SPAparameter
	// range increased by the argument value (which may be negative).
	// This is only used to move portions of a periodic surface by
	// integral multiples of the period. The base class implementation
	// is fine.

//	virtual void shift_u( double );
//	virtual void shift_v( double );


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

	// Find the normal to the surface at a point on the surface 
	// nearest to the given point.

	virtual SPAunit_vector point_normal(
				SPAposition const &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF
			) const;


	// Find the principal axes of curvature of the surface at a
	// given point, and the curvatures in those directions.

	virtual void point_prin_curv(
				SPAposition const &,
				SPAunit_vector &,		// first axis direction
				double &,			// curvature in first direction
				SPAunit_vector &,		// second axis direction
				double &,			// curvature in second direction
				SPApar_pos const & = *(SPApar_pos *)NULL_REF,
                evaluate_surface_quadrant = evaluate_surface_unknown
			) const;


	// Find the curvature of a cross-section curve of the surface at
	// the given point.  The cross-section curve is given by the
	// intersection of the surface with a plane passing through the 
	// given point and with given normal. The default using
	// point_prin_curv is fine.

//	virtual double point_cross(
//				SPAposition const &,
//				SPAunit_vector const &,
//				SPApar_pos const & = *(SPApar_pos *)NULL_REF
//			) const;


	// Find the point on the surface nearest to the given point, 
	// iterating from the given SPAparameter values (if supplied).
	// Return the found point, the normal to the surface at that 
	// point and the SPAparameter values at the found point.

	virtual void point_perp(
				SPAposition const &,
				SPAposition &,
				SPAunit_vector &,
				surf_princurv &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF,
				SPApar_pos & = *(SPApar_pos *)NULL_REF,
				logical f_weak = FALSE,
			    SPApar_box const & = *(SPApar_box *)NULL_REF
			) const;


	// Find the SPAparameter values of a point on the surface. We use
	// a slightly different parametrisation from standard, to speed
	// up evaluation: the SPAparameter value is that for the intersection
	// of the approximating surface with the true surface normal at
	// the given point.

	virtual SPApar_pos param(
				SPAposition const &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF
			) const;

	// Find the change in surface SPAparameter corresponding to a unit
	// offset in a given direction at a given uv, the direction
	// lying in the surface.

	virtual SPApar_vec param_unitvec(
				SPAunit_vector const &,
				SPApar_pos const &
			) const;

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
	// SPAparameter values.

	virtual SPAunit_vector eval_normal(
				SPApar_pos const &
			) const;

	// Find the principal axes of curvature of the surface at a
	// point with given SPAparameter values, and the curvatures in those
	// directions.

	virtual void eval_prin_curv(
				SPApar_pos const &,
				SPAunit_vector &,		// first axis direction
				double &,			// curvature in first direction
				SPAunit_vector &,		// second axis direction
				double &,			// curvature in second direction
                evaluate_surface_quadrant = evaluate_surface_unknown
			) const;

	// Find the curvature of a cross-section curve of the surface at
	// the point on the surface with given SPAparameter values.
	// The cross-section curve is defined as the intersection of 
	// the surface with a plane passing through the point on the
	// surface and normal to the given direction, which must lie in
	// the surface.

	virtual double eval_cross(
				SPApar_pos const &,
				SPAunit_vector const &
			) const;

	// Test whether a point lies on the surface.

	virtual logical test_point_tol(
				SPAposition const &,
				double,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF,
				SPApar_pos & = *(SPApar_pos *)NULL_REF
			) const;


	// Save and restore. Save is easy, as derived class switching goes
	// through the normal virtual function mechanism. Restore is more
	// complicated, because until it is invoked we do not have an
	// object of the right class. Instead we switch on a table defined
	// by static instances of the restore_ss_def class (see below), to
	// invoke a simple friend function which constructs an object of
	// the right (derived) type. Then it can call the appropriate
	// member function to do the actual work.

	/***
	* @nodoc
	**/
	void process( geometry_definition_processor& p ) const;

public:
	static int id();
	virtual int type() const;

	virtual char const *type_name() const;

	virtual void save_data() const;

	virtual logical need_save_as_approx(int save_to_version, logical check_progenitors) const;

protected:
	friend DECL_KERN subtype_object *restore_tube_spl_sur();
	void restore_data();


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
DECL_KERN subtype_object *restore_tube_spl_sur();

#endif
