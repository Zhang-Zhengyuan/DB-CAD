/* ORIGINAL: acis2.1/faceter/meshing/af_laddr.hxx */
// $Id: af_laddr.hxx,v 1.12 2001/05/09 19:47:01 gradclif Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _AF_LADDR
#define _AF_LADDR

#include "dcl_fct.h"
#include "af_xpar.hxx"
#include "af_geom.hxx"

class AF_WORKING_FACE;

// An AF_LADDER defines a set of parallel lines in uv space.
//
// The ladder contains lines (rungs) numbered 0<=i<=m.
// Each rung is the horizontal line
//		b = b0 + (i/m)*(bm-b0)
// in an ab coordinate system defined by a 2D-to-2D transformation
// The xform operation of the transform take coordinates from
//		the applicatoin uv space into the ladder ab space.
// The invert operation converts ladder coordinates to application space.
//
// This class is also used as the base class for non uniform ladder class. 
//

#include "af_ladut.hxx"


typedef enum {
	AF_LADDER_GENERAL_VECTOR,
	AF_LADDER_FIXED_U,
	AF_LADDER_FIXED_V } AF_LADDER_DIRECTION;

enum {
	AF_LADDER_TYPE,
	AF_VAR_LADDER_TYPE };

class DECL_FCT AF_LADDER : public ACIS_OBJECT {
	void fix_db();	// computes db as noted above.
	AF_LADDER_DIRECTION dir;	// Summarizes transform. This is
		// redundant information, but useful to save time.
  public:
	double db;		// Actual step between grid lines.
					// This is always nonzero !!!  Degenerate case
					//  b0==bm  will generate many lines other than
					// at b0, but normal cases will require no
					//  tests before division.
	AF_PAR_TRANS *T;
    unsigned int m;		// Max line index. Lines are numbered 0..m inclusive

	double b0;
	double bm;	
	// return the type of AF_LADDER
	virtual int id() const;
	// return the min rung index other than the baseline
	int  first_internal_id() const;
	// test if id is an internal rung, and return its altitude
	virtual int get_internal_altitude(unsigned id,double &b) const;
	virtual double altitude(unsigned id) const;	// return the altitude of the i'th line.
	int degenerate() const;	// test if the count or altitude change is null
	virtual void select_internal_id_range(
		double v0,double v1,		// range of values select in grid
		int &i0,int &i1,			// a range of grid lines that completely
									// covers the v0..v1 range.  It may
									// include grid lines outside the range.
		logical inclusive = FALSE);
	AF_LADDER(unsigned m,
		AF_LADDER_DIRECTION _dir,
		AF_PAR_TRANS *transform,double b0,double bm);
	AF_LADDER(unsigned m,
		AF_LADDER_DIRECTION _dir,
		AF_PAR_TRANS *transform,const PAR_BOX &pb);
	AF_LADDER();
	virtual ~AF_LADDER();
	PAR_VEC get_dir();	// Query the direction.
	virtual void debug();
    double get_db();
	double get_abs_range();	// return absolute size of b range.
	void reset_count(int mnew,int mmax);	// Try to change the count to mnew, up
							// to limit mmax
};

// AF_VAR_LADDER is a deriven class of the ladder class that has the rungs
// unequally spaced. It saves the positions of grid lines in an array and overrides 
// the functions of get a grid line SPAposition. 

