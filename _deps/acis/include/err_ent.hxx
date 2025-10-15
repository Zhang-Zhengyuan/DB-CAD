// $Id: err_ent.hxx,v 1.1 2001/09/10 16:27:50 ywoo Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Emacs indenting style choice
// Indent style: D3

//#include "boolean/dcl_bool.h"
#include "dcl_intr.h"
#include "errmsg.hxx"

#include "entity.hxx"
#include "tophdr.hxx"
#include "lists.hxx"

// for errors
#include "sanity.err"
#include "param.hxx"
/**
* @file err_ent.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

#if !defined ( ERROR_ENTITY_CLASS )
#define ERROR_ENTITY_CLASS 

/** \addtogroup ACISCHECKING
 *  \brief Declared at <err_ent.hxx>, SPAintr
 *  @{
 */

// Identifier used to find out (via identity() defined below) to what 
// an entity pointer refers.
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ERROR_ENTITY, INTR )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

/**
 * @nodoc
 */
extern DECL_INTR int ERROR_ENTITY_TYPE;

// Identifier that gives number of levels of derivation of this class
// from ENTITY

/**
 * @nodoc
 */
#define ERROR_ENTITY_LEVEL (ENTITY_LEVEL + 1)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif



/**
 * Stores information about improper intersections in an ENTITY.  
 * <br>
 * <b>Role:</b> This class is used to report problems found during
 * checks for improper intersections in an entity.  It is used by @href api_check_entity 
 * when <tt>check_ff_int</tt> is set to TRUE or the <tt>check_level</tt>
 * is set to 70, and by @href api_check_entity_ff_ints and @href api_check_list_ff_ints.
 * Please refer to the documentation for @href api_check_entity_ff_ints for 
 * more information about improper intersections. <br><br>
 * Each @href ERROR_ENTITY contains the following information: <br> 
 * <br>
 * <ul>
 * <li>Pointers to each of the improperly intersecting entities;</li>
 * <li>An @href ENTITY_LIST containing edges and vertices that are in the 
 * intersection but not in the original body (this could be NULL when
 * the problem is improper containment);</li>  
 * <li>An error ID number indicating the problem type. Possible values
 * and their associated error message strings are: <br><br>
 * <table>
 * <tr>
 * <td><tt>CHECK_BAD_FF_INT</tt></td> <td>"improper face/face intersection"</td>
 * </tr>
 * <tr>
 * <td><tt>CHECK_BAD_FF_COIN</tt></td> <td>"improper face/face coincidence"</td>
 * </tr>
 * <tr> 
 * <td><tt>CHECK_BAD_SHELL_CONT</tt></td> <td>"improper shell containment"</td>
 * </tr>
 * <tr> 
 * <td><tt>CHECK_BAD_LUMP_CONT</tt></td> <td>"improper lump containment"</td>
 * </tr>
 * <tr> 
 * <td><tt>CHECK_FAILED_FF_INT</tt></td> <td>"intersection failure"</td>
 * </tr>
 * <tr> 
 * <td><tt>CHECK_BAD_EE_INT</tt></td> <td>"improper edge/edge intersection"</td>
 * </tr>
 * </table>
 * </li>
 * <br>
 * An example use of this class can be found in <tt>scm/scmext/bool/ffint_scm.cpp</tt>.<br>
 * @see ENTITY, ENTITY_LIST, insanity_list
 */
class DECL_INTR ERROR_ENTITY : public ENTITY
    {
    // data members
    ENTITY* _owner0;  // pointers to the problem entities
    ENTITY* _owner1;

    err_mess_type err_id;  // error id number identifying the error
    ENTITY_LIST* intersection_list;   // list of entities not on the 
                                      // body that lie on the improper
                                      // intersection between _owner0
                                      // and _owner1

	
    // Include the standard member functions for all entities.

    /**
     * @nodoc
     */
    ENTITY_FUNCTIONS( ERROR_ENTITY, INTR )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:

    /**
     * @nodoc
     */
    TRANSFORM_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
    /**
	 * C++ allocation constructor requests memory for this object but does not populates it with the data. 
	 * Applications should call this constructor only with the overloaded new operator inherited from the 
	 * <tt>ENTITY</tt> class (for example, x=new ERROR_ENTITY(...)), because this reserves the memory on 
	 * the heap, a requirement to support roll back and history management.
	 */
    ERROR_ENTITY();

	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied 
	 * as arguments. Applications should call this constructor only with the overloaded new operator 
	 * inherited from the <tt>ENTITY</tt> class (for example, x=new ERROR_ENTITY(...)), because this 
	 * reserves the memory on the heap, a requirement to support roll back and history management.
	 * @param ent0
     * ptr to problem entity.
	 * @param ent1
     * ptr to problem entity.
	 * @param eid
     * error ID number.
	 * @param i_list
     * list of entities not on the body that lie on the improper intersection between ent0 and ent1.
	 */
    ERROR_ENTITY( ENTITY *ent0, ENTITY *ent1, 
		  err_mess_type eid, 
		  ENTITY_LIST* i_list);

    // data access
    /** 
	 * Returns a pointer to specified owning entity.
	 * @param i
     * owner number.
	 */
    ENTITY* get_owner( int i) const { return ( i == 0 ? _owner0 : _owner1 ); }

    /** 
	 * Returns the error number associated with entities.
	 */
    err_mess_type get_error_id() const { return err_id; }
    /** 
	 * Returns the intersection list between the two entities.
	 */
    ENTITY_LIST* get_intersection_list() const { return intersection_list; }
    //const char* error_message() const { return err_message[ err_id ]; }

public:

	// A point or curve region representing a self-intersection of a curve or
	// an intersection between two boundary entities (for instance, two edges of 
	// a face).

	class DECL_INTR param_info :public ACIS_OBJECT
	{
		// The parameter space data corresponding to the self-intersection.

		logical _param0_set, _param1_set;
		SPApar_pos _param_pos0, _param_pos1;		

		// The vertex or edge created to represent the self-intersection region
		// in object space.

		ENTITY * _error_ent;
		
		// The next self-intersection region.

		param_info *next;		

		// The owning error entity object for all the self-intersections between
		// two entities.

		ERROR_ENTITY * _owner_ent;

		ENTITY * get_owner( logical first_owner ) const 
		{
			return first_owner ? _owner_ent->get_owner( 0 ) :
				_owner_ent->get_owner( 1 ) ; 
		}

	public:	

		// An intersection between two edges.
		
		param_info( double par_pos0, double par_pos1, ENTITY * ent );		

		// An intersection between two faces.

		param_info( SPApar_pos par_pos0, SPApar_pos par_pos1,  ENTITY * ent );


		/**
		 * @nodoc
		 */
		void set_param( logical first_owner, double const& param );

		/**
		 * @nodoc
		 */
		void set_param( logical first_owner, SPApar_pos const& uv );

		/**
		 * @nodoc
		 */
		void get_param( logical first_owner, SPApar_pos& param) const;

		/**
		 * @nodoc
		 */
		void get_param( logical first_owner, double& param) const;

		/**
		 * @nodoc
		 */
		void set_next( param_info* next_ptr);

		/**
		 * @nodoc
		 */
		param_info * get_next() const;

		/**
		 * @nodoc
		 */
		void set_owner( ERROR_ENTITY * owner_ent)
		{
			_owner_ent = owner_ent;
		}
	};

	param_info * _par_info;

	/**
     * @nodoc
     */
	void add_param_info( param_info* par_info);

};

/** @} */

#endif






