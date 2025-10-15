/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header file for the bool options.

#if !defined(BOOL_API_OPTIONS_H)
#define BOOL_API_OPTIONS_H

//  Placeholder for options to api_boolean

#include "dcl_bool.h"
#include "acis_options.hxx"
#include "clash_bodies.hxx"

class ENTITY;
struct imprint_assoc_data;
class BoolOptions_internal;
class bool_options_ibw;
class incr_bool_handle_impl;

/**
 * \defgroup BOOLOPTSAPI Boolean Options
 * \ingroup BOOLOPTIONSAPI
 *
 */

/**
* @file bool_api_options_prot.hxx
* @CAA2Level L1
* @CAA2Usage U1
*/

/*! \addtogroup BOOLOPTSAPI
 *
 *  @{
 */


/**
 * As a pure abstract class this class has no public use as of now.
 * It simply serves as a common base class for all variants of Boolean options.
 */
class DECL_BOOL bool_base_options : public ACIS_OBJECT
{
protected:
	///\cond
    bool_base_options();
    bool_base_options(const char* gme);
    bool_options_ibw *bo_ibw_impl;
    friend incr_bool_handle_impl;
    ///\endcond

public:
    virtual ~bool_base_options() = 0;

    ///\cond
    bool is_ibw_active() const;
    bool gme_is_ibw_active() const;
    virtual int type() const = 0;
    ///\endcond
};


/**
* Enumeration to specify a body which takes part in
* a Boolean operation.
* @param BOOL_BODY_BLANK
* The blank body in a Boolean operation.
* @param BOOL_BODY_TOOL
* The tool body in a Boolean operation.
*/
enum bool_body_type 
{ 
   BOOL_BODY_BLANK = 0,
   BOOL_BODY_TOOL 
};


/**
* @nodoc
*
* Enum describing the nature of the induced intersections between coincident
* entities.
* <br>
* <b>Role:</b> The @href bool_interaction_type enum describes how the
* faces neighboring a matched entity interact with the other body.
* <br><br>
* The @href bool_match_info class uses this enum for providing additional
* information about a coincidence between a blank and a tool entity. Although
* this information does not ensure the success of the Boolean operation, it
* does, however, allow assumptions to be made to improve the performance of
* the operation. See the documentation on @href bool_match_info for a description
* of the circumstances in which this information is useful.
* <br><br>
* The values the enum may take include:
* <br><br>
* <b><tt>bool_interaction_unknown</tt>:</b> Indicates that it is not known how the faces
* neighboring a coincidence interact with the other body (or it is known,
* but does not fall into one of the other categories).
* <br><br>
* <b><tt>bool_interaction_global</tt>:</b> Indicates that for each pair of faces (that
* is, one face from tool and blank) for which the match induces an
* intersection, there is no other intersection between either face and the
* other body (apart from intersections induced by other matches).  This is
* the same as the condition which must be satisfied for all faces for a glue
* to be valid (see @href api_boolean_glue).
* @param bool_interaction_unknown
* Nothing is known about interaction between entities.
* @param bool_interaction_global
* Some intersection calculations may be safely avoided.
*/
enum bool_interaction_type
{
    bool_interaction_unknown,
    bool_interaction_global
};

