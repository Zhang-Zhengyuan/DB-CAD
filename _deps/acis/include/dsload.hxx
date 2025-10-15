/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dsload.hxx
// created  : March 4, 1995
// author   : George Celniker
// contains : classes DS_load, 
//                    DS_pt_press,
//                    DS_dyn_load,
//                    DS_dist_press,
//                    DS_vector_load,
//                    DS_attractor,
//                    DS_spring, 
//                    DS_spring_set,
//                    DS_crv_load,
// synopsis : Representation for loads in the SPAmatrix
//            problem min(1/2xAx - bx) subject to the constraints
//                    Cx = d. 
//            Loads are used to generate the terms of the b SPAmatrix
//            Each load is stored independently.  Changes in load
//            gains, the addition and removal of loads, and changes
//            in the regions that a load acts upon can be made
//            without having to recompute the bulk of the solution
//            results.  Loads will be the key interactive component
//            for sculpting deformable models.
//
//            Loads are managed as a link-list much like constraints

#ifndef DS_load_H
#define DS_load_H

#include "dsstdef.hxx" // Define NULL, DS_assert(), DS_is_zero() // Define NULL, DS_assert(), DS_is_zero()
#include "dsblmem.hxx" // DS_copy_double_block() // DS_copy_double_block()
#include "dsgauss.hxx" // DS_linear_gauss_pt_count(),gauss consts // DS_linear_gauss_pt_count(),gauss consts
#include "dsmat.hxx"   // class DS_diag_mat   // class DS_diag_mat
#include "dmapinum.hxx"// enum DS_LDS// enum DS_LDS
#include <math.h>                      // ceil()

// bmt - moved to dmapinum.hxx
// type_id enum

// forward references
class DS_pfunc ;           // parametric function class
class DS_eqns ;            // holder of  Ax=b, Cx=d, Li xi=Lc eqns
//class DS_symeq ;           // holder of Ax=b, Cx=d, Li xi=Lc eqns
class DS_tprod_1d ;        // Derived parametric function class
class DS_dmod ;
#include "dmicon.hxx"               // Class DM_icon               // Class DM_icon

//============================================================
// Class DS_load
//============================================================

class DS_load : public ACIS_OBJECT             // identifier = lds
{
  protected:
   DS_LDS   lds_type_id ;  // enum showing specialization type
   DM_icon* lds_icon;      // the icon we own
   DS_dmod* lds_dmod;      // dmod back pointer - acquaintance
   DS_dmod* Dmod()         { return lds_dmod ;}
   double   lds_gain ;     // the load's gain, user sculpting handle
   int      lds_tag ;      // tag so apps can track loads
   DS_load *lds_next ;     // a singly linked list
  public:
   // Type_id, constructor, copy, Make_copy, destructor
   DS_LDS Type_id()        // Type_id access
                           { return lds_type_id ; }
   int Tag()               // Application tag access
                           { return lds_tag ; }
   void Set_tag(int tag);

   DS_load                 // default constructor
    (DS_dmod* dmod,
     DS_LDS type_id,       // derived class identifier
     double gain=0.0,      // the load's gain, user sculpting handle
     int    tag = -1,      // tag so apps can track loads
     DS_load *next=NULL) ; // linked list pointer
                          
   DS_load                 // copy constructor
    (DS_load &load) ;      // copies all members in the linked list

   DS_load &operator=      // assignment operator
    (DS_load &load) ;

   virtual DS_load*        // Virtual Make_copy (public function)
    Make_copy()=0 ;        // copies all members in the linked list

   void
    Size_arrays            // Size_arrays
    (int grobj_count=0) ;  // number of graphic object pointers

   virtual                 // Virtual destructor
  ~DS_load()               // deletes all members in the linked list
                           { Size_arrays() ;
                             lds_dmod = NULL;
                             if(lds_next != NULL) 
                               { ACIS_DELETE lds_next ;
                                 lds_next = NULL ;
                               }
                           }
// Default implementation does nothing; some loads have no graphics.
   virtual DM_icon* Get_icon() const ;
   virtual void Set_icon(DM_icon*);
   virtual void Set_icon_owner(DS_dmod*,int tag);
   virtual void Notify_icon_tag_obj_change();
   virtual void Draw(const DM_icon_draw_args& )const;
   virtual void Setstate(const DM_icon_cmd_args& args);
   virtual void Query(DM_icon_query_args& args) const;

  // DS_load simple data access for items and list 
  const double &                 // ret: the load's gain for ref or change
    Gain() const                { return lds_gain ; } 

  void Set_gain            // eff: set a load's gain value
   (double gain)           { lds_gain = gain ; }

  int                      // ret: number of linked elems starting
   List_length() ;         //      with this one.

  DS_load *                // ret: Parent load when deep in list or
   Is_load_in_list         // ret: Null when 1st or not in list
    (DS_load *tgt_load,    // in : Cstrn to look for 
     int &in_listP) ;      // out: 1=in list,0=not_in_list

  DS_load *               // Get the linked list's next DS_load
   Next()                  { return lds_next ; }

  void
   Set_next                // set the next pointer in this load
    (DS_load *load)        { lds_next = load ; }

  DS_load *
   Nearest_to_iline        // ret: closest linked list load to
                           //        iline = pi0 + u(pi1-pi0)
    (double *pi0,          // in : image space pt, Sized:[image_dim]
     double *pi1,          // in : image space pt, Sized:[image_dim]
     double max_dist,      // in : limits search area
     int    &pt_index,     // out: image_pt index in load for closest pt
     double &dist2,        // out: dist**2 between load and line
     double &uu) ;         // out: iline param for pt closest to load

  virtual double           // bas: DS_load virtual function
   Dist2_to_iline          // ret: dist**2 between load and iline
                           //      or -1 when measure makes no sense
                           //      (iline = pi0 + u(pi1-pi0))
    (double *pi0,          // in : image space pt, Sized:[image_dim]
     double *pi1,          // in : image space pt, Sized:[image_dim]
     int    &pt_index,     // out: pt index of returned dist2_to_iline
     double &uu)=0 ;       // out: iline u val at pt nearest to load


  // DS_load side_effect functions

  void
   Build_A                 // eff: Add all load terms to A in Ax=b
                           //      Requires call to be made from
                           //      the linked list's first DS_load.
    (DS_pfunc *tgt_pfunc,  // in : the defining parametric equation
     DS_eqns *eqns); // in : holder of Ax=b, Cx=d eqns

  void
   Build_b                 // eff: Add all load terms to b in Ax=b.
                           //      Requires call to be made from
                           //      the linked list's first DS_load.
    (DS_pfunc *tgt_pfunc,  // in : Shape model's parametric function
     DS_eqns *eqns, // in : out holder of Ax=b, Cx=d eqns
     const double *x = NULL,      // in : used for dynamic loads
     const double *old_x = NULL ) ;// in : used for dynamic loads

  void                     // eff: shift the basis domain values.
   Shift_load_domain       //      iterates through all loads on list
    (double *du) ;         // in : domain shift SPAvector, Sized:Domain_dim()

   int                     // rtn: 0=done,-1=scale too small
                           // eff: shift the basis domain values.
   Scale_load_domain       //      iterates through all loads on list
    (double scale) ;       // in : domain scale factor

                           // use: after tgt_pfunc X or domain changes
  int                      // rtn: 0=no A rebuild, 1=rebuild A SPAmatrix
   Update_load_pts         // eff: reset (X,P,basis) dependent vals 
    (DS_pfunc *tgt_pfunc,  // in : Shape model's parametric function
     int pfunc_state=0) ;  // orof: DS_PFN_CHANGED_X      = X val change
                           //       DS_PFN_CHANGED_P_ON   = Set def shape
                           //       DS_PFN_CHANGED_P_OFF  = Clear def shape
                           //       DS_PFN_CHANGED_DOMAIN = Domain change


  // DS_cstrn side_effect functions
  // DS_load virtual side-effect functions
  virtual void             // bas: DS_load virtual function
   Shift_domain            // eff: shift the basis domain values
    (double *du)=0 ;       // in : domain shift SPAvector, Sized:Domain_dim()

  virtual                  // bas: DS_load virtual function
   int                     // rtn: 0=done,-1=scale too small
   Scale_domain            // eff: scale the basis domain values
    (double scale)=0 ;     // in : domain scale factor

  virtual void             // bas: DS_load virtual function
   Build_this_A            // eff: Add one load's terms to b in Ax=b
    (DS_pfunc *tgt_pfunc,  // in : the defining parametric equation
     DS_eqns *eqns)=0 ;    // in : holder of Ax=b equation

  virtual void             // bas: DS_load virtual function
   Build_this_b            // eff: Add one load's terms to b in Ax=b
    (DS_pfunc *tgt_pfunc,  // in : Shape model's parametric function
     DS_eqns *eqns,        // in : out holder of Ax=b, Cx=d eqns
     const double *x = NULL,      // in : used for dynamic loads
     const double *old_x = NULL) = 0;// in : used for dynamic loads

  virtual                  // bas: DS_load virtual function
   int                     // rtn: 0=no A rebuild,1=A rebuild,-1=bad_dpts
    Update_pts             // eff: update load cached data after
                           //      changes in tgt_pfunc X or domain
    (DS_pfunc *tgt_pfunc,  // in : Shape model's parametric function
     int pfunc_state=0)=0; // orof: DS_PFN_CHANGED_X      = X val change
                           //       DS_PFN_CHANGED_P_ON   = Set def shape
                           //       DS_PFN_CHANGED_P_OFF  = Clear def shape
                           //       DS_PFN_CHANGED_DOMAIN = Domain change

} ; // end class DS_load

//============================================================
// Class DS_pt_press
//============================================================

// DS_pt_press graphics:  The pt_press graphics consists of one
//                        end point marker.
//  cst_grobj[0] = end point
//          

