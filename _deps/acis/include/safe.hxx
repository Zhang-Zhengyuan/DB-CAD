/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef SAFE_HXX_INCLUDED
#define SAFE_HXX_INCLUDED

/**
 * @file safe.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

#if !defined( NOT_THREAD_SAFE_ACIS )

/**
 * The Thread-Safe ACIS define.
 */
#define THREAD_SAFE_ACIS

#endif

#ifdef __cplusplus

#include <stddef.h>

#include "dcl_base.h"
#include "logical.h"
#include "static_types.hxx"

/**
 * Defines the maximum number of threads ACIS can utilize.
 */
#define MAX_ACTIVE_THREADS 1024

/**
 * Returns the number of currently active threads.
 */
DECL_BASE int thread_count();

/**
 * Returns the maximum number of threads ACIS can utilize.
 */
DECL_BASE int max_thread_count();

/**
 * Returns the current thread's (ACIS assigned) ID.
 */
DECL_BASE int thread_id();

/**
 * Returns TRUE if ACIS is running in thread-safe mode.
 */
DECL_BASE logical thread_safety_enabled();

/**
 * Sets ACIS into thread-safe mode.
 */
DECL_BASE void thread_safe_region_begin();

/**
 * Sets ACIS into non-thread-safe mode (i.e. serial mode).
 */
DECL_BASE void thread_safe_region_end();


#ifdef THREAD_SAFE_ACIS

///////////////////////////////////////////////////////////////////////
//
// Thread control block
//
///////////////////////////////////////////////////////////////////////

/**
 * @nodoc
 */
struct thread_control_block {
	double* tls;
	int id;
};

/**
 * @nodoc
 */
DECL_BASE thread_control_block* get_thread_control_block();

/**
 * @nodoc
 */
DECL_BASE void set_thread_control_block( thread_control_block*);


///////////////////////////////////////////////////////////////////////
//
// Safe base class
//
///////////////////////////////////////////////////////////////////////
/**
 * @nodoc
 */
class safe_base {

	safe_base* next_sb;

	const void * curraddr;

protected:

	int offset;

public:

	DECL_BASE safe_base( const void * );

#ifdef ENABLE_TSA_STATS
	DECL_BASE const void* address( const void*, int access) const;
#else
	DECL_BASE const void* address( void ) const;
#endif

	safe_base* next() const;
	int ofst() const { return offset; }

	virtual void init( void* addr) = 0;
	virtual void term( logical final_term) = 0;
};


///////////////////////////////////////////////////////////////////////
//
// safe type address access macro
//
///////////////////////////////////////////////////////////////////////
#ifdef ENABLE_TSA_STATS
/**
 * @nodoc
 */
#define get_threadval_ptr(_cast_,_access_) ((_cast_)safe_base::address(_access_))
#else
/**
 * @nodoc
 */
#define get_threadval_ptr(_cast_,_access_unused_) ((_cast_)safe_base::address())

#endif

///////////////////////////////////////////////////////////////////////
//
// base class for safe built-in types
// contains all the methods
//
///////////////////////////////////////////////////////////////////////

/**
 * @nodoc
 */
template <class type>
class safe_type : public safe_base {

private:

	const type initval;

	type currval;

public:

	safe_type( type val = 0 ) 
		: initval(val), currval(val), safe_base((const void *)&currval)
	{}

	void init( void* addr) {
		*(type*)addr = initval;
	}	

	void term( logical final_term ) {
		if( final_term ) {
			offset = 0;
		}
		else {
			type* threadval = get_threadval_ptr(type*, TLSAccessTerm);
			*threadval = initval;
		}
	}

	type getinit() const {
		return initval;
	}

	type setinit( type val ) const {
		return initval = (type) val;
	}

	type value() {
		type* threadval = get_threadval_ptr(type*, TLSAccessRead);
		return *threadval;
	}

	type& ref() {
		type* threadval = get_threadval_ptr(type*, TLSAccessAddress);
		return *(type *)threadval;
	}

private:

	type& operator=( const safe_type<type>&);
};



///////////////////////////////////////////////////////////////////////
//
// safe function types and necessary operators
//
///////////////////////////////////////////////////////////////////////

/**
 * Defines a class with thread-local storage for and behavior of function pointers.
 * <br>
 * <b>Role:</b> This class is a member of the ACIS thread-local storage mechanisms called safe types.
 * The safe types not only provide thread specific values for their respective data types,
 * but also provide operators that allow them to behave like the data types they encapsulate.
 * <br><br>
 * <b>Note:</b> Refer to the technical article <i>Thread-Safe ACIS</i> for more information.
 * @see safe_function_type, safe_integral_type, safe_floating_type, safe_pointer_type, safe_object_pointer
*/
template <class type>
class safe_function_type : public safe_type<type> {

public:

