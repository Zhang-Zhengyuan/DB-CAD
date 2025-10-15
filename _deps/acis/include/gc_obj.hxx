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
//    Define a virtual class from which objects which have pointers
//    to objects which are allocated off the heap can be derived.
//
// design---
//    There are come scheme objects which contain pointers to data which
//    is allocated from the heap.  When the scheme garbage collector
//    removes the scheme object, we need to also free the memory which
//    was allocated from the heap.  GC_Object is a virtual class from
//    which objects of this type can be derived.
//
//    When you create an instance of a GC_Object, it is added to an
//    array of GC_Objects which are active.  After scheme does a
//    garbage collection, it goes through this array and deletes any
//    any objects in it which are no longer referenced by another scheme
//    object.
//
// example---
//    For a source code example of derivation from GC_Object see
//    scmext/surf_typ.cxx.
//
//----------------------------------------------------------------------

#ifndef gc_obj_hxx
#define gc_obj_hxx

#include "scheme.hxx"
#include "logical.h"

/**
 * @file gc_obj.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup SCMAPI
 * \Declared at <gc_obj.hxx>
 * @{
 */

/**
 * Virtual class from which objects which have pointers
 * to objects which are allocated off the heap can be derived.
 */
class GC_Object: public ACIS_OBJECT
{
  private:
    ScmObject The_Scheme_Object;

  public:

   /**
	* Create a GC_Object.  When a GC_Object is created, it is
	* automatically added to the array of GC_Objects
    */
    GC_Object(ScmObject);
   /**
	*    Delete a GC_Object.
	*    The destructor for GC_Object is virtual.  Each implementation
	*    must define their own which deletes the data which it points to.
    */
    virtual ~GC_Object();
   /**
	*    Update the scheme object in a GC_Object after a garbage collection.
	*    Returns TRUE if the object is still active, or FALSE if it should
	*    be deleted.
    */
    logical update();
};
/**
 * Deletes the <tt>GC_Object</tt>.
 * <br><br>
 * <b>Effect:</b> System routine
 * This procedure deletes all of the GC_Objects that the system knows
 * about.  It does not do any tests to see if they are still in use.
 * It is intended to be called when you are exitting the application
 * and want to free up any memory that was allocated.
 **/
void delete_GC_Objects();
/** @} */
#endif
