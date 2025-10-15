/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ASM_API_HXX
#define ASM_API_HXX

#include "dcl_asm.h"
#include "asm_event_type.hxx"
#include "api.hxx"
#include "asm_model.hxx"

class ENTITY_LIST;
class ASM_ASSEMBLY;
class ASM_MODEL_REF;
class HISTORY_STREAM;
class asm_model_list;
class asm_model_entity_mgr;
class entity_mgr_factory;
class entity_handle;
class entity_handle_list;
class component_handle;
class component_handle_list;
class asm_model_info;
class asm_cleanup_options;
class asm_restore_options;
class ATTRIB_COMPONENT_PROP_OWNER;
class SPAbox;
class SPAboxing_options;
class ATTRIB;
class asm_save_options_internal;
class FileInterface;
class asm_restore_file;
/**
 * \defgroup ACISASSEMBLIES Assembly Modeling
 *
 */
/**
 * @file asm_api.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISASSEMBLIES
 *
 * @{
 */

// Note that "ASM_LEAF" only includes leaf sub_components
/**
 * Specifier for which types of nodes should be walked in a request on a tree.
 * @param ASM_IMMEDIATE
 * Immediate children of the specified node only. Does not include the specified node.
 * @param ASM_SUB
 * All children of the specified node. Does not include the specified node.
 * @param ASM_ALL
 * All children of the specified node, plus the specified node.
 * @param ASM_LEAF
 * All children of the specified node which do not themselves have children. 
 * Includes the specified node if the specified node has no children 
 * (in this case, the specified node is the only leaf).
 */
enum asm_get_request{ ASM_IMMEDIATE, ASM_SUB, ASM_ALL, ASM_LEAF};

/**
 * 	Provides a data structure for passing optional arguments to assembly modeling "get" commands.
 */
class DECL_ASM asm_get_options : public ACIS_OBJECT
{
public:
	asm_get_request request;
/**
 * Default constructor. The default request is ASM_ALL.
 */
	asm_get_options();

/**
 * Constructor for choosing a particular request.
 * <br><br>
 * @param the_request
 * request value to be used.
 */
	asm_get_options(asm_get_request the_request);

	void set_request(asm_get_request the_request);
	asm_get_request get_request();
};

class asm_save_file_factory;
/**
 * options object for asmi_save_model_list().
 */
class DECL_ASM asm_save_options : public ACIS_OBJECT
{
	logical history;          // IN: default FALSE
	asm_save_file_factory* save_factory;
	logical use_sat_for_part_models;
	logical text_mode;

public:
/**
 * Default constructor - asm_save_file_factory pointer set to NULL by default.
 */
	asm_save_options();
/**
 * Copy constructor.
 * <br><br>
 * @param other
 * asm_save_options to copy(if other is NULL, default implementation is used).
 */
	asm_save_options(asm_save_options* other);
/**
 * Constructor - specifies the history value. asm_save_file_factory pointer set to NULL by default.
 * <br><br>
 * @param with_history
 * value history to hold.
 */
	asm_save_options(logical with_history);
/**
 * Returns asm_save_file_factory pointer.
 */
	asm_save_file_factory* get_save_factory() const;
/**
 * Sets asm_save_file_factory pointer.
 * <br><br>
 * @param the_factory
 * pointer to asm_save_file_factory to hold.
 */	
	void set_save_factory(asm_save_file_factory* the_factory);
/**
 * Returns history.
 */
	logical get_with_history() const;
/**
 * Sets history.
 * <br><br>
 * @param set_to
 * value history to hold.
 */
    void set_with_history(logical set_to);

	logical get_default_asat_save_parts_as_sat() { return use_sat_for_part_models; }

	void set_default_asat_save_parts_as_sat(logical new_value) { use_sat_for_part_models = new_value; }

	logical get_default_asat_save_text_mode() { return text_mode; }

	void set_default_asat_save_text_mode(logical new_value) { text_mode = new_value; }
};
/**
 * options object for asmi_restore_model_list().
 */
class DECL_ASM asm_restore_options : public ACIS_OBJECT
{
	logical history;
	entity_mgr_factory* factory;
	asm_restore_file* restore_file;
public:
/**
 * Default constructor -  entity_mgr_factory and asm_restore_file pointer set to NULL by default.
 */
    asm_restore_options();
/**
 * Returns history. 
 */
	logical get_with_history() const;
/**
 * Sets history.
 * <br><br>
 * @param set_to
 * value history to hold.
 */
    void set_with_history(logical set_to);
/**
 * Returns entity_mgr_factory pointer.
 */
	entity_mgr_factory* get_entity_mgr_factory() const;
/**
 * Sets entity_mgr_factory pointer.
 * <br><br>
 * @param new_factory
 * pointer to entity_mgr_factory to hold.
 */
    void set_entity_mgr_factory(entity_mgr_factory* new_factory);
/**
 * Gets the user asm_restore_file pointer associated with the outer-most file
 * used to communicate with customer code during atomic restore.
 */
	asm_restore_file* get_restore_file() const;
/**
 * Sets asm_restore_file pointer.
 * <br><br>
 * @param the_file
 * pointer to asm_restore_file to hold.
 */	
  void set_restore_file(asm_restore_file* the_file);
};

///////////////////////////////
// API_ASM and API_MODEL macros
///////////////////////////////
/**
 *.
 */
#define ASM_VERS_BEGIN(opts) int status = 0; EXCEPTION_BEGIN ALGORITHMIC_VERSION_BLOCK(opts ? &opts->get_version() : NULL); EXCEPTION_TRY
/**
 *.
 */
#define ASM_END EXCEPTION_CATCH_FALSE status = resignal_no; EXCEPTION_END_NO_RESIGNAL outcome result(status);
/**
 * @nodoc
 **/
class asm_info {
	asm_model const* model;
	logical ended;
	asm_info();
public:
	asm_info( asm_model const* m) : model(m), ended(FALSE) { model->begin(); }
	~asm_info() { if(!ended) { outcome o(0); model->end( o, ASM_NO_CHANGE);}}
	void end( outcome o, asm_event_type e) { ended = TRUE; model->end(o,e,NULL); }
	void end( outcome o, asm_event_type e, asm_event_info* ei) { ended = TRUE; model->end(o,e,ei); }
};
/**
 *.
 */
#define API_MODEL_BEGIN(_model_) asm_info this_asm_info( _model_); API_BEGIN asm_model_entity_mgr* mgr = (_model_)->mgr();
/**
 *.
 */
