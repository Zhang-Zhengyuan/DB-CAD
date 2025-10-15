/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//----------------------------------------------------------------------
// purpose---
//    Declare procedures for working with component_handle objects in scheme
//
//----------------------------------------------------------------------
#ifndef comp_handle_typ_hxx
#define comp_handle_typ_hxx

//======================================================================
#include "scheme.hxx"
#include "logical.h"
#include "vlists.hxx"

class component_handle;
class component_handle_list;

//======================================================================
logical             is_Scm_Component_Handle( ScmObject );
logical             is_Scm_Component_Handle_alive( ScmObject );
logical             is_Scm_Component_Handle_List( ScmObject );

ScmObject           make_Scm_Component_Handle( component_handle * );
ScmObject           make_Scm_Component_Handle_List( int npts, component_handle **chs );

component_handle*   get_Scm_Component_Handle( ScmObject );
void                get_Scm_Component_Handle_List( ScmObject, component_handle_list & );

void                delete_GC_Component_Handle_Array();

component_handle*   scm_lookup_component_handle( int portion_idx, int owner_idx);
int                 scm_lookup_component_handle(component_handle *comp_hnd, int owner_idx);

//======================================================================
// Container class. Application specific to SchemeExt. This is a map between
// integer values and sets of void pointers. Used a base class for several
// assembly handle containers.
//
class ContainerTable : public ACIS_OBJECT
{
public:
    ContainerTable() {}
    ~ContainerTable();
    int     lookupKey(int owner_id);
    void*   lookupInTable(int elem_id, int owner_id);
    int     lookupInTable(void *elem_ptr, int owner_id);
    void    addToTable(void* elem_ptr, int& elem_id, const int owner_id);

private:
    VOID_LIST       m_KeyList;
    VOID_LIST       m_ListOfVoidList;
};

#endif
