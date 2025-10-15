// $Id: annotation_mac.hxx,v 1.31 2001/11/13 17:14:23 ktatters Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//-----------------------------------------------------------------------
// Purpose---
//		Macros to generate some of the boiler plate for the ENTITY data
//	in an annotation and thereby make it easier to define annotations
//-----------------------------------------------------------------------

#ifndef __ANNOTATION_MAC_HXX__
#define __ANNOTATION_MAC_HXX__

#include "eelists.hxx"
#include "face.hxx"
#include "edge.hxx"
#include "vertex.hxx"
#include "point.hxx"
#include "versions.hxx"

// STI ROLL  Includes for DEEP_COPY capability
class pointer_map;

class annotation_descriptor /*: public ACIS_OBJECT aggregate init problem */ {
public:
	enum  {
		in,
		out
	} datum_type;
	const char* display_name;
	logical same_body;
};

// Macros for any extra data in addition to the ents.
#define ANNOTATION_EXTRA_DATA_FUNCTIONS(type) \
	void save_extra() const; \
	void restore_extra(); \
	void copy_extra(type const* from, pointer_map *pm = NULL ); \
	void debug_extra( FILE* fp ) const;

#define ANNOTATION_EXTRA_DEF \
void THIS()::debug_extra( FILE* fp ) const \
{ \
SPAUNUSED(fp) \


#define ANNOTATION_SAVE_EXTRA_DEF \
} \
void THIS()::save_extra() const \
{ \

#define ANNOTATION_RESTORE_EXTRA_DEF \
} \
void THIS()::restore_extra() \
{ \

#define ANNOTATION_COPY_EXTRA_DEF \
} \
void THIS()::copy_extra(THIS() const* from, pointer_map *pm) \
{ \

#define ANNOTATION_COPY_WITH_DEEP_COPY_EXTRA_DEF \
} \
void THIS()::copy_extra(THIS() const* from, pointer_map *pm) \
{ \
SPAUNUSED(from) \
SPAUNUSED(pm) \

#define ANNOTATION_TERMINATE_EXTRA_DEF \
}
// End Macros for any extra data in addition to the ents.

// Use this in the annotation class declaration
#define ANNOTATION_MAP_DECL_NO_ENTS() \
protected: \
	void member_lost_internal(const ENTITY* entity); \
public: \
	virtual const char* member_name(const ENTITY* entity) const; \
	virtual void member_lost(const ENTITY* entity); \
	virtual void inputs(ENTITY_LIST& list, logical no_tags = TRUE) const; \
	virtual void outputs(ENTITY_LIST& list) const; \
	virtual void unhook_members(); \
	virtual void hook_members(); \
	virtual void lose_input_tags(); \
	virtual void lose_lists(); \
	virtual ENTITY*& find_entity_ref_by_name(const char* name, logical& isInput); \
	virtual ENTITY* get_entity_by_name(const char* name); \
	virtual void set_entity_by_name( const char* name, ENTITY* value); \
	virtual logical is_entity_by_name( const char* name, ENTITY* entity);

#define ANNOTATION_MAP_DECL(num) \
protected: \
	LOCAL_CONST int num_ents; \
	LOCAL_CONST annotation_descriptor descriptors[]; \
	ENTITY* ents[num]; \
    logical unhooked_out_is_ee[num]; \
	\
	ANNOTATION_MAP_DECL_NO_ENTS()


