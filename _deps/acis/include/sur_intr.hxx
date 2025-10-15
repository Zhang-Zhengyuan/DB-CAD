/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// The sl interface class used to pass data from one
// interactive skinning and lofting API to another.
/*******************************************************************/
#if !defined( SLINTR_HEADER_FILE )
#define SLINTR_HEADER_FILE
/**
 * @file sur_intr.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup SLIAPI
 * @{
 */
#include "dcl_skin.h"
#include "alltop.hxx"
#include "lists.hxx"
#include "isgskin.hxx"
#include "guidecrv.hxx"
#include "skin_opts.hxx"
#include "acis_journal.hxx"
class spline;
class Mcurve_data;
class AcisJournal;
class curve_curve_int;
/**
 * Abstract base class that defines the interactive skinning and lofting interfaces.
 * <br><br>
 * <b>Role:</b> The interactive skinning and lofting operations use a common interface defined by this
 * class and consist of ten identical steps.
 * <br><br>
 * This class cannot be instantiated because it contains pure virtual methods which define the
 * skinning and lofting interfaces. These methods are implemented in the <tt>AcisLoftingInterface</tt> 
 * and <tt>AcisSkinningInterface</tt> classes. The <tt>AcisSLInterface</tt> class also contains
 * methods that implement functionality common to both the skinning and lofting interfaces.
 * <br><br>
 * @see SPAunit_vector, BODY
 */
class DECL_SKIN AcisSLInterface : public ACIS_OBJECT
{
public:
   /**
    * C++ allocation constructor requests memory for this object but does not populate it.
    */
    AcisSLInterface();

   /**
    * Creates an <tt>AcisSLInterface</tt> object and initializes its option settings.
    *<br><br>
    * @param opts
    * skin options
    */
    AcisSLInterface(skin_options* opts);

   /**
    * C++ destructor for <tt>AcisSLInterface</tt> which deallocates memory.
    */
    virtual ~AcisSLInterface();

   /**
    * Creates a set of wires that are broken-up according to the skinning and lofting breakup algorithm 
    * (The original wires or coedges that were sent in for input still exist) and it is this list of broken up wires
    * that will be used in the skinning or lofting algorithm.
    * <br><br>
    * @param wire_list
    * array of wire bodies.
    */
    virtual outcome makeWires(BODY**& wire_list) = 0;

   /**
    * Aligns the directions of the wires in the skinning or lofting profiles.
    * <br><br>
    * <b>Role:</b> This is required to create the surfaces.
    * <br><br>
    * @param start
    * Index of starting profile to align.
    * @param end
    * Index of ending profile to align.
    */
    virtual outcome alignWires(int start = 0, int end = 0) = 0;

   /**
    * Creates an equal number of coedges in each wire of the skinning or lofting profiles.
    * <br><br>
    * <b>Role:</b> This is required to create the surfaces.
    * <br><br>
    * @param start
    * Index of starting profile to breakup.
    * @param end
    * Index of ending profile to breakup.
    */
    virtual outcome breakupWires(int start = 0, int end = 0) = 0;

   /**
    * Performs an alignment of the start vertices of the wires in the skinning/lofting profiles.
    * <br><br>
    * <b>Role:</b> This allows a %surface of minimum twist to be built.
    * <br><br>
    * @param start
    * Index of starting profile to be aligned.
    * @param end
    * Index of ending profile to be aligned.
    */
    virtual outcome minimizeTwist(int start = 0, int end = 0) = 0;

   /**
    * Builds a list of edges that represent the extents of the surfaces if the wires were to be skinned at the present configuration.
    * <br><br>
    * <b>Role:</b> This function is intended to be used as a preview of the potential lofting body that
    * will be created. The application developer may delete this list of edges at any time, allow the
    * end user to move the vertices of a lofting or skinning wire and recreate the edges.
    * <br><br>
    * @param edge_list
    * list of created edges.
    */
    virtual outcome buildEdges(ENTITY_LIST& edge_list) = 0;

   /**
    * Builds a list of faces.
    * <br><br>
    * @param face_list
    * list of created faces.
    */
    virtual outcome buildFaces(ENTITY_LIST& face_list) = 0;

   /**
    * Builds the sheet body from the data in the skinning or lofting interface object.
    * <br><br>
    * @param body
    * created body.
    */
    virtual outcome createBody(BODY*& body) = 0;

