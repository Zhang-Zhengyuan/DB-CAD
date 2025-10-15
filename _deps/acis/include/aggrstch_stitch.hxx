/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


#if !defined (ATTRIB_HH_AGGR_STITCH_CLASS)
#define ATTRIB_HH_AGGR_STITCH_CLASS

/**
 * @file aggrstch_stitch.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup STITCHAPI
 *
 * @{
 */
#include "alltop.hxx"
#include "lists.hxx"
#include "eelists.hxx"
#include "dcl_stitch.h"

#include "aggrstchbase.hxx"

#include "stchapi.hxx"

// For internal use only
/**
 * @nodoc
 */
DECL_STITCH extern int ATTRIB_HH_AGGR_STITCH_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_HH_AGGR_STITCH_LEVEL (ATTRIB_HH_AGGR_STITCH_BASE_LEVEL + 1)

class ATTRIB_HH_ENT_STITCH_EDGE;
class EE_LIST;

// For internal use only
/**
 *@nodoc
 */
enum ANAL_STITCH {
	FULL_STITCH,			// Denotes input has full topology
	PARTIAL_STITCH, 		// Denotes input has partial topology
	NO_STITCH				// Denotes input has no topology
};

// For internal use only
/**
 * @nodoc
 */
struct DECL_STITCH bhl_topology_check_results {

	logical is_non_manifold;  // to indicate if the body has non-manifold edges

	int n_unshared_loops;				// Number of unshared loops
	int n_valid_unshared_edges;		// Number of valid unshared edges

	// Fields holding counts of instances of Wrong Topology

	int n_invalid_unshared_edges;		// Number of invalid unshared edges
	int n_seq_coedges_no_common_vert; // Number of sequential coedges not sharing common vertex
	int n_partners_no_common_edge; // Number of coedge partners not sharing common edge
	int n_wrong_coedge_sense_in_loop; // Number of coedges with wrong coedge sense
	int n_wrong_partner_coedge_sense; // Number of partner coedges not having opposite sense

	// Constructor
// For internal use only
/**
 * @nodoc
 */
	bhl_topology_check_results() { reset(); }

// For internal use only
/**
 * @nodoc
 */
	void reset()
	{
		is_non_manifold = FALSE;

		n_unshared_loops = 0;
		n_valid_unshared_edges = 0;

		n_invalid_unshared_edges = 0;
		n_seq_coedges_no_common_vert = 0;
		n_partners_no_common_edge = 0;
		n_wrong_coedge_sense_in_loop = 0;
		n_wrong_partner_coedge_sense = 0;
	}
};

/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated.
 * Class for storing results of Stitch Analyze phase.
 */
class DECL_STITCH bhl_anal_stitch_results {
public:
	/**
	 * Denotes input analysis summary.
	 */
	ANAL_STITCH bhl_anal_stitch;	// Denotes input analysis summary
	/**
	 * Recommended minimum tolerance.
	 */
	double bhl_stitch_recommended_min_tol;		// Recommended minimum tolerance
	/**
	 * Recommended maximum tolerance.
	 */
	double bhl_stitch_recommended_max_tol;		// Recommended maximum tolerance
	/**
	 * Determines if the incoming topology should be ignored.
	 */
	logical bhl_ignore_topology_recommended;	// Whether incoming topology be ignored

	// Results of topology check of input body
	/**
	 * Results of topology check of input body.
	 */
	bhl_topology_check_results topology_check_results;

	// Constructor function
	/**
	 * C++ constructor requests memory for this object and initializes the object option.
	 */
	bhl_anal_stitch_results ()
	{
		bhl_stitch_recommended_min_tol = 0.0;
		bhl_stitch_recommended_min_tol = 0.0;
		bhl_ignore_topology_recommended = TRUE;
	}

};
/**
 * @nodoc
 * For internal use only
 */
class DECL_STITCH bhl_stitch_options {
public:
	double bhl_curr_stitch_tol ;
	logical bhl_ignore_top ;
	double bhl_stitch_min_tol ;
	double bhl_stitch_max_tol ;
	int bhl_stitch_steps ;
    logical bhl_stitch_check_normals ;
    logical bhl_stitch_face_normals ;
    logical bhl_make_one_solid;
//    logical bhl_stitch_repeat ;
	logical hh_stitch_attrib ;
	logical bhl_stitch ;
	logical bhl_stitch_plus;
	logical bhl_stitch_option; //TRUE if incremental
	double bhl_body_box_size;
	double bhl_tang_tol;

	// RS (Mar 07,2002) : Value to store the effective maximum stitch tolerance
	// This is the least maximum tolerance at which all the edges got really stitched
	double bhl_eff_stitch_max_tol;

