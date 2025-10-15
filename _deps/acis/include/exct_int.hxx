/* ORIGINAL: acis2.1/kerngeom/intcur/exct_int.hxx */
/* $Id: exct_int.hxx,v 1.20 2002/08/09 17:15:24 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Define an interpolated curve subtype which is an exact spline
// curve.

#if !defined( exact_int_cur_CLASS)
#define exact_int_cur_CLASS

#include "logical.h"

#include "acis.hxx"
#include "dcl_kern.h"

#include "intdef.hxx"

#include "debugmsc.hxx"

/**
* @file exct_int.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISCURVES
 *
 * @{
 */
class surface;

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
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN subtype_object* restore_exact_int_cur();
#if defined D3_STANDALONE || defined D3_DEBUG
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN D3_istream& operator>>(D3_istream&, int_cur*&);
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN D3_ostream& operator<<(D3_ostream&, int_cur const&);
#endif

// The class definition itself.
/**
 * Represents an exact intersection spline curve.
 * <br>
 * <b>Role:</b> This class represents a spline curve. The <tt>bs3_curve</tt>
 * representing the given curve is considered to be exact.
 * <br><br>
 * @see SPAinterval
 */

class DECL_KERN exact_int_cur : public int_cur {
    // Allow extensions to declare themselves as friends. USE WITH EXTREME CAUTION
#ifdef exact_int_cur_FRIENDS
    exact_int_cur_FRIENDS
#endif

private:
    // Store the original (unextended) range of the curve when the curve is
    // extended. If this SPAinterval is empty, it implies that the curve is
    // unextended.

    SPAinterval	_unextended_range;


    exact_int_cur() {}


protected:

    // A utility to return the start or end point and derivative of the curve.
    // This is useful in handling extended curves. end = 0 for the start, 1 for
    // the end point. It returns the SPAparameter of the relevant end point.

    double evaluate_end(SPAposition& P, SPAvector& D, int end)	const;

public:
    // Construct a spline curve. Surfaces on which it lies are not
    // necessary for curve evaluation, but may be useful for other
    // purposes, as may the parametric forms (which do not need to
    // be exact).
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param surf
 * spline surface.
 * @param f_cur
 * 1st surface where curve lies.
 * @param s_cur
 * 2nd surface where curve lies.
 * @param f_surf
 * 1st surface curve.
 * @param s_surf
 * 2nd surface curve.
 */
    exact_int_cur(
        bs3_curve surf,			// spline curve
        surface const& f_cur = *(surface*)NULL_REF,
        // first surface on which curve lies
        surface const& s_cur = *(surface*)NULL_REF,
        // second surface on which curve lies
        bs2_curve f_surf = NULL,	// curve in SPAparameter space of the
        // first surface
        bs2_curve s_surf = NULL	// curve in SPAparameter space of the
        // second surface
    );

    // Copy constructor
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param old
 * spline surface.
 */
    exact_int_cur(const exact_int_cur& old);

    // STI ROLL
/*
// tbrv
*/
/**
 * @nodoc
 */
    virtual void full_size(SizeAccumulator&, logical = TRUE) const;
    // STI ROLL

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
    virtual int_cur* deep_copy(pointer_map* pm = NULL) const;

private:
    // We do not need a specific destructor, as we do not add any
    // subsidiary structure, but it is virtual, so we declare one
    // to quieten the compiler.

    ~exact_int_cur();

    // The default action is to return any surface stored, so this
    // is fine.

//	virtual surface const *surf1() const;
//	virtual surface const *surf2() const;


    // The default action is to return any pcurve stored.

//	virtual bs2_curve pcur1() const;
//	virtual bs2_curve pcur2() const;


    // Construct a duplicate in free store of this object but with
    // zero use count.

    virtual subtrans_object* copy() const;


    // Test for equality - not guaranteed to find all cases of curve
    // superimposition, but reliably flagging cases of inequality.
    // The base class int_cur version is sufficient.

//	virtual logical operator==( subtype_object const & ) const;


    // Parameter shift: the base class version is fine.

//	virtual void shift( double );

    // Reparametrise the curve. The default inplementation is fine.

    void reparam(
        double,
        double
    );

    // Divide an exact spline into two pieces at a given SPAparameter
    // value, possibly adjusting the spline approximations to an
    // exact value at the split point.

    virtual void split(
        double,
        SPAposition const&,
        int_cur* [2]
    );


    // Has the curve been extended?

    logical	extended()	const { return !_unextended_range.empty(); }


    // Spline concatenation: the base class version is fine.

//	virtual void append( int_cur & );

    // Transformation. The base class version is fine.

//	virtual void operator*=( SPAtransf const & );


    // Geometric evaluation

    // Tangent direction to curve at given point. The base class
    // just uses eval_direction(), which is fine.

//	virtual SPAunit_vector point_direction(
//				SPAposition const &,
//				SPAparameter const &
//			) const;

    // Curvature at point on curve. The base class just uses
    // eval_curvature(), which is fine.

//	virtual SPAvector point_curvature(
//				SPAposition const &,
//				SPAparameter const &
//			) const;

