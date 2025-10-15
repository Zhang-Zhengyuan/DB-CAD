/* ORIGINAL: 3dt2.1/scheme/flnm_lst.hxx */
// $Id: flnm_lst.hxx,v 1.5 2000/12/26 19:05:54 products Exp $
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
//    Header file for filename_list classs (see filename_list.cxx)
//
// access---
//    Do not Doc.
//----------------------------------------------------------------------

#ifndef flnm_lst_hxx
#define flnm_lst_hxx

#include "mmgr.hxx"

class filename_list: public ACIS_OBJECT
{
public:
    filename_list(const char* pat);        // constructor
    ~filename_list();                      // destructor

    int num_files;                    // number of files matching pattern
    char** fname_ptrs;
};

#endif
