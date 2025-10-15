// $Id: map.hxx,v 1.7 2002/01/30 17:07:00 skenny Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//---------------------------------------------------------------------------
// purpose---
//	Implementing a pointer to pointer mapping
//---------------------------------------------------------------------------

#ifndef LISTS_MAP_HXX
#define LISTS_MAP_HXX

#include "lists.hxx"
#include "glists.hxx"
#include "lists.err"
#include "errorsys.hxx"

/**
 * @nodoc
 */
LIST(VOID_LIST,KERN);

class DECL_KERN VoidVoidMap : public ACIS_OBJECT {
	VOID_LIST	few;
	VOID_LIST_LIST	many;
public:
	void put(void* m, void* f) {
		int i = few.lookup(f);
		if(i == -1) {
			i = few.add(f);
			int j = many.add( ACIS_NEW VOID_LIST);
			if(i != j) sys_error(MAP_CONFUSION);
		}
		if(i == -1) sys_error(MAP_CONFUSION);
		VOID_LIST* ptrs_for_i = many[i];
		ptrs_for_i->add(m);
	}
	void* get(void* m) {
		for(int i = 0; i < few.count(); i++ ) {
			VOID_LIST* ptrs_for_i = many[i];
			if(ptrs_for_i->lookup(m) != -1) {
				return few[i];
			}
		}
		return NULL;
	}
	void clear() {
		for(int i = 0; i < few.count(); i++ ) {
			VOID_LIST* ptrs_for_i = many[i];
			ACIS_DELETE ptrs_for_i;
		}
		few.clear();
		many.clear();
	}
	VoidVoidMap() {}
	~VoidVoidMap() {
		clear();
	}

// This is to remove the warning 
//	copy constructor could not be generated PRS
protected:
	VoidVoidMap(const VoidVoidMap&) {}
};

#endif //LISTS_MAP_HXX
