// $Id: dscsatt.hxx,v 1.4 2002/08/09 17:25:31 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dscsatt.hxx
// created  : May 11, 1995
// author   : GWCelniker
// synopsis : The Deformable Surface constraint attribute class
// contains : class ATTRIB_DSCSTRN
//                  ATTRIB_PT_CSTRN
//                  ATTRIB_CRV_CSTRN
//                  ATTRIB_LINK_CSTRN
//                  ATTRIB_AREA_CSTRN

#ifndef DS_CSATTRB_H
#define DS_CSATTRB_H

#include "agcatt.hxx"    // organization class attribute    // organization class attribute
#include "dsstdef.hxx"  // DS_assert()  // DS_assert()

extern DECL_ADM int ATTRIB_DSCSTRN_TYPE;
#define    ATTRIB_DSCSTRN_LEVEL (ATTRIB_AGC_LEVEL + 1)

// forward declarations
class pcurve ; 
class DS_pfunc_save ;
class DS_zone_save ;

// keep these the same if you can
/* version 4.0 behavior bits
#define ACS_POSITION  (1 << 0) // constrain shape SPAposition at cstrn
#define ACS_TANGENT   (1 << 1) // constrain shape tangent  at cstrn
#define ACS_CURVATURE (1 << 2) // constraint shape curvature at cstrn
#define ACS_ON_OFF    (1 << 3) // bit when set means on - else toggled off 
*/

#define ACS_ON_OFF        ( 1 << 3 ) // bit when set means on - else toggled off 

#define ACS_POS_FREE      (    0   ) // free SPAposition
#define ACS_POS_FIXED     ( 1 << 0 ) // fixed SPAposition (old ACS_POSITION)
#define ACS_POS_LINKED    ( 1 << 4 ) // linked SPAposition (for link_cstrns)

#define ACS_TAN_FREE      (    0   ) // free tangent
#define ACS_TAN_FIXED     ( 1 << 1 ) // fixed tangent (old ACS_TANGENT)
#define ACS_TAN_LINKED    ( 1 << 5 ) // linked tangent (for link_cstrns)

#define ACS_NORM_FIXED    ( 1 << 14) // fixed norm (for pt_cstrn on surf)

#define ACS_C2_FREE       (    0   ) // free curvature
#define ACS_C2_FIXED      ( 1 << 2 ) // fixed tangent (old ACS_CURVATURE)
#define ACS_C2_LINKED     ( 1 << 7 ) // linked curvature (for link_cstrns)

// constraint states used only by 2nd curve in link_cstrns

#define ACS_POS_2_FREE    (    0   ) // free SPAposition                      
#define ACS_POS_2_FIXED   ( 1 << 8 ) // fixed SPAposition  
#define ACS_POS_2_LINKED  ( 1 << 9 ) // linked SPAposition 
                                                       
#define ACS_TAN_2_FREE    (    0   ) // free tangent    
#define ACS_TAN_2_FIXED   ( 1 << 10) // fixed tangent    used also by pttan cstrn
#define ACS_TAN_2_LINKED  ( 1 << 11) // linked tangent
                                                      
#define ACS_C2_2_FREE     (    0   ) // free curvature
#define ACS_C2_2_FIXED    ( 1 << 12) // fixed tangent 
#define ACS_C2_2_LINKED   ( 1 << 13) // linked curvature 

#define ACS_NORM_FIXED    ( 1 << 14) // fixed normal

class ATTRIB_DM2ACIS;

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_DSCSTRN, NONE)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_PT_CSTRN, NONE)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_CRV_CSTRN, NONE)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_LINK_CSTRN, NONE)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_AREA_CSTRN, NONE)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

//============================================================
// class ATTRIB_DSCSTRN
//============================================================

class DECL_ADM ATTRIB_DSCSTRN: public ATTRIB_AGC   // id "acs"
{
 protected:

  int acs_tag ;            // tag so apps can track a constraint
  int acs_behavior ;       // orof: ACS_ON_OFF
                           //       ACS_POS_FREE     ACS_TAN_FREE  
                           //       ACS_POS_FIXED    ACS_TAN_FIXED  
                           //       ACS_POS_LINKED   ACS_TAN_LINKED
                           // 
                           //       ACS_POS_2_FREE   ACS_TAN_2_FREE  
                           //       ACS_POS_2_FIXED  ACS_TAN_2_FIXED 
                           //       ACS_POS_2_LINKED ACS_TAN_2_LINKED
                           //
                           //       ACS_NORM_FIXED
  double acs_tang_gain ;   // gain applied to stored tang vecs
  int acs_emech_type;       // id type number for any emech (0 for none)
  int acs_tight_state;     // tight state for the constraint

 public:

  ATTRIB_DSCSTRN           // Default Constructor
  (ENTITY *owner=NULL,     // in  : The 'owner' of this attribute    
   int behavior=0,         // orof: ACS_POS_FIXED,  ACS_TAN_FIXED,
                           //       ACS_C2_FIXED,   ACS_ON_OFF
   double tang_gain=1.0,   // in  : gain applied to stored tang vecs
   int tag = -1,           // in  : tag so apps can track a constraint
   int emech_type=0,
   int tight_state=1)
  : ATTRIB_AGC(owner)      // initialize 'this' attrib's parent
                           { acs_tag       = tag ;
                             acs_behavior  = behavior ;
                             acs_tang_gain = tang_gain ; 
                             acs_emech_type= emech_type;
                             acs_tight_state=tight_state;
                           }