#define ANNOTATION_MAP_START() \
	const annotation_descriptor THIS()::descriptors[] = {

#define ANNOTATION_MAP_ENTRY(type, name, same_body) \
	{ annotation_descriptor::type, name, same_body },

#define ANNOTATION_MAP_NO_ENTS() \
void THIS()::member_lost(const ENTITY* entity) { \
	member_lost_internal(entity); \
	if(empty()) \
		lose(); \
} \
void THIS()::member_lost_internal(const ENTITY* entity) { \
	PARENT()::member_lost_internal(entity); \
} \
void THIS()::inputs(ENTITY_LIST& list, logical no_tags) const { \
	PARENT()::inputs(list, no_tags); \
} \
void THIS()::outputs(ENTITY_LIST& list) const { \
	PARENT()::outputs(list); \
} \
const char* THIS()::member_name(const ENTITY* entity) const { \
	return PARENT()::member_name(entity); \
} \
void THIS()::unhook_members() { \
	PARENT()::unhook_members(); \
} \
void THIS()::hook_members() { \
	PARENT()::hook_members(); \
} \
void THIS()::lose_input_tags() { \
	PARENT()::lose_input_tags(); \
} \
void THIS()::lose_lists() { \
	PARENT()::lose_lists(); \
} \
ENTITY*& THIS()::find_entity_ref_by_name(const char* name, logical& isInput) \
{ \
	return PARENT()::find_entity_ref_by_name(name, isInput); \
} \
ENTITY* THIS()::get_entity_by_name(const char* name) { \
	return PARENT()::get_entity_by_name(name); \
} \
void THIS()::set_entity_by_name(const char* name, ENTITY* value) { \
	PARENT()::set_entity_by_name(name, value); \
} \
logical THIS()::is_entity_by_name( const char* name, ENTITY* entity) { \
	return PARENT()::is_entity_by_name(name, entity); \
} \
ENTITY_DEF(not_used) \
	debug_extra(fp); \
SAVE_DEF \
	save_extra(); \
RESTORE_DEF \
	restore_extra(); \
COPY_WITH_DEEP_COPY_DEF \
	/* STI ROLL  Add DEEP COPY capabiltiy. */ \
	copy_extra(from, pm); \
SCAN_DEF \
FIX_POINTER_DEF \
TERMINATE_DEF 

// A more traditional name
#define ANNOTATION_MAP_DEF_NO_ENTS ANNOTATION_MAP_NO_ENTS

#if USE_COUNT_ATTRIB_TAG
#define LOSE_TAG(t) ((ATTRIB_TAG*)t)->remove();
#else
#define LOSE_TAG(t) t->lose();
#endif



#define ANNOTATION_MAP_END() \
};\
const int THIS()::num_ents = sizeof(THIS()::descriptors)/sizeof(THIS()::descriptors[0]); \
void THIS()::member_lost(const ENTITY* entity) { \
	member_lost_hook(entity); \
	member_lost_internal(entity); \
	if(empty()) \
		lose(); \
} \
void THIS()::member_lost_internal(const ENTITY* entity) { \
	PARENT()::member_lost_internal(entity); \
	for(int i = num_ents; i--; ) { \
		member_lost_helper(ents[i], entity); \
	} \
} \
void THIS()::inputs(ENTITY_LIST& list, logical no_tags) const { \
	PARENT()::inputs(list, no_tags); \
	for(int i = num_ents; i--; ) { \
		if(descriptors[i].datum_type == annotation_descriptor::in)  { \
			inputs_helper(ents[i], list, no_tags); \
		} \
	} \
} \
void THIS()::outputs(ENTITY_LIST& list) const { \
	PARENT()::outputs(list); \
	for(int i = num_ents; i--; ) { \
		if(descriptors[i].datum_type == annotation_descriptor::out)  { \
			list.add(ents[i]); \
			if(ents[i] && is_EE_LIST(ents[i])) { \
				EE_LIST* elist = (EE_LIST*) ents[i]; \
				elist->init(); \
				ENTITY* e = elist->next(); \
				while(e) {\
					list.add(e); \
					e = elist->next(); \
				} \
			} \
		} \
	} \
} \
const char* THIS()::member_name(const ENTITY* entity) const { \
	const char* answer = NULL; \
	for(int i = num_ents; i--; ) { \
		/* if the same entity is used in the annotation more than once \
		 * the first instance found is returned.\
		 */ \
		if(contains_this_entity(ents[i], entity)) { \
			answer = descriptors[i].display_name; \
			break; \
		} \
	} \
	return answer ? answer : PARENT()::member_name(entity); \
} \
void THIS()::unhook_members() { \
	/* Handling of ATTRIB_TAG is in ANNOTATION::hook and ANNOTATION::unhook */ \
    if(!members_are_hooked()) \
        return; \
	PARENT()::unhook_members(); \
	for(int i = num_ents; i--; ) { \
        unhooked_out_is_ee[i] = FALSE; \
        if(ents[i] && is_EE_LIST(ents[i])) { \
            if(descriptors[i].datum_type == annotation_descriptor::out) \
                unhooked_out_is_ee[i] = TRUE; \
			EE_LIST* elist = (EE_LIST*) ents[i]; \
			elist->init(); \
			ENTITY* e = elist->next(); \
			while(e) {\
				unhook(e); \
				e = elist->next(); \
			} \
		} \
        else \
            unhook(ents[i]); \
	} \
} \
void THIS()::hook_members() { \
	/* Handling of ATTRIB_TAG is in ANNOTATION::hook and ANNOTATION::unhook */ \
    if(members_are_hooked()) \
        return; \
	PARENT()::hook_members(); \
	for(int i = num_ents; i--; ) { \
		if(ents[i] && is_EE_LIST(ents[i])) { \
			EE_LIST* elist = (EE_LIST*) ents[i]; \
			elist->init(); \
			ENTITY* e = elist->next(); \
			while(e) {\
				hook(e); \
				e = elist->next(); \
			} \
		} \
        else \
            hook(ents[i]); \
	} \
} \
void THIS()::lose_input_tags() { \
	PARENT()::lose_input_tags(); \
	for(int i = num_ents; i--; ) { \
		if(descriptors[i].datum_type == annotation_descriptor::in) { \
			if(is_ATTRIB_TAG(ents[i])) { \
				LOSE_TAG(ents[i]); \
				backup(); \
				ents[i] = NULL; \
			} \
			else if(ents[i] && is_EE_LIST(ents[i])) { \
				EE_LIST* elist = (EE_LIST*) ents[i]; \
				elist->init(); \
				ENTITY* e = elist->next(); \
				while(e) {\
					if(is_ATTRIB_TAG(e)) { \
						elist->remove(e); \
						/* LOSE_TAG(e); done in EE_LIST::remove() */ \
					} \
					e = elist->next(); \
				} \
			} \
		} \
	} \
} \
void THIS()::lose_lists() { \
	PARENT()::lose_lists(); \
	for(int i = num_ents; i--; ) { \
		if(ents[i] && \
           (members_are_hooked() || \
            descriptors[i].datum_type == annotation_descriptor::in || \
            unhooked_out_is_ee[i] == TRUE ) && \
           is_EE_LIST(ents[i])) { \
			ents[i]->lose(); \
		} \
	} \
} \
ENTITY*& THIS()::find_entity_ref_by_name(const char* name, logical& isInput) \
{ \
	for(int i = num_ents; i--; ) { \
		if(strcmp(descriptors[i].display_name, name) == 0) { \
			isInput = descriptors[i].datum_type == annotation_descriptor::in; \
			return ents[i]; \
		} \
	} \
	return PARENT()::find_entity_ref_by_name(name, isInput); \
} \
ENTITY* THIS()::get_entity_by_name(const char* name) { \
	for(int i = num_ents; i--; ) { \
		if(strcmp(descriptors[i].display_name, name) == 0) { \
			return get_actual_entity(ents[i]); \
		} \
	} \
	return PARENT()::get_entity_by_name(name); \
} \
void THIS()::set_entity_by_name(const char* name, ENTITY* value) { \
	for(int i = num_ents; i--; ) { \
		if(strcmp(descriptors[i].display_name, name) == 0) { \
			if(descriptors[i].datum_type == annotation_descriptor::out ) { \
				set_output_entity(ents[i], value); \
			} else { \
				set_input_entity(ents[i], value); \
			} \
			return; \
		} \
	} \
	PARENT()::set_entity_by_name(name, value); \
} \
logical THIS()::is_entity_by_name( const char* name, ENTITY* entity) { \
	for(int i = num_ents; i--; ) { \
		if(strcmp(descriptors[i].display_name, name) == 0) { \
            if (members_are_hooked() || \
                descriptors[i].datum_type == annotation_descriptor::in) \
              return contains_this_entity(ents[i], entity, TRUE); \
            else if (unhooked_out_is_ee[i] == TRUE) \
              return ( ((EE_LIST*) INTEXTEND ents[i])->lookup(entity) >= 0 ); \
            else \
              return (entity == ents[i]); \
		} \
	} \
	return PARENT()::is_entity_by_name(name, entity); \
} \
ENTITY_DEF(not_used) \
	for(int i = 0; i < num_ents; i++ ) { \
		debug_helper(descriptors[i].display_name,ents[i], fp); \
	} \
	debug_extra(fp);\
