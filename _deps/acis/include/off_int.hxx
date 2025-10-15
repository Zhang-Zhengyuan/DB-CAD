/* ORIGINAL: acis2.1/kerngeom/intcur/off_int.hxx */
/* $Id: off_int.hxx,v 1.32 2002/08/09 17:15:24 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Define an interpolated curve subtype which is the intersection of
// two surfaces offset from given ones.

#if !defined( off_int_cur_CLASS)
#define off_int_cur_CLASS

#include "logical.h"

#include "acis.hxx"
#include "dcl_kern.h"

#include "fit.hxx"

#include "intdef.hxx"

#include "debugmsc.hxx"
/**
* @file off_int.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISCURVES
 *
 * @{
 */

class acis_istream;
class acis_ostream;

class surface;

class off_int_interp;

class SPAposition;
class SPAvector;
class SPAunit_vector;
class SPAtransf;

class SPAparameter;
class SPApar_pos;
class SPApar_dir;

// STI ROLL
class SizeAccumulator;
// STI ROLL
subtype_object *restore_off_int_cur();
#if defined D3_STANDALONE || defined D3_DEBUG
/*
 // tbrv
 */
 /**
  * @nodoc
 */
DECL_KERN D3_istream& operator>>( D3_istream &, int_cur *& );
/*
 // tbrv
 */
 /**
  * @nodoc
 */
DECL_KERN D3_ostream& operator<<( D3_ostream &, int_cur const & );
#endif

// The class definition itself.
/**
 * Represents a spline curve obtained by the intersection of two surfaces that are offsets of the given surfaces.
 * <br>
 * <b>Role:</b> This class represents a spline curve obtained by the intersection
 * of two surfaces that are offsets of the given surfaces. <tt>off_int_cur</tt> is derived
 * from the base class <tt>int_cur</tt>.
 * <br><br>
 * @see SPAinterval
 */
class DECL_KERN off_int_cur: public int_cur {
// Allow extensions to declare themselves as friends. USE WITH EXTREME CAUTION
#ifdef off_int_cur_FRIENDS
off_int_cur_FRIENDS
#endif

private:
	// Offsets from the two surfaces. Each one is positive if the
	// offset curve lies outside the surface, and negative if it is
	// inside.

	double offset1;
	double offset2;


    // Record the intervals for which the curve lies above each surface. These
    // are only set if the corresponding pcur_data is present. A null SPAinterval
    // (i.e. the SPAinterval [1,0]) implies that the curve does not lie over the
    // corresponding surface at all.

    SPAinterval    over1;
    SPAinterval    over2;

	// A function to set the over-surface SPAinterval.

	SPAinterval	over_surface( surface const &sf, bs3_curve cur, bs2_curve pcur );


    off_int_cur() : over1( interval_infinite ), over2( interval_infinite ) {}


    // Set the safe range to the full range, unless the surface normals are
    // parallel at either end, or either surface is singular.

    virtual	void    set_safe_range();


public:
	// Construct an off_int_cur when the interpolating curve has
	// already been found. This is needed by the curve interpolator
	// class, so we make it a friend.
    /**
     * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
     * <br><br>
     * @param inter_curv
     * interpolating curve.
     * @param tol
     * fit tolerance.
     * @param first_offset
     * first surface to offset.
     * @param sec_offset
     * second surface to offset.
     * @param first_dist
     * first surface offset distance.
     * @param sec_dist
     * second surface offset distance.
     * @param first_spl
     * first surface spline perp projection.
     * @param sec_spl
     * second surface spline perp projection.
     * @param range
     * safe range for the curve.
     * @param fir_curv
     * first surface curve interval.
     * @param sec_curv
     * second surface curve interval.
     */

