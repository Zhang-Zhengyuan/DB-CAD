/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef FACET_OPTIONS_HXX
#define FACET_OPTIONS_HXX

#include "mmgr.hxx"
#include "dcl_fct.h"

#include <ostream>

class facet_options_internal;
class facet_options_expert;
class ENTITY_LIST;

/**
 * @file facet_options.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup FCTAPI
 *  \brief Declared at <facet_options.hxx>
 * @{
 */

/**
 * Interface class for representing user-specified faceting requirements.
 * <br>
 * <b>Role:</b> This class serves as a base class for the workflow-specific facet-option
 * classes, including @href facet_options_visualization, @href facet_options_precise, and 
 * @href facet_options_expert. Instances of these classes are utilized by @href api_facet_entity, 
 * @href api_facet_entities, @href api_facet_unfaceted_entity, @href api_facet_unfaceted_entities, 
 * and @href FacetEntity.
 * <br><br>
 * <b>Note:</b> Each of the derived classes possess the same set of parameters. The differences
 * among the derived classes are their interfaces and default parameter values. These provide
 * very different behaviors for the different facet option classes.
 * <br><br>
 * For additional information refer to the Technical Article <i>Facet Options</i>.
 * @see facet_options_visualization, facet_options_precise, facet_options_expert.
 **/
class DECL_FCT facet_options : public ACIS_OBJECT
{
protected:
	facet_options_internal* m_internal_options;
	facet_options_expert*   m_expert_options;

	facet_options(){}
	facet_options( facet_options const&);

public:
	/**
	* @nodoc
	**/
	// internal use only.  If you use this copy constructor you will be unable to access any subclass methods
	facet_options( facet_options_internal* internal_opts );

	virtual ~facet_options();

	/**
	 * @nodoc
	 */
	facet_options_internal* get_internal_options() const; // internal use only

	void output(std::ostream& os) const;

	/**
	* Returns a <tt>facet_options_expert</tt> object that is based upon <tt>this</tt> object.
	* <br><br>
	* <b>Role:</b> The <tt>facet_options_visualization</tt>
	* and <tt>facet_options_precise</tt> classes have been optimized for particular workflows.
	* Users willing to take more risk for the sake of additional customizability may use the
	* <tt>get_expert_options</tt> method to convert any of the workflow-based options into an expert
	* options object.  The expert options object allows the customization of all parameters.
	*
	* The expert options object reference obtained by this method is essentially the same options object,
	* with more controls exposed.
	*/
	facet_options_expert& get_expert_options();
};

/**
 * Face quality options used by <tt>facet_options_visualization</tt>.
 * <br>
 * <b>Role:</b> Specifies the %surface tolerance and normal tolerance when faceting faces.
 * @param coarse
 * The requested %surface tolerance is 0.004 times the diagonal of the body's bounding box and the normal tolerance is 40 degrees.
 * @param medium_coarse
 * The requested %surface tolerance is 0.002 times the diagonal of the body's bounding box and the normal tolerance is 30 degrees.
 * @param medium_fine
 * The requested %surface tolerance is 0.001 times the diagonal of the body's bounding box and the normal tolerance is 20 degrees.
 * @param fine
 * The requested %surface tolerance is 0.0005 times the diagonal of the body's bounding box and the normal tolerance is 10 degrees.
 * @param other_face_quality
 * Do not use as an input setting.  Only returned by queries.
 */
enum DECL_FCT face_quality_level
{
	coarse=0,				// .004		* (diagonal of bounding box )
	medium_coarse=1,		// .002		* (diagonal of bounding box )
	medium_fine=2,			// .001		* (diagonal of bounding box )
	fine=3,					// .0005	* (diagonal of bounding box )
	other_face_quality		// do not use this as input setting.  Only used for output.
};

/**
 * Edge quality options used by <tt>facet_options_visualization</tt>.
 * <br>
 * <b>Role:</b> Specifies the positional tolerance and angular tolerance when faceting edges. 
 * These tolerances are specified as a fraction of the positional and angular tolerances
 * used to facet faces.
 * @param medium
 * Edges are to be faceted using the same tolerances as faces.
 * @param better
 * Edges are to be faceted using tolerances equal to one half the face tolerances.
 * @param best
 * Edges are to be faceted using tolerances equal to one quarter the face tolerances.
 * @param other_edge_quality
 * Do not use as an input setting.  Only returned by queries.
 */
enum DECL_FCT edge_quality_level
{
	medium=0,				// edges faceted to 1*( face surface and normal tol )
	better=1,				// edges faceted to .5*( face surface and normal tol )			
	best=2,					// edges faceted to .25*( face surface and normal tol )
	other_edge_quality		// do not use this as input setting.  Only used for output.
};

