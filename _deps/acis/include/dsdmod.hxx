/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dsdmod.hxx
// created  : Jan 4, 1995
// author   : GW Celniker
// contains : DS_dmod, DS_dsurf, DS_dcurv
// synopsis : A deformable model is a SPAmatrix equation
//            Ax(t) = b(t), which optimizes the
//            function xAx - bx subject to linear constraints
//            Cxi=di, and mixed linear constraints Lixi = Lc,
//             and a set of SPAmatrix solution technologies.
//            Derived classes will have to be responsible
//            for building the A SPAmatrix, the b SPAmatrix
//            and copying the answer into the proper places at
//            each iteration. The b SPAmatrix is updated each
//            time iteration by the derived class and a new
//            solution x(t) is found.

// state   : the DS_dmod object manages the input matrices
//           A, b, C, d, L, and Lc.  Changes in the DS_pfunc,
//           DS_cstrns, and
//           DS_loads will cascade into changes in the input
//           matrices.  The cost of the solve loop depends on
//           which input matrices have changed.  DS_dmod needs
//           to know the following states for each input SPAmatrix
//              Matrix_needs_to_be_built        / Matrix_ready
//              Matrix_changed_after_last_solve / Matrix_constant
//
//           The I/O functions     set   the Build_matrix   states
//           The build functions   set   the Matrix_changed states
//                                 clear the Build_matrix   states
//           The Solve() function  clear the Matrix_changed states
//
// hier     : Local deformations are supported by building a hierarchy
// archical : of compatable patches.  The solver
// dimension: A deformable surface is the derived class DS_dsurf
//            A deformable curve   is the derived class DS_dcurv
//
// hierarchical deformable models.
// During sculpting a single FACE or EDGE ENTITY has one
// ATTRIB_DM2ACIS attribute that coordinates all the sculpting
// activities.  That ATTRIB_DM2ACIS object contains a pointer to
// the deformable model represented as a DS_dmod object.  The
// DS_dmod object may be a single deformable model or a hierarchy of
// patches that support global and local deformations.
// The root DS_dmod deformable model pointed to be the ATTRIB_DM2ACIS
// object may have any number of children deformable models organized
// into a pure hierarchical tree.  The hierarchy is stored by three
// pointers, a parent, a child, and a sibling pointer, stored within each
// DS_dmod object.
// The root DS_dmod'd parent_pointer value is set to NULL.
// All children DS_dmod parent_pointer values are non-NULL.
// The child pointer points to the first of a parent's children.
// The sibling pointer is used to store all the children of one parent
// as a linked list.  There is no significance to the order in which
// children are placed on the sibling list.
// Every DS_dmod, constraint, and load within one complete
// heriarchy is assigned a unique tag number so that a user-interface
// may uniquely specify the root_parent, a child, a load, or a cstrn
// by knowing only that object's tag number.
// The root_parent DS_dmod tag is always assigned the value 1.
// All subsequent children, loads, and constraints are assigned tag
// numbers in the order that they are created.  Tag numbers
// remain constant before and after a save and restore cycle.
// Tag numbers of deleted objects are never re-used.
//
// The active DS_dmod: For access efficiency the DS_dmod containing
// the load or cstrn last searched for by a tag value is saved in
// the ATTRIB_DM2ACIS object's root dmod as the value of the
// dmo_active pointer.
// Methods of ATTRIB_DM2ACIS that operate on one DS_dmod, load, or
// constraint default to operating on the active DS_dmod.
// Also when searching for an object by tag value the active DS_dmod
// is always searched first assuming that most commands will operate
// on the same DS_dmod as the last.  When the active DS_dmod fails
// to contain a desired tag object then the DS_dmod hierarchy is
// searched in an exhasutive and systematic fasion.
//
// Draw_state: Each DS_dmod stores its own draw state, i.e. the
// set of information requested by the user to be displayed.
// Actual graphical data and management maintained in the ATTRIB_DM2ACIS
// object
//
// Seams: Seams are just an ordered listing of those constraints
//        that are acting to sew a child patch onto a parent shape.
//        Corners in seams can be found by intersecting the seams
//        in domain space and checking the tangents at the intersection
//        points.  Seams that have corners can not track the tangent
//        angle of the seam constraints.
//        The seam list pointers are redundant to the pointers in
//        the constraint list and so should not be deleted when freeing
//        the containing DS_dmod.
//
// Walking: Now that DS_dmods are a hierarchy every method that causes
//          a side effect on the data stored in the DS_dmod has
//          an opportunity to work in different manners.  Apply
//          the side effect to the 'this' object and quit or
//          apply the side effect to all or some of the objects in
//          hierarchy.
//          This choice is built into the interface of those methods
//          that have this choice to make.  All such methods take
//          an integer argument called walk_flag.
//          When the walk_flag == 0 the side effect will be applied
//          only to the 'this' object.
//          When the walk_flag is == 1 then the side effect will be
//          applied to the 'this' object and recursively to all of
//          its offspring but not to its sibling.
//          When the walk_flag is == 2
//          then the side effect will be applied to the 'this'
//          object and recursively to all of its offspring and
//          and to all of its younger siblings and their offspring.
//
//          Calling such methods on the root object with walk_flag
//          equal to 1 will apply the side-effect to every element
//          in the hierarchy.  Calling such methods on a member
//          object with walk_flag == 1 will cause only that portion of
//          of the hierarchy immediately under the member object
//          to be operated upon.  I expect that the walk_flag value
//          equal to 2 will only be used internally to complete
//          a walk_flag == 1 request, but I suspect that someone
//          else may envision a use for the walk_flag == 2 value
//          and so I leave it in the interface.

#ifndef DS_DSURF_H
#define DS_DSURF_H


#include <math.h>                      // cos(), sin()
#include "dsmat.hxx"   // for DS_diag_mat   // for DS_diag_mat
#include "dsstdef.hxx" // DS_PI // DS_PI
#include "dsload.hxx"  // class DS_load, enum DS_LDS  // class DS_load, enum DS_LDS
#include "dscstrn.hxx" // class DS_cstrn, enum DS_CST // class DS_cstrn, enum DS_CST
#include "dsdmesh.hxx" // class DS_dmesh // class DS_dmesh
#include "dmapinum.hxx"// DM_DRAW_SEAMS, ...// DM_DRAW_SEAMS, ...
                                       // enum DS_DMO
#include "dmicon.hxx"               // Class DM_icon               // Class DM_icon

/**
* @file dsdmod.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup DMAPI
 *
 * @{
 */


// forward references for "use" members and their derived type enums
class DS_pfunc ;                       // parametric functions
#ifndef DS_USE_SYMEQ
#include "dsbridge.hxx" // class DS_bridge // class DS_bridge
  class DS_lueqns ;
#include "dsluslvr.hxx"
#else /*DS_USE_SYMEQ*/
  class DS_symeq ;                       // symetric equations
#endif /*DS_USE_SYMEQ*/
class DS_dmesh ;                       // multisurf complex
class DS_srf_icon;
class DS_crv_icon;

// dmo_abcd_state values
// State that does not need to persistent is set in dmo_abcd_state
// State that does need to persist is set in dmo_draw_state whose
//   bits are defined in the file "dmapi.hxx"
/**
 * 1=rebuild A, 0=ready
 */
#define DMO_REBUILD_A       (1 << 0 ) 
/**
 * 1=A changed since last Solve()
 */
#define DMO_CHANGED_A       (1 << 1 ) 
/**
 * 1=rebuild b, 0=ready
 */
#define DMO_REBUILD_B       (1 << 2 ) 
/**
 * 1=b changed since last Solve()
 */
#define DMO_CHANGED_B       (1 << 3 ) 
/**
 * 1=rebuild C, 0=ready
 */
#define DMO_REBUILD_C       (1 << 4 ) 
/**
 * 1=C changed since last Solve()
 */
#define DMO_CHANGED_C       (1 << 5 ) 
/**
 * 1=rebuild d, 0=ready
 */
#define DMO_REBUILD_D       (1 << 6 ) 
/**
 * 1=d changed since last Solve()
 */
#define DMO_CHANGED_D       (1 << 7 ) 
/**
 * 1= rebuild L, 0=ready
 */
#define DMO_REBUILD_L       (1 << 8 ) 
/**
 * 1=L changed since last Solve()
 */
#define DMO_CHANGED_L       (1 << 9 ) 
/**
 * 1= rebuild Lc, 0=ready
 */
#define DMO_REBUILD_LC      (1 << 10) 
/**
 * 1=Lc changed since last Solve()
 */
#define DMO_CHANGED_LC      (1 << 11) 
/**
 * 1= rebuild min_Cd_map, 0 = ready
 */
#define DMO_REBUILD_MIN_MAP (1 << 12) // 
/**
 * 1=Cd Map changed since last Solve
 */
#define DMO_CHANGED_MIN_MAP (1 << 13) // 
/**
 *  1= init dof_map, 0 = ready
 */
#define DMO_INIT_DOF_MAP    (1 << 14) //
/**
 * 1=order the marked bits in dof_map
 */
#define DMO_ORDER_DOF_MAP   (1 << 15) //
/**
 * 1=dof_map changed since last Solve
 */
#define DMO_CHANGED_DOF_MAP (1 << 16) //
/**
 * 1=Rebuild Tag objects
 */
#define DMO_REBUILD_TAGS    (1 << 17) //
/**
 * (A pfunc param has changed)
 *  1=Pfunc X changed since last Rebuild_tag_objs()
 * 
 */
#define DMO_CHANGED_X       (1 << 18) // 
/**
 * 1=Set default shape (changed P)
 */
#define DMO_CHANGED_P_ON    (1 << 19) // 
/**
 * 1=Cleared default (changed P) since last Rebuild_tag_objs()
 */
#define DMO_CHANGED_P_OFF   (1 << 20) // 
/**
 * 1=Pfunc domain changed since last Rebuild_tag_objs()
 */
#define DMO_CHANGED_DOMAIN  (1 << 21) // 
/**
 * 1=changed shape approximately
 */
#define DMO_APPROX_SHAPE    (1 << 22) //
/**
 * 1=seam cstrns tracking a parent
 */
#define DMO_TRACKING        (1 << 23) //

/**
 * 1=add mass and damping effect
 * 0=no mass or damping effect
 */
#define DMO_DYN_TERMS       (1 << 24) //
                                      // 

/**
 * 1=copy pfunc->dof_vec to x,old_x
 * calls Update_load_pts(pfunc,0)
 * calls Update_cstrn_pts(pfunc,0)
 * 0=ready
 */
#define DMO_INIT_X          (1 << 25) // 
/**
 * 1=copy dmo_symeq->X to ->First_x
 */
#define DMO_FIRST_X         (1 << 26) // 
/**
 * tmp state bit used by DS_dmesh::Decouple_sibling
 */
#define DM_OLDER_SIBLING    (1 << 28) // 
/**
 * Make a list of the solver's change bits.
 * (other change bits are used elsewhere in the code)
 */
#define DMO_CHANGED_BITS ( DMO_CHANGED_A | DMO_CHANGED_MIN_MAP | \
                           DMO_CHANGED_B | DMO_CHANGED_DOF_MAP | \
                           DM0_CHANGED_C | DMO_CHANGED_X  | \
                           DMO_CHANGED_D | \
                           DMO_CHANGED_L | \
                           DMO_CHANGED_LC )

/**
 * seams are SPAposition continuous
 */
#define DMO_SEAM_C0    0  // 
/**
 * seams are SPAposition/tangent continuous
 */
#define DMO_SEAM_C1    1  // 
/**
 * seams are pos/tang/curvature continuous
 */
#define DMO_SEAM_C2    2  // 
//==============================================================
// Base class DS_dmod
//==============================================================
/**
 * Pointer to this class acts as an handle.
 * <br>
 * <b>Role:</b> Pointers to this SDM interface class <tt>DS_dmod</tt> should be considered as
 * handles - they should always be forward-referenced, and never dereferenced.
 * They are created, copied, and deleted by DM API's, and only used as handle (tag)
 * arguments to DM API's. For example, the functions <tt>DM_make_bspline_curve</tt> and
 * <tt>DM_make_bspline_surface</tt> will make a curve or surface <tt>DS_pfunc</tt> from arrays of knots
 * and control points. The resulting <tt>DS_pfunc</tt> is then used to make a <tt>DS_dmod</tt> with one of
 * the functions <tt>DM_make_dmod_curve</tt> or <tt>DM_make_dmod_surface</tt>; the resulting <tt>DS_dmod</tt> object
 * assumes ownership of the <tt>DS_pfunc</tt>. After completing the desired DM API calls, the user
 * must call <tt>DM_delete_dmod</tt> on the <tt>DS_dmod</tt> pointer.
 * @see DM_icon, DS_pfunc, DM_default_icon
 */


