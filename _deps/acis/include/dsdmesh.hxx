/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dsdmesh.hxx
// created  : Sept 2, 1998
// author   : GW Celniker
// contains : DS_dmesh DS_mlink
// synopsis : Manages multisurf deformations.  Each component
//            surface is treated as a single element in a
//            large finite element program.  The DS_dmesh
//            object manages the assembly of the mesh model
//            by applying a set of continuity constraints 
//            accross the boundaries of each component surface.
// DS_dmesh : Each deformable modeling hierarchy can represent
//            a multi-surface deformable modeling mesh.  
//            The idea of a mesh is that several faces
//            connected to each other in some Brep model
//            can be sculpted and deformed as one large 
//            deformable modeling surface. This idea is
//            called multi-surface deformation.
// DS_mlink : Connecting the multiple DS_dmods of a single
//            DS_dmesh together are a set of constraints
//            called links.  The links are represented by the
//            class DS_mlink, and consist of a pointer to a
//            DS_link_cstrn constraint object and a pair of
//            pointers to the DS_dmods being connected.
//            The link_cstrn object is placed on the
//            constraint list of both of its dmods and
//            within the DS_mlink object.  Only the DS_mlink
//            object deletes this memory.
// Mesh Rep : Each deformable modeling mesh is represented by
//            a hierarchy of DS_dmod structures and one 
//            DS_dmesh object. The DS_dmod hierarchy may
//            have one or a set of sibling root nodes.  Each
//            root node represents one base face in the
//            multi-surface deformable mesh. Only one root
//            node is used to represent a single deformable
//            surface in which case no DS_dmesh object is
//            required.
//            The sibling root nodes are connected to one
//            another through a forward pointer linked list of
//            sibling pointers.  Each DS_dmod has a back pointer
//            to the deformable mesh's DS_dmod structure.
// Patches  : Patches are small deformable surfaces that
//            are attached to their parents.  Patches enable
//            local deformations. Patches are represented
//            as children within the deformable modeling
//            hierachy.  
// LIMITATION: Currently, patches can only lie within one parent.
// TODO:!!!    We will let each patch attach to any number of
//             Parents and then patches will be able to 
//             cover multiple sibling root dmods.
// DS_dmesh : The DS_dmesh data structure must contain enough
//            information to control the solution process for
//            a deformable mesh.  This data consists of
//              1. a pointer to the start of the root sibling list
//              2. a list of mesh edge constraints
//              3. a list of local to global dof numbering
//                   for each root DS_dmod.
//              4. Some state information
//              5. A list of shared loads (so that a single
//                    load SPAparameter is shared across many
//                    root siblings)
//              6. a DS_symeq object.
// Solutions: We solve the DS_dmesh problem in pieces.
//              1. The DS_dmesh uses the set of mesh edge cstrns
//                 to determine which dof numbers in each
//                 root sibling DS_dmod is used to connect the
//                 deformable mesh together.
//              2. Each root sibling's dof set is divided into
//                 internal and bounding sets.
//              3. Each root sibling DS_dmod reduces its set of 
//                 equations eliminating all the internal dofs
//                 leaving a set of symmetric linear equations
//                 whose unknowns are the bounding dofs.
//              4. The DS_dmesh then gathers together all of the
//                 reduced models and combines them into one
//                 huge combined model.
//              5. The deformable mesh model equation set is of
//                 the form:
//                  Minimize(1/2xAx - bx) subject to Cx=d,
//                  where x is the list of all boundary dofs.
//                 These equations are represented in a 
//                 DS_symeq object.
//              6. The above equations are solved for the 
//                 boundary dofs.
//              7. The boundary dof solutions are used to
//                 get the internal dof solutions for each
//                 root sibling. This completes the solution
//                 step for the siblings.
//              8. Then recursively, the solution for every
//                 deformable patch for every root DS_dmod is
//                 found.  This completes the solution step for
//                 for the complete deformable mesh structure.
//
// Constrns : The constraints of the root sibling DS_dmods must
//            be propagated to the combined mesh problem as
//            the solution proceeds. 
//            Each constraint can be expressed as one or more
//            linear equations on the dofs of the deformable 
//            model.  Think of each linear equation as connecting
//            some of the dofs together.  
//            These constraints can be divided into
//            groups depending on which dofs they relate.
//              1. Mesh edge constraints will connect dofs from
//                 two different root-sibling DS_dmods.
//                 All dofs used in a Mesh edge constraint are
//                 called linking dofs.  Any dof not used
//                 in a mesh edge constraint is called an
//                 internal dof.
//              2. Constraints on a single root-sibling DS_dmod
//                 connect together only those dofs from that
//                 root-sibling DS_dmod.  There are three
//                 possible connections that are of interest
//                 to solving the DS_dmesh set of equations.
//                 2.a) Cstrns on internal dofs only
//                 2.b) Cstrns on internal and external dofs
//                 2.c) Cstrns on linking dofs only
//              3. All Mesh edge constraints are applied to the
//                 combined set of Mesh equations.
//              4. All cstrns of type 2.c (only linking dofs)
//                 are promoted to the DS_dmesh object and
//                 applied to the mesh equations.
//              5. All cstrns of type 2.a (only internal dofs)
//                 are applied to the root-sibling dofs prior
//                 to the original reduction step.
//              6. The mixed dof cstrns are all reduced to
//                 eliminate as many internal dofs as possible.
//                 This will result in a set of equations
//                 of the form Idof = Sum(wi*Idofs) + Sum(wb*Ldofs) 
//                 and 0 = Sum(wb*Ldofs).  The constraints of
//                 the first form are used to reduce the 
//                 DS_dmod's dof set prior to the first reduction
//                 step.  The constraints of the second form are
//                 used to augment the Mesh edge constraints
//                 on the DS_dmesh object.
// state    :

