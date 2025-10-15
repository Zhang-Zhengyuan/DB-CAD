/*******************************************************************/
/*    Copyright (c) 2008-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef SPA_SPRING_BACK_DEF_HXX
#define SPA_SPRING_BACK_DEF_HXX

class ENTITY_LIST;
class SPA_springback_impl;
class outcome;
class AcisOptions;
class SPAposition;
class SPA_spring_back_options;
class SPA_spring_back_def;
class SPAbox;
class SPAvector;
class SPAunit_vector;
#include "mmgr.hxx" // ACIS_OBJECT

#include "dcl_warp.h"
#include "lists.hxx"
#include "spldef.hxx"
/**
 * \defgroup ACISSPRINGBACK Springback
 */
/**
 * @file spa_spring_back_def.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISSPRINGBACK
 *
 * @{
 */

/**
* Controls how check errors are handled during a springback operation.
* @param SB_NO_CHECK
* Do not check for errors (not recommended).
* @param SB_CHECK_FAIL
* Check for errors; if any are found, abort the operation and restore the body to its original state.
* @param SB_CHECK_WARN
* Check for errors; if any are found, issue a warning, but complete the operation; return a list of
* any problem faces in the <tt>SPA_spring_back_options</tt> object.
**/
enum sb_check_result_mode
{
	SB_NO_CHECK,
	SB_CHECK_FAIL, 
	SB_CHECK_WARN
};


/**
 * Optional inputs for a springback deformation.
 * <br><br>
 * <b>Role:</b> This class specifies optional inputs for a springback operation.
 * Passing a <tt>NULL SPA_spring_back_options</tt> object to <tt>api_spring_back</tt> is
 * equivalent to passing a <tt>SPA_spring_back_options</tt> object with its default (recommended) settings.
 * <br><br>
 * @see SPA_spring_back_def, api_spring_back
 */
class DECL_WARP  SPA_spring_back_options : public ACIS_OBJECT
{
	// Give access to m_problem_faces so faces with check errors can be added
	friend DECL_WARP outcome api_spring_back(ENTITY_LIST&, SPA_spring_back_def&, SPA_spring_back_options*, AcisOptions*);

private:
	SPA_spring_back_options (SPA_spring_back_options const& SPA_spring_back_options);
	SPA_spring_back_options &operator=  (SPA_spring_back_options const& SPA_spring_back_options);
	logical m_check_fairness;
	logical m_maintain_surface_ranges;
	copy_knots_mode m_copy_knots;
	sb_check_result_mode m_check_result_mode;
	ENTITY_LIST m_problem_faces;

public:
	/**
	* Constructs a default <tt>SPA_spring_back_options</tt> object.
	* <br><br>
	* <b>Role:</b> The object's settings are initialized to their recommended values: fairness checks
	* are enabled, deformations yielding illegal geometry are avoided, surface ranges are not
	* maintained unless necessary, and the knots on B-spline surfaces are not preserved in
	* deriving the deformed surface.
	*/
	SPA_spring_back_options();
	/**
	* Destructs a <tt>SPA_spring_back_options</tt> object.
	*/
	~SPA_spring_back_options();
	/**
	* By default, the <tt>api_spring_back</tt> checks for geometry that becomes illegal after applying the
	* springback deformation, and avoids applying the deformation in that case. (This corresponds to the
	* mode setting <tt>SB_CHECK_FAIL</tt>.)
	* You can override this behavior, at the risk of possibly making illegal faces or edges, using
	* this method. Refer to @href sb_check_result_mode for further details.
	 * @param mode
	 *   You can input <tt>SB_NO_CHECK</tt> or <tt>SB_CHECK_WARN</tt> to override the default behavior.
	*/
	void set_check_result_mode(sb_check_result_mode mode);
	/**
	* Returns <tt>SB_CHECK_FAIL</tt> if <tt>api_spring_back</tt> follows the default behavior,
	* meaning that it checks for illegal geometry after applying the spring-back deformation and
	* avoids applying the deformation in that case.
	*/
	sb_check_result_mode get_check_result_mode() const;
	/**
	*  When the check-result mode is set to <tt>SB_CHECK_WARN</tt> for a call to <tt>api_spring_back</tt>,
	*  this method reports faces found to have illegal geometry because of the operation. In
	*  other modes it has no effect on the input list.
	 * @param faces
	 *   Faces with illegal geometry are added to this list.
	*/
	void get_problem_faces(ENTITY_LIST& faces) const;

 	/**
	 * Determines whether or not <tt>api_spring_back</tt> assesses the springback definition
	 * for potential problems resulting from the ensuing deformation.
	 * @param do_check
	 *   If <tt>TRUE</tt>, check the fairness of the spring-back deformation and abort the operation
	 *   if problems are expected.
	 */
	void set_do_check_fairness(logical do_check);
 	/**
	 * Returns the current setting that controls "fairness" checking. 
	 */
	logical get_do_check_fairness() const;

