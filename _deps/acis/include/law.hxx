// $Id: law.hxx,v 1.109 2002/08/09 17:15:32 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// 12-Jun-bmt surface_law_data - add evaluate method
#ifndef ACIS_LAW_HXX
#define ACIS_LAW_HXX

// including this file gets all the rest of the law stuff.
#include "main_law.hxx"
#include "law_base.hxx"
#include "law_data.hxx"
#include "law_util.hxx"
#include "internal_law.hxx"
#include "box.hxx"

#include "dcl_kern.h"
#include "surdef.hxx"


/**
* @file law.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISKERNLAWS
 *
 * @{
 */


class pointer_map;
class curve;
class BODY;
class WIRE;
class COEDGE;
class EDGE;
class pcurve;
class surface;
class FACE;
class SURFACE;
class CURVE;
class pattern;
class SPAbox;

/*
// tbrv
*/
/**
 * @nodoc
 */
typedef void (*proc_extend_curve_callback)(curve &,	SPAinterval const &);

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN void set_extend_curve_callback(proc_extend_curve_callback pfnCallback);

/*
// tbrv
*/
/**
 * @nodoc
 */
typedef void (*proc_extend_surface_callback)(surface &, SPApar_box const &);


/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN void set_extend_surface_callback(proc_extend_surface_callback pfnCallback);

// These next 4 are defined in simpsurf.cxx

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN SURFACE  *simplify_surface_law(law *in_law,
							             const SPAinterval& uint,
							             const SPAinterval& vint,
										 logical  flip);

/**
 * 示例
 */
DECL_KERN SURFACE* gme_simplify_surface_law(law* in_law,
											const SPAinterval& uint,
											const SPAinterval& vint,
											logical  flip);

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN SURFACE  *test_for_cone(law      *in_law,
                                  const SPAinterval& uint,
                                  const SPAinterval& vint,
                                  logical  flip);
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN CURVE *simplify_curve_law(law *in_law,
					                const SPAinterval& tint);

/**
 * @nodoc
 */
DECL_KERN CURVE *test_for_line(law *in_law, const SPAinterval& tint);

/**
 * @nodoc
 */
DECL_KERN CURVE *test_for_circle(law *in_law, const SPAinterval& tint);

/**
 * @nodoc
 */
DECL_KERN CURVE *test_for_ellipse(law *in_law, const SPAinterval& tint);

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN logical is_helix(EDGE     *the_edge,
						   SPAvector   &axis,
						   SPAposition &root = *(SPAposition *)NULL_REF,
						   double   &pitch = *(double *)NULL_REF,
						   double   &radius = *(double *)NULL_REF,
						   logical  &right_handed = *(logical *)NULL_REF
						   );
/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_KERN logical is_helix(curve    const &the_curve,
						   const SPAinterval& domain,
						   SPAvector   &axis,
						   SPAposition &root = *(SPAposition *)NULL_REF,
						   double   &pitch = *(double *)NULL_REF,
						   double   &radius = *(double *)NULL_REF,
						   logical  &right_handed = *(logical *)NULL_REF
						   );
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN logical is_helix(law      *the_law,
						   const SPAinterval& domain,
						   SPAvector   &axis,
						   SPAposition &root = *(SPAposition *)NULL_REF,
						   double   &pitch = *(double *)NULL_REF,
						   double   &radius = *(double *)NULL_REF,
						   logical  &right_handed = *(logical *)NULL_REF
						   );

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN logical is_planar(curve const &the_curve,
				  const SPAinterval& domain,
				  logical &pl_data_ok = *(logical *)NULL_REF,		// Flag returns TRUE if the plane data has been correctly computed
				  SPAposition &root = *(SPAposition *)NULL_REF,			// Root of the plane if the curve is planar
				  SPAunit_vector &normal = *(SPAunit_vector *)NULL_REF);	// Normal of the plane if the curve is planar
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN curve_law * make_curve_law(EDGE *in_edge,SPAtransf &in_trans= *(SPAtransf *)NULL_REF);

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN law *restore_law();

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN void save_law(law const* the_law); // was a method of law. now a function.