#ifndef DS_DMESH_H
#define DS_DMESH_H


#include <math.h>                      // cos(), sin()
//#include "dshusk/dskernel/dsload.hxx"  // class DS_load, enum DS_LDS
#include "dscstrn.hxx" // class DS_cstrn, enum DS_CST // class DS_cstrn, enum DS_CST
#ifndef DS_USE_SYMEQ
  #include "dsbridge.hxx" // class DS_bridge // class DS_bridge
#endif /*DS_USE_SYMEQ*/

// forward references for "use" members and their derived type enums
#ifndef DS_USE_SYMEQ
  class DS_lueqns;
#include "dsluslvr.hxx" // class DS_bridge // class DS_bridge
#else /*DS_USE_SYMEQ*/
  class DS_symeq ;
#endif /*DS_USE_SYMEQ*/
class DS_dmod ;
//class DS_cstrn ;
//class DS_link_cstrn ;
class DS_mlink ;                       
class DS_load ;
// dms_state values Merge all required solve states
#define DMS_REBUILD_DOF_MAPS (1 <<  0) // 1=rebuild all link_dof_maps
#define DMS_CHANGED_DOF_MAPS (1 <<  1) // 1=changed since last solve

#define DMS_REBUILD_A        (1 <<  2) // 1=rebuild mesh A SPAmatrix
#define DMS_CHANGED_A        (1 <<  3) // 1=changed since last solve
                                   
#define DMS_REBUILD_B        (1 <<  4) // 1=rebuild mesh b SPAmatrix
#define DMS_CHANGED_B        (1 <<  5) // 1=changed since last solve
                                     
#define DMS_REBUILD_C        (1 <<  6) // 1=rebuild mesh C SPAmatrix
#define DMS_CHANGED_C        (1 <<  7) // 1=changed since last solve
                                     
#define DMS_REBUILD_D        (1 <<  8) // 1=rebuild mesh d SPAmatrix
#define DMS_CHANGED_D        (1 <<  9) // 1=changed since last solve
                                     
#define DMS_REBUILD_L        (1 << 10) // 1=rebuild mesh d SPAmatrix
#define DMS_CHANGED_L        (1 << 11) // 1=changed since last solve
                                     
#define DMS_REBUILD_LC       (1 << 12) // 1=rebuild mesh d SPAmatrix
#define DMS_CHANGED_LC       (1 << 13) // 1=changed since last solve
                                     
