/* ORIGINAL: acis2.1/kernutil/errorsys/errorsys.hxx */
/* $Id: errorsys.hxx,v 1.23 2000/12/26 18:48:12 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#include "errorbase.hxx"
#include "err_info.hxx"

#if !defined( ERROR_SYS_OLD )
#define ERROR_SYS_OLD
#include "dcl_kern.h"

class ENTITY;
class ENTITY_LIST;

/**
* @file errorsys.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup KERNAPI
 *
 * @{
 */
/**
 * @nodoc
 */
DECL_KERN void sys_error( err_mess_type err_num, ENTITY *e0, ENTITY *e1 = NULL ); 

/**
 * @nodoc
 */
DECL_KERN void sys_error( err_mess_type err_num, const ENTITY_LIST& err_ents );

/**
 * @nodoc
 */
DECL_KERN void check_safe_entity( ENTITY*& pEnt );

/**
 * @nodoc
 * Add entity to error info; should be called inside a catch block like 
 *
 * EXCEPTION_CATCH_TRUE
 * // do cleanup stuff here
 * if (resignal_no != 0)
 * {
 *      RESIGNAL_ADDITIONAL_ENTITY(ent1);
 *      RESIGNAL_ADDITIONAL_ENTITY(ent2);
 * }
 * EXCEPTION_END
 *
 * ...or...
 *
 * EXCEPTION_CATCH_FALSE
 *      RESIGNAL_ADDITIONAL_ENTITY(ent1);
 *      RESIGNAL_ADDITIONAL_ENTITY(ent2);
 * EXCEPTION_END
 */

#define RESIGNAL_ADDITIONAL_ENTITY(ent) { \
	ENTITY *check_e = ent; \
    check_safe_entity(check_e); \
    if (check_e != NULL) \
    { \
        error_info *info = ERROR_INFO_PTR; \
        if (info == NULL) error_info_base_ptr = ACIS_NEW error_info(0,SPA_OUTCOME_FATAL,check_e); \
            else info->add_entity(check_e); \
    }}

/*! @} */
#endif
