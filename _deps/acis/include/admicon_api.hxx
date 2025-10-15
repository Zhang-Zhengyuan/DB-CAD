/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ADMICON_API_H
#define ADMICON_API_H
/**
* @file admicon_api.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISDM
 *
 * @{
 */
#include "dcl_admicon.h"
#include "api.hxx"  // class outcome  // class outcome
class adm_options;
/**
* Terminates the <tt>admicon</tt> library.
* <br><br>
* <b>Effect: </b>System routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Deformable Modeling
 * <br><br>
* @param adm_opts
* adm options.
* @param acis_opts
* ACIS options such as version and journal.
**/
DECL_ADM_ICON outcome api_terminate_admicon(adm_options *adm_opts=NULL, AcisOptions *acis_opts=NULL);
/**
* Initializes the <tt>admicon</tt> library.
* <br><br>
* <b>Effect: </b>System routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Deformable Modeling
 * <br><br>
* @param adm_opts
* adm options.
* @param acis_opts
* ACIS options such as version and journal.
**/
DECL_ADM_ICON outcome api_initialize_admicon(adm_options *adm_opts=NULL, AcisOptions *acis_opts=NULL);

 /** @} */
#endif //ADMICON_API_H