/**
 * A facet-options class that is appropriate to workflows demanding efficient visualization.
 * <br>
 * <b>Role:</b> This class is designed to meet the needs of applications for which
 * faceting performance is more important than precision, although not at the expense
 * of representing individual features belonging to an entity.
 * <br><br>
 * The interface to this facet options classes allows you to specify the <i>Face Quality</i>
 * and <i>Edge Quality</i>. These two quantities specify the positional and angular tolerances
 * used when faceting faces and edges. The default value for <i>Face Quality</i> is 
 * <tt>medium_coarse</tt> and the default value for <i>Edge Quality</i> is <tt>better</tt>.
 * <br><br>
 * Do not derive from this class.
 * <br><br>
 * For additional information on <tt>facet_options_visualization</tt> objects refer to the Technical Article <i>Facet Options</i>.
 * @see facet_options, facet_options_precise, facet_options_expert.
 **/
class DECL_FCT facet_options_visualization : public facet_options
{
	facet_options_visualization( facet_options_visualization const&);
public:

	/**
	* C++ constructor, creating a <tt>facet_options_visualization</tt> object and
	* populating it with its default settings.
	* <br><br>
	* <b>Role:</b> The default quality-level settings are <tt>medium_coarse</tt> for faces and
	* <tt>better</tt> for edges.
	*/
	facet_options_visualization();

	/**
	* @nodoc
	**/
	~facet_options_visualization();

	/**
	* Specifies the quality level for faceting faces.
	* <br><br>
	* <b>Role:</b> Possible settings are <tt>coarse</tt>, <tt>medium_coarse</tt>, <tt>medium_fine</tt>,
	* and <tt>fine</tt>.  The default value is <tt>medium_coarse</tt>.
	*/
	void set_face_quality( face_quality_level face_quality );

	/**
	* Returns the face quality setting contained in this options object.
	*/
	face_quality_level	get_face_quality() const;

	/**
	* Specifies the quality level for faceting edges.
	* <br><br>
	* <b>Role:</b> Possible settings are <tt>medium</tt>, <tt>better</tt>, and <tt>best</tt>. 
	* The default value is <tt>better</tt>.
	*/
	void set_edge_quality( edge_quality_level edge_quality );

	/**
	* Returns the edge quality setting contained in this options object.
	*/
	edge_quality_level	get_edge_quality() const;

};

/**
* Polygon-type options used by 
* <tt>facet_options_precise</tt> and <tt>facet_options_expert</tt>.
* @param all_triangles
* The mesh should consists of triangles only.
* @param triangles_on_fringe_only
* The mesh should have triangles only on its fringe.
* @param no_triangles_full_grid
* The mesh should contain no triangles and the grid should be extended to face edges.
* @param no_triangles_quad_tree
* The mesh should contain no triangles.
**/
enum DECL_FCT polygon_type
{
	all_triangles,
	triangles_on_fringe_only,
	no_triangles_full_grid,
	no_triangles_quad_tree,
	bridges
};


/**
 * A facet-options class that is appropriate to workflows demanding precise meshes.
 * <br>
 * <b>Role:</b> This class is designed to meet the needs of analysis-type applications in which
 * mesh quality is more important than performance. Some performance is sacrificed to provide maximum 
 * robustness and repair minor flaws in the resulting mesh.
 * <br><br>
 * The interface to this facet options class allows you to specify the following parameters:
 * <table width = 100%>
 * <tr width = 100%>
 * <td valign = "top" width = 25%><tt>normal_tolerance</tt></td>
 * <td valign = "top">Specifies the maximum angle (in degrees) between %surface normals 
 * at points on a facet. 
 * The default <tt>normal_tolerance</tt> value is 0, which means the normal tolerance is ignored.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>surface_tolerance</tt></td>
 * <td valign = "top">Specifies the maximum allowed distance between the %surface and the mesh. 
 * The default <tt>surface_tolerance</tt> value is -1, which means the %surface tolerance is ignored.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>max_edge_length</tt></td>
 * <td valign = "top">Constrains the maximum length of a side of a grid cell in object space. 
 * More precisely, the maximum edge length value constrains the maximum length of a diagonal of a grid cell, 
 * because the length of a grid cell diagonal provides an upper limit for the edge length of a facet. 
 * Therefore, this parameter also constrains the maximum edge length of a facet.
 * The default <tt>max_edge_length</tt> value is 0, which means the maximum edge length is ignored.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>min_grid_lines_u</tt></td>
 * <td valign = "top">Specifies the minimum number of <i>u</i> grid lines (i.e., grid lines in the <i>u</i>-direction). 
 * The default <tt>min_grid_lines_u</tt> value is 0. </td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>min_grid_lines_v</tt></td>
 * <td valign = "top">Specifies the minimum number of <i>v</i> grid lines (i.e., grid lines in the <i>v</i>-direction). 
 * The default <tt>min_grid_lines_v</tt> value is 0. </td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>aspect_ratio</tt></td>
 * <td valign = "top">Specifies the maximum ratio of the long side to the short side of a grid cell in 3D space. 
 * The aspect ratio value does not guarantee that triangles will have a particular aspect ratio; 
 * it applies only to the aspect ratio of grid cells. If the value of the grid aspect ratio parameter is 
 * less than or equal to 0.0, the grid aspect ratio is ignored. If the value of the grid aspect ratio parameter 
 * is greater than 0.0 and less than or equal to 1.0, a value of 1.0 is used.
 * The default <tt>grid_aspect_ratio</tt> value is 0, which means the grid aspect ratio is ignored.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>polygon_type</tt></td>
 * <td valign = "top">Specifies the desired polygon shapes for the output mesh. 
 * The default <tt>polygon_type</tt> value is <tt>all_triangles</tt>, which means the mesh should
 * consist entirely of triangular facets.</td>
 * </tr>
 * </table> 
 * <br><br>
 * Do not derive from this class.
 * <br><br>
 * For additional information on <tt>facet_options_precise</tt> objects refer to the Technical Article <i>Facet Options</i>.
 * @see facet_options, facet_options_visualization, facet_options_expert.
 **/
