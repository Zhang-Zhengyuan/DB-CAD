// $Id: dsblock.hxx,v 1.9 2001/04/02 15:46:45 btomas Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dsblock.hxx
// created  : January 7, 1999
// author   : John Sloan
// contains : DS_dbl_block, DS_int_block, DS_BLK_EXACT_ENUM
// synopsis : Implements classes to handle allocation/destruction of mem blocks
//            This is safer than raw pointers since heap memory is deleted
//            automatically in destructor.
//
//            Memory block is accessed through appropriate conversion routine
//            e.g.  operator double*()
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
// contains : Class DS_dbl_block  block of doubles
//            Class DS_int_block  block of ints

#ifndef DS_BLOCK_H
#define DS_BLOCK_H

#include "mmgr.hxx"
#include "dcl_ds.h"
//=====================================================
// Concrete class DS_dbl_block
//=====================================================

class DECL_DM DS_dbl_block : public ACIS_OBJECT        // identifier = dbk
{

//=====================================================
// Enumeration DS_dbl_block::Exact_flag
//=====================================================
// use as a flag to force overloading function to call
// exact-size versions of DS_*_block:: Need() and Grow()
 enum Exact_flag {EXACT=1};  // just need type

 private:
  int      dbk_size   ;   // size of block (latest size request from client)
  int      dbk_cap    ;   // capacity of block (size of allocated memory)
  int      dbk_mult_i ;   // block growth multiplier (when integral)
  double*  dbk_data   ;   // the block

// TODO:??? (JHS) Extension to allow real blmults
//   int     dbk_mult_i ;   // == 0 indicates real
//   double  dbk_mult_d ;   // block growth multiplier (when real)

 public:

  // constructors, copy, destructor
  DS_dbl_block                // eff: default constructor
   (int size=0,               // in : initial block size
    int blmult=2);            // in : block growth multiplier
  
  DS_dbl_block                // eff : copy constructor
   (                          //       only allocates src.Size() (not Capacity)
    const DS_dbl_block &src); // in  : src object
                                
  DS_dbl_block &operator=     // eff : assignment operator
   (                          //       only allocates src.Size() (not Capacity)
    const DS_dbl_block &src); // in  : src object

 ~DS_dbl_block();          // destructor

  // simple_DS_dbl_block object data access
  int Size()     const;     // current requested size
  int Capacity() const;     // guarenteed to have this much

  // array access
        double              // returns rvalue
    operator[](int i)
    const;                   // so it doesn't change *this

          double &        // returns lvalue
    operator[](int i);

  // conversion to double* operators
  operator                  // conversion operator
    const double* const()   // const pointer to const data
    const;                   // so it doesn't change *this

  operator                  // conversion operator
          double* const();   // const pointer to non-const data
  //const                   // so it changes *this

  DS_dbl_block &
   Insert(int index, int count);

  DS_dbl_block &
   Insert(int index, int count, double val);

  DS_dbl_block &
   Remove(int index, int count);

  double* Release();

  void Copy_array(const double* data, int size);

  // operators to allow block to be used as a stack
  void Push(double val);        // Pushes val onto stack
  int                           // rtn : 0 if already empty, 1 if success
       Pop(double & val);       // out : value popped (unchanged if 0 returned)
  int                           // rtn : 0 if empty, 1 if success
       Top(double & val) const; // out : value of last element (unchanged if empty)

  DS_dbl_block &             // rtn : allow chaining
   Need(                     // eff : gets new mem, destroys data
        int new_size);       // in  : new size needed

  DS_dbl_block &             // rtn : allow chaining
   Need(                     // eff : gets new mem, destroys data
        int new_size,        // in  : new size needed
        Exact_flag);  // in  : dummy argument to indicate size==cap

  DS_dbl_block &             // rtn : allow chaining
   Grow(                     // eff : gets new mem, maintains data
        int new_size);       // in  : new size needed

