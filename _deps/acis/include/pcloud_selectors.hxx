/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


#ifndef FIND_CLOSEST_POINTS_HXX
#define FIND_CLOSEST_POINTS_HXX
#include "SPAposition_cloud.hxx"
#include "dcl_base.h"
#include "container_utils.hxx"
#include "vector.hxx"
class VOID_LIST;

/**
* @file pcloud_selectors.hxx
* @CAA2Level L1
* @CAA2Usage U1
*! \addtogroup FCTAPI
*  \brief Declared at <pcloud_selectors.hxx>
*  @{
*/

/**
* @nodoc
**/
// internal testing purposes only.
DECL_BASE double find_closest_points_brute_force(
		SPAposition_ptr_const_alias_array const& cloudlet, 
		SPAposition const& test_point,
		VOID_LIST& out_closest_points,
		double resolution=SPAresmch);

#ifdef INTERNAL_DEBUG_CHECKS
/**
* @nodoc
**/
// internal testing purposes only.
DECL_BASE void reset_brute_force_count();
/**
* @nodoc
**/
// internal testing purposes only.
DECL_BASE int get_brute_force_count();
#endif

/**
* This struct is useful for closet-point and related queries. 
**/
struct position_distance_pair
#ifndef aix
	:public ACIS_OBJECT
#endif
{
	/**
	* default constructor
	**/
	position_distance_pair( SPAposition const& pos, double dist )
	{
		position=pos;
		distance=dist;
	}
	/**
	* position element of the pair
	**/
	SPAposition position;
	/**
	* distance element of the pair
	**/
	double distance;

	/**
	* For the purposes of making an ordered set (c.f., position_distance_pair_set)
	* we order position_distance_pairs by their distances.
	**/
	bool operator<( position_distance_pair const& other ) const
	{
		return distance < other.distance;
	}
};

class position_distance_pair_set_impl;
class position_distance_pair_set_iterator_impl;
/**
* @nodoc
**/
class DECL_BASE position_distance_pair_set_iterator: public ACIS_OBJECT
{
public:
	/**
	*@nodoc
	**/
	position_distance_pair_set_iterator( position_distance_pair_set_iterator_impl* impl);
	/**
	* @nodoc
	**/
	~position_distance_pair_set_iterator();
	/**
	* @nodoc
	**/
	position_distance_pair_set_iterator(position_distance_pair_set_iterator const& other);
	/**
	* @nodoc
	**/
	position_distance_pair const& operator*() const;
	/**
	*@nodoc
	**/
	position_distance_pair const* operator->() const;
	/**
	*@nodoc
	**/
	void operator++();
	/**
	*@nodoc
	**/
	logical operator==(position_distance_pair_set_iterator const&) const;
	/**
	*@nodoc
	**/ 
	logical operator!=(position_distance_pair_set_iterator const& other) const;
	/**
	*@nodoc
	**/ 
	position_distance_pair_set_iterator_impl* get_impl();
private:

	position_distance_pair_set_iterator_impl* m_impl;
};

/**
* Represents an ordered set of position distance pairs
**/
class DECL_BASE position_distance_pair_set: public ACIS_OBJECT
{
public:
	/**
	* returns number of pairs in this set
	**/
	size_t size() const;

	/**
	* inserts an element into the ordered set.
	**/
	void insert( position_distance_pair const& this_pair );

	/**
	* returns iterator pointing at begining of the set.
	**/
	position_distance_pair_set_iterator begin() const;
	
	/**
	* returns iterator pointing at ending of the set.
	**/
	position_distance_pair_set_iterator end() const;

	/**
	* returns lower bound for given position_distance_pair.
	**/
	position_distance_pair_set_iterator lower_bound(position_distance_pair const& p) const;

	/**
	* returns upper bound for given position_distance_pair.
	**/
	position_distance_pair_set_iterator upper_bound(position_distance_pair const& p) const;

	/**
	* erases a portion of the set.
	**/
	void erase( position_distance_pair_set_iterator const& start_of_range, position_distance_pair_set_iterator const& one_past_end_of_range) ;

	/**
	*@nodoc
	**/
	position_distance_pair_set();

	/**
	*@nodoc
	**/
	~position_distance_pair_set();

private:
	position_distance_pair_set_impl* m_impl;
	position_distance_pair_set(position_distance_pair_set const&);
	position_distance_pair_set& operator=(position_distance_pair_set const&);
};



