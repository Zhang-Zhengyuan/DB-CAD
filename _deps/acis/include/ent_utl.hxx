/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//    Utility functions for ENTITY creation
#ifndef ent_utl_hxx
#define ent_utl_hxx
class ENTITY;
class outcome;
#include "logical.h"
#include "api.hxx"
/**
 * @file ent_utl.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup SCMAPI
 * \Declared at <ent_utl.hxx>
 * @{
 */
/**
 * Prepares for the definition of a new <tt>ENTITY</tt>.
 * <br><br>
 * <b>Role:</b> Use this routine in conjunction with <tt>start_entity_creation</tt> to
 * bracket modifications to entities. <tt>start_entity_creation</tt> and <tt>end_entity_creation</tt>
 * can be nested.
 * <br><br>
 * Using <tt>start_entity_creation</tt>, <tt>end_entity_creation</tt>, <tt>start_entity_modification</tt> and
 * <tt>end_entity_modification</tt> or any associated wrapper functions is not recommended.
 * <br><br>
 * <b>Effect:</b> System routine
 **/
 void start_entity_creation(void);
/**
 * Mark end of entity creation.
 * The difference between <tt>end_entity_creation</tt> and <tt>end_entity_creation2</tt> is
 * that <tt>end_entity_creation</tt> will raise an exception (it calls sys_error)
 * if the outcome is not OK, while <tt>end_entity_creation2</tt> will just return.
 * The optional <i>displayable</i> argument controls whether or not the
 * ENTITY is displayed. If it is TRUE, then the display is controlled
 * by the auto display flag setting for the <tt>PART_CONTEXT</tt> to which the
 * ENTITY is added. If it is FALSE, it will not be displayed.
 */
 void end_entity_creation(ENTITY*, outcome, logical displayable=TRUE);
/**
 * Mark end of entity creation.
 * The difference between <tt>end_entity_creation</tt> and <tt>end_entity_creation2</tt> is
 * that <tt>end_entity_creation</tt> will raise an exception (it calls sys_error)
 * if the outcome is not OK, while <tt>end_entity_creation2</tt> will just return.
 * The optional <i>displayable</i> argument controls whether or not the
 * ENTITY is displayed. If it is TRUE, then the display is controled
 * by the auto display flag setting for the <tt>PART_CONTEXT</tt> to which the
 * ENTITY is added. If it is FALSE, it will not be displayed.
 */
 void end_entity_creation2(ENTITY*, outcome, logical displayable=TRUE);
 /**
 * Mark end of entity creation.
 * The difference between <tt>end_entity_creation</tt> and <tt>end_entity_creation2</tt> is
 * that <tt>end_entity_creation2</tt> will register last outcome and return.
 * The optional <i>displayable</i> argument controls whether or not the
 * ENTITY is displayed. If it is TRUE, then the display is controled
 * by the auto display flag setting for the <tt>PART_CONTEXT</tt> to which the
 * ENTITY is added. If it is FALSE, it will not be displayed.
 */
 void end_entity_creation2(ENTITY_LIST const&, outcome const&, logical displayable = TRUE);
/**
 * Prepares for <tt>ENTITY</tt> modification.
 * <br><br>
 * <b>Role:</b> Use this routine in conjunction with <tt>end_entity_modification</tt> to
 * bracket modifications to entities. <tt>start_entity_modification</tt> and
 * <tt>end_entity_modification</tt> can be nested.
 * <br><br>
 * Using <tt>start_entity_creation</tt>, <tt>end_entity_creation</tt>, <tt>start_entity_modification</tt>,
 * and <tt>end_entity_modification</tt> or any associated wrapper functions is not recommended.
 * <br><br>
 * <b>Effect:</b> System routine
 **/
 void start_entity_modification(void);
/**
 * Mark end of entity modification.
 * The difference between <tt>end_entity_modification</tt> and
 * <tt>end_entity_modification2</tt> is that <tt>end_entity_modification</tt> will raise
 * an exception (it calls sys_error) if the outcome is not OK, while
 * <tt>end_entity_modification2</tt> will just return.
 * The optional <i>redisplay</i> argument controls whether or not the display
 * should be updated.
 */
 void end_entity_modification(ENTITY*, outcome, logical redisplay=TRUE);

/**
* Mark end of entity modification.
* The difference between <tt>end_entity_modification< / tt> and
* <tt>end_entity_modification2< / tt> is that <tt>end_entity_modification< / tt> will raise
* an exception(it calls sys_error) if the outcome is not OK, while
* <tt>end_entity_modification2< / tt> will just return.
* The optional <i>redisplay< / i> argument controls whether or not the display
* should be updated.
*/
void end_entity_modification(ENTITY_LIST const&, outcome const&, logical redisplay = TRUE);

/**
 * Mark end of entity modification.
 * The difference between <tt>end_entity_modification</tt> and
 * <tt>end_entity_modification2</tt> is that <tt>end_entity_modification</tt> will raise
 * an exception (it calls sys_error) if the outcome is not OK, while
 * <tt>end_entity_modification2</tt> will just return.
 * The optional <i>redisplay</i> argument controls whether or not the display
 * should be updated.
 */
 void end_entity_modification2(ENTITY*, outcome, logical redisplay=TRUE);
/**
 * @nodoc
 * <tt>record_entity</tt> is used when you create a new ENTITY. Its main purpose 
 * is to add the ENTITY to the active PART_CONTEXT, but it first finds
 * the active PART_CONTEXT and creates one if needed. The <i>displayable</i>
 * argument is just passed to PART_CONTEXT::add. If it is TRUE, then
 * the ENTITY display is controlled by the auto display flag of the
 * PART_CONTEXT. If it is FALSE, then the ENTITY is added to the
 * PART_CONTEXT but is not displayed.
 */
 void record_entity(ENTITY*, logical displayable=TRUE);
 /**
 * @nodoc
 * Like <tt>record_entity</tt> but for use with multiple entities.  <tt>record_entity</tt>
 * redraws the scene every time it is called.  When adding lots of entities this is wastefully slow.
 **/
 void record_entities(ENTITY_LIST const& ents, logical displayable = TRUE);
/**
 * <tt>update_entity</tt> is used to update an ENTITY when it was been modified.
 * It invokes the update method of the PART_CONTEXT that the ENTITY
 * belongs to. The <i>redisplay</i> argument controls whether or not
 * the display of the ENTITY is updated. It is passed to
 * <tt>PART_CONTEXT::update</tt>.
 */
 void update_entity(ENTITY*, logical redisplay = TRUE);
 /**
 * @nodoc
 * Like <tt>update_entity</tt> but for use with multiple entities.  <tt>update_entity</tt>
 * redraws the scene every time it is called.  When updating lots of entities this is wastefully slow.
 **/
 void update_entities(ENTITY_LIST const& ents, logical redisplay = TRUE);
 /**
 * @nodoc
 */
 void record_bgraph(bool redisplay = true);
 /** @} */

#endif