  ATTRIB_DSCSTRN           // Copy Constructor
  (ATTRIB_DSCSTRN &cstrn,  // in : The attribute being copied
   ENTITY *owner)          // in : owner for new attribute
  : ATTRIB_AGC(owner)      // initialize 'this' attrib's parent
                           { acs_tag       = cstrn.acs_tag ;
                             acs_behavior  = cstrn.acs_behavior ;
                             acs_tang_gain = cstrn.acs_tang_gain ;
                             acs_emech_type= cstrn.acs_emech_type;
                           }

  ATTRIB_DSCSTRN &operator=  // assignment operator
   (const ATTRIB_DSCSTRN &cstrn) 
                           { // no work condition
                             if(&cstrn != this) {
                             ((ATTRIB_AGC *)this)->operator=(cstrn) ;
                             acs_tag       = cstrn.acs_tag ;
                             acs_behavior  = cstrn.acs_behavior ;
                             acs_tang_gain = cstrn.acs_tang_gain ;
                             acs_emech_type= cstrn.acs_emech_type;
                             }
                             return *this ;
                           }

  virtual ATTRIB_AGC*      // eff: Virtual ATTRIB_AGC copy function
   Make_copy(ENTITY *owner)// in : owner for new attribute
                           { return ( ACIS_NEW ATTRIB_DSCSTRN(*this,owner) ) ; }

  virtual logical pattern_compatible() const;
                                    // returns TRUE 
  // ATTRIB_DSCSTRN simple data access routines

  int  Tag()               // ret: the current tag value
                           { return acs_tag ; }

  void Set_tag(int tag)    // eff: set the current tag value
                           { backup() ;
                             acs_tag = tag ; 
                           }

  int Behavior()           // ret: the current behavior bit array
                           { return acs_behavior ; }

  void Set_behavior        // eff : set the current behavior bit array
    (int behavior)         // in : orof ACS_ON_OFF
                           //           ACS_POS_FREE     ACS_TAN_FREE    
                           //           ACS_POS_FIXED    ACS_TAN_FIXED   
                           //           ACS_POS_LINKED   ACS_TAN_LINKED  
                           //                                          
                           //           ACS_POS_2_FREE   ACS_TAN_2_FREE  
                           //           ACS_POS_2_FIXED  ACS_TAN_2_FIXED  
                           //           ACS_POS_2_LINKED ACS_TAN_2_LINKED
                           //
                           //                            ACS_NORM_FIXED
                           { backup() ;
                             acs_behavior = behavior ;
                           }

  double Tang_gain()       // ret: the current tang_gain value
                           { return acs_tang_gain ; }
  void Set_tang_gain       // eff: set the stored tang_gain value
    (double tang_gain)     { backup() ;
                             acs_tang_gain = tang_gain ; 
                           }

  int Emech_type()       {return acs_emech_type;}
  void Set_emech_type       // eff: set the stored tang_gain value
    (int emech_type)     { backup() ;
                             acs_emech_type = emech_type ; 
                           }
  
  int Tight_state()      {return acs_tight_state;}
  void Set_tight_state   // eff: set the tight state
     (int state)         { backup();
                           acs_tight_state = state; 
                         }

  virtual void             // bas: class ATTRIB_DSCSTRN virtual function
   Fixup_src_data(ATTRIB_DM2ACIS *);     // eff: resets src_data pointers to appropriate
                           //      ENTITIES.
                           // use: Making edges tolerant moves the ATTRIB_DSCSTRN's
                           //      so the src_data pointers need to be set.

  // ATTRIB_DSCSTRN implementations of ATTRIB virtual functions

  virtual void             // bas: class ATTRIB virtual function
   split_owner             // eff: delete the attribute
   (ENTITY *)              // in : the new entity being created
                           { lose() ; }

  virtual void             // bas: class ATTRIB virtual function
   merge_owner             // eff: delete the attribute
   (ENTITY *,              // in : other = the merge's 2nd topology entity
    logical )              // in : deleted = True=this_entity to be deleted
                           //               False=other_entity to be deleted
                           { lose() ; }

  virtual void             // bas: class ATTRIB virtual function
   trans_owner             // eff: no-op - no points or vectors
   (SPAtransf const&)         // in : the affine transformation
                           { }

 // header declarations for all ACIS attribute standard functions
 /**
  * @nodoc
  */
 ATTRIB_FUNCTIONS(ATTRIB_DSCSTRN, NONE)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

} ; // end class ATTRIB_DSCSTRN


//==================================================
// Class ATTRIB_PT_CSTRN Derived from ATTRIB_DSCSTRN 
//==================================================

extern DECL_ADM int ATTRIB_PT_CSTRN_TYPE ;
#define    ATTRIB_PT_CSTRN_LEVEL (ATTRIB_DSCSTRN_LEVEL + 1)

class DECL_ADM ATTRIB_PT_CSTRN: public ATTRIB_DSCSTRN   // id "apc"
{
 protected:

  int     apc_image_dim ;  // [array_size] problem image dim size
  int     apc_domain_dim ; // [array_size] problem domain dim size

  double *apc_domain_pt ;  // domain point load SPAposition
                           // Sized:[apc_domain_dim]
  double *apc_domain1_dir; // 1st tangent constraint uv direction
                           // Sized:[apc_domain_dim]
  double *apc_domain2_dir; // 2nd tangent constraint uv direction
                           // Sized:[apc_domain_dim]

  double *apc_base_pt ;    // image loc of domain_pt on surface
                           // Sized:[apc_image_dim]
  double *apc_tang1_val ;  // 1st tangent vec at domain_pt on surface
                           // Sized:[apc_image_dim]
  double  apc_curv1_val ;  // curvature vec at domain_pt on surface

