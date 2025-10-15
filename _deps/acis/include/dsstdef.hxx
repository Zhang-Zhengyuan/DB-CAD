// $Id: dsstdef.hxx,v 1.47 2001/10/17 22:06:20 btomas Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dsstdef.hxx
// created  : Feb 15, 1995
// author   : George Celniker
// synopsis : standard variables and constants that have to be 
//            available to compile

/* summary
define NULL
DS_TRUE
DS_FALSE
DS_DEFAULT_NTGRL_DEGREE
DS_PI
DS_is_zero(val)
DS_not_zero(val)
DS_not_zero_element_count  // eff: count non-zero elements in a SPAvector
DS_assert(exp)
DS_dbif(val)               // conditionally compiled if statement
DS_dbendif                 // conditionally compiled end if 
DS_sgn(b)                  // ret: +1 if b>0.0, else -1
DS_qsort_double_compare    // eff: qsort subr to order a double array
DS_qsort_int_compare       // eff: qsort subr to order an int array
DS_sym_index_2offset(i,j,n)// ret: array[NxN] offset for index[i,j]
DS_invert_index_map()        // eff: build the inverse of an index mapping
DS_pt_pt_dist2             // ret: pt to pt dist**2 for any dimension
DS_pt_line_dist2           // ret: min(dist**2) from pt to line
DS_pt_polyline_dist2       // ret: min(dist**2) from pt to polyline
DS_line_line_dist2         // ret: min(dist**2) from line to line
DS_line_polyline_dist2     // ret: min(dist**2) from line to polyline
DS_line_sphere_min_loc     // eff: get line pt closest to sphere
DS_line_plane_xsect        // rtn: 0=success,-1=no intersection
DS_line_round_2d_xsect     // rtn: number of xsect found (0,1,or 2)
DS_line_sphere_xsect       // rtn: number of xsects found (0,1 or 2)
DS_span_span_xsect         // eff: xsect two any dimensional spans
DS_span_span_xsectP        // rtn: 1=spans xsect,0=spans don't
DS_quadratic_real_roots    // rtn: real_root_count in a*u**2+b*u+c=0
DS_quadratic_roots         // eff: calc complex roots for quadratic
DS_is_parallel(v,w)        // rtn: 1 if SPAvector a is parallel to b
DS_is_parallel_2vec(v,w)   // rtn: 1 if SPAvector a is parallel to b
DS_angle_3vec              // eff: measure angle from vec1 to vec2 in rads
DS_copy_3vec(v,r)          // eff: copy src into tgt
s = DS_normalize_vec       // eff: normalizes the input vec, rtn size
s = DS_normalize_3vec(v)   // eff: normalizes the input vec, rtn size
s = DS_normalize_2vec(v)   // eff: normalizes the input vec, rtn size
DS_determinate_3mat        // rtn: determinate[a,b,c]
DS_triple_product(a,b,c)   // rtn: (axb).c (same as determinate)
DS_cross_3vec(a,b,r)       // eff: store r = (a X b)
DS_cross_2vec(a,b)         // rtn: a[0]*b[1]-a[1]*b[0]
s = DS_dot_3vec(a,b)       // rtn: dot_product(a,b)
s = DS_dot_2vec(a,b)       // rtn: dot_product(a,b)
s = DS_dot_vec(dim,a,b)    // rtn: dot_product(a,b)
s = DS_size_2vec(a)        // rtn: size of a = sqrt(sum_i(ai*ai))
s = DS_size_3vec(a)        // rtn: size of a = sqrt(a0**2+a1**2+a2**2)
s = DS_size_vec(dim,a)     // rtn: size of a = sqrt(sum_i(ai*ai))
s = DS_size2_vec(dim,a)    // rtn: size**2 of a = sum_i(ai*ai)
s = DS_size2_3vec(a)       // rtn: dot_product(a,a)
s = DS_size2_2vec(a)       // rtn: dot_product(a,a)
DS_add_3vec(a,b,r)         // eff: store r = a + b
DS_add_vec(d,a,b,r)        // eff: store r = a + b
DS_sub_2vec(a,b,r)         // eff: store r = a - b
DS_sub_3vec(a,b,r)         // eff: store r = a - b
DS_sub_vec(d,a,b,r)        // eff: store r = a - b
DS_add_scaled_3vec         // eff: store r = a*v0 + b*v1
      (a,v0,b,v1,r)        
DS_add_scaled_vec          // eff: store r = a*v0 + b*v1
      (d,a,v0,b,v1,r)      
DS_add_3scaled_3vec        // eff: store r = a*v0 + b*v1 + c*v2
      (a,v0,b,v1,c,v2,r)
DS_Wuv_2Wt(Cu,Cv,Wu,Wv,Wt) // err: store r =  Cu*Wu+Cv*Wv (direction direvative)
DS_Wuv_2Wn(Cu,Cv,Wu,Wv,Wn) // eff: store r = -Cv*Wu+Cu*Wv (normal to dir direvative)
DS_Wuv_2Wtt                // eff: Wnn = Cu*Cu*Wuu+2*Cu*Cv*Wuv+Cv*Cv*Wvv
DS_Wuv_2Wnn                // eff: Wnn = Cv*Cv*Wuu-2*Cu*Cv*Wuv+Cu*Cu*Wvv
DS_scale_3vec(a,s,r)       // eff: store r = a * s
DS_scale_vec(d,a,s,r)      // eff: srore r = a * s for d dimensions
DS_tangent_2basis(t,n,b)   // rtn: basis vecs aligned with tangent
DS_twovec_2basis(u,v,t,n,b)// rtn: basis vecs oriented by 2 input vecs
DS_calc_perp_in_plane()    // eff: find orthoganal direction given 2 input vecs
DS_calc_km                 // eff: calc curvature vec k*m for Wu and Wuu
DS_calc_frenet_frame       // eff: get t,b,m from Wu, Wuu
DS_calc_curvature          // rtn: k = size(cross(Wu,Wuu))/size(Wu)**3
DS_calc_torsion            // eff: T = det[Wu,Wuu,Wuuu]/size(cross(Wu,Wuu))**2
DS_calc_normal_vec()       // eff: calc normal vec given derivative values
DS_xsect_spans             // rtn: largest span in both input spans
DS_span_ratio              // eff: compare the area of 2 spans
DS_rot_vec_about_axis      // eff: rotate a vec about an axis
DS_sym_mat_to_LDLt_with_diag_pivots    // eff: convert A to LDLt in Ay = b
DS_solve_LDLt_equal_b_with_diag_pivots // eff: solve for y in LDLt y = b
DS_calc_offset             // rtn: compute offset to map center
                           //      of one range to another.
DS_PYTHAG                  // rtn: sqrt(a*a+b*b) as a*sqrt(1+(b/a)**2)
DS_list_compliment         // rtn: malloc list compliment array

*/

