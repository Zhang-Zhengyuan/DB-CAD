/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Base class for sweep annotations
/*******************************************************************/
#ifndef __SWP_ANNO_HXX__
#define __SWP_ANNO_HXX__

#include "annotation.hxx"
#include "dcl_swp.h"
#include "edge.hxx"
#include "face.hxx"
#include "vertex.hxx"
#include "coedge.hxx"
/*******************************************************************/
/**
 * @file swp_anno.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup SWPAPI
 * @{
 */
/*
 // tbrv
 */
/**
 * @nodoc
 */
#define ANNOTATE_COEDGE FALSE  // == TRUE code removed 19980917
/*
 // tbrv
 */
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( SWEEP_ANNO_EDGE_TOP, SWP )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/*
 // tbrv
 */
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( SWEEP_ANNO_EDGE_LAT, SWP )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/*
 // tbrv
 */
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( SWEEP_ANNO_VERTEX_TOP, SWP )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( SWEEP_ANNO_VERTEX_LAT, SWP )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/*
  // tbrv
 */
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( SWEEP_ANNO_END_CAPS, SWP )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
  /*
  // tbrv
 */
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( SWEEP_ANNO_EDGE_MITER, SWP )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
  /*
  // tbrv
 */
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( SWEEP_ANNO_VERTEX_MITER, SWP )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
  /*
  // tbrv
 */
/**
 * @nodoc
 */

ENTITY_IS_PROTOTYPE(SWEEP_ANNOTATION, SWP)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

//-------------------------------------------------------------------
// Base class.  Items common to all sweep annotations.
//-------------------------------------------------------------------
/**
 * @nodoc
 */
extern DECL_SWP int SWEEP_ANNOTATION_TYPE;
/*
 // tbrv
 */
/**
 * @nodoc
 */
#define SWEEP_ANNOTATION_LEVEL (ANNOTATION_LEVEL + 1)

// annotate lateral faces from sources
/*
// tbrv
 */
/**
 * @nodoc
 */
DECL_SWP void annotate_lateral_faces(ENTITY_LIST &lateral_faces,
                                     ENTITY_LIST &profile_edges,
                                     ENTITY_LIST &profile_vertices,
                                     ENTITY      *path_ent,
                                     logical      open_wire);
/*
// tbrv
 */
/**
 * @nodoc
 * find where the entity is copied from
 */
DECL_SWP ENTITY * get_genesis_entity(ENTITY *in_ent);

// Find the profile profile entity from sweeping and annotating
// a previous path segment.
/*
// tbrv
 */
/**
 * @nodoc
 */
DECL_SWP ENTITY* get_genesis_profile_ent(ENTITY* entity);
/**
 * Implements the base class for sweeping annotations.
 * <br>
 * <b>Role:</b> This is the base (organization) class for sweeping annotations.
 * More specific sweep annotation classes are derived from this class.
 * The <tt>SWEEP_ANNOTATION</tt> base class holds the inputs to the sweep.
 * Derived classes hold the different types of outputs.
 * <br><br>
 * The inputs to sweeping are the path and profile segments.
 * <br><br>
 * In the simple case, the path is the edge along which the
 * the profile was swept to create the given outputs.  When
 * the profile is swept along multiple path segments, there
 * may be an end to end merge.  In this case the path is an
 * <tt>ENTITY_LIST</tt> containing the path from the start to end of the
 * merge. When segments of the path are not C2 continuous,
 * aditional sweep segments are generated to construct the
 * mitre. The path for those sweep segments is the vertex
 * joining the two path segments.
 * <br><br>
 * The profile is an <tt>EDGE</tt> or <tt>VERTEX</tt> of the input profile,
 * depending on the derived type of annotation. If side to
 * side merging of the sweep results occurs, the profile will
 * be an <tt>ENTITY_LIST</tt> of <tt>EDGEs</tt> and <tt>VERTEXs</tt> spanning the merge.
 * <br><br>
 * As with all inputs, the values are actually stored as
 * <tt>ATTRIB_TAGs</tt> which refer to the input entity.
 * @see ANNOTATION, ENTITY
 */
