/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

/*******************************************************************/

#ifndef AUX_DATA_HOLDER_HXX
#define AUX_DATA_HOLDER_HXX
// ------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------
// Includes:

#include <string>
#include "acis.hxx"
#include "dcl_kern.h"

// ------------------------------------------------------------------------------------------------
// Forward decls:

class i_data_element;
class i_data_set;
class i_data_set_list;

// ------------------------------------------------------------------------------------------------

/**
 * @file aux_data_holder.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISERRORMANAGEMENT
 *
 * @{
 */

// ------------------------------------------------------------------------------------------------

/**
 * Interface to hold heterogenous data with appropriate descriptions. This is
 * used by the class error_info to provide auxiliary data.
 * <br>
 * <b>Role:</b> This interface holds a classified collection of data elements along
 * with their descriptions. It also publishes a list of descriptions of the contents 
 * that it enumerates. The life-time is managed by reference count.
 *
 * @see i_data_set_list, error_info, outcome
 */

class DECL_KERN i_aux_data_holder
{	

public:

	/**
	 * Provides an array containing descriptions of the data held by this object. 
	 * The array of names will be allocated on free store and the memory cleanup
	 * is responsibility of the caller. Returns the number of descriptions provided.
	 */

	virtual size_t get_descriptions( char const **&desc ) const = 0;

	/**
	 * Provides a list of data-sets corresponding to the given description. Returns NULL
	 * if the given description is not recognized by this interface. The list returned
	 * is owned by this interface, so shouldn't be deleted.
	 */

	virtual i_data_set_list const* get_elements( char const *desc ) const = 0;


	/**
	 * @deprecated R2016 1.0. Use get_descriptions( char **&desc ) instead.
	 * Provides an array containing descriptions of the data held by this object. 
	 * The array of names will be allocated on free store and the memory cleanup
	 * is responsibility of the caller. Returns the number of descriptions provided.
	 */

	virtual size_t get_descriptions( std::string *&desc ) const = 0;
	
	/**
	 * @deprecated R2016 1.0. Use get_elements( char const *desc ) instead. 
	 * Provides a list of data-sets corresponding to the given description. Returns NULL
	 * if the given description is not recognized by this interface. The list returned
	 * is owned by this interface, so shouldn't be deleted.
	 */

	virtual i_data_set_list const* get_elements( std::string const &desc ) const = 0;

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
	// Destructor

	virtual ~i_aux_data_holder(){}
};
//

/**
 * Interface to a list of data sets.
 * <br>
 * <b>Role:</b> This interface holds a list of data sets and provides a mechanism to
 * iterate over all its members. Every member of the list is of type "i_data_set".
 * The members of the list are owned by the list, so shouldn't be deleted.
 *
 * @see i_data_set, i_aux_data_holder, error_info, outcome
 */

class DECL_KERN i_data_set_list
{
public:

	/**
	 * Returns the first data set in the list. Also, resets the iterator to point to
	 * the first set.
	 */
	
	virtual i_data_set* first() const = 0;

	/**
	 * Returns the next set in the list. If called after first(), returns the second set.
	 * Returns NULL if the iterator has reached the end of the list.
	 */

	virtual i_data_set* next() const = 0;

	/**
	 * Returns the number of data sets provided by this list.
	 */

	virtual size_t count() const = 0;

protected:

	/**
	 * @nodoc
	 */
	// Destructor

	virtual ~i_data_set_list(){}
};
//


/**
 * Interface to a set of data elements.
 * <br>
 * <b>Role:</b> Provides an interface to a set of data elements. Every member of the 
 * set is of type 'i_data_element'. The members of the set are owned by the list, 
 * so shouldn't be deleted.
 *
 * @see i_data_element, i_data_set_list, error_info, outcome
 */

class DECL_KERN i_data_set
{
public:

	/**
	 * Returns the first data element in the set. Also, resets the iterator to point to
	 * the first element.
	 */
	
	virtual i_data_element* first() const = 0;

	/**
	 * Returns the next data element in the set. If called after first(), returns the second element.
	 * Returns NULL if the iterator has reached the end of the set.
	 */

	virtual i_data_element* next() const = 0;

	/**
	 * Returns the number of data elements that are part of this set.
	 */

	virtual size_t count() const = 0;

protected:
	
	/**
	 * @nodoc
	 */
	// Destructor

	virtual ~i_data_set(){}
};

/**
 * Interface to a data element.
 * <br>
 * <b>Role:</b> Provides an interface to a data element along with a description and type.
 * The actual C++ type of the data content is specified by the "type()" function. 
 * To get the actual data, the caller has to typecast the address returned by the "value()" function.
 */

class DECL_KERN i_data_element
{
public:

	/**
	 * Describes the contents of this data element.
	 */

	virtual char const* description_str() const = 0;

	/**
	 * Returns the C++ data type of the contents.
	 */

	virtual char const* type_str() const = 0;

	/**
	 * Returns a generic handle to the actual data. This handle needs to be type-casted 
	 * to the respective type as specified by the <tt>type()</tt> function. For example, 
	 * if the <tt>type()</tt> function returns "SPAposition" then the return of this 
	 * function should be type-casted to 'SPAposition*'. The returned object is owned
	 * by this interface, and hence should not be deleted.
	 */

	virtual void* value() = 0;

protected:

	/**
	 * @deprecated R2016 1.0. Use description_str instead.
	 * Describes the contents of this data element.
	 */

	std::string description() const { return std::string(); }

	/**
	 * @deprecated R2016 1.0. Use type_str instead.
	 * Returns the C++ data type of the contents.
	 */

	std::string type() const { return std::string(); }
	

	/**
	 * @nodoc
	 */
	// Destructor

	virtual ~i_data_element(){}
};
//

// ------------------------------------------------------------------------------------------------
/** @} */

#endif	// AUX_DATA_HOLDER_HXX