// These were in law_data.hxx:

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN law_data *restore_law_data();



/**
 * Serves as a wrapper for an ACIS <tt>WIRE</tt> object.
 * <br>
 * <b>Role:</b> This is a law data class that holds a pointer to a WIRE.
 * <br><br>
 * @see curve, SPAinterval, SPAposition, SPAvector
 */
class DECL_KERN wire_law_data : public base_wire_law_data
{
protected:
	curve**         acis_curves;
	double*         starts;
	double*         scales;
	SPAinterval*    param_ranges;
	int             size;
	logical         is_simple;
	// For caching
	int*            which_cached;	    // holds the time tags
	double*         tvalue;	    	    // holds the SPAparameter values
	SPAposition*    cached_f;	        // holds positions
	SPAvector*      cached_df;
	SPAvector*      cached_ddf;
	int             point_level;	    // size of tvalue
	int             derivative_level;   // How many derivatives are cached
public:
 /**
  * Applications are required to call this destructor for their law data types.
  */
	~wire_law_data();
 /**
  * Constructs a <tt>wire_law_data</tt> from an ACIS <tt>WIRE</tt>.
  * <br><br>
  * @param in_wire_body
  * wire.
  */
	wire_law_data(WIRE* in_wire_body);

 /**
  * Constructs a <tt>wire_law_data</tt> from an ACIS <tt>BODY</tt>.
  * <br><br>
  * @param in_wire_body
  * body.
  */
	wire_law_data(BODY* in_wire_body);

 /**
  * Constructs a <tt>wire_law_data</tt> from an ACIS <tt>COEDGE</tt> and an optional transform.
  * <br><br>
  * @param in_wire_coedge,
  * coedge.
  * @param in_trans
  * transform.
  */
	wire_law_data(
            COEDGE* in_wire_coedge,
            const SPAtransf& in_trans = *(const class SPAtransf*)NULL_REF);

 /**
  * Constructs a <tt>wire_law_data</tt> from an ACIS <tt>EDGE</tt> and an optional transform.
  * <br><br>
  * @param in_wire_edge
  * edge.
  * @param in_trans
  * transform.
  */
	wire_law_data(
            EDGE* in_wire_edge,
            const SPAtransf& in_trans = *(const class SPAtransf*)NULL_REF);

 /**
  * Constructs a <tt>wire_law_data</tt> from an array of ACIS <tt>curves</tt> and all of their defining data.
  * <br><br>
  * @param in_acis_curves
  * underlying curves.
  * @param in_starts
  * start parameter.
  * @param in_scales
  * scale for curve parameters.
  * @param in_param_ranges
  * parameter range.
  * @param in_size
  * number of parameters.
  */
	wire_law_data(
            curve**         in_acis_curves,
            double*         in_starts,
            double*         in_scales,
            SPAinterval*    in_param_ranges,
            int             in_size);

/**
 * Sets the number of levels for the points stored and the derivatives stored.
 * <br><br>
 * @param in_point_level
 * number of levels.
 * @param in_derivative_level
 * number of derivs.
 */
	void set_levels(int in_point_level = 4, int in_derivative_level = 2);


/*
// tbrv
*/
/**
 * @nodoc
 */
	virtual void save();// virtual method in base class

/*
// tbrv
*/
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type);// virtual method in base class

/*
// tbrv
*/
/**
 * @nodoc
 */
	SPAvector eval(double para, int deriv, int side = 0);// virtual method in path_law_data class

 /**
  * Given a parameter, this method returns the edge in <tt>wire_law_data</tt> that contains the specified parameter, and the parameter on that edge.
  * <br><br>
  * @param in_param
  * edge in list.
  * @param which_edge
  * edge.
  * @param edge_param
  * edge parameter.
  */
	void map_to_edge(
            double  in_param,
            int&    which_edge,
            double& edge_param);

