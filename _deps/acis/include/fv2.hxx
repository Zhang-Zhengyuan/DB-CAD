/* ORIGINAL: acis2.1/kernutil/d3_fn2/fv2.hxx */
/* $Id: fv2.hxx,v 1.17 2002/08/09 17:15:29 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef FV2H
#define FV2H

#include "dcl_kern.h"
#include "logical.h"
#include "cnc.hxx"
#include "res.hxx"

class 	FUNC_2V;


#if defined D3_STANDALONE || defined D3_DEBUG

class D3_ostream;
class D3_istream;

#endif

enum FVAL_2V_TYPE
    {
    FVAL_2V_ROOT,		// Fval found by root finder
    FVAL_2V_EXTREME,		// Fval found by turning point finder
    FVAL_2V_AXIS,		// Fval found by axispoint finder
    FVAL_2V_SECOND,		// Fval found by second-order turning pt finder
    FVAL_2V_UNCLASSIFIED	
    };

#if defined D3_STANDALONE || defined D3_DEBUG

extern	DECL_KERN D3_ostream& operator<<( D3_ostream&, const FVAL_2V_TYPE& );
extern	DECL_KERN D3_istream& operator>>( D3_istream&, FVAL_2V_TYPE& );

#endif

class DECL_KERN FVAL_2V  : public ACIS_OBJECT
    {
private:
    CONIC	_conic;		// Local expansion of the function

    double      r, s;           // The mapping from the conic axes to the
                                // function SPAparameter space is
                                //      u = x*r - v*s, v = u*s + v*r
                                //      ( r = cos(theta), s = sin(theta) )
 

    // Map the conic parameters into the surface parameters, and vice versa.

    SPApar_vec     conic_to_pspace( const SPApar_vec& )	const;
    SPApar_vec     pspace_to_conic( const SPApar_vec& )	const;


    void	make_conic();	// Create the conic expansion

public:

    FUNC_2V*	_fn;
    SPApar_pos	_uv;
    double 	_value;
    double	_fu;
    double	_fv;
    double	_fuu;
    double	_fuv;
    double	_fvv;

	double _fuuu;
	double _fuuv;
	double _fuvv;
	double _fvvv;

    FVAL_2V_TYPE _type;

public:
    
	void	set_type( FVAL_2V_TYPE type )	{ _type = type; }

    CONIC&	conic()	{ if ( _conic.unset() ) make_conic(); return _conic; }

    friend class FUNC_2V;

		FVAL_2V( FUNC_2V* fn, FVAL_2V_TYPE = FVAL_2V_UNCLASSIFIED );

    void set_fn( FUNC_2V* fn )	{ if(fn!=_fn) set_to_null(); _fn = fn; }

		FVAL_2V();

		FVAL_2V( const FVAL_2V& fval );

    virtual	void	reset();

    void set_to_null() 
	{ _value = _fu = _fv = _fuu = _fuv = _fvv = _fuuu = _fuuv = _fuvv = _fvvv = SPAnull;
	  _type = FVAL_2V_UNCLASSIFIED;
	  }

    virtual	int overwrite( const SPApar_pos& uv, 
				   const SPApar_vec& dir = *(SPApar_vec*) NULL_REF ,int derivs = 2);

    void	set_values( double, double, double, 
				   double, double, double, const SPApar_pos& uv,
				   double fuuu = SPAnull,double fuuv = SPAnull,double fuvv = SPAnull,double fvvv = SPAnull
				    );

    virtual FVAL_2V& 	operator=( const FVAL_2V& fval );

    virtual	FVAL_2V* copy()	const;


    // The periodic copy function copies the fval and overwrites the 
    // SPAparameter values with the supplied ones. It has no way of checking 
    // whether this is valid...

    virtual	FVAL_2V* periodic_copy( const SPApar_pos& )	const;


    // The default destructor does nothing, but is provided so that 
    // derived classes can inherit it properly. 

    virtual	~FVAL_2V();


    FUNC_2V&	fn()		const	{ return *_fn; }
    const SPApar_pos&	uv()	const	{ return _uv; }
    double	value()		const	{ return _value; }
    double	fu()		const	{ return _fu; }
    double	fv()		const	{ return _fv; }
    double	fuu()		const	{ return _fuu; }
    double	fuv()		const	{ return _fuv; }
    double	fvv()		const	{ return _fvv; }
    double	fuuu()		const	{ return _fuuu; }
    double	fuuv()		const	{ return _fuuv; }
    double	fuvv()		const	{ return _fuvv; }
    double	fvvv()		const	{ return _fvvv; }
    double	deriv( int i, int j )	const;

    // The convention on tangent directions is that the function value is 
    // positive to the right hand side. The FVAL_2V::forward_tangent and 
    // FVAL_2V::backward_tangent functions are for use at a terminator 
    // (crossing lines). The opposite direction to that returned by 
    // FVAL_2V::forward_tangent is also a forward tangent (the direction 
    // returned is arbitrary, but consistent). 

    SPApar_dir	tangent() const	{ return SPApar_dir( -_fv, _fu ); }
    SPApar_dir	forward_tangent();
    SPApar_dir	backward_tangent();


    // Return the conic axis direction closest to the supplied direction
    // Return a step to this axis, or the distance to the axis. 

    SPApar_dir	conic_axisdir( const SPApar_dir& ref );	

    SPApar_vec	step_to_axispoint( const SPApar_dir& ref );

    double	conic_axisdist( const SPApar_dir& ref );


    // This function returns the curvature, directed with the gradient. 
    // A negative curvature indicates a concave curve (so that a circle, 
    // for example, has negative curvature). 

    virtual	double 	curvature()	const;		


    // See the file cnc.h for a precise description of the following functions. 

    SPApar_vec	step_to_centre() 
			{ return conic_to_pspace( conic().centre() ); }
    SPApar_vec	step_to_nearpoint() 
			{ return conic_to_pspace( conic().nearpoint() ); }
    SPApar_vec	step_to_farpoint() 
			{ return conic_to_pspace( conic().farpoint() ); }

    FVAL_2V_TYPE type()		const	{ return _type; }
 

    // Queries about the shape of the conic - is it closed, does it represent 
    // parallel lines, is the leading term positive. 

    logical	conic_closed()		{ return conic().closed(); }
    logical	conic_parallel()	{ return conic().parallel(); }
    logical	conic_positive()	{ return conic().positive(); }

	logical	zero()	const;
    logical	flat()	const 	{ return _fu*_fu + _fv*_fv < SPAresnor*SPAresnor; }


    // The degenerate() function states whether tangent directions can be
    // calculated from the second order terms, given that the FVAL_2V is flat. 
    // It is virtual so that SSI_FVALs can ask the underlying surfaces if
    // it's looking at a degenerate point (in this context, a non-C2 point).

    virtual 	logical	degenerate();


    // Test whether an FVAL_2V has all its derivatives set (if it hasn't, the
    // conic expansion cannot be performed). 

    logical	incomplete()
	{ return _fuu == SPAnull || _fuv == SPAnull || _fvv == SPAnull ||
	         _fu == SPAnull  || _fv  == SPAnull || _value == SPAnull; }

    // Test whether a turning point is isolated. If it is degenerate it is never
    // considered to be isolated. 

    logical	isolated();


    // The 'flat extent' of a terminator is the distance, along one of
    // the axis directions, to a point with a value exceeding SPAresabs. 

    double	minimum_flat_extent() { return conic().minimum_flat_extent(); }
    SPApar_vec	minimum_flat_extent_step() 
            { return conic_to_pspace( conic().minimum_flat_extent_step() ); } 
    double	maximum_flat_extent();

    logical	close( const SPApar_pos& ppos );


    // Note that the following functions indicate how the FVAL_2V was
    // formed - they are not geometric tests on whether the FVAL_2V has a 
    // particular property. 

    logical	is_root()	const		// Was fval_2v found by 
	    { return _type == FVAL_2V_ROOT; }	// FUNC_2V::nearpoint or a 
                                                // similar relaxation function?

    logical	is_tangent()	const		// Was fval_2v found by 
	    { return _type == FVAL_2V_EXTREME; }// FUNC_2V::turning_point?


    // Return the number of derivatives that the fval is evaluated to
    int derivs_evaluated() const;

    // For a periodic function, return the parameters in the base range 
    // of the function. 

    SPApar_pos     base_uv();


    // Move the fval to the base range, allowing for periodicity. An fval on 
    // the upper boundary is moved to the lower one. 

    virtual	void	move_to_base_range(); 


    // Adjust the parameters of the fval to be as close as possible to those 
    // of the supplied fval, by adjusting wrt the period. 

    virtual	void	adjust_period( FVAL_2V& target );


#if defined D3_STANDALONE || defined D3_DEBUG

    virtual void print( D3_ostream&, int level )	const;
    virtual void input( D3_istream& );

#endif
    };


#if defined D3_STANDALONE || defined D3_DEBUG

extern DECL_KERN D3_ostream& operator<<( D3_ostream&, const FVAL_2V& );
extern DECL_KERN D3_istream& operator>>( D3_istream&, FVAL_2V& );

#endif
#endif
