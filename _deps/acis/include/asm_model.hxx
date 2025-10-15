/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ASM_MODEL_HXX
#define ASM_MODEL_HXX

#include "dcl_asm.h"
#include "entity.hxx"
#include "lists.hxx"
#include "logical.h"
#include "vlists.hxx"
#include "box.hxx"
#include "entity_handle_list.hxx"
#include "comp_handle_list.hxx"
#include "asm_model_info.hxx"
#include "ent_handle_holder.hxx"
#include "comp_ent_handle_list.hxx"
#include "asm_model_list.hxx"
#include "asm_event_type.hxx"
#include "asm_model_kern.hxx"

/**
* @file asm_model.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISASSEMBLIES
 *
 * @{
 */

class asm_model_entity_mgr;
class ASM_MODEL_REF;
class ASM_ASSEMBLY;
class ENTITY;
class ENTITY_LIST;
class ENTITY_ARRAY;
class asm_model_list;
class entity_handle;
class component_handle;
class asm_model_mgr;
class asm_restore_seq_no_mgr;
class outcome;
class SPAboxing_options;
class asm_save_options_internal;
class model_entity_handle_seq_table;
class ATTRIB_COMPONENT_PROP_OWNER;
class asm_restore_file;
class asm_save_file;
class asm_model_internal;
class asm_event_info;

/**
 * Class object to encapsulate requests for optional behaviors when cleaning up handle objects.
 * <br>
 * <b>Role:</b> <tt>asm_cleanup_options</tt> is used to select which type of handle objects should be
 * considered when calling an asmi cleanup routine. The default behavior is to attempt to clean up all
 * handle types.
 * <br><br>
 * @see asmi_cleanup, asmi_model_cleanup, asmi_model_cleanup_tree, asmi_cleanup_handles, asmi_model_cleanup_handles
 */

class DECL_ASM asm_cleanup_options : public ACIS_OBJECT
{
    logical ent;
    logical comp;
    logical comp_ent;

public:
    /**
     * Default constructor; cleans up all handles.
     */
    asm_cleanup_options();

    /**
     * Constructor that specifies which handle types to clean up.
     * <br><br>
     * @param ent_handles
     * entity handles will be cleaned if TRUE.
     * @param comp_handles
     * component handles will be cleaned if TRUE.
     * @param comp_ent_handles
     * component entity handles will be cleaned if TRUE.
     */
    asm_cleanup_options(logical ent_handles, logical comp_handles, logical comp_ent_handles);

    /**
     * Indicates whether entity handles will be cleaned.
     */
    logical get_clean_entity_handles() const;
    void set_clean_entity_handles(logical value);
    /**
     * Indicates whether component handles will be cleaned.
     */
    logical get_clean_component_handles() const;
    void set_clean_component_handles(logical value);
    /**
     * Indicates whether component entity handles will be cleaned.
     */
    logical get_clean_component_entity_handles() const;
    void set_clean_component_entity_handles(logical value);
};

// customers are NOT allowed to derive from asm_model
/**
 * Class object to represent an assembly or part model within assembly modeling.
 * <br>
 * <b>Role:</b> Represents a part or assembly model within assembly modeling.
 * <br><br>
 * The <tt>asm_model</tt> class is the fundamental organizational class object within
 * ACIS Assembly Modeling.
 * Each <tt>asm_model</tt> object represents either a part model or (sub-)assembly model.
 * Each <tt>asm_model</tt> object is associated with an <tt>asm_model_entity_mgr</tt>
 * object, which in turn wraps the application code object associated with the model.
 * For more information, refer to the Assembly Modeling technical articles.
 */
class DECL_ASM asm_model : public asm_model_kern
{
    // mgr to which this model is bound
    asm_model_entity_mgr* mgr_ptr; // asm_model_entity_mgr to which this model is bound
    asm_model_internal* data_ptr;
    // interface for managing atomic saves involving this model
    asm_save_file* save_file_ptr;
    asm_restore_file* restore_file_ptr;