	off_int_cur(
			bs3_curve inter_curv,			// interpolating curve
			double tol,				// fitol
			surface const & first_offset,	// first surface to offset
			surface const & sec_offset,	// second surface to offset
			double first_dist,				// offset from first surface
			double sec_dist,				// offset from second surface
			bs2_curve first_spl = NULL,	// spine perp. projn. to (spline) surf1
			bs2_curve sec_spl = NULL,	// spine perp. projn. to (spline) surf2
            const SPAinterval& range = *(const class SPAinterval*) NULL_REF,   // safe_range for the curve
            const SPAinterval& fir_curv= *(const class SPAinterval*) NULL_REF,   // SPAinterval for which curve
                                                // is over surf1
            const SPAinterval& sec_curv= *(const class SPAinterval*) NULL_REF    // SPAinterval for which curve
                                                // is over surf2
		);

	// Copy constructor
    /**
     * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
     * <br><br>
     * @param int_cur
     * int curve.
     */
	off_int_cur( const off_int_cur& int_cur);

	// The deep_copy method makes a copy without shared data
    /**
     * Creates a copy of an item that does not share any data with the original.
     * <br><br>
     * <b>Role:</b> Allocates new storage for all member data and any pointers.
     * Returns a pointer to the copied item.
     * <br><br>
     * @param pm
     * list of items within the entity that are already deep copied.
     */
	virtual int_cur *deep_copy(pointer_map * pm = NULL) const;

	// STI ROLL
	/*
	// tbrv
	*/

	/**
	 * @nodoc
	 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL

    // Calculate the discontinuity information if it was never stored.  This
	// function is intended to support restore of old versions of int_curs.
    /**
     * Calculates the discontinuity information for the <tt>off_int_cur</tt>.
     */
    virtual	void 	calculate_disc_info();

    // Functions to return the offset distances.
    /*
    // tbrv
    */
    /**
     * @nodoc
     */

    double off1() const { return offset1; }
    /*
    // tbrv
    */
    /**
     * @nodoc
     */

    double off2() const { return offset2; }

protected:
	// We do not need a specific destructor, as we do not add any
	// subsidiary structure, but it is virtual, so we declare it
	// anyway.

	~off_int_cur();

private:
	// Return NULL as surfaces defining the curve are distant from it, unless
	// the logical flag is TRUE to force the surface to be returned.

	virtual surface const *surf1( logical force = FALSE ) const;
	virtual surface const *surf2( logical force = FALSE ) const;


	// Return NULL as surfaces defining the curve are distant from it, unless
	// the logical flag is TRUE to force the pcurve to be returned.

	virtual bs2_curve pcur1( logical force = FALSE ) const;
	virtual bs2_curve pcur2( logical force = FALSE ) const;


	// Construct a duplicate in free store of this object but with
	// zero use count.

	virtual subtrans_object *copy() const;


	// Assignment. This copies all the underlying information.

	virtual int_cur &operator=(
				int_cur const &
			);


	// Test for equality - not guaranteed to find all cases of curve
	// superimposition, but reliably flagging cases of inequality.

	virtual logical operator==( subtype_object const & ) const;


    // The following virtual functions have to be supplied so that the 'over
    // surface' ranges can be kept up to date.

    // Parameter shift: adjust the spline curves to have a SPAparameter
    // range increased by the argument value (which may be negative).
    // This is only used by intcurve::split to move portions of a
    // periodic curve by integral multiples of the period. The default
    // just shifts the parametrisation of the approximating splines.

    virtual void shift( double );


    // Perform a linear transformation on the parametrisation, so
    // that it starts and ends at the given values (which must be
    // in increasing order). The default just reparametrises the
    // approximating splines.

    virtual void reparam(
                double,
                double
            );


    // Concatenate the contents of two curves into one. The curves are
    // guaranteed to be the same base or derived type, and to have
    // contiguous SPAparameter ranges ("this" is the beginning part of
    // the combined curve, the argument gives the end part). The
    // default concatenates the approximating splines, assuming that
    // any other information is the same for both curves and the
    // result.

    virtual void append( int_cur & );


	// Divide a off_int_cur into two pieces at a given SPAparameter value,
	// possibly adjusting the spline approximations to an exact value
	// at the split point.

	virtual void split(
				double,
				SPAposition const &,
				int_cur *[ 2 ]
			);