   /**
    * Does a postprocess stitch of the loft/skinned body back to the edges/coedges given as input.
    * <br><br>
    * @param body
    * created body
    */
    virtual outcome postProcessStitch(BODY*& body);

   /**
    * This function modifies the position of a vertex on a coedge of a wire.
    * <br><br>
    * <b>Role:</b> It is used to alter the coedges of a wire of the skinning process so the end-user may
    * control the body generated.
    * <br><br>
    * @param coedge1
    * coedge to left of position.
    * @param coedge2
    * coedge to right of position.
    * @param pos
    * position to adjust vertex to.
    * @param wire
    * wire this belongs to.
    */
    virtual outcome modifyWire(COEDGE* coedge1, COEDGE* coedge2, const SPAposition &pos, WIRE* wire) = 0;

   /**
    * Adds a vertex to the middle of the specified coedge and to each of the corresponding coedges in the wire list.
    * <br><br>
    * @param coedge
    * coedge to add vertex to middle of.
    * @param wire
    * wire to add vertex on.
    */
    virtual outcome addVertex(COEDGE* coedge, WIRE* wire) = 0;

   /**
    * Removes the specified vertex from its coedge and a vertex from each of the corresponding coedges in each wire in the wire list.
    * <br><br>
    * @param wire
    * wire to remove vertex from.
    * @param pos
    * position of vertex to be removed.
    */
    virtual outcome removeVertex(WIRE*  wire, const SPAposition &pos) = 0;

   /**
    * Deletes a degenerate coedge in each wire of a list of wires.
    * <br><br>
    * <b>Role:</b> It will remove a degenerate coedge in each wire of a list of wires. In order for the
    * coedges to be removed, each wire in the list must  contain a degenerate coedge at the same
    * position. That is, if the fifth coedge of wire one is degenerate, it and the other coedges will
    * only be removed if the fifth coedge of every other wire is degenerate as well.
    * <br><br>
    * @param wire_list
    * list of modified wires.
    */
    virtual outcome collapseWires(ENTITY_LIST& wire_list) = 0;

   /**
    * Sets the given vertex to be the starting vertex for the operation.
    * <br><br>
    * <b>Role:</b> It  sets the given wire to be fixed, meaning it is skipped over during the minimize
    * twist operation.
    * <br><br>
    * @param wire
    * wire that contains the vertex.
    * @param vertex
    * becomes new starting vertex.
    */
    virtual outcome setStartVertex(WIRE* wire, VERTEX* vertex);

   /**
    * Sets the given vertex to be the starting vertex for the operation.
    * <br><br>
    * <b>Role:</b> It sets the given wire to be fixed, meaning it is skipped over during the minimize
    * twist operation.
    * <br><br>
    * @param index
    * index of wire which contains the vertex.
    * @param vertex
    * becomes new starting vertex.
    */
    virtual outcome setStartVertex(int index, VERTEX* vertex);

   /**
    * Sets the wire given by the index to be fixed, meaning it is skipped over during the minimize twist operation.
    * <br><br>
    * @param index
    * index of wire.
    */
    virtual outcome setWireFixed(int index);

   /**
    * Sets the wire given by the index to be not fixed, meaning it is not skipped over during the minimize twist operation.
    * <br><br>
    * @param index
    * index of wire.
    */
    virtual outcome setWireUnFixed(int index);

   /**
    * Any vertex used as the starting vertex must lie on the convex hull.
    * <br><br>
    * <b>Role:</b> This method returns a list of vertices lying on the convex hull.
    * <br><br>
    * @param wire
    * wire.
    * @param vertex_list
    * vertices lying on the convex hull.
    */
    virtual outcome validStartVertices(WIRE* wire, ENTITY_LIST& vertex_list);

   /**
    * Estimates the minimum radius of curvature of skinned surfaces.
    * <br><br>
    * @param min_rad
    * returned minimum radius of curvature.
    */
    virtual outcome estimateMinRadOfCurvature(double &min_rad) = 0;

   /**
    * Simplify the wire geometry.
    */
    virtual outcome simplifyWires();

   /**
    * Returns the type of the interface object: either "skinning" or "lofting".
    */
    virtual char* interfaceType() = 0;

   /**
    * Adds a mapping curve to the interface object. 
    * <br><br>
    * @param num
    * number of points in array - must equal the number of profiles.
    * @param pts
    * array containing the points on the mapping curve - one per profile.
    */
    virtual	logical addMappingCurves(int num, SPAposition* pts);

