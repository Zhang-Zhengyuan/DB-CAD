/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dspfunc.hxx
// created  : Jan 5, 1995
// author   : GW Celniker
// contains : Class DS_pfunc, DS_tprod_1d, DS_tprod_2d (DS_basis_dsc)
//            DS_call_segment_curve_by_pfunc_isoline()
// synopsis : The abstract class DS_pfunc implements parametric
//            functions which are represented as a linear sum
//            of weighted basis functions.
//            These functions commonly represent shapes but may
//            be used to represent any other property.
//
//            let W = W(u), W(u,v), or W(u,v,w) be the shape
//
//                     i<N
//            and W = Sum (Xi * PHi(u,...))
//                     i=0
//
//            where u,v,w = parametric variables
//                  Xi    = a Degree of Freedom
//                  Phi   = a basis function
//                  N     = DOF_count
//
//            The shape W exists in image space.  The image space
//            has a dimension typically 1D, 2D, or 3D for
//            representing shape but any value in general.
//            The size of a W value is the image space dimension.
//            For example, in 3-space a W value is 3 SPAvector [x y z].
//            Currently, only scalar basis functions are supported
//            and consequently the size of a dof value, Xi, like
//            a W value is also the image space dimension. DOF values
//            are stored explicitly in the DS_pfunc and can be
//            changed at run time while basis functions are
//            represented in code. Each DOF value is associated with
//            one basis function and both can be addressed by index.
//
//            Domains and Sub-domains(elements):
//            Each DS_pfunc has a domain
//            which is the set of all SPAparameter locations, [u v w],
//            on which the function is defined.  The function
//            is not defined outside the domain. Piece-wise
//            parametric functions have domains which are partitioned
//            into sub-domains called elements.  Each sub-domain has
//            its own definition for W.  Tensor product surfaces have
//            regularly shaped domain partitions.  Triangle surfaces
//            have irregular shape partitions.
//
//            dsc_basis functions also have domains.  Typically the
//            domain of a single basis function is a sub-set
//            of the pfunc's partitioned sub-domains.  dsc_basis function
//            domains are always bounded by the DS_pfunc partition
//            boundaries.
//
//            DS_pfuncs track the number of sub-domains and can refer
//            to each uniquely by an index that starts at 0.
//            DS_pfuncs can report the list of basis function
//            indicies defined in each sub-domain and the
//            set of sub-domain indicies over which a basis function
//            is defined.
//
// dof_vec &: Shape W = Sum(Xi * Phii)     for i=0 to i=dof_count-1
// dof_def  :  where Xi are the dof values of the pfunc equation
//                   Phii are the basis function values
//            The deformable modeling paradigm computes displacements
//            from a default shape so
//            X = P + Q
//             where P = the default or 'permanent' shape
//                   Q = the displacements from the permanent shape
//            dof_vec stores Xi values
//            dof_def stores Pi values
//            The deformable modeling paradigm computes a set of
//            Qi values for each solve cycle which are then added to
//            the Pi values to update the Xi values.
// TODO: ??? a change ntgrl_degree function

#ifndef DS_PFUNC_H
#define DS_PFUNC_H

// nested includes
//  inline functions tend to require the addition of includes in
//  .hxx files

#include "dsgauss.hxx"  // access to DS_gauss_wt  // access to DS_gauss_wt
#include "dsstdef.hxx"  // for NULL, DS_assert()  // for NULL, DS_assert()
#include "dsblmem.hxx"  // DS_copy_double_block  // DS_copy_double_block
#include "dsbasis.hxx"  // class DS_basis  // class DS_basis
                                        // DS_MAX_BASE_VAL
#include "dsstatic.hxx"
#include "dmerror.hxx"  // DM_API error numbers  // DM_API error numbers
#include "dmapinum.hxx" // enum DS_PFN // enum DS_PFN
#include "dcl_ds.h"

/**
* @file dspfunc.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup DMAPI
 *
 * @{
 */

// forward class declarations
// forward reference
class DS_eqns ;                        // holder of Ax=b, Cx=d equations
class DS_singularity_info;

// constants used in DS_pfunc

// bmt - moved to dmapinum.hxx
// DS_pfunc type_id enumerator

// DS_pfunc change state values.  used by other classes to
// communicate changes in pfunc state
#define DS_PFN_CHANGED_X      ( 1 << 0 ) // X values changed
#define DS_PFN_CHANGED_P_ON   ( 1 << 1 ) // P changed when default shape set
#define DS_PFN_CHANGED_P_OFF  ( 1 << 2 ) // P changed when default shape cleared
#define DS_PFN_CHANGED_DOMAIN ( 1 << 3 ) // domain partition changed
#define DS_PFN_CSTRN_ON       ( 1 << 4 ) // cstrn turned on
#define DS_PFN_APPROX_SHAPE   ( 1 << 5 ) // shape changed approximately
#define DS_PFN_TRACKING       ( 1 << 6 ) // cstrn tracking a parent

// enumerate the value_blocks and order for each domain dimension
/*
// tbrv
*/
/**
 * @nodoc
 */
enum DS_PFN_VAL1 { pfn_u, pfn_du, pfn_duu, pfn_duuu } ;
/*
// tbrv
*/
/**
 * @nodoc
 */
enum DS_PFN_VAL2 { pfn_uv,
                   pfn_duv,   pfn_udv,
                   pfn_duuv,  pfn_dudv,  pfn_udvv,
                   pfn_duuuv, pfn_duudv, pfn_dudvv, pfn_udvvv } ;
/*
// tbrv
*/
/**
 * @nodoc
 */
enum DS_PFN_VAL3 { pfn_uvw,
                   pfn_duvw,   pfn_udvw,   pfn_uvdw,
                   pfn_duuvw,  pfn_dudvw,  pfn_udvvw,
                   pfn_udvdw,  pfn_uvdww,  pfn_duvdw,
                   pfn_duuuvw, pfn_duudvw, pfn_duuvdw,
                   pfn_dudvvw, pfn_dudvdw, pfn_duvdww,
                   pfn_udvvvw, pfn_udvvdw, pfn_udvdww,
                   pfn_uvdwww } ;

// An enum for elem_val_flag values used by DS_pfunc methods
// Calc_val(), Calc_dcrv_val(), Calc_val_size(), Elem_value_count()
/*
// tbrv
*/
/**
 * @nodoc
 */
enum DS_EVAL{pfn_Xunknown=-1,
             pfn_X,        // 0= X only, xyz only
             pfn_Xu,       // 1= X only, xyz and 1st derivs
             pfn_Xuu,      // 2= X only, xyz and 1st and 2nd derivs
             pfn_Xuuu,     // 3= X only, xyz and 1st,2nd,3rd derivs
             pfn_XP,       // 4= X & P, xyz only
             pfn_XPu,      // 5= X & P, xyz and 1st derivs
             pfn_XPuu,     // 6= X & P, xyz and 1st and 2nd derivs
             pfn_XPuuu } ; // 7= X & P, xyz and 1st,2nd,3rd derivs

// return number of evals to be made
#define ELEM_2BASE_VAL_FLAG(elem_val_flag) (((int)elem_val_flag)%4+1)

// return 0 for X only evals and 1 for X and P evals
#define EVAL_CALC_P(elem_val_flag)         (((int)elem_val_flag)/4)

// return X only component of the DS_eval value
#define EVAL_X(elem_val_flag)   ((DS_EVAL) (((int)elem_val_flag)%4))
#define EVAL_X_COUNT(elem_val_flag)        (((int)elem_val_flag)%4)

// note: The field pfn_domain_edit is set whenever
//       Application packages may run commands which
//       edit the domain description of a DS_pfunc.  These
//       commands currently include Elevate_degree and
//       Split_elem and may include more over time.  Thus
//       enabling applications to update any related
//       application function data descriptions if required.
//       For example in ACIS, when the deformable model is committed
//       to the face, pfn_domain_edit is used to decide to either
//       replace the bs_geometry or to merely update the existing
//       bs_geometry control point locations.
//

//======================================================
// class DS_pfunc
//======================================================

/**
 * Pointer to this class acts as an handle.
 *<br>
 * <b>Role:</b> Pointers to this <tt>SDM</tt> interface class <tt>DS_pfunc</tt> should be considered as
 * handles - they should always be forward-referenced, and never dereferenced.
 * They are created, copied, and deleted by <tt>DM</tt> api's, and only used as handle (tag)
 * arguments to <tt>DM</tt> api's. For example, the functions <tt>DM_make_bspline_curve</tt> and
 * <tt>DM_make_bspline_surface</tt> will make a curve or surface <tt>DS_pfunc</tt> from arrays of knots
 * and control points. The resulting <tt>DS_pfunc</tt> is then used to make a <tt>DS_dmod</tt> with one
 * of the functions <tt>DM_make_dmod_curve</tt> or <tt>DM_make_dmod_surface</tt>; the resulting <tt>DS_dmod</tt>
 * object assumes ownership of the <tt>DS_pfunc</tt>. After completing the desired <tt>DM</tt> api calls,
 * the user must call <tt>DM_delete_dmod</tt> on the <tt>DS_dmod</tt> pointer.
 *<br><br>
 * @see DS_dmod
 */

