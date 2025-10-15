/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// ENTITY is the common class for all data structure entity
// definitions. It defines the base class for all types.
// We provide a range of macros to declare and define a large number
// of the standard member functions, used by most derived classes.
#if !defined( ENTITY_CLASS )
#define ENTITY_CLASS
#include <stddef.h>
#include "dcl_kern.h"
#include "logical.h"
#include <string.h>
#include "method.hxx"
#include "metharg.hxx"
#include "pattern_enum.hxx"
#if defined( sgi4 )
#include <generic.h>
#include <stdio.h>
#endif
#include "bullsmal.hxx"
#include "container.hxx"
/**
 * \defgroup ACISTOPOLOGY Topology
 * \ingroup KERNAPI
 *
 */

/**
 * @file entity.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISTOPOLOGY
 *
 * @{
 */
// TOPOLOGY BITS
/**
 * ASSEMBLY ID
 */
#define ASM_ASSEMBLY_ID	 0x40000000
/**
 * ASSEMBLY MODEL  ID
 */
#define ASM_MODEL_REF_ID 0xE0000004
/**
 * BODY ID
 */
#define BODY_ID			 0x10000000
/**
 * LUMP ID
 */
#define LUMP_ID			 0x04000000
/**
 * SHELL ID
 */
#define SHELL_ID		 0x02000000
/**
 * SUBSHELL ID
 */
#define	SUBSHELL_ID		 0x01000000
/**
 * FACE ID
 */
#define FACE_ID			 0x00800000
/**
 * LOOP ID
 */
#define LOOP_ID			 0x00004000
/**
 * WIRE ID
 */
#define WIRE_ID			 0x00002000
/**
 * COEDGE ID
 */
#define COEDGE_ID		 0x00001000
/**
 * TCOEDGE ID
 */
#define TCOEDGE_ID		 0x00001100
/**
 * EDGE ID
 */
#define EDGE_ID			 0x00000400
/**
 * TEDGE ID
 */
#define TEDGE_ID		 0x00000410
/**
 * VERTEX ID
 */
#define VERTEX_ID		 0x00000002
/**
 * TVERTEX ID
 */
#define TVERTEX_ID		 0x00000003
/**
 * ATTRIB ID
 */
#define ATTRIB_ID		 0x20000000
// GEOMETRY BITS
/**
 * TRANSFORM ID
 */
#define TRANSFORM_ID	 0x08000000
/**
 * SURFACE ID
 */
#define SURFACE_ID		 0x00FF0000
/**
 * PLANE ID
 */
#define PLANE_ID		 0x00400000
/**
 * SPHERE ID
 */
#define SPHERE_ID		 0x00200000
/**
 * TORUS ID
 */
#define TORUS_ID		 0x00100000
/**
 * CONE ID
 */
#define CONE_ID			 0x00080000
/**
 * SPLINE ID
 */
#define SPLINE_ID		 0x00040000
/**
 * MESHSURF ID
 */
#define MESHSURF_ID		 0x00020000
/**
 * STRIPC ID
 */
#define STRIPC_ID		 0x00010000
/**
* @nodoc 
**/
#define CUSTOM_SURFACE_ID 0x00800000
/**
 * PCURVE ID
 */
#define PCURVE_ID		 0x00000800
/**
 * CURVE ID
 */
#define CURVE_ID		 0x000003F8
/**
 * STRAIGHT ID
 */
#define STRAIGHT_ID		 0x00000200
/**
 * ELLIPSE ID
 */
#define ELLIPSE_ID		 0x00000100
/**
 * HELIX ID
 */
#define HELIX_ID         0x00000008
/**
 * INTCURVE ID
 */
#define INTCURVE_ID		 0x00000080
/**
 * COMCURV ID
 */
#define COMPCURV_ID		 0x00000040
/**
 * DEGENERATE ID
 */
#define DEGENERATE_ID	 0x00000020
/**
 * UNDEFC ID
 */
#define UNDEFC_ID		 0x00000010
/**
 * APOINT ID
 */
#define APOINT_ID		 0x00000001

class HISTORY_STREAM;
class BULLETIN_BOARD;
class BULLETIN;
class BASE_TAG_MANAGER;
class pattern;
class pattern_holder;
class VOID_LIST;
class SizeAccumulator;
class DELTA_STATE;
/**
 * @nodoc
 */
int get_history_size(HISTORY_STREAM*, int&, DELTA_STATE*);  // This guy is being declared as a
                                                            // friend function to class ENTITY.
                                                            // I don't want to make this file
                                                            // dependant on any more header files
                                                            // so I didn't put him in a header file.
// STI ROLL

class pointer_map;

// STI dgh begin - moved from later in the file to here as they are
// now needed earlier in the file.

// First construct macros to concatenate THIS() and PARENT() with
// other text to make function names, etc. This is rather system-
// dependent.

#if defined( THIS_FUNCTION )
#undef THIS_FUNCTION
#endif
#if defined( PARENT_FUNCTION )
#undef PARENT_FUNCTION
#endif
#if defined( DECL_FUNCTION )
#undef DECL_FUNCTION
#endif

// STI dgh -  can't use STDC on HPUX 9.0.5 as it breaks other files
//			  but do need to used this method for concatenation;
//			  for the older operating system it doesn't matter
//			  so lets always do it

// STIPORT TAC added sgi for same reasons
// STIPORT TAC macintosh as well
#if defined( __STDC__ ) || defined (hp700) || defined ( sgi ) || defined ( mac ) || defined (__ICL) || defined (__clang__)
// ANSI standard preprocessors define the ## operator, which we can
// use fairly easily for the purpose, the only problem being to ensure
// that the THIS() and PARENT() macros are substituted first, to avoid
// problems with surrounding spaces which some preprocessors insist on
// putting in. The concatenation macros are also defined in
// debug/module.hxx, so we conditionalise their definition to avoid
// conflict.

#if !defined( CONCAT )
#define CONCAT2(a,b) a ## b
#define CONCAT(a,b) CONCAT2(a,b)
#endif

// STI dgh
#define IS_THIS_FUNCTION(name) CONCAT(is_, name)
#define CAST_THIS_FUNCTION(name) CONCAT(cast_, name)
#define THIS_FUNCTION(name) CONCAT(THIS(),name)
#define PARENT_FUNCTION(name) CONCAT(PARENT(),name)
#define DECL_FUNCTION(lib) CONCAT(DECL_,lib)

#else
// Systems in the past have accepted a simple concatenation. Some
// may require use of "generic.hxx", but we avoid that for the
// moment, as it may not exist, and also tends to cause side-effects.

// STI dgh begin
#define IS_() is_
#define IS_THIS_FUNCTION(name) IS_()name
#define CAST_() cast_
#define CAST_THIS_FUNCTION(name) CAST_()name
// STI dgh end
#define THIS_FUNCTION(name) THIS()name
#define PARENT_FUNCTION(name) PARENT()name
#define decl_() DECL_
#define DECL_FUNCTION(lib) decl_()lib

#endif

// STIPORT TAC for sgi4
// sgi 4.0.5 C++ compiler doesn't define __STDC__, but we might
// want to, so let's define the following regardless.
#if defined( sgi4 )
#define IS_THIS_FUNCTION(name) name2(is_, name)
#define CAST_THIS_FUNCTION(name) name2(cast_, name)
#define THIS_FUNCTION(name) name2(THIS(),name)
#define PARENT_FUNCTION(name) name2(PARENT(),name)
#define DECL_FUNCTION(lib) name2(DECL_,lib)
#endif

// STI dgh end

// Macros for simplifying the definition of user entities. There are
// rather a lot of member functions required for any entity, but
// many of them are the same for all (at the source level),
// and some of the others have elements in common. These macros are
// to help with this commonality.

// Macro defined for save, restore and copy declarations. The macro
// argument is the ENTITY type being defined. Generally the "common"
// functions which do the main work are protected, as they are only
// called from the main entry functions or by themselves up the
// derivation tree. Restore_common is public, though, as the main
// restore function is not a member function, and making it a friend
// would stop its being declared static (i.e. local to the defining
// module).

// STI jmb: Add reason why to copy_scan

/**
 * Used by <tt>SCAN_DEF</tt> (<tt>copy_scan</tt>), <tt>COPY_DEF/COPY_WITH_DEEP_COPY_DEF</tt> 
 * (<tt>copy_common</tt>), and <tt>FIX_POINTER_DEF</tt> (<tt>fix_common</tt>) 
 * to indicate for what purpose an entity is being scanned.
 * <br><br>
 * In general, scanning is a process by which the caller can determine what entities 
 * are "reachable" from a given entity.
 * <br><br>
 * Some ACIS specific examples:
 * <br><br>
 * When a <tt>BODY</tt> is <tt>SCAN_COPY</tt>'ed, it adds its <tt>lump()</tt>,
 * <tt>wire()</tt>, and <tt>transform()</tt> to the scan-list.
 * <br><br>
 * When a <tt>LUMP</tt> is <tt>SCAN_COPY</tt>'ed, it adds its owning <tt>body()</tt>, 
 * the <tt>next()</tt> <tt>LUMP</tt>, and its first <tt>shell()</tt> to the scan-list.
 * <br><br>
 * When a <tt>COEDGE</tt> is <tt>SCAN_COPY</tt>'ed, it adds its <tt>previous()</tt>
 * <tt>COEDGE</tt>, its <tt>next()</tt> <tt>COEDGE</tt>, it's <tt>partner()</tt> 
 * <tt>COEDGE</tt>, its <tt>edge()</tt>, its <tt>owner()</tt> (a <tt>LOOP</tt> or 
 * <tt>WIRE</tt>), and its <tt>geometry()</tt>.
 * <br><br>
 * When the base class <tt>ENTITY</tt> is <tt>SCAN_COPY</tt>'ed, it walks its attribute
 * list forwards (using <tt>next()</tt>) and adds the first <tt>copyable()</tt> 
 * attribute it finds.
 * <br><br>
 * When the base class <tt>ATTRIB</tt> is <tt>SCAN_COPY</tt>'ed, it first walks the 
 * attribute linked list backwards (using <tt>previous()</tt>) and adds the first 
 * <tt>copyable()</tt> attribute it finds. Then it walks the attribute linked list 
 * forwards (using <tt>next()</tt>) and adds the first <tt>copyable()</tt> attribute 
 * it finds. Finally, it adds its <tt>owner()</tt>.
 * <br><br>
 * Typically, when an entity is scanned, it simply adds everything it can reach to the 
 * scan list. However, depending on the <tt>SCAN_TYPE</tt>, it may be that some of the 
 * entities within reach are excluded. Refer to the following descriptions of each 
 * <tt>SCAN_TYPE</tt> to see how each is used.
 * <br><br>
 * @param SCAN_UNSPECIFIED
 * Like <tt>SCAN_DISTRIBUTE</tt>, intends to reach everything possible.
 * @param SCAN_COPY
 * The default scan used by <tt>copy_scan</tt>. The type of scan used by 
 * <tt>api_copy_entity</tt>, <tt>api_copy_body</tt>, and <tt>api_copy_entity_list</tt>.
 * @param SCAN_DELETE
 * The type of scan used by <tt>api_del_entity</tt> and <tt>api_del_entity_list</tt>.
 * @param SCAN_DISTRIBUTE
 * Like <tt>SCAN_UNSPECIFIED</tt>, intends to reach everything possible.
 * @param SCAN_DEEP_COPY
 * The type of scan used by <tt>api_deep_copy_entity</tt> and 
 * <tt>api_deep_copy_entity_list</tt>.
 * @param SCAN_PATTERN
 * The type of scan used when creating a new pattern element.  The scan should not 
 * add any topological or geometrical entities to the scan-list if they do not belong to 
 * the pattern element being scanned.
 * @param SCAN_PATTERN_DOWN
 * Like <tt>SCAN_PATTERN</tt>, except we don't scan parents or siblings.
 * @param SCAN_DOWN
 * The type of scan used to scan down a topology hierarchy, excluding siblings of 
 * starting entity. Used primarily for copy purposes.
 * @param SCAN_UP
 * Not currently implemented or used (reserved for possible use to scan up a topological 
 * hierarchy.)
 * @param SCAN_SAVE_BUT_NOT_COPY
 * The type of scan used to find entities that need to be saved but should be ignored while copying.
 * This is used by multi-threaded save/restore.
 * @see ENTITY
 */