#ifndef DS_STDEF_H
#define DS_STDEF_H

#include "dcl_ds.h" // define DECL_DM // define DECL_DM
#include "mmgr.hxx" // class ACIS_OBJECT
#include <math.h>          // fabs(), sqrt()

#define DS_CONTINUOUS 10 // magic number used to label 
                         // seg_bnds as continuous
// default values to ensure good numerical integration
#define DS_DEFAULT_NTGRL_DEGREE 10
#define DS_DEFAULT_ELEM_COUNT   8

// maximum solve tries while waiting for convergence
#define DS_SOLVE_MAX_ITER_COUNT 20 

#define DS_PI 3.1415926535898 // same number used in ACIS.hxx
// (double)  (3.141592653589793238462643)

// eff: return max or min of two numbers
inline double DS_MAX(double a,double b) {return(a>b ? a:b) ; }
inline int    DS_MAX(int a,   int b)    {return(a>b ? a:b) ; }
inline double DS_MIN(double a,double b) {return(a<b ? a:b) ; }
inline int    DS_MIN(int a,   int b)    {return(a<b ? a:b) ; }
inline double DS_3MAX
  (double a, double b, double c)        { return( (a>b) ? ((a>c) ? a:c)
                                                        : ((b>c) ? b:c)); 
                                        }
inline int    DS_3MAX
  (int a, int b, int c)                 { return( (a>b) ? ((a>c) ? a:c)
                                                        : ((b>c) ? b:c));
                                        }
inline double DS_3MIN
  (double a, double b, double c)        { return( (a<b) ? ((a<c) ? a:c)
                                                        : ((b<c) ? b:c));
                                        }
inline int    DS_3MIN
  (int a, int b, int c)                 { return( (a<b) ? ((a<c) ? a:c)
                                                        : ((b<c) ? b:c));
                                        }
#define DS_SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))
#define DS_XOR(a,b) ( ((a) && !(b)) || (!(a) && (b)))

#ifndef mac
//# include <iostream.h>      // for cout, cerr, '<<', 
#endif
#include <assert.h>        // for assert()

#ifndef NULL               // NULL is defined in several of the
#ifdef __cplusplus         // C and C++ include files.  However
#define NULL 0             // it is not part of the C or C++
#else                      // languages.  Some of the simpler
#define NULL ((void *)0)   // DS files include no header functions
#endif                     // and need a NULL definition
#endif

// define boolean values
const int DS_TRUE=1;
const int DS_FALSE=0;

//============================================================
// class DS_qsort_data   -  used for qsort
//============================================================
class DS_qsort_data ;
class DS_dm2qsort_data ;

class DS_qsort_data : public ACIS_OBJECT       // identifier = qsd
  {
   protected:
     double qsd_dval ;     // the double value
     int    qsd_ival ;     // the data value

   public:
   
   // Constructor, Copy, assignment, destructor
   DS_qsort_data           // default constructor
     (double dval=0,       // in : the value to store
      int    ival=0)       // in : the data value being stored
                           { qsd_dval = dval ;
                             qsd_ival = ival ;
                           }

   DS_qsort_data           // copy constructor
    (DS_qsort_data &data)  { qsd_dval = data.qsd_dval ;
                             qsd_ival = data.qsd_ival ;
                           }

   DS_qsort_data &operator=// assignment operator
    (DS_qsort_data &data)  {  // no work condition
                              if(&data != this) {
                             qsd_dval = data.qsd_dval ;
                             qsd_ival = data.qsd_ival ;
                             }
                             return *this ;
                           }

   virtual                 // destructor
   ~DS_qsort_data()        { qsd_dval = 0 ;
                             qsd_ival = 0 ;
                           }
                                   
   // data access
   double &Dval()          { return qsd_dval ; }
   void Set_dval(double d) { qsd_dval = d ; } 

   int &Ival()             { return qsd_ival ; }
   void Set_ival(int i)    { qsd_ival = i ; }

} ; // end class DS_qsort_data

//============================================================
// Portable Exception Handling Macros
//============================================================

#include "dmexcept.hxx"  // DM_EXCEPTION_BEGIN  // DM_EXCEPTION_BEGIN
                                         // DM_EXCEPTION_TRY
                                         // DM_EXCEPTION_CATCH_TRUE
                                         // DM_EXCEPTION_CATCH_FALSE
                                         // DM_EXCEPTION_END
    
