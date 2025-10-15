/* ORIGINAL: 3dt2.1/geomhusk/splsurf.hxx */
// $Id: splsurf.hxx,v 1.10 2002/08/09 17:21:36 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//---------------------------------------------------------------------
// purpose---
//    define a spline surface class
//
//---------------------------------------------------------------------

#ifndef splsurf_hxx
#define splsurf_hxx

//=====================================================================

#include "dcl_cstr.h"
#include "logical.h"

#include "position.hxx"

/**
* @file splsurf.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

//======================================================================
// Not found anywhere else in ACIS.
/**
 * @nodoc
 */
typedef struct tag_param_data {
    int  degree;             // polynomial degree
    int  rational;           // rational/nonrational flag
    int  form;               // open (0), closed(1), or periodic(2)
    int  pole;               // type of surface singularities
    int  num_ctrlpts;        // count of control points
    } param_data;

// Not found anywhere else in ACIS.
/**
 * @nodoc
 */
typedef struct tag_knots {
    int     count;
    double  *array;
    } knots;

/**
 * Defines a data structure used to construct a B-spline surface.
 * <br>
 * <b>Role:</b> The <tt>splsurf</tt> class contains data structures that allow the creation of
 * a B-spline surface using control points, knot vectors, and weights. The contents of this class 
 * closely resemble the internal data structure used by ACIS for rational and nonrational, nonuniform
 * B-spline surfaces. 
 * <br><br>
 * The two-dimensional grid of control points is contained in a one-dimensional array in which 
 * the <i>v</i> index varies first. That is, a row of <i>v</i> control points is specified 
 * for the first <i>u</i> value. Then, the row of <i>v</i> control points for the next 
 * <i>u</i> value. If the surface is rational, its weights are ordered the same as its control points
 * and the weight values must be positive.
 * <br><br>
 * The knot vector in <i>u</i> (and <i>v</i>) should form a non-negative,
 * non-decreasing sequence with any knot value appearing at most degree <i>u</i>
 * (degree <i>v</i>) times. In ACIS all B-spline surfaces interpolate their boundary control points. 
 * To achieve this, the end knot multiplicities are either (a) equal to the degree, or (b) equal to the degree 
 * plus one. If the end knot multiplicities are equal to the degree, then the number of knots should be equal 
 * to the number of control points plus the degree minus one <tt>num_crtlpts + degree - 1</tt>. If the end knot 
 * multiplicities are equal to the degree plus one, then the number of knots should be equal to the number of 
 * control points plus the degree plus one <tt>num_crtlpts + degree + 1</tt>. 
 * No assumption is made about the relationship between parameter values and object space distances; 
 * however, it is advantageous for the parameterization to be as homogeneous as possible. 
 * (That is, it is advantageous for the parameterization to be somewhat proportional to arc length.)
 * In addition, it is advantageous for the parameterization in the two directions be of similar scale.
 * ACIS also prefers that surfaces be G<sup>2</sup> continuous; therefore, it may be advantageous to
 * limit the knot multiplicities of interior knots to being less than <tt>degree - 2</tt>.
 * <br><br>
 * The degree in <i>u</i> (and <i>v</i>) must be specified as a positive integer. <br>
 * The form of the surface in <i>u</i> (or <i>v</i>) is specfied as follows: <br>
 * 0 => open,  <br>
 * 1 => closed,  <br>
 * 2 => periodic.  <br>
 * Singularities on the surface in the <i>u</i> (or <i>v</i>) direction are specified as follows: <br>
 * 0 => no singularity at <i>u</i>-minimum or <i>u</i>-maximum boundary, <br>
 * 1 => has a singularity at the <i>u</i>-minimum boundary, <br>
 * 2 => has a singularity at the <i>u</i>-maximum boundary, <br>
 * 3 => has a singularity at both boundaries. 
 * <br><br>
 * Row and column indexing are 0-based for all functions. <br>
 * @see splgrid
 */
class DECL_CSTR splsurf : public ACIS_OBJECT {
    private:

    param_data  u_pd;        // surface data u
    param_data  v_pd;        // surface data v

    SPAposition *ctrlpts;    // array of control points
    double   *weights;       // array of weights - rational surfaces only

    knots    u_knots;        // knots u
    knots    v_knots;        // knots v

    public:

    // constructor
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 */
    splsurf();
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param surf
 * original splsurf.
 */
    splsurf(splsurf& surf);  // copy constructor

    // destructor
// tbrv
/**
 * @nodoc
 */
    ~splsurf();

	// print data
/**
 * Prints the data for the <tt>splsurf</tt>.
 */
	void print();

