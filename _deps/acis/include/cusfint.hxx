/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//                curve-surface intersection object
// Intersect a curve with a surface.  Only a bounded portion of the
// curve is of interest, given by start and end SPAparameter values,
// further delimited by a bounding SPAbox.
//
// The routine returns all intersections inside the SPAbox and none
// outside, and takes special action at the end points.  If an end
// point lies on the surface, the intersection is first coerced to
// that point, and only then checked against the SPAbox.  This way
// a vertex is treated the same for every edge running through it.
/*******************************************************************/

#ifndef CUSFINT_HXX
#define CUSFINT_HXX

#include <stddef.h>
#include "logical.h"
#include "acis.hxx"
#include "dcl_kern.h"
#include "debugmsc.hxx"
#include "param.hxx"
#include "position.hxx"
#include "curdef.hxx"

#if defined D3_STANDALONE || defined D3_DEBUG
#include "cusfint_io.hxx"
#endif

class curve;
enum evaluate_curve_side;

extern option_header csi_init_sym_rel;
/**
 * @file cusfint.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup INTRMAIN
 *
 * @{
 */

/**
 * Types of point to surface relationships.
 * @param no_end_point
 * Point and its param are not initialized to anything meaningful.
 * @param point_unknown_surf
 * Point at unknown location with respect to the surface.
 * @param point_off_surf
 * Point not on the surface.
 * @param point_on_surf
 * Point on the surface.
 */
enum point_surf_rel {
	no_end_point,
	point_unknown_surf,
	point_off_surf,
	point_on_surf
};

enum gme_point_surf_rel {
	gme_no_end_point,
	gme_point_unknown_surf,
	gme_point_off_surf,
	gme_point_on_surf
};

/**
 * Specifies the curve bounds of interest.
 * <br>
 * <b>Role:</b> This class describes the start and end of a curve, with
 * particular reference to a curve-surface intersection.
 * @see SPAparameter, SPAposition
 */

class DECL_KERN curve_bounds : public ACIS_OBJECT {

public:
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Creates curve bounds given the start and end relations, positions,
 * and parameters. If the start or end relation is <tt>TRUE</tt>, it means that the start
 * or end point, respectively, is on the surface. If the start position or end
 * position is <tt>NULL</tt>, that side of the cure is unbounded for the intersection.
 * <br><br>
 * @param s_rel
 * start relation.
 * @param s_posi
 * start position.
 * @param s_param
 * start param on curve.
 * @param e_rel
 * end relation.
 * @param e_posi
 * end position.
 * @param e_param
 * end param on curve.
 */
 	curve_bounds(
				logical s_rel, SPAposition const &s_posi, double s_param,
				logical e_rel, SPAposition const &e_posi, double e_param,
                double stol = 0.0, double etol = 0.0
			);


/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Creates curve bounds given the start and end relations, positions,
 * and parameters.
 * <br><br>
 * @param s_rel
 * start relation.
 * @param s_posi
 * start position.
 * @param s_param
 * start param on curve.
 * @param e_rel
 * end relation.
 * @param e_posi
 * end position.
 * @param e_param
 * end param on curve.
 */
    curve_bounds(
				point_surf_rel s_rel, SPAposition const &s_posi, double s_param,
				point_surf_rel e_rel, SPAposition const &e_posi, double e_param,
                double stol = 0.0, double etol = 0.0
			);


/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Creates curve bounds given the start and end positions and parameters.
 * If the start position or end position is <tt>NULL</tt>, that side of the cure is unbounded
 * for the intersection.
 * <br><br>
 * @param s_posi
 * start position.
 * @param s_para
 * start paramter.
 * @param e_posi
 * end position.
 * @param e_para
 * end paramter.
 */
 	curve_bounds(
				SPAposition const & s_posi, double s_para,
				SPAposition const & e_posi, double e_para,
                double stol = 0.0, double etol = 0.0
			);
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Creates curve bounds given the curve and the start and end positions
 * on the curve.
 * <br><br>
 * @param cur
 * curve.
 * @param s_posi
 * start position.
 * @param e_posi
 * start position.
 */
 	curve_bounds(
				curve const &cur,
				SPAposition const &s_posi = *(const class SPAposition *)NULL_REF,
				SPAposition const &e_posi = *(const class SPAposition *)NULL_REF,
                double stol = 0.0, double etol = 0.0
			);
/**
 * Writes debug information about <tt>curve_bounds</tt> to the printer or to the specified file.
 * <br><br>
 * @param fname
 * file name.
 */
 	void debug( FILE *fname = debug_file_ptr ) const;
	void gme_debug(FILE *fname = debug_file_ptr) const;
/**
 * The end parameter on the curve.
 */
 	SPAparameter end_param;
/**
 * The end position, which can be <tt>NULL</tt>.
 */
 	SPAposition end_point;
/**
 * The end relation.
 * <br><br>
 * <b>Role:</b> <tt>TRUE</tt> means that the end point is on the surface; <tt>FALSE</tt>
 * means that the end point is off the surface.
 * <br><br>
 * @see point_surf_rel.
 */
 	point_surf_rel end_rel;
/**
 * The start parameter on the curve.
 */
 	SPAparameter start_param;
/**
 * The start position, which can be <tt>NULL</tt>.
 */
 	SPAposition start_point;
/**
 * The start relation.
 * <br><br>
 * <b>Role:</b> <tt>TRUE</tt> means that the start point is on the surface;
 * <tt>FALSE</tt> means that the start point is off the surface.
 * <br><br>
 * @see point_surf_rel.
 */
 	point_surf_rel start_rel;

// STL ajr 05Apr06. Added tolerance for fix for 81495.
private:
/**
 * The tolerance of the start vertex
 */
    double start_tol;

