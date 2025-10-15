// $Id: pe_mesh.hxx,v 1.21 2002/08/16 17:32:31 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if ! defined ( POLYEDGE_MESH_INCLUDE )
#define POLYEDGE_MESH_INCLUDE

// *************************************************************************

#include "pe_defin.hxx"
#include <stdio.h>

/**
* @file pe_mesh.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */


#if ! defined ( OLD_APP )
  // special Defines
  #define NO_MATE   IBIT(0)   // ignore mate edges
  #define NO_SINV   IBIT(1)   // ignore self-invers edges
#endif

  #include "logical.h"
  #include "param.hxx"
  #include "position.hxx"
  #include "unitvec.hxx"
  #include "box.hxx"
#if defined ( ACIS_MESH )
  #include "acis.hxx"
  #include "entity.hxx"
  #include "face.hxx"
  #include "vertex.hxx"
  #include "edge.hxx"
  #include "transfrm.hxx"
  #include "mesh.hxx"
  //#include <ihl_husk/dcl_ihl.h>
  #include "dcl_ihl.h"

#else

  #define FORWARD   0
  #define REVERSED  1
  typedef logical REVBIT;

#endif

// *************************************************************************
// ERROR-DEFINES
// *************************************************************************
#define FC_ERR_NONE            0
#define FC_ERR_NO_ALLOC_POL    1
#define FC_ERR_NO_ALLOC_EDG    2
#define FC_ERR_NO_ALLOC_NOD    3
#define FC_ERR_NO_ALLOC_EXT    4
#define FC_ERR_POL_TOO_BIG     5
#define FC_ERR_EDG_TOO_BIG     6
#define FC_ERR_NOD_TOO_BIG     7
#define FC_ERR_EXT_TOO_BIG     8

// *************************************************************************
// other DEFINES
// *************************************************************************
#define NO_NF        0xFFFFFFFF
#define PE_NULL_EDGE 0xFFFFFFFF
#define PE_NULL_NODE 0xFFFFFFFF

#define HAS_POLYNODE_EXT	1		// define for new GetPolyNodeExt() functions

// *************************************************************************
class TRANSFORM;
class SPAbox;
class PE_POLY;
class RES_INFO;
class POLYEDGE_MESH;

// *************************************************************************
class DECL_IHL PE_NODE : public ACIS_OBJECT
{
    friend class POLYEDGE_MESH;
private:
    SPAposition xyz;
    void     *ent;
    uint     nflg;              // intern: Index to first PE_EDGE with this
                                //         PE_NODE as Start-Node
    ushort   user_flag : 1,
             is_vert   : 1,
             on_edge   : 1;
public:
    PE_NODE()  {}
    ~PE_NODE() {}

// Called only by MeshManager (indirect, during MESH-construction)
    void SetNode(const SPAposition &xyz, void *dta);
    void SetRef1(int iedg)                           { nflg = iedg;     }
    uint GetRef1() const                             { return ( nflg ); }

// simple Read-Functions
    uint       GetUserFlag() const              { return ( user_flag ); }
    uint       IsVert() const                     { return ( is_vert ); }
    uint       OnEdge() const                     { return ( on_edge ); }
    void *     DataPtr() const                        { return ( ent ); }
    const SPAposition & GetPosition() const              { return ( xyz ); }
    void       NodeData(SPAposition &pos, void *&dta) const;
    void       GetNodeDbl(double *point3d) const;

// left in for compatibility
    SPAposition & GetPosition()                          { return ( xyz ); }

// simple Update-Functions
    void SetUserFlag(uint bit)              { user_flag = (ushort)bit;  }
    void SetDataPtr(void *dta)                    { ent = dta;          }
    void UpdCoords(const SPAposition *newp)          { xyz = *newp;        }

#if defined ACIS_MESH
    ENTITY * GetEntity() const               { return ( (ENTITY*)ent ); }
#endif

#if defined OLD_APP
// for OLD_APP only
    void GetNode(SPAposition &pos, ENTITY *&entity)
    {
        pos = xyz;
        entity = (ENTITY*)ent;
    }
    void     SetEntity(ENTITY *entity)       { ent = entity;            }
    int      IsMarkiert()                    { return ( user_flag );    }
    void     Markiere(uint bit)              { user_flag= (ushort)bit;  }
#endif
    void     Debug(int nr, FILE *dbgfp);
};

