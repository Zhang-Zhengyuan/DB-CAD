/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef __VA_MESH_OPTION_HXX__
#define __VA_MESH_OPTION_HXX__

// ACIS includes-
#include <dcl_vabridge.h>
#include "base.hxx"
#include "api.hxx"
#include "acistype.hxx"

class global_mesh_controls;
class surf_mesh_controls_internal; 
class tet_mesh_controls_internal; 
class CELL3D;
class FACE;
class EDGE;
class VERTEX;

/**
 * @file va_mesh.hxx
 *
 * @addtogroup Mesh
 * @{
 */

/**
 * Options object that controls the generation of meshes by the Mesher.
 * This is an abstract base class. Applications should instantiate either
 * a va_surface_mesh_options object to control the generation of %surface meshes,
 * or a va_tet_mesh_options object to control the generation of tet meshes or
 * hybrid meshes.
 *
 * \sa va_surface_mesh_options, va_tet_mesh_options
 */
class DECL_VABRIDGE va_base_mesh_options : public ACIS_OBJECT
{
public:
	/**
	 * Default constructor.
	 */
	va_base_mesh_options();

	/**
	 * Copy constructor.
	 *
	 * @param rhs 
	 * Reference to a va_base_mesh_options object
	 */
	va_base_mesh_options(const va_base_mesh_options& rhs);

	/**
	 * Destructor.
	 */
	virtual ~va_base_mesh_options();

	/**
	 * Assignment operator.
	 *
	 * @param rhs - Reference to a va_base_mesh_options object
	 */
	const va_base_mesh_options& operator=(const va_base_mesh_options& rhs);

	/**
	 * Returns the Target Edge Length.
	 *
	 * @return 
	 * target edge length for mesh elements
	 */
	double getTargetEdgeLength() const
	{
		return m_TargetEdgeLength;
	};

	/**
	 *  Sets the Target Edge Length. 
	 *
	 * @param value 
	 * new target edge length value. The default value is 1.0.
	 */
	void setTargetEdgeLength( double value )
	{
		m_TargetEdgeLength = value;
	};

	/**
	 * Returns the Maximum Growth Rate.
	 */
	virtual double getGrowthRate() const = 0;

	/**
	 * Sets Maximum Growth Rate.
	 *
	 * @param value 
	 * new element growth rate value
	 */
	virtual void setGrowthRate( double value ) = 0;

	/**
	 * Returns the Maximum Edge Length:Altitude Ratio.
	 *
	 * @return 
	 * maximum edge length:altitude ratio for mesh elements
	 */
	double getMaxEdgeAltRatio() const
	{
		return m_MaxEdgeAltRatio;
	};

	/**
	 * Sets the Maximum Edge Length:Altitude Ratio.
	 *
	 * @param value 
	 * new maximum edge length:altitude ratio value. The default value is 20.0.
	 */
	void setMaxEdgeAltRatio( double value )
	{
		m_MaxEdgeAltRatio = value;
	};

	/**
	 * Returns the Maximum Interior Angle.
	 *
	 * @return 
	 * maximum interior angle for mesh elements
	 */
	double getMaxAngle() const
	{
		return m_MaxAngle;
	};

	/**
	 * Sets the Maximum Interior Angle.
	 *
	 * @param value 
	 * new maximum interior angle value. The default value is 135.0.
	 */
	void setMaxAngle( double value )
	{
		m_MaxAngle = value;
	};

	/**
	 * Sets the mesh debug file name.
	 * 
	 * If the mesh debug file name has a non-NULL string value,
	 * a mesh debug file will be created when a mesh is generated.
	 * A mesh debug file contains the information used to generate
	 * the mesh. It can be used to communicate meshing issues to
	 * Spatial or VKI.
	 *
	 * @param filename 
	 * new file name
	 */
	void setMeshFile(const char* filename);

   /**
	* Returns the mesh debug file name.
	*/
	const char* getMeshFile() const { return m_MeshFile; };

