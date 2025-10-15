// $Id: ga_api.hxx,v 1.9 2002/08/09 17:19:21 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for ga_husk api routines.

#if !defined( GA_HUSK_API_ROUTINES )
#define GA_HUSK_API_ROUTINES

#include "dcl_ga.h"
#include "api.hxx"
#include "at_name.hxx"
/**
 * \defgroup GAAPI Generic Attributes
 * \ingroup ACISATTRIBUTES
 */
/**
* @file ga_api.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup GAAPI
 *
 * @{
 */

/**
* Initializes the generic attributes library.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
*/
DECL_GA outcome api_initialize_generic_attributes();

/**
* Terminates the generic attributes library.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
*/
DECL_GA outcome api_terminate_generic_attributes();

/**
* Creates an ATTRIB_GEN_NAME owned by the given entity.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
* @param ent
* owning entity.
* @param name
* attribute name.
* @param split_opt
* The attribute's behavior when its owner is split (default: <tt>SplitKeep</tt>).
* @param merge_opt
* The attribute's behavior when its owner is merged (default: <tt>MergeKeepKept</tt>).
* @param trans_opt
* The attribute's behavior when its owner is transformed (default: <tt>TransIgnore</tt>).
* @param copy_opt
* The attribute's behavior when its owner is copied (default: <tt>CopyCopy</tt>).
* @param ao
* acis options.
* <br><br>
* @see ATTRIB_GEN_NAME, split_action, merge_action, trans_action, copy_action
**/
DECL_GA outcome api_add_generic_named_attribute(
    ENTITY* ent,
    const char* name,
	split_action split_opt =SplitKeep,
	merge_action merge_opt =MergeKeepKept,
	trans_action trans_opt =TransIgnore,
	copy_action copy_opt =CopyCopy,
    AcisOptions *ao = NULL);

/**
* Creates an ATTRIB_GEN_INTEGER owned by the given entity.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
* @param ent
* owning entity.
* @param name
* attribute name.
* @param value
* attribute value.
* @param split_opt
* The attribute's behavior when its owner is split (default: <tt>SplitKeep</tt>).
* @param merge_opt
* The attribute's behavior when its owner is merged (default: <tt>MergeKeepKept</tt>).
* @param trans_opt
* The attribute's behavior when its owner is transformed (default: <tt>TransIgnore</tt>).
* @param copy_opt
* The attribute's behavior when its owner is copied (default: <tt>CopyCopy</tt>).
* @param ao
* acis options.
* <br><br>
* @see ATTRIB_GEN_INTEGER, split_action, merge_action, trans_action, copy_action
**/
DECL_GA outcome api_add_generic_named_attribute(
    ENTITY* ent,
    const char* name,
	int value,
	split_action split_opt =SplitKeep,
	merge_action merge_opt =MergeKeepKept,
	trans_action trans_opt =TransIgnore,
	copy_action copy_opt =CopyCopy,
    AcisOptions *ao = NULL);

/**
* Creates an ATTRIB_GEN_REAL owned by the given entity.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
* @param ent
* owning entity.
* @param name
* attribute name.
* @param value
* attribute value.
* @param split_opt
* The attribute's behavior when its owner is split (default: <tt>SplitKeep</tt>).
* @param merge_opt
* The attribute's behavior when its owner is merged (default: <tt>MergeKeepKept</tt>).
* @param trans_opt
* The attribute's behavior when its owner is transformed (default: <tt>TransIgnore</tt>).
* @param copy_opt
* The attribute's behavior when its owner is copied (default: <tt>CopyCopy</tt>).
* @param ao
* acis options.
* <br><br>
* @see ATTRIB_GEN_REAL, split_action, merge_action, trans_action, copy_action
**/
DECL_GA outcome api_add_generic_named_attribute(
    ENTITY* ent,
    const char* name,
	double value,
	split_action split_opt =SplitKeep,
	merge_action merge_opt =MergeKeepKept,
	trans_action trans_opt =TransIgnore,
	copy_action copy_opt =CopyCopy,
    AcisOptions *ao = NULL);

/**
* Creates an ATTRIB_GEN_STRING owned by the given entity.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
* @param ent
* owning entity.
* @param name
* attribute name.
* @param value
* attribute value.
* @param split_opt
* The attribute's behavior when its owner is split (default: <tt>SplitKeep</tt>).
* @param merge_opt
* The attribute's behavior when its owner is merged (default: <tt>MergeKeepKept</tt>).
* @param trans_opt
* The attribute's behavior when its owner is transformed (default: <tt>TransIgnore</tt>).
* @param copy_opt
* The attribute's behavior when its owner is copied (default: <tt>CopyCopy</tt>).
* @param ao
* acis options.
* <br><br>
* @see ATTRIB_GEN_STRING, split_action, merge_action, trans_action, copy_action
**/
DECL_GA outcome api_add_generic_named_attribute(
    ENTITY* ent,
    const char* name,
	const char *value,
	split_action split_opt = SplitKeep,
	merge_action merge_opt = MergeKeepKept,
	trans_action trans_opt =TransIgnore,
	copy_action copy_opt =CopyCopy,
    AcisOptions *ao = NULL);