//============================================================
// DS_assert(exp) with breakpoint capabilities 
// turned on by DSdebug
//============================================================

// assert and error macros which do not debend on 
// defines DSDEBUG1 and DSDEBUG

#define DS_assert_dmerr(exp, val) if(!(exp)){DM_sys_error(val) ; }
#define DS_assert_rtn(name,exp,val) \
        if(!(exp)){ rtn_err = val ; \
                    name; }   
#define DS_assert_rtn_val(name,exp,err,rtn) \
        if(!(exp)){ rtn_err = err ;         \
                    name(rtn); }   
#define DS_assert_err(exp,err) if(!(exp)){rtn_err=err;}   

// macros which debend on the compile time defines
// DSDEBUG1 and DSDEBUG

#ifdef DSDEBUG1

// extern int DS_dbstate ;
DECL_DM void DS_assert_break    // eff: a debugging breakpoint place
 (const char* file,     // in : file name for break call
	 int line) ;           // in : line number for break call
#define DS_assert1(exp)    if(!(exp)){DS_assert_break(__FILE__, __LINE__);}
#define DS_assert(exp)    if(!(exp)){DS_assert_break(__FILE__, __LINE__);}
//#define DS_assert1(exp)   if(!(exp)){DS_assert_break();}
//#define DS_assert(exp)    if(!(exp)){DS_assert_break();}
#define DS_break          DS_assert_break(__FILE__, __LINE__)
#define DS_name(exp)      exp
#define DS_CRVDEBUG  
#define DSDEBUG

#elif defined DSDEBUG

// extern int DS_dbstate ;
DECL_DM void DS_assert_break    // eff: a debugging breakpoint place
 (const char* file,     // in : file name for break call
	 int line) ;           // in : line number for break call

#define DS_assert1(exp)
//#define DS_assert(exp)    if(!(exp)){DS_assert_break();}
#define DS_assert(exp)    if(!(exp)){DS_assert_break(__FILE__, __LINE__);}
#define DS_break          DS_assert_break(__FILE__, __LINE__)
#define DS_name(exp)      exp

#else
 
#define DS_assert(exp) 
#define DS_assert1(exp)  
#define DS_break 
#define DS_name(exp)

#endif

//============================================================
// DS_DOUBLE_SMALL, DS_is_zero(), DS_not_zero()
//============================================================

// kev. TSA. May 2008.
#ifdef THREAD_SAFE_ACIS
extern DECL_DM safe_floating_type<double> DS_tolerance ;              // follows (SPAresabs)
extern DECL_DM safe_floating_type<double> DS_angle_tol ;
#else
extern DECL_DM double DS_tolerance ;              // follows (SPAresabs)
extern DECL_DM double DS_angle_tol ;
#endif

#define DS_DOUBLE_CLOSE    DS_tolerance           // 1.0e-06
#define DS_DOUBLE_FIT     (DS_DOUBLE_CLOSE*1.0e3) // 1.0e-03 (should be SPAresfit)
//#define DS_DOUBLE_FIT     (DS_DOUBLE_CLOSE*6.0e4) // 6.0e-02 fixes missed intersection in slab-bugtest
#define DS_DOUBLE_SMALL   (DS_DOUBLE_CLOSE/1.0e3) // 1.0e-09
#define DS_DOUBLE_CSMALL  (DS_DOUBLE_CLOSE/1.0e6) // 1.0e-12
#define DS_DOUBLE_TINY    (DS_DOUBLE_CLOSE/1.0e8) // 1.0e-14
#define DS_DOUBLE_MACHINE (DS_DOUBLE_CLOSE/1.0e9) // 1.0e-15

#define DS_ANGLE_CLOSE    DS_angle_tol            // 1.0e-10

inline int DS_is_zero      // ret: 1=close to zero, 0=not close
     (double val,          // in : value to be checked
      double tol)          // in : tolerance size used for zero
                           {return(fabs(val)<tol ?1:0);
                           }
inline int DS_not_zero     // ret: 1=not real close to zero, 0=is close
     (double val,          // in : value to be checked
      double tol)          // in : tolerance size used for zero
                           {return(fabs(val)<tol ?0:1);
                           }
inline int DS_neg_not_zero// ret: 1=negative and not close to zero
  (double val,            // in : value to be checked
   double tol)            // in : tolerance size used for zero
                          {return(val < -tol?1:0) ; }
inline int DS_pos_not_zero// ret: 1=positive and not close to zero
  (double val,            // in : value to be checked
   double tol)            // in : tolerance size used for zero
                          {return(val > tol?1:0) ; }

inline int DS_neg_or_zero // ret: 1=negative or close to zero
  (double val,            // in : value to be checked
   double tol)            // in : tolerance size used for zero
                          {return(val <= tol?1:0) ; }
inline int DS_pos_or_zero // ret: 1=positive or close to zero
  (double val,            // in : value to be checked
   double tol)            // in : tolerance size used for zero
                          {return(val >= -tol?1:0) ; }

inline int DS_in_span     // ret: 1=val in span with no tolerance
  (double min,            // in : bottom of span
   double max,            // in : top of span
   double val)            // in : value to check
                          {return(val >= min && val <= max?1:0) ; }

inline int DS_near_span   // ret: 1=val in span with tolerance                                                       
  (double min,            // in : bottom of span
   double max,            // in : top of span
   double val,            // in : value to check
   double tol)            // in : tolerance size used for bounds
                          {return(   val >= min - tol
                                  && val <= max + tol)?1:0 ;
                          }
