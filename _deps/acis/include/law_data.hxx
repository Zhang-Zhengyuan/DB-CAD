/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef LAW_DATA_HXX
#define LAW_DATA_HXX

#include "dcl_law.h"
#include "logical.h"
#include "law_base.hxx"
#include "vector.hxx"
#include "interval.hxx"

class base_pointer_map;
class SPAinterval;
class SPApar_vec;
class law;
class SPAtransf;
class SPAposition;
class SPAunit_vector;


/////////////////////////////////////////////////////////////
//
// The law_data class
//
/////////////////////////////////////////////////////////////
/**
 * @file law_data.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISKERNLAWS
 *
 * @{
 */
 /**
  * Serves as a wrapper for ACIS objects, for passing as arguments to laws.
  * <br>
  * <b>Role:</b> This is the virtual base class for a series of wrappers that handle
  * specific ACIS entities and other ACIS classes. Objects constructed from its
  * derived classes may be parsed into strings through the use of tags, such as "edge",
  * "transform", "path", "curve" and "wire" and then used by <tt>api_str_to_law</tt>. They are
  * returned by the <tt>law</tt> method <tt>string_and_data</tt>.
  * @see multiple_data_law, unary_data_law
  */
class DECL_LAW law_data : public ACIS_OBJECT
{
	friend class law_data_list;
	int use_count;
protected:
	virtual ~law_data();
public:
/**
 * Constructs a <tt>law_data</tt>.
 */
	law_data();

/**
 * @nodoc
 */
	virtual logical operator==(law_data const& rhs) const;

/**
 * Saves the <tt>law_data</tt> with its encapsulated data.
 */
	virtual void save() = 0;
/**
 * Returns <tt>NULL</tt>.
 */
	virtual law_data* restore();
/**
 * Returns the string that represents this <tt>law_data's</tt> symbol.
 * <br><br>
 * <b>Role:</b> This is a pure virtual method, forcing derived classes to
 * define their own versions of this method. Hence, this method is called from
 * the derived class and not from this abstract class.
 * <br><br>
 * @param type
 * type of symbol - standard ACIS type.
 */
	virtual const char* symbol(law_symbol_type type = DEFAULT) = 0;
/**
 * Returns a string that represents this <tt>law_data</tt>.
 * <br><br>
 * <b>Role:</b> The law function is composed of its symbol, associated parentheses,
 * and the strings associated with its sublaws. It is provided as a user-friendly
 * interface to laws. A derived class must override this function to be able
 * to save a <tt>law_data</tt>.
 * <br><br>
 * @param type
 * type of symbol - standard ACIS type.
 * @param count
 * count.
 * @param ldn
 * law data node.
 */
	virtual char* string(
            law_symbol_type type    =   DEFAULT,
            int&            count   =   *(int*) NULL_REF,
            law_data_node*& ldn     =   *(law_data_node**) NULL_REF);
/**
 * Increments the use count of this <tt>law_data</tt>.
 * <br><br>
 * <b>Role:</b> This method is called in all <tt>law_data</tt> constructors for the <tt>law_data</tt> being constructed, and
 * on all of its sublaws.
 */
	void add();
/**
 * Decrements the use count of this <tt>law_data</tt>, and deletes the <tt>law_data</tt> when the count drops to zero.
 * <br><br>
 * <b>Role:</b> This method is called by all <tt>law_data</tt> destructors for a <tt>law_data</tt>, and on all its sublaws.
 * Users of a <tt>law_data</tt> should call <tt>remove</tt> instead of <tt>delete</tt> to delete it.
 */
	void remove();
/**
 * Returns the version of ACIS in which this <tt>law_data</tt> class first appeared.
 * <br><br>
 * <b>Role:</b> If a <tt>law_data</tt> is part of a model that is to be saved at a previous
 * ACIS release level, this is used to indicate whether the law can be saved
 * or not.
 */
	virtual int date();
/**
 * Creates a copy of an item that does not share any data with the original.
 * <br><br>
 * <b>Role:</b> Allocates new storage for all member data and any pointers.
 * Returns a pointer to the copied item.
 * <br><br>
 * In a <i>deep</i> copy, all the information about the copied item is self-contained
 * in a new memory block. By comparison, a <i>shallow</i> copy stores only the first
 * instance of the item in memory, and increments the reference count for each copy.
 * <br><br>
 * The <tt>pointer_map</tt> keeps a list of all pointers in the original object that
 * have already been deep-copied. For example, a <tt>deep_copy</tt> of a complex model
 * results in self contained data, but identical sub-parts within the model are allowed
 * to share a single set of data.
 * <br><br>
 * @param pm
 * list of items within the entity that are already deep copied.
 */
	virtual law_data* deep_copy(base_pointer_map* pm = NULL) const = 0;
/**
 * Sets the domain of this <tt>law_data</tt> to the given interval.
 * <br><br>
 * @param new_domain
 * the new domain.
 */
	virtual law_data* set_domain(SPAinterval* new_domain);
/**
 * Returns the use count of this <tt>law_data</tt>.
 */
	int ref_count() { return use_count; };

