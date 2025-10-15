// $Id: ref_pat.hxx,v 1.6 2002/06/20 22:13:02 jenglund Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef _REF_PAT_HXX
#define _REF_PAT_HXX

#include "api.hxx"
#include "dcl_bool.h"
#include "pattern_enum.hxx"
/**
 * @file ref_pat.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISPATTERNS
 *
 * @{
 */
class ENTITY;
class pattern;
class ENTITY_LIST;

/**
 * Refreshes the elements of a pattern to incorporate changes made to one
 * of them.
 * <br><br>
 * <b>Role</b>: This function refreshes the elements of the pattern <tt>in_pat</tt> to to
 * incorporate changes made to one of them. The entity <tt>in_ent</tt> should be taken from
 * the modified element, while <tt>refresh_list</tt> should contain all pattern entities to
 * be refreshed. It should not include anything from the modified element. Internally,
 * a copy of the pattern is made, and it is the copy that is actually applied to the 
 * entity. By default, <tt>in_ent</tt> is associated with the first pattern element 
 * (index 0), but may be associated with another element by furnishing the associated 
 * zero-based <tt>seed_index</tt>.<br>
 * <br>
 * For cases in which the pattern is applied to a "bump" on a substrate rather than
 * to an autonomous entity, the limits of the bump are automatically computed, but
 * the user may choose to override the default limits by furnishing a list of
 * no_cross_faces.
 * <br><br>
 * For performance reasons, the function does not check the generated pattern of
 * entities for intersection, containment, or compatibility unless the user changes
 * the checking option <tt>check</tt> from its default value. This argument takes the
 * following values:<br><br>
 * <table>
 * <tr>
 * <td valign = "top"><tt>PAT_DONT_CHECK</tt></td>
 * <td valign = "top">do no checking</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>PAT_CHECK_DONT_FIX</tt></td>
 * <td valign = "top">check the patterned entities and roll back in the case of failure</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>PAT_CHECK_AND_FIX</tt></td>
 * <td valign = "top">check the patterned entities.  If only the
 * containment check fails, drop the problem elements from the pattern and re-apply
 * it to the seed.  If either the intersection or compatibility check fails, roll
 * back.</td>
 * </tr>
 * </table>
 * <br><br>
 * <b>Errors:</b> An entity type not supporting patterns is specified, or (if <tt>check</tt> is
 * TRUE) the pattern has problems with intersection, containment, or compatibility.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param in_ent
 * seed entity
 * @param refresh_list
 * pattern entities to refresh
 * @param in_pat
 * pattern to apply
 * @param seed_index
 * zero-based pattern index
 * @param no_cross_faces
 * list of faces that bound seed
 * @param check
 * checking option
 * @param ao
 * ACIS options such as version and journal
 **/
DECL_BOOL outcome api_pattern_update_entities(
    ENTITY*         in_ent,
    ENTITY_LIST&    refresh_list,
    pattern*        in_pat,
    int             seed_index      = 0,
    ENTITY_LIST&    no_cross_faces  = *(ENTITY_LIST*)NULL_REF,
    PAT_CHECK_TYPE  check           = PAT_DONT_CHECK,
	AcisOptions*    ao              = NULL);


