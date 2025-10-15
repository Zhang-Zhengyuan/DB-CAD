/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( PROJ_OPTIONS_CLASS )
#define PROJ_OPTIONS_CLASS

#include "dcl_bool.h"   // DECL_BOOL
#include "mmgr.hxx"     // ACIS_OBJECT
#include "lists.hxx"	// ENTITY_LIST
#include "vlists.hxx"   // VOID_LIST

class ENTITY;
class ENTITY_LIST;
class BODY;
class self_ints_behavior;
struct imprint_assoc_data;

/**
 * \defgroup BOOLPROJOPTSAPI Projection Options
 * \ingroup BOOLOPTIONSAPI
 *
 */

/**
* @file proj_opts.hxx
* @CAA2Level L1
* @CAA2Usage U1
*/

/*! \addtogroup BOOLPROJOPTSAPI
 *
 *  @{
 */

/** 
 * Enumeration to specify imprint type in project operations
 * @param IMPRINT_ALL
 * Imprint the projected wire.
 * @param IMPRINT_NONE
 * Do not imprint projected wire.
 */

enum proj_imprint_type {
	IMPRINT_ALL,
	IMPRINT_NONE
};

/**
 * Enumeration to specify the body referred to in get_projected_entities
 * @param PROJ_SOURCE_WIRE
 * The wire body which is being projected.
 * @param PROJ_TARGET_BODY
 * The body which the wire is being projected onto.
 */

enum proj_body_type {
	PROJ_SOURCE_WIRE,
	PROJ_TARGET_BODY
};

/**
 * Class to hold information and options for a projection operation.
 * 
 * <b>Role:</b> A @href project_options object supplies information to a given
 * projection operation.
 *
 * This class can be used to specify whether projected entities are tracked.
 * If the track_entities flag is set then a list of the projected edges and vertices 
 * is retained and can be returned to the caller after the projection has been 
 * completed. This is done by calling the project_options method 
 * list_projected_entities(proj_body_type, ENTITY_LIST &). The enumerated type
 * @href proj_body_type controls whether the entities to be returned are the progenitor
 * entities on the wire body or the projected entities on the target body. In either case
 * the associated entities can be returned by calling the project_options method
 * list_associated_entities(ENTITY *, ENTITY_LIST&).
 *
 * The class can be used to control the tolerance used during the projection.
 *
 * The class can be used to supply a maximum distance to use for projection.
 *
 * The class can also be used to set an behavior to resolve curve self-intersection
 * in the projected edge and improper intersection between multiple projected edges.
 * To set this behavior use @href set_self_ints_behavior() by supplying combination
 * of enumerated type @href project_options::self_ints_granularity and enumerated type 
 * @href project_options::self_ints_action
 *
 * An enumerated type @href proj_imprint_type is present to specify whether to perform imprint of 
 * the projected wire onto the body.
 * If the imprint is not done then the projected wire body can be returned by calling
 * the project_options method get_projected_wire_body().
 *
 * Additional control can be obtained over which edges are projected onto which faces
 * by providing "seed pairs". The methods add_seed_pair(), clear_seed_pairs() control this
 * behaviour.
 *
 * Instead of using the entire body the projection operation can use a list of faces
 * from the target body. The methods set_target_faces(), clear_target_faces() control this behaviour.
 */
class DECL_BOOL project_options : public ACIS_OBJECT
{
    double              _tolerance;      // controls creation of tolerant entities
	double				_max_distance;		 // controls the maximum distance between edge and its projection
    logical             _track_edges;    // controls creation of tracked edges list
    imprint_assoc_data *_assoc_ptr;			// Association object
    self_ints_behavior *_sib; 			// Notes the self intersection behavior to be follwed.
    proj_imprint_type	_imprint_type;   // imprint action after projection
    BODY *              _wire_body;      // wire body returned if _imprint_type is IMPRINT_NONE

	VOID_LIST			_seed_pairs;	 // pointers to pairs of entities
	ENTITY_LIST         _target_faces;      // target body face list to be considered in projections
	logical             _target_faces_set; // was the list set by set_target_faces()?

    void                init_variables();

public:
    /**
    * Default constructor.
    */
    project_options();
    /**
    * Destructor.
    */
    virtual ~project_options();
    /**
    * Return the current tolerance value.
    */
    double              get_tolerance() const;
	/**
    * Set the tolerance value.
    */
    void                set_tolerance(double);
	/**
	* Return the current maximum distance value.
	*/
	double				get_max_distance() const;
	/**
	* Set the maximum distance value.
	*/
	void				set_max_distance(double);
    /**
    * Return the current status of the track_entities flag.
    */
    logical             get_track_entities() const;
	/**
    * Set the track_entities flag.
    */
    void                set_track_entities(logical);
	/**
    * Return the list of projected entities.
    */
    void                list_projected_entities(proj_body_type, ENTITY_LIST&);
	/**
	* Return the list of entities associated with given entity.
	*/
	void				list_associated_entities(ENTITY *, ENTITY_LIST&);
    
	/**
	* Enumerates the different actions for the self-intersection checks performed during
	* Projection operation.
	* <br/>
	* <b>Role:</b> The enum @href self_ints_action is used in @href project_options to 
	* a Projection operation to improve the output quality.
	* @param SIA_DEFAULT
	* This flag represents the default action, decided at run-time. 
	* Please refer Technical Article <i>Projection</i> for more details.
	* @param SIA_IGNORE
	* No self-intersection checks are performed. The projection output may check bad.
	* @param SIA_FAIL
	* Self-intersection checks are performed for the @href self_ints_granularity set. 
	* Projection operation will fail, if found bad intersections.
	* @param SIA_RESOLVE
	* Self-intersection checks are performed for the @href self_ints_granularity set. 
	* Projection operation will try to resolve bad intersections. If unable to resolve 
	* then the operation will fail.
	*/

