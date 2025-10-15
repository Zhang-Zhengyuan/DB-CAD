/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef SPAepd_result_hxx
#define SPAepd_result_hxx
#include "mmgr.hxx"
#include "dcl_query.h"
#include "intrapi.hxx"
class ENTITY;
class FACE;
#include "position.hxx"
#include "param.hxx"
#include "unitvec.hxx"

class SPAepd_result_list_impl;
class SPAepd_result;
class SPAepd_options_impl;

/**
* @file SPAepd_result.hxx
* @CAA2Level L1
* @CAA2Usage U1
*! \addtogroup FCTAPI
*  \brief Declared at <SPAepd_result.hxx>
*  @{
*/



class DECL_QUERY SPAepd_options: public ACIS_OBJECT
{
public:
	enum distance_mode
	{
		distance_to_solid_boundary,
		distance_to_solid_interior,
		positive_if_outside_negative_if_inside
	};
	/**
	* Specifies the meaning of the distance returned by the API.
	*
	* In detail:
	* distance_to_solid_boundary mode returns a positive distance which is 
	* the distance to the closest FACE, EDGE or VERTEX on the boundary of a solid.
	*
	* distance_to_solid_interior mode returns a positive distance if the point is
	* outside the body, and zero otherwise.
	*
	* positive_if_outside_negative_if_inside returns a signed distance (where inside 
	* is negative, outside is positive).  In closest_point_on_face containment mode,
	* the sign is determined by dotting the outward facing normal with the closest point to 
	* test point vector.
	**/
	void set_distance_mode( distance_mode mode );
	/**
	* returns the distance mode used by this options object
	**/
	distance_mode get_distance_mode() const;

	enum containment_check_mode
	{
		closest_point_on_face,	// only check containment if closest point is on a face.
		closest_point_all		// always check containment.
	};
	/**
	* Sets when the API will do a containment check.  It is faster to use
	* closest_point_on_face mode, but in this case whenever the 
	* closest point is on an EDGE or VERTEX, the distance returned 
	* is positive regardless of containment status.
	**/
	void set_containment_check( containment_check_mode mode );
	/**
	* Returns containment check mode.
	**/
	containment_check_mode get_containment_check() const;

	/**
	* Default constructor.
	**/
	SPAepd_options();
	/**
	* Destructor
	**/
	~SPAepd_options();

	/**
	* @nodoc
	**/
	SPAepd_options_impl* get_impl();
	/**
	* copy constructor
	**/
	SPAepd_options(SPAepd_options const& other);
	/**
	* assignment operator
	**/
	SPAepd_options& operator=(SPAepd_options const& other);
private:
	SPAepd_options_impl* m_impl;
};

class DECL_QUERY SPAepd_result_list : public ACIS_OBJECT
{
public:
	/**
	* resets iterator to point at first results.
	**/
	void init();

	/**
	* Writes at most num_requested SPAepd_results into the buffer.
	* The return value gives the actual number of results written.
	* The client is responsible for deallocating the buffer (this is automatic if
	* the buffer is put on the stack as in the following code
	*
	* SPAepd_result buffer[10]
	* int num_returned = result_list.next_results( 10, buffer );
	**/
	int next_results( int num_requested, SPAepd_result* buffer );

	/**
	* return number of result objects in the list.
	**/
	int size() const;

	/**
	* Default constructor.  Creates empty result list.
	**/
	SPAepd_result_list();

	/**
	* Copy constructor
	**/
	SPAepd_result_list( SPAepd_result_list const& other);

	/**
	* Assignment operator.
	**/
	SPAepd_result_list& operator=(SPAepd_result_list const& other);

	/**
	* Destructor.  Cleans up result list.
	**/
	~SPAepd_result_list();

	// internal use only
	/**
	* @nodoc
	**/
	void set_impl( SPAepd_result_list_impl* impl );

