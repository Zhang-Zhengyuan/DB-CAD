/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dscstrn.hxx
// created  : March 4, 1995
// author   : George Celniker
// contains : classes DS_cstrn
//                    DS_crv_cstrn
//                    DS_pt_cstrn
//                    DS_link_cstrn 
//                    DS_area_cstrn       
//                    DS_tan_pt_cstrn    // obsolete
//
//            global functions
//            DS_sub_CP_from_d()
//            DS_get_curve_seg_bnds()
//            DS_call_src_CW_func()
//            DS_scale_s_pts_for_src_dmod()
//
// synopsis : Representation for constraints in the SPAmatrix 
//            minimization problem 1/2 xAx - bx. (solve Ax = b)
//            Subject to constraints Cx = d                   
//
//            Each constraint is one or more linear equations 
//            in x that eventually become one or more rows in 
//            the Cx = d equation set.  Constraints are stored
//            independently and used to create a C SPAmatrix for
//            processing.  Solving the SPAmatrix problem requires that
//            the C SPAmatrix be modified losing the individuality
//            of each constraint.  Only enough constraint data
//            is stored in a DS_cstrn object to generate the 
//            the C SPAmatrix.
// 
//            Multisurface. When a link_constraint's behavior
//            is to link two dmods so that deform as one,
//            then that link_constraints equations move to the
//            joint problem.  When a link_constraint's behavior
//            is to constrain a curve on one or both of its dmods,
//            then its constraint equations are added to the
//            CX = d equation set for the dmods.
//
//            Area constraints divide the surface into two regions
//            an area that can move and an area that can't.
//            Area constraints are currently enforced by
//            separating the constrained DS_pfunc's dofs into
//            two groups, fixed and free.  The fixed dofs are
//            removed from the problem's matrices as they are 
//            being built.  Alternatively, in the future it
//            may be of interest to add these constraints as
//            simple dof = value equations to the Cx=d equation
//            set.  Multiple area constraints are treated by
//            taking the union of the zones within the constrained
//            DS_pfunc that can move.

#ifndef DS_cstrn_H
#define DS_cstrn_H


#include "dsstdef.hxx" // Define NULL, DS_assert() // Define NULL, DS_assert()
#include "dsblmem.hxx" // DS_copy_double_block(), ... // DS_copy_double_block(), ...
//#include "dshusk/dskernel/dsblock.hxx" // DS_dbl_block()
#include "dsgauss.hxx" // DS_linear_gauss_pt_count(),gauss consts // DS_linear_gauss_pt_count(),gauss consts
#include "dmerror.hxx" // error number macros // error number macros
#include "dspfunc.hxx" // class DS_pfunc // class DS_pfunc
                                       // enum DS_EVAL
#include "dsemech.hxx" // enum DM_xtan_trans // enum DM_xtan_trans
#include "dmapinum.hxx" // enum DM_flipped_state et al. // enum DM_flipped_state et al.
//
#include "dsmbrmt.hxx"   // class DS_mbvec_row_matrix   // class DS_mbvec_row_matrix
#include "dsbcmat.hxx"   // class DS_block_clm_matrix   // class DS_block_clm_matrix
#include "dmicon.hxx"               // Class DM_icon               // Class DM_icon
// #include "dshusk/dskernel/dseqns.hxx"  // DS_eqns
// forward references
class DS_dmod ;                        // deformable model class
class DS_zone ;                        // domain sub-region class
#ifdef DS_WN
class DS_symeq ;                       // holder of Ax=b, Cx=d eqns
#endif //DS_WN
class DS_eqns  ;                       // holder of Ax=b, Cx=d eqns
class DS_enforcement_mechanism;
class Spatial_abs_hurler;
class DS_abs_CW_server;
//typedef struct _iobuf FILE;          // stdio.h streams

//============================================================
// Global convenience function declarations
//============================================================

int                        // rtn: 0=success, else -1=bad input
DS_get_curve_seg_bnds      // eff: turn inputs into seg_bnds data
  (DS_dmod *dmod,          // in : dmod being constrained
   DS_pfunc *src_C_pfunc,  // in : C(s)=[u(s),v(s)]
   void (*src_CW_func)     // opt: user given src_crv description
      (void *src_data,       //     in : src_data for eval
       double s,             //     in : curve s param value
       double *C,            //     out: surface C=[u,v] point value
       double *Cs,           //     out: surface dC/ds vec in surf for s
       double *W,            //     out: image space W=[x,y,z] point val
       double *Wu,           //     out: image space dW/du for s
       double *Wv,           //     out: image space dW/dv for s
       double *Wuu,          //     out: image space d2W/du2
       double *Wuv,          //     out: image space d2W/dudv
       double *Wvv,          //     out: image space d2W/dv2
       double &dist2),       //     out: dist**2(xyz,proj_to_3d(uv))
   void *src_data,         // opt: req data when given src_CW_pfunc
   int &seg_count,         // out: curve in dmod segment count
   int &seg_bnds_count,    // out: seg1_count + 1
   int *&xsect_cont,       // out: continuity of each seg_bnd xsect
                           //      sized:[seg_bnds_count]
   double *&seg_bnds) ;    // out: ordered knot-crossing param vals
                           //      sized:[seg_bnds_count]

void               
DS_call_src_CW_func        // eff: map ACIS curve s params to surface
                           //      C and image_space W pts           
  (void (*src_CW_func)     // opt : func to calc curve C and W pt vals
     (void *src_data,             // in : app data passed to PST_add_crv_cstrn() call
      double s,                   // in : edge s param value
      double *C,                  // out: surface C=[u,v] pt value for s
      double *Cs,                 // out: surface dC/ds vec in surface for s
      double *W,                  // out: image space W=[x,y,z] pt value for s
      double *Wu,                 // out: image space dW/du for s
      double *Wv,                 // out: image space dW/dv for s
      double *Wuu,                // out: image space d2W/du2 for s
      double *Wuv,                // out: image space d2W/dudv for s
      double *Wvv,                // out: image space d2W/dv2 for s
      double &dist2),      // out: dist**2(xyz,proj_to_3d(uv))
   void *src_data,         // in : src data ptr passed to PST_add_crv_cstrn() call
                           //      secretly of type DS_crv2acis
   double domain_scale,    // in : pfunc_space=domain_scale*dmod_space
   double s,               // in : edge s param value
   double C[2],            // out: surface C=[u,v] pt value for s
   double Cs[2],           // out: surface dC/ds vec in surface for s
   double W[3],            // out: image space W=[x,y,z] pt value for s
   double Wu[3],           // out: image space dW/du tangent in u dir
   double Wv[3],           // out: image space dW/dv tangent in v dir
   double Wuu[3],          // out: image space d2W/du2 in u dir
   double Wuv[3],          // out: image space d2W/dudv in u and v dir
   double Wvv[3],          // out: image space d2W/dv2 in  v dir
   double &dist2) ;        // out: image_space dist**2 between curve
                           //      and surface points

double *                   // rtn: scaled s_pts for eval call
DS_scale_s_pts_for_src_dmod// eff: scale cstrn_s_pts for src_dmod
  (DS_dmod *src_dmod,      // in : dmod acting as cstrn src_dmod
   int s_pt_count,         // in : number of s pts
   double *cstrn_s_pts,    // in : s_pts in cstrn domain,
                           //      sized:[s_pt_count
   double *scaled_s_pts) ; // out: s_pts in src_dmod domain,
                           //      sized:[s_pt_count

double *                   // rtn: scaled s_pts for eval call
DS_scale_s_pts_for_src_dmod// eff: scale cstrn_s_pts for src_dmod
  (DS_pfunc *src_C_pfunc,  // in : domain curve
   DS_pfunc *src_W_pfunc,  // in : image curve
   int s_pt_count,         // in : number of s pts
   double *cstrn_s_pts,    // in : s_pts in cstrn domain,
                           //      sized:[s_pt_count
   double *scaled_s_pts) ; // out: s_pts in src_dmod domain,
                           //      sized:[s_pt_count

//==============================================================
// DS_cstrn and its derived class data
//==============================================================

// bmt - moved to dsmapinum.hxx
// type_id enum

// obsolete classes
//              ds_tnc,      // DS_tan_pt_cstrn  derived class identifier
//              ds_dfp,      // DS_dof_pair    derived class identifier
//              ds_dft,      // DS_dof_tangent derived class identifier

// bmt - moved to dsmapinum.hxx
//enum DS_CSTRN_SRC

// bmt - moved to dsmapinum.hxx
// DS_CSTRN behavior bits  (bit values for cst_behavior bit array)
// note: DS_CST_TAN_2_FIXED is overloaded
//  for link-cstrns it indicates the state of one side of the link
//  for pt-cstrns on surfaces it is used for the 2nd tang cstrn


// for convenience (everything but deletable, stopable, and on/off)
// lists of behaviors used in different circumstances
#define DS_CST_CSTRNS \
 (  DS_CST_POS_FREE    | DS_CST_POS_2_FREE    \
  | DS_CST_POS_FIXED   | DS_CST_POS_2_FIXED   \
  | DS_CST_POS_LINKED  | DS_CST_POS_2_LINKED  \
  | DS_CST_TAN_FREE    | DS_CST_TAN_2_FREE    \
  | DS_CST_TAN_FIXED   | DS_CST_TAN_2_FIXED   \
  | DS_CST_TAN_LINKED  | DS_CST_TAN_2_LINKED  \
  | DS_CST_CURV_FREE   | DS_CST_CURV_2_FREE   \
  | DS_CST_CURV_FIXED  | DS_CST_CURV_2_FIXED  \
  | DS_CST_CURV_LINKED | DS_CST_CURV_2_LINKED \
  | DS_CST_NORM_FIXED  | DS_CST_BINORM_FIXED  \
  | DS_CST_CURV_ONLY_LINKED \
  | DS_CST_CURV_ONLY_FIXED \
  | DS_CST_CURV_2_ONLY_LINKED \
  | DS_CST_CURV_2_ONLY_FIXED )
#define DS_CST_PTS_ON_CURVES \
 (  DS_CST_POS_FIXED   | DS_CST_TAN_FIXED     \
  | DS_CST_CURV_FIXED                         \
  | DS_CST_NORM_FIXED  | DS_CST_BINORM_FIXED  )

#define DS_CST_PTS_ON_SURFS \
 (  DS_CST_POS_FIXED   | DS_CST_POS_2_FIXED  \
  | DS_CST_TAN_FIXED   | DS_CST_TAN_2_FIXED  \
  | DS_CST_CURV_FIXED  | DS_CST_CURV_2_FIXED \
  | DS_CST_NORM_FIXED )

#define DS_CST_CURVE_BEHAVIORS \
 ( DS_CST_POS_FREE      \
  | DS_CST_POS_FIXED    \
  | DS_CST_POS_LINKED   \
  | DS_CST_TAN_FREE     \
  | DS_CST_TAN_FIXED    \
  | DS_CST_TAN_LINKED   \
  | DS_CST_CURV_FREE    \
  | DS_CST_CURV_FIXED   \
  | DS_CST_CURV_LINKED  \
  | DS_CST_CURV_ONLY_LINKED \
  | DS_CST_CURV_ONLY_FIXED )

#define DS_CST_LINK_BEHAVIORS \
 ( DS_CST_POS_FREE     | DS_CST_POS_2_FREE    \
  | DS_CST_POS_FIXED   | DS_CST_POS_2_FIXED   \
  | DS_CST_POS_LINKED  | DS_CST_POS_2_LINKED  \
  | DS_CST_TAN_FREE    | DS_CST_TAN_2_FREE    \
  | DS_CST_TAN_FIXED   | DS_CST_TAN_2_FIXED   \
  | DS_CST_TAN_LINKED  | DS_CST_TAN_2_LINKED  \
  | DS_CST_CURV_FREE   | DS_CST_CURV_2_FREE   \
  | DS_CST_CURV_FIXED  | DS_CST_CURV_2_FIXED  \
  | DS_CST_CURV_LINKED | DS_CST_CURV_2_LINKED \
  | DS_CST_CURV_ONLY_LINKED \
  | DS_CST_CURV_ONLY_FIXED \
  | DS_CST_CURV_2_ONLY_LINKED \
  | DS_CST_CURV_2_ONLY_FIXED )

#define DS_CST_AREA_BEHAVIORS \
 ( DS_CST_POS_FIXED )

#define DS_CST_AREA_CSTRNS
   
// an enum for pt_index values (used mainly by DS_pt_cstrn) that
// show up in virtual and local methods.
enum DS_CST_PT_INDEX                   // eff: label pt_cstrn types 
   {                                   //      for value, image_pt, 
                                       //      def, and old access
     PST_UNDEF   = 0,
     PST_BASE_PT = DS_CST_POS_FIXED,   // "position"
     PST_TANG    = DS_CST_TAN_FIXED,   // "tangent"
     PST_TANG1   = DS_CST_TAN_FIXED, 
     PST_TANG2   = DS_CST_TAN_2_FIXED,
     PST_NORM    = DS_CST_NORM_FIXED,  // "normal"
     PST_CURV    = DS_CST_CURV_FIXED,  // "curvature"
     PST_CURV1   = DS_CST_CURV_FIXED,   
     PST_CURV2   = DS_CST_CURV_2_FIXED,
     PST_BINORM  = DS_CST_BINORM_FIXED // "curve binorm" 
   } ;                     // NOTE: changes to this enum must be echo'd in

//===========================================================
// DS_cstrn status bits (bit values for cst_status bit array)
//===========================================================

#define DS_CST_SET_BASE         (1 <<  0) // 1=Base_set by user since last update
#define DS_CST_SET_TANG1        (1 <<  1) // 1=Tang1 set by user since last update
#define DS_CST_SET_TANG2        (1 <<  2) // 1=Tang2 set by user since last update
#define DS_CST_SET_NORM         (1 <<  3) // 1=norm set by user since last update
#define DS_CST_SET_CURV1        (1 <<  4) // 1=curv1 set by user since last update
#define DS_CST_SET_CURV2        (1 <<  5) // 1=curv2 set by user since last update
#define DS_CST_SET_BINORM       (1 <<  6) // 1=binorm set by user since last update

#define DS_CST_ALL_SET_PTS  (  DS_CST_SET_BASE  \
                             | DS_CST_SET_TANG1 \
                             | DS_CST_SET_TANG2 \
                             | DS_CST_SET_NORM  \
                             | DS_CST_SET_CURV1 \
                             | DS_CST_SET_CURV2 \
                             | DS_CST_SET_BINORM)
                                        
#define DS_CST_BASE_PT_TRACKING (1 <<  6) // 1=pt_cstrn base_pt being tracked
#define DS_CST_END_PT_TRACKING  (1 <<  7) // 1=pt_cstrn {vector}_pt being tracked
                                        
// CSTRN STATE bits                     
#define DS_CST_CALC_MAPS        (1 <<  8) // 1=recalc link_cstrn maps
#define DS_CST_CALC_C           (1 <<  9) // 1=recalc link_cstrn C mats
#define DS_CST_CALC_D           (1 << 10) // 1=recalc link_cstrn d mats
                                        
#define DS_CST_CALC_W1          (1 << 11) // 1 recalc pfunc1->W
#define DS_CST_CALC_W2          (1 << 12) // 1 recalc pfunc2->W
#define DS_CST_CALC_WN1         (1 << 13) // 1 recalc pfunc1->Wn
#define DS_CST_CALC_WN2         (1 << 14) // 1 recalc pfunc2->Wn
#define DS_CST_CALC_WNN1        (1 << 15) // 1 recalc pfunc1->Wnn
#define DS_CST_CALC_WNN2        (1 << 16) // 1 recalc pfunc2->Wnn


// behavior notes:
// When a cstrn is DS_CST_STOPABLE it can be turned on and off
//      and its state can be changed between SPAposition, tangent, and
//      SPAposition-tangent.
// When a cstrn is not DS_CST_STOPABLE it can never be turned off.
//      and its state can be changed only between SPAposition and 
//      SPAposition-tangent.
// When a cstrn is DS_CST_DELETABLE it can be removed and deleted
//      from the model, otherwise it can not.
// The DS_CST_TRACKING bits are for undoing bad point constraints
//      changes.  Point constraints are tracked by changing either
//      the base_pt or the tang_vec values.  The system will not
//      know until some later solve time whether the changes have
//      caused a previously redundant constraint to become a
//      conflicting constraint.  The function DS_pt_cstrn->Set_image_pt()
//      calls the functions DS_pt_cstrn->Set_base_pt()
//      and DS_pt_cstrn->Set_end_pt() to set the tracking bits.
//      The functions DS_cstrn->Good_tracking_response() and
//      DS_cstrn->Bad_tracking_response() are called by the solver
//      once it discovers if the constraints are conflicting or not.
//      Those functions clear the tracking bits and if needed copy
//      the old save values back into the current value slots.
//
//// behaviors: Position, Tangent and C2 constraints are all
//      independent but treated in the same manner.
//      Most cstrns affect just one DS_dmod and register
//      their behaviors using the behavior bits,
//          DS_CST_POS_FIXED, 
//          DS_CST_TAN_FIXED, and
//          DS_CST_CURV_FIXED.
//      A set of convenience behavior bit macros, all defined
//      to the value 0, exist just to make the interface complete.
//      These macros include:
//          DS_CST_POS_FREE,  DS_CST_POS_2_FREE,
//          DS_CST_TAN_FREE,  DS_CST_TAN_2_FREE,
//          DS_CST_CURV_FREE, DS_CST_CURV_2_FREE.
// 
//      The DS_link_cstrn constraint affects 2 DS_dmods
//      and also uses the behavior bits,
//          DS_CST_POS_2_FIXED, 
//          DS_CST_TAN_2_FIXED, and
//          DS_CST_CURV_2_FIXED.
//      Make sure that all behavior2 bits are set to free
//      (cleared) for those constraints which affect only
//      one DS_dmod.
// 
//      The use of a constraint on a dmod is determined
//      by the combination of behavior bits which are set.
//      Each constraint has a behavior state for two dmods
//      consisting of FREE, FIXED, or LINKED making for a total
//      of nine distinct behavior states for each constraint.
//      Some of these combinations are symmetrical and
//      other are equivalent and so the total number
//      distinct states is 5 as shown in the table below.
//
//      When a constraint is enforced as a link it is
//      brought into the computation in the joint problem.
//      When a constraint is enforced as a fixed constraint it is
//      brought into the computation in each separate problem.
//      When a constraint is not being enforced it does not
//      affect the solution process.
//      In the table below, each curve (there are 2 per link)
//      is marked as off, crv, or lnk to show where its 
//      terms are used in the solution process.   
//
//      +-----------+-----------------------------+
//      |           |        behavior2            |
//      +-----------+---------+---------+---------+
//      | behavior1 |  FREE   |  FIXED  |  LINKED |
//      +-----------+---------+---------+---------+
//      |   FREE    | off-off | off-fix | lnk-lnk |
//      +-----------+---------+---------+---------+
//      |   FIXED   | fix-off | fix-fix | fix-fix |
//      +-----------+---------+---------+---------+
//      |   LINKED  | lnk-lnk | fix-fix | lnk-lnk |
//      +-----------+---------+---------+---------+
//
//      The mapping of this table is captured in the
//      Cstrn_use() method.
//
// STATE: The behavior bit array is also used to store state
//        information for each constraint as needed.  This 
//        state information keeps track of what information
//        may need to be computed with each call.  Different
//        constraints use the state array differently. 

// tang_gain and tan_display_gain are two scalar values that modify the
// magnitude of the tang_vector constraint.  tang_gain is used
// to scale the SPAvector, which is used as the constraint's target value.
// This term is used instead of just changing the SPAvector for
// seam constraints.  With this term a tracking-seam constraint
// can be made G0 instead of C0.  Each time the constraint is tracked
// the tang_vec is recalculated from its parent's shape.  But the
// tang_gain term remains persistent through that update.
//
// gwc:modified, the tang_gain trick did not work robustly and
// has been disabled.  tang_gain should always be set to 1.0,
// until we figure out how to fix the problem.
// 
// TAN_DISPLAY_GAIN, C2_DISPLAY_GAIN:
// The tan_display_gain and C2_display_gain values are used to 
// scale the plotting of tangent and curvature vectors.  These
// values are used for display purposes only and do not change the
// constraint target values that get built into the d SPAmatrix.
// 
// Often times the actual tangent and curvature vectors will be 
// too large or too small to be used effectively as an interactive 
// handle for mouse pick and drag editing.  The display gains vary
// the actual {vec}_pt values stored (so that picking will work in the 
// normal way.)
// They do not affect the stored value of {vec}_vec.  The
// tan_display_gain and C2_diplay_gain values are not stored so 
// each call to a function which recomputes the relationship
//   tang_pt = base_pt + tan_display_gain * tang_vec and
//   curv_pt = base_pt + C2_display_gain  * curv_vec,
// has tan_display_gain and C2_display_gain as some of their 
// arguments. The tan_display_gain does not affect any of the 
// default positions.

// LIST MANAGEMENT:
// cstrns exist on a single linked list whose head is stored 
// within a DS_dmod.
// Multisurface. The constraint is stored on two 
// different linked lists.  Each DS_cstrn function that
// iterates over the linked list requires a pointer to the
// DS_dmod to determine which list to use.  By default
// everything iterates off the next1 list.  Only lnk_cstrns
// ever use the next2 list.

//=============================================================
//  class DS_cstrn
//=============================================================

class DS_cstrn : public ACIS_OBJECT   // identifier = cst
{
  protected:
   DM_icon* cst_icon;        // cstrn icon
   int cst_tightness ;       // tightness flag: 1 ==> tighten constraints (pre 6.2 behavior)
                             //                 0 ==> do not tighten constraints
   DS_CST cst_type_id ;      // enum showing specialization type
   int cst_tag ;             // input identifying tag or error code
   DS_CSTRN_SRC cst_src_type;// Records cstrn origin. oneof:
                             //    ds_solid_cstrn=shared topo bndry
                             //    ds_bound_cstrn=unshared topo bndry
                             //    ds_user_cstrn  =user created cstrn
                             //    ds_seam_cstrn  =join 2 hierarchical faces
                             //    ds_link_cstrn  =join 2 dmesh faces
   int cst_C_row_count;    // number of non_min_Cd Cx=d rows for SPAposition constraint
   int cst_Cn_row_count;   // number of non_min_Cd Cx=d rows for tangent constraint 
   int cst_Cnn_row_count ; // number of non_min_Cd Cx=d rows for curvature constraint
                           // note: the row_count variables have 
                           //       evolved into convenience members
                           //       for derived classes.
                           //       Actual run-time row counts are now
                           //       computed in the virtual functions,
                           //       Line_row_count() and mix_count()
                           //       and indirectly through the
                           //       dof_maps in crv_cstrn and link_cstrn.
                           //       pt_cstrns - don't use them
                           //       crv_cstrns - use them for size_arrays
                           //       link_cstrns - use them for size_arrays
                           //       area_cstrn - stores and uses C_row_count
//   double cst_tang_gain ;  // vary the src_tang constraint magnitudes

   int    cst_behavior ;   // or of: persistent state bits
                           //        DS_CST_DELETABLE=may be deleted
                           //        DS_CST_STOPABLE =may be disabled
                           //        DS_CST_ON_OFF   =is enabled
                           //
                           //        DS_CST_POS_FREE    DS_CST_POS_2_FREE      
                           //        DS_CST_POS_FIXED   DS_CST_POS_2_FIXED     
                           //        DS_CST_POS_LINKED  DS_CST_POS_2_LINKED    
                           //       
                           //        DS_CST_TAN_FREE    DS_CST_TAN_2_FREE  
                           //        DS_CST_TAN_FIXED   DS_CST_TAN_2_FIXED  
                           //        DS_CST_TAN_LINKED  DS_CST_TAN_2_LINKED
                           //
                           //        DS_CST_CURV_FREE   DS_CST_CURV_2_FREE  
                           //        DS_CST_CURV_FIXED  DS_CST_CURV_2_FIXED  
                           //        DS_CST_CURV_LINKED DS_CST_CURV_2_LINKED
                           //
                           //        DS_CST_NORM_FIXED
                             
   int    cst_status ;     // or of: non persistent state bits
                           //        DS_CST_SET_BASE  =Base_set by user 
                           //        DS_CST_SET_TANG1 =Tang1_set by user
                           //        DS_CST_SET_TANG2 =Tang2 set by user
                           //        DS_CST_SET_NORM  = norm set by user
                           //        DS_CST_SET_CURV1 =curv1 set by user
                           //        DS_CST_SET_CURV2 =curv2 set by user
                           //        DS_CST_SET_BINORM=binorm set by user
                           //        DS_CST_BASE_PT_TRACKING=base_pt is being tracked
                           //        DS_CST_END_PT_TRACKING=tang_pt is being tracked
                           //
                           //        DS_CST_CALC_MAPS  DS_CST_CALC_W1  
                           //        DS_CST_CALC_C     DS_CST_CALC_W2  
                           //        DS_CST_CALC_D     DS_CST_CALC_WN1 
                           //                          DS_CST_CALC_WN2 
                           //                          DS_CST_CALC_WNN1
                           //                          DS_CST_CALC_WNN2

   DS_cstrn *cst_next1 ;   // a singly linked list
   DS_cstrn *cst_next2 ;   // a 2nd singly linked list for lnk_cstrns
   DS_dmod  *cst_dmod1 ;   // constrained dmod
   DS_dmod  *cst_dmod2 ;   // link_cstrn=other_dmod, crv_cstrn=parent_dmod
   void *cst_src1_data ;   // Application data passed to lct_src_CW_func 
   void *cst_src2_data ;   // callback.
   DS_abs_CW_server *cst_CW_server;

                           //
                      
  public:
   virtual DM_icon*
     Get_icon() const;
   virtual void 
     Set_icon(DM_icon*);
   virtual void
     Set_icon_owner(DS_dmod*, int tag);
   virtual void
     Notify_icon_tag_obj_change();
   virtual void
     Draw(const DM_icon_draw_args& args)const;
   virtual void
     Setstate(const DM_icon_cmd_args& args);
   virtual void 
      Query(DM_icon_query_args& args) const;
   // DS_cstrn Type_id, Src_type, constructor, copy, Make_copy, destructor
   DS_CST Type_id()        // Type_id access
                           { return cst_type_id ; }

   DS_CSTRN_SRC            // rtn: ds_solid_cstrn, ds_bound_cstrn,
     Src_type()            //      ds_user_cstrn,  ds_seam_cstrn
                           //      ds_link_cstrn,  ds_undef_cstrn
                           { return cst_src_type ;
                           }

  void *Src_data           // rtn: application data ptr
     (DS_dmod *dmod)       // in : for this dmod 
                           { //DS_assert(dmod) ;
                             return
                             (  (dmod == cst_dmod1) ? cst_src1_data 
                              : (dmod == cst_dmod2) ? cst_src2_data
                              :                       NULL) ;
                           }

  virtual void *Get_passthrough_src_data(DS_dmod *dmod);
  virtual DS_abs_CW_server *Get_CW_src_data(DS_dmod *dmod);

  virtual void Take_CW_src_data(DS_abs_CW_server *&sd);
 
  

  void                     // req: dmod1 and dmod2 are set
   Set_src_data            // eff: store application data ptr
   (void *src_data,        // in : ptr to data that built link_cstrn
    DS_CSTRN_SRC src_type, // in : records cstrn origin. oneof:
                           //     ds_solid_cstrn=shared topo bndry
                           //     ds_bound_cstrn=unshared topo bndry
                           //     ds_user_cstrn  =user created cstrn
                           //     ds_link_cstrn  =multi-surf cstrn
                           //     ds_seam_cstrn  =join 2 hierarchical faces
    DS_dmod *dmod);         // in : for this dmod

   int Tag()               // rtn: cstrn tag value
                           { return cst_tag ; }

   void Set_tag(int tag);   // eff: set the tag value

   DS_cstrn                // default constructor
    (DS_CST type_id,       // in: derived class identifier
     int tag = -1,         // in: input identifier
     DS_CSTRN_SRC Src_type=// in: oneof ds_solid_cstrn, ds_bound_cstrn,
        ds_user_cstrn,     //           ds_user_cstrn,  ds_seam_cstrn
                           //           ds_link_cstrn,  ds_undef_cstrn 
     void *src1_data=NULL, // in: app pointer used by all cstrns
     void *src2_data=NULL, // in: app pointer used by link_cstrns
     int row_C_count = 0,  // in: number of Cx=d rows for SPAposition constraint
     int row_Cn_count = 0, // in: number of Cx=d rows for tangent constraint
     int row_Cnn_count = 0,// in: number of Cx=d rows for curvature constraint
     int behavior=         // in: how constraint interacts with the model    
      (  DS_CST_STOPABLE   // in: orof DS_CST_STOPABLE  (may be disabled)   
       | DS_CST_DELETABLE  //          DS_CST_DELETABLE (may be deleted)    
       | DS_CST_ON_OFF     //          DS_CST_ON_OFF    (1=cstrn is on,0=off)       
       | DS_CST_POS_FIXED),//          PST_POS_FREE     PST_TAN_FREE    
                           //          PST_POS_FIXED    PST_TAN_FIXED   
                           //          PST_POS_LINKED   PST_TAN_LINKED  
                           //                                     
                           //          PST_POS_2_FREE   PST_TAN_2_FREE  
                           //          PST_POS_2_FIXED  PST_TAN_2_FIXED  
                           //          PST_POS_2_LINKED PST_TAN_2_LINKED
     double tang_gain=1.0, // in : gain applied to stored tang vecs
     DS_dmod  *dmod1=NULL, // in : dmod for linked list1
     DS_cstrn *next1=NULL, // in : linked list pointer
     DS_dmod  *dmod2=NULL, // in : dmod for linked list 2
     DS_cstrn *next2=NULL);// in : 2nd linked list pointer for lnk_cstrns
                          
   DS_cstrn                // copy constructor
    (DS_cstrn &cstrn) ;    // in : object to copy deeply

   DS_cstrn &operator=     // assignment operator
    (DS_cstrn &cstrn) ;

   virtual DS_cstrn*       // Virtual Make_copy (public function)
    Make_copy()=0 ;        // copies all members in the linked list

   void
    Size_arrays            // Size_arrays
    (int grobj_count=0) ;  // number of graphic object pointers

  void                     // com: a destructor with an argument
  Destruct_list            // eff: delete all list members with
                           //      special handling for link_cstrns
   (DS_dmod *dmod) ;       // in : for this dmod

   virtual                 // bas: DS_cstrn Virtual destructor
  ~DS_cstrn();              // eff: delete one cstrn - to delete the 
                           //      list use Destruct_list().

   void Print              // eff: output a written cstrn report
     (FILE *file,          // in : stream to receive report
      DS_dmod *dmod) ;     // in : for this dmod 
                           // note: not for use with the ACIS
                           //       integration     

  int Is_seam() const      { return (cst_src_type==ds_seam_cstrn); }
  // DS_cstrn simple data access for items and list
  int Is_tight() const     { return cst_tightness; }

  void Set_tight_on()      { cst_tightness = 1; }

  void Set_tight_off()     { if (cst_src_type!=ds_seam_cstrn) cst_tightness = 0; }

  int Is_dmod1             // rtn: 1=dmod is dmod1;0=not
    (DS_dmod *dmod)        { return (cst_dmod1 == dmod ? 1 : 0) ; }
                           
  int Is_dmod2             // rtn: 1=dmod is dmod1;0=not
    (DS_dmod *dmod)        { return (cst_dmod2 == dmod ? 1 : 0) ; }

  int Dmod_tgt             // rtn: input dmod tgt number or 0
    (DS_dmod *dmod)        // in : for this dmod
                           { return (  dmod == NULL      ? 0
                                     : dmod == cst_dmod1 ? 1
                                     : dmod == cst_dmod2 ? 2
                                     : 0 ) ;
                           }
                           
  DS_dmod *Dmod            // rtn: cstrn dmod by target number
    (int tgt)              // in : 1=cst_dmod1, 2=cst_dmod2   
                           { DS_assert(   tgt == 1
                                       || tgt == 2) ;
                             return(  tgt == 1 ? cst_dmod1
                                    : tgt == 2 ? cst_dmod2
                                    : NULL ) ;
                           }                         
  DS_dmod *Dmod1()         { return cst_dmod1 ; }
  DS_dmod *Dmod2()         { return cst_dmod2 ; }
  DS_dmod *Other_dmod      // rtn: the other stored DS_dmod
   (DS_dmod *dmod)         // in : for this dmod
                           { DS_assert(dmod) ;
                             return
                             (  (dmod == cst_dmod1) ? cst_dmod2
                              : (dmod == cst_dmod2) ? cst_dmod1
                                                    : NULL) ;
                           }

  DS_pfunc *Pfunc          // rtn: dmod's pfunc or NULL
   (DS_dmod *dmod) ;       // in : for this dmod

  void 
   Clear_dmod_reference    // eff: set dmod reference to NULL
     (DS_dmod *dmod)       // in : for this dmod
                           { if(dmod == cst_dmod1)cst_dmod1 = NULL;
                             if(dmod == cst_dmod2)cst_dmod2 = NULL;
                           }

  int Cd_line_row_count    // ret: total pt_cstrn Cx=d row count
   (DS_dmod *dmod) ;       // in : for this dmod
                           // Req: call made from list's 1st cstrn.

  int LLc_row_count        // ret: total link_list Lxi=Lc row count
   (DS_dmod *dmod) ;       // in : for this dmod
                           // Req: call made from list's 1st cstrn.

private:
  virtual                  // bas: DS_cstrn pure virtual function     
   int Line_row_count      // rtn: total non_min_Cd Cx=d row count
    (DS_dmod *dmod)=0 ;    // in : for this dmod 

  virtual                  // bas: DS_cstrn pure virtual function
   int Mix_count           // rtn: Lxi=Lc row count for one cstrn only
   (DS_dmod *dmod)=0;      // in : dmod = for this dmod
public:

/* // unused function
  int Cd_row_count         // ret: total link_list Cx=d row count
   (int min_Cd_row_count,  // in : size of min_Cd array
    DS_dmod *dmod) ;       // in : for this dmod
                           // Req: call made from list's 1st cstrn.
*/ // end unused function

                           // rtn: 0=constraint not used     
  DECL_DM int                      //      1=cstrn applied to dmod
  Cstrn_use                //      2=cstrn applied to dmesh (lnk_cstrn)
   (int cstrn_type,        // in : DS_CST_POSITION 
                           //      DS_CST_TANGENT  
                           //      DS_CST_CURVATURE
    DS_dmod *dmod,         // in : for this dmod 
    int refine_flag=0) ;   // in : 0=use state bits as is
                           //      1=treat links as fixed

  int                      // ret: number of linked elems starting
   List_length             //      with this one.
   (DS_dmod *dmod) ;       // in : for this dmod


  DS_cstrn *               // ret: Parent cstrn when deep in list or
   Is_cstrn_in_list        //      Null when 1st or not in list
    (DS_cstrn *tgt_cstrn,  // in : Cstrn to look for 
     int &in_listP,        // out: 1=in list,0=not_in_list
     DS_dmod *dmod);       // in : for this dmod 

  int Dmod_index           // ret: dmod_index == 1 or 2 or -1 for err
    (DS_dmod *dmod)        // in : for this dmod 
                           { return(  (dmod == cst_dmod1) ?  1
                                    : (dmod == cst_dmod2) ?  2
                                                          : -1) ;
                           }

  DS_cstrn *               // rtn: next cstrn on the list
   Next                    // eff: switch on dmod value
    (DS_dmod *dmod)        // in : for this dmod 
                           { DS_assert(dmod) ;
                             return
                             ( (dmod == cst_dmod1) ? cst_next1 
                              :(dmod == cst_dmod2) ? cst_next2 
                                                   : NULL) ; 
                           } // end DS_cstrn::Next()

  void
   Set_next                // eff: set the next pointer in this cstrn
    (DS_cstrn *cstrn,      // in : future next cstrn
     DS_dmod  *dmod,       // in : for this dmod 
     int dmod_index=1)     // in : which dmod (for lnk_cstrns)
                           { DS_assert(   dmod_index == 1
                                       || dmod_index == 2) ;
                             if(dmod_index == 2){cst_next2 = cstrn ; 
                                                 cst_dmod2 = dmod ;
                                                }
                             else               {cst_next1 = cstrn ; 
                                                 cst_dmod1 = dmod ;
                                                }
                           } // end DS_cstrn::Set_next()

  virtual DS_enforcement_mechanism*
   Mech() {return NULL;}

  int                      // ret  : allowed changes for cstrn
   Behavior_bits()         // or of: DS_CST_DELETABLE= may be deleted
                           //        DS_CST_STOPABLE = may be disabled
                           //        DS_CST_ON_OFF   =is enabled
                           //
                           //        DS_CST_POS_FREE    DS_CST_POS_2_FREE      
                           //        DS_CST_POS_FIXED   DS_CST_POS_2_FIXED     
                           //        DS_CST_POS_LINKED  DS_CST_POS_2_LINKED    
                           //       
                           //        DS_CST_TAN_FREE    DS_CST_TAN_2_FREE  
                           //        DS_CST_TAN_FIXED   DS_CST_TAN_2_FIXED  
                           //        DS_CST_TAN_LINKED  DS_CST_TAN_2_LINKED
                           //
                           //        DS_CST_CURV_FREE   DS_CST_CURV_2_FREE  
                           //        DS_CST_CURV_FIXED  DS_CST_CURV_2_FIXED  
                           //        DS_CST_CURV_LINKED DS_CST_CURV_2_LINKED
                           //
                           //        DS_CST_NORM_FIXED
                           { return cst_behavior ; 
                           }

  virtual void             // base: DS_cstrn virtual function
   Set_coupled_behaviors   // eff: put cst_behavior into legal state
    ()                     {  // unstopable cstrns are always SPAposition cstrns
                              if(!Is_stopable()) 
                                cst_behavior |= DS_CST_POS_FIXED ;

                              // curvature cstrns are always tang cstrn
                              if(cst_behavior & DS_CST_CURV_FIXED)
                                cst_behavior |= DS_CST_TAN_FIXED ;

                              if(cst_behavior & DS_CST_CURV_2_FIXED)
                                cst_behavior |= DS_CST_TAN_2_FIXED ;

                              // curvature links are always tang links
                              if(cst_behavior & DS_CST_CURV_LINKED)
                                cst_behavior |= DS_CST_TAN_LINKED ;

                              if(cst_behavior & DS_CST_CURV_2_LINKED)
                                cst_behavior |= DS_CST_TAN_2_LINKED ;
                              // set curvature-only internal bits
                              // no need to erase external curvature-only bits
                              if (cst_behavior & DS_CST_CURV_2_ONLY_LINKED) {
                                cst_behavior |= DS_CST_CURV_2_LINKED;
                              }
                              if (cst_behavior & DS_CST_CURV_ONLY_LINKED) {
                                cst_behavior |= DS_CST_CURV_LINKED;
                              }
                              if (cst_behavior & DS_CST_CURV_2_ONLY_FIXED) {
                                cst_behavior |= DS_CST_CURV_2_FIXED;
                              }
                              if (cst_behavior & DS_CST_CURV_ONLY_FIXED) {
                                cst_behavior |= DS_CST_CURV_FIXED;
                              }
                           }