/**
* @nodoc
*
* Enum describing the nature of a coincidence between entities.
* <br>
* <b>Role:</b> The @href bool_overlap_type enum describes the relative
* disposition of two faces.  The value of the enum is ignored if either
* entity is an edge.
* <br><br>
* This enum is used by the @href bool_match_info class to provide
* additional information about a coincidence between a blank and a tool
* entity.  This information is not essential to the success of the Boolean
* operation but may allow assumptions to be made to improve the performance
* of the operation.  See @href bool_match_info for a
* description of the circumstances in which this information is useful.
* <br><br>
* The values the enum may take are:
* <br><br>
* <b><tt>bool_overlap_general</tt>:</b> Indicates that the overlap between the
* entities is either unknown or is known but does not fall into one of the
* other categories.
* <br><br>
* <b><tt>bool_overlap_blank_contains_tool</tt>:</b> Indicates that the tool entity is
* strictly in the interior of the blank entity, and none of the boundaries of
* the tool entity meet any part of the boundary of the blank entity.
* <br><br>
* <b><tt>bool_overlap_tool_contains_blank</tt>:</b> Indicates that the blank entity is
* strictly in the interior of the tool entity.
* <br><br>
* <b><tt>bool_overlap_exact</tt>:</b> Indicates that the boundaries of the blank and
* tool entities are also coincident.  For a pair of faces, each vertex on one
* face meets a vertex on the other and each edge on one face is coincident
* with an edge on the other.  If an edge on one face is closed, the
* corresponding edge on the other face must also be closed, but it is not
* required for the vertices to be coincident.
* <br><br>
* @param bool_overlap_general
* Nothing is known about the overlap of the matched entities.
* @param bool_overlap_blank_contains_tool
* The blank entity completely contains the tool entity.
* @param bool_overlap_tool_contains_blank
* The tool entity completely contains the blank entity.
* @param bool_overlap_exact
* The tool and blank entities also match on their boundaries.
* <br><br>
*/
enum bool_overlap_type
{
    bool_overlap_general,
    bool_overlap_blank_contains_tool,
    bool_overlap_tool_contains_blank,
    bool_overlap_exact
};

/**
* Enum controlling whether artifacts are merged at the end of a regularized Boolean operation.
* <br>
* <b>Role:</b> The @href bool_merge_type enum is used in options to a Boolean
* operation to control what merging occurs at the end of that operation.
* <br><br>
* This enum is used by the BoolOptions class to control whether merging is
* done by the Boolean.  In addition, if no merging is carried out, it
* controls whether information about the entities which would have been
* considered for merging is retained for later use.  This last possibility is
* intended to allow applications to implement a delayed merge.
* <br><br>
* The values the enum may take are:
* <br><br>
* <b><tt>bool_merge_unset</tt>:</b> Indicates that merging is controlled by
* the global option <tt>merge</tt> and not by the BoolOptions object.  This is the
* default value for the merge type option in a BoolOptions object.
* <br><br>
* <b><tt>bool_merge_on</tt>:</b> Indicates mergeable entities created by the
* Boolean should be merged at the end of the operation.
* <br><br>
* <b><tt>bool_merge_off</tt>:</b> Indicates mergeable entities created by the
* Boolean should not be merged at the end of the operation.
* <br><br>
* <b><tt>bool_merge_delayed</tt>:</b> Indicates mergeable entities created by
* the Boolean should not be merged at the end of the operation, and a list of
* candidates for merging should be created and retained by the Boolean
* options.  When the merge option has this value the list of candidates can
* be obtained from the Boolean options using the method
* @href BoolOptions#list_merge_candidates.
* <br><br>
* <b>Note:</b> Merging does not occur during non-regularized Booleans;  
* therefore, this option has no effect on the result and the 
* list of merge candidates will always be empty.
* @param bool_merge_unset
* Control of merging is by a global option.
* @param bool_merge_on
* The merging is carried out.
* @param bool_merge_off
* Merging is not carried out.
* @param bool_merge_delayed
* Merging is not carried out and a list of candidates is returned.
* <br><br>
*/
enum bool_merge_type
{
    bool_merge_unset,
    bool_merge_on,
    bool_merge_off,
    bool_merge_delayed
};

