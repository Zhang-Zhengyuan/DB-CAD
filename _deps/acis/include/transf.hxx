/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for SPAtransf.

// This class records a general transformation of 3D vectors.
// It is in effect a 4 x 3 SPAmatrix to multiply a homogeneous SPAvector,
// but is stored specially to reflect the use to which it is put.


#if !defined( TRANSF_CLASS )
#define TRANSF_CLASS

#include "logical.h"
#include "base.hxx"
#include "dcl_base.h"
#include "debugmsc.hxx"
#include "vector.hxx"
#include "unitvec.hxx"
#include "matrix.hxx"

/**
* @file transf.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISGEOMOPERATORS
 *
 * @{
 */
class SPAunit_vector;

/**
 * @nodoc
 */
DECL_BASE SPAtransf restore_transf(SPAmatrix const&, SPAvector const&, double, logical, logical, logical);
DECL_BASE SPAtransf gme_restore_transf(SPAmatrix const&, SPAvector const&, double, logical, logical, logical);
/**
 * @nodoc
 */
DECL_BASE SPAtransf restore_transf(SPAmatrix const&, SPAvector const&, const double = 1.0);
DECL_BASE SPAtransf gme_restore_transf(SPAmatrix const&, SPAvector const&, const double = 1.0);

/**
* Constructs a transformation retrieving the needed information
* from the provided transformation matrix and the scaling vector.
* <br><br>
* <b>Role:</b> An affine transformation matrix in R^{3x3} 
* and a translation vector in R^3 provide all the neccessary information
* to create a SPAtransform. This interface does not allow any redundancies. 
* The rotation, reflect and shear flags are set internally.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param affine
* affine transformation matrix.
* @param translation
* translation vector
**/
DECL_BASE SPAtransf make_transf(SPAmatrix const& affine, SPAvector const& translation);
DECL_BASE SPAtransf gme_make_transf(SPAmatrix const& affine, SPAvector const& translation);
/**
* Constructs a transformation that scales uniformly in all directions.
* <br><br>
* <b>Role:</b> The scale factor must be positive definite.  If a transformation
* producing an inversion is desired, one should use a combination of reflections
* rather than a negative scaling.  For example, the transformation
* <pre> T = reflect_transf(x_axis) * reflect_transf(y_axis) * reflect_transf(z_axis)</pre>
* may be used to point-reflect an object about the origin.
* <br><br>
* <b>Errors:</b> The scale factor is not positive definite.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param scale
* scale factor.
**/
DECL_BASE SPAtransf scale_transf( double scale );
DECL_BASE SPAtransf gme_scale_transf( double scale );
/**
* Constructs a transformation that scales non-uniformly in the x, y, and z directions.
* <br><br>
* <b>Role:</b> When the desired transformation is effectively scalar (i.e., all three 
* scale factors are within <tt>SPAresabs</tt> of one another), this function uses 
* <tt>scale_transf(double)</tt> to construct the desired transformation. In all other 
* cases, application of the returned transformation to an <tt>ENTITY</tt> invokes the 
* Space Warping component. 
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param xs
* x scale factor.
* @param ys
* y scale factor.
* @param zs
* z scale factor.
**/
DECL_BASE SPAtransf scale_transf( double xs, double ys, double zs );
DECL_BASE SPAtransf gme_scale_transf( double xs, double ys, double zs );
/**
* Constructs a transformation that both scales non-uniformly and shears.
* <br><br>
* <b>Role:</b> When the desired transformation is effectively scalar (i.e., the <tt>x</tt>, 
* <tt>y</tt>, and <tt>z</tt> scale factors are within <tt>SPAresabs</tt> of one another and 
* the shear factors are all within <tt>SPAresabs</tt> of zero), this function uses 
* <tt>scale_transf(double)</tt> to construct the desired transformation.  In all other cases, 
* application of the returned transformation to an <tt>ENTITY</tt> invokes the Space Warping 
* component. 
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param xs
* x scale factor.
* @param ys
* y scale factor.
* @param zs
* z scale factor.
* @param xys
* xy shear factor.
* @param xzs
* xz shear factor.
* @param yzs
* yz shear factor.
**/
DECL_BASE SPAtransf scale_transf( double xs, double ys, double zs, double xys, double xzs, double yzs );
DECL_BASE SPAtransf gme_scale_transf(double xs, double ys, double zs, double xys, double xzs, double yzs);