class DECL_DM DS_pfunc : public ACIS_OBJECT            // identifier = pfn
 {
  protected:
   DS_PFN pfn_type_id ;    // derived-class identifier
   int pfn_ref_count ;     // number of references to this object

   int pfn_domain_edit;    // 0 = domain_dsc constant since creation
                           // 1 = domain_dsc changed since creation
   int pfn_domain_dim;     // [prob-size] the domain space size
   int pfn_ntgrl_degree ;  // [prob-size] accuracy: polynomials of
                           //   ntgrl_degree are integrated exactly

   int pfn_image_dim ;     // [prob-size] the projection space size
   int pfn_dof_count ;     // [prob-size] the number of Xi values
   int pfn_value_count ;   // number of basis_vals at a point loc
                           // value_count = 0 for domain_dim = 0
                           // value_count = 3 for domain_dim = 1
                           //   [Ph(u), duPh, duuPh]
                           // value_count = 6 for domain_dim = 2
                           //   [ph(uv), duPh, dvPh,
                           //    duuPh, duvPh, dvvPh]
                           // value_count = 10 for domain_dim = 3
                           //   [ph(uvw), duPh, dvPh, dwPh,
                           //    duuPh, duvPh, dvvPh,
                           //    dvwPh, dwwPh, dwuPh]
                           // value_count=1+(n+3)*n/2 (n=domain_dim)
   int pfn_default_state ; // when dof_def = 0.0 default_state = 0
                           // else default_state = 1.  The default
                           // affects how the cstrns build d in Cx=d
                           // by letting d = d-Cp when state==1.
   int pfn_interior_state; // control smoothing across C0 elem bndrys
                           // 0 = C0 between elements allowed
                           // 1 = C1 enforced between elements

   int pfn_elem_dof_count; // [prob-size] max element dof count
   int pfn_elem_gpt_count; // [prob-size] max elem gauss pt count
   int pfn_elem_count ;    // [prob-size] element count in pfunc


   double *pfn_dof_vec ;   // array of Xi values
                           // sized :[pfn_image_dim * pfn_dof_count]
                           // stored:[X0[0,1...],X1[0,1...],...]
   double *pfn_dof_def ;   // array of Pi values (default shape)
                           // Xi = Pi + Qi where (Qi = displacements)
                           // sized :[pfn_image_dim * pfn_dof_count]
                           // stored:[P0[0,1...],P1[0,1...],...]
#ifndef DS_USE_SYMEQ
#else
   int pfn_fixed_elem_count;// number of elems fixed in pfn_elem_state
   int *pfn_elem_state ;   // 0=free to move,1=fixed
                           // sized:[pfn_elem_count]
#endif //DS_USE_SYMEQ
   int *pfn_elem_dof_map ; // one element's local to global dof map
                           // sized: pfn_elem_dof_count
                           // set with Calc_elem_dof_map()
   double
      *pfn_elem_gpt_bas ;  // one elem's vals at its gauss_pts set
                           // by Calc_elem_bas_at_gpts(elem_index).
                           // 1d := [   u[pt0[dof0...dofN],pt1...
                           //       [  du[pt0[dof0...dofN],pt1...
                           //       [ duu[pt0[dof0...dofN],pt1...
                           //       [duuu[pt0[dof0...dofN],pt1...
                           // 2d := [   uv[pt0[dof0...dofN],pt1...
                           //       [  duv[pt0[dof0...dofN],pt1...
                           //       [  vdu[pt0[dof0...dofN],pt1...
                           //       [ duuv[pt0[dof0...dofN],pt1...
                           //       [ dudv[pt0[dof0...dofN],pt1...
                           //       [duuuv[pt0[dof0...dofN],pt1...
                           //       [duudv[pt0[dof0...dofN],pt1...
                           //       [dudvv[pt0[dof0...dofN],pt1...
                           //       [udvvv[pt0[dof0...dofN],pt1...
                           // dof and gpt ordering for 2d tensors
                           // glob0(0,0),glob1(0,1),...
                           // globN(1,0),...
                           // sized:[  pfn_value_count
                           //        * pfn_elem_gpt_count
                           //        * pfn_elem_dof_count]
   double
       pfn_elem_area ;     // elemi-domain-area/std-elem-domain-area
                           // current scaling value used on the
                           // standard_gauss-wgts
   double
       pfn_std_elem_area ; // std-elem-domain-area for the wgts
                           // stored in pfn_elem_gpt_wgt(std)
   double
      *pfn_elem_gpt_wgt;   // gauss weights for each gauss pt. The
                           // order of gauss pts is managed by
                           // the derived classes and corresponds
                           // to the order of gauss pts values stored
                           // in pfn_elem_gpt_bas. The weights are
                           // cached in 2 blocks, The first for
                           // internal use for elems of a standard
                           // size.  The second block for external
                           // use has weights adjusted to a
                           // particular element's area.
                           // Use Calc_elem_gpt_wgts() to build
                           // these values for a particular element
                           // sized:[2 * pfn_elem_gpt_count]

   DS_singularity_info* pfn_lo_sing_info; // stores info for making G1 constraints
   DS_singularity_info* pfn_hi_sing_info; // at hi/lo singularities

   // memory management for Calc_elem_bas_vals() calls or whatever
// kev. TSA. May 2008.
#ifdef THREAD_SAFE_ACIS
   LOCAL_VAR safe_integral_type<int>     pfn_use_count ; // count of DS_pt_cstrns in existence
   LOCAL_VAR safe_integral_type<int>     pfn_dscr_size ; // size of scratch double block
   LOCAL_VAR safe_pointer_type<double>   pfn_dscr ;      // scratch a reusable growing scratch block
   LOCAL_VAR safe_integral_type<int>     pfn_iscr_size;  // size of scratch int block
   LOCAL_VAR safe_pointer_type<int>      pfn_iscr ;      // scratch for Calc_elem_bas_vals()
#else
   LOCAL_VAR int     pfn_use_count ; // count of DS_pt_cstrns in existence
   LOCAL_VAR int     pfn_dscr_size ; // size of scratch double block
   LOCAL_VAR double *pfn_dscr ;      // scratch a reusable growing scratch block
   LOCAL_VAR int     pfn_iscr_size;  // size of scratch int block
   LOCAL_VAR int    *pfn_iscr ;      // scratch for Calc_elem_bas_vals()
#endif

  public:
/*
	  void *operator new      // for debugging
    (size_t len)           { return ( malloc(len) ) ; }

	  void operator delete    // for debugging
    (void *p)              { free (p) ; }
*/
  // Type_id, constructors, copy, Make_copy, destructor, Size_arrays
/**
 * @nodoc
 */
	  double pfn_domain_scale_kludge ;  // Temporary variable used only to pass dmod total domain scale to
                                     // DS_segment_curve_by_pfunc_isolines for 2D intersectors.

/**
 * @nodoc
 */
   DS_PFN                  // rtn: Type_id access
    Type_id()              { return(pfn_type_id) ; }
/**
 * @nodoc
 */
   int Ref_count()         // rtn: number of references to this obj
                           { return(pfn_ref_count) ; }
/**
 * @nodoc
 */
   void Increment_ref_count// eff: increase number of refs to 'this'
     ()                    { pfn_ref_count++ ; }
/**
 * @nodoc
 */
   void Decrement_ref_count// eff: decrement use_count and delete
     ()                    { DS_assert(pfn_ref_count > 0) ;
                             pfn_ref_count-- ;
                             if(pfn_ref_count == 0)
                               { ACIS_DELETE this ; }
                           }

                           // eff: default constructor
/**
 * @nodoc
 */
 DS_pfunc                // note: constructor does not load Dof_vec
    (DS_PFN type_id=ds_pfn,// in : derived-class identifier
     double *dof_vec=NULL, // in : init dof_vec values to copy or NULL
     double *dof_def=NULL, // in : init dof_def values to copy or NULL
                           //   when NULL pfn_default_state set = 0
     int image_dim=0,      // in : projection space size
     int domain_dim=0,     // in : domain space size
     int dof_count=0,      // in : number of dofs in the DS_pfunc
     int elem_count=0,     // in : number of elems in the DS_pfunc
     int elem_dof_count=0, // in : max number of dofs in one elem
     int elem_gpt_count=0, // in : max number of gpts in one elem
     int interior_state=1, // in : 0=allow internal C0 bends
                           //      1=enforce internal C1 bends
     int ntgrl_degree=     // in : accuracy: polynomials of ntgrl_degree
     DS_DEFAULT_NTGRL_DEGREE);//           are integrated exactly
/**
 * @nodoc
 */
  virtual                  // bas: DS_pfunc pure virtual function
   DS_pfunc *Build_patch   // rtn: a compatible sub-patch for 'this' object
    (double *min,          // in : lower domain point for patch
     double *max,          // in : upper domain point for patch
     int refinement)=0 ;   // in : parent_knot_spacing/child_knot_spacing
                           // note: built patch's default_state = 0
/**
 * @nodoc
 */
   DS_pfunc                // copy constructor
    (DS_pfunc &pfunc) ;
/**
 * @nodoc
 */
   virtual DS_pfunc*       // Virtual Make_copy
    Make_copy()=0 ;
/**
 * @nodoc
 */
   virtual DS_pfunc*       // Virtual Make_copy given new elem_counts
    Make_new_elem_count    // rtn: new pfunc with old pfunc's shape
    (int *elem_count,      // in : array of desired elem_counts
     int elem_count_size,  // in : elem_count array size (equal to domain_dim)
     double &max_xerr,     // out: max X err of X = P + Q
     double &max_perr)=0 ; // out: max P err in X = P + Q
/**
 * @nodoc
 */
   DS_pfunc &operator=     // assignment operator
    (DS_pfunc &pfunc) ;
/**
 * @nodoc
 */
   virtual
  ~DS_pfunc();             // Virtual destrutor
/**
 * @nodoc
 */
   void
    Size_arrays            // variable array memory management
    (int image_dim=0,      // dimension of projection space
     int dof_count=0,      // total dof count in pfunc
     int value_count=0,    // number of basis-values at one gauss_pt
     int elem_dof_count=0, // max number of dofs in one elem
     int elem_gpt_count=0, // max number of gauss_pts in one elem
     int elem_count=0) ;   // number of elements in pfunc

  // DS_pfunc simple data access for scratch memory
/**
 * @nodoc
 */
  LOCAL_PROC void
   Size_static_arrays       // eff: alloc scratch memory blocks
    (int dscr_size=0,       // desired pfn_dscr_size
     int iscr_size=0) ;     // desired pfn_iscr_size

/**
 * @nodoc
 */
 LOCAL_PROC double *Dscr()      { return pfn_dscr ; }
/**
 * @nodoc
 */
 LOCAL_PROC int     Dscr_size() { return pfn_dscr_size ; }
/**
 * @nodoc
 */
 LOCAL_PROC int    *Iscr()      { return pfn_iscr ; }
/**
 * @nodoc
 */
 LOCAL_PROC int     Iscr_size() { return pfn_iscr_size ; }

  // DS_pfunc simple data access
    // in future add elem_index arg for pfuncs with mixed elements
    // in future add domain_index for non-symmetric integration
/**
 * @nodoc
 */
   virtual DS_basis*        // note: this is a tensor product function
    Basis                   // ret: ptr to domain basis function
     (int domain_index=0)=0;// in : specifies which domain index
/**
 * @nodoc
 */
  int Is_domain_dsc_edited()// ret: 0=NO, 1=YES
                            { return pfn_domain_edit ; }

/**
 * @nodoc
 */
 void                      // eff: set domain_dsc edit state,
   Set_domain_dsc_state     //      0=constant, 1=changed
   (int domain_dsc_state)   { DS_assert (   domain_dsc_state == 0
                                         || domain_dsc_state == 1) ;
                              pfn_domain_edit = domain_dsc_state ;
                            }
/**
 * @nodoc
 */
   virtual int              // bas: DS_pfunc virtual function
    End_cond                // ret: oneof 0=open|1=closed|2=periodic
     (int domain_index=0)=0;// in : domain index(0=u,1=v)
/**
 * @nodoc
 */
   virtual void             // bas: DS_pfunc virtual function
    Set_end_cond            // eff: change end-cond values
     (int domain_index,     // in : domain index(0=u,1=v)
      int end_cond)=0;      // in : 0=open|1=closed|2=periodic
/**
 * @nodoc
 */
   virtual int
    Singular                // ret: oneof  0=none|1=low|2=high|3=both
     (int domain_index=0)=0;// in : domain index(0=u,1=v)
/**
 * @nodoc
 */
   virtual void             // bas: DS_pfunc virtual function
    Set_singular            // eff: change end-cond values
     (int domain_index,     // in : domain index(0=u,1=v)
      int singular)=0;      // in : 0=none|1=low|2=high|3=both
/**
 * @nodoc
 */
   int  Image_dim()        { return pfn_image_dim ; }
/**
 * @nodoc
 */
   int  Domain_dim()       { return pfn_domain_dim ; }
/**
 * @nodoc
 */
   int  Dof_count()        { return pfn_dof_count ; }

/**
 * @nodoc
 */
   int  Elem_count()       { return pfn_elem_count ; }
/**
 * @nodoc
 */
   int  Elem_dof_count()   { return pfn_elem_dof_count ; }
/**
 * @nodoc
 */
   int  Elem_gpt_count()   { return pfn_elem_gpt_count ; }
/**
 * @nodoc
 */
   int  Value_count()      { return pfn_value_count ; }
/**
 * @nodoc
 */
   int  Ntgrl_degree()     { return pfn_ntgrl_degree ; }
/**
 * @nodoc
 */
   int                     // ret: array-size for pfn_dof_vec or
    Dof_vec_size           //      pfn_dof_def arrays
     (int  image_dim,      // in : image dimension size
      int  dof_count)      // in : number of dofs in pfunc
                           { return (image_dim * dof_count) ; }
/**
 * @nodoc
 */
   double *Dof_vec()       // rtn: ptr to pfunc X dof_vec array
                           { return pfn_dof_vec ; }
/**
 * @nodoc
 */
   double *Dof_vec         // rtn: ptr to one pfunc Xi dof location
     (int glob_dof_index)  // in : target global dof number
                           {DS_assert(glob_dof_index < pfn_dof_count
                                  &&  glob_dof_index >= 0) ;
                            return (  pfn_dof_vec
                                     + pfn_image_dim
                                     * glob_dof_index) ;
                           }
/**
 * @nodoc
 */

   double Dof_vec          // rtn: one pfunc Xi dof value
     (int glob_dof_index,  // in : target global dof index
      int image_index)     // in : target image_dim index
                           {DS_assert(glob_dof_index < pfn_dof_count
                                  &&  glob_dof_index >= 0) ;
                            DS_assert(   image_index < pfn_image_dim
                                      && image_index >= 0) ;
                            return (  pfn_dof_vec
                                     [  pfn_image_dim*glob_dof_index
                                      + image_index]) ;
                           }
/**
 * @nodoc
 */
   double *Dof_def()       // rtn: ptr to pfunc P dof_def array
                           { return pfn_dof_def ; }
/**
 * @nodoc
 */
   double *Dof_def         // rtn: ptr to one pfunc Pi dof location
     (int glob_dof_index)  // in : target global dof number
                           {DS_assert(glob_dof_index < pfn_dof_count
                                  &&  glob_dof_index >= 0) ;
                            return (  pfn_dof_def
                                     + pfn_image_dim
                                     * glob_dof_index) ;
                           }
/**
 * @nodoc
 */
   int                     // rtn: size of gpt_loc or gpt_norm
    Graphics_pt_size       //      graphics arrays
     (int elem_pt_count,   // in : number of pts to draw per element
      int elem_count,      // in : number of elements in pfunc
      int image_dim)       // in : image dimension for pfunc
                           { return (  elem_pt_count
                                     * elem_count
                                     * image_dim) ;
                           }
/**
 * @nodoc
 */
   int  Default_state()    // rtn: 0: Dof_def = 0
                           //      1: Dof_def = default shape
                           {return pfn_default_state ; }
/**
 * @nodoc
 */
   void Set_default_shape  // eff: sets Dof_def SPAvector (default shape)
      (int shape_flag) ;   // in : 0: set Dof_def = 0
                           //      1: set Dof_def = Dof_vec
/**
 * @nodoc
 */
   int Interior_state()    // rtn: 0 = C0 between elements allowed
                           //      1 = C1 enforced between elements
                           { return pfn_interior_state ; }
/**
 * @nodoc
 */
   void Set_interior_state // eff: sets pfn_interior_state value
     (int interior_flag)   // in : 0 = C0 between elements allowed
                           //      1 = C1 enforced between elements
                           { DS_assert(   interior_flag == 0
                                       || interior_flag == 1) ;
                             pfn_interior_state = interior_flag ;
                           }
/**
 * @nodoc
 */
   void Set_dof_image_pt   // eff: sets one Xi dof_vec's image_pt
      (int dof_index,      // in : tgt control pt's index
       double *image_pt) ; // in : src image pt
/**
 * @nodoc
 */
   void Set_dof_default_pt // eff: sets one Pi dof_def's image_pt
      (int dof_index,      // in : tgt control pt's index
       double *image_pt) ; // in : src image pt

/**
 * @nodoc
 */
#ifndef DS_USE_SYMEQ
#else
  int Fixed_elem_count()   // rtn: number of fixed elems in
                           //      pfn_elem_state
                           { return pfn_fixed_elem_count ; }
/**
 * @nodoc
 */
  void Set_fixed_elem_count// set: number of fixed elems
   (int fixed_elem_count)  // in : value to store
                           { pfn_fixed_elem_count =
                                    fixed_elem_count ;
                           }
/**
 * @nodoc
 */
#endif //DS_USE_SYMEQ
  int Elem_state_size      // ret: size of pfn_elem_state
   (int elem_count)        // in : for this given elem_count
                           { return elem_count ; }

/**
 * @nodoc
 */
#ifndef DS_USE_SYMEQ
#else
  int *Elem_state()        // ret: array of element states
                           { return pfn_elem_state ; }
/**
 * @nodoc
 */
  int Elem_state           // ret: 0=free to move, 1=fixed
   (int elem_index)        // in : elem index to query
                           { DS_assert(   elem_index >= 0
                                       && elem_index < pfn_elem_count) ;
                             return(pfn_elem_state[elem_index]) ;
                           }
/**
 * @nodoc
 */
 void Init_elem_state     // eff: set all elem state values
   (int value)             // in : the value to save
                           //      0=free to move, 1= fixed
                           { DS_assert(value == 0 || value == 1) ;
                             for(int ii=0;ii<pfn_elem_count;ii++)
                               pfn_elem_state[ii]=value ;
                           }

#endif //DS_USE_SYMEQ
/**
 * @nodoc
 */
 virtual int
    Elem_dof_map_size      // ret: array-size for pfn_dof_map
    (int elem_dof_count)   // in : max dof_count per element
                           { return elem_dof_count ; }
/**
 * @nodoc
 */
   int *                   // use: after Calc_elem_dof_map(e_index)
    Elem_dof_map()         // rtn: elem_index to glob_index map array
                           { return pfn_elem_dof_map ; }
/**
 * @nodoc
 */
   int                     // use: after Calc_elem_dof_map(e_index)
    Elem_dof_map           // ret: global dof_index
     (int elem_dof_index)  // in : elem dof_index in elem=elem_index
                           {DS_assert
                            (   elem_dof_index >= 0
                             && elem_dof_index < pfn_elem_dof_count);
                             return
                               pfn_elem_dof_map[elem_dof_index] ;
                           }
/**
 * @nodoc
 */
   virtual int
   Elem_gpt_bas_size       // ret: array-size for pfn_elem_gpt_bas
    (int  value_count,     // in : basis values per sample point
     int  elem_dof_count,  // in : dofs per element
     int  elem_gpt_count)  // in : gpts per element
                           { return(  value_count
                                    * elem_dof_count
                                    * elem_gpt_count) ;
                           }
/**
 * @nodoc
 */
   int
   Elem_gpt_val_size()     // ret: val-blk size in pfn_elem_gpt_bas
                           { return(  pfn_elem_dof_count
                                    * pfn_elem_gpt_count) ;
                           }

/**
 * @nodoc
 */
   double                  // use: after Calc_elem_bas_at_gpts()
   *Elem_gpt_val_blk       // ret: val_blk ptr from pfn_elem_gpt_bas
    (int val_type)         // in : oneof(pfn_u   |pfn_du  |pfn_duu
                           //            pfn_uv  |pfn_duv |pfn_udv
                           //            pfn_duuv|pfn_dudv|pfn_udvv)
                           //            pfn_duuuv|pfn_duudv|pfn_dudvvv
                           //            pfn_udvvv
                           { return(  pfn_elem_gpt_bas
                                     + Elem_gpt_val_size()
                                     * val_type) ;
                           }
/**
 * @nodoc
 */
   double                  // use: after Calc_elem_bas_at_gpts()
   *Elem_gpt_val_pt        // ret: val_blk ptr from pfn_elem_gpt_bas
    (int val_type,         // in : oneof(pfn_u   |pfn_du  |pfn_duu
                           //            pfn_uv  |pfn_duv |pfn_udv
                           //            pfn_duuv|pfn_dudv|pfn_udvv)
                           //            pfn_duuuv|pfn_duudv|pfn_dudvvv
                           //            pfn_udvvv
     int gpt_index)        // in : gpt identifier >= 0
                           { DS_assert(   gpt_index < pfn_elem_gpt_count
                                       && gpt_index >= 0) ;
                             return(  pfn_elem_gpt_bas
                                     + (  val_type
                                        * pfn_elem_gpt_count
                                        + gpt_index)
                                     * pfn_elem_dof_count) ;
                           }
/**
 * @nodoc
 */
    int
    Elem_gpt_wgt_size      // ret: array-size for pfn_elem_gpt_wgt
    (int elem_gpt_count)   { return 2 * elem_gpt_count ; }

/**
 * @nodoc
 */
    int
    Elem_std_gpt_wgt_size  // ret: std pfn_elem_gpt_wgts cnt-size
    (int elem_gpt_count)   { return elem_gpt_count ; }


   // DS_pfunc side-effect functions
/**
 * @nodoc
 */
   virtual double const *  // use: with Calc_elem_gpt_wgts()
    Calc_elem_bas_at_gpts  // ret: basis-val blk for one elem
                           //     (pfn_elem_gpt_bas values at gpts)
    (int elem_index,       // in : element identifier >= 0
     int base_val_flag)=0; // in: 1=B 2=B,Bu 3=B,Bu,Buu 4=B,Bu,Buu,Buuu
                           // eff: build pfn_elem_gpt_bas for elem
                           // note: ret values valid for elem_index
                           //       only until next call to
                           //       Calc_elem_bas_at_gpts().


 // use: internal only ( run after
/**
 * @nodoc
 */
 virtual void            //      pfn_elem_gpt_count changes)
    Cache_elem_gpt_wgts    // eff: init pfn_elem_gpt_wgt by storing
      ()=0 ;               //      std_elem_size gpt_wgts in order
/**
 * @nodoc
 */
   double const *          // use: with Calc_elem_bas_at_gpts()
    Calc_elem_gpt_wgts     // ret: ptr to gauss_pt weight array
      (int elem_index) ;   //      needed to integrate functions of
                           //      the basis-val blk terms
                           // eff: modify gpt_wgts for elem's area
                           // note: ret values valid for elem_index
                           //       only until next call to
                           //       Calc_elem_gpt_wgts().
/**
 * @nodoc
 */
   virtual void            // bas: DS_pfunc virtual function
    Calc_elem_gpt_locs     // eff: calc array of gauss pt locations
     (int elem_index,      // in : specify the target element
      double *gpt_loc)=0 ; // out: gpt_locs=[u0..,un,v0..,vn,w0..,wn]
                           //   sized:[Elem_gpt_count()*Domain_dim()]
                           // ass: gpt_loc memory preallocated
/**
 * @nodoc
 */
   virtual int const *     // use: with Calc_elem_bas_at_gpts()
    Calc_elem_dof_map      // ret: elem dof numbers for one elem
    (int elem_index,       // in : elem identifier >= 0
     int &elem_dof_cnt)=0; // out: number of dofs in this elem
                           // note: ret values valid for elem_index
                           //       only until next call to
                           //       Calc_elem_dof_map().
/**
 * @nodoc
 */
  virtual                  // bas: DS_pfunc with an implementation
   int                     // rtn: 0=success,-1=bad dpt values
    Calc_elem_bas_vals     // eff: load array with basis elem_bas vals
     (int elem_index,      // in : which element >= 0
      int dpt_count,       // in : number of sample points
      int base_val_flag,   // in : 1=B 2=B,Bu 3=B,Bu,Buu 4=B,Bu,Buu,Buuu
      int dpt_size,        // in : amount of alloc memory in dpt
      double *dpt,         // in : [u0...,un,v0...,vn,w0...,wn]
                           //      sized:[pt_count * pfn_domain_dim]
      int val_size,        // in : amount of alloc memory in val
      double *val,         // out:2d[  uv[pt0[dof0...dofN],pt1...
                           //       [ duv[pt0[dof0...dofN],pt1...
                           //       [ udv[pt0[dof0...dofN],pt1...
                           //       [duuv[pt0[dof0...dofN],pt1...
                           //       [dudv[pt0[dof0...dofN],pt1...
                           //       [udvv[pt0[dof0...dofN],pt1...
                           //       [duuuv[pt0[dof0...dofN],pt1...
                           //       [duudv[pt0[dof0...dofN],pt1...
                           //       [dudvv[pt0[dof0...dofN],pt1...
                           //       [udvvv[pt0[dof0...dofN],pt1...
                           //     1d[   u[pt0[dof0...dofN],pt1...
                           //       [  du[pt0[dof0...dofN],pt1...
                           //       [ duu[pt0[dof0...dofN],pt1...
                           //       [duuu[pt0[dof0...dofN],pt1...
                           // sized:[Elem_bas_val_size()]
                           // ass: memory for val is pre-allocated
      int d_scratch_size,  // in : amount of alloc memory in d_scratch
      double *d_scratch,   // sized:[  basis_u->Vals_size(pt_count,DS_MAX_BASE_VAL)
                           //        + basis_v->Vals_size(pt_count,DS_MAX_BASE_VAL)]
      int i_scratch_size,  // in : amount of alloc memory in i_scratch
      int    *i_scratch)=0;// sized:[  basis_u->Span_dof_count()
                           //        + basis_v->Span_dof_count()]
/**
 * @nodoc
 */
  virtual                  // bas: DS_pfunc with an implementation
   int                     // rtn: 0=success,-1=bad dpt values
   Calc_vals               // eff: calc and store disp and def vals
    (int dpt_count,        // in : number of evaluation points
     double *dpt,          // in : [u0...,un,v0...,vn,w0...,wn]
                           //      sized:[dpt_count * pfn_domain_dim]
     DS_EVAL elem_val_flag,// in : specify how many answers to calc
                           //      with  X   = Q + P
                           //      (or total = displacement + default)
                           //  pfn_X    = X only, xyz only
                           //  pfn_Xu   = X only, xyz and 1st derivs
                           //  pfn_Xuu  = X only, xyz and 1st and 2nd derivs
                           //  pfn_Xuuu = X only, xyz and 1st,2nd,3rd derivs
                           //  pfn_XP   = X & P, xyz only
                           //  pfn_XPu  = X & P, xyz and 1st derivs
                           //  pfn_XPuu = X & P, xyz and 1st and 2nd derivs
                           //  pfn_XPuuu= X & P, xyz and 1st,2nd,3rd derivs
     int val_size,         // in : amount of alloc memory in val
     double *val) ;        // out: example for elem_val_flag == pfn_XPuu where
                           //      SPAposition = displacement + default
                           // 1d= displacement_xyz     [pt0..,ptN],
                           //                 dxyz/du  [pt0..,ptN],
                           //                d2xyz/duu [pt0..,ptN],
                           //          default_xyz     [pt0..,ptN],
                           //                 dxyz/du  [pt0..,ptN],
                           //                d2xyz/duu [pt0..,ptN] ]
                           // 2d= displacement_xyz     [pt0..,ptN],
                           //                 dxyz/du  [pt0..,ptN],
                           //                 dxyz/dv  [pt0..,ptN],
                           //                d2xyz/duu [pt0..,ptN],
                           //                d2xyz/duv [pt0..,ptN],
                           //                d2xyz/dvv [pt0..,ptN],
                           //          default_xyz     [pt0..,ptN],
                           //                 dxyz/du  [pt0..,ptN],
                           //                 dxyz/dv  [pt0..,ptN],
                           //                d2xyz/duu [pt0..,ptN],
                           //                d2xyz/duv [pt0..,ptN],
                           //                d2xyz/dvv [pt0..,ptN] ]
                           // sized:[Calc_val_size(dpt_cnt,elem_val_flag)]
                           // ass: memory for val is pre-allocated
/**
 * @nodoc
 */
   int
    Calc_val_size          // rtn: Calc_vals() val memory size
    (int dpt_count,        // in : number of evaluation points
     DS_EVAL elem_val_flag)// in : specify how many answers to calc
                           //      with  X   = Q + P
                           //      (or total = displacement + default)
                           //  pfn_X    = X only, xyz only
                           //  pfn_Xu   = X only, xyz and 1st derivs
                           //  pfn_Xuu  = X only, xyz and 1st and 2nd derivs
                           //  pfn_Xuuu = X only, xyz and 1st,2nd,3rd derivs
                           //  pfn_XP   = X & P, xyz only
                           //  pfn_XPu  = X & P, xyz and 1st derivs
                           //  pfn_XPuu = X & P, xyz and 1st and 2nd derivs
                           //  pfn_XPuuu= X & P, xyz and 1st,2nd,3rd derivs
                           { return
                                (  pfn_image_dim
                                 * dpt_count
                                 * Elem_value_count(pfn_domain_dim,
                                                    elem_val_flag)) ;
                           }

/**
 * @nodoc
 */
  virtual void             // bas: class DS_pfunc virtual function
   Calc_gpt_graphics       // eff: calc gauss pt locs and norms
    (double *gpt_loc,      // out: all elem gpt locs
                           //      Sized:[pfunc->Graphics_pt_size()]
     double *gpt_norm)=0 ; // out: all elem gpt norms
                           //      Sized:[pfunc->Graphics_pt_size()]
/**
 * @nodoc
 */
  virtual void             // bas: class DS_pfunc virtual function
   Calc_curv_comb          // eff: calc curvature comb pt locs and norms
  (int elem_pt_count,      // in : number of pts per elem
   DS_pfunc *proj_pfunc,   // in : Project uv curve to xyz for plotting.
                           //      Must be NULL for 3D curves.
   int torque_flag,        // in : 1=gen torsion (binorm) data,0=don't
   int log_flag,           // in : 1=scale norm and binorm vecs by
                           //      log(curvature) and log(torsion)
                           //      0=scale by curvature and torsion
                           //        values
   double *gpt_loc,        // out: all elem gpt locs
                           //      Sized:[pfunc->Graphics_pt_size()+image_dim]
   double *gpt_norm,       // out: all elem gpt norms
                           //      Sized:[pfunc->Graphics_pt_size()+image_dim]
   double *gpt_binorm) ;   // out: all elem gpt binorms
                           //      Sized:[pfunc->Graphics_pt_size()+image_dim]
/**
 * @nodoc
 */
  virtual                  // bas: class DS_pfunc virtual function
   int                     // rtn: 0=success, -1=bad_dpts
   Split_elem              // eff: split the domain rep of the pfunc
     (int elem_index,      // in : tgt elem to be split
      int split_flag=0,    // use: optional derived class control
                           //      not used for 1d DS_pfuncs, for 2d
                           //      0=split_u,1=split_v,2=split_both
      double *dpt=NULL)=0; // use: optional derived class location
                           //      Sets field pfn_domain_edit.
/**
 * @nodoc
 */
  virtual                  // bas: class DS_pfunc virtual function
   double                  // rtn: err=Ntgrl((W(uv)-W*(uv))**2)duv
    Elevate_degree         // eff: inc pfunc elem polynomial degrees by one
     (int cont_flag=0)=0 ; // in : 0 = preserve current elem continuity
                           //      1 = increment elem continuity
                           // note: preserving continuity allows the
                           //       new shape to exactly form the old
                           // note: Sets field pfn_domain_edit.
                           // bas: class DS_pfunc virtual function
/**
 * @nodoc
 */
 virtual
  double                   // rtn: err=Ntgrl((W(uv)-W*(uv))**2)duv
   Set_degree              // eff: inc pfunc elem polynomial degrees by one
   (int& rtn_flag,         // out: 0=success, -1=new degree<3
   const int* new_degree)=0;// in : desired degree
/**
 * @nodoc
 */
  virtual double           // rtn: approx_shape error
   Extrapolate_domain      // eff: increase domain by 5% and adjust
     ()=0 ;                //      image so shape over orig domain
                           //      stays constant.
/**
 * @nodoc
 */
  virtual void             // bas: DS_pfunc pure virtual function
   Shift_domain            // eff: shift the basis domain values
    (double *du)=0 ;       // in : domain shift SPAvector, Sized:Domain_dim()
/**
 * @nodoc
 */
  virtual                  // bas: DS_pfunc pure virtual function
   int                     // rtn: 0=done,-1=scale too small
   Scale_domain            // eff: scale the basis domain values
    (double scale)=0 ;     // in : domain scale factor
/**
 * @nodoc
 */
  void
   Shift_image             // eff: shift the image values
    (double *dW) ;         // in : image shift SPAvector, sized:Image_dim()
/**
 * @nodoc
 */

 void
  Scale_image              // eff: scale the image values
  (double *scale_vec) ;    // in : scale factor for each dimension
                           //      not used when NULL.
/**
 * @nodoc
 */
 int                      // rtn: 0=success, -1=bad_tgt_domain
                           // eff: set the 'this' dofs to best
   Approx_shape            //      approx the shape of src_pfunc
    (DS_pfunc *src_pfunc,  // in : The shape to approximate. The
                           //      src_domain must be == tgt_domain
     DS_pfunc *            // in : opt projection when src is a dcrv
        proj_pfunc,        //      if NULL use src_pfunc->Calc_vals()
                           //      else proj_pfunc->Calc_dcrv_vals
                           //                            (src_pfunc)
     int iter_flag,        // in : 0=no iter,1=iter to satisfy tgt_max_xerr     double *domain_min,   // in : optional bounding region or NULL
     double *domain_max,   // in : optional bounding region or NULL
     double *domain_min,   // in : optional bounding region or NULL
     double tgt_max_xerr,  // in : defines error limit (0=no error check)
     double &total_xerr,   // out: xerr = Ntgrl(W(u)-W'(u))**2)du
     double &total_perr,   // out: perr = Ntgrl(P(u)-P'(u))**2)du
     double &max_xerr,     // out: max dist between W(u) and W'(u)
     double &max_perr,     // out: max dist between P(u) and P'(u)
     int &xerr_elem_index, // out: largest contributor to total_xerr
     int &perr_elem_index);// out: largest contributor to total_perr
                           // note: pfn_dof_def values are only set when
                           //       pfn_default_state == 1

  // DS_pfunc non-side-effect functions
/**
 * @nodoc
 */
   int                     // rtn: 1=yes,0=no
    Is_iso_parameter       // eff: check all dofs for constant values
     (int *iso_index) ;    // out: iso_index[ii] = 1 for iso values
                           //                    = 0 for not
                           //      sized:[image_dim]
/**
 * @nodoc
 */
   int                     // ret: dof_index or -1
    Find_cpt_by_iline      // eff: A geometric pick based on the
                           //       line pi = pi0 + u (pi1-pi0)
     (double *pi0,         // in : image space pt, Sized:[image_dim]
      double *pi1,         // in : image space pt, Sized:[image_dim]
      double max_dist,     // in : max dist in which to find cstrn
      double &dist2,       // out: dist**2 to iline from load
      double &uu) ;        // out: iline param of pt closest to cstrn

/*// obsolete code
   int                     // ret: count of pos, 1st, and 2nd derivs
    Calc_value_count       //      vals for domain_dim and elem_val_flag
    (int domain_dim,       // in : function's domain dimension
     DS_EVAL elem_val_flag)// in : specify how many answers to calc
                           //      with X = Q + P
                           //      (or total = displacement + default)
                           //  pfn_X    = X only, xyz only
                           //  pfn_Xu   = X only, xyz and 1st derivs
                           //  pfn_Xuu  = X only, xyz and 1st and 2nd derivs
                           //  pfn_Xuuu = X only, xyz and 1st,2nd,3rd derivs
                           //  pfn_XP   = X & P, xyz only
                           //  pfn_XPu  = X & P, xyz and 1st derivs
                           //  pfn_XPuu = X & P, xyz and 1st and 2nd derivs
                           //  pfn_XPuuu= X & P, xyz and 1st,2nd,3rd derivs
                           {
                             int d = domain_dim ;
                             int f = EVAL_TYPE_COUNT(elem_val_flag) ;
                             int g = 1 + EVAL_P_TYPE(elem_val_flag) ;
                             return
                             (  (  1                           // SPAposition
                                 + (f >= 1 ?  d : 0)           // 1st ds
                                 + (f >= 2 ?((d+1)*d)/2 : 0)   // 2nd ds
                                 + (f >= 3 ?(2+3*(d-1)*d)/2:0))// 3rd ds
                              * g ) ;                          // X&P or X only
                           }
*/ // end obsolete code
/**
 * @nodoc
 */
   int                     // ret: num of pos, 1st,2nd,3rd derivs
    Elem_value_count       //      vals for domain_dim & elem_val_flag
    (int domain_dim,       // in : function's domain dimension
     DS_EVAL elem_val_flag)// in : specify how many answers to calc
                           //      with X = Q + P
                           //      (or total = displacement + default)
                           //  pfn_X    = X only, xyz only
                           //  pfn_Xu   = X only, xyz and 1st derivs
                           //  pfn_Xuu  = X only, xyz and 1st and 2nd derivs
                           //  pfn_Xuuu = X only, xyz and 1st,2nd,3rd derivs
                           //  pfn_XP   = X & P, xyz only
                           //  pfn_XPu  = X & P, xyz and 1st derivs
                           //  pfn_XPuu = X & P, xyz and 1st and 2nd derivs
                           //  pfn_XPuuu= X & P, xyz and 1st,2nd,3rd derivs
                           { int d = domain_dim ;
                             int f = EVAL_X_COUNT(elem_val_flag) ;
                             int g = 1 + EVAL_CALC_P(elem_val_flag) ;
                             return
                             (  (  1                           // SPAposition
                                 + (f >= 1 ?  d : 0)           // 1st ds
                                 + (f >= 2 ?((d+1)*d)/2 : 0)   // 2nd ds
                                 + (f >= 3 ?(2+3*(d-1)*d)/2:0))// 3rd ds
                              * g ) ;                          // X&P or X only
                           }

/**
 * @nodoc
 */
   int                     // ret: num of pos, 1st,2nd,3rd derivs
    Base_value_count       //      vals for domain_dim & base_val_flag
    (int domain_dim,       // in : function's domain dimension
     int base_val_flag)    // in : specify how many answers to calc
                           //      1=B 2=B,Bu 3=B,Bu,Buu 4=B,Bu,Buu,Buuu
                           // out: f |1d|2d
                           //      ---------
                           //      1 | 1| 1 uv
                           //      2 | 2| 3 duv udv
                           //      3 | 3| 6 duuv dudv udvv
                           //      4 | 4|10 duuuv duudv dudvv udvvv
                           { int d = domain_dim ;
                             int f = base_val_flag ;
                             return
                             ( 1                            // SPAposition
                              +(f >= 2 ?  d : 0)            // 1st ds
                              +(f >= 3 ?((d+1)*d)/2 : 0)    // 2nd ds
                              +(f >= 4 ?(2+3*(d-1)*d)/2:0));// 3rd ds
                           }
/**
 * @nodoc
 */
   virtual int             // bas: DS_pfunc virtual function
    Calc_elem_gpt_count    // ret: number of gauss_pts in one elem
     (int ntgrl_degree)=0; // in : the polynomial degree that can
                           //      be integrated exactly
/**
 * @nodoc
 */
   virtual double          // bas: DS_pfunc virtual function
    Calc_elem_area         // ret: area of elem_index
     (int elem_index)=0 ;  // in : specifies which element
/**
 * @nodoc
 */
   int                     // use: with Calc_elem_bas_vals()
    Elem_bas_val_size      // ret:Calc_elem_bas_vals() val size
    (int pt_count,         // in : number val points in one elem]
     int base_val_flag)    // in : specify what will be stored.
                           //     1=B 2=B,Bu 3=B,Bu,Buu 4=B,Bu,Buu,Buuu
                           { return
                              (  Base_value_count(pfn_domain_dim,
                                                  base_val_flag)
                               * pt_count
                               * pfn_elem_dof_count) ;
                           }
/**
 * @nodoc
 */
   virtual int             // bas: a DS_pfunc virtual function
    Elem_bas_scratch_size  // ret: Calc_elem_bas_vals() scratch size
     (int pt_count)=0;     // in : number of pts to eval in elem
/**
 * @nodoc
 */
   virtual int             // bas: a DS_pfunc virtual function
    Elem_bas_iscratch_size // ret: Calc_elem_bas_vals() iscratch size
     ()=0 ;
/**
 * @nodoc
 */
   double *                // use: with Calc_elem_bas_vals()
    Elem_pt_bas_val        // ret: ptr to [dof0,dof1,..dofN] basis
                           //      values for pt_index and val_type
     (double *val,         // in : Calc_elem_bas_vals() return val
      int val_type,        // in : pfn_ u |du |duu   or
                           //      pfn_ uv|duv|udv|duuv|dudv|udvv
      int pt_count,        // in : number of points evaluated in val
      int pt_index)        // in : specify pt in question
                           { DS_assert(   pt_index >= 0
                                       && pt_index < pt_count) ;
                             return(  val
                                    + (val_type*pt_count + pt_index)
                                    *  pfn_elem_dof_count) ;
                           }

/**
 * @nodoc
 */
 virtual int              // bas: DS_pfunc pure virtual function
   Dpt_2elem_index         // ret: elem_index containing dpt or -1
  (double *dpt,            // in : domain loc Sized:[Domain_dim()]
   int knot_flag=-1)=0 ;   // in : Specify rtn span when u==knot_val
                           //      0 = low span
                           //      1 = high span
                           //     -1 = don't use - for compatability
/**
 * @nodoc
 */
  virtual                  // def: written for tensor product shapes
  int                      // rtn: 0=success,-1=non NULL input ptr,-2=not a tprod
   Dbox_2elem_list         // eff: find all contained elem indices
    (double *min_pt,       // in : lower-left SPAbox corner
     double *max_pt,       // in : upper-right SPAbox corner
     int &elem_count,      // out: size of elem_list
     int *&elem_list) ;    // out: list of contained elem indices
                           //      must be NULL on entry.
/**
 * @nodoc
 */
  virtual                  // def: written for tensor product shapes
  int                      // rtn: 0=success,-1=non NULL input ptr,-2=not a tprod
   Dbox_2elem_touched_list
                           // eff: find all touched elem indices
    (double *min_pt,       // in : lower-left SPAbox corner
     double *max_pt,       // in : upper-right SPAbox corner
     int &elem_count,      // out: size of elem_list
     int *&elem_list) ;    // out: list of contained elem indices
                           //      must be NULL on entry.
/**
 * @nodoc
 */
  virtual                 // def: written for all pfunc shapes
  int                      // rtn: 0=success or neg err flag
   Elem_list_2total_dof_list// eff: map elem indices to dof indices
    (int elem_count,       // in : size of elem_list
     int *elem_list,       // in : list of elem indices to check
     int &dof_count,       // out: size of dof_list
     int *&dof_list) ;     // out: list of contained dof indices
                           //      must be NULL on entry
/**
 * @nodoc
 */
  virtual                 // def: written for all pfunc shapes
  int                      // rtn: 0=success or neg err flag
   Elem_list_2local_dof_list// eff: map elem indices to dof indices
    (int elem_count,       // in : size of elem_list
     int *elem_list,       // in : list of elem indices to check
     int &dof_count,       // out: size of dof_list
     int *&dof_list) ;     // out: list of contained dof indices
                           //      must be NULL on entry
/**
 * @nodoc
 */
  virtual                  // def: written for consecutive elem numbering
  int                      // rtn: 0=success,-1=non NULL input ptr
   Compliment_elem_list    // eff: make set=all_elems-elem_list
    (int elem_count,       // in : size of elem_list
     int *elem_list,       // in : list of elem indices
     int &com_elem_count,  // out: size of com_elem_list
     int *&com_elem_list); // out: compliment list of input elem_list
                           //      malloced
/**
 * @nodoc
 */
#ifndef DS_USE_SYMEQ
#else
  virtual                  // def: written for consecutive dof numbering
  int                      // rtn: 0=success,-1=non NULL input ptr
   Compliment_dof_list     // eff: make set=all_dofs-dof_list
    (int dof_count,        // in : size of dof_list
     int *dof_list,        // in : list of dof indices
     int &com_dof_count,   // out: size of com_dof_list
     int *&com_dof_list) ; // out: compliment list of input dof_list
                           //      malloced
#endif //DS_USE_SYMEQ
/**
 * @nodoc
 */

  virtual void             // bas: DS_pfunc pure virtual function
   Domain_min              // eff: store lowest corner of the min
    (double *dpt)=0 ;      // out:   sized domain bounding SPAbox.
                           //     dpt Sized:[domain_dim] on input
/**
 * @nodoc
 */
  virtual void             // bas: DS_pfunc pure virtual function
   Domain_max              // eff: store upper corner of the min
    (double *dpt)=0 ;      // out:   sized domain bounding SPAbox.
                           //     dpt Sized:[domain_dim] on input
/**
 * @nodoc
 */
  virtual double           // rtn: total domain area
   Domain_area             // eff: returns rectangular area in
    ()                     //      domain_min to domain_max
                           //      derived classes may specialize as
                           //      needed.
                           { DS_assert(pfn_domain_dim <= 3) ;
                             double min[3], max[3], area = 1.0 ;
                             Domain_min(min) ;
                             Domain_max(max) ;
                             for(int ii=0;ii<pfn_domain_dim;ii++)
                               { area *= max[ii] - min[ii] ; }
                             return area ;
                           }
/**
 * @nodoc
 */
  virtual void             // bas: DS_pfunc virtual function
   Elem_domain_min         // eff: store lowest corner of the min
    (int elem_index,       // in : elem in question's identifier
     double *dpt)=0 ;      // out:   sized domain bounding SPAbox.
                           //     dpt Sized:[domain_dim] on input
/**
 * @nodoc
 */
  virtual void             // bas: DS_pfunc virtual function
   Elem_domain_max         // eff: store upper corner of the min
    (int elem_index,       // in : element in question's identifier
     double *dpt)=0 ;      // out:   sized domain bounding SPAbox.
                           //     dpt Sized:[domain_dim] on input
/**
 * @nodoc
 */
  virtual                  // bas: DS_pfunc virtual function
   double                  // rtn: shortest pfunc elem span
    Min_span_length()=0 ;  // eff: search every elem span
/**
 * @nodoc
 */
  virtual void             // bas: DS_pfunc virtual function
   Image_bound             // eff: find image-space bounding SPAbox for pfunc
    (double *min,          // out: bounding-SPAbox min, sized:[pfn_image_dim]
     double *max) ;        // out: bounding-SPAbox max, sized:[pfn_image_dim]
                           // default behavior - rtn dof's bounding-SPAbox
/**
 * @nodoc
 */
  virtual int              // ret: total Cx=d row count in rep
   Cd_line_row_count()=0 ; // inf: NURB cstrns for closed & periodic
/**
 * @nodoc
 */
  virtual void             // eff: build pfunc's Cx=d eqn rows as
   Build_Cd_row            //      needed for closed and periodic
    (DS_eqns *eqns,        // in : holder of Ax=b, Cx=d eqns
     int &next_Cd_row)=0 ; // i/o: 1st available row in Cx=d
/**
 * @nodoc
 */
  virtual void             // eff: build pfunc's Cx=d eqn rows as
   Build_d_row             //      needed for closed and periodic
    (DS_eqns *eqns,        // in : holder of Ax=b, Cx=d eqns
     int &next_Cd_row)=0 ; // i/o: 1st available row in Cx=d

  // DS_pfunc functions that operate on domain curves

/**
 * @nodoc
 */
                           // bas: DS_pfunc virtual function
   virtual double *        // ret: array of segment u boundary vals
    Calc_dcrv_segments     // eff: Segment src_pfunc at xsects with
                           //      elem_domain_bndries and src_knots
     (DS_pfunc *src_pfunc, // in : defines the domain curve to xsect
      int *&xsect_cont,    // out: continuity at every 'this' xsect bndry
      int &seg_count)=0 ;  // out: number of segments returned
                           //      return array Sized:[seg_count+1]
                           // mallocs: both xsect_cont and return arrays

   // TODO:??? Something when the domain curve is not defined by a
   //       pfunc, but rather by some user defined function.
/**
 * @nodoc
 */
  virtual                  // bas: DS_pfunc virtual function
   double *                // use: called by Calc_dcrv_segments()
    Calc_uv_line_xsects    // eff: rtn array of xsects with uv-lines
     (DS_pfunc *surf_pfunc,// in : The surface shape being constrained
      int u_count,         // in : number of constant u curves
      double *uu,          // in : array of uu values
      int *cont_u,         // in : u continuity (-1=curve_bound,0=C0,1=C1..)
      int v_count,         // in : number of constant v curves
      double *vv,          // in : array of vv values
      int *cont_v,         // in : v continuity (-1=curve_bound,0=C0,1=C1..)
      int *&xsect_cont,    // out: continuity at each xsect
      int &seg_count)=0 ;  // out: number of segments returned
                           //      return array Sized:[seg_count+1]
                           // mallocs: both xsect_cont and return arrays
/**
 * @nodoc
 */
  virtual                  // bas: DS_pfunc virtual function
   int                     // rtn: 1=found an xsect, 0=didn't
    Calc_iline_xsect       // eff: rtn 1st xsect with iline
                           //      iline = p0 + u(p1-p0)
    (double *p0,           // in : image space pt, Sized:[image_dim]
     double *p1,           // in : image space pt, Sized:[image_dim]
     double max_dist,      // in : max_dist for near misses
     double *image_pt,     // out: image-loc for nearest xsect
                           //      sized:[image_dim]
     double &iline_u,      // out: iline param val for nearest xsect
     double *pfunc_uv)=0 ; // out: pfunc param val for nearest xsect
                           //      sized:[domain_dim]
/**
 * @nodoc
 */
   DS_pfunc *              // ret: proj of src_pfunc or NULL
    Calc_dcrv_image        // eff: project domain_crv to image space
     (DS_pfunc *src_pfunc, // in : defines the domain curve to proj
      int min_degree,      // in : min degree polynomial for output curve
      double err_tol) ;    // in : defines the accuracy of the output
                           //      curve to the actual projection
                           //      err_tol >= dist_between(W(C(s)),W(s))
                           //      The max dist between the projection
                           //      curve and any projected points.
/**
 * @nodoc
 */
   int                     // rtn: 0=success, -1=bad_dpts
                           // eff: load array with src_pfunc pt vals
    Calc_dcrv_vals         //      projected to image P and X spaces
    (int src_pt_count,     // in : number of evaluation points
     double *src_pt,       // in : 1d = [u0,u1,..un]
                           //      2d = [u0...,un,v0...,vn,w0...,wn]
                           //      sized:[src_pt_count*src_domain_dim]
     DS_pfunc *src_pfunc,  // in : the domain_crv being projected
     DS_EVAL elem_val_flag,// in : specify how many answers to calc
                           //      with  X   = Q + P
                           //      (or total = displacement + default)
                           //  pfn_X    = X only, xyz only
                           //  pfn_Xu   = X only, xyz and 1st derivs
                           //  pfn_Xuu  = X only, xyz and 1st and 2nd derivs
                           //  pfn_Xuuu = X only, xyz and 1st,2nd,3rd derivs
                           //  pfn_XP   = X & P, xyz only
                           //  pfn_XPu  = X & P, xyz and 1st derivs
                           //  pfn_XPuu = X & P, xyz and 1st and 2nd derivs
                           //  pfn_XPuuu= X & P, xyz and 1st,2nd,3rd derivs
     int val_size,         // in : amount of alloc memory in val
     double *val) ;        // out: for ex. elem_val_flag == pfn_XPuu where
                           //      SPAposition = displacement + default
                           // 1d= displacement_xyz     [pt0..,ptN],
                           //                 dxyz/du  [pt0..,ptN],
                           //                d2xyz/duu [pt0..,ptN],
                           //          default_xyz     [pt0..,ptN],
                           //                 dxyz/du  [pt0..,ptN],
                           //                d2xyz/duu [pt0..,ptN] ]
                           // 2d= displacement_xyz     [pt0..,ptN],
                           //                 dxyz/du  [pt0..,ptN],
                           //                 dxyz/dv  [pt0..,ptN],
                           //                d2xyz/duu [pt0..,ptN],
                           //                d2xyz/duv [pt0..,ptN],
                           //                d2xyz/dvv [pt0..,ptN],
                           //          default_xyz     [pt0..,ptN],
                           //                 dxyz/du  [pt0..,ptN],
                           //                 dxyz/dv  [pt0..,ptN],
                           //                d2xyz/duu [pt0..,ptN],
                           //                d2xyz/duv [pt0..,ptN],
                           //                d2xyz/dvv [pt0..,ptN] ]
                           // sized:[Calc_val_size(dpt_cnt,elem_val_flag)]
                           // ass: memory for val is pre-allocated

} ; // end class DS_pfunc

