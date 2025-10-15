/* ORIGINAL: 3dt2.1/geomhusk/bnd_arc.hxx */
// $Id: bnd_arc.hxx,v 1.6 2002/08/09 17:15:14 jeff Exp $
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
//    include file to define class bounded_arc
//
//----------------------------------------------------------------------

/*  Prevent this file from being included more than once:             */
#ifndef bnd_arc_hxx
#define bnd_arc_hxx

// include files---

#include "dcl_kern.h"
#include "logical.h"
#include "bnd_crv.hxx"

/**
* @file bnd_arc.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISCURVES
 *
 * @{
 */

class bounded_line;
class SPAposition;

/*
// tbrv
*/
/**
 * @nodoc
 */
typedef	enum	{	// arc direction sense
	CCW, CW, UNKNOWN_SENSE, COINC_SENSE
}	sense_type;

//======================================================================
// class bounded_arc adds no new data to bounded_curve.  It only provides some
// additional constructors and redefines some virtual functions.

/**
 * Defines a <tt>bounded_arc</tt> as a subtype of a <tt>bounded_curve</tt>.
 * <br>
 * <b>Role:</b> This class adds no new data to the <tt>bounded_curve</tt> class from which it is
 * derived, but it provides additional constructors and redefines some virtual functions.
 * @see bounded_curve, bounded_line
 */