	enum self_ints_action
	{
		SIA_DEFAULT = 0,
		SIA_IGNORE,
		SIA_FAIL,
		SIA_RESOLVE
	};

	/**
	* Enumerates the different granularity levels for the self-intersection checks performed during
	* Projection operation.
	* <br/>
	* <b>Role:</b> The enum @href self_ints_granularity is used in @href project_options to 
	* a Projection operation to improve the output quality.
	* @param SIG_WITHIN_EDGE
	* Checks if the projected edge self-intersects. If found self-intersection, then 
	* @href self_ints_action will be applied.
	* @param SIG_BETWEEN_EDGES
	* Checks if the projected edges create improper intersections. if found such intersections
	* then @href self_ints_action will be applied.
	*/

	enum self_ints_granularity
	{
		SIG_WITHIN_EDGE = 0,
		SIG_BETWEEN_EDGES
	};

	/**
	 * Sets map of @href self_ints_granularity and @href self_ints_action. 
	 * For each @href self_ints_granularity user can supply the action to be applied.
	 * If user does not supply the actions, behavior will be default. [ Please refer 
	 * Technical Article <i>Projection</i> for default behavior. ]<br/>
	 * SIG_WITHIN_EDGE, SIA_DEFAULT<br/>
	 * SIG_BETWEEN_EDGES, SIA_DEFAULT<br/>
	 * </br><br/>
	 * User can override default settings. All possible values and their meaning is described below.<br/>
	 * <br/>
	 * SIG_WITHIN_EDGE, SIA_IGNORE    : No curve self intersection checks performed.<br/>
     * SIG_WITHIN_EDGE, SIA_FAIL      : Check for curve self-intersection. Fail if found.<br/>
	 * SIG_WITHIN_EDGE, SIA_RESOLVE   : Check and resolve curve self-intersections. Fail if unresolvable.<br/>
	 * <br/>
	 * SIG_BETWEEN_EDGES, SIA_IGNORE  : No Improper intersection checks performed. <br/>
	 * SIG_BETWEEN_EDGES, SIA_FAIL    : Check for improper intersections. Fail if found. <br/>
	 * SIG_BETWEEN_EDGES, SIA_RESOLVE : Check and Resolve improper intersections. Fail if unresolvable.<br/>
	 */

	void set_self_ints_behavior( self_ints_granularity sig, self_ints_action sia );
	
	/**
	* <b>Obsolete:</b> use set_self_ints_behavior() instead.
	* <br/>
    * Switch on self-intersection checking. This is old way of setting, which is equivalent to following.
	* <br/> When turned ON : set_self_ints_behavior( SIG_WITHIN_EDGE, SIA_FAIL );
	* <br/> When turned OFF : set_self_ints_behavior( SIG_WITHIN_EDGE, SIA_IGNORE );
    */
    
	void set_check_selfints(logical);
    
	/**
    * Return the current status of the imprint_type.
    */
    proj_imprint_type	get_imprint_type() const;
    /**
    * Set the imprint_type.
    */
    void                set_imprint_type(proj_imprint_type);
    /**
    * Get the pointer to projected wire body.
    */
    BODY *              get_projected_wire_body();
    /**
    * Add a pair of seed entities, either of them may be NULL
    */
	void				add_seed_pair(ENTITY *ent0, ENTITY *ent1);
    /**
    * Clear the list of seed entities.
    */
    void                clear_seed_pairs();
    /**
    * Set a list of faces on the target body to be used in projections.
    */
	void				set_target_faces(ENTITY_LIST &);
    /**
    * Clear the list of target body faces to be used in projections.
    */
    void                clear_target_faces();
	/**
    * Removes any items from the options which were used as outputs from a previous projection operation.
    */
    void                reset_outputs();
	/**
	 * @nodoc
	 */
	// For internal use only. Returns the combination of self_ints_bhavior specified by user.
	self_ints_behavior const* get_sib() const;
    /**
    * @nodoc
    * Get the pointer to seed pairs
    */
    VOID_LIST *			get_seed_pairs();
    /**
    * @nodoc
    * Get the pointer to target body face list
    */
    ENTITY_LIST *		get_target_faces();
    /**
    * @nodoc
    * Was target body face list initialized?
    */
    logical				target_faces_set();
    /**
    * @nodoc
    * Set the pointer to projected wire body.
    */
    void                set_projected_wire_body(BODY *wire_body);
    /**
    * @nodoc
    */
    void                set_association_data( imprint_assoc_data *&assoc_ptr );
	/**
    * @nodoc
    * Always returns FALSE.
    */
    logical operator== (project_options const&) const
    {
        return FALSE;
    };
    /**
    * @nodoc
    * Always returns TRUE.
    */
    logical operator!= (project_options const& ) const
    {
        return TRUE;
    };
};

/**
* @nodoc
* If incorrect self-intersection behavior string is passed then return false else
* return true and sib_val can be used.
*/
DECL_BOOL bool 
read_self_int_behavior( const char *sig_str, const char *sia_str,
						project_options::self_ints_granularity &sig_ref, 
						project_options::self_ints_action &sia_ref );

/**
* @nodoc
*/
DECL_BOOL void 
write_self_int_behavior( project_options const *proj_opts, 
						char *sibs_str );

/*! @} */

#endif
