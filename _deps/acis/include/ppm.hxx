/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _PPM
#define _PPM
#include "dcl_fct.h"
#include "logical.h"
#include "box.hxx"
#include "mesh.hxx"
#include "vtplate.hxx"
class POLYGON;
class POLYGON_VERTEX;
/**
 * @file ppm.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 *! \addtogroup FCTAPI
 *  \brief Declared at <ppm.hxx>
 *  @{
 */
/**
 * The function pointer used by apply method to process each polygon.
 * <br>
 * <i><b>Note:</b> User has to implement the function.</i>
 * <br><br>
 * @param poly
 * polygon pointer.
 * @param SPAptr
 * pointer to args.
 * @param num
 * number of arg.
 */
typedef void (*polygon_func_fn)(POLYGON* poly,void* SPAptr,int num);

/**
 * Defines a collection of polygons.
 * <br>
 * <b>Role:</b> This class defines a mesh for polygons. The polygons in the mesh
 * are kept in a singly linked, <tt>NULL</tt>-terminated list. The mesh maintains pointers
 * to the first and last polygons and also a count of the number of polygons in
 * the mesh.
 * @see POLYGON, POLYGON_POINT_MESH_MANAGER, POLYGON_VERTEX
 */
class DECL_FCT POLYGON_POINT_MESH : public MESH {
 // The polygons in the mesh are kept in a singly linked,
 //  null terminated list.  The mesh maintains pointers to
 //  the first and last polygons, and also a running count.
   POLYGON *first_poly;
   POLYGON *last_poly;
   logical remove_vertex_template;

public :

   // Newly created POLYGON_POINT_MESH has empty polygon list:
/**
 * C++ constructor, creating a <tt>POLYGON_POINT_MESH</tt>.
 */
   POLYGON_POINT_MESH();

/**
 * C++ destructor, deleting a <tt>POLYGON_POINT_MESH</tt>.
 */
   ~POLYGON_POINT_MESH();

/**
 * @nodoc
 */
   void add();

/**
 * @nodoc
 */
   void remove_ref(logical=TRUE);

/**
 * @nodoc
 */
   void set_remove_vertex_template(logical _remove_vertex_template)
   {
	   remove_vertex_template=_remove_vertex_template;
   }
   // Queries about the polygon list:
/**
 * Returns a pointer to the first polygon.
 */
   POLYGON* first();
/**
 * Returns a pointer to the last polygon.
 */
   POLYGON* last();
/**
 * Returns the number of polygons.
 */
   int      count();
/**
 * Returns the size of the polygon point mesh.
 */
   unsigned size();

   // Insertion at begining and end:
/**
 * Adds a polygon as the first polygon in the mesh.
 * <br><br>
 * @param poly
 * polygon.
 */
   void     prepend( POLYGON *poly );
/**
 * Appends a polygon.
 * <br><br>
 * @param poly
 * polygon.
 */
   void     append( POLYGON *poly );
/**
 * Appends the polygons of the given <tt>POLYGON_POINT_MESH</tt>.
 * <br><br>
 * <b>Role:</b> If destroy is <tt>TRUE</tt>, the polygons are transferred, otherwise
 * they are copied.
 * <br><br>
 * @param ppmesh
 * the one to concatenate.
 * @param dest
 * destroy input.
 */
   void     concatenate(POLYGON_POINT_MESH*& ppmesh,int dest);
/**
 * Removes all the polygons.
 */
   POLYGON* remove();

/**
 * Inserts a polygon before an existing one.
 * <br><br>
 * <b>Role:</b> If checking is requested and the key is not found, the polygon
 * will not be inserted. If no checking is requested, the polygon will always
 * be inserted.
 * <br><br>
 * @param poly
 * polygon to insert.
 * @param key
 * existing polygon.
 * @param check
 * check existing polygon.
 */
   void insert( POLYGON *poly, POLYGON *key, logical check = FALSE );

/**
 * Applies a function to all the polygons. The function is called with each polygon, the argument, and flags.
 * <br><br>
 * @param polygon_func
 * function to apply.
 * @param arg
 * pointer to argument.
 * @param num
 * number of args.
 */

