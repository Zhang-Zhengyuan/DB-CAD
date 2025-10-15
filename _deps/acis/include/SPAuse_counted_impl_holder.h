/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef SPAuse_counted_impl_holder_hxx
#define SPAuse_counted_impl_holder_hxx

#include "dcl_base.h"
#include "mmgr.hxx"

class SPAUseCounted;


class DECL_BASE SPAuse_counted_impl_holder 
{
public:
	/**
	* @nodoc
	**/
	MMGR_FREELIST_THIS

#if 0
;
#endif
	/**
	* @nodoc.
	**/
	SPAuse_counted_impl_holder();
	/**
	* @nodoc.
	**/
	SPAuse_counted_impl_holder( SPAUseCounted* in_impl );

	/**
	* @nodoc.
	**/
	SPAuse_counted_impl_holder const& operator=(SPAuse_counted_impl_holder const& other);

	/**
	* @nodoc.
	**/
	SPAuse_counted_impl_holder(SPAuse_counted_impl_holder const& other);

	/**
	* @nodoc.
	**/
	SPAUseCounted* get();
	/**
	* @nodoc.
	**/
	SPAUseCounted const* get() const;

	/**
	* @nodoc.
	**/	
	void reset(SPAUseCounted* );

	/**
	* @nodoc.
	**/
	~SPAuse_counted_impl_holder();

private:
	SPAUseCounted* m_impl;
};


#endif