	double m_sliver_param;

	//constructor function
	bhl_stitch_options()
	{

		bhl_curr_stitch_tol = 0;
		bhl_ignore_top = FALSE;
		bhl_stitch_min_tol = 0.00001;
		bhl_stitch_max_tol = 1.0;
		bhl_stitch_steps = 4;
		bhl_stitch_check_normals = FALSE;
		bhl_stitch_face_normals = FALSE;
		bhl_make_one_solid = FALSE;
//		bhl_stitch_repeat = FALSE;
		hh_stitch_attrib = FALSE;
		bhl_stitch = TRUE;
		bhl_stitch_plus = FALSE;
		bhl_stitch_option = TRUE;
		bhl_body_box_size = -1.0;
		// RS (Jul 05,2001) : Initialize the tang_tol to 0.1
		bhl_tang_tol = 0.1 ;
		bhl_eff_stitch_max_tol = bhl_stitch_min_tol;
	}
};

// For internal use only
/**
 * @nodoc
 */
// Class for Stitch results

class DECL_STITCH bhl_stitch_results {
public:
	int bhl_stitch_no_solids;			// Number of solid bodies made
	int bhl_stitch_no_open;				// Number of open bodies made
	int bhl_stitch_no_unshared_edges;	// Number of final unshared edges
	int bhl_no_unshared_loops;				// Number of unshared loops
	int bhl_no_valid_unshared_edges;		// Number of valid unshared edges
	int bhl_no_invalid_unshared_edges;		// Number of invalid unshared edges
	int bhl_stitch_curr_shared_edges ;
	int bhl_stitch_curr_total_edges ;
	int bhl_stitch_no_total_edges ;
	int bhl_stitch_tot_unshared_edges ;
	int bhl_stitch_num_solids;			// Number of solid bodies made
	int bhl_stitch_num_open;			// Number of solid bodies made
	int bhl_stitch_curr_tol_count ;
	int bhl_stitch_curr_tol_order ;
	int bhl_tot_no_coalasced_edges ;
	int bhl_tot_no_merged_edges ;
	int bhl_tot_no_split_edges  ;
	double bhl_stitch_tol ;
	logical bhl_curr_stitch_state ;

	// Results topology check of stitched body
	bhl_topology_check_results topology_check_results;

	// Constructor function
	bhl_stitch_results ()
	{
		bhl_stitch_no_solids = 0;
		bhl_stitch_no_open = 0;
		bhl_stitch_no_unshared_edges = 0;
		bhl_no_unshared_loops = 0;
		bhl_no_valid_unshared_edges = 0;
		bhl_no_invalid_unshared_edges = 0;
		bhl_stitch_curr_shared_edges = 0;
		bhl_stitch_curr_total_edges = 0;
		bhl_stitch_no_total_edges = 0;
		bhl_stitch_tot_unshared_edges = 0;
		bhl_stitch_num_solids = 0;
		bhl_stitch_num_open = 0;
		bhl_stitch_curr_tol_count = 0;
		bhl_stitch_curr_tol_order = 0;
		bhl_tot_no_coalasced_edges = 0;
		bhl_tot_no_merged_edges = 0;
		bhl_tot_no_split_edges = 0;
		bhl_stitch_tol = 0.01;
		bhl_curr_stitch_state = 0;
	}

} ;

/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated. For healing, use API interfaces
 * and not Direct Interface.
 * <br><br>
 * Aggregate healing attribute class for the stitching phase.
 * <br>
 * <b>Role:</b> The <tt>ATTRIB_HH_AGGR_STITCH</tt> class is attached to the body to be healed. It is used by
 * the stitching healing phase.
 */
class DECL_STITCH ATTRIB_HH_AGGR_STITCH : public ATTRIB_HH_AGGR_STITCH_BASE
{

/////////////////////////
// MEMBER DATA VARIABLES

protected :

	logical m_do_stitch;	// Logical to denote if
							// stitching is required
	logical m_stepped;		// Logical to denote to
							// perform incremental or
							// stepped stitching
	double m_min_tol;		// Minimum Stitch tolerance
	double m_max_tol;		// Maximum Stitch tolerance

	logical m_stitch_performed;

	// RNC 15 july 99: addition of option for logging changes to entities
	// in stitching module
	logical m_do_stitch_log;

	// Class/Struct containing results of stitching
	bhl_anal_stitch_results anal_stitch_results;
	bhl_stitch_results m_stitch_results;
	bhl_stitch_options stitch_opt;
	logical invalid_stitch;
// STI cpp begin
		logical save_sw;
// STI cpp end

