/* ORIGINAL: acis2.1/kernutil/box/box.hxx */
/* $Id: SPAbox.hxx,v 1.10 2002/08/16 17:29:42 products Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for SPAbox.

// This class represents a bounding box.  It is implemented as an
// axis-dependent, axis-aligned, rectangular box, given by a triple
// of real intervals.


#if !defined( BOX_CLASS )
#define BOX_CLASS

#include "base.hxx"
#include "dcl_base.h"
#include "debugmsc.hxx"
#include "logical.h"
#include "interval.hxx"

/**
 * @file box.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISGEOMOPERATORS
 *
 * @{
 */

class SPAvector;
class SPAmatrix;
class SPAtransf;
class SPAposition;
class SPAbox;

/**
 * Creates a box that encloses the two given boxes.
 * <br><br>
 * @param box1
 * first box.
 * @param box2
 * second box.
 */
DECL_BASE SPAbox operator|( SPAbox const & box1, SPAbox const & box2);
DECL_BASE SPAbox gme_operator_or( SPAbox const & box1, SPAbox const & box2);
/**
 * Finds the overlap of two boxes that are known to overlap (see SPAbox::operator&&). This method finds the intersection.
 * <br><br>
 * @param box1
 * first box.
 * @param box2
 * second box.
 */
DECL_BASE SPAbox operator&( SPAbox const & box1, SPAbox const & box2);
DECL_BASE SPAbox  gme_operator_and( SPAbox const & box1, SPAbox const & box2);
/**
 * Determines whether two boxes overlap.
 * <br><br>
 * <b>Role:</b> This method returns TRUE if either box is NULL or if all the intervals of one box overlap
 * the corresponding intervals of the other box.
 * <br><br>
 * @param box1
 * first box.
 * @param box2
 * second box.
 */
DECL_BASE logical operator&&( SPAbox const & box1, SPAbox const & box2);
DECL_BASE logical gme_operator_and_and( SPAbox const & box1, SPAbox const & box2);
/**
 * Determines if a given box encloses a given position.
 * <br><br>
 * @param p
 * position.
 * @param b
 * given box.
 */
DECL_BASE logical operator<<( SPAposition const & p, SPAbox const & b );
DECL_BASE logical gme_operator_left_shift( SPAposition const & p, SPAbox const & b );
/**
 * Equality operator for determining whether two boxes are identical.
 * <b>Role:</b> The criteria are strict, so this operator should not be used when arithmetic equality is intended.
 * @param box1
 * first box.
 * @param box2
 * second box.
 */
DECL_BASE logical operator==( SPAbox const &box1, SPAbox const & box2);
DECL_BASE logical gme_operator_equal( SPAbox const &box1, SPAbox const & box2);
/**
 * Inequality operator for determining whether two boxes are not identical.
 * <b>Role:</b> The criteria are strict, so this operator should not be used when arithmetic inequality is intended.
 * @param box1
 * first box.
 * @param box2
 * second box.
 */
DECL_BASE logical operator!=( SPAbox const &box1, SPAbox const &box2 );
DECL_BASE logical gme_operator_no_equal( SPAbox const &box1, SPAbox const &box2 );
/**
 * Translates a box.
 * <br><br>
 * @param box1
 * given box.
 * @param vec1
 * vector.
 */
DECL_BASE SPAbox operator+( SPAbox const & box1, SPAvector const & vec1);
DECL_BASE SPAbox gme_operator_add( SPAbox const & box1, SPAvector const & vec1);
/**
 * Translates a box.
 * <br><br>
 * @param vec1
 * vector.
 * @param box1
 * given box.
 */
DECL_BASE SPAbox operator+( SPAvector const & vec1, SPAbox const & box1);
DECL_BASE SPAbox gme_operator_add( SPAvector const & vec1, SPAbox const & box1);
/**
 * Translates a box.
 * <br><br>
 * @param box1
 * given box.
 * @param vec1
 * vector.
 */
DECL_BASE SPAbox operator-( SPAbox const & box1, SPAvector const & vec1);
DECL_BASE SPAbox gme_operator_sub( SPAbox const & box1, SPAvector const & vec1);
/**
 * Scales a box.
 * <br><br>
 * @param box1
 * given box.
 * @param d
 * scale factor.
 */