	/**
	* By default, the <tt>api_spring_back</tt> ignores the original knots values associated with 
	* B-spline surfaces when deriving the deformed surfaces. This corresponds to the
	* mode setting <tt>NO_COPY_KNOTS</tt>.
	* You can override this behavior, forcing preservation of the knot values, using
	* this method. Refer to @href copy_knots_mode for further details.
	* @param mode
	*   You can input <tt>NO_COPY_KNOTS</tt> or <tt>COPY_KNOTS_INITIALLY</tt> to override the current behavior.
	*/
	void set_copy_knots_mode(copy_knots_mode mode);

	/**
	* Returns <tt>NO_COPY_KNOTS</tt> if <tt>api_spring_back</tt> follows the default behavior,
	* meaning that it ignores the original knots values associated with 
	* B-spline surfaces when deriving the deformed surfaces. Otherwise, it returns
	* <tt>COPY_KNOTS_INITIALLY</tt>.
	*/
	copy_knots_mode get_copy_knots_mode() const;

  	/**
	 * Determines whether or not <tt>api_spring_back</tt> maintains the surface ranges of the input
	 * <br><br>
	 * <b>Role:</b> Subsetting is considered an integral part of the definition of a surface's
	 * range. Therefore, if you set this option to <tt>FALSE</tt> and wish subsetting to be
	 * ignored, you should use <tt>surface::unlimit</tt> prior to the Springback operation.
	 * <br><br>Be aware that setting this option to <tt>FALSE</tt> may make the success of the
	 * desired Springback option less likely, or may result in larger gaps in the output.
	 * @param do_maintain
	 *   If <tt>TRUE</tt>, maintain the ranges.
	 */
	void set_do_maintain_surface_ranges(logical do_maintain);
	/**
	 * Returns the current setting that controls maintaining surface ranges or not. 
	 */
	logical get_do_maintain_surface_ranges() const;
/**
 * @nodoc
 */
	logical operator== (SPA_spring_back_options const& in_opts) const;
/**
 * @nodoc
 */
	logical operator!= (SPA_spring_back_options const& in_opts) const;
};

class surface;
/**
 * Definition of the mapping which fulfills the springback deformation.
 * <br><br>
 * <b>Role:</b> This class defines the mapping of the springback deformation. It allows
 * specification of both fixed and shaping constraints and provides queries for per-constraint gaps in 
 * the computed deformation. Fixed constraints can be specified with faces, wire bodies, and positions. 
 * Shaping constraints can be specified with wire bodies and positions.
 * In addition, the domain of the springback deformation can be specified. 
 * Domain specification by the user is only required for efficient multiple calls to 
 * <tt>api_spring_back</tt> with the same <tt>SPA_spring_back_def</tt>, but with different bodies. In this case, the caller must first set the domain of the 
 * <tt>SPA_spring_back_def</tt> to encompass all of bodies to be used in all future calls to <tt>api_spring_back</tt> with the single <tt>SPA_spring_back_def</tt>.
* <br><br>
 * @see SPA_spring_back_options, api_spring_back
 */
class DECL_WARP SPA_spring_back_def : public ACIS_OBJECT
{
private:
	SPA_springback_impl* m_impl;

	// Copy CTOR, operator=. These should not be used.
	SPA_spring_back_def (SPA_spring_back_def const& SPA_spring_back_def);
	SPA_spring_back_def &operator=  (SPA_spring_back_def const& SPA_spring_back_def);


public:
/**
 * @nodoc
 */
	SPA_springback_impl* get_impl() const;
	/**
	* Constructs a <tt>SPA_spring_back_def</tt> object. The press direction must be specified at construction and cannot be changed over the object's lifetime.
	*/
	SPA_spring_back_def(SPAvector const& press_dir);
	/**
	* Destructs a <tt>SPA_spring_back_def</tt> object.
	*/
	virtual ~SPA_spring_back_def();

	/**
	* Returns the press direction.
	* <br><br>
	* <b>Role:</b> This is a vector which models the press direction in a springback stamping.
	*/
	SPAunit_vector const& get_press_direction() const;