   /**
    * Removes all mapping curves from the interface object.
    */
    outcome clearMappingCurves();

   /**
    * Removes the specified mapping %curve.
    * <br><br>
    * @param index
    * index into list of mapping curves.
    */
    logical removeMappingCurve(int index);

   /**
    * Returns a list of positions for the specified mapping %curve.
    * <br><br>
    * @param num
    * index into list of mapping curves.
    * @param pos_list
    * returned list of positions.
    */
    logical getMappingCurve(int num, SPAposition*& pos_list = *(SPAposition**)NULL_REF);

   /**
    * Returns a B-spline approximation of the i<sup>th</sup> mapping %curve.
    * <br><br>
    * @param num
    * index into list of mapping curves.
    * @param cur
    * returned B-spline approximation of the mapping %curve.
    */
    logical getMappingCurve(int num, curve*& cur);

   /**
    * Adds a guide %curve to the interface object.
    * <br><br>
    * <b>Role:</b> The %curve underlying the guide must be C<sup>1</sup> continuous and non-looping. In addition,
    * the guide must touch each wire profile within <tt>SPAresabs</tt> and START and STOP exactly on the first
    * and last profiles. Direction is not important. The guides can intersect the section profiles on or
    * off vertices. If the guide does not lie on any vertices, no %surface edge is created. If the guide
    * intersects the first profile off a vertex but intersects any other profile on a vertex, a %surface
    * edge is made. 
    * <br><br>
	* In addition to accepting guides, the overloaded <tt>API</tt> supporting guide curves takes the
    * <tt>logical</tt> flag <tt>virtualGuides</tt>. This flag toggles the guides from being local %surface control to
    * global %surface control. For example, in the case of local %surface control, the guide %curve only
    * affects the %surface created from the edges of the wire that the guide intersects. In the case of
    * <tt>virtualGuides</tt> being <tt>TRUE</tt>, the guide affects all the surfaces made from the wire body cross
    * sections.
    * <br><br>
    * @param guide
    * guide.
    */
    virtual outcome addGuide(EDGE* guide);

   /**
    * Adds a guide %curve to the interface object.
    * <br><br>
    * <b>Role:</b> The %curve underlying the guide must be C<sup>1</sup> continuous and non-looping. In addition,
    * the guide must touch each wire profile within <tt>SPAresabs</tt> and START and STOP exactly on the first
    * and last profiles. Direction is not important. The guides can intersect the section profiles on or
    * off vertices. If the guide does not lie on any vertices, no %surface edge is created. If the guide
    * intersects the first profile off a vertex but intersects any other profile on a vertex, a %surface
    * edge is made. 
    * <br><br>
	* In addition to accepting guides the overloaded <tt>API</tt> supporting guide curves takes the
    * <tt>logical</tt> flag <tt>virtualGuides</tt>. This flag toggles the guides from being local %surface control to
    * global %surface control. For example, in the case of local %surface control, the guide %curve only
    * affects the %surface created from the edges of the wire that the guide intersects. In the case of
    * <tt>virtualGuides</tt> being <tt>TRUE</tt>, the guide affects all the surfaces made from the wire body cross
    * sections.
    * <br><br>
    * @param guide
    * guide.
    */
    virtual outcome addGuide(COEDGE* guide);

   /**
    * Removes all guides from the interface object.
    */
    virtual outcome clearGuides();

   /**
    * Gets the guides from the interface object.
    * <br><br>
    * @param numberOfGuides
    * number of guides.
    * @param guides
    * guide curves.
    */
    virtual outcome getGuides(int& numberOfGuides, curve**& guides);

   /**
    * Sets the value of each skinning/lofting option.
    * <br><br>
    * @param arclen
    * <tt>arclength</tt> option.
    * @param no_twist
    * <tt>no_twist</tt> option.
    * @param align_opt
    * <tt>align</tt> option.
    * @param simpl_opt
    * <tt>simplify</tt> option.
    */
    outcome setOptions(logical arclen, logical no_twist, logical align_opt, logical simpl_opt);

   /**
    * Returns the profile law.
    * <br><br>
    * @param index
    * index.
    */
    virtual law** getProfileLaw(int index) = 0;

    /*
    // tbrv
    */
   /**
    * @nodoc
    */
    virtual spline* makeSurface(int) = 0;

   /**
    * Gets a pointer to the wire at the given index in the list of temporary wires.
    * <br><br>
    * @param index
    * index.
    */
    virtual WIRE* getWireAt(int index);

