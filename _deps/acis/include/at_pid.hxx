/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header file for the persistent identifier data attribute
// This data is contained in the pid_base object
// This attribute is derived from the ATTRIB_SG

// This is mostly standard attribute definition stuff

#if !defined( PID_ATTRIB )
#define PID_ATTRIB

#include <time.h>

#include "acis.hxx"
#include "dcl_pid.h"

#include "at_sg.hxx"

#include "pid.hxx"

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_PID, PID )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
DECL_PID logical pid_attrib_save( const ATTRIB_PID*, pid_base& pid_out );
DECL_PID logical pid_attrib_restore( const pid_base* bn_in, ATTRIB_PID* );
DECL_PID logical pid_attrib_copy( const ATTRIB_PID* from,  ATTRIB_PID* to  );

extern DECL_PID int ATTRIB_PID_TYPE;
#define ATTRIB_PID_LEVEL (ATTRIB_SG_LEVEL + 1)

#define PID_BASE_NAME_LENGTH 8

class DECL_PID ATTRIB_PID: public ATTRIB_SG {
private:

	pid_base Pid;  // the persistent id data itself

private:
	// Field notification of rollbacks, to allow maintenance of
	// a list of outstanding objects of this class, used for (semi-)
	// automatic cleanup of decorated bodies.

//	void roll_notify( BULLETIN_TYPE, ENTITY * );

public:
	ATTRIB_PID(
				ENTITY*        own = NULL
				);

	ATTRIB_PID(
				ENTITY*         own,
				const pid_base& pid
				);

    virtual logical pattern_compatible() const;
                                    // returns TRUE 
	const pid_base* get_pid() const;

	const char*     get_base_name() const;
	long            get_time_val()  const;
	int             get_index()     const;
	int             get_copy_num()  const;


// the user may replace these functions with one of
//  their own creation to control attribute migration

	virtual void split_owner( ENTITY *);
	virtual void merge_owner( ENTITY *, logical );

	friend DECL_PID logical pid_attrib_save(
									const ATTRIB_PID*,
									pid_base&     pid_out
					    			);
	friend DECL_PID logical pid_attrib_restore(
									const pid_base* bn_in,
									ATTRIB_PID*
					    			);

	friend DECL_PID logical pid_attrib_copy(
									const ATTRIB_PID* from, 
									ATTRIB_PID* to 
					    			);

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_PID, PID )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

DECL_PID const ATTRIB_PID*  get_pid_attrib( const ENTITY*);
DECL_PID logical            got_pid_attrib( const ENTITY*);

#endif