/**
* Selects the closest points to a test point from a cloud.
* The resolution data member specifies the resolution at which points
* are considered the same.
**/
class DECL_BASE closest_point_selector: public SPAposition_cloud_selector
{
	SPAposition const& test_point;
	const double resolution;
	position_distance_pair_set results;

public:
	/**
	* Constructor for query.  Requires test point, resolution and cloud.
	**/
	closest_point_selector( 
		SPAposition const& in_test_point, 
		const double in_resolution, 
		SPAposition_cloud& cloud );

	/**
	* This class maintains a sphere centered around the test_point that contains the closest points.
	* Initially the sphere is setup using some guessed points from the cloud.  When this method examines a subcloud,
	* if the subcloud is large (i.e., contains a lot of points) it returns TRUE or FALSE depending on whether
	* the closest point sphere meets the box of the subcloud.  For a small subcloud,
	* this method examines all the contained points and adds any closer points than the current best.
	* It also maintains the closest point sphere.
	**/
	logical examine( SPAposition_subcloud const& subcloud);

	/**
	* Returns the distance between test point and its closest point in the cloud.
	**/
	double get_best_distance() const;

	/**
	* Fills out_closest_points with the points obtained from running the selector.
	**/
	void get_results(SPAposition_vector& out_closest_points ) const;
};


/**
* Selects all points within a sphere.
**/
class DECL_BASE sphere_selector: public SPAposition_cloud_selector
{
	SPAposition const& sphere_center;
	const double sphere_radius;
	SPAposition_vector results;

public:
	/**
	* Constructor requires sphere center and radius
	**/
	sphere_selector( SPAposition const& center, const double radius);

	/**
	* Examine for large subclouds returns TRUE or FALSE depending on whether the test sphere
	* meets the subcloud's box.  For small subclouds, the selector just examines each point by
	* brute force.
	**/
	logical examine( SPAposition_subcloud const& subcloud);

	/**
	* Fills out_sphere_points with the points which are contained in the sphere.
	**/
	void get_results(SPAposition_vector& out_sphere_points ) const;
};

/**
* Selects all points of a cloud within a box.
**/
class DECL_BASE box_selector: public SPAposition_cloud_selector
{
	SPAbox const& m_box;
	SPAposition_vector results;

public:
	/**
	* Constructor requires box to select points from.
	**/
	box_selector( SPAbox const& box);

	/**
	* For large subclouds examine returns TRUE or FALSE depending on whether the selection box meets the
	* subcloud's box.
	*
	* For small subclouds, it checks each point to see whether it is in the box.
	**/
	logical examine( SPAposition_subcloud const& subcloud);

	/**
	* Fills out_selected_points with all the points from the box.
	**/
	void get_results(SPAposition_vector& out_selected_points) const;
};

/**
* Selects all points within a desired thickness of a plane.
**/
class DECL_BASE slice_selector: public SPAposition_cloud_selector
{
public:
	/**
	* Constructor takes root point, normal and thickness for the plane.
	**/
	slice_selector( SPAposition const & in_root, SPAvector const & in_normal, double in_thickness );

	/**
	* Examine for large subclouds returns TRUE or FALSE depending on if the thickened plane meets the
	* subclouds box.  For small subclouds, it just checks each point.
	**/
	logical examine( SPAposition_subcloud const& subcloud);

	/**
	* Fills out_selected_points with the results of the selector.
	**/
	void get_results( SPAposition_vector& out_selected_points ) const;

private:

	typedef enum { back_of_plane, on_plane, front_of_plane } pos_wrt_plane_enum;
	SPAposition root;
	SPAvector normal;
	double thickness;
	SPAposition_vector results;
	slice_selector::pos_wrt_plane_enum dist_from_plane( SPAposition const & test_pos, double & dist );
};

/**
* Selects a user specified number of nearest neighbors of a specified point.
**/
class DECL_BASE nearest_nbrs_selector: public SPAposition_cloud_selector
{
	position_distance_pair_set m_results;
	SPAposition m_test_point;
	int m_num_nbrs;
	double m_farthest_nbr_dist;
public:
	/**
	* Constructor requires test points, cloud, and how many neighboors the query is supposed to find.
	**/
	nearest_nbrs_selector( SPAposition const& test_point, SPAposition_cloud const& cloud, int how_many_nbrs);
	/**
	* Similar to closest_point selector's examine, except now the sphere is the smallest sphere centered on test point
	* that contains the n closest points found so far.
	**/
	logical examine( SPAposition_subcloud const& cloud);

	/**
	* Fills out_results with current
	**/
	void get_results( SPAposition_vector& out_results ) const;
};

/* @} */
#endif
