// $Id: dsabvec.hxx,v 1.9 2001/02/13 18:26:25 jsloan Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dsabvec.hxx
// created  : Jan 6, 1999
// author   : John Sloan
// contains : DS_abs_vec  - abstract SPAvector base class
// synopsis : Represents a linear algebra SPAvector.
//            Interface is tuned to avoid excessive heap allocations - 
//            only inplace arithmetic operations allowed.
//            (perhaps class should be called "inpvec" instead....
//            Anticipates sparse SPAvector derived classes, so direct
//            access to elements by clients is not allowed.
// Multi-methods:
//            Assignments such as DS_abs_vec::operator=(const DS_abs_vec &)
//            can be implemented element-by-element as a default.  This
//            default implementation is likely to be inefficient for 
//            sparse derived classes, however.  We need a method for
//            overriding the default operation.  The method implemented
//            here is multi-methods, discussed in Stroustrup 
//            "Design + Evolution ..." p.299.  Doubly virtual operator= can
//            be overriden to call singly virtual Assign_into(Derived type)
//            (i.e. 1 Assign_into for each derived type which will override).
//            Assign_into can have default implementation and overrides as
//            usual.  Since this re-couples the derived types back into
//            the abstract base class, we guard the Assign_into's with
//            a macro flag "DERIVED_TYPE_ON".  This allows the user to
//            decide at compile time how many derived types will be linked
//            into the program


#ifndef DS_ABVEC_H
#define DS_ABVEC_H
#include "mmgr.hxx" // class ACIS_OBJECT
class DS_int_block;
// class forward declarations for multi-methods
class DS_multi_banded_vec;    // derived class
class DS_block_vec;    // derived class
class DS_abcd_vec;    // derived class

// "multi-method override on" macro flags
#define DS_MULTI_BANDED_VEC_ON
#define DS_BLOCK_VEC_ON
#define DS_ABCD_VEC_ON

class DS_abs_vec : public ACIS_OBJECT              // identifier abv
{
 public:
// type_id enum
enum TYPE_ID { ds_abv,         // base class DS_abs_vec
              ds_blv,         // derived class DS_block_vec
              ds_mbv,         // derived class DS_multi_banded_vector
              ds_abc,         // derived class DS_abcd_vec
//              ds_spv,         // derived abstract class DS_sparse_vec
              ds_abv_none } ; // a null value for subroutine returns

// flag for switching between -=,=,+= in routines which
// take result SPAvector lvalues as argument
enum EQ_FLAG { MINUSEQ  =-1,
               OVERWRITE= 0,
               PLUSEQ   = 1};
#ifdef DSDEBUG
int                                        // rtn: 0==> file open failure       
Debug_dump_vector(int itag0=0, int itag1=0, int iappend=0) const;
                                           // eff: dump the SPAmatrix to a file
#endif /* DSDEBUG */
  protected:
  // Type_id, constructor, copy, Make_copy, destructor
   const TYPE_ID   abv_type_id ;  // enum showing specialization type
   int            abv_size ;     // total number of elements in SPAvector
// TODO:??? (JHS) data members above should be private, since public or
//                protected access methods are available for both.

  protected:
   void Set_size(                // eff: acceess method for derived classes
                                 //      to register size changes
                 int new_size)   // in : new size
                                 {abv_size = new_size;}

  public:

   TYPE_ID Type_id() const       // Type_id query method
                                { return abv_type_id ; }
 
  DS_abs_vec                    // eff: "default" constructor
   (TYPE_ID type_id,             // in : derived class identifier (required)
    int    size=0)              // in : initial size
    :
     abv_type_id(type_id),
     abv_size(size)
    {}
                          
  DS_abs_vec                    // eff: copy constructor
   (const DS_abs_vec&abs_vec)
   : 
    abv_type_id(abs_vec.Type_id()), 
    abv_size(abs_vec.Size())
   {}
                          
  virtual DS_abs_vec*          // ret: (upcast) pointer to concrete type
   Make_copy() const=0 ;       // eff: clones derived class
  
  virtual ~DS_abs_vec() {}     // destructor

// Size control/query functions
  virtual int  Size() const            // eff: returns current size
                               {return abv_size;}

  virtual void 
   Resize                      // eff: grows (or shrinks) array
    (int new_size,             // in : new size
     double val=0.0)=0;        // in : new elements are initialized to val

  virtual void
   Reserve                      // eff: grows (or shrinks) array
    (int new_size)=0;           // in : new size
                                //      does not initialize elements

