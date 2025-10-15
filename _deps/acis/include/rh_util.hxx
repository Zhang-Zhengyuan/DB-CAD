/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//    Define rendering utility funtions
#ifndef rh_util_hxx
#define rh_util_hxx
#include "api.hxx"
#include "dcl_rb.h"
#include "logical.h"
/**
 * @file rh_util.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup RENDERBASEAPI
 *
 * @{
 */

class outcome;
class ENTITY_LIST;
class PART;
class ENTITY;
class view3d;

/**
 * @nodoc
 */
DECL_RB outcome set_clipping_planes( ENTITY_LIST& );

/**
 * Determines if a character string represents a legal background type.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * @param type
 * entity.
 **/
 DECL_RB logical is_Background_Type( const char *type );

/**
 * Determines if a character string represents a legal foreground type.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param type
 * string.
 **/
 DECL_RB logical is_Foreground_Type( const char *type );

/**
 * Determines if a character string represents a legal color type.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param type
 * string.
 **/
 DECL_RB logical is_Color_Type( const char *type );

/**
 * Determines if a character string represents a legal displacement type.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * @param type
 * string.
 **/
 DECL_RB logical is_Displace_Type( const char *type );

/**
 * Determines if a character string represents a legal light type.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param type
 * string.
 **/
DECL_RB logical is_Light_Type( const char *type );

/**
 * Determines if a character string represents a legal reflection type.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param type
 * string.
 **/
DECL_RB logical is_Reflect_Type( const char *type );

/**
 * Determines if a character string represents a legal texture space type.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param type
 * string.
 **/
DECL_RB logical is_Texture_Space_Type( const char *type );

/**
 * Determines if a character string represents a legal transparency type.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param type
 * entity.
 **/
DECL_RB logical is_Transp_Type( const char *type );

/** @} */
#endif

