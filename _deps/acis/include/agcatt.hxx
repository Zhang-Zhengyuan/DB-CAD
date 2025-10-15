/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : agcatt.hxx
// created  : Apr 5, 1995
// author   : GWCelniker
// synopsis : The AGC attribute class
// 26-Apr-99 erik: Removed obsolete is_<class> documentation template.

// Attribute declaration for an organization class attribute. Each
// application developer receives one of these customised for himself.
// All attributes specific to the application developer are then made
// derived classes of this attribute, ensuring that different
// developers can assign identifiers independently without mutual
// interference.

#ifndef DS_ATTRIB_H
#define DS_ATTRIB_H

#include "attrib.hxx"
#include "dcl_adm.h"            // DECL_ADM            // DECL_ADM

// This attribute type is a derived class of ATTRIB which
//      is derived from ENTITY
// Note:  Naming restrictions.
//        The macro MASTER_ATTRIB_DECL is used to
//        concatenate "ATTRIB_AGC" with "_LEVEL" and "_TYPE".
//        So the extern int has to be named "ATTRIB_AGC_TYPE"
//        and the constant "ATTRIB_AGC_LEVEL"
/**
* @file agcatt.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISDM
 *
 * @{
 */

extern DECL_ADM int ATTRIB_AGC_TYPE ;
#define ATTRIB_AGC_LEVEL (ATTRIB_LEVEL + 1)

// MASTER_ATTRIB_DECL( ATTRIB_AGC )
// GWC change: expanded MASTER_ATTRIB_DECL macro by hand so
//             that virtual Make_copy() function could be added
/**
 * Designates to whom the attribute belongs.
 * <br>
 * <b>Role:</b> Attribute declaration for an organization class attribute.
 * Each application developer receives one of these customized for himself.
 * All attributes specific to the application developer are then made derived
 * classes of this attribute, ensuring that different developers can assign
 * identifiers independently without mutual interference.
 */
class DECL_ADM ATTRIB_AGC : public ATTRIB {
public:
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded
 * <tt>new</tt> operator inherited from the <tt>ENTITY</tt> class (for example, <tt>x=new ATTRIB_AGC(...)</tt>),
 * because this reserves the memory on the heap, a requirement to support roll
 * back and history management.
 * <br><br>
 * @param ent
 * entity to get attribute.
 */
 ATTRIB_AGC(ENTITY *ent = NULL) ;
/**
 * Copies the attribute from the owning entity for use with another entity.
 * <br><br>
 * <b>Role:</b> Returns newly made attribute copy.
 * <br><br>
 * @param owner
 * owning entity.
 */
  virtual ATTRIB_AGC*      // rtn: newly made attribute copy
   Make_copy               // eff: Virtual ATTRIB_AGC copy function
    (ENTITY *owner)        { return( ACIS_NEW ATTRIB_AGC(owner) ) ; }
/**
 * Implements the assignment operator.
 * <br><br>
 * @param agc
 * object with values to copy.
 */
 ATTRIB_AGC &operator=     // eff: assignment operator
   (const ATTRIB_AGC &agc) // in : object with values to copy
                           { // no work condition
                             if(&agc != this) {
                            ((ATTRIB *)this)->operator=(agc) ;
                             }
                             return *this ;
                           }

 /**
  * @nodoc
  */
 ATTRIB_FUNCTIONS(ATTRIB_AGC, NONE)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

} ;

 /** @} */
#endif // end DS_ATTRIB_H