class DECL_SWP SWEEP_ANNOTATION : public ANNOTATION
{
protected:
    enum {
        e_path,
            e_profile,
            e_num_datums
    };
    /*
    // tbrv
    */
   /**
    * @nodoc
    */
    ANNOTATION_MAP_DECL(e_num_datums)
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif
   /*
    // tbrv
    */
   /**
    * @nodoc
    */
    ENTITY_FUNCTIONS(SWEEP_ANNOTATION, SWP)
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif
    // Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
    // explicitly here:
public:
   /**
    * C allocation constructor.
    */
    SWEEP_ANNOTATION(
        ENTITY* path = NULL,
        ENTITY* profile = NULL
        )
    {
        ents[e_path] = copy_ATTRIB_TAG(get_genesis_entity(path),this);
        ents[e_profile] = copy_ATTRIB_TAG(get_genesis_profile_ent(profile),this);
    }
   /**
     * Return the ATTRIB_TAG or EE_LIST of ATTRIB_TAGs for the path.
     */
    ENTITY* path() const { return ents[e_path]; }
   /**
     * If the path is a single EDGE, return it.  Otherwize return NULL.
     * Dereferencing thru the ATTRIB_TAG is done automatically.
     */
    EDGE* path_edge() const {
        ENTITY* actual = get_actual_live_entity_or_attrib_tag(ents[e_path]);
        return (EDGE*) (is_EDGE(actual) ? actual : NULL);
    }
   /**
     *If the path is a single VERTEX, return it.  Otherwize return NULL.
     * Dereferencing thru the ATTRIB_TAG is done automatically.
     */
    VERTEX* path_vertex() const {
        ENTITY* actual = get_actual_live_entity_or_attrib_tag(ents[e_path]);
        return (VERTEX*) (is_VERTEX(actual) ? actual : NULL);
    }
   /**
     * Return the ATTRIB_TAG or EE_LIST of ATTRIB_TAGs for the profile
     */
    ENTITY* profile() const { return ents[e_profile]; }
   /**
     * If the profile is a single EDGE, return it.  Otherwize return NULL.
     * Dereferencing thru the ATTRIB_TAG is done automatically.
     */
    EDGE* profile_edge() const;
   /**
    * If the profile is a single VERTEX, return it.  Otherwize return NULL.
    * Dereferencing thru the ATTRIB_TAG is done automatically.
    */
    VERTEX* profile_vertex() const;
   /**
    * @nodoc
    * Data changing -- Used by split and merge -- internal use only
    */
    void set_profile(ENTITY* e) { set_input_entity(ents[e_profile], e);	}
   /**
    * @nodoc
    */
    void add_path(ENTITY* e) { add_input_entity(ents[e_path], e); }
   /**
    * @nodoc
    */
    void add_profile(ENTITY* e) { add_input_entity(ents[e_profile], e); }
   /**
    * @nodoc
    * Notifications passed on via ATTRIB_ANNOTATION -- internal use only
    */
    virtual void split_member(
        ENTITY *,	// the member entity being split
        ENTITY *	// "new entity" arising from split
        ) {}
   /**
    * @nodoc
    */
    virtual void merge_member(
                            ENTITY *,   // the first entity being merged
                            ENTITY *,   // "other entity"
                            logical     // TRUE if first entity will be deleted
                            ) {}
};
/**
 * @nodoc
 */
extern DECL_SWP int SWEEP_ANNO_EDGE_TOP_TYPE;
/*
// tbrv
 */
/**
 * @nodoc
 */
#define SWEEP_ANNO_EDGE_TOP_LEVEL (SWEEP_ANNOTATION_LEVEL + 1)
/**
 * Defines the top topology from a swept edge, which is another edge.
 * <br>
 * <b>Role:</b> This class identifies the top (ending) topology created from a profile edge
 * during sweeping.
 * <br><br>
 * The top topology is generated at the far boundary of the sweep and
 * consists of the edges and vertices of a profile matching the input
 * profile.  There are separate derived classes for the top topology
 * corresponding to an edge of the profile and a vertex of the profile.
 * <dl>
 * <dt>Inputs:</dt>
 * <dd>Profile - edge</dd>
 * <dd>Path - edge</dd>
 * <dt>Outputs:</dt>
 * <dd>Top_edge - edge </dd>
 * </dl>
 * @see ANNOTATION, ENTITY
 */