	// Stitches two edges, one is the att->owner() and the
	// other is the att->partner_edge()
	logical stitch_two_edges (ATTRIB_HH_ENT_STITCH_EDGE* att);

	// Attach attributes to all edges
	void attach_attribs_to_edges();

	// GSSL/RS - (Jun 21,2002) : Trim the coedges of all the edges
	// that are marked for stitching
	logical trim_marked_coedges(void);

	// GSSL/RS - (Jun 26,2002) : Member to store the edges that got stitched
	EE_LIST *m_edges_stitched;

	// Tolerant stitch option data member.
	tolerant_stitch_options* m_sopts;

	// GSSL/RS - (Jun 03,2003) : Whether the topology check is to be skipped in analyze
	logical m_skip_topo_check;

public:

////////////////////
// MEMBER FUNCTIONS

 	// Constructor
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt> operator inherited
 * from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_HH_AGGR_STITCH(...)</tt>), because this reserves the memory on
 * the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param owner
 * owning body.
 */
	ATTRIB_HH_AGGR_STITCH (BODY* owner = NULL);

/**
 * Returns <tt>TRUE</tt> if this is pattern compatible.
 */
    virtual logical pattern_compatible() const;
                                    // returns TRUE
 	// Chain individual attributes
/**
 * Attaches an individual entity-level attribute to the given entity.
 * <br><br>
 * <b>Role:</b> This method chains individual attributes.
 * <br><br>
 * @param e
 * entity to attach to.
 */
	virtual ATTRIB_HH_ENT*  attach_attrib( ENTITY* e);

	// Get all the entities chained to the body. These are
	// entities to which individual attributes are attached.
/**
 * Gets all the entities chained to the body.
 * <br><br>
 * <b>Role:</b> These are entities to which individual attributes are attached.
 * <br><br>
 * @param e
 * list of entities.
 */
	virtual void entity_list(ENTITY_LIST& e) const;

	// Get the attribute for the corresponding entity
/**
 * Gets the entity-level attribute for the corresponding entity.
 * <br><br>
 * @param e
 * owning entity.
 */
	virtual ATTRIB_HH_ENT* get_attrib( ENTITY* e) const;

	//MG16Apr99 get the stitch options
/**
 * @nodoc
 */
	bhl_stitch_options const get_stitch_options(){
		return  stitch_opt;}
/**
 * @nodoc
 */
	bhl_stitch_options &get_stitch_options_for_update();

/**
 * @nodoc
 */
	bhl_stitch_options * set_stitch_options()
	{
		backup();
		return  &stitch_opt;
	}
/**
 * @nodoc
 */
	bhl_stitch_results const get_stitch_results()
	{
		return  m_stitch_results;
	}


 	// Remove the attribute for the entity
/**
 * Removes the entity-level stitch attribute from the entity.
 * <br><br>
 * @param e
 * owning entity.
 */
	virtual void detach_attrib( ENTITY* e);

	// If edge is already shared and doesnt need to be stitched
	// then detach its attrib.
/**
 * If an edge is already shared and doesn't need to be stitched this method detaches its entity-level stitch attribute.
 */
	void detach_redundant_attribs();

	// Remove all individual attributes.
	// LOSE_DEF calls it as well
/**
 * Removes all individual entity-level attributes from the entities of the owning body.
 */
	virtual void  cleanup();

	// Reset cache in all edges
/**
 * Resets cache in all edges.
 */
	void reset_cache();

	// Pair up two given edges. Return true if possible.
/**
 * Attempts to pair up two given edges.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if it is possible to pair the two edges.
 * <br><br>
 * @param edge1
 * first edge.
 * @param edge2
 * second edge.
 * @param tol
 * tolerance to use.
 */
	logical pair (EDGE* edge1, EDGE* edge2, double tol);

	// Reset the options and tolerances based on heuristics
/**
 * Performs the analyze stage of stitching.
 * <br><br>
 * <b>Role:</b> Analyzes and classifies the types of entities in the body, determines the number
 * of each type of entity, and determines an appropriate "stitch box" to use for stitching.
 * This method then sets the options and tolerances needed by stitching based on this analysis of
 * the body.
 */
	virtual void analyze();

	// Perform the pairing and attach individual attributes
	// to edges
/**
 * This method performs the pairing of edges and populates the individual entity-level attributes to the edges.
 * <br><br>
 * <b>Role:</b> This method splits lumps into bodies, performs the stitching calculations on these
 * bodies and stores the results, then merges the lumps back into the owner body.
 */
	virtual void calculate();

	// Traverse the body and add entity attributes to edge pairs
	// which are candidates for stitching at a given tolerance.
