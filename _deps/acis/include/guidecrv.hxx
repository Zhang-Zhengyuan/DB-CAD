/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef GUIDECRV_HXX_INCLUDED
#define GUIDECRV_HXX_INCLUDED

#include <stdio.h>
#include <math.h>

#include "position.hxx"
#include "dcl_skin.h"

class curve;
class SPAposition;
class SPAunit_vector;
class SPAtransf;
class SPAvector;
class WIRE;
class RenderingObject;

class DECL_SKIN guide_curve : public ACIS_OBJECT
{
	curve         *gcurve;		// guide curve
	SPAposition      *anchors;		// anchor SPAposition
	int            n_anchors;	// number of anchors
	int            given;		// is this guide curve given
	guide_curve   *m_previous;	// previous guide curve
	guide_curve   *m_next;		// next guide curve
	int            n_intp;		// number of interpolation point
	int            n_alloc;		// number of alloation for anchors

	// look at neighbor guide curves and make a similar one
	virtual curve *imitate(guide_curve *ref);

	// create discrete points from the guide curve
	// map is where the anchor locates and params are their SPAparameter location
	virtual void   make_guide_points(const curve *, int& num_points, SPAposition *&, int **map=NULL, double **params=NULL);
	virtual void   make_guide_points_approx(const curve *, int& num_points, SPAposition *&, int **map=NULL, double **params=NULL);
	virtual void   make_guide_points_sample(const curve *, int& num_points, SPAposition *&, int **map=NULL, double **params=NULL);

	// curve interpolation scheme
	virtual curve *interpolate(SPAposition *pnts, logical periodic = FALSE);

	// transform from on SPAposition SPAvector pair to the other
	virtual SPAtransf match_transform(const SPAposition &, SPAvector &, SPAposition &, SPAvector &);

	// if both neighbors have given guide curves, use the blend scheme to create a curve
	virtual curve *blend_guide_curves(curve *, curve *, int f_del=TRUE);

	// find the transformation SPAmatrix from one SPAvector to another
	SPAtransf         align(const SPAunit_vector &from, const SPAunit_vector &to);

	// this class can be extended to include vectors at anchors, so it will
	// interploate guide points with tangency control

public:

	guide_curve();
   ~guide_curve();

    void         create_anchors(int num);				// memory allocation
	void         add_anchor(const SPAposition &in_pos);	// add a anchor to the guide curve
	int          is_given() { return given; }			// flag if the guide curve is given
	int          set_curve(curve *);					// assign a guide curve to the class
	void         set_next(guide_curve *in)     { m_next=in; }
	void         set_previous(guide_curve *in) { m_previous=in; }

	curve       *guide() { return gcurve; }						// get guide curve
	SPAposition     get_anchor_at(int nth) { return anchors[nth]; }// get anchor
	double      *get_anchor_params(curve *incrv=NULL);			// find the SPAparameter location of all anchors

	guide_curve *previous() { return m_previous; }
	guide_curve *next()     { return m_next; }
	virtual int  generate();							// create a guide curve from its neighbors
	virtual int  generate_symmetrical();

	void         set_nintp(int npnt) { n_intp=npnt; }	// set the number of discrete points
	virtual int  npoints() { return n_intp; }			// find the discretization number
	virtual int  nanchors() { return n_anchors; }			// find the number of anchors
	void show( RenderingObject *pRO, const ENTITY* ent_with_transf = NULL ) const;
//	void         set_version(AcisVersion& _inversion) {m_version = _inversion;};
};

DECL_SKIN int create_virtual_guide_curves(
	int           nsec,			// number of wires
	WIRE        **wires,		// wires
	int           ncrv,			// number of user guide curves to be added
	curve       **crvs,			// user guide curves
	guide_curve *&guides,		// return guide_curve classes
	int          &nproblems = *(int *) NULL_REF,	// (optional) number of
	curve      ***problem_crvs=NULL					// user guide curves that have problem
);

#endif // GUIDECRV_HXX_INCLUDED