  int                      // ret : current state for cstrn
   Status_bits()           // orof:  DS_CST_SET_BASE =Base_set by user 
                           //        DS_CST_SET_TANG1 =Tang1_set by user
                           //        DS_CST_SET_TANG2 =Tang2 set by user
                           //        DS_CST_SET_NORM  = norm set by user
                           //        DS_CST_SET_CURV1 =curv1 set by user
                           //        DS_CST_SET_CURV2 =curv2 set by user
                           //        DS_CST_SET_BINORM=binorm set by user
                           //        DS_CST_BASE_PT_TRACKING=base_pt is being tracked
                           //        DS_CST_END_PT_TRACKING=tang_pt is being tracked
                           //
                           //        DS_CST_CALC_MAPS  DS_CST_CALC_W1  
                           //        DS_CST_CALC_C     DS_CST_CALC_W2  
                           //        DS_CST_CALC_D     DS_CST_CALC_WN1 
                           //                          DS_CST_CALC_WN2 
                           //                          DS_CST_CALC_WNN1
                           //                          DS_CST_CALC_WNN2
                           { return cst_status ;
                           }

  int Is_deletable()       // ret: 1=cstrn can be deleted,0=can not
                           { return(cst_behavior&DS_CST_DELETABLE? 1 : 0) ;}
  int Is_stopable()        // ret: 1=cstrn can be turned off,0=can not
                           { return(cst_behavior&DS_CST_STOPABLE ? 1 : 0) ;}
  int Is_on()              // ret: 1=cstrn is on,0=cstrn is off
                           { return(cst_behavior&DS_CST_ON_OFF   ? 1 : 0) ;}
  int Is_off()             // ret: 1=cstrn is off,0=cstrn is on
                           { return(cst_behavior&DS_CST_ON_OFF   ? 0 : 1) ;}

  int Enabled_crv_cstrn    // rtn: 1=crv_cstrns are in list, 0=no
   (DS_dmod *dmod) ;       // in : for this dmod 

  virtual                  // bas: DS_cstrn virtual function
   int                     // rtn: 1=changes allowed,0=not allowed,-1=bad_dpts
   Set_on_off              // eff: When allowed, Turn cstrn on or off
    (int on_off,           // in : 0=off,1=on
     int pfunc_state=0) ;  // orof: DS_PFN_CHANGED_X      = X val change
                           //       DS_PFN_CHANGED_P_ON   = Set def shape
                           //       DS_PFN_CHANGED_P_OFF  = Clear def shape
                           //       DS_PFN_CHANGED_DOMAIN = Domain change
                           //       DS_PFN_CSTRN_ON = crv_cstrn turned on
                           //       DS_PFN_TRACKING = crv_cstrn tracking parent

//=====================================================
// Is_position, Is_postion_linked,
// Is_tang1, Is_tang2, Is_tangent_linked,
// Is_curvature, Is_curvature_linked
//=====================================================

// these funtions query the behavior of the cstrn and
// the answer is independent of the on/off state of the cstrn.
// these functions are used a little for graphics and
// mainly as convenience routines within the class.
// Except for the graphics needs they would be private functions.

  int Is_position          // ret: 1=cstrn is a SPAposition constraint,0=not
    (DS_dmod *dmod,        // in : for this dmod 
     int refine_flag=0)    // in : 0=use state bits as is
                           //      1=treat links as fixed
                           { return(  1==Cstrn_use(DS_CST_POSITION,
                                                   dmod,
                                                   refine_flag) 
                                    ? 1
                                    : 0) ;
                           }
  int Is_tangent           // ret: 1=cstrn is a tangent cstrn,0=not
    (DS_dmod *dmod,        // in : for this dmod 
     int refine_flag=0)    // in : 0=use state bits as is
                           //      1=treat links as fixed
                           // note: for pt_cstrns on surfaces return
                           //       1 for DS_CST_TAN_2_FIXED or
                           //             DS_CST_TAN_FIXED bits.
                           { return(  1==Cstrn_use(DS_CST_TANGENT,
                                                   dmod,
                                                   refine_flag) 
                                    ? 1
                                    : 0) ;
                           }

  int Is_normal()          // ret: 1=cstrn is normal cstrn,0=not
                           // note: currently only used for pt_cstrns 
                           //       on surfaces
                           { return( (cst_behavior&DS_CST_NORM_FIXED)
                                    ? 1 
                                    : 0) ;
                           }  

  int Is_curvature         // ret: 1=cstrn is a curvature cstrn,0=not
    (DS_dmod *dmod,        // in : for this dmod 
     int refine_flag=0)    // in : 0=use state bits as is
                           //      1=treat links as fixed
                           { return(  1==Cstrn_use(DS_CST_CURVATURE,
                                                   dmod,
                                                   refine_flag) 
                                    ? 1
                                    : 0) ;
                           }
  int Is_position_linked   // ret: 1=cstrn is a pos link-cstrn,0=not
    (DS_dmod *dmod)        // in : for this dmod 
                           { return(  2==Cstrn_use(DS_CST_POSITION,dmod) 
                                    ? 1
                                    : 0) ;
                           }
  int Is_tangent_linked    // ret: 1=cstrn is a tangent link-cstrn,0=not
    (DS_dmod *dmod)        // in : for this dmod 
                           { return(  2==Cstrn_use(DS_CST_TANGENT,dmod) 
                                    ? 1
                                    : 0) ;
                           }
  int Is_curvature_linked  // ret: 1=cstrn is a curvature link-cstrn,0=not
    (DS_dmod *dmod)        // in : for this dmod 
                           { return(  2==Cstrn_use(DS_CST_CURVATURE,dmod) 
                                    ? 1
                                    : 0) ;
                           }

//========================================================
// Is_Cd_contributor,      Is_LLc_contributor
// Is_dof_map_contributor, Is_min_map_contributor
// Is_joint_Cd_contributor
//========================================================

  virtual                  // bas: DS_cstrn pure virtual function
  int Is_Cd_contributor    // ret: 1=cstrn adds Cd equations,0=doesn't
    (DS_dmod *dmod,        // in : for this dmod 
     int refine_flag=0)=0; // in : 0=use state bits as is
                           //      1=treat links as fixed

  virtual                  // bas: DS_cstrn pure virtual function
  int Is_LLc_contributor   // ret: 1=cstrn adds LLc equations,0=doesn't
    (DS_dmod *dmod)=0 ;    // in : for this dmod 

  virtual                  // bas: DS_cstrn pure virtual function
  int                      // rtn: 1=cstrn affects min_map,0=doesn't
   Is_dof_map_contributor  // eff: area and link cstrns do,
     ()=0 ;                //      pt_cstrns and crv_cstrns do not.

  virtual                  // bas: DS_cstrn pure virtual function
  int                      // rtn: 1=cstrn adds to min_map,0=doesn't
   Is_min_map_contributor  // eff: crv and link cstrns may add,
                           //      pt_cstrns do not.
    (DS_dmod *dmod,        // in : for this dmod 
     int refine_flag=0)=0; // in : 0=use state bits as is
                           //      1=treat links as fixed

  virtual                  // bas: DS_cstrn pure virtual function
  int                      // rtn: 1=cstrn adds joint Cx=d eqns,0=doesn't
   Is_joint_Cd_contributor // eff: link cstrns may add to joint Cx=d
    ()=0 ;

  int                      // ret : deletable and stopable behavior bits
   Rights()                // orof: DS_CST_STOPABLE, DS_CST_DELETABLE
                           { return(cst_behavior & (  DS_CST_STOPABLE
                                                    | DS_CST_DELETABLE)) ;
                           }
  int                      // ret : things constrained when cstrn is "on"
   Behavior()              // orof DS_CST_POS_FREE     DS_CST_POS_2_FREE        
                           //      DS_CST_POS_FIXED    DS_CST_POS_2_FIXED       
                           //      DS_CST_POS_LINKED   DS_CST_POS_2_LINKED      
                           //                                                  
                           //      DS_CST_TAN_FREE     DS_CST_TAN_2_FREE   
                           //      DS_CST_TAN_FIXED    DS_CST_TAN_2_FIXED  
                           //      DS_CST_TAN_LINKED   DS_CST_TAN_2_LINKED 
                           //
                           //      DS_CST_CURV_FREE    DS_CST_CURV_2_FREE  
                           //      DS_CST_CURV_FIXED   DS_CST_CURV_2_FIXED 
                           //      DS_CST_CURV_LINKED  DS_CST_CURV_2_LINKED
                           //
                           //      DS_CST_NORM_FIXED
                           { return(cst_behavior & DS_CST_CSTRNS) ;
                           }

  int                      // rtn : 1=all changes allowed,0=pos must be on
  Set_behavior             // eff : set what the cstrn constrains when "on"
  (double tan_display_gain,// in : tang_pt = base_pt + tan_display_gain*tang_vec
   double C2_display_gain, // in : curv_pt = base_pt + C2_display_gain*curv*tang_val
   int behavior,           // orof DS_CST_POS_FREE     DS_CST_POS_2_FREE        
                           //      DS_CST_POS_FIXED    DS_CST_POS_2_FIXED       
                           //      DS_CST_POS_LINKED   DS_CST_POS_2_LINKED      
                           //                                                  
                           //      DS_CST_TAN_FREE     DS_CST_TAN_2_FREE   
                           //      DS_CST_TAN_FIXED    DS_CST_TAN_2_FIXED  
                           //      DS_CST_TAN_LINKED   DS_CST_TAN_2_LINKED 
                           //
                           //      DS_CST_CURV_FREE    DS_CST_CURV_2_FREE  
                           //      DS_CST_CURV_FIXED   DS_CST_CURV_2_FIXED 
                           //      DS_CST_CURV_LINKED  DS_CST_CURV_2_LINKED
                           //
                           //      DS_CST_NORM_FIXED
   int pfunc_state) ;      // orof: DS_PFN_CHANGED_X      = X val change
                           //       DS_PFN_CHANGED_P_ON   = Set def shape
                           //       DS_PFN_CHANGED_P_OFF  = Clear def shape
                           //       DS_PFN_CHANGED_DOMAIN = Domain change
                           //       DS_PFN_APPROX_SHAPE   = shape changed approximately
                           //       DS_PFN_CSTRN_ON = crv_cstrn turned on
                           //       DS_PFN_TRACKING = crv_cstrn tracking parent

   void                    // use: remember which cstrns are set by user
                           //      and should not be rebuilt by Update_pts()
   Mark_set_status         // eff: mark constraint as user changed
    (int status)           // orof: DS_CST_SET_BASE
                           //       DS_CST_SET_TANG1
                           //       DS_CST_SET_TANG2
                           //       DS_CST_SET_NORM
                           //       DS_CST_SET_CURV1
                           //       DS_CST_SET_CURV2
                           //       DS_CST_SET_BINORM
                           { DS_assert(0 == (  status
                                             & ~DS_CST_ALL_SET_PTS));
                             cst_status |= (  status
                                            & DS_CST_ALL_SET_PTS) ;
                           }

  void                     // eff: remove constraint set state
   Clear_set_status        //      from all cstrns on list
    (DS_dmod *dmod)        // in : for this dmod
                           { DS_assert(dmod) ;
                             DS_cstrn *cstrn = this ;
                             for(;cstrn!=NULL;cstrn=cstrn->Next(dmod))
                              { cstrn->cst_status &= 
                                       ~DS_CST_ALL_SET_PTS ;
                              }

                           }
/*   double  Tang_gain()     // rtn: gain applied to stored tang vecs
                           { return cst_tang_gain ; }
   void Set_tang_gain      // eff: set the tang_gain magnitude
    (double tang_gain)     { cst_tang_gain = tang_gain ;
                           }*/

  DS_cstrn *
   Nearest_to_iline        // ret: closest linked list cstrn to
                           //        iline = pi0 + u(pi1-pi0)
    (double *pi0,          // in : image space pt, Sized:[image_dim]
     double *pi1,          // in : image space pt, Sized:[image_dim]
     double max_dist,      // in : max dist in which to find cstrn
     int seam_flag,        // in : 1=search for seams,0=don't
     DS_CST_PT_INDEX       // out: image_pt index in cstrn for closest pt
            &pt_index,     
     double &dist2,        // out: dist**2 between cstrn and line
     double &uu,           // out: iline param for pt closest to cstrn
     DS_dmod *dmod) ;      // in : for this dmod 

  virtual double           // bas: DS_cstrn virtual function
   Dist2_to_iline          // ret: dist**2 between cstrn and iline
                           //      iline = pi0 + u(pi1-pi0)
    (double *pi0,          // in : image space pt, Sized:[image_dim]
     double *pi1,          // in : image space pt, Sized:[image_dim]
     DS_CST_PT_INDEX       // out: image_pt index in cstrn for closest pt
            &pt_index,     
     double &uu)=0 ;       // out: iline param for pt closest to cstrn
  virtual int              // bas: DS_cstrn virtual function
   Image_dim()=0 ;         // rtn: image dim for constraint

/* unneeded in base class - callers should downcast to DS_pt_cstrn 
  virtual void             // bas: DS_cstrn virtual function
   Set_image_pt            // eff: copies ipt into cstrn's image pt
   (double *ipt,           // in : pt to copy, Sized:Image_dim()
    double tan_display_gain,      // in : tang_pt = base_pt+tan_display_gain*tang_vec
    int pt_index)=0 ;      // in : index of image_pt to update
                           //      0 = update pst_base_pt
                           //      1 = update pst_tang_pt

  virtual                  // bas: DS_cstrn virtual function
   int                     // rtn: 0=success,-1=bad_dpt
   Set_domain_pt           // eff: copies dpt into pst_domain_pt
   (double *dpt,           // in : pt to copy, Sized:Domain_pt_size()
    double tan_display_gain,      // in : tang_pt = base_pt+tan_display_gain*tang_vec
    int image_save_flag)=0;// orof: 1=calc new base_pt
                           //       2=calc new tang_vec
                           //       4=calc new Wuu,Wuv,Wvv vecs
                           //       0=save old base_pt and tang_vec
*/
  virtual int              // bas: DS_cstrn virtual function
   Domain_dim()=0 ;        // rtn: domain dim for constraint

  virtual void             // bas: DS_cstrn pure virtual function
   Shift_domain            // eff: shift the basis domain values
    (double *du,           // in : domain shift SPAvector, Sized:Domain_dim()
     DS_dmod *dmod)=0 ;    // in : for this dmod 

  virtual                  // bas: DS_cstrn pure virtual function
   int                     // rtn: 0=done,-1=scale too small
   Scale_domain            // eff: scale the basis domain values
    (double gain,          // in : domain scale factor
     DS_dmod *dmod)=0 ;    // in : for this dmod 

  // DS_cstrn side_effect functions
  void                     // eff: shift the basis domain values.
   Shift_cstrn_domain      //      iterates through all cstrns on list
    (double *du,           // in : domain shift SPAvector, Sized:Domain_dim()
     DS_dmod *dmod) ;      // in : for this dmod 

   int                     // rtn: 0=done,-1=scale too small
                           // eff: shift the basis domain values.
   Scale_cstrn_domain      //      iterates through all cstrns on list
    (double scale,         // in : domain scale factor
     DS_dmod *dmod) ;      // in : dmod owning cstrn list being traversed

  void                     // eff: update all cstrn cached data
  Update_cstrn_pts         //      after changes in constrained pfunc
  (double tan_display_gain,// in : tang_pt = base_pt + tan_display_gain*tang_vec
   double C2_display_gain, // in : curv_pt = base_pt + C2_display_gain*curv*tang_val
   int pfunc_state,        // orof: DS_PFN_CHANGED_X      = X val change
                           //       DS_PFN_CHANGED_P_ON   = Set def shape
                           //       DS_PFN_CHANGED_P_OFF  = Clear def shape
                           //       DS_PFN_CHANGED_DOMAIN = Domain change
                           //       DS_PFN_CSTRN_ON = crv_cstrn turned on
                           //       DS_PFN_TRACKING = crv_cstrn tracking parent
   DS_dmod *dmod) ;        // in : for this dmod 

  void
   Build_A                 // eff: Add all cstrn terms to A in Ax=b
                           //      Requires call to be made from
                           //      the linked list's first DS_cstrn.
  (DS_eqns *eqns,          // in : holder of Ax=b, Cx=d eqns
   DS_dmod  *dmod) ;       // in : for this dmod 

  void
   Build_b                 // eff: Add all cstrn terms to b in Ax=b.
                           //      Requires call to be made from
                           //      the linked list's first DS_cstrn.
    (DS_eqns *eqns,        // in : holder of Ax=b, Cx=d eqns
     DS_dmod *dmod) ;      // in : for this dmod 

  void                     // eff: build C and d matrices in Cx=d.  
   Build_Cd                //      Requires call to be made from    
                           //      the linked list's first DS_cstrn.
    (DS_eqns *eqns,        // in : holder of Ax=b, Cx=d eqns
     int &next_Cd_row,     // i/o: next available Cx=d row
     DS_dmod *dmod,        // in : for this dmod 
     int refine_flag=0) ;  // in : 0=use state bits as is
                           //      1=treat links as fixed

  void                     // eff: build just the d SPAmatrix in Cx=d. 
   Build_d                 //      Requires call to be made from    
                           //      the linked list's first DS_cstrn.
    (DS_eqns *eqns,        // in : holder of Ax=b, Cx=d eqns
     int &next_Cd_row,     // i/o: next available Cx=d row
     DS_dmod *dmod) ;      // in : for this dmod 

  void                     // eff: build L and Lc matrices in Lxi=Lc.
   Build_L_Lc              //      Requires call to be made from     
                           //      the linked list's first DS_cstrn.
    (DS_eqns *eqns,        // in : holder of Ax=b, Cx=d eqns
     DS_dmod *dmod) ;      // in : for this dmod 


  void                     // eff: build just the Lc SPAmatrix in Lxi=Lc
   Build_Lc                //      Requires call to be made from     
                           //      the linked list's first DS_cstrn.
   (DS_eqns *eqns,         // in : holder of Ax=b, Cx=d eqns
    DS_dmod *dmod) ;       // in : for this dmod 

#ifndef DS_USE_SYMEQ
#else
  int                      // rtn: fixed dof count or -1=error                     
   Mark_fixed_dofs_and_elems// eff: mark all dofs that are fixed
    (int dof_count,        // in : sizeof dof_map
     int *dof_map,         // i/o: dof_map for constrained pfunc
     int &fixed_dof_count, // out: number of fixed dofs
     int elem_count,       // in : sizeof elem_state
     int *elem_state,      // i/o: 0=free to move,1=fixed
     int &fixed_elem_count,// out: number of fixed elems
     DS_dmod *dmod) ;      // in : for this dmod 
#endif //DS_USE_SYMEQ
/* // obsolete unused function     
  double                   // rtn: max residual error
   Check_d                 // eff: Check residual = C(X-P) - d
    (int *cstrn_dof_map,   // in : global to used dof map for cstrns.
                           //      sized:[dmod->Domain_dim()]
     int min_Cd_dof_count, // in : global used_dof count
     DS_dmod *dmod) ;      // in : for this dmod 
*/ // obsolete unused function
  // DS_cstrn virtual side-effect functions
  virtual                  // bas: DS_cstrn virtual function
  int                      // rtn: 0=success,-1=bad_dpts
  Update_pts               // eff: update all cstrn cached data
                           //      after changes in pfunc
  (double tan_display_gain,// in : tang_pt = base_pt + tan_display_gain*tang_vec
   double C2_display_gain, // in : curv_pt = base_pt + C2_display_gain*curv*tang_val
   int pfunc_state)=0;     // orof: DS_PFN_CHANGED_X      = X val change
                           //       DS_PFN_CHANGED_P_ON   = Set def shape
                           //       DS_PFN_CHANGED_P_OFF  = Clear def shape
                           //       DS_PFN_CHANGED_DOMAIN = Domain change
                           //       DS_PFN_TRACKING = crv_cstrn track parent

  virtual void             // bas: DS_cstrn virtual function
   Build_A_row             // eff: build one cstrn's Cx=d eqn rows
    (DS_dmod * /*dmod*/,        // in : shape to constrain
     DS_eqns * /*eqns*/) {}     // in : holder of Ax=b, Cx=d eqns

  virtual void             // bas: DS_cstrn virtual function
   Build_b_row             // eff: build one cstrn's Cx=d eqn rows
    (DS_dmod * /*dmod*/,        // in : shape to constrain
     DS_eqns * /*eqns*/) {}     // in : holder of Ax=b, Cx=d eqns

  virtual void             // bas: DS_cstrn virtual function
   Build_Cd_row            // eff: build one cstrn's Cx=d eqn rows
    (DS_dmod *dmod,        // in : shape to constrain
     DS_eqns *eqns,        // in : holder of Ax=b, Cx=d eqns
     int &next_Cd_row,     // i/o: next available Cx=d row
     int refine_flag=0)=0; // in : 0=use state bits as is
                           //      1=treat links as fixed

  virtual void             // bas: DS_cstrn virtual function
   Build_d_row             // eff: build one cstrn's d in Cx=d eqn
    (DS_dmod *dmod,        // in : shape to constrain
     DS_eqns *eqns,        // in : holder of Ax=b, Cx=d eqns
     int &next_Cd_row)=0 ; // i/o: next available Cx=d row
     
  virtual void             // bas: DS_cstrn virtual function                        
   Build_L_Lc_row          // eff: build one pt-cstrn's Lxi=Lc eqn rows             
    (DS_dmod *dmod,        // in : shape to constrain
     DS_eqns *eqns,        // in : holder of Ax=b, Cx=d eqns
     int next_L_Lc_row)=0; // in : 1st available row of Li xi=Lc

  virtual void             // bas: DS_cstrn virtual function
   Build_Lc_row            // eff: build one pt-cstrn's d of Cx=d eqn
    (DS_dmod *dmod,        // in : shape to constrain
     DS_eqns *eqns,        // in : holder of Ax=b, Cx=d eqns
     int next_L_Lc_row)=0; // in : 1st available row of Li xi=Lc

#ifndef DS_USE_SYMEQ
#else
  virtual                  // bas: DS_cstrn virtual function
  int                      // rtn: free_dof_count, -2=no effect or -1=error                     
   Mark_this_free_dofs     // eff: mark all free dofs and elems in input
   (DS_dmod *dmod,         // in : shape to constrain
    int dof_count,         // in : sizeof dof_map
    int *dof_map,          // i/o: dof_map for constrained pfunc
    int &free_dof_count,   // out: dofs changed from fixed to free
    int elem_count,        // in : sizeof elem_state
    int *elem_state,       // i/o: 0=free to move, 1=fixed
    int &free_elem_count)=0;// out: elems changed from fixed to free
#endif //DS_USE_SYMEQ
  virtual void             // bas: DS_cstrn virtual function
  Bad_track_response       // eff: reset tracked cstrns. 
  (double tan_display_gain,// in : tang_pt = base_pt + tan_display_gain*tang_vec
   double C2_display_gain  // in : curv_pt = base_pt + C2_display_gain *tang_vec
   )=0;                    //      Called when tracking yields cstraint 
                           //      confilcts

  virtual void                     
   Good_track_response     // eff: tell cstrn that tracking succeeded
    ()                     { // clear the cst_status tracking bits 
                             cst_status &= ~(  DS_CST_END_PT_TRACKING  
                                             | DS_CST_BASE_PT_TRACKING) ;
                           }

  virtual                  // bas: DS_cstrn virtual function
  int                      // rtn: 0=success,-1=file failure
   Dump                    // eff: write all internal vals to file
    (char *filename,       // in : filename = file to write
     FILE *fp) ;           // in : existing file or NULL

} ; // end class DS_cstrn

//=============================================================
//  class DS_pt_cstrn
//=============================================================

// DS_pt_cstrn graphics:  The pt_cstrn graphics consists of 15 graphic
//                        objects + 4 * C2_display_count.
//  cst_grobj[ 0] = base_pt                    marker   handle     
//  cst_grobj[ 1] = tang1_pt                   marker   handle 
//  cst_grobj[ 2] = tang1_pt - base_pt         polyline handle   
//  cst_grobj[ 3] = tang2_pt                   marker   handle
//  cst_grobj[ 4] = tang2_pt - base_pt         polyline handle
//  cst_grobj[ 5] = norm_pt                    marker   handle
//  cst_grobj[ 6] = norm_pt - base_pt          polyline handle
//  cst_grobj[ 7] = norm_debug_pt              marker   handle
//  cst_grobj[ 8] = norm_debug_pt - base_pt    polyline handle
//  cst_grobj[ 9] = curv1_pt                   marker   handle
//  cst_grobj[10] = curv1_pt - base_pt         polyline handle
//  cst_grobj[11] = curv2_pt                   marker   handle
//  cst_grobj[12] = curv2_pt - base_pt         polyline handle
//  cst_grobj[13] = binorm_pt - base_pt        polyline handle
//  cst_grobj[14] = binorm_pt                  marker   handel

#define DS_PST_ICON_COUNT 15

//  cst_grobj[15] = domain_dir1 polyline       polyline handle
//  cst_grobj[16] = domain_dir2 polyline       polyline handle
//  cst_grobj[..] = domain_dir1 curvature comb C2_display_count markers +
//                                             C2_display_count polylines
//  cst_grobj[..] = domain_dir2 curvature comb C2_display_count markers +
//                                             C2_display_count polylines
          
// Current ACIS pt_cstrn sources, privledges, and src_data
//
// DS_CSTRN_SRC    delete  stop   src_    comment
//                  able   able   data 
//
// ds_solid_cstrn    NO     NO    YES     pt from solid model bndry
//                                        any number of edges may
//                                        connect to pt
// ds_bound_cstrn    NO    YES    YES     pt from sheet model bndry
//                                        only edge being deformed
//                                        connects to pt
// ds_user_cstrn    YES    YES    NULL    pt from user interface
//                                        and not from topology
//                                        boundaries

// The point constraint is a device to allow an end-user to
// specify the differential geometry properties (SPAposition,
// tangent, normal, binormal, and curvature) of a point
// on a deformable model shape.  The point constraint works
// for surfaces, curves in 3D, and curves in 2D.
// Many but not all combinations of the constraint behaviors
// are allowed.  
//
// A point constraint can control the following geometry properties:
//   Curves:                      Surfaces:
//   Position                     Position 
//   Tangent (velocity)           Tangent1 in domain1_dir direction
//   Normal direction             Tangent2 in domain2_dir direction
//   Binormal direction           Normal direction
//   Curvature+Normal+Tangent     Curvature1+Normal+Tangent1 in domain1_dir direction
//                                Curvature2+Normal+Tangent2 in domain2_dir direction
//
// DIFFERENTIAL GEOMETRY PROPERTIES OF A POINT CONSTRAINT
//
// Every point constraint has a location on the deformable
// model being constrained.  This location is specified by
// a domain point which is [s0] for a curve or [u0,v0] for 
// a surface.  The shape being constrained is given as a
// parametric mapping,
//   W = [x(s),y(s),z(s)] for curves,
//   W = [x(u,v),y(u,v),z(u,v)] for surfaces
//   where: s = the SPAparameter space variable for a curve and
//          u,v = the SPAparameter space variables for a surface.
//
// The constraint point's image space properties are found
// by evaluating the deformable model mapping W at the domain
// space location of the constraint.
//
// The SPAposition is defined as
//   Position =    W(s0)       for curves
//              or W(u0,v0)    for surfaces
//
// The tangent, velocity, acceleration, and curvature values for a
// curve are defined as:
//   Tangent      = normalized(Ws),
//   Velocity     = Ws,
//   Acceleration = Wss,
//   Curvature    = ||cross_product(Ws,Wss)|| / ||Ws||**3
//   Binormal     = normalized(cross(Ws,Wss))
//   Normal       = normalized(cross(binormal,Ws))
//   where: W = [x(s),y(s),z(s)] = the shape being constrained
//          Ws = dW/ds = 1st parametric derivative
//          Wss = d2W/ds2 = 2nd parametric derivative
//
// The tangent, veloctiy, acceleration, and curvature values for a
// surface are a function of both the domain point location (u0,v0)
// and a domain space direction, domain_dir = [du, dv].
// 
//   domain_dir   = [du dv]
//   Wa           = Wu*du + Wv*dv 
//                = velocity in the direction of domain_dir
//   Waa          = Wuu*du*du + 2*Wuv*du*dv + Wvv*dv*dv
//                = acceleration in the direction of domain_dir
//   Tangent      = normalized(Wa)
//   Velocity     = Wa
//   Acceleration = Waa
//   Curvature    = ||cross_product(Wa,Waa)|| / ||Wa||**3
// where: W=[x(u,v),y(u,v),z(u,v)] = the shape being constrained
//        Wu  = dW/du = partial derivative in u direction,
//        Wv  = dW/dv = partial derivative in v direction,
//        Wuu = d2W/du2   = 2nd partial derivative in u direction
//        Wuv = d2W/du dv = 2nd cross partial derivative
//        Wvv = d2W/dv2   = 2nd partial derivative in v direction
//
// The normal and binormal directions for a curve are defined as:
//  
//   Tangent  = normalized(Ws),
//   Binormal = normalized(cross_product(Ws,Wss)),
//   Normal   = cross_product(binormal,tangent).
//
// The normal direction for a surface is defined as,
//
//   normal   = normalized(cross_product(Wu,Wv))
//            = normalized(cross_product(Wa1,Wa2)).
//   where: Wa1 = velocity SPAvector in some direction domain1_dir=[du1,dv1] and
//          Wa2 = velocity SPAvector in some direction domain2_dir=[du2,dv2]. 
//
// CONSTRAINT EQUATIONS FOR POINTS
//
// The dofs in the constraint equations are
// the displacements Q of  W = Q + P. 
// With W = shape [x y z]  W = Wi*Phi = sum_i(Wi * Phi)
//      Q = displacement   
//      P = default shape
//
// and  Ws, Wu, Wv, and Wa are 1st derivatives
//      Wss, Wuu, Wuv, Wvv and Waa are 2nd derivatives 
//
// and  Wi  = ith W dof    (Wsi, Wui, ...)                                
//      Phi = ith basis function
//      Phsi, Phui, Phvi = 1st derivatives of the basis functions                           
//      Phssi, Phuui, Phuvi Phvvi = 2nd derivatives
//
// Position: constrain W so that W = pst_base_pt
//           Phi * Wi = pst_base_pt
//           Phi * Qi = pst_base_pt - Phi * Pi
//                    = pst_base_pt - pst_base_def
//
// Tangent : constrain Ws so that Ws = pst_tang1_val
// curves  : Phui * Wi = pst_tang1_val
//           Phui * Qi = pst_tang1_val - Phui * Pi
//                     = pst_tang1_val - pst_tang1_def
//
// Tang1   : constrain Wa1 so that Wa1 = pst_tang1_val
// surfaces: du1*Wu + dv1*Wv = pst_tang1_val
//          (du1*Phui * dv1*Phvi) * Qi =   pst_tang1_val 
//                                        - (du1*Phui * dv1*Phvi) 
//                                        * Pi 
//                                      =   pst_tang1_val 
//                                        - pst_tang1_def
//
// Tang2   : constrain Wa2 so that Wa2 = pst_tang2_val
// surfaces: du2*Wu + dv2*Wv = pst_tang2_val
//           (du2*Phui * dv2*Phvi) * Qi =   pst_tang2_val 
//                                        - (du2*Phui * dv2*Phvi) 
//                                        * Pi 
//                                      =   pst_tang2_val 
//                                        - pst_tang2_def
//
// Normal  : constrain the normal so that m = pst_norm_val.
// curves  : To set m we need to set the tangent and m
//            with the requirement that dot_product(t,m) = 0
//           Ws = pst_tang_val
//           dot_product(cross(t,m),Wss) = 0
//             dot_product(cross(t,m),Qs+Ps) =
//             dot_product(cross(t,m),Phxi * Qi) = 
//                  -dot_product(cross(t,m),Phsi * Pi)
//           This constraint is set by the values
//           pst_tang1_val and
//           pst_norm_val
// 
// Binormal: constraint the tangent and normal vectors to be
// curves  : in a plane perpendicular to pst_binorm_val
//           dot_product(b,Ws)  = 0
//           dot_product(b,Wss) = 0
//             dot_product(b,Qs+Ps)   = 0
//             dot_product(b,Qss+Pss) = 0 
//               dot_product(b,Phsi*Qi)  = -dot_product(b,Phsi*Pi)
//               dot_product(b,Phssi*Qi) = -dot_product(b,Phssi*Pi) 
//
// Normal  : constrain the normal so that n = pst_norm_val.
// surfaces: dot_product(n,Wu) = 0 and 
//           dot_product(n,Wv) = 0
//
//           dot_product(n,Phui * Qi) = -dot_product(n,Phui * Pi) and 
//           dot_product(n,Phvi * Qi) = -dot_product(n,Phvi * Pi)
//
// curvature: constrain curvature so that k = pst_curv1_val and
// curves   : constrain the osculating plane so that it includes
//            the vectors, pst_tang1_val and pst_norm_val.
// 
//                 ||cross(Ws,Wss)||
//             k = -------------------------
//                     ||Ws||**3
//
//            use tangent constraint so  Ws = pst_tang1_val
//            use normal constraint  so   m = pst_norm_val
//            Then constrain curvature    k = pst_curv1_val
//              by restricting the values of Wuu to 
//                Wuu = k*m + h*Wu   (where h is any value)
//              which is enforced by the pair of constraints.
//            
//            dot_product(Wss-k*m,perp1(Ws)) = 0 
//            dot_product(Wss-k*m,perp2(Ws)) = 0
//
//              which yields
//                dot_product(Qss,perp1(Ws)) = -dot_product(Pss-k*m,perp1(Ws)) 
//                dot_product(Qss,perp2(Ws)) = -dot_product(Pss-k*m,perp2(Ws))
// 			          decompose above to [L0 L1 L2] [x0,x1,x2]t= Lc where
//              [Li =  Phssi    .perp(Ws)[i]] 
//              [Lc = -(Pss-k*m).perp(Ws)   ]
//
// curvature: given a direction in the surface a = [du dv]
// surfaces : constrain curvature in that direction 
//            so that k = pst_curv_val and and constrain the
//            osculating plane in that direction so that it includes
//            the vectors, pst_tang_val and pst_norm_val.
// 
//                 ||cross(Wa,Waa)||
//             k = -------------------------
//                     ||Wa||**3
//
//            use tangent constraint so  Wa = pst_tang_val
//            use normal constraint  so   n = pst_norm_val
//            Then constrain curvature    k = pst_curv_val
//              by restricting the values of Waa to 
//                Waa = k*n + h*Wa   (where h is any value)
//              which is enforced by the pair of constraints.
//            
//            dot_product(Waa-k*n,perp1(Wa)) = 0 
//            dot_product(Waa-k*n,perp2(Wa)) = 0 
//
//            with Wa  = Wu*du + Wv*dv
//                 Waa = Wuu*du*du + 2*Wuv*du*dv + Wvv*dv*dv
//
//            Two such constraints can be supported as long as
//            both constraints share a common normal, n SPAvector.
//            The values for each curvature constraint on a surface
//            are shown below:
//
//            PST_CURV1:               PST_CURV2:
//            pst_domain1_dir,        pst_domain2_dir,
//            pst_tang1_val,          pst_tang2_dir,
//            pst_norm_val,           pst_norm_val,
//            pst_curv1_val           pst_curv2_val
//
// GEOMETRIC CONSTRAINT COMBINATIONS FOR POINT CONSTRAINTS
//
// The SPAposition constraint behavior may be used by itself or with 
// any other constraint combination.
//
// What we call a tangent constraint is really a constraint
// on the velocity of the curve, both the direction and magnitude
// of Ws or Wa are specified and constrained.  The tangent constraint
// may be used by itself or with other constraint combinations,
// The tangent constraint is required when using the curvature
// constraint.
//
// The normal constraint is a constraint on the direction of the
// normal SPAvector, not its magnitude.  The normal constraint
// on surfaces be used by itself or with other constraint combinations.
// The normal constraint on curves must be used in conjunction with
// the tangent constraint.  The normal constraint is required when 
// using the curvature constraint.
//
// The binormal constraint is a constraint on the tangent and
// normal directios for a curve.  The tangent and normal directions
// are restricted to lie within a plane perpendicular to the
// binormal SPAvector.
//
// The curvature constraint is a combination constraint which
// specifies, the tangent, normal direction, and curvature value
// at that point.
//
// A point constraint can apply one tangent, normal, binormal,
// and curvature constraint to a curve but can apply two such 
// constraints to surfaces.  The surface constraints are applied 
// in a direction specified by the end-user at run time.  These 
// directions, called domain1_dir and domain2_dir, default to the 
// u and v directions.
//
// VIEWING AND MODIFYING THE CONSTRAINT PARAMETERS
//
// To help support interactive mouse-based editing of the constraint
// values, a set of constraint image points are maintained in the point
// constraint data structure.  These points define locations and vectors
// from which all the values needed to set the constraints
// may be derived.  These points may be plotted, picked, and
// moved to form the basis of an interactive package for 
// editing these parameters.  These points include:
//
// base_pt = location of the SPAposition constraint and base for all
//           SPAvector values.
// tang1_pt = tan_display_gain * (base_pt + tang1_val)
//            SPAvector end point used to specify the curve tangent
//            and the surface tangent1 constraint vectors as;
// tang2_pt = tan_display_gain * (base_pt + tang2_val)
//            Vector end point used to specify the surface
//            tangent2 SPAvector.
// norm_pt  = tan_display_gain * scale * (base_pt + norm_val)
//            Vector end point used to specify the normal SPAvector
//            direction for a curve or surface.  The normal SPAvector
//            sets the normal direction for the normal constraint 
//            and the curvature constraints.  When a curvature
//            constraint is being used, the norm_pt is not
//            checked for picking or plotting, rather the 
//            curv_pts are used.
//            The scale SPAparameter is stored in the magnitude of the
//            norm_vector as: 
//            scale = magnitude(norm_pt-base_pt) / tan_display_gain
//            and defaults 1.0 and is changed by moving the norm_pt.
// binorm_pt= tan_display_gain * scale * (base_pt + norm_val)
//            The scale SPAparameter is stored in the magnitude of the
//            binorm_vector as: 
//            scale = magnitude(binorm_pt-base_pt) / tan_display_gain
//            and defaults 1.0 and is changed by moving the binorm_pt.
//            
// curv1_pt = C2_display_gain * (base_pt + curv1_vec)
//            where curv1_vec = k1*norm_val
//              k1       = curvature value for curves or for surfaces
//                         in the domain1_dir direction,
//              norm_val = curve or surface unit-sized normal-SPAvector.
//            When in use, the curv1_vec is always perpendicular
//            to the tang1_val.
// curv2_pt = C2+_display_gain * (base_pt + curv2_vec)
//            where curv2_vec = k2*m
//              k2       = curvature value for surfaces in the 
//                         domain2_dir direction,
//              norm_val = surface unit-sized normal-SPAvector.
//
// The tan_display_gain and C2_display_gain parameters are included
// in the point definitions so that the size of the displayed
// vectors may be modified at run time for viewing convienence.
//
// Each of the image_pts is backed up with one old copy of itself
// so that one iteration of pick and drag undo can be supported.
//
// DEFAULT CONSTRAINT VALUES
//  
// Whenever the default shape feature is enabled, the
// enforcement of SPAposition and tangent constraints requires
// the storage of a default value for the SPAposition or constraint
// value.  No such values are required for the normal or
// curvature constraints.  Default values are calculated,
// updated and used internally, they never get passed through
// an interface.
//
// THE POINT CONSTRAINT STRATEGY
//
// The basic flavor for using a point constraint is:
//  1. Create and apply the constraint to a deformable model,
//     (DM_add_pt_cstrn()).
//  2. Select which geometry properties are being constrained,
//     (DM_set_cstrn_behavior()).
//  3. Modify the geometry property values by moving the
//     point constraint display points as described above,
//     (DM_set_pt_xyz()).
//  4. Call solve to see how the constraint modifications changed
//     the deformable model shape,
//     (DM_solve()).
//  5. Optionally, the point constraint's domain SPAposition and
//     domain directions (for surfaces) may be modified with
//     5a. (DM_set_pt_uv()),
//     5b. (DM_set_cstrn_pttan_uv_dir()).
//  

