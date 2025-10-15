/* ORIGINAL: 3dt2.1/geomhusk/efilter.hxx */
// $Id: efilter.hxx,v 1.13 2001/01/11 18:08:27 ywoo Exp $
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
//    define entity_filter classes
//
// usage---
//    Entity_filters can be passed to routines which return a list of
//    ENTITYs, such as pick routines.  The called routine can use the
//    test function in the filter to select a subset of the possibilities.
//
//    Entity_filter is more flexible than a simple pointer to a function,
//    because complex logical expressions can be built using
//    entity_and_filter, entity_or_filter and entity_not_filter.
//
//    Entity_test_filter handles the case of a single pointer to function.
//
// example---
//    Create an entity_filter which will return true for green verticies
//    or blue points.  This example uses the gihusk to get the color.
//
//    logical is_blue( const ENTITY* e ) {
//        int color; api_gi_get_entity_color( e, color );
//        return color == 4;
//    }
//
//    logical is_green( const ENTITY* e ) {
//        int color; api_gi_get_entity_color( e, color );
//        return color == 2;
//    }
//
//    entity_filter *complex_filter = new entity_or_filter(
//        new entity_and_filter(
//            new entity_test_filter( is_green ),
//            new entity_test_filter( is_VERTEX )
//        ),
//        new entity_and_filter(
//            new entity_test_filter( is_blue ),
//            new entity_test_filter( is_APOINT )
//        )
//    );
//
//----------------------------------------------------------------------

#ifndef efilter_hxx
#define efilter_hxx

//======================================================================

#include "dcl_kern.h"
#include "logical.h"
// ywoo 08Jan01: added the header file.
#include "mmgr.hxx"
// ywoo: end.

/**
* @file efilter.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISENTITYOPS
 *
 * @{
 */

class ENTITY;

//======================================================================

//----------------------------------------------------------------------
// purpose---
//    Base class for entity filters.
//
// type---
//    Abstract.  This class cannot be instantiated.
//---------------------------------------------------------------------
/**
 * Provides generic methods and data for all filters.
 * <br>
 * <b>Role:</b> This class is the base class for providing filters. Derived classes include
 * <tt>entity_and_filter</tt>, <tt>entity_binary_filter</tt>, <tt>entity_color_filter</tt>, <tt>entity_not_filter</tt>,
 * <tt>entity_or_filter</tt>, and <tt>entity_test_filter</tt>.
 *<br><br>
 * @see entity_binary_filter, entity_not_filter
 */
 class DECL_KERN entity_filter : public ACIS_OBJECT
{
  protected:
    int use_Count;

	// Delete an entity_filter
    virtual ~entity_filter();

  public:

	// increment_use() and lose() need to be public because derived
	// classes, use them on entity_filters that they point to, instead
	// of on themselves.

	// Increment the use count.  This object will not be destroyed
	// while users still exist.
/**
 * Increments the use count for an entity_filter.
 * <br><br>
 * <b>Role:</b> This object is not destroyed while users still exist.
 */
 	void increment_use() {use_Count++;}

	// Decrement the use count and destroy this object if the use
	// count is zero.
/**
 * Posts a delete bulletin to the bulletin board indicating the instance is no longer used in the active model.
 * <br><br>
 * <b>Role:</b> The lose methods for attached attributes are also called.
 */
 	void lose();

	// Create an entity_filter
/**
 * C++ constructor, creating an entity_filter.
 */
     entity_filter();

	// Return true if the ENTITY passes the test.
/**
 * Tests the entity_filter.
 * <br><br>
 * @param filter
 * entity filter.
 */
     virtual logical test(const ENTITY* filter) const = 0;
};

//----------------------------------------------------------------------
//----------------------------------------------------------------------
// purpose---
//    Base class for entity_filters which are a combination of two other
//    entity_filters
//
//    This class takes over ownership of the entity_filters that it
//    uses, and it will lose them when it is lost.  You should not
//    lose or delete them yourself.
//
// type---
//    Abstract.  This class cannot be instantiated.
//---------------------------------------------------------------------
/**
 * Combines <tt>entity_and_filter</tt> and <tt>entity_or_filter</tt> classes.
 * <br><br>
 * <b>Role:</b> This class is a base class for <tt>entity_filters</tt> that are
 * a combination of two other entity filters.
 *<br><br>
 * @see entity_filter
 */
 class DECL_KERN entity_binary_filter : public entity_filter
{
  protected:
    entity_filter *left_Filter;
    entity_filter *right_Filter;

	// Delete an entity_binary_filter and lose to child filters.
    virtual ~entity_binary_filter();

  public:
	// Create an entity_binary_filter from two entity_filters
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 *<br><br>
 * @param left
 * left entity filter.
 * @param right
 * right entity filter.
 */
     entity_binary_filter(entity_filter *left, entity_filter *right);

	// Return true if the ENTITY passes the test
/**
 * Tests the binary filter, This method returns <tt>TRUE</tt> if the <tt>ENTITY</tt> passes the test.
 * <br><br>
 *@param bin_filter
 * binary filter.
 */
     virtual logical test(const ENTITY* bin_filter) const = 0;
};

