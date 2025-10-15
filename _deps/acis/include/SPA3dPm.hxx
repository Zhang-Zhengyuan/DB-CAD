/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef SPA3dPm_hxx
#define SPA3dPm_hxx

#include "acis_journal.hxx"
#include "api.hxx"
#include "spa_progress_info.hxx"

#include "SPA3dPmMesh.hxx"
#include "SPA3dPmSessions.hxx"
#include "SPA3dPmVolumeAssoc.hxx"

#include "SPA3dPmCadSurfSession.hxx"
#include "SPA3dPmCleanerSession.hxx"
#include "SPA3dPmHexaSession.hxx"
#include "SPA3dPmHybridSession.hxx"
#include "SPA3dPmHybridCfdSession.hxx"
#include "SPA3dPmTetraSession.hxx"
#include "SPA3dPmTetraHpcSession.hxx"

#include "SPA3dPmAcis.err"

/**
* \defgroup ACIS3DPM 3D Precise Mesh
*/

/**
* @file SPA3dPm.hxx
* @CAA2Level L1
* @CAA2Usage U1
* \addtogroup ACIS3DPM
*
* @{
*/

/**
* Create a cadsurf session from a list of entities.
* <br><br>
* <b>Role:</b> This API creates a cadsurf session from a list of entities.
* <br><br>
* <b>Effect:</b> Read-only.
* <br><br>
* <b>Journal:</b> Unavailable
* <br><br>
* <b>Product(s):</b> 3D Precise Mesh CadSurf
* <br><br>
* @param[in] inEnts : list of entities to be converted to mesh
* @param[out] outSession : a cadsurf session object that is created by the function and returned through this reference
* @param[in] ao : (optional) ACIS options
* @return The outcome object indicating success or failure.
*/
DECL_SPA3dpm
outcome api_3dpm_create_cadsurf_session(const ENTITY_LIST &inEnts, Spa3dpm::CadSurfSession &outSession, AcisOptions *ao = nullptr);

/**
* Create a cadsurf session from a mesh.
* <br><br>
* <b>Role:</b> This API creates a cadsurf session from a mesh.
* <br><br>
* <b>Effect:</b> Read-only.
* <br><br>
* <b>Journal:</b> Unavailable
* <br><br>
* <b>Product(s):</b> 3D Precise Mesh CadSurf
* <br><br>
* @param[in] inMesh : the input mesh
* @param[out] outSession : a cadsurf session object that is created by the function and returned through this reference
* @param[in] ao : (optional) ACIS options
* @return The outcome object indicating success or failure.
*/
DECL_SPA3dpm
outcome api_3dpm_create_cadsurf_session(const Spa3dpm::Mesh &inMesh, Spa3dpm::CadSurfSession &outSession, AcisOptions *ao = nullptr);


/**
* Create a cleaner session from a mesh.
* <br><br>
* <b>Role:</b> This API creates a cleaner session from a mesh.
* <br><br>
* <b>Effect:</b> Read-only.
* <br><br>
* <b>Journal:</b> Unavailable
* <br><br>
* <b>Product(s):</b> 3D Precise Mesh CadSurf
* <br><br>
* @param[in] inMesh : the input mesh
* @param[out] outSession : a cleaner session object that is created by the function and returned through this reference
* @param[in] ao : (optional) ACIS options
* @return The outcome object indicating success or failure.
*/
DECL_SPA3dpm
outcome api_3dpm_create_cleaner_session(const Spa3dpm::Mesh &inMesh, Spa3dpm::CleanerSession &outSession, AcisOptions *ao = nullptr);

/**
* Create a hexa session from a mesh.
* <br><br>
* <b>Role:</b> This API creates a hexa session from a mesh.
* <br><br>
* <b>Effect:</b> Read-only.
* <br><br>
* <b>Journal:</b> Unavailable
* <br><br>
* <b>Product(s):</b> 3D Precise Mesh Hexa
* <br><br>
* @param[in] inMesh : the input mesh
* @param[out] outSession : a hexa session object that is created by the function and returned through this reference
* @param[in] ao : (optional) ACIS options
* @return The outcome object indicating success or failure.
*/
DECL_SPA3dpm
outcome api_3dpm_create_hexa_session(const Spa3dpm::Mesh &inMesh, Spa3dpm::HexaSession &outSession, AcisOptions *ao = nullptr);

