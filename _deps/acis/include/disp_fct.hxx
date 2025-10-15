/* ORIGINAL: 3dt2.1/facet/fct_utl.hxx */
// $Id: disp_fct.hxx,v 1.6 2001/12/14 19:53:18 jmb Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef DISP_FCT_HXX
#define DISP_FCT_HXX

#include "logical.h"

class ENTITY;
class SchemeView;

void DisplayEntityFacets(ENTITY* ent, SchemeView*, logical hide);
#endif
