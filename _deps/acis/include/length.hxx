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
#if !defined(LENGTH_HXX)
#define LENGTH_HXX
#include "dcl_compound.h"
#include "acis.hxx"
#include "length_units.hxx"
/**
 * @file length.hxx
 * \addtogroup ACISCOMPOUND
 * @{
 */
/**
*<summary>Length</summary>
*/
class DECL_COMPOUND length : public ACIS_OBJECT
{

	double _value;

    length_units _units;

	length();

public:


	~length();

	length(const length& rhs);

    length(const double& xunits, const length_units& type);

	length const& operator=(const length& rhs);

	length const& operator*(const double& rhs);
	length const& operator*=(const double& rhs);

	length const& operator/(const double& rhs);
	length const& operator/=(const double& rhs);

	logical operator==(const length& rhs) const;

	logical operator!=(const length& rhs) const;

    double value() const {return _value;};

    length_units units() const {return _units;};

    double expressed_in(const length_units& type) const;

};
/** @} */
#endif
