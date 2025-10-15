// $Id: loft_intr.hxx,v 1.31 2002/07/22 15:36:59 goetting Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// The lofting interface class used to pass data from one
// interactive lofting API to another.

#if !defined( LOFTINTR_HEADER_FILE )
#define LOFTINTR_HEADER_FILE

#include "sur_intr.hxx"
#include "skin_opts.hxx"
#include "option.hxx"

#include "dcl_skin.h"
#include "law.hxx"  

/**
 * @file loft_intr.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup SLIAPI
 * @{
 */

class spline;
class FACE;

extern option_header use_loft_laws;

/**
 * Passes data from one interactive lofting API function to the next and controls the execution of 
 * the interactive lofting operation.
 * <br>
 * <b>Role:</b> <tt>AcisLoftingInterface</tt> maintains the data and controls the execution of an interactive 
 * lofting operation. The object can be directly manipulated by the application (that is, instantiation, 
 * calling of methods, and destruction) or through API functions. The use of API functions is recommended.
 * <br><br>
 * Interactive lofting operations consist of ten steps, which correspond to virtual methods. 
 * The 10 steps are nearly identical for skinning and lofting; therefore, applications can
 * reuse code for both types of operations.
 * <br><br>
 * Applications can derive from this class and extend the lofting functionality by overriding the
 * virtual methods.
 * @see AcisSkinningInterface
 */
class DECL_SKIN AcisLoftingInterface : public AcisSLInterface
{
	public:

		/**
		 * C++ allocation constructor requests memory for this object but does not populate it.
		 */
		AcisLoftingInterface();

		/**
		 * Creates a new object and initializes it with the given arguments.
		 * <br><br>
		 * @param number
		 * number of profiles.
		 * @param coedge_list
		 * array of profiles.
		 * @param opts
		 * skinning options.
		 */
		AcisLoftingInterface(int number, Loft_Connected_Coedge_List* coedge_list, skin_options* opts);

		/**
		 * Creates a new object and initializes it with the given arguments.
		 * Creates an <tt>AcisLoftingInterface</tt> object for <i>Lofting Between Faces</i>.
		 * <br><br>
		 * @param face1
		 * first face.
		 * @param factor1
		 * takeoff factor for coedges on face1.
		 * @param face2
		 * second face.
		 * @param factor2
		 * takeoff factor for coedges on face2.
		 * @param opts
		 * skinning options.
		 */
		AcisLoftingInterface(FACE *face1, double factor1, FACE *face2, double factor2, skin_options* opts);
		
		/**
		 * C++ destructor that deallocates memory.
		 */
		virtual ~AcisLoftingInterface();

		// Public Member Functions

		/**
		 * Creates and returns pointers to the temporary skinning wires used in the align, minimize twist, and breakup algorithms.
		 * <br><br>
		 * @param wire_list
		 * array of wire bodies.
		 */
		virtual outcome makeWires(BODY**& wire_list);

		/**
		 * Aligns the directions of the wires in the skinning or lofting profiles between start and end indices.
		 * <br><br>
		 * <b>Role:</b> This is required to create the surfaces. 
		 * If <tt>start</tt> and <tt>end</tt> both equal 0, then all profiles are processed.
		 * <br><br>
		 * @param start
		 * Index of starting profile to align.
		 * @param end
		 * Index of ending profile to align.
		 */
		virtual outcome alignWires(int start = 0, int end = 0);

		/**
		 * Creates an equal number of coedges in each wire of the skinning or lofting profiles between start and end indices.
		 * <br><br>
		 * <b>Role:</b> This is required to create the surfaces.
		 * If <tt>start</tt> and <tt>end</tt> both equal 0, then all profiles are processed.
		 * <br><br>
		 * @param start
		 * Index of starting profile to breakup.
		 * @param end
		 * Index of ending profile to breakup.
		 */
		virtual outcome breakupWires(int start = 0, int end = 0);

		/**
		 * Performs a twist minimization on the temporary skinning wires between start and end indices.
		 * <br><br>
		 * <b>Role:</b> This allows a %surface of minimum twist to be built.
		 * If <tt>start</tt> and <tt>end</tt> both equal 0, then all profiles are processed.
		 * <br><br>
		 * @param start
		 * Index of starting profile to align.
		 * @param end
		 * Index of ending profile to align.
		 */
		virtual outcome minimizeTwist(int start = 0, int end = 0);

