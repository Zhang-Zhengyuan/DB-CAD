/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header file defining attributes for local ops.
/*******************************************************************/
// Jul-31-09 wsd : added set_apoint.
// Jul-21-09 wsd : added new member tangent so that we do not make unnecessary curve extensions.
// Jun-09-09 wsd : added delete_csi to ATTRIB_LOP_VERTEX.
// Apr-08-09 bhd : R20 implemented to_tolerant_owner virtual method to avoid crash
// Jan-16-09 wsd : moved temp_data class from vertattr.cpp so that we can make one of these in OFFSET_THICKEN.
// Jan-13-09 ioo : Added member (_coedge_apt_list) and its access function  to ATTRIB_LOP_VERTEX
// Jan-05-09 wsd : Added member to ATTRIB_LOP_FACE to know if we need par_int_curs for its edges.
// Nov-06-08 wsd : 87969, need set_splitting and compute_characteristic_length() to be public process_special_4_edge_vertex.
// May-21-08 bhd : R19 added approx_offset and is_reparam_exact_offset data
//                 members and access functions to ATTRIB_LOP_FACE
// May-07-08 jkf : R19 m_original_tolerance (not used, though)
// Apr-23-08 ioo : R18SP3 added new data member _same_curve to ATTRIB_LOP_EDGE
// Apr-09-08 jkf : R18SP3 86784 set_lop_csi()
// Nov-27-07 jkf : R19 add tweak argument to resolve()
// Sep-07-07 bhd : R18 implemted virtual method to_tolerant_owner() to
//                 ensure the attribute is updated properly when the
//                 owner becomes tolerized
// Jul-26-07 wsd : Added method to set the convexity for the attribute.
// Jan-11-07 bhd : R17 add ATTRIB_LOP_FACE attribute
// Feb-25-04 jkf : R14 74480 74481 Pass a TWEAK object to ::init()
//                 and ::Make_Attrib_LOP_EDGE()
/*******************************************************************/
#if !defined( LOP_EDGE_ATTRIB )
#define LOP_EDGE_ATTRIB
#include "acis.hxx"
#include "at_sys.hxx"
#include "attrib.hxx"
#include "box.hxx"
#include "collection.hxx"
#include "cvty.hxx"	
#include "dcl_lop.h"
#include "intcusf.hxx"
#include "lists.hxx"
//#include "loptools.hxx"
#include "position.hxx"
#include "tophdr.hxx"
#include "usecount.hxx"
#include "vlists.hxx"
#include "err_info_list.hxx"

class APOINT;
class CURVE;
class curve;
class EDGE;
class ENTITY;
class FACE;
class lop_cu_sf_int;
class PCURVE;
class pcurve;
class surf_surf_int;
class SURFACE;
class surface;
class TWEAK;
class VERTEX;
class error_info;
class error_info_list;

#if defined D3_STANDALONE || defined D3_DEBUG
	class   D3_ostream;
	class   D3_istream;
#endif
/*******************************************************************/
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_LOP_VERTEX, LOP )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_LOP_EDGE, LOP )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_LOP_FACE, LOP )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
DECL_LOP logical do_lop_csi(COEDGE *coed, void *data, const surface &sur );
DECL_LOP logical do_lop_cci(EDGE *ed0, EDGE *ed1, void *data, logical &found_intersection = *(logical *)NULL_REF );
DECL_LOP logical lateral_intersect(COEDGE *coed, void *data, logical );
DECL_LOP logical non_lat_tang_intersect(COEDGE *coed, void *data, logical );
DECL_LOP logical prop_intersect(COEDGE *coed, void *data, logical );
DECL_LOP logical curves_cci(COEDGE *coed, void *data, logical );
DECL_LOP logical all_curves_cci(COEDGE *coed, void *data, logical );
DECL_LOP logical curves_cci_no_cross(COEDGE *coed, void *data, logical );
DECL_LOP logical curves_pp(COEDGE *coed, void *data, logical edge = TRUE);

// The base class contains ....

extern DECL_LOP int ATTRIB_LOP_VERTEX_TYPE;
#define ATTRIB_LOP_VERTEX_LEVEL (ATTRIB_SYS_LEVEL + 1)

class DECL_LOP ATTRIB_LOP_VERTEX: public ATTRIB_SYS
{
private:
    // data
	// original body data
    int       _n_edges;     		// no of edges meeting at vertex
	SPAposition  _initial_pos;         // initial SPAposition

    /**
     * Self explanatory
     */
    double _m_original_tolerance;
	// initial state of attribute
	logical   _fixed;				// the vertex is not moving, 
	                                // so use its coords
	logical   _prespecified;		// the final point was preset

	// current state of attribute
	logical   _splitting;           // vertex will split
	logical   _free_splitting;		// vertex is free and will split
	logical   _resolved;			// the final point has been set
	logical   _undefined;			// the final point cannot be calculated
	                                // ( eg sfs undefined )
	// intersection data
	TWEAK    *_tweak;               // the tweak
    FACE     *_tweak_face;			// one of the faces being tweaked
    SURFACE  *_tool_surface;		// its new surface
	// results 
    lop_cu_sf_int *_lop_csi;		// chain of possible common positions
    lop_cu_sf_int *_all_lop_csi;	// chain of all positions
	lop_cu_sf_int *_new_param;      // chain of to store new parameters
    
    double _characteristic_length;	// Used for splitting vertex situation.
	logical		  _keep_as_tolerant; // Flag to tell whether we want to retain this 
									// vertex as a tolerant vertex when reverting 
									// tolerant vertices to regular vertices in
									// tolerant conversions.
	logical _tangent;               // wolf 07-20-09 is the vertex tangent;
	// final results
    APOINT   *_apoint; 				// the final point

	// results for splitting vertices
	ENTITY_LIST _coedge_list; // coedge end
	ENTITY_LIST _apoint_list; // new apoint

	SPACOLLECTION *_coedge_apt_list; //new list that combines the _coedge_list and _apoint_list
	// STI rr 12/19/01 begin: When copying
	// lop_cu_sf_int data from one vertex 
	// attribute to another, the curves in
	// the lop_cu_sf_int data needed to be
	// copied too. The void list below stores
	// these copied curves to be cleaned up later.
	VOID_LIST _curve_list;
	// STI rr end.	
	
	
	
//VPL 24May2001: made constructor private. this gets called from the static create methods.
//there are separate init functions corresponding to the constructors. these are called from respective
//create methods.
	// vertex is fixed at its initial SPAposition ( eg in taper )
	ATTRIB_LOP_VERTEX(
				ENTITY *own			 // owner
			);

//init functions corresponding to the constructors.
	logical init(
				ENTITY *own					 // owner
			);
	// when final SPAposition known( eg in move, offset );-
	logical init(
                APOINT        *pos,          // prespecified point
			    TWEAK         *tw = NULL     // tweak, if known
			);


