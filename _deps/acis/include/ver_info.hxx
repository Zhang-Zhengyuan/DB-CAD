/* ORIGINAL: 3dt2.1/main/ver_info.hxx */
// $Id: ver_info.hxx,v 1.7 2000/12/26 19:05:39 products Exp $
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
//    Header file for version and deltas which can be used in tests
//    to prohibit more modify Personal ACIS functionality depending on 
//    when it was implemented
//
// author---
//    dkl 02-08-90
//
//----------------------------------------------------------------------

#ifndef ver_infoP_hxx
#define ver_infoP_hxx

#include "logical.h"

// Problem:  MAX_HDR_LEN used to be 100 and was being over run.  We
//           have upped it to 300, but should really be dynamic.
#define MAX_HDR_LEN 300
//                              rel  rev  delta
#define OLDEST_LEGAL_PART       1,   3,   0
#define FACE_TYPE_MOD           1,   3,   59
#define HOLE_OP_MOD             1,   6,   57

extern logical part_newer_than(
	int release,
	int revision,
	int delta);
extern logical part_older_than(
	int release,
	int revision,
	int delta);
extern logical part_newer_than_current_version();
extern logical part_older_than_current_version();


// declarations for getting and setting current release of part
extern void set_release( int release );
extern void set_revision( int revision );
extern void set_delta( int delta );
extern int get_release();
extern int get_revision();
extern int get_delta();

// declarations for getting relevant sti info
extern const char* get_sti_delta_id();
extern const char* get_sti_os_id();
extern const char* get_sti_date();
extern const char* get_build_date();
extern const char* get_header();

extern int   get_sti_rel_no();
extern int   get_sti_rev_no();
extern int   get_sti_delta_no();
extern const char* get_sti_version_id();

#ifdef __cplusplus
extern "C" 
#endif
const char* get_window_title();
#endif
