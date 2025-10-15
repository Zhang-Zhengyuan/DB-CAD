/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( ATTRIB_SPACOLLECTION_CLASS )
#define ATTRIB_SPACOLLECTION_CLASS

#include "dcl_kern.h"
#include "at_ct.hxx"
#include "collection.hxx"

#define ATTRIB_SPAGROUP ATTRIB_SPACOLLECTION
#define ATTRIB_SPAGROUP_LEVEL ATTRIB_SPACOLLECTION_LEVEL
#define ATTRIB_SPAGROUP_NAME ATTRIB_SPACOLLECTION_NAME
#define ATTRIB_SPAGROUP_TYPE ATTRIB_SPACOLLECTION_TYPE
#define is_ATTRIB_SPAGROUP is_ATTRIB_SPACOLLECTION

class SPAGROUP;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_SPACOLLECTION, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

extern DECL_KERN int ATTRIB_SPACOLLECTION_TYPE;
#define ATTRIB_SPACOLLECTION_LEVEL (ATTRIB_CT_LEVEL + 1)

class DECL_KERN ATTRIB_SPACOLLECTION: public ATTRIB_CT {

	//	Pointer to collection entity that contains
	//	the list of entities in the collection
	SPACOLLECTION *collection_ptr; 

public:

	// Construct the initial attribute - add collection
	ATTRIB_SPACOLLECTION( ENTITY * = NULL, SPACOLLECTION * = NULL );

	// Return collection pointer
	SPACOLLECTION* collection() const {return collection_ptr;}

	SPAGROUP* group() const {return (SPAGROUP *) collection_ptr;}

	// Set collection
	void set_collection(SPACOLLECTION *);

	// notification function
	virtual void split_owner( ENTITY * );
	virtual void merge_owner( ENTITY *, logical );
	virtual void copy_owner( ENTITY * );
	virtual void trans_owner( const SPAtransf& );
	virtual void trans_owner_list( const SPAtransf&, ENTITY_LIST & );
	virtual void replace_owner( ENTITY *, logical );
	virtual void reverse_owner();
	virtual void warp_owner( law *);
	virtual void lop_change_owner();
	virtual void replace_owner_geometry( ENTITY * );
	virtual void to_tolerant_owner( ENTITY * );
	virtual void from_tolerant_owner( ENTITY * );

	// Standard housekeeping functions

	virtual logical copyable() const;
	virtual logical savable() const;

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_SPACOLLECTION, KERN )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};


#endif

