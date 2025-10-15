/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header file for mesh error information return object.
/*******************************************************************/

#if !defined( MESH_ERROR_INFO )
#define MESH_ERROR_INFO
/**
 * @file mesherr_info.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * @addtogroup Mesh
 *  @{
 */
#include <stdio.h>
#include "dcl_vabridge.h"
#include "err_info.hxx"
#include "errorsys.hxx"

// Forward declaration of internal data object.
class mesh_error_info_internal;

/**
 * Enumeration of different types of information avaliable from 
 * an unsuccessful mesh generation operation.
 * @param INCONS_TYPE
 * Information pertaining to inconsistently oriented input triangles.
 * @param TRIZEROANG_TYPE
 * Information pertaining to zero area triangles.
 * @param FREEEDGE_TYPE
 * Information pertaining to triangles with free edges.
 * @param INTERSECT_TYPE
 * Information pertaining to intersecting boundary triangles.
 * @param NODEINSERT_TYPE
 * Information pertaining to uninserted nodes.
 * @param UNREC_TYPE
 * Information pertaining to unrecovered boundary triangles (or edges).
 */
enum mesh_error_info_type 
{ 
	INCONS_TYPE,
	TRIZEROANG_TYPE,
	FREEEDGE_TYPE,
	INTERSECT_TYPE,
	NODEINSERT_TYPE,
	UNREC_TYPE
};

/**
 * Class for storing error information for a 3D Mesh operaion.
 * <br>
 * <b>Role:</b> This class is derived from <tt>error_info</tt>.  In addition to containing an
 * ACIS error message, a <tt>mesh_error_info</tt> object may contain error information from
 * the VKI Mesher. There are two types of information that may be returned from the VKI Mesher:
 * <ul>
 * <li> <b>Error Messages:</b> A single 3D Mesh operation can produce multiple exceptions.
 *      Each exception will produce error information. A <tt>mesh_error_info</tt> object contains
 *      a list containing the information produced by these exceptions. An application can can query 
 *      the number of exceptions, and then for each exception obtain the message strings pertaining
 *      to that specific exception.</li>
 * <li> <b>Mesh Generation Information:</b> If the failed operation was a mesh generation operation
 *      (that is, it was generating a surface, tet, or hybrid mesh) the Mesher often can provide 
 *      additional information about what caused the error. This information can be obtained by 
 *      calling the function <tt>get_mesh_gen_info</tt> with an enumeration value for a particular type
 *      of information. This function returns an integer specifying an array size and an array of 
 *      mesh elements or nodes. If any of these arrays are of non-zero size, this may provide
 *      insight into the cause of the meshing error.
 * </ul>
 */
class DECL_VABRIDGE mesh_error_info : public error_info 
{
	 // Class holding the VKI info about the exception(s).
	mesh_error_info_internal* m_meii;

public:

	/**
	 * Constructor.
	 */
	mesh_error_info();
	/**
	 * Destructor.
	 */
	virtual ~mesh_error_info();

	/**
	 * Error ID.
	 */
	static int id();

	/**
	 * Type ID of this object.
	 */ 
	virtual int type() const; 

	// Exception-related function.

	/**
	 * @nodoc
	 * Adds an exception to the list of exceptions.
	 * This functions is for internal use only.
	 */
	void add_exception( const char* function_name, 
						const char* error_flag, 
						const char* error_msg );

	/**
	 * Gets the number of exceptions in the list of exceptions.
	 */
	int get_num_exceptions( ) const;

	/**
	 * Gets the strings pertaining to a specific exception (specified by the list index).
	 * The returned strings should not be freed by the calling function.
	 */
	void get_exception( int index, 
						const char*& function_name, 
						const char*& error_flag, 
						const char*& error_msg ) const;

	/**
	 * @nodoc
	 * Records additional information returned by a meshing operation.
	 * After a failed mesh generation operation this function stores an 
	 * array of integer values corresponding to problematic triangles
	 * or nodes discovered by the Mesher.
	 * This function is for internal use only.
	 * @param type
	 * (input) type of information stored.
	 * @param num_values
	 * (input) array size.
	 * @param values
	 * (input) array containing triangle or node indicies.
	 */
	void set_mesh_gen_info( mesh_error_info_type type, 
							const int num_values, 
							const int* values );

	/**
	 * Gets additional information returned by a meshing operation.
	 * After a failed mesh generation operation this function returns an 
	 * array of integer values corresponding to problematic triangles
	 * or nodes discovered by the Mesher.
	 * The returned array should not be freed by the calling function.
	 * @param type
	 * (input) type of information requested.
	 * @param num_values
	 * (output) array size.
	 * @param values
	 * (output) array containing triangle or node indicies.
	 */
	void get_mesh_gen_info( mesh_error_info_type type, 
							int& num_values, 
							const int*& values );


};

/*! @} */
#endif