		/**
		 * Performs a simplification on the temporary skinning wires.
		 * <br><br>
		 * @param iHandleAllCurves
		 *    TRUE: merge all curves.<br>
		 *    FALSE: merge straight and ellipse curves only.
		 */
		outcome simplifyWires(logical iHandleAllCurves = FALSE);

		/**
		 * Smooths curves at G<sup>0</sup> vertices, if possible (i.e., curves are nearly G<sup>1</sup>).
		 * @param oSmoothDone   
		 *    TRUE:  at least one %curve has been modified.<br>
		 *    FALSE: smoothing was not performed; i.e., wires are intacted.
		 * @param iSmoothAngle
		 *    Angle (in radians) for two tangent vectors to be consided nearly in the same direction.
		 */
		virtual outcome smoothWires(logical &oSmoothDone, const double iSmoothAngle, double &oTol = *(double *)NULL_REF );

		/**
		 * Builds a list of edges that represent the extents of the surfaces if the wires were to be skinned at the present configuration.
		 * <br><br>
		 * <b>Role:</b> This function is intended to be used as a preview of the potential lofting body that
		 * will be created. The application developer may delete this list of edges at any time, allow the
		 * end user to move the vertices of a lofting or skinning wire and recreate the edges.
		 * <br><br>
		 * @param edge_list
		 * returned edge list.
		 */
		virtual outcome buildEdges(ENTITY_LIST& edge_list);

		/**
		 * Builds a list of faces.
		 * <br><br>
		 * @param face_list
		 * returned face list.
		 */
		virtual outcome buildFaces(ENTITY_LIST& face_list);

		/**
		 * Creates a body.
		 * <br><br>
		 * @param body
		 * body created.
		 */
		virtual outcome createBody(BODY*& body);

		/**
		 * Moves the position of the vertex between the two coedges.
		 * <br><br>
		 * @param coedge1
		 * first coedge associated with vertex.
		 * @param coedge2
		 * second  coedge associated with vertex.
		 * @param adjusted_point
		 * new position of vertex.
		 * @param wire
		 * wire body.
		 */
		virtual outcome modifyWire(COEDGE* coedge1, COEDGE* coedge2, const SPAposition& adjusted_point, WIRE* wire);

		/**
		 * @nodoc
		 */
		virtual outcome addVertex(COEDGE* coed, WIRE* wire);

		/**
		 * Removes the specified vertex from its coedge and a vertex from each of the corresponding coedges in each wire in the wire list.
		 * <br><br>
		 * @param wire
		 * wire to remove vertex from.
		 * @param pos
		 * position of vertex to be removed.
		 */
		virtual outcome removeVertex(WIRE* wire, const SPAposition& pos);

		/**
		 * @nodoc
		 */
		virtual outcome collapseWires(ENTITY_LIST&);

		/**
		 * @nodoc
		 */
		virtual outcome setLoftingOptions(logical perpendicular, logical closed, logical solid);

		/**
		 * Returns the skinning option values.
		 * <br><br>
		 * @param arc_length
		 * <tt>arc_length</tt> option value.
		 * @param noTwist
		 * <tt>no_twist</tt> option value.
		 * @param align
		 * <tt>align</tt> option value.
		 * @param perpendicular
		 * <tt>perpendicular</tt> option value.
		 * @param simplify
		 * <tt>simplify</tt> option value.
		 * @param closed
		 * <tt>closed</tt> option value.
		 * @param solid
		 * <tt>solid</tt> option value.
		 * @param periodic
		 * <tt>periodic</tt> option value.
		 * @param virtualGuides
		 * <tt>virtualGuides</tt> option value.
		 * @param estimate_loft_tanfacs
		 * <tt>estimate_loft_tanfacs</tt> option value.
		 * @param match_vertices
		 * <tt>match_vertices</tt> option value.
		 * @param no_new_twist_vertices
		 * <tt>no_new_twist_vertices</tt> option value.
		 * @param allow_same_uv
		 * <tt>allow_same_uv</tt> option value.
		 * @param arc_length_u
		 * <tt>arc_length_u</tt> option value.
		 * @param postprocess_stitch
 		 * <tt>postprocess_stitch</tt> option value.
		 */
		outcome getOptions	
			(
			logical& arc_length,
			logical& noTwist,
			logical& align,
			logical& perpendicular,
			logical& simplify,
			logical& closed,
			logical& solid,
			logical& periodic,
			logical& virtualGuides,
			logical& estimate_loft_tanfacs,
			logical& match_vertices,
			logical& no_new_twist_vertices,
			logical& allow_same_uv,
			logical& arc_length_u, 
			logical& postprocess_stitch
			);


