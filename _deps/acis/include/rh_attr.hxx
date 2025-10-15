/* ORIGINAL: rbase2.1/rnd_husk/attribs/rh_attr.hxx */
// @(#) $Id: rh_attr.hxx,v 1.12 2002/08/09 17:20:13 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// *****************************************************************
//
//  File name : $RCSfile: rh_attr.hxx,v $
//
//  Description:
//
//       Render Attribute Class Declaration
//
//       A Rendering Attribute can hold information on
//
//           surface visual attributes         ( RH_MATERIAL * )
//                                             ( RH_TEXTURE_SPACE * )
//           surface sidedness for rendering   ( logical )
//
//           entity tranformation for rendering
//
//
//  *****************************************************************
//
//  History:
//     23-June-92   PMK created
//
#if !defined ( ATTRIB_RENDER_CLASS )
#define ATTRIB_RENDER_CLASS

#include "dcl_rb.h"
#include "rbase.hxx"

#include "entity.hxx"
#include "usecount.hxx"

#ifndef NEW_FACET
#include "faceter/attribs/attlwd.hxx"
#else
//#include "faceter/acisintf/af_types.h"
#include "attrib.hxx"
#include "datamsc.hxx"

extern DECL_RB int ATTRIB_RH_TYPE;


/**
 * @nodoc
 */
#define ATTRIB_RH_LEVEL (ATTRIB_LEVEL + 1)

/*
// tbrv
// The ATTRIB_RH class wasn't doc'ed in R10, so it
// will remain undoc'ed.
*/
/**
 * @nodoc
 */
MASTER_ATTRIB_DECL( ATTRIB_RH, RB );
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif


#endif

#include "rh_enty.hxx"

class SPAtransf;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_RENDER, RB )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

extern DECL_RB int ATTRIB_RENDER_TYPE;

#define ATTRIB_RENDER_LEVEL ( ATTRIB_RH_LEVEL + 1 )

class DECL_RB ATTRIB_RENDER : public ATTRIB_RH
{
   private:

      RH_MATERIAL      * ent_material;
      int                ent_sides;
      RH_TEXTURE_SPACE * ent_tspace;
      SPAtransf           * ent_tran;
      logical            tran_mod;

      ATTRIB_RENDER
      (
	 ENTITY*            owner,
	 RH_MATERIAL *      material,
	 int                sides,
	 RH_TEXTURE_SPACE * texture_space,
	 SPAtransf           * mtl_tran ,
	 logical            tran_flag
      );

   public:

      ATTRIB_RENDER
      (
	 ENTITY*            = NULL,
	 RH_MATERIAL *      = NULL,
	 int                = 0,
	 RH_TEXTURE_SPACE * = NULL
      );

      //
      // Material properties
      //

      RH_MATERIAL* material() const         { return( ent_material ); }
      void         set_material( RH_MATERIAL *mtl );

      //
      // Sidedness
      //

      int      sides() const                 { return( ent_sides ); }
      void     set_sides( int sd );

      //
      // Texture Space for wrapped textures
      //

      RH_TEXTURE_SPACE *texture_space() const { return( ent_tspace ); }
      void              set_texture_space( RH_TEXTURE_SPACE *);

      //
      // Local transform
      //

      SPAtransf * local_transf()          const  { return(  ent_tran ); }
      logical  local_transf_modified() const  { return( tran_mod ); }

      // Functions called to aid attribute migration during
      // modelling operations.

      // the owner of this attribute is about to be split in two
      // - the argument is the new piece

      virtual void split_owner( ENTITY* ) ;

      // the owner of this attribute is about to be copied
      // - the argument is the copied piece

      virtual void copy_owner( ENTITY* ) ;


      // the owner of this attribute is about to be merged
      // with the given entity.
      // The logical argument is TRUE if the owner is to be deleted
      // in the merge.

      virtual void merge_owner( ENTITY*, logical ) ;

      virtual void trans_owner( SPAtransf const & ) ;

      /**
       * @nodoc
       */
      ATTRIB_FUNCTIONS( ATTRIB_RENDER, RB )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

	  /**
	   * @nodoc
	   */
	  USE_COUNT_REFERENCE_DECL  // STI jmb
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
};

//--------------------------------------------------------------
//
//  Class RH_ENTITY_PROPS
//
//    Render Husk Entity Properties. Currently this class is
//    used internally as a means of holding all the render
//    entity properties that can be attached to an entity
//    as an attribute. Equivalent to the ATTRIB_RENDER values.
//    It may be made visible and moved used in the Attribute
//    itself in future releases
//
//--------------------------------------------------------------

class DECL_RB RH_ENTITY_PROPS
{
   public:

      RH_MATERIAL      *material;
      int               sides;
      RH_TEXTURE_SPACE *texture_space;
      SPAtransf           *local_transf;
      logical           local_transf_modified;


      RH_ENTITY_PROPS()
      {
	  material              = NULL;
	  sides                 = 0;
	  texture_space         = NULL;
	  local_transf          = NULL;
	  local_transf_modified = FALSE;
      }
};


//
//  Function: rh_create_entity_tran
//
//  Description:
//     A utility function which is used to obtain a SPAtransf which
//     will be stored within a RENDER_ATTRIB to keep track of any
//     transformations made to an ENTITY from the time it has a
//     RH_MATERIAL attached to it.
//     Initially the SPAtransf is set to the inverse of any BODY SPAtransf
//     which may affect the ENTITY
//
DECL_RB SPAtransf * rh_create_entity_tran( ENTITY * ent );

//
//  Function:  rh_get_entity_transf
//  Description:
//
//      Get a SPAtransf for the BODY  owning an ENTITY
//      if any
//
DECL_RB logical rh_get_entity_transf( ENTITY * entity, SPAtransf & new_tran );

//
//  Function: rh_equal_transf
//
//  Description:
//   A utility function which tests if two ACIS transfs are
//   the same ( within ACIS tolerances )
//
DECL_RB logical rh_equal_transf( SPAtransf const &tran1, SPAtransf const & tran2 );


//
// rh_calc_primitive_transf
//
//    Determine if any transform needs to be applied to the
//    primitive facets generated for an entity
//
DECL_RB logical  rh_calc_primitive_transf
	(
		ENTITY                 * entity,
		RH_ENTITY_PROPS const  & props,
		SPAtransf                 & prim_tran
	);

#endif
