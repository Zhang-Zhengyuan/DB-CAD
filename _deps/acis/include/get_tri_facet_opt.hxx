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
#if !defined(GET_TRI_FACET_OPT_HXX)
#define GET_TRI_FACET_OPT_HXX
#include "dcl_compound.h"
#include "acis.hxx"
/**
 * @file get_tri_facet_opt.hxx
 * \addtogroup ACISCOMPOUND
 * @{
 */
/**
*<summary> Options for get triangle face information.</summary>
*/
class DECL_COMPOUND get_tri_facet_options : public ACIS_OBJECT
{
	/**
	* <summary>Should the normals be extracted.Default value is true.</summary>
	*/
	logical _get_normals;

public:
	/**
	* <summary>Constructor.</summary>
	*/
	get_tri_facet_options();
	/**
	* <summary>Set normals to be extracted.</summary>
	*/
	void set_get_normals(const logical& value){_get_normals = value;};
	/**
	* <summary>Should the normals be extracted.Default value is true.</summary>
	*/
	logical get_normals() const{ return _get_normals;};

};

/** @} */
#endif
