/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//                Local Operations Options
/*******************************************************************/

#if !defined(LOP_OPTIONS_H)
#define LOP_OPTIONS_H
#include "acis.hxx"
#include "lists.hxx"
#include "vlists.hxx"
#include "dcl_lopt.h"
#include "offset_opts.hxx"
class SPACOLLECTION;
class option_header;
class KERN_PROTECTED_LIST;

class FACE;

/*******************************************************************/
/**
 * @file lop_opts.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup LOCALOPS
 *  @{
 */
/**
 * Information about a successful remove operation.
 * The most useful data is a list of the new EDGEs that
 * replaced the FACE.
 */
struct DECL_LOPT remove_face_info : public ACIS_OBJECT
{
    /**
     *   C++ constructor;
     */
	remove_face_info ();
    /**
     *   C++ destructor.
     */
    ~remove_face_info ();
    /**
     * Removed face. Could be deleted, do not touch.
     */  
   	FACE * _face;
    /**
     *  This face is a vertex blend
     */
   	logical _vertex_blend;
    /**
     *  Edges that replaced the face in the model.
     */
   	ENTITY_LIST _entities;

};

/**
 * Information about a partially successful remove operation with fail-safe approach.
 */
struct DECL_LOPT remove_fail_safe_info : public ACIS_OBJECT
{
    /**
     *   C++ constructor;
     */
	remove_fail_safe_info ();
    /**
     *   C++ destructor.
     */
    ~remove_fail_safe_info ();
    /**
     *  Edges (sheet-edges) that were created indicating the existence of open gap after the remove operation.
	 *  Faces that were not removed indicating the partial success of the remove operation.
     */
   	ENTITY_LIST _entities;

};



struct DECL_LOPT lop_fail_safe_info : public ACIS_OBJECT
{
    /**
     *   C++ constructor;
     */
	lop_fail_safe_info ();
    /**
     *   C++ destructor.
     */
    ~lop_fail_safe_info ();
    /**
     *  Edges (sheet-edges) that were created indicating the existence of open gap after the remove operation.
	 *  Faces that were not removed indicating the partial success of the remove operation.
     */
   	ENTITY_LIST _entities;

};
/**
 * Specifies whether vent faces should be added during some local operations.
 * <br>
 * <b>Role:</b> Used by Local Operations to control the behavior of
 * vent face addition. This option is passed to the API functions through
 * @href lop_options .
 * @param vent_type_none
 * Do not add a vent face to fill a gap.
 * @param vent_type_add
 * Add a vent face under specific circumstances to fill a gap.
 * @param vent_type_taper
 * This value has not been implemented and should not be used.
 * @param vent_face_always
 * Always add a vent face to fill a gap.
 **/
enum vent_type_id 
{
    vent_type_none,
	vent_type_add,
	vent_type_taper,
	vent_face_always
};
/**
 * Options for local operations.
 * <br>
 * <b>Role :</b> This class contains information to be used to control
 * the behavior and/or results of Local Operations.
 * <br>
 * This option class is discussed in <i>Local Operations Options</i>. They
 * include :<br><br>
 * <table border=0 cellspacing=0 cellpadding=8 >
 * <tr>
 * <td valign = "top"><tt>repair_self_intersections</tt></td>
 * <td valign = "top">Specifies whether to inspect and
 * repair self-intersections caused by the Local Operation. Default value
 * is <tt>TRUE</tt>.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>simplify</tt></td>
 * <td valign = "top">Specifies whether to simplify the offset spline
 * surfaces to analytic surfaces, if possible. Default value for 
 * this flag is <tt>TRUE</tt>. This option replaces functionality of the global
 * option <tt>lop_simplify</tt>.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>check_remote_face_face_intersections</tt></td>
 * <td valign = "top">Specifies whether to inspect and
 * repair intersections between remote pairs of faces caused by the Local Operation.
 * Meaningful only if the <tt>repair_self_intersections</tt> option is <tt>TRUE</tt>.
 * Default value is <tt>FALSE</tt>. If <tt>repair_self_intersections</tt> is <tt>TRUE</tt> and
 * this option is <tt>FALSE</tt>, only local repair of individual faces is done.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>vent_type</tt></td>
 * <td valign = "top">Sets the behavior for vent face addition. 
 * Local operations may cause two faces that are originally connected through a tangent edge to separate. 
 * The gap between the two faces can be closed in some cases by extending the surfaces of the new faces
 * and intersecting. An alternate approach for closing the gap is to insert a new face which is perpendicular
 * to the two faces. This new face is called a vent face. 
 * Currently, this option only affects operations involving tapering and offsetting; it does not affect move
 * and remove operations. For shelling, this option controls vent face addition only during the offsetting 
 * phase of the operation, but does not control the addition of vent faces around open faces in hollow-body 
 * operations and does not control the addition of vent faces between rim faces in sheet-thicken operations. 
 * There are four possible values of type @href vent_type_id : 
 * <ul>
 * <li><tt>vent_type_none</tt> : Always tries to fill the gap between the faces by extending and intersecting 
 * the faces. </li>
 * <li><tt>vent_type_add</tt> (default): Allows the gap between the faces to be filled by adding vent faces. 
 * The specific circumstances for adding vent faces vary by operation. </li>
 * <li><tt>vent_type_taper</tt> : Not used. </li>
 * <li><tt>vent_face_always</tt> : Always fills the gap by adding a vent face. Currently used only in offsetting. </li>
 * </ul>
 * </td>
 * </tr>
 * </table>
 * In addition, after a Remove Face operation the options object contains information about the Remove Faces operation. 
 * For example, it will contain a list of edges that result from a removing face.
 */
