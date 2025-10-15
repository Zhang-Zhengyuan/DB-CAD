/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Define a curve, which is the subset( could be super-subset) of a curve.

#if !defined( subset_int_cur_CLASS)
#define subset_int_cur_CLASS

#include "acis.hxx"
#include "dcl_kern.h"
#include "logical.h"

#include "intdef.hxx"

#include "debugmsc.hxx"

/**
* @file sub_int.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISCURVES
 *
 * @{
 */

class curve;

class SPAposition;
class SPAvector;
class SPAunit_vector;
class SPAtransf;
class SPAinterval;

class SPAparameter;

// STI ROLL
class SizeAccumulator;
// STI ROLL
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN subtype_object *restore_subset_int_cur();
/**
 * Number specifies the extension type.
 * @param EXTEND_CURVATURE
 * extend the curvature
 * @param EXTEND_TANGENT
 * extend the tangent
 */
typedef enum {
	EXTEND_CURVATURE = 0,
	EXTEND_TANGENT = 1
} CURVE_EXTENSION_TYPE;

// The class definition itself.
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN subset_int_cur: public int_cur {

// Allow extensions to declare themselves as friends. USE WITH EXTREME CAUTION
#ifdef subset_int_cur_FRIENDS
subset_int_cur_FRIENDS
#endif

	// method of extension
	CURVE_EXTENSION_TYPE extension_type;

	// original curve which is to be extended
	curve *progen_curve;

private:

	// Private member which makes an approximate curve for the
	// given curve-type.  This method takes the old and new ranges
	// and construct a bs3_curve approximating the given curve type
	// to the new range requested.  This should be called after the
	// private data in the class is set as this routine blindly uses
	// it.
	bs3_curve
	make_sub_approx(
			SPAinterval const &	// New range
	);


	// Private constructor, usually used in copy and split member
	// functions.
	subset_int_cur() { extension_type = EXTEND_CURVATURE;
					   progen_curve = NULL;};

public:

	// Virtual function which extends the given curve only
	// curvature continuously.  Alan does not like this way, so
	// we are implementing a external function called as sg_extend_curve
	// which does the job.
	// virtual int_cur * extend(
	// 					curve const 	&progen_curve, 	// Progenitor
	// 					SPAinterval const 	&n_range		// New range
	// 				);


	// Main constructor, This takes the input; a progenitor curve whichs
	// is to be extended, the progenitor range, the new range which is the
	// range of the extended curve, and finally the extension type.
	// The progenitor is extended parametrically on either/both ends to
	// match the requested range.  The extension types allowed are curvature
	// continuous and tangent continuous.
	subset_int_cur(
			curve const &,				// progenitor curve
			SPAinterval const &,			// Range of the progenitor
			SPAinterval const &,			// New range wanted( extend or trim ).
			CURVE_EXTENSION_TYPE const &// extension type
	);

	// Copy constructor. This has been added so that data in the int_cur class
	// can be kept up to date when the curve is copied. This constructor must
	// call the base class copy constructor, and the copy function must use this
	// copy constructor. PC  23-Nov-95

	subset_int_cur( const subset_int_cur& );

	// PC end

	// The deep_copy method makes a copy without shared data

	virtual int_cur *deep_copy(pointer_map * pm = NULL) const;

	/**
	* @nodoc
	**/
	virtual void process( geometry_definition_processor& p ) const;

	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL
/**
 * @nodoc
 */
	virtual void minimize( minimize_helper*);

	// STI rr 5862 (08/08/2003) begin: Return the progenitor curve and the extension type to caller. Now used by
	// LOP code rep_rb.cpp and sweep extension code swp_sext.cpp.
	void get_progen_curve(curve *&prog_curve, CURVE_EXTENSION_TYPE &ext_type = *(CURVE_EXTENSION_TYPE*)NULL_REF);
	// STI rr end.
private:

	// Destructor to remove the copy of the original curve which is
	// subsetted

	~subset_int_cur();


	// Construct a duplicate in free store of this object but with
	// zero use count.

	virtual subtrans_object *copy() const;

	// Test for equality - not guaranteed to find all cases of curve
	// superimposition, but reliably flagging cases of inequality.

	virtual logical operator==( subtype_object const & ) const;


	// Perform a linear transformation on the parametrisation, so
	// that it starts and ends at the given values (which must be
	// in increasing order).

	virtual void reparam(
					double,
					double
				);

	// Divide an intersection curve into two pieces at a given
	// SPAparameter value, possibly adjusting the spline approximations
	// to an exact value at the split point.

	virtual void split(
				double,
				SPAposition const &,
				int_cur *[ 2 ]
			);

	// Concatenate the contents of two curves into one.  The curves are
	// assumed to be the same base or derived type, and to have contiguous
	// SPAparameter ranges( "this" is the begining part of the combined curve,
	// the argument gives the end_part ).

	virtual void append( int_cur & );

	// Transformation

	virtual void operator*=( SPAtransf const &);

	// Geometric evaluation

	// Tangent direction to intcurve at given point. The base class
	// version uses param() and eval(), and so is always valid, but
	// many derived types will have a better way.

	virtual SPAunit_vector point_direction(
				SPAposition const &,
				SPAparameter const & = *(SPAparameter *)NULL_REF
			) const;


	// Find the foot of the perpendicular from the given point to
	// the curve, and tangent to and curvature of the curve at that
	// point, and its SPAparameter value. If an input SPAparameter value
	// is supplied (as argument 5), the perpendicular found is the
	// one nearest to the supplied SPAparameter SPAposition, otherwise it
	// is the one at which the curve is nearest to the given point.
	// Any of the return value arguments may be a NULL reference, in
	// which case it is simply ignored.

	virtual void point_perp(
				SPAposition const &,
				SPAposition &,
				SPAunit_vector &,
				SPAvector &,
				SPAparameter const & = *(SPAparameter *)NULL_REF,
				SPAparameter & = *(SPAparameter *)NULL_REF,
				logical f_weak = FALSE

		) const;

    // Parameter value for given point on curve. The base class
    // version finds the foot of the perpendicular to the approximating
    // spline, and returns the SPAparameter value of that. This will be
    // correct for many derived types, but not necessarily all.

    virtual double param(
                SPAposition const &,
                SPAparameter const & = *(SPAparameter *)NULL_REF
            ) const;

	// Find SPAposition, first and second derivative on curve at given
	// SPAparameter value.

	virtual void eval(
				double,
				SPAposition &,
				SPAvector & = *(SPAvector *)NULL_REF,
				SPAvector & = *(SPAvector *)NULL_REF,
				logical = FALSE
			) const;

    // Find curvature.

    virtual SPAvector eval_curvature(
                double,
                logical = FALSE
            ) const;


	// Save and restore. Save is easy, as derived class switching goes
	// through the normal virtual function mechanism. Restore is more
	// complicated, because until it is invoked we do not have an
	// object of the right class. Instead we switch on a table defined
	// by static instances of the restore_ts_def class (see below), to
	// invoke a simple friend function which constructs an object of
	// the right (derived) type. Then it can call the appropriate
	// member function to do the actual work.

public:
	static int id();
	virtual int type() const;

	virtual char const *type_name() const;

	virtual void save() const;

	virtual void save_data() const;

	virtual logical need_save_as_approx(int save_to_version, logical check_progenitors) const;

private:
	friend DECL_KERN subtype_object *restore_subset_int_cur();
	void restore_data();

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
// Now a utility function, which take a curve and the original curve
// range and the new-range requested and also the extension type and
// constructs an extension curve.  The SPAparameter range is naturally
// extended on the either/both ends of the curve to match the new
// range.  The extension type determines wether the curve is extended
// curvature continuously or, tangent continuously.  The extension-methods
// do not check for any possible self-intersections.  Also the curvature
// extension is done by reflecting the original curve about the tangent
// plane on the ends.  If one does not specify the extension type, the default
// is to do a tangent extension.
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN curve *
sg_extend_curve(
	curve const &,				// input curve(progenitor).
	SPAinterval const &,			// original curve range( progenitor range )
	SPAinterval const &,			// New range requated.
	CURVE_EXTENSION_TYPE ext_type = EXTEND_TANGENT

);
/** @} */
#endif
