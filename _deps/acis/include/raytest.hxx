/* ORIGINAL: acis2.1/kerndata/raytest/raytest.hxx */
/* $Id: raytest.hxx,v 1.11 2002/08/09 17:18:48 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header file for classes ray and hit, and for ray test routines.
//
// History:
//
// TH 27-Nov-98    Updating entity_hit class
// TH 21-Nov-08    Added entity_hit_list class
// DM 27-May-08	   Added grouping for documentation
// AL 04-Apr-2006: Moved curve ray testing functions into protected headers
//                 and added ray_curve_edge (general curve ray test).
//
/*******************************************************************/


#if !defined( RAY_TEST )
#define RAY_TEST

#include <float.h>

#include "dcl_query.h"
#include "logical.h"
#include "debugmsc.hxx"
#include "intcusf.hxx"
#include "param.hxx"
#include "position.hxx"
#include "unitvec.hxx"

/**
* @file raytest.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

/** \addtogroup INTRAPIRAYTEST
 *  \brief Declared at <raytest.hxx>
 *  @{
 */

class SPAbox;
class curve;
class intcurve;
class ellipse;
class SPAparameter;
class straight;
class SPAtransf;
class BODY;
class EDGE;
class ENTITY;
class ENTITY_LIST;
class FACE;
class LUMP;
class SUBSHELL;
class SHELL;
class VERTEX;
class WIRE;

// Define a class to represent a 3d ray.

class ray;
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_QUERY ray operator*( ray const &, SPAtransf const & );
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_QUERY logical operator&&( ray const&, SPAbox const& );
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_QUERY logical rayxbox( ray const &r, SPAbox const &b,
	SPAinterval& ray_portion = *(SPAinterval*) NULL_REF );

/**
 * Represents a 3D ray.
 * @see SPAposition, SPAunit_vector
 */
class DECL_QUERY ray : public ACIS_OBJECT {

public:

/**
 * The point on the ray.
 */
	SPAposition root_point;	// point on ray
/**
 * The direction of the ray.
 */
	SPAunit_vector direction;	// direction of ray
/**
 * The ray radius.
 */
	double radius;			// ray radius
/**
 * The maximum parameter value of hits that have been found so far.
 */
	double max_p;		// maximum SPAparameter value of hits so far found
/**
 * The maximum number of hits wanted.
 * <br><br>
 * <b>Role:</b> If this value is 0, it means that all hits are wanted.
 */
	int hits_wanted;	// maximum number of hits wanted (0 means all)
/**
 * The number of hits found.
 */
	int hits_found;			// number of hits found

	// Construct a ray from a SPAposition and a SPAunit_vector and
	// optional ray radius and maximum number of hits wanted.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param pos
 * point on ray.
 * @param uv
 * direction of ray.
 * @param rad
 * ray radius.
 * @param hit
 * maximum number of hits.
 */
	ray( SPAposition const &pos, SPAunit_vector const &uv,
		 double rad = 100.0 * SPAresabs, int hit= 0 );

	// Construct a ray as a copy of another ray.

/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param r
 * ray.
 */
	ray( ray const &r );

/**
 * C++ assignment operator
 * <br><br>
 * @param r
 * ray.
 */
	ray &operator=( ray const &r );

	// Transform a ray.
/*
// tbrv
*/
/**
 * @nodoc
 */
	friend DECL_QUERY ray operator*( ray const &, SPAtransf const & );
/**
 * Transforms a ray.
 * <br><br>
 * @param transf
 * transform.
 */
	ray &operator*=( SPAtransf const &transf );

	// Test if ray cuts a SPAbox.
/*
// tbrv
*/
/**
 * @nodoc
 */
	friend DECL_QUERY logical operator&&( ray const&, SPAbox const& );
/*
// tbrv
*/
/**
 * @nodoc
 */
	friend DECL_QUERY logical rayxbox(
			ray const &r,
			SPAbox const &b,
			SPAinterval& ray_portion );
	// Print out details of the ray for information
/**
 * Outputs debug information to the printer or to the specified file.
 * <br><br>
 * @param fp
 * file name.
 */
	void debug( FILE * fp = debug_file_ptr ) const;
};

