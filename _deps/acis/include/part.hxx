/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//----------------------------------------------------------------------
// purpose---
//    Define virtual PART class
//
//----------------------------------------------------------------------

#ifndef part_hxx
#define part_hxx
#include <stdio.h>
#include "dcl_part.h"
#include "logical.h"
#include "api.hxx"
#include "bulletin.hxx"
#include "entityid.hxx"
#include "histid.hxx"
#include "ent_cb.hxx"
/**
 * @file part.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup PARTAPI
 *
 * @{
 */


class ENTITY;
class ENTITY_LIST;
class entity_filter;
class outcome;
class FileInterface;
class ID_ATTRIB;
class PART;
class entity_callback_list;
class HISTORY_STREAM;
class acis_pm_entity_mgr;

// For internal use only
/**
 * @nodoc
 */

DECL_PART logical initialize_part_manager();

// For internal use only
/**
 * @nodoc
 */
DECL_PART logical terminate_part_manager();

//======================================================================
// The part_handle class is used to identify a PART.  We do not use the
// pointer to the PART directly, because the PART may get deleted when
// there are deleted ENTITIES that still point to it.  If we do a
// rollback of the deleted ENTITIES, we need to be able to check to see
// if the PART is still valid.  The part_handle is a combination of
// an integer ID which is unique for each PART, and the PARTS SPAposition
// in an array of all PARTS.  This dta is packed into a single 32 bit
// value, so that in the current implementation, there is a limit of
// 65536 PARTS which can exists at any given time.  If you create more
// than 65536 PARTS in a single session, the PART IDs will start to be
// reused, so that there is some slight chance that a deleted ENTITY
// could have a handle to a PART that is different than the PART is was
// originally in, but this is not worth worrying about.


// For internal use only
/**
 * @nodoc
 * The <tt>part_handle</tt> class is used to identify a <tt>PART</tt>.
 * <br>
 * <b>Role:</b> We do not use the pointer to the <tt>PART</tt> directly, because
 * the <tt>PART</tt> may get deleted when there are deleted <tt>ENTITIES</tt>
 * that still point to it.  If we do a rollback of the deleted <tt>ENTITIES</tt>,
 * we need to be able to check to see if the <tt>PART</tt> is still valid.
 */
class DECL_PART part_handle
{
private:
	unsigned short m_nId;
	unsigned short m_nIndex;

public:
	part_handle() : m_nId(0), m_nIndex(0) {}
	part_handle(int id, int index)
	{
		m_nId=(short)id;
		m_nIndex=(short)index;
	}
	~part_handle() {}

	unsigned int Id() const { return m_nId; }
	unsigned int Index() const { return m_nIndex; }

	PART* Part() const;

	int operator==(part_handle ph)
		{ return (m_nId == ph.m_nId && m_nIndex == ph.m_nIndex); }

};

// These are values that can be set in PartFlags
/**
 *.
 */
#define PART_DO_NOT_DELETE 0x00000001L

//======================================================================
/**
 * Provides methods and data for grouping entities together as parts.
 * <br><b>Role:</b> This class provides basic functions for working with parts.
 * @see entity_callback_list,ID_ATTRIB, active_part,HISTORY_STREAM, HISTORY_STREAM_LIST
 */
class DECL_PART PART  : public ACIS_OBJECT {

	void detect_op_leaks( void );

// For internal use only
/**
 * @nodoc
 */
  	friend DECL_PART logical initialize_part_manager();

// For internal use only
/**
 * @nodoc
 */
	friend DECL_PART logical terminate_part_manager();

  protected:
	// These will be used to calc picking buffer.
/**
 * Number of faces.
 */
	int		m_nNumFaces;			// number of faces.
/**
 * Number of edges.
 */
	int		m_nNumEdges;			// number of edges.
/**
 * Number of tolerant coedges.
 */
	int		m_nNumTCoEdges;			// number of TCoedges.
/**
 *  Number of vertices.
 */
	int		m_nNumVertices;			// number of vertices.
/**
 * The next available entity ID.
 */
    entity_id_t NextAvailableId;
/**
 * The <tt>part_handle</tt>.
 */
    part_handle ThePartHandle;
/**
 * The name of the part.
 */
    char* ThePartName;
/**
 * The <tt>logical</tt> that determines whether a part has been modified.
 */
    logical HasBeenModified;
/**
 * The flags for the part.
 */
	unsigned long PartFlags;

