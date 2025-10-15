/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dsbridge.hxx
// created  : September 2, 1999
// author   : bmt
// contains : DS_bridge  - solution container class
// synopsis : Virtual base class for eqns linear equations.
#ifndef DS_bridge_H
#define DS_bridge_H
#include <string.h>                    // NULL
#include "dsbcmat.hxx"  
#include "dsblock.hxx"  
//
  class DS_lueqns;
  class DS_bridge : public ACIS_OBJECT
{
   protected:
     int brg_nrows;
     int brg_nclms;
     int brg_i;
     DS_block_clm_matrix brg_old_x;
     DS_block_clm_matrix brg_first_x;
     DS_dbl_block brg_x_block;
     DS_dbl_block brg_old_x_block;
   public:
     DS_block_clm_matrix brg_x;
     void Init_bridge(int nrows, int ncols );
     int Nrows() const;
     int Nclms() const;
     void Set_x_elem( int row, int col, double val );
     double Calc_x_old_x_dist2();
     const double* X_ptr();
     const double* Old_x_ptr();
     friend class DS_lueqns;
     double X(int irow, int icol) const;
     double First_x(int irow, int icol) const;
     void Copy_x_to_old_x();
     void Copy_x_to_first_x();
     void Set_x_to_dif(const double* vec0, const double* vec1, int nrows, int ncols);
     void Zero_x() {brg_x.Zero();}
//     void Set_x(const DS_block_clm_matrix* mat ) ;
// default constructor
   DS_bridge() {
     brg_nrows = 0;
     brg_nclms = 0;
     brg_i = 0;
   }
// default destructor
   ~DS_bridge() {
    }


};

#endif //DS_bridge_H
