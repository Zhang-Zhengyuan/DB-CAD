/* $Id: insanity_list.hxx,v 1.12 2002/08/23 20:49:56 bloyd Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef INSANITY_LIST
#define INSANITY_LIST

#include <stdio.h>
#include <stdlib.h>
#include "errorbase.hxx"
#include "dcl_intr.h"
#include "mmgr.hxx"
#include "insanity_tbl.hxx"
#include "err_info.hxx"

class ENTITY;
class ENTITY_LIST;
class SPAtransf;
class SPApar_pos;
class insanity_list;
/**
* @file insanity_list.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

extern DECL_INTR option_header checker_limit;

/** \addtogroup ACISCHECKING
 *  \brief Declared at <insanity_list.hxx>, SPAintr
 *  @{
 */

/*
// tbrv
*/
/**
 * @nodoc
 */
typedef struct _msg_data {

	int			type;  // 1: int, 2: double, 3: string
	int			int_data;
	double		dbl_data;
	char		str_data[128];
	_msg_data	*next;

} msg_data;

/**
 * Severity of a check issue.
 * @param ERROR_TYPE
 * Check issue is an error.
 * @param WARNING
 * Check issue is serious, but not always an error.
 * @param NOTE
 * Check issue is noteworthy, but not always an error.
 * @param INFO
 * Check issue is purely informational.
 * @param ALL
 * Check issue fits into one of the above categories.
 */
typedef enum _etype {
	ERROR_TYPE,
	WARNING,
	NOTE,
	INFO,
	ALL
} insanity_type;


/**
 * Insanity data primary fail function.
 * @param ent
 * entity to check.
 * @param trans
 * transform.
 * @param list
 * insanity list.
 */
typedef int (*primary_fail_fn)( ENTITY * ent, const SPAtransf * trans, insanity_list * list );


/**
 * Insanity data secondary fail function.
 * @param ent
 * entity to check.
 */
typedef insanity_list * (*subsequent_fail_fn)( ENTITY * ent );


/**
 * This class holds information about problems (insanities) found when checking an ACIS model.
 * <br>
 * <b>Role:</b> As an ACIS model is checked, a list of problems or insanities is
 * generated.  The information about each insanity is stored an <tt>insanity_data</tt>
 * object.  The <tt>insanity_data</tt> objects are stored and accessed using the
 * <tt>insanity_list</tt> class.  The <tt>insanity_data</tt> class provides member methods for
 * accessing information about the insanity.
 * @see insanity_list, ENTITY
 */
class DECL_INTR insanity_data : public error_info
{
private:
	ENTITY		*ent;				// insane entity
	/// int			insane_id;		// insanity number - derived from base class
	insanity_type	etype;		// insanity type
	insanity_sub_category sub_category;
	SPApar_pos* uv_parameter;
    bool  owns_error_entity;

protected:
	
    insanity_data( insanity_data const& orig );         
    insanity_data &operator=( insanity_data const& rhs );

	// Destructor
    virtual ~insanity_data();

public:
	/**
	 * @nodoc
	 */
	int (*chk_func)( ENTITY*, const SPAtransf*, insanity_list* );	// pointer to failed primary function

	/**
	 * @nodoc
	 */
	insanity_list* (*sub_chkfn)( ENTITY* );						// pointer to failed subsequent function

	/**
	 * @nodoc
	 */
	char		*aux_msg;

	/**
	 * @nodoc
	 */
	msg_data	*aux_data;

	/**
	 * C++ allocation constructor requests memory for this object but does not populate it.
	 */
	insanity_data();

	/**
	 * @nodoc
	 */
    insanity_data(ENTITY *e,
				  int id,
				  insanity_type type,
				  primary_fail_fn pfunc,
				  subsequent_fail_fn sub_pfunc,
		          const insanity_sub_category& sub,
				  char *mesg,
				  msg_data *data
				  );
	/**
	 * @nodoc
	 */
	insanity_data(ENTITY *e,
				  int id,
				  insanity_type type,
				  primary_fail_fn pfunc,
				  subsequent_fail_fn sub_pfunc,
		          const insanity_sub_category& sub,
				  char *mesg,
				  ...
				  );