	// this is a list of entity callbacks to call for notifications.
/**
 * The list of entity callbacks.
 */
	entity_callback_list m_pEntityCallbacks;

	// The model scale is used to specify the units that the PART is
	// stored in.  It is a scale factor that is multiplied by length
	// units in the PART to get the size in millimeters.
/**
 * The model scale specifies the units in which the <tt>PART</tt> is stored.
 * <br><br>
 * <b>Role:</b> It is a scale factor that is multiplied by length units in the
 * <tt>PART</tt> to get the size in millimeters.
 */
	double m_unitsScale;

	// This flag controls whether the units scale factor should be saved
	// with the PART.  If it is TRUE, then a UNITS_SCALE ENTITY will be
	// written into the save file.
/**
 * This flag controls whether the units scale factor is saved with the <tt>PART</tt>.
 * <br><br>
 * <b>Role:</b> If it is <tt>TRUE</tt>, then a <tt>UNITS_SCALEENTITY</tt> is written
 * into the save file.
 */
	logical m_bSaveScale;

	// Rollback history for all toplevel entities in this part unless they
	// have their own history attached via attribute.
/**
 * This is a roll back history for all top-level entities in this part, unless they already have their own history attached using attributes.
 */
	HISTORY_STREAM* m_pHistoryStream;

	// Rollback histories for some top-level entities which have history
	// attached via ATTRIB_HISTORY.  m_pHistoryStream goes in here as well,
	// as the first entry.
/**
 * @nodoc
 */
	HISTORY_STREAM_LIST m_cHistoryStreams;

	// Whether to attach a history_stream attribute to entities when they are
	// added to this part.
/**
 * Roll back histories for some top-level entities which have attached <tt>ATTRIB_HISTORY</tt>.
 * <br><br>
 * <b>Role:</b> <tt>m_pHistoryStream</tt> goes in here as well as the first entry.
 */
	logical m_lEntityHistory;

/**
 * @nodoc
 */
	logical m_update_time_stamp;
/**
 * @nodoc
 */
	friend class acis_pm_entity_mgr;		// to have access to the entity manager back pointer
/**
 * @nodoc
 */
	acis_pm_entity_mgr* m_ent_mgr;	// entity manager back pointer

  public:
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br><b>Role:</b> Optionally, this adds it to the array.
 * <br><br>
 * @param add_to_array
 * Add part to array?
 */
    PART(logical add_to_array = TRUE);
/**
 * C++ destructor, deleting a <tt>PART</tt>.
 */
    virtual ~PART();
/**
 * Attaches a history stream to the given <tt>PART</tt>.
 */
	HISTORY_STREAM* attach_history_stream();
/**
 * Removes a history stream from a <tt>PART</tt>.
 */
	void remove_history_stream();
/**
 * Replace the current history stream.
 * <br><br>
 * @param hs
 * history stream.
 */
	void replace_history_stream(HISTORY_STREAM* & hs); // STI ROLL
/**
 * Returns a history stream for the given <tt>PART</tt>.
 */
	HISTORY_STREAM* history_stream() { return m_pHistoryStream; }
/**
 * Returns whether the given entity has a history stream.
 */
	logical entity_history() { return m_lEntityHistory; }
/**
 * Turns on or off flag stating whether <tt>ENTITY</tt> has a history stream.
 * <br><br>
 * @param onoff
 * history flag.
 */
	void set_entity_history(logical onoff) { m_lEntityHistory = onoff; }
/**
 * Gets the index for the history stream.
 * <br><br>
 * @param hist
 * stream pointer.
 */
	history_id_t get_history_index(HISTORY_STREAM* hist) {
		return m_cHistoryStreams.lookup(hist);
	}
/**
 * Look up a history stream of a part by its id.
 * <br><br>
 * @param id
 * history stream.
 */
	HISTORY_STREAM* lookup_history(history_id_t id) {
		return m_cHistoryStreams[(int)id];
	}

