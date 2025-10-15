/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Main definition file for ACIS error information return objects.
// Objects derived from this base class are used to return information
// in the outcome class. These objects are specifically designed so
// that APIs can automatically return additional information to the user
// following an error by simply changing the sys_error call.

#if !defined( ERROR_INFO_BASE )
#define ERROR_INFO_BASE

#include "base.hxx"
#include "dcl_base.h"

//#define GME_BASE_ERR_INFO_BASE

/**
 * @file err_info_base.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISERRORMANAGEMENT
 *
 * @{
 */

#ifndef NULL
/**
 * @nodoc
 */
#define NULL 0
#endif


/**
 * An integer identifier that maps to a string.
 * <br>
 * <b>Role:</b> This is the type passed to <tt>sys_error</tt> and <tt>sys_warning</tt> to signify a particular error.
 * @see sys_error, sys_warning
 */
typedef int err_mess_type;


/**
 * @nodoc
 */
class error_info_base_list;
class error_info;

/**
 * @nodoc
 */
#define SPA_NO_ERROR  0

/**
 * Enum for specifying different severities of an error that is
 * represented by an <tt>error_info</tt> object.
 * <dl><dd>
 * <table border=1 cellpadding=4 cellspacing=0 bordercolor=navy>
 * <tr><td><tt>SPA_OUTCOME_FATAL</tt></td><td>Represents a fatal error, which would cause the operation
 * that encountered the error to roll back. An API function should only add a
 * single error_info with severity <tt>SPA_OUTCOME_FATAL</tt>, 
 * since the first one encountered causes the API function to roll.</td></tr>
 * <tr><td><tt>SPA_OUTCOME_ERROR</tt></td><td>Represents an error encountered by a 
 * failsafe routine during an atomic operation.  If the <tt>careful</tt> option is
 * set to <tt>FALSE</tt> (default), then the API function will continue to other atomic
 * operations which may also result in other error_info objects of 
 * severity <tt>SPA_OUTCOME_ERROR</tt>.  Note that this severity cannot occur 
 * when failsafe behavior is turned off (by setting the <tt>careful</tt> option to <tt>TRUE</tt>),
 * since such errors have severity <tt>SPA_OUTCOME_FATAL</tt> in that case.</td></tr>
 * <tr><td><tt>SPA_OUTCOME_PROBLEM</tt></td><td>Represents a problem encountered during an 
 * operation. This category of errors are less severe compared to <tt>SPA_OUTCOME_FATAL</tt>
 * and <tt>SPA_OUTCOME_ERROR</tt>.</td></tr>
 * <tr><td><tt>SPA_OUTCOME_INSANITY</tt></td><td>Represents an insanity in the model.</td></tr>
 * </table>
 * </dd></dl>
 */
typedef enum
{
	SPA_OUTCOME_FATAL = 0,
	SPA_OUTCOME_ERROR,
	SPA_OUTCOME_PROBLEM,
	SPA_OUTCOME_INSANITY
} spa_outcome_severity_type;

/**
 * Defines objects for returning ACIS error information.
 * <br>
 * <b>Role:</b> Objects derived from this base class are used to return information in the
 * <tt>outcome</tt> class. These objects are specifically designed so that following an error, API functions
 * can automatically return additional information to the user, simply by changing the
 * <tt>sys_error</tt> call.
 * This class stores the error number, error severity, and reasons for the error.
 * Every reason is an instance of <tt>error_info_base</tt> or of
 * a class derived from <tt>error_info_base</tt>.
 * <br><br>
 * Although no restriction is placed on the information you may attach to <tt>error_info_base</tt>-derived objects,
 * you should be aware that new <tt>ENTITY</tt>s will be lost during roll back, meaning that
 * pointers to such objects will become stale.  The derived <tt>error_info</tt> class handles this
 * problem by returning only live entities via its <tt>get_entities_alive</tt> method.
 *
 * @see spa_outcome_severity_type, error_info, error_info_list, error_info_base_list
 */
class DECL_BASE error_info_base : public ACIS_OBJECT {

private:
	// use count data
    int use_count;

protected:
	// Error number
	err_mess_type m_err_num;

	// Severity
	spa_outcome_severity_type m_severity;

	// Reasons
	error_info_base_list *m_reasons;
	
	// C++ destructor, deleting an error_info_base.
	virtual ~error_info_base();

	// Returns the number of entity ids that own the error represented by this <tt>error_info_base</tt>.
	virtual int get_entity_id_count() const;

	virtual error_info* error_info_cast_vf();
	virtual void add_reason_vf(error_info_base* ei);
	virtual void add_reasons_vf(const error_info_base_list& eil);

public:

	/**
	 * C++ default constructor to construct an <tt>error_info_base</tt> object.
	 */
    error_info_base();

