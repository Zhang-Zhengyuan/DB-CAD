/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (BHL_PROGRESS)
#define BHL_PROGRESS




#include <stdio.h>

#include "dcl_heal.h"
#include "logical.h"

#include "hmainhdr.hxx"
#include "aggrsimg.hxx"
//#include "healhusk/attrib/aggrstch.hxx"
#include "hanalsol.hxx"
#include "huvsolv.hxx"
#include "hsecndry.hxx"
#include "hsharped.hxx"
#include "bhl_stru2.hxx"
#include "bhl_stru.hxx"



//DECL_HEAL extern HealStates bhl_curr_heal_state;

DECL_HEAL extern HealStates bhl_get_current_state(BODY *body);

//DECL_HEAL extern logical bhl_curr_stitch_state;  //MA (16 Oct 1997)

DECL_HEAL extern int bhl_get_simplify_progress( bhl_simplify_progress* = NULL,BODY * =NULL );

DECL_HEAL extern int bhl_get_stitch_progress( bhl_stitch_progress* = NULL,BODY * = NULL);

DECL_HEAL extern int bhl_get_geombuild_analyze_progress( bhl_geombuild_progress* = NULL,BODY * = NULL);

DECL_HEAL extern int bhl_get_analytic_progress( bhl_analytic_progress* = NULL, BODY * = NULL  );

DECL_HEAL extern int bhl_get_sharpedge_progress( bhl_sharpedge_progress* = NULL,BODY*  = NULL );

DECL_HEAL extern int bhl_get_wrapup_progress( bhl_wrapup_progress* = NULL ,BODY * =NULL);

DECL_HEAL extern int bhl_get_isospline_progress( bhl_isospline_progress* = NULL,BODY* = NULL);

DECL_HEAL extern int bhl_get_genspline_progress( bhl_genspline_progress* = NULL,BODY* =NULL);

#endif
