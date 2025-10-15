/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef VBL_BDYH_INTERNAL
#define VBL_BDYH_INTERNAL
#include "dcl_kern.h"
#include "logical.h"
#include "pos.hxx"
#include "cvc.hxx"
#include "math.hxx"
#include "svc.hxx"
#include "bxx.hxx"
#include "vbl_bdy.hxx"
/**
 * @file vbl_bdyi.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISGEOMETRY
 *
 * @{
 */
class 	curve;
class 	pcurve;
class 	ellipse;
class SizeAccumulator;
class minimize_helper;
#if defined D3_STANDALONE || defined D3_DEBUG
class 	D3_ostream;
class 	D3_istream;
#endif
class	BOUNDED_SURFACE;
/**
 * @nodoc
 *
 * The abstract base class defines all the functions the underlying geometry of
 * the boundary curves must provide. Below are 4 implementations needed to
 * support making vertex blend (and wire covering) surfaces from within Acis.
 *
 * This implements the boundary geometry as a pcurve on the boundary surface -
 * we can't compute Nll for these.
 */
class DECL_KERN BDY_GEOM_PCURVE : public BDY_GEOM
    {
private:

    // The pcurve proper.

    pcurve*		_pcurve;

    // The curve in 3-space - NB. this is *not* used except for regurgitation if
    // someone is interested in the 3-space form of this boundary and may be
    // NULL. We do own this curve, however. We also assume it's parametrised on
    // [ 0, 1 ].

    curve*		_curve;

    // For computations on points on curve.

    BOUNDED_SURFACE*    _bsf;
    SVEC		_svec;

    // du and dv along the curve wrt s, and second derivs.

    SPApar_vec		_duv;
    SPApar_vec		_dduv;

    // Whether above is calculated.

    int			_n_ders;

    // Cached bounding SPAbox.

    SPAbox			_bound;

    // Whether we've caculated the SPAbox.

    char		_got_bound;

    // For caching whether we can compute exact Nll.

    int			_accurate;

    virtual void	_set_l();

    virtual void	_prepare_l( int, logical = 0 );

    virtual SPAvector	_Xderiv( const SPAvector& );

    virtual SPAunit_vector	_compute_magic() const;

    BDY_GEOM_PCURVE( const BDY_GEOM_PCURVE&, pointer_map * pm);
		// called ONLY by deep_copy.

public:
    BDY_GEOM_PCURVE( logical = FALSE, double = 1, logical = 1,
		    logical = 1, const SPAunit_vector& = *(const class SPAunit_vector*)NULL_REF );
    ~BDY_GEOM_PCURVE();
    BDY_GEOM_PCURVE( const BDY_GEOM_PCURVE& );

    // Return the pcurve.

    const pcurve *pcur() { return _pcurve; }

    // Return the bounded surface.

    const BOUNDED_SURFACE *bsf() { return _bsf; }

    // Unique integer representing the type.

    virtual int		type() const;

    // For these we don't in general get accurate second derivs.

    virtual int		continuity();

    // splat _bound

    virtual void	reset();

    virtual BDY_GEOM*	copy() const;
	virtual BDY_GEOM*	deep_copy(pointer_map * pm = NULL) const;

	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	virtual void minimize( minimize_helper*);


    // There may or may not be a 3-space curve - it is not required.

    virtual void	set_geometry( const pcurve&,
				     const curve& = *(curve*)NULL_REF );

    virtual const SPAposition&	P();

    virtual SPAvector	Pl();

    virtual SPAvector	Pll();

    virtual SPAunit_vector	N();

    virtual SPAvector	Nl();

    virtual SPAvector	Nll();

    // We may produce accurate second derivs, we may not.

    virtual logical	accurate();

    virtual SPAbox		bound();

    virtual void	operator*=( const SPAtransf& );

    virtual curve*	cur( double&, double& );

#if defined D3_STANDALONE || defined D3_DEBUG
    virtual void	print( D3_ostream& os ) const;
    virtual void	input( D3_istream& is ) ;
#endif
	virtual logical need_save_as_approx(int save_to_version, logical check_progenitors) const;
    virtual void	save() const;
    virtual void	restore();
    };
/**
 * @nodoc
 * This implements the boundary geometry as a curve on a surface which must be a
 * plane: we only need keep the unit normal (and a non-const pointer to the
 * curve) in addition to the bounded curve. It is also used to represent a curve
 * for which we have no normal data to interpolate. In these cases _N is set to
 * 0 and the curve will be marked as wholly non-smooth by the BDY_GEOM. So no
 * cross boundary derivatives will ever be calculated.
 */
