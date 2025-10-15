/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef PE_MANAG_INCLUDE
#define PE_MANAG_INCLUDE

#include "dcl_ihl.h"

#include "meshmg.hxx"
#include "meshat.hxx"
#include "pe_mesh.hxx"

/**
 * @file pe_manag.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup IHLAPI
 *  \brief Declared at <pe_manag.hxx>
 *  @{
 */

/**
 * @nodoc
 */

#if defined ( ACIS_FACETER )
    #define V_ENTITY    ENTITY
    #define V_FACE      FACE
    #define V_EDGE      EDGE
    #define V_VERTEX    VERTEX
#else
    #define V_ENTITY    void
    #define V_FACE      void
    #define V_EDGE      void
    #define V_VERTEX    void
#endif


// These defines define the application ids that are used when an
// an PE_MESH is attached to a FACE

#define PE_MESH_APP 0x00494459
#define PE_MESH_ID 0x00494459

// the following flags may be useful in future
// they can be set by the new function:  UpdateOutputFlag(uint flgs)
#define   NEED_NODE_EXT 1     // TRUE: mesh with additional node information
#define   NOT_GLOBAL    2     // TRUE: use the indexed protocoll
#define   NO_ENTITY_PTR 4     // TRUE: the pointer given by faceter
                              //       are  N O T   ENTITY pointer

// to use this flags the faceter (or any other calling system)
// has to call / provide some new functions:
// NEED_NODE_EXT: the calling system must
//                  - set the flag NEED_NODE_EXT ( by UpdateOutputFlag() )
//                  - during facetting you can ask this flag: need_node_ext()
//                  - calculate the NODE_EXT data
//                  - call the new function:   AnnounceGlobalNode(...)
// NOT_GLOBAL:    the calling system must
//                  - set the flag NOT_GLOBAL ( by UpdateOutputFlag() )
//                  - provide a global: announce_counts(...) function
//                    (for we allocate after this call the mesh arrays
//                     in one step.)
// NO_ENTITY_PTR: the calling system must
//                  - set the flag NO_ENTITY_PTR( by UpdateOutputFlag() )
//                    (then the mesh and hidden line ignore this pointer)

/************************************************************************/

/**
 * Defines a mesh manager for interactive hidden line removal (IHL).
 * <br>
 * <b>Role:</b> The <tt>PE_MESH_MANAGER</tt> class is a mesh manager that
 * uses the global indexed protocol. It creates a @href POLYEDGE_MESH that
 * can be attached to an entity.
 */
class DECL_IHL PE_MESH_MANAGER : public MESH_MANAGER
{
private:
    POLYEDGE_MESH *mesh;
    void          *aktface;         // FACE
    int           dbl_sided;
    int           last_edg;

#if ! defined ( OLD_APP )
    uint          flags;    // = [ NEED_NODE_EXT ] [ | ... ]
#endif

    // node output without NODE_EXT
    virtual void * AnnouncePureNode(int inode, void *ptr, const SPAposition &X)
    {
        mesh->SetNode(inode, ptr, X);
        return ( (void*) INTEXTEND (inode) );
    }

public:

	/**
	 * Initializes the private data of this <tt>PE_MESH_MANAGER</tt>.
	 */
    void Init(POLYEDGE_MESH *aktmesh);

	/**
	 * C++ constructor.
	 */
#if defined ( OLD_APP )
    PE_MESH_MANAGER(POLYEDGE_MESH *aktmesh = NULL)   { Init(aktmesh); }
#else
    PE_MESH_MANAGER(POLYEDGE_MESH *aktmesh = NULL, uint flgs = 0)
    {
        Init(aktmesh);
        flags = flgs;
        set_app_id(PE_MESH_APP);
        set_user_id(PE_MESH_ID);
    }
#endif

	/**
	 * C++ destructor.
	 */
    ~PE_MESH_MANAGER()   {}

	/**
	 * Returns the application identification number of the mesh.
	 */
	MESH_APP_ID get_app_id() {return MESH_MANAGER::get_app_id();}

	/**
	 * Returns the user identification number of the mesh.
	 */
	MESH_USER_ID get_user_id() {return MESH_MANAGER::get_user_id();}

	/**
	 * Specifies the application identification number of the mesh.
	 * <br>
	 * @param appid
	 * id to use.
	 */
	void set_app_id(MESH_APP_ID appid) {MESH_MANAGER::set_app_id(appid);}