#define API_MODEL_END(_event_type_) API_END this_asm_info.end( result, _event_type_);
/**
 *.
 */
#define API_MODEL_END2(_event_type_, _event_info_) API_END this_asm_info.end( result, _event_type_, _event_info_);
/**
 * Make model active
 */
#define MODEL_BEGIN(_model_) asm_info this_asm_info( _model_); outcome result; EXCEPTION_BEGIN asm_model_entity_mgr* mgr = (_model_)->mgr(); EXCEPTION_TRY 
/**
 *.
 */
#define MODEL_END(_event_type_) EXCEPTION_CATCH_FALSE result = outcome(resignal_no); EXCEPTION_END_NO_RESIGNAL this_asm_info.end( result, _event_type_ );
/**
 *.
 */
#define MODEL_END2(_event_type_, _event_info_) EXCEPTION_CATCH_FALSE result = outcome(resignal_no); EXCEPTION_END_NO_RESIGNAL this_asm_info.end( result, _event_type_, _event_info_ );


//////////////////////
// Interfaces to create a model object
//////////////////////

/**
 * Creates a new <tt>asm_model</tt> object that wraps an <tt>asm_model_entity_mgr</tt>.
 * <br><br>
 * <b>Errors:</b> NULL input manager.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param mgr
 * entity manager that will be bound to the model.
 * @param create_assembly
 * if TRUE, an assembly entity will be automatically added to the new model.
 * @param new_model
 * pointer to the newly created model.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_model_create( asm_model_entity_mgr* mgr,
											logical create_assembly,
											asm_model*& new_model,
											AcisOptions* ao = NULL );

/**
 * Creates a new <tt>asm_model</tt> object that wraps an <tt>asm_model_entity_mgr</tt> provided by the
 * globally registered <tt>entity_mgr_factory</tt>.
 * <br><br>
 * <b>Role:</b> Note that if <tt>model_info.model_has_asm</tt> is TRUE, a new assembly object will be 
 * automatically created and added to the model.
 * A customer-specific factory can be globally registered by using <tt>asmi_set_entity_mgr_factory</tt>.
 * <br><br>
 * <b>Errors:</b> NULL input manager.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param model_info
 * state information for the new model; used to create the entity manager. 
 * If <tt>model_info.model_has_asm</tt> is TRUE, an assembly object will be added to the model.
 * @param new_model
 * pointer to the newly created model.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_model_create( asm_model_info& model_info,
									asm_model*& new_model,
									AcisOptions* ao = NULL );

// INTERNAL USE ONLY
/**
 * @nodoc
 **/
DECL_ASM outcome asmi_model_create_for_translation(ENTITY_LIST & ents, 
										 asm_model_info& model_info,
										 asm_model*& new_model,
										 AcisOptions* ao = NULL);

/**
 * Creates a new <tt>asm_model</tt> object that wraps a new <tt>default_entity_mgr</tt>.
 * <br><br>
 * <b>Role:</b> This function is intended for use by customers who have their own assembly modeling
 * system and wish to export one of their assemblies into ASAT format. It creates a 
 * <tt>default_entity_mgr</tt> object which is bound to the history stream associated with the first 
 * entity in the ents list, or the default stream if there are no entities in the list. Or, if the first 
 * entity does not point to a history stream, it creates a new <tt>asm_model</tt> object which wraps 
 * the entity manager, and adds the entities from the ents list to the model.
 * <br><br>
 * The default entity manager is created with settings such that it owns neither its entities
 * nor its associated history stream; this is so the customer's entities and/or stream will not be 
 * deleted during cleanup after the ASAT file has been written.
 * Note that if <tt>model_info.model_has_asm</tt> is TRUE, a new assembly object will be automatically 
 * created and added to the model
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param ents
 * list of top-level entities to be added to the model. Should not include an <tt>ASM_ASSEMBLY</tt> entity.
 * @param model_info
 * state information for the new model used to create the entity manager. 
 * If <tt>model_info.model_has_asm</tt> is TRUE, an assembly object will be added to the model.
 * @param new_model
 * pointer to the newly created model.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_model_create_for_export(ENTITY_LIST & ents, 
										 asm_model_info& model_info,
										 asm_model*& new_model,
										 AcisOptions* ao = NULL);

//////////////////////
// Interfaces to manage the assembly object owned by an asm_model.
// NOTE: entity_handle for assembly object is intentionally not returned, as it should not be needed 
// by non-direct interface client code.
//////////////////////

/**
 * Creates an assembly entity within a model, which changes the model from a part model into an 
 * assembly model.
 * <br><br>
 * <b>Errors:</b> Model already has assembly or has no entity manager.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param model
 * model to which the assembly entity will be added.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_model_create_assembly(asm_model* model,
                                            AcisOptions* ao = NULL);

/**
 * Returns the model that owns an assembly.
 * <br><br>
 * <b>Effect:</b> Read only.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param assembly
 * assembly whose owning model is returned.
 * @param model
 * model that owns the assembly.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome api_asm_assembly_get_owning_model(
	const ASM_ASSEMBLY*	assembly,
	asm_model*&			model,
	AcisOptions*		ao = NULL);

/**
 * Loses the assembly entity within a model, which changes the model from an assembly model into a part model.
 * <br><br>
 * <b>Errors:</b> Model has no entity manager.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param model
 * model from which assembly entity will be removed.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_model_del_assembly(asm_model* model,
                                          AcisOptions* ao = NULL);


/**
 * Queries a model to determine if it contains a live assembly.
 * <br><br>
 * <b>Errors:</b> No entity manager.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param model
 * model which is queried to determine if it owns an assembly entity.
 * @param answer
 * result of the query.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_model_has_assembly(asm_model* model, 
								   logical & answer,
								   AcisOptions* ao = NULL);

/**
 * Queries a model to determine if it contains a live assembly.
 * <br><br>
 * <b>Errors:</b> No entity manager.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * @param model
 * model which is queried to determine if it owns an assembly entity.
 **/
DECL_ASM logical has_assembly(asm_model* model);

//////////////////////
// Interface to query a model for its entity manager.
//////////////////////