enum SCAN_TYPE 
{
	SCAN_UNSPECIFIED,
	SCAN_COPY,
	SCAN_DELETE,
	SCAN_DISTRIBUTE,
	SCAN_DEEP_COPY,
	SCAN_PATTERN,
    SCAN_PATTERN_DOWN,
	SCAN_DOWN,
	SCAN_UP,
	SCAN_SAVE_BUT_NOT_COPY
};
// STI jmb: Add reason why to copy_scan end -- and copy_scan line below.

// STI ROLL  Add DEEP COPY capability
//////////////////////////////////////////////////////////////

/**
 * @nodoc
 */
#define sav_res_cop( type ) \
public: \
	virtual void save( ENTITY_LIST & ) const; \
	virtual void copy_scan( ENTITY_LIST &, SCAN_TYPE reason = SCAN_COPY, logical dpcpy_skip = FALSE) const; \
	virtual logical is_deepcopyable( ) const; \
	virtual ENTITY* copy_data( ENTITY_LIST &, pointer_map *pm = NULL, logical dpcpy_skip = FALSE, SCAN_TYPE reason = SCAN_COPY ) const;  \
	virtual void fix_pointers( ENTITY *[], SCAN_TYPE reason = SCAN_COPY ); \
\
	void restore_common(); \
protected: \
	void save_common( ENTITY_LIST & ) const; \
	void copy_common( ENTITY_LIST &, type const *, pointer_map *pm = NULL, logical dpcpy_skip = FALSE, SCAN_TYPE reason = SCAN_COPY ); \
	void fix_common( ENTITY *[], SCAN_TYPE reason = SCAN_COPY ); \
public:



// Declare the common utility routines for the attribute header file.
// A bug (or something) in AIX requires that a virtual destructor be
// public, but it is important for development that on other machines
// it be private, so fix it up here.


#define ENTITY_IS( name, lib ) \
DECL_FUNCTION(lib) logical IS_THIS_FUNCTION(name) (const ENTITY*);

#define ENTITY_IS_PROTOTYPE( name, lib ) \
class name; \
ENTITY_IS( name, lib ) \
inline name* CAST_THIS_FUNCTION(name) (ENTITY *e) { return IS_THIS_FUNCTION(name) (e) ? (name*)e : nullptr; } \
inline const name* CAST_THIS_FUNCTION(name) (const ENTITY *e) { return IS_THIS_FUNCTION(name) (e) ? (const name*) e : nullptr; }

#define ENTITY_FUNCTIONS( name, lib ) \
private: \
	virtual ENTITY *make_copy() const; \
protected: \
	virtual ~name(); \
	void fixup_copy( name * ) const; \
public: \
	virtual int identity( int = 0 ) const; \
	virtual const char *type_name() const; \
	virtual unsigned size() const; \
	virtual void debug_ent( FILE * ) const; \
\
	/* basic routines for save, restore and copy */ \
	sav_res_cop( name ) \
\
/* STI aed: Pass METHOD_IDs by reference for efficiency, per Joe Esch */ \
	static MethodFunction add_method( METHOD_ID const &, MethodFunction ); \
	virtual logical call_method( METHOD_ID const &, METHOD_ARGS const & ); \
\
/* STI dgh: Method to determine if an entity is of a specific type */ \
	friend ENTITY_IS( name, lib )  \
\
public: \
	virtual void lose();

#define TRANSFORM_FUNCTION \
public: \
	virtual logical apply_transform( \
		SPAtransf const &, ENTITY_LIST &, logical = FALSE, logical = TRUE);

#define LOOKUP_FUNCTION \
public: \
	virtual int lookup( logical ) const;

#define FULLSIZE_FUNCTION \
protected: \
virtual void full_size(SizeAccumulator&, logical) const; \
public:

// STI jmb: Moved from usecount.hxx
// Use this inside a class declaration to declare that this class has
// a roll_notify method.

#define ROLL_NOTIFY_DECL \
void roll_notify( BULLETIN_TYPE, ENTITY* );

// For consistent naming (like TRANSFORM_FUNCTION, LOOKUP_FUNCTION, etc.)
#define ROLL_NOTIFY_FUNCTION ROLL_NOTIFY_DECL
// STI jmb: end

// Define the same utility routines for the implementation file.
// Several of them have user-defined interludes, so we declare several
// macros to bracket the interludes. These macros must be used in
// the same order as they are defined here.

// This sequence of macros must appear in the attribute implementation
// file before any other implementation routines - in particular before
// constructors and the object copy routine - but after the definition
// of THIS() and PARENT().

// STI dgh move the macros to concatenate THIS() and PARENT() to the
// beginning of the file.

// Introduce the main part of a user-defined entity definition.
// This declares the subclass restore object, and then standard
// routines. Note that the argument to the entity definition macros
// is not now used, but is retained for backwards compatibility.

#define ENTITY_DEF_ID( ext_id, ext_id_id ) \
	UTILITY_DEF(ext_id_id) \
	DEBUG_DEF

#define ENTITY_DEF( ext_id ) \
	UTILITY_DEF(0) \
	DEBUG_DEF

#define UTILITY_DEF(ext_id_id) \
\
int THIS_FUNCTION(_TYPE) = ext_id_id; \
\
/* STI dgh - define function to determine if an entity is of a specific type */ \
\
logical IS_THIS_FUNCTION( THIS() ) (const ENTITY* e) { \
	return \
		((e != NULL) && (e->identity(THIS_FUNCTION(_LEVEL)) == THIS_FUNCTION(_TYPE))); \
}\
\
/* Link restore routine with rest of system */ \
\
static ENTITY *THIS_FUNCTION(_restore_data)(); \
extern DECL_FUNCTION(PARENT_LIB) restore_def *PARENT_FUNCTION(_subclasses); \
DECL_FUNCTION(THIS_LIB) restore_def *THIS_FUNCTION(_subclasses); \
static restore_def THIS_FUNCTION(_restore_def)( \
							PARENT_FUNCTION(_subclasses), \
							THIS_FUNCTION(_NAME), \
							THIS_FUNCTION(_TYPE), \
							THIS_FUNCTION(_restore_data), \
							&THIS_FUNCTION(_subclasses) \
						); \