	// Get the handle for this PART
/**
 * Gets the handle for this <tt>PART</tt>.
 */
	part_handle handle() const { return ThePartHandle; }

    // Add an ENTITY to a PART and return its ID.
/**
 * Adds an <tt>ENTITY</tt> to a <tt>PART</tt> and returns its <tt>ID</tt>.
 * <br><br>
 * @param en
 * entity.
 * @param owner
 * owner entity.
 */
    entity_id_t add(ENTITY* en, ENTITY * owner = NULL);

	// This virtual form of add is used when an ID_ATTRIB already exists so
	// that the ENTITY already has an ID.  It should only be called from
	// the non-virtual add method, and from methods of ID_ATTRIB when adding
	// ENTIITES back into a PART after a roll operation
/**
 * Adds an <tt>ENTITY</tt> to a <tt>PART</tt>.
 * <br><br>
 * <b>Role:</b> Use this virtual form of add when an <tt>ID_ATTRIB</tt> already exists
 * so that the <tt>ENTITY</tt> already has an <tt>ID</tt>. Call this only from the non-virtual
 * add function, and from functions of <tt>ID_ATTRIB</tt> when adding entities back into
 * a <tt>PART</tt> after a roll operation.
 * <br><br>
 * @param en
 * entity.
 * @param en_id
 * entity ID.
 */
	virtual void add_entity(ENTITY* en , entity_id_t en_id);

/**
 * Adds an <tt>ENTITY</tt> to a <tt>PART</tt>.
 * <br><br>
 * <b>Role:</b> Use this virtual form of add when an <tt>ID_ATTRIB</tt> already exists
 * so that the <tt>ENTITY</tt> already has an <tt>ID</tt>. Call this only from the non-virtual
 * add function, and from functions of <tt>ID_ATTRIB</tt> when adding entities back into
 * a <tt>PART</tt> after a roll operation.
 * <br><br>
 * @param en
 * entity.
 * @param en_id
 * entity ID.
 */

    virtual void add_entity(ENTITY* en , entity_id_t en_id, ENTITY *)
    {
        add_entity( en, en_id );
    }

	// Call this method when an entity has been updated somehow.  The part
	// itself doesn't really care, but this will trigger the entity_callback
	// mechanism with a pm_Update_Entity so that observers will know that
	// something interesting has occurred.
/**
 * Call this method when an entity has been updated somehow.
 * <br><br><b>Role:</b> The part itself doesn't really care, but this will trigger
 * the <tt>entity_callback</tt> mechanism with a <tt>pm_Update_Entity</tt> so that observers
 * will know that something interesting has occurred. During a later rollback,
 * the <tt>entity_callback</tt> mechanism will be triggered with a <tt>pm_Roll_Update_Entity</tt> .
 * <br><br>
 * This does change the model and therefore starts a new <tt>DELTA_STATE</tt> if one
 * is not already open. The reason for the model change is create a change
 * bulletin on the <tt>ID_ATTRIB</tt> (and the <tt>DISPLAY_ATTRIB</tt> if using the GI too).
 * This change bulletin is noticed during rollback and is used keep the <tt>PART</tt>
 * (and display) in sync with the active state of the model.
 * <br><br>
 * The intended usage is for one to call <tt>PART::update</tt> after each change to the model.
 * One should update inside the delta state where the changes occurred.
 * That is, call <tt>PART::update</tt> and then <tt>api_part_note_state</tt>.
 * There is no need to call <tt>PART::update</tt> after roll.
 * <br><br>
 * @param en
 * entity.
 */
	virtual void update( ENTITY * en);

    // Remove an ENTITY from the part.  The version which takes an ENTITY*
	// will lose the ID_ATTRIB.  The version which takes an entity_id_t
	// assumes that the ID_ATTRIB has already been lost, so it just does
	// any cleanup that is needed to remove it from its tables.  It should
	// usually only be necessary to implement the second form in a derived
	// class.
/**
 * Removes an <tt>ENTITY</tt> from a <tt>PART</tt>.
 * <br><br>
 * @param en
 * entity.
 */
    virtual logical remove(ENTITY* en);
/**
 * Removes an <tt>ENTITY</tt> from a <tt>PART</tt>.
 * <br><br><b>Role:</b>This version assumes that the <tt>ID_ATTRIB</tt> has already been lost,
 * so it cleans up anything that is needed to remove it from its tables.
 * It should only be necessary to implement the second form in a derived class.
 * <br><br>
 * @param en_id
 * entity ID.
 */
    virtual logical remove_id(entity_id_t en_id);

