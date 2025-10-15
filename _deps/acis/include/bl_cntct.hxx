/* ORIGINAL: acis2.1/sg_husk/vrbln/bl_cntct.hxx */
// $Id: bl_cntct.hxx,v 1.27 2002/08/09 17:15:38 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


#include "acis.hxx"
#include "dcl_kern.h"

// Declaration of the v_bl_contacts class.
//
//	This represents the surface contact information where a
//	general blend surface contacts its defining surfaces.
//	It contains all of the information (derivatives, etc.)
//	of the defining surfaces at a particular v-SPAparameter ("slice").
//	Most of this is stored in the blend_support objects.
//	It also contains the v-SPAparameter value, point and tangent on
//	the defining curve (which define the slicing plane), and
//	the offset-intersection point (spine point) and derivatives.
//	In addition, information about the v-derivatives of the contact
//	points can be stored, along with a count of the number of derivatives
//	that have currently been evaluated.  If this->num_derivs is less
//	than zero, then nothing is set.
//	In general, if this->num_derivs is set to n, then the object
//	is guaranteed to have proper information for up to n derivatives.
//	This applies to: left_dv, right_dv, etc; left_norm_v, right_norm_v, etc;
//	the offset point and derivatives; the radii; and the blend_supports.
//
//	Related to this is this->num_env_derivs, which indicates how many
//	derivatives have been adjusted for the envelope surface.
//	This counter enables callers to invoke the slice_derivs() methods
//	of the surfaces multiple times.
//
//	This object has a "next" pointer to allow linked lists.
//	
//	This is meant to be a transitory object, and is not saved or restored.
//

// Modification---
//
//  03-Jul-07 pvc   added methods for initialization of a set of derivatives,
//                  added storage/retrieval of derivatives of section 
//                  curvature at contacts, section tangent at contacts
//	07-Sep-05 hcd	Made methods for two ways of checking slice singularities.
//	08-Oct-04 syh	Changes for handling discontinuity during evaluation.(76422)
//  09-Jan-04 puc   Added function to adjust for periodic wrap around.
//	24-Oct-02 bwc	Added member variables to handle 4th derivatives.
//	01-Jul-99 bd	Comments only, concerning operator=() and copy c'tor.
//	06-Mar-99 bd	New members crv_der2, crv_der3, crv_der4.
//	22-Dec-98 bd	Add member num_env_derivs to v_bl_contacts class.
//	01-Oct-98 bd	Pass const-ref to in_plane().
//  14 Aug 97 nvp	Add remove_all() to delete all the members in the list
//	02-Aug-96 bd	Remove blend_supports, because they copy surfaces and
//		BOUNDED_SURFACES, etc, upon every creation; replace them with local
//		SVECs, CVECs, bs2_curves;
//		new constructors; remove extended_left/right_srf;
//	26-Mar-96 bd	Add left_pt() and right_pt();
//	21-Feb-96 bd	Remove two c'tors, that took segend and off_sfsf_pos
//		(for micro-kernel project).
//	07-Feb-96 bd	Use a var_blend_spl_sur instead of a var_rad_spl_sur.
//	30-Oct-95 bd	Make arg to assignment operator const. (Compiler warning)
//	17-Oct-95 bd	Add logical 'brief' arg to debug(); also cosmetic.
//

//
// Implementation notes:
//
// About the off_pt member and its derivatives:
//	These are calculated and stored by the spring curve evaluator
//	(eval_springs(), set_save_slice()).  They are used in the
//	get_section() methods to determine the plane of the cross curve.
//
// About "ownership":  We try not to own anything substantial, because
//	these are transient objects that get created and deleted on
//	every evaluation.
//


#if !defined V_BL_CONTACTS_DEF
#define V_BL_CONTACTS_DEF


#include <stdio.h>	// kernutil/debug/debugmsc.hxx needs this...

#include "bs2curve.hxx"

#include "param.hxx"
#include "position.hxx"
#include "vector.hxx"
#include "unitvec.hxx"
#include "curdef.hxx"
#include "bs3curve.hxx"

class SVEC;
class CVEC;

class blend_spl_sur;
class SizeAccumulator;
class v_bl_contacts;
class var_blend_spl_sur;

// A contact point of the blend, which stores the SVEC or CVEC based
// on whether the support is a surface or curve. Also the surface/curve
// derivatives w.r.t the defining curve parameter. These are setup by
// the evaluator and later used by u-cross section creation.