class DECL_FCT facet_options_precise: public facet_options
{
	facet_options_precise( facet_options_precise const&);
public:
	/**
	* C++ constructor, creating a <tt>facet_options_precise</tt> object and
	* populating it with its default settings.
	*/
	facet_options_precise();

	/**
	* @nodoc
	**/
	~facet_options_precise();
	
	// surface and normal tolerance
	// will be adhered to or a reason for failing will be given.
	
	/**
	* Returns the user-specified %surface tolerance.
	*/
	double get_surface_tolerance() const;

	/**
	* Specifies the desired %surface tolerance for faceting faces.
	*/
	void set_surface_tolerance( double surf_tol );

	/**
	* Returns the user-specified normal tolerance.
	*/
	double get_normal_tolerance() const;

	/**
	* Specifies the desired normal tolerance for faceting faces.
	*/
	void set_normal_tolerance( double normal_tol );

	/**
	* Specifies the intended maximum aspect ratio of a grid cell in 3D space.
	* <br><br>
	* <b>Role:</b> The minimum value is 1. It is not always possible to attain the user-specified aspect ratio.
	*/
	void set_grid_aspect_ratio( double aspect_ratio );

	/**
	* Returns the user-specified grid aspect ratio.
	*/
	double get_grid_aspect_ratio() const;

	/**
	* Specifies the maximum length of a facet-edge.
	*/
	void set_max_edge_length( double max_edge_length );

	/**
	* Returns the user-specified maximum facet-edge length.
	*/
	double get_max_edge_length() const;

	/**
	* Specifies the minimum number of grid lines in the <i>u</i>-direction of a face.
	*/
	void set_min_grid_lines_u( int min_grid_lines );

	/**
	* Specifies the minimum number of grid lines in the <i>v</i>-direction of a face
	*/
	void set_min_grid_lines_v( int min_grid_lines );

	/**
	* Returns the user-specified minimum number of grid lines in the <i>u</i>-direction of a face.
	*/
	int get_min_grid_lines_u() const;

	/**
	* Returns the user-specified minimum number of grid lines in the <i>v</i>-direction of a face.
	*/
	int get_min_grid_lines_v() const;

	/**
	* Specifies the desired polygon shape for the output mesh.
	*/
	void set_polygon_type( polygon_type type );

	/**
	* Returns the user-specified polygon shape for the output mesh.
	*/
	polygon_type get_polygon_type() const;

};


/**
* Enumeration used by <tt>facet_options_expert</tt> to specify the mode in which
* the %surface tolerance is treated.
* @param af_surface_tolerance_from_refinement
* The %surface tolerance should be read from refinements.
* @param af_surface_tolerance_pure_number
* The absolute value of the %surface tolerance is to be entered.
* @param af_surface_tolerance_relative_to_body_diag
* The %surface tolerance as a fraction of the body diagonal is to be entered.
**/
enum DECL_FCT tolerance_specification_mode
{
	af_surface_tolerance_from_refinement,
	af_surface_tolerance_pure_number,
	af_surface_tolerance_relative_to_body_diag
};

