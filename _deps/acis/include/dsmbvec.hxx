// $Id: dsmbvec.hxx,v 1.8 2000/12/26 18:33:22 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dsmbvec.hxx
// author   : John Sloan
// created  : January 11, 1999
// synopsis : A multi-banded SPAvector storage implementation scheme is optimized
//            for arrays with "bands" of contiguous non-zero elements,
//            separated by large gaps of zero elements.  Three numbers are
//            stored for each band: the index of the first non-zero
//            element (Min), the index of the first zero element past the
//            band (Max), and the location within the array of doubles of the
//            first non-zero element (Offset).  All ranges are zero based, and
//            in this implementation Offset(Nbands()) is used to return the
//            size of the non-zero element array. Note that single-element
//            operations are inefficient in this representation, requiring
//            O(Nbands/2) operations.  Also note that changing the zero status
//            of any set of elements requires O(Num_non_zero()/2) operations.
//
// contains : Class DS_multi_banded_vector

#ifndef DS_MBVEC_H
#define DS_MBVEC_H

// nested includes
#include "dsblock.hxx"     // DS_dbl_block, DS_int_block     // DS_dbl_block, DS_int_block
#include "dsabvec.hxx"     // DS_abs_vec, DS_*_VEC_ON macros     // DS_abs_vec, DS_*_VEC_ON macros

class DS_multi_banded_vec : public DS_abs_vec // identifier = mbv
{
  private:
  
   int           mbv_nbands ;      // number of bands in SPAvector
   DS_int_block  mbv_minmax  ;     // holds (offset,min,max) info for bands,
                                   // needs 3*nbands+1
   DS_dbl_block  mbv_data   ;      // non-zero element array
                                   // needs Offset(Nbands())

  public:
  // constructor, copy, destructor, clear
  DS_multi_banded_vec             // eff: default constructor
   (int size=0,                   // in : initial SPAvector size
    int blmult=2);                // in : block growth multiplier
   
  DS_multi_banded_vec             // copy constructor
   (const DS_multi_banded_vec &src)    
    : DS_abs_vec( ds_mbv, src.Size()),
      mbv_nbands(src.Nbands()),
      mbv_minmax(src.mbv_minmax),
      mbv_data(src.mbv_data)
    {} // end copy constructor

  DS_multi_banded_vec              // construct from a DS_abs_vec
   (const DS_abs_vec & src)
    :
      DS_abs_vec( ds_mbv, src.Size())
    {src.Assign_into(*this);}

  DS_multi_banded_vec &operator=   // assignment operator
   (const DS_multi_banded_vec &src);
                          
  virtual DS_abs_vec*              // Virtual Make_copy (public function)
   Make_copy() const               // copies all members in the linked list
                                   {return ACIS_NEW DS_multi_banded_vec(*this);}

  virtual
  ~DS_multi_banded_vec() {}        // destructor

// Virtual function implementations
  // resizing routine - responsible for calling Set_size()
   virtual
    void Resize(                   // eff: grows or shrinks array size
                int new_size,      // in : new size
                double val=0.0);   // in : initializes new elements to val
                                   // note: allocated memory can only grow

  virtual
   void Reserve(                   // eff: grows array
                int new_size);     // in : new size
                                   //      does not initialize new elements

  virtual void
   Wipe();                    // eff: sets size to zero and deallocates
                                //      all reserved data space
  public:

  virtual const double
   operator[]               // eff: array element access interface
   (int elem) const;        // in : the element

  virtual DS_abs_vec &      // out: allow chaining
   Set_elem                 // eff: overwrites the value of one element
   (int elem,               // in : the element
   double value);           // in : vec[elem] = value

  virtual DS_abs_vec &      // out: allow chaining
   Pluseq_elem              // eff: adds to the value to one element
   (int elem,               // the element
    double value);          // vec[elem] += value

  virtual DS_abs_vec &      // out: allow chaining
   Timeseq_elem             // eff: multiplies one element by the value
   (int elem,               // in : the element
    double value);          // in : vec[elem] *= value

  virtual int               // ret: the element index
   Largest_elem() const;    // eff: finds element with largest fabs

  virtual int               // ret: the element index
   Largest_elem(const DS_int_block& mask) const;    
                            // eff: finds element with largest fabs with non-zero mask entry

  virtual int               // ret: 0 if flase, 1 otherwise
    Or(const DS_int_block& mask) const;
                            // eff: logical or on entries