struct v_bl_contact_pt : public ACIS_OBJECT
{
    v_bl_contact_pt( SVEC const *sv );

    // These are not trivial as SVEC / CVEC objects are on the heap and
    // owned by this class.

    v_bl_contact_pt( v_bl_contact_pt const &orig );
    v_bl_contact_pt &operator=( v_bl_contact_pt const &rhs );
    ~v_bl_contact_pt();

    // v-derivatives of the blend surface at the contact points.

	SPAvector dv, dvv, dvvv, dvvvv;

    // Derivatives of the surface normals w.r.t. the v parameter.
    // These get calculated when the contact pts and derivs are found 
    // (off_int), and are needed to get the cross section.

	SPAvector norm_v, norm_vv, norm_vvv, norm_vvvv;

    SVEC *svec;
    CVEC *cvec;
    bs2_curve bs2;
};

class DECL_KERN v_bl_contacts : public ACIS_OBJECT 
{
    // allow private methods of v_bl_contacts to be called from methods of var_blend_spl_sur
    friend class var_blend_spl_sur;
    
public:

	// Handle discontinuity in blend surface due to supports. 

	evaluate_curve_side _v_side;

	double param;
	SPAposition	crv_point;		// could be called "plane_point".
	SPAvector crv_tangent;	    // could be called "plane_normal".

	SPAposition	off_pt;			// "spine" point.

	SVEC *left_svec, *right_svec;
	CVEC *left_cvec, *right_cvec;
	bs2_curve left_bs2,	right_bs2;

    // derivatives of the defining curves
    // These are w.r.t. its own SPAparameter, not our v.

	SPAvector crv_der2, crv_der3, crv_der4, crv_der5;

    // Stored radius values. Enough for value and four derivatives.

	double left_rad[ 5 ];
	double right_rad[ 5 ];

    // v-derivatives of the blend surface at the contact points

	SPAvector left_dv, right_dv;
	SPAvector left_dvv, right_dvv;
	SPAvector left_dvvv, right_dvvv;
	SPAvector left_dvvvv, right_dvvvv;

    // Derivatives of the surface normals w.r.t. the v SPAparameter.
    // These get calculated when the contact pts and derivs are found 
    // (off_int), and are needed to get the cross section.

	SPAvector left_norm_v, right_norm_v;
	SPAvector left_norm_vv, right_norm_vv;
	SPAvector left_norm_vvv, right_norm_vvv;
	SPAvector left_norm_vvvv, right_norm_vvvv;

    // Derivatives of the offset (spine) point w.r.t. the v SPAparameter.
    // These get calculated when the contact pts and derivs are found
    // (eval_springs(), set_save_slice()), and are needed to get the cross 
    // section.

	SPAvector off_pt_v, off_pt_vv, off_pt_vvv, off_pt_vvvv;

    // Allow lazy evaluation.

	int num_derivs;		    // which of left_dv, etc, are valid.
	int	num_env_derivs;	    // which have been adjusted for envelope surface.
	logical param_set;
	logical plane_set;		// covers crv_point and crv_tangent.
	logical left_contact_set;
	logical right_contact_set;

    // Allow lists of these.
    // Note: this pointer is set to null in the copy constructor,
    // and is simply copied directly in the assignment operator.

	v_bl_contacts *next;

    // Constructors:

    // Null constructor.

	v_bl_contacts();

    // Main constructor, given a blend spl_sur:

 	v_bl_contacts( blend_spl_sur *blend_srf );

    // Copy constructor.
    // Note: this sets the 'next' pointer to null in the new copy.

	v_bl_contacts( v_bl_contacts* );

    // Destructors:

	~v_bl_contacts();		// Deletes a single object -- but does nothing
							// with the 'next' pointer.

    void reset();   // resets local variables of this to ensure no
                    // propagation of bad data

    // Assignment operator.	This creates new SVECs from the old ones.
    // Note: this does a simple copy of the 'next' pointer.

    v_bl_contacts &operator=( v_bl_contacts const & );

	void delete_chain();	// Deletes all objects chained from the given
							// object. Doesn't delete the given object.

    // Member retrieval:

    // For convenience: fishing out the contact points can be a bother.

