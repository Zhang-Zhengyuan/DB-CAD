/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (BHL_SEC_UTIL)
#define BHL_SEC_UTIL

#include "dcl_stitch.h"
#include "base.hxx"
#include "add_pcu.hxx"

class SPAposition;
class EDGE;
class intcurve;
class pcurve;

#include "bs3curve.hxx"
#include "bs2curve.hxx"

#include "bhl_stru2.hxx"

/*
extern int bhl_geom_bld_pcurves;
extern int bhl_geom_bld_bool_pole;
extern int bhl_geom_bld_reverse;
extern int bhl_geom_bld_trim_edges;
extern DECL_STITCH logical hh_use_sg_add_pcurve;
*/
// Trimming edge related routines

DECL_STITCH logical
bhl_trim_bs3_curve(bs3_curve , SPAposition const& ,  SPAposition const& ,
			logical bit, bs3_curve&,logical * = NULL);
DECL_STITCH logical
bhl_trim_bs3_curve(bs3_curve, double, double, bs3_curve&);

DECL_STITCH logical
hh_add_pcurve_to_coedge( COEDGE* , logical = FALSE, logical = FALSE);

logical
bhl_add_pcurve_to_coedge( COEDGE* );

void
bhl_remove_bad_pcurves_from_entity(ENTITY *, logical force = FALSE);


DECL_STITCH logical
bhl_trim_pcurve(const pcurve&, COEDGE*, pcurve*&);

/*
logical
bhl_trim_pcurve(COEDGE*, PCURVE*&);
*/

// GSSL/RS (Aug 16,2002):Added a new argument trim_not_needed
DECL_STITCH logical
bhl_trim_edge( EDGE* , logical &trim_not_needed = *(logical *)NULL_REF);

DECL_STITCH logical
bhl_trim_intcurve( const intcurve* , const SPAposition&, const SPAposition&, logical, intcurve*& );

DECL_STITCH logical
bhl_trim_coedge( COEDGE* );

DECL_STITCH void
bhl_trim_coedges( ENTITY* );

// GSSL/RS - (Apr 25,2002) : Exported the class
class DECL_STITCH HH_BS2_CURVE_WRAP{
public: 
	bs2_curve set_bs2_curve(const pcurve& );
	HH_BS2_CURVE_WRAP();
	~HH_BS2_CURVE_WRAP();
	bs2_curve unhook();
private:

protected:
	bs2_curve bs2;
};


#endif

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