/**
 * A facet-options class that is appropriate to workflows demanding the maximum control over faceting.
 * <br>
 * <b>Role:</b> This class is designed to meet the needs of applications that need more control over
 * the faceting algorithm than provided by the @href facet_options_visualization or @href facet_options_precise classes.
 * <br><br>
 * The <tt>facet_options_expert</tt> interface allows you to specify the parameters in the table below to control the output mesh.
 * Generally speaking, if you are using refinements to control the faceting algorithm, you cannot specify any of the parameters
 * in the table below. Conversely, if you specify any of the parameters in the table below, you cannot use refinements.
 * The exceptions to this rule are the parameters that control edge tolerances. These can be used with refinements.
 * <table width = 100%>
 * <tr width = 100%>
 * <td valign = "top" width = 25%><tt>tolerance_specification_mode</tt></td>
 * <td valign = "top">Specifies the source of the %surface tolerance value. 
 * Potential values are:
 * <ul>
 * <li> <tt>af_surface_tolerance_from_refinement</tt>: Surface tolerances are obtained from refinements.</li>
 * <li> <tt>af_surface_tolerance_pure_number</tt>: Surface tolerance is obtained from <tt>surface_tolerance</tt> parameter.</li>
 * <li> <tt>af_surface_tolerance_relative_to_body_diag</tt>: Surface tolerance is obtained from 
 *      <tt>surface_tolerance_as_fraction_of_body_diagonal</tt> parameter.</li>
 * </ul>
 * The default value is <tt>af_surface_tolerance_from_refinement</tt>, indicating the %surface tolerance values 
 * (and other parameter values) are obtained from refinements.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>surface_tolerance_as_fraction_of_body_diagonal</tt></td>
 * <td valign = "top">Specifies the maximum allowed distance between the %surface and the mesh
 * as a fraction of the length of the diagonal of the bounding box of the body containing the face being faceted. 
 * The default value is unspecified, which means the %surface tolerance is ignored.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>surface_tolerance</tt></td>
 * <td valign = "top">Specifies the maximum allowed distance between the %surface and the mesh. 
 * The default value is unspecified, which means the %surface tolerance is ignored.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>normal_tolerance</tt></td>
 * <td valign = "top">Specifies the maximum angle (in degrees) between %surface normals 
 * at points on a facet. The default value is 0, which means the normal tolerance is ignored.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>edge_sag_tolerance_as_fraction_of_surface_tolerance</tt></td>
 * <td valign = "top">Specifies the ratio of the edge position/angular tolerance to the face 
 * position/angular tolerance. The default value is 1.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>edge_sag</tt></td>
 * <td valign = "top">Specifies the edge position tolerance The default value is <tt>DBL_MAX</tt>, 
 * which means the positional tolerance is calculated from the 
 * <tt>edge_sag_tolerance_as_fraction_of_surface_tolerance</tt> and the <tt>surface_tolerance</tt>.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>edge_max_angle</tt></td>
 * <td valign = "top">Specifies the edge angular tolerance. The default value is <tt>DBL_MAX</tt>, 
 * which means the angular tolerance is calculated from the 
 * <tt>edge_sag_tolerance_as_fraction_of_surface_tolerance</tt> and the <tt>normal_tolerance</tt>.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>max_edge_length</tt></td>
 * <td valign = "top">Constrains the maximum length of a side of a grid cell in object space. 
 * More precisely, the maximum edge length value constrains the maximum length of a diagonal of a grid cell, 
 * because the length of a grid cell diagonal provides an upper limit for the edge length of a facet. 
 * Therefore, this parameter also constrains the maximum edge length of a facet.
 * The default value is 0, which means the maximum edge length is ignored.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>max_grid_lines</tt></td>
 * <td valign = "top">Specifies the maximum number of grid lines in either the <i>u</i> or <i>v</i> direction.
 * The default value is 2048. </td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>min_grid_lines_u</tt></td>
 * <td valign = "top">Specifies the minimum number of <i>u</i> grid lines (i.e., grid lines in the <i>u</i>-direction). 
 * The default value is 0. </td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>min_grid_lines_v</tt></td>
 * <td valign = "top">Specifies the minimum number of <i>v</i> grid lines (i.e., grid lines in the <i>v</i>-direction). 
 * The default value is 0. </td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>aspect_ratio</tt></td>
 * <td valign = "top">Specifies the maximum ratio of the long side to the short side of a grid cell in 3D space. 
 * The aspect ratio value does not guarantee that triangles will have a particular aspect ratio; 
 * it applies only to the aspect ratio of grid cells. 
 * The default value is 0, which means the grid aspect ratio is ignored.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>use_grid_to_edges</tt></td>
 * <td valign = "top">Specifies whether a grid is used and whether the points
 * where the grid cuts the edge is inserted to the edge. The default value is <tt>FALSE</tt>.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>polygon_type</tt></td>
 * <td valign = "top">Specifies the desired polygon shapes for the output mesh. 
 * The default value is <tt>all_triangles</tt>, which means the mesh should
 * consist entirely of triangular facets.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>min_node_tedge_distance_factor</tt></td>
 * <td valign = "top">Specifying a value greater than zero will make the algorithm
 * remove the nodes that are within a certain distance to a tolerant edge. The distance
 * is computed as the product of <tt>min_node_tedge_distance_factor</tt> and the edge tolerance. Default value is zero.</td>
 * </tr>
 * </table>
 * <br><br>
 * The interface to this facet options class also allows you to specify the following parameters to control the faceting algorithm. 
 * In general, these parameters affect the performance versus robustness criteria. These parameters can be set regardless of
 * the tolerance specification mode; meaning these parameters can be specified when using refinements.  
 * <table width = 100%>
 * <tr width = 100%>
 * <td valign = "top" width = 25%><tt>do_narrow_region_repair</tt></td>
 * <td valign = "top">Improves robustness where projected edges are very close together, but has a performance cost.
 * The default value is <tt>TRUE</tt>.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>check_and_generate_pcurves</tt></td>
 * <td valign = "top">Recalculated pcurves are used for faceting only: the persistent model is not changed. 
 * <tt>TRUE</tt> has a performance cost, but improves robustness against overly loose pcurves. 
 * Refer to @href api_set_pcurve_tightness to permanently change pcurves. 
 * The default value is <tt>TRUE</tt>.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>allow_facet_unspecified_faces</tt></td>
 * <td valign = "top"><tt>TRUE</tt> indicates that faces that were not specified can be faceted. 
 * <tt>FALSE</tt> indicates they cannot be faceted. <tt>TRUE</tt> has a performance cost, but improves robustness.
 * The default value is <tt>TRUE</tt>.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>allow_refacet_specified_faces</tt></td>
 * <td valign = "top"><tt>TRUE</tt> indicates that faces that were specified can be faceted more than once. 
 * <tt>FALSE</tt> indicates they cannot be refaceted. <tt>TRUE</tt> has a performance cost, but improves robustness. 
 * The default value is <tt>TRUE</tt>.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>allow_special_case</tt></td>
 * <td valign = "top"> <tt>TRUE</tt> indicates that faces with specific geometry types can be faceted using 
 * geometry-specific algorithms to speed up faceting. 
 * <tt>FALSE</tt> indicates they cannot. <tt>TRUE</tt> increases performance, but may not satisfy all tolerances.
 * The default value is <tt>FALSE</tt>.</td>
 * </tr>
 * </table> 
 * <br><br>
 * For additional information on <tt>facet_options_expert</tt> objects refer to the Technical Article <i>Facet Options</i>.
 * @see facet_options, facet_options_visualization, facet_options_precise.
 **/
