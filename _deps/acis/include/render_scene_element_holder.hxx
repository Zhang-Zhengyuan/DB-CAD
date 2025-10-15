/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/**
* ACIS Compound
*/
#if !defined(RENDER_SCENE_ELEMENT_HOLDER_HXX)
#define RENDER_SCENE_ELEMENT_HOLDER_HXX
#include "dcl_compound.h"
#include "acis.hxx"
class render_scene_element;
class entity_handle;
class component_handle;
class component_entity_handle;
/**
 * @file render_scene.hxx
 * \addtogroup ACISCOMPOUND
 * @{
 */
/**
*<summary> Scene graph holder, contains an specific type of scene graph </summary>
*/

class DECL_COMPOUND render_scene_element_holder : public ACIS_OBJECT
{
	render_scene_element* _impl;

public:

    render_scene_element_holder();

	render_scene_element_holder(render_scene_element* impl);

    render_scene_element_holder(const render_scene_element_holder& other);

	render_scene_element_holder& operator=(const render_scene_element_holder& other);

	virtual ~render_scene_element_holder();

	void set_impl(render_scene_element* impl);

    render_scene_element* get_impl() const;

	logical operator==(const render_scene_element_holder& rhs);

};
/** @} */
#endif