	/**
	* Returns the springback domain of definition. Note that the domain of defintion automatically encompasses all of the constraints.
	* <br><br>
	* <b>Role:</b> Users need only be concerned with this to make multiple calls to <tt>api_spring_back</tt> with the same <tt>SPA_spring_back_def</tt> and different
	* lists of bodies. In that case, the springback domain of definition must encompass all of the input bodies.
	*/
	SPAbox get_def_domain() const;
	/**
	* Enlarge the springback domain of definition.
	* <br><br>
	* <b>Role:</b> Users need only be concerned with this to make multiple calls to <tt>api_spring_back</tt> with the same <tt>SPA_spring_back_def</tt> and different
	* lists of bodies. In that case, the springback domain of definition must encompass all of the input bodies.
	* For efficiency, and to minimize constraint gaps, the springback domain of definition should minimally encompass both the constraints and 
	* the bodies subject to the springback deformation.
	 * @param box
	 *   the springback domain of definition will be enlarged (if needed) to encompass the input box.
	*/
	void expand_def_domain(SPAbox const& box);
	/**
	 * Construct a fixed constraint and return a tag which can be used for gap queries.
	 * <br><br>
	 * <b>Role:</b> A springback deformation is specified by its domain and its constraints.
	 * The springback deformation will interpolate smoothly between the specified constraints.
	 * @param fixed_ents
	 *   faces and wire bodies to remain fixed under the springback deformation
	 * @param fixed_pts
	 *   positions to remain fixed under the springback deformation
	 * @param npts
	 *   number of points in the <tt>fixed_pts</tt> array
	 */
	int add_fixed_constraint(ENTITY_LIST const& fixed_ents, SPAposition const* fixed_pts=NULL, int npts=0);
	/**
	 * Construct a shaping constraint and return a tag which can be used for gap queries.
	 * <br><br>
	 * <b>Role:</b> A springback deformation is specified by its domain and its constraints.
	 * The springback deformation will interpolate smoothly between the specified constraints.
	 * @param before_ents
	 *   wire bodies to move in a specified fashion by the springback deformation
	 * @param after_ents
	 *   wire bodies corresponding to the desired result of the springback deformation on the <tt>before_ents</tt>
	 * @param before_pts
	 *   points to move in a specified fashion by the springback deformation
	 * @param after_pts
	 *   points corresponding to the desired result of the springback deformation on the <tt>before_pts</tt>
	 * @param npts
	 *   number of points in each of the pts arrays
	 */
	int add_shaping_constraint(ENTITY_LIST const& before_ents, ENTITY_LIST const& after_ents, SPAposition const* before_pts=NULL, SPAposition const* after_pts=NULL, int npts=0);

	/**
	 * Construct a shaping constraint and return a tag which can be used for gap queries.
	 * <br><br>
	 * <b>Role:</b> A springback deformation is specified by its domain and its constraints.
	 * The springback deformation will interpolate smoothly between the specified constraints.
	 * @param before_pts
	 *   points to move in a specified fashion by the springback deformation
	 * @param after_pts
	 *   points corresponding to the desired result of the springback deformation on the <tt>before_pts</tt>
	 * @param npts
	 *   number of points in each of the pts arrays
	 * @param pt_weights
	 *  Optional weights, either NULL, or one for each before-after point pair. Use a larger number to emphasize a particular before-after point pair.
	 */
	int add_shaping_constraint(SPAposition const* before_pts, SPAposition const* after_pts, int npts, double const* pt_weights);

	/**
	 * Construct a boundary plane and return a tag which can be used for gap queries.
	 * <br><br>
	 * <b>Role:</b> A boundary plane divides the domain of the springback deformation into a fixed side and a movable side. 
	 * The fixed side of the boundary plane will remain fixed under the springback deformation. 
	 * Notes: 
	 * The boundary plane must be parallel to the press direction.
	 * All shaping constraints must lie on the movable side of the domain.
	 * @param root_point
	 *   a point on the boundary plane; the <tt>root_point</tt> should be near the constraints
	 * @param normal
	 *   the normal direction of the boundary plane, which points into the moving side of the domain
	 */
	int set_boundary_plane(SPAposition const& root_point, SPAvector const& normal);

	/**
	* Finalize the <tt>SPA_spring_back_def</tt> so it is in a completed state, that is, gap and goodness queries can be made, 
	* and the springback deformation can be aplied to multiple bodies.
	* <br><br>
	* <b>Role:</b> After all the constraints have been added to the <tt>SPA_spring_back_def</tt>, users can call finalize_def, and then query the constraint
	* gaps, or the deformation goodness. If mulitple springback deformations are anticipated using a single <tt>SPA_spring_back_def</tt>, finalize_def can be 
	* called with a box or entity list whose box is large enough to encompass all the anticipated bodies. Note that after the call to finalize_def, no further changes can be made to the <tt>SPA_spring_back_def</tt>;
	* for example, no more constraints can be added. Note that <tt>finalize_def</tt> is only required if <tt>api_spring_back</tt> will be called on multiple bodies
	* with the same <tt>SPA_spring_back_def</tt>, or if constraint gap or deformation goodness queries are required before calling <tt>api_spring_back</tt>.
	* @param domain_box
	* box which encompasses all bodies whose springback deformation is anticipated.
	*/
	void finalize_def(SPAbox const& domain_box);
	/**
	* Finalize the <tt>SPA_spring_back_def</tt> so it is in a completed state, that is, gap and goodness queries can be made, 
	* and the springback deformation can be aplied to multiple bodies.
	* <br><br>
	* <b>Role:</b> After all the constraints have been added to the <tt>SPA_spring_back_def</tt>, users can call finalize_def, and then query the constraint
	* gaps, or the deformation goodness. If mulitple springback deformations are anticipated using a single <tt>SPA_spring_back_def</tt>, finalize_def can be 
	* called with a box or entity list whose box is large enough to encompass all the anticipated bodies. Note that after the call to finalize_def, no further changes can be made to the <tt>SPA_spring_back_def</tt>;
	* for example, no more constraints can be added. Note that <tt>finalize_def</tt> is only required if <tt>api_spring_back</tt> will be called on multiple bodies
	* with the same <tt>SPA_spring_back_def</tt>, or if constraint gap or deformation goodness queries are required before calling api_spring_back.
	* @param domain_entities
	* entity_list whose box encompasses all bodies whose springback deformation is anticipated.
	*/
		void finalize_def(ENTITY_LIST const& domain_entities);