void
DS_not_zero_element_count  // eff: count non-zero array elems
  (double *array,          // in : ptr to array of values to check
   int array_count,        // in : size of array
   double tol,             // in : tolerance size used for zero
   int &count,             // out: count of non-zero elements
   double &max_value) ;    // out: max value encountered

inline double 
DS_sgn                     // ret: +1 if b>0.0, else -1
   (double b)              {return ( b > 0.0 ? 1.0 : -1.0 ) ;
                           }

inline int                 // eff: subr called by qsort to order
DS_qsort_double_compare    //      an array of doubles
   (const void *elem1, 
    const void *elem2)     
    { return( *(double *)elem1  < *(double *)elem2 ? -1 :
              *(double *)elem1 == *(double *)elem2 ?  0 : 1 ) ; 
    }

inline int                   // eff: subr called by qsort to order
DS_qsort_data_double_compare //      an array of doubles with data
   (const void *elem1,
    const void *elem2)
    { return(   ((DS_qsort_data *)elem1)->Dval() 
             <  ((DS_qsort_data *)elem2)->Dval() ? -1
             :  ((DS_qsort_data *)elem1)->Dval()
             == ((DS_qsort_data *)elem2)->Dval() ? 0 : 1 ) ;
    }

inline int                   // eff: subr called by qsort to order
DS_qsort_data_int_compare    //      an array of doubles with data
   (const void *elem1,
    const void *elem2)
    { return(   ((DS_qsort_data *)elem1)->Ival() 
             <  ((DS_qsort_data *)elem2)->Ival() ? -1
             :  ((DS_qsort_data *)elem1)->Ival()
             == ((DS_qsort_data *)elem2)->Ival() ? 0 : 1 ) ;
    }

inline int                 // eff: subr called by qsort to order
DS_qsort_int_compare       //      an array of ints
   (const void *elem1, 
    const void *elem2)     
    { return( *(int *)elem1  < *(int *)elem2 ? -1 :
              *(int *)elem1 == *(int *)elem2 ?  0 : 1 ) ; 
    }

int
DS_sym_index_2offset       // ret: ret: array[NxN] offset for index[i,j]
  (int ii,                 // in : index i (required to be < n)
   int jj,                 // in : index j (required to be < n)
   int nn) ;               // in : sym-SPAmatrix size paramater

int                        // rtn: 0=success,-1=error
DS_invert_index_map        // eff: build the inverse of an index mapping
  (int *in_map,            // in : map to invert, size:[in_count]
   int  in_count,          // in : size of in_map
   int *out_map,           // out: inverse of in_map, size:[out_count]
   int  out_count) ;       // in : size of out_map
                           // note: out_map is preallocated
double
DS_pt_pt_dist2             // ret: pt to pt dist**2 for any dimension
  (const double *p0,             // in : the point in question
   const double *p1,             // in : the other point in question
   int     dim) ;          // in : length of each pt

double
DS_pt_line_dist2           // ret: min(dist**2) between pt and line
  (const double *p0,             // in : the point in question
   const double *c0,             // in : parametric representation of line
   const double *c1,             // in :  given as line = c0 + u (c1- c0)
   int     dim,            // in : length of each pt
   double &uu) ;           // out: uu_loc on line closest to point

double
DS_pt_polyline_dist2       // ret: min(dist**2) from pt to polyline
  (const double *pt,             // in : pt_loc [xx,yy,zz] to be classified
   const double *polyline,       // in : ordered [xyz0, xyz1.., xyzn]
   int     pt_count,       // in : number of points in polyline
   int     dim,            // in : length of each pt
   int    &seg_index,      // out: segment index containing the min
   double &uu) ;           // out: dist along seg to min 0 < uu <= 1

double
DS_line_line_dist2         // ret: min(dist**2) between line and line
  (const double *p0,             // in : p0 of line1 = p0 + uu*(p1-p0)
   const double *p1,             // in : p1 of line1 = p0 + uu*(p1-p0)
   const double *c0,             // in : c0 of line2 = c0 + uu*(c1-c0)
   const double *c1,             // in : c1 of line2 = c0 + uu*(c1-c0)
   int     dim,            // in : length of each pt
   double &u1,             // out: u1 of min_pt on line1
   double &u2) ;           // out: u2 of min_pt on line2

double
DS_line_polyline_dist2     // ret: min(dist**2) from line to polyline
  (const double *p0,             // in : p0 of line = p0 + uu*(p1-p0)
   const double *p1,             // in : p1 of line = p0 + uu*(p1-p0)
   const double *polyline,       // in : ordered [xyz0, xyz1.., xyzn]
   int     pt_count,       // in : number of points in polyline
   int     dim,            // in : length of each pt
   double &line_uu,        // out: dist along line to min(not bounded)
   int    &seg_index,      // out: segment index containing the min
   double &seg_uu) ;       // out: dist along seg to min 0 < uu <= 1

int                        // rtn: -1=err, 0,1 or 2 = xsect_count
DS_line_sphere_min_loc     // eff: get line pt closest to sphere
  (int image_dim,          // in : problem's dimension size
   const double *pC,             // in : Sphere Center |(p-pC)/ABC|-R=0
   const double *ABC,            // in :  Aspect ratios, if(NULL)ABCi=1.0
   double R,               // in :  Radius
   const double *p0,             // in : p0 of iline = p0 + u*(p1-p0)
   const double *p1,             // in : p1 of iline = p0 + u*(p1-p0)
   double *uu) ;           // out: uu of iline at nearest pt [u0 u1]
                           //      u1 defined when xsect_count == 2