class DECL_SWP SWEEP_ANNO_EDGE_TOP : public SWEEP_ANNOTATION
{
protected:
    enum {
        e_top_edge,
            e_num_datums
    };
public:
   /*
    // tbrv
    */
   /**
    * @nodoc
    */
    ANNOTATION_MAP_DECL(e_num_datums)
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif
   /*
    // tbrv
    */
   /**
    * @nodoc
    */
    ENTITY_FUNCTIONS(SWEEP_ANNO_EDGE_TOP, SWP)
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif
    // Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
    // explicitly here:
public:
   /**
    * C allocation constructor.
    */
    SWEEP_ANNO_EDGE_TOP(
        ENTITY* path = NULL,
        ENTITY* profile = NULL,
        EDGE* top_edge = NULL
        );
   /**
    * Specifies the edge to use as the top edge of sweeping.
    * <br><br>
    * @param e
    * edge for top.
    */
    void set_top_edge(EDGE* e) { set_output_entity(ents[e_top_edge], e); }
   /**
    * Removes unnecessary annotations on edge.
    * <br><br>
    * @param e
    * edge to remove.
    */
    void remove_top_edge(EDGE* e) { remove_output_entity(ents[e_top_edge], e); }
   /*
    // tbrv
    */
   /**
    * @nodoc
    */
    virtual void split_member(
                            ENTITY *,	// the member entity being split
                            ENTITY *	// "new entity" arising from split
        );
   /*
    // tbrv
    */
   /**
    * @nodoc
    */
    virtual void merge_member(
        ENTITY *,	// the first entity being merged
        ENTITY *,	// "other entity"
        logical		// TRUE if first entity will be deleted
        );
   /**
    * Returns a pointer to the top edge from sweeping.
    */
    ENTITY* top_edge() const { return ents[e_top_edge]; }
   /**
    * Determines whether or not the given edge is a top edge of the sweep operation.
    * <br><br>
    * @param entity
    * entity to test.
    */
    logical is_top(const ENTITY* entity) const { return contains_this_entity(ents[e_top_edge], entity); }
};
/**
 * @nodoc
 */
extern DECL_SWP int SWEEP_ANNO_EDGE_LAT_TYPE;
/*
// tbrv
 */
/**
 * @nodoc
 */
#define SWEEP_ANNO_EDGE_LAT_LEVEL (SWEEP_ANNOTATION_LEVEL + 1)

/**
 * Defines the annotation class for the lateral topology corresponding to an edge of the profile which becomes a face in sweeping.
 * <br>
 * <b>Role:</b> The lateral topology consists of the faces and edges filling the space between
 * the profile and the top topology. This identifies the lateral (side) topology created from a
 * profile edge during sweeping.
 * <dl>
 * <dt>Inputs:</dt>
 * <dd>Profile - edge</dd>
 * <dd>Path - edge</dd>
 * <dt>Outputs:</dt>
 * <dd>Lateral_face - face </dd>
 * </dl>
 * @see ANNOTATION, ENTITY
 */
