/* ORIGINAL: acis2.1/kernbody/faceprop/faceprop.hxx */
/* $Id: faceprop.hxx,v 1.7 2002/08/09 17:18:45 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Define the class to contain areal properties, including summation
// operators.

#if !defined( AREA_PROP_CLASS )
#define AREA_PROP_CLASS

#include "dcl_query.h"
#include "massprop.hxx"

class area_property;
DECL_QUERY area_property operator-(area_property const &);
DECL_QUERY area_property operator+(area_property const &,area_property const &);
DECL_QUERY area_property operator-(area_property const &,area_property const &);
DECL_QUERY area_property operator*(double,area_property const &);
DECL_QUERY area_property operator*(area_property const &,double);
DECL_QUERY area_property operator*(area_property const &,SPAtransf const &);

class DECL_QUERY area_property: public moments {
	area_property( moments const & );

public:
	area_property();	// make zeroed area_property
	area_property( double );	// set area only
	area_property( double, double, double, double, double, double );
	area_property( area_property const & );
	area_property(
				double,
				SPAvector const &,
				symtensor const &
			);

	// Historical function names.

	double area() const;
	double first_moment_a() const;
	double first_moment_b() const;
	double second_moment_a() const;
	double second_moment_b() const;
	double second_moment_ab() const;

	// Simple linear operators for accumulating area properties.

	friend DECL_QUERY area_property operator+(
				area_property const &,
				area_property const &
			);
	friend DECL_QUERY area_property operator-(
				area_property const &,
				area_property const &
			);
	friend DECL_QUERY area_property operator-(
				area_property const &
			);
	area_property const &operator+=(
				area_property const &
			);
	area_property const &operator-=(
				area_property const &
			);
	friend DECL_QUERY area_property operator*(
				double,
				area_property const &
			);
	friend DECL_QUERY area_property operator*(
				area_property const &,
				double
			);
	area_property const &operator*=(
				double
			);

	// Transform the area property to correspond to the transformed
	// body.

	friend DECL_QUERY area_property operator*(
				area_property const &,
				SPAtransf const &
			);
	area_property const &operator*=(
				SPAtransf const &
			);
};

#endif