/**
 * Applies a pattern to a seed element represented by the given entity.
 * <br><br>
 * <b>Role:</b> Applies the pattern <tt>in_pat</tt> to the seed entity <tt>in_ent</tt> or list of
 * seed entities <tt>in_ents</tt>. Internally, a copy of the pattern is made, and it is the
 * copy that is actually applied to the entity. By default, the seed entity is
 * associated with the first pattern element (index 0), but may be associated
 * with another element by furnishing the associated zero-based <tt>seed_index</tt>.
 * <br><br>
 * For cases in which the pattern is applied to a "bump" on a substrate rather than
 * to an autonomous entity, the limits of the bump are automatically computed, but
 * the user may choose to override the default limits by furnishing a list of
 * <tt>no_cross_faces</tt>.
 * <br><br>
 * For performance reasons, the function does not check the generated pattern of
 * entities for intersection, containment, or compatibility unless the user changes
 * the checking option <tt>check</tt> from its default value. This argument takes the
 * following values:
 * <br><br>
 * <table>
 * <tr>
 * <td valign = "top"><tt>PAT_DONT_CHECK</tt></td>
 * <td valign = "top">do no checking</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>PAT_CHECK_DONT_FIX</tt></td>
 * <td valign = "top">check the patterned entities and roll back in the case of failure</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>PAT_CHECK_AND_FIX</tt></td>
 * <td valign = "top">check the patterned entities.  If only the
 * containment check fails, drop the problem elements from the pattern and re-apply
 * it to the seed.  If either the intersection or compatibility check fails, roll
 * back.</td>
 * </tr>
 * </table>
 * <br>
 * <b>Errors:</b> An entity type not supporting patterns is specified, or (if checking is
 * enabled through the check option) the pattern has problems with intersection,
 * containment or compatibility.
 * <br><br>
 * <b>Limitations:</b> Prior to R11, an attempt to re-pattern an altered seed entity with a
 * pattern identical to the original one would result in an invalid body, as the API
 * would create new pattern elements intersecting with the old ones. The API would
 * detect this and fail if <tt>check</tt> was set to <tt>PAT_CHECK_DONT_FIX</tt>.
 * <br><br>
 * Beginning with R11, the API attempts to detect if the new pattern is identical with a former
 * pattern on the seed, if any; if successful in this, it merges the new pattern
 * with the old by undoing the old pattern-generated elements and regenerating them
 * based upon the new pattern.  This behavior is obviously of use mainly when the seed
 * has been altered and it is desired to refresh the other pattern elements based upon
 * this alteration.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param in_ent
 * (in/out) Seed entity.
 * @param in_pat
 * (in) Pattern to apply.
 * @param seed_index
 * (in) Pattern index for seed entity.
 * @param no_cross_faces
 * (in) List of faces that bound the seed.
 * @param check
 * (in) Desired checking option.
 * @param ao
 * (in) ACIS options such as version and journal.
 **/
DECL_BOOL outcome api_pattern_apply_to_entity(
    ENTITY*         in_ent,
    pattern*        in_pat,
    int             seed_index      = 0,
    ENTITY_LIST&    no_cross_faces  = *(ENTITY_LIST*)NULL_REF,
    PAT_CHECK_TYPE  check           = PAT_DONT_CHECK,
    AcisOptions*    ao              = NULL);

/**
 * Applies a pattern to seed element represented by the entities in the given list.
 * <br><br>
 * <b>Role:</b> Applies the pattern <tt>in_pat</tt> to the seed entity <tt>in_ent</tt> or list of
 * seed entities <tt>in_ents</tt>. Internally, a copy of the pattern is made, and it is the
 * copy that is actually applied to the entity. By default, the seed entity is
 * associated with the first pattern element (index 0), but may be associated
 * with another element by furnishing the associated zero-based <tt>seed_index</tt>.
 * <br><br>
 * For cases in which the pattern is applied to a "bump" on a substrate rather than
 * to an autonomous entity, the limits of the bump are automatically computed, but
 * the user may choose to override the default limits by furnishing a list of
 * <tt>no_cross_faces</tt>.
 * <br><br>
 * For performance reasons, the function does not check the generated pattern of
 * entities for intersection, containment, or compatibility unless the user changes
 * the checking option <tt>check</tt> from its default value. This argument takes the
 * following values:
 * <br><br>
 * <table>
 * <tr>
 * <td valign = "top"><tt>PAT_DONT_CHECK</tt></td>
 * <td valign = "top">do no checking</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>PAT_CHECK_DONT_FIX</tt></td>
 * <td valign = "top">check the patterned entities and roll back in the case of failure</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>PAT_CHECK_AND_FIX</tt></td>
 * <td valign = "top">check the patterned entities.  If only the
 * containment check fails, drop the problem elements from the pattern and re-apply
 * it to the seed.  If either the intersection or compatibility check fails, roll
 * back.</td>
 * </tr>
 * </table>
 * <br>
 * <b>Errors:</b> An entity type not supporting patterns is specified, or (if checking is
 * enabled through the check option) the pattern has problems with intersection,
 * containment or compatibility.
 * <br><br>
 * <b>Limitations:</b> Prior to R11, an attempt to re-pattern an altered seed entity with a
 * pattern identical to the original one would result in an invalid body, as the API
 * would create new pattern elements intersecting with the old ones. The API would
 * detect this and fail if <tt>check</tt> was set to <tt>PAT_CHECK_DONT_FIX</tt>.
 * <br><br>
 * Beginning with R11, the API attempts to detect if the new pattern is identical with a former
 * pattern on the seed, if any; if successful in this, it merges the new pattern
 * with the old by undoing the old pattern-generated elements and regenerating them
 * based upon the new pattern.  This behavior is obviously of use mainly when the seed
 * has been altered and it is desired to refresh the other pattern elements based upon
 * this alteration.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param in_ents
 * (in/out) List of seed entities.
 * @param in_pat
 * (in) Pattern to apply.
 * @param seed_index
 * (in) Pattern index for seed entity.
 * @param no_cross_faces
 * (in) List of faces that bound the seed.
 * @param check
 * (in) Desired checking option.
 * @param ao
 * (in) ACIS options such as version and journal.
 **/