class DECL_KERN BDY_GEOM_PLANE : public BDY_GEOM
    {
private:

    // Bounded representation of the curve.

    BOUNDED_CURVE	_bcu;

    // The curve - unfortunately we need a non-const pointer to it.

    curve*		_curve;

    // For doing computations on points on the curve.

    CVEC		_cvec;

    // The normal to the plane the curve is in, or zero if this is a curve
    // 'without a normal'.

    SPAunit_vector		_norm;

    virtual void	_set_l();

    virtual void	_prepare_l( int, logical = 0 );

    // This special version is probably better for non-intcurves.

    virtual logical	_param( const SPAposition&, double );

    virtual SPAvector	_Xderiv( const SPAvector& );

    virtual logical	magic_const() const;

	BDY_GEOM_PLANE( const BDY_GEOM_PLANE& old, pointer_map * pm );
		// called ONLY by deep_copy.
public:

    BDY_GEOM_PLANE( logical = FALSE, double = 1, logical = 1,
		   logical = 1, const SPAunit_vector& = *(const class SPAunit_vector*)NULL_REF );
    ~BDY_GEOM_PLANE();
    BDY_GEOM_PLANE( const BDY_GEOM_PLANE& );

    virtual int		type() const;

    virtual BDY_GEOM*	copy() const;
	virtual BDY_GEOM*	deep_copy(pointer_map * pm = NULL) const;

	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	virtual void minimize( minimize_helper*);

    virtual void	set_geometry( const curve&, const SPAunit_vector&, double,
				     double );

    virtual const SPAposition&	P();

    virtual SPAvector	Pl();

    virtual SPAvector	Pll();

    virtual SPAunit_vector	N();

    virtual SPAvector	Nl();

    virtual logical	accurate();

    virtual SPAbox		bound();

    virtual void	operator*=( const SPAtransf& );

    virtual curve*	cur( double&, double& );

#if defined D3_STANDALONE || defined D3_DEBUG
    virtual void	print( D3_ostream& os ) const;
    virtual void	input( D3_istream& is ) ;
#endif
	virtual logical need_save_as_approx(int save_to_version, logical check_progenitors) const;
    virtual void	save() const;
    virtual void	restore();
    };

// This class implements the boundary geometry as an ellipse (actually a circle)
// which is a section of a cylindrical, toroidal or pipe surface, corresponding
// to the types CYLINDER, TORUS or PIPE. The only difference between these cases
// is how the twist vectors are calculated. This gives rise to the fourth case,
// GIVEN_TWIST, where we no longer know what surface the circle really lies on
// but we know what the relevant twist values are. As soon as the twists have
// been calculated any of the first three types could mutate themselves into a
// GIVEN_TWIST type. Currently only PIPE types do as we can then chuck away the
// enormous pipe surface. Consequently PIPE types are very short-lived, as soon
// as the second twist SPAvector has been worked out it mutates.

/**
* Text specifies surface type.
*<br>
* @param CYLINDER
* cylinder.
* @param TORUS
* torus.
* @param PIPE
* pipe.
* @param GIVEN_TWIST
* given twist.
**/

enum underlying_sf_type { CYLINDER, TORUS, PIPE, GIVEN_TWIST };
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN BDY_GEOM_CIRCLE : public BDY_GEOM
    {
private:

    // Bounded representation of the curve.

    BOUNDED_CURVE	_bcu;

    // We also need a non-const pointer to the curve...

    ellipse*		_curve;

    // For doing computations on points on curve.

    CVEC		_cvec;

    // Radius of circle, radius < 0 => inward pointing normal. (Note: Negative
    // radius circles are no longer produced - it used to denote a reversed
    // normal on the underlying surface.)

    double		_radius;

    // Whether we're on the end of a cylinder, torus, pipe or are just given the
    // twist vectors.

    underlying_sf_type	_underlying_sf_type;

    // Centre of the torus, if there is one.

    SPAposition		_centre;

    // Pcurve and pipe surface, if present.

    pcurve*		_pcurve;
    BOUNDED_SURFACE*    _bsf;

    // The fixed twist vectors, if given.

    SPAvector		_twist0;
    SPAvector		_twist1;

    virtual void	_set_l();

    virtual void	_prepare_l( int, logical = 0 );

    // Param on circles is easy.

    virtual logical	_param( const SPAposition&, double );

    virtual SPAvector	_Xderiv( const SPAvector& );

    virtual logical	magic_const() const;

    BDY_GEOM_CIRCLE( const BDY_GEOM_CIRCLE&, pointer_map * pm);
		// called ONLY by deep_copy.

public:

    BDY_GEOM_CIRCLE( logical = FALSE, double = 1, logical = 1,
		    logical = 1, const SPAunit_vector& = *(const class SPAunit_vector*)NULL_REF );
    ~BDY_GEOM_CIRCLE();
    BDY_GEOM_CIRCLE( const BDY_GEOM_CIRCLE& );

    // Return the pcurve, possibly NULL.

    const pcurve *pcur() { return _pcurve; }

    // Return the pipe bounded surface, if present.

    const BOUNDED_SURFACE *bsf() { return _bsf; }

    // Return the centre of the torus, if there is one.

    const SPAposition&centre() { return _centre; }

    // Return the circle radius.

    double radius() { return _radius; }

    // Return the underlying type of BDY_GEOM_CIRCLE.

    underlying_sf_type	underlying_type() { return _underlying_sf_type; }

    virtual int		type() const;

    virtual BDY_GEOM*	copy() const;
	virtual BDY_GEOM*	deep_copy(pointer_map * pm = NULL) const;

	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	virtual void minimize( minimize_helper*);

    // Different functions for cone, torus, pipe or given twists,
    // respectively.

    virtual void	set_geometry( const ellipse&, double, double );
    virtual void	set_geometry( const ellipse&, double, double,
				     const SPAposition& );
    virtual void	set_geometry( const ellipse&, double, double,
				     const pcurve& );
    virtual void	set_geometry( const ellipse&, double, double,
				     const SPAvector&, const SPAvector& );

    virtual const SPAposition&	P();

    virtual SPAvector	Pl();

    virtual SPAvector	Pll();

    virtual SPAunit_vector	N();

    virtual SPAvector	Nl();

    virtual SPAvector	Nll();

    virtual logical	accurate();

    virtual SPAbox		bound();

    virtual void	operator*=( const SPAtransf& );

    virtual curve*	cur( double&, double& );

#if defined D3_STANDALONE || defined D3_DEBUG
    virtual void	print( D3_ostream& os ) const;
    virtual void	input( D3_istream& is ) ;
#endif
	virtual logical need_save_as_approx(int save_to_version, logical check_progenitors) const;
    virtual void	save() const;
    virtual void	restore();
    };

