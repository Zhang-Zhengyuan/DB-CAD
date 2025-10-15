/* ORIGINAL: acis2.1/kerndata/top/tophdr.hxx */
/* $Id: tophdr.hxx,v 1.9 2000/12/26 18:47:09 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Common header for all topological data classes. Pulls in master
// definitions if needed, and declares all entity types referenced
// in the topology.

#if !defined( TOP_HEADER )
#define TOP_HEADER

#include "logical.h"

#include "entity.hxx"

// Declare the data structure record classes, to avoid illegal forward
// references: it does no harm if we are not interested in them.

class BODY;
class LUMP;
class SHELL;
class SUBSHELL;
class FACE;
class LOOP;
class COEDGE;
class EDGE;
class VERTEX;
class WIRE;

class TCOEDGE;
class TEDGE;
class TVERTEX;

class SURFACE;
class CURVE;
class PCURVE;
class APOINT;
class TRANSFORM;

class ATTRIB;

// Useful definition for sense bit for faces and edges.

typedef logical REVBIT;
#define FORWARD FALSE
#define REVERSED TRUE

#endif