	// general constructor, does a csi using lateral edge and new surface
	logical init(
                FACE          *tweak_face,   // face being tweaked
                SURFACE       *tool_surface, // tool surface
			    const SPAbox     &bx,           // intersection SPAbox
			    TWEAK         *twk,          // tweak 
			    const logical cross         // TRUE if cross curves known
			);

	// constructor for a 3 edge vertex, lying on an edge between
	// two faces where one is tweaked to the other
	logical init(
			    COEDGE        *coed,         // owner is end of this coed
			                                 // which lies between the faces
			    TWEAK         *tw           // tweak
			);


	// constructor for a mergeable 2 edge vertex - uses other vertices
	// and sets possible points by interpolation of curve parameters
	logical init(
                FACE          *tweak_face,   // face being tweaked
                SURFACE       *tool_surface, // tool surface
			    COEDGE        *coed,         // owner is start of this coed
			                                 // on face
			    TWEAK         *tw           // tweak
			);


	// when vertex splitting and no csi results at all
	logical init(
			    TWEAK         *tw            // tweak
			);

	// when vertex splitting and csi results known
	logical init(
			    TWEAK         *tw,           // tweak
			    lop_cu_sf_int *csi,           // csi
				logical copy_to_all_lop_csi
			);
	
	// STI rr (12/19/01) begin: If we simplify a splitting
	// vertex into two non splitting vertices, then
	// their csi solutions need to be duplicated too.
	// except for unrelated csi, i.e csi that don't
	// lie on any of the curve solutions of the edges
	// at the vertex. This function deletes such 
	// unreated csi.
	void delete_unrelated_csi( lop_cu_sf_int *& lop_csi, 
						  VOID_LIST &curve_list, 
						  VOID_LIST &other_list,
						  ATTRIB_LOP_VERTEX *alv);
	// STI rr (12/19/01) begin: Get the copy of
	// of the curve cu from the _curve_list stored
	// in this attribute.
	curve *get_curve_copy( const curve *cu );
	// STI rr end.

public:
	void compute_characteristic_length();
    virtual logical pattern_compatible() const;

	void set_keep_as_tolerant( logical l ) { backup(); _keep_as_tolerant = l; }
	logical keep_as_tolerant() { return _keep_as_tolerant; }

	// bhd 27Aug07 - implement this virtual function so the
	// keep_as_tolerant flag gets set properly
	virtual void to_tolerant_owner( ENTITY *tol_ent );

                                    // returns TRUE 
	// Simple read functions for the data members.

	// original data
	TWEAK    *tweak()             const { return _tweak; }
    int       no_of_edges();
    VERTEX   *vertex()            const { return ( VERTEX * ) entity(); }
	const SPAposition &initial_pos() const { return _initial_pos; }

	// state of attribute
    logical   fixed()             const { return _fixed; }
    logical   resolved()          const { return _resolved; }
    logical   prespecified()	  const { return _prespecified; }
	logical   splitting()         const { return _splitting; }
	logical   free_splitting()    const { return _free_splitting; }
	logical   undefined()         const { return _undefined; }
	void 	  set_undefined();
	void      set_splitting(logical in_splitting) { _splitting = in_splitting; }
	void      set_tangent(logical in_tangent) { _tangent = in_tangent; }
	logical   get_tangent() {return _tangent;}
	void      set_apoint(SPAposition in_pos);

	// intersection data
    FACE     *tweak_face()        const { return _tweak_face; }
    SURFACE  *tool_surface()      const { return _tool_surface; }
    lop_cu_sf_int *lop_csi()      const { return _lop_csi; }
	logical   make_all_lop_csi();

	// for setting error info
	error_info_list _err_info_list;
	void set_error_info(error_info* error_info);
	error_info_list& error_info_list() { return _err_info_list; }

//VPL 13Jan2003 : split the TWEAK intialization into two functions
	void make_lop_csi_point_perp();
    /**
     * Add  the lop_csi properly to hashed table 
     */
	logical   add_lop_csi( lop_cu_sf_int *lop_csi );

	logical is_curve_in_lop_csi(const curve &cu);

    /**
     * Force the input to be our _lop_csi (Apr-09-08 jkf : R18SP3 86784)
     */
    void   set_lop_csi( lop_cu_sf_int *lop_csi );
	
	// for r5206 jm 22 May 98 - to delete _all_lop_csi prior to merge
	void  delete_all_lop_csi() 
	{
        if ( _all_lop_csi != NULL )
        {
			backup();
			cleanup( _all_lop_csi ); 
	        _all_lop_csi = NULL;
        }
    }

	// end for r5206 jm 22 May 98

	// Appends the input csi to the existing all_lop_csi list
	void append_to_all_lop_csi(lop_cu_sf_int *new_csi);

    // wolf 07-09-09 delete a given csi out of the _lop_csi list.
	void delete_csi_from_lop_csi(lop_cu_sf_int *del_lcsi);

	void delete_csi_from_all_lop_csi(lop_cu_sf_int *del_lcsi);

    lop_cu_sf_int *all_lop_csi()  const { return _all_lop_csi; }
    int			 no_of_geom_points() const;
	double		 geom_tol(int i) const;
    SPAposition  geom_point( int i ) const;
	logical		 param( curve const *cu, SPAposition const &pos, double &t );
	logical      test_point( curve const *cu, SPAposition const &pos, double &t );
 
	// final data
    APOINT   *apoint()		      const { return _apoint; }

	ENTITY_LIST &coedge_list() { return _coedge_list; }
	ENTITY_LIST &apoint_list() { return _apoint_list; }

	SPACOLLECTION *coedge_apt_list() { return _coedge_apt_list; }

	// Constructors
//need the default constructor
	ATTRIB_LOP_VERTEX();
//VPL 25May2001: made constructor private. this gets called from the static create methods.

	// vertex is fixed at its initial SPAposition ( eg in taper )
	static ATTRIB_LOP_VERTEX* Make_ATTRIB_LOP_VERTEX(
				ENTITY *own = NULL			 // owner
			);

