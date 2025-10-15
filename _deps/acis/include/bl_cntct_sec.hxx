/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef BLEND_CONTACT_SECTION_HXX
#define BLEND_CONTACT_SECTION_HXX
// Header file for blend contact/section derivatives.
#include "acis.hxx"
#include "dcl_kern.h"
#include "logical.h"
#include <stdio.h>
#include "position.hxx"
#include "param.hxx"
#include "interval.hxx"
#include "vector.hxx"
#include "unitvec.hxx"
#include "blnd_eval_ver.hxx"

/**
 * @file bl_cntct_sec.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup KERNAPI
 *
 * @{
 */
class SPAvector;
class v_bl_contacts;
class blend_section;
class blend_spl_sur;

#define MAX_BLEND_DERIV 5

/**
 * @nodoc
 * A class to contain a point and up to the MAX_BLEND_DERIV'th
 * derivative.
 */
class DECL_KERN pt_derivs : public ACIS_OBJECT
	{
public:
    pt_derivs();

	SPAposition pt;
	SPAvector derivs[ MAX_BLEND_DERIV ];

	// Access the i'th derivative, 0 < i <= MAX_BLEND_DERIV

	SPAvector const &deriv( int i ) const;
	SPAvector &deriv( int i );
	};
/**
 * Information about the contact point of the blend for a blend cross 
 * section.
 */
class DECL_KERN blend_contact : public ACIS_OBJECT
{
public:

	// Construct a contact point object from its bits and pieces.

	blend_contact(
			int n_der = -1,					// Number of derivatives
			pt_derivs *sup_d = NULL,		// Spring position and derivatives.		
			logical sup_d_owned = FALSE,	// Whether derivative object owned.
			double rad = 0.,				// Radius value and
			double const *rad_deriv = NULL,	// derivatives.
			SPApar_pos const &uv =			// Support parameter space point.
				* ( SPApar_pos const * ) NULL_REF,
			blend_contact *n = NULL			// Next contact point.
			);

	// Copy constructor and assignment operator for a contact, these are 
	// unfortunately not trivial due to a workaround to avoid changing
	// public interface of blend_section.

	blend_contact( blend_contact const &orig );
	blend_contact &operator=( blend_contact const &rhs );

	// Create a copy, if the new blend section is provided, ensures that 
	// the pt_derivs not owned by the blend contact point to the new blend
	// section.

	blend_contact *copy( 
					blend_section const &old_sec, 
					blend_section &new_sec 
					) const;

	// Delete the position/derivative object, if owned.

	~blend_contact();

	// The number of derivatives successfully calculated.

	int n_derivatives() const { return _n_der; }
	void set_n_derivatives( int n_der ) { _n_der = n_der; }

	// The position / derivative data of the contact point of the section 
	// on the support.

	SPAposition const &pos() const;
	void set_pos( SPAposition const &p );

	// Set the i-th derivative where i > 0

	SPAvector const &deriv( int i ) const;
	void set_deriv( int i, SPAvector const &d );

	// Access to radius value and its derivatives. These could be different
	// for each contact point of the section if the blend type is a chamfer
	// or elliptical blend.

	double radius() const { return _rad; }
	double radius_derivative( int i ) const;

	// Modify the radius data. 

	void set_radius( double const r ) { _rad = r; }

	// Set the i-th radius derivative where i > 0

	void set_radius_derivative( int i, double const rd );

	// The support parameter space value at the contact point. Check that
	// the uv value is set before accessing.

	logical uv_set() const { return _uv_set; }

	SPApar_pos const &sup_uv() const { return _uv; }
	void set_sup_uv( SPApar_pos const &uv );

	// The contact information associated with the next support of the
	// blend section. 

	blend_contact const *next() const { return _next; }
	blend_contact *next() { return _next; }

	// For linking up the contact chain.

	void set_next( blend_contact *n ) { _next = n; }

	// Contact list chain management.

	// Copy the contact list chain, if the new blend section is provided,
	// ensures that the pt_derivs not owned by the blend contact point to
	// the new blend section.

	blend_contact *copy_chain( 
						blend_section const &this_sec =
							*(blend_section const *)NULL_REF,
						blend_section &new_sec = 
							*(blend_section *) NULL_REF 
						) const;

	// Delete the contact list, starting from this contact point.

	void delete_chain();

private:

	// Number of derivatives successfully calculated.

	int _n_der;

	// The position/derivative of the blend at the contact point. The object
	// might not be owned by the contact info objects, this is needed to ensure
	// that the public interface of blend_section stays the same as previous
	// versions.

	logical _pt_deriv_owned;
	pt_derivs *_sup_derivs;

	// The radius value/derivatives associated with a contact point
	// of the cross section.