class DECL_LOPT lop_options : public ACIS_OBJECT
{

   /**
    *   Remove face information. For internal use only.
    */
	 KERN_PROTECTED_LIST* _remove_face_info_internal;
   /**
    *   Remove face information.
    */
	 VOID_LIST _remove_face_info;
    /**
    *   Fail-Safe Remove information. For internal use only.
    */
	 KERN_PROTECTED_LIST* _remove_fail_safe_info_internal;
	/**
    *   Remove fail-safe information.
    */
	 VOID_LIST _remove_fail_safe_info;

	 VOID_LIST _lop_fail_safe_info;

	 KERN_PROTECTED_LIST* _lop_fail_safe_info_internal;
	/**
	 *  Check and repair self-intersections.
	 */
	logical _mRepairSelfIntersections;
    /**
	 *  Offset options (currently contains <tt>simplify</tt> option)
	 */
	offset_options _mOffsetOpts;
	/**
	 *  Vent face behavior for some Local Operations.
	 */
	vent_type_id _mVentType;

	/**
	 *  Check and repair intersections between remote face pairs.
	 */
	logical _check_remote_face_face_intersections;
	/**
	 * Internal initialization.
	 */
	void init();

public:

	/**
	* C++ allocation constructor requests memory for this object but does not populate it.
	*/
	lop_options();
	/**
	 * C++ copy constructor.<br><br>
	 * @param rRhs
	 * lop_options to copy.
	 */
	lop_options(const lop_options& rRhs);
	/**
	 * C++ default destructor.
	 */
	~lop_options();
	/**
	 * Gets the status of the <tt>repair_self_intersections</tt> flag.
	 */
	logical get_repair_self_int() const;
    /**
	 * Gets the status of <tt>simplify</tt> flag, which specifies whether the offset
     * spline surface is to be simplified.
	 */
	logical get_simplify() const;
    /**
     *  Gets offsetting options.
     */
    offset_options get_offset_options();

	/**
	 * Gets the status of the <tt>check_remote_face_face_intersections</tt> flag.
	 */
	logical get_check_remote_face_face_intersections() const;
	/**
	 * Sets the <tt>repair_self_intersections</tt> flag to the given value.
	 * <br><br>
	 * @param bVal
	 * New value.
	 */
	void    set_repair_self_int(logical bVal);
    /**
	 * Sets the <tt>simplify</tt> flag, which specifies whether the offset spline surface
     * is to be simplified.
	 * <br><br>
	 * @param bVal
	 * New value.
	 */
	void    set_simplify(ofst_simplify_state bVal);
	/**
	 * Sets the <tt>check_remote_face_face_intersections</tt> to the given value.
	 * <br><br>
	 * @param bVal
	 * New value.
	 */
	void    set_check_remote_face_face_intersections(logical bVal);
	/**
	 * Assignment Copy Operator for <tt>lop_options</tt>.
	 * <br><br>
	 * Note: It does not copy any remove face info data.
	 * @param rRhd
	 * Option Object to be copied/assigned.
	 */
	lop_options& operator=(const lop_options& rRhd);
	/**
	 * Equals Operator.
	 * <br><br>
	 * Note: It does not compare any remove face info data.
	 * @param rRhs
	 * lop_options to be compared.
	 */
	logical operator==(const lop_options& rRhs);

	/**
	 * Non-equals operator.
	 * @param rRhs
	 * lop_options to be compared.
	 */
	logical operator!=(const lop_options& rRhs);
	 /**
	  * Sets the type of vent face addition behavior.
	  * @param ventType
	  * Vent face behavior expected for some local operations.
	  */
	 void set_vent_type(vent_type_id ventType);
	 /**
	  * Gets the type of vent face addition behavior.
	  */
	 vent_type_id get_vent_type();
	/**
	  * Sets values according to the version context.
	  */
	void verify_version();