	/**
	 * Specifies the user identification number of the mesh.
	 * <br>
	 * @param userid
	 * id to use.
	 */
	void set_user_id(MESH_USER_ID userid) {MESH_MANAGER::set_user_id(userid);}

#if ! defined ( OLD_APP )
	/**
	 * @nodoc
	 */
    void UpdateOutputFlags(uint flgs)   { flags = flgs; }
#endif

	/**
	 * @nodoc
	 */
    void SetOutputMesh(POLYEDGE_MESH *aktmesh)  { Init(aktmesh); }

	/**
	 * @nodoc
	 */
    POLYEDGE_MESH * GetOutputMesh()   { return(mesh); }

	/**
	 * Used with the indexed protocol to flag requested data.
	 * <br>
	 * <b>Role:</b> If this function returns <tt>TRUE</tt> each polygon node will be output
	 * together with the corresponding coedge, coordinates and surface normal.
	 * This member function always returns <tt>TRUE</tt>.
	 */
   virtual logical need_indexed_polynode_with_data()   { return TRUE; } // later may be do like this->  flags&NEED_NODE_EXT

	/**
	 * If this function returns <tt>TRUE</tt>, the facets will be output with the global indexed protocol.
	 * <br>
	 * <b>Role:</b> This member function always returns <tt>TRUE</tt>.
	 */
    virtual logical need_global_indexed_polygons()   { return TRUE; }

	/**
	 * If this function returns <tt>TRUE</tt>, the number of polygons, nodes, and node references by polygons are announced before other output.
	 * <br>
	 * <b>Role:</b> This member function always returns <tt>TRUE</tt>.
	 */
    virtual logical need_precount_of_global_indexed_polygons() { return TRUE; }

	/**
	 * Used with the indexed protocol to flag requested data.
	 * <br>
	 * <b>Role:</b> If this function returns <tt>TRUE</tt>, each polygon node that lies on a
	 * model edge will be output together with the corresponding coedge. 
	 * This member function always returns <tt>TRUE</tt>.
	 */
    virtual logical need_coedge_pointers_on_polyedges() { return TRUE; }
 
	/**
	 * Returns <tt>TRUE</tt> if you are only interested in getting approximate counts of polygons, nodes and edges.
	 * <br>
	 * <b>Role:</b> This can save time but may use more memory.
	 * This member function always returns <tt>FALSE</tt>.
	*/
    virtual logical need_approx_counts() { return FALSE; }

	/**
	 * Announces the number of polygons, nodes, and node references by polygons.
	 * <br>
	 * <b>Role:</b> Used with the coordinate, indexed and global indexed protocols.
	 * <br>
	 * @param npoly
	 * number of polygons.
	 * @param nnode
	 * number of nodes.
	 * @param npolynode
	 * number of node references.
	 */
    virtual void announce_counts(int npoly, int nnode, int npolynode)
    {
#if defined ( OLD_APP )
        mesh->InitMesh(npoly, npolynode, nnode);
#else
        // if the flag: NEED_NODE_EXT is set, addionally an array
        // of PE_NODE_EXT[nnode] will be allocated.
        mesh->InitMesh(npoly, npolynode, nnode, flags&NEED_NODE_EXT);
#endif
    }

	/**
	 * This is called to reannounce the exact number of nodes and polygons that were already announced.
	 * <br>
	 * <b>Role:</b> This is useful if user used <tt>need_approx_counts()</tt> and made it return <tt>TRUE</tt>.
	 * <br>
	 * @param npoly
	 * number of polygons to follow.
	 * @param nnode
	 * number of nodes to follow.
	 * @param npolynode
	 * number of nodes when counted each time used by a polygon.
	 */
    virtual void reannounce_counts(int npoly,int nnode,int npolynode)
    {
        mesh->ResetMeshCounts(npoly, npolynode, nnode);
    }

    // the following functions get ENTITY pointer from the current faceter

    // current node output functions without information about NODE_EXT

	/**
	 * Announces a node on a model <tt>VERTEX</tt>.
	 * <br>
	 * <b>Role:</b> Used with the global indexed protocol.
	 * <br>
	 * @param inode
	 * 0-based node index.
	 * @param vertex
	 * the model vertex.
	 * @param X
	 * node coordinates.
	 */
    virtual void *announce_global_node(int            inode,
                                       V_VERTEX       *vertex,
                                       const SPAposition &X )
    {
        return ( AnnouncePureNode(inode, vertex, X) );
    }