	// control points
/**
 * Returns <tt>TRUE</tt> if the pointer to the control points array has been set; otherwise, returns <tt>FALSE</tt>.
 */
    logical is_ctrlpts() const { return ctrlpts ? TRUE : FALSE; }
/**
 * Returns <tt>TRUE</tt> if the pointer to the weights array has been set; otherwise, it returns <tt>FALSE</tt>.
 */
    logical is_weights() const { return weights ? TRUE : FALSE; }
/**
 * Returns a pointer to the control points array.
 */
    const SPAposition *ctrlpt_array() const { return ctrlpts; }
/**
 * Returns a pointer to a weights array.
 */
 	const double   *weight_array() const { return weights; }
/**
 * Sets a pointer to an array of positions that are control points for a B-spline surface.
 * <br>
 * <b>Role:</b> Sets a pointer to an array of <tt>SPAposition</tt>s constructed on the heap. The array will be deleted
 * when the <tt>splsurf</tt> is destructed.
 * <br><br>
 * @param points
 * array of control points.
 * @param rows_u
 * number of rows (number of control points in the <i>u</i> direction).
 * @param cols_v
 * number of columns (number of control points in the <i>v</i> direction).
 */
    void set_ctrlpt_array(SPAposition* points, int rows_u, int cols_v);
/**
 * Sets a pointer to an array of doubles that are weights for the control points of a rational spline surface.
 * <br>
 * <b>Role:</b> Sets a pointer to an array of doubles constructed on the heap. The array will be deleted
 * when the <tt>splsurf</tt> is destructed. The weights must be specified in the same order as 
 * control points. Weights are specified for rational surfaces only. 
 * <br><br>
 * @param wts
 * array of weights.
 */
    void set_weight_array(double* wts);

    //   row-column indexing is always zero-based
/**
 * Changes one of the elements in the array of control points.
 * <br><br>
 * @param row_u
 * row index (index in the <i>u</i> direction).
 * @param col_v
 * column index (index in the <i>v</i> direction).
 * @param new_pos
 * new control point position.
 */
    void set_ctrlpt_item(int row_u, int col_v, SPAposition& new_pos)
        { *(ctrlpts + (v_pd.num_ctrlpts * row_u) + col_v) = new_pos; }
/**
 * Returns one of the elements from the array of control points.
 * <br><br>
 * @param row_u
 * row index (index in the <i>u</i> direction).
 * @param col_v
 * column index (index in the <i>v</i> direction).
 */
    SPAposition ctrlpt_item(int row_u, int col_v) const
        { return *(ctrlpts + (v_pd.num_ctrlpts * row_u) + col_v); }
/**
 * Changes one of the elements in the array of weights.
 * <br><br>
 * @param row_u
 * row index (index in the <i>u</i> direction).
 * @param col_v
 * column index (index in the <i>v</i> direction).
 * @param new_wt
 * new weight value.
 */
    void set_weight_item(int row_u, int col_v, double& new_wt)
        { *(weights + (v_pd.num_ctrlpts * row_u) + col_v) = new_wt; }
/**
 * Returns one of the elements from the array of weights.
 * <br><br>
 * @param row_u
 * row index (index in the <i>u</i> direction).
 * @param col_v
 * column index (index in the <i>v</i> direction).
 */
    double weight_item(int row_u, int col_v) const
        { return *(weights + (v_pd.num_ctrlpts * row_u) + col_v); }
/**
 * Returns the total number of points in the control points array.
 */
    int ctrlpt_count() const { return u_pd.num_ctrlpts * v_pd.num_ctrlpts; }
/**
 * Returns the number of rows in the control points array; that is, the number of control points in the <i>u</i> direction.
 */
	int ctrlpt_rows()  const { return u_pd.num_ctrlpts; }
/**
 * Returns the number of columns in the control points array; that is, the number of control points in the <i>v</i> direction.
 */
    int ctrlpt_cols()  const { return v_pd.num_ctrlpts; }

