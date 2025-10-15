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
#if !defined(RENDER_SCENE_ELEMENT_HXX)
#define RENDER_SCENE_ELEMENT_HXX
#include "dcl_compound.h"
#include "acis.hxx"

#include "render_scene_holder.hxx"
/**
 * @file render_scene_element.hxx
 * \addtogroup ACISCOMPOUND
 * @{
 */
/**
*<summary> Abstract class for scene graph, to be derived per rendering system</summary>
*/
class DECL_COMPOUND render_scene_element : public ACIS_OBJECT
{
    friend class render_scene_element_holder;

    int _count;

    render_scene_holder _owning_scene;

protected:
    
    //const char* _name;

    void add_ref(){_count++;};

    void rem_ref();

    int count(){return _count;};
    
public:
    render_scene_element();

    render_scene_element(const render_scene_holder & owning_scene);

    virtual ~render_scene_element(){};

	virtual const char* get_name()const = 0;

    render_scene_holder get_owning_scene()const {return _owning_scene;}

    void set_owning_scene(const render_scene_holder& owner){_owning_scene = owner;}

};


/** @} */
#endif