//======================================================
// Global DS_pfunc aware functions
//======================================================
/**
 * @nodoc
 */
double *                   // rtn: ordered knot-crossing param values
DS_call_segment_curve_by_pfunc_isolines// eff: x-sect(pcurve,knot-iso-param-lines)
  (void *src_data,         // in : contains curve and param_range data
                           //      (secretly a DS_crv2acis ptr)
   DS_pfunc *surf_pfunc,   // in : DShusk surface shape description
   int knot_count_u,       // in : number of unique u-knot values
   double *knot_u,         // in : array of unique  u-knot values
   int *cont_u,            // in : continuity at knot_u boundary
   int knot_count_v,       // in : number of unique v-knot values
   double *knot_v,         // in : array of unique  v-knot values
   int *cont_v,            // in : continuity at knot_v boundary
   int *&xsect_cont,       // out: continuity of each seg_bnd xsect
   int &seg_count) ;       // out: size of the returned array - 1
                           // mallocs: both xsect_cont and return arrays

//======================================================
// class DS_basis_dsc
//======================================================

// section  : DStprod.hxx
// created  : Jan 18, 1995
// author   : GW Celniker
// synopsis : The abstract class of DS_tprod implements surfaces
//            which are constructed from curve basis functions
//            as a tensor product surface. This class enables
//            a 1D basis function implementation to be turned
//            into any dimensional object.