class DS_dmod : public ACIS_OBJECT             // identifier = dmo
{
  protected:
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   DM_icon* dmo_icon;
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   DS_DMO dmo_type_id ;    // derived class identifier
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   int dmo_tag ;           // unique id for each DS_dmod in a hierarchy
                           // root_tag == 2
                           // tag = 1 reserved for the active dmod

   // hierarchical dmod data - family ptrs, domain scale, and seam data
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   DS_dmod *dmo_parent ;   // ptr to parent dmod or NULL for root
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   DS_dmod *dmo_sibling ;  // ptr to next sibling or NULL for none
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   DS_dmod *dmo_child ;    // ptr to 1st child or NULL for none
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   DS_dmesh *dmo_dmesh ;   // ptr to dmesh complex or NULL for none

 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   int dmo_seam_count ;    // number of cstrns in seam
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   int dmo_seam_state ;    // specify patch connection continuity
                           // for root-sbilings: affects links
                           // for children     : affects seams
                           // oneof: DMO_SEAM_C0,
                           //        DMO_SEAM_C1,
                           //        DMO_SEAM_C2
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   DS_cstrn **dmo_seam ;   // array of ordered ptrs to seam cstrns
                           // seams stitch this dmod to its parent

 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   double dmo_domain_scale;// domain=dmo_domain_scale*parent_domain
                           // for all dmods:
                           // domain=Total_domain_scale()*solid_model_domain

   // hierarchy management data - used only by the root dmod
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   DS_dmod *dmo_active ;   // the hierarchy's active dmod
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   int dmo_tag_count ;     // last tag id value assigned to a
                           // tag obj in the dmod hierarchy.
                           // used to assign unique tag ids to all
                           // objects.

   // application data that stores 1-to-1 with DS_pfuncs
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   void *dmo_tmp_shape ;   // secretly an ACIS (surface * or curve *)
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   void *dmo_entity ;      // secretly an ACIS (FACE * or EDGE *)

   // graphic state, SPAparameter, and scratch values
   // data stored for each dmod - but used by applications
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   int dmo_draw_state ;    // bit array managed by calling application
                           // ACIS note: bit array defined in DMAPI.HXX
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   int dmo_mesh_u ;        // number of mesh points in u dir (crvs & srfs)
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   int dmo_mesh_v ;        // number of mesh points in v dir (srfs only)
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   int dmo_comb_pt_count;         // number of comb vecs shown in each elem
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   double dmo_comb_gain ;         // curv_pt = base_pt+C2_display_gain *curv_vec
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   double dmo_tan_display_gain ;  // tang_pt = base_pt+tan_display_gain*tang_vec

   // scratch for tmp_data rendering
#ifdef THREAD_SAFE_ACIS
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   LOCAL_VAR safe_integral_type<int> dmo_use_count ;     // number of active dmods
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   LOCAL_VAR safe_integral_type<int> dmo_gpt_size ;      // [array_size] size of pt_loc or pt_norm
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   LOCAL_VAR safe_pointer_type<double> dmo_gpt_loc  ;  // scratch for pt loc graphics
                                     // Sized:[dmo_gpt_size] [xyz0,xyz1,...xyzn]
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   LOCAL_VAR safe_pointer_type<double> dmo_gpt_norm ;  // scratch for pt norm vectors
                                     // Sized:[dmo_gpt_size] [dxyz0,dxyz1,..dxyzn]
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   LOCAL_VAR safe_pointer_type<double> dmo_gpt_binorm; // scratch for pt bi-norm vectors
                                     // Sized:[dmo_gpt_size] [dxyz0,dxyz1,..dxyzn]
#else
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   LOCAL_VAR int dmo_use_count ;     // number of active dmods
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   LOCAL_VAR int dmo_gpt_size ;      // [array_size] size of pt_loc or pt_norm
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   LOCAL_VAR double *dmo_gpt_loc  ;  // scratch for pt loc graphics
                                     // Sized:[dmo_gpt_size] [xyz0,xyz1,...xyzn]
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   LOCAL_VAR double *dmo_gpt_norm ;  // scratch for pt norm vectors
                                     // Sized:[dmo_gpt_size] [dxyz0,dxyz1,..dxyzn]
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   LOCAL_VAR double *dmo_gpt_binorm; // scratch for pt bi-norm vectors
                                     // Sized:[dmo_gpt_size] [dxyz0,dxyz1,..dxyzn]
#endif

   // dimension independent material properties
   //   note: dimension dependent alpha and beta are in derived classes
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   double  dmo_gamma ;     // "gamma": resist rate of change of
                           //          bending - to smooth out
                           //          curvature constraints.
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   double  dmo_delta ;     // "delta": resist displacement
                           //          from default shape
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   double dmo_epsilon ;

   // integration parameters (used in the construction of A)
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   double  dmo_dt ;        // time step
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   double  dmo_mass ;      // effective mass for each dof
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   double  dmo_damp ;      // stabilizing damping for each dof

   // shape model description
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   DS_pfunc *
           dmo_pfunc ;     // represents shape W = Sum (DOFi * Phi)

   // Ax = b, Cx = d optimization problem description
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   int     dmo_abcd_state; // bit array keeping track of which
                           // input arrays have changed between
                           // calls to Solve().
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   double dmo_last_dist2 ; // used to determine convergence
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
#ifndef DS_USE_SYMEQ
   DS_lueqns*               // represents Ax=b problem
           dmo_eqns ;
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   DS_lu_solver dmo_lusolver;
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   DS_bridge
           dmo_bridge ;
#else /*DS_USE_SYMEQ*/
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   DS_symeq*
           dmo_eqns ;
#endif /*DS_USE_SYMEQ*/
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   DS_diag_mat             // scratch row-diagonal matrices to hold
           dmo_K ;         // one elem's stiffness and mass matrices
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   DS_diag_mat             // sized_by:[dmo_pfunc->Elem_dof_count()]
           dmo_mc ;


   // User interaction data - identify these by tag, pos, or type
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   int     dmo_load_count; // number of loads applied to surface
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   DS_load*                // ptr to the first load on the load list
           dmo_load ;      // description and data of each load
                           // stored as a linked list

 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   int     dmo_cstrn_count;// number of def_mod constraints
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   DS_cstrn*               // ptr to the 1st cstrn on the cstrn list
           dmo_cstrn ;     // description and data of each cstrn
                           // stored as a linked list

   // data for DS_dmesh
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   int dmo_sibling_index ; // temp index for DS_dmesh::Decouple_siblings
 /*
 // tbrv
 */
 /**
  * @nodoc
  */
   int dmo_mesh_dof_offset;// Location of dmod's 1st dof in dmesh
                           // global dof array.  Its the link_dof_count
                           // sum of all dmod's in front of this
                           // one on the DS_dmesh dmod list.
                           // only the link_dofs go into the joint problem

  public:

   // constructors, copy, Size_arrays, destructor
 /**
  * @nodoc
  */
   DECL_DM DS_dmod                 // DS_dmod default constructor
    (DS_DMO type_id,       // in : derived class identifier
     int tag=2,            // in : unique obj id (2=root,1=active dmod)
     int draw_state=0,     // in : draw bits for application graphics
     DS_pfunc *pfunc=NULL, // in : defines the shape model
     double gamma  = 0.0,  // in : resist bending rate of change
     double delta  = 0.0,  // in : resist moving from default shape
     double dt     = 1.0,  // in : implicit integration time step
     double mass   = 0.0,  // in : physical param (causes rippling)
     double damp   = 0.0); // in : physical param (stable intgrtn)

 /**
  * @nodoc
  */
   virtual                 // bas: DS_dmod pure virtual function
    DS_dmod *              // rtn: the newly created child dmod or NULL
     Add_patch             // eff: add a patch to the 'this' dmod
     (void *patch_entity,  // in : app entity ptr stored with patch
      double *min,         // in : lower domain point for patch
      double *max,         // in : upper domain point for patch
      int seam_count,      // in : number of pfuncs in the seam
      DS_pfunc **seam,     // in : array of pfuncs that define the seam
      void **seam_data,    // in : app entity data stored with each seam
      int refinement,      // in : parent_knot_spacing/child_knot_spacing
      int &tag,            // i/o: start and final available tag ids
      int draw_state)=0 ;  // in : draw bits for application graphics

/* // moved to dmapi.cxx
   virtual void            // bas: class DS_dmod pure virtual function
    Build_square_seam      // eff: build a set of curves to act as a seam
     (double *min,         // in : patch lower domain point,sized:[domain_dim]
      double *max,         // in : patch upper domain point,sized:[domain_dim]
      int &pfunc_count,    // out: number of pfuncs in pfunc_array
      DS_pfunc **&pfunc)=0;// out: newly created pfunc array (NULL on input)
                           // mallocs: pfunc array and each pfunc entry

   virtual void            // use: for surface dmods only
    Build_ellipse_seam     // eff: build a elliptical seam
     (double *cpt,         // in : center point for patch    , sized:[2]
      double *axis1,       // in : SPAvector from cpt to 1st-axis end point
      double *axis2,       // in : SPAvector from cpt to 2nd-axis end point
      double *min,         // out: lower domain ellipse bound, sized:[2]
      double *max,         // out: upper domain ellipse bound, sized:[2]
      int &pfunc_count,    // out: always set to 1
      DS_pfunc **&pfunc)=0;// out:newly created pfunc array (NULL on input)
                           // mallocs: pfunc array and each pfunc entry
*/ // end moved to dmapi.cxx
/**
 * @nodoc
 */

   DS_dmod                 // eff: copy constructor
    (DS_dmod &dmod,        // in : object to copy
     int walk_flag=0) ;    // in : specify how deep to go
                           //      0='this' only, 1='this' and offspring
/**
 * @nodoc
 */

   DS_dmod &operator=      // eff: assignment operator
    (DS_dmod &dmod) ;      // in : the object from which to copy values
                           // note: assignment drops hierarchy links.
                           //       use Make_copy to copy hierarchy
                           //       sub_trees.

/**
 * @nodoc
 */
   virtual DS_dmod*        // Virtual Make_copy
     Make_copy             // eff: virtual copy function
      (int walk_flag=0)=0; // in : specify how deep to go
                           //      0='this' only, 1='this' and offspring

/**
 * @nodoc
 */
    void                    // Size_arrays()
    Size_arrays            // eff: internal memory management
    (int seam_count=0) ;   // in : expected number of cstrns in seam
/**
 * @nodoc
 */
   virtual                 // Virtual destructor
   ~DS_dmod() ;            // eff: free 'this' and all offspring data

/**
 * @nodoc
 */
   DS_DMO Type_id()        // Run time type checking
   { return dmo_type_id ; }

 // Graphics interface
/**
 * @nodoc
 */
   virtual void Set_icon_owner(int tag);
/**
 * @nodoc
 */
   virtual void Notify_icon_tag_obj_change();
/**
 * @nodoc
 */
   virtual void Draw(const DM_icon_draw_args& options)const;
/**
 * @nodoc
 */
   virtual void Setstate(const DM_icon_cmd_args& args);
/**
 * @nodoc
 */
   virtual void Query(DM_icon_query_args& args) const;
/**
 * @nodoc
 */
   virtual DM_icon* Get_icon()const {return dmo_icon;}
/**
 * @nodoc
 */
   virtual void Set_icon(DM_icon* ic) ;

/**
 * @nodoc
 */
   int Is_tracking_dmod()  // rtn: 1=yes,0=no
                           { return(  (   dmo_type_id == ds_tpc
                                      || dmo_type_id == ds_ttc
                                      || dmo_type_id == ds_tcc)
                                    ? 1 : 0) ;
                           }
/**
 * @nodoc
 */
   void Set_type_id        // set: the stored type_id
    (DS_DMO type_id)       { dmo_type_id = type_id ; }

/**
 * @nodoc
 */
   void Print              // eff: output a written report
    (FILE *file,           // in : FILE stream to receive report
     int walk_flag=0) ;    // in : specify how deep to go
                           //      0='this' only, 1='this' and offspring
                           // note: due to toolkit linking conflicts
                           //       this is not for use with ACIS
                           //       integration.

/**
 * @nodoc
 */
   int Tag()               // rtn: tag id for 'this' dmod
   { return dmo_tag ; }
/**
 * @nodoc
 */
 void Set_tag(int tag)   // eff: set DS_dmod tag value
                           { dmo_tag = tag ;
                             Set_icon_owner(tag);
                           }

/**
 * @nodoc
 */
 void Reassign_tags      // eff: give all tag objects a new tag value
    (int &tag,             // i/o: starting and next available tag number
     int walk_flag=0) ;    // in : specify how deep to go
                           //      0='this' only, 1='this' and offspring

/**
 * @nodoc
 */
  void Increment_tags      // eff: inc all tag object tag values
   (int tag_shift,         // in : amount to shift each tag value
    int walk_flag) ;       // in : specify how deep to go
                           //      0='this' only,
                           //      1='this' and offspring
                           //      2='this',siblings,and offspring
// bmt - provisional DECL until DM_get_max_tag is working
/**
 * @nodoc
 */
 DECL_DM int                     // rtn: max tag value used in dmod
    Max_tag_value          // eff: give all tag objects a new tag value
     (int walk_flag) ;     // in : specify how deep to go
                           //      0='this' only, 1='this' and offspring
                           //      2=this,sibling, and offspring