/**
* Constructs a shear transformation.
* <br><br>
* <b>Role:</b> Non-zero values supplied as input to this function result
* in a transformation that involves changing the angles between axes.
* This shearing is more than just non-uniform scaling. Shearxy is a ratio
* of the displacement of the x coordinate as the y coordinate changes.
* When shearxy = 1, the x axis appears at a 45-degree angle to how 
* it normally appears; a value of 2 results in a 63.435-degree angle.
* Note that application of such a transformation to an <tt>ENTITY</tt>
* makes use of the Space Warping component.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param shearxy
* Shears the x coord as the y coord changes.
* @param shearxz
* Shears the x coord as the z coord changes.
* @param shearyz
* Shears the y coord as the z coord changes.
**/
DECL_BASE SPAtransf shear_transf( double shearxy, double shearxz, double shearyz);
DECL_BASE SPAtransf gme_shear_transf( double shearxy, double shearxz, double shearyz);

/**
* Constructs a transformation corresponding to a simple rotation by an angle about 
* a given axis.
* <br><br>
* <b>Role:</b> The angle should be specified in radians.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param angle
* rotation angle in radians.
* @param axis
* axis of rotation.
**/
DECL_BASE SPAtransf rotate_transf( double angle, SPAvector const &axis );
DECL_BASE SPAtransf gme_rotate_transf(double angle, SPAvector const& axis);


/**
* Constructs a transformation corresponding to a reflection through a plane, 
* specified by its normal.
* <br><br>
* <b>Role:</b> The vector defines the normal to a plane passing through the
* origin. The function generates a transformation matrix describing a reflection
* across this plane.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param axis
* normal to the reflection plane.
**/
DECL_BASE SPAtransf reflect_transf( SPAvector const &axis );
DECL_BASE SPAtransf gme_reflect_transf(SPAvector const& axis);
/**
* Constructs a transformation corresponding to a translation by a given vector.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param disp
* displacement vector defining the translation.
**/
DECL_BASE SPAtransf translate_transf( SPAvector const &disp );
DECL_BASE SPAtransf gme_translate_transf( SPAvector const &disp );
/**
* Constructs a coordinate transformation.
* <br><br>
* <b>Role:</b> Creates a transformation that carries the origin to the given position, and
* the <i>x</i>-axis and <i>y</i>-axis to the given unit vectors. If the second unit vector is not
* orthogonal to the first, this method uses an orthogonal unit vector in the plane of the two
* given vectors to direct the <i>y</i>-axis.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param new_origin
* position to which the origin is to be translated.
* @param new_x_axis
* vector with which the x-axis is to be aligned.
* @param new_y_axis
* vector with which the y-axis is to be aligned.
**/
DECL_BASE SPAtransf coordinate_transf( SPAposition const &new_origin, SPAunit_vector const &new_x_axis,
	SPAunit_vector const &new_y_axis );
DECL_BASE SPAtransf gme_coordinate_transf( SPAposition const &new_origin, SPAunit_vector const &new_x_axis,
	SPAunit_vector const &new_y_axis );
/**
 * Creates the composition of two transforms.
 * <br><br>
 * @param t1
 * first transformation.
 * @param t2
 * second transformation.
 */
DECL_BASE SPAtransf operator*( SPAtransf const &t1, SPAtransf const &t2 );
DECL_BASE SPAtransf gme_operator_multiply( SPAtransf const &t1, SPAtransf const &t2 );

/**
 * Creates the composition of two transforms.
 * <br><br>
 * @param t1
 * first transformation.
 * @param t2
 * second transformation.
 */
DECL_BASE SPAtransf operator*( SPAtransf const &t1, SPAtransf const *t2);
DECL_BASE SPAtransf gme_operator_multiply( SPAtransf const &t1, SPAtransf const *t2);

