/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Defines the class blend_eval_version
#ifndef BLND_EVAL_VER_HXX
#define BLND_EVAL_VER_HXX

#include "dcl_kern.h"
#include "logical.h"
#include "mmgr.hxx"

// This class represents a blend evaluator version
class DECL_KERN blend_eval_version : public ACIS_OBJECT
{
protected:
	enum eval_version_type
	{
		blend_eval_none = 0,
		new_evaluator_version = 1,
		hermite_uinterp_version = 2,
		proj_holdline_version = 4,
		sided_eval_version = 8,
		common_approx_version = 16,
		three_curve_pointy_end_version = 32,
		var_ssi_version = 64,
		new_fdc_evaluator_version = 128
	};

	// Attributes
	int m_ver_val;
	logical m_is_constrain_eval_version;

	// Internal functions
	blend_eval_version& copy(const blend_eval_version &from);

	void set_version( eval_version_type ver, logical enable );

public:
	// Default constructor
	blend_eval_version();

	// Copy constructor
	blend_eval_version(const blend_eval_version &from);

	// Assignment operator
	blend_eval_version& operator=(const blend_eval_version &from);
	
	// Destructor
	~blend_eval_version();

	// Query functions - to know the respective version
	logical is_constrain_eval_version() const
	{	return m_is_constrain_eval_version;	}

	logical is_new_evaluator_version() const
	{	return ( m_ver_val & new_evaluator_version ) != 0;	}

	logical is_hermite_uinterp_version() const
	{	return ( m_ver_val & hermite_uinterp_version ) != 0; }

	logical is_proj_holdline_version() const
	{	return ( m_ver_val & proj_holdline_version ) != 0; }

	logical is_sided_eval_version() const
	{	return ( m_ver_val & sided_eval_version) != 0; }

	logical is_common_approx_version() const
	{	return ( m_ver_val & common_approx_version ) != 0; }

	logical is_three_curve_pointy_end_version() const
	{	return ( m_ver_val & three_curve_pointy_end_version ) != 0; }

	logical is_var_ssi_version() const
	{ return ( m_ver_val & var_ssi_version ) != 0; }

	logical is_new_fdc_evaluator_version() const
	{	return ( m_ver_val & new_fdc_evaluator_version ) != 0;	}

	// Type cast to an int for direct write into SAT files

	operator int() const
	{	return m_ver_val;	}

	// Function to set a version - this will get added to the existing 
	// versions.

	void set_constrain_eval_version(logical enable = TRUE);

	void set_new_evaluator_version(logical enable = TRUE);

	void set_hermite_uinterp_version(logical enable = TRUE);

	void set_proj_holdline_version(logical enable = TRUE);

	void set_sided_eval_version(logical enable = TRUE);

	void set_common_approx_version(logical enable = TRUE);

	void set_three_curve_pointy_end_version(logical enable = TRUE);

	void set_var_ssi_version( logical enable = TRUE );
	
	void set_new_fdc_evaluator_version( logical enable = TRUE );

	// Make blend surface class a friend of this
	friend class blend_spl_sur;

	friend logical operator==( blend_eval_version const& v1, blend_eval_version const& v2 );
};

// STI let 10 Oct 2018: Added an operator==
inline logical operator==( blend_eval_version const& v1, blend_eval_version const& v2 )
{
	return ( (v1.m_ver_val == v2.m_ver_val) && 
			 (v1.m_is_constrain_eval_version == v2.m_is_constrain_eval_version) );
}

#endif