DECL_BASE SPAbox operator*( SPAbox const & box1, double d);
DECL_BASE SPAbox gme_operator_multiply( SPAbox const & box1, double d);
/**
 * Scales a box.
 * <br><br>
 * @param d
 * scale factor.
 * @param box1
 * given box.
 */
DECL_BASE SPAbox operator*( double d, SPAbox const & box1);
DECL_BASE SPAbox gme_operator_multiply( double d, SPAbox const & box1);
/**
 * Scales a box.
 * <br><br>
 * @param box1
 * given box.
 * @param d
 * scale factor.
 */
DECL_BASE SPAbox operator/( SPAbox const & box1, double d);
DECL_BASE SPAbox gme_operator_divide( SPAbox const & box1, double d);
/**
 * Transforms a box by the given matrix.
 * <br><br>
 * <b>Role:</b> This results in a box in the new coordinate system that is sufficient to enclose the true transformed box.
 * <br><br>
 * @param box1
 * given box.
 * @param mat1
 * matrix.
 */
DECL_BASE SPAbox operator*( SPAbox const & box1, SPAmatrix const & mat1);
DECL_BASE SPAbox gme_operator_multiply( SPAbox const & box1, SPAmatrix const & mat1);
/**
 * Transforms a box by the given transform.
 * <br><br>
 * <b>Role:</b> This results in a box in the new coordinate system that is sufficient to enclose the true transformed box.
 * <br><br>
 * @param box1
 * given box.
 * @param trans
 * transformation.
 */
DECL_BASE SPAbox operator*( SPAbox const & box1, SPAtransf const & trans);
DECL_BASE SPAbox gme_operator_multiply( SPAbox const & box1, SPAtransf const & trans);
/**
 * Finds the extent of the box along the given direction.
 * <br><br>
 * @param unit_vec
 * direction.
 * @param box1
 * given box.
 */
DECL_BASE SPAinterval operator% ( SPAunit_vector const & unit_vec, SPAbox const & box1);
DECL_BASE SPAinterval gme_operator_percent ( SPAunit_vector const & unit_vec, SPAbox const & box1);
/**
 * Finds the extent of the box along the given direction.
 * <br><br>
 * @param box1
 * given box.
 * @param unit_vec
 * direction.
 */
DECL_BASE SPAinterval operator%( SPAbox const & box1, SPAunit_vector const & unit_vec);
DECL_BASE SPAinterval gme_operator_percent( SPAbox const & box1, SPAunit_vector const & unit_vec);
/**
 * Modifies a SPAbox by increasing it in size by a constant amount in all directions.
 * <br><br>
 * @param b
 * given box.
 * @param delta
 * amount to extend.
 */
DECL_BASE SPAbox enlarge_box(SPAbox const& b, double delta);
DECL_BASE SPAbox gme_enlarge_box(SPAbox const& b, double delta);
/** @} */
/**
 * \addtogroup ACISGEOMETRICATOMS
 *
 * @{
 */
/**
 * Represents a bounding box.
 * <br>
 * <b>Role:</b> This class represents a bounding box. It is implemented as an axis-dependent,
 * axis-aligned rectangular box, given by a triple of real intervals.
 * <br><br>
 * ACIS boxes model entities for algorithm efficiency by constructing a simple bounding
 * shape to surround the model entity (as closely as reasonably possible). When two entities are
 * tested for interaction, the boxes can be tested first to filter out obviously disjoint cases.
 * <br><br>
 * The major items boxed are body, shell, and face. There is an additional model entity,
 * the subshell, that exists solely to provide more efficient box testing. When sensible, a shell containing
 * many faces is subdivided spatially into n subshells of faces. Each subshell fills approximately
 * 1/n of the space filled by the original shell box. The subshells can in turn be subdivided
 * if they contain a sufficient number of faces. A shell or subshell so subdivided can also contain faces directly,
 * when these faces span the majority of the original box, and do not fit into any subshell box.
 * The present algorithm for this subdivision limits <i>n</i> to 2.
 * <br><br>
 * Boxes are axis-aligned rectangular parallelepipeds. Subshell, shell, and body boxes are obtained
 * by determining the overall limits of the boxes of the entities making up the body, shell, or subshell.
 * When two bodies are compared, one must be transformed into the coordinate system of the other in order
 * for the comparison to take place. For preliminary testing, each box is transformed and
 * then boxed in the new coordinate system. This is not optimal, but it is relatively quick.
 * <br><br>
 * Boxes are computed only when needed, and changed entities merely require the existing box (if any) to be deleted;
 * however, after a box is computed it is saved for later reuse. Boxes are not logged for roll back purposes,
 * nor are they saved to a disk file. A box pointer in a roll back record is always set to NULL.
 * After a roll back, such boxes must be recomputed.
 * @see SPAinterval
 */
