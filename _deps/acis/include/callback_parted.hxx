/* ORIGINAL: 3dt2.1/examples/motif/parted/callback.hxx */
// $Id: callback_parted.hxx,v 1.1 2002/07/18 15:03:59 skenny Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifdef MOTIF
//----------------------------------------------------------------------
// purpose---
//    header defining callbacks for main menu and window widgets
//
//----------------------------------------------------------------------

#include "Xm/Xm.h"


// defines for colors
#define BLACK_COLOR   0
#define RED_COLOR     1
#define GREEN_COLOR   2
#define BLUE_COLOR    3
#define CYAN_COLOR    4
#define YELLOW_COLOR  5
#define MAGENTA_COLOR 6
#define WHITE_COLOR   7


// general close widget CB
void CloseitCB(Widget w, XtPointer , XtPointer );

// File menu callbacks
void OpenFDlgOkCB(Widget , XtPointer , XtPointer );
void OpenFCallback(Widget , XtPointer , XtPointer );
void SaveFCallback(Widget , XtPointer , XtPointer );
void SaveAsFCallback(Widget , XtPointer , XtPointer );
void CloseFCallback(Widget , XtPointer , XtPointer );
void QuitFCallback(Widget , XtPointer , XtPointer );

// Edit menu callbacks
void RollBCallback(Widget , XtPointer , XtPointer );
void RollFCallback(Widget , XtPointer , XtPointer );
void ClearCallback(Widget , XtPointer , XtPointer );

// View menu callbacks
void RefreshCallback(Widget , XtPointer , XtPointer );
void EyePointCallback(Widget , XtPointer , XtPointer );
void RenderCallback(Widget , XtPointer , XtPointer );

// Solid menu callbacks
void BlockCallback(Widget , XtPointer , XtPointer );
void SphereCallback(Widget , XtPointer , XtPointer );
void CylinderCallback(Widget , XtPointer , XtPointer );
void ConeCallback(Widget , XtPointer , XtPointer );

// Command entered callback
void CmdCallback(Widget , XtPointer , XtPointer );

// set background color callbacks
void BBlackCallback(Widget , XtPointer , XtPointer );
void BRedCallback(Widget , XtPointer , XtPointer );
void BGreenCallback(Widget , XtPointer , XtPointer );
void BBlueCallback(Widget , XtPointer , XtPointer );
void BCyanCallback(Widget , XtPointer , XtPointer );
void BYellowCallback(Widget , XtPointer , XtPointer );
void BMagentaCallback(Widget , XtPointer , XtPointer );
void BWhiteCallback(Widget , XtPointer , XtPointer );



#endif
