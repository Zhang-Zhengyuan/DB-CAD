/* ORIGINAL: acis2.1/kerngeom/intcur/par_int.hxx */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

/* $Id: par_int.hxx,v 1.27 2002/08/09 17:15:24 jeff Exp $ */


// Define an interpolated curve subtype which is a SPAparameter line
// on a spline surface, for a parametrisation where the true
// SPAparameter line is everywhere within fit tolerance of the spline
// approximation's SPAparameter line.

#if !defined( par_int_cur_CLASS)
#define par_int_cur_CLASS
/**
* @file par_int.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISCURVES
 *
 * @{
 */
#include "logical.h"

#include "acis.hxx"
#include "dcl_kern.h"

#include "fit.hxx"

#include "intdef.hxx"

#include "debugmsc.hxx"

class surface;
class spline;

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

class SPAinterval;
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN subtype_object *restore_par_int_cur();
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

// Class to select constant-SPAparameter direction.

/**
 * Specifies the curve direction.
 * @param par_int_cur_u
 * u parameter curve.
 * @param par_int_cur_v
 * v parameter curve.
 * @param par_int_cur_unset
 * unset.
 * @param par_int_cur_general
 * neither a u or v parameter curve.
 **/
enum par_int_cur_dir {
	par_int_cur_u,		// u SPAparameter line (v = constant)
	par_int_cur_v,		// v SPAparameter line (u = constant)
	par_int_cur_unset,  // STL ajr 19May06. Fix for 81755.
    par_int_cur_general // a general parameter curve
};

// The class definition itself.

/**
 * Represents an exact spline curve in the parameter space of a surface.
 * <br>
 * <b>Role:</b> This class represents a 3D spline curve as a 2D parameter curve on a spline
 * surface. The spline surface is used to map the 2D parameter curve from <tt>(u,v)</tt> parameter space
 * into <tt>(x,y,z)</tt> euclidean space. The approximate parameter curve is everywhere within the fit
 * tolerance of the exact parameter curve.
 */
class DECL_KERN par_int_cur: public int_cur {
// Allow extensions to declare themselves as friends. USE WITH EXTREME CAUTION
#ifdef par_int_cur_FRIENDS
par_int_cur_FRIENDS
#endif

private:
	// The only additional data required is to select which surface
	// and pcurve to use as the definition.

	logical par_sur1;		// TRUE if sur1 and par1 define the
							// curve, FALSE if it is sur2 and par2.

    // STL ajr 19May06. To calculate discon info properly, record direction.
    par_int_cur_dir cur_dir;

	par_int_cur() {cur_dir = par_int_cur_unset; par_sur1 = TRUE; }

public:
	// Construct a constant SPAparameter curve.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Constructs a constant parameter curve. If logical is <tt>TRUE</tt>, the surface is made
 * the first surface of the int_cur; if it is <tt>FALSE</tt>, the surface is made the second surface of
 * the <tt>int_cur</tt>.
 * <br><br>
 * @param sur_cur
 * surface where curve lies.
 * @param dir
 * select u or v direction.
 * @param uvparam
 * constant u or v parameter.
 * @param surf
 * surface.
 * @param discontinue
 * discontinuity.
 */
	par_int_cur(
			surface const &sur_cur,		// surface on which curve lies
			par_int_cur_dir dir,	// select u or v direction
			double uvparam,				// constant v or u SPAparameter
			logical surf = TRUE,		// TRUE to make the surface #1,
								// FALSE for surface #2
		    const discontinuity_info& discontinue = *(discontinuity_info*) NULL_REF
		);

	// Construct a general SPAparameter curve, given an exact bs2_curve
	// and an approximate bs3_curve.

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Constructs a general parameter curve, given an exact <tt>bs2_curve</tt> and an approximate
 * <tt>bs3_curve</tt>. If logical is <tt>TRUE</tt>, the surface is made the first surface of the <tt>int_cur</tt>; if it is
 * <tt>FALSE</tt>, the surface is made the second surface of the <tt>int_cur</tt>.
 * <br><br>
 * @param spl
 * spline curve.
 * @param tol
 * fit tolerance.
 * @param sur_cur
 * surface where curve lies.
 * @param cur_par
 * surface curve in parameter space.
 * @param sur
 * surface.
 * @param discontinue
 * discontinuity.
 */
	par_int_cur(
			bs3_curve spl,			// spline curve
			double tol,				// fit tolerance
			surface const &sur_cur,	// surface on which curve lies
			bs2_curve cur_par,			// curve in SPAparameter space of the
								// surface
			logical sur = TRUE,		// TRUE to make surf1 SPAparameter curve
								// define the true curve, FALSE for
								// surf2
		    const discontinuity_info& discontinue = *(discontinuity_info*) NULL_REF
		);