   void apply(polygon_func_fn polygon_func, void*arg,int num);

/**
 * Gets the <tt>POLYGON_POINT_MESH</tt> bounding box.
 */
   SPAbox get_box();
/**
 * Returns <tt>TRUE</tt> if the polygon given is in this mesh.
 * <br><br>
 * @param key
 * polygon pointer.
 */
   logical search(POLYGON *key);
/**
 * Transforms a <tt>POLYGON_POINT_MESH</tt>.
 * <br><br>
 * @param T
 * transformation.
 */
   virtual logical transform(SPAtransf const& T);
/**
 * Prints a <tt>POLYGON_POINT_MESH</tt>.
 * <br><br>
 * @param fp
 * output file stream.
 */
   void print(FILE * fp);
};

/*
// tbrv:  needs more doc in the header
*/
/**
 * Defines a <tt>POLYGON</tt>.
 * @see POLYGON_VERTEX, VERTEX_TEMPLATE, POLYGON_POINT_MESH, POLYGON_POINT_MESH_MANAGER, POLYGON_VERTEX
 */
class DECL_FCT POLYGON : public ACIS_OBJECT
{
   friend class POLYGON_POINT_MESH;

   POLYGON *next_poly;

   POLYGON_VERTEX *first_vertex;
   POLYGON_VERTEX *last_vertex;

   VERTEX_TEMPLATE *node_template;

public :

/**
 * C++ constructor, creating a <tt>POLYGON</tt> using the specified parameters.
 * <br><br>
 * @param _node_template
 * vertex template - node template.
 */
   POLYGON( VERTEX_TEMPLATE* _node_template = 0 );
/**
 * C++ destructor, deleting a <tt>POLYGON</tt>.
 */
   ~POLYGON();

/**
 * @nodoc
 */
   void add();

/**
 * @nodoc
 */
   void remove_ref(logical=TRUE);
/**
 * Copies the <tt>POLYGON</tt>.
 */
   POLYGON* copy();
/**
 * Returns the next <tt>POLYGON</tt>.
 */
   POLYGON* next() { return next_poly; }
/**
 * Sets the vertex template.
 * <br><br>
 * @param _node_template
 * vertex template - node template.
 */
   void set_vertex_template( VERTEX_TEMPLATE* _node_template )
   {
       if (node_template)
			node_template->remove();
       if (_node_template){
	   		_node_template->share();
	   		node_template = _node_template;
       }
       else
            node_template = NULL;


   }

   // Query vertices

/**
 * Returns the first <tt>POLYGON</tt>.
 */
   POLYGON_VERTEX* first() { return ( first_vertex ); }
/**
 * Returns the last <tt>POLYGON</tt>.
 */
   POLYGON_VERTEX* last()  { return ( last_vertex ); }
/**
 * Returns the count.
 */
   int             count();

/**
 * Returns the size of the polygon.
 */
   unsigned size();

/**
 * Returns the vertex template.
 */
   VERTEX_TEMPLATE* vertex_template() { return ( node_template ); }

   // Vertex list manipulation
/**
 * Prepends the <tt>POLYGON</tt>.
 * <br><br>
 * @param pv
 * vertex list.
 */
   void            prepend( POLYGON_VERTEX *pv );
/**
 * Appends the <tt>POLYGON</tt>.
 * <br><br>
 * @param pv
 * vertex list.
 */
   void            append( POLYGON_VERTEX *pv );
/**
 * Removes the <tt>POLYGON</tt>.
 */
   POLYGON_VERTEX* remove();
/**
 * Inserts the <tt>POLYGON</tt>.
 * <br><br>
 * @param poly
 * poly.
 * @param key
 * key.
 * @param check
 * check.
 */
   void insert( POLYGON_VERTEX *poly, POLYGON_VERTEX *key,
                logical check = FALSE );
/**
 * Performs a search.
 * <br><br>
 * @param key
 * key.
 */
   logical search(POLYGON_VERTEX *key);
/**
 * Transforms the <tt>POLYGON</tt>.
 * <br><br>
 * @param T
 * transformation.
 */
   void transform(SPAtransf const& T);
/**
 * Prints the <tt>POLYGON</tt>.
 * <br><br>
 * @param fp
 * file.
 */
   void print(FILE *fp);
};