	// default constructor
/**
 * Default constructor.
 */
	safe_function_type(const type val = 0 ) 
		: safe_type<type>( val )
	{}

	// copy constructor
/**
 * Copy constructor.
 */
	safe_function_type(const safe_function_type<type>& val ) 
		: safe_type<type>( val)
	{}

	// type cast operator
/**
 * Returns the type specific safe type.
 */
	operator type () const {
		const type* threadval = get_threadval_ptr(type*, TLSAccessRead);
		return *threadval;
	}

	// address-of, non-const
/**
 * Returns a non-const address of the type specific safe type.
 */
	type* operator &() {
		type* threadval = get_threadval_ptr(type*, TLSAccessAddress);
		return threadval;
	}

	// address-of, const
/**
 * Returns a const address of the type specific safe type.
 */
	const type* operator &() const {
		type* threadval = get_threadval_ptr(type*, TLSAccessAddress);
		return threadval;
	}

	// assignment
/**
 * Assignment operator.
 */
	type& operator=( const type& val) {
		type* threadval = get_threadval_ptr(type*, TLSAccessWrite);
		return *threadval = val;
	}

	// assigment of safe_function_type to safe_function_type
/**
 * Safe type specific assignment operator.
 */
	type & operator=( const safe_function_type<type>& val) {
		type* threadval = get_threadval_ptr(type*, TLSAccessWrite);
		return *threadval = (type)val;
	}
};



///////////////////////////////////////////////////////////////////////
//
// safe floating point types and necessary operators
//
///////////////////////////////////////////////////////////////////////

/**
 * Defines a class with thread-local storage for and behavior of floating point data types.
 * <br>
 * <b>Role:</b> This class is a member of the ACIS thread-local storage mechanisms called safe types.
 * The safe types not only provide thread specific values for their respective data types,
 * but also provide operators that allow them to behave like the data types they encapsulate.
 * <br><br>
 * <b>Note:</b> Refer to the technical article <i>Thread-Safe ACIS</i> for more information.
 * @see safe_function_type, safe_integral_type, safe_floating_type, safe_pointer_type, safe_object_pointer
*/
template <class type>
class safe_floating_type : public safe_function_type<type> {

public:

	// default constructor
/**
 * Default constructor.
 */
	safe_floating_type(const type val = 0.0 ) 
		: safe_function_type<type>( val )
	{}

	// copy constructor
/**
 * Copy constructor.
 */
	safe_floating_type(const safe_floating_type<type>& val ) 
		: safe_function_type<type>( val )
	{}

	// assignment - each sub-safe-type needs to implement this.  Otherwise,
	// the compiler will try to promote the rhs of the assignment to a safe-
	// type, which will fail (b/c safe-types cannot be created at run-time).
/**
 * Assignment operator.
 */
	type & operator=(const type& val) {
		return safe_function_type<type>::operator=(val);
	}

	// assigment of safe_floating_type to safe_floating_type
/**
 * Safe type specific assignment operator.
 */
	type & operator=( const safe_floating_type<type>& val) {
		type* threadval = get_threadval_ptr(type*, TLSAccessWrite);
		return *threadval = (type) val;
	}

	// unary operators += -= *= /= ++ --
/**
 * Unary operator +=.
 */
	type operator+=( type val) {
		type* threadval = get_threadval_ptr(type*, TLSAccessWrite);
		return *threadval += val;
	}
/**
 * Unary operator -=.
 */
	type operator-=( type val) {
		type* threadval = get_threadval_ptr(type*, TLSAccessWrite);
		return *threadval -= val;
	}
/**
 * Unary operator *=.
 */
	type operator*=( type val) {
		type* threadval = get_threadval_ptr(type*, TLSAccessWrite);
		return *threadval *= val;
	}
/**
 * Unary operator /=.
 */
	type operator/=( type val) {
		type* threadval = get_threadval_ptr(type*, TLSAccessWrite);
		return *threadval /= val;
	}
/**
 * Unary operator ++ (prefix).
 */
	type operator++() { // prefix
		type* threadval = get_threadval_ptr(type*, TLSAccessWrite);
		return *threadval += 1;
	}
/**
 * Unary operator ++ (postfix).
 */
	type operator++( int) { // postfix
		type* threadval = get_threadval_ptr(type*, TLSAccessWrite);
		type preval = *threadval;
		*threadval += 1;
		return preval;
	}
/**
 * Unary operator -- (prefix).
 */
	type operator--() { // prefix
		type* threadval = get_threadval_ptr(type*, TLSAccessWrite);
		return *threadval -= 1;
	}
/**
 * Unary operator -- (postfix).
 */
	type operator--( int) { // postfix
		type* threadval = get_threadval_ptr(type*, TLSAccessWrite);
		type preval = *threadval;
		*threadval -= 1;
		return preval;
	}
};