   // DS_dmod iterator
/**
 * @nodoc
 */
 DS_dmod *Next           // rtn: sequence of dmods for recursion or NULL
    (int walk_flag,        // in : specify how deep to go
                           //      0='this' only, 1='this' and offspring
                           //      2=this,sibling, and offspring
 	DS_cstrn *&next_cstrn,// i/o: cstrn to start src_W_dmod search
     int &dmod_flag) ;     // i/o: dmod to start child,sibling search
                           //      1=rtn sibling next if appropriate
                           //      2=rtn child next if appropriate
                           //      0=all done (rtn NULL on next call)

   // DS_dmod parent/child hierarchy access and management function
/**
 * @nodoc
 */
 	DS_dmod *Parent()       // rtn: ptr to parent dmod
                           { return dmo_parent ; }

/**
 * @nodoc
 */
 	DS_dmod *Base_parent()  // rtn: ptr to oldest ancestor of this
                           //      dmod.  Rtns this when this has
                           //      no parent.
                           { if(dmo_parent==NULL) return(this) ;
                             else
                              return(dmo_parent->Base_parent()) ;
                           }
/**
 * @nodoc
 */

   DS_dmod *Sibling()      // rtn: ptr to next younger sibling dmod
   { return dmo_sibling ; }

/**
 * @nodoc
 */
 	int Is_first_sibling()  // rtn: 1=yes, 0=no
                           { return(First_sibling() == this
                                    ? 1 : 0 ) ;
                           }
/**
 * @nodoc
 */
 	int Is_last_sibling()   // rtn: 1=yes, 0=no
                           { return(Last_sibling() == this
                                    ? 1 : 0 ) ;
                           }

/**
 * @nodoc
 */
 	int Is_sibling          // rtn: 1=yes, 0=no
     (DS_dmod *dmod)       // in : dmod to look for on sibling list
                           { if(dmod == this) return 1 ;
                             // else blocks required for hp700_9
                             else{
                             if(dmo_sibling == NULL) return 0 ;
                             else{
                             return(dmo_sibling->Is_sibling(dmod)) ;
                             }}
                           }
/**
 * @nodoc
 */
   DS_dmod *First_sibling()// rtn: ptr to first sibling dmod
                           { return(  (   dmo_parent
                                       && dmo_type_id
                                       == dmo_parent->dmo_type_id)
                                    ? dmo_parent->dmo_child
                                    : dmo_dmesh
                                    ? dmo_dmesh->Dmod()
                                    : this) ;
                           }
/**
 * @nodoc
 */
   DS_dmod *Last_sibling() // rtn: ptr to youngest sibling dmod
                           { DS_dmod *sib = this ;
                             for(;sib->Sibling()!=NULL;
                                 sib=sib->Sibling()) ;
                             return(sib) ;
                           }
/**
 * @nodoc
 */
   int Sibling_index()     { return dmo_sibling_index ; }
/**
 * @nodoc
 */
 	void Set_sibling_index  // eff: set dmo_sibling_index value
     (int sibling_index)   // in : to this value
                           { DS_assert(sibling_index >= 0) ;
                             dmo_sibling_index = sibling_index ;
                           }
/**
 * @nodoc
 */
   DS_dmod *Child()        // rtn: ptr to 1st child dmod
   { return dmo_child ; }
/**
 * @nodoc
 */
   DS_dmesh *Dmesh()       // rtn: prt to dmesh complex ptr
   { return dmo_dmesh ; }
/**
 * @nodoc
 */

   DS_dmod *Root() ;       // rtn: ptr to hierarchy's root DS_dmod
/**
 * @nodoc
 */
   DS_dmod *Active()       // rtn: active dmod ptr stored in root dmod
                           { return( Root()->dmo_active ) ;
                           }
/**
 * @nodoc
 */
   void Set_parent         // eff: change the parent ptr value
    (DS_dmod *parent)      // in : ptr to the new parent value
    { dmo_parent = parent ; }

/**
 * @nodoc
 */
   void Set_sibling        // eff: change the sibling ptr value
    (DS_dmod *sibling)     // in : ptr to the new sibling value
    { dmo_sibling = sibling ; }

/**
 * @nodoc
 */
   void Set_child          // eff: change the child ptr value
    (DS_dmod *child)       // in : ptr to the new child value
    { dmo_child = child ; }
/**
 * @nodoc
 */
   void Set_dmesh          // eff: change root ptrs to dmesh
    (DS_dmesh *dmesh)      { dmo_dmesh = dmesh ;
                             if(dmo_sibling)
                               dmo_sibling->Set_dmesh(dmesh) ;
                           }
/**
 * @nodoc
 */
   void Make_active()      // eff: make this the root's active dmod
   { Root()->dmo_active = this ; }
/**
 * @nodoc
 */
   DS_dmod *               // rtn: DS_dmod ptr or NULL when not found
    Find_offspring_by_tag  // eff: search dmod hierarchy for tag object
     (int tag) ;           // in : target child's unique identifier
/**
 * @nodoc
 */
   DS_dmod *               // rtn: DS_dmod ptr or NULL when not found
    Find_offspring_by_entity//eff: search dmod hierarchy for entity value
     (void *entity) ;      // in : target child's unique entity value
/**
 * @nodoc
 */
   int                     // rtn: 0=success,-1=dmod not on child list
    Rm_child               // eff: remove and delete dmod from child list
     (DS_dmod *dmod) ;     // in : dmod to remove from child list
                           // note: Add_patch() makes hierarchy children
                           // note: deletes all dmod offspring
/**
 * @nodoc
 */
   DS_dmod *               // rtn: ptr to dmod removed from sibling list
    Rm_sibling             // eff: remove dmod's sibling from sibling list
     ()                    // note: Add_patch() makes hierarchy children
                           // note: does no deletes
                           { // get a pointer to the sibling
                             DS_dmod *rtn_dmod = dmo_sibling ;

                             // remove the sibling list's reference to the sibling
                             if(dmo_sibling) dmo_sibling = dmo_sibling->dmo_sibling ;

                             // remove the sibling's reference to the sibling list
                             rtn_dmod->dmo_sibling = NULL ;

                             // all done
                             return (rtn_dmod) ;
                           } // end Rm_sibling
/**
 * @nodoc
 */
   int                     // rtn: number of siblings counting this one
    Sibling_count()        // eff: walk sibling ptrs and rtn the count
                           { int count = 0 ;
                             DS_dmod *sib = this ;
                             for(;sib!=NULL;sib=sib->dmo_sibling) count++ ;
                             return(count) ;
                           } // Sibling_count
/**
 * @nodoc
 */
   int                     // rtn: 1=yes,0=no
    Is_dmod_in_hierarchy   // eff: recursively search hierarchy for input
     (DS_dmod *dmod) ;     // in : the dmod to search for
/**
 * @nodoc
 */
   int                     // rtn: 1=yes,0=no
    Is_dmod_a_child        // eff: search child's sibling list for input
     (DS_dmod *dmod) ;     // in : the dmod to search for

                           // use: Called by Add_patch()
/**
 * @nodoc
 */
   int                     // rtn: 0=success,-1=already a member (no action)
    Add_child              // eff: add a dmod child to this dmod
    (DS_dmod *dmod) ;      // in : dmod to add to child list
                           // note: members can not be made into children
                           // note: dmod->Parent() is set equal to 'this'

/**
 * @nodoc
 */
   double                  // rtn: domain_scale of
     Domain_scale()        //      domain=domain_scale*parent_domain
                           { return(dmo_domain_scale) ;
                           }
/**
 * @nodoc
 */
   void                    // eff: set domain_scale of
     Set_domain_scale      //      domain=domain_scale*parent_domain
      (double domain_scale)// in : domain value (not 0.0)
                           //note: In general use Scale_domain()
                           { if(DS_pos_not_zero(domain_scale,
                                                DS_DOUBLE_CLOSE))
                               dmo_domain_scale = domain_scale ;
                           }
/**
 * @nodoc
 */
   double                  // rtn: scale in domain=scale*solid_domain
     Total_domain_scale()  // eff: concatenate all ancestor scales
                           //      stopping at point where a tracking
                           //      curve connects to its constrained surface
                           { if(   dmo_parent==NULL
                                || dmo_type_id != dmo_parent->dmo_type_id)
                               return(dmo_domain_scale) ;
                             else
                               return
                              (  dmo_domain_scale
                               * dmo_parent->Total_domain_scale()) ;
                           }
/**
 * @nodoc
 */
   double *                // rtn: ptr to output
    Scale_dpt_to_parent    // eff: scale dpt from child to parent space
     (double *child_dpt,   // in : this dpt, sized:[domain_dim]
      double *parent_dpt); // out: parent_dpt = this_dpt/dmo_scale_domain ;
/**
 * @nodoc
 */
   double *                // rtn: ptr to output
    Scale_dpt_from_parent  // eff: scale dpt from parent to child space
     (double *child_dpt,   // out: this dpt, sized:[domain_dim]
      double *parent_dpt); // in : this_dpt = parent_dpt*dmo_scale_domain ;

   // constraint state access/management functions
/**
 * @nodoc
 */
 int Mesh_dof_offset()   { return dmo_mesh_dof_offset ; }
/**
 * @nodoc
 */
   void Set_mesh_dof_offset// eff: set the 1st dof global dof number
     (int mesh_dof_offset) // in : new value for mesh dof offset
                           { dmo_mesh_dof_offset =
                                         mesh_dof_offset ;
                           }

   // seam access/management functions
// bmt lib project - provisional DECL
/**
 * @nodoc
 */
 DECL_DM int Sheet_edge_count(); // rtn: number of ds_bound_cstrn crv_cstrns
/**
 * @nodoc
 */
 DECL_DM int Solid_edge_count(); // rtn: number of ds_solid_cstrn crv_cstrns
/**
 * @nodoc
 */
 DECL_DM int Link_count() ;      // rtn: number of ds_link_cstrn cstrns
/**
 * @nodoc
 */
   int Seam_count()        // rtn: number of cstrns in seam
   { return dmo_seam_count ; }
/**
 * @nodoc
 */
   int Seam_state()        // rtn: continuity degree of patch to parent
   { return dmo_seam_state ; }
/**
 * @nodoc
 */
 void Set_seam_state     // eff: set continuity degree between patch and parent
    (int seam_state)       // in : oneof DMO_SEAM_C0, DMO_SEAM_C1,
                           //            DMO_SEAM_C2
                           {
                             DS_assert(   seam_state == DMO_SEAM_C0
                                       || seam_state == DMO_SEAM_C1
                                       || seam_state == DMO_SEAM_C2) ;
                             dmo_seam_state = seam_state ;
                           }

/**
 * @nodoc
 */
 DS_cstrn **Seam()       { return dmo_seam ; }
/**
 * @nodoc
 */
   DS_cstrn  *Seam(int ii) // rtn: iith member of the seam list
                           { DS_assert(ii >= 0 && ii < dmo_seam_count) ;
                             return(dmo_seam[ii]) ;
                           }

/**
 * @nodoc
 */
 void Add_to_seam_list   // eff: place crv_cstrn on end of seam list
     (DS_crv_cstrn *cstrn) // in : crv_cstrn ptr to add to seam list
                           {
                             Size_arrays(dmo_seam_count+1) ;
                             dmo_seam[dmo_seam_count-1] = cstrn ;

                           }
// bmt - lib proj - provisional DECL
/**
 * @nodoc
 */
 DECL_DM void Refresh_seam_data () ; // eff: recalc and store seam_count and seam

   // access to application data that stores 1-to-1 with dmods
/**
 * @nodoc
 */

   void *Tmp_shape()       // rtn: ptr to Tmp shape (surface * or curve *)
   { return dmo_tmp_shape ; }

/**
 * @nodoc
 */
 void Set_tmp_shape      // set: dmo_tmp_shape pointer value
     (void * tmp_shape)    // in : ptr value to save for application
     { dmo_tmp_shape = tmp_shape ; }


/**
 * @nodoc
 */
 void *Entity()          // rtn: ptr to Surf_mesh (INDEXED_MESH *)
      { return dmo_entity ; }

/**
 * @nodoc
 */
 void Set_entity         // set: dmo_entity pointer value
     (void *entity) ;        // in : ptr value to save for application