// This class implements a boundary section as a degenerate point. For example,
// the 'zero radius circle' which we image across an unblended edge. It will
// still represent an entire polygon edge in SPAparameter space. Although the
// SPAposition will always return a single point and Ps and Pss zero, the normal N
// and derivatives Ns and Nss will work as though a circle is really there,
// unless either the degenerately blended edge was smooth (returning a constant
// normal N) or the boundary is marked as wholly non-smooth so the normal data
// will be ignored.
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN BDY_GEOM_DEG : public BDY_GEOM
    {
private:

    // The SPAposition.

    SPAposition		_pos;

    // The major and minor radii for generating the 'normal circle'.

    SPAunit_vector		_Nmaj, _Nmin;

    // We cache the normal at s=1 for i/o. (_Nmaj represents the normal at s=0.)

    SPAunit_vector		_N_1;

    // The range of the 'normal circle' runs from 0 to _range.

    double		_range;

    // For caching the normal and derivatives.

    int			_n_ders;
    SPAunit_vector		_norm;
    SPAvector		_dnorm, _ddnorm;

    virtual void	_set_l();

    virtual void	_prepare_l( int, logical = 0 );

    // Should rarely be called.

    virtual logical	_param( const SPAposition&, double );

    virtual SPAvector	_Xderiv( const SPAvector& );

    virtual logical	magic_const() const;

//    BDY_GEOM_DEG( const BDY_GEOM_DEG&, pointer_map * pm);
//		// called ONLY by deep_copy.

public:

    BDY_GEOM_DEG( logical = FALSE, double = 1, logical = 1, logical = 1,
		 const SPAunit_vector& = *(const class SPAunit_vector*)NULL_REF );
    ~BDY_GEOM_DEG();
    BDY_GEOM_DEG( const BDY_GEOM_DEG& );

    virtual int		type() const;

    virtual logical	degenerate() const;

    virtual BDY_GEOM*	copy() const;
	virtual BDY_GEOM*	deep_copy(pointer_map * pm = NULL) const;

	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	virtual void minimize( minimize_helper*);

    // Set the geometry knowing the SPAposition, the normal at s=0 and the normal
    // at s=1.

    virtual void	set_geometry( const SPAposition&,
				     const SPAunit_vector&, const SPAunit_vector& );


    virtual const SPAposition&	P();

    virtual SPAvector	Pl();

    virtual SPAvector	Pll();

    virtual SPAunit_vector	N();

    virtual SPAvector	Nl();

    virtual SPAvector	Nll();

    virtual logical	accurate();

    virtual SPAbox		bound();

    virtual void	operator*=( const SPAtransf& );

    // cur should never be called for a degenerate boundary.

    virtual curve*	cur( double&, double& );

#if defined D3_STANDALONE || defined D3_DEBUG
    virtual void	print( D3_ostream& os ) const;
    virtual void	input( D3_istream& is ) ;
#endif
    virtual void	save() const;
    virtual void	restore();
    };


/** @} */
#endif
