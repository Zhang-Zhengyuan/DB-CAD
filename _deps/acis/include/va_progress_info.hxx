/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for progress meter for meshing

#if !defined( MESH_PROGRESS_INFO_HXX )
#define MESH_PROGRESS_INFO_HXX

#include "dcl_vabridge.h"
#include "spa_progress_info.hxx"

/**
* @file va_progress_info.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup Mesh
 *
 * @{
 */

/**
 * Class for 3D Mesh progress meter.
 * <b>Role: </b> Instances of this class contain the progress data for a mesh 
 * generation operation. An application can query the mesh_progress_info object
 * in its progress metering callback function for information about the 
 * percentage completion, the phase of the meshing operation, or the current 
 * number of nodes or elements. In addition, member functions exist to cancel 
 * mesh generation or progress metering. 
 */
class DECL_VABRIDGE mesh_progress_info : public SPA_progress_info 
{

private:
	// Instance Data Members
	int m_percentage;
	int m_phase;
	int m_num_nodes;
	int m_num_elements;
	logical m_meshing_canceled;

	// Class Data Member (not instance data member)
	// Because this value needs to be persistent between calls to the
	// VKI progress monitoring callback function and the local instances of 
	// mesh_progress_info aren't persistent, we store the value in the class
	// not the instance, which means we need to reinitialize it for each
	// meshing operation.
 
	static logical m_metering_canceled;

public:
	// Constructor
	mesh_progress_info();

	// ********************************
	//    Query Functions.
	// ********************************

/**
 * Returns a positive integer between 0 and 100 indicating the percentage of the 
 * operation that has completed. The return value 0 indicates this is the first 
 * call to the user callback from the ACIS functionality. The return value 100
 * indicates that this is the last call to the user callback from the ACIS functionality.
 */
	int percentage () const;

/**
 * Returns a positive integer representing the phase of the mesh generation operation.
 * Mesh generation consists of phases 1 to 5 for hybrid meshes, tetrahedral meshes,
 * or triangular surface meshes. Mesh generation consists of phases 1 to 7 for
 * quadrilateral surface meshes.
 */
	int phase () const;

/**
 * Returns the number of nodes currently generated.
 */
	int num_nodes () const;

/**
 * Returns the number of elements currently generated.
 */
	int num_elements () const;

/**
 * User callback functions can query for which ACIS functionality is calling. Typically   
 * this is done on the first call to the callback to customize the dialog window, and 
 * on subsequent calls to branch to the appropriate callback code path, if the same 
 * callback is metering several different ACIS API functions.
 */
	SPA_progress_info_type_id get_type() const;

/**
 * User callback functions can query if progress metering has been canceled.
 * Progress metering can be canceled by the application if, for example, there
 * are very few elements in the input geometry.
 */
	logical progress_metering_canceled() const;

/**
 * User callback functions can query if mesh generation has been canceled.
 * Mesh generation can be canceled by the application if, for example, 
 * mesh generation is calculating too many elements.
 */
	logical meshing_canceled() const;

	// ********************************
	//  Data Member Setting Functions.
	// ********************************

/**
 * @nodoc.
 * For internal use only.  Sets the percentage completion value.
 */
	void set_percentage( int percent );

/**
 * @nodoc.
 * For internal use only.  Sets the mesh generation phase.
 */
	void set_phase ( int phase );

/**
 * @nodoc.
 * For internal use only.  Sets the number of nodes generated.
 */
	void set_num_nodes ( int num );

/**
 * @nodoc.
 * For internal use only.  Sets the number of elements generated.
 */
	void set_num_elements ( int num );

/**
 * User callback functions can cancel the progress metering for one operation.
 * Progress metering can be canceled by the application if, for example, there
 * are very few elements in the input geometry. Progress metering is typically
 * cancelled on the first call to the callback function for a given operation.
 * @param cancel 
 * Specifies that progress metering should be canceled.
 */
	void set_progress_metering_canceled( logical cancel );

/**
 * User callback functions can cancel the mesh generation operation.
 * Meshing can be canceled by the application if, for example, 
 * mesh generation is calculating too many elements. 
 * @param cancel 
 * Specifies that mesh generation should be canceled.
 */
	void set_meshing_canceled( logical cancel );

	// ******************************************
	//  Notify the Application's Progress Meter.
	// ******************************************

/**
 * @nodoc.
 * For internal use only.  Calls the application's callback function.
 */
	int update();

};

 /** @}*/
#endif