class DS_pt_press : public DS_load  // identifier = ppr
{
  protected:
   int     ppr_elem_index; // cache the elem_index for the pt. When
                           // set to -1 it is a flag to recompute 
                           // the uv, duv, and udv basis values
                           // in call Calc_ipt_idir()

   int ppr_elem_dof_count; // [array_size] max elem dof count
   double *ppr_pos ;       // pt's basis vals (surf = uv, curv = u)  
   double *ppr_d1 ;        // pt's basis vals (      duv        du)
   double *ppr_d2 ;        // pt's basis vals (      udv       duu)
   int *ppr_elem_dof_map ; // map basis functions to global dofs

   int     ppr_image_dim;  // [array_size] problem image dim size
   double *ppr_image_pt ;  // pt load image (pos and normalized dir)
                           // Sized:[2 * ppr_image_dim]
                           // Stored:[xx, yy, zz, dx, dy, dz ]
                           //     or [xx, yy, dx, dy ]

   int     ppr_domain_dim; // [array_size] problem domain dim size
   double *ppr_domain_pt ; // domain point constraint SPAposition
                           // Sized:[ppr_domain_dim]

   int     ppr_negate ;    // negate_gain_flag(1=negate,0=don't)

  public:
   // DS_pt_press constructor, copy, Make_copy, Size_arrays, Destructor

   DS_pt_press              // Default Constructor
    (DS_dmod *dmod,         // owning dmod
     double gain=0.0,       // the load's gain, user sculpting handle
     DS_pfunc *
           tgt_pfunc=NULL,  // shape used to compute image_pt (not_saved)
     int negate=0,          // negate_gain_flag(1=negate,0=don't)
     double *domain_pt=NULL,// load loc, Sized:[pfunc->Domain_dim()]
     int tag= -1,           // tag so apps can track loads
     DS_load *next=NULL) ;  // linked list pointer

   DS_pt_press             // eff: Copy Constructor
   (DS_pt_press &pt_press);// in : object being copied

   DS_pt_press &operator=  // eff: assignment operator
    (DS_pt_press &pt_press);

   DS_load*                // bas: DS_load virtual function
    Make_copy()            // eff: Virtual DS_load function
                           { return( ACIS_NEW DS_pt_press(*this)) ; }

   void
    Size_arrays            // Size_arrays
     (int elem_dof_count=0,// max dof_count per elem in tgt_pfunc
      int image_dim=0,     // Size of the problem's image space
      int domain_dim=0) ;  // Size of the problem's domain space

   virtual
  ~DS_pt_press();           // Destructor

  // DS_pt_press simple data access function

  int     Elem_index()     { return ppr_elem_index ; }

  int     Image_dim()      { return ppr_image_dim ; }
  double *Image_pt()       { return ppr_image_pt ; }
  double &Image_pt(int d)  { DS_assert(d >= 0 && d < ppr_image_dim) ;
                             return( ppr_image_pt[d]) ; }
  double *Image_dir()      { return ppr_image_pt+ppr_image_dim ; }
  double &Image_dir(int d) { DS_assert(d >= 0 && d < ppr_image_dim) ;
                             return( ppr_image_pt[ppr_image_dim+d]) ; }
  int     Domain_dim()     { return ppr_domain_dim ; }
  double *Domain_pt()      { return ppr_domain_pt ; }
  double &Domain_pt(int d) { DS_assert(d >= 0 && d < ppr_domain_dim) ;
                             return( ppr_domain_pt[d]) ; }

  int                      // rtn: 0=success,-1=bad_dpt
                           // eff: copies dpt into pt_press, and then
   Set_domain_pt           //      recomputes image_pt and elem_index
   (double *dpt,           // in : pt to copy, Sized:Domain_pt_size()
    DS_pfunc *tgt_pfunc) ; // in : the shape being loaded

  int Negate_flag()        // rtn: 1=negate,0=don't
                           { return ppr_negate ; }
  void Set_negate_flag     // eff: set negate value
    (int negate_flag)      // in : 1=negate,0=don't
                           { if(   negate_flag == 0
                                || negate_flag == 1)
                               ppr_negate = negate_flag ;
                           }

  int Basis_block_size     // ret: double count bigger than mem_blk
   (int elem_dof_count,    // in : max dof_count per elem in pfunc
    int image_dim,         // in : image dim
    int domain_dim)        // in : domain dim
                           { return(  elem_dof_count  // ppr_pos
                                    + elem_dof_count  // ppr_d1
                                    + elem_dof_count  // ppr_d2
                                    + image_dim       // ppr_image_pt
                                    + image_dim       //  (ppr_image_dir)
                                    + domain_dim      // ppr_domain_pt
                                    + (int)ceil       // ppr_elem_dof_map
                                      (  elem_dof_count  
                                        * (double)sizeof(int)
                                        / (double)sizeof(double))); }

  //  implementations of DS_load virtual functions
  // DS_pt_press non-side-effect functions

  double                   // bas: DS_load virtual function
   Dist2_to_iline          // ret: dist**2 between load and iline
                           //      or -1 when measure makes no sense
                           //      (iline = pi0 + u(pi1-pi0))
    (double *pi0,          // in : image space pt, Sized:[image_dim]
     double *pi1,          // in : image space pt, Sized:[image_dim]
     int    &pt_index,     // out: pt index of returned dist2_to_iline
     double &uu) ;         // out: iline u val at pt nearest to load

  // DS_pt_press side-effect functions

  int                      // rtn: 0=success,-1=bad_dpts                     
                           // eff: update image_pt,image_dir vals
   Calc_ipt_idir           //      for x & dpt. (Cache basis values)
    (DS_pfunc *tgt_pfunc); // in : Shape Model, parametric function

  //  implementations of DS_load virtual functions
  // DS_pt_press side-effect functions

  virtual void             // bas: DS_load virtual function
   Shift_domain            // eff: shift the basis domain values
    (double *du)           // in : domain shift SPAvector, Sized:Domain_dim()
                           { DS_assert(du) ;
                             for(int ii=0;ii<ppr_domain_dim;ii++) 
                               ppr_domain_pt[ii] += du[ii] ;
                           }

  virtual                  // bas: DS_load virtual function
   int                     // rtn: 0=done,-1=scale too small
   Scale_domain            // eff: scale the basis domain values
    (double scale)         // in : domain scale factor
                           { // no work condition
                             if(DS_is_zero(scale,DS_DOUBLE_CLOSE))
                               return(-1) ;

                             // scale
                             for(int ii=0;ii<ppr_domain_dim;ii++) 
                               ppr_domain_pt[ii] *= scale ;
                             return(0) ;
                           } 

  virtual void             // bas: DS_load virtual function
   Build_this_A            // eff: Add one load's terms to b in Ax=b
    (DS_pfunc *,           // in : tgt_pfunc = the defining parametric equation
     DS_eqns *)            // in : eqns = holder of Ax=b equation

       { }                 // act: DS_pt_press adds no terms to A

  virtual void             // bas: DS_load virtual function
   Build_this_b            // eff: Add one load's terms to b in Ax=b
    (DS_pfunc *tgt_pfunc,  // in : Shape model's parametric function
     DS_eqns *eqns,        // in : out holder of Ax=b, Cx=d eqns
     const double *x = NULL,      // in : used for dynamic loads
     const double *old_x = NULL) ;// in : used for dynamic loads

  virtual                  // bas: DS_load virtual function
   int                     // rtn: 0=no A rebuild,1=A rebuild,-1=bad_dpts
    Update_pts             // eff: update load cached data after
                           //      changes in tgt_pfunc X or domain
    (DS_pfunc *pfunc,      // in : Shape model's parametric function
     int pfunc_state=0) ;  // orof: DS_PFN_CHANGED_X      = X val change
                           //       DS_PFN_CHANGED_P_ON   = Set def shape
                           //       DS_PFN_CHANGED_P_OFF  = Clear def shape
                           //       DS_PFN_CHANGED_DOMAIN = Domain change
                           
} ; // end class DS_pt_press

//==========================================================
// Class DS_dyn_load
//==========================================================


// DS_dyn_load graphics:  The DS_dyn_load graphics have no graphics
//          

class DS_dyn_load : public DS_load     // identifier = dld
{
  protected:
   double  dld_mass ;      // M = mass * I   in Mx" + Cx' + Kx = g
   double  dld_damp ;      // C = damp * I   in Mx" + Cx' + Kx = g
   double  dld_dt   ;      // delta time  for x" and x'
   DS_diag_mat *dld_mc ;   // mass/damp SPAmatrix

  public:
   // DS_dyn_load constructor, copy, Make_copy, Destructor

   DS_dyn_load             // Default Constructor
    (DS_dmod *dmod,
     double mass=1.0,      // M = mass * I   in Mx" + Cx' + Kx = g
     double damp=5.0,      // C = damp * I   in Mx" + Cx' + Kx = g
     double dt=1.0,        // delta time for x" and x'
     int dof_count=0,      // number of dofs in parent DS_pfunc
     int tag = -1,         // tag so apps can track a load
     DS_load *next=NULL)   // linked list pointer   
                           : DS_load(dmod, ds_dld, 0.0, tag, next)
                           { dld_mass = mass ;
                             dld_damp = damp ;
                             dld_dt   = dt ;
                             dld_mc   = ACIS_NEW DS_diag_mat(dof_count) ;
                             if(!dld_mc) DM_sys_error(DM_FREE_STORE_OVERFLOW) ;
                           }                    

   DS_dyn_load             // Copy Constructor
   (DS_dyn_load &dyn_load) : DS_load(dyn_load)
                           { dld_mass = dyn_load.dld_mass ;
                             dld_damp = dyn_load.dld_damp ;
                             dld_dt   = dyn_load.dld_dt ;
                             dld_mc   = ACIS_NEW DS_diag_mat
                                              (*dyn_load.dld_mc) ;
                             if(!dld_mc) DM_sys_error(DM_FREE_STORE_OVERFLOW) ;

                           }