	// when final SPAposition known( eg in move, offset );-
	static ATTRIB_LOP_VERTEX* Make_ATTRIB_LOP_VERTEX(
				ENTITY        *own,			 // owner
                APOINT        *pos,          // prespecified point
			    TWEAK         *tw = NULL     // tweak, if known
			);


	// general constructor, does a csi using lateral edge and new surface
	static ATTRIB_LOP_VERTEX* Make_ATTRIB_LOP_VERTEX(
				ENTITY        *own,			 // owner
                FACE          *tweak_face,   // face being tweaked
                SURFACE       *tool_surface, // tool surface
			    const SPAbox     &bx,           // intersection SPAbox
			    TWEAK         *twk,          // tweak 
			    const logical cross         // TRUE if cross curves known
			);

	// constructor for a 3 edge vertex, lying on an edge between
	// two faces where one is tweaked to the other
	static ATTRIB_LOP_VERTEX* Make_ATTRIB_LOP_VERTEX(
				ENTITY        *own,			 // owner
			    COEDGE        *coed,         // owner is end of this coed
			                                 // which lies between the faces
			    TWEAK         *tw           // tweak
			);


	// constructor for a mergeable 2 edge vertex - uses other vertices
	// and sets possible points by interpolation of curve parameters
	static ATTRIB_LOP_VERTEX* Make_ATTRIB_LOP_VERTEX(
				ENTITY        *own,			 // owner
                FACE          *tweak_face,   // face being tweaked
                SURFACE       *tool_surface, // tool surface
			    COEDGE        *coed,         // owner is start of this coed
			                                 // on face
			    TWEAK         *tw           // tweak
			);


	// when vertex splitting and no csi results at all
	static ATTRIB_LOP_VERTEX* Make_ATTRIB_LOP_VERTEX(
				ENTITY        *own,			 // owner
			    TWEAK         *tw            // tweak
			);

	// when vertex splitting and csi results known
	static ATTRIB_LOP_VERTEX* Make_ATTRIB_LOP_VERTEX(
				ENTITY        *own,			 // owner
			    TWEAK         *tw,           // tweak
			    lop_cu_sf_int *csi,          // csi
				logical copy_to_all_lop_csi = FALSE
			);



	// scanner functions in curve surface constructor
	friend DECL_LOP logical do_lop_csi( COEDGE *coed, void *data,
								const surface &sur );
	friend DECL_LOP logical do_lop_cci( EDGE *ed0, EDGE *ed1, void *data, logical &found_intersection );

	friend DECL_LOP logical lateral_intersect( COEDGE *coed, void *data, logical );
	friend DECL_LOP logical non_lat_tang_intersect( COEDGE *coed, void *data, logical );
	friend DECL_LOP logical prop_intersect( COEDGE *coed, void *data, logical );
	friend DECL_LOP logical curves_cci( COEDGE *coed, void *data, logical );
	friend DECL_LOP logical all_curves_cci( COEDGE *coed, void *data, logical );
	friend DECL_LOP logical curves_cci_no_cross( COEDGE *coed, void *data, logical );
	friend DECL_LOP logical curves_pp( COEDGE *coed, void *data, logical );

	// resolve - set point once known, deleting csis
	logical resolve( APOINT *ap, COEDGE *coed = NULL );

	// to clean up csi/ssi on blends
	logical check_pos( SPAposition const &foot ) const; 

	// resolve - save point from csi chain to member apoint, and delete ssis
    void save_point( int i );

	// STIBUGFIX (r6670) rr (11/06/00) begin: sort the csis in order of
	// their distance to the estimated SPAposition for the vertex SPAposition.
	void sort_csi(COEDGE *coed);
	// STIBUGFIX (r6670) rr end.
	// delete csis
	void cleanup();
	void cleanup( lop_cu_sf_int * );

	// STI rr (12/19/01) begin: In certain special 
	// circumstances it is possible to simplify a
	// splitting vertex into two non-splitting vertices
	// When this happens we split the results in the
	// vertex attribute of the formerly splitting vertex
	// so that each of the two non-splitting vertices
	// have appropriate csi data in them.
	void split( VERTEX *new_vert );
	// STI rr end.

	// Make a copy

	ATTRIB_LOP_VERTEX(  ATTRIB_LOP_VERTEX const * );

	virtual ATTRIB_LOP_VERTEX* copy() const;

    // don't save with copy-scan mechanism
    logical duplicatable() const;

    // allow copies with copy-scan mechanism
    logical copyable() const;

	// Test two LOP_ATTRIBUTES for equality.

	virtual logical operator==( ATTRIB_LOP_VERTEX const& ) const;

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_LOP_VERTEX, LOP )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

    /**
     * @nodoc
     */
    USE_COUNT_REFERENCE_DECL
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

	double characteristic_length() const { return _characteristic_length; }

#if defined D3_STANDALONE || defined D3_DEBUG

    void        print( D3_ostream& os, int level )      const;

#endif
    /**
     * Get the tolerance that the vertex had when the attribute was made
     */
    double get_original_tolerance();
};


#if defined D3_STANDALONE || defined D3_DEBUG

extern  DECL_LOP D3_ostream& operator<<( D3_ostream& os, const ATTRIB_LOP_VERTEX& );

#endif

enum ATTRIB_LOP_EDGE_dir
    { 
    ATT_LOP_EDGE_correct, 	// cu direction correct for edge   
    ATT_LOP_EDGE_incorrect, // cu direction incorrect
    ATT_LOP_EDGE_unknown    // cu direction correctness unknown
    };

// AH 12/03/2001 data for deciding edge sense before soln tree
enum ATTRIB_LOP_EDGE_dsense
    { 
    dFORWARD, 	
    dREVERSED, 
    dunknown   
    };



extern DECL_LOP int ATTRIB_LOP_EDGE_TYPE;
#define ATTRIB_LOP_EDGE_LEVEL (ATTRIB_SYS_LEVEL + 1)

class DECL_LOP ATTRIB_LOP_EDGE: public ATTRIB_SYS {
private:
    // data

	logical _new_bisector_solution;

	// original body data
	COEDGE        *_coed0;     // coedge of face 0
	COEDGE        *_coed1;     // coedge of face 1
	LOPT_EDGE_cvty _cvty;       // original convexity of edge
	SPAposition      _old_start_pos; // original start pos;
	SPAposition      _old_mid_pos;   // original mid pos;
	SPAposition      _old_end_pos;   // original end pos;
	SPAposition      _est_mid_pos;   // estimated mid pos;