class DECL_SWP SWEEP_ANNO_EDGE_LAT : public SWEEP_ANNOTATION
{
protected:
    enum {
        e_lateral_face,
            e_num_datums
    };
public:
   /*
    // tbrv
    */
   /**
    * @nodoc
    */
    ANNOTATION_MAP_DECL(e_num_datums)
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif
   /*
    // tbrv
    */
   /**
    * @nodoc
    */
    ENTITY_FUNCTIONS(SWEEP_ANNO_EDGE_LAT, SWP)
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif
    // Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
    // explicitly here:
public:
   /**
    * C allocation constructor.
    */
    SWEEP_ANNO_EDGE_LAT(
        ENTITY* path = NULL,
        ENTITY* profile = NULL,
        FACE* lateral_face = NULL
        );
   /*
    // tbrv
    */
   /**
    * @nodoc
    */
    virtual void split_member(
        ENTITY *,	// the member entity being split
        ENTITY *	// "new entity" arising from split
        );
   /*
    // tbrv
    */
   /**
    * @nodoc
    */
    virtual void merge_member(
        ENTITY *,	// the first entity being merged
        ENTITY *,	// "other entity"
        logical		// TRUE if first entity will be deleted
        );
   /**
    * Returns a pointer to the created lateral face from sweeping.
    */
    ENTITY* lateral_face() const { return ents[e_lateral_face]; }
};
/**
 * @nodoc
 */
extern DECL_SWP int SWEEP_ANNO_VERTEX_TOP_TYPE;
/*
 // tbrv
 */
/**
 * @nodoc
 */
#define SWEEP_ANNO_VERTEX_TOP_LEVEL (SWEEP_ANNOTATION_LEVEL + 1)
/**
 * Defines annotation relating to the vertices at the top of a sweep.
 * <br>
 * <b>Role:</b> This annotation identifies the top (ending) topology created from a profile
 * vertex during sweeping.
 * <br><br>
 * The top topology corresponding to a vertex of the profile may have
 * up to two edges and three vertices.  This occurs when sweeping
 * with positive draft and triangular faces must be generated to account
 * for the expansion of the profile as it is swept.  The left to
 * right ordering uses the right hand rule with the fingers following
 * the profile and the thumb pointing in the direction of the sweep.<br>
 * The knuckles are at the left and the tips of the fingers at the right.
 * <dl>
 * <dt>Inputs:</dt>
 * <dd>Profile - vertex</dd>
 * <dd>Path - edge</dd>
 * <dt>Outputs:</dt>
 * <dd>Right_top_edge - edge</dd>
 * <dd>Left_top_edge - edge</dd>
 * <dd>Right_top_vertex - vertex</dd>
 * <dd>Left_top_vertex - vertex</dd>
 * <dd>Mid_top_vertex - vertex<dd>
 * </dl>
 * @see ANNOTATION, ENTITY
 */
