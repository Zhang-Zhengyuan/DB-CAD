/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dsbasis.hxx
// created  : Jan 4, 1995
// author   : GWCelniker
// contains : Class DS_basis, DS_bspln
// synopsis : Class DS_bspln represents B-spline surfaces
//            A B-spline is a B-spline but the equations for 
//            a B-spline can look very different depending on which
//            parameters are being used to write the equations.
//            In this work:
//            Bi,n = 1D B-spline basis function whose non-zero range
//                      is ui-1 <= u < ui+n-1
//                 This choice is made because the the first and
//                 last basis functions use only n knots while
//                 all the rest use n+1 knots.  The first knot
//                 is numbered 0 and is used as the lowest
//                 knot in both B0,n and B1,n.
//            n = polynomial order (degree + 1 = order)
//            k = polynomial degree
//
// knots    : knots are stored in an array of unique knot values
//            with a separate integer array stored to record
//            knot multiplicities called count.  The count array
//            records the highest knot number index that shares
//            a common knot value. The knots can be referred
//            to by their knot number or by their knot index
//            the knot_index refers to the unique knot value count
//            the knot_number refers to the total knot value count
//            Polynomial-spans relate to knot_indicies as
//               Span_umin = knot[span_index + span_offset]
//            DOF numbers relate to knot_numbers. 
//               Basis function i has domain ui-1 <= u <= ui+k
 
// note: Function argument comments of the form 
//       "sized:[size_argument]" indicate the amount of memory 
//       that is assumed to be pre-allocated to a pointer in 
//       the argument list of a function.  Failing to allocate 
//       this amount of memory to a pointer before a function 
//       call will result in program errors that will not be 
//       detectable by the compiler.

// bmt 18-Dec-02 class DS_bspln, 
//               add bsp_last_eval data member to cache last eval'd span
//

#ifndef DS_BASIS_HXX
#define DS_BASIS_HXX

#include "dsstdef.hxx"        // DS_assert()        // DS_assert()
#include "dcl_ds.h"

// type_id enum, bas value enum, and the max bas_val enum value
enum DS_BAS     { ds_bas, ds_bsp } ;
enum DS_BAS_VAL { bas_u, bas_du, bas_duu, bas_duuu } ;

#define DS_MAX_BASE_VAL 4  // u,du,duu,duuu
        

//==========================================================
// class DS_basis
//==========================================================

class DECL_DM DS_basis : public ACIS_OBJECT {           // identifier = bas
  protected:

   DS_BAS bas_type_id ;    // enum showing specialization type

   int bas_dof_count ;     // number of dofs in basis function
   int bas_span_count ;    // number of span pieces in basis function
   int bas_span_dof_count; // number of dofs per span

  public:

/*
	  void *operator new      // for debugging
    (size_t len)           { return ( malloc(len) ) ; }

	  void operator delete    // for debugging
    (void *p)              { free (p) ; }
*/
   // Type_id, constructors, copy, Make_copy, destructor
   DS_BAS Type_id()        // Type_id access
                           { return bas_type_id ; }

   DS_basis                // default constructor
   (DS_BAS type_id,        // in : derived class identifier
    int dof_count=0,       // in : total dofs for this basis
    int span_count=0,      // in : number of elements in basis
    int span_dof_count=0)  // in : number of dofs per elem      
                           { 
                             bas_type_id        = type_id ;
                             bas_dof_count      = dof_count ;
                             bas_span_count     = span_count ;
                             bas_span_dof_count = span_dof_count ; 
                           }

  virtual                  // bas: DS_basis pure virtual function
   DS_basis *Build_patch   // rtn: a compatable sub-patch for 'this' object
    (double min,           // in : lower domain point for patch
     double max,           // in : upper domain point for patch
     int refinement)=0 ;   // in : parent_knot_spacing/child_knot_spacing

   DS_basis                // copy constructor
    (DS_basis &basis)      // in : basis being copied
                           { 
                             bas_type_id        = basis.bas_type_id ;
                             bas_dof_count      = basis.bas_dof_count ;
                             bas_span_count     = basis.bas_span_count ;
                             bas_span_dof_count = basis.bas_span_dof_count ; 
                           }

