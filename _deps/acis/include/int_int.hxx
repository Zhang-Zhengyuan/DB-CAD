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

// Define an interpolated curve subtype which is an int spline
// curve.

#if !defined( int_int_cur_CLASS)
#define int_int_cur_CLASS

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
DECL_KERN subtype_object* restore_int_int_cur();
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
 * Represents an int intersection spline curve.
 * <br>
 * <b>Role:</b> This class represents a spline curve. The <tt>bs3_curve</tt>
 * representing the given curve is considered to be int.
 * <br><br>
 * @see SPAinterval
 */

class DECL_KERN int_int_cur : public int_cur {
    // Allow extensions to declare themselves as friends. USE WITH EXTREME CAUTION
#ifdef int_int_cur_FRIENDS
    int_int_cur_FRIENDS
#endif

private:

    int_int_cur() {}


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
    int_int_cur(
        bs3_curve surf,			// spline curve
        double tol,
        surface const& f_cur = *(surface*)NULL_REF,
        // first surface on which curve lies
        surface const& s_cur = *(surface*)NULL_REF,
        // second surface on which curve lies
        bs2_curve f_surf = NULL,	// curve in SPAparameter space of the
        // first surface
        bs2_curve s_surf = NULL,	// curve in SPAparameter space of the
        // second surface
        const SPAinterval& safe_range = *(SPAinterval*)NULL_REF
    );
    //int_int_cur(
    //    bs3_curve            bs3,
    //    double               bs3tol,
    //    surface const& surf1,
    //    surface const& surf2,
    //    bs2_curve            pcurve1,
    //    bs2_curve            pcurve2,
    //    const SPAinterval& safe_range = *(const class SPAinterval*)NULL_REF
    //);

    // Copy constructor
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param old
 * spline surface.
 */
    int_int_cur(const int_int_cur& old);

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


public:

    int_int_cur(
        const char* gme,
        bs3_curve surf,			// spline curve
        double tol,
        surface const& f_cur = *(surface*)NULL_REF,
        // first surface on which curve lies
        surface const& s_cur = *(surface*)NULL_REF,
        // second surface on which curve lies
        bs2_curve f_surf = NULL,	// curve in SPAparameter space of the
        // first surface
        bs2_curve s_surf = NULL,	// curve in SPAparameter space of the
        // second surface
        const SPAinterval& safe_range = *(SPAinterval*)NULL_REF
    );

    virtual void full_size(SizeAccumulator&, logical = TRUE) const;

    /**
    * Calculates the discontinuity information if it was never stored.
    * <br><br>
    * <b>Role:</b> This function is intended to support restore of old versions of <tt>int_curs</tt>.
    */
    virtual	void 	calculate_disc_info();

private:
    // We do not need a specific destructor, as we do not add any
    // subsidiary structure, but it is virtual, so we declare one
    // to quieten the compiler.

    ~int_int_cur();

    virtual	void    set_safe_range();

    virtual int_cur* deep_copy(pointer_map* pm = NULL) const;
    virtual void process(geometry_definition_processor& p) const;


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


    // Divide an exact spline into two pieces at a given SPAparameter
    // value, possibly adjusting the spline approximations to an
    // exact value at the split point.

    virtual void split(
        double,
        SPAposition const&,
        int_cur* [2]
    );


    // Spline concatenation: the base class version is fine.

//	virtual void append( int_cur & );

    // Transformation. The base class version is fine.

    virtual void operator*=(SPAtransf const&);


    // Geometric evaluation

    // Tangent direction to curve at given point. The base class
    // just uses eval_direction(), which is fine.

    virtual SPAunit_vector point_direction(
        SPAposition const&,
        SPAparameter const&
    ) const;

    // Curvature at point on curve. The base class just uses
    // eval_curvature(), which is fine.

    virtual SPAvector point_curvature(
        SPAposition const&,
        SPAparameter const&
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
        curve_evaldata*,	// Initialisation data for iterations.
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

    virtual curve_evaldata* make_evaldata() const;

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

protected:
    virtual logical need_save_approx_as_full() const;

private:
    friend DECL_KERN subtype_object* restore_int_int_cur();
    void restore_data();




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

public:
    //通过静态成员函数（不需要对象调用，类名::就可以调用）调用私有构造函数 https://blog.csdn.net/qq_34028920/article/details/77429546
    static int_int_cur* gme_int_int_cur_public_constructor(
        bs3_curve surf,			// spline curve
        double tol,
        surface const& f_cur = *(surface*)NULL_REF,
        // first surface on which curve lies
        surface const& s_cur = *(surface*)NULL_REF,
        // second surface on which curve lies
        bs2_curve f_surf = NULL,	// curve in SPAparameter space of the
        // first surface
        bs2_curve s_surf = NULL,	// curve in SPAparameter space of the
        // second surface
        const SPAinterval& safe_range = *(SPAinterval*)NULL_REF
    );
};

/** @} */
#endif