   /**
    * Returns the number of wires in the temporary set of wires used for skinning or lofting.
    */
    int getNumberOfWires() {return m_numberOfWires;};

   /**
    * Returns a pointer to the temporary set of wires that are used during the skinning or lofting operation.
    */
    BODY** getWireList() {return m_wireList;};

   /**
    * Returns a pointer to the skinned or lofted body.
    */
    BODY* getBody() {return m_body;};

   /**
    * Returns the list of internal coedges.
    */
    virtual Loft_Connected_Coedge_List* get_internalCoedgeList() = 0;

   /**
    * Returns the guide %curve of the specified index. 
    * <br><br>
    * <b>Role:</b> The guides that have been added to the interface object are stored in an array.
    * <br><br>
    * @param index
    * array index of desired guide %curve.
    */
    virtual curve* getGuideAt(int index) const;

   /**
    * Returns the number of guides in the interface object.
    */
    int getNumberOfGuides() const { return m_numberOfGuideCurves; };

    /*
    // tbrv
    */
   /**
    * @nodoc
    */
    void set_haveBrokenUp(logical set){m_haveBrokenUp = set;}

    /*
    // tbrv
    */
   /**
    * @nodoc
    */
    virtual logical find_sli_max_tol(double &max_error);

    
   /**
    * Sets the model tolerance.
    */
    void SetModelTolerance (const double iTolerance) {m_maxModelTolerance = iTolerance;}

   /**
    * Returns <tt>TRUE</tt> if all profiles consist of %straight edges.
    */
    logical is_all_straights ();