    // cache info
    SPAbox entity_mgr_box;
    mutable logical box_valid;

    // friends
    friend class asm_model_holder;	// these are the only classes allowed to modify the use count
    friend class asm_model_list_eng;
    friend class asm_model_internal;
    friend void unhold_mra(ENTITY* ent);
    friend void sg_asmi_add_assembly(asm_model* owning_model);

    // use counting
    //int use_cnt;	// number of uses of this model (includes active, backed up, and lost model_refs as well as other instantiations of asm_model_holder)
                    // moved to asm_model_kern
    //void add_count(); // moved to asm_model_kern
    virtual void rem_count();

    // backpointer to owning model manager
    asm_model_mgr* owning_model_mgr;		// may not always be a singleton in the future
    friend class asm_seq_number_mgr;
    // entity handle management
    entity_handle_list ent_handles;
    // component handle management
    component_handle_list comp_handles;
    // component entity handle management
    component_entity_handle_list comp_ent_handles;

    // holding lists to ensure that assembly(s) and model-refs owned by this model's manager
    // always are bound to an entity_handle (allowing them access to the model)
    entity_handle_list assembly_handles;
    entity_handle_list mref_handles;

    // internal utility functions
    void hold_assembly_handle(entity_handle* asm_handle);
    virtual void unhold_assembly_handle(entity_handle* asm_handle);

    virtual void unhold_mref_handle(entity_handle* mref_handle);

    // notify (and de-notify) this model of sub-model
    virtual void unlink_sub_model(asm_model* sub_model);

    // used during restore of model's entities to re-hold assemblies and mrefs
    void hold_mrefs_and_assemblies();

    friend void read_additional_ids(asm_model*);

    asm_model_info* restore_model_info_ptr;

    // arrays to hold unfixed handles
    entity_handle_list unfixed_ent_handles;
    component_handle_list unfixed_comp_handles;
    component_entity_handle_list unfixed_comp_ent_handles;

    component_handle* lookup_comp(entity_handle_list const& mrefs) const;
    component_entity_handle* lookup_comp_ent(component_handle* comp, entity_handle* ent) const;

public:
    // INTERNAL PUBLIC METHODS - INTERNAL USE ONLY
    /**
     * @nodoc
     */
    virtual void   link_sub_model(asm_model* sub_model);
    /**
     * @nodoc
     */
    void hold_mref_handle(entity_handle* mref_handle);
    // assembly model info - used during restore
/**
 * @nodoc
 */
    void set_restore_model_info(asm_model_info const& model_info);
    /**
     * @nodoc
     */
    asm_model_info const& restore_model_info() const;
    /**
     * @nodoc
     */
    model_entity_handle_seq_table* seq_table;
    /**
     * @nodoc
     */
    ENTITY_ARRAY* restore_sat_sequence_numbers;

private:
    asm_model(asm_model_entity_mgr* ent_mgr);  // disallow creation on the stack
    ~asm_model(); // destructor - make it private so that only deregister_model_ref can destruct a asm_model

public:
    /**
     * @nodoc
     */
    static asm_model* create(asm_model_entity_mgr* ent_mgr);

private:
    // unbinds mgr_ptr
    friend class asm_model_entity_mgr;
    /**
     * @nodoc
     */
    logical unbind_mgr(asm_model_entity_mgr* mgr);

    // boxing
    friend DECL_ASM void get_component_box(component_handle const*, SPAbox&, SPAboxing_options*);
    /**
     * @nodoc
     */
    void refresh_box_cache(SPAboxing_options* boxing_opts); // for internal use only
    /**
     * @nodoc
     */
    void set_box_cache_state(logical valid) const;// for internal use only
    /**
     * @nodoc
     */
    logical box_cache_valid(SPAboxing_options* boxing_opts) const;// for internal use only
    /**
     * @nodoc
     */
    SPAbox calculate_box(SPAboxing_options* boxing_opts) const; // for internal use only
    /**
     * @nodoc
     */
    SPAbox get_box(SPAboxing_options* boxing_opts) const; // return the box, in internal model coordinates; will NOT update the cache, if it is stale
    /**
     * @nodoc
     */
    SPAbox get_box(SPAboxing_options* boxing_opts); // return the box, in internal model coordinates; will update the cache, if it is stale

