/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef _STITCH_OPT__HXX_
#define _STITCH_OPT__HXX_

#include <stdio.h>
#include "dcl_stitch.h"
#include "body.hxx"

#include "aggrstch_stitch.hxx"
#include "bhl_stru2.hxx"
#include "stitch_callback.hxx" // hh_set_stitch_opt_callback // hh_set_stitch_opt_callback

//---------------------------------------------------------------------------
/* bhealer_callback_function
*  This function is periodically called during all the stages of Body
*  Healing.
*  The return value of this function is interpreted as below:
*  	0  : No action, the calling function proceeds
*	1  : Interrupt received, abort Body Healing
*/
DECL_STITCH int bhealer_callback_function( void);


// Callback functions for stitch module
DECL_STITCH void stitch_callback_function(BODY*);


// Typedefs for function-types used as callback functions for setting
// options and tolerances for various modeles.
typedef void (*bhl_stitch_opt_callbk) (bhl_anal_stitch_results*);


// The options and tolerances for stitching module
// being performed during auto-heal can be set by the user using
// a callback function. Body-Healer will call this function giving
// the recommended values for all the options and tolerances in a
// structure. If the user desires to change any option value, then
// he should change that particular member of the structure.
// Following are the functions to get and set the function pointer
// for this purpose.
DECL_STITCH
bhl_stitch_opt_callbk bhl_get_stitch_options_callback ();

DECL_STITCH
void bhl_set_stitch_options_callback (bhl_stitch_opt_callbk);

// The following functions are called during autoheal and surface-heal
// to set the options and tolerances of the respective modules.
void
bhl_stitch_options_callback (
				bhl_anal_stitch_results* anal_stitch_results);


// All the following declarations are commented out because now all the
// options and tolerances can be set using callbacks.

//---------------------------------------------------------------------------
// All globals functions and variables are declared here
// These globals can be used by the application developer to enquire or set
// the options provided in this husk.
//---------------------------------------------------------------------------

//--------------- Options for Stitch --------------------------------------
//
// bhl_stitch
/* This option specifies whether stitch should be performed or not
*  during autoheal
*/
DECL_STITCH logical bhl_get_stitch( void);
DECL_STITCH void bhl_set_stitch( logical );

// bhl_stitch_min_tol
/* This option specifies the minimum tolerance to be used by the
*  Body Healer during the stitch operation.
*  eg. if the interface gaps in the incoming data are in the range
*  0.001 to 0.01 then this tolerance is set to 0.001
*  Default Value : 0.01
*/
// bhl_get_stitch_min_tol() and bhl_set_stitch_min_tol()  can be used
// to get and set the value for this option
DECL_STITCH double bhl_get_stitch_min_tol( void);
DECL_STITCH void bhl_set_stitch_min_tol( double val);


// bhl_stitch_max_tol
/* This option specifies the maximum tolerance to be used by the
*  Body Healer during the stitch operation.
*  Recommended Value : This value must be of the order of the
*  maximum gap size between faces in the incoming data.
*  eg. if the interface gaps in the incoming data are in the range
*  0.001 to 0.01 then this tolerance is set to 0.001
*  Default Value : 0.1
*/
// bhl_get_stitch_max_tol() and bhl_set_stitch_max_tol() can be used
// to get and set the value for this option
DECL_STITCH double bhl_get_stitch_max_tol( void);
DECL_STITCH void bhl_set_stitch_max_tol(double val);

// bhl_stitch_plus
// This option specifies whether stepped stitching should be attempted
// before using incremental stitching.
DECL_STITCH logical bhl_get_stitch_plus(void);
DECL_STITCH void bhl_set_stitch_plus(logical val);

void stch_set_stitch_tolerances(BODY *inp_body, double max_stitch_tol);

#endif //STITCH_OPT__HXX