	/**
	* Requests that this AcisSLInterface either lose acis entities or detach them.
	* <br><br>
	* <b>Role:</b> To lose or detach acis entities owned by this AcisSLInterface.
	* Detach releases non-model memory allocation and only unlink model objects.
	* Other mode, releases non-model objects as well as model objects.
	* <br><br>
	* @param detach
	*  <tt>logical</tt> if true, only detach aciS entites. False by default.
	*/
	void lose(logical detach = FALSE);

protected:
   /**
    * @nodoc
    */
    int vertexIndex(SPAposition*, WIRE*);
   /**
    * @nodoc
    */
    int vertexIndex(VERTEX*, WIRE*);
   /**
    * @nodoc
    */
    logical checkMappingCurve(int, SPAposition*);
   /**
    * @nodoc
    */
    logical doesMappingCurveExist(int num, SPAposition* pts);
   /**
    * @nodoc
    */
    virtual	logical addMappingCurvesInternal(int num, SPAposition* pts, logical guides_used = FALSE);
   /**
    * @nodoc
    */
    logical storeDefaultUnwist(BODY** in_wires,SPAposition* &all_pts,SPAposition**& new_pts,SPAposition*& ref_pts);
   /**
    * @nodoc
    */
    logical storeNewTwistPositions(BODY** in_wires,SPAposition**& new_pts,SPAposition*& ref_pts);
   /**
    * @nodoc
    */
    void removeDefaultUntwistNewPositions();
   /**
    * @nodoc
    */
    logical putBackDefaultUntwistNewPositions();
   /**
    * @nodoc
    */
    logical makeMinimizeTwistFromPositions(SPAposition* all_pts);
   /**
    * @nodoc
    */
    virtual outcome addGuide(curve*& guide);
   /**
    * @nodoc
    */
    logical alignGuideCurve(curve* guide);
   /**
    * @nodoc
    */
    int guideCurveOnVertices(curve*& guide, 
        SPAposition* guide_positions = NULL, int* coedge_indices = NULL, logical snap = FALSE);
   /**
    * @nodoc
    */
    int guideIntersectsProfile(curve*&       guide, 
        int&          out_index        = *(int*)NULL_REF, 
        SPAposition&  out_intersection = *(SPAposition*)NULL_REF,
        int           profile_index    = -1, 
        logical&      interior         = *(logical*)NULL_REF, 
        SPAparameter& guide_par        = *(SPAparameter*)NULL_REF,
        SPAparameter& coedge_par       = *(SPAparameter*)NULL_REF,
        logical       snap             = FALSE, 
        double&       snap_param       = *(double*) NULL_REF, 
        SPAposition&                   = *(SPAposition*) NULL_REF);
   /**
    * @nodoc
    */
    logical alignWiresViaGuide(curve* guide_curve);
   /**
    * @nodoc
    */
    logical orderGuides();
   /**
    * @nodoc
    */
    void swapGuides(curve** guides,int i,int j);
   /**
    * @nodoc
    */
    void qsortGuides(curve** guides,int left,int right);
   /**
    * @nodoc
    */
    int compareGuideParameters(curve* guide1, curve* guide2);
   /**
    * @nodoc
    */
    double whatParamThisGuide(curve* guide);
   /**
    * @nodoc
    */
    logical makeOneMappingCurve(curve* guide_curve, SPAposition*& pts1);
   /**
    * @nodoc
    */
    outcome makeVirtualGuideCurves(int& numberOfAllGuideCurves, guide_curve *&all_guides);
   /**
    * @nodoc
    */
    logical storeTemporaryGuideVertices(VERTEX* vert);
   /**
    * @nodoc
    */
    logical removeTemporaryGuideVertices(logical closed);
   /**
    * @nodoc
    */
    int m_numberOfWires;
   /**
    * @nodoc
    */
    int m_numberOfInWires;
   /**
    * @nodoc
    */
    BODY** m_wireList;
   /**
    * @nodoc
    */
    BODY** m_inWires;
   /**
    * @nodoc
    */
    logical* m_fixedWireList;
   /**
    * @nodoc
    */
    logical m_allClosed;
   /**
    * @nodoc
    */
    logical m_degenerate;
   /**
    * @nodoc
    */
    logical m_start_degenerate;
   /**
    * @nodoc
    */
    logical m_end_degenerate;
   /**
    * @nodoc
    */		
    SPAposition m_start_point;
   /**
    * @nodoc
    */
    SPAposition m_end_point;
   /**
    * @nodoc
    */
    SPAposition* m_centroids;
   /**
    * @nodoc
    */
    SPAunit_vector* m_normals;
   /**
    * @nodoc
    * made during alignment
    */
    logical m_periodic_u;
   /**
    * @nodoc
    */
    ENTITY_LIST m_original_coedge_list;
   /**
    * @nodoc
    */
    logical    m_haveAligned;
   /**
    * @nodoc
    */
    logical    m_haveUnTwisted;
   /**
    * @nodoc
    */
    logical    m_haveStoredDefaultUntwist;
   /**
    * @nodoc
    */
    SPAposition*  m_defaultUntwist;
   /**
    * @nodoc
    */	    
    SPAposition** m_defaultUntwistNewPositions;
   /**
    * @nodoc
    */
    SPAposition*  m_newPositionsReferencePositions;
   /**
    * @nodoc
    */
    logical*   m_haveSimplified;
   /**
    * @nodoc
    */
    logical    m_haveBrokenUp;
   /**
    * @nodoc
    */
    logical    m_needSimplifyForBadSurface;
   /**
    * @nodoc
    */
    Mcurve_data* mcurve_data;
   /**
    * @nodoc
    */
    int          m_numberOfGuideCurves;
   /**
    * @nodoc
    */
    VOID_LIST    m_guideCurves;
   /**
    * @nodoc
    */
    double m_maxGuideTolerance;
   /**
    * @nodoc
    */
    double m_maxModelTolerance;
   /**
    * @nodoc
    */
    skin_options m_skin_options;
   /**
    * @nodoc
    * The body
    */
    BODY* m_body;
   /**
    * @nodoc
    */
    logical make_profile_edges_tolerant( curve* guide_curve,EDGE*& edge,ENTITY_LIST& elist, double& max_tol);
   /**
    * @nodoc
    */
    logical m_make_tolerant;
   /**
    * @nodoc
    */
    logical trim_guide_curve( curve*& guide_curve,
                            curve_curve_int* lintersection,
                            SPAposition guide_start,
                            SPAposition guide_end,
                            COEDGE* coedge,
                            logical intersection_is_coedge_start,
                            logical intersection_is_coedge_end );
   /**
    * @nodoc
    */
    friend class SkinProfileCluster;
   /**
    * @nodoc
    */
    friend class SkinProfileClusterList;
    
};
/*
// tbrv
 */
/**
 * @nodoc
 * This function computes the minimum radius of curvature of skin surfaces. It has
 * been written with skin surfaces in mind and is not a general purpose function.
 * This function should be used with extreme caution if the user understands the
 * algorithm and its purpose.
 */
DECL_SKIN double get_minimum_radius_of_curvature_skin(ENTITY_LIST &face_list, int &face_number = *(int *)NULL_REF );
/** @} */
#endif
