/* ORIGINAL: acis2.1/kerngeom/d3_srf/svc.hxx */
/* $Id: svc.hxx,v 1.36 2002/08/09 17:15:23 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//
// Modification---
//	30Aug07 pvc	added function are_surfaces_G2_at_point.

#ifndef SVCH
#define SVCH

#include "dcl_kern.h"
#include "logical.h"
#include "vec.hxx"
#include "pos.hxx"
#include "uni.hxx"
#include "bsf.hxx"
#include "math.hxx"
#include "surdef.hxx"
#include "errorsys.hxx"
#include "err_info.hxx"

class CURVATURE_MATRIX;
class THIRD_RANK_TENSOR;
class FD_CACHE;
class BDRY_CACHE;
class BOUNDARY;
class DEGENERATE_BOUNDARY;
class CVEC;

// STI ROLL
class SizeAccumulator;
// STI ROLL

#ifdef D3_DEBUG

class PS_FILE;

#endif

#if defined D3_DEBUG

class   D3_ostream;
class   D3_istream;

#endif


// This enumerated type is used to describe the parametric nature of an SVEC. At 
// a regular SVEC, Pu * Pv is non-zero. Other types of singularities which we
// support correspond to one of the enumerated types herein: 

enum SVEC_TYPE 
    {
    SVEC_not_set,                    
    SVEC_regular,                    // Pr * Ps is non-zero
    SVEC_badly_singular,             // Not one of the supported singularities
    SVEC_normal_degeneracy,          // Pr is zero but Prs * Ps is not.
    SVEC_tangent_degeneracy          // Pr is zero, as is Prs * Ps, 
    };

// An svec is a point on a surface. The essential parts of the svec structure
// are the surface (which is accessed through a pointer to a BOUNDED_SURFACE),
// and a SPAparameter pair, if the surface is parametric, or a point if it is not.
// The BOUNDED_SURFACE, and the real surface it contains, are shared and are not
// deleted by the svec.  The BOUNDED_SURFACE may be amended by the SVEC (for
// example, in order to cache surface singularity information) and so is not
// const; the surface it contains, however, is constant.  Derivatives may be
// stored in the svec; these are evaluated lazily.  The surface normal and 
// derivatives at the SVEC are also stored and evaluated lazily. 

class DECL_KERN SVEC : public ACIS_OBJECT
    { 
private:

    BOUNDED_SURFACE* _bsf;              // The underlying surface

    SPApar_pos          _uv;               // Parameter pair
 
    // STL apc 30 Apr 02 Cache the value of the SPAparameter pair mapped to
    // base range [returned from base_uv()]. This helps performance noticably
    // when there is a lot of subdivision e.g. 68184
    SPApar_pos _base_uv;	// Parameter pair mapped to base range

    SPAposition         _Pos;              // Position

    SPAvector*	     _derivs;	      	// Pu, Pv, Puu, Puv, Pvv
    SPAvector*          _normals; 		// Surface normal and derivatives
    int 	     _nd_ws;
    int 	     _nn_ws;
    void	     prepare_deriv_workspace( int nd );
    void	     prepare_normal_workspace( int nn );

    SPAvector*	     _deriv_ptr[6];	// Array of pointers into _derivs, for 
                                        // the benefit of the surface evaluators

    int              _nd_got;          	// Number of derivatives currently stored

    int              _nn_got;           // Number of normal derivatives stored

    BOUNDARY*        _boundary;         // Pointer to closest bdry if singular.

    SVEC_TYPE        _type;             // singular/regular classification. 

    BDRY_CACHE*      _bdry_cache;       // Used to cache boundary derivative
					// info (used mainly if the SVEC is
					// singular)

    FD_CACHE*        _fd_cache;         // Used to cache finite difference info
					// (again, used mainly if the SVEC is
					// singular so that high derivatives are
					// required)

    // Evaluation sides.  Svecs record the u and v evaluation sides separately
    // because this is more convenient. _uside and _vside will have the value
    // -1, 0 or 1. 1 implies evaluation above, -1 below, and 0 implies that the
    // side is not important and need not be considered. The value 99 is used in
    // arguments to functions and indicates that the caller doesn't know the
    // side, and wants it to be set appropriately (the value 99 is never
    // stored). quad is kept up to date with _uside and _vside and is used when
    // the surface evaluate function is called. 

    int 	     _uside;		// Evaluation sides
    int		     _vside;

    evaluate_surface_quadrant _quad;    // evaluation quadrant.

    // Private functions providing lazy evaluation follow.

    // Evaluate and store n derivatives: the return value indicates how many
    // derivatives were calculated, which is n if there were no problems. The
    // n_approx argument indicates how many of the requested derivatives may be
    // approximated; if set to -1, all of them may. If a derivative is requested
    // and is not available in exact form it is approximated unless n_approx
    // indicates it should not be, in which case it is not returned.

    int get_data( int n, int n_approx );  

	// Calculate the derivative of the surface w.r.t original uv before mapping 
	// by SPApar_bilinear_transf.
	
	void update_mapped_derivs( SPAvector * deriv_ptr[], int nd );

    // Find the normal and its derivatives at this SVEC and cache them.  As a 
    // side-effect, find if the point is regular or singular: our operational 
    // definition of a singular point is one where we can't find a normal by 
    // crossing Pu with Pv.  Note this includes the poles of spheres for which 
    // the normal is found easily enough by geometric methods. 

    int get_normals( int n, int n_approx );  

    // Special cases of the above function used when the surface is known to be 
    // a circular cone or a sphere. 

    int get_cone_normals( int n );
    int get_sphere_normals( int n );

    // Set the pointer to the near-boundary which is defined in the function's
    // comments:

    void get_boundary();

    // This function provides the common parts of the constructors:

    void initialize( BOUNDED_SURFACE* bsf, double u, double v, 
		     int us=99, int vs=99 );

public:

    SVEC( BOUNDED_SURFACE* bsf = 0, double u = SPAnull, double v = SPAnull,
	  int us=99, int vs=99 );
    
    SVEC( BOUNDED_SURFACE* bsf, const SPApar_pos& uv, int us=99, int vs=99 );
    
    SVEC( BOUNDED_SURFACE* bsf, const SPAposition& P );
    
    SVEC( const SVEC& svec );
                
    ~SVEC();

    SVEC& operator=( const SVEC& );


    // The overwrite functions return the SVEC so that compound statements can
    // be written e.g. M = svec.overwrite( 0, 0 ).N();

    SVEC& overwrite( double u = SPAnull, double v = SPAnull, int us=99, int vs=99 );

    SVEC& overwrite( const SPApar_pos& uv, int us=99, int vs=99 )
	{ 
	return overwrite( uv.u, uv.v, us, vs ); 
	}

    SVEC& overwrite( const SPAposition& P, int us=99, int vs=99 );

    SVEC& overwrite( const SPApar_pos& uv, const SPAposition& P,
		     SPAvector** derivs = NULL, int ngot = 0,
		     int us=99, int vs=99 );


    // Data access functions: 

    const surface&   sf() const  { return _bsf ? _bsf->sf() 
                                               : *(const surface*)NULL_REF ; }

    BOUNDED_SURFACE& bsf() const { return *_bsf; }	

	SPApar_bilinear_transf const * par_trf() const { return bsf().transf(); }
	
    SPApar_pos&    uv()     { if ( (double)_uv.u == SPAnull ) parametrise( _Pos ); 
			   return _uv; }

    double      u()             { return uv().u; }
    double      v()             { return uv().v; }
    int		uside()		{ return _uside; }
    int		vside()		{ return _vside; }

	// The transformed uv i.e. mapped_uv = uv * transf.

	SPApar_pos mapped_uv( SPApar_pos const &uv ) const;

	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL
/**
 * @nodoc
 */
	virtual void minimize( minimize_helper*);

    // For efficiency, before an svec is used, it is advisable to call 
    // 'prepare_data' to evaluate and cache the derivatives that will be 
    // required. Otherwise, if for example the user first asks for first 
    // derivatives and then for second, part of the work required to evaluate 
    // first derivatives may be repeated. 
    // 
    // prepare_data returns how many derivatives were successfully found. 'n' is
    // the number of derivatives requested, and for a successful evaluation will
    // be the number returned. Some derivatives may be approximated; 'n_approx'
    // is the highest derivative for which an approximation is acceptable, so
    // that the n_approx+1 th derivative (if n_approx < n) will be returned if
    // and only if it can be evaluated precisely. Derivatives up to n_approx
    // might be approximated. In other words, n_approx is the number of
    // derivatives that must be returned, even if this means using
    // approximations.  The default value of -1 implies that any number of 
    // derivatives can be approximated; it is equivalent to n_approx = n. 

    void	check_data( int i );
    void	check_normals( int i );

    int  prepare_data( int n, int n_approx = -1 )  
			{ return n > _nd_got ? get_data( n, n_approx ) : n; }

    SPAposition&   P()             { prepare_data(0); return _Pos; }

    SPAvector&     Pu()            { check_data(1); return _derivs[0]; }
    SPAvector&     Pv()            { check_data(1); return _derivs[1]; }

    SPAvector&     Puu()           { check_data(2); return _derivs[2]; }
    SPAvector&     Puv()           { check_data(2); return _derivs[3]; }
    SPAvector&     Pvv()           { check_data(2); return _derivs[4]; } 
    SPAvector&     Puuu()          { check_data(3); return _derivs[5]; }
    SPAvector&     Puuv()          { check_data(3); return _derivs[6]; }
    SPAvector&     Puvv()          { check_data(3); return _derivs[7]; }
    SPAvector&     Pvvv()          { check_data(3); return _derivs[8]; }

    SPAvector&     Puuuu()         { check_data(4); return _derivs[9]; }
    SPAvector&     Puuuv()         { check_data(4); return _derivs[10]; }
    SPAvector&     Puuvv()         { check_data(4); return _derivs[11]; }
    SPAvector&     Puvvv()         { check_data(4); return _derivs[12]; }
    SPAvector&     Pvvvv()         { check_data(4); return _derivs[13]; }

    SPAvector&     Puuuuu()        { check_data(5); return _derivs[14]; }
    SPAvector&     Puuuuv()        { check_data(5); return _derivs[15]; }
    SPAvector&     Puuuvv()        { check_data(5); return _derivs[16]; }
    SPAvector&     Puuvvv()        { check_data(5); return _derivs[17]; }
    SPAvector&     Puvvvv()        { check_data(5); return _derivs[18]; }
    SPAvector&     Pvvvvv()        { check_data(5); return _derivs[19]; }

    // return derivative d(i+j)P / du(i) dv(j) - e.g. derivative(1,2) = Puvv

    SPAvector&     derivative( int i, int j );


    // For efficiency, before normals are requested, it is necessary to call 
    // 'prepare_normals' to evaluate and cache the derivatives that will be 
    // required.  Note that this is different from the rule for derivatives, 
    // where the call to prepare_data is optional but recommended.  The function 
    // N() is an exception - this can be called with no preceding call to 
    // prepare_normals(). 
    // 
    // Calculate and cache the surface normal N and, optionally, its 
    // derivatives Nu, Nv, Nuu, Nuv, Nvv, Nuuu, Nuuv, Nuvv, Nvvv, Nuuuu, 
    // Nuuuv, Nuuvv, Nuvvv and Nvvvv.  The return code indicates the number of 
    // derivatives calculated. 
    // 
    // If supplied, the n_approx flag indicates how many derivatives may be 
    // approximated if necessary. 
    // e.g. if called with nderiv = 2 and n_approx = 2 all the derivatives 
    // may be approximated, but if n_approx = 1, second derivatives 
    // should only be returned if accurate. The default value of -1 is 
    // equivalent to n_approx = nderiv,  i.e. approximate as many as necessary.

    int		prepare_normals( int n, const int n_approx = -1 )
	{ return n > _nn_got ? get_normals( n, n_approx ) : n; }

    SPAunit_vector&   N()	  { if ( _nn_got == -1 ) get_normals( 0, -1 ); 
			    return * (SPAunit_vector*) & _normals[0]; }

    SPAvector&     Nu()	  { check_normals(1); return _normals[1]; }
    SPAvector&     Nv()	  { check_normals(1); return _normals[2]; }

    SPAvector&     Nuu()     { check_normals(2); return _normals[3]; }
    SPAvector&     Nuv()     { check_normals(2); return _normals[4]; }
    SPAvector&     Nvv()     { check_normals(2); return _normals[5]; }

    SPAvector&     Nuuu()    { check_normals(3); return _normals[6]; }
    SPAvector&     Nuuv()    { check_normals(3); return _normals[7]; }
    SPAvector&     Nuvv()    { check_normals(3); return _normals[8]; }
    SPAvector&     Nvvv()    { check_normals(3); return _normals[9]; }

    SPAvector&     Nuuuu()   { check_normals(4); return _normals[10]; }
    SPAvector&     Nuuuv()   { check_normals(4); return _normals[11]; }
    SPAvector&     Nuuvv()   { check_normals(4); return _normals[12]; }
    SPAvector&     Nuvvv()   { check_normals(4); return _normals[13]; }
    SPAvector&     Nvvvv()   { check_normals(4); return _normals[14]; }

    // return normal derivative d(i+j)N / du(i) dv(j) 
    // - e.g. normal_derivative(1,2) = Nuvv

    SPAvector&     normal_derivative( int i, int j );


    // The boundary and finite differences caches will probably get used mainly
    // for singular points, and when 2nd derivatives of N are required on
    // surfaces without 3rd accurate derivatives; thus they are evaluated lazily.

    BDRY_CACHE& bdry_cache();

    FD_CACHE&   fd_cache(); 

    // Has the svec got null parameters and point?

    logical unset();        


    // Has the svec got null SPAparameter values?

    logical uv_unset()	{ return _uv.u == SPAnull; }


    // Singularity type functions: we only recognise singular points which
    // correspond to a degenerate SPAparameter space edge. 

    SVEC_TYPE type() 
	{ 
	if ( _type == SVEC_not_set ) prepare_normals( 0, -1 ); 
	return _type; 	
	}

    logical regular()            { return type() == SVEC_regular; }

    logical badly_singular()     { return type() == SVEC_badly_singular; }

    logical treatably_singular() { return !regular() && !badly_singular(); }

    // Return the "near-boundary" to this SVEC:

    BOUNDARY& boundary() 
	{ 
	if ( ! _boundary ) get_boundary();
	return *_boundary;
	}

    DEGENERATE_BOUNDARY& degenerate_boundary();

    // Return the direction into the singularity corresponding to the non-zero
    // surface derivative there. The direction points *at* the singular point
    // from inside the surface patch. 

    SPAvector singular_direction();

    // For a periodic surface, return the parameters in the base range 
    // of the surface. 

    SPApar_pos     base_uv();


    // Adjust the parameters of the svec to be as close as possible to those 
    // of the given SPAparameter pair, by adjusting wrt the period. 

    void	adjust_period( const SPApar_pos& );


    // Return U = norm( Pu ) and its derivatives Uu and Uv, and V = N * U,  and
    // its derivatives Vu and Vv. Returns FALSE if the svec is at a bad
    // singularity; simple singularities are OK though. 

    logical UV_vecs( SPAvector& U, SPAvector& Uu, SPAvector& Uv,
                     SPAvector& V, SPAvector& Vu, SPAvector& Vv );

    // Principal directions  - the normal is also returned for convenience. 
    // The function returns TRUE if the directions are unique, FALSE if the 
    // point is umbilical. 

    logical     principal_directions( SPAunit_vector& N, SPAunit_vector& P0,
                                                      SPAunit_vector& P1 );

    // Relax the svec to a SPAposition whose normal goes through the given point.
    // estimate_and_relax estimates parameters if none are supplied, or if
    // relaxation fails with those supplied. parametrise is a very robust
    // version using subdivision, which should only be used if the point is on
    // the surface. For relax and estimate_and_relax the second (logical) argument
    // indicates with TRUE that the relaxation is probably difficult and to use
    // svec_robust_relax straight away. (Over a range of vertex blend examples
    // setting it to TRUE saves over 20%).  
	// The third argument prevents the relaxation from flipping the sense of the 
	// svec normal (if set to TRUE).  This can be used to prevent the relaxation 
	// from moving beyond a surface singularity. 

    logical     relax(const SPAposition&, logical = FALSE, logical = FALSE );
    logical     estimate_and_relax(const SPAposition&, logical=FALSE );  

    // This function does tha same as estimate_and_relax, but it also
    // constarins the point to a given plane. 
    // NOTE: the point is also root_point of the plane.
    logical estimate_and_relax_in_plane(const SPAposition&, const SPAunit_vector&, logical=FALSE );  

    logical     parametrise( const SPAposition& ); 

    // This routine moves the singular SVEC along its degenerate boundary to a
    // point at which the singular_direction there best matches the given
    // direction T. 

    logical	parametrise_singular( CVEC&, logical forwards );

    // Evaluate the distance function from a point X to the surface, when X is 
    // known to lie on the svec normal (i.e. relax has already been called). 

    int		distance( const SPAposition&    iX,
			  double&            value, 
			  CURVATURE_MATRIX&  = *(CURVATURE_MATRIX*) NULL_REF, 
			  THIRD_RANK_TENSOR& = *(THIRD_RANK_TENSOR*)NULL_REF );


    // Represent a SPAvector in the SVEC tangent plane as a*Pu + b*Pv

    SPApar_vec     direction( const SPAvector& D );


    // Computes the same SPApar_vec duv as the above, and also its derivatives duv_u
    // and duv_v with respect to u and v.
    // Return FALSE if we can't find duv, else TRUE.

    logical     directions( const SPAvector& D, SPApar_vec& duv, SPApar_vec& duv_u,
			    SPApar_vec& duv_v );

    // Computes the same SPApar_vec as the above direction function,
    // (i.e. Calculates ut, vt for a supplied SPAvector, represented
    // as ut*Pu + vt*Pv), and also calculates utt and vtt for
    // a second given direction, represented as
    //   ut*ut*Pu + 2*ut*vt*Puv + vt*vt*Pvv + utt*Pu + vtt*Pv.
    // Similarly, uttt, and vttt can be computed for a third given
    // direction represented as
    //   ut*ut*ut*Puuu + 3*ut*ut*vt*Puuv + 3*ut*vt*vt*Puvv +
    //   vt*vt*vt*Pvvv + 3*ut*utt*Puu + 3*(ut*vtt+vt*utt)*Puv +
    //   3*vt*vtt*Pvv + uttt*Pu + vttt*Pv,
    // and if required, utttt and vtttt can be found for a fourth
    // given direction, represented as
    //   ut*ut*ut*ut*Pu + 4*ut*ut*ut*vt*Puuuv + 6*ut*ut*vt*vt*Puuvv +
    //   4*ut*vt*vt*vt*Puvvv + vt*vt*vt*vt*Pvvvv + 6*ut*ut*utt*Puuu +
    //   6*(2*ut*utt*vt+ut*ut*vtt)*Puuv + 6*(utt*vt*vt+2*ut*vt*vtt)*Puvv +
    //   6*vt*vt*vtt*Pvvv + (3*utt*utt+4*ut*utt)*Puu +
    //   (3*utt*vtt+2*ut*vttt+2*uttt*vt)*Puv + (3*vtt*vtt+4*vt*vttt)*Pvv +
    //   utttt*Pu + vtttt*Pv.
    //
    // The offset is non-zero if the CVEC is offset from the SVEC bounded
    // surface. In this case, the SPAparameter derivatives for the offset
    // surface are calculated. Note that if the offset is zero, nd_in - 1
    // normal derivatives are required, whereas if the offset is non-zero,
    // nd_in normal derivatives are required.
    // The number of derivatives that were succesfully computed are returned
    // via the supplied duv pointer. It is the responsibility of the caller
    // to ensure that enough normal derivatives and enough SPApar_vec workspace
    // in the duv pointer are supplied. The caller also must supply one
    // direction SPAvector for each derivative requested.
    //

    int param_derivs( CVEC &cvec, SPApar_vec *duv, int nd, 
		      const double offset = 0.0, const int n_approx = -1 );

    // Same as the above param_derivs function, but takes an array of
    // pointers to curve derivatives instead of a CVEC. It is
    // preferable to use the version of this function which takes a
    // CVEC, as then more derivatives can be calculated if needed for
    // the case where the SVEC is singular.

    int param_derivs( const SPAvector **cu_derivs, SPApar_vec *duv, int nd_in, 
		      const double offset = 0.0, const int n_approx = -1 );


    // Return derivatives of the normal wrt the CVEC curve SPAparameter. 

    int normal_derivs( CVEC &cvec, SPAvector *Nd, int nd, const int n_approx=-1 );


    // Normal curvature in a direction in the tangent plane. 

    double      kn( const SPAvector& D );


