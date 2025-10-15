/* ORIGINAL: acis2.1/sg_husk/face/mk_face.hxx */
/* $Id: mk_face.hxx,v 1.19 2002/08/09 17:21:37 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


#if !defined ( SG_MAKE_FACE )
#define SG_MAKE_FACE

#include "dcl_cstr.h"

#include "logical.h"
// ywoo 08Jan01: added the header file.
#include "base.hxx"
// ywoo: end

/**
* @file mk_face.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

class SPAposition;
class SPAvector;
class SPAunit_vector;
class surface;
class curve;
class FACE;
class law;
class SPApar_box;

#include "kernapi.hxx"

/**
 * @nodoc
 */
DECL_CSTR FACE *
make_face_spl_ctrlpts(
			int					,//degree_u,
			logical				,//rational_u,
			int					,//form_u,
			int					,//pole_u,
			int					,//num_ctrlpts_u,
			int					,//degree_v,
			logical				,//rational_v,
			int					,//form_v,
			int					,//pole_v,
			int					,//num_ctrlpts_v,
			const SPAposition[] 	,//ctrlpts,
			const double[]	 	,//weights,
			double				,//point_tol,
			int					,//num_knots_u,
			const double[]		,//knots_u,
			int					,//num_knots_v,
			const double[]		,//knots_v,
			double				 //knot_tol
		);

/**
 * @nodoc
 */
DECL_CSTR FACE *
make_face_spl_intp(
			int					,//number of point in u
			int					,//number of points in v
			const SPAposition[] 	,//array of points (u varies first)
			const SPAunit_vector[]	,//array of start tangents in u
			const SPAunit_vector[]	,//array of end tangents in u
			const SPAunit_vector[]	,//array of start tangents in v
			const SPAunit_vector[]	 //array of end tangents in v
			);


/**
 * @nodoc
 */
DECL_CSTR FACE *
make_face_spl_fit(
			double				,//fit tolerance,
			int					,//number of points in u
			int					,//number of points in v
			const SPAposition[]	,//array of points (u varies first)
			const SPAunit_vector[]	,//array of start tangents
			const SPAunit_vector[]	 //array of end tangents
			);
/**
 * @nodoc
 */
DECL_CSTR FACE*
make_plface(
			const SPAposition& 	,//origin,
			const SPAposition& 	,//left,
			const SPAposition& 	 //right
			);
/**
 * @nodoc
 */
DECL_CSTR FACE*
make_planar_disk(
		const SPAposition&    origin,          // origin point on the plane.
		const SPAunit_vector& normal,		    // normal of the plane
		const double       radius,	        // radius of the outer loop
		const logical      half_space=FALSE // If set to true a full half space is returned
		);
/**
 * @nodoc
 */
DECL_CSTR FACE*
make_conic_face(
		double R,	// vertex radius
		double K,	// conic constant
		double E,	// extent of face (i.e. y^2<E )
		double L	// Length of conic trough
		);
/**
 * @nodoc
 */
DECL_CSTR FACE*
make_conic_face(
		double R,	// vertex radius
		double K,	// conic constant
		double E	// extent of face (i.e. x^2+y^2<E)
		);
/**
 * @nodoc
 */
DECL_CSTR FACE*
make_cnface(
			const SPAposition& 	,// center
			const SPAunit_vector& 	,// normal
			const SPAvector& 		,// major_axis
			double 				,// radius_ratio
			double 				,// sine of half angle
			double 				,// cosine of half angle
			double 				,// start angle (degrees)
			double 				,// end angle (degrees)
			double height		 // height of cone
			);
/**
 * @nodoc
 */
DECL_CSTR FACE*
make_spface(
			const SPAposition&		,//center
			double		 		,//radius
			const SPAunit_vector& 	,//latitude direction
			const SPAunit_vector& 	,//longitude direction
			double 				,//start latitude (degrees)
			double 				,//end latitude (degrees)
			double 				,//start longitude (degrees)
			double				 //end longitude (degrees)
			);
/**
 * @nodoc
 */
DECL_CSTR FACE*
make_lawface(
			law * in_law        ,//defining law
			double minu,
			double maxu,
			double minv,
			double maxv,
			int    in_law_number=0,
			law**  in_other_laws=NULL
			);
/**
 * @nodoc
 */
DECL_CSTR FACE*
make_trface(
			const SPAposition& 	,//center
			const SPAunit_vector& 	,//normal
			double 				,//major radius
			double 				,//minor radius
			const SPAposition& 	,//point on torus defining 0.0
			double 				,//start angle in u (degrees)
			double 				,//end angle in u (degrees)
			double 				,//start angle in v (degrees)
			double 				 //end angle in v (degrees)
		);

#endif
