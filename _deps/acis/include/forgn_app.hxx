// $Id: forgn_app.hxx,v 1.3 2002/08/26 14:50:43 goetting Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
 
// Global function pointer for a call back to the absf surface approximator

#if !defined( FOREIGN_APPROXIMATOR_HEADER )
#define FOREIGN_APPROXIMATOR_HEADER

#include "dcl_kern.h"
#include "err_info.hxx"

class surface;
class option_header;
//class bs3_surface;
#include "bs3surf.hxx"
typedef bs3_surface (*foreign_surface_approx) (surface*, double, double&, int, double*, int, double*);
#ifdef THREAD_SAFE_ACIS
extern safe_function_type<foreign_surface_approx> foreign_approximator;
#else
extern foreign_surface_approx foreign_approximator;
#endif
extern DECL_KERN void set_foreign_approximator_function(foreign_surface_approx _foreign_approximator);
extern DECL_KERN option_header replace_procedural_surface;
extern DECL_KERN option_header use_cgm_approx;
extern DECL_KERN option_header debug_approx_info;

// do not doc
DECL_KERN bs3_surface generate_foreign_approximation(surface *surf, double tol, double& keep_tol, 
                                           int num_u = 0, double* u_knots = NULL,
                                           int num_v = 0, double* v_knots = NULL);


#endif	