    // model tree data
/**
 * @nodoc
 */
    asm_model_list owning_models;
    /**
     * @nodoc
     */
    asm_model_list sub_models;
    friend class ASM_ASSEMBLY;		// only class allowed to modify tree structure

    /**
     * @nodoc
     */
    void add_owning_model(asm_model* model, int& add_count);
    /**
     * @nodoc
     */
    void remove_owning_model(asm_model* model, int& add_count);
    /**
     * @nodoc
     */
    void add_sub_model(asm_model* model, int& add_count);
    /**
     * @nodoc
     */
    void remove_sub_model(asm_model* model, int& add_count);
    /**
     * @nodoc
     */
    void remove_from_tree();
    friend class asm_model_mgr;
    /**
     * @nodoc
     */
    void reset_model_tree_data();

public:
    // INTERNAL PUBLIC METHODS - INTERNAL USE ONLY
    /**
     * @nodoc
     */
    void write_sub_models(asm_save_options_internal* sopts);
    /**
     * @nodoc
     */
    void read_sub_models();
    /**
     * @nodoc
     */
    void link_models_post_restore();
    /**
     * @nodoc
     */
    void unlink_sub_models_post_restore();

public:
    /**
     * Returns the <tt>asm_model_entity_mgr</tt> associated with this model.
     * <br><br>
     * <b>Role:</b> Returns the entity manager object to which this model is bound.
     */
    asm_model_entity_mgr* mgr() const; // mgr to which this model is bound

    /**
     * @nodoc
     */
    logical is_usable() const;

    /**
     * @nodoc
     */
    logical is_complete() const;

    /**
     * Returns an entity handle for this model's assembly, if any.
     * <br><br>
     * <b>Role:</b> Returns an entity_handle bound to the assembly object owned by this model's entity
     * manager. Returns NULL if the model has no assembly, that is, it is a part model.
     */
    entity_handle* get_assembly_handle();

    /**
     * Returns a structure containing information about the model.
     * <br><br>
     * <b>Role:</b>
     * This routine is used to query the model for information about itself which is to be saved to
     * an ASAT file.
     */
    asm_model_info get_model_info() const;

    /**
     * Returns the units associated with a model.
     * <br><br>
     * <b>Role:</b>
     * This routine is used to query the model for the number of mm corresponding to a distance of 1.0.
     * For example, for each 1 unit (cm) of length, the routine will return a value of 10.0.
     */
    double get_units() const;

    /**
     * Returns the positional tolerance associated with a model.
     * <br><br>
     * <b>Role:</b>
     * This routine is used to query the model for the value of SPAresabs used for entities within the model.
     */
    double get_resabs() const;

    /**
     * Returns the angular tolerance associated with a model.
     * <br><br>
     * <b>Role:</b>
     * This routine is used to query the model for the value of SPAresnor used for entities within the model.
     * <br><br>
     * <b><i>Note:</i></b> You should <i>never</i> change SPAresnor away from its initial value of 1.0e-10.
     */
    double get_resnor() const;

    /**
     * @nodoc
     */
    void add_entities(ENTITY_LIST& ents);

    // INTERNAL PUBLIC METHODS AND DATA - INTERNAL USE ONLY
    /**
     * @nodoc
     */
    int sequence_number_within_save_file;

    /**
     * @nodoc
     */
    int first_entity_handle_id_within_save_file;

    /**
     * @nodoc
     */
    asm_save_file* save_file() const; // used during atomic restore