/**
* Create a hybrid session from a mesh.
* <br><br>
* <b>Role:</b> This API creates a hybrid session from a mesh.
* <br><br>
* <b>Effect:</b> Read-only.
* <br><br>
* <b>Journal:</b> Unavailable
* <br><br>
* <b>Product(s):</b> 3D Precise Mesh Hybrid Tetcore, 3D Precise Mesh Hybrid Hexcore
* <br><br>
* @param[in] inMesh : the input mesh
* @param[out] outSession : a hybrid session object that is created by the function and returned through this reference
* @param[in] ao : (optional) ACIS options
* @return The outcome object indicating success or failure.
*/
DECL_SPA3dpm
outcome api_3dpm_create_hybrid_session(const Spa3dpm::Mesh &inMesh, Spa3dpm::HybridSession &outSession, AcisOptions *ao = nullptr);

/**
* Create a hybrid_cfd session from a mesh.
* <br><br>
* <b>Role:</b> This API creates a hybrid_cfd session from a mesh.
* <br><br>
* <b>Effect:</b> Read-only.
* <br><br>
* <b>Journal:</b> Unavailable
* <br><br>
* <b>Product(s):</b> 3D Precise Mesh Hybrid CFD
* <br><br>
* @param[in] inMesh : the input mesh
* @param[out] outSession : a hybrid_cfd session object that is created by the function and returned through this reference
* @param[in] ao : (optional) ACIS options
* @return The outcome object indicating success or failure.
*/
DECL_SPA3dpm
outcome api_3dpm_create_hybrid_cfd_session(const Spa3dpm::Mesh &inMesh, Spa3dpm::HybridCfdSession &outSession, AcisOptions *ao = nullptr);

/**
* Create a tetra session from a mesh.
* <br><br>
* <b>Role:</b> This API creates a tetra session from a mesh.
* <br><br>
* <b>Effect:</b> Read-only.
* <br><br>
* <b>Journal:</b> Unavailable
* <br><br>
* <b>Product(s):</b> 3D Precise Mesh Tetra
* <br><br>
* @param[in] inMesh : the input mesh
* @param[out] outSession : a tetra session object that is created by the function and returned through this reference
* @param[in] ao : (optional) ACIS options
* @return The outcome object indicating success or failure.
*/
DECL_SPA3dpm
outcome api_3dpm_create_tetra_session(const Spa3dpm::Mesh &inMesh, Spa3dpm::TetraSession &outSession, AcisOptions *ao = nullptr);

/**
* Create a tetra_hpc session from a mesh.
* <br><br>
* <b>Role:</b> This API creates a tetra_hpc session from a mesh.
* <br><br>
* <b>Effect:</b> Read-only.
* <br><br>
* <b>Journal:</b> Unavailable
* <br><br>
* <b>Product(s):</b> 3D Precise Mesh Tetra HPC
* <br><br>
* @param[in] inMesh : the input mesh
* @param[out] outSession : a tetra_hpc session object that is created by the function and returned through this reference
* @param[in] ao : (optional) ACIS options
* @return The outcome object indicating success or failure.
*/
DECL_SPA3dpm
outcome api_3dpm_create_tetra_hpc_session(const Spa3dpm::Mesh &inMesh, Spa3dpm::TetraHpcSession &outSession, AcisOptions *ao = nullptr);

/**
* Compute a mesh from a session. Use the funcions @href <tt>api_3dpm_create_cadsurf_session</tt> and @href <tt>api_3dpm_compute_mesh</tt> together to convert a list of entities to a mesh.
* Use the functions @href <tt>api_3dpm_create_tetra_session</tt> and @href <tt>api_3dpm_compute_mesh</tt> together to transform a mesh to a mesh using the tetra operator, etc.
* <br><br>
* <b>Role:</b> This API computes a mesh from a session.
* <br><br>
* <b>Effect:</b> Read-only.
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D Precise Mesh CadSurf, 3D Precise Mesh Hexa, 3D Precise Mesh Tetra, 3D Precise Mesh Tetra HPC, 3D Precise Mesh Hybrid Tetcore, 3D Precise Mesh Hybrid Hexcore,
* 3D Precise Mesh Hybrid CFD
* <br><br>
* @param[in] inSession : the MeshGems session, an object created by one of
* @href <tt>api_3dpm_create_cadsurf_session</tt>, @href <tt>api_3dpm_create_cleaner_session</tt>, @href <tt>api_3dpm_create_hexa_session</tt>, @href <tt>api_3dpm_create_hybrid_session</tt>,
* @href <tt>api_3dpm_create_hybrid_cfd_session</tt>, @href <tt>api_3dpm_create_tetra_session</tt>, @href <tt>api_3dpm_create_tetra_hpc_session</tt>.
* @param[out] outMesh : the resulting mesh
* @param[in] inParams : (optional) a set of parameters for the mesh computation
* @param[in] ao : (optional) ACIS options
* @return The outcome object indicating success or failure.
*/
DECL_SPA3dpm
outcome api_3dpm_compute_mesh(Spa3dpm::MgSession &inSession, Spa3dpm::Mesh &outMesh, const Spa3dpm::Parameters *inParams = nullptr, AcisOptions *ao = nullptr);