/**
* @nodoc
*
* Class to hold coincidence information for a Boolean operation.
* <br><br>
* <b>Role:</b> A @href bool_match_info object supplies
* information about a known or intended
* coincidence between blank and tool entities.
* <br><br>
* The match information allows the caller to specify a pair of entities
* which is known to be coincident <em>by construction</em>, whether the
* entities are coincident according to the coincidence rules of ACIS or
* not.  It is also possible to supply extra information about the nature of
* the coincidence which may help to improve the performance of the
* Boolean.
* <br><br>
* Each of the entities may be a face or an edge, in any combination: that is, it
* is possible to specify tool-blank correspondences as face-face, face-edge,
* edge-face, and edge-edge matches.
* <br><br>
* A match between a pair of entities indicates that the entities may be
* regarded as coincident to the given tolerance.  An edge-edge match implies
* that the faces around the tool edge intersect the faces around the blank
* edge along the common curve of intersection.  An edge-face match implies
* the faces around the tool edge intersect the blank face along the curve of
* the edge, and similarly for a face-edge match.  A face-face match implies
* that all the neighbors of the tool face intersect the blank face along a
* curve coincident with the common edge of the faces, and similarly for the
* neighbors of the blank face.  Although it is common for the faces
* themselves to be coincident, it is not necessary for them to be coincident
* in order to be matched, merely that the condition on the edges of the faces
* is satisfied.
* <br><br>
* The tolerance indicates the level at which one expects the entities to be
* coincident.  The default value is the prevailing value of
* <tt>SPAresabs</tt> which is suitable for truly coincident entities.  If the
* entities are not coincident according to the rules of ACIS coincidence, but
* are intended to be coincident as far as the Boolean operation is concerned
* (for example, there are two edge curves, one of which is an approximation
* of the other), then this value should be an estimate of the tolerance at
* which the entities are coincident.  The value may be an upper bound, but
* should not be so loose as to cause clashes between these entities and
* distinct adjoining entities.  Note that if this value is larger than the
* prevailing value of <tt>SPAresabs</tt>, then any edge which is specified in
* the match and which survives the Boolean will be converted to a tolerant
* edge, even when a looser tolerance is not necessary.
* <br><br>
* The additional information consists of two parts: a type of interaction and
* the type of overlap. The type of interaction of the match refers to the
* induced intersections between adjoining entities. If the interaction type
* is not unknown, then the intersections induced by the match are the only
* intersections relevant to the Boolean. For example, an edge-face match
* implies that both the incident faces of the edge meet the matched face
* along the edge; the interaction type may be used to indicate that the
* neighboring faces do not meet the matched face anywhere else. One may set
* the type of interaction when other intersections exist, but these are
* induced by another match which is also specified in the same set of
* options.
* <br><br>
* If the interaction type is known, then the Boolean may be able to reduce
* the number of intersection calculations it performs. Additionally, if one
* specifies the overlap type (see @href bool_overlap_type for values) , then
* further optimizations may be possible.
*/
class DECL_BOOL bool_match_info : public ACIS_OBJECT
{
private:
    ENTITY   *tool_entity_;
    ENTITY   *blank_entity_;
    double    tolerance_;
    bool_interaction_type interaction_type_;
    bool_overlap_type   overlap_type_;

public:
    /**
    * Default constructor.  By default, the tool and blank entities are NULL, the
    * tolerance 0, the interaction type <tt>bool_interaction_unknown</tt>, and
    * the overlap type <tt>bool_overlap_general</tt>.
    */
    bool_match_info();
    bool_match_info(const char* gme);

    /**
    * Destructor.
    */
    virtual ~bool_match_info(){};

    /**
    * Copies the input values to a @href bool_match_info object.
    * @param tool_entity
    * Face or edge in the tool body.
    * @param blank_entity
    * Face or edge in the blank body.
    * @param tol = 0
    * Tolerance of the match.
    * @param interaction_type
    * Property of the intersections between the entities.
    * @param overlap_type
    * Property of the overlap between the entities.
    */
    void bool_match_info_initialise(
        ENTITY                *tool_entity,
        ENTITY                *blank_entity,
        double                 tol = 0,
        bool_interaction_type  interaction_type = bool_interaction_unknown,
        bool_overlap_type      overlap_type = bool_overlap_general
        );
    void gme_bool_match_info_initialise(
        ENTITY                *tool_entity,
        ENTITY                *blank_entity,
        double                 tol = 0,
        bool_interaction_type  interaction_type = bool_interaction_unknown,
        bool_overlap_type      overlap_type = bool_overlap_general
        );