 	SPAposition	left_pt() const;
	SPAposition	right_pt() const;
	SPApar_pos left_uv() const;
	SPApar_pos right_uv() const;
	SPAunit_vector left_norm() const;
	SPAunit_vector right_norm() const;

    // Access to the center contact point of three entity blends.

    v_bl_contact_pt const *center_contact() const { return _center_contact; }
    v_bl_contact_pt *center_contact() { return _center_contact; }

    // Note: member setting methods could deal with the set_* member flags, 
    // but it proves to be more convenient to let the caller do that 
    // explicitly.
    // The caller knows better what exactly is set and what isn't.

    // Transform this slice by the given SPAtransf.

	void operator*=( SPAtransf const &xfm );

	logical equals( 
		        double v_param, 
		        evaluate_curve_side side = evaluate_curve_unknown, 
		        double tolerance = SPAresmch
                ) const;

    // Access to the data based on support index and order of the 
    // derivatives. The supports are ordered, left, right and finally
    // center for these functions. The center support should be queried
    // only for three entity blends.

    SVEC *svec( int sup_index ) const;
    SPAvector const &spr_pos_dv( int sup_index, int order = 1 ) const;
    SPAvector const &spr_norm_dv( int sup_index, int order = 1 ) const;

    // other:

	logical in_plane( SPAposition const &, double tol = SPAresabs ) const;

	void debug( char const *leader, logical brief, FILE *fp ) const;

	void full_size( SizeAccumulator &est, logical countSelf = TRUE );

	void minimize( minimize_helper * );

	logical surface_singularity();

	logical slice_singularity();

   // note: surface_normal_dv(ind, 0) is actually a unit_vector
    SPAvector  const &surface_normal_dv(int sup_index, int order)    const;

    // note: section_tangent_dv(ind, 0) is actually a unit vector
    SPAvector  const &section_tangent_dv(int sup_index, int order)   const;

    double            section_curvature_dv(int sup_index, int order) const;
private:

    // The center contact point of three entity blends.

    v_bl_contact_pt *_center_contact;

    // ensure that "this" has the necessary derivative computations of
    // the surface normal along the spring curve
    void initialize_surface_normal_dvs(int n_derivs, logical is_convex);

    // ensure that "this" has the necessary derivative computations
    // for the cross sectional tangent along the spring curve.
    // NOTE:
    // initialize_surface_normal_dvs must be called before attempting
    // to call initialize_section_tangent_dvs
    void initialize_section_tangent_dvs(int n_derivs);

    // ensure that "this" has the necessary derivative computations of
    // the cross sectional curvature along the spring curve 
    // NOTE:
    // initialize_section_tangent_dvs must be called before attempting
    // to call intialize_section_curvature_dvs
    void initialize_section_curvature_dvs(
									SPAinterval const &v_range,
                                    int n_derivs,
                                    v_bl_contacts *contact_vphd2 = NULL,
                                    v_bl_contacts *contact_vph   = NULL,
                                    v_bl_contacts *contact_vmhd2 = NULL,
                                    v_bl_contacts *contact_vmh   = NULL
                                    );

    void initialize_section_curvature_dvs(
                                   int       n_derivs,
                                   bs3_curve curvature_curve,
                                   logical   compare_data = FALSE
                                   );
    
 
    // need pointer to the blend surface so that temporary additional cross
    // sections for approximate derivatives of curvature can be
    // constructed on the fly...
    int        m_nsn_derivs_computed,
               m_nst_derivs_computed,
               m_nsc_derivs_computed;
    SPAvector  m_surface_normal_dvs[3][5];
    SPAvector  m_section_tangent_dvs[3][5];        
    double     m_section_curvature_dvs[3][5];

    // temporary for debugging of tangent derivatives
    SPAvector  m_section_P_dvs[5];
    double     m_section_alpha_dvs[5];
    double     m_section_beta_dvs [5];
};