    // Find an ENTITY given its ID
/**
 * Look up a part by its id.
 * <br><br>
 * @param en_id
 * entity ID.
 */
    virtual ENTITY* lookup(entity_id_t en_id) = 0;

    // Get a list of all top level ENTITIES in the part which pass
    // the given entity_filter
/**
 * Gets a list of all top level entities in the <tt>PART</tt> that pass the given filter.
 * <br><br>
 * @param en_list
 * list of entities.
 * @param en_filter
 * entity filter.
 */
    virtual void get_entities(ENTITY_LIST& en_list, entity_filter* en_filter = NULL) const = 0;

	// Get the number of ENTITIES in the PART
/**
 * Gets the number of entities in the <tt>PART</tt>.
 */
	virtual unsigned long count() const;

	// Determine if a PART has any ENTITIES in it
/**
 * Returns <tt>TRUE</tt> if a <tt>PART</tt> is currently empty; otherwise, it returns <tt>FALSE</tt>.
 */
	virtual logical is_empty() const;

    // Write debug information about the part
/**
 * Writes debug information about a <tt>PART</tt> to standard output or to the specified file.
 * <br><br>
 * @param file
 * file name.
 */
    virtual void debug(FILE* file) = 0;

    // Load a part from a .SAT file
/**
 * Loads a <tt>PART</tt> from a file.
 * <br><br><b>Role:</b> This function uses the ACIS functions <tt>start_delta_state</tt>,
 * <tt>api_restore_entity_list</tt>, <tt>api_restore_entity</tt>, and <tt>note_delta_state</tt> in the part load.
 * <br><br>
 * @param file
 * file pointer.
 * @param text_flag
 * flag text.
 * @param en_list
 * entity list.
 * @param his_flag
 * flag for history.
 */
    virtual outcome load(FILE* file, logical text_flag, ENTITY_LIST& en_list, logical his_flag = 0);
/**
 * Loads a <tt>PART</tt> from the specified sat file.
 * <br><br>
 * @param file
 * file name.
 * @param text_flag
 * flag text.
 * @param en_list
 * entity list.
 * @param his_flag
 * flag for history.
 */
    outcome load(const char* file, logical text_flag, ENTITY_LIST& en_list, logical his_flag = 0);
/**
 * Loads a <tt>PART</tt> from a <tt>FileInterface</tt>.
 * <br><br>
 * @param file
 * file Interface.
 * @param en_list
 * entity list.
 * @param his_flag
 * flag for history.
 */
    virtual outcome load(FileInterface* file, ENTITY_LIST& en_list,logical his_flag = 0);

    // Save this PART to a file
/**
 * Saves a <tt>PART</tt> to a file, given the file pointer.
 * <br><br>
 * @param file
 * part file.
 * @param text_flag
 * text.
 * @param his_flag
 *  with history.
 * @param mainline_only
 * main line flag.
 */
    virtual outcome save(FILE* file, logical text_flag, logical his_flag= 0, logical mainline_only=0);  // STI ROLL
/**
 * Saves a <tt>PART</tt> to a file, given the filename as a character string.
 * <br><br>
 * @param file
 * filename.
 * @param text_flag
 * text.
 * @param his_flag
 *  with history.
 */
    outcome save(const char* file, logical text_flag, logical his_flag = 0);

/**
 * Saves a <tt>PART</tt> to a file whose name is based on the part's name.
 * <br><br>
 * @param text_flag
 * text.
 * @param his_flag
 * history.
 */
    outcome save(logical text_flag, logical his_flag = 0);
/**
 * Saves a <tt>PART</tt> to a file interface, with or without history.
 * <br><br>
 * @param file
 * file interface.
 * @param his_flag
 * with history if TRUE.
 */
    virtual outcome save(FileInterface* file, logical his_flag  = 0);

