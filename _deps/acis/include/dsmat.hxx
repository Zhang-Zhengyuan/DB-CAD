/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dsmat.hxx
// created  : July 15, 1996
// author   : GW Celniker
// synopsis : Implements a general rectangular SPAmatrix of doubles.
//            DS_row_mat stores the SPAmatrix as a sequence of rows.
//            SPAmatrix sized [MxN]
                  // A[ 00  01  02  ...  n-1]
                  //  [ n   n+1 n+2 ... 2n-1]
                  //  [     .               ]
                  //  [         .           ]
                  //  [             .       ]
                  //  [                m*n-1]
//            DS_col_mat stores the SPAmatrix as a sequence of columns.
//            SPAmatrix sized [MxN]
                  // A[ 00 m    . . .       ]
                  //  [ 01 m+1              ]
                  //  [ 03 m+2              ]
                  //  [ . . .               ]
                  //  [ m-1 2m-1 . . . m*n-1]
//            DS_diag_mat stores a symmetric SPAmatrix in row-diagonal
//            format
                  // A[ 00 10 20 ... n0]
                  //  [    01 11 ...   ]
                  //  [      .         ]
                  //  [         .      ]
                  //  [           .    ]
                  //  [              0 ]
//            The elements of the SPAmatrix are allocated in one 
//            call to malloc and a 2nd array of pointers is allocated
//            to support SPAmatrix like addressing.
// [GC Note]: It will be easy to check performance with acesss
//            through mat or access through vec and pick the best 
//            for final release.
//
// contains : Class DS_row_mat  (matrices stored one row at a time)
//            Class DS_col_mat  (matrices stored one col at a time)
//            Class DS_diag_mat (sym matrices stored in row-diag form)

#ifndef DS_MAT_H
#define DS_MAT_H

// nested includes
#include "dsblmem.hxx"     // DS_copy_double_block(), ...     // DS_copy_double_block(), ...
#include "dsstdef.hxx"     // DS_assert()     // DS_assert()

class DS_row_mat : public ACIS_OBJECT          // identifier = row_
{
  private:
   int      row_m ;        // [array_size] SPAmatrix row count
   int      row_n ;        // [array_size] SPAmatrix col count
   double **row_mat ;      // array [row_m] of cached row pointers
   double  *row_vec ;      // array [row_m*row_n] of elements

  public:
   // constructor, copy, destructor, Size_arrays, clear
   DS_row_mat              // eff: default constructor
    (int m=0,              // in : desired SPAmatrix row_count
     int n=0)              // in : desired SPAmatrix col_count
                           { row_m   = 0 ;
                             row_n   = 0 ;
                             row_mat = NULL ;
                             
                             Size_arrays(m,n) ; 
                           }

   DS_row_mat              // copy constructor
    (DS_row_mat &src)      { row_m   = 0 ;
                             row_n   = 0 ; 
                             row_mat = NULL ;
                             
                             Size_arrays(src.row_m,src.row_n) ;
                             DS_copy_double_block
                                  (row_vec,        // tgt
                                   src.row_vec,    // src 
                                   Vec_size()) ;   // count
                           }  // end copy constructor

  DS_row_mat &operator=    // assignment operator
   (DS_row_mat &src)       {  // no work condition
                              if(&src != this) {
                             Size_arrays(src.row_m,src.row_n) ;
                             DS_copy_double_block
                                  (row_vec,       // tgt
                                   src.row_vec,   // src 
                                   Vec_size()) ;  // count
                             }
                             return *this ;
                           }

  virtual
  ~DS_row_mat()            // destructor
                           { Size_arrays() ; }

   void                    // Size_arrays()
    Size_arrays            // eff: manage internal array memory
     (int m=0,             // in : desired SPAmatrix row count
      int n=0);            // in : desired SPAmatrix col count

   void                    // Clear()
    Clear()                // set all row_vec values to 0
                           {  DS_clear_double_block
                                   (row_vec,      // tgt
                                    Vec_size()) ; // size
                           } // end clear()
     
  // size and data access
  int Row_count()          { return row_m ; }
  int Col_count()          { return row_n ; } 
  double **Mat()           { return row_mat ; }
  double *Vec()            { return row_vec ; }
  double &Elem     
   (int i, int j)          { DS_assert(   i >= 0 && i < row_m) ;
                             DS_assert(   j >= 0 && j < row_n) ;
                             return row_mat[i][j] ; }

  int Blk_size             // eff: rtn malloc size of internal arrays
    (int m,                // in : SPAmatrix row count
     int n)                // in : SPAmatrix col count
                           { return ( Mat_size(m) + Vec_size(m,n)) ;}

  int Mat_size             // ret: a double count bigger than row_mat
   (int m)                 // in : SPAmatrix row count 
                           { return (int)ceil
                                   (  m
                                    * (double)sizeof(double *)
                                    / (double)sizeof(double) ) ; 
                           }

  int Vec_size             // eff: rtn double size of vec block
   (int m,                 // in : SPAmatrix row count
    int n)                 // in : SPAmatrix col count
                           { return (m*n) ; }

  int Vec_size             // eff: rtn current size of vec block
   ()                      { return (row_m * row_n) ; }

} ; // end class DS_row_mat

//==========================================================
// Class DS_col_mat for column stored matrices
//==========================================================