//            if a curve W(u) is given by
//              W(u)   = Sum( Xi * PHi(u))
//               where Xi  = the Degrees of Freedom of the curve
//                     PHi = the curve basis functions
//                     u   = parametric variable with defined
//                            valid domain u_min <= u < u_max
//            then a tensor product surface can be given as
//              W(u,v) = Sum(Xij * PHi(u) * Phj(v))
//               where v   = another parametric variable with
//                           a valid domain v_min <= v < v_max
// indexing : A DS_tprod element is made from the cross product
// elems,     of each curve's spans with the spans of the other
// gauss_pts, curves.  A 1D DS_tprod uses its only basis function
// and dofs.  to specify its numbering for gauss_pts, dofs, and
//            spans.  Higher order DS_tprod functions derive their
//            numbering from their constituent basis functions
//            as a simple outer product as shown for the 3D case
//            below. DS_tprod elems are numbered consecutively
//            as [e0[0,0,0], e1[0,0,1], ... ek[0,1,0] ... e[l,m,n]]
//            where [i,j,k] = span_i dim0 basis
//                            span_j dim1 basis   (the highest dim
//                            span_k dim2 basis    varies fastest)
//            The gauss pts of a DS_tprod elem are generated
//            as the cross product of the gauss pts of each span
//            used in the elem.  Like elem numbering, gauss_pt
//            numbering is sequenced as [gpt0[0,0,0]...gpt[l,m,n]
//            where [i,j,k] = gauss_pt_i dim0 span
//                            gauss_pt_j dim1 span  (the highest dim
//                            gauss_pt_k dim2 span   varies fastest)
//            The dofs of a DS_tprod elem are generated as the cross
//            product of the curve dofs.  These are numbered
//            consecutively as [dof0[0,0,0],[dof1[0,0,1],...dofn[i,j,k]
//            where [i,j,k] = dof_i dim0 curve
//                            dof_j dim1 curve  (the highest dim
//                            dof_k dim2 curve   varies fastest)