    /**
    * Accessor for tool entity.
    */
    ENTITY *get_tool_entity() const { return tool_entity_; };

    /**
    * Accessor for blank entity.
    */
    ENTITY *get_blank_entity() const { return blank_entity_; };

    /**
    * Accessor for match tolerance.
    */
    double  get_tolerance() const { return tolerance_; };

    /**
    * Accessor for match interaction type.
    */
    bool_interaction_type get_interaction_type() const { return interaction_type_; };

    /**
    * Accessor for match overlap type.
    */
    bool_overlap_type get_overlap_type() const { return overlap_type_; };

    /**
    * Sets match tool entity.
    * @param ent
    * Face or edge in the tool body.
    */
    void set_tool_entity(ENTITY *ent) { tool_entity_ = ent; };

    /**
    * Sets match blank entity.
    * @param ent
    * Face or edge in the blank body.
    */
    void set_blank_entity(ENTITY *ent) { blank_entity_ = ent; };

    /**
    * Sets match tolerance.
    * @param ent
    * The tolerance of the match.
    */
    void set_tolerance(double tol) { tolerance_ = tol; };

    /**
    * Sets match interaction type.
    * @param interaction_type
    * The interaction type of the match.
    */
    void set_interaction_type(bool_interaction_type  interaction_type) { interaction_type_ = interaction_type; };

    /**
    * Sets match overlap type.
    * @param overlap_type
    * The overlap type of the match.
    */
    void set_overlap_type(bool_overlap_type overlap_type) { overlap_type_ = overlap_type; };
    
    /**
    * @nodoc
    * Always returns FALSE.
    * @param bmi
    * bool_match_info object.
    */
    logical operator== (bool_match_info const& bmi) const
    {
        SPAUNUSED(bmi);
        return FALSE;
    };
    /**
    * @nodoc
    * Always returns TRUE.
    * @param bmi
    * bool_match_info object.
    */
    logical operator!= (bool_match_info const& bmi) const
    {
        SPAUNUSED(bmi);
        return TRUE;
    };

};

/**
* Class to hold information and options for a Boolean operation.
* <br>
* <b>Role:</b> A @href BoolOptions object supplies information to, and retrieves
* information from, a given Boolean operation. 
* <br><br>
* This class can be used to specify a <b>near coincidence fuzz</b> value. If this
* value is set and is greater than <tt>SPAresabs</tt> then it is used as a hint to the Boolean
* that pairs of entities that are coincident to within the fuzz value may be
* regarded as coincident. The Boolean may then force coincidence by adding
* tolerances where necessary. Depending on the situation the fuzz value may be
* ignored by the Boolean (for instance, if forcing coincidence would result in
* the creation of bad geometry). The default value for this parameter is 0.
* <br><br>
* The class can be used to control whether merging is done by the Boolean operation. 
* This is controlled by the <b>merge type</b> parameter. This parameter may have one of
* four possible settings:
* <br>
* <ul>
* <li><b>Unset</b> - specifies that merging at the end of regularized Boolean operations is controlled
      by the global option <tt>merge</tt>.</li>
* <li><b>On</b> - specifies that merging will be performed at the end of regularized Boolean operations.</li>
* <li><b>Off</b> - specifies that merging will not be performed at the end of regularized Boolean operations.</li>
* <li><b>Delayed</b> - specifies that merging will not be performed at the end of regularized Boolean operations;
*     however, a list of merge candidates will be returned in the BoolOptions object.
*     This allows applications to implement a <i>delayed merge</i>. The list of merge candidates 
*     can be obtained using the method @href list_merge_candidates.</li>
* </ul>
* The default value for the merge type is <tt>bool_merge_unset</tt>.
* <br><br>
* The class can be used to return lists of imprinted edges or/and vertices after an imprint
* operation. (A vertex is included in the list if the imprint operation resulted in a degenerate edge 
* in one of the bodies.) This functionality is controlled by the flag set by @href set_track_entities
* method and retrieved by @href get_track_entities method. The default value for the <b>track entities</b> 
* parameter is <tt>FALSE</tt>. The lists are obtained by the method
* get_imprinted_entities(bool_body_type, ENTITY_LIST&). The enum @href bool_body_type 
* controls whether the entities to be returned are from the blank body or tool body. In either case
* the associated entities (that is, the entities in the other body) can be retrived by calling 
* the method get_associated_entities(ENTITY *, ENTITY_LIST&).
* <br><br>
* The class can be used to return information on whether the blank and tool bodies clash 
* following a Boolean operation. This is returned via the @href get_clash_info method.
*/
class DECL_BOOL BoolOptions : public bool_base_options
{
public:

    /**
    * Default constructor
    */
    BoolOptions();
    BoolOptions(const char* gme);
    /**
    * Destructor.
    */
    virtual ~BoolOptions();

    /**
    * @nodoc
    * Accessor which returns the number of <tt>bool_match_info</tt> objects
    * stored in this object.
    */
    int num_matches() const { return num_matches_; };

    /**
    * @nodoc
    * Accessor which returns a pointer to the nth <tt>bool_match_info</tt> object
    * stored in this object.
    * @param n
    * Index of the required match information.
    */
    bool_match_info *nth_match(int n) const {return matches_ + n; };

    /**
    * @nodoc
    * This class enables the caller to supply information on known coincidences
    * between entities in the blank and tool bodies.  This information is presented
    * as an array of objects of the class @href bool_match_info, each of which
    * specifies a particular pair of matching entities and, optionally, useful
    * characteristics of the match.  See the documentation for @href bool_match_info.
    * The contents of the given array of <tt>bool_match_info</tt> objects is
    * copied and stored in the <tt>BoolOptions</tt> object.
    * @param n
    * Number of elements in the array.
    * @param 
    * Array of bool_match_info objects.
    */
    void set_match_array(int n, bool_match_info *info);
    void gme_set_match_array(int n, bool_match_info *info);

    /**
    * Sets the value of the near coincidence fuzz. The default value is 0.
    */
    void set_near_coincidence_fuzz(double fuzz);
    void gme_set_near_coincidence_fuzz(double fuzz);

    /**
    * Returns the value of the near coincidence fuzz.
    */
    double near_coincidence_fuzz() const;
    double gme_near_coincidence_fuzz() const;

    /**
    * @nodoc
    * Through this class, the caller requests that the match
    * information be verified.  The match information is verified by using the
    * intersectors to check coincidences.  If the verification flag is set and if
    * a pair of entities is found not to be coincident to the given tolerance,
    * then the Boolean operation fails with an error.  By default, the
    * verification flag is unset, and should only be set for debugging purposes.
    */
    void set_verify_matches_flag() { verify_matches_flag_ = TRUE; };

    /**
    * @nodoc
    * Unsets the verification flag.
    */
    void unset_verify_matches_flag() { verify_matches_flag_ = FALSE; };

    /**
    * @nodoc
    * Returns TRUE if the verification flag for matched Booleans is set, FALSE
    * otherwise.
    */
    logical verify_matches() const { return verify_matches_flag_; };

    /**
    * Sets the merge type. 
	* See the documentation for @href bool_merge_type. The default value is <tt>bool_merge_unset</tt>. 
    */
    void set_merge_type(bool_merge_type mt) { merge_option_ = mt; };

    /**
    * Queries the merge type. See the documentation for  @href bool_merge_type.
    */
    bool_merge_type get_merge_type() { return merge_option_; };

    /**
    * Retrieves a list of merge candidates left over from a Boolean carried out with a delayed merge.
    */
    void list_merge_candidates(ENTITY_LIST &candidates);
    void gme_list_merge_candidates(ENTITY_LIST &candidates);

