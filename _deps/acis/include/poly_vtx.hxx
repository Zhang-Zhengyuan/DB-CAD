/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _POLY_VTX_HXX_
#define _POLY_VTX_HXX_
#include "dcl_fct.h"
#include "nodedata.hxx"
#include "vtplate.hxx"
#include "position.hxx"
#include "unitvec.hxx"
#include "param.hxx"
/**
* @file poly_vtx.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup FCTAPI
 *  \brief Declared at <poly_vtx.hxx>
 *  @{
 */
/**
 * The <tt>polygon_vertex</tt> class is used to store the data at a polygon vertex.
 * @see INDEXED_MESH, indexed_polygon, SPApar_pos, SPAposition, SPAunit_vector
 */
class DECL_FCT polygon_vertex : public ACIS_OBJECT
{
private:
	SPAposition			m_Position;
	SPAunit_vector			m_Normal;
	SPApar_pos				m_SurfaceUV;
	double				m_Color[3];
	af_node_instance*	aux_info;

public:
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 */
	polygon_vertex();

/**
 * C++ destructor, deleting a polygon_vertex.
 */
	~polygon_vertex();

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param pos
 * position.
 * @param uv
 * unit vector.
 * @param parpos
 * parameter position.
 */
	polygon_vertex(const SPAposition& pos, const SPAunit_vector& uv, const SPApar_pos& parpos);

/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param polver
 * polygon vertex.
 */
	polygon_vertex(const polygon_vertex& polver);

/**
 * Compares the current instances of the polygon vertex with the one supplied as the argument.
 * <br><br>
 * @param polver
 * polygon vertex.
 */
	polygon_vertex& operator=(const polygon_vertex&);

/**
 * Return the position.
 */
	const SPAposition& get_position() const { return m_Position; }
/**
 * Return the normal polygon vertex.
 */
	const SPAunit_vector& get_normal() const { return m_Normal; }

/**
 * Returns the uv parameter.
 */
	const SPApar_pos& get_uv() const { return m_SurfaceUV; }

/**
 * Return the polygon vertex color.
 */
	const double* get_color() const { return m_Color; }

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
	logical get_parameter_data ( VERTEX_TEMPLATE* vertemp, parameter_token partok, NODE_DATA_CELL_TYPE* nodedata) const;

/**
 * Sets the position.
 * <br><br>
 * @param pos
 * position.
 */
	void set_position(const SPAposition& pos) { m_Position = pos; }

/**
 * Set the normal polygon vertex.
 * <br><br>
 * @param norm
 * unit vector.
 */
	void set_normal(const SPAunit_vector& norm) { m_Normal = norm; }

/**
 * Set the uv position.
 * <br><br>
 * @param uv
 * uv parameter position.
 */
	void set_uv(const SPApar_pos& uv) { m_SurfaceUV = uv; }

/**
 * Set the uv parameter position.
 * <br><br>
 * @param u
 * u parameter.
 * @param v
 * v parameter.
 */
	void set_uv(double u, double v) { m_SurfaceUV.u = u; m_SurfaceUV.v = v; }

/**
 * Sets the polygon vertex color.
 * <br><br>
 * @param color
 * color to set.
 */
	void set_color(const double* color) { if (color != NULL){
												m_Color[0] = color[0];
												m_Color[1] = color[1];
												m_Color[2] = color[2];
											}
										 }

/**
 * Set the data for the polygon vertex.
 * <br><br>
 * @param pos
 * position.
 * @param uv
 * unit vector.
 * @param parpos
 * parameter position.
 */
	void set_data(const SPAposition& pos, const SPAunit_vector& uv, const SPApar_pos& parpos);

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

/**
 * Reverse the polygon vertex.
 */
/**
 * @nodoc
 */
	void reverse() { m_Normal = (-m_Normal); }


/**
 * @nodoc
 */
    void operator*=(const SPAtransf& t);
};

/* @} */
#endif /* _POLY_VTX_HXX_ */