	/**
	 * Type of mesh options object.
	 */
	enum MESH_OPTIONS_TYPE {
		SURFACE_MESH_OPTIONS,		///< va_surface_mesh_options object
		TET_MESH_OPTIONS			///< va_tet_mesh_options object
	};

	/**
	 * Returns the type of mesh options object.
	 */
	virtual MESH_OPTIONS_TYPE getType( void ) const = 0;

	/**
	 * Returns the flag specifying which VKI connect object is used for queries.
	 *
	 * The default behavior is to use the connect object for the result of
	 * the meshing operation; that is the output mesh. The connect object
	 * for the input geometry can be queried by setting the flag to <tt>TRUE</tt>.
	 */
	logical getUseInternalConnect() { return m_useInternalConnect; }

	/**
	 * Sets the flag specifying which VKI connect object is used for queries.
	 *
	 * The default behavior is to use the connect object for the result of
	 * the meshing operation; that is the output mesh. The connect object
	 * for the input geometry can be queried by setting the flag to <tt>TRUE</tt>.
	 *
	 * @param value 
	 * new value for <tt>useInternalConnect</tt> flag. The default value is <tt>FALSE</tt>.
	 */
	void setUseInternalConnect(logical value) { m_useInternalConnect = value; }

	/**
	 * @nodoc
	 * The following two functions are for internal use only.
	 */

	global_mesh_controls* add_gmc( void );
	global_mesh_controls* get_gmc( void ) const { return m_gmc; }

private:
	char*   m_MeshFile;           // Debug file name for load/save.
	double  m_TargetEdgeLength;   // Target edge length of generated elements.
	double  m_MaxEdgeAltRatio;    // Maximum edge length:altitude ratio.
	double  m_MaxAngle;           // Maximum interior angle.
	logical m_useInternalConnect; // Use the internal (input) Connect object. 

	// For global meshing controls
	global_mesh_controls* m_gmc;       // Internal data structure to hold VKI mesh controls
};

/**
 * Options object that controls the generation of %surface meshes by the Mesher.
 *
 * \sa va_mesh, va_surfmesh, va_tet_mesh_options, va_base_mesh_options
 */
class DECL_VABRIDGE va_surface_mesh_options : public va_base_mesh_options
{
public:
	/**
	 * Default constructor.
	 */
	va_surface_mesh_options();

	/**
	 * Copy constructor.
	 */
	va_surface_mesh_options(const va_surface_mesh_options& rhs);

	/**
	 * Destructor.
	 */
	virtual ~va_surface_mesh_options();

	/**
	 * Assignment operator
	 *
	 * @param rhs va_surface_mesh_options object
	 * @returns Reference to va_surface_mesh_options object
	 */
	va_surface_mesh_options& operator=(const va_surface_mesh_options& rhs);

	/**
	 * Specifies the shape of generated elements.
	 */
	enum GENERATION_MESH_SHAPE {
		SURFMESH_SHAPETRI,		///< Triangular shape
		SURFMESH_SHAPEQUAD		///< Quadrilateral shape
	};

	/**
	 *  Returns the shape of generated elements.
	 *
	 * @return shape of mesh elements
	 */
	GENERATION_MESH_SHAPE getMeshShape() const
	{
		return m_MeshShape;
	};

	/**
	 *  Sets the shape of generated elements.
	 *
	 * @param shape 
	 * shape of mesh elements. The default value is <tt>SURFMESH_SHAPETRI</tt>.
	 */
	void setMeshShape (GENERATION_MESH_SHAPE shape)
	{
		m_MeshShape = shape;
	};

	/**
	 *  Returns the Minimum Edge Length.
	 *
	 * @return minimum edge length for mesh elements
	 */
	double getMinEdgeLength() const
	{
		return m_MinEdgeLength;
	};

	/**
	 *  Sets the Minimum Edge Length.
	 *
	 * @param value 
	 * minimum edge length value of mesh elements. The default value is 0.1.
	 */
	void setMinEdgeLength( double value )
	{
		m_MinEdgeLength = value;
	};

