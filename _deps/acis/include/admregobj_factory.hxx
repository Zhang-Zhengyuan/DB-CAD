/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef ADM_REGOBJ_FACTORY_H
#define ADM_REGOBJ_FACTORY_H
//
// Header for the ADM_regobj_factory class
//
// Purpose: The ADM_regobj_factory is used by the ACIS Deformable Modeling
// kernel to construct ADM_regobj objects.
//
// Description: The ACIS Deformable Modeling kernel holds a singleton ADM_regobj_factory
// which it uses to create ADM_regobj objects. The ADM_regobj_factory::Make_regobj method
// constructs an ADM_regobj.
//
// Related classes: The class ADM_regobj_factory_mgr provides the ACIS
// Deformable Modeling kernel with a singleton ADM_regobj_factory object.
// Implementations overriding the ADM_regobj_factory must
// initialize the ADM_regobj_factory_mgr.
// An example implementation of the ADM_regobj_factory is provided by the
// ADM_giregobj_factory class in the admgi_control library.
// The class ADM_regobj_factory_mgr provides the ACIS
// Deformable Modeling kernel with a singleton ADM_regobj_factory object.
//
#include "dcl_adm.h"

/**
* @file admregobj_factory.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISDM
 *
 * @{
 */

class ADM_regobj;
class ATTRIB_DM2ACIS;
class Spatial_abs_hurler;
#include "mmgr.hxx" // class ACIS_OBJECT
/**
 * This is used by the ACIS Deformable Modeling kernel to construct and destruct <tt>ADM_regobj</tt> objects.
 * <br>
 * <b>Role:</b> The <tt>Make_regobj</tt> method constructs an <tt>ADM_regobj</tt> and the <tt>Unmake_regobj</tt>
 * method destructs an <tt>ADM_regobj</tt>.
 * <br><br>
 * <b>Related classes:</b> The class <tt>ADM_regobj_factory_mgr</tt> provides the ACIS
 * Deformable Modeling kernel with a singleton <tt>ADM_regobj_factory</tt> object.
 * Implementations overriding the <tt>ADM_regobj_factory</tt> must initialize the <tt>ADM_regobj_factory_mgr</tt>.
 * <br><br>
 * An example implementation of the <tt>ADM_regobj_factory</tt> is provided by the <tt>ADM_giregobj_factory</tt>
 * class in the <tt>admgi_control</tt> library.
 */
class DECL_ADM ADM_regobj_factory : public ACIS_OBJECT {

  private:

  protected:

    ADM_regobj_factory(){};

    virtual ~ADM_regobj_factory(){};

  public:
/**
 * Lose method for self-destruction.
 */
    virtual void Lose()=0;                                  // public destructor
/**
 * Creates an <tt>ADM_regobj</tt> object.
 * <br><br>
 * @param err_handler
 * error handler.
 * @param owner
 * owner.
 */
    virtual ADM_regobj* Make_regobj(                        // creates an ADM_regobj object
                                     Spatial_abs_hurler& err_handler,   // error handler
                                     ATTRIB_DM2ACIS* owner)=0;    // ATTRIB_DM2ACIS which will be
                                                            // registered with the view-controller


};



 /** @} */
#endif //ADM_REGOBJ_FACTORY_H