	/**
	 * Announces a node on a model <tt>EDGE</tt> with its curve parameter.
	 * <br>
	 * <b>Role:</b> Used with the global indexed protocol.
	 * <br>
	 * @param inode
	 * 0-based node index.
	 * @param edge
	 * the model edge.
	 * @param X
	 * node coordinates.
	 * @param t
	 * curve parameter.
	 */
    virtual void *announce_global_node(int            inode,
                                       V_EDGE         *edge,
                                       const SPAposition &X,
                                       double         t )
    {
        t;   // C4705 statement has no effect
        return ( AnnouncePureNode(inode, edge, X) );
    }

	/**
	 * Announces a node interior to a model <tt>FACE</tt> with its surface parameters.
	 * <br>
	 * <b>Role:</b> Used with the global indexed protocol.
	 * <br>
	 * @param inode
	 * 0-based node index.
	 * @param face
	 * the model face.
	 * @param X
	 * node coordinates.
	 * @param uv
	 * surface parameters.
	 */
    virtual void *announce_global_node(int           inode,
                                       V_FACE        *face,
                                       const SPAposition &X,
                                       const SPApar_pos  &uv )
    {
        uv;   // C4705 statement has no effect
        return ( AnnouncePureNode(inode, face, X) );
    }

	/**
	 * Announces the beginning of global mesh output.
	 * <br>
	 * <b>Role:</b> The entity is the top level entity of the entire mesh,
	 * typically a <tt>BODY</tt>, but can be a <tt>LUMP</tt>, a <tt>SHELL</tt>, or a <tt>FACE</tt>. Used with the global
	 * indexed protocol. 
	 * <br>
	 * This member function does nothing.
	 * <br>
	 * @param ent
	 * top level entity.
	 */
    virtual void begin_global_mesh_output(V_ENTITY *ent)
    {
        ent;   // C4705 statement has no effect
    }

	/**
	 * Terminates global mesh output.
	 * <br>
	 * <b>Role:</b> Used with the global indexed protocol.
	 * <br>
	 * @param ent
	 * top level entity being faceted.
	 */
    virtual void end_global_mesh_output(V_ENTITY *ent)
    {
        ent;   // C4705 statement has no effect
        mesh->Complete();
    }

	/**
	 * Saves the global mesh output.
	 * <br>
	 * <b>Role:</b> Attaches the mesh to the facetted ENTITY and deletes any existing mesh.
	 * <br>
	 * @param entity
	 * top level entity being faceted.
	 */
	virtual void save_global_mesh_output(
		ENTITY* entity
	)
	{
		// Attach the mesh to the facetted ENTITY (delete any existing MESHES)
		af_update(entity, MESH_MANAGER::get_app_id(), MESH_MANAGER::get_user_id(), mesh,TRUE);

		// We have given the MESH to the facetted ENTITY, so release it here.
		mesh = NULL;
	}


    // Face pointer for the group of oncoming polygons.
    // REMARK: only if ACIS_MESH is set, we can recognize,
    //         if the FACE is double-sided or not.
    //         So it may be useful, that the faceter (or any other calling
    //         system) give this information to the MESH_MANAGER by a second
    //         SPAparameter.

	/**
	 * Announce the face pointer for oncoming polygons.
	 * <br>
	 * @param newface
	 * face pointer for the group of oncoming polygons.
	 */
    virtual void announce_polygon_model_face(V_ENTITY *newface)
    {
#if defined ( ACIS_MESH )
        if ( newface && newface->identity() == FACE_TYPE )
        {
            dbl_sided = ( ((FACE*)newface)->sides() == DOUBLE_SIDED &&
                          ((FACE*)newface)->cont() == BOTH_OUTSIDE );

            aktface = newface;
        }
#else
            aktface = newface;
#endif
    }

