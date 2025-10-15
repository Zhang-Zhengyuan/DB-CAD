/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (ATTRIB_HH_ENT_STITCH_EDGE_CLASS)
#define ATTRIB_HH_ENT_STITCH_EDGE_CLASS

/**
 * @file entstch_stitch.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup STITCHAPI
 *
 * @{
 */
#include "edge.hxx"
#include "position.hxx"
#include "mmgr.hxx"

#include "at_sys.hxx"
#include "att_hh.hxx"
#include "dcl_stitch.h"
#include "entstchbase.hxx"
#include "at_enty_stitch.hxx"
#include "geom_utl2.hxx"


// For internal use only
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_HH_ENT_STITCH_EDGE, STITCH)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
// For internal use only
/**
 * @nodoc
 */
typedef void KEY_TYPE;
// For internal use only
/**
 * @nodoc
 */
typedef void DATA_TYPE;

// For internal use only
/**
 * @nodoc
 */
enum STCH_EDGE_SPLIT_INFO
{
	STCH_NOT_SPLIT = 0,
	STCH_SPLIT_AT_START = 1,
	STCH_SPLIT_AT_END = 2
};

// For internal use only
/**
 * @nodoc
 */
class HASH_ENTRY_TYPE : public ACIS_OBJECT {
public:

	DATA_TYPE* m_data;
	KEY_TYPE* m_key;
	logical m_tombstone;
	void reset() {
		*this = HASH_ENTRY_TYPE(NULL, NULL, FALSE);
	}

	HASH_ENTRY_TYPE() {reset();}
	HASH_ENTRY_TYPE(
		DATA_TYPE* data,
		KEY_TYPE* key,
		logical tombstone)
	{
		m_data = data;
		m_key = key;
		m_tombstone = tombstone;
	}
};

// For internal use only
/**
 * @nodoc
 */
class DECL_STITCH hash_table
{
	size_t m_array_size;
	size_t m_count;
	HASH_ENTRY_TYPE* m_array;

	void init();
	logical correct_size();
	size_t hash_function(KEY_TYPE* key);
	size_t fit_in_size(size_t index);
	size_t jump_index(size_t index);
	size_t get_free_index(KEY_TYPE* key);

public:
// For internal use only
	/**
	 * @nodoc
	 */
	MMGR_FREELIST_THIS
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

	hash_table();
	~hash_table();
	hash_table(const hash_table&);
	const hash_table& operator = ( const hash_table & );

	void clear();
	DATA_TYPE* getnext(size_t& curr_index);
	bool add(DATA_TYPE* element, KEY_TYPE* key);
	DATA_TYPE* get(KEY_TYPE* key);
	logical remove(KEY_TYPE* key);
};

// For internal use only
/**
 * @nodoc
 */
class DECL_STITCH hh_gap_size_entry
{
public:
	/**
	 * @nodoc
	 */
	MMGR_FREELIST_THIS
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

	EDGE* m_otheredge;
	double m_gap_size;
	double m_mid_gap_for_check;
	double m_st_gap_for_check;
	double m_end_gap_for_check;
	logical m_cannot_pair;
	double m_gap_from_surf_of_other_edge;

	hh_gap_size_entry() {reset();}

	hh_gap_size_entry(
		EDGE* other_edge,
		double gap,
		double mid_gap_for_check,
		double st_gap_for_check,
		double end_gap_for_check,
		double 	gap_from_surf_of_other_edge,
		logical cannot_pair)
	{
		m_otheredge = other_edge;
		m_gap_size = gap;
		m_mid_gap_for_check = mid_gap_for_check;
		m_st_gap_for_check = st_gap_for_check;
		m_end_gap_for_check = end_gap_for_check;
		m_gap_from_surf_of_other_edge = gap_from_surf_of_other_edge;
		m_cannot_pair = cannot_pair;
	}

	void reset() {
		*this = hh_gap_size_entry(NULL, -1.0, -1.0, -1.0,-1.0, -1.0,FALSE);
	}
};

// For internal use only
/**
 * @nodoc
 */