  virtual DS_abs_vec &      // out: allow chaining
   Zero();                  // eff: zeros all elements, can be more optimal
                            // in : than Set(0.0), but routine could test for
                            // in : zero so maybe not necessary.

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

  virtual DS_abs_vec &      // ret: allow chaining
   operator=                // eff: overwrites whole SPAvector
                            //      but maintains actual type of *this
                            //      use Make_copy to clone v
   (const DS_abs_vec & v)// in : (*this)[i] = v[i] for all i elements of v
                            {v.Assign_into(*this,OVERWRITE); return *this;}
 
  virtual DS_abs_vec &        // out: allow chaining
   operator+=                 // eff: increments by SPAvector
    (const DS_abs_vec & v) // in : (*this)[i] += v[i] for all i elements of v
                            {v.Assign_into(*this,PLUSEQ); return *this;}
 
  virtual DS_abs_vec &        // out: allow chaining
   operator-=                 // eff: increments by SPAvector
    (const DS_abs_vec & v) // in : (*this)[i] -= v[i] for all i elements of v
                            {v.Assign_into(*this,MINUSEQ); return *this;}

  virtual DS_abs_vec &        // out: allow chaining
   Overwrite                 // eff: increments by SPAvector
    (const DS_abs_vec & v, // in : (*this)[i] = mult*v[i] for all i elements of v
     double mult)         // in : multiplier
                            {v.Assign_into(*this,mult,OVERWRITE); return *this;}

  virtual DS_abs_vec &        // out: allow chaining
   Pluseq                 // eff: increments by SPAvector
    (const DS_abs_vec & v, // in : (*this)[i] += mult*v[i] for all i elements of v
     double mult)         // in : multiplier
                            {v.Assign_into(*this,mult,PLUSEQ); return *this;}

  virtual DS_abs_vec &        // out: allow chaining
   Minuseq                 // eff: increments by SPAvector
    (const DS_abs_vec & v, // in : (*this)[i] -= mult*v[i] for all i elements of v
     double mult)         // in : multiplier
                                 {v.Assign_into(*this,mult,MINUSEQ); return *this;}

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

  DS_multi_banded_vec &      // out: allow chaining
   operator+=               // eff: increments by SPAvector
    (const DS_multi_banded_vec & v) // in : (*this)[i] += v[i] for all i elements of v
                            //      requires (*this).Size() == v.Size()
                            { return Pluseq(v,1); }
                            /* OVERWRITE below was wrong anyway
                            // TODO:??? (JHS) this is a prototype implementation
                            //          which uses Sum.  Needs to be individually
                            //          implemented to optimize for speed
                            { v.Assign_into(*this, 1, OVERWRITE); return *this;}
                            */

  DS_multi_banded_vec &      // out: allow chaining
   operator-=               // eff: increments by SPAvector
    (const DS_multi_banded_vec & v) // in : (*this)[i] += v[i] for all i elements of v
                            //      requires (*this).Size() == v.Size()
                            { return Pluseq(v,-1); }
                            /* OVERWRITE below was wrong anyway
                            // TODO:??? (JHS) this is a prototype implementation
                            //          which uses Sum.  Needs to be individually
                            //          implemented to optimize for speed
                            { v.Assign_into(*this, -1, OVERWRITE); return *this;}
                            */

   DS_multi_banded_vec &               // out: allow chaining
    Sum(                               // eff: *this = a + b   
     const DS_multi_banded_vec & a,    // in : first SPAvector
     const DS_multi_banded_vec & b,    // in : second SPAvector
     double tol=0.0);                  // in : optional tolerance to explicitly
                                       //      zero out elements with magnitude
                                       //      less than tol

   DS_multi_banded_vec &               // out: allow chaining
    Pluseq(                            // eff: *this += mult*v   
     const DS_multi_banded_vec & v,    // in : rhs SPAvector
     double mult,                      // in : scalar multiplier
     double tol=0.0);                  // in : optional tolerance to explicitly
                                       //      zero out elements with magnitude
                                       //      less than tol

   virtual double                      // out: result = (*this).v
    Dotprod(                           // eff: evaluates dot product
            const DS_abs_vec & v) const// in : with abstract SPAvector v
                                       {return v.Dotprod(*this);}

   int Nbands()        const    // eff: returns number of bands in SPAvector
                                {return mbv_nbands ;}

   int Non_zero_elems()const    // eff: returns number of non-zero elements
                                //      in SPAvector
                                //      implemented as last offset
                                {return Offset(Nbands()) ;}

