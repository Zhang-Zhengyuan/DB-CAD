/* ORIGINAL: acis2.1/kerngeom/d3_vbl/vbl_bdy.hxx */
/* $Id: vbl_bdy.hxx,v 1.17 2002/08/09 17:15:23 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef VBL_BDYH
#define VBL_BDYH

#include "dcl_kern.h"
#include "logical.h"
#include "pos.hxx"
#include "bxx.hxx"
#include "math.hxx"
#include "uni.hxx"
#include "param.hxx"
#include "dispatch.hxx"

class 	curve;
class 	pcurve;
class 	ellipse;
class pointer_map;

class SizeAccumulator;
class minimize_helper;

#if defined D3_STANDALONE || defined D3_DEBUG
class 	D3_ostream;
class 	D3_istream;
#endif

// This class is for caching information about a particular point l, 0<=l<=1, on
// the boundary. We store l and its derivs wrt u,v. Also the SPAposition and normal
// at this point on the boundary, and their derivs wrt l.

class DECL_KERN BDY_CACHE : public ACIS_OBJECT
    {
    friend class BDY_GEOM;
    BDY_CACHE();
    ~BDY_CACHE();
    double	l, lu, lv, luu, luv, lvv;
    SPAposition	P;
    SPAvector	Pl, Pll;
    SPAunit_vector	N;
    SPAvector	Nl, Nll;
    };

// As a rule, any boundary section may be a cross curve or not. Being a cross
// curve means the magic SPAvector will be used for shape control. It will attempt
// to compute the magic SPAvector itself, but occasionally this is not possible and
// must be passed (eg. if the normal is constant etc.). Furthermore any boundary
// section may or may not be smooth (ie. interpolate the cross boundary
// derivative info). It it possible to be smooth in one direction but not the
// other (see below).

// All a derived class needs to define is an evaluator for SPAposition, normal and
// derivs wrt l on the boundary, and a few of geometric functions (corner
// cross-derivative, bounding SPAbox, relaxation etc.). All the caching of s and t
// points is handled by the base class.

//
// The base class for different representations of the boundary. This is all the
// VBL_SURF has to know about and provides a variety of different functions to
// do with computing the cross boundary derivative, setting up cached data,
// default relaxation methods etc.
//

class DECL_KERN BDY_GEOM : public ACIS_OBJECT
    {
private:
    // For computing and caching the cross-boundary derivative. Note nderivs
    // also used in deciding how much stuff is in _s_cache and _t_cache. We
    // always evaluate s and t points to the same level. This is easier, and the
    // symmetricness of evaluation means we always need the same of each.

    int		_nderivs;

    // Where information about the s and t points is kept. The s point is
    // regarded as going along the boundary with s=l, the t point goes backwards
    // with t=1-l. Everything is set up so that if a t value is not explicitly
    // set, then _t_cache points to the same cache as _s_cache and t is regarded
    // as being 1-s. Thus no extra evaluation is down over and above that
    // necessary for s. _t_cache is only set independently if a t value is
    // explicitly set.

    BDY_CACHE*	_s_cache;
    BDY_CACHE*	_t_cache;

    // Starting and ending values for the cross-boundary derivatives.

    // STIPORT TAC macintosh _D0 renamed to _Dzero due to conflicts with MWCWG5 internals
    SPAvector	_Dzero, _D1;

    // 'fullness' of the cross-boundary derivative from this boundary section.
    // Only really applicable if a cross-curve.

    double	_fullness;

    // Is this a cross curve? This means that an attempt will be made to use the
    // magic SPAvector for shape control.

protected:
    logical	_cross_curve;
private:

    // Whether smooth at the s=0 and s=1 ends. For a regular vertex blend these
    // are always 1. If _smooth0 is 1 then the corner interpolant which uses
    // this boundary for its s SPAparameter will pick up its cross boundary
    // derivative in this direction. If _smooth1 is 1 then the one which uses
    // this for its t SPAparameter will pick up the cross boundary derivative in
    // this direction.

    logical	_smooth0, _smooth1;

    // Starting and ending positions of this boundary section, and the mid
    // point is also cached for later convenience.

    SPAposition	_C0, _C1, _C_mid;

    // The 3-space SPAposition which is half way in SPAparameter space from 0,0 to the
    // vertex at s=1. Cached for later convenience.

    SPAposition	_half_C1;

    // Whether the s=0 and s=1 corners are concave.

    logical	_concave0, _concave1;

    // The magic SPAvector, if there is one. If the boundary is known to have a
    // constant magic SPAvector then this is it (up to sign), otherwise it's used
    // to compute the fullness magnitudes which will get set into the boundary.

    SPAunit_vector	_magic;

    // Cached normals at either end.

    SPAunit_vector	_N0, _N1;

    // Magitudes to use for s and t direction shape control vectors.

    double	_mag_s, _mag_t;

    // The actual s and t shape control vectors, and derivs.

    SPAvector	_M_s, _Ms_s, _Mss_s, _M_t, _Mt_t, _Mtt_t;

    // Initialize the data members appropriately. Only called by base class
    // constructors.

    void	_initialize( logical cross_curve, double fullness,
			    logical smooth0, logical smooth1,
			    const SPAunit_vector& magic );

    // Starting and ending twist vectors.

    SPAvector	_DD0, _DD1;

    // Vectors derived from the above ones to influence the shape of the cross
    // boundary derivatives. These are just quantities cached for efficiency.

    SPAvector	_DD0D, _DD1D;

    // Cached sine and cosine values.

    double	_sine, _cosine, _sine_1, _cosine_1;

    // Cached location of the vertex.

    SPApar_pos	_vertex;
    
    // Compute shape control SPAvector data for both s and t points.

    void		_get_boundary_data( int );

    // Transfer the given level of computed values into the correct cache. The
    // logical indicates whether the normal is needed or not.

    void		_transfer( int, BDY_CACHE*, logical );

    // The SPAparameter at which evaluation happens.

    double	_l;

    // Things a particular implementation may need to do to precompute data.
    // The first int specifies the number of required derivatives of the curve,
    // either 0, 1 or 2. The second logical specifies whether the normal is
    // needed, either to the same level, or not at all.

    virtual void	_prepare_l( int, logical = 0 ) = 0;

    // Things a particular implementation may need to do when s is set.

    virtual void	_set_l() = 0;

    // Relaxation method to return TRUE if it succeeded - a default is provided
    // which should be pretty much always ok, but maybe you can do better for
    // some simple geometries.

    virtual logical	_param( const SPAposition&, double );

    // The corner twist SPAvector. Only needs to work at l==0 and l==1.

    virtual SPAvector	_Xderiv( const SPAvector& ) = 0;

    // Normal and derivs at points s and t. These are only needed internally to
    // compute the cross boundary deriv. No one else has any business asking for
    // them. Indeed, in some circumstances they may not even get set.

    const SPAunit_vector&	_N_s() { return _s_cache->N; };

    const SPAvector&	_Ns_s() { return _s_cache->Nl; };

    const SPAvector&	_Nss_s() { return _s_cache->Nll; };

    const SPAunit_vector&	_N_t() { return _t_cache->N; };

    SPAvector		_Nt_t() { return -_t_cache->Nl; };

    const SPAvector&	_Ntt_t() { return _t_cache->Nll; };

protected:

    // Default method for computing the 'direction in which to push the surface'
    // ie. the magic SPAvector. When the magic SPAvector is non-const this gets used
    // in determining the magnitude to be applied to the shape control. By
    // default it comes from N0*N1 though this need not always be so.

    virtual SPAunit_vector	_compute_magic() const;

public:

    BDY_GEOM( logical = FALSE, double = 0, logical = 1, logical = 1,
	     const SPAunit_vector& = *(const class SPAunit_vector*)NULL_REF );
    BDY_GEOM( const BDY_GEOM& );
    virtual ~BDY_GEOM();

    // A unique integer representing the type. This is used for i/o.

    virtual int		type() const = 0;

    // How many levels of derivatives this boundary will allow the final blend
    // surface to have.

    virtual int		continuity();

    // Vertex SPAposition V_i-1 at s = 0, and V_i at s = 1.
    
    const SPAposition&	C0() const { return _C0; };
    const SPAposition&	C1() const { return _C1; };

    // And the mid point of the curve.

    const SPAposition&	C_mid() const { return _C_mid; };

    // The SPAposition half way from 0,0 to the vertex at s=1. Note that this only
    // gets set and used by param_guess at the moment.

    SPAposition&		half_C1() { return _half_C1; };

    // Deriv info at s = 0, and s = 1.
    
    // STIPORT TAC macintosh _D0 renamed to _Dzero
    const SPAvector&	D0() const { return _Dzero; };
    const SPAvector&	D1() const { return _D1; };

    // Return the twist vectors.
    
    const SPAvector&	DD0() const { return _DD0; };
    const SPAvector&	DD1() const { return _DD1; };

    // Return whether the corners are concave.

    logical	concave0() const { return _concave0; };
    logical	concave1() const { return _concave1; };

    // Return the magic SPAvector.

    const SPAunit_vector& magic() const;

    // Whether the magic SPAvector is constant. Default assumes no.

    virtual logical	magic_const() const;

    // Cached bits and pieces for computation of the blending functions.

    SPApar_dir	l1, l2;
    SPApar_pos	uvp;

    // Whether the twists are incompatible at the s=0 end, and whether the
    // twists are zero at the s=0 end and the s=1 end. These things are all
    // worth knowing as we can do some optimisations based on them.

    logical	bad_twist, zero_twist0, zero_twist1;

    // Sine and cosine of 2 * i * pi / n.
    
    double	sine() const { return _sine; };
    double	cosine() const { return _cosine; };

    // Sine and cosine of ( 2 * i - 1 ) * pi / n.
    
    double	sine_1() const { return _sine_1; };
    double	cosine_1() const { return _cosine_1; };

    // Local SPAparameter _s_i along this edge, and derivs wrt u,v.
    
    double&	s() { return _s_cache->l; };
    double&	su() { return _s_cache->lu; };
    double&	sv() { return _s_cache->lv; };
    double&	suu() { return _s_cache->luu; };
    double&	suv() { return _s_cache->luv; };
    double&	svv() { return _s_cache->lvv; };

    // Local parameter_t_i+1 back along this edge, and derivs wrt u,v. Note that
    // even the t cache stores its values going forward as well.

    double	t() { return 1 - _t_cache->l; };
    double	tu() { return -_t_cache->lu; };
    double	tv() { return -_t_cache->lv; };
    double	tuu() { return -_t_cache->luu; };
    double	tuv() { return -_t_cache->luv; };
    double	tvv() { return -_t_cache->lvv; };

    // Return references to the t cache so they can be set. Note this assumes
    // it's been set independently from _s_cache, these can only be called after
    // set_t.

    double&	tu_r() { return _t_cache->lu; };
    double&	tv_r() { return _t_cache->lv; };
    double&	tuu_r() { return _t_cache->luu; };
    double&	tuv_r() { return _t_cache->luv; };
    double&	tvv_r() { return _t_cache->lvv; };

    double	b, bu, bv, buu, buv, bvv;

    // Set the value of s and t. t does not need to be set in which case it is
    // taken as 1-s. s does need setting.

    void		set_s( double );
    void		set_t( double );

    // Similarly, precompute points derivs, cross-boudnary derivs etc. at s and
    // t.

    void		prepare( int );

    // Vector contributing correct info for this edge, and derivs wrt s,t.
    
    SPAvector	Q, Qs, Qt, Qss, Qst, Qtt;

    // Returning cross-boundary derivative and optionally its derivs. There are
    // two cross-boundary derivs, depending on whether this is the left or right
    // edge as we form the corner interpolant. Because of the generalised shape
    // control vectors each cross boundary deriv is also a function of the magic
    // SPAvector (constant or not) of the other edge of corner interpolant. Hence
    // the other boundary must be passed.

    void		Ds_field( BDY_GEOM&, SPAvector&, SPAvector* = NULL,
				 SPAvector* = NULL );

    void		Dt_field( BDY_GEOM&, SPAvector&, SPAvector* = NULL,
				 SPAvector* = NULL );

    void		Ds( BDY_GEOM&, SPAvector&, SPAvector& = *(SPAvector*)NULL_REF,
			   SPAvector& = *(SPAvector*)NULL_REF );

    void		Dt( BDY_GEOM&, SPAvector&, SPAvector& = *(SPAvector*)NULL_REF,
			   SPAvector& = *(SPAvector*)NULL_REF );

    // Return the 'fullness'.

    double	fullness() const { return _fullness; };

    // Actually uv pos of vertex V_i-1.
    
    const SPApar_pos&	vertex() const { return _vertex; };

    // Are we supposed to be G1 (ie. smooth) across this boundary?

    logical		smooth0() const { return _smooth0; };
    logical		smooth1() const { return _smooth1; };
    logical		smooth() const { return _smooth0 && _smooth1; };

    // Is it a cross curve or a spring curve?

    logical		cross_curve() const { return _cross_curve; };

    // Is it a degenerate boundary? (Default = NO).

    virtual logical	degenerate() const;

    // Make the given pointer point to a copy of this BDY_GEOM, this is kind-of
    // an assignment operator for BDY_GEOM pointers.

    void		replace( BDY_GEOM*& ) const;
	void		deep_copy_replace( BDY_GEOM*& old, pointer_map * pm = NULL ) const;

    // Set up cached starting and ending conditions.

    void		set_bounds( int, double, double, const SPAvector&,
				   const SPAvector& );

    // Set the cross-boundary deriv values affected by the fullness factor.

    void		set_fullness( double, double );

    // Setting the (s-type) SPAparameter for evaluation, and querying results.

    double		l() { return _l; };

    void		set_l( double );

    virtual const SPAposition&	P() = 0;

    virtual SPAvector	Pl() = 0;

    virtual SPAvector	Pll() = 0;

    virtual SPAunit_vector	N() = 0;

    virtual SPAvector	Nl() = 0;

    // Default returns zero SPAvector.
    
    virtual SPAvector	Nll();

    // Reset cached data - default does nothing.
    
    virtual void	reset();

    // Copy on the heap.
    
    virtual BDY_GEOM*	copy() const = 0;
	virtual BDY_GEOM*	deep_copy(pointer_map * pm = NULL) const = 0;

	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	virtual void minimize( minimize_helper*);
    
    // Returning the data kept about the s and t points.

    const SPAposition&	P_s() { return _s_cache->P; };

    const SPAvector&	Ps_s() { return _s_cache->Pl; };

    const SPAvector&	Pss_s() { return _s_cache->Pll; };

    const SPAposition&	P_t() { return _t_cache->P; };

    SPAvector		Pt_t() { return -_t_cache->Pl; };

    const SPAvector&	Ptt_t() { return _t_cache->Pll; };

    // Return if 2nd deriv of N accurate (default = NO).
    
    virtual logical	accurate();

    virtual SPAbox		bound() = 0;

    // Transform the base class BDY_GEOM bits. Derived classes will need to call
    // this, but will need to do more as well.

    virtual void	operator*=( const SPAtransf& );

    // Return SPAparameter along curve of point closest to given point. Returns < 0
    // if closest point is off the start, > 1 if off the end. Also checks the
    // two end points aren't closer and returns them if they are.
    
    double		param( const SPAposition&, double );

    // Return a 3-space curve for the edge on the heap and also start and end
    // parameters.

    virtual curve*	cur( double&, double& ) = 0;

    // D3 i/o.

#if defined D3_STANDALONE || defined D3_DEBUG
    virtual void	print( D3_ostream& os ) const = 0;
    virtual void	input( D3_istream& is ) = 0;
#endif

	virtual logical need_save_as_approx(int save_to_version, logical check_progenitors) const;
    virtual void	save() const = 0;
    virtual void	restore() = 0;
    };

#undef THIS
#define THIS() BDY_GEOM
DISPATCH_DECL(KERN)
#undef THIS


// Now functions to make the right kinds of boundary given bits of geometry.

// From some pcurve of no particular significance, with or without some 3space
// representation.

extern DECL_KERN BDY_GEOM* make_BDY_GEOM( logical cross_curve, const pcurve& pcur,
			       const curve& cur, double fullness,
			       logical smooth0, logical smooth1,
			       const SPAunit_vector& magic );

// From a curve lying in a plane.

extern DECL_KERN BDY_GEOM* make_BDY_GEOM( logical cross_curve, const curve& cur,
			       const SPAunit_vector& N, double start, double end,
			       double fullness, logical smooth0,
			       logical smooth1, const SPAunit_vector& magic );

// From a circle lying on a cylinder/cone, giving the sine and cosine of the
// half angle.

extern DECL_KERN BDY_GEOM* make_BDY_GEOM( logical cross_curve, const ellipse& cur,
			       double start, double end, double fullness,
			       logical smooth0, logical smooth1,
			       const SPAunit_vector& magic );

// From a circle lying on a torus.

extern DECL_KERN BDY_GEOM* make_BDY_GEOM( logical cross_curve, const ellipse& cur,
			       double start, double end, const SPAposition& centre,
			       double fullness, logical smooth0,
			       logical smooth1, const SPAunit_vector& magic );

// From a circle lying on some form of pipe surface, giving the circle as a
// pcurve on the pipe.

extern DECL_KERN BDY_GEOM* make_BDY_GEOM( logical cross_curve, const ellipse& cur,
			       double start, double end, const pcurve& pcur,
			       double fullness, logical smooth0,
			       logical smooth1, const SPAunit_vector& magic );

// From some circle lying on some surface which, at the circle, is tangent to a
// cylinder or cone, giving the sine and cosine of the half angle.

extern DECL_KERN BDY_GEOM* make_BDY_GEOM( logical cross_curve, const ellipse& cur,
			       double start, double end, const SPAvector& twist0,
			       const SPAvector& twist1, double fullness,
			       logical smooth0, logical smooth1,
			       const SPAunit_vector& magic );

// From a point representing a degenerate curve.

extern DECL_KERN BDY_GEOM* make_BDY_GEOM( logical cross_curve, const SPAposition& P,
			       const SPAunit_vector& N0, const SPAunit_vector& N1,
			       double fullness, logical smooth0,
			       logical smooth1, const SPAunit_vector& magic );

// For a curve where nothing is known of the surface it's on, so making a
// section of boundary where G1 continuity is not maintained.

extern DECL_KERN BDY_GEOM* make_BDY_GEOM( logical cross_curve, const curve& cur,
			       double start, double end, double fullness,
			       const SPAunit_vector& magic );

// ********************************************************************
// Function:
//    BDY_GEOM_restore
// 
// Action:
//    Internal to ACIS and not intended for direct usage.
// 
// Prototype:
//    BDY_GEOM* BDY_GEOM_restore();
// 
// Includes:
//    #include "kernel/acis.hxx"
//    #include "kernel/kerngeom/d3_vbl/vbl_bdy.hxx"
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
//    Restore the data from a save file. This uses a dispatch table, whereby the
//    proper restore functions have been previously registered. At runtime, the proper
//    restore routine is called.
// 
//    if (form == PCURVE_BOUNDARY)
//    BDY_GEOM_PCURVE::restore Routine to perform actual work.
//    else if (form == PLANE_BOUNDARY)
//    BDY_GEOM_PLANE::restore  Routine to perform actual work.
//    else if (form == CIRCLE_BOUNDARY)
//    BDY_GEOM_CIRCLE::restore Routine to perform actual work.
//    else if (form == DEGENERATE_BOUNDARY)
//    BDY_GEOM_DEG::restore    Routine to perform actual work.
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
//    kern/kernel/kerngeom/d3_vbl/vbl_bdy.hxx
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
#endif
