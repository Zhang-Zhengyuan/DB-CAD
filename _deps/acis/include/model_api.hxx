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
#if !defined(MODEL_API_HXX)
#define MODEL_API_HXX
#include "dcl_compound.h"
#include "api.hxx"
#include "asm_model.hxx"
#include "asm_api.hxx"
#include "rgbcolor.hxx"
#include "asm_model_element.hxx"
/**
 * @file model_api.hxx
 * \addtogroup ACISCOMPOUND
 * @{
 */
/**
* <summary> Save a models by specifying the file name.</summary>
* <param name = "file_name">File name.</param>
* <param name = "product_id">Product identifier to be saved with the model.</param>
* <param name = "model">Input model.</param>
* <param name = "ao">ACIS options.</param>
* <remarks>Will return an error if the file cannot be opened or if its name does not include the extension "asat", "asab", "sat", or "sab".</remarks>
* <returns>outcome object</returns>
*/
DECL_COMPOUND outcome asmi_save_model_contents(const char* file_name, const char* product_id, asm_model* model,	AcisOptions* ao);
/**
* <summary> Restore list of models by specifying the file name.</summary>
* <param name = "file_name">File name</param>
* <param name = "primary_models">Output model list</param>
* <param name = "restore_options">Restore file options.</param>
* <param name = "ao">ACIS options.</param>
* <remarks>Will return an error if the file does not exit or if its name does not include the extension "asat", "asab", "sat", or "sab".</remarks>
* <returns>outcome object</returns>
*/
DECL_COMPOUND outcome asmi_restore_model_list(const char* file_name, asm_model_list& primary_models, asm_restore_options* restore_options = NULL, AcisOptions * ao=NULL);
/**
* <summary>Returns the box of an assembly in the context of the assembly's root model.</summary>
* <param name="model">The model whose box is to be calculated.</param>
* <param name="box">Box of the component in the space of the assembly's root model.</param>
* <param name="ao">ACIS options.</param>
**/
DECL_COMPOUND outcome asmi_model_get_box(asm_model* model, SPAbox& box, AcisOptions * ao=NULL);
/**
* <summary>Highlights or un-highlights a component entity with the given color.</summary>
* <param name="ce">The given component entity.</param>
* <param name="color">Color to be used.</param>
* <param name="ao">ACIS options.</param>
*/
DECL_COMPOUND outcome asmi_set_entity_color(component_entity_handle* ce, const rgb_color& color, AcisOptions* ao = NULL);
/**
* <summary>Gets the RGBColor of an EntityCli.</summary>
* <param name="ce">The given component EntityCli.</param>
* <param name="color">Color returned.</param>
* <param name="inherit">Traverse up the tree to find the inherited color.</param>
* <param name="found">Is this color explicitly set (not default).</param>
* <param name="ao">ACIS options.</param>
*/
DECL_COMPOUND outcome asmi_get_entity_color(component_entity_handle* ce, rgb_color& color, const logical& inherit, logical& found, AcisOptions* ao = NULL);


DECL_COMPOUND outcome asmi_model_get_root_element(asm_model* model, asm_model_element*& element, AcisOptions* ao  = NULL);
/** @} */
#endif
