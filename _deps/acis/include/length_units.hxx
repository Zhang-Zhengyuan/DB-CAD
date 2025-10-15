/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/**
* ACIS Compound
*/
#if !defined(LENGTH_UNITS_HXX)
#define LENGTH_UNITS_HXX
#pragma once
#include "dcl_compound.h"
#include "acis.hxx"

/**
 * @file length_units.hxx
 * \addtogroup ACISCOMPOUND
 * @{
 */
/**
*<summary>Class that describes units of length.</summary>
*/
class DECL_COMPOUND length_units : public ACIS_OBJECT
{
friend class length;
	double _factor;

	SPAWCHAR_T* _name;

	length_units(const double& factor, const SPAWCHAR_T* name);

	double convert_factor_to(const length_units& type) const;

public:

    length_units();

	~length_units();

	length_units(const length_units& rhs);

    length_units( const SPAWCHAR_T* name, const double& isxunits, const length_units& of_type);

	length_units const& operator=(const length_units& rhs) ;

	logical operator==(const length_units& rhs) const;

	logical operator!=(const length_units& rhs) const;

	static length_units mm();

	static length_units m();

	static length_units inch();

	static length_units cm();

    double factor() const {return _factor;};

	const SPAWCHAR_T* name() const{return _name;};

};

/** @} */
#endif