// note:
// Every function that requires to compute the pt to val relationships
// as described above, take the tan_display_gain and C2_display_gain values as arguments.  
// Consistent rendering and I/O will only occur when tan_display_gain 
// values are used consistently.  (A unique pair of these gains
// is stored with each dmod).

// Point constraint state values
#define DSCALC_BASE_PT      (1  <<  0) // recalc pst_base_pt & pst_tang_pt 
#define DSCALC_BASE_DEF     (1  <<  1) // recalc pst_base_def              

#define DSCALC_TANG1_VAL    (1  <<  2) // recalc pst_tang_vec & pst_tang_pt
#define DSCALC_TANG1_PT     (1  <<  3) // recalc pst_tang_pt
#define DSCALC_TANG1_DEF    (1  <<  4) // recalc pst_tang_def  
            
#define DSCALC_TANG2_VAL    (1  <<  5) // recalc pst_tang_vec & pst_tang_pt
#define DSCALC_TANG2_PT     (1  <<  6) // recalc pst_tang_pt
#define DSCALC_TANG2_DEF    (1  <<  7) // recalc pst_tang_def
              
#define DSCALC_NORM_VAL     (1  <<  8) // recalc pst_tang_vec & pst_tang_pt
#define DSCALC_NORM_PT      (1  <<  9) // recalc pst_tang_pt
              
#define DSCALC_CURV1_VAL    (1  << 10) // recalc pst_Wuu_vec
#define DSCALC_CURV1_PT     (1  << 11) // recalc pst_curv1_pt

#define DSCALC_CURV2_VAL    (1  << 12) // recalc pst_Wvv_vec
#define DSCALC_CURV2_PT     (1  << 13) // recalc pst_curv2_pt

#define DSCALC_BINORM_VAL   (1  << 14) // recalc pst_tang_vec & pst_tang_pt
#define DSCALC_BINORM_PT    (1  << 15) // recalc pst_tang_pt
              
#define DSCALC_PARENT_PFUNC (1  << 16) // use parent_pfunc for calcs

#define DSCALC_ALL_PTS  (  DSCALC_BASE_PT   | DSCALC_NORM_PT   \
                         | DSCALC_TANG1_PT  | DSCALC_TANG2_PT  \
                         | DSCALC_CURV1_PT  | DSCALC_CURV2_PT  \
                         | DSCALC_BINORM_PT )

#define DSCALC_ALL_DEFS (  DSCALC_BASE_DEF  \
                         | DSCALC_TANG1_DEF \
                         | DSCALC_TANG2_DEF )

#define DSCALC_ALL_VALS (  DSCALC_BASE_PT   | DSCALC_NORM_VAL  \
                         | DSCALC_TANG2_VAL | DSCALC_TANG1_VAL \
                         | DSCALC_CURV1_VAL | DSCALC_CURV2_VAL \
                         | DSCALC_BINORM_VAL)

class DS_pt_cstrn : public DS_cstrn  // identifier = pst
{
  protected:
   int     pst_elem_index; // cache the elem_index for the pt. When
                           // set to -1 it is a flag to recompute in
                           // call Calc_def_and_image_pts()

   int     pst_image_dim ; // [array_size] problem image dim size
   int     pst_domain_dim; // [array_size] problem domain dim size

   double *pst_domain_pt ; // domain point constraint SPAposition
                           // Sized:[pst_domain_dim]

   double *pst_domain1_dir;// uv1 direction of constrained tangent SPAvector 1
                           // Sized:[pst_domain_dim]
                           // for surfaces only

   double *pst_domain2_dir;// uv2 direction of constrained tangent SPAvector 2
                           // Sized:[pst_domain_dim]
                           // for surfaces only

   // a pt_cstrn consists of the set
   //  { domain_pt,                       // domain location
   //    domain1_dir, domain2_dir,        // surf domain directions
   //    base_pt,                         // target value
   //    tang1_val, tang2_val, norm_val,  // target value
   //    binorm_val, curv1, curv2         // target value
   //  }
   //  The base pt, image vecs, and curvature values all act as 
   //  target values for constraints.
   //  Every target value, has an old and a default value.
   //  Every target value also has a pt value used for rendering
   //    and picking.  The pt values for tang1, tang2, norm,
   //    and binorm are of the form
   //      vec_pt = base_pt + tan_display_gain * val,
   //    The pt values for curv1 and curv2 are of the form
   //      vec_pt = base_pt + C2_display_gain * curv * norm_val
   //               where curv = k     = curvature
   //                     norm_val = m = normal direction
   //  The gain values in the pt = base + gain * vec definitions
   //  allow for run time viewing optimization.

   double *pst_base_pt ;   // image loc of domain_pt on surface
                           // Sized:[pst_image_dim] (X of X = P + Q)
   double *pst_base_old ;  // last valid base_pt saved for un-doing
                           // illegal pt tracking
   double *pst_base_def ;  // image loc of domain_pt on default_shape
                           // Sized:[pst_image_dim] (P of X = P + Q)


   double *pst_tang1_val ; // constrained value for tangent 1 at given pt
                           // Sized:[pst_image_dim] (dX1 of dX1 = dP1 + dQ1)
                           // constraint target SPAvector: t1.dW = dX1
   double *pst_tang1_pt ;  // image loc of tang end, tpt = X + dX
                           // tang_pg = base_pt + tan_display_gain*tang_vec
                           // for graphics and picking
                           // Sized:[pst_image_dim]
   double *pst_tang1_old ; // last valid tangent_pt saved for un-doing
                           // illegal pt tracking
   double *pst_tang1_def ; // constrained value for tangent on default_shape
                           // Sized:[pst_image_dim] (dP of dX = dP + dQ)

   // NOTE:  tang2 memory is used only by deformable surface
   double *pst_tang2_val ; // constrained value for tangent 2 at given pt
                           // Sized:[pst_image_dim] (dX2 of dX2 = dP2 + dQ2)
                           // constraint target SPAvector: t2.dW = dX2
   double *pst_tang2_pt ;  // image loc of tang end, tpt = X + dX
                           // tang_pg = base_pt + tan_display_gain*tang_vec
                           // for graphics and picking
                           // Sized:[pst_image_dim]
   double *pst_tang2_old ; // last valid tangent_pt saved for un-doing
                           // illegal pt tracking
   double *pst_tang2_def ; // constrained value for tangent on default_shape
                           // Sized:[pst_image_dim] (dP of dX = dP + dQ)


   double *pst_norm_val ;  // constrained value for normal at given pt
                           // SPAunit_vector, Sized:[pst_image_dim] 
                           // for curves and surfaces, tangents and binorm
                           // are perp to norm_val.
   double *pst_norm_pt ;   // image loc of normal end_pt
                           // norm_pt = base_pt + tan_display_gain*scale*norm_val
                           // for graphics and picking
                           // Sized:[pst_image_dim]
   double pst_norm_size ;  // size of norm_vec for rendering
   double *pst_norm_old ;  // last valid norm_pt saved for un-doing
                           // illegal pt tracking

   double *pst_binorm_val ;// constrained value for binormal at given pt
                           // SPAunit_vector, Sized:[pst_image_dim] 
                           // for curves, tangent and norm are required
                           // to be perpendicular to the binorm
   double *pst_binorm_pt ; // image loc of binormal end_pt
                           // binorm_pt = base_pt + tan_display_gain*scale*binorm_val
                           // for graphics and picking
                           // Sized:[pst_image_dim]
   double pst_binorm_size; // isze of binorm_vec for rendering
   double *pst_binorm_old ;// last valid binorm_pt saved for un-doing
                           // illegal pt tracking


   double pst_curv1_val ;  // curvature, k, value for curves and 
                           // surface domain1_dir direction.
                           // Constrain d2W/du2 value 
                           //    on line = k*m + h*Wu_vec
                           //  where h is free variable
                           // Note: when changing Wu_vec k remains constant
                           // Sized:[pst_image_dim]
   double *pst_curv1_pt ;  // curv1_pt = base_pt + C2_display_gain*k*m
                           // where k=curvature
                           //       m=curve's normal direction
                           //      ||cross(Wu_vec,Wuu_vec)||
                           //  k = -------------------------
                           //          ||Wu_vec||**3
                           //  m = cross(b,t) with t = normalize(Wu_vec)
                           //  b = normalized(cross(Wu_vec,Wuu)
                           // Sized:[pst_image_dim]
   double *pst_curv1_old ; // last valid Wuu_pt saved for un-doing
                           // illegal pt tracking


   double pst_curv2_val ;  // curvature, k, value for surface 
                           // domain2_dir direction.
                           // Constrain d2W/du2 value 
                           //    on line = k*m + h*Wu_vec
                           //  where h is free variable
                           // Note: when changing Wu_vec k remains constant
                           // Sized:[pst_image_dim]
   double *pst_curv2_pt ;  // curv2_pt = base_pt + C2_display_gain*k*m
                           // where k=curvature
                           //       m=curve's normal direction
                           //      ||cross(Wv_vec,Wvv_vec)||
                           //  k = -------------------------
                           //          ||Wv_vec||**3
                           //  m = cross(b,t) with t = normalize(Wv_vec)
                           //  b = normalized(cross(Wv_vec,Wvv_vec)
                           // Sized:[pst_image_dim]
   double *pst_curv2_old ; // last valid Wvv_pt saved for un-doing
                           // illegal pt tracking

  public:
   // DS_pt_cstrn constructor, copy, Make_copy, Size_arrays, Destructor

 DS_pt_cstrn                             // eff: Default Constructor
  (DS_dmod       *dmod   = NULL,         // in : constrained shape
   DS_dmod  *parent_dmod = NULL,         // in : Parent's shape (for children only)
   DS_CSTRN_SRC src_type = ds_user_cstrn,// in : oneof: ds_user_cstrn,
                                         //       ds_solid_cstrn, ds_bound_cstrn, or
                                         //       ds_invalid_cstrn for failed builds
   void     *src_data   = NULL,          // in : ACIS(edge,vertex)or NULL
   double   *domain_pt  = NULL,          // in : pt_cstrn loc,   sized:[domain_dim]
   double   *base_pt    = NULL,          // in : pt_loc or NULL, sized:[image_dim]
   double   *domain1_dir= NULL,          // in : pt_cstrn dir,   sized:[domain_dim]
                                         //      when NULL, defaults to u_dir = [1,0]
   double   *tang1_val  = NULL,          // in : tang1_val or NULL,sized:[image_dim]
   double   *curv1_val  = NULL,          // in : curvature in domain1_dir direction
                                         //      or NULL for unspecified, sized:[1]
   double   *norm_val   = NULL,          // in : normal vec or NULL, sized:[image_dim]
   double   *binorm_val = NULL,          // in : binormal vec for curves or NULL, 
                                         //      sized:[image_dim]
   double   *domain2_dir= NULL,          // in : pt_cstrn dir,   sized:[domain_dim]
                                         //      when NULL, defaults to v_dir = [0,1]
   double   *tang2_val  = NULL,          // in : tang2_val or NULL,sized:[image_dim]
   double   *curv2_val  = NULL,          // in : curvature in domain2_dir direction
                                         //      or NULL for unspecified, sized:[1]
   int       behavior=DS_CST_POS_FIXED,  // orof: DS_CST_POS_FIXED    = fix pt SPAposition
                                         //       DS_CST_TAN_FIXED    = fix tang1 value
                                         //       DS_CST_CURV_FIXED   = fix curv1 value
                                         //       DS_CST_NORM_FIXED   = fix normal SPAvector
                                         //       DS_CST_TAN_2_FIXED  = fix tang2 value
                                         //       DS_CST_CURV_2_FIXED = fix curv2 value
   double    tan_display_gain =1.0,      // in : tang_pt = base_pt + tan_display_gain*tang_vec
                                         //      norm_pt = base_pt + tan_display_gain*norm_val
   double    C2_display_gain  =1.0,      // in : curv_pt = base_pt + C2_display_gain *curv_vec
   int       comb_pt_count    =25,       // in : number of vecs to place in 
                                         //      curv1_comb and curv2_comb
   int       tag  = -1,                  // in : tag for apps
   DS_cstrn *next = NULL) ;              // in : linked list ptr

   DS_pt_cstrn             // Copy Constructor
   (DS_pt_cstrn &pt_cstrn);// in : object being copied

   DS_pt_cstrn &operator=  // assignment operator
    (DS_pt_cstrn &pt_cstrn);

   DS_cstrn*               // bas: DS_cstrn virtual function
    Make_copy()            // eff: Virtual DS_cstrn copy function
                           { return( new DS_pt_cstrn(*this)) ; }

   void
    Size_arrays            // Size_arrays
     (int image_dim=0,     // Size of the problem's image space
      int domain_dim=0) ;  // Size of the problem's domain space

   void
    Init_vectors() ;       // eff: set all vectors to SPAunit_vector

   virtual
  ~DS_pt_cstrn() ;         // Destructor

  // DS_pt_cstrn simple data access and set functions

  int  Elem_index()        { return pst_elem_index ; }
  void Clear_elem_index()  { pst_elem_index = -1 ; }
  int  Image_dim()         { return pst_image_dim ; }
  int  Domain_dim()        { return pst_domain_dim ; }

  double *Domain_pt()      { return pst_domain_pt ; }
  double &Domain_pt(int d) { DS_assert(d >= 0 && d < pst_domain_dim) ;
                             return( pst_domain_pt[d]) ; 
                           }

  int                      // int: 0=success,-1=bad_dpt
  Set_domain_pt            // eff: copies dpt into pst_domain_pt
  (double *dpt,            // in : pt to copy, Sized:Domain_pt_size()
   double tan_display_gain,// in : tang_pt = base_pt+tan_display_gain*tang_vec
   double C2_display_gain, // in : curv_pt = base_pt + C2_display_gain*curv*tang_val
   int image_save_flag) ;  // orof: 1=calc new base_pt
                           //       2=calc new tang and norm vals
                           //       4=calc new curv1 and curv2 vals
                           //       0=save old vals


  DECL_DM double *                // ret: tang1_dir, tang2_dir
  Domain_dir               // eff: return pst_domain1_dir or pst_domain2_dir
  (DS_CST_PT_INDEX         // in : which domain_dir to return
             pt_index) ;   // oneof: PST_TANG1,  PST_TANG2,
                           //        PST_CURV1,  PST_CURV2

  DECL_DM double &                // ret: component of tang1_dir, tang2_dir
  Domain_dir               // eff: return pst_domain1_dir or pst_domain2_dir
  (DS_CST_PT_INDEX         // in : which domain_dir to return
             pt_index,     // oneof: PST_TANG1,  PST_TANG2,
                           //        PST_CURV1,  PST_CURV2
   int ii) ;               // in : domain space component

  int                      // int: 0=success,-1=bad_dpt
  Set_domain_dir           // eff: copies dpt into pst_domain_pt
  (double *domain_dir,     // in : pt to copy, Sized:Domain_pt_size()
   double tan_display_gain,// in : tang_pt = base_pt+tan_display_gain*tang_vec
   double C2_display_gain, // in : curv_pt = base_pt + C2_display_gain*curv*tang_val
   DS_CST_PT_INDEX         // in : which domain_dir to return
             pt_index) ;   // oneof: PST_TANG1,  PST_TANG2,
                           //        PST_CURV1,  PST_CURV2

  DECL_DM double *                // ret: oneof the pt_cstrn target vals
  Cstrn_val                // eff: branch on pt_index
  (DS_CST_PT_INDEX         // in : which cstrn_val to return
             pt_index) ;   // oneof: PST_BASE_PT, PST_TANG1, PST_TANG2, 
                           //        PST_NORM,    PST_CURV1, PST_CURV2,
                           //        PST_BINORM

  DECL_DM double &                // ret: comp of oneof the pt_cstrn target vals
  Cstrn_val                // eff: branch on pt_index
 (DS_CST_PT_INDEX          // in : which cstrn_val to return
            pt_index,      // oneof: PST_BASE_PT, PST_TANG1, PST_TANG2, 
                           //        PST_NORM,    PST_CURV1, PST_CURV2,
                           //        PST_BINORM
   int ii) ;               // in : image space component index

  void 
  Set_cstrn_val            // set: cstrn_val, and update image_pt
  (double *cstrn_val,      // in : val to copy, Sized:[pst_image_dim or 1]
   double tan_display_gain,// in : tang_pt = base_pt + tan_display_gain*tang_vec
   double C2_display_gain, // in : curv_pt = base_pt + C2_display_gain*curv*tang_val
   DS_CST_PT_INDEX         // in : which cstrn_val to update
            pt_index) ;    // oneof: PST_BASE_PT, PST_TANG1, PST_TANG2,
                           //        PST_NORM,    PST_CURV1, PST_CURV2,
                           //        PST_BINORM

  int                      // ret: sizeof Cstrn_val
  Cstrn_val_size           // eff: branch on pt_index
  (DS_CST_PT_INDEX         // in : which image_pt to return
             pt_index) ;   // oneof: PST_BASE_PT, PST_TANG1, PST_TANG2,
                           //        PST_NORM,    PST_CURV1, PST_CURV2,
                           //        PST_BINORM

  void                     // eff: for internal use only, used
  Set_all_cstrn_vals       //      when restoring stored cstrns 
  (double *domain_pt,      // in : domain pt location for cstrn
   double *domain1_dir,    // in : value of domain1_dir direction
   double *domain2_dir,    // in : value of domain2_dir direction
   double *base_pt,        // in : image space pt location
   double *tang1_val,      // in : value for tang1 image direction
   double *tang2_val,      // in : value for tang2 image direction
   double *norm_val,       // in : value for norm image direction
   double *binorm_val,     // in : value for binorm image direction
   double curv1_val,       // in : value for domain1 direction curvature
   double curv2_val,       // in : value for domain2 direction curvature
   double tan_display_gain,// in : tang_pt = base_pt + tan_display_gain*tang_vec
   double C2_display_gain);// in : curv_pt = base_pt + C2_display_gain*curv*tang_val
   
  DECL_DM double *                // ret: base_pt, tang1_pt, tang2_pt, 
  Image_pt                 //      norm_pt, curv1_pt, curv2_pt
  (DS_CST_PT_INDEX         // in : which image_pt to return
              pt_index) ;  // oneof: PST_BASE_PT, PST_TANG1, PST_TANG2,
                           //        PST_NORM,    PST_CURV1, PST_CURV2,
                           //        PST_BINORM
                          
  DECL_DM double &                // ret: comp of base_pt, tang1_pt, tang2_pt,
  Image_pt                 //              norm_pt, curv1_pt, or curv2_pt
  (DS_CST_PT_INDEX         // in : which image_pt to return
             pt_index,     // oneof: PST_BASE_PT, PST_TANG1, PST_TANG2,
                           //        PST_NORM,    PST_CURV1, PST_CURV2,
                           //        PST_BINORM
   int ii) ;               // in : image space component index

  void 
  Set_image_pt             // eff: update image pt, recalc cstrn_val
  (double *ipt,            // in : pt to copy, Sized:Image_dim()
   double tan_display_gain,// in : tang_pt = base_pt+tan_display_gain*tang_vec
   double C2_display_gain, // in : curv_pt = base_pt + C2_display_gain*curv*tang_val
   DS_CST_PT_INDEX         // in : which image_pt to return
             pt_index,     // oneof: PST_BASE_PT, PST_TANG1, PST_TANG2,
                           //        PST_NORM,    PST_CURV1, PST_CURV2,
                           //        PST_BINORM
   int curvature_sign=0) ; // in : used when domain_dim==2 &&
                           //      pt_index==PST_CURV1 or PST_CURV2
                           //   +1: use curv_pt =  k *  n
                           //   -1: use curv_pt = -k * -n
                           //      to solve for new values k and n

  double Norm_size()       { return pst_norm_size ; }
  double Binorm_size()     { return pst_binorm_size ; }

private:
  void                     // use: called by Set_image_pt()
  Set_base_pt              // set: base_pt = ipt, update image_pts
  (double *ipt,            // in : pt to copy, Sized:Image_dim()
   double tan_display_gain,// in : tang_pt = base_pt + tan_display_gain*tang_vec
   double C2_display_gain);// in : curv_pt = base_pt + C2_display_gain *tang_vec

  void                     // use: called by Set_image_pt()
  Set_end_pt               // set: tgt image_pt and update cstrn_vals
  (double *end_pt,         // in : image space loc for end_pt
   double tan_display_gain,// in : tang_pt = base_pt + tan_display_gain*tang_vec
   double C2_display_gain, // in : curv_pt = base_pt + C2_display_gain *tang_vec
   DS_CST_PT_INDEX         // in : selects image_pt to be modified
             pt_index,     // oneof: PST_TANG1, PST_TANG2,
                           //        PST_NORM,  PST_CURV1, PST_CURV2,
                           //        PST_BINORM
   int curvature_sign) ;   // in : used when domain_dim==2 &&
                           //      pt_index==PST_CURV1 or PST_CURV2
                           //   +1: use curv_pt =  k *  n
                           //   -1: use curv_pt = -k * -n
                           //      to solve for new values k and n
public:

  double *                 // ret: base_def, tang1_def, tang2_def 
  Cstrn_def                // eff: return default constraint values
  (DS_CST_PT_INDEX         // in : which cstrn_def to return
             pt_index) ;   // oneof: PST_BASE_PT, PST_TANG1, PST_TANG2

  double &                 // ret: comp of base_def, tang1_def, tang2_def  
  Cstrn_def                // eff: return default constraint values
  (DS_CST_PT_INDEX         // in : which cstrn_def to return
             pt_index,     // oneof: PST_BASE_PT, PST_TANG1, PST_TANG2
   int ii) ;               // in : image space component index

  void 
  Set_cstrn_def            // eff: update image pt, recalc cstrn_val
  (double *cstrn_def,      // in : pt to copy, Sized:Image_dim()
   DS_CST_PT_INDEX         // in : which image_pt to return
             pt_index) ;   // oneof: PST_BASE_PT, PST_TANG1, PST_TANG2

  double *                 // ret: oneof the pt_cstrn's old image_pts
  Image_old                // eff: return old image_pt pointers
   (DS_CST_PT_INDEX        // in : which image_old to return
              pt_index) ;  // oneof: PST_BASE_PT, PST_TANG1, PST_TANG2,
                           //        PST_NORM,    PST_CURV1, PST_CURV2,
                           //        PST_BINORM

  double &                 // ret: comp of oneof the pt_cstrn' old image_pts
  Image_old                // eff: return old image_pt comp values
   (DS_CST_PT_INDEX        // in : which image_old to return
              pt_index,    // oneof: PST_BASE_PT, PST_TANG1, PST_TANG2,
                           //        PST_NORM,    PST_CURV1, PST_CURV2,
                           //        PST_BINORM
    int ii) ;              // in : image space component

  // DS_pt_cstrn convenience behavior checks
  virtual void             // base: DS_cstrn virtual function
   Set_coupled_behaviors   // eff: put cst_behavior into legal state
    () ;                
    
  int Is_pos()             // rtn: 1=cstrn is a pos_cstrn,0=not
                           { return(  (cst_behavior & DS_CST_POS_FIXED)
                                    ? 1 : 0) ;
                           }
  int Is_tang1()           // rtn: 1=cstrn is a tang1_cstrn,0=not
                           { return(  (cst_behavior & DS_CST_TAN_FIXED)
                                    ? 1 : 0) ;
                           }
  int Is_tang2()           // rtn: 1=cstrn is a tang2_cstrn,0=not                                                         
                           { return(  (cst_behavior & DS_CST_TAN_2_FIXED)
                                    ? 1 : 0) ;
                           }
  int Is_norm()            // rtn: 1=cstrn is a norm_cstrn,0=not
                           { return(  (cst_behavior & DS_CST_NORM_FIXED)
                                    ? 1 : 0) ;
                           }
  int Is_curv1()           // rtn: 1=cstrn is a curv1_cstrn,0=not
                           { return(  (cst_behavior & DS_CST_CURV_FIXED)
                                    ? 1 : 0) ;
                           }
  int Is_curv2()           // rtn: 1=cstrn is a curv2_cstrn,0=not                                                         
                           { return(  (cst_behavior & DS_CST_CURV_2_FIXED)
                                    ? 1 : 0) ;
                           }
  int Is_binorm()          // rtn: 1=cstrn is a binorm_cstrn,0=not
                           { return(  (cst_behavior & DS_CST_BINORM_FIXED)
                                    ? 1 : 0) ;
                           }

//========================================================
// cstrns only contribute if turned on.
// Is_Cd_contributor, Is_LLc_contributor
// Is_dof_map_contributor, Is_min_map_contributor
// Is_joint_Cd_contributor
//========================================================

  virtual                  // bas: DS_cstrn pure virtual function
  int Is_Cd_contributor    // ret: 1=cstrn adds Cd equations,0=doesn't
    (DS_dmod *dmod,        // in : for this dmod 
     int refine_flag=0)    // in : 0=use state bits as is
                           //      1=treat links as fixed
                           { if(Is_off()) return(0) ;
                             return( (   Is_position (dmod,refine_flag)
                                      || Is_tangent  (dmod,refine_flag))
                                    ? 1 : 0 ) ;
                           }

  virtual                  // bas: DS_cstrn pure virtual function
  int Is_LLc_contributor   // ret: 1=cstrn adds LLc equations,0=doesn't
    (DS_dmod *)            // in : dmod = for this dmod 
                           { if(Is_off()) return(0) ;
                             return(  (   Is_tang1()
                                       || Is_norm()
                                       || (pst_domain_dim == 1 && Is_binorm())
                                       || Is_curv1()
                                       || (pst_domain_dim == 2 && Is_tang2())
                                       || (pst_domain_dim == 2 && Is_curv2()))
                                    ? 1 : 0) ;
                           }

  virtual                  // bas: DS_cstrn pure virtual function
  int                      // rtn: 1=cstrn affects min_map,0=doesn't
   Is_dof_map_contributor  // eff: area and link cstrns do,
     ()                    //      pt_cstrns and crv_cstrns do not.
                           { return(0) ;
                           }

  virtual                  // bas: DS_cstrn pure virtual function
  int                      // rtn: 1=cstrn adds to min_map,0=doesn't
   Is_min_map_contributor  // eff: crv and link cstrns may add,
                           //      pt_cstrns do not.
    (DS_dmod *,            // in : dmod = for this dmod 
     int =0)               // in : refine_flag = 0=use state bits as is
                           //                    1=treat links as fixed
                           { return(0) ;
                           }

  virtual                  // bas: DS_cstrn pure virtual function
  int                      // rtn: 1=cstrn adds to joint prob Cx=d eqns
   Is_joint_Cd_contributor // eff: link cstrns may add to joint Cx=d
    ()                     { return(0) ;
                           }

  int                      // rtn: 1=ok,0=error
   Check_cstrn_vals() ;    // eff: check vals for self consistency

  int                      // rtn: 1=ok,2=unexpected access
                           // eff: signals error when pt_index
   Check_vec_io            // is inappropriate for domain_dim
   (DS_CST_PT_INDEX        // oneof: PST_BASE_PT, PST_TANG1, PST_TANG2,
              pt_index) ;  //        PST_NORM,    PST_CURV1, PST_CURV2,
                           //        PST_BINORM
  void                    
  Cstrn_val_2image_pt      // eff: calc image_pt for a cstrn_val
  (double *cstrn_val,      // in : cstrn value,sized:[image_dim or 1]
   double tan_display_gain,// in : tang_pt = base_pt + tan_display_gain*tang_val
   double C2_display_gain, // in : curv_pt = base_pt + C2_display_gain *curv_val*norm_val
   DS_CST_PT_INDEX         // in : which image_pt to calulate
             pt_index) ;   // oneof: PST_BASE_PT, PST_TANG1, PST_TANG2,
                           //        PST_NORM,    PST_CURV1, PST_CURV2,
                           //        PST_BINORM

  void                    
  Image_pt_2cstrn_val      // eff: calc cstrn_val for a image_pt
  (double *image_pt,       // in : image pt,sized:[image_dim or 1]
   double tan_display_gain,// in : tang_pt = base_pt + tan_display_gain*tang_val
   double C2_display_gain, // in : curv_pt = base_pt + C2_display_gain *curv_val*norm_val
   DS_CST_PT_INDEX         // in : which cstrn_val to calulate
             pt_index,     // oneof: PST_BASE_PT, PST_TANG1, PST_TANG2,
                           //        PST_NORM,    PST_CURV1, PST_CURV2,
                           //        PST_BINORM
   int curvature_sign) ;   // in : used when domain_dim==2 &&
                           //      pt_index==PST_CURV1 or PST_CURV2
                           //   +1: use curv_pt =  k *  n
                           //   -1: use curv_pt = -k * -n
                           //      to solve for new values k and n

  void                     // eff: calc display pt positions from
  Calc_display_pts         //      target values and display gains
  (double tan_display_gain,// in : tang_pt = base_pt + tan_display_gain*tang_val
   double C2_display_gain);// in : curv_pt = base_pt + C2_display_gain *curv_val*norm_val

  void                     // eff: calc projection of image_vec
   Calc_uv_projection      //      into uv space of constrained shape
   (double *image_vec,     // in : image space vec to be projected
    double *domain_dir) ;  // out: domain space vec found by projection

  int                      // rtn: 0=success,-1=bad_dpt
  Calc_def_and_image_pts   // eff: calc def and ipt for dpt
  (double tan_display_gain,// in : tang_pt = base_pt + tan_display_gain*tang_vec
   double C2_display_gain, // in : curv_pt = base_pt + C2_display_gain *tang_vec
   int work_flag) ;        // in : an orof the following:
    // orof: DSCALC_BASE_PT   = recalc pst_base_pt & pst_tang_pt
    //       DSCALC_BASE_DEF  = recalc pst_base_def
    //
    //       DSCALC_TANG1_VAL = recalc pst_tang1_val & pst_tang1_pt
    //       DSCALC_TANG1_PT  = recalc pst_tang1_pt
    //       DSCALC_TANG1_DEF = recalc pst_tang1_def
    //
    //       DSCALC_TANG2_VAL = recalc pst_tang2_val & pst_tang2_pt
    //       DSCALC_TANG2_PT  = recalc pst_tang2_pt
    //       DSCALC_TANG2_DEF = recalc pst_tang2_def
    //
    //       DSCALC_NORM_VAL  = recalc pst_norm_val & pst_norm_pt
    //       DSCALC_NORM_PT   = recalc pst_norm_pt
    //
    //       DSCALC_CURV1_VAL = recalc pst_Wuu_vec & pst_curv1_pt
    //       DSCALC_CURV1_PT  = recalc pst_curv1_pt
    //
    //       DSCALC_CURV2_VAL = recalc pst_Wvv_vec & pst_curv2_pt
    //       DSCALC_CURV2_PT  = recalc pst_curv2_pt
    //
    //       DSCALC_BINORM_VAL= recalc pst_binorm_val & pst_binorm_pt
    //       DSCALC_BINORM_PT = recalc pst_binorm_pt
    //
    //       DSCALC_PARENT_PFUNC = use parent_pfunc for calcs

  // stored array sizes
  int Image_pt_size        // rtn: size of any image or tang pt array
   (int image_dim)         // in : image dimension for constrained shape
                           { return image_dim ; 
                           }

  int Domain_pt_size       // rtn: size of pst_domain_pt array
   (int domain_dim)        // in : domain dimension of constrained shape
                           { return domain_dim ; 
                           }

  int Block_size           // rtn: total size of all arrays in object
   (int image_dim,         // in : image dimension for constrained shape
    int domain_dim)        // in : domain dimension of constrained shape
                           {return
                            ( 21*image_dim   // base_pt,old,def      3
                                             // tang1_val,pt,old,def 4
                                             // tang2_val,pt,old,def 4
                                             // norm_val,pt,old      3
                                             // curv1_pt,old         2
                                             // curv2_pt,old         2
                                             // binorm_val,pt,old    3
                             + 3*domain_dim);// domain_pt
                                             // domain1_dir,domain2_dir
                           }

  // DS_pt_cstrn implementations of DS_cstrn virtual functions
  // non-side-effect functions

  double                   // bas: DS_cstrn virtual function
   Dist2_to_iline          // ret: dist**2 between cstrn and iline
                           //      iline = pi0 + u(pi1-pi0)
    (double *pi0,          // in : image space pt, Sized:[image_dim]
     double *pi1,          // in : image space pt, Sized:[image_dim]
     DS_CST_PT_INDEX       // out: image_pt index in cstrn for closest pt
            &pt_index,     
     double &uu) ;         // out: iline param for pt closest to cstrn
 
  // implementations of DS_cstrn virtual functions
  // DS_pt_cstrn side-effect functions

  virtual void             // bas: DS_cstrn pure virtual function
   Shift_domain            // eff: shift the basis domain values
    (double *du,           // in : domain shift SPAvector, Sized:Domain_dim()
     DS_dmod *dmod) ;      // in : for this dmod 

  virtual                  // bas: DS_cstrn pure virtual function
   int                     // rtn: 0=done,-1=scale too small
   Scale_domain            // eff: scale the basis domain values
    (double gain,          // in : domain scale factor
     DS_dmod *dmod) ;      // in : for this dmod 

  virtual                  // bas: DS_cstrn virtual function
  int                      // rtn: 0=success,-1=bad_dpts
  Update_pts               // eff: update all cstrn cached data
                           //      after changes in pfunc
  (double tan_display_gain,// in : tang_pt = base_pt + tan_display_gain*tang_vec
   double C2_display_gain, // in : curv_pt = base_pt + C2_display_gain *tang_vec
   int pfunc_state) ;      // orof: DS_PFN_CHANGED_X      = X val change
                           //       DS_PFN_CHANGED_P_ON   = Set def shape
                           //       DS_PFN_CHANGED_P_OFF  = Clear def shape
                           //       DS_PFN_CHANGED_DOMAIN = Domain change
                           //       DS_PFN_TRACKING       = tracking parent

  virtual void             // bas: DS_cstrn virtual function
   Build_Cd_row            // eff: build one pt-cstrn's Cx=d eqn row
    (DS_dmod *dmod,        // in : shape to constrain
     DS_eqns *eqns,        // in : holder of Ax=b, Cx=d eqns
     int &next_Cd_row,     // i/o: next available Cx=d row
     int refine_flag=0) ;  // in : 0=use state bits as is
                           //      1=treat links as fixed

  virtual void             // bas: DS_cstrn virtual function
   Build_d_row             // eff: build one pt-cstrn's d of Cx=d eqn
    (DS_dmod *dmod,        // in : shape to constrain
     DS_eqns *eqns,        // in : holder of Ax=b, Cx=d eqns
     int &next_Cd_row) ;   // i/o: next available Cx=d row


  virtual void             // bas: DS_cstrn virtual function                        
   Build_L_Lc_row          // eff: build one pt-cstrn's Lxi=Lc eqn rows             
    (DS_dmod *dmod,        // in : shape to constrain
     DS_eqns *eqns,        // in : holder of Ax=b, Cx=d eqns
     int next_L_Lc_row) ;  // in : 1st available row of Li xi=Lc

  virtual void             // bas: DS_cstrn virtual function
   Build_Lc_row            // eff: build one pt-cstrn's d of Cx=d eqn
    (DS_dmod *dmod,        // in : shape to constrain
     DS_eqns *eqns,        // in : holder of Ax=b, Cx=d eqns
     int next_L_Lc_row) ;  // in : 1st available row of Li xi=Lc
#ifndef DS_USE_SYMEQ
#else
  virtual                  // bas: DS_cstrn virtual function
  int                      // rtn: free_dof_count, -2=no effect or -1=error                     
   Mark_this_free_dofs     // eff: mark all free dofs and elems in input
   (DS_dmod *,             // in : dmod = shape to constrain
    int ,                  // in : dof_count = sizeof dof_map
    int *,                 // i/o: dof_map = dof_map for constrained pfunc
    int &free_dof_count,   // out: dofs changed from fixed to free
    int ,                  // in : elem_count = sizeof elem_state
    int *,                 // i/o: elem_state = 0=free to move, 1=fixed
    int &free_elem_count)  // out: elems changed from fixed to free
                           { free_dof_count = 0 ;
                             free_elem_count = 0 ;
                             return -2 ; 
                           }
#endif //DS_USE_SYMEQ

  virtual void             // bas: DS_cstrn virtual function
  Bad_track_response       // eff: reset tracked cstrns. 
  (double tan_display_gain,// in : tang_pt = base_pt + tan_display_gain*tang_vec
   double C2_display_gain  // in : curv_pt = base_pt + C2_display_gain *tang_vec
   ) ;                     //      Called when tracking yields cstraint 
                           //      confilcts

  virtual                  // bas: DS_cstrn virtual function
  int Dump                 // rtn: 0=success,-1=file failure
    (char *filename,       // in : filename = file to write
     FILE *fp)             // in : existing file or NULL
                           { // local file access
                             int close = 0 ;
                             if(NULL == fp){fp = fopen(filename, "w");
                                            close = 1 ;
                                           }
                             if (!fp) return(-1) ;

                             DS_cstrn::Dump(filename,fp);
                             if(close) fclose(fp) ;
                             return(0) ; 
                           }
private:
  int Check_behavior()     // rtn: 1=behavior is ok,0=error
                           // eff: check that the tan and norm bits are  
                           //      set if the C2 bits are set
                           { int tgt ;
                             // 1st the tang1 direction
                             tgt =   DS_CST_CURV_FIXED 
                                   & DS_CST_TAN_FIXED
                                   & DS_CST_NORM_FIXED ;
                             DS_assert(   0   == (cst_behavior & tgt)
                                       || tgt == (cst_behavior & tgt)) ;
                             if(   0   != (cst_behavior & tgt)
                                && tgt != (cst_behavior & tgt)) return(0) ;

                             // 2nd the tang2 direction
                             tgt =   DS_CST_CURV_2_FIXED 
                                   & DS_CST_TAN_2_FIXED
                                   & DS_CST_NORM_FIXED ;
                             DS_assert(   0   == (cst_behavior & tgt)
                                       || tgt == (cst_behavior & tgt)) ;
                             if(   0   != (cst_behavior & tgt)
                                && tgt != (cst_behavior & tgt)) return(0) ;
                             return(1) ;
                           } // end DS_pt_cstrn::Check_behavior

private:
  virtual                  // bas: DS_cstrn pure virtual function     
   int Line_row_count      // rtn: total non_min_Cd Cx=d row count
    (DS_dmod * /*dmod*/)        // in : dmod = for this dmod 
                           { if(Is_off()) return(0) ;
                             int cnt = 0 ;
                             DS_assert(Check_behavior()) ;
                             if(cst_behavior & DS_CST_POS_FIXED   ) cnt++ ;
                             if(cst_behavior & DS_CST_TAN_FIXED   ) cnt++ ;
                             if(cst_behavior & DS_CST_TAN_2_FIXED ) cnt++ ;
                             return(cnt) ;
                           }

private:
  virtual                  // base: DS_cstrn
   int Mix_count           // rtn: Lxi=Lc row count for one cstrn only
   (DS_dmod *)             // in : dmod = for this dmod
                           { int cnt = 0 ;
                             if(Is_off()) return(0) ;
                             if(Is_norm())  cnt += pst_domain_dim ;
                             if(Is_binorm())cnt += 2 ;
                             if(Is_curv1()) cnt += pst_image_dim - 1 ;
                             if(Is_curv2()) cnt += pst_image_dim - 1 ;
                             return(cnt) ;
                           }

} ; // end class DS_pt_cstrn 