SAVE_DEF \
	for(int i = num_ents; i--; ) { \
		write_ptr( ents[i], list); \
	} \
    /* STI sallen (7/00) - added this block */ \
    if (!members_are_hooked() && save_version_number >= ANNO_HOOKED_VERSION) { \
        for(int i = num_ents; i--; ) { \
            write_logical( unhooked_out_is_ee[i], "not_output_ee", "output_ee" ); \
        } \
    } \
	save_extra(); \
RESTORE_DEF \
	for(int i = num_ents; i--; ) { \
		ents[i] = read_ptr(); \
	} \
    /* STI sallen (7/00) - added this block */ \
    if (!members_are_hooked() && restore_version_number >= ANNO_HOOKED_VERSION) { \
        for(int i = num_ents; i--; ) { \
            unhooked_out_is_ee[i] = read_logical( "not_output_ee", "output_ee" ); \
        } \
    } \
    else { \
        /* keep the same behavior */ \
        for(int i = num_ents; i--; ) { \
            unhooked_out_is_ee[i] = TRUE; \
        } \
    } \
	restore_extra(); \
COPY_WITH_DEEP_COPY_DEF \
	/* STI ROLL  	// STI ROLL  Deep copy capable */ \
	for(int i = num_ents; i--; ) { \
		ents[i] = (ENTITY*) INTEXTEND list.lookup(from->ents[i]); \
        /* STI sallen (7/00) - added this line */ \
        unhooked_out_is_ee[i] = from->unhooked_out_is_ee[i]; \
	} \
	copy_extra(from, pm); \
SCAN_DEF \
	for(int i = num_ents; i--; ) { \
		if(reason == SCAN_DISTRIBUTE && members_are_hooked() /* || descriptors[i].same_body*/) { \
			list.add(ents[i]); \
		} \
	} \
FIX_POINTER_DEF \
	for(int i = num_ents; i--; ) { \
		ents[i] = read_array(array, ents[i]); \
	} \
TERMINATE_DEF 


#endif //__ANNOTATION_MAC_HXX__
