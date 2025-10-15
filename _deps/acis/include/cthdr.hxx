/* ORIGINAL: acis2.1/ct_husk/classes/cthdr.hxx */
// $Id: cthdr.hxx,v 1.9 2000/12/26 18:31:17 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Common header for all cellular data classes. Pulls in base
// definitions if needed, and declares all entity types referenced
// in the ct_husk.

#if !defined( CELL_HEADER )
#define CELL_HEADER

#include "logical.h"

#include "entity.hxx"

// Declare the data structure record classes, to avoid illegal forward
// references: it does no harm if we are not interested in them.

class SUPERCELL;
class CELL;
class CELL3D;
class CELL2D;
class CSHELL;
class CFACE;

#endif
