/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef API_CURVE_APPROX_HXX
#define API_CURVE_APPROX_HXX

#include "dcl_kern.h"
#include "bs2curve.hxx"
#include "bs3curve.hxx"
class outcome;
class COEDGE;
class EDGE;
class SPA_COEDGE_approx_options;
class SPA_EDGE_approx_options;
class AcisOptions;

/**
* @file api_curve_approx.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BSGEOMETRY
 *
 * @{
 */


/**
 * Provides a uniform approach for accessing various curve approximation routines. 
 * <br><br> 
 * <b>Role:</b> Computes a <tt>bs3_curve</tt> approximation to the curve underlying the
 * incoming EDGE.
 * <br><br> 
 * <b>Errors: </b> None.
 * <br><br> 
 * <b>Effect: </b> Creates a new <tt>bs3_curve</tt>. The model is unchanged. 
 * <br><br>
 * <b>Journal: </b> Available
 * <br> 
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param edge_to_approx
 * Specifies the edge whose %curve is being approximated (required).
 *
 * @param approx_spline_result 
 * Returns the computed approximation (required).
 * 
 * @param approx_opts 
 * The characteristics of the approximation are controlled by this optional parameter. 
 * The default option values are: <br><tt>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; requested_tol &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; = SPAresfit<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; param_mode &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; = SPA_approx_param_mode_BASE_PARAMETERIZATION <br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; domain &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; = unspecified <br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; degree &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; = SPA_approx_degree_CUBIC </tt><br>
 * <br>
 * Also contained is information about the fit (CHECK AFTER THE API EXECUTES) <br><tt>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; get_actual_tol() </tt>
 *
 * @param ao 
 * Specifies ACIS options such as versioning and journaling (optional).
 * @see SPA_approx_options.hxx, acis_options.hxx 
 **/
DECL_KERN outcome api_approx(EDGE    const           *edge_to_approx,
                             bs3_curve               &approx_spline_result,
                             SPA_EDGE_approx_options *approx_opts = NULL,
                             AcisOptions             *ao          = NULL);

/**
 * Provides a uniform approach for accessing various parameter space (2D) curve 
 * approximation routines.  
 * <br><br> 
 * <b>Role:</b> Computes a <tt>bs2_curve</tt> approximation to the domain curve lying on the FACE 
 * associated with the LOOP of which this COEDGE is an element. 
 * The actual tolerance of the approximation is the distance between the approximating curve and the 
 * projection of the edge geometery onto the surface bounded by the coedge.
 * <br><br> 
 * <b>Errors: </b> None.  
 * <br><br>
 * <b>Effect: </b> Creates a new <tt>bs2_curve</tt>. The model is unchanged. 
 * <br><br>
 * <b>Journal: </b> Available 
 * <br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param coedge_to_approx
 * Specifies the coedge whose parameter-space %curve is being approximated (required).
 *
 * @param approx_2D_spline_result 
 * Returns the computed approximation (required). <br>
 * To access the %surface associated with the approximation use: <br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <tt>coedge_to_approx->loop()->face()->geometry()->equation() </tt>
 * 
 * @param approx_opts 
 * Characteristics of the approximation are controlled by this optional parameter. 
 * The default option values are: <br><tt>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; requested_tol &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; = SPAresfit<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; param_mode &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; = SPA_approx_param_mode_BASE_PARAMETERIZATION <br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; domain &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; = unspecified <br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; degree &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; = SPA_approx_degree_CUBIC </tt><br>
 * <br>
 * Also contained is information about the fit (CHECK AFTER THE API EXECUTES) <br><tt>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; get_actual_tol() </tt>
 *
 * @param ao 
 * Specifies ACIS options such as versioning and journaling (optional).
 * @see SPA_approx_options.hxx, acis_options.hxx 
 **/
DECL_KERN outcome api_approx(COEDGE const              *coedge_to_approx,
                             bs2_curve                 &approx_2D_spline_result,
                             SPA_COEDGE_approx_options *approx_opts = NULL,
                             AcisOptions               *ao          = NULL);


/** @} */
#endif // API_CURVE_APPROX_HXX
