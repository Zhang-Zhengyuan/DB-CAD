/* ORIGINAL: 3dt2.1/render/li_view.hxx */
// $Id: li_view.hxx,v 1.6 2000/12/26 19:05:53 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//----------------------------------------------------------------------
// purpose---
//    Declare procedures for setting up rendering
//
//----------------------------------------------------------------------

#ifndef li_view_hxx
#define li_view_hxx

#include "rbapp_cb.hxx"

//======================================================================
class view3d;

void Do_LightWorks_Initialization();
void Prepare_To_Render( view3d *view);
/*
class toolkit_rbase_app_callback : public rbase_app_callback
{
public:
	virtual void image_start(int, int, int, int, int, float, int);

	virtual void image_scanline(int, void *);

	virtual void image_end(int);
	 
	virtual int check_interrupt();
};
*/
//======================================================================
#endif
