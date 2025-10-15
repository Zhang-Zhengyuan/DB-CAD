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
#if !defined(RENDER_SCENE_HXX)
#define RENDER_SCENE_HXX
#include "dcl_compound.h"
#include "acis.hxx"
#include "render_scene_element_holder.hxx"
class entity_handle;
class component_handle;
class component_entity_handle;
/**
 * @file render_scene.hxx
 * \addtogroup ACISCOMPOUND
 * @{
 */
/**
*<summary> Abstract class for scene graph, to be derived per rendering system</summary>
*/
class DECL_COMPOUND render_scene : public ACIS_OBJECT
{
    friend class render_scene_holder;

    int _count;

    render_scene_element_holder _root;
protected:

	const char* _name;
 
    void add_ref(){_count++;};

    void rem_ref();

    int count(){return _count;};
public:
	render_scene();

    render_scene_element_holder root() {return _root;}

    void set_root(const render_scene_element_holder& root){_root = root;}

    virtual ~render_scene();

	virtual const char* get_name()const = 0;

    virtual void highlight(entity_handle* element) = 0;

    virtual void highlight(component_handle* element) = 0;

    virtual void highlight(component_entity_handle* element) = 0;
    
    virtual void set_is_hidden(const logical& on) = 0;

    virtual logical get_is_hidden() = 0;

    virtual void clear_all_highlights() = 0;



};
/** @} */
#endif