\
/* "Runtime virtual functions" */ \
\
static METHOD_TABLE THIS_FUNCTION(_METHODS); \
\
/* STI aed: Pass METHOD_IDs by reference for efficiency, per Joe Esch */ \
MethodFunction THIS()::add_method(METHOD_ID const &id, MethodFunction func) \
{ \
	return THIS_FUNCTION(_METHODS).add(id, func); \
} \
\
logical THIS()::call_method(METHOD_ID const &id, METHOD_ARGS const &args) \
{ \
	if (strcmp(args.id(), id.arg_id())) \
		return FALSE; \
\
	MethodFunction func = THIS_FUNCTION(_METHODS).find(id); \
\
	if (func) \
		return (*func)(this, args); \
	else \
		return PARENT()::call_method(id, args); \
} \
\
ENTITY *THIS()::make_copy() const { \
	THIS() *rollback = ACIS_NEW THIS(); \
	*rollback = *this; \
	fixup_copy( rollback ); \
	return rollback; \
} \
\
int THIS()::identity( int level ) const { \
	return \
		level == 0 ? THIS_FUNCTION(_TYPE) : \
		level < 0  ? PARENT()::identity( level + 1 ) : \
		level > THIS_FUNCTION(_LEVEL) ? -1 : \
		level == THIS_FUNCTION(_LEVEL) ? THIS_FUNCTION(_TYPE) : \
						PARENT()::identity( level ); \
} \
\
const char *THIS()::type_name() const { \
	return THIS_FUNCTION(_NAME); \
} \
\
unsigned THIS()::size() const { \
	return sizeof( THIS() ); \
} \
\
void THIS()::save( ENTITY_LIST &list ) const { \
	save_begin(); \
	save_common( list ); \
	save_end( list ); \
} \
\
static ENTITY *THIS_FUNCTION(_restore_data)() { \
	THIS() *new_ent = ACIS_NEW THIS(); \
	new_ent->restore_common(); \
	return new_ent; \
} \
\
void THIS()::fix_pointers( ENTITY *array[], SCAN_TYPE reason ) { \
	fix_common( array, reason ); \
} \
\
ENTITY *THIS()::copy_data( ENTITY_LIST &list, pointer_map *pm, logical dpcpy_skip, SCAN_TYPE reason ) const { \
	THIS()* new_att = ACIS_NEW THIS(); \
	new_att->copy_common( list, this, pm, dpcpy_skip, reason ); \
	return new_att;



// We place the break here, rather than finishing copy_data(), not
// because the user will ever want to place anything extra into
// copy_data(), but so that the following macros (except TERMINATE_DEF, of
// course), can come in any order.


#define DEBUG_DEF \
} \
\
void THIS()::debug_ent( FILE *fp ) const { \
	PARENT()::debug_ent( fp );

// At this point the application writer places code to write out
// useful information about the entity (none if he does not want
// to).

#define TRANSFORM_DEF \
} \
\
logical THIS()::apply_transform( \
	const SPAtransf &tform, ENTITY_LIST &list, logical negate, logical reset_pattern ) { \
	if (list.lookup( this ) >= 0) \
		return TRUE; \
	PARENT()::apply_transform(tform, list, negate, reset_pattern ); \
	list.add( this ); \
	trans_attrib( this, tform, &list );

// At this point the application writer places code to transform the
// entity. The application writer must include a return statement
// with the value TRUE.

#define LOOKUP_DEF \
} \
\
int THIS()::lookup( logical insert ) const { \
	DEBUG_LIST *list = dmap->get(&THIS_FUNCTION(_TYPE)); \
	if (list == NULL) \
		if (insert) { \
			list = ACIS_NEW DEBUG_LIST( THIS_FUNCTION(_NAME) ); \
			dmap->put(&THIS_FUNCTION(_TYPE), list); \
		} \
		else \
			return -1; \
\
	return list->lookup( this, insert );

// Nothing should be inserted in this space, but LOOKUP_DEF is defined
// this way so that it fits in with all the other XXX_DEFs.

#define SAVE_DEF \
} \
\
void THIS()::save_common( ENTITY_LIST &list ) const { \
	write_id_level( THIS_FUNCTION(_NAME), THIS_FUNCTION(_LEVEL) ); \
	PARENT()::save_common( list );

// Here the application writer inserts code to write out specific
// attribute information to the save file, and to insert any pointers
// into the list.


#define RESTORE_DEF \
} \
\
void THIS()::restore_common() { \
	PARENT()::restore_common();

// Here the application writer inserts code to read back specific
// attribute information from the save file.

#define FULLSIZE_DEF \
} \
\
void THIS()::full_size(SizeAccumulator& est, logical countSelf) const { \
if (countSelf) est += size(); \
PARENT()::full_size(est, FALSE);

// Here the application writer inserts code to count the size
// of all things this class owns. Note we use a type called
// SizeAccumulator which needs it's include file.


// STI ROLL  Add DEEP COPY capability
#define COPY_DEF \
} \
\
logical THIS()::is_deepcopyable() const { \
	return FALSE; \
} \
void THIS()::copy_common( ENTITY_LIST &list, THIS() const *from, pointer_map *pm, logical dpcpy_skip, SCAN_TYPE reason ) { \
	PARENT()::copy_common( list, from, pm, dpcpy_skip, reason );

#define COPY_WITH_DEEP_COPY_DEF \
} \
\
logical THIS()::is_deepcopyable() const { \
	return PARENT()::is_deepcopyable(); \
} \
void THIS()::copy_common( ENTITY_LIST &list, THIS() const *from, pointer_map *pm, logical dpcpy_skip, SCAN_TYPE reason ) { \
	PARENT()::copy_common( list, from, pm, dpcpy_skip, reason );


// Here the application writer inserts code to copy data items into
// the new object, using "list" to convert any pointers into indices.


// STI jmb: Added SCAN_TYPE
#define SCAN_DEF \
} \
\
void THIS()::copy_scan( ENTITY_LIST &list, SCAN_TYPE reason, logical dpcpy_skip ) const { \
	PARENT()::copy_scan( list, reason, dpcpy_skip );

// Here the application writer inserts code to enter any pointers
// into the list.



#define FIX_POINTER_DEF \
} \
\
void THIS()::fix_common( ENTITY *array[], SCAN_TYPE reason ) { \
	PARENT()::fix_common( array , reason );


// Here the application writer inserts code to convert any pointers
// from array indices into array contents, where a negative index
// represents NULL. Special care must be taken with entries pointing
// to objects with use counts, for example, to ensure that the target
// is correctly updated, and that the pointer is not followed until
// it has been converted from the array index.

#define FIXUP_COPY_DEF \
} /* close out previous definition */ \
void THIS()::fixup_copy( THIS() * rollback ) const { \
	PARENT()::fixup_copy(rollback); \

// Use this in a class definition to declare the arguments and opening
// bracket for the roll_notify method

#define ROLL_NOTIFY_DEF \
} /* close out previous definition */ \
void \
THIS()::roll_notify( \
	BULLETIN_TYPE type, \
	ENTITY*       other \
	) \
{ \
    PARENT()::roll_notify(type, other); \

// Here the application writer inserts code to update any structures
// associated with this entity type as a result of the roll.  This
// function is called just before the bulletin is rolled.  So for example:
// if the type is CREATE_BULLETIN, the entity is about to be removed from
// active use in the model.  For a CHANGE_BULLETIN, the "other" entity is
// about to become active, while this one will be removed from the model.
// for a DELETE_BULLETIN, this entity is about to come back to life.
//
// The base-class implementation takes care of removing or adding the
// entity from its pattern holder, if there is one.

// STI jmb: end

#define TERMINATE_DEF \
}

// Simple copy, lose and destroy functions. These are adequate for
// entities which do not have special requirements for updating
// connected objects, or any dependent structure. Otherwise the user
// must define his own.

#define SIMPLE_COPY \
	void THIS()::fixup_copy( THIS() * rollback ) const { \
	PARENT()::fixup_copy(rollback);  /* STI jmb */ \
}
#define SIMPLE_LOSE \
void THIS()::lose() { \
	PARENT()::lose(); \
}
#define SIMPLE_DTOR \
THIS()::~THIS()() { \
	/* STI jmb: the check in check_destroy has be commented out */ \
	/* for years, so get rid of it and its static data */ \
	/* check_destroy(); */\
}

#define SIMPLE_COPY_LOSE \
	SIMPLE_COPY \
	SIMPLE_LOSE \
	SIMPLE_DTOR

// **************************************************************

// Finally two macros for constructing application-developer master
// attributes. These are very stylised, and so can be constructed
// entirely by macro.


#define MASTER_ENTITY_DECL( name, lib ) \
class DECL_FUNCTION(lib) name: public ENTITY { \
	ENTITY_FUNCTIONS( name, lib ) \
public: \
	name(); \
	LOOKUP_FUNCTION \
\
};

#define MASTER_ENTITY_DEFN( ext_id ) \
ENTITY_DEF( ext_id ) \
	/* No specific data to print out */ \
LOOKUP_DEF \
SAVE_DEF \
	/* No specific data to save */ \
RESTORE_DEF \
	/* No specific data to restore */ \
COPY_WITH_DEEP_COPY_DEF \
	/* STI ROLL  Deep copy capable */ \
	/* No specific data to copy */ \
SCAN_DEF \
	/* No specific pointer data */ \
FIX_POINTER_DEF \
	/* No specific pointer data */ \
TERMINATE_DEF \
\
THIS()::THIS()() {} \
\
SIMPLE_COPY_LOSE


// ===================================================================

// Now declare the base class ENTITY, from which all data structure
// objects are derived.

// ===================================================================


class BULLETIN;
class ATTRIB;
class ENTITY_LIST;
class SPAtransf;


// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

extern DECL_KERN int ENTITY_TYPE;


// Identifier that gives number of levels of derivation of this class
// from ENTITY.

#define ENTITY_LEVEL 0


// Class defined to handle construction, copying and deletion of
// extra data read in from a "save" file, for unknown derived types.
// It is in effect a "clever pointer". The class it points to has
// a use count, and is effectively constant, so that copying an
// object of this class does not need to duplicate the object
// referenced, merely to copy the pointer and increment the use count.

class unknown_entity_text;
/**
 * Implements the member functions to point to an object containing textual data to be read from a save file.
 * <br>
 * <b>Role:</b> This class implements the member functions for a pointer to an
 * object containing textual data to be read from a save file, which represents
 * an unknown entity type to ACIS.
 * @see ENTITY
 */
class DECL_KERN unknown_text_pointer : public ACIS_OBJECT {
	unknown_entity_text *ptr;
public:
/**
 * Constructs an <tt>unknown_text_pointer</tt> (default constructor).
 */
	unknown_text_pointer() { ptr = NULL; }
/**
 * C++ destructor, deleting a <tt>unknown_entity_pointer</tt>.
 */
	~unknown_text_pointer();
/**
 * Sets the defining unknown entity text via the given reference (assignment operator).
 * <br><br>
 * @param ptr
 * unknown entity text.
 */
	unknown_text_pointer const &operator=(
				unknown_text_pointer const & ptr
			);
/**
 * Sets the defining unknown entity text via the given pointer (assignment operator).
 * <br><br>
 * @param ptr
 * unknown entity text.
 */
	unknown_text_pointer const &operator=(
				unknown_entity_text * ptr
			);
/**
 * Returns the pointer to the defining unknown entity text.
 */
	unknown_entity_text *operator->() const { return ptr; }
/**
 * Casts into an <tt>unknown_entity_text</tt> by returning the pointer to the defining <tt>unknown_entity_text</tt>.
 */
	operator unknown_entity_text *() const { return ptr; }
/**
 * Returns <tt>TRUE</tt> if the given pointer points to the defining unknown entity text; otherwise, it returns <tt>FALSE</tt>.
 * <br><br>
 * @param p
 * pointer to the unknown_entity_text being tested.
 */
	logical operator==( unknown_entity_text *p ) const
		{ return ptr == p; }
/**
 * Returns <tt>FALSE</tt> if the given pointer points to the defining unknown entity text; otherwise, it returns <tt>TRUE</tt>.
 * <br><br>
 * @param p
 * pointer to the unknown_entity_text being tested.
 */
	logical operator!=( unknown_entity_text *p ) const
		{ return ptr != p; }
	// STI ROLL
/**
 * @nodoc
 */
	void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL
};
/** @} */
/**
 * \addtogroup ACISENTITYOPS
 *
 * @{
 */
class restore_data;

ENTITY_IS_PROTOTYPE(ENTITY, KERN )

#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
DECL_KERN void roll_once(BULLETIN_BOARD * );

/**
 * @nodoc
 */
DECL_KERN logical fix_entities_in_entity_array(restore_data& rd, int start_ent);


