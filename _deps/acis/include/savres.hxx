/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for save and restore.
#if !defined( SAVRES_CLASSES )
#define SAVRES_CLASSES
#include <stdio.h>	
#ifdef sun4_cxx4 
#include <stdlib.h>
#endif
#include "dcl_kern.h"
#include "fileinfo.hxx"
#include "fileif.hxx"
#include "logical.h"
#include "savres_small.hxx"
#include "base.hxx"
#include "spa_progress_info.hxx"
#include "lists.hxx"
/**
* @file savres.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISSAVERESTORE
 *
 * @{
 */
class BODY;
class ENTITY;
class ENTITY_LIST;
class curve;
class surface;
class FileInterface;
class FileInfo;
#define ACIS_EOF  "End-of-ACIS-data" 
#define ACIS_HISTORY_BEGIN "Begin-of-ACIS-History-Data"
#define ACIS_HISTORY_EOS "End-of-ACIS-History-Section"
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN FileInterface*
get_savres_file_interface(
	FILE* file_ptr,
	logical mode_text
	);
/*
// tbrv
*/
/**
 * @nodoc
 */
//DECL_KERN logical save_entity_list_on_file(
//	FileInterface* file_if,
//	ENTITY_LIST    const &entities,
//	logical        with_history,
//	ENTITY_LIST&   list,
//	int&           count
//	);
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN ENTITY_ARRAY : public ACIS_OBJECT {
	ENTITY** array_ents;
	int   array_size;
	int      array_max_used_index;
 public:
	ENTITY_ARRAY()
		: array_ents(NULL), array_size(0), array_max_used_index(-1)
	{}
	~ENTITY_ARRAY() { if (array_ents != NULL) ACIS_DELETE [] STD_CAST array_ents; }
	void extend( int how_much_bigger );
	int size() { return array_size; }
	ENTITY*& operator[] (int i);
	ENTITY** ents() const { return array_ents; }
	int num_ents() { return array_max_used_index + 1; }
};

/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN restore_data : public ACIS_OBJECT {
 public:
	// Data local to the restore process which needs to be passed
	// to between various restore functions
	restore_data(ENTITY_LIST& entities)
		:	output_ents(entities), history_flag(0), next_ent(0), last_ent(0),
	        num_ents(0),num_ents_to_restore(0), init_ents(0), multi_body_blocks(0)
			{}
	ENTITY_LIST  &output_ents;
	// jamesg - restored APATTERN objects are put into this list
	// to be lost after restore is completed.  This solves operational
	// memory leaks of APATTERNS (which are not owned by any other ENTITY).
	ENTITY_LIST  restored_apatterns;
	ENTITY_ARRAY array;
	logical      history_flag;
	int          next_ent;
	int          last_ent;
	int          num_ents;
	int          num_ents_to_restore;
	int          init_ents;
	int			 multi_body_blocks;

// Assignment operator could not be generated warning
// is taken care of by adding a private one.  PRS
private:
	restore_data& operator=(const restore_data&) {return *(restore_data *)NULL_REF;}
};

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN logical
restore_some_entities(
	restore_data& rd
	);

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN logical
fix_entities_in_entity_array(
	restore_data& array,         // STI ROLL
	int          start_ent
	);

/*
// tbrv
*/
/**
 * @nodoc
 */
//DECL_KERN logical
//restore_entity_list_from_file(
//	FileInterface* file_if,
//	restore_data&  rd
//	);

// STI jhs 10mar01 begin - move to savres_small.hxx
/*
DECL_KERN ENTITY *
read_array(
	ENTITY *array[],
	int i
	);

DECL_KERN ENTITY *
read_array(
	ENTITY *array[],
	const void * ptr
	);
*/
// STI jhs 10mar01 end



#ifdef THREAD_SAFE_ACIS

