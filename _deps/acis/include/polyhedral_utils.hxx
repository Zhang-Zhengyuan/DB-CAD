/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef polyhedral_utils_hxx
#define polyhedral_utils_hxx

#include "dcl_polyhlc.h"
#include "api.hxx"
#include "SPA_raw_mesh.hxx"  

class BODY;
class FACE;

/**
 * @file polyhedral_utils.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \defgroup POLYUTLAPI Polyhedral Utilities
 * \ingroup POLYHLAPI
 * @{
 */

/**
* Determines whether an ACIS body is polyhedral or not.
* <br><br>
* <b>Role:</b> Determines whether an ACIS body is polyhedral or not, which is useful for polyhedral-specific operations such as <tt>api_polyhedral_decimate</tt>.
* ACIS body whose all faces have polyhedral geometry will return TRUE, while Hybrid ACIS body where some of the faces have exact geometry ( analytical surfaces
* and/or splines ) will return FALSE.
* <br><br>
* <b>Errors:</b>
* <br><br>
* <b>Limitations:</b> 
* <br><br>
* <b>Journal:</b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Polyhedral
* <br><br>
* @param iBody
* any ACIS BODY
**/
DECL_POLYHLC bool is_polyhedral_body(BODY* iBody);

/**
* Determines whether an ACIS entity is hybrid or not.
* <br><br>
* <b>Role:</b> Determines whether an ACIS body is hybrid or not. A hybrid body is composed of some faces having precise geometry and the rest having polyhedral mesh geometry.
* <br><br>
* <b>Errors:</b>
* <br><br>
* <b>Limitations:</b>
* <br><br>
* <b>Journal:</b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Polyhedral
* <br><br>
* @param iEnt
* ACIS ENTITY
**/
DECL_POLYHLC bool is_hybrid_entity(ENTITY const* iEnt);

/**
* Determines whether an ACIS polyhedral body is watertight or not.
* <br><br>
* <b>Role:</b> Determines whether or not a polyhedral body is watertight, which is important for many modeling operations.  In a watertight polyhedral mesh every facet bar is shared by exactly two facets.
* <br><br>
* <b>Errors:</b>
* <br><br>
* <b>Limitations:</b> 
* <br><br>
* <b>Journal:</b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Polyhedral
* <br><br>
* @param iBody
* ACIS BODY with polyhedral geometry
**/
DECL_POLYHLC bool is_polyhedral_body_watertight(BODY const* iBody);

class polyhedral_mesh_options;
/**
* Gets the mesh geometry (triangles) of a given FACE. 
* <br><br>
* <b>Role:</b> Retrieves the polyhedral mesh that forms the surface geometry of the input FACE. 
* <br><br>
* <b>Errors:</b>
* <br><br>
* <b>Limitations:</b> 
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Polyhedral
* <br><br>
* @param face
* Input FACE
* @param output_mesh
* Spa_raw_mesh_sptr object that holds the polyhedral mesh geometry
* @param pmo
* polyhedral_mesh_options (for future use)
* @param ao
* ACIS options
**/
DECL_POLYHLC outcome api_polyhedral_get_mesh(FACE const* face, 
											Spa_raw_mesh_sptr& output_mesh,
											polyhedral_mesh_options* pmo = NULL,
											AcisOptions* ao = NULL);

/**
* Gets the mesh geometry (triangles) of a given BODY.
* <br><br>
* <b>Role:</b> Retrieves the polyhedral global mesh geometry of the input BODY.
* <br><br>
* <b>Errors:</b>
* <br><br>
* <b>Limitations:</b> 
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Polyhedral
* <br><br>
* @param body
* Input ACIS BODY
* @param output_mesh
* Spa_raw_mesh_sptr object that holds the global polyhedral mesh geometry
* @param pmo
* polyhedral_mesh_options (for future use) 
* @param ao
* ACIS options
**/
DECL_POLYHLC outcome api_polyhedral_get_mesh(BODY const* body, 
											Spa_raw_mesh_sptr& output_mesh,
											polyhedral_mesh_options* pmo = NULL,
											AcisOptions* ao = NULL);

class polyhedral_manifold_options;
/**
* Checks whether the input polyhedral body has any non-manifold topology and(/or) geometry.
* <br>
* <b>Role: </b> This API checks the ACIS polyhedral body's geometry and topology,for non-manifold entities. 
* <br><br>
* <b>Errors:</b> <tt>body</tt> is a NULL pointer or is not a polyhedral body.
* <br><br>
* <b>Limitations:</b> 
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Polyhedral
* <br><br>
* @param body
* Body to be checked.
* @param is_manifold
* Whether or not the input body is manifold
* @param pmo
* Polyhedral manifold options (for future use)
* @param ao
* ACIS options.
**/
DECL_POLYHLC outcome api_polyhedral_manifold_checker( BODY* body, 
													  bool& is_manifold, 
													  polyhedral_manifold_options* pmo = NULL, 
													  AcisOptions* ao = NULL);


/**
* Returns the voxel size corresponding to a polyhedral offset operation.
* <br><br>
* <b>Role:</b> Computes the voxel-size that will be used during offset operation involving a Polyhedral or Hybrid
* ACIS body for the given Level-Of-Detail and Offset-distance.
* <br><br>
* <b>Errors:</b>
* <br><br>
* <b>Limitations:</b>
* <br><br>
* <b>Journal:</b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Polyhedral
* <br><br>
* @param iBody
* a Polyhedral or Hybrid ACIS BODY
**/
DECL_POLYHLC double compute_voxel_size_for_polyhedral_offset(BODY const* iBody, int iLevelOfDetail, double iOffsetDistance);

/** @} */
#endif
