/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for save and restore.
#if !defined( SAVRES_SMALL_HXX )
#define SAVRES_SMALL_HXX
#include <stdio.h>
#ifdef sun4_cxx4 
#include <stdlib.h>
#endif
#include "dcl_kern.h"
#include "logical.h"
/**
 * @file savres_small.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISSAVERESTORE
 *
 * @{
 */

class restore_def;
class ENTITY;
class ENTITY_LIST;
class FileInfo;
class FileInterface;

/**
* Converts ENTITY indices back to pointers; use in FIX_POINTER_DEF.
* <br><br>
* <b>Role</b>: Used in FIX_POINTER_DEF (ENTITY::fix_common) during restore to convert
* ENTITY indices back into pointers.
* <br><br>
* Background: Entity pointers are converted to indices when saved. When restored,
* these indices are converted back into pointers in FIX_POINTER_DEF (ENTITY::fix_common).
* <br><br>
* The array argument is local data within FIX_POINTER_DEF. The second argument, i, indexes
* the array and returns the pointer.
* <br><br>
* @param array
* array of entities.
* @param i
* index of desired entity.
**/
DECL_KERN ENTITY * read_array( ENTITY *array[], int i );

/**
* Converts ENTITY indices back to pointers; use in FIX_POINTER_DEF.
* <br><br>
* <b>Role</b>: Used in FIX_POINTER_DEF (ENTITY::fix_common) during restore to convert
* ENTITY indices back into pointers.
* <br><br>
* Background: Entity pointers are converted to indices when saved. When restored,
* these indices are converted back into pointers in FIX_POINTER_DEF (ENTITY::fix_common).
* <br><br>
* The array argument is local data within FIX_POINTER_DEF. The second argument, SPAptr, is
* treated (read: casted) as an integral index used to index the array. Effectively, this call 
* is equivalent to the other version of <tt>read_array</tt>; this is provided for convenience.
* <br><br>
* @param array
* array of entities.
* @param p
* pointer (really an index) of desired entity.
**/
DECL_KERN ENTITY * read_array( ENTITY *array[], const void * p );

#ifndef NO_MESH_CLASSES

/*
// tbrv
*/
/**
 * @nodoc
 */

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

/**
* Read an ENTITY pointer.
* <br><br>
* <b>Role</b>: As part of the restore process, this function reads an ENTITY pointer using the
* currently installed <tt>FileInterface</tt>'s <tt>read_pointer</tt> method.
* <br><br>
* @see FileInterface, GetActiveFile, SetActiveFile, BinaryFile
**/
DECL_KERN ENTITY * read_ptr( void );

/**
* Write an ENTITY pointer.
* <br><br>
* <b>Role</b>: As part of the save process, this function writes an ENTITY pointer using the
* currently installed <tt>FileInterface</tt>'s <tt>write_pointer</tt> method.
* <br><br>
* @param ent
* Given entity pointer.
* @param list
* <tt>list</tt> is provided as local data within your <tt>SAVE_DEF</tt> implementation.
* <br><br>
* @see FileInterface, GetActiveFile, SetActiveFile, BinaryFile
*/
DECL_KERN void write_ptr( ENTITY * ent, ENTITY_LIST & list, FileInterface* = NULL );


// Routines to setup and reset save/restore operations
// to assist with some debug operations.

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN void begin_local_savres(
	FILE *,					// File to read or write
	int = -1,				// Major version (default = current)
	int = -1				// Minor version (default = current)
	);

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN void end_local_savres();

// Routine to set save file header info.

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN void set_file_info(
	unsigned long,		// Mask indicating which fields to update
	const FileInfo &	// Structure containing info to be updated
	);

// Routine to get header info for last restored file.

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN void get_file_info(FileInfo &);

// Routine to get the restoring_history variable from the context object.
// A routine is provided to break circular dependencies in the includes.

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN logical get_restoring_history(void);

// STI let (02/01): Added new global function
// Routine to get standard saver format flag
// Under normal circumstances this always returns TRUE.

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN logical get_standard_save_flag();

// JHS: Hook for decoupling savres
#include "savres.hxx"

/** @} */
#endif