	ATTRIB_LOP_EDGE_dsense _dsense;  // AH - 12/03/2001 for deciding sense 
                                     // before soln tree

	// initial state of attribute
	logical        _fixed;	      // edge curve is not moving, so using its geom
	logical        _prespecified; // final edge curve was preset
	logical		   _kill_me;	  // indicates edges to be removed; set to
								  // FALSE by default

	// current state of attribute
	logical        _resolved;  // final edge curve has been set 
	logical        _complete;  // final edge curve and its sense set
	logical        _undefined; // the final curve cannot be calculated
	                           // ( eg sfs undefined )
	logical		   _implemented; // The attribute has been implemented on the edge, but
								 // is still not lost from the edge, to enable splitting 
								 // of convexity points at the end of LOP, if necessary.
    // STI TOL_MOD swa: flag indicating edge should be tolerant in final model
    //                  and whether or not pcurves can be re-used or edge must
    //                  be reprojected.  Also flag for whether final tedge is
    //                  tangent -- _tol_tangent flag has no meaning when _tolerant
    //                  is not TRUE
    logical        _tolerant;
    logical        _trust_tolerant_pcurve_0;
	logical        _trust_tolerant_pcurve_1;
    logical        _tolerant_tangent;
	double		   _tolerance;		// If this value is positive then use this value as the 
									// tolerance of the tolerant edge that will be made from
									// a regular edge. For now this value is set only by remove_face.
    // STI TOL_MOD end:

	logical			_bisector_solution;
	logical			_bisector_pcurves_made;
	SURFACE			*_bisector_SF;
	// STI rr (12/19/01) If we know that the owner of
	// this edge attribute is a degenerate edge we mark 
	// it as such when we can. It is useful while c
	// computing the distance score of such edges.	
	logical			_owner_is_degenerate;
	// STI rr end.
	// intersection data
    SURFACE       *_sf0;       // new surface of face 0 
    SURFACE       *_sf1;       // new surface of face 1
	SPAbox            _bx;        // SPAbox for intersections
	// results
    surf_surf_int *_sf_sf_int; // possible curves

	// SSI with bisector surface and the second surface. 
	// Only set for bisector surface intersections.
	surf_surf_int *_other_sf_sf_int; 

	// final results
    CURVE         *_cu;        // final curve
	ATTRIB_LOP_EDGE_dir _dir;  // correctness of direction of new curve
	ATTRIB_LOP_EDGE_dir _old_dir;  // backup value of _dir if _dir changed for rbi.
    PCURVE        *_pc0;       // final pcurve of face 0
    PCURVE        *_pc1;       // final pcurve of face 1
    logical       _start_param_set;  // final start SPAparameter
	logical       _end_param_set ;   // final end SPAparameter 
    double        _start_param;      // final start SPAparameter
	double        _end_param ;       // final end SPAparameter
    void          set_convexity();
    VOID_LIST     _convexity_points; // convexity points

	logical _moveable;			     // specifies weather this guy is movable or not
	logical _copyable;			     // specifies weather this guy is movable or not
	logical _duplicatable;		     // specifies weather this guy is movable or not
	// STI rry 08/18/04 begin:  // use old pcurves in changed model, when the pcurves are iso lines in old and changed model
	logical _saved_pcu0;		// However since prespecified curve solution of such edges were not getting extended in 		 
	logical _saved_pcu1;		// CURVE *TWEAK::extend_cu( COEDGE *coed ), tell that code to extend such curves anyway. 		 
								// Currently, only offsetting saves pcurves from old model to use them in new model when 
								// the pcurves are iso lines.
	// STI rry end.
	// wolf 10-25-04
	// keep the unlimited range of the curve around, needed for tangent edge on rot_spl_sur with singularity
	SPAinterval _limited_param_range;

	// wolf 04-13-06
	// need to know if an edge attribute got added on a free degenerate edge, such that we can avoid lateral intersection lateron
	logical _free_degen;

    // wolf 04-11-08 - 86787 (ioo Apr-23-08)
    // logical indicating if a curve was prespecified which is the same as the edge curve, but might be subset to the edge range, so we do not need to
    // do a set_geometry on the edge in implement solution.
    logical _same_curve;
									 
	

//VPL 24May2001: made constructor private. this gets called from the static create methods.
	ATTRIB_LOP_EDGE(
				ENTITY *own					// owner
			);
//these are private init functions called from static create methods.
	logical init(
		ENTITY *own,						// owner
        TWEAK  *tw
		);

	// used in place of : copy constructor ( with new owner )
	logical init(
			    const ATTRIB_LOP_EDGE &att         // subject of copy
			);

    // used in place of : sf sf constructor
	logical init(
				ENTITY        *own,			// owner
			    COEDGE        *coed0,       // coedge on face 0 
                SURFACE       *sf0,		    // new surface on face 0
			    COEDGE        *coed1,       // coedge on face 1 
                SURFACE       *sf1,    		// new surface on face 1
			    const SPAbox  &bx,          // intersection SPAbox
                logical       tweaking,     // true if from a tweak
				TWEAK*		  tw,
				logical		  save_curve = TRUE,
				logical		  degenerate = FALSE // TRUE if the attrib is built for a degenerate free edge
			);

    // used in place of : bisector sf
	logical init(
				ENTITY        *own,			// owner
			    COEDGE        *coed0,       // coedge on face 0 
                SURFACE       *sf0,		    // new surface on face 0
			    COEDGE        *coed1,       // coedge on face 1 
                SURFACE       *sf1,    		// new surface on face 1
				SURFACE		  *bisector_sf, // bisector surface
			    const SPAbox  &bx,          // intersection SPAbox
                logical       tweaking,     // true if from a tweak
				TWEAK*        tw
			);

	// used in place of : curve constructor
	logical init(
                CURVE         *cu,		    // prespecified edge CURVE
			    ATTRIB_LOP_EDGE_dir dir,    // prespecifed direction
			    const SPAbox     &bx,       // intersection SPAbox,
				VOID_LIST	const &convexity_points = *(VOID_LIST*)NULL_REF,
                logical same_curve = FALSE
   			);

