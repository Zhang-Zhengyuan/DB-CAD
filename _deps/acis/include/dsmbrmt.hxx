// $Id: dsmbrmt.hxx,v 1.4 2000/12/26 18:33:22 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dsmbrmt.hxx
// created  : February 11, 1999
// author   : John Sloan
// contains : DS_mbvec_row_matrix  - concrete block (column-ordered) SPAmatrix class
// synopsis : Represents a linear algebra SPAmatrix.
//            Interface is tuned to avoid excessive heap allocations - 
//            only inplace arithmetic operations allowed.
//            Anticipates sparse implementation derived classes, so direct
//            access to elements by clients is not allowed.
//


#ifndef DS_MBRMT_H
#define DS_MBRMT_H

#include "dsabmat.hxx"  // class DS_clm_matrix  // class DS_clm_matrix
#include "dsmbvbl.hxx"  // class DS_blvec_block  // class DS_blvec_block
#include "dsmbvec.hxx"  // class DS_blvec_block  // class DS_blvec_block

class DS_mbvec_row_matrix : public DS_row_matrix    // identifier bcm
{
  protected:
   DS_mbvec_block mrm_row;
//For future optimization with single contiguous array
//   DS_dbl_block         bcm_data;
//   mutable DS_dbl_alias_block *   mrm_row;

  public:
   // constructor, copy, Make_copy, destructor
 
  DS_mbvec_row_matrix                    // eff: "default" constructor
   (int    nrows=0,             // in : initial size
    int    nclms=0)
    :
     DS_row_matrix(ds_mrm,0,0)
    {Reserve(nrows,nclms);}
                          
  DS_mbvec_row_matrix                    // eff: copy constructor
   (const DS_mbvec_row_matrix & mat)
   : 
    DS_row_matrix(mat),
    mrm_row(mat.mrm_row)
   {
    //For future optimization with single contiguous array
    //Recalc_clm_ptrs();
   }
                          
  virtual DS_abs_matrix*          // ret: (upcast) pointer to concrete type
   Make_copy() const;              // eff: clones derived class
  
  virtual ~DS_mbvec_row_matrix() {}     // destructor

  virtual void 
   Resize                      // eff: grows (or shrinks) array
    (int nrows,             // in : new size
     int nclms,             // in : new size
     double val=0.0);        // in : new elements are initialized to val

  virtual void
   Reserve                      // eff: grows (or shrinks) array
    (int nrows,
     int nclms);           // in : new size
                                //      does not initialize elements
  virtual void
   Wipe()
                                 {
                                  mrm_row.Wipe();
                                  Set_size(0,0);
                                 }

// Array element arithmetic manipulation routines
// TODO:??? (JHS) Should these routines be returning void instead?

// single element changes
// TODO:??? (JHS) Could write Iterator/Element class (returned by op[])
// instead, which would hold a pointer reference to element then write
// single-element arithmetic routines on that.  Probably not worth it though
/*  Can't return reference to element since this is a sparse representation
  double &
  operator()
  (int row,
   int clm)       // in : the element index
                            {return mrm_row[row][clm];}
*/
  void Remove_zeros(double tol=0.0);    // Remove values less than tol from the SPAmatrix.
                                        // Update all data structures.

  virtual const double      // rtn: element value
   operator()               // eff: array element query (rvalue)
   (int row,
    int clm) const      // in : the element index
                            {return mrm_row[row][clm];}

  virtual DS_abs_matrix &      // ret: allow chaining
   Set_elem                 // eff: overwrites the value of one element
   (int row,
    int clm,               // in : the element index
   double value)         // in : vec[elem] = value
                             {mrm_row[row].Set_elem(clm,value); return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Pluseq_elem              // eff: adds to the value of one element
   (int row,
    int clm,               // in : the element index
    double value)        // in : vec[elem] += value
                             {mrm_row[row].Pluseq_elem(clm,value); return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Timeseq_elem             // eff: multiplies one element by the value
   (int row,
    int clm,               // in : the element index
    double value)        // in : vec[elem] *= value
                             {mrm_row[row].Timeseq_elem(clm,value); return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Set_clm                 // eff: overwrites the value of one element
   (int clm,               // in : the element index
    double value);         // in : vec[elem] = value

  virtual DS_abs_matrix &      // ret: allow chaining
   Pluseq_clm              // eff: adds to the value of one element
   (int clm,               // in : the element index
    double value);        // in : Row(clm) += value

  virtual DS_abs_matrix &      // ret: allow chaining
   Timeseq_clm             // eff: multiplies one element by the value
   (int clm,               // in : the element index
    double value);        // in : vec[elem] *= value

  virtual DS_abs_matrix &      // ret: allow chaining
   Set_clm                 // eff: overwrites the value of one element
   (int clm,               // in : the element index
    const DS_abs_vec & vec);         // in : vec[elem] = value

  virtual DS_abs_matrix &      // ret: allow chaining
   Set_clm                 // eff: overwrites the value of one element
   (int clm,               // in : the element index
    const DS_abs_vec & vec,
    double mult);         // in : vec[elem] = value

  virtual DS_abs_matrix &      // ret: allow chaining
   Pluseq_clm              // eff: adds to the value of one element
   (int clm,               // in : the element index
    const DS_abs_vec & vec);        // in : clm(clm) += vec*mult

  virtual DS_abs_matrix &      // ret: allow chaining
   Pluseq_clm              // eff: adds to the value of one element
   (int clm,               // in : the element index
    const DS_abs_vec & vec,
    double mult);        // in : clm(clm) += vec*mult

  // column operations

  virtual 
   const DS_abs_vec &
    Row(int clm) const
                            {return mrm_row[clm];}

  virtual 
   DS_abs_vec &
    Row(int clm)
                            {return mrm_row[clm];}

// full SPAmatrix changes
  virtual DS_abs_matrix &      // ret: allow chaining
   Zero();                // eff: zeros all elements, can be more optimal
                            //      than Set(0.0), but Set could test for
                            //      zero so maybe not necessary.

  virtual DS_abs_matrix &      // ret: allow chaining
   Negate();              // eff: negates all non-zero elements

  virtual void                    // eff: clone TRANSPOSE
  Make_transpose(DS_mbvec_row_matrix& trans) const;

} ; // end class DS_mbvec_row_matrix
#endif // DS_MBRMT_H