// The generic data structure entity, of which all the specific types
// are subclasses.
/**
 * Represents common data and functionality that is mandatory in all classes that are permanent objects in the model.
 * <br>
 * <b>Role:</b> All classes representing permanent objects in ACIS derive from
 * the <tt>ENTITY</tt> class. <tt>ENTITY</tt> does not represent any object within the modeler.
 * Instead, <tt>ENTITY</tt> represents common data and functionality that is mandatory in
 * all classes that represent permanent objects.
 * <br><br>
 * <tt>ENTITY</tt> contains a pointer to the bulletin. By default the the bulletin board
 * logging mechanism is on, and the pointer supports roll back mechanisms. At
 * the start of a modeling operation, the bulletin pointer is initialized
 * (to <tt>NULL</tt>) in every entity. When an entity is changed, the <tt>backup</tt> function
 * creates a new bulletin, if needed.
 * <br><br>
 * <tt>ENTITY</tt> constructors and destructors also manipulate bulletin board records.
 * Because entities are continually backed up onto the bulletin board, all objects
 * descended from <tt>ENTITY</tt> must be allocated on free store using the <tt>new</tt> operator.
 * The <tt>new</tt> operator is overridden for entities. It calls a special free store
 * allocator that manipulates a private free list. Use of a private free list
 * allows ACIS extensive control over memory management. Refer to the <i>Kernel
 * Component</i> section for additional information on bulletins and the <i>Base Component</i> section 
 * in the Spatial Product Documentation for additional information on memory management.
 * <br><br>
 * <tt>ENTITY</tt> also contains pointers from objects to system-defined and user-defined
 * attributes. Not all objects use these attribute pointers, but automatic
 * creation and deletion of attributes for any object is supported.
 * <br><br>
 * <b>Deep Copying Entities</b>
 * <br><br>
 * For a copy of an entity that does not share underlying information with the
 * original, a deep copy should be used. A deep copy is a copy made by replicating
 * the object plus any assets owned by the object, including objects pointed at
 * by data members of the object being copied. The copied item does not share
 * any data with the original. A deep copy allocates new storage for all member
 * data and any pointers, so that all the information about that item is self-contained
 * in its own memory block.
 * <br><br>
 * An example of when a deep copy of an entity is needed is when a copy of the
 * entity needs to be moved into a different history stream, breaking all ties
 * with the previous history stream.
 * <br><br>
 * A call to function <tt>api_deep_copy_entity</tt> can be used to create a deep copy.
 * There are some entities that are can not be deep copied (by design). If such
 * entities are present during a deep copy, a <tt>sys_error</tt> will be thrown. A flag
 * can be passed into the API if attributes that can't be deep copied need to be
 * skipped over when doing a deep copy. Any non-attribute entities that cannot
 * be deep copied will throw a <tt>sys_error</tt> regardless of the logical flag setting.
 * <br><br>
 * <b>Entity Macros</b>
 * <br><br>
 * Macros simplify the definition of user entities. Many of the methods required
 * for any entity are the same for all entities at the source level. Other methods
 * have elements in common. All of these methods are defined in the following
 * nested macros in the <tt>entity.hxx</tt> file:
 * <pre>
 * MASTER_ENTITY_DECL
 *	ENTITY_FUNCTIONS
 *	LOOKUP_FUNCTION
 *	TRANSFORM FUNCTION
 * MASTER_ENTITY_DEFN
 *	ENTITY_DEF
 *	  UTILITY_DEF
 *
 * <br><br>DEBUG_DEF
 *	LOOKUP_DEF
 *	SAVE_DEF
 *	RESTORE_DEF
 *	COPY_WITH_DEEP_COPY_DEF
 *	SCAN_DEF
 *	FIX_POINTER_DEF
 *	TERMINATE_DEF
 *	SIMPLE_COPY_LOSE</pre>
 * <b>Classes Derived From ENTITY</b>
 * <br><br>
 * The model data structure is based on derivatives of the C++ base class <tt>ENTITY</tt>
 * (must be uppercase). The data structure allows consistent performance in generic
 * operations such as system debugging, change records (bulletin board), roll back,
 * attributes, and model archiving and communication.
 * <br><br>
 * The classes derived from <tt>ENTITY</tt> that represent the topology of the model include:
 * <tt>BODY</tt>, <tt>LUMP</tt>, <tt>SHELL</tt>, <tt>SUBSHELL</tt>, <tt>FACE</tt>, <tt>LOOP</tt>, <tt>COEDGE</tt>, <tt>EDGE</tt>, <tt>VERTEX</tt>, and <tt>WIRE</tt>.
 * Each class inherits all properties of <tt>ENTITY</tt>, adds unique structural data,
 * and provides access to that data. Data access provides a layer of removable
 * checking. Data structures derived from <tt>ENTITY</tt> allow extensions that can remove
 * some redundancy in the model. This data structure also permits segmentation,
 * which can limit the data to only the regions of interest in a large model.
 * <br><br>
 * The classes derived from <tt>ENTITY</tt> that represent the geometry of the models
 * include: <tt>CURVE</tt>, which derives <tt>ELLIPSE</tt>, <tt>INTCURVE</tt>, and <tt>STRAIGHT</tt>; <tt>PCURVE</tt>; <tt>APOINT</tt>;
 * <tt>SURFACE</tt>, which derives <tt>CONE</tt>, <tt>PLANE</tt>, <tt>SPHERE</tt>, <tt>SPLINE</tt>, and <tt>TORUS</tt>; and <tt>TRANSFORM</tt>.
 * The specific geometric information is private to these subclasses. All access
 * to these classes passes through member functions.
 * <br><br>
 * The <tt>ENTITY</tt>-derived class <tt>ATTRIB</tt> provides a generic access function from which
 * system derived attributes are defined and user-defined attributes can be
 * defined. <tt>ATTRIB</tt> allows the application a means to determine the type of
 * attribute represented, and the way it is processed. The modeler uses system-defined
 * attributes during its operations. Refer to the <i>Kernel Component</i> section in
 * the Spatial Product Documentation for more information about using
 * the <tt>ATTRIB</tt> class. For an image illustrating the relationship of model classes, refer 
 * to technical article <i>Classes with Images</i> in the <i>Kernel Component</i> section. 
 * <br><br>
 * Lowercase classes (<tt>surface</tt>, <tt>curve</tt>, <tt>plane</tt>, <tt>cone</tt>, <tt>straight</tt>, <tt>ellipse</tt>, etc.) are
 * not derived from <tt>ENTITY</tt> but are defined as a convenient means of recording and
 * passing around groups of data.
 * <br><br>
 * <b>Restoring Entities</b>
 * <br><br>
 * Restore proceeds in two stages:
 * <br><br>
 * 1. Read the save file record and construct the new object, leaving pointers to
 * other restored entities in symbolic form (as indices into an array), and
 * entering the address into the array.
 * <br>
 * 2. After all new objects have been constructed, visit each to convert the
 * symbolic <tt>ENTITY</tt> pointers into genuine ones.
 * <br><br>
 * Restore Stage 1. The <tt>restore_entity_from_file</tt> function reads the id string from
 * the start of each save file record, and searches its tables for the id components,
 * starting with the last (base) one, and proceeding towards the leaf. It then
 * calls the restore function for the leaf-most class found. That function constructs
 * an object of the appropriate derived type, and then calls its <tt>restore_common</tt>
 * member function. That function in turn calls its parent's <tt>restore_common</tt>
 * member function, and then reads data specific to the derived class. Finally,
 * <tt>restore_entity_from_file</tt> reads any more data in the save file record, and
 * constructs an <tt>unknown_entity_text</tt> record to contain it, together with any
 * unrecognized id strings.
 * <br><br>
 * Restore Stage 2. The <tt>fix_pointers</tt> member function for each entity in the array
 * is called, with the array as argument. This calls <tt>fix_common</tt>, which calls its
 * parent's <tt>fix_common</tt>, and then corrects any pointers in the derived class. In
 * practice there is never anything special for <tt>fix_pointers</tt> to do, but it is
 * retained for consistency and compatibility.
 * @see ATTRIB, unknown_text_pointer, APOINT, ATTRIB_TAG, BULLETIN, COEDGE, CURVE, EE_LIST, HISTORY_STREAM, PCURVE, SURFACE, WIRE, entity_gedge, entity_gvertex, entity_with_ray, standard_error_info
 */
class DECL_KERN ENTITY : public ACIS_OBJECT {

private:
	friend int get_history_size(	HISTORY_STREAM *,
									int &,
									DELTA_STATE * );

	// Pointer to start of chain of attributes attached to this entity
	// (NULL if entity has no attributes).

	ATTRIB *attrib_ptr;

	// "Pointer" to a structure which contains information read from
	// a "save" file, but not recognised by this version of Acis. Its
	// sole purpose is to be reproduced on re-saving the model (with
	// adjusted pointer values), so that information is not lost.

	unknown_text_pointer text_ptr;

	void destroy();

	virtual ENTITY *make_copy() const;


protected:

    // Contains data used by the ENTITY that is not part of the roll.
	// (The rollback pointer was moved into this object.)
	/*
	// tbrv
	*/
	/**
	 * @nodoc
	 */
	entity_data_container data_container;

	/**
	 * @nodoc
	 */
	pattern_holder* pattern_ptr;

	/**
	 * @nodoc
	 */
    int pattern_idx;

    // Caches the specified pattern index if it is legal (-1 or greater);
    // otherwise returns FALSE.  Does not call backup() if roll is TRUE.
	/**
	 * @nodoc
	 */
    logical set_pattern_index(	int index,
								logical roll = FALSE);

/**
 * Fields notifications of rollback operations.
 * <br><br>
 * <b>Role:</b> The first argument specifies the type of bulletin being rolled back
 * (create, delete or change). In the case of a change notification, the second
 * argument contains the information to be restored into this object. The base
 * implementation, which is used for almost every derived class, does nothing at all.
 * <br><br>
 * @param bulletin_type
 * type of bulletin being rolled back.
 * @param ent
 * information to be restored into this argument.
 */
	virtual void roll_notify( BULLETIN_TYPE bulletin_type, ENTITY * ent);

	/**
	 * @nodoc
	 */
	friend DECL_KERN void roll_once( BULLETIN_BOARD * );