    /**
     * @nodoc
     */
    void set_save_file(asm_save_file* sfile); // used during atomic restore

    /**
     * @nodoc
     */
    asm_restore_file* restore_file() const; // used during atomic restore

    /**
     * @nodoc
     */
    void set_restore_file(asm_restore_file* rfile); // used during atomic restore

    /**
     * @nodoc
     */
    void make_safe_after_failed_restore();

    /**
     * @nodoc
     */
    asm_model_mgr* get_owning_manager() const;
    /**
     * @nodoc
     */
    void release_owning_manager();	// notify model that owning manager is going away

    /**
     * @nodoc
     **/
    void get_property_owner(component_handle* comp, ATTRIB_COMPONENT_PROP_OWNER*& owner_attrib, logical create_if_absent);

    /**
     * @nodoc
     */
    void get_lookup_entity_handles(entity_handle_list& handles) const; // clears then adds "no-tombstone" list to handles
    /**
     * @nodoc
     */
    int lookup_entity_handles_count() const; // returns iteration_count of entity_handle list

    // method to get list of handles to save
/**
 * @nodoc
 */
    void get_lookup_component_handles(component_handle_list& handles) const; // clears then adds "no-tombstone" list to handles

    // method to get list of handles to save
/**
 * @nodoc
 */
    void get_lookup_component_entity_handles(component_entity_handle_list& handles) const; // clears then adds "no-tombstone" list to handles

    // method to create and register an entity handle being restored

/**
 * @nodoc
 */
    logical entity_handles_fixed();

    /**
     * @nodoc
     */
    void restore_entity_handle(asm_restore_file* current_file,
        int handle_id_in_this_file,
        int handle_id_in_model_file,
        int entity_sequence_id_in_model_file);

    /**
     * @nodoc
     */
    void restore_component_handle(entity_handle_list& model_refs);

    // method to create and register a component entity handle being restored
/**
 * @nodoc
 */
    void restore_component_entity_handle(entity_handle* ent, component_handle* comp);

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        // fix up pointers after restore
    /**
     * @nodoc
     */
    logical fixup(asm_restore_seq_no_mgr* mgr);
    /**
     * @nodoc
     */
    logical fixup_entity_handles(ENTITY_ARRAY& array);

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // Data member to stash primary (top-level) entity IDs associated with this model during restore
    /**
     * @nodoc
     */
    mutable ENTITY_LIST restore_ents;

    // Function to register primary entities with entity manager during restore
    /**
     * @nodoc
     */
    void register_loaded_entities() const;
    /**
     * @nodoc
     */
    void done_restoring();

    // cleanup function
    /**
     * @nodoc
     */
    void clear_restore_info() const;

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    /**
     * @nodoc
     **/
    void get_all_owning_models(logical include_self, asm_model_list& models);
    /**
     * @nodoc
     */
    void get_immediate_owning_models(asm_model_list& models) const;
    /**
     * @nodoc
     */
    void get_immediate_sub_models(asm_model_list& models, logical include_lost = FALSE) const;
    /**
     * @nodoc
     */
    void get_top_level_entities(ENTITY_LIST& entities, logical include_assemblies) const;	// used by rendering
    /**
     * @nodoc
     */
    void get_top_level_entities(entity_handle_list& entities, logical include_assemblies) const;

    //++++++++++++++++++++ methods for entity_handle management ++++++++++++++++++++++++++++++++++++++++++
        // routines for clients of asm_model to acquire and manage handles
    /**
     * @nodoc
     * This signature is deprecated in favor of the other one that takes the check_entity_owner argument.
     */
    entity_handle* get_entity_handle(ENTITY const* entity) const;	// return existing handle or make new handle (add to ent_handles)
    // note that this is declared a const method even though it may add new handles


/**
 * Returns the entity handle associated with an ENTITY.
 * <br><br>
 * <b>Role:</b> Returns the entity handle associated with an ENTITY contained in the model. If no such
 * handle exists, one will be created.
 * <br><br>
 * @param entity
 * The entity for which a handle is being requested
 * @param check_entity_owner
 * Optional logical argument that controls if the api checks to see if the input entity is owned by the model.
 */
    entity_handle* get_entity_handle(ENTITY const* entity, logical check_entity_owner) const;	// return existing handle or make new handle (add to ent_handles)




