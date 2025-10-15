/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef ADM_REG_OBJ_H
#define ADM_REG_OBJ_H
// Header for the ADM_regobj class.
//
// Purpose: The ADM_regobj interface class provides abstract registration
// methods to the DM kernel, allowing a deformable modeling hierarchy to
// register with the view-controller.
//
// Description: The ADM_regobj contract with the ADM kernel has 3 parts.
// First, the DM hierarchy, encapsulated by the ATTRIB_DM2ACIS, owns the
// ADM_regobj object. Registration of the ATTRIB_DM2ACIS with the view-controller
// happens at creation, and deregistration happens at destruction. Registration
// typically involves passing callback, and other model data to the view-controller.
// Second, the ADM_regobj supports ACIS roll by its Activate & Deactivate methods.
// When a copy of an ATTRIB_DM2ACIS is placed on a bulletin board, its
// ADM_regobj's Deacactivate method is called so it is no longer drawn, and similarly
// for the ADM_regobj's Activate method, when an ATTRIB_DM2ACIS is restored from a
// bulletin board.
// Third, the ADM_regobj supports ACIS roll by its Make_backup_copy method. When a copy
// of an ATTRIB_DM2ACIS is placed on a bulletin board, the copy's ADM_regobj pointer is
// set to the value returned by the original ATTRIB_DM2ACIS's ADM_regobj::Make_backup_copy
// method. This allows implementations to e.g., store draw registration data through
// the ACIS roll.
//
// Related classes: The ACIS Deformable Modeling kernel does not directly construct
// ADM_regobj objects; this is done using the methods of the ADM_regobj_factory.
// Implementations overriding the ADM_regobj must also override the ADM_regobj_factory.
// An example implementation of the ADM_regobj is provided in the ADM_giregobj classe
// in the admgi_control library.
//
#include "dcl_adm.h"                      // DECL_ macros                      // DECL_ macros
#include "mmgr.hxx"
/**
* @file admregobj.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISDM
 *
 * @{
 */

class Spatial_abs_hurler;

/**
 * Provides abstract registration methods to the kernel, allowing a deformable modeling hierarchy to register with the view controller.
 * <br><br>
 * <b>Role: </b>The ADM_regobj contract with the ADM kernel has three parts.
 * First, the deformable modeling hierarchy, encapsulated by <tt>ATTRIB_DM2ACIS</tt>, owns
 * the <tt>ADM_regobj</tt> object. Registration of the <tt>ATTRIB_DM2ACIS</tt> with the view
 * controller happens at creation, and deregistration happens at destruction.
 * Registration typically involves passing callback, and other model data to the
 * view controller.
 * <br><br>
 * Second, the <tt>ADM_regobj</tt> supports ACIS roll by its Activate and Deactivate methods.
 * When a copy of an <tt>ATTRIB_DM2ACIS</tt> is placed on a bulletin board, its <tt>ADM_regobj</tt>'s
 * Deactivate method is called so it is no longer drawn, and similarly for the
 * <tt>ADM_regobj</tt>'s Activate method, when an <tt>ATTRIB_DM2ACIS</tt> is restored from a bulletin board.
 * <br><br>
 * Third, the <tt>ADM_regobj</tt> supports ACIS roll by its <tt>Make_backup_copy</tt> method.
 * When a copy of an <tt>ATTRIB_DM2ACIS</tt> is placed on a bulletin board, the copy's
 * <tt>ADM_regobj</tt> pointer is set to the value returned by the original <tt>ATTRIB_DM2ACIS</tt>'
 * <tt>ADM_regobj::Make_backup_copy</tt> method. This allows implementations to store draw
 * registration data through the ACIS roll, for example.
 * <br><br>
 * <b>Related classes:</b> The ACIS Deformable Modeling kernel does not directly
 * construct or destroy <tt>ADM_regobj</tt> objects; creation is done using the <tt>ADM_regobj_factory</tt> <tt>Make_regobj</tt> method
 * and destruction is done using the <tt>ADM_regobj</tt> <tt>Lose</tt> method. Implementations overriding the <tt>ADM_regobj</tt> must also override
 * the <tt>ADM_regobj_factory</tt>. An example implementation of the <tt>ADM_regobj</tt> is provided
 * in the <tt>ADM_giregobj</tt> class in the <tt>admgi_control</tt> library.
 */
class DECL_ADM ADM_regobj : public ACIS_OBJECT {
  private:
// These will not be called.
    ADM_regobj &
    operator=                             // assignment Operator
    (const ADM_regobj&);

    ADM_regobj(const ADM_regobj& );       // copy constructor

  protected:
/**
 * Clients should not call constructor - should only call the <tt>ADM_regobj_factory</tt> <tt>Make_regobj</tt> method
 */
    ADM_regobj() {}
/**
 * Clients should not call DTOR - should only call the <tt>ADM_regobj</tt> <tt>Lose</tt> method
 */
    virtual ~ADM_regobj(){}

  public:
/**
 * Lose method for self-destruction - destructor is protected.
 */
    virtual void Lose()=0;                          // public destructor
/**
 * This is called when an owning <tt>ATTRIB_DM2ACIS</tt> is returned from a bulletin board.
 * <br><br>
 * @param err_handler
 * error handler.
 */
    virtual void Activate(                          // called when an owning ATTRIB_DM2ACIS is
                                                    // returned from a bulletin board
                           Spatial_abs_hurler& err_handler)=0;  //
/**
 * This is called when an owning <tt>ATTRIB_DM2ACIS</tt> is placed on a bulletin board.
 * <br><br>
 * @param err_handler
 * error handler.
 */
    virtual void Deactivate(                        // called when an owning ATTRIB_DM2ACIS is
                                                    // placed on a bulletin board
                             Spatial_abs_hurler& err_handler)=0;
/**
 * Sets the <tt>ADM_regobj</tt> pointer on an <tt>ATTRIB_DM2ACIS</tt> copied for placement on a bulletin board.
 * <br><br>
 * @param err_handler
 * error handler.
 */
    virtual ADM_regobj* Make_backup_copy(           // create an ADM_regobj object suitable
                                                    // for backing up an ATTRIB_DM2ACIS
                                                    // to a bulletin board; e.g, can store state data
                                                    // to be restored by roll
                                          Spatial_abs_hurler& err_handler)const=0;
};

 /** @} */
#endif //ADM_REG_OBJ_H
