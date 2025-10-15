/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header file for the persistent identifier data
// This data is contained in the ATTRIB_PID attribute

#if !defined( PID_HXX )
#define PID_HXX

#include <time.h>
#include "dcl_pid.h"
#include "logical.h"
#include "mmgr.hxx"

/**
 * @nodoc
 * @file pid.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup PERSISTIDAPI
 *
 * @{
 */

#define PID_BASE_NAME_LENGTH 8

class ATTRIB_PID;
class ENTITY;
class pid_base;

/**
 * @nodoc
 */
DECL_PID void pid_base_init(pid_base* );

/**
 * @nodoc
 */
DECL_PID const pid_base* get_next_current_pid();

//
//   The ACIS persistent identifier (pid) consists of the following:
//
//   	 1. - A eight character session user identifier.
//
//   	 2. - The time of the session start
//
//   	 3. - The index of this identifier within this session.
//
//   	 4. - A copy index for this identifier.
//
/**
 * @nodoc
 * Defines the persistent identifier data (<tt>PID</tt>).
 * <br>
 * <b>Role:</b> The ACIS PID consists of the following items:<ol>
 * <li>An 8-character session user <tt>ID</tt>.</li>
 * <li>The time of the session start.</li>
 * <li>The index of this identifier within this session.</li>
 * <li>A copy index for this identifier.</li></ol>
 */
class DECL_PID pid_base : public ACIS_OBJECT
{

private:

// the user identifier is null character terminated

	char      base_name[PID_BASE_NAME_LENGTH];  // user id

	long      time_val;  // time in seconds from jan 1, 1970.

	int       index;     // entity index

	int       copy_num;  // copy number

public:
 /**
 * C++ allocation constructor requests memory for this object but does not populate it.
 */
	pid_base();
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param p_base
 * bitwise copy.
 */
	pid_base( const pid_base* p_base );                   // bitwise copy
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param nm
 * session name.
 * @param tm
 * session time.
 * @param index
 * entity index.
 * @param copy_num
 * copy number.
 */
	pid_base(
				  const char*    nm,
				  const long     tm,
				  const int      index,
				  const int      copy_num = 1
				  );
/**
 * Saves the base name, time value, index, and copy number.
 */
	void  		  save() const;
/**
 * Restores the <tt>pid_base</tt> from a file.
 * <b>Role:</b> <br>
 * <pre>
 * if (restore_version_number &lt; CONSISTENT_VERSION)
 * 		read_int			  //Number of characters
 *    for( i = 0; i &lt; numchars; i++ )
 * 		read_int                 	  //read in a character at a time
 * else
 * 		read_string		           //Read in the string
 * 		read_long			   //time value in seconds from January 1, 1970
 * 		read_int		      	   //Entity index
 * 		read_int			   //copy number
 * </pre>
 */
	void  		  restore();
/**
 * Returns the pid session user name for an <tt>ENTITY</tt>, or <tt>NULL</tt> if none exists.
 */
	const char*   get_base_name() const  { return base_name; };
/**
 * Returns the time of session start for the <tt>ENTITY's</tt> identifier, measured in seconds since January <tt>1</tt>, <tt>1970</tt>, or zero if none exists.
 */
	long          get_time_val()  const  { return time_val; };
/**
 * Returns the index of the eight character session user identifier, or zero if none exists.
 */

	int           get_index()     const  { return index; };
/**
 * Returns the copy number of the <tt>ENTITY's</tt> identifier, or zero if none exists.
 */
	int           get_copy_num()  const  { return copy_num; };
/**
 * Sets the <tt>pid_base</tt> to an existing one.
 *<br><br>
 * @param p_base
 * bit-wise copy.
 */
	void set_pid_base( const pid_base* p_base);
/**
 * Sets the <tt>pid_base</tt>, with all the necessary information.
 *<br><br>
 * @param nm
 * session name.
 * @param tm
 * session time.
 * @param index
 * entity index.
 * @param copy_num
 * copy number.
 */
	void set_pid_base(
				  const char*    nm,
				  const long     tm,
				  const int      index,
				  const int      copy_num = 1
				  );

// to allow replacement of the statup initialization process

/**
 * @nodoc
 */
friend DECL_PID void pid_base_init( pid_base* );

// to get the next current id for this session

/**
 * @nodoc
 */
friend DECL_PID const pid_base* get_next_current_pid();

};

// to set the next current id for this entity, false on failure

/**
 * @nodoc
 */
DECL_PID logical            set_next_current_pid( ENTITY* );

// to remove the id for this entity
/**
 * @nodoc
 */

DECL_PID void               remove_pid( ENTITY* );

// to get the pid_base for this entity, returns NULL if none
/**
 * @nodoc
 */
DECL_PID const pid_base*    get_pid( const ENTITY*);

// to get the session user name for this entity, returns NULL if none
/**
 * @nodoc
 */
DECL_PID const char*        get_base_name( const ENTITY*);

// to get the session time for this entity, returns zero if none
/**
 * @nodoc
 */
DECL_PID long               get_time_val( const ENTITY*);

// to get the index for this entity, returns zero if none
/**
 * @nodoc
 */
DECL_PID int                get_index( const ENTITY*);

// to get the copy number for this entity, returns zero if none
/**
 * @nodoc
 */
DECL_PID int                get_copy_num( const ENTITY*);

// insures that the 8 character
// (null character included) limit is not violated for the base_name
// field and that a null character terminates the name.

/**
 * @nodoc
 */
DECL_PID void               create_pid_base_name(
	               						const char*   nm,
	               						char*         bn);
/** @} */
#endif