   // graphic state, SPAparameter, and scratch values
/**
 * @nodoc
 */
 int Draw_state()        // rtn: draw_state bit array
     { return dmo_draw_state ; }

/**
 * @nodoc
 */
    int                     // rtn: 0=no changes made,1=changes were made
    Set_draw_state         // set: draw_state bit array for app graphics
     (int draw_state,      // in : new draw_state value to save
      int walk_flag=0) ;   // in : specify how deep to go
                           //      0='this' only, 1='this' and offspring

/**
 * @nodoc
 */
   int Mesh_u()            { return dmo_mesh_u ; }
/**
 * @nodoc
 */
   int Mesh_v()            { return dmo_mesh_v ; }

/**
 * @nodoc
 */
   void Get_mesh_count     // get: number of pts drawn in u and v dirs
    (int &mesh_u,          // out: number of pts in u dir (crvs & srfs)
     int &mesh_v)          // out: number of pts in v dir (crvs only)
   { mesh_u = dmo_mesh_u ; mesh_v = dmo_mesh_v ; }

/**
 * @nodoc
 */
    int                     // rtn: 0=no changes made,1=changes were made
    Set_mesh_count         // set: number of pts drawn in u and v dirs
    (int mesh_u,           // in : number of pts in u dir (crvs & srfs)
     int mesh_v,           // in : number of pts in v dir (crvs only)
     int walk_flag=0) ;    // in : specify how deep to go
                           //      0='this' only, 1='this' and offspring

/**
 * @nodoc
 */
   int Draw_count          // rtn: number dmod graphics currently being drawn
    (int dof_draw_bit,     // in : bit for drawing dofs
     int gpt_draw_bit,     // in : bit for drawing gpts
     int cstrn_comb_bit,   // in : bit for drawing curve cstrn curv_combs
     int seg_bnds_bit,     // in : bit for drawing curve cstrn seb_bnds
     int curv_comb_bit,    // in : bit for drawing curvature_combs
     int torq_comb_bit,    // in : bit for drawing torsion_combs
     int elem_draw_bit,    // in : bit for drawing dmod elem boundaries
     int &shape_count,     // i/o: surf_mesh count in patch hierarchy
     int walk_flag=0) ;    // in : specify how deep to go
                           //      0='this' only, 1="this" and offspring

/**
 * @nodoc
 */
 int Gpt_size()          // rtn: size of memory in pt_loc or pt_norm
 { return dmo_gpt_size ; }

/**
 * @nodoc
 */
 int Comb_pt_count()     // rtn: number of tines per elem in a comb plot
 { return dmo_comb_pt_count ; }

/**
 * @nodoc
 */
   double Comb_gain()      // rtn: gain on comb vec magnitudes in comb plot
   { return dmo_comb_gain ; }

/**
 * @nodoc
 */
 int                     // rtn: 0=no changes made,1=changes were made
    Set_comb_graphics      // set: comb_plot tine number_per_elem and length
     (int comb_pt_count,   // in : new comb_pt_count value
      double comb_gain,    // in : new comb_gain value
      int walk_flag=0) ;   // in : specify how deep to go
                           //      0='this' only, 1='this' and offspring

/**
 * @nodoc
 */
 double *Gpt_loc()       // rtn: ptr to gpt_loc array
                           //      Sized:[dmo_gpt_size] [xyz0,xyz1,..xyzn]
	{ return dmo_gpt_loc ; }

/**
 * @nodoc
 */
 double *Gpt_norm()      // rtn: ptr to gpt_norm array
                           //      Sized:[dmo_gpt_size] [dxyz0,..dxyzn]
        { return dmo_gpt_norm ; }

/**
 * @nodoc
 */
 double *Gpt_binorm()    // rtn: ptr to gpt_binorm array
                           //      Sized:[dmo_gpt_size] [dxyz0,..dxyzn]
       { return dmo_gpt_binorm ; }

/**
 * @nodoc
 */
   double Tan_display_gain()  // rtn: graphical gain for tang vecs
   { return dmo_tan_display_gain ; }

/**
 * @nodoc
 */
 double C2_display_gain()  // rtn: graphical gain for curv vecs
 { return dmo_comb_gain ; }

/**
 * @nodoc
 */
 int    C2_point_count() // rtn: number of vecs in curvature comb
 { return dmo_comb_pt_count ; }

/**
 * @nodoc
 */
 void
    Set_tan_display_gain   // set: graphic scaling for all tang_vec cstrns
    (double tan_display_gain, // in : graphic scaling for tang_vec cstrns
     int walk_flag=0) ;    // in : specify how deep to go
                           //      0='this' only, 1='this' and offspring

   // DS_dmod dimension independent material properties
/**
 * @nodoc
 */
 virtual int Get_alpha   // rtn: 0=success, -1=wrong domain_dim
    (int domain_dim,       // in : 1=1d curves, 2=2d surfaces
     double *alpha)=0 ;    // out: 1d = [alpha],         sized:1
                           //    : 2d = [au, av, theta], sized:3

/**
 * @nodoc
 */
 virtual int Set_alpha   // rtn: 0=success, -1=wrong domain_dim
    (int domain_dim,       // in : 1=1d curves, 2=2d surfaces
     double *alpha,        // in : 1d = [alpha],         sized:1
                           //    : 2d = [au, av, theta], sized:3
	 int walk_flag=0)=0 ;  // in : specify how deep to go
                           //      0='this' only, 1='this' and offspring

/**
 * @nodoc
 */
 virtual int Get_beta    // rtn: 0=success, -1=wrong domain_dim
    (int domain_dim,       // in : 1=1d curves, 2=2d surfaces
     double *beta)=0 ;     // out: 1d = [beta],          sized:1
                           //    : 2d = [bu, bv, theta], sized:3

/**
 * @nodoc
 */
 virtual double Get_epsilon()const;

/**
 * @nodoc
 */
 virtual void Set_epsilon(double);

/**
 * @nodoc
 */
 virtual int Set_beta    // rtn: 0=success, -1=wrong domain_dim
    (int domain_dim,       // in : 1=1d curves, 2=2d surfaces
     double *beta,         // in : 1d = [beta],          sized:1
                           //    : 2d = [bu, bv, theta], sized:3
     int walk_flag=0)=0 ;  // in : specify how deep to go
                           //      0='this' only, 1='this' and offspring

/**
 * @nodoc
 */
 double Gamma()          // rtn: 'resist bending rate of change' val
 { return dmo_gamma ; }
/**
 * @nodoc
 */
 void Get_gamma(double &gamma)        //// get: 'resist bending rate of change' val out: gamma value
 	{ gamma = dmo_gamma ; }
/**
 * @nodoc
 */
 void Set_gamma          // set: 'resist bending rate of change' val
    (double gamma,         // in : new value to use
     int walk_flag=0) ;    // in : specify how deep to go
                           //      0='this' only, 1='this' and offspring

/**
 * @nodoc
 */
 double Delta()          // rtn: current 'resist displacement' value
        { return dmo_delta ; }
/**
 * @nodoc
 */
 void Get_delta          // get: current 'resist displacement' value
    (double &delta)        // out: delta value
    { delta = dmo_delta ; }
/**
 * @nodoc
 */
 void Set_delta          // set: 'resist displacement' value
    (double delta,         // in : new value to use
     int walk_flag=0) ;    // in : specify how deep to go
                           //      0='this' only, 1='this' and offspring

   // DS_dmod integration parameters
/**
 * @nodoc
 */
 double Dt()             // rtn: dynamic time step size
        { return dmo_dt ; }
/**
 * @nodoc
 */
 double Mass()           // rtn: dynamic 'mass' value
       { return dmo_mass ; }
/**
 * @nodoc
 */
  double Damp()           // rtn: dynamic 'damping' value
         { return dmo_damp ; }
/**
 * @nodoc
 */
 void Get_dynamics       // get: dyanmic time, mass, and damping values
    (double &dt, double &mass, double &damp)
                           { dt   = dmo_dt ;
                             mass = dmo_mass ;
                             damp = dmo_damp ;
                           }
/**
 * @nodoc
 */
 void Set_dynamics       // set: dynamic time, mass, and damping values
    (double dt,            // in : new integration time step value
     double mass,          // in : new dynamic mass value
     double damp,          // in : new dynamic damping value
     int walk_flag=0) ;    // in : specify how deep to go
                           //      0='this' only, 1='this' and offspring
/**
 * @nodoc
 */
   int Integral_degree() ; // rtn: gauss integration accuracy

   // DS_dmod shape model description and editing
/**
 * @nodoc
 */
   DS_pfunc* Pfunc()       // rtn: ptr to the dmod's shape definition
            { return dmo_pfunc ; }

/**
 * @nodoc
 */
 int Dof_count            // rtn: degree of freedom count for dmod
   (int walk_flag=0) ;     // in : specify how deep to go
                           //      0='this' only, 1="this" and offspring
/**
 * @nodoc
 */
  int Dmesh_dof_count() ;  // rtn: dof count in all root-siblings
                           // ass: the call is started from head of
                           //      root sibling list
/**
 * @nodoc
 */
  double Min_span_length   // rtn: shortest elem span in dmod's pfunc
   (int walk_flag=0) ;     // in : specify how deep to go
                           //      0=this,1= +offspring,2= +siblings
/**
 * @nodoc
 */
  void                     // eff: Splits pfunc's domain description
   Split                   //      making more dofs for more deform
                           //      ability but longer solution times.
    (int dpt_count,        // in : number of places to split domain
     double *dpt,          // in : locations at which to split domain
     int split_flag=1,     // in : 1= split exactly at split_pt pos
                           //      0= split elem containing dpt in half
                           //      0 increases numerical stability
     int walk_flag=1) ;    // in : 1= also split the children
                           // warning: sequences of splits with
                           //      split_flag == 0 may have
                           //      surprising results.
                           // note: children should always be split

/* // TODO:??? no yet implemented
   int                     // rtn: 1=success, 0=no change
    Set_elem_count         // set: tensor-product pfunc elem count
     (int *elem_count,     // in : array of desired elem_counts
      int elem_count_size, // in : elem_count array size (equal to domain_dim)
      int fit_flag) ;      // in : specifies tol requirement to satisfy
                           //      0 = shapes match to SPAresabs everywhere
                           //      1 = shapes match to SPAresabs at cstrns
                           //      2 = shapes match to best tol possible
*/
/**
 * @nodoc
 */
  void
  Set_degree      // eff: Change polynomial degree.
    (int& rtn_flag,        // in/out: 0=success, -3=not a root dmod, -1=deg<3
     int* new_deg);        // in : new degree(s)

/**
 * @nodoc
 */
   void Elevate_degree     // eff: Increase shape polynomial degree.
     (int walk_flag=1,     // in : 1=also elevate the children
      int cont_flag=0) ;   // in : 0 = preserve current elem continuity
                           //      1 = increment elem continuity
                           // note: preserving continuity allows the
                           //       new shape to exactly form the old
                           // note: Sets field pfn_domain_edit.
                           // note: children should always be elevated
/**
 * @nodoc
 */
   int                     // rtn: 0=success, 1=dmod not modified
    Extrapolate            // eff: Extrapolate the pfunc a small bit.
     () ;                  //      Requires system to be rebuilt but
                           //      causes no changes in tag objects.
                           // note: increases the size of a parent
                           //       to contain an increased child
                           // note: dmods within multi-surface meshes
                           //       will not be extrapolated.
/**
 * @nodoc
 */
   void Shift_domain       // eff: change the domain range
     (double *du,          // in : domain shift SPAvector, Sized:Domain_dim()
      int walk_flag=1) ;   // in : 1=shift the offspring
/**
 * @nodoc
 */
   int                     // rtn: 0=done,-1=scale too small
   Scale_domain            // eff: change the domain range
     (double scale,        // in : domain scale factor
      int walk_flag=1) ;   // in : 0=this,1= +offspring,2= +siblings
/**
 * @nodoc
 */
  int                      // rtn: 1=found an xsect, 0=didn't
   Iline_xsect             // eff: get 1st pfunc/iline xsect
                           //      iline = p0 + u(p1-p0)
    (double *p0,           // in : image space pt, Sized:[image_dim]
     double *p1,           // in : image space pt, Sized:[image_dim]
     double max_dist,      // in : max dist for near misses
     double *image_pt,     // out: image-loc for nearest xsect
                           //      sized:[image_dim]
     double &iline_u,      // out: iline param val of xsect pt
     double *pfunc_uv,     // out: pfunc param val of xsect pt
                           //      sized:[domain_dim]
     DS_dmod *&tgt_dmod,   // out: ptr to dmod containing closest xsect
     int walk_flag=0) ;    // in : specify how deep to search
                           //      0= only search 'this'
                           //      1= also search offspring
                           //      2= also search siblings and offspring

