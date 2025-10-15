// $Id: dsrhblk.hxx,v 1.3 2000/12/26 18:33:24 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dsrhblk.hxx
// created  : February 23, 1999
// author   : John Sloan
// contains : DS_reduce_log container class

#ifndef DS_RHBLK_H
#define DS_RHBLK_H
#include "mmgr.hxx" // class ACIS_OBJECT

class DS_row_handle : public ACIS_OBJECT
// TODO:??? (JHS) should be struct, but genblk macros not equipped
//                to handle this.  Could put class/struct extra macro
//                into genblk - may not be worth it.
{
 public:

  int    true_row;
  int    first;
  int    last;
};

// define DS_reduce_log as block of DS_reduce_events
#define DS_GENBLK_CLASS     DS_row_handle
#define DS_GENBLK_CONTAINER DS_row_handle_block

#include "dsgenblk.hxx"

#undef  DS_GENBLK_CLASS
#undef  DS_GENBLK_CONTAINER

#endif // DS_RHBLK_H
