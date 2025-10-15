/* ORIGINAL: 3dt2.1/geomhusk/sld_utl.hxx */
// $Id: swp_expo.hxx,v 1.4 2002/08/09 17:28:44 jeff Exp $
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
//    Declare procedures for creating solids
//
//----------------------------------------------------------------------

#ifndef SWP_EXPO_HXX
#define SWP_EXPO_HXX

#include "dcl_swp.h"
#include "api.hxx"

class ENTITY;
class BODY;
class FACE;
class surface;
class SPAposition;
class SPAvector;
class SPAunit_vector;
class sweep_options;


DECL_SWP outcome sweep_wire_along_vector(
	BODY*,
	const SPAvector&,
	double
);

DECL_SWP outcome sweep_face_along_vector(
	FACE*,
	const SPAvector&,
	double
);

DECL_SWP outcome sweep_wire_about_axis(
	BODY*,
	const SPAposition& ,
	const SPAunit_vector&,
	double,
	double
);

DECL_SWP outcome sweep_face_about_axis(
	FACE*,
	const SPAposition& ,
	const SPAunit_vector&,
	double,
	double
);

//=============================================================
outcome DECL_SWP sweep_rigid(
	ENTITY*,
	logical,
	BODY*,
	const SPAvector&,
	BODY*&,
	sweep_options*
);
					
outcome DECL_SWP sweep_along_vector(
	ENTITY*,
	logical,
	double,
	const SPAvector&,
	double,
	int,
	BODY*&
);
					 
outcome DECL_SWP sweep_about_axis(
	ENTITY*,
	logical ,
	const SPAposition& ,
	const SPAunit_vector& ,
	double ,
	int ,
	double,
	int,
	BODY*&
);

outcome DECL_SWP sweep_along_path(
	ENTITY* ,
	logical ,
	BODY* ,
	const surface&,
	surface*,  
	double,
	int,
	BODY*&
);
					  
outcome DECL_SWP sweep_with_twist(  
	ENTITY*,
	logical,
	BODY*,
	double,
	BODY*&
);

#endif