/**
 * Returns a pointer to the entity manager owned by a model. 
 * <br><br>
 * <b>Role:</b> The intent of this ASM routine is to allow customer code to determine
 * which customer part is bound to a model.
 * <br><br>
 * <b>Errors:</b> NULL model pointer.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param model
 * model to be queried.
 * @param mgr
 * pointer to the model's entity manager.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_model_get_entity_mgr(asm_model const* model, 
									 asm_model_entity_mgr* & mgr, 
									 AcisOptions* ao = NULL);

//////////////////////
// Interface to query for the entities managed by the model's entity manager
//////////////////////
/**
 * Returns a list of entity handles for the top-level part-modeling entities contained within
 * a model.
 * <br><br>
 * <b>Role:</b> Does <i>not</i> include the model's assembly entity, if any, in the list.
 * This query does not take into account suppressed and over-ridden models.
 * <br><br>
 * <b>Errors:</b> NULL model pointer.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param model
 * model to be queried.
 * @param ents
 * list of entity handles of the requested entities.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_model_get_entities(asm_model const* model,
								entity_handle_list & ents,
								AcisOptions* ao = NULL );

//////////////////////
// Interfaces to query model for part information, including units and tolerances
//////////////////////

/**
 * Returns a <tt>model_info</tt> object containing information about a model. 
 * <br><br>
 * <b>Role:</b> This information includes units, tolerances, and a customer ID number.
 * <br><br>
 * <b>Errors:</b> NULL model pointer.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param model
 * model to be queried.
 * @param model_info
 * object in which returned model information will be stored by the routine.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_model_get_info(asm_model const* model, 
									 asm_model_info & model_info, 
									 AcisOptions* ao = NULL);

//DECL_ASM outcome asm_get_model_state(asm_model const* model, asm_model_state & state, AcisOptions* ao = NULL); // PART, ASSEMBLY, EXTERNAL, etc.
//////////////////////
// Interface to query for components of a model. This is the core query function for walking an assembly model.
//////////////////////
/**
 * Returns a list of the components of a model by walking the model's assembly tree. 
 * <br><br>
 * <b>Role:</b> This is a preferred query function for client code to use to unfold the
 * assembly tree, as it correctly takes into account suppressed and over-ridden models.
 * An options object allows client code to specify which components are returned. 
 * The default behavior is ASM_ALL, which includes all components, including the root
 * component (corresponding to the model object).
 * <br><br>
 * <b>Errors:</b> NULL model pointer.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param model
 * model whose components are to be found.
 * @param components
 * returned list of <tt>entity_handles</tt> for components of the model.
 * @param get_opts
 * options object that specifies which components are to be returned.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_model_get_components(asm_model*	model,
									 component_handle_list&		components,
									 asm_get_options* get_opts = NULL,
									 AcisOptions*				ao = NULL); 

/**
 * Returns a list of all models upon which the input root model depends, that is, all models
 * that appear below the root model in the assembly tree.
 * <br><br>
 * <b>Role:</b> The intent of this routine is to allow the user to find all the models
 * that are involved in the definition of an assembly, so that, for example, a global
 * assembly state can be noted (by noting each of the models' state).
 * The <tt>asm_get_request</tt> enumeration determines which models are returned. The
 * value ASM_ALL specifies that all sub-models of the root_model, including itself, be
 * returned; ASM_SUB specifies all proper sub-models of the root_model; ASM_IMMEDIATE
 * specifies only the proper sub-models referenced by the root's assembly model; ASM_LEAF
 * specifies all sub-models that are not assembly models (if <tt>root_model</tt> is not
 * an assembly, the root model is returned).
 * <br><br>
 * <b>Errors:</b> NULL model pointer.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param root_model
 * root model to be queried.
 * @param which_models
 * specifies which models are to be returned.
 * @param models
 * returned list of models upon which the root depends.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_model_get_sub_models( asm_model* root_model,
										asm_get_request which_models,
										asm_model_list& models, 
										AcisOptions* ao = NULL );

//////////////////////
// Interface to assist during save-with-history of models which share a history stream
//////////////////////
/**
 * Returns a list of all models whose entity managers are bound to the same history
 * stream as this model's entity manager.
 * <br><br>
 * <b>Role:</b> Note that the input model <i>is</i> included in the list.
 * <br><br>
 * <b>Errors:</b> NULL model pointer.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param this_model
 * model to be tested for history sharing.
 * @param sharing_models
 * list of models whose entity managers share <tt>this_model</tt>'s entity manager's history stream,
 * including <tt>this_model</tt>.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_get_models_which_share_history(asm_model* this_model, 
													 asm_model_list & sharing_models, 
													 AcisOptions* ao = NULL);
//////////////////////
// Interfaces for performing save and restore
//////////////////////
/**
 * Saves a list of <tt>asm_model</tt> objects to a single file.
 * <br><br>
 * <b>Role:</b> Only top-level (primary) models need to be specified; the routine
 * automatically saves any sub-models upon which the primary models depend.
 * <br><br>
 * <b>Errors:</b> NULL model pointer.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param file_ptr
 * open file descriptor.
 * @param text_mode
 * TRUE indicates an ASCII file format; FALSE indicates a binary file.
 * @param primary_models
 * list of root models to save.
 * @param sopts
 * save options
 * @param ao
 * ACIS options such as versioning and journaling.
 **/

DECL_ASM outcome asmi_save_model_list(
	FILE*				file_ptr,
	logical				text_mode,
	asm_model_list &	primary_models,
	asm_save_options*	sopts=NULL,
	AcisOptions*		ao=NULL);