    /**
    * @nodoc
    */
    void note_merge_candidates(ENTITY_LIST &candidates);
    void gme_note_merge_candidates(ENTITY_LIST &candidates);

    /**
    * Removes any items from the options which were used as outputs from a previous Boolean operation.
    */
    void reset_outputs();
    void gme_reset_outputs();

    /**
    * @nodoc
    * Sets the value of the clash info.
    */
    void set_clash_info(body_clash_type newclash);
    void gme_set_clash_info(body_clash_type newclash);

    /**
    * Returns the value of the clash info.
    */
    body_clash_type get_clash_info() const { return clash_info_ ; };

	/**
	* Return the current status of the track_entities flag.
    */
    logical             get_track_entities() const;
    logical             gme_get_track_entities() const;

	/**
    * Set the track_entities flag. The default value is <tt>FALSE</tt>.
    */
    void                set_track_entities(logical te);
    void                gme_set_track_entities(logical te);

	/**
    * Return the list of the imprinted entities from a body specified by the enum @href bool_body_type.
    */
    void                get_imprinted_entities( bool_body_type body_type, ENTITY_LIST& edges);
    void                gme_get_imprinted_entities( bool_body_type body_type, ENTITY_LIST& edges);
	
    /**
    * Return the list of associated entities with a given entity for an imprinting operation.
    */
    void                get_associated_entities ( ENTITY *ent, ENTITY_LIST& assoc_ents);
    void                gme_get_associated_entities ( ENTITY *ent, ENTITY_LIST& assoc_ents);

	/**
	 * Enumerates various performance optimization hints that can be supplied to a Boolean operation.
	 * <br>
	 * <b>Role:</b> The @href optimization_hints enum is used in @href BoolOptions to a Boolean operation
	 * to improve its performance.
	 * @param OH_DEFAULT
	 * No hints provided. Default performance optimization is applied.
	 * @param OH_TOPO_COMPLEX
	 * Indicates that the topological complexity of the inputs is much higher than the geometrical complexity.
	 * For example, the blank and tool have only analytic geometry but the number of faces would be
	 * in order of 10000.
	 * @param OH_NON_TOLERANT
	 * Indicates that the input bodies have no tolerant entities in them. If near coincidence fuzz is supplied
	 * then this hint is ignored as Booleans would need to deal with tolerances during the operation.
	 */

	enum optimization_hints
	{
		OH_DEFAULT = 0,
		OH_TOPO_COMPLEX = 1,
		OH_NON_TOLERANT = 2
	};

	/**
	 * Sets combination of performance optimization hints for Boolean operation. The value 
	 * to be passed is either single or a combination of members of @href optimization_hints.
	 * Multiple hints must be combined by doing a bitwise-OR. Example: OH_TOPO_COMPLEX | OH_NON_TOLERANT.
	 * If inappropriate hints are supplied then the behavior of Boolean operation is unpredictable. 
	 */

	void set_optimization_hints( unsigned hints );
	void gme_set_optimization_hints( unsigned hints );

	/**
	 * @nodoc
	 */
	// For internal use only. Returns the combination of performance optimization hints specified by user.

	unsigned get_optimization_hints() const;
	unsigned gme_get_optimization_hints() const;

	/**
	* Enumerates certain design constraints that may be detected and used by the Boolean operation. Knowledge of these 
	* design constraints can potentially help Booleans succeed or produce better output quality.<br><br>
	* <b>Role:</b> The @href design_constraints enum is used in @href BoolOptions to hint the Boolean operation
	* to improve its output based on the design constraints detected.
	* @param DC_DEFAULT
	* This is the default behavior. Indicates that the algorithm will detect and attempt resolution only for certain scenarios. 
	* In such scenarios, failure is evident because of geometrical complexities, but an elegant solution is possible using design constraint information.
	* @param DC_TANGENCY_BLEND
	* Indicates that the Boolean operation will do implicit detection of blends wherever needed (locally). 
	* This helps the boolean algorithm to take better decisions in regions where blend interactions cause near-tangency.
	* This augments the overall success rate and output quality, although more computation time may be required for detection of blends.
	*/

