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
//    define entity data type in scheme
//
//----------------------------------------------------------------------

#ifndef ent_typ_hxx
#define ent_typ_hxx

//======================================================================
#ifndef _EXPORT_MACRO
#define _EXPORT_MACRO
#endif

#ifndef SKIP_FORWARD_DECLARATIONS
#include "scheme.hxx"

/**
* @file ent_typ.hxx
* @CAA2Level L1
* @CAA2Usage U1
* \addtogroup SCMAPI
* \Declared at <ent_typ.hxx>
* @{
*/
class ENTITY;
class BODY;
class FACE;
class LOOP;
class EDGE;
class ENTITY_LIST;
class WIRE;
class COEDGE;
class TEDGE;
class TCOEDGE;
class VERTEX;
class TVERTEX;
class LAW;
class APATTERN;
class entity_handle;
class entity_handle_list;
#include "efilter.hxx"
#endif

/**
* Determines if a Scheme object is an <tt>ENTITY</tt>.
* <br><br>
* <b>Role:</b> This function is overloaded and can be used in three ways.
* <br><br>
* Determines if a Scheme object is an <tt>ENTITY</tt>.
* <br>
* Determines if a Scheme object is an <tt>ENTITY</tt> of a type which passes a filter test.
* <br>
* Determines if a Scheme object is an <tt>ENTITY</tt> which also passes an additional test.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param e
* Scheme object.
**/
_EXPORT_MACRO logical is_Scm_Entity(ScmObject e);
/**
* Determines if a Scheme object is an <tt>ENTITY</tt>.
* <br><br>
* <b>Role:</b> This function is overloaded and can be used in three ways.
* <br><br>
* Determines if a Scheme object is an <tt>ENTITY</tt>.
* <br>
* Determines if a Scheme object is an <tt>ENTITY</tt> of a type which passes a filter test.
* <br>
* Determines if a Scheme object is an <tt>ENTITY</tt> which also passes an additional test.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param o
* Scheme object.
* @param filt
* filter.
**/
_EXPORT_MACRO logical is_Scm_Entity(ScmObject o, const entity_filter &filt);

/**
* Determines if a Scheme object is an <tt>ENTITY</tt>.
*<br><br>
* <b>Role:</b> This function is overloaded and can be used in three ways.
* <br><br>
* Determines if a Scheme object is an <tt>ENTITY</tt>.
* <br>
* Determines if a Scheme object is an <tt>ENTITY</tt> of a type which passes a filter test.
* <br>
* Determines if a Scheme object is an <tt>ENTITY</tt> which also passes an additional test.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param o
* Scheme object.
* @param test
* test procedure.
**/
_EXPORT_MACRO logical is_Scm_Entity(ScmObject o, entity_test_proc test);
/**
 * Determines if a Scheme object is an ENTITY_LIST
 * @nodoc
 */
_EXPORT_MACRO logical is_Scm_Entity_List(ScmObject);
/**
* Creates a C++ <tt>ENTITY</tt> from a Scheme entity.
* <br><br>
* <b>Role:</b> type is a character string describing type of ENTITY filter is
* looking for, such as <tt>"BODY"</tt>, and is used for printing error messages.
* <br><br>
* This function is overloaded:
* <br><br>
* Creates an <tt>ENTITY</tt> from a Scheme entity.
* <br>
* Creates an <tt>ENTITY</tt> from a Scheme object given an addition constraint in the form
* of an <tt>entity_filter</tt>.
* <br>
* Creates an <tt>ENTITY</tt> from a Scheme object given an addition constraint in the form
* of an entity test procedure.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param e
* Scheme object.
**/
_EXPORT_MACRO ENTITY* get_Scm_Entity(ScmObject e);
/**
* Creates a C++ <tt>ENTITY</tt> from a Scheme entity.
* <br><br>
* <b>Role:</b> type is a character string describing type of <tt>ENTITY</tt> filter is
* looking for, such as <tt>"BODY"</tt>, and is used for printing error messages.
* <br><br>
* This function is overloaded:
* <br><br>
* Creates an <tt>ENTITY</tt> from a Scheme entity.
* <br>
* Creates an <tt>ENTITY</tt> from a Scheme object given an addition constraint in the form
* of an <tt>entity_filter</tt>.
* <br>
* Creates an <tt>ENTITY</tt> from a Scheme object given an addition constraint in the form
* of an entity test procedure.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param o
* Scheme object.
* @param filt
* filter test.
* @param type
* type.
**/

