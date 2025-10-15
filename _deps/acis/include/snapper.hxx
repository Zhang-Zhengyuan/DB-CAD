/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (BHL_SNAP)
#define BHL_SNAP

#define bhl_anal_tgt_acc 10

enum snap {
	START,
	PROCEED,
	CHANGE,
	STOP
};

class HH_Trans;

snap
choose_snap(snap, HH_Trans const & , snap , HH_Trans const &, HH_Trans &);

snap
choose_best_snap(snap, HH_Trans const & , snap , HH_Trans const &, HH_Trans &);

snap
combine_snap(snap, HH_Trans const & , snap , HH_Trans const &, HH_Trans &);

snap
total_snap(snap, HH_Trans const & , snap , HH_Trans const &, HH_Trans &);

#endif


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
