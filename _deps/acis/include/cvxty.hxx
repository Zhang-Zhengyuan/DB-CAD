/* $Id: cvxty.hxx,v 1.7 2001/01/11 18:06:56 ywoo Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef CVXTY_H
#define CVXTY_H

// Indent style: Acis

// History:
//
// 28-Jun-99 DAP	New code.

#include "logical.h"

#include "dcl_intr.h"

#include "debugmsc.hxx"
// ywoo 08Jan01: added the header file.
#include "static_types.hxx"
// ywoo: end

// These constants are provided for convenience as input to the
// cvty constructor (see below).

/**
* @file cvxty.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

/** \addtogroup INTRAPICONVEXITY
 *  \brief Declared at <cvxty.hxx>, SPAintr
 *  @{
 */

/*
// tbrv
*/
/**
 * @nodoc
 */
extern GLOBAL_CONST DECL_INTR unsigned int cvty_unset;

/*
// tbrv
*/
/**
 * @nodoc
 */
extern GLOBAL_CONST DECL_INTR unsigned int cvty_convex;

/*
// tbrv
*/
/**
 * @nodoc
 */
extern GLOBAL_CONST DECL_INTR unsigned int cvty_concave;

/*
// tbrv
*/
/**
 * @nodoc
 */
extern GLOBAL_CONST DECL_INTR unsigned int cvty_tangent;

/*
// tbrv
*/
/**
 * @nodoc
 */
extern GLOBAL_CONST DECL_INTR unsigned int cvty_inflect;

/*
// tbrv
*/
/**
 * @nodoc
 */
extern GLOBAL_CONST DECL_INTR unsigned int cvty_mixed;

/*
// tbrv
*/
/**
 * @nodoc
 */
extern GLOBAL_CONST DECL_INTR unsigned int cvty_knife;

/*
// tbrv
*/
/**
 * @nodoc
 */
extern GLOBAL_CONST DECL_INTR unsigned int cvty_unknown;

// Class representing the convexity at a point or along a single edge
// (or something equivalent), such as "convex", or "tangent_convex" etc.

// For a precise enumeration of all the kinds of convexity value that
// can ever be returned see pt_cvty.hxx (convexity at a single point)
// and ed_cvty.hxx (convexity along an entire edge).

/**
 * Class representing the convexity at a point or along a single edge (or something equivalent), such as "convex", or "tangent_convex".
 * <br><br>
 * <b>Role:</b> For a precise enumeration of all the kinds of convexity value
 * that can ever be returned see <tt>pt_cvty.hxx</tt> (convexity at a single point) and
 * <tt>ed_cvty.hxx</tt> (convexity along an entire edge).
 * @see ed_cvty_info, pt_cvty_info
 */
class DECL_INTR cvty {
public:
	// Generic constructor. You could make, for example, "tangent
	// concave" instance of the class with cvty( cvty_tangent |
	// cvty_concave ). Default argument makes an "unset" convexity.
/**
 * Default constructor.<br><br>
 * @param bits
 * convexity not set.
 */
// jb Mar 2003: mkman doesn't understand ":" syntax in inline constructors.
//	cvty( unsigned int bits = cvty_unset ) : bits_data( bits ) {}
	cvty( unsigned int bits = cvty_unset );
	// Copy constructor, assignment operator, destructor: defaults are fine.

	// Test for an unset convexity.

/**
 * Returns TRUE if this form of convexity applies.
 */
	logical unset() const { return bits_data == cvty_unset; }

	// Data access functions. These are the basic ones. Remember that a
	// function such as convex() will return TRUE for anything with the
	// convex bit set (i.e. including tangent_convex, knife_convex etc.)

/**
 * This will return TRUE for anything with the convex bit set (including <tt>tangent_convex</tt>, <tt>knife_convex</tt>).
 */
	logical convex() const { return bits_data & cvty_convex; }
/**
 * This will return TRUE for anything with the concave bit set.
 */
	logical concave() const { return bits_data & cvty_concave; }
/**
 * Returns TRUE if this form of convexity applies.
 */
	logical tangent() const { return bits_data & cvty_tangent; }
/**
 * This will return TRUE for anything with the inflect bit set.
 */
	logical inflect() const { return bits_data & cvty_inflect; }
/**
 * This will return TRUE for anything with the concave bit set.
 */
	logical mixed() const { return bits_data & cvty_mixed; }
/**
 * This will return TRUE for anything with the knife bit set.
 */
	logical knife() const { return bits_data & cvty_knife; }
/**
 * Returns TRUE if this form of convexity applies.
 */
	logical unknown() const { return bits_data & cvty_unknown; }

