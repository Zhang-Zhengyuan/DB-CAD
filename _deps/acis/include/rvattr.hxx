/* ORIGINAL: acis2.1/faceter/attribs/rvattr.hxx */
// $Id: rvattr.hxx,v 1.12 2002/08/09 17:21:51 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _RVATTR
#define _RVATTR



#include "dcl_fct.h"
#include "entity.hxx"

#include "af_enum.hxx"
#include "refine.hxx"
#include "vtplate.hxx"
#include "atteye3d.hxx"

class law;

extern DECL_FCT int ATTRIB_EYE_REF_VT_TYPE;

#define ATTRIB_EYE_REF_VT_LEVEL ( ATTRIB_EYE_LEVEL + 1 )

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_EYE_REF_VT, FCT )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

//DOC
// TOPIC(ATTRIB_EYE_REF_VT_TYPE,
//				ATTRIB_EYE_REF_VT_TYPE;Faceter classes)
//
// The ATTRIB_EYE_REF_VT class is an ACIS attribute used to
//	attach REFINEMENT and VERTEX_TEMPLATE instances to other
//	entities.
//
// The class can hold a pointer to one of each class.  However,
// this implementation assumes that exactly one of those pointers is
// non-null.   This allows independent replacement, and use of multiple
// refinements or vertex templates, without undue complication.
//
//
//END
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_FCT ATTRIB_EYE_REF_VT : public ATTRIB_EYE
{
   REFINEMENT      *ref ;
   VERTEX_TEMPLATE *vt ;

   public:

      ATTRIB_EYE_REF_VT( ENTITY* = NULL,
                     REFINEMENT* = NULL, VERTEX_TEMPLATE* = NULL);

	//MEMBER
	// Simple queries to extract the respective entities.
      REFINEMENT* refinement() { return( ref); }
      VERTEX_TEMPLATE* vertex_template() { return ( vt); }
	//MEMBER
	// Install respective entity pointers.
      void set_refinement( REFINEMENT* refine);
      void set_vertex_template( VERTEX_TEMPLATE* vtplate);
	//END 

      virtual logical pattern_compatible() const;   // returns TRUE

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
      ATTRIB_FUNCTIONS( ATTRIB_EYE_REF_VT, FCT )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	//MEMBER
	// Search an entity for the first attached ATTRIB_EYE_REF_VT which
	// has a non-null VT pointer.
	LOCAL_PROC ATTRIB_EYE_REF_VT* find_vt_header(const ENTITY*);
	// Search an entity for the first attached ATTRIB_EYE_REF_VT which
	// has a non-null refinement pointer whose surface mode matches given
	// value (exactly -- not by the inheritance rules!!)
	LOCAL_PROC ATTRIB_EYE_REF_VT* find_ref_header(const ENTITY*,AF_SURF_MODE mode);
	// Search an entity for the first attached ATTRIB_EYE_REF_VT which
	// has no refinement or vertex template.
	LOCAL_PROC ATTRIB_EYE_REF_VT* find_unused(const ENTITY*);
	LOCAL_PROC ATTRIB_EYE_REF_VT* find_unused_ref(const ENTITY*);
	LOCAL_PROC ATTRIB_EYE_REF_VT* find_unused_vt(const ENTITY*);
	//END
// STI jmb: Standard use count implementation
   /**
    * @nodoc
    */
   USE_COUNT_REFERENCE_DECL
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// STI jmb: end		
};
DECL_FCT logical af_query(const ENTITY * E,REFINEMENT *&R,AF_SURF_MODE mode);

DECL_FCT logical af_query(const ENTITY * E,VERTEX_TEMPLATE *&T);

DECL_FCT logical af_update(ENTITY *E,REFINEMENT *R);

DECL_FCT logical af_update(ENTITY *E,VERTEX_TEMPLATE *T);

#endif