class DECL_FCT facet_options_expert : public facet_options
{
	facet_options_expert( facet_options_expert const&);
public:
	// the settings here will probably change a bit as time progresses.  
	// Intended to be the whole gamut of what can be done with our faceter.

	/**
	* C++ constructor, creating a <tt>facet_options_expert</tt> object and
	* populating it with its default settings.
	* <br>
	* <b>Important:</b> The default tolerance specification mode is <tt>af_surface_tolerance_from_refinement</tt>.
	*/
	facet_options_expert();
	
	/**
	* @nodoc
	**/
	~facet_options_expert();

	/**
	* @nodoc
	**/
	// internal use only
	facet_options_expert( facet_options_internal* internal_opts );

	/** Specifies the tolerance specification mode of this facet options object.
	* <br>
	* <b>Note:</b> The %surface tolerance can be specified by one of three means.
	* It may be specified by a tolerance value; it may be specified by a fraction of the length
	* of the diagonal of the bounding box of the body containing the face being faceted; or it
	* may be specified using a @href REFINEMENT. These correspond to the enumerated values:
	* <tt>af_surface_tolerance_pure_number</tt>, <tt>af_surface_tolerance_relative_to_body_diag</tt>, and
	* <tt>af_surface_tolerance_from_refinement</tt>.
	*/
	void set_tolerance_specification_mode( tolerance_specification_mode mode );

	/**
	* Returns the tolerance specification mode of this facet options object.
	*/
	tolerance_specification_mode get_tolerance_specification_mode() const;

	/**
	* Specifies the %surface tolerance for faceting faces.
	* <br>
	* <b>Note:</b> This method also changes the <tt>tolerance_specification_mode</tt> to 
	* the <tt>af_surface_tolerance_pure_number</tt> mode.
	*/
	void set_surface_tolerance( double surf_tol );

	/**
	* Returns the specified %surface tolerance.
	*/
	double get_surface_tolerance() const;

	/**
	* Specifies the %surface tolerance for faceting faces, as a fraction of the body diagonal length.
	* <br>
	* <b>Note:</b> This method also changes the <tt>tolerance_specification_mode</tt> to 
	* the <tt>af_surface_tolerance_relative_to_body_diag</tt> mode.
	*/
	void set_surface_tolerance_as_fraction_of_body_diagonal( double surf_tol_frac );

	/**
	* Returns the specified fractional %surface tolerance.
	*/
	double get_surface_tolerance_as_fraction_of_body_diagonal() const;