	// Save a list of ENTITIES to a file using the context of this
	// PART.  This can be used for example to save a selection to
	// the clipboard.  It is also using internally by the PART save
	// methods.
/**
 * Save a list of <tt>ENTITYs</tt> to a file using the context of this <tt>PART</tt>.
 * <br><br>
 * <b>Role:</b> This can be used to save a selection to the clipboard.
 * It is also using internally by the <tt>PART</tt> save methods.
 * <br><br>
 * @param en_list
 * list of entities.
 * @param file
 * filename.
 * @param save_scale
 * version.
 * @param with_history
 * save with or without history.
 * @param l mainline_only
 * main line flag.
 */
	virtual outcome save_selection(
		const ENTITY_LIST& en_list, FileInterface* file, logical save_scale,
		logical with_history = 0, logical mainline_only = FALSE);  // STI ROLL

	/**
	 * Deletes all entities and all their history from this PART's history stream.
	 * (Note that if distributed history is not enabled and multiple PARTs
	 * are sharing the same history stream, this call will delete all entities
	 * and all their histories from all PARTs sharing the same history stream.)
	 */
    virtual void clear();

    // Create an ID for an ENTITY.  This method is intended to provide
    // an ID for an ENTITY which belongs to another ENTITY which is
    // already in the PART.  For example, you could call it to get an
    // ID for and EDGE of a solid which is in the PART.  If the owner of
    // the ENTITY is not already in the PART, it is added to the PART.
/**
 * Creates an <tt>ID</tt> for an <tt>ENTITY</tt>.
 * <br><br><b>Role:</b>  This function provides an <tt>ID</tt> for an <tt>ENTITY</tt>
 * that belongs to another <tt>ENTITY</tt> that is already in the <tt>PART</tt>.
 * For example, call it to get an <tt>ID</tt> for an <tt>EDGE</tt> of a solid that
 * is in the <tt>PART</tt>. If the owner of the <tt>ENTITY</tt> is not already in
 * the <tt>PART</tt>, it is added to the <tt>PART</tt>.
 * <br><br>
 * @param en
 * entity.
 */
    virtual entity_id_t create_id(ENTITY* en) = 0;

/**
 * Creates an <tt>ID</tt> for an <tt>ENTITY</tt>.
 * <br><br><b>Role:</b>  This function provides an <tt>ID</tt> for an <tt>ENTITY</tt>
 * that belongs to another <tt>ENTITY</tt> that is already in the <tt>PART</tt>.
 * For example, call it to get an <tt>ID</tt> for an <tt>EDGE</tt> of a solid that
 * is in the <tt>PART</tt>. If the owner of the <tt>ENTITY</tt> is not already in
 * the <tt>PART</tt>, it is added to the <tt>PART</tt>.
 * <br><br>
 * @param en
 * entity.
 */
    virtual entity_id_t create_id( ENTITY* en, ENTITY * ) 
    {
        return create_id( en );
    }

    // Remove IDs from all ENTITIES which are not top level
/**
 * Removes <tt>IDs</tt> from all non-top level entities.
 */
    virtual void remove_ids() = 0;

    // Get the ID of this PART
/**
 * Gets the <tt>ID</tt> of this <tt>PART</tt>.
 */
    unsigned int get_part_id() const {return ThePartHandle.Id();}

    // Set or Get the PART name
/**
 * Returns the name of the <tt>PART</tt>.
 */
    const char* part_name() const {return ThePartName;}
/**
 * Sets the name for the <tt>PART</tt>.
 * <br><br>
 * @param file
 * name.
 */
    void set_part_name(const char* file);

    // See if the PART has been modified since the last save
/**
 * Returns <tt>TRUE</tt> if a <tt>PART</tt> has been modified since the last save; otherwise, it returns <tt>FALSE</tt>.
 */
    logical is_modified() const {return HasBeenModified;}

    // Indicate that an ENTITY in the PART has been modified
/**
 * Returns <tt>TRUE</tt> if an <tt>ENTITY</tt> in the <tt>PART</tt> has been modified; otherwise, it returns <tt>FALSE</tt>.
 * <br><br>
 * @param mod
 * modified or not.
 */
    void set_modified(logical mod) {HasBeenModified = mod;}