// *************************************************************************
#if ! defined ( OLD_APP )
// *************************************************************************
class DECL_IHL PE_NODE_EXT : public ACIS_OBJECT
{
private:
    SPAunit_vector normal;
    SPApar_pos     surface_uv;
    double		tpar;
    void        *aux_info;

public:
    PE_NODE_EXT()  {}
    ~PE_NODE_EXT() {}

    void Init(const SPAunit_vector &normvec, const SPApar_pos &uv, const double &tp)
    {
        normal = normvec;
        surface_uv = uv;
        tpar = tp;
        aux_info = NULL;
    }
    void SetNormal(const SPAunit_vector &normvec)      { normal = normvec;  }
    void SetUV(const SPApar_pos &uv)                   { surface_uv = uv;   }
    void SetTpar(const double &tp)                  { tpar = tp;         }
    void SetInfo(void *info)                        { aux_info = info;   }

    const SPAunit_vector & GetNormal() const           { return normal;     }
    const SPApar_pos     & GetUV() const               { return surface_uv; }
          double GetTpar() const                    { return tpar;       }
    const void   * GetInfo() const                  { return aux_info;   }
};
// *************************************************************************
#endif // OLD_APP
// *************************************************************************


// *************************************************************************
class DECL_IHL PE_EDGE : public ACIS_OBJECT
{
    friend class POLYEDGE_MESH;
private:
    POLYEDGE_MESH *owner;       // new, only for first time

    void    *ent;               // User-Pointer, maybe the real EDGE
    uint    nod1;               // Index of the Start-NODE
    uint    nod2;               // Index of the End-NODE
    uint    nflg;               // intern: Index to next PE_EDGE with the
                                //         same PE_NODE as Start-Node
    uint    poly;               // Index to the owning PE_POLY
    uint    first     : 1,      // this PE_EDGE is the first of owning POLY
            last      : 1,      // this PE_EDGE is the last  of owning POLY
            on_edge   : 1,      // this PE_EDGE is along a real EDGE
            is_mate   : 1,      // this PE_EDGE with bigger index as its mate
            is_sinv   : 1,      // mate-PE_EDGE belongs to the same PE_POLY
            has_tans  : 1,      // this PE_EDGE has pre-computed head and tail tangents
            user_flag : 2;      // Flag for User
    //ushort  type      : 12;     // Type-Info for User  (in MegaCad type
                                //      is set during mesh-construction)