   DS_dyn_load &operator=  // assignment operator
   (DS_dyn_load &dyn_load) {  // no work condition
                              if(&dyn_load != this) {
                             ((DS_load *)this)->operator=(dyn_load);
                             dld_mass = dyn_load.dld_mass ;
                             dld_damp = dyn_load.dld_damp ;
                             dld_dt   = dyn_load.dld_dt ;
                             if(dld_mc){ACIS_DELETE dld_mc ;
                                        dld_mc = NULL ;
                                       }
                             dld_mc   = ACIS_NEW DS_diag_mat
                                              (*dyn_load.dld_mc) ;
                             if(!dld_mc) DM_sys_error(DM_FREE_STORE_OVERFLOW) ;

                             }
                             return *this ;
                           }

   DS_load*                // bas: DS_load virtual function
    Make_copy()            // eff: Virtual DS_load copy function
                           { return( ACIS_NEW DS_dyn_load(*this)) ; }
   virtual
  ~DS_dyn_load()              // Destructor
                           { if(dld_mc) { ACIS_DELETE dld_mc ; 
                                          dld_mc = NULL ;
                                        }
                           }

  // DS_dyn_load simnple data access
  double Mass()            { return dld_mass ; }
  double Damp()            { return dld_damp ; }
  double Dt()              { return dld_dt ; }
  DS_diag_mat *Mc()        { return dld_mc ; }

  void Get_dyn_terms       // quick access convenience routine
   (double &mass,          // out: values set to current 
    double &damp,          //      dyn_load values
    double &dt)            { mass = dld_mass ;
                             damp = dld_damp ;
                             dt   = dld_dt ;
                           }
 
  void Set_dyn_terms       // convenience routine for quick access
   (double mass,           // in : set the dyn load values with
    double damp,           //      these values.
    double dt)             { dld_mass = mass ;
                             dld_damp = damp ;
                             dld_dt   = dt ;
                           }
  
  // DS_dyn_load implementations of DS_load virtual functions
  // DS_dyn_load non-side-effect functions

  double                   // bas: DS_load virtual function
   Dist2_to_iline          // ret: dist**2 between load and iline
                           //      or -1 when measure makes no sense
                           //      (iline = pi0 + u(pi1-pi0))
    (double* ,             // in : pi0 = image space pt, Sized:[image_dim]
     double* ,             // in : pi1 = image space pt, Sized:[image_dim]
     int    &pt_index,     // out: pt index of returned dist2_to_iline
     double &uu)           // out: iline u val at pt nearest to load

                           { uu = 0 ; pt_index = 0 ; return( -1.0 ) ; }

  // DS_dyn_load implementations of DS_load virtual functions

  // DS_dyn_load implementations of DS_load virtual functions
  // DS_dyn_load side-effect functions

  virtual void             // bas: DS_load virtual function
   Shift_domain            // eff: shift the basis domain values
    (double *)             // in : du = domain shift SPAvector, Sized:Domain_dim()
                           { }

  virtual                  // bas: DS_load virtual function
   int                     // rtn: 0=done,-1=scale too small
   Scale_domain            // eff: scale the basis domain values
    (double)               // in : scale = domain scale factor
                           { return(0) ; } 

  virtual void             // bas: DS_load virtual function
   Build_this_A            // eff: Add one load's terms to b in Ax=b
    (DS_pfunc *tgt_pfunc,  // in : the defining parametric equation
     DS_eqns *eqns) ;      // in : holder of Ax=b equation

  virtual void             // bas: DS_load virtual function
   Build_this_b            // eff: Add one load's terms to b in Ax=b
    (DS_pfunc *tgt_pfunc,  // in : Shape model's parametric function
     DS_eqns *eqns,        // in : out holder of Ax=b, Cx=d eqns
     const double *x = NULL,      // in : used for dynamic loads
     const double *old_x = NULL) ;// in : used for dynamic loads

  virtual                  // bas: DS_load virtual function
   int                     // rtn: 0=no A rebuild,1=A rebuild,-1=bad_dpts
    Update_pts             // eff: sizes dld_mc to pfunc->Dof_count()
    (DS_pfunc *tgt_pfunc,  // in : Shape model's parametric function
     int pfunc_state=0) ;  // orof: DS_PFN_CHANGED_X      = X val change
                           //       DS_PFN_CHANGED_P_ON   = Set def shape
                           //       DS_PFN_CHANGED_P_OFF  = Clear def shape
                           //       DS_PFN_CHANGED_DOMAIN = Domain change
                           
} ; // end class DS_dyn_load

//============================================================
// Class DS_dist_press
//============================================================

// DS_dist_press graphics:  The DS_dist_press have no graphics.
// DS_dist_press domain:    Distributed pressures are applied over
//                          a rectangular region
//                          1d = curve segment
//                          2d = rectangle
//                          3d = rectangular solid
// graphics = 1 polyline

class DS_dist_press : public DS_load  // identifier = dpr
{
  protected:
    
   int dpr_domain_dim ;       // [array-size] assoc prob domain size
   double *dpr_domain_min ;   // lower boundary of applied load
   double *dpr_domain_max ;   // upper boundary of applied load
   int dpr_negate ;           // negate_gain_flag(1=negate,0=don't)

   // TODO: ??? should we consider saving gpt uv,udv,vdu values
   //            at every element

  public:
   // DS_dist_press constructor, copy, Make_copy, Size_arrays, Destructor

   DS_dist_press           // Default Constructor
    ( DS_dmod* dmod,
      double gain=0.0,      // in : load's gain; user sculpting handle
     int domain_dim=0,     // in : loaded geom's domain dimension
     int negate=0,         // in : negate_gain_flag(1=negate,0=don't)
     double *dpt_min=NULL, // in : lower corner of the loaded domain
     double *dpt_max=NULL, // in : upper corner of the loaded domain
     int tag= -1,          // in : tag so apps can track loads
     DS_load *next=NULL);  // in : linked list pointer

   DS_dist_press              // Copy Constructor
   (DS_dist_press &dist_press);// object being copied
   DS_dist_press &operator= // eff: assignment operator
   (DS_dist_press &dist_press);
   DS_load*                // bas: DS_load virtual function
    Make_copy()            // eff: Virtual DS_load copy function
                           { return( ACIS_NEW DS_dist_press(*this)) ; }

   void Size_arrays        // eff: allocate internal array memory
    (int domain_size=0) ;  // in : assoc geom's domain size

   virtual
  ~DS_dist_press() ;       // Destructor


  // DS_dist_press simple data access function

  int Domain_dim()         { return dpr_domain_dim ; }
  double *Domain_min()     { return dpr_domain_min ; }
  double Domain_min(int i) { DS_assert(i>=0&&i<dpr_domain_dim);
                             return dpr_domain_min[i] ;
                           }
  double *Domain_max()     { return dpr_domain_max ; }
  double Domain_max(int i) { DS_assert(i>=0&&i<dpr_domain_dim);
                             return dpr_domain_max[i] ;
                           }
  int Negate_flag()        { return dpr_negate ; }
  void Set_negate_flag     // eff: set dpr_negate
    (int negate_flag)      // in : 1=negate,0=don't
                           { if(   negate_flag == 0 
                                || negate_flag == 1)
                               dpr_negate = negate_flag ;
                           }

  int Block_size           // rtn: size of all internal arrays
    (int domain_dim)       // in : assoc geom's domain size
                           { return( 2 * domain_dim) ; }

  //  implementations of DS_load virtual functions
  // DS_dist_press non-side-effect functions

  double                   // bas: DS_load virtual function
   Dist2_to_iline          // ret: dist**2 between load and iline
                           //      or -1 when measure makes no sense
                           //      (iline = pi0 + u(pi1-pi0))
    (double* ,             // in : pi0 = image space pt, Sized:[image_dim]
     double* ,             // in : pi1 = image space pt, Sized:[image_dim]
     int    &pt_index,     // out: pt index of returned dist2_to_iline
     double &uu)           // out: iline u val at pt nearest to load
                           { // return a useless number
                             uu = 0 ; pt_index = 0 ; return( -1.0 ) ;
                           }

  //  implementations of DS_load virtual functions
  // DS_dist_press side-effect functions

  virtual void             // bas: DS_load virtual function
   Shift_domain            // eff: shift the basis domain values
    (double *du)           // in : domain shift SPAvector, Sized:Domain_dim()
                           { DS_assert(du) ;
                             for(int ii=0;ii<dpr_domain_dim;ii++)
                               { dpr_domain_min[ii] += du[ii] ;
                                 dpr_domain_max[ii] += du[ii] ;
                               }
                           }

  virtual                  // bas: DS_load virtual function
   int                     // rtn: 0=done,-1=scale too small
   Scale_domain            // eff: scale the basis domain values
    (double scale)         // in : domain scale factor
                           { // no work condition
                             if(DS_is_zero(scale,DS_DOUBLE_CLOSE))
                               return(-1) ;
                             // scale
                             for(int ii=0;ii<dpr_domain_dim;ii++) 
                               { dpr_domain_min[ii] *= scale ;
                                 dpr_domain_max[ii] *= scale ;
                               }
                             return(0) ;
                           } 

  virtual void             // bas: DS_load virtual function
   Build_this_A            // eff: Add one load's terms to b in Ax=b
    (DS_pfunc *,           // in : tgt_pfunc = the defining parametric eqn
     DS_eqns *)           // in : symeq = holder of Ax=b equation
       { }                 // act: DS_dist_press adds no terms to A