//======================================================
// class DS_basis_dsc
//======================================================
/**
 * @nodoc
 */
class DECL_DM DS_basis_dsc         // identifier = dsc
{                          // convenience class used to organize
friend class DS_tprod ;    // summary information about basis
friend class DS_tprod_1d ; // for use by DS_tprod and DS_rprod only
friend class DS_tprod_2d ;
friend class DS_rprod_1d ;
friend class DS_rprod_2d ;


  protected:               // descriptions for DS_tprod objects
                           // no inheritance, so no type_id

   DS_basis *dsc_basis ;   // ptr to the basis function
   int dsc_span_count ;    // [array-size] span_count in basis curve
   int dsc_span_gpt_count; // [array-size] gpt_count  per span
   int dsc_span_dof_count; // [array-size] dof_count  per span
                           // span_size = (  span_gpt_count
                           //              * span_dof_count)
                           // note: there are 3 value_blks per span
   double *                // one span's gauss_pt locs
       dsc_span_gpt_loc ;  //      sized:[dsc_span_gpt_count]
   int    *dsc_span_map ;  // one span's local to global dof map
                           //      sized:[dsc_span_dof_count]
   double *dsc_span_val ;  // array of basis values and derivatives
                           // at a set of gauss pts in each span
                           // of the "used" DS_basis.
                           // sized: [Span_val_size() =
                           //     (  dsc_span_count (variable)
                           //       * value_count = DS_MAX_BASE_VAL
                           //       * dsc_span_gpt_count (variable)
                           //       * dsc_span_dof_count (variable))]
                           // ordered: [span0_blk, span1_blk,...]
                           //  span_blk  =
                           //     [  u[gpt0[dof0...dofN],gpt1...
                           //       du[gpt0[dof0...dofN],gpt1...
                           //      duu[gpt0[dof0...dofN],gpt1...]
                           //     duuu[gpt0[dof0...dofN],gpt1...]

   // constructor, copy, destructor, Size_arrays
   DS_basis_dsc            // constructor
    (DS_basis *basis=NULL, // in : "used" DS_basis being described
     int span_gpt_count=0);// in : gauss_pt per span for integration
                           // note: The basis ptr value is stored.
                           //       The calling program should not
                           //       continue to use basis. It will
                           //       be deleted along with its pfunc.

   DS_basis_dsc            // copy constructor
    (DS_basis_dsc &basis_dsc) ;

   DS_basis_dsc &operator= // assignment operator
    (DS_basis_dsc &) ;

   virtual
  ~DS_basis_dsc()          // destructor
                           { if(dsc_basis) ACIS_DELETE dsc_basis ;
                             dsc_basis = NULL ;
                             Size_arrays() ;
                           }

   void Size_arrays        // Size_arrays and store size_indicators
    (int span_count=0,     // in : number of spans in basis curve
     int span_gpt_count=0, // in : number of gauss-pts per span
     int span_dof_count=0);// in : number of dofs per span

   // simple DS_basis_dsc data and size-variable access
   int Span_count()        {return (dsc_span_count); }
   int Span_gpt_count()    {return (dsc_span_gpt_count) ; }
   int Span_dof_count()    {return (dsc_span_dof_count) ; }
   int Value_size()        {return (  dsc_span_gpt_count
                                    * dsc_span_dof_count) ;
                           }
   int Span_size           // rtn: one span's val storage blk size
    (int base_val_flag)    // in : val count saved (B,Bu,Buu,Buuu)
                           {return (  base_val_flag
                                    * Value_size()) ; }
   int Span_gpt_size
    (int span_gpt_count)   {return (span_gpt_count) ; }
   int Span_map_size
    (int span_dof_count)   {return (span_dof_count) ; }
   int Span_val_size       // rtn: one curve's val storage blk size
    (int span_count,       // in : number of spans in curve
     int base_val_flag,    // in : val count saved (B,Bu,Buu,Buuu)
     int span_gpt_count,   // in : number of gauss pts per span
     int span_dof_count)   // in : number of dofs per span
                           {return (  span_count
                                    * base_val_flag
                                    * span_gpt_count
                                    * span_dof_count) ;
                           }

   // return pointers to various value_blks within dsc_span_val
   // note: the offsets in these ptr access functions are
   // computed assuming that DS_MAX_BASE_VAL values have
   // been cached for each gauss point.
   double *Span_val(int span_index)
                       {DS_assert(   span_index >= 0
                                  && span_index < dsc_span_count);
                        return(  dsc_span_val
                               + (DS_MAX_BASE_VAL*span_index  )
                               * dsc_span_gpt_count
                               * dsc_span_dof_count) ; }
   double *Span_uval(int span_index)
                       {DS_assert(   span_index >= 0
                                  && span_index < dsc_span_count);
                        return(  dsc_span_val
                               + (DS_MAX_BASE_VAL*span_index  )
                               * dsc_span_gpt_count
                               * dsc_span_dof_count) ; }
   double *Span_duval(int span_index)
                       {DS_assert(   span_index >= 0
                                  && span_index < dsc_span_count);
                        return(  dsc_span_val
                               + (DS_MAX_BASE_VAL*span_index+1)
                               * dsc_span_gpt_count
                               * dsc_span_dof_count) ; }
   double *Span_duuval(int span_index)
                       {DS_assert(   span_index >= 0
                                  && span_index < dsc_span_count);
                        return(  dsc_span_val
                               + (DS_MAX_BASE_VAL*span_index+2)
                               * dsc_span_gpt_count
                               * dsc_span_dof_count) ; }
   double *Span_duuuval(int span_index)
                       {DS_assert(   span_index >= 0
                                  && span_index < dsc_span_count);
                        return(  dsc_span_val
                               + (DS_MAX_BASE_VAL*span_index+3)
                               * dsc_span_gpt_count
                               * dsc_span_dof_count) ; }