	double _rad;
	double _rad_derivs[ MAX_BLEND_DERIV ];

	// These record the uv parameters on the supporting surfaces if these are
	// known (the logical records whether they are known). These should be
	// filled in where possible and may help subsequent evaluations.

	logical _uv_set;
	SPApar_pos _uv;

	// The next contact point in the list.

	blend_contact *_next;
	};

/**
 * @nodoc
 * A class in which to return data specific to a blend surface evaluation at a v
 * parameter. We return: spine, defining curve, left and right contact point
 * information. For constant radius rolling ball blends, for example, spine and
 * defining curve are the same.
 */
class DECL_KERN blend_section : public ACIS_OBJECT
	{
public:

	// Create an object that would represent an unevaluated section.

    blend_section( logical two_sup = TRUE );

	// Copy constructor

	blend_section( blend_section const &orig );

	// Constructor given a v_bl_contacts object (a "slice"). This is a
	// deprecated function. The blend section should be created directly.

	blend_section( v_bl_contacts const *vbl_slice );

	// Assignment operator for caching

	blend_section &operator=( blend_section const &rhs );

	// Remove the contact chain.

	~blend_section();

public:

	// Blend section inquiry functions other than contact point information.

	// Surface parameter of the cross-section.
	
	double v_par() const { return v_param; }
	void set_v_par( double const v_p ) { v_param = v_p; }

	// Section evaluation side for sections at blend surface 
	// v-discontinuities.

	int v_side() const { return side; }
	void set_v_side( int s ) { side = s; }

	// Defining curve information.

	int num_def_deriv() const { return def_nder; }

	// Position / derivative of the defining curve point associated with the
	// blend section.

	SPAposition const &def_pos() const;
	void set_def_pos( SPAposition const &def_p );

	// The i-th defining curve derivative where i > 0

	SPAvector const &def_deriv( int ith_der ) const;
	void set_def_deriv( int ith_der, SPAvector const &def_d );

	// Section spine point information.

	int num_spine_deriv() const { return spine_nder; }

	// Position / derivative of the spine point.

	SPAposition const &spine_pos() const;
	void set_spine_pos( SPAposition const &def_p );

	// The i-th spine derivative where i > 0

	SPAvector const &spine_deriv( int ith_der ) const;
	void set_spine_deriv( int ith_der, SPAvector const &sp_d );

	// Store the normal to the cross curve plane, which for 
	// variable radius blends may not be the same as the spine tangent. 
	// If the first spine derivative is stored, then we know that the 
	// normal has been set.

	SPAunit_vector const &section_normal() const { return xcurve_normal; }
	void set_section_normal( SPAunit_vector const &n ) { xcurve_normal = n; }

	// The blend evaluation version for this section.

	blend_eval_version const &get_eval_version() const { return _eval_version; }
	void set_eval_version( blend_eval_version const &bev ) { _eval_version = bev; }

public:

	// Inquiry functions related to the contact points of the blend surface
	// cross section.

	// Set the number of contacts of the section. We only expect two or three
	// supports of the blend.

	void set_num_contacts( logical two_sup );

	// Access to the position / derivative data of the contact point of the
	// section on the left support.

	blend_contact const *left_contact() const;
	blend_contact *left_contact();

	// Access to the position / derivative data of the contact point of the
	// section on the right support.

	blend_contact const *right_contact() const;
	blend_contact *right_contact();

	// Access to the position / derivative data of the contact point of the
	// section on the center support. This is valid only for three entity
	// blends.

	blend_contact const *center_contact() const;
	blend_contact *center_contact();

	// Set the list of contacts. Assumes ownership of the list.

	void set_contact_list( blend_contact *c_list );

public:

	// These data members are public to ensure backward compatibility, it is 
	// recommended that they are not accessed directly.

	// Blend surface v parameter at which the section was calculated.

    double v_param;	

	// Number of defining curve, spine and spring derivatives successfully
	// calculated.

	int def_nder, spine_nder, spring_nder;	

	// Evaluation side wrt v

	int side;	    

    // Now point and derivative information for the 4 points.

    pt_derivs spine, def_crv, left, right;

	// These record the uv parameters on the supporting surfaces if these are
	// known (the logical records whether they are known). These should be
	// filled in where possible and may help subsequent evaluations.

	SPApar_pos left_uv, right_uv;
	logical uv_set;

	// The normal to the cross curve plane.

	SPAunit_vector xcurve_normal;

private:

	// Blend contact information. The number of contacts would depend on the
	// number of supports of the blend.

	blend_contact *_contact_list;

	// Record the evaluation version to prevent using an inappropriate cached section.
	blend_eval_version _eval_version;

	};

/*! @} */
#endif