/*
// tbrv
*/
/**
 * @nodoc
 */
	double curvature(double para);// virtual method in path_law_data class

 /**
  * Takes in a law and returns an array of laws and size of the array.
  * <br><br>
  * <b>Role:</b> The incoming law is split into an array of laws. Each array element
  * corresponds to an underlying curve in the wire, and has had its domain appropriately
  * remapped. This method is used by sweeping and wire-offsetting to apply a single law
  * to all of the curves of a wire.
  * <br><br>
  * type = 0: <tt>param_rail</tt> is FALSE.<br>
  * type = 1: used for mapping draft laws.<br>
  * type = 2: <tt>param_rail</tt> is TRUE.<br>
  * type = 3: used for mapping scale laws.
  * <br><br>
  * @param in_law
  * pointer to sublaw.
  * @param out_size
  * size of array.
  * @param type
  * type of mapping.
  */
	law** map_laws(law* in_law, int* out_size, int type = 0);

/*
// tbrv
*/
/**
 * @nodoc
 */
	int singularities(
            double**    where,
            int**       type,
            double      start,
            double      end);// virtual method in path_law_data class
 /**
  * Returns whether or not this <tt>wire_law_data</tt> is simple.
  */
	logical simple();

 /**
  * Returns the maximum curvature of the wire.
  */
	double max_curvature();

 /*
 // tbrv
 */
 /**
  * @nodoc
  */
	double* grid(int& size);

 /*
 // tbrv
 */
 /**
  * @nodoc
  */
	virtual law_data* deep_copy(base_pointer_map* pm = NULL) const;// virtual method in base class
};


/**
 * Serves as a wrapper for an ACIS <tt>curve</tt> object.
 * <br>
 * <b>Role:</b> This is a law data class that holds a pointer to a curve.
 * <br><br>
 * @see curve, SPAposition, SPAvector
 */
class DECL_KERN curve_law_data : public base_curve_law_data
{
protected:
	curve*          acis_curve;
	// For caching
private:
	int*            which_cached;	    // holds the time tags
	double*         tvalue;	    	    // holds the SPAparameter values
	int*			tside;				// holds the sidedness of the cached value
	SPAposition*    cached_f;	        // holds positions
	SPAvector*      cached_df;
	SPAvector*      cached_ddf;
	int             point_level;	    // size of tvalue
	int             derivative_level;   // How many derivatives are cached
public:

 /**
  * Applications are required to call this destructor for their law data types.
  */
	~curve_law_data();

 /**
  * Constructs a default object, with in_acis_pcurve as NULL
  */
        curve_law_data();

 /**
  * Constructs a <tt>curve_law_data</tt> from an ACIS <tt>curve</tt> and optional starting and ending parameters.
  * <br><br>
  * @param in_acis_curve
  * underlying ACIS curve.
  * @param in_start
  * start parameter.
  * @param in_end
  * end parameter.
  */
	curve_law_data(
            const curve& in_acis_curve,
            double in_start = 0,
            double in_end = 0);

 /**
  * Establishes the number of parameter values to store in <tt>tvalue</tt>, which in turn establishes the positions for <tt>cached_f</tt>.
  * <br><br>
  * @param in_point_level
  * number of positions.
  * @param in_derivative_level
  * number of derivatives.
  */
	void set_levels(int in_point_level = 4, int in_derivative_level = 2);

/*
// tbrv
*/
/**
 * @nodoc
 */
	virtual void save();// virtual method in base class

/*
// tbrv
*/
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type);// virtual method in base class

/*
// tbrv
*/
/**
 * @nodoc
 */
	SPAvector eval(double para, int deriv, int side = 0);// virtual method in path_law_data class

/*
// tbrv
*/
/**
 * @nodoc
 */
	double curvature(double para);// virtual method in path_law_data class

/*
// tbrv
*/
/**
 * @nodoc
 */
	double length_param(double base, double length);// virtual method in base_curve_law_data class