   DS_basis &operator=     // assignment operator
    (DS_basis &basis)      // in : right side object of assignment
                           { // no work condition
                             if(&basis != this) {
                               bas_type_id        = basis.bas_type_id ;
                               bas_dof_count      = basis.bas_dof_count ;
                               bas_span_count     = basis.bas_span_count ;
                               bas_span_dof_count = basis.bas_span_dof_count ; 
                             }
                             // all done
                             return *this ;
                           }
   virtual DS_basis*
    Make_copy()=0 ;        // Virtual Make_copy

                           // Virtual Make_copy given new span_count
   virtual DS_basis*       // rtn: new DS_basis
    Make_new_span_count    // eff: new basis from old and span_count
     (int span_count)=0 ;  // in : desired span_count

   virtual
  ~DS_basis(){ }           // Virtual destructor

   // DS_basis simple data access
   int Dof_count     ()    {return bas_dof_count ; }
   int Span_count    ()    {return bas_span_count ; }
   int Span_dof_count()    {return bas_span_dof_count ; }

   // DS_basis vals access convenience functions (non-side-effect)
   // vals = memory block written to by virtual function 
   // Eval_span() 
    
   int Vals_size           // ret: the Eval_span() vals-blk size
    (int eval_count,       // in : number of locs evaluated
     int base_val_flag)    // in : 3=B,Bu,Buu; 4=B,Bu,Buu,Buuu
                           { return(  base_val_flag 
                                    * eval_count 
                                    * bas_span_dof_count) ; 
                           }
   int Vals_blk_size       // ret: Eval_span() B,Bu,& Buu blk sizes
    (int eval_count)       // in : eval_count used to build vals
                           { return(  eval_count
                                    * bas_span_dof_count);
                           }

   double *Vals_blk        // ret: ptr to a u,du, or duu value blk
    (double *vals,         // in : ptr to a Eval_span() vals-blk
     int eval_count,       // in : eval_count used to build vals
     DS_BAS_VAL val_type)  // oneof: bas_u|bas_du|bas_duu|bas_duuu
                           {return
                             (  vals 
                              + (val_type * eval_count)
                              * bas_span_dof_count) ;
                           }
   double *Vals_pt_blk     // ret: ptr to a pt's u,du,duu,duuu value blk
    (double *vals,         // in : ptr to a Eval_span() vals-blk
     int eval_count,       // in : eval_count used to build vals
     int pt_index,         // in : which of the eval points
     DS_BAS_VAL val_type)  // oneof: bas_u|bas_du|bas_duu|bas_duuu
                           {return
                             (  vals 
                              + (val_type * eval_count + pt_index)
                              * bas_span_dof_count) ;
                           }
                           
   // DS_basis virtual side-effect function
   
   virtual void            // bas: class DS_basis virtual function
    Set_span_dof_map       // eff: set dof_map for indicated span
     (int span_index,      // in : identifies B-spline span
      int *dof_map)=0 ;    // out: sequence of dofs in span
                           //      sized:[bas_span_dof_count]

  virtual                  // bas: class DS_basis virtual function
   int                     // rtn: 0=success,-1=bad_dpts
    Eval_span              // calc: basis val,1st,2nd,3rd Derivs
     (int span_index,      // in : identifies spline span
      int eval_count,      // in : number of locations to evaluate
      int base_val_flag,   // in : specify what evaluations to make
                           //      1=B 2=B,Bu 3=B,Bu,Buu 4=B,Bu,Buu,Buuu
      double *uu,          // in : uu locations to make evaluation
      int dof_map_size,    // in : amount of alloc memory in dof_map
      int *dof_map,        // out: the index and order of each
                           //      evaluated basis function
                           //      sized:[bas_span_dof_count]
      int vals_size,       // in : amount of alloc memory in vals
      double *vals)=0 ;    // out: fills a given array with
                           //      [Bi(uu), B'i(uu), B"i(uu)] for 
                           // each basis function and uu location
                           // ass: vals memory is pre-allocated 
                           //      sized:[3 * eval_count
                           //               * bas_span_dof_count]
                           // vals = [B_blk, B'_blk, B"_blk]
                           //  B_blk  = [u0_blk,  u1_blk,  ...]
                           //  B'_blk = [u'0_blk, u'1_blk, ...]
                           //  B"_blk = [u"0_blk, u"1_blk, ...]
                           //  u_blk  = [B0(u),  B1(u), ...]
                           //  u'_blk = [B'0(u), B'1(u),...]
                           //  u"_blk = [B"0(u), B"1(u),...]