/**
 * Defines a vertex on a polygon.
 * <br>
 * <i><b>Note:</b> This class is not derived from</i> <tt>ENTITY</tt>.
 * @see POLYGON, POLYGON_POINT_MESH_MANAGER
 */

class DECL_FCT POLYGON_VERTEX : public ACIS_OBJECT
{
	friend class POLYGON_POINT_MESH;
	friend class POLYGON;

	POLYGON_VERTEX *next_vertex;

	af_node_instance *node_instance;

	POLYGON_VERTEX() { next_vertex = 0; node_instance = 0; }
	~POLYGON_VERTEX();

	void set_data( af_node_instance *data );
	// This is for compatibility.  Kill it someday!!
	void set_data( void *SPAptr){ set_data ((af_node_instance *)SPAptr);};

public :

   // Create a vertex that references the same data as pre-existing pv:
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param pv
 * vertex pointer.
 */
   POLYGON_VERTEX( POLYGON_VERTEX *pv );
   // Create a vertex that references new data constructed from node_template:
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param node_template
 * vertex template - node template.
 */
   POLYGON_VERTEX( VERTEX_TEMPLATE* node_template );

   // Delete a polygon-vertex and return its data to the free-list, if
   // last reference
/**
 * Destroys itself.
 * <br><br>
 * @param node_template
 * vertex template - node template.
 */
   void destroy( VERTEX_TEMPLATE* node_template );

/**
 * Returns a pointer to the next vertex in a polygon.
 */
   POLYGON_VERTEX* next();

   // Query vertex data
/**
 * Returns the data.
 */
   const af_node_instance* data()
		{return node_instance ; } // Should be private.

   // Set vertex data
/**
 * Sets the position of the vertex.
 * <br><br>
 * @param pt
 * point.
 */
   void set_point( const SPAposition& pt);
/**
 * Sets the normal of the vertex.
 * <br><br>
 * @param vertemp
 * vertex template.
 * @param uv
 * normal.
 */
   void set_normal( VERTEX_TEMPLATE* vertemp, const SPAunit_vector& uv);
/**
 * Sets the parameter data as specified in the array.
 * <br><br>
 * @param vertemp
 * vertex template.
 * @param partok
 * parameter token.
 * @param nodedata
 * node data cell type.
 */
   void set_parameter_data( VERTEX_TEMPLATE* vertemp, parameter_token partok, NODE_DATA_CELL_TYPE* nodedata );

   // Get vertex data - return TRUE if the required data exists
/**
 * Returns the position of the vertex.
 * <br><br>
 * <b>Role:</b> If the return value is <tt>FALSE</tt>, the method could not determine
 * the position.
 * <br><br>
 * @param pt
 * point.
 */
   logical point( SPAposition& pt);
/**
 * Returns the normal of the vertex.
 * <br><br>
 * <b>Role:</b> If the return value is <tt>FALSE</tt>, the method could not determine the normal.
 * <br><br>
 * @param vertemp
 * vertex template.
 * @param uv
 * normal.
 */
   logical normal( VERTEX_TEMPLATE* vertemp, SPAunit_vector& uv);
/**
 * Gets the parameter data of the node and returns it in the array.
 * <br><br>
 * @param vertemp
 * vertex template.
 * @param partok
 * parameter token.
 * @param nodedata
 * node data cell type.
 */
   logical get_parameter_data( VERTEX_TEMPLATE* vertemp, parameter_token partok, NODE_DATA_CELL_TYPE* nodedata);

/**
 * Prints the polygon vertex.
 * <br><br>
 * @param vt
 * vertex template.
 * @param to
 * file to print to.
 */
   void print( VERTEX_TEMPLATE *vt = 0, FILE *to = stderr );

};

/* @} */

#endif