/** @} */
/** \addtogroup INTRAPIRAYTEST
 *  \brief Declared at <raytest.hxx>
 *  @{
 */

// Define a class to represent an intersection of a ray with a
// face, edge or vertex.

// Two types of hit can occur: a hit through a face, edge or vertex
// where the ray coincides with entity hit at an isolated point; and
// a hit along a (flat or ruled surface) face or (straight) edge, where
// the ray coincides with a finite region of the entity hit (when the
// SPAparameter value is set to a value corresponding to a point within
// the coincident region of the ray).
/*
// tbrv
*/
/**
 * @nodoc
 */
enum hit_type {hit_thru, hit_along };
/**
 * Represents an intersection of a ray with a face, edge, or vertex.
 * <br>
 * <b>Role:</b> This class represents an intersection of a ray with a face, edge,
 * or vertex. Two types of hits can occur: a hit through a face, edge, or
 * vertex where the ray coincides with the entity hit at an isolated point; and
 * a hit along a (flat or ruled surface) face or (straight) edge, where the ray
 * coincides with a finite region of the entity hit (when the parameter value
 * is set to a value corresponding to a point within the coincident region of the ray).
 * @see SPAparameter, ENTITY
 */
class DECL_QUERY hit : public ACIS_OBJECT {

public:
/**
 * The next hit in a chain of hits.
 */
	hit*			next;			// next hit in chain of hits
/**
 * The face, edge, or vertex that is hit.
 */
	ENTITY*			entity_hit;		// face, edge or vertex
/**
 * The type of hit, which can be either through a face edge or vertex (<tt>hit_thru</tt>) or along a face or edge (<tt>hit_along</tt>).
 */
	hit_type		type_of_hit;	// thru or along
/**
 * An enumerated type that describes the relationship between the ray and the surface in one half neighborhood of the intersection.
 */
	curve_surf_rel	ray_surf_rel;	// ray-surface relation
/**
 * If the type of hit is the through type, this provides the parameter value on the ray.
 */
	SPAparameter	ray_param;		// if thru type, gives SPAparameter
									// value on ray	
/**
 * The entity's SPAparameter(s) of the hit
 */
	double			u_or_t;			// If entity is a FACE, this is the u param.  If EDGE, this is the t param.
	double			v;				// If entity is a FACE, this is the v param.  If EDGE, this is DBL_MAX.

/**
 * The face, edge, or vertex that is hit. Used by utility functions to test if two hits are on the same object.
 */
	virtual void* what_hit() const;

	// Constructor for a hit.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param inithit
 * initialize entity hit.
 * @param type
 * initialize the type of hit.
 * @param para
 * initialize the ray parameter.
 * @param rel
 * initialize the ray surface relation.
 * @param npt
 * initialize the next pointer.
 */
	hit
		(
		ENTITY* inithit,
		hit_type type,
		double para,
		curve_surf_rel rel = curve_unknown,
		hit* npt = NULL,
		double u_or_t = DBL_MAX,
		double v = DBL_MAX
	);

	virtual ~hit();
	// Print out details of the hit for information.
/**
 * Outputs information about the debug to the printer or to the specified file.
 * <br><br>
 * @param fp
 * file name.
 */
	void debug( FILE * fp= debug_file_ptr ) const;
};

// Find length and max. param. of a hit list.

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_QUERY int enquire_hit_list( hit*, double& = *(double*)NULL_REF );

/**
* Deletes a hit list.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param list
* hit list to delete.
**/
DECL_QUERY void delete_hit_list( hit* list);
DECL_QUERY void gme_delete_hit_list( hit* list);

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_QUERY void debug_hit_list( hit* );

/** @} */
/** \addtogroup INTRAPIRAYTEST
 *  \brief Declared at <raytest.hxx>
 *  @{
 */

// Define the non-member functions for testing rays.

