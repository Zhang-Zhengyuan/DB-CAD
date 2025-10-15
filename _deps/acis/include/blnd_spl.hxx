/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// This file resided formerly in the advanced blending husk, but the geometry
// definitions need to be generally available.
#if !defined BLEND_SPL_SUR_HXX
#define BLEND_SPL_SUR_HXX
#include "acis.hxx"
#include "dcl_kern.h"
#include "logical.h"
#include <stdio.h>
#include "surdef.hxx"
#include "spldef.hxx"
#include "curdef.hxx"
#include "position.hxx"
#include "param.hxx"
#include "interval.hxx"
#include "subtrans.hxx"
#include "bs2curve.hxx"
#include "bs3curve.hxx"
#include "blnd_eval_ver.hxx"
#include "bl_cntct_sec.hxx"
/**
 * @file blnd_spl.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISSURFACES
 *
 * @{
 */
class var_cross_section;
class var_radius;
class BOUNDED_CURVE;
class BOUNDED_SURFACE;
class CVEC;
class SVEC;
class SPAtransf;
class SPAvector;
class SPAbox;
class var_blend_spl_sur;
class v_bl_contacts;
class list_of_contacts;
class intcurve;
class SPAcrvGeom;
class SizeAccumulator;
class var_rad_three_entity;
class blend_section;

/*
// tbrv
*/
/**
 * @nodoc
 */
typedef enum
    {
	BLEND_SUPPORT_ID,
	BLEND_SUPPORT_SURFACE_ID,
	BLEND_SUPPORT_CURVE_ID,
	BLEND_SUPPORT_ZERO_CURVE_ID,
	BLEND_SUPPORT_POINT_ID,
	BLEND_SUPPORT_CURVE_ON_SURFACE_ID
    } 
    eblend_support_id;


#if defined D3_DEBUG || defined D3_STANDALONE
class	D3_ostream;
class blend_support;
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN D3_ostream &operator<<(D3_ostream &os, const blend_support& bs );
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN D3_istream &operator>>(D3_istream &is, blend_support& bs );
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
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN subtype_object *restore_blend_spl_sur();

// The blend support class represents the geometry that a spring curve of a
// blend lies on. It will be either a surface, a curve or a point. In general,
// only one of these will be stored in the class, although there is room for all
// three to be stored. This is to allow the administrative functions such as
// save and restore to be implemented just once, in the base class.
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN blend_support : public ACIS_OBJECT
	{
	// Allow extensions to declare themselves as friends. USE WITH 
	// EXTREME CAUTION

#ifdef blend_support_FRIENDS
	blend_support_FRIENDS
#endif

protected:

    BOUNDED_CURVE *_bcu;
    CVEC *_cvec;
    BOUNDED_SURFACE *_bsf;
    SVEC *_svec;

private:

    // This is where the results of the last evaluation are cached.

	double _t;				// SPAparameter of last evaluation
	int _side;				// side of last evaluation
	int _nderivs;			// no. of derivs cached
	SPAposition _P2;		// cached SPAposition
	SPAvector *_derivs;		// array of cached derivs
	int _deriv_store_len;	// no. of elements in _derivs array

private:

    // This is the "real" evaluator that derived classes must implement.

    virtual	int evaluate_internal( 
                    CVEC &def_cvec,
                    double radius,
                    SPAposition &P,
			        SPAvector *derivs,
                    int nd = 0,
                    logical constrain_eval = FALSE
                    ) = 0;

	// Private version of save and restore. Allows not saving/restoring of
	// the support pcurve corresponding to the blend spring.

/**
 * @nodoc
 */
    void save_data( logical save_pcur ) const;

/**
 * @nodoc
 */
    void restore_data( logical restore_pcur );

public:

    surface *srf;
    curve *crv;
    SPAposition pt;

	// A bs2_curve will be present if the blend support is a parametric surface.
	// It represents the spring curve in the SPAparameter space of the surface.
	// The bs2_curve may be left null when the blend_support is first made, and
	// inserted later.

	// NOTE: in future, we may decide to use the bs2_curve when the support
	// entity is a curve, to reparametrise the curve in the blend surface
	// SPAparameter.

    bs2_curve bs2;

	// Constructors. Provide a null constructor so that data may be set in any
	// way which is convenient.

    blend_support();

	// Constructor given all the data (any amount of which may be NULL). The
	// curves and surfaces, which are passed by reference, are copied, and the
	// bs2_curve.

    blend_support( 
            surface const &sf,
		    curve const &cu,
		    SPAposition const &p,
		    bs2_curve b,
			SPApar_box const &pbx =
				*(SPApar_box const *)NULL_REF,
			SPAinterval const &r =
				*(SPAinterval const *)NULL_REF
		    );

    // Destructor to delete the surface, curve, bs2_curve, BOUNDED_CURVE and cvec.

    virtual	~blend_support();

    // Evaluator function, called from the blend_spl_sur evaluator. A cvec on
    // the reference curve is provided. The evaluator evaluates the spring curve
    // SPAposition and nd derivatives, wrt the blend surface SPAparameter. If
    // the support is a curve then 'evaluate' also sets the cvec; if a surface,
    // it sets the svec.

    // The evaluator is non-virtual because it provides a caching mechanism so
    // that repeat evaluations at the same spine SPAparameter (but for different
    // transverse SPAparameter on the surface) will be quick.

    int evaluate( 
            CVEC &def_cvec,
            double radius,
            SPAposition &P,
			SPAvector *derivs = NULL,
            int nd = 0,
            logical constrain_eval = FALSE
            );

/**
 * @nodoc
 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;

/**
 * @nodoc
 */
	virtual void minimize( minimize_helper *mh );

public:

    CVEC &cvec() const { return *_cvec; }
    SVEC &svec() const { return *_svec; }

    // Member-setting functions.

    // These assume ownership of the given surfaces.
    // If the caller doesn't want to lose them, they must make copies.

    // Replace the surface.

	void set_surface( 
				surface *sf, 
				SPApar_box const &pbx = 
					*(SPApar_box const *)NULL_REF 
				);

    // Replace the curve.

	void set_curve( 
				curve *cu, 
				SPAinterval const &r = 
					*(SPAinterval const *)NULL_REF  
				);

    // Now the administrative functions that are required to support their
    // counterparts in blend_spl_sur.

    // Construct a duplicate in free store of this object.

    virtual blend_support *copy() const = 0;

	// Similar to the above but the underlying data is not shared.

	virtual blend_support *deep_copy( pointer_map *pm = NULL ) const = 0;

    // Test for equality. This does not guarantee that all effectively equal
	// blend_supports are determined to be equal, but does guarantee that
	// different blend_supports are correctly identified as such.

    virtual logical operator==( blend_support const &rhs ) const;

    // Transform this blend_support by the given SPAtransf.

    virtual void operator*=( SPAtransf const &trf );

	// Extend this blend support. This is for use by constant round
	// blend surfaces. They pass the newly extended spine and whether
	// this is the left or right support. The should be ok for
	// everything except the blend_support_zero_curve. Not sure about
	// the blend_support_curve_on_surface as none are ever made (yet).
    // Also passes whether the blend_spl_sur is left handed or not.

	virtual void extend( curve const &spine, logical left, logical left_handed );

	// A *hopefully* more generic version of the above. Let the blend
	// support update its data after updating the blend surface.

	virtual void update( 
					blend_spl_sur const &spl,
					SPApar_box const &pbx = 
						*(SPApar_box const *)NULL_REF, 
					SPAinterval const &r = 
						*(SPAinterval const *)NULL_REF    
					);

	// *NNN* Another overloaded extend used by var_radius blend supports

    // The normal virtual function mechanism. Restore is more complicated,
    // because until it is invoked we do not have an object of the right class.

	static int id();
	virtual int type() const;

    virtual char const *type_name() const = 0;

	// Save and restore. Save is easy, as derived class switching goes through
	// the normal virtual function mechanism. Restore is more complicated,
	// because until it is invoked we do not have an object of the right class.
	// Instead we switch on a table defined by static instances of the
	// restore_ss_def class (see below), to invoke a simple friend function
	// which constructs an object of the right (derived) type. Then it can call
	// the appropriate member function to do the actual work.

/**
 * Saves the information associated with this <tt>blend_support</tt> to a SAT file.
 */

    virtual void save_data() const;

/**
 * @nodoc
 */
	virtual logical need_save_as_approx(
									int save_to_version, 
									logical check_progenitors
									) const;

/**
 * Restores the data from a save file.
 * <br><br>
 * <b>Role:</b> The restore operation switches on a table defined by static instances
 * of the <tt>restore_subtype_def</tt> class. This invokes a simple friend function which
 * constructs an object of the right derived type. Then it calls the appropriate base
 * class member function to do the actual work.
 */
    void restore_data();

#if defined D3_DEBUG || defined D3_STANDALONE

    friend DECL_KERN D3_ostream &operator<<( D3_ostream &os, const blend_support& bs );
    friend DECL_KERN D3_istream &operator>>( D3_istream &is,  blend_support& bs );

	virtual void input(
				D3_istream &
			);

	virtual void print(
				D3_ostream &
			) const;
#endif

    // Debug printout.

    void debug( char const *, logical, FILE * ) const;

    friend class blend_spl_sur;
	};