	/**
	* Specifies the normal tolerance for faceting faces.
	* <br>
	* <b>Note:</b> If the <tt>tolerance_specification_mode</tt> is <tt>af_surface_tolerance_from_refinement</tt>,
	* this method will change the <tt>tolerance_specification_mode</tt> to <tt>af_surface_tolerance_relative_to_body_diag</tt>.
	*/
	void set_normal_tolerance( double normal_tol );

	/**
	* Returns the specified normal tolerance.
	*/
	double get_normal_tolerance() const;

	/**
	* Sets the positional and angular tolerances for edge faceting to be a fraction of %surface and normal tolerance.
	* <br>
	* <b>Note:</b> If the <tt>edge_sag</tt> or <tt>edge_max_angle</tt> were set before calling this method,
	* this method will invalidate those values. 
	**/
	void set_edge_sag_tolerance_as_fraction_of_surface_tolerance( double edge_sag_frac );

	/**
	* Gets the fraction of %surface and normal tolerance that the edges should be faceted to.
	**/
	double get_edge_sag_tolerance_as_fraction_of_surface_tolerance() const;

	/**
	* Sets the maximum distance between edges and their facet polylines.
	* <br>
	* <b>Note:</b> If this method is called after calling @href set_edge_sag_tolerance_as_fraction_of_surface_tolerance,
	* the <tt>edge_sag</tt> value specified by this method takes precidence over the value calculated by the other method. 
	**/
	void set_edge_sag( double edge_sag);

	/**
	* Gets the maximum distance between edges and their faceted polylines.
	* <br>
	* If this method returns a value of <tt>DBL_MAX</tt>, the maximum distance should be calculated
	* using the <tt>edge_sag_tolerance_as_fraction_of_surface_tolerance</tt> and the <tt>surface_tolerance</tt>.
	**/
	double get_edge_sag() const;

	/**
	* Sets the maximum angle (given in degrees) between tangents at facet points of edges.
	* <br>
	* <b>Note:</b> If this method is called after calling @href set_edge_sag_tolerance_as_fraction_of_surface_tolerance,
	* the <tt>edge_max_angle</tt> value specified by this method takes precidence over the value calculated by the other method. 
	**/
	void set_edge_max_angle( double edge_angle_in_degrees);

	/**
	* Gets the maximum angle (in degrees) between tangents at facet points of edges.
	* <br>
	* If this method returns a value of <tt>DBL_MAX</tt>, the maximum angle should be calculated
	* using the <tt>edge_sag_tolerance_as_fraction_of_surface_tolerance</tt> and the <tt>normal_tolerance</tt>.
	**/
	double get_edge_max_angle() const;

	/**
	* Specifies the intended maximum aspect ratio of a grid cell facet in 3D space.
	* <br>
	* <b>Note:</b> If the value of the grid aspect ratio parameter is less than or equal to 0, 
	* the grid aspect ratio is ignored. If the value of the grid aspect ratio parameter 
    * is greater than 0 and less than or equal to 1, a value of 1 is used.
    * The default value is 0, which means the grid aspect ratio is ignored.  
	* It is not always possible to attain the user-specified aspect ratio. 
	* <br><br>
	* <b>Note:</b> If the <tt>tolerance_specification_mode</tt> is <tt>af_surface_tolerance_from_refinement</tt>,
	* this method will change the <tt>tolerance_specification_mode</tt> to <tt>af_surface_tolerance_relative_to_body_diag</tt>.
	*/
	void set_grid_aspect_ratio( double aspect_ratio );

	/**
	* Returns the user-specified grid aspect ratio.
	*/
	double get_grid_aspect_ratio() const;

	/**
	* Specifies the maximum number of grid lines in either the <i>u</i> or <i>v</i> direction.
	* <br>
	* <b>Note:</b> If the <tt>tolerance_specification_mode</tt> is <tt>af_surface_tolerance_from_refinement</tt>,
	* this method will change the <tt>tolerance_specification_mode</tt> to <tt>af_surface_tolerance_relative_to_body_diag</tt>.
	*/
	void set_max_grid_lines( int max_grid_lines );

	/**
	* Returns the user-specified maximum number of grid lines in either the <i>u</i> or <i>v</i> direction.
	*/
	int get_max_grid_lines() const;

	/**
	* Specifies the maximum length of a facet edge.
	* <br>
	* <b>Note:</b> If the <tt>tolerance_specification_mode</tt> is <tt>af_surface_tolerance_from_refinement</tt>,
	* this method will change the <tt>tolerance_specification_mode</tt> to <tt>af_surface_tolerance_relative_to_body_diag</tt>.
	*/
	void set_max_edge_length( double max_edge_length );

	/**
	* Returns the user-specified maximum edge length.
	*/
	double get_max_edge_length() const;

