// $Id: dsgenblk.hxx,v 1.6 2001/07/23 20:05:34 btomas Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dsgenblk.hxx
// created  : February 6, 1999
// author   : John Sloan
// contains : Generic Block Class (macro'd) template
// usage    : Define macros for contained and container class names in
//            separate .hxx file, include dsgenblk.hxx, then undefine names
//            example:
//
//               class Foo;
//               #define DS_GENBLK_CLASS     Foo
//               #define DS_GENBLK_CONTAINER DS_foo_block
//               #include "dsgenblk.hxx"
//               #undef  DS_GENBLK_CLASS
//               #undef  DS_GENBLK_CONTAINER
//
// synopsis : Implements classes to handle allocation/destruction of mem blocks
//            This is safer than raw pointers since heap memory is deleted
//            automatically in destructor.
//
//            Memory block is accessed through appropriate conversion routine
//            e.g.  operator DS_GENBLK_CLASS*()
//
//            Grows memory through calls to Need(size) and Grow(size)
//            Need(size) implies throwing away old data
//            Grow(size) implies keeping old data
//
//            Default behavior is to only reallocate memory when requested
//            size exceeds current allocation.  New allocation will be
//            some multiple of current allocation, controlled by 
//            set_blmult(m = 2).
//            If current allocation is zero, new allocation will be 
//            requested size.  Allocated memory size can be queried with
//            Size().
//
//            To override default behavior, use
//            Need(size, DS_BLK_EXACT)
//            Grow(size, DS_BLK_EXACT)
//            which guarentee allocated size will == size.
//
//            Allocation is released by calling Wipe()
//            TODO:??? (JHS) and/or Need/Grow(0) ?
//                           what should behavior of Need/Grow(0) be?
//                           is Wipe() needed in addition to DS_BLK_EXACT?
//                     At moment, Need(0) behaves same as Need(1)
//
// contains : Class DS_GENBLK_CONTAINER  block of DS_GENBLK_CLASSs

// Note - this is a "template" file, so may be included multiple times
//        Therefore don't include the usual multiple-include guard macro

//=====================================================
// Concrete class DS_GENBLK_CONTAINER
//=====================================================

#include "dsstatic.hxx"

class DS_GENBLK_CONTAINER         // identifier = blk
{
 public:
//=====================================================
// Enumeration DS_GENBLK_CONTAINER::Exact_flag
//=====================================================
// use as a flag to force overloading function to call
// exact-size versions of DS_*_block:: Need() and Grow()
 enum Exact_flag {EXACT=1};  // just need type

  // deep copy - uses operator=
  LOCAL_PROC void Copy_block(
   DS_GENBLK_CLASS      * const tgt,
   DS_GENBLK_CLASS const* const src,
   int                          count);

  // shallow swap (memcpy swaps tgt with src)
  LOCAL_PROC void Swap_block(
         DS_GENBLK_CLASS* const tgt,
         DS_GENBLK_CLASS* const src,
   int                          count);

  // shallow swap (memcpy swaps tgt with src)
  LOCAL_PROC void Swap(
         DS_GENBLK_CLASS* const tgt,
         DS_GENBLK_CLASS* const src);

 private:
  int      blk_size   ;   // size of block (latest size request from client)
  int      blk_cap    ;   // capacity of block (size of allocated memory)
  int      blk_mult_i ;   // block growth multiplier (when integral)

  DS_GENBLK_CLASS*
           blk_data   ;   // the block

// TODO:??? (JHS) Extension to allow real blmults
//   int     blk_mult_i ;   // == 0 indicates real
//   double  blk_mult_d ;   // block growth multiplier (when real)

 public:

  // constructors, copy, destructor
  DS_GENBLK_CONTAINER                // eff: default constructor
   (int size=0,                      // in : initial block size
    int blmult=2)                    // in : block growth multiplier
    :
     blk_size(0),
     blk_cap(0),
     blk_mult_i(blmult),
     blk_data(0)
   {Need(size) ;}
  
  DS_GENBLK_CONTAINER                // eff : copy constructor
   (                                 //       only allocates src.Size() (not Capacity)
    DS_GENBLK_CONTAINER const &src); // in  : src object
                                
  DS_GENBLK_CONTAINER &operator=     // eff : assignment operator
   (                                 //       only allocates src.Size() (not Capacity)
    DS_GENBLK_CONTAINER const &src); // in  : src object

 ~DS_GENBLK_CONTAINER()              // destructor
                                     { Wipe() ; }

  // simple_DS_GENBLK_CONTAINER object data access
  int Size()     const {return blk_size;}     // current requested size
  int Capacity() const {return blk_cap;}      // guarenteed to have this much

  // array access
    DS_GENBLK_CLASS const &              // returns rvalue
    operator[](int i)
    const;                   // so it doesn't change *this

          DS_GENBLK_CLASS &        // returns lvalue
    operator[](int i);

  // conversion to DS_GENBLK_CLASS* operators
  operator                           // conversion operator
    DS_GENBLK_CLASS const* const()   // const pointer to const data
    const                            // so it doesn't change *this
                                     {return blk_data;}

  operator                           // conversion operator
          DS_GENBLK_CLASS* const()   // const pointer to non-const data
                                     {return blk_data;}

  // shallow copy when moving elements
  DS_GENBLK_CONTAINER &
   Insert(
          int index,
          int count);

  // shallow copy when moving elements
  DS_GENBLK_CONTAINER &
   Insert(
          int index,
          int count, 
          DS_GENBLK_CLASS const & val);

  // shallow copy when moving elements
  DS_GENBLK_CONTAINER &
   Remove(
          int index,
          int count);

  DS_GENBLK_CONTAINER &             // rtn : allow chaining
   Need(                            // eff : gets new mem, destroys data
        int new_size);              // in  : new size needed

  DS_GENBLK_CONTAINER &             // rtn : allow chaining
   Need(                            // eff : gets new mem, destroys data
        int new_size,               // in  : new size needed
        Exact_flag);         // in  : dummy argument to indicate size==cap

  DS_GENBLK_CONTAINER &             // rtn : allow chaining
   Grow(                            // eff : gets new mem, maintains data
        int new_size);              // in  : new size needed

  DS_GENBLK_CONTAINER &             // rtn : allow chaining
   Grow(                     // eff : gets new mem, maintains data
        int new_size,        // in  : new size needed
        Exact_flag);  // in  : dummy argument to indicate size==cap

  DS_GENBLK_CONTAINER &             // rtn : allow chaining
   Wipe();                   // eff : frees all allocated arrays, NULLs ptrs

 private:
  // responsible for resetting blk_cap
  void Free_data();
  void   Alloc_block(int new_cap); // can only grow
  void Realloc_block(int new_cap); // can only grow


} ; // end class DS_GENBLK_CONTAINER