/**
 * Saves a list of <tt>asm_model</tt> objects to a single file using a <tt>FileInterface</tt> object.
 * <br><br>
 * <b>Role:</b> Only top-level (primary) models need to be specified; the routine
 * automatically saves any sub-models upon which the primary models depend.
 * <br><br>
 * <b>Errors:</b> NULL model pointer.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param file_if
 * FileInterface object.
 * @param primary_models
 * list of root models to save.
 * @param sopts
 * save options.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_save_model_list(
	FileInterface*		 file_if,
	asm_model_list&      primary_models,
	asm_save_options* sopts=NULL,
	AcisOptions* ao=NULL);

/**
 * Saves an assembly model and its sub-models to multiple files in atomic mode.
 * <br><br>
 * <b>Role:</b> Only the top-level model needs to be specified; the routine  
 * automatically saves any sub-models upon which the input model depends.
 * <br><br>
 * <b><i>Note:</i></b> This routine should only be used to embed atomic ASAT (or ASAB) segments 
 * in customer files; it should <i>never</i> be used to write files with .ASAT or .ASAB extensions.
 * <br><br>
 * <b>Errors:</b> NULL model pointer.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param model
 * top-level model to save.
 * @param sopts
 * save options; these include <tt>save-with-history</tt> and <tt>save-as-text</tt>.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_save_model_atomic(
	asm_model*     model,
	asm_save_options* sopts=NULL,
	AcisOptions* ao=NULL);

/**
 * Restores one or more assembly models from a file.
 * <br><br>
 * <b>Role:</b> The list of primary models that was specified in <tt>asmi_save_model_list</tt>
 * is returned.
 * Each restored model is bound to a customer entity manager, created by the
 * customer-created <tt>entity_mgr_factory</tt> that has been globally registered using 
 * <tt>asmi_set_entity_mgr_factory</tt>. This globally registered factory can be overridden 
 * for this particular call to <tt>asmi_restore_model_list</tt> by using an <tt>asm_restore_options object</tt>.
 * <br><br>
 * <b>Errors:</b> NULL model pointer.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param file_ptr
 * open file descriptor.
 * @param text_mode
 * TRUE if the file is text, FALSE if it is binary.
 * @param primary_models
 * list of root models that were saved.
 * @param ropts
 * restore options.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_restore_model_list(
	FILE*		 file_ptr,
	logical 	 text_mode,
	asm_model_list &     primary_models,
	asm_restore_options* ropts=NULL,
	AcisOptions * ao=NULL);

/**
 * Restores a list of <tt>asm_model</tt> objects using a <tt>FileInterface</tt> object.
 * <br><br>
 * <b>Role:</b> The list of primary models that was specified in <tt>asmi_save_model_list</tt>
 * is returned.
 * Each restored model is bound to a customer entity manager, created by the
 * customer-created <tt>entity_mgr_factory</tt> that has been globally registered using 
 * <tt>asmi_set_entity_mgr_factory</tt>. This globally registered factory can be overridden 
 * for this particular call to <tt>asmi_restore_model_list</tt> by using an <tt>asm_restore_options object</tt>.
 * <br><br>
 * <b>Errors:</b> NULL model pointer.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param file_if
 * <tt>FileInterface</tt> object.
 * @param primary_models
 * list of root models that were saved.
 * @param ropts
 * restore options.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_restore_model_list(
	FileInterface*		 file_if,
	asm_model_list &     primary_models,
	asm_restore_options* ropts=NULL,
	AcisOptions * ao=NULL);

/**
 * Resets the globally registered entity manager factory.
 * <br><br>
 * <b>Role:</b> This function replaces the current globally registered entity manager factory
 * (used to create entity managers during ASAT restore) with the new factory being passed in.
 * Note that this will call the <tt>lose</tt> method (signalling a destruction request) on the 
 * entity manager factory which is currently registered, and that the <tt>lose</tt> method
 * will be called on the entity manager factory which is registered when ACIS is terminated.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param factory
 * The <tt>entity_mgr_factory</tt> object to be registered.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_set_entity_mgr_factory( 
	entity_mgr_factory*& factory,
	AcisOptions * ao=NULL);


/**
 * Gets a pointer to the globally registered entity manager factory.
 * <br><br>
 * <b>Role:</b> This function returns a pointer to the current globally registered entity manager factory
 * (used to create entity managers during ASAT restore).
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param factory
 * The currently registered <tt>entity_mgr_factory</tt> object.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_get_entity_mgr_factory( 
	entity_mgr_factory*& factory,
	AcisOptions * ao=NULL);

//////////////////////
// Interfaces to manage the model references associated with an assembly model
//////////////////////

/**
 * Creates a new model reference object and adds it to an assembly model.
 * <br><br>
 * <b>Role:</b> The supplied transform determines the location and orientation of the
 * model reference within the assembly, in the assembly model's coordinate system and units.
 * This transform may only involve rotation and translation; reflection, shear, and scaling
 * transforms are not valid.
 * <br><br>
 * <b>Errors:</b> <tt>owning_model</tt> not an assembly, invalid transform for assembly modeling.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param assembly_model
 * assembly model to which the model reference will be added.
 * @param transform
 * location and orientation of the model reference.
 * @param referenced_model
 * the model to which the model reference refers.
 * @param model_ref
 * <tt>entity_handle</tt> for the newly created <tt>ASM_MODEL_REF</tt> object.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_model_add_model_ref(asm_model* assembly_model,
									   SPAtransf & transform,
									   asm_model * referenced_model,
									   entity_handle *& model_ref,
									   AcisOptions* ao = NULL);

/**
* Creates a new model reference object and adds it to an assembly model.
* <br><br>
* <b>Role:</b> The supplied transform determines the location and orientation of the
* model reference within the assembly, in the assembly model's coordinate system and units.
* This transform may only involve rotation and translation; reflection, shear, and scaling
* transforms are not valid.
* <br><br>
* <b>Errors:</b> <tt>owning_model</tt> not an assembly, invalid transform for assembly modeling.
* <br><br>
* <b>Effect:</b> Changes model.
* <br><br>
* <b>Journal:</b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
* <br><br>
* @param assembly_model
* assembly model to which the model reference will be added.
* @param transform
* location and orientation of the model reference.
* @param referenced_model
* the model to which the model reference refers.
* @param model_ref
* <tt>entity_handle</tt> for the newly created <tt>ASM_MODEL_REF</tt> object.
* @param pushback
* Option to control order of insertion of model references in the list. If pushback is equal to 
* true then elements will be inserted from the tail of the list otherwise from the head of list.
* @param ao
* ACIS options such as versioning and journaling.
**/
DECL_ASM outcome asmi_model_add_model_ref(asm_model* assembly_model,
                                       SPAtransf & transform,
                                       asm_model * referenced_model,
                                       entity_handle *& model_ref,
                                       logical const pushback,
                                       AcisOptions* ao = NULL);