_EXPORT_MACRO ENTITY* get_Scm_Entity(ScmObject o, const entity_filter& filt, const char* type);

/**
* Creates a C++ <tt>ENTITY</tt> from a Scheme entity.
* <br><br>
* <b>Role:</b> type is a character string describing type of <tt>ENTITY</tt> filter is
* looking for, such as <tt>"BODY"</tt>, and is used for printing error messages.
* <br><br>
* This function is overloaded:
* <br><br>
* Creates an <tt>ENTITY</tt> from a Scheme entity.
* <br>
* Creates an <tt>ENTITY</tt> from a Scheme object given an addition constraint in the form
* of an <tt>entity_filter</tt>.
* <br>
* Creates an <tt>ENTITY</tt> from a Scheme object given an addition constraint in the form
* of an entity test procedure.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param o
* Scheme object.
* @param test
* entity test.
* @param type
* type.
**/

_EXPORT_MACRO ENTITY* get_Scm_Entity(ScmObject o, entity_test_proc test, const char* type);
/**
 * Creates a C++ entity_handle from a Scheme entity.
* <br><br>
* <b>Role:</b> Retrieves the entity_handle associated with this ENTITY, as long as
* the ENTITY does exist in a PART and the PART is associated with an asm_model.
* <br><br>
* @param o
* Scheme object.
 */
_EXPORT_MACRO entity_handle* get_Scm_Entity_Handle(ScmObject o);
/**
 * Creates a C++ ENTITY_LIST from a Scheme list
 */
_EXPORT_MACRO void get_Scm_Entity_List(ScmObject, ENTITY_LIST&);
/**
 * @nodoc
 * Creates a C++ ENTITY_LIST from a Scheme Entity List
 */
_EXPORT_MACRO void get_Scm_Entity_List(int, ScmObject*, ENTITY_LIST&);
/**
 * @nodoc
 */

_EXPORT_MACRO void get_Scm_Entity_List(ScmObject, const entity_filter&,
						 const char*, ENTITY_LIST&);
/**
 * @nodoc
 */
_EXPORT_MACRO void get_Scm_Entity_List(int, ScmObject*, const entity_filter&, const char*, ENTITY_LIST&);
/**
 * @nodoc
 */
_EXPORT_MACRO void get_Scm_Entity_List(ScmObject, entity_test_proc, const char*, ENTITY_LIST&);
/**
 * @nodoc
 */
_EXPORT_MACRO void get_Scm_Entity_List(int, ScmObject*, entity_test_proc, const char*, ENTITY_LIST&);
/**
 * Creates a C++ ENTITY array from a Scheme Entity List, returns number
 * of elements
 * @nodoc
 */
_EXPORT_MACRO int get_Scm_Entity_Array(ScmObject, ENTITY**&);
/**
 * @nodoc
 */
_EXPORT_MACRO int get_Scm_Entity_Array(int, ScmObject*, ENTITY**&);

/**
* Creates a Scheme entity from a C++ <tt>ENTITY</tt>.
* <br><br>
* @param ent
* entity.
**/
_EXPORT_MACRO ScmObject make_Scm_Entity(ENTITY *ent );

/**
* Creates a Scheme entity from a C++ <tt>ENTITY</tt>.
* <br><br>
* @param ent
* entity.
* @param owner
* owner entity.
**/
_EXPORT_MACRO ScmObject make_Scm_Entity(ENTITY *ent, ENTITY * owner );

/**
* Creates a Scheme entity from an entity handle.
* <br><br>
* <b>Role:</b> This routine ensures that any side effects from making the
* Scheme object are registered in the proper history stream.
* <br><br>
* @param ent_hdl
* entity handle.
**/
_EXPORT_MACRO ScmObject make_Scm_Entity(const entity_handle* ent_hdl);

