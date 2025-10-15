/*******************************************************************/
/*    Copyright (c) 2016-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef InsertMixedMeshIntoRenderingObject_h
#define InsertMixedMeshIntoRenderingObject_h

#include "gen_rendobj.hxx"
#include "container_utils.hxx"

// if ro is a hoops rendering object, put the mesh into it.
void InsertMixedMeshIntoRenderingObject(
	RenderingObject &ro,
	const SPAint_vector &hoopsConnectivity,
	const float *coords,
	int numCoords,
	rgb_color color,
	double alpha = 0
	);

// put a mesh into hoops where each face of the mesh
// can have a different color.
void InsertMixedMeshIntoRenderingObject(
	RenderingObject &ro,
	const SPAint_vector &hoopsConnectivity,
	const float *coords,
	int numCoords,
	const float *faceColors,
	int numColors
	);

#endif