class DECL_SWP SWEEP_ANNO_VERTEX_TOP : public SWEEP_ANNOTATION
{
protected:
    enum {
        e_right_top_edge,
            e_left_top_edge,
            e_right_top_vertex,
            e_left_top_vertex,
            e_mid_top_vertex,
            e_num_datums
    };
   /*
    // tbrv
    */
   /**
    * @nodoc
    */
    ANNOTATION_MAP_DECL(e_num_datums)
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif
   /*
    // tbrv
    */
   /**
    * @nodoc
    */
    ENTITY_FUNCTIONS(SWEEP_ANNO_VERTEX_TOP, SWP)
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif
    // Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
    // explicitly here:
public:
   /**
    * C allocation constructor.
    */
    SWEEP_ANNO_VERTEX_TOP(
        ENTITY* path = NULL,
        ENTITY* profile_vertex = NULL,
        EDGE* right_top_edge = NULL,
        EDGE* left_top_edge = NULL,
        VERTEX* right_top_vertex = NULL,
        VERTEX* left_top_vertex = NULL,
        VERTEX* mid_top_vertex = NULL
        );
   /**
    * @nodoc
    */    
    logical is_top(const ENTITY* entity) const {
        return (
            contains_this_entity(ents[e_right_top_edge], entity) ||
            contains_this_entity(ents[e_left_top_edge], entity	) ||
            contains_this_entity(ents[e_right_top_vertex], entity) ||
            contains_this_entity(ents[e_left_top_vertex], entity) ||
            contains_this_entity(ents[e_mid_top_vertex], entity)
            );
    }
   /**
     * @nodoc
     */
    void set_mid_top_vertex(VERTEX* v) { set_output_entity(ents[e_mid_top_vertex], v); }
   /**
     * @nodoc
     */
    void set_right_top_vertex(VERTEX* v) { set_output_entity(ents[e_right_top_vertex], v); }
   /**
     * @nodoc
     */
    void set_left_top_vertex(VERTEX* v) { set_output_entity(ents[e_left_top_vertex], v); }
   /**
     * @nodoc
     */
    void set_right_top_edge(EDGE* v) { set_output_entity(ents[e_right_top_edge], v); }
   /**
     * @nodoc
     */
    void set_left_top_edge(EDGE* v) { set_output_entity(ents[e_left_top_edge], v); }
   /**
     * @nodoc
     * Adding data -- Used for annotating gap faces -- internal use only
     */    
    void add_right_top_edge(ENTITY* e) { add_output_entity(ents[e_right_top_edge], e); }
   /**
     * @nodoc
     */
    void add_left_top_edge(ENTITY* e) { add_output_entity(ents[e_left_top_edge], e); }
   /**
     * @nodoc
     */
    void add_right_top_vertex(ENTITY* e) { add_output_entity(ents[e_right_top_vertex], e); }
   /**
     * @nodoc
     */
    void add_left_top_vertex(ENTITY* e) { add_output_entity(ents[e_left_top_vertex], e); }
   /**
    * Remove unnecessary annotation on mid vertex
    */
    void remove_mid_top_vertex(VERTEX* v) { remove_output_entity(ents[e_mid_top_vertex], v); }
   /**
     * @nodoc
     * Notifications passed on via ATTRIB_ANNOTATION -- internal use only
     */
    virtual void split_member(
        ENTITY *,	// the member entity being split
        ENTITY *	// "new entity" arising from split
        );
   /**
     * @nodoc
     */
    virtual void merge_member(
        ENTITY *,	// the first entity being merged
        ENTITY *,	// "other entity"
        logical		// TRUE if first entity will be deleted
        );
   /**
     * @nodoc
     */    
    ENTITY* right_top_edge() const { return ents[e_right_top_edge]; }
   /**
     * @nodoc
     */
    ENTITY* left_top_edge() const { return ents[e_left_top_edge]; }
   /**
     * @nodoc
     */
    ENTITY* right_top_vertex() const { return ents[e_right_top_vertex]; }
   /**
     * @nodoc
     */
    ENTITY* left_top_vertex() const { return ents[e_left_top_vertex]; }
   /**
     * @nodoc
     */
    ENTITY* mid_top_vertex() const { return ents[e_mid_top_vertex]; }
};
/**
 * @nodoc
 */
extern DECL_SWP int SWEEP_ANNO_VERTEX_LAT_TYPE;
/*
// tbrv
 */
/**
 * @nodoc
 */
#define SWEEP_ANNO_VERTEX_LAT_LEVEL (SWEEP_ANNOTATION_LEVEL + 1)
/**
 * Defines annotation relating to vertex of lateral topology of a sweep.
 * <br>
 * <b>Role:</b> This annotation identifies the lateral (side) topology created from a profile
 * vertex during sweeping.
 * <br><br>
 * The lateral topology corresponding to a vertex of the profile may
 * have up to two faces and three edges.  This occurs when sweeping
 * with positive draft and triangular faces must be generated to account
 * for the expansion of the profile as it is swept.  The left to
 * right ordering uses the right hand rule with the fingers following
 * the profile and the thumb pointing in the direction of the sweep.
 * The knuckles are at the left and the tips of the fingers at the right.
 * <dl>
 * <dt>Inputs:</dt>
 * <dd>Profile - vertex</dd>
 * <dd>Path - edge</dd>
 * <dt>Outputs:</dt>
 * <dd>Right_lateral_face - face</dd>
 * <dd>Left_lateral_face - face</dd>
 * <dd>Right_lateral_edge - edge</dd>
 * <dd>Left_lateral_edge - edge</dd>
 * <dd>Mid_lateral_edge - edge </dd>
 * </dl>
 * @see ANNOTATION, ENTITY
 */