  virtual void             // bas: DS_load virtual function
   Build_this_b            // eff: Add one load's terms to b in Ax=b
    (DS_pfunc *tgt_pfunc,  // in : Shape model's parametric function
     DS_eqns *eqns,        // in : out holder of Ax=b, Cx=d eqns
     const double *x = NULL,      // in : used for dynamic loads
     const double *old_x = NULL) ;// in : used for dynamic loads

  virtual                  // bas: DS_load virtual function
   int                     // rtn: 0=no A rebuild,1=A rebuild,-1=bad_dpts
    Update_pts             // eff: update load cached data after
                           //      changes in tgt_pfunc X or domain
    (DS_pfunc *tgt_pfunc,  // in : Shape model's parametric function
     int pfunc_state=0) ;  // orof: DS_PFN_CHANGED_X      = X val change
                           //       DS_PFN_CHANGED_P_ON   = Set def shape
                           //       DS_PFN_CHANGED_P_OFF  = Clear def shape
                           //       DS_PFN_CHANGED_DOMAIN = Domain change
                           
} ; // end class DS_dist_press

//============================================================
// Class DS_vector_load
//============================================================

// DS_vector_load graphics:  The DS_vector_load graphics consist
//                           of two point marker and one 2pt
//                           polyline marker.
//  cst_grobj[0] = SPAvector base_pt 
//  cst_grobj[1] = SPAvector end_pt
//  cst_grobj[2] = 2-pt polyline
//  The base point for the SPAvector load is the curve SPAposition
//  u=0.5 or the surface SPAposition u=v=0.5

// DS_vector_load domain:    The DS_vector_load is applied over
//                           the entire shape domain

class DS_vector_load : public DS_load  // identifier = vec
{
  protected:
    
   int vec_image_dim ;        // [array-size] assoc prob image size
   double *vec_image_vec ;    // direction SPAvector load
   double *vec_base_pt ;      // base point for rendering
   double *vec_end_pt ;       // end point for rendering
                              // magnitude is set by gain value

  public:
   // DS_vector_load constructor, copy, Make_copy, Size_arrays, Destructor

   DS_vector_load          // Default Constructor
   (DS_dmod *dmod,         // in : owning dmod
    double gain=0.0,       // in : load's gain; user sculpting handle
    DS_pfunc *pfunc=NULL,  // in : the shape being loaded
    double *image_vec=NULL,// in : SPAvector direction, sized:[image_dim]
    int tag= -1,           // in : tag so apps can track loads
    DS_load *next=NULL) ;  // in : linked list pointer

   DS_vector_load                  // Copy Constructor
   (DS_vector_load &vector_load) ; // object being copied

   DS_vector_load &operator=       // eff: assignment operator
   (DS_vector_load &vector_load) ; // object with vals being copied
                            
   DS_load*                // bas: DS_load virtual function
    Make_copy()            // eff: Virtual DS_load copy function
                           { return( ACIS_NEW DS_vector_load(*this)) ; }

   void Size_arrays        // eff: allocate internal array memory
    (int image_size=0) ;   // in : assoc geom's domain size

   virtual
  ~DS_vector_load() ;       // Destructor


  // DS_vector_load simple data access function

  int Image_dim()          { return vec_image_dim ; }
  double *Image_vec()      { return vec_image_vec ; }
  double *Base_pt()        { return vec_base_pt ; }
  double *End_pt()         { return vec_end_pt ; }
  double *Image_pt         // ret: base_pt or end_pt
   (int pt_index)          // in : 0=base_pt, 1=end_pt
                           { return pt_index==0 ? vec_base_pt:vec_end_pt;}
  double &Image_pt         // ret: component of base_pt or end_pt
   (int pt_index,          // in : 0=base_pt, 1=end_pt
    int i)                 { DS_assert(i >= 0 && i < vec_image_dim) ;
                             return(pt_index == 0 ? 
                                    vec_base_pt[i] : vec_end_pt[i]) ; 
                           }

  void                     // eff: calcs a scaled SPAvector for graphics
   Set_image_vec           //      and refreshes end_pt
   (double *image_vec) ;   // in : SPAvector that defines direction
                           //      sized:[vec_image_dim]

  void                     // eff: calcs a base pt for graphics
   Set_base_pt             //      and updates end_pt
   (double *base_pt) ;     // in : the location of the base_pt
                           //      sized:[vec_image_dim]

  void                     // eff: set end_pt and 
   Set_end_pt              //      and update image_vec
   (double *end_pt) ;      // in : the location of the end_pt
                           //      sized:[vec_image_vec]

  // implementations of DS_load virtual functions
  // DS_vector_load non-side-effect functions

  double                   // bas: DS_load virtual function
   Dist2_to_iline          // ret: dist**2 between load and iline
                           //      or -1 when measure makes no sense
                           //      (iline = pi0 + u(pi1-pi0))
    (double *pi0 ,         // in : image space pt, Sized:[image_dim]
     double *pi1 ,         // in : image space pt, Sized:[image_dim]
     int    &pt_index,     // out: pt index of returned dist2_to_iline
     double &uu) ;         // out: iline u val at pt nearest to load

  // implementations of DS_load virtual functions
  // DS_vector_load side-effect functions

  virtual void             // bas: DS_load virtual function
   Shift_domain            // eff: shift the basis image values
    (double *)             // in : du = image shift SPAvector, Sized:Image_dim()
                           { } // no actions for a vector_load

  virtual                  // bas: DS_load virtual function
   int                     // rtn: 0=done,-1=scale too small
   Scale_domain            // eff: scale the basis domain values
    (double)               // in : scale = domain scale factor
                           { return(0) ; }  // no actions for a vector_load

  virtual void             // bas: DS_load virtual function
   Build_this_A            // eff: Add one load's terms to b in Ax=b
    (DS_pfunc *,           // in : tgt_pfunc = the defining parametric eqn
     DS_eqns *)            // in : eqns = holder of Ax=b equation

       { }                 // act: DS_vector_load adds no terms to A

  virtual void             // bas: DS_load virtual function
   Build_this_b            // eff: Add one load's terms to b in Ax=b
    (DS_pfunc *pfunc,      // in : Shape model's parametric function
     DS_eqns *eqns,        // in : out holder of Ax=b, Cx=d eqns
     const double *x = NULL,      // in : used for dynamic loads
     const double *old_x = NULL) ;// in : used for dynamic loads

  virtual                  // bas: DS_load virtual function
   int                     // rtn: 0=no A rebuild,1=A rebuild,-1=bad_dpts
    Update_pts             // eff: update load cached data after
                           //      changes in tgt_pfunc X or image
    (DS_pfunc *pfunc,      // in : Shape model's parametric function
     int pfunc_state=0) ;  // orof: DS_PFN_CHANGED_X      = X val change
                           //       DS_PFN_CHANGED_P_ON   = Set def shape
                           //       DS_PFN_CHANGED_P_OFF  = Clear def shape
                           //       DS_PFN_CHANGED_DOMAIN = Image change
                           
} ; // end class DS_vector_load


//============================================================
// Class DS_attractor
//============================================================

// DS_attractor graphics:  The DS_attractor graphics consist
//                           of one point marker and 3 2pt
//                           polyline markers.
//  cst_grobj[0] = attractor location
//  cst_grobj[1] = x 2-pt polyline
//  cst_grobj[2] = y 2-pt polyline
//  cst_grobj[3] = z 2-pt polyline  (3d image space only)
//  The base point for the SPAvector load is the curve SPAposition
//  u=0.5 or the surface SPAposition u=v=0.5

// DS_attractor domain:    The DS_attractor is applied over
//                           the entire shape domain

// load = gain(W-W0)*x/(W-W0)**(n/2)
//    where W is shape
//          W0 is SPAposition of attractor
//          n  is a measure of locality (0,1,2,...)
class DS_attractor : public DS_load  // identifier = att
{
  protected:
    
   int att_image_dim ;        // [array-size] assoc prob image size
   double *att_image_pt ;     // attractor's image space location
   int     att_power ;        // measure of locality
                              // 0=global, 1,2,3 more local

  public:
   // DS_attractor constructor, copy, Make_copy, Size_arrays, Destructor

   DS_attractor            // Default Constructor
    (DS_dmod *dmod,        // in : owning dmod
     double gain=0.0,      // in : load's gain; user sculpting handle
     DS_pfunc *pfunc=NULL, // in : the shape being loaded
     double *image_pt=NULL,// in : attractor's loc, sized:[image_dim]
     int power=2,          // in : 0=global, 2,3... = local affect
     int tag= -1,          // in : tag so apps can track loads
     DS_load *next=NULL) ; // in : linked list pointer

   DS_attractor                // Copy Constructor
   (DS_attractor &attractor) ; // object being copied

   DS_attractor &operator=     // eff: assignment operator
   (DS_attractor &attractor) ; // object with vals being copied
                            
   DS_load*                // bas: DS_load virtual function
    Make_copy()            // eff: Virtual DS_load copy function
                           { return( ACIS_NEW DS_attractor(*this)) ; }

   void Size_arrays        // eff: allocate internal array memory
    (int image_size=0) ;   // in : assoc geom's domain size

   virtual
  ~DS_attractor() ;       // Destructor


  // DS_attractor simple data access function

  int Image_dim()          { return att_image_dim ; }
  double *Image_pt()       { return att_image_pt ; }

  void Set_image_pt        // eff: set the image_pt value
    (double *image_pt)     // in : ACIS_NEW value, sized:[att_image_dim]
                           { DS_copy_double_block(att_image_pt,
                                                  image_pt,
                                                  att_image_dim) ;
                           }

  int Power()              { return att_power ; }
  void Set_power           // eff: set locality of load
    (int power)            // in : 0=global, 1,2,3... more local
                           { if(power < 0) att_power = 0 ;
                             else          att_power = power ;
                           }
  // implementations of DS_load virtual functions
  // DS_attractor non-side-effect functions