   int Min(int bandid) const    // eff: returns 1st non-zero element in band
                                {return mbv_minmax[3*bandid+1]  ;}

   int Max(int bandid) const    // eff: returns 1 + last non-zero element
                                //      in band
                                {return mbv_minmax[3*bandid+2];}
                           
   int Begin() const            // eff: returns 1st non-zero element in array
                                {if (Nbands()==0) {return Size();}
                                 else             {return Min(0);} }

   int End() const              // eff: returns 1 + last non-zero element 
                                //      in array
                                {if (Nbands()==0) {return 0;}
                                 else             {return Max(Nbands()-1);} }

   int Offset(int bandid) const // eff: returns 1 + last non-zero element 
                                //      in band
                                {return mbv_minmax[3*bandid];}

  void Remove_zeros(            // eff: remove zero elements from SPAvector
        double tol=0.0);        // in : tolerance for deciding what's zero

  virtual void
   Assign_into                   // eff: overwrites argument SPAvector
    (DS_multi_banded_vec & v,    // out: writes to multi-banded SPAvector
     EQ_FLAG flag)
      const
                                 {
                                  switch(flag)
                                   {
                                    case OVERWRITE:
                                      v = *this;
                                      break;
                                    case PLUSEQ:
                                      v += *this;
                                      break;
                                    case MINUSEQ:
                                      v -= *this;
                                      break;
                                   }
                                  }
  virtual void
   Assign_into                   // eff: overwrites argument SPAvector
    (DS_multi_banded_vec & v,    // out: writes to multi-banded SPAvector
     double mult,
     EQ_FLAG flag)
      const;

   double                                  // out: result = (*this).v
    Dotprod(                               // eff: evaluates dot product
            const DS_multi_banded_vec & v) // in : with concrete SPAvector v
                                  const;
/*
#ifdef DS_ABCD_VEC_ON
  virtual void
   Assign_into                   // eff: overwrites argument SPAvector
    (DS_abcd_vec & v,
     EQ_FLAG flag=OVERWRITE)    // out: writes to multi-banded SPAvector
      const;

  virtual void
   Assign_into                   // eff: overwrites argument SPAvector
    (DS_abcd_vec & v,
     double mult,
     EQ_FLAG flag=OVERWRITE)    // out: writes to multi-banded SPAvector
      const;
#endif // DS_ABCD_VEC_ON
*/
#ifdef DS_BLOCK_VEC_ON
  virtual void
   Assign_into                   // eff: overwrites argument SPAvector
    (DS_block_vec & v,
     EQ_FLAG flag=OVERWRITE)    // out: writes to multi-banded SPAvector
      const;

  virtual void
   Assign_into                   // eff: overwrites argument SPAvector
    (DS_block_vec & v,
     double mult,
     EQ_FLAG flag=OVERWRITE)    // out: writes to multi-banded SPAvector
      const;

  virtual double                                  // out: result = (*this).v
    Dotprod(                               // eff: evaluates dot product
            const DS_block_vec & v)       // in : with concrete SPAvector v
                                  const;
#endif // DS_BLOCK_VEC_ON

protected:
// TODO:??? (JHS) these are low-level band manipulation routines - they
// should probably be in a friend class to hide their interface from
// client classes.  This would also allow much more inlining of these
// routines (without raising code-bloat concerns).
// All routines should leave *this in valid state
// TODO:??? (JHS) decide if size zero or contiguous bands are an invalid state
//                if not, routines should be private instead.

  // The following low-level routines are responsible for changing band limits,
  // but are NOT responsible for calling Set_size().
  // These routines do NOT change band number (i.e. delete or merge bands).
  void Lower_band_max(               // eff: shrinks end of band
                      int band,      // in : bandid (0,....,Nbands()-1)
                      int new_max);  // in : new max of band

  void Raise_band_max(               // eff: grows end of band
                      int band,      // in : bandid (0,....,Nbands()-1)
                      int new_max);  // in : new max of band
                                     //      leaves new values unitialized

  void Raise_band_max(               // eff: grows end of band
                      int band,      // in : bandid (0,....,Nbands()-1)
                      int new_max,   // in : new max of band
                      double val);   // in : initializes new elems to val

  void Lower_band_min(               // eff: grows beginning of band
                      int band,      // in : bandid (0,....,Nbands()-1)
                      int new_min);  // in : new min of band
                                     //      leaves new values unitialized

