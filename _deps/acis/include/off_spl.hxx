/* ORIGINAL: acis2.1/sg_husk/offsur/off_spl.hxx */
/* $Id: off_spl.hxx,v 1.38 2002/08/20 15:20:47 jenglund Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Class to represent the offset of a surface.  This is derived from
// the base class spl_sur, which is used by the spline surface class to
// contain the surface description proper.  If S(u,v) is the original
// surface, the offset surface O(u,v) is
//
//		O(u,v) = S(u,v) + d * N(u,v).
//
// Where N(u,v) is the surface normal and d is the offset distance.

#if !defined( off_spl_sur_CLASS )
#define off_spl_sur_CLASS

#include <stdio.h>

#include "dcl_kern.h"
#include "spldef.hxx"

#include "param.hxx"
#include "position.hxx"
/**
* @file off_spl.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISSURFACES
 *
 * @{
 */

class BOUNDED_SURFACE;

// STI ROLL
class SizeAccumulator;
// STI ROLL

class spl_sur_data_container;

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN subtype_object *restore_off_spl_sur();
#if defined D3_STANDALONE || defined D3_DEBUG
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN D3_istream& operator>>( D3_istream &, spl_sur *& );
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN D3_ostream& operator<<( D3_ostream &, spl_sur const & ); 
#endif
/**
 * Represents the offset of a surface.
 * <br>
 * <b>Role:</b> This class represents the offset of a surface. This is derived
 * from the base class, <tt>spl_sur</tt>, which is used by the spline surface class to
 * contain the surface description proper.
 * If <tt>S(u,v)</tt> is the original surface, the offset surface <tt>O(u,v)</tt> is:
 * <tt>O(u,v) = S(u,v) + d* N(u,v)</tt>
 * where <tt>N(u,v)</tt> is the surface normal and <tt>d</tt> is the offset distance. Along with
 * the benefits of having evaluators based off the offset definition, the <tt>off_spl_sur</tt>
 * class simplifies offsets of offset surfaces. So, if the user offsets the progenitor
 * surface by some distance <tt>(d)</tt>, and then offsets that surface by the distance <tt>(-d)</tt>,
 * the result is the progenitor surface again. Also, if the progenitor is offset
 * by the distance <tt>(d1)</tt> and the offset surface is then offset by the distance
 * <tt>(d2)</tt>, the resulting surface is an offset of the progenitor by the distance
 * <tt>(d1 + d2)</tt>.
 *<br><br>
 * @see surface
 */
class DECL_KERN off_spl_sur: public spl_sur {
// Allow extensions to declare themselves as friends. USE WITH EXTREME CAUTION
#ifdef off_spl_sur_FRIENDS
off_spl_sur_FRIENDS
#endif

	friend class SVEC;

private:
	surface* progenitor;	// surface being offset
    BOUNDED_SURFACE* bsf;	// Bounded part of progenitor surface
	BOUNDED_SURFACE* bsf_approx;	// Bounded part of progenitor's approximation surface, used for fast evaluation
	double offset_dist;		// offset distance
	logical prog_reversed;	// whether or not the progenitor is reversed
	// STI let (r4217): Added data member
	logical reflected;		// whether or not the progenitor was reflected
							// after creation

	logical approx_offset;      // flag indicating that progenitor is procedual or not
							// procedual is of type spline but not exact_spl_sur

	SPApar_box safe_range;

	logical adaptive_offset; // flag to allow adaptive offset project to be applied

	logical did_adaptive_offset; // flag to indicate that the progenitor ran through the adaptive offset project.

	// wolf 04-19-05
	// flag for checking illegal bispan, supports of offset blends need not pass this check
	logical check_illegal_bispan; 

	mutable spl_sur_data_container * make_approx_failure;

	// Constructors.

	// Construct a generally null object, preparatory to filling it
	// in.

	off_spl_sur();


    // Calculate the discontinuity information from the underlying surface.

    virtual	void 	calculate_disc_info();


public:
	// A constructor given all the basic data required.
    /**
     * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
     *<br><br>
     * @param org_surf
     * original surface.
     * @param offset
     * offset distance.
     * @param parameter
     * parametric box.
     */
	off_spl_sur(
				const surface & org_surf,
				double offset,
				const SPApar_box * parameter = NULL,
				logical use_approximate_progenitor = FALSE,
				logical subsetted = FALSE,
				logical in_adaptive_offset = FALSE,
				logical did_adaptive_offset = FALSE
			);