/*
// tbrv
*/
/**
 * @nodoc
 */
	double length(double start, double end);// virtual method in base_curve_law_data class

/**
 * Returns a pointer to the <tt>law</tt> class used as part of the <tt>curve_law_data</tt>.
 */
	law* law_form();

/**
 * Returns a pointer to the reference curve stored as part of the <tt>curve_law_data</tt>.
 */
	curve* curve_data();

/*
// tbrv
*/
/**
 * @nodoc
 */
	double point_perp(SPAposition in_point);// virtual method in base_curve_law_data class

/*
// tbrv
*/
/**
 * @nodoc
 */
	double point_perp(
            SPAposition in_point,
            double in_t);// virtual method in base_curve_law_data class

  /*
  // tbrv
  */
 /**
  * @nodoc
  */
	double closest_point(SPAposition in_point);

 /*
 // tbrv
 */
 /**
  * @nodoc
  */
	double closest_point(
            SPAposition in_point,
            double in_t);
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
	virtual law_data* deep_copy(base_pointer_map* pm = NULL) const;// virtual method in base class

 /*
 // tbrv
 */
 /**
  * @nodoc
  */
	law_data* set_domain(SPAinterval* new_domain);// virtual method in base class

 /*
 // tbrv
 */
 /**
  * @nodoc
  */
	int singularities(double** where, int** type, double start, double end);// virtual method in path_law_data class
};



/**
 * Serves as a wrapper for ACIS <tt>pcurve</tt> objects, for passing them as arguments to laws.
 * <br>
 * <b>Role:</b> This is a wrapper to handle specific ACIS <tt>pcurve</tt> objects. Such wrapper classes
 * are used by <tt>api_str_to_law</tt>. They are returned by the law method <tt>string_and_data</tt>.
 * <br><br>
 * @see pcurve, SPAposition, SPAvector
 */

class DECL_KERN pcurve_law_data : public base_pcurve_law_data
{
protected:
	pcurve*         acis_pcurve;
	// For caching
	int*            which_cached;	    // holds the time tags
	double*         tvalue;	    	    // holds the SPAparameter values
	SPAposition*    cached_f;	        // holds positions
	SPAvector*      cached_df;
	SPAvector*      cached_ddf;
	int             point_level;	    // size of tvalue
	int             derivative_level;   // How many derivatives are cached
public:

 /**
  * C++ destructor, deleting a <tt>pcurve_law_data</tt>.
  */
	~pcurve_law_data();

 /**
  * Constructs a default object, with in_acis_pcurve as NULL
  */
	pcurve_law_data();

 /**
  * Constructs a <tt>pcurve_law_data</tt> from an ACIS <tt>pcurve</tt> and optional starting and ending parameters.
  * <br><br>
  * <b>Role:</b> It sets <tt>use_count</tt> to 1 and increments <tt>how_many_laws</tt>.
  * It sets <tt>dlaw</tt>, <tt>slaw</tt>, and <tt>lawdomain</tt> to NULL.
  * <br><br>
  * @param in_acis_pcurve
  * pointer to ACIS pcurve.
  * @param in_start
  * starting parameter.
  * @param in_end
  * ending parameter.
  */
	pcurve_law_data(
            const pcurve& in_acis_pcurve,
            double in_start = 0,
            double in_end = 0);

 /**
  * Changes the number of points and derivative levels to cache.
  * <br><br>
  * @param in_point_level
  * input point level.
  * @param in_derivative_level
  * number of derivatives.
  */
	void set_levels(int in_point_level = 4, int in_derivative_level = 2);

/*
// tbrv
*/
/**
 * @nodoc
 */
	virtual void save();// virtual method in base class

/*
// tbrv
*/
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type);// virtual method in base class

/*
// tbrv
*/
/**
 * @nodoc
 */
	SPAvector eval(double para, int deriv, int side = 0);// virtual method in path_law_data class

/*
// tbrv
*/
/**
 * @nodoc
 */
	double curvature(double para);// virtual method in path_law_data class