	// used in place of : cu and status constructor
	logical init(
                CURVE         *cu,		    // prespecified edge CURVE
			    ATTRIB_LOP_EDGE_dir dir,    // prespecifed direction
			    const SPAbox     &bx,          // intersection SPAbox
			    TWEAK         *tw,          // the tweak
				SURFACE*	  sf0,
				SURFACE*      sf1
   			);

public:
    virtual logical pattern_compatible() const;
                                    // returns TRUE 
	// Simple read functions for the data members.
	// original data
    EDGE   				*edge() const { return ( EDGE * ) entity(); }
    COEDGE              *coed( int index ) const;
    LOPT_EDGE_cvty 		convexity() const { return _cvty; }

	SPAposition const &old_start_pos() const { return _old_start_pos; }
	SPAposition const &old_mid_pos() const { return _old_mid_pos; }
	SPAposition const &old_end_pos() const { return _old_end_pos; }
	SPAposition est_mid_pos( TWEAK *tw ) const;

    ATTRIB_LOP_EDGE_dir dir()   const { return _dir; }

	// STI rr (12/19/01) begin: If the dir is going to be
	// unset then we record the value before unsetting it
	// as it can be used later.
    ATTRIB_LOP_EDGE_dir old_dir()   const { return _old_dir; }
    void set_dir( ATTRIB_LOP_EDGE_dir dir ) { backup(); _old_dir = _dir; _dir = dir; }
	// STI rr end.
    void unset_dir();

	// state of attribute
    logical             fixed() const { return _fixed; }
    logical             prespecified() const { return _prespecified; }
    logical             resolved() const { return _resolved; }
    logical             complete() const { return _complete; }
    logical             kill_me() const { return _kill_me; }
    logical             undefined() const { return _undefined; }
	void                set_undefined();
	logical             decided() const 
	                        {if ( _dsense == dunknown ) return FALSE; 
						     else return TRUE; }
	void                set_dsense( ATTRIB_LOP_EDGE_dsense sense ) 
	                        { backup(); _dsense = sense; }
	REVBIT              dsense() const 
	                       {if ( _dsense == dFORWARD ) return FORWARD; 
						     else return REVERSED; }

	logical				free_degenerate() const { return _free_degen;}
	
	// STI rr (12/19/01) begin: If we know that the 
	// owner is degenerate then we mark it as such whenever possible.
	// Aids in distance score computations.
	void mark_owner_degenerate(){ backup(); _owner_is_degenerate = TRUE;}
	logical owner_degenerate(){ return _owner_is_degenerate; }
	// STI rr end.
	// intersection data
    SURFACE            *sf( int index )  const;
	SPAbox const &         bx() const { return _bx; }

	// intersection results - possible curves
    surf_surf_int       *sf_sf_int() const  { return _sf_sf_int; }
    int                 no_of_geom_curves()  const;
    const curve         *geom_curve( int i ) const;
	curve               *geom_curve_for_update( int i ) const;
	// final curve and pcurves
    CURVE               *cu()   const { return _cu;  }
    PCURVE              *pcu( int index ) const;

	void                set_start_param( double const &t ) 
	{ backup(); _start_param_set = TRUE; _start_param = t; }
	logical             start_param_set() { return _start_param_set; }
	double              start_param() { return _start_param; }

	void                set_end_param( double const &t ) 
	{ backup(); _end_param_set = TRUE; _end_param = t; }
	logical             end_param_set() { return _end_param_set; }
	double              end_param() { return _end_param; }

    // split edge at convexity change points
	// STI rr (02/07/2003) Added argument to control postponing of splits if needed
	// and return value to tell us if we have postponed splitting or not.
    logical split_at_convexity_points(logical postpone_if_necessary = FALSE);

	VOID_LIST convexity_points() { return _convexity_points; }
	// for setting error info
	error_info_list _err_info_list;
	void set_error_info(error_info* error_info);
	error_info_list& error_info_list() { return _err_info_list; }

    logical same_curve() { return _same_curve; }
	// STI rr (12/19/01) begin: When two edges of the
	// model are merged after the computation of their
	// edge attributes, this function updates the edge
	// attribute of the surviving edge after the merge.
	void after_merge();
	// STI rr end.
    // Constructors
	ATTRIB_LOP_EDGE(); //Need public default constructor for ATTCOPY_DEF

//VPL 24May2001: made all constructors private. these static functions call the constructors inside.
//appropriate init function is called after the construction is over
//the constructors do not generate exceptions, however the init function may generate exception. in this case,
//the function will return NULL. otherwise it will return a valid ATTRIB_LOP_EDGE object.
	static ATTRIB_LOP_EDGE* Make_ATTRIB_LOP_EDGE(
				ENTITY *own = NULL,			// owner
                TWEAK  *tw  = NULL
			);

	// copy constructor ( with new owner )
	static ATTRIB_LOP_EDGE* Make_ATTRIB_LOP_EDGE(
				ENTITY        *own,			// owner
			    const ATTRIB_LOP_EDGE &att // subject of copy
			);

    // sf sf
	static ATTRIB_LOP_EDGE* Make_ATTRIB_LOP_EDGE(
				ENTITY        *own,			// owner
			    COEDGE        *coed0,       // coedge on face 0 
                SURFACE       *sf0,		    // new surface on face 0
			    COEDGE        *coed1,       // coedge on face 1 
                SURFACE       *sf1,    		// new surface on face 1
			    const SPAbox  &bx,          // intersection SPAbox
                logical       tweaking,     // true if from a tweak
				TWEAK*        tw,
				logical	      degenerate = FALSE // TRUE if attribute is made for a degenerate free edge
			);

   // bisector sf
	static ATTRIB_LOP_EDGE* Make_ATTRIB_LOP_EDGE(
				ENTITY        *own,			// owner
			    COEDGE        *coed0,       // coedge on face 0 
                SURFACE       *sf0,		    // new surface on face 0
			    COEDGE        *coed1,       // coedge on face 1 
                SURFACE       *sf1,    		// new surface on face 1
				SURFACE		  *bisector_sf, // bisector surface
			    const SPAbox  &bx,          // intersection SPAbox
                logical       tweaking,     // true if from a tweak
				TWEAK*        tw
			);

	// cu
	static ATTRIB_LOP_EDGE* Make_ATTRIB_LOP_EDGE(
				ENTITY        *own,			// owner
                CURVE         *cu,		    // prespecified edge CURVE
			    ATTRIB_LOP_EDGE_dir dir,    // prespecifed direction
			    const SPAbox     &bx,       // intersection SPAbox
				VOID_LIST	const &convexity_points = *(VOID_LIST*)NULL_REF,
                logical same_curve = FALSE
   			);

