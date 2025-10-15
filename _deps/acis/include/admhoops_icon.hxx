/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DM_HOOPS_ICON_H
#define DM_HOOPS_ICON_H
//
// Header for the DM_HOOPS_icon class.
//
// Purpose: The DM_HOOPS_icon class is used for drawing DM tag objects.
// The DM_HOOPS_icon class provides an example of a common 
// command/query interface base class, which derives from the DM_icon 
// pure interface class. 
//
// Description: The dmicon library provides concrete examples of icons
// for all DM tag objects. The icons in the dmicon libarary derive from 
// the DM_HOOPS_icon base class.
//
// Related classes: Examples of concrete DM_HOOPS_icon objects are found
// in the dmicon library. 
// A DM tag object only creates a concrete icon via the DM_icon_factory. 
// Many DM_HOOPS_icon methods are encapsulated in either
// DM_def_cmd_args, or DM_def_query_args, e.g., Set_icon_width or Get_icon_width.
// Base class (DM_icon) methods take a Spatial_abs_hurler for error handling.

//-----------------------------------------------------------//
// HOOPS-specific Header File(s)
//-----------------------------------------------------------//
#include <hc.h>
#include "ha_bridge.h"

#include "dmicon.hxx"     // base class DM_icon     // base class DM_icon
#include "dcl_admhoops.h"            // DECL macros            // DECL macros

#include "ds2acis.hxx"
#include "admhoops_draw_args.hxx"
#include "transfrm.hxx"  // class SPAtransf

class Spatial_abs_hurler;
class DM_dbl_array;

class DECL_ADMHOOPS DM_HOOPS_icon : public DM_icon {

  private:
// Users should not call assignment operator or copy constructor - use Make_copy
    DM_HOOPS_icon&operator=(const DM_HOOPS_icon&);  // Assignment Operator

  protected:

    DM_HOOPS_icon(const DM_HOOPS_icon&) ; // Copy constructor

// Users should not create these!
    DM_HOOPS_icon();
// Call Lose to delete.
    virtual ~DM_HOOPS_icon();

	DM_icon *m_DM_icon; // Original DM_icon that was in the icon factory.
	ENTITY  *m_TL_ATTRIB_Owner;
	HC_KEY  m_HistoryKey;
	SPAtransf m_BodyTransf;
	ATTRIB_DM2ACIS *m_ATTRIB_DM2ACIS;
	char    m_SegString[50];
//	HC_KEY m_Key;

  public:
	DM_HOOPS_icon(DM_icon *icon);

  void Lose();
  void Set_owner(Spatial_abs_hurler &h,DS_dmod* new_dmod,int new_tag);
  void Get_owner(DS_dmod*& dmod, int& tag)const;
  void Tag_object_changed(Spatial_abs_hurler&);
	void Draw(Spatial_abs_hurler &h, const DM_icon_draw_args &args)const;
  void Set_state(Spatial_abs_hurler &h, const DM_icon_cmd_args &args);
	void Query(Spatial_abs_hurler &h, DM_icon_query_args &args)const;
  DM_icon* Make_copy(Spatial_abs_hurler &h)const ;

	HC_KEY GetHOOPSKey() const;
};
#endif //DM_HOOPS_ICON_H