/**
 * Traverses the body and adds individual entity-level attributes to edge pairs that are candidates for stitching at the specified tolerance.
 * <br><br>
 * @param tol
 * tolerance to use.
 */
	void calculate_at_tol(double tol);

	// Do stitch al the edges paired up by the attributes.
/**
 * Applies (fixes) all the changes that are stored in individual attributes for the stitching phase to the body.
 * <br><br>
 * <b>Role:</b> The old topology is then stored in the attributes. This method actually performs the
 * stitching of the edges that are paired up via the individual attributes.
 */
	virtual void fix();

	// Print all the attrib results into string
/**
 * Prints statistics of the results of the stitching phase to the specified string.
 * <br><br>
 * @param str
 * character string.
 */
	virtual void sprint(char *str);

	// Print results of analysis into a string
/**
 * Prints statistics of the results of the analyze stage of the stitching phase to the specified string.
 * <br><br>
 * @param str
 * character string.
 */
	virtual void sprint_analyze(char *str);

	// Print results of calculations into a string
/**
 * Prints statistics of the results of the calculate stage of the stitching phase to the specified string.
 * <br><br>
 * @param str
 * character string.
 */
	virtual void sprint_calculate(char *str);

	// Print results of fix into a string
/**
 * Prints statistics of the results of the fix stage of the stitching phase to the specified string.
 * <br><br>
 * @param str
 * character string.
 */
	virtual void sprint_fix(char *str);

	// Print all the attrib results
/**
 * Prints statistics of the results of the stitching phase to the specified file.
 * <br><br>
 * @param fp
 * file pointer.
 */
	virtual void print(FILE *fp);

	// Print results of analysis
/**
 * Prints statistics of the results of the analyze stage of the stitching phase to the specified file.
 * <br><br>
 * @param fp
 * file pointer.
 */
	virtual void print_analyze(FILE *fp);

	// Print results of calculations
/**
 * Prints statistics of the results of the calculate stage of the stitching phase to the specified file.
 * <br><br>
 * @param fp
 * file pointer.
 */
	virtual void print_calculate(FILE *fp);

	// Print results of fix
/**
 * Prints statistics of the results of the fix stage of the stitching phase to the specified file.
 * <br><br>
 * @param fp
 * file pointer.
 */
	virtual void print_fix(FILE *fp);


	// Get/Set functions for stitch tolerances
/**
 * Logical to denote if stitching is required.
 */
	logical do_stitch() const;
/**
 * Performs stitching based on the value of the <tt>m_stepped</tt> flag.
 * <br><br>
 * <b>Role:</b> Gets the value of the <tt>m_stepped</tt> flag, which indicates whether or not stepped stitching should be used.
 * If <tt>TRUE</tt>, stepped stitching is performed before incremental. Otherwise, only incremental
 * stitching is performed.
 */
	logical stepped() const;
/**
 * Gets the current value of the minimum stitching tolerance.
 */
	double min_tol() const;
/**
 * Gets the current value of the maximum stitching tolerance.
 */
	double max_tol() const;

/**
 * Sets a flag indicating whether or not the stitching phase needs to be used.
 * <br><br>
 * <b>Role:</b> If <tt>TRUE</tt>, the stitching phase is used; otherwise, it is not used.
 * <br><br>
 * @param flag
 * new flag value.
 */
	void set_do_stitch(logical flag);
/**
 * Sets the value of the <tt>m_stepped</tt> flag, which indicates whether or not stepped stitching should be used.
 * <br><br>
 * <b>Role:</b> If <tt>TRUE</tt>, stepped stitching is performed before incremental. Otherwise, only
 * incremental stitching is performed.
 * <br><br>
 * @param m_stepped
 * new flag value.
 */
	void set_stepped(logical m_stepped);
/**
 * Sets the new value of the minimum stitching tolerance.
 * <br><br>
 * @param tol
 * tolerance to use.
 */
	void set_min_tol(double tol);
/**
 * Sets the new value of the maximum stitching tolerance.
 * <br><br>
 * @param tol
 * tolerance to use.
 */
	void set_max_tol(double tol);

	// Query functions for analysis
/**
 * Returns a number that indicates the type of body based on analysis.
 * <br><br>
 * <b>Role:</b>
 * <br>
 * &nbsp;&nbsp;&nbsp;&nbsp;1  Body is a solid<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;2  Body is a sheet<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;3  Body is very bad and needs user to intervene (e.g., manually stitch)
 */
	int type() const;		// Returns 1 if solid, 2 if sheet,
							// 3 if bad stitching needing user
							//  intervention

	// Returns the unshared edges which are valid and can be
	// be closed by cover sheet
