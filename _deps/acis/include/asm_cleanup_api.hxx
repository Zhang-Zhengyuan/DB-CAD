/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ASM_CLEANUP_API_HXX
#define ASM_CLEANUP_API_HXX

#include "dcl_asm.h"
#include "asm_event_type.hxx"
#include "api.hxx"
#include "asm_api.hxx"

class asm_model;
class asm_model_list;
class asm_model_entity_mgr;
class entity_mgr_factory;
class entity_handle;
class entity_handle_list;
class component_handle;
class component_handle_list;
class asm_model_info;
class asm_get_options;
class asm_cleanup_options;

/**
* @file asm_cleanup_api.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISASSEMBLIES
 *
 * @{
 */

//////////////////////
// Interfaces to delete unneeded models and handles from memory
//////////////////////

/**
 * Deletes all unneeded model and handle objects.  
 * <br><br>
 * <b>Role:</b> An object is considered unneeded if it is not being held by any other
 * object (see technical article on assembly-modeling use counting).
 * This routine (recursively) asks the global model manager to search for and delete any
 * <tt>asm_model</tt> objects that are not being held.
 * It also calls cleanup on each <tt>asm_model</tt> object, which removes unneeded handles
 * from the model objects.
 * This procedure is performed recursively until only objects that are being held remain.  
 * An <tt>asm_cleanup_options</tt> object can be passed in to specify which types of handle
 * are cleaned up.  The default behavior is to clean all handle types.
 * <br><br>
 * <b>Errors:</b> None.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param copts
 * cleanup options object, which determines which types of handle will be cleaned.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_cleanup(asm_cleanup_options const* copts = NULL, 
							  AcisOptions* ao = NULL);

/**
 * Deletes a model object and its handle objects, if unneeded.
 * <br><br>
 * <b>Role:</b> An object is considered unneeded if it is not being held by any other
 * object (see technical article on assembly-modeling use counting).
 * This routine tests a single model object to see if it is being held.  If it is not
 * being held, the model object is deleted.
 * If the model object is being held (and so is not deleted), the routine attempts to clean
 * up the handles owned by that object. 
 * An <tt>asm_cleanup_options</tt> object can be passed in to specify which types of
 * handle are cleaned up.  The default behavior is to clean all handle types.
 * <br><br>
 * <b>Errors:</b> None.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param model
 * model to be cleaned (is set to NULL if the model is deleted).
 * @param copts
 * cleanup options object, which determines which types of handle will be cleaned.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_model_cleanup(asm_model*& model, 
							  asm_cleanup_options const* copts = NULL, 
							  AcisOptions* ao = NULL);

/**
 * Deletes a list of model objects and their handle objects, if unneeded.
 * <br><br>
 * <b>Role:</b> An object is considered unneeded if it is not being held by any other
 * object (see technical article on assembly-modeling use counting).
 * This routine tests each model object to see if it is being held.  If it is not being
 * held, the model object is deleted.
 * If the model object is being held (and so is not deleted), the routine attempts to clean
 * up the handles owned by that object. 
 * An <tt>asm_cleanup_options</tt> object can be passed in to specify which types of handle
 * are cleaned up.  The default behavior is to clean all handle types.
 * <br><br>
 * <b>Errors:</b> None.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param model_list
 * list of models to be cleaned (models that are deleted are removed from the list).
 * @param copts
 * cleanup options object, which determines which types of handle will be cleaned.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_model_cleanup(asm_model_list& model_list, 
							  asm_cleanup_options const* copts = NULL, 
							  AcisOptions* ao = NULL);

/**
 * Deletes all sub-models of a list of model objects and their handle objects,
 * if unneeded.
 * <br><br>
 * <b>Role:</b> An object is considered unneeded if it is not being held by any other
 * object (see technical article on assembly-modeling use counting).
 * This routine sets the input list to not hold the models in the list.  It finds all sub-models
 * of the models in the list (including the models in the list) tests each
 * sub-model object to see if it is being held.  If it is not being held, the (sub-)model object
 * is deleted.
 * If the model object is being held (and so is not deleted), the routine attempts to clean
 * up the handles owned by that object. 
 * <br><br>
 * If the <tt>force_clear</tt> flag is set to TRUE, the <tt>clear</tt> method is called on
 * the entity managers of all the sub-models, even if the use count is not zero, and the
 * <tt>asm_model</tt>
 * objects are removed from the global model manager and put into a state that they will
 * automatically be deleted (without a further call to cleanup)
 * when their use count goes to zero.
 * <br><br>
 * An <tt>asm_cleanup_options</tt> object can be passed in to specify which types of handle
 * are cleaned up.  The default behavior is to clean all handle types.
 * <br><br>
 * <b>Errors:</b> None.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param model_list
 * list of models to be cleaned (models that are deleted are removed from the list).
 * @param force_clear
 * logical to force associated entity managers to be cleared regardless of model use count.
 * @param copts
 * cleanup options object, which determines which types of handle will be cleaned.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_model_cleanup_tree(asm_model_list& model_list, 
							  logical force_clear,
							  asm_cleanup_options const* copts = NULL, 
							  AcisOptions* ao = NULL);

/**
 * Deletes all unneeded handle objects.
 * <br><br>
 * <b>Role:</b> An object is considered unneeded if it is not being held by any other
 * object (see technical article on assembly-modeling use counting).
 * This routine (recursively) asks the global model manager to call cleanup on each
 * <tt>asm_model</tt> object, which removes unneeded handles from the model object.
 * This procedure is performed recursively until only handles that are being held remain.  
 * An <tt>asm_cleanup_options</tt> object can be passed in to specify which types of handle
 * are cleaned up.  The default behavior is to clean all handle types.
 * <br><br>
 * <b>Errors:</b> None.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param copts
 * cleanup options object, which determines which types of handle will be cleaned.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_cleanup_handles(asm_cleanup_options const* copts = NULL,
									  AcisOptions* ao = NULL);

/**
 * Deletes all unneeded handle objects belonging to a model.
 * <br><br>
 * <b>Role:</b> An object is considered unneeded if it is not being held by any other
 * object (see technical article on assembly-modeling use counting).
 * This routine cleans up the handles associated with a single model object.
 * An <tt>asm_cleanup_options</tt> object can be passed in to specify which types of
 * handle are cleaned up.  The default behavior is to clean all handle types.
 * <br><br>
 * <b>Errors:</b> None.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param model
 * model to be cleaned
 * @param copts
 * cleanup options object, which determines which types of handle will be cleaned.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_model_cleanup_handles(asm_model* model,
									  asm_cleanup_options const* copts = NULL,
									  AcisOptions* ao = NULL);

/**
 * Deletes all unneeded handle objects belonging to a models in a list.
 * <br><br>
 * <b>Role:</b> An object is considered unneeded if it is not being held by any other
 * object (see technical article on assembly-modeling use counting).
 * This routine cleans up the handles associated with each model object in the list.
 * An <tt>asm_cleanup_options</tt> object can be passed in to specify which types of handle
 * are cleaned up.  The default behavior is to clean all handle types.
 * <br><br>
 * <b>Errors:</b> None.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param model_list
 * list of models to be cleaned.
 * @param copts
 * cleanup options object, which determines which types of handle will be cleaned.
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_model_cleanup_handles(asm_model_list& model_list,
									  asm_cleanup_options const* copts = NULL,
									  AcisOptions* ao = NULL);

/**
 * Deletes all unneeded model objects.
 * <br><br>
 * <b>Role:</b> An object is considered unneeded if it is not being held by any other
 * object (see technical article on assembly-modeling use counting).
 * This routine (recursively) asks the global model manager to search for and delete any
 * <tt>asm_model</tt> objects that are not being held.
 * This procedure is performed recursively until only model objects that are being held
 * remain.  
 * Note that this routine does <b>not</b> attempt to clean up handle objects.
 * <br><br>
 * <b>Errors:</b> None.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_cleanup_models(AcisOptions* ao = NULL);

/**
 * Deletes a model object, if it is unneeded.
 * <br><br>
 * <b>Role:</b> An object is considered unneeded if it is not being held by any other
 * object (see technical article on assembly-modeling use counting).
 * This routine tests a single model object to see if it is being held.  If it is not
 * being held, the model object is deleted.
 * Note that this routine does <b>not</b> attempt to clean up handle objects.
 * <br><br>
 * <b>Errors:</b> None.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param model
 * model to be cleaned (is set to NULL if the model is deleted).
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_model_cleanup_model(asm_model*& model,
									AcisOptions* ao = NULL);

/**
 * Deletes a list of model objects, if they are unneeded.
 * <br><br>
 * <b>Role:</b> An object is considered unneeded if it is not being held by any other
 * object (see technical article on assembly-modeling use counting).
 * This routine tests each model object to see if it is being held.  If it is not being
 * held, the model object is deleted.
 * Note that this routine does <b>not</b> attempt to clean up handle objects.
 * <br><br>
 * <b>Errors:</b> None.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param model_list
 * list of models to be cleaned (models that are deleted are removed from the list).
 * @param ao
 * ACIS options.
 **/
DECL_ASM outcome asmi_model_cleanup_model(asm_model_list& model_list,
									AcisOptions* ao = NULL);


 /** @} */
#endif // ASM_CLEANUP_API_HXX

