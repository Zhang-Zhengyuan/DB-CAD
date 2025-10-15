// $Id: stde_info.hxx,v 1.5 2000/12/26 18:48:12 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( STD_ERROR_INFO )
#define STD_ERROR_INFO

// Main definition file for standard error information return object.
// Contains space for two ENTITYs, which is sufficient for use in many
// circumstances.  For example, in LOP when unable to solve for an EDGE,
// the error LOP_TWK_NO_EDGE will produce an outcome that points to
// a standard error info object with entity0 set to the EDGE in question.
// This happens automatically by calling sys_error( LOP_TWK_NO_EDGE, edge )

// Also contains flags that warn user if the returned ENTITY will be
// deleted on rollback.

#include "dcl_kern.h"
#include "logical.h"
#include "err_info_base.hxx"
#include "err_info.hxx"

/**
 * @file stde_info.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISERRORMANAGEMENT
 *
 * @{
 */

class ENTITY;
/**
 * Encapsulates standard error information.
 * <br>
 * <b>Role:</b> This class contains standard error information, which is sufficient
 * for use in many circumstances. For example, when working in the Local Operations
 * Component, if unable to solve for an <tt>EDGE</tt>, the error <tt>LOP_TWK_NO_EDGE</tt> will
 * produce an outcome that points to a standard error info object. The argument
 * <tt>entity0</tt> will be set to the <tt>EDGE</tt> in question.
 * <br><br>
 * This class also contains flags that warn the user if the returned <tt>ENTITY</tt> will
 * be deleted upon roll back.
 * <br><br>
 * Two overloaded versions of the function <tt>sys_error</tt> set a global pointer to an
 * <tt>error_info</tt> object. One version is passed an <tt>error_info</tt> object, and the other
 * creates a <tt>standard_error_info</tt> object when <tt>sys_error</tt> is passed one or two <tt>ENTITY</tt>s.
 * The <tt>standard_error_info</tt> class is derived from <tt>error_info</tt>, which provides error
 * data that is adequate in a majority of cases, such as local operations and
 * blending.
 * <br><br>
 * In the Local Ops, Remove Faces, and Shelling Components, the <tt>error_info</tt> object
 * returns an <tt>ENTITY</tt> that further specifies where the local operation first fails,
 * when such information is available. A <tt>standard_error_info</tt> object is adequate
 * for use in these components, and more detailed information could be returned,
 * if necessary, by deriving a new class.
 * <br><br>
 * @see ENTITY, error_info
 */
class DECL_KERN standard_error_info: public error_info {
public:
    // default return data
    /**
     * First entity with standard error information.
     */
    ENTITY *entity0;
    /**
     * Second entity with standard error information.
     */
    ENTITY *entity1;
    /**
     * Flag that warns whether the returned <tt>entity0</tt> will be deleted on rollback.
     */
    logical entity0_dead; // flags to warn if "dead" entity is being returned
    /**
     * Flag that warns whether the returned <tt>entity1</tt> will be deleted on rollback.
     */
    logical entity1_dead;

    // constructor and destructor
    /**
     * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
     * <br><br>
     * @param e0
     * Pointer to first entity associated with the error.
     * @param e1
     * Pointer to second entity associated with the error.
     */
    standard_error_info( ENTITY* e0 = NULL, ENTITY* e1 = NULL );
    standard_error_info( char const* gme, ENTITY* e0 = NULL, ENTITY* e1 = NULL );
    /**
     * C++ destructor, deleting a <tt>standard_error_info</tt>.
     */
    virtual ~standard_error_info();

    // identify the error object
    /**
	 * Returns the id number for the class <tt>standard_error_info</tt>.
	 */
    static int id();

	/**
	 * Returns the id number for whichever class this object happens to be. If this
	 * object is of the class <tt>standard_error_info</tt>, then the id number returned by this function
	 * would be equal to the one returned by <tt>standard_error_info::id</tt>.
	 */
    virtual int type() const;
    virtual int gme_type() const;

	/**
	 * @nodoc 
	 */
	// Sets the entity0. This method is recommended for setting entity0 instead of directly
	// setting through the member variable
    void set_entity0(ENTITY *ent);
    void gme_set_entity0(ENTITY *ent);
	/**
	 * @nodoc 
	 */
	// Sets the entity1. This method is recommended for setting entity1 instead of directly
	// setting through the member variable
    void gme_set_entity1(ENTITY *ent);
    void set_entity1(ENTITY *ent);
};

/** @} */
#endif

