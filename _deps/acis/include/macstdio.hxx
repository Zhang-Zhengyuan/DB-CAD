/* $Id: macstdio.hxx,v 1.7 2002/01/30 17:32:56 skenny Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifdef mac

#ifndef mac_stdio_hxx
#define mac_stdio_hxx

#include "carbonheader.h"

/* stdio window status (returned by stdioStatus */
#define STDIO_CLOSED	-1
#define STDIO_INPUT		0
#define STDIO_OUTPUT	1

#ifdef __cplusplus
extern "C"	{
#endif

WindowPtr stdioOpen(Boolean bQuit);
Boolean stdioTextSelected();
Boolean stdioInputSelected();
void init_mstdio(int (*)(const char *, int), bool);
void stdioActivate(EventRecord *);
void stdioChar(char, UInt32);
void stdioCommand(const char *);
void stdioClear();
void stdioClearLine();
void stdioSelectAll();
void stdioClose();
void stdioCut();
void stdioCopy();
void stdioEndOutput();
void stdioIdleTasks();
void stdioPaste();
void stdioStartOutput();
void stdioFlush();
int	stdioStatus();
void MacStdioOutput( FILE * outptr );

#ifdef __cplusplus
}
#endif

#endif

#endif