class DECL_STITCH hh_pt_perp_entry
{
public:
	/**
	 * @nodoc
	 */
	MMGR_FREELIST_THIS
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

	VERTEX* m_vert;
	SPAposition m_foot;
	logical m_position_on_edge;

	hh_pt_perp_entry() {reset();}
	hh_pt_perp_entry(
		VERTEX* vert,
		const SPAposition &foot,
		logical position_on_edge)
	{
		m_vert = vert;
		m_foot = foot;
		m_position_on_edge = position_on_edge;
	}

	void reset() {
		*this = hh_pt_perp_entry(NULL, SPAposition(0,0,0), FALSE);
	}
};

// For internal use only
/**
 * @nodoc
 */
class DECL_STITCH HH_GAP_SIZE_CACHE
{
	hash_table m_hash_table;
public:
// For internal use only
	/**
	 * @nodoc
	 */
	MMGR_FREELIST_THIS
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

	HH_GAP_SIZE_CACHE();
	~HH_GAP_SIZE_CACHE();

	void add_gap_size_entry(const hh_gap_size_entry& new_gap_size_entry);
	logical get_gap_size_entry(EDGE* edge, hh_gap_size_entry& entry);
	logical remove_gap_size_entry(EDGE* edge);
	void reset();
};

// For internal use only
/**
 * @nodoc
 */
class DECL_STITCH HH_PT_PERP_CACHE
{
	hash_table m_hash_table;
public:
// For internal use only
	/**
	 * @nodoc
	 */
	MMGR_FREELIST_THIS
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

	HH_PT_PERP_CACHE();
	~HH_PT_PERP_CACHE();

	void add_pt_perp_entry(const hh_pt_perp_entry& new_pt_perp_entry);
	logical get_pt_perp_entry(VERTEX* vertex, hh_pt_perp_entry& entry);
	logical remove_pt_perp_entry(VERTEX* vertex);
	void reset();
};


// For internal use only
/**
 * @nodoc
 */
DECL_STITCH extern int ATTRIB_HH_ENT_STITCH_EDGE_TYPE;

/**
 * @nodoc
 */
#define ATTRIB_HH_ENT_STITCH_EDGE_LEVEL (ATTRIB_HH_ENT_STITCH_BASE_LEVEL + 1)

class ATTRIB_HH_AGGR_STITCH;

/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated. For healing, use API interfaces
 * and not Direct Interface.
 * <br><br>
 * Individual entity-level healing attribute class attached to edges in the stitching phase.
 * <br>
 * <b>Role:</b> <tt>ATTRIB_HH_ENT_STITCH_EDGE</tt> is the individual entity-level attribute class attached to
 * edges during the stitching phase. Individual entity-level attributes are attached to the individual
 * entities of body being healed to store entity-specific information about each phase or subphase of
 * the healing process. The individual entity-level attributes for each phase are managed by the
 * aggregate attribute for that phase.
 * @see SPAposition, EDGE
 */
class DECL_STITCH ATTRIB_HH_ENT_STITCH_EDGE : public ATTRIB_HH_ENT_STITCH_BASE
{
protected:

	EDGE* m_partner_edge;	// Paired edge

	double m_gap; // gap size between the paired edges
	EDGE* gap_ref_edge;		// Edge whose gap size is stored

	SPAposition start;	// original start pos
	SPAposition end;	// origial end pos
	double m_len;		// original length
	double m_box_tol;	//Tolerance depending on SPAbox size.PVA(1.12.98)

	// MA (5 July 99) : Commenting
	// Storing history was not found useful. In fact it could cause problems.

	// Variables used to store history of the previous
	// analysis, just to save time.
	//EDGE* prev_edge;
	//double prev_tol;
	//double prev_gap;
	//logical prev_ret_val;
// STI cpp begin
		logical save_sw;
// STI cpp end

	// Functions/members for gap_size cache
	HH_GAP_SIZE_CACHE m_gap_size_cache;
	HH_PT_PERP_CACHE m_pt_perp_cache;
	//logical analyze_using_cache(EDGE*, double tol, double& gap);

