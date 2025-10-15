/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef BASE_POINTER_MAP_HXX
#define BASE_POINTER_MAP_HXX

#include "dcl_law.h"
#include "mmgr.hxx"

/**
* @file base_pointer_map.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISKERNLAWS
 *
 * @{
 */
class law;
class law_data;
class void_void_map;

/**
 * @nodoc
 */
class DECL_LAW pointer_pair_list : public ACIS_OBJECT
{
	void_void_map* _map;

public:

	pointer_pair_list();
	~pointer_pair_list();
	void add(const void * key, void * result);
	void * find(const void * key);
};


/**
 * This class tracks reference counting of sub-objects used in making a deep copy.
 * <br>
 * <b>Role:</b> The base_pointer_map class is used in conjunction with deep_copy. A 
 * regular copy attempts to optimize time and space by reference counting an object, 
 * or pieces of the object, instead of copying the whole thing. The result of a deep 
 * copy does not share any data with the original object. However, if the original 
 * object had reference counted sub-objects, it is desirable that the deep copied 
 * object also share its corresponding sub-objects. The  base_pointer_map class is 
 * used to keep track of this kind of information. Thus, if the original object were 
 * a car with four wheels, all of which referenced one wheel object, the deep copied 
 * object would also contain four wheels which reference only one wheel object.
 * @see pointer_map, api_deep_copy_entity
 */
class DECL_LAW base_pointer_map : public ACIS_OBJECT
{
protected:
	pointer_pair_list pointer_list;

public:
	/**
	 * Default constructor.
	 */
	base_pointer_map();

	/**
	 * Returns a referenced pointer, original, or copy, to the law.
	 * @param law
	 * input law.
	 */
	law * get_law(const law * law);

	/**
	 * Returns a referenced pointer, original, or copy, to the  law_data.
	 * @param ld
	 * input law-data.
	 */
	law_data * get_law_data(const law_data * ld);

	/**
	 * Default destructor.
	 */
	virtual ~base_pointer_map();
};

/** @} */
#endif


