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
#if !defined(RENDER_BRIDGE_HXX)
#define RENDER_BRIDGE_HXX
#include "dcl_compound.h"
#include "acis.hxx"
#include "api.hxx"
#include "render_scene_element_holder.hxx"
#include "render_scene_holder.hxx"
#include "render_scene_element.hxx"
#include "render_scene.hxx"
#include "rgbcolor.hxx"
#include "asm_api.hxx"
#include "asm_model_element.hxx"
/**
 * @file rendering_bridge.hxx
 * \addtogroup ACISCOMPOUND
 * @{
 */
/**
* <summary>Abstract class for bridge between rendering engine and ACIS functionality</summary>
*/
class DECL_COMPOUND render_bridge : public ACIS_OBJECT
{

	virtual void render_bridge_destructor_vf(){};

	virtual outcome initialize_vf() = 0;

	virtual outcome terminate_vf() = 0;

	virtual outcome render_model_vf(asm_model* model, render_scene_holder& scene, AcisOptions * ao=NULL) = 0;

	virtual outcome get_render_scenes_vf(asm_model* model, unsigned long& nscenes, render_scene_holder*& scenes, AcisOptions* ao = NULL) = 0;

    virtual outcome delete_render_scene_contents_vf(const render_scene_holder& scene, AcisOptions* ao = NULL) = 0;

    virtual outcome highlight_model_element_vf(asm_model_element* elem, const rgb_color& color, const logical& on, AcisOptions * ao=NULL) = 0;

    virtual outcome highlight_render_scene_element_vf(const render_scene_element_holder& elem, const logical& on, AcisOptions * ao=NULL) = 0;
    
    virtual outcome hide_render_scene_element_vf(const render_scene_element_holder& elem, const logical& on, AcisOptions * ao=NULL) = 0;

	virtual render_scene_holder make_empty_render_scene_vf() = 0;

	virtual render_scene_element_holder make_empty_render_scene_element_vf() = 0;
    

public:
	render_bridge(){};

	~render_bridge(){ render_bridge_destructor_vf();};

	outcome initialize();

	outcome terminate();

	outcome render_model(asm_model* model, render_scene_holder& scn_grph, AcisOptions * ao=NULL);

	outcome get_render_scenes(asm_model* model, unsigned long& nscenes, render_scene_holder*& scenes, AcisOptions* ao = NULL);

    outcome delete_render_scene_contents(const render_scene_holder& scene, AcisOptions* ao = NULL);

    outcome highlight_model_element(asm_model_element* elem, const rgb_color& color, const logical& on, AcisOptions * ao=NULL);

    outcome highlight_render_scene_element(const render_scene_element_holder& elem, const logical& on, AcisOptions * ao=NULL);

    outcome hide_render_scene_element(const render_scene_element_holder& elem, const logical& on, AcisOptions * ao=NULL);

	render_scene_holder make_empty_render_scene();

	render_scene_element_holder make_empty_render_scene_element();
};

/** @} */
#endif
