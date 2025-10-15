// $Id: dsblvec.hxx,v 1.3 2000/12/26 18:33:15 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dsblvec.hxx
// author   : John Sloan
// created  : February 5, 1999
// synopsis : A block SPAvector storage implementation scheme is optimized
//            for dense arrays.  This class is just a SPAvector wrapper for
//            the DS_double_block block memory class.
//
// contains : Class DS_block_vector

#ifndef DS_BLVEC_H
#define DS_BLVEC_H

// nested includes
#include "dsblock.hxx"     // DS_dbl_block     // DS_dbl_block
#include "dsabvec.hxx"     // DS_abs_vec     // DS_abs_vec

class DS_block_vec : public DS_abs_vec // identifier = blv
{
  private:
   DS_dbl_block  blv_data   ;     // data array - knows size and capacity as well
  public:
  // constructor, copy, destructor, clear
  DS_block_vec                    // eff: default constructor
   (int size=0,                   // in : initial SPAvector size
    int blmult=2)                 // in : block growth multiplier
    :
     DS_abs_vec(ds_blv, size),    // DS_abs_vec responsible for typeid, size
     blv_data(size,blmult)        // allocs size elements of data
    {}

   
  DS_block_vec                    // copy constructor
   (const DS_block_vec &src)    
    : DS_abs_vec(ds_blv, src.Size()),
      blv_data(src.blv_data)
    {} // end copy constructor

  DS_block_vec              // construct from a DS_abs_vec
   (const DS_abs_vec & src)
    :
      DS_abs_vec( ds_blv, src.Size())
    {src.Assign_into(*this);}

  DS_block_vec &operator=   // assignment operator
   (const DS_block_vec &src);

  DS_block_vec &operator+=   // += operator
   (const DS_block_vec &src);

  DS_block_vec &operator-=   // -= operator
   (const DS_block_vec &src);

  DS_block_vec &Overwrite   // += operator
   (const DS_block_vec &src,
          double mult);

  DS_block_vec &Pluseq   // += operator
   (const DS_block_vec &src,
          double mult);

  DS_block_vec &Minuseq   // -= operator
   (const DS_block_vec &src,
          double mult);
                          
  virtual DS_abs_vec*              // Virtual Make_copy (public function)
   Make_copy() const               // copies all members in the linked list
                                   {return new DS_block_vec(*this);}

  virtual
  ~DS_block_vec() {}        // destructor

// Virtual function implementations
  // resizing routine - responsible for calling Set_size()
   virtual
    void Resize(                   // eff: grows or shrinks array size
                int new_size,      // in : new size
                double val=0.0);   // in : initializes new elements to val

  virtual
   void Reserve(                   // eff: grows array
                int new_size);     // in : new size
                                   //      does not initialize new elements

  virtual void
   Wipe()
                                {blv_data.Wipe();}
  public:

  virtual const double
   operator[]               // eff: array element access interface
   (int elem) const         // in : the element
                            {return blv_data[elem];}

  double &
   operator[]               // eff: array element access interface
   (int elem)               // in : the element
                            {return blv_data[elem];}

  virtual DS_abs_vec &      // out: allow chaining
   Set_elem                 // eff: overwrites the value of one element
   (int elem,               // in : the element
   double value)            // in : vec[elem] = value
                            {blv_data[elem]  = value; return *this;}

  virtual DS_abs_vec &      // out: allow chaining
   Pluseq_elem              // eff: adds to the value to one element
   (int elem,               // the element
    double value)          // vec[elem] += value
                            {blv_data[elem] += value; return *this;}

  virtual DS_abs_vec &      // out: allow chaining
   Timeseq_elem             // eff: multiplies one element by the value
   (int elem,               // in : the element
    double value)          // in : vec[elem] *= value
                            {blv_data[elem] *= value; return *this;}

  virtual DS_abs_vec &      // out: allow chaining
   Zero()                  // eff: zeros all elements, can be more optimal
                            // in : than Set(0.0), but routine could test for
                            // in : zero so maybe not necessary.
                            {(*this)=0; return *this;}

  virtual DS_abs_vec &      // out: allow chaining
   Negate();                // eff: negates all non-zero elements

  virtual DS_abs_vec &
   operator=               // eff: adds to the value of all elements
   (double value);          // in : vec[elem] = value for all elem

  virtual DS_abs_vec &
   operator+=               // eff: adds to the value of all elements
   (double value);          // in : vec[elem] += value for all elem

  virtual DS_abs_vec &      // out: allow chaining
   operator*=               // eff: multiplies the value of all elements
   (double value);          // in : vec[elem] *= value for all elem