  double                   // bas: DS_load virtual function
   Dist2_to_iline          // ret: dist**2 between load and iline
                           //      or -1 when measure makes no sense
                           //      (iline = pi0 + u(pi1-pi0))
    (double *pi0 ,         // in : image space pt, Sized:[image_dim]
     double *pi1 ,         // in : image space pt, Sized:[image_dim]
     int    &pt_index,     // out: pt index of returned dist2_to_iline
     double &uu) ;         // out: iline u val at pt nearest to load

  // implementations of DS_load virtual functions
  // DS_attractor side-effect functions

  virtual void             // bas: DS_load virtual function
   Shift_domain            // eff: shift the basis image values
    (double *)             // in : du = image shift SPAvector, Sized:Image_dim()
                           { } // no actions for an attractor

  virtual                  // bas: DS_load virtual function
   int                     // rtn: 0=done,-1=scale too small
   Scale_domain            // eff: scale the basis domain values
    (double)               // in : scale = domain scale factor
                           { return(0) ; } // no actions for an attractor                            

  // TODO:??? I bet since the attractor is a lot like
  // a spring that it can be stabilized if we add terms
  // to the A SPAmatrix.  Consider updating this function.
  // If this change is made then the return
  // value of Update_pts() needs to change as well.
  virtual void             // bas: DS_load virtual function
   Build_this_A            // eff: Add one load's terms to b in Ax=b
    (DS_pfunc *,           // in : tgt_pfunc = the defining parametric eqn
     DS_eqns * )           // in : eqns = holder of Ax=b equation
       { }                 // act: DS_attractor adds no terms to A

  virtual void             // bas: DS_load virtual function
   Build_this_b            // eff: Add one load's terms to b in Ax=b
    (DS_pfunc *pfunc,      // in : Shape model's parametric function
     DS_eqns *eqns,        // in : out holder of Ax=b, Cx=d eqns
     const double *x = NULL,      // in : used for dynamic loads
     const double *old_x = NULL) ;// in : used for dynamic loads

  virtual                  // bas: DS_load virtual function
   int                     // rtn: 0=no A rebuild,1=A rebuild,-1=bad_dpts
    Update_pts             // eff: update load cached data after
                           //      changes in tgt_pfunc X or image
    (DS_pfunc *pfunc,      // in : Shape model's parametric function
     int pfunc_state=0) ;  // orof: DS_PFN_CHANGED_X      = X val change
                           //       DS_PFN_CHANGED_P_ON   = Set def shape
                           //       DS_PFN_CHANGED_P_OFF  = Clear def shape
                           //       DS_PFN_CHANGED_DOMAIN = Image change
                           
} ; // end class DS_attractor

//============================================================
// Class DS_spring
//============================================================

// An attractor is a sliding spring.  One end of the spring is
// fixed to a location within the surface (given by a domain point)
// and the other end is fixed in image space to the image point

// DS_spring graphics:  The DS_spring graphics consists of one
//                      pair of end point markers and a line segment
//                      stored as a two point polyline.
//  cst_grobj[0] = end point 1
//  cst_grobj[1] = end point 2
//  cst_grobj[2] = 2-pt polyline
//          

class DS_spring : public DS_load  // identifier = spr
{
  protected:
   int spr_elem_index ;    // cache the elem_index for the pt. When
                           // set to -1 it is a flag to recompute 
                           // the uv, duv, and udv basis values
                           // in call Calc_ipt_idir()
   int spr_slide_state ;   // 0=fixed, 1=sliding

   int spr_elem_dof_count; // [array_size] max elem dof count
   int spr_image_dim ;     // [array_size] problem image dim size
   int spr_domain_dim ;    // [array_size] problem domain dim size

   double *spr_pos ;       // pt's basis vals (surf = uv, surf =  u)
   double *spr_d1 ;        // pt's basis vals (surf =duv, surf = du)
   double *spr_d2 ;        // pt's basis vals (surf =udv, surf =duu)

   double *spr_surf_pt ;   // image pt of domain_pt on surface shape
   double *spr_surf_dir ;  // image tangs on surface shape at domain pt
   double *spr_def_pt ;    // image loc of domain_pt on default shape
   double *spr_free_pt ;   // free image pt, Sized[spr_image_dim]
                           // Stored:[xx, yy, zz]

   double *spr_domain_pt ; // domain point constraint SPAposition
                           // Sized:[spr_domain_dim]

   int *spr_elem_dof_map ; // map basis functions to global dofs

  public:
   // DS_spring constructor, copy, Make_copy, Size_arrays, Destructor

   DS_spring                // Default Constructor
    (DS_dmod *dmod,         // owning dmod
     double gain=0.0,       // the load's gain, user sculpting handle
     DS_pfunc *
           tgt_pfunc=NULL,  // shape used to calc ipt for dpt (not saved)
     double *domain_pt=NULL,// load domain loc,Sized:[pfunc->Domain_dim()]
     double *free_pt=NULL,  // load image loc, Sized:[pfunc->Image_dim()]
                            // When(free_pt==NULL)set free_pt = 
                            //    image_loc of domain_pt on surface
     int slide_state=0,     // 0=fixed,1=sliding
     int tag= -1,           // tag so apps can track loads
     DS_load *next=NULL) ;  // linked list pointer

   DS_spring               // eff: Copy Constructor
   (DS_spring &spring) ;   // in : object being copied

   DS_spring &operator=    // eff: assignment operator
    (DS_spring &spring) ;

   DS_load*                // bas: DS_load virtual function
    Make_copy()            // eff: Virtual DS_load copy function
                           { return( ACIS_NEW DS_spring(*this)) ; }

   void
    Size_arrays            // eff: Size_arrays
     (int elem_dof_count=0,// in : max dof_count per elem in tgt_pfunc
      int image_dim=0,     // in : Size of the problem's image space
      int domain_dim=0) ;  // in : Size of the problem's domain space

   virtual
  ~DS_spring() ;            // eff: Destructor


  // DS_spring simple data access function

  int     Elem_index()     { return spr_elem_index ; }
  int     Slide_state()    // rtn: 0=fixed,1=sliding
                           { return spr_slide_state ; }
  void    Set_slide_state  // eff: load the slide_state value
   (int slide_state)       // in : 0=fixed,1=sliding
                           { spr_slide_state = slide_state ;
                           }

  int     Image_dim()      { return spr_image_dim ; }
  int     Elem_dof_count() { return spr_elem_dof_count ; }
  int    *Elem_dof_map()   { return spr_elem_dof_map ; }
  double *Basis_uv()       { return spr_pos ; }
  double *Surf_pt()        { return spr_surf_pt ; }
  double &Surf_pt(int d)   { DS_assert(d >= 0 && d < spr_image_dim) ;
                             return( spr_surf_pt[d]) ; }
  double *Surf_dir()       { return spr_surf_dir ; }
  double *Surf_dir(int d)  { DS_assert(d >= 0 && d < spr_domain_dim) ;
                             return( spr_surf_dir + d*spr_image_dim) ; }
  double *Def_pt()         { return spr_def_pt ; }
  double &Def_pt(int d)    { DS_assert(d >= 0 && d < spr_image_dim) ;
                             return( spr_def_pt[d]) ; }
 
  double *Free_pt()        { return spr_free_pt ; }
  double &Free_pt(int d)   { DS_assert(d >= 0 && d < spr_image_dim) ;
                             return( spr_free_pt[d]) ; }
  int     Domain_dim()     { return spr_domain_dim ; }
  double *Domain_pt()      { return spr_domain_pt ; }
  double &Domain_pt(int d) { DS_assert(d >= 0 && d < spr_domain_dim) ;
                             return( spr_domain_pt[d]) ; }

  void Set_free_pt         // eff: copies ipt into cstrn
   (double *ipt)           // in : pt to copy, Sized:Domain_pt_size()
                           { DS_assert(ipt != NULL) ;
                             DS_copy_double_block(spr_free_pt,
                                                  ipt,
                                                  spr_image_dim) ;
                           }

  int                      // rtn: 0=success,-1=bad_dpt 
   Set_domain_pt           // eff: copies dpt into cstrn, updates ipt
   (double *dpt,           // in : pt to copy, Sized:Domain_pt_size()
    DS_pfunc *tgt_pfunc)   // in : the shape being loaded

                           { int rtn = 0 ;
                             spr_elem_index = -1 ;
                             DS_assert(dpt != NULL) ;
                             DS_copy_double_block(spr_domain_pt,
                                                  dpt,
                                                  spr_domain_dim) ;
                             if(tgt_pfunc)
                               rtn = Calc_ipt_idir(tgt_pfunc) ;
                             return(rtn) ;
                           }

  int Basis_block_size     // ret: double count bigger than mem_blk
   (int elem_dof_count,    // in : max dof_count per elem in pfunc
    int image_dim,         // in : image dim
    int domain_dim)        // in : domain dim
                           { return(  elem_dof_count       // spr_pos
                                    + elem_dof_count       // spr_d1
                                    + elem_dof_count       // spr_d2
                                    + image_dim            // spr_surf_pt
                                    + image_dim*domain_dim // spr_surf_dir
                                    + image_dim            // spr_def_pt
                                    + image_dim            // spr_free_pt
                                    + domain_dim           // spr_domain_pt
                                    + Elem_dof_map_size    // spr_elem_dof_map
                                         (elem_dof_count)) ;
                           }

  int Elem_dof_map_size    // ret: size of dof_map_size in doubles
   (int elem_dof_count)    // in : max dof_count per elem in pfunc
                           { return( (int)ceil            
                                      (  elem_dof_count  
                                        * (double)sizeof(int)
                                        / (double)sizeof(double))) ;
                           }

  //  implementations of DS_load virtual functions
  // DS_spring non-side-effect functions