	error_info_base(char const * gme);
	/**
	 * C++ constructor to construct an <tt>error_info_base</tt> object specified by
	 * an error number and severity.
	 * <br><br>
	 * @param err_mess
	 * <tt>err_mess_type</tt> indicating the error.
	 * @param err_severity
	 * error severity
	 */
	error_info_base(err_mess_type err_mess, spa_outcome_severity_type);

	error_info_base(char const * gme,err_mess_type err_mess, spa_outcome_severity_type spa_severity);
	/**
	 * Increments the use count. Customers should only have to
	 * pay attention to use counting if they explicitly call <tt>add</tt>.
	 */
	void gme_add();

	#ifdef GME_BASE_ERR_INFO_BASE
	void add() {
		gme_add();
	}
	#else
	void add();
	#endif

	/**
	 * Decrements the use count. Customers should only have to
	 * pay attention to use counting if they explicitly call <tt>add</tt>.
	 */
	void gme_remove();
	#ifdef GME_BASE_ERR_INFO_BASE
	void remove() {
		gme_remove();
	}
	#else
	void remove();
	#endif
	/**
	 * Returns the id number for the class <tt>error_info_base</tt>.
	 */
    static int id();

	/**
	 * Returns the id number for whichever class this object happens to be. If this
	 * object is of the class <tt>error_info</tt>, then the id number returned by this function
	 * would be equal to the one returned by <tt>error_info::id</tt>.
	 */
    virtual int type() const;
	virtual int gme_type() const;
	error_info* error_info_cast();

	/**
	 * Returns the error number of the error that this <tt>error_info_base</tt> object is
	 * encapsulating.
	 */
	err_mess_type error_number() const;
	err_mess_type gme_error_number() const;
	
	/**
	 * Returns the message string of the error that this <tt>error_info_base</tt> object is
	 * encapsulating.
	 */
	char const* error_message() const;
	char const* gme_error_message() const;
	/**
	 * Returns the severity of the error that this <tt>error_info_base</tt> object is
	 * encapsulating.
	 */
	spa_outcome_severity_type severity() const;
	spa_outcome_severity_type gme_severity() const;
	/**
	 * Returns <tt>TRUE</tt> if this <tt>error_info_base</tt> object has at least one reason; returns <tt>FALSE</tt> otherwise.
	 */
	logical has_reasons() const;
	logical gme_has_reasons() const;
	/**
	 * Adds all the reasons of this <tt>error_info_base</tt> into the given list, <tt>err_reasons</tt>.
	 * In some cases, an <tt>error_info_base</tt> may need to provide some additional information 
	 * about the errors or problems that would have caused the error that is indicated
	 * by this <tt>error_info_base</tt> object. These additional errors and problems are stored as reasons.
	 * For example, an <tt>error_info_base</tt> object encapsulating the error 'Operation
	 * failed due to check errors' might include the list of check errors in the
	 * 'reasons' list.
	 * <br><br>
	 * @param err_reasons
	 * <tt>error_info_base_list</tt> to which the reasons are added.
	 */
	void reasons(error_info_base_list& err_reasons) const;
	void gme_reasons(error_info_base_list& err_reasons) const;
	/**
	 * @nodoc
	 */
	// This method is for internal use only
	// Sets the error number
	void set_error_number(err_mess_type err_mess);
	void gme_set_error_number(err_mess_type err_mess);
	/**
	 * @nodoc
	 */
	// This method is for internal use only
	// Sets the severity of the error
	void set_severity(spa_outcome_severity_type err_severity);
	void gme_set_severity(spa_outcome_severity_type err_severity);
	/**
	 * @nodoc
	 */
	// This method is for internal use only
	// Adds a reason to this error_info_base
	void add_reason(error_info_base* ei);
	void gme_add_reason(error_info_base* ei);
	/**
	 * @nodoc
	 */
	// This method is for internal use only
	// Adds a list of reasons
	void add_reasons(const error_info_base_list& eil);
	void gme_add_reasons(const error_info_base_list& eil);
	/**
	 * @nodoc
	 */
	// This function is for internal use only
	// Friend function to return the entity id count of error_info_base
	friend int get_error_info_entity_id_count_internal(const error_info_base *);

};


// used to assign unique id to object derived from error_info_base objects
/**
 * @nodoc
 */
DECL_BASE void assign_error_type_no( int * etn );

// Function to set (and reset) global error info pointer.  This function
// is called in API_SYS_BEGIN to automatically pass an error_info_base
// object through a longjmp.
/**
 * @nodoc
 */
DECL_BASE void set_global_error_info( error_info_base *e = NULL );
DECL_BASE void gme_set_global_error_info( error_info_base *e = NULL );
// Function to read global error info pointer.  Called in API_SYS_BEGIN
// after an error return to automatically put the error info
// object into the API outcome.
/**
 * @nodoc
 */
DECL_BASE error_info_base* global_error_info();
DECL_BASE error_info_base* gme_global_error_info();
/** @} */
#endif

