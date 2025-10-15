/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dseqns.hxx
// created  : August 23, 1999
// author   : bmt
// contains : DS_eqns  - base class for eqns linear equations
// synopsis : Virtual base class for eqns linear equations.
#ifndef DS_eqns_H
#define DS_eqns_H
#include "dsstdef.hxx" // for DS_TRUE, DS_FALSE // for DS_TRUE, DS_FALSE
class DS_overlap_vector;
class DM_dbl_array;

class DS_eqns : public ACIS_OBJECT
{
   private: 
// Should not call 
   DS_eqns&
   operator=                                       // Assignment Operator
   (const DS_eqns&);
     
   public:
     enum TYPE_ID { ds_sym,
                    ds_kkt };
//
  virtual void
   Size_arrays                      // Size_arrays
   (int dof_count=0,                // row count of Ax=b, note A is square
    int image_dim=0,                // col count of b, x, and d       
    int cd_row_count=0,             // row count of Cx=d              
    int LLc_row_count=0) = 0;       // row count of L[x0|..|xi-1]=Lc  

// clear functions
     virtual void Clear_A() = 0;    // set A of Ax=b to zero
     virtual void Clear_b() = 0;    // set b of Ax=b to zero
     virtual void Clear_x() = 0;    // set x and old_x of Ax=b to zero
     virtual void Clear_Cd() = 0;   // set Cx=d arrays to zero
     virtual void Clear_d() = 0;    // set d of Cx=d to zero
     virtual void Clear_L() = 0;    // set L of Lx=Lc to zero
     virtual void Clear_Lc() = 0;   // set LC of Lx=Lc to zero
     virtual void Clear_LA() = 0;   // set LA of LA y = lb to zero

// DS_symeq simple data access and management functions
     virtual int Dof_count() const = 0;        // row count of A = column count of A
     virtual int Image_dim() const = 0;        // 3 or 2
     virtual int Cd_row_count() const = 0;     // (unreduced) row count for the C SPAmatrix
     virtual int LLc_row_count() const = 0;    // (unreduced) row count for the L SPAmatrix
/*     virtual double A(int ni, int nj) = 0;  // the ijth entry in A
     virtual double B(int ni, int nj) = 0;  // the ijth entry in b
     virtual double C(int ni, int nj) = 0;  // the ijth entry in C
     virtual double D(int ni, int nj) = 0;  // the ijth entry in d
     virtual double L(int ni, int nj, int nx) = 0;  
*/                                             // the ijth entry in the nx-th component of L
                                             // 0<=nx<=2
// Controlled build functions
  virtual void Add_to_A    // eff: Add terms to A SPAmatrix
   (int ni,                // in : i of A[i][j] += value (sme_n)
    int nj,                // in : j of A[i][j] += value (sme_n)
    double val) = 0;       // in : value of A[i][j] += value

  void Add_outer_product_to_A      // eff: Add terms to A SPAmatrix
   (DS_overlap_vector const & vec, // in : SPAvector whose outer product will be added to A
    double weight);                // in : multiplier of A[i][j] += weight*vec[i]*vec[j]

  virtual void Add_to_b    // eff: Add terms to b SPAmatrix
   (int ni,                // in : i of b[i][d] += value (sme_n)
    int id,                // in : d of b[i][d] += value (sme_i)
    double val) = 0;       // in : value of b[i][d] += value

  void Add_outer_product_to_B      // eff: Add terms to B SPAmatrix
   (DS_overlap_vector const & overlap, // in : SPAvector of size Ndof
    DM_dbl_array const & vec,      // in : SPAvector of size Image_dim
    double weight);                // in : multiplier of B[i][mu] += weight*overlap[i]*vec[mu]

  virtual void             // bas: pure DS_eqns virtual method        
   Add_to_C_min            // eff: Add terms to C "minized" SPAmatrix
   (int ni,                // in : dof index into Cx=d min eqns
    int nj,                // in : dof index of col in C[i][j] += value
    double val)=0 ;        // in : value   of C[i][j] += value

  virtual void             // bas: pure DS_eqns virtual method        
   Add_to_C_line           // eff: Add terms to C SPAmatrix
   (int qi,                // in : row index into Cx=d line eqns
    int nj,                // in : dof index of col in C[i][j] += value
    double val)=0 ;        // in : value   of C[i][j] += value

  virtual void             // bas: pure DS_eqns virtual method        
   Add_to_d_min            // eff: Add terms to d "minimized" SPAmatrix
   (int ni,                // in : dof index into Cx=d min eqns
    int id,                // in : d index of d[i][d] += value
    double val)=0 ;        // in : value   of d[i][d] += value

  virtual void             // bas: pure DS_eqns virtual method        
   Add_to_d_line           // eff: Add terms to d SPAmatrix
   (int qi,                // in : row index into Cx=d line eqns
    int id,                // in : d index of d[i][d] += value
    double val)=0 ;        // in : value   of d[i][d] += value

   virtual void Add_to_Li   // eff: Add terms to Li of Li*xi+LA*r=Lc
    (int id,                // in : i of Li[j][k] += value (sme_i)
     int lj,                // in : j of Li[j][k] += value (sme_l)
     int nk,                // in : k of Li[j][k] += value (sme_n)
     double val) = 0;       // in : value of Li[j][k] += value

   virtual void Add_to_Lc   // eff: Add terms to Lc of Li*xi+LA*r=Lc
    (int li,                // in : i of Lc[i] += value    (sme_l)
     double val) = 0;       // in : value of Lc[i] += value

  virtual void Add_to_LA    // eff: Add terms to LA of Li*xi+LA*r=Lc
    (int li,                // in : i of LA[i][j] += value (sme_l)
     int lj,                // in : j of LA[i][j] += value (sme_l)
     double val) = 0;       // in : value of LA[i][j] += value

  DS_eqns                   // eff: "default" constructor
    (TYPE_ID type_id=ds_sym)// in : derived class identifier (required)
    :
     bul_type_id(type_id)
    {}

  virtual ~DS_eqns()        // destructor
     {}

//
   protected:
    const TYPE_ID   bul_type_id ;  // enum showing specialization type


}; // end of class DS_eqns

#endif //DS_eqns_H
