/* ORIGINAL: acis2.1/kerngeom/d3_crv/bcu.hxx */
/* $Id: bcu.hxx,v 1.14 2002/08/09 17:15:22 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef BCUH
#define BCUH

#include "dcl_kern.h"
#include "ivl.hxx"
#include "math.hxx"
#include "curdef.hxx"

class CVEC_ARRAY;
class SPAN;
class SPAposition;

// STI ROLL
class SizeAccumulator;
// STI ROLL


class DECL_KERN BOUNDED_CURVE : public ACIS_OBJECT
{
	const curve* _cu;
	SPAinterval	_range;
	SPAinterval	_unsubset_range;
	SPAN* _root;
	logical	_periodic;
	int _acc_derivs;    // Number of accurate curve derivatives
	                    // is cached here for efficiency.
	double _turning_angle;
	void calculate_turning_angle();

	// This function is called to make the root SPAN '_root' when it is zero.

	void make_root();

public:
	BOUNDED_CURVE();

	BOUNDED_CURVE( const curve* cu, const SPAinterval& range );

	~BOUNDED_CURVE();

	BOUNDED_CURVE& operator=( const BOUNDED_CURVE& );

	const curve& cu() const { return *_cu; }
	SPAinterval	range() const { return _range; }
	SPAinterval	unsubset_range();
	SPAN* root() { if ( _root == 0 ) make_root(); return _root; }

	double start() const { return _range.start_pt(); }
	double end() const { return _range.end_pt(); }

	logical periodic() const { return _periodic; }
	logical closed() const;

	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL
	/**
	* @nodoc
	*/
	virtual void minimize( minimize_helper*);

	int accurate_derivs()
	{ 
		if ( _acc_derivs < 0 ) _acc_derivs = _cu->accurate_derivs();
		return _acc_derivs; 
	}

	double turning_angle()
	{ 
		if ( _turning_angle == SPAnull ) calculate_turning_angle();
		return _turning_angle; 
	}

	// Find the smallest distance from the point iX to the curve; return TRUE if
	// that point has a curve tangent orthogonal to the separation SPAvector.

	logical distance( 
		const SPAposition& iX, 
		double& min_distance, 
		double& t_min,
		double fit = 0.001 * SPAresabs 
		);

	// March the bounded curve, attempting to achieve an angle of
	// target_angle at each step, and returning an array of cvecs.
	// npts is set to the number of cvecs. 

	CVEC_ARRAY*	march( int& npts, double target_angle = 0.4 );


#if defined D3_DEBUG 

	void plot( FILE* fp );

	void save( FILE* fp );

#endif
};


extern DECL_KERN logical _BCU_point_min_distance( const BOUNDED_CURVE& bcu, 
												 const SPAposition&,
												 double* dist, 
												 double* t );


#if defined D3_STANDALONE || defined D3_DEBUG

extern DECL_KERN D3_ostream &operator<<( D3_ostream &os, const BOUNDED_CURVE& bcu );
extern DECL_KERN BOUNDED_CURVE*	BCU_input( D3_istream&, curve*& );

#endif
#endif