#if defined D3_DEBUG

    void        print( D3_ostream& os, int level )      const;
    void        input( D3_istream& is );

#endif
     
    
#ifdef D3_DEBUG

    void        draw( PS_FILE& ps )                     const; 

#endif
    };

DECL_KERN logical acceptable_normal_length( const double& d, 
					    BOUNDED_SURFACE& bsf );

// The FD_CACHE class is declared here as it is used in the
// SVEC functions when the SVEC is singular. An SVEC may point to a
// FD_CACHE object: these objects provide additional evaluation functions
// for the SVEC if the SVEC is singular or if finite difference methods are
// required to calculate derivatives.

// The finite-difference cache class support evaluation of SVEC quantities by
// finite differencing. By storing nearby the SVECs we speed up evaluation if
// for example both 3rd surface derivatives and normal derivatives have to be
// calculated by finite differences. The boundary coordinate system (rs) is used
// in order to generate nearby points which are internal to the patch. The
// finite difference scheme is one-sided and second order accuarate. 

class DECL_KERN FD_CACHE : public ACIS_OBJECT
    {
private:

    SVEC*     _sv;        // pointer to parent SVEC 

    double    _dr;        // finite difference shift in r
    double    _ds;        //    ditto                in s

    SVEC      _sv_dr1;    // this is _sv shifted by _dr in the r direction
    SVEC      _sv_dr2;    // this is _sv shifted by 2*_dr in the r direction
    SVEC      _sv_ds1;    // this is _sv shifted by _ds in the s direction
    SVEC      _sv_ds2;    // this is _sv shifted by 2*_ds in the s direction
    
    void    setup();      // Set up the finite differencing points

public:

    FD_CACHE( SVEC* sv );

    ~FD_CACHE();
    
    void overwrite()      { _dr = SPAnull; }

    void prepare_svecs()  { if ( _dr == SPAnull ) setup(); }

    SVEC&         svec()  { return *_sv; }

    SVEC&         sv_dr1() { prepare_svecs(); return _sv_dr1; }
    SVEC&         sv_dr2() { prepare_svecs(); return _sv_dr2; }
    SVEC&         sv_ds1() { prepare_svecs(); return _sv_ds1; }
    SVEC&         sv_ds2() { prepare_svecs(); return _sv_ds2; }
    const double& dr()     { prepare_svecs(); return _dr; }
    const double& ds()     { prepare_svecs(); return _ds; }

    BOUNDARY&     bdry()   { return _sv->boundary(); }

    // These functions are the main service that the class provides; they return
    // TRUE if they evaluate successfully what was asked for. 

    logical surface_3rd_derivs( SPAvector& Puuu, SPAvector& Puuv,
			        SPAvector& Puvv, SPAvector& Pvvv );

    logical normal_1st_derivs( SPAvector& Nu, SPAvector& Nv );

    logical normal_2nd_derivs( SPAvector& Nuu, SPAvector& Nuv, SPAvector& Nvv );

	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL

#if defined D3_DEBUG

    void        print( D3_ostream& os, int level )      const;

#endif
    };


// If an illegal svec is found, then the bsf owning it may instruct the 
// svec to abort processing.  It then generates a sys_error with an
// error_info packet containing the parameters of the bad point. 

class DECL_KERN svec_error_info : public error_info 
    {
    SPApar_pos _uv;	// Parameters of illegal point on surface

public:
    svec_error_info( const SPApar_pos& uv ) : _uv( uv ) {}

    SPApar_pos& 	uv()	{ return _uv; }

    static 	int	id();
    virtual	int	type()	const;
    };




#if defined D3_DEBUG

extern  DECL_KERN D3_ostream& operator<<( D3_ostream& os, const SVEC& sv );
extern  DECL_KERN D3_ostream& operator<<( D3_ostream& os, const SVEC_TYPE );

extern  DECL_KERN D3_ostream& operator<<( D3_ostream& os, const FD_CACHE& );

extern  DECL_KERN D3_istream& operator>>( D3_istream& is, SVEC& sv );

#endif

DECL_KERN
logical svecs_equal( SVEC& sv0, SVEC& sv1, double tol = SPAresabs );

#endif
