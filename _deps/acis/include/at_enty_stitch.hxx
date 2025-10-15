/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (ATTRIB_HH_ENT_CLASS)
#define ATTRIB_HH_ENT_CLASS

/**
 * @file at_enty_stitch.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup STITCHAPI
 *
 * @{
 */
#include "vlists.hxx"

#include "att_hh.hxx"

// For internal use only
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_HH_ENT, STITCH)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// For internal use only
/**
 * @nodoc
 */
DECL_STITCH extern int ATTRIB_HH_ENT_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_HH_ENT_LEVEL (ATTRIB_HH_LEVEL + 1)

class ATTRIB_HH_AGGR;

// Declaration of the base class for all the individual
// entity attributes for the various modules.
/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated. For healing, use API interfaces
 * and not Direct Interface.
 * <br><br>
 * Base HEAL individual entity-level attribute class.
 * <br>
 * <b>Role:</b> <tt>ATTRIB_HH_ENT</tt> is the base individual entity-level attribute class from which other
 * HEAL individual entity-level attribute classes are derived. Individual entity-level attributes
 * are attached to the individual entities of body being healed to store entity-specific information
 * about each phase or subphase of the healing process. The individual entity-level attributes for
 * each phase or subphase are managed by the aggregate attribute for that phase/subphase.
 */
class DECL_STITCH ATTRIB_HH_ENT : public ATTRIB_HH
{
		// Pointer to the parent aggregate attribute
		//ATTRIB_HH_AGGR* m_parent;

        // RNC 8 july 99: void list added in attrib_hh_ent
	    // for logging on changes done to each entity during
	    // the healing process.
protected:
	    VOID_LIST*  m_log_list;

public:
 		// Constructor
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt> operator inherited
 * from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_HH_ENT(...)</tt>), because this reserves the memory on
 * the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param e
 * owning entity.
 */
		ATTRIB_HH_ENT(ENTITY* e = NULL);

/**
 * Returns <tt>TRUE</tt> if this is pattern compatible.
 */
        virtual logical pattern_compatible() const;
                                    // returns TRUE
		// Get the pointer to the owning body
		// up the topological heirarchy
/**
 * Returns the pointer to the owning body.
 */
		BODY* get_owner_body();

		// Function to sketch the owner entity
		// in default body colour.
		// To be used only for test-harness

// For internal use only
/**
 * @nodoc
 */
		virtual void draw();

		// Function to sketch the owner entity
		// in the specified colour.
		// To be used only for test-harness
/**
 * Sketches the owner entity in the specified color.
 * <br><br>
 * @param col
 * specified color.
 */
		virtual void draw_in_col(int col);

		// RNC : 8 july 99 function added for adding strings to log list.
/**
 * Adds strings to the log list.
 * <br><br>
 * @param str
 * string to add.
 */
		virtual void append_to_log(char *str);
/**
 * Returns the log list.
 */
		virtual VOID_LIST* get_log_list();

		// For internal use only
		/**
		 * @nodoc
		 */
		ATTRIB_FUNCTIONS (ATTRIB_HH_ENT, STITCH)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

// For internal use only
/**
 * @nodoc
 */
DECL_STITCH BODY* hh_get_owner_body(ENTITY* e);

/** @} */
#endif // ATTRIB_HH_ENT_CLASS