	// cu and status
	static ATTRIB_LOP_EDGE* Make_ATTRIB_LOP_EDGE(
				ENTITY        *own,			// owner
                CURVE         *cu,		    // prespecified edge CURVE
			    ATTRIB_LOP_EDGE_dir dir,    // prespecifed direction
			    const SPAbox     &bx,          // intersection SPAbox
			    TWEAK            *tw,          // the tweak
				SURFACE* srf0 = NULL,
				SURFACE* srf1 = NULL
   			);



	// these two constructors then may require the use of set_pcurves
	// to set a known pcurve on a specified coedge
	logical set_pcurve( COEDGE *coed, PCURVE *PC );


	// save old edge data
	void save_data(); 

	//save curve and pcurve from ssi chain into member CURVE
    void save_cu( int i );
	void save_pcu( int i, int index );

	//resolve - set curve once known, deleting ssi chain
    logical resolve( CURVE *cu, REVBIT sense, TWEAK* tw );

	// delete ssi chain
	void cleanup();
	
    void set_kill_me();

	void set_complete() { backup(); _complete = TRUE; }
	// STI rr (02/07/2003) begin: New flag is set if the edge is implemented
	// so that the edge does not get implemented multiple times. Edge attribute 
	// cannot be lost as it has information on convexity points from the 
	// intersectors. New functions to set and query this flag for the attribute.
	void set_implemented() { backup(); _implemented = TRUE; }

	void reset_for_solving() { _complete = FALSE; _implemented = FALSE;}

	logical implemented() { return _implemented; }
	// STI rr end.
    // STI TOL_MOD swa:  Functions to set and query whether final edge should be tolerant
    //                   and whether or not pcurves can be re-used
    void mark_as_tolerant() { backup(); 
                              _tolerant = _trust_tolerant_pcurve_0 = _trust_tolerant_pcurve_1 = _tolerant_tangent = TRUE; }
    logical is_tolerant() { return _tolerant; }
    logical is_tangent_tolerant() { return _tolerant && _tolerant_tangent; }
	void recalc_tolerant_pcurves() {backup(); _trust_tolerant_pcurve_0 = FALSE; _trust_tolerant_pcurve_1 = FALSE; }
    void recalc_tolerant_pcurve_0() {backup(); _trust_tolerant_pcurve_0 = FALSE; }
	void recalc_tolerant_pcurve_1() {backup(); _trust_tolerant_pcurve_1 = FALSE; }
    logical can_trust_tolerant_pcurves() { return (_trust_tolerant_pcurve_0 && _trust_tolerant_pcurve_1); }
    
	logical can_trust_tolerant_pcurve_0() { return _trust_tolerant_pcurve_0; }
	logical can_trust_tolerant_pcurve_1() { return _trust_tolerant_pcurve_1; }
	
    void mark_not_tangent_tolerant() {backup(); _tolerant_tangent = FALSE; }
    // STI TOL_MOD end:

	// STI rr 04/01/04 begin: Set the tolerance we want to assign to the edge in here
	// in case we have an idea on what it will be. For now this is being used only by
	// the remove face code when it makes lateral edges tolerant.
	double tolerance() { return _tolerance; }
	void set_tolerance(double tol){ backup(); _tolerance = tol; }
	// STI rr end.

	logical bisector_sol() const { return _bisector_solution; }
	SURFACE *bisector_sf() const { return _bisector_SF; } 
	// Make a copy of a lop ( provokes error )

	ATTRIB_LOP_EDGE(  ATTRIB_LOP_EDGE const * );

	virtual ATTRIB_LOP_EDGE* copy() const;

    // don't save with copy-scan mechanism
    logical duplicatable() const;

    // allow copies with copy-scan mechanism
    logical copyable() const;

    logical deletable() const;

	logical moveable() const;

	void set_movable(logical is_moveable) {backup(); _moveable = is_moveable;};
	void set_copyable(logical is_copyable) {backup(); _copyable = is_copyable;};
	void set_duplicatable(logical is_duplicatable) {backup(); _duplicatable = is_duplicatable;};
	void add_saved_pcurves();
	void set_saved_pcurve( int i, logical l );
	logical saved_pcurve(int i ) { return i==0 ? _saved_pcu0 : _saved_pcu1; };

	SPAinterval get_limited_range(void) {return _limited_param_range;};

	// bhd 08Aor09 - implement this virtual function so the
	// coedge data members get updated when the edge gets tolerized
	virtual void to_tolerant_owner( ENTITY *tol_ent );

	// Test two LOP_EDGE_ATTRIBS for equality.

	virtual logical operator==( ATTRIB_LOP_EDGE const& ) const;

	void merge_owner (ENTITY * entity,
		logical delete_owner);

	void set_new_bisector_solution (logical bisector_solution);

	logical get_new_bisector_solution (void) const;

	void set_convexity(LOPT_EDGE_cvty cvty);

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_LOP_EDGE, LOP )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

    /**
     * @nodoc
     */
    USE_COUNT_REFERENCE_DECL
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

#if defined D3_STANDALONE || defined D3_DEBUG

    void        print( D3_ostream& os, int level )      const;

#endif
};

// void delete_lop_edge(ENTITY *);

#if defined D3_STANDALONE || defined D3_DEBUG

extern  DECL_LOP D3_ostream& operator<<( D3_ostream& os, const ATTRIB_LOP_EDGE& );

#endif


// bhd 11Jan07 - Declaration of ATTRIB_LOP_FACE and related classes

// Class to handle use-counted object that is not an entity
// The main point here is that we need a container for which we can
// define the assigment operator and the destructor to take care of
// use counting.  If we don't, then the object may get deleted prematurely
// because backup() calls make_copy() which creates a temporary copy
class DECL_LOP error_info_container
{
	error_info* _error_info;

public:

	error_info_container();
	~error_info_container();

    error_info_container& operator=( const error_info_container& );

	void set( error_info* );
	error_info* get() const;
};

// The ATTRIB_LOP_FACE class
// This class keeps track of all the data used by TWEAK and derived
// classes for a face involved in tweak.  This attribute eliminates
// the need to shuffle arrays around as faces get added or removed
// from being involved in tweak.