extern DECL_KERN safe_object_pointer<FileInfo> save_info;
extern DECL_KERN safe_object_pointer<FileInfo> restored_info;
extern DECL_KERN safe_integral_type<logical> restoring_history;
extern DECL_KERN safe_integral_type<logical> saving_history;
extern DECL_KERN safe_integral_type<logical> saving_history_ents;

#else

extern DECL_KERN FileInfo *save_info;
extern DECL_KERN FileInfo *restored_info;
extern DECL_KERN logical restoring_history;
extern DECL_KERN logical saving_history;
extern DECL_KERN logical saving_history_ents;

#endif


// STI jmb: end

// Declare a class which records an entity type
// name, a pointer to a restore_data routine
// for that type of entity, and a link pointer.
// A constructor for the class links instances
// of the class into a chain at initialisation.
/*
// tbrv
*/
/**
 * @nodoc
 */
typedef ENTITY *(*ptr_to_restore_routine_type)();

// Each instance of this object will generate a unique type.
// It is used to generate a unique type for each entity, among
// other things.
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN type_generator {
public:
	type_generator(int& type);
};

/**
 * Records an entity type name, a pointer to a restore_data routine for that type of entity, and a link pointer.
 * <br>
 * <b>Role</b>: This class records an entity type name, a pointer to a restore_data
 * routine for that type of entity, and a link pointer. A constructor for the class
 * links instances of the class into a chain at initialization.
 */
class DECL_KERN restore_def : public ACIS_OBJECT {

#ifdef RESTORE_DEF_FRIENDS
	RESTORE_DEF_FRIENDS
#endif
	// The part of this thing that generates unique type numbers
	type_generator tg;

	// List pointer to associate all entities which are derived from
	// the same base entity.

	restore_def *next_ptr;

	// STI sfb: This goes back to the owners subclass list head.  This
	// will help us to properly unlink ourselves during destruct time.
	restore_def **owning_class_restore_list;

	// The first component of the external identifier of this class.
	// This is qualified by the identifers of all the entities from
	// which this one is derived, and so only needs to be unique
	// amongst entities in the same restore_def list.

	char const *entity_name;

	// Function to call to restore an entity of this type from a save
	// file, after reading and decoding its identifier string.

	ptr_to_restore_routine_type ptr_to_restore_routine;


	// Pointer to the head of a list of the restore_defs for all
	// classes immediately derived from this one. This has to be a
	// pointer to a pointer, as the list head must be initialised to
	// NULL before any restore_def constructor belonging to its list
	// is called. The order of calling of static initialisers is
	// undefined, so we cannot do this initialisation in a constructor,
	// but must do it statically for a simple pointer.

	restore_def * const *sub_classes;


	// Unique (to this invocation of Acis) integer identifier of
	// objects of this class, allocated by the constructor. The value
	// is therefore undefined, but returned to the class implementation
	// for comparisons.

	int entity_code;

public:
	// Constructor for a main class, possibly with sub-classes.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * @param extent
 * external entity name
 * @param code
 * entity type code
 * @param head
 * sub-class head_ptr
 * @param entptr
 * ptr to associated entities
 */
	restore_def(
			char const * extent,		// string external entity name
			int &code,				// entity type code, allocated by
								// the constructor
			ptr_to_restore_routine_type head,
			restore_def * const *entptr = NULL
								// pointer to sub-class head_ptr
								// (not null if sub-classes
								// can be present)
		);


	// Constructor for a sub-class of a main class or sub-class,
	// possibly with sub-classes.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * @param own
 * owning class head_ptr
 * @param extent
 * external entity name
 * @param code
 * entity type code
 * @param head
 * sub-class head_ptr
 * @param entptr
 * ptr to associated entities
 */
	restore_def(
			restore_def *& own,		// owning class head_ptr (gets updated)
			char const *extent,		// string external entity name
			int &code,				// entity type code, allocated by
								// the constructor
			ptr_to_restore_routine_type head,
			restore_def * const *entptr = NULL
								// ptr to sub-class head_ptr
								// (not null if sub-classes
								// can be present)
		);

