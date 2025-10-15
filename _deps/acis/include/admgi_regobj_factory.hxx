/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef ADMGI_REGOBJ_FACTORY_H
#define ADMGI_REGOBJ_FACTORY_H
//
// Header for the ADM_giregobj_factory class.
//
// Purpose: The ADM_giregobj_factory class provides a concrete ADM_regobj_factory to the ADM kernel,
// allowing the creation of concrete ADM_regobj objects - specifically, ADM_giregobj's.
//
// Description: The ACIS Deformable Modeling kernel holds a singleton ADM_regobj_factory
// which it uses to create ADM_regobj objects. The ADM_regobj_factory::Make_regobj method
// constructs an ADM_regobj.
//
// Related classes: The ADM_giregobj_factory is a concrete ADM_regobj_factory
// which constructs ADM_giregobj's.
// See the description of the base class ADM_regobj for a discussion of the
// contract between the ADM_regobj and the ACIS Deformable Modeling kernel.
// The class ADM_regobj_factory_mgr provides the ACIS
// Deformable Modeling kernel with a singleton ADM_regobj_factory object.
//
#include "admregobj_factory.hxx"        // base class ADM_regobj_factory        // base class ADM_regobj_factory
#include "dcl_admgi_control.h"            // DECL_ macros            // DECL_ macros

/**
* @file admgi_regobj_factory.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ADMGI
 *
 * @{
 */

class ATTRIB_DM2ACIS;
class Spatial_abs_hurler;

/**
 *This class provides a concrete ADM_regobj_factory to the ADM kernel, allowing the creation of concrete ADM_regobj objects.
 * <b>Role</b>: The ACIS Deformable Modeling kernel holds a singleton ADM_regobj_factory which it uses to create ADM_regobj objects. The ADM_regobj_factory::Make_regobj method constructs an ADM_regobj.
 * <b>Related Classes</b>: The ADM_giregobj_factory is a concrete ADM_regobj_factory which constructs ADM_giregobjs. See the description of the base class ADM_regobj for a discussion of the contract between the ADM_regobj and the ADM kernel. The class ADM_regobj_factory_mgr provides the ADM kernel with a singleton ADM_regobj_factory object.
 * @see ADM_giregobj
 */
class DECL_DMGI_CONTROL ADM_giregobj_factory : public ADM_regobj_factory {

   protected:

     virtual ~ADM_giregobj_factory() {};                    // use Lose method

   public:

/**
 *Lose method for self-destruction.
 */
    virtual void Lose();                                    // public destructor
/**
 *Creates an ADM_regobj object.
 *<b>Role</b>: ATTRIB_DM2ACIS specifies which attribute will be registered with the view-controller.
 *@param abs_hurler
 *error handler
 *@param attr
 *attribute
 */
    virtual ADM_regobj* Make_regobj(                        // creates an ADM_regobj object
                                     Spatial_abs_hurler& abs_hurler,   // error handler
                                     ATTRIB_DM2ACIS* attr);      // ATTRIB_DM2ACIS which will be
                                                            // registered with the view-controller

};

/** @} */

#endif /*ADMGI_REGOBJ_FACTORY_H*/

