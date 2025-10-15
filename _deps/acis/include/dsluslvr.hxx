/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dsluslvr.hxx
// created  : Sep 21, 1999
// author   : bmt
// contains : DS_lu_solver
// synopsis : performs lu factor & solve on DS_multi_banded_row_matrix

#ifndef DS_luslvr_H
#define DS_luslvr_H
//
#include "dsfpred.hxx" //class DS_row_reducer //class DS_row_reducer
#include "dsreduc.hxx" //class DS_row_reducer //class DS_row_reducer
#include "dsmbrmt.hxx" //class DS_mbvec_row_matrix //class DS_mbvec_row_matrix
#include "dsbcmat.hxx" //class DS_mbvec_row_matrix //class DS_mbvec_row_matrix
#include "dslueqns.hxx"  //class DS_lueqns  //class DS_lueqns

//================================================================
class DS_lu_solver : public ACIS_OBJECT                      // identifier lus

{
// Should not call 
   DS_lu_solver&
   operator=                                       // Assignment Operator
   (const DS_lu_solver&);

//
     DS_matblock lus_rs_matblock;        // Defines the block structure of the mesh A-SPAmatrix
// Size of the raw KKT SPAmatrix
     int lus_n;                          // KKT SPAmatrix size; KKT SPAmatrix is square
     int lus_na;                         // Size of the A SPAmatrix; A SPAmatrix is square
     int lus_nc;                         // Rank of the C SPAmatrix = number of C-rows in the KKT SPAmatrix; mixed linear constraints NOT included
     int lus_nc_unred;                   // Number of unreduced C-rows; this is the number of C rows which eqns. gives us
     int lus_lp;                         // Rank of the mixed-linear constraint part of the C-SPAmatrix
     int lus_nraw;                       // Dof before zone-fixed dofs are removed
// Size of the range space blocks
     int lus_k11_nrows;                  // Size of the K11 part of the KKT SPAmatrix; K11 is a square block
     int lus_k22_nrows;                  // Size of the K22 part of the KKT SPAmatrix; K22 is a square block
// FYI:
//    lus_k21_nclms = lus_k11_nrows;
//    lus_k21_nrows = lus_k22_nrows;
//
     DS_mbvec_row_matrix lus_k11_matrix;  // Square with size lus_k11_nrows
     DS_mbvec_row_matrix lus_k21_matrix;  // Rectangular, lus_k22_rows by lus_k11_nrows
     DS_mbvec_row_matrix lus_k22_matrix;  // Square with size lus_k22_nrows
     DS_row_reducer lus_k22_reducer;      // LU-factor the k22 block.
//
     DS_mbvec_row_matrix lus_kkt_matrix; // KKT SPAmatrix.
     DS_mbvec_row_matrix lus_C_matrix;   // C SPAmatrix.
     DS_mbvec_row_matrix lus_C_red;      // C-SPAmatrix, result of reducing CT.
     DS_mbvec_row_matrix lus_L_matrix;   // Mixed linear constraint SPAmatrix.
     DS_block_vec lus_Scale_A;           // Used for symmetric scaling of A; also scales B on the left and C on the right.
                                         // Final solution is then lus_Scale_A * x
//
     DS_fp_reducer lus_CT_fpreducer;     // Full pivot reducer for the CT SPAmatrix
     DS_mbvec_row_matrix lus_C_lt;       // lt factor in C = ut*lt*P ==> CT = PT*l*u
     DS_mbvec_row_matrix lus_C_ut;       // ut factor in C = ut*lt*P ==> CT = PT*l*u
     DS_mbvec_row_matrix lus_C_zone_fixed_mat; // Zone-fixed part of the C SPAmatrix; removed before building RS SPAmatrix; used to build zone-reduced RHS
     DS_mbvec_row_matrix lus_A_zone_fixed_mat; // Zone-fixed part of the A SPAmatrix; removed before building RS SPAmatrix; used to build zone-reduced RHS
//
     DS_row_reducer lus_reducer;         // Formerly the KKT reducer
     DS_row_reducer lus_L_reducer;       // reducer for the mixed linear constraints
//
     DS_int_block lus_pivot_mask;
     int lus_nonlink_dof;
//
     DS_int_block lus_kkt_perm;          // RS SPAmatrix symmetric permutation
     DS_int_block lus_kkt_perm_inv;      // Inverse of lus_kkt_perm. lus_kkt_perm_inv[lus_kkt_perm[i]] == i
//
     DS_int_block lus_ct_perm;
// Zone stuff
     DS_int_block lus_zone_dof_map;      // Map from the zone-free dof's to the original KKT dof's
     DS_int_block lus_zone_dof_inv_map;  // Map from the original KKT dof's to the zone-free dof's; -1 if dof is zone-fixed
     DS_int_block lus_zone_Crow_map;     // Map from the zone-free C-rows to the originall KKT C-rows; 
                                         // Original KKT has all the non-zero eqns C-rows; 0's are removed
     DS_int_block lus_zone_Crow_inv_map; // Map for the original KKT (nonzero) C-rows to the zone-free C-rows.
     DS_int_block lus_zone_dof2crow_map; // -1 if dof is zone-free, otherwise, i-th entry is the FIXED C-row which fixes i-th dof
     DS_int_block lus_zone_dof2crow_inv_map; // i-th entry is the fixed dof which is fixed by the i-th fixed crow
//
     DS_int_block lus_link_dof_mask;     // =1 if this zone-free dof is a link, 0 otherwise
//
   public:
     DS_lu_solver( )                     // eff: constructor      
      :
       lus_reducer(lus_kkt_matrix),
       lus_k22_reducer(lus_k22_matrix),
       lus_L_reducer(lus_L_matrix),
       lus_CT_fpreducer(lus_C_red),
       lus_n(0),
       lus_na(0),
       lus_nc(0),
       lus_lp(0),
       lus_nonlink_dof(0),
       lus_nraw(0),
       lus_nc_unred(0)
      {}
         
//  
     ~DS_lu_solver() {}                                     // Destructor
      int Factor_rs_matrix(int abcd_state);
      int Apply_u_inv(DS_block_clm_matrix& soln, const DS_block_clm_matrix& rhs);

