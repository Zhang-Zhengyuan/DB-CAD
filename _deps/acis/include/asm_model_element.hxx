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
#if !defined(ASM_MODEL_ELEMENT_HXX)
#define ASM_MODEL_ELEMENT_HXX
#include "dcl_compound.h"
#include "acis.hxx"
class entity_handle;
class component_handle;
class component_entity_handle;
class entity_handle_holder;
class component_handle_holder;
class component_entity_handle_holder;
/**
 * @file asm_model_element.hxx
 * \addtogroup ACISCOMPOUND
 * @{
 */
/**
*<summary> Abstract class for model elements</summary>
*/
class DECL_COMPOUND asm_model_element: public ACIS_OBJECT
{
    enum model_element_type{
    COMPONENT,
    COMPONENT_ENTITY,
    ENTITY_HANDLE
    }_type;

    component_handle_holder* _component;

    component_entity_handle_holder* _component_entity;

    entity_handle_holder* _entity_handle;

    asm_model_element();
public:

    asm_model_element(const asm_model_element& element); 

    asm_model_element(component_handle* element);

    asm_model_element(component_entity_handle* element);

    asm_model_element(entity_handle* element);

    ~asm_model_element();

    const char* type_name();

    //void get_children(asm_model_element**& elems, int& nelem); TODO: Belongs in different classs (TreeElement or TreeStructure)

    ACIS_OBJECT* asm_element();

};
/** @} */
#endif