class DECL_BASE SPAbox : public ACIS_OBJECT {

	SPAinterval xb;	// extent of SPAbox in x direction
	SPAinterval yb;	// extent of SPAbox in y direction
	SPAinterval zb;	// extent of SPAbox in z direction

	friend DECL_BASE SPAbox operator|( SPAbox const &, SPAbox const & );
	friend DECL_BASE SPAbox operator&( SPAbox const &, SPAbox const & );
	friend DECL_BASE logical operator&&( SPAbox const &, SPAbox const & );
	friend DECL_BASE logical gme_operator_and_and( SPAbox const &, SPAbox const & );
	friend DECL_BASE SPAbox operator+( SPAbox const &, SPAvector const & );
	friend DECL_BASE SPAbox operator+( SPAvector const &, SPAbox const & );
	friend DECL_BASE SPAbox operator-( SPAbox const &, SPAvector const & );
	friend DECL_BASE SPAbox operator*( SPAbox const &, double );
	friend DECL_BASE SPAbox operator*( double, SPAbox const & );
	friend DECL_BASE SPAbox operator/( SPAbox const &, double );
	friend DECL_BASE SPAbox operator*( SPAbox const &, SPAmatrix const & );
	friend DECL_BASE SPAbox operator*( SPAbox const &, SPAtransf const & );
	friend DECL_BASE SPAinterval operator%( SPAunit_vector const &, SPAbox const & );
	friend DECL_BASE SPAinterval operator%( SPAbox const &, SPAunit_vector const & );
	friend DECL_BASE logical operator<<( SPAposition const & p, SPAbox const & b );
	friend DECL_BASE logical operator==( SPAbox const &box1, SPAbox const & box2);
	friend DECL_BASE logical gme_operator_equal( SPAbox const &box1, SPAbox const & box2);
	friend DECL_BASE logical operator!=( SPAbox const &box1, SPAbox const & box2);
	friend DECL_BASE SPAbox enlarge_box(SPAbox const& b, double delta);
	friend DECL_BASE SPAbox gme_enlarge_box(SPAbox const& b, double delta);

public:
	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * @param xdir
	 * extent in x-direction.
	 * @param ydir
	 * extent in y-direction.
	 * @param zdir
	 * extent in z-direction.
	 */
	SPAbox( SPAinterval const & xdir, SPAinterval const & ydir, SPAinterval const & zdir);
	SPAbox(const char*, SPAinterval const & xdir, SPAinterval const & ydir, SPAinterval const & zdir);
	// An uninitialised box is empty (because all its intervals are).
	/**
	 * C++ allocation constructor requests memory for this object but does not populate it.
	 */
	SPAbox();
	SPAbox(const char*);

	// Construct an infinitesimal box from one position.
	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * @param old
	 * given point.
	 */
	SPAbox( SPAposition const & old);
	SPAbox(const char*, SPAposition const & old);

	// Construct a box from two positions.
	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * @param first_pt
	 * first point.
	 * @param second_pt
	 * second point.
	 */
	SPAbox( SPAposition const & first_pt, SPAposition const & second_pt);
	SPAbox(const char*, SPAposition const & first_pt, SPAposition const & second_pt);

	// Construct a box as a copy of another box
	/**
	 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
	 * <br><br>
	 * @param old
	 * given box.
	 */
	SPAbox( SPAbox const &old );
	SPAbox(const char*, SPAbox const &old );

	/**
	 * Extracts the low end of the leading diagonal of the box.
	 */
	SPAposition low() const;
	SPAposition gme_low() const;
	/**
	 * Extracts the high end of the leading diagonal of the box.
	 */
	SPAposition high() const;
	SPAposition gme_high() const;
	/**
	 * Extracts the middle of the leading diagonal of the box.
	 */
	SPAposition mid() const;
	SPAposition gme_mid() const;