	/**
	* Specifies the minimum number of grid lines in the <i>u</i>-direction of a face.
	* <br>
	* <b>Note:</b> If the <tt>tolerance_specification_mode</tt> is <tt>af_surface_tolerance_from_refinement</tt>,
	* this method will change the <tt>tolerance_specification_mode</tt> to <tt>af_surface_tolerance_relative_to_body_diag</tt>.
	*/
	void set_min_grid_lines_u( int min_grid_lines );
	/**
	* Specifies the minimum number of grid lines in the <i>v</i>-direction of a face.
	* <br>
	* <b>Note:</b> If the <tt>tolerance_specification_mode</tt> is <tt>af_surface_tolerance_from_refinement</tt>,
	* this method will change the <tt>tolerance_specification_mode</tt> to <tt>af_surface_tolerance_relative_to_body_diag</tt>.
	*/
	void set_min_grid_lines_v( int min_grid_lines );

	/**
	* Returns the user-specified minimum number of grid lines in the <i>u</i>-direction of a face.
	*/
	int get_min_grid_lines_u() const;

	/**
	* Returns the user-specified minimum number of grid lines in the <i>v</i>-direction of a face.
	*/
	int get_min_grid_lines_v() const;

	/**
	* Specifies the polygon shape for the output mesh.
	* <br>
	* <b>Role:</b> This action is ineffective under the <tt>af_surface_tolerance_from_refinement</tt> mode.
	*/
	void set_polygon_type( polygon_type type );

	/**
	* Returns the user-specified polygon shape for the output mesh.
	*/
	polygon_type get_polygon_type() const;

	/**
	* Specifies whether edges are seeded with the edge-grid intersection points. This behavior is optional only
	* for faces on analytic surfaces: for faces on %spline surfaces it is always done.
	* <br>
	* <b>Note:</b> This action is ineffective under the <tt>af_surface_tolerance_from_refinement</tt> mode.
	*/
	void set_use_grid_to_edges( logical in_value );

	/**
	* Returns whether <i>grid to edges</i> is required by this options object.
	*/ 
	logical get_use_grid_to_edges() const;

	/**
	* Specifies whether preemptive narrow region repair is done.  
	* The repair procedure consists of heuristics to detect when edges
	* are close enough for the facet edges to clash.  The repair aligns nearby edges facets.
	* <br>
	* This reduces the frequency of clockwise and upside down facets, but has a performance penalty.
	*/
	void set_do_narrow_region_repair( logical in_value );

	/**
	* Returns whether preemptive narrow region repair is required by this options object.
	*/
	logical get_do_narrow_region_repair() const;

	/**
	* Specifies whether the faceter is allowed to generate new, <i>temporary</i> bs2_curves for use during faceting
	* if the pcurves are not tight enough.  Turning this option on does not change the pcurves in the model.
	* <br>
	* Turning this option on improves robustness but has a performance cost.  If this frequently helps your model,
	* please consider using @href api_set_pcurve_tightness to improve the pcurve quality on your model.
	*/
	void set_check_and_generate_pcurves( logical in_value );

	/**
	* Returns whether the faceter will generate its own bs2_curves for internal use by the faceter
	* if the pcurves are not tight enough.
	*/
	logical get_check_and_generate_pcurves() const;

	/**
	* Sets whether it is ok to use special case gridding algorithms for specific %surface types.  
	* These special cases algorithms are significantly faster than the more general algorithms;
	* however, the grids generated by these algorithms may slightly miss tolerances on the diagonal.  
	* Speed over accuracy performance optimization.
	* <br>
	* Even with this flag set, large features are accurately recognized and isoparameter lines
	* are faceted respecting %surface and normal tolerance.
	**/
	void set_allow_special_case( logical do_allow);

	/**
	* Returns the value of the special case flag.  Default for visualization facet options is <tt>TRUE</tt>.
	**/
	logical get_allow_special_case() const;

	/**
	* Sets whether it is ok to refacet faces that are specified in the API facet entity or entities functions.
	* These API functions are: @href api_facet_entity, @href api_facet_entities, @href api_facet_unfaceted_entity, 
	* and @href api_facet_unfaceted_entities.
	* A "specified face" is a face expected to be faceted. If one specifies a body, all faces of the body
	* are "specified" unless one is calling a "unfaceted" API function and that face is already faceted.
	* This facilitates higher quality meshes, because the faceter can facet face A, then when it facets face B and
	* determines that more facet nodes are needed along the edge between face A and face B, it can refacet face A 
	* to insure no gaps between facets occur between faces, and results in high quality meshes.
	* See http://doc.spatial.com/index.php/Faceter_Robustness_Guidelines#Faceting_One_Face_May_Re-Facet_Adjoining_Faces
	**/
	void set_allow_refacet_specified_faces( logical do_allow);