	uint	mate;				// Index to mate-PE_EDGE

public:
    PE_EDGE()  {}
    ~PE_EDGE() {}

// Called only by MeshManager (indirect, during MESH-construction)
    void SetEdge(uint pol, uint nod, void *dta, POLYEDGE_MESH *mesh, uint on_real_edge);
    void SetPolygon(uint ipol)                  { poly =  ipol;        }
    void SetNode1(uint inod)                    { nod1 = inod;         }
    void SetNode2(uint inod)                    { nod2 = inod;         }
    void SetNxtNodRef(uint iedg)                { nflg = iedg;         }
    void SetMateMark()                          { is_mate = TRUE;      }
    void ClrMateMark()                          { is_mate = FALSE;     }
    void SetMark_SelfInvers()                   { is_sinv = TRUE;      }
    void SetFirst()                             { first = TRUE;        }
    void SetLast()                              { last = TRUE;         }
    void SetMate(uint mate_idx)                 { mate = mate_idx;     }
	void SetEdgeFlag(uint flag)                 { on_edge = flag;      }

// only for internal use
    uint NxtNodRef() const                 { return ( nflg );          }

// simple Read-Functions
    void   *DataPtr() const                { return ( ent );           }
    //ushort GetType()                       { return ( type );          }
    uint   GetUserFlag() const             { return ( user_flag );     }
    uint   Is_SelfInvers() const           { return ( is_sinv );       }
    uint   GetMateFlag() const             { return ( is_mate );       }
    //uint   First()                         { return ( first );         }
    //uint   Last()                          { return ( last );          }
    uint   OnEdge() const                  { return ( on_edge );       }

#if defined( ACIS_MESH ) && !defined( OLD_APP )
// tangents
	void   CalcTangents();
	void   ClearTangents()                 { has_tans = FALSE;         }
    uint   HasTangents() const             { return ( has_tans );      }
#endif

// simple Update-Functions
    void SetUserFlag(uint flag)            { user_flag = flag;         }
    //void SetType(ushort newtype)           { type = newtype;           }
    void SetDataPtr(void *dta)             { ent = dta;                }

//  Read-Functions for the old array-organized POLYEDGE_MESH
    uint NodeIndices(uint *inodes) const;
    uint GetNode1() const                  { return ( nod1 );          }
    uint GetNode2() const                  { return ( nod2 );          }
    uint GetPolygon() const                { return ( poly );          }
	uint GetMate() const                   { return ( mate );          }

//  Read-Function for the new  pointer-organized MESH,
//  (at time only virtual by using the owner-Pointer)
    PE_NODE *StartNode() const;
    PE_NODE *EndNode() const;
    PE_POLY *Polygon() const;
    PE_EDGE *Mate() const;
    PE_EDGE *NextInLoop(REVBIT sense = FORWARD) const;
    PE_EDGE *Next() const;
    uint    Index() const;

#if defined ACIS_MESH
    ENTITY * GetEntity() const             { return ( (ENTITY*)ent );  }
#endif

#if defined ( OLD_APP )
// for OLD_APP only
    #define EndPktIndizes  NodeIndices

    //int Aktiv(uint filter) {return( ! ((is_mate|(is_sinv<<1)) & filter) ); }
    uint IgnoreBits()                   { return ( (is_mate|(is_sinv<<1)) ); }

    // special function for OLD_APP (CutMesh())
    //void UpdateEdge(uint node1,uint node2,uint pol,uint newtype,ENTITY *entity);
    void UpdateEdge(uint node1,uint node2,uint pol,ENTITY *entity);

    //void CheckEben();
#endif
    void Debug(int nr, FILE *dbgfp);
};
// *************************************************************************

