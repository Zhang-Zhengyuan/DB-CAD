/* ORIGINAL: 3dt2.1/geomhusk/splgrid.hxx */
// $Id: splgrid.hxx,v 1.10 2002/08/09 17:21:36 jeff Exp $
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
//    define a spline surface grid class
//
//---------------------------------------------------------------------

#ifndef splgrid_hxx
#define splgrid_hxx

//=====================================================================

#include "dcl_cstr.h"
#include "logical.h"
#include "position.hxx"
#include "unitvec.hxx"

/**
* @file splgrid.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */


//=====================================================================

/**
 * Defines a data structure for the construction of a B-spline surface.
 * <br>
 * <b>Role:</b> A <tt>splgrid</tt> class contains data structures that allow the creation of a B-spline 
 * surface that either interpolates a grid of points or approximates a grid of points to a defined tolerance.
 * The <tt>splgrid</tt> contains data including a grid of points and boundary tangent vectors. 
 * The grid points are ordered in the array such that the <i>u</i> index varies first.
 * That is, a row of <i>u</i> points is specified for the first <i>v</i> value. 
 * Then a row of <i>u</i> points for the next <i>v</i> value. And so on...
 * <br><br>
 * If the <tt>splgrid</tt> is used to construct a surface that <i>interpolates</i> the grid of points,
 * unit tangent vectors can be specified for the boundary conditions for <i>u</i> and <i>v</i> at the 
 * start and the end of the surface. Either all boundary conditions must be specified or all must be 
 * <tt>NULL</tt>. If the <tt>splgrid</tt> is used to construct a surface that <i>approximates</i> the 
 * grid of points, unit tangent vectors can be specified for the boundary conditions for <i>u</i> at 
 * the start and end of the surface. Unit tangent vectors specified for the start or end <i>v</i> 
 * boundaries are ignored.
 * <br><br>
 * Note: If a <tt>splgrid</tt> object is to be used with @href api_face_spl_apprx, the order of the points
 * in the array must be such that the <i>v</i> index varies first.
 * That is, a row of <i>v</i> points is specified for the first <i>u</i> value. 
 * Then a row of <i>v</i> points for the next <i>u</i> value. And so on...
 * This order will affect the behavior of the member functions @href gridpts_item and @href set_gridpts_item.
 * @see SPAposition, SPAunit_vector, splsurf
 */
class DECL_CSTR splgrid : public ACIS_OBJECT {
    private:

    int  num_u_pts;               // number of columns of grid points
    int  num_v_pts;               // number of rows of grid points

    SPAposition  *gridpts;        // array of positions on the surface
    double    fit_tol;            // a point fit tolerance used when
                                  // approximating the spline surface

    SPAunit_vector  *u_tanvec_start;  // array of tangent vectors defining
    SPAunit_vector  *u_tanvec_end;    // the boundary conditions for u and
    SPAunit_vector  *v_tanvec_start;  // v at the start and end
    SPAunit_vector  *v_tanvec_end;

    public:

    // constructor
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 */
   	splgrid();
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param grid
 * original constructor.
 */
    splgrid(splgrid &grid);  // copy constructor

    // destructor
// tbrv
/**
 * @nodoc
 */
    ~splgrid();

	// print data
/**
 * Prints the data for <tt>splgrid</tt>.
 */
	void print();