//----------------------------------------------------------------------
//----------------------------------------------------------------------
// purpose---
//    Binary filter which returns true if either of its two child
//    filters returns true.
//
//    This class takes over ownership of the entity_filters that it
//    uses, and it will lose them when it is lost.  You should not
//    lose or delete them yourself.
//
// type---
//    Concrete.  This class can be instantiated.
//---------------------------------------------------------------------
/**
 * Returns <tt>TRUE</tt> if either of its two child filters returns <tt>TRUE</tt>.
 * <br><br>
 * <b>Role:</b> This class is a binary filter that returns <tt>TRUE</tt> if
 * either of its two child filters returns <tt>TRUE</tt>.
 */
 class DECL_KERN entity_or_filter : public entity_binary_filter
{
  public:
	// Create an entity_or_filter from two entity_filters
/**
 * C++ constructor, creating an <tt>entity_or_filter</tt> using the specified parameters.
 * <br><br>
 * @param left
 * left entity filter.
 * @param right
 * right entity filter.
 */
     entity_or_filter(entity_filter *left, entity_filter *right);

	// Return true if either the left filter or the right filter returns true.
	// Both filters are always tested.
/**
 * Tests both filters.
 * <br><br>
 * <b>Role:</b> This method returns <tt>TRUE</tt> if either the left filter or the right
 * filter returns <tt>TRUE</tt>; both filters are always tested.
 * <br><br>
 * @param e
 * entity.
 */
   virtual logical test(const ENTITY* e) const;
};

//----------------------------------------------------------------------
//----------------------------------------------------------------------
// purpose---
//    Binary filter which returns true if both of its two child
//    filters return true.
//
//    This class takes over ownership of the entity_filters that it
//    uses, and it will lose them when it is lost.  You should not
//    lose or delete them yourself.
//
// type---
//    Concrete.  This class can be instantiated.
//---------------------------------------------------------------------
/**
 * Returns <tt>TRUE</tt> if both of its two child filters return <tt>TRUE</tt>.
 * <br>
 * <b>Role:</b> This class is a binary filter that returns <tt>TRUE</tt> if both of
 * its two child filters return <tt>TRUE</tt>.
 */
 class DECL_KERN entity_and_filter : public entity_binary_filter
{
  public:
	// Create an entity_and_filter from two entity_filters
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param left
 * left entity filter.
 * @param right
 * right entity filter.
 */
     entity_and_filter(entity_filter* left, entity_filter* right);

	// Return true if both the left and right filters return true.
	// Both filters are always tested.
/**
 * Tests both filters.
 * <br><br>
 * <b>Role:</b> This method returns <tt>TRUE</tt> if both the left and right filters return
 * <tt>TRUE</tt>; both filters are always tested.
 *<br><br>
 * @param e
 * entity.
 */
   virtual logical test(const ENTITY* e) const;
};

//----------------------------------------------------------------------
//----------------------------------------------------------------------
// purpose---
//    Entity filter which returns true if and only if its child filter
//    returns false.
//
//    This class takes over ownership of the entity_filter that it
//    uses, and it will lose it when it is lost.  You should not
//    lose or delete it yourself.
//
// type---
//    Concrete.  This class can be instantiated.
//---------------------------------------------------------------------
/**
 *  Returns <tt>TRUE</tt> if and only if its child filter returns <tt>FALSE</tt>, and vice versa.
 * <br><br>
 * <b>Role:</b> This class is a filter that returns <tt>TRUE</tt> if and only if its child
 * filter returns <tt>FALSE</tt>, and vice versa.
 *<br><br>
 * @see entity_filter
 */
 class DECL_KERN entity_not_filter : public entity_filter
{
  protected:
    entity_filter *the_Filter;

	// Delete an entity_not_filter and lose the child filter.
    virtual ~entity_not_filter();

  public:
	// Create an entity_not_filter from an entity_filter
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 *<br><br>
 * @param filter
 * entity not filter
 */
     entity_not_filter(entity_filter* filter);

	// Return true if and only if the child filter returns false.
/**
 * Tests the child filter, This method returns <tt>TRUE</tt> if and only if the child method returns <tt>FALSE</tt>.
 *<br><br>
 * @param filter
 * child filter.
 */
     virtual logical test(const ENTITY* filter) const;
};

//----------------------------------------------------------------------
//----------------------------------------------------------------------
// purpose---
//    Entity filter which uses a C++ function for a test
//
// usage---
//    The C++ function must take a 'const ENTITY*' and return a 'logical'
//
// type---
//    Concrete.  This class can be instantiated.
//---------------------------------------------------------------------

// type for procedure which tests an ENTITY
/**
 * Procedure which tests an <tt>ENTITY</tt>.
 * <br>
 * <b>Role:</b> Definition type for a procedure which tests an entity.
 */
typedef logical (*entity_test_proc) (const ENTITY*);

//----------------------------------------------------------------------
/**
 * Returns <tt>TRUE</tt> if the given C++ test function returns <tt>TRUE</tt>.
 * <br>
 * <b>Role:</b> This class is a filter that returns <tt>TRUE</tt> if the given C++ test
 * function returns <tt>TRUE</tt>.
 */
 class DECL_KERN entity_test_filter : public entity_filter
{
  protected:
    entity_test_proc the_Test_Proc;
  public:

	// Create an entity_test_filter from a C++ function
/**
 * C++ constructor, creating an <tt>entity_test_filter</tt> using the specified parameters.
 * <br><br>
 * @param proc
 * test procedure.
 */
     entity_test_filter(entity_test_proc proc);

	// Return true if and only if the child function returns true.
/**
 * Tests the child filter, This method returns <tt>TRUE</tt> if and only if the child function returns <tt>TRUE</tt>.
 * <br><br>
 * @param e
 * entity.
 */
     virtual logical test(const ENTITY* e) const;
};

//======================================================================
/** @} */
#endif