  double                   // bas: DS_load virtual function
   Dist2_to_iline          // ret: dist**2 between load and iline
                           //      or -1 when measure makes no sense
                           //      (iline = pi0 + u(pi1-pi0))
    (double *pi0,          // in : image space pt, Sized:[image_dim]
     double *pi1,          // in : image space pt, Sized:[image_dim]
     int    &pt_index,     // out: pt index of returned dist2_to_iline
     double &uu) ;         // out: iline u val at pt nearest to load

  // DS_spring side-effect functions

  int                      // rtn: 0=success,-1=bad_dpts                     
   Calc_ipt_idir           // eff: update def_pt,image_pt,image_dir
    (DS_pfunc *tgt_pfunc); // in : Shape Model, parametric function

  int                      // rtn: 0=no slide,1=pt slid
   Slide_pt                // eff: move Free_pt() under Surf_pt() 
    (DS_pfunc *pfunc) ;    // in : shape being loaded
                                                                       
  //  implementations of DS_load virtual functions
  // DS_spring side-effect functions

  virtual void             // bas: DS_load virtual function
   Shift_domain            // eff: shift the basis domain values
    (double *du)           // in : domain shift SPAvector, Sized:Domain_dim()
                           { DS_assert(du) ;
                             for(int ii=0;ii<spr_domain_dim;ii++)
                               spr_domain_pt[ii] += du[ii] ;
                           }

  virtual                  // bas: DS_load virtual function
   int                     // rtn: 0=done,-1=scale too small
   Scale_domain            // eff: scale the basis domain values
    (double scale)         // in : domain scale factor
                           { // no work condition
                             if(DS_is_zero(scale,DS_DOUBLE_CLOSE))
                               return(-1) ;
                             // scale
                             for(int ii=0;ii<spr_domain_dim;ii++) 
                               { spr_domain_pt[ii] *= scale ;
                               }
                             return(0) ;
                           }
                            
  virtual void             // bas: DS_load virtual function
   Build_this_A            // eff: Add one load's terms to A in Ax=b
    (DS_pfunc *tgt_pfunc,  // in : the defining parametric equation
     DS_eqns *eqns) ;      // in : holder of Ax=b equation

  virtual void             // bas: DS_load virtual function
   Build_this_b            // eff: Add one load's terms to b in Ax=b
    (DS_pfunc *tgt_pfunc,  // in : Shape model's parametric function
     DS_eqns *eqns,        // in : out holder of Ax=b, Cx=d eqns
     const double *x = NULL,      // in : used for dynamic loads
     const double *old_x = NULL) ;// in : used for dynamic loads

  virtual                  // bas: DS_load virtual function
   int                     // rtn: 0=no A rebuild,1=A rebuild,-1=bad_dpts
    Update_pts             // eff: update load cached data after
                           //      changes in tgt_pfunc X or domain
    (DS_pfunc *pfunc,      // in : Shape model's parametric function
     int pfunc_state=0) ;  // orof: DS_PFN_CHANGED_X      = X val change
                           //       DS_PFN_CHANGED_P_ON   = Set def shape
                           //       DS_PFN_CHANGED_P_OFF  = Clear def shape
                           //       DS_PFN_CHANGED_DOMAIN = Domain change
                           
} ; // end class DS_spring

//============================================================
// Class DS_spring_set
//============================================================

// A spring_set is a set of springs where each spring is a single
// point attractor with one end attached to a fixed location in
// image space and the other end attached to a point on the
// defining DS_pfunc and specified by a domain point.

// Four points are associated with each spring in the set
// Domain_pt() = domain space specification for spring end on DS_pfunc
// Surf_pt()   = image space spring end location on  the DS_pfunc
// Def_pt()    = image space default location for Domain_pt()
// Free_pt()   = image space spring end location off the DS_pfunc

// DS_spring_set graphics:  The DS_spring_set graphics consists of a
//                      pair of end point markers and a line segment
//                      stored as a two point polyline for each
//                      spring in the spring set
//  cst_grobj[3*ipt  ] = end point
//  cst_grobj[3*ipt+1] = end point
//  cst_grobj[3*ipt+2] = 2-pt polyline
//          

class DS_spring_set : public DS_load  // identifier = sps
{
  protected:
   int sps_comp_flag ;     // oneof 0|-1
                           //  0 = use saved basis uv values
                           // -1 = recompute basis uv values
                           // in call Calc_ipt_idir()
   int sps_pt_count ;      // [array_size] number of pts in point set
   int sps_image_dim ;     // [array_size] problem image dim size
   int sps_domain_dim ;    // [array_size] problem domain dim size
   int sps_elem_dof_count; // [array_size] max elem dof count

   double *sps_domain_pt ; // domain point positions
                           //    Stored:[uv0,uv1..,uvN]
                           //     Sized:[domain_dim * pt_count]

   double *sps_surf_pt ;   // image loc of domain_pt on surface shape
                           //    Stored:[xyz0,xyz1..,xyzN]
                           //     Sized:[image_dim * pt_count]

   double *sps_def_pt ;    // image loc of domain_pt on default shape
                           //    Stored:[xyz0,xyz1..,xyzN]
                           //     Sized:[image_dim * pt_count]

   double *sps_free_pt ;   // free image pt
                           //    Stored:[xyz0,xyz1..,xyzN]
                           //     Sized:[image_dim * pt_count]

   double *sps_pos ;        // elem uv basis values for each domain_pt
                           //    Stored:[bas_blk0..,bas_blkN]
                           //     Sized:[elem_dof_count * pt_count]

   // note: we choose not to store elem_index and elem_dof_map
   //       these are computed on the fly as needed

  public:
   // DS_spring_set constructor, copy, Make_copy, Size_arrays, Destructor

   DS_spring_set           // Default Constructor
   (DS_dmod *dmod,         // owning dmod
    double  gain=0.0,      // the load's gain
    DS_pfunc *
            tgt_pfunc=NULL,// loaded shape (not saved)
    int     pt_count=0,    // number of springs in spring_set
    double *domain_pt=NULL,// load domain loc,Stored:[uv0,uv1..,uvN]
                           //   Sized:[pfunc->Domain_dim()*pt_count]
    double *free_pt=NULL,  // load image loc, Stored:[xyz0..,xyzN]
                           //   Sized:[pfunc->Image_dim()*pt_count]
                           // When(free_pt==NULL)set free_pt = 
                           //    image_loc of domain_pt on surface
    int     tag= -1,       // tag so apps can track loads
    DS_load *next=NULL) ;  // linked list pointer

   DS_spring_set           // eff: Copy Constructor
   (DS_spring_set &
           spring_set) ;   // in : object being copied

   DS_spring_set &operator=// eff: assignment operator
    (DS_spring_set &spring_set) ;

   DS_load*                // bas: DS_load virtual function
    Make_copy()            // eff: Virtual DS_load copy function
                           { return( ACIS_NEW DS_spring_set(*this)) ; }

   void
   Size_arrays             // eff: Size_arrays
    (int pt_count=0,       // in : count of springs in spring_set
     int image_dim=0,      // in : Size of the problem's image space
     int domain_dim=0,     // in : Size of the problem's domain space
     int elem_dof_count=0);// in : max dof_count per elem in tgt_pfunc

   virtual
  ~DS_spring_set() ;       // eff: Destructor


  // DS_spring_set simple data access function

  int     Comp_flag()      // rtn: 0=ok to use clean uv basis values
                           //     -1=recompute dirty uv basis values
                           { return sps_comp_flag ; }

  int     Pt_count()       { return sps_pt_count ; }
  int     Image_dim()      { return sps_image_dim ; }
  int     Domain_dim()     { return sps_domain_dim ; }
  int     Elem_dof_count() { return sps_elem_dof_count ; }

  double *Domain_pt()      { return sps_domain_pt ; }
  double *Domain_pt(int p) { DS_assert(p >= 0 && p < sps_pt_count) ;
                             return sps_domain_pt+p*sps_domain_dim ;
                           } 
  // note: after a domain_pt change use Calc_ipts() to compute 
  //       Surf_pt and Def_pt

  double *Surf_pt()        { return sps_surf_pt ; }
  double *Surf_pt(int p)   { DS_assert(p >= 0 && p < sps_pt_count) ;
                             return sps_surf_pt + p*sps_image_dim ;
                           } 

  double *Def_pt()         { return sps_def_pt ; }
  double *Def_pt(int p)    { DS_assert(p >= 0 && p < sps_pt_count) ;
                             return sps_def_pt + p*sps_image_dim ;
                           } 

  double *Free_pt()        { return sps_free_pt ; }
  double *Free_pt(int p)   { DS_assert(p >= 0 && p < sps_pt_count) ;
                             return sps_free_pt + p*sps_image_dim ;
                           }
  void Set_free_pt         // eff: change the free pt loc of a spring
    (int p,                // in : index of free_pt
     double *ipt)          // in : new image loc, sized:[sps_image_dim]
                           { DS_assert(p >= 0 && p < sps_pt_count) ;
                             DS_copy_double_block
                                   (sps_free_pt+sps_image_dim*p,
                                    ipt,
                                    sps_image_dim) ;
                           }

  double *Pos_basis()      { return sps_pos ; }
  double *Pos_basis(int p) { DS_assert(p >= 0 && p < sps_pt_count) ;
                             return sps_pos + p*sps_elem_dof_count ;
                           } 

  int Basis_block_size     // ret: double count bigger than mem_blk
   (int pt_count,          // in : spring count
    int image_dim,         // in : image dim
    int domain_dim,        // in : domain dim
    int elem_dof_count)    // in : max dof_count per elem in pfunc
                           { return
                              (  pt_count
                               * (  domain_dim     // sps_domain_pt
                                  + image_dim      // sps_surf_pt
                                  + image_dim      // sps_def_pt
                                  + image_dim      // sps_free_pt
                                  + elem_dof_count // sps_pos
                                  ) ) ;
                           }

  //  implementations of DS_load virtual functions
  // DS_spring_set non-side-effect functions