	/**
	 * Announces an indexed node on a polygon, including polynode coordinate data.
	 * <br>
	 * <b>Role:</b> This signature of <tt>announce_indexed_polynode</tt> is called if
	 * <tt>need_indexed_polynode_with_data</tt> returns <tt>TRUE</tt>. The node identifier is the one
	 * previously received from <tt>announce_indexed_node</tt>. Used with the indexed protocol.
	 * <br>
	 * @param ent
	 * entity (<tt>COEDGE</tt> or <tt>NULL</tt>) along the edge following the polynode.
	 * @param ipoly
	 * 0-based polygon index. This is the same as the immediately preceding call to <tt>start_indexed_polygon</tt>.
	 * @param i
	 * 0-based counter within the polygon. This increments sequentially on successive calls.
	 * @param pnode
	 * node identifer as previously received from <tt>announce_indexed_node</tt>.
	 * @param edge_tpar
	 * if the node lies on an edge, then returns tpar, otherwise garbage.
	 * @param uv
	 * parametric coordinates.
	 * @param X
	 * cartesian coordinates.
	 * @param N
	 * surface normal.
	 */
    virtual void announce_indexed_polynode(V_ENTITY *ent,
											int ipoly,
											int i,
											void *pnode,
											const double &edge_tpar, 
											const SPApar_pos &uv,
											const SPAposition &X,
											const SPAunit_vector &N);

	/**
	 * Announces an indexed node on a polygon, including coedge pointers.
	 * <br>
	 * <b>Role:</b> This signature of <tt>announce_indexed_polynode</tt> is called if 
	 * <tt>need_coedge_pointers_on_polyedges</tt> returns <tt>TRUE</tt>. 
	 * The node identifier is the one previously received from
	 * <tt>announce_indexed_node</tt>. Used with the indexed protocol.
	 * <br>
	 * @param ent
	 * entity (<tt>COEDGE</tt> or <tt>NULL</tt>) along the edge following the polynode.
	 * @param ipoly
	 * 0-based polygon index. This is the same as the immediately preceding call to <tt>start_indexed_polygon</tt>.
	 * @param i
	 * 0-based counter within the polygon. This increments sequentially on successive calls.
	 * @param pnode
	 * node identifer as previously received from <tt>announce_indexed_node</tt>.
	 */
	virtual void announce_indexed_polynode(V_ENTITY *ent, int ipoly, int i, void *pnode);

	/**
	 * Announces an indexed node on a polygon.
	 * <br>
	 * <b>Role:</b> This signature of <tt>announce_indexed_polynode</tt> is called if both 
	 * <tt>need_coedge_pointers_on_polyedges</tt> and <tt>need_indexed_polynode_with_data</tt> return <tt>FALSE</tt>, 
	 * which is the default. The node identifier is the one previously received from <tt>announce_indexed_node</tt>.
	 * Used with the indexed protocol.
	 * <br>
	 * @param ipoly
	 * 0-based polygon index. This is the same as the immediately preceding call to <tt>start_indexed_polygon</tt>.
	 * @param i
	 * 0-based counter within the polygon. This increments sequentially on successive calls.
	 * @param pnode
	 * node identifer as previously received from <tt>announce_indexed_node</tt>.
	 */
    virtual void announce_indexed_polynode(int ipoly, int i, void *pnode);

	/**
	 * Announces the beginning of a new polygon output.
	 * <br>
	 * <b>Role:</b> Used with the indexed protocol.
	 * <br>
	 * @param ipoly
	 * 0-based polygon index.
	 * @param npolynode
	 * number of nodes around the polygon.
	 * @param ishare
	 * info about which edge of previous polygon is shared with this one.
	 */
    virtual void start_indexed_polygon(int ipoly, int npolynode, int ishare=-2);

	/**
	 * Announces the end of the output for a polygon.
	 * <br>
	 * <b>Role:</b> Used with the indexed protocol. 
	 * <br>
	 * This member function does nothing.
	 * <br>
	 * @param ipoly
	 * 0-based polygon index.
	 */
    virtual void end_indexed_polygon(int ipoly)
    {
        ipoly; // C4705 statement has no effect
    }

    // Two new functions to enable additional node information,
    // the faceter should calculate this data.

	/**
	 * @nodoc
	 */
    virtual logical need_node_ext()
    {
#if defined ( OLD_APP )
        return ( FALSE );
#else
        return ( (flags&NEED_NODE_EXT) != 0 );
#endif
    }

    // a general node output function instead of the three functions above
    //   inode     : node identifier ( = array index )
    //   ptr       : can be FACE, EDGE, VERTEX or anything else
    //   uv, normal: should be NULL, if  need_node_ext() returns NULL
    //               we prefer pointer instead of references,

	/**
	 * @nodoc
	 */
    virtual void * AnnounceGlobalNode(int               inode,
                                      void              *ptr,
                                      const double			&tp,
                                      const SPAposition    &X,
                                      const SPApar_pos     &uv,
                                      const SPAunit_vector &normal );
};

/*! @} */
#endif // PE_MANAG_INCLUDE
