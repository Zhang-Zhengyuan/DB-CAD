/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DSGI_REG_OBJ_H
#define DSGI_REG_OBJ_H
//
// Header for the ADM_giregobj class.
//
// Purpose: The ADM_giregobj class provides a concrete ADM_regobj to the ADM kernel,
// allowing a deformable modeling hierarchy to register with the GI component view-controller.
//
// Description: See the description of the base class ADM_regobj for a discussion of the
// contract between the ADM_regobj and the ACIS Deformable Modeling kernel.
//
// Related classes: The ACIS Deformable Modeling kernel does not directly concstruct or destroy
// ADM_regobj objects; this is done using the methods of the ADM_regobj_factory. The
// ADM_giregobj_factory is a concrete ADM_regobj_factory which constructs ADM_giregobj's.
//
#include "dcl_admgi_control.h"      // DECL_ macros      // DECL_ macros
#include "admregobj.hxx"          // base classs ADM_regobj          // base classs ADM_regobj

/**
* @file admgi_regobj.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ADMGI
 *
 * @{
 */
class deformable_figure;
class ATTRIB_DM2ACIS;
class ADM_giregobj_factory;

/**
 * This class provides a concrete ADM_regobj to the ADM kernel.
 * <b>Role</b>: This class allows a deformable modeling hierarchy to register with the GI component view-controller. See the description of the base class ADM_regobj for a discussion of the contract between the ADM_regobj and the ADM kernel.
 * <b>Related Classes</b>: The ADM kernel does not directly construct or destroy ADM_regobj objects; this is done using the methods of the ADM_regobj_factory. The ADM_giregobj_factory is a concrete ADM_regobj_factory which constructs ADM_giregobjs.
 * @see deformable_figure
 */
class DECL_DMGI_CONTROL ADM_giregobj : public ADM_regobj {

  private:
    deformable_figure* m_fig;
    ATTRIB_DM2ACIS* m_att;

    // Should not call these.
    ADM_giregobj &
    operator=                          // Assignment Operator
    (const ADM_giregobj&);

    ADM_giregobj(const ADM_giregobj& ) ; // Copy constructor

  protected:
    ADM_giregobj(ATTRIB_DM2ACIS* att=0,deformable_figure* fig=0);  //only constructed by the ADM_giregobj_factory
    virtual ~ADM_giregobj();                                       //users should call Lose
  public:
    // tbrv
    /**
    * @nodoc
    */
    friend class ADM_giregobj_factory;                             //method for constructing
    /**
    * Lose method for self-destruction.
    */
    virtual void Lose();                            // public destructor
    /**
    * Called when an owning ATTRIB_DM2ACIS is returned from a bulletin board.
    *@param abs_hurler
    *error handler
    */
    virtual void Activate(                          // called when an owning ATTRIB_DM2ACIS is
                                                    // returned from a bulletin board
                           Spatial_abs_hurler& abs_hurler);    //
    /**
    *Called when an owning ATTRIB_DM2ACIS is placed on a bulletin board.
    *@param abs_hurler
    *error handler
    */
    virtual void Deactivate(                        // called when an owning ATTRIB_DM2ACIS is
                                                    // placed on a bulletin board
                             Spatial_abs_hurler& abs_hurler);
    /**
    *Sets the ADM_regobj pointer on an ATTRIB_DM2ACIS copied for placement on a bulletin board.
    *@param abs_hurler
    *error handler
    */
    virtual ADM_regobj* Make_backup_copy(           // sets the ADM_regobj pointer on an
                                                    // ATTRIB_DM2ACIS copied for placement
                                                    // on a bulletin board
                                          Spatial_abs_hurler& abs_hurler)const;
    /**
    *Access method for Graphic Interaction deformable_figure.
    */
    deformable_figure* Fig() const;                 // access method for GI husk deformable_figure
};
/** @} */
#endif /*DSGI_REG_OBJ_H*/