	// grid points
/**
 * Returns <tt>TRUE</tt> if the pointer to the grid points array has been set; otherwise, returns <tt>FALSE</tt>.
 */
    logical is_gridpts() const { return gridpts ? TRUE : FALSE; }
/**
 * Returns a pointer to the grid points array.
 */
    const SPAposition *gridpts_array() const { return gridpts; }
/**
 * Sets a pointer to an array of positions that form a grid of points on a spline surface.
 * <br>
 * <b>Role:</b> Sets a pointer to an array of <tt>SPAposition</tt>s constructed on the heap. The array will be deleted
 * when the <tt>splgrid</tt> is destructed.
 * <br><br>
 * @param points
 * grid points array.
 * @param rows_v
 * number of rows.
 * @param cols_u
 * number of columns.
 */
    void set_gridpts_array(SPAposition *points, int rows_v, int cols_u);
/**
 * Returns one of the elements in the array of grid points.
 * <br><br>
 * @param row
 * row index.
 * @param col
 * column index.
 */
    SPAposition gridpts_item(int row, int col) const
        { return *(gridpts + (num_u_pts * row) + col); }
/**
 * Changes one of the elements in the array of grid points.
 * <br><br>
 * @param row
 * row index.
 * @param col
 * column index.
 * @param new_pos
 * new position.
 */
    void set_gridpts_item(int row, int col, SPAposition &new_pos)
        { *(gridpts + (num_u_pts * row) + col) = new_pos; }
/**
 * Returns the number of grid points in each row of the grid points array.
 */
    int u_points() const { return num_u_pts; }
/**
 * Returns the number of grid points in each column of the grid points array.
 */
    int v_points() const { return num_v_pts; }
/**
 * Sets the surface fit tolerance.
 * <br><br>
 * @param tol
 * surface fit tolerance.
 */
    void set_tol(double tol) { fit_tol = tol; }
/**
 * Returns the surface fit tolerance.
 */
    double tol() const { return fit_tol; }

    // tangent vectors - u
/**
 * Returns <tt>TRUE</tt> if the pointer to the <i>u</i> starting tangent vector array has been set; otherwise, returns <tt>FALSE</tt>.
 */
    logical is_u_start() const { return u_tanvec_start ? TRUE : FALSE; }
/**
 * Returns <tt>TRUE</tt> if the pointer to the <i>u</i> ending tangent vector array has been set; otherwise, returns <tt>FALSE</tt>.
 */
	logical is_u_end()   const { return u_tanvec_end   ? TRUE : FALSE; }
/**
 * Returns a pointer to an array of tangent vectors defining the starting boundary conditions in the <i>u</i> direction.
 */
	const SPAunit_vector *u_start_array() const { return u_tanvec_start; }
/**
 * Returns a pointer to an array of tangent vectors defining the ending boundary conditions in the <i>u</i> direction.
 */
	const SPAunit_vector *u_end_array() const { return u_tanvec_end; }
/**
 * Sets the array of tangent vectors defining the starting boundary conditions in the <i>u</i> direction.
 * <br>
 * <b>Role:</b> Sets a pointer to an array of <tt>SPAunit_vector</tt>s constructed on the heap. 
 * The array will be deleted when the <tt>splgrid</tt> is destructed.
 * <br><br>
 * @param tanvecs
 * array of tangent vectors.
 */
  	void set_u_start_array(SPAunit_vector *tanvecs);
/**
 * Sets the array of tangent vectors defining the ending boundary conditions in the <i>u</i> direction.
 * <br>
 * <b>Role:</b> Sets a pointer to an array of <tt>SPAunit_vector</tt>s constructed on the heap. 
 * The array will be deleted when the <tt>splgrid</tt> is destructed.
 * <br><br>
 * @param tanvecs
 * array of tangent vectors.
 */
	void set_u_end_array(SPAunit_vector *tanvecs);
/**
 * Returns one of the elements from the array of tangent vectors defining the starting boundary conditions in the <i>u</i> direction.
 * @param index
 * element index.
 */
    SPAunit_vector u_start_item(int index) const
        { return *(u_tanvec_start + index); }
/**
 * Changes one of the elements in the array of tangent vectors defining the starting boundary condition in the <i>u</i> direction.
 * <br><br>
 * @param index
 * element index.
 * @param new_vec
 * new tangent vector.
 */
    void set_u_start_item(int index, SPAunit_vector &new_vec)
        { *(u_tanvec_start + index) = new_vec; }
/**
 * Returns one of the elements from the array of tangent vectors defining the end boundary conditions in the <i>v</i> direction.
 * <br><br>
 * @param index
 * element index.
 */
    SPAunit_vector u_end_item(int index) const
        { return *(u_tanvec_end + index); }

/**
 * Changes one of the elements in the array of tangent vectors defining the ending boundary condition in the <i>v</i> direction.
 * <br><br>
 * @param index
 * element index.
 * @param new_vec
 * new tangent vector.
 */
    void set_u_end_item(int index, SPAunit_vector &new_vec)
        { *(u_tanvec_end + index) = new_vec; }