 /**
 * The tolerance of the end vertex
 */
    double end_tol;

public:

    double get_start_tol() const; 
    double get_end_tol() const; 

};
/**
 * Provides a hook for a client intersector to attach private information to an intersection.
 * <br>
 * <b>Role:</b> This base class is purely to provide a virtual destructor to ensure
 * that anything derived by the client gets destroyed properly when the intersection
 * object is destroyed.
 */
class DECL_KERN curve_surf_userdata : public ACIS_OBJECT
{
public:
	virtual ~curve_surf_userdata();
};

/**
 * Types of relationships between curves and surfaces.
 * @param curve_unknown
 * Relationship unknown.
 * @param curve_in
 * Curve is inside surface, with a first-order approach.i.e. at the intersection
 * point the curve tangent and the surface normal have a non-zero dot product.
 * @param curve_out
 * The curve is outside the surface, with a first-order approach.
 * @param curve_in_tan
 * The curve is inside the surface, but tangent to it.
 * @param curve_out_tan
 * The curve is outside of the  surface, but tangent to it.
 * @param curve_coin
 * The curve is coincident with the surface
 * @param curve_dummy_coin
 * The curve is coincident but there are no bounds by which to indicate the fact.
 * This record only serves to show this - other fields are ignored.
 */
enum curve_surf_rel {
	curve_unknown,
	curve_in,
	curve_out,
	curve_in_tan,
	curve_out_tan,
	curve_coin,
	curve_dummy_coin
};


#pragma warning ( push )
#pragma warning( disable: 4251 )

/**
 * Represents the intersection of a curve with a surface and returns the intersections as a list.
 * <br>
 * <b>Role:</b> This class represents the details of a curve-surface intersection.
 * This class returns all intersections inside the bounding box in a list.
 * This list can be walked using the "next" data member of the class.
 * <br><br>
 * If an end point lies on the surface, the intersection is first coerced
 * to that point and then checked against the bounding box. In this way, a
 * vertex is treated the same for every edge running through it.
 * @see SPApar_pos, SPAparameter, SPAposition
 */
class DECL_KERN curve_surf_int : public ACIS_OBJECT
{
public:
		
/**
 * @nodoc
 * INTERNAL USE ONLY: Relationship of the CSI with the object boundaries.
*/
	// NOT BOUNDARY :: the intersection point doesn't lie either on curve boundary 
    //                 or surface boundary
	// ON_CRV_START :: Intersection lies on curve start point
	// ON_CRV_END :: Intersection lies on curve end point
	// ON_SF_BNDRY :: Intersection lies on surface boundary
	// ON_CU_START_AND_SF_BNDRY :: Intersection lies at curve start as well as on 
    //                 surface boundary
	// ON_CU_END_AND_SF_BNDRY :: Intersection lies at curve end as well as on surface 
    //                 boundary

	enum boundary_rel_type {
		NOT_BOUNDARY = 0,
		AT_CRV_START = 1,
		AT_CRV_END = 2,
		ON_SF_BNDRY = 4,
		ON_CRV_START_AND_SF_BNDRY = 5,
		ON_CRV_END_AND_SF_BNDRY = 6
	};

	enum gme_boundary_rel_type {
		gme_NOT_BOUNDARY = 0,
		gme_AT_CRV_START = 1,
		gme_AT_CRV_END = 2,
		gme_ON_SF_BNDRY = 4,
		gme_ON_CRV_START_AND_SF_BNDRY = 5,
		gme_ON_CRV_END_AND_SF_BNDRY = 6
	};

/**
 * @nodoc
 * INTERNAL USE ONLY: A wrapper class over the above enum.
*/

	class boundary_rel : public ACIS_OBJECT
	{
		boundary_rel_type _rel;

	public:

        // These are needed for automatic conversion to the enum.
		