//		The routine raytest fires a ray at a body and returns on the heap
//	a list of hits (each recording the entity hit, the type of hit, and
//	the SPAparameter value at the hit) or NULL if no hits are found.
//	A value of zero for hits permitted means all hits are returned.
//	Logicals allow shells or wires or both to be selected as
//	targets for	the ray.  Only entities in the forward direction
//	along the ray can be hit.
//		If the ray hits the interior of a face, the face is
//	returned; if it hits an edge of a face, i.e. two adjacent
//	faces, the edge is returned; and if it hits all faces adjacent
//	at a vertex, the vertex is returned.
//		A call to this routine may cause boxes to be computed and
//	thus the model may change.
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_QUERY hit* raytest
(
	ray&,					// ray
	int,					// number of target entities
	ENTITY*[]				// array of target entities
);
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_QUERY hit* raytest_body
(
	ray&,					// ray
	BODY*					// target_body
);
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_QUERY hit* raytest_wire
(
	ray&,					// ray
	WIRE*					// target_wire
);

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_QUERY hit* raytest_lump
(
	ray&,					// ray
	LUMP*,					// target_lump
	SHELL*	skip_shell=NULL
);

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_QUERY hit* raytest_shell
(
	ray&,					// ray
	SHELL*					// target_shell
);

/**
 * Tests a ray against the given face.
 * <br><br>
 * <b>Role:</b> The routine raytest fires a ray at a body and returns on the
 * heap a list of hits or NULL if no hits are found. The heap contains the entity
 * hit, the type of hit, and the parameter value at the hit. A value of zero for
 * hits permitted means all hits are returned. Logicals allow shells or wires or
 * both to be selected as targets for the ray. Only entities in the forward
 * direction along the ray can be hit.
 * <br><br>
 * If the ray hits the interior of a face, the face is returned; if it hits an edge
 * of a face (i.e., two adjacent faces), the edge is returned; and if it hits all
 * faces adjacent at a vertex, the vertex is returned.
 * <br><br>
 * <b>Limitations:</b> A call to this routine may cause boxes to be computed and thus
 * the model may change.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param this_ray
 * ray.
 * @param this_face
 * target face.
 **/
DECL_QUERY hit* raytest_face
(
	ray& this_ray,					// ray
	FACE* this_face					// target_face
);
DECL_QUERY hit* gme_raytest_face
(
	ray& this_ray,					// ray
	FACE* this_face					// target_face
);

/**
 * Tests a ray against the given edge.
 * <br><br>
 * <b>Role:</b> The routine raytest fires a ray at a body and returns on the
 * heap a list of hits or NULL if no hits are found. The heap contains the entity
 * hit, the type of hit, and the parameter value at the hit. A value of zero for
 * hits permitted means all hits are returned. Logicals allow shells or wires or
 * both to be selected as targets for the ray. Only entities in the forward
 * direction along the ray can be hit.
 * <br><br>
 * If the ray hits the interior of a face, the face is returned; if it hits an edge
 * of a face (i.e., two adjacent faces), the edge is returned; and if it hits all
 * faces adjacent at a vertex, the vertex is returned.
 * <br><br>
 * <b>Limitations:</b> A call to this routine may cause boxes to be computed and thus
 * the model may change.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param this_ray
 * ray.
 * @param this_edge
 * target edge.
 **/
DECL_QUERY hit* raytest_edge
(
	ray& this_ray,					// ray
	EDGE* this_edge					// target_edge
);
DECL_QUERY hit* gme_raytest_edge
(
	ray& this_ray,					// ray
	EDGE* this_edge					// target_edge
);

/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_QUERY hit* raytest_vertex
(
	ray&,					// ray
	VERTEX*					// target_vertex
);
DECL_QUERY hit* gme_raytest_vertex
(
	ray&,					// ray
	VERTEX*					// target_vertex
);

// Merge two already ordered lists of hits, truncate to wanted number
// of hits, and update ray with number of hits and maximum SPAparameter.
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_QUERY hit* merge_hits
(
	hit*,				// first list
	hit*,				// second list
	ray&,
	double tol = SPAresabs
);
DECL_QUERY hit* gme_merge_hits
(
	hit*,				// first list
	hit*,				// second list
	ray&,
	double tol = SPAresabs
);

/** @} */


// CLASSES FOR RAYFIRE INTERFACE
class component_entity_handle;
class object_hit_array;


