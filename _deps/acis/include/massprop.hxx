/* ORIGINAL: acis2.1/kernbody/massprop/massprop.hxx */
/* $Id: massprop.hxx,v 1.9 2002/08/09 17:18:46 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for mass property class

#if !defined( MASSPROP_CLASS )
#define MASSPROP_CLASS

#include "logical.h"

#include "acis.hxx"
#include "dcl_query.h"

#include "debugmsc.hxx"

#include "tensor.hxx"

#include "matrix.hxx"
#include "position.hxx"
#include "vector.hxx"

/**
* @file massprop.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

/** \addtogroup INTRAPIPHYSICALPROPS
 *  \brief Declared at <massprop.hxx>
 *  @{
 */

class BODY;
class plane;
class SPAtransf;

// Define a class for manipulating generic "mass" properties. This
// will support volumetric, areal and linear measures.
class moments;
/**
 * @nodoc
 * Add (accumulate) mass properties.<br><br>
 * @param mom1
 * first moment.
 * @param mom2
 * second moment.
 */
DECL_QUERY moments operator+(moments const &mom1,moments const &mom2);
/**
 * @nodoc
 * Subtract mass properties.<br><br>
 * @param mom1
 * first moment.
 * @param mom2
 * second moment.
 */
DECL_QUERY moments operator-(moments const &mom1,moments const &mom2);
/**
 * @nodoc
 * Subtract mass properties.<br><br>
 * @param mom
 * moment.
 */
DECL_QUERY moments operator-(moments const &mom);
/**
 * @nodoc
 * Scale the mass properties by a constant factor.<br><br>
 * @param factor
 * factor.
 * @param mom
 * moments.
 */
DECL_QUERY moments operator*(double factor,moments const &mom);
/**
 * @nodoc
 * Scale the mass properties by a constant factor.<br><br>
 * @param mom
 * moments.
 * @param factor
 * factor.
 */
DECL_QUERY moments operator*(moments const &mom,double factor);
/**
 * @nodoc
 * Transform the mass properties to correspond to the transformed body.<br><br>
 * @param mom
 * moments.
 * @param transf
 * transformation.
 */
DECL_QUERY moments operator*(moments const &mom,SPAtransf const &transf);



 /**
  * @nodoc
  * <b>Obsolete:</b> use @href mass_props instead.
  * Manipulates generic mass properties.
  * <br><br>
  * <b>Role:</b> This supports volumetric,
  * real, and linear measures.
  * @see SPAmatrix, SPAposition, SPAvector, symtensor
  */
class DECL_QUERY moments : public ACIS_OBJECT {
	double zeroth_m;
	SPAvector first_m;
	symtensor second_m;

	double zeroth_err;
	double first_err;
	double second_err;

	int nvalid;			// 1 if only the volume has been set, 2 if
						// volume and first moments, 3 if all three
						// are valid. Negative if this mass property
						// has been initialised to zero.

	SPAposition orig;		// origin from which first and second moments
						// are measured.
	logical orig_set;	// TRUE if the origin has been set explicitly,
						// when it is checked during addition or
						// subtraction. If FALSE, compatibility of
						// origins is assumed.

	SPAvector princ_inert;	// principal inertias
	SPAmatrix princ_mat;	// affine transformation to bring axes to
						// principal axes
	logical princ_known;// TRUE if the principal inertias have been
						// calculated since the last change to zeroth_m,
						// first_m or second_m

	// Evaluate the principal inertias and axes (called by
	// princ_inertias() and princ_axes()).