/**
 * Returns a pointer to the actual ACIS <tt>pcurve</tt> wrapped by this <tt>pcurve_law_data</tt>.
 */
	pcurve* pcurve_data();

/*
// tbrv
*/
/**
 * @nodoc
 */
	virtual law_data* deep_copy(base_pointer_map* pm = NULL) const;// virtual method in base class

/*
// tbrv
*/
/**
 * @nodoc
 */
	int singularities(double** where, int** type, double start, double end);// virtual method in path_law_data class
};


/**
 * Serves as a wrapper for ACIS <tt>surface</tt> objects.
 * <br>
 * <b>Role:</b> This is a law data class that holds a pointer to a surface.
 * <br><br>
 * @see surface, SPAinterval, SPApar_pos, SPAposition
 */
class DECL_KERN surface_law_data : public base_surface_law_data
{
protected:
	surface*        acis_surface;
	SPAinterval     u_domain;
	SPAinterval     v_domain;
	// For caching
	int*            which_cached;   // holds the time tags
	SPApar_pos*     tvalue;	        // holds the SPAparameter values
	SPAposition*    cached_f;	    // holds positions
	int             point_level;    // size of tvalue

	void init(
            const surface& in_acis_surface,
            const SPAinterval& in_u_domain,
            const SPAinterval& in_v_domain);// called by both constructors.
public:
/**
 * Applications are required to call this destructor for their law data types.
 */
	~surface_law_data();

/**
 * Constructs a default object, with in_acis_pcurve as NULL
 */
        surface_law_data();

/**
 * Constructs a <tt>surface_law_data</tt> from an ACIS <tt>surface</tt> and the corresponding domains in <i>u</i> and <i>v</i>.
 * <br><br>
 * @param in_acis_surface
 * surface.
 * @param in_u_domain
 * u parameter range.
 * @param in_v_domain
 * v parameter range.
 */
   surface_law_data(
            const surface& in_acis_surface,
            const SPAinterval& in_u_domain,
            const SPAinterval& in_v_domain);
/**
 * Constructs a <tt>surface_law_data</tt> from an ACIS <tt>surface</tt>.
 * <br><br>
 * @param in_acis_surface
 * surface.
 */
   surface_law_data(const surface& in_acis_surface);

/**
 * Establishes the number of positions stored in <tt>tvalue</tt> in preparation for starting over and clears out cached arrays of the positions and their derivatives.
 * <br><br>
 * @param in_point_level
 * number of tvalues.
 * @param in_derivative_level
 * number of derivs.
 */
   void set_levels(int in_point_level = 4, int in_derivative_level = 2);

/*
// tbrv
*/
/**
 * @nodoc
 */
	virtual void save();// virtual method in base class

/*
// tbrv
*/
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type);// virtual method in base class

/*
// tbrv
*/
/**
 * @nodoc
 */
	void eval(                              // virtual method in base_surface_law_data class
                SPApar_pos&     uv,
                SPAposition&    pos,
                SPAvector*      dpos,       // first derivatives - array of
				                            // length 2 in order xu, xv
                SPAvector*      ddpos);	    // second derivatives - array of
								            // length 3 in order xuu, xuv, xvv

/*
// tbrv
*/
/**
 * @nodoc
 */
	int evaluate(
                SPApar_pos&     uv,
                SPAposition&    pos,
                SPAvector*      dpos,       // first derivatives - array of
				                            // length 2 in order xu, xv
                SPAvector*      ddpos,	    // second derivatives - array of
				int const*      side);


/*
// tbrv
*/
/**
 * @nodoc
 */
	void eval_normal(
				SPApar_pos &uv,
				SPAunit_vector &norm
			);
/**
 * Returns a pointer to the reference surface stored as part of the <tt>surface_law_data</tt>.
 */
	surface* surface_data();
/*
// tbrv
*/
/**
 * @nodoc
 */
	SPApar_pos point_perp(SPAposition in_point);// virtual method in base_surface_law_data class

