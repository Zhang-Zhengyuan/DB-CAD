/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

/**
 * @file load_part.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup SCMAPI
 * \Declared at <load_part.hxx>
 * @{
 */

#if !defined (IGES_LOAD_PART)
#define IGES_LOAD_PART
#include "scmobject.h"
#include "api.hxx"
/**
 * @nodoc
 */
extern ScmObject V_Part_Load_Path;

#ifdef __cplusplus

class PART;
class ENTITY_LIST;
/**
 * @nodoc
 */
 extern "C" ScmObject General_Open_File (ScmObject, int, ScmObject);

/**
 * Loads a file into a <tt>PART</tt>.
 * <br><br>
 * <b>Role:</b> This API loads the entities defined in an open file <tt>fp</tt> into the
 * specified part. The file must be open and positioned to the start of the entity
 * data to be read. All <tt>api_pm</tt> functions should be thought of as requiring the use
 * of the <tt>PART_CONTEXT</tt> class.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param fp
 * file containing entities to load.
 * @param text_mode
 * TRUE (text) or FALSE (binary).
 * @param the_part
 * part in which to load entities.
 * @param with_history
 * TRUE to restore history if it exists in the file.
 * @param new_entities
 * returns list of entities loaded into part.
 **/

outcome api_pm_load_part(FILE* fp, logical text_mode, PART* the_part, logical with_history, ENTITY_LIST &new_entities);
#endif
/** @} */
#endif