int
DS_line_plane_xsect        // rtn: 0=success,-1=no intersection
  (const double *norm,           // in : unit-normal of plane
   const double *pt,             // in : pt on plane
   const double *p0,             // in : p0 of iline = p0 + u*(p1-p0)
   const double *p1,             // in : p1 of iline = p0 + u*(p1-p0)
   double &uu,             // out: uu of iline at xsection
   double *xsect) ;        // out: xsection pt in image space

int                        // rtn: number of xsects found (0,1,or 2)
DS_line_round_2d_xsect     // eff: xsect line and round in 2d
  (const double *p0,             // in : p0 of line = p0 + u*(p1-p0)
   const double *p1,             // in : p1 of line = p0 + u*(p1-p0)
   const double *pc,             // in : circ = pc + pa*cos(s) + pb*sin(s)
   const double *pa,             // in : pa of circ
   const double *pb,             // in : bo of circ
   double *uu,             // out: u line value at xsects.Sized:[2]
   double *ss,             // out: ss vals at xsects in radians.
                           //      Sized:[2]
   double *xsects) ;       // out: [u0,v0,u1,v1] xsect locations. 
                           //      Sized:[4]
                           // com: outputs undefined when rtn==0

int                        // rtn: number of xsects (0,1 or 2)
DS_line_sphere_xsect       // eff: implicit Sphere/explicit Line calc
  (int image_dim,          // in : problem's dimension size
   const double *pC,             // in : Sphere Center of |(p-pC)/ABC|-R=0
   const double *ABC,            // in :   Aspect ratios, if(NULL) ABCi=1.0
   double R,               // in :   Radius
   const double *p0,             // in : p0 of iline = p0 + u*(p1-p0)
   const double *p1,             // in : p1 of iline = p0 + u*(p1-p0)
   double *uu) ;           // out: uu of iline at nearest pt [u0 u1]
                           //      u1 defined when xsect_count == 2

int                        // rtn: 1=x_sect is non_null,0=xsect is null                      
DS_span_span_xsect         // eff: get xsect of two spans
  (int span_dim,           // in : number of span dimensions
   const double *min1,           // in : min vals for span1,sized:span_dim
   const double *max1,           // in : max vals for span1,sized:span_dim
   const double *min2,           // in : min vals for span2,sized:span_dim
   const double *max2,           // in : max vals for span2,sized:span_dim
   double *x_min,          // out: xsect min values,  sized:span_dim
   double *x_max) ;        // out: xsect max values,  sized:span_dim

int                        // rtn: 1=x_sect is non_null,0=xsect is null                      
DS_span_span_xsectP        // eff: test whether two span xsect
  (int span_dim,           // in : number of span dimensions
   const double *min1,           // in : min vals for span1,sized:span_dim
   const double *max1,           // in : max vals for span1,sized:span_dim
   const double *min2,           // in : min vals for span2,sized:span_dim
   const double *max2) ;         // in : max vals for span2,sized:span_dim


int                        // rtn: number of real roots (0,1 or 2)
DS_quadratic_real_roots    // eff: calc roots of a*u*u+b*u+c=0
  (double a,               // in : a of   a*u*u + b*u + c = 0
   double b,               // in : b of   a*u*u + b*u + c = 0
   double c,               // in : c of   a*u*u + b*u + c = 0
   double *uu) ;           // out: root values. Sized:[2]

int                        // rtn: 1=success,0= a&b coeffs == 0                        
DS_quadratic_roots         // eff: calc roots of a*u*u+b*u+c=0
  (double a,               // in : a of   a*u*u + b*u + c = 0
   double b,               // in : b of   a*u*u + b*u + c = 0
   double c,               // in : c of   a*u*u + b*u + c = 0
   double rr[2],           // out: real root values. Sized:[2]
   double ii[2]) ;          // out: imaginary root values, Sized:[2]

int                        // rtn: 1=vecs are parallel, 0=they aren't
DS_is_parallel             // rtn: 1 if 3D SPAvector a is parallel to b
  (const double *v,              // in : SPAvector 1
   const double *w,              // in : SPAvector 2
   double tol) ;           // in : tol = sin(tolerance_angle)

// DECL_DM to keep ds2acis, dscommit DS_acis_assert's
int DECL_DM                // rtn: 1=vecs are parallel, 0=they aren't
DS_is_parallel_2vec        // rtn: 1 if 2D SPAvector a is parallel to b
  (const double *v,              // in : SPAvector 1
   const double *w,              // in : SPAvector 2
   double tol) ;           // in : tol = sin(tolerance_angle)

double                     // rtn: angle between 2 vectors in radians
DS_angle_3vec              // eff: measure angle from vec1 to vec2
  (const double *v,              // in : SPAvector 1, Sized:[3]
   const double *w) ;            // in : SPAvector 2, Sized:[3]

int                        // rtn: number of real roots (0,1 or 2)
DS_cubic_real_root_count   // eff: calc roots of u*u*u*+a*u*u+b*u+c=0
  (double a,               // in : a of   u*u*u + a*u*u + b*u + c = 0
   double b,               // in : b of   u*u*u + a*u*u + b*u + c = 0
   double c,               // in : c of   u*u*u + a*u*u + b*u + c = 0
   int &root_count,        // out: number of real roots to abc
   double &Q,              // out: Q=(a*a - 3*b)/9
   double &R) ;            // out: R=(2*a*a*a - 9*a*b + 27*c)/54
                           //      root_count == 1 when R*R - Q*Q*Q > 0
                           //      root_count == 3 when Q*Q*Q - R*R >= 0

void
DS_copy_3vec               // eff: copy src into tgt
  (const double *src,            // in : source SPAvector
   double *tgt) ;          // out: target SPAvector