// *************************************************************************
class DECL_IHL PE_POLY : public ACIS_OBJECT
{
    friend class POLYEDGE_MESH;
private:
    POLYEDGE_MESH  *owner;                 // new, only for first time
    //SPAunit_vector  *nev;                   // new
    void           *face;                  // void* instead of FACE*
    uint           edge;                   // index to its first PE_EDGE
    uint           edg_count   : 29,       // number of edges of this polygon
                   last        :  1,       // this is the last polygon
                   dbl_sided   :  1,       // for hidden line:
                                           // (sheets: TRUE, volume: FALSE)
                 //p_eben      :  1,       // only for OLD_APP
                 //f_eben      :  1,       // only for OLD_APP
                   user_flag   :  1;

public:
    PE_POLY()  {}
    ~PE_POLY() {}

// Called (only by MeshManager) during mesh construction
    void SetPoly(uint count, uint nr, void *dta,
                 POLYEDGE_MESH *mesh, uint dblsid);
    void SetLast()                                      { last = TRUE;  }
    void ClrLast()                                      { last = FALSE; }
	void SetEdgeNum(uint edgnr)                         { edge = edgnr; }

// simple Read-Functions
    uint EdgeCount() const                  { return ( edg_count );     }
    uint GetUserFlag() const                { return ( user_flag );     }
    uint IsDblSided() const                 { return ( dbl_sided );     }
    void *DataPtr() const                   { return ( face );          }

// simple Update-Function
    void SetDataPtr(void *dta)              { face = dta;               }
    void SetUserFlag(uint flag )            { user_flag = (ushort)flag; }

//  Read-Functions for the old array-organized POLYEDGE_MESH
    uint   EdgeNum() const                  { return ( edge );          }

//  Read-Function for the new  pointer-organized MESH,
//  (at time only virtual by using the owner-Pointer)
    PE_EDGE *EdgePtr() const;
    int     Normal(SPAunit_vector *normal) const;
    PE_POLY *Next();

#if defined ACIS_MESH
    FACE * GetFace() const                  { return ( (FACE*)face );   }
#endif

#if defined OLD_APP
// for OLD_APP only, some of them not  yet implemented / used
    uint   GetEdge()                        { return ( edge );          }
    uint   GetEdgeAnz()                     { return ( edg_count );     }
    void   SetFace(FACE *fac)               { face = fac;               }
    //uint   IsPolyEben()                   { return ( p_eben );        }
    //uint   IsFaceEben()                   { return ( f_eben );        }
    //void CheckEben();
#endif
    void   Debug(int nr, FILE *dbgfp);
};
// *************************************************************************



// *************************************************************************
  // Actioncodes for 'ResetMesh'
  #define POL_MRK IBIT(0)
  #define EDG_MRK IBIT(1)
  #define NOD_MRK IBIT(2)
  #define NEV_DEL IBIT(3)

/*! \addtogroup IHLAPI
 *  \brief Declared at <pe_mesh.hxx>
 *  @{
 */

/**
 * Defines a mesh for the Interactive Hidden Line Component (IHL).
 * <br>
 * <b>Role:</b> A <tt>POLYEDGE_MESH</tt> is a mesh format that utilizes a
 * global indexed protocol. It has been specifically designed for use by
 * the Interactive Hidden Line Component and is not intended for general use.
 */
class DECL_IHL POLYEDGE_MESH : public MESH
{
protected:
    uint        pol_count;
    uint        edg_count;
    uint        nod_count;
    PE_POLY     *pol;
    PE_EDGE     *edg;
    PE_NODE     *nod;
    SPAunit_vector *polnv;
#if ! defined ( OLD_APP )
	uint		ext_count;
    PE_NODE_EXT *nod_ext;
	uint		*ext_map;
#endif
    ushort      fc_error;