	// A constructor given all the basic data required and the
    // approximating bs3_surface and its fit tolererance.
    /**
     * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
     *<br><br>
     * @param offset_surface
     * surface being offset.
     * @param offset
     * offset distance.
     * @param approx_bs3
     * approximating bs3 surface.
     * @param tol
     * fit tolerance to bs3.
     * @param parameter
     * parameter region.
     */
	off_spl_sur(
				const surface & offset_surface,
				double offset,
		        bs3_surface approx_bs3,
		        double tol,
				const SPApar_box * parameter = NULL,
				logical use_approximate_progenitor = FALSE,
				logical subsetted = FALSE
			);

	// Copy constructor. Derived classes should also define copy constructors,
	// and use this one, to ensure that spl_sur data is initialised.
    /**
     * C++ copy constructor requests memory for this object and populates it with the data supplied as arguments.
     *<br><br>
     * @param ptr_cpy
     * pointer to copy.
     */
    off_spl_sur( const off_spl_sur& ptr_cpy);

	// Make a copy without sharing subdata.
    /**
     * Creates a copy of an item that does not share any data with the original.
     *<br><br>
     * <b>Role:</b> Allocates new storage for all member data and any pointers.
     * Returns a pointer to the copied item.
     *<br><br>
     * @param pm
     * list of items within the entity that are already deep copied.
     */
	virtual spl_sur *deep_copy(pointer_map * pm = NULL) const;

	// Routines to give access to offset definition
    /**
     * Returns the progenitor (the surface being offset).
     */
	const surface* get_progenitor() const;
	/**
	 * Returns the offset distance from the surface.
	 */
	double get_offset_distance() const;
	/**
	 * Is the approximation used to evaluate the surface or not.
	 */
	const logical get_approx_offset() const;

	logical get_did_adaptive_offset() const;

    // The off_spl_sur may fail to evaluate a point on the true surface if
    // the evaluation parameters are beyond the legal bounds of a surface.
    // In this case, an approximate point and derivatives are evaluated
    // (generated from the approximating surface).  Then this flag is set
    // to FALSE; otherwise it is TRUE.  The flag always refers to the
    // previous evaluation.   PC May-00.

    // The mechanism descibed above did not work in conjunction with evaluator
    // caching, and has been replaced by a flag which prescribes the action to
    // be taken if an illegal surface point is found.  abort_on_failure is
    // FALSE by default, in which case the evaluator attempts to recover.  If
    // the flag is set, then the evaluator returns -99 on an illegal point.
    /*
    // tbrv
    */

    /**
     * @nodoc 
     */
    logical abort_on_failure;

    /**
     * @nodoc 
     */
    logical illegal_eval_found;

	// STI ROLL
	/*
	// tbrv
	*/

	/**
	 * @nodoc
	 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL
/**
 * @nodoc
 */
	virtual void minimize( minimize_helper*);

private:
	// Destroy the surface. The parent destructor handles the spline
	// approximation.

	virtual ~off_spl_sur();

    // Function to determine if this spl_sur contains a pipe surface.

    virtual logical contains_pipe() const;

	// Duplication. Can't be done by constructor, as we want it
	// to be virtual.

	virtual subtrans_object *copy() const;


	// Test for equality. This does not guarantee that all
	// effectively equal surfaces are determined to be equal, but
	// does guarantee that different surfaces are correctly
	// identified as such.

	logical operator==( subtype_object const & ) const;


	// Transformation. The base class transforms the spline and fit
	// tolerance: we must handle the underlying surface.

	virtual void operator*=( SPAtransf const & );

	// Parameter shift: adjust the spline surface to have a SPAparameter
	// range increased by the argument value (which may be negative).
	// This is only used to move portions of a periodic surface by
	// integral multiples of the period, so "shift_v" will never be
	// used, and so we can leave it undefined here.

	virtual void shift_u( double );
	virtual void shift_v( double );


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

    // Divide a surface into separate pieces which are smooth (and therefore
    // suitable for offsetting or blending). The surface is split at its non-G1
    // discontinuities, and if it is closed after this, it is then split into
    // two. The functions return the number of pieces, and the pieces themselves
    // are a return argument.


    int split_at_kinks( spl_sur**& pieces, logical udir,
				double curvature = 0.0 ) const;