	/**
	 * C++ destructor.
	 * <br><br>
	 * This C++ destructor should never be called directly. Instead, applications
	 * should use the overloaded <tt>lose</tt> method inherited from the <tt>ENTITY</tt> class, because
	 * this supports history management. (For example, <tt>x=new ENTITY(...)</tt> then later <tt>x->lose</tt>.)
	 */
	virtual ~ENTITY();

/**
 * Fixes any pointers in the <tt>ENTITY</tt> after copying for backup.
 * <br><br>
 * <b>Role:</b> For <tt>ENTITY</tt>, this method has nothing to do, but because it is called by
 * all data types, it is required to terminate the copy chain. (Supplied by the
 * <tt>ENTITY_FUNCTIONS</tt> and <tt>SIMPLE_COPY_CONE</tt> macros.)
 * <br><br>
 * @param rollback
 * the ENTITY to copy for backup.
 */
	void fixup_copy( ENTITY * rollback ) const;

/**
 * Handles the save operation.
 * <br><br>
 * <b>Role:</b> Writes out the savable data associated with the base class. In the case of
 * <tt>ENTITY</tt>, saveable data is simply the attribute pointer. In the case of child classes
 * derived from <tt>ENTITY</tt>, class-specific <tt>save_common</tt> methods handle the
 * class-specific data. Called by the <tt>save</tt> method. (Supplied by the
 * <tt>ENTITY_FUNCTIONS</tt> and <tt>SAVE_DEF</tt> macros.)
 * <br><br>
 * Each derived class's <tt>save_common</tt> method first writes out that class's
 * identifier, calls its parent class's <tt>save_common</tt> method, and then writes out
 * data particular to the derived class. Thus the save record consists of identifiers in
 * leaf-to-base order, followed by class data in base-to-leaf order. As it writes out
 * any <tt>ENTITY</tt> pointers, it adds them to the <tt>ENTITY_LIST</tt>, so that any dependent data also
 * gets written to the file.
 * <br><br>
 * @param entity_list
 * ENTITYs to save
 */
	void save_common( ENTITY_LIST & entity_list ) const;

/**
 * Fills in this <tt>ENTITY</tt> with data from the copied <tt>ENTITY</tt>, converting 
 * pointers into indices in the supplied <tt>ENTITY_LIST</tt>.
 * <br><br>
 * <b>Role:</b> Supports the copy operation. (Supplied by the <tt>ENTITY_FUNCTIONS</tt> 
 * and <tt>COPY_DEF</tt> macros.) Whenever the operation encounters a pointer to a 
 * dependent <tt>ENTITY</tt>, it find the index of the latter in the supplied 
 * <tt>ENTITY_LIST</tt> and sets the corresponding pointer in the copy to that index.
 * <br><br>
 * @param list
 * list of ENTITYs being copied.
 * @param from
 * original ENTITY being copied.
 * @param pm
 * pointer map.
 * @param dpcpy_skip
 * This logical indicates how non-deepcopyable attributes are handled during a deep copy.  
 * If TRUE, the copy will continue even if it encounters attributes marked as not deep-copyable. 
 * By default (FALSE), if any attributes that are found to be not deep-copyable, a sys_error 
 * will be thrown. This flag affects only how attributes are handled; any non-deepcopyable 
 * non-attribute entities encountered will cause a sys_error to be thrown regardless of this 
 * flag's setting.
 * <br><br>
 * This argument is used only by the ENTITY base class.  No other classes should need to utilize 
 * this argument. 
 * @param reason
 * type of scan.
 */
 	void copy_common(	ENTITY_LIST & list,
						const ENTITY * from,
						pointer_map *pm = NULL,
						logical dpcpy_skip = FALSE,
						SCAN_TYPE reason = SCAN_COPY );

/**
 * Converts pointers from copy indices into real pointers.
 * <br><br>
 * <b>Role:</b> Called by <tt>fix_pointers</tt>. For <tt>ENTITY</tt>, converts the
 * attribute pointer, and any pointers in the <tt>unknown_entity_text</tt> record.
 * (Supplied by the <tt>ENTITY_FUNCTIONS</tt> and <tt>FIX_POINTER_DEF</tt> macros.)
 * <br><br>
 * @param array
 * ENTITY pointers to fix.
 * @param reason
 * type of scan.
 */
	void fix_common(	ENTITY * array[],
						SCAN_TYPE reason = SCAN_COPY );

/**
 * Checks that entity deletion is legitimate.
 * <br><br>
 * <b>Role:</b> This method complains if there is an attempt to delete an entity other
 * than through the private member function <tt>destroy</tt>, which is carefully
 * controlled.
 */
	void check_destroy() const {}; // Keep just for compatibility.


public:

// Jeff - 06.29.07 Debug helper for non-repeatable addresses - internal use only
#ifdef INTERNAL_DEBUG_CHECKS
/**
 * @nodoc
 */
	int index(){return data_container.entity_data_ptr->index;}
// check whether enitity is lost. If yes sys_error is thrown.
	void check_lost_ent() const;

#endif

// This function is hidden from mkman in the MASTER_ENTITY_DECL macro; to have it documented,
// we include it here:
#if 0
/**
 * Constructs an <tt>ENTITY</tt> (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it. The
 * allocation constructor is used primarily by restore. Applications should call
 * this constructor only with the overloaded <tt>new</tt> operator, because this
 * reserves the memory on the heap, a requirement to support roll back and history
 * management.
 */
    ENTITY();
#endif

/**
 * Returns a type identifier for <tt>this</tt> ENTITY.
 * <br><br>
 * <b>Role:</b> If <tt>level</tt> is unspecified or 0, returns the type identifier <tt>ENTITY_TYPE</tt>. If
 * <tt>level</tt> is specified, returns <tt>ENTITY_TYPE</tt> for that level of derivation from <tt>ENTITY</tt>. The
 * level of this class is defined as <tt>ENTITY_LEVEL</tt>.
 * <br><br>
 * A positive <tt>level</tt> specifies the level of derivation <i>down</i> from the
 * <tt>ENTITY</tt> class to examine. <tt>ENTITY</tt> itself has <tt>level</tt> 0. If <tt>level</tt> is
 * unspecified or 0, the type of the most specific class of object is returned. If
 * <tt>level</tt> is 1, the type of the class one level down from <tt>ENTITY</tt> is returned.
 * If <tt>level</tt> is 2, the type of the parent class 2 levels down from <tt>ENTITY</tt> is
 * returned, and so on. If <tt>level</tt> is greater than the level of the instance,
 * -1 ("unknown") is returned.
 * <br><br>
 * A negative <tt>level</tt> similarly specifies the level of derivation <i>up</i> from the
 * most specific class of object returned.
 * <br><br>
 * (Supplied by the <tt>ENTITY_FUNCTIONS</tt> and <tt>UTILITY_DEF</tt> macros.)
 * <br><br>
 * @param level
 * the derivation level at which the object is to be identified.
 */
	virtual int identity( int level = 0 ) const;

/**
 * Returns a name for this <tt>ENTITY</tt>'s type.
 * <br><br>
 * <b>Role:</b> For the <tt>ENTITY</tt> class, <tt>unknown</tt> is returned.
 */
	virtual const char *type_name() const;

/**
 * Returns the size, in bytes, of this <tt>ENTITY</tt>.
 * <br><br>
 * <b>Role:</b> (Supplied by the <tt>ENTITY_FUNCTIONS</tt> and <tt>UTILITY_DEF</tt> macros.)
 */
	virtual unsigned size() const;

/**
 * Returns TRUE if this is an unknown entity. An unknown entity is
 * one that is restored from SAT/SAB but is not native to the running app.
 */
	logical is_unknown_entity( void ) const;

/**
 * @nodoc
 */
	unknown_entity_text* get_unknown_entity_text() const;

/**
 * Prints the type and address of this <tt>ENTITY</tt>, roll back pointer, attributes, and any unknown subtype information.
 * <br><br>
 * <b>Role:</b> This method is called by <tt>DEBUG_LIST::debug</tt> for an entirely unknown
 * <tt>ENTITY</tt> type, or at the start of any derived class's <tt>debug_ent</tt> method. Each
 * derived class's <tt>debug_ent</tt> method uses the debug function described in the
 * Spatial Product Documentation to print information added at that
 * level. (Supplied by the <tt>ENTITY_FUNCTIONS</tt> and <tt>DEBUG_DEF</tt> macros.)
 * <br><br>
 * @param file_pointer
 * file pointer.
 */
	virtual void debug_ent( FILE * file_pointer ) const;

/**
 * Performs a save operation.
 * <br><br>
 * <b>Role:</b> Calls <tt>save_begin</tt>, the particular class's <tt>save_common</tt>,
 * and then <tt>save_end</tt>. (Supplied by the <tt>ENTITY_FUNCTIONS</tt> and <tt>UTILITY_DEF</tt> macros.)
 * <br><br>
 * This <tt>save</tt> method is just like any other <tt>save</tt>, except that it calls
 * <tt>save_begin</tt> with an optional argument to suppress writing of a trailing
 * dash, because <tt>save_common</tt> does not write out an identifier, or call
 * any parent class's <tt>save_common</tt>.
 * <br><br>
 * @param entity_list
 * ENTITYs to save.
 */
	virtual void save( ENTITY_LIST & entity_list) const;

/**
 * Adds dependent <tt>ENTITYs</tt> of this <tt>ENTITY</tt> to the list of <tt>ENTITYs</tt> to be copied.
 * <br><br>
 * <b>Role:</b> Supports the copy operation. (Supplied by the <tt>ENTITY_FUNCTIONS</tt> and
 * <tt>SCAN_DEF</tt> macros.) The default type of scan (reason) is <tt>SCAN_COPY</tt>.
 * <br><br>
 * @param list
 * ENTITYs to copy.
 * @param reason
 * type of copy.
 * @param dpcpy_skip
 * This logical indicates how non-deepcopyable attributes are handled during a deep copy.  
 * If TRUE, the copy will continue even if it encounters attributes marked as not deep-copyable. 
 * By default (FALSE), if any attributes that are found to be not deep-copyable, a sys_error 
 * will be thrown. This flag effects only how attributes are handled; any non-deepcopyable 
 * non-attribute entities encountered will cause a sys_error to be thrown regardless of this 
 * flag's setting.
 * <br><br>
 * This argument is used only by the ATTRIB base class.  No other classes should need to utilize 
 * this argument. 
 */
	virtual void copy_scan(		ENTITY_LIST & list,
								SCAN_TYPE reason = SCAN_COPY,
								logical dpcpy_skip = FALSE) const;

/**
 * Returns <tt>TRUE</tt> if this <tt>ENTITY</tt> can be deep copied.
 */
	virtual logical is_deepcopyable() const;

/**
 * Allocates a new <tt>ENTITY</tt> and calls <tt>copy_common</tt> to fill it in.
 * <br><br>
 * <b>Role:</b> Supports the copy operation. (Supplied by the <tt>ENTITY_FUNCTIONS</tt> and
 * <tt>UTILITY_DEF</tt> macros.)
 * <br><br>
 * @param list
 * ENTITYs to copy.
 * @param pm
 * pointer map.
 * @param dpcpy_skip
 * This logical indicates how non-deepcopyable attributes are handled during a deep copy.  
 * If TRUE, the copy will continue even if it encounters attributes marked as not deep-copyable. 
 * By default (FALSE), if any attributes that are found to be not deep-copyable, a sys_error 
 * will be thrown. This flag affects only how attributes are handled; any non-deepcopyable 
 * non-attribute entities encountered will cause a sys_error to be thrown regardless of this 
 * flag's setting.
 * <br><br>
 * This argument is used only by the ENTITY base class.  No other classes should need to utilize 
 * this argument. 
 * @param reason
 * scan type.
 */
	virtual ENTITY* copy_data(	ENTITY_LIST & list,
								pointer_map *pm = NULL,
								logical dpcpy_skip = FALSE,
								SCAN_TYPE reason = SCAN_COPY ) const;

/**
 * Converts pointers from copy indices into real pointers.
 * <br><br>
 * <b>Role:</b> The <tt>fix_pointers</tt> method for each entity in the restore array is called, with
 * the given array as an argument. This calls <tt>fix_common</tt>, which calls its
 * parent's <tt>fix_common</tt>, and then corrects any pointers in the derived
 * class. In practice there is never anything special for <tt>fix_pointers</tt> to do,
 * but it is retained for consistency and compatibility. (Supplied by the
 * <tt>ENTITY_FUNCTIONS</tt> and <tt>UTILITY_DEF</tt> macros.)
 * <br><br>
 * @param entity_array
 * ENTITY pointers to fix.
 * @param reason
 * type of scan.
 */
	virtual void fix_pointers(	ENTITY * entity_array[],
								SCAN_TYPE reason = SCAN_COPY );

/**
 * Restores data for a record from disc to memory.
 * <br><br>
 * <b>Role:</b> Writes pointers as indices (<tt>fix_pointers</tt> turns them back
 * into pointers) and returns a pointer to the <tt>ENTITY</tt> made. Generally, "common" functions
 * that do the main work are protected, as they are called only from the main entry
 * functions or by themselves up the derivation tree.
 * <br><br>
 * This method is public, however, because the main restore function is not a member function,
 * and making it a friend would make it unable to be declared static, i.e., local to the
 * defining module. (Supplied by the <tt>ENTITY_FUNCTIONS</tt> and <tt>RESTORE_DEF</tt> macros.)
 * <pre>
 *    read_ptr  Pointer to record in save file for first attribute owned by entity.
 *    if (restore_version_number >= ENTITY_TAGS_VERSION)
 *       read_int      new tag </pre>
 */
	void restore_common();

/**
 * Registers an implementation function for a run-time virtual method.
 * <br><br>
 * <b>Role:</b> (Supplied by the <tt>ENTITY_FUNCTIONS</tt> and <tt>UTILITY_DEF</tt> macros.)
 * <br><br>
 * @param method_id
 * method id.
 * @param method_func
 * method function.
 */
	static MethodFunction add_method(	const METHOD_ID & method_id,
										MethodFunction method_func);

/**
 * Executes the implementation function for a routine virtual method.
 * <br><br>
 * <b>Role:</b> (Supplied by the <tt>ENTITY_FUNCTIONs</tt> and <tt>UTILITY_DEF</tt> macros).
 * <br><br>
 * @param method_id
 * method id.
 * @param method_args
 * method args.
 */
	virtual logical call_method(	const METHOD_ID & method_id,
									const METHOD_ARGS & method_args);

