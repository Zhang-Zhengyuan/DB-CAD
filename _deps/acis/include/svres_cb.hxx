// $Id: svres_cb.hxx,v 1.8 2001/05/01 15:56:50 jsloan Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//-----------------------------------------------------------------------
// Purpose---
//	Callbacks to let people manipulate entities before SAT file saves
//  and after SAT file restores.
//-----------------------------------------------------------------------

// THIS FILE CONTAINS THE INTERFACE TO UNSUPPORTED TEST CODE.
// USE THIS INTERFACE DIRECTLY AT YOUR OWN RISK!

#ifndef __SVRESCB_HXX__
#define __SVRESCB_HXX__

#include "dcl_kern.h"
#include "logical.h"
#include "tlkit_cb.hxx"

class DECL_KERN svres_callbacks : public toolkit_callback 
// THIS CLASS NAME WAS CHANGED TO INDICATE TO CUSTOMERS THAT
// IT IS SLATED FOR REMOVAL IN A FUTURE VERSION OF ACIS.
// IF YOU ARE USING IT, PLEASE CONTACT support@spatial.com 
{
public:
	virtual void Before_Save(  int save_vers);
	virtual void After_Restore(int save_vers);
};

class DECL_KERN svres_callbacks_list : public toolkit_callback_list 
// THIS CLASS NAME WAS CHANGED TO INDICATE TO CUSTOMERS THAT
// IT IS SLATED FOR REMOVAL IN A FUTURE VERSION OF ACIS.
// IF YOU ARE USING IT, PLEASE CONTACT support@spatial.com 
{
public:
	void add(   svres_callbacks* cb) { toolkit_callback_list::add(cb); }
	void append(svres_callbacks* cb) { toolkit_callback_list::append(cb); }

	virtual void Before_Save(  int save_vers);
	virtual void After_Restore(int save_vers);
};

// THIS FUNCTION NAME WAS CHANGED TO INDICATE TO CUSTOMERS THAT
// THE svres_callbacks CLASS IS SLATED FOR REMOVAL IN A FUTURE VERSION 
// OF ACIS. IF YOU ARE USING IT, PLEASE CONTACT support@spatial.com 
extern DECL_KERN svres_callbacks_list& get_svres_callbacks_list();

#endif //__SVRESCB_HXX__
