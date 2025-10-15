/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dslueqns.hxx
// created  : Sep 21, 1999
// author   : bmt
// contains : DS_lueqns  - Linear eqns for KKT-LU solver; taken from DS_cgeqns taken from jhs' DS_hybrid
//                                

#ifndef DS_LUEQNS_H
#define DS_LUEQNS_H

#include "dsmbrmt.hxx"   // class DS_mbvec_row_matrix   // class DS_mbvec_row_matrix
#include "dsbcmat.hxx"   // class DS_block_clm_matrix   // class DS_block_clm_matrix
#include "dsblvec.hxx"   // class DS_block_vec   // class DS_block_vec
#include "dsbridge.hxx"  // class DS_bridge  // class DS_bridge
#include "dsreduc.hxx"   // class DS_row_reducer   // class DS_row_reducer
#include "dseqns.hxx"    // class DS_eqns, DS_lueqns base class    // class DS_eqns, DS_lueqns base class
#include "dsmatblk.hxx"  // class DS_mat_block  // class DS_mat_block

// local state bits for tracking changes in the ABCD problem
#define SYM_CHANGED_A     (1 << 0) // 1=A changed since last Solve()
#define SYM_CHANGED_B     (1 << 1) // 1=b changed since last Solve()
#define SYM_CHANGED_C     (1 << 2) // 1=C changed since last Solve()
#define SYM_CHANGED_D     (1 << 3) // 1=d changed since last Solve()
#define SYM_CHANGED_L     (1 << 4) // 1=L changed since last Solve()
#define SYM_CHANGED_LC    (1 << 5) // 1=Lc changed since last Solve()
#define SYM_INIT_LA       (1 << 6) // 1=clear LA prior to Solve()
#define SYM_INIT_DOF_MAP  (1 << 7) // 1=clear dof_map in Reduce_Cd_toCde()
#define SYM_INIT_BF       (1 << 8) // 1=rebuild bf before using
#define SYM_INIT_DF       (1 << 9) // 1=rebuild df before using
#define SYM_INIT_LCF      (1 << 10)// 1=rebuild Lcf before using


class DS_lueqns  : public DS_eqns           // identifier cge
{
  private:

  int lue_n;           // N
  int lue_q;           // This is the number of point constraints
  int lue_l;           // NLc
  int lue_i;           // Dim
  int lue_m;           // dimension of span CT = number of independent rows of C
                       // The "free count" is N - M (=P); supports DM_get_dof_state
  int lue_lp;          // The number of independent rows of Lc; supports DM_get_dof_state

  int c_reduced_state; // =1 if C is reduced, 0 if C is not reduced

  DS_matblock lue_matblock;

  DS_mbvec_row_matrix lue_A;
  DS_mbvec_row_matrix lue_C;
  DS_mbvec_row_matrix lue_L[3];

  DS_block_clm_matrix lue_B;
  DS_block_clm_matrix lue_D;
  DS_block_vec        lue_Lc;

  DS_block_clm_matrix lue_X;

  DS_mbvec_row_matrix lue_Cred;
  DS_block_clm_matrix lue_Dred;
  DS_row_reducer      lue_Creducer;

  DS_mbvec_row_matrix lue_Ltmp[3];
  DS_mbvec_row_matrix lue_Lred;
  DS_block_vec        lue_Lcred;
  DS_row_reducer      lue_Lreducer;

  DS_multi_banded_vec lue_Scale_C;  // Used for scaling C & D
  DS_multi_banded_vec lue_Scale_A;  // Used for scaling C & D



 public:
  // constructor
  DS_lueqns                  // default constructor for Ax=b, Cx=d
   (int dof_count=0,         // row count of Ax=b, note A:[NxN]   (sme_n)
    int image_dim=0,         // col count of b, x, and d          (sme_i)
    int cd_row_count=0,      // Number of point constraints       (sme_q)
    int LLc_row_count=0,     // row count of L[x0|...|xi-1] = Lc  (sme_l)
    int halfband_A=0);       // An upper-diag count(excl main-diagonal)

  // copy constructor
  DS_lueqns                  // copy constructor for Ax=b, Cx=d
   (const DS_lueqns & eq);

  DS_lueqns & operator=(const DS_lueqns & eq);