	// Construct a general SPAparameter curve, with an additional surface
	// and SPAparameter curve.

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Constructs a general parameter curve, with an additional surface and parameter
 * curve. If logical is <tt>TRUE</tt>, the surface is made the first surface of the <tt>int_cur</tt>; if it is
 * <tt>FALSE</tt>, the surface is made the second surface of the <tt>int_cur</tt>.
 * <br><br>
 * @param spl
 * spline curve.
 * @param tol
 * fit tolerance.
 * @param sur1
 * 1st surface for curve.
 * @param sur2
 * 2nd surface for curve.
 * @param cur1
 * 1st curve on surface.
 * @param cur2
 * 2nd curve on surface.
 * @param sur
 * surface.
 * @param discontinue
 * discontinuity.
 */
	par_int_cur(
			bs3_curve spl,			// spline curve
			double tol,				// fit tolerance
			surface const &sur1,	// first surface on which curve lies
			surface const &sur2,	// second surface on which curve lies
			bs2_curve cur1,			// curve in SPAparameter space of the
								// first surface
			bs2_curve cur2,			// curve in SPAparameter space of the
								// second surface
			logical sur = TRUE,		// TRUE to make surf1 SPAparameter curve
								// define the true curve, FALSE for
								// surf2
		    const discontinuity_info& discontinue = *(discontinuity_info*) NULL_REF
		);

	// Copy constructor

/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param cur
 * par_int_curve.
 */
	par_int_cur( const par_int_cur& cur);

	// The deep_copy method makes a copy without shared data

/**
 * Creates a copy of an item that does not share any data with the original.
 * <br><br>
 * <b>Role:</b> Allocates new storage for all member data and any pointers. Returns a pointer to
 * the copied item.
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

private:
	// We do not need a specific destructor, as we do not add any
	// subsidiary structure, but it is virtual, so we define it
	// anyway.

	~par_int_cur();

	// The default action is to return any surface stored, so this
	// is fine.

//	virtual surface const *surf1() const;
//	virtual surface const *surf2() const;


	// The default action is to return any pcurve stored.

//	virtual bs2_curve pcur1() const;
//	virtual bs2_curve pcur2() const;


	// Construct a duplicate in free store of this object but with
	// zero use count.

	virtual subtrans_object *copy() const;


	// Test for equality - not guaranteed to find all cases of curve
	// superimposition, but reliably flagging cases of inequality.

	virtual logical operator==( subtype_object const & ) const;

	// STI dgh begin - make this public to support translators

public:

/*
// tbrv
*/
/**
 * @nodoc
 */
	bs2_curve pcur() const { return par_sur1 ? pcur1_data : pcur2_data; };

/*
// tbrv
*/
/**
 * @nodoc
 */
	surface const *surf() const { return par_sur1 ? surf1_data : surf2_data; };

    // Calculate the discontinuity information if it was never stored.  This
	// function is intended to support restore of old versions of int_curs.

/**
 * Calculates the discontinuity information if it was never stored.
 * <br><br>
 * <b>Role:</b> This function is intended to support restore of old versions of <tt>int_curs</tt>.
 */
    virtual	void 	calculate_disc_info();

protected:

	// STI dgh end

	// Parameter shift: the base class version is fine.

//	virtual void shift( double );

	// Reparametrise the curve. The default inplementation is fine.

//	void reparam(
//				double,
//				double
//			);

	// Divide an intersection curve into two pieces at a given
	// SPAparameter value, possibly adjusting the spline approximations
	// to an exact value at the split point.

	virtual void split(
				double,
				SPAposition const &,
				int_cur *[ 2 ]
			);

	// Spline concatenation: the base class version is fine.

//	virtual void append( int_cur & );

	// Transformation. The base class version is fine.


//	virtual void operator*=( SPAtransf const & );


	// Geometric evaluation

	// Tangent direction to curve at given point. The default uses
	// param() and eval_deriv().

//	virtual SPAunit_vector point_direction(
//				SPAposition const &,
//				SPAparameter const &
//			) const;

	// Curvature at point on curve. The default uses param() and
	// eval_curvature().

//	virtual SPAvector point_curvature(
//				SPAposition const &,
//				SPAparameter const &
//			) const;


	// Find the foot of the perpendicular from the given point to
	// the curve, and tangent to the curve at that point, and its
	// SPAparameter value. If an input SPAparameter value is supplied (as
	// argument 4), the perpendicular found is the one nearest to
	// the supplied SPAparameter SPAposition, otherwise it is the one
	// at which the curve is nearest to the given point. Any of the
	// return value arguments may be a NULL reference, in which case
	// it is simply ignored. The default version is fine for us.

//	virtual void point_perp(
//				SPAposition const &,
//				SPAposition &,
//				SPAunit_vector &,
//				SPAparameter const & = *(SPAparameter *)NULL_REF,
//				SPAparameter & = *(SPAparameter *)NULL_REF
//			) const;


