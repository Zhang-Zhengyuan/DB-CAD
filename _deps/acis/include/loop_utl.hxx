// $Id: loop_utl.hxx,v 1.25 2002/08/09 17:15:15 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// purpose---
//    LOOP utilities
//
//======================================================================

#ifndef LOOP_UTL_HXX
#define LOOP_UTL_HXX

//======================================================================

#include "dcl_kern.h"
#include "logical.h"
#include "api.hxx"
#include "ptfcenum.hxx"
#include "position.hxx"

class LOOP;
class ENTITY_LIST;
class COEDGE;
class BODY;
class FACE;
class surface;
class SPApar_pos;
class pcurve;
class curve;
class SPAunit_vector;
class SPAposition;
class SPAtransf;
class WIRE;
class CONE;
class SPApar_box;


//======================================================================

DECL_KERN loop_type get_loop_type(LOOP *, int info[2]=NULL);
DECL_KERN logical   get_periphery_edges_from_face(FACE*, ENTITY_LIST &);
DECL_KERN logical   validate_face_loops(FACE *, int info[5]=NULL);
DECL_KERN void      set_periphery_first_loop(FACE *in_face);

DECL_KERN logical   valid_chain(COEDGE*,logical&,COEDGE*&,int&);
DECL_KERN logical   find_planar_chain_normal(COEDGE *, SPAunit_vector &, logical &);
DECL_KERN logical   find_planar_chain_normal(BODY *, SPAunit_vector &, logical &);

DECL_KERN int       count_coedges_in_chain(COEDGE *in_coed);
DECL_KERN logical   closed_coedge_chain(COEDGE *in_coed, int &n_coed);
DECL_KERN COEDGE*   get_first_coedge_from_body(BODY *a_body);
DECL_KERN void      make_first_coedge(COEDGE *&a_coedge);
DECL_KERN void      get_coedges_in_chain(COEDGE *, ENTITY_LIST &);
DECL_KERN WIRE     *get_first_wire(BODY *a_body);
DECL_KERN FACE     *get_first_face(BODY *a_body);
DECL_KERN logical  sg_get_loop_par_box(LOOP *this_loop, SPApar_box& loop_range, logical &box_set_loop, logical test_old_uv_bound );

DECL_KERN SPAposition  get_cone_apex(CONE* acone, double &u_param= *(double *) NULL_REF);

DECL_KERN bool is_degenerate_loop(const LOOP* loop);

//======================================================================
// DEBUG FUNCTIONS

DECL_KERN void scheme_display_entity(FILE *out, const curve *a_curve, 
								     SPAtransf &use_transf=*(SPAtransf *) NULL_REF,
								     int num_pt=4);

DECL_KERN void scheme_display_entity(FILE *out, SPAposition &a_pos, double size=0.125);

DECL_KERN void scheme_display_entity(FILE *out, ENTITY *start, logical=FALSE, int num=4);

DECL_KERN void scheme_display_entity(FILE *out, const surface *a_surf, 
								     SPAtransf &use_transf=*(SPAtransf *) NULL_REF, int num=4);

//======================================================================

#endif

