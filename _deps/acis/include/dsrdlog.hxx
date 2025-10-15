// $Id: dsrdlog.hxx,v 1.4 2000/12/26 18:33:23 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dsrdlog.hxx
// created  : February 23, 1999
// author   : John Sloan
// contains : DS_reduce_log container class

#ifndef DS_RDLOG_H
#define DS_RDLOG_H
#include "mmgr.hxx"
#include "dsstdef.hxx"  // DS_TRUE, DS_FALSE  // DS_TRUE, DS_FALSE
#include "dsrhblk.hxx"  // class DS_row_handle  // class DS_row_handle

class DS_reduce_event : public ACIS_OBJECT
// TODO:??? (JHS) should be struct, but genblk macros not equipped
//                to handle this.  Could put class/struct extra macro
//                into genblk - may not be worth it.
{
 public:

  int    reducer_row;
  int    reducee_row;
  double mult;
  int    caused_zero;

  DS_reduce_event()  // default constructor for array construction
   :
    reducer_row(0),
    reducee_row(0),
    mult(0),
    caused_zero(DS_FALSE)
   {}

  DS_reduce_event(
    int reducee, 
    int reducer, 
    double rmult, 
    int zero)
   :
    reducer_row(reducer),
    reducee_row(reducee),
    mult(rmult),
    caused_zero(zero)
   {}
};

// define DS_reduce_log as block of DS_reduce_events
#define DS_GENBLK_CLASS     DS_reduce_event
#define DS_GENBLK_CONTAINER DS_reduce_log

#include "dsgenblk.hxx"

#undef  DS_GENBLK_CLASS
#undef  DS_GENBLK_CONTAINER

// add push function
void Push_onto(DS_reduce_log & log,
                          const DS_reduce_event event);

inline void Erase(DS_reduce_log & log)
 {log.Need(0);}

#endif // DS_RDLOG_H