/**
* Update the size map of a cadsurf session.
* <br><br>
* <b>Role:</b> This API updates the size map of a cadsurf session. It is assumed that a previous version of the mesh was already computed.
* <br><br>
* <b>Effect:</b> Read-only.
* <br><br>
* <b>Journal:</b> Unavailable
* <br><br>
* <b>Product(s):</b> 3D Precise Mesh CadSurf
* <br><br>
* @param[in] inSession : the MeshGems cadsurf session, an object created by @href <tt>api_3dpm_create_cadsurf_session</tt>.
* @param[in] size_map : a native MeshGems size map.
* @param[in] ao : (optional) ACIS options
* @return The outcome object indicating success or failure.
*/
DECL_SPA3dpm
outcome api_3dpm_update_sizemap(Spa3dpm::CadSurfSession &inSession, sizemap_t *size_map, AcisOptions *ao = nullptr);

/**
* Update the mesh of a cadsurf session after updating the size map.
* <br><br>
* <b>Role:</b> This API computes a mesh from a cadsurf session whose size map was updated by @href <tt>api_3dpm_update_sizemap</tt>. It is assumed that a previous version of the mesh was already computed.
* The operation updates (recomputes) those parts of the mesh where the new size map is different from the original.
* <br><br>
* <b>Effect:</b> Read-only.
* <br><br>
* <b>Journal:</b> Unavailable
* <br><br>
* <b>Product(s):</b> 3D Precise Mesh CadSurf
* <br><br>
* @param[in] inSession : the MeshGems cadsurf session, an object created by @href <tt>api_3dpm_create_cadsurf_session</tt>.
* @param[out] outMesh : the updated mesh. This can be the same as the old mesh object, in which case the old mesh is overwritten by the updated mesh.
* @param[in] ao : (optional) ACIS options
* @return The outcome object indicating success or failure.
*/
DECL_SPA3dpm
outcome api_3dpm_update_mesh(Spa3dpm::CadSurfSession &inSession, Spa3dpm::Mesh &outMesh, AcisOptions *ao = nullptr);

/**
* Create a volume element association class from a mesh. The @href <tt>Spa3dpm::Cell_VolumeElem_Assoc</tt> class can be used to find the 3D volume elements belonging to a CELL, or 
* to find a CELL belonging to a 3D volume element of the mesh. The input mesh must be a volume mesh created by either a tetra, a tetra_hpc, a hybrid, a hybrid_cfd or a hexa session.
* It must also originally come from an ACIS model through a cadsurf session.
* <br><br>
* <b>Role:</b> This API creates a cell-volume_element association object from a volume mesh.
* <br><br>
* <b>Effect:</b> Read-only.
* <br><br>
* <b>Journal:</b> Unavailable
* <br><br>
* <b>Product(s):</b> 3D Precise Mesh CadSurf, 3D Precise Mesh Hexa, 3D Precise Mesh Tetra, 3D Precise Mesh Tetra HPC, 3D Precise Mesh Hybrid Tetcore, 3D Precise Mesh Hybrid Hexcore,
* 3D Precise Mesh Hybrid CFD
* <br><br>
* @param[in] inMesh : the input mesh.
* @param[out] volumeAssoc : the association object
* @return The outcome object indicating success or failure.
*/
DECL_SPA3dpm
outcome api_3dpm_create_volume_assoc(const Spa3dpm::Mesh &inMesh, Spa3dpm::Cell_VolumeElem_Assoc &volumeAssoc);

