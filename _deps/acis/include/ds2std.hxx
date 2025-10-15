/* $Id: ds2std.hxx,v 1.3 2002/08/20 14:01:04 dlavende Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : ds2std.hxx
// created  : Oct. 22, 1996
// author   : GWCelniker
// contains : DS_acis_assert, DS_acis_verify
// synopsis : Contains standard ACIS macros and declariations used
//            to integrate Deformable Modeling with ACIS but
//            which are not deformable modeling dependent.
//            These are mostly convenience routines.
//            
// contains : DS_signal_dm_errors()
//            DS_position_to_global()
//            DS_position_to_shape_space()
//            DS_vector_to_shape_space()
//            DS_unit_vector_to_shape_space()
//            DS_vector_to_global_space()
//            DS_pixels_to_length()

#ifndef DM2STD_ATTRB_H
#define DM2STD_ATTRB_H

#include "api.hxx"
#include "errorsys.hxx" // sys_error(),  // sys_error(), 
                                                 // sys_warning()
#include "dcl_adm.h"
#include "ds2acis.err"            // sys_error() defines            // sys_error() defines

// forward declarations
class ATTRIB_DM2ACIS ;
class SPAposition;
class SPAvector;
class SPAunit_vector;

// ACIS BASED STD DEFINITIONS: INCLUDED IN THIS FILE FOR CONVENIENCE
#define DS_acis_assert(exp,val) if(!(exp)){ DS_sys_error(val) ; }
#define DS_acis_verify(exp,val) if(!(exp)){ DS_sys_warning(val) ; }

inline void DS_sys_error  // force DS_acis_assert thru a sub for debug
  (int val)               { sys_error(val) ; }
inline void DS_sys_warning// force DS_acis_assert thru a sub for debug
  (int val)               { sys_warning(val) ; }

// error reporting functions

DECL_ADM void               // eff: convert DM library errors
DS_signal_dm_errors        //      into ACIS error signals
 (int ) ;                  // in : rtn_flag = a DM library error value
                           // note: src in file ds2acis.cxx

#endif // DM2STD_ATTRB_H

