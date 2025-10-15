/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef SPAUseCounted_H
#define SPAUseCounted_H

#include "dcl_base.h"
#include "mmgr.hxx"

/**
* @nodoc.
**/
class DECL_BASE SPAUseCounted : public ACIS_OBJECT
{
   
public:
	/**
	* @nodoc.
	**/
	SPAUseCounted();

	/**
	* @nodoc.
	**/
	void AddRef();
	/**
	* @nodoc.
	**/
	void Release();

protected:	
	// virtual destructor required, so that Release can call "delete this" 
	// make destructor protected so that clients much go through use-counting
	// sub-classes should also make their destructors private or protected
	virtual ~SPAUseCounted();
	// these are necessary so derived classes can have sensible copy constructors.
	// Memberwise copy leaks for usecounted.
	SPAUseCounted(SPAUseCounted const& other);
	SPAUseCounted& operator=(SPAUseCounted const& other);

private:
	volatile int _useCount;
};

/**
* @nodoc.
**/
#include "SPAuse_counted_impl_holder.h"

/**
* @nodoc.
**/
template< class DerivedFromSPAUseCounted >
class DerivedFromSPAUseCounted_sptr : public ACIS_OBJECT
{
	SPAuse_counted_impl_holder holder;
public:
	/**
	* @nodoc.
	**/
	explicit DerivedFromSPAUseCounted_sptr(DerivedFromSPAUseCounted* use_counted_object ): holder( use_counted_object)
	{
	}
	/**
	* @nodoc.
	**/
	DerivedFromSPAUseCounted_sptr()
	{
	}

	/**
	* @nodoc.
	**/
	DerivedFromSPAUseCounted& operator*()
	{
		return *(DerivedFromSPAUseCounted*)holder.get();
	}
	/**
	* @nodoc.
	**/
	DerivedFromSPAUseCounted const& operator*() const
	{
		return *(DerivedFromSPAUseCounted*)holder.get();
	}
	/**
	* @nodoc.
	**/
	DerivedFromSPAUseCounted const* operator->() const
	{
		return (DerivedFromSPAUseCounted*)holder.get();
	}
	/**
	* @nodoc.
	**/
	DerivedFromSPAUseCounted* operator->()
	{
		return (DerivedFromSPAUseCounted*)holder.get();
	}
	/**
	* @nodoc.
	**/
	DerivedFromSPAUseCounted* get()
	{
		return (DerivedFromSPAUseCounted*)holder.get();
	}
	DerivedFromSPAUseCounted const* get() const
	{
		return (DerivedFromSPAUseCounted*)holder.get();
	}
	/**
	* @nodoc.
	**/
	void reset( DerivedFromSPAUseCounted* new_ptr )
	{
		holder.reset(new_ptr);
	}
};

#endif

