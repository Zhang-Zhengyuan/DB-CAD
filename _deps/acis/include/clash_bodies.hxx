/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header defining data structures for body clash functionality
/*******************************************************************/

#if !defined( CLASH_BODIES_HXX )
#define CLASH_BODIES_HXX

#include "dcl_query.h"
#include "mmgr.hxx"
#include "queryapi.hxx"
#include <vector>

class ENTITY;
class BODY;
class n_body_clash_results_impl;

/**
* @file clash_bodies.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

class body_clash_result;

/** \addtogroup INTRAPICLASH
 *  \brief Declared at <clash_bodies.hxx>
 *  @{
 */

/**
 * This class holds the result of clash-testing a pair of <tt>EDGE</tt> or
 * <tt>FACE</tt> entities.
 * <br>
 * <b>Role:</b> When <tt>api_clash_bodies</tt> is called in classify-sub-entities
 * mode, a list of <tt>entity_clash_pair</tt> objects is part of the result data
 * returned in the <tt>body_clash_result</tt>. The information contained in each
 * instance is the pair of entities under consideration, and the manner in which
 * they clash. Under normal usage, users do not need to construct or destroy
 * objects of this class - they are managed by the <tt>body_clash_result</tt>
 * class.
 */
class DECL_QUERY entity_clash_pair : public ACIS_OBJECT {

	/**
     * @nodoc
	 */
	entity_clash_relation a_rel;

	/**
     * @nodoc
	 */
    entity_clash_relation b_rel;


protected:
	friend class body_clash_result;	

	/**
     * @nodoc
	 */
	ENTITY* a;

	/**
     * @nodoc
	 */
	ENTITY* b;

	/**
     * @nodoc
	 */
	entity_clash_type type;

	/**
     * @nodoc
	 */
	entity_clash_pair* next_pair;

public:
	
	/**
	 * @nodoc
	 */
	entity_clash_pair(ENTITY* ea,ENTITY* eb,entity_clash_type t, entity_clash_pair* n = 0)
		: a(ea),b(eb),type(t),next_pair(n) {}

	/**
	 * @nodoc
	 */
	entity_clash_pair(ENTITY* ea,ENTITY* eb,entity_clash_type t,
					  entity_clash_relation ar, entity_clash_relation br,
					  entity_clash_pair* n = 0)
		: a(ea), b(eb), type(t), a_rel(ar), b_rel(br), next_pair(n) {}

	/**
	 * @nodoc
	 */
	~entity_clash_pair();

    /**
	 * Returns the classification of how this pair of entities clash.
	 */
	entity_clash_type clash_type() const { return type; }

	/**
     * Returns the first entity of the pair.
	 */
	ENTITY* entity_a() const { return a; }

	/**
     * Returns the second entity of the pair.
	 */
	ENTITY* entity_b() const { return b; }

	/**
     * Returns the next <tt>entity_clash_pair</tt> object in this list.
	 */
	const entity_clash_pair* next() const { return next_pair; }

	/**
	 * @nodoc Return the clash relations (for internal use)
	 */
	entity_clash_relation clash_rel_a() const { return a_rel; }

	/**
	 * @nodoc Return the clash relations (for internal use)
	 */
	entity_clash_relation clash_rel_b() const { return b_rel; }

	/**
	 * @nodoc (for internal use only)
	 */
	void update_info(entity_clash_relation _a_rel, entity_clash_relation _b_rel, entity_clash_type _type)
	{
		a_rel = _a_rel; b_rel = _b_rel; type = _type;
	}
	
	/**
	* @nodoc (for internal use only)
	*/
	void swap_info();
};

/** 
 * Enumeration of the different ways in which a pair of <tt>BODY</tt> entities
 * can be classified as a result of performing a clash detection test.
 * @param CLASH_UNKNOWN
 * Whether the bodies clash has not been determined. This is the value used
 * when a <tt>body_clash_result</tt> object is initialised. A successful call to
 * <tt>api_clash_bodies</tt> will never set this as the clash type.
 * @param CLASH_NONE
 * The bodies do not clash at all.
 * @param CLASH_UNCLASSIFIED
 * The bodies clash, but more detail on how they clash is unavailable. This
 * value is set when a successful call to <tt>api_clash_bodies</tt> in
 * existence-only mode detects that the bodies do in fact clash.
 * @param CLASH_CONTAINED
 * One of the bodies is wholly contained within the other body. There is no
 * contact between the boundaries (faces, edges and vertices) of the bodies.
 * @param CLASH_CONTAINED_ABUTS
 * One of the bodies is wholly contained within the other body, but the bodies
 * do have some boundaries (faces, edges and/or vertices) which touch.
 * @param CLASH_ABUTS
 * The bodies do not share volume (i.e. their intersection is empty), but they
 * do have some boundaries (faces, edges and/or vertices) which touch.
 * @param CLASH_COINCIDENT
 * The bodies are entirely coincident.
 * @param CLASH_INTERLOCK
 * The boundaries of the bodies interpenetrate. There are regions of each body
 * that are inside and outside the other.
 */