//=============================================================
//  class DS_crv_cstrn
//=============================================================

// Crv cstrns are implemented by keeping track of 2 different
// groups of 3 distinct sets of point spaces.  The first group of
// points relate to the points on the surface that are to be constrained
// called the out points because it is the output of the constraint
// to set their positions.  The second group of points are all those points
// that define the constraint's target shape and are called the source points
// because their values define the source values used to build the 
// constraint equations.  We specify the points on the
// surface to be constrained by their [u,v] locations which are
// given by a source curve function, C(s) = [u(s),v(s)].  Every surface
// point being constrained is associated with an image space point
// W(u,v) = [x(u,v),y(u,v),z(u,v)].  We can write down the image-space
// constrained curve shape as a function of s by using the C(s) function
// to generate the [u,v] values for the shape function W as:
//      W(s) = W(C(s)).
//
// Position constraints are enforced by ensuring that the
// Integral( (W(s) - W*(s))**2 ) ds = 0
//
// The source W*(s) function is the desired location of the curve given
// as a funtion of s.
//
// Since the points in a curve are continuous, the SPAposition
// constraint also constrains the surface tangent in the direction
// of the curve, Wt.  To make a surface normal constraint all we have
// to do is add a constraint on the surface tangent in the direction
// perpendicular to the curve, Wn. The surface normal is computed as
// Wnormal = Wt x Wn.  When both Wt and Wn are constrained so is the
// surface normal.
//
// The tangent constraint is enforced by ensuring that the
// Integral( (Wn(s) - Wn*(s))**2) ds = 0.
//
// The surface tangent Wn can be computed from the surface tangents
// Wu and Wv if the curve's uv space tangent SPAvector is known as
//
//  [Wn] = [-Cv Cu][Wu]
//  [Wt]   [ Cu Cv][Wv]
//
//  (use convenience functions DS_Wuv_2Wn() and DS_Wuv_2Wt()
//
//  A curvature constraint can be supported by adding a
//  constraint on the 2nd cross derivative,
//  Integral( (Wnn(s) - Wnn*(s))**2) ds = 0.
//
//  The surface 2nd derivative Wnn can be computed from the surface
//  derivatives Wuu, Wuv, and Wvv as
//  Wnn = Cv*Cv*Wuu - 2.0*Cv*Cu*Wuv + Cu*Cu*Wvv
//
//  (use the convenience function DS_Wuv_2Wnn())
//
//  The shape function W for the surface is a linear sum of weighted
//  basis functions,
//  W(u,v) = Sumij(Xij*Phii(u)*Phij(v)).
//
// To build the curve SPAposition, tangent, and curvature
// constraints we need to have the following information.
//
// The source data  C(s), Cs(s), W*(s), Wu*(s), Wv*(s),
//                  Wuu*(s), Wuv*(s), and Wvv*(s).
// And the basis description of the out points, 
//    Phii(u), d(Phii(u))/du, d2(phii(u))/du2
//    Phij(v), d(Phij(v))/dv, d2(phii(v))/dv2
//
// We use gaussian quadrature for evaluating the constraint integrals.
// All the required functions can be sampled at the gauss points and saved.


// DS_CSTRN_SRC records the source for crv_cstrns.  Crv_cstrns of
// different sources may be treated differently.  Crv_cstrns
// automatically generated from bounding edge topologies may
// not be deleted but under special circumstances may turned off.
// Crv_cstrns added by a user during a sculpting session may always
// be deleted and may also be turned off.  Crv_cstrns that are
// turned off may be displayed but generate no curve contstraints
// in the min(1/2xAx - bx) subject to constraints Cx = d equation
// set.

// Current ACIS crv_cstrn sources, privledges, and src_data
/// DS_CSTRN_SRC   delete  stop   src_  src_CW   comment
//                  able    able   data  _func
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

// Crv_Cstrn shape description.
// A crv_cstrn may be described in 3 seperate manners
//   1. A domain space pfunc curve.
//   2. A domain space and an image space curve set.
//   3. A user supplied function capable of computing
//        uv and xyz values for any given u SPAparameter value.
//
//  Case 1: cct_src_CW_func       == NULL and
//          cct_src_C_pfunc       != NULL and
//          cct_src_W_pfunc       == NULL and
//          cct_src_Wn_pfunc      == NULL and
//          cct_src_Wnn_pfunc     == NULL
//    uv values are computed from cct_src_C_pfunc and xyz values
//    are computed by projecting those values through the surface
//    pfunc object being constrained.  Constraint tolerances
//    are evaluated by comparing the xyz values computed when the
//    constraint was created to the current xyz values.
//
//  Case 2: cct_src_CW_func       == NULL and
//          cct_src_C_pfunc       != NULL and
//          cct_src_W_pfunc       != NULL and
//          cct_src_Wn_pfunc      != NULL and
//          cct_src_Wnn_pfunc     != NULL 
//    uv values are computed from cct_src_C_pfunc and xyz values
//    are computed from cct_src_W_pfunc.  Constraint tolerances
//    are evaluated by comparing the xyz values to xyz values
//    computed by projecting the uv values through the surface
//    pfunc definition.
//
//  Case 3: cct_src_CW_func != NULL  
//   uv and xyz values are generated by making calls to the
//   function cct_src_CW_func for each desired curve u SPAparameter.
//   Constraint tolerance are evaluated by comparing the xyz
//   values to the xyz values generated by projecting the uv
//   values through the surface pfunch definition.
//
// Tracking curve constraints:
//   Tracking curve constraints works as long as the
//   the source image_shape is compatible with the 
//   the projected image of the constrained curve.  This
//   means that an iso-SPAparameter line can be tracked as long
//   as the source curve is described by a B-spline or NURB curve
//   using the same degree, and knot SPAvector.  It also means that
//   rigid body motions of non-isoparameter lines should be possible.
//   In general, its difficult to predict the behavior of the deformation
//   for a tracking non-isoparameter curve constraint.
//
//   Curve tracking is accomplished through the src_CW_func and
//   src_W_pfunc, src_Wn_pfunc, and src_Wnn_pfunc set.  The application
//   modifies the functions, and then informs the point constraint
//   to update its points with a call to update_pts.  Subsequent
//   calls to solve on the deformable model will deform the shape
//   to track the new source shapes.
//   
// NOTE: Default shape values, P, in the constrained pfunc's
//       definition are not used in building any of the cached
//       crv_cstrn data.  Instead the P affects on the d SPAmatrix
//       of the Cx=d equations are dynamically built into the
//       outputs of the build_Cd_row() and build_d_row() functions.
//       So when P changes no changes need be made to the cached
//       crv_cstrn data, but the calling application will have
//       to rebuild the d SPAmatrix with a call to build_d_row()
//       before the next call to solve().

// DS_CRV_CSTRN graphics:  The crv_cstrn graphics consists of one
//                         polyline.
//  cst_grobj[0] = polyline_handle
//          

// typedef int (*FUNC)(int) ;

class DS_crv_cstrn : public DS_cstrn  // identifier = cct
{
   DS_enforcement_mechanism *cct_mech;
   //DM_xtan_trans             lct_xtan_trans;
  DM_icon               *cct_icon;

  protected:

   int cct_surf_dof_count; // [array_size] dof count in assoc pfunc
#ifdef DS_WN // try to implement cross-tangent gain constraint as a load
   int cct_elem_dof_count; // [array_size] assoc pfunc's elem dof count
#endif // DS_WN // try to implement cross-tangent gain constraint as a load
   int cct_domain_dim ;    // [array_size] assoc pfunc's domain size
   int cct_image_dim ;     // [array_size] assoc pfunc's image size
   int cct_ntgrl_degree ;  // [array_size] ntgrl accuracy in each elem
   int cct_seg_count ;     // [array_size] the curve is divided into 
                           //            segments on elem boundaries
   DS_pfunc *              // Optional source curve C description.
       cct_src_C_pfunc ;   // When present it is used to map    
                           // crv-domain pts to surf-domain pts,
                           // i.e. s pts to C(s), Cu(s), and Cv(s).

   DS_pfunc *              // Optional source curve W description.
       cct_src_W_pfunc ;   // When present it is used to generate
                           // the src SPAposition values (W(s)).
                           // Its expected image_dim is cct_image_dim.
                           // W(s) = W_pfunc(s). 

   DS_pfunc *              // Optional source curve Wn description.
       cct_src_Wn_pfunc ;  // When present it is used to generate
                           // the cross-tangent values (Wn(s)).
                           // Its expected image_dim is cct_image_dim. 
                           // Wn(s) = Wn_pfunc(s).

   DS_pfunc *              // Optional source curve curvature description.
       cct_src_Wnn_pfunc ; // When present it is used to generate
                           // the cross-curvature values, k(s).
                           // we enforce k*n(s) = Wnn_pfunc(s)*n.
                           //   where n(s)=cross(Wn(s),Wt(s))
                           //         k(s)=  size(cross(Wn,Wnn))
                           //              / size(Wn)**3  
                           // Its expected image_dim is 1.
                           // curvature can only be constrained if
                           // SPAposition and tangent are constrained.
                           // the W and Wn values may come from
                           // the pfuncs or the current shape.

   DS_dmod *               // optional tracking dmod to
       cct_src_W_dmod ;    // reshape cct_src_W_pfunc

   DS_dmod *               // optional tracking dmod to
       cct_src_Wn_dmod ;   // reshape cct_src_W_pfunc

   DS_dmod *               // optional tracking dmod to
       cct_src_Wnn_dmod ;  // reshape cct_src_W_pfunc

#ifdef DS_WN // try to implement cross-tangent gain constraint as a load
  double cct_Wn_weight ;   // the alpha of curve tang gain cstrns
#endif // DS_WN // try to implement cross-tangent gain constraint as a load

  void (*cct_src_CW_func)  // Optional source func to calc C and W crvs
    (void *src_data,       // in : app defined data
     double s,                // in : edge s param value
     double *C,               // out: surface C=[u,v] pt value for s
     double *Cs,              // out: surface dC/ds vec in surface for s
     double *W,               // out: image space W=[x,y,z] pt value for s
     double *Wu,              // out: image space dW/du tangent for s
     double *Wv,              // out: image space dW/dv tangent for s
     double *Wuu,             // out: image space d2W/du2 for s
     double *Wuv,             // out: image space d2W/dudv for s
     double *Wvv,             // out: image space d2W/dv2 for s
     double &dist2) ;         // out: dist**2(xyz,proj_to_3d(uv))
                           // note: for ACIS  use: DS_true_edge_eval()

   int cct_dof_count;      // total number of dofs used by cstrn
                           // different than cct_surf_dof_count
   int *cct_dof_map;       // map global to cstrn dof indices
                           // use: each used dof is assigned an
                           //      index in order of appearance
                           //      unused dofs == -1
                           //      ex: [-1 0 -1 1 -1 -1 2]
                           //      dofs 1,3,6 are being used
                           //      dofs 0,2,4,5 are not
                           // note: this map is built conservatively,
                           //       it contains every dof associated
                           //       with every element through which
                           //       the constraint runs.  It can
                           //       be used for C0 and C1 constraints.
                           // sized:[cct_surf_dof_count]
      
   // crv pt-arrays - the order of array declaration is assumed
   //                 by the evaluator functions. Don't change them                        
   double *cct_seg_bnd_s ; // Curve param values on segment end-pts.
                           // Sized:[cct_seg_count+1] 
                           // Segments are the crv_cstrn pieces that
                           // lie between the surface pfunc knot
                           // boundaries. The 1st and last entries
                           // must equal the crv_cstrn param range.

#ifdef DS_WN // try to implement cross-tangent gain constraint as a load
   int    *cct_seg_elem ;  // associated Pfunc elem number for each seg
                           // sized:[cct_seg_count]
   double *cct_src_C_bas ; // tgt_pfunc basis_uvs for each tgt_pt
                           // Sized:[Basis_pts_size()]
                           // Order:[pt0[bas0u..,basMu]..,
                           //        pt0[bas0v..,basMv]..,
                           //        ptNu[...],ptNv[...]]
   double *cct_b ;         // Crv load's b SPAmatrix entries
                           //    Sized:[ crl_image_dim
                           //           *crl_surf_dof_count]
                           //    Order:[x0..,xN,y0..,yN,z0..,zN]


#endif // DS_WN // try to implement cross-tangent gain constraint as a load

   double *cct_seg_bnd_W ; // Src xyz positions for each seg_bnd
                           // in cct_seg_bnd_s.  These numbers are
                           // not used in crv-cstrn processing. They
                           // are saved for debugging and rendering.
                           // Sized:[cct_image_dim*(cct_seg_count+1)]

   double *cct_src_s_pts ; // Crv end pt and segment gauss pt arrays.
                           // crv u vals.    Sized:[Image_pt_count()]
                           // Stored:[u0,u1..,un]
   double *cct_src_C_pts ; // crv C=uv vals.     Sized:[Domain_pts_size()]
                           // Stored:[u0,u1..,un,v0,v1..,vn]
   double *cct_src_Cs_pts; // crv Cs=dC/ds vals. Sized:[Domain_pts_size()]
                           // Stored:[du0,du1..,dun,dv0,dv1..,dvn]
   double *cct_src_W_pts;  // crv xyz vals.      Sized:[Image_pts_size()]
                           // Stored:[xyz0,xyz1..,xyzn]
   double *cct_src_Wn_pts; // crv -Cv*dW/du+Cu*dW/dv vals. Sized:[Image_pts_size()]
                           // Stored:[Wn0,Wn1..,Wnm]
   double *cct_src_Wnn_pts;// crv -Cv*-Cv*Wuu  Sized:[Image_pts_size()]
                           //     -Cv* Cu*Wuv
                           //     +Cu* Cu*Wvv. 
                           // Stored:[Wnn0,Wnn1..,Wnnm]
   double *cct_out_W_pts;  // cstrn xyz vals.    Sized:[Image_pts_size()]
                           // Stored:[xyz0,xyz1..,xyzn]
   double *cct_out_Wn_pts; // cstrn -Cv*dW/du+Cu*dW/dv vals.  Sized:[Image_pts_size()]
                           // Stored:[dWu0,dWu1..,dWun]
   double *cct_out_Wnn_pts;// crv -Cv*-Cv*Wuu  Sized:[Image_pts_size()]
                           //     -Cv* Cu*Wuv
                           //     +Cu* Cu*Wvv. 
                           // Stored:[Wnn0,Wnn1..,Wnnm]
   double *cct_pick_pt ;   // Sized:[ cct_image_dim]. Set for display
                           // by Dist2_to_iline().

   DS_mbvec_row_matrix cct_C[3];
   DS_block_clm_matrix cct_d[3]; 
//   double *cct_Cd ;        // Crv cstrn's C and d SPAmatrix entries
                           //    [C x= d] sized:[(surf_dof_count+image_dim) x cct_dof_count]
                           // Cd=[------]
                           //    [Cnx=dn] sized:[(surf_dof_count+image_dim) x cct_dof_count]
                           //  we store d = Int_ds(R(s)PHi(s))
                           //  we use   d = d - CP 
                           //   where P = default control point locs
                           // Sized:[ (  cct_surf_dof_count
                           //          + cct_image_dim)
                           //        *(  cst_C_row_count=cct_dof_count     // for C
                           //          + cst_Cn_row_count=cct_dof_count)]  // for Cn
                           // (for 5.2 cst_C_row_count == cst_Cn_row_count = cct_dof_count
                           //          and access to both C and Cn arrays
                           //          is supported by the single map
                           //          cct_dof_map)
                           // Order:[CD0(C00,..,C0n,scale_0(xyz)),CD1(xyz),...]

  // ACIS note: src_data is being used to secretly store ptrs
  //            to a COEDGE and FACE so that topology boundaries
  //            can be mapped back to their source COEDGEs
  //            at commit_2ACIS() and for use by the src_CW_func
  //            callbacks.

  public:
   // DS_crv_cstrn Constructor, copy, Make_copy, Size_arrays, Destructor

  DS_crv_cstrn                   //      Default Constructor
   (Spatial_abs_hurler & hurler,  // i/o: error reporting mechanism
    DS_enforcement_mechanism* & mech,// in : curve cstrn vs curve load
                                     // out: ptr value copied in then NULL'ed
    DS_dmod  *dmod         =NULL,// in : The shape being constrained        
    DS_dmod  *parent_dmod  =NULL,// in : Seam's parent shape for hierarchies
    DS_pfunc *src_C_pfunc  =NULL,// opt: Domain crv to calc C pt vals      
    DS_pfunc *src_W_pfunc  =NULL,// opt: Image  crv to calc W pt vals         
    DS_pfunc *src_Wn_pfunc =NULL,// opt: Image  crv to calc Wn vec vals
    DS_pfunc *src_Wnn_pfunc=NULL,// opt: function to calc k curvature vals
    void (*src_CW_func)          // opt: func to calc curve C and W pt vals
     (void *src_data,             // in : app defined data
      double s,                   // in : edge s param value
      double *C,                  // out: surface C=[u,v] pt value for s
      double *Cs,                 // out: surface dC/ds vec in surface for s
      double *W,                  // out: image space W=[x,y,z] pt value for s
      double *Wu,                 // out: image space dW/du for s
      double *Wv,                 // out: image space dW/dv for s
      double *Wuu,                // out: image space d2W/du2 for s
      double *Wuv,                // out: image space d2W/dudv for s
      double *Wvv,                // out: image space d2W/dv2 for s
      double &dist2) = NULL,      // out: dist**2(xyz,proj_to_3d(uv))
    void *src_data   = NULL,    // opt: Passed to src_CW_func callbacks      
    DS_CSTRN_SRC                // in : records cstrn origin. oneof:
        src_type=ds_user_cstrn, //      ds_solid_cstrn=shared topo bndry
                                //      ds_bound_cstrn=unshared topo bndry
                                //      ds_user_cstrn  =user created cstrn
                                //      ds_seam_cstrn  =join 2 hierarchical faces
    int seg_count    = 0,       // in : number of segments in Src_C_pfunc
                                //     segment=Src_C_pfunc piece in 1 elem
    double *seg_bnds = NULL,    // in : segment end-pt ordered param vals.
                                //      Sized:[seg_count+1]
    int behavior =              // orof: DS_CST_POS_FIXED  (fix surf to crv)
           DS_CST_POS_FIXED,    //      DS_CST_TAN_FIXED  (fix surf_tang across crv)
                                //      DS_CST_CURV_FIXED (fix surf_curv across crv)
    double tang_gain = 1.0,     // in : vary tang constraint magnitude
    int tag          = -1,      // in : tag so apps can track cstrns
    DS_cstrn *next   = NULL) ;  // in : linked list pointer

                           // Copy Constructor
   DS_crv_cstrn            // rtn: deep copy of crv_cstrn
    (DS_crv_cstrn &        // in : object being copied
       crv_cstrn) ;

   DS_crv_cstrn &operator= // assignment operator
    (DS_crv_cstrn &) ;

   DS_cstrn*               // bas: DS_cstrn virtual function
    Make_copy()            // eff: Virtual DS_cstrn copy function
                           { return( new DS_crv_cstrn(*this)) ; }

   void
    Size_arrays            // eff: Size internal arrays
    (int surf_dof_count=0, // in : constrained pfunc's dof count
#ifdef DS_WN // try to implement cross-tangent gain constraint as a load
     int elem_dof_count=0, // in : constrained pfunc's elem dof count
#endif // DS_WN // try to implement cross-tangent gain constraint as a load
     int C_row_count=0,    // in : number of Cx=d rows in SPAposition cstrn
     int Cn_row_count=0,   // in : number of Cx=d rows in tangent cstrn
     int Cnn_row_count=0,  // in : number of Cx=d rows in curvature cstrn
     int domain_dim=0,     // in : assoc pfunc's domain dimension
     int image_dim=0,      // in : assoc pfunc's image dimension
     int ntgrl_degree=0,   // in : gauss integration accuracy for Cd
     int seg_count=0) ;    // in : count of one-elem crv-pieces

   virtual
  ~DS_crv_cstrn() ;        // Destructor
                           //   delete cct_src_C_pfunc ;
                           //   Size_arrays() ;

  // Access enforcement mechanism
  virtual DS_enforcement_mechanism* Mech()
   {return cct_mech;}

  void Bind_new_emech(DS_enforcement_mechanism*& mech);

  // DS_crv_cstrn simple data access function

  int     Surf_dof_count() { return cct_surf_dof_count ; }
  int     Domain_dim()     { return cct_domain_dim ; }      
  int     Image_dim()      { return cct_image_dim ; }
  int     Ntgrl_degree()   { return cct_ntgrl_degree ; }
  int     Seg_count()      { return cct_seg_count ; }
  int     Seg_bnds_count() { return (  cct_seg_count == 0 
                                     ? 0 : cct_seg_count + 1) ; }
  int     Used_dof_count() { return cct_dof_count ; }

private:
  virtual                  // bas: DS_cstrn pure virtual function     
   int Line_row_count      // rtn: total non_min_Cd Cx=d row count
    (DS_dmod * /*dmod*/)        // in : dmod = for this dmod 
                           { // all DS_crv_cstrn eqns are in min_Cd
                             return(0) ;
                           }

  virtual                  // base: DS_cstrn
   int Mix_count           // rtn: Lxi=Lc row count for one cstrn only
   (DS_dmod *)             // in : dmod = for this dmod
                           { // DS_crv_cstrn do not generate mixed cstrns
                             return(0) ;
                           }
void                       // bas: DS_cstrn virtual function
Build_row // eff: build one crv-cstrn's Cx=d eqn set
  (DS_dmod *dmod,          // in : shape to constrain
   DS_eqns *eqns,        // in : holder of Ax=b, Cx=d eqns
   int &next_Cd_row,       // i/o: next available Cx=d row
   int build_LHS,
   int build_RHS);

public:

  virtual
  DS_abs_CW_server *Get_CW_src_data(DS_dmod* dm);

  virtual
  void *Get_passthrough_src_data(DS_dmod* dm);
//========================================================
// cstrns only contribute if turned on.
// Is_Cd_contributor, Is_LLc_contributor
// Is_dof_map_contributor, Is_min_map_contributor
// Is_joint_Cd_contributor
//========================================================

  virtual                  // bas: DS_cstrn pure virtual function
  int Is_Cd_contributor    // ret: 1=cstrn adds Cd equations,0=doesn't
    (DS_dmod *dmod,        // in : for this dmod 
     int refine_flag=0)    // in : 0=use state bits as is
                           //      1=treat links as fixed
                           { if(Is_off()) return 0 ;
                             return( (   Is_position (dmod,refine_flag)
                                      || Is_tangent  (dmod,refine_flag)
                                      || Is_curvature(dmod,refine_flag))
                                    ? 1 : 0 ) ;
                           }

  virtual                  // bas: DS_cstrn pure virtual function
  int Is_LLc_contributor   // ret: 1=cstrn adds LLc equations,0=doesn't
    (DS_dmod * /*dmod*/)        // in : for this dmod 
                           { // crv_cstrns have no mixed eqns
                             return(0) ;
                           }

  virtual                  // bas: DS_cstrn pure virtual function
  int                      // rtn: 1=cstrn affects min_map,0=doesn't
   Is_dof_map_contributor  // eff: area and link cstrns do,
     ()                    //      pt_cstrns and crv_cstrns do not.
                           { return(0) ;
                           }

  virtual                  // bas: DS_cstrn pure virtual function
  int                      // rtn: 1=cstrn adds to min_map,0=doesn't
   Is_min_map_contributor  // eff: crv and link cstrns may add,
                           //      pt_cstrns do not.
    (DS_dmod *dmod,        // in : for this dmod 
     int refine_flag=0)    // in : 0=use state bits as is
                           //      1=treat links as fixed
                           { if(Is_off()) return 0 ;
                             return( (   Is_position (dmod,refine_flag)
                                      || Is_tangent  (dmod,refine_flag)
                                      || Is_curvature(dmod,refine_flag))
                                    ? 1 : 0 ) ;
                           }

  virtual                  // bas: DS_cstrn pure virtual function
  int                      // rtn: 1=cstrn adds to joint prob Cx=d eqns
   Is_joint_Cd_contributor // eff: link cstrns may add to joint Cx=d
    ()                     { return(0) ;
                           }


  int Image_pt_count       // rtn: number of sampled curve points
    (int ntgrl_degree,     // in : gauss integration accuracy for Cd
     int seg_count)        // in : count of one-elem crv-pieces
                           { return(  seg_count == 0 ? 0
                                    : (  2 + seg_count
                                       * DS_linear_gauss_pt_count
                                                   (ntgrl_degree))) ; 
                           }  
  int Image_pt_count()     { return(Image_pt_count(cct_ntgrl_degree,
                                                   cct_seg_count)) ;
                           }

  int *Dof_map()           { return cct_dof_map ; }
  int Dof_map(int i)       // rtn: local index for global dof index
                           { DS_assert(i >= 0 && i < cct_surf_dof_count);
                             return( cct_dof_map[i]) ;
                           }

  int Dof_map_size         // rtn: cct_dof_map size in doubles
    (int surf_dof_count)   // in : dof count in constrained pfunc 
                           { if(surf_dof_count)
                              return(
                               (int)ceil(  surf_dof_count
                                         * (double)sizeof(int)
                                         / (double)sizeof(double))) ;
                             else return(0) ;
                           }
  
#ifdef DS_WN // try to implement cross-tangent gain constraint as a load

  int *Seg_elem()          // rtn: ptr to cct_seg_elem array
                           { return(cct_seg_elem) ; }

  int Seg_elem(int ii)     // rtn: elem index for iith segment
                           { DS_assert(ii>= 0 && ii <cct_seg_count) ;
                             return(cct_seg_elem[ii]) ;
                           }

  int Seg_elem_size        // rtn: cct_seg_elem size in doubles
    (int seg_count)        // in : domain_crv single elem segment cnt
                           { if(seg_count)
                              return(
                               (int)ceil(  seg_count
                                         * (double)sizeof(int)
                                         / (double)sizeof(double))) ;
                             else return(0) ;
                           }
  
  double *Src_C_bas()      // rtn: cct_src_C_bas
                           // Order:[pt0[bas0u..,basMu]..,
                           //        pt0[bas0v..,basMv]..,
                           //        ptNu[...],ptNv[...]]
                          { return cct_src_C_bas ; }

  double *Src_C_bas        // rtn: ptr to ipt 1st deriv bas values
   (int elem_dof_count,    // in : constrained pfunc's elem count
    int ipt)               // in : ipt index
                           { return(  cct_src_C_bas  
                                    + 2*elem_dof_count) ;
                           }

  int Src_C_bas_size       // rtn: crl_tgt_bas array size
                           // Order:[pt0[bas0u..,basMu]..,
                           //        pt0[bas0v..,basMv]..,
                           //        ptNu[...],ptNv[...]]
   (int ntgrl_degree,      // in : gauss integration accuracy
    int elem_dof_count,    // in : elem dof count for pfunc
    int seg_count)         // in : domain_crv single elem segment cnt 
                           { return(  seg_count == 0 ? 0
                                    : (  2 * elem_dof_count
                                       * (  2 + seg_count
                                          * DS_linear_gauss_pt_count
                                                 (ntgrl_degree)))
                                   ) ;
                           }
                            
  double *B()              // rtn: cstrn b's SPAmatrix index
                           // Order:[x0..,xN,y0..,yN,z0..,zN]
                           { return(cct_b) ; }

  double *B(int i)         // rtn: cstrn b's SPAmatrix index
                           // Order:[x0..,xN,y0..,yN,z0..,zN]
                           { DS_assert(i>=0 && i<cct_image_dim) ;
                            return(cct_b+i*cct_surf_dof_count) ; 
                           }

  double *B(int i,         // rtn: cstrn b's SPAmatrix index
            int dof_index) // in : surf_dof index
                           // Order:[x0..,xN,y0..,yN,z0..,zN]
                           { DS_assert(i>=0 && i<cct_image_dim) ;
                             DS_assert(i>=0 && i<cct_surf_dof_count) ;
                            return( cct_b
                                   +i*cct_surf_dof_count
                                   +dof_index) ; 
                           }

  int B_size               // rtn: size of cstrn's b array
   (int image_dim,         // in : assoc pfunc's image_dim
    int surf_dof_count)    // in : assoc pfunc's surf dof_count
                           { return(surf_dof_count*image_dim) ; }

  double Wn_weight()       {return( cct_Wn_weight ) ; }
  void Set_Wn_weight       // eff: set Wn_weight value
    (double Wn_weight)     // in : new value for Wn_weight
                           {
                             cct_Wn_weight = Wn_weight ;
                           }
#endif // DS_WN // try to implement cross-tangent gain constraint as a load

  double *Seg_bnds()       { return cct_seg_bnd_s ; }
  double  Seg_bnds(int i)  { DS_assert(i >= 0 && i < cct_seg_count+1);
                             return(cct_seg_bnd_s[i]) ;
                           }
  double *Seg_bnds_W()     { return cct_seg_bnd_W ; }
  double *Seg_bnds_W
           (int i)         { DS_assert(i >= 0 && i < cct_seg_count+1);
                             return(cct_seg_bnd_W+cct_image_dim*i);
                           }

  DS_pfunc *Src_C_pfunc()  { return cct_src_C_pfunc ; }
  void Set_src_C_pfunc     // eff: change src_C_pfunc value 
   (DS_pfunc *src_C_pfunc);// in : new src_C_pfunc description
                           
  int Is_src_pfunc         // rtn: 1=yes,0=no
   (DS_pfunc *src_pfunc)   // in : pfunc to check
                           { return
                             (  (   src_pfunc == cct_src_W_pfunc
                                 || src_pfunc == cct_src_Wn_pfunc
                                 || src_pfunc == cct_src_Wnn_pfunc)
                              ? 1 : 0) ;
                           }        
                         
  DS_pfunc *Src_W_pfunc()  { return cct_src_W_pfunc ; }
                           
  DS_pfunc *Src_Wn_pfunc() { return cct_src_Wn_pfunc ; }

  DS_pfunc *Src_Wnn_pfunc(){ return cct_src_Wnn_pfunc ; }

  void Set_src_pfuncs       // set: all src_W_pfuncs at once
  (DS_pfunc *src_W_pfunc,   // in : cstrn pos shape, nested
   DS_pfunc *src_Wn_pfunc,  // in : cross-tang shape, nested
   DS_pfunc *src_Wnn_pfunc);// in : curvature shape, nested

  int Is_src_dmod          // rtn: 1=yes,0=no
   (DS_dmod *src_dmod)     // in : dmod to check
                           { return
                             (  (   src_dmod == cct_src_W_dmod
                                 || src_dmod == cct_src_Wn_dmod
                                 || src_dmod == cct_src_Wnn_dmod)
                              ? 1 : 0) ;
                           }        
                         
  DS_dmod *Src_W_dmod()    { return cct_src_W_dmod ; }
                           
  DS_dmod *Src_Wn_dmod()   { return cct_src_Wn_dmod ; }

  DS_dmod *Src_Wnn_dmod()  { return cct_src_Wnn_dmod ; }

  void Set_src_dmods       // set: all src_W_dmods at once
  (DS_dmod *src_W_dmod,    // in : cstrn pos shape, nested
   DS_dmod *src_Wn_dmod,   // in : cross-tang shape, nested
   DS_dmod *src_Wnn_dmod); // in : curvature shape, nested
                         
  void (*Src_CW_func())      // rtn: user given crv function description
    (void *src_data,         // in : app defined data
     double s,               // in : edge s param value
     double *C,              // out: surface C=[u,v] pt value for s
     double *Cs,             // out: surface dC/ds vec in surface for s
     double *W,              // out: image space W=[x,y,z] pt value for s
     double *Wu,             // out: image space dW/du tangent for s
     double *Wv,             // out: image space dW/dv tangent for s
     double *Wuu,            // out: image space d2W/du2 for s
     double *Wuv,            // out: image space d2W/dudv for s
     double *Wvv,            // out: image space d2W/dv2 for s
     double &dist2)          // out: dist**2(xyz,proj_to_3d(uv))
                             { return cct_src_CW_func ; }

  void Set_src_CW_func       // Set: func & data used to calc C and W vals
   (void *src_data,          // in : data to pass to func at compute time
    void (*src_CW_func)      // in : func to use to compute C and W vals
     (void *,                  // in : src_data = app defined data
      double ,                 // in : s = edge s param value
      double *,                // out: C = surface C=[u,v] pt value for s
      double *,                // out: Cs = surface dC/ds vec in surface for s
      double *,                // out: W = image space W=[x,y,z] pt value for s
      double *,                // out: image space dW/du for s
      double *,                // out: image space dW/dv for s
      double *,                // out: Wuu = image space d2W/du2 for s
      double *,                // out: Wuv = image space d2W/dudv for s
      double *,                // out: Wvv = image space d2W/dv2 for s
      double &)) ;             // out: dist2 = dist**2(xyz,proj_to_3d(uv))


  double *Src_s_pts()      { return cct_src_s_pts ; }
  double  Src_s_pts(int i) { DS_assert(i >= 0 && i < Image_pt_count()) ;
                             return(cct_src_s_pts[i]) ;
                           }
  double *Src_C_pts()      { return cct_src_C_pts ; }
  double  Src_C_pts_u      // rtn: u coordinate of ith src_C_pts
           (int i)         { DS_assert(i >= 0 && i < Image_pt_count()) ;
                             return(cct_src_C_pts[i]) ;
                           }
  double  Src_C_pts_v      // rtn: v coordinate of ith src_C_pts
           (int i)         { DS_assert(i >= 0 && i < Image_pt_count()) ;
                             return(cct_src_C_pts[i+Image_pt_count()]) ;
                           }
  double *Src_Cs_pts()     { return cct_src_Cs_pts ; }
  double  Src_Cs_pts_u     // rtn: u coordinate of ith src_Cs_pts
           (int i)         { DS_assert(i >= 0 && i < Image_pt_count()) ;
                             return(cct_src_Cs_pts[i]) ;
                           }
  double  Src_Cs_pts_v     // rtn: v coordinate of ith src_Cs_pts
           (int i)         { DS_assert(i >= 0 && i < Image_pt_count()) ;
                             return(cct_src_Cs_pts[i+Image_pt_count()]) ;
                           }

  double *Src_W_pts()      { return cct_src_W_pts ; }
  double *Src_W_pts(int i) {DS_assert(i >= 0 && i < Image_pt_count()) ;
                             return(cct_src_W_pts + i*cct_image_dim);
                           }
  double *Src_Wn_pts()     { return cct_src_Wn_pts ; }
  double *Src_Wn_pts(int i){DS_assert(i >= 0 && i < Image_pt_count()) ;
                             return(cct_src_Wn_pts + i*cct_image_dim);
                           }

  double *Src_Wnn_pts()    { return cct_src_Wnn_pts ; }
  double *Src_Wnn_pts(int i){DS_assert(i >= 0 && i < Image_pt_count()) ;
                             return(cct_src_Wnn_pts + i*cct_image_dim);
                           }

  double *Out_W_pts()      { return cct_out_W_pts ; }
  double *Out_W_pts(int i) { DS_assert(i >= 0 && i < Image_pt_count()) ;
                             return(cct_out_W_pts + i*cct_image_dim);
                           }
  double *Out_Wn_pts()     { return cct_out_Wn_pts ; }
  double *Out_Wn_pts(int i){ DS_assert(i >= 0 && i < Image_pt_count()) ;
                             return(cct_out_Wn_pts + i*cct_image_dim);
                           }
  double *Out_Wnn_pts()    { return cct_out_Wnn_pts ; }
  double *Out_Wnn_pts(int i){ DS_assert(i >= 0 && i < Image_pt_count()) ;
                             return(cct_out_Wnn_pts + i*cct_image_dim);
                           }

  double *Pick_pt()        { return cct_pick_pt ; }
  double C(int deriv, int irow, int idof) { return cct_C[deriv](irow,idof); }
  double d(int deriv, int irow, int idim) { return cct_d[deriv](irow,idim); }
  void Add_to_d(int deriv, int irow, int idim, double val) { cct_d[deriv].Pluseq_elem(irow,idim,val); }
  void Add_to_C(int deriv, int irow, int idof, double val) { cct_C[deriv].Pluseq_elem(irow,idof,val); }
//  double *Cd()             { return cct_Cd ; }
//  double *Cd_pos()         { return cct_Cd ; }
//  double *Cd_tan()         { return (  cct_Cd 
//                                     + Cd_size(cst_C_row_count,
//                                               cct_surf_dof_count,
//                                               cct_image_dim) ) ;
//                           }
//  double *Cd_curv()        { return (  cct_Cd 
//                                     + Cd_size(  cst_C_row_count
//                                               + cst_Cn_row_count,
//                                               cct_surf_dof_count,
//                                               cct_image_dim) ) ;
//                           }

  int Crow_offset(int i)   { DS_assert(i >= 0 && i < cct_surf_dof_count) ;
                             return(i * (  cct_surf_dof_count
                                         + cct_image_dim)) ;
                           }

  int Drow_offset(int i)   { DS_assert(i >= 0 && i < cct_surf_dof_count);
                             return (  cct_surf_dof_count
                                     + i * (  cct_surf_dof_count
                                            + cct_image_dim)) ;
                           }

//  double *Cpos_row(int i)  { DS_assert(i >= 0 && i < cct_surf_dof_count);
//                             return (  cct_Cd 
//                                     + i * (  cct_surf_dof_count
//                                            + cct_image_dim)) ;
//                           }
//  double *Dpos_row(int i)  { DS_assert(i >= 0 && i < cct_surf_dof_count);
//                             return (  cct_Cd + cct_surf_dof_count
//                                     + i * (  cct_surf_dof_count
//                                            + cct_image_dim)) ;
//                           }
//
//  double *Ctan_row(int i)  { DS_assert(i >= 0 && i < cct_surf_dof_count);
//                             return (  cct_Cd 
//                                     + (i + cst_C_row_count)
//                                     * (  cct_surf_dof_count
//                                        + cct_image_dim)) ;
//                           }
//  double *Dtan_row(int i)  { DS_assert(i >= 0 && i < cct_surf_dof_count);
//                             return (  cct_Cd + cct_surf_dof_count
//                                     + (i + cst_C_row_count)
//                                     * (  cct_surf_dof_count
//                                        + cct_image_dim)) ;
//                           }
//
//  double *Ccurv_row(int i) { DS_assert(i >= 0 && i < cct_surf_dof_count);
//                             return (  cct_Cd 
//                                     + (i + cst_C_row_count
//                                          + cst_Cn_row_count)
//                                     * (  cct_surf_dof_count
//                                        + cct_image_dim)) ;
//                           }
//  double *Dcurv_row(int i) { DS_assert(i >= 0 && i < cct_surf_dof_count);
//                             return (  cct_Cd + cct_surf_dof_count
//                                     + (i + cst_C_row_count
//                                          + cst_Cn_row_count)
//                                     * (  cct_surf_dof_count
//                                        + cct_image_dim)) ;
//                           }

  // DS_crv_cstrn internal array size functions
                                
  int Seg_bnd_s_size       // rtn: seg_cuts array size
    (int seg_count)        // in : number of curve segments
                           { return (  seg_count == 0 
                                     ? 0 
                                     : seg_count + 1) ; 
                           }
  int Seg_bnd_W_size       // rtn: seg_bnds image_pt array size
    (int image_dim,        // in : image space size
     int seg_count)        // in : number of curve segments
                           { return (  image_dim 
                                     * Seg_bnd_s_size(seg_count)) ;
                           }
  int Seg_bnd_W_size()  { return(Seg_bnd_W_size
                                        (cct_image_dim,
                                         cct_seg_count) );
                           }