///////////////////////////////////////////////////////////////////////
// derive a safe integral type from safe floats
///////////////////////////////////////////////////////////////////////

/**
 * Defines a class with thread-local storage for and behavior of integral data types.
 * <br>
 * <b>Role:</b> This class is a member of the ACIS thread-local storage mechanisms called safe types.
 * The safe types not only provide thread specific values for their respective data types,
 * but also provide operators that allow them to behave like the data types they encapsulate.
 * <br><br>
 * <b>Note:</b> Refer to the technical article <i>Thread-Safe ACIS</i> for more information.
 * @see safe_function_type, safe_integral_type, safe_floating_type, safe_pointer_type, safe_object_pointer
*/
template <class type>
class safe_integral_type : public safe_floating_type<type> {

public:

	// default constructor
/**
 * Default constructor.
 */
	safe_integral_type( const type& val = 0 )
		: safe_floating_type<type>( val ) 
	{}

	// copy constructor
/**
 * Copy constructor.
 */
	safe_integral_type( const safe_integral_type<type>& val ) 
		: safe_floating_type<type>( val )
	{}

	// assignment - each sub-safe-type needs to implement this.  Otherwise,
	// the compiler will try to promote the rhs of the assignment to a safe-
	// type, which will fail (b/c safe-types cannot be created at run-time).
/**
 * Assignment operator.
 */
	type & operator=(const type& val) {
		return safe_function_type<type>::operator=(val);
	}

	// assignment of safe type to safe type 
/**
 * Safe type specific assignment operator.
 */
	type & operator=( const safe_integral_type<type>& val) {	
		type* threadval = get_threadval_ptr(type*, TLSAccessWrite);
		return *threadval = (type) val;
	}

	//
	// Bitwise operators are for integral types only
	//
/**
 * Bitwise operator %= (for integral types only).
 */
	type operator%=( type val) {
		type* threadval = get_threadval_ptr(type*, TLSAccessWrite);
		return *threadval %= val;
	}
/**
 * Bitwise operator ^= (for integral types only).
 */
	type operator^=( type val) {
		type* threadval = get_threadval_ptr(type*, TLSAccessWrite);
		return *threadval ^= val;
	}
/**
 * Bitwise operator &= (for integral types only).
 */
	type operator&=( type val) {
		type* threadval = get_threadval_ptr(type*, TLSAccessWrite);
		return *threadval &= val;
	}
/**
 * Bitwise operator |= (for integral types only).
 */
	type operator|=( type val) {
		type* threadval = get_threadval_ptr(type*, TLSAccessWrite);
		return *threadval |= val;
	}
/**
 * Bitwise operator <<= (for integral types only).
 */
	type operator<<=( type val) {
		type* threadval = get_threadval_ptr(type*, TLSAccessWrite);
		return *threadval <<= val;
	}
/**
 * Bitwise operator >>= (for integral types only).
 */
	type operator>>=( type val) {
		type* threadval = get_threadval_ptr(type*, TLSAccessWrite);
		return *threadval >>= val;
	}
};

///////////////////////////////////////////////////////////////////////
//
// safe pointer to basic types
//
///////////////////////////////////////////////////////////////////////

/**
 * Defines a class with thread-local storage for and behavior of generic pointers.
 * <br>
 * <b>Role:</b> This class is a member of the ACIS thread-local storage mechanisms called safe types.
 * The safe types not only provide thread specific values for their respective data types,
 * but also provide operators that allow them to behave like the data types they encapsulate.
 * <br><br>
 * <b>Note:</b> Refer to the technical article <i>Thread-Safe ACIS</i> for more information.
 * @see safe_function_type, safe_integral_type, safe_floating_type, safe_pointer_type, safe_object_pointer
*/
template <class type>
class safe_pointer_type : public safe_base {

private:

	type const* initval;

	type * currval;

public:

	// constructor
/**
 * Default constructor.
 */
	safe_pointer_type( type* val = NULL ) 
		: initval(val), currval(val), safe_base((const void *)&currval)
	{}
	

