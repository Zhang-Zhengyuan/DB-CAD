#pragma once
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef api_offset_edges_h
#define api_offset_edges_h

class FACE;
class EDGE;
class offset_edges_on_faces_options_impl;
class ofst_edges_input_handle;

#include "dcl_hlc.h"
#include "mmgr.hxx"
#include "api.hxx"

/**
 * @file api_offset_edges.h
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISQUERIES
 * \addtogroup
 * @{
 */

/**
* Class <tt>offset_edges_on_faces_options</tt> controls the user
* adjustable options for api_offset_edges_on_faces. The main
* adjustable parameters are (1) default Edge Offset (offset value to be applied to every edge)
* (2) custom Edge Offset Values (specific offset value defined for the given Edge)
* and (3) piecewise Linear (user can provide discreation Sag value if this option is selected).
**/
class DECL_HLC offset_edges_on_faces_options : public ACIS_OBJECT
{
	offset_edges_on_faces_options_impl* m_impl;
public:
	/**
	* Allows the user to control the offset distance from a given edge. (Custom edge offset)
	**/
	void set_offst_distance(EDGE *edge, double offset);
	/**
	* Allows the user to control the offset distance from all edges in the input. (Default edge offset)
	**/
	void set_offst_distance(double offset);
	/**
	* @nodoc.
	**/
	void set_piecewiselinear(bool value);
	/**
	* @nodoc.
	**/
	void set_discr_sag(double value);
	/**
	* Returns the offset which is in effect for the given edge
	**/
	double get_offst_distance(const EDGE *edge) const;
	/**
	* Returns the default offset value
	**/
	double get_offst_distance() const;
	/**
	* checks the value of piecewiseLinear , whether set to true or false
	**/
	bool isPiecewiseLinear() const;
	/**
	* Returns the discreation Sag value
	**/
	double get_discrSag_val() const;

	/**
	* checks CustomEdgeOfstFlag value, whether set to true or false
	**/
	bool getcustomEdgeOfstFlag() const;
	/**
	* @nodoc.
	**/
	offset_edges_on_faces_options();
	/**
	* @nodoc.
	**/
	offset_edges_on_faces_options(const offset_edges_on_faces_options &);
	/**
	* @nodoc.
	**/
	const offset_edges_on_faces_options& operator=(const offset_edges_on_faces_options &);
	/**
	* @nodoc.
	**/
	~offset_edges_on_faces_options();
	/**
	* @nodoc. internal use only.
	**/
	const offset_edges_on_faces_options_impl* get_impl() const;
};

/**
 * The API <tt>api_offset_edges_on_faces</tt> creates a wire body whose edges are the offset of edges on faces for all edges of Edge List.
 * <br><br>
 * <b>Role:</b> This API creates a wire body by offseting edges where the offset
 * edges lie on faces which are topologically connected to the input. The offset
 * distance is shortest distance between input Edge and Offset Edge.
 * <br><br>
 * <b>Limitations:</b>
 * <ul>
 * <li>Offsets beyond a sheet boundary are not handled.</li>
 * <li>Negative offset distances are currently not allowed.</li>
 * <li>Depending on the offset distance, the topology of the resulting offset wire edges may differ from the input edges.</li>
 * </ul>
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param ofst_edges_input_handle
 * Input handle for offset edges on faces.
 * @param offset_edges_on_faces_options
 * controls the user passing options such as default offset value,custom edge offset values.
 * @param out_wire_body
 * offset wire body.
 * @param ao
 * ACIS options such as versioning and journaling.
**/
DECL_HLC
outcome api_offset_edges_on_faces
(
	ofst_edges_input_handle* ofstedgh,
	BODY                    *&out_wire_body,
	const offset_edges_on_faces_options *offedgeopts,
	AcisOptions             *ao = NULL
);
/** @} */
#endif