  double  apc_norm_size ;  // size used to calc norm_pt
  double *apc_norm_val ;   // normal vec at domain_pt on crv or surf
                           // Sized:[apc_image_dim]
  double  apc_binorm_size; // size used to calc binorm_pt
  double *apc_binorm_val ; // binormal vec at domain_pt on curve
                           // Sized:[apc_image_dim]
  double *apc_tang2_val ;  // 2nd tangent vec at domain_pt on surface
                           // Sized:[apc_image_dim]
  double  apc_curv2_val ;  // curvature vec at domain_pt on surface

 public:

  ATTRIB_PT_CSTRN           //      Default Constructor
  (ENTITY *owner=NULL,      // in : The 'owner' of this attribute
   int image_dim=0,         // in : [array-size] image dimension
   int domain_dim=0,        // in : [array-size] domain dimension
   int behavior=0,          // in : orof ACS_POS_FIXED,  ACS_TAN_FIXED,
                            //           ACS_C2_FIXED,   ACS_ON_OFF
   double tang_gain=1.0,    // in : gain applied to stored tang_val
   double *domain_pt  =NULL,// in : SPAposition to copy [domain_dim]
   double *domain1_dir=NULL,// in : tang dir to copy [domain_dim]
   double *domain2_dir=NULL,// in : tang dir to copy [domain_dim]
   double *base_pt  =NULL,  // in : SPAposition to copy [image_dim]
   double *tang1_val=NULL,  // in : tang vec to copy [image_dim]
   double *tang2_val=NULL,  // in : tang vec to copy [image_dim]
   double  norm_size=-1.0,  // in : size used to calc norm_pt
   double *norm_val =NULL,  // in : norm vec to copy [image_dim]
   double  binorm_size=-1.0,// in : size used to calc binorm_pt
   double *binorm_val=NULL, // in : binorm vec to copy [image_dim]
   double  curv1_val=-1.0,  // in : curvature val to copy 
   double  curv2_val=-1.0,  // in : curvature val to copy 
   int tag= -1) ;           // in : tag so apps can track a constraint
                            // sized:[domain_dim](NULL if dmn_dim = 0)

  ATTRIB_PT_CSTRN          //      Copy Constructor
  (ATTRIB_PT_CSTRN &apc,   // in : The attribute being copied
   ENTITY *owner=NULL) ;   // in : owner for new attribute

  ATTRIB_PT_CSTRN &operator=   // assignment operator
   (const ATTRIB_PT_CSTRN &); // in : object with values to copy

  virtual logical pattern_compatible() const;
                                    // returns TRUE 
  virtual ATTRIB_AGC*      // eff: Virtual ATTRIB_AGC copy function
   Make_copy(ENTITY *owner)// in : owner for new attribute
                           { return ( ACIS_NEW ATTRIB_PT_CSTRN(*this,owner) ) ; }

  void
   Size_arrays             // Size Internal memory arrays
    (int image_dim,        // in : image space size
     int domain_dim) ;     // in : domain space size

  void
   Init_vectors() ;        // eff: set all vectors to SPAunit_vector

   // ATTRIB_PT_CSTRN simple data access routines

  int     Domain_dim()const{ return apc_domain_dim ; } 
  int     Image_dim() const{ return apc_image_dim ; } 

  double *Domain_pt()      { return apc_domain_pt ; }
  double &Domain_pt(int d) { DS_assert(d >= 0 && d < apc_domain_dim) ;
                             return(apc_domain_pt[d]) ;
                           }

  double *Domain1_dir()      { return apc_domain1_dir ; }
  double &Domain1_dir(int d) { DS_assert(d >= 0 && d < apc_domain_dim) ;
                               return(apc_domain1_dir[d]) ;
                             }
  double *Domain2_dir()      { return apc_domain2_dir ; }
  double &Domain2_dir(int d) { DS_assert(d >= 0 && d < apc_domain_dim) ;
                               return(apc_domain2_dir[d]) ;
                             }

  double *Base_pt()        { return apc_base_pt ; }
  double &Base_pt(int d)   { DS_assert(d >= 0 && d < apc_image_dim) ;
                             return(apc_base_pt[d]) ;
                           }

  double *Tang1_val()      { return apc_tang1_val ; }
  double &Tang1_val(int d) { DS_assert(d >= 0 && d < apc_image_dim) ;
                             return(apc_tang1_val[d]) ;
                           }

  double  Norm_size()      { return apc_norm_size ; }
  void Set_norm_size       // eff: set apc_norm_size
    (double norm_size)     { apc_norm_size = norm_size ;
                           }

  double *Norm_val()       { return apc_norm_val ; }
  double &Norm_val(int d)  { DS_assert(d >= 0 && d < apc_image_dim) ;
                             return(apc_norm_val[d]) ;
                           }

  double  Binorm_size()    { return apc_binorm_size ; }
  void Set_binorm_size     // eff: set apc_binorm_size
    (double binorm_size)   { apc_binorm_size = binorm_size ;
                           }

  double *Binorm_val()     { return apc_binorm_val ; }
  double &Binorm_val(int d){ DS_assert(d >= 0 && d < apc_image_dim) ;
                             return(apc_binorm_val[d]) ;
                           }

  double *Tang2_val()      { return apc_tang2_val ; }
  double &Tang2_val(int d) { DS_assert(d >= 0 && d < apc_image_dim) ;
                             return(apc_tang2_val[d]) ;
                           }

  double *Curv1_val()      { return &apc_curv1_val ; }
  void Set_curv1_val       // set: apc_curv1_val
    (double curv1_val)     // in : the value to copy
                           { apc_curv1_val = curv1_val ; 
                           }