/**
* Creates a list of Scheme entities from an <tt>ENTITY_LIST</tt> object.
* <br><br>
* @param elist
* entity list.
**/
_EXPORT_MACRO ScmObject make_Scm_Entity_List(const ENTITY_LIST& elist);


/**
* Creates a list of Scheme entities from an <tt>ENTITY_LIST</tt> object.
* <br><br>
* @param elist
* entity list.
* @param owner_func
* function to fetch owner.
* @param data
* user data.
**/
_EXPORT_MACRO ScmObject make_Scm_Entity_List(const ENTITY_LIST& elist,
    ENTITY * (*fetch_owner)( ENTITY * ent, void * data ), void * data );

/**
* Creates a list of Scheme entities from an ENTITY array.
**/
_EXPORT_MACRO ScmObject make_Scm_Entity_List(int num, ENTITY* elist[]);

/**
* Creates a list of Scheme entities from an <tt>entity_handle_list</tt>.
* <br><br>
* <b>Role:</b> This routine ensures that any side effects from making the
* Scheme objects are registered in the proper history stream.
* <br><br>
* <b>Limitation:</b> All entity handles must be from the same model.
* <br><br>
* @param ent_hdls
* entity handle list.
**/
_EXPORT_MACRO ScmObject make_Scm_Entity_List(const entity_handle_list& ent_hdls);
/**
 * @nodoc
 */
_EXPORT_MACRO void call_entity_creation_hook(ENTITY*);
/**
 * Get ACIS BODY from scheme object
 */
_EXPORT_MACRO BODY* get_Scm_Body(ScmObject);
/**
 * Get ACIS BODY from scheme object
 */
_EXPORT_MACRO BODY* get_Scm_Solid(ScmObject);
/**
 * @nodoc
 */
_EXPORT_MACRO BODY* get_Scm_Mixed_Body(ScmObject);
/**
 * Get ACIS Wire BODY from scheme object (must not have faces)
 */
_EXPORT_MACRO BODY* get_Scm_Wire_Body(ScmObject);
/**
 * Get ACIS FACE from scheme object
 */
_EXPORT_MACRO FACE* get_Scm_Face(ScmObject);
/**
 * Get ACIS LOOP from scheme object
 */
_EXPORT_MACRO LOOP* get_Scm_Loop(ScmObject);
/**
 * Get ACIS EDGE from scheme object
 */
_EXPORT_MACRO EDGE* get_Scm_Edge(ScmObject);
/**
 * Get ACIS WIRE from scheme object
 */
_EXPORT_MACRO WIRE* get_Scm_Wire(ScmObject);
/**
 * Get ACIS COEDGE from scheme object
 */
_EXPORT_MACRO COEDGE *get_Scm_Coedge(ScmObject);
/**
 * Get ACIS TEDGE from scheme object
 */
_EXPORT_MACRO TEDGE *get_Scm_Tedge(ScmObject);
/**
 * Get ACIS TCOEDGE from scheme object
 */
_EXPORT_MACRO TCOEDGE *get_Scm_Tcoedge(ScmObject);
/**
 * Get ACIS LAW from scheme object
 */
_EXPORT_MACRO LAW*  get_Scm_LAW(ScmObject);
/**
 * Get ACIS VERTEX from scheme object
 */
_EXPORT_MACRO VERTEX*  get_Scm_Vertex(ScmObject);
/**
 * Get ACIS TVERTEX from scheme object
 */
_EXPORT_MACRO TVERTEX*  get_Scm_Tvertex(ScmObject);
/**
 * Get ACIS PATTERN from scheme object
 */
_EXPORT_MACRO APATTERN*  get_Scm_APATTERN(ScmObject);

/** @} */

_EXPORT_MACRO void get_face_type_str(FACE *face, char *face_type_string);

_EXPORT_MACRO void get_loop_type_str (LOOP *loop, char *str_return );

_EXPORT_MACRO void angle_between_edges(EDGE* edge1, EDGE* edge2, double &angle1, double &angle2, logical output_degrees);

#endif

