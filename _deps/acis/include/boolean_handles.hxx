/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// ------------------------------------------------------------------------------------------------
// Handles to the input to and output from Boolean operations.
// ------------------------------------------------------------------------------------------------

#ifndef BOOLEAN_HANDLES_HXX
#define BOOLEAN_HANDLES_HXX

// ------------------------------------------------------------------------------------------------

/**
 * @file boolean_handles.hxx
 */

// ------------------------------------------------------------------------------------------------
// Includes:

#include "acis.hxx"
#include "dcl_bool.h"

// ------------------------------------------------------------------------------------------------
// Forward decls:


// ------------------------------------------------------------------------------------------------

/**
 * Represents an abstract handle to the input or to the output of any ACIS operation.
 */

class DECL_BOOL acis_handle_base
{
public:

protected:

	/**
	 * @nodoc
	 */
	// Default constructor.

	acis_handle_base();

	/**
	 * @nodoc
	 */
	// Destructor.

	virtual ~acis_handle_base();
};

// ------------------------------------------------------------------------------------------------

/**
 * Represents an abstract handle to the output from a Boolean operation.
 * The user can have a shared ownership of this handle by increasing the reference count. 
 * After usage, the user must release the object by calling remove_ref().
 */

class DECL_BOOL boolean_output_handle : public acis_handle_base
{
public:
	/**
	 * Increases the reference count.
	 */

	virtual void add_ref() const = 0;

	/**
	 * Decreases the reference count.
	 */

	virtual void remove_ref() const = 0;

protected:

	/**
	 * @nodoc
	 */
	// Default constructor.

	boolean_output_handle();

	/**
	 * @nodoc
	 */
	// Destructor.

	virtual ~boolean_output_handle();
};

// ------------------------------------------------------------------------------------------------

/**
 * Represents an abstract handle to the inputs given to a Boolean operation.
 * Once created and passed to an API, the handle is fully owned by the API.
 */

class DECL_BOOL boolean_input_handle : public acis_handle_base
{

protected:

	/**
	 * @nodoc
	 */
	// Default constructor.

	boolean_input_handle();

	/**
	 * @nodoc
	 */
	// Destructor.

	virtual ~boolean_input_handle();

};

// ------------------------------------------------------------------------------------------------

/**
 * Represents a handle to the output from a Slice operation.
 * The user can have a shared ownership of this handle by increasing the reference count. 
 * After usage, the user must release the object by calling remove_ref().
 */

class DECL_BOOL slice_output_handle : public boolean_output_handle
{
protected:

	/**
	 * @nodoc
	 */
	// Default constructor.

	slice_output_handle();

	/**
	 * @nodoc
	 */
	// Destructor.

	virtual ~slice_output_handle();
};

// ------------------------------------------------------------------------------------------------

/**
 * Represents a handle to the output from a Imprint operation.
 * The user can have a shared ownership of this handle by increasing the reference count. 
 * After usage, the user must release the object by calling remove_ref().
 */

class DECL_BOOL imprint_output_handle : public boolean_output_handle
{
protected:

	/**
	 * @nodoc
	 */
	// Default constructor.

	imprint_output_handle();

	/**
	 * @nodoc
	 */
	// Destructor.

	virtual ~imprint_output_handle();
};

// ------------------------------------------------------------------------------------------------

/**
 * Represents a handle to the output from a Subdivide face operation.
 * The user can have a shared ownership of this handle by increasing the reference count. 
 * After usage, the user must release the object by calling remove_ref().
 */

class DECL_BOOL subdivide_face_output_handle : public boolean_output_handle
{
protected:

	/**
	 * @nodoc
	 */
	// Default constructor.

	subdivide_face_output_handle();

	/**
	 * @nodoc
	 */
	// Destructor.

	virtual ~subdivide_face_output_handle();
};

// ------------------------------------------------------------------------------------------------

#endif // BOOLEAN_HANDLES_HXX
