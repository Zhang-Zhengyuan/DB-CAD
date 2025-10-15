/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef PATTERN_HOLDER_HXX
#define PATTERN_HOLDER_HXX

/**
* @file pattern_holder.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISPATTERNS
 *
 * @{
 */
#include "dcl_kern.h"
#include "logical.h"

#include "vlists.hxx"
#include "position.hxx"
#include "transf.hxx"

#include "api.hxx"
#include "lists.hxx"
#include "pattern_enum.hxx"

class law;
class SPAtransf;
class ENTITY;
class pattern_datum;
class pointer_map;
class law_data;
class APATTERN;
class ENTITY_LIST;
class VOID_LIST;
class restore_data;
class pattern;
class ATTRIB;
class ATTRIB_PAT_HOLDER;
class FACE;

// This enum is to specify the reason for which attributes and/or
// tags on pattern elements are being cached [used in calls to
// pattern_holder::update_cache()]

/*
// Documentation : To be revisited
*/
/**
 * @nodoc
 */
enum PAT_CACHE_TYPE
{
    PAT_CACHE_REFRESH,  // The pattern is being refreshed
    PAT_CACHE_SAVE      // The body is being saved
};


/*
// Documentation : To be revisited
*/
/**
 * @nodoc
 */
class DECL_KERN pattern_holder : public ACIS_OBJECT
{
private:
	pattern*    the_pattern;
	ENTITY_LIST other_ents;
	mutable int use_count;
	mutable int live_owner_count;
    // For attrib_cache_size, a value of -1 means "there is no cache,
    // but do not copy the attributes."  This signal is needed when the
    // pattern attributes are in the process of being restored, when we
    // don't want the attributes of the seed entity copied.  A value of
    // 0 means that "there is no cache, and it's OK to copy."
    int         attrib_cache_size;
    int         tag_cache_size;
    ATTRIB**    attrib_cache;
    int*        tag_cache;
    // The following constructor is to be used only during restore,
    // when the argument is actually an index rather than a pointer
    pattern_holder(APATTERN* in_apat);
    logical unsavable() const;
    logical incompatible() const;
    void process_cache();
    void restore_cache(logical keep_tags);
    void update_cache(PAT_CACHE_TYPE reason, logical roll = FALSE);
    void empty_cache();
    void empty_attrib_cache();
    void empty_tag_cache();
    void lose_cache();
    void lose_attrib_cache();
    void remove_pattern_annos();
    void deep_copy_attrib_cache(const pattern_holder* from_ph);
    void split_attrib_cache(ATTRIB_PAT_HOLDER* new_aph);
    logical remove_element(int index);
    logical restore_ent_to_list(ENTITY* ent, int old_idx, logical roll = TRUE);
    logical remove_ent_from_list(ENTITY* ent, logical roll = TRUE);

    // Declare these as friends so they can use the functions above
    friend class BODY;
    friend class LUMP;
    friend class SHELL;
    friend class FACE;
    friend class LOOP;
    friend class WIRE;
    friend class COEDGE;
    friend class EDGE;
    friend class VERTEX;
    friend class SURFACE;
    friend class CURVE;
    friend class PCURVE;
    friend class APOINT;
    friend class ENTITY;
    friend class ATTRIB;
    friend class ATTRIB_PAT_HOLDER;
    friend DECL_KERN ENTITY* copy_pattern_ent(ENTITY* entity);
	friend pattern_holder* restore_pattern_holder( ENTITY* ent, int pat_idx, APATTERN* apat_idx );

public:

	pattern_holder(
        pattern*    in_pat,         // Pattern that it holds
        ENTITY*     in_ent,         // Seed entity to which
                                    // the object is attached
        logical     do_cache_index  // Set the cached pattern
            = TRUE                  // index of in_ent if TRUE
        );

	virtual ~pattern_holder();

	void add() const;
	void remove();

	void add_live_owner() const;
	void remove_live_owner();

    // Do not document
    void roll_notify(
        BULLETIN_TYPE type,
        ENTITY* notify_ent,
        ENTITY* copy_ent = NULL);

    // Returns the index (in other_ents) of the specified entity, or
    // -1 if the entity isn't found
    int find_entity(const ENTITY* ent) const;

    // do not doc
    const ENTITY_LIST& get_list() const;

    // do not doc
    void split(const ENTITY_LIST& keep_ents, const ENTITY_LIST& split_ents);