	// Get or set the PART flags
/**
 * Gets the <tt>PART</tt> flags.
 */
	unsigned long get_flags() const { return PartFlags; }
/**
 * Sets the <tt>PART</tt> flags.
 * <br><br>
 * @param flag
 * flags.
 */
	void set_flags(unsigned long flag);

	//------------------------------------------------------------
	// Each PART maintains its own list of entity_callbacks which are
	// invoked when ENTITIES in the PART are affected
	// STI sfb: this has been modified. The old version replaced the
	// 			actual callback list itself, which was bad because it
	//			prevented multiple observers on the same part.  This
	//		    version maintains the callback list in the part, but
	//			allows observers to register callbacks.  All methods
	//			return the callback list that the part is holding.
/**
 * Returns a list of <tt>entity_callbacks</tt> for the part, which are invoked when the entities in the <tt>PART</tt> are affected.
 */
	entity_callback_list* get_callbacks() { return &m_pEntityCallbacks; }
/**
 * Invokes an <tt>entity_callback</tt> for the <tt>PART</tt>, which is invoked when the entities in the <tt>PART</tt> are affected.
 * <br><br>
 * @param cb
 * entity callback.
 */
	entity_callback_list* set_callback( entity_callback* cb);
/**
 * Removes a list of <tt>entity_callbacks</tt> from the part.
 * <br><br>
 * @param cb
 * callback.
 */
	entity_callback_list* remove_callback( entity_callback *cb);

// For internal use only
/**
 * @nodoc
 */
	entity_callback_list* remove_no_dtor_callback( entity_callback *cb);

	// This is a convenience method that does the check to see if
	// there is an entity_callback_list for the PART, and if there
	// is one, execute the callback on it.
/**
 * Determines if there is an <tt>entity_callback_list</tt> for the <tt>PART</tt> and, if there is one, executes the callback on it.
 * <br><br>
 * @param en_event
 * type of event.
 * @param en
 * entity.
 */
    void execute_callbacks(entity_event_type en_event, ENTITY* en );

	//------------------------------------------------------------
	// Get or set the units scale for the PART.  The second argument
	// controls whether or not any ENTITIES which are already in the
	// PART should be scaled to use the new units scale.  If it is
	// TRUE, then the PART is scaled, otherwise the value of the
	// units scale is changed, but the ENTITY definitions themselves
	// are not.
/**
 * Gets the units scale for the <tt>PART</tt>.
 */
	double UnitsScale() const { return m_unitsScale; }
/**
 * Sets the units scale for the <tt>PART</tt>.
 * <br><br><b>Role:</b> Setting the units scale for a <tt>PART</tt> does not change the data;
 * it changes how that data is interpreted. When the units scale is changed,
 * the use may also want to scale all the entities in the <tt>PART</tt> so that
 * they remain the same size.
 * <br><br>
 * @param scale
 * units scale to use.
 * @param  scale_part
 * part is scaled?
 */
	void SetUnitsScale(double scale, logical scale_part = TRUE);

	// These two methods let you get or set the units scale using
	// a character string which represents the units to use.
/**
 * Return the units used in the <tt>PART</tt>.
 */
	const char* PartUnits() const;
/**
 * Sets the units scale using a character string that represents the units to use.
 * <br><br>
 * @param unitsName
 * units scale to use.
 * @param  scale_part
 * part is scaled?
 */
	void SetPartUnits(const char* unitsName, logical scale_part = TRUE);

	// These methods let you control whether or not to save the units
	// scale with the PART
/**
 * Returns <tt>TRUE</tt> if the method saves the units scale with the <tt>PART</tt>; otherwise, this method returns <tt>FALSE</tt>.
 */
	logical SaveScale() const { return m_bSaveScale; }
/**
 * Determines whether the units scale is saved with the <tt>PART</tt>.
 * <br><br>
 * @param save
 * save units?
 */
	void SetSaveScale(logical save) { m_bSaveScale = save; }

	// These methods perform the conversion between PART units and
	// millimeters
/**
 * Converts between millimeters to <tt>PART</tt> units.
 * <br><br>
 * @param value
 * scale factor.
 */
	double MMToModel(double value) const { return value / m_unitsScale; }
/**
 * Converts between <tt>PART</tt> units to millimeters.
 * <br><br>
 * @param value
 * scale factor.
 */
	double ModelToMM(double value) const { return value * m_unitsScale; }

