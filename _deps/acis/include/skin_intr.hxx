/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// The skinning interface class used to pass data from one
// skinning API to another.
/*******************************************************************/
#if !defined( SKININTR_HEADER_FILE )
#define SKININTR_HEADER_FILE
#include "dcl_skin.h"
#include "sur_intr.hxx"
#include "skin_opts.hxx"
#include "alltop.hxx"
#include "skinapi.hxx"
#include "vlists.hxx"
    
/**
 * @file skin_intr.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup SLIAPI
 * @{
 */
class spline;
class guide_curve;

/**
 * Type of skinning operation to perform.
 * @param SKIN_BASIC
 * Skinning through a sequence of wires.
 * @param SKIN_RULED
 * Skinning linearly between pairs of wires in a sequence of two or more wires.
 * @param SKIN_NORMAL
 * Skinning through a sequence of wires with a tangent field defined by a normal to wire planes.
 * @param SKIN_VECTORS
 * Skinning through a sequence of wires with a tangent field defined by a sequence of vectors.
 * @param SKIN_DRAFT
 * Skinning through a sequence of wires with a tangent field defined by draft angles.
 * @param SKIN_PATH
 * Skinning through a sequence of wires with a tangent field defined by a %curve.
 */
enum Skin_Type
{
    SKIN_BASIC,
    SKIN_RULED,
    SKIN_NORMAL,
    SKIN_VECTORS,
    SKIN_DRAFT,
    SKIN_PATH,
    SKIN_RULED_V_DIR,
	SKIN_SWEEP_PATH
};

/**
 * Passes data from one interactive skinning API function to the next and controls the execution of 
 * the interactive skinning operation.
 * <br>
 * <b>Role:</b> <tt>AcisSkinningInterface</tt> maintains the data and controls the execution of an interactive 
 * skinning operation. The object can be directly manipulated by the application (that is, instantiation, 
 * calling of methods, and destruction) or through API functions. The use of API functions is recommended.
 * <br><br>
 * Interactive skinning operations consist of ten steps, which correspond to virtual methods. 
 * The 10 steps are nearly identical for skinning and lofting; therefore, applications can
 * reuse code for both types of operations.
 * <br><br>
 * Applications can derive from this class and extend the skinning functionality by overriding the
 * virtual methods.
 * @see AcisLoftingInterface
 */
class DECL_SKIN AcisSkinningInterface : public AcisSLInterface
{
public:

   /**
    * C++ allocation constructor requests memory for this object but does not populate it.
    */
    AcisSkinningInterface();

   /**
    * Creates a new object and initializes it with the given arguments.
    * <br><br>
    * @param inList
    * list of wire bodies.
    * @param skinning_type
    * type of skinning operation.
    * @param opts
    * skinning options.
    */
    AcisSkinningInterface(ENTITY_LIST& inList, Skin_Type skinning_type, skin_options* opts);

   /**
    * C++ destructor that deallocates memory.
    */
    virtual ~AcisSkinningInterface();

   /**
    * Initializes an object with the given arguments.
    * <br><br>
    * @param inList
    * list of wire bodies.
    * @param skinning_type
    * type of skinning operation.
    * @param opts
    * skinning options.
    */
    logical initialize(ENTITY_LIST& inList, Skin_Type skinning_type, skin_options* opts);
    
   /**
    * Creates and returns pointers to the temporary skinning wires used in the align, minimize twist and breakup algorithms.
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
    * Index of ending profile to align.
    * @param end
    * Index of ending profile to align.
    */
    virtual outcome minimizeTwist(int start = 0, int end = 0);

   /**
    * Builds a list of edges that represent the extents of the surfaces if the wires were to be skinned at the present configuration.
    * <br><br>
    * <b>Role:</b> This function is intended to be used as a preview of the potential skinned body that
    * will be created. The application developer may delete this list of edges at any time, allow the
    * end user to move the vertices of a lofting or skinning wire and recreate the edges.
    * <br><br>
    * @param edge_list
    * returned edge list.
    */
    virtual outcome buildEdges(ENTITY_LIST& edge_list);

   /**
    * Creates a list of faces.
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
    virtual outcome modifyWire(COEDGE* coedge1, COEDGE* coedge2, const SPAposition &adjusted_point, WIRE* wire);

    /*
    // tbrv
    */
   /**
    * @nodoc
    */
    virtual outcome addVertex(COEDGE*, WIRE*);

   /**
    * Removes the specified vertex from its coedge and a vertex from each of the corresponding coedges in each wire in the wire list.
    * <br><br>
    * @param wire
    * wire to remove vertex from.
    * @param pos
    * position of vertex to be removed.
    */
    virtual outcome removeVertex(WIRE* wire, const SPAposition &pos);

    /*
    // tbrv
    */
   /**
    * @nodoc
    */
    virtual outcome collapseWires(ENTITY_LIST&);

    /*
    // tbrv
    */
   /**
    * @nodoc
    */
    virtual outcome setSkinningOptions(logical closed, logical solid, logical virtualGuides = FALSE);

