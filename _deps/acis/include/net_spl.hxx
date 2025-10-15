/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// $Id:
//
// This surface class defines a net surface between a list of curves.
//
//	The surface parametrization is: u-direction and v-direction corresponds to 
//  the U and V curves to be surfaced.
//
//	The input to this surface class is :
//
//	The curves to be surfaced.  All the curves are reparametrized to lie
//	in ( 0.0 - 1.0 ) range.
//
//	The evaluation process:
//
//	STEP 1:
//      Loft the V curves in the U direction.
//
//	STEP 2:
//      Loft the U curves in the V direction.
//
//	STEP 3:
//      Create a Tensor product patch from the corner intersection
//      of the mesh of curves.
//
//  STEP 4:
//		Sum the Loft surfaces and subtract off the Tensor product surface.
//

#if !defined( NET_SPL_SUR_CLASS )
#define NET_SPL_SUR_CLASS			// STIPORT pes _MSC_VER does not like trailing ;

#include "dcl_kern.h"
#include "logical.h"
#include "vector.hxx"
#include "position.hxx"
#include "debugmsc.hxx"
#include "allcudef.hxx"
#include "bs3ccont.hxx"
#include "vlists.hxx"

class curve;
class intcurve;
class SPApar_pos;
class SPAposition;
class SPAvector;
class SPAtransf;

class SPAbox;
class SPApar_box;
class law;
class r3_law_cache;

// STI ROLL
class SizeAccumulator;
// STI ROLL

class bernstein_basis;
class netspl_corner;

#include "spldef.hxx"
#include "lists.hxx"

// changed to SESSION_LOCAL VAR  -     extern DECL_KERN int net_spl_sur_type;

class DECL_KERN net_spl_sur: public spl_sur {
// Allow extensions to declare themselves as friends. USE WITH EXTREME CAUTION
#ifdef net_spl_sur_FRIENDS
net_spl_sur_FRIENDS
#endif

protected:

	// Declare the function make_net_approx as a friend of the net_spl_sur
	// class, as this will enable this function to use the members of this
	// class freely.
	friend DECL_KERN bs3_surface make_net_approx(
							net_spl_sur const &,
							SPAinterval const &,									// u SPAparameter range
							SPAinterval const &,									// v SPAparameter range
							double &, 												// tol returned
							logical self_int_test,							// logical
							VOID_LIST& bad_uvs,				// bad_uvs  
							SPApar_box& exclude_region,	// exclude region
							SPApar_box& extension_box);	// extension_box

    friend DECL_KERN void get_preferred_knots(net_spl_sur const &this_surf,
		                                     SPAinterval	const	&range_u,
		                                     SPAinterval	const	&range_v,
		                                     int& number_of_u,
                                             double*& u_cuts,
                                             int& number_of_v,
                                             double*& v_cuts);

	friend DECL_KERN bs3_surface make_net_approx_hermite(net_spl_sur	const&, SPAinterval	const&,
		                                       SPAinterval	const&, double &tol, double knot_tol, double& max_dist);

	friend DECL_KERN double  surface_error( 
				net_spl_sur const& origin_surf,
				bs3_surface& approx_surf,
				int nu,
				double *u_knots,
				int nv,
				double *v_knots,
				double&,
				double&);

	// Make or remake the approximating surface.  This function, which is a 
    // virtual function of spl_sur, is implemented using make_net_approx. 
	// The function stores the approximating surface and the actual fit error 
    // that was achieved in the spl_sur, overriding the declared const-ness 
    // of the function to do this. 

    virtual void	make_approx( 
							 double fit, 
							 const spline& spl = *(spline*) NULL_REF, 
						     logical force = FALSE
							 )const; 

//GSSL VPL Incremental approx project. change return type to logical.
	virtual logical incremental_make_approx(double fit,
										 SPAinterval u_range,
										 SPAinterval v_range,
										 logical self_int_test = FALSE,
							             VOID_LIST& bad_uvs = *(VOID_LIST*)NULL_REF,			// bad_uvs  
							             SPApar_box& exclude_region = *(SPApar_box*) NULL_REF,	// exclude region
							             SPApar_box& extension_box = *(SPApar_box*) NULL_REF);	// extension_box);

	void compute_cubic_blend_vectors (evaluate_curve_side curve_side,
									 logical first_required, 
									 logical second_required, 
									 logical third_required,
									 logical direction,
									 int v_index, int u_index,
									 curve *v_cur0, curve *v_cur1,
									 curve *u_cur0, curve *u_cur1,
									 double SPAparameter, double t_parameter, double *v_knots, double *u_knots,
									 bernstein_basis &blend_data, 
									 SPAvector &n0, SPAvector &n1,
									 SPAvector &n0d, SPAvector &n1d,
									 SPAvector &n0dd, SPAvector &n1dd,
									 SPAvector &n0ddd, SPAvector &n1ddd ) const;