    virtual void Set0Mesh(int errnr, int free_all);
    int          FacetErr(int errnr);

public:
    POLYEDGE_MESH();
    ~POLYEDGE_MESH();

// Called only by MeshManager (during MESH-construction)
    int  InitMesh(uint p_count, uint e_count, uint n_count,
                  uint with_node_ext = 0);
#if ! defined ( OLD_APP )
	int  InitMeshEx(uint p_count, uint e_count, uint n_count, uint x_count);
	int  AllocNodeExt(uint x_count);
#endif
    void  ResetMeshCounts(uint p_count, uint e_count, uint n_count);
    void SetPoly(uint polnr, uint edg_count, uint edgnr,
                 void *face, uint dbl_sided);
    void SetEdge(uint edgnr, uint polnr, uint nodnr, void *ent, uint on_real_edge);
    void SetNode(uint nodnr, void *ent, const SPAposition &pkt);
#if ! defined ( OLD_APP )
    void SetNodeExt(uint nodnr, const SPApar_pos &uv, const SPAunit_vector &normal,const double &tpar);
#endif
    void MarkEndEdges();
    void EdgeEndPoints();
    void NodeRefList();
    void MarkMates();
#if defined ( ACIS_MESH ) && !defined ( OLD_APP )
    void CalcEdgeTangents();
#endif
    void Complete();

// simple Read-Functions
    uint PolCount(void) const                   { return( pol_count ); }
    uint NodCount(void) const                   { return( nod_count ); }
    uint EdgCount(void) const                   { return( edg_count ); }
#if ! defined ( OLD_APP )
    uint NodExtCount(void) const                { return( ext_count ); }
#endif
    uint IsFacetError() const                   { return( fc_error  ); }

// complex routines
    void            DeleteMesh(int error)       { Set0Mesh(error, TRUE); }
    int             IsManifold() const;
    POLYEDGE_MESH * CopyMesh(int with_ents) const;
	virtual SPAbox		get_box();  // from MESH
    void            MakeBox(SPAbox *quad) const;
    void            ResetMesh(uint action);
    void          * PolyVoidPtr(uint ipol) const;
#if defined ( ACIS_MESH )
    FACE          * poly_face(uint ipol) const
    {
        return ( (FACE*)PolyVoidPtr(ipol) );
    }
#endif
    int             IsPolDoubleSided(uint ipol) const;
    int             CalcNormalVecToPolygons(TRANSFORM*,
                                            int (* func)(uint,void*),
												void*);

// Read-Functions for the old array-organized POLYEDGE_MESH
  #define  GET_PTR(nr, lim, ptr)   ( (nr) < (lim) ? (ptr)+(nr) : NULL )
    PE_EDGE     * GetEdge(uint nr) const    { return GET_PTR(nr, edg_count, edg);   }
    PE_NODE     * GetNode(uint nr) const    { return GET_PTR(nr, nod_count, nod);   }
    PE_POLY     * GetPoly(uint nr) const    { return GET_PTR(nr, pol_count, pol);   }
    SPAunit_vector * GetNormal(uint nr) const  { return GET_PTR(nr, pol_count, polnv); }

#if ! defined ( OLD_APP )
	PE_NODE_EXT * GetNodeExt(uint nr) const { return GET_PTR(nr, ext_count, nod_ext); }
    const SPAunit_vector & GetNodeNormal(uint nr) const {return GET_PTR(nr, ext_count, nod_ext)->GetNormal(); }
    const SPApar_pos & GetNodeUV(uint nr) const {return GET_PTR(nr, ext_count, nod_ext)->GetUV(); }
          double GetNodeTpar(uint nr) const { return GET_PTR(nr, ext_count, nod_ext)->GetTpar(); }

	// node-ext access using polygon number an edge number within polygon
	int				SetPolyNodeExtMap(uint polnr,uint edgnr,uint extnr);
	uint			GetPolyNodeExtMap(uint polnr,uint edgnr) const;
	PE_NODE_EXT *	GetPolyNodeExt(uint polnr,uint edgnr) const;

	// node-ext access using global edge number
	int				SetPolyNodeExtMap(uint edgnr,uint extnr);
	uint			GetPolyNodeExtMap(uint edgnr) const;
	PE_NODE_EXT *	GetPolyNodeExt(uint edgnr) const;
#endif

  #undef  GET_PTR

    uint             Mate(uint edgnr) const;
    uint             PolyTo2Edges(uint iedg1, uint iedg2) const;
    int              PolyNormal(uint polnr, SPAunit_vector *normal) const;
    const SPAposition & EdgeStartPos(uint) const;
    const SPAposition & EdgeEndPos(uint) const;
    uint             NextEdge(const PE_POLY*,uint,REVBIT = FORWARD) const;

    int  IsMate(uint edgnr) const
        { return( edg[edgnr].GetMateFlag() ); }
    int  IsEdgeMarked(uint edgnr) const
        { return ( edg[edgnr].GetUserFlag() || edg[edgnr].Is_SelfInvers() ); }
    void MarkEdge(uint edgnr, uint usflg)
        { edg[edgnr].SetUserFlag(usflg); }
    uint IsPolyMarked(uint polynr) const
        { return ( pol[polynr].GetUserFlag() ); }
    void MarkPoly(uint polynr, uint usflg)
        { pol[polynr].SetUserFlag(usflg); }


#if defined ( OLD_APP )
// for OLD_APP only
    #define EdgAnfp EdgeStartPos
    #define EdgEndp EdgeEndPos

