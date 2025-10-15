/* ORIGINAL: 3dt2.1/geomhusk/bnd_point.hxx */
// $Id: bnd_pnt.hxx,v 1.5 2002/08/09 17:15:14 jeff Exp $
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
//    include file to define class bounded_point
//
//----------------------------------------------------------------------

/*  Prevent this file from being included more than once:             */
#ifndef bnd_point_hxx
#define bnd_point_hxx

// include files---

#include "dcl_kern.h"
#include "logical.h"
#include "bnd_crv.hxx"

class degenerate_curve;

// class bounded_point adds no new data to bounded_curve.  It only provides some
// additional constructors and redefines some virtual functions.

class DECL_KERN bounded_point : public bounded_curve
{

  public:

    // bounded_point constructors

    // default constrautor - only used for restore
    bounded_point();

    // copy constructor
    bounded_point(const bounded_point&);

	// create a bounded_point from an EDGE
    bounded_point(EDGE*, const SPAtransf*);

    // point from a SPAposition
    bounded_point(const SPAposition&);

    // point from a degenerate curve
    bounded_point(const degenerate_curve&);

// bounded_curve virtual methods
//	virtual SPAunit_vector get_normal() const;
//    virtual logical change_start_pt(const SPAposition&);
//    virtual logical change_end_pt(const SPAposition&);
//    virtual double eval_t( const  pick_ray& ) const;

    virtual logical is_point() const;

  public:

	// make an edge from the point
//	EDGE* make_edge() const;

    // make a copy of the point
    virtual bounded_curve*  copy( const SPAtransf* =  NULL  ) const;

};

// create a point between two points
DECL_KERN bounded_curve* new_point(
 						const SPAposition& pt
 						);


#endif