	// STI ROLL
/**
 * @nodoc
 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL
};


/////////////////////////////////////////////////////////////
//
// The path_law_data class
//
/////////////////////////////////////////////////////////////
/**
 * Serves as a wrapper for either a curve or wire object, for input into a law.
 * <br>
 * <b>Role:</b> This allows curves and wires to behave in the same way. It places
 * a global parameterization on all of the curves in a wire.
 */
class DECL_LAW path_law_data : public law_data
{
protected:
/**
 * The starting parameter value of the path.
 */
	double start;
/**
 * The ending parameter value of the path.
 */
	double end;
public:
/**
 * Constructs a <tt>path_law_data</tt>.
 * <br><br>
 * <b>Role:</b> This is an abstract data class. An instance of this is never
 * created directly. One of the derived classes from this class calls this
 * constructor method.
 * <br><br>
 * @param in_start
 * start parameter.
 * @param in_end
 * end parameter.
 */
	path_law_data(double in_start = 0, double in_end = 0);
/**
 * Returns the ending parameter of the path.
 */
	double data_end()   {return end;}
/**
 * Returns the starting parameter of the path.
 */
	double data_start() {return start;}
/**
 * This method evaluates the <i>n</i>th derivative of the given law at the specified parameter value.
 * <br><br>
 * <b>Role:</b> For convenience, all law classes inherit this method. It calls
 * the main <tt>evaluate</tt> member function and does some checking using
 * <tt>take_dim</tt> and <tt>return_dim</tt>.
 * <br><br>
 * @param para
 * parameter position.
 * @param deriv
 * number of derivatives.
 * @param side
 * left (-1) or right (1) - default = 0.
 */
	virtual SPAvector eval(double para, int deriv, int side = 0) = 0;
/**
 * Returns the curvature of the path at the given parameter.
 * <br><br>
 * @param para
 * parameter for test.
 */
	virtual double curvature(double para) = 0;
/**
 * Specifies where in this <tt>law_data</tt> there might be discontinuities.
 * <br><br>
 * <b>Role:</b> The array <tt>where</tt> notes where the discontinuity occurs. The <tt>type</tt>
 * indicates 0 if there is a discontinuity, 1 if the discontinuity in the 1st
 * derivative, and any integer <i>n</i> if the discontinuity is in the <i>n</i>th derivative.
 * The value -1 means that the type is not defined.
 * <br><br>
 * @param where
 * where discontinuities exist.
 * @param type
 * discontinuity types.
 * @param start
 * start parameter.
 * @param end
 * end parameter.
 */
	virtual int singularities(
            double**    where,
            int**       type,
            double      start,
            double      end);
	// STI ROLL
/**
 * @nodoc
 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL
};

/////////////////////////////////////////////////////////////
//
// The base_wire_law_data class
//
/////////////////////////////////////////////////////////////
/**
 * Abstract base class for accessing a <tt>wire_law_data</tt> with or without the ACIS kernel.
 */
class DECL_LAW base_wire_law_data : public path_law_data
{
public:
/**
 * Constructs a <tt>base_wire_law_data</tt>.
 * <br><br>
 * @param in_start
 * start parameter.
 * @param in_end
 * end parameter.
 */
	base_wire_law_data(double in_start = 0, double in_end = 0);
};

/**
 * @nodoc
 */
inline base_wire_law_data::base_wire_law_data(double in_start, double in_end) :
      path_law_data(in_start, in_end) {}

/////////////////////////////////////////////////////////////
//
// The base_curve_law_data class
//
/////////////////////////////////////////////////////////////
/**
 * Abstract base class for accessing a <tt>curve_law_data</tt> with or without the ACIS kernel.
 */
class DECL_LAW base_curve_law_data : public path_law_data
{
public:
/**
 * Constructs a <tt>base_curve_law_data</tt>.
 * <br><br>
 * @param in_start
 * start parameter.
 * @param in_end
 * end parameter.
 */
	base_curve_law_data(double in_start = 0, double in_end = 0);
/**
 * Returns the parameter value at the end point.
 * <br><br>
 * @param base
 * starting parameter.
 * @param length
 * distance to end.
 */
	virtual double length_param(double base, double length) = 0;
/**
 * Returns the distance between the two parameters.
 * <br><br>
 * @param start
 * start param.
 * @param end
 * end param.
 */
	virtual double length(double start, double end) = 0;
/**
 * Finds the point on the curve nearest to the given point.
 * <br><br>
 * @param in_point
 * point.
 */
	virtual double point_perp(SPAposition in_point) = 0;
/**
 * Finds the point on the curve nearest to the given point, given a guess as to its value.
 * <br><br>
 * @param in_point
 * point.
 * @param in_t
 * parameter.
 */
	virtual double point_perp(SPAposition in_point, double in_t) = 0;
/**
 * @nodoc
 */
	virtual double closest_point(SPAposition in_point);
/**
 * @nodoc
 */
	virtual double closest_point(SPAposition in_point, double in_t);
/**
 * Returns a pointer to the <tt>law</tt> class used as part of the <tt>base_curve_law_data</tt>.
 */
	virtual law* law_form() = 0;

/**
 * @nodoc
 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
};

/**
 * @nodoc
 */
inline base_curve_law_data::base_curve_law_data(double in_start, double in_end) :
		path_law_data(in_start, in_end) {}

/////////////////////////////////////////////////////////////
//
// The base_pcurve_law_data class
//
/////////////////////////////////////////////////////////////
/**
 * Abstract base class for accessing a <tt>pcurve_law_data</tt> with or without the ACIS kernel.
 * <br>
 * <b>Role:</b> This is a wrapper to handle specific ACIS pcurve classes. These
 * wrapper classes are used by <tt>api_str_to_law</tt>. These are returned by the law
 * method <tt>string_and_data</tt>.
 */
class DECL_LAW base_pcurve_law_data : public path_law_data
{
public:
/**
 * Constructs a <tt>base_pcurve_law_data</tt>.
 * <br><br>
 * @param in_start
 * start parameter.
 * @param in_end
 * end parameter.
 */
	base_pcurve_law_data(double in_start = 0, double in_end = 0);
};

/**
 * @nodoc
 */
inline base_pcurve_law_data::base_pcurve_law_data(double in_start, double in_end) :
		path_law_data(in_start, in_end) {}

/////////////////////////////////////////////////////////////
//
// The base_surface_law_data class
//
/////////////////////////////////////////////////////////////
/**
 * Abstract base class for accessing a <tt>surface_law_data</tt> with or without the ACIS kernel.
 * <br>
 * <b>Role:</b> This is a law data class that holds a pointer to a surface.
 */
class DECL_LAW base_surface_law_data : public law_data
{
public:
/**
 * Constructs a <tt>base_surface_law_data</tt>.
 */
	base_surface_law_data();
/**
 * Takes in a <i>uv</i> parameter position and returns various properties.
 * <br><br>
 * <b>Role:</b> This takes in a <i>uv</i> parameter position and returns the corresponding
 * <i>xyz</i> position on the surface; a vector array, which holds the derivative with
 * respect to <i>u</i> and the derivative with respect to <i>v</i>; and an array with three
 * vectors, which correspond to the second derivative with respect to <i>u</i>, the
 * derivative with respect to <i>u</i> and then to <i>v</i>, and the second derivative with
 * respect to <i>v</i>.
 * <br><br>
 * @param uv
 * parameter position at which to evaluate.
 * @param pos
 * output surface position.
 * @param dpos
 * array of 1st derivatives.
 * @param ddpos
 * array of 2nd derivatives.
 */
	virtual void eval(
                SPApar_pos&     uv,
                SPAposition&    pos,
                SPAvector*      dpos,       // first derivatives - array of
				                            // length 2 in order xu, xv
                SPAvector*      ddpos) = 0;	// second derivatives - array of
								            // length 3 in order xuu, xuv, xvv
/**
 * Takes in a <i>uv</i> parameter position and returns various properties.
 * <br><br>
 * <b>Role:</b> This takes in a <i>uv</i> parameter position and returns the corresponding
 * <i>xyz</i> position on the surface; a vector array, which holds the derivative with
 * respect to <i>u</i> and the derivative with respect to <i>v</i>; and an array with three
 * vectors, which correspond to the second derivative with respect to <i>u</i>, the
 * derivative with respect to <i>u</i> and then to <i>v</i>, and the second derivative with
 * respect to <i>v</i>.
 * <br><br>
 * @param uv
 * parameter position at which to evaluate.
 * @param pos
 * output surface position.
 * @param dpos
 * array of 1st derivatives.
 * @param ddpos
 * array of 2nd derivatives.
 * @param side
 * array of flags for sided evaluation, -1 for below, 0 for don't care, 1 for above
 */
	virtual int evaluate(
                SPApar_pos&     uv,
                SPAposition&    pos,
                SPAvector*      dpos,       // first derivatives - array of
				                            // length 2 in order xu, xv
                SPAvector*      ddpos, // second derivatives - array of length 3 in order xuu, xuv, xvv
				int const* side);
								            