  // DS_dmod end condition code
/**
 * @nodoc
 */
  virtual void             // bas: DS_dmod virtual function
   Get_end_conds           // eff: get end-states for d2a-dsurf
    (int &end_cond_u,      // in : oneof 0=open|1=closed|2=periodic
     int &singular_u,      // in : oneof 0=none|1=low|2=high|3=both
     int &end_cond_v,      // in : oneof 0=open|1=closed|2=periodic
     int &singular_v) ;    // in : oneof 0=none|1=low|2=high|3=both
/**
 * @nodoc
 */
  void
   Ch_end_conds            // eff: changes end-states for d2a_dmod
    (int end_cond_u,       // in : oneof 0=open|1=closed|2=periodic
     int singular_u=0,     // in : oneof 0=none|1=low|2=high|3=both
     int end_cond_v=0,     // in : oneof 0=open|1=closed|2=periodic
     int singular_v=0) ;   // in : oneof 0=none|1=low|2=high|3=both
                           // note: only roots may be closed or periodic

   // DS_dmod Ax = b optimization problem description
#ifndef DS_USE_SYMEQ
//   DS_cgeqns * Symeq()          // rtn: ptr to symetric equation set
//                                // SHOULDN'T USE THIS!!!!
//                                // Replaced by access functions below.
//                              { return dmo_eqns ; }
// The next for functions are used only by the api function DM_get_dof_state

/**
 * @nodoc
 */
   int                        // rtn: dmo_symeq LLc_row_count, or 0 if no symeq
                              //      for cg_eqns
   LLc_row_count              // eff:
    ();                       // in :
/**
 * @nodoc
 */
   int                        // rtn: dmo_symeq Fixed_count, or 0 if no symeq
                              //      for cg_eqns
   Fixed_count               // eff:
    ();                       // in :
 /**
  * @nodoc
  */
   int                        // rtn: dmo_symeq Free_count, or 0 if no symeq
                              //      for cg_eqns
   Free_count               // eff:
    ();                       // in :
/**
 * @nodoc
 */
   int                        // rtn: dmo_symeq Free_mix_count, or 0 if no symeq
                              //      for cg_eqns
   Free_mix_count               // eff:
    ();                       // in :
/**
 * @nodoc
 */
   int                        // rtn: dmo_symeq Cd_row_count, or 0 if no symeq
                              //      for cg_eqns, this is the point constraint count!
   Cd_row_count               // eff:
    ();                       // in :
/**
 * @nodoc
 */
   int                        // rtn: dmo_symeq Image_dim, or 0 if no symeq
   Image_dim                  // eff:
    ();                       // in :
   // Provisional access functions for the dmesh build
/**
 * @nodoc
 */
 double A(int nrow, int ncol);
/**
 * @nodoc
 */
 double B(int nrow, int ncol);
/**
 * @nodoc
 */
 double C(int nrow, int ncol);
/**
 * @nodoc
 */
 double D(int nrow, int ncol);
/**
 * @nodoc
 */
 double X(int nrow, int ncol);
/**
 * @nodoc
 */
 double L(int nrow, int ncol, int nx);
/**
 * @nodoc
 */
 double Lc(int row);
/**
 * @nodoc
 */
 void Copy_x_to_my_bridge();
/**
 * @nodoc
 */
 void Copy_x_to_bridge(DS_bridge* brdg, int row_offset=0);
/**
 * @nodoc
 */
 void Copy_x_from_bridge(DS_bridge* brdg, int row_offset=0);
/**
 * @nodoc
 */
 void Copy_bridge_x_to_old_bridge_x();
#else /*DS_USE_SYMEQ*/
/**
 * @nodoc
 */
 DS_symeq * Symeq()          // rtn: ptr to symetric equation set
         { return dmo_eqns ; }
#endif /*DS_USE_SYMEQ*/
/**
 * @nodoc
 */
 int *Min_Cd_map() ;        // rtn: ptr to symeq min_Cd_map
/**
 * @nodoc
 */
   int *Dof_map() ;           // rtn: ptr to symeq link_dof_map
/**
 * @nodoc
 */
   int  &Abcd_state()         // rtn: source SPAmatrix state bit array
        { return dmo_abcd_state ; }
/**
 * @nodoc
 */
   void Update_abcd_state     // eff: change abcd state for changes
    (DS_cstrn *cstrn,         // in : in this changing cstrn
     int area_action_flag) ;  // in : 1=rebuild dof_map for area cstrns
                              //      0=don't rebuild now because we
                              //      will call again in a minute
/**
 * @nodoc
 */
   double &Last_dist2()       // rtn: dof-coord max motion squared
                              //      for last solve
           { return dmo_last_dist2 ; }

/**
 * @nodoc
 */
 int                        // rtn: abcd_state for DS_symeq funcs
    Symeq_abcd_state() ;      // eff: map abcd_state->symeq_abcd_state
/**
 * @nodoc
 */
   void Set_state          // eff: set DS_dmod state value
     (int state)           // in : the input state value
   { dmo_abcd_state = state ; }

   // solver state access
/**
 * @nodoc
 */
 int State()             // rtn: state for 'this' dmesh
     { return dmo_abcd_state ; }
/**
 * @nodoc
 */
   void Set_all_rebuild_bits()// eff: set all rebuild SPAmatrix bits
                              { dmo_abcd_state |=
                                              DMO_REBUILD_A
                                            | DMO_REBUILD_B
                                            | DMO_REBUILD_C
                                            | DMO_REBUILD_D
                                            | DMO_REBUILD_L
                                            | DMO_REBUILD_LC
                                            | DMO_REBUILD_MIN_MAP
                                            | DMO_INIT_DOF_MAP
                                            | DMO_INIT_X ;

                                // pass the rebuild onto the mesh
                                if(dmo_dmesh)
                                 dmo_dmesh->Set_dof_map_rebuild() ;
                              }
/**
 * @nodoc
 */
   void Set_A_rebuild()       // eff: set the rebuild_A SPAmatrix state bit
       { dmo_abcd_state |=  DMO_REBUILD_A ; }
/**
 * @nodoc
 */
   void Set_b_rebuild()       // eff: set the rebuild_b SPAmatrix state bit
        { dmo_abcd_state |=  DMO_REBUILD_B ; }

/**
 * @nodoc
 */
 void                       // eff: set the rebuild_Cd_min_map state bit
    Set_Cd_min_map_rebuild    //      causes rebuild of dmesh joint problem
    (int walk_flag=0)         // in : specify how deep to go
                              //      0='this' only, 1=offspring also
                              //      2=this,sibs,and offspring
                              { dmo_abcd_state |=   DMO_REBUILD_MIN_MAP ;
                                if(dmo_dmesh)
                                 dmo_dmesh->Set_dof_map_rebuild() ;
                                // recurse as requested
                                if(walk_flag == 2 && dmo_sibling)
                                  dmo_sibling->Set_Cd_min_map_rebuild(2) ;
                                if(walk_flag & 3  && dmo_child)
                                  dmo_child->Set_Cd_min_map_rebuild(2) ;
                              }
/**
 * @nodoc
 */
 void                       // eff: set the DM_INIT_DOF_MAP state bit
     Set_init_dof_map         //      causes rebuild of dmesh joint problem
    (int walk_flag=0)         // in : specify how deep to go
                              //      0='this' only, 1=offspring also
                              //      2=this,sibs,and offspring
                              { dmo_abcd_state |= DMO_INIT_DOF_MAP ;
                                if(dmo_dmesh)
                                 dmo_dmesh->Set_dof_map_rebuild() ;
                                // recurse as requested
                                if(walk_flag == 2 && dmo_sibling)
                                  dmo_sibling->Set_init_dof_map(2) ;
                                if(walk_flag & 3  && dmo_child)
                                  dmo_child->Set_init_dof_map(2) ;
                              }
/**
 * @nodoc
 */
   void Set_order_dof_map     // eff: set the DM_ORDER_DOF_MAP state bit
    (int walk_flag=0)         // in : specify how deep to go
                              //      0='this' only, 1=offspring also
                              //      2=this,sibs,and offspring
                              { dmo_abcd_state |= DMO_ORDER_DOF_MAP ;
                                // recurse as requested
                                if(walk_flag == 2 && dmo_sibling)
                                  dmo_sibling->Set_order_dof_map(2) ;
                                if(walk_flag & 3  && dmo_child)
                                  dmo_child->Set_order_dof_map(2) ;
                              }
/**
 * @nodoc
 */
   void Set_C_rebuild()       // eff: set the rebuild_C SPAmatrix state bit
                              //      set rebuild dmesh joint problem
                              { dmo_abcd_state |=  DMO_REBUILD_C ;
                                if(dmo_dmesh)
                                 dmo_dmesh->Set_dof_map_rebuild() ;
                              }
/**
 * @nodoc
 */
   void Set_d_rebuild()       // eff: set the rebuild_d SPAmatrix state bit
       { dmo_abcd_state |=  DMO_REBUILD_D ; }
/**
 * @nodoc
 */
   void Set_L_rebuild()       // eff: set the rebuild_L SPAmatrix state bit
        { dmo_abcd_state |=  DMO_REBUILD_L ; }
/**
 * @nodoc
 */
   void Set_Lc_rebuild()      // eff: set the rebuild_Lc SPAmatrix state bit
        { dmo_abcd_state |=  DMO_REBUILD_LC ; }
/**
 * @nodoc
 */
   void Set_dyn_effect()      // eff: turn on dynamic effect state bit
        { dmo_abcd_state |=  DMO_DYN_TERMS ; }
/**
 * @nodoc
 */
   void Clear_dyn_effect()    // eff: turn off dynamic effect state bit
        { dmo_abcd_state &= ~DMO_DYN_TERMS ; }
/**
 * @nodoc
 */
   void Set_x_init()          // eff: set the init X array state bit
        { dmo_abcd_state |=  DMO_INIT_X    ; }
/**
 * @nodoc
 */
   void Set_first_x()         // eff: set the first X array state bit
        { dmo_abcd_state |= DMO_FIRST_X    ; }
/**
 * @nodoc
 */
   void Clear_x_init()        // eff: clear the init X array state bit
        { dmo_abcd_state &= ~DMO_INIT_X    ; }
/**
 * @nodoc
 */
   int                        // rtn: 0=success,1=bad input reason
     Set_tag_obj_rebuild_on   // eff: tell solver to rebuild tag_objs
         (int reason) ;       // in : orof DMO_CHANGED_X
                              //           DMO_CHANGED_P_ON
                              //           DMO_CHANGED_P_OFF
                              //           DMO_CHANGED_DOMAIN
                              //           DMO_APPROX_SHAPE
                              //           DMO_TRACKING

   // DS_dmod tag number management
/**
 * @nodoc
 */
 int                     // rtn: unique tag number for tag obj id
     Assign_next_tag()     // eff: increment and return the
                           //      hierarchy's tag id value. Call
                           //      this function to assign a tag
                           //      value for every tag obj being
                           //      created to make all
                           //      hierarchy tag values unique.
                           { DS_dmod *root = Root() ;
                             root->dmo_tag_count++ ;
                             return(root->dmo_tag_count) ;
                           }
/**
 * @nodoc
 */
   int Tag_count()         // rtn: last tag id value assigned to
                           //      a tag obj in this hierarchy
                           { return(Root()->dmo_tag_count) ;
                           }

/**
 * @nodoc
 */
   void Set_tag_count      // eff: set hierarchy's dmo_tag_count
     (int tag_count)       // in : desired tag number start value.
                           // note: take care when changing this
                           //       value not to create redundant
                           //       tag values in a hierarchy.
                           { DS_dmod *root = Root() ;
                             root->dmo_tag_count = tag_count ;
                           }

   // DS_dmod user interaction data - control points (dofs)
/**
 * @nodoc
 */
 int                     // rtn: drawn dof index or -1
    Find_cpt_by_iline      // eff: A geometric pick based on the
                           //       line pi = pi0 + u (pi1-pi0)
    (double *pi0,          // in : image space pt, Sized:[image_dim]
     double *pi1,          // in : image space pt, Sized:[image_dim]
     double max_dist,      // in : max dist in which to find cstrn
     double &dist2,        // out: dist**2 to iline from load
     double &uu,           // out: iline param val of closest pt
     DS_dmod *&tgt_dmod,   // out: ptr to dmod containing closest cpt
     int cpt_draw_bit,     // in : bit in dmo_draw_state to draw cpts
     int walk_flag=0) ;    // in : specify how deep to search
                           //      0= only search 'this'
                           //      1= also search offspring