/**
 * @nodoc
 * This is already documented in vector.hxx
 *
 * Transforms a vector, ignoring the translation part of the transformation.
 * <br><br>
 * @param v
 * vector to be transformed.
 * @param t
 * transformation to be applied.
 */
DECL_BASE SPAvector operator*( SPAvector const &v, SPAtransf const &t );
DECL_BASE SPAvector gme_operator_multiply( SPAvector const &v, SPAtransf const &t );

/**
* @nodoc
* This is already documented in unitvec.hxx
*
* Transforms a unit vector.
* <br><br>
* <b>Role:</b> This function ignores the translation and scaling parts of the
* transformation, but complains if there is a shear.
* <br><br>
* @param uv
* unit vector to be transformed.
* @param t
* transformation to be applied.
*/
DECL_BASE SPAunit_vector operator*( SPAunit_vector const &uv, SPAtransf const &t );
DECL_BASE SPAunit_vector gme_operator_multiply( SPAunit_vector const &uv, SPAtransf const &t );

/**
* @nodoc
* This is already documented in position.hxx
*
* Transforms a position.
* <br><br>
* @param p
* position to be transformed.
* @param t
* transformation to be applied.
*/
DECL_BASE SPAposition operator*( SPAposition const &p, SPAtransf const &t );
DECL_BASE SPAposition gme_operator_multiply( SPAposition const &p, SPAtransf const &t );

/** @} */

/**
 * \addtogroup ACISGEOMETRICATOMS
 *
 * @{
 */
/**
 * Represents the decomposition of a general transformation into a series of 
 * elementary transformations.
 * <br>
 * <b>Role:</b> Used by the <tt>SPAtransf::compose method</tt>.
 */
struct DECL_BASE transf_decompose_data
{
	/**
	 * Uniform scaling iff <tt>scalex</tt> == <tt>scaley</tt> == <tt>scalez</tt>.
	 */
	double		scalex;		// uniform scaling iff scalex == scaley == scalez
	/**
	 * Same as <tt>scalex</tt> if uniform scaling.
	 */
	double		scaley;		// should be the same as scalex if uniform scaling
	/**
	 * Same as <tt>scalex</tt> if uniform scaling.
	 */
	double		scalez;		// should be the same as scalex if uniform scaling
	/**
	 * Shears x as y changes, normally 0.
	 */
	double		shearxy;	// shear x as y changes, normally 0
	/**
	 * Shears x as z changes, normally 0.
	 */
	double		shearxz;	// shear x as z changes, normally 0
	/**
	 * Shears y as z changes, normally 0.
	 */
	double		shearyz;	// shear y as z changes, normally 0
	/**
	 * Radians rotated around x axis.
	 */
	double		rotatex;	// radians rotated around x axis
	/**
	 * Radians rotated around y axis.
	 */
	double		rotatey;	// radians rotated around y axis
	/**
	 * Radians rotated around z axis.
	 */
	double		rotatez;	// radians rotated around z axis
	/**
	 * Translation in x.
	 */
	double		translatex;	// translation in x
	/**
	 * Translation in y.
	 */
	double		translatey;	// translation in y
	/**
	 * Translation in z.
	 */
	double		translatez;	// translation in z
	/**
	 * Radians rotated about <tt>rotate_axis</tt> (alternative representation).
	 */
	double		rotate_radians;	
	/**
	 * Rotation axis (alternative representation).
	 */
	SPAunit_vector	rotate_axis;	

	/**
	* Reflection plane normal. Root point of the plane is origin.
	*/
	SPAunit_vector	reflection_plane_normal;

	/**
	* Default constructor.
	*/
	transf_decompose_data();
};

/**
 * Represents a general 3D affine transformation.
 * <br>
 * <b>Role:</b> This class is used to represent any combination of translations,
 * rotations, reflections, shearing, and scaling.  When shearing and/or non-uniform
 * scaling are present, the application of such transforms to an <tt>ENTITY</tt>
 * makes use of the Space Warping component.
 * @see SPAmatrix, SPAvector, SPAposition
 */
