/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

/**
 * @file tolerize_ent_opts.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISTOLERANT
 *
 * @{
 */
#ifndef TOLERIZE_ENT_CLASS
#define TOLERIZE_ENT_CLASS
//
// Options object for api_tolerize_entity
//
#include "dcl_kern.h"
#include "tol_param.hxx"
#include "lists.hxx"
#include "logical_state.hxx"
class AcisOptions;
class outcome;
class error_info_list;
class teo_data;
/**
* options object for use with api_tolerize_entity.
* <br><br>
* <bf>Role:<\bf>
* This object allows the user to communicate with the API.
* <br>
* When an edge or vertex is replaced by a tedge or tvertex,
* the old pointer to the edge or vertex no longer points
* to anything in the model.  The remap_ptr method
* allows the user to get the pointer to the tolerant object
* corresponding to the old edge or vertex.
* <br>
* There is an option to automatically repair tedge errors.
*<br>
*  There is an option for the API to record gaps as it tolerizes, so that 
* the user can look up the gaps using get_original_gap.
*<br>
* The output data stored in the options object is
* cleared each time the API is called.
***/
class DECL_KERN tolerize_entity_opts: public ACIS_OBJECT
{
private:
	/**
	* @nodoc
	**/
	teo_data *data;
	/**
	* @nodoc
	**/
	logical_state _do_tti_repair;

public:
	tolerize_entity_opts();
	~tolerize_entity_opts();
	/**
	* returns a pointer to the tolerant ENTITY in the model replacing possibly_stale_ptr;
	* if the input pointer was not replaced by a tolerant entity, the input pointer is returned.
	* @param possibly_stale_ptr
	* pointer to an edge or vertex present in the model before calling API_tolerize_entity.
	**/
	ENTITY		*remap_ptr(ENTITY *possibly_stale_ptr);
	/**
	* Returns a list of affected entities.  An entity is considered affected if
	* it has been tolerized or has had its tolerance change.
	* This method will remove any existing contents of the list.
	* Warning: the pointers on the affected entities list are likely to be stale.
	* Do not dereference them without using remap_ptr first.
	**/
	void		get_affected_entities(ENTITY_LIST &list );
	/**
	* returns the gap which was present on ent before tolerization.  
	* If ent was not present before tolerization, or
	* if the user has not used set_report_gap_data(TRUE) returns -1.
	**/
	double		get_original_gap(ENTITY *ent);
	/**
	* set option for API to report orginal gaps
	* @param report_gaps
	* report_gaps indicates whether the API will
	* record entity gaps during tolerization for reporting.
	* Default is FALSE because gap reporting may have
	* a performance impact.
	**/
	void		set_report_gaps(logical report_gaps);
	/**
	* returns logical indicating whether API will 
	* record gaps so they can be reported using get_original_gap
	**/
	logical		get_report_gaps();
	/**
	* limits the maximum gap which will be repaired by tolerizing.
	* The default behavior is to tolerize arbitrarily large gaps.
	* @param max_gap
	**/
	void		set_max_gap(double mg);
	/**
	* returns double indicating size of maximum gap fixed by tolerization.
	* -1 indicates default, which is to fix arbitrary gaps by tolerizing.
	**/
	double		get_max_gap();
	/**
	* set option so that API will do TEDGE repair.
	* @param do_repair
	* logical indicating whether API does tedge repair
	**/
	void set_do_tedge_repair(logical do_repair);
	/**
	* returns a logical indicating whether API will do TEDGE repair.
	* The default value is FALSE
	**/
	logical get_do_tedge_repair() const;
	/**
	* set whether API should examine edges and vertices which are
	* already tolerant.
	* @param ignore
	* if ignore==TRUE, API will not check gaps for
	* entities which are already tolerant. Defaults to TRUE.
	* if ignore==FALSE, API will make sure tolerance
	* is larger than the gap of each edge or vertex.
	**/
	void		set_ignore_tentities(logical ignore);
	/**
	* returns logical which indicates whether the
	* API ignores already tolerant edges and vertices.
	**/
	logical		get_ignore_tentities();

	/**
	* @nodoc
	**/
	teo_data *get_data();

private:
	/**
	* @nodoc
	**/
	void reset_output_data(int n);

};
/** @} */
#endif