  int Image_pts_size       // rtn: true_pts and proj_pts array sizes
    (int ntgrl_degree,     // in : gauss integration accuracy for Cd
     int image_dim,        // in : image space size
     int seg_count)        // in : count of one-elem crv-pieces 
                           { return (  Image_pt_count(ntgrl_degree,
                                                     seg_count)
                                     * image_dim) ;
                           }
  int Image_pts_size()     {return(Image_pts_size(cct_ntgrl_degree,
                                                  cct_image_dim,
                                                  cct_seg_count)) ;
                           }                            
  int Domain_pts_size      // rtn: domain_pts array_size
    (int ntgrl_degree,     // in : gauss integration accuracy for Cd
     int domain_dim,       // in : domain space size
     int seg_count)        // in : count of one-elem crv-pieces 
                           { return (  Image_pt_count(ntgrl_degree,
                                                      seg_count)
                                     * domain_dim) ;
                           } 
  int Domain_pts_size()    {return(Domain_pts_size(cct_ntgrl_degree,
                                                   cct_domain_dim,
                                                   cct_seg_count)) ;
                           }

  int Cd_size              // rtn: Cd array size(either pos, tan, or curv)
    (int row_count,        // in : number of constraint equations
     int surf_dof_count,   // in : constrained pfunc's dof count
     int image_dim)        // in : image space size
                           { return (  (surf_dof_count + image_dim)
                                     * row_count) ; 
                           }
  int Cd_max_size()        // rtn: size of one pos, tan, or curv Cd block
                           { return( Cd_size(cct_surf_dof_count,
                                             cct_surf_dof_count,
                                             cct_image_dim) ) ;
                           }
  int Block_without_Cd_size// rtn: Sum of all arrays except Cd sizes
    (int domain_dim,       // in : assoc pfunc's domain dimension
     int image_dim,        // in : assoc pfunc's image dimension
     int surf_dof_count,   // in : assoc pfunc's surf dof count
#ifdef DS_WN // try to implement cross-tangent gain constraint as a load
     int elem_dof_count,   // in : assoc pfunc's elem dof count
#endif // DS_WN // try to implement cross-tangent gain constraint as a load
     int ntgrl_degree,     // in : gauss integration accuracy for Cd
     int seg_count)        // in : count of one-elem crv-pieces
                           { return(  // cct_dof_map
                                      (  seg_count 
                                       ? Dof_map_size(surf_dof_count)
                                       : 0)

                                    + // cct_seg_bnd_s
                                      Seg_bnd_s_size(seg_count)
#ifdef DS_WN // try to implement cross-tangent gain constraint as a load
                                    + // cct_seg_elem
                                      Seg_elem_size(seg_count)

                                    + // cct_src_C_bas
                                      Src_C_bas_size(ntgrl_degree,
                                                     elem_dof_count,
                                                     seg_count)
                                    + // cct_b
                                      B_size(image_dim,surf_dof_count)
#endif // DS_WN // try to implement cross-tangent gain constraint as a load
                                    + // cct_seg_bnd_W
                                      Seg_bnd_W_size(image_dim,seg_count)
                                    + // cct_src_s_pts
                                      Image_pt_count(ntgrl_degree,
                                                     seg_count) 
                                    + // cct_src_C_pts, cct_src_Cs_pts
                                      2*Domain_pts_size(ntgrl_degree,
                                                        domain_dim,
                                                        seg_count)
                                    + // cct_src_W_pts,  cct_out_W_pts
                                      // cct_src_Wn_pts, cct_out_Wn_pts 
                                      // cct_src_Wnn_pts,cct_out_Wnn_pts
                                      6*Image_pts_size(ntgrl_degree,
                                                       image_dim,
                                                       seg_count)
                                    + // cct_pick_pt
                                      (seg_count ? image_dim : 0)
                                   ) ;
                           } // end Block_without_Cd_size()

  int Block_size           // rtn: Sum of all array sizes
    (int surf_dof_count,   // in : constrained pfunc's dof count
#ifdef DS_WN // try to implement cross-tangent gain constraint as a load
     int elem_dof_count,   // in : constrained pfunc's elem dof count
#endif // DS_WN // try to implement cross-tangent gain constraint as a load
     int C_row_count,      // in : number of Cx=d rows in SPAposition cstrn
     int Cn_row_count,     // in : number of Cx=d rows in tangent cstrn
     int Cnn_row_count,    // in : number of Cx=d rows in curvature cstrn
     int domain_dim,       // in : assoc pfunc's domain dimension
     int image_dim,        // in : assoc pfunc's image dimension
     int ntgrl_degree,     // in : gauss integration accuracy for Cd
     int seg_count)        // in : count of one-elem crv-pieces
                           { return(  Block_without_Cd_size(domain_dim,    
                                                            image_dim,     
                                                            surf_dof_count,
#ifdef DS_WN // try to implement cross-tangent gain constraint as a load
                                                            elem_dof_count,
#endif // DS_WN // try to implement cross-tangent gain constraint as a load
                                                            ntgrl_degree,  
                                                            seg_count)     
                                    + // size for Cd_pos, Cd_tan, Cd_curv
                                      Cd_size(  C_row_count 
                                              + Cn_row_count
                                              + Cnn_row_count,
                                              surf_dof_count,
                                              image_dim)
                                   ) ; 
                           }
  // DS_crv_cstrn internal side-effect functions

  int                      // rtn: cct_dof_count value
   Calc_dof_map            // eff: calc used_dof_map and
    () ;
                           // use: Called by Deflt Cnstrctr
  int                      // rtn: 0=success,-1=NULL_srcs,-2=bad_dpts
   Calc_src_pts            // eff: calc src u,uv,xyz pt arrays
   (int calc_src_s_flag,   // in : 1=calc src_s vals,0=don't
    int calc_src_C_flag);  // in : 1=calc src_C vals, 0=don't

  int                      // rtn: 0=ok,-1=no pfunc,-2=bad dpts,-3=bad val_flag
   Calc_constrained_W_pts  // eff: calc W vals from constrained shape
   (DS_EVAL val_flag,      // in :  one of
                           //  pfn_X    = W only
                           //  pfn_Xu   = W, Wn, and Wt
                           //  pfn_Xuu  = W, Wn, Wt, and Wnn
   int parent_flag) ;      // in : used by seams only
                           //      1=calc pts from parent shape
                           //      0=calc pts from child shape
                           // eff: load DS_pfunc::Dscr with
                           // ordered:[W   [pt0,..,ptN]]
                           //         [Wt  [pt0,..,ptN]]
                           //         [Wn  [pt0,..,ptN]]
                           //         [Wnn [pt0,..,ptN]]

  void                     // use: Called by Calc_src_pts()
   Get_s_pts_from_seg_bnds // eff: calc cct_src_s_pts from
    () ;                   //      Seg_bnds and ntgrl_degree

  int                      // use: Called by Calc_src_pts()
   Get_CW_pts_from_CW_func // eff: calc uv & xyz_pts from func_desc
    (int calc_src_C_flag); // in : 1=calc src_C vals, 0=don't
                           // use: Called by Calc_src_pts()
  int                      // rtn: 0=success,-1=no_src_pfunc,-2=bad_dpt
   Get_C_pts_from_C_pfunc  // eff: calc cct_src_C_pts from uv_desc
     () ;

                           // use: Called by Calc_src_pts()
  int                      // rtn: 0=success,-1=no_src_C_pfunc,-2=bad_dpts
   Get_seg_bnd_W_pts_from_C_pts // eff: Build cct_seg_bnd_W from uv_desc
     () ;


#ifdef DS_WN // try to implement cross-tangent gain constraint as a load
                           // use: Called by Calc_src_pts()
  int                      // rtn: 0=success,-1=no_pfunc,-2=bad_dpt
   Get_seg_elem_from_C_pts // eff: Build cct_seg_elem from uv_pts
    () ;

#endif // DS_WN // try to implement cross-tangent gain constraint as a load

                           // use: Called by Calc_src_pts()
  int                      // rtn: 0=success,-1=no_pfunc,-2=bad_src_pt
   Get_W_pts_from_C_pts    // eff: calc cct_src_W_pts from uv_pts
    () ;
                           // use: Called by Calc_src_pts()
  int                      // rtn: 0=ok,-1=bad_src_pfunc,-2=bad_src_pt
   Get_W_pts_from_W_pfuncs // eff: calc cct_src_W_pts, 
    () ;                   //           cct_src_Wn_pts,
                           //           cct_src_Wnn_pts from src_pfuncs

  void 
   Clear_d() ;             // eff: clear all d entries in cct_d
  void 
   Clear_Cd();             // eff: clear all d entries in cct_Cd
  void
   Zero_Cd() ;             // eff: set all entries in C and d cache to 0

  void                     // use: called by Default Constructor
   Calc_Cd                 // eff: calc and store Cd arrays with
                           //      d_stored = Int_ds(R(s)*Phi(s))
                           //      d_total = d_stored - C*P
    (int ) ;               // in : pfunc_state = used when debug checking

  void                     // use: called by Default Constructor
   Calc_d() ;              // eff: calc d of Cx=d
                           //      d_stored = Int_ds(R(s)*Phi(s))
                           //      d_total = d_stored - C*P

                           // use: when cstrn is off i.e. if(Is_off()) 
  int                      // rtn: 0=success, -1=bad_dpts
   Calc_out_W_pts          // eff: calc ipts when X or P changes
    () ;

  int                      // ret: 0 for (d==CX) else (d!=CX)
   Compare_d_with_CX       // eff: compare d to CX (X=pfunc->Dof_vec)
   (int pfunc_state,       // in : used to exclude tracking seams           
    double &max_err) ;      // out: max ri = abs(di - CijXj)

  double                   // rtn: max_dist(Src_W_pts,Out_W_pts)  
   Compare_src_to_out_W_pts// eff: check cstrn tolerance 
    (double &max_dist,     // out: max dist error
     double &max_angle) ;  // out: max angle error

  void
   Find_max_gap            // eff: Find maximum gap between curve and target
    (double &max_dist,     // out: largest displacement error
     double &s_val,        // out: curve param it occurs at
     double *const uv);    // out: uv value it occurs at

  virtual                  // bas: DS_cstrn virtual function
   int                     // rtn: 1=changes allowed,0=not allowed,-1=bad_dpts
   Set_on_off              // eff: When allowed, Turn cstrn on or off
    (int on_off,           // in : 0=off,1=on
     int pfunc_state=0) ;  // orof: DS_PFN_CHANGED_X      = X val change
                           //       DS_PFN_CHANGED_P_ON   = Set def shape
                           //       DS_PFN_CHANGED_P_OFF  = Clear def shape
                           //       DS_PFN_CHANGED_DOMAIN = Domain change
                           //       DS_PFN_CSTRN_ON = crv_cstrn turned on
                           //       DS_PFN_TRACKING = crv_cstrn tracking parent

  // DS_crv_cstrn implementations of DS_cstrn virtual functions
  //  non-side-effect functions

  double                   // bas: DS_cstrn virtual function
   Dist2_to_iline          // ret: dist**2 between cstrn and iline
                           //      iline = pi0 + u(pi1-pi0)
    (double *pi0,          // in : image space pt, Sized:[image_dim]
     double *pi1,          // in : image space pt, Sized:[image_dim]
     DS_CST_PT_INDEX       // out: image_pt index in cstrn for closest pt
            &pt_index,     
     double &uu) ;         // out: iline param for pt closest to cstrn
/* no longer in base class
  virtual void             // bas: DS_cstrn virtual function
   Set_image_pt            // eff: copies ipt into cstrn's image pt
   (double *,              // in : ipt = pt to copy, Sized:Image_dim()
    double ,               // in : tan_display_gain, tpt = bpt+tan_display_gain*tvec
    int )                  // in : pt_index = index of image_pt to update
                           { }

  virtual                  // bas: DS_cstrn virtual function
   int                     // rtn: 0=success,-1=bad_dpt
   Set_domain_pt           // eff: copies dpt into pst_domain_pt
   (double *,              // in : dpt = pt to copy, Sized:Domain_pt_size()
    double ,               // in : tan_display_gain tpt = bpt+tan_display_gain*tvec
    int )                  // orof: 1=calc new base_pt
                           //       2=calc new tang_vec
                           //       4=calc new Wuu,Wuv,Wvv vecs
                           //       0=save old base_pt and tang_vec
                           { return(0) ; }
*/
  // implementations of DS_cstrn virtual functions
  // DS_crv_cstrn side-effect functions

  virtual void             // bas: DS_cstrn pure virtual function
   Shift_domain            // eff: shift the basis domain values
    (double *du,           // in : domain shift SPAvector, Sized:Domain_dim()
     DS_dmod *dmod) ;      // in : for this dmod 

  virtual                  // bas: DS_cstrn pure virtual function
   int                     // rtn: 0=done,-1=scale too small
   Scale_domain            // eff: scale the basis domain values
    (double gain,          // in : domain scale factor
     DS_dmod *dmod) ;      // in : for this dmod 

  virtual                  // bas: DS_cstrn virtual function
   int                     // rtn: 0=success,-1=bad_dpts
   Update_pts              // eff: update all cstrn cached data
                           //      after changes in pfunc
  (double tan_display_gain,// in : tang_pt = base_pt + tan_display_gain*tang_vec
   double C2_display_gain, // in : curv_pt = base_pt + C2_display_gain *tang_vec
   int pfunc_state) ;      // orof: DS_PFN_CHANGED_X      = X val change
                           //       DS_PFN_CHANGED_P_ON   = Set def shape
                           //       DS_PFN_CHANGED_P_OFF  = Clear def shape
                           //       DS_PFN_CHANGED_DOMAIN = Domain change
                           //       DS_PFN_CSTRN_ON= crv_cstrn toggled on
                           //       DS_PFN_TRACKING= crv_cstrn tracking parent

  virtual void             // bas: DS_cstrn virtual function
   Build_A_row             // eff: build one cstrn's Cx=d eqn rows
    (DS_dmod *dmod,        // in : shape to constrain
     DS_eqns *eqns);       // in : holder of Ax=b, Cx=d eqns

  virtual void             // bas: DS_cstrn virtual function
   Build_b_row             // eff: build one cstrn's Cx=d eqn rows
    (DS_dmod *dmod,        // in : shape to constrain
     DS_eqns *eqns);       // in : holder of Ax=b, Cx=d eqns

  virtual void             // bas: DS_cstrn virtual function
   Build_Cd_row            // eff: build one pt-cstrn's Cx=d eqn row
    (DS_dmod *dmod,        // in : shape to constrain
     DS_eqns *eqns,        // in : holder of Ax=b, Cx=d eqns
     int &next_Cd_row,     // i/o: next available Cx=d row
     int refine_flag=0) ;  // in : 0=use state bits as is
                           //      1=treat links as fixed

  virtual void             // bas: DS_cstrn virtual function
   Build_d_row             // eff: build one pt-cstrn's d of Cx=d eqn
    (DS_dmod *dmod,        // in : shape to constrain
     DS_eqns *eqns,        // in : holder of Ax=b, Cx=d eqns
     int &next_Cd_row) ;   // i/o: next available Cx=d row

#ifdef DS_WN // try to implement cross-tangent gain constraint as a load
  virtual void             // bas: DS_cstrn virtual function                        
   Build_L_Lc_row          // eff: build one pt-cstrn's Lxi=Lc eqn rows             
    (DS_dmod *dmod,        // in : shape to constrain
     DS_symeq *symeq,      // in : holder of Ax=b, Cx=d eqns
     int next_L_Lc_row) ;  // in : 1st available row of Li xi=Lc

#else // DS_WN  // try to implement cross-tangent gain constraint as a load

  virtual void             // bas: DS_cstrn virtual function                        
   Build_L_Lc_row          // eff: build one pt-cstrn's Lxi=Lc eqn rows             
    (DS_dmod *,            // in : dmod = shape to constrain
     DS_eqns *,            // in : eqns = holder of Ax=b, Cx=d eqns
     int )                 // in : next_L_Lc_row = 1st available row of Li xi=Lc
                           { }

#endif // DS_WN // try to implement cross-tangent gain constraint as a load

  virtual void             // bas: DS_cstrn virtual function
   Build_Lc_row            // eff: build one pt-cstrn's d of Cx=d eqn
    (DS_dmod *,            // in : dmod = shape to constrain
     DS_eqns *,            // in : symeq = holder of Ax=b, Cx=d eqns
     int )                 // in : next_L_Lc_row = 1st available row of Li xi=Lc
                           { }

  void
  Sub_CP_from_d            // eff: set d = d-C*P for default shapes
  (DS_pfunc *pfunc,        // in : shape being constrained
   DS_eqns *eqns,          // in : holder of Ax=b, Cx=d eqns
   int next_Cd_row,        // in : 1st available row in min_Cd Cx=d
   int Cd_flag,            // in : 0=SPAposition  constraint
                           //      1=tangent   constraint
                           //      2=curvature constraint
   int *used_dof_map) ;    // in : used_dof_index to global_index,
                           //      inverse of cct_dof_map,
                           //      sized:[cct_dof_count]
#ifndef DS_USE_SYMEQ
#else
  virtual                  // bas: DS_cstrn virtual function
  int                      // rtn: free_dof_count, -2=no effect, or -1=error                     
   Mark_this_free_dofs     // eff: mark all free dofs and elems in input
   (DS_dmod *,             // in : dmod = shape to constrain
    int ,                  // in : dof_count = sizeof dof_map
    int *,                 // i/o: dof_map = dof_map for constrained pfunc
    int &free_dof_count,   // out: dofs changed from fixed to free
    int ,                  // in : elem_count = sizeof elem_state
    int *,                 // i/o: elem_state = 0=free to move, 1=fixed
    int &free_elem_count)  // out: elems changed from fixed to free
                           { free_dof_count = 0 ;
                             free_elem_count = 0 ;
                             return -2 ; 
                           }
#endif //DS_USE_SYMEQ

  virtual void             // bas: DS_cstrn virtual function
  Bad_track_response       // eff: reset tracked cstrns. 
  (double ,                // in : tan_display_gain = tang_pt = base_pt + tan_display_gain*tang_vec
   double                  // in : C2_display_gain = curv_pt = base_pt + C2_display_gain *tang_vec
   )                       //      Called when tracking yields cstraint 
                           //      confilcts
                           { }

  virtual                  // bas: DS_cstrn virtual function
  int                      // rtn: 0=success,-1=file failure
   Dump                    // eff: write all internal vals to file
    (char *filename,       // in : filename = file to write
     FILE *fp)             // in : existing file or NULL
                           { // local file access
                             int close = 0 ;
                             if(NULL == fp){fp = fopen(filename, "w");
                                            close = 1 ;
                                           }
                             if (!fp) return(-1) ;

                             DS_cstrn::Dump(filename,fp);
                             if(close) fclose(fp) ;
                             return(0) ; 
                           }

#ifdef DS_WN // try to implement cross-tangent gain constraint as a load
  void                     // bas: DS_cstrn virtual function
   Build_this_A            // eff: Add one cstrn's terms to b in Ax=b
    (DS_dmod *,            // in : dmod = shape to constrain
     DS_eqns *eqns) ;      // in : holder of Ax=b, Cx=d eqns

  void                     // bas: DS_cstrn virtual function
   Build_this_b            // eff: Add one cstrn's terms to b in Ax=b
    (DS_dmod *,            // in : dmod = Shape to constrain
     DS_eqns *eqns) ;      // in : holder of Ax=b, Cx=d eqns

#endif // DS_WN // try to implement a cross tangent gain constraint as a load
 
} ; // end class DS_crv_cstrn

//=============================================================
//  class DS_link_cstrn
//=============================================================

// Mesh Link Constraints (link_cstrns) are used to constrain the
// boundary of two different surfaces in a multi-surface dmesh
// patch to stay connected with either C0 or C1 continuity.
// The link_cstrn is implemented by keeping track of 4 different
// groups of distinct sets of points.  The 4 groups include
// the uv points on both surfaces ordered so that we know which
// uv point on one surface corresponds with which uv point on the
// other surface and the 3 space xyz point projections of those 
// uv points.
// Let C(r) = [u(r)] be the curve on one surface that connects to
//            [v(r)]  
//     K(t) = [u(t)] the curve on the 2nd surface.
//            [v(t)]
// Each point on the curves must correspond to a point on the
// other curve.  Each [uv] point on the surfaces map to points
// in xyz space through the surface parametric functions,
//     W(u,v) = [x(u,v)] for the first surface and
//              [y(u,v)]
//              [z(u,v)]
//     V(u,v) = [x(u,v)] for the second surface.
//              [y(u,v)]
//              [z(u,v)]
// We require some common parametric mapping r(s) and
// t(s) that defines the correspondence between points on the
// first and second surface curves.  The constraint we
// seek is of the form,
//
//    W(C(r(s))) = V(K(t(s))) for all values of s.
//
// The constraint is made a positive definite scalar by
// squaring it and integrating over the length of the curves.
//   err = Integral( (W(s) - V(s))**2 ) ds
// When the err is 0 the constraint is satisfied. Also when
// the constraint is violated, the err will always be a positive
// number, i.e. it is positive definite.
//
// The functions W(u,v) and V(u,v) are functions of the unknown
// dofs of the problem in the form,
//    W = Sum_i(wi*Pi_i(u,v)) and V = Sum_j(wj*Pj_j(u,v))
// where Pi and Pj are the basis functions for each parametric
// surface and wi and wj are the unknown dofs.  The
// surfaces may have a default shape of the form
//    W = Qw + Pw  and Qw = W - Pw
//    V = Qv + Pv  and Qv = V - Pv
// where P is the default rest shape and Q is the displacement.
// Then
//   W = Sum_i((qi+pi)*Pi_i(u,v)) and 
//   V = Sum_j((qj+pj)*Pj_j(u,v))
//
// Substituting the surface definitions into the constraint
// equation yields a constraint equation given in the 
// unknowns of the problem.
//
//  err = Integral( (  Sum_i((qi+pi)*Pi_i(s)) 
//                   - Sum_j((qj+pj)*Pj_j(s)) )**2 ) ds
//
// Since err is positive definite, we can find a set of
// equivalent linear equations by minimizing the err.  
// To minimize we take the partial with respect to each
// qi and qj and set equal to 0. Note: each pi and pj are
// constants.
//
// d(err)/dqi_k = 0 = Integral( 2*(  Sum_i((qi+pi)*Pi_i(s)) 
//                                 - Sum_j((qj+pj)*Pj_j(s)) )
//                               * Pi_k(s) ) ds
// d(err)/dqj_m = 0 = Integral(-2*(  Sum_i((qi+pi)*Pi_i(s)) 
//                                 - Sum_j((qj+pj)*Pj_j(s)) )
//                               * Pj_m(s) ) ds
//
// These equations can be written in SPAmatrix form as
//  [C11 | C12][qi]    [C11 | C12][pi]
//  [----+----][--] = -[----+----][--]
//  [C21 | C22][qj]    [C21 | C22][pj]
//
// where C11_kl = Integral( Pi_k(s) * Pi_l(s)) ds
//       C12_kn = Integral(-Pi_k(s) * Pj_n(s)) ds
//       C21_ml = Integral(-Pj_m(s) * Pi_l(s)) ds
//       C22_mn = Integral( Pj_m(s) * Pj_n(s)) ds
//
// Indices k and l range over the number of dofs in surface 1.
// Indcies m and n range over the number of dofs in surface 2.
//
// Note that Transpose(C12) = C21 and that C11 and C22 are 
// symmetric. The SPAmatrix C is a positive definite symmetric
// SPAmatrix.
//
// All integrals are taken using gauss integration.
// 
// The tangent constraint is enforced by ensuring that the
// Integral( (Wn(s) - Vn*(s))**2) ds = 0.
//
// To build the curve SPAposition and tangent constraints we need to
// have the following information.
//
// The source data:  C(s), K(s), Pi(uv), and Pj(uv).
// We choose to represent C(s) and K(s) as a set of boundary
// points supplied to the object at construction time described
// by a pfunc curve and a set of curve domain values that specify
// the segment boundaries.
// The integrations are executed by evaluating the 
// gauss points scaled to fit within each input segment.
//
// DS_CSTRN_SRC records the source for link_cstrn.  

// Current ACIS link_cstrn sources, privledges, and src_data
/// DS_CSTRN_SRC   delete  stop   src_  src_CW   comment
//                  able   able   data  _func
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
// ds_link_cstrn     NO    YES    YES  NO   crv joins 2 dmesh faces

// link_cstrn shape description.
// A link_cstrn is described in the following manner
//   1. A pair of ptrs to surface pfuncs describing the face shapes.
//   2. A pair of domain space pfunc curves, one on each surface.
//   3. A pair of segment boundaries marking each corresponding
//      segment on each domain space pfunc curves.  Each
//      domain space curve has to be segmented where ever the
//      surface pfunc descriptions have a discontinuity, i.e.
//      an element boundary.  The segments are assumed to
//      correspond to one another and the mapping within
//      each segment interior to its corresponding segment
//      is assumed to be affine.  That is a simple linear, offset
//      and scaling factor.
//   4. The constraint shows up in 3 locations in the 
//      dmod hierarchy.
//      4.a The constraint is pointed to by a DS_mlink which
//          is on a list of DS_mlinks contained in the
//          DS_dmesh structure.
//      4.b The constraint is on the list of constraints
//          belonging to the first dmod that it connects.
//      4.c The constraint is on the list of constraints
//          belonging to the second dmod that it connects.
//      The constraints are on the DS_dmesh->DS_mlink list
//      so that the link constraints can be built.  The 
//      constraints are on the DS_dmod->DS-cstrn list for
//      graphics, interaction, and so that a link constraint
//      can act like a curve constraint when its behavior
//      is set to 'fixed'.
//
// The constraint is built by iterating the following sequence
// over every segment.
//   1. compute all the gauss point r and t values in both segments
//   2. Project r and t points into surface domain C(r) and K(t)
//      points.
//   3. Use the C and K points to compute the basis functions
//      for both surfaces, Pi(C(r)) and Pj(K(t)).
//   4. Use the basis function evaluations to compute and
//      store C11, C12, and C22.

// NOTE: Default shape values, P, in the constrained pfunc's
//       definition are not used in building any of the cached
//       link_cstrn data. The effect of default shapes is
//       stored completely in the SPAmatrix multiply d=C*q which
//       is computed when requested in build_Cd_row() and
//       build_d_row().
//
// DS_LINK_CSTRN graphics:  The link_cstrn graphics consists of two
//                          polylines.
//  cst_grobj[0] = polyline_handle for dmod1
//  cst_grobj[1] = polyline_handle for dmod2
//          
class DS_enforcement_mechanism;
class DS_link_cstrn : public DS_cstrn  // identifier = lct 
{
   DS_enforcement_mechanism *lct_mech;
   DM_xtan_trans             lct_xtan_trans;

   int lct_surf1_dof_count;// [array_size] dof count in assoc pfunc
   int lct_surf2_dof_count;// [array_size] dof count in assoc pfunc

   int lct_C1_dof_count ;  // [array_size] surf1 pos dof count
   int lct_C2_dof_count ;  // [array_size] surf2 pos dof count

   int lct_Cn1_dof_count ; // [array_size] surf1 tan dof count
   int lct_Cn2_dof_count ; // [array_size] surf2 tan dof count

                           // TODO:??? curvature not yet supported
   int lct_Cnn1_dof_count; // [array_size] surf1 C2 dof count
   int lct_Cnn2_dof_count; // [array_size] surf2 C2 dof count

   SPA_DM_flipped_state lct_flipped_state; // oneof SPA_DM_unknown_flip
                                       //       SPA_DM_unflipped
                                       //       SPA_DM_flipped

   int lct_flipped_coords; // 0=surfs have same uv coord handedness 
                           // 1=surfs have diff uv coord handedness

   int lct_domain_dim ;    // [array_size] surf pfunc's domain size
   int lct_image_dim ;     // [array_size] surf pfunc's image size
   int lct_ntgrl_degree ;  // [array_size] segment ntgrl accuracy
   int lct_seg_count ;     // [array_size] the curve is divided  
                           //   into segments on elem boundaries

   int lct_src1_dir ;      // +1=traverse in param direction
                           // -1=traverse opposing param direction
                           //  0=collapsed to a point
   int lct_src2_dir ;      // +1=traverse in param direction
                           // -1=traverse opposing param direction
                           //  0=collapsed to a point

   DS_pfunc *              // Shape 1 source curve C description.
       lct_src1_C_pfunc ;  // used to map crv-domain pts to   
                           // surf-domain pts, i.e. r pts to C(r),
                           // Cu(r), and Cv(r).

   DS_pfunc *              // Shape 2 source curve C description.
       lct_src2_C_pfunc ;  // used to map crv-domain pts to   
                           // surf-domain pts, i.e. t pts to K(t),
                           // Ku(t), and Kv(t).

  DS_pfunc *               // Optional source curve pos descriptions
      lct_src1_W_pfunc ;   // When present it is used to generate
                           // the src SPAposition values (W1(s)) and (W2(s)).
  DS_pfunc *               // Its expected image_dim is cct_image_dim.
      lct_src2_W_pfunc ;   // W1(s) = src1_W_pfunc(s).
                           // W2(s) = src2_W_pfunc(s).

  DS_pfunc *               // Optional source curve cross-tang descriptions
      lct_src1_Wn_pfunc ;  // When present it is used to generate
                           // the src SPAposition values (Wn1(s)) and (Wn2(s)).
  DS_pfunc *               // Its expected image_dim is cct_image_dim.
      lct_src2_Wn_pfunc ;  // Wn1(s) = src1_Wn_pfunc(s).
                           // Wn2(s) = src2_Wn_pfunc(s).

  DS_pfunc *               // Optional source curve curvature descriptions
      lct_src1_Wnn_pfunc ; // When present it is used to generate
                           // the src SPAposition values (Wnn1(s)) and (Wnn2(s)).
  DS_pfunc *               // Its expected image_dim is cct_image_dim.
      lct_src2_Wnn_pfunc ; // k1(s) = src1_Wnn_pfunc(s).
                           // k2(s) = src2_Wnn_pfunc(s).
                           // we enforce k*n(s) = Wnn_pfunc(s)*n.
                           //   where n(s)=cross(Wn(s),Wt(s))
                           //         k(s)=  size(cross(Wn,Wnn))
                           //              / size(Wn)**3  

   DS_dbl_block lct_Cperp1 ;// Remember uv projections of xyz vectors
   DS_dbl_block lct_Cperp2 ;// perpendicular to edge in surfaces 1 and 2

   DS_dmod *               // optional tracking dmod to
       lct_src1_W_dmod ;   // reshape lct_src_W_pfunc

   DS_dmod *               // optional tracking dmod to
       lct_src1_Wn_dmod ;  // reshape lct_src_W_pfunc

   DS_dmod *               // optional tracking dmod to
       lct_src1_Wnn_dmod ; // reshape lct_src_W_pfunc

   DS_dmod *               // optional tracking dmod to
       lct_src2_W_dmod ;   // reshape lct_src_W_pfunc

   DS_dmod *               // optional tracking dmod to
       lct_src2_Wn_dmod ;  // reshape lct_src_W_pfunc

   DS_dmod *               // optional tracking dmod to
       lct_src2_Wnn_dmod ; // reshape lct_src_W_pfunc

  void (*lct_src_CW_func)  // Optional source func to calc C and W crvs
    (void *src_data,       // in : app defined data
     double s,                // in : edge s param value
     double *C,               // out: surface C=[u,v] pt value for s
     double *Cs,              // out: surface dC/ds vec in surface for s
     double *W,               // out: image space W=[x,y,z] pt value for s
     double *Wu,              // out: image space dW/du tangent for s
     double *Wv,              // out: image space dW/dv tangent for s
     double *Wuu,             // out: image space d2W/du2 for s
     double *Wuv,             // out: image space d2W/dudv for s
     double *Wvv,             // out: image space d2W/dv2 for s
     double &dist2) ;         // out: dist**2(xyz,proj_to_3d(uv))
                           // note: for ACIS  use: DS_true_edge_eval()

   // DS_link_cstrn internal array storage - the order of array
   // declarations is used in Size arrays - Coordinate any changes.
   int *lct_C1_dof_map;    // surf1_dof_index to link_C_index mapping
                           //  map[i] == -1  means an unused dof value
                           //  map[i] >= 0   means a used dof value
                           //  e.g. [ 0 -1 1 2 -1 -1 3 ... ]
                           // sized:[lct_surf1_dof_count]
   int *lct_C2_dof_map;    // surf2_dof_index to link_C_index mapping
                           // sized:[lct_surf2_dof_count]

   int *lct_Cn1_dof_map;   // surf1_dof_index to link_Cn_index mapping
                           // sized:[lct_surf1_dof_count]
   int *lct_Cn2_dof_map;   // surf2_dof_index to link_Cn_index mapping
                           // sized:[lct_surf2_dof_count]

   int *lct_Cnn1_dof_map;  // surf1_dof_index to link_Cnn_index mapping
                           // sized:[lct_surf1_dof_count]
   int *lct_Cnn2_dof_map;  // surf2_dof_index to link_Cnn_index mapping
                           // sized:[lct_surf2_dof_count]

                           // use: each used dof is assigned an
                           //      index in order of appearance
                           //      unused dofs == -1
                           //      ex: [-1 0 -1 1 -1 -1 2]
                           //      dofs 1,3,6 are being used
                           //      dofs 0,2,4,5 are not

   double *lct_seg1_bnd_s; // Curve param values on segment end-pts.
   double *lct_seg2_bnd_s; // Sized:[lct_seg_count+1] 
                           // Segments are the link_cstrn pieces that
                           // lie between the surface pfunc knot
                           // boundaries. The 1st and last entries
                           // must equal the link_cstrn param range.

   double *lct_seg1_bnd_W; // Src xyz positions for each seg_bnd
   double *lct_seg2_bnd_W; // in lct_seg_bnd_s.  These numbers are
                           // not used in crv-cstrn processing. They
                           // are saved for debugging and rendering.
                           // Sized:[lct_image_dim*(lct_seg_count+1)]

   // on a segment by segment basis we need to store
   double *lct_src1_s_pts; // crv1 u vals for segment gauss pts.
   double *lct_src2_s_pts; // crv2 u vals for segment gauss pts.
                           // Sized:[Segment_pt_count(3)]
                           // store gauss_pts and upper bound_pt
                           // Stored:[u0,u1..,un]

   double *lct_src1_C_pts; // crv1 C=uv vals. 
   double *lct_src2_C_pts; // crv1 C=uv vals.
                           // Sized:[2*Segment_pt_count(3)]
                           // store gauss_pts and upper bound_pt
                           // Stored:[u0,u1..,un,v0,v1..,vn]

   double *lct_src1_Cs_pts;// crv1 Cs=dC/ds vals. 
   double *lct_src2_Cs_pts;// crv1 Cs=dC/ds vals.
                           // Sized:[2*Segment_pt_count(3)]
                           // store gauss_pts and upper bound_pt
                           // Stored:[du0,du1..,dun,dv0,dv1..,dvn]

   // target shapes for whole curve in case behavior is set to FIXED
   // these arrays store: [crv_end_pt,seg_gauss_pts,crv_end_pt]
   double *lct_src1_W_pts; // crv 1 xyz vals. Sized:[Image_pts_size()]
                           // stored:[xyz0, xyz1, ...]
   double *lct_src2_W_pts; // crv 2 xyz vals. Sized:[Image_pts_size()]
                           // stored:[xyz0, xyz1, ...]
   double *lct_src1_Wn_pts;// crv -Cv*dW/du+Cu*dW/dv vals. Sized:[Image_pts_size()]
                           // stored:[Wn0, Wn1, ...]
   double *lct_src2_Wn_pts;// crv -Cv*dW/du+Cu*dW/dv vals. Sized:[Image_pts_size()]
                           // stored:[Wn0, Wn1, ...] 
   double *lct_src1_Wnn_pts;// Cv*Cv*Wu-2*Cu*Cv*Wuv+Cv*Cv*Wvv. Sized:[Image_pts_size()]
                           // stored:[Wn0, Wn1, ...]
   double *lct_src2_Wnn_pts;// Cv*Cv*Wu-2*Cu*Cv*Wuv+Cv*Cv*Wvv. Sized:[Image_pts_size()]
                           // stored:[Wn0, Wn1, ...] 

   // for graphical and debugging - store an entire curve's worth
   double *lct_out1_W_pts; // crv1 xyz vals.      
   double *lct_out2_W_pts; // crv2 xyz vals.
                           // Sized:[Image_pts_size()]
                           // Stored:[xyz0,xyz1..,xyzn]

   double *lct_out1_Wn_pts;// crv1 -Cv*dW/du+Cu*dW/dv vals. 
   double *lct_out2_Wn_pts;// crv2 -Cv*dW/du+Cu*dW/dv vals.
                           // Sized:[Image_pts_size()]
                           // Stored:[dWu0,dWu1..,dWun]

   double *lct_out1_Wnn_pts;// Cv*Cv*Wu-2*Cu*Cv*Wuv+Cv*Cv*Wvv. Sized:[Image_pts_size()]
                           // stored:[Wn0, Wn1, ...]
   double *lct_out2_Wnn_pts;// Cv*Cv*Wu-2*Cu*Cv*Wuv+Cv*Cv*Wvv. Sized:[Image_pts_size()]
                           // stored:[Wn0, Wn1, ...] 

   double *lct_pick_pt ;   // Sized:[ lct_image_dim]. Set for display
                           // by Dist2_to_iline().

   // Crv cstrn's C SPAmatrix entries
   int *lct_dmesh_C_map;   // map link_C_index to dmesh_C_index vals
                           // use  : temp area modified as needed
                           //        ordered:[C1_map, C2_map]
                           // sized:[  max(lct_C1_dof_count,
                           //              lct_Cn1_dof_count,
                           //              lct_Cnn2_dof_count)
                           //        + max(lct_C2_dof_count,
                           //              lct_Cn2_dof_count,
                           //              lct_Cnn2_dof_count)]

   double *lct_C11 ;       // sized: n*(n+1)/2 
   double *lct_C12 ;       // sized: n*m
   double *lct_C22 ;       // sized: m*(m+1)/2
                           // where n=lct_C1_dof_count
                           //       m=lct_C2_dof_count

   double *lct_Cn11 ;      // sized: nn*(nn+1)/2 
   double *lct_Cn12 ;      // sized: nn*mm
   double *lct_Cn22 ;      // sized: mm*(mm+1)/2
                           // where nn=lct_Cn1_dof_count
                           //       mm=lct_Cn2_dof_count

   double *lct_Cnn11 ;     // sized: nnn*(nnn+1)/2 
   double *lct_Cnn12 ;     // sized: nnn*mmm
   double *lct_Cnn22 ;     // sized: mmm*(mmm+1)/2
                           // where nnn=lct_Cnn1_dof_count
                           //       mmm=lct_Cnn2_dof_count

   double *lct_d1 ;        // sized: n*lct_image_dim
   double *lct_d2 ;        // sized: m*lct_image_dim
   double *lct_dn1 ;       // sized: nn*lct_image_dim
   double *lct_dn2 ;       // sized: mm*lct_image_dim
   double *lct_dnn1 ;      // sized: nnn*lct_image_dim
   double *lct_dnn2 ;      // sized: mmm*lct_image_dim

  // ACIS note: src_data is being used to secretly store ptrs
  //            to a pair of COEDGEs and FACEs so that topology 
  //            boundaries can be mapped back to their sources.
  //            For use by commit_2owner() and for use by the src_CW_func
  //            callbacks.