	// These are also provided for convenience. They currently include
	// all the forms of convexity that can ever be returned. For a
	// precise description of what they all mean, see the files
	// pt_cvty.hxx and ed_cvty.hxx. Note that these are not mutually
	// exclusive, for example, convex_mixed() will always say TRUE when
	// tangent_convex_mixed() does (but not vice versa).

/**
 * This will return TRUE for anything with the <tt>convex_mixed</tt> bit set.
 */
	logical convex_mixed() const;
/**
 * This will return TRUE for anything with the <tt>concave_mixed</tt> bit set.
 */
	logical concave_mixed() const;
/**
 * Returns TRUE if this form of convexity applies.
 */
	logical tangent_convex() const;
/**
 * Returns TRUE if this form of convexity applies.
 */
	logical tangent_concave() const;
/**
 * Returns TRUE if this form of convexity applies.
 */
	logical tangent_mixed() const;
/**
 * Returns TRUE if this form of convexity applies.
 */
	logical tangent_convex_mixed() const;
/**
 * Returns TRUE if this form of convexity applies.
 */
	logical tangent_concave_mixed() const;
/**
 * Returns TRUE if this form of convexity applies.
 */
	logical tangent_inflect() const;
/**
 * Returns TRUE if this form of convexity applies.
 */
	logical tangent_inflect_convex() const;
/**
 * Returns TRUE if this form of convexity applies.
 */
	logical tangent_inflect_concave() const;
/**
 * Returns TRUE if this form of convexity applies.
 */
	logical tangent_inflect_mixed() const;
/**
 * Returns TRUE if this form of convexity applies.
 */
	logical tangent_inflect_convex_mixed() const;
/**
 * Returns TRUE if this form of convexity applies.
 */
	logical tangent_inflect_concave_mixed() const;
/**
 * This will return TRUE for anything with the <tt>knife_convex</tt> bit set.
 */
	logical knife_convex() const;
/**
 * This will return TRUE for anything with the <tt>knife_concave</tt> bit set.
 */
	logical knife_concave() const;
/**
 * This will return TRUE for anything with the concave bit set.
 */
	logical knife_mixed() const;
/**
 * This will return TRUE for anything with the concave bit set.
 */
	logical knife_convex_mixed() const;
/**
 * This will return TRUE for anything with the <tt>knife_concave_mixed</tt> bit set.
 */
	logical knife_concave_mixed() const;

	// Test 2 convexities for being identical. Seems kind of useful.
/**
 * Test to see if two convexities are identical.<br><br>
 * @param other
 * other convexity.
 */
	logical operator==( cvty const &other ) const
	{
		return bits_data == other.bits_data;
	}

	// Data setting functions. In each case pass the logical as TRUE to
	// turn the property on, FALSE to turn it off. All return *this.

/**
 * Data setting functions.
 * <br><br>
 * <b>Role:</b> Pass the logical as TRUE to turn the property on, FALSE to turn it off.<br><br>
 * @param prop
 * property setting - default is on.
 */
	cvty &set_convex( logical prop= TRUE );
/**
 * Data setting functions.
 * <br><br>
 * <b>Role:</b> Pass the logical as TRUE to turn the property on, FALSE to turn it off.<br><br>
 * @param prop
 * property setting - default is on.
 */
	cvty &set_concave( logical prop= TRUE );
/**
 * Data setting functions.
 * <br><br>
 * <b>Role:</b> Pass the logical as TRUE to turn the property on, FALSE to turn it off.<br><br>
 * @param prop
 * property setting - default is on.
 */
	cvty &set_tangent( logical prop= TRUE );
/**
 * Data setting functions.
 * <br><br>
 * <b>Role:</b> Pass the logical as TRUE to turn the property on, FALSE to turn it off.<br><br>
 * @param prop
 * property setting - default is on.
 */
	cvty &set_inflect( logical prop= TRUE );
/**
 * Data setting functions.
 * <br><br>
 * <b>Role:</b> Pass the logical as TRUE to turn the property on, FALSE to turn it off.<br><br>
 * @param prop
 * property setting - default is on.
 */
	cvty &set_mixed( logical prop= TRUE );
/**
 * Data setting functions.
 * <br><br>
 * <b>Role:</b> Pass the logical as TRUE to turn the property on, FALSE to turn it off.<br><br>
 * @param prop
 * property setting - default is on.
 */
	cvty &set_knife( logical prop= TRUE );
/**
 * Data setting functions.
 * <br><br>
 * <b>Role:</b> Pass the logical as TRUE to turn the property on, FALSE to turn it off.<br><br>
 * @param prop
 * property setting - default is on.
 */
	cvty &set_unknown( logical prop= TRUE );

	// Debug. Prints the "string representation" to the file.
/**
 * Debug printout.
 * <br><br>
 * <b>Role:</b> Outputs a title line and all the curves, pcurves, and surfaces
 * details of the class for inspection to standard output or to the specified file.<br><br>
 * @param fp
 * file to print to.
 */
	void debug( FILE *fp = debug_file_ptr ) const;

	// The readable "string representation" of this thing, in case you
	// want to do other things with it than send it to a file. The
	// passed "str" must be big enough. Returns the given arg for
	// convenience.

/**
 * The readable "string representation" of this.
 * <br><br>
 * <b>Role:</b> In case you want to do other things with it then send it to a file.
 * The passed str must be big enough. Returns the given argument for convenience.<br><br>
 * @param str
 * string size.
 */
	char *string( char *str ) const;

private:
	unsigned int bits_data;
};

// jb Mar 2003: do the inline for default constructor

/**
 * @nodoc
 */
inline cvty::cvty( unsigned int bits ) : bits_data( bits ) {}

/** @} */

#endif