	/**
	 * Returns the Maximum Spanning Angle.
	 *
	 * @return maximum spanning angle for mesh elements
	 */
	double getMaxSpanningAngle() const
	{
		return m_MaxSpanningAngle;
	};

	/**
	 * Sets the Maximum Spanning Angle.
	 *
	 * @param value 
	 *  maximum spanning angle for each mesh element. The default value is 30.0 degrees.
	 */
	void setMaxSpanningAngle( double value )
	{
		m_MaxSpanningAngle = value;
	};

	/**
	 * Returns the Maximum Chord Height.
	 *
	 * @return maximum chord height for a mesh element
	 */
	double getMaxChordHeight() const
	{
		return m_MaxChordHeight;
	};

	/**
	 * Sets the Maximum Chord Height.
	 *
	 * @param value 
	 *  maximum chord height for each mesh element. The default value is 0, implying ignore this parameter.
	 */
	void setMaxChordHeight( double value )
	{
		m_MaxChordHeight = value;
	};

	/**
	 * Returns the Maximum Growth Rate.
	 */
	double getGrowthRate() const
	{
		return m_GrowthRate;
	};

	/**
	 * Sets the Maximum Growth Rate.
	 *
	 * @param value 
	 * the maximum element growth rate. The default value is 1.5.
	 */
	void setGrowthRate( double value )
	{
		m_GrowthRate = value;
	};

	/**
	 * Order of mesh elements to be generated.
	 */
	enum MESH_ORDER {
		MESH_LINEAR = 2,		///< Linear mesh
		MESH_PARABOLIC = 3		///< Parabolic mesh
	};

	/**
	 * Returns the order of generated elements.
	 */
	MESH_ORDER getOrder(void) const
	{
		return m_Order;
	};

	/**
	 * Sets the order of generated elements.
	 *
	 * This is the order of the generated elements, which specifies the number of
	 * nodes to generate along an edge. A value of <tt>MESH_LINEAR</tt> generates 
	 * linear elements, a value of <tt>MESH_PARABOLIC</tt> generates parabolic elements. 
	 * The default value is <tt>MESH_LINEAR</tt>.
	 *
	 * @param order 
	 * order of a %surface or volumetric element.
	 */
	void setOrder(MESH_ORDER order);

	/**
	* Returns whether non-manifold input geometry is accepted.
	*  
	* @return <tt>TRUE</tt> or <tt>FALSE</tt>
	*/
	logical getAcceptNonManifold() const { return m_acceptNonManifold; }

	/** 
	* The SurfMesh module supports the %surface meshing of non-manifold geometries, 
	* in which more than two input triangles share a common edge. 
	* Such geometries are common when modeling branched shell structures, 
	* assemblies of solids, multiple material solids, regions with baffles, etc. 
	* By default, however, for historical reasons, the Mesher assumes 
	* that non-manifold geometry is a modeling error and attempts to remove 
	* non-manifold input triangles from the geometry. This option controls 
	* <tt>SURFMESH_NONMANIFOLD</tt> to enable support for non-manifold 
	* geometries. The default in ACIS is <tt>TRUE</tt>.
	* 
	* @param value 
	* <tt>TRUE/FALSE</tt> 
	*/

	void setAcceptNonManifold(logical value) { m_acceptNonManifold = value; }

	// ***** The following two functions have been deprecated. (8/2013) *****

	/**
	 * @nodoc
	 * Returns whether a mesh has been marked to be "tetrahedralized" when
	 * input body has internal faces.
	 *
	 * @return <tt>TRUE</tt> or <tt>FALSE</tt>
	 */
	logical getEnableIntSurfBack() const { return m_enableIntSurfBack; }

	/**
	* @nodoc
	* Facilitates generation of triangulations on the boundaries 
	* of solids which are to be subsequently tetrahedralized by the TetMesh module. 
	* In this case all internal triangles are replicated by a coincident triangle 
	* with opposite sense, that is, the internal triangles are "double backed". 
	* This is the convention used by the TetMesh module for flagging boundary 
	* triangles which are internal. This option enables <tt>SURFMESH_INTSURFBACK</tt>.
	* The default value is <tt>FALSE</tt>.
	* 
	* @param value 
	* <tt>TRUE/FALSE</tt> 
	*/
	void setEnableIntSurfBack(logical value) { m_enableIntSurfBack = value; }