DECL_BOOL outcome api_pattern_apply_to_entity(
    ENTITY_LIST     in_ents,
    pattern*        in_pat,
    int             seed_index      = 0,
    ENTITY_LIST&    no_cross_faces  = *(ENTITY_LIST*)NULL_REF,
    PAT_CHECK_TYPE  check           = PAT_DONT_CHECK,
    AcisOptions*    ao              = NULL);

/**
 * Applies a pattern to a seed element specified by the given list of faces.
 * <br><br>
 * <b>Role:</b> Applies the pattern <tt>in_pat</tt> to the seed element represented
 * by the list of faces <tt>in_faces</tt>. Internally, a copy of the pattern is made, and it is the
 * copy that is actually applied to the seed entities. By default, the seed entities are
 * associated with the first pattern element (index 0), but may be associated
 * with another element by furnishing the associated zero-based <tt>seed_index</tt>.
 * <br><br>
 * This API may be successful in situations where @href api_pattern_apply_to_entity
 * is not, i.e., when the seed element cannot be identified from a single face, or
 * when the feature being patterned runs along one or more edges (<i>rail edges</i>), or
 * astride one or more faces (<i>rail faces</i>).  It comes with limitations, however:
 * <br><br>
 * <dl>
 * <dd>
 * 1. Only linear and circular patterns, without reflection, are supported when the
 *    pattern runs along rail edges or faces;
 * </dd>
 * <dd>
 * 2. The transforms defining such patterns must be ordered in such a way that they
 *    represent <i>monotonic</i> translations or rotations.
 * </dd>
 * <dd>
 * 3. Rail edges and faces must be uninterrupted within the extent of the pattern.
 *    For example, it is not possible to pattern along colinear rail edges that are
 *    joined by a vertex, even when the edges are consistent with the applied pattern.
 * </dd>
 * </dl>
 * <br>
 * It is recommended that the user adopt this API rather than
 * @href api_pattern_apply_to_entity in cases where the full set of faces defining the
 * seed pattern element are available.  Internally, this API examines the input
 * and calls @href api_pattern_apply_to_entity on its own when doing so would more
 * likely be successful.
 * <br><br>
 * For performance reasons, the function does not check the generated pattern of
 * entities for intersection, containment, or compatibility unless the user changes
 * the checking option <tt>check</tt> from its default value. This argument takes the
 * following values:
 * <br><br>
 * <table>
 * <tr>
 * <td valign = "top"><tt>PAT_DONT_CHECK</tt></td>
 * <td valign = "top">do no checking</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>PAT_CHECK_DONT_FIX</tt></td>
 * <td valign = "top">check the patterned entities and roll back in the case of failure</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>PAT_CHECK_AND_FIX</tt></td>
 * <td valign = "top">check the patterned entities.  If only the
 * containment check fails, drop the problem elements from the pattern and re-apply
 * it to the seed.  If either the intersection or compatibility check fails, roll
 * back.</td>
 * </tr>
 * </table>
 * <br>
 * <b>Errors:</b> The input list contains something other than a face, or (if checking is
 * enabled through the check option) the pattern has problems with intersection,
 * containment or compatibility.  The pattern is of an unsupported type.  One or
 * more rail edges or faces is interrupted.
 * <br><br>
 * <b>Limitations:</b> Only linear and circular, non-reflecting patterns are supported
 * when the pattern runs along rail edges or faces.  The transforms defining such patterns
 * much be in monotonic order.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param in_faces
 * List of faces defining the seed pattern element.
 * @param in_pat
 * Pattern to apply.
 * @param seed_index
 * Pattern index associated with the seed element.
 * @param check
 * Desired checking option.
 * @param ao
 * ACIS options such as version and journal.
 **/
DECL_BOOL outcome api_pattern_apply_to_faces(
    ENTITY_LIST     in_faces,
    pattern*        in_pat,
    int             seed_index      = 0,
    PAT_CHECK_TYPE  check           = PAT_DONT_CHECK,
    AcisOptions*    ao              = NULL);

/** @} */

#endif

