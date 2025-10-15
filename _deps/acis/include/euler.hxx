/* ORIGINAL: acis2.1/kerndata/euler/euler.hxx */
/* $Id: euler.hxx,v 1.10 2002/08/09 17:21:13 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for Euler routines.

// These routines provide some Euler operations for solids.
// These deal only with simple cases for the moment.

#if !defined( EULER_HDR_DEF )
#define  EULER_HDR_DEF

#include <stdio.h>
#include "base.hxx"
#include "dcl_eulr.h"
#include "logical.h"

class COEDGE;
class VERTEX;
class EDGE;
class LOOP;
class FACE;
class SHELL;
class LUMP;
class BODY;
class CURVE;
class PCURVE;
class APOINT;
class SURFACE;
class WIRE;
class SPAposition;
class ENTITY_LIST;

//	Description of minob:
//
//		Euler op to make a minimal body composed of a body, lump, 
//		shell, face, loop, edge, coedge, vertex and point.
//		Only the vertex has any geometry, i.e. a point set to the
//		given SPAposition.
//
//	Arguments:
//		SPAposition const &pos
//
//	Returns:
//		BODY *
//
//	Errors:
//

DECL_EULR BODY * minob( SPAposition const & );

/*
* @note 生成一个仅包含一个点的body
* @return 生成的body
* @param pos
* body包含的顶点的位置
*/
DECL_EULR BODY* gme_minob(SPAposition const&);

//	Description of minlump:
//
//	Make a minimal lump composed of a vertex, coedge, edge, loop,
//	face, shell and lump.
//	The isolated vertex is characterised by having a coedge with 
//	next and previous coedge pointers referring to the coedge and an
//	edge with null curve pointer.
//
//	Arguments:
//		SPAposition const&	SPAposition at which new vertex is to lie
//		BODY *			body that will own the new lump
//
//	Returns:
//		LUMP *			pointer to lump created
//
//	Errors:
//

DECL_EULR LUMP * minlump(SPAposition const &, BODY *);
/*
* @note 生成一个仅包含一个点的lump，将其添加到一个指定的body中
* @return 生成的lump
* @param pos
* lump包含的顶点的位置
* @param body
* 生成的lump所属的body
*/
DECL_EULR LUMP* gme_minlump(SPAposition const& pos, BODY* body);

//	Description of aded:
//
//		A new edge is added from the end of prev_coedge to the start of
//		next_coedge.  Both coedges must belong to the same face (error
//		given if not).	If both coedges belong to the same loop, a new
//		face will be created and added to the body shell (subshells
//		ignored for the present).  The calling routine should supply a
//		new surface oriented so that it is in the FORWARD sense with
//		respect to the face (which lies to the left of the new edge).
//		If the given coedges belong to different loops of the face, the
//		loops will be coalesced into one.
//
//		The routine CHECKS that prev_coedge belongs to a loop (i.e. not
//		to a wire) and that prev_coedge and next_coedge belong to the
//		same face.
//
//		It ASSUMES the geometry of the given surface and curve is
//		consistent.
//
//		Trims the given curve to the edge; trims and reparameterises
//		the given left pcurve (if an explicit pcurve) to match the
//		curve; uses the right pcurve as given.
//
//	Arguments:
//		COEDGE*		prev_coedge		new edge starts at end of
//									prev_coedge and is adjacent
//									(moving clockwise about end)
//		COEDGE*		next_coedge		new edge ends at start of
//									next_coedge and is adjacent
//									(moving counterclockwise about start)
//		CURVE*		cur				curve given to new edge (can
//									be null)
//		SURFACE*	surf			surface given to new face (can
//									be null), the new face (if created)
//                                  is in the same sense as the surface.
//		PCURVE*		pcur_left		pcurve for left coedge of new edge
//									(can be null)
//		PCURVE*		pcur_right		pcurve for right coedge of new edge
//									(can be null)
//		logical		no_trim			Suppress the trimming of
//									underlying edge geometry.  Useful
//									if we want to extend this edge's
//									curve later, as in blending.
//      ENTITY*     new_faces       new faces created 
//      logical     assume_valid    Assume input face is valid, so that
//                                  geometrical tests can be used.
//
//	Returns:
//		COEDGE*						new (forward) coedge of new edge
//
//	Errors:
//		error1		prev_coedge is not owned by a loop
//		error2		prev_coedge and next_coedge do not belong to the
//					same face
//