	enum design_constraints
	{
		DC_DEFAULT = 0,
		DC_TANGENCY_BLEND = 1,		
	};

	/**
	* Sets a combination of @href design_constraints that the Boolean operation may detect and use
	* for processing. The value to be passed is either a single or a combination of members of 
	* the enum @href design_constraints. Multiple hints must be combined by doing a bitwise-OR.
	*/

	void set_detect_design_constraints( unsigned constraints );
	void gme_set_detect_design_constraints( unsigned constraints );

	/**
	* @nodoc
	*/
	// For internal use only. Returns the combination of design_constraints specified by user.

	unsigned get_design_constraints() const;
	unsigned gme_get_design_constraints() const;

	/**
	* @nodoc
	*/
	// For internal use only. Enumerates various actions for a Boolean operation when a crumble is encountered.
	
	enum crumble_action
	{
		CA_DEFAULT = 0,
		CA_LEGACY,
		CA_IGNORE,
		CA_COALESCE,
		CA_SMART
	};

	/**
	* @nodoc
	*/
	// For internal use only. Sets a preferred action when Boolean operation encounters crumble
	// situation such as BODY_VERTEX_CRUMBLE.
	
	void set_preferred_crumble_action( crumble_action ca );
    void gme_set_preferred_crumble_action( crumble_action ca );

	/**
	* @nodoc
	*/
	// For internal use only.
	
	crumble_action get_preferred_crumble_action() const;
    crumble_action gme_get_preferred_crumble_action() const;

	/**
    * @nodoc
    */
    void set_imprint_association( imprint_assoc_data *&assoc_ptr );
    void gme_set_imprint_association( imprint_assoc_data *&assoc_ptr );
	
	/**
    * @nodoc
    * Always returns FALSE.
    * @param in_bool_opt
    * BoolOptions object.
    */
    logical operator== (BoolOptions const& in_bool_opt) const
    {
        SPAUNUSED(in_bool_opt);
        return FALSE;
    };
    /**
    * @nodoc
    * Always returns TRUE.
    * @param in_bool_opt
    * BoolOptions object.
    */
    logical operator!= (BoolOptions const& in_bool_opt) const
    {
        SPAUNUSED(in_bool_opt);
        return TRUE;
    };

	/**
	* @nodoc
	*/
	int type() const override
	{
		return id();
	}

	/**
	* @nodoc
	*/
	static int id();
    static int gme_id();

private:

    int               num_matches_;
    logical           verify_matches_flag_;
    bool_merge_type   merge_option_;
    bool_match_info  *matches_;
    ENTITY_LIST      *delayed_merge_entities_;
    double            near_coincidence_fuzz_ ;
    body_clash_type   clash_info_;
	logical           track_edges_;    // controls creation of tracked edges list
    imprint_assoc_data *assoc_ptr_;
	unsigned		  perf_optm_hints_;
	unsigned		  design_constraints_;
	
	// For internal use only.

	crumble_action ca_pref_;

	/**
     * @nodoc
     */
	friend class BoolOptions_internal;
};

/*! @} */

/**
* @nodoc
*/
//  Functions to print equivalent strings for enums for use in debug output.
//  tbrv
DECL_BOOL const char* bool_interaction_type_text(bool_interaction_type);
DECL_BOOL const char* gme_bool_interaction_type_text(bool_interaction_type);
/**
* @nodoc
*/
DECL_BOOL const char* bool_overlap_type_text(bool_overlap_type);
DECL_BOOL const char* gme_bool_overlap_type_text(bool_overlap_type);

/**
* @nodoc
*/
DECL_BOOL const char* bool_merge_type_text(bool_merge_type bmt);
DECL_BOOL const char* gme_bool_merge_type_text(bool_merge_type bmt);


#endif