	/**
	 * Returns the corners of the box - labeled from 0 to 7 where the corners correspond 
	 * to writing the index in binary as x * 4 + y * 2 + z and letting zero corresponds 
	 * to the low values, getting position (x, y, z).
	 * <br><br>
	 * @param index
	 * any value from 0 to 7.
	 */
    SPAposition corner(int index) const;
	SPAposition gme_corner(int index) const;
	/**
	 * Retrieves the x-coordinate range.
	 */
	SPAinterval x_range() const;
	SPAinterval gme_x_range() const;
	/**
	 * Retrieves the y-coordinate range.
	 */
	SPAinterval y_range() const;
	SPAinterval gme_y_range() const;
	/**
	 * Retrieves the z-coordinate range.
	 */
	SPAinterval z_range() const;
	SPAinterval gme_z_range() const;

	/**
	 * Tests if the box is empty.
	 */
	logical empty() const;
	logical gme_empty() const;
	/**
	 * Returns TRUE if any of the coordinate ranges is infinite or FALSE otherwise.
	 */
	logical infinite() const;
	logical gme_infinite() const;
	/**
	 * Returns TRUE if the coordinate ranges are finite below or FALSE otherwise.
	 */
	logical finite_below() const;
	logical gme_finite_below() const;
	/**
	 * Returns TRUE if the coordinate ranges are finite above or FALSE otherwise.
	 */
	logical finite_above() const;
	logical gme_finite_above() const;
	/**
	 * Returns TRUE if the coordinate ranges are finite or FALSE otherwise.
	 */
    logical finite() const { return !infinite(); }
	/**
	 * @brief：判断坐标范围是否有限
	 * @return: 判断结果
	 */
	logical gme_finite() const;
	/**
	 * Returns TRUE if the coordinate ranges are finite or FALSE otherwise.
	 */
	logical bounded() const { return finite(); }
	/**
	 * @brief：判断坐标范围是否有界
	 * @return: 判断结果
	 */
	logical gme_bounded() const;
	/**
	 * Returns TRUE if the coordinate ranges are either finite or finite below or FALSE otherwise.
	 */
	logical bounded_below() const { return finite() || finite_below(); }
	/**
	 * @brief：判断坐标范围是否下有界
	 * @return: 判断结果
	 */
	logical gme_bounded_below() const;
	/**
	 * Returns TRUE if the coordinate ranges are either finite or finite above or FALSE otherwise.
	 */
	logical bounded_above() const { return finite() || finite_above(); }
	/**
	 * @brief：判断坐标范围是否上有界
	 * @return: 判断结果
	 */
	logical gme_bounded_above() const;
	/**
	 * Returns TRUE if any of the coordinate ranges is infinite or FALSE otherwise.
	 */
	logical unbounded() const { return !bounded(); }
	/**
	 * @brief：判断坐标范围是否无界
	 * @return: 判断结果
	 */
	logical gme_unbounded() const;
	/**
	 * Returns TRUE if either any of the coordinate ranges is infinite or the coordinate ranges are finite above or FALSE otherwise.
	 */
	logical unbounded_below() const { return infinite() || finite_above(); }
	/**
	 * @brief: 判断坐标范围是否下无界
	 * @return: 判断结果
	 */
	logical gme_unbounded_below() const;
	/**
	 * Returns TRUE if either any of the coordinate ranges is infinite or the coordinate ranges are finite below or FALSE otherwise.
	 */
 	logical unbounded_above() const { return infinite() || finite_below(); }
	/**
	 * @brief: 判断坐标范围是否上无界
	 * @return: 判断结果
	 */
	logical gme_unbounded_above() const;

