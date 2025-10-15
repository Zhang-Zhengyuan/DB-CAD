/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef BHL_OPT__HXX
#define BHL_OPT__HXX

#include <stdio.h>

#include "body.hxx"

#include "dcl_heal.h"
#include "bhl_stru.hxx"
#include "bhl_opt2.hxx"

// Callback functions for individual modules
DECL_HEAL void simplify_callback_function(BODY*);
DECL_HEAL void geombuild_callback_function(BODY*);

// STI cpp begim
//DECL_HEAL void display_callback_function(ENTITY*, int, logical);
// STI cpp end


// Typedefs for function-types used as callback functions for setting
// options and tolerances for various modeles.

typedef void (*bhl_simgeom_opt_callbk) (bhl_anal_simgeom_results*);

typedef void (*bhl_geombuild_opt_callbk) (bhl_anal_geometry_results*);

// STI cpp begin
// typedef void (*bhl_display_opt_callbk) (ENTITY*, int, logical);
// STI cpp end


// The options and tolerances for geometry simplification module
// being performed during auto-heal can be set by the user using
// a callback function. Body-Healer will call this function giving
// the recommended values for all the options and tolerances in a
// structure. If the user desires to change any option value, then
// he should change that particular member of the structure.
// Following are the functions to get and set the function pointer
// for this purpose.
DECL_HEAL
bhl_simgeom_opt_callbk bhl_get_simgeom_options_callback ();

DECL_HEAL
void bhl_set_simgeom_options_callback (bhl_simgeom_opt_callbk);


// The options and tolerances for geometry building module
// being performed during auto-heal can be set by the user using
// a callback function. Body-Healer will call this function giving
// the recommended values for all the options and tolerances in a
// structure. If the user desires to change any option value, then
// he should change that particular member of the structure.
// Following are the functions to get and set the function pointer
// for this purpose.
DECL_HEAL
bhl_geombuild_opt_callbk bhl_get_geombuild_options_callback ();

DECL_HEAL
void bhl_set_geombuild_options_callback (bhl_geombuild_opt_callbk);

// STI cpp begin
DECL_HEAL
//void bhl_set_display_options_callback (bhl_display_opt_callbk);
// STI cpp end

// The following functions are called during autoheal and surface-heal
// to set the options and tolerances of the respective modules.
void
bhl_simgeom_options_callback (
				bhl_anal_simgeom_results* anal_simgeom_results);
void
bhl_geombuild_options_callback (
				bhl_anal_geometry_results* input_anal_geometry_results);


// All the following declarations are commented out because now all the
// options and tolerances can be set using callbacks.

//---------------------------------------------------------------------------
// All globals functions and variables are declared here
// These globals can be used by the application developer to enquire or set
// the options provided in this husk.
//---------------------------------------------------------------------------

//--------------- Options for Pre-Processing --------------------------------
//
// bhl_sim_geom_tol
/* This option sets the tolerance to be used by the body healer
*  during geometry simplification. This tolerance value must be
*  between 0 and 1. It governs the accuracy of spline to analytic
*  conversions.
*  Recommended Value : 	A low value enforces accurate conversion
*  whereas if a large value is specified [0.05, 1] then the splines
*  representing approx. analytic surfaces may also get converted.
*  Default Value : 0.01
*/
// bhl_get_sim_geom_tol() and bhl_set_sim_geom_tol() can be used to get
// and set the value for this option
DECL_HEAL double  bhl_get_sim_geom_tol( void);
DECL_HEAL void bhl_set_sim_geom_tol( double val);




//--------------- Options for Building Geometry ----------------------------
//
// bhl_geom_build
/* This option specifies whether geometry building should be performed or not
*  during autoheal
*/
DECL_HEAL logical bhl_get_geom_build( void);
DECL_HEAL void bhl_set_geom_build( logical );

// bhl_geom_build_tol
/* This option specifies the tolerance to be used by the Body Healer
*  during the geometry building operations. In most cases this value
*  set to the same value as stitch tolerance.
*  In the case of analytic bodies with tangential imperfections, it
*  is recommended that this tolerance value be set slightly higher
*  than the stitch tolerance since geometric transformations are
*  applied in the Tangency Analytic Solver.
*  Default Value : 0.01
*/
// bhl_get_geom_build_tol() and bhl_set_geom_build_tol() can be used to
// get and set values for this option
DECL_HEAL double bhl_get_geom_build_tol(void);
DECL_HEAL void  bhl_set_geom_build_tol( double val);

