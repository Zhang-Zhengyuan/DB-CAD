/* ORIGINAL: acis2.1/sg_husk/debug/sg_debug.hxx */
/* $Id: sg_debug.hxx,v 1.16 2002/08/09 17:15:35 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


//
// Modification---
//	04-Feb-99 bd	Add more details, and a new arg, and some explanation, to dbedge.
//	30-Apr-98 bd	Add dbslice.
//	22-Dec-97 bd	Add dbbs2curve, dbbs3curve, dbbs3surface, dbedge, dbcoedge,
//		and hooks for dbbs2c(), dbbs3c().
//

#if !defined( SG_DBG_HDR_DEF )
#define  SG_DBG_HDR_DEF

#include <stdio.h>
#include "dcl_kern.h"

#include "bs2curve.hxx"
#include "bs3curve.hxx"
#include "bs3surf.hxx"
#include "bl_cntct.hxx"

class ENTITY;
class EDGE;
class COEDGE;

class curve;
class pcurve;
class SPAposition;
class surface;
class SPAtransf;
class SPAunit_vector;
class SPAvector;
class law;

#include "lists.hxx"

	// Pointers to the standard FILE streams
	// The following 3 vars have been changed to LOCAL_VAR
	//extern DECL_KERN FILE* dbfile;
	//extern DECL_KERN FILE* dbout;
	//extern DECL_KERN FILE* dberr;

	// Help routine
	DECL_KERN void dbhelp( );			// print some help about the commands

	// Input-Output routines
	DECL_KERN void dbopen( const char *);		// open file to redirect db output
	DECL_KERN void dbclose();					// close output file
	DECL_KERN void dbflush();					// flush output file

	// Geometry routines
	DECL_KERN void dbuvec( SPAunit_vector const * );// debug a unit SPAvector
	DECL_KERN void dbvec( SPAvector const * );		// debug a SPAvector
	DECL_KERN void dbpos( SPAposition const * );	// debug a SPAposition
	DECL_KERN void dbtransf( SPAtransf const * );	// debug a transformation
	DECL_KERN void dbcurve( curve const * );	// debug a curve
	DECL_KERN void dbpcurve( pcurve const * );	// debug a pcurve
	DECL_KERN void dbsurface( surface const * );// debug a surface

	DECL_KERN void dbbs2curve( bs2_curve );	  // debug a bs2_curve
	DECL_KERN void dbbs3curve( bs3_curve );	  // debug a bs3_curve
	DECL_KERN void dbbs3surface( bs3_surface ); // debug a bs3_surface

	// dbedge: The int argument is a debug level:
	//  0: start and end points
	//  1: param range, bounding SPAbox, and reversal flag
	//  2: debug the underlying curve
	DECL_KERN void dbedge( EDGE const *, int = 0 );// print info about an EDGE

	DECL_KERN void dbcoedge( COEDGE * );		// print info about a COEDGE
	DECL_KERN void dbbs2c( bs2_curve );			// print info about a bs2_curve
	DECL_KERN void dbbs3c( bs3_curve );			// print info about a bs3_curve
		// print eval results for a bs2_curve at incremental parameters values:
	DECL_KERN void bs2_curve_step_eval( bs2_curve, int );
		// print eval results for a bs3_curve at incremental parameters values:
	DECL_KERN void bs3_curve_step_eval( bs3_curve, int );
	DECL_KERN void dblaw( law const * );		// debug a law

		// For internal use (do not doc):
	DECL_KERN int dbslice( v_bl_contacts *, int=1 );

	// Entity routines
	DECL_KERN void dbent( ENTITY const * );		// debug an entity
	DECL_KERN void dbentall( ENTITY const * );  // debug ENTITY, siblings, and children
	DECL_KERN void dbentallsz( ENTITY const * );// debug size for ENTITY, sibs, and children
	DECL_KERN void dbentkids( ENTITY const * );	// debug ENTITY and children
	DECL_KERN void dbsave( ENTITY const * );	// save an ENTITY to dbfile

	// ENTITY_LIST routines
	DECL_KERN void dblist( ENTITY_LIST const *);// debug an ENTITY_LIST (pointer debug)
	DECL_KERN void dblistent( ENTITY_LIST const *);// debug an ENTITY_LIST (ENTITY debug)

	// Debug routines
	DECL_KERN void dbdbg( char const *, int );	// turn on debugging module
	DECL_KERN void dbopti( char const *, int );	// turn on option (int arg)
	DECL_KERN void dboptd( char const *, double );// turn on option (double arg)
	DECL_KERN void dboptc( char const *, char const * );// turn on option (char* arg)


#endif