  double *Curv2_val()      { return &apc_curv2_val ; }
  void Set_curv2_val       // set: apc_curv2_val
    (double curv2_val)     // in : the value to copy
                           { apc_curv2_val = curv2_val ; 
                           }

  int Domain_pt_size       // ret: the computed domain space size
    (int domain_dim)       { return domain_dim ; }

  int Image_pt_size        // ret: the computed image space size
    (int image_dim)        { return image_dim ; }

  int Block_size           // rtn: total internal array size
   (int image_dim,         // in : image  space dimension
    int domain_dim)        // in : domain space dimension
                           { return(  3 * domain_dim   // domain_pt
                                                       // domain1_dir, 
                                                       // domain2_dir
                                    + 5 * image_dim) ; // base_pt
                                                       // tang1, 
                                                       // tang2, 
                                                       // norm
                                                       // binorm
                           }

  virtual void             // bas: class ATTRIB_DSCSTRN virtual function
   Fixup_src_data(ATTRIB_DM2ACIS *);     // eff: resets src_data pointers to appropriate
                           //      ENTITIES.
                           // use: Making edges tolerant moves the ATTRIB_DSCSTRN's
                           //      so the src_data pointers need to be set.

  // ATTRIB_PT_CSTRN implementations of ATTRIB virtual functions

  virtual void             // bas: class ATTRIB virtual function
   split_owner             // eff: delete the attribute
   (ENTITY *)              // in : the new entity being created
                           { lose() ; }

  virtual void             // bas: class ATTRIB virtual function
   merge_owner             // eff: delete the attribute
   (ENTITY *,              // in : other = the merge's 2nd topology entity
    logical )              // in : deleted = True=this_entity to be deleted
                           //                False=other_entity to be deleted
                           { lose() ; }

  virtual void             // bas: class ATTRIB virtual function
   trans_owner             // eff: affine transformation callback
   (SPAtransf const& xform) ; // in : the affine transformation

  /**
   * @nodoc
   */
  ATTRIB_FUNCTIONS(ATTRIB_PT_CSTRN, NONE)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

} ; // end class ATTRIB_PT_CSTRN

//==================================================
// Class ATTRIB_CRV_CSTRN Derived from ATTRIB_DSCSTRN 
//==================================================

extern DECL_ADM int ATTRIB_CRV_CSTRN_TYPE ;
#define    ATTRIB_CRV_CSTRN_LEVEL (ATTRIB_DSCSTRN_LEVEL + 1)

class DECL_ADM ATTRIB_CRV_CSTRN: public ATTRIB_DSCSTRN   // id "acc"
{
  protected:

   DS_pfunc_save *         // Save description of the domain curve
     acc_domain_crv_save ; 
     
   DS_pfunc_save *         // Save description of src_W_pts
     acc_src_W_pts_save ;
                    
   DS_pfunc_save *         // Save description of src_Wn_pts
     acc_src_Wn_pts_save ;
                    
   DS_pfunc_save *         // Save description of src_Wnn_pts
     acc_src_Wnn_pts_save ;               
   
  // ACIS usage note:
  // Things have changed: As of 8/20/98
  //  1.) All crv_cstrns map to an ACIS curve.
  //      Each ACIS topology curve is marked with an ATTRIB_CRV_CSTRN.
  //      ds_solid_cstrn crv_cstrns from ACIS will have a NULL
  //      acc_domain_crv_save since their definition of the
  //      curve shape comes from the functional call-back
  //      mechanism supported by curve constraints.
  //      
  //  out of date approach  
  //  1.) crv_cstrns are created to represent topology boundaries.  
  //      In which case they are NOT stored as an ATTRIB_CRV_CSTRN
  //      because their geometry is stored as an attribute of their
  //      associated topology entity. During sculpting all
  //      such DS_curve_cstrns have a usage type of ds_solid_cstrn
  //      or ds_bound_cstrn.  Association between the 
  //      DS_crv_cstrn and the topology entity is kept by placing
  //      the COEDGE pointer into the DS_crv_cstrn's DS_crv2acis object.
  //  1a.)crv_cstrns which act as seams to connect a child face to
  //      a parent face are identifed by the presence of the
  //      ATTRIB_DS_SEAM attribute on the acis edge.  During 
  //      init all such ACIS EDGES generate a curve constraint
  //      labeled as ds_seam_cstrn in the child constraint and
  //      do not generate a curve constraint for the parent.
  //      The fields in the ATTRIB_DS_SEAM object identify the
  //      the child and the parent.  Associating the child's
  //      crv_cstrn and the topology entity is kept by placing
  //      the COEDGE pointer into the DS_crv_cstrn's DS_crv2acis object.
  //  2.) Non-boundary crv_cstrns may be added interactively
  //      by an end-user at any time.  These crv_cstrns
  //      are stored as ATTRIB_CRV_CSTRNs. During sculpting
  //      all such DS_curve_cstrns have a usage type of 
  //      ds_user_bndry.  Association between the ATTRIB_CRV_CSTRN
  //      and the DS_crv_cstrn is kept by tag number just like
  //      all other cstrn and load types

// Current ACIS crv_cstrn sources, privledges, and src_data
//
// DS_CSTRN_SRC    delete  stop   src_  src_CW  comment
//                  able   able   data    _func
//
// ds_solid_cstrn    NO     NO    YES  YES  crv from solid model bndry
//                                          any number of faces may
//                                          connect to crv
// ds_bound_cstrn    NO    YES    YES  NO   crv from sheet model bndry
//                                          only face being deformed
//                                          connects to crv
// ds_user_cstrn    YES    YES    NULL NO   crv from user interface
//                                          and not from topology
//                                          boundaries
// ds_seam_cstrn     NO     NO    YES  NO   crv joins 2 hierarchical faces
// ds_link_cstrn     NO     NO    YES  NO   crv joins 2 multisurface faces


