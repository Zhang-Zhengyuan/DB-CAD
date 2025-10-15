/* $Id: law_spl.hxx,v 1.28 2002/08/20 15:20:47 jenglund Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// 12-Jun-09 bmt law_spl_sur - add calculate_disc_info & evaluate methods to support 
//               discontinuities and sided evaluation.
#if !defined( law_spl_sur_CLASS )
#define law_spl_sur_CLASS			

#include "logical.h"
#include "acis.hxx"
#include "dcl_kern.h"
#include "law.hxx"
#include "debugmsc.hxx"

class intcurve;
class SPApar_pos;
class SPAposition;
class SPAvector;
class SPAunit_vector;
class SPAtransf;
class SPAbox;
class SPApar_box;
class VOID_LIST;
//class fvec;

// STI ROLL
class SizeAccumulator;
// STI ROLL


#include "bs3curve.hxx"
#include "position.hxx"
#include "spldef.hxx"

class law_spl_sur;
DECL_KERN bs3_surface make_law_approx( law_spl_sur const &,  double tol, 
									  double &, logical self_int_test = FALSE, 
									  VOID_LIST& bad_uvs= *(VOID_LIST*)NULL_REF, 
									  SPApar_box& exclude_region = *(SPApar_box*)NULL_REF, 
									  SPApar_box& extension_box = *(SPApar_box*)NULL_REF);
DECL_KERN law_spl_sur* make_law_spl_sur(
		law*				in_law,
		const SPAinterval&	urange,
		const SPAinterval&	vrange,
		double              tol				= SPAresfit,
		int					in_law_number	= 0,
		law**				in_other_laws	= NULL);

DECL_KERN double surface_tolerance (law_spl_sur const& origin_surf, 
									bs3_surface &approx_surf  );
DECL_KERN subtype_object *restore_law_spl_sur();


class DECL_KERN law_spl_sur: public spl_sur {

// Allow extensions to declare themselves as friends.
#ifdef law_spl_sur_FRIENDS
law_spl_sur_FRIENDS
#endif

protected:

	law *data_law;

    // The u_range and v_range fields that were in this class are now in the 
    // base class spl_sur.  PC Sep-98. 

	/////////////////////////////////////////////////////////////////
	//
	//  Helper laws make law surfaces act more analitic by filling
	//  in closed form solutions to things other that evaluation of
	//  the surface.
	//
	/////////////////////////////////////////////////////////////////

	int law_number;		// Number of helper laws

	law **other_laws;	// Extra laws for methods of the class

	//////////////////// Short definitions //////////////////////////
	//
	//	0	xyz->uv weak inverse
	//	1	(px,py,pz,vx,vy,vz)->t's p+t*v
	//	2	Untransformed data law
	//	3	The transform
	//	4	Identity of the surface type constant law
	//	5	Vector law for holding defining data
	//  6   xyz->uv strong inverse
	//  7   xyz->t a tie breaker for branched inverses
	//
	//////////////////// Long definitions ///////////////////////////

	// Helper law number zero in the other_laws array is
	// the inverse law (i.e. xyz->uv).  But off the surface it may
	// be any continious extension.
	
	// Helper law number one is the line intersection law
	// a law that takes six values and returns some fixed 
	// number of t values.  The six input values are 
	// <px,py,pz> and <vx,vy,vz> where p is the root point on
	// the line and v is its direction SPAvector.  The return
	// t values are such that p+t*v is may be a point of intersection
	// the values are checked so that bad or extra intersections
	// may be returned.

	// Helper law number two is the untransformed data_law
	
	// Helper law number three is transform on the surface

	// Helper law number four is identity of the surface
	// this law should be a constant_law that that is not used by
	// any other surface type

	// Helper law number five is a law to hold defining data

	// Helper law number six is line number zero except it returns the 
	// closest point the the surface.

	/////////////////////// End of helper law docs ////////////////////

	// Constructor : Private for internal use only
	// Default constructor, used in to create a empty class, so that
	// the copy function can use it.
	law_spl_sur();

	//fvec *facets; // Used to find starting values for point_perp
    virtual	void 	calculate_disc_info();

public:

	// Declare the function make_law_approx as a friend of the law_spl_sur
	// class, as this will enable this function to use the members of
	// this class freely.  If self_int_test is TRUE, self intersection check is
    // done on the surface and NULL is returned if singularities are found.  
    // Par_pos of singularities (which must be deleted by the caller)
    // are returned in the bad_uvs list if provided.  An exclude region is an area
    // that is not tested against itself (e.g. in the case of extensions where
    // a region is assumed to be good).  If the exclude_region and extension_box
    // are both supplied, the extension SPAbox contains the exclude_region and no
    // singularities.
	friend DECL_KERN bs3_surface make_law_approx( 
								law_spl_sur const &, 
								double tol,
								double &,
                                logical self_int_test,
                                VOID_LIST& bad_uvs,
                                SPApar_box& exclude_region,
                                SPApar_box& extension_box

							);

	// Make or remake the approximating surface.  This function, which is a 
    // virtual function of spl_sur, is implemented using make_law_approx. 
	// The function stores the approximating surface and the actual fit error 
    // that was achieved in the spl_sur, overriding the declared const-ness 
    // of the function to do this. 

    virtual void	make_approx( 
							 double fit, 
							 const spline& spl = *(spline*) NULL_REF, 
						     logical force = FALSE
							 ) const; 


	friend DECL_KERN double  surface_tolerance
           ( law_spl_sur  const& origin_surf,
             bs3_surface   &approx_surf 
			);

	// Use make_law_spl_sur() rather then this constructor to avoid possible 
	// leaks caused by sys-errors
	law_spl_sur(
			law *,                      // defining law
			SPAinterval const &,			// u range
			SPAinterval const &,			// v range
			double tol=SPAresfit,          // tolerance to fit bspline
			int in_law_number=0,		// Number of extra laws
			law **in_other_laws=NULL	// array of extra laws
		);

	// Make a copy without sharing subdata.

	virtual spl_sur *deep_copy(pointer_map * pm = NULL) const;

	int accurate_derivs(
               SPApar_box const & = *(SPApar_box *)NULL_REF
                                   // Defaults to the whole surface
           ) const;

	law *get_law() const;

	law **get_other_laws() const;
	
	int get_law_number() const;

	SPAbox bound(SPApar_box const & = *(SPApar_box *)NULL_REF);

	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL

protected:
	// Destructor needs to delete the spine curve and law.

	virtual ~law_spl_sur();

	// Construct a duplicate in free store of this object but with
	// zero use count.

	virtual subtrans_object *copy() const;


	// Test for equality. This does not guarantee that all
	// effectively equal surfaces are determined to be equal, but
	// does guarantee that different surfaces are correctly
	// identified as such.

	logical operator==( subtype_object const & ) const;

	// Parameter shift: adjust the spline surface to have a SPAparameter
	// range increased by the argument value (which may be negative).
	// This is only used to move portions of a periodic surface by
	// integral multiples of the period.

    virtual void shift_u( double );

	// Parameter shift: adjust the spline surface to have a SPAparameter
	// range increased by the argument value (which may be negative).
	// This is only used to move portions of a periodic surface by
	// integral multiples of the period.

	virtual void shift_v( double );

	// Transform this law by the given SPAtransf.

	virtual void operator*=( SPAtransf const &	);

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

	// Geometric evaluation.

	// Find the normal to the surface at a point on the surface 
	// nearest to the given point.

	virtual SPAunit_vector point_normal(
				SPAposition const &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF
			) const;


	// Find the principal axes of curvature of the surface at a
	// given point, and the curvatures in those directions.

	virtual void point_prin_curv(
				SPAposition const &,
				SPAunit_vector &,		// first axis direction
				double &,			// curvature in first direction
				SPAunit_vector &,		// second axis direction
				double &,			// curvature in second direction
				SPApar_pos const & = *(SPApar_pos *)NULL_REF,
                evaluate_surface_quadrant = evaluate_surface_unknown
			) const;


	// Find the point on the surface nearest to the given point, 
	// iterating from the given SPAparameter values (if supplied).
	// Return the found point, the normal to the surface at that 
	// point and the SPAparameter values at the found point.

	virtual void point_perp(
				SPAposition const &,
				SPAposition &,
				SPAunit_vector &,
				surf_princurv &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF,
				SPApar_pos & = *(SPApar_pos *)NULL_REF,
				logical f_weak = FALSE,
			    SPApar_box const & = *(SPApar_box *)NULL_REF
			) const;


	// Find the SPAparameter values of a point on the surface.

	virtual SPApar_pos param(
				SPAposition const &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF
			) const;


	// Find the change in surface SPAparameter corresponding to a unit
	// offset in a given direction at a given uv, the direction
	// lying in the surface.

	virtual SPApar_vec param_unitvec(
				SPAunit_vector const &,
				SPApar_pos const &
			) const;

	virtual int evaluate(
			SPApar_pos const &uv,
			SPAposition &pos,
			SPAvector **deriv,
			int nd,
			evaluate_surface_quadrant quad
		) const;

	// Find the SPAposition and first and second  derivatives of the
	// surface at a given point.
/*
	virtual void eval(
				SPApar_pos const &uv,
				SPAposition &pos,
				SPAvector *dpos,	// first derivatives - array of
								// length 2 in order xu, xv
				SPAvector *ddpos	// second derivatives - array of
								// length 3 in order xuu, xuv, xvv
			) const;
*/
	// Find the normal to the surfac at the point with given SPAparameter
	// values.
	
	virtual SPAunit_vector eval_normal(
				SPApar_pos const &
			) const;

	// Find the principal axes of curvature of the surface at a
	// point with given SPAparameter values, and the curvatures in those
	// directions.

	virtual void eval_prin_curv(
				SPApar_pos const &,
				SPAunit_vector &,		// first axis direction
				double &,			// curvature in first direction
				SPAunit_vector &,		// second axis direction
				double &,			// curvature in second direction
                evaluate_surface_quadrant = evaluate_surface_unknown
			) const;

	// Find the curvature of a cross-section curve of the surface at
	// the point on the surface with given SPAparameter values.
	// The cross-section curve is defined as the intersection of 
	// the surface with a plane passing through the point on the
	// surface and normal to the given direction, which must lie in
	// the surface.

	virtual double eval_cross(
				SPApar_pos const &,
				SPAunit_vector const &
			) const;

	// Test whether a point lies on the surface.

	virtual logical test_point_tol(
				SPAposition const &,
				double,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF,
				SPApar_pos & = *(SPApar_pos *)NULL_REF
			) const;

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

	// Copy constructor.

	law_spl_sur( const law_spl_sur& );

	static int id();

	virtual int type() const;

	virtual char const *type_name() const;

	virtual void save() const;
	
	virtual void save_data() const;

	virtual logical need_save_as_approx(int save_to_version, logical check_progenitors) const;

	/**
	* @nodoc
	**/
	virtual void process( geometry_definition_processor& p ) const;

protected:
	friend DECL_KERN subtype_object *restore_law_spl_sur();
	void restore_data();


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

#if 0
class DECL_KERN fvec : public ACIS_OBJECT
{
private:
	int      use_count;
	SPAposition *xyz;
	double   *u;
	double   *v;
	int      unum;
	int      vnum;
	SPApar_box  uvbounds;
protected:
	~fvec();
public:
	fvec( SPAposition *in_xyz,
		  double   *in_u,
		  double   *in_v,
		  int      in_unum,
		  int      in_vnum,
		  SPApar_box  in_uvbounds);
	fvec( bs3_surface in_bs);
	void add();
	void remove();
	SPApar_pos point_perp(SPAposition xyz);
	fvec *deep_copy();
	void split_u( double in_u,
				  fvec   *&low,
			      fvec   *&high);
	void split_v( double in_v,
				  fvec   *&low,
			      fvec   *&high);
	void transform(SPAtransf const &t);
	void shift_u( double ushift);
	void shift_v( double vshift);
};
#endif

#endif