  void Raise_band_min(               // eff: shrinks beginning of band
                      int band,      // in : bandid (0,....,Nbands()-1)
                      int new_min);  // in : new min of band

  void Lower_band_min(               // eff: grows beginning of band
                      int band,      // in : bandid (0,....,Nbands()-1)
                      int new_min,   // in : new min of band
                      double val);   // in : initializes new elems to val

   // The following intermediate-level routines
   //   use Raise/Lower routines to reduce/grow bandsize
   //   use Insert/Remove size_zero routines to adjust band numbers

   int Find_band(                    // eff: if elem lives in a band,
                                     //      returns the bandid
                                     //      else returns Nbands()
                 int elem) const;    // in : the element index

   int Find_gap (                    // eff: if elem lives in a gap,
                                     //      returns the bandid after the gap
                                     //      else returns -1
                 int elem) const;    // in : the element index

   int Force_band(                   // eff: Always returns bandid of the
                                     //      band the elem lives in.
                                     //      If elem doesn't live in
                                     //      a band when Force_band is called,
                                     //      Force_band creates or expands a 
                                     //      band for elem to live in
                  int elem);         // in : the element index

// TODO:??? (JHS) following 2 routines do not check for contiguous bands
//                decision should be made whether this is a valid state
//                before deriving from this class
   int                               // rtn: bandid of new band
       Insert_band(                  // eff: inserts a band with elems
                   int min,          // in : min<=elem
                   int max);         // in :      elem<max
                                     //      leaves new values unitialized

   int                               // rtn: bandid of new band 
       Insert_band(                  // eff: inserts a band with elems
                   int min,          // in : min<=elem
                   int max,          // in :      elem<max
                   double val);      // in : initialization value for elems

   void Remove_band(                 // eff: deletes a band
                    int bandid)      // in : band to be deleted(0,....,Nbands()-1)
                                       {
                                        Lower_band_max(bandid, Min(bandid));
                                        Remove_size_zero_band(bandid);
                                       }

private:                             // helper routines - not guaranteed
                                     // to leave *this in valid state

  void Set_nbands(                   // eff: adjusts mbv_nbands and ensures
                                     //      that mbv_minmax is correct size
                  int nbands)        // in : number of bands to adjust to
                                     {mbv_nbands = nbands;
                                      mbv_minmax.Grow(Minmax_size(nbands));}

  int Minmax_size(                   // eff: returns correct mbv_minmax size
                  int nbands)        // in : number of bands to be in mbv_minmax
                                     {return 3*nbands+1;}

  
  void Shift_offsets(                     // eff: low level helper routine to 
                                          //      adjust offsets after a band
                                          //      has changed size
                     int changed_bandid,  // in : the changed band
                     int change);         // in : size of the change

  // Next 3 routines responsible for incrementing/decrementing mbv_nbands
  int                                     // rtn: bandid of new band
       Insert_size_zero_band(             // eff: creates size a zero band
                             int index);  // in : index location of band

  void Remove_size_zero_band(             // eff: destroys a size zero band
                             int bandid); // in : bandid of band to be destroyed

  void Minimize_bandnum(                  // eff: scan band structure after
                                          //      specified band, merging
                                          //      contiguous bands and 
                                          //      removing zero size bands
                        int firstband=0); // in : start at firstband

  // Next two routines used by remove_zeros
  void Remove_zeros_from_index(           // eff: remove elements corresponding
                                          //      to zero values from minmax
                                          //      array
            const int* const zero_vec,    // in : index locations of zeros
                  int        count);      // in : number of zeros

  void Remove_zeros_from_data (           // eff: remove zero data from
                                          //      mbv_data
            const int* const zero_loc,    // in : zero locations in mbv_data
                  int        count);      // in : number of zeros


  // Next three routines allow changes to  min/max/offset
  // (non-const int* casts to satisfy HP700)
  int & Min(                // eff: return handle to Min of a band
            int bandid)     // in : band's id
                            {return ((int*)mbv_minmax)[3*bandid+1]  ;}

  int & Max(                // eff: return handle to Max of a band
            int bandid)     // in : band's id
                            {return ((int*)mbv_minmax)[3*bandid+2];}

  int & Offset(             // eff: return handle to Offset of a band
               int bandid)  // in : band's id
                            {return ((int*)mbv_minmax)[3*bandid]  ;}

} ; // end class DS_dbl_block


#endif //DS_BLOCK_H

