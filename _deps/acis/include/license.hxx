/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef SPATIAL_LICENSING
#define SPATIAL_LICENSING

#include "dcl_base.h"

/**
 * @file license.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISLICENSE
 *
 * @{
 */


class spa_unlock_result;


/**
 * Unlocks licensed Spatial products.
 * <br><br>
 * <b>Role:</b> This routine must be called before any Spatial products can
 * be accessed.  To unlock Spatial products a valid Application License Key
 * and a valid Installation License Key (usually) is required.  The returned
 * <tt>spa_unlock_result</tt> object can be queried to determine if the
 * validation process was successful.
 * <br><br>
 * <b>Note for developing products built on ACIS:&nbsp;&nbsp;</b>
 * <tt>spa_unlock_products</tt> is called by the <i>Unlock Function</i>,
 * <tt>unlock_spatial_products_&lt;ALKIndex&gt;</tt>, obtained from 
 * Spatial's web site. The call to 
 * <tt>unlock_spatial_products_&lt;ALKIndex&gt;</tt> must be made after 
 * ensuring that the Base component is initialized. The necessary sequence of 
 * events is to call <tt>unlock_spatial_products_&lt;ALKIndex&gt;</tt> after 
 * the call to <tt>api_start_modeller</tt> or after an explicit call to 
 * <tt>initialize_base</tt>. The reason for this requirement is that licensing 
 * is a consumer of the memory management system, which is initialized 
 * simultaneously with the Base component. Calling initialization APIs associated 
 * with other libraries such as Deformable Modeling or Warping before calling 
 * the unlock function is possible to do; however, 
 * <tt>unlock_spatial_products_&lt;ALKIndex&gt;</tt> must be called before 
 * attempting to use any modeling functionality.
 * <br><br>
 * This functionality is supported on Windows operating systems only. 
 * <br><br>
 * For additional information on licensing applications containing Spatial products,
 * refer to the Technical Article <i>Application Licensing</i>.
 * <br><br>
 * @param unlock_str
 * application-specific string obtained from Spatial's web site.
 * <br>
 * @see spa_unlock_result
 **/
DECL_BASE spa_unlock_result spa_unlock_products( const char* unlock_str );

/**
 * @nodoc
 */
//DECL_BASE void spa_print_error_text( spa_unlock_result res );

/** @} */

#endif