  double                   // bas: DS_load virtual function
   Dist2_to_iline          // ret: dist**2 between load and iline
                           //      or -1 when measure makes no sense
                           //      (iline = pi0 + u(pi1-pi0))
    (double *pi0,          // in : image space pt, Sized:[image_dim]
     double *pi1,          // in : image space pt, Sized:[image_dim]
     int    &pt_index,     // out: pt index of returned dist2_to_iline
     double &uu) ;         // out: iline u val at pt nearest to load

  // DS_spring_set side-effect functions

  int                      // rtn: 0=success,-1=bad_dpts
                           // use: after domain_pt or pfunc changes
   Calc_ipts_and_uvs       // eff: update def_pt, image_pt, and uv
    (DS_pfunc *tgt_pfunc); // in : Shape Model, parametric function
                           // com: if(sps_comp_flag < 0) calc sps_pos
                           //      else                  use  sps_pos

  // DS_spring_set implementations of DS_load virtual functions
  //  side-effect functions

  virtual void             // bas: DS_load virtual function
   Shift_domain            // eff: shift the basis domain values
    (double *du)           // in : domain shift SPAvector, Sized:Domain_dim()
                           { DS_assert(du) ;
                             for(int i0=0,jj=0;jj<sps_pt_count;jj++)
                               for(int ii=0;ii<sps_domain_dim;ii++,i0++)
                                 {  sps_domain_pt[i0] += du[ii] ; }
                           }

  virtual                  // bas: DS_load virtual function
   int                     // rtn: 0=done,-1=scale too small
   Scale_domain            // eff: scale the basis domain values
    (double scale)         // in : domain scale factor
                           { // no work condition
                             if(DS_is_zero(scale,DS_DOUBLE_CLOSE))
                               return(-1) ;
                             // scale
                             for(int i0=0,jj=0;jj<sps_pt_count;jj++)
                               for(int ii=0;ii<sps_domain_dim;ii++,i0++)
                                 {  sps_domain_pt[i0] *= scale ; }
                             return(0) ;
                           } 

  virtual void             // bas: DS_load virtual function
   Build_this_A            // eff: Add one load's terms to A in Ax=b
    (DS_pfunc *tgt_pfunc,  // in : the defining parametric equation
     DS_eqns *eqns) ;      // in : holder of Ax=b equation

  virtual void             // bas: DS_load virtual function
   Build_this_b            // eff: Add one load's terms to b in Ax=b
    (DS_pfunc *tgt_pfunc,  // in : Shape model's parametric function
     DS_eqns *eqns,        // in : out holder of Ax=b, Cx=d eqns
     const double *x = NULL,      // in : used for dynamic loads
     const double *old_x = NULL) ;// in : used for dynamic loads

  virtual                  // bas: DS_load virtual function
   int                     // rtn: 0=no A rebuild,1=A rebuild,-1=bad_dpts
    Update_pts             // eff: update load cached data after
                           //      changes in tgt_pfunc X or domain
    (DS_pfunc *pfunc,      // in : Shape model's parametric function
     int pfunc_state=0) ;  // orof: DS_PFN_CHANGED_X      = X val change
                           //       DS_PFN_CHANGED_P_ON   = Set def shape
                           //       DS_PFN_CHANGED_P_OFF  = Clear def shape
                           //       DS_PFN_CHANGED_DOMAIN = Domain change
                           
} ; // end class DS_spring_set

//=============================================================
//  class DS_crv_load
//=============================================================

// Think of DS_crv_load as an extruded pt_spring load
//    The shape of the tgt_pfunc is modified by a distributed
//    spring that is attached to the src_pfunc on one end
//    and the tgt_pfunc on the other.  The binding to the
//    tgt_pfunc is represented by a DS_tprod_1d and the
//    spring connects pts of common parameterization.
//    
// nomenclature: tgt_pfunc is the pfunc being loaded
//               src_pfunc gives the crv_load's image_space shape
//     W(uv) = shape of tgt_pfunc for domain_pt uv
//     W(uv) = Sum(xi*Phii(uv))
//     W'(s) = shape of src_pfunc for domain_pt s
//     C(s)  = shape of SPAparameter curve in the tgt_pfunc domain
//         X = P + Q
//      X = total dof  (xi)
//      P = default shape  (pi)
//      Q = displacement shape  (qi)
//
// Force of a crv_load  (let uv = C(s))
//      f(s) = gain*(W'(s) - W(C(s)))
// Compute A and b terms in Aq = b
//      bi = Ntgrl(Phii(uv) * f(uv)) duv
//      but f(uv) is zero everywhere except where uv = C(s)
//   so bi = Ntgrl(Phii(C(s))*gain*(W'(s) - Sumj(xj*Phij(C(s))))) ds
//  and bi = gain*(-Sumj(Ntgrl(Phii(C(s))*Phij(C(s))*xj) ds)
//                 +     Ntgrl(Phii(C(s))*W'(s)) ds)
//  expand xj = (xj - pj) + pj
//
//  For stability move all terms with xj-pj into the A SPAmatrix
//      and leave all other terms in the b SPAmatrix of Aq=b
//
//  so Aij = gain*Ntgrl(Phii(C(s))*Phij(C(s)) (the xj-pj coefficients)
//     bi  = gain*(-Sumj(Ntgrl(Phii(C(s))*Phij(C(s))*pj) ds)
//                 +     Ntgrl(Phii(C(s))*W'(s)) ds
//
// DS_CRV_LOAD graphics:  The crv_load graphics consists of one
//                         polyline and a pair of end point markers.
//  cst_grobj[0] = pair of end points
//  cst_grobj[1] = polyline
//          