  virtual void
   Wipe()=0;                    // eff: sets size to zero and deallocates
                                //      all reserved data space

// Array element arithmetic manipulation routines
// TODO:??? (JHS) Should these routines be returning void instead?

// single element changes
// TODO:??? (JHS) Could write Iterator/Element class (returned by op[])
// instead, which would hold a pointer reference to element then write
// single-element arithmetic routines on that.  Probably not worth it though

  virtual const double      // rtn: element value
   operator[]               // eff: array element query (rvalue)
   (int elem) const=0;      // in : the element index

  const double
   Elem                     // eff: array element query (rvalue)
   (int elem) const         // in : the element index
                            {return (*this)[elem];}

  virtual DS_abs_vec &      // ret: allow chaining
   Set_elem                 // eff: overwrites the value of one element
   (int elem,               // in : the element index
   double value)=0;         // in : vec[elem] = value

  virtual DS_abs_vec &      // ret: allow chaining
   Pluseq_elem              // eff: adds to the value of one element
   (int elem,               // in : the element index
    double value)=0;        // in : vec[elem] += value

  DS_abs_vec &              // ret: allow chaining
   Minuseq_elem             // eff: subtracts from the value of one element
   (int elem,               // in : the element index
    double value)           // in : vec[elem] -= value
                            {Pluseq_elem(elem, -value); return *this;}

  virtual DS_abs_vec &      // ret: allow chaining
   Timeseq_elem             // eff: multiplies one element by the value
   (int elem,               // in : the element index
    double value)=0;        // in : vec[elem] *= value

  DS_abs_vec &              // ret: allow chaining
   Diveq_elem               // eff: divides one element by the value
   (int elem,               // in : the element index
    double value)           // in : vec[elem] /= value
                            {
                             // TODO:!!! (JHS)  Trap for divide-by-0
                             Timeseq_elem(elem,1/value); return *this;}

// value browsing

  virtual int               // ret: the element index
   Largest_elem() const;    // eff: finds element with largest fabs
//
  virtual int                                        // ret: the element index
   Largest_elem(const DS_int_block& mask) const;    // eff: finds element with largest fabs with non-zero mask entry
#ifndef NONDSABSGEOM
// reduction operations
  virtual double Sum() const; // ret: sum of all elements
#endif // DSABSGEOM
// full SPAvector changes
  virtual DS_abs_vec &      // ret: allow chaining
   Zero()=0;                // eff: zeros all elements, can be more optimal
                            //      than Set(0.0), but Set could test for
                            //      zero so maybe not necessary.

  virtual DS_abs_vec &      // ret: allow chaining
   Negate()=0;              // eff: negates all non-zero elements

// Note that SPAvector =,+=,-= operators use the doubly virtual 
// multi-method "Assign_from" below
  virtual DS_abs_vec &      // ret: allow chaining
   operator=                // eff: overwrites whole SPAvector
                            //      but maintains actual type of *this
                            //      use Make_copy to clone v
   (const DS_abs_vec & v)   // in : (*this)[i] = v[i] for all i elements of v
                            {Assign_from(v, OVERWRITE); return *this;}

  virtual DS_abs_vec &        // out: allow chaining
   operator+=                 // eff: increments by SPAvector
    (const DS_abs_vec & v) // in : (*this)[i] = v[i] for all i elements of v
                              //      requires (*this).Size() == v.Size()
                            {Assign_from(v, PLUSEQ); return *this;}

  virtual DS_abs_vec &        // out: allow chaining
   operator-=                 // eff: increments by SPAvector
    (const DS_abs_vec & v)    // in : (*this)[i] = v[i] for all i elements of v
                              //      requires (*this).Size() == v.Size()
                            {Assign_from(v, MINUSEQ); return *this;}

  virtual DS_abs_vec &        // out: allow chaining
   Overwrite                 // eff: increments by SPAvector
    (const DS_abs_vec & v, // in : (*this)[i] += mult*v[i] for all i elements of v
     double mult)         // in : multiplier
                              //      requires (*this).Size() == v.Size()
                            {Assign_from(v, mult, OVERWRITE); return *this;}

  virtual DS_abs_vec &        // out: allow chaining
   Pluseq                 // eff: increments by SPAvector
    (const DS_abs_vec & v, // in : (*this)[i] += mult*v[i] for all i elements of v
     double mult)         // in : multiplier
                              //      requires (*this).Size() == v.Size()
                            {Assign_from(v, mult, PLUSEQ); return *this;}

