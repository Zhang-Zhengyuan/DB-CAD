/* ORIGINAL: 3dt2.1/gihusk/rb_utl.hxx */
// $Id: gen_rb_utl.hxx,v 1.2 2001/12/26 23:25:27 jmb Exp $
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
//    Declare procedures for setting up rubber banding
//
//----------------------------------------------------------------------

#ifndef gen_rb_utl_hxx
#define gen_rb_utl_hxx

//======================================================================
#include "logical.h"

class rubberband_manager;
class rubberband_driver;
//----------------------------------------------------------------------

void RBAddDriver(rubberband_driver*);
void RBRemoveDriver(const char*);
void RBRemoveDriver(rubberband_driver*);
int RBClearDrivers(logical);

rubberband_manager* RBPush();
rubberband_manager* RBPop();

// There exists a class for lists of rubberband_drivers.  The only
// public functions are for the iterator and for obtaining the active one.

// Return the first driver in the list or NULL to indicate list is empty.
//rubberband_driver* init(rubberband_driver_list*);

// Return the next driver in the list or NULL to indicate no more.
//rubberband_driver* next(rubberband_driver_list*);

// Return driver list from given depth in stack of such lists.
//rubberband_driver_list* RBList(int = 0);

//void RBStartNotify(const PickEvent&);
//void RBUpdateNotify(const PickEvent&);
class SchemeView;
void RBStopNotify();
void RBRepaintNotify(SchemeView*);
void RBDeleteView_notify(SchemeView*);

//======================================================================
#endif
