/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (ATTRIB_HH_AGGR_CLASS)
#define ATTRIB_HH_AGGR_CLASS

/**
 * @file at_aggr_stitch.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup STITCHAPI
 *
 * @{
 */
#include "att_hh.hxx"
#include "dcl_stitch.h"
#include "bhl_stru2.hxx"
#include "hh_stru2.hxx"

// For internal use only
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_HH_AGGR, STITCH)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
// For internal use only
/**
 * @nodoc
 */
DECL_STITCH extern int ATTRIB_HH_AGGR_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_HH_AGGR_LEVEL (ATTRIB_HH_LEVEL + 1)

class ATTRIB_HH_ENT;

// Declaration of the base class for all aggregate attributes
// for the vrious healing modules.

/**
 * @nodoc
 * <b>Obsolete:</b> This class has been deprecated. For healing, use API interfaces
 * and not Direct Interface.
 * <br><br>
 * Base HEAL aggregate attribute class.
 * <br>
 * <b>Role:</b> <tt>ATTRIB_HH_AGGR</tt> is the base aggregate attribute class from which other HEAL
 * aggregate attribute classes are derived. Aggregate attributes are attached to the body being
 * healed to store information about each phase or subphase of the healing process. Aggregate
 * attributes also manage the individual attributes attached to entities of the body during healing.
 * @see ENTITY, VERTEX, SPAposition
 */
class DECL_STITCH ATTRIB_HH_AGGR : public ATTRIB_HH
{
protected:

		MODULE_HEAL_STATUS m_module_state;

public:

		// Constructor
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt> operator inherited
 * from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_HH_AGGR(...)</tt>), because this reserves the memory on
 * the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param owner
 * owning body to heal.
 */
		ATTRIB_HH_AGGR (BODY* owner = NULL);
/**
 * Returns <tt>TRUE</tt> if this is pattern compatible.
 */
        virtual logical pattern_compatible() const;
                                    // returns TRUE
		////////////////////////////////////////////////////////
		// The following are virtual functions which need to
		// be implemented by every by every class derived
		// from this class.

		//RNC : 16 Jun 99
/**
 * Sets the current status of healing.
 * <br><br>
 * @param state
 * status.
 */
		virtual void set_heal_status (enum MODULE_HEAL_STATUS state);
/**
 * Gets the current status of healing.
 */
		enum MODULE_HEAL_STATUS module_state() {return m_module_state;}

		// Chain individual attributes
/**
 * This is a virtual function that must be implemented by every class derived from this class.
 * <br><br>
 * <b>Role:</b> Attaches an individual entity-level attribute to the given entity. This method chains
 * individual attributes.
 * <br><br>
 * @param e
 * entity to attach to.
 */
		virtual  ATTRIB_HH_ENT*  attach_attrib( ENTITY* e);

		// Get all the entities chained to the body. These are
		// entities to which individual attributes are attached.
/**
 * This is a virtual function that must be implemented by every class derived from this class.
 * <br><br>
 * <b>Role:</b> Gets all the entities chained to the body. These are entities to which individual
 * attributes are attached.
 * <br><br>
 * @param e
 * list of entities.
 */
		virtual void entity_list(ENTITY_LIST& e) const;

		// Get the attribute for the corresponding entity
/**
 * This is a virtual function that must be implemented by every class derived from this class.
 * <br><br>
 * <b>Role:</b> Gets the attribute for the corresponding entity.
 * <br><br>
 * @param e
 * owning entity.
 */
		virtual ATTRIB_HH_ENT* get_attrib( ENTITY* e) const;

 		// Remove the attribute for the entity
/**
 * This is a virtual function that must be implemented by every class derived from this class.
 * <br><br>
 * <b>Role:</b> Removes the attribute for the entity.
 * <br><br>
 * @param e
 * owning entity.
 */
		virtual void detach_attrib( ENTITY* e);

		// Remove all individual attributes.
		// LOSE_DEF calls it as well
/**
 * This is a virtual function that must be implemented by every class derived from this class.
 * <br><br>
 * <b>Role:</b> Removes all individual attributes.
 */
		virtual void  cleanup();

		// Analyze the body and compute the best options
		// and tolerances for a particular module.
/**
 * This is a virtual function that must be implemented by every class derived from this class.
 * <br><br>
 * <b>Role:</b> Analyzes the body and compute the best options and tolerances for a particular
 * healing phase/subphase.
 */
		virtual void analyze();

		// Perform calculations and store all the
		// recommended changes in individual attributes
/**
 * This is a virtual function that must be implemented by every class derived from this class.
 * <br><br>
 * <b>Role:</b> Performs the calculations for a particular healing phase or subphase and stores
 * all the recommended changes in individual attributes.
 */
		virtual void calculate();

		// Fixup all the changes stored in individual attributes
/**
 * This is a virtual function that must be implemented by every class derived from this class.
 * <br><br>
 * <b>Role:</b> Applies (fixes) all the changes that are stored in individual attributes for a
 * particular healing phase/subphase to the body.
 */
		virtual void fix();

		// Print the results of the operations in the module
/**
 * This is a virtual function that must be implemented by every class derived from this class.
 * <br><br>
 * <b>Role:</b> Prints the results of the operations in a particular healing phase/subphase.
 * <br><br>
 * @param fp
 * file to print to.
 */
		virtual void print(FILE* fp);

		// Print results of analysis
/**
 * This is a virtual function that must be implemented by every class derived from this class.
 * <br><br>
 * <b>Role:</b> Prints the results of the analyze stage of a particular healing phase/subphase.
 * <br><br>
 * @param fp
 * file to print to.
 */
		virtual void print_analyze(FILE* fp);

		// Print results of calculations
/**
 * This is a virtual function that must be implemented by every class derived from this class.
 * <br><br>
 * <b>Role:</b> Prints the results of the calculate stage of a particular healing phase/subphase.
 * <br><br>
 * @param fp
 * file to print to.
 */
		virtual void print_calculate(FILE* fp);

		// Print results of fix
/**
 * This is a virtual function that must be implemented by every class derived from this class.
 * <br><br>
 * <b>Role:</b> Prints the results of the fix stage of a particular healing phase.
 * <br><br>
 * @param fp
 * file to print to.
 */
		virtual void print_fix(FILE* fp);

		// Print results of check
/**
 * This is a virtual function that must be implemented by every class derived from this class.
 * <br><br>
 * <b>Role:</b> Prints the results of the check stage of a particular healing phase/subphase.
 * <br><br>
 * @param fp
 * file to print to.
 */
		virtual void print_check(FILE* fp);

		//
		///////////////////////////////////////////////////////

		/**
		 * @nodoc
		 */
		ATTRIB_FUNCTIONS (ATTRIB_HH_AGGR, STITCH)
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
DECL_STITCH ATTRIB_HH_AGGR* find_aggr_attrib(ENTITY* e, int type);
/** @} */
#endif // ATTRIB_HH_AGGR_CLASS