#define DMS_INIT_X           (1 << 14) // 1=copy dmod->link_dofs to x, old_x
#define DMS_FIRST_X          (1 << 15) // 1=copy dms_symeq->X to First_x()
#define DMS_CHANGED_X        (1 << 16) // 1=x values changed                                      
#define DMS_REBUILD_JOINT_PROBLEM  (  DMS_REBUILD_A | DMS_REBUILD_B \
                                    | DMS_REBUILD_C | DMS_REBUILD_D \
                                    | DMS_REBUILD_L | DMS_REBUILD_LC)

#define DMS_REBUILD_ALL   ( DMS_REBUILD_DOF_MAPS | \
                            DMS_INIT_X | \
                            DMS_REBUILD_JOINT_PROBLEM )

#define DMS_CHANGED_BITS  ( DMS_CHANGED_A | DMS_CHANGED_DOF_MAPS | \
                            DMS_CHANGED_B | DMS_CHANGED_X  | \
                            DMS_CHANGED_C | \
                            DMS_CHANGED_D | \
                            DMS_CHANGED_L | \
                            DMS_CHANGED_LC )                      
                                     
// DS_dmesh type_id enumerator
enum DS_DMS { ds_dms,         // base class DS_dmesh
              ds_lnk,         // base class DS_mlink
              ds_undef_mesh   // used for failed constructions
            } ;   

//=====================================================
// Base class DS_dmesh
//=====================================================

class DS_dmesh : public	ACIS_OBJECT            // identifier = dms
{
  protected:
   // DS_dmesh object data
   DS_DMS dms_type_id ;    // derived class identifier
   int dms_tag ;           // unique id for each dmesh
                           // only 1 per hierarchy for now, but you 
                           // never known when we will need 2.

   int dms_state ;         // Holds internal state to determine
                           // required processing 

   // DS_dmesh deformable model data
   DS_dmod *dms_dmod ;     // ptr to first root dmod (a linked list)

   // DS_dmesh equation data
   int dms_dof_count ;     // total dof count in all root siblings

   int dms_link_dof_count; // number of dofs used by link cstrns
   int dms_image_dim ;     // problem's image dimension size
   int dms_Cec_row_count;  // passed on (Cei,c)(xc)=(dei,c) eqn count
   int dms_LLc_row_count;  // passed on Lx=Lc eqn count

#ifndef DS_USE_SYMEQ
   DS_lueqns *dms_symeq;   // The mesh set of equations
   DS_lu_solver dms_lusolver;
   DS_bridge  dms_bridge;   // Solution management class
#else /*DS_USE_SYMEQ*/
   DS_symeq *dms_symeq ;   // The mesh set of equations
#endif /*DS_USE_SYMEQ*/
   // DS_dmesh tag object data
   int dms_mlink_count ;   // number of cstrns on dmesh cstrn-list
   DS_mlink *dms_mlink ;   // first link cstrn (a linked list)

   int dms_load_count ;    // number of loads on dmesh load-list
   DS_load  *dms_load ;    // Shared loads (TODO:???)

  public:

   // constructors, copy, Size_arrays, destructor
   DS_dmesh                // DS_dmesh default constructor
    (DS_dmod *dmod,        // in : head of root-sibling-list
     int tag=-1) ;         // in : unique dmesh tag_id (or -1)

   DS_dmesh                // eff: copy constructor
    (DS_dmesh &dmesh) ;    // in : object to copy

   DS_dmesh &operator=     // eff: assignment operator
    (DS_dmesh &dmesh) ;    // in : the object from which to copy values
                           //      Causes a deep copy of hierarchy
                           //      to be placed in tgt DS_dmesh

    virtual DS_dmesh*      // Virtual Make_copy
     Make_copy()           // eff: virtual copy function
                           { return( new DS_dmesh(*this)) ; }

/* // obsolete
   void                    // Size_arrays()
    Size_arrays            // eff: internal memory management
    (int dof_count=0) ;    // in : dof count in all root-siblings
*/ // end obsolete 

   virtual                 // Virtual destructor
   ~DS_dmesh() ;           // eff: free 'this' and all offspring data

   // simple DS_dmesh object data access
   DS_DMS Type_id()        // Run time type checking
                           { return dms_type_id ; }

   int Tag()               // rtn: tag id for 'this' dmesh
                           { return dms_tag ; }

   void Set_tag(int tag)   // eff: set DS_dmesh tag value
                           { dms_tag = tag ; }