      int Factor(int abcd_state);                           // Factor the kkt SPAmatrix
      int Solve(DS_block_clm_matrix& soln, DS_block_clm_matrix& lambda, const DS_lueqns& eqns, int abcd_state);
                                                            // Forward and backward sub on the eqns rhs
#ifdef DSDEBUG
      int Apply_kkt(const DS_block_clm_matrix& x, DS_block_clm_matrix& prod );
      double Check_kkt_inverse(const DS_lueqns& eqns);
#endif
      int Apply_kkt_inverse(DS_block_clm_matrix& soln, const DS_block_clm_matrix& rhs );
                                                            // Forward and backward sub on an arbitrary rhs
      int Build_kkt(const DS_lueqns& eqns, int abcd_state); // Build the kkt SPAmatrix from the A, B, C, & D matrices
      int Build_rs_matrix(const DS_lueqns& eqns, int abcd_state); // Build the kkt SPAmatrix from the A, B, C, & D matrices
      int Build_L(const DS_lueqns& eqns, int abcd_state);   // Build the kkt mixed-linear constraint SPAmatrix.
      int Factor_L( int abcd_state );                       // Factor the kkt mixed-linear constraint SPAmatrix.
      int Nrows() const {return lus_n;}
      int Nrows_A() const {return lus_na;}
      int Nrows_C() const {return lus_nc;};
      int Nrows_C_unred() const {return lus_nc_unred;}
      int Nrows_K11() const {return lus_k11_nrows;};
      int Nrows_K22() const {return lus_k22_nrows;};
      int Nrows_Lc() const {return lus_lp;}
      int Scale_KKT();
      int Scale_solution();
      int Scale_B();
      int Scale_D();
      int Reduce_CT(const DS_matblock& blocks);
      int Nonlink_dof() const {return lus_nonlink_dof;}
      int Reduce_d_by_CT(DS_block_clm_matrix& d, double tol);
      int Dof_raw() const {return lus_nraw;} // Total dof before zones are removed
}; 


#endif //DS_luslvr_H