	/**
	 * @nodoc
	 */
	friend DECL_KERN logical is_ENTITY( const ENTITY * );

/**
 * Posts a delete bulletin to the bulletin board indicating the instance is no longer used in the active model.
 * <br><br>
 * <b>Role:</b> The <tt>lose</tt> methods for attached attributes are also called.
 */
	virtual void lose();

/**
 * Constructs an <tt>ENTITY</tt> from another <tt>ENTITY</tt> (copy constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data
 * supplied as the argument. Applications should call this constructor only with
 * the overloaded <tt>new</tt> operator, because this reserves the memory on the heap,
 * a requirement to support roll back and history management.
 * <br><br>
 * @param e
 * the ENTITY to be copied.
 */
    ENTITY(const ENTITY& e);

/**
 * Transfers data from another <tt>ENTITY</tt> to this <tt>ENTITY</tt> (assignment operator).
 * <br><br>
 * If you don't implement this in your derived <tt>ENTITY</tt>, then the compiler will provide
 * an implementation on your behalf. You need to decide if this is appropriate.
 * <br><br>
 * If you do decide to implement this method, you must first call your parent class's operator=.
 * For example, if you implement a customer attribute derived from <tt>ATTRIB</tt>, the first line of
 * your operator= implementation should be:  <tt>ATTRIB::operator=( e );</tt>
 * <br><br>
 * @param e
 * the ENTITY to be copied.
 */
    ENTITY& operator=(const ENTITY& e);

	// Entity constructor, initialising common entries, and
	// creating a bulletin.
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
	ENTITY();


	// Backup for rollback.  This just creates a duplicate record and a
	// change bulletin, if needed.  The duplication is done via a
	// virtual function, to allow each derived type to have its say.
/**
 * Creates a duplicate record and a change bulletin, if needed, as a backup for roll back.
 * <br><br>
 * <b>Role:</b> The duplication is done via the virtual function <tt>make_copy</tt>,
 * to let each derived type do its own backup. Before performing a change,
 * each routine checks whether the data structure is posted on the bulletin
 * board. If not, the posting is done automatically. Called by all system change functions.
 */
	ENTITY *backup();


	// Data reading routines.

    // STI ROLL begin:
/**
 * Returns the pointer to the bulletin board entry for this <tt>ENTITY</tt>.
 */
	BULLETIN *&rollback() const { return data_container.rollback(); }
/**
 * Returns a pointer to the history stream for this <tt>ENTITY</tt>.
 */
    HISTORY_STREAM *history() const { return data_container.history(); }
/**
 * Data reading routine.
 * <br><br>
 * @param assign_new
 * flag to assign a new tag number or not.
 */
    tag_id_type tag(logical assign_new = TRUE) const { return data_container.tag(this, assign_new); }
/**
 * Checks whether or not the record is already backed up.
 */
	logical needs_rollback() const { return data_container.rollback() == NULL; }
/**
 * Returns the number of bulletins referring to this <tt>ENTITY</tt>.
 */
    int bulletin_count() const { return data_container.bulletin_count(); }

    // STI ROLL end
/**
 * Returns this <tt>ENTITY</tt>'s attribute pointer, or <tt>NULL</tt> if there is no attached attribute.
 */
	ATTRIB *attrib() const { return attrib_ptr; }


	/**
	 * @nodoc
	 */
	entity_handle* handle() const;

	/**
	 * @nodoc
	 */
	void set_proxy(entity_proxy* new_proxy);

	/**
	 * @nodoc
	 */
	entity_proxy* proxy() const;

	//
	// DEBUG FUNCTIONS
	//

	// method to write scheme commands to file for visualization
	// [in] pFp   - file where the scheme command is to be written
	// [in] Color - color of the entity drawn
	//
	/*
	// tbrv
	*/

	/**
	 * @nodoc
	 */
	virtual void debug_scheme( FILE *pFp, int Color ) const;


	// Member setting functions.

    // STI ROLL begin: Private functions to set history stream and assign tag value on restore
private:

    void set_history(HISTORY_STREAM *hs) { data_container.set_history(this, hs); }
    logical assign_tag(tag_id_type id) { return data_container.assign_tag(this, id); }
    int add_bulletin_count() { return data_container.add_bulletin_count(); }
    int remove_bulletin_count( logical clear_history ) 
    { 
        return data_container.remove_bulletin_count( this, clear_history ); 
    }

    // make fix_entities_in_entity_array a friend so that it can blow away tags
    // when restoring without history
    friend DECL_KERN logical fix_entities_in_entity_array(restore_data& rd, int start_ent);
    // make pattern_holder a friend so that it can restore tags to pattern elements that
    // weren't saved
    friend class pattern_holder;
    friend class ATTRIB_PAT_HOLDER;

	// Methods for finding no-change bulletins
    logical bulletin_no_change(ENTITY const* other,            // other entity in bulletin
                               logical       ignore_attrib)    // TRUE is used by BULLETIN::attrib_only_change().
                                                        const; // It ignores ENTITY::attrib_ptr and allows
                                                               // tolerant (SPAresnor) comparisons between doubles.
                                                               // FALSE is used by BULLETIN::no_change().
                                                               // It requires exact matches when doing comparisons
protected:
    // virtual function for comparing subclass data - called by bulletin_no_change
/**
 * Compares subclass data.
 * <br><br>
 * <b>Role:</b> This virtual method is called by <tt>bulletin_no_change</tt>. When the
 * <tt>identical_comparator</tt> argument is <tt>TRUE</tt>, it requires an exact match when
 * comparing doubles, and returns the result of <tt>memcmp</tt> as a default (for
 * non-overridden subclasses). A <tt>FALSE</tt> argument indicates tolerant compares, in which
 * case <tt>FALSE</tt> is returned by default.
 * <br><br>
 * @param other
 * other ENTITY in the bulletin.
 * @param identical_comparator
 * identity compare flag.
 */
    virtual logical bulletin_no_change_vf(
        const ENTITY* other,            // other entity in bulletin
        logical identical_comparator)   // TRUE requires exact match when comparing doubles
                                 const; // and returns the result of memcmp as a default
                                        // (for non-overridden subclasses)
                                        // FALSE indicates tolerant compares and
                                        // returns FALSE as a default

protected:
/**
 * Returns TRUE if this entity can have a pattern_holder. For internal use only.
 */
	virtual logical holds_pattern() const {return FALSE;}
public:
    // STI ROLL end
/**
 * Changes the attribute pointer of this <tt>ENTITY</tt> to point to the given attribute.
 * <br><br>
 * @param attr
 * the new attribute.
 */
	void set_attrib( ATTRIB * attr);