   /**
    * Returns a list of <tt>remove_face_info</tt> structures contained
    * in a <tt>VOID_LIST</tt>. They contain information about a face 
    * removed in a blend sequence. Each
	* object has a pointer to the face that's been removed and then
	* a list of entities (at this time there should just be one
	* entity in the list). There will be either a vertex or edge
	* in the list of entities.
	*/
	VOID_LIST &get_remove_face_info();

   /**
    * @nodoc
    * Returns a list of remove_face_info structures that contain
	* information about a face removed in a blend sequence. Each
	* object has a pointer to the face that's been removed and then
	* a list of entities (at this time there should just be one
	* entity in the list). There will be either a vertex or edge
	* in the list of entities.
	*/
	KERN_PROTECTED_LIST &get_remove_face_info_internal();

    /**
     * @nodoc
     * Add the remove face information element. Internal book keeping. Do not use.
	 * @param info
	 * Rem face info corresponding to a new face.
	 */
	void add_remove_face_info (SPACOLLECTION * info);
    /**
     * Clear the remove face information.Internal book keeping. Do not use.
     */
	void clear_remove_face_info();

	/**
     * @nodoc
     * Clear the internal remove face information.Internal book keeping. Do not use.
     */
	void clear_remove_face_info_internal();

	/**
     * @nodoc
     * copy the internal list used for tracking to the external list. Internal book keeping. Do not use.
	 */
	void publish_remove_face_info();

	// ============= Fail-Safe Remove Information =========================

	/**
    * Returns a list of <tt>remove_fail_safe_info</tt> structures contained
    * in a <tt>VOID_LIST</tt>. They contain information about entities (edges) that were created 
	* as part of fail-safe approach or the entities ( faces) that were left behind 
	* due to the inability to remove them. 
	*/
	VOID_LIST &get_remove_fail_safe_info();

	/**
    * @nodoc
    * Returns a list of entities ( edges or faces) that were part of the fail-safe remove approach.
	* If exisiting, the edges represent free boundaries of the open-gap that still exists after the operation.
	* If existing, the faces represent the ones that the remove operation was unable to remove.
	*/
	KERN_PROTECTED_LIST &get_remove_fail_safe_info_internal();

	/**
     * @nodoc
     * Clear the internal remove fail-safe information. Internal book keeping. Do not use.
     */
	void clear_remove_fail_safe_info_internal();

	/**
     * @nodoc
     * Add the fail-safe information element from remove-operation. Internal book keeping. Do not use.
	 * @param info
	 * Fail-safe entities
	 */
	void add_remove_fail_safe_info (SPACOLLECTION* info);
    /**
     * Clear the remove fail-safe information.Internal book keeping. Do not use.
     */
	void clear_remove_fail_safe_info();
	/**
     * @nodoc
     * copy the internal list used for tracking to the external list. Internal book keeping. Do not use.
	 */
	void publish_remove_fail_safe_info();

	/**
    * Returns a list of <tt>lop_fail_safe_info</tt> structures contained
    * in a <tt>VOID_LIST</tt>. They may contain only edges or only sheet bodies or both. These entities are created as part of failsafe approach .
	* For more details, refer to the <tt>Failsafe Offsetting </tt> section.
	*/

	VOID_LIST &get_lop_fail_safe_info();

	/**
    * @nodoc
    * Returns a list of entities ( currently sheet bodies) that were part of the fail-safe Offset approach.
	* If existing, the bodies represent the faces that the LOP operation was unable to solve.
	*/

	KERN_PROTECTED_LIST &get_lop_fail_safe_info_internal();
	/**
     * @nodoc
     * Clear the internal lop fail-safe information. Internal book keeping. Do not use.
     */
	void clear_lop_fail_safe_info_internal();
	/**
     * @nodoc
     * Add the fail-safe information element from lop-operation. Internal book keeping. Do not use.
	 * @param info
	 * Fail-safe entities
	 */
	void add_lop_fail_safe_info (SPACOLLECTION* info);
	/**
     * @nodoc
	 * Clear the lop fail-safe information.Internal book keeping. Do not use.
     */
	void clear_lop_fail_safe_info();

	/**
     * @nodoc
     * copy the internal list used for tracking to the external list. Internal book keeping. Do not use.
	 */
	void publish_lop_fail_safe_info();




};

/**
 * @nodoc
 */
#define SET_LOP_OPTIONS(pLopts) \
    lop_options dummy; if (!pLopts) pLopts = &dummy; pLopts->verify_version();
/*! @} */
#endif