   virtual                 // bas: class DS_basis virtual function
   int                     // rtn: 0 when span is split else rtn -1
    Split_span             // eff: divides a span into two parts
      (int span_index,     // in : the tgt span to be split.
       double *uu=NULL)=0; // in : optional derived class loc control
                           //      after this call the span count
                           //      is incremented by one and
                           //      all data in derived classes may
                           //      be altered.

  virtual void             // bas: class DS_basis virtual function
   Elevate_degree          // eff: increases basis function degree by one.
    (int cont_flag=0)=0 ;  // in : 0 = preserve current elem continuity
                           //      1 = increment elem continuity
                           // note: preserving continuity allows the
                           //       new shape to exactly form the old

  virtual void             // bas: class DS_basis virtual function
  Set_degree     // eff: Change basis to new degree
    (int new_deg,          // in : new degree
     int& rtn_flag )=0;    // out: 0=success, -1=new degree<3

  virtual void             // eff: increase domain by 5% by adjusting
   Extrapolate_domain      //      End knot values.
    (int ext_flag)=0 ;     // in : specify which ends to extend
                           //      0=none,1=low,2=high,3=both

  virtual void             // bas: DS_basis pure virtual function
   Shift_domain            // eff: shift the basis domain values
    (double du)=0 ;        // in : domain shift amount

  virtual                  // bas: DS_basis pure virtual function
   int                     // rtn: 0=done,-1=scale too small
   Scale_domain            // eff: scale the basis domain values
    (double scale)=0 ;     // in : domain scale amount

   // DS_basis virtual non-side-effect functions

   virtual double          // bas: class DS_basis virtual function
    Span_umin              // ret: span bot domain bound value
    (int span_index)=0 ;   // in : specifies which span

   virtual double          // bas: class DS_basis virtual function
    Span_umax              // ret: span top domain bound value
     (int span_index)=0 ;  // in : specifies which span

   virtual double          // bas: class DS_basis virtual function
    Span_length            // ret: length = top-bot domain bound vals
     (int span_index)=0 ;  // in : specifies which span

   double                  // ret: min span length in basis
    Min_span_length        // eff: search all spans for minimum
      ()                   { int ii ;
                             double min = Span_length(0) ;
                             for(ii=1;ii<bas_span_count;ii++)
                               {
                                 if(min > Span_length(ii))
                                   min = Span_length(ii) ;
                               }
                             return min ;
                           }

   virtual int             // bas: class DS_basis virtual function
    Span_upper_continuity  // ret: -1=curve-bound,0=C0,1=C1,...
     (int span_index)=0 ;  // in : specifies which span

   virtual int             // bas: class DS_basis virtual function
    Span_lower_continuity  // ret: -1=curve-bound,0=C0,1=C1,...
     (int span_index)=0 ;  // in : specifies which span

   virtual int             // bas: class DS_basis virtual function
    U_2span_index          // ret: Span_index containing u or -1
     (double u,            // in : the span location
      int knot_flag=-1)=0; // in : Specify rtn span when u==knot_val
                           //      0 = low span
                           //      1 = high span
                           //     -1 = don't use - for compatability      

} ; // end class DS_basis

//==========================================================
// class DS_bspln : public DS_basis 
//==========================================================

class DECL_DM DS_bspln : public DS_basis {  // identifier = bsp
  protected:
   // Inherit bas_dof_count, bas_span_count, bas_span_dof_count
   // constraint: total_knot_count = bas_dof_count + bsp_degree - 1

   // B-spline basis function description
   int bsp_degree;         // [size-arg] polynomial degree = k, 
                           //                  order=n, n = k + 1
   int bsp_span_offset ;   // cached by Derive_spans: 
                           // 1st span start in knot SPAvector 
                           //    0 <= span_offset < degree
                           //    depending on end knot multiplicities
   int bsp_knot_count ;    // [size-arg] distinct knot value count 
                           //   (not the total knot count)

   double *bsp_knot ;      // knot value SPAvector    [bsp_knot_count]
   int    *bsp_index ;     // knot end index SPAvector[bsp_knot_count]
                           // rather than storing the number of
                           // knots for each knot value, store the
                           // highest knot index that shares that
                           // knot value.  Examples:
                           // no mults   bsp_index = [0,1,2,3,4,5,6]
                           // some mults bsp_index = [2,3,5,6]
                           // Ex: mult end knots to interp ends
                           // let d=degree and s = segments then
                           // index = [d-1,d,d+1,..d+s-2,d+s-2+d] 

