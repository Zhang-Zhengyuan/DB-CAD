/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef DM_TAG_ARRAY_H
#define DM_TAG_ARRAY_H
// Header for the DM_tag_array class.
//
// Purpose: The DM_tag_array class is a read-only container class for Deformable Modeling tags,
// supporting tag query methods and icon broadcast methods.
//
// Description: Deformable Modeling tag query methods return a DM_tag_array populated with tag
// object tags. Thus, clients needn't allocate or destroy heap memory. Queries on a single
// DS_dmod or a DS_dmod hierarchy are supported. The returned DM_tag_array can be passed as-is to
// the Deformable Modeling icon broadcast draw and set_state methods; the compiler will
// automatically use the member const int* cast method.
//
// Related methods: A DM_tag_array object is returned by the DM_get_tags, DM_get_dmod_tags,
// ATTRIB_DM2ACIS::Get_tags, and ATTRIB_DM2ACIS::Get_dmod_tags methods. A DM_tag_array can
// be passed in place of a const int* to the icon broadcast methods DM_draw_icon,
// DM_draw_dmod_icons, ATTRIB_DM2ACIS::Draw_icon, ATTRIB_DM2ACIS::Draw_dmod_icons, and
// the corresponding XXX_setstate_icon, XXX_setstate_dmod_icons methods.
//
#include "dsblock.hxx"    // class DS_int_block    // class DS_int_block

#include "dcl_ds.h"

/**
* @file dm_tag_array.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup DMAPI
 *
 * @{
 */

/**
 * The <tt>DM_tag_array</tt> class supports tag query methods and icon broadcast methods.
 * <br>
 * <b>Role:</b> This is a read-only container class for deformable modeling tags.
 * Deformable modeling tag query methods return a <tt>DM_tag_array</tt> populated with tag object tags.
 * Thus, clients needn't allocate or destroy heap memory. Queries on a single <tt>DS_dmod</tt> or
 * a <tt>DS_dmod</tt> hierarchy are supported. The returned <tt>DM_tag_array</tt> can be passed as-is to the
 * deformable modeling icon broadcast <tt>draw</tt> and <tt>set_state</tt> methods; the compiler will automatically
 * use the provided <tt>const int* const cast</tt> method.
 * <br><br>
 * <b>Related methods</b><br>
 * A <tt>DM_tag_array</tt> object is returned by the @href DM_get_tags, @href DM_get_dmod_tags,
 * <tt>ATTRIB_DM2ACIS::Get_tags</tt>, and <tt>ATTRIB_DM2ACIS::Get_dmod_tags</tt> methods. A
 * <tt>DM_tag_array</tt> can be passed in place of a const int* to the icon broadcast methods
 * <tt>DM_draw_icon</tt>, <tt>DM_draw_dmod_icons</tt>, <tt>ATTRIB_DM2ACIS::Draw_icon</tt>,
 * <tt>ATTRIB_DM2ACIS::Draw_dmod_icons</tt>, and the corresponding <tt>XXX_setstate_icon</tt>,
 * <tt>XXX_setstate_dmod_icons</tt> methods.
 * @see DM_get_tags, DM_get_dmod_tags, DM_draw_dmod_icons
 */
 class DECL_DM DM_tag_array {
  private:
    friend class DM_tag_array_initializer;
    DS_int_block m_block;
// Users should not call assignment operator or copy constructor
    DM_tag_array&
    operator=                              // assignment Operator
    (const DM_tag_array&);
    DM_tag_array(const DM_tag_array&);     // copy constructor
  public:
/**
 * Public constructor.
 */
     DM_tag_array();                        // public constructor
/**
 * Public destructor.
 */
    ~DM_tag_array();                       // public destructor
/**
 * Used for data access.
 */
    int operator[](int) const;             // data access
/**
 * Number of array elements.
 */

    int Size()const;                       // number of array elements
/**
 * Read-only cast operator.
 */

    operator const int*()const;      // read-only cast operator
};

/** @} */


#endif //DM_TAG_ARRAY_H