DECL_EULR COEDGE* aded(
	COEDGE*,
	COEDGE*,
	CURVE* = NULL,
	SURFACE* = NULL,
	PCURVE* = NULL,
	PCURVE* = NULL,
	logical = FALSE,
    ENTITY_LIST * new_faces = NULL,
    logical assume_valid = TRUE
);
/*
* @brief 在两条给定的coedge之间添加一条带有两个coedge的edge，必须保证两条给定的coedge在同一个face上。若两条给定的coedge在同一个loop中，则生成新的face，新的face位于新添加的edge的左侧
* @return 添加的两条coedge中与edge方向相同的一条
* @param prev_coedge
* 新加入的edge将把prev_coedge的end端点作为其start
* @param next_coedge
* 新加入的edge将把next_coedge的start端点作为其start
* @param cur
* 新加入的edge的几何属性
* @param surf
* 对于可能新增的face，它的几何属性.
* @param pcur_left
* 与新加入的edge同向的coedge的几何属性
* @param pcur_right
* 与新加入的edge反向的coedge的几何属性
* @param no_trim
* 是否修剪新加入的edge（意义不明，暂时没用上）
* @param new_faces
* 若不为空，则将可能新生成的face加入到链表的末尾
* @param assum_valid
* 调用者是否保证输入的coedge所在的face几何属性存在（暂时没用）
*/
DECL_EULR COEDGE* gme_aded(
	COEDGE* prev_coedge,
	COEDGE* next_coedge,
	CURVE* cur = NULL,
	SURFACE* surf = NULL,
	PCURVE* pcur_left = NULL,
	PCURVE* pcur_right = NULL,
	logical no_trim = FALSE,
	ENTITY_LIST* new_faces = NULL,
	logical assume_valid = TRUE
);


//	Description of adev:
//
//		The routine expects to be given a point for the end of the edge
//		and	a curve along which the new edge will lie (with sense
//		FORWARD).	The	routine returns the coedge of the new edge,
//		adjacent to the given coedge, that is, adjacent (going
//		clockwise) to the end of the given coedge.
//
//		The routine ASSUMES the given coedge belongs to an embedded
//		edge of	a solid	or to an edge, internal or external, of a sheet
//		(the new edge will always be internal).
//
//		If the given point is the SAME point as the point of the end 
//		vertex of prev_coedge, assumes a geometrically closed edge is
//		wanted (edge constructor adjusts edge end SPAparameter accordingly).
//
//		Trims the given curve to the edge; trims and reparameterises
//		the given left pcurve (if an explicit pcurve) to match the
//		curve; uses the right pcurve as given.
//
//	Arguments:
//		COEDGE*		prev_coedge	coedge at the end of which the new edge
//								is added
//		APOINT*		point		point at which end of new edge lies
//		CURVE*		cur			curve along which new edge lies (can be
//								null)
//		SURFACE*	surf		surface given to new face (can
//								be null)
//		PCURVE*		pcur_left	pcurve for left coedge of new edge
//								(can be null)
//		PCURVE*		pcur_right	pcurve for right coedge of new edge
//								(can be null)
//		logical		no_trim		Suppress the trimming of
//								underlying edge geometry.  Useful
//								if we want to extend this edge's
//								curve later, as in blending.
//
//	Returns:
//		COEDGE*					new (forward) coedge of new edge
//
//	Errors:
//		none
//

DECL_EULR COEDGE* adev(
	COEDGE*,
	APOINT*,
	CURVE*,
	SURFACE* = NULL,
	PCURVE* = NULL,
	PCURVE * = NULL,
	logical = FALSE
);

/*
* @brief 在一条给定的coedge的end端点和给定的point位置之间添加一条edge，edge方向由coedge的end端点指向给定的point位置，若给定的point与coedge的end端点重合，则生成一条起点与终点重合的封闭edge.函数默认添加的edge是一条嵌入边，即prev_coedge是在face中的coedge。
* @return 添加的两条coedge中与edge方向相同的一条
* @param prev_coedge
* 新加入的edge将把prev_coedge的end端点作为其start
* @param point
* 函数将在point位置处新建一个端点vertex，并将其作为新加入的edge的end
* @param cur
* 新加入的edge的几何属性
* @param surf
* 对于可能新增的face，它的几何属性（意义不明，目前实验的情况没有发现会生成新的face，嵌入边理论上不应该关联任何新的face，因此这个参数没用上）
* @param pcur_left
* 与新加入的edge同向的coedge的几何属性
* @param pcur_right
* 与新加入的edge反向的coedge的几何属性
* @param no_trim
* 后续是否会修剪新加入的edge的几何属性（意义不明，暂时没用上）
*/
DECL_EULR COEDGE* gme_adev(
	COEDGE* prev_coedge,
	APOINT* point,
	CURVE* cur,
	SURFACE* surf = NULL,
	PCURVE* pcur_left = NULL,
	PCURVE* pcur_right = NULL,
	logical no_trim = FALSE
);