   double *bsp_ui ;        // scratch, ui values    :[2*k]
   double *bsp_du ;        // scratch, u-ui values  :[2*k]
   double *bsp_dui ;       // scratch, uj-ui values :[k*(k+1)/2]
   double *bsp_bi ;        // scratch, Bi,k values  :[n*(n+1)/2]

   int bsp_last_eval;          // optimization, last evaluation was on span [last_eval,last_eval+1)

  public :
/*
	  void *operator new      // for debugging
    (size_t len)           { return ( malloc(len) ) ; }

	  void operator delete    // for debugging
    (void *p)              { free (p) ; }
*/
   // DS_bspln constructor, copy, Make_copy, destructor, Size_arrays
   DS_bspln                // eff: default constructor
    (int degree=0,         // in : polynomial degree
     int dof_count=0,      // in : control point count
     int knot_count=0,     // in : number of distinct knot values
     int *count=NULL,      // in : Specify the multiple knots by
                           //      setting count[i] = maximum knot
                           //      index value for location knot[i]
                           //      size:[knot_count]  Examples:
                           //      no multiples count = [0,1,2,3,4]
                           //      some multiples     = [1,2,4]
                           // Ex: mult end knots to interp ends
                           // let d=degree and s = segments then
                           // index = [d-1,d,d+1,..d+s-2,d+s-2+d] 
     double *knot=NULL) ;  // in : knot value array size:[knot_count]
                           //      ordered [u0 < u1 < u2 ... ]
                           // note: count and knot values are copied.
                           //       The calling program continues
                           //       to manage this memory.

  virtual                  // bas: DS_basis pure virtual function
   DS_basis *Build_patch   // rtn: a compatable sub-patch for 'this' object
    (double min,           // in : lower domain point for patch
     double max,           // in : upper domain point for patch
     int refinement) ;     // in : parent_knot_spacing/child_knot_spacing

   DS_bspln                // copy constructor
    (DS_bspln &bspln) ;

   DS_bspln &operator=     // assignment operator
    (DS_bspln &bspln) ;    // in : right side object of assignment
               

   DS_basis*
    Make_copy();            // Make_copy, Virtual DS_basis function

                           // Virtual Make_copy given new span_count
   virtual DS_basis*       // rtn: new DS_basis
    Make_new_span_count    // eff: new basis from old and span_count
     (int span_count) ;    // in : desired span_count

   virtual
  ~DS_bspln()              // destructor
                           { Size_arrays() ; }

   void
    Size_arrays            // Size_arrays
    (int knot_count=0,     // number of distinct knot values
     int degree    =0) ;   // B-spline polynomial degreesize_arrays

   // DS_bspln simple data access
   int Degree()            { return bsp_degree ; }
   int Unique_knot_count() { return bsp_knot_count ; }
   double *Knots()         { return bsp_knot ; }
   double  Knots(int i)    { DS_assert(i >= 0 && i < bsp_knot_count) ;
                             return bsp_knot[i] ;
                           }
   int *Knot_index()       { return bsp_index ; }

   int Knot_number_2index  // ret : knot array index for knot_index
    (int knot_number) ;    // in  : index of knot in question
                           // note: the knot array is not organized
                           //       one for one with knot numbering

   double Knot_val         // ret: knot uu value for knot_number
    (int knot_number)      // in : knot number in question
                           { int index = Knot_number_2index(knot_number) ;
                             return(bsp_knot[index]) ; 
                           }

   int Knot_index_size     // ret: array-size
    (int knot_count)       // in : number of unique knot locations
                           { return knot_count ; }

   int Scratch_size        // ret: array-size
    (int degree)           { return (degree==0 ? 0 : 
                             (  2 * degree                // bsp_ui
                              + 2 * degree                // bsp_du
                              + (degree  )*(degree+1)/2   // bsp_dui
                              + (degree+1)*(degree+2)/2));// bsp_bi
                           }

   int Span_umin_number    // span bot domain bound knot number 
    (int span_index)       // in : specifies which span
                           { DS_assert(   span_index >= 0
                                       && span_index < bas_span_count);
                             return bsp_index[ span_index 
                                             + bsp_span_offset] ; 
                           }
  
   int Span_umax_number    // span top domain bound knot number
    (int span_index)       // in : specifies which span
                           { DS_assert(   span_index >= 0
                                       && span_index < bas_span_count);
                             return bsp_index[ span_index 
                                              + bsp_span_offset]+1 ;
                           }

