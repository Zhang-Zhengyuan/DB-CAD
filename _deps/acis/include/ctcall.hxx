// $Id: ctcall.hxx,v 1.4 2002/01/31 18:37:33 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( CT_CALLBACKS_HXX )
#define CT_CALLBACKS_HXX

class ENTITY;

//#include "kernel/kernapi/api/api.hxx"
#include "dcl_intr.h"
#include "api.hxx"

class insanity_list;

typedef outcome (*check_entity_ct_callback_fn)( const ENTITY *, insanity_list *&, AcisOptions* );

#ifdef THREAD_SAFE_ACIS
extern DECL_INTR safe_function_type< check_entity_ct_callback_fn > check_entity_ct_callback;
#else
extern DECL_INTR check_entity_ct_callback_fn check_entity_ct_callback;
#endif


#endif