   void Increment_tags     // eff: increment all dmesh tag values
     (int tag_shift) ;     // in : amount to shift each tag value

   // solver state access
   int State()             // rtn: state for 'this' dmesh
                           { return dms_state ; }

   void Set_state          // eff: set DS_dmesh state value
     (int state)           // in : the input state value
                           { dms_state = state ; }

   void                    // use: when any
    Set_dof_map_rebuild()  // eff: mark state bits to rebuild dof map
                           { // mark all joint matrices for rebuild
                             dms_state |= DMS_REBUILD_ALL ;
                           }

                           // use: called by member dmods 
   void                    //       who change their cstrns
    Set_Cd_rebuild()       // eff: Remember to rebuild joint CD 
                           { Set_dof_map_rebuild() ;
                           }

                           // use: called by member dmods 
   void                    //       who change their d matrices
    Set_d_rebuild()        // eff: Remember to rebuild joint CD 
                           { dms_state |= DMS_REBUILD_D ;
                           }



   // simple DS_dmesh equation data access
   DS_dmod *Dmod()         // rtn: head of root-sibling list
                           { return(dms_dmod) ; }

   void Clear_dmod_ptr()   // eff: set dms_dmod ptr to NULL
                           // note: only used by destructors
                           { dms_dmod = NULL ; }

   int Dof_count()         // rtn: total root-sibling dof count
                           { return(dms_dof_count) ; }
#ifndef DS_USE_SYMEQ
//   DS_cgeqns *Symeq()       // rtn: ptr to DS_dmesh symeq set
//                           { return(dms_symeq) ; }
#else /*DS_USE_SYMEQ*/
   DS_symeq *Symeq()       // rtn: ptr to DS_dmesh symeq set
                           { return(dms_symeq) ; }
#endif /*DS_USE_SYMEQ*/
   // simple DS_dmesh tag object data access
   int Mlink_count()       // rtn: number of cstrns in DS_dmesh
                           { return(dms_mlink_count) ; }

   int Dmod_link_count     // rtn: number of Mlinkds attached to dmod
    (DS_dmod *dmod) ;      // in : dmod for whom links are being counted

   DS_mlink *Mlink()       // rtn: head of DS_dmesh mlink list
                           { return(dms_mlink) ; }

   DS_mlink *              // rtn: ptr to DS_mlink containing DS_link_cstrn
    Find_mlink_by_cstrn    // eff: search all mlinks for input cstrn
     (DS_cstrn *cstrn) ;   // in : target cstrn to search for

  DS_cstrn *               // ret: Parent cstrn when deep in list or
   Is_cstrn_in_list        // ret: Null when 1st or not in list
    (DS_cstrn *tgt_cstrn,  // in : Cstrn to look for 
     int &in_listP) ;      // out: 1=in list,0=not_in_list

   int Load_count()        // rtn: number of loads in DS_dmesh
                           { return(dms_load_count) ; }

   DS_load *Load()         // rtn: head of DS_dmesh load list
                           { return(dms_load) ; }

   // DS_dmesh construct functions
   int                     // rtn:0=success,-1=not a root,-2=in hierarchy
    Add_dmod               // eff: add dmod to root-sibling list
     (DS_dmod *dmod,       // in : DS_dmod to add to list
      int &tag_shift) ;    // out: amount dmod tags were shifted
                           // deallocs: dmod->Dmesh() if it exists

   DS_dmod *               // rtn: ptr to removed dmod or NULL
    Rm_dmod                // eff: remove dmod from root-sib list
    (DS_dmod *dmod,        // in : DS_dmod to add to list
     int sibling_flag) ;   // in : 0=remove only this dmod from dmesh
                           //      1=remove this and all its subsequent 
                           //        siblings from dmesh  

   int                     // rtn: cstrn's unique identifier
    Add_link               // eff: add connecting edge cstrn
     (DS_link_cstrn *cstrn,// in : connection constraint
      int tag=-1) ;        // in : unique tag id 
                           //      or -1 to assign next value

   DS_dmod *               // rtn: ptr to detached dmod or NULL                    
    Rm_link                // eff: remove and rtn link from dmesh
     (int tag,             // in : unique DS_cstrn identifier
      int detached_flag) ; // in :1=separate sib lists when appropriate
                           //     0=don't