		boundary_rel( boundary_rel_type r = NOT_BOUNDARY )
			: _rel( r )
		{}

		boundary_rel& operator=( boundary_rel_type r )
		{
			_rel = r;
			return *this;
		}

		operator boundary_rel_type() const { return _rel; }

        // Create a composite type when the csi lies on multiple 
		// object boundaries.

		boundary_rel& operator|=( boundary_rel const& rhs )
		{
			int this_rel = static_cast< int >( _rel );
			int rhs_rel = static_cast< int >( rhs._rel );
			_rel = static_cast< boundary_rel_type >( this_rel + rhs_rel );
			return *this;
		}
	};

public:
#ifdef INTERNAL_DEBUG_CHECKS
	int _csi_index;
	#ifdef THREAD_SAFE_ACIS
		static safe_integral_type<int> _cnt;
	#else
		static int _cnt;
	#endif
	
	// BRI:QMV
	enum algo_type{ ALGO_UNSET, ALGO_SPECIAL, 
			ALGO_GENERAL, ALGO_SP_GENERAL };
	algo_type _algo_used;

#endif //INTERNAL_DEBUG_CHECKS

/**
 * C++ initialize constructor requests memory for this object and populates it with the  data supplied as arguments.
 * <br><br>
 * @param pt
 * intersection point.
 * @param next_int
 * next intersection.
 * @param para
 * param at intersection.
 * @param tol
 * intersection tolerance
 * @param before_para
 * relationship before param.
 * @param after_para
 * relationship after param.
 */
 	curve_surf_int(
				SPAposition const &pt,
				curve_surf_int *next_int,
				double para,
				double const tol = SPAresabs,
				curve_surf_rel before_para = curve_unknown,
				curve_surf_rel after_para = curve_unknown
			);
	curve_surf_int(
				const char *gme,
				SPAposition const &pt,
				curve_surf_int *next_int,
				double para,
				double const tol = SPAresabs,
				curve_surf_rel before_para = curve_unknown,
				curve_surf_rel after_para = curve_unknown
			);

/**
  * @nodoc
  * <b>Obsolete:</b> use @href curve_surf_int with tolerance instead.
  * C++ initialize constructor requests memory for this object and populates it with the  data supplied as arguments.
  * <br><br>
  */
 	curve_surf_int(
				curve_surf_int *next_int,
				SPAposition const &pt,
				double para,
				curve_surf_rel before_para = curve_unknown,
				curve_surf_rel after_para = curve_unknown
			);
	curve_surf_int(
				const char *gme,
				curve_surf_int *next_int,
				SPAposition const &pt,
				double para,
				curve_surf_rel before_para = curve_unknown,
				curve_surf_rel after_para = curve_unknown
			);

/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param intsec
 * curve-surf intersection.
 */
 	curve_surf_int( curve_surf_int const &intsec );
	curve_surf_int( const char *gme, curve_surf_int const &intsec );
/**
 * C++ destructor, deleting a <tt>curve_surf_int</tt>.
 */
 	~curve_surf_int();
#ifndef NO_MESH_CLASSES
/**
 * General purpose pointer used to store the element on a <tt>meshsurf</tt> from which this intersection originated.
 */
	const void *data1;
/**
 * General purpose pointer used to store the element on a <tt>compcurv</tt> from which this intersection originated.
 */
 	const void *data2;
#endif
/**
 * Writes debug information about <tt>curve_surf_int</tt> to the printer or to the specified file.
 * <br><br>
 * @param fname
 * file name
 */
 	void debug( FILE *fname = debug_file_ptr ) const;