double                     // rtn: the input magnitude of vec
DS_normalize_vec           // eff: normalizes the input vec
  (int dim,                // in : vector's dimension
   double *vec) ;          // i/o: SPAvector to be normalized

double                     // rtn: the input magnitude of vec
DS_normalize_3vec          // eff: normalizes the input vec
  (double *vec) ;          // i/o: SPAvector to be normalized

double                     // rtn: the input magnitude of vec
DS_normalize_2vec          // eff: normalizes the input vec
  (double *vec) ;          // i/o: SPAvector to be normalized

void
DS_cross_3vec              // eff: comp and store r = (a X b)
  (const double *a,              // in : a of r = (a X b)
   const double *b,              // in : b of r = (a X b)
   double *r) ;            // out: r of r = (a X b)

double                     // rtn: a[0]*b[1]-a[1]*b[0]
DS_cross_2vec              // eff: result is the sign and amount
                           //      of area spanned by a and b
  (const double *a,              // in : a of r = (a X b)
   const double *b) ;            // in : b of r = (a X b)

double                     // rtn: determinate[a,b,c]
DS_determinate_3mat        // eff: calc det[a,b,c]
  (const double *a,              // in : a of [a,b,c]
   const double *b,              // in : b of [a,b,c]
   double *c) ;            // out: c of [a,b,c]

double                     // rtn: (axb).c (same as det_3mat)
DS_triple_product          // eff: calc dot_product(cross(a,b),c)
  (const double *a,              // in : a of [a,b,c]
   const double *b,              // in : b of [a,b,c]
   double *c) ;            // out: c of [a,b,c]

double
DS_dot_3vec                // rtn: dot_product(a,b)
  (const double *a,              // in : a of r = (a X b)
   const double *b) ;            // in : b of r = (a X b)

double
DS_dot_2vec                // rtn: dot_product(a,b)
  (const double *a,              // in : a of r = (a X b)
   const double *b) ;            // in : b of r = (a X b)

double
DS_dot_vec                 // rtn: dot_product(a,b)
  (int dim,
   const double *a,              // in : a of r = (a X b)
   const double *b) ;             // in : b of r = (a X b)

double
DS_size_2vec               // rtn: size of a = sqrt(a0*a0+a1*a1)
  (const double *a) ;            // in : tgt SPAvector, sized:[2]

double
DS_size_3vec               // rtn: size of a = sqrt(a0**2+a1**2+a2**2)
  (const double *a) ;            // in : tgt SPAvector, sized:[3]

double
DS_size_vec                // rtn: size of a = sqrt(sum_i(ai*ai))
  (int dim,                // in : dimension of a
   const double *a) ;            // in : tgt SPAvector

double
DS_size2_vec               // rtn: size**2 of a = (sum_i(ai*ai))
  (int dim,                // in : dimension of a
   const double *a) ;            // in : tgt SPAvector

double
DS_size2_3vec              // rtn: dot_product(a,a)
  (const double *a) ;            // in : tgt SPAvector

double
DS_size2_2vec              // rtn: dot_product(a,a)
  (const double *a) ;            // in : tgt SPAvector

void
DS_add_3vec                // eff: store r = a + b
  (const double *a,              // in : source SPAvector
   const double *b,              // in : source SPAvector
   double *r) ;            // out: r = a + b

void
DS_add_vec                 // eff: store r = a + b
  (int dimension,          // in : SPAvector length
   const double *a,              // in : source SPAvector
   const double *b,              // in : source SPAvector
   double *r) ;            // out: r = a + b

double *                   // rtn: ptr to r
DS_sub_2vec                // eff: store r = a - b
  (const double *a,              // in : source SPAvector
   const double *b,              // in : source SPAvector
   double *r) ;            // out: r = a - b

double *                   // rtn: ptr to r
DS_sub_3vec                // eff: store r = a - b
  (const double *a,              // in : source SPAvector
   const double *b,              // in : source SPAvector
   double *r) ;            // out: r = a - b

double *                   // rtn: ptr to r
DS_sub_vec                 // eff: store r = a - b
  (int dimension,          // SPAvector sizes
   const double *a,              // in : source SPAvector
   const double *b,              // in : source SPAvector
   double *r) ;            // out: r = a - b

void
DS_add_scaled_3vec         // eff: store r = a*v0 + b*v1
  (double  a,              // in : scale of v0
   const double *v0,             // in : source SPAvector
   double  b,              // in : scale of v1
   const double *v1,             // in : source SPAvector
   double *r) ;            // out: r = a*v0 + b*v1

void
DS_add_3scaled_3vec        // eff: store r = a*v0 + b*v1 + c*v2
  (double  a,              // in : scale of v0
   const double *v0,             // in : source SPAvector
   double  b,              // in : scale of v1
   const double *v1,             // in : source SPAvector
   double  c,              // in : scale or v2
   const double *v2,             // in : source SPAvector
   double *r) ;            // out: r = a*v0 + b*v1 + c*v2

void
DS_add_scaled_vec          // eff: store r = a*v0 + b*v1
  (int dim,                // in : size of r, v0, and v1
   double  a,              // in : scale of v0
   const double *v0,             // in : source SPAvector
   double  b,              // in : scale of v1
   const double *v1,             // in : source SPAvector
   double *r) ;            // out: r = a*v0 + b*v1

void
DS_Wuv_2Wt                 // eff: store Wt = Cu*Wu + Cv*Wv
  (int image_dim,          // in : image_dim of W vectors
   double  Cu,             // in : u unit-SPAvector coordinate
   double  Cv,             // in : v unit-SPAvector coordinate
   double *Wu,             // in : parametric derivative in u
   double *Wv,             // in : parametric derivative in v
   double *Wt) ;           // out: Wt = Cu*Wu + Cv*Wv the
                           //      direction derivative in dir C.