	// Used to get the max possible picks.
/**
 * Returns the maximum number of picks possible.
 */
	virtual int get_MaxNumberofHits()
	{
		return count() * (m_nNumFaces + m_nNumEdges + m_nNumTCoEdges + m_nNumVertices);
	}

    // STI ROLL - function to make sure default stream is the active part's stream when
    //            distributing
/**
 * Make sure the default stream is the active part's stream when distributing.
 */
    HISTORY_STREAM* update_default_stream() const;

    // STI ROLL - function to put ATTRIB_ID's in correct state after loading
    //            a history stream
/**
 * Returns whether the time stamp mechanism is active or not.
 */
    logical time_stamp_allow_update() { return m_update_time_stamp; };

/**
 * Turn on time stamp mechanism.
 */
    void time_stamp_activate();

/**
 * Turn off time stamp mechanism.
 */
    void time_stamp_deactivate();

/**
 * @nodoc
 */
	acis_pm_entity_mgr* get_entity_manager() const;

private:
    outcome fixup_part_ids(HISTORY_STREAM* pStream);

// This is to remove the warning
//	copy constructor could not be generated PRS
protected:
/**
 * C++ constructor.
 * <br><br>
 * @param prt
 * part.
 */
	PART(const PART& prt);
};

//======================================================================

// Find the PART that an ENTITY belongs to

/**
* Gets the <tt>PART</tt> for the specified <tt>ENTITY</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param entity
* entity.
**/
DECL_PART PART* get_part(const ENTITY *entity);

#ifdef THREAD_SAFE_ACIS
extern DECL_PART safe_object_pointer<PART> backgroundPart;
#else
extern DECL_PART PART* backgroundPart;
#endif



// Detirmine if two ENTITYs are in the same part

// For internal use only
/**
 * Check that two entities are in the same part.
 * <br><br>
 * <b>Role:</b> With multiple histories this check is required before operations
 * that merge entities together to avoid intermixing the histories.  When the check
 * fails, the caller can either abort with an error or copy one of the entities
 * into the part of the other.
 * <br><br>
 * @param ent1
 * first entity.
 * @param ent2
 * second entity.
 */
DECL_PART logical same_part(const ENTITY* ent1, const ENTITY* ent2);

// Return the number of PARTS defined
/**
* Counts and returns the number of parts.
* <br><br>
* The <tt>next_id</tt> also increments.
* <br><br>
* <b>Effect:</b> Read-only
**/
DECL_PART int count_parts();

// The proceudre get_next_part is used to iterate through all of the PARTs
// that are defined.  The argument is used to keep track of the SPAposition
// in the list of PARTS 0 it is both an input and output argument.
// To iterate though all of the PARTs, you can use code similar to the
// following example.
//   int pos = 0;
//   PART* part = NULL;
//   while( (part = get_next_part(pos)) != NULL ) {
//     do something with the part
//   }

/**
* Gets the next <tt>PART</tt> with an <tt>ID</tt> that is greater than or equal to the <tt>next_id</tt>.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param index
* part ID.
**/
DECL_PART PART* get_next_part(int &index);

// Delete all PARTs that are defined.  This is normally only called to
// clean up from toolkit_terminate.

// For internal use only
/**
 * @nodoc
 */
DECL_PART void delete_all_parts();


// This procedure lets you lookup a PART if you know its ID but not its
// handle.  If you know the PART's handle, you should use the Part
// method of part_handle to get the PART instead of this procedure
// because it will be faster and more reliable.
/**
* Finds a <tt>PART</tt> given the <tt>PART's ID</tt> but not its handle.
* <br><br>
* <b>Role:</b> Looks up a <tt>PART</tt> given the <tt>PART's ID</tt> but not its handle.
* If the user knows the <tt>PART's</tt> handle, use the <tt>Part</tt> method of <tt>part_handle</tt>
* to get the <tt>PART</tt> instead of this procedure because it is faster and more reliable.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param id
* part id.
**/
DECL_PART PART* find_part(unsigned int id);



/** @} */
#endif