//
// Obsolete constructors and methods for v_bl_contacts:
//
// Construct a mostly empty v_bl_contacts, given surfaces for the supports.
//	v_bl_contacts( surface *left, surface *right);
//
// Construct a mostly empty v_bl_contacts, given two blend_support's.
//	Problem: creates new BOUNDED_SURFACES, makes copies of surfaces, etc.
//	v_bl_contacts( blend_support *left, blend_support *right);
//
// Construct a mostly empty v_bl_contacts, given FACEs and curves.
//	v_bl_contacts(	FACE *left_srf, curve *left_crv,
// 					FACE *right, curve *right_crv);
//
// These are unused and have been removed.
//	// Construct a slice from a segend.
//		v_bl_contacts( segend, blend_spl_sur *, int );	// uses the surface...
//	// Construct a slice from an off_sfsf_pos.
//		v_bl_contacts( off_sfsf_pos );
//
// Construct a slice from a point_data object.
//	No: Not enough info in a point_data; need surfaces.
//	The only place this might be required is in bl_edge_::true_point(),
//	which is not implemented.
//		v_bl_contacts( point_data pt, logical is_left );
//
//	These are (were) here to help with backward compatibility.
//	SVEC &	left_contact()  const	{ return *left_svec; }
//	SVEC &	right_contact() const	{ return *right_svec; }
//


//			-------------
//
//  Linked list of v_bl_contacts.
//  The list is sorted on param.
//  The ordering may be increasing or decreasing ( this is reversable ),
//  with no duplicates.
//
//	This is just bookkeeping, i.e. it doesn't do
//	technical things such as finding slices, etc.
//
//	Note: one thing that makes this not just a generic list is the
//	special elements start_slice and end_slice.  They may be used
//	to identify special slices that may or may not exist in a list,
//	such as vertex-blend mates.
//	These slices are distinct: there can be a start but no end,
//	several slices with no start or end, etc.
//	Also, there can be v_bl_contacts before start_slice and after end_slice.
//	list must always point to the very beginning of the list, however
//	(i.e. start_slice can't be before list).
//
// Methods to set_start() and set_end().
//	These are the only ways to set start_slice and end_slice.
//	These automatically add the input slices to the list.
//	So there can be a list with no start and/or end,
//	but not a start or end without a list.
//

enum slice_rel { BEFORE, EQUAL, AFTER, UNKNOWN };


class DECL_KERN list_of_contacts : public ACIS_OBJECT 
    {

private:

	int count;
	v_bl_contacts *list;
	v_bl_contacts *start_slice;
	v_bl_contacts *end_slice;
	logical	is_increasing;
	double	tol;

    // some private member functions:

    slice_rel relation( v_bl_contacts *s1, v_bl_contacts *s2 );
    logical	before( v_bl_contacts *s1, v_bl_contacts *s2 );
    logical	after( v_bl_contacts *s1, v_bl_contacts *s2 );
    logical	equal( v_bl_contacts *s1, v_bl_contacts *s2 );
    void insert_after ( v_bl_contacts *s1, v_bl_contacts *s2 );
    v_bl_contacts *find_match( v_bl_contacts *given_slice );

public:

    // constructors:

	list_of_contacts( double par_tol = SPAresabs );
	list_of_contacts( v_bl_contacts * );
	list_of_contacts( const list_of_contacts & );	// copy constructor.

    // A destructor is required:

	~list_of_contacts();

    // Assignment operator.

    list_of_contacts &operator=( list_of_contacts const & );

    // utilities:

    // Member access

	int n_contacts() const { return count; }
	v_bl_contacts *first_slice() const	{ return start_slice; }
	v_bl_contacts *last_slice()	const { return end_slice; }
	v_bl_contacts *beginning() const { return list; }
	v_bl_contacts *end() const;
	v_bl_contacts*locate( double param, double tol ) const;
	logical increasing() const { return is_increasing; }

    // We don't know this technical stuff here:

	void set_tol( double new_tol ) { tol = new_tol; }
    double get_tol( void ) const { return tol; }

    // Returns a pointer to the deleted slice, if any

	v_bl_contacts *add( v_bl_contacts * );
	void remove( v_bl_contacts *, logical lose = TRUE );
	void set_first( v_bl_contacts* );
	void set_last( v_bl_contacts* );
	void replace( v_bl_contacts *old_slice, v_bl_contacts *new_slice );
//	void make_increasing();	... no: just say:
//	void make_decreasing();	...		if (!list.increasing()) list.reverse();
	void reverse();

	void remove_all();

	logical adjust_for_periodicity( SPAinterval const &p_range );

    // somebody is apparently still calling this:

	void debug( char const *leader, FILE *fp ) const;
	void debug( char const *leader, logical brief, FILE *fp ) const;

	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
    };

#endif
