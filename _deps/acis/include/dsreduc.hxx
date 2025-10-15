// $Id: dsreduc.hxx,v 1.12 2001/04/17 17:43:46 btomas Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dsreduc.hxx
// created  : Feb 21, 1999
// author   : John Sloan
// contains : DS_mb_row_reducer
// synopsis : performs row reduction on DS_multi_banded_row_matrix
// TODO:??? (JHS) Add First, Last data  as members to DS_abs_vec.  This
//                would allow rewrite to DS_row_reducer through interface.

#ifndef DS_REDUC_H
#define DS_REDUC_H

#include "dsbcmat.hxx"   // class DS_block_clm_matrix   // class DS_block_clm_matrix
#include "dsrhblk.hxx" //class DS_row_handle_block //class DS_row_handle_block
#include "dsrdlog.hxx" //class DS_reduce_log //class DS_reduce_log

#include "dsmbrmt.hxx" //class DS_mbvec_row_matrix, //class DS_mbvec_row_matrix,
                                       //class DS_abs_matrix
#include "dsmbvec.hxx" //class DS_multi_banded_vec, DS_abs_vec //class DS_multi_banded_vec, DS_abs_vec

// forward declare class ref arguments
//class DS_abs_matrix;
//class DS_mbvec_row_matrix;
//class DS_abs_vec;
//class DS_multi_banded_vec;

class DS_row_reducer : public ACIS_OBJECT                // identifier red

{
// Should not call 
   DS_row_reducer&
   operator=                                       // Assignment Operator
   (const DS_row_reducer&);

  DS_int_block          red_col_perm;
  DS_row_handle_block   red_row;
  DS_reduce_log         red_log;
  double                red_tol;

  DS_mbvec_row_matrix & red_mat;

  int Apply_u_inv( 
    const DS_block_clm_matrix& rhs,
    DS_block_clm_matrix& soln);


  public:

  DS_row_reducer(                    // eff: constructor
   DS_mbvec_row_matrix & M,    // 
   double tolerance=1.e-12)
   :
    red_row(M.Nrows()),
    red_tol(tolerance), 
    red_mat(M),
    red_col_perm(M.Nrows())
  {
   Init_handles();
  }

//  virtual // Not to be derived from
  ~DS_row_reducer() {}     // destructor

  const DS_row_handle_block &
   Row_handles() const
                             {return red_row;}

  double Tol() const {return red_tol;}
  // Set_tol should be used only in op= of owning objects
  void Set_tol(double tol) {red_tol = tol;}

//BMT_TODO: decide the right approach here!
  void Reduce(double tol, int zeros_at_top=0);
  DS_multi_banded_vec & Row(int rowid)
   {
    return *(DS_multi_banded_vec*)&red_mat.Row(red_row[rowid].true_row);
   }
     
  void Swap_rows(int r1, int r2)
    {
     DS_row_handle *const base = red_row;
     red_row.Swap( (base+r1), (base+r2) );
    }

  int                                 // rtn: last row with same min_nz
  Swap_pivot_row(int top_row);        // eff: chooses among rows with same
                                      //      min_nz value for pivot, and
                                      //      swaps with top_row.  Returns
                                      //      last row with same min_nz.
  void Prioritized_reduce(double tol, int zeros_at_top, const DS_int_block& mask, int mask_max);       // eff: 
//
  int                                 // rtn: last row with same min_nz
  Prioritized_swap_pivot_row          // eff: chooses among rows with same
    (int top_row, const DS_int_block& mask, int mask_max );     //      min_nz value for pivot, and
                                      //      swaps with top_row.  Returns
                                      //      last row with same min_nz.
  int Apply_mat_inv( 
    DS_block_clm_matrix& rhs, 
    DS_block_clm_matrix& soln );

  void Reduce_row(
   int reducee, 
   int reducer,
   double tol);

  int                                 // rtn: next unprocessed row to work on
   Migrate_row(                       // eff: Migrates row_id downward until it finds
                                      //      block whose min_nz is >= row_id's.
                                      //      If row is zero'd, swaps it with zero_loc
                                      //      instead.
  int row_id,
  int zero_loc);
  int Sort_rows(int offset);

  int                                 // rtn: number of conflicts
   Apply_log      // eff: repeats row reduction operations on M
    (DS_abs_matrix & M) const;

  int                                 // rtn: number of conflicts
   Apply_log      // eff: repeats row reduction operations on v
    (DS_abs_vec & v) const;

  void Build_Lmat(DS_mbvec_row_matrix & L) const;
  void Build_Lmat_fwd(DS_mbvec_row_matrix & L) const;

  private:
   int First_row_nonzero(int row) {
    if (red_row[row].last == 0 ) {
       return -1;
    }
    return red_col_perm[red_row[row].first];
   }
   void Init_handles();                     // eff: Initializes the min, max values for
                                       //      each row handle then sorts handles on min
   void Zero_event(int zero_row_id);
   void Zero_event(int reducee, int reducer, double mult);
   void Reduce_event(int reducee, int reducer, double mult);
} ; // end class DS_mb_row_reducer

#endif // DS_REDUC_H