    int  Reduce(int abcd_state);
//
// Size query; used by the api function DM_get_dof_state 
    int Raw_C_size() const   // Size of the raw C-SPAmatrix, including 0 rows
       { return lue_q+lue_n; }
    int Dof_count() const
       { return lue_n; }     // Size of the square SPAmatrix A
    int Image_dim() const
       { return lue_i; }     // Image dim, 2 for curves, 3 for surfs
    int Cd_row_count() const   
       { return lue_q; }     // Number of point constraints
    int LLc_row_count() const
       { return lue_l; }
    int Fixed_count() const  // return the row dimension of C
       { return lue_m; }     // name chosen to be consistent with symeq 
    int Free_count() const   // return dim A - row dim C
       { return lue_n - lue_m; }
                             // name chosen to be consistent with symeq 
    int Free_mix_count() const
       { return lue_lp; }    // return the row rank of Lc; lue_lp is set after calling Reduce_L()
// State query
    int Is_C_reduced() const
       { return c_reduced_state; }
// clear functions
    void Clear_A() ;         // set A of Ax=b to zero
    void Clear_b() ;         // set b of Ax=b to zero
    void Clear_x() ;         // set x and old_x of Ax=b to zero
    void Clear_Cd();         // set Cx=d arrays to zero
    void Clear_d() ;         // set d of Cx=d to zero
    void Clear_L() ;         // set L of Lx=Lc to zero
    void Clear_Lc();         // set LC of Lx=Lc to zero
    void Clear_LA();         // set LA of LA y = lb to zero

    const DS_mbvec_row_matrix & A() const {return lue_A;}
    const DS_block_clm_matrix & B() const {return lue_B;}
    const DS_mbvec_row_matrix & C() const {return lue_C;}
    const DS_block_clm_matrix & D() const {return lue_D;}
    const DS_mbvec_row_matrix & L(int ii) const {return lue_L[ii];}
    const DS_block_vec & Lc() const {return lue_Lc;}
    const DS_block_clm_matrix & X() const {return lue_X;}

    void Set_A(const DS_abs_matrix & mat) {*(DS_abs_matrix*)&lue_A = mat;}
    void Set_B(const DS_abs_matrix & mat) {*(DS_abs_matrix*)&lue_B = mat;}
    void Set_C(const DS_abs_matrix & mat) {*(DS_abs_matrix*)&lue_C = mat;}
    void Set_D(const DS_abs_matrix & mat) {*(DS_abs_matrix*)&lue_D = mat;}
    void Set_L(const DS_abs_matrix & mat, int ii) {*(DS_abs_matrix*)&(lue_L[ii]) = mat;}
    void Set_Lc(const DS_abs_vec & vec) {*(DS_abs_vec*)&lue_Lc = vec;}
    void Set_X(const DS_abs_matrix & mat) {*(DS_abs_matrix*)&lue_X = mat;}

// Element query - DS_dmesh::Build_joint* calls the dmod's element queries
    double A(int ni, int nj) const // the ijth entry in A
       { return lue_A(ni,nj);}
    double B(int ni, int nj) const // the ijth entry in b
       {return lue_B(ni,nj);}  
    double C(int ni, int nj) const // the ijth entry in C
       {return lue_C(ni,nj);}  
    double D(int ni, int nj) const // the ijth entry in d
       {return lue_D(ni,nj);}  
    double L(int ni, int nj, int nx) const
       {return lue_L[nx](ni,nj);}
    double X(int ni, int nj) const // the ijth entry in X
       {return lue_X(ni,nj);}
    double Lc(int ni ) const
       {DS_assert(0<=ni&&ni<lue_Lc.Size());
        return lue_Lc[ni];}
    double Reduced_C(int ni, int nj) const; //the ijth entry of C, with redundant rows removed
    double Reduced_D(int ni, int nj) const; //the ijth entry of D, with redundant rows removed
    double Reduced_L(int row, int col, int nx) const;
    double Reduced_Lc(int row) const;
    int Get_nblocks() const
      { return lue_matblock.Get_nblocks();}
    int Get_block_edge(int n) const
      { return lue_matblock.Get_block_edge(n);}
    void Resize_blocks(int nmatsize, int nblocks ){ lue_matblock.Resize(nmatsize,nblocks);}
    void Set_block_edge(int nblock, int nedge) { lue_matblock.Set_block_edge(nblock,nedge); }
    int Is_Reduced_C_row_link(int n) const;
    int Is_C_row_link (int row) const;
    int Is_A_row_link (int row) const;
    int Is_reduced_C_row_zone_fixed (int row) const;
    int Is_C_row_zone_fixed(int row) const ;
    int C_row_zone_fixed(int row) const; // return -1 if not zone-fixed, fixed dof otherwise
    int Is_C_row_zero(int row) const;
    int Nonzero_C_row_count() const;
    int Zone_fixed_count () const;
    void Get_zone_fixed(DS_int_block& fixed_dof) const;
    void Get_link_dofs(DS_int_block& link_dof_mask ) const; 
    void Get_matblock(DS_matblock& block) const;
    int C_row_begin(int row) const {return ((DS_multi_banded_vec)lue_C.Row(row)).Begin();}
    int C_row_end(int row)   const {return ((DS_multi_banded_vec)lue_C.Row(row)).End();}
    int A_row_begin(int row) const {return ((DS_multi_banded_vec)lue_A.Row(row)).Begin();}
    int A_row_end(int row)   const {return ((DS_multi_banded_vec)lue_A.Row(row)).End();}
    void Unscale_Cnd();
   DS_lueqns*                // Make_copy
    Make_copy()            { return( ACIS_NEW DS_lueqns(*this)) ; }

//  The following two functions are why DS_lueqns is a friend of bridge.
    void Copy_x_from_bridge(const DS_bridge* brg, int row_offset=0 );
    void Copy_x_to_bridge(DS_bridge* brg, int row_offset=0 );
// Controlled build functions
    void Add_to_A            // eff: Add terms to A SPAmatrix
      (int ni,                          // in : i of A[i][j] += value (sme_n)
       int nj,                          // in : j of A[i][j] += value (sme_n)
       double val) ;                    // in : value of A[i][j] += value