	virtual void eval_normal(
				SPApar_pos &uv,
				SPAunit_vector &norm
			) = 0;
/**
 * Finds the parameter position on the surface perpendicular to the given position outside of the surface.
 * <br><br>
 * @param in_point
 * point.
 */
	virtual SPApar_pos point_perp(SPAposition in_point) = 0;
/**
 * Finds the parameter position on the surface perpendicular to the given position outside of the surface, given a guess as to its value.
 * <br><br>
 * @param in_point
 * point.
 * @param in_par_pos
 * parameter position guess.
 */
	virtual SPApar_pos point_perp(SPAposition in_point, SPApar_pos in_par_pos) = 0;
/**
 * Returns the Gaussian curvature at the specified parameter.
 * <br><br>
 * @param in_par_pos
 * param.
 */
	virtual double eval_gaussian_curvature(const SPApar_pos& in_par_pos) const = 0;
/**
 * Returns the mean curvature at the specified parameter.
 * <br><br>
 * @param in_par_pos
 * param.
 */
	virtual double eval_mean_curvature(const SPApar_pos& in_par_pos) const = 0;
/**
 * Returns the maximum curvature at the specified parameter.
 * <br><br>
 * @param in_par_pos
 * param.
 */
	virtual double eval_max_curvature(const SPApar_pos& in_par_pos) const = 0;
/**
 * Returns the minimum curvature at the specified parameter.
 * <br><br>
 * @param in_par_pos
 * param.
 */
	virtual double eval_min_curvature(const SPApar_pos& in_par_pos) const = 0;
/**
 * Establishes the domain of a given term in the law.
 * <br><br>
 * @param which
 * term whose domain is sought.
 * @param answer
 * the interval bounding the specified term.
 */
	virtual logical term_domain(int which, SPAinterval& answer) = 0;
/**
 * Returns the parameter position <i>u, v</i> on the spline approximating surface.
 * <br><br>
 * @param in_par_pos
 * parameter position at which to evaluate the spline.
 */
	virtual SPAposition bs3_eval(const SPApar_pos& in_par_pos) const = 0;
};

/**
 * @nodoc
 */
inline base_surface_law_data::base_surface_law_data() : law_data() {}

/////////////////////////////////////////////////////////////
//
// The law_law_data class
//
/////////////////////////////////////////////////////////////
/**
 * Serves as a wrapper for a law object, for passing into unary and multiple law data classes.
 * <br>
 * <b>Role:</b> This is a law data class that holds a pointer to a law.
 * @see law
 */
class DECL_LAW law_law_data: public law_data
{
	law* data;
public:
/**
 * Applications are required to call this destructor for their law data types.
 */
	~law_law_data();

/**
 * @nodoc
 */
	virtual logical operator==(law_data const& rhs) const;

/**
 * Constructs a <tt>law_law_data</tt>.
 * <br><br>
 * <b>Role:</b> The constructor method for creating a <tt>law_data</tt> wrapper to a law.
 * <br><br>
 * @param in_data
 * pointer to sublaw.
 */
	law_law_data(law* in_data);
/**
 * @nodoc
 */
	void save();    // virtual method in base class
/**
 * @nodoc
 */
	const char* symbol(law_symbol_type type); // virtual method in base class
/**
 * @nodoc
 */
	char* string(law_symbol_type type, int& count, law_data_node*& ldn); // virtual method in base class
/**
 * @nodoc
 */
	law* data_law() {return data;}	 //Use with extreme caution!
/**
 * @nodoc
 */
	int date(); // virtual method in base class
/**
 * @nodoc
 */
	virtual law_data* deep_copy(base_pointer_map* pm = NULL) const; // virtual method in base class
/**
 * @nodoc
 */
	law_data* set_domain(SPAinterval* new_domain); // virtual method in base class
	// STI ROLL
/**
 * @nodoc
 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL
};


/////////////////////////////////////////////////////////////
//
// The base_transform_law_data class
//
/////////////////////////////////////////////////////////////
/**
 * Abstract base class for accessing <tt>transform_law_data</tt> with or without the ACIS kernel.
 * <br>
 * <b>Role:</b> This is a law data class that holds a pointer to a transform.
 * @see SPAtransf
 */
class DECL_LAW base_transform_law_data : public law_data
{
protected:
/**
 * Holds the data.
 */
	SPAtransf* data;
/**
 * Inverses transform.
 */
	SPAtransf* data_inverse;
public:
/**
 * Default destructor.
 */
	~base_transform_law_data();

/**
 * @nodoc
 */
	virtual logical operator==(law_data const& rhs) const;

/**
 * Constructs a <tt>base_transform_law_data</tt>, creating a <tt>transform_law_data</tt> that is a  wrapper for specified ACIS <tt>SPAtransf</tt> object.
 * <br><br>
 * @param in_data
 * transform to wrap.
 */
	base_transform_law_data(const SPAtransf* in_data);
/**
 * Transforms the specified position by the transform in <tt>base_transform_law_data</tt>.
 * <br><br>
 * @param p
 * position.
 */
	SPAposition transform(SPAposition p);
/**
 * Performs a rotation transformation.
 * <br><br>
 * @param v
 * rotation vector.
 */
	SPAvector rotate(SPAvector v);
/**
 * Performs an inverse transform on the specified position by the transform in <tt>base_transform_law_data</tt>.
 * <br><br>
 * @param p
 * position.
 */
	SPAposition transform_inverse(SPAposition p);
/**
 * Performs an inverse rotation transformation.
 * <br><br>
 * @param v
 * rotation vector.
 */
	SPAvector rotate_inverse(SPAvector v);
/**
 * Returns the transform wrapped by this <tt>transform_law_data</tt>.
 */
	SPAtransf* get_trans() { return data;}	//Use with extreme caution!
/**
 * @nodoc
 */
	virtual law_data* deep_copy(base_pointer_map* pm = NULL) const;// virtual method in base class
/**
 * Returns a pointer to a law of this type.
 * <br><br>
 * <b>Role:</b> Used by parsing to create an instance of this law.
 * <br><br>
 * @param in_data
 * array of transforms.
 */
	virtual base_transform_law_data* make_one(const SPAtransf* in_data) const = 0;

/**
 * @nodoc
 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
};

///////////////////////////////////////////////////////////////////////////////
//
// The base_position_array_law_data class
//
// data[x+y*dims[0]+z*dim[0]*dim[1]] is the (x,y,z) point in the array
//
///////////////////////////////////////////////////////////////////////////////
/**
 * @nodoc
 */
class DECL_LAW base_position_array_law_data : public law_data
{
protected:
	SPAposition*    data;
	int*            dims;
	int             dimension;