    // Parameter value for given point on curve. The default
    // implementation is fine.

//	virtual double param(
//				SPAposition const &,
//				SPAparameter const &
//			) const;


    // Find the closest point on the curve, the foot, to the given point,
    // and optionally its SPAparameter value. If an input SPAparameter value is
    // supplied (as the first SPAparameter argument), the foot found is only
    // a local solution nearest to the supplied SPAparameter SPAposition. Any
    // of the return value arguments may be a NULL reference, in which
    // case it is simply ignored.
    virtual void closest_point(
        SPAposition const& pos,
        SPAposition& foot,
        SPAparameter const& param_guess = *(SPAparameter*)NULL_REF,
        SPAparameter& param_actual = *(SPAparameter*)NULL_REF
    ) const;


    // Find SPAposition, first and second derivative on curve at given
    // SPAparameter value.

    virtual void eval(
        double,
        SPAposition&,
        SPAvector&,
        SPAvector&,
        logical
    ) const;

    // Find SPAposition on curve at given SPAparameter value. The default
    // version using eval() is fine.

//	virtual SPAposition eval_position(
//				double,
//				logical
//			) const;

    // Find parametric derivative (magnitude and direction). The
    // default version using eval() is fine.

//	virtual SPAvector eval_deriv(
//				double,
//				logical
//			) const;

    // Find curvature. The default version using eval() is fine.

//	virtual SPAvector eval_curvature(
//				double,
//				logical
//			) const;


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
        SPAposition&,			// Point on curve at given SPAparameter
        SPAvector** = NULL, 	// Array of pointers to vectors, of
        // size nd. Any of the pointers may
        // be null, in which case the
        // corresponding derivative will not
        // be returned.
        int = 0,       		// Number of derivatives required (nd)
        evaluate_curve_side = evaluate_curve_unknown
        // the evaluation location - above,
        // below or don't care.
    ) const;


    // Return the number of derivatives which evaluate() can find
    // "accurately" (and fairly directly), rather than by finite
    // differencing, over the given portion of the curve. If there
    // is no limit to the number of accurate derivatives, returns
    // the value ALL_CURVE_DERIVATIVES, which is large enough to be
    // more than anyone could reasonably want.

    virtual int accurate_derivs(
        SPAinterval const& = *(SPAinterval*)NULL_REF
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

public:

    /**
     * Returns the ID for the <tt>exact_int_cur</tt> list.
     */
    static int id();
    /**
     * Returns the type of <tt>exact_int_cur</tt>.
     */
    virtual int type() const;
    /**
     * Returns the string <tt>"exactcur"</tt>.
     */
    virtual char const* type_name() const;
    /**
     * Saves the data for the <tt>exact_int_cur</tt> to the save file.
     */
    virtual void save_data() const;
    /**
    * @nodoc
    **/
    virtual void process(geometry_definition_processor& p) const;

protected:
    virtual logical need_save_approx_as_full() const;

private:
    friend DECL_KERN subtype_object* restore_exact_int_cur();
    void restore_data();


    // Check for any data errors in the curve, and correct the errors if
    // possible.  The various arguments provide control over which checks
    // are made, which fixes can be applied and which fixes were actually
    // applied.  The function returns a list of errors that remain in the
    // curve on exit.
    // See chk_stat.hxx for information on the argument types used here.

    virtual	check_status_list* check(
        const check_fix& input = *(const check_fix*)NULL_REF,
        // supplies a set of flags which say which fixes
        // are allowable (the default is to fix nothing)
        check_fix& result = *(check_fix*)NULL_REF,
        // returns a set of flags which say which fixes
        // were applied
        const check_status_list* = (const check_status_list*)NULL_REF
        // list of checks that are to be made.  If the
        // list is null, then every possible check will
        // be made; otherwise, the function will only
        // check for things in the list.  The return
        // value for the function will then be a subset
        // of this list.
    );


#if defined D3_STANDALONE || defined D3_DEBUG

    /*
      // tbrv
    */
    /**
     * @nodoc
    */
    friend DECL_KERN D3_istream& operator>>(
        D3_istream&,
        int_cur*&
        );

    /*
      // tbrv
    */
    /**
     * @nodoc
    */
    friend DECL_KERN D3_ostream& operator<<(
        D3_ostream&,
        int_cur const&
        );

    /*
      // tbrv
    */
    /**
     * @nodoc
    */
    virtual void input(
        D3_istream&
    );
    /*
      // tbrv
    */
    /**
     * @nodoc
    */
    virtual void print(
        D3_ostream&
    ) const;

#endif

    // Debug printout
    /*
      // tbrv
    */
    /**
     * @nodoc
    */
    virtual void debug(
        char const*,
        logical,
        FILE*
    ) const;
    /*
      // tbrv
    */
    /**
     * @nodoc
    */
    void debug_data(
        char const*,
        logical,
        FILE*
    ) const;
};

/** @} */
#endif
