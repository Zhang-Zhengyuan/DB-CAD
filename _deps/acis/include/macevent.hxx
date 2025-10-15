/* $Id: macevent.hxx,v 1.8 2002/01/30 17:32:56 skenny Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

/*----------------------------------------------------------------------*/
/* purpose---															*/
/*    define an event loop interface for Macintosh systems.				*/
/*																		*/
/*----------------------------------------------------------------------*/

#ifdef mac

#ifndef event_loop_hxx
#define event_loop_hxx

#include "logical.h"

void	event_loop(void);

#ifdef __cplusplus
extern "C"	{
#endif

	pascal OSErr HandleQuit (const AppleEvent *aevt, AEDescList *reply, long refcon);

	OSStatus OnCommand(HICommand *pCmd);
	OSStatus OnCommandUpdate(HICommand *pCmd);
	OSStatus OnContextMenu(Point pt);
	OSStatus OnClose();

	OSStatus DoAboutAcis();
	OSStatus DoFileDialog(Boolean save);

	OSType   SafeOSTypeFromFSRef(FSRef* fsRef);
	
//logical	 process_event(logical wait);
	
#ifdef __cplusplus
}
#endif

/* menu bar Resource ID's */
enum {rMBarID = 128};
enum {rAppleMenu = 128, rFileMenu, rEditMenu, rViewMenu};

#endif

#endif