	// copy constructor
/**
 * Copy constructor.
 */
	safe_pointer_type( const safe_pointer_type<type>& val ) 
		: initval( val), currval(val), safe_base((const void *)&currval)
	{}
/**
 * @nodoc
 */
	void init( void* addr) {
		*(type**)addr = (type*)initval;
	}	
/**
 * @nodoc
 */
	void term( logical final_term ) {
		if( final_term ) {
			offset = 0;
		}
		else {
			type** threadval = get_threadval_ptr(type**, TLSAccessTerm);
			*threadval = (type*)initval;
		}
	}
/**
 * @nodoc
 */
	type* setinit( type * val ) {
		return initval = val;
	}

	// pointer to reference, used for *& parameters in function calls
/**
 * Returns a pointer reference of the type specific safe type.
 */
	type*& ptrref() {
		type** threadval = get_threadval_ptr(type**, TLSAccessAddress);
		return *threadval;
	}

	// cast operator
/**
 * Returns the type specific safe type (cast operator).
 */
	operator type *() const {
		type** threadval = (type**)get_threadval_ptr(type**, TLSAccessAddress);
		return *threadval;
	}

	// dereference
/**
 * Returns the dereferenced type specific safe type (indirection operator).
 */
	type& operator*() const {
		type** threadval = (type**)get_threadval_ptr(type**, TLSAccessAddress);
		return **threadval;
	}
		
	// address-of, non-const
/**
 * Returns the non-const address of the type specific safe type.
 */
	type** operator&() {
		type** threadval = get_threadval_ptr(type**, TLSAccessAddress);
		return threadval;
	}

	// address-of, const
/**
 * Returns the const address of the type specific safe type.
 */
	const type** operator&() const {
		const type** threadval = (const type**)get_threadval_ptr(const type**, TLSAccessAddress);
		return threadval;
	}

/**
 * Returns the type specific safe type at index n (subscript operator).
 */
	type& operator[] (int index) {
		type** threadval = get_threadval_ptr(type**, TLSAccessAddress);
		return *(*threadval + index );
	}

	// assignment
/**
 * Assignment operator.
 */
	type *& operator=( type* val) {
		type** threadval = get_threadval_ptr(type**, TLSAccessWrite);
		return (*threadval = val);
	}

	// assignment of safe pointer to safe pointer
/**
 * Safe type specific assignment operator.
 */
	type *& operator=( const safe_pointer_type<type> & val) {	
		type** threadval = get_threadval_ptr(type**, TLSAccessWrite);
		return (*threadval = val);
	}

// disallow pointer arithmetic on safe types
private:
	type& operator++(); 		// { /* prefix*/ }
	type& operator++( int); 	// { /* postfix */ }
	type& operator--();   		// { /* prefix*/ }
	type& operator--( int); 	// { /* postfix */ }

};

///////////////////////////////////////////////////////////////////////
//
// safe pointers to objects and UDT
//
///////////////////////////////////////////////////////////////////////

/**
 * Defines a class with thread-local storage for and behavior of object pointers.
 * <br>
 * <b>Role:</b> This class is a member of the ACIS thread-local storage mechanisms called safe types.
 * The safe types not only provide thread specific values for their respective data types,
 * but also provide operators that allow them to behave like the data types they encapsulate.
 * <br><br>
 * <b>Note:</b> Refer to the technical article <i>Thread-Safe ACIS</i> for more information.
 * @see safe_function_type, safe_integral_type, safe_floating_type, safe_pointer_type, safe_object_pointer
*/
template <class type>
class safe_object_pointer : public safe_pointer_type<type> {

public:

/**
 * Default constructor.
 */
	safe_object_pointer( type* val = NULL ) 
		: safe_pointer_type<type>(val)
	{}
	
/**
 * Returns a pointer reference of the type specific safe type.
 */
	type *& ref() {
		type** threadval = get_threadval_ptr(type**, TLSAccessAddress);
		return *threadval;
	}
/**
 * Returns the type specific safe type via dereference (indirect member access operator).
 */
	type* operator ->() const {
		type** threadval = (type**)get_threadval_ptr(type**, TLSAccessAddress);
		return *threadval;
	}

/**
 * Assignment operator.
 */
	type *& operator=(type * val) {
		return safe_pointer_type<type>::operator=(val);
	}
};

#endif /* SAFE_HXX_INCLUDED */

#endif /* THREAD_SAFE_ACIS */

#endif /* __cplusplus */



