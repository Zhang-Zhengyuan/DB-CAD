/* ORIGINAL: abl2.1/blend/kernbool/entent/bl_sup.hxx */
/* $Id: bl_sup.hxx,v 1.10 2002/08/09 17:28:29 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Modification---
//
//  30-Aug-07 pvc   Added class bl_continuity_level and methods 
//                  lateral_continuity to support_entity.
//  21-May-04 puc   Made the support chain doubly linked.
//  20-Apr-04 syh	Added function to trim interferences, mostly to take care of
// 		periodic interferences while handling multiple cap sheet. 
//  28-Oct-03 puc   Part of the changes made for entity-entity coupling project,
//  Added a function to change the owning blend attribute of the 
//  support entity, this is required for situations where bi-blend
//  code creates a face and then transfers the ownership of the blend face to
//  the adjacent edge attribute.
//                  
//	24-Jan-97 DAP	New attribute based blend stage 2.
//	1 Apr 96  nvp 	Add function to set_spring_cur_sense
//	08-Mar-96 bd	Add acis.hxx include.
//	25-Jan-96 aed	Change null refs to NULL_REF.
//	29-Dec-95 dap	Declared class v_bl_contacts instead of including
//			header file, removed include of cvc.hxx.
//	20-Dec-95 nvp	add on_support()
//

#ifndef BL_SUP_ENT
#define BL_SUP_ENT

#include "dcl_blnd.h"
#include "acis.hxx"

#include "logical.h"
#include "bl_para.hxx"
#include "box.hxx"

class surface;
class SPAposition;
class SizeAccumulator;
class face_face_int;

class ENTITY;
class FACE;
class EDGE;
class COEDGE;
class VERTEX;
class BODY;

class ATTRIB_BLEND;
class ENTITY_LIST;

class bl_point_curve;
class bl_pp_pcur;
class blend_int;

class blend_seg;
class bl_segment;

class v_bl_contacts;

// Containment of spring curve point w.r.t its support.

typedef enum 
	{
	point_unknown_support,
	point_inside_support,
	point_boundary_support,
	point_outside_support
	} point_support_containment;

class DECL_BLND bl_continuity_level : public ACIS_OBJECT {
    
public:
    enum continuity { G0 = 0, G1 = 1, G2 = 2 };

    bl_continuity_level( continuity c = G0 ) : m_cont(c) {}

    logical is_G0() const { return m_cont >= G0; }
    logical is_G1() const { return m_cont >= G1; }
    logical is_G2() const { return m_cont >= G2; }

private:

    continuity m_cont;
};


// Class storing a supporting entity, or the entity of the body on which
// the blend is lying. It remembers the spring curve on it, its
// direction, and so on. Convention dictates that the list of support
// entities is kept anticlockwise (though this makes no odds for
// entity-entity blends). Spring curves are kept in the same
// direction as they lie parameterwise on the blend surface. If this is
// not in the anticlockwise direction, the reversal flag is set.


class DECL_BLND support_entity : public ACIS_OBJECT 
{
public:

	// Constructors, memory management, etc.

	support_entity( ATTRIB_BLEND * );

	virtual ~support_entity();

	// Delete the list from here on.

	void delete_all();

	// Copy the list from here on. Calls the pure virtual copy. This is
	// only provided because we would like to be able to roll back to
	// just after the attribute was added, so all we need to remember is
	// anything that was known *prior* to the start of blend stage 1.

	support_entity *copy_all() const;

	virtual support_entity *copy() const = 0;

	// The entity's SPAbox.

	virtual SPAbox get_box() const = 0;

	// The owning attribute.

	ATTRIB_BLEND *att() const { return _attrib; }

	void  set_att( ATTRIB_BLEND *own_att ) { _attrib = own_att; }

	// The entity the blend is running along in contact with.

	ENTITY *entity() const { return _entity; }

	// The spring point or curve, with associated SPAparameter point or
	// curve relating it to the blend surface.

	bl_point_curve *spring_point_curve() const { return _spring_point_curve; }

	// Stored SPAparameter curve information about the spring curve on the
	// underlying entity (if a spline thing). When the entity is a
	// curve, we are interested only in the u SPAparameter of the uv
	// SPAparameter pairs we obtain; this is because we don't have
	// bs1_curves so use bs2_curves instead.

	bl_pp_pcur *other_pp_pcur() const { return _other_pp_pcur; }
	void set_other_pp_pcur( bl_pp_pcur *p );

	// TRUE if the spring curve above going clockwise round the surface.

	logical reversed() const { return _reversed; }
    logical fwd() const { return !_reversed; }
	void set_reversed( logical rev ) { _reversed = rev; }

	// Set the spring curve of the support.

	void set_spring( bl_point_curve *pc, logical rev );	

	// Whether the spring curve is reversed with respect the sequence.

	logical seq_reversed() const;

	// The list of interference points, or change points. Each derived
	// class makes its own flavour of these. They store where we have run
	// into/over something, storing positional and parametric
	// information and the like.

	blend_int *interferences() const { return _interferences; }

	// First in the list of interferences, but may be given a sense
	// flag. The idea is, if you pass the support's sense, you get the
	// first_int in the coedge sense.

	blend_int *first_int( logical rev = FALSE ) const;

	// Last in the list of interferences (or NULL if none). Takes a
	// reversal flag just as first_int does.

	blend_int *last_int( logical rev = FALSE ) const;

	// The next support_entity, return NULL when we reach the end.

	support_entity *next() const { return _next; }
    support_entity *prev() const { return _prev; }

	// And setting.

	void set_next( support_entity *sup_ent ) { _next = sup_ent; }
    void set_prev( support_entity *sup_ent ) { _prev = sup_ent; }

	// Imprint is probably the single most crucial function. It imprints
	// the spring curve on the thing it runs along, whatever that is,
	// setting the list of interference points. Creating and
	// interpreting these points properly is what blending is all about.

	virtual void imprint( logical convex, BODY *sheet ) = 0;

	// Set the list of interferences.

	void set_interferences( blend_int *ints ) { _interferences = ints; }

	// Add a new blend_int to the list of interferences, in order of
	// increasing spring curve SPAparameter. If there is an existing
	// blend_int at the same SPAparameter value (to resolution) the
	// existing one is returned and the new one NOT added; else NULL is
	// returned (if it was added).

	blend_int *add_int( blend_int *new_one );

	// Remove a blend_int from the interferences. Only removes it; does
	// not delete it.

	void remove_int( blend_int * );

	// Look to see if there is a blend_int already at the given
	// SPAparameter value (to within resolution). Optionally, if the other
	// args are given, also returns the preceding and following
	// blend_ints (in increasing spring curve SPAparameter order) if there
	// are any, else sets the passed pointers to NULL. Note that the
	// preceding and following are found even if NULL is returned to
	// indicate the two either side of the given SPAparameter value.

	blend_int *lookup(
				  double par,
				  blend_int *&before = 
					*(blend_int **)NULL_REF,
				  blend_int *&after = 
					*(blend_int **)NULL_REF
				  ) const;

	// Request a blend_int at the given SPAparameter. If one exists it will
	// be returned, otherwise a new one made. This is the kosher way to
	// get new blend_ints after imprinting has been done. The returned
	// blend_int will have been added to the list. The logical flag
	// allows you to work in coedge sense if you so wish.

	blend_int *request_int( double par, logical rev = FALSE );

	// Canonicalise the list of interferences. This does any necessary
	// munging when the spring curve is periodic and so on, so that the
	// virtual imprint doesn't have to worry about it. This function
	// ensures that the list of imprints always begins with an out-in/on
	// and ends with an in/on-out.

	void canonicalise_interferences();

	// Return the SPAparameter value(s) of the given point on the
	// underlying entity, and spring curve SPAparameter.

	virtual bl_par_info param( SPAposition const &, double ) = 0;

	// Sometimes there may be bits of information to work out or make an
	// educated guess at before we start. Default does nothing but
	// returns TRUE to indicate "all is ok".

	virtual logical establish_parameters();

	// Attach any attributes to mark the intersection of the whole of
	// the spring curve with the underlying entities. Often this will
	// involve attaching faceints (when the spring curve is a curve),
	// but the routine is also called if the spring curve is a point.
	// For each spring edge it is called and passed the COEDGE on the
	// blend face of that edge. If the edge has degenerated to a point,
	// a NULL coedge is passed along with a non-null VERTEX. The sheet
	// face is also passed.

	virtual void attach_global_ints( COEDGE *spr_coed,
							 VERTEX *spr_vert, FACE *sh_face ) = 0;

	// Attach any further known non-intersections between underlying
	// entities and the blend face. We may assume any known
	// intersections to have been marked.

	virtual void attach_non_ints( FACE *sh_face ) = 0;

	// The "lateral face" that will be advertised as such to the
	// existing blend stage 2 code. Stage 2 only.

	virtual FACE *lateral_face() const = 0;

	// (A copy of) It's surface, corrected to point with the face
	// normal.

	surface *lat_sf();

	// Clear the local copy of the lateral FACE's surface.  This is 
	// useful if you are extending the FACE's surface because if you don't 
	// get rid of local copy it will be used rather than the new, extended
	// surface stored in the FACE.

	void clear_lat_sf();

    // Track requied tangency of the blend with its supports

	bl_continuity_level lateral_continuity() const { return _cont_level; }
	void set_lateral_continuity( bl_continuity_level l ) { _cont_level = l; }

	// Normally, there is no auxiliary surface for the spring curve, not
	// defined at the base class level, anyway. The default returns NULL.

	virtual surface *aux_sf();

	// Accessing and setting the auxilliary face pointer. This must be
	// done (and the appropriate efints added) prior to calling
	// imprint() if trim_face_line_to_face is to be called. imprint()
	// discards the aux_face when it is done.

	FACE *aux_face() const { return _aux_face; }
	void set_aux_face( FACE *f ) { _aux_face = f; }

	// A virtual function to determine whether we are on the entity or outside
	// Provided for variable radius blending only.

	virtual point_support_containment on_support( v_bl_contacts *, logical ) = 0;

	// Check the convexity relations of the given spring coedge, i.e.
	// check the blend surface really does appear to sit inside (for
	// convex), outside (for concave), the blank. A default is provided
	// which returns TRUE (i.e. ok).

	virtual logical check_cvxty( COEDGE *spr_coed );

	// Trim interferences to this change point, in the given direction. 

	void trim_interferences( double trim_par, logical fwd );

	virtual void full_size( SizeAccumulator &, logical = TRUE ) const;

protected:

	// Make a "dummy" blend_int. Normally this means making a blend int
	// to mark point when the imprinting process didn't actually produce
	// one, or at least an appropriate one. Passed a spring curve
	// SPAparameter. Adds it to the interference list.

	// Examples of use:
	// 1. To make extra blend_ints for the start/end of the spring
	// curve's SPAparameter range did not begin with out-in/on or end with
	// in/on-out (both bounded non-periodic and periodic cases).
	// 2. In entity-entity blending if one support entity has a
	// blend_int which delimits a segment, we must have one on other
	// support entity too. This gets called to make one only if there
	// isn't one already.

	// After imprinting has been done, request_int should always be
	// called.

	virtual blend_int *make_int( double ) = 0;

	// Make the lateral face_face_int at the spring parameter. Used by
	// derived classes to provide 'make_int' functionality.

	face_face_int *make_lateral_ffi( double par ) const;

	// Optimize the imprint of the spring curve on the lateral support.

	virtual void optimize_lateral_imprint();

	// Helper function for imprint optimization. Mark coincidence of the
	// spring curve with an edge.

	logical mark_coincident( EDGE *sup_edge, logical rev );

	// Imprint the spring curve on the lateral face.

	void imprint_lateral_face( logical convex, BODY *sheet );

	// Allocate a new blend_int. This virtual function is provided
	// so that derived classes can make derived blend_ints.

	virtual blend_int *new_blend_int( face_face_int *, COEDGE * ) = 0;

	// The underlying support entity.

	ENTITY *_entity;
	
	// The lateral face surface, corrected to point the same way as 
	// the lateral face.

	surface *_lat_sf;

	// Auxilliary surface is wanted for imprinting.

	surface *_aux_sf;

	// If the imprint process can be optimised by using new imprinting
	// then the calling code must set the auxilliary face into here, and 
	// pre-empt the appropriate efints.

	FACE *_aux_face;

private:

	// The implicit blend attribute.

	ATTRIB_BLEND *_attrib;

	bl_point_curve *_spring_point_curve;

	logical _reversed;

	// The list of interferences of the spring, owned by the support.

	blend_int *_interferences;

	support_entity *_next;
    support_entity *_prev;

	// A SPApar_pos or bs2_curve describing the spring curve (or point) on
	// the underlying surface or curve. This is needed for stage two.

	bl_pp_pcur *_other_pp_pcur;

	// Continuity requirement of the blend with respect to the underlying 
	//support.

	bl_continuity_level _cont_level;
};

// Investigates the spring curves to determine whether the blend is 
// periodic.

logical periodic_blend( support_entity *sup );

// Investigates the spring curves to determine whether the blend is 
// closed.

logical closed_blend( support_entity *sup );

// Check whether this blend face corresponds to a periodic blend.

logical
periodic_blend( 
    FACE *blend_face,
    logical &v_par = *(logical *)NULL_REF // Whether periodic in the v.
    );

#if defined D3_DEBUG || defined D3_STANDALONE

class D3_ostream;

// We shall adopt a convention whereby passing a pointer gives a brief
// output, a reference gives fuller output including the interferences.

DECL_BLND D3_ostream &operator<<( D3_ostream &os, support_entity const & );
DECL_BLND D3_ostream &operator<<( D3_ostream &os, support_entity const * );

#endif


#endif