	/**
	 * Compounds one box into another - this method extends this box until it also encloses the given box.
	 * <br><br>
	 * @param box
	 * given box.
	 */
	SPAbox & operator|=( SPAbox const & box);
	SPAbox & gme_operator_or_assign( SPAbox const & box);
	/**
	 * Limits one box by another - this method forms the intersection of this box with the given box, which results in this box.
	 * <br><br>
	 * @param box
	 * given box.
	 */
	SPAbox & operator&=( SPAbox const & box);
	SPAbox & gme_operator_and_assign( SPAbox const & box);
	/**
	 * Determines if this box entirely encloses the given point.
	 * <br><br>
	 * <b>Role:</b> This method returns TRUE if this box is NULL or if the given point is strictly
	 * within this box or within this box enlarged by SPAresabs in all six directions (+x, -x, +y, -y, +z, -z).
	 * Otherwise, or if the given point is NULL, this method returns FALSE.
	 * <br><br>
	 * @param pos
	 * given point.
	 */
	logical operator>>( SPAposition const & pos) const;
	logical gme_operator_right_shift( SPAposition const & pos) const;
	/**
	 * Determines if this box entirely encloses the given box.
	 * <br><br>
	 * <b>Role:</b> This method returns TRUE if this box is NULL or if the given box is strictly
	 * within this box or within this box enlarged by SPAresabs in all six directions (+x, -x, +y, -y, +z, -z).
	 * Otherwise, or if the given box is NULL, this method returns FALSE.
	 * <br><br>
	 * @param box
	 * given box.
	 */
	logical operator>>( SPAbox const & box) const;
	logical gme_operator_right_shift( SPAbox const & box) const;
	/**
	 * Determines if the given box encloses this box.
	 * <br><br>
	 * @param b
	 * given box.
	 */
	logical operator<<( SPAbox const &b ) const
		{ return b >> *this; }
	/**
	 * @brief 判断给定box是否包含了本box
	 * @param b 给定box
	 * @return 判断结果
	 */
	logical gme_operator_left_shift( SPAbox const& b ) const;

	/**
	 * Translates this box.
	 * <br><br>
	 * @param vec
	 * vector.
	 */
	SPAbox &operator+=( SPAvector const & vec);
	SPAbox &gme_operator_add_assign( SPAvector const & vec);
	/**
	 * Translates this box.
	 * <br><br>
	 * @param vec
	 * vector.
	 */
	SPAbox &operator-=( SPAvector const & vec);
	SPAbox &gme_operator_sub_assign( SPAvector const & vec);
	/**
	 * Transforms this box by the given transform.
	 * <br><br>
	 * <b>Role:</b> This results in a box in the new coordinate system that is sufficient
	 * to enclose the true transformed box.
	 * <br><br>
	 * @param trans
	 * double.
	 */
	SPAbox &operator*=( double trans);
	SPAbox &gme_operator_multiply_assign( double trans);
	/**
	 * Scales this box.
	 * <br><br>
	 * @param scale
	 * double.
	 */
	SPAbox &operator/=( double scale);
	SPAbox &gme_operator_divide_assign( double scale);
	/**
	 * Transforms this box by the given matrix.
	 * <br><br>
	 * <b>Role:</b>This results in a box in the new coordinate system that is sufficient
	 * to enclose the true transformed box.
	 * <br><br>
	 * @param mat
	 * given matrix.
	 */
	SPAbox &operator*=( SPAmatrix const & mat);
	SPAbox &gme_operator_multiply_assign( SPAmatrix const & mat);
	/**
	 * Transforms this box by the given transform.
	 * <br><br>
	 * <b>Role:</b>This results in a box in the new coordinate system that is sufficient
	 * to enclose the true transformed box.
	 * <br><br>
	 * @param trans
	 * given transformation.
	 */
	SPAbox &operator*=( SPAtransf const & trans);
	SPAbox &gme_operator_multiply_assign( SPAtransf const & trans);
	/**
	 * Outputs a title and information about the box to the debug file or to the specified file.
	 * <br><br>
	 * @param title
	 * title.
	 * @param fp
	 * file pointer
	 */
	void debug( char const * title, FILE * fp = debug_file_ptr ) const;
	void gme_debug( char const * title, FILE * fp = debug_file_ptr ) const;
};

/** @} */
/**
 * \addtogroup ACISGEOMOPERATORS
 *
 * @{
 */

/**
 * @nodoc
 */
inline logical operator<<( SPAposition const & p, SPAbox const & b )
	{ return b >> p; }
/**
 * @brief  判断该点是否在box内
 * @param p 空间点对象
 * @param b 指定box
 * @return 判断结果
 */
logical gme_operator_left_shift(SPAposition const& p, SPAbox const& b);

/** @} */
#endif

