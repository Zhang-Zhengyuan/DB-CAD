/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef DS_FPRED_H
#define DS_FPRED_H

#include "dsmbrmt.hxx" //class DS_mbvec_row_matrix, //class DS_mbvec_row_matrix,
                                       //class DS_abs_matrix
#include "dsmbvec.hxx" //class DS_multi_banded_vec, DS_abs_vec //class DS_multi_banded_vec, DS_abs_vec
//fwd ref
class DS_matblock;
//
class DS_fp_reducer : public ACIS_OBJECT                // identifier fpr

{
// Should not call 
   DS_fp_reducer&
   operator=                                       // Assignment Operator
   (const DS_fp_reducer&);

  double                fpr_tol;
  DS_int_block          fpr_col_perm;
  DS_int_block          fpr_row_perm;
  DS_int_block          fpr_col_pivot_mask;

  DS_mbvec_row_matrix & fpr_u_mat;  // After reduction, will store the upper triangle
public:
//
  void Init_fp_reducer(const DS_int_block& link_mask);
//
  DS_fp_reducer(                    // eff: constructor
   DS_mbvec_row_matrix & M,    // 
   double tolerance=1.e-12)
   :
    fpr_row_perm(M.Nrows()),
    fpr_col_perm(M.Nclms()),
    fpr_col_pivot_mask(M.Nclms()),
    fpr_tol(tolerance), 
    fpr_u_mat(M)
  {
  }
//
  int Fp_reduce(DS_mbvec_row_matrix& l_mat, DS_int_block& c_perm, double tol,
                const DS_matblock& blocks,                        // Compute the full-pivot reduction. When done, have PMQ = LU.
                const DS_int_block& link_mask);                   // P is encoded in fpr_row_perm, and Q is encoded in fpr_col_perm
                                                                  // PMQ[i,j] = M[fpr_row_perm[i],fpr_col_perm[j]].
private:
  double Get_pivot( int& is_masked, int& ir_piv, int& ic_piv, int begin, int end, 
                    double pivot_tol, const DS_mbvec_row_matrix& mat, const DS_int_block& nonzero_rows);

};

#endif // DS_FPRED_H
