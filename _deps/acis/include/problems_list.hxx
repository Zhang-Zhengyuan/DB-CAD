/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef PROBLEMS_LIST_HXX
#define PROBLEMS_LIST_HXX

class problems_list;
class outcome;
// Global function to add a problem to the current problems_list
// This can be used to register problems which are warnings rather
// than fatal or failsafe errors.  It can also be used in cases where
// failsafe behavior cannot be implemented using the API_FAILSAFE_BEGIN/END
// macros.
//#include "api.hxx"
#include "dcl_kern.h"
/**
 * @nodoc
 */
DECL_KERN void add_problem_to_current_problems_list( const error_info* ei );

// Problems list propagation type
/**
 * @nodoc
 */
enum problems_list_prop_type
{
	PROBLEMS_LIST_PROP_IGNORE,				// don't propagate the problems list
	PROBLEMS_LIST_PROP_ONLY,				// always propagate the problems list and don't add any failsafe errors
	PROBLEMS_LIST_PROP_OR_IGNORE,			// depending on outcome, propagate the problems list or don't, don't add failsafe errors
	PROBLEMS_LIST_PROP_OR_ADD_PROBLEM		// depending on outcome, propagate the problems list or add a new failsafe error
};

// Class to handle problems list propagation
/**
 * @nodoc
 */
class DECL_KERN problems_list_prop : public ACIS_OBJECT
{
	friend DECL_KERN void add_problem_to_current_problems_list( const error_info* ei );

	problems_list_prop* higher_level_prop;	// problems_list propagation object from higher level API block
	problems_list* current_list;			// problems_list from current API block

public:
	// Constructor creates the current problems_list list and pushes it onto the
	// problems_list propagation stack
	problems_list_prop();

	// Destructor ensures propagation stack is unwound (e.g. if returning
	// from inside of an API_* block)
	~problems_list_prop();

	// Main routine that handles problems_list propagation and adding problems
	// depending on the result and propagation type.  This routine always unwinds
	// the problems_list propagation stack.
	//
	// If prop_type == PROBLEMS_LIST_PROP_IGNORE, this routine does nothing (behavior
	// of the API_NOP_BEGIN/END block).
	//
	// If prop_type == PROBLEMS_LIST_PROP_ONLY, this routine will always propagate
	// the problems_list regardless of outcome indicates success or not (behavior of
	// API_BEGIN/END and API_SYS_BEGIN/END blocks).
	//
	// If prop_type == PROBLEMS_LIST_PROP_OR_IGNORE, this routine will only propagate
	// the problems list if the outcome was successful, otherwise no action is taken
	// (behavior of API_TRIAL_BEGIN/END blocks).
	//
	// If prop_type == PROBLEMS_LIST_PROP_OR_ADD_PROBLEM, this routine will only propagate
	// the information if the outcome was successful, otherwise a new problem is added to
	// the problems_list or a sys_error is thrown.  A problem is added only if failsafe
	// behavior is on globally and the failsafe_on flag is TRUE, otherwise a fatal error 
	// is thrown (behavior of API_FAILSAFE_BEGIN/END blocks).
	void process_result( outcome& result, problems_list_prop_type prop_type, logical failsafe_on );

private:
	// Add a problem to the current problems list
	void add_problem_to_current_list( const error_info* ei );

	// Merge a list of problems with the current list
	// If the current list has not been created, this routine will
	// simply transfer ownership of problems to the propagation object
	// and set problems to NULL.
	void merge_problems_with_current_list( problems_list*& problems );

	// Merge the current problems_list with the outcome object and merge it
	// with the higher level problems_list
	void propagate_current_list( outcome& result );
};

#endif		// PROBLEMS_LIST_HXX