  public:
   // DS_link_cstrn Constructor, copy, Make_copy, Size_arrays, Destructor

DS_link_cstrn              //      Default Constructor
  (DS_enforcement_mechanism* & mech,// in : link cstrn vs link load
                                    // out: ptr value copied in then NULL'ed
   DS_dmod *dmod1          =NULL,// in : constraint surface shape 1
   DS_dmod *dmod2          =NULL,// in : constraint surface shape 2
   DS_pfunc *src1_C_pfunc  =NULL,// in : Shape 1 domain crv for C vals
   DS_pfunc *src2_C_pfunc  =NULL,// in : Shape 2 domain crv for C vals
   DS_pfunc *src1_W_pfunc  =NULL,// opt: Image  crv to calc W pt vals         
   DS_pfunc *src1_Wn_pfunc =NULL,// opt: Image  crv to calc Wn vec vals
   DS_pfunc *src1_Wnn_pfunc=NULL,// opt: function to calc k curvature vals
   DS_pfunc *src2_W_pfunc  =NULL,// opt: Image  crv to calc W pt vals         
   DS_pfunc *src2_Wn_pfunc =NULL,// opt: Image  crv to calc Wn vec vals
   DS_pfunc *src2_Wnn_pfunc=NULL,// opt: function to calc k curvature vals
   void (*src_CW_func)           // opt : func to calc curve C and W pt vals
    (void *src_data,               // in : app defined data
     double s,                     // in : edge s param value
     double *C,                    // out: surface C=[u,v] pt value for s
     double *Cs,                   // out: surface dC/ds vec in surface for s
     double *W,                    // out: image space W=[x,y,z] pt value for s
     double *Wu,                   // out: image space dW/du for s
     double *Wv,                   // out: image space dW/dv for s
     double *Wuu,                  // out: image space d2W/du2 for s
     double *Wuv,                  // out: image space d2W/dudv for s
     double *Wvv,                  // out: image space d2W/dv2 for s
     double &dist2)        =NULL,  // out: dist**2(xyz,proj_to_3d(uv))
   void *src1_data         =NULL,// opt: Passed to src_CW_func callbacks
   void *src2_data         =NULL,// opt: Passed to src_CW_func callbacks
   DS_CSTRN_SRC src_type=        // in : records cstrn origin. oneof:
         ds_link_cstrn,          //      ds_solid_cstrn=shared topo bndry
                                 //      ds_bound_cstrn=unshared topo bndry
                                 //      ds_user_cstrn  =user created cstrn
                                 //      ds_seam_cstrn  =join 2 hierarchical faces
                                 //      ds_link_cstrn  =join 2 multisurf-faces
   int seg_count              =0,// in : number of segments in domain_crv
                                 //      segment=domain_crv piece in 1 elem
   double *src1_seg_bnds   =NULL,// in : segment end-pt ordered param vals.
                                 //      Sized:[seg_count+1]
   double *src2_seg_bnds   =NULL,// in : segment end-pt ordered param vals.
                                 //      Sized:[seg_count+1]
   int behavior=DS_CST_POS_FIXED,// in : orof DS_CST_POS_FREE     DS_CST_TAN_FREE    
                                 //           DS_CST_POS_FIXED    DS_CST_TAN_FIXED   
                                 //     DS_CST_POS_LINKED   DS_CST_TAN_LINKED  
                                 //                                
                                 //     DS_CST_POS_2_FREE   DS_CST_TAN_2_FREE  
                                 //     DS_CST_POS_2_FIXED  DS_CST_TAN_2_FIXED  
                                 //     DS_CST_POS_2_LINKED DS_CST_TAN_2_LINKED
   int tag              =-1,     // in : tag so apps can track cstrns
   DS_cstrn *next1      =NULL,   // in : 1st dmod list pointer
   DS_cstrn *next2      =NULL,   // in : 2nd dmod list pointer
   SPA_DM_flipped_state
      flipped_state = SPA_DM_flip_unknown); // in: indicates relative handedness of two
                                        //     surfaces being linked
                                        //     SPA_DM_unflipped    ==> same handedness;
                                        //     SPA_DM_flipped      ==> opposite handedness;
                                        //     SPA_DM_flip_unknown ==> unspecified by user;
                                        //                           package will guess

                           // Copy Constructor
   DS_link_cstrn           // rtn: deep copy of link_cstrn
    (DS_link_cstrn &       // in : object being copied
       link_cstrn) ;

   DS_link_cstrn &operator=// assignment operator
    (DS_link_cstrn &) ;

   DS_cstrn*               // bas: DS_cstrn virtual function
    Make_copy()            // eff: Virtual DS_cstrn copy function
                           { return( new DS_link_cstrn(*this)) ; }

   void
    Size_arrays            // eff: Size internal arrays
    (int surf1_dof_count=0,// in : constraint surface 1 dof count
     int surf2_dof_count=0,// in : constraint surface 2 dof count
     int C1_dof_count=0,   // in : surf1 pos  used dof count
     int C2_dof_count=0,   // in : surf2 pos  used dof count
     int Cn1_dof_count=0,  // in : surf1 tan  used dof count
     int Cn2_dof_count=0,  // in : surf2 tan  used dof count
     int Cnn1_dof_count=0, // in : surf1 curv used dof count
     int Cnn2_dof_count=0, // in : surf2 curv used dof count
     int domain_dim=0,     // in : assoc pfunc's domain dimension
     int image_dim=0,      // in : assoc pfunc's image dimension
     int ntgrl_degree=0,   // in : gauss integration accuracy for Cd
     int seg_count=0) ;    // in : count of one-elem crv-pieces

   virtual
  ~DS_link_cstrn() ;       // Destructor
                           //   delete lct_src_C_pfunc ;
                           //   Size_arrays() ;

  // Access enforcement mechanism
  virtual DS_enforcement_mechanism* Mech()
   {return lct_mech;}

  // DS_link_cstrn virtual simple data access functions
private:
  virtual                  // bas: DS_cstrn pure virtual function     
   int Line_row_count      // rtn: total non_min_Cd Cx=d row count
    (DS_dmod * /*dmod*/)        // in : dmod = for this dmod 
                           { // all DS_link_cstrn eqns are in min_Cd
                             return(0) ;
                           }

  virtual                  // base: DS_cstrn
   int Mix_count           // rtn: Lxi=Lc row count for one cstrn only
   (DS_dmod *)             // in : dmod = for this dmod
                           { // DS_link_cstrn generate no mixed cstrns
                             return(0) ;
                           }
public:

void                                 // 
Find_link_total_gap         // eff: check cstrn tolerance 
  (int behavior,
   double *dist);       // out: total error
//========================================================
// cstrns only contribute if turned on.
// Is_Cd_contributor, Is_LLc_contributor
// Is_dof_map_contributor, Is_min_map_contributor
// Is_joint_Cd_contributor
//========================================================

  virtual                  // bas: DS_cstrn pure virtual function
  int Is_Cd_contributor    // ret: 1=cstrn adds Cd equations,0=doesn't
    (DS_dmod *dmod,        // in : for this dmod 
     int refine_flag=0)    // in : 0=use state bits as is
                           //      1=treat links as fixed
                           { if(Is_off()) return(0) ;
                             return( (   Is_position (dmod,refine_flag)
                                      || Is_tangent  (dmod,refine_flag)
                                      || Is_curvature(dmod,refine_flag))
                                    ? 1 : 0 ) ;
                           }

  virtual                  // bas: DS_cstrn pure virtual function
  int Is_LLc_contributor   // ret: 1=cstrn adds LLc equations,0=doesn't
    (DS_dmod *)            // in : dmod = for this dmod 
                           { return(0) ;
                           }

  virtual                  // bas: DS_cstrn pure virtual function
  int                      // rtn: 1=cstrn affects min_map,0=doesn't
   Is_dof_map_contributor  // eff: area and link cstrns do,
     ()                    //      pt_cstrns and crv_cstrns do not.
                           { return( (Is_on()  // links only constribute if turned on
                                      /* &&  // and if at least one linked behavior ???
                                      (Is_position_linked( cst_dmod1) || Is_position_linked( cst_dmod2)
                                       Is_tangent_linked(  cst_dmod1) || Is_tangent_linked(  cst_dmod2)
                                       Is_curvature_linked(cst_dmod1) || Is_curvature_linked(cst_dmod2)
                                      )*/
                                     ) ? 1 : 0) ;
                           }

  virtual                  // bas: DS_cstrn pure virtual function
  int                      // rtn: 1=cstrn adds to min_map,0=doesn't
   Is_min_map_contributor  // eff: crv and link cstrns may add,
                           //      pt_cstrns do not.
    (DS_dmod *dmod,        // in :  = for this dmod 
     int refine_flag=0)    // in :  = 0=use state bits as is
                           //                    1=treat links as fixed
                           { return(Is_Cd_contributor(dmod,refine_flag)) ;
                           }

  virtual                  // bas: DS_cstrn pure virtual function
  int                      // rtn: 1=cstrn adds to joint prob Cx=d eqns
   Is_joint_Cd_contributor // eff: link cstrns may add to joint Cx=d
    ()                     { if(Is_off() || !cst_dmod1) return(0) ;
                             return
                             ( (   Is_position_linked (cst_dmod1)
                                || Is_tangent_linked  (cst_dmod1)
                                || Is_curvature_linked(cst_dmod1))
                              ? 1 : 0 ) ;
                           }

  // DS_link_cstrn simple data access function
  // Access DS_link_cstrn Array size variables

  int Surf1_dof_count()    { return lct_surf1_dof_count ; }
  int Surf2_dof_count()    { return lct_surf2_dof_count ; }
  int Surf_dof_count       // rtn: surf total dof count
   (DS_dmod *dmod)         // in : for this dmod 
                           { DS_assert(dmod) ;
                             DS_assert(   dmod == cst_dmod1
                                       || dmod == cst_dmod2) ;
                             return
                              (  (dmod == cst_dmod1) ? lct_surf1_dof_count
                               : (dmod == cst_dmod2) ? lct_surf2_dof_count
                                                     : -1) ;
                           }

  int Domain_dim()         { return lct_domain_dim ; }      
  int Image_dim()          { return lct_image_dim ; }
  int Ntgrl_degree()       { return lct_ntgrl_degree ; }
  int Seg_count()          { return lct_seg_count ; }
  int Seg_bnds_count()     { return (  lct_seg_count == 0 
                                     ? 0 
                                     : lct_seg_count + 1) ; 
                           }

  int Src1_dir()           // rtn: 1=pos dir,2=neg dir,0=const
                           { return lct_src1_dir ; }
  int Src2_dir()           { return lct_src2_dir ; }

  int C1_dof_count()       { return lct_C1_dof_count ; }
  int C2_dof_count()       { return lct_C2_dof_count ; }
  int C_dof_count          // rtn: appropriate C_dof_count
   (DS_dmod *dmod)         // in : for this dmod
                           { DS_assert(dmod) ;
                             DS_assert(   dmod == cst_dmod1
                                       || dmod == cst_dmod2) ;
                             return
                              ( (dmod==cst_dmod1) ? lct_C1_dof_count
                               :(dmod==cst_dmod2) ? lct_C2_dof_count
                                                  : 0) ;
                           }

  int Cn1_dof_count()      { return lct_Cn1_dof_count ; }
  int Cn2_dof_count()      { return lct_Cn2_dof_count ; }
  int Cn_dof_count         // rtn: appropriate C_dof_count
   (DS_dmod *dmod)         // in : for this dmod
                           { DS_assert(dmod) ;
                             DS_assert(   dmod == cst_dmod1
                                       || dmod == cst_dmod2) ;
                             return
                              ( (dmod==cst_dmod1) ? lct_Cn1_dof_count
                               :(dmod==cst_dmod2) ? lct_Cn2_dof_count
                                                  : 0) ;
                           }

  int Cnn1_dof_count()     { return lct_Cnn1_dof_count ; }
  int Cnn2_dof_count()     { return lct_Cnn2_dof_count ; }
  int Cnn_dof_count        // rtn: appropriate C_dof_count
   (DS_dmod *dmod)         // in : for this dmod
                           { DS_assert(dmod) ;
                             DS_assert(   dmod == cst_dmod1
                                       || dmod == cst_dmod2) ;
                             return
                              ( (dmod==cst_dmod1) ? lct_Cnn1_dof_count
                               :(dmod==cst_dmod2) ? lct_Cnn2_dof_count
                               : 0) ;
                           }

  // DS_link_cstrn convenience behavior function

  // Access derived sizes
  int Image_pt_count       // rtn: number of sampled curve points for one curve
    (int ntgrl_degree,     // in : gauss integration accuracy for Cd
     int seg_count)        // in : count of one-elem crv-pieces
                           { return(  seg_count == 0 ? 0
                                    : (  2 + seg_count
                                       * DS_linear_gauss_pt_count
                                                   (ntgrl_degree))) ; 
                           }  
  int Image_pt_count()     { return(Image_pt_count(lct_ntgrl_degree,
                                                   lct_seg_count)) ;
                           }

  int Gauss_pt_count       // rtn: number of gauss pts for one elem
    (int ntgrl_degree)     { return(DS_linear_gauss_pt_count
                                          (ntgrl_degree)) ;
                           }

  int Gauss_pt_count()     { return(DS_linear_gauss_pt_count
                                          (lct_ntgrl_degree)) ;
                           }

  int Segment_pt_count     // rtn: gpt_count + bndry_pt_count
   (int ntgrl_degree,      // in : gauss integration accuracy
    int bndry_flag)        // orof: 0=gpt_only,1=low_bnd,2=up_bnd
                           { int gpt_count =
                                  DS_linear_gauss_pt_count
                                     (ntgrl_degree) ;
                             int bpt_count =
                                   ((bndry_flag & 1) ? 1 : 0)
                                  +((bndry_flag & 2) ? 1 : 0) ;
                             return(  gpt_count 
                                    ? gpt_count + bpt_count 
                                    : 0) ;
                           }
  int Segment_pt_count     // rtn: gauss_pt + asked for bndry_pt count
   (int bndry_flag)        // orof: 0=gpt_only,1=low_bnd,2=up_bnd
                           { return Segment_pt_count
                                       (lct_ntgrl_degree,
                                        bndry_flag) ;
                           }

  // Access DS_link_cstrn shape definitons and app data
  DS_pfunc *Src1_C_pfunc() { return lct_src1_C_pfunc ; }
  DS_pfunc *Src2_C_pfunc() { return lct_src2_C_pfunc ; }

  DS_pfunc *Src_C_pfunc    // rtn: appropriate src_C_pfunc
    (DS_dmod *dmod)        // in : for this dmod
                           { DS_assert(dmod) ;
                             DS_assert(   dmod == cst_dmod1
                                       || dmod == cst_dmod2) ;
                             return
                              (  (dmod == cst_dmod1) ? lct_src1_C_pfunc
                               : (dmod == cst_dmod2) ? lct_src2_C_pfunc
                                                     : NULL) ;
                           }
  void Set_src_C_pfunc     // eff: set appropriate src_C_pfunc
   (DS_dmod *dmod,         // in : for this dmod
    DS_pfunc *src_C_pfunc);// in : the pfunc definition to save

  int Is_src_pfunc         // rtn: 1=yes,0=no
   (DS_dmod *dmod,         // in : for this dmod
    DS_pfunc *src_pfunc) ; // in : pfunc to match
                         
  DECL_DM DS_pfunc * Src_W_pfunc    // rtn: desired src_W_pfunc
   (DS_dmod *dmod) ;       // in : for this dmod

  DECL_DM DS_pfunc * Src_Wn_pfunc   // rtn: desired src_Wn_pfunc
   (DS_dmod *dmod) ;       // in : for this dmod

  DECL_DM DS_pfunc * Src_Wnn_pfunc  // rtn: desired src_Wnn_pfunc
   (DS_dmod *dmod) ;       // in : for this dmod
 
  void Set_src_pfuncs       // set: all src_W_pfuncs at once
  (DS_dmod *dmod,           // in : for this dmod
   DS_pfunc *src_W_pfunc,   // in : cstrn pos shape, nested
   DS_pfunc *src_Wn_pfunc,  // in : cross-tang shape, nested
   DS_pfunc *src_Wnn_pfunc);// in : curvature shape, nested

  int Is_src_dmod          // rtn: 1=yes,0=no
   (DS_dmod *dmod,         // in : for this dmod
    DS_dmod *src_dmod) ;   // in : dmod to match
                         
  DECL_DM DS_dmod * Src_W_dmod      // rtn: desired src_W_dmod
   (DS_dmod *dmod) ;       // in : for this dmod

  DECL_DM DS_dmod * Src_Wn_dmod     // rtn: desired src_Wn_dmod
   (DS_dmod *dmod) ;       // in : for this dmod

  DECL_DM DS_dmod * Src_Wnn_dmod    // rtn: desired src_Wnn_dmod
   (DS_dmod *dmod) ;       // in : for this dmod
 
  void Set_src_dmods       // set: all src_W_dmods at once
  (DS_dmod *dmod,          // in : for this dmod
   DS_dmod *src_W_dmod,    // in : cstrn pos shape, nested
   DS_dmod *src_Wn_dmod,   // in : cross-tang shape, nested
   DS_dmod *src_Wnn_dmod); // in : curvature shape, nested
                         
  void (*Src_CW_func())    // rtn: user given crv function description
    (void *src_data,       // in : app defined data
     double s,             // in : edge s param value
     double *C,            // out: surface C=[u,v] pt value for s
     double *Cs,           // out: surface dC/ds vec in surface for s
     double *W,            // out: image space W=[x,y,z] pt value for s
     double *Wu,           // out: image space dW/du tangent for s
     double *Wv,           // out: image space dW/dv tangent for s
     double *Wuu,          // out: image space d2W/du2 for s
     double *Wuv,          // out: image space d2W/dudv for s
     double *Wvv,          // out: image space d2W/dv2 for s
     double &dist2)        // out: dist**2(xyz,proj_to_3d(uv))
                           { return lct_src_CW_func ; }

  void Set_src_CW_func     // Set: func & data used to calc C and W vals
   (void *src1_data,       // in : data to pass to func at compute time
    void *src2_data,       // in : data to pass to func at compute time
    void (*src_CW_func)    // in : func to use to compute C and W vals
     (void *,                // in : src_data = app defined data
      double ,               // in : s = edge s param value
      double *,              // out: C = surface C=[u,v] pt value for s
      double *,              // out: Cs = surface dC/ds vec in surface for s
      double *,              // out: W = image space W=[x,y,z] pt value for s
      double *,              // out: image space dW/du for s
      double *,              // out: image space dW/dv for s
      double *,              // out: Wuu = image space d2W/du2 for s
      double *,              // out: Wuv = image space d2W/dudv for s
      double *,              // out: Wvv = image space d2W/dv2 for s
      double &)) ;           // out: dist2 = dist**2(xyz,proj_to_3d(uv))

  // Access DS_link_cstrn internal arrays
  int *C_dof_map           // rtn: ptr to surf ==> used_dof map
    (DS_dmod *dmod)        // in : for this dmod
                           { DS_assert(dmod) ;
                             DS_assert(   dmod == cst_dmod1
                                       || dmod == cst_dmod2) ;
                             return
                              (  (dmod == cst_dmod1) ? lct_C1_dof_map
                               : (dmod == cst_dmod2) ? lct_C2_dof_map
                                                     : NULL) ;
                           }

  int *Cn_dof_map          // rtn: ptr to surf ==> used_dof map
    (DS_dmod *dmod)        // in : for this dmod 
                           { DS_assert(dmod) ;
                             DS_assert(   dmod == cst_dmod1
                                       || dmod == cst_dmod2) ;
                             return
                              (  (dmod == cst_dmod1) ? lct_Cn1_dof_map
                               : (dmod == cst_dmod2) ? lct_Cn2_dof_map
                                                     : NULL) ;
                           }

  int *Cnn_dof_map         // rtn: ptr to surf ==> used_dof map
    (DS_dmod *dmod)        // in : for this dmod 
                           { DS_assert(dmod) ;
                             DS_assert(   dmod == cst_dmod1
                                       || dmod == cst_dmod2) ;
                             return
                              (  (dmod == cst_dmod1) ? lct_Cnn1_dof_map
                               : (dmod == cst_dmod2) ? lct_Cnn2_dof_map
                                                     : NULL) ;
                           }

  int *C1_dof_map()        { return lct_C1_dof_map ; }
  int  C1_dof_map(int i)   // rtn: local index for global dof index
                           { DS_assert(i >= 0 && i < lct_surf1_dof_count);
                             return( lct_C1_dof_map[i]) ;
                           }

  int *Cn1_dof_map()       { return lct_Cn1_dof_map ; }
  int  Cn1_dof_map(int i)  // rtn: local index for global dof index
                           { DS_assert(i >= 0 && i < lct_surf1_dof_count);
                             return( lct_Cn1_dof_map[i]) ;
                           }

  int *C2_dof_map()        { return lct_C2_dof_map ; }
  int  C2_dof_map(int i)   // rtn: local index for global dof index
                           { DS_assert(i >= 0 && i < lct_surf2_dof_count);
                             return( lct_C2_dof_map[i]) ;
                           }

  int *Cn2_dof_map()       { return lct_Cn2_dof_map ; }
  int  Cn2_dof_map(int i)  // rtn: local index for global dof index
                           { DS_assert(i >= 0 && i < lct_surf2_dof_count);
                             return( lct_Cn2_dof_map[i]) ;
                           }

  int *Cnn2_dof_map()      { return lct_Cnn2_dof_map ; }
  int  Cnn2_dof_map(int i) // rtn: local index for global dof index
                           { DS_assert(i >= 0 && i < lct_surf2_dof_count);
                             return( lct_Cnn2_dof_map[i]) ;
                           }

  int *Dmesh_C_map()       { return lct_dmesh_C_map ; }
  int  Dmesh_C_map(int i)  // rtn: dmesh_C_index for link_C_index
                           { DS_assert(   i >= 0 
                                       && i  < Dmesh_C_map_count
                                                (lct_C1_dof_count,
                                                 lct_C2_dof_count,
                                                 lct_Cn1_dof_count,
                                                 lct_Cn2_dof_count,
                                                 lct_Cnn1_dof_count,
                                                 lct_Cnn2_dof_count)) ;
                             return(lct_dmesh_C_map[i]) ;
                           }

  double *Seg_bnds         // rtn: Seg_bnds array
    (DS_dmod *dmod)        // in : for this dmod
                           { DS_assert(dmod) ;
                             DS_assert(   dmod == cst_dmod1
                                       || dmod == cst_dmod2) ;
                             return
                              ( (dmod==cst_dmod1) ? lct_seg1_bnd_s
                               :(dmod==cst_dmod2) ? lct_seg2_bnd_s
                               : NULL) ;
                           }
  double *Seg1_bnds()      { return lct_seg1_bnd_s ; }
  double  Seg1_bnds(int i) { DS_assert(i >= 0 && i < lct_seg_count+1);
                             return(lct_seg1_bnd_s[i]) ;
                           }

  double *Seg2_bnds()      { return lct_seg2_bnd_s ; }
  double  Seg2_bnds(int i) { DS_assert(i >= 0 && i < lct_seg_count+2);
                             return(lct_seg2_bnd_s[i]) ;
                           }

  double *Seg1_bnds_W()    { return lct_seg1_bnd_W ; }
  double *Seg1_bnds_W
           (int i)         { DS_assert(i >= 0 && i < lct_seg_count+1);
                             return(lct_seg1_bnd_W+lct_image_dim*i);
                           }

  double *Seg2_bnds_W()    { return lct_seg2_bnd_W ; }
  double *Seg2_bnds_W
           (int i)         { DS_assert(i >= 0 && i < lct_seg_count+2);
                             return(lct_seg2_bnd_W+lct_image_dim*i);
                           }

  // Access DS_link_cstrn segment length internal array storage
  double *Src1_s_pts()     { return lct_src1_s_pts ; }
  double  Src1_s_pts       // rtn: ith src1 s_bnd value
   (int i,                 // in : s index
    int DS_name(bndry_flag)=0) // orof: 0=gpt_only,1=low_bnd,2=up_bnd
                           { DS_assert(   i >= 0 
                                       && i < Segment_pt_count(bndry_flag)) ;
                             return(lct_src1_s_pts[i]) ;
                           }

  double *Src2_s_pts()     { return lct_src2_s_pts ; }
  double  Src2_s_pts       // rtn: ith src2 s_bnd value
   (int i,                 // in : s index
    int DS_name(bndry_flag)=0) // orof: 0=gpt_only,1=low_bnd,2=up_bnd
                           { DS_assert(   i >= 0 
                                       && i < Segment_pt_count(bndry_flag)) ;
                             return(lct_src2_s_pts[i]) ;
                           }

  double *Src1_C_pts()     { return lct_src1_C_pts ; }
  double  Src1_C_pts_u     // rtn: u coordinate of ith src_C_pts
   (int i,                 // in : s index
    int DS_name(bndry_flag)=0) // orof: 0=gpt_only,1=low_bnd,2=up_bnd
                           { DS_assert(   i >= 0 
                                       && i < Segment_pt_count(bndry_flag)) ;
                             return(lct_src1_C_pts[i]) ;
                           }
  double  Src1_C_pts_v     // rtn: v coordinate of ith src_C_pts
   (int i,                 // in : v index desired
    int bndry_flag=0)      // orof: 0=gpt_only,1=low_bnd,2=up_bnd
                           { DS_assert(   i >= 0 
                                       && i < Segment_pt_count(bndry_flag)) ;
                             return(lct_src1_C_pts[Segment_pt_count(bndry_flag)+i]) ;
                           }

  double *Src2_C_pts()     { return lct_src2_C_pts ; }
  double  Src2_C_pts_u     // rtn: u coordinate of ith src_C_pts
   (int i,                 // in : u index desired
    int DS_name(bndry_flag)=0) // orof: 0=gpt_only,1=low_bnd,2=up_bnd
                           { DS_assert(   i >= 0 
                                       && i < Segment_pt_count(bndry_flag)) ;
                             return(lct_src2_C_pts[i]) ;
                           }
  double  Src2_C_pts_v     // rtn: v coordinate of ith src_C_pts
   (int i,                 // in : v index desired
    int bndry_flag=0)      // orof: 0=gpt_only,1=low_bnd,2=up_bnd
                           { DS_assert(   i >= 0 
                                       && i < Segment_pt_count(bndry_flag)) ;
                             return(lct_src2_C_pts[Segment_pt_count(bndry_flag)+i]) ;
                           }

  double *Src1_Cs_pts()    { return lct_src1_Cs_pts ; }
  double  Src1_Cs_pts_u    // rtn: u coordinate of ith src_Cs_pts
   (int i,                 // in : u index desired
    int DS_name(bndry_flag)=0) // orof: 0=gpt_only,1=low_bnd,2=up_bnd
                           { DS_assert(   i >= 0 
                                       && i < Segment_pt_count(bndry_flag)) ;
                             return(lct_src1_Cs_pts[i]) ;
                           }
  double  Src1_Cs_pts_v    // rtn: v coordinate of ith src_Cs_pts
   (int i,                 // in : v index desired
    int bndry_flag=0)      // orof: 0=gpt_only,1=low_bnd,2=up_bnd
                           { DS_assert(   i >= 0 
                                       && i < Segment_pt_count(bndry_flag)) ;
                             return(lct_src1_Cs_pts[i+Segment_pt_count(bndry_flag)]) ;
                           }

  double *Src2_Cs_pts()    { return lct_src2_Cs_pts ; }
  double  Src2_Cs_pts_u    // rtn: u coordinate of ith src_Cs_pts
   (int i,                 // in : u index desired
    int DS_name(bndry_flag)=0) // orof: 0=gpt_only,1=low_bnd,2=up_bnd
                           { DS_assert(   i >= 0 
                                       && i < Segment_pt_count(bndry_flag)) ;
                             return(lct_src2_Cs_pts[i]) ;
                           }
  double  Src2_Cs_pts_v    // rtn: v coordinate of ith src_Cs_pts
   (int i,                 // in : v index desired
    int bndry_flag=0)      // orof: 0=gpt_only,1=low_bnd,2=up_bnd
                           { DS_assert(   i >= 0 
                                       && i < Segment_pt_count(bndry_flag)) ;
                             return(lct_src2_Cs_pts[i+Segment_pt_count(bndry_flag)]) ;
                           }

  // Access DS_link_cstrn curve length internal array storage
  double *Src_W_pts        // rtn: src_W_pts array
    (DS_dmod *dmod)        // in : for this dmod
                           { DS_assert(dmod) ;
                             DS_assert(   dmod == cst_dmod1
                                       || dmod == cst_dmod2) ;
                             return
                              ( (dmod==cst_dmod1) ? lct_src1_W_pts
                               :(dmod==cst_dmod2) ? lct_src2_W_pts
                               : NULL) ;
                           }
  double *Src1_W_pts()     { return lct_src1_W_pts ; }
  double *Src1_W_pts(int i){ DS_assert(   i >= 0 
                                       && i < Image_pt_count()) ;
                             return(  lct_src1_W_pts 
                                    + i*lct_image_dim) ;
                           }

  double *Src2_W_pts()     { return lct_src2_W_pts ; }
  double *Src2_W_pts(int i){ DS_assert(   i >= 0 
                                       && i < Image_pt_count()) ;
                             return(  lct_src2_W_pts 
                                    + i*lct_image_dim) ;
                           }

  double *Src_Wn_pts       // rtn: src_Wn_pts array
    (DS_dmod *dmod)        // in : for this dmod
                           { DS_assert(dmod) ;
                             DS_assert(   dmod == cst_dmod1
                                       || dmod == cst_dmod2) ;
                             return
                              ( (dmod==cst_dmod1) ? lct_src1_Wn_pts
                               :(dmod==cst_dmod2) ? lct_src2_Wn_pts
                               : NULL) ;
                           }
  double *Src1_Wn_pts()     { return lct_src1_Wn_pts ; }
  double *Src1_Wn_pts(int i){ DS_assert(   i >= 0 
                                        && i < Image_pt_count()) ;
                             return(  lct_src1_Wn_pts 
                                    + i*lct_image_dim) ;
                            }

  double *Src2_Wn_pts()     { return lct_src2_Wn_pts ; }
  double *Src2_Wn_pts(int i){ DS_assert(   i >= 0 
                                        && i < Image_pt_count()) ;
                             return(  lct_src2_Wn_pts 
                                    + i*lct_image_dim) ;
                            }
  double *Src_Wnn_pts        // rtn: src_Wn_pts array
    (DS_dmod *dmod)          // in : for this dmod
                             { DS_assert(dmod) ;
                               DS_assert(   dmod == cst_dmod1
                                         || dmod == cst_dmod2) ;
                               return
                                ( (dmod==cst_dmod1) ? lct_src1_Wnn_pts
                                 :(dmod==cst_dmod2) ? lct_src2_Wnn_pts
                                 : NULL) ;
                             }
  double *Src1_Wnn_pts()     { return lct_src1_Wnn_pts ; }
  double *Src1_Wnn_pts(int i){ DS_assert(   i >= 0 
                                         && i < Image_pt_count()) ;
                              return(  lct_src1_Wnn_pts 
                                     + i*lct_image_dim) ;
                             }

  double *Src2_Wnn_pts()     { return lct_src2_Wnn_pts ; }
  double *Src2_Wnn_pts(int i){ DS_assert(   i >= 0 
                                         && i < Image_pt_count()) ;
                              return(  lct_src2_Wnn_pts 
                                     + i*lct_image_dim) ;
                             }

  double *Out_W_pts        // rtn: out_W_pts array
    (DS_dmod *dmod)        // in : for this dmod
                           { DS_assert(dmod) ;
                             DS_assert(   dmod == cst_dmod1
                                       || dmod == cst_dmod2) ;
                             return
                              ( (dmod==cst_dmod1) ? lct_out1_W_pts
                               :(dmod==cst_dmod2) ? lct_out2_W_pts
                               : NULL) ;
                           }
  double *Out1_W_pts()     { return lct_out1_W_pts ; }
  double *Out1_W_pts(int i){ DS_assert(   i >= 0 
                                       && i < Image_pt_count()) ;
                             return(  lct_out1_W_pts 
                                    + i*lct_image_dim) ;
                           }

  double *Out2_W_pts()     { return lct_out2_W_pts ; }
  double *Out2_W_pts(int i){ DS_assert(   i >= 0 
                                       && i < Image_pt_count()) ;
                             return(  lct_out2_W_pts 
                                    + i*lct_image_dim) ;
                           }

  double *Out_Wn_pts       // rtn: out_Wn_pts array
    (DS_dmod *dmod)        // in : for this dmod
                           { DS_assert(dmod) ;
                             DS_assert(   dmod == cst_dmod1
                                       || dmod == cst_dmod2) ;
                             return
                              ( (dmod==cst_dmod1) ? lct_out1_Wn_pts
                               :(dmod==cst_dmod2) ? lct_out2_Wn_pts
                               : NULL) ;
                           }
  double *Out1_Wn_pts()     { return lct_out1_Wn_pts ; }
  double *Out1_Wn_pts(int i){ DS_assert(   i >= 0 
                                        && i < Image_pt_count()) ;
                             return(  lct_out1_Wn_pts 
                                    + i*lct_image_dim) ;
                            }

  double *Out2_Wn_pts()     { return lct_out2_Wn_pts ; }
  double *Out2_Wn_pts(int i){ DS_assert(   i >= 0 
                                        && i < Image_pt_count()) ;
                             return(  lct_out2_Wn_pts 
                                    + i*lct_image_dim) ;
                            }

  double *Out_Wnn_pts        // rtn: out_Wnn_pts array
    (DS_dmod *dmod)          // in : for this dmod
                             { DS_assert(dmod) ;
                               DS_assert(   dmod == cst_dmod1
                                         || dmod == cst_dmod2) ;
                               return
                                ( (dmod==cst_dmod1) ? lct_out1_Wnn_pts
                                 :(dmod==cst_dmod2) ? lct_out2_Wnn_pts
                                 : NULL) ;
                             }
  double *Out1_Wnn_pts()     { return lct_out1_Wnn_pts ; }
  double *Out1_Wnn_pts(int i){ DS_assert(   i >= 0 
                                         && i < Image_pt_count()) ;
                              return(  lct_out1_Wnn_pts 
                                     + i*lct_image_dim) ;
                             }

  double *Out2_Wnn_pts()     { return lct_out2_Wnn_pts ; }
  double *Out2_Wnn_pts(int i){ DS_assert(   i >= 0 
                                         && i < Image_pt_count()) ;
                              return(  lct_out2_Wnn_pts 
                                     + i*lct_image_dim) ;
                             }

  // Access DS_link_cstrn pick-pt and C SPAmatrix data
  double *Pick_pt()        { return lct_pick_pt ; }

  double *C11()            { return lct_C11 ; }
  double *C12()            { return lct_C12 ; }
  double *C22()            { return lct_C22 ; }

  double *Cn11()           { return lct_Cn11 ; }
  double *Cn12()           { return lct_Cn12 ; }
  double *Cn22()           { return lct_Cn22 ; }

  double *Cnn11()          { return lct_Cnn11 ; }
  double *Cnn12()          { return lct_Cnn12 ; }
  double *Cnn22()          { return lct_Cnn22 ; }

  double *D1()             { return lct_d1 ; }
  double *D2()             { return lct_d2 ; }
  double *Dn1()            { return lct_dn1 ; }
  double *Dn2()            { return lct_dn2 ; }
  double *Dnn1()           { return lct_dnn1 ; }
  double *Dnn2()           { return lct_dnn2 ; }

  int C11_row_offset       // rtn: offset to diag member or row[i]
    (int i)                { DS_assert(   i >= 0 
                                       && i < lct_C1_dof_count) ;
                             return(lct_C1_dof_count*i-((i-1)*i)/2) ;
                           }
  int C12_row_offset       // rtn: offset to 1st member or row[i]
    (int i)                { DS_assert(   i >= 0 
                                       && i < lct_C1_dof_count) ;
                             return(lct_C2_dof_count*i) ;
                           }
  int C22_row_offset       // rtn: offset to diag member of row[i]
    (int i)                { DS_assert(   i >= 0
                                       && i < lct_C2_dof_count) ;
                             return(lct_C2_dof_count*i-((i-1)*i)/2) ;
                           }

  int Cn11_row_offset      // rtn: offset to diag member of row[i]
    (int i)                { DS_assert(   i >= 0 
                                       && i < lct_Cn1_dof_count) ;
                             return(lct_Cn1_dof_count*i-((i-1)*i)/2) ;
                           }
  int Cn12_row_offset      // rtn: offset to 1st member of row[i]
    (int i)                { DS_assert(   i >= 0 
                                       && i < lct_Cn1_dof_count) ;
                             return(lct_Cn2_dof_count*i) ;
                           }
  int Cn22_row_offset      // rtn: offset to diag member of row[i]
    (int i)                { DS_assert(   i >= 0
                                       && i < lct_Cn2_dof_count) ;
                             return(lct_Cn2_dof_count*i-((i-1)*i)/2) ;
                           }
  int Cnn11_row_offset     // rtn: offset to diag member of row[i]
    (int i)                { DS_assert(   i >= 0 
                                       && i < lct_Cnn1_dof_count) ;
                             return(lct_Cnn1_dof_count*i-((i-1)*i)/2) ;
                           }
  int Cnn12_row_offset     // rtn: offset to 1st member of row[i]
    (int i)                { DS_assert(   i >= 0 
                                       && i < lct_Cnn1_dof_count) ;
                             return(lct_Cnn2_dof_count*i) ;
                           }
  int Cnn22_row_offset     // rtn: offset to diag member of row[i]
    (int i)                { DS_assert(   i >= 0
                                       && i < lct_Cnn2_dof_count) ;
                             return(lct_Cnn2_dof_count*i-((i-1)*i)/2) ;
                           }

  double *Cii_row          // rtn: ptr to diag elem of row[i]
    (int i,                // in : for this i and
     DS_dmod *dmod)        // in : for this dmod
                           { DS_assert(   dmod == cst_dmod1
                                       || dmod == cst_dmod2) ;
                             return
                             ( (dmod==cst_dmod1) ? C11_row(i)
                              :(dmod==cst_dmod2) ? C22_row(i)
                              : NULL) ;
                           }

  double *C11_row          // rtn: ptr to diag elem of row[i]
    (int i)                { DS_assert(   i >= 0 
                                       && i < lct_C1_dof_count) ;
                             return( lct_C11 
                                    +lct_C1_dof_count*i
                                    -((i-1)*i)/2) ;
                           }

  double Cii               // rtn: lct_C11[i][j]
    (int i,                // in : for this row
     int j,                // in : for this column
     DS_dmod *dmod)        // in : for this dmod
                           { return (  (i < j)
                                     ? Cii_row(i,dmod)[j-i]
                                     : Cii_row(j,dmod)[i-j]) ;
                           }
                             
  double *C12_row          // rtn: ptr to start of row[i]
    (int i)                { DS_assert(   i >= 0 
                                       && i < lct_C1_dof_count) ;
                             return( lct_C12 
                                    +lct_C2_dof_count*i) ;
                           }
  double *C22_row          // rtn: ptr to diag elem of row[i]
    (int i)                { DS_assert(   i >= 0
                                       && i < lct_C2_dof_count) ;
                             return( lct_C22 
                                    +lct_C2_dof_count*i
                                    -((i-1)*i)/2) ;
                           }

  double *Cnii_row         // rtn: ptr to diag elem of row[i]
    (int i,                // in : for this i and
     DS_dmod *dmod)        // in : for this dmod
                           { DS_assert(   dmod == cst_dmod1
                                       || dmod == cst_dmod2) ;
                             return
                             ( (dmod==cst_dmod1) ? Cn11_row(i)
                              :(dmod==cst_dmod2) ? Cn22_row(i)
                              : NULL) ;
                           }