	// Calculate and add the dicontinuities from the generating curves
	// to the net surface
	void curve_add_discontinuities();
    void add_curve_irregularities_to_disc_info(logical u_param, int curve_number, curve *crv, curve_irregularities*& cirr, discontinuity_info *input_disc_info);

	// Number of curves to be surfaced and some mapping information.
	int no_crv_u;
	int no_crv_v;

	// Corner data
	netspl_corner **corner_data;

	// Array of curves to be surfaced saved in an entity_list
	ENTITY_LIST u_curves;
	ENTITY_LIST v_curves;

	// Array of doubles indicating the knot values at each section that is
	// being interpolated.  Also save the u-range.
	double *u_knots;
	double *v_knots;

	enum PARAM_FLAG
	{
		PARAM_UNSET = 0,
		RECALCULATE_TWIST = 1,
		RECALCULATE_DS_DT = 2,
		USE_NEW_PERIODIC_ALGO = 4
	};

	int flag_num;

	// Default constructor, used in to create a empty class, so that
	// the copy function can use it.
	net_spl_sur();

    // Calculate the discontinuity information from the defining curves. 

    virtual	void 	calculate_disc_info();

	// boundary SPAvector field (used in ACIS7.0+)
	law			 *bnd_laws[4];
	r3_law_cache *bnd_cache[4];

public:

	// Construct net surface from the given section curves
	net_spl_sur(
			int,			  // Number of V curves in the array
			int,              // Number of U curves in the array
			curve **,		  // Array[no] of v curves
			curve **,		  // Array[no] of u curves
			double *,		  // Array[no] of v knots values ( increasing order ).
			double *,		  // Array[no] of u knots values ( increasing order ).
			netspl_corner **,        // Array[no][no] of corners. ( col major )
			law **in_law=NULL // boundary SPAvector field (used in ACIS7.0+)
		);


   // Copy constructor.
   net_spl_sur( const net_spl_sur& );

   // Make a copy without sharing subdata.

   virtual spl_sur *deep_copy(pointer_map * pm = NULL) const;

   int accurate_derivs(
               SPApar_box const & = *(SPApar_box *)NULL_REF
                                   // Defaults to the whole surface
           ) const;

   // STI ROLL
   virtual void full_size(SizeAccumulator&, logical = TRUE) const;
   // STI ROLL
/**
 * @nodoc
 */
	virtual void minimize( minimize_helper*);
   

protected:
	// Destructor needs to delete the net curve.

	virtual ~net_spl_sur();

	// Construct a duplicate in free store of this object but with
	// zero use count.

	virtual subtrans_object *copy() const;


	// Test for equality. This does not guarantee that all
	// effectively equal surfaces are determined to be equal, but
	// does guarantee that different surfaces are correctly
	// identified as such.

	logical operator==( subtype_object const & ) const;


	// Transform this net by the given SPAtransf.

	virtual void operator*=( SPAtransf const &	);


    // Parameter shift: adjust the spline surface to have a SPAparameter
    // range increased by the argument value (which may be negative).
    // This is only used to move portions of a periodic surface by
    // integral multiples of the period.

    virtual void shift_u( double );
    virtual void shift_v( double );

	// Divide a surface into two pieces at a given SPAparameter value.
	// If the split is at the end of the SPAparameter range, the spl_sur
	// is just returned as the appropriate half (in increasing
	// SPAparameter order), and the other is NULL. Otherwise a new spl_sur
	// is used for one part, and the old one is modified for the other.

	virtual void split_u(
				double,
				spl_sur *[ 2 ]
			);
	virtual void split_v(
				double,
				spl_sur *[ 2 ]
			);


	// Concatenate the contents of two surfaces into one. The surfaces
	// are guaranteed to be the same base or derived type, and to have
	// contiguous SPAparameter ranges ("this" is the beginning part of
	// the combined surface (i.e. lower SPAparameter values), the
	// argument gives the end part).

	// virtual void append_u( spl_sur & );
	// virtual void append_v( spl_sur & );


	// Geometric evaluation.

	// Find the SPAparameter values of a point on the surface
	
	virtual SPApar_pos param(
				 SPAposition const &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF
			) const;

	// Find the SPAposition and first and second  derivatives of the
	// surface at a given point.

	virtual void eval(
				SPApar_pos const &uv,
				SPAposition &pos,
				SPAvector *dpos,	// first derivatives - array of
								// length 2 in order xu, xv
				SPAvector *ddpos	// second derivatives - array of
								// length 3 in order xuu, xuv, xvv
			) const;

