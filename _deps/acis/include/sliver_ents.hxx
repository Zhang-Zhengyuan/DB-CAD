/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef SPA_SLIVER_ENTITY_HXX
#define SPA_SLIVER_ENTITY_HXX

#include "mmgr.hxx"
#include "dcl_bool.h"
class outcome;
class FACE;
class AcisOptions;
class BODY;
class ENTITY_LIST;
class ENTITY;
class EDGE;
class VERTEX;

/*******************************************************************/
/**
 * @file sliver_ents.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BOOLAPI
 *
 * @{
 */

class sliver_entity_options_impl;

/**
 * @nodoc
 */
enum sliver_ents_operation {sliver_ents_unknown=-1, sliver_ents_thicken=0, sliver_ents_Boolean=1, sliver_ents_all=2};
/**
 * @nodoc
 */
class DECL_BOOL sliver_entity_options : public ACIS_OBJECT
{
protected:
	sliver_entity_options_impl* m_impl;
	// Don't allow clients to call copy CTOR or operator=    
	sliver_entity_options(sliver_entity_options const& input);
	sliver_entity_options& operator= (sliver_entity_options const& input); 

public:
	sliver_entity_options();
	~sliver_entity_options();

	// Remove or just find sliver entities.
	logical get_do_remove() const;

	// Faces with area>cutoff are not sliver candidates.
	double get_area_cutoff() const;
	// Edges smaller than min_feature_tol are not sliver candidates.
	double get_min_feature_tol() const;
	// Is the sliver removal a Boolean pre-process, or a sheet thicken pre-process?
	sliver_ents_operation get_operation() const;
	// For Boolean operations, we can consider both a tool and blank simultaneously.
	BODY* get_second_body() const;

	// For use in sliver detection mode.
	void get_sliver_ents(ENTITY_LIST& sliver_ents) const;

	// Switch between sliver detection and sliver removal mode.
	void set_do_remove(logical do_remove);

	void set_area_cutoff(double cutoff);
	void set_min_feature_tol(double tol);
	void set_operation(sliver_ents_operation op);
	void set_second_body(BODY* body);

	// Return the total face area found when checking for sheet thicken sliver faces.
	double get_total_thicken_face_area() const;
	// Return the total sliver face area found when checking for sheet thicken sliver faces.
	double get_total_thicken_sliver_face_area() const;

	// For internal use only.
	void set_total_thicken_face_area(double area);
	void set_total_thicken_sliver_face_area(double area);
	void add_sliver_ent(ENTITY* ent);
	sliver_entity_options_impl* get_impl() const;
};
/**
 * @nodoc
 */
DECL_BOOL outcome api_sliver_entities(ENTITY* in_entity, sliver_entity_options* seo=NULL, AcisOptions* ao=NULL);