   // DS_dmod user interaction data - constraints
/**
 * @nodoc
 */
 int                     // rtn: total number of cstrns in 'this' object
    Cstrn_count()          { return(dmo_cstrn_count) ; }
/**
 * @nodoc
 */
   DS_cstrn *              // rtn: the first cstrn on linked list
    Cstrn()                { return dmo_cstrn ; }

/**
 * @nodoc
 */
 int                     // rtn: total number of crv_cstrns
    Tracking_curve_count();//      with tracking curve dmods
/**
 * @nodoc
 */
   DS_cstrn *              // rtn: drawn cstrn found by input line or NULL
    Find_cstrn_by_iline    // eff: A geometric pick based on the
                           //       line pi = pi0 + u (pi1-pi0)
    (double *pi0,          // in : image space pt, Sized:[image_dim]
     double *pi1,          // in : image space pt, Sized:[image_dim]
     double max_dist,      // in : max dist in which to find cstrn
     DS_CST_PT_INDEX       // out: image_pt index in cstrn for closest pt
         &pt_index,        //      (only used by DS_pt_cstrns)
     double &dist2,        // out: dist**2 to iline from load
     double &uu,           // out: iline param val of closest pt
     DS_dmod *&tgt_dmod,   // out: ptr to dmod containing closest cstrn
     int cstrn_draw_bit,   // in : bit in dmo_draw_state to draw cstrns
     int seams_draw_bit,   // in : bit in dmo_draw_state to draw seams
     int walk_flag=0) ;    // in : specify how deep to search
                           //      0= only search 'this'
                           //      1= also search offspring
/**
 * @nodoc
 */
  DS_cstrn *               // rtn: first link-list cstrn to match tag
   Find_cstrn_by_tag       //      or NULL if not found
    (int       tag,        // in : identifying tag
     DS_dmod *&tgt_dmod,   // out: ptr to dmod containing closest cstrn
     int    walk_flag=0) ; // in : specify how deep to search
                           //      0= only search 'this'
                           //      1= also search offspring
/**
 * @nodoc
 */
  DS_cstrn *               // rtn: first link-list cstrn to match tag
   Find_cstrn_by_src_dmod  //      or NULL if not found
    (DS_dmod *src_dmod,    // in : src_dmod to match
     DS_dmod *&tgt_dmod,   // out: ptr to dmod containing closest cstrn
     int    walk_flag=0) ; // in : specify how deep to search
                           //      0= only search 'this'
                           //      1= also search offspring
/**
 * @nodoc
 */
   void                    // eff: Put cstrn on link-list just once.
    Add_cstrn              //      Mark C and d for rebuild.
     (DS_cstrn *cstrn,     // in : element to add to linked list
      int dmod_index=1) ;  // in : 1 or 2 (for link_cstrns)
/**
 * @nodoc
 */
   DS_dmod *               // rtn: detached dmod from deleted link_cstrn
    Rm_cstrn               // eff: rm cstrn from list and delete it.
    (DS_cstrn *cstrn,      // in : element to rm and delete
     int deletable_flag=1, // in : 1=only rm deletable cstrns
                           //      0=rm any cstrn
     int detached_flag=1); // in : 1=separate sib lists for links
                           //      0=don't
                           // note: Mark C and d for rebuild.
/**
 * @nodoc
 */
   int                     // rtn: 0=success,-1=bad_cstrn_dpts
                           // eff: toggle cstrn state on and off
    Toggle_cstrn           //      and mark C and d for rebuild.
     (DS_cstrn *cstrn) ;   // in : cstrn whose state is toggled

/**
 * @nodoc
 */
 int                      // rtn: 0=changed,-1=failed,-2=no change
   Set_cstrn_behavior      // eff: set cstrn behavior, mark Cd for rebuild
    (DS_cstrn *cstrn,      // in : cstrn to modify
     int behavior) ;       // in : orof DS_CST_POS_FREE     DS_CST_TAN_FREE
                           //           DS_CST_POS_FIXED    DS_CST_TAN_FIXED
                           //           DS_CST_POS_LINKED   DS_CST_TAN_LINKED
                           //
                           //           DS_CST_POS_2_FREE   DS_CST_TAN_2_FREE
                           //           DS_CST_POS_2_FIXED  DS_CST_TAN_2_FIXED
                           //           DS_CST_POS_2_LINKED DS_CST_TAN_2_LINKED
/**
 * @nodoc
 */
  int                      // rtn: 0=state_set,-1=not_set,-2=bad_cstrn_dpts
   Set_cstrn_state         // eff: set cstrn state, mark Cd for rebuild
    (DS_cstrn *cstrn,      // in : cstrn to modify
     int state) ;          // in : 1=turn cstrn on, 0=turn cstrn off
/**
 * @nodoc
 */
  int                      // rtn: 0=zone_flag set,-1=not_set
   Set_area_cstrn_flag     // eff: set cstrn state, mark rebuild all
    (DS_cstrn *cstrn,      // in : cstrn to modify
     int zone_flag) ;      // in : 0=zone area moves
                           //        zone compliment fixed
                           //      1=zone area fixed
                           //        zone compliment moves
/**
 * @nodoc
 */
  int                      // rtn: 0 when mag is varied, else -1
   Set_tang_gain           // eff: vary crv_tang_cstrn constraint magnitude
    (DS_cstrn *cstrn,      // in : cstrn to modify
     double tang_gain) ;   // in : vary tangent constraint magnitude
/**
 * @nodoc
 */
  DS_CST                   // rtn: obj's enum type or ds_cnone
   Ch_image_pt_cstrn       // eff: change existing pt cstrn image
                           //      pt loc. Mark d for rebuild.
   (DS_cstrn *cstrn,       // in : cstrn to change
    DS_CST_PT_INDEX        // in : index of cstrn image_pt to change
       pt_index,           //      PST_BASE_PT = update base_pt
                           //      PST_TANG    = update tang1_pt
                           //      PST_TANG1   = update tang1_pt
                           //      PST_TANG2   = update tang2_pt
                           //      PST_NORM    = update norm1_pt
                           //      PST_CURV1   = update curv1_pt
                           //      PST_CURV2   = update curv2_pt
                           //      PST_BINORM  = update binorm_pt
    double *image_pt,      // in : new image loc, Sized:[Image_dim()]
    int curvature_sign=0); // in : used when domain_dim==2 &&
                           //      pt_index==PST_CURV1 or PST_CURV2
                           //   +1: use curv_pt =  k *  n
                           //   -1: use curv_pt = -k * -n
                           //      to solve for new values k and n
/**
 * @nodoc
 */
  DS_CST                   // rtn: obj's enum type or ds_cnone
   Ch_value_pt_cstrn       // eff: change existing pt cstrn value.
                           //      Mark d for rebuild.
    (DS_cstrn *cstrn,      // in : cstrn to change
     DS_CST_PT_INDEX       // in : index of cstrn value to change
        pt_index,          //      PST_BASE_PT = update base_pt
                           //      PST_TANG    = update tang1_pt
                           //      PST_TANG1   = update tang1_pt
                           //      PST_TANG2   = update tang2_pt
                           //      PST_NORM    = update norm1_pt
                           //      PST_CURV1   = update curv1_pt
                           //      PST_CURV2   = update curv2_pt
                           //      PST_BINORM  = update binorm_pt
     int cstrn_val_size,   // in : size of cstrn_val array
     double *cstrn_val) ;  // in : new cstrn val, Sized:[cstrn_val_size]

/**
 * @nodoc
 */
  DS_CST                   // rtn: obj's enum type or ds_cnone
   Ch_domain_pt_cstrn      // eff: change existing pt cstrn domain
                           //      pt loc. Mark C & d for rebuild.
    (DS_cstrn *cstrn,      // in : cstrn to change
     double *domain_pt) ;  // in : new domain loc,Sized:[Domain_dim()]
/**
 * @nodoc
 */
  DS_CST                   // rtn: obj's enum type or ds_cnone
   Ch_domain_dir_cstrn     // eff: change existing pt cstrn domain
                           //      pt dir. Mark C & d for rebuild.
    (DS_cstrn *cstrn,      // in : cstrn to change
     double *domain_dir,   // in : new domain dir, Sized:[domain_dim]
     DS_CST_PT_INDEX       // in : which domain_dir to modify
       pt_index) ;         //      PST_TANG1,
                           //      PST_TANG2,
                           //      PST_CURV1,
                           //      PST_CURV2
/**
 * @nodoc
 */
  DS_CST                   // rtn: obj's enum type or ds_cnone
  Ch_cstrn_src_pfuncs      // eff: change link or crv_cstrn srcs.
  (DS_cstrn *cstrn,        // in : cstrn to change
   DS_pfunc *src_W_pfunc,   // in : cstrn pos shape, [nested]
   DS_pfunc *src_Wn_pfunc,  // in : cross-tang shape,[nested]
   DS_pfunc *src_Wnn_pfunc);// in : curvature shape, [nested]
/**
 * @nodoc
 */
  DS_CST                   // rtn: obj's enum type or ds_cnone
   Update_cstrn_pts        // eff: rebuild cstrn stored arrays
    (DS_cstrn *cstrn) ;    // in : cstrn to change

   //=============================================
   // DS_dmod user interaction data - loads
   //=============================================
/**
 * @nodoc
 */

   int                     // rtn: total number of loads
    Load_count()           { return dmo_load_count ; }
/**
 * @nodoc
 */
   DS_load *               // rtn: the first load on linked list
    Load()                 { return dmo_load ; }
/**
 * @nodoc
 */
   DS_load *               // rtn: drawn load found by input line or NULL
    Find_load_by_iline     // eff: A geometric pick based on the
                           //       line pi = pi0 + u (pi1-pi0)
    (double *pi0,          // in : image space pt, Sized:[image_dim]
     double *pi1,          // in : image space pt, Sized:[image_dim]
     double max_dist,      // in : max dist in which to find cstrn
     int    &pt_index,     // out: image_pt index in cstrn for closest pt
     double &dist2,        // out: dist**2 to iline from load
     double &uu,           // out: iline param val of closest pt
     DS_dmod *&tgt_dmod,   // out: ptr to dmod containing closest cpt
     int draw_load_bit,    // in : bit dmo_draw_state to draw loads
     int walk_flag=0) ;    // in : specify how deep to search
                           //      0= only search 'this'
                           //      1= also search offspring
                           //      2= search siblings and offspring
/**
 * @nodoc
 */
  DS_load *                // rtn: first load on link-list of type
   Find_load_by_type       //      or NULL if none found
    (DS_LDS type,          // in : oneof: ds_ppr = pt_press
                           //             ds_dpr = dist_press
                           //             ds_vec = vector_load
                           //             ds_att = attractor load
                           //             ds_dld = dyn_load
                           //             ds_spr = spring
     DS_dmod *&tgt_dmod,   // out: ptr to dmod containing closest cpt
     int walk_flag=0) ;    // in : specify how deep to search
                           //      0= only search 'this'
                           //      1= also search offspring
/**
 * @nodoc
 */
  DS_load *                // rtn: first link-list load to match tag
   Find_load_by_tag        //      or NULL if not found
    (int tag,              // in : identifying tag
     DS_dmod *&tgt_dmod,   // out: ptr to dmod containing closest cpt
     int walk_flag=0) ;    // in : specify how deep to search
                           //      0= only search 'this'
                           //      1= also search offspring
/**
 * @nodoc
 */
   void
    Add_load               // eff: Put load on link-list just once.
                           //      Mark SPAmatrix b for rebuild.
    (DS_load *load) ;      // in : element to add to linked list
/**
 * @nodoc
 */
   void
    Rm_load                // eff: rm load from list and delete it.
                           //      Mark SPAmatrix b for rebuild.
    (DS_load *load) ;      // in : element to rm and delete
/**
 * @nodoc
 */

  DS_LDS                   // rtn: type when gain set else ds_lnone
    Ch_load_gain           // eff: change load's gain. Mark b(and
                           //      A if DS_LDS==ds_spr) for rebuild
  (DS_load *load,          // in : load to change
   double gain) ;          // in : new gain value
/**
 * @nodoc
 */
  DS_LDS                   // rtn: type when gain set else ds_lnone
   Ch_load_domain_pt       // eff: change existing pt load domain
                           //      pt loc. Mark b for rebuild.
    (DS_load *load,        // in : load to change
     double *domain_pt) ;  // in : new domain loc,Sized:[Domain_dim()]

   // DS_dmod side-effect functions
/**
 * @nodoc
 */
   void                    // use: called by Build_AbCd()
    Build_A() ;            // eff: build A SPAmatrix in Ax=b.
                           //      Clears SPAmatrix A rebuild bit
                           //      Sets   SPAmatrix A changed bit
/**
 * @nodoc
 */
   int                     // rtn: 1=0.k,0=err. for Debug only
    Check_A() ;            // eff: check that every row adds to 0.0