class DECL_SWP SWEEP_ANNO_VERTEX_LAT : public SWEEP_ANNOTATION
{
protected:
    enum {
        e_right_lateral_face,
            e_left_lateral_face,
            e_right_lateral_edge,
            e_left_lateral_edge,
            e_mid_lateral_edge,
            e_num_datums
    };
    /*
    // tbrv
    */
   /**
    * @nodoc
    */
    ANNOTATION_MAP_DECL(e_num_datums)
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif
 /*
   // tbrv
    */
   /**
    * @nodoc
    */
    ENTITY_FUNCTIONS(SWEEP_ANNO_VERTEX_LAT, SWP)
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif
    // Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
    // explicitly here:
public:
   /**
    * C allocation constructor.
    */
    SWEEP_ANNO_VERTEX_LAT(
        ENTITY* path = NULL,
        ENTITY* profile_vertex = NULL,
        FACE* right_lateral_face = NULL,
        FACE* left_lateral_face = NULL,
        EDGE* right_lateral_edge = NULL,
        EDGE* left_lateral_edge = NULL,
        EDGE* mid_lateral_edge = NULL
        );
/**
 * @nodoc
 * Data changing -- Used by split and merge -- internal use only
 */    
    
    void set_mid_lateral_edge(EDGE* e) { set_output_entity(ents[e_mid_lateral_edge], e); }
   /**
    * @nodoc
    */
    void set_right_lateral_edge(EDGE* e) { set_output_entity(ents[e_right_lateral_edge], e); }
   /**
    * @nodoc
    */
    void set_left_lateral_edge(EDGE* e) { set_output_entity(ents[e_left_lateral_edge], e); }
   /**
    * @nodoc
    * // Adding data -- Used for annotating gap faces -- internal use only
    */
    void add_right_lateral_face(ENTITY* e) { add_output_entity(ents[e_right_lateral_face], e); }
   /**
    * @nodoc
    */
    void add_left_lateral_face(ENTITY* e) { add_output_entity(ents[e_left_lateral_face], e); }
   /**
    * @nodoc
    */
    void add_right_lateral_edge(ENTITY* e) { add_output_entity(ents[e_right_lateral_edge], e); }
   /**
    * @nodoc
    */
    void add_left_lateral_edge(ENTITY* e) { add_output_entity(ents[e_left_lateral_edge], e); }
   /**
    * @nodoc
    * Notifications passed on via ATTRIB_ANNOTATION -- internal use only
    */
    virtual void split_member(
        ENTITY *,	// the member entity being split
        ENTITY *	// "new entity" arising from split
        );
   /**
    * @nodoc
    */
    virtual void merge_member(
        ENTITY *,	// the first entity being merged
        ENTITY *,	// "other entity"
        logical		// TRUE if first entity will be deleted
        );
   /**
    * 
    */
    ENTITY* right_lateral_face() const { return ents[e_right_lateral_face]; }
   /**
    * 
    */
    ENTITY* left_lateral_face() const { return ents[e_left_lateral_face]; }
   /**
    * 
    */
    ENTITY* right_lateral_edge() const { return ents[e_right_lateral_edge]; }
   /**
    * 
    */
    ENTITY* left_lateral_edge() const { return ents[e_left_lateral_edge]; }
   /**
    * 
    */
    ENTITY* mid_lateral_edge() const { return ents[e_mid_lateral_edge]; }
};
/**
 * @nodoc
 */
extern DECL_SWP int SWEEP_ANNO_END_CAPS_TYPE;
/*
// tbrv
 */
/**
 * @nodoc
 */
#define SWEEP_ANNO_END_CAPS_LEVEL (ANNOTATION_LEVEL + 1)

/**
 * Defines annotation relating to capping faces of a sweep.
 * <br>
 * <b>Role:</b> This annotation identifies the faces at the start and end of the sweep. It is
 * unique in that only output entities are available. One should process the information
 * immediately after the sweep, or at least before doing another sweep, so one can tell which
 * sweep it applies to.
 * <br><br>
 * This annotation gives information about the capping faces of sweep.
 * It is unique in that only output entities are available.  One should
 * process the information immediatly after the sweep, or at least before
 * doing another sweep, so one can tell which sweep it applies to.
 * <dl>
 * <dt>Inputs:</dt>
 * <dd>none</dd>
 * <dt>Outputs:</dt>
 * <dd>Start_face - face</dd>
 * <dd>End_face - face </dd>
 * </dl>
 * @see ANNOTATION, ENTITY
 */