	logical analyze_using_cache(EDGE*, double st_tol,double end_tol, double mid_tol, double& gap);

	logical get_gap_from_cache(EDGE*, double& gap,
				double& mid_gap_for_check, double& st_gap_for_check,double& end_gap_for_check, double& gap_from_surf_of_other_edge, logical& cannot_pair);
	logical add_gap_size_entry_to_cache(EDGE*, double gap,
				double mid_gap_for_check, double st_gap_for_check,double end_gap_for_check, double gap_from_surf_of_other_edge, logical cannot_pair);

	 
	
	// Store the original pcurve for the coedge whose edge is getting
	// split/merged.
	PCURVE* m_orig_pcur;
	// Store the original pcurve for the partner coedge whose edge is
	// getting split/merged.
	PCURVE* m_orig_partner_pcur;
	// Bit the store the original coedge sense. This is needed to
	// the negate the pcurve if necessary.
	REVBIT m_orig_coedge_sense;
	// Bit the store the original coedge partner sense. This is needed to
	// the negate the pcurve if necessary.
	REVBIT m_orig_coedge_partner_sense;

	// Store the original pcurve for the coedge whose edge is getting
	// split at the onset of stitch, so that while merging the untouched
	// edges we can replace the coedge with its original geometry.
	// Also store the coedge sence and an enum storing the
	// info weather the splitting vertex is at the start or the end of
	// the edge OR weather the edge has been split at all.
	PCURVE* m_unsplitted_coedge_pcur;
	REVBIT m_unsplitted_coedge_sense;
	STCH_EDGE_SPLIT_INFO m_stch_edge_split_info;

	logical m_skip_stitch;


	// sxt 79782
	 merge_cur_treatment m_merge_cur_trt; // treatment while merging with other edge
	 logical m_use_curve;

public:

	// Constructor
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt> operator inherited
 * from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_HH_ENT_STITCH_EDGE(...)</tt>), because this reserves
 * the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param e
 * owning edge.
 */
	ATTRIB_HH_ENT_STITCH_EDGE (EDGE* e = NULL);

/**
 * Returns <tt>TRUE</tt> if this is pattern compatible.
 */
    virtual logical pattern_compatible() const;
                                    // returns TRUE
	// Used when edge is split.

/**
 * Splits the attribute when the edge is split.
 * <br><br>
 * @param ent
 * owning entity.
 */
	virtual void split_owner(ENTITY* ent);
/**
 * Notifies the <tt>class_name</tt> that its owning <tt>ENTITY</tt> is about to be merged with given entity.
 * <br><br>
 * <b>Role:</b> The application has the chance to delete or otherwise modify the attribute. After the merge,
 * this owner will be deleted if the logical deleting owner is <tt>TRUE</tt>, otherwise it will be retained
 * and other entity will be deleted. The default action is to do nothing. This function is supplied by the
 * application whenever it defines a new attribute, and is called when a merge occurs.
 * <br><br>
 * @param ent
 * given entity.
 * @param flag
 * deleting owner.
 */
	virtual void merge_owner(ENTITY* ent, logical flag);

	// Analyze this edge and another edge to see if they
	// can be paired. This could be used by UI. This function
	// would return the gap size between the two edges
/**
 * Analyzes the owning edge and another specified edge to see if they can be paired at the specified tolerance.
 * <br><br>
 * <b>Role:</b> This method returns the gap size between the two edges.
 * <br><br>
 * @param e
 * edge to test pairing.
 * @param tol
 * tolerance to test at.
 * @param gap
 * gap between edges.
 */
	logical analyze(EDGE* e, double tol, double& gap);


/**
 * @nodoc
 */
	logical analyze(EDGE* e, double st_tol, double end_tol, double mid_tol,double& gap);


	// Sets the partner edge of the owner as the given edge.
	// This function does no analysis and dumbly sets the
	// partner. Hence it is risky to use this function without
	// prior analysis.
