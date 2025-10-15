/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _VTPLATE
#define _VTPLATE
#include <stdio.h>
#include "dcl_fct.h"
#include "logical.h"
#include "entity.hxx"
#include "usecount.hxx"
#include "nodedata.hxx"
/**
 * @file vtplate.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup FCTAPI
 *  \brief Declared at <vtplate.hxx>
 *  @{
 */
//******************************************************
// The VERTEX_TEMPLATE defines the layout of various data
//	items stored with a 'node' of a mesh.
//
// Every node contains its own coordinates.
// In addition, a node has a pointer to an array of
//	additional fields. The sizes of each field are as defined
//	by af_get_parameter_token_cell_count
//
//******************************************************

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(VERTEX_TEMPLATE, FCT )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

extern DECL_FCT int VERTEX_TEMPLATE_TYPE ;

/*
// tbrv
*/
/**
 * @nodoc
 */
#define VERTEX_TEMPLATE_LEVEL 1
/**
 * Creates parameter templates.
 * <br>
 * <b>Role:</b> This entity enables applications to request attachment of any
 * of the following data to the nodes of a mesh provided the <tt>MESH_MANAGER</tt>
 * is capable of handling the request.
 * <br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-  coordinate data<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-  normal to the surface<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-  uv parameter on the surface<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-  partial derivatives in u and v on the surface<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-  magnitude of the partial derivatives in u and v on the surface<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-  RGB color<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-  transparency<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-  texture<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-  pointer to user defined data<br>
 * <br>
 * Nonglobal indexed mesh managers and coordinate mesh managers create meshes
 * on a face basis and so can be designed to attach this type of data to the
 * nodes. Global indexed mesh managers share node data between faces and so
 * face related information is not stored with this type of mesh manager.
 * <br>
 * By defining a default <tt>VERTEX_TEMPLATE</tt> or attaching one to a body, lump, shell,
 * or face, the application program can request that certain data be attached
 * to a nodes of the mesh corresponding to that entity. A particular <tt>MESH_MANAGER</tt>
 * must honor this request by allocating storage and wherever possible initializing
 * values for this data.<br>
 * The currently active vertex template defines the data attached to a node of a
 * polygon on a face and is the vertex template on the face, shell, lump, body,
 * or default in that order. The only common data for all nodes is the coordinate.
 * The normal, uv parameter, uv partial derivatives, and magnitudes of the uv
 * partial derivatives can be calculated and stored automatically by the
 * <tt>MESH_MANAGER</tt> if so requested. Space for the RGB color, transparency, texture,
 * and pointer must be allocated by the <tt>MESH_MANAGER</tt> but the values must be
 * initialized by the application program.
 * @see INDEXED_MESH_MANAGER, POLYGON, indexed_polygon
 */
class DECL_FCT VERTEX_TEMPLATE : public ENTITY
{

	af_node_mapping *node_mapping;
// STI jmb: Standard use count implementation
// 	int use_count ;
//
//    void use() { if(this ) use_count++ ; }
// STI jmb: end

public :

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( VERTEX_TEMPLATE, FCT )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:

	/**
	 * @nodoc
	 */
	LOOKUP_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

	// Create.
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 * <br><br>
 * <b>Role:</b> The allocation constructor is used primarily by restore.
 * Applications should call this constructor only with the overloaded new operator
 * inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new VERTEX_TEMPLATE</tt>), because
 * this reserves the memory on the heap, a requirement to support roll back and
 * history management.
 * <br>
 * After this constructor is called, the method <tt>allocate_node_mapping</tt> must be
 * called to create a new <tt>af_node_mapping</tt> instance.
 * <br><br>
 * @param data
 * pointer to data.
 */
	VERTEX_TEMPLATE( void *data = 0 ) ; // Should be private (public for restore)
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded
 * new operator inherited from the <tt>ENTITY</tt> class (for example,
 * <tt>x=new VERTEX_TEMPLATE(...)</tt>), because this reserves the memory on the heap, a
 * requirement to support roll back and history management.
 * <br><br>
 * @param n_tokens
 * number of tokens.
 * @param tokens
 * tokens.
 */
	VERTEX_TEMPLATE( int n_tokens, parameter_token tokens[] ) ;

	// Define the assignment operator to fix up the af_node_mapping when the
	// VERTEX_TEMPLATE is copied during backup
/**
 * Implements an assignment operator, which makes a copy of a <tt>VERTEX_TEMPLATE</tt>.
 * <br><br>
 * @param vertemp
 * vertex template.
 */
	VERTEX_TEMPLATE& operator=(const VERTEX_TEMPLATE& vertemp);