//	Description:
//
//		Euler op to kill a non-prop coedge and edge and faces and
//		(single) loop of the coedge.
//
//	Arguments:
//		COEDGE*		edge
//
//	Returns:
//		void
//
//	Errors:
//		face being lost has more than one loop

DECL_EULR void kefl( COEDGE* );

/*
* @brief 删除2-manifold形体的一条edge及其下属的拓扑结构，并将这条边所在的两个面进行合并
* @return
* @param coedge
* 待删除的edge的一条coedge
*/
DECL_EULR void gme_kefl(COEDGE* coedge);

//	Description of kemkl:
//
//		Given a edge that is known to be a prop, that is, 
//		has the same loop to either side (i.e. is embedded),
//		deletes the edge, its coedge and partner coedge, and makes
//		a new loop in the face.  The new loop contains the end
//		vertex of the coedge.  If either end of the edge is a spur,
//		the spur vertex remains as an isolated vertex (i.e. a vertex
//		surrounded by an edge (with both ends referring to the vertex
//		and geometry set to null) and coedge (with next and previous
//		coedges being itself).  Detects but does not yet handle the 
//		case where both ends are spurs.
//
//	Arguments:
//		COEDGE*		coedge
//
//	Returns:
//		LOOP* 		(new loop made)
//
//	Errors:
//		Makes no checks except for double spur vertex case - 
//		caller must ensure edge is a prop i.e. has same loop 
//		to either side.
 
DECL_EULR LOOP* kemkl( EDGE* );



//	Description of kev:
//
//		Euler op to kill an embedded wire edge (i.e. an edge with 
//		both its coedges referring to the same loop) and spur 
//		vertex (at end of given coedge)
//
//	Arguments:
//		COEDGE*		coedge
//
//	Returns:
//		void
//
//	Errors:
//		Makes no checks - caller must ensure edge is an embedded
//		 wire with a spur and must supply the correct coedge

DECL_EULR void kev(COEDGE*);



//	Description of ke:
//
//		Euler op to kill a wire edge and either kill a spur 
//		vertex or create a loop (if wire is a bridge)
//
//	Arguments:
//		EDGE*		edge
//
//	Returns:
//		LOOP*		new loop
//
//	Errors:
//		Makes no checks - caller must ensure edge is a wire

DECL_EULR LOOP* ke(EDGE*);

/*
* @brief 删除一条两个coedge均在同一个面的边（多余的边, spur edge or bridge edge）
* @return 删除bridge edge使面上增加新的环
* @param edge
* 待删除的边
*/
DECL_EULR LOOP* gme_ke(EDGE*);


//	Description of kv:
//		Remove an isolated vertex.
//
//	Arguments:
//		VERTEX*		vertex
//
//	Returns:
//		void
//
//	Errors:
//		Makes no checks - caller must ensure vertex is isolated

DECL_EULR void kv(VERTEX*);

/*
* @note 删除拓扑结构中的一个孤立点(函数本身不判断点的属性，调用者需要保证该顶点是孤立的)
* @return
* @param vertex
* 待删除的孤立点
*/
DECL_EULR void gme_kv(VERTEX*);

//	Description of kf:
//
// Operation to kill a face thereby changing a closed set of
// faces into an open set or diminishing an already open set,
// or leaving a possibly partly-covered wire body.

// It removes a face together with its loops and coedges, except
// that if removing a coedge from an edge would leave the 
// edge with no coedge, i.e. the edge is to become a wire edge,
// it may be kept as a wire edge, depending on the setting of
// the logical argument.
//
//	Arguments:
//		FACE*		face
//		logical		discard_wire_edges (default FALSE)
//
//	Returns:
//		void
//
//	Errors:
//		

