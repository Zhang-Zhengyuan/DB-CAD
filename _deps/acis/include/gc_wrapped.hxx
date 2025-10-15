/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef gc_wrapped_hxx
#define gc_wrapped_hxx

#include "scheme.hxx"
#include "gc_obj.hxx"

template<typename wrappedType>
class GC_Wrapper : public GC_Object
{
	wrappedType _data;
public:
	GC_Wrapper(ScmObject o, wrappedType data) : GC_Object(o), _data(data)
	{
	}
	wrappedType const get() const
	{
		return _data;
	}
	wrappedType get()
	{
		return _data;
	}
	void set(wrappedType data)
	{
		_data = data;
	}
};

template< typename T >
struct S_Wrapped
{
    ScmObject    tag;
	GC_Wrapper<T> *wrapped_object;
};

template<typename T>
ScmObject make_Scm_Wrapped(T the_mesh, int type )
{
	ScmObject r = Alloc_Object(sizeof(S_Wrapped<T>), type, 0);
	((struct S_Wrapped<T>*)POINTER(r))->tag = scm_Null;
	((struct S_Wrapped<T>*)POINTER(r))->wrapped_object =
		new GC_Wrapper<T>(r, the_mesh);
	return r;
}

#endif