  double Cnii              // rtn: lct_Cnii[i][j]
    (int i,                // in : for this row
     int j,                // in : for this column
     DS_dmod *dmod)        // in : for this dmod
                           { return (  (i < j)
                                     ? Cnii_row(i,dmod)[j-i]
                                     : Cnii_row(j,dmod)[i-j]) ;
                           }

  double *Cn11_row         // rtn: ptr to diag elem of row[i]
    (int i)                { DS_assert(   i >= 0 
                                       && i < lct_Cn1_dof_count) ;
                             return( lct_Cn11 
                                    +lct_Cn1_dof_count*i
                                    -((i-1)*i)/2) ;
                           }
  double *Cn12_row         // rtn: ptr to start of row[i]
    (int i)                { DS_assert(   i >= 0 
                                       && i < lct_Cn1_dof_count) ;
                             return( lct_Cn12 
                                    +lct_Cn2_dof_count*i) ;
                           }
  double *Cn22_row         // rtn: ptr to diag elem of row[i]
    (int i)                { DS_assert(   i >= 0
                                       && i < lct_Cn2_dof_count) ;
                             return( lct_Cn22 
                                    +lct_Cn2_dof_count*i
                                    -((i-1)*i)/2) ;
                           }


  double *Cnnii_row        // rtn: ptr to diag elem of row[i]
    (int i,                // in : for this i and
     DS_dmod *dmod)        // in : for this dmod
                           { DS_assert(   dmod == cst_dmod1
                                       || dmod == cst_dmod2) ;
                             return
                             ( (dmod==cst_dmod1) ? Cnn11_row(i)
                              :(dmod==cst_dmod2) ? Cnn22_row(i)
                              : NULL) ;
                           }

  double Cnnii             // rtn: lct_Cnnii[i][j]
    (int i,                // in : for this row
     int j,                // in : for this column
     DS_dmod *dmod)        // in : for this dmod
                           { return (  (i < j)
                                     ? Cnnii_row(i,dmod)[j-i]
                                     : Cnnii_row(j,dmod)[i-j]) ;
                           }

  double *Cnn11_row        // rtn: ptr to diag elem of row[i]
    (int i)                { DS_assert(   i >= 0 
                                       && i < lct_Cnn1_dof_count) ;
                             return( lct_Cnn11 
                                    +lct_Cnn1_dof_count*i
                                    -((i-1)*i)/2) ;
                           }
  double *Cnn12_row        // rtn: ptr to start of row[i]
    (int i)                { DS_assert(   i >= 0 
                                       && i < lct_Cnn1_dof_count) ;
                             return( lct_Cnn12 
                                    +lct_Cnn2_dof_count*i) ;
                           }
  double *Cnn22_row        // rtn: ptr to diag elem of row[i]
    (int i)                { DS_assert(   i >= 0
                                       && i < lct_Cnn2_dof_count) ;
                             return( lct_Cnn22 
                                    +lct_Cnn2_dof_count*i
                                    -((i-1)*i)/2) ;
                           }

  double *Di               // rtn: lct_di
    (int i,                // in : for this row
     DS_dmod *dmod)        // in : for this dmod
                           { DS_assert(   dmod == cst_dmod1
                                       || dmod == cst_dmod2) ;
                             return
                             ( (dmod==cst_dmod1) ? D1_row(i)
                              :(dmod==cst_dmod2) ? D2_row(i)
                              : NULL) ;
                           }

  double *Dni              // rtn: lct_dni
    (int i,                // in : for this row
     DS_dmod *dmod)        // in : for this dmod
                           { DS_assert(   dmod == cst_dmod1
                                       || dmod == cst_dmod2) ;
                             return
                             ( (dmod==cst_dmod1) ? Dn1_row(i)
                              :(dmod==cst_dmod2) ? Dn2_row(i)
                              : NULL) ;
                           }

  double *Dnni             // rtn: lct_dnni
    (int i,                // in : for this row
     DS_dmod *dmod)        // in : for this dmod
                           { DS_assert(   dmod == cst_dmod1
                                       || dmod == cst_dmod2) ;
                             return
                             ( (dmod==cst_dmod1) ? Dnn1_row(i)
                              :(dmod==cst_dmod2) ? Dnn2_row(i)
                              : NULL) ;
                           }

  double *D1_row(int i)    // rtn: ptr to start of row[i]
                           { DS_assert(   i >= 0
                                       && i < lct_C1_dof_count) ;
                             return(lct_d1 + lct_image_dim*i) ;
                           }

  double *Dn1_row(int i)   // rtn: ptr to start of row[i]
                           { DS_assert(   i >= 0
                                       && i < lct_Cn1_dof_count) ;
                             return(lct_dn1 + lct_image_dim*i) ;
                           }

  double *Dnn1_row(int i)  // rtn: ptr to start of row[i]
                           { DS_assert(   i >= 0
                                       && i < lct_Cnn1_dof_count) ;
                             return(lct_dnn1 + lct_image_dim*i) ;
                           }

  double *D2_row(int i)    // rtn: ptr to start of row[i]
                           { DS_assert(   i >= 0
                                       && i < lct_C2_dof_count) ;
                             return(lct_d2 + lct_image_dim*i) ;
                           }

  double *Dn2_row(int i)   // rtn: ptr to start of row[i]
                           { DS_assert(   i >= 0
                                       && i < lct_Cn2_dof_count) ;
                             return(lct_dn2 + lct_image_dim*i) ;
                           }

  double *Dnn2_row(int i)  // rtn: ptr to start of row[i]
                           { DS_assert(   i >= 0
                                       && i < lct_Cnn2_dof_count) ;
                             return(lct_dnn2 + lct_image_dim*i) ;
                           }

  // DS_link_cstrn internal array size functions
                                
  int Dof_map_size         // rtn: lct_dof_map size in doubles
    (int surf_dof_count)   // in : dof count in constrained pfunc 
                           { if(surf_dof_count)
                              return(
                               (int)ceil(  surf_dof_count
                                         * (double)sizeof(int)
                                         / (double)sizeof(double))) ;
                             else return(0) ;
                           }

  int Dmesh_C_map_size     // rtn: lct_dof_map size in doubles
    (int C1_dof_count,     // in : surf1 pos  cstrn dof count
     int C2_dof_count,     // in : surf2 pos  cstrn dof count
     int Cn1_dof_count,    // in : surf1 tan  cstrn dof count
     int Cn2_dof_count,    // in : surf2 tan  cstrn dof count
     int Cnn1_dof_count,   // in : surf1 curv cstrn dof_count
     int Cnn2_dof_count)   // in : surf2 curv cstrn dof_count
                           {return(
                            (int)ceil( Dmesh_C_map_count
                                        (C1_dof_count, 
                                         C2_dof_count, 
                                         Cn1_dof_count,
                                         Cn2_dof_count,
                                         Cnn1_dof_count,
                                         Cnn2_dof_count)
                                      * (double)sizeof(int)
                                      / (double)sizeof(double)));
                           } // end Dmesh_C_map_size()

  int Dmesh_C_map_count    // rtn: lct_dof_map size in ints
    (int C1_dof_count,     // in : surf1 pos  cstrn dof count
     int C2_dof_count,     // in : surf2 pos  cstrn dof count
     int Cn1_dof_count,    // in : surf1 tan  cstrn dof count
     int Cn2_dof_count,    // in : surf2 tan  cstrn dof count
     int Cnn1_dof_count,   // in : surf1 curv cstrn dof_count
     int Cnn2_dof_count)   // in : surf2 curv cstrn dof_count
                           {return(  DS_3MAX(C1_dof_count,
                                             Cn1_dof_count,
                                             Cnn1_dof_count)
                                   + DS_3MAX(C2_dof_count,
                                             Cn2_dof_count,
                                             Cnn2_dof_count) ) ;
                           } // end Dmesh_C_map_count()
  
  int Seg_bnd_s_size       // rtn: seg_cuts array size
    (int seg_count)        // in : number of curve segments
                           { return (  seg_count <= 0 
                                     ? 0 
                                     : seg_count + 1) ; 
                           }
  int Seg_bnd_W_size       // rtn: seg_bnds image_pt array size
    (int image_dim,        // in : image space size
     int seg_count)        // in : number of curve segments
                           { return (  image_dim 
                                     * Seg_bnd_s_size(seg_count)) ;
                           }
  int Seg_bnd_W_size()     { return(Seg_bnd_W_size
                                        (lct_image_dim,
                                         lct_seg_count) );
                           }

  int Image_pts_size       // rtn: true_pts and proj_pts array sizes
    (int ntgrl_degree,     // in : gauss integration accuracy for Cd
     int image_dim,        // in : image space size
     int seg_count)        // in : count of one-elem crv-pieces 
                           { return (  Image_pt_count(ntgrl_degree,
                                                     seg_count)
                                     * image_dim) ;
                           }
  int Image_pts_size()     { return(Image_pts_size(lct_ntgrl_degree,
                                                   lct_image_dim,
                                                   lct_seg_count)) ;
                           }                            

  int Domain_pts_size      // rtn: domain_pts array_size
    (int ntgrl_degree,     // in : gauss integration accuracy for Cd
     int domain_dim,       // in : domain space size
     int seg_count)        // in : count of one-elem crv-pieces 
                           { return (  Image_pt_count(ntgrl_degree,
                                                      seg_count)
                                     * domain_dim) ;
                           } 
  int Domain_pts_size()    {return(Domain_pts_size(lct_ntgrl_degree,
                                                   lct_domain_dim,
                                                   lct_seg_count)) ;
                           }

  int C11_size             // rtn: C11 array size
    (int dof1_count)       { return(dof1_count*(dof1_count+1)/2) ; }
  
  int C12_size             // rtn: C12 array size
    (int dof1_count,       // in : surf1 used dof count
     int dof2_count)       // in : surf2 used dof count
                           { return(dof1_count*dof2_count) ; }

  int C22_size             // rtn: C22 array size
    (int dof2_count)       { return(dof2_count*(dof2_count+1)/2) ; }

  int D_size               // rtn: d size
    (int image_dim,        // in : assoc pfunc's image dimension
     int dof_count)        // in : surf used dof count
                           { return(image_dim * dof_count) ;
                           }
                             
  int C_size               // rtn: C11+C12+C22 array size
    (int dof1_count,       // in : surf1 used dof count
     int dof2_count)       // in : surf2 used dof count
                           { // either pos or tan C size
                             return( (dof1_count+dof2_count)
                                    *(dof1_count+dof2_count+1)
                                    /2 ); 
                           }

  int Block_without_Cd_size// rtn: Sum of all arrays except Cd sizes
    (int surf1_dof_count,  // in : surf1 pfunc's dof count
     int surf2_dof_count,  // in : surf2 pfunc's dof count
     int image_dim,        // in : assoc pfunc's image dimension
     int ntgrl_degree,     // in : gauss integration accuracy for Cd
     int seg_count)        // in : count of one-elem crv-pieces
                           { return(  // lct_C1_dof_map,   lct_C1_dof_map
                                      // lct_Cn1_dof_map,  lct_Cn2_dof_map
                                      // lct_Cnn1_dof_map, lct_Cnn2_dof_map
                                      3*Dof_map_size(surf1_dof_count)
                                    + 3*Dof_map_size(surf2_dof_count)

                                    + // lct_seg1_bnd_s
                                      // lct_seg2_bnd_s
                                      2*Seg_bnd_s_size(seg_count)

                                    + // lct_seg1_bnd_W
                                      // lct_seg2_bnd_W
                                      2*Seg_bnd_W_size(image_dim,seg_count)

                                    + // lct_src1_s_pts
                                      // lct_src2_s_pts
                                      (  seg_count
                                       ? 2*Segment_pt_count(ntgrl_degree,3) 
                                       : 0)

                                    + // lct_src1_C_pts, lct_src1_Cs_pts
                                      // lct_src2_C_pts, lct_src2_Cs_pts
                                      (  seg_count
                                       ? 4*2*Segment_pt_count(ntgrl_degree,3)
                                       : 0)

                                    + // lct_src1_W_pts,   lct_src2_W_pts
                                      // lct_src1_Wn_pts,  lct_src2_Wn_pts
                                      // lct_src1_Wnn_pts, lct_src2_Wnn_pts
                                      // lct_out1_W_pts,   lct_out2_W_pts
                                      // lct_out1_Wn_pts,  lct_out2_Wn_pts
                                      // lct_out1_Wnn_pts, lct_out2_Wnn_pts
                                      12*Image_pts_size(ntgrl_degree,
                                                        image_dim,
                                                        seg_count)
                                    + // lct_pick_pt
                                      (seg_count ? image_dim : 0)
                                   ) ;
                           } // end Block_without_Cd_size()

  int Block_size           // rtn: Sum of all array sizes
    (int surf1_dof_count,  // in : surf1 pfunc's dof count
     int surf2_dof_count,  // in : surf2 pfunc's dof count
     int C1_dof_count,     // in : surf1 pos used dof count
     int C2_dof_count,     // in : surf2 pos used dof count
     int Cn1_dof_count,    // in : surf1 tan used dof count
     int Cn2_dof_count,    // in : surf2 tan used dof count
     int Cnn1_dof_count,   // in : surf1 curv used dof count
     int Cnn2_dof_count,   // in : surf2 curv used dof count
     int ,                 // in : domain_dim = assoc pfunc's domain dimension
     int image_dim,        // in : assoc pfunc's image dimension
     int ntgrl_degree,     // in : gauss integration accuracy for Cd
     int seg_count)        // in : count of one-elem crv-pieces
                           { return(  Block_without_Cd_size(surf1_dof_count,
                                                            surf2_dof_count,
                                                            image_dim,     
                                                            ntgrl_degree,  
                                                            seg_count)     
                                    + // dmesh_C_map
                                      Dmesh_C_map_size(C1_dof_count,
                                                       C2_dof_count,
                                                       Cn1_dof_count,
                                                       Cn2_dof_count,
                                                       Cnn1_dof_count,
                                                       Cnn2_dof_count) 
                                    + // alloc size for C
                                      C_size(C1_dof_count,
                                             C2_dof_count)
                                    + // alloc size for Cn
                                      C_size(Cn1_dof_count,
                                             Cn2_dof_count) 
                                    + // alloc size for Cnn
                                      C_size(Cnn1_dof_count,
                                             Cnn2_dof_count) 
                                    + // alloc size for d1, d2, dn1, dn2
                                      D_size(image_dim,
                                              C1_dof_count
                                             +C2_dof_count
                                             +Cn1_dof_count
                                             +Cn2_dof_count
                                             +Cnn1_dof_count 
                                             +Cnn2_dof_count)  
                                   ) ; 
                           }  // end DS_link_cstrn::Block_size

  // DS_link_cstrn internal side-effect functions

  void                     // eff: use DS_CST_FIXED states
   Mark_calc_src_behaviors //      to mark recalc src bits
    (int) ;                // in : mark_tan_link flat; 1==> mark tan/curv links for update

  void
   Calc_dof_map            // eff: calc used_dof_map and
    () ;

  int                      // rtn: 0=ok,-1=NULL_srcs,-2=bad_dpts
   Calc_seg_src_pts        // eff: calc src s,uv,duv,udv arrays
                           //      for gpts and optional bndry pts
    (int segment_index,    // in : for this segment being investigated
     int bndry_flag=0) ;   // in : orof 1=add in lower seg boundary
                           //           2=add in upper seg boundary

  void
   Calc_flipped_coords     // eff: set lct_flipped_coords
    () ;                   //      0=surfs share uv coord handedness
                           //      1=surfs differ uv coord handedness

  int                      // rtn: 0=ok,-1=bad speeds
  Check_src_pts_speed      // eff: checks for constant speeds
  (double speed_tol,       // in : allowed limit in speed variations
   double &min_ratio1,     // out: min of size(src1_Cs_pts)/
                           //                size(src2_Cs_pts)
   double &max_ratio1,     // out: max of size(src1_Cs_pts)/
                           //                size(src2_Cs_pts)
   double &min_ratio2,     // out: min of delta(src1_C_pts)/
                           //                delta(src2_C_pts)
   double &max_ratio2);    // out: max of delta(src1_C_pts)/
                           //                delta(src2_C_pts)
  void
   Calc_basis_vals         // eff: get basis vals for src_C_pts
    () ;

  void                     // use: Called by Calc_seg_src_pts()
   Seg_s_pts_from_seg_bnds // eff: calc lct_src1_s_pts, lct_src2_s_pts
    (int segment_index,    // in : for this segment
     int bndry_flag=0) ;   // in : orof 1=add in lower seg boundary
                           //           2=add in upper seg boundary

  int                      // eff: calc lct_src1_C_pts, lct_src1_Cs_pts
   Seg_C_pts_from_CW_func  //           lct_src2_C_pts, lct_src2_Cs_pts
   (int bndry_flag=3)  ;   // in : orof 1=add in lower seg boundary
                           //           2=add in upper seg boundary

  int                      // use: Called by Calc_seg_src_pts()
   Seg_CW_pts_from_CW_func // eff: calc C & W pts from func
    (DS_dmod *dmod,        // in : for this dmod
     int segment_index,    // in : for this segment
     int bndry_flag =0,    // in : orof 1=add in lower seg boundary
                           //           2=add in upper seg boundary
     int calc_W_flag=7,    //orof: 1=calc W pts, 
                           //      2=calc Wn pts,
                           //      4=calc Wnn pts
                           //      0=don't  
     int rot_flag=1) ;     // 1 ==> update rotations; 0 ==> don't
                                     
  int                      // rtn: 0=ok,-1=bad_dmod,-2=bad_C_vals
   Seg_W_pts_from_C_pts    // eff: W_pts = W(C(s))
    (DS_dmod *dmod,        // in : for this dmod
     int seg,              // in : for this segment
     int bndry_flag,       // in : orof 1=add in lower seg boundary
                           //           2=add in upper seg boundary
     int calc_W_flag,      //orof: 1=calc W pts, 
                           //      2=calc Wn pts,
                           //      4=calc Wnn pts
                           //      0=don't 
     int rot_flag) ;       // 1 ==> update rotations; 0 ==> don't

  int                      // rtn: 0=ok,-1=bad_dmod,-2=bad_C_vals
  Seg_W_pts_from_src_pfuncs// eff: W_pts = src_W_pfunc(s)
    (DS_dmod *dmod,        // in : for this dmod
     int seg,              // in : for this segment
     int bndry_flag,       // in : orof 1=add in lower seg boundary
                           //           2=add in upper seg boundary
     int calc_W_flag) ;    //orof: 1=calc W pts, 
                           //      2=calc Wn pts,
                           //      4=calc Wnn pts
                           //      0=don't 

  int                      // rtn: 0=ok,-1=no pfunc,-2=bad dpts,-3=bad val_flag
  Calc_constrained_W_pts   // eff: calc W vals from constrained shape
    (DS_pfunc *pfunc,      // in : the constrained shape
     DS_EVAL val_flag,     // in :  one of
                           //  pfn_X    = W only
                           //  pfn_Xu   = W, Wt, and Wn
                           //  pfn_Xuu  = W, Wt, Wn, and Wnn
     int pt_count,         // in : number of points to evaluate
     double *src_C_pts,    // in : domain pts
     double *src_Cs_pts) ; // in : domain dirs 
                           // eff: load DS_pfunc::Dscr with
                           // ordered:[W   [pt0,..,ptN]]
                           //         [Wt  [pt0,..,ptN]]
                           //         [Wn  [pt0,..,ptN]]
                           //         [Wnn [pt0,..,ptN]]

                           // use: Called by Calc_seg_src_pts()
  int                      // rtn: 0=success,-1=no_src_pfunc,-2=bad_dpt
   Seg_C_pts_from_C_pfunc  // eff: calc lct_src_C_pts from uv_desc
    (int bndry_flag=0) ;   // in : orof 1=add in lower seg boundary
                           //           2=add in upper seg boundary

   
                           // use: called by Default Constructor
  int                      // rtn: 0=success, -1=bad_dpts, -2=bad pfuncs
   Calc_Cd                 // eff: calc and store Cd arrays 
    (int refine_flag=0) ;  // in : 0=use state bits as is
                           //      1=treat links as fixed

                           // use: called by Default Constructor
  int                      // rtn: 0=success, -1=bad_dpts, -2=bad pfuncs
   Calc_d                  // eff: calc and store d arrays 
    (int refine_flag=0) ;  // in : 0=use state bits as is
                           //      1=treat links as fixed

  int                      // rtn: -1 bad_dpts, else 0
   Calc_seg_bnd_W          // eff: load lct_seg1_bnd_W
    () ;                   //      and  lct_seg2_bnd_W

  int                      // rtn: 0=success, -1=bad_dpts, -2=bad speeds
   Calc_src_W_pts(         // eff: calc ipts when X or P changes
     int rot_flag) ;       // 1 ==> update rotations; 0 ==> don't

  int                      // rtn: 0=success, -1=bad_dpts
   Calc_out_W_pts          // eff: calc ipts when X or P changes
    () ;                   //      always use W=dmod->Pfunc(C(s))

  double                   // rtn: max_dist(lct_seg1_bnd_W,lct_seg2_bnd_W)
   Compare_bnd_W_pts       // eff: check input geometry tolerance
    () ; 

  double                   // rtn: max_dist(Src1_pts,Src2_pts)  
   Compare_out_W_pts       // eff: check cstrn tolerance 
    (double &max_dist,     // out: largest displacement error
     double &max_angle) ;  // out: largest angle error 

  double                   // rtn: max_dist(Src_pts,Out_pts)  
   Compare_src_to_out_W_pts// eff: check cstrn tolerance 
    (DS_dmod *dmod,        // in : for this dmod
     double &max_dist,     // out: largest displacement error
     double &max_angle,    // out: largest angle error 
     int refine_flag=0) ;  // in : 0=use state bits as is
                           //      1=treat links as fixed

  int                      // ret: 0 for (d==CX) else (d!=CX) 
   Compare_d_with_CX       // eff: compare d to CX            
   (double &max_err) ;     // out: max ri = abs(di - CijXj)

  int                      // ret: 0 for (d==CX) else (d!=CX) 
   Compare_d_with_CX       // eff: compare d to CX            
   (double *err,       // out: max ri = abs(di - CijXj)
    int behavior);

  int                      // rtn: recalculated seg_count value 
   Recalc_segments         // eff: revise lct_src#_seg_bnds
    () ;

  int                      // rtn: 1=yes, 0=no, -1=no segs to test
   Is_s1_to_s2_mapping_simple // eff: test mapping
    (double &scale,        // out: scale of  s2 = scale*s1+offset
     double &offset) ;     // out: offset of s2 = scale*s1+offset

  virtual                  // bas: DS_cstrn virtual function
   int                     // rtn: 1=changes allowed,0=not allowed,-1=bad_dpts
   Set_on_off              // eff: When allowed, Turn cstrn on or off
    (int on_off,           // in : 0=off,1=on
     int pfunc_state=0) ;  // orof: DS_PFN_CHANGED_X      = X val change
                           //       DS_PFN_CHANGED_P_ON   = Set def shape
                           //       DS_PFN_CHANGED_P_OFF  = Clear def shape
                           //       DS_PFN_CHANGED_DOMAIN = Domain change
                           //       DS_PFN_CSTRN_ON = link_cstrn turned on
                           //       DS_PFN_TRACKING = link_cstrn tracking parent
                           
  // DS_link_cstrn implementations of DS_cstrn virtual functions
  //  non-side-effect functions

  double                   // bas: DS_cstrn virtual function
   Dist2_to_iline          // ret: dist**2 between cstrn and iline
                           //      iline = pi0 + u(pi1-pi0)
    (double *pi0,          // in : image space pt, Sized:[image_dim]
     double *pi1,          // in : image space pt, Sized:[image_dim]
     DS_CST_PT_INDEX       // out: image_pt index in cstrn for closest pt
            &pt_index,     
     double &uu) ;         // out: iline param for pt closest to cstrn
/*
  virtual void             // bas: DS_cstrn virtual function
   Set_image_pt            // eff: copies ipt into cstrn's image pt
   (double *,              // in : ipt = pt to copy, Sized:Image_dim()
    double ,               // in : tan_display_gain, tpt = bpt+tan_display_gain*tvec
    int )                  // in : pt_index = index of image_pt to update
                           { }

  virtual                  // bas: DS_cstrn virtual function
   int                     // rtn: 0=success,-1=bad_dpt
   Set_domain_pt           // eff: copies dpt into pst_domain_pt
   (double *,              // in : dpt = pt to copy, Sized:Domain_pt_size()
    double ,               // in : tan_display_gain tpt = bpt+tan_display_gain*tvec
    int )                  // orof: 1=calc new base_pt
                           //       2=calc new tang_vec
                           //       4=calc new Wuu,Wuv,Wvv vecs
                           //       0=save old base_pt and tang_vec
                           { return(0) ; }
*/
  // implementations of DS_cstrn virtual functions
  // DS_link_cstrn side-effect functions

  virtual void             // bas: DS_cstrn pure virtual function
   Shift_domain            // eff: shift the basis domain values
    (double *du,           // in : domain shift SPAvector, Sized:Domain_dim()
     DS_dmod *dmod) ;      // in : for this dmod 

  virtual                  // bas: DS_cstrn pure virtual function
   int                     // rtn: 0=done,-1=scale too small
   Scale_domain            // eff: scale the basis domain values
    (double gain,          // in : domain scale factor
     DS_dmod *dmod) ;      // in : for this dmod 

  virtual                  // bas: DS_cstrn virtual function
   int                     // rtn: 0=success,-1=bad_dpts
   Update_pts              // eff: update all cstrn cached data
                           //      after changes in pfunc
  (double tan_display_gain,// in : tang_pt = base_pt + tan_display_gain*tang_vec
   double C2_display_gain, // in : curv_pt = base_pt + C2_display_gain *tang_vec
   int pfunc_state) ;      // orof: DS_PFN_CHANGED_X      = X val change
                           //       DS_PFN_CHANGED_P_ON   = Set def shape
                           //       DS_PFN_CHANGED_P_OFF  = Clear def shape
                           //       DS_PFN_CHANGED_DOMAIN = Domain change
                           //       DS_PFN_CSTRN_ON= link_cstrn toggled on
                           //       DS_PFN_TRACKING= link_cstrn tracking parent
  
  virtual void             // bas: DS_cstrn virtual function
   Build_Cd_row            // eff: build one pt-cstrn's Cx=d eqn row
    (DS_dmod *dmod,        // in : shape to constrain
     DS_eqns *eqns,        // in : holder of Ax=b, Cx=d eqns
     int &next_Cd_row,     // i/o: next available Cx=d row
     int refine_flag=0) ;  // in : 0=use state bits as is
                           //      1=treat links as fixed
  
  virtual void             // bas: DS_cstrn virtual function
   Build_row               // eff: build one pt-cstrn's Cx=d eqn row
    (DS_dmod *dmod,        // in : shape to constrain
     DS_eqns *eqns,        // in : holder of Ax=b, Cx=d eqns
     const DS_enforcement_mechanism* mech, // in : curve constraint emech to use
     int &next_Cd_row,     // i/o: next available Cx=d row
     int refine_flag,      // in : 0=use state bits as is
                           //      1=treat links as fixed
     int build_LHS,
     int build_RHS);

  virtual void             // bas: DS_cstrn virtual function
   Build_d_row             // eff: build one pt-cstrn's d of Cx=d eqn
    (DS_dmod *dmod,        // in : shape to constrain
     DS_eqns *eqns,        // in : holder of Ax=b, Cx=d eqns
     int &next_Cd_row) ;   // i/o: next available Cx=d row

  virtual void             // bas: DS_cstrn virtual function
   Build_A_row             // eff: build one pt-cstrn's d of Cx=d eqn
    (DS_dmod *dmod,        // in : shape to constrain
     DS_eqns *eqns) ;      // in : holder of Ax=b, Cx=d eqns

  virtual void             // bas: DS_cstrn virtual function
   Build_b_row             // eff: build one pt-cstrn's d of Cx=d eqn
    (DS_dmod *dmod,        // in : shape to constrain
     DS_eqns *eqns) ;      // in : holder of Ax=b, Cx=d eqns

  virtual void             // bas: DS_cstrn virtual function                        
   Build_L_Lc_row          // eff: build one pt-cstrn's Lxi=Lc eqn rows             
    (DS_dmod *,            // in : dmod = shape to constrain
     DS_eqns *,            // in : symeq = holder of Ax=b, Cx=d eqns
     int )                 // in : next_L_Lc_row = 1st available row of Li xi=Lc
                           { }

  virtual void             // bas: DS_cstrn virtual function
   Build_Lc_row            // eff: build one pt-cstrn's d of Cx=d eqn
    (DS_dmod *,            // in : dmod = shape to constrain
     DS_eqns *,            // in : symeq = holder of Ax=b, Cx=d eqns
     int )                 // in : next_L_Lc_row = 1st available row of Li xi=Lc
                           { }
#ifndef DS_USE_SYMEQ
#else
  virtual                  // bas: DS_cstrn virtual function
  int                      // rtn: free_dof_count, -2=no effect or -1=error                     
   Mark_this_free_dofs     // eff: mark all free dofs and elems in input
   (DS_dmod *,             // in : dmod = shape to constrain
    int ,                  // in : dof_count = sizeof dof_map
    int *,                 // i/o: dof_map = dof_map for constrained pfunc
    int &free_dof_count,   // out: dofs changed from fixed to free
    int ,                  // in : elem_count = sizeof elem_state
    int *,                 // i/o: elem_state = 0=free to move, 1=fixed
    int &free_elem_count)  // out: elems changed from fixed to free
                           { free_dof_count = 0 ;
                             free_elem_count = 0 ;
                             return -2 ; 
                           }
#endif //DS_USE_SYMEQ

  virtual void             // bas: DS_cstrn virtual function
  Bad_track_response       // eff: reset tracked cstrns. 
  (double ,                // in : tan_display_gain = tang_pt = base_pt + tan_display_gain*tang_vec
   double                  // in : C2_display_gain = curv_pt = base_pt + C2_display_gain *tang_vec
   )                       //      Called when tracking yields cstraint 
                           //      confilcts
                           { }

  virtual                  // bas: DS_cstrn virtual function
  int                      // rtn: 0=success,-1=file failure
   Dump                    // eff: write all internal vals to file
    (char *filename,       // in : file to write
     FILE *fp) ;           // in : existing file or NULL

  // DS_link_cstrn dmesh building functions
  void                     // eff: build link_dof to 
   Calc_link_2dmesh_C_map  //      dmesh_dof map
    (int  map_type,        // oneof: DS_CST_POSITION
                           //        DS_CST_TANGENT, 
                           //        DS_CST_CURVATURE
     int  nc1_count,       // in : number of xc dofs for dmod1
     int  xc1_offset,      // in : dmesh_C_index = xc1_index+xc1_offset
     int *xc1_map,         // in : xc1_index=>pfunc1_index mapping
                           //      sized:[nc1_count]
     int  nc2_count,       // in : number of xc dofs for dmod2
     int  xc2_offset,      // in : dmesh_C_index = xc1_index+xc1_offset
     int *xc2_map) ;       // in : xc2_index=>pfunc2_index mapping
                           //      sized:[nc2_count]

#ifndef DS_USE_SYMEQ
  void                     // bas: 
   Build_dmesh_A_row      // eff: add in msh-cstrn's Cx=d eqn set
  ( int offset1,           // in : sum of dmod dofs which come before dmod1
    int offset2,           // in : sum of dmod dofs which come before dmod2
    DS_eqns* dmesh_eqns ); // in/out: equations to build into
  void                     // bas: 
   Build_dmesh_b_row      // eff: add in msh-cstrn's Cx=d eqn set
  ( int offset1,           // in : sum of dmod dofs which come before dmod1
    int offset2,           // in : sum of dmod dofs which come before dmod2
    DS_eqns* dmesh_eqns ); // in/out: equations to build into
  void                     // bas: 
   Build_dmesh_Cd_row      // eff: add in msh-cstrn's Cx=d eqn set
  ( int offset1,           // in : sum of dmod dofs which come before dmod1
    int offset2,           // in : sum of dmod dofs which come before dmod2
    int build_c,           // in : build C flag; 0 for no, 1 for yes
    DS_eqns* dmesh_eqns ); // in/out: equations to build into
  void                     // bas: 
   Build_dmesh_row      // eff: add in msh-cstrn's Cx=d eqn set
  ( int offset1,           // in : sum of dmod dofs which come before dmod1
    int offset2,           // in : sum of dmod dofs which come before dmod2
    int build_LHS,         // in : build LHS flag; 0 for no, 1 for yes
    int build_RHS,         // in : build LHS flag; 0 for no, 1 for yes
    DS_eqns* dmesh_eqns ); // in/out: equations to build into
#else /*DS_USE_SYMEQ*/
  void                     // bas: DS_cstrn virtual function
   Build_dmesh_Cd_row      // eff: add in msh-cstrn's Cx=d eqn set
  (int  nc1_count,         // in : number of xc dofs for dmod1
   int  xc1_offset,        // in : dmesh_C_index = xc1_index+xc1_offset
   int *xc1_map,           // in : xc1_index=>pfunc1_index mapping
                           //      sized:[nc1_count]
   int  nc2_count,         // in : number of xc dofs for dmod2
   int  xc2_offset,        // in : dmesh_C_index = xc1_index+xc1_offset
   int *xc2_map,           // in : xc2_index=>pfunc2_index mapping
                           //      sized:[nc2_count]
   DS_eqns *dmesh_eqns,    // in : holder of dmesh Ax=b, Cx=d eqns
   int next_Cd_row) ;      // in : next available row in Cx=d
  void                     // 
   Build_dmesh_d_row       // eff: build one pt-cstrn's d of Cx=d eqn
  (int  nc1_count,         // in : number of xc dofs for dmod1
   int  xc1_offset,        // in : dmesh_C_index = xc1_index+xc1_offset
   int *xc1_map,           // in : xc1_index=>pfunc1_index mapping
                           //      sized:[nc1_count]
   int  nc2_count,         // in : number of xc dofs for dmod2
   int  xc2_offset,        // in : dmesh_C_index = xc1_index+xc1_offset
   int *xc2_map,           // in : xc2_index=>pfunc2_index mapping
                           //      sized:[nc2_count]
   DS_eqns *dmesh_eqns,    // in : holder of dmesh Ax=b, Cx=d eqns
   int next_Cd_row) ;      // in : next available row in Cx=d
#endif /*DS_USE_SYMEQ*/

   // DS_link_cstrn internal helper function
   void
   Sub_CP_from_d           // eff: set d = d- C*P for default shapes
    (const DS_enforcement_mechanism* mech,
     DS_pfunc *pfunc,      // in : shape being constrained
     DS_eqns *eqns,        // in : holder of Ax=b, Cx=d eqns
     int next_Cd_row,      // in : next available Cx=d row
     int *used_dof_map,    // in : used_dof to global index for this cstrn
                           //      inverse of C_dof_map or Cn_dof_map 
     int used_dof_count,   // in : size of used_dof_map
     double *C) ;          // in : 1st C_row stored in crv_cstrn,
                           //      stored in upper triangular form.
                           //      oneof, lct_C11, lct_Cn11
                           //             lct_C22, lct_Cn22

} ; // end class DS_link_cstrn

//============================================================
// DS_area_constraint
//============================================================

// An area constraint is a SPAposition constraint on a region
// within a larger surface it partitions the surface into
// two regions, a part that is fixed, and a part that is
// free to move.  When 2 or more areas are combined, the
// parts of the surface which are free to move are unioned 
// together to continue dividing the surface up into two parts.
//
// TODO:??? decide on and implement a behavior for area 
//          constraints within a child patch.
// TODO:??? implement a variant of this area patch in
//          which the fixed area is just rigid

class DS_area_cstrn : public DS_cstrn  // identifier = act
{
  protected:
   int     act_zone_flag ; // 0=zone area is free to move
                           //   zone compliment area is fixed
                           // 1=zone area is fixed
                           //   zone compliment area is free to move
   DS_zone *act_zone ;     // defines an area

  public:


 // DS_area_cstrn constructor, copy, Make_copy, Size_arrays, Destructor
 DS_area_cstrn                  // eff: Default Constructor
  (DS_dmod       *dmod  = NULL, // in : constrained shape
   DS_CSTRN_SRC src_type        // in : records cstrn origin. oneof:
                = ds_user_cstrn,//      ds_solid_cstrn=shared topo bndry
                                //      ds_bound_cstrn=unshared topo bndry
                                //      ds_user_cstrn  =user created cstrn
                                //      ds_seam_cstrn  =join 2 hierarchical faces
   void     *src_data  = NULL,  // opt: app data for this cstrn or NULL
   int       zone_flag = 0,     // in : 0=zone area is free to move           
                                //         zone compliment area is fixed       
                                //      1=zone area is fixed                  
                                //        zone compliment area is free to move
   DS_zone  *zone      = NULL,  // in : tgt area [nested]
   int       tag       = -1,    // in : tag for apps
   DS_cstrn *next      = NULL); // in : linked list ptr or NULL [nested]

   DS_area_cstrn                // eff: Copy Constructor
   (DS_area_cstrn &area_cstrn); // in : object being copied

   DS_area_cstrn &operator=     // eff: Assignment Operator
    (DS_area_cstrn &area_cstrn);// in : object being copied

    DS_cstrn*                   // bas: DS_cstrn virtual function
     Make_copy()                // eff: Virtual DS_cstrn copy function
                                { return( new DS_area_cstrn(*this)) ; }

   virtual
  ~DS_area_cstrn() ;            // eff: Destructor

  // DS_area_cstrn simple data access function

  int Zone_flag()          // rtn: zone flag stae
                           //      0=zone area is free to move           
                           //        zone compliment area is fixed       
                           //      1=zone area is fixed                  
                           //        zone compliment area is free to move
                           { return act_zone_flag ; }

  int Set_zone_flag        // eff: set act_zone_flag value
    (int zone_flag) ;      // in : 0=zone area free
                           //      1=zone compliment free

  DS_zone *Zone()          // rtn: ptr to nested zone
                           { return act_zone ; }

private:
  virtual                  // bas: DS_cstrn pure virtual function     
   int Line_row_count      // rtn: total non_min_Cd Cx=d row count
    (DS_dmod * /*dmod*/)        // in : dmod = for this dmod 
                           { // all DS_area_cstrn eqns are in Dof_map
#ifndef DS_USE_SYMEQ
                             if(Is_off()) return(0) ;
                             return(cst_C_row_count) ;
#else /* DS_USE_SYMEQ */
                             return(0) ;
#endif /* DS_USE_SYMEQ */
                           }

  virtual                  // base: DS_cstrn
   int Mix_count           // rtn: Lxi=Lc row count for one cstrn only
   (DS_dmod *)             // in : dmod = for this dmod
                           { // DS_area_cstrn generate no mixed cstrns
                             return(0) ;
                           }
public:

//========================================================
// cstrns only contribute if turned on.
// Is_Cd_contributor, Is_LLc_contributor
// Is_dof_map_contributor, Is_min_map_contributor
// Is_joint_Cd_contributor
//========================================================

  virtual                  // bas: DS_cstrn pure virtual function
  int Is_Cd_contributor    // ret: 1=cstrn adds Cd equations,0=doesn't
    (DS_dmod *,            // in : dmod = for this dmod 
     int =0)               // in : refine_flag = 0=use state bits as is
                           //                    1=treat links as fixed
                           { 
#ifndef DS_USE_SYMEQ
                             return(Is_on() ? 1 : 0) ;
#else
                             return(0) ;
#endif /* DS_USE_SYMEQ */
                           }

