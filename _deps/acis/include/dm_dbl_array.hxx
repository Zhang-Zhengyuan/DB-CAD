/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//
#ifndef DM_DBL_ARRAY_H
#define DM_DBL_ARRAY_H
// Header for the DM_dbl_array class.
//
// Purpose: The DM_dbl_array class is a pseudo-read-only container class for
// double type supporting Deformable modeling geometry query methods.
//
// Description: Some deformable modeling geometry queries return a DM_dbl_array populated with
// doubles. Thus, clients needn't allocate or destroy heap memory. Queries to curve-based
// tag object loads and constraints are supported. The DM_dbl_array can also be used
// as an array, using the DM_set_array_size method and the DM_dbl_array::Set_elem method.
//
// Related methods: A DM_dbl_array is returned by the DM_eval_crv_src_domain, DM_eval_crv_tgt,
// ATTRIB_DM2ACIS::Eval_crv_src, and ATTRIB_DM2ACIS::Eval_crv_tgt methods.
// Note that methods taking a const double* argument can be passed a DM_dbl_array - no
// user casting is necessary, due to the member const double* cast operator.
//
// Related classes: DM_dbl_array access or set methods taking a Spatial_error_hurler will
// check array boundaries.
//
class Spatial_abs_hurler;

#include "dsblock.hxx"
#include "mmgr.hxx" // class ACIS_OBJECT

#include "dcl_ds.h"

/**
* @file dm_dbl_array.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup DMAPI
 *
 * @{
 */

/**
 * This class is a pseudo-read-only container class for double type supporting deformable modeling geometry query methods.
 * <br><br>
 * <b>Role</b>:Some deformable modeling geometry queries return a <tt>DM_dbl_array</tt> populated with
 * doubles. Thus, clients needn't allocate or destroy heap memory. Queries to curve-based tag
 * object loads and constraints are supported. The <tt>DM_dbl_array</tt> can also be used as an expandable
 * array, using the DM_set_array_size method and the <tt>DM_dbl_array</tt>::<tt>Set_elem method</tt>.
 * <br><br>
 * <b>Related methods:</b> A <tt>DM_dbl_array</tt> is returned by the <tt>DM_eval_crv_src</tt>, <tt>DM_eval_crv_tgt</tt>,
 * <tt>ATTRIB_DM2ACIS</tt>::<tt>Eval_crv_src</tt>, and <tt>ATTRIB_DM2ACIS</tt>::<tt>Eval_crv_tgt</tt> methods. Cached data (array
 * elements) can be reset with <tt>Set_elem methods</tt>, and can be resized with <tt>DM_set_array_size</tt>.
 * <br><br>
 * <b>Related classes:</b> <tt>DM_dbl_array</tt> access or set methods taking a <tt>Spatial_error_hurler</tt> will
 * check array boundaries.
 * @see DM_cct_icon, DM_cld_icon, DM_crv_icon, DM_dbx_icon, DM_grd_icon, DM_lct_icon, DM_lld_icon, DM_sps_icon
 */
class DECL_DM DM_dbl_array : public ACIS_OBJECT {
  private:
    friend class DM_dbl_array_initializer;
    DS_dbl_block m_block;
// Users should not call assignment operator or copy constructor
    DM_dbl_array&
    operator=                              // assignment Operator
    (const DM_dbl_array&);
    DM_dbl_array(const DM_dbl_array&);     // copy constructor
  public:

/**
 * Default constructor.
 */

    DM_dbl_array();                                           // public constructor
/**
 * Public constructor with non-zero size.<br><br>
 * @param nelems
 * number of elements in array.
 */
    DM_dbl_array(int nelems);                                 // public constructor with non-zero size
/**
 * Default destructor.
 */
    ~DM_dbl_array();                                          // public destructor
/**
 * Access array element with no bounds checking.<br><br>
 * @param index
 * index.
 */
   	double operator[](int index) const;                             // access array element NO bounds checking
/**
 * Access array element with bounds checking.<br><br>
 * @param err_handler
 * error handler.
 * @param index
 * index.
 */
  	double Get_elem(Spatial_abs_hurler& err_handler,int index) const;           // access array element with bounds checking
/**
 * Array size == number of elements.
 */
 	int Size()const;                                          // array size == number of elements

// tbrv
/**
 * Cast to read-only double.
 */
    operator const double* ()const;                           // cast to read-only double*
/**
 * Sets one array element to the specified value with bounds checking.<br><br>
 * @param err_handler
 * error handler.
 * @param index
 * element to be set.
 * @param val
 * value element will be set to.
 */

 	void Set_elem(Spatial_abs_hurler& err_handler,int index,double val);            // set array element with bounds checking
/**
 * Sets one array element to the specified value with no bounds checking.<br><br>
 * @param index
 * element to be set.
 * @param val
 * value element will be set to.
 */
    void Set_elem(int index,double val);                                // set array element NO bounds checking

/**
 * Sets a number of consecutive array elements to the specified values with no bounds checking.<br><br>
 * @param err_handler
 * error handler.
 * @param index
 * start index.
 * @param val
 * value elements will be set to.
 * @param nelems
 * number of elements to be set.
 */
 	void Set_elem(Spatial_abs_hurler& err_handler,int index,const double* val,int nelems); // set array elements with bounds checking
                                                              // last argument is how many consecutive elmts to set                                                             // second argument is starting index in DM_dbl_array
/**
 * Sets a number of consecutive array elements to the specified values with no bounds checking.<br><br>
 * @param index
 * start index.
 * @param val
 * value elements will be set to.
 * @param nelems
 * number of elements to be set.
 */
     void Set_elem(int index ,const double* val,int nelems);                     // set array elements NO bounds checking
                                                              // last argument is how many consecutive elmts to set
                                                              // second argument is starting index in DM_dbl_array
};


/** @} */

#endif //DM_DBL_ARRAY_H
