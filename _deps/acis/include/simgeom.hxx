/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (BHL_SIMGEOM)
#define BHL_SIMGEOM




#include "logical.h"


#include "bs3surf.hxx"
#include "dcl_heal.h"
#include "aggrsimg.hxx"
#include "tols.hxx"

class SPAposition;
class SPAunit_vector;
class SPAvector;
class curve;
class SPApar_box;
class FACE;
class EDGE;
class COEDGE;
class ENTITY_LIST;
class simplify_edge_options ;
class simplify_face_options;

// Tolerances
/*
DECL_HEAL extern double bhl_sim_nor_tol;
DECL_HEAL extern double bhl_sim_pos_tol;
DECL_HEAL extern double bhl_sim_doub_tol;

// Initial and Final Splines

DECL_HEAL extern int bhl_sim_initial_splines;
DECL_HEAL extern int bhl_sim_final_splines;
DECL_HEAL extern int bhl_sim_total_faces;
DECL_HEAL extern int bhl_sim_processed_faces;
*/
// Seperate conversion statistics

/*
DECL_HEAL extern int bhl_sim_no_pl;
DECL_HEAL extern int bhl_sim_no_sph;
DECL_HEAL extern int bhl_sim_no_cyl;
DECL_HEAL extern int bhl_sim_no_con;
DECL_HEAL extern int bhl_sim_no_tor;
DECL_HEAL extern int bhl_sim_no_str;
DECL_HEAL extern int bhl_sim_no_cir;
DECL_HEAL extern int bhl_sim_no_ell;
*/
/*
class simplify_edge_options {
protected:
	double m_pos_tol;
	double m_max_radius;

public:
	simplify_edge_options ();

	// Get Functions
	double get_pos_tol();
	double get_max_radius();

	// Set Functions
	void set_pos_tol(double tol);
	void set_max_radius(double radius);
	
};
*/

DECL_HEAL void bhl_simplify_geometry(ENTITY_LIST &);
DECL_HEAL void bhl_simplify_geometry_surfaces(ENTITY_LIST &);
DECL_HEAL void bhl_simplify_geometry_curves(ENTITY_LIST &);
logical bhl_simplify_curve_geometry (ENTITY* this_ent);

DECL_HEAL logical simSplineFaceGeometry(FACE *faceIn, const simplify_face_options& simp_fc_opts);
DECL_HEAL logical simSplineEdgeGeometry(EDGE *edgeIn, simplify_edge_options* simp_opts);
DECL_HEAL void simCoedgeGeometry(COEDGE *coedgeIn);

DECL_HEAL double

simPtLineDistance(SPAposition &, SPAposition &, SPAunit_vector &);

DECL_HEAL logical
simArePointsCoplanar(SPAposition *, int , SPAposition &, SPAunit_vector &, FACE *, const simplify_face_options& simp_fc_opts);
/*
DECL_HEAL logical
simArePointsLinear(SPAposition *, int , SPAposition &, SPAunit_vector &, double);

DECL_HEAL logical
simArePointsCircular(SPAposition *, int , SPAposition &, SPAvector &, SPAunit_vector &, double, double);

DECL_HEAL logical
simArePointsElliptical(SPAposition *, int , SPAposition & , SPAvector &,
												SPAunit_vector &, double&, double, double);
*/
DECL_HEAL logical
isSplineFacePlanar(FACE *, SPAposition &, SPAunit_vector &, const simplify_face_options& simp_fc_opts, logical force = FALSE);

DECL_HEAL logical
isSplineFaceCylinderical(FACE *, SPAposition &, SPAvector &, SPAunit_vector &,
													logical &, const simplify_face_options& simp_fc_opts, logical force = FALSE);

DECL_HEAL logical
isSplineFaceSpherical(FACE *, SPAposition& , double& , logical&, 
									const simplify_face_options& simp_fc_opts, logical force = FALSE );

DECL_HEAL logical
isSplineFaceTorus(FACE *, SPAposition& , SPAunit_vector& ,
							double& , double& , logical&, double, 
							const simplify_face_options& simp_fc_opts, logical force = FALSE);

DECL_HEAL logical
isSplineFaceConical(FACE *, SPAposition &, SPAvector &, SPAunit_vector &,
							double& , double& , logical&, 
							const simplify_face_options& simp_fc_opts, logical force = FALSE);

DECL_HEAL void
sim90degcone(FACE* );
/*
DECL_HEAL SPAposition
get_intersection_point_of_two_lines(SPAposition& , SPAposition& ,
									SPAunit_vector& , SPAunit_vector& );
*/

DECL_HEAL void
get_best_circle(SPAposition *, int , SPAposition &, SPAvector &, SPAunit_vector &);

DECL_HEAL logical
get_best_ellipse(SPAposition *, int , curve*& );

DECL_HEAL void
get_points_on_torus(bs3_surface& , SPApar_box& , SPAposition*& , int& , double );

DECL_HEAL void
hh_analyze_simgeom(BODY* inp_body, hh_simplify_options *simp_opts = NULL);

DECL_HEAL void
hh_calculate_simgeom(BODY* inp_body);

DECL_HEAL void
hh_fix_simgeom(BODY* inp_body);

DECL_HEAL void
hh_cleanup_simgeom(BODY* inp_body);

logical
sim_spline_edge_geometry( EDGE* orig_edge, EDGE*& output_edge, logical  to_tolerize_if_required, 
						 simplify_edge_options* simp_opts );
// RNC : 5 july 99
extern void
makeSplineFacePlanar(FACE *, SPAposition &, SPAunit_vector &, const simplify_face_options& simp_fc_opts);
extern void
makeSplineFaceCylinderical(FACE *, SPAposition &, SPAvector &, SPAunit_vector &, logical &, const simplify_face_options& simp_fc_opts);
extern void
makeSplineFaceConical(FACE *, SPAposition &, SPAvector &, SPAunit_vector &, double, double, logical &, const simplify_face_options& simp_fc_opts);
extern void
makeSplineFaceTorus(FACE *, SPAposition &, SPAunit_vector &, double, double, logical &, const simplify_face_options& simp_fc_opts);
extern void
makeSplineFaceSpherical(FACE *, SPAposition &, double &, logical &, const simplify_face_options& simp_fc_opts);

#endif