class DECL_QUERY object_hit : public ACIS_OBJECT
{
protected:
	double		_ray_param;
	void*		_hit_object;
	double		_u_or_t;	// If FACE, this is the _hit_object's surface u param. If EDGE, it's the edge t param.
	double		_v;			// If FACE, this is the _hit_object's surface v param. If EDGE, it's DBL_MAX.
public:
	object_hit( double in_ray_param, void* in_entity = 0, double in_u_or_t = DBL_MAX, double in_v = DBL_MAX ) :
		_ray_param( in_ray_param ), _hit_object( in_entity ), _u_or_t( in_u_or_t ), _v( in_v ) {};
	object_hit();
	object_hit( const object_hit& );
	object_hit &operator=( const object_hit& );
	~object_hit() {};
	const double hit_param() { return _ray_param; }
	const double entity_u_or_t() { return _u_or_t; }
	const double entity_v() { return _v; }
};

/**
 * Represents an intersection of a ray with a face, edge, or vertex.
 * <br>
 * <b>Role:</b> This class represents a clash of a ray with a face, edge, or vertex.
 * The parameter value is a value where the ray is within ray radius of the hit entity.
 * @see SPAparameter, ENTITY
 */
class DECL_QUERY entity_hit : public object_hit
{
public:
	entity_hit( ENTITY* hit_ent, double param, double u_or_t = DBL_MAX, double v = DBL_MAX ) : object_hit( param, hit_ent, u_or_t, v ) {};
	~entity_hit() {};
	/**
	 * Returns the ray parameter of a hit.
	 */
	const double hit_param() { return _ray_param; }
	/**
	 * Returns the face, edge or vertex that is hit.
	 */
	ENTITY* hit_entity() { return (ENTITY*)_hit_object; }

	/**
	 * Returns the entity parameter of a hit. If the hit entity is an <tt>EDGE</tt> then its parameter "t"
	 * is returned. If the hit entity is a <tt>FACE</tt> then it returns the "u" parameter.
	 */
	const double entity_u_or_t() { return _u_or_t; }
	
	/**
	 * Returns the "v" parameter of a hit if the hit entity is a <tt>FACE</tt>.
	 */

	const double entity_v() { return _v; }
};

/**
 * entity_hit_list中用到object_hit_array来存储entity_hit，但并未给出object_hit_array的定义。
 * 因此在此将object_hit_array定义成链表节点。
 * @see entity_hit_list
 */
class object_hit_array : public ACIS_OBJECT
{
public:
	entity_hit* hit;
	object_hit_array *next;
};

/**
 * Represents a variable length, read-only list of entity_hit objects.
 * <br>
 * <b>Role:</b> This class provides the mechanism for returning ray hits
 * to the caller of api_ray_fire.
 * @see entity_hit, SPAparameter, ENTITY
 */

class DECL_QUERY entity_hit_list : public ACIS_OBJECT
{
	object_hit_array* _hit_array;
	logical _reversed;

public:

	entity_hit_list();
	~entity_hit_list();
/**
 * Returns number of items in the entity_hit_list.
 */
	int count() const;

/**
 * 返回长度，当使用gme函数得到entity_hit_list时，请使用gme_count。
 */
	int gme_count() const;

/**
 * Returns a pointer to the ith item in the entity_hit_list.
 * The order of the hits returned can be changed using the
 * functions forward_order and reverse_order. The default
 * is forward order; that is, with increasing parameter values.
 */
       entity_hit* operator[](int i);

/**
 * 对标[]操作符的函数，当使用gme函数得到entity_hit_list时，请使用at而不是[]
 */
       entity_hit* at(int i);
/**
 * The entity_hits returned by operator[] are in increasing parameter order.
 */
       void forward_order();
/**
 * The entity_hits returned by operator[] are in decreasing parameter order.
 */
       void reverse_order();

/**
 * @nodoc
 */
	object_hit_array* get_impl() const;

/**
 * 在末尾添加一个entity_hit，因为acis并未提供构造添加entity_hit的方法，因此创建此函数。
 */
	void push_back( entity_hit* );
};




#endif
