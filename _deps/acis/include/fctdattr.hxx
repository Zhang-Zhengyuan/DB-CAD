/* ORIGINAL: acis2.1/faceter/attribs/fctdattr.hxx */
// $Id: fctdattr.hxx,v 1.11 2002/08/09 17:21:51 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _FCTDATTR
#define _FCTDATTR

#include "dcl_fct.h"
#include "entity.hxx"

#include "af_enum.hxx"
#include "atteye3d.hxx"

class law;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_EYE_FCTD_MARK, FCT )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

extern DECL_FCT int ATTRIB_EYE_FCTD_MARK_TYPE;

#define ATTRIB_EYE_FCTD_MARK_LEVEL ( ATTRIB_EYE_LEVEL + 1 )

//DOC
// TOPIC(ATTRIB_EYE__TYPE,
//				ATTRIB_EYE_FCTD_MARK_TYPE;Faceter classes)
//
// The ATTRIB_EYE_FCTD_MARK class is an ACIS attribute used to
// mark faceted faces by attaching one instance of it to the
// faceted face. This attribute is lost on any split, merge or 
// transform action apllied to that face.
//
//
//END
class DECL_FCT ATTRIB_EYE_FCTD_MARK : public ATTRIB_EYE
{

   public:

      ATTRIB_EYE_FCTD_MARK( ENTITY* = NULL );
	// MEMBER
	
	//END 

    virtual logical pattern_compatible() const;
                                    // returns TRUE 

      // Functions called to aid attribute migration during 
      // modelling operations.

      // the owner of this attribute is about to be split in two
      // - the argument is the new piece 

      virtual void split_owner( ENTITY*);

      // the owner of this attribute is about to be merged 
      // with the given entity.
      // The logical argument is TRUE if the owner is to be deleted 
      // in the merge.

      virtual void merge_owner( ENTITY*, logical);

      virtual void trans_owner( SPAtransf const &);

	  virtual void warp_owner( law *);

      virtual void lop_change_owner();

      virtual void replace_owner_geometry( ENTITY* );

      /**
       * @nodoc
       */
      ATTRIB_FUNCTIONS( ATTRIB_EYE_FCTD_MARK, FCT )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};


#endif