DECL_EULR void kf( FACE *, logical = FALSE );
/*
* @brief 移除一个面，会使得封闭的形体变为开放的，或者留下一个被部分覆盖的带有wire的body。会移除面上的loop与coedge，如果移除掉coedge会导致该edge不关联任何的coedge，则根据函数的discard_wire_edges参数决定是否保留这条edge，这条edge将被视为一个wire的一部分。
* @return
* @param face 被移除的面
* @param discard_wire_edges 是否移除删除面后不与任何coedge关联的edge，该选项默认为FALSE，可用于移除面后保留面的框架
*/
DECL_EULR void gme_kf(FACE*, logical = FALSE);
//	Description of coedge_start_at_apex:
//
//		Routine to test the vertex at the start of the given coedge
//	to see if it lies at the apex of a cone or torus or at a
//	singularity of a spline surface.
//
//	Arguments:
//		COEDGE*		coedge
//
//	Returns:
//		LOGICAL
//
//	Errors:

DECL_EULR logical coedge_start_at_apex(COEDGE*);
/*
* @brief 判断该顶点是否为圆锥、圆环面、样条面的奇点
* @return
* @param vertex
* 待判断的顶点
*/
DECL_EULR logical gme_coedge_start_at_apex(COEDGE*);
//	Description of cone_apex:
//
//		Routine to test a vertex to see if it lies at the 
//		apex of a cone.
//
//	Arguments:
//		VERTEX*		vertex
//		FACE*		face
//
//	Returns:
//		LOGICAL
//
//	Errors:

DECL_EULR logical cone_apex(VERTEX*, FACE* = NULL);

/*
* @brief 判断该顶点是否为圆锥顶点
* @return
* @param vertex
* 待判断的顶点
*/
DECL_EULR logical gme_cone_apex(VERTEX* vertex);
//	Description of spline_apex:
//
//		Routine to test the vertex at the start of the given coedge
//	to see if it lies at a singularity of a spline surface.
//
//	Arguments:
//		COEDGE*		coedge
//
//	Returns:
//		LOGICAL
//
//	Errors:

DECL_EULR logical spline_apex(COEDGE*);
/*
* @brief 判断该顶点是否为样条面的奇点
* @return
* @param vertex
* 待判断的顶点
*/
DECL_EULR logical gme_spline_apex(COEDGE* coedge);
//	Description of torus_apex:
//
//		Routine to test a vertex to see if it lies at the 
//		apex of a (lemon or apple) torus
//
//	Arguments:
//		VERTEX*		vertex
//		FACE*		face
//
//	Returns:
//		LOGICAL
//
//	Errors:

DECL_EULR logical torus_apex(const VERTEX*, FACE* = NULL) ;

/*
* @brief 判断该顶点是否为圆环面的奇点
* @return
* @param vertex
* 待判断的顶点
*/
DECL_EULR logical gme_torus_apex(const VERTEX*);


//	Description of shke:
//
//		Euler op to kill a sheet edge and its vertices, leaving 
//		its neighbouring edges, if any, unbounded.
//
//	Arguments:
//		EDGE*		edge
//
//	Returns:
//		void
//
//	Errors:
//		Makes no checks - caller must ensure edge is an external
//		edge of a sheet.

DECL_EULR void shke(EDGE*);

DECL_EULR void trim_edge_geom(EDGE *);

DECL_EULR void
eulr_merge_bodies( BODY*, BODY* );

DECL_EULR void
eulr_merge_lumps( LUMP*, LUMP* );

DECL_EULR void
eulr_merge_shells( SHELL*, SHELL* );

////////
//Euler opts detele face merge edges , delete loop merge edges and 
//delete edge merge vertices
////////
DECL_EULR logical
check_eulr_delete_edge_merge_vertices( EDGE*);

DECL_EULR logical
check_eulr_delete_loop_merge_edges( LOOP*);

DECL_EULR logical
check_eulr_delete_face_merge_edges( FACE*);


DECL_EULR 
VERTEX *eulr_delete_edge_merge_vertices( EDGE*, VERTEX* = NULL);

DECL_EULR 
EDGE *eulr_delete_face_merge_edges ( FACE*, EDGE* = NULL);

DECL_EULR
EDGE *eulr_delete_loop_merge_edges ( LOOP* , EDGE* = NULL);

#endif
