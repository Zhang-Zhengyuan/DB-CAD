/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Modification history
// --------------------
// 18-Oct-2002 dal: Add ct_reverse_face.
// 08-Feb-2012 let: Added ct_find_cshell_cfaces.
// 10-Feb-2012 let: Added ct_attach_cfaces_to_faces and changed 
//                  signature of ct_recompute_cells.
//             

#if !defined(COMPUTE_COMPUTE)
#define COMPUTE_COMPUTE

#include "dcl_ct.h"
#include "tophdr.hxx"
#include "cthdr.hxx"

class ATTRIB_FACECFACE;
class ATTRIB_CELL;
class ENTITY_LIST;

//----------------------------------------------------------------

DECL_CT logical 
ct_attach_ct_to_lump(LUMP *);

DECL_CT logical
ct_attach_ct_to_body(BODY *);

DECL_CT void
ct_update_cells();

DECL_CT void
ct_attach_cfaces_to_shell(SHELL *, ENTITY_LIST&);

DECL_CT void
ct_attach_cfaces_to_faces(ENTITY_LIST&, ENTITY_LIST&);

DECL_CT CELL3D*
ct_sort_cshells_into_cells(CSHELL *, LUMP *);

DECL_CT ATTRIB_FACECFACE*
ct_cface_attrib(FACE *);

DECL_CT logical
ct_face_not_in_cshell_twice(FACE *);

DECL_CT logical
ct_is_sheet(CSHELL *);

DECL_CT CFACE*
ct_cface_from_sense(FACE *, REVBIT);

DECL_CT CFACE*
ct_find_associated_cface(FACE *,ENTITY *);

DECL_CT void
ct_propagate_update_data(ENTITY *);

DECL_CT void
ct_recompute_cells(LUMP *, ENTITY_LIST& );

DECL_CT CELL2D*
ct_find_2d_cells(SHELL *, CELL2D *, ENTITY_LIST& );

DECL_CT CSHELL*
ct_compute_cshells_on_shell(SHELL *, CSHELL *, ENTITY_LIST& );

DECL_CT void
ct_invalidate_cell_data(FACE *);

DECL_CT void
ct_find_cshell_cfaces(CFACE *, ENTITY_LIST&, ENTITY_LIST&, logical allow_dsbo_faces = FALSE);

DECL_CT CFACE*
ct_close_cshell_on_cface(CFACE *, ENTITY_LIST&);

DECL_CT CFACE*
ct_close_cell2d_on_face(FACE *, ENTITY_LIST&);

DECL_CT void
ct_remove_cface(FACE *, REVBIT);

DECL_CT void
ct_reverse_face(FACE *);

DECL_CT void
ct_unhook_cface(CFACE *);

DECL_CT CFACE *
ct_unhook_cfaces( ENTITY_LIST const& given_cfaces, logical keep_first = TRUE );

DECL_CT void
ct_unhook_cshell(CSHELL *);

DECL_CT void
ct_unhook_cell(CELL *);

DECL_CT void
ct_plant_void_cshells( ATTRIB_CELL *, CSHELL * );

DECL_CT CSHELL*
ct_plant_peripheral_cshells( ATTRIB_CELL *, CSHELL * );

DECL_CT void
ct_sort_peripheral_cshells( ATTRIB_CELL * );

DECL_CT void
ct_add_new_cfaces_at_nm_vert( VERTEX *, ENTITY_LIST&, ENTITY_LIST&);

DECL_CT CFACE*
ct_find_closest_inward( COEDGE *, CFACE *, logical allow_dsbo_faces = FALSE );

DECL_CT void
ct_expand_body( BODY *);

DECL_CT void
ct_expand_lump( LUMP *);

DECL_CT void
ct_expand_attrib_cell( ATTRIB_CELL *);

DECL_CT void
ct_expand_supercell( SUPERCELL *);

DECL_CT void
ct_flatten_body( BODY *);

DECL_CT void
ct_flatten_lump( LUMP *);

DECL_CT void
ct_flatten_attrib_cell( ATTRIB_CELL *);

DECL_CT void
ct_flatten_supercell( SUPERCELL *);

#endif