	// Transformation

	virtual void operator*=( SPAtransf const & );


	// Geometric evaluation

	// Tangent direction to curve at given point.

	virtual SPAunit_vector point_direction(
				SPAposition const &,
				SPAparameter const &
			) const;

	// Curvature at point on curve.

	virtual SPAvector point_curvature(
				SPAposition const &,
				SPAparameter const &
			) const;

	// Parameter value for given point on curve. The default
	// int_cur implementation is fine.

//	virtual double param(
//				SPAposition const &,
//				SPAparameter const &
//			) const;

	// Find SPAposition, first and second derivative on curve at given
	// SPAparameter value. The default using evaluate() is fine.

//	virtual void eval(
//				double,
//				SPAposition &,
//				SPAvector &,
//				SPAvector &,
//				logical
//			) const;

	// Find SPAposition on curve at given SPAparameter value. The default
	// version is fine.

//	virtual SPAposition eval_position(
//				double,
//				logical
//			) const;

	// Find parametric derivative (magnitude and direction). The
	// default version is fine.

//	virtual SPAvector eval_deriv(
//				double,
//				logical
//			) const;

	// Find curvature.
	// The default is fine, now that second derivatives are accurate.

// 	virtual SPAvector eval_curvature(
// 				double,
// 				logical
// 			) const;


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
                double,				// Parameter
                SPAposition &,			// Point on curve at given SPAparameter
                SPAvector ** = NULL, 	// Array of pointers to vectors, of
									// size nd. Any of the pointers may
									// be null, in which case the
									// corresponding derivative will not
									// be returned.
                int = 0,       		// Number of derivatives required (nd)
				evaluate_curve_side = evaluate_curve_unknown
									// the evaluation location - above,
									// below or don't care.
            ) const;


	// The evaluate_iter() function calculates derivatives, of any order
	// up to the number requested, and stores them in vectors provided
	// by the user. It returns the number it was able to calculate;
	// this will be equal to the number requested in all but the most
	// exceptional circumstances. A certain number will be evaluated
	// directly and (more or less) accurately; higher derivatives will
	// be automatically calculated by finite differencing; the accuracy
	// of these decreases with the order of the derivative, as the
	// cost increases.

	virtual int evaluate_iter(
                double,				// Parameter
				curve_evaldata *,	// Initialisation data for iterations.
                SPAposition &,			// Point on curve at given SPAparameter
                SPAvector ** = NULL, 	// Array of pointers to vectors, of
									// size nd. Any of the pointers may
									// be null, in which case the
									// corresponding derivative will not
									// be returned.
                int = 0,       		// Number of derivatives required (nd)
				evaluate_curve_side = evaluate_curve_unknown
									// the evaluation location - above,
									// below or don't care.
            ) const;

	// Construct a data object to retain evaluation information across
	// calls to evaluate_iter(). This is to allow subsidiary calls
	// within an iterative evaluator to start iteration much closer to the
	// required result than is possible just using the curve information
	// itself.

	virtual curve_evaldata *make_evaldata() const;


	// An evaluator that takes surface arguments in addition to curve
	// arguments.  As well as returning curve SPAposition and derivatives,
    // it returns the derivatives of the surface wrt t (these will often
    // but not always be equal to the curve derivs) and also the derivatives
    // of the surface parameters wrt t.  The array of vectors to return
    // the curve derivatives must be of length at least nd_cu, and the
    // various arrays of vectors to return the surface data can either
    // be null, indicating that this particular derivative is not required,
    // or be of length at least nd_sf.
    //
    // Unlike the other evaluators, this function OVERWRITES the integer
    // arguments specifying the numners of derivatives required, with the
    // number actually obtained.  The function itself returns information
    // about the surface data that was calculated:
    //    0 => no surface data (e.g. exact_int_cur)
    //	  1 => data for first surface only
    //	  2 => data for second surface only
    //	  3 => data for both surfaces