class DS_col_mat : public ACIS_OBJECT           // identifier = col_
{
  private:
   int      col_m ;        // [array_size] SPAmatrix row count
   int      col_n ;        // [array_size] SPAmatrix col count
   double **col_mat ;      // array [col_m] of cached row pointers
   double  *col_vec ;      // array [col_m*col_n] of elements

  public:
   // constructor, copy, destructor, Size_arrays, clear
   DS_col_mat              // eff: default constructor
    (int m=0,              // in : desired SPAmatrix col_count
     int n=0)              // in : desired SPAmatrix col_count
                           { col_m   = 0 ;
                             col_n   = 0 ;
                             col_mat = NULL ;

                             Size_arrays(m,n) ; 
                           }

   DS_col_mat              // copy constructor
    (DS_col_mat &src)      { col_m = 0 ;
                             col_n = 0 ;
                             col_mat = NULL ;
 
                             Size_arrays(src.col_m,src.col_n) ;
                             DS_copy_double_block
                                  (col_vec,        // tgt
                                   src.col_vec,    // src 
                                   Vec_size()) ;   // count
                           }  // end copy constructor

  DS_col_mat &operator=    // assignment operator
   (DS_col_mat &src)       {  // no work condition
                             if(&src != this) {
                             Size_arrays(src.col_m,src.col_n) ;
                             DS_copy_double_block
                                  (col_vec,       // tgt
                                   src.col_vec,   // src 
                                   Vec_size()) ;  // count
                             }
                             return *this ;
                           }

  virtual
  ~DS_col_mat()            // destructor
                           { Size_arrays() ; }

   void                    // Size_arrays()
    Size_arrays            // eff: manage internal array memory
     (int m=0,             // in : desired SPAmatrix row count
      int n=0);            // in : desired SPAmatrix col count

   void                    // Clear()
    Clear()                // set all col_vec values to 0
                           {  DS_clear_double_block
                                   (col_vec,      // tgt
                                    Vec_size()) ; // size
                           } // end clear()
     
  // size and data access
  int Row_count()          { return col_m ; }
  int Col_count()          { return col_n ; } 
  double **Mat()           { return col_mat ; }
  double *Vec()            { return col_vec ; }
  double &Elem     
   (int i, int j)          { DS_assert(   i >= 0 && i < col_m) ;
                             DS_assert(   j >= 0 && j < col_n) ;
                             return col_mat[j][i] ; }

  int Blk_size             // eff: rtn malloc size of internal arrays
    (int m,                // in : SPAmatrix row count
     int n)                // in : SPAmatrix col count
                           { return ( Mat_size(m) + Vec_size(m,n)) ;}

  int Mat_size             // ret: a double count bigger than col_mat
   (int n)                 // in : SPAmatrix col count 
                           { return (int)ceil
                                   (  n
                                    * (double)sizeof(double *)
                                    / (double)sizeof(double) ) ; 
                           }

  int Vec_size             // eff: rtn double size of vec block
   (int m,                 // in : SPAmatrix row count
    int n)                 // in : SPAmatrix col count
                           { return (m*n) ; }

  int Vec_size             // eff: rtn current size of vec block
   ()                      { return (col_m * col_n) ; }

} ; // end class DS_col_mat

class DS_diag_mat : public ACIS_OBJECT         // identifier = dia_
{
  private:
   int      dia_n ;        // (prob-size) array is an [nxn] SPAmatrix
   double **dia_mat ;      // array [n] of cached row pointers
   double  *dia_vec ;      // array [(n+1)*n/2] of elements

  public:
   // constructor, copy, destructor, Size_arrays, clear
   DS_diag_mat(int n = 0)  // default constructor
                           {
                             dia_n   = 0 ; 
                             dia_mat = NULL ;
                             dia_vec = NULL ;
                             Size_arrays(n) ; 
                           }

   DS_diag_mat             // copy constructor
    (DS_diag_mat &src)
                           { dia_n   = 0 ;
                             dia_mat = NULL ;
                             dia_vec = NULL ;
                             Size_arrays(src.dia_n) ;
                             DS_copy_double_block
                                  (dia_vec,        // tgt
                                   src.dia_vec,    // src 
                                   Size()) ;       // count
                           }  // end copy constructor

  DS_diag_mat &operator=   // assignment operator
   (DS_diag_mat &src)      { // no work condition
                             if(&src != this) {
                             Size_arrays(src.dia_n) ;
                             DS_copy_double_block
                                  (dia_vec,       // tgt
                                   src.dia_vec,   // src 
                                   Size()) ;      // count
                             }
                             return *this ;
                           }
   
   virtual
  ~DS_diag_mat()           // destructor
                           {Size_arrays() ; }

   void                    // Size_arrays()
    Size_arrays(int n=0) ; // manage mat and vec memory

   void                    // Clear()
    Clear()                // set all dia_vec values to 0
                           {  DS_clear_double_block
                                   (dia_vec,    // tgt
                                    Size()) ;   // size
                           } // end clear()
     
  // size and data access 
   double *Vec()           { return dia_vec ; }
   double &Elem
    (int i, int j)         { DS_assert(   j >= i) ;
                             DS_assert(   i >= 0 && i < dia_n
                                       && j >= 0 && j < dia_n) ;
                             return dia_mat[j-i][i] ; }
   int   Count()           { return dia_n ; }
   int   Size(int n)       { return ((n+1)*n)/2 ; }
   int   Size()            { return ((dia_n+1)*dia_n)/2 ; }

} ; // end class DS_diag_mat


#endif //DS_MAT_H