   double *Span_gpt_locs() // rtn: gpt_locs set by Set_span_gpt_locs()
                           { return dsc_span_gpt_loc ; }

   // DS_basis_dsc side-effect functions
   void
    Shift_domain           // eff: shift the basis domain values
    (double du) ;          // in : domain shift amount

   int                     // rtn: 0=done,-1=scale too small
    Scale_domain           // eff: shift the basis domain values
    (double gain) ;        // in : domain scale factor

   void                    // eff: Change the description's
    Set_basis              // eff:    basis function
    (DS_basis *basis,      // in : basis function to describe
     int span_gpt_count);  // gauss_pt per span for integration

   void Eval_basis() ;     // load dsc_span_val with basis-vals
                           // for every span

   void Set_span_gpt_locs  // load dsc_span_gpt_loc for one span
    (int span_index) ;     // in : specifies which span

   void Set_span_dof_map   // load dsc_span_map dofs for one span
    (int span_index)       // in : specifies which span
                           { DS_assert(   span_index >= 0
                                       && span_index < dsc_span_count);
                             dsc_basis->Set_span_dof_map
                                               (span_index,
                                                dsc_span_map) ;
                           }

} ; // end class DS_basis_dsc

//======================================================
// class DS_tprod_1d
//======================================================
/**
 * @nodoc
 */
class DECL_DM DS_tprod_1d : public DS_pfunc
{                          // identifier = tp1
  protected:

   // summary description data of each basis
   DS_basis_dsc tp1_basis_dsc ; // basis function for curve
   int          tp1_end_cond ;  // oneof 0=open|1=closed|2=periodic

  public:
/*
	  void *operator new      // for debugging
    (size_t len)           { return ( malloc(len) ) ; }

	  void operator delete    // for debugging
    (void *p)              { free (p) ; }
*/
   // constructor, copy, destructor

  DS_tprod_1d             // default constructor
   (DS_basis *basis,       // in : defining basis function
    double *dof_vec=NULL,  // in : init dof_vec vals to copy or NULL
    double *dof_def=NULL,  // in : init dof_def vals to copy or NULL
    int image_dim=0,       // in : projection space size
    int interior_state=1,  // in : 0=allow C0 internal bends
                           //      1=enforce C1 internal bends
    int ntgrl_degree=      // in : degree of polynomials functions
    DS_DEFAULT_NTGRL_DEGREE,//       exactly integrated
    int end_cond=0) ;      // in : oneof 0=open|1=closed|2=periodic
                           // note: basis is stored in tprod_1d.
                           //       It will be deleted along with
                           //       tprod_1d and should no longer
                           //       be used by the calling program.

  virtual                  // bas: DS_pfunc pure virtual function
   DS_pfunc *Build_patch   // rtn: a compatable sub-patch for 'this' object
    (double *min,          // in : lower domain point for patch
     double *max,          // in : upper domain point for patch
     int refinement) ;     // in : parent_knot_spacing/child_knot_spacing
                           // note: built patch's default_state = 0

   DS_tprod_1d             // copy constructor
    (DS_tprod_1d &tprod_1d) ;

   DS_tprod_1d &operator=  // assignment operator
    (DS_tprod_1d &) ;

   DS_pfunc*               // Make_copy, Virtual DS_pfunc function
    Make_copy()            { return( ACIS_NEW DS_tprod_1d(*this)) ; }


   virtual DS_pfunc*       // Virtual Make_copy given new elem_counts
    Make_new_elem_count    // rtn: new pfunc with old pfunc's shape
    (int *elem_count,      // in : array of desired elem_counts
     int elem_count_size,  // in : elem_count array size (equal to domain_dim)
     double &max_xerr,     // out: max X err of X = P + Q
     double &max_perr) ;   // out: max P err in X = P + Q

   virtual
  ~DS_tprod_1d()           // bas: destructor, DS_pfunc virtual function
                           // note: deletes "contained" DS_basis_dsc
                           {
                             // stop freeing of un-allocated memory
                             // when pfn_elem_dof_count == 0 delete
                             // is not called for pfn_elem_dof_map
                             // and pfn_elem_gpt_bas
                             pfn_elem_dof_count = 0 ;
                             pfn_elem_dof_map   = NULL ;
                             pfn_elem_gpt_bas   = NULL ;
                           }

   // simple data access
   DS_basis_dsc *Basis_dsc() {return &tp1_basis_dsc ; }

   // instantiations of DS_pfunc virtual simple data access functions

   // GWCelniker note; The element and the basis gpt_bas values
   // are one and the same for the DS_tprod_1d surface.  Tight
   // loop compute time efficiencies can be had by moving the
   // DS_pfunc pointers around the DS_basis_dsc values instead
   // of copying those values.
   //
   // So, DS_tprod_1d overloads the DS_pfunc variable-sized
   // array size functions to make sure they are never used
   // to allocate or deallocate memory.
   //
                           // bas: DS_pfunc virtual function
   virtual DS_basis*       // note: this is a tensor product function
    Basis                  // ret: ptr to domain basis function
     (int domain_index=0)  // in : specifies which domain index
                           { return (  domain_index == 0
                                     ? tp1_basis_dsc.dsc_basis
                                     : NULL ) ;
                           }

   virtual int             // bas: DS_pfunc virtual function
    End_cond               // ret: oneof 0=open|1=closed|2=periodic
     (int domain_index=0);  // in : domain index(0=u)

   virtual void            // bas: DS_pfunc virtual function
    Set_end_cond           // eff: change end-cond values
    (int domain_index,     // in : domain index(0=u)
     int end_cond);         // in : 0=open|1=closed|2=periodic

   virtual int             // bas: DS_pfunc virtual function
    Singular               // ret: oneof 0=none|1=lo|2=hi|3=both
     (int) //domain_index) // in : domain index(0=u)
                           { return(0) ;
                           }

   virtual void            // bas: DS_pfunc virtual function
    Set_singular           // eff: no change - curves aren't singular
     (int, // domain_index,// in : domain index(0=u)
      int) // singular)    // in : 0=none|1=low|2=high|3=both
                           { }

   int                      // bas: DS_pfunc virtual function
    Elem_dof_map_size       // ret: array-size for pfn_dof_map
    (int) //elem_dof_count) // in : max dof_count per element
                            {
                             return( 0 ) ;
                            }
   int                      // bas: DS_pfunc virtual function
    Elem_gpt_bas_size       // ret: array-size for pfn_elem_gpt_bas
    (int /*value_count*/,   // in : basis values per sample point
     int /*elem_dof_count*/,// in : max dofs per element
     int /*elem_gpt_count*/)// in : max gauss-points per element
                            {
                              return( 0 ) ;
                            }

   // DS_tprod_1d side-effect functions
   // instantiations of DS_pfunc virtual side-effect functions

   double const *          // bas: DS_pfunc virtual function
   Calc_elem_bas_at_gpts   // ret: basis-val blk for one elem
                           //     (pfn_elem_gpt_bas values at gpt)
     (int elem_index,      // in : which element >= 0
      int )                // in : 1=B 2=B,Bu 3=B,Bu,Buu 4=B,Bu,Buu,Buuu
                           // Note: the returned array values remain
                           //   valid for elem_index only until the
                           //   next Calc_elem_bas_at_gpts() call
                           { DS_assert(   elem_index >= 0
                                       && elem_index < pfn_elem_count);
                             // set pfn_elem_gpt_bas ptr to span val
                             pfn_elem_gpt_bas =
                                tp1_basis_dsc.Span_val(elem_index) ;
                             // return pfn_elem_gpt_bas ptr
                             return pfn_elem_gpt_bas ;
                           }

                           // use: internal only ( run after
   virtual void            //      pfn_elem_gpt_count changes)
    Cache_elem_gpt_wgts    // eff: init pfn_elem_gpt_wgt by storing
      () ;                 //      std_elem_size gpt_wgts in order

   virtual void            // bas: DS_pfunc virtual function
    Calc_elem_gpt_locs     // eff: calc array of gauss pt locations
     (int elem_index,      // in : specify the target element
      double *gpt_loc)     // out: gpt_locs=[u0..,un,v0..,vn,w0..,wn]
                           //     sized:[Elem_gpt_count()*1]
                           // ass: gpt_loc memory preallocated
                           {
                             DS_assert(   elem_index >= 0
                                       && elem_index < Elem_count());
                             tp1_basis_dsc.Set_span_gpt_locs
                                                   (elem_index) ;
                             DS_copy_double_block
                                (gpt_loc,
                                 tp1_basis_dsc.dsc_span_gpt_loc,
                                 tp1_basis_dsc.dsc_span_gpt_count) ;
                           }

  int const *              // bas: DS_pfunc virtual function
   Calc_elem_dof_map       // ret: elem dof number sequence
    (int elem_index,       // in : elem identifier >= 0
     int &elem_dof_count); // out: number of dofs in this elem
                           // note: the returned array values remain
                           //       valid for elem_index only until
                           //       the next Calc_elem_dof_map() call

  virtual                  // bas: DS_pfunc with an implementation
   int                     // rtn: 0=success,-1=bad dpt values
    Calc_elem_bas_vals     // eff: load an array with elem_bas values
     (int elem_index,      // in : which element >= 0
      int dpt_count,       // in : number of sample points
      int base_val_flag,   // in : 1=B 2=B,Bu 3=B,Bu,Buu 4=B,Bu,Buu,Buuu
      int dpt_size,        // in : amount of alloc memory in dpt
      double *dpt,         // in : [u0,u1,...,un]
                           //      sized:[pt_count]
      int val_size,        // in : amount of alloc memory in val
      double *val,         // out:= [   u[pt0[dof0...dofN],pt1...
                           //       [  du[pt0[dof0...dofN],pt1...
                           //       [ duu[pt0[dof0...dofN],pt1...
                           //       [duuu[pt0[dof0...dofN],pt1...
                           // sized:[Elem_bas_val_size()]
                           // ass: memory for val is pre-allocated
      int, //d_scratch_size// in : amount of alloc memory in d_scratch
      double* /*d_scratch*/,// not-used in this virtual func instance
      int i_scratch_size,  // in : amount of alloc memory in i_scratch
      int *i_scratch) ;    // sized:[Elem_bas_iscratch_size()]

  virtual void             // bas: class DS_pfunc virtual function
   Calc_gpt_graphics       // eff: calc gauss pt locs and norms
    (double *gpt_loc,      // out: all elem gpt locs
                           //      Sized:[pfunc->Graphics_pt_size()]
     double *gpt_norm) ;   // out: all elem gpt norms
                           //      Sized:[pfunc->Graphics_pt_size()]

  virtual                  // bas: class DS_pfunc virtual function
   int                     // rtn: 0=success, -1=bad_dpts
   Split_elem              // eff: split the domain rep of the pfunc
     (int elem_index,      // in : tgt elem to be split
      int split_flag=0,    // in : not used by DS_tprod_1d
      double *dpt=NULL) ;  // use: if NULL, elem split in half, else
                           //      elem split at uu = *dpt
                           //      Sets field pfn_domain_edit.

  virtual                  // bas: class DS_pfunc virtual function
   double                  // rtn: err=Ntgrl((W(uv)-W*(uv))**2)duv
    Elevate_degree         // eff: inc pfunc elem polynomial degrees by one
     (int cont_flag=0) ;   // in : 0 = preserve current elem continuity
                           //      1 = increment elem continuity
                           // note: preserving continuity allows the
                           //       new shape to exactly form the old
                           // note: Sets field pfn_domain_edit.
                           // bas: class DS_pfunc virtual function

  virtual
  double                   // rtn: err=Ntgrl((W(uv)-W*(uv))**2)duv
   Set_degree              // eff: inc pfunc elem polynomial degrees by one
    (int& rtn_flag,        // out: 0=success, -1=new degree<3
    const int* new_degree);// in : desired degree

  virtual double           // rtn: approx_shape error
   Extrapolate_domain      // eff: increase domain by 5% and adjust
     () ;                  //      image so shape over orig domain
                           //      stays constant.

  virtual void             // bas: DS_pfunc pure virtual function
   Shift_domain            // eff: shift the basis domain values
    (double *du) ;         // in : domain shift SPAvector, Sized:Domain_dim()

  virtual                  // bas: DS_pfunc pure virtual function
   int                     // rtn: 0=done,-1=scale too small
   Scale_domain            // eff: scale the basis domain values
    (double scale) ;       // in : domain scale factor


   // DS_tprod_1d non-side-effect functions
   // instantiations of DS_pfunc non-side-effect functions

   int                     // bas: DS_pfunc virtual function
    Calc_elem_gpt_count    // ret: number of gauss_pts in one elem
     (int ntgrl_degree)    // in : min polynomial degree that can
                           //      be integrated exactly
                           {
                             // Span and elem gpt_cnts are the same
                             return (DS_linear_gauss_pt_count
                                                (ntgrl_degree)) ;
                           }

   double                  // bas: DS_pfunc virtual function
    Calc_elem_area         // ret: area of elem_index
     (int elem_index)      // in : specifies which element
                           { DS_assert(   elem_index >= 0
                                       && elem_index < pfn_elem_count);
                             return tp1_basis_dsc.dsc_basis->
                                         Span_length(elem_index) ;
                           }

   virtual int             // bas: a DS_pfunc virtual function
    Elem_bas_scratch_size  // ret: Calc_elem_bas_vals() scratch size
     (int /*pt_count*/)    // in : number of pts to eval in elem
                           { return 0 ; }

   virtual int             // bas: a DS_pfunc virtual function
    Elem_bas_iscratch_size // ret: Calc_elem_bas_vals() iscratch size
     ()                    { return tp1_basis_dsc.dsc_basis->
                                                Span_dof_count() ;
                           }


  virtual int              // bas: DS_pfunc virtual function
   Dpt_2elem_index         // ret: elem_index containing dpt or -1
  (double *dpt,            // in : domain loc Sized:[Domain_dim()]
   int knot_flag=-1) ;     // in : Specify rtn span when u==knot_val
                           //      0 = low span
                           //      1 = high span
                           //     -1 = don't use - for compatability

  virtual void             // bas: DS_pfunc virtual function
   Domain_min              // eff: store lowest corner of the min
    (double *dpt)          // out:   sized domain bounding SPAbox.
                           //     dpt Sized:[domain_dim] on input
                           { dpt[0] =
                              tp1_basis_dsc.dsc_basis->Span_umin(0) ;
                           }