//STI mka
/**
 * Returns indication of whether this vertex template is deletable.
 */
        virtual logical deletable() const;
/**
 * Allocates an <tt>af_node_mapping</tt>.
 * <br><br>
 * <b>Role:</b> This must be called after a new <tt>VERTEX_TEMPLATE</tt> is constructed.
 */
	void allocate_node_mapping();
	// Modify
/**
 * Changes the tokens defined in a <tt>VERTEX_TEMPLATE</tt>.
 * <br><br>
 * @param n_tokens
 * number of tokens.
 * @param tokens
 * tokens.
 */
	void modify( int n_tokens, parameter_token tokens[] ) ;

	// Test to see if fully defined.
/**
 * Determines whether the <tt>VERTEX_TEMPLATE</tt> is fully defined.
 */
	logical valid() const;

	// Queries
/**
 * Returns TRUE if the <tt>VERTEX_TEMPLATE</tt> contains a <tt>NORMAL_TOKEN</tt>.
 */
	logical normal_defined() const
				{ return node_mapping->defined(NORMAL_TOKEN) != 0;};
/**
 * Determines whether the <tt>VERTEX_TEMPLATE</tt> contains the specified parameter token.
 * <br><br>
 * @param id
 * token ID.
 */
	logical parameter_defined( parameter_token id ) const
				{ return node_mapping->defined(id) != 0;};
/**
 * Returns the number of extra cells in the <tt>VERTEX_TEMPLATE</tt>.
 */
	int extra_cells() const
				{ return node_mapping->extra_cells();};
/**
 * Creates a new instance of the node.
 */
	af_node_instance *new_instance();

/**
 * Gets the parameter mapping for the <tt>VERTEX_TEMPLATE</tt>.
 */
	const af_node_mapping *get_mapping(){ return node_mapping;};
	// Operations
/**
 * Determines if two <tt>VERTEX_TEMPLATE</tt>s are the same.
 * <br><br>
 * @param vertemp
 * vertex template.
 */
	logical same( VERTEX_TEMPLATE* vertemp) const ;
// STI jmb: Standard use count implementation
// 	VERTEX_TEMPLATE* share() { use() ; return ( this ) ; }
/**
 * Increments a <tt>VERTEX_TEMPLATE</tt>'s use count.
 */
	VERTEX_TEMPLATE* share();
    /**
     * @nodoc
     */
    USE_COUNTED_DECL
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// STI jmb: end
/**
 * Copies a <tt>VERTEX_TEMPLATE</tt>.
 */
	VERTEX_TEMPLATE* copy() const ;
/**
 * Merges tokens from another <tt>VERTEX_TEMPLATE</tt> into this <tt>VERTEX_TEMPLATE</tt>.
 * <br><br>
 * @param vertemp
 * other vertex template.
 */
	void merge(VERTEX_TEMPLATE* vertemp);
/**
 * Notifies the <tt>VERTEX_TEMPLATE</tt> that its owning <tt>ENTITY</tt> is about to be merged with given entity.
 * <br><br>
 * <b>Role:</b> The application has the chance to delete or otherwise modify the
 * attribute. After the merge, this owner will be deleted if the logical deleting
 * owner is <tt>TRUE</tt>, otherwise it will be retained and other entity will be deleted.
 * The default action is to do nothing. This function is supplied by the
 * application whenever it defines a new attribute, and is called when a merge occurs.
 * <br><br>
 * @param other
 * given entity.
 * @param delete_other
 * deleting owner.
 */
	void merge_owner( ENTITY* other, logical delete_other );
/**
 * Notifies the <tt>VERTEX_TEMPLATE</tt> that its owner is about to be split into two parts.
 * <br><br>
 * <b>Role:</b> The application has the chance to duplicate or otherwise modify
 * the attribute. The default action is to do nothing. This function is supplied
 * by the application whenever it defines a new attribute, and is called when a split occurs.
 * <br><br>
 * @param new_entity
 * new_entity.
 */
	void split_owner( ENTITY* new_entity );

/**
 * Gets the parameter location and site of the parameter for a given token in the <tt>VERTEX_TEMPLATE</tt>.
 * <br><br>
 * @param token
 * token ID.
 * @param offset
 * offset of value.
 * @param count
 * number of cells.
 */
	logical locate(parameter_token token, int &offset, int &count) const
		{ return node_mapping->locate(token,offset,count) != 0; }

	// Output
/**
 * Prints the contents of the <tt>VERTEX_TEMPLATE</tt> on the specified file.
 * <br><br>
 * @param fp
 * file pointer.
 */
	void print( FILE* fp = stderr ) const ;

} ;

/* @} */

#endif