	/**
	* Returns whether a new mesh is to be produced by refaceting the input ENTITYs.
	*
	* @return <tt>TRUE</tt> or <tt>FALSE</tt>
	*/
	logical getEnableEntityFaceting() const { return m_enableEntityFaceting; }

	/**
	 * Enables faceting mesh when attempting to generate a mesh. 
	 *
	 * @note This parameter is useful only if a tessellation mesh has been previously generated. 
	 * If a mesh is to be generated from scratch and no tessellation mesh has been generated, then 
	 * this parameter must be set to <tt>TRUE</tt>, otherwise the mesher will no have input geometry.
	 * The default value is <tt>TRUE</tt>.
	 *
	 * @param value 
	 * <tt>TRUE/FALSE</tt> 
	 */
	void setEnableEntityFaceting(logical value) { m_enableEntityFaceting = value; }

	/**
	 * Returns whether a %surface mesh has been marked to generate back-to-back elements on internal ACIS faces.
	 *
	 * @return <tt>TRUE</tt> or <tt>FALSE</tt>
	 */
	logical getEnableBackToBack() const { return m_enableBackToBack; }

	/**
	 * Specifies whether back-to-back %surface elements are generated on internal ACIS faces.
	 *
	 * The generation of back-to-back %surface elements on internal ACIS faces is required
	 * for a %surface mesh that is to be used as input for the generation of a tet mesh
	 * or hybrid mesh; however, such elements may not be desired for other %surface mesh
	 * applications.  The default value is <tt>TRUE</tt>.
	 * 
	 * @param value 
	 * <tt>TRUE/FALSE</tt>
	 */
	void setEnableBackToBack(logical value) { m_enableBackToBack = value; }

	/**
	 * Returns whether a %surface mesh has been marked to add short edge lengths to the size map.
	 *
	 * @return <tt>TRUE</tt> or <tt>FALSE</tt>
	 */
	logical getEnablePresNodeSize() const { return m_enablePresNodeSize; }

	/**
	 * Specifies whether short edge lengths are added to the size map. 
	 *
	 * If this parameter has a value of <tt>TRUE</tt>, the lengths of short edges will be added
	 * to the size map used to generate the %surface mesh. This causes small elements to be generated
	 * in the region surrounding short edges and it may result in a higher element count. If this 
	 * parameter has a value of <tt>FALSE</tt>, then the lengths of short edges will not be added to 
	 * the size map used to generate the %surface mesh. This may cause elements with large aspect ratios 
	 * to be generated in the region surrounding short edges and it may result in a lower element count. 
	 * This option enables the <tt>SURFMESH_PRESNODESIZE</tt> parameter. The default value is <tt>FALSE</tt>.
	 * 
	 * @param value 
	 * <tt>TRUE/FALSE</tt>
	 */
	void setEnablePresNodeSize(logical value) { m_enablePresNodeSize = value; }

	/**
	 * Returns whether a %surface mesh will use the Maximum Spanning Angle for edge curvature
	 * as well as face curvature.
	 *
	 * @return <tt>TRUE</tt> or <tt>FALSE</tt>
	 */
	logical getEnableEdgeTangSize() const { return m_enableEdgeTangSize; }

	/**
	 * Specifies whether a %surface mesh will use the Maximum Spanning Angle for edge curvature
	 * as well as face curvature. 
	 *
	 * If this parameter has a value of <tt>TRUE</tt>, the Maximum Spanning Angle will be used for 
	 * edge curvature. This may cause additional elements to be generated in the region surrounding 
	 * edges. If this parameter has a value of <tt>FALSE</tt>, the Maximum Spanning Angle will 
	 * not be used for edge curvature. Generally, the meshing of edges is controlled by the meshing
	 * of the adjacent faces, in which case this option should have a value of <tt>FALSE</tt>.
	 * This option enables the <tt>SURFMESH_EDGETANGSIZE</tt> parameter. The default value is <tt>FALSE</tt>.
	 * 
	 * @param value 
	 * <tt>TRUE/FALSE</tt>
	 */
	void setEnableEdgeTangSize(logical value) { m_enableEdgeTangSize = value; }