class DECL_BASE SPAtransf {

	SPAmatrix affine_part;			// always normalised: det == + or - 1
	SPAvector translation_part;
	double scaling_part;

	unsigned int rotate_flag : 1;	// TRUE is there is a rotation
	unsigned int reflect_flag : 1;	// TRUE if there is a reflection
	unsigned int shear_flag : 1;	// TRUE if there is a shear
	unsigned int identity_flag : 1;	// TRUE if this is known to be the identity

	// Simple constructor.  This is private, as we don't want to check
	// the SPAmatrix for unit determinant every time.
public:

	/**
	 * @nodoc
	 */
	MMGR_FREELIST_THIS
#if 0
		; // semicolon needed for mkman (doc tool) parsing
#endif

	// NOTE: This has been made public so read_transf can access it.
	// read_transf used to be its friend, but can't anymore since
	// they are now in two different components, or dlls.
	// bottom line: Be good and DONT USE THIS CONTRUCTOR!
/**
 * @nodoc
 */
	SPAtransf(SPAmatrix const &, SPAvector const &, double,
		logical, logical, logical);
	SPAtransf(char const* gme, SPAmatrix const &, SPAvector const &, double,
				logical, logical, logical);
	// WARNING: Don't use the above constructor! Treat it as private.

public:
	/**
	 * C++ allocation constructor requests memory for this object and initializes it to the identity transform.
	 */
	SPAtransf();			// creates the identity transformation
		SPAtransf(char const* gme);

/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param t
 * transformation.
 */
	SPAtransf(SPAtransf const &t);	// makes a copy
	SPAtransf(char const* gme, SPAtransf const &t);
	//void update();           // check parameters to update status

	// print data
/**
 * Prints the transform data.
 */
	void print() const;
	void gme_print() const;
	// Data reading routines. These all handle a null transformation
	// by treating it as the identity.

/**
 * Returns the affine portion of the transformation.
 * <br><br>
 * <b>Role:</b> Always normalized: <tt>det == + or -1</tt>.
 */
	SPAmatrix affine() const;
	SPAmatrix gme_affine() const;

	/**
	 * Returns the vector representing the translational portion of the transformation.
	 */
	SPAvector translation() const;
	SPAvector gme_translation() const;
	/**
	 * Returns the scaling factor of the transformation.
	 */
	double scaling() const;
	double gme_scaling() const;
	/**
	 * Queries whether or not the transformation has a rotational component.
	 */
	logical rotate() const;
	logical gme_rotate() const;
	/**
	 * Queries whether or not the transformation is reflecting.
	 */
	logical reflect() const;
	logical gme_reflect() const;
	/**
	 * Queries whether or not the transformation has a shearing component.
	 */
	logical shear() const;
	logical gme_shear() const;
	/**
	 * Queries whether or not the transformation is the identity.
	 */
	logical identity() const;
	logical gme_identity() const;

	// Set all of the components of the transformation. Sets the
	// identity flag if all components are exactly right.
/**
 * @nodoc
 */
	friend DECL_BASE SPAtransf restore_transf(
		SPAmatrix const &,
		SPAvector const &,
		double,
		logical,
		logical,
		logical
	);

	friend DECL_BASE SPAtransf gme_restore_transf(
		SPAmatrix const&,
		SPAvector const&,
		double,
		logical,
		logical,
		logical
	);

	// Construction routines. All set the identity flag if the arguments
	// are == the appropriate values.

	// Set transformation to arg * identity.
/**
 * @nodoc
 */
	friend DECL_BASE SPAtransf scale_transf(double scale);
	friend DECL_BASE SPAtransf gme_scale_transf(double scale);

	// Set transformation to differential scaling. Note that such
	// a transformation is not supported in the rest of Acis.
/**
 * @nodoc
 */
	friend DECL_BASE SPAtransf scale_transf(double xs, double ys, double zs);
	friend DECL_BASE SPAtransf gme_scale_transf(double xs, double ys, double zs);
	/**
	 * @nodoc
	 */
	friend DECL_BASE SPAtransf scale_transf(double xs, double ys, double zs, double xys, double xzs, double yzs);
	friend DECL_BASE SPAtransf gme_scale_transf(double xs, double ys, double zs, double xys, double xzs, double yzs);