/*
// tbrv
*/
/**
 * @nodoc
 */
	SPApar_pos point_perp(
            SPAposition in_point,
            SPApar_pos  in_par_pos);// virtual method in base_surface_law_data class

/*
// tbrv
*/
/**
 * @nodoc
 */
	double eval_gaussian_curvature(const SPApar_pos& in_par_pos) const;// virtual method in base_surface_law_data class

/*
// tbrv
*/
/**
 * @nodoc
 */
	double eval_mean_curvature(const SPApar_pos& in_par_pos) const;// virtual method in base_surface_law_data class

/*
// tbrv
*/
/**
 * @nodoc
 */
	double eval_max_curvature(const SPApar_pos& in_par_pos) const;// virtual method in base_surface_law_data class

/*
// tbrv
*/
/**
 * @nodoc
 */
	double eval_min_curvature(const SPApar_pos& in_par_pos) const;// virtual method in base_surface_law_data class

/*
// tbrv
*/
/**
 * @nodoc
 */
	logical term_domain(int which, SPAinterval& answer);// virtual method in base_surface_law_data class

/*
// tbrv
*/
/**
 * @nodoc
 */

	law_data* set_domain(SPAinterval* new_domain);// virtual method in base class

/*
// tbrv
*/
/**
 * @nodoc
 */
	SPApar_pos* grid(int& size, SPApar_vec& gsize);


/*
// tbrv
*/
/**
 * @nodoc
 */
	virtual law_data* deep_copy(base_pointer_map* pm = NULL) const;// virtual method in base class

/*
// tbrv
*/
/**
 * @nodoc
 */
	SPAposition bs3_eval(const SPApar_pos& in_par_pos) const;// virtual method in base_surface_law_data class

};

/**
 * Serves as a wrapper for an ACIS <tt>SPAtransf</tt> object.
 * <br>
 * <b>Role:</b> This is a law data class that holds a pointer to a transform.
 */
class DECL_KERN transform_law_data : public base_transform_law_data
{
public:

/**
 * Constructs a <tt>transform_law_data</tt>.
 * <br><br>
 * @param in_data
 * the transform to wrap.
 */
  transform_law_data(const SPAtransf* in_data);

/*
// tbrv
*/
/**
 * @nodoc
 */
   virtual void save();// virtual method in base class

/*
// tbrv
*/
/**
 * @nodoc
 */
   virtual base_transform_law_data*
       make_one(const SPAtransf* in_data) const;// virtual method in base_transform_law_data class

/*
// tbrv
*/
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type);// virtual method in base class
};



/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN position_array_law_data: public base_position_array_law_data
{
public:
	position_array_law_data(SPAposition const *in_data,
		                    int const *dim_size,
							int dimension);
	virtual void save();
	virtual base_position_array_law_data * make_one(SPAposition const *in_data,
		                    int const *dim_size,
							int dimension)const;
	char  const *symbol(law_symbol_type type);
};



/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN pattern_law_data: public base_pattern_law_data
{
protected:
	pattern* pat;
    int     root_index;
public:
	~pattern_law_data();
	pattern_law_data(pattern* in_pat, int in_root_index = 0);
    SPAtransf eval(const double* coords);
	const char* symbol(law_symbol_type type);
	virtual void save();
	virtual law_data* deep_copy(base_pointer_map* pm = NULL) const;
	logical term_domain(int which, SPAinterval& answer);
    int take_dim() const;
    int scale_dim() const;
};

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN COEDGE *find_first_coedge(BODY *wire_body,int &size);
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN int find_coedge_number(COEDGE *start_coedge);
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN COEDGE *start_of_wire_chain(COEDGE* wire_coedge);
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN SPAtransf &get_face_trans(FACE *face);
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN law *make_rotate_law( law *in_law,
							    SPAtransf const *in_trans);

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN law *make_transform_law( law *in_law,
								  SPAtransf *in_trans);

/** @} */
#endif /* ACIS_LAW_HXX */