    // Returns a pointer to the pattern entity indexed
    // by index, or NULL if index is invalid.  The value
    // of from_list indicates whether the entity was already
    // in existence (from_list=TRUE) or whether it had to
    // be newly created (from_list=FALSE).  In the latter case,
    // out_trans gives the transform needed to produce the
    // corresponding pattern element from the previous pattern
    // element.
	ENTITY* get_entity(
        int             index,      // pattern index indicating
                                    // the entity to be returned
        SPAtransf&   out_trans,  // the corresponding pattern
                                    // transformation
        logical&        from_list   // flag specifying whether or not
                                    // the entity was already in existence
                                    // at the time of the call
        );

	ENTITY* get_entity_no_create(
        int             index
        );

    // Returns a pointer to the pattern entity following in_ent
    // in the pattern, or NULL if in_ent is not part of the pattern
    // or if from_list is input as TRUE and the entity following
    // in_ent is not yet part of the list.
    // The return value of from_list indicates whether the entity was
    // already in existence (from_list=TRUE) or whether it had to
    // be newly created (from_list=FALSE).  In the latter case,
    // out_trans gives the transform needed to produce the
    // corresponding pattern element from in_ent.
	ENTITY* get_next_entity(
        const ENTITY*   in_ent,     // pattern entity whose "next" is
                                    // to be returned
		SPAtransf&   out_trans,  // the corresponding pattern
                                    // transformation
        logical&        from_list   // on input, indicates whether the
                                    // entity following in_ent can be
                                    // created if it is not already in
                                    // existence; on output, flag specifies
                                    // whether or not the entity was already
                                    // in existence at the time of the call
        );

	ENTITY* get_next_entity_no_create(
        const ENTITY*   in_ent
        );

    // Returns the pattern index associated with in_ent, or -1 if in_ent
    // is not part of the pattern.
	int get_index(
        const ENTITY* in_ent        // pattern entity whose index
                                    // is to be returned
        );

    // Adds in_ent to the end of the list of entities associated
    // with this object's pattern.
	void set_next(
        ENTITY* in_ent,         // Entity to be added
        logical do_cache_index  // Set the cached pattern index
            = TRUE              // of in_ent if TRUE
        );

	pattern* get_pattern();

    // Returns TRUE if every pattern member has been created (and
    // has therefore been added to other_ents) or if the_pattern
    // is NULL
    logical is_pattern_expanded() const;

    // Removes in_ent from the list of pattern entities held
    // by the object.  Returns FALSE if the entity is not
    // found
    logical remove_entity_from_list(
        ENTITY* in_ent      // entity to be removed
        );

    // Restores the entity in_ent to the list of patterns held
    // by the object, putting it in the location specified by
    // index.  Returns FALSE if the index is invalid.
    logical restore_entity_to_list(
        ENTITY* in_ent,     // entity to be restored
        int index           // location in the list at
                            // which to restore in_ent
        );

    // Removes the base lump(s) of the pattern from in_body.
    // ("Base lumps" refers to the lumps belonging to pattern
    // element indexed by first_present.)  Returns the total
    // number of lumps removed from the pattern.
    int remove_base_lumps(
        BODY*   in_body,        // body to be examined
        int     first_present   // index of the first element
                                // present in the pattern
        );

    // Returns TRUE if in_pat points to the same pattern object
    // held by this object.
	logical same(
        pattern* in_pat     // pattern object to be compared
        );

    // Both overloads compare the list of other_ents to the input list
    // and return TRUE if every member of other_ents is in the list
    logical compare_list(
        const ENTITY**  ents,
        int             num_ents
        ) const;
    logical compare_list(const ENTITY_LIST& ent_list) const;
    // Compares the list of other_ents to the input lists and returns
    // the index of the list that contains every member of other_ents,
    // or -1 if there is no such list
    int compare_lists(const ENTITY_LIST* ent_lists, int num_lists) const;

    // Saves object to list
    void save(
        ENTITY_LIST& list,  // list to which this object is saved
        logical former      // is this a former pattern holder instead
                            // of an active one?
        );

    // Restores the object by reading its pattern from array,
    // assuming that the_pattern is currently holding the index
    // specifying where the corresponding APATTERN entity is to
    // be found in array.  The entity ent specifies the seed
    // pattern entity of the object.
    void restore(
        ENTITY* array[],    // array in which the object's
                            // APATTERN is to be found
        ENTITY* ent         // seed pattern entity
        );

	// Checks the the validity of the attribute cache.
	// Returns TRUE if the attrib_cache is okay.
	logical attrib_cache_is_valid();
};

/** @} */
#endif // PATTERN_HOLDER_HXX