	// STI sfb: Destructor to clean up after ourselves later.
/**
 * C++ destructor for restore_def which deallocates memory.
 */
	~restore_def();

/**
 * Removes an object from the restore list.
 * @param sub_classes_list
 * list of classes
 * @param object_to_remove
 * objects to remove
 */
	static logical remove_from_list(restore_def** sub_classes_list, restore_def* object_to_remove) ;

	// Read-only access to data members.
/**
 * Returns a pointer to the routine to restore objects of this class.
 */
	ptr_to_restore_routine_type get_restore_routine() const {
		return ptr_to_restore_routine;
	}

/**
 * Returns a pointer to a list of restore_defs for all classes immediately derived from this one.
 */
	restore_def *get_sub_classes() const {
		return sub_classes == NULL ? NULL : *sub_classes;
	}

/**
 * Returns the string describing objects of this class.
 */
	char const *get_ent_name() const { return entity_name; }
/**
 * Returns the integer identifier of objects of this class.
 */
	int get_ent_code() const { return entity_code; }
/**
 * Returns a pointer to the next restore_def in the list.
 */
	restore_def *next() const { return next_ptr; }
};


// Top-level functions for save, restore and copy. The first three
// are the traditional ones; the others are exactly the same, but
// accept any entity type as the "seed". In all cases, any structure
// reachable by any sequence of pointers from the seed is saved,
// restored or copied.

// Save body on file in text or binary.
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN logical save_body_on_file(
			FILE *,		// open file pointer
			logical,	// mode (true for text)
			BODY *		// body to be saved
		);

// Read body from file, in text or binary.
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN BODY *restore_body_from_file(
			FILE *,		// open file pointer
			logical		// mode (true for text)
		);

// Duplicate a body.
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN BODY *copy_body_from_body(
			BODY *
		);

// Save general entity structure on file in text or binary.
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN logical save_entity_on_file(
			FILE *,		// open file pointer
			logical,	// mode (true for text)
			ENTITY *	// first entity to be saved
		);

// Read entity structure from file, in text or binary.
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN ENTITY *restore_entity_from_file(
			FILE *,		// open file pointer
			logical		// mode (true for text)
		);

// Duplicate an entity structure.
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN ENTITY *copy_entity_from_entity(
			ENTITY *,		// entity to copy
			logical = FALSE,	// deep copy flag
			logical = FALSE, 	// ignore attributes not deepcopyable
            logical = TRUE,      // copy patterns
			logical = FALSE		// copy down only

		);
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN void copy_entity_list_from_entity_list(
			ENTITY_LIST&,		// entities to copy
            ENTITY_LIST&,       // copied entities
			logical = FALSE,	// deep copy flag
			logical = FALSE, 	// ignore attributes not deepcopyable
            logical = TRUE,      // copy patterns
			logical = FALSE		// copy down only
		);

/**
 * @nodoc
 * A non-api version of api_down_copy_entity.
 */
DECL_KERN void down_copy_entity(
	ENTITY *	in_ent,
	ENTITY *&	copy,
	logical deep = FALSE,
	logical dpcpy_skip= FALSE);

/**
 * @nodoc
 * jamesg - should be called once restore is completely done
 * to avoid operational memory leaks of unowned APATTERN objs
 */
void lose_restored_apattern_entities(restore_data &rd);

// Save a list of entities and their dependent structure on a file
// in text or binary.
/*
// tbrv
*/
/**
 * @nodoc
 */
//DECL_KERN logical save_entity_list_on_file(
//			FILE *,		// open file pointer
//			logical,	// mode (true for text)
//			ENTITY_LIST const &	// initial entities to be saved
//		);

// Read entity structure from file, in text or binary.
/*
// tbrv
*/
/**
 * @nodoc
 */
//DECL_KERN logical restore_entity_list_from_file(
//			FILE *,		// open file pointer
//			logical,	// mode (true for text)
//			ENTITY_LIST &	// set to the top-level entities
//						// saved on the file.
//		);