	// Identify the owner, if any, in the data structure of this
	// entity. NULL indicates no owner or an unidentifiable one.
	// Objects of the base class have no identifiable owner.
/**
 * Identifies the owner, if any, from the data structure of this <tt>ENTITY</tt>.
 * <br><br>
 * <b>Role:</b> A return value of <tt>NULL</tt> indicates no owner or an unidentifiable one.
 * Objects of the <tt>ENTITY</tt> base class have no identifiable owner.
 */
	virtual ENTITY *owner() const;

	// Indicate whether this entity is normally destroyed by lose(),
	// or whether it is shared between multiple owners using a use
	// count, and so gets destroyed implicitly when every owner has
	// been lost. Most entities are destroyed explicitly using lose(),
	// and so the default returns TRUE.
/**
 * Indicates whether or not this <tt>ENTITY</tt> is normally destroyed by <tt>lose</tt>.
 * <br><br>
 * <b>Role:</b> Returns <tt>FALSE</tt> if the <tt>ENTITY</tt> is shared among multiple owners by
 * means of a use count, and so gets destroyed implicitly when every owner has been
 * lost. Most <tt>ENTITYs</tt> are destroyed explicitly using <tt>lose</tt>, so the default
 * returns <tt>TRUE</tt>.
 */
	virtual logical deletable() const;

    // Indicate whether or not a pattern can be applied to this
    // entity.  The default is FALSE, so this needs to be overridden
    // for patternable entities.  (Note that an entity can
    // have a non-NULL pattern_ptr even if it is not patternable,
    // for a pattern may be put upon a patternable parent entity.)
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Indicate whether or not a pattern can be applied to this entity.
 * The default is <tt>FALSE</tt>, so this needs to be overridden for patternable
 * entities. An entity can have a non-<tt>NULL</tt> <tt>pattern_ptr</tt> even if it is not
 * patternable, for a pattern may be put upon a patternable parent entity.
 */
    virtual logical patternable() const {return FALSE;}// internal use only
    // An entity is a "pattern child" when it is not responsible for
    // creating new entities when the pattern is applied, this being
    // the responsbility of some owning entity.  Can be overridden
    // if an entity is either always or never a pattern child to
    // always return either TRUE or FALSE.
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> An entity is a "pattern child" when it is not responsible for creating
 * new entities when the pattern is applied, this being the responsibility of some
 * owning entity.  If an entity is either always or never a pattern child, this
 * can be overridden to always return either <tt>TRUE</tt> or <tt>FALSE</tt>.
 */
    virtual logical is_pattern_child() const;// internal use only
    // Sets the pattern on this entity, replacing any existing pattern.
    // This needs to be overridden if the pattern needs passing down to
    // any child or sibling entities.
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Sets the pattern on this entity, replacing any existing pattern. This
 * needs to be overridden if the pattern needs passing down to any child or sibling entities.
 * <br><br>
 * @param in_pat
 * pattern.
 * @param keep_pointer
 * keep pointer defaults TRUE.
 */
	virtual void set_pattern(pattern* in_pat, logical keep_pointer = TRUE);// internal use only
    // Adds the pattern on this entity, if any, to the list, if the
    // pattern is not already there.  Needs overriding if there are
    // child entities to be queried.
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Adds the pattern on this entity, if any, to the list, if the pattern
 * is not already there. Needs overriding if there are child entities to be queried.
 * <br><br>
 * @param list
 * list of all patterns.
 */
    virtual void get_all_patterns(VOID_LIST& list);// internal use only
    // Removes the pattern from the parent entity owning the pattern,
    // returning FALSE if no pattern was found.  Should be overridden
    // if the pattern needs to be removed from siblings.
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Removes the pattern from the parent entity owning the pattern,
 * returning <tt>FALSE</tt> if no pattern was found. Should be overridden if the pattern
 * needs to be removed from siblings.
 */
    virtual logical remove_pattern();// internal use only

/**
 * @nodoc
 */
    virtual logical remove_pattern_global(pattern* pat = NULL);// internal use only

    // Removes this entity, along with all entities on the same pattern
    // element, from the pattern.  Should be overridden if this entity
    // can be a parent pattern entity, as the default behavior is simply
    // to pass the call on to the entity's owner.
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Removes the pattern element associated with this entity from the
 * pattern. Returns <tt>FALSE</tt> if this entity is not part of a pattern element,
 * otherwise <tt>TRUE</tt>. Should be overriden if this entity can be a parent pattern
 * entity, as the default behavior is simply to pass the call on to the entity's owner.
 */
    virtual logical remove_from_pattern();// internal use only
    // Removes the entity from its pattern list.  Should be overridden
    // if there are child entities to be removed as well.
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Removes the entity from its pattern list. Should be overridden
 * if there are child entities to be removed as well
 */
    virtual logical remove_from_pattern_list();// internal use only
    // Copy the pattern holder of this entity to target, and add the target
    // entity to the other_ents list of the pattern holder.  Should be
    // overridden if there are child entities that should implement this
    // behavior as well.
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Copy the pattern holder of this entity to <tt>target</tt>, and add the target
 * entity to the <tt>other_ents</tt> list of the pattern holder. Should be overridden if there are child entities that should
 * implement this behavior as well.
 * <br><br>
 * @param target
 * target.
 */
    virtual logical copy_pattern_down(ENTITY* target) const;// internal use only
    // Attaches an ATTRIB_PAT_HOLDER object, based on the pattern pat, to
    // this entity.  If pat is NULL, all such objects are instead removed.
    // Should be overridden if the entity has child entities that also
    // need the attrib attached or removed.  Currently returns TRUE in all
    // cases where pat is non-NULL, as the code intentionally doesn't check
    // for an existing ATTRIB_PAT_HOLDER object (the calling program should
    // check).
/**
 * @nodoc
 */
    logical copy_pat_holder_attrib(ENTITY* new_ent, logical include_children = TRUE);// internal use only
/**
 * @nodoc
 */
    virtual logical attach_pattern_attrib(pattern* pat, logical include_children = TRUE);// internal use only

    // End virtual pattern-related

    // Other pattern-related methods:

    // Returns TRUE if pattern_ptr is non-NULL and does not refer to a
    // _former_ pattern holder.
/**
 * @nodoc
 */
    logical has_pattern_holder() const;// internal use only
    // Returns TRUE if pattern_ptr is non-NULL and refers to a _former_
    // pattern holder [pattern_index() < -1].
/**
 * @nodoc
 */
    logical has_former_pattern_holder() const;// internal use only

    // Replaces the existing pattern holder (if any) with a new one
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Replaces the existing pattern holder (if any) with a new one.
 * <br><br>
 * @param in_pat
 * pattern holder.
 */
	void set_pattern_holder(pattern_holder* in_pat);// internal use only

/**
 * @nodoc
 */
	void reset_pattern_holder();// internal use only

    // Gets the pattern holder, if any, from the entity (default) or
    // from its parent.  Returns NULL if pattern_ptr is non-NULL but
    // refers to a _former_ pattern holder [pattern_index() < -1].  If the
    // pattern holder is desired in that case, use get_former_pattern_holder()
    // instead.
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Gets the pattern holder, if any, from the entity (default) or from its parent.
 * <br><br>
 * @param consider_parent
 * possible parent.
 */
	pattern_holder* get_pattern_holder(logical consider_parent = FALSE) const;// internal use only

    // Returns pattern_ptr for the case in which this entity is no longer a
    // member of the pattern [pattern_index() < -1].  If the entity is still a
    // member of the pattern, returns NULL.
/**
 * @nodoc
 */
	pattern_holder* get_former_pattern_holder() const;// internal use only

    // Adds the pattern holder on this entity, if any, to the list, if
    // the holder is not already there, and returns TRUE if this entity
    // had a holder.
/**
 * @nodoc
 */
    logical get_pattern_holder(VOID_LIST& list);// internal use only
    // Processes the pattern-holding attribs on this entity, if any,
    // by transferring the attrib's pattern holder to this object and
    // then losing the attrib.  If the entity does not have such an
    // attrib, in_ph (if non-NULL) is copied to the entity.  The function
    // returns a pointer to the pattern_holder, regardless of  its source.
/**
 * @nodoc
 */
    pattern_holder* process_pat_holder_attribs(pattern_holder* in_ph);// internal use only
    // If there is a pattern holder on this entity as well as an pattern
    // holder attribute, this function moves the cached attributes and tags
    // from the latter's pattern holder to this entity's pattern holder, then
    // removes the pattern holder attribute.  Returns TRUE if such a transfer
    // took place.
/**
 * @nodoc
 */
    logical transfer_pat_holder_attrib_caches();// internal use only
    // Gets the entity that owns the pattern on this entity, if there
    // is one.  If one of the entity's siblings is the seed entity, it
    // returns a pointer to this sibling.
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Gets the entity that owns the pattern on this entity, if there
 * is one.If one of the entity's siblings is the seed entity, it returns
 * a pointer to this sibling.
 */
    ENTITY* get_pattern_owner();// internal use only
    // Gets the pattern index associated with this entity, returning
    // -1 if pattern_ptr is NULL.  This is found by looking up the entity
    // in the pattern holder's entity list unless from_cache = TRUE, in
    // which case the cached index (pattern_idx) is used directly.  NOTE:
    // index values < -1 are used to store _former_ indices within a non_NULL
    // pattern_ptr; in this case, the returned value "n" should be replaced
    // by -(2+n) to obtain the original index, which is relative to the
    // pattern domain rather than to the current state of the pattern.  Better
    // yet, call the former_pattern_index() method in this case to get the
    // original index.
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Gets the pattern index associated with this entity, returning -1 if
 * no pattern is present.
 * <br><br>
 * @param from_cache
 * logical argument.
 */
    int pattern_index(logical from_cache = FALSE) const;// internal use only
    // Converts the absolute pattern index of a patterned entity into that of
    // a former pattern element, and returns the result.  Specifically, if the
    // absolute index is n, it is changed to -(2+n).
/**
 * @nodoc
 */
    int set_former_pattern_index();// internal use only
    // For entities that were once part of a pattern, but are no longer,
    // returns the former index (i.e., the absolute index relative to the
    // pattern domain rather than to what its current position would be in
    // the pattern.)  Returns -1 if the entity is currently a pattern member
    // or if it never was a pattern member.
/**
 * @nodoc
 */
    int former_pattern_index() const;// internal use only
    // Removes the pattern if there are unsavable or pattern-incompatible
    // attributes present.
/**
 * @nodoc
 * <b>Obsolete:</b> do not use.
 * <br><br>
 * <b>Role:</b> Removes the pattern if there are unsavable or pattern-incompatible attributes present.
 */
    logical remove_pattern_if_unsavable();// internal use only
    // Removes the pattern if there are pattern-incompatible attributes
    // present.
/**
 * @nodoc
 */
    logical remove_pattern_if_incompatible();// internal use only
    // Returns TRUE if every pattern member associated with this object has
    // been created (and therefore added to the pattern holder's list of
    // other entities), or if there is no pattern on the object
/**
 * @nodoc
 */
    logical is_pattern_expanded() const;// internal use only