	void gme_debug( FILE *fname = debug_file_ptr ) const;
/**
 * @nodoc
 * INTERNAL USE ONLY: This is <tt>TRUE</tt> if the intersection is not tightly defined (a tangency or small-angle crossing).
 */
 	logical fuzzy;
/**
 * @nodoc
 * INTERNAL USE ONLY: The high end of the parameter range if it is fuzzy; the same as param if it is not fuzzy.
 */
 	SPAparameter high_param;
/**
 * The point of intersection.
 */
 	SPAposition int_point;
/**
 * Relationship between curve and surface in the positive direction.<br><br>
 * <b>Role:</b> The relationship between the curve and the surface in the neighborhood
 * of the intersection, in the positive parameter direction.
 * <br><br>
 * @see curve_surf_rel.
 */
 	curve_surf_rel high_rel;
/**
 * @nodoc
 * INTERNAL USE ONLY: The low end of the parameter range if it is fuzzy; the same as param if it is not fuzzy.
 */
 	SPAparameter low_param;
/**
 * Returns relationship between curve and surface.<br><br>
 * <b>Role:</b> The relationship between the curve and the surface in the neighborhood
 * of the intersection, in the negative parameter direction.
 * <br><br>
 * @see curve_surf_rel.
 */
 	curve_surf_rel low_rel;
/**
 * The pointer to the list.
 */
 	curve_surf_int* next;
/**
 * The parameters of the intersection point on the curve.
 */
 	SPAparameter param;
/**
 * The parameters of the intersection point on the surface.
 */
 	SPApar_pos surf_param;
/**
 * Supports tolerant modeling.
 * <br><br>
 * <b>Role:</b> The value is used to record the tolerance value
 * of the intersection. It is defaulted to <tt>SPAresabs</tt>.
 */
 	double tolerance;
/**
 * Pointer to an arbitrary object to store user data.
 * <br><br>
 * <b>Role:</b> If non-<tt>NULL</tt>, it is deleted when this object is deleted. It is the
 * responsibility of the user's class derived from this to ensure that the destructor
 * does what is necessary.
 */
 	curve_surf_userdata *userdata;

/**
 * @nodoc
 * INTERNAL USE ONLY: This relation tells whether the intersection point lies on curve / 
 * surface boundary or not.
 */	
	boundary_rel _boundary_rel;

/**
 * @nodoc
 * INTERNAL USE ONLY: Method to set low_rel.
 */	
	void set_low_rel(curve_surf_rel rel = curve_unknown)
	{
		low_rel = rel;
	}
	void gme_set_low_rel(curve_surf_rel rel = curve_unknown){
		low_rel = rel;
	}
	
/**
 * @nodoc
 * INTERNAL USE ONLY: Method to set high_rel.
 */	
	void set_high_rel(curve_surf_rel rel = curve_unknown)
	{
		high_rel = rel;
	}
	void gme_set_high_rel(curve_surf_rel rel = curve_unknown){
		high_rel = rel;
	}
/**
 * @nodoc
 * INTERNAL USE ONLY: Method to query low_rel.
 */	
	curve_surf_rel get_low_rel() const
	{
		return low_rel; 
	}
	curve_surf_rel gme_get_low_rel() const
	{
		return low_rel; 
	}

/**
 * @nodoc
 * INTERNAL USE ONLY: Method to query high_rel.
 */	
	curve_surf_rel get_high_rel() const
	{
		return high_rel;
	}
	curve_surf_rel gme_get_high_rel() const
	{
		return high_rel;
	}

/**
 * @nodoc
 * INTERNAL USE ONLY: Whether the intersection point corresponds to start or end of coincidence
 * region which is inside or outside the region of interest for given side.
 */	
	bool is_coin_or_dummy_coin(evaluate_curve_side side= evaluate_curve_unknown ) const;
/**
 * @nodoc
 * INTERNAL USE ONLY: Whether the intersection point corresponds to an end of coincidence
 * region which is inside the region of interest.
 */	
	bool is_coin(evaluate_curve_side side = evaluate_curve_unknown) const;

/**
 * @nodoc
 * INTERNAL USE ONLY: Whether the intersection point corresponds to an end of coincidence
 * region which is outside the region of interest.
 */	
	bool is_dummy_coin(evaluate_curve_side side = evaluate_curve_unknown) const;

/**
 * @nodoc
 * INTERNAL USE ONLY: Whether the region spanned by this intersection and the last
 * intersection is coincident but outside the region of interest.
 */	
	bool is_dummy_coin_region( curve_surf_int const * last_int ) const;

/**
 * @nodoc
 * INTERNAL USE ONLY: Whether the region spanned by this intersection and the last
 * intersection is coincident but outside the region of interest.
 */	
	bool is_coin_region( curve_surf_int const * next_int ) const;
/**
 * @nodoc
 * INTERNAL USE ONLY: temporary function.
 */	
	static curve_surf_rel coin_other_rel( logical dummy = FALSE) 
	{
		return dummy ?  curve_dummy_coin : csi_init_sym_rel.on() ?  curve_unknown :  curve_coin;
	}
/**
 * @nodoc
 */
// Internal function to initialize data fields


private:

	void initialize(
					SPAposition const &int_point_val,
					curve_surf_int *next_val,
					double SPAparameter,
					double const epsilon,
					curve_surf_rel low_rel_val,
					curve_surf_rel high_rel_val
			);
};

#pragma warning ( pop )

/**
 * Deletes a linked list of <tt>curve_surf_ints</tt>, given the head of the list.
 * <br><br>
 * <b>Role:</b> The head is always set to zero on exit.
 * <br><br>
 * @param head
 * head.
 */
DECL_KERN	void	delete_curve_surf_ints( curve_surf_int*& head );
/** @} */

#ifdef INTERNAL_DEBUG_CHECKS
DECL_KERN const char* algo_type_str( curve_surf_int::algo_type type);
#endif // INTERNAL_DEBUG_CHECKS

#endif
