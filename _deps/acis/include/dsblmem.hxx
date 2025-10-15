/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dsblmem.hxx
// author   : GW Celniker
// created  : Feb 1, 1995
// synopsis : isolate memory management calls to one location
//            so that advanced features and fixes can be
//            retroactively applied to the entire system

#ifndef DS_BLOCK_MEM_H
#define DS_BLOCK_MEM_H

// define functions
//  DS_copy_double_block       (double  *tgt, double *src, int count) ;
//  DS_clear_double_block      (double  *tgt, int count) ;
//  DS_size_double_block       (double **src, int old_count, int new_count) ;
//  DS_resize_double_block     (double **src, int old_count, int new_count) ;
//  DS_chunksize_double_block  (double **src, int old_count, int new_count, int block_size) ;
//  DS_insert_into_double_block(double **src, int old_count, int index, int val_count,double *value) ;
//
//  DS_copy_int_block       (int  *tgt, int *src, int count) ;
//  DS_clear_int_block      (int  *tgt, int count) ;
//  DS_size_int_block       (int **src, int old_count, int new_count) ;
//  DS_resize_int_block     (int **src, int old_count, int new_count) ;
//  DS_chunksize_int_block  (int **src, int old_count, int new_count, int block_size) ;
//  DS_insert_into_int_block(double **src, int old_count, int index, int val_count,int *value) ;

#include "dcl_ds.h"              // DECL_DM              // DECL_DM
// forward declarations
#include "dsprint.hxx"  // for printf indirection  // for printf indirection

//===========================================================
// functions for doubles
//===========================================================

void DECL_DM
DS_copy_double_block
  (double *tgt, const double *src, int count) ;

DECL_DM void
DS_clear_double_block
  (double *tgt, int count) ;

void
DS_size_double_block       // increase or decrease memory alloc size
  (double **src,           // without preserving data
   int old_count, 
   int new_count) ;

void
DS_resize_double_block     // increase or decrease block_memory size
  (double **src,           // while preserving data
   int old_count, 
   int new_count) ;

void                       // reduce number of allocation calls
DS_chunksize_double_block  // increase block_memory size by chunks
  (double **src,           // i/o: value changed to pt at memory
   int old_count,          // in : actual  use count for src block
   int new_count,          // in : desired use count for src block
   int chunk_size) ;       // in : elem-count per allocation chunk

void                       // eff: inc src size by val_count 
DS_insert_into_double_block//      and insert value array into src
  (double **src,           // i/o: array size increased by val_count
   int old_count,          // in : input size of tgt 
   int index,              // in : index loc to add new values
   int val_count,          // in : number of new values to add
   double *value) ;        // in : value array to copy into block

void
DS_dump_double_block       // eff: write array values to file
  (FILE *fp,               // in : ptr to file open for write
   int count,              // in : sizeof array
   double *array,          // in : array to print
   int vals_per_line) ;    // in : array vals per line

void
DS_dump_upper_tri_double_block  // eff: write array values to file
  (FILE *fp,               // in : ptr to file open for write
   int nn,                 // in : sizeof array = (nn)*(nn+1)/2
   double *array) ;        // in : array to print

void
DS_dump_upper_diag_double_block  // eff: write array values to file
  (FILE *fp,               // in : ptr to file open for write
   int nn,                 // in : sizeof array = (nn)*(nn+1)/2
   double *array) ;        // in : array to print


//===========================================================
// same functions for ints
//===========================================================

void DECL_DM
DS_copy_int_block
  (int *tgt, const int *src, int count) ;

void
DS_clear_int_block
  (int *tgt, int count) ;

void
DS_size_int_block          // increase or decrease memory alloc size
  (int **src,              // without preserving data
   int old_count, 
   int new_count) ;

void
DS_resize_int_block        // increase or decrease block_memory size
  (int **src,              // while preserving data
   int old_count, 
   int new_count) ;

void                       // reduce number of allocation calls
DS_chunksize_int_block     // increase block_memory size by chunks
  (int **src,              // i/o: value changed to pt at memory
   int old_count,          // in : actual  use count for src block
   int new_count,          // in : desired use count for src block
   int chunk_size) ;       // in : elem-count per allocation chunk

void                       // eff: inc src size by val_count 
DS_insert_into_int_block   //      and insert value array into src
  (int **src,              // i/o: array size increased by val_count
   int old_count,          // in : input size of tgt 
   int index,              // in : index loc to add new values
   int val_count,          // in : number of new values to add
   int *value) ;           // in : value array to copy into block

void
DS_dump_int_block          // eff: write array values to file
  (FILE *fp,               // in : ptr to file open for write
   int count,              // in : sizeof array
   int *array,             // in : array to print
   int vals_per_line) ;    // in : array vals per line

/* the old template way of doing business

template <class type> void 
DS_copy_block
  (type *tgt, type *src, int count)
// modifies: tgt
// effects : copies the element values of src to tgt
{
  memcpy((void *)tgt,
         (const void *)src,
         count * sizeof(type)) ;

} // end DS_copy_block<type>

template <class type> void
DS_clear_block
  (type *tgt, int count)
// modifies: tgt
// effects : sets the [count*sizeof(type)] block of memory pointed
//           to be tgt to 0.
{ 
  memset((void *)tgt, 0, count * sizeof(type)) ;
} // end DS_clear_block

template <class type> void
DS_resize_block            // increase or decrease block_memory size
  (type **src, 
   int old_count, 
   int new_count)
{
  // check for the delete case
  if(new_count == 0)      
   { 
     if(old_count > 0) delete [] *src ;
     *src = NULL ; 
     return ; 
   }

  // check for the no work case
  if(new_count <= old_count) return ;

  // get new memory, copy old values, delete old_memory, set src ptr
  type *tgt = new type[new_count] ;
  if(!(*tgt)) DM_sys_error(DM_FREE_STORE_OVERFLOW) ;

  DS_copy_block(tgt, *src, old_count) ;
  if(old_count > 0) delete [] *src ;
  *src = tgt ;

} // end DS_resize_block

template <class type> void // reduce number of allocation calls
DS_chunksize_block         // increase block_memory size by chunks
  (type **src,             // i/o: value changed to pt at memory
   int old_count,          // in : actual  use count for src block
   int new_count,          // in : desired use count for src block
   int chunk_size)         // in : elem-count per allocation chunk
{
  // input argument checking
  DS_assert(old_count >= 0) ;
  DS_assert(new_count >= 0) ;

  // The delete case
  if(new_count == 0) { if(old_count > 0)delete [] *src ;
                       *src = NULL ; 
                       return ; }

  // The first call case
  if(old_count == 0) { *src = 
                         new type[   chunk_size
                                  *((new_count-1)/chunk_size+1)] ;
                        return ; }

  // check to see if new memory is needed
  if(new_count > ((old_count-1)/chunk_size + 1) * chunk_size)
    {
      // get new memory
      type *tgt = new type[chunk_size*((new_count-1)/chunk_size+1)];
      if(!(*tgt)) DM_sys_error(DM_FREE_STORE_OVERFLOW) ;


      // copy old values 
      DS_copy_block(tgt, *src, old_count) ;

      // delete old memory
      delete [] *src ;

      // set src-ptr
      *src = tgt ;
    }

} // end DS_chunksize_block

// end of old template way of doing business */

#endif // DS_BLOCK_MEM_H


