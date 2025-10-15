/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ASM_SAVRES_ATOMIC_HXX
#define ASM_SAVRES_ATOMIC_HXX

#include "dcl_asm.h"
#include "asm_savres.hxx"
#include "asm_model_list.hxx"
#include "entity_handle_list.hxx"

class asm_model;
class entity_handle;
class FileInterface;
class asm_save_options;
class asm_save_options_internal;
class asm_save_file;

// three modes in which a model can be saved to ASAT
// ASM_MONOLITHIC - all sub-models contained in ASAT file
// ASM_ATOMIC_ASAT - not all sub-models contained in ASAT file.  Part models saved as ASAT
// ASM_ATOMIC_SAT - contents of a part model are saved to SAT
enum asat_save_mode {ASM_MONOLITHIC=0, ASM_ATOMIC_ASAT, ASM_ATOMIC_SAT};

class DECL_ASM asm_file_data : public ACIS_OBJECT
{
	SPAWCHAR_T*       m_file_name;
	asat_save_mode m_save_mode;
	logical        m_text_mode;
	int            m_cust_id;

	void*          m_file_ptr;
public:

	asm_file_data();
	asm_file_data(const asm_file_data & other);

	asm_file_data & operator=(const asm_file_data & other);
	~asm_file_data();

	asat_save_mode save_mode() const;
	void set_save_mode(asat_save_mode mode);

	logical text_mode() const;
	void set_text_mode(logical mode);

	SPAWCHAR_T const* file_name() const;
	void set_file_name(SPAWCHAR_T const* new_name);

	// used to indicate segment (or other location information) within a file
	int id() const;
	void set_id(int cust_id);

	// convenience pointer for use during run-time to tie asm_file_data to a
	// customer file object.  Managed by client code, but copied by operator= etc.
	void* file_ptr() const;
	void set_file_ptr(void* value);
};

class DECL_ASM asm_restore_file : public ACIS_OBJECT
{
	virtual logical needs_restore_vf() const;

	virtual asm_file_data const* get_file_data_vf() const=0;

	virtual asm_model* lookup_model_vf(int model_id_in_this_file,
							asm_file_data const* model_file_data,
							int model_id_in_model_file) const=0;

	virtual asm_restore_file* register_model_vf(asm_model* model,
							int model_id_in_this_file,
							asm_file_data const* model_file_data,
							int model_id_in_model_file)=0;

	virtual entity_handle* lookup_entity_handle_vf(int handle_id_in_this_file, 
								                asm_model const* owning_model,
		                                        int handle_id_in_model_file,
												int entity_sequence_id_in_model_file) const=0;

	virtual void register_entity_handle_vf(entity_handle* eh,
										int handle_id_in_this_file, 
	                                    int handle_id_in_model_file,
												int entity_sequence_id_in_model_file)=0;

	// called by atomic restore when:
	// A) The call to register the missing model's asm_model object returned this file,
	// but the missing model is not in this file (i.e. its entities
	// are not present)
	// B) The call to register the missing model's asm_model object returned this file,
	// but the call to open on this file failed (returned a NULL pointer)
	// The missing model's asm_model object  will be unbound from this file.
	// The customer may re-attempt to load the model (after restore_done() is called on this file)
	// by calling asmi_model_repair_failed_load(asm_model*, FileInterface*, asm_restore_options*);
	// In the mean time, all entity handles owned by the missing model will be bound
	// to NULL entity pointers.
	virtual void model_missing_from_this_file_vf(asm_model* missing_model);

	// ACIS is ready to read this file
	// should return NULL if fails
	// if NULL is returned, close_vf() will not be called on this file
	virtual FileInterface* open_vf()=0;

	// ACIS is done reading this file
	virtual void close_vf()=0;

	// allows customer to delete this object - ACIS promises not to use this object
	// after calling done_restoring
	virtual void done_restoring_vf();

public:
	// default is TRUE
	logical needs_restore() const;

	asm_file_data const* get_file_data() const;

	asm_model* lookup_model(int model_id_in_this_file,
							asm_file_data const* model_file_data,
							int model_id_in_model_file) const;

