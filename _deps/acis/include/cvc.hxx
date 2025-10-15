/* ORIGINAL: acis2.1/kerngeom/d3_crv/cvc.hxx */
/* $Id: cvc.hxx,v 1.20 2002/08/09 17:15:22 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef CVCH
#define CVCH

#include "dcl_kern.h"
#include "vec.hxx"
#include "pos.hxx"
#include "uni.hxx"
#include "bcu.hxx"
#include "cur.hxx"
#include "math.hxx"
#include "curdef.hxx"

// STI ROLL
class SizeAccumulator;
// STI ROLL

#if defined D3_STANDALONE || defined D3_DEBUG

class D3_ostream;
class D3_istream;
 
#endif


// A cvec is a point on a curve. The essential parts of the cvec structure are
// the curve, and a SPAparameter value. The curve is shared and is not deleted 
// or amended by the cvec. Derivatives may be stored in the cvec; 
// these are evaluated lazily. 

class DECL_KERN CVEC    : public ACIS_OBJECT
    {
    BOUNDED_CURVE* _bcu;              // The underlying bounded curve
    SPAparameter      _t;                // Parameter value
    SPAposition       _Pos;              // Position

    enum { MAX_DERIVS = 5 };          // Maximum number of derivatives that can
                                      // be cached in the CVEC.

    SPAvector     _derivs[ MAX_DERIVS ]; // Array of derivatives

    SPAvector* _deriv_ptr[ MAX_DERIVS ]; // Array of pointers to derivatives, used
                                      // in evaluation

    int            _nderivs;          // Number of derivatives currently stored
    int            _n_accurate;       // Number which are accurate

    int		   _side;	      // Evaluation side.

    // Evaluate and store n derivatives; the return value specifies how many
    // derivatives were successfully calculated. 

    int            get_data( int n ); 
    
    // This function provides the common part of CVEC construction:

    void initialize( BOUNDED_CURVE* bcu = NULL, double t = SPAnull, int side = 0 );

public:

    
    CVEC();                           // Dummy constructor to allow arrays 
                                      // of uninitialized cvecs
    
    CVEC( BOUNDED_CURVE* bcu, double t=SPAnull, int side = 0 );
    
    CVEC( const CVEC& cvec );

    CVEC& operator=( const CVEC& );
    
    ~CVEC();
    
    logical unset() const { return (double)_t == SPAnull; }

    CVEC& overwrite( double t = SPAnull, int side = 0 );
    
    int prepare_data( int n )   { return n > _nderivs ? get_data( n ) : n; }

    const curve&   cu()   	const   { return _bcu->cu(); }

    BOUNDED_CURVE& bcu()	const	{ return *_bcu; }

    int n_accurate_derivs() const { return _n_accurate; }

    SPAparameter   t()    const   { return _t; }
    SPAposition&   P()            { prepare_data( 0 );  return _Pos; }
    SPAvector&     Pt()           { prepare_data( 1 );  return _derivs[0]; }
    SPAvector&     Ptt()          { prepare_data( 2 );  return _derivs[1]; }
    SPAvector&     Pttt()         { prepare_data( 3 );  return _derivs[2]; }
    SPAvector&     Ptttt()        { prepare_data( 4 );  return _derivs[3]; }
    SPAvector&     Pttttt()       { prepare_data( 5 );  return _derivs[4]; }

    // Return the n'th derivative:

    const SPAvector&     derivative( const int n );

    int		side() const   { return _side; }

    SPAunit_vector T();
    SPAvector      Tt();          // dT/dt
    int		tangents( SPAvector* tan, int nd );

    double      base_t();      // Return SPAparameter reduced to base 
                               // range for periodic curve

    logical     relax( const SPAposition& iX );
    logical     estimate_and_relax( const SPAposition& iX );
    logical     relax_to_plane( const SPAposition& iX, const SPAunit_vector& N );
    logical	parametrise(const SPAposition& );
    
#if defined D3_STANDALONE || defined D3_DEBUG
    
    void        print( D3_ostream& os, int level ) const;
    void        input( D3_istream& is );
    
#endif

	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL
/**
 * @nodoc
 */
	virtual void minimize( minimize_helper*);
};

// A class which represents an array of CVECs. This is provided because
// "delete [] cvecs" (where cvecs points to an array of CVECs) doesn't work on
// certain compilers, and this class keeps the messy alternative hidden.

class DECL_KERN CVEC_ARRAY : public ACIS_OBJECT
    {
	public:
    int		n;
    CVEC*	cvecs;
    CVEC_ARRAY( int num = 0 ) { cvecs = ACIS_NEW CVEC[ n = num ]; };
    CVEC&	operator[] ( int i ) const { return cvecs[ i ]; };
    CVEC_ARRAY& operator=( const CVEC_ARRAY& other );
    ~CVEC_ARRAY();
    };

#if defined D3_STANDALONE || defined D3_DEBUG

extern  DECL_KERN D3_ostream& operator<<( D3_ostream& os, const CVEC& cv );
extern  DECL_KERN D3_istream& operator>>( D3_istream& is, CVEC& cv );

#endif
#endif

