/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef CONTAINER_UTILS_HXX
#define CONTAINER_UTILS_HXX

#include "dcl_base.h"
#include "base.hxx"

class SPAposition;
class SPAposition_array;
class SPAdouble_array;
class param_info;
class param_info_array;
class SPAvector;
class SPAvector_array;
class SPAunit_vector;
class SPAunit_vector_array;

/**
* @file SPAposition_cloud.hxx
* @CAA2Level L1
* @CAA2Usage U1
*! \addtogroup FCTAPI
*  \brief Declared at <container_utils.hxx>
*  @{
*/

/**
* Provides functionality similar to std::vector<SPAposition> within ACIS.  Used for convenience in scheme.
**/
class DECL_BASE SPAposition_vector: public ACIS_OBJECT
{
public:
	/**
	*Constructs empty position vector
	**/
	SPAposition_vector();

	//internal use only
	/**
	* @nodoc
	**/
	SPAposition_vector( SPAposition_array*& in_array );

	/**
	*Deletes all data stored
	**/
	~SPAposition_vector();

	/**
	*returns number of positions present in this vector
	**/
	int size() const;

	/**
	*returns const& to iith element of this vector
	**/
	SPAposition const& operator[](int ii) const;

	/**
	*returns a changeable reference to iith element of this vector
	**/
	SPAposition& operator[](int ii);

	/**
	* Adds a position to this vector.  May resize the vector to ensure enough space is present for
	* this positions along with those previously added.
	**/
	void push_back( SPAposition const& pos_to_add );

	/**
	* changes allocated block size and vector size.
	**/
	void resize( int new_size );

	/**
	* erases all contents of vector.
	**/
	void clear();

	/**
	* copy contents into other vector
	**/
	void copy_to( SPAposition_vector& out_copy ) const;

	/**
	* Converts SPAposition_vector to a const pointer to it's owned storage
	**/
	operator SPAposition const* const() const;

	/**
	* Converts SPAposition_vector to a pointer to it's owned storage
	**/
	operator SPAposition* const();

	//internal use only
	/**
	* @nodoc
	**/
	SPAposition_array* get_array();
private:
	SPAposition_array* m_array;

	// since these are large and most people don't want to copy them, disable copy constructor and assignment operator
	SPAposition_vector( SPAposition_vector const& );
	SPAposition_vector& operator=( SPAposition_vector const& );
};


/**
* Provides functionality similar to std::vector<double> within ACIS.  Used for convenience in scheme.
**/
class DECL_BASE SPAdouble_vector: public ACIS_OBJECT
{
public:
	/**
	*Constructs empty position vector
	**/
	SPAdouble_vector();

	/**
	*@nodoc
	**/
	SPAdouble_vector( SPAdouble_array*& in_array );

	/**
	*@nodoc
	**/
	~SPAdouble_vector();

	/**
	* Returns number of doubles in double vector.
	**/
	int size() const;

	/**
	* Returns const reference to ii-th double in this vector.
	**/
	double const& operator[](int ii) const;

	/**
	*Returns reference to ii-th double in this vector.
	**/
	double& operator[](int ii);

	/**
	* Adds a double to the end of this vector, resizing if necessary.
	**/
	void push_back( double const& pos_to_add );
	
	/**
	* changes allocated block size and vector size.
	**/
	void resize( int new_size );
	
	/**
	* erases all contents of vector.
	**/
	void clear();

	/**
	* copy contents into other vector
	**/
	void copy_to( SPAdouble_vector& out_copy ) const;

	/**
	* @nodoc
	**/
	operator double const* const() const;

	/**
	* @nodoc
	**/
	operator double* const();

	/**
	* @nodoc
	**/
	SPAdouble_array* get_array();
private:
	SPAdouble_array* m_array;
	// since these are large and most people don't want to copy them, disable copy constructor and assignment operator
	SPAdouble_vector( SPAdouble_vector const& );
	SPAdouble_vector& operator=( SPAdouble_vector const& );
};

class SPAint_array;

class DECL_BASE SPAint_vector: public ACIS_OBJECT
{
public:
	/**
	*Constructs empty position vector
	**/
	SPAint_vector();

	/**
	*@nodoc
	**/
	SPAint_vector( SPAint_array*& in_array );

	/**
	*@nodoc
	**/
	~SPAint_vector();

