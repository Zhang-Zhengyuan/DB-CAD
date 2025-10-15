/* ORIGINAL: acis2.1/spline/sg_bs3s/bs3scont.hxx */
/* $Id: bs3scont.hxx,v 1.8 2002/08/09 17:15:42 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


#if !defined( BS3_SURFACE_CONT_CLASS )
#define BS3_SURFACE_CONT_CLASS

#include "dcl_spl.h"
#include "bs3surf.hxx"
// ywoo 08Jan01: added the header file.
#include "mmgr.hxx"
// ywoo: end

class SPAvector;
class SPApar_pos;

class DECL_SPLINE bs3_surface_cont : public ACIS_OBJECT {

private:
	// The data to save the number of u knots and the number of offsending u
	// knots.
	int no_u_knots, no_v_knots;

	// Array of SPApar_pos which save the specific uv values which are incorrect,
	// i.e., there is either a surface discontinuity or too much multiplicity
	SPApar_pos *off_u_knots;
	SPApar_pos *off_v_knots;

	// Arrays of left and right tangents at the above_knot value.
	SPAvector *off_u_left_tans, *off_u_right_tans;
	SPAvector *off_v_left_tans, *off_v_right_tans;

	// Arrays which save the multiplicity at the above knot values.
	int *off_u_multps, *off_v_multps;

	// Save the degree of the bspline in u and v direction
	int degree_u, degree_v;

public:

	// Constructor given all the data.  This routine takes the bs3_surface
	// and classifies it, and saves the information.
	bs3_surface_cont( const bs3_surface );

	// Class Destructor
	~bs3_surface_cont();

	// Query functions

	// Return the number of discontinuities in the u SPAparameter, the actual
	// SPAparameter values  and tangents are returned in the arguments.

	int u_disconts( 
			SPApar_pos *&, 		// Knots returned
			SPAvector *&, 			// Left u partials
			SPAvector *& 			// Right u partials
	);


	// Return the number of discontinuities in the v SPAparameter, the actual
	// SPAparameter values  and tangents are returned in the arguments.

	int v_disconts( 
			SPApar_pos *&, 		// Knots returned
			SPAvector *&, 			// Left v partials
			SPAvector *& 			// Right v partials
	);



	// Return the number of knots in u-direction where the multiplicity is
	// greater than the degree of the bspline.

	int u_multps( SPApar_pos *& );



	// Return the number of knots in v-direction where the multiplicity is
	// greater than the degree of the bspline.

	int v_multps( SPApar_pos *& );

	// Debug Function
	// void debug( char const *, FILE * );

};

#endif