	// Concatenate the contents of two surfaces into one. This is only
	// called after splitting a periodic surface, to rejoin the pieces
	// the other way round.

	virtual void append_u( spl_sur & );
	virtual void append_v( spl_sur & );


	// Geometric evaluation.

	// Evaluate the normal at the given point.

// STI let (r4217): Added member function
	virtual SPAunit_vector point_normal(
				SPAposition const &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF
			) const;


	// Find the principal axes of curvature of the surface at a
	// given point, and the curvatures in those directions. The base
	// class finds the SPAparameter value, and then uses eval_prin_curv.

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
	// from the given SPAparameter values (if supplied). The cross-
	// section curve is determined by the intersection of the surface
	// with a plane passing through the point on the surface and with
	// given normal. The default uses the principal curvatures from
	// point_prin_curv, which is the best we can do.

//	virtual double point_cross(
//				SPAposition const &,
//				SPAunit_vector const &,
//				SPApar_pos const & = *(SPApar_pos *)NULL_REF
//			) const;


	// Find the point on the surface nearest to the given point,
	// iterating from the given SPAparameter values (if supplied).
	// Return the found point, the normal to the surface at that
	// point and the SPAparameter values at the found point.

// STI let (r4217): Added member function
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


	// Return a cone bounding the normal direction of the surface. The
	// cone is deemed to have its apex at the origin, and has a given
	// axis direction and (positive) half-angle. If the logical
	// argument is TRUE, then a quick approximation may be found. The
	// approximate result may lie wholly within or wholly outside the
	// guaranteed bound (obtained with a FALSE argument), but may not
	// cross from inside to outside. Flags in the returned object
	// indicate whether the cone is in fact the best available, and
	// if not whether this result is inside or outside the best cone.
	// Note that this cannot be const, as it uses subset(), which is
	// itself not const.

// STI let (r4217): Added member function
	virtual surf_normcone normal_cone(
				SPApar_box const & = *(SPApar_box *)NULL_REF,
				logical = FALSE
			);

	// Return a direction which points outward from the surface. This
	// should be the outward normal if the point is not singular,
	// otherwise a fairly arbitrary outward direction.

// STI let (r4217): Added member function
	virtual SPAunit_vector eval_outdir(
				SPApar_pos const &
			) const;

	// Find the SPAparameter values of a point on the surface.

	virtual SPApar_pos param(
				SPAposition const &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF
			) const;


	// Find the change in surface SPAparameter corresponding to a unit
	// offset in a given direction at a given SPAposition, the SPAposition
	// and direction both lying in the surface. The default evaluates
	// the first derivatives, and inverts.

//	virtual SPApar_vec param_unitvec(
//				SPAunit_vector const &,
//				SPApar_pos const &
//			) const;


	// Find the SPAposition and first and second  derivatives of the
	// surface at a given point.

// 	virtual void eval(
// 				SPApar_pos const &uv,
// 				SPAposition &pos,
// 				SPAvector *dpos,	// first derivatives - array of
// 								// length 2 in order xu, xv
// 				SPAvector *ddpos	// second derivatives - array of
// 								// length 3 in order xuu, xuv, xvv
// 			) const;
//

	// Find the point on the surface with given SPAparameter values.
	// The default uses eval().

//	virtual SPAposition eval_position(
//				SPApar_pos const &
//			) const;


 	// Find the normal to the surface at the point with given
	// SPAparameter values.  We need to take the left-handedness
	// into account.

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
	// the surface. The default uses eval_prin_curv, which is fine
	// for us.

//	virtual double eval_cross(
//				SPApar_pos const &,
//				SPAunit_vector const &
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


private:

	int evaluate_linear_extension(
                SPAposition &pos,		// Point on surface at given SPAparameter
                SPAvector derivs[4][8],  	// Array of pointers to arrays of
									// vectors, of size nd. Any of the
									// pointers may be null, in which
									// case the corresponding derivatives
									// will not be returned. Otherwise
									// they must point to arrays long
									// enough for all the derivatives of
									// that order - i.e. 2 for the first
									// derivatives, 3 for the second, etc.
                int nd,       		// Number of derivatives required 
				logical u_extend,
				logical v_extend,
				double delta_u,
				double delta_v
			    ) const;

	// Indicate whether the SPAparameter coordinate system of the surface
	// is right- or left-handed. With a right-handed system, at any
	// point the outward normal is given by the cross product of the
	// increasing u direction with the increasing v direction, in that
	// order. With a left-handed system the outward normal is in the
	// opposite direction from this cross product.