  public:

  ATTRIB_CRV_CSTRN         //      Default Constructor
  (ENTITY *owner=NULL,     // in : The 'owner' of this attribute
   int behavior=0,         // in : orof ACS_POS_FIXED,  ACS_TAN_FIXED,
                           //           ACS_C2_FIXED,   ACS_ON_OFF
   double tang_gain=1.0,   // in : vary tang constraint magnitude
   int emech_type=0,
   int tight_state=1,
   DS_pfunc_save *         // in : Save description of domain crv
     domain_crv_save=NULL, //
   DS_pfunc_save *         // in : Save description of src_W_pts
     src_W_pts_save=NULL,  //
   DS_pfunc_save *         // in : Save description of src_Wn_pts
     src_Wn_pts_save=NULL, //
   DS_pfunc_save *         // in : Save description of src_Wnn_pts
     src_Wnn_pts_save=NULL,//
   int tag= -1) ;          // in : tag so apps can track a constraint
                           // sized:[domain_dim](NULL if dmn_dim = 0)

  ATTRIB_CRV_CSTRN         //      Copy Constructor
  (ATTRIB_CRV_CSTRN &acc,  // in : The attribute being copied
   ENTITY *owner=NULL) ;   // in : owner for new attribute

  ATTRIB_CRV_CSTRN &operator=  // assignment operator
   (const ATTRIB_CRV_CSTRN &);   // in : object with values to copy

  virtual ATTRIB_AGC*      // eff: Virtual ATTRIB_AGC copy function
   Make_copy(ENTITY *owner)// in : owner for new attribute
                           { return ( ACIS_NEW ATTRIB_CRV_CSTRN(*this,owner) ) ; }

  virtual logical pattern_compatible() const;
                                    // returns TRUE 
  // ATTRIB_CRV_CSTRN simple data access routines
  DS_pfunc_save *          // rtn: Save description of domain crv
   Domain_crv_save()       { return acc_domain_crv_save ; }

  void                     // eff: replace domain crv Save 
   Set_domain_crv_save     //      description with input
    (DS_pfunc_save *       // in : Modified Save dsc of domain crv
      domain_crv_save) ;   //      input stored/deleted with ATTRIB

  DS_pfunc_save *          // rtn: Save description of W_pts crv
   Src_W_pts_save()        { return acc_src_W_pts_save ; }

  void                     // eff: replace W_pts Save 
   Set_src_W_pts_save      //      description with input
    (DS_pfunc_save *       // in : Modified Save dsc of W_pts crv
      src_W_pts_save) ;    //      input stored/deleted with ATTRIB

  DS_pfunc_save *          // rtn: Save description of Wn_pts crv
   Src_Wn_pts_save()        { return acc_src_Wn_pts_save ; }

  void                     // eff: replace Wn_pts Save 
   Set_src_Wn_pts_save      //      description with input
    (DS_pfunc_save *       // in : Modified Save dsc of Wn_pts crv
      src_Wn_pts_save) ;    //      input stored/deleted with ATTRIB

  DS_pfunc_save *          // rtn: Save description of Wnn_pts crv
   Src_Wnn_pts_save()        { return acc_src_Wnn_pts_save ; }

  void                     // eff: replace Wnn_pts Save 
   Set_src_Wnn_pts_save      //      description with input
    (DS_pfunc_save *       // in : Modified Save dsc of Wnn_pts crv
      src_Wnn_pts_save) ;    //      input stored/deleted with ATTRIB

  virtual void             // bas: class ATTRIB_DSCSTRN virtual function
   Fixup_src_data(ATTRIB_DM2ACIS *);     // eff: resets src_data pointers to appropriate
                           //      ENTITIES.
                           // use: Making edges tolerant moves the ATTRIB_DSCSTRN's
                           //      so the src_data pointers need to be set.

  // ATTRIB_CRV_CSTRN implementations of ATTRIB virtual functions
  virtual void
	to_tolerant_owner(ENTITY* tol_ent);

  virtual void             // bas: class ATTRIB virtual function
   split_owner             // eff: deepcopy attrib onto new_entity
   (ENTITY* ent);          // in : the new entity being created

  virtual void             // bas: class ATTRIB virtual function
   merge_owner             // eff: att callback on topology merges
   (ENTITY *,              // in : other = the merge's 2nd topology entity
    logical )              // in : deleted = True=this_entity to be deleted
                           //                False=other_entity to be deleted
                           { lose() ; }

  virtual void             // bas: class ATTRIB virtual function
   trans_owner             // eff: no-opt - no image pts to move
   (SPAtransf const&)         // in : xform = the affine transformation
                           { }

  /**
   * @nodoc
   */
  ATTRIB_FUNCTIONS(ATTRIB_CRV_CSTRN, NONE)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

} ; // end class ATTRIB_CRV_CSTRN

//==================================================
// Class ATTRIB_LINK_CSTRN Derived from ATTRIB_DSCSTRN 
//==================================================

extern DECL_ADM int ATTRIB_LINK_CSTRN_TYPE ;
#define    ATTRIB_LINK_CSTRN_LEVEL (ATTRIB_DSCSTRN_LEVEL + 1)

class DECL_ADM ATTRIB_LINK_CSTRN: public ATTRIB_DSCSTRN   // id "alc"
{
  protected:

   ENTITY        *alc_entity_1 ;           // remember which face(edge) is domain1
   DS_pfunc_save *alc_entity_1_crv_save ;  // Save description of the domain1 curve

