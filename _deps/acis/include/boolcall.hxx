// $Id: boolcall.hxx,v 1.11 2002/01/07 23:00:46 rocon Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Emacs indenting style choice
// Indent style: D3

#if !defined( BOOL_CALLBACKS_HXX )
#define BOOL_CALLBACKS_HXX

class ENTITY;
class BODY;
class VERTEX;
class ENTITY_LIST;
class AcisOptions;
class WIRE;
class insanity_list;

//class outcome;
#include "logical.h"
#include "safe.hxx"
#include "api.hxx"
#include "dcl_intr.h"

typedef BODY * (*make_error_body_callback_fn)( ENTITY_LIST *insane_ents );
typedef void (*clean_error_list_callback_fn)( ENTITY_LIST *insane_ents );
typedef outcome (*check_entity_ff_ints_callback_fn)( const ENTITY *, ENTITY_LIST *, logical &, FILE *, insanity_list *&, AcisOptions* );
typedef outcome (*check_wire_ee_ints_callback_fn)(const ENTITY *, ENTITY_LIST *, logical &, FILE *, insanity_list *&, AcisOptions *);
typedef void (*repair_wire_vertex_callback_fn)( VERTEX *vert );

// TSAFIX: global function pointers
#ifdef THREAD_SAFE_ACIS

extern DECL_INTR safe_function_type< make_error_body_callback_fn > make_error_body_callback;
extern DECL_INTR safe_function_type< clean_error_list_callback_fn > clean_error_list_callback;
extern DECL_INTR safe_function_type< check_entity_ff_ints_callback_fn > check_entity_ff_ints_callback;
extern DECL_INTR safe_function_type< check_wire_ee_ints_callback_fn > check_wire_ee_ints_callback;
extern DECL_INTR safe_function_type< repair_wire_vertex_callback_fn > repair_wire_vertex_callback;

#else

extern DECL_INTR make_error_body_callback_fn make_error_body_callback;
extern DECL_INTR clean_error_list_callback_fn clean_error_list_callback;
extern DECL_INTR check_entity_ff_ints_callback_fn check_entity_ff_ints_callback;
extern DECL_INTR check_wire_ee_ints_callback_fn check_wire_ee_ints_callback;
extern DECL_INTR repair_wire_vertex_callback_fn repair_wire_vertex_callback;

#endif

#endif