// The blend support class is a pure base class. Now come the derived classes,
// for a surface, a curve and a point respectively.
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN blend_support_surface : public blend_support 
	{
	// Allow extensions to declare themselves as friends. USE WITH 
	// EXTREME CAUTION

#ifdef blend_support_surface_FRIENDS
blend_support_surface_FRIENDS
#endif

public:

    blend_support_surface();

    blend_support_surface( 
					surface const &sf, 
					bs2_curve b = NULL,
					SPApar_box const &pbx =
						*(SPApar_box const *)NULL_REF
					);

    virtual	~blend_support_surface();

	// Functions for getting/setting flag indicating whether this 
	// support_surface contains the holdline curve for holdline 
    // blending.

	logical get_has_holdline() { return _has_holdline_curve; }
	void set_has_holdline( logical hl ) { _has_holdline_curve = hl; }

private:

    // Evaluator function, called from the blend_spl_sur evaluator. A 
    // point on the support surface, in the plane defined by the reference 
    // curve, is returned, together with derivatives with respect to the 
    // parameter. The svec on the support surface is set following the call 
    // to evaluate.

    virtual	int evaluate_internal(
							CVEC &def_cvec,
							double radius,
							SPAposition &P,
							SPAvector *derivs,
							int nd = 0,
							logical constrain_eval = FALSE
							);

    int evaluate_constrained_derivs(
							CVEC &def_cvec,
							SPAvector *derivs,
							int nd = 0
							);

	logical _has_holdline_curve;


public:
    
    // Now the administrative functions that are pure virtual 
    // in the base class.

    // Construct a duplicate in free store of this object.

    virtual blend_support *copy() const;

	// Similar to the above but the underlying data is not shared.

	virtual blend_support *deep_copy( pointer_map *pm = NULL ) const;

	static int id();
	virtual int type() const;

    // Give a string identifying this type of blend_support

    virtual char const *type_name() const;
	};

/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN blend_support_curve : public blend_support 
	{
	// Allow extensions to declare themselves as friends. USE WITH EXTREME 
	// CAUTION

#ifdef blend_support_curve_FRIENDS
	blend_support_curve_FRIENDS
#endif

public:

    blend_support_curve();

    blend_support_curve( 
			curve const &cu, 
			bs2_curve = NULL,
			SPAinterval const &r =
				*(SPAinterval const *)NULL_REF 
			);

    // Do-nothing destructor.

    virtual	~blend_support_curve();

private:

    // Evaluator function, called from the blend_spl_sur evaluator. A 
    // point on the support curve, in the plane defined by the reference 
    // curve, is returned, together with derivatives with respect to the 
    // parameter. The cvec on the support curve is set following the call 
    // to evaluate.

    virtual	int evaluate_internal( 
                    CVEC &def_cvec,
                    double radius,
                    SPAposition &P,
			        SPAvector *derivs,
                    int nd = 0,
                    logical constrain_eval = FALSE 
                    );

public:

    // Now the administrative functions that are pure virtual in the 
    // base class.

    // Construct a duplicate in free store of this object.

    virtual blend_support *copy() const;

	// Similar to the above but the underlying data is not shared.

	virtual blend_support * deep_copy( pointer_map * pm = NULL)const;

	static int id();
	virtual int type() const;

	// Give a string identifying this type of blend_support

    virtual char const *type_name() const;
    };

/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN blend_support_zero_curve : public blend_support 
	{
	// Allow extensions to declare themselves as friends. USE WITH EXTREME 
	// CAUTION

#ifdef blend_support_zero_curve_FRIENDS
	blend_support_zero_curve_FRIENDS
#endif

public:

    blend_support_zero_curve();

	blend_support_zero_curve( 
					curve const &cu,
					SPAinterval const &r =
						*(SPAinterval const *)NULL_REF
					);

    // Do-nothing destructor.

    virtual	~blend_support_zero_curve();

private:

    // Evaluator function, called from the blend_spl_sur evaluator. A 
    // point on the support curve, in the plane defined by the reference 
    // curve, is returned, together with derivatives with respect to the 
    // parameter. The cvec on the support curve is set following the call 
    // to evaluate.

    virtual	int evaluate_internal( 
                    CVEC &def_cvec,
                    double radius,
                    SPAposition &P,
			        SPAvector *derivs,
                    int nd = 0,
                    logical constrain_eval = FALSE 
                    );

public:

    // Now the administrative functions that are pure virtual in the 
    // base class.

    // Construct a duplicate in free store of this object.

    virtual blend_support *copy() const;

	// Similar to the above but the underlying data is not shared.

 	virtual blend_support *deep_copy( pointer_map * pm = NULL ) const;

	// Extend the zero curve to match the spine.

	virtual void extend( curve const &spine, logical left, logical left_handed );

	static int id();
	virtual int type() const;

	// Give a string identifying this type of blend_support

    virtual char const *type_name() const;
	};

/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN blend_support_point : public blend_support 
	{
	// Allow extensions to declare themselves as friends. USE WITH EXTREME 
	// CAUTION

#ifdef blend_support_point_FRIENDS
	blend_support_point_FRIENDS
#endif

public:

    blend_support_point();

    blend_support_point( SPAposition const &p );

    // Do-nothing destructor.

    virtual	~blend_support_point();

private:

    // Evaluator function, called from the blend_spl_sur evaluator.

    virtual	int evaluate_internal( 
                    CVEC &def_cvec,
                    double radius,
                    SPAposition &P,
			        SPAvector *derivs,
                    int nd = 0,
                    logical constrain_eval = FALSE 
                    );

public:

    // Now the administrative functions that are pure virtual in the base class.

    // Construct a duplicate in free store of this object.

    virtual blend_support *copy() const;

	// Similar to the above but the underlying data is not shared.

	virtual blend_support *deep_copy( pointer_map *pm = NULL ) const;

	static int id();
	virtual int type() const;

	// Give a string identifying this type of blend_support

    virtual char const *type_name() const;
	};


// Derived class for the curve-on-surface case.
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN blend_support_curve_on_surface : public blend_support 
	{
	// Allow extensions to declare themselves as friends. USE WITH EXTREME 
	// CAUTION

#ifdef blend_support_curve_on_surface_FRIENDS
	blend_support_curve_on_surface_FRIENDS
#endif

public:

    blend_support_curve_on_surface();

    blend_support_curve_on_surface( 
                surface const &sf,
				curve const &cu,
				bs2_curve b = NULL,
				SPApar_box const &pbx =
					*(SPApar_box const *)NULL_REF,
				SPAinterval const &r =
					*(SPAinterval const *)NULL_REF
				);

    // Do-nothing destructor.

    virtual	~blend_support_curve_on_surface();

private:

    // Evaluator function, called from the blend_spl_sur evaluator. A 
    // point on the support curve, in the plane defined by the reference 
    // curve, is returned, together with derivatives with respect to the 
    // parameter. The cvec on the support curve is set following the call 
    // to evaluate.

    virtual	int evaluate_internal( 
                    CVEC &def_cvec,
                    double radius,
                    SPAposition &P,
			        SPAvector *derivs,
                    int nd = 0,
                    logical constrain_eval = FALSE 
                    );

public:

    // Now the administrative functions that are pure virtual in the 
    // base class.

    // Construct a duplicate in free store of this object.

    virtual blend_support *copy() const;

	// Similar to the above but the underlying data is not shared.

	virtual blend_support *deep_copy( pointer_map *pm = NULL ) const;

	// Issue an error as we don't yet know if the default is correct or
	// if we need anything special here.

	virtual void extend( curve const &spine, logical left, logical left_handed );

	static int id();
	virtual int type() const;

    // Give a string identifying this type of blend_support

    virtual char const *type_name() const;
	};

// This class is used as a base class for all blend surfaces. It tries to
// predict some of the fields that derived classes will need; for example, it
// contains pointers for a left surface, a left curve and a left point although
// in practice only one of these will be needed in a particular derived class.
// The reason for doing this is that the base class can (probably) completely
// handle the administrative functions such as operator=, save and
// restore, making these trivial for the derived classes.

/**
 * Provides common functionality and data for all blend surfaces.
 * <br>
 * <b>Role:</b> This is an abstract class that tries to predict some of the fields
 * that derived classes will need; for example, it contains pointers for a left surface,
 * a left curve and a left point although in practice only one of these will be needed
 * in a particular derived class. The reason for doing this is that the base class can
 * (probably) completely handle the administrative functions such as operator=, save and
 * restore, making these trivial for the derived classes.
 * @see rb_blend_spl_sur
 */