/**
 * Returns a list of the unshared edges that are valid and can be closed by a cover sheet.
 * <br><br>
 * @param e
 * list of edges.
 */
	void valid_unshared_edges(ENTITY_LIST& e) const;

	// Returns the number of unshared loops which can probably
	// be closed by a cover sheet.
/**
 * Returns the number of unshared loops that can probably be closed by a cover sheet.
 */
	int num_unshared_loops() const;

	// Return the invalid unshared edges which require user
	// to manually stitch bad regions
/**
 * Returns the invalid unshared edges that require the user to manually stitch bad regions.
 * <br><br>
 * @param e
 * list of edges.
 */
	void invalid_unshared_edges(ENTITY_LIST& e) const;

	// Returns the number of invalid unshared edges formed
	// after stitch calculation
/**
 * Returns the number of invalid unshared edges formed after the stitch calculate stage.
 */
	int num_invalid_unshared_edges() const
	{
	    ENTITY_LIST elist;
		invalid_unshared_edges(elist);
		return elist.count();
	}

	// Returns the number of valid unshared edges formed
	// after stitch calculation
/**
 * Returns the number of the number of valid unshared edges formed after stitch calculation.
 */
	int num_valid_unshared_edges() const
	{
	    ENTITY_LIST elist;
		valid_unshared_edges(elist);
		return elist.count();
	}

	// Return number of individual parts
/**
 * Returns the number of solid lumps.
 */
	int num_solid_lumps() const;
/**
 * Returns the number of sheet lumps.
 */
	int num_sheet_lumps() const;
/**
 * Returns the number of free faces after stitching analysis.
 */
	int num_free_faces() const;

	// RNC : 3 Aug 99 log list addition functions
	// Get/Set functions for logging simplification module changes
/**
 * Returns <tt>FALSE</tt> if logging has been turned off, otherwise, returns <tt>TRUE</tt>.
 */
	logical do_stitch_log() const;
/**
 * Sets a flag indicating whether or not is to be used. If <tt>TRUE</tt>, logging is used; otherwise, it is not used.
 * <br><br>
 * @param flag
 * new flag value.
 */
	void set_do_stitch_log(logical flag);


	// Returns TRUE if any stitching was performed
/**
 * Returns <tt>TRUE</tt> if any stitching was performed.
 */
	logical stitch_performed() const { return m_stitch_performed;}

	// Results of stitching

	// For internal use only
/**
 * @nodoc
 */
	const bhl_stitch_results results() const {
		return m_stitch_results;
	}

	// GSSL/RS - (Jun 26,2002) : Single face stitching
 	// To get the list of stitched edges

	// For internal use only
/**
 * @nodoc
 */
	logical get_stitched_edges(ENTITY_LIST& stched_edges);

	// GSSL/RS - (Jun 27,2002) : Single face stitching
	// Tolerize only the stitched / modified edges

	// For internal use only
/**
 * @nodoc
 */
	logical tolerize_stitched_edges(ENTITY_LIST &edges_to_exclude);

	// For internal use only
/**
 * @nodoc
 */
	logical initialize_edges_stitched_list()
	{
		if (!m_edges_stitched)
		{	m_edges_stitched = ACIS_NEW EE_LIST (FALSE);	}
		return (m_edges_stitched != NULL);
	}
	// For internal use only
/**
 * @nodoc
 */
	logical destroy_edges_stitched_list()
	{
		if (m_edges_stitched)
		{
			m_edges_stitched->clear();
			m_edges_stitched->lose();
			m_edges_stitched = NULL;
			return TRUE;
		}
		return FALSE;
	}

	// Member function to get tolerant stitch option
/**
 * Gets the tolerant stitch option object.
 */
	tolerant_stitch_options* get_tolerant_stitch_option();

	// Member function to set tolerant stitch option
/**
 * Sets the tolerant stitch option object.
 */
	void set_tolerant_stitch_option(tolerant_stitch_options* sopts);

	// For internal use only
// tbrv
/**
 * @nodoc
 */
	logical is_topo_check_to_be_skipped() const;

	// For internal use only
// tbrv
/**
 * @nodoc
 */
	void skip_topo_check(logical skip);

	// For internal use only
/**
 * @nodoc
 */
	ATTRIB_FUNCTIONS (ATTRIB_HH_AGGR_STITCH, STITCH)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

/**
 * @nodoc
 * For internal use only
 */
DECL_STITCH ATTRIB_HH_AGGR_STITCH* find_aggr_stitch (BODY* inp_body);
/** @} */
#endif // ATTRIB_HH_AGGR_STITCH_CLASS