// DS_crv_load graphics: The DS_spring_set graphics consists of a
//                       2 polylines and a line-segment for
//                       each image point
//  cst_grobj[0]  = surf_polyline
//  cst_grobj[1]  = free_polyline
//  cst_grobj[ii] = 2-pt polyline for each image_pt
//          
/*class DS_crv_load : public DS_load  // identifier = crl
{
  protected:

   int crl_surf_dof_count; // [array_size] tgt_pfunc dof_count 
   int crl_elem_dof_count; // [array_size] tgt_pfunc elem_dof_count
   int crl_image_dim ;     // [array_size] problem image dim size
   int crl_ntgrl_degree ;  // [array_size] gauss integration accuracy
   int crl_seg_count ;     // [array_size] count of param curve
                           // segments that lie completely within
                           // one tgt_pcurve element

   DS_pfunc *              // The crv_load's param_curve shape
           crl_domain_crv;   
   DS_pfunc *              // The crv_load's image space shape
           crl_src_pfunc ;

   double *crl_seg_bnds ;  // domain curve param vals at segment end-pts.
                           // Segs lie between span end-pts & xsect-
                           // pts with tgt_pfunc knot boundaries.
                           // 1st and last entries must equal parm 
                           // range for entire curve load.
                           // Sized:[Seg_bnds_size(seg_count)]
   double *crl_tgt_ipts ;  // seg gauss-pt image-locs plus 2 end-pts
                           // on tgt_pfunc.  Sized:[Image_pts_size()]
                           // Order:[xyz0..,xyzN]Cnt=Image_pt_count()
   double *crl_tgt_bas ;   // tgt_pfunc basis_uvs for each tgt_pt
                           // Sized:[Basis_pts_size()]
                           // Order:[pt0[bas0..,basM]..,ptN[...]]
   double *crl_src_ipts ;  // seg gauss-pt image-locs plus 2 end-pts
                           // on src_pfunc.  Sized:[Image_pts_size()]
                           // Order:[xyz0..,xyzN]Cnt=Image_pt_count()
   double *crl_pick_pt ;   // Set for display by Dist2_to_iline()
                           //    Sized:[crl_image_dim]
   double *crl_b ;         // Crv load's b SPAmatrix entries
                           //    Sized:[ crl_image_dim
                           //           *crl_surf_dof_count]
                           //    Order:[x0..,xN,y0..,yN,z0..,zN]
   int    *crl_tgt_elem ;  // The tgt_elem associated to each ipt
                           // Sized:[Image_pts_size()],[e0,...eN]
  public:
   // DS_crv_load Constructor, copy, Make_copy, Size_arrays, Destructor

   DS_crv_load                  // Default Constructor
    (double    gain     =0.0,   // in : initial load gain
     DS_pfunc *tgt_pfunc=NULL,  // in : The shape being loaded.
                                //      not stored/deleted with load
     DS_pfunc *domain_crv =NULL,// in : load domain curve shape.
                                //      stored/deleted with load
     DS_pfunc *src_pfunc=NULL,  // in : load image space shape.
                                //      stored/deleted with load
     int       seg_count= 0,    // in : segment count in seg_bnds
     double   *seg_bnds =NULL,  // in : ordered list of curve param 
                                //      vals marking segment end 
                                //      pts. Sized:[seg_count+1]
     int       tag      = -1,   // in : tag so apps can track loads
     DS_load  *next     =NULL); // in : linked list pointer 

                             // eff: Copy Constructor
   DS_crv_load               // rtn: deep copy of crv_load
    (DS_crv_load &crv_load); // in : object being copied

   DS_crv_load &operator=  // eff: assignment operator
    (DS_crv_load &crv_load);                 

   DS_load*                // bas: DS_load virtual function
    Make_copy()            // eff: Virtual DS_load copy function
                           { return( new DS_crv_load(*this)) ; }

   void
   Size_arrays             // eff: Size_arrays
    (int surf_dof_count=0, // in : tgt_pfunc dof_count
     int elem_dof_count=0, // in : tgt_pfunc dofs per elem count
     int image_dim=0,      // in : tgt_pfunc image_dim
     int ntgrl_degree=0,   // in : gauss integration accuracy
     int seg_count=0) ;    // in : domain_crv single_elem segment cnt

   virtual
   ~DS_crv_load() ;        // eff: Destructor
                           //   delete crl_domain_crv ;
                           //   Size_arrays() ;

  // DS_crv_load simple data access function

  int     Surf_dof_count() { return crl_surf_dof_count ; }
  int     Image_dim()      { return crl_image_dim ; }
  int     Ntgrl_degree()   { return crl_ntgrl_degree ; }
  int     Seg_count()      { return crl_seg_count ; }

  int Image_pt_count()     // rtn: the current ipt_count in memory
                           { return (  crl_seg_count == 0 
                                     ? 0
                                     :(  2 
                                       + crl_seg_count
                                       * DS_linear_gauss_pt_count
                                               (crl_ntgrl_degree)));
                           }
  int Image_pt_count       // rtn: the expected ipt_count for inputs
   (int ntgrl_degree,      // in : gauss integration accuracy
    int seg_count)         // in : number of segs in domain_crv
                           { return (  seg_count == 0 
                                     ? 0 
                                     :(  2 
                                       + seg_count
                                       * DS_linear_gauss_pt_count
                                               (ntgrl_degree))) ;
                           }

  DS_pfunc *Domain_crv()   { return crl_domain_crv ; }
  DS_pfunc *Src_pfunc()    { return crl_src_pfunc ; }

  double *Seg_bnds()       // ret: seg_bnd array, stored[u0,u1..,un]
                           { return crl_seg_bnds ; }
  double  Seg_bnds(int i)  { DS_assert(i >= 0 && i < crl_seg_count+1);
                             return(crl_seg_bnds[i]) ;
                           }

  double *Tgt_ipts()       // rtn: tgt image_pt array [xyz0..,xyzN]
                           { return crl_tgt_ipts ; }
  double *Tgt_ipts(int i)  { DS_assert(i >= 0 && i<Image_pt_count());
                             return(crl_tgt_ipts + i*crl_image_dim);
                           }

  double *Tgt_pt_bas()     // rtn: tgt pt bas array [xyz0..,xyzN]
                           { return crl_tgt_bas ; }
  double *Tgt_pt_bas       // rtn: ith tgt_pt bas array [b0,b1..,bN]
           (int i)         // in : tgt_pt index
                           { DS_assert(i >= 0 && i<Image_pt_count());
                             return(  crl_tgt_bas 
                                    + i*crl_elem_dof_count);
                           }
  int    *Tgt_pt_elem()    // rtn: tgt_pt elem_number array [e0..,eN]
                           { return crl_tgt_elem ; }
  int     Tgt_pt_elem      // rtn: tgt_pt elem_number of ith tgt_pt
           (int i)         // in : tgt_pt index
                           { DS_assert(i >= 0 && i<Image_pt_count());
                             return(crl_tgt_elem[i]) ;
                           }

  double *Src_ipts()       // rtn: src image_pt array [xyz0..,xyzN]
                           { return crl_src_ipts ; }
  double *Src_ipts         // rtn: ith src image_pt [xyz]
           (int i)         // in : image_pt index
                           { DS_assert(i >= 0 && i<Image_pt_count());
                             return(crl_src_ipts + i*crl_image_dim);
                           }

  double *Pick_pt()        { return crl_pick_pt ; }

  double *B()              // rtn: b array, [x0..,xN,y0..,yN,z0..,zN]
                           { return crl_b ; }
  double *B                // rtn: one dof_image_block of b array
          (int i)          // in : image_index, e.g. 0=x,1=y,2=z
                           { DS_assert(i >= 0 && i < crl_image_dim);
                             return(crl_b + i*crl_surf_dof_count) ;
                           }
  double &B                // rtn: b for image_dim & glob_dof_number
          (int i,          // in : image_index, e.g. 0=x,1=y,2=z
           int d)          // in : global dof_index
                           { DS_assert(i>=0 && i<crl_image_dim);
                             DS_assert(d>=0 && d<crl_surf_dof_count);
                             return(crl_b[i*crl_surf_dof_count+d]) ;
                           }

  // DS_crv_load internal array size functions
                                
  int Seg_bnds_size        // rtn: seg_cuts array size
    (int seg_count)        // in : domain_crv single elem segment cnt
                           { return(seg_count==0 ? 0 : seg_count+1); 
                           }

  int Image_pts_size       // rtn: image_pts array size
    (int ntgrl_degree,     // in : gauss integration accuracy
     int image_dim,        // in : tgt_pfunc image_dim
     int seg_count)        // in : domain_crv single elem segment cnt 
                           { return(  seg_count == 0 ? 0
                                    : Image_pt_count(ntgrl_degree,
                                                     seg_count)
                                      * image_dim) ;
                           } 
  int Tgt_pt_elem_size     // rtn: tgt_pt_elem size in doubles
    (int ntgrl_degree,     // in : gauss integration accuracy
     int seg_count)        // in : domain_crv single elem segment cnt
                           { return
                             ((int)ceil( Image_pt_count(ntgrl_degree,
                                                        seg_count)
                                        * (double)sizeof(int)
                                        / (double)sizeof(double))) ;
                           }

  int Basis_pts_size       // rtn: crl_tgt_bas array size
   (int ntgrl_degree,      // in : gauss integration accuracy
    int tgt_elem_dof_count,// in : number of dofs per tgt_pfunc elem
    int seg_count)         // in : domain_crv single elem segment cnt 
                           { return(  seg_count == 0 ? 0
                                    : (  tgt_elem_dof_count
                                       * (  2 + seg_count
                                          * DS_linear_gauss_pt_count
                                                 (ntgrl_degree)))
                                   ) ;
                           } 

  int B_size               // rtn: b array size
    (int surf_dof_count,   // in : tgt_pfunc dof_count
     int image_dim)        // in : tgt_pfunc image_dim
                           { return (image_dim * surf_dof_count) ; }

  int Block_size           // rtn: Sum of all array sizes
    (int surf_dof_count,   // in : tgt_pfunc dof_count
     int elem_dof_count,   // in : tgt_pfunc dofs per elem count
     int image_dim,        // in : tgt_pfunc image_dim
     int ntgrl_degree,     // in : gauss integration accuracy
     int seg_count)        // in : domain_crv single elem segment cnt
                           { return 
                              (  Seg_bnds_size       // crl_seg_bnds
                                    (seg_count)  
                               + Basis_pts_size      // crl_tgt_bas
                                    (ntgrl_degree,   
                                     elem_dof_count,      
                                     seg_count)  
                               + 2 * Image_pts_size  // crl_tgt_ipts
                                    (ntgrl_degree,   // crl_src_ipts
                                     image_dim,      
                                     seg_count)  
                               + image_dim           // crl_pick_pt
                               + B_size
                                    (surf_dof_count, // crl_b
                                     image_dim)
                               + Tgt_pt_elem_size    // crl_tgt_elem
                                    (ntgrl_degree,
                                     seg_count)) ; 
                           }
  // DS_crv_load internal side-effect functions

   int                     // rtn: 0=success,-1=bad_dpts
                           // use: after tgt_pfunc P val or domain
                           //      changes or src_pfunc val changes
   Calc_ipts_and_b         // eff: refresh all cached data arrays
    (DS_pfunc *tgt_pfunc); // in : Shape model's parametric function

                           // use: after calc_ipts_and_b() and each
  void                     //      time tgt_pfunc X vals change
   Calc_ipts               // eff: calc tgt_ipts using cached data
    (DS_pfunc *tgt_pfunc); // in : Shape model's parametric function

  // DS_crv_load implementations of DS_load virtual functions
  //             non-side-effect functions

  virtual double           // bas: DS_load virtual function
   Dist2_to_iline          // ret: dist**2 between load and iline
                           //      iline = pi0 + u(pi1-pi0)
    (double *pi0,          // in : image space pt, Sized:[image_dim]
     double *pi1,          // in : image space pt, Sized:[image_dim]
     int    &pt_index,     // out: pt index of returned dist2_to_iline
     double &uu) ;         // out: iline param for pt closest to load
 
  // DS_crv_load implementations of DS_load virtual functions
  //             side-effect functions

  virtual void             // bas: DS_load virtual function
   Shift_domain            // eff: shift the basis domain values
    (double *du) ;         // in : domain shift SPAvector, Sized:Domain_dim()

  virtual                  // bas: DS_load virtual function
   int                     // rtn: 0=done,-1=scale too small
   Scale_domain            // eff: scale the basis domain values
    (double scale) ;       // in : domain scale factor

  virtual void             // bas: DS_load virtual function
   Build_this_A            // eff: Add one load's terms to A in Ax=b
    (DS_pfunc *tgt_pfunc,  // in : the defining parametric equation
     DS_eqns *eqns) ;      // in : holder of Ax=b equation

  virtual void             // bas: DS_load virtual function
   Build_this_b            // eff: Add one load's terms to b in Ax=b
    (DS_pfunc *tgt_pfunc,  // in : Shape model's parametric function
     DS_eqns *eqns,        // in : out holder of Ax=b, Cx=d eqns
     const double *x = NULL,      // in : used for dynamic loads
     const double *old_x = NULL) ;// in : used for dynamic loads

  virtual                  // bas: DS_load virtual function
   int                     // rtn: 0=no A rebuild,1=A rebuild,-1=bad_dpts
    Update_pts             // eff: update load cached data after
                           //      changes in tgt_pfunc X or domain
    (DS_pfunc *tgt_pfunc,  // in : Shape model's parametric function
     int pfunc_state=0) ;  // orof: DS_PFN_CHANGED_X      = X val change
                           //       DS_PFN_CHANGED_P_ON   = Set def shape
                           //       DS_PFN_CHANGED_P_OFF  = Clear def shape
                           //       DS_PFN_CHANGED_DOMAIN = Domain change
                           
} ; // end class DS_crv_load
*/

#endif // DS_load_H