void
DS_Wuv_2Wtt                // eff: Wtt= Cu*Cu*Wu+2*Cu*Cv*Wuv+Cv*Cv*Wvv
  (int image_dim,          // in : image_dim of W vectors
   double  Cu,             // in : u unit-SPAvector coordinate
   double  Cv,             // in : v unit-SPAvector coordinate
   double *Wuu,            // in : parametric derivative in u
   double *Wuv,            // in : parametric derivative in u and v
   double *Wvv,            // in : parametric derivative in v
   double *Wtt) ;          // out: Wtt=     Cu*Cu*Wu
                           //          +2.0*Cu*Cv*Wuv
                           //          +    Cv*Cv*Wvv
                           //      direction derivative in dir C.

void
DS_Wuv_2Wn                 // eff: store Wn = -Cv*Wu + Cu*Wv
  (int image_dim,          // in : image_dim of W vectors
   double  Cu,             // in : u unit-SPAvector coordinate
   double  Cv,             // in : v unit-SPAvector coordinate
   double *Wu,             // in : parametric derivative in u
   double *Wv,             // in : parametric derivative in v
   double *Wn) ;           // out: Wn = -Cvu*Wu + Cu*Wv the
                           //      direction derivative in dir 
                           //      normal to SPAunit_vector C.

void
DS_Wuv_2Wnn                // eff: Wtt= Cv*Cv*Wu-2*Cv*Cu*Wuv+Cu*Cu*Wvv
  (int image_dim,          // in : image_dim of W vectors
   double  Cu,             // in : u unit-SPAvector coordinate
   double  Cv,             // in : v unit-SPAvector coordinate
   double *Wuu,            // in : parametric derivative in u
   double *Wuv,            // in : parametric derivative in u and v
   double *Wvv,            // in : parametric derivative in v
   double *Wnn) ;          // out: Wnn=     Cv*Cv*Wu
                           //          -2.0*Cv*Cu*Wuv
                           //          +    Cu*Cu*Wvv
                           //      direction derivative in dir C.
void
DS_project_away_vec        // eff: res= raw - proj*(dotprod(raw,proj)/|proj|^2)
  (      int dim,          // in : dim of vectors
   const double *raw_vec,  // in : initial SPAvector
   const double *proj_vec, // in : project raw perpendicular to this SPAvector
         double *res_vec);  // out: resulting SPAvector (dotprod(res,proj) = 0)
// effects :

void
DS_calc_uv_projection      // eff: 
  (int image_dim,          // in : image_dim of W vectors
   const double *tangu,             // in : 
   const double *tangv,             // in : 
   const double *image_vec,         // in : 
   double *domain_dir);           // out: 

void
DS_calc_xyz_perp_coefs      // eff: 
  (int image_dim,          // in : image_dim of W vectors
   const double *edge_dir,             // in : 
   const double *tangu,             // in : 
   const double *tangv,             // in : 
   double *perp_dir);           // out: .
// modifies: 
// effects : 

void
DS_scale_3vec              // eff: store r = s*a
  (const double *a,              // in : source SPAvector
   double  s,              // in : scale
   double *r) ;            // out: r = s*a

DECL_DM void
DS_scale_vec               // eff: store r = s*a
  (int dim,                // in : dimension of vectors a and r
   const double *a,              // in : source SPAvector
   double  s,              // in : scale value
   double *r) ;            // out: r = s*a

void
DS_tangent_2basis          // rtn   : basis vecs aligned with tangent
  (double *tangent,        // in/out: 3D tangent SPAvector
   double *norm,           // out   : 3d vec perp to tangent
   double *binorm) ;       // out   : 3d vec perp to tangent and norm

void
DS_twovec_2basis           // rtn: basis vecs oriented by 2 input vecs
  (double *vec0,           // in : unit-SPAvector going into a corner
   double *vec1,           // in : unit-SPAvector coming out of corner
   double *tangent,        // out: cross(norm,binorm)
   double *norm,           // out: norm(cross(vec1,vec0))
   double *binorm) ;       // out: norm(norm(vec0)+norm(vec1))

int                        // 0=success, -1=cross(vec0,vec1)=0
                           // eff: find perp dir to SPAvector in 
DS_calc_perp_in_plane      //      a given plane.
  (double *vec0,           // in : the tan dir, sized:[3]
   double *vec1,           // in : 2nd vec in plane, sized[3]
   double *norm) ;         // out: unit vec perp to vec0 and 
                           //      coplanar with vec0 and vec1

void                      
DS_calc_km                 // eff: calc curvature vec k*m for Wu and Wuu
  (double *Wu,             // in : dW/du, sized:[3]
   double *Wuu,            // in : d2W/duu, sized:[3]
   double *km) ;           // out: k*m =  cross(cross(Wu,Wuu),Wu)
                           //           / dot(Wu,Wu)**2

int                        // rtn: 0=success, -1=cross(Wu,Wuu)=0
DS_calc_frenet_frame       // eff: get t,b,m from Wu, Wuu
  (double *Wu,             // in : dW/du, sized:[3]
   double *Wuu,            // in : d2W/duu, sized:[3]
   double *t,              // out: tangent t=normalized(Wu)
   double *m,              // out: normal  n=cross(b,t)
   double *b) ;            // out: bi-normal b= nomalized(cross(Wu,Wuu))

