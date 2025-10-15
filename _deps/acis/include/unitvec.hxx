/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for SPAunit_vector.
// Unit vectors work just like vectors, and so are a derived type.
#if !defined( UNITVEC_CLASS )
#define UNITVEC_CLASS
#include "base.hxx"
#include "dcl_base.h"
#include "vector.hxx"
/**
 * @file unitvec.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISGEOMOPERATORS
 *
 * @{
 */
class SPAtransf;
class SPAinterval;
/**
 * Performs a unary minus operation.
 * <br><br>
 * @param uv
 * unit vector.
 */
DECL_BASE SPAunit_vector operator-( SPAunit_vector const &uv);
DECL_BASE SPAunit_vector gme_operator_sub( SPAunit_vector const &uv);
/**
 * @nodoc
 * This is already documented in position.hxx
 *
 * Returns a <tt>SPAposition</tt> as the cross-product of a <tt>SPAposition</tt> and a <tt>SPAunit_vector</tt>.
 * <br><br>
 * @param p
 * position.
 * @param uv
 * unit vector.
 */
DECL_BASE SPAposition operator*( SPAposition const &p, SPAunit_vector const &uv);
DECL_BASE SPAposition gme_operator_multiply( SPAposition const &p, SPAunit_vector const &uv);
/**
 * @nodoc
 * This is already documented in position.hxx
 *
 * Returns a <tt>SPAposition</tt> as the cross-product of a <tt>SPAunit_vector</tt> and a <tt>SPAposition</tt>.
 * <br><br>
 * @param uv
 * unit vector.
 * @param p
 * position.
 */
DECL_BASE SPAposition operator*( SPAunit_vector const &uv, SPAposition const &p);
DECL_BASE SPAposition gme_operator_multiply( SPAunit_vector const &uv, SPAposition const &p);
/**
 * @nodoc
 */
DECL_BASE SPAunit_vector normalise( SPAvector const &v );
DECL_BASE SPAunit_vector gme_normalise( SPAvector const &v );
/**
 * Transforms a unit vector by the rotation matrix in a transformation.
 * <br><br>
 * <b>Role:</b> This method returns an error if the transformation contains a shear component.
 * <br><br>
 * @param uv
 * unit vector.
 * @param t
 * transformation.
 */
DECL_BASE SPAunit_vector operator*( SPAunit_vector const &uv, SPAtransf const &t);
DECL_BASE SPAunit_vector gme_operator_multiply( SPAunit_vector const &uv, SPAtransf const &t);
/**
 * Transforms a unit vector by the rotation matrix in a transformation.
 * <br><br>
 * <b>Role:</b> This method returns an error if the transformation contains a shear component.
 * <br><br>
 * @param uv
 * unit vector.
 * @param t
 * transformation.
 */
DECL_BASE SPAunit_vector operator*( SPAunit_vector const &uv, SPAtransf const *t);
DECL_BASE SPAunit_vector gme_operator_multiply( SPAunit_vector const &uv, SPAtransf const *t);
/** @} */
/**
 * \addtogroup ACISGEOMETRICATOMS
 *
 * @{
 */
/**
 * Provides a direction in 3D Cartesian space that has unit length.
 * <br>
 * <b>Role:</b> This class provides a direction in 3D Cartesian space that has unit length. Because it
 * is a derived class of vector, it inherits the functionality of vectors. There are a few
 * operations that are peculiar to unit vectors.
 * @see SPAtransf
 */
class DECL_BASE SPAunit_vector: public SPAvector {

	// Unary minus.
	friend DECL_BASE SPAunit_vector operator-( SPAunit_vector const &uv );

	// Scalar product of a position. This is just as for a vector,
	// but declared explicitly to avoid an ambiguity.
	friend double operator%( SPAposition const &p, SPAunit_vector const &uv );
	friend double operator%( SPAunit_vector const &uv, SPAposition const &p );

	// Find a position as the cross-product of a position with a
	// unit vector.
	friend DECL_BASE SPAposition operator*( SPAposition const &p, SPAunit_vector const &uv );
	friend DECL_BASE SPAposition operator*( SPAunit_vector const &uv, SPAposition const &p );

	// The only way to get back from a vector to a unit_vector is by
	// means of normalise().  We make it explicit, and have no
	// constructor from vector to unit_vector in order to make clear
	// the change in value.
	friend DECL_BASE SPAunit_vector normalise( SPAvector const &v );

	// Transform a unit vector by the rotation matrix in a
	// transformation (gives an error if the transformation contains
	// a shear).
	friend DECL_BASE SPAunit_vector operator*( SPAunit_vector const &uv, SPAtransf const &t );
	friend DECL_BASE SPAunit_vector operator*( SPAunit_vector const &uv, SPAtransf const *t );


public:

	/**
	 * C++ allocation constructor requests memory for this object but does not populate it.
	 */
	SPAunit_vector() {}	// allow unitialised unit_vectors


	// Construct a unit SPAvector from three components.
	// Always normalises result.
	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * <b>Role:</b> Creates a <tt>SPAvector</tt> from three doubles representing the x, y,
	 * and z coordinate values.
	 * <br><br>
	 * @param x
	 * x-coordinate.
	 * @param y
	 * y-coordinate.
	 * @param z
	 * z-coordinate.
	 */
	SPAunit_vector( double x, double y, double z);
	SPAunit_vector( const char *gme, double x, double y, double z);

	// Construct a unit SPAvector from an array of three doubles.
	// Always normalises result.
	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * @param u
	 * array of 3 doubles.
	 */
	SPAunit_vector( double u[ 3 ] );
	SPAunit_vector( const char *gme, double u[ 3 ] );
	/**
	 * Transforms a unit vector by the rotation matrix in a transformation.
	 * <br><br>
	 * <b>Role:</b> This method returns an error if the transformation contains a shear component.
	 * <br><br>
	 * @param t
	 * transform.
	 */
	SPAunit_vector const &operator*=( SPAtransf const &t );
	SPAunit_vector const &gme_operator_multiply_assign( SPAtransf const &t );
};

/** @} */
/**
 * \addtogroup ACISGEOMOPERATORS
 *
 * @{
 */
/**
 * Returns the dot-product of a position and a unit vector.
 * <br><br>
 * @param p
 * position.
 * @param uv
 * unit vector.
 */
inline double operator%( SPAposition const &p, SPAunit_vector const &uv )
{
	double dp;
	dp = p.coord[0] * uv.comp[0], dp += p.coord[1] * uv.comp[1], dp += p.coord[2] * uv.comp[2];
	return dp;
}

/**
 * Returns the dot-product of a unit vector and a position.
 * <br><br>
 * @param uv
 * unit vector.
 * @param p
 * position.
 */
inline double operator%( SPAunit_vector const &uv, SPAposition const &p )
{
	double dp;
	dp = p.coord[0] * uv.comp[0], dp += p.coord[1] * uv.comp[1], dp += p.coord[2] * uv.comp[2];
	return dp;
}

// Standard axis vectors: (1,0,0), (0,1,0), (0,0,1)

// These are doc'ed in Discussion Topics.
/**
 * @nodoc
 */
extern DECL_BASE const SPAunit_vector x_axis, y_axis, z_axis;

// Zero length "unit vector", used for signalling exceptional
// conditions.

// These are doc'ed in Discussion Topics.
/**
 * @nodoc
 */
extern GLOBAL_CONST DECL_BASE SPAunit_vector null_unitvec;



/** @} */
#endif

