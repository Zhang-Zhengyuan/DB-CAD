/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DSHOOPS_REG_OBJ_H
#define DSHOOPS_REG_OBJ_H
//
// Header for the ADM_hoopsregobj class.
//
// Purpose: The ADM_hoopsregobj class provides a concrete ADM_regobj to the ADM kernel,
// allowing a deformable modeling hierarchy to register with the GI component view-controller.
//
// Description: See the description of the base class ADM_regobj for a discussion of the
// contract between the ADM_regobj and the ACIS Deformable Modeling kernel.
//
// Related classes: The ACIS Deformable Modeling kernel does not directly concstruct or destroy
// ADM_regobj objects; this is done using the methods of the ADM_regobj_factory. The
// ADM_hoopsregobj_factory is a concrete ADM_regobj_factory which constructs ADM_hoopsregobj's.
//
#include "dcl_admhoops.h"      // DECL_ macros      // DECL_ macros
#include "admregobj.hxx"          // base classs ADM_regobj          // base classs ADM_regobj

#include "hc.h"
#include "ha_bridge.h"

/**
* @file admhoops_regobj.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ADMHOOPS
 *
 * @{
 */
class ATTRIB_DM2ACIS;
class ADM_hoopsregobj_factory;

/**
 * This class provides a concrete <tt>ADM_regobj</tt> to the ADM kernel.
 * <br>
 * <b>Role:</b> This class allows a deformable modeling hierarchy to register with the ha_bridge component.
 * See the description of the base class <tt>ADM_regobj</tt> for a discussion of the contract
 * between the <tt>ADM_regobj</tt> and the ADM kernel.
 * <br><br>
 * <b>Related Classes:</b> The ADM kernel does not directly construct or destroy <tt>ADM_regobj</tt> objects;
 * this is done using the methods of the <tt>ADM_regobj_factory</tt>. The <tt>ADM_hoopsregobj_factory</tt> is a
 * concrete <tt>ADM_regobj_factory</tt> which constructs <tt>ADM_hoopsregobjs</tt>.
 */
class DECL_ADMHOOPS ADM_hoopsregobj : public ADM_regobj
{
  private:
    ATTRIB_DM2ACIS *m_ATTRIB_DM2ACIS;
		int is_active;
	HC_KEY  m_HistoryKey;
	ENTITY *m_TL_ATTRIB_Owner;

	//HC_KEY m_Key;

    // Should not call these.
    ADM_hoopsregobj &
    operator=                          // Assignment Operator
    (const ADM_hoopsregobj&);

    ADM_hoopsregobj(const ADM_hoopsregobj& ) ; // Copy constructor

  protected:
    ADM_hoopsregobj(ATTRIB_DM2ACIS* att=0);  //only constructed by the ADM_hoopsregobj_factory
    virtual ~ADM_hoopsregobj();                                       //users should call Lose
  public:
    // tbrv
    /**
    * @nodoc
    */
    friend class ADM_hoopsregobj_factory;                             //method for constructing
    /**
    * Lose method for self-destruction.
    */
    virtual void Lose();                            // public destructor
    /**
    * Called when an owning <tt>ATTRIB_DM2ACIS</tt> is returned from a bulletin board.
	* <br><br>
    * @param abs_hurler
    * error handler.
    */
    virtual void Activate(                          // called when an owning ATTRIB_DM2ACIS is
                                                    // returned from a bulletin board
                           Spatial_abs_hurler& abs_hurler);    //
    /**
    *Called when an owning <tt>ATTRIB_DM2ACIS</tt> is placed on a bulletin board.
	* <br><br>
    *@param abs_hurler
    *error handler.
    */
    virtual void Deactivate(                        // called when an owning ATTRIB_DM2ACIS is
                                                    // placed on a bulletin board
                             Spatial_abs_hurler& abs_hurler);
    /**
    *Sets the <tt>ADM_regobj</tt> pointer on an <tt>ATTRIB_DM2ACIS</tt> copied for placement on a bulletin board.
	* <br><br>
    *@param abs_hurler
    *error handler.
    */
    virtual ADM_regobj* Make_backup_copy(           // sets the ADM_regobj pointer on an
                                                    // ATTRIB_DM2ACIS copied for placement
                                                    // on a bulletin board
                                          Spatial_abs_hurler& abs_hurler)const;

    /**
    *Gets the HOOPS key for the associated <tt>ATTRIB_DM2ACIS</tt>.
	* <br><br>
    *@param abs_hurler
    *error handler.
    */
	virtual HC_KEY GetHOOPSKey();
};
/** @} */
#endif /*DSHOOPS_REG_OBJ_H*/