public:
    /**
     * An evaluator that takes surface arguments in addition to the usual arguments.
     * <br><br>
     * <b>Role:</b> As well as returning curve position and derivatives, it returns
     * the derivatives of the surface wrt t (these will often but not always be equal
     * to the curve derivatives) and also the derivatives of the surface parameters
     * with respect to t. The array of vectors to return the curve derivatives
     * must be of length at least <tt>nd_cu</tt>, and the various arrays of vectors to
     * return the surface data can either be null, indicating that this particular
     * derivative is not required, or be of length at least <tt>nd_sf</tt>
     * Unlike the other evaluators, this function <tt>OVERWRITES</tt> the integer arguments
     * specifying the numbers of derivatives required, with the number actually
     * obtained. The function itself returns information about the surface data
     * that was calculated:
     * <br><br>
     *<table border="0" width="100%">
	 *<tr>
	 *<td width="6%" align="right"><tt>0</tt></td>
	 *<td width="4%"></td>
	 *<td width="87%"> <tt>no surface data (e.g. exact_int_cur)</tt></td>
	 *</tr>
	 *<tr>
	 *<td width="6%" align="right"><tt>1</tt></td>
	 *<td width="4%"></td>
	 *<td width="87%"><tt>data for first surface only</tt></td>
	 *</tr>
	 *<tr>
	 *<td width="6%" align="right"><tt>2</tt></td>
	 *<td width="4%"></td>
	 *<td width="87%"> <tt>data for second surface only</tt></td>
	 *</tr>
	 *<tr>
	 *<td width="6%" align="right"><tt>3</tt></td>
	 *<td width="4%"></td>
	 *<td width="87%"> <tt>data for both surfaces</tt></td>
	 *</tr>
	 *</table>
	 * <br><br>
     * @param parameter
     * Parameter.
     * @param pt
     * Point on curve at given parameter.
     * @param vec
     * Derivatives of the off_int_cur.
     * @param nd_cu
     * number of curve derivatives required/calculated.
     * @param nd_sf
     * number of surface derivatives required/calculated.
     * @param eval
     * evaluatio location.
     * @param pt_1
     * point on support surface 1.
     * @param vec_1
     * Derivatives of the first support surface.
     * @param pt_2
     * point on support surface 2.
     * @param vec_2
     * Derivatives of the second support surface.
     * @param par_1
     * Parameters on surface 1.
     * @param der1
     * Derivatives of parameters on surface 1.
     * @param par_2
     * Parameters on surface 2.
     * @param der2
     * Derivatives of parameters on surface 2.
     * @param guess1
     * optional guess value for first SPApar_pos.
     * @param guess2
     * optional guess value for second SPApar_pos.
     */
	virtual int evaluate_surfs(
                double parameter ,				// Parameter
                SPAposition & pt,			// Point on curve at given SPAparameter
                SPAvector * vec,		 	// Derivatives of the off_int_cur
					                // (the caller must supply an array of
					                // length nd_cu, or NULL to indicate that
					                // curve derivatives are not required)
                int& nd_cu,     	// Number of curve derivs required/calculated
                int& nd_sf,     	// Number of surface derivs required/calculated
				evaluate_curve_side eval = evaluate_curve_unknown,
									// the evaluation location - above,
									// below or don't care.
                SPAposition & pt_1 = *(SPAposition*) NULL_REF,	// Point on support surface 1
                SPAvector * vec_1= NULL, 	// Derivatives of the first support surface.
                SPAposition & pt_2 = *(SPAposition*) NULL_REF,	// Point on support surface 2
                SPAvector * vec_2 = NULL, 	// Derivatives of the second support surface.
				SPApar_pos& par_1= *(SPApar_pos*) NULL_REF,  // Parameters on surface 1
				SPApar_vec* der1= NULL,  	// Derivatives of parameters on surface 1
				SPApar_pos&  par_2 = *(SPApar_pos*) NULL_REF,  // Parameters on surface 2
				SPApar_vec* der2 = NULL, 	// Derivatives of parameters on surface 2
				SPApar_pos const & guess1 = *(SPApar_pos *)NULL_REF,
					   // optional guess value for first SPApar_pos
				SPApar_pos const & guess2 = *(SPApar_pos *)NULL_REF
					   // optional guess value for second SPApar_pos
            ) const;