    void Add_to_b            // eff: Add terms to b SPAmatrix
      (int ni,                          // in : i of b[i][d] += value (sme_n)
       int id,                          // in : d of b[i][d] += value (sme_i)
       double val) ;                    // in : value of b[i][d] += value

    void Add_to_C_min        // eff: Add terms to C SPAmatrix
      (int qi,                          // in : i of C[i][j] += value (sme_q)
       int nj,                          // in : j of C[i][j] += value (sme_n)
       double val) ;                    // in : value of C[i][j] += value

    void Add_to_C_line       // eff: Add terms to C SPAmatrix
      (int qi,                          // in : i of C[i][j] += value (sme_q)
       int nj,                          // in : j of C[i][j] += value (sme_n)
       double val) ;                    // in : value of C[i][j] += value

    void Add_to_d_min        // eff: Add terms to d SPAmatrix
      (int qi,                          // in : i of d[i][d] += value (sme_q)
       int id,                          // in : j of d[i][d] += value (sme_i)
       double val);                     // in : value of d[i][d] += value

    void Add_to_d_line       // eff: Add terms to d SPAmatrix
      (int qi,                          // in : i of d[i][d] += value (sme_q)
       int id,                          // in : j of d[i][d] += value (sme_i)
       double val);                     // in : value of d[i][d] += value

    void Add_to_Li           // eff: Add terms to Li of Li*xi+LA*r=Lc
      (int id,                // in : i of Li[j][k] += value (sme_i)
       int lj,                // in : j of Li[j][k] += value (sme_l)
       int nk,                // in : k of Li[j][k] += value (sme_n)
       double val) ;          // in : value of Li[j][k] += value

    void Add_to_Lc           // eff: Add terms to Lc of Li*xi+LA*r=Lc
      (int li,                // in : i of Lc[i] += value    (sme_l)
       double val) ;          // in : value of Lc[i] += value

    void Add_to_LA           // eff: Add terms to LA of Li*xi+LA*r=Lc
      (int li,                // in : i of LA[i][j] += value (sme_l)
       int lj,                // in : j of LA[i][j] += value (sme_l)
       double val) ;          // in : value of LA[i][j] += value

//
// bmt scale
    int Scale_A();
    int Scale_B();
    int Scale_C();
    int Scale_D();
// end bmt
    void Size_arrays                      // Size_arrays
    (int dof_count=0,                // row count of Ax=b, note A is square
    int image_dim=0,                // col count of b, x, and d       
    int cd_row_count=0,             // row count of Cx=d              
    int LLc_row_count=0);       // row count of L[x0|..|xi-1]=Lc  
    void Set_C_row_dim(int nsize); // eff : set sme_m,  the row dimension of C  int Reduce_C();
 private:
  void Set_L_row_dim(int nsize); // eff : set sme_lp, the row dimension of L  int Reduce_L();
  int Reduce_L();
  int Reduce_C();
  int  Check_D();
  int  Check_LC();
} ; // end class DS_lueqns

#endif // DS_LUEQNS_H        