class DECL_KERN blend_spl_sur : public spl_sur
	{
	// Allow extensions to declare themselves as friends. USE WITH EXTREME CAUTION
#ifdef blend_spl_sur_FRIENDS
	blend_spl_sur_FRIENDS
#endif

protected:

    BOUNDED_CURVE *_def_bcu;
    CVEC *_def_cvec;

	blend_section *_section_data;

	// support_u_param_range - the u parameters of the supports ( not
	// necessarily 0:1 )

	SPAinterval _support_u_param_range;

	// The fitol that was requested when the approx sf was first made. When we
	// extend it, we carry on to this same fitol.

	double initial_fitol;

	// The number of u points sampled in fitting the approx sf. When we extend
	// it, we must continue sampling exactly the same points.

	int initial_num_u_pts;

    // Stores the version information used to select the blend evaluation
    // algorithms.

    blend_eval_version _eval_version;

	intcurve *left_spring;
	intcurve *right_spring;
	intcurve *spine;
	curve *save_spine;
	double new_evaluator_curve_tolerance;
    
    bs3_curve _curvature_curve_approximation;
        

	// Update the defining curve and its cvec.

    void update_def_curve( curve const *c );

	// If the summary information is saved for 3 curve evaluators, we need to
	// reconstruct these curves while restoring the surface.

	virtual void reconstruct_approximating_curves();

	// Calculate the discontinuity information along u

	virtual void calculate_disc_info_u( SPAinterval const &range_of_interest );

	// Calculate the discontinuity information along v

	virtual void calculate_disc_info_v( SPAinterval const &range_of_interest ) = 0;

	// Adjust the given eval side based on v_param, closure of surface,
	// and the eval version

	void adjust_eval_side_v( double v_param, evaluate_curve_side &side ) const;

	// Whether the surface primary data for stabilizing the evaluator like 
	// defining curve and support bs2 curves should be saved/restored from the
	// SAT file.

	virtual logical serialize_primary_evaldata() const { return TRUE; }

	// Whether the center support of the blend should be saved even if it is
	// NULL.

	virtual logical serialize_center_support() const { return FALSE; }

public:

	// A flag recording whether an approximating surface is necessary for this
	// surface. If the surface is only used to support other blends (e.g. a
	// complete pipe surrounding an edge, supporting an edge-face blend) then it
	// will never need an approximating surface (and indeed, the approximating
	// surface may be impossible to create since the blend could be illegal).
//
// If an attempt is made to make an approximating surface for a blend surface
// which thinks it doesn't need one (i.e. this flag is set) then the
// approximating surface will be made, and a warning will be issued.  So the
// flag does not guard the surface from ever getting an approximating surface,
// it merely states that one should not be necessary.
//
// On save and restore, this flag is saved and read from the bs3_surface flag
// in the .sat file.  This is only important if the blend gets saved to a
// pre-5.0 version.  Since 5.0, approximating surfaces can be evaluated lazily
// and whether the approximation should be present is not an issue.  Before 5.0,
// the approximating surface was present if it was necessary, but was not
// present for these special cases of a blend supporting a blend.  When a blend
// is restored into a 4.0 version, the approximating surface MUST be made
// immediately if it might be required later and the bs3_surface flag determines
// this.
//
// The flag is FALSE by default (the constructors set it FALSE).  Applications
// which make a surface which is not intended to have an approximating surface
// can set it by hand. If an approximating surface is generated for the surface,
// then the flag will be unset.
/**
 * Flag to determine whether an approximation is required.
 */

	logical approximation_not_reqd;


// The support entities. These may be a surface, a curve or a point. These have
// to be pointers, to allow classes derived from blend_support to be used.

/**
 * A support entity.
 * <br><br>
 * <b>Role:</b> May be a surface, a curve or a point. Have to be a pointer, to
 * allow classes derived from <tt>blend_support</tt> to be used.
 */

    blend_support *left_support;

/**
 * A support entity.
 * <br><br>
 * <b>Role:</b> May be a surface, a curve or a point. Have to be a pointer, to
 * allow classes derived from <tt>blend_support</tt> to be used.
 */

    blend_support *right_support;

/**
 * A support entity.
 * <br><br>
 * <b>Role:</b> May be a surface, a curve or a point. Have to be a pointer, to
 * allow classes derived from <tt>blend_support</tt> to be used.
 */
	blend_support *center_support;

	// Normally the u direction travels clockwise about the spine
	// direction from the left support to the right one, so that the
	// surface (as given by Pu*Pv) is naturally concave. If the surface
	// is "left_handed", u travels anticlockwise, still from the left
	// support to right, and the surface is naturally convex.
/**
 * Flag to indicate the handedness.
 */

	logical left_handed;

// *NNN* Flag to indicate if the supports are extended.
/**
 * Flag to indicate whether the supports are extended.
 */

	logical supports_extended;


	logical extend_hi_u, extend_lo_u;

// Defining curve (reference curve). For a rolling ball blend, this will be the
// blend spine, i.e. the path of the centre of the ball. def_cvec is a cvec
// on the def_curve which is set by each call to evaluate, and may subsequently
// be used by the application.
/**
 * Defining curve (reference curve).
 * <br><br>
 * <b>Role:</b> For a rolling ball blend, this is the blend spine, i.e., the path
 * of the center of the ball. <tt>def_cvec</tt> is a cvec on the <tt>def_curve</tt> which is set by
 * each call to evaluate, and may subsequently be used by the application.
 */

    curve *def_curve;
/**
 * Returns the CVEC on the defining curve of the blend, which is set each time the blend surface is evaluated.
 */

    CVEC &def_cvec() const { return *_def_cvec; }

// Handle cached data
/**
 * Method for handling section data.
 */

    blend_section &section_data() const	{ return *_section_data; }
/**
 * Method for handling cached blend_section data.
 * <br><br>
 * @param v
 * v parameter.
 * @param spine_nder
 * number of required spine derivs.
 * @param def_nder
 * number of required defining curve derivs.
 * @param spring_nder
 * number of required spring derivs.
 * @param xcrv_norm
 * whether to fill in xcurve normal.
 * @param section
 * all output in here.
 * @param side
 * evaluation side.
 * @param bev
 * blend evaluation version.
 */

	logical	check_cache( double v, int spine_nder, int def_nder,
				 int spring_nder, logical xcrv_norm,
				 blend_section &section, int side, blend_eval_version const& bev ) const;

// Also set by each call to evaluate are cvecs or svecs on the support entities,
// which can be accessed by the following functions. Support cvecs will be set
// if the blend support is a curve. Support svecs will be set if the support is
// a surface.
/**
 * Returns the CVEC on the left or right blend support, if that support is, or contains a curve.
 * <br><br>
 * <b>Role:</b> This CVEC is set each time the blend surface is evaluated.
 * <br><br>
 * @param i
 * left support if 0 otherwise right.
 */

    CVEC &support_cvec( int i )	const
	    { return ( i==0 ? left_support : right_support )->cvec(); }
/**
 * Returns the SVEC on the left or right support, if that support is, or contains a surface.
 * <br><br>
 * <b>Role:</b> This SVEC is set each time the blend surface is evaluated.
 * <br><br>
 * @param i
 * left support if 0 otherwise right.
 */
     SVEC &support_svec( int i )   const
	    { return ( i==0 ? left_support : right_support )->svec(); }

// Objects describing the radius. If the radius is constant then the value of
// the double is used and the var_radius pointer will be zero. Otherwise,
// the value of the double will be ignored. The variable radius objects are
// pointers, so that if the left and right radii are not different,
// then right_rad will point to the same object as left_rad.
// Also, rad == left_rad, always, for convenience.
/**
 * Objects describing the radius.
 * <br><br>
 * <b>Role:</b> If the radius is constant then the value of the double is used and
 * the <tt>var_radius</tt> pointer will be zero. Otherwise, the value of the double will be ignored.
 * The variable radius objects are pointers, so that if the left and right radii are not
 * different, then <tt>right_rad</tt> will point to the same object as <tt>left_rad</tt>. Also, <tt>rad</tt> equals
 * <tt>left_rad</tt>, always, for convenience.
 */

    double left_offset;
/**
 * Objects describing the radius.
 * <br><br>
 * <b>Role:</b> If the radius is constant then the value of the double is used and the
 * <tt>var_radius</tt> pointer will be zero. Otherwise, the value of the double will be ignored.
 * The variable radius objects are pointers, so that if the left and right radii are not
 * different, then <tt>right_rad</tt> will point to the same object as <tt>left_rad</tt>. Also, <tt>rad</tt> equals
 * <tt>left_rad</tt>, always, for convenience.
 */

    double right_offset;
/**
 * Objects describing the radius.
 * <br><br>
 * <b>Role:</b> If the radius is constant then the value of the double is used and the
 * <tt>var_radius</tt> pointer will be zero. Otherwise, the value of the double will be ignored.
 * The variable radius objects are pointers, so that if the left and right radii are not
 * different, then <tt>right_rad</tt> will point to the same object as <tt>left_rad</tt>. Also, <tt>rad</tt> equals
 * <tt>left_rad</tt>, always, for convenience.
 */

    var_radius *rad;
/**
 * Objects describing the radius.
 * <br><br>
 * <b>Role:</b> If the radius is constant then the value of the double is used and the
 * <tt>var_radius</tt> pointer will be zero. Otherwise, the value of the double will be ignored.
 * The variable radius objects are pointers, so that if the left and right radii are not
 * different, then <tt>right_rad</tt> will point to the same object as <tt>left_rad</tt>. Also, <tt>rad</tt> equals
 * <tt>left_rad</tt>, always, for convenience.
 */

    var_radius *left_rad;

/**
 * Objects describing the radius.
 * <br><br>
 * <b>Role:</b> If the radius is constant then the value of the double is used and the
 * <tt>var_radius</tt> pointer will be zero. Otherwise, the value of the double will be ignored.
 * The variable radius objects are pointers, so that if the left and right radii are not
 * different, then <tt>right_rad</tt> will point to the same object as <tt>left_rad</tt>. Also, <tt>rad</tt> equals
 * <tt>left_rad</tt>, always, for convenience.
 */

    var_radius *right_rad;

// Object describing the cross section. If this is zero then the cross section
// is assumed to be circular, or elliptical if the radius functions are not
// equal.

// STI bd: *section must be modified:   const var_cross_section* section;
/**
 * Object describing the cross section.
 * <br><br>
 * <b>Role:</b> If this is zero then the cross section is assumed to be circular,
 * or elliptical if the radius functions are not equal.
 */
     var_cross_section *section;


// The "legal" v range, i.e. that v range over which the surface is
// well-behaved. This is initialised to an infinite SPAinterval, but if
// self-intersecting regions of surface are discovered this range bounds
// the surface away from them. When infinite (note semi-infinite will be
// quite common) this means the surface is legal in that direction as
// far as has been analysed.
/**
 * The "legal" <i>v</i> range is that <i>v</i> range over which the surface is well-behaved.
 * <br><br>
 * <b>Role:</b> This is initialized to an infinite interval, but if self-intersecting regions
 * of surface are discovered, this range bounds the surface away from them. When infinite,
 * this means the surface is legal in that direction as far as has been analyzed.
 * Semi-infinite will be quite common.
 */

	SPAinterval legal_range;

// Constructors.

// Construct a generally null blend surface.
// This is to allow flexibility for constructors for derived classes
// to set the common data members in whatever way is most convenient.

/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 */

    blend_spl_sur();
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param blnd
 * blend spl sur.
 */

    blend_spl_sur( blend_spl_sur const &blnd );
/**
 * @nodoc
 */
    virtual logical is_valid_blend_surface() { return TRUE; }

/**
 * @nodoc
 */
	void deep_copy_elements_blend( blend_spl_sur const &bss, pointer_map *pm = NULL );

	// Make a copy without sharing subdata.
/**
 * @nodoc
 */
	virtual spl_sur *deep_copy( pointer_map *pm = NULL ) const = 0;

/**
 * @nodoc
 */
	virtual void full_size( SizeAccumulator &, logical = TRUE ) const;
	
/**
 * @nodoc
 */
	virtual void minimize( minimize_helper * );

	// Function to publicly set discontinuities in the spl_sur.

	void add_discs_from_array(
                        const logical &is_u,
			            const int &num_discs,
                        const double *discs,
			            const int *orders
                        );

	// Construct a blend_spl_sur given everything that is needed. Only certain
	// combinations of input are valid, but this is not enforced by the constructor.
	// The reason for this is that we anticipate the derived classes will have
	// constructors that ensure only valid combinations are passed through to this
	// constructor. For example, the derived classes are expected to make the blend
	// support entities (on the heap) and pass them to this constructor.
	//
	// The constructor copies the reference curve (which is passed by reference),
	// but assumes ownership of the data that is passed to it by pointer - namely
	// the blend_supports, radius functions and cross sections.

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Only certain combinations of input are valid, but this is not enforced
 * by the constructor. The reason for this is that the derived classes are anticipated
 * and have constructors that ensure only valid combinations are passed through to this
 * constructor. For example, the derived classes are expected to make the blend support
 * entities (on the heap) and pass them to this constructor.
 * <br><br>
 * The constructor copies the reference curve (which is passed by reference), but
 * assumes ownership of the data that is passed to it by pointer - namely the <tt>blend_supports</tt>,
 * radius functions and cross sections.
 * <br><br>
 * @param left_support
 * blend support for left side.
 * @param right_support
 * blend support for right side.
 * @param def_crv
 * defining curve.
 * @param v_range
 * v param range.
 * @param left_offset
 * left offset.
 * @param right_offset
 * right offset.
 * @param radius1
 * left radius.
 * @param radius2
 * rt rad if diff from lt.
 * @param x_sect
 * cross section if not circular.
 * @param u_closure
 * u closure.
 * @param v_closure
 * v closure.
 */

	blend_spl_sur(	
			blend_support *left_sup,
			blend_support *right_sup,
			curve const &def_crv,
			SPAinterval const &v_range,
			double left_offset,
			double right_offset,
			var_radius *radius1,
			var_radius *radius2 = NULL,
			var_cross_section const *x_sect = NULL,
			closed_forms u_closure = OPEN,
			closed_forms v_closure = CLOSURE_UNSET
			);

	// A more generic constructor that does not depend on the number of 
	// supports. 

	blend_spl_sur(
			logical two_sup,                        // The number of supports, two or three.
			blend_support *sup_arr[],               // The support array.
			curve const &def_crv,                   // The defining curve of the blend.
			SPAinterval const &v_range,             // Blend surface parameter range.
			var_radius *left_rad,                   // The blend radius, left radius for chamfers.
			var_radius *right_rad = NULL,           // The right radius for chamfers.
			var_cross_section *x_sect = NULL,       // The cross-section, default is circular.
			closed_forms v_closure = CLOSURE_UNSET  // Closure of the blend.
			);

    // Destructor needs to delete the reference curve, blend_supports,
    // radius and section objects.
/**
 * C++ destructor, deleting a <tt>blend_spl_sur</tt>.
 */

    virtual ~blend_spl_sur();


	// Make or remake the approximating surface.  The force flag forces the
	// approximating surface to be made even if it is illegal.  This can be
	// used to restore old parts that were not checked properly before being
	// saved.  The spline argument 'spl' may be null but if it is supplied
	// the function may be a little faster.

	// The make_approx function (which is a virtual function of spl_sur) is an
	// interface to the blend-specific function make_approximating_surface.

/**
 * Makes or remakes an approximation of the surface, within the given tolerance.
 * <br><br>
 * @param fit
 * fit tolerance.
 * @param spl
 * pointer to output spline approx.
 * @param force
 * flag for forcing.
 */

    virtual void	make_approx(
							 double fit,
							 const spline& spl = *(spline*) NULL_REF,
							 logical force = FALSE ) const;


    // After a blend_spl_sur has been constructed, and all its data is
	// in place, the approximating surface must be made. This function
	// is an interface to extend_approx_sf_v which does the really hard
	// work. The fit tolerance may be specified or not. If not the
	// default (-1.0) is used. A fit of < 0 always means to choose its
	// own "sensible" value.

	// If range and start are unspecified it makes the defining curve's
	// entire range of surface, regardless of whether the surface is
	// legal everywhere.

	// If range is specified and start is not, it makes exactly the
	// given range of surface, regardless of legality or otherwise.

	// If the range is unspecified but the start *is*, it makes as much
	// surface either side of the start as is legal. If the start lies
	// in an illegal region of the surface, it will make nothing at all.
	// For periodic defining curves, it is possible for the final
	// surface range to cross the curve's join SPAparameter.

	// If the range and start are both given, makes up to the given
	// range, starting where indicated, and watching out for illegal
	// regions of surface. When periodic, will never allow the bottom of
	// the surface to go beyond the top minus the period.

	// A region SPAbox may be specified to indicate a particular region of
	// interest of the spine - often defining curves may be
	// much longer than we need. If a region is passed, construction of
	// the approximating surface stops when the spine has wandered
	// outside the region. If the spine starts outside the region,
	// construction only stops when it enters the region and then leaves.
	// The region may be omitted or left as a NULL reference to be
	// ignored completely.

	// Returns TRUE if any surface at all was made, else FALSE.
/**
 * After a <tt>blend_spl_sur</tt> has been constructed and all its data is in place, the approximating surface must be made.
 * <br><br>
 * <b>Role:</b> This function is an interface to <tt>extend_approx_sf</tt> which does the really hard work.
 * The fit tolerance may be specified or not. If not specified, the default of -1.0 is used.
 * A fit less than zero always means to have the routine choose a "sensible" value.
 * <br><br>
 * If range and start are unspecified, the defining curve's entire range becomes that of
 * the surface, regardless of whether the surface is legal everywhere. If range is specified
 * and start is not, the range becomes exactly the given range of the surface, regardless of
 * legality or otherwise.
 * <br><br>
 * If the range is unspecified but the start is specified, the range becomes as much
 * surface either side of the start as is legal. If the start lies in an illegal region of
 * the surface, the range becomes nothing at all. For periodic defining curves, it is
 * possible for the final surface range to cross the curve's join parameter.
 * <br><br>
 * If the range and start are both given, makes up to the given
 * range, starting where indicated, and watching out for illegal
 * regions of surface. When periodic, will never allow the bottom of
 * the surface to go beyond the top minus the period.
 * <br><br>
 * A region <tt>SPAbox</tt> may be specified to indicate a particular region of
 * interest of the spine - often defining curves may be
 * much longer than we need. If a region is passed, construction of
 * the approximating surface stops when the spine has wandered
 * outside the region. If the spine starts outside the region,
 * construction only stops when it enters the region and then leaves.
 * The region may be omitted or left as a <tt>NULL</tt> reference to be
 * ignored completely.
 * <br><br>
 * Returns <tt>TRUE</tt> if any surface at all was made, else <tt>FALSE</tt>.
 * <br><br>
 * @param requested_tol
 * requested tolerance.
 * @param range
 * interval pntr to interval.
 * @param start
 * surface start pntr to const.
 * @param region
 * bounding region.
 */

    logical make_approximating_surface(
							   double requested_tol = -1.0,
							   SPAinterval const &range = 
                                * (SPAinterval const *) NULL_REF,
							   double const &start = 
                                * (double const *) NULL_REF,
							   SPAbox const &region = 
                                * (SPAbox const *) NULL_REF,
                               list_of_contacts const &slice_list =
                                * (list_of_contacts const *) NULL_REF,
							   logical stop_if_illegal = TRUE,
							   logical & found_sf_illegal =
							    * (logical *)NULL_REF
							   );

	// This is the new way to manufacture the approximating surface,
	// capable of extending an existing one. It extends the bs3_surface
	// from spine SPAparameter start to end, or makes it initially if no
	// approx surface is yet there. It makes it to the requested
	// tolerance, or better. If the requested tolerance is < 0, it
	// chooses its own default "sensible" tolerance.

	// Note that start may be bigger than end and the surface is
	// constructed in decreasing SPAparameter order. One of start or end
	// should match an end of the existing approx sf (if there is one),
	// and the other SPAparameter should clearly continue on from there. If
	// the logical flag is set the approximating surface is terminated
	// if it is found to self intersect or scrunch up. It still makes
	// what it legally could, however.

	// A region SPAbox may be specified to indicate a particular region of
	// interest of the defining curve - often defining curves may be
	// much longer than we need. If a region is passed, construction of
	// the approximating surface stops when the spine has wandered
	// outside the region. If the spine starts outside the region,
	// construction only stops when it enters the region and then leaves.
	// The region may be omitted or left as a NULL reference to be
	// ignored completely.

	// The return value is TRUE if all the requested surface was
	// made, or FALSE if it was terminated early, or the any other
	// infelicity occurs. Note that leaving the region (if that was
	// passed) constitutes making everything that was requested.
/**
 * This creates the approximating surface and is capable of extending an existing surface along v.
 * <br><br>
 * <b>Role:</b> It extends the <tt>bs3_surface</tt> from spine parameter start to end, or makes it
 * initially if no approximating surface is yet there. The surface is created to the
 * requested tolerance, or better. If the requested tolerance is less than 0, a default
 * is chosen with "sensible" tolerance.
 * <br><br>
 * The start may be bigger than end, and the surface is constructed in decreasing
 * parameter order. Either start or end should match an end of the existing approximating
 * surface (if there is one). The other parameter should clearly continue on from there.
 * If the logical flag is set, the approximating surface is terminated if it is found to
 * self intersect or scrunch up. However, it still makes what it legally can.
 * <br><br>
 * The return value is <tt>TRUE</tt> if all the requested surface was made, or <tt>FALSE</tt> if it was
 * terminated early, or any other infelicity occurs.
 * <br><br>
 * @param start
 * starting spine parameter.
 * @param end
 * ending spine parameter.
 * @param requested_tol
 * requested tolerance.
 * @param stop_if_illegal
 * flag for approximating.
 * @param region
 * bounding box surface.
 * @param num_u
 * integer.
 */
	logical extend_approx_sf_v(
					 double start,
					 double end,
					 double requested_tol,
					 logical stop_if_illegal,
					 logical & found_sf_illegal,
					 SPAbox const &region =
                        * (SPAbox const *) NULL_REF,
					 int const &num_u = 
                        * (int const*) NULL_REF,
                     list_of_contacts const &slice_list =
                        * (list_of_contacts const *) NULL_REF					 
					 );

/**
 * This function is capable of extending an existing approximating surface along u.
 * <br><br>
 * <b>Role:</b> It extends the <tt>bs3_surface</tt> upto the given u range.
 * <br><br>
 * If the logical flag is set, the approximating surface is terminated if it is found to
 * self intersect or scrunch up. However, it still makes what it legally can.
 * <br><br>
 * The return value is <tt>TRUE</tt> if all the requested surface was made, or <tt>FALSE</tt>
 * if it was terminated early, or any other infelicity occurs.
 * <br><br>
 * @param u_r
 * The extended u range expected.
 * @param stop_if_illegal
 * flag for approximating.
 * @param region
 * bounding box surface.
 */

	logical extend_approx_sf_u(
					 SPAinterval const &u_r,
					 logical stop_if_illegal,
					 SPAbox const &region =
                        * (SPAbox const *) NULL_REF
					 );

	// Function to find stationary points. Only does anything if one of
	// both blend_supports are on curves. Searchs from the given start
	// SPAparameter for stationary points, in the given direction as far as
	// the defining curve allows and if it finds one adds that
	// information to the legal_range. The first logical flag indicates
	// to search up the range, the second whether the start point is to
	// be counted if it turns out to be a root. Optionally, a limit to
	// the search may be given, otherwise a default behaviour of
	// searching the rest of the curve will be used.

	// Returns TRUE if a stationary point is found. The legal_range may
	// be queried to find where it is. If nothing is found returns
	// FALSE.

	// NOTE: currently doesn't work correctly for variable radius
	// blends.
/**
 * Function to find stationary points.
 * <br><br>
 * <b>Role:</b> Only does anything if one of both <tt>blend_supports</tt> is on curve. Searches from
 * the given start parameter for stationary points in the given direction as far as the
 * defining curve allows. If it finds one, it adds that information to the legal range.
 * The first logical flag is for searching up the range. The second logical flag is for
 * whether the start point is to be counted if it turns out to be a root. Optionally, a
 * limit to the search may be given, otherwise a default behavior of searching the rest of
 * the curve will be used.
 * <br><br>
 * Returns <tt>TRUE</tt> if a stationary point is found. The <tt>legal_range</tt> may be queried to find
 * where it is. If nothing is found returns <tt>FALSE</tt>. Currently doesn't work correctly for
 * variable radius blends.
 * <br><br>
 * @param start
 * start parameter for pt.
 * @param search_fwd
 * forward search.
 * @param ignore_start_root
 * flag for if start point is root.
 * @param end
 * limit to search.
 */

	logical find_stationary_pt(
					   double start,
					   logical search_fwd,
					   logical ignore_start_root,
					   double &end = *(double *)NULL_REF
					   );

    // This is the old make_approximating_surface function. It is
	// retained temporarily because variable radius surfaces call (a
	// derived version of) it. The new version of the function is much
	// better: it fits the requested tolerance, does it quite carefully,
	// in fact, detects regions where the surface becomes illegally
	// scrunched up and all sorts. Furthermore, it is designed to
	// provide the capability that will be necessary for the surface
	// extension work. When time permits this old function should be
	// removed and the new one upgraded as necessary to work for all
	// blend surfaces.
/**
 * Creates a surface after a <tt>blend_spl_sur</tt> has been constructed and all its data is in place.
 * <br><br>
 * <b>Role:</b> This is the old way of creating a surface.
 * <br><br>
 * @param requested_tol
 * desired tolerance.
 */

    virtual logical old_make_approximating_surface( double requested_tol );

// Function to enquire whether the spl_sur is a variable radius type.
/**
 * Returns <tt>TRUE</tt> if the <tt>blend_spl_sur</tt> is a variable radius type; otherwise, it returns <tt>FALSE</tt>.
 */

	logical is_var_rad_type() const {  return rad != NULL; }

// Inquiry functions concerning size and shape.
/**
 * Returns <tt>TRUE</tt> if the given <tt>blend_spl_sur</tt> is circular; otherwise, it returns <tt>FALSE</tt>.
 */

	virtual logical is_circular() const;

// Inquiry functions concerning size and shape.
/**
 * Returns <tt>TRUE</tt> if the given <tt>blend_spl_sur</tt> is curvature continuous; otherwise, it returns <tt>FALSE</tt>.
 */
    logical is_curvature_continuous() const;
        
/**
 * Returns <tt>TRUE</tt> if the blend radius at the start or end point of the <tt>blend_spl_sur</tt> is zero.
 * <br><br>
 * @param at_start
 * at start point if true.
 * @param tol
 * tolerance.
 */

	virtual logical zero_end_radius(
		logical at_start, double tol = SPAresabs ) const;
/**
 * Returns <tt>TRUE</tt> if the blend radius slope at the start or end point of the <tt>blend_spl_sur</tt> is zero.
 * <br><br>
 * @param at_start
 * at start point if true.
 * @param tol
 * tolerance.
 */

	virtual logical zero_end_rad_slope(
		logical at_start, double tol = SPAresabs ) const;

    // Calculate the discontinuity information from the spine curve.
	// Note that the constructors DO NOT call this function. This must
	// be called explicitly after any bs3_surface has been made (which
	// the constructors don't make either).
/**
 * @nodoc
 */
    virtual	void 	calculate_disc_info();

	/**
	 * Saves an approximation of the surface, or calls the subtype object's save method.
	 */
	virtual void save() const;

// Member setting functions.

private:

	// a utility routine used by some of the following:

	closed_forms determine_v_closure() const;

public:
/**
 * @nodoc
 */
	void remove_linear_extension_u( bs3_surface &bs3, logical low_end );
/**
 * @nodoc
 */
	double calc_local_fit_err(
                        blend_spl_sur *blnd_srf,
						bs3_surface approx_srf,
						const SPAinterval& u_range,
						const SPAinterval& v_range,
						int num_u_pts,
						int num_v_pts
                        );

	// Set the initial_fitol and initial_num_u
/**
 * Sets the initial fit tolerance.
 * <br><br>
 * @param fitol
 * fit tolerance.
 */

	void set_initial_fitol( double fitol) { initial_fitol = fitol; }
/**
 * Sets the initial <i>u</i> value.
 * <br><br>
 * @param num_u
 * u value.
 */

	void set_initial_num_u( int num_u ) { initial_num_u_pts = num_u ; }


	// Set the bs2_curves into the left support data.
/**
 * Sets the <tt>bs2_curves</tt> into the left support data.
 * <br><br>
 * @param curve
 * bs2 curve.
 */
    void set_left_bs2_curve( bs2_curve  curve);

	// Set the bs2_curves into the right support data.
/**
 * Sets the <tt>bs2_curves</tt> into the right support data.
 * <br><br>
 * @param curve
 * bs2 curve.
 */
    void set_right_bs2_curve( bs2_curve curve );

	// Set the bs2_curves into the center support data.
/**
 * Sets the <tt>bs2_curves</tt> into the center support data.
 * <br><br>
 * @param curve
 * bs2 curve.
 */
    void set_center_bs2_curve( bs2_curve curve );

//	 Set the approximating surface and fit tolerance.
//	 These are protected members of the base class.
/**
 * Sets the approximating surface tolerance.
 * <br><br>
 * @param approx
 * approx. bs3 surface.
 */

    void set_sur( bs3_surface approx );
/**
 * Sets the approximating fit tolerance.
 * <br><br>
 * @param tol
 * tolerance.
 */

    void set_fitol( double tol );

// Set the u-SPAparameter range.
// Don't allow start > end.  If so, makes an empty SPAinterval.
/**
 * Sets the <i>u</i>-parameter range.
 * <br><br>
 * <b>Role:</b> Don't allow start > end. If so, makes an empty interval.
 * <br><br>
 * @param start
 * start.
 * @param end
 * end.
 */

    void set_u_range( double start, double end );

// Set the v-SPAparameter range.
// Don't allow start > end.  If so, makes an empty SPAinterval.
/**
 * Set the <i>v</i>-parameter range.
 * <br><br>
 * <b>Role:</b> Don't allow start > end. If so, makes an empty interval.
 * <br><br>
 * @param start
 * start.
 * @param end
 * end.
 */
     void set_v_range( double start, double end );

// Set closure properties.  These are protected members of spl_sur.
/**
 * Sets closure properties.
 * <br><br>
 * <b>Role:</b> This is a protected member of <tt>spl_sur</tt>.
 * <br><br>
 * @param cl
 * closure.
 */

	void set_u_closure( closed_forms cl ) { closed_in_u = cl; }
/**
 * Sets closure properties.
 * <br><br>
 * <b>Role:</b> This is a protected member of <tt>spl_sur</tt>.
 * <br><br>
 * @param cl
 * closure.
 */

	void set_v_closure( closed_forms cl ) { closed_in_v = cl; }

// Determine the singularity of the surface.  The results are stored
// in the u_singularity and v_singularity flags.
/**
 * Determines the singularity of the surface.
 * <br><br>
 * <b>Role:</b> The results are stored in the <tt>u_singularity</tt> and <tt>v_singularity</tt> flags.
 */
     void	determine_singularity();


// Update the legal_range of the blend surface, given the SPAparameter at
// which the surface must stop, and whether the bound is an upper bound
// or not. Does the correct thing for periodic def_curves.
/**
 * Updates the <tt>legal_range</tt> of the blend surface.
 * <br><br>
 * <b>Role:</b> Given the parameter at which the surface must stop, and whether the bound
 * is an upper bound or not. Does the correct thing for periodic <tt>def_curves</tt>.
 * <br><br>
 * @param v_param
 * range value.
 * @param is_upper_bound
 * true if upper bound.
 */

	void update_legal_range( double v_param, logical is_upper_bound );

// Check for bad singularities at the ends, and set legal range to avoid them.
// The argument indicates which end to look at: 0 for both (the default),
// negative for the low end, and positive for the high end.
/**
 * Checks for bad singularities at the ends and sets the legal range such that it avoids them.
 * <br><br>
 * <b>Role:</b> The argument indicates which end to look at. 0 is for both ends (the default).
 * A negative number represents the low end and a positive the high end.
 * <br><br>
 * It checks whether or not a <tt>safe_range</tt> has been applied to the defining curve. If so,
 * it checks for the case in which the angle between the vectors from the spine point to the
 * contact points is 180 degrees, i.e. the contact points and the spine point are collinear,
 * with the spine point in the middle. The related degeneracy, where the angle is zero and
 * the contact points coincide (tangent surfaces), is not a "bad" singularity and doesn't hurt
 * anything.
 * <br><br>
 * If the safe range limits a bad singularity, then the end of the safe range are before
 * the singularity actually happens, so it checks for "close" to a bad singularity at the end.
 * <br><br>
 * @param which_end
 * which end to look at.
 */

	void check_safe_range( int which_end = 0 );

// A query function to check whether a given v-SPAparameter value
// is within the legal range.
/**
 * A query function to check whether a given <i>v</i>-parameter value is within the legal range.
 * <br><br>
 * @param v_param
 * given parameter.
 */

	logical legal_v_param( double v_param ) const;


// A function to extend the blend surface in place, including all the supporting
// data. Not sure exactly how this should be specified - how about giving a
// v-SPAparameter range which is bigger than the current range.
/**
 * Extends the blend surface in place, including all the supporting data.
 * <br><br>
 * @param new_v_range
 * new v range.
 */

    virtual void	extend_surface( const SPAinterval& new_v_range );


    // Construct a duplicate in free store of this object but with
    // zero use count.
/**
 * @nodoc
 */
     virtual subtrans_object *copy() const = 0;


    // Test for equality. This does not guarantee that all
    // effectively equal surfaces are determined to be equal, but
    // does guarantee that different surfaces are correctly
    // identified as such.
/**
 * Tests two blends for equality.
 * <br><br>
 * <b>Role:</b> This does not guarantee that all effectively equal surfaces are determined
 * to be equal, but does guarantee that different surfaces are correctly identified as such.
 * <br><br>
 * @param obj
 * subtype object.
 */

    logical operator==( subtype_object const &obj ) const;


    // Transform this blend by the given SPAtransf.
/**
 * Transforms this blend by the given transform.
 * <br><br>
 * @param trans
 * transformation.
 */

    virtual void operator*=( SPAtransf const &trans	);


    // Parameter shift: adjust the spline surface to have a SPAparameter
    // range increased by the argument value (which may be negative).
    // This is only used to move portions of a periodic surface by
    // integral multiples of the period.
/**
 * Adjusts the spline surface to have a parameter range increased by the argument value (which may be negative).
 * <br><br>
 * <b>Role:</b> This is only used to move portions of a periodic surface by integral
 * multiples of the period.
 * <br><br>
 * @param shift
 * shift.
 */

    virtual void shift_u( double shift );
/**
 * Adjusts the spline surface to have a parameter range increased by the argument value (which may be negative).
 * <br><br>
 * <b>Role:</b> This is only used to move portions of a periodic surface by integral
 * multiples of the period.
 * <br><br>
 * @param shift
 * shift.
 */

    virtual void shift_v( double shift );


    // Divide a surface into two pieces at a given SPAparameter value.
    // If the split is at the end of the SPAparameter range, the spl_sur
    // is just returned as the appropriate half (in increasing
    // SPAparameter order), and the other is NULL. Otherwise a new spl_sur
    // is used for one part, and the old one is modified for the other.
/**
 * Divides a surface into two pieces at the <i>u</i>-parameter value.
 * <br><br>
 * <b>Role:</b> If the split is at the end of the parameter range, the <tt>spl_sur</tt> is just
 * returned as the appropriate half (in increasing parameter order), and the other is <tt>NULL</tt>.
 * Otherwise a new <tt>spl_sur</tt> is used for one part, and the old one is modified for the other.
 * <br><br>
 * @param dl
 * double.
 * @param surface
 * second spline surface.
 */

    virtual void split_u(
			 double dl,
			 spl_sur *surface [ 2 ]
			);
/**
 * Divides a surface into two pieces at the <i>v</i>-parameter value.
 * <br><br>
 * <b>Role:</b> If the split is at the end of the parameter range, the <tt>spl_sur</tt> is just
 * returned as the appropriate half (in increasing parameter order), and the other is <tt>NULL</tt>.
 * Otherwise a new <tt>spl_sur</tt> is used for one part, and the old one is modified for the other.
 * <br><br>
 * @param dl
 * double.
 * @param surface
 * second spline surface.
 */
    virtual void split_v(
			 double dl,
			 spl_sur * surface[ 2 ]
			);


    // Concatenate the contents of two surfaces into one. The surfaces
    // are guaranteed to be the same base or derived type, and to have
    // contiguous SPAparameter ranges ("this" is the beginning part of
    // the combined surface (i.e. lower SPAparameter values), the
    // argument gives the end part).

/**
 * Concatenates the contents of two surfaces into one.
 * <br><br>
 * <b>Role:</b> The surfaces are guaranteed to be the same base or derived type,
 * and to have contiguous parameter ranges ("this" is the beginning part of the
 * combined surface (i.e., lower parameter values), the argument gives the end part).
 * <br><br>
 * @param surface
 * spline surface.
 */

    virtual void append_u( spl_sur &surface );
/**
 * Concatenate the contents of two surfaces into one.
 * <br><br>
 * <b>Role:</b> The surfaces are guaranteed to be the same base or derived type,
 * and to have contiguous parameter ranges ("this" is the beginning part of the
 * combined surface (i.e., lower parameter values), the argument gives the end part).
 * <br><br>
 * @param surface
 * spline surface.
 */
    virtual void append_v( spl_sur &surface );


// Geometric evaluation.

// The inverting functions: point_normal(), point_prin_curve(),
// point_cross(), point_outdir(), point_perp() and param(),
// all eventually call point_perp() to do the actual inversion.
// point_perp() can be optimized for our particular surface,
// and then using the defaults for the other inverting functions
// will be optimized as much as possible.

	// Find the normal to the surface at a point on the surface
	// nearest to the given point.

//	virtual SPAunit_vector
//	point_normal(	SPAposition const &,
//					SPApar_pos const & = *(SPApar_pos *)NULL_REF
//			) const;


	// Find the principal axes of curvature of the surface at a
	// given point, and the curvatures in those directions.

//		virtual void point_prin_curv(
//					SPAposition const &,
//					SPAunit_vector &,		// first axis direction
//					double &,			// curvature in first direction
//					SPAunit_vector &,		// second axis direction
//					double &,			// curvature in second direction
//					SPApar_pos const & = *(SPApar_pos *)NULL_REF,
//                  evaluate_surface_quadrant = evaluate_surface_unknown
//			) const;


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


// Find the angle between at def_cvec, according to the rules
/**
 * Finds the angle between two vectors, in a plane determined by the tangent vector in the given CVEC.
 * <br><br>
 * @param P
 * position.
 * @param Tan
 * tangent direction.
 * @param R0
 * first vectors.
 * @param R1
 * second vectors.
 * @param rr_sina
 * sine of angle between vectors.
 * @param rr_cosa
 * cosine of angle between vectors.
 * @param v_value
 * v-value of the blend.
 */

	virtual double blend_total_angle( SPAposition &P,
                                  SPAunit_vector &Tan,
			                      SPAvector const &R0,
                                  SPAvector const &R1,
			                      double &rr_sina = *(double*) NULL_REF,
                                  double &rr_cosa = *(double*) NULL_REF,
                                  double v_value  = 0.0
                                ) const;

// Find the angle between two blend u radii, according to the rules
/**
 * Finds the angle between two vectors, in a plane determined by the tangent vector in the given <tt>CVEC</tt>.
 * <br><br>
 * @param Tan
 * tangent direction.
 * @param R0
 * first vectors.
 * @param R1
 * second vectors.
 * @param rr_sina
 * sine of angle between vectors.
 * @param rr_cosa
 * cosine of angle between vectors.
 * @param v_value
 * v-value of the blend.
 */

	virtual double blend_angle( SPAunit_vector &Tan,
			                SPAvector const &R0,
                            SPAvector const &R1,
			                double &rr_sina = *(double*) NULL_REF,
                            double &rr_cosa = *(double*) NULL_REF,
                            double v_value  = 0.0
                          ) const;


	// Find the point on the surface nearest to the given point,
	// iterating from the given SPAparameter values (if supplied).
	// Return the found point, the normal to the surface at that
	// point and the SPAparameter values at the found point.

/**
 * @nodoc
 */
    virtual void point_perp(
			    SPAposition const &point1,
			    SPAposition &point2,
			    SPAunit_vector &unit_vec,
			    surf_princurv &curv,
			    SPApar_pos const &par_pos1 = *(SPApar_pos *)NULL_REF,
			    SPApar_pos &par_pos2 = *(SPApar_pos *)NULL_REF,
				logical f_weak=FALSE,
			    SPApar_box const &par_box = *(SPApar_box *)NULL_REF
			    ) const;


    // Find the SPAparameter values of a point on the surface. We use
    // a slightly different parametrisation from standard, to speed
    // up evaluation: the SPAparameter value is that for the intersection
    // of the approximating surface with the true surface normal at
    // the given point.
    // This one must be defined, it's "pure virtual".
/**
 * @nodoc
 */
    virtual SPApar_pos param(
			  SPAposition const &pt,
			  SPApar_pos const &result = *(SPApar_pos *)NULL_REF
			  ) const;

    // Find the change in surface SPAparameter corresponding to a unit
    // offset in a given direction at a given uv, the direction
    // lying in the surface.

//		virtual SPApar_vec param_unitvec(
//					SPAunit_vector const &,
//					SPApar_pos const &
//				) const;

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
	// SPAparameter values.

//		virtual SPAunit_vector eval_normal(
//					SPApar_pos const &
//				) const;

	// Return a direction which points outward from the surface. This
	// should be the outward normal if the point is not singular,
	// otherwise a fairly arbitrary outward direction.
/**
 * @nodoc
 */
	virtual SPAunit_vector eval_outdir(
				SPApar_pos const &dir
			) const;


	// Find the principal axes of curvature of the surface at a
	// point with given SPAparameter values, and the curvatures in those
	// directions.

//		virtual void eval_prin_curv(
//					SPApar_pos const &,
//					SPAunit_vector &,		// first axis direction
//					double &,			// curvature in first direction
//					SPAunit_vector &,		// second axis direction
//					double &,			// curvature in second direction
//                  evaluate_surface_quadrant = evaluate_surface_unknown
//				) const;

	// Find the curvature of a cross-section curve of the surface at
	// the point on the surface with given SPAparameter values.
	// The cross-section curve is defined as the intersection of
	// the surface with a plane passing through the point on the
	// surface and normal to the given direction, which must lie in
	// the surface.

//		virtual double eval_cross(
//					SPApar_pos const &,
//					SPAunit_vector const &
//				) const;


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
 * @nodoc
 */
	virtual int evaluate( 
						SPApar_pos const &para, // Parameter
						SPAposition &pt,		// Point on surface at given parameter
						SPAvector** arr = NULL, // Array of pointers to arrays of vectors, of size nd.
                                              // Any of the pointers may be null, in which case the corresponding
                                              // derivatives will not be returned. Otherwise they must point to
                                              // arrays long enough for all the derivatives oft hat order - i.e.
                                              // 2 for the first derivatives, 3 for the second, etc.
						int num = 0, // Number of derivatives required (nd)
						evaluate_surface_quadrant quad = evaluate_surface_unknown // the evaluation location - above,
                                              // below for each SPAparameter direction, or don't care.
						) const = 0;


    // Return the number of derivatives which evaluate() can find
    // "accurately" (and fairly directly), rather than by finite
    // differencing, over the given portion of the surface. If there
    // is no limit to the number of accurate derivatives, returns
    // the value ALL_SURFACE_DERIVATIVES, which is large enough to be
    // more than anyone could reasonably want.
/**
 * @nodoc
 */
    virtual int accurate_derivs(
				 SPApar_box const &box = *(SPApar_box *)NULL_REF
				 // Defaults to the whole surface
				 ) const =0;


    // A form of evaluation specific to blend_spl_surs (certain
    // numerical algorithms used by blending need this function).
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
/**
 * A form of evaluation specific to <tt>blend_spl_surs</tt> (certain numerical algorithms used by blending need this function).
 * <br><br>
 * <b>Role:</b> Evaluates spine, defining curve, contact points and their derivatives at
 * the given <i>v</i>-parameter, according to the <tt>blend_section</tt> class declaration as above. We may
 * specify exactly how may spine and spring curve derivatives we require. As the two are
 * typically connected you may get more than you asked for, but you are guaranteed to get
 * at least what you ask for. Implementations of this should also ensure it does no more
 * than is necessary. Finally the logical flag indicates whether you require the cross
 * curve normal filled in; again this may (will) have implications on the amount of other
 * stuff you get back, but if passed as <tt>TRUE</tt> then this is guaranteed to be returned.
 * Note that calling this with for example -1, -1 and <tt>TRUE</tt> is valid.
 * <br><br>
 * @param v
 * v parameter.
 * @param spine_nder
 * number of required spine derivs.
 * @param spring_nder
 * no. of req'd spring derivs.
 * @param xcrv_norm
 * whether to fill in xcurve normal.
 * @param section
 * all output in here.
 * @param eval
 * the evaluation location.
 */

    virtual void compute_section(
						 double v,		// v SPAparameter
						 int spine_nder,	// number of required spine derivs
						 int spring_nder,	// no. of req'd spring derivs
						 logical xcrv_norm,	// whether to fill in xcurve normal
						 blend_section &section,	// all output in here
						 int eval= 0	 // the evaluation location - 1 => above,
						             // -1 => below, 0 => don't care.
						 ) const = 0;


    // Test whether a point lies on the surface.

//		virtual logical test_point_tol(
//					SPAposition const &,
//					double,
//					SPApar_pos const & = *(SPApar_pos *)NULL_REF,
//					SPApar_pos & = *(SPApar_pos *)NULL_REF
//				) const;
//
// Construct an iso-SPAparameter line on the surface. A u SPAparameter line
// runs in the direction of increasing u SPAparameter, at constant v.
// A v SPAparameter line runs in the direction of increasing v, at
// constant u. The parametrisation in the non-constant direction
// matches that of the surface, and has the range obtained by
// use of param_range_u() or param_range_v() appropriately.
//
// We provide our own definition which normally does the default thing.
// When there is no approx surface the default method crashes, so we
// do something of our own in such circumstances.
/**
 * Constructs an isoparameter line on the surface.
 * <br><br>
 * <b>Role:</b> A <i>u</i> parameter line runs in the direction of increasing <i>u</i> parameter,
 * at constant v. A <i>v</i> parameter line runs in the direction of increasing <i>v</i>,
 * at constant <i>u</i>. The parameterization in the non-constant direction matches that of
 * the surface, and has the range obtained by use of param_range_u() or param_range_v()
 * appropriately.
 * <br><br>
 * @param u
 * constant u parameter.
 * @param owner
 * surface where curve is.
 */

    curve *u_param_line(
			double u,
			spline const &owner
			) const;

	// For v_param_line, we can make a blend_int_cur rather than a
	// par_int_cur, but otherwise do the same as the base class. A
	// blend_int_cur is the same as a par_int_cur, but more wary about
	// zero length derivatives at the end of the curve.

/**
 * Constructs an isoparameter line on the surface.
 * <br><br>
 * <b>Role:</b> For <tt>v_param_line</tt>, we can make a <tt>blend_int_cur</tt> rather than a <tt>par_int_cur</tt>, but otherwise do
 * the same as the base class. A <tt>blend_int_cur</tt> is the same as a <tt>par_int_cur</tt>, but more
 * wary about zero length derivatives at the end of the curve.
 * <br><br>
 * @param v
 * constant v parameter.
 * @param owner
 * surface where curve is.
 */

	curve *v_param_line(
				double v,
				spline const &owner
				) const;

    // To enable three curve evaluator

	virtual void enable_new_evaluator( logical enable = TRUE );

	// Get evaluator version.

	blend_eval_version const &get_eval_version() const
	{ return _eval_version; }

	void set_left_spring( intcurve *left_spr );
	void set_right_spring( intcurve *right_spr );
	void set_spine( intcurve *sp );
	void set_save_spine( curve *c );

	intcurve *get_left_spring() const { return left_spring; }
	intcurve *get_right_spring() const { return right_spring; }
	intcurve *get_spine() const { return spine; }
	curve *get_save_spine() const { return save_spine; }

    double get_new_evaluator_curve_tolerance() const { return new_evaluator_curve_tolerance; }
	void set_new_evaluator_curve_tolerance( double new_tol ) { new_evaluator_curve_tolerance = new_tol; }

    void make_approximating_curves( surface *surf );

	void 	generate_new_evaluator_curve_approximation(
                curve *curve_in,
                bs3_curve &bs3_out,
		        const SPAinterval &range = * (SPAinterval*) NULL_REF,
                int num_seeds = 0,
		        int *cont = NULL,
                double *seeds = NULL,
                logical check_derivs = FALSE
                ) const;

	void 	generate_curvature_curve_approximation(
                SPAinterval        const &range, 
                discontinuity_info const &discs
                );

    virtual logical update_new_evaluator_curves();

    virtual SPAcrvGeom *make_crvGeom_object( curve * = NULL, 
											int = 0, 
											int * = NULL, 
											double * = NULL ) const
	{	return NULL;	}

	virtual SPAcrvGeom *get_left_spring_crvGeom(
		int nseeds = 0, int *cont = NULL, double *seeds = NULL);
	virtual SPAcrvGeom *get_right_spring_crvGeom( 
		int nseeds = 0, int *cont = NULL, double *seeds = NULL);
	virtual SPAcrvGeom *get_spine_crvGeom( 
		int nseeds = 0, int *cont = NULL, double *seeds = NULL) = 0;

    virtual double  blend_radius( double ) const = 0;

	/**
	 * Returns the string <tt>"blend_spl_sur"</tt>.
	 */

    virtual char const *type_name() const = 0;

	// Save and restore. Save is easy, as derived class switching goes through
	// the normal virtual function mechanism. Restore is more complicated,
	// because until it is invoked we do not have an object of the right class.
	// Instead we switch on a table defined by static instances of the
	// restore_ss_def class (see below), to invoke a simple friend function
	// which constructs an object of the right (derived) type. Then it can call
	// the appropriate member function to do the actual work.

/**
 * Saves the information associated with this <tt>blend_spl_sur</tt> to a SAT file.
 */

    virtual void save_data() const;
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
    friend DECL_KERN subtype_object *restore_blend_spl_sur();

/**
 * Restores the data from a save file.
 * <br><br>
 * <b>Role:</b> The restore operation switches on a table defined by static instances
 * of the <tt>restore_subtype_def</tt> class. This invokes a simple friend function which
 * constructs an object of the right derived type. Then it calls the appropriate base
 * class member function to do the actual work.
 */
    void restore_data();

	/**
	* @nodoc
	**/
	void process(geometry_definition_processor& p ) const;

#if defined D3_STANDALONE || defined D3_DEBUG

/**
 * @nodoc
 */
	friend DECL_KERN D3_istream& operator>>(
				D3_istream &,
				spl_sur *&
			);
/**
 * @nodoc
 */
	friend DECL_KERN D3_ostream& operator<<(
				D3_ostream &,
				spl_sur const &
			);

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
 * @nodoc
 */
    virtual void debug(
		       char const * chr,
		       logical logi,
		       FILE *fp
		       ) const;
/**
 * @nodoc
 */
    void debug_data(
		    char const *chr,
		    logical logi,
		    FILE *fp
		    ) const;

	// For now declare the blend_geom_spl class as a friend - it needs
	// to call our extend method, at least until such time as the spline
	// method is smart about data sharing as they are for transforms.

/**
 * @nodoc
 */
	friend class blend_geom_spl;
	};

