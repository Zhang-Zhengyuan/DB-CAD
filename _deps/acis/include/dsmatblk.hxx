/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dsmatblk.cxx    
// author   : bmt
// created  : October 15, 1999
// synopsis : implementations of DS_matblock - SPAmatrix virtual block structure


#include "dsblock.hxx"  //class DS_int_block  //class DS_int_block

class DS_matblock {

  private:
    DS_int_block mbl_matblock;   // End of each block.
    int mbl_nblocks;             // Number of blocks.
    int mbl_matsize;           // Matrix size.
  public:
    DS_matblock ( )              // Constructor.
    :
    mbl_nblocks(0),
    mbl_matsize(0)
    {}
    int What_block(int n) const;       // Returns the block n is in, or -1 if n is not in any block.
    void Resize( int n_matsize, int n_blocks );
    void Set_block_edge( int n_block, int n_xcomponent );
    int Get_nblocks() const
      { return mbl_nblocks; }
    int Get_block_edge(int n) const
      { DS_assert(n>=0&&n<Get_nblocks()); return mbl_matblock[n];}
    int Get_mat_size() const
      { return mbl_matsize;}
    void Make_copy(DS_matblock& blk) const;
};