  virtual DS_abs_vec &        // out: allow chaining
   Minuseq                // eff: increments by SPAvector
    (const DS_abs_vec & v, // in : (*this)[i] += mult*v[i] for all i elements of v
     double mult)          // in : multiplier
                             //      requires (*this).Size() == v.Size()
                             {Assign_from(v, mult, MINUSEQ); return *this;}

//      note that the following 3 routines are multi-methods
//      use double dispatch to implement for now
//      See Stroustup "Design + Evolution ..." p.299
//      for discussion/alternatives using Type_id()
  virtual void
   Assign_from
    (const DS_abs_vec & v,
           EQ_FLAG flag) ;

  virtual void
   Assign_from
    (const DS_abs_vec & v,
           double mult,
           EQ_FLAG flag) ;

   virtual double                     // out: result = (*this).v
    Dotprod                           // eff: evaluates dot product
     (const DS_abs_vec & v) const;  // in : with abstract SPAvector v

  virtual DS_abs_vec &      // ret: allow chaining
   operator=               // eff: adds to the value of all elements
   (double value)=0;        //      vec[elem] += value for all elem

  virtual DS_abs_vec &      // ret: allow chaining
   operator+=               // eff: adds to the value of all elements
   (double value)=0;        //      vec[elem] += value for all elem

   DS_abs_vec &             // ret: allow chaining
   operator-=               // eff: subtracts from the value of all elements
   (double value)           //      vec[elem] -= value for all elem        
                            {return *this += (-value);}

  virtual DS_abs_vec &      // ret: allow chaining
   operator*=               // eff: multiplies the value of all elements
   (double value)=0;        // in : vec[elem] *= value for all elem

   DS_abs_vec &             // ret: allow chaining
   operator/=               // eff: divides the value of all elements
   (double value)           // in : vec[elem] /= value for all elem
                            {// TODO:!!! (JHS) Trap for divide-by-0
                             return *this *= (1/value);}
                              // - declaration needed for multi-methods

// Double dispatch helper functions - two for each derived type
#ifdef DS_MULTI_BANDED_VEC_ON
  virtual void
   Assign_into                   // eff: overwrites argument SPAvector
    (DS_multi_banded_vec & v,
     EQ_FLAG flag=OVERWRITE)    // in : writes to multi-banded SPAvector
     const;

  virtual void
   Assign_into                   // eff: overwrites argument SPAvector
    (DS_multi_banded_vec & v,
     double mult,
     EQ_FLAG flag=OVERWRITE)    // in : writes to multi-banded SPAvector
     const;

   virtual double                     // out: result = (*this).v
    Dotprod                           // eff: evaluates dot product
     (const DS_multi_banded_vec & v) const;  // in : with abstract SPAvector v
#endif // DS_MULTI_BANDED_VEC_ON

#ifdef DS_BLOCK_VEC_ON
  virtual void
   Assign_into                   // eff: overwrites argument SPAvector
    (DS_block_vec & v,
     EQ_FLAG flag=OVERWRITE)    // in : writes to multi-banded SPAvector
     const;

  virtual void
   Assign_into                   // eff: overwrites argument SPAvector
    (DS_block_vec & v,
     double mult,
     EQ_FLAG flag=OVERWRITE)    // in : writes to multi-banded SPAvector
     const;

   virtual double                     // out: result = (*this).v
    Dotprod                           // eff: evaluates dot product
     (const DS_block_vec & v) const; // in : with abstract SPAvector v
#endif // DS_BLOCK_VEC_ON

#ifdef DS_ABCD_VEC_ON
  virtual void
   Assign_into                   // eff: overwrites argument SPAvector
    (DS_abcd_vec & v,
     EQ_FLAG flag=OVERWRITE)    // in : writes to multi-banded SPAvector
     const;

  virtual void
   Assign_into                   // eff: overwrites argument SPAvector
    (DS_abcd_vec & v,
     double mult,
     EQ_FLAG flag=OVERWRITE)    // in : writes to multi-banded SPAvector
     const;

   virtual double                     // out: result = (*this).v
    Dotprod                           // eff: evaluates dot product
     (const DS_abcd_vec & v) const; // in : with abstract SPAvector v
#endif // DS_ABCD_VEC_ON

} ; // end class DS_abs_vec

inline double 
Dotprod(
 const DS_abs_vec & a, 
 const DS_abs_vec & b)
                                 {return a.Dotprod(b);}
#endif // DS_ABVEC_H