	void calc_princ();

public:
/**
 * Initializes the moment to zero.
 */
	void zero();
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 */
	moments();	// initialises to zero.
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.<br><br>
 * @param mom
 * moment.
 */
	moments( moments const &mom );
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.<br><br>
 * @param dval
 * integer.
 * @param vec
 * vector.
 * @param tens
 * symtensor.
 */
	moments(
				double dval,
				SPAvector const &vec,
				symtensor const &tens
			);

/**
 * C++ assignment operator copies data from another object to <tt>this</tt> object.
 */
    moments& operator=(const moments& mom);
/**
 * Returns the zeroth moment of the moment, or zero if unset.
 */
	double zeroth_moment() const;
/**
 * Returns the first moment of the moments, in a vector, or zeroes if unset.
 */
	SPAvector first_moment() const;
/**
 * Returns the second moments of the moment, in a symtensor, or zeroes if unset.
 */
	symtensor second_moment() const;
/**
 * Returns the error of the zeroth moment, or zero if unset.
 */
	double zeroth_error() const;
/**
 * Returns the error of the first moment, or zero if unset.
 */
	double first_error() const;
/**
 * Returns the error of the second moment, or zero if unset.
 */
	double second_error() const;
/**
 * Returns the origin point from which the first and second moments of the mass property are measured.
 */
	SPAposition origin() const;
/**
 * Sets the zeroth moment of the moment.<br><br>
 * @param dval
 * double.
 */
	void set_zeroth_moment( double dval);
/**
 * Sets the first moment of the moment.<br><br>
 * @param vec
 * vector.
 */
	void set_first_moment( SPAvector const &vec );
/**
 * Sets the second moment of the moment.<br><br>
 * @param tens
 * symtensor.
 */
	void set_second_moment( symtensor const &tens );
/**
 * Sets the zeroth error of a moment.<br><br>
 * @param e
 * double.
 */
	void set_zeroth_error( double e ) { zeroth_err = e; }
/**
 * Sets the first error of a moment.<br><br>
 * @param e
 * double.
 */
	void set_first_error( double e ) { first_err = e; }
/**
 * Sets the second error of a moment.<br><br>
 * @param e
 * double.
 */
	void set_second_error( double e ) { second_err = e; }
/**
 * Sets the origin from which the first and second moments are measured.<br><br>
 * @param pos
 * position.
 */
	void set_origin( SPAposition const &pos );
/**
 * Returns TRUE if no moments are set.
 */
	logical unset() const { return nvalid <= 0; }
/**
 * Returns TRUE if the zeroth moments have been set.
 */
	logical zeroth_set() const { return nvalid >= 1; }
/**
 * Returns TRUE if the first moments have been set.
 */
	logical first_set() const { return nvalid >= 2; }
/**
 * Returns TRUE if the second moments have been set.
 */
	logical second_set() const { return nvalid >= 3; }

	// Calculate the physically significant quantities.
/**
 * Return the centroid.
 */
	SPAposition centroid();
/**
 * Returns the principal moments of inertia.
 */
	SPAvector princ_inertias();
/**
 * Returns the principal axes of the mass_property.
 */
	SPAmatrix princ_axes();

	// Simple linear operators for accumulating mass properties.
/*
// tbrv
*/
/**
 * @nodoc
 */
	friend DECL_QUERY moments operator+(
				moments const &,
				moments const &
			);
/*
// tbrv
*/
/**
 * @nodoc
 */
	friend DECL_QUERY moments operator-(
				moments const &,
				moments const &
			);
/*
// tbrv
*/
/**
 * @nodoc
 */
	friend DECL_QUERY moments operator-(
				moments const &
			);
/**
 * Add (accumulate) moments together.<br><br>
 * @param mom
 * moment.
 */
	moments const &operator+=(
				moments const &mom
			);
/**
 * Subtract moments.<br><br>
 * @param mom
 * moment.
 */
	moments const &operator-=(
				moments const &mom
			);
/*
// tbrv
*/
/**
 * @nodoc
 */
	friend DECL_QUERY moments operator*(
				double,
				moments const &
			);
/*
// tbrv
*/
/**
 * @nodoc
 */
	friend DECL_QUERY moments operator*(
				moments const &,
				double
			);
/**
 * Scale the mass properties by a constant factor.<br><br>
 * @param dval
 * factor.
 */
	moments const &operator*=(
				double dval
			);

	// Transform the mass property to correspond to the transformed
	// body.

/*
// tbrv
*/
/**
 * @nodoc
 */
	friend DECL_QUERY moments operator*(
				moments const &,
				SPAtransf const &
			);
/**
 * Transform the mass properties to correspond to the transformed body.<br><br>
 * @param trans
 * transform.
 */
	moments const &operator*=(
				SPAtransf const &trans
			);

	// Simple value display.
/**
 * Outputs a title line and information about the mass property to the debug file or to the specified file.<br><br>
 * @param title
 * character.
 * @param fp
 * file.
 */
	void debug( char const *title, FILE *fp = debug_file_ptr ) const;

};


// Define a class for manipulating volumetric properties. The name is an
// unfortunate historical aberration. Practically everything translates
// directly to the underlying class functions, the only exception being
// transformation, where everything has an additional factor of the cube of
// the scaling, to take into account the change in volume.

