/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header file defining attributes for blending. There are two classes
// of blend attribute: those derived from ATTRIB_BLEND, which are
// attached to the body to be blended, and indicate the blend
// operations requested, and those derived from ATTRIB_BLINFO, which
// are attached to the blend sheet during blending to record its
// relationship with the blank body.

#if !defined( BLEND_ATTRIB )
#define BLEND_ATTRIB
#include "acis.hxx"
#include "dcl_blnd.h"
#include "param.hxx"
#include "unitvec.hxx"
#include "at_sys.hxx"
#include "errorsys.hxx"
#include "bl_tweak.hxx"
#include "bl_end.hxx"
#include "bl_enum.hxx"
#include "lists.hxx"
class SPAposition;
class SizeAccumulator;
class curve;
class pcurve;
class surface;
class plane;
class ENTITY;
class FACE;
class COEDGE;
class EDGE;
class VERTEX;
class SURFACE;
class BLEND_EDIT;
class ATT_BL_ENT_ENT;
class var_cross_section;
class segend;
class ffsegment;
class ffblend_geom;
class support_entity;
class blend1_data;
class blend_seq;
class blend_node;
class blend_debug_info;
class ENTITY_LIST;
class CURVE;

/**
* @file bl_att.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BLNDAPI
 *
 * @{
 */

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( ATTRIB_BLEND, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( ATTRIB_FFBLEND, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( ATTRIB_FBLEND, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( ATTRIB_BLEND_SUPPORT, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( ATTRIB_BLINFO, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( ATTRIB_EXPBLEND, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( ATTRIB_SPRING, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( ATTRIB_ADJEDGE, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( ATTRIB_ADJFACE, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( ATTRIB_XEDGE, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( ATTRIB_TAN_XEDGE, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( ATTRIB_XVERT, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( ATTRIB_XVERTED, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( ATTRIB_ECOIN, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( ATTRIB_XPOS, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif


// Implicit blend attributes. These are attached to entities on the
// body to be blended, to record the information necessary for the
// blending operation to take place.  They can also be attached to the
// faces made for the explicit blend (and find a use in blend editing).
// They record the type of blend, and any numerical data required to
// specify this instance.  They are all derived from a common base class.

// Enumerated types used below.

#include "bl_enum.hxx"

// The base class contains a list of blend sheet faces constructed for
// the implicit blend, a status indicator that notes whether the
// explicit blend geometry if any is known to agree with the implicit
// blend geometry if any, and a logical set to false if the attribute
// contains any out-of-body pointers (needed to control body scanning
// in copy and delete of blend sheet bodies and blend bodies with explicit
// blend attributes).

/**
 * @nodoc
 */
extern DECL_BLND int ATTRIB_BLEND_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_BLEND_LEVEL (ATTRIB_SYS_LEVEL + 1)

/**
 * Defines the basic blend attribute from which derive specific blend attributes.
 * <br>
 * <b>Role:</b> This class defines the basic blend attribute from which derive specific blend
 * attributes.
 */
class DECL_BLND ATTRIB_BLEND: public ATTRIB_SYS {
private:
	// List of blend sheet faces constructed for this blend.

	int nfaces_data;
	FACE **sheet_face_ptr;	
	
    blend_status bl_stat;
	err_mess_type err_mess_data;
	BLEND_EDIT *lop_ptr;
	logical suspended_flag;
	
    // Negative means that this attribute has been seen but not attempted.

    int num_attempts;	

    // Whether this is a user provided attribute or was generated internally
    // to process bi-blend gaps, narrow support regions and such.

    logical primary;

	// Initialized to FALSE, set to TRUE after split_owner
	logical split_child;

	//Specifies the blend curve direction
	blend_int_cur_dir _blend_int_cur_dir;

protected:

	logical no_out_of_body_ptrs_stat;

	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL

public:
/**
 * @nodoc
 */
    virtual logical pattern_compatible() const;
                                    // returns TRUE
/**
 * Status indicator which notes whether the explicit blend geometry is any 
 * known to agree with the implicit blend geometry.
 */
	logical no_out_of_body_ptrs() const
		{ return no_out_of_body_ptrs_stat; }
/**
 * @nodoc
 */
	void set_no_out_of_body_ptrs( logical tweak = FALSE );

	// Simple read functions for the data members.
/**
 * Returns the number of blend sheet faces constructed for this blend.
 */
	int nfaces() const { return nfaces_data;}

/**
 * Return a sheet face from the sheet face list.
 * <br><br>
 * <b>Role:</b>	Returns <tt>NULL</tt> if the index is out of range.
 * <br><br>
 * @param index
 * index of sheet face in list.
 */
	FACE *sheet_face( int index = 0 );
/**
 * Returns the sheet face list for this blend.
 */
	FACE **sheet_face_list() const { return sheet_face_ptr; }
/**
 * Read function for a data member, returns <tt>bl_stat</tt>.
 */
	blend_status bl_status() const { return bl_stat; }
/**
 * Returns a <tt>lop_ptr</tt>.
 * <br><br>
 * <b>Role:</b> Read function for data members.
 */
	BLEND_EDIT *lop() const { return lop_ptr; }
/**
 * @nodoc
 */
	logical is_suspended()  const { return suspended_flag; }
/**
 * @nodoc
 */
	logical been_seen()     const { return num_attempts != 0; }
/**
 * @nodoc
 */
	int times_attempted()   const { return num_attempts; }

/**
 * @nodoc
 */
    // Whether this is an original attribute or an inherited one
    // after split_owner call

    logical set_split_child( logical in = TRUE );

/**
 * @nodoc
 */
    logical is_split_child( void ) const { return split_child; }


/**
 * @nodoc
 */
    // Whether this is a user-provied attribute or was generated
    // internally.

    logical set_primary( logical pri = TRUE );

/**
 * @nodoc
 */
    logical is_primary( void ) const { return primary; }


	// Inquiry functions concerning size and shape.
	// Return the usual defaults at this level,
	// so that only unusual derived classes need to worry about it.
/**
 * Returns <tt>TRUE</tt> if the blend is circular.
 * <br><br>
 * <b>Role:</b> Returns the expected defaults so only unusually derived classes need to worry
 * about returning something different.
 */
	virtual logical is_circular() const { return TRUE; }

	// Returns TRUE if blend is a flat chamfer
/**
 * @nodoc
 */
	virtual logical is_flat_chamfer() const;


	// Returns TRUE if constant offset blend
	// This function returns positive left_offset and right_offset 
	// values irrespective of blend convexity.
/**
 * @nodoc
 */
	virtual logical is_constant_offset( 
						double &left_offset 
							= *( double *)NULL_REF, 
						double &right_offset 
							= *( double *)NULL_REF 
						) const;


/**
 * Returns <tt>TRUE</tt> if the blend end radius is zero ( less than <tt>resabs</tt>) at the start point if <tt>at_start</tt> is TRUE, or at the end point if <tt>at_start</tt> is FALSE.
 * <br><br>
 * @param start
 * at start.
 * @param tolerance
 * tolerance.
 */
	virtual logical zero_end_radius(
		logical start /*at_start*/, double tolerance /*tol*/ = SPAresabs ) const 
	{ 
		SPAUNUSED(start)
		SPAUNUSED(tolerance)
		return FALSE; 
	}
/**
 * Returns <tt>TRUE</tt> if the blend end radius slope is zero ( less than <tt>resabs</tt>) at the start point if <tt>at_start</tt> is TRUE, or at the end point if <tt>at_start</tt> is FALSE.
 * <br><br>
 * @param start
 * at start.
 * @param tolerance
 * tolerance.
 */
	virtual logical zero_end_rad_slope(
		logical start/*at_start*/, double tolerance /*tol*/ = SPAresabs ) const 
	{ 
		SPAUNUSED(start)
		SPAUNUSED(tolerance)
		return TRUE; 
	}

	// The blend attribute can record an error e.g. when a blend fails
	// and the attribute is to be moved to the end of the list for retry
	// later.  If the later attempts also fail, a failure message can be
	// displayed to indicate the original error.
/**
 * The blend attribute can record an error.
 * <br><br>
 * <b>Role:</b> For example, an error can be recorded when a blend fails and the attribute is
 * to be moved to the end of the list for retry later. If the later attempts also fail,
 * a failure message can be displayed to indicate the original error.
 */
	err_mess_type get_error_mess() const { return err_mess_data; }

	// This is the official function to discover what entities the
	// spring curves of a blend face run along, i.e. its "supports".
	// For a regular face-face blend, for example, a length of 2 will
	// be returned together with the array (on the heap).  An array
	// is used rather than an ENTITY_LIST so that entries can be null
	// (as can happen during deletions when explicit blend attributes
	// and support attributes exist, and can be deleted in any order).
/**
 * The "supports" of a blend are the entities the spring curves of the blend actually run along, which get added to the list.
 * <br><br>
 * <b>Role:</b> Any realizable attribute must implement this and return how many it added.
 * The official function to discover what entities the spring curves of a blend face run along,
 * i.e. its "supports". For a regular face-face blend, for example, a length of 2 will be returned
 * together with the array (on the heap). An array is used rather than an <tt>ENTITY_LIST</tt> so that
 * entries can be null (as can happen during deletions when explicit blend attributes and support
 * attributes exist, and can be deleted in any order).
 * <br><br>
 * @param entity1
 * starting entity.
 */
	virtual int supports( ENTITY **& entity1) const;

	// Member setting functions. These ensure that the attribute
	// is safely backed up before making the change.

/**
 * @nodoc
 */
	void set_sheet_face( FACE *face1 = NULL );
/**
 * @nodoc
 */
	void set_sheet_face_list( int n = 0, FACE **face1 = NULL );

	/**
	 * @nodoc
	 * Remove the blend face from the blend sheet. At times, capping may remove some
	 * or all blend faces in an attribute. 
	 */

	void remove_blend_face( FACE *face);

/**
 * @nodoc
 */
	void set_blend_status( blend_status blnd_stat = bl_stat_unset );
/**
 * @nodoc
 */
	void set_error_mess( err_mess_type errmess = 0 );
/**
 * @nodoc
 */
	void set_lop( BLEND_EDIT *Tweakptr = NULL );
/**
 * @nodoc
 */
	void suspend();
/**
 * @nodoc
 */
	void lift_suspension();
/**
 * @nodoc
 */
	void note_seen();
/**
 * @nodoc
 */
	void note_attempt();
/**
* @nodoc
*/
	//set method for blend int curve direction
	void set_blend_int_cur_dir(const blend_int_cur_dir& blend_int_cur_direction);
/**
* @nodoc
*/
	//get method for blend int curve direction
	blend_int_cur_dir get_blend_int_cur_dir()const;

	// Set i-th support entity of blend to be the given entity.
/**
 * @nodoc
 */
	virtual void set_support( int n , ENTITY *entity1);

	// Normalise the support attribute structure, i.e. discard any
	// support attribute on an entity not adjacent to the entity owning
	// the related blend attribute and set no_out_of_body_ptrs_stat true.
/**
 * @nodoc
 */
	virtual void normalise_supports();

	// Main constructor.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=ATTRIB_BLEND(...)</tt>), because this reserves the
 * memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param entity1
 * entity owner.
 * @param blend_status
 * blend status.
 */
	ATTRIB_BLEND(
				ENTITY *entity1 = NULL,			// owner
				blend_status blend_stat = bl_stat_unset
			);

	// Make a copy of a blend (setting implicit blend data only and
	// without attaching the copy to an entity).
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB BLEND(...)</tt>), because this reserves
 * the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param blnd
 * blend.
 */
	ATTRIB_BLEND( ATTRIB_BLEND const *blnd);
/**
 * Copies a blend and sets the implicit blend data only without attaching the copy to an entity.
 */
	virtual ATTRIB_BLEND* copy() const;

	// Test two blends for equality.
/**
 * Tests two blends for equality.
 * <br><br>
 * @param blnd
 * blend.
 */
	virtual logical operator==( ATTRIB_BLEND const& blnd) const;

	// Return a measure of continuity at blend spring curves.
/**
 * Returns a measure of continuity at blend spring curves.
 * <br><br>
 * <b>Role:</b> <tt>Unset_continuity</tt>, <tt>position_continuous</tt>, <tt>slope_continuous</tt>, or <tt>curvature_continuous</tt>.
 */
	virtual bl_continuity continuity( int sup_index ) const;

	// This is the entry point to the blend stage 1 processing that
	// makes a set of sheet faces for the blend attribute. The default
	// raises an error; any genuinely realisable derived attribute must
	// implement this.
	//
	// Its return values are:
	//   1: ok.
	//   0: the blend didn't work, but it might later, e.g., after
	//      surrounding geometry / topology is created.
	//  -1: this entire blend is never going to work.
/**
 * @nodoc
 */
	virtual int make_sheet_faces( blend1_data & data1, BODY *sheet_body );


	// At the start of blend stage 2, where boolean attributes are hung
	// on the blank and sheet bodies, attributes which contributed faces
	// to the sheet body get a chance to pre-empt the default
	// processing. This default function, therefore, does nothing,
	// leaving everything to the existing default code. The logical
	// return value may indicate a status code in future, although it is
	// not currently used.
/**
 * @nodoc
 */
	virtual logical attach_ints(
						BODY *tool,		// sheet body
						BODY *blank		// blended body
						);

	// Recompute blend geometry for tweaking.

/**
 * @nodoc
 */
	virtual blend_info *recompute_blend_geom(
						int nblnd,
						FACE ** array1		// array of blend faces
			);

// Returns whether to accept a zero face state.
/**
 * @nodoc
 */
	virtual logical accept_zero_face_attr( )
		{ return( FALSE ); }

	// enquire
/**
 * @nodoc
 */
	virtual blend_debug_info* enquire(FILE*fp);

/**
 * @nodoc
 */
	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_BLEND, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

/**
 * @nodoc
 */
DECL_BLND void delete_blend(ENTITY *);
/**
 * @nodoc
 */
DECL_BLND void delete_exp_blend(FACE *);

// A routine to test if two faces are adjacent and if so to return a common
// edge or vertex, otherwise null.

/**
 * @nodoc
 */
DECL_BLND ENTITY *adjacent(FACE *, FACE *);


// When an ATTRIB_FFBLEND makes left and right support_entities, we wish
// the blend attribute to take on ownership of these. Sometimes,
// however, the same attribute may make several pairs (when
// fill_gap_in_chain is used, or bi-blends happen), and we then need to
// own all of them. So instead of keeping a single list of
// support_entities, we maintain a stack of such lists. This means that
// it is not necessarily trivial finding the "right" supports from the
// attribute, but this should not really matter as the code always
// maintains the correct ones in context.

/**
 * @nodoc
 */
struct support_entity_stack
{
	/**
	 * @nodoc
     */
	support_entity_stack( support_entity *l, support_entity_stack *n );
	~support_entity_stack();
	
	support_entity *list;
	support_entity_stack *next;

	void set_narrow_support( support_entity *n_sup, const SPAinterval &o_i );

	logical narrow_support( void ) const;

	support_entity *_narrow_support;
	SPAinterval    _other_imprint;

	/**
	 * @nodoc
	 */
	MMGR_SUPPORT_THIS
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

};

/**
 * @nodoc
 */
class support_entity_dlist 
{
public:

	struct support_entity_dlist_elem : public ACIS_OBJECT
	{

		/**
	    * @nodoc
        */
	    support_entity_dlist_elem( 
		    support_entity *l, 
		    support_entity_dlist_elem *n = NULL, 
		    support_entity_dlist_elem *p = NULL
		    )
		   : _list( l ), _next( n ), _prev( p ) 
		{}


		support_entity *_list;
	    support_entity_dlist_elem *_next;
	    support_entity_dlist_elem *_prev;

	    /**
	    * @nodoc
	    */
	    MMGR_SUPPORT_THIS
        #if 0
        ; // semicolon needed for mkman (doc tool) parsing
        #endif
	};

	/**
	 * @nodoc
     */
	support_entity_dlist( support_entity *l = NULL );

	void  append( support_entity *l );
	void  prepend( support_entity *l );

	void  init( logical fwd = TRUE );

	support_entity_dlist_elem *next( void );
	support_entity_dlist_elem *prev( void );

	~support_entity_dlist( void );

private:

	support_entity_dlist( const support_entity_dlist & );
	support_entity_dlist &operator=( const support_entity_dlist & );

	support_entity_dlist_elem *_first;
	support_entity_dlist_elem *_last;
	support_entity_dlist_elem *_curr;

	logical first_call;
};


// A structure to cache the left, right support faces reordering
// information.

/**
 * @nodoc
 */

struct DECL_BLND bl_reorder_info
{
    enum bl_face_reorder_info
	{
	    ro_not_reqd = 0x0,
	    ro_const_radius,
	    ro_variable_radius,
	    ro_impossible,
	    ro_unset
	};

	bl_face_reorder_info  _left_info;
	bl_face_reorder_info  _right_info;

	logical _rev;

	bl_reorder_info( 
				bl_face_reorder_info l = ro_unset,
		        bl_face_reorder_info r = ro_unset,
				logical rev = FALSE 
				)
		: _left_info( l ), _right_info( r ), _rev( rev ) {}

	logical unset( void ) const 
	{ 
		return _left_info == ro_unset && _right_info == ro_unset; 
	}

	int   reorder_no( void ) const
	{
		if ( _left_info == ro_impossible || _right_info == ro_impossible ) 
			return 0;

		int l_no = needs_reorder( _left_info ) ? 1 : 0;
		int r_no = needs_reorder( _right_info ) ? 1 : 0;
		int acc_no = l_no + r_no;
		if ( reversed() )
			acc_no = acc_no > 0 ? 0 : 1;
		return acc_no;
	}

	logical  needs_reorder( bl_face_reorder_info i ) const
	{
		return i == ro_const_radius || i == ro_variable_radius;
	}

	logical reversed( void ) const { return _rev; }
	void  set_reversed( logical  r = TRUE ) { _rev = r; }

};

/**
 * @nodoc
 */
extern DECL_BLND int ATTRIB_FFBLEND_TYPE;
#define ATTRIB_FFBLEND_LEVEL (ATTRIB_BLEND_LEVEL + 1)

/**
 * Defines a general face-face blend.
 * <br>
 * <b>Role:</b> <tt>ATTRIB_FFBLEND </tt>is a face-to-face blend, and records the blend convexity,
 * the faces that the blend runs between, the setback at either end of the owning entity
 * (when an edge), and the defining plane so an approximate starting position for a
 * rolling ball can be specified.
 * <br><br>
 * Usually local geometry supplies these details, but if not, the user must set one or more
 * of them directly. A blend can be of zero size but have nonzero setbacks.
 * <br>
 * @see ATTRIB_CONST_CHAMFER, ATTRIB_CONST_ROUND, ATTRIB_VAR_BLEND
 */
class DECL_BLND ATTRIB_FFBLEND: public ATTRIB_BLEND {
 private:
	// Blend convexity, derived from the convexity of the owning
	// edge if any, but otherwise is found from adjacent blends
	// or is given explicitly.

	bl_ed_convexity cvxty_data;

	// Faces that blend runs between.  Can be unset if the owner is an
	// edge; otherwise must be set before blend geometry can be found.

	FACE *left_face_ptr;
	FACE *right_face_ptr;

	// Setbacks are only meaningful if the owner is an edge.

	double setback_at_start_data;	// zero or positive
	double setback_at_end_data;

	// At each end, declare a setback difference equal to
	// right_setback - left_setback, to permit oblique cross curves.
	// The value can be zero, positive or negative.
	// Also provide at each end a logical to note when the setback
	// difference is set or not.  If the difference is not set, the
	// setback will be found from local intersections of spring curves
	// and a (usually) oblique cross curve will be made (provided the
	// given setback is small enough).  The default is a zero difference
	// with the setback difference noted as set.

	double sbdiff_at_start_data;
	double sbdiff_at_end_data;
	logical sbdiff_set_at_start_data;
	logical sbdiff_set_at_end_data;

	// For stopped blends, the angle of the "stopping plane".

	double start_stop_angle_data, end_stop_angle_data;

	// Define what is to happen when a face-face blend, or rather a spring
	// curve of a face-face blend, meets a smooth or non-smooth edge in the
	// middle of the blend sequence (see description above where blend_how
	// is defined).

	blend_how bl_how_data;

	// Define a plane that cuts the blend.  Must be set if the
	// blend is an isolated face-face blend when it specifies
	// a plane where the blend process can start.

	plane *def_plane_ptr;

	// Record condition at the start and end of a blend.

	bl_end_condition bl_end_con_at_start_data;
	bl_end_condition bl_end_con_at_end_data;

	// Maintain a stack of support_entity lists for this attribute.

	support_entity_stack *support_stack_ptr;

	// For caching the return value of the "needs_reorder()" member
	// function.

	bl_reorder_info  ro_info;

    // Whether to accept a zero face state.

	logical acpt_zero_face_attr;

	// The old implementation of set_ffblends which wasn't optimized enough

	void set_ffblends_preR17(ENTITY_LIST const &list_entity) const;

	//Flag for a reorder blend face
	logical reorder_blend;

protected:

	// The existence of this stack of supports which which cannot
	// reasonably be copied throughout rollback means we have to be a
	// bit more devious and perform an ATTRIB_EFINT-like stunt when the
	// rollback copy is restored.

	void roll_notify( BULLETIN_TYPE, ENTITY * );

	// Record a copy of blend geometry to save recomputing it
	// (replaces earlier separate cache).

	ffblend_geom *ffblend_geom_ptr;

	// Account for transforms present on the body within blending.

	virtual void trans_data( SPAtransf const & );

	// Find the spine geometry for a blend with constant left and right 
	// oriented offsets. 

	ffblend_geom *find_const_ffblend_spine(
										double left_offset,
										double right_offset,
										logical open_at_start,
										logical open_at_end,
										segend *start_segend,
										segend *end_segend,
										SPAbox const &region,
										var_cross_section const &cs
										);

	// Find the spring curves geometry for a blend with constant left 
	// and right oriented offsets.

	ffblend_geom *find_const_ffblend_springs(
										ffblend_geom *spine_ffg,
										double left_offset,
										double right_offset,
										logical open_at_start,
										logical open_at_end,
										segend *start_segend,
										segend *end_segend,
										SPAbox const &region
										);
	
	// Given blend boundary details in a ffblend_geom, find the
	// surface and return it and the given boundary details in a
	// ffblend_geom.

	// This interface can be used only for blends with constant 
	// left and right oriented offsets

	ffblend_geom *find_const_ffblend_surface( 
										ffblend_geom *blg,
										double left_offset,
										double right_offset,
										var_cross_section const &cs
										);

	// Find cross curve in given plane (must be normal to spine and
	// spring curves).  The curve found runs from left to right across
	// the blend.
/**
 * @nodoc
 */
	curve *find_const_ffblend_cross_curve(
									ffblend_geom *blg,
									plane const &on_pln,
									SPAbox const &in_box,
									var_cross_section const &cs,
									SPAposition const &left_pos = 
										*(SPAposition const *)NULL_REF,	
									SPAposition const &right_pos = 
										*(SPAposition const *)NULL_REF, 
									SPAposition const &spine_pos = 
										*(SPAposition const *)NULL_REF	
									);

    // Find the blend geometry for a blend with constant left / right
    // oriented offsets. Uses the above functions for its processing.

    ffblend_geom *find_const_ffblend_geometry(
			                            double left_offset,
			                            double right_offset,
			                            logical open_at_start,
			                            logical open_at_end,
			                            segend	*start_segend,
			                            segend	*end_segend,
			                            SPAbox const &region,
										var_cross_section const &cs
			                            );

	// The enum supplying information to virtual fn set_ffblend so as to
	// help decide whether attrib reversal is required or not.

	enum set_ffblend_input
	{
		att_unset,
		att_first,
		att_fwd,
		att_rev
	};

	// Set this ffblend on the given edge having first removed any 
	// existing ffblend attribute.

	virtual void set_ffblend( EDGE *edge, set_ffblend_input &en );


public:
/**
 * @nodoc
 */
     virtual logical pattern_compatible() const;

	// Simple cache for storing the segend used for computing
	// the vertex blend boundary.

/**
 * @nodoc
 */
	segend * vbl_segend;


	// Constructors.  The owner will often be an edge but can be a
	// shell, lump or body for blends between distant faces.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY </tt>class (for example, <tt>x=new ATTRIB_FFBLEND(...)</tt>), because this reserves
 * the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param entity1
 * entity owner.
 * @param setbk_start
 * setback at start.
 * @param setbk_end
 * setback at end.
 * @param lface
 * left face.
 * @param rface
 * right face.
 * @param dstart
 * start setback difference.
 * @param dend
 * end setback difference.
 * @param set_start
 * set start setback.
 * @param set_end
 * set end setback.
 * @param set_midblnd
 * set mid-blend behavior.
 * @param blnd_sts
 * blend status.
 * @param endcond_start
 * bl_end_condition at start.
 * @param endcond_end
 * bl_end_condition at end.
 * @param start_stop_ang
 * start stop angle.
 * @param end_stop_ang
 * end stop angle.
 */
	ATTRIB_FFBLEND(
				ENTITY * entity1= NULL,			// owner
				double setbk_start = 0,					// setback at start
				double setbk_end = 0,					// setback at end
				FACE * lface= NULL,				// left face
				FACE * rface= NULL,				// right face
				double dstart= 0,					// setback diff at start
				double dend= 0,					// setback diff at end
				logical set_start= TRUE,				// setback diff set at start
				logical set_end = TRUE,				// setback diff set at end
				blend_how set_midblnd= bl_how_default,	// set mid-blend behaviour
				blend_status blnd_sts= bl_stat_unset,
				bl_end_condition  endcond_start= bl_end_unset,	// bl_end_condit at start
				bl_end_condition endcond_end= bl_end_unset,	// bl_end_condit at end
		        double start_stop_ang = 0.0,	// start stop angle
		        double end_stop_ang = 0.0,		// end stop angle
				logical reorder_blend = FALSE		//reorder blend
			);

	// Make a copy of a blend (setting implicit blend data only and
	// without attaching the copy to an entity).
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY </tt>class (for example, <tt>x=new ATTRIB_FFBLEND(...)</tt>), because this reserves
 * the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param exist_blnd
 * existing blend.
 */
	ATTRIB_FFBLEND( ATTRIB_FFBLEND const *exist_blnd);
/**
 * @nodoc
 */
	virtual ATTRIB_BLEND* copy() const;

	// Simple read functions for the data members.
/**
 * Returns the blend convexity,non-const version.
 */
	bl_ed_convexity cvxty();
/**
 * Returns the blend convexity, which derives from the convexity of the owning edge if any; otherwise, it is found from adjacent blends or is given explicitly.
 */
	bl_ed_convexity cvxty() const;
/**
 * Returns TRUE if the blend is convex; otherwise, it returns FALSE.
 */
	logical convex() const;

/**
 * Returns the left face along which the blend runs.
 * <br><br>
 * <b>Role:</b> This can be unset if the owner is an edge; otherwise, set it
 * before finding blend geometry.
 */
	FACE *left_face() const;

/**
 * Returns the left face, non-const version.
 */
	FACE *left_face();

/**
 * Returns the right face along which the blend runs.
 * <br><br>
 * <b>Role:</b> This can be unset if the owner is an edge; otherwise, set it before finding blend
 * geometry.
 */
	FACE *right_face() const;

/**
 * Returns the right face, non-const version.
 */
	FACE *right_face();
/**
 * Returns the setback at start, a setback is meaningful only if the owner is an edge.
 */
	double setback_at_start() const { return setback_at_start_data; }
/**
 * Returns the setback at end, a setback is meaningful only if the owner is an edge.
 */
	double setback_at_end() const { return setback_at_end_data; }
/**
 * Identifies the setback as different at start.
 */
	double setback_diff_at_start() const { return sbdiff_at_start_data; }
/**
 * Identifies the setback as different at end.
 */
	double setback_diff_at_end() const { return sbdiff_at_end_data; }
/**
 * Sets the different setback to the start.
 */
	logical sbdiff_set_at_start() const { return sbdiff_set_at_start_data; }
/**
 * Sets the different setback to the end.
 */
	logical sbdiff_set_at_end() const { return sbdiff_set_at_end_data; }
/**
 * Returns the <tt>bl_how_data</tt>, which is the mid-blend behavior.
 */
	blend_how bl_how() const { return bl_how_data; }
/**
 * Returns the reference plane, which is necessary if the owning entity is not an edge.
 */
	plane def_plane() const;
/**
 * Returns the reference plane, non-const version.
 */
	plane def_plane();
/**
 * Returns the <tt>bl_end_con_at_start_data</tt> data, which records the condition at the start of the blend.
 * <br><br>
 * <b>Role:</b> The values are defined in the enum <tt>bl_end_condition</tt>.
 */
	bl_end_condition bl_end_con_at_start() const
		{ return bl_end_con_at_start_data; }
/**
 * Returns the <tt>bl_end_con_at_end_data</tt> data, which records the condition at the end of the blend.
 * <br><br>
 * <b>Role:</b> The values are defined in the enum <tt>bl_end_condition</tt>.
 */
	bl_end_condition bl_end_con_at_end() const
		{ return bl_end_con_at_end_data; }

/**
 * Returns the <tt>support_stack_ptr</tt>, which maintains a stack of <tt>support_entity</tt> lists for this attribute.
 */
	support_entity_stack *support_stack() const { return support_stack_ptr; }
/**
 * Returns the <tt>start_stop_angle_data</tt> (for stopped blends, this is the start angle of the stopping plane).
 */
	double start_stop_angle() const { return start_stop_angle_data; }
/**
 * Returns the <tt>end_stop_angle_data</tt> (for stopped blends, this is the end angle of the stopping plane).
 */
	double end_stop_angle() const { return end_stop_angle_data; }

	// If this attribute is owned by a face and lop (the current local op) is
	// set, these functions get the (possibly) tweaked surface of the left
	// or right support faces.

/**
 * If this attribute is owned by a face and the current local operation is set, these functions get the (possibly) tweaked surface of the left support faces.
 */
	SURFACE *left_tw_surf() const;

/**
 * If this attribute is owned by a face and the current local operation is set, these functions get the (possibly) tweaked surface of the right support faces.
 */
	SURFACE *right_tw_surf() const;

	// This is the official function to discover what entities the
	// spring curves of a blend face run along, i.e. its "supports".
	// For a regular face-face blend, for example, a length of 2 will
	// be returned together with the array (on the heap).  An array
	// is used rather than an ENTITY_LIST so that entries can be null
	// (as can happen during deletions when explicit blend attributes
	// and support attributes exist, and can be deleted in any order).
/**
 * @nodoc
 */
	virtual int supports( ENTITY ** &start_entity ) const;

	// Return the number of side faces that are (constant round) blends
	// of a smaller radius (and same convexity) as ourselves, and
	// therefore will require some blend re-ordering. This function will
	// therefore normally return 0, but sometimes 1 or even 2 (if we're
	// blending back over a mitre of smaller blends).
/**
 * @nodoc
 */
	bl_reorder_info  reorder();

	// Whether this blend needs reordering. Normally, therefore, this
	// returns TRUE iff reorder() (q.v.) returns non-zero. However,
	// there are occasionally blends that need reordering even when the
	// faces either side would not obviously suggest it.  For such
	// blends this function will be arranged to return TRUE. (Note that
	// reorder() returning non-zero remains a sufficient condition.)
/**
 * @nodoc
 */
	logical needs_reorder();	// non-const as it may cache its return value

	// Return whether the left face means we need to compute some
	// reordered blends.  
	// NOTE: derived versions of these should invoke these (parent) methods
	// to check for "impossible".
/**
 * @nodoc
 */
	virtual bl_reorder_info::bl_face_reorder_info 
		        left_reorder(logical &convex = *(logical*)NULL_REF);
/**
 * @nodoc
 */
	virtual bl_reorder_info::bl_face_reorder_info 
		        right_reorder(logical &convex = *(logical*)NULL_REF);

	// Member setting functions. These ensure that the attribute
	// is safely backed up before making the change.

/**
 * Sets the blend convexity, which derives from the convexity of the owning edge if any; otherwise, it is found from adjacent blends or is given explicitly.
 * <br><br>
 * @param con_val
 * convexity value.
 */
	void set_cvxty( bl_ed_convexity con_val );
/**
 * Sets the value of the left face.
 * <br><br>
 * <b>Role</b>: The left face can be unset if the owner is an edge; otherwise,
 * it must be set before blend geometry can be found.
 * <br><br>
 * @param lface
 * left face value.
 */
	void set_left_face( FACE * lface);
/**
 * Sets the value of the right face.
 * <br><br>
 * <b>Role:</b> The right face can be unset if the owner is an edge; otherwise,
 * it must be set before blend geometry can be found.
 * <br><br>
 * @param rface
 * right face value.
 */
	void set_right_face( FACE * rface);

/**
 * @nodoc
 */
    // Sets the value of the left and right support faces.
    // It also swaps the right and left blend data if 'reversed' 
    // flag is set to TRUE

	virtual void set_support_faces( 
		FACE *left_face,             // left support face
		FACE *right_face,            // right support face
		logical reversed             //flag to indicate whether the blend data 
                                     // needs to be swapped between left and right sides
		);
/**
 * Sets the value for the setback at start.
 * <br><br>
 * <b>Role:</b> Setbacks are only meaningful if the owner is an edge.
 * <br><br>
 * @param setbk_start
 * setback at start.
 */
	void set_setback_at_start( double setbk_start);
/**
 * Sets the value for the setback at end.
 * <br><br>
 * <b>Role:</b> Setbacks are only meaningful if the owner is an edge.
 * <br><br>
 * @param setbk_end
 * setback at end.
 */
	void set_setback_at_end( double setbk_end);
/**
 * Sets the setback difference to the start.
 * <br><br>
 * @param dsetbk_start
 * real.
 */
	void set_setback_diff_at_start( double dsetbk_start);
/**
 * Sets the setback difference to the end.
 * <br><br>
 * @param dsetbk_end
 * real.
 */
	void set_setback_diff_at_end( double dsetbk_end);
/**
 * Resets the different setback to the start.
 * <br><br>
 * @param reset_startsetbk
 * logical.
 */
	void set_sbdiff_set_at_start( logical reset_startsetbk);
/**
 * Resets the different setback to the end.
 * <br><br>
 * @param reset_endsetbk
 * logical.
 */
	void set_sbdiff_set_at_end( logical reset_endsetbk);
/**
 * Sets the mid-blend behavior.
 * <br><br>
 * <b>Role:</b> This defines what is to happen when a face-face blend, or rather
 * a spring curve of a face-face blend, meets a smooth or nonsmooth edge in the
 * middle of the blend sequence.
 * <br><br>
 * @param mblnd
 * mid-blend behavior.
 */
	void set_bl_how( blend_how  mblnd);
/**
 * Defines a plane that cuts the blend.
 * <br><br>
 * <b>Role:</b> It must be set if the blend is an isolated face-face blend when it
 * specifies a plane where the blend process can start.
 * <br><br>
 * @param plane1
 * plane.
 */
	void set_def_plane( plane const & plane1);
/**
 * Sets the <tt>bl_end_con_at_start_data</tt> data, which records the condition at the start of the blend.
 * <br><br>
 * <b>Role:</b> The values are defined in the enum <tt>bl_end_condition</tt>.
 * <br><br>
 * @param bend_cond
 * new value.
 */
	void set_bl_end_con_at_start( bl_end_condition bend_cond );
/**
 * Sets the <tt>bl_end_con_at_end_data</tt> data, which records the condition at the end of the blend.
 * <br><br>
 * <b>Role:</b> The values are defined in the enum <tt>bl_end_condition</tt>.
 * @param bend_cond
 * new value.
 */
	void set_bl_end_con_at_end( bl_end_condition bend_cond);
/**
 * @nodoc
 */
	void set_support_stack( support_entity_stack * supp_ptr);
/**
 * Sets the <tt>start_stop_angle_data</tt> (for stopped blends, this is the start angle of the stopping plane).
 * <br><br>
 * @param angle
 * new angle.
 */
	void set_start_stop_angle( double angle);
/**
 * Sets the <tt>end_stop_angle_data</tt> (for stopped blends, this is the end angle of the stopping plane").
 * <br><br>
 * @param angle
 * new angle.
 */
	void set_end_stop_angle( double angle);
/**
 * @nodoc
 */
	void set_needs_reorder( logical ro );

	// Set i-th support entity of blend to be the given entity.
	// i is 0 for left, 1 for right.
/**
 * @nodoc
 */
	virtual void set_support( int nsupp, ENTITY *supp_entity);

	// Normalise the support attribute structure, i.e. discard any
	// support attribute on an entity not adjacent to the entity owning
	// the related blend attribute and set no_out_of_body_ptrs_stat true.
/**
 * @nodoc
 */
	virtual void normalise_supports();

	// Reverse left-to-right properties of an asymmetric blend.
/**
 * Reverses the left-to-right properties of an asymmetric blend.
 */
	virtual void reverse();		// pure

	// Functions called to aid attribute migration during modelling
	// operations.
/**
 * @nodoc
 */
	virtual void split_owner( ENTITY * entity);
									// the owner of this attribute is
									// about to be split in two - the
									// argument is the new piece.

	// Associate setbacks to new and old edges (made virtual so that e.g.
	// variable radius blends can redefine it to do nothing).
/**
 * @nodoc
 */
	virtual void split_setbacks( ATTRIB_FFBLEND * entity);
/**
 * @nodoc
 */
	virtual void merge_owner( ENTITY *entity, logical del_owner );
									// the owner of this attribute is
									// about to be merged with the
									// given entity. The logical
									// argument is TRUE if the owner is
									// to be deleted in the merge.

	// A further function to allow geometric attributes to transform
	// themselves when their owner is transformed.
/**
 * @nodoc
 */
	virtual void trans_owner(
				SPAtransf const & trans );

// Account for transforms present on the body within blending.
/**
 * @nodoc
 */
	virtual void trans_blend( logical, logical );

	// Test two blends for equality.
/**
 * @nodoc
 */
	virtual logical operator==( ATTRIB_BLEND const& blnd) const;

	// Return the v-SPAparameter range.
	// This is not guaranteed to be identical to that of the finished blend.
	// It is often the same as that of the owning edge.
/**
 * Return the v-parameter range.
 * <br><br>
 * <b>Role:</b> This is not guaranteed to be identical to that of the finished
 * blend. It is often the same as that of the owning edge.
 */
	virtual SPAinterval v_param_range() const;

	// Return a measure of continuity at blend spring curves
	// i.e. unset_continuity, position_continuous, slope_continuous,
	// or curvature_continuous.
/**
 * Returns the continuity of the blend at the spring curves.
 */
	virtual bl_continuity continuity( int sup_index ) const;

	// Test if blend is of zero width i.e. no faces need be made
	// but usually a setback will have been recorded at one or both
	// ends of the edge.
/**
 * Tests if blend is of zero width.
 * <br><br>
 * <b>Role:</b> No faces need to be made, but a setback has been recorded at
 * one or both ends of the edge.
 */
	virtual logical is_zero() const;

	// Return a measure of the size of the blend. Specifically, this means
	// how far the blend spine may be from the underlying surfaces. So for
	// a constant radius rolling ball, it's just the radius. Returning less
	// than this maximum offset value risks unreliability in geometrically
	// extremish (perhaps sharp or shallow) situations.
/**
 * Returns a measure of the size of the blend cross-section (how far the blend spine may be from the underlying surfaces).
 * <br><br>
 * @param at_start
 * at start.
 */
	virtual double blend_size( logical at_start  = TRUE) const;

	// Return a measure of the size of the blend at one end of the edge.
	// For any constant-radius blends, this is equivalent to blend_size.
	// Return that as a default.
/**
 * Returns a measure of the size of the blend at one end of the edge.
 * <br><br>
 * <b>Role:</b> For any constant-radius blends, this is equivalent to <tt>blend_size</tt>. This should only be called for derived classes.
 * <br><br>
 * @param at_start
 * at start.
 */
	virtual double blend_size_at_vtx( logical at_start  = TRUE ) const
	{ return blend_size( at_start ); }

	// The 'lateral size' is a notion of how far away from the blended edge
	// the spring curves get. So for a constant chamfer it's either the
	// left or right range. This number is not critical, it is used only in
	// autosetback calculations.

	// The coedge argument is necessary just to identify which end of the
	// edge we're interested in - namely near the vertex which STARTS the
	// coedge.
/**
 * Returns the lateral size.
 * <br><br>
 * <b>Role:</b> Coedge is necessary to identify the end of the edge, which is near the vertex
 * that starts the coedge.
 * <br><br>
 * @param coedge1
 * coedge.
 */
	virtual double lateral_size(COEDGE *coedge1) const;

	// Set this ffblend on the given entity having first removed
	// any existing ffblend attribute.
/**
 * Sets this <tt>ffblend</tt> on the given entity having first removed any existing ffblend attribute.
 * <br><br>
 * @param entity
 * entity.
 */
	void set_ffblend(ENTITY *entity);

	// Set copies of this ffblend on a list of edges.  Blends are
	// assigned directly to the first edge (in the list) of each
	// connected set of edges.  Remaining edges in the set have the
	// blend assigned based on their relative sense (edges connected
	// end to start or start to end have the same sense, those
	// connected start to start or end to end have opposite sense).
	// Copies include setbacks.
/**
 * Sets copies of this <tt>ffblend</tt> on a list of edges.
 * <br><br>
 * <b>Role:</b> Blends are assigned directly to the first edge (in the list) of each connected
 * set of edges. Remaining edges in the set have the blend assigned based on their relative sense
 * (edges connected end-to-start or start-to-end have the same sense, those connected start-to-start
 * or end-to-end have opposite sense). The copies include setbacks.
 * <br><br>
 * @param list_entity
 * list of entities.
 */
	void set_ffblends(ENTITY_LIST const &list_entity) const;

	// Find details of blend geometry i.e. spring curves, auxiliary
	// surfaces and blend surface.

/**
 * @nodoc
 */
	virtual ffblend_geom *find_ffblend_geometry(
		logical op_start= FALSE,			// open at start
		logical op_end = FALSE,			// open at end
		segend * start_seg= NULL,			// start segend if any
		segend * end_seg= NULL,			// end segend if any
		SPAbox const &bound_box = *(SPAbox const *)NULL_REF,
        double & reduced_resabs = *(double*)       NULL_REF
	  );

	// Find the spine curve or point.

/**
 * @nodoc
 */
	virtual ffblend_geom *find_ffblend_spine(
		logical op_start,			// open at start
		logical op_end,			// open at end
		segend * start_seg,			// start segend if any
		segend * end_seg,			// end segend if any
		SPAbox const & bound_box= *(SPAbox const *)NULL_REF
	  );

	// Find spring curves.  The curves returned have the same sense
	// as the spine.
/**
 * @nodoc
 */
	virtual ffblend_geom *find_ffblend_spring_curves(
		ffblend_geom * spine,		// contains spine
		logical op_start ,			// open at start
		logical op_end,			// open at end
		segend * start_seg,			// start segend if any
		segend * end_seg,			// end segend if any
		SPAbox const & bound_box = *(SPAbox const *)NULL_REF
	  );

	// Find cross curve in given plane (must be normal to spine and
	// spring curves).  The curve found runs from left to right across
	// the blend.
/**
 * @nodoc
 */
	virtual curve *find_ffblend_cross_curve(
		ffblend_geom *,
		plane const &,
		SPAbox const &,
		SPAposition & = *(SPAposition *)NULL_REF,	// left_pos
		SPAposition & = *(SPAposition *)NULL_REF, // right_pos
		SPAposition & = *(SPAposition *)NULL_REF	// spine_pos
	  );

	// Given blend boundary details in a ffblend_geom, find the
	// blend surface and return it and the given boundary details
	// in a ffblend_geom.
/**
 * @nodoc
 */
	virtual ffblend_geom *find_ffblend_surface(ffblend_geom *);

	// Find SPAbox enclosing blend.
/**
 * @nodoc
 */
	virtual SPAbox find_ffblend_region(
		logical op_start,			// open at start
		logical op_end,			// open at end
		segend *start_seg,			// start segend if any
		segend *end_seg			// end segend if any
	  );

	// Remove any cached geometry data. 
/**
 * @nodoc
 */
	virtual void remove_ffblend_geometry();

	// This is the entry point to the blend stage 1 processing that
	// makes a set of sheet faces for the blend attribute. For face-face
	// blends this method calls the existing function
	// make_sheet_faces_for_ff.
	//
	// Its return values are:
	//   1: ok.
	//   0: the blend didn't work, but it might later, e.g., after
	//      surrounding geometry / topology is created.
	//  -1: this entire blend is never going to work.
/**
 * @nodoc
 */
	virtual int make_sheet_faces( blend1_data & data_blnd1, BODY *sheet_body );

/**
 * @nodoc
 */
	virtual blend_info *recompute_blend_geom(
							int nblnd,
							FACE **	blnd_face	// array of blend faces
			);


	// Sample the spine of a blend at intervals.
	// Sample at n_samples locations, unless n_samples < 2,
	// then sample according to tol.
	// If n_samples < 2 and tol < SPAresabs, then tol is set to SPAresfit.
	// The resulting points are *approximations* to the actual spine positions.
/**
 * @nodoc
 */
	virtual void sample_spine( 
                int n_samples, 
                double tol,
				int &npts, 
                SPAposition* &pts_ptr, 
                double *params = NULL,
                SPAunit_vector &start_dir =
                  * ( SPAunit_vector * ) NULL_REF,
                SPAunit_vector &end_dir =
                  * ( SPAunit_vector * ) NULL_REF
                );

	// If a face-face blend will not fit anywhere along the blend,
	// attempt to create an entity-entity blend. Optionally a v parameter
	// can be specifed where the new set of support entities will be searched.

/**
 * @nodoc
 */
	ATT_BL_ENT_ENT *make_ent_ent_blend( 
							SPAparameter const &v_param = 
								* ( SPAparameter * ) NULL_REF 
							);

    // Return if an entity-entity blend can be done for this type of
    // blend.
/**
 * @nodoc
 */
    virtual logical ent_ent_blend_implemented() const { return TRUE; }

	// If a face-face blend will not fit anywhere along the blend,
	// find entities for an entity-entity blend.
/**
 * @nodoc
 */
	void find_ee_ents(
			ENTITY *&left_ent,	// output: entity in left face
			ENTITY *&right_ent,	// output: entity in right face
			SPAposition &spine_pt  // output: spine SPAposition
		);

	// The same, but the attempt is made at a particular v-SPAparameter.
	// This method is invoked by the previous one.
/**
 * @nodoc
 */
	void find_ee_ents(
			double v_param,		// SPAparameter along the blend
			ENTITY *&left_ent,	// output: entity in left face
			ENTITY *&right_ent,	// output: entity in right face
			SPAposition &spine_pt  // output: spine SPAposition
		);

// Gets whether to accept a zero face state.
/**
 * @nodoc
 */
	virtual logical accept_zero_face_attr( )
		{ return( acpt_zero_face_attr ); }

// Sets whether to accept a zero face state.
/**
 * @nodoc
 */
	void set_accept_zero_face_attr( logical in )
		{ acpt_zero_face_attr = in; }

		// enquire
/**
 * @nodoc
 */
	virtual blend_debug_info* enquire(FILE*fp);

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_FFBLEND, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	/**
	 * @nodoc
	 */
	FULLSIZE_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

	/**
	* @nodoc
	*/
	//set reorder blend
	void set_reorder_blend(logical reorder_bl = TRUE) 
		{ reorder_blend = reorder_bl; }

	/**
	* @nodoc
	*/
	//check for a reorder blend
	logical is_reorder_blend()const
		{ return reorder_blend; }

};

// Very handy little function to find the (first) ATTRIB_FFBLEND on any
// given entity.

/**
 * @nodoc
 */
DECL_BLND ATTRIB_FFBLEND * find_ffblend_attrib( ENTITY *owner );

// Define a class to record a blend supported on one side only by a face, or
// more precisely, at one or more coedges which have the support face as their
// partner face.  If there are several such coedges, they must lie on the same
// curve, the curve at which the blend surface springs from the support face.
// For now this attribute is used only when recomputing blend geometry in
// healing when a face-face blend has been trimmed back on one side, usually
// by another blend.

/**
 * @nodoc
 */
extern DECL_BLND int ATTRIB_FBLEND_TYPE;
#define ATTRIB_FBLEND_LEVEL (ATTRIB_BLEND_LEVEL + 1)

// tbrv
/**
 * @nodoc
 */
class DECL_BLND ATTRIB_FBLEND: public ATTRIB_BLEND {
private:
	// Blend convexity given explicitly.

	bl_ed_convexity cvxty_data;

	// The support face from which blend springs.  Coedges of the owning face
	// whose partners are coedges of the support face, must lie on the same
	// curve.  This is the support curve which with the surface of the support
	// face defines the blend surface.

	FACE *support_face_ptr;

	// Logical set true if the support face is on the left side of the blend.

	logical support_face_on_left_data;

	// Define a plane that cuts the blend.

	plane *def_plane_ptr;

	// Record condition at the start and end of a blend.

	bl_end_condition bl_end_con_at_start_data;
	bl_end_condition bl_end_con_at_end_data;

protected:

	// Record a copy of blend geometry to save recomputing it
	// (replaces earlier separate cache).

	ffblend_geom *ffblend_geom_ptr;

public:

	// Constructor.

	ATTRIB_FBLEND(
				ENTITY * = NULL,			// owner, for now a face
				FACE * = NULL,				// support face
				logical = TRUE,				// true if support is on the left
				bl_ed_convexity = bl_ed_undefined_cvxty,
				plane const & = *(plane const *)NULL_REF,	// def-plane
				blend_status = bl_stat_unset,
				bl_end_condition = bl_end_unset,	// bl_end_condit at start
				bl_end_condition = bl_end_unset		// bl_end_condit at end
			);

	// Make a copy of a blend (setting implicit blend data only and
	// without attaching the copy to an entity).

	ATTRIB_FBLEND( ATTRIB_FBLEND const *);

	virtual ATTRIB_BLEND* copy() const;

    virtual logical pattern_compatible() const;

	// Simple read functions for the data members.

	bl_ed_convexity cvxty();

	bl_ed_convexity cvxty() const;

	logical convex() const;

	FACE *support_face() const;

	FACE *support_face();
	logical support_face_on_left() const { return support_face_on_left_data; }

	plane def_plane() const;

	plane def_plane();

	bl_end_condition bl_end_con_at_start() const
		{ return bl_end_con_at_start_data; }

	bl_end_condition bl_end_con_at_end() const
		{ return bl_end_con_at_end_data; }

	// This is the official function to discover what entities the
	// spring curves of a blend face run along, i.e. its "supports".
	// For a fblend, an array of length 2 will be returned together with
	// the array (on the heap) containing a pointer to a face and a null
	// (if the support is on the left, the first entry will be
	// non-null and the second null).  An array is used rather than an
	// ENTITY_LIST so that entries can be null (as can happen during
	// deletions when explicit blend attributes and support attributes
	// exist, and can be deleted in any order).

	virtual int supports( ENTITY **& ) const;

	// Member setting functions. These ensure that the attribute
	// is safely backed up before making the change.

	void set_cvxty( bl_ed_convexity );

	void set_support_face( FACE *, logical );	// logical true for left

	void set_def_plane( plane const & );

	void set_bl_end_con_at_start( bl_end_condition );

	void set_bl_end_con_at_end( bl_end_condition );

	// Set support entity of blend to be the given entity.
	// i is 0 for left, 1 for right.

	virtual void set_support( int, ENTITY *);

	// Normalise the support attribute structure, i.e. discard any
	// support attribute on an entity not adjacent to the entity owning
	// the related blend attribute and set no_out_of_body_ptrs_stat true.

	virtual void normalise_supports();

	// Reverse left-to-right properties of an asymmetric blend.

	virtual void reverse();		// pure

	// Functions called to aid attribute migration during modelling
	// operations.

	virtual void split_owner( ENTITY * );
									// the owner of this attribute is
									// about to be split in two - the
									// argument is the new piece.

	virtual void merge_owner( ENTITY *, logical );
									// the owner of this attribute is
									// about to be merged with the
									// given entity. The logical
									// argument is TRUE if the owner is
									// to be deleted in the merge.

	// A further function to allow geometric attributes to transform
	// themselves when their owner is transformed.

	virtual void trans_owner(
				SPAtransf const &
			);

	// Test two blends for equality.

	virtual logical operator==( ATTRIB_BLEND const& ) const;

	// Return a measure of continuity at blend spring curves
	// i.e. unset_continuity, position_continuous, slope_continuous,
	// or curvature_continuous.

	virtual bl_continuity continuity( int sup_index ) const;

	// Return a measure of the size of the blend. Specifically, this means
	// how far the blend spine may be from the underlying surfaces. So for
	// a constant radius rolling ball, it's just the radius. Returning less
	// than this maximum offset value risks unreliability in geometrically
	// extremish (perhaps sharp or shallow) situations.

	virtual double blend_size( logical at_start  = TRUE) const;

	// Find details of blend geometry i.e. spring curves, auxiliary
	// surfaces and blend surface.  NB We continue to use ffblend_geom
	// since we still make a blend surface but no spring curves.

	virtual ffblend_geom *find_ffblend_geometry(
		logical = FALSE,			// open at start
		logical = FALSE,			// open at end
		segend * = NULL,			// start segend if any
		segend * = NULL,			// end segend if any
		SPAbox const & = *(SPAbox const *)NULL_REF
	  );

	// Find the spine curve or point.

	virtual ffblend_geom *find_ffblend_spine(
		logical,			// open at start
		logical,			// open at end
		segend *,			// start segend if any
		segend *,			// end segend if any
		SPAbox const & = *(SPAbox const *)NULL_REF
	  );

	// Given blend boundary details in a ffblend_geom, find the
	// blend surface and return it and the given boundary details
	// in a ffblend_geom.

	virtual ffblend_geom *find_ffblend_surface(ffblend_geom *);

	// Find SPAbox enclosing blend.

	virtual SPAbox find_ffblend_region(
		logical,			// open at start
		logical,			// open at end
		segend *,			// start segend if any
		segend *			// end segend if any
	  );

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_FBLEND, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	/**
	 * @nodoc
	 */
	FULLSIZE_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
};

// Attribute used to mark a supporting entity referred to by a blend attribute.
// Needed so that the blend attribute can be updated or deleted when the
// support undergoes a change.  Holds a pointer to the blend attribute
// concerned.  Also contains a pointer to another support attribute so
// that multiple supports can be recorded if need be.

/**
 * @nodoc
 */
extern DECL_BLND int ATTRIB_BLEND_SUPPORT_TYPE;
#define ATTRIB_BLEND_SUPPORT_LEVEL (ATTRIB_BLEND_LEVEL + 1)

// tbrv
/**
 * @nodoc
 */
class DECL_BLND ATTRIB_BLEND_SUPPORT: public ATTRIB_BLEND {
private:
	int	support_index_data;					// record which support 0, 1, ..
	ATTRIB_BLEND *bl_att_ptr;				// related blend attribute
	ATTRIB_BLEND_SUPPORT *prev_supp_ptr;	// links multiple support entities
	ATTRIB_BLEND_SUPPORT *next_supp_ptr;	// links multiple support entities

public:

	ATTRIB_BLEND_SUPPORT(					// constructor
				ENTITY * = NULL,			// owner
				ATTRIB_BLEND * = NULL,		// related blend attribute
				int = 0						// index
			);

	// Make a copy of a blend support (setting implicit blend data only and
	// without attaching the copy to an entity).

	ATTRIB_BLEND_SUPPORT( ATTRIB_BLEND_SUPPORT const *);

	virtual ATTRIB_BLEND* copy() const;

    virtual logical pattern_compatible() const;

	// Simple read functions for the data members.

	int support_index() const { return support_index_data; }

	ATTRIB_BLEND *blend_att() const { return bl_att_ptr; }

	ATTRIB_BLEND_SUPPORT *next_supp() const { return next_supp_ptr; }

	ATTRIB_BLEND_SUPPORT *previous_supp() const { return prev_supp_ptr; }

	// Member setting function. Ensures that the attribute
	// is safely backed up before making the change.

	void set_support_index( int );

	void set_blend_att( ATTRIB_BLEND * );

	void set_previous_supp( ATTRIB_BLEND_SUPPORT * );

	void set_next_supp( ATTRIB_BLEND_SUPPORT * );

	// Functions called to aid attribute migration during modelling
	// operations.

	virtual void split_owner( ENTITY * );
									// the owner of this attribute is
									// about to be split in two - the
									// argument is the new piece.

	virtual void merge_owner( ENTITY *, logical );
									// the owner of this attribute is
									// about to be merged with the
									// given entity. The logical
									// argument is TRUE if the owner is
									// to be deleted in the merge.


	// Return a descriptive string label for the object saying in a more
	// human readable format what kind the object is. Useful for various
	// things, for example, debugging, display etc. May return NULL when
	// there is no particular string to describe the object.

	virtual char const *label() const;

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_BLEND_SUPPORT, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	/**
	 * @nodoc
	 */
	FULLSIZE_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
};

// Attributes used internally by the blending algorithm to record
// intermediate results. They are all derived from a common base
// attribute, to simplify clearup.

/**
 * @nodoc
 */
extern DECL_BLND int ATTRIB_BLINFO_TYPE;
#define ATTRIB_BLINFO_LEVEL (ATTRIB_SYS_LEVEL + 1)

/**
 * Defines attributes used internally by the blending algorithm to record intermediate results.
 * <br>
 * <b>Role:</b> This class defines attributes used internally by the blending
 * algorithm to record intermediate results. It is derived from a common base
 * attribute to simplify clean up.
 */
class DECL_BLND ATTRIB_BLINFO: public ATTRIB_SYS {
public:
	// Constructor just passes the owner on to ATTRIB_SYS.
/**
* C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
* <br><br>
* <b>Role:</b> Applications should call this constructor only with the overloaded
* new operator inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_BLINFO(...)</tt>),
* because this reserves the memory on the heap, a requirement to support roll back
* and history management.
* <br><br>
* @param entity1
* entity.
*/
	ATTRIB_BLINFO( ENTITY *entity1 = NULL );

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_BLINFO, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:


/**
 * @nodoc
 */
    virtual logical pattern_compatible() const;
                                    // returns TRUE
	// Return a descriptive string label for the object saying in a more
	// human readable format what kind the object is. Useful for various
	// things, for example, debugging, display etc. May return NULL when
	// there is no particular string to describe the object.


/**
 * Labels the blend information.
 */
	virtual char const *label() const;
};

// Now the specific sheet body attributes.



/**
 * @nodoc
 */
extern DECL_BLND int ATT_CAP_FACE_TYPE;
#define ATT_CAP_FACE_LEVEL (ATTRIB_BLINFO_LEVEL + 1)
/**
 * Attribute to be attached to a cap face, to record the blank
 * body which was used to generate the non-univex capping solution.
 */
class DECL_BLND ATT_CAP_FACE : public ATTRIB_BLINFO
{

public:

	ATT_CAP_FACE(
			FACE *cap_f = NULL,			// The cap face of the blend sheet.
			FACE *blank_f = NULL,		// The blank face extended to create
										// the cap face.
			ENTITY *graph_ent = NULL	// The blend graph entity for which the
										// tool was generated.
			);

	// The split method just copies the attribute "as is" to the new
	// owner.

	virtual void split_owner( ENTITY *new_ent );

	logical pattern_compatible() const;

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATT_CAP_FACE, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", 
// we do so explicitly here:
public:

	// The face of the blank body which was extended to generate the cap face.

	FACE *blank_face() const { return _blank_face; }
	void set_blank_face( FACE *f );

	// The entity being blended. Will point to the vertex for a mixed convexity
	// vertex blend, the blended edge for a blend sequence.

	ENTITY *blended_entity() const { return _graph_entity; }
	void set_blended_entity( ENTITY *ent );

	// If the graph is alive, i.e if within blend stage one, returns the blend
	// sequence if the cap face was generated during capping of as sequence.

	blend_seq *seq() const;

	// Similarly, the blend node if the cap face was generated during capping of
	// a mixed convexity vertex blend.

	blend_node *node() const;

private:

	ENTITY *_graph_entity;

	FACE *_blank_face;
};

// Very handy little function to find the (first) ATTRIB_EXPBLEND on any
// given entity.

/**
 * @nodoc
 */
DECL_BLND ATT_CAP_FACE *find_cap_face_attrib( ENTITY *owner );

// Define an attribute to be attached to a sheet face, to record the
// implicit blend which gave rise to it.

/**
 * @nodoc
 */
extern DECL_BLND int ATTRIB_EXPBLEND_TYPE;
#define ATTRIB_EXPBLEND_LEVEL (ATTRIB_BLINFO_LEVEL + 1)

// tbrv
/**
 * @nodoc
 */
class DECL_BLND ATTRIB_EXPBLEND : public ATTRIB_BLINFO {
private:

	ATTRIB_BLEND *imp_blend_ptr;
							// edge or vertex blend giving rise to
							// this face.
	bl_ed_convexity	cvxty_data;
							// convexity of the blended entity, and
							// hence of the explicit sheet face. A
							// convex blend removes material from the
							// blended body, while a concave one adds
							// material. May be "unknown" for a vertex
							// blend between edges of different
							// convexities.
	logical end_cap_data;	// TRUE if this is not in fact a blend
							// face, but a mixed-convexity end cap.
							// If TRUE, the implicit blend pointer
							// if NULL, but the end_vertex pointer
							// is set.
	VERTEX *end_vert_ptr;	// Vertex at the end of a blend sequence
							// which gives rise to this mixed-convexity
							// end cap.
	FACE *end_face_ptr;		// Face at the end of a blend sequence
							// whose surface extended forms a
							// capping face.

	// So that a blend face can know what it's ffblend_geom was. The
	// actual blend attribute, unfortunately, is likely to forget.

	ffblend_geom *ffbl_geom_ptr;

	// So that blend faces can remember what the previous/next faces are
	// for this blend attribute, ordered along the spine or defining
	// curve for the blend. Usually there will be no more than a single
	// face in each sense, unless later on, when maybe arbitrary splits
	// have happened.

	int num_prev_faces_data, num_next_faces_data;
	FACE **prev_faces_ptr, **next_faces_ptr;

	logical end_cap() const { return end_cap_data; }

	VERTEX *end_vert() { return end_vert_ptr; }

	FACE *end_face() { return end_face_ptr; }	

	void set_end_vert( VERTEX * );

	void set_end_face( FACE * );

	ATTRIB_EXPBLEND(
				FACE *sheet_face,					
				VERTEX *mixed_cap_vtx,	// end vertex for mixed end cap
				FACE *blank_face,		// end face whose extension caps blend
				bl_ed_convexity c = bl_ed_undefined_cvxty
				);

public:

    virtual logical pattern_compatible() const;
                                    // returns TRUE
	// Simple read functions for the data members.

	ATTRIB_BLEND *imp_blend() const { return imp_blend_ptr; }

	bl_ed_convexity cvxty() const { return cvxty_data; }
	
	// Member setting functions. These ensure that the attribute
	// is safely backed up before making the change.

	void set_imp_blend( ATTRIB_BLEND * );

	void set_cvxty( bl_ed_convexity );

	// Query/set the ffblend_geom. Our query method doesn't up the use
	// count, thank goodness, but our set method does - because the
	// attribute now has a recorded use.

	ffblend_geom *ffbl_geom() const { return ffbl_geom_ptr; }

	void set_ffbl_geom( ffblend_geom * );

	// For accessing and manipulating the prev/next face lists. First,
	// simple access. The size of the array is the return argument.

	int prev_faces( FACE **& = *(FACE ***)NULL_REF ) const;

	int next_faces( FACE **& = *(FACE ***)NULL_REF ) const;

	// Handy variants of the above. Returns the *first* face only.

	FACE *prev_face() const
	{ return prev_faces_ptr ? prev_faces_ptr[ 0 ] : NULL; }
	FACE *next_face() const
	{ return next_faces_ptr ? next_faces_ptr[ 0 ] : NULL; }

	// Return TRUE if the given face is in the prev/next faces lists.

	logical is_prev_face( FACE * ) const;

	logical is_next_face( FACE * ) const;

	// Set the lists explicitly. We leave it to the caller to take care
	// of the information in those adjacent faces. The given list is
	// *copied*. (Pass with no args to clear the lists.)

	void set_prev_faces( int = 0, FACE ** = NULL );

	void set_next_faces( int = 0, FACE ** = NULL );

	// Handly simple versions of the above for the usual single face
	// case.

	void set_prev_face( FACE * );

	void set_next_face( FACE * );

	// Add the given face to the prev/next faces list, or remove it.
	// None of these actually manipulate those adjacent faces to make
	// them match - that is left for the caller.

	void add_prev_face( FACE * );

	void add_next_face( FACE * );

	void remove_prev_face( FACE * );

	void remove_next_face( FACE * );

	// Main constructors.

	ATTRIB_EXPBLEND(
				FACE *sheet_face = NULL,
				ATTRIB_BLEND *implicit_att = NULL,	
				bl_ed_convexity c = bl_ed_undefined_cvxty
				);		

	// The split method just copies the attribute "as is" to the new
	// owner.

	virtual void split_owner( ENTITY *new_ent );

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_EXPBLEND, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

// Very handy little function to find the (first) ATTRIB_EXPBLEND on any
// given entity.

/**
 * @nodoc
 */
DECL_BLND ATTRIB_EXPBLEND *find_expblend_attrib( ENTITY *owner );

// Attach expblend attrib to topology created for a implicit blend attribute.

/**
 * @nodoc
 */
ATTRIB_EXPBLEND *
attach_expblend_attrib( 
				FACE *blend_face,
				ATTRIB_BLEND *imp_att,
				bl_ed_convexity c
				);

// Attribute that will mark edges lying on spring curves so they may
// be specially handled later.  The attribute refers to the entity
// (face, edge or vertex) of the blended body from which the blend springs.

/**
 * @nodoc
 */
extern DECL_BLND int ATTRIB_SPRING_TYPE;
#define ATTRIB_SPRING_LEVEL (ATTRIB_BLINFO_LEVEL + 1)

/**
 * @nodoc
 */
class DECL_BLND ATTRIB_SPRING: public ATTRIB_BLINFO {
private:

	COEDGE *bl_coedge_ptr;		// coedge of the blended edge, lying on the
                                // same support

	ENTITY *lateral_ent_ptr;	// entity on which spring curve lies

	surface *aux_surface_ptr;	// a surface roughly perpendicular to the
								// blend body surface at the spring curve.
								// Only needed if this is a blend with slope
								// or higher continuity and therefore
								// the lateral face or one or both faces of
								// the lateral edge is tangential to the
								// blend sheet.  Has sense such that the
								// normal points away from the blend.

	pcurve *lateral_pcur_ptr;	// pcurve for spring curve with respect to
								// the surface of the lateral face (or to
								// the face of the lateral edge, that lies
								// beyond the blend i.e. on the "away" side)

	pcurve *lateral_other_pcur_ptr;	// pcurve for spring curve with respect to
								// to the other face of the lateral edge
								// i.e. not the "away" side.

    bl_continuity spr_cont;

public:

	// Constructors.

	ATTRIB_SPRING(
            bl_continuity c = slope_continuous,
			EDGE * = NULL,			// spring edge
			ENTITY * = NULL,		// lateral face or edge
			surface const* = NULL,	// auxiliary surface
			pcurve const* = NULL,	// lateral surface pcurve
			pcurve const* = NULL,	// lateral surface pcurve
            COEDGE * = NULL
			);

	ATTRIB_SPRING(
			EDGE *,			// spring edge
			logical,		// true for left
			ffsegment *
			);

    virtual logical pattern_compatible() const;
                                    // returns TRUE

	// Simple read function for the data members.

    COEDGE  *bl_coedge() const { return bl_coedge_ptr; }

	ENTITY *lateral_ent() const { return lateral_ent_ptr; }

	FACE *lateral_face() const;

	surface *aux_surface() const { return aux_surface_ptr; }

	pcurve *lateral_pcur() const { return lateral_pcur_ptr; }

	pcurve *lateral_other_pcur() const { return lateral_other_pcur_ptr; }

    bl_continuity continuity() const { return spr_cont; }

	// Member setting functions. These ensure that the attribute
	// is safely backed up before making the change.

	void set_lateral_ent( ENTITY* );

	void set_lateral_face( FACE * );

	void set_aux_surface( surface * );

	void set_lateral_pcur( pcurve * );

	void set_lateral_other_pcur( pcurve * );

    void set_continuity( bl_continuity c ) { spr_cont = c; }

    void set_bl_coedge( COEDGE *c );

	// Owner split. Just copy the attribute and its attendant bits and
	// pieces to the new entity.

	virtual void split_owner( ENTITY *new_ent );

	// Owner merge. 

	virtual void merge_owner( ENTITY *other_ent, logical delete_owner );

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_SPRING, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

// Find the (first) ATTRIB_SPRING attribute on any given entity.

/**
 * @nodoc
 */
ATTRIB_SPRING *find_spring_attrib( ENTITY *owner, EDGE *blended_ent );

// Attribute to record blank body edges that are in contact with the
// sheet boundary, termed "adjacent edges".  The edge and the
// SPAparameter value on the edge at the point of contact are recorded.
// The attribute is attached to the vertex of the sheet in contact
// with the adjacent edge.

/**
 * @nodoc
 */
extern DECL_BLND int ATTRIB_ADJEDGE_TYPE;
#define ATTRIB_ADJEDGE_LEVEL (ATTRIB_BLINFO_LEVEL + 1)

/**
 * @nodoc
 */
class DECL_BLND ATTRIB_ADJEDGE: public ATTRIB_BLINFO {
private:

	COEDGE *bl_coedge_ptr;	// coedge of blend edge lying on the face of 
                            // "coedge_ptr"

	EDGE *edge_ptr;
	SPAparameter edge_param_data;
	COEDGE *coedge_ptr;

public:

    virtual logical pattern_compatible() const;
                                    // returns TRUE
	// Simple read function for the data members.

	COEDGE *bl_coedge() const { return bl_coedge_ptr; }

	EDGE *edge() const { return edge_ptr; }

	SPAparameter edge_param() const { return edge_param_data; }

	// The coedge is recorded in case blend stage 2 wants to know it to
	// help it do the right thing in tolerant modeling situations. It is
	// the coedge that was intercepted to give rise to this adjedge
	// attribute, and is obviously one of the edge's coedges.

	COEDGE *coedge() const { return coedge_ptr; }

	// Member setting functions. These ensure that the attribute
	// is safely backed up before making the change.

	void set_edge( EDGE * );

	void set_edge_param( SPAparameter );

	// Owner split. Just copy the attribute and its attendant bits and
	// pieces to the new entity.

	virtual void split_owner( ENTITY *new_ent );

	// Owner merge. 

	virtual void merge_owner( ENTITY *other_ent, logical delete_owner );

	// Main constructor.

	ATTRIB_ADJEDGE(                
				VERTEX * = NULL,	// sheet vertex
				EDGE * = NULL,		// adjacent edge
				double = 0.0,		// edge SPAparameter at sheet vertex
		        COEDGE * = NULL,	// coedge giving rise to us
                COEDGE * = NULL     // coedge of blend edge 
			);

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_ADJEDGE, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

// Find the (first) ATTRIB_ADJEDGE attribute on any given entity.

/**
 * @nodoc
 */
ATTRIB_ADJEDGE *find_adjedge_attrib( ENTITY *owner, COEDGE *blended_ent );

// Attribute to record blank body faces that are in contact with the
// sheet boundary, termed "adjacent faces".  The face and the
// SPAparameter value on the face at the point of contact are recorded.
// The attribute is attached to the vertex of the sheet in contact
// with the adjacent face.

/**
 * @nodoc
 */
extern DECL_BLND int ATTRIB_ADJFACE_TYPE;
#define ATTRIB_ADJFACE_LEVEL (ATTRIB_BLINFO_LEVEL + 1)

/**
 * @nodoc
 */
class DECL_BLND ATTRIB_ADJFACE: public ATTRIB_BLINFO {
private:

	COEDGE *bl_coedge_ptr;	// coedge of blend edge lying on "adj_face"

	FACE *face_ptr;
	SPApar_pos	face_param_data;

public:

    virtual logical pattern_compatible() const;
                                    // returns TRUE
	// Simple read functions for the data members.

	COEDGE *bl_coedge() const { return bl_coedge_ptr; }

	FACE *face() const { return face_ptr; }
	SPApar_pos face_param() const { return face_param_data; }

	// Member setting functions. These ensure that the attribute
	// is safely backed up before making the change.

	void set_face( FACE * );

	void set_face_param( SPApar_pos );

	// Owner split. Just copy the attribute and its attendant bits and
	// pieces to the new entity.

	virtual void split_owner( ENTITY *new_ent );

	// Owner merge. 

	virtual void merge_owner( ENTITY *other_ent, logical delete_owner );

	// Main constructors.

	ATTRIB_ADJFACE(
				ENTITY *,					// sheet vertex
				FACE *,						// adjacent face                
				SPApar_pos	const & =
					* ( SPApar_pos const * ) NULL_REF,	
                                            // face SPAparameter at sheet vertex
                COEDGE * = NULL             // coedge of blend edge lying on "adj_face"
			);

	ATTRIB_ADJFACE();

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_ADJFACE, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

// Find the (first) ATTRIB_ADJEDGE attribute on any given entity.

/**
 * @nodoc
 */
ATTRIB_ADJFACE *find_adjface_attrib( ENTITY *owner, COEDGE *blended_ent );

// Attribute to be attached to cross edges of the blend sheet,
// recording the blended edge giving rise to the face on
// one side of the cross edge. This is used internally during blend1,
// but should be removed from the sheet at the end of this stage.

/**
 * @nodoc
 */
extern DECL_BLND int ATTRIB_XEDGE_TYPE;
#define ATTRIB_XEDGE_LEVEL (ATTRIB_BLINFO_LEVEL + 1)

/**
 * @nodoc
 */
class DECL_BLND ATTRIB_XEDGE: public ATTRIB_BLINFO {
private:

	COEDGE *coedge_ptr;		// coedge of blend edge pointing away from
							// this cross edge
	logical rem_xcur_data;	// true if xcur will be removed later (in
							// rolling-ball vertex blending)

public:

    virtual logical pattern_compatible() const;
                                    // returns TRUE
	// Simple read function for the data member.

	COEDGE *coedge() const { return coedge_ptr; }

	logical rem_xcur() const { return rem_xcur_data; }

	// Member setting function. This ensures that the attribute
	// is safely backed up before making the change.

	void set_coedge( COEDGE * );

	void set_rem_xcur( logical );

	// It is just about possible that our owner could be split. If so,
	// just do a straight copy of the attribute onto the new entity.

	virtual void split_owner( ENTITY *new_ent );

	// Main constructor.

	ATTRIB_XEDGE(
				EDGE * = NULL,		// cross edge
				COEDGE * = NULL		// blend coedge
			);

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_XEDGE, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

// Find an ATTRIB_XEDGE attribute on the given cross edge given the
// coedge of blend edge pointing away from this edge.

ATTRIB_XEDGE *
find_xedge_attrib( EDGE const *edge, COEDGE const *bl_coedge = NULL );



// Attribute to be attached to cross edges of the blend sheet, that
// meet the blend body tangentially.  These occur when the sheet face has
// been made for a blended cuspate vertex.  We record the unblended edge
// on whose curve the sheet edge lies.  This is used internally during blend1,
// but should be removed from the sheet at the end of this stage.

/**
 * @nodoc
 */
extern DECL_BLND int ATTRIB_TAN_XEDGE_TYPE;
#define ATTRIB_TAN_XEDGE_LEVEL (ATTRIB_BLINFO_LEVEL + 1)

/**
 * @nodoc
 */
class DECL_BLND ATTRIB_TAN_XEDGE: public ATTRIB_BLINFO {
private:

	COEDGE *coedge_ptr;			// coedge of unblended edge, whose start is
								// the blended vertex

	FACE *lateral_face_ptr;		// face on which cross curve lies

	surface *aux_surface_ptr;	// surface roughly perpendicular
								// to the face at the cross curve,
								// if this is a round (and therefore
								// the face and the blend sheet are
								// tangential

	pcurve *lateral_pcur_ptr;	// pcurve for cross curve with respect
								// to the lateral face surface

public:

    virtual logical pattern_compatible() const;
                                    // returns TRUE
	// Simple read functions for the data members.

	COEDGE *coedge() const { return coedge_ptr; }

	FACE *lateral_face() const { return lateral_face_ptr; }

	surface *aux_surface() const { return aux_surface_ptr; }

	pcurve *lateral_pcur() const { return lateral_pcur_ptr; }

	// Member setting functions. These ensure that the attribute
	// is safely backed up before making the change.

	void set_coedge( COEDGE * );

	void set_lateral_face( FACE * );

	void set_aux_surface( surface * );

	void set_lateral_pcur( pcurve * );

	// It is just about possible that our owner could be split. If so,
	// just do a straight copy of the attribute onto the new entity.

	virtual void split_owner( ENTITY *new_ent );

	// Main constructor.

	ATTRIB_TAN_XEDGE(
				EDGE * = NULL,			// cross edge
				COEDGE * = NULL,		// blend coedge
				FACE * = NULL,			// lateral face
				surface const* = NULL,	// auxiliary surface
				pcurve const* = NULL	// lateral surface pcurve
			);

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_TAN_XEDGE, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};


// Define an attribute to be attached to point vertices of the blend
// sheet, recording the corresponding body vertex. This roughly
// corresponds to an XEDGE attribute, for cases where the cross edge
// disappears. This is used internally during blend1, but should be
// removed from the sheet at the end of this stage.

/**
 * @nodoc
 */
extern DECL_BLND int ATTRIB_XVERT_TYPE;
#define ATTRIB_XVERT_LEVEL (ATTRIB_BLINFO_LEVEL + 1)

/**
 * @nodoc
 */
class DECL_BLND ATTRIB_XVERT: public ATTRIB_BLINFO {
private:

	VERTEX *vertex_ptr;		// vertex on blended body corresponding
							// to this point vertex.

public:

    virtual logical pattern_compatible() const;
                                    // returns TRUE
	// Simple read function for the data member.

	VERTEX *vertex() const { return vertex_ptr; }

	// Member setting function. This ensures that the attribute
	// is safely backed up before making the change.

	void set_vertex( VERTEX * );

	// No split_owner needed. These live on vertices which we should not
	// be splitting.

	// Main constructor.

	ATTRIB_XVERT(
				VERTEX * = NULL,	// sheet vertex
				VERTEX * = NULL		// corresponding body vertex
			);

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_XVERT, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};


// Define an attribute to be attached to point vertices of the
// vertex blend sheet, pointing to the associated edge which
// was blended with a zero radius blend.

/**
 * @nodoc
 */
extern DECL_BLND int ATTRIB_XVERTED_TYPE;
#define ATTRIB_XVERTED_LEVEL (ATTRIB_BLINFO_LEVEL + 1)

/**
 * @nodoc
 */
class DECL_BLND ATTRIB_XVERTED: public ATTRIB_BLINFO {
private:

	EDGE *edge_ptr;			// edge on blended body corresponding
							// to this point vertex.

	double edge_param_data;		// location on the edge of the vertex
	SPApar_pos face_param_data;	// location of the vertex on left face

public:

    virtual logical pattern_compatible() const;
                                    // returns TRUE
	// Simple read function for the data members.

	EDGE *edge() const { return edge_ptr; }

	double edge_param() { return edge_param_data; }

	SPApar_pos const &face_param() { return face_param_data; }

	// No split_owner needed. These live on vertices which we should not
	// be splitting.

	// Main constructor.

	ATTRIB_XVERTED(
				VERTEX * = NULL,	// sheet vertex
				EDGE * = NULL,		// corresponding body vertex
			    double = 0.0,		// SPAparameter along edge
				SPApar_pos const & = *(const class SPApar_pos*)NULL_REF   // pos on sf
			);

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_XVERTED, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};


// Attribute to be attached to edges of the sheet, where they are
// coincident with edges of the blended body.

/**
 * @nodoc
 */
extern DECL_BLND int ATTRIB_ECOIN_TYPE;
#define ATTRIB_ECOIN_LEVEL (ATTRIB_BLINFO_LEVEL + 1)

/**
 * @nodoc
 */
class DECL_BLND ATTRIB_ECOIN: public ATTRIB_BLINFO {
private:

	EDGE *edge_ptr;			// edge on blended body corresponding
							// to this edge.

public:

    virtual logical pattern_compatible() const;
                                    // returns TRUE
	// Simple read function for the data member.

	EDGE *edge() const { return edge_ptr; }

	// Member setting function. This ensures that the attribute
	// is safely backed up before making the change.

	void set_edge( EDGE * );

	// It is possible that our owner could be split. If so, just do a
	// straight copy of the attribute onto the new entity.

	virtual void split_owner( ENTITY *new_ent );

	// Main constructor.

	ATTRIB_ECOIN(
				EDGE * = NULL,		// sheet edge
				EDGE * = NULL		// corresponding body edge
			);

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_ECOIN, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};


// Short-lived attribute to be attached to cross edges during healing to
// record details of a circular cross curve.

/**
 * @nodoc
 */
extern DECL_BLND int ATTRIB_XPOS_TYPE;
#define ATTRIB_XPOS_LEVEL (ATTRIB_BLINFO_LEVEL + 1)

// tbrv
/**
 * @nodoc
 */
class DECL_BLND ATTRIB_XPOS: public ATTRIB_BLINFO {
private:

	SPAposition x_circle_centre_data;
	SPAunit_vector x_circle_normal_data;
	SPAposition left_pos_data;
	SPAposition right_pos_data;

public:

    virtual logical pattern_compatible() const;
                                    // returns TRUE
	// Simple read function for the data member.

	SPAposition centre() const { return x_circle_centre_data; }

	SPAunit_vector normal() const { return x_circle_normal_data; }

	SPAposition left() const { return left_pos_data; }

	SPAposition right() const { return right_pos_data; }

	// Member setting function. This ensures that the attribute
	// is safely backed up before making the change.

	void set_centre( SPAposition const & );

	void set_normal( SPAunit_vector const & );

	void set_left( SPAposition const & );

	void set_right( SPAposition const & );

	// Main constructor.

	ATTRIB_XPOS(
				EDGE * = NULL,		// owner
				SPAposition const & = *(const class SPAposition *)NULL_REF,
				SPAunit_vector const & = *(const class SPAunit_vector *)NULL_REF,
				SPAposition const & = *(const class SPAposition *)NULL_REF,
				SPAposition const & = *(const class SPAposition *)NULL_REF
			);

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_XPOS, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

/**
 * @nodoc
 */
extern int ATTRIB_HOLDLINE_EDGE_TYPE;
#define ATTRIB_HOLDLINE_EDGE_LEVEL ( ATTRIB_SYS_LEVEL + 1 )

/**
 * @nodoc
 */
class DECL_BLND ATTRIB_HOLDLINE_EDGE : public ATTRIB_SYS {

public:
	ATTRIB_HOLDLINE_EDGE() {}
	ATTRIB_HOLDLINE_EDGE( ENTITY* owner );
	ATTRIB_HOLDLINE_EDGE( ATTRIB_HOLDLINE_EDGE const & );

	virtual ATTRIB_HOLDLINE_EDGE *copy() const;
protected:
	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_HOLDLINE_EDGE, BLND )
};

// Find the (first) ATTRIB_HOLDLINE_EDGE attribute on the given edge.

/**
 * @nodoc
 */
DECL_BLND ATTRIB_HOLDLINE_EDGE *
find_holdline_edge_attrib( EDGE *hl_edge );


 /** @} */
#endif