   void                    // eff: move mlink from 
     Move_link             //      one dmesh to another
      (DS_mlink *mlink,    // in : link to move
       DS_dmesh *dmesh) ;  // in : dmesh to receive the mlink

   // DS_dmesh solver functions
/* // unused function
  void                     // use: build dof_maps one dmod at a time
   Build_dof_and_min_Cd_map// eff: build dof_map for input dmod
    (DS_dmod *dmod) ;      // in : dmod to query and modify
*/ // end unused function
#ifndef DS_USE_SYMEQ
#else /*DS_USE_SYMEQ*/
  void                     // eff: build all dof_maps that partition
   Build_dof_and_min_Cd_maps//     dmod dofs into link and not_link.
    () ;
#endif /*DS_USE_SYMEQ*/
  int                      // rtn: 0=success,pos_num=conflict_count
                           //      -1=never converged.
   Solve                   // eff: solve all DS_dmesh equations
    (int iter=1,           // in : iter count before stopping
     int walk_flag=0,      // in : specify how deep to go
                           //      0='roots' only, 1=offspring also
     double max_dist2=     // in : Used when iter == -1, converges
        DS_DOUBLE_CLOSE) ; //      when dof max-motion-squared
                           //      is less than max_dist2

   // DS_dmesh query functions

   DS_dmod*                // rtn: dmod to match input
    Find_dmod              //      or NULL when not found
    (DS_dmod *tgt_dmod,    // in : DS_dmod to find
     DS_dmod *&elder) ;    // out: dmod object in front of found
                           //      object or NULL when found obj
                           //      is first on the link list.

   DS_dmod*                // rtn: first link-list dmod to match tag
    Find_dmod_by_tag       //      or NULL when not found
    (int tag,              // in : tag id to search for
     DS_dmod *&elder) ;    // out: dmod object in front of found
                           //      object or NULL when found obj
                           //      is first on the link list.

   DS_mlink *              // rtn: first link-list mlink to match tag
    Find_mlink_by_tag      //      or NULL when not found
     (int tag,             // in : tag id to search for
      DS_mlink *&elder) ;  // out: mlink object in front of found
                           //      object or NULL when found obj
                           //      is first on the link list.

   DS_mlink *              // rtn: drawn mlink found by input line or NULL
    Find_mlink_by_iline    // eff: A geometric pick based on the 
                           //       line pi = pi0 + u (pi1-pi0)
    (double *pi0,          // in : image space pt, Sized:[image_dim]
     double *pi1,          // in : image space pt, Sized:[image_dim]
     double max_dist,      // in : max dist in which to find mlink
     DS_CST_PT_INDEX       // out: image_pt index in mlink for closest pt
               &pt_index,  
     double &dist2,        // out: dist**2 to iline from cstrn
     double &uu,           // out: iline param of pt closest to mlink
     int cstrn_draw_bit) ; // in : bit in dmo_draw_state to draw cstrns


   // functions needed to get the job done
   private:

   // DS_dmesh side-effect functions
   DS_dmod *               // rtn: decoupled sibling list or NULL                    
    Decouple_siblings      // eff: check links for decoupled dmod sets
     (DS_dmod *dmod1,      // in : dmod1 of the removed link
      DS_dmod *dmod2) ;    // in : dmod2 of the removed link
                           // use: called by Rm_link()
     
   void                    // use: called by Build_dof_and_min_Cd_maps()
    Calc_dmod_offsets      // eff: assign each dmod a dof_index offset
     () ;                  //      and calc dms_dof_count and
                           //      dms_link_dof_count
                           // use: called by Solve()

/* // obsolete
   int                     // rtn: total link dof count
    Calc_joint_dof_map     // eff: index all dmesh dofs and
     () ;                  //      calc dms_link_dof_count and
                           //      aizes dms_symeq 
                           // use: called by Solve()
*/ // end obsolete

/* // needed for a sequence joint problem solver
   void                    // use: called by Solve()
    Sort_dmod_cstrns       // eff: 
      () ;
*/ // end needed for a sequence joint problem solver
#ifndef DS_USE_SYMEQ
                           // use: called by Solve()
   int                     // rtn: number of dmod eqns built
    Build_dmod_eqns        // eff: Build root dmods linear equations
     () ;                  //      
#else /*DS_USE_SYMEQ*/
                           // use: called by Solve()
   int                     // rtn: number of conflicting cstrns
    Reduce_dmods           // eff: reduce root dmods leaving
     () ;                  //      link dof linear equations
#endif /*DS_USE_SYMEQ*/
#ifndef DS_USE_SYMEQ
   void                    // use: called by Solve()
    Size_eqns() ;         // eff: allocate joint problem memory
#else  /*DS_USE_SYMEQ*/   
   void                    // use: called by Solve()
    Size_symeq() ;         // eff: allocate joint problem memory
#endif /*DS_USE_SYMEQ*/     
   void                    // use: called by Solve()
    Init_x_old_x           // eff: copy dof values to dmesh prob
     () ;                  // 

