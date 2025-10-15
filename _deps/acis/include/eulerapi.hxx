/* ORIGINAL: acis2.1/kernapi/api/kernapi.hxx */
/* $Id: eulerapi.hxx,v 1.11 2002/03/08 18:02:04 ywoo Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for api routines.

// For details of how to call each routine, consult the documentation
// for individual routines.

#if !defined( EULERAPI_HXX )
#define EULERAPI_HXX

class BODY;
class ENTITY_LIST;
class FACE;
class TEDGE;
class decomp_options;

#include "dcl_eulr.h"
#include "eulerapi.hxx"
#include "api.hxx"

/**
 * \defgroup EULERAPI Euler Operations
 *
 * Perform topological manipulations on a solid body.
 */

/**
* @file eulerapi.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup EULERAPI
 *
 * @{
 */

/**
* Initializes the Euler Operations Component library.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
* <br><br>
**/
DECL_EULR outcome api_initialize_euler_ops();

/**
* Terminates the Euler Operations Component library.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
* <br><br>
**/
DECL_EULR outcome api_terminate_euler_ops();

/**
* Combines two given bodies into one body that contains all the lumps and wires 
* from the original bodies.
* <br><br>
* <b>Role:</b> The lumps and wires of the <tt>from_body</tt> are combined with those of
* the <tt>to_body</tt>, then the <tt>from_body</tt> is deleted.
* <br><br>
* The <tt>to_body</tt> becomes the combined body, the <tt>from_body</tt> is deleted. This is
* equivalent to the unite function on disjoint objects with no check for
* disjointedness. It is much faster than the unite function and can handle wires.
* <br><br>
* <b>Errors:</b> The pointer to <tt>from_body</tt> or <tt>to_body</tt> is <tt>NULL</tt> 
* or does not point to a <tt>BODY</tt>.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral 
* <br><br>
* @param from_body
* first body (deleted).
* @param to_body
* second body (kept).
* @param ao
* acis options.
**/
DECL_EULR outcome api_combine_body(
			BODY *from_body,				// body containing lumps to be merged
								// into the second
			BODY *to_body,				// body to receive merged lump list
		 AcisOptions* ao = NULL
	);
/*
* @note 将两个body中的所有lump与wire合并到一个body中
* @return API执行的结果
* @param from_body
* 第一个body（合并后将被删除）
* @param to_body
* 第二个body（合并结果将会保存在该body中）
* @param ao
* Acis选项
*/
DECL_EULR outcome gme_api_combine_body(
			BODY *from_body,				// body containing lumps to be merged
								// into the second
			BODY *to_body,				// body to receive merged lump list
		 AcisOptions* ao = NULL
	);
/**
* Separates the given body into a list of bodies each representing a single lump or wire.
* <br><br>
* <b>Role:</b> Each lump and wire in the body is extracted into a separate body.
* Shared geometries are duplicated to avoid sharing across bodies. The first lump
* or the first wire (if there are no lumps) is left in the old body and all others
* are transferred to new bodies. Pointers to all bodies are placed in order of the
* original lumps and then wires in the output array which is created on the heap.
* The first entry in the array is always the original body.
* <br><br>
* If the input body has none or only one lump or wire, the original body is not
* changed and is the only body returned.
* <br><br>
* <b>Errors:</b> Pointer to body is <tt>NULL</tt> or not to a <tt>BODY</tt>.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral 
* <br><br>
* @param in_body
* body to be separated.
* @param nbody
* returned number of bodies.
* @param bodylist
* returned array of body pointers (free store).
* @param ao
* acis options.
**/
DECL_EULR outcome api_separate_body(
			BODY *in_body,				// body with multiple lumps
			int &nbody,				// number of bodies returned
			BODY **&bodylist,			// array of pointers to single-lump
								      // bodies
		 AcisOptions* ao = NULL
	);
/*
* @note 将给定body中的每一个lump，每一个wire都拆分成单独的body，其中第一个lump（或第一个wire若给定body没有lump）还在原始的body中
* @return API执行的结果
* @param in_body
* 给定的body
* @param nbody
* in_body中lump与wire的总数
* @param bodylist
* in_body拆分后的结果数组
* @param ao
* Acis选项
*/
DECL_EULR outcome gme_api_separate_body(
			BODY *in_body,				// body with multiple lumps
			int &nbody,				// number of bodies returned
			BODY **&bodylist,			// array of pointers to single-lump
								      // bodies
		 AcisOptions* ao = NULL
	);