	asm_restore_file* register_model(asm_model* model,
							int model_id_in_this_file,
							asm_file_data const* model_file_data,
							int model_id_in_model_file);

	entity_handle* lookup_entity_handle(int handle_id_in_this_file, 
								                asm_model const* owning_model,
		                                        int handle_id_in_model_file,
												int entity_sequence_id_in_model_file) const;

	void register_entity_handle(entity_handle* eh,
										int handle_id_in_this_file, 
	                                    int handle_id_in_model_file,
												int entity_sequence_id_in_model_file);

	// called by atomic restore when:
	// A) The call to register the missing model's asm_model object returned this file,
	// but the missing model is not in this file (i.e. its entities
	// are not present)
	// B) The call to register the missing model's asm_model object returned this file,
	// but the call to open on this file failed (returned a NULL pointer)
	// The missing model's asm_model object  will be unbound from this file.
	// The customer may re-attempt to load the model (after restore_done() is called on this file)
	// by calling asmi_model_repair_failed_load(asm_model*, FileInterface*, asm_restore_options*);
	// In the mean time, all entity handles owned by the missing model will be bound
	// to NULL entity pointers.
	void model_missing_from_this_file(asm_model* missing_model);

	// ACIS is ready to read this file
	FileInterface* open();

	// ACIS is done reading this file
	void close();

	// allows customer to delete this object - ACIS promises not to use this object
	// after calling done_restoring unless it is explicitly given back by the customer
	// in a call to asm_restore_file::register_model() or asmi_restore_model_list(), i.e. not deleted by the customer.
	void done_restoring();

	virtual ~asm_restore_file();
};


class DECL_ASM asm_seq_number_mgr : public ACIS_OBJECT
{
	asm_save_options_internal* my_sopts;
	asm_save_file* my_file;
	asm_model_list my_primaries;

public:
	asm_seq_number_mgr(asm_save_options_internal* sopts, asm_save_file* this_file, asm_model_list & primary_models);

	// returns the file being saved
	asm_save_file* get_this_file() const;

	// adds all models for which a model segment will be saved in this file to the list
	// a model may have a model segment in more than one file
	void get_all_file_models(asm_model_list & models) const;

	// adds all models whose entities are will be saved in an entity segment in this file to the list
	// a model's entities may only be saved in one file
	void get_saved_file_models(asm_model_list & models) const;

	// returns information about model which can be saved by the customer as metadata
	// to allow customer to implement asm_restore_file:;lookup_model() during restore
	void get_model_save_info(asm_model* model,            // the model
	                         int & model_id_in_this_file, // the model's sequence number in this file
	                         asm_save_file*& model_file,  // the file in which the model's entities are saved
	                         int & model_id_in_model_file) const; // the model's sequence number in model_file

	// adds all entity handles which will be saved in this file to the list
	// owning models for the entity handles can be obtained by querying the entity handles.
	void get_all_file_entity_handles(entity_handle_list & ent_handles) const;

	// returns information about entity handle which can be saved by the customer as metadata
	// to allow customer to implement asm_restore_file::lookup_entity_handle() during restore
	// owning models for the entity handles can be obtained by querying the entity handles.
	void get_entity_handle_info(entity_handle* handle,                         // the handle
	                            int & handle_id_in_this_file,                  // the handle's sequence number in this file
							    asm_model*& owning_model,
	                            int & handle_id_in_model_file,                 // the handle's sequence number in the file in which its entity is saved
	                            int & entity_sequence_id_in_model_file) const; // the handle's entity's sequence number in the entity segment (ASAT) or sat file (SAT) in which it's saved
};

class DECL_ASM asm_save_file : public ACIS_OBJECT
{
protected:
// not implemented in R17
//	virtual logical needs_save_vf()=0;

	virtual asm_file_data const* get_file_data_vf() const=0;