    // uv parameters
/**
 * Returns the polynomial degree in the <i>u</i> direction.
 */
 	int u_param_degree()   const { return u_pd.degree; }
/**
 * Returns if the surface is weighted (rational or nonrational) for each control point for the <i>u</i> direction.
 */
    int u_param_rational() const { return u_pd.rational; }
/**
 * Returns the form of the surface in the <i>u</i> direction.
 * <br><br>
 * <b>Role:</b> This method returns 0 (open), 1 (closed), or 2 (periodic).
 */
    int u_param_form()     const { return u_pd.form; }
/**
 * Returns pole singularities of the surface in the <i>u</i> direction.
 * <br><br>
 * <b>Role:</b> This method returns 0 (no singularity at the minimum or maximum boundary),
 * 1 (singularity at the minimum boundary), 2 (singularity at the maximum boundary), or
 * 3 (singularity at both boundaries).
 */
    int u_param_pole()     const { return u_pd.pole; }
/**
 * Returns the number of control points in the <i>u</i> direction.
 */
    int u_param_ctrlpts()  const { return u_pd.num_ctrlpts; }
/**
 * Returns the polynomial degree in the <i>v</i> direction.
 */
    int v_param_degree()   const { return v_pd.degree; }
/**
 * Returns if the surface is weighted (rational or nonrational) for each control point in the <i>v</i> direction.
 */
    int v_param_rational() const { return v_pd.rational; }
/**
 * Returns the form of the surface in the <i>v</i> direction.
 * <br><br>
 * <b>Role:</b> This method returns 0 (open), 1 (closed), or 2 (periodic).
 */
    int v_param_form()     const { return v_pd.form; }
/**
 * Returns pole singularities of the surface in the <i>v</i> direction.
 * <br><br>
 * <b>Role:</b> This method returns 0 (no singularity at the minimum or maximum boundary),
 * 1 (singularity at the minimum boundary), 2 (singularity at the maximum boundary), or
 * 3 (singularity at both boundaries).
 */
    int v_param_pole()     const { return v_pd.pole; }
/**
 * Returns the number of control points in the <i>v</i> direction.
 */
    int v_param_ctrlpts()  const { return v_pd.num_ctrlpts; }
/**
 * Changes the surface parameters in the <i>u</i> direction.
 * <br><br>
 * @param degree
 * degree in <i>u</i> direction.
 * @param rational
 * rational or nonrational in <i>u</i> direction.
 * @param form
 * surface form in <i>u</i> direction.
 * @param pole
 * singularies in <i>u</i> direction.
 */
    void set_u_param(int degree, int rational, int form, int pole);
/**
 * Changes the surface parameters in the <i>v</i> direction.
 * <br><br>
 * @param degree
 * degree in <i>v</i> direction.
 * @param rational
 * rational or nonrational in <i>v</i> direction.
 * @param form
 * surface form in <i>v</i> direction.
 * @param pole
 * singularies in <i>v</i> direction.
 */
    void set_v_param(int degree, int rational, int form, int pole);

    // knots
/**
 * Returns a pointer to an array of knots for the <i>u</i>-parameter.
 */
 	const double *u_knot_array() const { return u_knots.array; }
/**
 * Returns a pointer to an array of knots for the <i>v</i>-parameter.
 */
    const double *v_knot_array() const { return v_knots.array; }
/**
 * Sets a pointer to an array of knots for the <i>u</i>-parameter.
 * <br>
 * <b>Role:</b> Sets a pointer to an array of doubles constructed on the heap. 
 * The array will be deleted when the <tt>splsurf</tt> is destructed.
 * <br><br>
 * @param array
 * array of knots.
 * @param count
 * number of knots in array.
 */
    void set_u_knot_array(double* array, int count);
/**
 * Sets a pointer to an array of knots for the <i>v</i>-parameter.
 * <br>
 * <b>Role:</b> Sets a pointer to an array of doubles constructed on the heap. 
 * The array will be deleted when the <tt>splsurf</tt> is destructed.
 * <br><br>
 * @param array
 * array of knots.
 * @param count
 * number of knots in array.
 */
    void set_v_knot_array(double* array, int count);
/**
 * Returns one of the elements from the array of knots for the <i>u</i>-parameter.
 * <br><br>
 * @param index
 * index into <i>u</i> knot array.
 */
    double u_knot_item(int index) const
        { return *(u_knots.array + index); }
/**
 * Changes one of the elements in the array of knots for the <i>u</i>-parameter.
 * <br><br>
 * @param index
 * index into <i>u</i> knot array.
 * @param new_knot
 * new <i>u</i> knot value.
 */
    void set_u_knot_item(int index, double &new_knot)
        { *(u_knots.array + index) = new_knot; }
/**
 * Returns one of the elements from the array of knots for the <i>v</i>-parameter.
 * <br><br>
 * @param index
 * index into <i>v</i> knot array.
 */
    double v_knot_item(int index) const
        { return *(v_knots.array + index); }
/**
 * Changes one of the elements in the array of knots for the <i>v</i>-parameter.
 * <br><br>
 * @param index
 * index into <i>v</i> knot array.
 * @param new_knot
 * new <i>v</i> knot value.
 */
    void set_v_knot_item(int index, double &new_knot)
        { *(v_knots.array + index) = new_knot; }
/**
 * Returns the number of knots in the array of knots for the <i>u</i>-parameter.
 */
    int u_knot_count() const { return u_knots.count; }
/**
 * Returns the number of knots in the array of knots for the <i>v</i>-parameter.
 */
    int v_knot_count() const { return v_knots.count; }
};

#endif

//----------------------------------
//  End of splsurf.hxx
//----------------------------------