   /**
    * Estimates the minimum radius of curvature of all the surfaces involved in the skin.
    * <br><br>
    * @param min_rad
    * minimum radius of curvature.
    */
    virtual outcome estimateMinRadOfCurvature(double &min_rad);

   /**
    * Returns the skinning option values.
    * <br><br>
    * @param arc_length
    * <tt>arc_length</tt> option value.
    * @param noTwist
    * <tt>no_twist</tt> option value.
    * @param align
    * <tt>align</tt> option value.
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
    * Returns the interface type, either "skinning" or "lofting."
	* This is always "skinning" for an <tt>AcisSkinningInterface</tt>.
    */
    virtual char* interfaceType();

    /*
    // tbrv
    */
   /**
    * @nodoc
    */
    virtual law** getProfileLaw(int /*index*/) {return NULL;}

   /**
    * Sets Skin with Path data.
    * <br><br>
    * @param path
    * skin path.
    */
    outcome setSkinningPath(BODY* path);

   /**
    * Sets Skin to the Planar Normal data.
    * <br><br>
    * @param normals
    * the type of the skin normal constraint.
    */
    outcome setSkinningNormal(skinning_normals normals);

   /**
    * Sets Skin with Vectors data.
    * <br><br>
    * @param in_vectors
    * array of unit vectors.
    * @param num_vectors
    * number of unit vectors.
    * @param in_magnitudes
    * array of magnitudes of vectors.
    * @param num_magnitudes
    * number of magnitudes.
    */
    outcome setSkinningVectorsAndMagnitudes(SPAvector* in_vectors, int num_vectors, double* in_magnitudes, int num_magnitudes);

   /**
    * Sets Skin with Draft Angles data.
    * <br><br>
    * @param draft_start
    * angle of start draft (radians).
    * @param draft_end
    * angle of end draft (radians).
    * @param draft_start_mag
    * magnitude of start draft.
    * @param draft_end_mag
    * magnitude of end draft.
    * @param start_normal
    * normal of starting profile.
    * @param end_normal
    * normal of ending profile.
    */
    outcome setSkinningDraftValues
		(
		double draft_start, 
		double draft_end, 
		double draft_start_mag, 
		double draft_end_mag, 
		const SPAvector &start_normal, 
		const SPAvector &end_normal
		);

	/*
    // tbrv
    */
   /**
    * @nodoc
    */
    virtual logical callCreateSectionList(Loft_Connected_Coedge_List*& sections) { return createSectionList(sections); }

   /**
    * Performs a simplification on the temporary skinning wires.
    * @param iHandleAllCurves
    *    TRUE: merge all curves.<br>
    *    FALSE: merge straight and ellipse curves only.
    */
    virtual outcome simplifyWires(logical iHandleAllCurves = FALSE);
    
   /**
    * Smooths curves at G<sup>0</sup> vertices, if possible (i.e., curves are nearly G<sup>1</sup>).
    * @param oSmoothDone   
    *    TRUE:  at least one %curve has been modified.<br>
    *    FALSE: smoothing was not performed; i.e., wires are intacted.
    * @param iSmoothAngle
    *    Angle (in radians) for two tangent vectors to be consided nearly in the same direction.
    */
    virtual outcome smoothWires
        (
        logical        &oSmoothDone,
        const double   iSmoothAngle,
        double &oTol = *(double *)NULL_REF
        );

    /*
    // tbrv
    */
   /**
    * @nodoc
    */
    outcome sg_order_coedges();

    /*
    // tbrv
    */
   /**
    * @nodoc
    */
    outcome order_coedge_list(COEDGE*& st_coed);
    
    // Utility

   /**
    * @nodoc
    */
    virtual spline* makeSurface(int sur_num);

    /*
    // tbrv
    */
   /**
    * @nodoc
    */
    virtual Loft_Connected_Coedge_List* get_internalCoedgeList() {return NULL;}

    /*
    // tbrv
    */
   /**
    * @nodoc
    */
    virtual logical find_sli_max_tol(double &max_error);

	/**
	* @nodoc
	*/
	void setPathParams(double *);

	/**
	* @nodoc
	*/
	double* getPathParams();

	/**
	* @nodoc
	*/
	void setRailLaw(law ** rail);

	/**
	* @nodoc
	*/
	law**  getRailLaw();

    protected:
        logical createSectionList(Loft_Connected_Coedge_List*&);
        outcome makePointCurves(SPAposition* centroids, SPAunit_vector* normals);
        outcome losePointCurves();
        
        BODY* m_path;
        BODY* m_copyPath;
        Skin_Type m_skinType;
        skinning_normals m_skinNormal;
        SPAvector* m_skinVectors;
        double* m_skinMagnitudes;
        double m_draftStart;
        double m_draftEnd;
        double m_draftStartMag;
        double m_draftEndMag;
		double* m_pathParams;
		law** m_railLaw;

        
    private:
        // Kill the copy constructor and equal operator
        // STI jmg: removed would not compile on UNIX
        // AcisSkinningInterface(AcisSkinningInterface&) {};
        // operator= (const AcisSkinningInterface&) {};
        friend class SkinProfileCluster;
        friend class SkinProfileClusterList;
};
/** @} */
#endif