/**
* Load a mesh from a file.
* <br><br>
* <b>Role:</b> This API loads a mesh from a file.
* <br><br>
* <b>Effect:</b> Read-only.
* <br><br>
* <b>Journal:</b> Unavailable
* <br><br>
* <b>Product(s):</b> 3D Precise Mesh CadSurf, 3D Precise Mesh Hexa, 3D Precise Mesh Tetra, 3D Precise Mesh Tetra HPC, 3D Precise Mesh Hybrid Tetcore, 3D Precise Mesh Hybrid Hexcore,
* 3D Precise Mesh Hybrid CFD
* <br><br>
* @param[in] fileName : path and name of the file
* @param[out] outMesh : the mesh loaded from the file
* @param[in] ao : (optional) ACIS options
* @return The outcome object indicating success or failure.
*/
DECL_SPA3dpm
outcome api_3dpm_load_mesh(const char *fileName, Spa3dpm::Mesh &outMesh, AcisOptions *ao = nullptr);

/**
* Save a mesh to a file.
* <br><br>
* <b>Role:</b> This API saves a mesh to a file.
* <br><br>
* <b>Effect:</b> Read-only.
* <br><br>
* <b>Journal:</b> Unavailable
* <br><br>
* <b>Product(s):</b> 3D Precise Mesh CadSurf, 3D Precise Mesh Hexa, 3D Precise Mesh Tetra, 3D Precise Mesh Tetra HPC, 3D Precise Mesh Hybrid Tetcore, 3D Precise Mesh Hybrid Hexcore,
* 3D Precise Mesh Hybrid CFD
* <br><br>
* @param[in] inMesh : the mesh to be saved
* @param[in] fileName : path and name of the file
* @param[in] ao : (optional) ACIS options
* @return The outcome object indicating success or failure.
*/
DECL_SPA3dpm
outcome api_3dpm_save_mesh(const Spa3dpm::Mesh &inMesh, const char *fileName, AcisOptions *ao = nullptr);

/**
* The @href <tt>mesh3dpm_progress_info</tt> class is part of the implementation of progress meter functionality for 3D Precise Mesh API functions.
* An object of type @href <tt>mesh3dpm_progress_info</tt> is input to the callback function of type @href <tt>proc_mesh3dpm_progress_callback</tt>.
* The percentage of the operation completed can be queried through the <tt>percentage</tt> method.
*/
class DECL_SPA3dpm mesh3dpm_progress_info : public SPA_progress_info
{
public:
	/**
	* Returns a positive integer (between 0 and 100) indicating the percentage of mesh operation
	* which has completed.
	*/
	virtual int percentage() const = 0;

	/**
	* User callbacks can query for which ACIS functionality is calling. Typically this
	* is done on the first call to the callback, to customize the dialog window, and
	* on subsequent calls to branch to the approriate callback code path, if the same
	* callback is metering several different ACIS API functions.
	*/
	virtual SPA_progress_info_type_id get_type() const;

};

/**
* Return type of @href <tt>proc_mesh3dpm_progress_callback</tt>. It determines if the ongoing 3D Precise Mesh
* operation must be interrupted or not.
* @param mesh3dpm_InterruptOperation
* @param mesh3dpm_ContinueOperation
*/
enum mesh3dpm_InterruptState { mesh3dpm_InterruptOperation, mesh3dpm_ContinueOperation };

/**
* Function pointer to implement 3D Precise Mesh progress meter and handle interrupt requests.
* @return whether an interrupt was requested by the user
*/
typedef mesh3dpm_InterruptState (*proc_mesh3dpm_progress_callback)(mesh3dpm_progress_info*);

/**
* Function for setting the mesh progress callback pointer.
* <br>
* <b>Role: </b>The callback mechanism is enabled by installing a custom callback function with
* the @href <tt>set_mesh3dpm_progress_callback</tt> function. The @href <tt>set_mesh3dpm_progress_callback</tt>
* function accepts one argument, which is the custom callback function.
* <br><br>
* @param callback_func_ptr
* custom callback function pointer.
*/
DECL_SPA3dpm
void set_mesh3dpm_progress_callback(proc_mesh3dpm_progress_callback callback_func_ptr);

/**
* Function for getting the mesh progress callback pointer.
* <br>
* <b>Role: </b>The mesh progress callback function pointer can be obtained by
* @href <tt>get_mesh3dpm_progress_callback</tt>.
*/
DECL_SPA3dpm
proc_mesh3dpm_progress_callback get_mesh3dpm_progress_callback();

/** @} */
#endif
