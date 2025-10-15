// $Id: dsbcmat.hxx,v 1.3 2000/12/26 18:33:14 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dsbcmat.hxx
// created  : February 11, 1999
// author   : John Sloan
// contains : DS_block_clm_matrix  - concrete block (column-ordered) SPAmatrix class
// synopsis : Represents a linear algebra SPAmatrix.
//            Interface is tuned to avoid excessive heap allocations - 
//            only inplace arithmetic operations allowed.
//            Anticipates sparse implementation derived classes, so direct
//            access to elements by clients is not allowed.
//


#ifndef DS_BCMAT_H
#define DS_BCMAT_H

#include "dsabmat.hxx"  // class DS_clm_matrix  // class DS_clm_matrix
#include "dsblvbl.hxx"  // class DS_blvec_block  // class DS_blvec_block
#include "dsblvec.hxx"  // class DS_blvec_block  // class DS_blvec_block

//For future optimization with single contiguous array
//#include "dshusk/dskernel/dsblock.hxx"  // class DS_dbl_block

class DS_dbl_alias_block;

class DS_block_clm_matrix : public DS_clm_matrix    // identifier bcm
{
  protected:
   DS_blvec_block bcm_clm;
//For future optimization with single contiguous array
//   DS_dbl_block         bcm_data;
//   mutable DS_dbl_alias_block *   bcm_clm;

  public:
   // constructor, copy, Make_copy, destructor
 
  DS_block_clm_matrix                    // eff: "default" constructor
   (int    nrows=0,             // in : initial size
    int    nclms=0)
    :
     DS_clm_matrix(ds_bcm,0,0)
    {Reserve(nrows,nclms);}
                          
  DS_block_clm_matrix                    // eff: copy constructor
   (const DS_block_clm_matrix & mat)
   : 
    DS_clm_matrix(mat),
    bcm_clm(mat.bcm_clm)
   {
    //For future optimization with single contiguous array
    //Recalc_clm_ptrs();
   }
                          
  virtual DS_abs_matrix*          // ret: (upcast) pointer to concrete type
   Make_copy() const;              // eff: clones derived class
  
  virtual ~DS_block_clm_matrix() {}     // destructor

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
                                  bcm_clm.Wipe();
                                  Set_size(0,0);
                                 }

// Array element arithmetic manipulation routines
// TODO:??? (JHS) Should these routines be returning void instead?

// single element changes
// TODO:??? (JHS) Could write Iterator/Element class (returned by op[])
// instead, which would hold a pointer reference to element then write
// single-element arithmetic routines on that.  Probably not worth it though
  double &
  operator()
  (int row,
   int clm)       // in : the element index
                            {return bcm_clm[clm][row];}

  virtual const double      // rtn: element value
   operator()               // eff: array element query (rvalue)
   (int row,
    int clm) const      // in : the element index
                            {return bcm_clm[clm][row];}

  virtual DS_abs_matrix &      // ret: allow chaining
   Set_elem                 // eff: overwrites the value of one element
   (int row,
    int clm,               // in : the element index
   double value)         // in : vec[elem] = value
                             {(*this)(row,clm) = value; return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Pluseq_elem              // eff: adds to the value of one element
   (int row,
    int clm,               // in : the element index
    double value)        // in : vec[elem] += value
                             {(*this)(row,clm) += value; return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Timeseq_elem             // eff: multiplies one element by the value
   (int row,
    int clm,               // in : the element index
    double value)        // in : vec[elem] *= value
                             {(*this)(row,clm) *= value; return *this;}

  virtual DS_abs_matrix &      // ret: allow chaining
   Set_row                 // eff: overwrites the value of one element
   (int row,               // in : the element index
    double value);         // in : vec[elem] = value

  virtual DS_abs_matrix &      // ret: allow chaining
   Pluseq_row              // eff: adds to the value of one element
   (int row,               // in : the element index
    double value);        // in : Row(row) += value

  virtual DS_abs_matrix &      // ret: allow chaining
   Timeseq_row             // eff: multiplies one element by the value
   (int row,               // in : the element index
    double value);        // in : vec[elem] *= value

  virtual DS_abs_matrix &      // ret: allow chaining
   Set_row                 // eff: overwrites the value of one element
   (int row,               // in : the element index
    const DS_abs_vec & vec);         // in : vec[elem] = value

  virtual DS_abs_matrix &      // ret: allow chaining
   Set_row                 // eff: overwrites the value of one element
   (int row,               // in : the element index
    const DS_abs_vec & vec,
    double mult);         // in : vec[elem] = value

  virtual DS_abs_matrix &      // ret: allow chaining
   Pluseq_row              // eff: adds to the value of one element
   (int row,               // in : the element index
    const DS_abs_vec & vec);        // in : Row(row) += vec*mult

  virtual DS_abs_matrix &      // ret: allow chaining
   Pluseq_row              // eff: adds to the value of one element
   (int row,               // in : the element index
    const DS_abs_vec & vec,
    double mult);        // in : Row(row) += vec*mult

  // column operations

  virtual 
   const DS_abs_vec &
    Clm(int clm) const
                            {return bcm_clm[clm];}

  virtual 
   DS_abs_vec &
    Clm(int clm)
                            {return bcm_clm[clm];}

// full SPAmatrix changes
  virtual DS_abs_matrix &      // ret: allow chaining
   Zero();                // eff: zeros all elements, can be more optimal
                            //      than Set(0.0), but Set could test for
                            //      zero so maybe not necessary.

  virtual DS_abs_matrix &      // ret: allow chaining
   Negate();              // eff: negates all non-zero elements
/*
  virtual DS_abs_matrix &      // ret: allow chaining
   operator=                // eff: overwrites whole SPAvector
                            //      but maintains actual type of *this
                            //      use Make_copy to clone v
   (const DS_abs_matrix & m);// in : (*this)[i] = m[i] for all i elements of m
                            //      note that this is a multi-method
                            //      use double dispatch to implement for now
                            //      See Stroustup "Design + Evolution ..." p.299
                            //      for discussion/alternatives using Type_id()

  virtual DS_abs_matrix &        // out: allow chaining
   Pluseq                 // eff: increments by SPAvector
    (const DS_abs_matrix & m,
     double mult=1); // in : (*this)[i] = mult*m[i] for all i elements of m
                              //      requires (*this).Size() == v.Size()

DS_abs_matrix &        // out: allow chaining
   Minuseq                 // eff: increments by SPAvector
    (const DS_abs_matrix & m,
     double mult=1)
                                    {return (*this).Pluseq(m,-mult);}

   virtual void                  
    Lmult                          
     (const DS_abs_vec & clm_vec,
      const DS_abs_vec & clm_res) const;  

   virtual void                     
    Rmult                          
     (const DS_abs_vec & row_vec,
            DS_abs_vec & row_res) const; 

   virtual void                    
    Lmult                             
     (const DS_abs_matrix & mat,
            DS_abs_matrix & res) const;                            

  virtual DS_abs_matrix &      // ret: allow chaining
   operator+=               // eff: adds to the value of all elements
   (double value);        //      vec[elem] += value for all elem

  virtual DS_abs_matrix &      // ret: allow chaining
   operator*=               // eff: multiplies the value of all elements
   (double value);        // in : vec[elem] *= value for all elem
*/

//For future optimization with single contiguous array
//  private:
//   void Recalc_clm_ptrs() const;
} ; // end class DS_block_clm_matrix

#endif // DS_BCMAT_H