// bhl_anal_tgt
/* This option specifies whether analytic solver should be performed or not
*  during autoheal
*/
DECL_HEAL logical bhl_get_anal_tgt( void);
DECL_HEAL void bhl_set_anal_tgt( logical );

// bhl_anal_tgt_tol
/* This option specifies the tolerance which is to be used during
*  the Tangency Analytic Solver operations. This values should be
*  between 0 and 1. A lower tolerance indicates that the tangential
*  imperfections are very small and hence large displacements on
*  surfaces should not be performed.
*  Default Value : 0.01
*/
// bhl_get_anal_tgt_tol()  and bhl_set_anal_tgt_tol() can be used to get
// and set the value for this option
DECL_HEAL double bhl_get_anal_tgt_tol( void);
DECL_HEAL void bhl_set_anal_tgt_tol( double opt_val);


// bhl_spline_tgt
/* This option specifies whether spline solver should be performed or not
*  during autoheal
*/
//DECL_HEAL logical bhl_get_spline_tgt( void);
//DECL_HEAL void bhl_set_spline_tgt( logical );

// bhl_spline_tgt_tol
/* This option specifies the tolerance which is to be used during the
*  Spline Solver operations. This value should always be between 0 and
*  1. A lower tolerance indicates that the tangential imperfections
*  are very small and hence large corrections of control points of
*  surfaces should not be performed.
*  Default Value : 0.01
*/
// bhl_get_spline_tgt_tol() and  bhl_set_spline_tgt_tol() can be used
// to get and set the value for this option
//DECL_HEAL double bhl_get_spline_tgt_tol( void);
//DECL_HEAL void bhl_set_spline_tgt_tol( double val);

// bhl_adv_spline_tgt
/* This option specifies whether adv spline solver should be performed or not
*  during autoheal
*/
//DECL_HEAL logical bhl_get_adv_spline_tgt( void);
//DECL_HEAL void bhl_set_adv_spline_tgt( logical );

// bhl_fix_geometry
// This option specifies whether geometry is to be fixed at various stages
// in analytic and spline solvers (e.g. computing surface intersections etc.)
// bhl_get_fix_geometry() and  bhl_set_fix_geometry() can be used
// to get and set the value for this option
DECL_HEAL logical bhl_get_fix_geometry(void);
DECL_HEAL void bhl_set_fix_geometry(logical val);

// bhl_heal_complete
// bhl_get_heal_complete() and  bhl_set_heal_complete() can be used
// to get and set the value for this option
DECL_HEAL logical bhl_get_heal_complete( void);
DECL_HEAL void bhl_set_heal_complete( logical val);

// bhl_remove_attribs
// This option specifies whether all the attributes are to be removed.
// bhl_get_remove_attribs() and  bhl_set_remove_attribs() can be used
// to get and set the value for this option

//DECL_HEAL logical bhl_get_remove_attribs( void);
//DECL_HEAL void bhl_set_remove_attribs( logical val);

// bhl_ignore_top
// This option specifies whether the topology of the enities are to be
// ignored during healing.
DECL_HEAL logical bhl_get_ignore_top(void);
DECL_HEAL void bhl_set_ignore_top(logical val);


//--------------- Options Body-Healer Heuristics ----------------------------
/*
// bhl_simgeom_user_defined
// This option specifies whether user-defined values for options related
// to simgeom should be used or whether body-healer should use its own
// intelligently calculated values.
DECL_HEAL logical bhl_get_simgeom_user_defined(void);
DECL_HEAL void bhl_set_simgeom_user_defined(logical val);

// bhl_stitch_user_defined
// This option specifies whether user-defined values for options related
// to stitch should be used or whether body-healer should use its own
// intelligently calculated values.
DECL_HEAL logical bhl_get_stitch_user_defined(void);
DECL_HEAL void bhl_set_stitch_user_defined(logical val);

// bhl_geombuild_user_defined
// This option specifies whether user-defined values for options related
// to geombuild should be used or whether body-healer should use its own
// intelligently calculated values.
DECL_HEAL logical bhl_get_geombuild_user_defined(void);
DECL_HEAL void bhl_set_geombuild_user_defined(logical val);
*/

#endif //BHL_OPT__HXX