   void                    // use: called by Solve() 
    Init_first_x           // eff: copies dms_symeq->X() to First_x().
     () ; 

   void                    // use: called by Solve()
    Build_joint_A          // eff: assemble mesh A SPAmatrix
      () ;     

   void                    // use: called by Solve()
    Build_joint_b          // eff: assemble mesh b SPAmatrix
      () ;

   void                    // use: called by Solve()
     Build_joint_Cd        // eff: assemble mesh C SPAmatrix
       () ;                // 

   void                    // use: called by Solve()
     Build_joint_d         // eff: assemble mesh d SPAmatrix
       () ;                //
               
   void                    // use: called by Solve()
     Build_joint_LLALc     // eff: assemble mesh L SPAmatrix
       () ;                //
               
   void                    // use: called by Solve()
     Build_joint_Lc        // eff: assemble mesh Lc SPAmatrix
       () ;                //

   int                     // rtn: abcd_state for DS_symeq funcs
    Symeq_abcd_state() ;   // eff: map dms_state->syme_abcd_state               

   void                    // use: called by Solve()
    Move_xc_to_dmods() ;   // eff: place joint problem solution
                           //      back in distributed problems

   void                    // use: called by Solve()
    Move_r_to_dmods() ;    // eff: move joint r solution back
                           //      to root-sibling problems
  
                             // use: called by Solve() 
   int                       // rtn: 0=not converged (yet)
                             //      1=converged
                             //      2=won't converge due to chaos
    Finish_dmods_solve       // eff: finish dmod solves and response
    (int iter,               // in : iter count before stopping
                             //      -1 = iter to convergence
     int ii,                 // in : used if iter == -1, iteration count
     double max_dist2,       // in : used if iter == -1, converges
                             //      when dof max-motion-squared
                             //      is less than max_dist2
     int conflict_count) ;   // in : count of confliting constraints
                             //      discovered in last solve step                                                                                   

   void                      // use: called by Solve()
    Dmods_unconverged_response(); // eff: restore state after
                             //      solve() fails to converge.

   void                      // use: called by Solve()
    Solve_dmods_children     // eff: call solve on all children
    (int iter,               // in : iter count before stopping
                             //      -1 = iter to convergence
     double max_dist2) ;     // in : Used when iter == -1, converges
                             //      when dof max-motion-squared 
                             //      is less than max_dist2

   void                      // use: called by Reduce_dmods()
   Mark_dmod_matrix_changes  // eff: mark dmesh abcd state
  (int dmod_abcd_state) ;    // in : dmod abcd state to review

// DS_dmesh functions to help the DS_dmesh side effect functions

   void                    // use: called by Rm_dmod()
    Purge_mlinks           // eff: search all mlinks and remove
     () ;                  //      any that refer to a dmod not
                           //      in the current hierarchy

  virtual                  // bas: DS_cstrn virtual function
  int                      // rtn: 0=success,-1=file failure
   Dump                    // eff: write all internal vals to file
    (char *filename,       // in : filename = file to write
     FILE *fp) ;           // in : existing file or NULL
        
} ; // end class DS_dmesh

//=====================================================
// Base class DS_mlink
//=====================================================

class DS_mlink : public ACIS_OBJECT            // identifier = lnk
{
  protected:
   // DS_mlink object data
   DS_DMS lnk_type_id ;    // derived class identifier