	// Set transformation to shearing. Note that such
	// a transformation is not supported in the rest of Acis.
/**
 * @nodoc
 */
	friend DECL_BASE SPAtransf shear_transf(double shearxy, double shearxz, double shearyz);
        friend DECL_BASE SPAtransf gme_shear_transf(double shearxy, double shearxz, double shearyz);
	// Set transformation to rotation by angle about SPAvector.

/**
 * @nodoc
 */
	friend DECL_BASE SPAtransf rotate_transf(double angle, SPAvector const &axis);
	friend DECL_BASE SPAtransf gme_rotate_transf(double angle, SPAvector const& axis);


	// Set transformation to reflection about plane through
	// origin and perpendicular to given SPAvector.

/**
 * @nodoc
 */
	friend DECL_BASE SPAtransf reflect_transf(SPAvector const &axis);
	friend DECL_BASE SPAtransf gme_reflect_transf(SPAvector const& axis);


	// Set transformation to translation along SPAvector.

/**
 * @nodoc
 */
	friend DECL_BASE SPAtransf translate_transf(SPAvector const &disp);
        friend DECL_BASE SPAtransf gme_translate_transf(SPAvector const &disp);

	// Set transformation to carry origin to given SPAposition, and
	// x and y axes to the given unit vectors.  If the second unit
	// SPAvector is not orthogonal to the first, uses instead a
	// unit SPAvector in the plane of the two given vectors, that is.

/**
 * @nodoc
 */
	friend DECL_BASE SPAtransf coordinate_transf(
		SPAposition const &new_origin,
		SPAunit_vector const &new_x_axis,
		SPAunit_vector const &new_y_axis);
	friend DECL_BASE SPAtransf gme_coordinate_transf(
		SPAposition const &new_origin,
		SPAunit_vector const &new_x_axis,
		SPAunit_vector const &new_y_axis);

	// Compare two transformations. This does not allow any tolerance,
	// so is not a general equality operator, but may be assumed to
	// return TRUE if one argument is a copy of the other.

/**
 * Compares two transformations.
 * <br><br>
 * <b>Role:</b> This method does not allow any tolerance. It is therefore not a general
 * equality operator, but returns <tt>TRUE</tt> if one transform is a copy of the other.
 * <br><br>
 * @param t
 * transformation with which to compare.
 */
	logical operator==(
		SPAtransf const &t
		) const;
	logical gme_operator_equal(SPAtransf const &t) const;

	/**
	 * Compares two transformations.
	 * <br><br>
	 * <b>Role:</b> This method does not allow any tolerance. It is therefore not a general
	 * equality operator, but returns <tt>FALSE</tt> if one transform is a copy of the other.
	 * <br><br>
	 * @param rhs
	 * transformation with which to compare.
	 */
	logical operator!=(
		SPAtransf const &rhs
		) const
	{
		return !(*this == rhs);
	}

	logical gme_operator_not_equal(SPAtransf const& rhs) const;

	// Multiply two transformations.
/**
 * @nodoc
 */
	friend DECL_BASE SPAtransf operator*(SPAtransf const &t1, SPAtransf const &t2);
	friend DECL_BASE SPAtransf gme_operator_multiply(SPAtransf const &t1, SPAtransf const &t2);
	/**
	 * @nodoc
	 */
	friend DECL_BASE SPAtransf operator*(
		SPAtransf const &t1,
		SPAtransf const *t2
		)
	{
		return t1 * (*t2);
	}
	friend DECL_BASE SPAtransf operator*( SPAtransf const& t1, SPAtransf const* t2 );

	/**
	 * Composes this transformation with another.
	 * <br><br>
	 * @param t
	 * transformation with which to form a composition.
	 */
	SPAtransf const &operator*=(SPAtransf const &t);
	SPAtransf gme_operator_multiply_assign(SPAtransf const &t);