  virtual void             // bas: DS_pfunc virtual function
   Domain_max              // eff: store upper corner of the min
    (double *dpt)          // out:   sized domain bounding SPAbox.
                           //     dpt Sized:[domain_dim] on input
                           { dpt[0] =
                              tp1_basis_dsc.dsc_basis->Span_umax
                                 (tp1_basis_dsc.dsc_basis->Span_count()-1) ;
                           }

  virtual void             // bas: DS_pfunc virtual function
   Elem_domain_min         // eff: store lowest corner of the min
    (int elem_index,       // in : elem in question's identifier
     double *dpt)          // out:   sized domain bounding SPAbox.
                           //     dpt Sized:[domain_dim] on input
                           { DS_assert(   elem_index >= 0
                                       && elem_index < pfn_elem_count);
                             dpt[0] =
                              tp1_basis_dsc.dsc_basis->Span_umin
                                       (elem_index) ;
                           }

  virtual void             // bas: DS_pfunc virtual function
   Elem_domain_max         // eff: store upper corner of the min
    (int elem_index,       // in : elem in question's identifier
     double *dpt)          // out:   sized domain bounding SPAbox.
                           //     dpt Sized:[domain_dim] on input
                           { DS_assert(   elem_index >= 0
                                       && elem_index < pfn_elem_count);
                             dpt[0] =
                              tp1_basis_dsc.dsc_basis->Span_umax
                                       (elem_index) ;
                           }

  virtual                  // bas: DS_pfunc virtual function
   double                  // rtn: shortest pfunc elem span
    Min_span_length()      // eff: search every elem span
                           { return(tp1_basis_dsc.dsc_basis->
                                              Min_span_length() ) ;
                           }

  virtual int              // ret: total Cx=d row count in rep
   Cd_line_row_count() ;   // inf: NURB cstrns for closed & periodic

  virtual void             // eff: build pfunc's Cx=d eqn rows as
   Build_Cd_row            //      needed for closed and periodic
    (DS_eqns *eqns,        // in : holder of Ax=b, Cx=d eqns
     int &next_Cd_row) ;   // i/o: 1st available row in Cx=d

  virtual void             // eff: build pfunc's Cx=d eqn rows as
   Build_d_row             //      needed for closed and periodic
    (DS_eqns *eqns,        // in : holder of Ax=b, Cx=d eqns
     int &next_Cd_row) ;   // i/o: 1st available row in Cx=d

  // DS_tprod_1d implementations of
  // DS_pfunc functions that operate on domain curves

                           // bas: DS_pfunc virtual function
   virtual double *        // ret: array of segment u boundary vals
    Calc_dcrv_segments     // eff: Segment src_pfunc at xsects with
                           //      elem_domain_bndries and src_knots
     (DS_pfunc *src_pfunc, // in :  defines domain crv to xsect
      int *&xsect_cont,    // out: continuity at every 'this' xsect bndry
      int &seg_count) ;    // out: number of segments returned
                           //      return array Sized:[seg_count+1]
                           // mallocs: both xsect_cont and return arrays

  virtual                  // bas: DS_pfunc virtual function
   double *                // use: called by Calc_dcrv_segments()
    Calc_uv_line_xsects    // eff: rtn array of xsects with uv-lines
     (DS_pfunc *surf_pfunc,// in : The surface shape being constrained
      int u_count,         // in : number of constant u curves
      double *uu,          // in : array of uu values
      int *cont_u,         // in : u continuity (-1=curve_bound,0=C0,1=C1..)
      int v_count,         // in : number of constant v curves
      double *vv,          // in : array of vv values
      int *cont_v,         // in : v continuity (-1=curve_bound,0=C0,1=C1..)
      int *&xsect_cont,    // out: continuity at each xsect
      int &seg_count) ;    // out: number of segments returned
                           //      return array Sized:[seg_count+1]
                           // mallocs: both xsect_cont and return arrays

  virtual                  // bas: DS_pfunc virtual function
   int                     // rtn: 1=found an xsect, 0=didn't
    Calc_iline_xsect       // eff: rtn 1st xsect with iline
                           //      iline = p0 + u(p1-p0)
    (double *p0,           // in : image space pt, Sized:[image_dim]
     double *p1,           // in : image space pt, Sized:[image_dim]
     double max_dist,      // in : max_dist for near misses
     double *image_pt,     // out: image-loc for nearest xsect
                           //      sized:[image_dim]
     double &iline_u,      // out: iline param val for nearest xsect
     double *pfunc_uv) ;   // out: pfunc param val for nearest xsect
                           //      sized:[domain_dim]

} ; // end class DS_tprod_1d

//======================================================
// class DS_tprod_2d
//======================================================

/**
 * @nodoc
 */
class DECL_DM DS_tprod_2d : public DS_pfunc
{
  protected:

   // summary description data of each basis
   DS_basis_dsc  tp2_basis_dsc_u ;
   int tp2_end_cond_u ;    // oneof 0=open|1=closed|2=periodic
   int tp2_singular_u ;    // oneof 0=none|1=low|2=high|3=both

   DS_basis_dsc  tp2_basis_dsc_v ;
   int tp2_end_cond_v ;    // oneof 0=open|1=closed|2=periodic
   int tp2_singular_v ;    // oneof 0=none|1=low|2=high|3=both

  public:
/*
	  void *operator new      // for debugging
    (size_t len)           { return ( malloc(len) ) ; }

	  void operator delete    // for debugging
    (void *p)              { free (p) ; }
*/
  // DS_tprod_2d constructor, copy, destructor

 DS_tprod_2d              // default constructor
   (DS_basis *basis_u=NULL,// in : defining u dir basis function
    DS_basis *basis_v=NULL,// in : defining v dir basis function
    double *dof_vec=NULL,  // in : init dof_vec vals to copy or NULL
    double *dof_def=NULL,  // in : init dof_def vals to copy or NULL
    int image_dim=0,       // in : projection space size
    int interior_state=1,  // in : 0=allow C0 internal bends
                           //      1=enforce C1 internal bends
    int ntgrl_degree=      // in : Gauss Integration accuracy
    DS_DEFAULT_NTGRL_DEGREE,
    int end_cond_u=0,      // in : oneof 0=open|1=closed|2=periodic
    int singular_u=0,      // in : oneof 0=none|1=low|2=high|3=both
    int end_cond_v=0,      // in : oneof 0=open|1=closed|2=periodic
    int singular_v=0) ;    // in : oneof 0=none|1=low|2=high|3=both
                           // note: bases are stored in tprod_2d.
                           //       They will be deleted along with
                           //       tprod_2d and should no longer
                           //       be used by the calling program.

  virtual                  // bas: DS_pfunc pure virtual function
   DS_pfunc *Build_patch   // rtn: a compatable sub-patch for 'this' object
    (double *min,          // in : lower domain point for patch
     double *max,          // in : upper domain point for patch
     int refinement) ;     // in : parent_knot_spacing/child_knot_spacing
                           // note: built patch's default_state = 0

   DS_tprod_2d             // copy constructor
    (DS_tprod_2d &tprod_2d) ;

   virtual DS_pfunc*       // Virtual Make_copy given new elem_counts
    Make_new_elem_count    // rtn: new pfunc with old pfunc's shape
    (int *elem_count,      // in : array of desired elem_counts
     int elem_count_size,  // in : elem_count array size (equal to domain_dim)
     double &max_xerr,     // out: max X err of X = P + Q
     double &max_perr) ;   // out: max P err in X = P + Q

   DS_tprod_2d &operator=  // eff: assignment operator
    (DS_tprod_2d &) ;

   DS_pfunc*               // eff: Virtual DS_pfunc copy function
    Make_copy()            { return( new DS_tprod_2d(*this)) ; }

   virtual
  ~DS_tprod_2d() { }       // eff: destructor, DS_pfunc virtual function
                           // note: deletes "contained" DS_basis_dsc

   // DS_tprod_2d Simple data access functions
   // Instantiations of DS_pfunc virtual functions

   // note: no need to overload DS_pfunc virtual data access
   //       functions Elem_dof_map_size() and
   //                 Elem_gpt_bas_size()
   //       their default behavior is just fine

   virtual DS_basis*       // bas: DS_pfunc virtual function
    Basis                  // ret: ptr to domain basis function
     (int domain_index=0)  // in : domain index(0=u,1=v)
                           { DS_assert(domain_index == 0 || domain_index == 1) ;
                             return(  domain_index == 0 ? tp2_basis_dsc_u.dsc_basis
                                    : domain_index == 1 ? tp2_basis_dsc_v.dsc_basis
                                    : NULL ) ;
                           }

   virtual int             // bas: DS_pfunc virtual function
    End_cond               // ret: oneof 0=open|1=closed|2=periodic
     (int domain_index=0);  // in : domain index(0=u,1=v)

   virtual void            // bas: DS_pfunc virtual function
    Set_end_cond           // eff: change end-cond values
     (int domain_index,    // in : domain index(0=u,1=v)
      int end_cond);        // in : 0=open|1=closed|2=periodic

   virtual int             // bas: DS_pfunc virtual function
    Singular               // ret: oneof  0=none|1=low|2=high|3=both
     (int domain_index=0)  // in : domain index(0=u,1=v)
                           { return(  domain_index == 0 ? tp2_singular_u
                                    : domain_index == 1 ? tp2_singular_v
                                    : -1 ) ;
                           }

   virtual void            // bas: DS_pfunc virtual function
    Set_singular           // eff: change end-cond values
     (int domain_index,    // in : domain index(0=u,1=v)
      int singular)        // in : 0=none|1=low|2=high|3=both
                           { if(   domain_index >= 0
                                && domain_index <= 1)
                               { DS_assert(   singular >= 0
                                           && singular <=3) ;
                                 if(domain_index == 0)
                                   tp2_singular_u = singular ;
                                 else
                                   tp2_singular_v = singular ;
                               }
                           }

   // DS_tprod_2d side-effect-functions
   // instantiations of DS_pfunc virtual side-effect functions

   double const *          // bas: DS_pfunc virtual function
    Calc_elem_bas_at_gpts  // ret: basis-val blk for one elem
                           //     (pfn_elem_gpt_bas values at gpt)
     (int elem_index,      // in : which element >= 0
      int base_val_flag) ; // in : 1=B, 2=B,Bu 3=B,Bu,Buu 4=B,Bu,Buu,Buuu
                           // Note: the returned array values remain
                           //   valid for elem_index only until the
                           //   next Calc_elem_bas_at_gpts() call

                           // use: internal only ( run after
   virtual void            //      pfn_elem_gpt_count changes)
    Cache_elem_gpt_wgts    // eff: init pfn_elem_gpt_wgt by storing
      () ;                 //      std_elem_size gpt_wgts in order


   virtual void            // bas: DS_pfunc virtual function
    Calc_elem_gpt_locs     // eff: calc array of gauss pt locations
     (int elem_index,      // in : specify the target element
      double *gpt_loc) ;   // out: gpt_locs=[u0..,un,v0..,vn,w0..,wn]
                           //     sized:[Elem_gpt_count()*2]
                           // ass: gpt_loc memory preallocated

   int const *             // bas: DS_pfunc virtual function
    Calc_elem_dof_map      // ret: elem dof numbers for one elem
    (int elem_index,       // in : elem identifier >= 0
     int &elem_dof_count); // out: number of dofs in this elem
                           // note: the returned array values remain
                           //       valid for elem_index only until
                           //       the next Calc_elem_dof_map() call

  virtual                  // bas: DS_pfunc with an implementation
   int                     // rtn: 0=success,-1=bad dpt values
    Calc_elem_bas_vals     // eff: load an array with elem_bas vals
     (int elem_index,      // in : which element >= 0
      int dpt_count,       // in : number of sample points
      int base_val_flag,   // in : 1=B 2=B,Bu 3=B,Bu,Buu 4=B,Bu,Buu,Buuu
      int dpt_size,        // in : amount of alloc memory in dpt
      double *dpt,         // in : [u0,u1,...,un,v0,v2,...,vn]
                           //      sized:[pt_count * 2]
      int val_size,        // in : amount of alloc memory in val
      double *val,         // out:= [   uv[pt0[dof0...dofN],pt1...
                           //       [  duv[pt0[dof0...dofN],pt1...
                           //       [  udv[pt0[dof0...dofN],pt1...
                           //       [ duuv[pt0[dof0...dofN],pt1...
                           //       [ dudv[pt0[dof0...dofN],pt1...
                           //       [ udvv[pt0[dof0...dofN],pt1...
                           //       [duuuv[pt0[dof0...dofN],pt1...
                           //       [duudv[pt0[dof0...dofN],pt1...
                           //       [dudvv[pt0[dof0...dofN],pt1...
                           //       [udvvv[pt0[dof0...dofN],pt1...
                           // sized:[Elem_bas_val_size()]
                           // ass: memory for val is pre-allocated
      int d_scratch_size,  // in : amount of alloc memory in d_scratch
      double *d_scratch,   // sized:[  basis_u->Vals_size(pt_count)
                           //        + basis_v->Vals_size(pt_count)]
      int i_scratch_size,  // in : amount of alloc memory in i_scratch
      int    *i_scratch) ; // sized:[  basis_u->Span_dof_count()
                           //        + basis_v->Span_dof_count()]

  virtual void             // bas: class DS_pfunc virtual function
   Calc_gpt_graphics       // eff: calc gauss pt locs and norms
    (double *gpt_loc,      // out: all elem gpt locs
                           //      Sized:[pfunc->Graphics_pt_size()]
     double *gpt_norm) ;   // out: all elem gpt norms
                           //      Sized:[pfunc->Graphics_pt_size()]

  virtual                  // bas: class DS_pfunc virtual function
   int                     // rtn: 0=success, -1=bad_dpts
   Split_elem              // eff: split the domain rep of the pfunc
     (int elem_index,      // in : tgt elem to be split
      int split_flag=2,    // use: 0=split_u,1=split_v,2=split_both
      double *dpt=NULL) ;  // use: if NULL elem split in half else
                           //      elem split at uv = *dpt
                           //      Sets field pfn_domain_edit.

  virtual                  // bas: class DS_pfunc virtual function
   double                  // rtn: err=Ntgrl((W(uv)-W*(uv))**2)duv
    Elevate_degree         // eff: inc pfunc elem polynomial degrees by one
     (int cont_flag=0) ;   // in : 0 = preserve current elem continuity
                           //      1 = increment elem continuity
                           // note: preserving continuity allows the
                           //       new shape to exactly form the old
                           // note: Sets field pfn_domain_edit.

