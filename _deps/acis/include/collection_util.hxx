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
// amt 02Nov02 Changing GROUP to SPACOLLECTION.
/*******************************************************************/

#if !defined(COLLECTION_UTIL)
#define COLLECTION_UTIL

#include "dcl_kern.h"

class SPACOLLECTION;
class ENTITY;
class ENTITY_LIST;

//----------------------------------------------------------------
//----------------------------------------------------------------

DECL_KERN void	col_return_collections(ENTITY *, ENTITY_LIST&);

DECL_KERN void	col_add_to_collection(ENTITY *, SPACOLLECTION *);

DECL_KERN void	col_remove_from_collection(ENTITY *, SPACOLLECTION *);

DECL_KERN void	col_return_ents(SPACOLLECTION *, ENTITY_LIST& );

DECL_KERN void	col_delete_collection_entities(SPACOLLECTION *);


#endif
