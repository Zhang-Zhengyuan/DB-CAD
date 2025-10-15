/* ORIGINAL: 3dt2.1/geomhusk/bnd_line.hxx */
// $Id: bnd_line.hxx,v 1.5 2002/08/09 17:15:14 jeff Exp $
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
//    include file to define class bounded_line
//
//----------------------------------------------------------------------

/*  Prevent this file from being included more than once:             */
#ifndef bnd_line_hxx
#define bnd_line_hxx

// include files---

#include "dcl_kern.h"
#include "logical.h"
#include "bnd_crv.hxx"

/**
* @file bnd_line.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISCURVES
 *
 * @{
 */

class straight;
class bounded_arc;

// class bounded_line adds no new data to bounded_curve.  It only provides some
// additional constructors and redefines some virtual functions.
/**
 * Defines a <tt>bounded_line</tt> as a subtype of <tt>bounded_curve</tt>.
 * <br>
 * <b>Role:</b> This class adds no new data to <tt>bounded_curve</tt>, but it provides additional
 * constructors and redefines some virtual functions.
 * @see bounded_curve
 */
class DECL_KERN bounded_line : public bounded_curve
{
    // make bounded_arc a friend so that it can use the
    // bounded_line::intersect_with_arc method
    friend class bounded_arc;

public:

    // bounded_line constructors

    // default constructor - only used for restore
/**
 * Constructs a <tt>bounded_line</tt> (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it.
 */
     bounded_line();

    // copy constructor
/**
 * Constructs a <tt>bounded_line</tt> from another one (copy constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data from the
 * object supplied as an argument.
 * <br><br>
 * @param b_line
 * bounded line that is to be copied.
 */
     bounded_line(const bounded_line& b_line);

	// create a bounded_line from an EDGE
/**
 * Constructs a <tt>bounded_line</tt> from an <tt>EDGE</tt> and a transform.
 * <br><br>
 * <b>Role:</b> The edge must be a straight.
 * <br><br>
 * @param edge
 * the EDGE used to construct the bounded line.
 * @param transf
 * transformation to be applied to the EDGE's CURVE.
 */
    bounded_line(EDGE* edge, const SPAtransf* trans);

    // line from two positions
/**
 * Constructs a <tt>bounded_line</tt> from two positions.
 * <br><br>
 * @param pos1
 * first position on the constructed line.
 * @param pos2
 * second position on the constructed line.
 */
     bounded_line(const SPAposition& pos1, const SPAposition& pos2);

    // line from straight + 2 parameters
/**
 * Constructs a <tt>bounded_line</tt> from a straight and two parameters values along the straight.
 * <br><br>
 * @param st
 * straight upon which the constructed line is based.
 * @param param1
 * parameter delimiting the constructed line.
 * @param param2
 * parameter delimiting the constructed line.
 */
    bounded_line(straight& st, double param1, double param2);

    // line from SPAposition + direction + distance
/**
 * Constructs a <tt>bounded_line</tt> from a position, a direction, and a distance.
 * <br><br>
 * @param pos
 * starting position for the constructed bounded_line.
 * @param dir
 * direction of the constructed bounded_line.
 * @param dist
 * length of the constructed bounded_line.
 */
    bounded_line(const SPAposition& pos, const SPAunit_vector& dir, double dist);

// bounded_curve virtual methods
//	virtual SPAunit_vector get_normal() const;
/**
 * Changes the start position of this <tt>bounded_line</tt>.
 * <br><br>
 * @param pos
 * the new start position.
 */
    virtual logical change_start_pt(const SPAposition& pos);
/**
 * Changes the end position of this <tt>bounded_line</tt>.
 * <br><br>
 * @param pos
 * the new end position.
 */
    virtual logical change_end_pt(const SPAposition& pos);
/**
 * Finds the closest point on this <tt>bounded_line</tt> to a given pick location and returns the curve parameter value.
 * <br><br>
 * @param loc
 * the pick location.
 */
    virtual double eval_t( const pick_ray& loc ) const;
/**
 * Finds if this <tt>bounded_line</tt> is also a <tt>bounded_line</tt>.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if this is a <tt>bounded_line</tt>; otherwise, it
 * returns <tt>FALSE</tt>.
 */
    virtual logical is_line() const;

  public:

	// make an edge from the line
/**
 * Makes an edge based upon this <tt>bounded_line</tt>.
 */
 	EDGE* make_edge() const;

    // make a copy of the line
/**
 * Makes a transformed copy of this <tt>bounded_line</tt>.
 * <br><br>
 * @param transf
 * transformation to apply to the copy.
 */
    virtual bounded_curve* copy( const SPAtransf* transf =  NULL ) const;

};

// create a line between two points
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN bounded_curve* new_line(
 						const SPAposition& pt1,
 						const SPAposition& pt2
 						);
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN bounded_line* create_line_offset(bounded_line* L,
                                 double dist,
                                 const SPAunit_vector& normal );


/** @} */
#endif