enum body_clash_type {
	CLASH_UNKNOWN,
	CLASH_NONE,
	CLASH_UNCLASSIFIED,
	CLASH_CONTAINED,
	CLASH_CONTAINED_ABUTS,
	CLASH_ABUTS,
	CLASH_COINCIDENT,
	CLASH_INTERLOCK
};

/** 
 * Data structure used to encapsulate the details of how two bodies clash.
 * <br>
 * <b>Role:</b>This class holds all of the details of the way in which two
 * bodies are found to clash by <tt>api_clash_bodies</tt>.
 */
class DECL_QUERY body_clash_result : public ACIS_OBJECT {
private:

	body_clash_type type;
	BODY* body_containing_other;
	entity_clash_pair* clashing_entities;
	bool a_in, a_out, b_in, b_out;

public:

	/**
	 * Construct an unset instance ready to be passed to <tt>api_clash_bodies</tt>
     * The instance is initialised such that the <tt>clash_type()</tt> query will
     * return <tt>CLASH_UNKNOWN</tt>, and the <tt>containing_body()</tt> and 
     * <tt>first_entity_clash_pair</tt> queries will return NULL.
     */
	body_clash_result()
		: type(CLASH_UNKNOWN),body_containing_other(0),clashing_entities(0),
	      a_in(false), a_out(false), b_in(false), b_out(false){}

	/**
	 * The destructor tidies up all of the data held by the instance. Any
     * <tt>entity_clash_pair</tt> objects in the instance will be destroyed.
	 */
	~body_clash_result();

	/**
	 * Resets the data of the instance to its initial values. Immediately after
     * calling this method on an instance of this class, the <tt>clash_type()</tt>
     * query will return <tt>CLASH_UNKNOWN</tt>, and the <tt>containing_body()</tt>
     * and  <tt>first_entity_clash_pair</tt> queries will return NULL.
	 */
	void reset();

	/**
     * Returns the classification of how the bodies clash
	 */
	body_clash_type clash_type() const
	    { return type; }

	/**
     * Returns the body that contains the other. This will return NULL
	 * unless the <tt>clash_type()</tt> is <tt>CLASH_CONTAINED</tt> or
     * <tt>CLASH_CONTAINED_ABUTS</tt>, in which case it will return a pointer
     * to whichever of the bodies has been determined to contain the other
     * under test.
	 */
	BODY* containing_body() const
	    { return body_containing_other; }

	/**
     * Returns the second entity of the pair.
	 */
	const entity_clash_pair* first_entity_clash_pair() const
	    { return clashing_entities; }

	/**
	 * @nodoc
	 */
	void set_clash_type(body_clash_type type);

	/**
	 * @nodoc
	 */
	void set_containing_body(BODY* body);

	/**
	 * @nodoc
	 */
	void add_entity_clash_pair(entity_clash_pair* ep);
};

/**
 * Data structure used to encapsulate the details of all body clashes that exist among a collection
 * of bodies and the manner in which the individual pairs of bodies clash. This results object is
 * populated by @href api_n_body_clash, which determines the clashing bodies from an input list.
 * <br>
 * <b>Role:</b>This class holds all of the details of the bodies that
 * are found to clash by @href api_n_body_clash. For each pair of bodies that clash, it stores a 
 * @href body_clash_result object that provides details regarding the clash between those two bodies.
 */
class DECL_QUERY n_body_clash_results : public ACIS_OBJECT {
	/**
	 * @nodoc Stores all the relevant results of the n-body clash, used internally
	 */
	n_body_clash_results_impl* m_results;

	/**
	 * @nodoc Keep track of the clash options that indicate the clash_mode and clash_behaviour
	 */
	body_clash_options clashopts;
public:
	/**
	 * Constructs a result object
	 */
	n_body_clash_results();

	/**
	 * The destructor is public, indicating the user takes ownership of instances.
	 * The destructor tidies up all of the data held by the instance. Any
	 * <tt>entity_clash_pair</tt> objects in the instance will be destroyed.
	 */
	~n_body_clash_results();

	/**
	 * @nodoc Get the underlying internal options object (internal use)
	 */
	n_body_clash_results_impl* get_impl();

	/**
	 * Get the pairs of bodies that are determined to be clashing by @href api_n_body_clash
	 */
	void get_clashing_pairs(std::vector<std::pair<BODY*, BODY*>>& clashing_pairs) const;

	/**
	 * Get the detailed body clash info for the given pair of bodies that are known to be clashing.
	 * The details of the clash are stored in a @href body_clash_result object. The level of detail
	 * stored is dependent on the @href clash_mode specified for the n-body clash operation.
	 * If the given bodies don't constitute a clash, returns <tt>nullptr</tt>.
	 */
	const body_clash_result* get_clash_info(BODY* body1, BODY* body2) const;

	/**
	 * @nodoc Set the clash mode and behaviour for the n-body clash operation (internal use)
	 */
	void set_clash_details(clash_mode mode, clash_behaviour behaviour);

	/**
	 * Get the @href clash_mode used for the n-body clash operation
	 */
	clash_mode get_clash_mode();

	/**
	 * Get the @href clash_behaviour used for the n-body clash operation
	 */
	clash_behaviour get_clash_behaviour();
};

/** @} */

#endif