   int Span_umin_index     // span bot domain bound knot index
    (int span_index)       // in : specifies which span
                           { DS_assert(   span_index >= 0
                                       && span_index < bas_span_count);
                             return(span_index+bsp_span_offset) ; 
                           }

   int Span_umax_index     // span top domain bound knot index
    (int span_index)       // in : specifies which span
                           { DS_assert(   span_index >= 0
                                       && span_index < bas_span_count);
                             return(span_index+bsp_span_offset+1) ; 
                           }
   void Eval_greville_pts  // eff: load array with greville vals
    (double *gre_pts)      // out: greville abscissae sequence
                           //      sized:[bspln->Dof_count()]
                           { DS_assert(gre_pts) ;
                             int ii, jj ;
                             for(ii=0;ii<bas_dof_count;ii++)
                                { gre_pts[ii] = 0.0 ;
                                  for(jj=0;jj<bsp_degree;jj++)
                                    gre_pts[ii] += Knot_val(ii+jj) ;
                                  gre_pts[ii] /= bsp_degree ;
                                }
                           } // end Eval_Greville_pts

  // DS_bspln instances of DS_basis virtual side-effect functions 
  
   virtual void            // bas: class DS_basis virtual function
    Set_span_dof_map       // eff: set dof_map for indicated span
     (int span_index,      // in : identifies B-spline span
      int *dof_map) ;      // out: sequence of dofs in span
                           //      sized:[bas_span_dof_count]

  virtual                  // bas: class DS_basis virtual function
   int                     // rtn: 0=success,-1=bad_dpts
    Eval_span              // calc:basis val,1st,2nd,3rd derivs 
     (int span_index,      // in : identifies spline span
      int eval_count,      // in : number of locations to evaluate
      int base_val_flag,   // in : specify what evaluations to make
                           //      1=B 2=B,Bu 3=B,Bu,Buu 4=B,Bu,Buu,Buuu
      double *uu,          // in : uu locations to make evaluation
      int dof_map_size,    // in : amount of alloc memory in dof_map
      int *dof_map,        // out: the index and order of each
                           //      evaluated basis function
                           //      sized:[bas_span_dof_count]
      int vals_size,       // in : amount of alloc memory in vals
      double *vals) ;      // out: fills a given array with
                           //      [Bi(uu), B'i(uu), B"i(uu)] for 
                           // each basis function and uu location
                           // ass: vals memory is pre-allocated 
                           //      sized:[3 * eval_count
                           //               * bas_span_dof_count]
                           // vals = [B_blk, B'_blk, B"_blk]
                           //  B_blk  = [u0_blk,  u1_blk,  ...]
                           //  B'_blk = [u'0_blk, u'1_blk, ...]
                           //  B"_blk = [u"0_blk, u"1_blk, ...]
                           //  u_blk  = [B0(u),  B1(u), ...]
                           //  u'_blk = [B'0(u), B'1(u),...]
                           //  u"_blk = [B"0(u), B"1(u),...]

   virtual                 // bas: class DS_basis virtual function
   int                     // rtn: 0 when span is split else rtn -1
    Split_span             // eff: divides a span into two parts
      (int span_index,     // in : the tgt span to be split.
       double *uu=NULL) ;  // in : optional derived class loc control
                           //      after this call the span count
                           //      is incremented by one and
                           //      all data in derived classes may
                           //      be altered.

  virtual void             // bas: class DS_basis virtual function
   Elevate_degree          // eff: increases basis function degree by one.
    (int cont_flag=0) ;    // in : 0 = preserve current elem continuity
                           //      1 = increment elem continuity
                           // note: preserving continuity allows the
                           //       new shape to exactly form the old

  virtual void             // bas: class DS_basis virtual function
   Set_degree              // eff: Change b-spline to new degree
    (int new_deg,          // in : new degree
     int& rtn_flag ) ;     // out: 0=success, -1=new degree<3


  virtual void             // eff: increase domain by 5% by adjusting
   Extrapolate_domain      //      End knot values.
     (int ext_flag) ;      // in : specify which ends to extend
                           //      0=none,1=low,2=high,3=both

  virtual void             // bas: DS_basis pure virtual function
   Shift_domain            // eff: shift the basis domain values
    (double du) ;          // in : domain shift amount

