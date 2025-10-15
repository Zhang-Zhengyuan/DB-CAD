/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef tighten_gaps_options_hxx
#define tighten_gaps_options_hxx

#include "dcl_heal.h"
#include "api.hxx"

class tighten_gaps_options_impl;

/**
* @file tighten_gaps_options.hxx
* @CAA2Level L1
* @CAA2Usage U1
* \addtogroup HEALAPI
*
* @{
*/

/**
* tighten_gaps_options is used to control the behavior of @href api_tighten_gaps. For example, the user supplied <tt>desired_gap_tightness</tt> value serves as a threshold 
* and TEDGEs / TVERTEXs with bigger tolerance values than this threshold will be tightened by api_tighten_gaps. A user is encouraged
* to set the <tt>desired_gap_tightness</tt> value to the desired manufacturability tolerance of the user application.
* The user can also choose to tighten only tangent analytic junctions or isospline junctions or sharp junctions or a combination using the tighten_gaps_option object. 
* For more details, refer to the Tighten Gaps Options technical article.
**/
class DECL_HEAL tighten_gaps_options : public ACIS_OBJECT
{
	tighten_gaps_options_impl* _impl;
	tighten_gaps_options (tighten_gaps_options const& other );
	tighten_gaps_options& operator=(tighten_gaps_options const& other );
public:
	tighten_gaps_options();
	~tighten_gaps_options();

	/**
	*Returns the desired gap tightness value.
	*/
	const double get_desired_tightness_value() const;
	/**
	* Sets the gap tightness value that a user desires in the body. The minimum value acceptable is SPAresabs. 
	* The default value is SPAresabs.
	*/
	void set_desired_tightness_value(double gap_tightness_val);

	/**
	* Returns the choice to heal all edges and vertices shared by analytic surfaces.
	*/
	const bool get_heal_all_analytic_junctions() const;
	/** 
	* Sets the choice to  heal all edges and vertices shared by analytic surfaces.
	* The default value is <tt>TRUE</tt>
	*/
	void set_heal_all_analytic_junctions(const bool heal_analytic);

	/**
	* Returns the choice to heal all edges shared by tangential isoparametric surfaces.	
	*/
	const bool get_heal_all_tangential_isospline_junctions() const;
	/**
	* Sets the choice to heal all edges shared by tangential isoparametric surfaces.	
	* The default value is <tt>TRUE</tt>
	*/
	void set_heal_all_tangential_isospline_junctions(const bool heal_isospl_edges);

	/**
	* Returns the choice to heal all edges and vertices that are shared by surfaces that intersect sharply.
	*/
	const bool get_heal_all_sharp_junctions() const;
	/**
	* Sets the choice to heal all edges and vertices that are shared by surfaces that intersect sharply.
	* The default value is <tt>TRUE</tt>
	*/
	void set_heal_all_sharp_junctions(const bool heal_sharp);
	
	/**
	*Returns the list of untightened edges and vertices.
	*/
	const ENTITY_LIST& get_untightened_entities() const;
	
	/**
	*@nodoc
	**/
	//internal use only.
	tighten_gaps_options_impl* get_impl();

	/**
	*@nodoc
	**/
	void clear();

};
/** @} */
#endif // tighten_gaps_options_hxx
