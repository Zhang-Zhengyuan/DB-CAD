/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef SPA3dPmMesh_hxx
#define SPA3dPmMesh_hxx

extern "C"
{
	#include "meshgems/meshgems.h"
}

#include "position.hxx"

#include "SPA3dPmACIS.h"

/**
* @file SPA3dPmMesh.hxx
* @CAA2Level L1
* @CAA2Usage U1
* \addtogroup ACIS3DPM
*
* @{
*/

namespace Spa3dpm
{

struct Mesh_Impl;

/**
* The <tt>Mesh</tt> class is responsible for the functionality of a MeshGems mesh.
* <tt>Mesh</tt> objects are light weight objects containing only a pointer to the implementation
* object. Copying a <tt>Mesh</tt> object does not duplicate the underlying mesh implementation, it only increases
* the number of light objects referencing the actual mesh. The default constructor <tt>Mesh()</tt> creates
* an object that points to no mesh implementation, and thus it is considered an uninitialized object.
* In case of an error, the class methods throw exceptions.
* All indexes are based at 1.
*/
class DECL_SPA3dpm Mesh
{
public:
	/**
	* This enum type is used to distinguish between the constructors <tt>Mesh(MeshCreate)</tt> and <tt>Mesh()</tt>. The first one
	* creates an initialized but empty mesh, the second creates an uninitialized mesh (an object without an underlying implementation).
	*/
	enum MeshCreate { CreateEmptyMesh };

	/**
	* Constructor.
	* Creates an initialized but empty mesh. Use the MeshGems' set methods to fill out the underlying mesh structure.
	* The input variable is a dummy used to distinguish the constructor from <tt>Mesh()</tt>.
	*/
	Mesh(MeshCreate);

	/**
	* Default constructor.
	* Creates an uninitialized object. It refers to no actual mesh. Use the assignment operator to make the object refer to an actual mesh.
	*/
	Mesh();

	/**
	* Copy constructor. The new object will reference the same mesh as the original object.
	*/
	Mesh(const Mesh &other);

	/**
	* Assignment operator. The "this" object will reference the same mesh as the "other" object.
	*/
	const Mesh &operator=(const Mesh &other);

	/**
	* Destructor. It only deletes the underlying mesh if this object was the last reference to it.
	*/
	~Mesh();

	/**
	* Returns true if the object refers to an actual mesh.
	*/
	bool Initialized() const;

	/**
	* Deep copy of the underlying mesh structure.
	*/
	Mesh				DeepCopy() const;

	/**
	* Returns the native Distene mesh pointer belonging to the mesh referred to by this object.
	*/
	meshgems_mesh_t		*GetMeshgemsMeshPtr();

	/**
	* Gets the number of vertices in the mesh
	* @return the number of vertices
	*/
	int					VertexCount() const;

	/**
	* Gets the coordinates of a mesh vertex
	* @param[in]	iVertex	: index of vertex from 1 to the number of vertices
	* @return	coordinates of the vertex
	*/
	SPAposition			VertexCoords(int iVertex) const;

	/**
	* Gets the tag of a mesh vertex
	* @param[in]	iVertex	: index of vertex from 1 to the number of vertices
	* @return	integer tag of the vertex
	*/
	int					VertexTag(int iVertex) const;

	/**
	* Gets the number of edges in the mesh
	* @return		the number of edges
	*/
	int					EdgeCount() const;

	/**
	* Gets the vertices of a mesh edge
	* @param[in]	iEdge			: index of edge from 1 to the number of edges
	* @return		verticesOnEdge	: integer[2] array containing the vertices of the edge
	*/
	void				EdgeVertices(int iEdge, int verticesOnEdge[2]) const;

	/**
	* Gets the tag of a mesh edge
	* @param[in]	iEdge	: index of edge from 1 to the number of edges
	* @return		integer tag of the edge
	*/
	int					EdgeTag(int iEdge) const;

	/**
	* Gets the number of triangles in the mesh
	* @return	the number of triangles
	*/
	int					TriangleCount() const;