  virtual                  // bas: DS_basis pure virtual function
   int                     // rtn: 0=done,-1=scale too small
   Scale_domain            // eff: scale the basis domain values
    (double scale) ;       // in : domain scale amount


  // DS_bspln instances of DS_basis non-side-effect virtual functions

   virtual double          // bas: class DS_basis virtual function
    Span_umin              // ret: span bot domain bound value
    (int span_index)       // in : specifies which span
     { DS_assert(   span_index + bsp_span_offset >= 0
                 && span_index + bsp_span_offset < bsp_knot_count) ;
       return bsp_knot[span_index + bsp_span_offset] ; }

   virtual double          // bas: class DS_basis virtual function
    Span_umax              // ret: span top domain bound value
     (int span_index)      // in : specifies which span
     { DS_assert(   span_index + bsp_span_offset + 1 >= 0
                 && span_index + bsp_span_offset + 1 < bsp_knot_count) ;
       return bsp_knot[span_index + bsp_span_offset + 1] ; }
  

   virtual double          // bas: class DS_basis virtual function
    Span_length            // ret: length = top-bot domain bound vals
     (int span_index)      // in : specifies which span
     { DS_assert(   span_index + bsp_span_offset >= 0
                 && span_index + bsp_span_offset + 1 < bsp_knot_count) ;
       return (  bsp_knot[span_index + bsp_span_offset + 1]
               - bsp_knot[span_index + bsp_span_offset]) ; }

   virtual int             // bas: class DS_basis virtual function
    Span_upper_continuity  // ret: -1=curve-bound,0=C0,1=C1,...
     (int span_index)      // in : specifies which span
                           { DS_assert(   span_index >= 0 
                                       && span_index < bas_span_count);
                             return(  span_index == bas_span_count-1
                                    ? -1
                                    : (  bsp_degree
                                       - Knot_mult(  bsp_span_offset 
                                                   + span_index
                                                   + 1))) ;
                           }

   virtual int             // bas: class DS_basis virtual function
    Span_lower_continuity  // ret: -1=curve-bound,0=C0,1=C1,...
     (int span_index)      // in : specifies which span
                           { DS_assert(   span_index >= 0 
                                       && span_index < bas_span_count);
                             return(  span_index == 0
                                    ? -1
                                    : (  bsp_degree
                                       - Knot_mult(  bsp_span_offset
                                                   + span_index))) ;
                           }


   virtual int             // bas: class DS_basis virtual function
    U_2span_index          // ret: Span_index containing u or -1
     (double u,            // in : the span location
      int knot_flag=1) ;   // in : Specify rtn span when u==knot_val
                           //      0 = low span
                           //      1 = high span
                           //     -1 = don't use - for compatability      
              
   void Derive_spans() ;   // set bas_span_count and bsp_span_offset

   int Knot_mult           // rtn: knot_count of knot[index]
    (int knot_index)       // in : specifies which unique knot
                           { DS_assert(   knot_index >= 0
                                       && knot_index < bsp_knot_count);
                             return (  knot_index > 0
                                     ? (  bsp_index[knot_index]
                                        - bsp_index[knot_index-1])
                                     : bsp_index[knot_index]) ; 
                           }
   int Knot_continuity     // rtn: continuity across knot boundary
     (int knot_index)      // in : specifies which unique knot
                           { // inside: cont = degree - multiplicity
                             // ends  : cont = degree - mult - 1
                             int cont =   bsp_degree 
                                        - Knot_mult(knot_index) ;
                             if(   knot_index == 0
                                || knot_index == bsp_knot_count-1)
                               cont -= 1 ;
                             return(cont) ;
                           }
   int *                   // rtn: array of continuity value for every knot
     Make_continuity_array // eff: build and return array of cont vals
                           // mallocs: calling function must free return array
      ();
 
   void Set_span_sizes     // set uj-ui and ui span values
    (int span_index,       // in : identifies B-spline span
     double *dui,          // out: signed ui values
                           //      assumed sized:[2*k] (k=degree)
     double *du) ;         // out: uj-ui values
                           //      assumed sized:[k*(k+1)/2]

} ; // end class DS_bspln

/*
//==========================================================
// class DS_hermite : public DS_basis 
//==========================================================

class DS_hermite 
{                          // identifier = hrm
  protected :

  public :

} ; // end class DS_hermite
*/

#endif // DS_BASIS_HXX
