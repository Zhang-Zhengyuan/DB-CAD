/* $Id: choputil.hxx,v 1.6 2001/09/18 18:41:54 dlavende Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//  Header file for small topology manipulation functions used by the chop
//  function.

#ifndef choputil_hxx
#define choputil_hxx

#include "dcl_bool.h"
#include "logical.h"
#include "edge.hxx"
#include "coedge.hxx"

class BODY;
class LUMP;
class SHELL;
class WIRE;

DECL_BOOL inline logical
is_free_edge(EDGE* ed)
{
    COEDGE  *ce = ed->coedge();
    return
        ( ce->partner() == NULL &&
          ce->loop() != NULL &&
          ed->geometry() != NULL );
}

DECL_BOOL void
transfer_shell(
    SHELL*,    //  Shell to be transferred
    LUMP*      //  Lump to receive shell
    );

DECL_BOOL void
transfer_lump(
    LUMP*,     //  Lump to be transferred
    BODY*      //  Body to receive lump
    );

#endif