    // tangent vectors - v
/**
 * Returns <tt>TRUE</tt> if the pointer to the <i>v</i> starting tangent vector array has been set; otherwise, it returns <tt>FALSE</tt>.
 */
    logical is_v_start() const { return v_tanvec_start ? TRUE : FALSE; }
/**
 * Returns <tt>TRUE</tt> if the pointer to the <i>v</i> ending tangent vector array has been set; otherwise, it returns <tt>FALSE</tt>.
 */
	logical is_v_end()   const { return v_tanvec_end   ? TRUE : FALSE; }

/**
 * Returns a pointer to an array of tangent vectors defining the starting boundary conditions in the <i>v</i> direction.
 */
 	const SPAunit_vector *v_start_array() const { return v_tanvec_start; }
/**
 * Returns a pointer to an array of tangent vectors defining the ending boundary conditions in the <i>v</i> direction.
 */
    const SPAunit_vector *v_end_array() const { return v_tanvec_end; }
/**
 * Sets the array of tangent vectors defining the starting boundary conditions in the <i>v</i> direction.
 * <br>
 * <b>Role:</b> Sets a pointer to an array of <tt>SPAunit_vector</tt>s constructed on the heap. 
 * The array will be deleted when the <tt>splgrid</tt> is destructed.
 * <br><br>
 * @param tanvecs
 * array of tangent vectors.
 */
    void set_v_start_array(SPAunit_vector *tanvecs);
/**
 * Sets the array of tangent vectors defining the ending boundary conditions in the <i>v</i> direction.
 * <br>
 * <b>Role:</b> Sets a pointer to an array of <tt>SPAunit_vector</tt>s constructed on the heap. 
 * The array will be deleted when the <tt>splgrid</tt> is destructed.
 * <br><br>
 * @param tanvecs
 * array of tangent vectors.
 */
    void set_v_end_array(SPAunit_vector *tanvecs);
/**
 * Returns one of the elements from the array of tangent vectors defining the starting boundary conditions in the <i>v</i> direction.
 * <br><br>
 * @param index
 * element index.
 */
    SPAunit_vector v_start_item(int index) const
        { return *(v_tanvec_start + index); }
/**
 * Changes one of the elements in the array of tangent vectors defining the starting boundary condition in the <i>v</i> direction.
 * <br><br>
 * @param index
 * element index.
 * @param new_vec
 * new tangent vector.
 */
    void set_v_start_item(int index, SPAunit_vector &new_vec)
        { *(v_tanvec_start + index) = new_vec; }
/**
 * Returns one of the elements from the array of tangent vectors defining the end boundary conditions in the <i>v</i> direction.
 * <br><br>
 * @param index
 * element index.
 */
    SPAunit_vector v_end_item(int index) const
        { return *(v_tanvec_end + index); }
/**
 * Changes one of the elements in the array of tangent vectors defining the ending boundary condition in the <i>v</i> direction.
 * <br><br>
 * @param index
 * element index.
 * @param new_vec
 * array of tangent vectors.
 */
    void set_v_end_item(int index, SPAunit_vector &new_vec)
        { *(v_tanvec_end + index) = new_vec; }
};

#endif

//----------------------------------
//  End of splgrid.hxx
//----------------------------------
