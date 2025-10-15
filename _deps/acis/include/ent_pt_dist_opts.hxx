/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/*******************************************************************/
//  Modification history
// AJR  01Jul09   Added new interface functions to allow new unsigned 
//                distance mode.   
//  DM  28May08   Documentation
//  DM  10Jul07   Remove references to help data. Allow signed distances for point
//                entity distance calculations
//  DM  18May07   Created file
/*******************************************************************/
// User entity-point distance options class definition. This object is required by
// api_entity_point_distance.

#ifndef ENT_PT_DIST_OPTS_HFILE
#define ENT_PT_DIST_OPTS_HFILE

#include "acis.hxx"
#include "dcl_query.h"

class SPAposition;
class param_info;

/**
* @file ent_pt_dist_opts.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

/** \addtogroup INTRAPIDISTANCE
 *  \brief Declared at <ent_pt_dist_opts.hxx>
 *  @{
 */

// This enumerated type encodes the entity-point distance mode.
/**
 * Specifies the <i>computation mode</i> of entity-point distance calculations.
 * @par Technical Article
 * <i>[Entity Point Distance](http://doc.spatial.com/articles/e/n/t/Entity-Point_Distance_f618.html)</i>
 * @par Role
 * <tt>SPAentity_point_distance_mode</tt> is an enumerated type used by
 * @href SPAentity_point_distance_options to specify the algorithm used by @href api_entity_point_distance.
 * The mode indicates the format of the input point data in order to improve the performance
 * of entity-point distance calculations for large data sets.
 * @param SPAentity_point_distance_default Computes the distances between the points and the entity using the best general method.
 * @param SPAentity_point_distance_scanline Computes the distances between the points and the entity. Performance is optimal when the input point data has a curvilinear structure, such as data obtained by a scanline.
 * @param SPAentity_point_distance_memory_minimize Computes the distances between the points and the entity. The method is optimized for memory usage and there is small gain in performance over the simple iteration method.
 * @param SPAentity_point_distance_simple_iteration Computes the distance between the points and the entity in a manner equivalent to calling <tt>api_entity_point_distance</tt> one point at a time.
 * @see SPAentity_point_distance_options, api_entity_point_distance
 */

// If additional enums are added then edit P_Point_Distance_Options correspondingly
enum SPAentity_point_distance_mode 
{
    SPAentity_point_distance_default,
    SPAentity_point_distance_scanline,
    SPAentity_point_distance_memory_minimize,
    SPAentity_point_distance_simple_iteration
};

// Introduce a new enum in order to add unsigned distance mode
/**
 * Specifies the <i>distance mode</i> of entity-point distance calculations.
 * @par Technical Article
 * <i>[Entity Point Distance](http://doc.spatial.com/articles/e/n/t/Entity-Point_Distance_f618.html)</i>
 * @par Role
 * <tt>SPAepd_distance_mode</tt> is an enumerated type used by
 * @href SPAentity_point_distance_options to specify the behavior of @href api_entity_point_distance.
 * The <i>distance mode</i> indicates whether distances values are computed for points inside the given <tt>ENTITY</tt>,
 * and if so, how such distance values are reported.
 * @param SPAepd_distance_default The default mode. Returns zero for any internal or boundary points, or a positive distance for external points.
 * @param SPAepd_distance_signed Returns negative distances for internal points, zero for boundary points, or positive distances for external points.
 * @param SPAepd_distance_unsigned Returns the same value as <tt>SPAepd_distance_signed</tt> except that the distance value is always non-negative.
 * @see SPAentity_point_distance_options, api_entity_point_distance
 */
enum SPAepd_distance_mode 
{
    SPAepd_distance_default,
    SPAepd_distance_signed,
    SPAepd_distance_unsigned
};

/**
 * Represents common data and functionality for entity-point distance options.
 * @par Role
 * A <tt>SPAentity_point_distance_options</tt> object is used as an optional argument to @href api_entity_point_distance.
 * @see api_entity_point_distance
 */