	/**
	 * Returns the type of this mesh options object.
	 */
	virtual MESH_OPTIONS_TYPE getType( void ) const
	{
		return SURFACE_MESH_OPTIONS;
	}

	/**
	 * @nodoc
	 * The following two functions are for internal use only.
	 */

	surf_mesh_controls_internal* add_ebc( void );
	surf_mesh_controls_internal* get_ebc( void ) const { return m_ebc; }

private:
	double m_MinEdgeLength;    // Minimum edge length of generated elements.
	double m_GrowthRate;       // Maximum growth rate.
	double m_MaxSpanningAngle; // Maximum spanning angle in degrees for generated elements.
	double m_MaxChordHeight;   // Maximum chord height.  (The maximum deviation from input geometry.)
	MESH_ORDER m_Order;        // Order of mesh to be generated (ie. MESH_LINEAR, MESH_PARABOLIC)
	GENERATION_MESH_SHAPE m_MeshShape;  // Shape of generated elements (i.e., Triangular, Quadrilateral).
	logical m_acceptNonManifold;        // Enable support for non-manifold geometries.
	logical m_enableIntSurfBack;        // Enable mesh to be tetrahedralized if internal faces exist. (Deprecated)
	logical m_enableEntityFaceting;     // Facet entities, thus creating surfmesh input.
	logical m_enableBackToBack;         // Generate back-to-back elements on internal faces.
	logical m_enablePresNodeSize;       // Add short edge lengths to the size map.
	logical m_enableEdgeTangSize;       // Use the spanning angle for edge curvature too.

	// For specifying ENTITY-based meshing controls
	surf_mesh_controls_internal* m_ebc;    // Internal entity-based controls data structure
};

/**
 * Options object that controls the generation of tetrahedral or hybrid meshes by the Mesher.
 *
 * \sa va_mesh, va_tetmesh, va_surface_mesh_options, va_base_mesh_options
 */
class DECL_VABRIDGE va_tet_mesh_options : public va_base_mesh_options
{
public:
	/**
	 * Default constructor.
	 */
	va_tet_mesh_options();

	/**
	 * Copy constructor.
	 */
	va_tet_mesh_options(const va_tet_mesh_options& rhs);

	/**
	 * Destructor.
	 */
	virtual ~va_tet_mesh_options();

	/**
	 * Assignment operator.
	 *
	 * @param rhs va_tet_mesh_options object
	 * @returns Reference to va_tet_mesh_options object
	 */
	va_tet_mesh_options& operator=(const va_tet_mesh_options& rhs);

	/**
	 * Returns the Maximum Growth Rate.
	 */
	double getGrowthRate() const
	{
		return m_GrowthRate;
	};

	/**
	 * Sets the Maximum Growth Rate.
	 *
	 * @param value 
	 * maximum element growth rate value. The default value is 2.0.
	 */
	void setGrowthRate( double value )
	{
		m_GrowthRate = value;
	};

	/**
	 * Returns the type of this mesh options object.
	 */
	virtual MESH_OPTIONS_TYPE getType( void ) const
	{
		return TET_MESH_OPTIONS;
	}

	/**
	 * @nodoc
	 * The following two functions are for internal use only.
	 */

	tet_mesh_controls_internal* add_ebc( void );
	tet_mesh_controls_internal* get_ebc( void ) const { return m_ebc; }

private:
	double m_GrowthRate;       // Maximum growth rate.

	// For specifying ENTITY-based meshing controls
	tet_mesh_controls_internal* m_ebc;    // Internal entity-based controls data structure
};

/** @} */
#endif /*__VA_MESH_OPTION_HXX__*/