	/**
	* Gets whether it is ok to refacet faces that are specified in the API facet entity or entities functions.
	* These API functions are: @href api_facet_entity, @href api_facet_entities, @href api_facet_unfaceted_entity, 
	* and @href api_facet_unfaceted_entities.
	* A "specified face" is a face expected to be faceted. If one specifies a body, all faces of the body
	* are "specified" unless one is calling a "unfaceted" API function and that face is already faceted.
	* This facilitates higher quality meshes, because the faceter can facet face A, then when it facets face B and
	* determines that more facet nodes are needed along the edge between face A and face B, it can refacet face A 
	* to insure no gaps between facets occur between faces, and results in high quality meshes.
	* See http://doc.spatial.com/index.php/Faceter_Robustness_Guidelines#Faceting_One_Face_May_Re-Facet_Adjoining_Faces
	**/
	logical get_allow_refacet_specified_faces() const;

	/**
	* Sets whether it is ok to facet faces that are not specified in the API facet entity or entities functions.
	* These API functions are: @href api_facet_entity, @href api_facet_entities, @href api_facet_unfaceted_entity, 
	* and @href api_facet_unfaceted_entities.
	* An "unspecified face" is a face not expected to be faceted. 
	* This facilitates higher quality meshes. The faceter can facet face A, and if it determines that more facet
	* nodes are needed along the edge between face A and face B, it can facet face B also, even if it is not specified in the API call.
	* This insures that no gaps between facets occur between faces, and results in high quality meshes.
	* See http://doc.spatial.com/index.php/Faceter_Robustness_Guidelines#Faceting_One_Face_May_Re-Facet_Adjoining_Faces
	**/
	void set_allow_facet_unspecified_faces( logical do_allow);

	/**
	* Gets whether it is ok to facet faces that are not specified in the API facet entity or entities functions.
	* These API functions are: @href api_facet_entity, @href api_facet_entities, @href api_facet_unfaceted_entity, 
	* and @href api_facet_unfaceted_entities.
	* An "unspecified face" is a face not expected to be faceted. 
	* This facilitates higher quality meshes. The faceter can facet face A, and if it determines that more facet
	* nodes are needed along the edge between face A and face B, it can facet face B also, even if it is not specified in the API call.
	* This insures that no gaps between facets occur between faces, and results in high quality meshes.
	* See http://doc.spatial.com/index.php/Faceter_Robustness_Guidelines#Faceting_One_Face_May_Re-Facet_Adjoining_Faces
	**/
	logical get_allow_facet_unspecified_faces() const;

	// reports if the faceter went deep into its fallback code path.
	// this may be useful for customers to detect automatically if bad faceting may have occurred.
	/**
	* @nodoc
	**/
	logical suspect_faceting_failed(ENTITY_LIST& out_suspect_faces) const;

	/**
	* It is possible to have faces which would not require any triangles to meet the other user specified constraints:
	* for example, a face with two edges, each just requiring one edge facet, wouldn't get any triangles.
	*
	* The flag <tt>each_face_needs_a_triangle</tt> specifies that the faceter should insert enough points
	* to ensure every face has at least one facet.  This flag is FALSE by default.  Users may want to set this
	* to true if they need a mapping between facets and FACEs such that each FACE is represented by at least one facet.
	* Setting this flag to TRUE will add more facets, and possibly degrade the quality by adding sliver triangles
	**/
	void set_each_face_needs_a_triangle( logical force_triangles);

	/**
	* Returns the state of the each_face_needs_a_triangle flag
	**/

	logical each_face_needs_a_triangle() const;

	/**
	* Sets the flag which tells the faceter to use a different algorithm on spheres, which reduces the facet count around the spheres's sigularity at the poles.
	* <br>
	* <b>Note:</b> Works only on spheres currently.
	**/
	void set_grid_one_dir( logical in_grid_one_dir );

	/**
	* Gets the flag which tells the faceter to use a different algorithm on spheres, which reduces the facet count around the spheres's sigularity at the poles.
	* <br>
	* <b>Note:</b> Works only on spheres currently.
	**/
	logical grid_one_dir() const;

	/**
	* Sets the <tt>min_node_tedge_distance_factor</tt> value. Nodes that are closer to a tolerant edge than the edge tolerance times the <tt>min_node_tedge_distance_factor</tt>
	* value will be removed by the faceter.
	**/
	void set_min_node_tedge_distance_factor(double dist);

	/**
	* Returns the current value of <tt>min_node_tedge_distance_factor</tt>. Nodes that are closer to a tolerant edge than the edge tolerance times
	* the <tt>min_node_tedge_distance_factor</tt> value will be removed by the faceter.
	**/
	double get_min_node_tedge_distance_factor() const;

	facet_options_expert& get_expert_options();
};
/** @} */
#endif