/**
 * Removes an existing model reference from its owning assembly and loses it.
 * <br><br>
 * <b>Errors:</b> <tt>owning_model</tt> not an assembly; non-existent <tt>model_ref</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param model_ref
 * entity_handle of the <tt>ASM_MODEL_REF</tt> object that is to be removed.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_model_remove_model_ref(entity_handle * model_ref,	// handle of model_ref to remove
									   AcisOptions* ao = NULL);

/**
 * Gets all immediate model references belonging to an assembly model.
 * <br><br>
 * <b>Role:</b> Does <i>not</i> search deeper into the tree for sub-references. 
 * Ignores suppression and overrides. This
 * routine will generally be called when translating the assembly into another format.
 * <br><br>
 * <b>Errors:</b> <tt>owning_model</tt> does not have an assembly.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param assembly_model
 * assembly model that will be queried for its model references.
 * @param model_ref_handles
 * list of entity handles corresponding to the model references.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_model_get_model_refs(asm_model const* assembly_model,
									     entity_handle_list & model_ref_handles, // immediate model_refs only
										 AcisOptions* ao = NULL);

//////////////////////
// Interfaces manipulating and querying model references
//////////////////////

/**
 * Gets the transform associated with a model reference.
 * <br><br>
 * <b>Errors:</b> Non-existent model reference.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param model_ref
 * entity handle for the model reference to be queried.
 * @param transf
 * the model reference's transform.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_model_ref_get_transform(entity_handle* model_ref,
                                      SPAtransf & transf,
									  AcisOptions* ao = NULL );

/**
 * Resets the transform associated with a model reference.
 * <br><br>
 * <b>Role:</b> The supplied transform determines the location and orientation of the
 * model reference within the assembly, in the assembly model's coordinate system and units.
 * This transform may only involve rotation and translation; reflection, shear, and scaling
 * transforms are not valid.
 * <br><br>
 * <b>Errors:</b> Non-existent model reference, invalid transform for assembly modeling.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param model_ref
 * entity handle for the model reference whose transform will be reset.
 * @param transf
 * the new transform.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_model_ref_set_transform(entity_handle* model_ref,
                                      SPAtransf & transf,
									  AcisOptions* ao = NULL );

/**
 * Applies a transform to a model reference or to all of the model references in an assembly.
 * <br><br>
 * <b>Role:</b> The composition of the supplied transform with the existing transform
 * determines the location and orientation of the model reference within the assembly, in
 * the assembly model's coordinate system and units. This transform may only involve
 * rotation and translation; reflection, shear, and scaling transforms are not valid.
 * <br><br>
 * <b>Errors:</b> Non-existent entity, invalid transform for assembly modeling.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param model_ref_or_assembly
 * entity handle for the model reference or the assembly containing the model references to which 
 * the transform will be applied.
 * @param transf
 * the transform to be applied.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_model_ref_apply_transform(entity_handle* model_ref_or_assembly,
                                      SPAtransf & transf,
									  AcisOptions* ao = NULL );

/**
 * Returns the model to which the specified model reference refers.
 * <br><br>
 * <b>Errors:</b> Non-existent model reference.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param model_ref
 * entity handle for the model reference whose model is being requested.
 * @param model
 * model referred to by <tt>model_ref</tt>.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_model_ref_get_model(entity_handle* model_ref,
										asm_model*& model, 
										AcisOptions* ao = NULL );

/**
 * Returns the model containing the assembly that owns a particular model reference.
 * <br><br>
 * <b>Errors:</b> Non-existent model reference.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param model_ref
 * entity handle for the model reference whose owning model is being requested.
 * @param model
 * model owning the assembly containing <tt>model_ref</tt>.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_model_ref_get_owning_model(entity_handle* model_ref,
										asm_model*& model, 
										AcisOptions* ao = NULL );

//////////////////////
// Interfaces for manipulating and querying components
// Note that the component view of the assembly is the preferred interface for querying the assembly tree,
// as it most closely corresponds to the physical objects which make up the assembly.
//////////////////////

/**
 * Returns a list of the sub-components of a component of a model, by walking the
 * underlying model's assembly tree. 
 * <br><br>
 * <b>Role:</b> This is a preferred query function for client code to use to unfold the
 * assembly tree, as it correctly takes into account suppressed and over-ridden models.
 * An options object allows client code to specify which components are returned.
 * <br><br>
 * <b><i>Note:</i></b> The caller must specify which sub-components (for example, ASM_IMMEDIATE) 
 * should be returned.
 * <br><br>
 * <b>Errors:</b> Non-existent component.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param component
 * component whose sub-components are to be found.
 * @param which_components
 * specifies which components are to be returned.
 * @param include_suppressed
 * if TRUE, suppression will be ignored when walking the model's assembly tree.
 * @param sub_components
 * sub-components of the input component.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_component_get_sub_components(
	const component_handle*	component, 
	asm_get_request which_components,
	logical include_suppressed,
	component_handle_list&		sub_components,
    AcisOptions*				ao = NULL);
/**
 * @nodoc
 **/
DECL_ASM outcome asmi_component_get_sub_components( // deprecated in favor of new signature
	const component_handle*	component, 
	asm_get_request which_components,
	component_handle_list&		sub_components,
    AcisOptions*				ao = NULL);