// bhd todo: Probably better to use a hierarchy of container classes
// for all the data.  These classes would know the type of TWEAK class
// they belong to, so then we could (most likely) eliminate the pointer
// to the TWEAK class.
extern DECL_LOP int ATTRIB_LOP_FACE_TYPE;
#define ATTRIB_LOP_FACE_LEVEL (ATTRIB_SYS_LEVEL + 1)

class DECL_LOP ATTRIB_LOP_FACE: public ATTRIB_SYS {
private:
	const TWEAK* _tweak_class;		// same face may be involved in nested TWEAK operations
									// e.g. offsetting calls remove
	// TWEAK data
    SURFACE* _tool_sf;				// final surface
	SURFACE* _old_sf;				// original surface
	// OFFSET data
	double _offset;
	error_info_container _offset_error_info_cont;	// track bad curvature errors

	error_info_list _err_info_list;

	logical _approx_offset;			// offset surface is approximate
	logical _reparam_exact_offset;  // offset surface is exact, but the parameterization differs from progenitor
	logical _need_par_int_curs;     // flag to show that a face needs par_int_curs for its edges.
	// PLANE_TAPER data
	CURVE* _neutral_curve_owner;			
	// EDGE_TAPER data
	EDGE* _taper_edge;
	logical _has_interval_override;
	SPAinterval _interval_override;
	// SHADOW_TAPER data
	FACE* _sil_face;		// Face on which the silhouette has been imprinted
                            // This is often not the same as the face to be tweaked, 
							// as in the class 2 taper, when the adjacent face
                            // is passed to the tweak.
	EDGE* _tweak_edge;		// Edge that will be the silhouette edge after the tweak
	EDGE* _sil_edge;		// Original silhouette edge, before merging of class 2 silhouettes
	int _taper_class;		// Shadow taper class


	// private constructor, called from the static create methods.
	ATTRIB_LOP_FACE( const TWEAK* tweak_class, ENTITY *own );

	// private init functions, called from static create methods.
	logical init( ENTITY *own );

	// used in place of : copy constructor ( with new owner )
	logical init( const ATTRIB_LOP_FACE &att );

public:

	// Simple read functions for the data members.
    FACE* face() const { return (FACE*)entity(); }
	const TWEAK* tweak_class() const { return _tweak_class; }
    SURFACE* tool_surface() const { return _tool_sf; }
	SURFACE* old_surface() const { return _old_sf; }
	double offset_distance() const { return _offset; }
	logical approximate_offset() const { return _approx_offset; }
	logical reparameterized_exact_offset() const { return _reparam_exact_offset; }
	logical need_par_int_curs() const { return _need_par_int_curs; }
	const error_info* offset_error_info() const;
	error_info_list& error_info_list() { return _err_info_list; }
	curve* neutral_curve() const;
	EDGE* taper_edge() const { return _taper_edge; }
	logical has_taper_edge_interval_override() const { return _has_interval_override; }
	SPAinterval taper_edge_interval_override() const { return _interval_override; }
	FACE* shadow_taper_silhouette_face() const { return _sil_face; }
	EDGE* shadow_taper_tweak_edge() const { return _tweak_edge; }
	EDGE* shadow_taper_silhouette_edge() const { return _sil_edge; }
	int shadow_taper_class() const { return _taper_class; }

	// Set methods for data members
	void set_tool_surface( SURFACE* tool_sf );
	void set_old_surface( SURFACE* old_sf );
	void set_offset_distance( double offset );
	void set_approximate_offset( logical approx_offset );
	void set_reparameterized_exact_offset( logical reparam_exact_offset );
	void set_need_par_int_curs( logical need_par_int_curs );
	//void set_offset_error_info(error_info* error_info);
	void set_error_info( error_info* error_info );
	void set_neutral_curve( curve*& neutral_curve );	// makes a copy and resets the pointer to the copy (so we can use CURVE's use-counting)
	void set_taper_edge( EDGE* taper_edge );
	void set_taper_edge_interval_override( SPAinterval& interval_override );
	void set_shadow_taper_silhouette_face( FACE* sil_face );
	void set_shadow_taper_tweak_edge( EDGE* tweak_edge);
	void set_shadow_taper_silhouette_edge( EDGE* sil_edge );
	void set_shadow_taper_class( int taper_class );

    // Constructors
	ATTRIB_LOP_FACE(); //Need public default constructor for ATTCOPY_DEF

	//appropriate init function is called after the construction is over
	//the constructors do not generate exceptions, however the init function may generate exception. in this case,
	//the function will return NULL. otherwise it will return a valid ATTRIB_LOP_FACE object.
	static ATTRIB_LOP_FACE* Make_ATTRIB_LOP_FACE( const TWEAK* tweak_class, ENTITY *own = NULL );

	// copy constructor ( with new owner )
	static ATTRIB_LOP_FACE* Make_ATTRIB_LOP_FACE( ENTITY *own, const ATTRIB_LOP_FACE &att );

	virtual logical pattern_copyable() const { return FALSE; }
	virtual logical deletable() const { return TRUE; }
	virtual logical savable() const { return FALSE; }

	virtual logical operator==( const ATTRIB_LOP_FACE& att ) const;

	ATTRIB_FUNCTIONS( ATTRIB_LOP_FACE, LOP )

	USE_COUNT_REFERENCE_DECL

#if defined D3_STANDALONE || defined D3_DEBUG

    void        print( D3_ostream& os, int level )      const;

#endif
};







extern DECL_LOP int ATTRIB_LOP_EDGE_TOOL_TYPE;
#define ATTRIB_LOP_EDGE_TOOL_LEVEL (ATTRIB_SYS_LEVEL + 1)

class DECL_LOP ATTRIB_LOP_EDGE_TOOL: public ATTRIB_SYS {
private:
	const TWEAK* _tweak_class;		// same face may be involved in nested TWEAK operations
									// e.g. offsetting calls remove
	// TWEAK data
    CURVE* _tool_crv;				// final surface
//	CURVE* _old_crv;				// original surface

	// private constructor, called from the static create methods.
	ATTRIB_LOP_EDGE_TOOL( const TWEAK* tweak_class, ENTITY *own );

	// private init functions, called from static create methods.
	logical init( ENTITY *own );

	// used in place of : copy constructor ( with new owner )
	logical init( const ATTRIB_LOP_EDGE_TOOL &att );

public:

	// Simple read functions for the data members.
    EDGE* edge() const { return (EDGE*)entity(); }
	const TWEAK* tweak_class() const { return _tweak_class; }
    CURVE* tool_curve() const { return _tool_crv; }
//	CURVE* old_curve() const { return _old_crv; }
	

	// Set methods for data members
	void set_tool_curve( CURVE* tool_crv );
//	void set_old_curve( CURVE* old_crv );

    // Constructors
	ATTRIB_LOP_EDGE_TOOL(); //Need public default constructor for ATTCOPY_DEF

	//appropriate init function is called after the construction is over
	//the constructors do not generate exceptions, however the init function may generate exception. in this case,
	//the function will return NULL. otherwise it will return a valid ATTRIB_LOP_EDGE_TOOL object.
	static ATTRIB_LOP_EDGE_TOOL* Make_ATTRIB_LOP_EDGE_TOOL( const TWEAK* tweak_class, ENTITY *own = NULL );

	// copy constructor ( with new owner )
	static ATTRIB_LOP_EDGE_TOOL* Make_ATTRIB_LOP_EDGE_TOOL( ENTITY *own, const ATTRIB_LOP_EDGE_TOOL &att );

	virtual logical pattern_copyable() const { return FALSE; }
	virtual logical deletable() const { return TRUE; }
	virtual logical savable() const { return FALSE; }

	virtual logical operator==( const ATTRIB_LOP_EDGE_TOOL& att ) const;

	ATTRIB_FUNCTIONS( ATTRIB_LOP_EDGE_TOOL, LOP )

	USE_COUNT_REFERENCE_DECL

#if defined D3_STANDALONE || defined D3_DEBUG

    void        print( D3_ostream& os, int level )      const;

#endif
};


extern DECL_LOP int ATTRIB_LOP_VERTEX_TOOL_TYPE;
#define ATTRIB_LOP_VERTEX_TOOL_LEVEL (ATTRIB_SYS_LEVEL + 1)

class DECL_LOP ATTRIB_LOP_VERTEX_TOOL: public ATTRIB_SYS {
private:
	const TWEAK* _tweak_class;		// same face may be involved in nested TWEAK operations
									// e.g. offsetting calls remove
	// TWEAK data
    APOINT* _tool_pt;				// final surface
//	APOINT* _old_pt;				// original surface

	// private constructor, called from the static create methods.
	ATTRIB_LOP_VERTEX_TOOL( const TWEAK* tweak_class, ENTITY *own );

	// private init functions, called from static create methods.
	logical init( ENTITY *own );

	// used in place of : copy constructor ( with new owner )
	logical init( const ATTRIB_LOP_VERTEX_TOOL &att );

public:

	// Simple read functions for the data members.
    VERTEX* vertex() const { return (VERTEX*)entity(); }
	const TWEAK* tweak_class() const { return _tweak_class; }
    APOINT* tool_point() const { return _tool_pt; }
//	APOINT* old_point() const { return _old_pt; }
	

	// Set methods for data members
	void set_tool_point( APOINT* tool_pt );
//	void set_old_point( APOINT* old_pt );

    // Constructors
	ATTRIB_LOP_VERTEX_TOOL(); //Need public default constructor for ATTCOPY_DEF

	//appropriate init function is called after the construction is over
	//the constructors do not generate exceptions, however the init function may generate exception. in this case,
	//the function will return NULL. otherwise it will return a valid ATTRIB_LOP_VERTEX_TOOL object.
	static ATTRIB_LOP_VERTEX_TOOL* Make_ATTRIB_LOP_VERTEX_TOOL( const TWEAK* tweak_class, ENTITY *own = NULL );

	// copy constructor ( with new owner )
	static ATTRIB_LOP_VERTEX_TOOL* Make_ATTRIB_LOP_VERTEX_TOOL( ENTITY *own, const ATTRIB_LOP_VERTEX_TOOL &att );

	virtual logical pattern_copyable() const { return FALSE; }
	virtual logical deletable() const { return TRUE; }
	virtual logical savable() const { return FALSE; }

	virtual logical operator==( const ATTRIB_LOP_VERTEX_TOOL& att ) const;

	ATTRIB_FUNCTIONS( ATTRIB_LOP_VERTEX_TOOL, LOP )

	USE_COUNT_REFERENCE_DECL

#if defined D3_STANDALONE || defined D3_DEBUG

    void        print( D3_ostream& os, int level )      const;

#endif
};

// wolf 01-16-09 moved from vertattr.cpp so that we can make one of these in OFFSET_THICKEN.
class temp_data : public ACIS_OBJECT
{
public:
	ATTRIB_LOP_VERTEX *alv;
	TWEAK *tw;
	SPAbox bx;
	int   n_lat_edge;
	logical failure;
	logical started;
	FACE *face;
	logical and_csis_for_tolerant_vertices;
	double tol;
	logical use_attributes;
	temp_data( ATTRIB_LOP_VERTEX *att, TWEAK *twk, const SPAbox& b )
		: alv( att ), tw( twk ), bx( b ), n_lat_edge( 0 ), 
		failure( FALSE ), started( FALSE ), face( NULL ), and_csis_for_tolerant_vertices( FALSE ), tol(SPAresabs),use_attributes(TRUE)
	{
		bx = b;
	}
	void init_chain_for_and() { started = FALSE; }
	void set_face( FACE *fa ) { face = fa; }
	void set_and_csis_for_tolerant_vertices( logical in_and_csis_for_tolerant_vertices ) { and_csis_for_tolerant_vertices = in_and_csis_for_tolerant_vertices; }
	void set_tol(double in_tol) {tol = in_tol;}
	void set_use_attributes(logical in_use_attributes) { use_attributes = in_use_attributes;}
};


// Global function to find an existing ATTRIB_LOP_FACE object
DECL_LOP ATTRIB_LOP_FACE* find_attrib_lop_face( const TWEAK* tweak_class, const FACE* face );
DECL_LOP ATTRIB_LOP_EDGE_TOOL* find_attrib_lop_edge_tool( const TWEAK* tweak_class, const EDGE* face );
DECL_LOP ATTRIB_LOP_VERTEX_TOOL* find_attrib_lop_vertex_tool( const TWEAK* tweak_class, const VERTEX* face );


#if defined D3_STANDALONE || defined D3_DEBUG

extern  DECL_LOP D3_ostream& operator<<( D3_ostream& os, const ATTRIB_LOP_FACE& );

#endif


#endif