    int      EdgeLoop(TRANSFORM *trf, uint bodynr, uint filter,
                      int (_cdecl * func)(tmeshkante *, void *, void *),
                      void *dta1, void *dta2);

    SPAunit_vector * GetNev(uint nr)  { return( (nr<pol_count) ? polnv+nr : NULL); }

    uint GetPolAnz(void)                           { return( pol_count ); }
    uint GetNodAnz(void)                           { return( nod_count ); }
    uint GetEdgAnz(void)                           { return( edg_count ); }

    int  IsKanteMarkiert(uint edgnr)
        { return ( edg[edgnr].GetUserFlag() || edg[edgnr].Is_SelfInvers() ); }
    void MarkiereKante(uint edgnr, uint usflg)
        { edg[edgnr].SetUserFlag(usflg); }
    uint IsPolygonMarkiert(uint polynr)
        { return ( pol[polynr].GetUserFlag() ); }
    void MarkierePolygon(uint polynr, uint usflg)
        { pol[polynr].SetUserFlag(usflg); }
#endif

#if defined ( DEBUG_MESH )
    void Debug(FILE *dbgfp);
    uint Size();
#endif // DEBUG_MESH


// CSR functions and data
public:
	void			AllocTangents(void);
	void			FreeTangents(void);
	logical			HasTangents(void) const { return head_tans != NULL && tail_tans != NULL; }

	void			SetHeadTan(uint edgnr,const SPAvector& tan) { head_tans[edgnr] = tan; }
	void			SetTailTan(uint edgnr,const SPAvector& tan) { tail_tans[edgnr] = tan; }
	const SPAvector&	GetHeadTan(uint edgnr) const { return head_tans[edgnr]; }
	const SPAvector&	GetTailTan(uint edgnr) const { return tail_tans[edgnr]; }

protected:
	SPAvector	*head_tans;
	SPAvector	*tail_tans;

// MRG functions and data
public:
	friend class RES_INFO;
	friend class PREPROCESSOR;

	// test for/free MRG-related info
	logical			HasResInfo(void) const { return neighbors != NULL; }
	void			FreeResInfo(void);

	// access the res level
	uint			GetMaxResLevel(logical withEdgeInfo = FALSE) const;
	uint			GetResLevel(void) const;

	// access the minimum counts
	uint			GetMinNodCount() const { return min_node_count; }
	uint			GetMinPolCount() const { return min_poly_count; }
	uint			GetMinEdgCount() const { return min_edge_count; }

	// access the maximum counts
	uint			GetMaxNodCount() const { return max_node_count; }
	uint			GetMaxPolCount() const { return max_poly_count; }
	uint			GetMaxEdgCount() const { return max_edge_count; }

	// collapse or expand the mesh
	uint			SetResLevel(uint target);
	uint			ResCollapse(void) { return SetResLevel(GetResLevel() + 1); }
	uint			ResExpand(void) { return SetResLevel(GetResLevel() - 1); }
	uint			SetPolyCount(uint targetPolys,logical goOver = FALSE);

	// copy the mesh, including the res info
	POLYEDGE_MESH * CopyMeshAndResInfo();

protected:
	uint *		neighbors;
	RES_INFO *	collapse_info;
	uint		min_node_count;
	uint		min_edge_node_count;
	uint		max_node_count;
	uint		min_poly_count;
	uint		max_poly_count;
	uint		min_edge_count;
	uint		max_edge_count;

	// helper functions
	void			FixEdgeRefs(uint maxNode = 0);
	void			FixMates(void);
};

/*! @} */

#endif  // POLYEDGE_MESH_INCLUDE