  DS_dbl_block &             // rtn : allow chaining
   Grow(                     // eff : gets new mem, maintains data
        int new_size,        // in  : new size needed
        Exact_flag);  // in  : dummy argument to indicate size==cap

  DS_dbl_block &             // rtn : allow chaining
   Wipe();                   // eff : frees all allocated arrays, NULLs ptrs

} ; // end class DS_dbl_block

// To convert DS_dbl_block --> DS_int_block
// replace dbk    --> ibk
//         dbl    --> int
//         double --> int (not all occurences)

//=====================================================
// Concrete class DS_int_block
//=====================================================

class DECL_DM DS_int_block : public ACIS_OBJECT        // identifier = ibk
{

//=====================================================
// Enumeration DS_int_block::Exact_flag
//=====================================================
// use as a flag to force overloading function to call
// exact-size versions of DS_*_block:: Need() and Grow()
 enum Exact_flag {EXACT=1};  // just need type

 private:
  int      ibk_size   ;   // size of block (latest size request from client)
  int      ibk_cap    ;   // capacity of block (size of allocated memory)
  int      ibk_mult_i ;   // block growth multiplier (when integral)
  int*  ibk_data   ;   // the block

// TODO:??? (JHS) Extension to allow real blmults
//   int     ibk_mult_i ;   // == 0 indicates real
//   double  ibk_mult_d ;   // block growth multiplier (when real)

 public:

  // constructors, copy, destructor
  DS_int_block                // eff: default constructor
   (int size=0,               // in : initial block size
    int blmult=2);            // in : block growth multiplier
  
  DS_int_block                // eff : copy constructor
   (                          //       only allocates src.Size() (not Capacity)
    const DS_int_block &src); // in  : src object
                                
  DS_int_block &operator=     // eff : assignment operator
   (                          //       only allocates src.Size() (not Capacity)
    const DS_int_block &src); // in  : src object

 ~DS_int_block();          // destructor

  // simple_DS_int_block object data access
  int Size()     const;       // current requested size
  int Capacity() const;       // guarenteed to have this much

  // array access
    int                    // returns rvalue
    operator[](int i)
    const;                   // so it doesn't change *this

          int &             // returns lvalue
    operator[](int i);

  // conversion to int* operators
  operator                  // conversion operator
    const int* const()   // const pointer to const data
    const;                   // so it doesn't change *this

  operator                  // conversion operator
          int* const();   // const pointer to non-const data
  //const                   // so it changes *this

  DS_int_block &
   Insert(int index, int count);

  DS_int_block &
   Insert(int index, int count, int val);

  DS_int_block &
   Remove(int index, int count);
  int* Release();

  void Copy_array(const int* data, int size);

  // operators to allow block to be used as a stack
  void Push(int val);        // Pushes val onto stack
  int                        // rtn : 0 if already empty, 1 if success
       Pop(int & val);       // out : value popped (unchanged if 0 returned)
  int                        // rtn : 0 if empty, 1 if success
       Top(int & val) const; // out : value of last element (unchanged if empty)

  DS_int_block &             // rtn : allow chaining
   Need(                     // eff : gets new mem, destroys data
        int new_size);       // in  : new size needed

  DS_int_block &             // rtn : allow chaining
   Need(                     // eff : gets new mem, destroys data
        int new_size,        // in  : new size needed
        Exact_flag);  // in  : dummy argument to indicate size==cap

  DS_int_block &             // rtn : allow chaining
   Grow(                     // eff : gets new mem, maintains data
        int new_size);       // in  : new size needed

  DS_int_block &             // rtn : allow chaining
   Grow(                     // eff : gets new mem, maintains data
        int new_size,        // in  : new size needed
        Exact_flag);  // in  : dummy argument to indicate size==cap

  DS_int_block &             // rtn : allow chaining
   Wipe();                   // eff : frees all allocated arrays, NULLs ptrs

} ; // end class DS_int_block

#endif //DS_BLOCK_H