   // note: the DS_mlink has no unique tag number
   //       of its own, it uses the tag number stored
   //       in its DS_link_cstrn object.

   // DS_mlink connectivity data
   DS_dmod       *lnk_dmod1 ;  // ptr to 1st connected dmod 
   DS_dmod       *lnk_dmod2 ;  // ptr to 2nd connected dmod
   DS_link_cstrn *lnk_cstrn ;  // ptr to connection constraint
                               // (freed when mlink object is deleted)
   // DS_mlink list data
   DS_dmesh      *lnk_dmesh ;  // back ptr to dmesh using this link
   DS_mlink      *lnk_next ;   // ptr to next mlink on linked list 


  public:

   // constructors, copy, Size_arrays, destructor
   DS_mlink                     // DS_mlink default constructor
    (DS_dmesh *dmesh=NULL,      // in : owner dmesh for link
     DS_dmod  *dmod1=NULL,      // in : 1st dmod being connected
     DS_dmod  *dmod2=NULL,      // in : 2nd dmod being connected
     DS_link_cstrn *cstrn=NULL, // in : the connecting constraint
     DS_mlink *next =NULL);     // in : next mlink on linked mlink list

   DS_mlink                // eff: copy constructor
    (DS_mlink &mlink) ;    // in : object to copy

   DS_mlink &operator=     // eff: assignment operator
    (DS_mlink &mlink) ;    // in : the object from which to copy values
                           //      Causes a deep copy of hierarchy
                           //      to be placed in tgt DS_mlink

    virtual DS_mlink*      // Virtual Make_copy
     Make_copy()           // eff: virtual copy function
                           { return( new DS_mlink(*this)) ; }

   virtual                 // Virtual destructor
   ~DS_mlink() ;           // eff: free 'this' and linked data

   // simple DS_mlink object data access
   DS_DMS Type_id()        // Run time type checking
                           { return lnk_type_id ; }

   int Tag()               // rtn: tag id for 'this' mlink
                           { return lnk_cstrn->Tag() ; }

   void Set_tag(int tag)   // eff: set DS_mlink tag value
                           { lnk_cstrn->Set_tag(tag) ; }

   // simple DS_mlink data access
   DS_dmod *Dmod1()        // rtn: head of root-sibling list
                           { return(lnk_dmod1) ; }

   DS_dmod *Dmod2()        // rtn: head of root-sibling list
                           { return(lnk_dmod2) ; }

   DS_link_cstrn *Cstrn()  // rtn: ptr to DS_mlink cstrn
                           { return(lnk_cstrn) ; }

   DS_dmesh *Dmesh()       // rtn: containing dmesh ptr
                           { return(lnk_dmesh) ; }

   void Set_dmesh          // eff: set the mlink's dmesh ptr val
    (DS_dmesh *dmesh)      // in : dmesh containing the mlink
                           { lnk_dmesh = dmesh ; }

   DS_mlink *Next()        // rtn: next mlink on linked list
                           { return(lnk_next) ; }

   DS_mlink *Last()        // rtn: last mlink on linked list
                           { DS_mlink *last = this ;
                             for(;last->lnk_next!=NULL
                                 ;last=last->lnk_next) ;
                             return(last) ;
                           }

   void Set_next           // eff: set the next linked object
    (DS_mlink *next)       // in : object to be next on linke list
                           { lnk_next = next ; }

  DS_cstrn *               // ret: Parent cstrn when deep in list or
   Is_cstrn_in_list        // ret: Null when 1st or not in list
    (DS_cstrn *tgt_cstrn,  // in : Cstrn to look for 
     int &in_listP) ;      // out: 1=in list,0=not_in_list

  // DS_dmesh side effect functions
  void                     // use: in dmesh build sequence
   Mark_linked_dofs        // eff: mark link-used dofs in dmod dof_maps
    (DS_dmod *dmod=NULL) ; //      NULL=mark both dmod maps
                           //      NOT NULL=mark only input dmod

  // debug functions
  virtual                  // bas: DS_cstrn virtual function
  int                      // rtn: 0=success,-1=file failure
   Dump                    // eff: write all internal vals to file
    (char *filename,       // in : filename = file to write
     FILE *fp) ;           // in : existing file or NULL

} ; // end class DS_mlink

#endif // DS_DMESH_H