	virtual logical left_handed_uv() const;


    // Determine the closure and singularity properties of the surface.
    // The results are stored in the u_closure, v_closure, u_singularity
    // and v_singularity flags.

    void	determine_closure_and_singularity();

// STI let (12/98) (slic025): Removed u_ and v_param_line methods
// The methods in the base class are preferrable.

	// Test whether a point lies on the surface, within a user-defined
	// tolerance.

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

public:
    /**
     * Returns the <tt>ID</tt> for the <tt>off_spl_sur list</tt>.
     */
	static int id();
	/**
	 * Returns the type of <tt>off_spl_sur</tt>.
	 */
	virtual int type() const;
    /**
     * Returns the string <tt>"offsur"</tt>.
     */
	virtual char const *type_name() const;
     /**
      * @nodoc
     */
	virtual logical need_save_as_approx(int save_to_version, logical check_progenitors) const;
    /**
     * Saves the information for the <tt>off_spl_sur</tt> to the save file.
     */
	virtual void save_data() const;

	/**
      * @nodoc
     */
	void make_approx_for_testing(double reqd_fit, const spline& spl, 
				  logical full_surf, logical keep_approx, 
                  logical illegal_bispan_check, logical abort_on_illegal = TRUE,
				  logical proceed_on_failure = FALSE, 
                  logical& failure_ignored = *(logical*)NULL_REF ) const;
	/**
      * @nodoc
     */
	logical check_surface(logical keep_approx = FALSE);

	/**
      * @nodoc
     */
	virtual void process(geometry_definition_processor& p) const;

protected:

	// Make or remake the approximating surface.  The force flag forces the
	// approximating surface to be made even if it is illegal.  This can be
	// used to restore old parts that were not checked properly before being
	// saved.  The spline argument 'spl' may be null but if it is supplied the
	// function may be a little faster.  The function stores the approximating
    // surface and the actual fit error that was achieved in the spl_sur,
	// overriding the declared const-ness of the function to do this.
	// The argument keep_approx allows the approximation to be made and passed
	// out even when it is not valid. Should only be used for adaptive offsetting.

    void	local_make_approx(
							 double fit,
							 const spline& spl  = *(spline*) NULL_REF,
						     logical force = FALSE,
							 logical keep_approx = FALSE,
							 logical illegal_bispan_check = FALSE,
                             logical abort_on_illegal = TRUE,
							 logical proceed_on_failure = FALSE,
							 logical& failure_ignored = *(logical*)NULL_REF
							 ) const;

	virtual void	make_approx(
							 double fit,
							 const spline& spl  = *(spline*) NULL_REF,
						     logical force = FALSE
							 ) const;


private:
	friend DECL_KERN subtype_object *restore_off_spl_sur();
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

// STI swa 27Jul98 -- return sweep information, if applicable
public:
    /**
     * Returns the path type used used for the <tt>sweep</tt> operation.
     */
    virtual sweep_path_type get_path_type() const;
    /**
     * Returns the path used for the offset. The caller has ownership of the returned curve and needs to cleanup to avoid memory leaks.
     */
    virtual curve * get_path() const;
    /**
     * Returns the rail used for profile orientation in the sweep operation.
     */
    virtual law * get_rail() const;
// STI swa END

};


// This function tests whether a surface (presumably the progenitor) is smooth.  If it is not,
// an offset surface cannot be made from it.  In this case, the function returns a range which
// IS valid.  If a 'must-have' SPAposition is provided, then the returned range will contain it,
// otherwise the largest possible range is returned.

/*
// tbrv
*/
/**
 * @nodoc
 */
extern DECL_KERN SPApar_box reduce_range_if_surface_not_G1(
							const surface& sf,
							const SPApar_box& range,
							double offset,
							const SPApar_pos& must_have = *(const SPApar_pos*)NULL_REF );


// This function tests whether a surface (presumably the progenitor) is smooth. (the marked discontinuities are real or not.  

/*
// tbrv
*/
/**
 * @nodoc
 */
extern DECL_KERN logical real_G1_discs(const surface& sf,
					  const SPApar_box& range,
					  double offset,
					  int &nu_real,
					  int &nv_real,
					  double*& real_udisc = *(double **)NULL_REF,
					  double*& real_vdisc = *(double **)NULL_REF
						  );



/** @} */
#endif