	/**
	* Returns number of ints in int vector.
	**/
	int size() const;

	/**
	* Returns const reference to ii-th int in this vector.
	**/
	int const& operator[](int ii) const;

	/**
	*Returns reference to ii-th int in this vector.
	**/
	int& operator[](int ii);

	/**
	* Adds a int to the end of this vector, resizing if necessary.
	**/
	void push_back( int const& pos_to_add );
	
	/**
	* changes allocated block size and vector size.
	**/
	void resize( int new_size );
	
	/**
	* erases all contents of vector.
	**/
	void clear();

	/**
	* copy contents into other vector
	**/
	void copy_to( SPAint_vector& out_copy ) const;

	/**
	* @nodoc
	**/
	operator int const* const() const;

	/**
	* @nodoc
	**/
	operator int* const();

	/**
	* @nodoc
	**/
	SPAint_array* get_array();
private:
	SPAint_array* m_array;
	// since these are large and most people don't want to copy them, disable copy constructor and assignment operator
	SPAint_vector( SPAint_vector const& );
	SPAint_vector& operator=( SPAint_vector const& );
};


/**
* Provides functionality similar to std::vector<SPAvector>.
**/
class DECL_BASE SPAvector_list : public ACIS_OBJECT
{
public:

	/**
	* Constructs an empty SPAvector list
	**/
	SPAvector_list();

	/**
	* Deletes all stored data
	**/
	~SPAvector_list();

	/**
	* Returns the number of elements present in this list
	**/
	int size() const;

	/**
	* Returns a const reference to the ith element of this list
	**/
	SPAvector const& operator[]( int i ) const;

	/**
	* Returns a reference to the ith element of this list
	**/
	SPAvector& operator[]( int i );

	/**
	* Adds an element at the end of this list
	**/
	void push_back( SPAvector const& v );

	/**
	* Changes the list size
	**/
	void resize( int new_size );

	/**
	* Erases the entire list
	**/
	void clear();

	/**
	* Make a copy of the list
	**/
	void copy_to( SPAvector_list& out_copy ) const;

	// internal use only
	/**
	* @nodoc
	**/
	SPAvector_array* get_array();

private:

	SPAvector_array *m_array;

	// Since these are large and most people don't want to copy them,
	// disable copy constructor and assignment operator.

	SPAvector_list( SPAvector_list const& );
	SPAvector_list& operator=( SPAvector_list const& );

	/**
	* Converts SPAvector_list to a const pointer to it's owned storage
	**/
	operator SPAvector const* const() const;

	/**
	* Converts SPAvector_list to a pointer to it's owned storage
	**/
	operator SPAvector* const();
};


/**
* Provides functionality similar to std::vector<SPAunit_vector>.
**/
class DECL_BASE SPAunit_vector_list : public ACIS_OBJECT
{
public:

	/**
	* Constructs an empty SPAunit_vector list
	**/
	SPAunit_vector_list();

	/**
	* Deletes all stored data
	**/
	~SPAunit_vector_list();

	/**
	* Returns the number of elements present in this list
	**/
	int size() const;

	/**
	* Returns a const reference to the ith element of this list
	**/
	SPAunit_vector const& operator[]( int i ) const;

	/**
	* Returns a reference to the ith element of this list
	**/
	SPAunit_vector& operator[]( int i );

	/**
	* Adds an element at the end of this list
	**/
	void push_back( SPAunit_vector const& v );

	/**
	* Changes the list size
	**/
	void resize( int new_size );

	/**
	* Erases the entire list
	**/
	void clear();

	/**
	* Make a copy of the list
	**/
	void copy_to( SPAunit_vector_list& out_copy ) const;

	// internal use only
	/**
	* @nodoc
	**/
	SPAunit_vector_array* get_array();

private:

	SPAunit_vector_array *m_array;

	// Since these are large and most people don't want to copy them,
	// disable copy constructor and assignment operator.

	SPAunit_vector_list( SPAunit_vector_list const& );
	SPAunit_vector_list& operator=( SPAunit_vector_list const& );

	/**
	* Converts SPAunit_vector_list to a const pointer to it's owned storage
	**/
	operator SPAunit_vector const* const() const;

	/**
	* Converts SPAunit_vector_list to a pointer to it's owned storage
	**/
	operator SPAunit_vector* const();
};
/* @} */

#endif