 // use: called by Build_A()
/**
 * @nodoc
 */
 DS_diag_mat *           // rtn: ptr to dyn's mc SPAmatrix or NULL
    Build_dyn_effect() ;   // eff: if(abcd_state & DMO_DYN_TERMS)
                           //           ensures there is 1  dyn_load
                           //      else ensures there is no dyn_load
/**
 * @nodoc
 */
   void Set_default_shape  // set: default shape state in dmo_pfunc
     (int shape_flag,      // in : 0: default_shape = 0
                           //      1: default_shape = current_shape
      int walk_flag=0);    // in : specify how deep to go
                           //      0='this' only, 1=offspring also
/**
 * @nodoc
 */
   int Default_state() ;   // rtn: dmo_pfunc default shape state
                           //      0=not useing a default shape
                           //      1=using a default shape
/**
 * @nodoc
 */
   void Get_default_state  // get: dmo_pfunc default shape state
     (int &flag) ;         // out: 0: default_shape = 0
                           //      1: default_shape = current_shape
/**
 * @nodoc
 */
   void Set_interior_state // set: interior state in dmo_pfunc
    (int interior_flag,    // in : 0 = C0 between elements allowed
                           //      1 = C1 enforced between elements
     int walk_flag=0) ;    // in : specify how deep to go
                           //      0='this' only, 1=offspring also
/**
 * @nodoc
 */
   int Interior_state() ;  // rtn: 0 = C0 between elements allowed
                           //      1 = C1 enforced between elements
/**
 * @nodoc
 */
   void                    // set: dof_map[ii] = -sme_n-1 for all xf
    Init_and_mark_dof_map  //      dof_map[ii] = 0 for all others
     (int walk_flag=0) ;   // in : specify how deep to go
                           //      0='this' only, 1=offspring also
                           //      2=this,sibs,and offspring
                           //      3=this and sibs only
/**
 * @nodoc
 */
   void                    // eff: give each marked dof an index
    Order_dof_map          //      eg [0 -1 1 -2 -3 2 3 ...]
     (int walk_flag=0) ;   // in : specify how deep to go
                           //      0='this' only, 1=offspring also
                           //      2=this,sibs,and offspring

#ifndef DS_USE_SYMEQ
/**
 * @nodoc
 */
   void                    // use: called by Build_abcd()
    Size_eq                // eff: size symeq for all build funtions
     (int walk_flag=0) ;   // in : specify how deep to go
                           //      0='this' only, 1=offspring also
                           //      2=this,sibs,and offspring
#else /*DS_USE_SYMEQ*/
/**
 * @nodoc
 */
 void                    // use: called by Build_abcd()
    Size_symeq             // eff: size symeq for all build funtions
     (int walk_flag=0) ;   // in : specify how deep to go
                           //      0='this' only, 1=offspring also
                           //      2=this,sibs,and offspring
#endif /*DS_USE_SYMEQ*/
/**
 * @nodoc
 */
   void                    // eff: mark used dofs in min_Cd_map
    Init_and_mark_min_Cd_map//      eg [0 -1 0 -1 -1 0 0 ...]
    (int walk_flag=0,      // in : specify how deep to go
                           //      0='this' only, 1=offspring also
                           //      2=this,sibs,and offspring
     int refine_flag=0) ;  // in : 1=being called for refine fit,
                           //        treat links as fixed.
                           //      0=being called for solve
/**
 * @nodoc
 */
  void                     // eff: give each marked dof an index
   Order_min_Cd_map        //      eg [0 -1 1 -1 -1 2 3 ...]
    (int walk_flag) ;      // in : specify how deep to go
                           //      0='this' only, 1=offspring also
                           //      2=this,sibs,and offspring
/**
 * @nodoc
 */
   void                    // use: called by Build_AbCd()
    Build_Cd               // eff: build C and d matrices of Cx=d
     (int refine_flag=0) ; // in : 1=being called for refine fit,
                           //        treat links as fixed.
                           //      0=being called for solve
                           //      Clears SPAmatrix C and d rebuild bits
                           //      Sets   SPAmatrix C and d changed bits
/**
 * @nodoc
 */
   void                    // use: called by Build_AbCd()
    Build_d() ;            // eff: build d SPAmatrix of Cx=d
                           //      Clears SPAmatrix d rebuild bit
                           //      Sets   SPAmatrix d changed bit
/**
 * @nodoc
 */
   void                    // use: called by Build_AbCd()
    Build_b() ;            // eff: build b SPAmatrix in Ax=b
                           //      Clears SPAmatrix b rebuild bit
                           //      Sets   SPAmatrix b changed bit
/**
 * @nodoc
 */
   void                    // use: called by Build_AbCd()
    Build_L() ;            // eff: Build Li matrices in Lixi=Lc
                           //      Clears matrices Li and Lc rebuild bit
                           //      Sets matrices Li and Lc changed bit
/**
 * @nodoc
 */
   void                    // use: called by Build_AbCd()
    Build_Lc() ;           // eff: Build Lc SPAmatrix in Lxi=Lc
                           //      Clears SPAmatrix Lc rebuild bit
                           //      Sets SPAmatrix Lc changed bit
/**
 * @nodoc
 */
   void                    // use: called by Build_AbCd() and Solve()
    Rebuild_tag_objs       // eff: recalc tag locs after a change in pfunc
     (int walk_flag=0) ;   // in : specify how deep to go
                           //      0='this' only, 1=offspring also
                           // eff: calls Update_load_pts(pfunc,1)
                           //      and Update_cstrn_pts(pfunc,1) to
                           //      refresh cstrns and loads after
                           //      changes in dmo_pfunc.  Updating
                           //      sliding springs may cause an A rebuild

// bmt - lib proj DECL - no interface methods
/**
 * @nodoc
 */
 DECL_DM void            // use: called by Build_AbCd()
    Init_x_old_x           // eff: calcs and saves Q = X - P
     () ;                  //      in symeq->X() and symeq->Old_x()
/**
 * @nodoc
 */
   void                    // use: called by Build_AbCd()
    Init_first_x           // eff: copies dmo_symeq->X() to First_x().
     () ;
/**
 * @nodoc
 */
   void                    // use: called by Solve()
    Move_x_to_pfunc        // eff: calcs and saves X = P + Q
     (int x_flag=1) ;      // in : 0=> let Q = dmo_symeq->First_x()
                           //      1=> let Q = dmo_symeq->X()
                           //      where X = pfunc->Dof_vec()
                           //            P = pfunc->Dof_def()
/**
 * @nodoc
 */
   void                    // use: called by Solve()
    Bad_track_response() ; //      when tracking attempt fails
                           // eff: reset all tracking cstraints
/**
 * @nodoc
 */
   void                    // use: called by Solve()
    Good_track_response(); //      when tracking attempt succeeds
                           // eff: tell cstrns tracking succeeded
/**
 * @nodoc
 */
   void                    // eff: compose the Ax=b, Cx=d, Lx=Lc
    Build_AbCd() ;         //      problem matrices.
/**
 * @nodoc
 */
   int                     // rtn: constraint conflict cnt(0=success)
                           //      or -1 for no convergence
     Solve                 // eff: refresh AbCd and solve for X iter times
    (int iter=1,           // in : iter count before stopping
                           //      -1 = iter to convergence
     int walk_flag=0,      // in : specify how deep to go
                           //      0='this' only, 1=offspring also
     double max_dist2=     // in : Used when iter == -1, converges
         DS_DOUBLE_CLOSE); //      when dof max-motion-squared
                           //      is less than max_dist2

                           // use: called by Solve()
/**
 * @nodoc
 */
 int                     // rtn: 0 = not converged (yet)
                           //      1 = converged
                           //      2 = figured out we won't converge
    Solve_response         // eff: actions to take after each
                           //      attempted solve step
    (int iter,             // in : iter count before stopping,
                           //      -1 = iter to convergence
     int ii,               // in : Used when iter == -1, iteration count
     double max_dist2,     // in : Used when iter == -1, converges
                           //      when dof max-motion-squared
                           //      is less than max_dist2
     int conflict_count) ; // in : count of conflicting solutions
                           //      discovered in last solve step
/**
 * @nodoc
 */
   void                    // use: called by Solve()
    Unconverged_response   // eff: restore state if
    ()  ;                  //      solve fails to converge

/**
 * @nodoc
 */
   double                  // rtn: max distance one dof moved.
    Refine_fit             // use: Prior to committing shape
    (double err_size,      // in : max ok crv_err without postprocessing
     double &residual,     // out: abs max elem value in r = Cx-d
     double &beg_dist_err, // out: max edge/crv_cstrn dist before refine
     double &end_dist_err, // out: max edge/crv_cstrn dist after refine
     int walk_flag=0) ;    // in : specify how deep to go
                           //      0='this' only, 1=offspring also
                           // eff: Moves the dofs by a minimal amount
                           //      to minimize the residual of the
                           //      constraint equations, r = Cx - d.

/**
 * @nodoc
 */
 int                     // rtn: 0=ok,-1=incompatible new pfunc
    Ch_pfunc               // eff: change definition for pfunc,
                           //      decrement references to old pfunc
     (DS_pfunc *pfunc) ;   // in : new pfunc
                           // note: new pfunc should have same domain
                           //       range as old to prevent tag obj problems

   // dimension dependent virtual functions
/**
 * @nodoc
 */
 virtual void            // bas: DS_dmod virtual function
    Build_elem_K           // eff: build dmo_K SPAmatrix for elem_index
     (int elem_index,      // in : indicates which elem to build
      int &elem_dof_count, // out: number of dofs in this element
      const int*
          &elem_dof_map)=0;// out: global index for each elem dof
/**
 * @nodoc
 */
   virtual void            // bas: DS_dmod virtual function
    Build_elem_K_debug     // eff: build dmo_K SPAmatrix for elem_index
    (int elem_index,       // in : indicates which elem to build
     int &elem_dof_count,  // out: number of dofs in this element
     const int*
         &elem_dof_map)=0; // out: global index for each elem dof
/**
 * @nodoc
 */
    int                    // rtn: number of bad rows (0=O.K.)
     Check_elem_K          // eff: Check Sum of each row = 0.0
      (int elem_index) ;   // in : index of elem being checked

} ; // end class DS_dmod

//==============================================================
// Derived class DS_dsurf : public DS_dmod
//==============================================================
/**
 * @nodoc
 */
class DS_dsurf : public DS_dmod  // identifier = dsf
{
  protected:

   // curve and surface deformation parameters
   double  dsf_au ;        // "alpha": stretch resistance
   double  dsf_av ;        //  (u dir, v dir, rotate angle (degrees))
   double  dsf_atheta ;
   double  dsf_bu ;        // "beta": bending resistance
   double  dsf_bv ;        //  (u dir, v dir, rotate angle (degrees))
   double  dsf_btheta ;

  public:

   // constructor, copy, destructor
   DECL_DM DS_dsurf                // DS_dsurf default constructor
    (DS_pfunc *pfunc=NULL, // in : defines the shape model
     int tag       =2,     // in : root dmod's have tag = 2
     int draw_state=       // in : draw bits for application graphics
          SPA_DM_DRAW_SEAMS    //      (draw loads, cstrns, and seams)
        | SPA_DM_DRAW_CSTRNS   //      SPA_DM_DRAW_CSTRNS | SPA_DM_DRAW_LOADS |
        | SPA_DM_DRAW_LOADS,   //      SPA_DM_DRAW_SEAMS,
     double au     = 1.0,  // in : resist stretch u direction
     double av     = 1.0,  // in : resist stretch v direction
     double atheta = 0.0,  // in : resist stretch rotation angle
     double bu     = 5.0,  // in : resist bending u direction
     double bv     = 5.0,  // in : resist bending v direction
     double btheta = 0.0,  // in : resist bending rotation angle
     double gamma  = 0.0,  // in : resist bending rate of change
     double delta  = 0.0,  // in : resist moving from default shape
     double dt     = 1.0,  // in : implicit integration time step
     double mass   = 1.0,  // in : physical param (causes rippling)
     double damp   = 5.0); // in : physical param (stable intgrtn)

   virtual                 // bas: DS_dmod pure virtual function
    DS_dmod *              // rtn: the newly created child dmod or NULL
     Add_patch             // eff: add a patch to the 'this' dmod
     (void *patch_entity,  // in : app entity ptr stored with patch
      double *min,         // in : lower domain point for patch
      double *max,         // in : upper domain point for patch
      int seam_count,      // in : number of pfuncs in the seam
      DS_pfunc **seam,     // in : array of pfuncs that define the seam
      void **seam_data,    // in : app entity data stored with each seam
      int refinement,      // in : parent_knot_spacing/child_knot_spacing
      int &tag,            // i/o: start and final available tag ids
      int draw_state) ;    // in : draw bits for application graphics

/* // moved to dmapi.cxx
   virtual void            // bas: class DS_dmod pure virtual function
    Build_square_seam      // eff: build a set of curves to act as a seam
     (double *min,         // in : lower domain point for patch
      double *max,         // in : upper domain point for patch
      int &pfunc_count,    // out: number of pfuncs in pfunc_array
      DS_pfunc **&pfunc) ; // out: newly created pfunc array (NULL on input)
                           // mallocs: pfunc array and each pfunc entry

   virtual void            // use: for surface dmods only
    Build_ellipse_seam     // eff: build a elliptical seam
     (double *cpt,         // in : center point for patch    , sized:[2]
      double *axis1,       // in : SPAvector from cpt to 1st-axis end point
      double *axis2,       // in : SPAvector from cpt to 2nd-axis end point
      double *min,         // out: lower domain ellipse bound, sized:[2]
      double *max,         // out: upper domain ellipse bound, sized:[2]
      int &pfunc_count,    // out: always set to 1
      DS_pfunc **&pfunc);  // out:newly created pfunc array (NULL on input)
                           // mallocs: pfunc array and each pfunc entry
*/ // moved to dmapi.cxx