	/**
	* Returns the maximum gap found on the constraint corresponding to the input tag.
	* <br><br>
	* <b>Role:</b> After calling <tt>api_spring_back</tt> for the first time, the constraint gaps can be queried with this method. If
	* <tt>api_spring_back</tt> has not been called with this <tt>SPA_spring_back_def</tt>, or if the tag does not correspond to an existing constraint, 
	* a non-physical value of -1.0 will be returned.
	* Note:
	* The constraint gap returned by this method is the gap produced by the springback mapping. If B-spline approximations are returned by api_spring_back,
	* the actual measured gaps between the constraint "after" edges and the corresponding body edges may be larger, due to the approximation tolerance.
	* @param tag
	*   the integer tag which was returned by the constraint creation method
	* @see add_fixed_constraint, see add_shaping_constraint, see set_boundary_plane
	*/
	double get_constraint_gap(int tag) const;
 	/**
	 *  This function returns a number between 0 and 10: numbers near 10 indicate good quality results are likely.  
	 *  2 or lower indicates severe problems are likely. Revise the springback definition if this function
	 *  returns a low value.
	 */
     double get_deformation_fairness();
     /**
     * Tells whether this springback definition is final, that is, not subject to further changes.
     */
     logical is_final() const;

	 /**
	 * Tells whether optimization for use with finite element data is on.
	 **/
	 logical get_do_FEA() const;
	 /**
	 * Toggles optimization for use with finite element data.
	 **/
	 void set_do_FEA(logical do_FEA);
	 /**
	* Map points according to the spring back mapping.
	* Returns TRUE if all input points were inside the SPAbox returned by the method get_def_domain().
	* If FALSE is returned, then some of the results are unreliable; specifically, the user needs to exclude 
	* image points corresponding to input points which are not within the get_def_domain() SPAbox.
	* <br><br>
	* <b>Role:</b>
	* After the <tt>SPA_spring_back_def</tt> has been finalized, either by calling api_spring_back, or
	* <tt>finalize_def()</tt>, the spring back map can be queried to map a set of points using the 
	* spring back mapping. A comprehensive set of image points may be useful to visualize the mapping 
	* or to localize constraint problems if the spring back operation fails.
	* @param npts
	*The number of points to be mapped.
	* @param inPoints
	*Const array of postions to be mapped, must be of size npts.
	* @param outPoints
	*On return, the image of the input points under the spring back mapping. Must be size npts.
	*/
	logical map_points(int npts, SPAposition const* inPoints, SPAposition* outPoints) const;
/**
 * @nodoc
 */
	logical operator== (SPA_spring_back_def const& in_def) const;
/**
 * @nodoc
 */
	logical operator!= (SPA_spring_back_def const& in_def) const;

};





/**
 * Modifies a list of bodies by applying the function defined by the <tt>SPA_spring_back_def</tt> to the bodies' underlying geometry.
 * This action is refered to as the springback deformation.
 * The <tt>SPA_spring_back_def</tt> interpolates smoothly in the space between the constraints.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D Springback
 * <br><br>
* @param bodies
 * bodies to be modified
 * @param sbd
 * <tt>SPA_spring_back_def</tt>, which specifies the constraints the springback mapping is to satisfy, and the domain of the mapping function.
 * @param sbo
 * <tt>SPA_spring_back_options</tt>, optional paramters affecting the behavior of the mapping
 * @param opts
 * ACIS options, specifying versioning or journaling
 */

DECL_WARP outcome api_spring_back(ENTITY_LIST& bodies, SPA_spring_back_def& sbd, SPA_spring_back_options* sbo=NULL, AcisOptions* opts=NULL);
/** @} */
#endif // SPA_SPRING_BACK_DEF_HXX
