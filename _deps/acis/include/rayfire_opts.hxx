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
/*******************************************************************/
// User ray_fire options class definition. This object is required by
// api_ray_fire and asmi_ray_fire

#ifndef RAY_FIRE_OPTS_HFILE
#define RAY_FIRE_OPTS_HFILE

#include "acis.hxx"
#include "dcl_query.h"

/**
* @file ray_fire_opts.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */


/** \addtogroup INTRAPISILHOUETTE
 *  \brief Declared at <ray_fire_opts.hxx>
 *  @{
 */

/**
 * Represents common data and functionality for ray fire options.
 * <br>
 * <b>Role:</b> <tt>ray_fire_options</tt> is the generic class to be used
 * in <tt>api_ray_fire</tt>.
 * <br><br>
 * @see api_ray_fire
 */
class DECL_QUERY rayfire_options : public ACIS_OBJECT
{
	int _entity_type;
	logical _single_hit_per_entity;

protected:

	
public:

    /**
	 * Constructor.
	 * <br><br>
	 * <b>Role:</b> Creates a ray_fire_options object.
	 * <br><br>
	 */
	rayfire_options():_entity_type(0),_single_hit_per_entity(FALSE) {};
	~rayfire_options() {};

    /**
	 * Returns the value of the entity_type option.
	 */
	int get_entity_type() const { return _entity_type; }

    /**
	 * Sets the entity type. Valid values are the enums "VERTEX_TYPE", "EDGE_TYPE" or "FACE_TYPE".
	 */
	void set_entity_type(int ent_type) { _entity_type = ent_type; }

	/**
	 * Returns the value of the single_hit_per_entity option.
	 */
	logical get_single_hit_per_entity() const { return _single_hit_per_entity; }

	/**
	 * Set the value of the single_hit_per_entity option. The default value is false. If set to true, the first hit for each hit entity will be returned.
	 */
	void set_single_hit_per_entity( logical set ) { _single_hit_per_entity = set; }
};

/** @} */

#endif // SIL_OPTS_HFILE