class DECL_KERN bounded_arc : public bounded_curve
{
  public:

// default constructor - used only for restore
/**
 * Constructs a <tt>bounded_arc</tt> (default constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object but does not populate it.
 */
    bounded_arc();

// copy constructor
/**
 * Constructs a <tt>bounded_arc</tt> from another one (copy constructor).
 * <br><br>
 * <b>Role:</b> Requests memory for this object and populates it with the data from the
 * object supplied as an argument.
 * <br><br>
 * @param b_arc
 * bounded arc that is to be copied.
 */
    bounded_arc(const bounded_arc& b_arc);

// create a bounded_arc from an EDGE
/**
 * Constructs a <tt>bounded_arc</tt> from an <tt>EDGE</tt> and a transform.
 * <br><br>
 * <b>Role:</b> The edge must be an ellipse.
 * <br><br>
 * @param edge
 * the EDGE used to construct the bounded arc.
 * @param transf
 * transformation to be applied to the EDGE's CURVE.
 */
    bounded_arc(EDGE* edge, const SPAtransf* transf);

// Construct a bounded_arc given an ACIS curve and start and end parameters.
/*
// tbrv
*/
/**
 * @nodoc
 */
    bounded_arc(const curve*, double, double);

//----------------------------------------------------------------------
// Purpose---
//    Create a bounded_arc which is a full circle given the center, radius,
//    and plane normal.
//----------------------------------------------------------------------
/**
 * Constructs a <tt>bounded_arc</tt> that is a full circle, given its center, radius, and plane normal.
 * <br><br>
 * @param center
 * center of the circle.
 * @param radius
 * radius of the circle.
 * @param normal
 * plane normal to the circle.
 */
    bounded_arc(
        const SPAposition& center,
        double radius,
        const SPAunit_vector& normal
        );

//----------------------------------------------------------------------
// Purpose---
//    Create an arc or circle given a center point, SPAvector to start point,
//    normal SPAvector, and start and end angles \(in radians\)
//    The radius is the length of the major_axis SPAvector
//
// Usage---
//    center_pt + major_axis corresponds to the point at 0 angle on the arc.
//
//----------------------------------------------------------------------
/**
 * Constructs a <tt>bounded_arc</tt> that is a full or partial ellipse, given its center, major axis, plane normal, start and end angles, and radius ratio.
 * <br><br>
 * <b>Role:</b> The angles are in radians. The radius is the length of the major-axis vector.
 * The major-axis direction corresponds to the point at the 0-degree angle on the arc, whose
 * position is given by <tt>center + majax</tt>.
 * <br><br>
 * @param center
 * center.
 * @param normal
 * plane normal to the ellipse.
 * @param majax
 * major axis of the ellipse.
 * @param t0
 * angle from the major axis to the start-point of the arc.
 * @param t1
 * angle from the major axis to the end-point of the arc.
 * @param ratio
 * ratio of the major-axis length to the minor-axis length.
 */
    bounded_arc(
        const SPAposition& center,    // center
        const SPAunit_vector& normal, // normal SPAvector
        const SPAvector& majax,       // SPAvector defining angle=0 axis and radius
        double t0,                 // angle from major_axis to start point
        double t1,                 // angle from major_axis to start point
		double ratio = 1.0         // radius ratio
        );

//----------------------------------------------------------------------
// Purpose---
//    Create a bounded_arc which passes through 3 positions
//
// Design---
//    If the positions are collinear, this routine signals an error.
//
//----------------------------------------------------------------------
/**
 * Constructs a circular <tt>bounded_arc</tt>, given three points on the arc and the logical indicating full or partial.
 * <br><br>
 * <b>Role:</b> If <tt>full</tt> is <tt>TRUE</tt>, a full circle is constructed.  If the
 * positions are collinear, this method returns an error.
 * <br><br>
 * @param pt1
 * first point on the circle.
 * @param pt2
 * second point on the circle.
 * @param pt3
 * third point on the circle.
 * @param full
 * flag to construct a full circle.
 */
    bounded_arc(
        const SPAposition& pt1,
        const SPAposition& pt2,
        const SPAposition& pt3,
        logical full
        );

//----------------------------------------------------------------------
// Purpose---
//    Create a bounded_arc given the center and two edge points
//
// Usage---
//    If pt1 == pt2, then a full circle is created
//
//    normal is only used if center, pt1 and pt2 do not determine a plane
//----------------------------------------------------------------------
/**
 * Constructs a circular <tt>bounded_arc</tt>, given its center, two points on the arc, and its plane normal.
 * <br><br>
 * <b>Role:</b> If <tt>pt1</tt> equals <tt>pt2</tt>, then a full circle is created. Use
 * <tt>normal</tt> only if <tt>center</tt>, <tt>pt1</tt>, and <tt>pt2</tt> do not
 * determine a plane.
 * <br><br>
 * @param center
 * center of the circle.
 * @param pt1
 * first point on the circle.
 * @param pt2
 * second point on the circle.
 * @param normal
 * plane normal to the circle.
 */
    bounded_arc(
        const SPAposition& center,
        const SPAposition& pt1,
        const SPAposition& pt2,
        const SPAunit_vector& normal
        );

//----------------------------------------------------------------------
// Purpose---
//    Create a bounded_arc given two points on the diagonal
//----------------------------------------------------------------------
/**
 * Constructs a circular <tt>bounded_arc</tt>, given two points on the arc's diagonal and its plane normal.
 * <br><br>
 * <b>Role:</b> If the positions are collinear, this method returns an error.
 * <br><br>
 * @param pt1
 * first point on the circle.
 * @param pt2
 * second point on the circle.
 * @param normal
 * plane normal to the circle.
 * @param full
 * flag to construct a full circle.
 */
    bounded_arc(
        const SPAposition& pt1,
        const SPAposition& pt2,
        const SPAunit_vector& normal,
        logical full
        );

//======================================================================
//
// bounded_curve virtual methods
/**
 * Returns the unit-vector normal to this <tt>bounded_arc</tt>.
 */
	virtual SPAunit_vector get_normal() const;
/**
 * Changes the start position of this <tt>bounded_arc</tt>.
 * <br><br>
 * @param start_pos
 * new start position.
 */
    virtual logical change_start_pt(const SPAposition& start_pos);
/**
 * Changes the end position of this <tt>bounded_arc</tt>.
 * <br><br>
 * @param end_pos
 * new end position.
 */
    virtual logical change_end_pt(const SPAposition& end_pos);
/**
 * Determines if this <tt>bounded_arc</tt> is also a <tt>bounded_arc</tt>.
 * <br><br>
 * <b>Role:</b> Returns <tt>TRUE</tt> if this is a <tt>bounded_arc</tt>; otherwise, it
 * returns <tt>FALSE</tt>.
 */
    virtual logical is_arc() const;

// bounded_arc specific inquiry methods

/**
 * Returns the center of this <tt>bounded_arc</tt>.
 */
	SPAposition get_center() const;
/**
 * Returns the radius of this <tt>bounded_arc</tt>.
 */
	double get_radius() const;
/**
 * Returns the radius ratio of this <tt>bounded_arc</tt>.
 */
    double get_radius_ratio() const;
/**
 * Returns the subtended angle of this <tt>bounded_arc</tt>.
 */
	double get_subtend() const;
/**
 * Returns the major axis of this <tt>bounded_arc</tt>.
 */
	SPAvector get_major_axis() const;
/**
 * Returns the sense of this <tt>bounded_arc</tt>.
 */
	sense_type 	get_sense() const;

// modify arc parameters
/**
 * Modifies the center position of this <tt>bounded_arc</tt>.
 * <br><br>
 * @param center
 * the new arc center position.
 */
    void set_center(const SPAposition& center);
/**
 * Modifies the radius of this <tt>bounded_arc</tt>.
 * <br><br>
 * @param rad
 * the new arc radius.
 */
    void set_radius(double rad);
/**
 * Modifies the radius ratio of this <tt>bounded_arc</tt>.
 * <br><br>
 * @param rad_ratio
 * the new arc radius ratio.
 */
    void set_radius_ratio(double rad_ratio);
/**
 * Modifies the plane normal to this <tt>bounded_arc</tt>.
 * <br><br>
 * @param normal
 * the new arc normal.
 */
    void set_normal(const SPAunit_vector& normal);
/**
 * Modifies the major axis of this <tt>bounded_arc</tt>.
 * <br><br>
 * @param maj_axis
 * the new arc major axis.
 */
    void set_major_axis(const SPAvector& maj_axis);
/**
 * Creates a transformed copy of this <tt>bounded_arc</tt>.
 * <br><br>
 * @param transf
 * transformation to apply to the copy.
 */
    virtual bounded_curve* copy( const SPAtransf* transf =  NULL  ) const;

};

//======================================================================

/** @} */
#endif