/**
 * Returns the total transform that positions and orients a component within the root
 * assembly.
 * <br><br>
 * <b>Role:</b> The <tt>units_rescale_factor</tt> argument determines if the transform
 * includes the rescaling necessary to account for differences in units between the component model 
 * and the root assembly.
 * <br><br>
 * <b>Errors:</b> Non-existent component.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param component
 * component being queried.
 * @param component_transf
 * transform required to move component from the origin to its location within the root assembly.
 * @param include_units_rescaling
 * if TRUE, returned transform includes the rescale factor implied by different units.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_component_get_transform(component_handle const* component,
						SPAtransf & component_transf,
						logical include_units_rescaling,
						AcisOptions* ao = NULL);

/**
 * Returns the relative transform between two components of an assembly.
 * <br><br>
 * <b>Role:</b> An object at <tt>origin_comp</tt> that is transformed by the returned transform
 * will appear at the same position and with the same orientation as <tt>target_comp</tt> in
 * the coordinate system of <tt>origin_comp</tt>.
 * <br><br>
 * The <tt>units_rescale_factor</tt> argument determines if the transform includes the
 * rescaling necessary to account for a difference in units between the component model
 * and the root assembly.
 * <br><br>
 * <b>Errors:</b> Non-existent component(s).
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param origin_comp
 * component that determines origination point of transformation.
 * @param target_comp
 * component that determines target point of transformation.
 * @param relative_transf
 * transform required to move from <tt>origin_comp</tt> to <tt>target_comp</tt>.
 * @param include_units_rescaling
 * if TRUE, the returned transform includes the rescale factor implied by different units.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_component_get_relative_transform(component_handle const* origin_comp, 
								component_handle const* target_comp, 
								SPAtransf & relative_transf,
								logical include_units_rescaling,
								AcisOptions* ao = NULL);

/**
 * Returns a list of entity handles for the part-modeling entities contained within a
 * component's model.
 * <br><br>
 * <b>Role:</b> Does <i>not</i> include the model's assembly entity, if any, in the list.
 * This is the preferred query function for client code to use in order to obtain the part
 * data associated with a particular component within the assembly tree, 
 * as it correctly takes into account suppressed and over-ridden models.
 * <br><br>
 * <b>Errors:</b> Non-existent component.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param component
 * component whose part modeling entities are to be found.
 * @param ents
 * list of entity handles for the requested entities.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_component_get_entities(component_handle const* component,
								entity_handle_list & ents,
								AcisOptions* ao = NULL );

/**
 * Returns a list of component-entity handles corresponding to the top-level entities
 * of a component.
 * <br><br>
 * <b>Role:</b> This is a convenience routine which repackages the result of 
 * @href asmi_component_get_entities as a list of <tt>component_entity_handles</tt>.
 * <br><br>
 * <b>Errors:</b> NULL component_handle pointer.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param comp
 * component to be queried.
 * @param comp_ent_list
 * the list of <tt>component_entity_handles</tt>.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM
outcome asmi_component_get_component_entities(component_handle *       comp,
											  component_entity_handle_list& comp_ent_list,
										      AcisOptions*                  ao = NULL);

/**
 * Returns the box of a component of an assembly in the context of the assembly's root model.
 * <br><br>
 * <b>Errors:</b> NULL model pointer.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param component
 * component whose box is to be calculated.
 * @param box
 * box of the component in the space of the assembly's root model.
 * @param boxing_opts
 * boxing options.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_component_get_box(component_handle const* component,
										SPAbox & box,
										SPAboxing_options* boxing_opts = NULL,
										AcisOptions* ao = NULL);

/**
 * Returns the path of model references, through the folded assembly tree, that
 * corresponds to a particular component.
 * <br><br>
 * <b>Role:</b> The path is returned as a root model at which the path begins, plus a
 * list of model-reference entity handles.
 * The intent of this routine is to allow client code to generate a unique identifier
 * for the component, based on the combination of the model and the list of model references.
 * No two components in memory will have identical paths, so customers may use identifiers
 * for the elements of the path (assigned by the customer) to uniquely identify the
 * corresponding components.
 * <br><br>
 * Note that the model-reference list is tip-to-tail, with the highest model reference in
 * the tree (the one owned by the root model) appearing first.
 * Also, note that the root model is only needed as an identifier for zero-length paths
 * (with no model references). Such paths correspond to the root component of a model.
 * <br><br>
 * <b>Errors:</b> Non-existent component.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param component
 * component to be queried.
 * @param root_model
 * the root model of the component. 
 * @param model_refs
 * list of model-reference entity handles that specify the steps in the component's path through 
 * the folded assembly tree.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_component_get_path(component_handle const* component,
										 asm_model* & root_model,
										 entity_handle_list & model_refs, 
										 AcisOptions* ao = NULL );

/**
 * Returns the root model of a specified component.
 * <br><br>
 * <b>Role:</b> This is the model corresponding to the root assembly
 * in which the component occurs.
 * <br><br>
 * <b>Errors:</b> Non-existent component.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param component
 * component to be queried.
 * @param root_model
 * model corresponding to the component's root assembly.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_component_get_root_model(component_handle* component,
										asm_model*& root_model, 
										AcisOptions* ao = NULL );
/**
 * Returns the parent component of a specified component.
 * <br><br>
 * <b>Errors:</b> Non-existent component.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param component
 * component to be queried.
 * @param parent
 * parent component. 
 * NULL if component is a root component, that is, if the component corresponds to a zero-length
 * model-ref path.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_component_get_parent(component_handle* component,
										component_handle*& parent, 
										AcisOptions* ao = NULL );

/**
 * Returns a pointer to the raw end model associated with a component.
 * <br><br>
 * <b>Role:</b> This routine should be used with caution, 
 * since the (sub-)components of the queried component's raw end model might not correspond
 * to the queried component's actual (sub-)components, due to properties applied to 
 * the component or its
 * sub-components at higher levels of the assembly tree. For example, if the
 * front left wheel of a car were suppressed, the components of the actual front axle assembly
 * (with only one wheel) would be different from the components of the raw front axle
 * assembly (with two wheels).
 * <br><br>
 * The intent of this routine is to aid customers translating into an assembly format
 * that does not support the application of properties (such as suppression) to components
 * from higher levels in the assembly tree. Such customers need to be able to query
 * whether two components of the assembly are defined by exactly the same model data in
 * order to share model data between instances. If the <tt>is_modified</tt> flag for a
 * component's model is TRUE, then the translated component cannot share model data with
 * any other translated component. If the <tt>is_modified</tt> flag will not be used by the 
 * caller, then the alternate signature of this routine (which omits the flag) should be used
 * for performance reasons.
 * <br><br>
 * <b>Errors:</b> Non-existent component.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param component
 * component to be queried.
 * @param model
 * pointer to the component's raw end model.
 * @param is_modified
 * if TRUE, then the component's model data is modified by properties applied higher in the assembly tree.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_component_get_unmodified_model(component_handle const* component,
										asm_model*& model, 
										logical & is_modified,
										AcisOptions* ao = NULL );


/**
 * Returns a pointer to the raw end model associated with a component.
 * <br><br>
 * <b>Role:</b> This routine returns a pointer to the raw end model of the component, that is,
 * the model referenced by the last model reference in the component's model reference path.
 * The (sub-)components of the queried component's raw end model might not correspond
 * to the queried component's actual (sub-)components due to properties applied to 
 * the component or its sub-components at higher levels of the assembly tree. For example, if the
 * front left wheel of a car were suppressed, the components of the actual front axle assembly
 * (with only one wheel) would be different from the components of the raw front axle
 * assembly (with two wheels).
 * <br><br>
 * This signature of the routine is much faster than the signature which returns an 
 * <tt>is_modified</tt> flag, because it does not have to scan the assembly tree for properties 
 * which might modify the component.
 * <br><br>
 * <b>Errors:</b> Non-existent component.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param component
 * component to be queried.
 * @param model
 * pointer to the component's raw end model.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_component_get_unmodified_model(component_handle const* component,
										asm_model*& model, 
										AcisOptions* ao = NULL );

/**
 * Queries whether a component has been modified by properties applied higher in the assembly tree.
 * <br><br>
 * <b>Role:</b> Refer to the documentation of asmi_component_get_unmodified_model() for details.
 * <br><br>
 * <b>Errors:</b> Non-existent component.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param component
 * component to be queried.
 * @param is_modified
 * if TRUE, then the component's model data is modified by properties applied higher in the assembly tree.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_component_is_model_modified(component_handle const* component,
						logical & is_modified, 
						AcisOptions* ao = NULL );

//////////////////////
// Interfaces for obtaining and querying handles
//////////////////////

/**
 * @nodoc
 * This signature is deprecated in favor of the other one that takes the check_entity_owner argument.
 **/
