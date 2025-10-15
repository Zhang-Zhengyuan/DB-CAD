/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//----------------------------------------------------------------------------
#ifndef AGGR_GEOM_UTILITY
#define AGGR_GEOM_UTILITY

#include "tophdr.hxx"

#include "dcl_stitch.h"

//----------------------------------------------------------------------------

class EDGE;
class FACE;
class COEDGE;
class VERTEX;
class CURVE;
class PCURVE;
class SURFACE;
class APOINT;


DECL_STITCH void hh_set_geometry( EDGE *, CURVE * );
DECL_STITCH void hh_set_sense( EDGE *, REVBIT sense_data);
DECL_STITCH void hh_set_geometry( COEDGE *, PCURVE * );
DECL_STITCH void hh_set_sense( COEDGE *, REVBIT sense_data);
DECL_STITCH void hh_set_geometry( FACE *, SURFACE *, const logical update_pcurve = 0);
DECL_STITCH void hh_set_sense( FACE *, REVBIT sense_data);
DECL_STITCH void hh_set_geometry( VERTEX *, APOINT *);
// void hh_set_geometry( ENTITY *, ENTITY *);

DECL_STITCH CURVE *		hh_get_geometry( EDGE *);
DECL_STITCH REVBIT		hh_get_sense(EDGE* e);
DECL_STITCH SURFACE *	hh_get_geometry( FACE * );
DECL_STITCH REVBIT		hh_get_sense(FACE* fa);
DECL_STITCH APOINT *	hh_get_geometry( VERTEX * );
DECL_STITCH PCURVE *	hh_get_geometry( COEDGE * );
DECL_STITCH REVBIT		hh_get_sense(COEDGE* e);

//----------------------------------------------------------------------------

#endif
