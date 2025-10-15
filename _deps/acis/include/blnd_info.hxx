/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef _BLND_INFO_HXX
#define _BLND_INFO_HXX

#include "lists.hxx"
#include "curve.hxx"

/**
 * @nodoc
 */
 class DECL_KERN blend_debug_info : public ACIS_OBJECT
 {
 public:
	 blend_debug_info():m_owner(NULL), m_def_cur(NULL){}
	 ~blend_debug_info();
	 
	 void set_owner(ENTITY* owns);
	 void set_support_ents(ENTITY_LIST& elist);
	 void set_curve(CURVE* cur);
	 ENTITY* m_owner;
	 ENTITY_LIST m_support_ents;
	 CURVE* m_def_cur;
 };


#endif