   // The evaluate() function calculates derivatives, of any order
   // up to the number requested, and stores them in vectors provided
   // by the user. It returns the number it was able to calculate;
   // this will be equal to the number requested in all but the most
   // exceptional circumstances. A certain number will be evaluated
   // directly and (more or less) accurately; higher derivatives will
   // be automatically calculated by finite differencing; the accuracy
   // of these decreases with the order of the derivative, as the
   // cost increases.

   virtual int evaluate(
               SPApar_pos const &,    // Parameter
               SPAposition &,         // Point on surface at given SPAparameter
               SPAvector ** = NULL,   // Array of pointers to arrays of
                                   // vectors, of size nd. Any of the
                                   // pointers may be null, in which
                                   // case the corresponding derivatives
                                   // will not be returned. Otherwise
                                   // they must point to arrays long
                                   // enough for all the derivatives of
                                   // that order - i.e. 2 for the first
                                   // derivatives, 3 for the second, etc.
               int = 0,            // Number of derivatives required (nd)
               evaluate_surface_quadrant = evaluate_surface_unknown
                                   // the evaluation location - above,
                                   // below for each SPAparameter direction,
                                   // or don't care.
           ) const;

	void eval_net(
				SPApar_pos const &uv,
				SPAposition &pos,
				SPAvector *dpos,	// first derivatives - array of
								// length 2 in order xu, xv
				SPAvector *ddpos,	// second derivatives - array of
								// length 3 in order xuu, xuv, xvv
				SPAvector *dddpos,	// third derivatives - array of
								// length 4 in order xuuu, xuuv, xuvv, xvvv
				evaluate_surface_quadrant quadrant
			) const;

	virtual void eval_loft(
				evaluate_surface_quadrant quadrant,
				int v_index,
				int u_index,
		        logical direction,
				SPApar_pos const &uv,
				bernstein_basis &v_blend_data, 
				bernstein_basis &u_blend_data,
				SPAvector &pos,
				SPAvector *dpos,	// first derivatives - array of
								// length 2 in order xu, xv
				SPAvector *ddpos,	// second derivatives - array of
								// length 3 in order xuu, xuv, xvv
				SPAvector *dddpos,	// third derivatives - array of
								// length 4 in order xuuu, xuuv, xuvv, xvvv
				SPAvector *ddddpos	// third derivatives - array of
								// length 5 in order xuuuu, xuuuv, xuuvv, xuvvv, xvvvv
			) const;

  	virtual void eval_tensor(
				int v_index,
				int u_index,
				SPApar_pos const &uv,
				bernstein_basis &v_blend_data, 
				bernstein_basis &u_blend_data,
				SPAvector &pos,
				SPAvector *dpos,	// first derivatives - array of
								// length 2 in order xu, xv
				SPAvector *ddpos,	// second derivatives - array of
								// length 3 in order xuu, xuv, xvv
				SPAvector *dddpos,	// third derivatives - array of
								// length 4 in order xuuu, xuuv, xuvv, xvvv
				SPAvector *ddddpos	// third derivatives - array of
								// length 5 in order xuuuu, xuuuv, xuuvv, xuvvv, xvvvv
			) const;

	void sg_determine_periodicity(logical& closed_in_v, logical& closed_in_u);

	double get_good_u_param(SPApar_pos uv) const;

    logical map_curve_param_to_surface_param(logical u_param, int curve_number, double curve_param, double& surface_param);

// STI let (12/98) (slic025): Removed u_ and v_param_line methods
// The methods in the base class are preferrable.


	// Save and restore. Save is easy, as derived class switching goes
	// through the normal virtual function mechanism. Restore is more
	// complicated, because until it is invoked we do not have an
	// object of the right class. Instead we switch on a table defined
	// by static instances of the restore_ss_def class (see below), to
	// invoke a simple friend function which constructs an object of
	// the right (derived) type. Then it can call the appropriate
	// member function to do the actual work.

public:
	static int id();
	virtual int type() const;

	virtual char const *type_name() const;

	virtual void save() const;
	virtual void save_data() const;
	virtual logical need_save_as_approx(int save_to_version, logical check_progenitors) const;

	void get_curves(	
		int			&v_no,			// Number of curves in the array
		int			&u_no,			// Number of curves in the array
		curve**		&v_crv_array,	// Array of v curves to be surfaced (JMG)
		curve**		&u_crv_array );	// Array of u curves to be surfaced (JMG)

	void get_v_knots(double u,int &out_no_knots,double **out_vknots) const;
	void get_u_knots(double v,int &out_no_knots,double **out_uknots) const;