  virtual
  double                   // rtn: err=Ntgrl((W(uv)-W*(uv))**2)duv
   Set_degree              // eff: inc pfunc elem polynomial degrees by one
   (int& rtn_flag,         // out: 0=success, -1=new degree<3
    const int* new_degree);// in : desired degree

  virtual double           // rtn: approx_shape error
   Extrapolate_domain      // eff: increase domain by 5% and adjust
     () ;                  //      image so shape over orig domain
                           //      stays constant.

  virtual void             // bas: DS_pfunc pure virtual function
   Shift_domain            // eff: shift the basis domain values
    (double *du) ;         // in : domain shift SPAvector, Sized:Domain_dim()

  virtual                  // bas: DS_pfunc pure virtual function
   int                     // rtn: 0=done,-1=scale too small
   Scale_domain            // eff: scale the basis domain values
    (double scale) ;       // in : domain scale factor

   // DS_tprod_2d non-side-effect functions
   void
    Elem_2span_index       // convert elem index into span indicies
     (int elem_index,      // in : element index to convert
      int &spanu_index,    // out: basis_u span index
      int &spanv_index)    // out: basis_v span index
                           { DS_assert(   elem_index >= 0
                                       && elem_index < pfn_elem_count);
                             // spanv_index varies fastest
                             spanu_index =
                              (  elem_index
                               / tp2_basis_dsc_v.dsc_span_count) ;
                             spanv_index =
                              (  elem_index
                               - spanu_index
                               * tp2_basis_dsc_v.dsc_span_count) ;
                           }
   int
    Span_2elem_index       // ret: elem_index for span_indicies
     (int spanu_index,     // in : basis_u identifier >= 0
      int spanv_index)     // in : basis_v identifier >= 0
                           { DS_assert
                             (   spanu_index >= 0
                              && spanu_index
                                  < tp2_basis_dsc_u.dsc_span_count
                              && spanv_index >= 0
                              && spanv_index
                                  < tp2_basis_dsc_v.dsc_span_count);
                             return(  tp2_basis_dsc_v.dsc_span_count
                                    * spanu_index
                                    + spanv_index) ;
                           }
   void
    Elem_2span_gpt_index   // set: span-gpt-indicies for elem-gpt-index
     (int elem_gpt_index,  // in : elem gpt identifier >= 0
      int &spanu_gpt_index,// out: basis_u span-gpt-index
      int &spanv_gpt_index)// out: basis_v span-gpt-index
                           { DS_assert
                              (   elem_gpt_index < pfn_elem_gpt_count
                               && elem_gpt_index >= 0) ;
                             // spanv_gpt_index varies the fastest
                             spanu_gpt_index =
                              (  elem_gpt_index
                               / tp2_basis_dsc_v.dsc_span_gpt_count);
                             spanv_gpt_index =
                              (  elem_gpt_index
                               - spanu_gpt_index
                               * tp2_basis_dsc_v.dsc_span_gpt_count);
                           }
   int
    Span_2elem_gpt_index   // ret: elem_gpt_index for span_gpt_indicies
     (int spanu_gpt_index, // in : basis_u gpt identifier >= 0
      int spanv_gpt_index) // in : basis_v gpt identifier >= 0
                           { DS_assert
                              (   spanu_gpt_index >= 0
                               && spanu_gpt_index
                                  < tp2_basis_dsc_u.dsc_span_gpt_count
                               && spanv_gpt_index >= 0
                               && spanv_gpt_index
                                  < tp2_basis_dsc_v.dsc_span_gpt_count);
                             return
                               (  tp2_basis_dsc_v.dsc_span_gpt_count
                                * spanu_gpt_index
                                + spanv_gpt_index) ;
                           }

   // DS_tprod_2d instantiations of DS_pfunc virtual
   // non-side-effect functions

   virtual int             // bas: DS_pfunc virtual function
    Calc_elem_gpt_count    // ret: number of gauss_pts in one elem
     (int ntgrl_degree)    // in : the polynomial degree that can be
                           //      integrated exactly

                           { // elem_gpt_count is the product of
                             // the u and v span gpt_counts
                             int size = DS_linear_gauss_pt_count
                                                     (ntgrl_degree) ;
                             return( size * size ) ;
                           }

   virtual double          // bas: DS_pfunc virtual function
    Calc_elem_area         // ret: area of elem_index
     (int elem_index)      // in : specifies which element
                           { DS_assert(   elem_index >= 0
                                       && elem_index < pfn_elem_count);
                             int spanu_index, spanv_index ;
                             Elem_2span_index(elem_index,
                                              spanu_index,
                                              spanv_index);
                             return(  tp2_basis_dsc_u.dsc_basis->
                                           Span_length(spanu_index)
                                    * tp2_basis_dsc_v.dsc_basis->
                                           Span_length(spanv_index));
                           }

   virtual int             // bas: a DS_pfunc virtual function
    Elem_bas_scratch_size  // ret: Calc_elem_bas_vals() scratch size
     (int pt_count)        // in : number of pts to eval in elem
                           { return (  tp2_basis_dsc_u.dsc_basis->
                                            Vals_size(pt_count,
                                                      DS_MAX_BASE_VAL)
                                     + tp2_basis_dsc_v.dsc_basis->
                                            Vals_size(pt_count,
                                                      DS_MAX_BASE_VAL)) ;
                           }

   virtual int             // bas: a DS_pfunc virtual function
    Elem_bas_iscratch_size// ret: Calc_elem_bas_vals() iscratch size
     ()                    { return (  tp2_basis_dsc_u.dsc_basis->
                                            Span_dof_count()
                                     + tp2_basis_dsc_v.dsc_basis->
                                            Span_dof_count()) ;
                           }

  virtual int              // bas: DS_pfunc virtual function
    Dpt_2elem_index        // ret: elem_index containing dpt or -1
  (double *dpt,            // in : domain loc Sized:[Domain_dim()]
   int knot_flag=-1) ;     // in : Specify rtn span when u==knot_val
                           //      0 = low span
                           //      1 = high span
                           //     -1 = don't use - for compatability

  virtual void             // bas: DS_pfunc virtual function
   Domain_min              // eff: store lowest corner of the min
    (double *dpt)          // out:   sized domain bounding SPAbox.
                           //     dpt Sized:[domain_dim] on input
                           { dpt[0] =
                              tp2_basis_dsc_u.dsc_basis->Span_umin(0) ;
                             dpt[1] =
                              tp2_basis_dsc_v.dsc_basis->Span_umin(0) ;
                           }

  virtual void             // bas: DS_pfunc virtual function
   Domain_max              // eff: store upper corner of the min
    (double *dpt)          // out:   sized domain bounding SPAbox.
                           //     dpt Sized:[domain_dim] on input
                           { dpt[0] =
                              tp2_basis_dsc_u.dsc_basis->Span_umax
                                 (tp2_basis_dsc_u.dsc_basis->Span_count()-1) ;
                             dpt[1] =
                              tp2_basis_dsc_v.dsc_basis->Span_umax
                                 (tp2_basis_dsc_v.dsc_basis->Span_count()-1) ;
                           }

  virtual void             // bas: DS_pfunc virtual function
   Elem_domain_min         // eff: store lowest corner of the min
    (int elem_index,       // in : elem in question's identifier
     double *dpt)          // out:   sized domain bounding SPAbox.
                           //     dpt Sized:[domain_dim] on input
                           { DS_assert(   elem_index >= 0
                                       && elem_index < pfn_elem_count);
                             int spanu, spanv ;
                              Elem_2span_index(elem_index, spanu, spanv) ;
                             dpt[0] =
                              tp2_basis_dsc_u.dsc_basis->Span_umin(spanu) ;
                             dpt[1] =
                              tp2_basis_dsc_v.dsc_basis->Span_umin(spanv) ;

                           }

  virtual                  // bas: DS_pfunc virtual function
   double                  // rtn: shortest pfunc elem span
    Min_span_length()      // eff: search every elem span
                           { double minu, minv ;
                             minu = tp2_basis_dsc_u.dsc_basis->
                                                Min_span_length() ;
                             minv = tp2_basis_dsc_v.dsc_basis->
                                                Min_span_length() ;
                             return(minu < minv ? minu : minv) ;
                           }

  virtual void             // bas: DS_pfunc virtual function
   Elem_domain_max         // eff: store upper corner of the min
    (int elem_index,       // in : elem in question's identifier
     double *dpt)          // out:   sized domain bounding SPAbox.
                           //     dpt Sized:[domain_dim] on input
                           { DS_assert(   elem_index >= 0
                                       && elem_index < pfn_elem_count);
                             int spanu, spanv ;
                              Elem_2span_index(elem_index, spanu, spanv) ;
                             dpt[0] =
                              tp2_basis_dsc_u.dsc_basis->Span_umax(spanu) ;
                             dpt[1] =
                              tp2_basis_dsc_v.dsc_basis->Span_umax(spanv) ;
                           }

  virtual int              // ret: total Cx=d row count in rep
   Cd_line_row_count() ;   // inf: NURB cstrns for closed & periodic

  virtual void             // eff: build pfunc's Cx=d eqn rows as
   Build_Cd_row            //      needed for closed and periodic
    (DS_eqns *eqns,        // in : holder of Ax=b, Cx=d eqns
     int &next_Cd_row) ;   // i/o: 1st available row in Cx=d

  virtual void             // eff: build pfunc's Cx=d eqn rows as
   Build_d_row             //      needed for closed and periodic
    (DS_eqns *eqns,        // in : holder of Ax=b, Cx=d eqns
     int &next_Cd_row) ;   // i/o: 1st available row in Cx=d

  // DS_pfunc functions that operate on domain curves

                           // bas: DS_pfunc virtual function
   virtual double *        // ret: array of segment u boundary vals
    Calc_dcrv_segments     // eff: Segment src_pfunc at xsects with
                           //      elem_domain_bndries and src_knots
     (DS_pfunc *src_pfunc, // in : defines the domain curve to xsect
      int *&xsect_cont,    // out: continuity at every 'this' xsect bndry
      int &seg_count) ;    // out: number of segments returned
                           //      return array Sized:[seg_count+1]
                           // mallocs: both xsect_cont and return arrays

  virtual                  // bas: DS_pfunc virtual function
   double *                // use: called by Calc_dcrv_segments()
    Calc_uv_line_xsects    // eff: rtn array of xsects with uv-lines
     (DS_pfunc *,          // in : surf_pfunc = The surface shape being constrained
      int,                 // in : u_count = number of constant u curves
      double *,            // in : uu = array of uu values
      int *,               // in : cont_u = u continuity (-1=curve_bound,0=C0,1=C1..)
      int ,                // in : v_count = number of constant v curves
      double *,            // in : vv = array of vv values
      int *,               // in : cont_v = v continuity (-1=curve_bound,0=C0,1=C1..)
      int *&,              // out: xsect_cont = continuity at each xsect
      int &seg_count)      // out: number of segments returned
                           //      return array Sized:[seg_count+1]
                           { // TODO:??? currently only a 1D operator
                             seg_count = 0 ;
                             return NULL ;
                           }

  virtual                  // bas: DS_pfunc virtual function
   int                     // rtn: 1=found an xsect, 0=didn't
    Calc_iline_xsect       // eff: rtn 1st xsect with iline
                           //      iline = p0 + u(p1-p0)
    (double *p0,           // in : image space pt, Sized:[image_dim]
     double *p1,           // in : image space pt, Sized:[image_dim]
     double, // max_dist,      // in : max_dist for near misses
     double *image_pt,     // out: image-loc for nearest xsect
                           //      sized:[image_dim]
     double &iline_u,      // out: iline param val for nearest xsect
     double *pfunc_uv) ;   // out: pfunc param val for nearest xsect
                           //      sized:[domain_dim]

} ; // end class DS_tprod_2d


/**
 * @nodoc
 */
class DS_tprod_3d
{
  protected:

  public:

} ; // end class DS_tprod_3d

// OBSOLETE CODE SNIPS - SAVED IN CASE WE GO BACK
/* try to compile without the overloading by changind
// arguments from DS_PFN_VAL1 and DS_PFN_VAL2 to ints

   double                  // use: after Calc_elem_bas_at_gpts()
   *Elem_gpt_val_blk       // ret: val_blk ptr from pfn_elem_gpt_bas
    (DS_PFN_VAL1 val_type) // oneof: pfn_u    | pfn_du   | pfn_duu
                           { return(  pfn_elem_gpt_bas
                                     + Elem_gpt_val_size()
                                     * val_type) ;
                           }

   double                  // use: after Calc_elem_bas_at_gpts()
   *Elem_gpt_val_pt        // ret: val_blk ptr from pfn_elem_gpt_bas
    (DS_PFN_VAL1 val_type, // in : oneof( pfn_u | pfn_du | pfn_duu)
     int gpt_index)        // in : gpt identifier >= 0
                           { DS_assert(   gpt_index < pfn_elem_gpt_count
                                       && gpt_index >= 0) ;
                             return(  pfn_elem_gpt_bas
                                     + (  val_type
                                        * pfn_elem_gpt_count
                                        + gpt_index)
                                     * pfn_elem_dof_count) ;
                           }

   double                  // use: after Calc_elem_bas_at_gpts()
   *Elem_gpt_val_blk       // ret: val_blk ptr from pfn_elem_gpt_bas
    (DS_PFN_VAL2 val_type) // in : oneof(pfn_uv  |pfn_duv |pfn_udv
                           //            pfn_duuv|pfn_dudv|pfn_udvv)
                           { return(  pfn_elem_gpt_bas
                                    + Elem_gpt_val_size()
                                    * val_type) ;
                           }
   double                  // use: after Calc_elem_bas_at_gpts()
   *Elem_gpt_val_pt        // ret: val_blk ptr from pfn_elem_gpt_bas
    (DS_PFN_VAL2 val_type, // in : oneof(pfn_uv  |pfn_duv |pfn_udv
                           //            pfn_duuv|pfn_dudv|pfn_udvv)
     int gpt_index)        // in : basis 0 gpt identifier >= 0
                           { DS_assert(   gpt_index < pfn_elem_gpt_count
                                       && gpt_index >= 0) ;
                             return(  pfn_elem_gpt_bas
                                     + (  val_type
                                        * pfn_elem_gpt_count
                                        + gpt_index)
                                     * pfn_elem_dof_count) ;
                           }

*/  // end obsolete code section
/** @} */
#endif // DS_PFUNC_H
