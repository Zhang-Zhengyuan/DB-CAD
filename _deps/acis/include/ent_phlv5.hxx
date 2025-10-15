// COPYRIGHT DASSAULT SYSTEMES 1989
/*******************************************************************/
//   Header for Class ENTITY_PHLV5.
/*******************************************************************/
#if !defined( ENTITY_PHLV5_CLASS )
#define ENTITY_PHLV5_CLASS

#include "entity.hxx"
#include "dcl_phlv5.h"

/**
 * @file ent_phlv5.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup PHLV5API
 *  \brief Declared at <ent_phlv5.hxx>
 *  @{
 */

/**
 * @nodoc
 */
#define ENTITY_PHLV5_LEVEL ( ENTITY_LEVEL + 1)

/**
 * @nodoc
 */
extern DECL_PHLV5 int ENTITY_PHLV5_TYPE;

/**
 * \class ENTITY_PHLV5
 * Defines owning organization for other <tt>PHL</tt> entity classes.
 * <br>
 * <b>Role:</b> All classes representing permanent objects in the <tt>PHL</tt> model (except attributes) derive
 * from the base class <tt>ENTITY_PHLV5</tt>, which is derived from the <tt>ENTITY</tt> class.
 * <br><br>
 * The <tt>ENTITY_PHLV5</tt> class does not represent any specific object within the modeler, but rather allows
 * consistent performance of operations, such as system debugging, change records (bulletin board), roll
 * back, attributes, and model archiving and communication, which are generic member functions of the
 * <tt>ENTITY</tt> class.
 * <br><br>
 * @see ENTITY
 */
MASTER_ENTITY_DECL( ENTITY_PHLV5, PHLV5 );
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

/** @} */
#endif
