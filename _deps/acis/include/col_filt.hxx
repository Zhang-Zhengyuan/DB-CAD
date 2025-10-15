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
//    Declare an entity_filter for testing color
//
//----------------------------------------------------------------------

#ifndef col_filt_hxx
#define col_filt_hxx

//======================================================================

#include "dcl_rb.h"
#include "efilter.hxx"
#include "rgbcolor.hxx"

class DECL_RB entity_color_filter : public entity_filter {
  private:
    rgb_color the_Target_Color;

  public:
    entity_color_filter(rgb_color color);
    virtual logical test(const ENTITY*) const;

};


//======================================================================
#endif