  virtual                  // bas: DS_cstrn pure virtual function
  int Is_LLc_contributor   // ret: 1=cstrn adds LLc equations,0=doesn't
    (DS_dmod *)            // in : dmod = for this dmod 
                           { return(0) ;
                           }

  virtual                  // bas: DS_cstrn pure virtual function
  int                      // rtn: 1=cstrn affects min_map,0=doesn't
   Is_dof_map_contributor  // eff: area and link cstrns do,
     ()                    //      pt_cstrns and crv_cstrns do not.
                           { 
#ifndef DS_USE_SYMEQ
                             return(0) ;
#else
                             return( Is_on() ? 1 : 0) ;
#endif /* DS_USE_SYMEQ */
                           }

  virtual                  // bas: DS_cstrn pure virtual function
  int                      // rtn: 1=cstrn adds to min_map,0=doesn't
   Is_min_map_contributor  // eff: crv and link cstrns may add,
                           //      pt_cstrns do not.
    (DS_dmod *,            // in : dmod = for this dmod 
     int =0)               // in : refine_flag = 0=use state bits as is
                           //                    1=treat links as fixed
                           { return(0) ;
                           }

  virtual                  // bas: DS_cstrn pure virtual function
  int                      // rtn: 1=cstrn adds to joint prob Cx=d eqns
   Is_joint_Cd_contributor // eff: link cstrns may add to joint Cx=d
    ()                     { return(0) ;
                           }


  // DS_area_cstrn implementations of DS_cstrn virtual functions
  // side-effect functions
  // DS_area_cstrn implementations of DS_cstrn virtual functions
  // non-side-effect functions

  double                   // bas: DS_cstrn virtual function
   Dist2_to_iline          // ret: dist**2 between cstrn and iline
                           //      iline = pi0 + u(pi1-pi0)
    (double * /*pi0*/,          // in : image space pt, Sized:[image_dim]
     double * /*pi1*/,          // in : image space pt, Sized:[image_dim]
     DS_CST_PT_INDEX       // out: image_pt index in cstrn for closest pt
            &pt_index,     
     double &uu)           // out: iline param for pt closest to cstrn
                           {  // TODO:??? need to find the
                              //          distance between an
                              //          element and a line.
                              //          This looks very expensive
                              //          so we won't do this at
                              //          this time.
                              // for short term -
                              // return a large number
                              uu = 0.0 ;  pt_index=PST_UNDEF ;
                              return(-1) ;
                           }
/* no longer virtual base class function
  virtual void             // bas: DS_cstrn virtual function
   Set_image_pt            // eff: copies ipt into cstrn's image pt
   (double *,              // in : ipt = pt to copy, Sized:Image_dim()
    double ,               // in : tan_display_gain, tpt = bpt+tan_display_gain*tvec
    int )                  // in : pt_index = index of image_pt to update
                           { }

  virtual                  // bas: DS_cstrn virtual function
   int                     // rtn: 0=success,-1=bad_dpt
   Set_domain_pt           // eff: copies dpt into pst_domain_pt
   (double *,              // in : dpt = pt to copy, Sized:Domain_pt_size()
    double ,               // in : tan_display_gain tpt = bpt+tan_display_gain*tvec
    int )                  // orof: 1=calc new base_pt
                           //       2=calc new tang_vec
                           //       4=calc new Wuu,Wuv,Wvv vecs
                           //       0=save old base_pt and tang_vec
                           { return(0) ; }
*/
  virtual int              // bas: DS_cstrn virtual function
   Image_dim() ;           // rtn: image dim for constraint

  virtual int              // bas: DS_cstrn virtual function
   Domain_dim() ;          // rtn: image dim for constraint
  virtual void             // bas: DS_cstrn pure virtual function
   Shift_domain            // eff: shift the basis domain values
    (double *du,           // in : domain shift SPAvector, Sized:Domain_dim()
     DS_dmod *dmod) ;      // in : for this dmod 

  virtual                  // bas: DS_cstrn pure virtual function
   int                     // rtn: 0=done,-1=scale too small
   Scale_domain            // eff: scale the basis domain values
    (double gain,          // in : domain scale factor
     DS_dmod *dmod) ;      // in : for this dmod 

  virtual                  // bas: DS_cstrn virtual function
   int                     // rtn: 0=success,-1=bad_dpts
   Update_pts              // eff: update all cstrn cached data
                           //      after changes in pfunc
   (double ,  // in : tan_display_gain = tang_pt = base_pt + tan_display_gain*tang_vec
    double ,  // in : C2_display_gain = curv_pt = base_pt + C2_display_gain *tang_vec
    int )                  // orof: pfunc_state = 
                           //       DS_PFN_CHANGED_X      = X val change
                           //       DS_PFN_CHANGED_P_ON   = Set def shape
                           //       DS_PFN_CHANGED_P_OFF  = Clear def shape
                           //       DS_PFN_CHANGED_DOMAIN = Domain change
                           //       DS_PFN_TRACKING       = tracking parent
#ifndef DS_USE_SYMEQ
                           ;
#else
                           { return 0 ; }
#endif //DS_USE_SYMEQ

  virtual void             // bas: DS_cstrn virtual function
   Build_Cd_row            // eff: build one pt-cstrn's Cx=d eqn row
    (DS_dmod *,            // in : dmod = shape to constrain
     DS_eqns *,            // in : symeq = holder of Ax=b, Cx=d eqns
     int &,                // i/o: next_Cd_row = next available Cx=d row
     int =0)               // in : refine_flag = 0=use state bits as is
                           //      1=treat links as fixed
#ifndef DS_USE_SYMEQ
                           ;
#else
                           { }
#endif /* DS_USE_SYMEQ */

  virtual void             // bas: DS_cstrn virtual function
   Build_d_row             // eff: build one pt-cstrn's d of Cx=d eqn
    (DS_dmod *,            // in : dmod = shape to constrain
     DS_eqns *,            // in : symeq = holder of Ax=b, Cx=d eqns
     int &)                // i/o: next_Cd_row = next available Cx=d row
#ifndef DS_USE_SYMEQ
                           ;
#else
                           { }
#endif /* DS_USE_SYMEQ */

  virtual void             // bas: DS_cstrn virtual function                        
   Build_L_Lc_row          // eff: build one pt-cstrn's Lxi=Lc eqn rows             
    (DS_dmod *,            // in : dmod = shape to constrain
     DS_eqns *,            // in : symeq = holder of Ax=b, Cx=d eqns
     int )                 // in : next_L_Lc_row = 1st available row of Li xi=Lc
                           { }

  virtual void             // bas: DS_cstrn virtual function
   Build_Lc_row            // eff: build one pt-cstrn's d of Cx=d eqn
    (DS_dmod *,            // in : dmod = shape to constrain
     DS_eqns *,            // in : symeq = holder of Ax=b, Cx=d eqns
     int )                 // in : next_L_Lc_row = 1st available row of Li xi=Lc
                           { }
#ifndef DS_USE_SYMEQ
#else
  virtual                  // bas: DS_cstrn virtual function
  int                      // rtn: free_dof_count, -2=no effect or -1=error                     
   Mark_this_free_dofs     // eff: mark all free dofs and elems in input
   (DS_dmod *dmod,         // in : shape to constrain
    int dof_count,         // in : sizeof dof_map
    int *dof_map,          // i/o: dof_map for constrained pfunc
    int &free_dof_count,   // out: dofs changed from fixed to free
    int elem_count,        // in : sizeof elem_state
    int *elem_state,       // i/o: 0=free to move, 1=fixed
    int &free_elem_count); // out: elems changed from fixed to free
#endif //DS_USE_SYMEQ

  void                     // rtn: list of fixed dofs
   Fixed_dofs              // eff: build or rtn fixed list of dofs
   (int &dof_count,        // out: number of dofs in dof_list
    int *&dof_list,        // out: list of dof index values
    int &free_flag) ;      // out: 0=rtn arg is referenced - do not delete
                           //      1=rtn arg is temporary  - do delete

  virtual void             // bas: DS_cstrn virtual function
  Bad_track_response       // eff: reset tracked cstrns. 
  (double ,                // in : tan_display_gain = tang_pt = base_pt + tan_display_gain*tang_vec
   double                  // in : C2_display_gain = curv_pt = base_pt + C2_display_gain *tang_vec
   )                       //      Called when tracking yields cstraint 
                           //      confilcts
                           { }

  virtual                  // bas: DS_cstrn virtual function
  int                      // rtn: 0=success,-1=file failure
   Dump                    // eff: write all internal vals to file
    (char *filename,       // in : filename = file to write
     FILE *fp)             // in : existing file or NULL
                           { // local file access
                             int close = 0 ;
                             if(NULL == fp){fp = fopen(filename, "w");
                                            close = 1 ;
                                           }
                             if (!fp) return(-1) ;

                             DS_cstrn::Dump(filename,fp);
                             if(close) fclose(fp) ;
                             return(0) ; 
                           }
} ; // end class DS_area_cstrn 

/* obsolete idea for building approx_shape

//=============================================================
//  class DS_shape_cstrn
//=============================================================

// A shape constraint is used to force the shape of one 
// pfunc to be as close as possible to the shape of another.
// The domain of the pfunc being copied must be a proper super-
// set of the domain of the pfunc being modified (e.g. the same
// size or bigger).
// DS_shape_cstrns and DS_shape_cstrns are very similar.  The only
// difference being that a DS_shape_cstrn is really a curve in
// a surface (or solid) constraint while a DS_shape_cstrn is
// a constraint on the entire shape of a DS_pfunc independent
// of domain_dim.
//
// The intended use for this constraint is as a one shot
// stand-alone deal.  The DS_shape_cstrn constraint by itself 
// should completely constrain all the dofs of the target DS_pfunc.
// The constraint should be applied only by itself, e.g. all
// other constraints should be turned off temporarily.
// After one solution cycle the constraint should be removed
// so that the target surface can return to being a deformable
// surface.  Proper use of the set_default capability will
// preserve the effect of this constraint.

// DS_SHAPE_CSTRN graphics:  There are no graphics for this cstrn
//          
// nomenclature:  the tgt_pfunc is the one being modified
//                the src_pfunc is the shape being approximated

class DS_shape_cstrn : public DS_cstrn  // identifier = shc
{
  protected:

   int shc_surf_dof_count; // [array_size] dof count in tgt_pfunc
   int shc_image_dim ;     // [array_size] problem image dim size
   int shc_ntgrl_degree ;  // [array_size] ntgrl accuracy in each elem

   DS_pfunc *shc_src_pfunc ;   // The shape being approximated

   double *shc_Cd ;        // Crv cstrn's C and d SPAmatrix entries
                           // Sized:[ (  shc_surf_dof_count
                           //          + shc_image_dim)
                           //          *shc_surf_dof_count) ]
                           // Order:[CD0(C00,..,C0n,scale_0(xyz)),CD1(xyz),...]

  public:
   // DS_shape_cstrn Constructor, copy, Make_copy, Size_arrays, Destructor

   DS_shape_cstrn              // Default Constructor
    (DS_pfunc *tgt_pfunc=NULL, // The shape being constrained
     DS_pfunc *src_pfunc=NULL, // The shape being approximated
     int ntgrl_degree=         // The accuracy of gauss integration
       DS_DEFAULT_NTGRL_DEGREE,
     int tag=-1,               // tag so apps can track cstrns
     DS_cstrn *next=NULL) ;    // linked list pointer     

                           // eff: Copy Constructor
   DS_shape_cstrn          // rtn: deep copy of shape_cstrn
    (DS_shape_cstrn &      // in : object being copied 
       shape_cstrn) ;

   DS_cstrn*               // bas: DS_cstrn virtual function
    Make_copy()            // eff: Virtual DS_cstrn copy function
                           { return( new DS_shape_cstrn(*this)) ; }

   void
    Size_arrays            // Size_arrays
    (int surf_dof_count=0, // dof_count in associated pfunc
     int image_dim=0,      // Size of the problem's image space
     int ntgrl_degree=0) ; // degree polynomial ntgrated in elems

   virtual
  ~DS_shape_cstrn() ;      // Destructor
                           //   delete shc_src_pfunc ;
                           //   Size_arrays() ;

  // DS_shape_cstrn simple data access function

  int     Surf_dof_count() { return shc_surf_dof_count ; }
  int     Image_dim()      { return shc_image_dim ; }
  int     Ntgrl_degree()   { return shc_ntgrl_degree ; }

  DS_pfunc *Src_pfunc()    { return shc_src_pfunc ; }

  double *Cd()             { return shc_Cd ; }
  double *C_row(int i)     { DS_assert(i >= 0 && i < shc_surf_dof_count);
                             return (  shc_Cd 
                                     + i * (  shc_surf_dof_count
                                            + shc_image_dim)) ;
                           }
  double *D_row(int i)     { DS_assert(i >= 0 && i < shc_surf_dof_count);
                             return (  shc_Cd 
                                     + shc_surf_dof_count
                                     + i * (  shc_surf_dof_count
                                            + shc_image_dim)) ;
                           }

  // DS_shape_cstrn internal array size functions
                                
  int Cd_size              // Cd array size
    (int surf_dof_count,   
     int image_dim)        { return (  (surf_dof_count + image_dim)
                                     *  surf_dof_count) ; 
                           }
  int Scratch_domain_size  // rtn: array size needed for 1 elem dpts
    (int domain_dim,       // in : domain dimension for problem
     int ntgrl_degree) ;   // in : accuracy of integration in elem

  int Scratch_image_size   // rtn: array size needed for 1 elem vals
    (int image_dim,        // in : image dimension for problem
     int ntgrl_degree) ;   // in : accuracy of integration in elem

  // DS_shape_cstrn internal side-effect functions

  void                     // use: called by Default Constructor
   Calc_Cd                 // eff: calc and store C and d
                           //      arrays for tgt_pfunc and src_pfunc
    (DS_pfunc *tgt_pfunc,  // in : Shape Model, parametric function
     int d_flag) ;         // in : Select which vals to use in d SPAmatrix
                           //      0=use src_pfunc X (total shape)
                           //      1=use src_pfunc P (default shape)
                           //      Where X = P + Q

                           // use: called by Default Constructor and
  void                     //      after P(pfunc->dof_def) changes
   Calc_d                  // eff: calc and store d array d=C(X-P)
    (DS_pfunc *tgt_pfunc,  // in : Shape Model, parametric function
     int d_flag) ;         // in : Select which vals to use in d SPAmatrix
                           //      0=use src_pfunc X (total shape)
                           //      1=use src_pfunc P (default shape)
                           //      Where X = P + Q

  //  implementations of DS_cstrn virtual functions
  // DS_shape_cstrn non-side-effect functions

  double                   // bas: DS_cstrn virtual function
   Dist2_to_iline          // ret: dist**2 between cstrn and iline
                           //      iline = pi0 + u(pi1-pi0)
    (double *pi0,          // in : image space pt, Sized:[image_dim]
     double *pi1,          // in : image space pt, Sized:[image_dim]
     double &uu)           // out: iline param for pt closest to cstrn
                           { // a no-op function
                             return(-1.0) ;
                           }
 
  //  implementations of DS_cstrn virtual functions
  // DS_shape_cstrn side-effect functions

  virtual void             // bas: DS_cstrn virtual function
   Update_pts              // eff: update all cstrn cached data
                           //      after changes in pfunc
   (DS_pfunc *pfunc,       // in : Shape being constrained
    DS_pfunc *parent_pfunc,// in : Shape's parent shape for hierarchies
    int pfunc_state)=0;    // in : orof 1=X changed
                           //           2=P changed
                           //           4=Domain changed
                           { // a no-op function
                           }
        
  virtual void             // bas: DS_cstrn virtual function
   Set_shape               // use: called after P(pfunc->dof_def) 
                           //      changes
    (DS_pfunc *tgt_pfunc)  // in : parametric function to constrain
                           { Calc_d(tgt_pfunc,0) ; }

  virtual void             // bas: DS_cstrn virtual function
   Build_Cd_row            // eff: build one pt-cstrn's Cx=d eqn row
    (DS_dmod *dmod,        // in : shape to constrain
     int *cstrn_dof_map,   // in : global to used dof map for cstrns.
                           //      sized:[pfunc->Domain_dim()]
     int min_Cd_dof_count, // in : global used_dof count
     int C_row_size,       // in : number of entries in one row of C
     int C_row_stride,     // in : memory distance between C rows
     double *C,            // in : array to store C in Cx=d
                           // Sized:[pfunc->Dof_count()*Row_count()]
     int d_row_size,       // in : number of entries in one row of d
     int d_row_stride,     // in : memory distance between d rows
     double *d) ;          // in : array to store d (copies ipt to d)
                           // Sized:[pfunc->Image_dim()*Row_count()]

  virtual void             // bas: DS_cstrn virtual function
   Build_d_row             // eff: build one pt-cstrn's d of Cx=d eqn
    (DS_dmod *dmod,        // in : shape to constrain
     int *cstrn_dof_map,   // in : global to used dof map for cstrns.
                           //      sized:[pfunc->Domain_dim()]
     int min_Cd_dof_count, // in : global used_dof count
     int d_row_size,       // in : number of entries in one row of d
     int d_row_stride,     // in : memory distance between d rows
     double *d) ;          // in : array to store d in Cx=d
                           // Sized:[pfunc->Image_dim()*Row_count()]

} ; // end class DS_shape_cstrn

// end obsolete approach for building approx_shape */

/* // obsolete approach for enforcing end conditions

// End condition constraints are now applied by the pfunc object
// directly to the Cd SPAmatrix without creating a bunch of
// intermediate cstrn objects.
// This had to be done because a pfunc may enforce 
// closed and periodic end conditions as either a change in
// the representation or as a set of constraint on dof values.

//=============================================================
//  class DS_dof_pair
//=============================================================

// constrain two dofs to always be the same

class DS_dof_pair : public DS_cstrn  // identifier = dfp
{
  protected:
   int     dfp_dof0_index; // one constrained dof's global dof #
   int     dfp_dof1_index; // other constrained dof's global dof #

  public:
   // DS_dof_pair constructor, copy, Make_copy, Size_arrays, Destructor

   DS_dof_pair             // Default Constructor
    (int dof0_index=0,     // one constrained dof's global dof #
     int dof1_index=0,     // other constrained dof's global dof #
     int tag=-1,           // tag so apps can track cstrns
     DS_cstrn *next=NULL)  // linked list pointer
                           :DS_cstrn(ds_dfp, // dfp identifier
                                     1,      // each dfp has 1 row
                                     tag,
                                     next)
                           { dfp_dof0_index = dof0_index ;
                             dfp_dof1_index = dof1_index ;
                           }

   DS_dof_pair             // eff: Copy Constructor
   (DS_dof_pair &dof_pair) // in : object being copied
                           : DS_cstrn(dof_pair)
                           {
                             dfp_dof0_index = dof_pair.dfp_dof0_index ;
                             dfp_dof1_index = dof_pair.dfp_dof1_index ;
                           }

   DS_cstrn*               // bas: DS_cstrn virtual function
    Make_copy()            // eff: Virtual DS_cstrn copy function
                           { return( new DS_dof_pair(*this)) ; }

   virtual
  ~DS_dof_pair()           // Destructor
                           {  }

  // DS_dof_pair simple data access function

  int     dof0_index()     { return dfp_dof0_index ; }
  int     dof1_index()     { return dfp_dof1_index ; }
  void Set_dof_indices
    (int dof0_index,
     int dof1_index)       { dfp_dof0_index = dof0_index ;
                             dfp_dof1_index = dof1_index ;
                           }

  //  implementations of DS_cstrn virtual functions
  // DS_dof_pair non-side-effect functions

  double                   // bas: DS_cstrn virtual function
   Dist2_to_iline          // ret: dist**2 between cstrn and iline
                           //      iline = pi0 + u(pi1-pi0)
    (double *pi0,          // in : image space pt, Sized:[image_dim]
     double *pi1,          // in : image space pt, Sized:[image_dim]
     double &uu)           // out: iline param for pt closest to cstrn
                           {
                             // can't select a dof_pair constraint
                             return(-1) ;
                           }
 
  //  implementations of DS_cstrn virtual functions
  // DS_dof_pair side-effect functions

  void                     // bas: DS_cstrn virtual function
   Build_Cd_row            // eff: build one pt-cstrn's Cx=d eqn row
    (DS_dmod *dmod,        // in : shape to constrain
     int *cstrn_dof_map,   // in : global to used dof map for cstrns.
                           //      sized:[pfunc->Domain_dim()]
     int min_Cd_dof_count, // in : global used_dof count
     int C_row_size,       // in : number of entries in one row of C
     int C_row_stride,     // in : memory distance between C rows
     double *C,            // in : array to store C in Cx=d
                           // Sized:[pfunc->Dof_count()*Row_count()]
     int d_row_size,       // in : number of entries in one row of d
     int d_row_stride,     // in : memory distance between d rows
     double *d) ;          // in : array to store d (copies ipt to d)
                           // Sized:[pfunc->Image_dim()*Row_count()]

  void                     // bas: DS_cstrn virtual function
   Build_d_row             // eff: build one pt-cstrn's d of Cx=d eqn
    (DS_dmod *dmod,        // in : shape to constrain
     int *cstrn_dof_map,   // in : global to used dof map for cstrns.
                           //      sized:[pfunc->Domain_dim()]
     int min_Cd_dof_count, // in : global used_dof count
     int d_row_size,       // in : number of entries in one row of d
     int d_row_stride,     // in : memory distance between d rows
     double *d) ;          // in : array to store d in Cx=d
                           // Sized:[pfunc->Image_dim()*Row_count()]

} ; // end class DS_dof_pair

//=============================================================
//  class DS_dof_tangent
//=============================================================

// constrain two pairs of dofs to su
//  This constraint is to be used to enforce the
//  "periodic" C1 continuity condition between NUB and NURB 
//  surfaces and curves who have multiple end knots such that the 
//  curve or surface interpolates the first and last control_point
//  locations.  For such shapes the control_points are the dofs.
//  Tangent C1 continuity is guaranteed
//  if the first and last control points are coincident and
//  the end points, the second control point, and the next
//  to last control point are on the same line.
//
//  Tangent continuity for a surface is enforced if the
//  DS_dof_tangent constraint is used for each row of control
//  points that crosses the periodic seam.
//
//  DS_dof_tangent constraints
//   dof_0 - dof_last = 0              // first and last constraint
//                                     // are coincident
//   scale_d(dof_1 - dof_0 ) 
//        - (dof_n - dof_n1) = 0       // end dofs are colinear and
//                                     // regularly spaced
//
//   For C1 continuity:
//     NUBs with multiple end knots so the shape interps end cpts
//    
//         scale_d = (knot_1 - knot_0)/(knot_n - knot_n1)
//
//     NURBs with multiple end knots so the shape interps end cpts
//       TODO:??? work out the scale_0 factor
//
//   Where:
//      dof_0  = the first dof in the row being constrained
//      dof_1  = the second dof in the row being constrained
//      dof_n  = the last dof in the row being constrained
//      dof_n1 = the pentultimate dof in the row being constrained
//      knot_0 = 1st unique knot value in the row being constrained
//      knot_1 = 2nd unique knot value in the row being constrained
//      knot_n = last unique knot value in the row being constrained
//      knot_n1= pentultimate unique knot value in the constrained row
//

//         
//   this is done by forcing dofa1 and dofb0 to be the same
//   and forcing scale_0(a1 - a0) = (b1 - b0)
//   use this to enforce tangent continuity across NUB and NURB
//   surfaces

class DS_dof_tangent : public DS_cstrn  // identifier = dft
{
  protected:
                           // An index is a global dof number
   int    dft_index_0 ;    // first        dof in constraint row
   int    dft_index_1 ;    // 2nd          dof in constraint row
   int    dft_index_n1 ;   // next to last dof in constraint row
   int    dft_index_n ;    // last         dof in constraint row
   double dft_scale_0 ;    // scale factor for beginning tangent
                           // tangent_1 = scale_0(index_1 - index_0)
                           // tangent_2 = (index_n - index_n1)
                           // constraints:  dof_0     = dof_n
                           //               tangent_1 = tangent_2
                           // C1 NUBs: scale_d =  (knot_1 - knot_0)
                           //                   / (knot_n - knot_n1)
                           // C1 NURBS:scale_d = ???
  public:
   // DS_dof_tangent constructor, copy, Make_copy, Size_arrays, Destructor

   DS_dof_tangent          // Default Constructor
    (int index_0   = 0,    // tangent_1 = scale_0(a1 - a0)
     int index_1   = 0,    // 
     int index_n1  = 0,    // tangent_2 = (b1 - b0)
     int index_n   = 0,    // 
     double scale_0=1.0,   // scale applied to tangent_1
     int tag=-1,           // tag so apps can track cstrns
     DS_cstrn *next=NULL)  // linked list pointer
                           :DS_cstrn(ds_dft, // dft identifier
                                     2,      // each dft has 2 rows
                                     tag,
                                     next)
                           { // TODO: error handling
                             DS_assert(index_0  != index_1 ) ;
                             DS_assert(index_0  != index_n1) ;
                             DS_assert(index_0  != index_n ) ;
                             DS_assert(index_1  != index_n1) ;
                             DS_assert(index_1  != index_n ) ;
                             DS_assert(index_n1 != index_n ) ;
                             DS_assert(DS_not_zero(scale_0,DS_DOUBLE_SMALL)) ;
                             dft_index_0  = index_0 ;
                             dft_index_1  = index_1 ;
                             dft_index_n1 = index_n1 ;
                             dft_index_n  = index_n ;
                             dft_scale_0  = scale_0 ;
                           }

   DS_dof_tangent             // Copy Constructor
   (DS_dof_tangent &dof_tangent) // in : object being copied
                           : DS_cstrn(dof_tangent)
                           {
                             dft_index_0  = dof_tangent.dft_index_0 ;
                             dft_index_1  = dof_tangent.dft_index_1 ;
                             dft_index_n1 = dof_tangent.dft_index_n1;
                             dft_index_n  = dof_tangent.dft_index_n ;
                             dft_scale_0  = dof_tangent.dft_scale_0 ;
                           }

   DS_cstrn*               // bas: DS_cstrn virtual function
    Make_copy()            // eff: Virtual DS_cstrn copy function
                           { return( new DS_dof_tangent(*this)) ; }

   virtual
  ~DS_dof_tangent()           // Destructor
                           {  }

  // DS_dof_tangent simple data access function

  int Dof_0_index()        { return dft_index_0 ; }
  int Dof_1_index()        { return dft_index_1 ; }
  int Dof_n_index()        { return dft_index_n ; }
  int Dof_n1_index()       { return dft_index_n1 ; }
  double Scale_0()         { return dft_scale_0 ; }

  //  implementations of DS_cstrn virtual functions
  // DS_dof_tangent non-side-effect functions

  double                   // bas: DS_cstrn virtual function
   Dist2_to_iline          // ret: dist**2 between cstrn and iline
                           //      iline = pi0 + u(pi1-pi0)
    (double *pi0,          // in : image space pt, Sized:[image_dim]
     double *pi1,          // in : image space pt, Sized:[image_dim]
     double &uu)           // out: iline param for pt closest to cstrn
                           {
                             // can't select a dof_pair constraint
                             return(-1) ;
                           }
 
  //  implementations of DS_cstrn virtual functions
  // DS_dof_tangent side-effect functions

  void                     // bas: DS_cstrn virtual function
   Build_Cd_row            // eff: build one pt-cstrn's Cx=d eqn row
    (DS_dmod *dmod,        // in : shape to constrain
     int *cstrn_dof_map,   // in : global to used dof map for cstrns.
                           //      sized:[pfunc->Domain_dim()]
     int min_Cd_dof_count, // in : global used_dof count
     int C_row_size,       // in : number of entries in one row of C
     int C_row_stride,     // in : memory distance between C rows
     double *C,            // in : array to store C in Cx=d
                           // Sized:[pfunc->Dof_count()*Row_count()]
     int d_row_size,       // in : number of entries in one row of d
     int d_row_stride,     // in : memory distance between d rows
     double *d) ;          // in : array to store d (copies ipt to d)
                           // Sized:[pfunc->Image_dim()*Row_count()]

  void                     // bas: DS_cstrn virtual function
   Build_d_row             // eff: build one pt-cstrn's d of Cx=d eqn
    (DS_dmod *dmod,        // in : shape to constrain
     int *cstrn_dof_map,   // in : global to used dof map for cstrns.
                           //      sized:[pfunc->Domain_dim()]
     int min_Cd_dof_count, // in : global used_dof count
     int d_row_size,       // in : number of entries in one row of d
     int d_row_stride,     // in : memory distance between d rows
     double *d) ;          // in : array to store d in Cx=d
                           // Sized:[pfunc->Image_dim()*Row_count()]

} ; // end class DS_dof_tangent

*/  // end of obsolete code


/* obsolete class approach for defining pt tangent constraints
//=============================================================
//  class DS_tan_pt_cstrn
//=============================================================

// DS_tan_pt_cstrn graphics:  The tan_pt_cstrn graphics consists of two
//                          point markers and one 2pt polyline marker.
//  cst_grobj[0] = point_marker_handle
//  cst_grobj[1] = tang_end_marker_handle
//  cst_grobj[2] = tang_polyline_handle
//

class DS_tan_pt_cstrn : public DS_cstrn  // identifier = tnc
{
  protected:
   int     tnc_elem_index; // cache the elem_index for the pt. When
                           // set to -1 it is a flag to recompute in
                           // call Calc_def_and_image_pts()

   int     tnc_image_dim ; // [array_size] problem image dim size
   int     tnc_domain_dim; // [array_size] problem domain dim size

   double *tnc_domain_pt ; // domain point constraint SPAposition
                           // Sized:[tnc_domain_dim]
   double *tnc_image_pt ;  // image loc of domain_pt on surface
                           // Sized:[tnc_image_dim] (X of X = P + Q)
   double *tnc_tang_pt ;   // image loc of tang end, tpt = X + dX
                           // Sized:[tnc_image_dim]
   double *tnc_tang_vec ;  // constrained value for tangent at given pt
                           // Sized:[tnc_image_dim] (dX of dX = dP + dQ)
   double *tnc_tang_old ;  // last valid tangent_pt saved for un-doing
                           // illegal pt tracking
   double *tnc_tang_def ;  // constrained value for tangent on default_shape
                           // Sized:[tnc_image_dim] (dP of dX = dP + dQ)

  public:
   // DS_tan_pt_cstrn constructor, copy, Make_copy, Size_arrays, Destructor

  DS_tan_pt_cstrn          // eff: Default Constructor
   (DS_pfunc *pfunc,       // in : shape being constrained
    double *domain_pt=NULL,// in : cstrn_pt loc, sized:pfunc->Domain_dim()
    double *tang_vec=NULL, // in : tan_val or NULL, sized:pfunc->Image_dim()
    int tag=-1,            // in : tag so apps can track cstrns
    DS_cstrn *next=NULL,   // in : linked list pointer
    int elem_index= -1) ;  // in : cached elem_index for ipt location     

   DS_tan_pt_cstrn           // Copy Constructor
   (DS_tan_pt_cstrn &) ;     // in : object being copied

   DS_tan_pt_cstrn &operator=  // assignment operator
    (DS_tan_pt_cstrn &) ;

   DS_cstrn*               // bas: DS_cstrn virtual function
    Make_copy()            // eff: Virtual DS_cstrn copy function
                           { return( new DS_tan_pt_cstrn(*this)) ; }

   void
    Size_arrays            // Size_arrays
     (int image_dim=0,     // Size of the problem's image space
      int domain_dim=0) ;  // Size of the problem's domain space

   virtual
  ~DS_tan_pt_cstrn() ;         // Destructor

  // DS_tan_pt_cstrn simple data access function
  int  Elem_index()        { return tnc_elem_index ; }
  void Clear_elem_index()  { tnc_elem_index = -1 ; }
  int  Image_dim()         { return tnc_image_dim ; }
  int  Domain_dim()        { return tnc_domain_dim ; }

  double *Domain_pt()      { return tnc_domain_pt ; }
  double &Domain_pt(int d) { DS_assert(d >= 0 && d < tnc_domain_dim) ;
                             return( tnc_domain_pt[d]) ; 
                           }
  double *Image_pt()       { return tnc_image_pt ; }
  double &Image_pt(int i)  { DS_assert(i >= 0 && i < tnc_image_dim) ;
                             return tnc_image_pt[i] ; 
                           }
  double *Tang_pt()        { return tnc_tang_pt ; }
  double &Tang_pt(int i)   { DS_assert(i >= 0 && i < tnc_image_dim) ;
                             return tnc_tang_pt[i] ; 
                           }
  double *Tang_vec()       { return tnc_tang_vec ; }
  double &Tang_vec(int i)  { DS_assert(i >= 0 && i < tnc_image_dim) ;
                             return( tnc_tang_vec[i]) ; 
                           }
  double *Tang_old()       { return tnc_tang_old ; }
  double &Tang_old(int i)  { DS_assert(i >= 0 && i < tnc_image_dim) ;
                             return( tnc_tang_old[i]) ; 
                           }
  double *Tang_def()       { return tnc_tang_def ; }
  double &Tang_def(int i)  { DS_assert(i >= 0 && i < tnc_image_dim) ;
                             return( tnc_tang_def[i]) ; 
                           }

  virtual void             // bas: DS_cstrn virtual function
   Set_domain_pt           // eff: copies dpt into tnc_domain_pt
   (double *dpt,           // in : pt to copy, Sized:Domain_pt_size()
    int image_save_flag) ; // in : 0=calc new image constraint values
                           // in : 1=save old image constraint values

  void Set_tang_vec        // eff: copies tang_vec into tnc_tang_vec
   (double *tang_vec)      // in : pt to copy, Sized:Image_pt_size()
                           { DS_assert(tang_vec != NULL) ;
                             if((cst_behavior & DS_CST_TRACKING) == 0)
                               {
                                 DS_copy_double_block(tnc_tang_old,
                                                      tnc_tang_vec,
                                                      tnc_image_dim);
                                 cst_behavior |= DS_CST_TRACKING ;
                               }
                             DS_copy_double_block(tnc_tang_vec,
                                                  tang_vec,
                                                  tnc_image_dim) ;
                             DS_add_vec(tnc_image_dim,
                                        tnc_tang_vec,
                                        tnc_image_pt,
                                        tnc_tang_pt) ;
                           }

  virtual void             // bas: DS_cstrn virtual function
   Set_image_pt            // eff: set the tang_pt and recalc tang_vec
   (double *tang_pt)       // in : image space loc for tang_pt
                           { DS_assert(tang_pt != NULL) ;
                             if((cst_behavior & DS_CST_TRACKING) == 0)
                               {
                                 DS_copy_double_block(tnc_tang_old,
                                                      tnc_tang_vec,
                                                      tnc_image_dim);
                                 cst_behavior |= DS_CST_TRACKING ;
                               }
                             DS_copy_double_block(tnc_tang_pt,
                                                  tang_pt,
                                                  tnc_image_dim) ;
                             DS_sub_vec(tnc_image_dim,
                                        tnc_tang_pt,
                                        tnc_image_pt,
                                        tnc_tang_vec) ;

                           }                             
  void Set_tang_def        // eff: copies tang_def into tnc_tang_def
   (double *tang_def)      // in : vec to copy, Sized:Image_pt_size()
                           { DS_assert(tang_def != NULL) ;
                             DS_copy_double_block(tnc_tang_def,
                                                  tang_def,
                                                  tnc_image_dim) ;
                           }

  void                     // eff: calc and store default and
   Calc_def_and_image_pts  //      image pts for current domain pt
    (DS_pfunc *pfunc,      // in : Shape Model, parametric function
     int work_flag) ;      // orof: 1 = recalc tnc_image_pt & tnc_tang_pt
                           //       2 = recalc tnc_tang_vec & tnc_tang_pt
                           //       4 = recalc tnc_tang_def

  // stored array sizes
  int Image_pt_size
   (int image_dim)         { return image_dim ; }

  int Domain_pt_size
   (int domain_dim)        { return domain_dim ; }

  int Block_size
   (int image_dim, 
    int domain_dim)        {return(  image_dim      // image_pt
                                   + image_dim      // tang_pt
                                   + image_dim      // tang_vec
                                   + image_dim      // tang_old
                                   + image_dim      // tang_def  
                                   + domain_dim) ;  // domain_pt
                           }

  //  implementations of DS_cstrn virtual functions
  // DS_tan_pt_cstrn non-side-effect functions

  double                   // bas: DS_cstrn virtual function
   Dist2_to_iline          // ret: dist**2 between cstrn and iline
                           //      iline = pi0 + u(pi1-pi0)
    (double *pi0,          // in : image space pt, Sized:[image_dim]
     double *pi1,          // in : image space pt, Sized:[image_dim]
     double &uu) ;         // out: iline param for pt closest to cstrn
 
  //  implementations of DS_cstrn virtual functions
  // DS_tan_pt_cstrn side-effect functions

  virtual void             // bas: DS_cstrn virtual function
   Update_pts              // eff: update all cstrn cached data
                           //      after changes in pfunc
   (DS_pfunc *pfunc,       // in : Shape being constrained
    DS_pfunc *parent_pfunc,// in : Shape's parent shape for hierarchies
    int pfunc_state) ;     // orof: DS_PFN_CHANGED_X      = X val change
                           //       DS_PFN_CHANGED_P_ON   = Set def shape
                           //       DS_PFN_CHANGED_P_OFF  = Clear def shape
                           //       DS_PFN_CHANGED_DOMAIN = Domain change

  virtual void             // bas: DS_cstrn virtual function
   Build_Cd_row            // eff: build one tang-cstrn's Cx=d eqn row
    (DS_dmod *dmod,        // in : shape to constrain
     int *cstrn_dof_map,   // in : global to used dof map for cstrns.
                           //      sized:[pfunc->Domain_dim()]
     int min_Cd_dof_count, // in : global used_dof count
     int C_row_size,       // in : number of entries in one row of C
     int C_row_stride,     // in : memory distance between C rows
     double *C,            // in : array to store C in Cx=d
                           // Sized:[pfunc->Dof_count()*Row_count()]
     int d_row_size,       // in : number of entries in one row of d
     int d_row_stride,     // in : memory distance between d rows
     double *d) ;          // in : array to store d (copies ipt to d)
                           // Sized:[pfunc->Image_dim()*Row_count()]

  virtual void             // bas: DS_cstrn virtual function
   Build_d_row             // eff: build one pt-cstrn's d of Cx=d eqn
    (DS_dmod *dmod,        // in : shape to constrain
     int *cstrn_dof_map,   // in : global to used dof map for cstrns.
                           //      sized:[pfunc->Domain_dim()]
     int min_Cd_dof_count, // in : global used_dof count
     int d_row_size,       // in : number of entries in one row of d
     int d_row_stride,     // in : memory distance between d rows
     double *d) ;          // in : array to store d in Cx=d
                           // Sized:[pfunc->Image_dim()*Row_count()]

  virtual void             // bas: DS_cstrn virtual function
   Bad_track_response      // eff: reset tracked cstrns. Called when
    ()                     //      tracking yields cstraint conflicts
                           { 
                             if(cst_behavior & DS_CST_TRACKING)
                               { DS_copy_double_block(tnc_tang_vec,
                                                      tnc_tang_old,
                                                      tnc_image_dim);
                                 cst_behavior &= ~DS_CST_TRACKING ;
                               }
                           }

} ; // end class DS_tan_pt_cstrn

*/ // end obsolete tangent point class implementation


#endif // DS_cstrn_H