class mass_property;
/**
 * @nodoc
 * Adds two mass properties together.<br><br>
 * @param m1
 * first mass property.
 * @param m2
 * second mass property.
 */
DECL_QUERY mass_property operator+(mass_property const &m1,mass_property const &m2);
/**
 * @nodoc
 * Subtracts the second <tt>mass_property</tt> from the first mass_property.<br><br>
 * @param m1
 * first mass property.
 * @param m2
 * second mass property.
 */
DECL_QUERY mass_property operator-(mass_property const &m1,mass_property const &m2);
/**
 * @nodoc
 * Performs the unary minus operation.<br><br>
 * @param mass
 * mass property.
 */
DECL_QUERY mass_property operator-(mass_property const &mass);
/**
 * @nodoc
 * Scales a <tt>mass_property</tt> by the given real.<br><br>
 * @param dval
 * double value.
 * @param mass
 * mass property.
 */
DECL_QUERY mass_property operator*(double dval,mass_property const &mass);
/**
 * @nodoc
 * Scales a <tt>mass_property</tt> by the given real.<br><br>
 * @param mass
 * mass property.
 * @param dval
 * double value.
 */
DECL_QUERY mass_property operator*(mass_property const &mass,double dval);
/**
 * @nodoc
 * Transforms the <tt>mass_property</tt> to correspond to the transformed body.<br><br>
 * @param mass
 * mass property.
 * @param transf
 * transformation.
 */
DECL_QUERY mass_property operator*(mass_property const &mass,SPAtransf const &transf);
/**
 * @nodoc
 * <b>Obsolete:</b> use @href mass_props instead.
 * Defines a class for manipulating mass properties.
 */
class DECL_QUERY mass_property: public moments {

public:
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.<br><br>
 * @param mass
 * existing mass property.
 */
	mass_property( moments const &mass );
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 */
	mass_property();	// initialises to zero.
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.<br><br>
 * @param mass
 * existing mass property.
 */
	mass_property( mass_property const &mass );
/*
// tbrv
*/
/**
 * @nodoc
 */
	mass_property(
				double,
				SPAvector const &,
				symtensor const &
			);

	// Historical function names.
/**
 * Returns the volume of the <tt>mass_property</tt>.
 */
	double volume() const { return zeroth_moment(); }
/**
 * Sets the volume of the <tt>mass_property</tt>.<br><br>
 * @param v
 * volume.
 */
	void set_volume( double v ) { set_zeroth_moment( v ); }

	// Simple linear operators for accumulating volume properties.

/*
// tbrv
*/
/**
 * @nodoc
 */
	friend DECL_QUERY mass_property operator+(
				mass_property const &,
				mass_property const &
			);
/*
// tbrv
*/
/**
 * @nodoc
 */
	friend DECL_QUERY mass_property operator-(
				mass_property const &,
				mass_property const &
			);
/*
// tbrv
*/
/**
 * @nodoc
 */
	friend DECL_QUERY mass_property operator-(
				mass_property const &
			);
/**
 * Adds two mass properties together.<br><br>
 * @param mass
 * mass property.
 */
	mass_property const &operator+=(
				mass_property const &mass
			);
/**
 * Subtracts the given <tt>mass_property</tt> from this mass_property.<br><br>
 * @param mass
 * mass property.
 */
	mass_property const &operator-=(
				mass_property const &mass
			);
/*
// tbrv
*/
/**
 * @nodoc
 */
	friend DECL_QUERY mass_property operator*(
				double,
				mass_property const &
			);
/*
// tbrv
*/
/**
 * @nodoc
 */
	friend DECL_QUERY mass_property operator*(
				mass_property const &,
				double
			);
/**
 * Scales a <tt>mass_property</tt> by the given real.<br><br>
 * @param dval
 * double value.
 */
	mass_property const &operator*=(
				double dval
			);

	// Transform the mass property to correspond to the transformed
	// body.
/*
// tbrv
*/
/**
 * @nodoc
 */
	friend DECL_QUERY mass_property operator*(
				mass_property const &,
				SPAtransf const &
			);
/**
 * Transforms the <tt>mass_property</tt> to correspond to the transformed body.<br><br>
 * @param transf
 * transformation.
 */
	mass_property const &operator*=(
				SPAtransf const &transf
			);
};

/** @} */

#endif

