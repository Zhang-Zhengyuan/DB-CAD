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
//    define the WCS class
//
// design---
//      A WCS is derived from ENTITY so that it can be saved and restored
//      and so that it can be rolled.  It is used to define a transform
//      which maps input into the coordinate system of the model.  Because
//      we often want to map both from and to model space, Both transforms
//      are kept to avoid having to keep re-computing the inverse.
//
//----------------------------------------------------------------------

#ifndef wcs_hxx
#define wcs_hxx

//======================================================================
/**
 * \defgroup ACISWCS Working Coordinate Systems
 * \ingroup KERNAPI
 *
 */
/**
 * @file wcs.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISWCS
 *
 * @{
 */
#include "dcl_kern.h"
#include "entity.hxx"
#include "transf.hxx"

//----------------------------------------------------------------------

class SPAposition;
class SPAvector;
class SPAunit_vector;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(WCS , KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

//======================================================================

// Identifier used to find out (via identity() defined below) to what
// an entity pointer refers.

extern DECL_KERN int WCS_TYPE;

/**
 * Identifier that gives number of levels of derivation of this class
 * from ENTITY.
 */
#define WCS_LEVEL 1


// Routine to restore an WCS from disc (declared here to make it
// a friend and static).

//LOCAL_PROC ENTITY* wcs_restore_data();


// WCS declaration proper.
/**
 * Defines a working coordinate system.
 * <br>
 * <b>Role:</b> The <tt>WCS</tt> class encapsulates the definition of a working coordinate system (WCS). A working
 * coordinate system (<tt>WCS</tt>) is a local coordinate system that simplifies the entering of position or vector
 * information while creating models.  The class contains a transformation that maps input into the coordinate system
 * of the model, along with the inverse used to map from model space to the WCS.  As it is derived from <tt>ENTITY</tt>, both
 * history and persistence are suppported.
 * <br><br>
 * Multiple working coordinate systems can be created; however, only one can be active at a time.
 * Some of the curve- and solid-creation <tt>API</tt> routines use the active WCS to determine the orientation
 * of the object being created. The <i>xy-</i>plane of the active WCS defines the plane of planar curves,
 * such as arcs.  API routines are provided that create a <tt>WCS</tt>, make it active, apply it to positions
 * and/or vectors, and query its properties.
 * <br><br>
 * @see SPAtransf
 */
class DECL_KERN WCS: public ENTITY {

    // transform from this coordinate system to model space
    SPAtransf to_model_transf;

    // transform from model space to this coordinate system
    // (this is the inverse of to_model_transf)
    SPAtransf to_wcs_transf;

	// Include the standard member functions for all entities.

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( WCS , KERN)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:


	/**
	 * @nodoc
	 */
	TRANSFORM_FUNCTION
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

public:

    //----------------------------------------
    // Constructors

	// Construct a WCS which has the identiy transform.  This is equivalent
    // to model space.
/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 * <br><br>
 * <b>Role:</b> The allocation constructor is used primarily by restore. Applications should call
 * this constructor only with the overloaded <tt>new</tt> operator, because this reserves the memory on the
 * heap, a requirement to support roll back and history management.
 */
	WCS();


	// Make a WCS from a SPAtransf. The SPAtransf defines the transformation which
    // maps from this WCS to model space.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt> operator,
 * because this reserves the memory on the heap, a requirement to support roll back and history
 * management.
 * <br><br>
 * Creates a <tt>WCS</tt> from a transformation. The <tt>SPAtransf</tt> argument defines the transformation that maps from the
 * <tt>WCS</tt> to model space.
 * <br><br>
 * @param to_model_space
 * the transformation that maps to model space.
 */
	WCS( SPAtransf const & to_model_space );

    //----------------------------------------
    // Destructor
    //  The destructor of WCS is private.  To delete a WCS, use the lose
    //  method.  If you lose the active WCS, then the active WCS is reset
    //  to be NULL which means that no mapping is performed.


    //----------------------------------------
    // Inquiry Functions

    // Get a SPAtransf which will map from this WCS to model space
/**
 * Returns the transformation that maps from this <tt>WCS</tt> to model space.
 */
	SPAtransf const &to_model() const;

    // Get a SPAtransf which will map from model space to this WCS
/**
 * Returns the transformation that maps from model space to this <tt>WCS</tt>.
 */
	SPAtransf const &to_wcs() const;

    // get the origin and the axes in model space
/**
 * Returns the origin of the coordinate axes in model space.
 */
    SPAposition origin() const;
/**
 * Returns the <i>x-</i>axis in model space.
 */
    SPAunit_vector x_axis() const;
/**
 * Returns the <i>y-</i>axis in model space.
 */
    SPAunit_vector y_axis() const;
/**
 * Returns the <i>z-</i>axis in model space.
 */
    SPAunit_vector z_axis() const;

    // map a SPAposition from model space to this WCS
/**
 * Maps a position from model space to this <tt>WCS</tt>.
 * <br><br>
 * @param position
 * the position to map.
 */
    SPAposition to_wcs(const SPAposition& position) const;

    // map a SPAvector from model space to this WCS
/**
 * Maps a vector from model space to this <tt>WCS</tt>.
 * <br><br>
 * @param vector
 * the vector to map.
 */
    SPAvector to_wcs(const SPAvector& vector) const;

    // map a SPAunit_vector from model space to this WCS
/**
 * Maps a unit vector from model space to this <tt>WCS</tt>.
 * <br><br>
 * @param vector
 * the unit vector to map.
 */
    SPAunit_vector to_wcs(const SPAunit_vector& vector) const;

    // map a SPAposition from this WCS to model space
/**
 * Maps a position from this <tt>WCS</tt> to model space.
 * <br><br>
 * @param position
 * the position to map.
 */
    SPAposition to_model(const SPAposition& position) const;

    // map a SPAvector from this WCS to model space
/**
 * Maps a vector from this <tt>WCS</tt> to model space.
 * <br><br>
 * @param vector
 * the vector to map.
 */
    SPAvector to_model(const SPAvector& vector) const;

    // map a SPAunit_vector from this WCS to model space
/**
 * Maps a unit vector from this <tt>WCS</tt> to model space.
 * <br><br>
 * @param vector
 * the unit vector to map.
 */
    SPAunit_vector to_model(const SPAunit_vector& vector) const;

    //----------------------------------------
    // Change Functions

    // Set the transform which maps from this WCS to model space
/**
 * Sets the transformation that maps from this <tt>WCS</tt> to model space.
 * <br><br>
 * @param trans
 * the new transformation.
 */
	void set_to_model( SPAtransf const& trans);

    // apply a transform to the WCS
/**
 * Applies a transformation to the <tt>WCS</tt>.
* <br><br>
 * @param trans
 * the transformation applied.
 */
	void operator*=( const SPAtransf& trans);

    //----------------------------------------
/*
// tbrv
*/
/**
 * @nodoc
 */
	int lookup( logical ) const;
/*
// tbrv
*/
/**
 * @nodoc
 */
    virtual void roll_notify(BULLETIN_TYPE, ENTITY*);
};

//----------------------------------------------------------------------

//======================================================================
/** @} */
#endif