	/**
	* @nodoc
	**/
	virtual void process( geometry_definition_processor& p ) const;

protected:
	friend DECL_KERN subtype_object *restore_net_spl_sur();
	void restore_data();


#if defined D3_STANDALONE || defined D3_DEBUG

	friend DECL_KERN D3_istream& operator>>(
				D3_istream &,
				spl_sur *&
			);

	friend DECL_KERN D3_ostream& operator<<(
				D3_ostream &,
				spl_sur const &
			);

	virtual void input(
				D3_istream &
			);

	virtual void print(
				D3_ostream &
			) const;

#endif

	// Debug printout. As for save and restore we split the operation
	// into two parts: the virtual function "debug" prints a class-
	// specific identifying line, then calls the ordinary function
	// "debug_data" to put out the details. It is done this way so that
	// a derived class' debug_data can call its parent's version first,
	// to put out the common data. Indeed, if the derived class has no
	// additional data it need not define its own version of debug_data
	// and use its parent's instead. A string argument provides the
	// introduction to each displayed line after the first, and a
	// logical sets "brief" output (normally removing detailed
	// subsidiary curve and surface definitions).

	virtual void debug(
				char const *,
				logical,
				FILE *
			) const;
	void debug_data(
				char const *,
				logical,
				FILE *
			) const;
};

DECL_KERN bs3_surface make_net_approx(
                                        net_spl_sur const &,
                                        SPAinterval const &,                                                                    // u SPAparameter range
                                        SPAinterval const &,                                                                    // v SPAparameter range
                                        double &,                                                                                               // tol returned
                                        logical self_int_test = FALSE,                                                  // logical
                                        VOID_LIST& bad_uvs = *(VOID_LIST*)NULL_REF,                             // bad_uvs
                                        SPApar_box& exclude_region = *(SPApar_box*) NULL_REF,   // exclude region
                                        SPApar_box& extension_box = *(SPApar_box*) NULL_REF);   // extension_box

DECL_KERN double  surface_error(
                                net_spl_sur const& origin_surf,
                                bs3_surface& approx_surf,
                                int nu = 0,
                                double *u_knots = NULL,
                                int nv = 0,
                                double *v_knots = NULL,
                                double& = *(double *) NULL_REF,
                                double& = *(double *) NULL_REF);

DECL_KERN void remap_and_eval( 
			evaluate_curve_side curve_side,
			curve *, 		// Curve to evaluated.
			double,			// u SPAparameter
			SPAposition &, 	// SPAposition returned.
			SPAvector &, 		// First derivative returned
			SPAvector &, 		// Second derivative returned
			SPAvector & = *( SPAvector *)NULL_REF,	// 3'rd derivative , Used for 
         SPAvector & = *( SPAvector *)NULL_REF	// 4'rd derivative 
										// computing second partials on a 
										// loft surface.
		);

// allocate memory for the corner data
DECL_KERN netspl_corner ** sg_create_corner_data ( int number_v, int number_u );

// release memory for the corner data
DECL_KERN void sg_delete_corner_data ( netspl_corner **iCorner, int number_u );

// Create the corner curve corner data
DECL_KERN void  sg_create_corner_data ( netspl_corner **corners,
						    int no_crv_v, int no_crv_u,
							curve **v_curves, curve **u_curves );

DECL_KERN void  sg_create_corner_data ( netspl_corner **corners,
						    int no_crv_v, int no_crv_u,
							ENTITY_LIST *v_curves, ENTITY_LIST *u_curves );

// Create the twist corner data
DECL_KERN void sg_calculate_twist_vectors ( netspl_corner **corners, logical closed_in_v, logical closed_in_u, 
						    int no_crv_v, int no_crv_u, double *v_knot, double *u_knot);

// Calculate the surface SPAparameter derivatives at each corner
DECL_KERN void sg_calculate_surface_parameter_derivatives ( 
															netspl_corner **corners,
															int no_crv_v, 
															int no_crv_u, 
															double *v_knots, 
															double *u_knots, 
															logical* net_boundary_degen_info, 
															logical do_check,
															logical *use_new_algo = NULL,
															curve **v_curves = NULL, 
															curve **u_curves = NULL															
														 );

DECL_KERN void sg_net_determine_periodicity(int no_v, curve** v_crv_array, int no_u, curve** u_crv_array,	  
							                logical& closed_in_v, logical& closed_in_u , logical laterial_degeneracy = FALSE);
DECL_KERN void get_net_boundary_degenerate_info(
											const curve* const * grid_v,
											const curve* const * grid_u,
											const int& num_grid_v,
											const int& num_grid_u,
											logical* net_boundary_degen_info
										);

#endif