// STI joe - begin Allow save/restore to new targets

// Thee are the "real" save and restore procedures - all of the others
// eventually call one of these.  The others should be removed from
// this file.  I think that the other API routines when away in ACIS 1.6
/*
// tbrv
*/
/**
 * @nodoc
 */
//DECL_KERN logical save_entity_list_on_file(
//            FileInterface*,     // open file pointer
//            ENTITY_LIST const&  // list of ENTITIES to save
//        );
/*
// tbrv
*/
/**
 * @nodoc
 */
//DECL_KERN logical restore_entity_list_from_file(
//            FileInterface*,     // open file pointer
//            ENTITY_LIST&        // list of restored ENTITIES
//        );

// STI joe - end

// Determine if a save version number represents a valid save format
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN logical is_valid_save_version( int );

// Select the version number to be used for save files, for backwards
// compatibility. Defaults to current Acis version.
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN void set_save_file_version(
			int = 0,		// major version number
			int = -1		// minor version number, default gives
							// error unless major version is zero.
		);

// Get the current save version
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN void get_save_file_version(
			int &,			// major version number
			int &			// minor version number
		);

// Useful routine for reading arrays of array indices
// (returns NULL for an index of -1).
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN ENTITY *read_array(
			ENTITY *[],
			int
		);

// Function to return the integer identifier of the entity described
// by the given external identifier. The identifier is also truncated
// to just the portion which could not be matched - up to, but not
// including the '-' separating the unrecognised portion from the
// recognised part. If nothing is recognised, 0 is returned, and the
// input string is not changed.
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN int find_entity_code(
			const char *
		);

// Similar to the above in its handling of the string, this funcition
// also returns the number of levels of derivation from ENTITY, giving
// all the data required for a call to the ENTITY::identity function.
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN void find_entity_code_and_level(
			const char *,
			int &code,
			int &level
		);

// STI jhs 10mar01 begin - move to savres_small.hxx
/*
#ifndef NO_MESH_CLASSES
DECL_KERN restore_def const *
find_restore_def( char * );
#endif

// Low-level read/write of ENTITY pointers. These appear here so that
// fundamental fileio does not need to know about ENTITY or
// ENTITY_LIST.

// ENTITY pointer output. Pointers are converted to integers,
// corresponding to the order of the entities in the save file,
// starting at 0, but are preceded by a '$' sign, to distinguish
// from ordinary integers. NULL pointers become "$-1".

DECL_KERN void write_ptr( ENTITY *, ENTITY_LIST & );

// Read pointer does not translate the external integer representation
// of the pointer into a true ENTITY pointer, as many of the target
// ENTITYs will not yet have been allocated. It simply casts the
// integer to a pointer - the value is corrected later.

DECL_KERN ENTITY *read_ptr();

// Routines to setup and reset save/restore operations
// to assist with some debug operations.

DECL_KERN void begin_local_savres(
	FILE *,					// File to read or write
	int = -1,				// Major version (default = current)
	int = -1				// Minor version (default = current)
	);

DECL_KERN void end_local_savres();

// Routine to set save file header info.

DECL_KERN void set_file_info(
	unsigned long,		// Mask indicating which fields to update
	const FileInfo &	// Structure containing info to be updated
	);

// Routine to get header info for last restored file.

DECL_KERN void get_file_info(FileInfo &);

// Routine to get the restoring_history variable from the context object.
// A routine is provided to break circular dependencies in the includes.

DECL_KERN logical get_restoring_history(void);

// STI let (02/01): Added new global function
// Routine to get standard saver format flag
// Under normal circumstances this always returns TRUE.
DECL_KERN logical get_standard_save_flag();
*/
// STI jhs 10mar01 end

// jdh - begin header changes for restore_progress
/*
// tbrv
*/
/**
 * Class for recording the part restore progress data.
 * <b>Role: </b> The class contains the progress data of part restoration. 
 * The <tt>restore_progress_data</tt> object has 4 methods available to the application 
 * that can be used to modify the default behavior of the callback and to retrieve 
 * the progress data. They are public exported methods of the <tt>restore_progress_data</tt> class.
 */
