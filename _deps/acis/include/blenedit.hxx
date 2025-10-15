/* ORIGINAL: acis2.1/kernbool/blending/blenedit.hxx */
/* $Id: blenedit.hxx,v 1.6 2002/08/09 17:28:26 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


#include "logical.h"

#ifndef LOPB
#define LOPB

#ifdef D3_DEBUG

class PS_FILE;

#endif

#if defined D3_STANDALONE || defined D3_DEBUG

class   D3_ostream;
class   D3_istream;

#endif

#include "dcl_blnd.h"
#include "box.hxx"
#include "curve.hxx"
#include "surface.hxx"

class   COEDGE;

// The BLEND_EDIT class is declared here.
// BLEND_EDIT is the pure virtual base class for use by those
// wishing to affect the calculation of blend surface geometry ( eg LOP )

class DECL_BLND BLEND_EDIT : public ACIS_OBJECT
    {
public:    

    // constructors
	// null
    BLEND_EDIT() {}

    // destructor
    virtual ~BLEND_EDIT() {}

    // data access
	// SPAbox to use for intersections
    virtual const SPAbox &inter_box() const = 0;

	// for getting the final surface ( unchanged or not ) on a coedge
	virtual SURFACE *get_surface( COEDGE *coed ) const = 0;

	// corresponding point on new surface cf old
	virtual logical correspond_point( const COEDGE *coed, 
			SPAposition const &old_pos, double const &t, 
			SPAposition &new_pos ) = 0;


    };


#endif