	// for use during atomic save (before writing model)
	// used to indicate which other models (primary or sub) should be saved (i.e. have their entities saved)
	// in the same atomic file as this model.  These methods only need to be implemented by customer if:
	//	A) Customer is saving with history and some models share history streams - NO, default implementation handles this
	//	B) Customer wants to save more than one model per atomic file
	// CONSTRAINTS:
	//	1)  When a model is saved with history, all models with which it shares history MUST be saved (with history) to the same file.
	//  2)  For a particular save_info setting (i.e. for a particular call to asmi_model_save_atomic()), 
	//	    each model should appear (i.e. have entities saved in) one and only one asat file
	//	3)  For a particular save_info, all models in the same atomic file must give consistent answers to their
	//	    asm_model_io_info queries, i.e. they must agree on save_location, save_type, etc.
	//	4)  All models in the "sub" list must be submodels of a primary model (either this model or a model in the "other primary" list).
	//	5)  Sub-class should NOT clear either model list before adding models
	//
	// asm_save_options provided to indicate whether save is with history or not
	// primary models will always contain the model currenty being saved when passed in.
	virtual void get_additional_models_in_atomic_file_vf(asm_save_options const* sopts, 
	                                                     asm_model_list & primary_models, 
	                                                     asm_model_list & other_sub_models); 

	// for use during atomic save (before writing model)
	// returns a properly positioned FileInterface pointer to a open file where the ASAT segment containing this model
	// (i.e. the ASAT segment where this models entities are saved).  The sequence number data can be used by customer 
	// code to obtain the sequence number of all models and handles used by the ASAT segment (whether in the ASAT segment being
	// saved or other ASAT segments containing sub-models).  This allows customers to save metadata in customer segments of the 
	// file that binds (sequence number + model location) to a customer naming scheme, which in turn allows robust independent
	// editing of files.
	virtual FileInterface* open_vf(asm_seq_number_mgr const & sequence_numbers)=0;

	// give customer opportunity to write customer data after ACIS data
	// not recommended except for SAT (where it is necessary)
	// customers should instead use the asm_seq_number_mgr passed into open to write their metadata in a segment before ACIS data
	virtual void post_save_vf(asm_seq_number_mgr const & sequence_numbers);

	// close the file.  Should be lightweight and no-throw, since it may be called during stack unwinding.
	virtual void close_vf()=0;

	// allows customer to delete this object - ACIS promises not to use this object
	// after calling done_restoring
//	virtual void done_saving_vf();

public:
	// default is TRUE
	// virtual function override not supported in R17
	logical needs_save();

	// if NULL is returned, assume this is top-level file in a monolithic save and use parameters from sopts
	// NULL can only be returned from a single file in a save session.
	asm_file_data const* get_file_data() const;

	void get_additional_models_in_atomic_file(asm_save_options const* sopts, 
	                                                     asm_model_list & primary_models, 
	                                                     asm_model_list & other_sub_models) ;

	// NOTE: sequence_numbers will be destroyed immediately after the call to open(), so customer code should not hold a reference to it
	FileInterface* open(asm_seq_number_mgr const & sequence_numbers);

	virtual ~asm_save_file();

	void post_save(asm_seq_number_mgr const & sequence_numbers);

	void close();

//	void done_saving();
};

class DECL_ASM asm_save_file_factory : public ACIS_OBJECT
{
protected:
	virtual void prepare_for_save_vf(asm_model_list const & models, asm_save_options* sopts)=0;
	virtual asm_save_file* get_file_vf(asm_model const* model)=0;
	virtual void done_saving_vf()=0;
public:
	// Notifies customer code of all models which will be saved which allows customer
	// to make a global decision as to which models will be saved in which files.
	// There is a requirement that (the entities belonging to) one model can be saved to only one file.
	void prepare_for_save(asm_model_list const & models, asm_save_options* sopts);

	// returns a pointer to the save file in which a model's entities will be saved.
	// the results of this routine must be consistent with asm_save_file::get_additional_models_in_atomic_files()
	// in terms of which models are associated with a particular file
	asm_save_file* get_file(asm_model const* model);

	// allows customer to delete all files associated with this save session.
	void done_saving();
};

#endif // ASM_SAVRES_ATOMIC_HXX