	// internal use only
	/**
	*@nodoc
	**/
	SPAepd_result_list_impl* get_impl();
private:
	SPAepd_result_list_impl* m_impl;
};


class DECL_QUERY SPAepd_result :public ACIS_OBJECT
{
public:
	/**
	* Returns the closest distance between point and entity
	**/
	double get_distance() const;

	/**
	* Returns closest point on entity to test point.
	**/
	SPAposition get_closest_point() const;
	
	/**
	* Returns type of closest entity (ent_is_edge, ent_is_face, or ent_is_vertex).
	**/
	param_info_type get_closest_entity_type() const;

	/**
	* Returns pointer to closest entity (FACE, EDGE or VERTEX).
	**/
	ENTITY* get_closest_entity() const;

	/**
	* If closest entity is a FACE returns parametric coordinates of the closest point on that FACE.
	**/
	SPApar_pos get_uv() const;

	/**
	* Returns parametric coordinates of the closest point on that FACE.
	**/
	SPApar_pos get_uv(FACE* which_face) const;

	/**
	* If point is on EDGE, returns curve parameter for that point.  Otherwise throws.
	**/
	double get_t() const;

	/**
	* Returns the normal of which_face at the closest point.  Throws
	* an error if the closest point is not on the specified which_face.
	**/
	SPAunit_vector get_normal( FACE* which_face ) const;

	/**
	* Default constructor; do not query uninitialized/default objects.
	**/
	SPAepd_result();
	/**
	* Destructor
	**/
	~SPAepd_result();
	/**
	* Copy constructor
	**/
	SPAepd_result( SPAepd_result const& other);
	/**
	* Assignment operator.
	**/
	SPAepd_result& operator=(SPAepd_result const& other);

	// internal use only
	/**
	* @nodoc
	**/
	SPAepd_result( SPAepd_result_list_impl* impl, int idx);

	/**
	*@nodoc
	**/
	int get_index_in_cloud() const;
private:
	int m_idx;
	SPAepd_result_list_impl* m_impl;
};

class SPAposition_cloud;

/**
* Finds the closest point on the given entity's boundary for each point in 
* the cloud. Stores result in SPAepd_result_list object.
**/
DECL_QUERY outcome api_entity_point_distance( 
					ENTITY* entity,
					SPAposition_cloud const& cloud,
					SPAepd_result_list& results,
					SPAepd_options* opts=NULL,
					AcisOptions* ao=NULL );
/**
* Finds the closest point on the given entities' boundary for each point in 
* the cloud. Stores result in SPAepd_result_list object.
**/
DECL_QUERY outcome api_entity_point_distance( 
					ENTITY_LIST const& entities,
					SPAposition_cloud const& cloud,
					SPAepd_result_list& results,
					SPAepd_options* opts=NULL,
					AcisOptions* ao=NULL );


//experimental
/**
* @nodoc
**/
class SPAepd_result_partitioner_impl;
class DECL_QUERY SPAepd_result_partitioner: public ACIS_OBJECT
{
public:
	/**
	* @nodoc
	**/
	SPAepd_result_partitioner( SPAepd_result_list& epd_results);
	/**
	* @nodoc
	**/
	~SPAepd_result_partitioner();

	/**
	* @nodoc
	**/
	void bin_results( int num_bin_pts, double const* bin_pts );
	/**
	* @nodoc
	**/
	int num_bins() const;
	/**
	* @nodoc
	**/
	// assumes which_bin >=0 && which_bin < num_bin_pts - 1
	SPAposition_cloud get_bin( int which_bin) const;

	/**
	* @nodoc
	**/ 
	SPAepd_result_partitioner_impl* get_impl();
private:
	SPAepd_result_partitioner_impl* m_impl;
	SPAepd_result_partitioner(SPAepd_result_partitioner const&);
	SPAepd_result_partitioner& operator=(SPAepd_result_partitioner const&);
};

/* @} */
#endif