/**
* Converts the face lists of each shell in the body into a hierarchy of subshells 
* with spatial locality.
* <br><br>
* <b>Role:</b> This function expands the body shells. A successful <tt>outcome</tt> 
* indicates that the process has run to completion, even if nothing has been changed.
* <br><br>
* <b>Errors:</b> Pointer to body is <tt>NULL</tt> or not to a <tt>BODY</tt>.
* <br><br>
* <b>Limitations:</b> May have no effect on shells with few faces.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
* <br><br>
* @param body
* body to be expanded.
* @param ao
* acis options.
**/
DECL_EULR outcome api_expand_body(
		BODY *body,				// body with shells to be expanded
								// (i.e. subshell structure added)
		AcisOptions* ao = NULL
	);

/**
 * @nodoc
 */
DECL_EULR outcome api_expand_body(
		BODY *body,				// body with shells to be expanded
								// (i.e. subshell structure added)
		decomp_options* opts,	// options object for decomposition
		AcisOptions* ao = NULL
	);
/*
* @note 按照一定的策略将body中每个shell的face列表转换为具有空间局部性的层次化subshell结构
* @return API执行的结果
* @param body
* 给定的待expand的body
* @param ao
* ACIS选项
*/
DECL_EULR outcome gme_api_expand_body(
	BODY* body,				// body with shells to be expanded
							// (i.e. subshell structure added)
	AcisOptions* ao = NULL
);
/**
* Removes subshell structures from the shells of the given body and places the 
* faces in a single list for each shell.
* <br><br>
* <b>Role:</b> There is no action when there are no subshells. The body shells
* are flattened. A successful <tt>outcome</tt> indicates that the process has run to
* completion, even if nothing has been changed.
* <br><br>
* <b>Errors:</b> Pointer to body is <tt>NULL</tt> or not to a <tt>BODY</tt>.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
* <br><br>
* @param body
* body to be flattened.
* @param ao
* acis options.
**/
DECL_EULR outcome api_flatten_body(
			BODY *body,				// body with shells to be flattened
								// (i.e. subshells removed)
		 AcisOptions* ao = NULL
	);

/*
* @note 从给定body的shell中删除subshell树状结构，并将一个shell下所属的面合并在每个shell的face单个列表中。
* @return API执行的结果
* @param body
* 给定的待flatten的body
* @param ao
* ACIS选项
*/
DECL_EULR outcome gme_api_flatten_body(
	BODY* body,				// body with shells to be flattened
						// (i.e. subshells removed)
	AcisOptions* ao = NULL
);

/**
* Unshare geometric entities shared across multiple bodies.   
* <br><br>
* <b>Role:</b> ACIS does not support sharing of geometric entities (i.e. geometry
* objects derived from <tt>ENTITY</tt> class) across multiple bodies. Sharing geometric entities across 
* multiple bodies will make them insane. This API creates independent instances of such geometric 
* entities shared across the given set of bodies.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* <b>Journal: </b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
* <br><br>
* @param bodies
* List of bodies that share geometric entities across themselves
* @param ao
* acis options.
**/
DECL_EULR outcome
api_unshare_body_geometry( ENTITY_LIST const &bodies,
						   AcisOptions const *ao = NULL );
/*
* @note
* 不允许不同的BODY中有共享的几何属性，拓扑结构的几何属性
* 指FACE中的SURFACE，EDGE中的CURVE，VERTEX中的APOINT。若
* 有不同的BODY它们中的几何属性指针指向同一个位置，需要将
* 几何属性进行深复制，然后使得这些指针指向不同的位置。这
* 种操作被称为对拓扑结构的unshare操作。
* @return API执行的结果
* @param bodies
* 需要进行unshare操作的body列表
* @param ao
* Acis选项
*/
DECL_EULR outcome
gme_api_unshare_body_geometry( ENTITY_LIST const &bodies,
						   AcisOptions const *ao = NULL );
/** @} */
#endif
