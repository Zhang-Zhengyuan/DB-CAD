/* ORIGINAL: acis2.1/kerngeom/pcurve/exp_par.hxx */
/* $Id: exp_par.hxx,v 1.19 2002/08/16 19:28:34 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Class for an explicit SPAparameter-space curve definition.

#if !defined( EXP_PAR_CUR_CLASS )
#define EXP_PAR_CUR_CLASS

#include "acis.hxx"
#include "dcl_kern.h"

#include "pcudef.hxx"

#include "bs2curve.hxx"

class surface;

// STI ROLL
class SizeAccumulator;
// STI ROLL
DECL_KERN subtype_object* restore_exp_par_cur();

// This is derived from the abstract base class par_cur, which handles
// certain operations, and provides virtual functions for the rest.

class DECL_KERN exp_par_cur : public par_cur {
private:

    // Data for an explicit SPAparameter curve.

    logical owns_bs2;
    bs2_curve cur_data;
    double fitol_data;
    double partol_data;
    surface* surf_data;
    // The surface on which the curve lies.


// Straightforward constructors

    exp_par_cur();
    exp_par_cur(
        bs2_curve,
        double,
        double,
        surface const&,
        logical = TRUE
    );
    exp_par_cur(
        exp_par_cur const&
    );
    ~exp_par_cur();

    friend class pcurve;	// to allow access to the constructor.

    // Extract the defining data.

    virtual bs2_curve cur() const;
    virtual double fitol() const;
    virtual double partol() const;
    virtual surface const* surf() const;

    void set_surface(surface const& sur);

    // Function to set the underlying bs2_curve to be the supplied
    // bs2_curve. If a bs2_curve already exists, this will be deleted.

    virtual void set_bs2_curve(bs2_curve new_bs2);

    // Duplication. Can't be done by constructor, as we want it
    // to be virtual.

    virtual subtrans_object* copy() const;


    // Test for equality.
    // It does not guarantee that all effectively equal curves are
    // determined to be equal, but does guarantee that different
    // curves are correctly identified as such.

    virtual logical operator==(subtype_object const&) const;


    // Transformation

    virtual void operator*=(SPAtransf const&);


    // Parameter shift: adjust the spline curve to have a SPAparameter
    // range increased by the argument value (which may be negative).
    // This is only used to move portions of a periodic curve by
    // integral multiples of the period.

    virtual void shift(double);


    // Perform a linear transformation on the parametrisation, so
    // that it starts and ends at the given values (which must be
    // in increasing order).

    virtual void reparam(
        double,
        double
    );


    // Divide a curve into two pieces at a given SPAparameter value,
    // possibly adjusting the spline approximations to an exact value
    // at the split point. If the SPAparameter value is at the beginning,
    // sets the first piece to NULL, and places the original curve
    // in the second slot; if the SPAparameter value is at the end,
    // places the original curve in the first slot, and sets the
    // second to NULL.

    virtual void split(
        double,
        SPApar_pos const&,
        par_cur* [2],
        SPApar_vec const& = *(SPApar_vec*)NULL_REF
    );


    // Concatenate the contents of two curves into one. The curves are
    // guaranteed to be the same base or derived type, and to have
    // contiguous SPAparameter ranges ("this" is the beginning part of
    // the combined curve, the argument gives the end part).

    virtual void append(par_cur&);


    // Find SPAposition, first and second derivative on curve at given
    // SPAparameter value.

    virtual void eval(
        double,
        SPApar_pos&,		// SPAposition in SPAparameter space
        SPApar_vec & = *(SPApar_vec*)NULL_REF,
        // first derivative
        SPApar_vec & = *(SPApar_vec*)NULL_REF
        // second derivative
    ) const;


    // Save and restore. Save is easy, as derived class switching goes
    // through the normal virtual function mechanism. Restore is more
    // complicated, because until it is invoked we do not have an
    // object of the right class. Instead our parent subtype_object
    // class searches a table for the right restore function, and
    // then calls it.

public:
    static int id();
    virtual int type() const;

    virtual char const* type_name() const;

    virtual void save_data() const;

    // The deep_copy method makes a copy without shared data

    virtual par_cur* deep_copy(pointer_map* pm = NULL) const;

    // STI ROLL
    virtual void full_size(SizeAccumulator&, logical = TRUE) const;
    // STI ROLL
/**
 * @nodoc
 */
    virtual void minimize(minimize_helper*);

private:
    friend DECL_KERN subtype_object* restore_exp_par_cur();
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
        char const*,
        logical,
        FILE*
    ) const;
    void debug_data(
        char const*,
        logical,
        FILE*
    ) const;

#if defined D3_STANDALONE || defined D3_DEBUG

    virtual void print(
        D3_ostream& os
    ) const;

    virtual void input(
        D3_istream& is
    );

#endif
public:
    // for save
    bs2_curve gme_get_cur_data();
    double gme_get_fitol_data();
    surface* gme_get_surf_data();
    // for restore
    void gme_set_surf_spl(spl_sur* spl);
    void** gme_get_surf_spl_ptr();
    //通过静态成员函数（不需要对象调用，类名::就可以调用）调用私有构造函数 https://blog.csdn.net/qq_34028920/article/details/77429546
    static exp_par_cur* gme_exp_par_cur_public_constructor(bs2_curve cur, double fit_tol, double par_tol, surface const& surf, logical owns_bs2 = TRUE);

};

#endif