   ENTITY        *alc_entity_2 ;           // remember which face(edge) is domain2
   DS_pfunc_save *alc_entity_2_crv_save ;  // Save description of the domain2 curve

   DS_pfunc_save *         // Save description of src_W_pts
     alc_src1_W_pts_save ;
                    
   DS_pfunc_save *         // Save description of src_Wn_pts
     alc_src1_Wn_pts_save ;
                    
   DS_pfunc_save *         // Save description of src_Wnn_pts
     alc_src1_Wnn_pts_save ;               
   
   DS_pfunc_save *         // Save description of src_W_pts
     alc_src2_W_pts_save ;
                    
   DS_pfunc_save *         // Save description of src_Wn_pts
     alc_src2_Wn_pts_save ;
                    
   DS_pfunc_save *         // Save description of src_Wnn_pts
     alc_src2_Wnn_pts_save ;               
   
  
  // ACIS usage note:
  //  1.) All link_cstrns map to an ACIS curve.
  //      Each ACIS topology curve connectig two faces in a multi-face
  //      set are assigned an ATTRIB_LINK_CSTRN object.
  //      (ACIS topology curves lieing between a parent and a child within
  //      a deformable modeling hierarchy are assigned an ATTRIB_CRV_CSTRN.)
  //      The ATTRIB_LINK_CSTRN object is used to store the parametric
  //      curves which describe the shape of the link constraint on
  //      both of the connected faces.

// Current ACIS link_cstrn sources, privledges, and src_data
//
// DS_CSTRN_SRC    delete  stop   src_  src_CW  comment
//                  able   able   data    _func
//
// ds_solid_cstrn    NO     NO    YES  YES  crv from solid model bndry
//                                          any number of faces may
//                                          connect to crv
// ds_bound_cstrn    NO    YES    YES  NO   crv from sheet model bndry
//                                          only face being deformed
//                                          connects to crv
// ds_user_cstrn    YES    YES    NULL NO   crv from user interface
//                                          and not from topology
//                                          boundaries
// ds_seam_cstrn     NO     NO    YES  NO   crv joins 2 hierarchical faces
// ds_link_cstrn     NO     NO    YES  NO   crv joins 2 multisurface faces

  int                      //     1 ==> is a zombie  0 ==> not
   Is_zombie() const;      //     Checks if this is a zombie attrib

  void Fixup_zombie();     //     Checks if *this is a zombie and blows away
                           //     all pointers if it is
  public:

  ATTRIB_LINK_CSTRN        //      Default Constructor
  (ENTITY *owner=NULL,     // in : The 'owner' of this attribute
   int behavior=0,         // in : orof ACS_POS_FREE     ACS_TAN_FREE    
                           //           ACS_POS_FIXED    ACS_TAN_FIXED   
                           //           ACS_POS_LINKED   ACS_TAN_LINKED  
                           //                                          
                           //           ACS_POS_2_FREE   ACS_TAN_2_FREE  
                           //           ACS_POS_2_FIXED  ACS_TAN_2_FIXED  
                           //           ACS_POS_2_LINKED ACS_TAN_2_LINKED
   double tang_gain  =1.0, // in : vary tang constraint magnitude
   int emech_type=0,       // in : emech description
   int tight_state=1,      // in : constraint tight state
   ENTITY *entity_1  =NULL,// in : 1st face being connected
   DS_pfunc_save *         // in : Save description of domain crv
    entity_1_crv_save=NULL,//    : for dmesh1 face
   ENTITY *entity_2  =NULL,// in : 2nd face being connected
   DS_pfunc_save *         // in : Save description of domain crv
    entity_2_crv_save=NULL,//    : for dmesh2 face
   DS_pfunc_save *         // in : Save description of src1_W_pts
     src1_W_pts_save=NULL, //
   DS_pfunc_save *         // in : Save description of src1_Wn_pts
     src1_Wn_pts_save=NULL,//
   DS_pfunc_save *         // in : Save description of src1_Wnn_pts
     src1_Wnn_pts_save=NULL,//
   DS_pfunc_save *         // in : Save description of src2_W_pts
     src2_W_pts_save=NULL, //
   DS_pfunc_save *         // in : Save description of src2_Wn_pts
     src2_Wn_pts_save=NULL,//
   DS_pfunc_save *         // in : Save description of src2_Wnn_pts
     src2_Wnn_pts_save=NULL,//
   int tag= -1) ;             // in : tag so apps can track a constraint

  ATTRIB_LINK_CSTRN        //      Copy Constructor
  (ATTRIB_LINK_CSTRN &alc, // in : The attribute being copied
   ENTITY *owner=NULL) ;   // in : owner for new attribute

  ATTRIB_LINK_CSTRN &operator=  // assignment operator
   (const ATTRIB_LINK_CSTRN &); // in : object with values to copy

  virtual ATTRIB_AGC*      // eff: Virtual ATTRIB_AGC copy function
   Make_copy(ENTITY *owner)// in : owner for new attribute
                           { return ( ACIS_NEW ATTRIB_LINK_CSTRN(*this,owner) ) ; }

  virtual logical pattern_compatible() const;
                                    // returns Is_zombie() 
  // ATTRIB_LINK_CSTRN simple data access routines

  ENTITY *Entity_1()       // rtn: ENTITY ptr for 1st face
                           { return alc_entity_1 ; }
 
  ENTITY *Entity_2()       // rtn: ENTITY ptr for 2nd face
                           { return alc_entity_2 ; }

  DS_pfunc_save *          // rtn: Save description of entity_1 crv
   Entity_1_crv_save()      { return alc_entity_1_crv_save ; }

  DS_pfunc_save *          // rtn: Save description of entity_2 crv
   Entity_2_crv_save()      { return alc_entity_2_crv_save ; }