DECL_ASM outcome api_asm_model_get_entity_handle(ENTITY* ent, 
											asm_model* owning_model,
											entity_handle* & handle,
											AcisOptions* ao = NULL );






/**
 * Returns the entity handle associated with an <tt>ENTITY</tt>.
 * <br><br>
 * <b>Role:</b> If the <tt>ENTITY</tt> does not have an associated entity handle, one
 * will be created.
 * Note that the <tt>ENTITY</tt> must be managed by the model's entity manager, either
 * directly or through its owner.
 * <br><br>
 * <b>Errors:</b> Entity does not belong to model.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param ent
 * pointer to the input entity.
 * @param owning_model
 * model which owns the entity, and which manages the entity handle.
 * @param handle
 * pointer to the entity's handle.
 * @param check_entity_owner
 * Check if the entity belongs to the owning model before creating a handle. This parameter affects the performance of this
 * API when the model has a large number of top level entities. Users of this API should always turn this check ON in Debug Builds. To 
 * obtain better performance in Release builds, they can turn it off they require.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome api_asm_model_get_entity_handle(ENTITY* ent,
											asm_model* owning_model,
											entity_handle* & handle,
						                    logical check_entity_owner,
											AcisOptions* ao = NULL );

/**
 * Returns the <tt>ENTITY</tt> associated with an entity handle.
 * <br><br>
 * <b>Errors:</b> Non-existent handle.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param handle
 * pointer to the entity handle.
 * @param ent
 * pointer to the handle's entity.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome api_asm_entity_handle_get_ptr(entity_handle* handle,
										 ENTITY* & ent,
										 AcisOptions* ao = NULL );

/**
 * Returns a component handle corresponding to a list of model references that describe
 * a path through the folded assembly tree.
 * <br><br>
 * <b>Role:</b> The path list must be tip-to-tail, with the highest (nearest the root of
 * the assembly tree) model reference appearing first.
 * Note that it is not necessary to specify the root model of the path, since it is the
 * owner of the first model reference in the list. If the path is zero-length
 * (corresponding to a single model in the assembly), then the alternate signature
 * of this routine that takes an <tt>asm_model</tt> pointer should be used.
 * <br><br>
 * <b><i>CAUTION:</i></b> This routine is intended to be used only by experts working with the folded
 * assembly tree (for example, in a translation application). Most customers should instead
 * use the component interface (for example, <tt>asmi_model_get_components</tt>) to walk the
 * assembly tree.
 * <br><br>
 * <b>Errors:</b> None.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param model_refs
 * list of model-reference entity handles.
 * @param comp_handle
 * pointer to the resulting component's handle.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_model_get_component_handle(entity_handle_list & model_refs,
									component_handle* & comp_handle,
										AcisOptions* ao = NULL );
/**
 * Returns a component handle corresponding to a list of model references that describe
 * a path through the folded assembly tree.
 * <br><br>
 * <b>Role:</b> This is a convenience routine, for use when it is unknown whether the
 * model reference list is empty.
 * The path list must be tip-to-tail, with the highest (nearest the root of the assembly
 * tree) model reference appearing first.
 * Note that it is not necessary to specify the root model of the path, since it is the
 * owner of the first model reference
 * in the list. If the path is zero-length (corresponding to a single model in the
 * assembly), then the alternate signature
 * of this routine that takes an <tt>asm_model</tt> pointer should instead be used.
 * <br><br>
 * <b><i>CAUTION:</i></b> This routine is intended to be used only by experts working with the folded
 * assembly tree (for example, in a translation application). Most customers should instead
 * use the component interface (for example, <tt>asmi_model_get_components</tt>) to walk the
 * assembly tree.
 * <br><br>
 * <b>Errors:</b> None.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param root_model
 * start of path.
 * @param model_refs
 * list of model-reference entity handles.
 * @param comp_handle
 * pointer to the resulting component's handle.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_model_get_component_handle(asm_model* root_model,
											entity_handle_list & model_refs,
									component_handle* & comp_handle,
										AcisOptions* ao = NULL );

/**
 * Returns a component handle corresponding to a list (path) of components.
 * <br><br>
 * <b>Role:</b> Because a component is just a path through the folded (model reference)
 * assembly tree, a tip-to-tail list of components (path) can be concatenated to form a
 * new component (path).
 * The path list must be tip-to-tail, with the highest (nearest the root of the assembly
 * tree) model reference appearing first.
 * <br><br>
 * <b>Errors:</b> Non-existent component.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param components
 * ordered list of component handles.
 * @param comp_handle
 * pointer to the resulting component's handle.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_model_get_component_handle(component_handle_list & components,
									component_handle* & comp_handle,
										AcisOptions* ao = NULL );

/**
 * Returns a component handle corresponding to a single model in the assembly tree. 
 * <br><br>
 * <b>Role:</b> This corresponds to a zero-length path in the folded assembly tree.
 * <br><br>
 * <b><i>CAUTION:</i></b> This routine is intended to be used only by experts working with the folded
 * assembly tree (for example, in a translation
 * application). Most customers should instead use the component interface (for example,
 * <tt>asmi_model_get_components</tt>) to walk the assembly tree.
 * <br><br>
 * <b>Errors:</b> None.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param comp_model
 * list of model-reference entity handles.
 * @param comp_handle
 * pointer to the resulting component's handle.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_model_get_component_handle(asm_model* comp_model,
									component_handle* & comp_handle,
										AcisOptions* ao = NULL );

/**
 * Returns a component handle corresponding to a single model reference in the assembly tree. 
 * <br><br>
 * <b>Role:</b> This corresponds to a length-one path in the folded assembly tree.
 * <br><br>
 * <b><i>CAUTION:</i></b> This routine is intended to be used only by experts working with the folded
 * assembly tree (for example, in a translation application). Most customers should instead
 * use the component interface (for example, <tt>asmi_model_get_components</tt>) to walk the
 * assembly tree.
 * <br><br>
 * <b>Errors:</b> None.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param model_ref
 * a single model-reference entity handle.
 * @param comp_handle
 * pointer to the resulting component's handle.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_model_get_component_handle(entity_handle* model_ref,
									component_handle* & comp_handle,
										AcisOptions* ao = NULL );

/**
 * Returns the component-entity handle corresponding to the specified component handle
 * and entity handle.
 * <br><br>
 * <b>Errors:</b> None.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param comp
 * the input component.
 * @param ent
 * the input entity.
 * @param comp_ent
 * the output component-entity.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/

DECL_ASM outcome asmi_model_get_component_entity_handle(component_handle* comp,
											entity_handle* ent,
											component_entity_handle* & comp_ent,
											AcisOptions* ao = NULL );

/**
 * Returns the entity and component handles corresponding to a component entity.
 * <br><br>
 * <b>Errors:</b> None.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param comp_ent
 * the input component-entity.
 * @param comp
 * the output component.
 * @param ent
 * the output entity.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_component_entity_handle_decompose(component_entity_handle* comp_ent,
											component_handle* & comp,
											entity_handle* & ent,
											AcisOptions* ao = NULL );


/**
 * Checks the consistency of an <tt>asm_model</tt>.
 * <br><br>
 * <b>Errors:</b> None.
 * <br><br>
 * <b>Effect:</b> None.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param given_asm_model
 * assembly model to be tested.
 * @param err
 * error_info object returned.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_check_model(
                 const asm_model*   given_asm_model,    // asm_model to check
                 error_info*&       return_err,
                 AcisOptions*       ao = NULL);


/**
 * Prunes a history stream at any delta states which contains <tt>ASM_ASSEMBLY</tt> bulletins. 
 * <br><br>
 * <b>Role:</b> This routine searches all delta states of the input history stream for bulletins
 * involving ASM_ASSEMBLY entities, and prunes the history stream at those delta states using 
 * @href api_prune_history.
 * <br><br>
 * It is provided for use by customers in overriding <tt>asm_model_entity_mgr::clear_vf</tt>, 
 * since a requirement of that method is that no such delta states survive the <tt>clear_vf</tt> 
 * operation. Note that this routine will clear the history stream (resulting
 * in deletion of all entities in the stream) if <tt>ASM_ASSEMBLY</tt> bulletins have been added 
 * to the stream since the last "note state" operation. For this reason, we strongly recommend 
 * that @href api_note_state be called on the input stream before calling this routine.
 * <br><br>
 * <b>Errors:</b> None.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param hs
 * history stream to be pruned.
 * @param cleared_history
 * returns TRUE if the history stream was cleared by this routine.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome api_asm_prune_assembly_history(
	HISTORY_STREAM* hs,
	logical & cleared_history,
	AcisOptions* ao = NULL);

/**
 * Returns ENTITY_LIST containing pointers to live entities associated with entity_handle_list.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param ent_handles
 * entity handle list from which you want to get live entity pointers.
 * @param live_ent_list
 * list which will contain exactly the live entities associated with entity handles on the input list.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/

DECL_ASM outcome asmi_entity_handle_list_get_live_entities( entity_handle_list const& ent_handles, 
															ENTITY_LIST& live_ent_list,
															AcisOptions* ao=NULL);

/**
* @nodoc
**/
class asmi_model_save_entities_options_impl;
/**
* @nodoc
**/
class DELTA_STATE_LIST;
/**
* Specifies options for using asmi_model_save_entities.
**/
class DECL_ASM asmi_model_save_entities_options: public ACIS_OBJECT
{
	asmi_model_save_entities_options_impl* _impl;
public:
	/**
	* Type for enumerating possible save modes.
	**/
	enum save_mode{ STANDARD_ACIS_BINARY, STANDARD_ACIS_TEXT};
	/**
	* Makes an options object.
	**/
	asmi_model_save_entities_options();
	/**
	* Destroys an options object.
	**/
	~asmi_model_save_entities_options();
	/**
	* Selects whether to save as SAT or SAB. Default is SAT.
	**/
	void set_save_mode( asmi_model_save_entities_options::save_mode new_mode);
	/**
	* Returns whether options object selects save as SAT or SAB.
	**/
	asmi_model_save_entities_options::save_mode get_save_mode();
	/**
	* Selects whether to allow saving entities from models with an assembly.
    * <b><i>Note:</i></b> This option deletes information about the assembly (such as, 
	* how the entities are put together in relation to each other).
	*
	* Default is false.
	**/
	void set_allow_save_of_model_with_assembly(logical new_val);
	/**
	* Indicates whether this options object will allow saving entities from models with an assembly.
	**/
	logical get_allow_save_of_model_with_assembly();
};
/**
 * Saves an assembly model to a SAT or SAB file. 
 * <br><br>
 * <b>Role:</b> This function saves the entities from an assembly model. It does not store any 
 * assembly related information: for example, how many times each ENTITY is used in the model, 
 * or which transforms are associated with each model reference.
 * It is most useful with part models; use with assembly models is not recommended.
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param model
 * model with entities to be stored.
 * @param file_to_save_to
 * file handle where you want the entities to be saved.
 * @param opts
 * options object for controlling whether you are allowed to save a model with an assembly 
 * (which could destroy information), and whether text or binary mode is used.
 * @param ao
 * ACIS options such as versioning and journaling.
 **/
DECL_ASM outcome asmi_model_save_entities(	asm_model* model,
									FILE* file_to_save_to,
									asmi_model_save_entities_options *opts=NULL,
									AcisOptions *ao=NULL);

/**
* Sets the default directory in which ACIS performs atomic save-restore. 
* <br><br>
* <b>Journal:</b> Not Available
* <br><br>
* @param default_atomic_asm_path
* A wide character string containing the folder path.
**/
DECL_ASM void set_default_asm_atomic_path( SPAWCHAR_T const* default_atomic_asm_path );

/**
* Returns the current default directory as a wide character string in which ACIS is performing atomic save-restore. 
* If it's NULL, ACIS performs the save-restore in executable's directory.
* <br><br>
* <b>Journal:</b> Not Available
* <br><br>
**/
DECL_ASM SPAWCHAR_T const * get_default_asm_atomic_path();

 /** @} */
#endif // ASM_API_HXX