	// Make BULLETIN a friend to permit it to "destroy()" things.
/**
 * @nodoc
 */
	friend class BULLETIN;

	// Make BASE_TAG_MANAGER a friend to permit it to assign_tag
/**
 * @nodoc
 */
	friend class BASE_TAG_MANAGER;

/**
 * @nodoc
 */
	friend class restore_thread_work;

/**
 * Looks up this <tt>ENTITY</tt> in the appropriate debug list.
 * <br><br>
 * <b>Role:</b> This method is not included among the standard <tt>ENTITY</tt> methods because
 * often subclasses may wish to be included in their parent's debug list. This
 * is done simply by not including a <tt>lookup</tt> method in the subclass.
 * <br><br>
 * When the logical argument is <tt>TRUE</tt>, the <tt>ENTITY</tt> is inserted into the debug list if it
 * does not already exist in the list.  If it is <tt>FALSE</tt> and the <tt>ENTITY</tt> does not exist in
 * the list, a -1 is returned.  (Supplied by the <tt>LOOK_FUNCTION</tt> and <tt>LOOKUP_DEF</tt> macros.)
 * <br><br>
 * @param insert
 * flag to insert into debug list.
 */
	virtual int lookup( logical insert) const;

/**
 * Transforms this <tt>ENTITY</tt>.
 * <br><br>
 * <b>Role:</b> The <tt>ENTITY_LIST</tt> is used to prevent sub-entities from being transformed
 * more than once.  This method is not included with the standard <tt>ENTITY</tt> methods in order
 * to allow nontransformable <tt>ENTITYs</tt> to use the default implementation.  (Supplied by the
 * <tt>TRANSFORM_FUNCTION</tt> and <tt>TRANSFORM_DEF</tt> macros.)
 * <br><br>
 * @param t
 * the transform to apply.
 * @param list
 * list of entities to which the transform has already been applied.
 * @param negate
 * negation flag.
 * @param reset_pattern
 * internal use only.
 */
	virtual logical apply_transform(
        const SPAtransf &t, ENTITY_LIST &list, logical negate = FALSE, logical reset_pattern = TRUE);



protected:
	// Functions to start and terminate the save record, called by
	// save() in each derived class.

/**
 * Starts a save record.
 * <br><br>
 * <b>Role:</b> Writes a preamble to a save file record, which contains any subclass
 * identifiers not known to this version of ACIS. Called by the <tt>save</tt> method.
 * <br><br>
 * @param terminator
 * terminator flag.
 */
	void save_begin( logical terminator = FALSE ) const;
/**
 * Terminates a save record.
 * <br><br>
 * <b>Role:</b> Writes out any data associated with subclasses not known to this version of
 * ACIS before terminating the record. Called by the <tt>save</tt> method.
 * <br><br>
 * @param list
 * list of entities.
 */
	void save_end( ENTITY_LIST &list ) const;

public:
	// Function to terminate a restore record, reading any trailing text
	// and filling in the unknown text pointer. Used only by the save/restore
	// system.
/**
 * Terminates a restore record.
 * <br><br>
 * <b>Role:</b> Reads any trailing text and fills in the unknown text pointer. Used only
 * by the save/restore system.
 * <br><br>
 * @param id_str
 * the id string.
 */
	void restore_end( char * id_str );

    // Function to get ids for entities pointed to within unknown text objects
    // during restore with history.  Used only by the save/restore system.
    /*
    // tbrv
    */

    /**
     * @nodoc
     */
    void fix_unknown_pointer_ids();

	// STI jmb:  Add use counting virtual functions
	// We add a function, is_use_counted() so that client code can ask the
	// question and decide how to deal with entity life time.  On a use counted
	// entity, each held pointer to it should be declared by calling add().  There
	// should be matching calls to remove() when done with the pointer. For normal
	// entities, simply call lose() when the entity is no longer part of the
	// active model.
	//
	// For use counted entities, the implementations of add() and remove()
	// come from macros in usecount.hxx.
	//
	// For normal entities, the add() and remove() functions are empty stubs inherited
	// from the ENTITY base class.

	// Tell whether this entity is use counted.  The default implemntation returns FALSE
/**
 * Indicates if this <tt>ENTITY</tt> is use-counted or not.
 * <br><br>
 * <b>Role:</b> The default implementation returns <tt>FALSE</tt>.
 */
	virtual logical is_use_counted() const;
	//virtual logical gme_is_use_counted() const;
	/*@todo:经过测试，我们不调用该虚函数的情况下，acis却会调用该新增的虚函数，会导致异常，暂不清楚原因，因此直接重写原虚函数*/

	// If the entity is use counted, add one to the count.  If the entity is
	// not use counted, signal an error.
/**
 * If this <tt>ENTITY</tt> is use counted, adds one to the count.
 * <br><br>
 * <b>Role:</b> If this <tt>ENTITY</tt> is not use counted, signals an error.
 * <br><br>
 * Every held pointer to a use counted <tt>ENTITY</tt> should be declared by calling <tt>add</tt>.
 * There should a corresponding call to <tt>remove</tt> when the user is done with the
 * pointer. For normal <tt>ENTITYs</tt>, simply call <tt>lose</tt> when the <tt>ENTITY</tt> is no longer
 * part of the active model. For use counted <tt>ENTITYs</tt>, the implementations of
 * <tt>add</tt> and <tt>remove</tt> come from macros in <tt>usecount.hxx</tt>. Use counted
 * <tt>ENTITYs</tt> implemented without the macros should implement these virtual methods manually.
 */
	virtual void add( );
	//virtual void gme_add( );
	/*@todo:经过测试，我们不调用该虚函数的情况下，acis却会调用该新增的虚函数，会导致异常，暂不清楚原因，因此直接重写原虚函数*/

	// If the entity is use counted, subtract one from the count and if the count
	// goes to zero and lose_if_zero is set, lose the entity.  If the entity is not
	// use counted, signal an error.
/**
 * Decrements the use count of this <tt>ENTITY</tt>.
 * <br><br>
 * <b>Role:</b> If the use count reaches 0, the <tt>ENTITY</tt>'s <tt>lose</tt> method is called if
 * the argument <tt>lose_if_zero</tt> is set to <tt>TRUE</tt>. (If the entity is not use counted,
 * an error is signaled.)
 * <br><br>
 * Every held pointer to a use counted <tt>ENTITY</tt> should be declared by calling <tt>add</tt>.
 * There should a corresponding call to <tt>remove</tt> when the user is done with the
 * pointer. For normal <tt>ENTITYs</tt>, simply call <tt>lose</tt> when the <tt>ENTITY</tt> is no longer
 * part of the active model. For use counted <tt>ENTITYs</tt>, the implementations of
 * <tt>add</tt> and <tt>remove</tt> come from macros in <tt>usecount.hxx</tt>. Use counted
 * <tt>ENTITYs</tt> implemented without the macros should implement these virtual methods manually.
 * <br><br>
 * @param lose_if_zero
 * flag to lose the ENTITY when the use count drops to 0.
 */
	virtual void remove( logical lose_if_zero = TRUE );
	//virtual void gme_remove( logical lose_if_zero = TRUE );
	/*@todo:经过测试，我们不调用该虚函数的情况下，acis却会调用该新增的虚函数，会导致异常，暂不清楚原因，因此直接重写原虚函数*/

	// If the entity is use counted, get the count.  If not use counted, return zero.
/**
 * Returns the use count of this <tt>ENTITY</tt>.
 * <br><br>
 * <b>Role:</b> Returns zero if the <tt>ENTITY</tt> is not use counted.
 */
	virtual int  use_count() const;
	//virtual int  gme_use_count() const;
	/*@todo:经过测试，我们不调用该虚函数的情况下，acis却会调用该新增的虚函数，会导致异常，暂不清楚原因，因此直接重写原虚函数*/

	// If the entity is use counted, set the count.  If not use counted, do nothing.
/**
 * Sets the reference use count of this <tt>ENTITY</tt>.
 * <br><br>
 * <b>Role:</b> If the <tt>ENTITY</tt> is not use counted, this method does nothing.
 * <br><br>
 * @param use
 * new use count.
 */
	virtual void set_use_count(int use );
	//virtual void gme_set_use_count(int use );
	/*@todo:经过测试，我们不调用该虚函数的情况下，acis却会调用该新增的虚函数，会导致异常，暂不清楚原因，因此直接重写原虚函数*/

/**
 * Increments the <tt>SizeAccumulator</tt> by the size, in bytes, of everything owned by this 
 * <tt>ENTITY</tt> and, optionally, of the <tt>ENTITY</tt> itself. 
 * <br><br>
 * <b>Role:</b> If <tt>countSelf</tt> is <tt>TRUE</tt>, increments the <tt>SizeAccumulator</tt> 
 * by the size, in bytes, of the <tt>ENTITY</tt> and everything owned by the <tt>ENTITY</tt>. 
 * If <tt>countSelf</tt> is <tt>FALSE</tt>, increments the <tt>SizeAccumulator</tt> by the size, 
 * in bytes, of everything owned by the <tt>ENTITY</tt>, but not including the size of the 
 * <tt>ENTITY</tt> itself. 
 * <br><br>
 * For example, if an <tt>ENTITY</tt> contained a character string and <tt>full_size</tt> were
 * called with <tt>countSelf == FALSE</tt>, the <tt>SizeAccumulator</tt> would be incremented by
 * the size, in bytes, of the character string. 
 */
	virtual void full_size(SizeAccumulator& est, logical countSelf = TRUE) const;
        public:
/**
 * @nodoc
 */
        void full_size(int&) const;   // This isn't returning an int since I don't want
	                              // to have an overloaded function differ by return type
	// STI ROLL

	// STI jmb: end
};



#ifdef THREAD_SAFE_ACIS
/*
// tbrv
*/
/**
 * @nodoc
 */
extern DECL_KERN safe_object_pointer<ENTITY_LIST> active_entities;
#else
/*
// tbrv
*/
/**
 * @nodoc
 */
extern DECL_KERN ENTITY_LIST *active_entities;
#endif
/** @} */
#endif