	// Return the inverse transformation (must be no shear in the
	// given transformation).

/**
 * Returns the inverse transformation.
 * <br><br>
 * <b>Role:</b> There must be no shear in the given transformation.
 */
	SPAtransf inverse() const;
	SPAtransf gme_inverse() const;

	// Decompose a non-degenerate SPAtransf into data that represents a sequence
	// of scaling, shearing, rotating, reflecting and translating.
	// This function will break the SPAtransf into various floating point numbers
	// that represent a sequence of transformations.
	//
	// Reflection is supported in the limited scope. 
	// The return value (logical flag) of this method depends on following conditions.
	//		1. return TRUE 
	//			- if it do not have reflection part.
	//			- if it has reflection part with zero shear, uniform scaling and no rotation.
	//		2. return FALSE
	//			- if it has any combination of reflection, shear, non-uniform scaling and rotation.
	//
	// There are many sequences of transformations that can lead to a particular transformation.
	// With the output data from this function one could reconstruct a SPAtransf that
	// would be an equivalent of the original SPAtransf.
	//
	// If the output DO NOT have reflection part then one can reconstruct SPAtransf by following sequence. 
	// The sequence to reconstructing a SPAtransf from the output of this function is:
	//     [scalex][scaley][scalez][shearxy]shearxz][shearyz]
	//     [rotatex][rotatey][rotatez][translatex][translatey][translatez]
	//
	// One can use an alternative representation for rotation and can substitute a single rotation,
	// [rotate_radians/rotate_axis], for [rotatex][rotatey][rotatez]
	// Then the sequence to reconstructing a SPAtransf is as follows:
	//     [scalex][scaley][scalez][shearxy]shearxz][shearyz]
	//     [rotate_radians/rotate_axis][translatex][translatey][translatez]
	//
	// If the output DO have reflection part then one can reconstruct SPAtransf by following sequence.
	//	   [scalex][scaley][scalez] [reflection_plane_normal] [translatex][translatey][translatez]
	//
	// In C code this would be:
	//		SPAtransf orig = get_from_somewhere_a_transf();
	//		SPAtransf recon;
	//		transf_decompose_data tdata;
	//		orig.decompose(orig, tdata);
	//
	//		if (tdata.scalex == tdata.scaley && tdata.scalex == tdata.scalez)
	//			recon *= scale_transf(tdata.scalex);
	//		else
	//			recon *= scale_transf(tdata.scalex, tdata.scaley, tdata.scalez);
	//		if (tdata.shearxy || tdata.shearxz || tdata.shearyz)
	//			recon *= shear_transf(tdata.shearxy, tdata.shearxz, tdata.shearyz);
	//
	//		//check for the zero value of rotation(rotatex, rotatey, rotatez)
	//		if (rotate_xyz_axes)
	//		{
	//			recon *= rotate_transf(tdata.rotatex, SPAvector(1, 0, 0));
	//			recon *= rotate_transf(tdata.rotatey, SPAvector(0, 1, 0));
	//			recon *= rotate_transf(tdata.rotatez, SPAvector(0, 0, 1));
	//		}
	//		else
	//		{
	//			if (FALSE == data.rotate_axis.is_zero())
	//			{
	//				recon *= rotate_transf(tdata.rotate_radians, tdata.rotate_axis);
	//			}
	//		}
	//
	//		if (FALSE == data.reflection_plane_normal.is_zero())
	//		{
	//			recon *= reflect_transf(tdata.reflection_plane_normal);
	//		}
	//
	//		recon *= translate_transf(
	//				SPAvector(tdata.translatex, tdata.translatey, tdata.translatez));
	//		// Now orig and recon are equivalent.
	//
	// Most ACIS functions do not support shearing, and therefore the shearing
	// parameters would most of the time be zero. They would be non-zero when one
	// performs a rotation and then non-uniform scaling. Again, most ACIS functions
	// do not support shearing, but this inforation does come in handy when using
	// the SPAtransf class for other things such as bitmaps.
	//
	// The original SPAtransf and the reconstructed SPAtransf may not be exactly equal, but
	// they are equivalent. This means that the internal matrices and other parameters
	// may not be same, but the transformation of a point by either SPAtransf will yield
	// the same results.
/**
 * Decomposes a non-degenerate transformation into data that represent a unique sequence of 
 * scaling, shearing, rotating, and translating.
 * <br><br>
 * Composition of reflection transform with rotation, shear and non uniform scaling is not supported with decompose structure.
 * <br><br>
 * @param data
 * output data.
 */
	logical decompose(
		transf_decompose_data& data) const;
		logical gme_decompose(
		transf_decompose_data& data) const;