protected:
	// Return the number of derivatives which evaluate() can find
	// "accurately" (and fairly directly), rather than by finite
	// differencing, over the given portion of the curve. If there
	// is no limit to the number of accurate derivatives, returns
	// the value ALL_CURVE_DERIVATIVES, which is large enough to be
	// more than anyone could reasonably want.

	virtual int accurate_derivs(
				SPAinterval const & = *(SPAinterval*)NULL_REF
								 	// Defaults to the whole curve
			) const;


	// Save and restore. Save is easy, as derived class switching goes
	// through the normal virtual function mechanism. Restore is more
	// complicated, because until it is invoked we do not have an
	// object of the right class. Instead we switch on a table defined
	// by static instances of the restore_ts_def class (see below), to
	// invoke a simple friend function which constructs an object of
	// the right (derived) type. Then it can call the appropriate
	// member function to do the actual work.

	// Note that if we ever implement the functions istr_to_intcurve
	// and curve_to_ostr defined above we shall have to have the same
	// sort of mechanism for them as for save/restore.

public:
	/**
	 * Returns the <tt>ID</tt> for the off_int_cur <tt>list</tt>.
	 */
	static int id();
	/**
	 * Returns the type of <tt>off_int_cur</tt>.
	 */
	virtual int type() const;
    /**
     * Returns the string <tt>"offintcur"</tt>.
     */
	virtual char const *type_name() const;
	/**
	 * Saves the information for the <tt>off_int_cur</tt> to a save file.
	 */
	void save_data() const;
	/**
	* @nodoc
	**/
	virtual void process(geometry_definition_processor& p ) const;

protected:
	virtual logical need_save_approx_as_full() const;

private:
	friend subtype_object *restore_off_int_cur();
	void restore_data();

#if defined D3_STANDALONE || defined D3_DEBUG

	friend DECL_KERN D3_istream& operator>>(
				D3_istream &,
				int_cur *&
			);

	friend DECL_KERN D3_ostream& operator<<(
				D3_ostream &,
				int_cur const &
			);

	virtual void input(
				D3_istream &
			);

	virtual void print(
				D3_ostream &
			) const;

#endif

	// Debug printout

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


// Now a class to fit a curve to the intersection of two offset
// surfaces given a preliminary list of points, directions etc.
/*
// tbrv
*/

/**
 * @nodoc
 */

class DECL_KERN off_int_interp: public curve_interp {

private:
	double offset1;			// offset from first surface
	double offset2;			// offset from second surface

public:
	// Constructor providing all the information required.

	off_int_interp(
			surface const &,	// first surface
			surface const &,	// second surface
			double,				// offset from first surface
			double,				// offset from second surface
			int,				// number of points
			SPAposition const *,	// array of positions
			SPAvector const *,		// array of tangents
			SPApar_pos const *,	// SPApar_pos on first surf if parametric
			SPApar_pos const *,	// SPApar_pos on second surf if parametric
			double				// tolerance for fit
		);

	// The same, but for the common case of equal offsets.

	off_int_interp(
			surface const &,	// first surface
			surface const &,	// second surface
			double,				// offset from surfaces
			int,				// number of points
			SPAposition const *,	// array of positions
			SPAvector const *,		// array of tangents
			SPApar_pos const *,	// SPApar_pos on first surf if parametric
			SPApar_pos const *,	// SPApar_pos on second surf if parametric
			double				// tolerance for fit
		);

	// Virtual destructor.

	~off_int_interp();

	// Find the true-point for intersection of offset surfaces.

	void true_point(
			double,			// tolerance
			point_data &	// approximate SPAposition, direction,
							// SPAparameter values on input, exact values
							// on output
		) const;

	// Construct the appropriate int_cur subclass object (in this
	// case an off_int_cur) from the data in this object after curve
	// interpolation.

	int_cur *make_int_cur();
};

/** @} */
#endif
