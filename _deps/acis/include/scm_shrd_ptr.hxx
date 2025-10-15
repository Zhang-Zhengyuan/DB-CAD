/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#pragma once

#include "acis.hxx"
#include <cstddef>

/**
* A basic shared pointer implementation that provides many but not all
* functionalities of the std::shared_ptr. Some limitations are:
* - constructor does not allow custom Deleter (ACIS_DELETE is used)
* - not thread safe
* - only explicit construction (no implicit conversion)
*/

template <typename T>
class scm_shared_ptr
{
	class ref_cntr
	{
		std::size_t cntr;
	public:
		MMGR_FREELIST_THIS;
		ref_cntr() : cntr(0)
		{
		}
		std::size_t incr()
		{
			return ++cntr;
		}
		std::size_t decr()
		{
			return --cntr;
		}
		std::size_t get() const
		{
			return cntr;
		}
	};

	T* ptr;
	ref_cntr* cnt;

	void clear()
	{
		if (0 == cnt->decr())
		{
			ACIS_DELETE ptr;
			ACIS_DELETE cnt;
		}
	}

public:
	explicit scm_shared_ptr(T* _ptr = nullptr)
		: ptr(_ptr), cnt(ACIS_NEW ref_cntr())
	{
		cnt->incr();
	}

	scm_shared_ptr(const scm_shared_ptr<T>& other)
		: ptr(other.ptr), cnt(other.cnt)
	{
		cnt->incr();
	}

	~scm_shared_ptr()
	{
		clear();
	}

	scm_shared_ptr<T>& operator=(const scm_shared_ptr<T>& other)
	{
		if (this != &other)
		{
			clear();

			ptr = other.ptr;
			cnt = other.cnt;
			cnt->incr();
		}
		return *this;
	}

	bool operator<(const scm_shared_ptr<T>& other) const
	{
		return ptr < other.ptr;
	}

	void reset(T* _ptr)
	{
		if (ptr != _ptr)
		{
			clear();

			ptr = _ptr;
			cnt = ACIS_NEW ref_cntr();
			cnt->incr();
		}
	}

	bool operator==(const scm_shared_ptr<T>& other)
	{
		return other.ptr == ptr;
	}

	T& operator*()
	{
		CONTRACT_ASSERT(ptr);
		return *ptr;
	}

	const T& operator*() const
	{
		CONTRACT_ASSERT(ptr);
		return *ptr;
	}

	T* get()
	{
		return ptr;
	}

	const T* get() const
	{
		return ptr;
	}

	T* operator->()
	{
		return get();
	}

	const T* operator->() const
	{
		return get();
	}

	std::size_t use_count() const
	{
		return cnt->get();
	}

	operator bool() const
	{
		return nullptr != ptr;
	}
};