	/**
	 * Makes an insanity data.
	 * <br><br>
	 * @param e
	 * entity to check.
	 * @param id
	 * insanity id.
	 * @param type
	 * type of insanity.
	 * @param pfunc
	 * primary function.
	 * @param sub_pfunc
	 * subsequent function.
	 * @param sub_category
	 * insanity sub category - default is NO_SUB_CATEGORY.
	 */
	insanity_data(ENTITY *e,
				  int id,
				  insanity_type type,
				  primary_fail_fn pfunc = NULL,
				  subsequent_fail_fn sub_pfunc = NULL,
		          const insanity_sub_category& sub_category = NO_SUB_CATEGORY
		  );

	/**
	 * Set the parameter position property for this instance.
	 * @param uv
	 * The parameter position to set.
	 */
	void set_par_pos(const SPApar_pos& uv);

	/**
	 * Gets <tt>ENTITY</tt>.
	 */
	ENTITY* get_ent() const;

	/**
	 * Gets <tt>insane_id</tt>. The insanity message string can be 
	 * obtained from <tt>insanity_data::get_message()</tt>.
	 */
	int get_insane_id() const;

	/**
	 * Returns the sub-category for this instance.
	 */
	insanity_sub_category get_sub_category() const;

	/**
	 * Gets insanity type.
	 */
	insanity_type get_type() const;

	/** 
	 * Sets uv to the related surface parameter position for the
	 * error, if relevant. Returns <tt>TRUE</tt> if a relevant parameter
	 * position exists, otherwise <tt>FALSE</tt>. The uv position is
	 * undefined if the method returns FALSE.
	 * @param uv
     * (Output) parameter position for error.
	 */
	logical get_par_pos(SPApar_pos& uv) const;

    /**
	 * @nodoc
	 */
    bool get_owns_error_entity() const { return owns_error_entity; }

    /**
	 * @nodoc
	 */
    void set_owns_error_entity( bool owns ) { owns_error_entity = owns; }

	/**
	 * Prints the message into the file.
	 * <br><br>
	 * @param fptr
	 * file pointer.
	 */
	void print_message(FILE *fptr) const;

	/**
	 * @nodoc
	 */
	 //Re-checks the sanity of the entity with the failed check function.
	
	insanity_list* recheck();

	/**
	 * Gets the error or insanity message.
	 */
	const char* get_message();

	/**
	 * Gets the auxiliary message (with any parameters filled in) as a new string.
	 * The caller is responsible for deleting the returned string. If no auxiliary
	 * message is present, a null pointer is returned.
	 * <br><br>
     * Typically, a non-null auxiliary message will contain tab and newline
     * characters, and may be broken over several lines.
	 */
	char* get_aux_msg() const;
};

// This class is used to implement a linked list of problems (insanities) that are found when
// checking a model.  Each problem is described by an insanity_data contained in the list.  Also
// each insanity_list contains a pointer to another insanity_list which allows the formation
// of the list.
/**
 * Implements a linked list of problems (insanities) that are found when checking a model.
 * <br>
 * <b>Role:</b> This class is used to implement a linked list of problems
 * (insanities) that are found when checking a model. Each problem is described
 * by an <tt>insanity_data</tt> contained in the list. Also each <tt>insanity_list</tt> contains
 * a pointer to another <tt>insanity_list</tt> which allows the formation of the list.
 */
class DECL_INTR insanity_list : public ACIS_OBJECT
{
public:
	// Constructor
	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * @param data
	 * insanity data.
	 * @param next
	 * pointer to next insanity data.
	 */
	insanity_list(insanity_data *data = NULL, insanity_list *next = NULL, int error_count=0);

	// Destructor
	/**
	 * Deletes an <tt>insanity_list</tt>.
	 */
	~insanity_list();

	// Return the insanity_data.
	/**
	 * Returns the <tt>insanity_data</tt>.
	 */
	insanity_data* data();

	/**
	 * Returns the <tt>insanity_data</tt>.
	 */
	const insanity_data* data() const;

