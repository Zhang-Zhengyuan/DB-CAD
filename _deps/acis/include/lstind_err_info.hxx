/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( LSTIND_ERR_INFO )
#define LSTIND_ERR_INFO
#include "acis.hxx"
#include "err_info.hxx"
#include "errorsys.hxx"
/**
 * @file lstind_err_info.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISERRORMANAGEMENT
 *
 * @{
 */

/**
 * Class for storing list-based ACIS error information.
 * <br>
 * <b>Role:</b> This class is derived from <tt>error_info</tt> and additionally stores an integer array.
 * <tt>list_index_error_info</tt> is specifically designed to hold an array of integers
 * in addition to the error number and severity that is represented by <tt>error_info</tt>.
 * This class can be used by those APIs that want to return error information specifying a list of integers
 * in their <tt>outcome</tt>; for instance, in order to notify failing members of an input list or array.
 *
 * @see error_info, spa_outcome_severity_type, outcome
 */
class DECL_KERN list_index_error_info : public error_info

{
    // Number of error indices
    int num_error_indices;

    // List of error indices
    int* error_indices;

protected:
    // Destructor
	virtual ~list_index_error_info();

public:
    
    /**
	 * C++ constructor to construct an <tt>list_index_error_info</tt> object with the 
	 * specified error number, error severity and a list of entities.
	 * <br><br>
	 * @param err_mess
	 * err_mess_type indicating the error.
	 * @param err_severity
	 * error severity
	 */
    list_index_error_info(int num_error_indices, int* error_indices, err_mess_type err_mess, spa_outcome_severity_type err_severity);
    list_index_error_info(const char* gme, int num_error_indices, int* error_indices, err_mess_type err_mess, spa_outcome_severity_type err_severity);
    /**
	 * Returns the list of error indices. 
	 */
    int const* get_error_indices() const;
    int const* gme_get_error_indices() const;

    /**
	 * Returns the number of error indices. 
	 */
    int get_num_error_indices() const;
    int gme_get_num_error_indices() const;
    /**
	 * Returns the id number for the class <tt>list_index_error_info</tt>.
	 */
    static int id();

    /**
	 * Returns the id number for whichever class this object happens to be. If this
	 * object is of the class <tt>list_index_error_info</tt>, then the id number returned by this function
	 * would be equal to the one returned by <tt>list_index_error_info::id()</tt>.
	 */
    virtual int gme_type() const;
    virtual int type() const;
};

/** @} */
#endif