  void Set_entity_1        // eff: set the entity_1 ptr value
   (ENTITY *entity_1)      // in : ptr value to store
                           { backup() ;
                             alc_entity_1 = entity_1 ; 
                           }

  void Set_entity_2        // eff: set the entity_2 ptr value
   (ENTITY *entity_2)      // in : ptr value to store
                           { backup() ;
                             alc_entity_2 = entity_2 ; 
                           }

  void 
   Set_entity_1_crv_save   // eff: set the entity_1_crv_save ptr value
    (DS_pfunc_save *       // in : ptr value to store
      entity_1_crv_save) ;

  void 
   Set_entity_2_crv_save   // eff: set the entity_2_crv_save ptr value
    (DS_pfunc_save *       // in : ptr value to store
      entity_2_crv_save) ;

  // Src1_ pts_save access
  DS_pfunc_save *          // rtn: Save description of W_pts crv
   Src1_W_pts_save()       { return alc_src1_W_pts_save ; }

  DS_pfunc_save *          // rtn: Save description of Wn_pts crv
   Src1_Wn_pts_save()      { return alc_src1_Wn_pts_save ; }

  DS_pfunc_save *          // rtn: Save description of Wnn_pts crv
   Src1_Wnn_pts_save()     { return alc_src1_Wnn_pts_save ; }

  // Src2_ pts_save access
  DS_pfunc_save *          // rtn: Save description of Wnn_pts crv
   Src2_Wnn_pts_save()     { return alc_src2_Wnn_pts_save ; }

  DS_pfunc_save *          // rtn: Save description of Wn_pts crv
   Src2_Wn_pts_save()      { return alc_src2_Wn_pts_save ; }

  DS_pfunc_save *          // rtn: Save description of W_pts crv
   Src2_W_pts_save()       { return alc_src2_W_pts_save ; }

  // Src pts_save access by entity
  DS_pfunc_save *          // rtn: Save description of W_pts crv
   Src_W_pts_save          //
    (ENTITY *entity)       // in : for this entity
                           { if(entity == alc_entity_1)
                               return alc_src1_W_pts_save ; 
                             else if(entity == alc_entity_2)
                               return alc_src2_W_pts_save ;
                             else return(NULL) ;
                           }

  DS_pfunc_save *          // rtn: Save description of Wn_pts crv
   Src_Wn_pts_save         // eff: branch on entity and rtn Wn_pts crv
    (ENTITY *entity)       // in : for this entity
                           { if(entity == alc_entity_1)
                               return alc_src1_Wn_pts_save ; 
                             else if(entity == alc_entity_2)
                               return alc_src2_Wn_pts_save ;
                             else return(NULL) ;
                           }

  DS_pfunc_save *          // rtn: Save description of Wnn_pts crv
   Src_Wnn_pts_save        // eff: branch on entity and rtn Wnn_pts crv
    (ENTITY *entity)       // in : for this entity
                           { if(entity == alc_entity_1)
                               return alc_src1_Wnn_pts_save ; 
                             else if(entity == alc_entity_2)
                               return alc_src2_Wnn_pts_save ;
                             else return(NULL) ;
                           }

  // Set Src1 save_pts curve data
  void                     // eff: replace W_pts Save 
   Set_src1_W_pts_save     //      description with input
    (DS_pfunc_save *       // in : Modified Save dsc of W_pts crv
      src1_W_pts_save) ;   //      input stored/deleted with ATTRIB

  void                     // eff: replace Wn_pts Save 
   Set_src1_Wn_pts_save    //      description with input
    (DS_pfunc_save *       // in : Modified Save dsc of Wn_pts crv
      src1_Wn_pts_save) ;  //      input stored/deleted with ATTRIB

  void                     // eff: replace Wnn_pts Save 
   Set_src1_Wnn_pts_save   //      description with input
    (DS_pfunc_save *       // in : Modified Save dsc of Wnn_pts crv
      src1_Wnn_pts_save) ; //      input stored/deleted with ATTRIB

  // Set Src2 save_pts curve data
  void                     // eff: replace W_pts Save 
   Set_src2_W_pts_save     //      description with input
    (DS_pfunc_save *       // in : Modified Save dsc of W_pts crv
      src2_W_pts_save) ;   //      input stored/deleted with ATTRIB

  void                     // eff: replace Wn_pts Save 
   Set_src2_Wn_pts_save    //      description with input
    (DS_pfunc_save *       // in : Modified Save dsc of Wn_pts crv
      src2_Wn_pts_save) ;  //      input stored/deleted with ATTRIB

  void                     // eff: replace Wnn_pts Save 
   Set_src2_Wnn_pts_save   //      description with input
    (DS_pfunc_save *       // in : Modified Save dsc of Wnn_pts crv
      src2_Wnn_pts_save) ; //      input stored/deleted with ATTRIB

  // Set Src save_pts curve data by entity
  void                     // eff: replace W_pts Save curve
   Set_src_W_pts_save      //      description with input
    (ENTITY *entity,       // in : for this entity
     DS_pfunc_save *       // in : using this Save dsc of W_pts crv
      src1_W_pts_save) ;   //      input stored/deleted with ATTRIB

  void                     // eff: replace Wn_pts Save curve
   Set_src_Wn_pts_save     //      description with input
    (ENTITY *entity,       // in : for this entity
     DS_pfunc_save *       // in : using this Save dsc of Wn_pts crv
      src1_Wn_pts_save) ;  //      input stored/deleted with ATTRIB

