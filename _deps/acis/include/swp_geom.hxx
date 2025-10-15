/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//             Internal Routines for the sweep functionality.
/*******************************************************************/
#if !defined ( SWP_GEOM )
#define SWP_GEOM
#include "dcl_swp.h"
#include "base.hxx"
class KERN_PROTECTED_LIST;
class ENTITY_LIST;
class COEDGE;
class law;
class surface;
class SPAtransf;
class sweeper;

// Main routine which computes the lateral surface for a rigid sweep.
DECL_SWP surface *
sg_rigid_surf( 
    COEDGE *,    // coedge to sweep for creating the lateral surface
    COEDGE *     // Path coedge
);

/*******************************************************************/
//
// Perpendicular Sweep with draft.
//
// Main sweep routine, which sweeps a chain of coedges(profile) along 
// a chain of coedges( path ), returns TRUE if successfull.( With Draft )
DECL_SWP logical
sg_sweep_perp_w_draft(
	sweeper *,		// Full set of sweep data, including options
    COEDGE *,       // chain coedge
    COEDGE *,       // Path coedge
	law  *rail_law, // Rail law
	law  *draft_law,// Draft law
	law  *scale_law,// Scale law
    COEDGE *,       // First Path
    COEDGE *,       // Last Path
    SPAtransf &,       // Path to shape( chain ) transformation
    int &,          // chain length
    int &,          // number of lateral faces, should be made
    const surface &,// To surface
    int,            // Draft_type
    COEDGE *&,      // Top coedge returned, for next sweep path
    KERN_PROTECTED_LIST &,  // Pcurve Coedge List accumulated
    KERN_PROTECTED_LIST &,  // Edges to be merged
    KERN_PROTECTED_LIST &   // Vertices to be merged
);

#endif
