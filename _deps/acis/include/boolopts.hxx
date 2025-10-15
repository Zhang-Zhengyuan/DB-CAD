/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header file for the bool options.

#if !defined(BOOLOPTS_H)
#define BOOLOPTS_H

#include "dcl_bool.h"
class option_header;

// SESSION_GLOBAL_VAR option_header track_face_pairs( "track_face_pairs", FALSE );
extern DECL_BOOL option_header track_face_pairs;

// SESSION_GLOBAL_VAR option_header all_free_edges( "all_free_edges", FALSE );
extern DECL_BOOL option_header all_free_edges;

// SESSION_GLOBAL_VAR option_header slow_bool4( "slow_bool4", FALSE );
extern DECL_BOOL option_header slow_bool4;

// SESSION_GLOBAL_VAR option_header auto_merge( "merge", TRUE );
extern DECL_BOOL option_header auto_merge;

// SESSION_GLOBAL_VAR option_header auto_trim( "trim#_faces", TRUE );
extern DECL_BOOL option_header auto_trim;

// SESSION_GLOBAL_VAR option_header debug_reboolean_option( "debug_reboolean", FALSE );
extern DECL_BOOL option_header debug_reboolean_option;

// SESSION_GLOBAL_VAR option_header keep_second_face( "keep_second_face", FALSE );
extern DECL_BOOL option_header keep_second_face;

// SESSION_GLOBAL_VAR option_header keep_second_edge( "keep_second_edge", FALSE );
extern DECL_BOOL option_header keep_second_edge;

extern DECL_BOOL option_header merge_spline_vertex;

// SESSION_GLOBAL_VAR option_header subsetting_on( "subset#ting", "spline");
extern DECL_BOOL option_header subsetting_on;

// GLOBAL_VAR option_header do_split_periodic( "do_split_periodic", TRUE );
extern DECL_BOOL option_header do_split_periodic;

// SESSION_GLOBAL_VAR option_header check_tangent( "check_ff_tangent_int", FALSE );
extern DECL_BOOL option_header check_tangent;

// SESSION_GLOBAL_VAR option_header check_ee_int_always( "check_ee_int_always", FALSE );
extern DECL_BOOL option_header check_ee_int_always;

// SESSION_GLOBAL_VAR option_header check_ff_int( "check_ff_int", FALSE );
extern DECL_BOOL option_header check_ff_int;

// SESSION_GLOBAL_VAR option_header new_periodic_splitting( "new_periodic_splitting", 0);
extern DECL_BOOL option_header new_periodic_splitting;

// SESSION_GLOBAL_VAR option_header split_face_checking( "split_face_checking", FALSE);
extern DECL_BOOL option_header split_face_checking;

// SESSION_GLOBAL_VAR option_header convexity_split( "convexity_split", TRUE );
extern DECL_BOOL option_header convexity_split;

// SESSION_GLOBAL_VAR option_header sliver_face_max_percent("sliver_max#_percent", 0.08);
extern DECL_BOOL option_header sliver_face_max_percent;

// SESSION_GLOBAL_VAR option_header failsafe_periodic_split( "fs_ps", FALSE );
extern DECL_BOOL option_header failsafe_periodic_split;

// SESSION_GLOBAL_VAR option_header bool_fs_imprint( "bool_fs_imprint", FALSE );
extern DECL_BOOL option_header bool_fs_imprint;

// SESSION_GLOBAL_VAR option_header bool_examine_mode( "bool_examine_mode", FALSE );
extern DECL_BOOL option_header bool_examine_mode;


#endif