	// Return next list.
	/**
	 * Returns the next list.
	 */
	insanity_list* next();

	/**
	 * Returns the next list.
	 */
	const insanity_list* next() const;

	// Return pointer
	/**
	 * Returns a pointer to this insanity list.
	 */
	insanity_list* output();

	// Print insanity messages.
	/**
	 * Prints the insanity messages.
	 * <br><br>
	 * @param fp
	 * file containing message.
	 * @param itype
	 * type of insanity.
	 */
	void print_messages(FILE* fp, insanity_type itype = ALL) const;

	// Append aux message to the last insanity_data in the list.
	// NB - explicitly cast input arguments to simple data types, e.g., SPAparameter ==> double
	/*
	// tbrv
	*/
	/**
	 * @nodoc
	 */
	void append_aux_msg(char*, ...);

	// Count insanity
	/**
	 * Counts the number of instances of insanity.
	 * <br><br>
	 * @param itype
	 * type of insanity.
	 */
	int count(insanity_type itype = ALL);
	
	// Add insane entities to the given ENTITY_LIST
	/**
	 * Adds insane entities to the given <tt>ENTITY_LIST</tt>.
	 * <br><br>
	 * @param insane_ents
	 * entity list of insanities that are of the same type.
	 * @param itype
	 * type of insanity.
	 */
	void make_entity_list(ENTITY_LIST* insane_ents, insanity_type itype = ALL);

	// Add insane entities to the given ENTITY_LIST
	/**
	 * Adds insane entities to the given <tt>ENTITY_LIST</tt>.
	 * <br><br>
	 * @param insane_ents
	 * entity list of insanities that are of the same type.
	 * @param insanity_id
	 * insanity id.
	 * @param itype
	 * type of insanity.
	 */
	void make_entity_list(ENTITY_LIST& insane_ents, int insanity_id, insanity_type itype = ALL);

	// Check if a specific insanity exists in the lists.
	/**
	 * Checks if a specific insanity exists in the lists.
	 * <br><br>
	 * @param insanity_id
	 * insanity id.
	 */
	logical exist(int insanity_id);

    /**
    * Checks if a specific insanity exists in the list.
    * 
    * @param ent         ENTITY related to given insanity_id
    * @param insanity_id insanity id.
    * @param type        insanity_type.
    */
    logical exist(const ENTITY* ent, int insanity_id, insanity_type type = ERROR_TYPE );

    /**
    * Checks if a specific insanity exists in the list.
    * 
    * @param ent         ENTITY to find
    */
    logical exist( const ENTITY* insane_ent );

	// Re-check the sanity of entities only with the failed check functions.
	/**
	 * Re-checks the sanity of entities only with the failed check functions.
	 * <br><br>
	 * @param itype
	 * type of insanity.
	 */
	insanity_list* recheck(insanity_type itype = ALL);
	
	// Internal Use Only!
	/**
	 * @nodoc
	 */
	void add_insanity(ENTITY *e,
					  int id,
					  insanity_type type,
					  int (*pfunc)( ENTITY*, const SPAtransf*, insanity_list* ) = NULL,
					  insanity_list* (*sub_pfunc)( ENTITY* ) = NULL,
			          const insanity_sub_category& sub_category = NO_SUB_CATEGORY
					  );
	/**
	 * @nodoc
	 */
	void add_insanity(insanity_list *new_list);
	
	/**
	 * @nodoc
	 */
	void add_insanity(insanity_data *data);
	
	/**
	* @nodoc
	*/
	insanity_list* tail();
	
	/**
	 * @nodoc
	 */
	struct node;

private:
	node *_node;
};

/**
 * Adds all the <tt>insanity_data</tt> objects from the <tt>insanity_list *ilist</tt> into the 
 * given <tt>error_info_list &target_list</tt> object.
 * <br><br>
 * @param ilist
 * insanity_list that has to converted to error_info_list
 * @param target_list
 * error_info_list to hold the insanity_data objects from ilist
 */
DECL_INTR void convert_insanity_list_into_error_info_list(const insanity_list *ilist, 
														  error_info_list &target_list);

#endif