class DECL_SWP SWEEP_ANNO_END_CAPS : public ANNOTATION
{
protected:
    enum {
        e_start_face,
            e_end_face,
            e_num_datums
    };
    /*
    // tbrv
    */
   /**
    * @nodoc
    */
    ANNOTATION_MAP_DECL(e_num_datums)
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif
          /*
          // tbrv
    */
   /**
    * @nodoc
    */
    ENTITY_FUNCTIONS(SWEEP_ANNO_END_CAPS, SWP)
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif
    // Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
    // explicitly here:
public:
    
   /**
    * C allocation constructor.
    */
    SWEEP_ANNO_END_CAPS(
        FACE* start_face = NULL,
        FACE* end_face = NULL
        )
    {
        ents[e_start_face] = start_face;
        ents[e_end_face] = end_face;
        //hook_members();
    }
   /**
    * @nodoc
    * Notifications passed on via ATTRIB_ANNOTATION -- internal use only
    */
    virtual void split_member(
        ENTITY *,	// the member entity being split
        ENTITY *	// "new entity" arising from split
        );
   /**
    * @nodoc
    */ 
    virtual void merge_member(
        ENTITY *,	// the first entity being merged
        ENTITY *,	// "other entity"
        logical		// TRUE if first entity will be deleted
        );
   /**
     *
     */
    FACE* start_face() const { return (FACE*) ents[e_start_face]; }
   /**
     *
     */
    FACE* end_face() const { return (FACE*) ents[e_end_face]; }
    
    // Data changing -- internal use only
   /**
    * @nodoc
    */ 
    void set_start_face(FACE* f) { set_output_entity(ents[e_start_face], f); }
   /**
    * @nodoc
    */ 
    void set_end_face(FACE* f) { set_output_entity(ents[e_end_face], f); }
};

//-------------------------------------------------------------------
// Derived Class: Miter Edges
//-------------------------------------------------------------------
extern DECL_SWP int SWEEP_ANNO_EDGE_MITER_TYPE;
/*
// tbrv
 */
/**
 * @nodoc
 */
#define SWEEP_ANNO_EDGE_MITER_LEVEL  (SWEEP_ANNOTATION_LEVEL + 1)

/**
 * Defines annotation relating to miter edges created during sweep sweep.
 * <br>
 * <b>Role:</b> This annotation identifies the miter edges created during sweep. It is
 * unique in that only output entities are available. One should process the information
 * immediately after the sweep, or at least before doing another sweep, so one can tell which
 * sweep it applies to.
 * <br><br>
 * This annotation gives information about the miter edges of sweep.
 * It is unique in that only output entities are available.  One should
 * process the information immediatly after the sweep, or at least before
 * doing another sweep, so one can tell which sweep it applies to.
 * <dl>
 * <dt>Inputs:</dt>
 * <dd>none</dd>
 * <dt>Outputs:</dt>
 * <dd>Miter_edge - edge</dd>
 * </dl>
 * @see ANNOTATION, ENTITY
 */
class DECL_SWP SWEEP_ANNO_EDGE_MITER : public SWEEP_ANNOTATION
{
protected:
    enum {
        e_miter_edge,
            e_num_datums
    };
    /*
    // tbrv
    */
   /**
    * @nodoc
    */
    ANNOTATION_MAP_DECL(e_num_datums)
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif
   /*
   // tbrv
    */
   /**
    * @nodoc
    */
    ENTITY_FUNCTIONS(SWEEP_ANNO_EDGE_MITER, SWP)
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif
    // Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
    // explicitly here:
public:
    