	/**
	* Gets the vertices of a mesh triangle
	* @param[in]	iTriangle			: index of triangle from 1 to the number of triangles
	* @param[out]	verticesOfTriangle	: integer[3] array containing the vertices of the triangle
	*/
	void				TriangleVertices(int iTriangle, int verticesOfTriangle[3]) const;

	/**
	* Gets the tag of a mesh triangle
	* @param[in]	iTriangle	: index of triangle from 1 to the number of triangles
	* @return					integer tag of the triangle
	*/
	int					TriangleTag(int iTriangle) const;

	/**
	* Gets the number of quadrilaterals in the mesh
	* @return the number of quadrilaterals
	*/
	int					QuadrangleCount() const;

	/**
	* Gets the vertices of a mesh quadrangle
	* @param[in]	iQuad			: index of quadrangle from 1 to the number of quadrilaterals
	* @param[out]	verticesOfQuad	: integer[4] array containing the vertices of the quadrangle
	*/
	void				QuadrangleVertices(int iQuad, int verticesOfQuad[4]) const;

	/**
	* Gets the tag of a mesh quadrangle
	* @param[in]	iQuad	: index of quadrangle from 1 to the number of quadrilaterals
	* @return		integer tag of the quadrangle
	*/
	int					QuadrangleTag(int iQuad) const;

	/**
	* Gets the number of tetrahedra in the mesh
	* @return	the number of tetrahedra
	*/
	int					TetrahedronCount() const;

	/**
	* Gets the vertices of a mesh tetrahedron
	* @param[in]	iTet					: index of tetrahedron from 1 to the number of tetrahedra
	* @param[out]	verticesOfTetraHedron	: integer[4] array containing the vertices of the tetrahedron
	*/
	void				TetrahedronVertices(int iTet, int verticesOfTetraHedron[4]) const;

	/**
	* Gets the tag of a mesh tetrahedron
	* @param[in]	iTet	: index of tetrahedron from 1 to the number of tetrahedra
	* @return		integer tag of the tetrahedron
	*/
	int					TetrahedronTag(int iTet) const;

	/**
	* Gets the number of hexahedra in the mesh
	* @return	the number of hexahedra
	*/
	int					HexahedronCount() const;

	/**
	* Gets the vertices of a mesh hexahedron
	* @param[in]	iHex			: index of hexahedron from 1 to the number of hexahedra
	* @param[out]	verticesOfHex	: integer[8] array containing the vertices of the hexahedron
	*/
	void				HexahedronVertices(int iHex, int verticesOfHex[8]) const;

	/**
	* Gets the tag of a mesh hexahedron
	* @param[in]	iHex	: index of hexahedron from 1 to the number of hexahedra
	* @return		integer tag of the hexahedron
	*/
	int					HexahedronTag(int iHex) const;

	/**
	* Gets the number of pyramids in the mesh
	* @return	the number of pyramids
	*/
	int					PyramidCount() const;

	/**
	* Gets the vertices of a mesh pyramid
	* @param[in]	iPyr				: index of pyramid from 1 to the number of pyramids
	* @param[out]	verticesOfPyramid	: integer[5] array containing the vertices of the pyramid
	*/
	void				PyramidVertices(int iPyr, int verticesOfPyramid[5]) const;

	/**
	* Gets the tag of a mesh pyramid
	* @param[in]	iPyr	: index of pyramid from 1 to the number of pyramids
	* @return		integer tag of the pyramid
	*/
	int					PyramidTag(int iPyr) const;

	/**
	* Gets the number of prisms in the mesh
	* return the number of prisms
	*/
	int					PrismCount() const;

	/**
	* Gets the vertices of a mesh prism
	* @param[in]	iPrism	: index of prism from 1 to the number of prisms
	* @param[out]	vPrism	: integer[6] array containing the vertices of the prism
	*/
	void				PrismVertices(int iPrism, int vPrism[6]) const;

	/**
	* Gets the tag of a mesh prism
	* @param[in]	iPrism	: index of prism from 1 to the number of prisms
	* @return		integer tag of the prism
	*/
	int					PrismTag(int iPrism) const;

private:
	Mesh_Impl *impl;
	friend struct II;
};

}

/** @} */
#endif