/**
 * Detects short edges and, optionally, replaces them with tolerant vertices.
 * <br><br>
 * <b>Role:</b> This API function detects edges whose length is less than the specified
 * tolerance and, optionally, replaces them with tolerant vertices. If
 * <tt>replace</tt> is set to <tt>FALSE</tt>, a list of detected short edges is returned
 * and no edges are replaced. If <tt>replace</tt> is set to <tt>TRUE</tt>, all detected
 * short edges are replaced, and a list of replacement tolerant vertices is
 * returned.
 * <br><br>
 * To replace a short edge that has already been detected, 
 * @href api_replace_edge_with_tvertex is recommended.
 * <br><br>
 * <b>Effect:</b> Read-only, unless <tt>replace</tt> is set to <tt>TRUE</tt>.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D InterOp
 * <br><br>
 * @param entity
 * (in) Entity with edges to be checked.
 * @param returned_list
 * (in/out) Returned with short edges or tolerant vertices added.
 * @param tolerance
 * (in) Maximum length of short edges.
 * @param replace
 * (in) Specifies if detected short edges should be replaced.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_detect_short_edges(
					  ENTITY* entity,			       //Entity in which edges are to be checked
					  ENTITY_LIST& returned_list,      //Returned list of either short edges or tVertices
					  const double tolerance = SPAresfit, //Maximum length of short edges
					  logical replace = FALSE		   //True if detected short edges should be replaced
					  , AcisOptions* ao = NULL
	);


// Function that accepts a list of short edge and replaces them with a list of
//tolerant vertices. Generally this function is used if the edge is shorter than SPAresfit.
/**
 * Replaces short edges with tolerant vertices.
 * <br><br>
 * <b>Role:</b> This API function replaces each edge in the list
 * <tt>edgeList</tt> with a tolerant vertex. The list of
 * replacement tolerant vertices
 * is returned via the argument <tt>tVertex_list</tt>.
 * <br><br>
 * This function does <i>not</i>
 * check the length of an edge before replacing it. The only case
 * where an edge will not be replaced is if it is closed.
 * To detect and replace edges shorter than a specified
 * tolerance, @href api_detect_short_edges is recommended.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D InterOp
 * <br><br>
* @param edgeList
 * (in) List of edges to be replaced.
 * @param tVertex_list
 * (in/out) List is emptied and returned, filled with replacement tolerant vertices.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_replace_edge_with_tvertex(
					  ENTITY_LIST& edgeList,    //List of edges to replace
					  ENTITY_LIST& tVertex_list //tVertex list replacing edges
					  , AcisOptions* ao = NULL
		);


//Function that accepts a sliver face and replaces it with a tolerant edge.
//Generally this function is used after calling api_detect_sliver_faces. Its
//main use is to replace a face if it is thinner than SPAresfit.
/**
 * Replaces a two or three-edge sliver face with a tolerant edge.
 * <br><br>
 * <b>Role:</b> This API function attempts to replace a face containing precisely one loop
 * of two or three edges with a tolerant edge. If successful, the face is removed
 * and an edge of the face is converted into a tolerant edge, returned
 * in the list <tt>tedges</tt>. The
 * topology of the owning body is then repaired.
 * <br><br>
 * This function does <i>not</i> check the size of the face before
 * replacing it. Faces in isolation or with free edges are not
 * replaced. To detect and replace sliver faces to a specified
 * tolerance, @href api_detect_sliver_faces
 * is recommended.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D InterOp
 * <br><br>
 * @param face
 * (in) Face to be replaced.
 * @param tedges
 * (in/out) Replacement @href TEDGE is added on return. Any edges removed during the operation may
 * also be removed from this list.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_replace_face_with_tedge(
					  FACE* face,   //FACE to be replaced
					  ENTITY_LIST& tedges //tedges replacing face
					  , AcisOptions* ao = NULL
	);


//Function that accepts an entity (usually a body or wire) and checks
//all faces to see if any are thinner that SPAresfit. If thin (sliver)
//faces are found they are passed back in a entity list. If no sliver
//faces are found an empty list is passed back.
/**
 * Detects sliver faces and, optionally, replaces them with tolerant edges.
 * <br><br>
 * <b>Role:</b> This API function detects sliver faces whose maximum
 * distance among the long edges is smaller than the specified tolerance and who have
 * at least one short edge and at most three long edges. A short edge is an edge whose
 * length is less than the specified tolerance; a long edge is an edge whose length is
 * greater than the specified tolerance. If <tt>tolerance</tt> is set
 * to -1, a suitable tolerance is calculated based on the bounding box of the entity.
 * If the argument <tt>replace</tt> is set to <tt>FALSE</tt>, a list of
 * detected sliver faces is returned and no sliver faces are
 * replaced. If <tt>replace</tt> is set to <tt>TRUE</tt>, the detected sliver
 * faces are replaced with tolerant edges, and the list of replacement
 * tolerant edges is returned.
 * <br><br>
 * The replacement of sliver faces follows this procedure. Firstly, any short
 * edges detected in the given entity are replaced with tolerant vertices as
 * is done in @href api_detect_short_edges. Then sliver faces are replaced
 * by tolerant edges. The resulting tolerant edges are then considered for
 * merging using the same method as @href api_clean_entity. Any remaining
 * tolerant edges are returned in the list <tt>sliver_faces</tt>.
 * <br><br>
 * To replace a sliver face that has already been detected, 
 * @href api_replace_face_with_tedge is recommended.
 * <br><br>
 * <b>Effect:</b> Read-only, unless <tt>replace</tt> is set to <tt>TRUE</tt>.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D InterOp
 * <br><br>
 * @param entity
 * (in) Entity to be checked for sliver faces.
 * @param sliver_faces
 * (in/out) Returned with sliver faces or tolerant edges added.
 * @param tolerance
 * (in) Maximum length of short edges.
 * @param replace
 * (in) Specifies if detected sliver faces should be replaced.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_detect_sliver_faces(
					  ENTITY* entity,   //Entity in which faces are to be checked
					  ENTITY_LIST& sliver_faces,  //Returned list of sliver faces
					  const double tolerance = -1,
					  logical replace = FALSE
					  , AcisOptions* ao = NULL
	);

/**
 * Replaces a zero length edge with a vertex.
 * <br><br>
 * <b>Role:</b> This API function replaces the given zero length edge 
 * with a vertex. If the start and end vertices of the edge are the
 * same vertex, the function retains this vertex. Otherwise a tolerant
 * vertex is constructed.
 * <br><br>
 * This function does <i>not</i> verify the edge is a zero length edge
 * before replacing it. 
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D InterOp
 * <br><br>
 * @param edge
 * (in) Zero length edge to be removed.
 * @param vertex
 * (out) Vertex replacing the zero length edge.
 * @param ao
 * (in) ACIS options such as versioning and journaling.
 **/
DECL_BOOL outcome api_remove_zero_length_edge(
					  EDGE* edge,
					  VERTEX*& vertex,
					  AcisOptions* ao = NULL
		);

#endif //SPA_SLIVER_ENTITY_HXX