  void                     // eff: replace Wnn_pts Save curve
   Set_src_Wnn_pts_save    //      description with input
    (ENTITY *entity,       // in : for this entity
     DS_pfunc_save *       // in : using this Save dsc of Wnn_pts crv
      src1_Wnn_pts_save) ; //      input stored/deleted with ATTRIB


  virtual void             // bas: class ATTRIB_DSCSTRN virtual function
   Fixup_src_data(ATTRIB_DM2ACIS *);     // eff: resets src_data pointers to appropriate
                           //      ENTITIES.
                           // use: Making edges tolerant moves the ATTRIB_DSCSTRN's
                           //      so the src_data pointers need to be set.

  // ATTRIB_LINK_CSTRN implementations of ATTRIB virtual functions

  virtual void             // bas: class ATTRIB virtual function
   split_owner             // eff: deepcopy attrib onto new_entity
   (ENTITY *)              // in : the new entity being created
                           { lose() ; }

  virtual void             // bas: class ATTRIB virtual function
   merge_owner             // eff: att callback on topology merges
   (ENTITY *,              // in : other = the merge's 2nd topology entity
    logical )              // in : deleted = True=this_entity to be deleted
                           //                False=other_entity to be deleted
                           { lose() ; }

  virtual void             // bas: class ATTRIB virtual function
   trans_owner             // eff: no-opt - no image pts to move
   (SPAtransf const&)         // in : xform = the affine transformation
                           { }

  /**
   * @nodoc
   */
  ATTRIB_FUNCTIONS(ATTRIB_LINK_CSTRN, NONE)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

} ; // end class ATTRIB_LINK_CSTRN

//==================================================
// Class ATTRIB_AREA_CSTRN Derived from ATTRIB_DSCSTRN 
//==================================================

extern DECL_ADM int ATTRIB_AREA_CSTRN_TYPE ;
#define    ATTRIB_AREA_CSTRN_LEVEL (ATTRIB_DSCSTRN_LEVEL + 1)

class DECL_ADM ATTRIB_AREA_CSTRN: public ATTRIB_DSCSTRN   // id "aac"
{
 protected:

  int     aac_zone_flag ;  // 0=zone area is free to move           
                           //   zone compliment area is fixed       
                           // 1=zone area is fixed                  
                           //   zone compliment area is free to move
  DS_zone_save *           // defines an area                       
        aac_zone_save ;
 public:

  ATTRIB_AREA_CSTRN        //      Default Constructor
  (ENTITY *owner=NULL,     // in : The 'owner' of this attribute
   int zone_flag=0,        // in : 0=zone area is free to move           
                           //        zone compliment area is fixed       
                           //      1=zone area is fixed                  
                           //        zone compliment area is free to move
   DS_zone_save *zone_save // in : defines a shape partition [nested]
       =NULL,
   int behavior=0,         // in : orof ACS_ON_OFF
   int tag= -1) ;          // in : tag so apps can track a constraint
                           // sized:[domain_dim](NULL if dmn_dim = 0)

  ATTRIB_AREA_CSTRN        //      Copy Constructor
  (ATTRIB_AREA_CSTRN &aac, // in : The attribute being copied
   ENTITY *owner=NULL) ;   // in : owner for new attribute

  ATTRIB_AREA_CSTRN &operator=   // assignment operator
   (const ATTRIB_AREA_CSTRN &); // in : object with values to copy

  virtual ATTRIB_AGC*      // eff: Virtual ATTRIB_AGC copy function
   Make_copy(ENTITY *owner)// in : owner for new attribute
                           { return ( ACIS_NEW ATTRIB_AREA_CSTRN(*this,owner) ) ; }

  virtual logical pattern_compatible() const;
                                    // returns TRUE 
   // ATTRIB_AREA_CSTRN simple data access routines

  int                      // rtn: 0=zone area is free to move           
   Zone_flag()             //        zone compliment area is fixed       
                           //      1=zone area is fixed                  
                           //        zone compliment area is free to move
                           { return aac_zone_flag ; }
  void
   Set_zone_flag           // eff: set zone_flag value
     (int zone_flag)       // in : input value to save
                           { aac_zone_flag = zone_flag ;
                           }
                             
  DS_zone_save *           // rtn: Save description of zone
   Zone_save()             { return aac_zone_save ; }

  void Set_zone_save       // eff: replace zone save dsc with input
   (DS_zone_save *         // in : Modified save dsc of zone
      zone_save) ;         //      stored/deleted with ATTRIB

  virtual void             // bas: class ATTRIB_DSCSTRN virtual function
   Fixup_src_data(ATTRIB_DM2ACIS *);     // eff: resets src_data pointers to appropriate
                           //      ENTITIES.
                           // use: Making edges tolerant moves the ATTRIB_DSCSTRN's
                           //      so the src_data pointers need to be set.

  // ATTRIB_AREA_CSTRN implementations of ATTRIB virtual functions

  virtual void             // bas: class ATTRIB virtual function
   split_owner             // eff: delete the attribute
   (ENTITY *)              // in : the new entity being created
                           { lose() ; }

  virtual void             // bas: class ATTRIB virtual function
   merge_owner             // eff: delete the attribute
   (ENTITY *,              // in : other = the merge's 2nd topology entity
    logical )              // in : deleted = True=this_entity to be deleted
                           //                False=other_entity to be deleted
                           { lose() ; }

  virtual void             // bas: class ATTRIB virtual function
   trans_owner             // eff: affine transformation callback
   (SPAtransf const& /*xform*/)   // in : the affine transformation
                           { }

  /**
   * @nodoc
   */
  ATTRIB_FUNCTIONS(ATTRIB_AREA_CSTRN, NONE)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

} ; // end class ATTRIB_AREA_CSTRN


#endif // DS_CSATTRB_H