	// Parameter value for given point on curve.

	virtual double param(
				SPAposition const &,
				SPAparameter const &
			) const;

	// Find SPAposition, first and second derivative on curve at given
	// SPAparameter value.

	virtual void eval(
				double,
				SPAposition &,
				SPAvector &,
				SPAvector &,
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

	// Find curvature. The default using eval() is OK.

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
 * <b>Role:</b> As well as returning curve position and derivatives, it returns the derivatives
 * of the surface wrt t (these will often but not always be equal to the curve derivs) and also
 * the derivatives of the surface parameters wrt t. The array of vectors to return the curve
 * derivatives must be of length at least <tt>nd_cu</tt>, and the various arrays of vectors to return the
 * surface data can either be null, indicating that this particular derivative is not required,
 * or be of length at least <tt>nd_sf</tt>.
 * <br>
 * Unlike the other evaluators, this function <tt>OVERWRITES</tt> the integer arguments specifying the
 * numbers of derivatives required, with the number actually obtained. The function itself
 * returns information about the surface data that was calculated:
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
 * @param par
 * parameter.
 * @param pt
 * point on curve at given parameter.
 * @param derivative
 * derivatives of off_int_cur.
 * @param nd_cu
 * no. of curve derivatives required/calc.
 * @param nd_sf
 * no. of surface derivatives required/calc.
 * @param eval_loc
 * the evaluation location above, below or don't care.
 * @param pt_sur1
 * point on support surface 1.
 * @param der_sur1
 * derivatives of 1st support surface.
 * @param pt_sur2
 * point on support surface 2.
 * @param der_sur2
 * derivatives of 2nd support surface.
 * @param par_sur1
 * Parameters on surface 1.
 * @param der_par_sur1
 * derivatives of parameters on surface 1.
 * @param par_sur2
 * Parameters on surface 2.
 * @param der_par_sur2
 * derivatives of parameters on surface 2.
 * @param guess1
 * optional guess value for 1st par_pos.
 * @param guess2
 * optional guess value for 2nd par_pos.
 */
	virtual int evaluate_surfs(
                double par,				// Parameter
                SPAposition &pt,			// Point on curve at given SPAparameter
                SPAvector *derivative,		 	// Derivatives of the off_int_cur
					                // (the caller must supply an array of
					                // length nd_cu, or NULL to indicate that
					                // curve derivatives are not required)
                int& nd_cu,     	// Number of curve derivs required/calculated
                int& nd_sf,     	// Number of surface derivs required/calculated
				evaluate_curve_side eval_loc = evaluate_curve_unknown,
									// the evaluation location - above,
									// below or don't care.
                SPAposition &pt_sur1 = *(SPAposition*) NULL_REF,	// Point on support surface 1
                SPAvector *der_sur1 = NULL, 	// Derivatives of the first support surface.
                SPAposition &pt_sur2 = *(SPAposition*) NULL_REF,	// Point on support surface 2
                SPAvector *der_sur2 = NULL, 	// Derivatives of the second support surface.
				SPApar_pos& par_sur1 = *(SPApar_pos*) NULL_REF,  // Parameters on surface 1
				SPApar_vec* der_par_sur1 = NULL,  	// Derivatives of parameters on surface 1
				SPApar_pos& par_sur2 = *(SPApar_pos*) NULL_REF,  // Parameters on surface 2
				SPApar_vec* der_par_sur2 = NULL, 	// Derivatives of parameters on surface 2
				SPApar_pos const &guess1 = *(SPApar_pos *)NULL_REF,
					   // optional guess value for first SPApar_pos
				SPApar_pos const &guess2 = *(SPApar_pos *)NULL_REF
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

public:
/**
 * Returns the <tt>ID</tt> for the par_int_cur <tt>list</tt>.
 */
	static int id();
/**
 * Returns the type of <tt>par_int_cur</tt>.
 */
	virtual int type() const;

/**
 * Returns the string <tt>"parcur"</tt>.
 */
	virtual char const *type_name() const;
/**
 * Saves the information for the <tt>par_int_cur</tt> to the save file.
 * <br><br>
 * <b>Role:</b> Stores the information from this class to the save file. This method is never
 * called directly. It is called by a higher hierarchical function if an item in the <tt>SAT</tt> file is
 * determined to be of this class type.
 */
	virtual void save_data() const;

	/**
	* @nodoc
	**/
	virtual void process(geometry_definition_processor& p) const;

// STL ajr 19May06. Added.
/*
// tbrv
*/
/**
 * @nodoc
 */
    par_int_cur_dir get_dir();

private:
	friend DECL_KERN subtype_object *restore_par_int_cur();
	void restore_data();

protected:

	// Make or remake the approximating curve.  The intcurve argument 'ic' may
    // be null but if it is supplied the function may be a little faster.  The
    // function stores the approximating curve and the actual fit error that
    // was achieved in the int_cur, overriding the declared const-ness of the
    // function to do this.

    virtual void	make_approx(
							 double fit,
							 const intcurve& ic = *(intcurve*) NULL_REF,
							 logical force=FALSE
							 ) const;

	// STI rr (03/04/2004) pcurves of par_int_curs can be made with zero fitol.
	virtual pcurve *pcur(
				int,
				logical = FALSE
			) const;
	// STI rr end.

	// Check for any data errors in the curve, and correct the errors if
	// possible.  The various arguments provide control over which checks
	// are made, which fixes can be applied and which fixes were actually
	// applied.  The function returns a list of errors that remain in the
	// curve on exit.
	// See chk_stat.hxx for information on the argument types used here.

	virtual	check_status_list*	check(
  		        const check_fix& input = *(const check_fix*) NULL_REF,
						 // supplies a set of flags which say which fixes
						 // are allowable (the default is to fix nothing)
				check_fix& result = *(check_fix*) NULL_REF,
						 // returns a set of flags which say which fixes
						 // were applied
				const check_status_list* = (const check_status_list*) NULL_REF
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
				D3_istream &,
				int_cur *&
			);

/*
// tbrv
*/
/**
 * @nodoc
 */
	friend DECL_KERN D3_ostream& operator<<(
				D3_ostream &,
				int_cur const &
			);

/*
// tbrv
*/
/**
 * @nodoc
 */
	virtual void input(
				D3_istream &
			);

/*
// tbrv
*/
/**
 * @nodoc
 */
	virtual void print(
				D3_ostream &
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
				char const *,
				logical,
				FILE *
			) const;
/*
// tbrv
*/
/**
 * @nodoc
 */
	void debug_data(
				char const *,
				logical,
				FILE *
			) const;
};


// Now a class to fit a 3d curve to the SPAparameter curve in a surface,
// given a SPAparameter SPAinterval on the defining curve and the number of
// (equally-spaced) points in the SPAinterval to be fitted.

/**
 * @nodoc
 * Fits a 3D curve to the parameter curve.
 * <br><br>
 * <b>Role:</b> This class fits a 3D curve to the parameter curve in a surface given a parameter
 * interval on the defining curve and the number of (equally-spaced) points int he interval to be
 * fitted.
 * <br><br>
 * @see pcurve
 */
class DECL_KERN par_int_interp: public curve_interp {

private:
	pcurve const *par_cur;	// defining SPAparameter-space curve

	logical par_sur1;		// TRUE if sur1 and par1 will define the
							// curve, FALSE if it is sur2 and par2.


public:
	// Constructor providing all the information required.

/**
 * C++ constructor, creating a <tt>par_int_interp</tt> using the specified parameters.
 * <br><br>
 * <b>Role:</b> Constructs an interpolated curve, given all the necessary information. If logical
 * is <tt>TRUE</tt> then the first surface is projected; if it is <tt>FALSE</tt>, then the second surface is
 * projected.
 * <br><br>
 * @param pspace
 * parameter-space curve.
 * @param pts
 * number of points.
 * @param cur_para
 * curve parameter range.
 * @param tol
 * fit tolerance.
 * @param proj_sur
 * projected surface.
 */
	par_int_interp(
			pcurve const &pspace,			// defining SPAparameter-space curve
			int pts,					// number of points
			SPAinterval const &cur_para,
			double tol,					// tolerance for fit
			logical proj_sur = TRUE			// TRUE to make the surface #1,
									// FALSE for surface #2
		);

	// Virtual destructor

/**
 * C++ destructor, deleting a <tt>part_int_interp</tt>.
 */
	~par_int_interp();

	// Find the true-point in 3d for a given SPAparameter value.

/**
 * Finds the true-point in 3D for a given parameter value.
 * <br><br>
 * <b>Role:</b> The input position, direction, and parameter values are approximate; the exact
 * values are provided as output.
 * <br><br>
 * @param tol
 * tolerance.
 * @param pt
 * point data.
 */
	void true_point(
			double tol,			// tolerance
			point_data &pt	// approximate SPAposition, direction,
							// SPAparameter values on input, exact values
							// on output
		) const;

	// Construct the appropriate int_cur subclass object (in this
	// case a par_int_cur) from the data in this object after curve
	// interpolation.

/**
 * Constructs the appropriate <tt>int_cur</tt> subclass object (in this case, a <tt>par_int_cur</tt>) from the data in this object after curve interpolation.
 */
	int_cur *make_int_cur();
};

/** @} */
#endif