/**
* Creates an ATTRIB_GEN_WSTRING owned by the given entity.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
* @param ent
* owning entity.
* @param name
* attribute name.
* @param value
* attribute value which is a wide character string
* @param split_opt
* The attribute's behavior when its owner is split (default: <tt>SplitKeep</tt>).
* @param merge_opt
* The attribute's behavior when its owner is merged (default: <tt>MergeKeepKept</tt>).
* @param trans_opt
* The attribute's behavior when its owner is transformed (default: <tt>TransIgnore</tt>).
* @param copy_opt
* The attribute's behavior when its owner is copied (default: <tt>CopyCopy</tt>).
* @param ao
* acis options.
* <br><br>
* @see ATTRIB_GEN_WSTRING, split_action, merge_action, trans_action, copy_action
**/
DECL_GA outcome api_add_generic_named_attribute(
    ENTITY* ent,
    const char* name,
	const SPAWCHAR_T *value,
	split_action split_opt = SplitKeep,
	merge_action merge_opt = MergeKeepKept,
	trans_action trans_opt =TransIgnore,
	copy_action copy_opt =CopyCopy,
    AcisOptions *ao = NULL);



/**
* Creates an ATTRIB_GEN_POSITION owned by the given entity.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
* @param ent
* owning entity.
* @param name
* attribute name.
* @param value
* attribute value.
* @param split_opt
* The attribute's behavior when its owner is split (default: <tt>SplitKeep</tt>).
* @param merge_opt
* The attribute's behavior when its owner is merged (default: <tt>MergeKeepKept</tt>).
* @param trans_opt
* The attribute's behavior when its owner is transformed (default: <tt>TransApply</tt>).
* @param copy_opt
* The attribute's behavior when its owner is copied (default: <tt>CopyCopy</tt>).
* @param ao
* acis options.
* <br><br>
* @see ATTRIB_GEN_POSITION, split_action, merge_action, trans_action, copy_action
**/
DECL_GA outcome api_add_generic_named_attribute(
    ENTITY* ent,
    const char* name,
	SPAposition &value,
	split_action split_opt = SplitKeep,
	merge_action merge_opt =MergeKeepKept,
	trans_action trans_opt =TransApply,
	copy_action copy_opt =CopyCopy,
    AcisOptions *ao = NULL);

/**
* Creates an ATTRIB_GEN_VECTOR owned by the given entity.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
* @param ent
* owning entity.
* @param name
* attribute name.
* @param value
* attribute value.
* @param split_opt
* The attribute's behavior when its owner is split (default: <tt>SplitKeep</tt>).
* @param merge_opt
* The attribute's behavior when its owner is merged (default: <tt>MergeKeepKept</tt>).
* @param trans_opt
* The attribute's behavior when its owner is transformed (default: <tt>TransApply</tt>).
* @param copy_opt
* The attribute's behavior when its owner is copied (default: <tt>CopyCopy</tt>).
* @param ao
* acis options.
* <br><br>
* @see ATTRIB_GEN_VECTOR, split_action, merge_action, trans_action, copy_action
**/
DECL_GA outcome api_add_generic_named_attribute(
    ENTITY* ent,
    const char* name,
	SPAvector &value,
	split_action split_opt =SplitKeep,
	merge_action merge_opt =MergeKeepKept,
	trans_action trans_opt =TransApply,
	copy_action copy_opt =CopyCopy,
    AcisOptions *ao = NULL);

/**
* Creates an ATTRIB_GEN_ENTITY owned by the given entity.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
* @param ent
* owning entity.
* @param name
* attribute name.
* @param value
* attribute value.
* @param split_opt
* The attribute's behavior when its owner is split (default: <tt>SplitKeep</tt>).
* @param merge_opt
* The attribute's behavior when its owner is merged (default: <tt>MergeKeepKept</tt>).
* @param trans_opt
* The attribute's behavior when its owner is transformed (default: <tt>TransApply</tt>).
* @param copy_opt
* The attribute's behavior when its owner is copied (default: <tt>CopyCopy</tt>).
* @param ao
* acis options.
* <br><br>
* @see ATTRIB_GEN_ENTITY, split_action, merge_action, trans_action, copy_action
**/
DECL_GA outcome api_add_generic_named_attribute(
    ENTITY* ent,
    const char* name,
	ENTITY *value,
	split_action split_opt = SplitKeep,
	merge_action merge_opt = MergeKeepKept,
	trans_action trans_opt = TransApply,
	copy_action copy_opt = CopyCopy,
    AcisOptions *ao = NULL);

/**
* Finds the first named attribute with the given name on the given entity.
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
* @param ent
* owning entity.
* @param name
* attribute name.
* @param ret_att
* returned attribute.
* @param ao
* acis options.
**/
DECL_GA outcome api_find_named_attribute(
    ENTITY*           ent,
    const char*       name,
	ATTRIB_GEN_NAME *&ret_att,
	AcisOptions *ao = NULL);

/**
* Removes a named attribute attached to an entity.
* <br>
* <b>Role:</b> This API function searches for a named attribute with the given name 
* on the given entity and, if one is found, it's <tt>lose</tt> method is called.
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
* @param ent
* owning entity.
* @param name
* attribute name.
* @param ao
* acis options.
**/
DECL_GA outcome api_remove_generic_named_attribute(
	ENTITY*           ent,
    const char*       name,
	AcisOptions *ao = NULL);
/** @} */
#endif