/**
 * Sets the partner edge of the owner as the given edge.
 * <br><br>
 * <b>Role:</b> This method just sets the partner and does no analysis. Therefore, analysis must be done
 * before using this method.
 * <br><br>
 * @param e
 * partner edge.
 */
	void set_partner_edge(EDGE* e);

	// Function to pair edge with another edge. Could
	// be set by UI. This performs an analysis of whether
	// the edges can be paired at the given tolerances.
	// and returns FALSE if unsuccessful.
/**
 * Pairs the owning edge with another edge.
 * <br><br>
 * <b>Role:</b> This performs an analysis of whether the edges can be paired at the given tolerance, and
 * returns <tt>FALSE</tt> if unsuccessful.
 * <br><br>
 * @param e
 * edge to pair with.
 * @param tol
 * tolerance to pair at.
 */
	logical pair(EDGE* e, double tol);

/**
 * @nodoc
 */
	logical pair(EDGE* e, double end_tol, double mid_tol);
	// Get the partner edge
/**
 * Gets the partner edge.
 */
	EDGE* partner_edge() const;

	// Find if the owner edge is unshared
/**
 * Determines if the owner edge is unshared and returns an appropriate flag.
 */
	logical unshared();

	// Returns whether the owner edge is either shared or has
	// has got paired up with another edge
/**
 * Returns <tt>TRUE</tt> if the owner edge is either shared or has has been paired up with another edge.
 */
	logical is_paired();

	// Get the gap size between the owner edge and the paired edge
	// This will return a valid size only if the owner edge is paired.
/**
 * Gets the gap size between the owner edge and the paired edge.
 * <br><br>
 * <b>Role:</b> This returns a valid size only if the owner edge is paired.
 */
	double gap_size();

	// Sets the gap size between the two stitched edges
/**
 * Sets the gap.
 * <br><br>
 * @param gap
 * new gap.
 */
	void set_gap(double gap);

	//Set the SPAbox tol of the face to the value passed PVA(1-12-98)
/**
 * Returns the <tt>box_tol</tt>, the tolerance depending on the box size.
 */
	double box_tol();
/**
 * Sets the <tt>box_tol</tt>, the tolerance depending on the box size.
 * <br><br>
 * @param box_tol
 * box tolerance.
 */
	void set_box_tol(double box_tol){backup(); m_box_tol = box_tol;}

	// Return the original start and end pos
/**
 * Returns the original start position.
 */
	SPAposition get_start() {return start;}
/**
 * Returns the original end position.
 */
	SPAposition get_end() {return end;}
/**
 * Sets the start position.
 * <br><br>
 * @param pos
 * new start position.
 */
	void set_start(const SPAposition& pos) {backup(); start = pos;}
/**
 * Sets the end position.
 * <br><br>
 * @param pos
 * new end position.
 */
	void set_end(const SPAposition& pos) {backup(); end = pos;}
/**
 * Returns the original length.
 */
	double get_len();
/**
 * Sets the length.
 * <br><br>
 * @param len
 * new length.
 */
	void set_len(double len) {backup(); m_len = len;}
// For internal use only
/**
 * @nodoc
 */
	logical get_pt_perp_from_cache(VERTEX*, SPAposition&, logical& );
// For internal use only
/**
 * @nodoc
 */
	void reset_cache();
// For internal use only
/**
 * @nodoc
 */
	void reset_cache_for_nearby_edges();
/**
 * Logs edge stitch details.
 * <br><br>
 * @param data1
 * log data.
 * @param data2
 * log data.
 */
	void set_stitch_edge_log_details(double * data1 = NULL, double * data2 = NULL);
/**
 * Returns edge stitch details from the log.
 * <br><br>
 * @param data1
 * log data.
 * @param data2
 * log data.
 * @param data3
 * log data.
 */
	void sprint_stitch_edge_log_details(char * data1 = NULL, double * data2 = NULL, double * data3 = NULL);

	// Function for computing the stitch tol ceiling
	// for the edge. Added for the bug 67989.
// For internal use only
/**
 * @nodoc
 */
	double get_stitch_tol_ceiling( void );

	// New member Functions.
	// Function to get the coedge old pcurve from edge attribute.