    /**
     * Returns all entity handles managed by the model.
     * <br><br>
     * <b>Role:</b> Overwrites the input list of handles with a list of all entity handles managed by the
     * model.
     * <br><br>
     * @param handles
     * the output list of handles.
     */
    void get_entity_handles(entity_handle_list& handles) const;		// get all instantiated handles (by adding? copy)
    /**
     * @nodoc
     */
    void remove_entity_handle(entity_handle* handle);
    //++++++++++++++++++++ end methods for entity_handle management ++++++++++++++++++++++++++++++++++++++

    //++++++++++++++++++++ methods for component_handle management ++++++++++++++++++++++++++++++++++++++
    /**
     * @nodoc
     */
    component_handle* get_component_handle(entity_handle_list& model_refs);
    /**
     * @nodoc
     */
    component_handle* get_component_handle(entity_handle* model_ref_path[], int n_model_refs);  // use ASM_MODEL_REF*[] instead?
    /**
     * @nodoc
     */
    void remove_component_handle(component_handle* handle);
    //++++++++++++++++++++ end methods for component_handle management ++++++++++++++++++++++++++++++++++++++

    //++++++++++++++++++++ methods for component_entity_handle management ++++++++++++++++++++++++++++++++++++++
    /**
     * @nodoc
     */
    component_entity_handle* get_component_entity_handle(entity_handle* ent_hdl, component_handle* comp_hdl);
    /**
     * @nodoc
     */
    void remove_component_entity_handle(component_entity_handle* handle);
    //++++++++++++++++++++ end methods for component_entity_handle management ++++++++++++++++++++++++++++++++++++++

    //++++++++++++++++++++ methods for cleanup ++++++++++++++++++++++++++++++++++++++
    /**
     * @nodoc
     */
    logical cleanup_handles(asm_cleanup_options const* co);	// clean up handles specified by options object
    /**
     * @nodoc
     */
    logical cleanup_model(asm_cleanup_options const* co);	// clean up entities and handles and if use count is zero, destruct
    //++++++++++++++++++++ end methods for cleanup ++++++++++++++++++++++++++++++++++++++

    //++++++++++++++++++++ methods for change notification ++++++++++++++++++++++++++++++++++++++
    /**
     * Notifies the model that its contents have been changed externally.
     * <br><br>
     * <b>Role:</b> This method must be called by the customer application if it changes the contents of a
     * model outside of the context of assembly modeling, for example, by performing a Boolean operation
     * on a body in the application document associated with the model.
     * It will result in <tt>sub_model_changed_vf</tt> being called on the entity managers of all parent
     * models of this model.
     * <br><br>
     * @param ev_type
     * the type of change that was made.
     */
    void contents_changed(asm_event_type ev_type) const;
private:
    void sub_model_contents_changed(asm_event_type ev_type, asm_model const* changed_model);
    //++++++++++++++++++++ end methods for change notification ++++++++++++++++++++++++++++++++++++++

public:
    //++++++++++++++++++++ methods to switch contexts ++++++++++++++++++++++++++++++++++
    /**
     * @nodoc
     */
    void begin() const;
    /**
     * @nodoc
     */
    void end(outcome& result, asm_event_type ev_type) const;
    /**
     * @nodoc
     */
    void end(outcome& result, asm_event_type ev_type, asm_event_info*) const;
    //++++++++++++++++++++ end methods to switch contexts ++++++++++++++++++++++++++++++++++

    entity_handle_list* gme_get_assembly_handles();
};


/** @} */
#endif // ASM_MODEL_HXX