class DECL_FCT AF_VAR_LADDER: public AF_LADDER
{
	double *par_val;	// An array that holds SPAparameter values in ascending order
							// from b0 to bm.
  public:
	// return the type of AF_VAR_LADDER
	virtual int id() const;
	// test if id is an internal rung, and return its altitude
	int get_internal_altitude(unsigned id,double &b) const;
	double altitude(unsigned id) const;	// return the altitude of the i'th line.
	void select_internal_id_range(
		double v0,double v1,		// range of values select in grid
		int &i0,int &i1,			// a range of grid lines that completely
									// covers the v0..v1 range.  It may
									// include grid lines outside the range.
		logical inclusive = FALSE);
	AF_VAR_LADDER( unsigned m,
		AF_LADDER_DIRECTION _dir,
		AF_PAR_TRANS *transform, 
		double b0, double bm,
		double *p_arr );	// Pointer to SPAparameter values array
	AF_VAR_LADDER( unsigned _m,
		AF_LADDER_DIRECTION _dir,
		AF_PAR_TRANS *transform, 
		const PAR_BOX &pb,
		//double *p_arr );	// Pointer to SPAparameter values array
		PARAMARR &p);
	AF_VAR_LADDER();
	virtual ~AF_VAR_LADDER();		// Destructor for the class.
	void reset_lines( double *arr, int _m ); // Reset the SPAparameter array to given
						                     // array and updates number of grid lines.
	double min_step();
	void debug();
};






//********************************************************
// Function: af_dual_ruling_conditions
// Purpose: enforce accuracy and count conditions that
//		involve 2 rulings
//********************************************************
DECL_FCT void af_dual_ruling_conditions(
		AF_LADDER &R1,AF_LADDER &R2,
		double dsdu1,double dsdu2,		// Space tangent lengths in R1,R2
		double grid_aspect_ratio,		// Tolerance for relative size of steps
		int mmax				// Limit on steps
		);

DECL_FCT void af_dual_ruling_conditions(
		AF_VAR_LADDER *R1,AF_VAR_LADDER *R2,
		double dsdu1,double dsdu2,		// Space tangent lengths in R1,R2
		double grid_aspect_ratio,		// Tolerance for relative size of steps
		int mmax				// Limit on steps
		);

//************************************************************
// Function: af_setup_ruling
// Purpose:  make a ruling with counts set according to curvature
//		and size criteria.
// Input:
//	range = the min and max altitudes that must be covered in the
//				ladder.
//	dsdu = the amount of geometric length covered per unit in parametric space.
//			(Used to determine the max SPAparameter space step indicated
//			by the max geometric step)
//	rho = the radius of curvature
//  dmax = max deviation from the surface (due to curvature)
//  hmax = max geometric size in the ladder.
//	dNmax = max angular variation (in degrees) between adjacent lines.
//	T = the SPAparameter space transform between the ladder direction and
//		the original parametric space.
//	uv_box=the original parametric SPAbox that must be covered by the ladder.
//************************************************************
DECL_FCT AF_LADDER *af_setup_ruling(
	const INTERVAL	&range,
	SURFACE_PROP	&prop,
	double			dmax,
	double			hmax,
	double			dNmax,
	double			dumax,
	int				minstep,
	int				maxstep,
	AF_PAR_TRANS	*T,
	AF_LADDER_DIRECTION dir,
	const PAR_BOX &	uv_box
);

//************************************************************
// Function: af_setup_var_ruling ( AF_VAR_LADDER ) type
// Purpose: make non unifrom ruling according to passed information
//			about curvature and twist.
// Input:
//	range = the min and max altitudes that must be covered in the
//				ladder.
//	dsdu = the amount of geometric length covered per unit in parametric space.
//			(Used to determine the max SPAparameter space step indicated
//			by the max geometric step)
//  dmax = max deviation from the surface (due to curvature)
//  hmax = max geometric size in the ladder.
//	dNmax = max angular variation (in degrees) between adjacent lines.
//	T = the SPAparameter space transform between the ladder direction and
//		the original parametric space.
//	uv_box=the original parametric SPAbox that must be covered by the ladder.
//  crv = an array of n cuvature values at n sub intervals of the given range
//  twist = an array of n twist vaues at n sub intervalw of the given range
//  n = number of sub intervals
//************************************************************
DECL_FCT AF_LADDER *af_setup_var_ruling(
	const INTERVAL	&range,
	SURFACE_PROP	&prop,
	double			dmax,
	double			hmax,
	double			dNmax,
	double			dumax,
	int				minstep,
	int				maxstep,
	AF_PAR_TRANS	*T,
	AF_LADDER_DIRECTION dir,
	const PAR_BOX &	uv_box,
	AF_WORKING_FACE	*fw
);


#endif