class DECL_QUERY SPAentity_point_distance_options : public ACIS_OBJECT
{
private:
    SPAentity_point_distance_mode             current_mode;
    SPAepd_distance_mode                      distance_mode;

public:
     /**
	 * Constructor.
	 * Creates a <tt>SPAentity_point_distance_options</tt> object and initializes it with default values.
	 */
    SPAentity_point_distance_options();

    /**
	 * Copy constructor.
	 * Creates a <tt>SPAentity_point_distance_options</tt> object from an existing options object.
     * @param epd_opts The given <tt>SPAentity_point_distance_options</tt> object.
	 */
    SPAentity_point_distance_options( const SPAentity_point_distance_options &epd_opts );

	/**
	 * Destructor.
	 */
    ~SPAentity_point_distance_options();

     /**
	 * Sets the <tt>SPAentity_point_distance_mode</tt> to be the specified value.
	 * @param epd_mode The given entity-point distance mode.
	 */
    void set_mode(SPAentity_point_distance_mode epd_mode);

     /**
	 * Resets the <tt>SPAentity_point_distance_mode</tt> to the default value, <tt>SPAentity_point_distance_default</tt>.
	 */
    void set_mode();

    /**
	 * Retrieves the current <tt>SPAentity_point_distance_mode</tt>.
	 */
    SPAentity_point_distance_mode get_mode() const { return current_mode; }

    /**
     * Sets whether to compute signed distances for points inside the <tt>ENTITY</tt> (<tt>BODY</tt> or <tt>LUMP</tt>).
     * @par Role
	 * By default @href api_entity_point_distance computes the distance to be zero when an input   
     * point is inside the given <tt>ENTITY</tt> (for a <tt>BODY</tt> or <tt>LUMP</tt>). <br>
	 * Set the argument to <tt>TRUE</tt> to compute a negative distance value for points inside the given <tt>ENTITY</tt>. <br>
	 * Set the argument to <tt>FALSE</tt> to return a zero distance valus for points inside the given <tt>ENTITY</tt>.
	 * <tt>set_distance_mode</tt> provides more control of the return value than <tt>set_signed_distances</tt>.
     * @param epd_signed_distances New setting for signed distances (<tt>TRUE</tt> or <tt>FALSE</tt>).
     */
    void set_signed_distances(logical epd_signed_distances); 

    /**
	 * Retrieves whether signed distances should be computed.
	 */
    logical get_signed_distances() const;

	/**
     * Sets whether to compute signed or unsigned distances for points inside the <tt>ENTITY</tt> (<tt>BODY</tt> or <tt>LUMP</tt>).
     * @par Role
	 * By default @href api_entity_point_distance computes the distance to be zero when an input   
     * point is inside the given <tt>ENTITY</tt> (for a <tt>BODY</tt> or <tt>LUMP</tt>). 
	 * <br>
	 * Set the <i>distance mode</i> to <tt>SPAepd_distance_signed</tt> to compute a negative distance value for points inside the given <tt>ENTITY</tt>. 
	 * <br>
	 * Set the <i>distance mode</i> to <tt>SPAepd_distance_unsigned</tt> to compute a positive distance value for points inside the given <tt>ENTITY</tt>. 
	 * <br>
	 * Set the <i>distance mode</i> to <tt>SPAepd_distance_default</tt> to return a zero distance value for points inside the given <tt>ENTITY</tt>.
     * @param epd_distance_mode New value for the distance mode.
     */
	void set_distance_mode(SPAepd_distance_mode epd_distance_mode) { distance_mode = epd_distance_mode; }

	/**
	 * Retrieves the current <tt>SPAepd_distance_mode</tt>.
	 */
	SPAepd_distance_mode get_distance_mode() const { return distance_mode; }
};

/** @} */

#endif   // ENT_PT_DIST_OPTS_HFILE
