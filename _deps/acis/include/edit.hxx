/* ORIGINAL: acis2.1/ct_husk/edit/edit.hxx */
// $Id: edit.hxx,v 1.7 2000/12/26 18:31:20 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

/*******************************************************************/
// amt 02Nov02 Changing GROUP to SPAGROUP.
/*******************************************************************/

#if !defined(EDIT_EDIT)
#define EDIT_EDIT

#include "dcl_ct.h"
#include "tophdr.hxx"
#include "cthdr.hxx"

class SPAGROUP;
class ct_merge_options;  // For Future Use (so signature won't change.) 
class ct_remove_options;  // For Future Use (so signature won't change.) 

//----------------------------------------------------------------
//----------------------------------------------------------------

DECL_CT logical ct_remove_ct_from_lump(LUMP *);

DECL_CT logical ct_remove_ct_from_body(BODY *);

DECL_CT logical ct_vacate_cell(CELL3D *);

DECL_CT BODY*	ct_copy_cell(CELL *);

DECL_CT SPAGROUP*	ct_make_group(ENTITY_LIST&);

DECL_CT void	ct_return_groups(ENTITY *, ENTITY_LIST&);

DECL_CT void	ct_add_to_group(ENTITY *, SPAGROUP *);

DECL_CT void	ct_remove_from_group(ENTITY *, SPAGROUP *);

DECL_CT void	ct_lose_group(SPAGROUP *);

DECL_CT void	ct_return_ents(SPAGROUP *, ENTITY_LIST& );

DECL_CT void ct_propagate_cface_atts(LUMP *);

DECL_CT BODY * ct_get_entity_body(LUMP *ent);
DECL_CT BODY * ct_get_entity_body(SHELL *ent);
DECL_CT BODY * ct_get_entity_body(FACE *ent);
DECL_CT BODY * ct_get_entity_body(LOOP *ent);
DECL_CT BODY * ct_get_entity_body(COEDGE *ent);
DECL_CT BODY * ct_get_entity_body(EDGE *ent);
DECL_CT BODY * ct_get_entity_body(VERTEX *ent);
DECL_CT BODY * ct_get_entity_body(CELL *ent);
DECL_CT BODY * ct_get_entity_body(CSHELL *ent);
DECL_CT BODY * ct_get_entity_body(CFACE *ent);
DECL_CT BODY * ct_get_entity_body(ENTITY *ent);

DECL_CT logical ct_merge_cells( ENTITY_LIST& cell_list, ct_merge_options* ctmo = NULL );

#endif
