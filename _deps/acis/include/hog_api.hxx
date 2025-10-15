/* ORIGINAL: 3dt2.1/gihusk/api/gi_api.hxx */
// $Id: hog_api.hxx,v 1.2 2002/02/28 16:40:43 skenny Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for Professional ACIS Graphic Interaction Husk  api routines.

#ifndef kernel_api_hog_routines_hxx
#define kernel_api_hog_routines_hxx
//======================================================================

#include "dcl_kern.h"
#include "api.hxx"

/**
* @file hog_api.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISKERNLAWS
 *
 * @{
 */

class law;

/**
* Creates a <tt>DL_item</tt> list of hairs to show a vector field.
* <br><br>
* <b>Role:</b> <tt>Field</tt> is a law specifying the vectors to show (the hairs). base
* is a law specifying where the roots of the hairs lie.
* <br>
* <tt>dim</tt> specifies whether a one-dimensional, two-dimensional, or three-dimensional
* array of hairs is produced.
* <br>
* <tt>starts</tt> and <tt>ends</tt> are arrays of one, two, or three start points and end points,
* depending on dim.
* <br>
* <tt>hairs</tt> is an array containing one, two, or three values, depending on dim,
* specifying how many hairs are to be created between the start and end points.
* <br>
* <tt>return_item</tt> contains the list of hairs for display.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param field
* vector field.
* @param base
* base of field.
* @param starts
* min value in each dimension.
* @param ends
* max value in each dimension.
* @param dim
* size of starts  and ends.
* @param hairs
* number of hairs in each dimension.
* @param return_item
* list of hairs returned.
* @param ao
* acis options.
**/

DECL_KERN outcome api_hedgehog( law *field,				// Vector field to show
							  law *base,				// Base of the SPAvector field
							  double *starts,			// Min value in each dimension
							  double *ends,				// Max value in each dimension
							  int dim,					// Size of starts and ends
							  int *hairs,				// Number of hairs in each dimension
							  ENTITY_LIST &return_item,	// List of hairs to be returned
							  AcisOptions* ao = NULL
							  );

/** @} */
#endif