// For internal use only
/**
 * @nodoc
 */
	PCURVE * get_old_pcurve();
	// Function to get the partner coedge old pcurve from edge attribute.
// For internal use only
/**
 * @nodoc
 */
	PCURVE * get_old_partner_pcurve();
	// Function to get the original coedge sense.
// For internal use only
/**
 * @nodoc
 */
	REVBIT get_orig_coedge_sense();
	// Function to get the original coedge partner sense.
// For internal use only
/**
 * @nodoc
 */
	REVBIT get_orig_coedge_partner_sense();

	// Function to get edge stitchability info
// For internal use only
/**
 * @nodoc
 */
	logical get_skip_stitch();

	// Function to set the coedge old pcurve to edge attribute.
// For internal use only
/**
 * @nodoc
 */
	void set_old_pcurve(PCURVE *);
	// Function to set the partner coedge old pcurve to edge attribute.
// For internal use only
/**
 * @nodoc
 */
	void set_old_partner_pcurve(PCURVE *);
	// Function to set the original coedge sense in the edge attribute.
// For internal use only
/**
 * @nodoc
 */
	void set_orig_coedge_sense(REVBIT);
	// Function to set the original coedge partner sense in the edge attribute.
// For internal use only
/**
 * @nodoc
 */
	void set_orig_coedge_partner_sense(REVBIT);

	// get/set functions for attributes.
// For internal use only
/**
 * @nodoc
 */
	PCURVE * get_unsplitted_coedge_pcurve();
// For internal use only
/**
 * @nodoc
 */
	REVBIT get_unsplitted_coedge_sense();
// For internal use only
/**
 * @nodoc
 */
	STCH_EDGE_SPLIT_INFO get_edge_split_info();

// For internal use only
/**
 * @nodoc
 */
	void set_unsplitted_coedge_pcurve(PCURVE *);
// For internal use only
/**
 * @nodoc
 */
	void set_unsplitted_coedge_sense(REVBIT);
// For internal use only
/**
 * @nodoc
 */
	void set_edge_split_info(enum STCH_EDGE_SPLIT_INFO);

	// Function to set edge stitchability info
// For internal use only
/**
 * @nodoc
 */
	void set_skip_stitch(logical skip);

	// Function to transfer ATTRIB_HH_ENT_STITCH_EDGE attribute from one owner
	// entity to another owner entity. For eg, to a tolerant edge after stitch.
// For internal use only
/**
 * @nodoc
 */
	logical moveable() const;

		// Function to set the treatment while merging this edge
// For internal use only
/**
 * @nodoc
 */
	void set_merge_treatment(enum merge_cur_treatment trt);

		// Function to get the treatment while merging this edge
// For internal use only
/**
 * @nodoc
 */
	merge_cur_treatment get_merge_treatment();

	// Function to set the treatment while merging this edge
// For internal use only
/**
 * @nodoc
 */
	void set_use_curve(logical);


/**
 * @nodoc
 */
//logical notify_stitch(EDGE* edge, double end_tol,double mid_tol);

// For internal use only
/**
 * @nodoc
 */	
logical get_gap_from_surf_of_edge(EDGE* edge,double& gap_from_surf_of_other_edge);


// Function to get the treatment while merging this edge
// For internal use only
/**
 * @nodoc
 */
	logical should_curve_be_used();


	// Standard Attrib functions

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS (ATTRIB_HH_ENT_STITCH_EDGE, STITCH)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	/**
	 * @nodoc
	 */
	ROLL_NOTIFY_DECL
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

};

// For internal use only
/**
 * @nodoc
 */
DECL_STITCH ATTRIB_HH_ENT_STITCH_EDGE* find_att_edge_stitch(EDGE*);

// For internal use only
/**
 * @nodoc
 */
logical hh_are_two_edge_geom_almost_same( EDGE* edge1, 		//  First Edge
	 							EDGE* edge2,		//  Second edge
								double st_toler,
								double end_toler,
								double mid_toler,
								double& gap_width
								);

/** @} */
#endif // ATTRIB_HH_ENT_STITCH_EDGE_CLASS