// ********************************************************************
// Function:
//    restore_blend_spl_sur
//
// Action:
//    Internal to ACIS and not intended for direct usage.
//
// Prototype:
//    subtype_object* restore_blend_spl_sur ();
//
// Includes:
//    #include "kernel/acis.hxx"
//    #include "kernel/kerngeom/splsur/blnd_spl.hxx"
//    #include "kernel/kernutil/subtype/subtype.hxx"
//
// Description:
//    Although this internal function is intended strictly for ACIS
//    usage, a minimal amount of information about this function is provided for the
//    sole purpose of being able to understand and trace restoration from a SAT file.
//    This function should never be called directly, because it makes assumptions
//    about the availability of a SAT file, the location of the input pointer into the
//    SAT file, and the validity of SAT data it expects to read in. It also may start
//    a lengthy process of nested function or class method calls, which have many of
//    the same assumptions.
//
//    None                                  Nothing is saved or restored.
//
// Errors:
//    None
//
// Limitations:
//    Not applicable
//
// Library:
//    kernel
//
// Filename:
//    kern/kernel/kerngeom/splsur/blnd_spl.hxx
//
// Effect:
//    System routine
//
// Access:
//    document
//
// Mechanisms:
//    SAT Save and Restore
//
// ********************************************************************
/** @} */
#endif