class restore_progress_data : public SPA_progress_info {

private:

	int64_t m_count;
	int64_t m_index;
	int64_t m_size;
	int m_interval;
	logical m_cancel_base_callback;
	logical m_cancel_rest_callback;
	FilePosition m_pos;
	FilePosition m_start_pos;
	int m_complete;
	int m_post;

	restore_data * m_rd;
	FileInterface * m_fi;

	logical _thread_safe;

	restore_progress_data();

public:

/*
// For internal use only
*/
/**
 * @nodoc
 */
	restore_progress_data( FileInterface *input_fi, restore_data *input_rd);

/*
// For internal use only
*/
/**
 * @nodoc
 */
	~restore_progress_data();

/*
// For internal use only
*/
/**
 * @nodoc
 */
	logical update();

/*
// For internal use only
*/
/**
 * @nodoc
 */
	void enable_thread_safe();
	
/**
 * The count method returns a positive integer count that is either the
 * number of entities to restore or the size of the restore file when the entity 
 * count is not available, or a zero when none of this information is available or obtainable. 
 */
	DECL_KERN int64_t count() const;

/**
 * The index method returns a positive integer of the achieved progress,
 * which can be either the number of entities restored or the number of bytes read 
 * from the file, or a zero on the very first callback call. The customer can modify 
 * the progress measurement during the very first call.
 */
	DECL_KERN int64_t index() const;

/**
 * The size method returns a positive integer of the acquired size of
 * the restore file, or a zero if the entity count is available or the file size 
 * could not be obtained. The size method accepts an integer input size from the 
 * customer on the very first callback call useful to replace the default restore 
 * measurement.
 */
	DECL_KERN int64_t size( int64_t input_size = -1);
/**
 * The interval method returns a positive integer indicating the current
 * callback frequency, which is set to 1 by default. The method accepts an integer 
 * indicating the customer desired callback frequency on the very first call to the 
 * callback. A frequency of 1 causes the callback to get called roughly every percent. 
 * A frequency of 2 causes the callback to get called for every entity restored, which 
 * could be quite often. Setting the frequency to zero will disable the callback for 
 * this restore only.	
 */
	DECL_KERN int interval( int input_interval = -1);

/**
 * Returns a positive integer between 0 and 100 indicating the percentage of the 
 * operation which has completed. The return value 0 indicates this is the first 
 * call to the user callback from the ACIS functionality. The return value 100
 * indicates that this is the last call to the user callback from the ACIS functionality.
 */
	DECL_KERN int percentage() const;
/**
 * User callbacks can query for which ACIS functionality is calling. Typically this  
 * is done on the first call to the callback, to customize the dialog window, and 
 * on subsequent calls to branch to the approriate callback code path, if the same 
 * callback is metering several different ACIS apis.
 */
	DECL_KERN SPA_progress_info_type_id get_type() const;

/**
 * Returns TRUE if the progress logic is thread-safe, FALSE otherwise.
 * When enabled, the callbacks may be called from worker threads. However, the calls
 * will never occur concurrently.
 */
	virtual logical thread_safe() const { return _thread_safe; }

};

/**
 * Function pointer to implement part restore progress meter.
 */
typedef int (*proc_restore_progress_callback)(restore_progress_data*);

/**
 * Function for setting the restore progress callback pointer.<br><br>
 * <b>Role: </b>The callback mechanism is enabled by installing a custom callback function with 
 * the <tt>set_restore_progress_callback</tt> function. The <tt>set_restore_progress_callback</tt>
 * function accepts one argument, which is the custom callback function.  
 * <br><br>
 * @param callback_func_ptr
 * custom callback function pointer.
 */
DECL_KERN void set_restore_progress_callback(proc_restore_progress_callback callback_func_ptr);

// jdh - end header changes for restore_progress

/** @} */
#endif