   DS_dsurf                // eff: copy constructor
    (DS_dsurf &dsurf,      // in : object to copy
     int walk_flag=0) ;    // in : specify how deep to go
                           //      0='this' only, 1='this' and offspring


   DS_dsurf &operator=     // eff: assignment operator
    (DS_dsurf &dsurf) ;    // in : the object from which to copy values
                           // note: assignment drops hierarchy links.
                           //       use Make_copy to copy hierarchy
                           //       sub_trees.


   virtual DS_dmod*        // bas: Virtual DS_dmod method
     Make_copy             // eff: virtual copy function
      (int walk_flag=0)    // in : specify how deep to go
                           //      0='this' only, 1='this' and offspring
                           { return( new DS_dsurf(*this,walk_flag)) ; }

   virtual                 // bas: virtual DS_dmod method
   ~DS_dsurf();             // eff: free 'this' and offspring data
   // DS_dsurf simple data access functions
   // DS_dsurf curve and surface deformation parameters
   double Au()             // rtn: u component of resist_stretch param
                           { return dsf_au ; }
   double Av()             // rtn: v component of resist_stretch param
                           { return dsf_av ; }
   double Atheta()         // rtn: resist_stretch rotation angle
                           { return dsf_atheta ; }

   // build alpha tensor for au,av, and atheta
   double A11()            // rtn: A11 of rotated resist_stretch param
                           { double c = cos(dsf_atheta*DS_PI/180.0) ;
                             double s = sin(dsf_atheta*DS_PI/180.0) ;
                             return c*c*dsf_au + s*s*dsf_av ;
                           }
   double A12()            // rtn: A12 of rotated resist stretch param
                           { double c = cos(dsf_atheta*DS_PI/180.0) ;
                             double s = sin(dsf_atheta*DS_PI/180.0) ;
                             return c*s*(dsf_av - dsf_au) ;
                           }
   double A22()            // rtn: A22 of rotated resist stretch param
                           { double c = cos(dsf_atheta*DS_PI/180.0) ;
                             double s = sin(dsf_atheta*DS_PI/180.0) ;
                             return c*c*dsf_av + s*s*dsf_au ;
                           }

   virtual                 // bas: virtual DS_dsurf method
    int                    // rtn: 0=success, -1=wrong domain_dim
     Get_alpha             // get: 'resist stretch' SPAparameter values
      (int domain_dim,     // in : should be 2 for 2d surfaces
       double *alpha) ;    // out: [au, av, theta], sized:3

   virtual                 // bas: virtual DS_dmod method
   int                     // rtn: 0=success, -1=wrong domain_dim
    Set_alpha              // set: 'resist stretch' SPAparameter values
    (int domain_dim,       // in : should be 2 for 2d surfaces
     double *alpha,        // in : 1d = [alpha],         sized:1
                           //    : 2d = [au, av, theta], sized:3
     int walk_flag=0) ;    // in : specify how deep to go
                           //      0='this' only, 1='this' and offspring

   double Bu()             // rtn: u component of resist_bending param
                           { return dsf_bu ; }
   double Bv()             // rtn: v component of resist_bending param
                           { return dsf_bv ; }
   double Btheta()         // rtn: resist bending rotation angle
                           { return dsf_btheta ; }

   virtual                 // bas: virtual DS_dsurf method
    int                    // rtn: 0=success, -1=wrong domain_dim
     Get_beta              // get: 'resist bending' param values
      (int domain_dim,     // in : should be 2 for 2d surfaces
       double *beta) ;     // out: [bu, bv, theta], sized:3

   virtual                 // bas: virtual DS_dsurf method
    int                    // rtn: 0=success, -1=wrong domain_dim
     Set_beta              // set: 'resist bending' param values
      (int domain_dim,     // in : should be 2 for 2d surfaces
       double *beta,       // in : [bu, bv, theta], sized:3
       int walk_flag=0) ;  // in : specify how deep to go
                           //      0='this' only, 1='this' and offspring

   // build beta tensor for bu,bv, and btheta
   double B11()            // rtn: B11 of rotated resist bending param
                           { double c = cos(dsf_btheta*DS_PI/180.0) ;
                             double s = sin(dsf_btheta*DS_PI/180.0) ;
                             return c*c*dsf_bu + s*s*dsf_bv ;
                           }
   double B12()            // rtn: B12 of rotated resist bending param
                           { double c = cos(dsf_btheta*DS_PI/180.0) ;
                             double s = sin(dsf_btheta*DS_PI/180.0) ;
                             return c*s*(dsf_bv - dsf_bu) ;
                           }
   double B22()            // rtn: B22 of rotated resist bending param
                           { double c = cos(dsf_btheta*DS_PI/180.0) ;
                             double s = sin(dsf_btheta*DS_PI/180.0) ;
                             return c*c*dsf_bv + s*s*dsf_bu ;
                           }

   // virtual DS_dsurf side-effect functions

   virtual void            // bas: DS_dmod virtual function
    Build_elem_K           // eff: build dmo_K SPAmatrix for elem_index
     (int elem_index,      // in : indicates which elem to build
      int &elem_dof_count, // out: number of dofs in this element
      const int*
          &elem_dof_map) ; // out: global index for each elem dof

   virtual void            // bas: DS_dmod virtual function
    Build_elem_K_debug     // eff: build dmo_K SPAmatrix for elem_index
    (int elem_index,       // in : indicates which elem to build
     int &elem_dof_count,  // out: number of dofs in this element
     const int*
         &elem_dof_map) ;  // out: global index for each elem dof

} ; // end class DS_dsurf

//===============================================================
// Derived class DS_dcurv : public DS_dmod
//===============================================================
/**
 * @nodoc
 */
class DS_dcurv : public DS_dmod  // identifier = dcv
{
  protected:

   // curve and surface deformation parameters
   double  dcv_alpha ;     // "alpha": stretch resistance
   double  dcv_beta ;      // "beta" : bending resistance
  public:

   // constructor, copy, destructor
   DECL_DM DS_dcurv                // DS_dcurv default constructor
    (DS_pfunc *pfunc=NULL, // in : defines the shape model
     int tag=2,            // in : root dmod't use 2 for a tag
     int draw_state=       // in : draw bits for application graphics
           SPA_DM_DRAW_SEAMS   //      (draw loads, cstrns, and seams)
         | SPA_DM_DRAW_CSTRNS  //      SPA_DM_DRAW_CSTRNS | SPA_DM_DRAW_LOADS |
         | SPA_DM_DRAW_LOADS,  //      SPA_DM_DRAW_SEAMS,
     double alpha  = 1.0,  // in : resistance to stretch
     double beta   = 5.0,  // in : resistance to bending
     double gamma  = 0.0,  // in : resist bending rate of change
     double delta  = 0.0,  // in : resistance to moving from default shape
     double dt     = 1.0,  // in : implicit integration time step
     double mass   = 1.0,  // in : physical param (causes rippling)
     double damp   = 5.0); // in : physical param (stable intgrtn)

   virtual                 // bas: DS_dmod pure virtual function
    DS_dmod *              // rtn: the newly created child dmod or NULL
     Add_patch             // eff: add a patch to the 'this' dmod
     (void *patch_entity,  // in : app entity ptr stored with patch
      double *min,         // in : lower domain point for patch
      double *max,         // in : upper domain point for patch
      int,                 // in : seam_count = not used
      DS_pfunc **,         // in : seam       = not used
      void **seam_data,    // in : app entity dta data stored with each seam
      int refinement,      // in : parent_knot_spacing/child_knot_spacing
      int &tag,            // i/o: start and final available tag ids
      int draw_state) ;    // in : draw bits for application graphics

/* // moved to dmapi.cxx
   virtual void            // bas: class DS_dmod pure virtual function
    Build_square_seam      // eff: build a set of curves to act as a seam
     (double *,            // in : min = lower domain point for patch
      double *,            // in : max = upper domain point for patch
      int &,               // out: pfunc_count = number of pfuncs in pfunc_array
      DS_pfunc **&)        // out: pfunc = newly created pfunc array (NULL on input)
                           // mallocs: pfunc array and each pfunc entry
                           { // TODO:???
                           }

   virtual void            // use: for surface dmods only
    Build_ellipse_seam     // eff: build a elliptical seam
     (double *,            // in : cpt = center point for patch    , sized:[2]
      double *,            // in : axis1 = SPAvector from cpt to 1st-axis end point
      double *,            // in : axis2 = SPAvector from cpt to 2nd-axis end point
      double *,            // out: min = lower domain ellipse bound, sized:[2]
      double *,            // out: max = upper domain ellipse bound, sized:[2]
      int &,               // out: pfunc_count = always set to 1
      DS_pfunc **&)        // out: pfunc = newly created pfunc array (NULL on input)
                           // mallocs: pfunc array and each pfunc entry
                           { // TODO:???
                           }
*/ // end moved to dmapi.cxx

   DS_dcurv                // eff: copy constructor
    (DS_dcurv &dcurv,      // in : object to copy
     int walk_flag=0) ;    // in : specify how deep to go
                           //      0='this' only, 1='this' and offspring

   DS_dcurv &operator=     // eff: assignment operator
    (DS_dcurv &dcurv) ;    // in : the object from which to copy values
                           // note: assignment drops hierarchy links.
                           //       use Make_copy to copy hierarchy
                           //       sub_trees.

   virtual DS_dmod*        // bas: Virtual DS_dmod method
     Make_copy             // eff: virtual copy function
      (int walk_flag=0)    // in : specify how deep to go
                           //      0='this' only, 1='this' and offspring
                           { return( new DS_dcurv(*this,walk_flag)) ; }

   virtual                 // bas: virtual DS_dmod method
   ~DS_dcurv();            // eff: free 'this' and 'this' offspring data

   // DS_dcurv simple data access functions
   // DS_dcurv curve and surface deformation parameters

   double Alpha()          // rtn: 'resist stretch' param value
                           { return dcv_alpha ; }

   virtual                 // bas: virtual DS_dmod method
    int                    // rtn: 0=success, -1=wrong domain_dim
     Get_alpha             // get: 'resist stretch' param value
      (int domain_dim,     // in : should be 1 for 1d curves
       double *alpha) ;    // out: [alpha], sized:1

   virtual                 // bas: virtual DS_dmod method
    int                    // rtn: 0=success, -1=wrong domain_dim
     Set_alpha             // set: 'resist stretch' param values
      (int domain_dim,     // in : should be 1 for 1d curves
       double *alpha,      // in : [alpha], sized:1
       int walk_flag=0) ;  // in : specify how deep to go
                           //      0='this' only, 1=offspring also

   double Beta()           // rtn: 'resist bending' param value
                           { return dcv_beta ; }

   virtual                 // bas: virutal DS_dmod method
    int                    // rtn: 0=success, -1=wrong domain_dim
     Get_beta              // get: 'resist bending' param value
     (int domain_dim,      // in : should be 1 for 1d curves
      double *beta) ;      // out: [beta], sized:1

   virtual                 // bas: virutal DS_dmod method
    int                    // rtn: 0=success, -1=wrong domain_dim
     Set_beta              // set: 'resist bending' param value
     (int domain_dim,      // in : should be 1 for 1d curves
      double *beta,        // out: [beta], sized:1
       int walk_flag=0) ;  // in : specify how deep to go
                           //      0='this' only, 1=offspring also

   // virtual DS_dcurv side-effect functions

   virtual void            // bas: DS_dmod virtual function
    Build_elem_K           // eff: build dmo_K SPAmatrix for elem_index
     (int elem_index,      // in : indicates which elem to build
      int &elem_dof_count, // out: number of dofs in this element
      const int*
          &elem_dof_map) ; // out: global index for each elem dof

   virtual void            // bas: DS_dmod virtual function
    Build_elem_K_debug     // eff: build dmo_K SPAmatrix for elem_index
    (int elem_index,       // in : indicates which elem to build
     int &elem_dof_count,  // out: number of dofs in this element
     const int*
         &elem_dof_map) ;  // out: global index for each elem dof


} ; // end class DS_dcurv
/** @} */
#endif // DS_DSURF_H



/*  left overs from intermediate states - saved in case they may
    be needed

   DS_diag_mat
           dmo_A ;         // row-diagonal SPAmatrix
                           //   sized:[dmo_free_count diag matrix]
                           // dmo_A = [ 00 10 20 ... n0]
                           //         [    01 11 ...   ]
                           //         [      .         ]
                           //         [         .      ]
                           //         [           .    ]
                           //         [              0n]
   double *dmo_b ;         // b SPAmatrix in Ax = b
                           //   sized:[dmo_free_count*dmo_image_dim]
   double *dmo_old_x ;     // last iteration values for x in Ax = b
                           //   sized:[dmo_free_count*dmo_image_dim]

*/