  virtual DS_abs_vec &      // out: allow chaining
   operator=                // eff: overwrites whole SPAvector
                            //      but maintains actual type of *this
                            //      use Make_copy to clone v
   (const DS_abs_vec & v)   // in : (*this)[i] = v[i] for all i elements of v
                            //      forces (*this).Size() == v.Size()
                            // note: This is a multi-method
                            //       use double dispatch to implement for now
                            //       See Stroustup "Design + Evolution ..." 
                            //       p.299 for discussion/alternatives using 
                            //       Type_id()
                            { v.Assign_into(*this); return *this; }

  virtual DS_abs_vec &      // out: allow chaining
   operator+=               // eff: increments by SPAvector
    (const DS_abs_vec & v)  // in : (*this)[i] += v[i] for all i elements of v
                            //      requires (*this).Size() == v.Size()
                            { v.Assign_into(*this,PLUSEQ); return *this; }

  virtual DS_abs_vec &operator-=   // -= operator
   (const DS_abs_vec &v)
                            { v.Assign_into(*this,MINUSEQ); return *this; }

  virtual DS_abs_vec &Overwrite   // = operator
   (const DS_abs_vec &v,
          double mult)
                            { v.Assign_into(*this,mult,OVERWRITE); return *this; }

  virtual DS_abs_vec &Pluseq   // += operator
   (const DS_abs_vec &v,
          double mult)
                            { v.Assign_into(*this,mult,PLUSEQ); return *this; }

  virtual DS_abs_vec &Minuseq   // -= operator
   (const DS_abs_vec &v,
          double mult)
                            { v.Assign_into(*this,mult,MINUSEQ); return *this; }

  virtual void      // out: allow chaining
   Assign_from                // eff: overwrites whole SPAvector
                            //      but maintains actual type of *this
                            //      use Make_copy to clone v
   (const DS_abs_vec & v,
          EQ_FLAG flag)   // in : (*this)[i] ?= v[i] for all i elements of v
                            //      forces (*this).Size() == v.Size()
                            // note: This is a multi-method
                            //       use double dispatch to implement for now
                            //       See Stroustup "Design + Evolution ..." 
                            //       p.299 for discussion/alternatives using 
                            //       Type_id()
                            { v.Assign_into(*this, flag);}

  virtual void      // out: allow chaining
   Assign_from                // eff: overwrites whole SPAvector
                            //      but maintains actual type of *this
                            //      use Make_copy to clone v
   (const DS_abs_vec & v,
          double mult,
          EQ_FLAG flag)   // in : (*this)[i] = v[i] for all i elements of v
                            //      forces (*this).Size() == v.Size()
                            // note: This is a multi-method
                            //       use double dispatch to implement for now
                            //       See Stroustup "Design + Evolution ..." 
                            //       p.299 for discussion/alternatives using 
                            //       Type_id()
                            { v.Assign_into(*this, mult, flag);}

   virtual double                      // out: result = (*this).v
    Dotprod(                           // eff: evaluates dot product
            const DS_abs_vec & v) const// in : with abstract SPAvector v
                                       {return v.Dotprod(*this);}

   double                                  // out: result = (*this).v
    Dotprod(                               // eff: evaluates dot product
            const DS_block_vec & v) // in : with concrete SPAvector v
                                     const;

   double                                  // out: result = (*this).v
    Dotprod(                               // eff: evaluates dot product
            const DS_multi_banded_vec & v) // in : with concrete SPAvector v
                                     const;

   virtual void
   Assign_into                   // eff: overwrites argument SPAvector
    (DS_block_vec & v,
     EQ_FLAG flag=OVERWRITE)           // out: writes to multi-banded SPAvector
      const;

   virtual void
   Assign_into                   // eff: overwrites argument SPAvector
    (DS_block_vec & v,
     double mult,
     EQ_FLAG flag=OVERWRITE)           // out: writes to multi-banded SPAvector
      const;

#ifdef DS_MULTI_BANDED_VEC_ON
  DS_block_vec &operator=   // conversion assignment operator
   (const DS_multi_banded_vec &src);

  DS_block_vec &operator+=   // conversion += operator
   (const DS_multi_banded_vec &src);

  DS_block_vec &operator-=   // conversion -= operator
   (const DS_multi_banded_vec &src);

  DS_block_vec &Overwrite   // += operator
   (const DS_multi_banded_vec &src,
          double mult);

  DS_block_vec &Pluseq   // += operator
   (const DS_multi_banded_vec &src,
          double mult);

  DS_block_vec &Minuseq   // -= operator
   (const DS_multi_banded_vec &src,
          double mult);
/*
  virtual void
   Assign_into                   // eff: overwrites argument SPAvector
    (DS_multi_banded_vec & v)    // out: writes to multi-banded SPAvector
      const;
                                 //{v = *this;}
*/
#endif // DS_MULTI_BANDED_VEC_ON

} ; // end class DS_block_vec


#endif //DS_BLVEC_H

