// $Id: dsmbvbl.hxx,v 1.3 2000/12/26 18:33:22 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dsmbvbl.hxx
// created  : February 7, 1999
// author   : John Sloan
// contains : DS_mbvec_block container class

#ifndef DS_MBVL_H
#define DS_MBVL_H

class DS_multi_banded_vec;

#define DS_GENBLK_CLASS     DS_multi_banded_vec
#define DS_GENBLK_CONTAINER DS_mbvec_block

#include "dsgenblk.hxx"

#undef  DS_GENBLK_CLASS
#undef  DS_GENBLK_CONTAINER

#endif // DS_MBVL_H