double                     // rtn: unsigned curvature
DS_calc_curvature          // eff: k = size(cross(Wu,Wuu))/size(Wu)**3
 (int image_dim,           // in : 2 or 3 for 2d or 3d
  double *Wu,              // in : dW/du, sized:[3]
  double *Wuu) ;           // in : d2W/du2, sized:[3]

int                        // rtn: 0=success, -1=torsion not defined
DS_calc_torsion            // eff: T = det[Wu,Wuu,Wuuu]/size(cross(Wu,Wuu))**2
 (double *Wu,              // in : dW/du, sized:[3]
  double *Wuu,             // in : d2W/du2, sized:[3]
  double *Wuuu,            // in : d3W/du3, sized:[3]
  double &torsion) ;       // out: torsion set for success

int                        // rtn: 1=norm,binorm unique,0=frenet on straight
DS_calc_normal_vec         // eff: calc normal vec given derivative values
 (int domain_dim,          // in : 1 for curves, 2 for surfaces
  int image_dim,           // in : 2 or 3 for curves, 3 for surfaces
  double *dW1,             // in : Wu for curves, Wu for surfaces
  double *dW2,             // in : Wuu for curves, Wv for surfaces
  double *norm_vec,        // out: normalized, normal SPAvector
  double *binorm_vec=NULL);// out: NULL on iput or
                           //      normalized, binorm (curves only)
                           //      dW1, dW2, image_dir sized:[image_dim]

/*
int                        // rtn:0=success,-1=n is zero,
DS_principal_curvatures    // eff: pein curvatures from para derivatives
  (double Wu[3],           // in : 1st para derivative dW/du
   double Wv[3],           // in : 1st para derivative dW/dv
   double Wuu[3],          // in : 2nd para derivative d2W/du2
   double Wuv[3],          // in : 2nd para derivative d2W/dudv
   double Wvv[3],          // in : 2nd para derivative d2W/dv2
   double &kmin,           // in : min curvature (a real number)
   double min_dir[2],      // in : angle to min curv (from u axis)
   double &kmax,           // in : max curvature (always a real number)
   double max_dir[2]) ;    // in : angle to max curv (from u axis)
*/

int                        // rtn: 1=x_sect is non_null,0=xsect is null                      
DS_xsect_spans             // eff: get xsect of two spans
  (int span_dim,           // in : number of span dimensions
   double *min1,           // in : min vals for span1,sized:span_dim
   double *max1,           // in : max vals for span1,sized:span_dim
   double *min2,           // in : min vals for span2,sized:span_dim
   double *max2,           // in : max vals for span2,sized:span_dim
   double *x_min,          // out: xsect min values,  sized:span_dim
   double *x_max) ;        // out: xsect max values,  sized:span_dim

double                     // rtn: area(span1)/area(span2)
DS_span_ratio              // eff: compare the area of 2 spans
  (int span_dim,           // in : number of span dimensions
   double *min1,           // in : min vals for span1,sized:span_dim
   double *max1,           // in : max vals for span1,sized:span_dim
   double *min2,           // in : min vals for span2,sized:span_dim
   double *max2) ;         // in : max vals for span2,sized:span_dim

void
DS_rot_vec_about_axis      // eff: rotate a vec about an axis
  (double *vec,            // in : SPAvector to rotate, sized:[3]
   double *axis,           // in : unit_vec axis of rotation, sized:[3]
   double  ang,            // in : right hand rotation in radians
   double *out_vec) ;      // out: out_vec = (v.a)*a
                           //                + cos(ang)*a^(a^v)
                           //                + sin(ang)*(a^v)
                           //   where: ^ = cross_product
                           //          . = dot_product
   
int                        // rtn: number of SPAmatrix struct errors (0=success)
                           // eff: reduce mat to LDLt form
DS_sym_mat_to_LDLt_with_diag_pivots 
  (int n,                  // in : mat size[nxn] stored upper diag only 
   double eps,             // in : value used to approximate zero.
   double *mat,            // i/o: tgt mat(in row order),sized:[n(n+1)/2]
   int *mat_map,           // out: column pivoting indirection, sized:[n]
   int &p) ;               // out: rank=number of independent rows in mat       

int                        // rtn: conflicting constraint count (0=success)
DS_solve_LDLt_equal_b_with_diag_pivots
                           // eff: solve for y in LDLt y = b
  (int n,                  // in : mat size[nxn] store upper diag only 
   int image_dim,          // in : number of columns in b SPAmatrix
   double eps,             // in : value used to approximate zero.
   double *mat,            // in : tgt mat(in row order),sized:[n(n+1)/2]
   int *mat_map,           // in : column pivoting indirection, sized:[n]
   int p,                  // in : rank=number of independent rows in mat       
   double *b) ;            // i/o: in=b,out=y stored by columns,sized:[n*i]

double                     // rtn: compute offset to map center
DS_calc_offset             //      of one range to another.
  (double cur_min,         // in : current range minimum
   double cur_max,         // in : current range maximum
   double scale) ;         // in : scale factor from cur range to
                           //      new range.

double                     // out: sqrt(a*a + b*b) 
DS_PYTHAG                  // eff: avoid overflow or underflow.
  (double a,               // in : 1st base leg
   double b) ;             // in : 2nd base leg

                           // def: written for consecutive elem numbering
int                        // rtn: 0=success,-1=non NULL input ptr,-2=failed new
DS_list_compliment         // eff: make set=all_elems-list 
  (int count,              // in : size of list
   int *list,              // in : list of indices
   int max_index_value,    // in : max index value allowed
   int &compliment_count,  // out: size of compliment_list
   int *&compliment_list) ;// out: compliment of input index list
                           //      mallocs


#endif // DS_STDEF_H