		/**
		 * @nodoc
		 */
		virtual outcome reenterLoftConnectedCoedgeList(int number, Loft_Connected_Coedge_List* edge_list, BODY**& body_list);

		/**
		 * Estimates the minimum radius of curvature of all the surfaces involved in the skin.
		 * <br><br>
		 * @param min_rad
		 * minimum radius of curvature.
		 */
		virtual outcome estimateMinRadOfCurvature(double &min_rad);

		/**
		 * @nodoc
		 */
		outcome loseSurfaceConditions();

		/**
		 * Returns the interface type, either "skinning" or "lofting."
		 * This is always "lofting" for an <tt>AcisLoftingInterface</tt>.
		 */
		virtual char* interfaceType();

		/**
		 * @nodoc
		 */
		outcome sections(int& _numberOfLoftingCoedges, Loft_Connected_Coedge_List*& _coedgeList);  

		// New functions to estimate tangent factors, get them, and set them if needed.

		/**
		 * @nodoc
		 */
		outcome estimateTanfacScale(SPAinterval &tan_range, 
									double &optimum = *(double *)NULL_REF,
									double &min_radius = *(double *)NULL_REF );

		/**
		 * @nodoc
		 */
		outcome setTangentFactors(double *tan_factors);

		/**
		 * @nodoc
		 */
		outcome getTangentFactors(double *&tan_factors);

		/**
		 * @nodoc
		 */
		virtual law** getProfileLaw(int index);

		/**
		 * @nodoc
		 */
		law* get_coedge_law(COEDGE* coedge, int profile_index);

		/**
		 * @nodoc
		 */
		int get_numberOfLaws(){return m_numberOfLaws;}

		/**
		 * Returns the list of internal coedges.
		 */
		virtual Loft_Connected_Coedge_List* get_internalCoedgeList() {return m_internalCoedgeList;}

		// Utility functions

		/**
		 * @nodoc
		 */
		virtual spline* makeSurface(int);

		/**
		 * @nodoc
		 */
		outcome ownLaws(logical _in) {m_ownLaws = _in; return (API_SUCCESS);};

		//curve* buildCurve();
		//spline* buildSurface();
		//void modifyCurve(int, int, SPAposition, curve*&);
		//void modifyEdge();

		/**
		 * @nodoc
		 */
		virtual logical find_sli_max_tol(double &max_error);

 		/**
		 * @nodoc
		 */
        outcome replaceSurfaceWithLaws();

 		/**
		 * @nodoc
		 */
        outcome postProcessStitch(BODY*& body);

	protected:
		outcome makePointCurves();
		outcome losePointCurves();

	private:
		int m_numberOfLoftingCoedges;
		Loft_Connected_Coedge_List* m_coedgeList;
		int m_numberOfInternalCoedges;
		Loft_Connected_Coedge_List* m_internalCoedgeList;
		int m_numberOfLaws;
		law** m_lawsList;
		logical m_start_degenerate;
		logical m_end_degenerate;
		logical m_fromFaces;
		FACE* m_face0;
		FACE* m_face1;
		logical m_face1_partner;
		logical m_face2_partner;
        logical m_ownLaws;

		// Kill the copy constructor and equal operator
		// STI jmg: removed would not compile on UNIX
		// AcisLoftingInterface(AcisLoftingInterface&) {};
		// operator= (const AcisLoftingInterface&) {};

		// GSSL/RS-LET
		friend class SkinProfileCluster;
		friend class SkinProfileClusterList;
};
/** @} */
#endif