	void calc_movement_ratios(
            int*    selection_list,
            int     selection_list_size,
            double  gravity_falloff,
            double* movement_ratios);

	double Distance(int i, int j);

public:
	~base_position_array_law_data();


/**
 * @nodoc
 */
	virtual logical operator==(law_data const& rhs) const;

	base_position_array_law_data(
            const SPAposition*  in_data,
            const int*          dim_size,
            int                 dimension);

//	char const *symbol(law_symbol_type type);
	SPAposition* get_data(); //Use with extreme caution!
	const int* get_dims();
	int get_dimension();

	void index_to_xyz(int index, int& x, int& y, int& z);
	void update_point(
            int* selection_list,  //Use with extreme caution!
            int selection_list_size,
            const SPAvector& delta_vector,
            double gravity_falloff);
	void reset_point(int which_point);  //Use with extreme caution!
	int get_data_size();
	virtual law_data* deep_copy(base_pointer_map* pm = NULL) const;
	virtual base_position_array_law_data* make_one(
            const SPAposition* in_data,
            const int* dim_size,
            int dimension) const = 0;

	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL
};

///////////////////////////////////////////
//
//  A class for making arrays of law_data's
//
///////////////////////////////////////////
/**
 * @nodoc
 */
class DECL_LAW law_data_node : public ACIS_OBJECT
{
public:
	law_data*       data;
    law_data_node*  next;
	//.//law_data_node(law_data *in_data,law_data_node *in_next);
    law_data_node(law_data* in_data, law_data_node* in_next, int& count);
	~law_data_node();
};

/**
 * @nodoc
 */
law_data** law_data_array(int* size, int& count, law_data_node*& ldn);
//.//void law_data_list_on();
//.//void law_data_list_off();
//.//int get_law_data_list_num();

// ********************************************************************
// Class:
//    base_pattern_law_data
//
// Access:
//    do not document
// ********************************************************************
/**
 * @nodoc
 */
class DECL_LAW base_pattern_law_data : public law_data
{
public:
    virtual SPAtransf eval(const double* coords) = 0;
	virtual logical term_domain(int which, SPAinterval& answer) = 0;
    virtual int take_dim() const = 0;
    virtual int scale_dim() const = 0;
};

/** @} */
#endif