   /**
    * C allocation constructor.
    */
    SWEEP_ANNO_EDGE_MITER(
        EDGE* miter_edge = NULL
        )
    {
        ents[e_miter_edge] = miter_edge;
        //hook_members();
    }
   /**
    * @nodoc
    */ 
    // Notifications passed on via ATTRIB_ANNOTATION -- internal use only
    virtual void split_member(
        ENTITY *,	// the member entity being split
        ENTITY *	// "new entity" arising from split
        );
   /**
    * @nodoc
    */ 
    virtual void merge_member(
        ENTITY *,	// the first entity being merged
        ENTITY *,	// "other entity"
        logical		// TRUE if first entity will be deleted
        );
   /**
    * 
    */ 
    EDGE* miter_edge() const { return (EDGE*) ents[e_miter_edge]; }
   /**
    * @nodoc
    * Data changing -- internal use only
    */ 
    void set_miter_edge(EDGE* e) { set_output_entity(ents[e_miter_edge], e); }
};


//-------------------------------------------------------------------
// Derived Class: Miter Vertices
//-------------------------------------------------------------------
extern DECL_SWP int SWEEP_ANNO_VERTEX_MITER_TYPE;
/*
// tbrv
 */
/**
 * @nodoc
 */
#define SWEEP_ANNO_VERTEX_MITER_LEVEL (SWEEP_ANNOTATION_LEVEL + 1)

/**
 * Defines annotation relating to miter vretices created during sweep sweep.
 * <br>
 * <b>Role:</b> This annotation identifies the miter vertices created during sweep. It is
 * unique in that only output entities are available. One should process the information
 * immediately after the sweep, or at least before doing another sweep, so one can tell which
 * sweep it applies to.
 * <br><br>
 * This annotation gives information about the miter vertices of sweep.
 * It is unique in that only output entities are available.  One should
 * process the information immediatly after the sweep, or at least before
 * doing another sweep, so one can tell which sweep it applies to.
 * <dl>
 * <dt>Inputs:</dt>
 * <dd>none</dd>
 * <dt>Outputs:</dt>
 * <dd>Miter_vertex - vertex</dd>
 * </dl>
 * @see ANNOTATION, ENTITY
 */
class DECL_SWP SWEEP_ANNO_VERTEX_MITER : public SWEEP_ANNOTATION
{
protected:
    enum {
        e_miter_vertex,
            e_num_datums
    };
    /*
    // tbrv
    */
   /**
    * @nodoc
    */
    ANNOTATION_MAP_DECL(e_num_datums)
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif
  /*
  // tbrv
    */
   /**
    * @nodoc
    */
    ENTITY_FUNCTIONS(SWEEP_ANNO_VERTEX_MITER, SWP)
#if 0
        ; // semicolon needed for mkman (doc tool) parsing)
#endif
    // Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
    // explicitly here:
public:
    
   /**
    * C allocation constructor.
    */
    SWEEP_ANNO_VERTEX_MITER(
        VERTEX* miter_vertex = NULL
        )
    {
        ents[e_miter_vertex] = miter_vertex;
    }
   /**
    * @nodoc
    * Notifications passed on via ATTRIB_ANNOTATION -- internal use only
    */    
    virtual void split_member(
        ENTITY *,	// the member entity being split
        ENTITY *	// "new entity" arising from split
        );
   /**
    * @nodoc
    */    
    virtual void merge_member(
        ENTITY *,	// the first entity being merged
        ENTITY *,	// "other entity"
        logical		// TRUE if first entity will be deleted
        );
   /**
    * @nodoc
    */
    VERTEX* miter_vertex() const { return (VERTEX*) ents[e_miter_vertex]; }
   /**
    * @nodoc
    * Data changing -- internal use only
    */
    void set_miter_vertex(VERTEX* v) { set_output_entity(ents[e_miter_vertex], v); }
};
/*
// tbrv
 */
/**
 * @nodoc
 */
void cleanup_extra_top_entities( ENTITY_LIST &edge_list );

// Function used only for resetting top annotations after face removed
// by api_remove_face.
/*
// tbrv
 */
/**
 * @nodoc
 */
void annotate_after_face_removed(BODY *degen_body);
/** @} */
#endif //__SWP_ANNO_HXX__