	// Construct a SPAtransf from the data supplied from the method decompose().
/**
 * Interprets the <tt>transf_decompose_data</tt> structure as a series of transformations.
 * <br><br>
 * <b>Role:</b> If the logical <tt>rotate_xyz_axes</tt> is <tt>TRUE</tt>, the sequence is:
 * <tt>[scalex] [scaley] [scalez] [shearxy] [shearxz] [shearyz]
 * [rotatex] [rotatey] [rotatez] [reflection_plane_normal] [translatex] [translatey] [translatez].</tt>
 * <br><br>
 * Or, if the logical <tt>rotate_xyz_axes</tt> is <tt>FALSE</tt>, the sequence is:
 * <tt>[scalex] [scaley] [scalez] [shearxy] [shearxz] [shearyz]
 * [rotate_radians / rotate_axis] [reflection_plane_normal]  [translatex] [translatey] [translatez].</tt>
 * <br><br>
 * @param data
 * input data.
 * @param rotate_xyz_axes
 * flag indicating the format of rotation data.
 */
	logical compose(
		const transf_decompose_data& data,	// INPUT: the transformation data
		 logical rotate_xyz_axes);	// INPUT: if TRUE, use rotatex, rotatey, and rotatez
									// otherwise use rotate_radians and rotate_axis
		logical gme_compose(
		const transf_decompose_data& data,	// INPUT: the transformation data
		 logical rotate_xyz_axes);	// INPUT: if TRUE, use rotatex, rotatey, and rotatez
									// otherwise use rotate_radians and rotate_axis

	// Transform a SPAvector, ignoring the translation part of the
	// transformation.
/**
 * @nodoc
 */
	friend DECL_BASE SPAvector operator*( SPAvector const &v, SPAtransf const &t );
	friend DECL_BASE SPAvector gme_operator_multiply( SPAvector const &v, SPAtransf const &t );


	// Transform a unit SPAvector.  Ignores the translation and
	// scaling parts, but complains if there is a shear
/**
 * @nodoc
 */
	friend DECL_BASE SPAunit_vector operator*( SPAunit_vector const &uv,
									SPAtransf const &t );
	friend DECL_BASE SPAunit_vector gme_operator_multiply( SPAunit_vector const &uv,
									SPAtransf const &t );

	// Transform a SPAposition.  Uses the complete 4x3 transformation.
/**
 * @nodoc
 */
	friend DECL_BASE SPAposition operator*( SPAposition const &p, SPAtransf const &t );
	friend DECL_BASE SPAposition gme_operator_multiply( SPAposition const &p, SPAtransf const &t );


	// Make read_transf a friend so that the non-checking private
	// constructor can be used when reading a SPAtransf from file.

// 	friend SPAtransf read_transf();
	// We can't be friends anymore since we're in two different dlls.
	// Instead, the contructor above was made public, with serious warnings
	// about anyone using it except read_transf.


	// Output details of a SPAtransf.
/**
 * Outputs the details of a transform to the specified file.
 * <br><br>
 * @param strlead
 * leader string.
 * @param fp
 * file pointer.
 */
	void debug( char const *strlead, FILE * fp= debug_file_ptr ) const;
	void gme_debug( char const *strlead, FILE * fp= debug_file_ptr ) const;
};

/**
 * @nodoc
 */
extern GLOBAL_CONST DECL_BASE SPAtransf identity_transform;

/** @} */
#endif

