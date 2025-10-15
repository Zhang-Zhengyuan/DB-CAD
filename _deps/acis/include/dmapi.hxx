// $Id: dmapi.hxx,v 1.70 2002/08/16 17:49:46 btomas Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dmapi.hxx
// created  : December 9, 1997
// author   : GW Celniker
// synopsis : Defines the standalone programming interface
//            to the deformable modeling kernel functions.

#ifndef DMAPI_H
#define DMAPI_H

// overview : A curve or surface model is turned into a deformable
//            model by assigning an energy to the curve or surface
//            based on its shape.  The DM Modeler changes the shape
//            of the curve or surface to minimize its energy.
//            The deformable model is sculpted by applying
//            loads and constraints.  Each time a load or a
//            constraint SPAparameter varies the deformable model is
//            solved for a new shape.
// Shape    : The shape of a deformable curve or surface
//            model is represented by the class DS_pfunc which
//            has the following derived types:
//            DS_pfunc              (base class for shape)
//              DS_tprod_2d         (for <i>B</i>-spline surfaces)
//              DS_rprod_2d         (for <tt>NURB</tt> surfaces)
//              DS_tprod_1d         (for <i>B</i>-spline curves)
//              DS_rprod_1d         (for <tt>NURB</tt> curves)
//              DS_circ             (for ellipses and circles)
//
//            The tensor product (tprod) and rational
//            surface (rprod) definitions of shape depend on
//            the explicit representation of their basis functions
//            encapsulated in the class DS_basis.
//            DS_basis              (base class for basis functions)
//              DS_bspln            (for <i>B</i>-spline Bernstein polynomials)
//
// Dmods    : The deformable model base class is called DS_dmod.
//            DS_dmod               (base class for deformable models)
//              DS_dsurf            (for deformable surfaces)
//              DS_dcurv            (for deformable curves)
//
// Patches  : Patches are used to support the local deformation
//            of deformable models. Deformable
//            model patches may be nested in a hierarchical
//            tree in which the links in the tree represent
//            containment and the nodes individual deformable
//            models.  Children in the tree are connected to their
//            parents through a set of seam constraints that
//            ensure that the child shape can connect to the
//            parent shape with C0 or C1 continuity.  The tree
//            is represented in the DS_dmod structure as a
//            set of three pointers: parent, child, and sibling.
//
// Multisurf: A set of root dmods may be linked together as siblings
//            in the dmod hierarchy.  The DM Modeler will deform
//            all such patches as if they were a single surface.
//            (In the current release siblings are required to
//             connect together along their quad boundaries.)
//
// Links    : Links are a special kind of constraint that enforces
//            geometric C0 and C1 constraints across multisurf
//            boundaries.  Without links each dmod member of a
//            set of multsurf dmod's would deform without
//            interacting with its neighbors.
//
// Loads    : Loads are forces that cause the deformable model
//            to change its shape.  These loads are
//            all represented in the base class DS_load which
//            has the following derived classes.
//            DS_load               (base class for loads)
//              DS_spring           (for a point-to-point spring)
//              DS_spring_set       (for a group of pt-to-pt springs)
//              DS_pt_press         (for a point pressure load)
//              DS_dist_press       (for a area pressure load)
//              DS_crv_load         (for a curve-to-curve spring)
//              DS-dyn_load         (for mass and damping effects)
//
// Cstrns   : Cstrns are constraints that can force certain
//            locations on the shape of a deformable model
//            to be user controlled while the shape is sculpted
//            through the application of loads.
//            Sculpting can also be accomplished by tracking the
//            SPAposition or tangent values of the point constraint.
//            Constraints are represented in the base class
//            DS_cstrn and have the following derived types.
//            DS_cstrn              (base class for constraints)
//              DS_crv_cstrn        (for curve constraints on surfaces)
//              DS_pt_cstrn         (for pt constraints on curves and surfaces)
//            Currently the DS_cstrn class can do the following:
//              a.) apply SPAposition and tangent constraints at
//                  any point on a curve,
//              b.) apply SPAposition, tangent, and surface
//                  normal constraints at any point on a surface,
//              c.) apply SPAposition and tangent constraints along
//                  any curve within a surface.
//
// tag objs : Dmods, loads, and constraints are all tag objects.
//            Each object is assigned an integer value which
//            is managed by the calling application. Functions
//            which modify tag objects identify the desired tag
//            object by its tag value.
//
// active   : For access efficiency the DS_dmod containing
// DS_dmod    the load or cstrn last searched for by a tag
//            value is saved in the root dmod as the value
//            of the dmo_active pointer.  Dmod API function
//            that operate on one DS_dmod, load, or constraint
//            default to operating on the active DS_dmod.
//            Also when searching for an object by tag value
//            the active DS_dmod is always searched first
//            assuming that most commands will operate on the
//            same DS_dmod as the last.  When the active
//            DS_dmod fails to contain a desired tag object
//            then the DS_dmod hierarchy is searched in an
//            exhaustive and systematic fashion.
//
// Default  : Each deformable model can be assigned a rest shape.
// Shape      The rest shape is the shape the deformable model
//            will assume when there are no applied loads or
//            constraints.  By default the original rest shape
//            is for the deformable model to be flat and of
//            zero area (or length).  This is a good mode to
//            work in when styling shapes from scratch.  At any
//            time the current shape of the deformable model can
//            be captured as the default shape with the function
//            DM_set_default_shape().  This mode is good for
//            creating shapes which a smooth deformation from an
//            existing shape.
//
// Symeq    : The deformable model is turned into a math model
//            which is represented in the class DS_symeq.  Each
//            time the end-user calls DS_solve(), the symeq object
//            is given the chance to recompute a new solution for
//            shape based on changes in the tag objects.  End
//            users will find that the shapes produced by solving
//            the deformable modeling equations will be naturally
//            fair and smooth.
//
// Graphics : This API does not support the graphics for deformable
//            modeling.  However, a bit array stored as an
//            integer has been included within every deformable
//            model object that can be used by an application
//            to control which pieces of deformable modeling
//            data get rendered for each deformable model.
//            That bit array can be accessed through the
//            set of DM_DRAW... macro names which are
//            defined for convenience only in this header file.
//
// Parsing  : This file is parsed to automatically generate the
//  Rules   : dmapiw.cxx and dmapiw.hxx files which support
//            DM API level journaling.  Specific information
//            has to be included in the API prototype headers
//            and comments for the parser/code generator to work.
//            These rules include:
//
//            1. Every declaration's argument has to have an argument name.
//            2. Every argument has to be declared on its own line.
//            3. Every argument has to have a line comment which
//               includes one of the following tokens
//               // in : ...
//               // out: ...
//               // i/o: ...
//               // opt: ...
//            4. The argument names used in the code definition
//               have to be the same as the function prototypes.
//            5. Any argument which is used as an array has
//               to contain the following comment
//               // ... sized:[size]
//               where ... is any character string and
//                     size is any expression which may be
//                     evaluated at run time that specifies the
//                     number of members in the array.
//            6. Any argument which is a function pointer must
//               have the following comment before the final
//               closing argument list parenthesis.
//               // ... use:[ACIS_function_name]
//               where ... means any characters and
//                     ACIS_function_name is the name of the
//                     ACIS integration function pointer
//            7. Pointers and references to pointers are labeled
//               to tell the journal function how to handle memory
//               management.  The journal mechanism keeps a table
//               of all pointers given memory during a journal play
//               session. Memory management for each such pointer
//               depends on the label on the line for each
//               pointer. The set of labels include:
//              +---------------+------------------------------------------+
//              |ptr-label      | Pointer use description                  |
//              +---------------+------------------------------------------+
//              |[nested]       | Means the DM API call sets or returns    |
//              |               | an internally referenced piece of memory.|
//              |               | Any queries or modifications thru this   |
//              |               | ptr acts directly on the memory of a DM  |
//              |               | API object.  Delete called on these ptrs |
//              |               | will cause memory errors.                |
//              +---------------+------------------------------------------+
//              |[freed]        | Used in destructor functions. The DM API |
//              |               | function calls delete on this pointer.   |
//              |               | References through this pointer after the|
//              |               | DM API call will cause memory errors.    |
//              +---------------+------------------------------------------+
//              |[NULL-entry]   | Pointer is NULL on entry to the DM API   |
//              |               | and assigned memory that the application |
//              |               | must delete when appropriate to prevent  |
//              |               | memory leaks.                            |
//              +---------------+------------------------------------------+
//              |[malloc]       | Label for a pointer allocated memory by  |
//              |               | the DM API call that the application     |
//              |               | must delete when appropriate to prevent  |
//              |               | memory leaks.                            |
//              +---------------+------------------------------------------+
//              |[pre-alloced]  | The memory referenced by the pointer is  |
//              |               | used by the DM API call, but is not      |
//              |               | realloced nor stored within a DM API obj.|
//              |               | The mem management state of the pointer  |
//              |               | is not changed by the call, i.e. [nested]|
//              |               | and not [nested] pointers remain the way |
//              |               | they were input to the function.         |
//              +---------------+------------------------------------------+
//              +[pass-thru]    | The pointer is stored in a DM API object |
//              |               | and passed to callback functions.  Delete|
//              |               | is not called on this pointer. The       |
//              |               | application must plan to free this memory|
//              |               | to prevent memory leaks when the DM API  |
//              |               | object is deleted.                       |
//              +---------------+------------------------------------------+
//               Every argument pointer must have the label on the same
//               line as the arguments declaration.
//            8. Every DM API function definition must use the
//               macro line
//               ENTER_[dm_api_function_name]
//               to support journal writing.
//            9. Every exit point in every DM API function definition
//               must be made by the macros
//               EXIT_[dm_api_function_name] and
//               EXIT_[dm_api_function_name](exit_value)
//               where exit_value is any run-time expression used
//               for the return value of the function.
//
// contains :
// DS_pfunc and DS_dmod constructors, copies, queries, and destructors
//   DM_make_bspline_curve      DM_get_bspline_curve
//   DM_make_nurb_curve         DM_get_nurb_curve
//   DM_make_circ_curve         DM_get_circ_curve
//   DM_make_bspline_surface    DM_get_bspline_surface
//   DM_make_nurb_surface       DM_get_nurb_surface
//   DM_make_dcurve_image
//
//   DM_get_pfunc_type_id
//   DM_get_pfunc_image_dim
//   DM_get_pfunc_domain_dim
//   DM_get_pfunc_dof_count
//   DM_get_pfunc_elem_count
//   DM_get_pfunc_knot_counts
//   DM_get_pfunc_dofs
//   DM_get_pfunc_degrees
//   DM_get_pfunc_knots
//   DM_get_pfunc_domain_min
//   DM_get_pfunc_domain_max
//   DM_get_pfunc_default_state
//   DM_set_pfunc_default_state
//   DM_scale_pfunc_image
//   DM_eval_pfunc
//   DM_copy_pfunc
//   DM_delete_pfunc
//
//   DM_scale_unit_dpt_to_dmod          DM_scale_unit_dpt_to_pfunc
//   DM_scale_unit_dvec_to_dmod         DM_scale_unit_dvec_to_pfunc
//   DM_scale_unit_dpt_from_dmod        DM_scale_unit_dpt_from_pfunc
//   DM_scale_unit_dpt_array_to_dmod    DM_scale_unit_dpt_array_to_pfunc
//   DM_scale_unit_dpt_array_from_dmod  DM_scale_unit_dpt_array_from_pfunc
//
//   DM_scale_dmod_dpt_to_pfunc
//   DM_scale_dmod_dvec_to_pfunc
//   DM_scale_dmod_dpt_from_pfunc
//   DM_scale_dmod_dpt_array_to_pfunc
//   DM_scale_dmod_dpt_array_from_pfunc
//
//   DM_make_dmod_curve         DM_get_dmod_curve
//   DM_make_dmod_surface       DM_get_dmod_surface
//   DM_make_tracking_curve_dmod
//   DM_make_orig_dmod_space_pfunc
//
//   DM_get_dmod_pfunc
//   DM_get_dmod_tag            DM_set_dmod_tag
//   DM_get_dmod_type_id        DM_set_dmod_type_id
//   DM_get_dmod_cstrn
//   DM_get_dmod_load
//   DM_get_dmod_domain_min
//   DM_get_dmod_domain_max
//   DM_get_dmod_domain_scale
//   DM_eval_dmod
//   DM_refine_dmod_fit
//   DM_copy_dmod
//   DM_delete_dmod
//   DM_next_dmod
//
// multisurf and patch management
//   DM_add_curve_patch
//   DM_add_surface_patch
//   DM_rm_patch
//   DM_get_active_patch
//   DM_get_active_patch_tag
//   DM_set_active_patch
//   DM_get_patch_continuity
//   DM_set_patch_continuity
//   DM_get_parent
//   DM_get_child
//   DM_get_sibling
//   DM_get_patch_seam_count
//   DM_get_patch_seam_tag
//   DM_find_patch_by_tag
//   DM_find_patch_by_tag_flag (same as DM_parse_tag_flag)
//   DM_find_patch_by_entity
//   DM_build_square_seam
//   DM_build_fillet_square_seam
//   DM_build_ellipse_seam
//   DM_build_square_zone
//   DM_delete_zone
//
// graphics management
//   DM_get_draw_state
//   DM_set_draw_state
//   DM_get_comb_graphics
//   DM_set_comb_graphics
//   DM_get_mesh_count
//   DM_set_mesh_count
//   DM_get_icon_radius         // for later release
//   DM_set_icon_radius         // for later release
//   DM_get_draw_count
//   DM_get_tan_display_gain
//   DM_set_tan_display_gain
//
// deformable model management
//   DM_get_integral_degree
//   DM_set_integral_degree     // for later release
//   DM_get_domain_dim
//   DM_get_image_dim
//   DM_solve
//   DM_split_dmod
//   DM_get_dmod_knots
//   DM_elevate_dmod_degree
//   DM_get_dmod_degree
//   DM_extrapolate_dmod
//   DM_print_dmod
//   DM_get_alpha
//   DM_set_alpha
//   DM_get_beta
//   DM_set_beta
//   DM_get_gamma
//   DM_set_gamma
//   DM_get_delta
//   DM_set_delta
//   DM_get_default_shape
//   DM_set_default_shape
//   DM_get_dynamics
//   DM_set_dynamics
//   DM_get_dof_state
//   DM_get_elem_count
//   DM_set_elem_count          // for later release
//   DM_get_entity
//   DM_set_entity
//
// constraint management
//   DM_add_pt_cstrn
//   DM_add_crv_cstrn
//   DM_add_link_cstrn
//   DM_add_area_cstrn
//   DM_mark_link_cstrn
//   DM_find_cstrn_by_tag
//   DM_get_cstrn
//   DM_get_cstrn_behavior       DM_set_cstrn_behavior
//   DM_set_cstrn_pttan_uv_dir
//   DM_get_cstrn_state          DM_set_cstrn_state
//   DM_get_cstrn_rights
//   DM_get_cstrn_type_id
//   DM_get_cstrn_src_data       DM_set_cstrn_src_data
//   DM_get_cstrn_src_pfuncs     DM_set_cstrn_src_pfuncs
//   DM_get_cstrn_src_dmods      DM_set_cstrn_src_dmods
//   DM_get_end_conds            DM_set_end_conds
//   DM_get_area_cstrn_flag      DM_set_area_cstrn_flag
//   DM_print_dmod_cstrns
//   DM_get_interior_state       DM_set_interior_state
//
// load management
//   DM_add_pt_press
//   DM_add_dist_press
//   DM_add_vector_load
//   DM_add_attractor
//   DM_add_spring
//   DM_add_spring_set
//   DM_add_crv_load
//   DM_find_load_by_tag
//   DM_get_pt_press
//   DM_get_dist_press
//   DM_get_vector_load
//   DM_get_attractor
//   DM_get_spring
//   DM_get_spring_set
//   DM_get_spring_length
//   DM_get_curve_load
//   DM_get_load_gain             DM_set_load_gain
//   DM_set_attractor_power
//   DM_get_load_type_id
//   DM_print_load              // for later release
//
// load and constraint management
//   DM_convert_loads_and_cstrns
//   DM_get_pt_uv                  DM_set_pt_uv
//   DM_get_pt_xyz                 DM_set_pt_xyz
//   DM_get_cstrn_value            DM_set_cstrn_value
//   DM_update_cstrn_src_pts
//
// tag management
//   DM_get_tag_summary
//   DM_get_sibling_list
//   DM_rm_tag_object
//   DM_assign_next_tag
//   DM_get_tag_count
//   DM_set_tag_count
//   DM_classify_tag
//   DM_parse_tag_flag
//   DM_is_type_tracking_curve
//   DM_is_type_patch_dmod
//
// interaction enabling functions
//   DM_find_tag_by_image_line
//   DM_xsect_dmod_by_image_line
//
// library management
//   DM_get_tolerance
//   DM_set_tolerance
//   DM_journal_on
//   DM_journal_off
//   DM_journal_play
//
// icon management
//   DM_set_icon_owner
//   DM_set_icon
//   DM_get_icon
//   DM_draw_icon
//   DM_setstate_icon
//   DM_query_icon
//
// tag object queries
//
//   DM_get_tags
//   DM_get_dmod_tags
//   DM_get_dmods
//   DM_param_min
//   DM_param_max
//   DM_eval_crv_src_domain
//   DM_eval_crv_tgt
//
// container class management
//   DM_set_array_size
//
// forward class declarations
#include "dcl_ds.h"              // DECL_DM define              // DECL_DM define
#include "safe.hxx"
#include <stdio.h>                      // NULL, FILE
#include "dmerror.hxx"  // dmapi error numbers  // dmapi error numbers
#include "dmapinum.hxx" // dmapi enumerations and macros constants // dmapi enumerations and macros constants
/**
 * \defgroup DEFMODAPI Deformable Modeling
 *
 * \defgroup DMAPI Deformable Modeling Direct Interface
 * \ingroup DEFMODAPI
 *
 */
/**
 * @file dmapi.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup DMAPI
 *
 * @{
 */
class DS_cstrn;
class DS_dmod;
class DS_pfunc;
class DS_zone;
class DS_load;
class DM_icon_draw_args;
class DM_icon_cmd_args;
class DM_icon_query_args;
class DM_icon;
class DM_tag_array;
class DM_dbl_array;
class DS_init_sdmhusk_args;

class SDM_options;

#define PI 3.1415926535898 // same number used in ACIS.hxx
// control point tag to index control
// The range of numbers used for tags has to
// be negative and start after the very last
// error number used in file dmerror.hxx
#define DM_is_tag_control_pt(tag)       (tag <= -500)
#define DM_control_pt_tag_2index(tag)   (-tag -500)
#define DM_control_pt_index_2tag(index) (-500 - index)

/*
// tbrv
*/
#ifdef THREAD_SAFE_ACIS
/**
 * @nodoc
 */
extern DECL_DM safe_integral_type<int> DM_use_legacy_scaling; // Global governing the use of legacy domain scaling
/**
 * @nodoc
 */
extern DECL_DM safe_integral_type<int> DM_constrain_G1_sing;  // internal use only
#else
/**
 * @nodoc
 */
extern DECL_DM int DM_use_legacy_scaling; // Global governing the use of legacy domain scaling
/**
 * @nodoc
 */
extern DECL_DM int DM_constrain_G1_sing;  // internal use only
#endif

typedef void (*SRC_CW_FUNC)     // opt: user given src_crv description, [pre-alloced]
                             // with ACIS use:[DS_true_edge_eval]
      (void *src_data,       //     in : src_data for eval, [pre-alloced]
       double s,             //     in : curve s param value, [pre-alloced]
       double *C,            //     out: surface C=[u,v] point value, [pre-alloced]
       double *Cs,           //     out: surface dC/ds vec in surf for s, [pre-alloced]
       double *W,            //     out: image space W=[x,y,z] point val, [pre-alloced]
       double *Wu,           //     out: image space dW/du for s, [pre-alloced]
       double *Wv,           //     out: image space dW/dv for s, [pre-alloced]
       double *Wuu,          //     out: image space d2W/du2
       double *Wuv,          //     out: image space d2W/dudv
       double *Wvv,          //     out: image space d2W/dv2
       double &dist2);       //     out: dist**2(xyz,proj_to_3d(uv))

// convenience tag type groupings
// see DM_is_type_tracking_curve()
//     DM_is_type_patch_dmod

//=========================================================
// deformable modeling prototype declarations
//=========================================================

//=========================================================
// DS_pfunc and DS_dmod Constructors,Copies,Queries,Deletes
//=========================================================

//===========================================================
// DS_pfunc *DM_make_bspline_curve
//===========================================================
/**
* Creates a <i>B</i>-spline curve from data and returns a pointer to the <tt>DS_pfunc</tt> <i>B</i>-spline curve or <tt>NULL</tt>.
* <br><br>
* <b>Role:</b> Modifies <tt>return_err</tt>.
* <br><br>
* Returns a pointer to the allocated and initialized memory used to represent a
* <i>B</i>-spline curve as a <tt>DS_pfunc</tt> description and sets the <tt>rtn_err</tt> as described below.
* Call <tt>DM_delete_pfunc()</tt> on the returned pointer to free its memory. All inputs
* are copied, not nested, within the constructed <tt>DS_pfunc</tt>.
* <br><br>
* Creates and returns a pointer to the <tt>DS_pfunc</tt> <i>B</i>-spline curve representation.
* <tt>image_dim</tt> is the size of the model space, using <tt>2</tt> for <tt>2D</tt> and <tt>3</tt> for <tt>3D</tt>. degree is
* the degree of <i>B</i>-spline polynomials. <tt>dof_count</tt> is the number of <i>B</i>-spline control
* points. <tt>knot_coun</tt>t is the number of unique knot values. The <tt>total_knot_count</tt>
* equals <tt>dof_count + degree - 1</tt>.
* <br><br>
* The <tt>total_knot_count</tt> will not equal the distinct knot_count when the <i>B</i>-spline
* has any multiple knots. The multiplicity of each knot is communicated through
* the <tt>knot_index</tt> array. Each knot, starting with the lowest <i>u</i> value knot, is
* assigned a <tt>knot_index</tt> starting at <tt>0</tt>. The <tt>knot_index</tt> array records the highest
* index value for all knots at each distinct knot location. For example, a common
* <i>B</i>-spline is one with multiple knots on the end points and single knots in the
* interior. A map of the knot index values might look like:
* <pre>
* +------+------+-------+------+
* 0  3  4  5  6
* 1           7
* 2           8 </pre>
* This knot array corresponds to a <tt>degree = 3</tt> <i>B</i>-spline with seven control points
* and four spans. The <i>B</i>-spline interpolates the first and last control point
* positions due to the multiple end knots.
* <br><br>
* There are nine total knot values and only five distinct knot values. The knot
* index array for this curve would be <tt>[2,3,4,5,8]</tt>. The knot array contains the
* domain space values (<i>u</i> values) for each distinct knot. For the above example,
* the knot array might be something like <tt>[0.00, 0.25, 0.50, 0.75, 1.00]</tt> for an
* even distribution of knot values from <tt>0</tt> to <tt>1</tt>.
* <br><br>
* Other common <i>B</i>-spline representation schemes store an extra unused knot at each
* end of the knot array. These extra knot values seem to appear in all the
* <i>B</i>-spline evaluation functions but in reality are never used and are not input to
* the deformable modeling package. Developers moving <i>B</i>-spline data between this
* and other systems must take care to watch out for this mismatch possibility on
* the end knots.
* <br><br>
* Deformable curves have both a current shape and a default shape. The default
* shape is stored as a second array of control points called dof_def. <tt>dof_def</tt> is
* sized and stored in the same manner as <tt>dof_vec</tt>. <tt>end_cond</tt> is a flag which can be
* used to have the deformable modeling package force the two ends of a <i>B</i>-spline
* curve to be continuous with one another. when <tt>end_cond</tt> equals <tt>0</tt> the curve
* remains open, when <tt>end_cond = 1</tt>it will be <tt>C0</tt> across the end points, and when
* <tt>end_cond = 2 </tt>it will be <tt>C1</tt> across the end points.
* <br><br>
* <b>Errors:</b>
*<dl><dt><tt>DM_BAD_IMAGE_DIM_VALUE.</tt><dt>
*<dd> The input value of the image dimension must be positive.</dd>
* <br>
* <dt><tt>DM_BAD_DEGREE_VALUE</tt></dt>
* <dd>The requested spline degrees must be <tt>0</tt> or greater.</dd>
* <br>
* <dt><tt>DM_BAD_KNOT_COUNT_VALUE</tt></dt>
* <dd>The number of knots must be positive.</dd>
* <br>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The knot_index and the <tt>knot_array</tt> pointers cannot be <tt>NULL</tt> on input.</dd>
* <br>
* <dt><tt>DM_BAD_KNOT_TO_CPT_COUNT</tt></dt>
* <dd>The last knot index number must equal <tt>dof_count + degree - 2</tt>.</dd>
* <br>
* <dt><tt>DM_BAD_NTGRL_DEGREE_VALUE</tt></dt>
* <dd>The required gauss integration degree is not supported. Currently limited to the
* range <tt>1</tt> to <tt>79</tt>. The <tt>ntgrl_degree</tt> must be equal to or larger than twice the spline
* degree for valid deformations.</dd>
*</dl>
* <br>
* <b>Effect:</b> Changes model
*<br><br>
* @param rtn_err
* out: 0=success or a negative err code.
* @param image_dim
* in: image space size (2=2d,3=3d).
* @param degree
* in: polynomial degree.
* @param dof_count
* in: control point count.
* @param knot_count
* in: number of distinct knot values.
* @param knot_index
* in: multiple knot count array. sized:[knot_count] specify the multiple knots by setting count[i] = maximum knot index value for location knot[i] Examples: no multiples count = [0,1,2,3,4] some multiples = [1,2,4].
* @param knot
* in: knot value array sized:[knot_count] ordered [u0 &lt; u1 &lt; u2 ... ].
* @param dof_vec
* in: init dof_vec vals or NULL for 0 sized:[image_dim* dof_count] ordered:[xyz0, xyz1, xyz2, ...].
* @param dof_def
* in: init default shape or NULL for 0 sized:[image_dim* dof_count] ordered:[xyz0, xyz1, xyz2, ...].
* @param end_cond
* in: enforce continuity between end points with constraints. If used must have multiple knots on the ends of the <i>B</i>-spline. 0=open (no constraints) 1=closed (C0 continuity) 2=periodic (C1 continuity) default value = 0.
* @param sdmo
* in:SDM_options pointer note: total_knot_count = dof_count+degree-1 the total and distinct knot counts vary when there are multiple knots note: the input array values are only copied. The calling program still needs to manage that memory.
**/
DECL_DM DS_pfunc *         // rtn: ptr to DS_pfunc <i>B</i>-spline curve or NULL, [malloc]
DM_make_bspline_curve      // eff: construct <i>B</i>-spline curve from data
  (int &rtn_err,           // out: 0=success or a negative err code
   int image_dim,          // in : image space size (2=2d,3=3d)
   int degree,             // in : polynomial degree
   int dof_count,          // in : control point count
   int knot_count,         // in : number of distinct knot values
   int *knot_index,        // in : multiple knot count array, [pre-alloced]
                           //      sized:[knot_count]
                           //      specify the multiple knots by
                           //      setting count[i] = maximum knot
                           //      index value for location knot[i]
                           //      Examples:
                           //      no multiples count = [0,1,2,3,4]
                           //      some multiples     = [1,2,4]
   double *knot,           // in : knot value array, [pre-alloced]
                           //      sized:[knot_count]
                           //      ordered [u0 < u1 < u2 ... ]
   double *dof_vec,        // in : init dof_vec vals or NULL for 0, [pre-alloced]
                           //      sized:[image_dim*dof_count]
                           //      ordered:[xyz0,xyz1,xyz2...]
   double *dof_def,        // in : init default shape or NULL for 0, [pre-alloced]
                           //      sized:[image_dim*dof_count]
                           //      ordered:[xyz0,xyz1,xyz2...]
   int end_cond=0,         // in : enforce continuity between end
                           //      points with constraints.  If
                           //      used must have multiple knots on
                           //      the ends of the <i>B</i>-spline.
                           //      0=open (no constraints)
                           //      1=closed (C0 continuity)
                           //      2=periodic (C1 continuity)
    SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: total_knot_count
                           //           = dof_count + degree - 1
                           //       the total and distinct knot
                           //       counts vary when there are
                           //       multiple knots.
                           // note: the input array values are only
                           //       copied. The calling program
                           //       still needs to manage that memory.

//===========================================================
// DS_pfunc *DM_get_bspline_curve
//===========================================================

/**
* Gets <i>B</i>-spline curve data and returns <tt>0</tt> for success or <tt>DM_NOT_A_</tt><i>B</i>-spline.
* <br><br>
* <b>Role:</b> Modifies <tt>image_dim</tt>, <tt>degree</tt>, <tt>dof_count</tt>, <tt>knot_count</tt>, <tt>knot_index</tt>,
* <tt>knot</tt>, <tt>dof_vec</tt>, <tt>dof_def</tt>, <tt>end_cond</tt>, <tt>ntgrl_degree</tt>.
* <br><br>
* This function places copies of the pfunc data into all the output variables. See
* <tt>DS_make_bspline_curve</tt> for argument use. All output arrays are to be allocated by
* the calling program. Arrays of the wrong size will cause memory errors. Any
* array input value of <tt>NULL</tt> will cause this subroutine to skip that output data.
* <br><br>
* This function also updates the output pointer values to point to the pfunc's
* nested data. Do not use these pointer values to free memory, nor after the pfunc
* has been deleted.
* <br>
* <tt>ntgrl_degree</tt> is an internal term used by the deformable modeling package to
* control the accuracy of building the deformable modeling equations. It is the
* <tt>max_degree</tt> polynomial function that is accurately integrated by the package. If
* the value is too small the system will not deform as expected. If the value is
* too large the system slows down. The term is returned for debugging and
* informative purposes only, and its value should always be at least twice the
* degree value. The maximum value is <tt>79</tt>.
* <br><br>
* The <i>B</i>-spline knot values returned in the knot array define the <tt>DS_pfunc</tt>'s domain
* space which is called the <tt>internal_pfunc_space</tt>. When used in a deformable model,
* the <tt>internal_pfunc_space</tt> may be scaled by a constant factor to optimize the
* numerical accuracy of computing high order derivatives. The domain space of the
* deformable model is called the <tt>orig_dmod_space</tt>, and it remains constant for
* input and output purposes. The relationship between the two spaces is always
* just, <tt>internal_pfunc_space = domain_scale * orig_dmod_space</tt>.
* <br><br>
* The <tt>domain_scale</tt> factor may be retrieved with the function,
* <tt>DM_get_dmod_domain_scale()</tt>.
* <br><br>
* <b>
:</b>
*<dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The pfunc cannot be <tt>NULL</tt> on input.</dd>
* <br>
* <dt><tt>DM_NOT_A_BSPLINE</tt></dt>
* <dd>The input pfunc type must be a <i>B</i>-spline.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param pfunc
* in: pfunc data structure to examine.
* @param image_dim
* out: image space size (2=2d, 3=3d).
* @param degree
* out: polynomial degree.
* @param dof_count
* out: control point count.
* @param knot_count
* out: number of distinct knot values.
* @param knot_index
* out: multiple knot count array sized:[knot_count] specify multiple knots by setting count[i]= maximum knot index value for location knot[i] Example: no multiples count = [0,1,2,3,4] some multiples = [1,2,4].
* @param knot
* out: knot value array sized:[knot_count] ordered [u0 &lt; u1 &lt; u2,...] output in internal_pfunc_space.
* @param dof_vec
* out: dof_vec vals (control pt locs) sized: [image_dim*dof_count] ordered [xyz0,xyz1,...].
* @param dof_def
* out: default shape (control pt locs) sized: [image_dim*dof_count] ordered [xyz0,xyz1,...].
* @param end_cond
* out: enforce continuity between end points with constraints. If used must have multiple knots on the ends of <i>B</i>-spline. 0=open default (no constraints) 1=closed (C0 continuity) 2=periodic (C1 continuity).
* @param ntgrl_degree
* out: for deformable modeling - the degree polynomial exactly integrated should be larger than degree. 10 is default. note: total_knot_count = dof_count + deg. - 1 the total and distinct knot counts vary when there are multiple knots. note: any input array set to NULL will be ignored by subroutine.
* @param sdmo
* in:SDM_options pointer note: total_knot_count = dof_count+degree-1 the total and distinct knot counts vary when there are multiple knots note: any input array set to NULL will be ignored by subroutine.
**/
DECL_DM void
DM_get_bspline_curve       // eff: retrieve <i>B</i>-spline curve data
  (int &rtn_err,           // out: 0=success or negative err code
   DS_pfunc *pfunc,        // in : pfunc data structure to examine, [pre-alloced]
   int &image_dim,         // out: image space size (2=2d,3=3d)
   int &degree,            // out: polynomial degree
   int &dof_count,         // out: control point count
   int &knot_count,        // out: number of distinct knot values
   int *&knot_index,       // out: multiple knot count array, [nested]
                           //      sized:[knot_count]
                           //      specify the multiple knots by
                           //      setting count[i] = maximum knot
                           //      index value for location knot[i]
                           //      Examples:
                           //      no multiples count = [0,1,2,3,4]
                           //      some multiples     = [1,2,4]
   double *&knot,          // out: knot value array, [nested]
                           //      sized:[knot_count]
                           //      ordered [u0 < u1 < u2 ... ]
   double *&dof_vec,       // out: dof_vec vals (control pts), [nested]
                           //      sized:[image_dim*dof_count]
                           //      ordered:[xyz0,xyz1,xyz2...]
   double *&dof_def,       // out: default shape (control pts), [nested]
                           //      sized:[image_dim*dof_count]
                           //      ordered:[xyz0,xyz1,xyz2...]
   int &end_cond,          // out: enforce continuity between end
                           //      points with constraints.  If
                           //      used must have multiple knots on
                           //      the ends of the <i>B</i>-spline.
                           //      0=open (no constraints)
                           //      1=closed (C0 continuity)
                           //      2=periodic (C1 continuity)
                           //      default value = 0
   int &ntgrl_degree,      // out: for deformable modeling - the
                           //      degree polynomial exactly integrated
                           //      should be at least twice the degree.
    SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: total_knot_count
                           //           = dof_count + degree - 1
                           //       the total and distinct knot
                           //       counts vary when there are
                           //       multiple knots.
                           // note: any input array set to NULL
                           //       will be ignored by subroutine.

//===========================================================
// DS_pfunc *DM_make_nurb_curve
//===========================================================

/**
* Creates a <tt>NURB</tt> curve from data and returns a pointer to <tt>DS_pfunc</tt>, <tt>NURB</tt> curve or <tt>NULL</tt>.
* <br><br>
* <b>Role:</b> Returns a pointer to the allocated and initialized memory used
* to represent a circular curve as a <tt>DS_pfunc</tt> description and sets the rtn_err as
* described below. Call <tt>DM_delete_pfunc()</tt> on the returned pointer to free its
* memory.
* <br><br>
* Creates and returns a pointer to the <tt>DS_pfunc</tt> <tt>NURB</tt> curve representation.
* <tt>image_dim</tt> is the size of the model space, use <tt>2</tt> for <tt>2D</tt> and <tt>3</tt> for <tt>3D</tt>. degree is
* the degree of <tt>NURB</tt> polynomials. <tt>dof_count</tt> is the number of <tt>NURB</tt> control points.
* <tt>knot_count</tt> is the number of unique values. The <tt>total_count</tt> equals <tt>dof_count +
* degree - 1</tt>. The <tt>total_knot_count</tt> will not equal the distinct <tt>knot_count</tt> when the
* <tt>NURB</tt> has any multiple s. The multiplicity of each is communicated through the
* knot_index array. Each starting with the lowest u value, is assigned a
* <tt>knot_index</tt> starting at <tt>0</tt>. The <tt>knot_index</tt> array records the highest index value
* for all s at each distinct location. For example, a common <tt>NURB</tt> is one with
* multiple s on the end points and single s in the interior. A map of the index
* values might look like,
* <pre>
* +------+------+-------+------+
* 0  3  4  5  6
* 1           7
* 2           8 </pre>
* This knot array corresponds to a degree = 3 <i>B</i>-spline with seven control points
* and four spans. The <tt>NURB</tt> interpolates the first and last control point positions
* due to the multiple end knots.
* <br><br>
* There are nine total knot values and only five distinct knot values. The knot
* index array for this curve would be <tt>[2,3,4,5,8]</tt>. The knot array contains the
* domain space values (<i>u</i> values) for each distinct knot. For the above example,
* the knot array might be something like <tt>[0.00, 0.25, 0.50, 0.75, 1.00]</tt> for an
* even distribution of knot values from <tt>0</tt> to <tt>1</tt>.
* <br><br>
* Other common <tt>NURB</tt> representation schemes store an extra unused knot at each end
* of the knot array. These extra knot values seem to appear in all the <tt>NURB</tt>
* evaluation functions but in reality are never used and are not input to the
* deformable modeling package. Developers moving <tt>NURB</tt> data between this and other
* systems must take care to watch out for this mismatch possibility on the end
* knots.
* <br><br>
* The dof_vec contains the curve's control point locations and is sized
* <tt>[image_dim*dof_count_u*dof_count_v]</tt>. It is ordered by control point, so a two
* dimensional curve with four control points would have a <tt>dof_vec</tt> stored as <tt>[x0,
* y0, x1, y1, x2, y2, x3, y3]</tt>, and a three dimensional curve would be stored as,
* <tt>[x0, y0, z0, x1, y1, z1, x2, y2, z2, x3, y3, z3]</tt>.
* <br><br>
* Deformable curves have both a current shape and a default shape. The default
* shape is stored as a second array of control points called <tt>dof_def</tt>. <tt>dof_def</tt> is
* sized and stored in the same manner as dof_vec. <tt>end_cond</tt> is a flag which can be
* used to have the deformable modeling package force the two ends of a <tt>NURB</tt> curve
* to be continuous with one another. when <tt>end_cond</tt> equals <tt>0</tt> the curve remains open,
* when <tt>end_cond = 1</tt> it will be <tt>C0</tt> across the end points, and when <tt>end_cond = 2</tt> it
* will be <tt>C1</tt> across the end points. Every control point is associated with a
* weight value. These weight values remain constant during deformable model
* sculpting and are input through the weight array which is <tt>sized:[dof_count]</tt> and
* ordered <tt>[w0, w1, ... wn]</tt>. Where <tt>wn</tt> is the weight associated to the <tt>nth</tt> control
* point.
* <br><br>
* <b>Errors:</b>
*<dl>
* <dt><tt>DM_BAD_IMAGE_DIM_VALUE</tt></dt>
* <dd>The input value of the image dimension must be positive.</dd>
* <br><br>
* <dt><tt>DM_BAD_DEGREE_VALUE</tt></dt>
* <dd>The requested spline degrees must be 0 or greater.</dd>
* <br><br>
* <dt><tt>DM_BAD_KNOT_COUNT_VALUE</tt></dt>
* <dd>The number of knots must be positive.</dd>
* <br><br>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* The knot_index, knot, and weight array pointers cannot be NULL on input.
* <br><br>
* <dt><tt>DM_BAD_KNOT_TO_CPT_COUNT</tt></dt>
* The last knot index number must equal dof_count + degree - 2.
* <br><br>
* <dt><tt>DM_BAD_NTGRL_DEGREE_VALUE</tt></dt>
* <dd>The required gauss integration degree is not supported. Currently limited to the
* range 1 to 79. The ntgrl_degree must be equal to or larger than twice the spline
* degree for valid deformations.</dd>
*</dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param image_dim
* in: image space size (2=2d,3=3d).
* @param degree
* in: polynomial degree.
* @param dof_count
* in: control point count.
* @param knot_count
* in: number of distinct knot values.
* @param knot_index
* in: multiple knot count array. [not-nested] sized:[knot_count] specify the multiple knots by setting count[i] = maximum knot index value for location knot[i] Examples: no multiples count = [0,1,2,3,4] some multiples = [1,2,4].
* @param knot
* in: knot value array [not-nested] sized:[knot_count] ordered [u0 &lt; u1 &lt; u2 ... ].
* @param dof_vec
* in: init dof_vec vals or NULL for 0 [not-nested] sized:[image_dim* dof_count] ordered:[xyz0, xyz1, xyz2, ...].
* @param dof_def
* in: init default shape or NULL for 0 [not-nested] sized:[image_dim* dof_count] ordered:[xyz0, xyz1, xyz2, ...].
* @param weight
* in: weight vals for each control pt [not-nested] sized:[dof_count] ordered:[w0, w1, ... wn].
* @param end_cond
* in: enforce continuity between end points with constraints. If used must have multiple knots on the ends of the nurb. 0=open (no constraints) 1=closed (C0 continuity) 2=periodic (C1 continuity) default value = 0.
* @param sdmo
* in:SDM_options pointer note: total_knot_count = dof_count+degree-1 the total and distinct knot counts vary when there are multiple knots note: the input array values are only copied. The calling program still needs to manage that memory.
**/
DECL_DM DS_pfunc *         // rtn: ptr to DS_pfunc nurb curve or NULL, [malloc]
DM_make_nurb_curve         // eff: construct nurb curve from data
  (int &rtn_err,           // out: 0=success or negative err code
   int image_dim,          // in : image space size (2=2d,3=3d)
   int degree,             // in : polynomial degree
   int dof_count,          // in : control point count
   int knot_count,         // in : number of distinct knot values
   int *knot_index,        // in : multiple knot count array, [pre-alloced]
                           //      sized:[knot_count]
                           //      specify the multiple knots by
                           //      setting count[i] = maximum knot
                           //      index value for location knot[i]
                           //      Examples:
                           //      no multiples count = [0,1,2,3,4]
                           //      some multiples     = [1,2,4]
   double *knot,           // in : knot value array, [pre-alloced]
                           //      sized:[knot_count]
                           //      ordered [u0 < u1 < u2 ... ]
   double *dof_vec,        // in : init dof_vec vals or NULL for 0, [pre-alloced]
                           //      sized:[image_dim*dof_count]
                           //      ordered:[xyz0,xyz1,xyz2...]
   double *dof_def,        // in : init default shape or NULL for 0, [pre-alloced]
                           //      sized:[image_dim*dof_count]
                           //      ordered:[xyz0,xyz1,xyz2...]
   double *weight,         // in : weight vals for each control pt, [pre-alloced]
                           //      sized:[dof_count]
                           //      ordered:[w0, w1,.. wn]
   int end_cond=0,         // in : enforce continuity between end
                           //      points with constraints.  If
                           //      used must have multiple knots on
                           //      the ends of the nurb.
                           //      0=open (no constraints)
                           //      1=closed (C0 continuity)
                           //      2=periodic (C1 continuity)
    SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: total_knot_count
                           //           = dof_count + degree - 1
                           //       the total and distinct knot
                           //       counts vary when there are
                           //       multiple knots.
                           // note: the input array values are only
                           //       copied. The calling program
                           //       still needs to manage that memory.

//===========================================================
// DS_pfunc *DM_get_nurb_curve
//===========================================================

/**
* Gets <tt>NURB</tt> curve data and returns 0 for success or <tt>DM_NOT_A_NURB</tt>.
* <br><br>
* <b>Role:</b> Modifies <tt>image_dim</tt>, <tt>degree</tt>, <tt>dof_count</tt>, <tt>knot_count</tt>, <tt>knot_index</tt>,
* <tt>knot</tt>, <tt>dof_vec</tt>, <tt>dof_def</tt>, <tt>weight</tt>, <tt>end_cond</tt>, <tt>ntgrl_degree</tt>.
* <br><br>
* Updates the output pointer values to point to the pfunc's nested data. Do not
* use these pointer values to free memory, nor after the pfunc has been deleted.
* <br><br>
* <tt>ntgrl_degree</tt> is an internal term used by the deformable modeling package to
* control the accuracy of building the deformable modeling equations. It is the
* <tt>max_degree</tt> polynomial function that is accurately integrated by the package. If
* the value is too small the system will not deform as expected. If the value is
* too large the system slows down. The term is returned for debugging and
* informative purposes only, and its value should always be at least twice the
* degree value. The maximum value is <tt>79</tt>.
* <br><br>
* The knot values returned in the knot array define the <tt>DS_pfunc</tt>'s domain space
* which is called the <tt>internal_pfunc_space</tt>. When used in a deformable model, the
* <tt>internal_pfunc_space</tt> may be scaled by a constant factor to optimize the
* numerical accuracy of computing high order derivatives. The domain space of the
* deformable model is called the <tt>orig_dmod_space</tt>, and it remains constant for
* input and output purposes. The relationship between the two spaces is always
* just, <tt>internal_pfunc_space = domain_scale * orig_dmod_space</tt>.
* <br><br>
* The <tt>domain_scale</tt> factor may be retrieved with the function,
* <tt>DM_get_dmod_domain_scale()</tt>.
* <br><br>
* <b>Errors:</b><dl> <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The deformable model cannot be NULL on entry.</dd>
* <dt><tt>DM_NOT_A_NURB</tt></dt>
* <dd>The input pfunc type must be a <tt>NURB</tt> curve.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param pfunc
* in: pfunc data structure to examine.
* @param image_dim
* out: image space size (2=2d,3=3d).
* @param degree
* out: polynomial degree.
* @param dof_count
* out: control point count.
* @param knot_count
* out: number of distinct knot values.
* @param knot_index
* out: multiple knot count array. sized:[knot_count] specify the multiple knots by setting count[i] = maximum knot index value for location knot[i] Examples: no multiples count = [0,1,2,3,4] some multiples = [1,2,4].
* @param knot
* out: knot value array sized:[knot_count] ordered [u0 &lt; u1 &lt; u2...] output in internal_pfunc_space.
* @param dof_vec
* out: dof_vec vals (control pt locs) sized:[image_dim* dof_count] ordered:[xyz0, xyz1, xyz2, ...].
* @param dof_def
* out: default shape (control pt locs) sized:[image_dim* dof_count] ordered:[xyz0, xyz1, xyz2, ...].
* @param weight
* in: weight vals for each control pt sized:[dof_count] ordered:[w0, w1, ... wn].
* @param end_cond
* out: enforce continuity between end points with constraints. If used must have multiple knots on the ends of the <i>B</i>-spline. 0=open (no constraints) 1=closed (C0 continuity) 2=periodic (C1 continuity) default value = 0.
* @param ntgrl_degree
* out: for deformable modeling - the degree polynomial exactly integrated should be larger than the degree. 10 is a good value. default value = 10.
* @param sdmo
* in:SDM_options pointer   note:total_knot_count =dof_count+degree-1 the total and distinct knot counts vary when there are multiple knots. note: any input array set to NULL will be ignored by subroutine.
**/
DECL_DM void
DM_get_nurb_curve          // eff: retrieve nurb curve data
  (int &rtn_err,           // out: 0=success or neg err code
   DS_pfunc *pfunc,        // in : pfunc data structure to examine, [pre-alloced]
   int &image_dim,         // out: image space size (2=2d,3=3d)
   int &degree,            // out: polynomial degree
   int &dof_count,         // out: control point count
   int &knot_count,        // out: number of distinct knot values
   int *&knot_index,       // out: multiple knot count array, [nested]
                           //      sized:[knot_count]
                           //      specify the multiple knots by
                           //      setting count[i] = maximum knot
                           //      index value for location knot[i]
                           //      Examples:
                           //      no multiples count = [0,1,2,3,4]
                           //      some multiples     = [1,2,4]
   double *&knot,          // out: knot value array, [nested]
                           //      sized:[knot_count]
                           //      ordered [u0 < u1 < u2 ... ]
   double *&dof_vec,       // out: dof_vec vals (control pt locs), [nested]
                           //      sized:[image_dim*dof_count]
                           //      ordered:[xyz0,xyz1,xyz2...]
   double *&dof_def,       // out: default shape (control pt locs), [nested]
                           //      sized:[image_dim*dof_count]
                           //      ordered:[xyz0,xyz1,xyz2...]
   double *&weight,        // out: weight vals for each control pt, [nested]
                           //      sized:[dof_count]
                           //      ordered:[w0, w1,.. wn]
   int &end_cond,          // out: enforce continuity between end
                           //      points with constraints.  If
                           //      used must have multiple knots on
                           //      the ends of the <i>B</i>-spline.
                           //      0=open (no constraints)
                           //      1=closed (C0 continuity)
                           //      2=periodic (C1 continuity)
                           //      default value = 0
   int &ntgrl_degree,      // out: for deformable modeling - the
                           //      degree polynomial exactly integrated
                           //      should be at least twice the degree.
    SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: total_knot_count
                           //           = dof_count + degree - 1
                           //       the total and distinct knot
                           //       counts vary when there are
                           //       multiple knots.
                           // note: any input array set to NULL
                           //       will be ignored by subroutine.

//===================================================
// DS_pfunc *DM_make_circ_curve
//===================================================

/**
* Creates a circular curve and returns a pointer to <tt>DS_pfunc</tt> circular curve or <tt>NULL</tt>.
* <br><br>
* <b>Role:</b> This function returns a pointer to the allocated and initialized
* memory used to represent a circular curve as a <tt>DS_pfunc</tt> description and sets the
* rtn_err as described below. Call <tt>DM_delete_pfunc()</tt> on the returned pointer to
* free its memory.
* <br><br>
* The circular curve represents complete or arc fragments of circles and ellipses
* in the form used by the deformable modeling package.
* <br><br>
* <tt>W = W(s) = xc + xa*cos(s) + xb*sin(s)</tt>
* <tt>0 &lt;= u_min &lt;= s &lt;= u_max &lt;= 2 pi</tt>
* <br><br>
* The curve has three dofs including
* <br><br>
* <tt>xc =</tt> the center-point of the curve,<br>
* <tt>xa =</tt> the vector from the center-point to the end-point of axis <tt>1</tt>,<br>
* <tt>xb = </tt>the vector from the center-point to the end-point of axis <tt>2</tt>,
* <br><br>
* Note that <tt>xa</tt> and xb are <tt>2D</tt> vectors and not <tt>2D</tt> points, i.e., the end points of
* the ellipse vectors are computed by:
* <br><br>
* <tt>end_point_axis_1 = xc + xa</tt>, and
* <tt>end_point_axis_2 = xc + xb</tt>.
* <br><br>
* When
* <br><br>
* <tt>xa = {r, 0} and xb = {0, r}</tt> we get a circle
* <br><br>
* <tt>xa = {r1,0} and xb = {0,r2}</tt> we get an ellipse.
* <br><br>
* When the vectors xa and xb equal each other the circular curve degenerates into
* a double line segment. This will not bother the deformable modeling package but
* may confuse the end user and is therefore prohibited. <tt>image_dim</tt> is the size of
* the model space. Use <tt>2</tt> for <tt>2D</tt> and <tt>3</tt> for <tt>3D</tt>.
* <br><br>
* When <tt>u_min</tt> equals <tt>0</tt> and <tt>u_max</tt> equals <tt>2*DS_PI</tt>, the circle or ellipse is a
* complete closed curve. Any other values for <tt>u_min</tt> and <tt>u_max</tt> make a circular or
* elliptical arc fragment.
* <br><br>
* <tt>dof_vec</tt> is an array of <tt>[xc,xa,xb]</tt>. It is sized <tt>3*image_dim</tt>, where a <tt>2D</tt> circ
* dof_vec <tt>array</tt> is organized as <tt>[Xxc, Yxc, Xxa, Yxa, Xxb, Yxb]</tt>.
* <br><br>
* <tt>mallocs</tt> is the calling program needs to call <tt>DM_delete_pfunc</tt> on the returned
* pointer. The new data structure does not reference the input arrays after this
* call.
* <br><br>
* Returns pointer to <tt>DS_pfunc</tt> when successful or <tt>NULL</tt> when a problem is
* encountered in the input.
* <br><br>
* <b>Errors:</b>
* <dl><dt><tt>DM_BAD_IMAGE_DIM_VALUE</tt></dt>
* <dd>The input value of the image dimension must be positive.</dd>
* <br><br>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The <tt>dof_vec pointer</tt> cannot be NULL on input.</dd>
* <br><br>
* <dt><tt>DM_BAD_ELEM_COUNT_VALUE</tt></dt>
* <dd>The <tt>elem_count</tt> must be greater than <tt>DS_DEFAULT_ELEM_COUNT</tt>.</dd>
* <br><br>
* <dt><tt>DM_BAD_NTGRL_DEGREE_VALUE</tt></dt>
* <dd>The required gauss integration degree is not supported. Currently limited to the
* range <tt>1</tt> to <tt>79</tt>. The <tt>ntgrl_degree</tt> must be equal to or larger than the
* <tt>DS_DEFAULT_NTGRL_DEGREE</tt> for valid deformations.</dd>
* <br><br>
* <dt><tt>DM_BAD_CIRC_SHAPE</tt></dt>
* <dd>The xa and xb vectors cannot be parallel or of zero length which would create a
* circular curve with no internal area.</dd>
* <br><br>
* <dt><tt>DM_BAD_CIRC_PARAM_VALUE</tt></dt>
* <dd>Input <tt>u_min</tt> and <tt>u_max</tt> values must be in the range of <tt>0</tt> to <tt>2*DS_PI</tt>.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* rtn: ptr to DS_pfunc circ curve or NULL eff: make crv W=xc+xa*cos(s) +xb*sin(s) with 0 &lt;= s &lt;= 2 pi out: 0=success or negative err code.
* @param image_dim
* in: image space size (2=2d,3=3d).
* @param dof_vec
* in: the 3 dofs [xc,xa,xb]. [not-nested] xc = center_point xa = vec from center_pt to axis_1 xb = vec from center_pt to axis_2 Sets dof_def = dof_vec sized:[3*image_dim].
* @param u_min
* in: domain min 0 &lt;= u_min &lt;= 2*DS_PI.
* @param u_max
* in: domain max 0 &lt;= u_max &lt;= 2*DS_PI note: the input array values are only copied. The calling program still needs to manage that memory.
* @param sdmo
* in: SDM_options pointer  note: the input array values are only copied. The calling program still needs to manage that memory.
**/
DECL_DM DS_pfunc *         // rtn: ptr to DS_pfunc circ curve or NULL, [malloc].
DM_make_circ_curve         // eff: make crv W=xc+xa*cos(s)+xb*sin(s)
                           //      with 0 <= s <= 2PI
  (int &rtn_err,           // out: 0=success or negative err code
   int image_dim,          // in : image space size (2=2d,3=3d)
   double *dof_vec,        // in : the 3 dofs [xc,xa,xb], [pre-alloced]
                           //      xc = center_point
                           //      xa = vec from center_pt to axis_1
                           //      xb = vec from center_pt to axis_2
                           //      Sets dof_def = dof_vec ;
                           //      sized:[3*image_dim]
   double u_min=0.0,       // in : domain min 0 <= u_min <= 2*DS_PI
   double u_max=2*PI,      // in : domain max 0 <= u_max <= 2*DS_PI
    SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: the input array values are only
                           //       copied. The calling program
                           //       still needs to manage that memory.
                           // note: the input array values are only
                           //       copied. The calling program
                           //       still needs to manage that memory.

//===================================================
// DS_pfunc *DM_get_circ_curve
//===================================================
/**
* Retrieves circular curve data and returns <tt>0</tt> for success or <tt>DM_NOT_A_CIRC</tt>.
* <br><br>
* <b>Role:</b> Modifies <tt>image_dim</tt>, <tt>dof_vec</tt>, <tt>dof_def</tt>, <tt>elem_count</tt>, <tt>ntgrl_degree</tt>.
* <br><br>
* Loads the defining data for a circular curve into the output variables. See
* <tt>DM_make_circ_curve()</tt> for the definition of a circular curve and the use of its
* defining arguments.
* <br><br>
* This function also updates the output pointer values to point to the pfunc's
* nested data. Do not use these pointer values to free memory, nor use these
* values after the pfunc has been deleted.
* <br><br>
* <tt>elem_count</tt> is the number of elements that a circular curve is subdivided into to
* improve the accuracy of numerical integration. This value should be at least as
* large as <tt>DS_DEFAULT_ELEM_COUNT</tt>. It is returned for debugging and informative
* reasons only.
* <br><br>
* <tt>ntgrl_degree</tt> is an internal term used by the deformable modeling package to
* control the accuracy of building the deformable modeling equations. It is the
* <tt>max_degree</tt> polynomial function that is accurately integrated by the package. If
* the value is too small the system will not deform as expected. If the value is
* too large the system slows down. The term is returned for debugging and
* informative purposes only, and its value should always be at least twice the
* degree value. The maximum value is <tt>79</tt>.
* <br><br>
* <b>Errors:</b>
* <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The pfunc cannot be NULL on input.</dd>
* <br><br>
* <dt><tt>DM_NOT_A_CIRC</tt></dt>
* <dd>The pfunc type must be a circ.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param pfunc
* in: pfunc data structure to examine.
* @param image_dim
* out: image space size (2=2d,3=3d).
* @param dof_vec
* out: the 3 current dofs [xc,xa,xb] xc = center_point xa = vec from center_pt to axis_1 xb = vec from center_pt to axis_2 Sets dof_def = dof_vec; Sized:[3*image_dim] (required).
* @param dof_def
* out: the 3 default dofs [xc,xa,xb] xc = center_point xa = vec from center_pt to axis_1 xb = vec from center_pt to axis_2 Sets dof_def = dof_vec; Sized:[3*image_dim] (required).
* @param elem_count
* out: break circ into elems for integration default value = 8.
* @param ntgrl_degree
* out: for deformable modeling - the degree polynomial exactly integrated. 10 is a good value. default value = 10. note: any input array set to NULL will be ignored by subroutine.
* @param sdmo
* in:SDM_options pointer note: any input array set to NULL will be ignored by subroutine.
**/
DECL_DM void
DM_get_circ_curve          // eff: retrieve circ curve data
  (int &rtn_err,           // out: 0=success or neg err code
   DS_pfunc *pfunc,        // in : pfunc data structure to examine, [pre-alloced]
   int &image_dim,         // out: image space size (2=2d,3=3d)
   double *&dof_vec,       // out: the 3 current dofs [xc,xa,xb], [nested]
                           //      xc = center_point
                           //      xa = vec from center_pt to axis_1
                           //      xb = vec from center_pt to axis_2
                           //      Sets dof_def = dof_vec ;
                           //      sized:[3*image_dim] (required)
   double *&dof_def,       // out: the 3 default dofs [xc,xa,xb], [nested]
                           //      xc = center_point
                           //      xa = vec from center_pt to axis_1
                           //      xb = vec from center_pt to axis_2
                           //      Sets dof_def = dof_vec ;
                           //      sized:[3*image_dim] (required)
   int &elem_count,        // out: break circ into elems for integration
                           //      default value = 8.
   int &ntgrl_degree,       // out: for deformable modeling - the
                           //      degree polynomial exactly integrated.
    SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: any input array set to NULL
                           //       will be ignored by subroutine.

//===========================================================
// DS_pfunc *DM_make_bspline_surface
//===========================================================
/**
* Creates a <i>B</i>-spline surface from data and returns a pointer to <tt>DS_pfunc</tt> <i>B</i>-spline surf or <tt>NULL</tt>.
* <br><br>
* <b>Role:</b> Modifies <tt>return_err</tt>.
* <br><br>
* Creates and returns a pointer to <tt>DS_pfunc</tt> <i>B</i>-spline surface representation. A
* <i>B</i>-spline surface is the tensor product of the underlying <i>B</i>-spline <i><i>u</i>-</i>curve and
* <i>v-</i>curve basis functions. The total number of dofs in the surface is just the
* product of the dof counts for <i>u</i> and <i>v</i> curve basis functions. The global dof
* numbers for each dof are the cross product of the <i>u</i>-curve and <i>v</i>-curve dof
* numbers with the <i>v</i>-curve dof numbers varying the fastest. This function's input
* arguments contain enough information to make both the <i>u</i>-curve and the <i>v</i>-curve
* basis functions as well as the dof values for the surface.
* <br><br>
* <tt>image_dim</tt> is the size of the model space, using <tt>2</tt> for <tt>2D</tt> and <tt>3</tt> for <tt>3D</tt>. <tt>degree_u</tt>
* and <tt>degree_v</tt> are the <i>B</i>-spline polynomial degrees in the surface's <i>u</i> and <i>v</i>
* directions. <tt>dof_count_u</tt> and <tt>dof_count_v</tt> are the number of <i>B</i>-spline control
* points in the surface's <i>u</i> and <i>v</i> directions. <tt>dof_count_u</tt> and <tt>dof_count_v</tt> are the
* number of <i>B</i>-spline control points in the surface's <i>u</i> and <i>v</i> directions. The
* <tt>knot_count_u</tt> and <tt>knot_count_v</tt> are the number of unique knot values in the
* surface's <i>u</i> and <i>v</i> directions.
* <br><br>
* The <tt>total_knot_count</tt> equals the <tt>dof_count + degree - 1</tt> for each <i>u</i> and <i>v</i> surface
* direction. The <tt>total_knot_count</tt> will not equal the distinct <tt>knot_count</tt> when the
* <tt>NURB</tt> has any multiple knots. The multiplicity of each knot is communicated
* through the <tt>knot_index</tt> array. Each knot, starting with the lowest <i>u</i> value knot,
* is assigned a <tt>knot_index</tt> starting at <tt>0</tt>. The <tt>knot_index</tt> array records the highest
* index value for all knots at each distinct knot location. For example, a common
* <i>B</i>-spline is one with multiple knots on the end points and single knots in the
* interior. A map of the knot index values might look like:
* <pre>
* +------+------+-------+------+
* 0  3  4  5  6
* 1           7
* 2           8 </pre>
* This knot array corresponds to a <tt>degree = 3</tt> <i>B</i>-spline with seven control points
* and four spans. The <i>B</i>-spline interpolates the first and last control point
* positions due to the multiple end knots.
* <br><br>
* There are nine total knot values and only five distinct knot values. The knot
* index array for this curve would be <tt>[2,3,4,5,8]</tt>. The knot array contains the
* domain space values (<i>u</i> values) for each distinct knot. For the above example,
* the knot array might be something like <tt>[0.00, 0.25, 0.50, 0.75, 1.00]</tt> for an
* even distribution of knot values from <tt>0</tt> to <tt>1</tt>.
* <br><br>
* Other common <i>B</i>-spline representation schemes store an extra unused knot at each
* end of the knot array. These extra knot values seem to appear in all the
* <i>B</i>-spline evaluation functions but in reality are never used and are not input to
* the deformable modeling package. Developers moving <i>B</i>-spline data between this
* and other systems must take care to watch out for this mismatch possibility on
* the end knots.
* <br><br>
* The <tt>dof_vec</tt> contains the surface's control point locations and is sized
* <tt>[image_dim*dof_count_u*dof_count_v]</tt>. It is ordered by control point, so a two
* dimensional surface with a <tt>2x2</tt> grid of control points will be organized as:
* <pre>
* dof01  dof11  Global dof numbers
* +----------+   dof0 = dof00
* |   |   dof1 = dof01
* |   |   dof2 = dof10
* |   |   dof3 = dof11
* +----------+
* dof00 dof10
* </pre>
* and will have a <tt>dof_vec</tt> stored as <tt>[x0, y0, x1, y1, x2, y2, x3, y3]</tt>, and the same
* surface in <tt>3d</tt> would be stored as, <tt>[x0, y0, z0, x1, y1, z1, x2, y2, z2, x3, y3,
* z3]</tt>. Deformable surfaces have both a current shape and a default shape. The
* default shape is stored as a second array of control points called <tt>dof_def</tt>.
* <tt>dof_def</tt> is sized and stored in the same manner as <tt>dof_vec</tt>.
* <br><br>
* Surfaces can have three different end conditions applied to them through the use
* of constraints. These include an <tt>end_cond</tt> which can force opposite edges of the
* surface square to connect to each other with either a <tt>C0</tt> or a <tt>C1</tt> continuity, and
* a singularity case which can collapse an entire boundary edge into a single
* point. These end conditions are communicated through the use of four flags,
* <tt>end_cond_u</tt>, <tt>end_cond_v</tt>, <tt>singular_u</tt>, and <tt>singular_v</tt>. The connectivity of the <i>u</i>
* and <i>v</i> boundary curves are set independently by the <tt>end_cond_u</tt> and <tt>end_cond_v</tt>
* flag values. When end condition equals <tt>0</tt> the surface remains open in that
* direction. When end condition equals <tt>1</tt> it will be <tt>C0</tt> across the end curves. And
* when end condition equals <tt>2</tt> it will be <tt>C1</tt> across the end curves. The singularity
* of the <i>u</i> and <i>v</i> boundaries are set independently by the <tt>singular_u</tt> and <tt>singular_v</tt>
* flag values. When singular equals <tt>0</tt> all edges are normal. When singular equals <tt>1</tt>
* the low end boundary curve is collapsed to a point. When singular equals <tt>2</tt> the
* high end boundary curve is collapsed to a point. When singular equals <tt>3</tt> both low
* and high end boundary curves are collapsed to a single point.
* <br><br>
* Returns a pointer to <tt>DS_pfunc</tt> when successful or <tt>NULL</tt> when a problem is
* encountered in the input.
* <br><br>
* <tt>mallocs</tt>: The calling program needs to call <tt>DM_delete_pfunc</tt> on the returned
* pointer. The new data structure does not reference the input arrays after this
* call.
* <br>
* <b>Errors:</b>
*<dl>
* <dt><tt>DM_BAD_IMAGE_DIM_VALUE</tt></dt>
* <dd>The input value of the image dimension must be positive.</dd>
* <dt><tt>DM_BAD_DEGREE_VALUE</tt></dt>
* <dd>The requested spline degrees must be <tt>0</tt> or greater.</dd>
* <dt><tt>DM_BAD_KNOT_COUNT_VALUE</tt></dt>
* <dd>The number of knots must be positive.</dd>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The knot_index and the <tt>knot_array</tt> pointers cannot be <tt>NULL</tt> on input.</dd>
* <dt><tt>DM_BAD_KNOT_TO_CPT_COUNT</tt></dt>
* <dd>The last knot index number must equal <tt>dof_count + degree - 2</tt>.</dd>
* <dt><tt>DM_BAD_END_COND_VALUE</tt></dt>
* <dd>The end condition values must be <tt>0</tt>, <tt>1</tt>, or <tt>2</tt>.</dd>
* <dt><tt>DM_BAD_SINGULAR_VALUE</tt></dt>
* <dd>The singular values must be <tt>0</tt>, <tt>1</tt>, <tt>2</tt>, or <tt>3</tt>.</dd>
* <dt><tt>DM_BAD_NTGRL_DEGREE_VALUE</tt></dt>
* <dd>The required gauss integration degree is not supported. Currently limited to the
* range <tt>1</tt> to <tt>79</tt>. The <tt>ntgrl_degree</tt> must be equal to or larger than twice the spline
* degree for valid deformations.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param image_dim
* in: image space size (2=2d,3=3d).
* @param degree_u
* in: u dir polynomial degree.
* @param dof_count_u
* in: u dir control point count.
* @param knot_count_u
* in: u dir distinct knot value count.
* @param knot_index_u
* in: u dir multiple knot count array. sized:[knot_count] specify the multiple knots by setting count[i] = maximum knot index value for location knot[i] Examples: no multiples count = [0,1,2,3,4] some multiples = [1,2,4].
* @param knot_u
* in: u dir knot values sized:[knot_count] ordered [u0 &lt; u1 &lt; u2...].
* @param degree_v
* in: v dir polynomial degree.
* @param dof_count_v
* in: v dir control point count.
* @param knot_count_v
* in: v dir distinct knot value count.
* @param knot_index_v
* in: v dir multiple knot count array. sized:[knot_count] specify the multiple knots by setting count[i] = maximum knot index value for location knot[i] Examples: no multiples count = [0,1,2,3,4] some multiples = [1,2,4].
* @param knot_v
* in: v dir knot value array sized:[knot_count] ordered [u0 &lt; u1 &lt; u2...].
* @param dof_vec
* in: init dof_vec vals or NULL for 0 sized:[image_dim* dof_count_u* dof_count_v] ordered:[C00, C01, ... C0m, C10, ... Cnm].
* @param dof_def
* in: init default shape or NULL for 0 ordered:[C00, C01, ... C0m, C10, ... Cnm].
* @param end_cond_u
* in: one of 0=open or 1=closed or 2=periodic.
* @param singular_u
* in: one of 0=none or 1=low or 2=high or 3=both.
* @param end_cond_v
* in: one of 0=open or 1=closed or 2=periodic.
* @param singular_v
* in: one of 0=none or 1=low or 2=high or 3=both.
* @param sdmo
* in:SDM_options pointer note: total_knot_count = dof_count+degree-1 the total and distinct knot counts vary when there are multiple knots note: the input array values are only copied. The calling program still needs to manage that memory.
**/
DECL_DM DS_pfunc *         // rtn: ptr to DS_pfunc <i>B</i>-spline surf or NULL, [malloc]
DM_make_bspline_surface    // eff: construct <i>B</i>-spline surf from data
  (int &rtn_err,           // out: 0=success or negative err code
   int image_dim,          // in : image space size (2=2d,3=3d)
   int degree_u,           // in : u dir polynomial degree
   int dof_count_u,        // in : u dir control point count
   int knot_count_u,       // in : u dir distinct knot value count
   int *knot_index_u,      // in : u dir multiple knot count array, [pre-alloced]
                           //      sized:[knot_count_u]
                           //      specify the multiple knots by
                           //      setting count[i] = maximum knot
                           //      index value for location knot[i]
                           //      Examples:
                           //      no multiples count = [0,1,2,3,4]
                           //      some multiples     = [1,2,4]
   double *knot_u,         // in : u dir knot value array, [pre-alloced]
                           //      sized:[knot_count_u]
                           //      ordered [u0 < u1 < u2 ... ]
   int degree_v,           // in : v dir polynomial degree
   int dof_count_v,        // in : v dir control point count
   int knot_count_v,       // in : v dir distinct knot value count
   int *knot_index_v,      // in : v dir multiple knot count array, [pre-alloced]
                           //      sized:[knot_count_v]
                           //      specify the multiple knots by
                           //      setting count[i] = maximum knot
                           //      index value for location knot[i]
                           //      Examples:
                           //      no multiples count = [0,1,2,3,4]
                           //      some multiples     = [1,2,4]
   double *knot_v,         // in : v dir knot value array, [pre-alloced]
                           //      sized:[knot_count_v]
                           //      ordered [u0 < u1 < u2 ... ]
   double *dof_vec,        // in : init dof_vec vals or NULL for 0, [pre-alloced]
                           //      sized:[image_dim*dof_count_u*dof_count_v]
                           //      ordered:[C00,C01,..C0m,
                           //               C10,.. Cnm]
   double *dof_def,        // in : init default shape or NULL for 0, [pre-alloced]
                           //      sized:[image_dim*dof_count_u*dof_count_v]
                           //      ordered:[C00,C01,..C0m,C10,..Cnm]
   int end_cond_u=0,       // in : oneof 0=open|1=closed|2=periodic
   int singular_u=0,       // in : oneof 0=none|1=low|2=high|3=both
   int end_cond_v=0,       // in : oneof 0=open|1=closed|2=periodic
   int singular_v=0,       // in : oneof 0=none|1=low|2=high|3=both
    SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: total_knot_count
                           //           = dof_count + degree - 1
                           //       the total and distinct knot
                           //       counts vary when there are
                           //       multiple knots.
                           // note: the input array values are only
                           //       copied. The calling program
                           //       still needs to manage that memory.

//===========================================================
// DS_pfunc *DM_get_bspline_surface
//===========================================================

/**
* Retrieves <i>B</i>-spline surface data and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies <tt>image_dim</tt>, <tt>dof_vec</tt>, <tt>dof_def</tt>, <tt>ntgrl_degree</tt>, <tt>degree_u</tt>,
* <tt>dof_count_u</tt>,<tt>knot_count_u</tt>,<tt>knot_index_u</tt>,<tt>knot_u</tt>, <tt>degree_v</tt>, <tt>dof_count_v</tt>, <tt>knot_count_v</tt>,
* <tt>knot_index_v</tt>, <tt>knot_v</tt>, <tt>end_cond_u</tt>, <tt>singular_u</tt>, <tt>end_cond_v</tt>, <tt>singular_v</tt>.
* <br><br>
* This function places copies of the pfunc data into all the output variables. See
* <tt>DS_make_bspline_curve</tt> for argument use. All output arrays are to be allocated by
* the calling program. Arrays of the wrong size will cause memory errors. Any
* array input value of <tt>NULL</tt> will cause this subroutine to skip that output data.
* <br><br>
* This function also updates the output pointer values to point to the pfunc's
* nested data. Do not use these pointer values to free memory, nor after the pfunc
* has been deleted.
* <br><br>
* <tt>ntgrl_degree</tt> is an internal term used by the deformable modeling package to
* control the accuracy of building the deformable modeling equations. It is the
* <tt>max_degree</tt> polynomial function that is accurately integrated by the package. If
* the value is too small the system will not deform as expected. If the value is
* too large the system slows down. The term is returned for debugging and
* informative purposes only, and its value should always be at least twice the
* degree value. The maximum value is <tt>79</tt>.
* <br><br>
* The <i>B</i>-spline knot values returned in the knot array define the <tt>DS_pfunc</tt>'s domain
* space which is called the <tt>internal_pfunc_space</tt>. When used in a deformable model,
* the internal_pfunc_space may be scaled by a constant factor to optimize the
* numerical accuracy of computing high order derivatives. The domain space of the
* deformable model is called the <tt>orig_dmod_space</tt>, and it remains constant for
* input and output purposes. The relationship between the two spaces is always
* just, <tt>internal_pfunc_space = domain_scale * orig_dmod_space</tt>.
* <br><br>
* The <tt>domain_scale</tt> factor may be retrieved with the function,
* <tt>DM_get_dmod_domain_scale()</tt>.
* <br><br>
* <b>Errors:</b>
*<dl>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The pfunc cannot be NULL on input.</dd>
* <dt><tt>DM_NOT_A_BSPLINE</tt></dt>
* <dd> The input pfunc type must be a <i>B</i>-spline.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param pfunc
* in: pfunc data structure to examine.
* @param image_dim
* out: image space size (2=2d,3=3d).
* @param degree_u
* out: u dir polynomial degree.
* @param dof_count_u
* out: u dir control point count.
* @param knot_count_u
* out: u dir number of distinct knot values.
* @param knot_index_u
* out: u dir multiple knot count array. sized:[knot_count] specify the multiple knots by setting count[i] = maximum knot index value for location knot[i] Examples: no multiples count = [0,1,2,3,4] some multiples = [1,2,4].
* @param knot_u
* out: u dir knot value array sized:[knot_count] ordered [u0 &lt; u1 &lt; u2...] output in internal_pfunc_space.
* @param degree_v
* out: v dir polynomial degree.
* @param dof_count_v
* out: v dir control point count.
* @param knot_count_v
* out: v dir number of distinct knot values.
* @param knot_index_v
* out: v dir multiple knot count array. sized:[knot_count] specify the multiple knots by setting count[i] = maximum knot index value for location knot[i] Examples: no multiples count = [0,1,2,3,4] some multiples = [1,2,4].
* @param knot_v
* out: v dir knot value array sized:[knot_count] ordered [u0 &lt; u1 &lt; u2...] output in internal_pfunc_space.
* @param dof_vec
* out: dof_vec vals (control pt locs) sized:[image_dim* dof_count] ordered:[xyz0, xyz1, xyz2, ...].
* @param dof_def
* out: default shape (control pt locs) sized:[image_dim* dof_count] ordered:[xyz0, xyz1, xyz2, ...].
* @param end_cond_u
* out: one of 0=open or 1=closed or 2=periodic.
* @param singular_u
* out: one of 0=none or 1=low or 2=high or 3=both.
* @param end_cond_v
* out: one of 0=open or 1=closed or 2=periodic.
* @param singular_v
* out: one of 0=none or 1=low or 2=high or 3=both.
* @param ntgrl_degree
* out: for deformable modeling - the degree polynomial exactly integrated should be larger than the degree. 10 is a good value. default value = 10 note: total_knot_count = dof_count + degree - 1 the total and distinct knot counts vary when there are multiple knots. note: any input array set to NULL will be ignored by subroutine.
* @param sdmo
* in:SDM_options pointer note: total_knot_count = dof_count+degree-1 the total and distinct knot counts vary when there are multiple knots note: any input array set to NULL will be ignored by  subroutine.
**/
DECL_DM void
DM_get_bspline_surface     // eff: retrieve <i>B</i>-spline surface data
  (int &rtn_err,           // out: 0=success or neg err code
   DS_pfunc *pfunc,        // in : pfunc data structure to examine, [pre-alloced]
   int &image_dim,         // out: image space size (2=2d,3=3d)
   int &degree_u,          // out: u dir polynomial degree
   int &dof_count_u,       // out: u dir control point count
   int &knot_count_u,      // out: u dir number of distinct knot values
   int *&knot_index_u,     // out: u dir multiple knot count array, [nested]
                           //      sized:[knot_count_u]
                           //      specify the multiple knots by
                           //      setting count[i] = maximum knot
                           //      index value for location knot[i]
                           //      Examples:
                           //      no multiples count = [0,1,2,3,4]
                           //      some multiples     = [1,2,4]
   double *&knot_u,        // out: u dir knot value array, [nested]
                           //      sized:[knot_count_u]
                           //      ordered [u0 < u1 < u2 ... ]
   int &degree_v,          // out: v dir polynomial degree
   int &dof_count_v,       // out: v dir control point count
   int &knot_count_v,      // out: v dir number of distinct knot values
   int *&knot_index_v,     // out: v dir multiple knot count array, [nested]
                           //      sized:[knot_count_v]
                           //      specify the multiple knots by
                           //      setting count[i] = maximum knot
                           //      index value for location knot[i]
                           //      Examples:
                           //      no multiples count = [0,1,2,3,4]
                           //      some multiples     = [1,2,4]
   double *&knot_v,        // out: v dir knot value array, [nested]
                           //      sized:[knot_count_v]
                           //      ordered [u0 < u1 < u2 ... ]
   double *&dof_vec,       // out: dof_vec vals (control pt locs), [nested]
                           //      sized:[image_dim*dof_count_u*dof_count_v]
                           //      ordered:[xyz0,xyz1,xyz2...]
   double *&dof_def,       // out: default shape (control pt locs), [nested]
                           //      sized:[image_dim*dof_count_u*dof_count_v]
                           //      ordered:[xyz0,xyz1,xyz2...]
   int &end_cond_u,        // out: oneof 0=open|1=closed|2=periodic
   int &singular_u,        // out: oneof 0=none|1=low|2=high|3=both
   int &end_cond_v,        // out: oneof 0=open|1=closed|2=periodic
   int &singular_v,        // out: oneof 0=none|1=low|2=high|3=both
   int &ntgrl_degree,      // out: for deformable modeling - the
                           //      degree polynomial exactly integrated
                           //      should be at least twice the degree.
    SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: total_knot_count
                           //           = dof_count + degree - 1
                           //       the total and distinct knot
                           //       counts vary when there are
                           //       multiple knots.
                           // note: any input array set to NULL
                           //       will be ignored by subroutine.

//===========================================================
// DS_pfunc *DM_make_nurb_surface
//===========================================================

/**
* Constructs a <tt>NURB</tt> surface from data and returns pointer to <tt>DS_pfunc</tt> <tt>NURB</tt> surf or NULL.
* <br><br>
* <b>Role:</b> Modifies <tt>return_err</tt>.
* <br><br>
* Creates and returns a pointer to <tt>DS_pfunc</tt> <tt>NURB</tt> surface representation. A <tt>NURB</tt>
* surface is the tensor product of the underlying <i>u</i>-curve and <i>v</i>-curve basis
* functions. The total number of dofs in the surface is just the product of the
* dof counts for <i>u</i> and <i>v</i> curve basis functions. The global dof numbers for each
* dof are the cross product of the <i>u</i>-curve and <i>v</i>-curve dof numbers with the
* <i>v</i>-curve dof numbers varying the fastest. This function's input arguments contain
* enough information to make both the <i>u</i>-curve and the <i>v</i>-curve basis functions as
* well as the dof values for the surface.
* <br><br>
* <tt>image_dim</tt> is the size of the model space, using <tt>2</tt> for <tt>2D</tt> and <tt>3</tt> for <tt>3D</tt>. <tt>degree_u</tt>
* and <tt>degree_v</tt> are the <tt>NURB</tt> polynomial degrees in the surface's <i>u</i> and <i>v</i> directions.
* <tt>dof_count_u</tt> and <tt>dof_count_v</tt> are the number of <tt>NURB</tt> control points in the
* surface's <i>u</i> and <i>v</i> directions. <tt>knot_count_u</tt> and <tt>knot_count_v</tt> are the number of
* unique knot values in the surface's <i>u</i> and <i>v</i> directions.
* <br><br>
* The <tt>total_knot_count</tt> equals the <tt>dof_count + degree - 1</tt> for each <i>u</i> and <i>v</i> surface
* direction. The <tt>total_knot_count</tt> will not equal the distinct <tt>knot_count</tt> when the
* <tt>NURB</tt> has any multiple knots. The multiplicity of each knot is communicated
* through the <tt>knot_index</tt> array. Each knot, starting with the lowest <i>u</i> value knot,
* is assigned a <tt>knot_index</tt> starting at <tt>0</tt>. The <tt>knot_index</tt> array records the highest
* index value for all knots at each distinct knot location. For example, a common
* <tt>NURB</tt> is one with multiple knots on the end points and single knots in the
* interior. A map of the knot index values might look like:
* <pre>
* +------+------+-------+------+
* 0  3  4  5  6
* 1           7
* 2           8 </pre>
* This knot array corresponds to a <tt>degree = 3</tt> <i>B</i>-spline with seven control points
* and four spans. The <i>B</i>-spline interpolates the first and last control point
* positions due to the multiple end knots.
* <br><br>
* There are nine total knot values and only five distinct knot values. The knot
* index array for this curve would be <tt>[2,3,4,5,8]</tt>. The knot array contains the
* domain space values (<tt>u</tt> values) for each distinct knot. For the above example,
* the knot array might be something like <tt>[0.00, 0.25, 0.50, 0.75, 1.00]</tt> for an
* even distribution of knot values from <tt>0</tt> to <tt>1</tt>.
* <br><br>
* Other common <i>B</i>-spline representation schemes store an extra unused knot at each
* end of the knot array. These extra knot values seem to appear in all the
* <i>B</i>-spline evaluation functions but in reality are never used and are not input to
* the deformable modeling package. Developers moving <i>B</i>-spline data between this
* and other systems must take care to watch out for this mismatch possibility on
* the end knots.
* <br><br>
* The <tt>dof_vec</tt> contains the surface's control point locations and is sized
* <tt>[image_dim*dof_count_u*dof_count_v]</tt>. It is ordered by control point, so a two
* dimensional surface with a <tt>2x2</tt> grid of control points will be organized as:
* <pre>
* dof01  dof11  Global dof numbers
* +----------+   dof0 = dof00
* |   |   dof1 = dof01
* |   |   dof2 = dof10
* |   |   dof3 = dof11
* +----------+
* dof00 dof10
* </pre>
* and will have a <tt>dof_vec</tt> stored as <tt>[x0, y0, x1, y1, x2, y2, x3, y3]</tt>, and the same
* surface in <tt>3d</tt> would be stored as, <tt>[x0, y0, z0, x1, y1, z1, x2, y2, z2, x3, y3,
* z3]</tt>. Deformable surfaces have both a current shape and a default shape. The
* default shape is stored as a second array of control points called <tt>dof_def</tt>.
* <tt>dof_def</tt> is sized and stored in the same manner as <tt>dof_vec</tt>.
* <br><br>
* Surfaces can have three different end conditions applied to them through the use
* of constraints. These include an <tt>end_cond</tt> which can force opposite edges of the
* surface square to connect to each other with either a <tt>C0</tt> or a <tt>C1</tt> continuity, and
* a singularity case which can collapse an entire boundary edge into a single
* point. These end conditions are communicated through the use of four flags,
* <tt>end_cond_u</tt>, <tt>end_cond_v</tt>, <tt>singular_u</tt>, and <tt>singular_v</tt>. The connectivity of the <i>u</i>
* and <i>v</i> boundary curves are set independently by the <tt>end_cond_u</tt> and <tt>end_cond_v</tt>
* flag values. When end condition equals <tt>0</tt> the surface remains open in that
* direction. When end condition equals <tt>1</tt> it will be <tt>C0</tt> across the end curves. And
* when end condition equals <tt>2</tt> it will be <tt>C1</tt> across the end curves. The singularity
* of the u and v boundaries are set independently by the <tt>singular_u</tt> and <tt>singular_v</tt>
* flag values. When singular equals <tt>0</tt> all edges are normal. When singular equals <tt>1</tt>
* the low end boundary curve is collapsed to a point. When singular equals <tt>2</tt> the
* high end boundary curve is collapsed to a point. When singular equals <tt>3</tt> both low
* and high end boundary curves are collapsed to a single point.
* <br><br>
* <b>Errors:</b>
* <dl>
* <dt><tt>DM_BAD_IMAGE_DIM_VALUE</tt></dt>
* <dd>The input value of the image dimension must be positive.</dd>
* <dt><tt>DM_BAD_DEGREE_VALUE</tt></dt>
* <dd>The requested spline degrees must be 0 or greater.</dd>
* <dt><tt>DM_BAD_KNOT_COUNT_VALUE</tt></dt>
* <dd>The number of knots must be positive.</dd>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The knot_index, knot, and weight array pointers cannot be NULL on input.</dd>
* <dt><tt>DM_BAD_KNOT_TO_CPT_COUNT</tt></dt>
*<dd> The last knot index number must equal dof_count + degree - 2.</dd>
* <dt><tt>DM_BAD_NTGRL_DEGREE_VALUE</tt></dt>
* <dd>The required gauss integration degree is not supported. Currently limited to the
* range 1 to 79. The ntgrl_degree must be equal to or larger than twice the spline
* degree for valid deformations.</dd>
*</dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param image_dim
* in: image space size (2=2d,3=3d).
* @param degree_u
* in: u dir polynomial degree.
* @param dof_count_u
* in: u dir control point count.
* @param knot_count_u
* in: u dir distinct knot value count.
* @param knot_index_u
* in: u dir multiple knot count array. sized:[knot_count] specify the multiple knots by setting count[i] = maximum knot index value for location knot[i] Examples: no multiples count = [0,1,2,3,4] some multiples = [1,2,4].
* @param knot_u
* in: u dir knot value array sized:[knot_count] ordered [u0 &lt; u1 &lt; u2...].
* @param degree_v
* in: v dir polynomial degree.
* @param dof_count_v
* in: v dir control point count.
* @param knot_count_v
* in: v dir distinct knot value count.
* @param knot_index_v
* in: v dir multiple knot count array. sized:[knot_count] specify the multiple knots by setting count[i] = maximum knot index value for location knot[i] Examples: no multiples count = [0,1,2,3,4] some multiples = [1,2,4].
* @param knot_v
* in: v dir knot value array sized:[knot_count] ordered [u0 &lt; u1 &lt; u2...].
* @param dof_vec
* in: init dof_vec vals or NULL for 0 sized:[image_dim* dof_count_u* dof_count_v] ordered:[C00, C01, ... C0m, C10, ... Cnm].
* @param dof_def
* in: init default shape or NULL for 0 ordered:[C00, C01, ... C0m, C10, ... Cnm].
* @param weight
* in: init weight for each control pt sized:[dof_count_u* dof_count_v] ordered:[w00, w01, ... w0m, ... wnm].
* @param end_cond_u
* in: one of 0=open or 1=closed or 2=periodic.
* @param singular_u
* in: one of 0=none or 1=low or 2=high or 3=both.
* @param end_cond_v
* in: one of 0=open or 1=closed or 2=periodic.
* @param singular_v
* in: one of 0=none or 1=low or 2=high or 3=both.
* @param sdmo
* in:SDM_options pointer note: total_knot_count = dof_count+degree-1 the total and distinct knot counts vary when there are multiple knots note: the input array values are only copied. The calling program still needs to manage that memory.
**/
DECL_DM DS_pfunc *         // rtn: ptr to DS_pfunc nurb surf or NULL, [malloc]
DM_make_nurb_surface       // eff: construct nurb surface from data
  (int &rtn_err,           // out: 0=success or negative err code
   int image_dim,          // in : image space size (2=2d,3=3d)
   int degree_u,           // in : u dir polynomial degree
   int dof_count_u,        // in : u dir control point count
   int knot_count_u,       // in : u dir distinct knot value count
   int *knot_index_u,      // in : u dir multiple knot count array, [pre-alloced]
                           //      sized:[knot_count_u]
                           //      specify the multiple knots by
                           //      setting count[i] = maximum knot
                           //      index value for location knot[i]
                           //      Examples:
                           //      no multiples count = [0,1,2,3,4]
                           //      some multiples     = [1,2,4]
   double *knot_u,         // in : u dir knot value array, [pre-alloced]
                           //      sized:[knot_count_u]
                           //      ordered [u0 < u1 < u2 ... ]
   int degree_v,           // in : v dir polynomial degree
   int dof_count_v,        // in : v dir control point count
   int knot_count_v,       // in : v dir distinct knot value count
   int *knot_index_v,      // in : v dir multiple knot count array, [pre-alloced]
                           //      sized:[knot_count_v]
                           //      specify the multiple knots by
                           //      setting count[i] = maximum knot
                           //      index value for location knot[i]
                           //      Examples:
                           //      no multiples count = [0,1,2,3,4]
                           //      some multiples     = [1,2,4]
   double *knot_v,         // in : v dir knot value array, [pre-alloced]
                           //      sized:[knot_count_v]
                           //      ordered [u0 < u1 < u2 ... ]
   double *dof_vec,        // in : init dof_vec vals or NULL for 0, [pre-alloced]
                           //      sized:[image_dim*dof_count_u*dof_count_v]
                           //      ordered:[C00,C01,..C0m,
                           //               C10,.. Cnm]
   double *dof_def,        // in : init default shape or NULL for 0, [pre-alloced]
                           //      sized:[image_dim*dof_count_u*dof_count_v]
                           //      ordered:[C00,C01,..C0m,C10,..Cnm]
   double *weight,         // in : init wieght for each control pt, [pre-alloced]
                           //      sized:[dof_count_u*dof_count_v]
                           //      ordered:[w00,w01,..w0m,..wnm]
   int end_cond_u=0,       // in : oneof 0=open|1=closed|2=periodic
   int singular_u=0,       // in : oneof 0=none|1=low|2=high|3=both
   int end_cond_v=0,       // in : oneof 0=open|1=closed|2=periodic
   int singular_v=0,       // in : oneof 0=none|1=low|2=high|3=both
    SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: total_knot_count
                           //           = dof_count + degree - 1
                           //       the total and distinct knot
                           //       counts vary when there are
                           //       multiple knots.
                           // note: the input array values are only
                           //       copied. The calling program
                           //       still needs to manage that memory.

//===========================================================
// DS_pfunc *DM_get_nurb_surface
//===========================================================
/**
* Gets <tt>NURB</tt> surface data and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies <tt>image_dim</tt>, <tt>knot</tt>, <tt>dof_def</tt>, <tt>ntgrl_degree</tt>, <tt>degree_u</tt>,
* <tt>dof_count_u</tt>, <tt>knot_count_u</tt>, <tt>knot_index_u</tt>, <tt>knot_u</tt>, <tt>degree_v</tt>,<tt>dof_count_v</tt>,<tt>knot_count_v</tt>,
* <tt>knot_index_v</tt>, <tt>knot_v</tt>, <tt>end_cond_u</tt>, <tt>singular_u</tt>, <tt>end_cond_v</tt>, <tt>singular_v</tt>
* <br><br>
* Updates the output pointer values to point to the pfunc's nested data. Do not
* use these pointer values to free memory, nor after the pfunc has been deleted.
* <br><br>
* <tt>ntgrl_degree</tt> is an internal term used by the deformable modeling package to
* control the accuracy of building the deformable modeling equations. It is the
* <tt>max_degree</tt> polynomial function that is accurately integrated by the package. If
* the value is too small the system will not deform as expected. If the value is
* too large the system slows down. The term is returned for debugging and
* informative purposes only, and its value should always be at least twice the
* degree value. The maximum value is <tt>79</tt>.
* <br><br>
* The knot values returned in the knot arrays define the <tt>DS_pfunc</tt>'s domain space
* which is called the <tt>internal_pfunc_space</tt>. When used in a deformable model, the
* <tt>internal_pfunc_space</tt> may be scaled by a constant factor to optimize the
* numerical accuracy of computing high order derivatives. The domain space of the
* deformable model is called the orig_dmod_space, and it remains constant for
* input and output purposes. The relationship between the two spaces is always
* just,
*<pre> internal_pfunc_space = domain_scale * orig_dmod_space.</pre>
* The domain_scale factor may be retrieved with the function,
* <tt>DM_get_dmod_domain_scale()</tt>.
* <br><br>
* <b>Errors:</b>
*<dl>
*<dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The deformable model cannot be NULL on entry.</dd>
* <dt><tt>DM_NOT_A_NURB</tt></dt>
* <dd>The input pfunc type must be a <tt>NURB</tt> curve.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param pfunc
* in: pfunc data structure to examine.
* @param image_dim
* out: image space size (2=2d,3=3d).
* @param degree_u
* out: u dir polynomial degree.
* @param dof_count_u
* out: u dir control point count.
* @param knot_count_u
* out: u dir number of distinct knot values.
* @param knot_index_u
* out: u dir multiple knot count array. sized:[knot_count] specify the multiple knots by setting count[i] = maximum knot index value for location knot[i] Examples: no multiples count = [0,1,2,3,4] some multiples = [1,2,4].
* @param knot_u
* out: u dir knot values sized:[knot_count] ordered [u0 &lt; u1 &lt; u2...] output in internal_pfunc_space.
* @param degree_v
* out: v dir polynomial degree.
* @param dof_count_v
* out: v dir control point count.
* @param knot_count_v
* out: v dir number of distinct knot values.
* @param knot_index_v
* out: v dir multiple knot count array. sized:[knot_count] specify the multiple knots by setting count[i] = maximum knot index value for location knot[i] Examples: no multiples count = [0,1,2,3,4] some multiples = [1,2,4].
* @param knot_v
* out: v dir knot values sized:[knot_count] ordered [u0 &lt; u1 &lt; u2...], output in current_pfunc_space.
* @param dof_vec
* out: dof_vec vals (control pt locs) sized:[image_dim* dof_count] ordered:[xyz0, xyz1, xyz2, ...].
* @param dof_def
* out: default shape (control pt locs) sized:[image_dim* dof_count] ordered:[xyz0, xyz1, xyz2, ...].
* @param weight
* out: init weight for each control pt sized:[dof_count_u* dof_count_v] ordered:[w00, w01, ... w0m, ... wnm].
* @param end_cond_u
* out: one of 0=open or 1=closed or 2=periodic.
* @param singular_u
* out: one of 0=none or 1=low or 2=high or 3=both.
* @param end_cond_v
* out: one of 0=open or 1=closed or 2=periodic.
* @param singular_v
* out: one of 0=none or 1=low or 2=high or 3=both.
* @param ntgrl_degree
* out: for deformable modeling - the degree polynomial exactly integrated should be larger than the degree. 10 is a good value. default value = 10.
* @param sdmo
* in:SDM_options pointer  note: total_knot_count = dof_count + degree - 1 the total and distinct knot counts vary when there are multiple knots. note: any input array set to NULL will be ignored by subroutine.
**/
DECL_DM void
DM_get_nurb_surface        // eff: retrieve nurb surface data
  (int &rtn_err,           // out: 0=success or neg err code
   DS_pfunc *pfunc,        // in : pfunc data structure to examine, [pre-alloced]
   int &image_dim,         // out: image space size (2=2d,3=3d)
   int &degree_u,          // out: u dir polynomial degree
   int &dof_count_u,       // out: u dir control point count
   int &knot_count_u,      // out: u dir number of distinct knot values
   int *&knot_index_u,     // out: u dir multiple knot count array, [nested]
                           //      sized:[knot_count_u]
                           //      specify the multiple knots by
                           //      setting count[i] = maximum knot
                           //      index value for location knot[i]
                           //      Examples:
                           //      no multiples count = [0,1,2,3,4]
                           //      some multiples     = [1,2,4]
   double *&knot_u,        // out: u dir knot value array, [nested]
                           //      sized:[knot_count_u]
                           //      ordered [u0 < u1 < u2 ... ]
   int &degree_v,          // out: v dir polynomial degree
   int &dof_count_v,       // out: v dir control point count
   int &knot_count_v,      // out: v dir number of distinct knot values
   int *&knot_index_v,     // out: v dir multiple knot count array, [nested]
                           //      sized:[knot_count_v]
                           //      specify the multiple knots by
                           //      setting count[i] = maximum knot
                           //      index value for location knot[i]
                           //      Examples:
                           //      no multiples count = [0,1,2,3,4]
                           //      some multiples     = [1,2,4]
   double *&knot_v,        // out: v dir knot value array, [nested]
                           //      sized:[knot_count_v]
                           //      ordered [u0 < u1 < u2 ... ]
   double *&dof_vec,       // out: dof_vec vals (control pt locs), [nested]
                           //      sized:[image_dim*dof_count_u*dof_count_v]
                           //      ordered:[xyz0,xyz1,xyz2...]
   double *&dof_def,       // out: default shape (control pt locs), [nested]
                           //      sized:[image_dim*dof_count_u*dof_count_v]
                           //      ordered:[xyz0,xyz1,xyz2...]
   double *&weight,        // out: init wieght for each control pt, [nested]
                           //      sized:[dof_count_u*dof_count_v]
                           //      ordered:[w00,w01,..w0m,..wnm]
   int &end_cond_u,        // out: oneof 0=open|1=closed|2=periodic
   int &singular_u,        // out: oneof 0=none|1=low|2=high|3=both
   int &end_cond_v,        // out: oneof 0=open|1=closed|2=periodic
   int &singular_v,        // out: oneof 0=none|1=low|2=high|3=both
   int &ntgrl_degree,      // out: for deformable modeling - the
    SDM_options *sdmo=NULL);// in : SDM_options pointer
                           //      degree polynomial exactly integrated
                           //      should be at least twice the degree.
                           // note: total_knot_count
                           //           = dof_count + degree - 1
                           //       the total and distinct knot
                           //       counts vary when there are
                           //       multiple knots.
                           // note: any input array set to NULL
                           //       will be ignored by subroutine.

//=============================================================
// DM_make_dcurve_image
//=============================================================
/**
* Creates a projected domain curve and returns the projected domain curve or <tt>NULL</tt>.
* <br><br>
* <b>Role:</b> Modifies <tt>return_err</tt>.
* <br><br>
* Builds and returns the pointer to a <tt>DS_pfunc</tt> curve shape which approximates the
* projection of the input dcurve into image space through the shape of the surface.
* <br><br>
* When <tt>domain_flag == 0</tt>, the dcurve is assumed to exist in the <tt>orig_dmod_space</tt>.
* When <tt>domain_flag == 2</tt>, the dcurve is assumed to exist in the
* <tt>internal_pfunc_space</tt>. When <tt>domain_flag</tt> is <tt>0</tt>, the dcurve is projected from the
* <tt>orig_dmod_space</tt> to the <tt>internal_pfunc_space</tt>, prior to being projected into the
* image_space through the surface pfunc using the simple scaling relationship,
*<pre> internal_pfunc_space = domain_scale * orig_dmod_space.</pre>
* When <tt>domain_flag == 0</tt>, the required value for <tt>domain_scale</tt> may be retrieved by
* us call, <tt>DM_get_dmod_domain_scale</tt>( <tt>rtn_err</tt>, <tt>dmod_containing_surface</tt>). Note, the
* input dcurve may not be given in <tt>unit_space</tt> for this function.
* <br><br>
* Returns the newly constructed projected <tt>DS_pfunc</tt> curve or <tt>NULL</tt> for an error.
* <br><br>
* <b>Errors:</b>
*<dl>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The dcurve and surface cannot be NULL on entry.</dd>
* <dt><tt>DM_MIXED_DCRV_DIM</tt></dt>
* <dd>The dcurve's image_dim must equal the surface's domain_dim.</dd>
* <dt><tt>DM_BAD_TOLERANCE_VALUE</tt></dt>
* <dd>The tolerance cannot be 0.0.</dd>
* <dt><tt>DM_BAD_DOMAIN_PT_RANGE</tt></dt>
* <dd>dcurve must be completely contained within the deformable model.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param domain_flag
* in: dcurve domain_space 0=original_dmod_space 2 = internal_pfunc_space.
* @param domain_scale
* in: used to map internal_pfunc_space = domain_scale * original_dmod_space.
* @param dcurve
* in: domain curve to be projected.
* @param surface
* in: surface defining curve projection.
* @param tolerance
* in: accuracy of the output curve to actual projection as tol >= dist_between(W (C(s)),W(s)) the max dist between the projection curve and any projected points.
* @param sdmo
* in:SDM_options pointer  note: dcurve->Image_dim = surface->Domain_dim.
**/
DECL_DM DS_pfunc *         // rtn: projected domain curve or NULL, [malloc].
DM_make_dcurve_image       // eff: build projected domain curve
  (int &rtn_err,           // out: 0=success or negative err code
   int domain_flag,        // in : dcurve domain_space
                           //      0=original_dmod_space
                           //      2=internal_pfunc_space
   double domain_scale,    // in : used to map
                           //      internal_pfunc_space
                           //       =   domain_scale
                           //         * original_dmod_space
   DS_pfunc *dcurve,       // in : domain curve to be projected, [pre-alloced]
   DS_pfunc *surface,      // in : surface defining curve projection, [pre-alloced]
   double tolerance,       // in : accuracy of the output curve
                           //      to actual projection as
                           //      tol >= dist_between(W(C(s)),W(s))
                           //      the max dist between the projection
                           //      curve and any projected points.
    SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: dcurve->Image_dim == surface->Domain_dim


//===========================================================
// DM_get_pfunc.. _type, _dofs, DM_copy_pfunc, DM_delete_pfunc
//===========================================================

/**
* Gets the type of a <tt>DS_pfunc</tt> and returns a <tt>DS_PFN</tt> <tt>DS_pfunc</tt> enumeration type.
* <br><br>
* <b>Role:</b> Modifies <tt>return_err</tt>.
* <br><br>
* Returns the enumeration type for the input pfunc.
* <br><br>
* Returns one of the following values:
*<pre> ds_tp1 = <i>B</i>-spline curve
* ds_rp1  = <tt>NURB</tt> curve
* ds_cir = circ curve
* ds_tp2 = <i>B</i>-spline surface
* ds_rp2 = <tt>NURB</tt> surface</pre>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or DM_NULL_INPUT_PTR.
* @param pfunc
* in: pfunc object to copy.
* @param sdmo
* in:SDM_options pointer  rtn: ds_tp1 = <i>B</i>-spline curve ds_rp1 = nurb curve ds_cir = circ curve ds_tp2 = <i>B</i>-spline surface ds_rp2 = nurb surface.
**/

DECL_DM DS_PFN             // rtn: a DS_PFN DS_pfunc enum type
DM_get_pfunc_type_id       // eff: check and return type for DS_pfunc
  (int &rtn_err,           // out: 0=success or DM_NULL_INPUT_PTR
   DS_pfunc *pfunc,        // in : pfunc object to query, [pre-alloced]
    SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // rtn: ds_tp1 = <i>B</i>-spline curve
                           //      ds_rp1 = nurb    curve
                           //      ds_cir = circ    curve
                           //      ds_tp2 = <i>B</i>-spline surface
                           //      ds_rp2 = nurb    surface

/**
* Gets the pfunc's image dimension value.
* <br><br>
* <b>Errors:</b>
*<dl>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The pfunc cannot be NULL on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param pfunc
* in: pfunc to query.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM int                // rtn: pfunc's image dim or 0
DM_get_pfunc_image_dim     // eff: return pfunc's image dimension
 (int &rtn_err,            // out: 0=success or neg err code
  DS_pfunc *pfunc,         // in : pfunc object to query, [pre-alloced]
    SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the pfunc's domain dimension.
* <br><br>
* <b>Errors:</b>
*<dl>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The pfunc cannot be NULL on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param pfunc
* in: pfunc to query.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM int                // rtn: pfunc's domain dim or 0
DM_get_pfunc_domain_dim    // eff: return pfunc's domain dimension
 (int &rtn_err,            // out: 0=success or neg err code
  DS_pfunc *pfunc,         // in : pfunc object to query, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Returns the pfunc's <tt>dof_count</tt> value.
* <br><br>
* <b>Role:</b> Returns <tt>pfunc</tt>'s <tt>dof_count</tt> (the number of control points for a
* <tt>NURB</tt> or <i>B</i>-spline curve or surface).
* <br><br>
* <b>Errors:</b>
*<dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The pfunc cannot be NULL on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param pfunc
* in: pfunc to query.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM int                // rtn: pfunc's dof_count value
DM_get_pfunc_dof_count     // eff: return pfunc's dof count
 (int &rtn_err,            // out: 0=success or neg err code
  DS_pfunc *pfunc,         // in : pfunc object to query, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Returns the pfunc's elem_count value.
* <br><br>
* <b>Role:</b> This function returns the <tt>pfunc</tt>'s <tt>elem_count</tt>. For <i>B</i>-splines and
* <tt>NURB</tt>s each unique knot value defines an element boundary in the domain space of
* the curve or surface.
* <br><br>
* <b>Errors:</b>
*<dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The pfunc cannot be NULL on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param pfunc
* in: pfunc to query.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM int                // rtn: pfunc's elem_count value
DM_get_pfunc_elem_count    // eff: return pfunc's element count
 (int &rtn_err,            // out: 0=success or neg err code
  DS_pfunc *pfunc,         // in : pfunc object to query, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the <tt>pfunc</tt>'s knot count values (<i>u</i> and <i>v</i> directions).
* <br><br>
* <b>Role:</b> This function gets <tt>knot_count_u</tt>, <tt>knot_count_v</tt>.
* <br><br>
* <b>Errors:</b>
* <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The pfunc cannot be NULL on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param pfunc
* in: pfunc to query.
* @param knot_count_u
* pfunc's u knot_count value.
* @param knot_count_v
* pfunc's v knot_count value (set to 0 by curves).
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_get_pfunc_knot_counts   // eff: return pfunc's dof count
 (int &rtn_err,            // out: 0=success or neg err code
  DS_pfunc *pfunc,         // in : pfunc object to query, [pre-alloced]
  int &knot_count_u,       // out: pfunc's u dir knot_count
  int &knot_count_v,       // out: pfunc's v dir knot_count
                           //      (set to 0 by curves)
   SDM_options *sdmo=NULL);// in : SDM_options pointer
/**
* Gets the degrees of freedom value of a <tt>pfunc</tt> and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies <tt>dof_count_u</tt>, <tt>dof_count_v</tt>, <tt>dof_vec</tt>, <tt>weight</tt>.
* <br><br>
* Queries the input pfunc and loads the output variables. Curves set the value
* <tt>dof_count_v</tt> to <tt>1</tt>. So that the total dof count can always be computed by
* <tt>dof_count_u * dof_count_v. non-NURB</tt> curves and surfaces set the weight parameter
* to <tt>NULL</tt>. <tt>type_id</tt> is set to <tt>ds_tp1</tt> (<i>B</i>-spline crv), <tt>ds_tp2</tt>(<i>B</i>-spline srf),
* <tt>ds_rp1</tt>(<tt>NURB crv</tt>), <tt>ds_rp2</tt>(<tt>NURB srf</tt>), <tt>ds_cir</tt>(<tt>circ crv</tt>)
* <br><br>
* <b>Errors:</b>
*<dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The pfunc cannot be NULL on entry.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param pfunc
* in: pfunc to query.
* @param type_id
* out: ds_tp1(<i>B</i>-spline crv), ds_tp2(<i>B</i>-spline srf) ds_rp1(<tt>NURB</tt> crv), ds_rp2(<tt>NURB</tt> srf) ds_cir(Circ crv).
* @param image_dim
* out: coordinate count for each dof value.
* @param dof_count_u
* out: u dir dof count.
* @param dof_count_v
* out: v dir dof count (set to 1 by curvs).
* @param dof_vec
* out: array of dof values sized:[image_dim* dof_count] ordered:[xyz0, xyz1, xyz2, ...].
* @param dof_def
* out: array of default dof values [nested] sized:[dof_count_u* dof_count_v*DM_get _pfunc_image_dim (rtn_err,pfunc)] ordered:[xyz0,xyz1, xyz2...].
* @param weight
* out: <tt>NURB</tt> weight values or NULL [nested] sized:[dof_count_u* dof_count_v].
* @param sdmo
* in:SDM_options pointer  note: return ptr values point to memory within the object. Do not free these pointers.
**/
DECL_DM void
DM_get_pfunc_dofs          // eff: query pfunc for dof values
  (int &rtn_err,           // out: 0=success or neg err code
   DS_pfunc *pfunc,        // in : pfunc to query, [pre-alloced]
   DS_PFN &type_id,        // out: ds_tp1(<i>B</i>-spline crv), ds_tp2(<i>B</i>-spline srf)
                           //      ds_rp1(<tt>NURB</tt> crv),    ds_rp2(<tt>NURB</tt> srf)
                           //      ds_cir(Circ crv)
   int &image_dim,         // out: coordinate count for each dof value
   int &dof_count_u,       // out: u dir dof count
   int &dof_count_v,       // out: v dir dof count (set to 1 by curvs)
   double *&dof_vec,       // out: array of dof values, [nested]
                           //      sized:[dof_count_u*dof_count_v*DM_get_pfunc_image_dim(rtn_err,pfunc)]
                           //      ordered:[xyz0,xyz1,xyz2...]
   double *&dof_def,       // out: array of default dof values, [nested]
                           //      sized:[dof_count_u*dof_count_v*DM_get_pfunc_image_dim(rtn_err,pfunc)]
                           //      ordered:[xyz0,xyz1,xyz2...]
   double *&weight,        // out: <tt>NURB</tt> weight values or NULL, [nested]
                           //      sized:[dof_count_u*dof_count_v]
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: return ptr values point to
                           //       memory within the object. Do
                           //       not free these pointers.

/**
* Gets degree of freedom or dof values.
* <br><br>
* <b>Role:</b> This function queries the input pfunc and loads the output
* variables <tt>type_id</tt>, <tt>degree_u</tt>, and <tt>degree_v</tt>. It returns zero for success or a
* negative error code. <tt>degree_u</tt> is set with the polynomial degree of the <i>u</i>
* direction basis functions. Surfaces set <tt>degree_v</tt> with the polynomial degree of
* the <i>v</i> direction basis functions. Only surfaces set the value of <tt>degree_v</tt>. When
* <tt>type_id</tt> equals <tt>ds_cir</tt>, <tt>degree_u</tt> is set to <tt>10</tt>, which is considered large. The
* real number is infinite.
* <br><br>
* <tt>type_id</tt> is set to <tt>ds_tp1</tt> (<i>B</i>-spline crv), <tt>ds_tp2</tt>(<i>B</i>-spline srf), <tt>ds_rp1</tt>(<tt>NURB crv</tt>),
* <tt>ds_rp2</tt>(<tt>NURB srf</tt>), <tt>ds_cir</tt>(<tt>circ crv</tt>)
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The pfunc cannot be NULL on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param pfunc
* in: pfunc to query.
* @param type_id
* out: ds_tp1(<i>B</i>-spline crv), ds_tp2(<i>B</i>-spline srf), ds_rp1(<tt>NURB</tt> crv), ds_rp2(<tt>NURB</tt> srf) ds_cir(Circ crv).
* @param degree_u
* out: u dir degree.
* @param degree_v
* out: v dir degree (not set by curves).
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_get_pfunc_degrees       // eff: query pfunc for poly degrees
  (int &rtn_err,           // out: 0=success or neg err code
   DS_pfunc *pfunc,        // in : pfunc to query, [pre-alloced]
   DS_PFN &type_id,        // out: ds_tp1(<i>B</i>-spline crv), ds_tp2(<i>B</i>-spline srf)
                           //      ds_rp1(<tt>NURB</tt> crv),    ds_rp2(<tt>NURB</tt> srf)
                           //      ds_cir(Circ crv)
   int &degree_u,          // out: u dir degree
   int &degree_v,          // out: v dir degree (not set by curves)
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the <tt>dof</tt> values for the <tt>pfunc</tt> and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies <tt>type_id</tt>, <tt>u_knot_count</tt>, <tt>v_knot_count</tt>, <tt>u_s</tt>, <tt>u_index</tt>, <tt>v_s</tt>,
* <tt>v_index.</tt>
* <br><br>
* Retrieves the pfunc's basis knot data. This includes a <tt>knot_count</tt> which is the
* count of unique knot values, an array of knot indices which encodes the multiple
* knot counts for each knot location. knots are numbered starting with <tt>0</tt> in the
* order of their knot values. Multiple knots (knots with the same knot values
* still get numbered individually.) The largest knot index value for each knot
* location is stored in the index array. The knots array stores the knot values.
* Only surfaces set the <i>v</i> direction output variables. Curves of type <tt>ds_cir</tt> do not
* set any of the knot output variables.
* <br><br>
* <tt>type_id</tt> is set to <tt>ds_tp1</tt> (<i>B</i>-spline crv), <tt>ds_tp2</tt>(<i>B</i>-spline srf), <tt>ds_rp1</tt>(<tt>NURB crv</tt>),
* <tt>ds_rp2</tt>(<tt>NURB srf</tt>), <tt>ds_cir(circ crv)</tt>
* <br><br>
* The output pointer values are set to memory being used by an active object. Do
* not free the memory associated with these pointers.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The deformable model cannot be NULL on entry.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param pfunc
* in: pfunc to query.
* @param type_id
* out: ds_tp1(<i>B</i>-spline crv), ds_tp2(<i>B</i>-spline srf) ds_rp1(<tt>NURB</tt> crv), ds_rp2(<tt>NURB</tt> srf) ds_cir(Circ crv).
* @param u_knot_count
* out: u dir knot_count.
* @param v_knot_count
* out: v dir knot_count (not set by curves).
* @param u_knots
* out: ptr to u_knot_array.
* @param u_index
* out: ptr to u index array.
* @param v_knots
* out: ptr to v_knot array (not set by curves).
* @param v_index
* out: ptr to v index array (not set by curves).
* @param sdmo
* in:SDM_options pointer  note: return ptr values point to memory within the object. Do not free these pointers.
**/
DECL_DM void
DM_get_pfunc_knots         // eff: query pfunc for knot values
  (int &rtn_err,           // out: 0=success or neg err code
   DS_pfunc *pfunc,        // in : pfunc to query, [pre-alloced]
   DS_PFN &type_id,        // out: ds_tp1(<i>B</i>-spline crv), ds_tp2(<i>B</i>-spline srf)
                           //      ds_rp1(<tt>NURB</tt> crv),    ds_rp2(<tt>NURB</tt> srf)
                           //      ds_cir(Circ crv)
   int &u_knot_count,      // out: u dir knot_count
   int &v_knot_count,      // out: v dir knot_count (not set by curves)
   double *&u_knots,       // out: ptr to u_knot_array, [nested]
                           //      sized:[u_knot_count]
   int    *&u_index,       // out: ptr to u index array, [nested]
                           //      sized:[u_knot_count]
   double *&v_knots,       // out: ptr to v_knot array, [nested] (not set by curves)
                           //      sized:[v_knot_count]
   int    *&v_index,       // out: ptr to v index array, [nested] (not set by curves)
                           //      sized:[v_knot_count]
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: return ptr values point to
                           //       memory within the object. Do
                           //       not free these pointers.

/**
* Gets the minimum domain point for a <tt>pfunc</tt> and returns 0 for success or an error.
* <br><br>
* <b>Role:</b> Modifies <tt>domain_min</tt>.
* <br><br>
* Retrieves the <tt>pfunc</tt> domain's minimum bounding point and stores it in <tt>domain_min</tt>.
* <br><br>
* The returned domain point location can be reported in either the pfunc's
* internal domain range (the actual value used internally) or it can be scaled to
* the unit range (from <tt>0.0 to 1.0</tt>) for convenience. When the domain flag is set to
* <tt>2</tt> an absolute domain point value is returned. When <tt>domain_flag</tt> is set to <tt>1</tt> a
* scaled domain point value is returned.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The deformable model cannot be NULL on entry.</dd>
* <br><br>
* <dt><tt>DM_NULL_OUTPUT_PTR</tt></dt>
* <dd>The domain min cannot NULL on entry.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param pfunc
* in: pfunc to query.
* @param domain_flag
* in: 2=domain_pts in internal_pfunc_space, 1=domain_pts in unit_space..
* @param domain_min
* out: ptr to max_pt array, sized:[domain_dim].
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_get_pfunc_domain_min    // eff: query pfunc for min domain pt
  (int &rtn_err,           // out: 0=success or neg err code
   DS_pfunc *pfunc,        // in : pfunc to query, [pre-alloced]
   int domain_flag,        // in : 2=domain_min in internal_pfunc_space,
                           //      1=domain_min in unit_space.
   double *domain_min,     // out: ptr to min_pt array, [pre-alloced]
                           //      sized:[DM_get_pfunc_domain_dim(rtn_err, pfunc)]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the maximum domain point for a <tt>pfunc</tt> and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies <tt>domain_max</tt>.
* <br><br>
* Retrieves the <tt>pfunc</tt> domain's maximum bounding point and stores it in <tt>domain_max</tt>.
* <br><br>
* The returned domain point location can be reported in either the pfunc's
* internal domain range (the actual value used internally) or it can be scaled to
* the unit range (from <tt>0.0 to 1.0</tt>) for convenience. When the <tt>domain_flag</tt> is set to
* <tt>2</tt> an absolute domain point value is returned. When <tt>domain_flag</tt> is set to <tt>1</tt> a
* scaled domain point value is returned.
* <br><br>
* <b>Errors:</b>
* <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The pfunc cannot be <tt>NULL</tt> on entry.</dd>
* <br><br>
* <dt><tt>DM_NULL_OUTPUT_PTR</tt></dt>
*<dd> The domain max is <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param pfunc
* in: pfunc to query.
* @param domain_flag
* in: 2=domain_pts in internal_pfunc_space, 1=domain_pts in unit_space.
* @param domain_max
* out: ptr to max_pt array, sized:[domain_dim].
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_get_pfunc_domain_max    // eff: query pfunc for max domain pt
  (int &rtn_err,           // out: 0=success or neg err code
   DS_pfunc *pfunc,        // in : pfunc to query, [pre-alloced]
   int domain_flag,        // in : 2=domain_max in internal_pfunc_space,
                           //      1=domain_max in unit_space.
   double *domain_max,     // out: ptr to min_pt array, [pre-alloced]
                           //      sized:[DM_get_pfunc_domain_dim(rtn_err, pfunc)]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the <tt>pfunc</tt>'s default state value.
* <br><br>
* <b>Role:</b> This function gets the <tt>pfunc</tt>'s default state value. A value of <tt>0</tt>
* means that the <tt>pfunc</tt> is not using a default shape. A value of <tt>1</tt> means that it is.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The <tt>pfunc</tt> cannot be NULL on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param pfunc
* in: pfunc to query.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM int                // rtn: 0=no default shape being used
                           //      1=default shape being used
DM_get_pfunc_default_state // eff: query pfunc for default_state
  (int &rtn_err,           // out: 0=success or neg err code
   DS_pfunc *pfunc,        // in : pfunc to query, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Sets the <tt>pfunc</tt>'s default state.
* <br><br>
* <b>Role:</b> This function modifies the <tt>pfunc</tt>'s default state value and sets
* the <tt>pfunc</tt>'s internal <tt>dof_def</tt> array of default shape dof values appropriately.
* When the <tt>default_state</tt> equals <tt>0</tt>, the <tt>pfunc</tt> will not be using a default shape and
* the <tt>dof_def</tt> values are all set to <tt>0.0</tt>. When the <tt>default_state</tt> equals <tt>1</tt>, the
* <tt>pfunc</tt> will be using a default shape and the <tt>dof_def</tt> values are all copied from
* the internal <tt>dof_vec</tt> array.
* <br><br>
* <i> <b>Note:</b> If this <tt>pfunc</tt> is nested within a DS_dmod object do not use this function
* to change its default state. Instead, use the <tt>DM_set_default_state</tt> function
* which works directly on the <tt>DM_dmod</tt> object.</i>
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The <tt>pfunc</tt> cannot be <tt>NULL</tt> on entry.</dd>
* <dt><tt>DM_NULL_OUTPUT_PTR</tt></dt>
* <dd>The domain_max cannot be NULL on entry,</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param pfunc
* pfunc to query.
* @param default_state
* 0=no default shape being used 1=default shape being used.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_set_pfunc_default_state // eff: query pfunc for default_state
  (int &rtn_err,           // out: 0=success or neg err code
   DS_pfunc *pfunc,        // in : pfunc to modify, [pre-alloced]
   int default_state,      // in : 0=stop using default shape
                           //      (set dof_def array = 0.0)
                           //      1=use default shape
                           //      (copy dof_vec into dof_def)
   SDM_options *sdmo=NULL);// in : SDM_options pointer
//=========================================================
//  DM_scale_dpt/_to_pfunc/_from_domd/_array_to_pfunc
//=========================================================

/**
* Scales domain points from the unit domain range into the dmod's actual domain range.
* <br><br>
* <b>Role:</b> Modifies <i>uv</i>.
* <br><br>
* Scales <i>uv</i> coordinate values to map the unit square into the actual parameter
* range of the input dmod. For example, an input uv value of <tt>[.5,.5]</tt> on a surface
* whose knot vector runs from <tt>2.0</tt> to <tt>12.0</tt> for both the <i>u</i> and <i>v</i> directions will
* have an output uv value of <tt>[7.0,7.0]</tt>.
* <br><br>
* <i><b>Note:</b> The <tt>DS_dmod</tt>'s domain range may be different from its contained
* <tt>DS_pfunc</tt>'s domain range. The <tt>DS_dmod</tt> domain range remains constant during a
* deformable modeling session while the <tt>DS_dmod</tt> domain range may change. Initially
* they start out the same.</i>
* <br><br>
* <b>Errors:</b>
*<br><br>
* <tt>rtn_err</tt>
*<br>set to <tt>0</tt> for success<br>
* <tt>DM_NULL_INPUT_PTR</tt> when dmod is <tt>NULL</tt> on entry.
*<br>
* <tt>DM_BAD_DOMAIN_DIM = input domain_dim != to dmod->Domain_dim() or domain_dim != 1
* or 2</tt><br>
* <tt>DM_BAD_DOMAIN_PT_RANGE = input par_pos not in 0 to 1 range</tt>
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: dmod to query.
* @param domain_dim
* in: dmod domain_dim (for error checking).
* @param uv
* i/o: scale uv coords to dmod domain range. sized:[domain_dim].
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void               // eff: scale a unit domain pt
DM_scale_unit_dpt_to_dmod  //    : to dmod domain range.
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : the defining dmod, [pre-alloced]
   int domain_dim,         // in : dmod domain_dim (for error checking)
   double *uv,             // i/o: scale uv coord values, [pre-alloced]
                           //      to dmod domain range.
                           //      sized:[domain_dim]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Scales domain vector values that are specified in a unit domain space that ranges from <tt>0</tt> to <tt>1</tt> into the deformable model's domain range.
* <br><br>
* <b>Role:</b> Modifies <tt>duv</tt>.
* <br><br>
* Scales <tt>duv</tt> coordinate values to map vectors expressed in the unit square into
* vectors in the actual parameter range of the curve or surface. For example, an
* input vector value of <tt>[.2,-.5]</tt> on a surface whose knot vector runs from <tt>2.0</tt> to
* <tt>12.0</tt> for both the <i>u</i> and <i>v</i> directions will have an output <i>uv</i> value of <tt>[2.0,-5.0]</tt>.
* <br><br>
* <i><b>Note:</b> The <tt>DS_dmod</tt>'s domain range may be different from its contained
* <tt>DS_pfunc</tt>'s domain range. The <tt>DS_dmod</tt> domain range remains constant during a
* deformable modeling session while the <tt>DS_pfunc</tt> domain range may change.
* Initially they start out the same.</i>
* <br><br>
* <b>Errors:</b>
* <br><br>
* <tt>rtn_err</tt>
*<br> set to <tt>0</tt> for success<br>
* <tt>DM_NULL_INPUT_PTR</tt> when <tt>dmod</tt> is <tt>NULL</tt> on entry.
*<br>
* <tt>DM_BAD_DOMAIN_DIM = input domain_dim != to dmod->Domain_dim() or domain_dim != 1
* or 2</tt>
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: dmod to query.
* @param domain_dim
* in: dmod domain_dim (for error checking).
* @param duv
* i/o: scale duv vector from unit square to dmod domain range sized: [domain_dim].
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void               // eff: scale a unit domain SPAvector
DM_scale_unit_dvec_to_dmod //    : to dmod's domain range.
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : the defining dmod, [pre-alloced]
   int domain_dim,         // in : dmod domain_dim (for error checking)
   double *duv,            // i/o: scale duv coord values, [pre-alloced]
                           //      todmod domain range.
                           //      sized:[domain_dim]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Scales domain point values specified in the deformable model's domain space into the <tt>unit_square</tt> domain space.
* <br><br>
* <b>Role:</b> Modifies <tt>uv</tt>.
* <br><br>
* Scales <tt>uv</tt> coordinate values to map surface parameter points from the <tt>dmod</tt>'s
* actual domain range into the unit square range. For example, an input <i>uv</i> value
* of <tt>[7.0,7.0]</tt> on a surface whose knot vector runs from <tt>2.0</tt> to <tt>12.0</tt> for both the <i>u</i>
* and <i>v</i> directions will have an output <i>uv</i> value of <tt>[.5,.5]</tt>.
* <br><br>
* <i><b>Note:</b> The <tt>DS_dmod</tt>'s domain range may be different from its contained
* <tt>DS_pfunc</tt>'s domain range. The DS_dmod domain range remains constant during a
* deformable modeling session while the <tt>DS_pfunc</tt> domain range may change.
* Initially they start out the same.</i>
* <br><br>
* <b>Errors:</b>
*<br><br>
* <tt>rtn_err</tt>
*<br> set to <tt>0</tt> for success<br>
*<tt>DM_NULL_INPUT_PTR</tt> when dmod is <tt>NULL</tt> on entry.
*<br>
*<tt>DM_BAD_DOMAIN_DIM = input domain_dim != to dmod->Domain_dim()</tt>
*<br>
*<tt>DM_BAD_DOMAIN_PT_RANGE = input par_pos not in dmod's domain range or domain_dim !
* = 1 or 2</tt>
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: dmod to query.
* @param domain_dim
* in: dmod domain_dim (for error checking).
* @param uv
* i/o: change uv coord value to fit in unit square.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void               // eff: scale a dmod domain pt
DM_scale_unit_dpt_from_dmod//    : to unit domain range.
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : the defining dmod, [pre-alloced]
   int domain_dim,         // in : dmod domain_dim (for error checking)
   double *uv,             // i/o: scale uv coord values, [pre-alloced]
                           //      to unit domain range.
                           //      sized:[domain_dim]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Scales an array of domain point values that lie from <tt>0</tt> to <tt>1</tt> into the dmod's actual <tt>domain_space</tt> range.
* <br><br>
* <b>Role:</b> Modifies <i>uv</i>.
* <br><br>
* Scales the <tt>dpt</tt> coordinate values to map the unit square into the actual
* parameter range of the curve or surface. For example, with <tt>main_dim = 2</tt> and
* <tt>pt_count = 2</tt>, an input dpt array of <tt>[.5,.5,.2,.4]</tt> on a surface whose knot vector
* runs from <tt>2.0 to 12.0</tt> for both the <i>u</i> and <i>v</i> directions will have an output <i>uv</i>
* value of <tt>[7.0,7.0,4.0,6.0]</tt>.
* <br><br>
* <i> <b>Note:</b> The <tt>DS_dmod</tt>'s domain range may be different from its contained
* <tt>DS_pfunc</tt>'s domain range. The DS_dmod domain range remains constant during a
* deformable modeling session while the <tt>DS_pfunc</tt> domain range may change.
* Initially they start out the same.</i>
* <br><br>
* <b>Errors:</b>
*<br><br>
*<tt>rtn_err</tt>
*<br> set to <tt>0</tt> for success
*<tt>DM_NULL_INPUT_PTR</tt> when <tt>dmod</tt> is <tt>NULL</tt> on entry.
*<br>
*<tt>DM_BAD_DOMAIN_DIM = input domain_dim != to dmod->Domain_dim()</tt>
*<br>
* <tt>DM_BAD_DOMAIN_DIM</tt> when <tt>domain_dim</tt> not equal to <tt>1</tt> or <tt>2</tt><br>
* <tt>DM_BAD_DOMAIN_PT_RANGE = input par_pos not</tt> in <tt>0</tt> to <tt>1</tt> range
* <br><br>
* <b>Effect:</b> Changes model
*<br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: dmod to query.
* @param domain_dim
* in: dmod domain_dim (for error checking).
* @param pt_count
* in: number of uv points in dpt.
* @param dpt
* i/o: change uv coord values from unit square to dmod domain space. 1d=[u0,u1,...,un] 2d=[uv0,uv1,...,uvn] in unit square, sized: [Domain_dim*pt_count].
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void               // eff: scale a unit domain pt array
DM_scale_unit_dpt_array_to_dmod// to dmod domain range.
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : the defining dmod, [pre-alloced]
   int domain_dim,         // in : dmod domain_dim (for error checking)
   int pt_count,           // in : number of uv points in dpt
   double *dpt,            // i/o: scale uv coord values, [pre-alloced]
                           //      to dmod domain range.
                           //      1d=[u0,u1,..un], 2d=[uv0,uv1..uvn]
                           //      sized:[domain_dim*pt_count]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Scales an array of <tt>dpts</tt> from a deformable model's parameter range to the <tt>unit_square</tt> parameter range.
* <br><br>
* <b>Role:</b> Modifies <tt>dpt</tt>. Scales the <tt>dpt</tt> coordinate values to map an array
* of domain points from the dmod's actual domain space into the unit-square space.
* When routine returns error dpt is left in an unknown state. For example, when
* <tt>domain_dim = 2</tt> and <tt>pt_count = 2</tt>, an input dpt array of <tt>[7.0,7.0,4.0,6.0]</tt> on a
* surface whose knot vector runs from <tt>2.0 to 12.0</tt> for both the <i>u</i> and <i>v</i> directions
* will have an output <tt>dpt</tt> array value of <tt>[.5,.5,.2,.4]</tt>.
* <br><br>
* <i> <b>Note:</b> The <tt>DS_dmod</tt>'s domain range may be different from its contained
* <tt>DS_pfunc</tt>'s domain range. The <tt>DS_dmod</tt> domain range remains constant during a
* deformable modeling session while the <tt>DS_pfunc</tt> domain range may change.
* Initially they start out the same.</i>
* <br><br>
* <b>Errors:</b>
* <br><br>
* <tt>rtn_err</tt>
*<br>set to <tt>0</tt> for success
* <tt>DM_NULL_INPUT_PTR</tt> when dmod is <tt>NULL</tt> on entry.
*<br>
* <tt>DM_BAD_DOMAIN_DIM = input domain_dim != to dmod->Domain_dim.</tt>
*<br>
* <tt>DM_BAD_DOMAIN_DIM</tt> when <tt>domain_dim</tt> not equal <tt>1</tt> or <tt>2.</tt>
*<br>
* <tt>DM_BAD_DOMAIN_PT_RANGE = input par_pos not in dmod's range.</tt>
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: dmod being queried.
* @param domain_dim
* in: dmod domain_dim (for error checking).
* @param pt_count
* in: number of uv points in dpt.
* @param dpt
* i/o: change uv coord values from dmod space to unit square. 1d=[u0,u1,...,un] 2d=[uv0,uv1,...,uvn] in unit square, sized [Domain_dim*pt_count].
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void               // eff: scale a dmod domain pt array
DM_scale_unit_dpt_array_from_dmod// to unit domain range.
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : the defining dmod, [pre-alloced]
   int domain_dim,         // in : dmod domain_dim (for error checking)
   int pt_count,           // in : number of uv points in dpt
   double *dpt,            // i/o: scale uv coord values, [pre-alloced]
                           //      to unit domain range.
                           //      1d=[u0,u1,..un], 2d=[uv0,uv1..uvn]
                           //      sized:[domain_dim*pt_count]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Scales domain points from the unit domain range into the <tt>pfunc</tt>'s actual domain range.
* <br><br>
* <b>Role:</b> Modifies <i>uv</i>.
* <br><br>
* Scales <i>uv</i> coordinate values to map the unit square into the actual parameter
* range of the input <tt>pfunc</tt>. For example, an input <i>uv</i> value of <tt>[.5,.5]</tt> on a surface
* whose knot vector runs from <tt>2.0 to 12.0</tt> for both the <i>u</i> and <i>v</i> directions will
* have an output <i>uv</i> value of [7.0,7.0].
* <br><br>
* <i><b>Note:</b> The <tt>DS_dmod</tt>'s domain range may be different from its contained
* <tt>DS_pfunc</tt>'s domain range. The <tt>DS_dmod</tt> domain range remains constant during a
* deformable modeling session while the <tt>DS_pfunc</tt> domain range may change.
* Initially they start out the same.</i>
* <br><br>
* <b>Errors:</b>
* <br><br>
*<tt>rtn_err</tt>
*<br>set to <tt>0</tt> for success
* <br>
* <tt>DM_NULL_INPUT_PTR</tt> when <tt>pfunc</tt> is <tt>NULL</tt> on entry.
* <br>
* <tt>DM_BAD_DOMAIN_DIM = input domain_dim != to pfunc->Domain_dim() or domain_dim != 1 or 2</tt><br>
* <tt>DM_BAD_DOMAIN_PT_RANGE = input par_pos not</tt> in <tt>0</tt> to <tt>1</tt> range
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param pfunc
* in: pfunc to query.
* @param domain_dim
* in: pfunc domain_dim (for error checking).
* @param uv
* i/o: scale uv coords to pfunc domain range. sized:[domain_dim].
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void               // eff: scale a unit domain pt
DM_scale_unit_dpt_to_pfunc //    : to pfunc domain range.
  (int &rtn_err,           // out: 0=success or neg err code
   DS_pfunc *pfunc,        // in : the defining pfunc, [pre-alloced]
   int domain_dim,         // in : pfunc domain_dim (for error checking)
   double *uv,             // i/o: scale uv coord values, [pre-alloced]
                           //      to pfunc domain range.
                           //      sized:[domain_dim]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Scales domain vector values that are specified in a unit domain space that ranges from <tt>0</tt> to <tt>1</tt> into the <tt>pfunc</tt>'s domain range.
* <br><br>
* <b>Role:</b> Modifies d<i>uv</i>.
* <br><br>
* Scales d<i>uv</i> coordinate values to map vectors expressed in the unit square into
* vectors in the actual parameter range of the curve or surface. For example, an
* input vector value of <tt>[.2,-.5]</tt> on a surface whose knot vector runs from <tt>2.0 to
* 12.0</tt> for both the <i>u</i> and <i>v</i> directions will have an output <i>uv</i> value of <tt>[2.0,-5.0]</tt>.
* <br><br>
* <i><b>Note:</b> The <tt>DS_dmod</tt>'s domain range may be different from its contained
* <tt>DS_pfunc</tt>'s domain range. The <tt>DS_dmod</tt> domain range remains constant during a
* deformable modeling session while the <tt>DS_pfunc</tt> domain range may change.
* Initially they start out the same.</i>
* <br><br>
* <b>Errors:</b>
* <br><br>
* <tt>rtn_err</tt>
* <br> set to <tt>0</tt> for success
* <br>
* <tt>DM_NULL_INPUT_PTR</tt> when <tt>pfunc</tt> is <tt>NULL</tt> on entry.
* <br>
* <tt>DM_BAD_DOMAIN_DIM = input domain_dim != to pfunc->Domain_dim() or domain_dim != 1 or 2</tt>
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param pfunc
* in: pfunc to query.
* @param domain_dim
* in: pfunc domain_dim (for error checking).
* @param duv
* i/o: scale duv vector from unit square to pfunc domain range sized:[domain_dim].
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void               // eff: scale a unit domain SPAvector
DM_scale_unit_dvec_to_pfunc//    : to <tt>pfunc</tt>'s domain range.
  (int &rtn_err,           // out: 0=success or neg err code
   DS_pfunc *pfunc,        // in : the defining pfunc, [pre-alloced]
   int domain_dim,         // in : pfunc domain_dim (for error checking)
   double *duv,            // i/o: scale duv coord values, [pre-alloced]
                           //      topfunc domain range.
                           //      sized:[domain_dim]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Scales domain point values specified in the <tt>pfunc</tt>'s <tt>domain_space</tt> into the <tt>unit_square</tt> <tt>domain_space</tt>.
* <br><br>
* <b>Role:</b> Modifies <i>uv</i>.
* <br><br>
* Scales <i>uv</i> coordinate values to map surface parameter points from the <tt>pfunc</tt>'s
* actual domain range into the unit square range. For example, an input <i>uv</i> value
* of <tt>[7.0,7.0]</tt> on a surface whose knot vector runs from <tt>2.0 to 12.0</tt> for both the <i>u</i>
* and <i>v</i> directions will have an output <i>uv</i> value of <tt>[.5,.5]</tt>.
* <br><br>
* <i><b>Note:</b> The <tt>DS_dmod</tt>'s domain range may be different from its contained
* <tt>DS_pfunc</tt>'s domain range. The <tt>DS_dmod</tt> domain range remains constant during a
* deformable modeling session while the <tt>DS_pfunc</tt> domain range may change.
* Initially they start out the same.</i>
* <br><br>
* <b>Errors:</b>
* <br><br><tt>rtn_err</tt>
*<br> set to <tt>0</tt> for success
* <tt>DM_NULL_INPUT_PTR</tt> when <tt>pfunc</tt> is <tt>NULL</tt> on entry.<br>
* <tt>DM_BAD_DOMAIN_DIM = input domain_dim != to pfunc->Domain_dim()</tt><br>
* <tt>DM_BAD_DOMAIN_PT_RANGE = input par_pos </tt>not in <tt>pfunc</tt>'s domain range or <tt>domain_dim!= 1 or 2</tt>
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param pfunc
* in: pfunc to query.
* @param domain_dim
* in: pfunc domain_dim (for error checking).
* @param uv
* i/o: change uv coord value to fit in unit square.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void               // eff: scale a pfunc domain pt
DM_scale_unit_dpt_from_pfunc//    : to unit domain range.
  (int &rtn_err,           // out: 0=success or neg err code
   DS_pfunc *pfunc,        // in : the defining pfunc, [pre-alloced]
   int domain_dim,         // in : pfunc domain_dim (for error checking)
   double *uv,             // i/o: scale uv coord values, [pre-alloced]
                           //      to unit domain range.
                           //      sized:[domain_dim]
   SDM_options *sdmo=NULL);// in : SDM_options pointer
/**
* Scales an array of domain point values that lie from <tt>0</tt> to <tt>1</tt> into the <tt>pfunc</tt>'s actual <tt>domain_space</tt> range.
* <br><br>
* <b>Role:</b> Modifies <i>uv</i>.
* <br><br>
* Scales the <tt>dpt</tt> coordinate values to map the unit square into the actual
* parameter range of the curve or surface. For example, with <tt>domain_dim = 2</tt> and
* <tt>pt_count = 2</tt>, an input dpt array of <tt>[.5,.5,.2,.4]</tt> on a surface whose knot vector
* runs from <tt>2.0 to 12.0</tt> for both the <i>u</i> and <i>v</i> directions will have an output <i>uv</i>
* value of <tt>[7.0,7.0,4.0,6.0]</tt>.
* <br><br>
* <i><b>Note:</b> The <tt>DS_dmod</tt>'s domain range may be different from its contained
* <tt>DS_pfunc</tt>'s domain range. The <tt>DS_dmod</tt> domain range remains constant during a
* deformable modeling session while the <tt>DS_pfunc</tt> domain range may change.
* Initially they start out the same.</i>
* <br><br>
* <b>Errors:</b>
* <br><br>
* <tt>rtn_err</tt>
* <br> set to <tt>0</tt> for success
* <tt>DM_NULL_INPUT_PTR</tt> when <tt>pfunc</tt> is <tt>NULL</tt> on entry.
* <br>
* <tt>DM_BAD_DOMAIN_DIM = input domain_dim != to pfunc->Domain_dim()</tt>
* <br>
* <tt>DM_BAD_DOMAIN_DIM</tt> when <tt>domain_dim</tt> not equal to <tt>1</tt> or <tt>2</tt>
* <br>
* <tt>DM_BAD_DOMAIN_PT_RANGE = input par_pos</tt> not in <tt>0</tt> to <tt>1</tt> range
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param pfunc
* in: pfunc to query.
* @param domain_dim
* in: pfunc domain_dim (for error checking).
* @param pt_count
* in: number of uv points in dpt.
* @param dpt
* i/o: change uv coord values from unit square to pfunc domain space. 1d=[u0,u1,...,un] 2d=[uv0,uv1,...,uvn] in unit square, sized [Domain_dim*pt_count].
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void               // eff: scale a unit domain pt array
DM_scale_unit_dpt_array_to_pfunc// to pfunc domain range.
  (int &rtn_err,           // out: 0=success or neg err code
   DS_pfunc *pfunc,        // in : the defining pfunc, [pre-alloced]
   int domain_dim,         // in : pfunc domain_dim (for error checking)
   int pt_count,           // in : number of uv points in dpt
   double *dpt,            // i/o: scale uv coord values, [pre-alloced]
                           //      to pfunc domain range.
                           //      1d=[u0,u1,..un], 2d=[uv0,uv1..uvn]
                           //      sized:[domain_dim*pt_count]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Scales an array of <tt>dpts</tt> from a <tt>pfunc</tt>'s param range to the <tt>unit_square</tt> parameter range.
* <br><br>
* <b>Role:</b> Modifies <tt>dpt</tt>. Scales the <tt>dpt</tt> coordinate values to map an array
* of domain points from the <tt>pfunc</tt>'s actual domain space into the unit-square space.
* When routine returns error <tt>dpt</tt> is left in an unknown state. For example, when
* <tt>domain_dim = 2</tt> and <tt>pt_count = 2</tt>, an input <tt>dpt</tt> array of <tt>[7.0,7.0,4.0,6.0]</tt> on a
* surface whose knot vector runs from <tt>2.0 to 12.0</tt> for both the <i>u</i> and <i>v</i> directions
* will have an output dpt array value of <tt>[.5,.5,.2,.4]</tt>.
* <br><br>
* <i><b>Note:</b> The <tt>DS_dmod</tt>'s domain range may be different from its contained
* <tt>DS_pfunc</tt>'s domain range. The <tt>DS_dmod</tt> domain range remains constant during a
* deformable modeling session while the <tt>DS_pfunc</tt> domain range may change.
* Initially they start out the same.</i>
* <br><br>
* <b>Errors:</b>
* <br><br>
* <tt>rtn_err</tt>
* <br>set to <tt>0</tt> for success
* <tt>DM_NULL_INPUT_PTR</tt> when <tt>pfunc</tt> is <tt>NULL</tt> on entry.
* <br>
* <tt>DM_BAD_DOMAIN_DIM = input domain_dim != to pfunc->Domain_dim.</tt>
* <br>
* <tt>DM_BAD_DOMAIN_DIM</tt> when <tt>domain_dim</tt> not equal <tt>1</tt> or <tt>2</tt>.
* <br>
* <tt>DM_BAD_DOMAIN_PT_RANGE = input par_pos</tt> not in <tt>pfunc</tt>'s range.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code
* @param pfunc
* in: pfunc being queried
* @param domain_dim
* in: pfunc domain_dim (for error checking)
* @param pt_count
* in: number of uv points in dpt
* @param dpt
* i/o: change uv coord values from pfunc space to unit square. 1d=[u0,u1,...,un] 2d=[uv0,uv1,...,uvn] in unit square, sized [Domain_dim*pt_count]
* @param sdmo
* in:SDM_options pointer
**/
DECL_DM void               // eff: scale a pfunc domain pt array
DM_scale_unit_dpt_array_from_pfunc// to unit domain range.
  (int &rtn_err,           // out: 0=success or neg err code
   DS_pfunc *pfunc,        // in : the defining pfunc, [pre-alloced]
   int domain_dim,         // in : pfunc domain_dim (for error checking)
   int pt_count,           // in : number of uv points in dpt
   double *dpt,            // i/o: scale uv coord values, [pre-alloced]
                           //      to unit domain range.
                           //      1d=[u0,u1,..un], 2d=[uv0,uv1..uvn]
                           //      sized:[domain_dim*pt_count]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM void               // eff: scale a dmod domain pt
DM_scale_dmod_dpt_to_pfunc //    : to pfunc domain range.
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : the dmod defining the domain space, [pre-alloced]
   int domain_dim,         // in : dmod domain_dim (for error checking)
   double *uv,             // i/o: scale uv coord values, [pre-alloced]
                           //      to pfunc domain range.
                           //      sized:[domain_dim]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM void               // eff: scale a dmod domain SPAvector
DM_scale_dmod_dvec_to_pfunc//    : to pfunc domain range.
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : the dmod defining the domain space, [pre-alloced]
   int domain_dim,         // in : dmod domain_dim (for error checking)
   double *duv,            // i/o: scale duv coord values, [pre-alloced]
                           //      to pfunc domain range.
                           //      sized:[domain_dim]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM void               // eff: scale a pfunc domain pt
DM_scale_dmod_dpt_from_pfunc//   : to dmod domain range.
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : the dmod defining the domain space, [pre-alloced]
   int domain_dim,         // in : dmod domain_dim (for error checking)
   double *uv,             // i/o: scale uv coord values, [pre-alloced]
                           //      to dmod domain range.
                           //      sized:[domain_dim]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM void               // eff: scale a dmod domain pt array
DM_scale_dmod_dpt_array_to_pfunc// to pfunc domain range.
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : the dmod defining the domain space, [pre-alloced]
   int domain_dim,         // in : dmod domain_dim (for error checking)
   int pt_count,           // in : number of uv points in dpt
   double *dpt,            // i/o: scale uv coord values, [pre-alloced]
                           //      to pfunc domain range.
                           //      1d=[u0,u1,..un], 2d=[uv0,uv1..uvn]
                           //      sized:[domain_dim*pt_count]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM void               // eff: scale a pfunc domain pt array
DM_scale_dmod_dpt_array_from_pfunc// to dmod domain range.
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : the dmod defining the domain space, [pre-alloced]
   int domain_dim,         // in : dmod domain_dim (for error checking)
   int pt_count,           // in : number of uv points in dpt
   double *dpt,            // i/o: scale uv coord values, [pre-alloced]
                           //      to dmod domain range.
                           //      1d=[u0,u1,..un], 2d=[uv0,uv1..uvn]
                           //      sized:[domain_dim*pt_count]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Scales the image of the input <tt>pfunc</tt>.
* <br><br>
* <b>Role:</b> Modifies <tt>pfunc</tt>. Scales the image of the input <tt>pfunc</tt> argument.
* For <i>B</i>-splines and <tt>NURB</tt>s, this is equivalent to scaling the image space location
* of every control point.
* <br><br>
* <b>Errors:</b>
*<dl><dt><tt>rtn_err</tt></dt>
*<dd>set to <tt>0</tt> for success</dd>
*<dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>when <tt>pfunc</tt> is <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_BAD_SCALE_PARAM_VALUE</tt></dt>
*<dd>when input scale parameter is <tt>0</tt>.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param pfunc
* in: pfunc being modified.
* @param scale
* in: amount to scale pfunc->image (for error checking).
* @param sdmo
* in:SDM_options pointer.
**/

DECL_DM void               // eff: scale the <tt>pfunc</tt>'s image space
DM_scale_pfunc_image       //      projection.
  (int &rtn_err,           // out: 0=success or neg err code
   DS_pfunc *pfunc,        // in : the pfunc being modified, [pre-alloced]
   double scale,           // in : amount to scale pfunc->image
   SDM_options *sdmo=NULL);// in : SDM_options pointer

//===========================================================
// DM_eval_pfunc(), DM_copy_pfunc(), DM_delete_pfunc()
//===========================================================

/**
* Calculates image positions for the input domain location and returns 0 or an error.
* <br><br>
* <b>Role:</b> Modifies <tt>W, dWu, dWv, dWuu, dWuv, dWvv, dWuuu, dWuuv, dWuvv, dWvvv.</tt>
* <br><br>
* This function uses the input <tt>pfunc</tt> to evaluate the image space values for the
* input domain point location. When <tt>domain_flag == 2</tt>, the input domain point is
* expected to be in the current domain range of the input pfunc. When <tt>domain_flag</tt>
* <tt>== 1</tt>, the the input domain point is assumed to be given in the unit-square range
* and the domain point is scaled to the actual pfunc domain range prior to being
* used in the evaluator. The function can compute and store the following,
*<br><br><table border="0" width="100%">
*<tr>
*<td width="5%" align="left"><tt>W</tt></td>
*<td width="5%"></td>
*<td width="90%">Position <tt>[x,y,z]</tt></td>
* </tr>
*<tr>
*<td width="5%" align="left"> <tt>dWu</tt></td>
*<td width="5%"></td>
*<td width="90%">1st parametric derivative in <i>u</i> direction</td>
*</tr>
*<tr>
*<td width="5%" align="left"><tt>dWv</tt></td>
*<td width="5%"></td>
* <td width="90%"> 1st parametric derivative in <i>v</i> direction</td>
*</tr>
*<tr>
*<td width="5%" align="left"> <tt>dWuu</tt></td>
*<td width="5%"></td>
*<td width="90%"> 2nd parametric derivative in <i>uu</i> direction</td>
*</tr>
*<tr>
*<td width="5%" align="left"> <tt>dWuv</tt></td>
*<td width="5%"></td>
*<td width="90%"> 2nd parametric derivative in <i>uv</i> direction</td>
*</tr>
*<tr>
*<td width="5%" align="left"> <tt>dWvv</tt></td>
*<td width="5%"></td>
*<td width="90%">&nbsp;2nd parametric derivative in <i>vv</i> direction</td>
*</tr>
*<tr>
*<td width="5%" align="left"> <tt>dWuuu</tt></td>
*<td width="5%"></td>
*<td width="90%"> 3rd parametric derivative in <i>uuu</i> direction</td>
*</tr>
*<tr>
*<td width="5%" align="left"><tt>dWuuv </tt></td>
*<td width="5%"></td>
*<td width="90%"> 3rd parametric derivative in <i>uuv</i> direction</td>
*</tr>
*<tr>
*<td width="5%" align="left"> <tt>dWuvv</tt></td>
*<td width="5%"></td>
*<td width="90%"> 3rd parametric derivative in <i>uvv</i> direction</td>
*</tr>
*<tr>
*<td width="5%" align="left"> <tt>dWvvv</tt></td>
*<td width="5%"></td>
*<td width="90%"> 3rd parametric derivative in <i>vvv</i> direction</td>
*</tr>
*</table>
* <br><br>
* The values for any output array set to <tt>NULL</tt> are not computed or stored. All
* output arrays, except for <i>W</i>, the position, default to <tt>NULL</tt> and may be omitted.
* <br><br>
* Surfaces can load all six output variables. Curves will only load <tt>W</tt>, <tt>dWu</tt>, and
* <tt>dWuu</tt> and will skip the others.
* <br><br>
* <b>Errors:</b>
*<dl>
*<dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd> The dpt input pointer cannot be <tt>NULL</tt>.</dd>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
* <dt><tt>DM_NULL_OUTPUT_PTR</tt></dt>
* <dd>The alpha cannot be NULL on entry.</dd>
* <dt><tt>DM_NON_NULL_OUTPUT_PTR</tt></dt>
*<dd> <i>W</i> output pointer cannot be <tt>NULL</tt> on entry.</dd>
* <dt><tt>DM_BAD_DOMAIN_DIM</tt></dt>
*<dd> The domain dimension must equal <tt>1</tt> or <tt>2</tt>.</dd>.
*<tt>DM_parse_tag_flag()</tt> errors
* <dt><tt>DM_scale_unit_dpt_to_pfunc()</tt></dt>
*<dd> errors.</dd>
* <dt><tt>DM_BAD_DOMAIN_PT_RANGE</tt></dt>
*<dd> The domain point must be completely contained by the deformable model.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param pfunc
* in: pfunc to evaluate.
* @param domain_flag
* in: 2=domain_pts in internal_pfunc_space, 1=domain_pts in unit_space..
* @param dpt
* in: domain point for evaluation sized:[domain_dim].
* @param W
* out: dmod xyz position for dpt loc W=[x,y,z], sized:[image_dim].
* @param dWu
* out: u_dir tang or NULL, dWu=[dx,dy,dz], sized:[image_dim].
* @param dWv
* out: v_dir tang or NULL, dWv=[dx,dy,dz], sized:[image_dim].
* @param dWuu
* out: uu parameter derivative or NULL dWuu=[dxx,dyy,dzz], sized:[image_dim].
* @param dWuv
* out: uv parameter derivative or NULL dWuv=[dxx,dyy,dzz], sized:[image_dim].
* @param dWvv
* out: uu parameter derivative or NULL dWvv=[dxx,dyy,dzz], sized:[image_dim].
* @param dWuuu
* out: uuu parametric derivative or NULL [dxx,dyy,dzz], sized:[image_dim].
* @param dWuuv
* out: uuv parametric derivative or NULL [dxx,dyy,dzz], sized:[image_dim].
* @param dWuvv
* out: uvv parametric derivative or NULL [dxx,dyy,dzz], sized:[image_dim].
* @param dWvvv
* out: vvv parametric derivative or NULL [dxx,dyy,dzz], sized:[image_dim].
* @param sdmo
* in:SDM_options pointer note: sets active dmod note: any output array set to NULL is not computed or stored.
**/
DECL_DM void
DM_eval_pfunc              // eff: calc image positions for uv location
  (int &rtn_err,           // out: 0=success or neg err code
   DS_pfunc *pfunc,        // in : pfunc to query, [pre-alloced]
   int domain_flag,        // in : 2=dpt in internal_pfunc_space,
                           //      1=dpt in unit_space.
   double *dpt,            // in : domain point for evaluation, [pre-alloced]
                           //      [u] or [u,v]
                           //      sized:[DM_get_pfunc_domain_dim(rtn_err, pfunc)]
   double *W,              // out: dmod xyz SPAposition for dpt loc, [pre-alloced]
                           //      W=[x,y,z]
                           //      sized:[DM_get_pfunc_image_dim(rtn_err, pfunc)]
   double *dWu =NULL,      // out: u_dir tang or NULL, [pre-alloced]
                           //      dWu=[dx,dy,dz]
                           //      sized:[DM_get_pfunc_image_dim(rtn_err, pfunc)]
   double *dWv =NULL,      // out: v_dir tang or NULL, [pre-alloced]
                           //      dWv=[dx,dy,dz]
                           //      sized:[DM_get_pfunc_image_dim(rtn_err, pfunc)]
   double *dWuu=NULL,      // out: uu parametric derivative or NULL, [pre-alloced]
                           //      [dxx,dyy,dzz]
                           //      sized:[DM_get_pfunc_image_dim(rtn_err, pfunc)]
   double *dWuv=NULL,      // out: uv parametric derivative or NULL, [pre-alloced]
                           //      [dxx,dyy,dzz]
                           //      sized:[DM_get_pfunc_image_dim(rtn_err, pfunc)]
   double *dWvv=NULL,      // out: vv parametric derivative or NULL, [pre-alloced]
                           //      [dxx,dyy,dzz]
                           //      sized:[DM_get_pfunc_image_dim(rtn_err, pfunc)]
   double *dWuuu=NULL,     // out: uuu parametric derivative or NULL, [pre-alloced]
                           //      [dxx,dyy,dzz]
                           //      sized:[DM_get_pfunc_image_dim(rtn_err, pfunc)]
   double *dWuuv=NULL,     // out: uuv parametric derivative or NULL, [pre-alloced]
                           //      [dxx,dyy,dzz]
                           //      sized:[DM_get_pfunc_image_dim(rtn_err, pfunc)]
   double *dWuvv=NULL,     // out: uvv parametric derivative or NULL, [pre-alloced]
                           //      [dxx,dyy,dzz]
                           //      sized:[DM_get_pfunc_image_dim(rtn_err, pfunc)]
   double *dWvvv=NULL,     // out: vvv parametric derivative or NULL, [pre-alloced]
                           //      [dxx,dyy,dzz]
                           //      sized:[DM_get_pfunc_image_dim(rtn_err, pfunc)]
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod
                           // note: any output array set to NULL
                           //       is not computed or stored
/**
* Creates a deep copy of the input <tt>pfunc</tt> and returns a pointer to the new <tt>DS_pfunc</tt> object.
* <br><br>
* <b>Role:</b> Makes and returns a pointer to a deep copy of the input pfunc.
* <br><br>
* Never use one <tt>pfunc</tt> to create two deformable model objects. Always use a
* separate copy of a pfunc for each deformable model.
* <br><br>
* Returns a <tt>NULL</tt> for failure or a pointer to an independent copy of of the input
* pfunc object.
* <br><br>
* <b>Errors:</b>
* <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The pfunc cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> System routine
* <br><br>
* @param rtn_err
* out: 0= success or DM_NULL_INPUT_PTR.
* @param pfunc
* in: pfunc object to copy, [not-nested].
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM DS_pfunc *         // rtn: ptr to new DS_pfunc object, [malloc]
DM_copy_pfunc              // eff: make a deep copy of input pfunc
  (int &rtn_err,           // out: 0=success or <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
   DS_pfunc *pfunc,        // in : pfunc object to copy, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer
/**
* Clears the <tt>pfunc</tt> data structure.
* <br><br>
* <b>Role:</b> Frees and clears all data values associated with the input
* <tt>DS_pfunc</tt> and sets the pfunc pointer to <tt>NULL</tt>.
* <br><br><ol>
* <li>The <tt>DS_pfunc</tt> pointer is invalid after this call.</li>
*<li>Do not use this function to delete the <tt>pfunc</tt> pointer after it has been
* used to build a <tt>DS_dmod</tt> deformable modeling object. The <tt>pfunc</tt> pointer will be
* deleted when the <tt>DS_dmod</tt> pointer is deleted.</li></ol>
* <br><br>
* <b>Errors:</b>
*<dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The pfunc cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> System routine
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param pfunc
* i/o: ptr to valid pfunc object set to NULL on exit.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_delete_pfunc            // eff: free the pfunc data structure
  (int &rtn_err,           // out: 0=success or neg err code
   DS_pfunc *&pfunc,       // i/o: ptr to valid pfunc object, [freed]
                           //      set to NULL on exit.
   SDM_options *sdmo=NULL);// in : SDM_options pointer

//===========================================================
// DS_dmod *DM_make_dmod_curve
//===========================================================
/**
* Creates a deformable curve model and returns a pointer to the deformable model object.
* <br><br>
* <b>Role:</b> Modifies <tt>return_err</tt>.
* <br><br>
* Builds and returns a deformable model of the pfunc shape. The deformable model
* entity pointer is made available to applications that may want to store data
* with each deformable model. This pointer is never accessed (and so may be set to
* NULL) but can be retrieved by an application with the <tt>DS_dmod</tt> method call
* <tt>DS_dmod->Entity()</tt> and it may be changed with the method
* <tt>DS_dmod->Set_entity(entity)</tt>. <tt>draw_state</tt> is a bit-array integer which may be used
* by the application to control which pieces of deformable modeling information
* are rendered for each deformable model. See the <tt>DM_DRAW_...</tt>environment
* variables defined in the file <tt>"dmapi.hxx"</tt> for a complete list. The <tt>draw_state</tt>
* defaults to drawing loads, constraints, and seams. tag is unique identifier for
* this deformable model. Since this deformable model will act as the root of its
* own patch hierarchy a good tag value is <tt>2</tt> which is its default value. alpha
* <tt>(default=1)</tt> is this deformable model's resistance to stretch parameter value.
* beta <tt>(default=5)</tt> is this deformable model's resistance to bending parameter
* value. delta <tt>(default=0)</tt> is this deformable model's resistance to displacement
* from the default shape. <tt>DS_solve()</tt> <tt>(default =1)</tt> is the deformable modeling time
* step size taken with each call to <tt>DM_solve()</tt>. mass <tt>(default 1.0)</tt> and damp
* (default <tt>5)</tt> are the dynamic parameters for this deformable model. With mass, a
* deformable model can overshoot and oscillate on its way to an equilibrium
* position. With damping the oscillations can diminish over time and the number of
* time steps until equilibrium is achieved may be increased.
* <br><br>
* mass <tt>(default 1.0)</tt> and damp <tt>(default 5)</tt> are the dynamic parameters for this
* deformable model. With mass a deformable model can overshoot and oscillate on
* its way to an equilibrium position. With damping the oscillations can diminish
* over time and the number of time steps until equilibrium is achieved may be
* increased.
* <br><br>
* <tt>mallocs</tt> is the calling program must free the memory of the returned deformable
* model pointer with a call to <tt>DM_delete_dmod</tt>. The <tt>pfunc</tt> pointer is stored within
* the deformable model and will be deleted when the deformable model is deleted.
* Do not call <tt>DM_delete_pfunc</tt> on that pointer after this call. Do not make a
* second deformable model with this pfunc pointer. Use <tt>DM_copy_pfunc</tt> if you want
* to make another deformable model with this shape.
* <br><br>
* <b>Errors:</b>
*<dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd> The <tt>pfunc</tt> cannot be <tt>NULL</tt> on input.</dd>
* <dt><tt>DM_BAD_TAG_VALUE</tt></dt>
* <dd>The tag value must be <tt>-1</tt>, to have the system assign one, or <tt>2</tt> or greater.</dd>
* <dt><tt>DM_BAD_ALPHA_VALUE</tt></dt>
*<dd> The value of alpha must be <tt>0</tt> or positive.</dd>
* <dt><tt>DM_BAD_BETA_VALUE</tt></dt>
*<dd> The value of beta must be <tt>0</tt> or positive.</dd>
* <dt><tt>DM_ZERO_ALPHA_AND_BETA</tt></dt>
* <dd>Both alpha and beta values cannot be <tt>0</tt>.</dd>:
* <dt><tt>DM_BAD_DELTA_VALUE</tt></dt>
* <dd>The value of delta must be positive.</dd>
* <dt><tt>DM_BAD_DT_VALUE</tt></dt>
* <dd>The value of <tt>dt</tt> must be positive.</dd>
* <dt><tt>DM_BAD_MASS_VALUE</tt></dt>
* <dd>The value of mass must be 0 or positive.</dd>
* <dt><tt>DM_BAD_DAMP_VALUE</tt></dt>
* <dd>The value of damp must be 0 or positive.</dd> </dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param pfunc
* in: defines the shape model.
* @param dmod_entity
* in: application ptr stored with dmod.
* @param draw_state
* in: draw bits for application graphics (draw loads, constraints, and seams).
* @param tag
* in: unique id for this object.
* @param alpha
* in: resistance to stretch [1.0].
* @param beta
* in: resistance to bending [5.0].
* @param gamma
* in: resist bending change rate.
* @param delta
* in: resistance to moving from default shape.
* @param dt
* in: implicit integration time step [1.0].
* @param mass
* in: physical param (causes rippling)[1.0].
* @param damp
* in: physical param (stable integration) [5.0].
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM DS_dmod *          // rtn: ptr to DS_dmod object or NULL, [malloc]
DM_make_dmod_curve         // eff: make a deformable curve model
  (int &rtn_err,           // out: 0=success or negative err code
   DS_pfunc *pfunc,        // in : defines the shape model, [nested]
   void *dmod_entity=NULL, // in : application ptr stored with dmod, [pass-thru]
   int draw_state=         // in : draw bits for application graphics
        SPA_DM_DRAW_SEAMS  |   //      default = draw seams
        SPA_DM_DRAW_CSTRNS |   //                     cstrns and
        SPA_DM_DRAW_LOADS,     //                     loads
   int tag = 2,            // in : unique id for this object
   double alpha  = 1.0,    // in : resistance to stretch
   double beta   = 5.0,    // in : resistance to bending
   double gamma  = 0.0,    // in : resist bending change rate
   double delta  = 0.0,    // in : resistance to moving from default shape
   double dt     = 1.0,    // in : implicit integration time step
   double mass   = 0.0,    // in : physical param (causes rippling)
   double damp   = 0.0,    // in : physical param (stabel intgrtn)
   SDM_options *sdmo=NULL);// in : SDM_options pointer

//=========================================================
// DS_dmod *DM_get_dmod_curve
//=========================================================

/**
* Gets deformable curve data.
* <br><br>
* <b>Role:</b> Modifies <tt>pfunc</tt>, <tt>draw_state</tt>, <tt>tag</tt>, <tt>alpha</tt>, <tt>beta</tt>, <tt>delta</tt>, <tt>dt</tt>, <tt>mass</tt>,
* and <tt>damp</tt>.
* <br><br>
* Loads the output variables with data from the input deformable model object.
* Note that the current value of pfunc will be overwritten so take care that it is
* not the only pointer to a current <tt>DS_pfunc</tt> object. Otherwise, a memory leak may
* occur. See function <tt>DM_make_dmod_curve()</tt> for definitions of the output arguments
* and their use.
* <br><br>
* The <tt>pfunc</tt> is loaded with a pointer to the contained <tt>DS_pfunc</tt> object which exists
* in the internal_pfunc_space. The <tt>pfunc</tt>'s domain space range is related to the
* <tt>DS_dmod</tt>'s domain space range by the <tt>domain_scale</tt> factor as:
* <br><br>
* <tt>internal_pfunc_space = domain_scale * orig_dmod_space.</tt>
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The deformable model cannot be NULL on entry.</dd>
* <dt><tt>DM_DMOD_NOT_A_CURVE</tt></dt>
* <dd>The deformable model must be a curve.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to query.
* @param domain_scale
* out: internal_pfunc_space = domain_scale * orig_dmod_space.
* @param pfunc
* out: the dmod's shape model, output in internal_pfunc_space.
* @param dmod_entity
* out: application ptr stored with dmod.
* @param draw_state
* out: draw bits for application graphics draw loads, cstrns, and seams).
* @param tag
* out: unique id for this object.
* @param alpha
* out: resistance to stretch [1.0].
* @param beta
* out: resistance to bending [5.0].
* @param gamma
* out: resist bending change rate.
* @param delta
* out: resistance to moving from default shape.
* @param dt
* out: implicit integration time step [1.0].
* @param mass
* out: physical param (causes rippling) [1.0].
* @param damp
* out: physical param (stable integration) [5.0].
* @param parent
* out: dmod's parent pointer.
* @param sibling
* out: dmod's sibling pointer.
* @param child
* out: dmod's 1st child.
* @param load_count
* out: number of loads in dmod.
* @param cstrn_count
* out: number of constraints in dmod.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_get_dmod_curve          // eff: make a curve deformable model
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to retrieve data from, [pre-alloced]
   double &domain_scale,   // out: internal_pfunc_space = domain_scale * orig_dmod_space
   DS_pfunc *&pfunc,       // out: the dmod's shape model, [nested]
                           //      output in internal_pfunc_space
   void *&dmod_entity,     // out: application ptr stored with dmod, [pass-thru]
   int &draw_state,        // out: draw bits for application graphics
                           //      (draw loads, cstrns, and seams)
   int &tag,               // out: unique id for this object
   double &alpha,          // out: resistance to stretch [1.0]
   double &beta,           // out: resistance to bending [5.0]
   double &gamma,          // out: resist bending change rate
   double &delta,          // out: resistance to moving from default shape
   double &dt,             // out: implicit integration time step [1.0]
   double &mass,           // out: physical param (causes rippling) [1.0]
   double &damp,           // out: physical param (stable intgrtn) [5.0]
   DS_dmod *&parent,       // out: dmod's parent pointer, [pre-alloced]
   DS_dmod *&sibling,      // out: dmod's sibling pointer, [nested]
   DS_dmod *&child,        // out: dmod's 1st child, [nested]
   int    &load_count,     // out: number of loads in dmod
   int    &cstrn_count,    // out: number of constraints in dmod
   SDM_options *sdmo=NULL);// in : SDM_options pointer

//=========================================================
// DS_dmod *DM_make_dmod_surface
//=========================================================

/**
* Creates a deformable surface model and returns a pointer to the deformable model object or <tt>NULL</tt>.
* <br><br>
* <b>Role:</b> Modifies <tt>return_err</tt>.
* <br><br>
* Builds and returns a deformable model of the pfunc shape. The <tt>dmod_entity</tt>
* pointer is made available to applications that may want to store data with each
* deformable model. This pointer is never accessed (and so may be set to <tt>NULL</tt>) but
* can be retrieved by an application with the deformable model method call
* <tt>DS_dmod</tt>->Entity() and it may be changed with the method
* <tt>DS_deformable_model->Set_entity (entity)</tt>. <tt>draw_state</tt> is a bit-array integer
* which may be used by the application to control which pieces of deformable
* modeling information are rendered for each deformable model. See the <tt>DM_DRAW_...</tt>
* environment variables defined in the file <tt>"dshusk/dskernel/dmapi.hxx"</tt> for a
* complete list. The <tt>draw_state</tt> defaults to drawing loads, constraints, and seams.
* tag is unique identifier for this deformable model. Since this deformable model
* will act as the root of its own patch hierarchy a good tag value is <tt>2</tt> which is
* its default value. au <tt>(default=1)</tt> is this deformable model's resistance to
* stretch parameter value in the <i>u</i> direction, av <tt>(default=1)</tt> is this deformable
* model's resistance to stretch parameter value in the <i>v</i> direction, and atheta is
* an amount in radians by which those stretch resistance may be rotated within the
* surface. bu <tt>(default=5)</tt> is this deformable model's resistance to bending
* parameter value in the <i>u</i> direction, bv <tt>(default=5)</tt> is this deformable model's
* resistance to bending parameter value in the v direction, and btheta is an
* amount in radians by which those bending resistances may be rotated within the
* surface. delta <tt>(default=0)</tt> is this deformable model's resistance to displacement
* from the default shape. dt <tt>(default =1)</tt> is the deformable modeling time step
* size taken with each call to DM_solve(). mass <tt>(default 0.0)</tt> and damp <tt>(default 0.0)</tt>
* are the dynamic parameters for this deformable model. With mass, a deformable
* model can overshoot and oscillate on its way to an equilibrium position. With
* damping the oscillations can diminish over time and the number of time steps
* until equilibrium is achieved may be increased. When both mass and damping are 0
* (default behavior), a <tt>DM_solve()</tt> solves directly for the equilibrium position,
* ignoring the current value of <i>dt</i>.
* <br><br>
* <tt>mallocs</tt> is the calling program must free the memory of the returned deformable
* model pointer with a call to <tt>DM_delete_dmod</tt>. The pfunc pointer is stored within
* the deformable model and will be deleted when the deformable model is deleted.
* Do not call <tt>DM_delete_pfunc</tt> on that pointer after this call. Do not make a
* second deformable model with this <tt>pfunc</tt> pointer. Use <tt>DM_copy_pfunc</tt> if you want
* to make another deformable model with this shape.
* <br><br>
* <b>Errors:</b>
*<dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The <tt>pfunc</tt> cannot be <tt>NULL</tt> on input.</dd>
* <tt>DM_BAD_TAG_VALUE</tt>
* The tag value must be -1, to have the system assign one, or 2 or greater.
*<dt><tt>DM_BAD_ALPHA_VALUE</tt></dt>
*<dd>The value of alpha must be 0 or positive.</dd>
*<dt><tt>DM_BAD_BETA_VALUE</tt></dt>
*<dd>The value of beta must be 0 or positive.</dd>
*<dt><tt>DM_BAD_DELTA_VALUE</tt></dt>
*<dd>The value of delta must be positive.</dd>
*<dt><tt>DM_BAD_DT_VALUE</tt></dt>
*<dd>The value of dt must be positive.</dd>
*<dt><tt>DM_BAD_MASS_VALUE</tt></dt>
*<dd>The value of mass must be 0 or positive.</dd>
*<dt><tt>DM_BAD_DAMP_VALUE</tt></dt>
*<dd>The value of damp must be 0 or positive.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param pfunc
* in: defines the shape model.
* @param dmod_entity
* in: application ptr stored with dmod.
* @param draw_state
* in: draw bits for application graphics (draw loads, constraints, and seams).
* @param tag
* in: unique id for this object.
* @param au
* in: resist stretch u direction.
* @param av
* in: resist stretch v direction.
* @param atheta
* in: resist stretch rotation angle.
* @param bu
* in: resist bending u direction.
* @param bv
* in: resist bending v direction.
* @param btheta
* in: resist bending rotation angle.
* @param gamma
* in: resist bending change rate.
* @param delta
* in: resistance to moving from default shape.
* @param dt
* in: implicit integration time step [1.0].
* @param mass
* in: physical param (causes rippling)[1.0].
* @param damp
* in: physical param (stable integration) [5.0].
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM DS_dmod *          // rtn: ptr to DS_dmod object or NULL, [malloc]
DM_make_dmod_surface       // eff: make a deformable surface model
  (int &rtn_err,           // out: 0=success or negative err code
   DS_pfunc *pfunc,        // in : defines the shape model, [nested]
   void *dmod_entity=NULL, // in : application ptr stored with dmod, [pass-thru]
   int draw_state=         // in : draw bits for application graphics
        SPA_DM_DRAW_SEAMS  |   //      default = draw seams
        SPA_DM_DRAW_CSTRNS |   //                     cstrns and
        SPA_DM_DRAW_LOADS,     //                     loads
   int tag = 2,            // in : unique id for this object
   double au     = 1.0,    // in : resist stretch u direction
   double av     = 1.0,    // in : resist stretch v direction
   double atheta = 0.0,    // in : resist stretch rotation angle
   double bu     = 5.0,    // in : resist bending u direction
   double bv     = 5.0,    // in : resist bending v direction
   double btheta = 0.0,    // in : resist bending rotation angle
   double gamma  = 0.0,    // in : resist bending change rate
   double delta  = 0.0,    // in : resistance to moving from default shape
   double dt     = 1.0,    // in : implicit integration time step
   double mass   = 0.0,    // in : physical param (causes rippling)
   double damp   = 0.0,    // in : physical param (stabel intgrtn)
   SDM_options *sdmo=NULL);// in : SDM_options pointer


//=========================================================
// DS_dmod *DM_get_dmod_surface
//=========================================================

/**
* Gets deformable surface data.
* <br><br>
* <b>Role:</b> Modifies <tt>pfunc</tt>, <tt>draw_state</tt>, <tt>tag</tt>, <tt>au</tt>, <tt>av</tt>, <tt>atheta</tt>, <tt>bu</tt>, <tt>bv</tt>, <tt>btheta</tt>,
* <tt>delta</tt>, <tt>dt</tt>, <tt>mass</tt>, and <tt>damp</tt>
* <br><br>
* Loads the output variables with data from the input deformable model object.
*<br><br>
* <i><b>Note:</b> the current value of pfunc will be overwritten so take care that it is not
* the only pointer to a current <tt>DS_pfunc</tt> object, otherwise a memory leak may occur.
* See function <tt>DM_make_dmod_surface()</tt> for definitions of the output arguments and
* their use.</i>
* <br><br>
* The <tt>pfunc</tt> is loaded with a pointer to the contained <tt>DS_pfunc</tt> object which exists
* in the <tt>internal_pfunc_space</tt>. The <tt>pfunc</tt>'s domain space range is related to the
* <tt>DS_dmod</tt>'s domain space range by the <tt>domain_scale</tt> factor as:
* <br><br>
* <tt>internal_pfunc_space = domain_scale * orig_dmod_space.</tt>
* <br><br>
* <b>Errors:</b>
*<dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd>
*<dt><tt>DM_DMOD_NOT_A_SURFACE</tt></dt>
*<dd>The deformable model must be a surface.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to retrieve data from.
* @param dmod_entity
* out: application ptr stored with dmod.
* @param domain_scale
* out: internal_pfunc_space = domain_scale * orig_dmod_space.
* @param pfunc
* out: the dmod's shape model output in internal_pfunc_space.
* @param draw_state
* out: draw bits for application graphics (draw loads, cstrns, and seams).
* @param tag
* out: unique id for this object.
* @param au
* out: u dir resistance to stretch [1.0].
* @param av
* out: v dir resistance to stretch [1.0].
* @param atheta
* out: resist stretch rotation angle.
* @param bu
* out: u dir resistance to bending [5.0].
* @param bv
* out: v dir resistance to bending [5.0].
* @param btheta
* out: resist bending rotation angle.
* @param gamma
* out: resist bending change rate.
* @param delta
* out: resistance to moving from default shape.
* @param dt
* out: implicit integration time step [1.0].
* @param mass
* out: physical param (causes rippling) [1.0].
* @param damp
* out: physical param (stable integration) [5.0].
* @param parent
* out: dmod's parent pointer.
* @param sibling
* out: dmod's sibling pointer.
* @param child
* out: dmod's 1st child.
* @param load_count
* out: number of loads in dmod.
* @param cstrn_count
* out: number of constraints in dmod.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_get_dmod_surface        // eff: retrieve deformable surface data
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to retrieve data from, [pre-alloced]
   void *&dmod_entity,     // out: application ptr stored with dmod, [pass-thru]
   double &domain_scale,   // out: internal_pfunc_space
                           //      = domain_scale*orig_dmod_space.
   DS_pfunc *&pfunc,       // out: the dmod's shape model, [nested]
                           //      output in internal_pfunc_space.
   int &draw_state,        // out: draw bits for application graphics
                           //      (draw loads, cstrns, and seams)
   int &tag,               // out: unique id for this object
   double &au,             // out: u dir resistance to stretch [1.0]
   double &av,             // out: v dir resistance to stretch [1.0]
   double &atheta,         // out: resist stretch rotation angle
   double &bu,             // out: u dir resistance to bending [5.0]
   double &bv,             // out: v dir resistance to bending [5.0]
   double &btheta,         // out: resist bending rotation angle
   double &gamma,          // out: resist bending change rate
   double &delta,          // out: resistance to moving from default shape
   double &dt,             // out: implicit integration time step [1.0]
   double &mass,           // out: physical param (causes rippling) [1.0]
   double &damp,           // out: physical param (stable intgrtn) [5.0]
   DS_dmod *&parent,       // out: dmod's parent pointer, [pre-alloced]
   DS_dmod *&sibling,      // out: dmod's sibling pointer, [nested]
   DS_dmod *&child,        // out: dmod's 1st child, [nested]
   int    &load_count,     // out: number of loads in dmod
   int    &cstrn_count,    // out: number of constraints in dmod
   SDM_options *sdmo=NULL);// in : SDM_options pointer

//=========================================================
// DM_get_dmod_... _pfunc, _tag, _type_id, _cstrn, _load
//=========================================================

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM int                  // rtn: new tag id or negative err code
DM_make_tracking_curve_dmod  // eff: build crv_dmod from crv or link_cstrn
  (int &rtn_err,             // out: 0=success or neg err code
   DS_dmod *dmod,            // in : dmod to query, [pre-alloced]
   int crv_tag,              // in : tag of crv or link_cstrn
   int crv_tgt,              // in : for link_cstrns,tgt = 1 or 2
   int tag=-1,               // in : when == -1 assign next tag number
                             //       else use this tag number
   SDM_options *sdmo=NULL);  // in : SDM_options pointer
                             // note: sets active dmod
/**
* Creates a <tt>pfunc</tt> copy with the <tt>pfunc</tt>'s domain set to equal the <tt>orig_dmod_space</tt> domain range.
* <br><br>
* <b>Role:</b> Copies the <tt>DS_pfunc</tt> ptr contained within the input dmod object
* and scales its domain so that the returned <tt>pfunc</tt>'s domain is equal to the
* <tt>dmod'sorig_dmod_space</tt>.
* <br><br>
* This function mallocs the returned <tt>DS_pfunc</tt> object, and it is the responsibility
* of the calling program to free this memory when done with it.
* <br><br>
* <tt>internal_pfunc_space = domain_scale * orig_dmod_space.</tt>
* <br><br>
* The domain_scale factor may be queried by the function,
* <tt>DM_get_dmod_domain_scale()</tt>.
* <br><br>
* <b>Errors:</b>
*<dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: dmod to query.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM DS_pfunc *           // rtn: pfunc copy in orig_dmod_space, [malloc]
DM_make_orig_dmod_space_pfunc// eff: copy pfunc with orig_dmod_space domain
  (int &rtn_err,             // out: 0=success or neg err code
   DS_dmod *dmod,            // in : dmod to query, [pre-alloced]
   SDM_options *sdmo=NULL);  // in : SDM_options pointer
                             // mallocs: returned pfunc object

/**
* Gets deformable model's <tt>pfunc</tt> and returns input's <tt>DS_pfunc</tt> shape pointer.
* <br><br>
* <b>Role:</b> Returns the <tt>ds_pfunc</tt> pointer contained within the input
* deformable model. The contained <tt>DS_pfunc</tt> object's domain defines
* <tt>internal_pfunc_space</tt>. This space is related to the <tt>pfunc</tt>'s original domain range
* by a <tt>domain_scale</tt> factor as:
* <br><br>
* <tt>internal_pfunc_space = domain_scale * orig_dmod_space</tt>.
* <br><br>
* The <tt>domain_scale</tt> factor may be queried by the function,
* <tt>DM_get_dmod_domain_scale()</tt>.
* <br><br>
* <b>Errors:</b>
*<dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to query.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM DS_pfunc *         // rtn: input's DS_pfunc shape ptr, [nested]
DM_get_dmod_pfunc          // eff: retrieve and return dmod's pfunc
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to query, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the type for <tt>DS_pfunc</tt> and returns the deformable model's tag number or an error.
* <br><br>
* <b>Errors:</b>
*<dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to query.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM int                // rtn: dmod's tag number or error number
DM_get_dmod_tag            // eff: check and return type for DS_pfunc
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to query, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Checks and returns the tag type for <tt>DS_pfunc</tt> and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Sets the tag number for the input deformable model.
* <br><br>
* <b>Errors:</b>
*<dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to modify.
* @param tag
* in: unique tag identifier to match.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM void
DM_set_dmod_tag            // eff: check and return type for DS_pfunc
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to modify, [pre-alloced]
   int tag,                // in : dmod's new tag value
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: make sure that tag is unique

/**
* Gets the tag type for <tt>DS_pfunc</tt> and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Returns the input deformable model's type identifier. The type
* identifier is one of <tt>DS_tag_srf_dmod</tt> or <tt>DS_tag_crv_dmod</tt>.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to query.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM DS_TAGS            // rtn: DS_tag_srf_dmod for surfaces
                           //      DS_tag_crv_dmod for curves
                           //      DS_tag_track_pos_crv
                           //      DS_tag_track_tan_crv
                           //      DS_tag_track_curv_crv
DM_get_dmod_type_id        // eff: check and return type for DS_pfunc
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to query, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM void
DM_set_dmod_type_id        // eff: check and return type for DS_pfunc
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to modify, [pre-alloced]
   DS_TAGS type_id,        // in : dmod's new type_id value
                           // DS_tag_srf_dmod,
                           // DS_tag_crv_dmod,
                           // DS_tag_track_pos_crv,
                           // DS_tag_track_tan_crv,
                           // DS_tag_track_curv_crv
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the deformable model's first constraint if it exists.
* <br><br>
* <b>Role:</b> When the deformable model has a constraint, the output
* constraint pointer is set to the first constraint object on the linked list of
* constraints.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to query.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM DS_cstrn *         // rtn: dmod's first cstrn or NULL, [nested]
DM_get_dmod_cstrn          // eff: search dmod for a cstrn
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to query, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer


/**
* Gets the deformable model for a load and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> When the deformable model has a load, this function sets the
* output load pointer to the first load object on the linked list of loads.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to query.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM DS_load *          // rtn: dmod's first load or NULL, [nested]
DM_get_dmod_load           // eff: search dmod for a load
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to query, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the minimum domain point for a dmod and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies <tt>domain_min</tt>.
* <br><br>
* Retrieves the dmod's domain's minimum bounding point and stores it in <tt>domain_min</tt>.
* <br><br>
* The returned domain point location can be reported in either the dmod's domain
* range (the original range with which the dmod was created), the unit_range where
* the point is scaled to range from <tt>0.0 to 1.0</tt>, or the <tt>pfunc</tt>'s domain range(the
* actual value used internally). When the <tt>domain_flag</tt> is set to <tt>0</tt>, <tt>domain_min</tt> is
* returned in <tt>orig_dmod_space</tt>, when the <tt>domain_flag</tt> is set to <tt>1</tt>, <tt>domain_min</tt> is
* returned in <tt>unit_space</tt>, and when the <tt>domain_flag</tt> is set to <tt>2</tt>, <tt>domain_min</tt> is
* returned in <tt>internal_pfunc_space</tt>.
* <br><br>
* <i><b>Note:</b> The <tt>DS_dmod</tt>'s domain range may be different from its contained
* <tt>DS_pfunc</tt>'s domain range. The <tt>DS_dmod</tt> domain range remains constant during a
* deformable modeling session while the <tt>DS_pfunc</tt> domain range may change.
* Initially they start out the same.</i>
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd>
* <dt><tt>DM_NULL_OUTPUT_PTR</tt></dt>
* <dd>The domain min cannot <tt>NULL</tt> on entry.</dd></dl>.
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to query.
* @param domain_flag
* n: 0=domain_min in orig_dmod_space, 1=domain_min in unit_space, 2=domain_min in internal_pfunc_space.
* @param domain_min
* out: ptr to max_pt array, sized:[domain_dim].
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_get_dmod_domain_min     // eff: query dmod for min domain pt
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to query, [pre-alloced]
   int domain_flag,        // in : 0=domain_min in orig_dmod_space,
                           //      1=domain_min in unit_space,
                           //      2=domain_min in internal_pfunc_space.
   double *domain_min,     // out: ptr to min_pt array, [pre-alloced]
                           //      sized:[DM_get_domain_dim(rtn_err, dmod)]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the maximum domain point for a dmod and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies <tt>domain_max</tt>.
* <br><br>
* Retrieves the dmod domain's maximum bounding point and stores it in <tt>domain_max</tt>.
* <br><br>
* The returned domain point location can be reported in either the dmod's domain
* range (the original range with which the dmod was created), the unit_range where
* the point is scaled to range from <tt>0.0 to 1.0</tt>, or the <tt>pfunc</tt>'s domain range(the
* actual value used internally). When the <tt>domain_flag</tt> is set to <tt>0</tt>, <tt>domain_min</tt> is
* returned in <tt>orig_dmod_space</tt>, when the <tt>domain_flag</tt> is set to <tt>1</tt>, <tt>domain_min</tt> is
* returned in <tt>unit_space</tt>, and when the <tt>domain_flag</tt> is set to <tt>2</tt>, <tt>domain_min</tt> is
* returned in <tt>internal_pfunc_space</tt>.
* <br><br>
* <i><b>Note:</b> The <tt>DS_dmod</tt>'s domain range may be different from its contained
* <tt>DS_pfunc</tt>'s domain range. The <tt>DS_dmod</tt> domain range remains constant during a
* deformable modeling session while the <tt>DS_pfunc</tt> domain range may change.
* Initially they start out the same.</i>
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The pfunc cannot be NULL on entry.</dd>
* <dt><tt>DM_NULL_OUTPUT_PTR</tt></dt>
*<dd>The domain max is NULL on entry.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to query.
* @param domain_flag
* in: 0=domain_max in orig_dmod_space, 1=domain_max in unit_space, 2=domain_max in internal_pfunc_space.
* @param domain_max
* out: ptr to max_pt array, sized:[domain_dim].
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_get_dmod_domain_max     // eff: query dmod for max domain pt
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to query, [pre-alloced]
   int domain_flag,        // in : 0=domain_max in orig_dmod_space,
                           //      1=domain_max in unit_space,
                           //      2=domain_min in internal_pfunc_space.
   double *domain_max,     // out: ptr to min_pt array, [pre-alloced]
                           //      sized:[DM_get_domain_dim(rtn_err, dmod)]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Returns the scale factor that relates the adjustable <tt>DS_pfunc</tt> domain range to the constant <tt>DS_dmod</tt> domain range.
* <br><br>
* <b>Role:</b> Returns the deformable model's domain scale factor.
* <br><br>
* The domain range of a <tt>DS_dmod</tt> deformable model is related to the domain range of
* the <tt>DS_dmod</tt>'s contained <tt>DS_pfunc</tt> object by a simple scaling:
* <br><br>
* <tt>internal_pfunc_space = domain_scale * orig_dmod_space.</tt>
* <br><br>
* At the time that a <tt>DS_pfunc</tt> is used to create a <tt>DS_dmod</tt> deformable modeling
* object <tt>(DM_make_dmod_surface</tt>, <tt>DM_make_dmod_curve</tt>, <tt>DM_add_curve_patch</tt>,
* <tt>DM_add_surface_patch</tt>) or when the domain of a deformable model is split
* (<tt>DM_split_dmod</tt>), the domain of the contained <tt>DS_pfunc</tt> may be scaled to optimize
* the numerical computation of high order derivative terms.
* <br><br>
* The domain scaling will occur when the minimum knot spacing becomes less than <tt>0.3.</tt>
*This scaling is for internal use only and in most places in the deformable
* modeling interface input and output happen in the original domain range called
* the <tt>orig_dmod_space</tt>. Some query functions return pointers to the internal
* <tt>DS_pfunc</tt> data to allow direct access of the stored data. In these circumstances,
* the interface exposes the internally used <tt>internal_pfunc_space</tt>. Any application
* which uses those values will have to be aware of the scaling between the
* <tt>internal_pfunc_space</tt> and the <tt>orig_dmod_space</tt>.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The pfunc cannot be NULL on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to query.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM double             // rtn: domain_scale of
DM_get_dmod_domain_scale   //      current_pfunc_domain =
                           //      domain_scale * orig_dmod_space
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to query
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Calculates image positions for the <i>uv</i> location and returns <tt>0</tt> or an error.
* <br><br>
* <b>Role:</b> Modifies <tt>W</tt>, <tt>dWu</tt>, <tt>dWv</tt>, <tt>dWuu</tt>, <tt>dWuv</tt>, <tt>dWvv</tt>, <tt>dWuuu</tt>, <tt>dWuuv</tt>, <tt>dWuvv</tt>,
* <tt>dWvvv</tt>.
* <br><br>
* This function is a convenience routine for accessing <tt>DM_eval_pfunc</tt> given a dmod.
* This function extracts the input dmod's <tt>DS_pfunc</tt> pointer and passed the call
* along to <tt>DM_eval_pfunc</tt>.
* <br><br>
* The <tt>DM_eval_dmod</tt> function uses the dmod's contained pfunc to evaluate the image
* space values for the input domain point location.
* <br><br>
* When <tt>domain_flag == 0</tt>, dpt is given in the dmod's original domain range. When
* <tt>domain_flag == 1</tt>, dpt is given in the unit-square range. When <tt>domain_flag == 2</tt>,
* dpt is given in the <tt>pfunc</tt>'s internal domain range.
* <br><br>
* <i><b>Note:</b> the orig_dmod_space is invariant, while the internal_pfunc_space may vary.
* See DM_get_dmod_domain_scale().</i>
* <br><br>
* The function can compute and store the following,
* <br><br>
*<table border="0" width="100%">
*<tr>
*<td width="5%" align="left"><tt>W</tt></td>
*<td width="5%"></td>
*<td width="90%">Position <tt>[x,y,z]</tt></td>
* </tr>
*<tr>
*<td width="5%" align="left"> <tt>dWu</tt></td>
*<td width="5%"></td>
*<td width="90%">1st parametric derivative in <i>u</i> direction</td>
*</tr>
*<tr>
*<td width="5%" align="left"><tt>dWv</tt></td>
*<td width="5%"></td>
* <td width="90%"> 1st parametric derivative in <i>v</i> direction</td>
*</tr>
*<tr>
*<td width="5%" align="left"> <tt>dWuu</tt></td>
*<td width="5%"></td>
*<td width="90%"> 2nd parametric derivative in <i>uu</i> direction</td>
*</tr>
*<tr>
*<td width="5%" align="left"> <tt>dWuv</tt></td>
*<td width="5%"></td>
*<td width="90%"> 2nd parametric derivative in <i>uv</i> direction</td>
*</tr>
*<tr>
*<td width="5%" align="left"> <tt>dWvv</tt></td>
*<td width="5%"></td>
*<td width="90%">&nbsp;2nd parametric derivative in <i>vv</i> direction</td>
*</tr>
*<tr>
*<td width="5%" align="left"> <tt>dWuuu</tt></td>
*<td width="5%"></td>
*<td width="90%"> 3rd parametric derivative in <i>uuu</i> direction</td>
*</tr>
*<tr>
*<td width="5%" align="left"><tt>dWuuv </tt></td>
*<td width="5%"></td>
*<td width="90%"> 3rd parametric derivative in <i>uuv</i> direction</td>
*</tr>
*<tr>
*<td width="5%" align="left"> <tt>dWuvv</tt></td>
*<td width="5%"></td>
*<td width="90%"> 3rd parametric derivative in <i>uvv</i> direction</td>
*</tr>
*<tr>
*<td width="5%" align="left"> <tt>dWvvv</tt></td>
*<td width="5%"></td>
*<td width="90%"> 3rd parametric derivative in <i>vvv</i> direction</td>
*</tr>
*</table>
*<br>
* The values for any output array set to <tt>NULL</tt> are not computed or stored. All
* output arrays, except for <i>W</i>, the position, default to <tt>NULL</tt> and may be omitted.
* <br><br>
* Surfaces can load all six output variables, Curves will only load <tt>W</tt>, <tt>dWu</tt>, and
* <tt>dWuu</tt> and will skip the others.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The dpt input pointer cannot be NULL.</dd>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd>
* <dt><tt>DM_NULL_OUTPUT_PTR</tt></dt>
*<dd>The alpha cannot be NULL on entry.</dd>
*<dt><tt>DM_NON_NULL_OUTPUT_PTR</tt></dt>
*<dd>The output pointer cannot be NULL on entry.</dd>
*<dt><tt>DM_BAD_DOMAIN_DIM</tt></dt>
*<dd>The domain dimension must equal 1 or 2.</dd>
*<tt>DM_parse_tag_flag()</tt> errors
*<dt><tt>DM_scale_unit_dpt_to_pfunc()</tt></dt>
*<dd>errors</dd>
*<dt><tt>DM_BAD_DOMAIN_PT_RANGE</tt></dt>
*<dd>The domain point must be completely contained by the deformable model.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: dmod to evaluate.
* @param domain_flag
* in: define dpt space 0=orig_dmod_space, 1=unit_space, 2= internal_pfunc_space.
* @param dpt
* in: domain point for evaluation sized:[domain_dim].
* @param W
* out: dmod xyz position for dpt loc W=[x,y,z], sized:[image_dim].
* @param dWu
* out: u_dir tang or NULL dWu=[dx,dy,dz], sized:[image_dim].
* @param dWv
* out: v_dir tang or NULL dWv=[dx,dy,dz], sized:[image_dim].
* @param dWuu
* out: uu parameter derivative or NULL dWuu=[dxx,dyy,dzz], sized:[image_dim].
* @param dWuv
* out: uv parameter derivative or NULL dWuv=[dxx,dyy,dzz], sized:[image_dim].
* @param dWvv
* out: uu parameter derivative or NULL dWvv=[dxx,dyy,dzz], sized:[image_dim].
* @param dWuuu
* out: uuu parametric derivative or NULL [dxx,dyy,dzz], sized:[image_dim].
* @param dWuuv
* out: uuv parametric derivative or NULL [dxx,dyy,dzz], sized:[image_dim].
* @param dWuvv
* out: uvv parametric derivative or NULL [dxx,dyy,dzz], sized:[image_dim].
* @param dWvvv
* out: vvv parametric derivative or NULL [dxx,dyy,dzz], sized:[image_dim].
* @param sdmo
* in:SDM_options pointer note: sets active dmod note: any output array set to NULL is not computed or stored.
**/
DECL_DM void
DM_eval_dmod               // eff: calc image positions for uv location
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to query, [pre-alloced]
   int domain_flag,        // in : 0=dpt in orig_dmod_space,
                           //      1=dpt in unit_space,
                           //      2=dpt in internal_pfunc_space.
   double *dpt,            // in : domain point for evaluation, [pre-alloced]
                           //      [u] or [u,v]
                           //      sized:[DM_get_domain_dim(rtn_err, dmod)]
   double *W,              // i/o: dmod xyz SPAposition for dpt loc, [pre-alloced]
                           //      W=[x,y,z]
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
   double *dWu =NULL,      // i/o: u_dir tang or NULL, [pre-alloced]
                           //      dWu=[dx,dy,dz]
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
   double *dWv =NULL,      // i/o: v_dir tang or NULL, [pre-alloced]
                           //      dWv=[dx,dy,dz]
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
   double *dWuu=NULL,      // i/o: uu parametric derivative or NULL, [pre-alloced]
                           //      [dxx,dyy,dzz]
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
   double *dWuv=NULL,      // i/o: uv parametric derivative or NULL, [pre-alloced]
                           //      [dxx,dyy,dzz]
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
   double *dWvv=NULL,      // i/o: vv parametric derivative or NULL, [pre-alloced]
                           //      [dxx,dyy,dzz]
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
   double *dWuuu=NULL,     // i/o: uuu parametric derivative or NULL, [pre-alloced]
                           //      [dxx,dyy,dzz]
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
   double *dWuuv=NULL,     // i/o: uuv parametric derivative or NULL, [pre-alloced]
                           //      [dxx,dyy,dzz]
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
   double *dWuvv=NULL,     // i/o: uvv parametric derivative or NULL, [pre-alloced]
                           //      [dxx,dyy,dzz]
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
   double *dWvvv=NULL,     // i/o: vvv parametric derivative or NULL, [pre-alloced]
                           //      [dxx,dyy,dzz]
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod
                           // note: any output array set to NULL
                           //       is not computed or stored

/**
* Modifies the shape of the deformable model to fit to its constraints.
* <br><br>
* <b>Role:</b> Modifies the target deformable model's control point locations
* (dofs). This function should be used after sculpting.
* <br><br>
* This function is used to minimize the numerical tolerances seen when enforcing
* the deformable modeling constraints on a deformable model.
* <br><br>
* It is intended to be used as a single post-processing step after sculpting is
* completed but before the deformed shape is used in a geometric modeling
* application.
* <br><br>
* This function tweaks the deformable model's shape slightly to improve the
* tolerances held by the curve constraints. Typically this function moves dof
* values slightly <tt>(1.E-05)</tt> to improve the constraint tolerances from about <tt>1.E-05</tt>
* to <tt>3.E-07</tt>. This function is expensive to run and should only be run to restore
* the curve constraint tolerances after a sculpting session. This function should
* only be run once per sculpting session.
* <br><br>
* Before refining, this function stores in <tt>beg_crv_dist_err</tt> the maximum distance
* between an edge and the surface's curve points as found by<br>
* <tt>DS_crv_->Compare_src_to_out_W_pts()</tt>. When <tt>beg_crv_dist_err</tt> is less than <tt>err_size</tt>
* processing aborts and a value of <tt>0</tt> is returned. Otherwise, the refinement
* processing is executed by calling the <tt>DS_symeq->Refine_Cx_equal_d()</tt> function to
* improve compliance with the constraints and computes and stores in
* <tt>end_crv_dist_err</tt> the refined distance error. The output residual is set with the
* maximum residual error in the constraint equations,
* <br><tt>res = Cx - d </tt>.
* <br><br>
* Increased compliance to the curve constraint equations is achieved by moving the
* dofs the minimum amount of distance to put the dof state back into the range of
* the constraint matrix. In other words, this function moves the control points to
* improve compliance with the constraints.
* <br><br>
* When the input dmod deformable model is one root sibling of a multi-surface
* sibling list, its link constraints are treated like curve constraints and the
* refinement is run only on the input dmod. To refine an entire sibling list, call
* this function once on each root sibling.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of hierarchy to modify.
* @param err_size
* in: max ok crv_err without postprocessing.
* @param residual
* out: abs max elem value in r = Cx-d.
* @param beg_crv_dist_err
* out: max edge/crv_cstrn dist before refine.
* @param end_crv_dist_err
* out: max edge/crv_cstrn dist after refine.
* @param max_dist_moved
* out: max distance one dof moved.
* @param walk_flag
* in: specify how deep to go 0=dmod only, 1=dmod and offspring 3=dmod,siblings and offspring.
* @param sdmo
* in:SDM_options  pointer eff: Moves the dofs by a minimal amount to minimize the residual of the constraint equations, r = Cx - d.
**/

DECL_DM void
DM_refine_dmod_fit         // use: After sculpting, prior to using shape
 (int &rtn_err,            // out: 0=success or neg err code
  DS_dmod *dmod,           // in : member of hierarchy to modify, [pre-alloced]
  double err_size,         // in : max ok crv_err without postprocessing
  double &residual,        // out: abs max elem value in r = Cx-d
  double &beg_crv_dist_err,// out: max edge/crv_cstrn dist before refine
  double &end_crv_dist_err,// out: max edge/crv_cstrn dist after refine
  double &max_dist_moved,  // out: max distance one dof moved
  int walk_flag=0,         // in : specify how deep to go
                           //      0=dmod only,1=dmod and offspring
                           //      2=dmod,siblings,and offspring
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // eff: Moves the dofs by a minimal amount
                           //      to minimize the residual of the
                           //      constraint equations, r = Cx - d.
// TODO:!!! a walk_flag value for siblings but no offspring

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM void
DM_find_max_gap            // use: Find maximum deviation along curve cstrn
 (int &rtn_err,            // out: 0=success or neg err code
  DS_dmod *dmod,           // in : member of hierarchy to modify
  int tag,                 // in : tag of curve to test
  double &max_dist,        // out: largest displacement error
  double &s_val,           // out: curve param it occurs at
  double *const uv_val,    // out: uv value it occurs at sized:[2]
   SDM_options *sdmo=NULL);// in : SDM_options pointer
//=========================================================
// DM_copy_dmod, DM_delete_dmod, DM_next_dmod
//=========================================================
/**
* Creates a deep copy of the input deformable model and returns a pointer to new deformable model object.
* <br><br>
* <b>Role:</b> Makes and returns a pointer to a deep copy of the input
* deformable model.
* <br><br>
* <tt>NULL</tt> for failure or a pointer to an independent copy of of the input deformable
* model object.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd>
*<dt><tt>DM_BAD_WALK_FLAG_VALUE</tt></dt>
*<dd>The walk_flag must be 0, 1 or 2.</dd></dl>
* <br>
* <b>Effect:</b> System routine
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: dmod object to copy.
* @param walk_flag
* in: 0='this' only, 1='this' and offspring.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM DS_dmod *          // rtn: ptr to new DS_dmod object or NULL, [malloc]
DM_copy_dmod               // eff: make a deep copy of input dmod
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod object to copy, [pre-alloced]
   int walk_flag=0,        // in : specify how deep to go
                           //      0=dmod only,1=dmod and offspring
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Clears the deformable model's data structure.
* <br><br>
* <b>Role:</b> Frees and clears all data values associated with the underlying
* deformable model and sets the deformable model pointer to <tt>NULL</tt>. Recursively
* deletes all of the dmod's children and all of the dmod's younger siblings.
* <br><br><ol>
*<li>The deformable model pointer is invalid after this call.</li>
* <li>The <tt>pfunc</tt> object referenced by this deformable model is also deleted. Do
* not call <tt>DM_delete_pfunc</tt> on that object.</li>
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd></dl>
* <br>
* <b>Effect:</b> System routine
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* i/o: ptr to valid dmod object set to NULL on exit.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_delete_dmod             // eff: free the dmod data structure
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *&dmod,         // i/o: ptr to valid dmod object, [freed]
                           //      set to NULL on exit.
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM DS_dmod *          // rtn: next dmod ptr in sequence or NULL, [pre-alloced]
DM_next_dmod               // eff: rtn sequence of dmods for 1 recursion level
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *tgt_dmod,      // in : current dmod in recursive subroutine
   int walk_flag,          // in : specify how deep to go
                           //      0='this' only,
                           //      1='this' and offspring
                           //      2='this',sibling, and offspring
   DS_cstrn *&next_cstrn,  // i/o: cstrn to start src_W_dmod search, [nested]
                           //      or NULL to skip src_W_dmod search
                           //      see DM_get_cstrn_dmod()
   int &dmod_flag,         // i/o: dmod to start child,sibling search
                           //      1=rtn sibling next if appropriate
                           //      2=rtn child next if appropriate
                           //      0=all done (rtn NULL on next call)
   SDM_options *sdmo=NULL);// in : SDM_options pointer


//=========================================================
// DM_add_curve_patch(), DM_add_surface_patch()
//=========================================================

/**
* Makes and adds a patch- to-patch hierarchy and returns a new patch tag identifier or an error.
* <br><br>
* <b>Role:</b> Modifies the target deformable model.
* <br><br>
* Adds a patch to the input deformable model. The domain of the patch is a span
* connected to its parent by a seam which consists of two point constraints. The
* patch may be made to connect to its parent with <tt>C0</tt> (position only) or <tt>C1</tt>
* (position and tangent) continuity but is returned by this function with <tt>C1</tt>
* continuity.
* <br><br>
* The size of the span is specified by the minimum and maximum <i>u</i>
* values which are given in the parent's domain space. When <tt>domain_flag</tt> is <tt>1</tt> the
* min and max <i>u</i> values are specified in a range that varies from 0 to 1 (and is
* scaled to the parent's dmod domain space in this function), and when <tt>domain_flag</tt>
* is <tt>0</tt> the min and max u values are specified in the parent's dmod domain space.
* When <tt>domain_flag</tt> is <tt>2</tt>, the min and max <i>u</i> values are specified in the parent's
* <tt>pfunc</tt>'s domain space.
* <br><br>
* The refinement argument specifies the number of control points in the child
* patch compared to the parent patch. For example, a refinement value of <tt>2</tt> will
* double the density of control points in the child compared to the parent.
* Refinement must be an integer value greater than 0. The patch entity pointer is
* made available to applications. It is stored with the patch's deformable model
* data structure but is never accessed. Applications can retrieve this pointer
* with the deformable model method call, <tt>DS_deformable</tt> model. Similarly, the seam
* data pointers are made available to applications to store data with each seam.
* These pointers are never accessed but can be accessed by applications with the
* call, <tt>DS_deformable model->Seam(ii)->Src_data()</tt>;
* <br><br>
* Returns the tag of the newly created patch if successful, or an error.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd>
*<dt><tt>DM_scale_unit_dpt_to_pfunc()</tt></dt>
*<dd>Errors</dd>
*<dt><tt>DM_UNCONTAINED_CHILD</tt></dt>
*<dd>The input patch boundary must be in the range of 0 to 1.</dd>
*<dt><tt>DM_BAD_REFINEMENT_VALUE</tt></dt>
*<dd>The input refinement must be larger than 0.</dd>
*<dt><tt>DM_PATCH_OCCLUDES_ROOT</tt></dt>
*<dd>The input patch completely covers the deformable model.</dd>
*<dt><tt>DM_BAD_DOMAIN_PT_RANGE</tt></dt>
*<dd>The domain point must be completely contained by the deformable model.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: dmod to be theparent.
* @param domain_flag
* in : 0=domain_pts in orig_dmod_space, 1=domain_pts in unit_space. 2=domain_pts in internal_pfunc_space.
* @param min
* in: min-domain (0 to 1 range).
* @param max
* in: max-domain (0 to 1 range).
* @param refinement
* in: parent_knot_spacing/ child_knot_spacing.
* @param patch_entity
* in: app entity ptr stored with patch.
* @param seam_data
* in: app entity data stored with each seam.
* @param sdmo
* in:SDM_options pointer.
**/

DECL_DM int                // rtn: new patch tag id or neg err code
DM_add_curve_patch         // eff: make and add a patch to patch hierarchy
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be the parent, [pre-alloced]
   int domain_flag,        // in : 0=domain_pts in orig_dmod_space,
                           //      1=domain_pts in unit_space,
                           //      2=domain_pts in internal_pfunc_space.
   double min,             // in : min-domain
   double max,             // in : max-domain
   int refinement,         // in : parent_knot_spacing/child_knot_spacing
   void *patch_entity,     // in : app entity ptr stored with patch, [pass-thru]
   void *seam_data[2],     // in : app entity data stored with each seam, [pre-alloced]
                           //      the seam_data is [nested], the array is not.
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Creates and adds a patch to the patch hierarchy and returns a new patch tag identifier or an error.
* <br><br>
* <b>Role:</b> Adds a patch to the input deformable model. The domain of the
* patch is a rectangle connected to its parent by a seam. The size of the patch
* domain is given by its minimum and maximum corner points, <tt>pt1</tt> and <tt>pt2</tt>. The shape
* of the seam is given by a set of domain (<tt>2D</tt>) curves that all must lie within the
* given patch domain. Think of the patch domain as a bounding box for all the seam
* curves. (The convenience methods, <tt>DM_build_square_seam()</tt> and
* <tt>DM_build_ellipse_seam()</tt> can be used to build the input arguments, <tt>seam_count</tt> and
* <tt>seam</tt>, for either a square or an elliptical patch seam boundary and will properly
* assign tag numbers for each of the seam entities).
* <br><br>
* Domain pts, <tt>pt1</tt> and <tt>pt2</tt>, and the image of the seam curves can be given in either
* the <tt>unit_space</tt>, the <tt>orig_dmod_space</tt>, or the <tt>internal_pfunc_space</tt>. When
* <tt>domain_flag</tt> is set to 0, these values are expected to be in the orig_dmod_space.
* When <tt>domain_flag</tt> is set to <tt>1</tt>, these values are expected to be in the <tt>unit_space</tt>.
* When <tt>domain_flag</tt> is set to <tt>2</tt>, these values are expected to be in the
* <tt>internal_pfunc_space</tt>. The <tt>orig_dmod_space</tt> is the domain range equal to the
* <tt>pfunc</tt>'s domain range at the time the dmod was constructed. The
* <tt>internal_pfunc_space</tt> is the domain range being used internally by the dmod's
* <tt>pfunc</tt>. This range may vary as the deformable modeling package adjusts the range
* to maximize the numerical accuracy of computing high order derivatives.
* <br><br>
* The refinement argument specifies the number of knots in the child patch
* compared to the parent patch. For example, a refinement value of <tt>2</tt> will double
* the density of knots in the child compared to the parent. refinement must be an
* integer value greater than <tt>0</tt>.
* <br><br>
* The <tt>patch_entity</tt> pointer is stored with the patch and can be accessed by an
* application with the method <tt>DS_dmod->Entity()</tt> and may be changed with the method
* <tt>DS_dmod->Set_entity()</tt>. The seam_data array is a set of pointers, each stored
* with its corresponding seam that can be accessed by an application by using,
* <tt>DS_dmod->Seam(ii)->Src_data()</tt> and may be set by the call
* <tt>DS_dmod->Seam(ii)->Set_src_data()</tt>.
* <br><br>
* When using hierarchical surface patches you will find that if the degree of the
* surface is less than that used for the patch, deformations will look unnatural.
* This is especially true if <tt>C1</tt> seam constraints are being used. This is because
* the patch doesn't have enough flexibility to conform to the seam constraints and
* deform as well. Increasing the surface degree adds much more flexibility to the
* surfaces. Certainly, degree <tt>4</tt> and <tt>5</tt> surfaces work better than degree <tt>3</tt> surfaces.
* <br><br>
* Returns the tag of the newly created patch or an error.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd>
*<dt><tt>DM_scale_unit_dpt_to_pfunc()</tt></dt>
*<dd>Errors.</dd>
*<dt><tt>DM_UNCONTAINED_CHILD</tt></dt>
*<dd>The input patch boundary must be in the range of 0 to 1.</dd>
*<dt><tt>DM_BAD_REFINEMENT_VALUE</tt></dt>
*<dd>The input refinement must be larger than 0.</dd>
*<dt><tt>DM_PATCH_OCCLUDES_ROOT</tt></dt>
*<dd>The input patch completely covers the deformable model.</dd>
*<dt><tt>DM_BAD_DOMAIN_PT_RANGE</tt></dt>
*<dd>The seams must be completely contained by the new child patch.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: dmod to be the parent.
* @param domain_flag
* in: 0=domain_pts and seams in orig_dmod_space, 1=domain_pts and seams in unit_space, 2=domain_pts and seams in internal_pfunc_space.
* @param pt1
* in: min-corner of domain box bounding all seams.
* @param pt2
* in: max-corner of all seams.
* @param refinement
* in: parent knot spacing/child knot spacing.
* @param patch_entity
* in: app entity ptr stored with patch.
* @param seam_count
* in: number of domain curves in seam (may be 0).
* @param seam
* in: seam curves sized:[seam_count].
* @param seam_data
* in: app data stored with each seam sized:[seam_count].
* @param sdmo
* in:SDM_options pointer.
**/

DECL_DM int                // rtn: new patch tag id or neg err code
DM_add_surface_patch       // eff: make and add a patch to patch hierarchy
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be the parent, [pre-alloced]
   int domain_flag,        // in : 0=domain pts and seams in orig_dmod_space,
                           //      1=domain pts and seams in unit_space,
                           //      2=domain pts and seams in internal_pfunc_space.
   double pt1[2],          // in : min-corner of domain SPAbox bounding all seams
   double pt2[2],          // in : max-corner of domain SPAbox bounding all seams
   int refinement,         // in : parent_knot_spacing/child_knot_spacing
   void *patch_entity,     // in : app entity ptr stored with patch, [pass-thru]
   int seam_count,         // in : number of domain curves in seam(may be 0)
   DS_pfunc **seam,        // in : seam curves, sized:[seam_count], [pre-alloced]
                           //      The seams are [nested], the seam array is not.
   void **seam_data,       // in : app data stored with each seam, sized:[seam_count]
                           //      [pre-alloced], the seam_data is [nested], the array is not.
   SDM_options *sdmo=NULL);// in : SDM_options pointer


//=========================================================
// DM_rm_patch()
//=========================================================

/**
* Removes and deletes a patch from a target deformable modeling hierarchy.
* <br><br>
* <b>Role:</b> Removes the deformable model identified by the <tt>tag_flag</tt> and all
* its children from the deformable modeling patch hierarchy. The deformable model
* identified by <tt>tag_flag</tt> must be a patch in the deformable modeling hierarchy. It
* can not be a root level deformable model.
* <br><br>
* After executing, this function sets the deformable modeling hierarchy's active
* patch to be the parent of the removed patch.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of target dmod hierarchy.
* @param tag_flag
* in: specify the tgt_dmod to remove 1 = active dmod 2 = member dmod with tag=tag_flag.
* @param sdmo
* in:SDM_options pointer note: sets active dmod note: also always removes offspring.
**/
DECL_DM void
DM_rm_patch                // eff: remove and delete patch
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of target dmod hierarchy, [pre-alloced]
   int tag_flag,           // in : specify the tgt_dmod to remove
                           //      1 = active dmod
                           //      2 = member dmod with tag==tag_flag
   SDM_options *sdmo=NULL);// in : SDM_options pointer

                           // note: also always removes offspring
                           // note: sets active dmod

//=========================================================
// DM_get_active_patch, DM_get_active_patch_tag, DM_set_active_patch
//=========================================================
/**
* Gets the pointer to the hierarchy's active patch or <tt>NULL</tt>.
* <br><br>
* <b>Role:</b> Returns the value of the deformable model's active deformable
* model pointer, or <tt>NULL</tt> when the input deformable model is <tt>NULL</tt> on entry.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod in hierarchy to search.
* @param sdmo
* in:SDM_options pointer.
**/

DECL_DM DS_dmod *          // rtn: ptr to the hierarchy's, [pre-alloced]
DM_get_active_patch        //      active patch or NULL
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : a dmod to search, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the tag number of the hierarchy's active patch.
* <br><br>
* <b>Role:</b> Returns the tag value of the deformable model's active
* deformable model.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd>
*<dt><tt>DM_NO_ACTIVE_DMOD</tt></dt>
*<dd>The input deformable model's hierarchy has no currently active deformable model.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod in hierarchy to search.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM int                // rtn: tag number of the hierarchy's
DM_get_active_patch_tag    //      active patch or neg err code
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : a dmod to search, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Sets the input deformable model to the hierarchy's active deformable model.
* <br><br>
* <b>Role:</b> Sets the deformable model's root deformable model dmo_active
* pointer to be this deformable model's value. Making a deformable model active
* allows many of the input and output commands to work on this deformable model by
* default.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd></dl>
* <br>
* <b>Effect:</b> System routine
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to make active.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void               // eff: make the input dmod the
DM_set_active_patch        //      hierarchy's active dmod
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : the dmod to make active, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

//====================================================
// DM_get_patch_continuity(), DM_set_patch_continuity()
//====================================================

/**
* Gets the patch-to-parent continuity and returns <tt>0</tt> for <tt>C0</tt> (position continuity) or <tt>1</tt> for <tt>C1</tt> (pos and tang continuity).
* <br><br>
* <b>Role:</b> Returns the deformable model's current patch-to-parent
* continuity. Patches may be connected to their parents with either <tt>C0</tt> (position)
* or <tt>C1</tt> (tangent) continuity.
* <br><br>
* Returns <tt>0 = C0</tt> positional continuity
* <br><br>
* Returns <tt>1 = C1</tt>positional and tangent continuity.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: the dmod to query.
* @param sdmo
* in:SDM_options pointer  note: may return negative err code.
**/
DECL_DM int                // rtn: 0=C0 (SPAposition continuity)
                           //      1=C1 (pos and tang continuity)
                           //      2=C2 (pos and tan and curvature)
DM_get_patch_continuity    // eff: rtn patch-to-parent continuity
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : the dmod to query, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: may return negative err code

/**
* Sets patch-to-parent continuity.
* <br><br>
* <b>Role:</b> Modifies deformable <tt>model->dmo_seam_state</tt>.
* <br><br>
* A convenience routine that sets the behaviors of all the seam or link
* constraints in one deformable model to a common value. This command is
* equivalent to running the command <tt>DM_set_cstrn_behavior()</tt> once on all the seams
* or links within the target deformable model. There is no requirement that all
* the seams and links of one deformable model share a common continuity behavior.
* <br><br>
* For child patches, the continuity behavior of all the patch's seams are modified,
* for root sibling patches, the continuity behavior of all the patch's link
* constraints are modified.
* <br><br>
* A patch may be connected to another patch with either <tt>C0</tt> (position), <tt>C1</tt>
* (position and tangent) or <tt>C2</tt> (position, tangent, and curvature) continuity. The
* value of the input argument, continuity, specifies the desired continuity as
* follows:
* <br><br>
* <tt>continuity = 0</tt> for position.
* <br><br>
* <tt>continuity = 1</tt> for position and tangent.
* <br><br>
* <tt>continuity = 2</tt> for position, tangent, and curvature.
* <br><br>
* <tt>continuity = -1</tt> to increment continuity from <tt>C0</tt> to <tt>C1</tt> to <tt>C2</tt> and back to <tt>C0</tt> again.
* <br><br>
* <b>Errors:</b>
*<dl><dt><tt>DM_BAD_CONTINUITY_VALUE</tt></dt>
* <dd>when the continuity input value is not <tt>0</tt> for <tt>C0</tt> continuity, <tt>1</tt> for <tt>C1</tt> continuity,
*<tt>2</tt> for <tt>C2</tt> continuity, or <tt>-1</tt> for toggling.</dd>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd></dl>
* <br>
* <b>Limitations:</b> Currently curvature constraints are only available for curve
* patches <tt>2</tt> for <tt>C2</tt> continuity, or <tt>-1</tt> for toggling.
*<br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: target dmod.
* @param continuity
* in:-1=Toggle between C0,C1 and C2 0=C0 (position continuity) 1=C1 (pos and tang continuity) 2=C2 (pos,tang,curv continuity).
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_set_patch_continuity    // eff: set patch-to-patch continuity
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : the dmod to modify, [pre-alloced]
   int continuity,         // in : -1=Toggle between C0, C1, C2
                           //       0=C0 (SPAposition continuity)
                           //       1=C1 (pos and tang continuity)
                           //       2=C2 (pos, tang, and curv cont)
   SDM_options *sdmo=NULL);// in : SDM_options pointer


//============================================================
// DM_get_parent(), DM_get_child(), DM_get_sibling(),
// DM_get_patch_seam_count(), DM_get_patch_seam_tag()
//============================================================

/**
* Gets the deformable model's parent and returns the parent pointer or <tt>NULL</tt>.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to be queried.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM DS_dmod *          // rtn: parent ptr or NULL, [pre-alloced]
DM_get_parent              // eff: return dmod's parent
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be queried, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets a deformable model's child and returns the child pointer or <tt>NULL</tt>.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* dmod to be queried.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM DS_dmod *          // rtn: parent's child or NULL, [nested]
DM_get_child               // eff: return dmod's child
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be queried, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the deformable model's sibling and returns the sibling pointer or <tt>NULL</tt>.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* dmod to be queried.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM DS_dmod *          // rtn: dmod's sibling or NULL, [nested]
DM_get_sibling             // eff: return dmod's sibling
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be queried, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the number of seam curve.
* <br><br>
* <b>Role:</b> Returns the number of curve constraints that are connecting this
* deformable model patch to its parent.
* <br><br>
* Returns <tt>0</tt> or positive number for success
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to be queried.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM int                // rtn: seam_count or negative err code
DM_get_patch_seam_count    // eff: count the number of seam curves
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be queried, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the tag number for a seam curve and returns the seam curve tag or an error.
* <br><br>
* <b>Role:</b> Returns the number of curve constraints that are connecting this
* deformable model patch to its parent.
* <br><br>
* Returns <tt>0</tt> or positive number for success
* <br><br>
* <b>Errors:</b><dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_BAD_SEAM_NUMBER_VALUE</tt></dt>
* <dd>The seam number cannot be less than 0, nor equal to or larger than the
*deformable model's seam count.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to be queried.
* @param seam_number
* in: seam number to be queried.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM int                // rtn: seam curve tag or neg err code
DM_get_patch_seam_tag      // eff: return tag number for a seam crv
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be queried, [pre-alloced]
   int seam_number,        // in : seam number to be queried
   SDM_options *sdmo=NULL);// in : SDM_options pointer

//============================================================
// DM_find_patch_...  _by_tag(), _by_tag_flag(), _by_entity()
//============================================================
/**
* Finds and returns the pointer to the <tt>DS_dmod</tt> deformable model whose tag matches the input tag value or returns <tt>NULL</tt>.
* <br><br>
* <b>Role:</b> Searches the deformable model's hierarchy for the deformable
* model whose tag value is the same as the input tag value. When found, the
* pointer to that deformable model is returned and that deformable model is made
* the active deformable model. When not found <tt>NULL</tt> is returned. The entire
* hierarchy containing the input dmod is automatically searched.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>When dmod is NULL on input.</dd>
*<dt><tt>DM_TAG_NOT_A_DMOD_MEMBER</tt></dt>
*<dd>The <tt>tag_flag</tt> does not identify a deformable model in the patch hierarchy.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of hierarchy to search.
* @param tag
* in: the tag value to find.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM DS_dmod *          // rtn: tag's dmod ptr or NULL, [pre-alloced]
DM_find_patch_by_tag       // eff: turn tag into dmod ptr
  (int &rtn_err,           // out: 0=success or negative err code
   DS_dmod *dmod,          // in : member of search, [pre-alloced]
   int tag,                // in : the tag value to find.
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: makes found patch active
/**
* Converts a <tt>tag_flag</tt> identifier into a deformable model pointer and a <tt>walk_flag</tt> value.
* <br><br>
* <b>Role:</b> Modifies <tt>walk_flag</tt>, returns pointer to identified deformable
* model.
* <br><br>
* <tt>tag_flag</tt> identifies the target deformable model as follows. When <tt>tag_flag</tt> is
* equal to <tt>+1</tt> or <tt>-1</tt>, the target is the active deformable model. When <tt>tag_flag</tt> is
* equal to <tt>+2</tt> or <tt>-2</tt>, the target is the root deformable model. When <tt>tag_flag</tt> is
* equal to some other value, then the target is the deformable model that has the
* same tag identifier as the absolute value of the <tt>tag_flag</tt> value. When there is
* no deformable model whose tag identifier matches the absolute value of the
* <tt>tag_flag</tt> value, then there is no target and <tt>NULL</tt> is returned.
* <br><br>
* In summary:
*
* +1 or -1 		active deformable model
* +2 or -2 		root deformable model
* + or -num 	deformable model with tag_id == |num|</pre>
* The sign of the <tt>tag_flag</tt> value specifies the value of the <tt>walk_flag</tt>. <tt>walk_flag</tt>
* is a typical input required by all functions which are capable of walking the
* deformable modeling hierarchy recursively.<br><br>
* Valid walk_flag values include:
* <br><br>
* <tt>0</tt> = operate only on the target deformable model.
* <br><br>
* <tt>1</tt> = operate only on the target deformable model and its offspring.
* <br><br>
* <tt>2</tt> = operate on the target deformable model, its younger siblings, and all
* their offspring.
*
* <br><br>
* For multisurface models: When input <tt>tag_flag</tt> value is <tt>-2</tt> (root dmod and
* offspring), then the <tt>walk_flag</tt> value is set to <tt>2</tt> (target, siblings, and
* offspring), and the returned deformable model pointer is set to the oldest root
* sibling. When input <tt>tag_flag</tt> value is <tt>-1</tt>, the returned deformable model pointer
* is set to the currently active deformable model and <tt>walk_flag</tt> is set to <tt>2</tt> if the
* active Deformable model is a root sibling, else walk_flag is set to <tt>1</tt>. When the
* input <tt>tag_flag</tt> value is any negative number other than <tt>-1</tt> or <tt>-2</tt>, <tt>walk_flag</tt> is
* set to <tt>1</tt>. When the input <tt>tag_flag</tt> value is positive, <tt>walk_flag</tt> is set to <tt>0</tt>.
* <br><br>
* <b>Errors:</b> Returns <tt>NULL</tt> for any errors, and sets <tt>rtn_err</tt> to one of the following
* possible error values:
*<dl>
*<dt><tt>DM_DMOD_NOT_A_ROOT_DMOD</tt></dt>
*<dd>The deformable model must be the root of a hierarchy tree.</dd>
*<dt><tt>DM_BAD_TAG_FLAG_VALUE</tt></dt>
*<dd>The <tt>tag_flag</tt> cannot equal <tt>0</tt>.</dd>
*<dt><tt>DM_NO_ACTIVE_DMOD</tt></dt>
*<dd>The input deformable model's hierarchy has no currently active deformable model.</dd>
*<dt><tt>DM_TAG_NOT_A_DMOD_MEMBER</tt></dt>
*<dd>The <tt>tag_flag</tt> does not identify a deformable model in the patch hierarchy.</dd>></dl>
* <br>
* <b>Effect:</b> System routine
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of hierarchy to search.
* @param tag_flag
* in: specify target dmod and how deep to go. 1=active, 2=root, else = member (tag=tag_flag) pos=tgt only, neg=tgt and offspring.
* @param walk_flag
* out: 0=tgt only 1=tgt and offspring 2=tgt, siblings, and offspring.
* @param sdmo
* in:SDM_options pointer note: makes found patch active.
**/

DECL_DM DS_dmod *          // rtn: ptr to target dmod, [pre-alloced]
DM_find_patch_by_tag_flag  // eff: turn tag_flag into dmod
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of target dmod hierarchy, [pre-alloced]
   int tag_flag,           // in : specify tgt dmod 1=active, 2=root
                           //      else=member(tag==tag_flag)
   int &walk_flag,         // out: 0=tgt_only, 1=tgt and offspring
                           //      2=dmod,siblings,and offspring
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: makes found patch active
/**
* Finds and returns pointer to <tt>DS_dmod</tt> deformable model whose entity pointer matches the input entity pointer or returns <tt>NULL</tt>.
* <br><br>
* <b>Role:</b> Searches the deformable model's hierarchy for the deformable
* model whose entity pointer value is the same as the input entity pointer value.
* When found, the pointer to that deformable model is returned. When not found
* <tt>NULL</tt> is returned. The entire deformable modeling hierarchy containing the input
* dmod is automatically searched.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>when dmod is NULL on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>No <tt>DS_dmod</tt> deformable model with a matching entity pointer was found.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of hierarchy to search.
* @param entity
* in: tgt_dmod entity value to match.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM DS_dmod *          // rtn: tag's dmod ptr or NULL, [pre-alloced]
DM_find_patch_by_entity    // eff: turn entity ptr into dmod ptr
  (int &rtn_err,           // out: 0=success or negative err code
   DS_dmod *dmod,          // in : member of target dmod hierarchy, [pre-alloced]
   void *entity,           // in : tgt_dmod entity value to match
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: makes found patch active
/**
* Builds a curve set to act as a child patch's seam given the parent's deformable model.
* <br><br>
* <b>Role:</b> Modifies <tt>pfunc_count</tt>, <tt>pfunc</tt>, the active deformable model.
* <br><br>
* Builds and returns a set of curves suitable for use as a patch seam boundary in
* the target deformable model. Checks that pfunc is <tt>NULL</tt> on entry to the
* subroutine and that the minimum and maximum points lie within the target
* deformable model's domain space. When the check fails, <tt>pfunc_count</tt> is set to <tt>0</tt>
* and pfunc is left unaltered. When the check passes, this function builds and
* stores in pfunc an array of four curves that form a square in the domain space
* of the target deformable model. It also sets the <tt>pfunc_count</tt> value to <tt>4</tt>.
* <br><br>
* The newly constructed seams are built in an image-space that is equal to the
* <tt>orig_dmod_space</tt> of the input dmod. The min and max domain point locations are
* scaled into this <tt>orig_dmod_space</tt>. These seams and min and max domain point
* values can be used directly as input to the function <tt>DM_add_surface_patch</tt> as
* long as the <tt>domain_flag</tt> value for the <tt>DM_add_surface_ptach()</tt> call is set to <tt>0</tt>.
* <br><br>
* Verifies that minimum and maximum represent legitimate points contained within
* the domain space of the object.
* <br><br>
* When <tt>domain_flag</tt> is set to <tt>1</tt>, dpt is given in the unit range and mapped to the
* <tt>pfunc</tt>'s actual domain range by this function. Otherwise, dpt is given in the
* <tt>pfunc</tt>'s actual domain range.
* <br><br>
* <tt>mallocs</tt> the pfunc array and the memory for each member in the array. The calling
* program is responsible for freeing this memory.
* <br><br>
* <b>Errors:</b><dl>
*<tt>DM_parse_tag_flag()</tt> errors
*<dt><tt>DM_BAD_DOMAIN_PT_RANGE</tt></dt>
*<dd>The minimum and maximum must be within the range of 0 to 1.</dd>
*<dt><tt>DM_NON_NULL_OUTPUT_PTR</tt></dt>
*<dd>pfunc must be NULL on entry.</dd>
*<dt><tt>DM_ZERO_AREA_PATCH</tt></dt>
*<dd>The requested child patch must not have a zero area.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: dmod for new seam (the parent).
* @param domain_flag
*  in: 0=min and max in orig_dmod_space, 1=min and max in unit_space, 2=min and max in internal_pfunc_space.
* @param min
* i/o: lower domain point output in orig_dmod_space.
* @param max
* i/o: upper domain point output in orig_dmod_space.
* @param pfunc_count
* out: number of pfuncs in pfunc array.
* @param pfunc
* out: newly created pfunc array (NULL on input) output in orig_dmod_space. mallocs: pfunc array and each pfunc entry.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_build_square_seam       // eff: build curve set to act as a seam
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod for new seam (the parent), [pre-alloced]
   int domain_flag,        // in : 0=min and max in orig_dmod_space,
                           //      1=min and max in unit_space,
                           //      2=min and max in internal_pfunc_space.
   double min[2],          // i/o: lower domain point for patch,
                           //      output in orig_dmod_space
   double max[2],          // i/o: upper domain point for patch,
                           //      output in orig_dmod_space
   int &pfunc_count,       // out: number of pfuncs in pfunc_array
   DS_pfunc **&pfunc,      // out: newly created pfunc array, [NULL-entry]
                           //      sized:[pfunc_count] on output
                           //      output in orig_dmod_space
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // mallocs: pfunc array and each pfunc entry
/**
* Builds a curve set to act as a seam.
* <br><br>
* <b>Role:</b> Modifies <tt>pfunc_count</tt>, <tt>pfunc</tt>, <tt>active_dmod</tt>.
* <br><br>
* This function builds and returns a set of curves suitable for use as a patch
* seam boundary in the tgt_dmod. Checks that pfunc is <tt>NULL</tt> on entry to the
* subroutine and that the min and max points lie within the tgt_dmod's domain
* space. When the check fails, <tt>pfunc_count</tt> is set to <tt>0</tt> and <tt>pfunc</tt> is left unaltered.
* When the check passes, builds and stores in pfunc an array of 4 curves that
* form a square in the domain space of the <tt>tgt_dmod</tt>. Sets the <tt>pfunc_count</tt> value to
* <tt>4</tt>.
* <br><br>
* The newly constructed seams are built in an image-space that is equal to the
* <tt>orig_dmod_space</tt> of the input dmod. The min and max domain point locations are
* scaled into this <tt>orig_dmod_space</tt>. These seams and min and max domain point
* values can be used directly as input to the function <tt>DM_add_surface_patch</tt> as
* long as the <tt>domain_flag</tt> value for the <tt>DM_add_surface_patch()</tt> call is set to <tt>0</tt>.
* <br><br>
* Verifies that minimum and maximum represent legitimate points contained within
* the domain space of the object.
* <br><br>
* When <tt>domain_flag</tt> is set to <tt>1</tt>, min, max, and radius are given in the unit range
* and mapped to the dmod's orig_dmod_space domain range by this function. When
* <tt>domain_flag</tt> is set to <tt>0</tt>, these inputs are given in <tt>orig_dmod_space</tt>. When
* <tt>domain_flag</tt> is set to <tt>1</tt>, these inputs are given in the <tt>internal_pfunc_space</tt>. It
* <tt>mallocs</tt> the pfunc array and the memory for each member in the array. The calling
* program is responsible for freeing this memory.
* <br><br>
* <b>Errors:</b>
* Sets <tt>rtn_err</tt> to 0 for success, else
*<br><br>
*<tt>DM_parse_tag_flag()</tt> errors
*<dt><tt>DM_BAD_DOMAIN_PT_RANGE</tt></dt>
*<dd>when min or max are not in the range of 0 to 1 or within the tgt_dmod.</dd>
*<dt><tt>DM_NON_NULL_OUTPUT_PTR</tt></dt>
*<dd>when pfunc is not NULL on entry.</dd>
*<dt><tt>DM_FILLET_RADIUS_TOO_BIG</tt></dt>
*<dd>when fillet radius is larger than Min(max[0]-min[0],max[1]-min[1])/2.0</dd>
*<dt><tt>DM_FILLET_RADIUS_NOT_POS</tt></dt>
*<dd>when fillet radius is negative or zero.</dd>
*<dt><tt>DM_ZERO_AREA_PATCH</tt></dt>
*<dd>The requested child patch must not have a zero area.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* n: dmod for new seam (the parent).
* @param domain_flag
* in : 0=min, max, radius in orig_dmod_space, 1=min, max, radius in unit_space, 2=min, max, radius in internal_pfunc_space.
* @param min
* i/o: patch lower domain point, output in orig_dmod_space.
* @param max
* i/o: patch upper domain point, output in orig_dmod_space.
* @param radius
* patch corner fillet radius.
* @param pfunc_count
* number of pfuncs in pfunc array.
* @param pfunc
* newly created pfunc array (NULL on input) output in orig_dmod_space mallocs: pfunc array and each pfunc entry.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_build_fillet_square_seam// eff: build curve set to act as a seam
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod for new seam (the parent), [pre-alloced]
   int domain_flag,        // in : 0=min, max, radius in orig_dmod_space,
                           //      1=min, max, radius in unit_space,
                           //      2=min, max, radius in internal_pfunc_space.
   double min[2],          // i/o: lower domain point for patch,
                           //      output in orig_dmod_space
   double max[2],          // i/o: upper domain point for patch,
                           //      output in orig_dmod_space
   double radius,          // in : fillet radius in the square corners
   int &pfunc_count,       // out: number of pfuncs in pfunc_array
   DS_pfunc **&pfunc,      // out: newly created pfunc array, [NULL-entry]
                           //      sized:[pfunc_count] on output,
                           //      output in orig_dmod_space
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // mallocs: pfunc array and each pfunc entry
/**
* Builds a curve set to act as a seam.
* <br><br>
* <b>Role:</b> Builds and returns an elliptical curve suitable for use as a
* patch seam boundary in the target deformable model. Checks that pfunc is <tt>NULL</tt> on
* entry to the subroutine. Builds a <tt>DS_circ</tt> object using the cpt, axis1, and axis2
* positions scaled to the target deformable model's domain space and then computes
* and stores the minimum sized bounding box for the ellipse in <tt>0</tt> to <tt>1</tt> space. This
* function checks that the minimum and maximum points lie within the target
* deformable model's object's domain space. When the check fails, pfunc_count is
* set to <tt>0</tt> and <tt>pfunc</tt> is left unaltered. When the check passes, this function
* stores in <tt>pfunc</tt> the <tt>DS_circular</tt> curve that forms the ellipse and sets the
* pfunc_count value to <tt>1</tt>.
* <br><br>
* The newly constructed seams are built in an image-space that is equal to the
* <tt>orig_dmod_space</tt> of the input dmod. The min and max domain point locations are
* scaled into this <tt>orig_dmod_space</tt>. These seams and min and max domain point
* values can be used directly as input to the function <tt>DM_add_surface_patch</tt> as
* long as the <tt>domain_flag</tt> value for the <tt>DM_add_surface_patch()</tt> call is set to <tt>0</tt>.
* <br><br>
* Verifies that minimum and maximum represent legitimate points contained within
* the domain space of the object.
* <br><br>
* When <tt>domain_flag</tt> is set to <tt>1</tt>, cpt, axis1, and axis2 are given in the unit range
* and mapped to the dmod's <tt>orig_dmod_space</tt> domain range by this function. When
* <tt>domain_flag</tt> is set to <tt>0</tt>, these inputs are given in <tt>orig_dmod_space</tt>. When
* <tt>domain_flag</tt> is set to <tt>2</tt>, these inputs are given in <tt>internal_pfunc_space</tt>.
* <br><br>
* <tt>mallocs</tt> the pfunc array and the memory for <tt>DS_circ</tt> member of the array. The
* calling program is responsible for freeing this memory.
* <br><br>
* <b>Errors:</b>
*<tt>DM_parse_tag_flag()</tt> errors
*<dt><tt>DM_BAD_DOMAIN_PT_RANGE</tt></dt>
*<dd>The minimum and maximum must be within the range of 0 to 1.</dd>
*<dt><tt>DM_NON_NULL_OUTPUT_PTR</tt></dt>
*<dd>pfunc must be NULL on entry.</dd>
*<dt><tt>DM_ZERO_AREA_PATCH</tt></dt>
*<dd>The requested child patch must not have a zero area.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: dmod for new seam (the parent).
* @param domain_flag
* in: 0=domain_pts in orig_dmod_space, 1=domain_pts in unit_space, 2=domain_pts in internal_pfunc_space.
* @param cpt
* in: center point for patch.
* @param axis1
* in: vector from cpt to 1st axis end point.
* @param axis2
* in: vector from cpt to 2nd axis end point.
* @param min
* out: lower domain ellipse bound, output in orig_dmod_space.
* @param max
* out: upper domain ellipse bound, output in orig_dmod_space.
* @param pfunc_count
* out: always set to 1.
* @param pfunc
* out: newly created pfunc array (NULL on input), output in orig_dmod_space, mallocs: pfunc array and each pfunc entry.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_build_ellipse_seam      // eff: build curve set to act as a seam
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod for new seam (the parent), [pre-alloced]
   int domain_flag,        // in : 0=domain_pts in orig_dmod_space,
                           //      1=domain_pts in unit_space,
                           //      2=domain_pts in internal_pfunc_space.
   double cpt[2],          // in : center point for patch
   double axis1[2],        // in : SPAvector from cpt to 1st-axis end point
   double axis2[2],        // in : SPAvector from cpt to 2nd-axis end point
   double min[2],          // out: lower domain ellipse bound,
                           //      output in orig_dmod_space
   double max[2],          // out: upper domain ellipse bound,
                           //      output in orig_dmod_space
   int &pfunc_count,       // out: always set to 1
   DS_pfunc **&pfunc,      // out:newly created pfunc array, [NULL-entry]
                           //     sized:[pfunc_count] on output,
                           //      output in orig_dmod_space
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // mallocs: pfunc array and each pfunc entry
/**
* Builds and returns a <tt>DS_zone</tt> object suitable for use as an area constraint.
* <br><br>
* <b>Role:</b> Builds and returns a <tt>DS_zone</tt> object suitable for use as input to
* the <tt>DM_add_area_cstrn()</tt> call. A <tt>DS_zone</tt> object specifies a sub-region within the
* deformable model's <tt>DS_pfunc</tt> object.
* <br><br>
* The input arguments min and max specify diagonally opposite corners of a
* rectangular subdomain of the <tt>DS_pfunc</tt>. This subdomain specifies the <tt>DS_zone</tt>
* sub-region, via the underlying <tt>DS_pfunc</tt> surface.
* <br><br>
* The inputs min and max are domain points which can be given in one of three
* different coordinate systems. The input domain flag value specifies which
* coordinate system is being used for these points.
* <br><br>
* When <tt>domain_flag = 0</tt>, the domain points are given in the original_dmod_space,
* (the domain range of the <tt>DS_pfunc</tt> object at the time it was used to build the
* <tt>DS_dmod</tt> object and before it may have been scaled internally.)
* <br><br>
* When <tt>domain_flag = 1</tt>, the domain points are given in the unit space, a square
* region that ranges from 0.0 to 1.0.
* <br><br>
* When <tt>domain_flag = 2</tt>, the domain points are given in the <tt>internal_pfunc_space</tt>,
* (the domain range currently stored in the deformable model's <tt>DS_pfunc</tt> object.
* This range often varies from the <tt>original_dmod_space</tt> due to internal scalings of
* the <tt>DS_pfunc</tt>'s domain.)
* <br><br>
* The internal representation of shape stored within the returned zone is saved in
* the <tt>internal_pfunc_space</tt> coordinate system.
* <br><br>
* <tt>mallocs</tt> the returned zone object is malloced and must be freed by the
* application at the appropriate time. However, once the zone is used to make an
* area constraint or some other kind of tag object, the DM library will free this
* memory when asked to free the tag object.
* <br><br>
* <b>Errors:</b>
*<tt>DM_parse_tag_flag()</tt> errors
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>When dmod is not NULL on input.</dd>
*<dt><tt>DM_BAD_DOMAIN_FLAG_VALUE</tt></dt>
*<dd>When the <tt>domain_flag</tt> value is not one of, 0=orig_dmod_space, 1=unit-space, 2=internal_pfunc_space.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: target dmod for new zone.
* @param domain_flag
* in: specify coordinate system for min and max pts. 0=orig_dmod_space, 1=unit_space, 2= internal_pfunc_space..
* @param min
* in : square's lower-left pt.
* @param max
* in : square's upper-right pt mallocs: returned zone object.
* @param sdmo
* in:SDM_options pointer.
**/

DECL_DM DS_zone *          // rtn: newly created zone
DM_build_square_zone       // eff: build partition of DS_pfunc shape
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : target dmod for new zone
   int domain_flag,        // in : 0=min and max in orig_dmod_space,
                           //      1=min and max in unit_space,
                           //      2=min and max in internal_pfunc_space.
   double min[2],          // in : lower domain point for zone,
                           //      output in orig_dmod_space
   double max[2],          // in : upper domain point for zone,
                           //      output in orig_dmod_space
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // mallocs: returned zone object

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM DS_zone *          // rtn: newly created zone
DM_build_poly_zone         // eff: build partition of DS_pfunc shape
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : target dmod for new zone
   int npts,               // in : number of points
    const double* us,      // in : the u params sized:[npts]
    const double* vs,      // in : the v params sized:[npts]
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // mallocs: returned zone object
                           // mallocs: returned zone object
/**
* Clears the zone data structure.
* <br><br>
* <b>Role:</b> Frees and clears all data values associated with the input
* <tt>DS_zone</tt> object and sets the zone pointer to <tt>NULL</tt>.
* <br><br>
* The <tt>DS_zone</tt> pointer is invalid after this call.
* <br><br>
* Do not call this function on a zone that has been used to create a tag object,
* e.g., an area constraint. That zone will be freed when the area constraint is
* freed.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The zone cannot be NULL on entry.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param zone
* i/o: ptr to valid zone object set to NULL on exit.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_delete_zone             // eff: free the zone data structure
  (int &rtn_err,           // out: 0=success or neg err code
   DS_zone *&zone,         // i/o: ptr to valid zone object set, [freed]
                           //      to NULL on exit.
   SDM_options *sdmo=NULL);// in : SDM_options pointer

//=========================================================
// Graphics Management
//=========================================================
// rtn: integer bit array for draw state

/**
* Gets the deformable model's draw state and returns an integer bit array.
* <br><br>
* <b>Role:</b> Returns the deformable model's draw state integer which can be
* used as a bit array to control the rendering of deformable model data. A
* suggested set of draw bits is given in the file dmapi.hxx as the <tt>DM_DRAW_...</tt>
* environment variable defines. These draw bits are used to cull tag objects from
* consideration by the function <tt>DM_find_tag_by_image_line()</tt> which finds and
* returns the closest tag object to an image space line.
* <br><br>
* Returns integer draw bit array for deformable model.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to be queried.
* @param sdmo
* in:SDM_options pointer.
**/

DECL_DM int                //      or negative err code
DM_get_draw_state          // eff: return the dmod's draw_state
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be queried, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Sets the target deformable model's <tt>draw_state</tt>.
* <br><br>
* <b>Role:</b> Modifies deformable <tt>model->dmo_draw_state</tt>
* <br><br>
* Sets the deformable model's <tt>draw_state</tt> value and returns <tt>0</tt> when no bits in
* <tt>draw_state</tt> are changed, and returns <tt>1</tt> when bits are altered.
* <br><br>
* A <tt>walk_flag</tt> value of <tt>0</tt> specifies that just the deformable model is modified, and
* a <tt>walk_flag</tt> value of <tt>1</tt> specifies that the deformable model and all its offspring
* are modified. A <tt>walk_flag</tt> value of <tt>2</tt> specifies that the deformable model, its
* siblings, and all their offspring are modified.
* <br><br>
* These draw bits are used to cull tag objects from consideration by the function
* <tt>DM_find_tag_by_image_line()</tt> which finds and returns the closest tag object to an
* image space line.
* <br><br>
* The set of values for draw_state may be made by the <tt>OR</tt> of the <tt>DM_DRAW_...</tt>
* variables defined in the <tt>"dmapi.hxx"</tt>.
* <br><br>
* <b>Errors:</b>
*<dl><dt><tt>DM_BAD_WALK_FLAG_VALUE</tt></dt>
*<dd>The walk_flag must be 0, 1 or 2.</dd>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: target dmod.
* @param draw_state
* desired value to store.
* @param walk_flag
* in: specify how deep to go 0=dmod only, 1=dmod and offspring 2=dmod, siblings and offspring.
* @param sdmo
* in:SDM_options pointer.
**/

DECL_DM int                // rtn: 0=no change,1=change,or neg err
DM_set_draw_state          // eff: set the dmod's draw_state
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be modified, [pre-alloced]
   int draw_state,         // in : desired value to store
   int walk_flag=0,        // in : specify how deep to go
                           //      0=dmod only,1=dmod and offspring
                           //      2=dmod,siblings,and offspring
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the parameters of the deformable model's curvature comb graphics and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Returns the deformable model's curvature comb plot parameters. A
* curvature comb plot is an excellent way to communicate a curve's curvature to an
* end user. The curvature comb is a set of vectors connected along the length of
* the curve. The length of each vector is proportional to the curvature at the
* point where the vector connects to the curve. the direction of the vector is in
* the binomial of the curve.
* <br><br>
* The parameters of the plot include <tt>comb_pt_count</tt> and <tt>comb_gain</tt>.
* <br><br>
* The <tt>comb_pt_count</tt> is the number of tines plotted within each element of the
* deformable <tt>model->Pfunc()</tt> shape. The comb_gain is a magnitude that is multiplied
* on the curvature to specify the length of the curvature vectors on the screen.
* Typically negative numbers are used so that the tines of the comb do not
* intersect one another.
* <br><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to be queried.
* @param comb_pt_count
* out: number of times-per-elem.
* @param comb_gain
* out: gain on comb vector magnitudes.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void               // eff: return the parameters of dmod's
DM_get_comb_graphics       //      curvature comb graphics
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be queried, [pre-alloced]
   int &comb_pt_count,     // out: number of tines-per-elem
   double &comb_gain,      // out: gain on comb SPAvector magnitudes
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Sets the deformable model's <tt>comb_graphics</tt> and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Sets the parameters that control the rendering of curvature
* combs, and curvature values for curvature point constraints. Curvature values
* are plotted for points on curves as a vector. The plotted vector is attached to
* the curve at the point being measured and points in the direction of the normal
* to the curve at that point. The curvature vector's magnitude is equal to the
* curvature * the input <tt>comb_gain</tt> value. So, for a given curve <tt>W=W(u)=[x(u0,y(u0,
* z(u)]t,</tt> the curvature vector plotted for some point, <tt>u0</tt>, on that curve is:
*<pre>
* plotted curvature base_pt = W(u0)
* plotted curvature vector = comb_gain * k * m
* where
* - k = curvature = size(cross_product(Wu,Wuu)) / size(Wu)**3
* - m = normal direction = cross_product(t,b)
* - Wu = dW(u0)/du = 1st parametric derivative taken at location u0.
* - Wuu = d2W(u0)/du2 = 2nd parametric derivative taken at location u0.
* - t = tangent = normalized(Wu)
* - b = bi-normal = normalized(cross_product(Wu,Wuu))
*</pre>
* A curvature comb report is generated by plotting several curvature vectors along
* the length of the curve. The input value <tt>comb_pt_count</tt> specifies how many evenly
* spaced curvature vectors to plot within each element of the target curve. The
* <tt>comb_pt_count</tt> value does not affect the rendering of curvature values for point
* constraints.
* <br><br>
* A unique set of curvature plotting parameters are stored for each deformable
* model within a deformable modeling hierarchy. For convenience, this command can
* apply the input curvature comb parameters to a set of deformable models with one
* call as determined by the value of <tt>walk_flag</tt>. When <tt>walk_flag</tt> is <tt>0</tt>, the
* parameters are applied only to the target input dmod. When <tt>walk_flag</tt> is <tt>1</tt>, the
* parameters are applied to the target dmod and all of its offspring, and a
* walk_flag value of <tt>2</tt> applies the parameters to the target dmod, its younger
* siblings, and all their offspring.
* <br><br>
* The <tt>comb_gain</tt> value is used when using the <tt>DM_set_pt_xyz()</tt> function to place the
* curvature vector end-point to determine the new target value for the curvature
* at that point.
* <br><br>
* <b>Errors:</b>
*<dl><dt><tt>DM_BAD_WALK_FLAG_VALUE</tt></dt>
*<dd>The <tt>walk_flag</tt> must be <tt>0</tt>, <tt>1</tt> or <tt>2</tt>.</dd>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to modify.
* @param comb_pt_count
* in: desired number of tines-per-elem.
* @param comb_gain
* in: desired gain on comb vector magnitudes.
* @param walk_flag
* in: specify how deep to go 0=dmod only, 1=dmod and offspring 2=dmod, siblings, and offspring.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM int                // rtn: 0=no change,1=change,or neg err
DM_set_comb_graphics       // eff: set the dmod's comb_graphics
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be modified, [pre-alloced]
   int comb_pt_count,      // in : desired number of tines-per-elem
   double comb_gain,       // in : desired gain on comb SPAvector magnitudes
   int walk_flag=0,        // in : specify how deep to go
                           //      0=dmod only,1=dmod and offspring
                           //      2=dmod,siblings,and offspring
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the deformable model's mesh graphics parameters and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Returns the deformable model's mesh plot parameters. A good way
* to render a deformable model is with a set of regularly sized polygons. Each
* time the end user changes the shape of the deformable model the locations of the
* mesh's vertices may be updated and rendered. If one avoids having to remesh then
* the rendering may be done quickly enough to support interactive sculpting. Since
* the end user does not know in advance what the shape of the deformable may be,
* any geometrically based meshing would be a mistake. This package stores a
* polygon count in the <i>u</i> and <i>v</i> directions that control the mesh resolution for
* plotting. This library does not use those values but makes sure they are
* available for a rendering module. The <tt>mesh_v</tt> parameter is not used for curves.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to be queried.
* @param mesh_u
* out: u_dir polygon count (crvs & srfs).
* @param mesh_v
* out: v_dir polygon count (srfs only).
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_get_mesh_count          // eff: get dmod's mesh graphics params
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be queried, [pre-alloced]
   int &mesh_u,            // out: u_dir polygon count (crvs & srfs)
   int &mesh_v,            // out: v_dir polygon count (srfs only)
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Sets the deformable model's <tt>mesh_count</tt> and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies deformable <tt>model->dmo_mesh_grid</tt>
* <br><br>
* Sets the parameters of the deformable model's <tt>mesh_grid</tt>. <tt>mesh_u</tt> is the number of
* polygons to draw in the <i>u</i> direction and <tt>mesh_v</tt> is the number of polygons to draw
* in the <i>v</i> direction. See <tt>DM_get_mesh_count()</tt> for a description of rendering
* meshes for deformable models. Returns <tt>0</tt> when the mesh parameters already equal
* the input values and no changes have to be made, otherwise returns <tt>1</tt> when
* changes to these values are made.
* <br><br>
* A <tt>walk_flag</tt> value of <tt>0</tt> specifies that just the deformable model is modified, and
* a <tt>walk_flag</tt> value of <tt>1</tt> specifies that the deformable model and all its offspring
* are modified. A <tt>walk_flag</tt> value of <tt>2</tt> specifies that the deformable model, its
* siblings, and all their offspring are modified.
* <br><br>
* The <tt>comb_graphic</tt> parameter values are not used in this library but may be passed
* to a rendering module.
* <br><br>
* <b>Errors:</b>
*<dl><dt><tt>DM_BAD_WALK_FLAG_VALUE</tt></dt>
*<dd>The <tt>walk_flag</tt> must be <tt>0</tt>, <tt>1</tt> or <tt>2</tt>.</dd>
*<dt><tt>DM_BAD_DRAW_MESH_VALUE</tt></dt>
*<dd>The values of <tt>mesh_u</tt> and <tt>mesh_v</tt> cannot be <tt>0</tt>.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to modify.
* @param mesh_u
* desired u_dir polygon count (crvs & srfs).
* @param mesh_v
* desired v_dir polygon count (srfs only).
* @param walk_flag
* in: specify how deep to go 0=dmod only, 1=dmod and offspring 2=dmod, siblings, and offspring.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM int                // rtn: 0=no change,1=change, or neg err code
DM_set_mesh_count          // eff: set the dmod's mesh_count
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be modified, [pre-alloced]
   int mesh_u,             // in : desired u_dir polygon count (crvs & srfs)
   int mesh_v,             // in : desired v_dir polygon count (srfs only)
   int walk_flag=0,        // in : specify how deep to go
                           //      0=dmod only,1=dmod and offspring
                           //      2=dmod,siblings,and offspring
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the deformable model's mesh graphics parameters.
* <br><br>
* <b>Role:</b> This function stores in <tt>handle_count</tt>, the number of patch
* graphic objects being displayed for the target deformable model and possibly its
* offspring. It stores in shape_count the number of <tt>surf_mesh</tt> objects being shown
* in the queried hierarchy. The current list of patch graphic objects includes:
* <br><br>
* <tt>DM_DRAW_CPTS</tt> look for control points
* <br><br>
* <tt>DM_DRAW_GAUSS_PTS</tt> look for vector norms
* <br><br>
* <tt>DM_DRAW_CSTRN_COMBS</tt> look for curve cstrn curv combs
* <br><br>
* <tt>DM_DRAW_SEG_BNDS</tt> look for seg_bnd points
* <br><br>
* <tt>DM_DRAW_CURVE_COMBS</tt> look for curve curvature combs
* <br><br>
* <tt>DM_DRAW_ELEMS</tt> look for elem boundaries
* <br><br>
* Within each deformable model only those graphic objects whose associated state
* bit is set in the draw state bit array are counted. Use <tt>DM_set_draw_state()</tt> to
* set the draw state bit array.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: dmod to query.
* @param handle_count
* patch graphic object count.
* @param shape_count
* surf mesh count in patch hierarchy.
* @param walk_flag
* specify how deep to go 0=dmod only, 1=dmod and offspring 2=dmod, siblings, and offspring.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_get_draw_count          // eff: get dmod's mesh graphics params
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : start of draw_count search, [pre-alloced]
   int &handle_count,      // out: patch graphic object count
   int &shape_count,       // out: surf mesh count in patch hierarchy
   int walk_flag=0,        // in : specify how deep to go
                           //      0=dmod only,1=dmod and offspring
                           //      2=dmod,siblings,and offspring
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets a display scaling for tangent vectors.
* <br><br>
* <b>Role:</b> Returns tangent <tt>display_gain</tt> used to scale the visual display of
* tangent vectors and stores the value. The visual display of the tangent vector
* is used for display, picking, and setting of tangent vector values on point
* constraints. Curvature vectors are scaled for plotting by the <tt>comb_gain</tt>
* parameter in <tt>DM_get_comb_graphics()</tt>.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to be queried.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM double             // rtn: display scaling for tang vecs
DM_get_tan_display_gain    // eff: get display scaling for tang_vecs
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be queried, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod

/**
* Sets display gain for the deformable model's tangent vectors and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies the deformable model <tt>dmo_tan_display_gain</tt> and sets the
* active deformable model.
* <br><br>
* Sets the displacement gain for the target deformable model. <tt>tan_display_gain</tt> may
* not be equal to <tt>0.0</tt>.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_BAD_WALK_FLAG_VALUE</tt></dt>
*<dd>The walk_flag must be 0, 1 or 2.</dd>
*<dt><tt>DM_BAD_DISP_GAIN_VALUE</tt></dt>
*<dd>tan_display_gain cannot be 0.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to modify.
* @param tan_display_gain
* display scaling for tang_vec constraints.
* @param walk_flag
* in: specify how deep to go 0=dmod only, 1=dmod and offspring 2=dmod, siblings, and offspring.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM void
DM_set_tan_display_gain    // eff: set display gain for dmod's tang_vecs
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be modified, [pre-alloced]
   double tan_display_gain=1.0,// in : display scaling for tang_vec cstrns
   int walk_flag=0,        // in : specify how deep to go
                           //      0=dmod only,1=dmod and offspring
                           //      2=dmod,siblings,and offspring
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod

//=========================================================
// Deformable Model Management
//=========================================================

/**
* Gets the deformable model's integral degree.
* <br><br>
* <b>Role:</b> Returns the deformable model's integral degree integer which is
* stored as part of the deformable <tt>model's->Pfunc()</tt> data. This number is used to
* set the number of gauss points used to integrate the deformable modeling
* equations of motion. The integral degree is the degree polynomial that would be
* exactly integrated by the number of gauss points used in the gauss quadrature.
* <br><br>
* Returns positive number integral degree when successful.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to be queried.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM int                // rtn: dmod's integral_degree or err code
DM_get_integral_degree     // eff: return the dmod's integral_degree
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be queried, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer


/**
* Gets the domain space dimension for a deformable model.
* <br><br>
* <b>Role:</b> Returns the deformable model's domain dimension. Valid return
* values are <tt>1</tt> for a curve parameterized in <i>u</i> and <tt>2</tt> for a surface parameterized in
* <i>u</i> and <i>v</i>.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to be queried.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM int                // rtn: dmod's domain_dim or err code
DM_get_domain_dim          // eff: rtn 1=curve and 2=surface
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be queried, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the image space dimension size and returns the deformable model's image dimension or an error.
* <br><br>
* <b>Role:</b> Returns the deformable model's image dimension which can be any
* positive integer but will generally be <tt>2</tt> for curves in two dimensional space or
* <tt>3</tt> for curves and surfaces in three dimensional space.
* <br><br>
* Returns a positive dimension number when successful.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to be queried.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM int                // rtn: dmod's image_dim or err code
DM_get_image_dim           // eff: rtn image space dimension size
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be queried, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Computes a deformable model's optimal control point positions or its current set of constraints and loads.
* <br><br>
* <b>Role:</b> Modifies the deformable model
* <br><br>
* Computes the optimal control-point positions for a deformable model given the
* deformable model's current set of loads, constraints, and deformation parameters
* using an optimization procedure. When the deformable model has been linked into
* a multi-deformable model mesh through the use of <tt>link_cstrns</tt> (see
* <tt>DM_add_link_cstrn</tt>) solve will simultaneously operate on all the deformable
* models within the deformable mesh.
* <br><br>
* After generating a solution for this deformable model or its deformable mesh,
* this function recursively generates a solve solution for all of this deformable
* model's siblings and offspring or all of this deformable model's mesh offspring.
* So a single call to this function on the root of a hierarchy tree will update
* all of the hierarchy's shapes. Alternatively, performance can be increased by
* calling this function on one of the descendents of a deformable modeling
* hierarchy. In this case solve will only be run on the passed in descendent and
* all of its younger siblings, and all of their offspring. Older siblings and
* ancestors will not be modified by such a call.
* <br><br>
* The state and parameter values of all of the deformable model's constraints,
* loads, and deformation parameters affect the computation. A change in any of
* these values will cause the shape of the deformable model to change on the next
* subsequent call to <tt>DM_solve()</tt>.
* <br><br>
* The intended use of this function is to act as part of the deformable modeling
* interaction loop. The steps in this loop are as follows;
* <br><br><ol>
* <li>Construct a shape model of a curve or surface</li>
* <li>Construct a deformable model for the shape model</li>
*<li>Add constraints and loads to the deformable model</li>
*<li>Modify the parameters of the constraints, loads, or deformation
* parameters.</li>
*<li> Call <tt>DM_solve()</tt> to change the control point positions of the deformable
* model</li>
* <li>Render the deformable model to see the change. You can loop back to
* steps <tt>3</tt> or <tt>4</tt> for interactive sculpting.</li>
* <li>When satisfied, commit the deformable model control positions back to
* the original shape model.</li></ol>
* <br>
* The <tt>iter_count</tt> can be used to run a solve on a deformable model more than one
* time before returning. It is slightly more efficient to call <tt>DM_solve</tt> once with
* an iter_count rather than to call <tt>DM_solve</tt> iter_count number of times in a row.
* The results will be the same.
* <br><br>
* For most of the deformable modeling package a steady state solution is found
* with one call to solve. This means that a second call to solve will not change
* the shape of the deformable model. Some features within the package may require
* more than one call to solve before reaching a steady state solution. These
* features include, dynamics, pressure loads, and attractor loads, all of which
* are forces whose effects on the deformable model depend on the shape of the
* deformable model.
* <br><br>
* When <tt>iter_count</tt> is positive, solve will be run on the input deformable model
* <tt>iter_count</tt> number of times before returning.
* <br><br>
* When <tt>iter_count</tt> is negative, solve will run repeatedly until a steady state
* solution is achieved or until a maximum number of iterations are computed.
* <br><br>
* Steady state is achieved when the maximum motion of any one control point is
* less than the input max_move value. A default value of max_move equal to the
* square root of <tt>DS_tolerance</tt> (equal to <tt>SPAresabs</tt> in <tt>ACIS</tt>) is used when the input
* value of <tt>max_move</tt> is set to <tt>0.0</tt> or is negative. max_move is only used when
* <tt>iter_count</tt> is equal to <tt>-1</tt>.
* <br><br>
* The maximum iteration count is set to <tt>DS_SOLVE_MAX_ITER_COUNT</tt>. If <tt>DM_solve()</tt>
* ever reaches <tt>DS_SOLVE_MAX_ITER_COUNT</tt> number of iterations, the package assumes
* that a steady state solution is not possible and restores the deformable model
* to the shape it had when entering this call and returns an error code.
* <br><br>
* Calling <tt>DM_solve</tt> with <tt>iter_count</tt> equal to <tt>-1</tt> when not using pressures,
* attractors, or dynamics will slow the system down. The package will execute two
* solves before it discovers that it achieved steady state in one iteration.
* <br><br>
* <b>Errors:</b>
*<dl><dt><tt>DM_CONFLICTING_CONSTRAINT</tt></dt>
* <dd>The solution process has stopped because some of the constraints were found to
* conflict with one another. If this happens the deformable model is returned in
*the same state as it was input to this routine.</dd>
*<dt><tt>DM_SOLVE_NEVER_CONVERGED</tt></dt>
* <dd>Solve iterated DS_SOLVE_MAX_ITER_COUNT number of times before reaching steady
*state</dd>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: root of hierarchy tree.
* @param iter_count
* in: number of time-steps to iterate or -1 to iter to convergence.
* @param max_move
* in: Used when iter=-1, converges when dof max_motion is less than max_move.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_solve                   // eff: calc dmod's control-pts new pos
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod, [pre-alloced]
   int iter_count=1,       // in : number of time-steps to iter or
                           //      -1 to iter to convergence
   double max_move=0.0,    // in : Used when iter == -1, converges
                           //      when dof max-motion
                           //      is less than max_move
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Splits the deformable model into finer elements and returns 0 for success or an error.
* <br><br>
* <b>Role:</b> This function calls the <tt>DS_dmod->Split_dmod()</tt> method to add
* element boundaries into existing elements. This is a recursive function and all
* child patches of deformable model that span the split points will be split as
* well.
* <br><br>
* No action is taken for each par-pos <i>u</i> and <i>v</i> value which is already a boundary in
* the deformable model's domain. In <i>B</i>-spline and <tt>NURB</tt> terms this means that this
* algorithm cannot be used to increase the multiplicity of an existing knot.
* <br><br>
* For <i>B</i>-spline and <tt>NURB</tt> shapes this is just the knot insertion algorithm. The
* <tt>split_pt_count</tt> tells how many split points will be added to the model. The
* <tt>split_pts</tt> array lists the locations for each split. For curves, <tt>split_pts</tt> is
* just a list of u coordinate values. For surfaces, split pts is a list of <i>uv</i>
* pairs. When the <tt>domain_flag</tt> is set to 1, each u or <i>uv</i> coordinate is given in the
* unit square and this function will map each point to the domain space of the
* deformable model. When the <tt>domain_flag</tt> is set to 0, each u and <i>uv</i> coordinate is
* given in the dmod's orig_dmod_space domain range. When the <tt>domain_flag</tt> is set to
* 1, each <i>u</i> and <i>v</i> coordinate is given in the dmod's <tt>pfunc</tt>'s <tt>internal_pfunc_space</tt>.
* <br><br>
* This function may be used to make a finer deformable model out of a coarser one.
* Experience has shown that deformable models with regularly spaced finite
* elements works the best. A good four element curve would have element boundaries
* boundaries as <tt>[0.0 0.25 0.50 .75 1.00]</tt>. When inserting boundaries into surfaces
* the split point is used to add both a <tt>u_direction</tt> and <tt>v_direction</tt> into the
* <i>B</i>-spline basis functions. This acts to add an element boundary that runs
* throughout the entire length of the model. The rule of creating regular elements
* still applies so always add split points along the diagonal axis of the unit
* square. So to turn a one element model into a <tt>3x3</tt> element model split at the
* following points <tt>split_pts</tt>= <tt>[.333 .333 .667 .667]</tt>.
* <br><br>
* <b>Errors:</b>
*<dl><dt><tt>DM_BAD_DOMAIN_PT_RANGE</tt></dt>
*<dd>The split points must be completely contained within the unit square.</dd>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to modify.
* @param domain_flag
* in: 0=split_pts in orig_dmod_space, 1=split_pts in unit_space, 2=split_pts in internal_pfunc_space.
* @param split_pt_count
* in: number of split locations.
* @param split_pts
* i/o: locs at which to split domain sized:[ split_pt_count *dmod->Pfunc()-> Domain_dim()] pts given in the <tt>unit_square</tt> ordered curves=[u0 u1 u2 ...] surfs = [u0 v0 u1 v1 ...].
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_split_dmod              // eff: split dmod into finer elements
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to modify, [pre-alloced]
   int domain_flag,        // in : 0=split_pts in orig_dmod_space,
                           //      1=split_pts in unit_space,
                           //      2=split_pts in internal_pfunc_space.
   int split_pt_count,     // in : number of split locations
   double *split_pts,      // i/o: locs at which to split domain, [pre-alloced]
                           //      sized:[split_pt_count*DM_get_domain_dim(rtn_err, dmod)]
                           //      ordered
                           //       curves=[u0 u1 u2 ...]
                           //       surfs =[u0 v0 u1 v1 ...]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Writes the deformable models into output arrays and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies <tt>u_knot_count</tt>, <tt>u_knot</tt>, <tt>v_knot_count</tt>, and <tt>v_kno</tt>.
* <br><br>
* When the <tt>dmod->Pfunc()</tt> is of type <i>B</i>-spline or <tt>NURB</tt>, this function loads the
* output with the number of knots and the knot values in the <i>u</i> and <i>v</i> directions.
* When the deformable model is a curve only the <tt>u_knot_count</tt> and the <tt>u_knot</tt> array
* are updated and <tt>v_knot_count</tt> is set to <tt>0</tt>.
* <br><br>
* This function updates the output pointer values to point to the <tt>dmod->Pfunc()</tt>'s
* nested data. Do not use these pointer values to free memory, nor after the dmod
* has been deleted.
* <br><br>
* <b>Errors:</b>
*<dl><dt><tt>DM_NOT_BSPLINE_OR_NURB</tt></dt>
*<dd>The deformable model must be a <i>B</i>-spline or a <tt>NURB</tt> in order to modify output
* variables.</dd>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to query.
* @param u_knot_count
* out: u_dir unique knot count.
* @param u_knot
* out: u_dir knot values in internal_pfunc_space.
* @param v_knot_count
* out: v_dir unique knot count (surfs only) in internal_pfunc_space.
* @param v_knot
* out: v_dir knot values (surfs only) note: mallocs the output arrays which must be deleted by the caller.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_get_dmod_knots          // eff: copy dmod knots into output arrays
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to query, [pre-alloced]
   int &u_knot_count,      // out: u_dir unique knot count
   double *&u_knot,        // out: u_dir knot values, [nested]
                           //      sized:[u_knot_count] on output
                           //      in internal_pfunc_space
   int &v_knot_count,      // out: v_dir unique knot count (surfs only)
   double *&v_knot,        // out: v_dir knot values (surfs only), [nested]
                           //      sized:[v_knot_count] on output
                           //      in internal_pfunc_space
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM void
DM_set_dmod_degree         // eff: change <i>B</i>-spline and nurb polynomial degree
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : the target dmod to modify
   int new_deg,            // in : the new degree; must be at least 3; applied to each domain dim
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM void
DM_set_dmod_degree         // eff: change <i>B</i>-spline and nurb polynomial degree
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : the target dmod to modify
   int new_deg_u,          // in : the new degree in the first domain dim; must be at least 3
   int new_deg_v,          // in : the new degree in the second domain dim; must be at least 3
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Increases the deformable model's polynomial degree by adding more degrees of freedom per element.
* <br><br>
* <b>Role:</b> Increases the basis function's polynomial degree for <i>B</i>-spline
* and <tt>NURB</tt> deformable models. This is a recursive function and all of the
* deformable model's offspring are elevated as well. This acts to add new dofs for
* the deformable model but does not increase the number of unique knots. The
* <tt>cont_flag</tt> argument determines how the element-to-element continuity behavior
* changes when the deformable model's degree is elevated.
* <br><br>
* When the <tt>cont_flag</tt> is set to <tt>1</tt>, the multiplicity counts on all the internal
* knots are left as they are and the continuity across the knots increases by one
* degree. To accommodate this one more degree of freedom (or control point) is
* added to each axis. So when a <tt>6x6</tt> control point surface has its degree elevated
* with <tt>cont_flag</tt> equal to <tt>1</tt>, it becomes a <tt>7x7</tt> control point surface. The down side
* is that the elevated deformable model shape will not be able to exactly assume
* the shape of the original surface.
* <br><br>
* When <tt>cont_flag</tt> is set to <tt>0</tt>, the multiplicity count for all internal knots is
* increased by one, keeping the original element-to-element continuity while
* increasing the degree of the polynomial within each element. To accommodate this,
* one control point is added for each element along each axis. So a <tt>6x6</tt> control
* point surface made of an array of <tt>3x3</tt> bicubic elements will become a <tt>9x9</tt> control
* point surface. The upside, bought by the addition of all these control points,
* is that the elevated surface can exactly assume the shape of the original
* surface.
* <br><br>
* All the deformable modeling algorithms are a function of the degree of freedom
* count. If you can possibly live with using <tt>cont_flag</tt> equals <tt>1</tt> you should do so.
* <br><br>
* When using hierarchical surface patches you will find that if the degree of the
* surface is less than <tt>4</tt> the patch deformations will look unnatural. This is
* especially true if <tt>C1</tt> seam constraints are being used. This is because the patch
* just doesn't have enough flexibility to conform to the seam constraints and to
* deform as well. Increasing the surface degree adds a lot of flexibility to the
* surfaces. Certainly degree <tt>4</tt> and <tt>5</tt> surfaces work better than degree <tt>3</tt>.
* <br><br>
* <b>Errors:</b>
*<dl><dt><tt>DM_BAD_CONT_FLAG_VALUE</tt></dt>
*<dd>The <tt>cont_flag</tt> value must be <tt>0</tt> or <tt>1</tt>.</dd>
* <tt>DM_NOT_BSPLINE_OR_NURB</tt>
* <dd>The deformable model must be a <i>B</i>-spline or a <tt>NURB</tt> in order to modify output
*variables.</dd>
* <br><br>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: the target dmod to modify.
* @param cont_flag
* in: 0=preserve current elem continuity 1=increment element continuity note: preserving elem continuity allows the new shape to be exactly the same as the original shape.
* @param sdmo
* in:SDM_options pointer.
**/

DECL_DM void
DM_elevate_dmod_degree     // eff: inc <i>B</i>-spline and nurb polynomial degree
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : the target dmod to modify, [pre-alloced]
   int cont_flag,          // in : 0=preserve current elem continuity
                           //      1=increment element continuity
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: preserving elem continuity allows
                           //       the new shape to be exactly the
                           //       same as the original shape.

/**
* Gets the deformable model's basis degree and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies <tt>u_degree</tt>, <tt>v_degree</tt>.
* <br><br>
* Retrieves the polynomial degrees for the deformable shape's basis functions in
* the <i>u</i> and <i>v</i> directions. The <tt>v_degree</tt> value is not used for curves.
* <br><br>
* <b>Errors:</b>
* <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd>
*<dt><tt>DM_NOT_BSPLINE_OR_NURB</tt></dt>
*<dd>The deformable model must be a <i>B</i>-spline or a <tt>NURB</tt> in order to modify output
* variables.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to query.
* @param u_degree
* out: u_dir polynomial degree.
* @param v_degree
* out: v_dir polynomial degree (surfs only).
* @param sdmo
* in:SDM_options pointer.
**/

DECL_DM void
DM_get_dmod_degree         // eff: return the dmod's basis degree
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to query, [pre-alloced]
   int &u_degree,          // out: u_dir polynomial degree
   int &v_degree,          // out: v_dir polynomial degree (surfs only)
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Extends the deformable model domain by a small amount and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies deformable model and possibly deformable
* <tt>model-&gt;Parent()</tt>.
* <br><br>
* Extends the domain of the underlying shape representation by <tt>5%</tt> in all
* directions for deformable models. If the original domain of a curve was <tt>100 &lt;= u
* &lt;= 200</tt> then the modified domain would be <tt>95 &lt;= u &lt;= 205</tt>. If the deformable model
* is a child patch and the extension in its domain would make it exceed the domain
* of its parent, then the parent's domain is also extended.
* <br><br>
* The shape of the deformable model in the extended region is defined by the edge
* element's original shape function. For <i>B</i>-splines and <tt>NURB</tt>s these shape functions
* are usually poorly behaved in regions far from the original domain. So repeated
* use of this function may build some ill-behaved looking shapes including
* self-intersections. However, use of the deformable modeling approach to reshape
* the extended shape with appropriate loads and constraints should make it look
* nice and smooth again. (that is, run <tt>DM_solve()</tt>)
* <br><br>
* A deformable model within a multi-surface mesh will not be extrapolated. Trying
* to do so will not change the database in any manner and may result in setting
* <tt>rtn_err</tt> = <tt>DM_MULTI_SURF_EXTRAPOLATE</tt>.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The deformable model cannot be NULL on entry.</dd>
* <dt><tt>DM_MULTI_SURF_EXTRAPOLATE</tt></dt>
* <dd>When <tt>DM_extrapolate_dmod</tt> is called on a deformable model that is already within
* a multi-surface mesh.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* i/o: target dmod to modify.
* @param sdmo
* in:SDM_options pointer.
**/

DECL_DM void
DM_extrapolate_dmod        // eff: extend dmod domain by small amount
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : target dmod to modify, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Generates a deformable model report and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies file.
* <br><br>
* Using <tt>fprintf</tt> statements, writes a report about the deformable model and its tag
* objects. When walk_flag is equal to <tt>0</tt> then the report covers only the input
* deformable model object. When walk_flag is equal to <tt>1</tt> then the report is written
* for deformable model and all deformable model's offspring. When <tt>walk_flag</tt> is <tt>2</tt>
* then the report is written for the deformable model, its siblings, and all their
* offspring.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd>
*<dt><tt>DM_BAD_WALK_FLAG_VALUE</tt></dt>
*<dd>when walk_flag is not 0, 1, or 2.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: target dmod to query.
* @param file
* in: stream to be written.
* @param walk_flag
* in: specify how deep to go 0=dmod only, 1=dmod & offspring 2=dmod, siblings, and offspring.
* @param sdmo
* in:SDM_options pointer.
**/

DECL_DM void
DM_print_dmod              // eff: output a dmod report
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : target dmod to query, [pre-alloced]
   FILE *file,             // in : stream to be written, [pre-alloced]
   int walk_flag=0,        // in : specify how deep to go
                           //      0=dmod only,1=dmod & offspring
                           //      2=dmod,siblings,and offspring
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the deformable model's alpha values and returns <tt>0</tt> or an error.
* <br><br>
* <b>Role:</b> Returns the deformable model's resistance to stretch alpha
* values. For curves there is only one alpha value and it is stored in <tt>alpha[0]</tt>.
* In surfaces the alpha value is a second order tensor which can be described with
* three numbers, a <tt>u_dir</tt> resistance, a <tt>v_dir</tt> resistance, and a rotation. The
* rotation is the angle between the u and v principal directions of the surface
* and the material property directions. The alpha values are stored as <tt>alpha[0] =
* au, alpha[1] = av, alpha[2] = atheta</tt>.
* <br><br>
* In practice, we have discovered that if end users become accustomed to sculpting
* by dragging point constraints, attempting to sculpt with alpha values is very
* counter intuitive. When sculpting, we recommend that you leave the alpha values
* at their default values and rely solely on loads and constraints for sculpting.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
* <dt><tt>DM_NULL_OUTPUT_PTR</tt></dt>
*<dd>The alpha cannot be NULL on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to be queried.
* @param alpha
* out: resistance to stretch values curves: [alpha=1.0], sized:[1] surfs: [au=1.0, av=1.0, atheta=0.0], sized:[3].
* @param sdmo
* in:SDM_options pointer.
**/

DECL_DM void
DM_get_alpha               // eff: return the dmod's alpha values
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be queried, [pre-alloced]
   double *alpha,          // out: resistance to stretch values, [pre-alloced]
                           //      surfs: [au=1.0, av=1.0, atheta0.0],
                           //      curves: [alpha=1.0]
                           //      sized:[DM_get_domain_dim(rtn_err, dmod)==1?1:3]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Sets the deformable model's alpha and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Sets the deformable model's resistance to stretch alpha value.
* Curves have just one alpha value while surfaces have three, a resistance to
* stretch in the <tt>u_dir</tt> and the <tt>v_dir</tt>, and a rotation angle. For surfaces, alpha is
* <tt>sized:[3]</tt> and stored as <tt>[au, av, atheta]</tt>. See <tt>DM_get_alpha</tt> for a further
* description of alpha and a note about its usage. A <tt>walk_flag</tt> value of <tt>0</tt>
* specifies that just the deformable model is modified, and a walk_flag value of <tt>1</tt>
* specifies that the deformable model and all its offspring are modified. A
* <tt>walk_flag</tt> value of <tt>2</tt> specifies that the deformable model, its siblings, and all
* their offspring be modified.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_BAD_WALK_FLAG_VALUE</tt></dt>
*<dd>The <tt>walk_flag</tt> must be <tt>0</tt>, <tt>1</tt> or <tt>2</tt>.</dd>
*<dt><tt>DM_BAD_ALPHA_VALUE</tt></dt>
*<dd>The value of alpha must be <tt>0</tt> or positive.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to modify.
* @param alpha
* in: desired value to store curves: [alpha=1.0], sized:[1] surfs: [au=1.0, av=1.0, atheta=0.0], sized:[3].
* @param walk_flag
* in: specify how deep to go 0=dmod only, 1=dmod and offspring 2=dmod, siblings, and offspring.
* @param sdmo
* in:SDM_options pointer.
**/

DECL_DM void
DM_set_alpha               // eff: set the dmod's alpha
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be modified, [pre-alloced]
   double *alpha,          // in : desired value to store, [pre-alloced]
                           //      surfs: [au=1.0, av=1.0, atheta=0.0],
                           //      curves: [alpha=1.0]
                           //      sized:[DM_get_domain_dim(rtn_err, dmod)==1?1:3]
   int walk_flag=0,        // in : specify how deep to go
                           //      0=dmod only,1=dmod and offspring
                           //      2=dmod,siblings,and offspring
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the deformable model's beta values and returns <tt>0</tt> or an error.
* <br><br>
* <b>Role:</b> Returns the deformable model's resistance to bending beta values.
* For curves there is only one beta value and it is stored in <tt>beta[0]</tt>. In
* surfaces the beta value is a second order tensor which can be described with
* three numbers, a <tt>u_dir</tt> resistance, a <tt>v_dir</tt> resistance, and a rotation. The
* rotation is the angle between the u and v principal directions of the surface
* and the material property directions. The beta values are stored as <tt>beta[0] = bu,
* beta[1] = bv, beta[2] = btheta.</tt>
* <br><br>
* In practice we have discovered that if end users become accustomed to sculpting
* by dragging point constraints, attempting to sculpt with beta values is very
* counter intuitive. When sculpting, we recommend that you leave the beta values
* at their default values and rely solely on loads and constraints for sculpting.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
* <dt><tt>DM_NULL_OUTPUT_PTR</tt></dt>
*<dd>The beta cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to be queried.
* @param beta
* out: resistance to bending values curves: [beta=5.0], sized:[1] surfs: [bu=5.0, bv=5.0, btheta=0.0], sized:[3].
* @param sdmo
* in:SDM_options pointer.
**/

DECL_DM void
DM_get_beta                // eff: return the dmod's beta values
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be queried, [pre-alloced]
   double *beta,           // out: resistance to bending values, [pre-alloced]
                           //      surfs: [bu=5.0, bv=5.0, btheta=0.0],
                           //      curves: [beta=5.0]
                           //      sized:[DM_get_domain_dim(rtn_err, dmod)==1?1:3]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Sets the deformable model's beta and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Sets the deformable model's resistance to bending beta values.
* Curves have just one beta value while surfaces have three, a resistance to
* bending in the <tt>u_dir</tt> and the <tt>v_dir</tt>, and a rotation angle. For surfaces, beta is
* <tt>sized:[3]</tt> and stored as <tt>[bu, bv, btheta]</tt>. See <tt>DM_get_beta</tt> for a further
* description of beta and a note about its usage. A <tt>walk_flag</tt> value of <tt>0</tt> specifies
* that just the deformable model is modified, and a <tt>walk_flag</tt> value of <tt>1</tt> specifies
* that the deformable model and all its offspring are modified. A <tt>walk_flag</tt> value
* of <tt>2</tt> specifies that the deformable model, its siblings, and their offspring are
* modified.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd>
*<dt><tt>DM_BAD_WALK_FLAG_VALUE</tt></dt>
*<dd>The <tt>walk_flag</tt> must be <tt>0</tt>, <tt>1</tt> or <tt>2</tt>.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to modify.
* @param beta
* out: resistance to bending values curves: [beta=5.0], sized:[1] surfs: [bu=5.0, bv=5.0, btheta=0.0], sized:[3].
* @param walk_flag
* in: specify how deep to go 0=dmod only, 1=dmod and offspring 2=dmod, siblings and offspring.
* @param sdmo
* in:SDM_options pointer.
**/

DECL_DM void
DM_set_beta                // eff: set the dmod's beta
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be modified, [pre-alloced]
   double *beta,           // in : desired value to store, [pre-alloced]
                           //      surfs: [bu=5.0, bv=5.0, btheta=0.0],
                           //      curves: [beta=5.0]
                           //      sized:[DM_get_domain_dim(rtn_err, dmod)==1?1:3]
   int walk_flag=0,        // in : specify how deep to go
                           //      0=dmod only,1=dmod and offspring
                           //      2=dmod,siblings,and offspring
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the deformable model's gamma values and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Returns the deformable model's resistance to the rate of change
* of bending value. For both curves and surfaces there is only one value.
* <br><br>
* The gamma term was added as an experiment to smooth out a deformable shape's
* response to curvature constraints. Without gamma a point curvature constraint
* will only change the curvature of a deformable model shape at the point of the
* constraint. We desire a smooth blending of curvature between the constraint and
* the rest of the shape. This blending is provided by gamma. When gamma equals <tt>0</tt>
* the system behaves as if the new term did not exist. When using curvature
* constraints or <tt>C2</tt> patch seams make sure to use gamma to smooth out the change of
* curvature.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to be queried.
* @param sdmo
* in:SDM_options pointer.
**/

DECL_DM double             // rtn: resist rate of bending change val
DM_get_gamma               // eff: return the dmod's gamma values
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be queried, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Sets the deformable model's gamma and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies deformable <tt>model->dmo_gamma</tt>
* <br><br>
* Sets the deformable model's resistance to bending rate of change gamma values.
* <br><br>
* A <tt>walk_flag</tt> value of <tt>0</tt> specifies that just the deformable model is modified, and
* a <tt>walk_flag</tt> value of <tt>1</tt> specifies that the deformable model and all its offspring
* are modified. A walk_flag value of <tt>2</tt> specifies that the deformable model, its
* siblings, and all their offspring are modified. are modified. See <tt>DM_get_gamma</tt>.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd>
*<dt><tt>DM_BAD_WALK_FLAG_VALUE</tt></dt>
*<dd>The walk_flag must be 0, 1 or 2.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to modify.
* @param gamma
* desired value to store.
* @param walk_flag
* in: specify how deep to go 0=dmod only, 1=dmod and offspring 2=dmod, siblings and offspring.
* @param sdmo
* in:SDM_options pointer.
**/

DECL_DM void
DM_set_gamma               // eff: set the dmod's gamma
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be modified, [pre-alloced]
   double gamma=0.0,       // in : desired value to store
   int walk_flag=0,        // in : specify how deep to go
                           //      0=dmod only,1=dmod and offspring
                           //      2=dmod,siblings,and offspring
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the deformable model's delta values and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies delta.
* <br><br>
* Returns the deformable model's resistance to displacement delta values. For both
* curves and surfaces there is only one value. Returns the deformable model's
* resistance to displacement delta values. For both curves and surfaces there is
* only one value.
* <br><br>
* The delta term was added as an experiment to approximate the behavior of the
* default shape mechanism before it was built. Since default shapes have been
* added to the system it is no longer necessary to use a non-zero value for delta
* when sculpting. The default shapes with delta equal to <tt>0</tt> are much more intuitive
* to use.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to be queried.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM double             // rtn: resistance to displacement values
DM_get_delta               // eff: return the dmod's delta values
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be queried, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer


/**
* Sets the deformable model's delta and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies deformable <tt>model->dmo_delta</tt>.
* <br><br>
* Sets the deformable model's resistance to displacement delta values. A <tt>walk_flag</tt>
* value of <tt>0</tt> specifies that just the deformable model is modified, and a <tt>walk_flag</tt>
* value of <tt>1</tt> specifies that the deformable model and all its offspring are
* modified. A <tt>walk_flag</tt> value of <tt>2</tt> specifies that the deformable model, its
* siblings, and all their offspring are modified.
* <br><br>
* The delta term was added as an experiment to approximate the behavior of the
* default shape mechanism before it was built. Since default shapes have been
* added to the system, we do not recommend that you use a non-zero value for delta
* when sculpting. The default shapes with delta set to <tt>0</tt> are much more intuitive
* to use.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_BAD_WALK_FLAG_VALUE</tt></dt>
*<dd>The <tt>walk_flag</tt> must be <tt>0</tt>, <tt>1</tt> or <tt>2</tt>.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to modify.
* @param delta
* in: desired value to store.
* @param walk_flag
* in: specify how deep to go 0=dmod only, 1=dmod and offspring 2=dmod, siblings and offspring.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_set_delta               // eff: set the dmod's delta
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be modified, [pre-alloced]
   double delta=0.0,       // in : desired value to store
   int walk_flag=0,        // in : specify how deep to go
                           //      0=dmod only,1=dmod and offspring
                           //      2=dmod,siblings,and offspring
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the deformable model's default shape values.
* <br><br>
* <b>Role:</b> Modifies <tt>shape_flag</tt>.
* <br><br>
* Returns the deformable model's <tt>shape_flag</tt> state as enabled or disabled.
* <br><br>
* Deformable models change their shape to reduce their internal energy to a
* minimum. That is, they naturally deform to resist stretching and bending. When
* the default shape mechanism is disabled, all deformable models want to be flat
* and of zero area. That is, they will shrink to a single point if they are solved
* with absolutely no constraints or loads to force them to behave otherwise. This
* is fine for about half the modeling jobs where one can start with a simple shape
* and through sculpting with large deformations create much more complicated
* shapes. The default shape mechanism can change the behavior of shrinking to a
* flat zero-area shape. When enabled, the <tt>default_shape</tt> is the rest shape that the
* deformable model will find if it is solved with no loads or constraints. The
* default shape mechanism is good for modifying an existing shape by a smooth
* amount. To do this, first capture the initial shape as the default shape with
* the command <tt>DM_set_default_shape()</tt> and then sculpt as normal.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to be queried.
* @param sdmo
* in:SDM_options pointer.
**/

DECL_DM int                // rtn: 0: shape_flag = disabled
                           //      1: shape_flag = enabled
DM_get_default_shape       // eff: return the dmod's default_shape values
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be queried, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Sets the deformable model's default shape and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies deformable <tt>model->dmo_default_shape</tt>
* <br><br>
* Sets the deformable model's resistance to displacement default shape values. A
* <tt>walk_flag</tt> value of <tt>0</tt> specifies that just the deformable model is modified, a
* <tt>walk_flag</tt> value of <tt>1</tt> specifies that the deformable model and all its offspring
* are modified, and a value of <tt>2</tt> specifies that the deformable model, its siblings,
* and all of their offspring are modified.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_BAD_SHAPE_FLAG_VALUE</tt></dt>
*<dd>The default shape must be <tt>0</tt> or <tt>1</tt>.</dd>
*<dt><tt>DM_BAD_WALK_FLAG_VALUE</tt></dt>
*<dd>The <tt>walk_flag</tt> must be <tt>0</tt>, <tt>1</tt> or <tt>2</tt>.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to modify.
* @param shape_flag
* in: 0=disable default shapes 1=set current shape as default shape.
* @param walk_flag
* in: specify how deep to go 0=dmod only, 1=dmod and offspring 2=dmod, siblings, and offspring..
* @param sdmo
* in:SDM_options pointer.
**/

DECL_DM void
DM_set_default_shape       // eff: set the dmod's default_shape
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be modified, [pre-alloced]
   int shape_flag,         // in : 0=disable default shapes
                           //      1=set current shape as default shape
   int walk_flag=0,        // in : specify how deep to go
                           //      0=dmod only,1=dmod and offspring
                           //      2=dmod,siblings,and offspring
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the deformable model's dynamic values and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies <tt>dt</tt>, <tt>mass</tt>, <tt>damp</tt>.
* <br><br>
* Deformable modeling is a simulation that mimics the behavior of actual physical
* systems. Each call to <tt>DM_solve()</tt> increments sculpting time by one time step. The
* deformable model equations are the equations of motion for a physical system.
* The internal energy, loads, and constraints define the stiffness for that system
* of equations. The mass and damp terms add an effective mass and damping to those
* equations of motion. A system with damping may take several time steps to reach
* an equilibrium position. A system with mass tends to overshoot and oscillate
* back and forth across the equilibrium position. Using dynamics one can create a
* very convincing animation in which each time step is one frame of the animation
* sequence. Changes in loads and constraints during the animation can create very
* "real" behaviors. For modeling purposes where the end user is only interested in
* defining the shape of an object, the dynamics feature may be of little interest.
* In these cases, both mass and damp should be set to <tt>0.0</tt>, which turns the dynamic
* behavior off and causes <tt>DM_solve()</tt> to solve directly for the equilibrium
* position. For animation purposes where the end user is concerned with how an
* object moves from one position to another, however, this is a very useful tool.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to be queried.
* @param dt
* out: dynamic sculpting time step size.
* @param mass
* out: dynamic deformable mass value.
* @param damp
* out: dynamic deformable damping value.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_get_dynamics            // eff: return the dmod's dynamics values
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be queried, [pre-alloced]
   double &dt,             // out: dynamic sculpting time step size
   double &mass,           // out: dynamic deformable mass value
   double &damp,           // out: dynamic deformable damping value
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Sets the deformable model's dynamics and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies deformable <tt>model->dynamics</tt> terms
* <br><br>
* Sets the deformable model's <tt>dt</tt>, <tt>mass</tt>, and damping dynamics values. See
* <tt>DM_get_dynamic</tt>.
* <br><br>
* To turn off dynamic behavior, set both mass and damp to <tt>0.0</tt>.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* The deformable model cannot be <tt>NULL</tt> on entry.
*<dt><tt>DM_BAD_WALK_FLAG_VALUE</tt></dt>
*<dd>The <tt>walk_flag</tt> must be <tt>0</tt>, <tt>1</tt> or <tt>2</tt>.</dd>
*<dt><tt>DM_BAD_DT_VALUE</tt></dt>
*<dd>The value of <tt>dt</tt> must be positive.</dd>
*<dt><tt>DM_BAD_MASS_VALUE</tt></dt>
*<dd>The value of mass must be <tt>0</tt> or positive.</dd>
*<dt><tt>DM_BAD_DAMP_VALUE</tt></dt>
*<dd>The value of damp must be <tt>0</tt> or positive.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to modify.
* @param dt
* in: dynamic sculpting time step size.
* @param mass
* in: dynamic deformable mass value.
* @param damp
* in: dynamic deformable damping value.
* @param walk_flag
* in: specify how deep to go 0=dmod only, 1=dmod and offspring 2=dmod,siblings, and offspring.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_set_dynamics            // eff: set the dmod's dynamics
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be modified, [pre-alloced]
   double dt=1.0,          // in : dynamic sculpting time step size
   double mass=1.0,        // in : dynamic deformable mass value
   double damp=5.0,        // in : dynamic deformable damping value
   int walk_flag=0,        // in : specify how deep to go
                           //      0=dmod only,1=dmod and offspring
                           //      2=dmod,siblings,and offspring
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the deformable model's <tt>dof_state</tt> values and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies <tt>dof_count</tt>, <tt>free_count</tt>, <tt>_count</tt>, <tt>fixed_count</tt> <tt>lambda_count</tt>,
* <tt>lfixed_count</tt>.
* <br><br>
* Returns the deformable model's <tt>dof_state</tt>. The term <tt>"dof"</tt> stands for Degree of
* Freedom. The deformable model resolves down to a set of simultaneous equations
* that must be solved. This system of equations has a total number of degrees of
* freedom which is just the number of control points for <i>B</i>-spline and <tt>NURB</tt>
* surfaces. All this means is that all the control points are degrees of freedom
* that can be moved by the deformable modeling algorithm. Each constraint applied
* to the system nails down or removes one or more degrees of freedom from the
* system. The remaining number of degrees of freedom, called the <tt>free_count</tt> are
* all the degrees of freedom that the system has left to deform the shape of the
* model. When the <tt>free_count</tt> gets too low because several constraints have been
* added to the system or because the end user is trying to deform a degree <tt>0</tt>
* <i>B</i>-spline, then the deformable model will not deform in a natural manner. The
* application should detect these conditions by examining these values after a
* call <tt>DS_solve()</tt>. When the free_count gets low (less than <tt>20%</tt> of the total dofs)
* then consider adding more dofs to the system with calls to
* DS_elevate_dmod_degree() and <tt>DS_split_ deformable model()</tt>. <tt>cstrn_count</tt> tells how
* many total constraint equations may be in effect. Often times this number is
* larger than the total dof count. The fixed_count is the number of independent
* constraint equations. The <tt>total_count</tt> will always equal the sum of the
* free_count and the <tt>fixed_count</tt>. A <i>B</i>-spline control point is not really one dof
* but three, since it can move in three dimensional space. Some constraints, like
* the surface normal constraint, cannot be written with one equation for one
* control point. Instead, such constraints can be written with one equation for
* each coordinate of the control point. These constraints are enforced using
* lagrange multipliers. <tt>lambda_count</tt> is the number of lagrange multiplier
* constraint equations, and one <tt>fixed_count</tt> is the number of independent lagrange
* multiplier constraint equations.
* <br><br>
* When adding degrees of freedom to a system we first bump up the degree of the
* system to three when no patches are used and to four when patches are present
* with <tt>DM_elevate_dmod_degree</tt>. Then we split the deformable model into a larger
* number of elements making sure each element is the same size as the rest with a
* call to <tt>DM_split_dmod()</tt>.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to be queried.
* @param dof_count
* out: total surface dof count.
* @param free_count
* out: number of free dofs to deform.
* @param cstrn_count
* out: total number of constraint equations.
* @param fixed_count
* out: independent constraint equations cnt.
* @param lambda_count
* out: lambda constraint eqn count.
* @param lfixed_count
* out: independent lambda constraint count.
* @param sdmo
* in:SDM_options pointer  note: use after a call to DM_solve().
**/
DECL_DM void
DM_get_dof_state           // eff: return the dmod's dof_state values
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be queried, [pre-alloced]
   int &dof_count,         // out: total surface dof count
   int &free_count,        // out: number of free dofs to deform
   int &cstrn_count,       // out: total number of constraint equations
   int &fixed_count,       // out: independent constraint equations cnt
   int &lambda_count,      // out: lambda constraint eqn count
   int &lfixed_count,      // out: independent lambda constraint count
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: use after a call to DM_solve()

/**
* Gets the deformable model's <tt>elem_count</tt> values and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies <tt>elem_count</tt>, <tt>u_span_count</tt>, <tt>v_span_count</tt>.
* <br><br>
* Returns the deformable model's <tt>elem_count</tt>. Places the deformable model's total
* <tt>elem_count</tt> in <tt>elem_count</tt>. For tensor product deformable models (like <i>B</i>-splines
* and <tt>NURB</tt>s) places the number <i>u</i> direction spans in the <tt>u_span_count</tt> and the
* number of <i>v</i>-direction spans in the <tt>v_span_count</tt>. <tt>v_span_count</tt> is not set when
* the input deformable model is a deformable curve.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to be queried.
* @param elem_count
* out: total dmod elem count.
* @param u_span_count
* out: span count in u_direction.
* @param v_span_count
* out: span count in v_direction (only set for surfaces).
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_get_elem_count          // eff: return the dmod's elem_count values
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be queried, [pre-alloced]
   int &elem_count,        // out: total dmod elem count
   int &u_span_count,      // out: span count in u_direction
   int &v_span_count,      // out: span count in v_direction
                           //      (only set for surfaces)
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the deformable model's entity pointer value and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Returns the deformable model's application entity pointer value.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to be queried.
* @param sdmo
* in:SDM_options pointer.
**/

DECL_DM void *             // rtn: application ptr stored with dmod, [pass-thru]
DM_get_entity              // eff: return dmod's entity ptr value
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be queried, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Sets the deformable model's entity pointer value and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies deformable <tt>model->dmo_entity</tt>
* <br><br>
* Sets the deformable model's application entity pointer value.
* <br><br>
* This value is never used by the deformable modeling library it is just stored
* and retrieved with the deformable model for an application's convenience.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to modify.
* @param entity
* in: desired value to store.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_set_entity              // eff: set the dmod's entity
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be modified, [pre-alloced]
   void *entity,           // in : desired value to store, [pass-thru]
   SDM_options *sdmo=NULL);// in : SDM_options pointer


//============================================================
// Constraint Management
//============================================================

//============================================================
// DM_add_pt_cstrn(),   DM_add_crv_cstrn(),
// DM_add_link_cstrn(), DM_add_area_cstrn()
//============================================================

/**
* Adds a point constraint to the target deformable model and returns a new tag identifier or an error.
* <br><br>
* <b>Role:</b> Builds and adds to the target deformable model a point
* constraint with a domain point equal to <tt>dpt</tt>.
* <br><br>
* The basic flavor for using a point constraint is:
* <br><br><ol>
*<li>Create and apply the constraint to a deformable model,
* (<tt>DM_add_pt_cstrn()</tt>).</li>
* <br>
* <li>Select which geometry properties are being constrained,
* (<tt>DM_set_cstrn_behavior()</tt>).</li>
* <br>
* <li>For tracking <tt>pt_cstrns</tt>, modify the geometry property values by moving
* the point constraint display points as described above,
* (<tt>DM_set_pt_xyz()</tt>).</li>
* <br>
*<li>Call solve to see how the constraint modifications changed the
* deformable model shape,
* (<tt>DM_solve()</tt>).</li>
* <br>
*<li>Optionally, change the point constraint's domain position and domain
* directions (directions for directional derivatives on surfaces) with
* <br><ol type = a>
* <li>(<tt>DM_set_pt_uv()</tt>),</li>
* <br>
* <li><tt>(DM_set_cstrn_pttan_uv_dir())</tt>.</li></ol>
* <li>Optionally, modify the constraint <tt>image_pt</tt> locations to optimize viewing
* and/or interactions,
* <tt>DM_set_tan_display_gain()</tt>
* <tt>DM_set_comb_graphics()</tt></li>
* <br><br></ol>
* The point constraint is a device to allow an end-user to specify the
* differential geometry properties (position, tangent, normal, and curvature) of a
* point on a deformable model shape. The point constraint works for surfaces,
* curves in <tt>3D</tt>, and curves in <tt>2D</tt>. The geometric properties constrained by a point
* constraint object are called the behaviors of the point constraint. Many but not
* all combinations of the constraint behaviors are allowed.
* <br><br>
* A point constraint can control the following geometry properties:
* <br><br>
* <table border="0" width="80%">
* <tr>
* <td width="33%" align="left">Curves:</td>
* <td width="67%" align="left">Surfaces:</td>
* </tr>
* <tr>
* <td width="33%" align="left"> Position</td>
* <td width="67%" align="left"> Position</td>
* </tr>
* <tr>
* <td width="33%" align="left"> Tangent (velocity)</td>
* <td width="67%" align="left"> Tangent1 in domain1_dir direction</td>
* </tr>
* <tr>
* <td width="33%" align="left">Normal direction</td>
* <td width="67%" align="left">Tangent2 in <tt>domain2_dir</tt> direction</td>
* </tr>
* <tr>
* <td width="33%" align="left"> Curvature+Normal+Tangent</td>
* <td width="67%" align="left"> Normal direction</td>
*</tr>
* <tr>
* <td width="33%" align="left"><font face="Times" size="3">Binormal direction</font></td>
*<td width="67%" align="left">Curvature1+Normal+Tangent1 in <tt>domain1_dir</tt> direction</td>
*</tr>
* </table>
*<br><br>
* 			Curvature2+Normal+Tangent2 in <tt>domain2_dir</tt> direction.
* <br><br>
* To completely specify a position constraint on a curve or surface, or normal
* direction constraint behavior on a curve or surface, or a curvature constraint
* on a curve, or a binormal constraint on a curve, the end-user must supply the
* following set of information.
* <br><br><ol>
* <li>location on the surface or curve, given by a parametric position</li>
* <li>a value for the geometric property being constrained</li>
* <br><ol type = a>
* <li>a position in image space for the position constraint</li>
* <li>a vector in image space for the normal constraint and a <tt>2nd</tt> image space
* vector specifying the tangent direction</li>
* <li>the curvature value, the normal direction, and the tangent direction</li>
* <li>a vector in image space for the binormal constraint.</li></ol></ol>
*<br><br>
* The tangent and curvature constraint behaviors for a point constraint on a
* surface constrain the surface geometric properties in a direction at a point on
* the surface. A surface can have two independent tangent and/or curvature
* constraints as long as the domain directions for each constraint is different.
* <br><br>
* To completely specify a tangent or curvature point constraint behavior on a
* surface the end-user must supply the following set of information,
* <br><br>
* <b>Viewing and Modifying the Constraint Parameters</b>
* <br>
*<ol>
* <li>location on the surface(<tt>domain_pt</tt>), given by a parametric position</li>
* <li>direction on the surface(<tt>domain_dir</tt>), given by a parametric vector</li>
* <li>value for the geometric property being constrained,</li>
* <br><ol type = a >
* <li>for tangent constraints; an image space vector used to specify the image
* space direction of the tangent.</li>
* <li> for curvature constraints; the curvature value, an image space vector
* used to specify the direction of the surface normal, and a <tt>2nd</tt> image space
* vector used to specify the surface tangent at that point.</li></ol></ol>
* <br><br>
* In an effort to make the point constraint intuitive to use, the point constraint
* geometric values may be set indirectly through a set of image points (sometimes
* called display points). Every geometric property which may be constrained by a
* point constraint is associated with a display point. To modify the geometric
* value of a point constraint, such as its position or its tangent direction, move
* the location of the associated <tt>image_pt</tt>.
* <br><br>
* To help support interactive mouse-based editing of the constraint values, the
* function <tt>DM_set_pt_xyz()</tt> can be used to directly set the <tt>image_pt</tt> location or to
* compute the image point location given an image space line (which can be
* generated from a viewing pick ray).
* <br><br>
* These points include:
* <br><br>
* <tt>base_pt</tt> = location of the position constraint and base for all vector values.
* <br><br>
* <tt>tang1_pt</tt> = <tt>tan_display_gain</tt> * <tt>(base_pt + tang1_val)</tt> vector end point used to
* specify the curve tangent and the surface <tt>tangent1</tt> constraint vectors as;
* <br><br>
* <tt>tang2_pt</tt> = <tt>tan_display_gain * (base_pt + tang2_val)</tt> Vector end point used to
* specify the surface tangent2 vector.
* <br><br>
* <tt>norm_pt = tan_display_gain * norm_size*(base_pt + norm_val)</tt> Vector end point
* used to specify the normal vector direction for a curve or surface. The normal
* vector sets the normal direction for the normal constraint and the curvature
* constraints. When a curvature constraint is being used, the <tt>norm_pt</tt> is not
* checked for picking or plotting, rather the <tt>curv_pts</tt> are used. <tt>norm_pt</tt> is not
* used by itself it may be used with either or both <tt>curv_pt</tt> and <tt>binorm_pt</tt>.
* <br><br>
* <tt>binorm_pt= tan_display_gain * binrom_size*(base_pt + binorm_val)</tt> Vector end
* point used to specify the plane containing the tangent and normal directions for
* a curve. binorm may be used by itself or with the tangent or with <tt>norm_pt</tt>.
* <br><br>
* <tt>curv1_pt = C2_display_gain * (base_pt + curv1_val*norm_val)</tt> where <tt>curv1_val</tt> =
* curvature value for curves or for surface in the domain1_dir direction, norm_val
* = curve or surface unit-sized normal-vector. When in use, the <tt>norm_val</tt> vector is
* always perpendicular to the <tt>tang1_val</tt>.
* <br><br>
* <tt>curv2_pt = C2_display_gain * (base_pt + curv2_val*norm_val)</tt> where curv2_val =
* curvature value for surfaces in the domain2_dir direction, and <tt>norm_val</tt> =
* surface unit-sized normal-vector.
* <br><br>
* The <tt>tan_display_gain</tt> and <tt>C2_display_gain</tt> parameters are included in the point
* definitions so that the size of the displayed vectors may be modified at run
* time for viewing convenience.
* <br><br>
* <b>Input Argument Use:</b>
* <br><br>
* When <tt>domain_flag</tt> is set to <tt>1</tt>, the input <tt>dpt</tt> is given in the unit-square space.
* When <tt>domain_flag</tt> is set to 0, dpt is given in the <tt>orig_dmod_space</tt>. When
* <tt>domain_flag</tt> is set to <tt>2</tt>, <tt>dpt</tt> is given in the dmod's <tt>pfunc</tt>'s <tt>internal_pfunc_space</tt>.
* <br><br>
* The constraint's <tt>base_pt</tt> location, (the point's location in image space) is set
* to the ipt value when ipt is not equal to <tt>NULL</tt>. When ipt equals <tt>NULL</tt>, the
* constraint's <tt>base_pt</tt> image point is set to the result of evaluating the
* deformable model's shape at the given dpt value. Similarly, the inputs
* <tt>domain1_dir</tt>, <tt>domain2_dir</tt>, <tt>tang1_val</tt>, <tt>tang2_val</tt>, <tt>norm_val</tt>, <tt>curv1_val</tt>, and
* <tt>curv2_val</tt> are all used to set the parameters of the point constraint unless they
* are <tt>NULL</tt> (<tt>-1.0</tt> for <tt>curv1_val</tt> and <tt>curv2_val</tt>), in which case those parameter
* values default to values found by evaluating the constrained shape.
* <br><br>
* The default values for <tt>domain1_dir</tt> and <tt>domain2_dir</tt> are computed in two steps. If
* the associated <tt>tang_val</tt> vector is given then the <tt>domain_dir</tt> direction is found
* by projecting that vector into the domain space of the constrained shape. When
* the associated <tt>tang_val</tt> is also set to <tt>NULL</tt> then <tt>domain1_dir</tt> defaults to the
* <tt>u_direction = [1 0]</tt>, and the <tt>domain2_dir</tt> defaults to the <tt>v_direction = [0 1]</tt>.
* For curves domain1_dir is always equal to the +s direction and the arrays
* <tt>domain1_dir</tt> and <tt>domain2_dir</tt> are not used.
* <br><br>
* The bits in behavior determine what properties are constrained at the given
* domain point. When the <tt>SPA_DM_POS_FIXED</tt> bit is set, the position of the point is
* constrained. When the <tt>SPA_DM_TAN_FIXED</tt> bit is set, the tangent at the point is
* constrained. When the <tt>SPA_DM_CURV_FIXED</tt> bit is set, the <tt>SPA_DM_TAN_FIXED</tt> and the
* <tt>SPA_DM_NORM_FIXED</tt> bits are set by the system and the curvature at the point is
* constrained. The legal values for behavior include:
* <br><br>
* For points on curves:
* <br><br>
* - <tt>SPA_DM_POS_FIXED</tt>,
* <br>
* - <tt>SPA_DM_TAN_FIXED</tt>,
* <br>
* - <tt>SPA_DM_NORM_FIXED</tt>,
* <br>
* - <tt>SPA_DM_BINORM_FIXED</tt>,
* <br>
* - <tt>SPA_DM_CURV_FIXED</tt>.
* <br><br>
* For points on surfaces:
* <br><br>
* - <tt>SPA_DM_POS_FIXED</tt>,
* <br>
* - <tt>SPA_DM_TAN_FIXED</tt>,
* <br>
* - <tt>SPA_DM_TAN_2_FIXED</tt>,
* <br>
* - <tt>SPA_DM_NORM_FIXED</tt>,
* <br>
* - <tt>SPA_DM_CURV_FIXED</tt>,
* <br>
* - <tt>SPA_DM_CURV_2_FIXED</tt>.
* <br><br>
* <tt>tag_flag</tt> selects the target deformable model. When tag equals <tt>-1</tt> the next
* available tag number is assigned to the newly created load. Otherwise, the input
* tag number is used. Other valid values for the tag flag are:
* <br><br>
* <table border="0" width="100%">
* <tr>
* <td width="3%" align="left"><tt>1</tt></td>
* <td width="97%" align="left">active deformable model</td>
* </tr>
* <tr>
* <td width="3%" align="left"><tt>2</tt></td>
* <td width="97%" align="left"> root deformable model</td>
* </tr>
*</table><br>
* Otherwise, the target is the deformable model whose tag identifier equals
* <tt>abs(tag_flag)</tt>.
* <br><br>
* Returns a tag number for the newly added constraint point.
* <br><br>
* <b>Errors:</b>
*<tt>DM_parse_tag_flag()</tt> errors
*<dt><tt>DM_scale_unit_dpt_to_pfunc()</tt></dt>
*<dd>errors</dd>
*<dt><tt>DM_NO_ROOT_DMOD</tt></dt>
*<dd>The the input deformable model cannot be <tt>NULL</tt>.</dd>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The input dpt value cannot be <tt>NULL</tt>.</dd>
*<dt><tt>DM_BAD_DOMAIN_PT_RANGE</tt></dt>
*<dd>The domain point must be completely contained by the deformable model.</dd>
*<dt><tt>DM_BAD_TAG_VALUE</tt></dt>
*<dd>The tag value must be <tt>-1</tt>, to have the system assign one, or <tt>2</tt> or greater.</dd>
*<dt><tt>DM_BAD_SRC_TYPE_PARENT</tt></dt>
* <dd>When <tt>src_type == ds_seam_cstrn</tt>, <tt>parent_dmod</tt> must be <tt>non-NULL</tt>, else <tt>parent_dmod</tt>
*must be <tt>NULL</tt>.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: hierarchy member.
* @param tag_flag
* in: specify tgt dmod 1 = active dmod, 2 = root dmod, else = member dmod with tag=tag_flag.
* @param parent_dmod
* opt: seam's parent shape or NULL.
* @param domain_flag
* specify dpt space 0=orig_dmod_space, 1=unit_space, 2= internal_pfunc_space.
* @param dpt
* in: pt's domain loc, sized:[Domain_dim].
* @param src_data
* opt: carried with cstrn object.
* @param src_type
* in:  cstrn origin. one of: ds_solid_cstrn, ds_bound_cstrn, ds_user_cstrn, ds_seam_cstrn.
* @param behavior
* or of: SPA_DM_POS_FIXED SPA_DM_TAN_FIXED SPA_DM_CURV_FIXED SPA_DM_TAN_2_FIXED SPA_DM_CURV_2_FIXED SPA_DM_NORM_FIXED.
* @param ipt
* in: pt's image pos or NULL to default, sized:[Image_dim].
* @param domain1_dir
* in:  1st directional derivative direction or NULL defaults to proj of tang1_val if possible else u_dir = [1 0]. sized:[Domain_dim].
* @param domain2_dir
* in:  2nd directional derivative direction or NULL defaults to proj of tang2_val if possible else v_dir = [0 1]. sized:[Domain_dim].
* @param tang1_val
* in:  dir1 tangent vector or NULL to default. sized:[Image_dim].
* @param tang2_val
* in:  dir2 tangent vector or NULL to default. sized:[Image_dim].
* @param norm_val
* in:  normal vector value or NULL to default. sized:[Image_dim].
* @param binorm_val
* in:  binormal vector value for curves or NULL. sized:[Image_dim].
* @param curv1_val
* in:  dir1 curvature value or NULL to default. sized:[1].
* @param curv2_val
* in:  dir2 curvature value or NULL to default. sized:[1].
* @param tag
* in: when == -1 assign next tag number else use this tag number.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/

DECL_DM int                // rtn: new tag id or negative err code

DM_add_pt_cstrn            // eff: add a pt constraint
 (int &rtn_err,            // out: 0=success or neg err code
  DS_dmod *dmod,           // in : dmod hierarchy member, [pre-alloced]
  int tag_flag,            // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
  DS_dmod *parent_dmod,    // opt: seam's parent shape or NULL, [pre-alloced]
  int domain_flag,         // in : 0=dpt in orig_dmod_space,
                           //      1=dpt in unit_space,
                           //      2=dpt in internal_pfunc_space.
  double *dpt,             // in : pt's domain loc in unit square, [pre-alloced]
                           //      sized:[DM_get_domain_dim(rtn_err, dmod)]
  void *src_data = NULL,   // opt: pass-thru data, [pass-thru]
  DS_CSTRN_SRC src_type=   // in : cstrn origin. oneof:
    ds_user_cstrn,         //  ds_solid_cstrn
                           //  ds_bound_cstrn
                           //  ds_user_cstrn
                           //  ds_seam_cstrn
  int behavior=            // in : orof
       SPA_DM_POS_FIXED,   //      SPA_DM_POS_FIXED
                           //      SPA_DM_TAN_FIXED
                           //      SPA_DM_CURV_FIXED
                           //      SPA_DM_TAN_2_FIXED
                           //      SPA_DM_CURV_2_FIXED
                           //      SPA_DM_NORM_FIXED
                           //      SPA_DM_BINORM_FIXED
  double *ipt=NULL,        // in : pt's image pos or NULL, [pre-alloced]
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
  double *domain1_dir=NULL,// in : 1st directional derivative direction
                           //      or NULL defaults to proj of tang1_val if
                           //      possible else to u_dir = [1 0].
                           //      sized:[DM_get_domain_dim(rtn_err, dmod)]
  double *domain2_dir=NULL,// in : 2nd directional derivative direction
                           //      or NULL defaults to proj of tang2_val if
                           //      possible else to v_dir = [0 1].
                           //      sized:[DM_get_domain_dim(rtn_err, dmod)]
  double *tang1_val=NULL,  // in : dir1 tangent SPAvector
                           //      or NULL to default.
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
  double *tang2_val=NULL,  // in : dir2 tangent SPAvector
                           //      or NULL to default.
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
  double *norm_val=NULL,   // in : normal SPAvector value
                           //      or NULL to default.
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
  double *binorm_val=NULL, // in : binormal SPAvector value for curves
                           //      or NULL to default.
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
  double *curv1_val=NULL,  // in : dir1 curvature value or NULL
                           //      to default. sized:[1]
  double *curv2_val=NULL,  // in : dir1 curvature value or NULL
                           //      to default. sized:[1]
  int tag=-1,              // in : when == -1 assign next tag number
                           //      else use this tag number
   SDM_options *sdmo=NULL);// in : SDM_options pointer

                           // note: sets active dmod

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM int                // rtn: new tag id or negative err code
DM_add_area_C0_load        // eff: adds area_load to DS_pfunc
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of target dmod hierarchy, [pre-alloced]
   int tag_flag,           // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
   int zone_flag,          // in : 0=zone area moves
                           //        zone compliment fixed
                           //      1=zone area fixed
                           //        zone compliment moves
   DS_zone *&zone,         // in : defines constraint partition, [nested]
   DS_pfunc *src_W_pfunc,  // in : target shape
   void *src_data ,        // opt: app data stored with area_cstrn, [pass-thru]
   double gain,            // in : load gain
   int tag,                // opt: when = -1 assign next tag number
                           //      else use this tag number
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod

/**
* Adds a curve constraint to the target model and returns a new tag identifier or a negative error code.
* <br><br>
* <b>Role:</b> Modifies the target deformable model.
* <br><br>
* Builds and adds a curve constraint to the target deformable model. The shape of
* the curve constraint is really two curves, one in the original domain space of
* the target deformable model and another curve shape specified within the target
* deformable model's image space. The shape of the curve in the target deformable
* model's original domain space is given by <tt>C(s) = [u(s), v(s)]</tt> where each valid
* curve parameter value, <i>s</i>, is used to compute curve positions <i>u</i> and <i>v</i>. The curve
* positions <i>u</i> and <i>v</i> must be returned in the original domain space of the
* constrained deformable model. The shape of the curve in three dimensional image
* space is given by <tt>W(s) = [x(s), y(s), z(s)]</tt> where each valid curve parameter
* value, <i>s</i>, is used to compute curve positions <i>x</i>, <i>y</i>, and <i>z</i>.
* <br><br>
* The parametric shape of the curve can be given in two ways: either by an
* explicit curve represented by a <tt>DS_pfunc</tt> (the <tt>src_C_pfunc</tt> input argument), or by
* a callback function (the <tt>src_CW_func</tt> input argument) but not by both. That is,
* only one of <tt>src_C_pfunc</tt> and <tt>src_CW_func</tt> can be something other than <tt>NULL</tt> on
* input. The <tt>src_CW_func</tt> must specify the shape of the constraint curve in the
* <tt>orig_dmod_space</tt>. That is each call to <tt>src_CW_func(s)</tt> must return <tt>C</tt> and <tt>Cs</tt> values
* as measured in the domain space of the deformable model at the time it was
* constructed. When the domain curve shape is specified by <tt>src_C_pfunc</tt>, the
* <tt>domain_flag</tt> value is used to determine in which space the curve was built. When
* <tt>domain_flag == 0</tt>, the src_C_<tt>pfunc</tt>'s image_space is equal to the <tt>orig_dmod_space</tt>.
* When <tt>domain_flag == 2</tt>, the <tt>src_C_pfunc</tt>'s image_space is equal to the
* internal_pfunc_space. When <tt>src_C_pfunc</tt> is supplied in the <tt>orig_dmod_space</tt>, it is
* scaled by this function from the <tt>orig_dmod_space</tt> to the <tt>internal_pfunc_space</tt> and
* its pointer is stored internally. <tt>src_C_pfunc</tt> may not be described to this
* function in <tt>unit_space</tt>.
* <br><br>
* The image space shape of the curve can be defined in three ways defining three
* different types of curve constraints. Seam constraints ( <tt>ds_seam_cstrn</tt>) are used
* to connect the constrained deformable model patch to its parent. User
* constraints (<tt>ds_user_cstrn</tt>) are used to allow the user to specify any curve
* within the deformable model to be constrained from further motion. And three
* dimensional curve constraints in which the curve shape is defined by an external
* curve, typically a curve in a solid model. There are two types of three
* dimensional curves: ds_bound_cstrn in which the boundary is not shared by any
* other faces as in the boundary of a sheet model, and <tt>ds_solid_cstrn</tt> in which the
* boundary is shared by other faces as with <tt>2</tt> manifold models.
* <br><br>
* The image space shape of a seam constraint is computed by evaluating the shape
* of the parent deformable model. For seam constraints the <tt>parent_dmod</tt> argument
* must be something other than <tt>NULL</tt>. The curve constraint for a seam must be built
* in the parent's original domain range.
* <br><br>
* The image space shape of a user constraint is computed by evaluating the shape
* of the deformable model or by evaluating an explicit function given by the
* <tt>src_W_pfunc</tt> argument. The <tt>src_W_pfunc</tt> argument is optional for user constraint
* curves. Similarly, the cross-tangent and the cross-curvature values can be
* specific functions represented by <tt>DS_pfunc</tt> objects in <tt>src_Wn_pfunc</tt> and
* <tt>src_Wnn_pfunc</tt>. When supplied and the behavior of the curve constraint is set to
* <tt>DM_POSITN</tt>, <tt>DM_TANG</tt> and/or <tt>DM_CURVATURE</tt> the shape of the constrained shape is
* deformed to satisfy the constraints,
* <pre>
* W(C(s)) = src_W_pfunc(s)  		(an image_space vector function)<br>
* Wn(C(s)) = src_Wn_pfunc(s) and 	(an image_space vector function)<br>
* k(C(s)) = src_Wnn_pfunc(s) 		(a scalar function)</pre> Where <i>s</i> is the parameter variable used to describe point locations on the const
* <br><br>
* <i>Wn(s)</i> is the cross tangent vector for value <i>s</i>
* <i>Wnn(s)</i> is the cross <tt>2nd</tt> parametric derivative for value <i>s</i><br>
* <i>Wu(s)</i> is the <i>u</i> direction parametric derivative <i>dW/du</i> for value <i>s</i><br>
* <i>Wv(s)</i> is the <i>v</i> direction parametric derivative <i>dW/dv</i> for value <i>s</i><br>
* <i>k(s) = size(cross(Wn,Wnn)/ size (Wn)**3</i> = the cross curvature value
*   for value and<br>
* <i>n(s)</i> = normalized <i>(cross(Wu,Wv))</i> = the surface normal for value <i>s</i>
* <br><br>
* The image space shape of a three dimensional curve constraint (<tt>ds_solid_cstrn</tt> or
* <tt>ds_bound_cstrn</tt>) is always defined by the <tt>src_CW_pfunc</tt> callback in which case the
* <tt>parent_dmod</tt>, <tt>src_C_pfunc</tt>, and <tt>src_W_pfunc</tt> are all set to <tt>NULL</tt>. The calling
* program is required to supply the callback function. For <tt>ACIS</tt> applications the
* callback is already written and is called, <tt>DS_true_edge_eval</tt> and the <tt>src_data</tt>
* pointer is populated by the class <tt>DS_2acis</tt>. The callback function takes as input
* a curve parameter value, s, and returns both a domain point and an image point
* value. The domain point value has to be returned in the constrained deformable
* model's original domain space.
* <br><br>
* The constraint may act to constrain any combination of the shape's position,
* tangent, and curvature along the length of the curve constraint determined by
* the behavior bit array value. The behavior bits are named <tt>SPA_DM_POS_FIXED</tt>,
* <tt>DM_TAN_FIXED</tt>, and <tt>DM_CURVATURE</tt>. The surface tangent and curvature constraints
* act in the direction perpendicular to the curve. Combining <tt>DM_POSITN</tt> and
* DM_TANG behaviors allows seam constraints to be C1 continuous with their parents.
* Combining <tt>SPA_DM_POS_FIXED</tt>, <tt>SPA_DM_TAN_FIXED</tt>, and <tt>SPA_DM_CURVATURE</tt> behaviors allows seam
* constraints to be <tt>C1</tt> continuous with their parents. Combining <tt>SPA_DM_POSITN</tt>,
* <tt>SPA_DM_TANG</tt>, and <tt>SPA_DM_CURVATURE</tt> behaviors allows seam constraints to be <tt>C2</tt> continuous
* with their parents. To be <tt>C1</tt> continuous <tt>tang_gain</tt> must be <tt>1.0</tt>.
* <br><br>
* When the tag value is <tt>-1</tt> the system selects a unique tag number to give to the
* newly created curve constraint. Otherwise, it uses the given tag value, but
* never checks that tag value for uniqueness.
* <br><br>
* <b>Inherited behaviors:</b> Constraints carry with them a set of behavior states
* including position, tangent, deletable, and stoppable. The position and tangent
* states are set by the behavior input argument. A deletable constraint may be
* removed by an end user at run time. A stoppable constraint may be disabled by an
* end user at run time. The deletable and stoppable behaviors are given based on
* the src_type as shown below. Also included in the table is a summary of which
* kinds of constraints will use the callback function and the source data.

* <br><br>
* <table border="0" width="75%">
* <tr>
* <td width="20%" align="left"><tt>DS_CSTRN_SRC</tt></td>
* <td width="10%" align="left"><tt>deletable</tt></td>
* <td width="10%" align="left"><tt>stopable</tt></td>
* <td width="10%" align="left"><tt>Src_data</tt></td>
* <td width="10%" align="left"><tt>Src_CW_func</tt></td>
* </tr>
* <tr>
* <td width="20%" align="left"><tt>ds_solid_cstrn</tt></td>
* <td width="10%" align="left">NO</td>
* <td width="10%" align="left">NO</td>
* <td width="10%" align="left">YES</td>
* <td width="10%" align="left">NO</td>
* </tr>
* <tr>
* <td width="20%" align="left"><tt>ds_bound_cstrn</tt></td>
* <td width="10%" align="left">NO</td>
* <td width="10%" align="left">YES</td>
* <td width="10%" align="left">YES</td>
* <td width="10%" align="left">NO</td>
* </tr>
* <tr>
* <td width="20%" align="left"><tt>ds_user_cstrn</tt></td>
* <td width="10%" align="left">YES</td>
* <td width="10%" align="left">YES</td>
* <td width="10%" align="left">NULL</td>
* <td width="10%" align="left">YES</td>
* </tr>
* <tr>
* <td width="20%" align="left"><tt>ds_seam_cstrn</tt></td>
* <td width="10%" align="left">YES</td>
* <td width="10%" align="left">NO</td>
* <td width="10%" align="left">NO</td>
* <td width="10%" align="left">NO</td>
* </tr>
*</table>
*<br>
* <tt>ds_solid_cstrn = curve</tt> is from a solid model boundary and any number of faces
* may connect to the curve.
* <br>
* <tt>ds_bound_cstrn = curve </tt>is from a sheet model boundary and only the face being
* deformed connects to <tt>crv</tt>.
* <br>
* <tt>ds_user_cstrn = curve</tt> is from a user interface and not intended to represent a
* topology boundary from a <i>B</i>-rep model.
* <br>
* <tt>ds_seam_cstrn = curve</tt> is used to join <tt>2</tt> hierarchical faces in a deformable
* modeling hierarchy.
* <br><br>
* Returns a tag identifier for newly created curve constraint.
* <br><br>
* The basic flavor for using a constraint is:
* <br><br><ol>
* <li> Create and apply the constraint to a deformable model,
* <tt>(DM_add_pt_cstrn())</tt>,
* <tt>(DM_add_crv_cstrn())</tt>,
* <tt>(DM_add_link_cstrn())</tt>,
* <tt>(DM_add_area_cstrn())</tt>.</li>
* <li> Select which geometry properties are being constrained,
* <tt>(DM_set_cstrn_behavior())</tt>.</li>
* <li> For <tt>pt_cstrns</tt>, modify the geometry property values by moving the point
* constraint display points as described above,
* <tt>(DM_set_pt_xyz())</tt>.</li>
* <li> Call solve to see how the constraint modifications changed the
* deformable model shape,
* <tt>(DM_solve())</tt>.</li>
* <li> Optionally, for <tt>pt_cstrns</tt>, the point constraint's domain position and
* domain directions (for surfaces) may be modified with
* <tt>(DM_set_pt_uv())</tt>,
* <tt>(DM_set_cstrn_pttan_uv_dir())</tt>.</li>
* <li> Optionally, modify the constraint rendering and image_pt locations to
* optimize viewing and/or interactions,
* <tt>DM_set_tan_display_gain()</tt>
* <tt>DM_set_comb_graphics()</tt>.</li></ol>
* <br>
* <b>Errors:</b>
*<dl><tt>DM_parse_tag_flag()</tt> errors
*<dt><tt>DM_NO_ROOT_DMOD</tt></dt>
*<dd>The root deformable model cannot be <tt>NULL</tt> on input.</dd>
*<dt><tt>DM_BAD_SRC_CURVE_TYPE</tt></dt>
*<dd>The src_type must be one of <tt>ds_solid_cstrn</tt>, <tt>ds_bound_cstrn</tt>, <tt>ds_user_cstrn</tt>, or <tt>ds_seam_cstrn</tt>.</dd>
*<dt><tt>DS_MISSING_SRC_DATA</tt></dt>
*<dd>The curve's source type conflicts with the particular set of input values given.</dd>
*<dt><tt>DM_MIXED_CRV_CSTRN_DIM</tt></dt>
*<dd>The image_dim of <tt>src_C_pfunc</tt> must  equal the deformable model's <tt>domain_dim</tt>.</dd>
*<dt><tt>DM_BAD_CRV_CSTRN_DIM</tt></dt>
*<dd> The input <tt>src_C_pfunc</tt> must be a curve, i.e. its <tt>domain_dim</tt> must be <tt>1</tt>.</dd>
*<dt><tt>DM_BAD_DOMAIN_PT_RANGE</tt></dt>
*<dd> The domain point must be  completely contained by the deformable model.</dd>
* <tt>DM_BAD_SRC_PFUNC_MAPPING</tt><dt></dt>
*<dd>Whenever a src_pfunc does not  have a proper <tt>domain_dim</tt> or <tt>image_dim</tt> value. The <tt>domain_dim</tt>
* value for all <tt>src_pfuncs</tt> must be <tt>1</tt> (they are all curves), and the <tt>image_dim</tt> for the <tt>src_W</tt> and
* <tt>src_Wn DS_pfuncs</tt> must equal the  <tt>image_dim</tt> of the object being constrained, and the <tt>image_dim</tt> of
*  the src_Wnn <tt>DS_pfunc</tt> must be <tt>1</tt>.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: member of target model hierarchy.
* @param tag_flag
* in: specify tgt model 1 = active model 2 = root model else = member model with tag=tag_flag.
* @param parent_dmod
* opt: Seam's parent shape for hierarchies. [not-nested].
* @param domain_flag
* in:  0=src_C_pfunc in orig_dmod_space 2=src_C_pfunc in internal_pfunc_space.
* @param src_C_pfunc
* opt: uv domain curve description.[nested] input in orig_dmod_space.
* @param src_W_pfunc
* opt: cstrn pos shape, [nested].
* @param src_Wn_pfunc
* opt: cross-tang shape, [nested].
* @param src_Wnn_pfunc
* opt: curvature shape, [nested].
* @param src_CW_func
* opt: user given src_crv description, [pre-alloced].
* @param src_data
* in: src_data for eval, [pre-alloced].
* @param s
* in: curve s param value.
* @param C
* out: surface C=[u,v] point value.
* @param Cs
* out: surface dC/ds vec in surf for s.
* @param W
* out: image space W=[x,y,z] point val.
* @param Wu
* out: image space dW/du.
* @param Wv
* out: image space dW/dv.
* @param Wuu
* out: image space d2W/du2.
* @param Wuv
* out: image space d2W/dudv.
* @param Wvv
* out: image space d2W/dv2.
* @param dist2)
* out: dist**2 (xyz,proj_to_3d(uv)).
* @param src_data
* src_CW_func callback data.
* @param src_type
* in: records cstrn one of: ds_solid_cstrn= shared topo bndry ds_bound_cstrn= unshared topo bndry ds_user_cstrn = user created cstrn ds_seam_cstrn =join 2 hierarchical faces.
* @param behavior
* OR of: SPA_DM_POS_FIXED SPA_DM_TAN_FIXED SPA_DM_CURV_FIXED.
* @param tang_gain
* UNUSED UNUSED.
* @param tag
* in: when == -1 assign next tag number else use this tag number.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM int                // rtn: new tag id or negative err code
DM_add_crv_cstrn           // eff: adds crv_cstrn along DS-pfunc
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in:  member of target dmod hierarchy, [pre-alloced]
   int tag_flag,           // in:  specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
   DS_dmod *parent_dmod =  // opt: Shape's parent shape for seams, [pre-alloced]
      NULL,
   int domain_flag=0,      // in:  0=src_C_pfunc in orig_dmod_space
                           //      2=src_C_pfunc in internal_pfunc_space
   DS_pfunc *src_C_pfunc=  // opt: uv domain curve description, [nested]
                      NULL,//      input in orig_dmod_space.
   DS_pfunc *src_W_pfunc=  // opt: cstrn pos shape,  [nested]
                      NULL,
   DS_pfunc *src_Wn_pfunc= // opt: cross-tang shape, [nested]
                      NULL,
   DS_pfunc *src_Wnn_pfunc=// opt: curvature shape,  [nested]
                      NULL,

   SRC_CW_FUNC src_CW_func=// opt: user given src_crv description, [pre-alloced]
                      NULL,
   void *src_data = NULL,  // opt: passed to src_CW_func callback, [pass-thru]
   DS_CSTRN_SRC src_type=  // in:  records cstrn origin. oneof:
     ds_user_cstrn,        //  ds_solid_cstrn=shared topo bndry
                           //  ds_bound_cstrn=unshared topo bndry
                           //  ds_user_cstrn =user created cstrn
                           //  ds_seam_cstrn =join 2 hierarchical faces
   int behavior=           // in : orof SPA_DM_POS_FIXED  = surf pos constrnt
        SPA_DM_POS_FIXED,  //           SPA_DM_TAN_FIXED  = surf tan constrnt
                           //           SPA_DM_CURV_FIXED = surf curv cstrnt
   double tang_gain=1.0,   // in : vary tang constraint magnitude
   int tag=-1,             // in : when == -1 assign next tag number
                           //       else use this tag number
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod
/**
* Adds a distributed curve spring and returns a new tag identifier or negative error code.
* <br><br>
* <b>Role:</b> Modifies the target deformable model.
* <br><br>
* <tt>DS_CSTRN_SRC src_type</tt> must be one of the following values:

* <br><br>
* <table border="0" width="100%">
* <tr>
* <td width="20%" align="left"><tt>ds_solid_cstrn</tt></td>
* <td width="80%" align="left">shared topo boundary</td>
* </tr>
* <tr>
* <td width="20%" align="left"><tt>ds_bound_cstrn</tt></td>
* <td width="80%" align="left">unshared topo boundary</td>
* </tr>
* <tr>
* <td width="20%" align="left"><tt>ds_user_cstrn</tt></td>
* <td width="80%" align="left">user created constraint</td>
* </tr>
* <tr>
* <td width="20%" align="left"><tt>ds_seam_cstrn</tt></td>
* <td width="80%" align="left">join two hierarchical faces</td>
* </tr>
*</table><br>
* Builds and adds a curve load to the target deformable model using <tt>src_C_pfunc</tt>,
* <tt>free_crv</tt>, and the gain input arguments. A curve load is a distributed spring
* which connects a set of points on the deformable model to a set of points in
* three dimensional space. The deformable model points are specified by a domain
* space curve, <tt>C(s) = [u(s), v(s)]</tt> where s is the curve's parameter. The domain
* space curve may be specified in either the loaded deformable model's original
* domain space (orig_dmod_space) or in the internal <tt>pfunc</tt>'s domain space
* (<tt>internal_pfunc_space</tt>). Set <tt>domain_flag = 0</tt>, when <tt>src_C_pfunc</tt> is specified in
* orig_dmod_space and set <tt>domain_flag = 1</tt>, when src_C_pfunc is specified in
* internal_pfunc_space. When <tt>domain_flag == 0</tt>, the <tt>src_C_pfunc</tt> is scaled from the
* <tt>orig_dmod_space</tt> to the <tt>internal_pfunc_space</tt> and its pointer is stored internally.
* The <tt>src_C_Pfunc</tt> may not be described to this function in a <tt>unit_space</tt> domain
* range.
* <br><br>
* The free space points are specified by an image space curve,<tt> W(s) = [x(s), y(s),
* z(s)]</tt> where <i>s</i> is the same parameter used for the domain space curve. For every
* curve parameter value, <i>s</i>, there is a point-to-point spring between the domain
* curve point on the surface and the image curve point in free space. The net
* effect is a curve spring load or an elastic membrane stretched between the two
* curves. The two curves, <tt>Src_C_pfunc</tt> and <tt>free_crv</tt> should share a common domain
* range. When <tt>src_W_pfunc</tt>, is <tt>NULL</tt> on entry it will be generated by projecting the
* <tt>src_C_pfunc</tt> through the deformable model's shape into image space.
* <br><br>
* The image space of the <tt>Src_C_pfunc</tt> is the same as the domain_space of the loaded
* pfunc. This function does NOT scale the <tt>Src_C_pfunc</tt> from the unit square domain
* into the <tt>pfunc</tt>'s domain.
* <br><br>
* This function has two signatures. The first signature (with only eight
* arguments) is supplied for backward compatibility. The second signature allows
* the freedom in specifying target shape and properties to be loaded; the user can
* load position, tangent, or curvature. It is intentionally modeled on the
* signature of <tt>DM_add_crv_cstrn</tt>; see that routine's documentation for a discussion
* the argument list.
* <br><br>
* Even though multiple behaviors can be associated with one curve load, it is
* strongly recommended that a different curve load be added for each behavior to
* be controlled. Each behavior will then have a separate tag which can be used to
* separately tune how closely the curve is pulled to position tangency targets.
* <br><br>
* <tt>gain</tt> is the stiffness of the elastic membrane.
* <br><br>
* <tt>tag_flag</tt> selects the target deformable model. When tag equals <tt>-1</tt> the next
* available tag number is assigned to the newly created load. Otherwise, the input
* tag number is used. Other valid values for the tag flag are:
* <br><br>
* <table border="0" width="100%">
* <tr>
* <td width="3%" align="left"><tt>1</tt></td>
* <td width="97%" align="left">active deformable model</td>
* </tr>
* <tr>
* <td width="3%" align="left"><tt>2</tt></td>
* <td width="97%" align="left"> root deformable model</td>
* </tr>
*</table><br>
* Otherwise, the target is the deformable model whose tag identifier equals
* <tt>abs(tag_flag)</tt>
* <br><br>
* Returns a the tag number of the newly created load.
* <br><br>
* <b>Errors:</b>
*<dl><tt>DM_parse_tag_flag()</tt> errors
*<dt><tt>DM_BAD_TAG_VALUE</tt></dt>
*<dd>The input tag value must be -1 or positive.</dd>
*<dt><tt>DM_NO_ROOT_DMOD</tt></dt>
*<dd>The the input deformable model cannot be <tt>NULL</tt>.</dd>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The input src_C_pfunc is NULL on entry.</dd>
*<dt><tt>DM_MIXED_CRV_LOAD_DIM</tt></dt>
*<dd> The <tt>src_C_pfunc</tt>'s <tt>image_dim</tt> must be equal to deformable model's <tt>domain_dim</tt>.</dd>
*<dt><tt>DM_MIXED_FREE_LOAD_DIM</tt></dt>
*<dd>The <tt>src_W_pfunc</tt>'s image_dim must be equal to the deformable model's <tt>image_dim</tt>.</dd>
*<dt><tt>DM_BAD_DOMAIN_PT_RANGE</tt></dt>
*<dd>The domain point must be completely contained by the deformable model.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* rtn: new tag id or negative err code.
* @param dmod
* member of target dmod hierarchy.
* @param tag_flag
* specify tgt dmod 1 = active dmod 2 = root dmod else = member dmod with tag==tag_flag.
* @param parent_dmod
* shape's parent shape for seams.
* @param domain_flag
* 0=src_C_pfunc in orig_dmod_space 2=src_C_pfunc in int pfunc space.
* @param src_C_pfunc
* uv domain curve description input in orig_dmod_space.
* @param src_W_pfunc
* xyz image curve description.
* @param src_Wn_pfunc
* xyz image curve description.
* @param src_Wnn_pfunc
* xyz image curve description.
* @param src_CW_func
* opt: user given src_crv description.
* @param src_data
* in:  src_data for eval, [pre-alloced].
* @param s
* in:  curve s param value.
* @param C
* out: surface C=[u,v] pt. value.
* @param Cs
* out: surface dC/ds vec in surf for s.
* @param W
* out: image space W=[x,y,z] pt. val.
* @param Wu
* out: image space dW/du for s.
* @param Wv
* out: image space dW/dv for s.
* @param Wuu
* out: image space d2W/du2.
* @param Wuv
* out: image space d2W/dudv.
* @param Wvv
* out: image space d2W/dv2.
* @param dist2
* out: dist**2 (xyz,proj_to_3d (uv)).
* @param src_data
* opt: passed to src_CW_func callback.
* @param src_type
* in:  records cstrn origin.
* @param behavior
* OR of: SPA_DM_POS_FIXED = surf pos cnstrn SPA_DM_TAN_FIXED = surf tan cnstrn SPA_DM_CURV_FIXED = surf curv cnstrn.
* @param gain
* in:  stiffness connecting the 2 curves.
* @param tag
* in:  when == -1 assign next tag number, else use this tag number.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/

DECL_DM int                // rtn: new tag id or negative err code
DM_add_crv_load           // eff: adds crv_cstrn along DS-pfunc
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of target dmod hierarchy, [pre-alloced]
   int tag_flag,           // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
   DS_dmod *parent_dmod =  // opt: Shape's parent shape for seams, [pre-alloced]
      NULL,
   int domain_flag=0,      // in : 0=src_C_pfunc in orig_dmod_space
                           //      2=src_C_pfunc in internal_pfunc_space
   DS_pfunc *src_C_pfunc=  // opt: uv domain curve description, [nested]
                      NULL,//      input in orig_dmod_space.
   DS_pfunc *src_W_pfunc=  // opt: cstrn pos shape,  [nested]
                      NULL,
   DS_pfunc *src_Wn_pfunc= // opt: cross-tang shape, [nested]
                      NULL,
   DS_pfunc *src_Wnn_pfunc=// opt: curvature shape,  [nested]
                      NULL,
   SRC_CW_FUNC src_CW_func=// opt: user given src_crv description, [pre-alloced]
                      NULL,
   void *src_data = NULL,  // opt: passed to src_CW_func callback, [pass-thru]
   DS_CSTRN_SRC src_type=  // in : records cstrn origin. oneof:
     ds_user_cstrn,        //  ds_solid_cstrn=shared topo bndry
                           //  ds_bound_cstrn=unshared topo bndry
                           //  ds_user_cstrn =user created cstrn
                           //  ds_seam_cstrn =join 2 hierarchical faces
   int behavior=           // in : or of SPA_DM_POS_FIXED  = surf pos constrnt
        SPA_DM_POS_FIXED,  //           SPA_DM_TAN_FIXED  = surf tan constrnt
                           //           SPA_DM_CURV_FIXED = surf curv cstrnt
   double gain=0.0,        // in : vary tang constraint magnitude
   int tag=-1,             // in : when == -1 assign next tag number
                           //       else use this tag number
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod

/**
* Adds a link constraint to connect two root siblings to make two deformable models act as one.
* <br><br>
* <b>Role:</b> Builds and adds to the dmod1 hierarchy a link constraint that
* will force <tt>dmod1</tt> and <tt>dmod2</tt> to deform as if they were one, and for their shape to
* be <tt>C0</tt>, <tt>C1</tt> or discontinuous across this link constraint. The input <tt>dmod1</tt> and
* <tt>dmod2</tt> must both be at the root levels of their deformable modeling hierarchies.
* <br><br>
* When <tt>dmod1</tt> and <tt>dmod2</tt> come from different deformable modeling hierarchies, i.e.
* this is the first link constraint between any of the siblings of dmod1 and any
* of the siblings of <tt>dmod2</tt>, <tt>dmod2</tt> and all of its siblings are added to the sibling
* list of dmod1. At that time, all of the tag identifiers in <tt>dmod2</tt>'s deformable
* modeling hierarchy are shifted by the output amount, <tt>tag_shift</tt>, to guarantee
* their uniqueness in the combined deformable modeling hierarchy. So, within the
* dmod2 hierarchy, all tag numbers will be changed as <tt> after_tag_number =
* before_tag_number + tag_shift.</tt> When dmod1 and <tt>dmod2</tt> are already siblings,
* <tt>tag_shift</tt> is set to <tt>0</tt>, and all the tag numbers are not changed.
* <br><br>
* A calling application has to take care to update any data associated by tag
* values. For the <tt>ACIS</tt> integration, this means that all <tt>ATTRIB_DSMODEL</tt>,
* <tt>ATTRIB_CSTRN</tt>, and <tt>ATTRIB_LOAD</tt> derived data associated with the <tt>dmod2</tt> hierarchy
* has to be updated.
* <br><br>
* The <tt>link_constraint</tt> is described by the shapes of two curves embedded within the
* dmods (dmod1 and dmod2) that they constrain. These curves are called <tt>curve1</tt> and
* <tt>curve2</tt>. The image-spaces of curve1 and curve2 are the original domain-spaces of
* the DS_dmods that they constrain. Both curve1 and <tt>curve2</tt> must share a common
* parameterization, <i>s</i>. The link constraint forces the points and cross-tangents of
* common s values on curve1 and curve2 to coincide. Symbolically, we refer to
* these two curves as <tt>C1(s)</tt> and <tt>C2(s)</tt>.
* <br><br>
* The parameterization of curves the <tt>C1</tt> and <tt>C2</tt> must be controlled, or the link
* constraints will be artificially stiff. That is the link constraint won't deform
* as generally as one would like. For a link constraint to work well, the speed of
* the curves <tt>C0</tt> and <tt>C1</tt> must be matched, that is the derivatives, <i>dC1/ds</i> and <i>dC2/ds</i>
* must be related to each other as
*<pre>
* dC1(s)/ds = constant * dC2(s)/ds for all values of s.
*</pre>
* When the link constraint curves <tt>C1</tt> and <tt>C2</tt> are just straight lines in the domain
* spaces of surfaces <tt>dmod1</tt> and <tt>dmod2</tt>, a good strategy for ensuring this
* relationship is to construct a simple linear curve for <tt>C1</tt> and <tt>C2</tt> that span the
* end points of the curve. An example of constructing <tt>C1</tt> in this manner is the
* call <pre> DM_make_bspline_curve(rtn_err, 2, 1, 2, 2, knot_index, knot, dof_vec, NULL, 0) ;</br>
* Where: int knot_index = {0, 1} ; double knot = {0.0, 1.0} ; double dof_vec =
* {C0(s_min).u(), C0(s_min).v(), C)(s_max).u(), C0(s_max).v()} ;
*</pre>
* The parametric shape of the curves can be given in two ways: either explicitly,
* by passing <tt>DS_pfunc</tt> objects (the <tt>src1_C_pfunc</tt> and <tt>src2_C_pfunc</tt> input arguments),
* or procedurally, by passing a callback function (the src_CW_func along with the
* <tt>src1_data</tt> and <tt>src2_data</tt> input arguments) but not by both. That is, either both
* <tt>src1_C_pfunc</tt> and <tt>src2_C_pfunc</tt> are non-NULL or <tt>src_CW_func</tt> is non-NULL on input,
* but not both. To apply the link_constraint, point locations for curve1 and
* <tt>curve2</tt> in <i>uv</i> space and <i>xyz</i> space will be evaluated. These computations will be
* made by either evaluating <tt>src1_C_pfunc</tt> and <tt>src2_C_pfunc</tt> directly, or by making a
* pair of calls on the function, <tt>src_CW_func</tt>, once with a pointer to src1_data and
* once with a pointer to <tt>src2_data</tt>. For <tt>ACIS</tt> applications the callback function is
* already written and is called, <tt>DS_true_edge_eval</tt> and the <tt>src_data</tt> pointer is
* populated by the class <tt>DS_2acis</tt>. Its important to have the callback function or
* the <tt>src_C_pfunc</tt> objects generate <i>uv</i> points that are in the original domain space
* of the deformable models that they constrain <tt>(orig_dmod_space)</tt>. Generating
* domain locations in either the unit-space or the <tt>internal_pfunc_space</tt> will cause
* errors.
* <br><br>
* When <tt>src1_C_pfunc</tt> and <tt>src2_C_prfunc</tt> are supplied, the <tt>domain_flag</tt> value is used
* to determine in which space the curves are built. When <tt>domain_flag == 0</tt>, the
* <tt>src_C_pfuncs</tt>' <tt>image_spaces</tt> are equal to their respective <tt>orig_dmod_spaces</tt>. When
* <tt>domain_flag == 2</tt>, the <tt>src_C_pfuncs</tt>' <tt>image_space</tt> are equal to their respective
* <tt>internal_pfunc_spaces</tt>. When <tt>src_C_pfuncs</tt> are supplied in their <tt>orig_dmod_spaces</tt>,
* they are scaled by this function from the <tt>orig_dmod_space</tt> to the
* <tt>internal_pfunc_space</tt> and their pointers are stored internally.
* <br><br>
* The image space shape of the curve constraints will be computed either by
* projecting the curve's computed <i>uv</i> points into xyz space using the shape
* definitions for <tt>dmod1</tt> and <tt>dmod2</tt>, or by calling <tt>src_CW_func</tt> once for <tt>curve1</tt> and
* once for <tt>curve2</tt>.
* <br><br>
* When using the <tt>src_CW_func</tt> callback, applications must store enough additional
* information for <tt>curve1</tt> and <tt>curve2</tt> in <tt>src1_data</tt> and <tt>src2_data</tt> to be able to
* compute the curve's <i>uv</i> and xyz point positions given a parametric value, s. The
* DM Modeler never accesses or frees the <tt>src_data</tt> memory, but can store and
* retrieve it for the convenience of the application. These pointers are passed
* whenever any user written callback functions called.
* <br><br>
* The behavior of the two curve constraints within the link constraint, called
* <tt>curve1</tt> and <tt>curve2</tt>, are determined by the value of the behavior argument which is
* constructed as a combination of the following bit constants. The positions and
* the cross-tangents along both <tt>curve1</tt> and <tt>curve2</tt> may be constrained independently
* in one of three ways: <tt>FREE</tt>, <tt>FIXED</tt>, and <tt>LINKED</tt>.
* <br><br>
* Curve 1 Positions
*<br>
* <tt>SPA_DM_POS_FREE</tt><br>
* <tt>SPA_DM_POS_FIXED</tt><br>
* <tt>SPA_DM_POS_LINKED</tt>
* <br><br>
* Curve 1 Tangents
*<br>
* <tt>SPA_DM_TAN_FREE</tt><br>
* <tt>SPA_DM_TAN_FIXED</tt><br>
* <tt>SPA_DM_TAN_LINKED</tt>
* <br><br>
* Curve 2 Positions
*<br>
* <tt><;&bsp>SPA_DM_POS_2_FREE</tt><br>
* <tt>  SPA_DM_POS_2_FIXED</tt><br>
* <tt>SPA_DM_POS_2_LINKED</tt>
* <br><br>
* Curve 2 Tangents
*<br>
* <tt>SPA_DM_TAN_2_FREE</tt><br>
* <tt>SPA_DM_TAN_2_FIXED</tt><br>
* <tt>SPA_DM_TAN_2_LINKED</tt>
* <br><br>
* A <tt>FREE</tt> curve property is free to move and is not constrained. A <tt>FIXED</tt> curve
* property is frozen and will remain in its current shape during subsequent
* deformations. A <tt>LINKED</tt> curve is free to move as long as it stays connected to
* the constraint to which it is linked.
* <br><br>
* For example.
*<pre>
* behavior1 = SPA_DM_POS_FREE | SPA_DM_POS_2_FREE | SPA_DM_TAN_FREE | SPA_DM_TAN_2_FREE ;
* behavior2 = SPA_DM_POS_LINKED | SPA_DM_POS_2_LINKED | SPA_DM_TAN_LINKED | SPA_DM_TAN_2_LINKED ;
* behavior3 = SPA_DM_POS_FIXED | SPA_DM_POS_2_LINKED | SPA_DM_TAN_LINKED | SPA_DM_TAN_2_LINKED ;
* behavior4 = SPA_DM_POS_FREE | SPA_DM_POS_2_FREE | SPA_DM_TAN_LINKED | SPA_DM_TAN_2_LINKED ;
*</pre>
* behavior1 specifies a link constraint which is disabled, while behavior2
* specifies a <tt>C1</tt> link that is free to move in space, and behavior3 specifies a <tt>C1</tt>
* link that is fixed in space but allowed to rotate about its own axis, and
* behavior4 specifies a link which is discontinuous, but keeps the cross tangents
* on each side of the link the same.
* <br><br>
* Mixing a curve1 <tt>FREE</tt> constraint with a curve2 <tt>LINKED</tt> is taken to be equivalent
* to having both curves be <tt>LINKED</tt>. Mixing a curve1 <tt>FIXED</tt> constraint with a curve2
* <tt>LINKED</tt> constraint is taken to be equivalent to both curves being FIXED in the
* same position, which is different than having both curves <tt>FIXED</tt>, in which case
* the two curves can be <tt>FIXED</tt> in different positions.
* <br><br>
* When the input argument, tag, is <tt>-1</tt> the system assigns the newly constructed
* link a unique tag number, otherwise it assigns it the input tag value. It is the
* calling application's responsibility to guarantee that assigned tag numbers are
* unique throughout the entire deformable modeling hierarchy.
* <br><br>
* Returns the tag id for the newly created link constraint.
* <br><br>
* <b>Errors:</b>
* Sets <tt>rtn_err</tt> to 0 for success, else
* <br><br>
* <dl><tt>DM_parse_tag_flag()</tt> errors.
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>when dmod1, dmod2 are <tt>NULL</tt> on input</dd>.
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>when <tt>src1_C_pfunc</tt>, or <tt>src2_C_pfunc</tt> == <tt>NULL</tt> on input and <tt>src_WC_func</tt> is <tt>NULL</tt></dd>.
*<dt><tt>DM_MIXED_CRV_CSTRN_DIM</tt></dt>
* <dd>when the image_dim of
* <tt>src1_C_pfunc</tt> != the dmod1's <tt>domain_dim</tt> or
*src2_C_pfunc != the dmod2's <tt>domain_dim</tt></dd>.
*<dt><tt>DM_BAD_CRV_CSTRN_DIM</tt></dt>
*<dd>when either the <tt>src1_C_pfunc</tt> or <tt>src2_C_pfunc</tt> are not a curve, i.e. one of their  <tt>domain_dim</tt>'s != 1.</dd>
*<dt><tt>DM_BAD_DOMAIN_PT_RANGE</tt></dt>
*<dd>when <tt>src1_C_pfunc</tt> is not contained by the tag1 dmod or <tt>src2_C_pfunc</tt> is not contained by the tag2 dmod.</dd>
*<dt><tt>DM_BAD_TAG_VALUE</tt></dt>
*<dd>when the input tag number is not <tt>-1</tt> or larger than <tt>0</tt>.</dd>
*<dt><tt>DM_NONMONOTONIC_LINK</tt></dt>
*<dd>when the link <tt>dcrvs</tt>, <tt>C1(s)</tt> or <tt>C2(s)</tt> are not described monotonically, that is  <i>dC/ds</i> changes sign for some valid values of <i>s</i>.</dd>
*<dt><tt>DM_UNEVEN_LINK_SPEEDS</tt></dt>
*<dd>when the link_cstrn's dcrvs, C1( <i>s0</i> and C2(s), represented by either <tt>src1_C_pfunc</tt>
* and <tt>src2_C_pfunc</tt> or by the callback <tt>src_CW_func</tt>, do not have a constantly
* related speed. That is if the requirement that <i>dC1/ds</i> = constant * <i>dC2/ds</i> is not
* true for all values of <i>s</i>.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* rtn: new tag id or negative err code.
* @param tag_shift
* out: dmod2's after_tags = before_tags + tag_shift.
* @param dmod1
* specify 1st tgt dmod.
* @param dmod2
* specify 2nd tgt dmod.
* @param domain_flag
* in:  0=src_C_pfuncs in orig_dmod_space 2=src_C_pfuncs in internal_pfunc_space.
* @param src1_C_pfunc
* uv domain curve description [nested] input in orig_dmod_space.
* @param src2_C_pfunc
* xyz image curve description [nested] input in orig_dmod_space.
* @param src1_W_pfunc
* opt: dmod1 cstrn pos shape, [nested].
* @param src1_Wn_pfunc
* opt: dmod1 cross-tang  shape, [nested].
* @param src1_Wnn_pfunc
* opt: dmod1 curvature  shape, [nested].
* @param src2_W_pfunc
* opt: dmod2 cstrn pos  shape, [nested].
* @param src2_Wn_pfunc
* opt: dmod2 cross-tang  shape, [nested].
* @param src2_Wnn_pfunc
* opt: dmod2 curvature  shape, [nested].
* @param src1_data
* application data stored for tgt1_dmod.
* @param src2_data
* application data stored for tgt2_dmod.
* @param src_data
* function to update.
* @param s
* in: curve s param value.
* @param C
* out: surface C=[u,v] point value.
* @param Cs
* out: surface dC/ds vec in surf for s.
* @param W
* out: image space W=[x,y,z] point val.
* @param Wu
* out: image space dW/du for s.
* @param Wv
* out: image space dW/dv for s tangent for s.
* @param Wuu
* out: image space d2W/du2.
* @param Wuv
* out: image space d2W/dudv.
* @param Wvv
* out: image space d2W/dv2.
* @param dist2
* out: dist**2 (xyz,proj_to_3d(uv)).
* @param behavior
* in: OR of SPA_DM_POS_FREE SPA_DM_POS_FIXED SPA_DM_POS_LINKED SPA_DM_POS_2_FREE SPA_DM_POS_2_FIXED SPA_DM_POS_2_LINKED SPA_DM_TAN_FREE SPA_DM_TAN_FIXED SPA_DM_TAN_LINKED SPA_DM_TAN_2_FREE SPA_DM_TAN_2_FIXED SPA_DM_TAN_2_LINKED.
* @param tang_gain
* UNUSED UNUSED.
* @param tag
* when = -1 assign next tag number else use this tag number.
* @param sdmo
* in:SDM_options pointer.
**/

DECL_DM int                // rtn: new tag id or negative err code
DM_add_link_cstrn          // eff: adds link_cstrn between DS_pfuncs
  (int &rtn_err,           // out: 0=success or neg err code
   int &tag_shift,         // out: dmod2's after_tags = before_tags + tag_shift
   DS_dmod *dmod1,         // in : specify 1st tgt dmod, [pre-alloced]
   DS_dmod *dmod2,         // in : specify 2nd tgt dmod, [pre-alloced]
   int domain_flag,        // in : 0=src_C_pfunc in orig_dmod_space
                           //      2=src_C_pfunc in internal_pfunc_space
   DS_pfunc *src1_C_pfunc, // in : uv domain curve1 description, [nested]
                           //      input in orig_dmod_space
   DS_pfunc *src2_C_pfunc, // in : uv domain curve2 description, [nested]
                           //      input in orig_dmod_space
   DS_pfunc *src1_W_pfunc=  // opt: dmod1 cstrn pos shape,   [nested]
                       NULL,
   DS_pfunc *src1_Wn_pfunc= // opt: dmod1 cross-tang shape, [nested]
                       NULL,
   DS_pfunc *src1_Wnn_pfunc=// opt: dmod1 curvature shape,  [nested]
                       NULL,
   DS_pfunc *src2_W_pfunc=  // opt: dmod2 cstrn pos shape,  [nested]
                       NULL,
   DS_pfunc *src2_Wn_pfunc= // opt: dmod2 cross-tang shape, [nested]
                       NULL,
   DS_pfunc *src2_Wnn_pfunc=// opt: dmod2 curvature shape,  [nested]
                       NULL,
   void *src1_data = NULL, // opt: application data stored for tgt1_dmod, [pass-thru]
   void *src2_data = NULL, // opt: application data stored for tgt2_dmod, [pass-thru]
   SRC_CW_FUNC src_data = NULL,  // opt: user given src_crv description, [pre-alloced]
   int behavior=           // in : orof SPA_DM_POS_FREE,      SPA_DM_POS_2_FREE,
     SPA_DM_POS_LINKED |       //       SPA_DM_POS_FIXED,     SPA_DM_POS_2_FIXED,
     SPA_DM_TAN_FREE   |       //       SPA_DM_POS_LINKED,    SPA_DM_POS_2_LINKED,
     SPA_DM_POS_2_LINKED |     //
     SPA_DM_TAN_2_FREE,        //       SPA_DM_TAN_FREE       SPA_DM_TAN_2_FREE
                           //           SPA_DM_TAN_FIXED,     SPA_DM_TAN_2_FIXED,
                           //           SPA_DM_TAN_LINKED,    SPA_DM_TAN_2_LINKED
                           //
                           //           SPA_DM_CURV_2_FREE,   SPA_DM_CURV_2_FREE
                           //           SPA_DM_CURV_2_FIXED,  SPA_DM_CURV_2_FIXED,
                           //           SPA_DM_CURV_2_LINKED, SPA_DM_CURV_2_LINKED
   double tang_gain=1.0,   // in : vary tang constraint magnitude
   int tag=-1,             // in : when == -1 assign next tag number
                           //       else use this tag number
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod
/**
* Adds a link constraint to connect two root siblings to make two deformable models act as one.
* <br><br>
* <b>Role:</b> Builds and adds to the <tt>dmod1</tt> hierarchy a link that will cause
* dmod1 and <tt>dmod2</tt> to deform simultaneously, and pulls them together with a spring
* load towards <tt>C0</tt> continuity across this link. The input <tt>dmod1</tt> and <tt>dmod2</tt> must both
* be at the root levels of their deformable modeling hierarchies.
* <br><br>
* The difference between using this function and <tt>DM_add_link_cstrn()</tt> is in how the
* link constraint is enforced. This function uses a spring load (which is
* equivalent to a penalty method) for enforcing the constraint, while the
* <tt>DM_add_link_cstrn()</tt> call enforces the link link constraint with lag range
* variables. The difference is that the penalty method will tend to be more
* tolerant, which can be an advantage if the constraint is being enforced too
* stiffly to bend properly with the lag range constraint approach.
* <br><br>
* See the documentation for <tt>DM_add_link_cstrn()</tt> for a description of all the input
* argument uses.
* <br><br>
* Returns the tag id for the newly created link constraint
* <br><br>
* <table border="0" width="100%">
* <tr>
* <td width="20%" align="left"><tt>DM_unflipped</tt> </td>
* <td width="80%" align="left">same handedness</td>
* </tr>
* <tr>
* <td width="10%" align="left"><tt>DM_flipped</tt></td>
* <td width="90%" align="left">opposite handedness</td>
* </tr>
* <tr>
* <td width="10%" align="left"><tt> DM_flip_unknown</tt></td>
* <td width="90%" align="left">unspecified by user, package will guess</td>
* </tr>
*</table>
*<br>
* <b>Errors:</b>
* Sets <tt>rtn_err</tt> to 0 for success, else
* <br><br>
* <dl><tt>DM_parse_tag_flag()</tt> errors
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>when <tt>dmod1</tt>, <tt>dmod2</tt> are <tt>NULL</tt> on input</dd>.
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>when <tt>src1_C_pfunc</tt>, or <tt>src2_C_pfunc</tt> == <tt>NULL</tt> on input and <tt>src_WC_func</tt> is <tt>NULL</tt></dd>.
*<dt><tt>DM_MIXED_CRV_CSTRN_DIM</tt></dt>
*<dd> when the image_dim of <tt>src1_C_pfunc</tt> != the dmod1's <tt>domain_dim</tt> or <tt>src2_C_pfunc</tt> != the dmod2's <tt>domain_dim</tt></dd>.
*<dt><tt>DM_BAD_CRV_CSTRN_DIM</tt></dt>
*<dd>when either the <tt>src1_C_pfunc</tt> or <tt>src2_C_pfunc</tt> are not a curve, i.e. one of their  <tt>domain_dim</tt>'s != 1.</dd>
*<dt><tt>DM_BAD_DOMAIN_PT_RANGE</tt></dt>
*<dd> when <tt>src1_C_pfunc</tt> is not contained by the tag1 dmod or <tt>src2_C_pfunc</tt> is not contained by the tag2 dmod.</dd>
*<dt><tt>DM_BAD_TAG_VALUE</tt></dt>
*<dd>when the input tag number is not -1 or larger than 0.</dd>
*<dt><tt>DM_NONMONOTONIC_LINK</tt></dt>
*<dd>when the link dcrvs, <tt>C1(s)</tt> or <tt>C2(s)</tt> are not described monotonically, that is <i>dC/ds</i> changes sign for some valid values of <i>s</i>.</dd>
*<dt><tt>DM_UNEVEN_LINK_SPEEDS</tt></dt>
*<dd>when the <tt>link_cstrn</tt>'s dcrvs, <tt>C1(s0)</tt> and <tt>C2(s)</tt>, represented by either <tt>src1_C_pfunc</tt> and <tt>src2_C_pfunc</tt> or by the callback <tt>src_CW_func</tt>, do not have a constantly
* related speed. That is if the requirement that dC1/ds = constant * <tt>dC2/ds</tt> is not
* true for all values of <i>s</i>.</dd>
*<dt><tt>DM_BAD_SRC_PFUNC_MAPPING</tt></dt>
* <dd>whenever a <tt>src_pfunc</tt> does not have a proper domain_dim or <tt>image_dim</tt> value. The
* <tt>domain_dim</tt> value for all <tt>src_pfuncs</tt> must be <tt>1</tt> (they are all curves), and the
* image_dim for the <tt>src_W</tt> and <tt>src_Wn</tt> <tt>DS_pfuncs</tt> must equal the <tt>image_dim</tt> of the
*object being constrained, and the <tt>image_dim</tt> of the <tt>src_Wnn</tt> <tt>DS_pfunc</tt> must be <tt>1</tt>.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param tag_shift
* out: dmod2's after_tags = before_tags + tag_shift.
* @param dmod1
* in: specify 1st tgt dmod, [pre-alloced].
* @param dmod2
* in: specify 2nd tgt dmod, [pre-alloced].
* @param domain_flag
* in: 0=src_C_pfunc in orig_dmod_space 2=src_C_pfunc in internal_pfunc_space.
* @param src1_C_pfunc
* in: 0=src_C_pfunc in orig_dmod_space 2=src_C_pfunc in internal_pfunc_space.
* @param src2_C_pfunc
* in:  uv domain curve2 description, [nested] input in orig_dmod_space.
* @param src1_W_pfunc
* opt: dmod1 cstrn pos shape, [nested].
* @param src1_Wn_pfunc
* opt: dmod1 cross-tang shape, [nested].
* @param src1_Wnn_pfunc
* opt: dmod1 curvature shape, [nested].
* @param src2_W_pfunc
* opt: dmod2 curvature shape, [nested].
* @param src2_Wn_pfunc
* opt: dmod2 cross-tang shape, [nested].
* @param src2_Wnn_pfunc
* opt: dmod2 curvature shape, [nested].
* @param src1_data
* opt: application data stored for tgt1_dmod, [pass-thru].
* @param src2_data
* opt: application data stored for tgt2_dmod, [pass-thru].
* @param src_CW_func
* opt: user given.
* @param src_data
* src_crv description, [pre-alloced], with ACIS use: [DS_true_edge_eval] in: src_data for eval, [pre-alloced].
* @param s
* in:  curve s param value, [pre-alloced].
* @param C
* out: surface C=[u,v] point value, [pre-alloced].
* @param Cs
* out: surface dC/ds vec in surf for s, [pre-alloced].
* @param W
* out: image space W=[x,y,z] point val, [pre-alloced].
* @param Wu
* out: image space dW/du for s, [pre-alloced].
* @param Wv
* out: image space dW/dv for s, [pre-alloced].
* @param Wuu
* out: image space d2W/du2.
* @param Wuv
* out: image space d2W/dudv.
* @param Wvv
* out: image space d2W/dv2.
* @param dist2
* out: dist**2 (xyz,proj_to_3d(uv)).
* @param gain
* in:  gain for load behavior.
* @param tag
* in:  when == -1 assign next tag number else use this tag number. note: sets active dmod.
* @param flipped_state
* in: indicates relative handedness of two surfaces being linked. sets active dmod.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM int                // rtn: new tag id or negative err code
DM_add_link_C0_load        // eff: adds SPAposition load on link between DS_pfuncs
  (int &rtn_err,           // out: 0=success or neg err code
   int &tag_shift,         // out: dmod2's after_tags = before_tags + tag_shift
   DS_dmod *dmod1,         // in : specify 1st tgt dmod, [pre-alloced]
   DS_dmod *dmod2,         // in : specify 2nd tgt dmod, [pre-alloced]
   int domain_flag,        // in : 0=src_C_pfunc in orig_dmod_space
                           //      2=src_C_pfunc in internal_pfunc_space
   DS_pfunc *src1_C_pfunc, // in : uv domain curve1 description, [nested]
                           //      input in orig_dmod_space
   DS_pfunc *src2_C_pfunc, // in : uv domain curve2 description, [nested]
                           //      input in orig_dmod_space
   DS_pfunc *src1_W_pfunc=  // opt: dmod1 cstrn pos shape,   [nested]
                       NULL,
   DS_pfunc *src1_Wn_pfunc= // opt: dmod1 cross-tang shape, [nested]
                       NULL,
   DS_pfunc *src1_Wnn_pfunc=// opt: dmod1 curvature shape,  [nested]
                       NULL,
   DS_pfunc *src2_W_pfunc=  // opt: dmod2 cstrn pos shape,  [nested]
                       NULL,
   DS_pfunc *src2_Wn_pfunc= // opt: dmod2 cross-tang shape, [nested]
                       NULL,
   DS_pfunc *src2_Wnn_pfunc=// opt: dmod2 curvature shape,  [nested]
                       NULL,
   void *src1_data = NULL, // opt: application data stored for tgt1_dmod, [pass-thru]
   void *src2_data = NULL, // opt: application data stored for tgt2_dmod, [pass-thru]
   SRC_CW_FUNC src_CW_func=// opt: user given src_crv description, [pre-alloced]
                      NULL,
   double gain=10000000.,  // in : gain for load behavior
   int tag=-1,             // in : when == -1 assign next tag number
                           //       else use this tag number
   SPA_DM_flipped_state
      flipped_state = SPA_DM_flip_unknown,  // in : indicates relative handedness of two
                                        //     surfaces being linked
                                        //     SPA_DM_unflipped    ==> same handedness;
                                        //     SPA_DM_flipped      ==> opposite handedness;
                                        //     SPA_DM_flip_unknown ==> unspecified by user;
                                        //                           package will guess
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod
/**
* Adds a link constraint to connect two root siblings to make two deformable models act as one.
* <br><br>
* <b>Role:</b> Builds and adds to the dmod1 hierarchy a link that will cause
* <tt>dmod1</tt> and <tt>dmod2</tt> to deform simultaneously, and pulls them together with a spring
* load towards rotated <tt>C1</tt> continuity across this link. The input <tt>dmod1</tt> and <tt>dmod2</tt>
* must both be at the root levels of their deformable modeling hierarchies. <tt>C1</tt> as
* used here means tangent continuity only; position (<tt>C0</tt>) continuity is unaffected
* by this load.
* <br><br>
* Rotated <tt>C1</tt> continuity is a generalization of <tt>C1</tt> continuity. It is designed to
* account for differences in the parameterizations of the two surfaces. To enforce
* rotated <tt>C1</tt> continuity, the surfaces are independently examined at every point
* along the link to determine the "cross-tangent derivative operator" at that
* point. The cross-tangent derivative operator is defined as the directional
* derivative (in <i>uv</i> space) for which the surface tangent vector is of unit length
* and perpendicular (inward) to the edge. This cross-tangent derivative operator
* is defined when the <tt>C1</tt> behavior is turned on, and depends on the surface shape
* at the time it is defined. Locally rotated <tt>C1</tt> is satisfied when the
* cross-tangent derivatives of the two surfaces are equal and opposite.
* <br><br>
* The difference between using this function and <tt>DM_add_link_cstrn()</tt> is in how the
* link constraint is enforced. This function uses a spring load (which is
* equivalent to a penalty method) for enforcing the constraint, while the
* <tt>DM_add_link_cstrn()</tt> call enforces the link link constraint with lag range
* variables. The difference is that the penalty method will tend to be more
* tolerant, which can be an advantage if the constraint is being enforced too
* stiffly to bend properly with the lag range constraint approach. In addition,
* link constraints are unable to enforce rotated <tt>C1</tt> continuity; they enforce
* strict <tt>C1</tt> continuity instead.
* <br><br>
* Refer to the documentation for <tt>DM_add_link_cstrn()</tt> for a description of all the
* input argument uses.
* <br><br>
* Returns the tag id for the newly created link constraint.
* <br><br>
* The handedness can be:
* <br><br>
* <table border="0" width="100%">
* <tr>
* <td width="20%" align="left"><tt>DM_unflipped</tt> </td>
* <td width="80%" align="left">same handedness</td>
* </tr>
* <tr>
* <td width="10%" align="left"><tt>DM_flipped</tt></td>
* <td width="90%" align="left">opposite handedness</td>
* </tr>
* <tr>
* <td width="10%" align="left"><tt> DM_flip_unknown</tt></td>
* <td width="90%" align="left">unspecified by user, package will guess</td>
* </tr>
*</table>
*<br>
* <b>Errors:</b> Sets <tt>rtn_err</tt> to <tt>0</tt> for success, else
*<br><br>
* <dl><tt>DM_parse_tag_flag()</tt> errors
*<dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>When <tt>dmod1</tt>, <tt>dmod2</tt> are <tt>NULL</tt> on input</dd>.
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>When <tt>src1_C_pfunc</tt>, or <tt>src2_C_pfunc</tt> == <tt>NULL</tt> on input and <tt>src_WC_func</tt> is <tt>NULL</tt></dd>.
*<dt><tt>DM_MIXED_CRV_CSTRN_DIM</tt></dt>
*<dd>When the <tt>image_dim</tt> of <tt>src1_C_pfunc</tt> != the dmod1's <tt>domain_dim</tt> or <tt>src2_C_pfunc</tt> != the dmod2's <tt>domain_dim</tt></dd>.
*<dt><tt>DM_BAD_CRV_CSTRN_DIM</tt></dt>
*<dd>When either the <tt>src1_C_pfunc</tt> or <tt>src2_C_pfunc</tt> are not a curve, i.e. one of their <tt>domain_dim</tt>'s != 1.</dd>
*<dt><tt>DM_BAD_DOMAIN_PT_RANGE</tt></dt>
*<dd>When <tt>src1_C_pfunc</tt> is not contained by the tag1 dmod or <tt>src2_C_pfunc</tt> is not contained by the tag2 dmod.</dd>
*<dt><tt>DM_BAD_TAG_VALUE</tt></dt>
*<dd>When the input tag number is not <tt>-1</tt> or larger than <tt>0</tt>.</dd>
*<dt><tt>DM_NONMONOTONIC_LINK</tt></dt>
*<dd>When the link dcrvs, <tt>C1(s)</tt> or <tt>C2(s)</tt> are not described monotonically, that is <i>dC/ds</i> changes sign for some valid values of <i>s</i>.</dd>
*<dt><tt>DM_BAD_SRC_PFUNC_MAPPING</tt></dt>
* <dd>Whenever a <tt>src_pfunc</tt> does not have a proper domain_dim or <tt>image_dim</tt> value. The
* domain_dim value for all <tt>src_pfuncs</tt> must be <tt>1</tt> (they are all curves), and the
* image_dim for the <tt>src_W</tt> and <tt>src_Wn</tt> <tt>DS_pfuncs</tt> must equal the <tt>image_dim</tt> of the
*object being constrained, and the <tt>image_dim</tt> of the <tt>src_Wnn</tt> <tt>DS_pfunc</tt> must be <tt>1</tt>.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* rtn: new tag id or negative err code.
* @param tag_shift
* out: dmod2's after_tags = before_tags + tag_shift.
* @param dmod1
* specify 1st tgt dmod.
* @param dmod2
* specify 2nd tgt dmod.
* @param domain_flag
* in:  0=src_C_pfuncs in orig_dmod_space 2=src_C_pfuncs in internal_pfunc_space.
* @param src1_C_pfunc
* uv domain curve description [nested] input in orig_dmod_space.
* @param src2_C_pfunc
* uv domain curve description [nested] input in orig_dmod_space.
* @param src1_W_pfunc
* opt: cstrn pos shape for dmod1, [nested].
* @param src1_Wn_pfunc
* opt: cross-tang shape for dmod1, [nested].
* @param src1_Wnn_pfunc
* opt: curvature shape for dmod1, [nested].
* @param src2_W_pfunc
* opt: cstrn pos shape for dmod2, [nested].
* @param src2_Wn_pfunc
* opt: cross-tang shape for dmod2, [nested].
* @param src2_Wnn_pfunc
* opt: curvature shape for dmod2, [nested].
* @param src1_data
* application data stored for tgt1_dmod.
* @param src2_data
* application data stored for tgt2_dmod.
* @param src_CW_func
* opt: user given src_crv description, [pre-alloced] with ACIS use:[DS_true_edge_eval].
* @param src_data
* function to update.
* @param s
* in: curve s param value.
* @param C
* out: surface C=[u,v] point value.
* @param Cs
* out: surface dC/ds vec in surf for s.
* @param W
* out: image space W=[x,y,z] point val.
* @param Wu
* out: image space dW/du.
* @param Wv
* out: image space dW/dv.
* @param Wuu
* out: image space d2W/du2.
* @param Wuv
* out: image space d2W/dudv.
* @param Wvv
* out: image space d2W/dv2.
* @param dist2
* out: dist**2 (xyz,proj_to_3d(uv)).
* @param gain
* strength of load.
* @param tag
* when = -1 assign next tag number else use this tag number.
* @param flipped_state
* in: indicates relative handedness of two surfaces being linked. sets active dmod.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM int                // rtn: new tag id or negative err code
DM_add_link_C1_load        // eff: adds tangent load on link between DS_pfuncs
  (int &rtn_err,           // out: 0=success or neg err code
   int &tag_shift,         // out: dmod2's after_tags = before_tags + tag_shift
   DS_dmod *dmod1,         // in : specify 1st tgt dmod, [pre-alloced]
   DS_dmod *dmod2,         // in : specify 2nd tgt dmod, [pre-alloced]
   int domain_flag,        // in : 0=src_C_pfunc in orig_dmod_space
                           //      2=src_C_pfunc in internal_pfunc_space
   DS_pfunc *src1_C_pfunc, // in : uv domain curve1 description, [nested]
                           //      input in orig_dmod_space
   DS_pfunc *src2_C_pfunc, // in : uv domain curve2 description, [nested]
                           //      input in orig_dmod_space
   DS_pfunc *src1_W_pfunc=  // opt: dmod1 cstrn pos shape,   [nested]
                       NULL,
   DS_pfunc *src1_Wn_pfunc= // opt: dmod1 cross-tang shape, [nested]
                       NULL,
   DS_pfunc *src1_Wnn_pfunc=// opt: dmod1 curvature shape,  [nested]
                       NULL,
   DS_pfunc *src2_W_pfunc=  // opt: dmod2 cstrn pos shape,  [nested]
                       NULL,
   DS_pfunc *src2_Wn_pfunc= // opt: dmod2 cross-tang shape, [nested]
                       NULL,
   DS_pfunc *src2_Wnn_pfunc=// opt: dmod2 curvature shape,  [nested]
                       NULL,
   void *src1_data = NULL, // opt: application data stored for tgt1_dmod, [pass-thru]
   void *src2_data = NULL, // opt: application data stored for tgt2_dmod, [pass-thru]
   SRC_CW_FUNC src_CW_func=// opt: user given src_crv description, [pre-alloced]
                      NULL,
   double gain=10000000.,  // in : gain for load behavior
   int tag=-1,             // in : when == -1 assign next tag number
                           //       else use this tag number
   SPA_DM_flipped_state
      flipped_state = SPA_DM_flip_unknown,  // in : indicates relative handedness of two
                                        //     surfaces being linked
                                        //     SPA_DM_unflipped    ==> same handedness;
                                        //     SPA_DM_flipped      ==> opposite handedness;
                                        //     SPA_DM_flip_unknown ==> unspecified by user;
                                        //                           package will guess
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod
/**
* Applies an area constraint that fixes a subarea of a deformable model.
* <br><br>
* <b>Role:</b> Builds and adds to the target deformable model an area
* constraint and returns the integer tag value for that constraint. An area
* constraint implements an isolated deformation by partitioning the shape of the
* deformable model into two pieces, a piece that is fixed and a piece that can be
* deformed.
* <br><br>
* The partitioned area is represented by a <tt>DS_zone</tt> structure. A zone can be
* created by calling the function <tt>DM_build_square_zone()</tt>. The zone marks off a
* region of the surface's domain.
* <br><br>
* The input pointer, dmod, is a pointer to any deformable model within the
* deformable modeling hierarchy. The input <tt>tag_flag</tt>, specifies which deformable
* model in the hierarchy is the target for the area constraint.
* <br><br>
* When <tt>tag_flag = 1</tt>, the target is the active deformable model. When <tt>tag_flag = 2</tt>,
* the target is the root sibling deformable model. Else the target is the
* deformable model whose tag id equals the <tt>tag_flag</tt> value.
* <br><br>
* The input <tt>zone_flag</tt> value specifies which of the two shape partitions defined by
* the input zone is fixed.
* <br><br>
* When <tt>zone_flag = 0</tt>, the zone exterior is fixed. When <tt>zone_flag = 1</tt>, the zone's
* interior is fixed. See the comments under Limitations on element locking.
* <br><br>
* The input <tt>src_data</tt>, is for the convenience of the application so that the
* application can store whatever additional information with the constraint that
* it desires. The deformable modeling package never accesses this pointer. If an
* application uses this pointer, the application has to free the memory associated
* with it prior to deleting the area constraint to prevent a memory leak.
* <br><br>
* When the tag value is <tt>-1</tt>, the deformable modeling library assigns a unique tag
* value to the newly constructed constraint, otherwise the input tag value is
* assigned to the newly constructed area constraint. Applications need to ensure
* that assigned tag values are unique to the entire deformable modeling hierarchy
* that contains the area constraint.
* <br><br>
* <b>Errors:</b>
* Sets <tt>rtn_err</tt> to 0 for success, else
*<br><br>
*<dl><tt>DM_parse_tag_flag()</tt> errors
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>when model or zone is  NULL on input.</dd>
*<dt><tt>DM_DMOD_MISSING_PFUNC</tt></dt>
*<dd>when model does not contain a valid <tt>DS_pfunc</tt> object.</dd>
*<dt><tt>DM_MIXED_AREA_CSTRN_DIM</tt></dt>
*<dd>when the domain_dim of zone != the model's domain_dim.</dd>
*<dt><tt>DM_BAD_ZONE_FLAG_VALUE</tt></dt>
*<dd>when zone_flag is not one of 0=zone area moves or 1=zone area fixed.</dd>
*<dt><tt>DM_BAD_TAG_VALUE</tt></dt>
*<dd>when the input tag number is not -1 or larger than 0.</dd></dl>
* <br>
* <b>Limitations:</b> Only rectangular zones can be used to build an area constraint.
* With <tt>NUBS</tt> or <tt>NURBS</tt>, area constraints will lock, or fix, areas corresponding to
* entire elements; an element is a rectangle bounded by adjacent break-point (i.e.,
* knot) parameter values. The actual area fixed corresponds to the minimal set of
* elements which contains the zone-fixed subdomain. A rule of thumb is that the
* zone subdomain fixes the elements it touches. When the <tt>zone_flag</tt> is <tt>1</tt>, all
* elements touched by the open interior of the zone subdomain are fixed. When the
* zone_flag is <tt>0</tt>, all elements touched by the open exterior of the zone subdomain
* are fixed. (Open means we exclude the boundary.) For example, consider an area
* constraint with its zone_flag 0, on a cubic <tt>NUBS</tt> or <tt>NURBS</tt>. In this case, the
* zone exterior is fixed, and the zone interior must contain at least a <tt>4</tt> by <tt>4</tt>
* array of elements to be able to deform. Typically we add zone constraints to
* cubic <tt>NURBS</tt> or <tt>NURBS</tt> which have more than <tt>10</tt> knots in each parameter.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: member of target dmod hierarchy, pre-allocated.
* @param tag_flag
* in:  specify tgt dmod 1 = active dmod 2 = root dmod else = member dmod with tag==tag_flag.
* @param zone_flag
* in: 0=zone area moves zone compliment fixed 1 =zone area fixed zone compliment moves.
* @param zone
* in: defines constraint partition, [nested].
* @param src_data
* opt: app data stored with area_cstrn, [pass-thru].
* @param tag
* opt: when = -1 assign next tag number else use this tag number.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM int                // rtn: new tag id or negative err code
DM_add_area_cstrn          // eff: adds area_cstrn to DS_pfunc
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of target dmod hierarchy, [pre-alloced]
   int tag_flag,           // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
   int zone_flag,          // in : 0=zone area moves
                           //        zone compliment fixed
                           //      1=zone area fixed
                           //        zone compliment moves
   DS_zone *zone,          // in : defines constraint partition, [nested]
   void *src_data = NULL,  // opt: app data stored with area_cstrn, [pass-thru]
   int tag=-1,             // opt: when = -1 assign next tag number
                           //      else use this tag number
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod
//Till here...
//====================================================
//  DM_find_cstrn_by_tag(), DM_get_cstrn()
//====================================================
/**
* Checks the entire hierarchy for a constraint with matching tag value.
* <br><br>
* <b>Role:</b> Searches every constraint in every deformable model in the
* hierarchy for a constraint with a matching tag. If found, this function returns
* the pointer to the constraint and updates the active deformable model pointer
* value. If the constraint is not a link constraint, <tt>patch_tag1</tt> is set with the
* tag identifier of the deformable model containing the identified constraint and
* <tt>patch_tag2</tt> is set to <tt>-1</tt>. When the constraint is a link constraint, <tt>patch_tag1</tt>
* and <tt>patch_tag2</tt> are set to the two deformable models linked by the constraint.
* <br><br>
* When a constraint with a matching tag identifier is not found, <tt>patch_tag1</tt> and
* <tt>patch_tag2</tt> are both set to <tt>-1</tt> and <tt>NULL</tt> is returned.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>The queried deformable modeling hierarchy does not contain a deformable model, a
* load, or a constraint with a tag value that matches the input tag value.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: member of dmod hierarchy to search.
* @param tag
* in: unique tag identifier to match.
* @param patch_tag1
* out: containing patch tag or err code.
* @param patch_tag2
* out: containing patch tag or err code.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM DS_cstrn *         // rtn: cstrn with matching tag value, [nested]
DM_find_cstrn_by_tag       // eff: search entire hierarchy for cstrn
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy to search, [pre-alloced]
   int tag,                // in : unique tag identifier to match
   int &patch_tag1,        // out: containing patch tag or -1
   int &patch_tag2,        // out: linked patch tag or -1
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets the active patch

/**
* Loads constraint data into return arguments and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> This function looks for the constraint identified by a tag in
* the entire patch hierarchy. When tag specifies a constraint, this function fills
* all the output arguments appropriately and returns <tt>0</tt>. It changes the active
* deformable model to the one containing the constraint. Otherwise, it returns
* <tt>DM_TAG_OBJECT_ NOT_FOUND</tt> and leaves output arguments unmodified.
* <br><br>
* Link constraints, used for multi-surface deformations, represent two different
* constraint shapes which are connected together by the constraint. Link
* constraints place the information about their second constraint in the arguments:
* <tt>patch2_tag</tt>, <tt>shape2</tt>, <tt>src2_data</tt>, <tt>uv2_pts_count</tt>, and <tt>uv2_pts</tt>. When the identified
* constraint is not a link constraint, the function sets these arguments to
* unusable values as:
* <br><br>
* <table border="0" width="100%">
* <tr>
* <td width="20%" align="left"><tt>patch2_tag</tt> </td>
* <td width="80%" align="left">-1</td>
* </tr>
* <tr>
* <td width="10%" align="left"><tt>shape2</tt></td>
* <td width="90%" align="left">	"none"</td>
* </tr>
* <tr>
* <td width="10%" align="left"><tt> src2_data</tt></td>
* <td width="90%" align="left">NULL</td>
* </tr>
* <tr>
* <td width="10%" align="left"><tt> uv2_pts_count</tt></td>
* <td width="90%" align="left">0</td>
* </tr>
*</table>
*<br><br>
* The shape will be one of "point", "straight", "parabola", "circ", "curve" or
* "area". Depending on the shape type, some number of the <tt>uv_pts</tt> are set. The
* <tt>uv_pts</tt> define the shape of the constraint in the domain of the deformable
* model's shape.
* <br><br>
* When <tt>domain_flag</tt> is set to <tt>1</tt>, the <i>uv</i> domain points are returned specified in the
* unit-domain (values range from <tt>0</tt> to <tt>1</tt>). When <tt>domain_flag</tt> is set to 0, the <i>uv</i>
* domain points are returned specified in the original dmod's domain space. When
* <tt>domain_flag</tt> is set to <tt>2</tt>, the <i>uv</i> domain points are returned in the
* <tt>internal_pfunc_space</tt>.
* <br><br>
* For "point" shapes, one <tt>uv_pt</tt> is set for the constraint's location in the
* surface domain.
* <br><br>
* For "straight" shapes, two <tt>uv_pts</tt> are set for the end point locations of the
* straight constraint.
* <br><br>
* For "parabola" shapes, three <tt>uv_pts</tt> are set for the first end point, the
* intersection point between the two end tangents, and the last end point of the
* parabola constraint.
* <br><br>
* For "circ" shapes, three <tt>uv_pts</tt> are set for the center point, and two points on
* the circumference of the ellipse constraint.
* <br><br>
* For "curve" shapes, no <tt>uv_pts</tt> are set.
* <br><br>
* For "area" shapes, two <tt>uv_pts</tt> are set for the constraint's upper and lower
* boundaries. For releases <tt>5.3</tt> and <tt>6.0</tt> the only supported area constraint is a
* rectangle.
* <br><br>
* The behavior argument is a bit array composed as an or of the following bit
* constants:
* <br><br>
* <table border="0" width="100%">
* <tr>
* <td width="50%" align="left"><tt>SPA_DM_POS_FREE</tt> </td>
* <td width="50%" align="left"><tt>SPA_DM_POS_2_FREE</tt></td>
* </tr>
* <tr>
* <td width="50%" align="left"><tt>SPA_DM_POS_FIXED</tt></td>
* <td width="50%" align="left"><tt>SPA_DM_POS_2_FIXED</tt></td>
* </tr>
* <tr>
* <td width="50%" align="left"><tt>SPA_DM_POS_LINKED</tt></td>
* <td width="50%" align="left"><tt>SPA_DM_POS_2_LINKED</tt></td>
* </tr>
*</table>
*<br>
* <table border="0" width="100%">
* <tr>
* <td width="50%" align="left"><tt>SPA_DM_TAN_FREE</tt> </td>
* <td width="50%" align="left"><tt>SPA_DM_TAN_2_FREE</tt></td>
* </tr>
* <tr>
* <td width="50%" align="left"><tt>SPA_DM_TAN_FIXED</tt></td>
* <td width="50%" align="left"><tt>SPA_DM_TAN_2_FIXED</tt></td>
* </tr>
* <tr>
* <td width="50%" align="left"><tt>SPA_DM_TAN_LINKED</tt></td>
* <td width="50%" align="left"><tt>SPA_DM_TAN_2_LINKED</tt></td>
* </tr>
*</table>
*<br>
* <table border="0" width="100%">
* <tr>
* <td width="50%" align="left"><tt>SPA_DM_CURV_FREE</tt> </td>
* <td width="50%" align="left"><tt>SPA_DM_CURV_2_FREE</tt></td>
* </tr>
* <tr>
* <td width="50%" align="left"><tt>SPA_DM_CURV_FIXED</tt></td>
* <td width="50%" align="left"><tt>SPA_DM_CURV_2_FIXED</tt></td>
* </tr>
* <tr>
* <td width="50%" align="left"><tt>SPA_DM_CURV_LINKED</tt></td>
* <td width="50%" align="left"><tt>SPA_DM_CURV_2_LINKED</tt></td>
* </tr>
*</table>
*<br>
*<table border="0" width="100%">
* <tr>
* <td width="50%" align="left"><tt>SPA_DM_NORM_FIXED</tt> </td>
* <td width="50%" align="left"><tt>SPA_DM_BINORM_FIXED</tt></td>
* </tr>
*</table>
*<br>
* <tt>ntgrl_degree</tt> is an internal term used by deformable modeling to control the
* accuracy of building the deformable modeling equations. It is the max_degree
* polynomial function that is accurately integrated by the package. If the value
* is too small the system will not deform as expected. If the value is too large
* the system slows down. The term is returned for debugging and informative
* purposes only, and its value should always be at least twice the degree value.
* The maximum value is <tt>79</tt>.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>The input tag cannot identify a deformable model, a load, a spring, a spring set,
* an attractor object, or a constraint in the model hierarchy.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of target dmod hierarchy.
* @param tag
* in: constraint identifier.
* @param type_id
* out: cstrn type_id DS_tag_pt_cstrn DS_tag_pt_seam DS_tag_crv_cstrn DS_tag_crv_seam DS_tag_link_cstrn DS_tag_area_cstrn DS_tag_link_load DS_tag_crv_load.
* @param src_type
* out: one of ds_solid_cstrn ds_bound_cstrn ds_user_cstrn ds_seam_cstrn ds_link_cstrn.
* @param patch1_tag
* out: containing patch tag.
* @param patch2_tag
* out: containing patch tag.
* @param shape1
* out: shape type, sized:[20].
* @param shape2
* out: shape type, sized:[20].
* @param behavior
* out: orof SPA_DM_POS_FREE SPA_DM_POS_2_FREE SPA_DM_POS_FIXED SPA_DM_POS_2_FIXED SPA_DM_POS_LINKED SPA_DM_POS_2_LINKED SPA_DM_TAN_FREE SPA_DM_TAN_2_FREE SPA_DM_TAN_FIXED SPA_DM_TAN_2_FIXED SPA_DM_TAN_LINKED SPA_DM_TAN_2_LINKED SPA_DM_CURV_FREE SPA_DM_CURV_2_FREE vDM_CURV_FIXED SPA_DM_CURV_2_FIXED SPA_DM_CURV_LINKED SPA_DM_CURV_2_LINKED SPA_DM_CURV_ONLY_FIXED SPA_DM_CURV_2_ONLY_FIXED SPA_DM_CURV_ONLY_LINKED SPA_DM_CURV_2_ONLY_LINKED SPA_DM_NORM_FIXED SPA_DM_BINORM_FIXED.
* @param state
* out: 0=disabled, 1=enabled.
* @param rights
* out: orof SPA_DM_STOPABLE,SPA_ DM_DELETABLE.
* @param src1_data
* out: src_data stored with cstrn.
* @param src2_data
* out: link_cstrn 2nd src_data.
* @param domain_flag
* orig_dmod_space 1=domain_pts in unit_space. 2=domain_pts in internal_pfunc_space..
* @param uv1_pts_count
* out: number of pts loaded into uv1_pts.
* @param uv2_pts_count
* out: number of pts loaded into uv2_pts.
* @param uv1_pts
* out: defining locs for simple shapes ordered [u0,v0,u1,v1,u2,v2].
* @param uv2_pts
* out: defining locs for simple shapes ordered [u0,v0,u1,v1,u2,v2].
* @param tang_gain
* out: UNUSED.
* @param ntgrl_degree
* out: crv_cstrn numerical integration accuracy.
* @param sdmo
* in:SDM_options pointer note: sets the active patch.
**/
DECL_DM void
DM_get_cstrn               // eff: load cstrn data into rtn args
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy to search, [pre-alloced]
   int tag,                // in : tag id for constraint to query
   DS_TAGS &type_id,       // out: cstrn type id
   DS_CSTRN_SRC &src_type, // out: ds_solid_cstrn: from solid model
                           //      ds_bound_cstrn: from sheet model
                           //      ds_user_cstrn:  from end user
                           //      ds_seam_cstrn:  from patch hierarcy
                           //      ds_link_cstrn:  for multi-surf
   int &patch1_tag,        // out: containing patch tag
   int &patch2_tag,        // out: link_cstrn 2nd patch tag
   char *shape1,           // out: shape type,       sized:[20], [pre-alloced]
   char *shape2,           // out: link_cstrn 2nd shape type, sized:[20], [pre-alloced]
   int &behavior,          // out: orof SPA_DM_POS_FREE    SPA_DM_POS_2_FREE
                           //           SPA_DM_POS_FIXED   SPA_DM_POS_2_FIXED
                           //           SPA_DM_POS_LINKED  SPA_DM_POS_2_LINKED
                           //
                           //           SPA_DM_TAN_FREE    SPA_DM_TAN_2_FREE
                           //           SPA_DM_TAN_FIXED   SPA_DM_TAN_2_FIXED
                           //           SPA_DM_TAN_LINKED  SPA_DM_TAN_2_LINKED
                           //
                           //           SPA_DM_CURV_FREE    SPA_DM_CURV_2_FREE
                           //           SPA_DM_CURV_FIXED   SPA_DM_CURV_2_FIXED
                           //           SPA_DM_CURV_LINKED  SPA_DM_CURV_2_LINKED
                           //
                           //           SPA_DM_CURV_ONLY_FIXED   SPA_DM_CURV_2_ONLY_FIXED
                           //           SPA_DM_CURV_ONLY_LINKED  SPA_DM_CURV_2_ONLY_LINKED
                           //
                           //           SPA_DM_NORM_FIXED   SPA_DM_BINORM_FIXED
   int &state,             // out: 0=disabled,1=enabled
   int &rights,            // out: orof DM_STOPABLE,DM_DELETABLE
   void *&src1_data,       // out: src_data stored with cstrn, [pass-thru]
   void *&src2_data,       // out: link_cstrn 2nd src_data, [pass-thru]
   int domain_flag,        // in : 0=uv_pts in orig_dmod_space,
                           //      1=uv_pts in unit_space,
                           //      2=uv_pts in internal_pfunc_space.
   int &uv1_pts_count,     // out: number of pts loaded into uv1_pts
   int &uv2_pts_count,     // out: number of pts loaded into uv2_pts
   double uv1_pts[6],      // out: defining locs for simple shapes
                           //      ordered[u0,v0,u1,v1,u2,v2]
   double uv2_pts[6],      // out: link_cstrn 2nd shape array
   double &tang_gain,      // out: magnitude scaling for crv_cstrn tangent
   int &ntgrl_degree,      // out: crv_cstrn numerical integration accuracy
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets the active patch

//====================================================
//  DM_get/set_cstrn behavior, state, rights, tangent_gain, end_conds
//====================================================
/**
* Gets the tight state for a constraint/load.
* <br><br>
* <b>Role:</b> This function queries the tag tight behavior bit. It returns <tt>1</tt>
* when tight is enabled, and returns <tt>0</tt> when tight is disabled. The <tt>rtn_err</tt>
* indicates success (<tt>0</tt>) or a negative error code. The dmod is the member of the
* deformable model hierarchy to be searched. The tag is a constraint identifier
* which sets the active deformable model to the owner of the tag.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
* <dd>The input tag cannot identify a deformable model, a
*load, a spring, a spring set, an attractor object, or a constraint in the model hierarchy.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* success or error code.
* @param dmod
* member of deformable model to search.
* @param tag
* tag identifier.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM int                        // rtn: 1=tight enabled,0=tight disabled
DM_get_tight_state         // eff: get on/off tight state
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of target dmod hierarchy to search
   int tag,                // in : cstrn identifier
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod
/**
* Modifies the tight enabled/disabled state.
* <br><br>
* <b>Role:</b> This function searches the deformable model hierarchy (using
* dmod) for the constraint with the given input tag value, and modifies that
* constraint's enabled/disabled behavior bit based on <tt>state_flag</tt>. The possible
* values for <tt>state_flag</tt> are:
*<br><br>
* <table border="0" width="50%">
* <tr>
* <td width="10%" align="left"><tt>-1</tt> </td>
* <td width="20%" align="left">toggle current state</td>
* </tr>
* <tr>
* <td width="10%" align="left"><tt>0</tt></td>
* <td width="20%" align="left">set to off (turn tight off)</td>
* </tr>
* <tr>
* <td width="10%" align="left"><tt> 1</tt></td>
* <td width="20%" align="left">set to on</td>
* </tr>
*</table>
*<br>
* This function passes the change request down to the deformable model so that all
* affected arrays can be updated. It also sets the active deformable model. The
* <tt>rtn_err</tt> indicates either success (if equal to <tt>0</tt>) or error (a negative error
* code).
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
* <dd>The input tag cannot identify a deformable model, a load, a spring, a spring set,
*an attractor object, or a constraint in the model hierarchy.</dd>
*<dt><tt>DM_BAD_STATE_FLAG_VALUE</tt></dt>
*<dd>The <tt>state_flag</tt> must be equal to <tt>-1</tt>, <tt>0</tt>, or <tt>1</tt>.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* success or error code.
* @param dmod
* root of deformable model hierarchy tree.
* @param tag
* unique tag identifier to match.
* @param state_flag
* how to set tight state.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM void
DM_set_tight_state         // eff: change tight state
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of target dmod hierarchy to search
   int tag,                // in : cstrn identifier
   int state_flag,         // in : -1=toggle current tight state,
                           //       0=turn tight off
                           //       1=turn tight on
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod

/**
* Gets the constraint's pos/tang behavior and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies behavior.
* <br><br>
* This function searches the deformable model hierarchy for a constraint with a
* given input tag value and loads the behavior with that constraint's behavior
* bits, which can be an or of the following bit constants:
* <br><br>
* <table border="0" width="100%">
* <tr>
* <td width="50%" align="left"><tt>DM_POS_FREE</tt> </td>
* <td width="50%" align="left"><tt>DM_POS_2_FREE</tt></td>
* </tr>
* <tr>
* <td width="50%" align="left"><tt>DM_POS_FIXED</tt></td>
* <td width="50%" align="left"><tt>DM_POS_2_FIXED</tt></td>
* </tr>
* <tr>
* <td width="50%" align="left"><tt> DM_POS_LINKED</tt></td>
* <td width="50%" align="left"><tt>DM_POS_2_LINKED</tt></td>
* </tr>
*</table>
*<br>
* <table border="0" width="100%">
* <tr>
* <td width="50%" align="left"><tt>DM_TAN_FREE</tt> </td>
* <td width="50%" align="left"><tt>DM_TAN_2_FREE</tt></td>
* </tr>
* <tr>
* <td width="50%" align="left"><tt>DM_TAN_FIXED</tt></td>
* <td width="50%" align="left"><tt>DM_TAN_2_FIXED</tt></td>
* </tr>
* <tr>
* <td width="50%" align="left"><tt> DM_TAN_LINKED</tt></td>
* <td width="50%" align="left"><tt>DM_TAN_2_LINKED</tt></td>
* </tr>
*</table>
*<br>
* <table border="0" width="100%">
* <tr>
* <td width="50%" align="left"><tt>DM_CURV_FREE</tt> </td>
* <td width="50%" align="left"><tt>DM_CURV_2_FREE</tt></td>
* </tr>
* <tr>
* <td width="50%" align="left"><tt>DM_CURV_FIXED</tt></td>
* <td width="50%" align="left"><tt>DM_CURV_2_FIXED</tt></td>
* </tr>
* <tr>
* <td width="50%" align="left"><tt> DM_CURV_LINKED</tt></td>
* <td width="50%" align="left"><tt>DM_CURV_2_LINKED</tt></td>
* </tr>
*</table>
*<br>
*<table border="0" width="100%">
* <tr>
* <td width="50%" align="left"><tt>DM_NORM_FIXED</tt> </td>
* <td width="50%" align="left"><tt>DM_BINORM_FIXED</tt></td>
* </tr>
*</table>
*<br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
* <dd>The input tag cannot identify a deformable model, a load, a spring, a spring set,
*an attractor object, or a constraint in the model hierarchy.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of dmod hierarchy to search.
* @param tag
* in: cstrn identifier.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM int                // rtn: orof SPA_DM_POS_FREE    SPA_DM_POS_2_FREE
                           //           SPA_DM_POS_FIXED   SPA_DM_POS_2_FIXED
                           //           SPA_DM_POS_LINKED  SPA_DM_POS_2_LINKED
                           //
                           //           SPA_DM_TAN_FREE    SPA_DM_TAN_2_FREE
                           //           SPA_DM_TAN_FIXED   SPA_DM_TAN_2_FIXED
                           //           SPA_DM_TAN_LINKED  SPA_DM_TAN_2_LINKED
                           //
                           //           SPA_DM_CURV_FREE   SPA_DM_CURV_2_FREE
                           //           SPA_DM_CURV_FIXED  SPA_DM_CURV_2_FIXED
                           //           SPA_DM_CURV_LINKED SPA_DM_CURV_2_LINKED
                           //
                           //           SPA_DM_CURV_ONLY_FIXED   SPA_DM_CURV_2_ONLY_FIXED
                           //           SPA_DM_CURV_ONLY_LINKED  SPA_DM_CURV_2_ONLY_LINKED
                           //
                           //           SPA_DM_NORM_FIXED  SPA_DM_BINORM_FIXED
DM_get_cstrn_behavior      // eff: get cstrn's pos/tang behavior
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of target dmod hierarchy, [pre-alloced]
   int tag,                // in : cstrn identifier
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod

/**
* Modifies the behavior of a constraint and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies the tag's behavior bit array and sets the active
* deformable model. This function passes the change request down to the deformable
* model so that all affected arrays can be updated. The behavior argument is a bit
* array composed as an OR of the following bit constants:
* <br><br>
* <table border="0" width="100%">
* <tr>
* <td width="50%" align="left"><tt>SPA_DM_POS_FREE</tt> </td>
* <td width="50%" align="left"><tt>SPA_DM_POS_2_FREE</tt></td>
* </tr>
* <tr>
* <td width="50%" align="left"><tt>SPA_DM_POS_FIXED</tt></td>
* <td width="50%" align="left"><tt>SPA_DM_POS_2_FIXED</tt></td>
* </tr>
* <tr>
* <td width="50%" align="left"><tt>SPA_DM_POS_LINKED</tt></td>
* <td width="50%" align="left"><tt>SPA_DM_POS_2_LINKED</tt></td>
* </tr>
*</table>
*<br>
* <table border="0" width="100%">
* <tr>
* <td width="50%" align="left"><tt>SPA_DM_TAN_FREE</tt> </td>
* <td width="50%" align="left"><tt>SPA_DM_TAN_2_FREE</tt></td>
* </tr>
* <tr>
* <td width="50%" align="left"><tt>SPA_DM_TAN_FIXED</tt></td>
* <td width="50%" align="left"><tt>SPA_DM_TAN_2_FIXED</tt></td>
* </tr>
* <tr>
* <td width="50%" align="left"><tt>SPA_DM_TAN_LINKED</tt></td>
* <td width="50%" align="left"><tt>SPA_DM_TAN_2_LINKED</tt></td>
* </tr>
*</table>
*<br>
*<table border="0" width="100%">
* <tr>
* <td width="50%" align="left"><tt>SPA_DM_NORM_FIXED</tt> </td>
* <td width="50%" align="left"><tt>SPA_DM_BINORM_FIXED</tt></td>
* </tr>
*</table>
*<br>
* The basic flavor for using a constraint is:
* <br><br><ol>
* <li> Create and apply the constraint to a deformable model,
* <tt>(DM_add_pt_cstrn())</tt>,
* <tt>(DM_add_crv_cstrn())</tt>,
* <tt>(DM_add_link_cstrn())</tt>,
* <tt>(DM_add_area_cstrn())</tt>.</li>
* <li> Select which geometry properties are being constrained,
* <tt>(DM_set_cstrn_behavior())</tt>.</li>
* <li> For <tt>pt_cstrns</tt>, modify the geometry property values by moving the point
* constraint display points as described above,
* <tt>(DM_set_pt_xyz())</tt>.</li>
* <li> Call solve to see how the constraint modifications changed the
* deformable model shape,
* <tt>(DM_solve())</tt>.</li>
* <li> Optionally, for pt_cstrns, the point constraint's domain position and
* domain directions (for surfaces) may be modified with
* <tt>(DM_set_pt_uv())</tt>,
* <tt>(DM_set_cstrn_pttan_uv_dir())</tt>.</li>
*<li> Optionally, modify the constraint rendering and <tt>image_pt</tt> locations to
* optimize viewing and/or interactions,
* <tt>DM_set_tan_display_gain()</tt>,
* <tt>DM_set_comb_graphics()</tt>.</li></ol>
*<br><br>
* Not all the behavior bits are used by all the constraints. Behavior bits not
* used by an object are ignored by that object. Point constraints on curves do not
* use the <tt>..._LINKED</tt> or the <tt>..._2_...</tt> behaviors. Point constraints on surfaces do
* not use the <tt>..._LINKED</tt> or the <tt>DM_BINORM_FIXED</tt> bits. Curve constraints do not use
* the <tt>DM_NORM_FIXED</tt> or the <tt>DM_BINORM_FIXED</tt> bits.
* <br><br>
* Point constraints couple some of the constraint behaviors together. Turning on
* the following behavior causes other behaviors to be turned on as follows:
* <br><br>
* <tt>DM_CURV_FIXED</tt> turns on <tt>DM_TAN_FIXED</tt> and <tt>DM_NORM_FIXED</tt>.
* <br><br>
* <tt>DM_CURV_2_FIXED</tt> turns on <tt>DM_TAN_2_FIXED</tt> and <tt>DM_NORM_FIXED</tt>.
* <br><br>
* For curves <tt>DM_NORM_FIXED</tt> turns on <tt>DM_TAN_FIXED</tt>.
* <br><br>
* Turning off a cascaded behavior without turning off the behavior which causes it
* to be turned on won't work. The behavior state will remain unmodified.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>The input tag cannot identify a constraint in the model hierarchy.</dd>
*<dt><tt>DM_BAD_CSTRN_CHANGE</tt></dt>
* <dd>The identified constraint's state prohibits toggling of <tt>DM_POSITN</tt> behavior.
*This would be for unstoppable constraints.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of dmod hierarchy tree.
* @param tag
* in: unique tag identifier to match.
* @param behavior
* in: or of SPA_DM_POS_FREE SPA_DM_POS_FIXED SPA_DM_POS_LINKED SPA_DM_POS_2_FREE SPA_DM_POS_2_FIXED SPA_DM_POS_2_LINKED SPA_DM_TAN_FREE SPA_DM_TAN_FIXED SPA_DM_TAN_LINKED SPA_DM_TAN_2_FREE SPA_DM_TAN_2_FIXED SPA_DM_TAN_2_LINKED SPA_DM_CURV_FREE SPA_DM_CURV_FIXED SPA_DM_CURV_LINKED SPA_DM_CURV_2_FREE SPA_DM_CURV_2_FIXED SPA_DM_CURV_2_LINKED SPA_DM_NORM_FIXED SPA_DM_BINORM_FIXED.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM void
DM_set_cstrn_behavior      // eff: change behavior for a cstrn
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy to search, [pre-alloced]
   int tag,                // in : cstrn identifier
   int behavior,           // in : orof SPA_DM_POS_FREE    SPA_DM_POS_2_FREE
                           //           SPA_DM_POS_FIXED   SPA_DM_POS_2_FIXED
                           //           SPA_DM_POS_LINKED  SPA_DM_POS_2_LINKED
                           //
                           //           SPA_DM_TAN_FREE    SPA_DM_TAN_2_FREE
                           //           SPA_DM_TAN_FIXED   SPA_DM_TAN_2_FIXED
                           //           SPA_DM_TAN_LINKED  SPA_DM_TAN_2_LINKED
                           //
                           //           SPA_DM_CURV_FREE   SPA_DM_CURV_2_FREE
                           //           SPA_DM_CURV_FIXED  SPA_DM_CURV_2_FIXED
                           //           SPA_DM_CURV_LINKED SPA_DM_CURV_2_LINKED
                           //
                           //           SPA_DM_CURV_ONLY_FIXED   SPA_DM_CURV_2_ONLY_FIXED
                           //           SPA_DM_CURV_ONLY_LINKED  SPA_DM_CURV_2_ONLY_LINKED
                           //
                           //           SPA_DM_NORM_FIXED  SPA_DM_BINORM_FIXED
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod

/**
* Sets the surface domain space direction for a point constraint's tangent and curvature constraint behaviors.
* <br><br>
* <b>Role:</b> Sets the domain space direction on a surface in which to
* evaluate a point tangent or curvature constraint. The direction is a unit vector
* in the <tt>"domain_dir"</tt> direction. For deformable surfaces, two domain space
* directions can be specified at a point; the <tt>which_dir</tt> argument selects between
* them. This function along with <tt>DM_set_pt_xyz()</tt> and <tt>DM_set_cstrn_behavior()</tt> gives
* complete run time control of point tangent and curvature constraints.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>Neither the deformable model nor the domain_dir can be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>The input tag cannot identify a constraint in the model hierarchy.</dd>
*<dt><tt>DM_BAD_WHICH_DIR_VALUE</tt></dt>
* <dd>The input which_dir value was not one of <tt>DM_TANG1_LEG</tt>, <tt>DM_TANG2_LEG</tt>,
*<tt>DM_CURV1_LEG</tt>, or <tt>DM_CURV2_LEG</tt>.</dd>
*<dt><tt>DM_ZERO_LENGTH_DOMAIN_DIR</tt></dt>
*<dd>When input domain direction vector has a zero length.</dd>
*<dt><tt>DM_PARALLEL_DOMAIN_DIRS</tt></dt>
*<dd>When the input domain directions is parallel to the other domain direction.</dd>
*<dt><tt>DM_NOT_A_PT_CSTRN</tt></dt>
*<dd>The constraint identified by the input tag value was not a point constraint.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of dmod hierarchy tree.
* @param tag
* in: point constraint identifier.
* @param domain_dir
* in: uv direction in which to evaluate tangent vector.
* @param which_dir
* in: which uv direction is being specified one of: DM_TANG1_LEG DM_TANG2_LEG DM_CURV1_LEG DM_CURV2_LEG.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM void
DM_set_cstrn_pttan_uv_dir  // eff: Set uv dir for point tangent & curvature constraints
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of target dmod hierarchy to search, [pre-alloced]
   int tag,                // in : point cstrn identifier
   double *domain_dir,     // in : uv direction for directional derivative (tangent)
                           //      sized:[DM_get_domain_dim(rtn_err, dmod)]
   int which_dir,          // in : oneof DM_TANG1_LEG
                           //            DM_TANG2_LEG
                           //            DM_CURV1_LEG
                           //            DM_CURV2_LEG
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod

/**
* Gets the state for a constraint and returns a <tt>0</tt> = success or an error.
* <br><br>
* <b>Role:</b> Queries the tag constraint enabled/disabled behavior bit.
* <br><br>
* Returns <tt>1</tt> when the constraint is enabled, returns <tt>0</tt> when the constraint is
* disabled.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
* <dd>The input tag cannot identify a deformable model, a load, a spring, a spring set,
*an attractor object, or a constraint in the model hierarchy.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of dmod hierarchy to search.
* @param tag
* in: cstrn identifier.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM int                // rtn: 1=cstrn enabled,0=cstrn disabled
DM_get_cstrn_state         // eff: get on/off state for a cstrn
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of target dmod hierarchy, [pre-alloced]
   int tag,                // in : cstrn identifier
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod
/**
* Modifies the enabled/disabled state for a deformable modeling constraint.
* <br><br>
* <b>Role:</b> Searches the deformable model hierarchy for the constraint with
* a given input tag value and modifies that constraint's enabled/disabled behavior
* bit and sets the active deformable model.
* <br><br>
* Passes the change request down to the deformable model so that all affected
* arrays can be updated.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
* <dd>The input tag cannot identify a deformable model, a load, a spring, a spring set,
*an attractor object, or a constraint in the model hierarchy.</dd>
*<dt><tt>DM_BAD_CSTRN_CHANGE</tt></dt>
*<dd>The <tt>crv_cstrn</tt> prohibits varying its curvature gain.</dd>
*<dt><tt>DM_BAD_STATE_FLAG_VALUE</tt></dt>
*<dd>The <tt>state_flag</tt> must be equal to <tt>-1</tt>, <tt>0</tt>, or <tt>1</tt>.</dd>
*<dt><tt>DM_BAD_DOMAIN_PT_RANGE</tt></dt>
*<dd>The domain point must be completely contained by the deformable model's pfunc.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: root of hierarchy tree.
* @param tag
* in: unique tag identifier to match.
* @param state_flag
* in: -1= toggle current cstrn state, 0 = turn cstrn off 1 = turn cstrn on.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM void
DM_set_cstrn_state         // eff: change state for a cstrn
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of target dmod hierarchy to search, [pre-alloced]
   int tag,                // in : cstrn identifier
   int state_flag,         // in : -1=toggle current cstrn state,
                           //       0=turn cstrn off,
                           //       1=turn cstrn on
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod

/**
* Gets a constraint's deletable/stoppable rights.
* <br><br>
* <b>Role:</b> Modifies rights.
* <br><br>
* Searches the deformable model hierarchy for the constraint with a given input
* tag value and loads that constraint's rights as bits, which can be one or both
* of <tt>SPA_DM_DELETABLE</tt> and/or <tt>SPA_DM_STOPABLE</tt>. Constraints which are <tt>SPA_DM_DELETABLE</tt> may be
* removed from the deformable model hierarchy with a call to DM_rm_tag_object().
* Constraints which are <tt>DM_STOPABLE</tt> may be toggled on and off by calls to
* <tt>DM_set_cstrn_state()</tt> and may have their <tt>SPA_DM_POS_FIXED</tt> behavior bit disabled with
* a call to <tt>DM_set_cstrn_behavior()</tt>. Constraints without the appropriate rights
* will not be acted upon by these functions.
* <br><br>
* Constraint's of different types are created with different rights (see
* <tt>DM_add_crv_()</tt>). These rights can not be changed at run time because the
* deformable modeling deformable model hierarchy depends on the continued
* existence of unstoppable and undeletable constraints. Control a constraint's
* rights by selecting the correct <tt>src_type</tt> value at construction time.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
* <dd>The input tag cannot identify a deformable model, a load, a spring, a spring set,
*an attractor object, or a constraint in the model hierarchy.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of dmod hierarchy to search.
* @param tag
* in: cstrn identifier.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM int                // rtn: orof DM_DELETABLE/DM_STOPABLE
DM_get_cstrn_rights        // eff: get cstrn's del/stop rights
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of target dmod hierarchy to search, [pre-alloced]
   int tag,                // in : cstrn identifier
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod
/*
DECL_DM double             // rtn: tang_vec gain for DM_TAN_FIXED cstrns
DM_get_cstrn_tangent_gain  // eff: get tangent_gain for a cstrn
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of target dmod hierarchy to search, [pre-alloced]
   int tag) ;              // in : cstrn identifier
                           // note: sets active dmod

DECL_DM void
DM_set_cstrn_tangent_gain  // eff: change tangent_gain for a cstrn
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of target dmod hierarchy to search, [pre-alloced]
   int tag,                // in : cstrn identifier
   double tangent_gain) ;  // in : tang SPAvector multiplier for
                           //      DM_TAN_FIXED constraints.
                           // note: sets active dmod
*/
/**
* Gets the type identifier of a constraint's tangent gain.
* <br><br>
* <b>Role:</b> Modifies <tt>type_id</tt>, <tt>src_type</tt>.
* <br><br>
* Searches the deformable model hierarchy for a curve constraint with the given
* input tag value and loads <tt>type_id</tt> and <tt>src_type</tt> with that constraint type values.
*<br><br>
* Valid <tt>type_ids</tt> include:
* <br>
* <ul><li><tt>DS_tag_pt_cstrn</tt></li><br>
* <li><tt>DS_tag_pt_seam</tt></li><br>
* <li><tt>DS_tag_crv_cstrn</tt></li><br>
* <li><tt>DS_tag_crv_seam</tt></li><br>
* <li><tt>DS_tag_link_cstrn</tt></li></ul>
* <br>
* Valid <tt>src_types</tt> include:
* <br>
* <ul><li><tt>ds_solid_cstrn</tt></li><br>
* <li><tt>ds_bound_cstrn</tt></li><br>
* <li><tt>ds_user_cstrn</tt></li><br>
* <li><tt>ds_seam_cstrn</tt></li><br>
* <li><tt>ds_link_cstrn</tt></li></ul>
* <br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
* <dd>The input tag cannot identify a deformable model, a load, a spring, a spring set,
*an attractor object, or a constraint in the model hierarchy.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of dmod hierarchy to search.
* @param tag
* in: cstrn identifier.
* @param type_id
* out: one of DS_tag_pt_cstrn DS_tag_pt_seam DS_tag_crv_cstrn DS_tag_crv_seam DS_tag_link_cstrn DS_tag_area_cstrn DS_tag_link_load DS_tag_crv_load.
* @param src_type
* out: one of ds_solid_cstrn ds_bound_cstrn ds_user_cstrn ds_seam_cstrn ds_link_cstrn.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM void
DM_get_cstrn_type_id       // eff: get cstrn's type_id and src_type vals
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of target dmod hierarchy to search, [pre-alloced]
   int tag,                // in : cstrn identifier
   DS_TAGS &type_id,       // out: DS_tag_pt_cstrn:   pt constraint
                           //      DS_tag_pt_seam:    pt seam cstrn
                           //      DS_tag_crv_cstrn:  curve cstrn
                           //      DS_tag_crv_seam:   curve seam cstrn
                           //      DS_tag_link_cstrn: multi-surf link cstrn
                           //      DS_tag_area_cstrn: area cstrn
                           //      DS_tag_
   DS_CSTRN_SRC &src_type, // out: ds_solid_cstrn: from solid model
                           //      ds_bound_cstrn: from sheet model
                           //      ds_user_cstrn:  from end user
                           //      ds_seam_cstrn:  from patch hierarcy
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod

/**
* Returns the pass-through user data pointer stored with a constraint.
* <br><br>
* <b>Role:</b> This function returns the pass-through user data pointer stored
* with a constraint. This pointer is stored for the convenience of the calling
* application that may want to store local application-specific data with each
* constraint. The pointer is never accessed or used by the <tt>DM</tt> Modeler package.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
* <dd>The input tag cannot identify a deformable model, a load, a spring, a spring set,
*an attractor object, or a constraint in the model hierarchy.</dd>
*<dt><tt>DM_BAD_SRC_DATA_TGT_VALUE</tt></dt>
*<dd> The input tgt value must be <tt>1</tt> or <tt>2</tt>.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: member of dmod hierarchy to search.
* @param tag
* cstrn identifier.
* @param tgt
* for link_cstrns, tgt = 1 or 2.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM void*              // rtn: ptr to src data stored with cstrn, [pass-thru]
DM_get_cstrn_src_data      // eff: get cstrn's src data pointer
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of target dmod hierarchy to search, [pre-alloced]
   int tag,                // in : cstrn identifier
   int tgt=1,              // in : for link_cstrns,tgt = 1 or 2
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod

/**
* Modifies the value of the pass through <tt>src_data</tt> pointer stored with each constraint.
* <br><br>
* <b>Role:</b> Sets the tag constraint's <tt>src_data</tt> pointer value. The <tt>src_data</tt>
* pointer value is stored with each constraint as a convenience for the
* application program which might want to store application specific data. The DM
* Modeler package never accesses or uses the <tt>src_data</tt> pointer. Calling
* applications that use this pointer must guard against memory leaks on their own
* as the constraints are deleted.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on input.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>The input tag cannot identify a constraint in the deformable model hierarchy.</dd>
*<dt><tt>DM_BAD_SRC_DATA_TGT_VALUE</tt></dt>
*<dd>The input target value must be 1 or 2.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: member of target dmod hierarchy to search.
* @param tag
* cstrn identifier.
* @param tgt
* for link_cstrns, tgt = 1 or 2.
* @param src_data
* new src_data ptr value.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM void
DM_set_cstrn_src_data      // eff: change src_data for a cstrn
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of target dmod hierarchy to search, [pre-alloced]
   int tag,                // in : cstrn identifier
   int tgt=1,              // in : for link_cstrns,tgt = 1 or 2
   void *src_data=NULL,    // in : new src_data ptr value, [pass-thru]
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod

/**
* Returns pointers to a curve or link constraint's source <tt>DS_pfunc</tt> objects,which define the shape of the constraint.
* <br><br>
* <b>Role:</b> This function returns pointers to the stored source <tt>DS_pfunc</tt>
* objects which can be used to specify the shape of a curve or link constraint.
* These functions are not required when constructing those constraints, in which
* case the pointers will be set to <tt>NULL</tt>. Refer to the functions,
* <tt>DM_add_crv_cstrn()</tt> and <tt>DM_add_link_cstrn()</tt>. These pointer values can be modified
* with the call <tt>DM_set_cstrn_src_pfuncs()</tt>.
* <br><br>
* The <tt>tgt</tt> value specifies which set of <tt>src_pfunc</tt> values to return when the input
* tag value identifies a link_cstrn; either the <tt>src_pfuncs</tt> associated with the
* first or second of the link's <tt>DS_dmond</tt> objects.
* <br><br>
* All output values will be set to <tt>NULL</tt> when calling this function on a point or
* area constraint.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd> The input tag cannot identify a deformable model, a load, a spring, a spring set,
*an attractor object, or a constraint in the model hierarchy.</dd>
*<dt><tt>DM_NOT_A_CRV_LINK_CSTRN</tt></dt>
*<dd>The input tag did not identify a curve or link constraint.</dd>
*<dt><tt>DM_BAD_SRC_DATA_TGT_VALUE</tt></dt>
*<dd>The input tgt value must be 1 or 2.</dd></dl><br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: member of dmod hierarchy to search.
* @param tag
* cstrn identifier.
* @param tgt
* for link_cstrns, tgt = 1 or 2.
* @param src_W_pfunc
* out: cstrn pos shape, [nested].
* @param src_Wn_pfunc
* out: cross tang shape, [nested].
* @param src_Wnn_pfunc
* out: curvature shape, [nested].
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM void
DM_get_cstrn_src_pfuncs    // eff: get cstrn's src data pointer
 (int &rtn_err,            // out: 0=success or neg err code
  DS_dmod *dmod,           // in : member of target dmod hierarchy to search, [pre-alloced]
  int tag,                 // in : cstrn identifier
  int tgt,                 // in : for link_cstrns,tgt = 1 or 2
  DS_pfunc *&src_W_pfunc,  // out: cstrn pos shape, [nested]
  DS_pfunc *&src_Wn_pfunc, // out: cross-tang shape,[nested]
  DS_pfunc *&src_Wnn_pfunc,// out: curvature shape, [nested]
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod

/**
* Sets a curve or link constraint's source <tt>DS_pfunc</tt> objects, which are used to specify the shape of the constraint.
* <br><br>
* <b>Role:</b> Sets the target curve or link constraint's defining source
* <tt>DS_pfunc</tt> objects. When supplied, these shapes are used to specify the target
* location for the constrained surface. Whenever any of the input values are NULL,
* the internal <tt>DS_pfunc</tt> object is removed from the constraint data structure and
* the shape of the constraint is defined as the current shape of the constrained
* surface. Whenever any of the input values equal an existing stored value, no
* actions are taken on that object.
* <br><br>
* When a previously stored <tt>DS_pfunc</tt> object is replaced or removed from its
* constraint with this call, its reference count is decremented. If that reference
* count reaches zero then the <tt>DS_pfunc</tt> will be automatically deleted. Calling
* delete on any of the src_pfunc objects will cause a memory error.
* <br><br>
* Refer to functions <tt>DM_add_crv_cstrn()</tt>, <tt>DM_add_link_cstrn()</tt>, and
* <tt>DM_get_cstrn_src_pfuncs()</tt>.
* <br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on input.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>The input tag cannot identify a constraint in the deformable model hierarchy.</dd>
*<dt><tt>DM_NOT_A_CRV_LINK_CSTRN</tt></dt>
*<dd>The input tag did not identify a curve or link constraint.</dd>
*<dt><tt>DM_BAD_SRC_DATA_TGT_VALUE</tt></dt>
*<dd>The input target value must be <tt>1</tt> or <tt>2</tt>.</dd>
*<dt><tt>DM_BAD_SRC_PFUNC_MAPPING</tt></dt>
* <dd>Whenever a src_pfunc does not have a proper <tt>domain_dim</tt> or <tt>image_dim</tt> value. The
* <tt>domain_dim</tt> value for all <tt>src_pfuncs</tt> must be <tt>1</tt> (they are all curves), and the
* image_dim for the <tt>src_W</tt> and <tt>src_Wn</tt> <tt>DS_pfuncs</tt> must equal the <tt>image_dim</tt> of the
*object being constrained, and the <tt>image_dim</tt> of the <tt>src_Wnn</tt> <tt>DS_pfunc</tt> must be <tt>1</tt>.</dd></dl>
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: member of target dmod hierarchy to search cstrn identifier for link_cstrns.
* @param tag
* cstrn identifier.
* @param tgt
* for link_cstrns, tgt = 1 or 2.
* @param src_W_pfunc
* in: cstrn pos shape, [nested].
* @param src_Wn_pfunc
* in: cross-tang shape, [nested].
* @param src_Wnn_pfunc
* in: curvature shape, [nested].
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM void
DM_set_cstrn_src_pfuncs    // eff: change src_data for a cstrn
 (int &rtn_err,            // out: 0=success or neg err code
  DS_dmod *dmod,           // in : member of target dmod hierarchy to search, [pre-alloced]
  int tag,                 // in : cstrn identifier
  int tgt,                 // in : for link_cstrns,tgt = 1 or 2
  DS_pfunc *src_W_pfunc,   // in : cstrn pos shape, [nested]
  DS_pfunc *src_Wn_pfunc,  // in : cross-tang shape,[nested]
  DS_pfunc *src_Wnn_pfunc, // in : curvature shape, [nested]
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod
/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM void
DM_get_cstrn_src_dmods     // eff: get cstrn's src data pointer
 (int &rtn_err,            // out: 0=success or neg err code
  DS_dmod *dmod,           // in : member of target dmod hierarchy to search, [pre-alloced]
  int tag,                 // in : cstrn identifier
  int tgt,                 // in : for link_cstrns,tgt = 1 or 2
  DS_dmod *&src_W_dmod,    // out: cstrn pos shape, [nested]
  DS_dmod *&src_Wn_dmod,   // out: cross-tang shape,[nested]
  DS_dmod *&src_Wnn_dmod,  // out: curvature shape, [nested]
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod
/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM void
DM_set_cstrn_src_dmods     // eff: change src_data for a cstrn
 (int &rtn_err,            // out: 0=success or neg err code
  DS_dmod *dmod,           // in : member of target dmod hierarchy to search, [pre-alloced]
  int tag,                 // in : cstrn identifier
  int tgt,                 // in : for link_cstrns,tgt = 1 or 2
  DS_dmod *src_W_dmod,     // in : cstrn pos shape, [nested]
  DS_dmod *src_Wn_dmod,    // in : cross-tang shape,[nested]
  DS_dmod *src_Wnn_dmod,   // in : curvature shape, [nested]
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod
/**
* Gets end states for target deformable model and returns 0 for success or an error.
* <br><br>
* <b>Role:</b> Modifies <tt>end_cond_u</tt>, <tt>singular_u</tt>, <tt>end_cond_v</tt>, <tt>singular_v</tt>.
* <br><br>
* Loads input arguments with the target deformable model's end condition values.
* <br><br>
* end_cond:
* <br>
* <tt>0 = </tt>open, no affect on surface
* <br>
* <tt>1 = </tt>closed, end cpts are constrained to be the same
* <br>
* <tt>2 = </tt>periodic, end tangents are constrained to be the same
* <br><br>
* singular:
* <br>
* <tt>0 =</tt> none, no affect
* <br>
* <tt>1 =</tt> low, all cpts on low boundary are constrained to be the same
* <br>
* <tt>2 = </tt>high, all cpts on high boundary are constrained to be the same
* <br>
* <tt>3 = </tt>both, all cpts on low boundary are constrained to be the same and all
* cpts on high boundary are constrained to be the same
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
*<br><br>
* @param rtn_err
* out: 0=success or negative err code
* @param dmod
* in: dmod to query
* @param end_cond_u
* out: one of 0=open or 1=closed or 2=periodic
* @param singular_u
* out: one of 0=none or 1=low or 2=high or 3=both
* @param end_cond_v
* out: one of 0=open or 1=closed or 2=periodic
* @param singular_v
* out: one of 0=none or 1=low or 2=high or 3=both
* @param sdmo
* in:SDM_options pointer  note: only end_cond_u set by 1d curves all other values set to -1.
**/
DECL_DM void
DM_get_end_conds           // eff: get end-states for target_dmod
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : root of dmod hierarchy to query, [pre-alloced]
   int &end_cond_u,        // out: oneof 0=open|1=closed|2=periodic
   int &singular_u,        // out: oneof 0=none|1=low|2=high|3=both
   int &end_cond_v,        // out: oneof 0=open|1=closed|2=periodic
   int &singular_v,        // out: oneof 0=none|1=low|2=high|3=both
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: only end_cond_u set by 1d curves
                           //       all other values set to -1.

/**
* Sets the end states for a deformable model and returns <tt>0</tt> for success or returns an error.
* <br><br>
* <b>Role:</b> Modifies deformable <tt>model->end_conds</tt> and deformable
* model->singulars.
* <br><br>
* Assigns values to the end condition descriptions contained in the deformable
* <tt>model->pfunc()</tt>. Only the root deformable model may have non-open end conditions
* at this time.
* <br><br>
* end_cond:
* <br>
* 0 = open, no effect on surface
* <br>
* 1 = closed, end control points are constrained to be the same
* <br>
* <tt>2 = </tt>periodic, end tangents are constrained to be the same
* <br><br>
* singular:
* <br>
* 0 = none, no effect
* <br>
* 1 = low, all control points on low boundary are constrained to be the same
* <br>
* <tt>2 = </tt>high, all control points on high boundary are constrained to be the
* same
* <br>
* <tt>3 = </tt>both, all control points on low boundary are constrained to be the
* same and all control points on high boundary are constrained to be the same.
* <br><br>
* <b>Errors:</b><dl><dt><tt>DM_DMOD_NOT_A_ROOT_DMOD</tt></dt>
*<dd>The deformable model must be the root of a hierarchy tree.</dd>
*<dt><tt>DM_BAD_END_COND_VALUE</tt></dt>
*<dd>The end condition values must be <tt>0</tt>, <tt>1</tt>, or <tt>2</tt>.</dd>
*<dt><tt>DM_BAD_SINGULAR_VALUE</tt></dt>
*<dd>The singular values must be <tt>0</tt>, <tt>1</tt>, <tt>2</tt>, or <tt>3</tt>.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: root of hierarchy tree.
* @param end_cond_u
* in: 0=open 1=closed 2=periodic.
* @param singular_u
* in: 0=none 1=low 2=high 3=both.
* @param end_cond_v
* in: 0=open 1=closed 2=periodic.
* @param singular_v
* in: 0=none 1=low 2=high 3=both.
* @param sdmo
* in:SDM_options pointer note: only end_cond_u used for 1d curves note: only dmod may be closed or periodic.
**/
DECL_DM void
DM_set_end_conds           // eff: set end-states for root_dmod
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod whose end conds can be set, [pre-alloced]
   int end_cond_u,         // in : oneof 0=open|1=closed|2=periodic
   int singular_u,         // in : oneof 0=none|1=low|2=high|3=both
   int end_cond_v,         // in : oneof 0=open|1=closed|2=periodic
   int singular_v,         // in : oneof 0=none|1=low|2=high|3=both
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: only end_cond_u used for 1d curves.
                           // note: only root_dmod may be closed or periodic
/**
* Returns the area constraint's fixed inside/outside flag where <tt>0=</tt>zone is <tt>free,1=</tt>zone compliment area is free.
* <br><br>
* <b>Role:</b> Returns <tt>0</tt> when the zone area is deformable and the zone
* compliment area is fixed. Returns <tt>1</tt> when the zone area is fixed and the zone
* compliment area is free to move.
* <br><br>
* Passes the change request down to the deformable model so that all affected
* arrays can be updated.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>The input tag does not identify an area constraint.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of dmod hierarchy to search.
* @param tag
* n: cstrn identifier.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM int                // rtn: 0=zone area moves
                           //        zone compliment fixed
                           //      1=zone area fixed
                           //        zone compliment moves
DM_get_area_cstrn_flag     // eff: get on/off state for a cstrn
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of target dmod hierarchy, [pre-alloced]
   int tag,                // in : cstrn identifier
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod

/**
* Modifies the fixed inside/outside flag for an area constraint.
* <br><br>
* <b>Role:</b> Searches the deformable model hierarchy for the area constraint
* with the given input tag value and modifies that area constraint's fixed
* inside/outside flag value. Also sets the active deformable model.
* <br><br>
* <li> When <tt>zone_flag == 0</tt>, the zone area moves and the zone compliment area is
* fixed.</li><br>
* <li> When <tt>zone_flag == 1</tt>, the zone area is fixed and the zone compliment area
* is free to move.</li><br>
* <li> When <tt>zone_flag == -1</tt>, the current <tt>zone_flag</tt> value is toggled.</li>
*<br><br>
* Passes the change request down to the deformable model so that all affected
* arrays can be updated.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>The input tag does not identify an area constraint in the model hierarchy.</dd>
*<dt><tt>DM_BAD_ZONE_FLAG_VALUE</tt></dt>
*<dd>The zone_flag must be equal <tt>-1</tt>, <tt>0</tt>, or <tt>1</tt>.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: root of hierarchy tree.
* @param tag
* in: unique tag identifier to match.
* @param zone_flag
* in: 0=zone area moves zone compliment fixed 1=zone area fixed zone compliment moves -1=toggle current zone_flag value.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_set_area_cstrn_flag     // eff: change state for a cstrn
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of target dmod hierarchy to search, [pre-alloced]
   int tag,                // in : cstrn identifier
   int zone_flag,          // in : 0=zone area moves
                           //        zone compliment fixed
                           //      1=zone area fixed
                           //        zone compliment moves
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod

//============================================================
// int DM_print_dmod_cstrns()
//============================================================

/**
* Generates a deformable model reports and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies the file.
*<br><br>
* Using <tt>fprintf</tt> statements this function writes
* a report about all the objects operating on the input deformable model.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to query.
* @param file
* in: stream to be written.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_print_dmod_cstrns       // eff: output a dmod report
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to query, [pre-alloced]
   FILE *file,             // in : stream to be written, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

//==========================================================
// DM_get_interior_state(), DM_set_interior_state()
//==========================================================

/**
* Gets the deformable model's interior state value.
* <br><br>
* <b>Role:</b> Returns value:
* <br><br>
* <tt>0 = </tt>allow <tt>C0</tt> between elements
* <tt>1 = </tt>enforce <tt>C1</tt> between elements
* <br><br>
* When <tt>interior_state == 0</tt>, the Deformable model is allowed to bend with <tt>C0</tt>
* discontinuity between elements. For <i>B</i>-splines and <tt>NURBS</tt>, <tt>C0</tt> discontinuity within
* a surface can be achieved by increasing the knot count at an internal knot
* boundary.
* <br><br>
* When <tt>interior_state == 1</tt>, the Deformable model prohibits <tt>C0</tt> bending between
* elements by adding <tt>C1</tt> internal tangent constraints between any elements whose
* internal representation will allow a <tt>C0</tt> bend. For <i>B</i>-splines and <tt>NURBS</tt>, this
* means that the deformation will be at least <tt>C1</tt> everywhere, even if the
* underlying representation has multiple knots that would normally allow a <tt>C0</tt>
* internal bend.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to be queried.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM int                // rtn: 0 = C0 between elements allowed
                           //      1 = C1 between elements enforced
DM_get_interior_state      // eff: get interior state for dmod
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be queried, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Sets the deformable model's interior state value and returns <tt>0</tt> for success or a negative error code.
* <br><br>
* <b>Role:</b> Modifies deformable model's interior state value.
* <br><br>
* When <tt>interior_state == 0</tt>, the Deformable model is allowed to bend with <tt>C0</tt>
* discontinuity between elements. For <i>B</i>-splines and <tt>NURBS</tt>, <tt>C0</tt> discontinuity within
* a surface can be achieved by increasing the knot count at an internal knot
* boundary.
* <br><br>
* When <tt>interior_state == 1</tt>, the Deformable model prohibits <tt>C0</tt> bending between
* elements by adding <tt>C1</tt> internal tangent constraints between any elements whose
* internal representation will allow a C0 bend. For <i>B</i>-splines and <tt>NURBS</tt>, this
* means that the deformation will be at least <tt>C1</tt> everywhere even if the underlying
* representation has multiple knots that would normally allow a <tt>C0</tt> internal bend.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_BAD_INTER_STATE_VALUE</tt></dt>
*<dd>The interior state must be <tt>0</tt> or <tt>1</tt>.</dd>
*<dt><tt>DM_BAD_WALK_FLAG_VALUE</tt></dt>
*<dd>The <tt>walk_flag</tt> must be <tt>0</tt>, <tt>1</tt> or <tt>2</tt>.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success.
* @param dmod
* or negative err code.
* @param interior_state
* in: dmod to modify.
* @param walk_flag
* in: 0=allow C0 bends.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_set_interior_state      // eff: set end-states for root_dmod
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to be modified, [pre-alloced]
   int interior_state,     // in : 0 = allow C0 between elements
                           //      1 = enforce C1 between elements
   int walk_flag,          // out: 0=tgt_only, 1=tgt and offspring
                           //      2=dmod,siblings,and offspring
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: makes found dmod active


//============================================================
// Load Management
//============================================================
//============================================================
// DM_add pt_press, dist_press, vector_load, attractor,
//        spring, spring_set, crv_load
//============================================================

/**
* Adds a pressure point to deformable model and returns a new tag identifier or an error.
* <br><br>
* <b>Role:</b> Modifies the target deformable model.
* <br><br>
* Builds and adds to the target deformable model a <tt>DS_pressure</tt> point with a domain
* point and gain as specified by the input arguments. When tag equals <tt>-1</tt> the
* system assigns the next available tag number to the newly created load.
* Otherwise, the input tag value is used as the load's tag number. When the
* <tt>negate_flag</tt> equals <tt>0</tt> the pressure load is applied in the deformable model's
* normal direction. When <tt>negate_flag</tt> is set, the pressure vector is negated before
* being applied.
* <br><br>
* When <tt>domain_flag</tt> is set to <tt>1</tt>, <tt>dp</tt>t is given in the unit range and mapped to the
* dmod's actual domain range by this function. When <tt>domain_flag</tt> is set to <tt>0</tt>, <tt>dpt</tt>
* is given in the dmod's original domain range (the one with which it was created).
* When <tt>domain_flag</tt> is set to <tt>2</tt>, <tt>dpt</tt> is given in the dmod's <tt>pfunc</tt>'s
* <tt>internal_pfunc_space</tt>.
* <br><br>
* <tt>tag_flag</tt> selects the target deformable model. When tag equals <tt>-1</tt> the next
* available tag number is assigned to the newly created load. Otherwise, the input
* tag number is used. Other valid values for the tag flag are:
* <br><br>
* <table border="0" width="100%">
* <tr>
* <td width="3%" align="left"><tt>1</tt></td>
* <td width="97%" align="left">active deformable model</td>
* </tr>
* <tr>
* <td width="3%" align="left"><tt>2</tt></td>
* <td width="97%" align="left"> root deformable model</td>
* </tr>
*</table><br>
* Otherwise, the target is the deformable model whose tag identifier equals
* abs(<tt>tag_flag</tt>).
* <br><br>
* Returns a newly created <tt>DS_load</tt>'s tag number.
* <br><br>
* <b>Errors:</b>
*<dl><tt>DM_parse_tag_flag()</tt> errors
*<dt><tt>DM_scale_unit_dpt_to_pfunc()</tt></dt>
*<dd>errors</dd>
*<dt><tt>DM_BAD_TAG_VALUE</tt></dt>
*<dd>The tag value must be <tt>-1</tt>, to have the system assign one, or <tt>2</tt> or greater.</dd>
*<dt><tt>DM_NO_ROOT_DMOD</tt></dt>
*<dd>The the input deformable model cannot be <tt>NULL</tt>.</dd>
*<dt><tt>DM_BAD_NEGATE_FLAG_VALUE</tt></dt>
*<dd>The <tt>negate_flag</tt> must be a <tt>0</tt> or a <tt>1</tt>.</dd>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The dpt cannot be <tt>NULL</tt> on input.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: member of target dmod hierarchy to search.
* @param tag_flag
* in: specify tgt dmod 1 = active dmod 2 = root dmod else = member dmod with tag=tag_flag.
* @param domain_flag
* orig_dmod_space, 1=dpt in unit_space, 2=dpt in internal_pfunc_space.
* @param dpt
* in: load's domain loc, [u,v] sized:[Domain_dim].
* @param gain
* in: magnitude of load gain.
* @param negate_flag
* in: change normal dir (1=negate,0=do not).
* @param tag
* in: when = -1 assign next tag number else use this tag number.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM int                // rtn: new tag id or negative err code
DM_add_pt_press            // eff: adds pt_press to d2a_dmod
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   int tag_flag,           // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
   int domain_flag,        // in : 0=dpt in orig_dmod_space,
                           //      1=dpt in unit_space,
                           //      2=dpt in internal_pfunc_space.
   double *dpt,            // in : load's domain loc, [u,v], [pre-alloced]
                           //      sized:[DM_get_domain_dim(rtn_err, dmod)]
   double gain=0.0,        // in : magnitude of load gain
   int negate_flag=0,      // in : change normal dir (1=negate,0=dont)
   int tag=-1,             // in : when == -1 assign next tag number
                           //      else use this tag number
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod
/**
* Adds a distributed pressure attribute to the deformable model and returns a new tag identifier.
* <br><br>
* <b>Role:</b> Modifies the target deformable model.
* <br><br>
* Builds and adds to the target deformable model a distributed pressure attribute
* with a gain specified by the input arguments. The distributed pressure applies a
* uniform pressure over a sub-rectangle of the deformable model's domain. That
* square is specified by its minimum and maximum corners given in the domain_min
* and domain_max. When domain_min and domain_max pointers are <tt>NULL</tt>, the
* distributed pressure is applied to the entire deformable model domain. When
* <tt>domain_flag</tt> is <tt>1</tt>, <tt>domain_min</tt> and <tt>domain_max</tt> are specified in the unit domain
* range (values from 0 to 1) and when <tt>domain_flag</tt> is 0, <tt>domain_min</tt> and <tt>domain_max</tt>
* are given in the target dmod's original domain space (the range of knot values
* with which the target dmod was originally created). When <tt>domain_flag</tt> is 2,
* <tt>domain_min</tt> and <tt>domain_max</tt> are given in the target dmod's <tt>pfunc</tt>'s internal domain
* space (the range of knot values used within the deformable modeler's internal
* calculations).
* <br><br>
* When <tt>tag</tt> equals <tt>-1</tt> the next available tag number is assigned to the newly
* created load, otherwise the input tag number is used.
* <br><br>
* <tt>tag_flag</tt> selects the <tt>target</tt> deformable model. When <tt>tag</tt> equals <tt>-1</tt> the next
* available tag number is assigned to the newly created load. Otherwise, the input
* tag number is used. Other valid values for the <tt>tag</tt> flag are:
* <br><br>
* <table border="0" width="100%">
* <tr>
* <td width="3%" align="left"><tt>1</tt></td>
* <td width="97%" align="left">active deformable model</td>
* </tr>
* <tr>
* <td width="3%" align="left"><tt>2</tt></td>
* <td width="97%" align="left"> root deformable model</td>
* </tr>
*</table><br>
* Otherwise, the target is the deformable model whose tag identifier equals
* <tt>abs(tag_flag)</tt>
* <br><br>
* Returns a newly created load's tag number.
* <br><br>
* <b>Errors:</b>
*<dl><tt>DM_parse_tag_flag()</tt> errors
*<dt><tt>DM_scale_unit_dpt_to_pfunc()</tt></dt>
*<dd>errors</dd>
*<dt><tt>DM_BAD_TAG_VALUE</tt></dt>
*<dd>The input tag value must be <tt>-1</tt> or positive.</dd>
*<dt><tt>DM_NO_ROOT_DMOD</tt></dt>
*<dd>The the input deformable model cannot be <tt>NULL</tt>.</dd>
*<dt><tt>DM_BAD_NEGATE_FLAG_VALUE</tt></dt>
*<dd>The <tt>negate_flag</tt> must be a <tt>0</tt> or a <tt>1</tt>.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: member of tgt dmod hierarchy to search.
* @param tag_flag
* in: specify tgt dmod 1 = active dmod 2 = root dmod else = member dmod with tag=tag_flag.
* @param domain_flag
* in: 0=domain_pts in orig_dmod_space 1=domain_pts in unit_space. 2=domain_pts in internal_pfunc_space..
* @param domain_min
* in: min-domain [not-nested].
* @param domain_max
* in: max-domain [not-nested].
* @param gain
* in: magnitude of load gain.
* @param negate_flag
* in: change normal dir (1=negate,0=do not).
* @param tag
* in: when == -1 assign next tag number else use this tag number.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/

DECL_DM int                // rtn: new tag id or negative err code
DM_add_dist_press          // eff: adds dist_press to d2a_dmod
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   int tag_flag=1,         // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
   int domain_flag=1,      // in : 0=domain_pts in orig_dmod_space,
                           //      1=domain_pts in unit_space,
                           //      2=domain_pts in internal_pfunc_space.
   double *domain_min=NULL,// in : low bound if NULL load all domain, [pre-alloced]
                           //      sized:[DM_get_domain_dim(rtn_err, dmod)]
   double *domain_max=NULL,// in : top bound if NULL load all domain, [pre-alloced]
                           //      sized:[DM_get_domain_dim(rtn_err, dmod)]
   double gain=0.0,        // in : magnitude of load gain
   int negate_flag=0,      // in : change normal dir (1=negate,0=dont)
   int tag=-1,             // in : when == -1 assign next tag number
                           //      else use this tag number
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod
/**
* Adds a vector load to deformable model and returns a tag identifier or an error.
* <br><br>
* <b>Role:</b> Builds and adds to the target deformable model a vector load
* specified by an image space vector and a gain. The direction of the vector load
* is specified either by <tt>image_vec</tt>, or computed as the shape normal at the center
* of the deformable model when <tt>image_vec</tt> is <tt>NULL</tt>.
* <br><br>
* A vector load is a constant vector force applied to the entire deformable shape.
* This load can be used to simulate a gravitational effect. The direction of the
* vector load is the direction of the input <tt>image_vec</tt>. Its magnitude is stored in
* gain.
* <br><br>
* <tt>tag_flag</tt> selects the target deformable model. When tag equals <tt>-1</tt> the next
* available tag number is assigned to the newly created load. Otherwise, the input
* tag number is used. Other valid values for the tag flag are:
* <br><br>
* <table border="0" width="100%">
* <tr>
* <td width="3%" align="left"><tt>1</tt></td>
* <td width="97%" align="left">active deformable model</td>
* </tr>
* <tr>
* <td width="3%" align="left"><tt>2</tt></td>
* <td width="97%" align="left"> root deformable model</td>
* </tr>
*</table><br>
* Otherwise, the target is the deformable model whose tag identifier equals
* <tt>abs(tag_flag)</tt>
* <br><br>
* Returns a new load's tag number when successful.
* <br><br>
* <b>Errors:</b>
*<dl><tt>DM_parse_tag_flag()</tt> errors
*<dt><tt>DM_BAD_TAG_VALUE</tt></dt>
*<dd>The input tag value must be <tt>-1</tt> or positive.</dd>
*<dt><tt>DM_NO_ROOT_DMOD</tt></dt>
*<dd>The the input deformable model cannot be <tt>NULL</tt>.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: member of target dmod hierarchy to search.
* @param tag_flag
* in: specify tgt dmod 1 = active dmod 2 = root dmod else = member dmod with tag=tag_flag.
* @param image_vec
* in: direction of vector_load or NULL sized:[image_dim] [not-nested].
* @param gain
* in: magnitude of load gain.
* @param tag
* in: when = -1 assign next tag number else use this tag number.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM int                // rtn: new tag id or negative err code
DM_add_vector_load         // eff: adds a vector_load to d2a_dmod
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   int tag_flag,           // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
   double *image_vec=NULL, // in : vector_load direction or NULL, [pre-alloced]
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
   double gain=0.0,        // in : magnitude of load gain
   int tag=-1,             // in : when == -1 assign next tag number
                           //      else use this tag number
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod
/**
* Adds an attractor to a deformable model and returns the new tag identifier or negative error code.
* <br><br>
* <b>Role:</b> Modifies the target deformable model.
* <br><br>
* Builds and adds to the target deformable model an attractor load specified by an
* image point location. When <tt>image_pt</tt> is <tt>NULL</tt> the attractor location is computed
* as an offset in the surface normal direction from the center of the target
* deformable model.
* <br><br>
* An attractor acts like a concentrated charge either attracting (negative gains)
* or repulsing (positive gains) the entire deformable model shape with a
* <tt>1/distance**2</tt> law. The distance is measured from the attractor's location to
* each point on the deformable model.
* <br><br>
* <tt>tag_flag</tt> selects the target deformable model. When tag equals <tt>-1</tt> the next
* available tag number is assigned to the newly created load. Otherwise, the input
* tag number is used. Other valid values for the tag flag are:
* <br><br>
* <table border="0" width="100%">
* <tr>
* <td width="3%" align="left"><tt>1</tt></td>
* <td width="97%" align="left">active deformable model</td>
* </tr>
* <tr>
* <td width="3%" align="left"><tt>2</tt></td>
* <td width="97%" align="left"> root deformable model</td>
* </tr>
*</table><br>
* Otherwise, the <tt>target</tt> is the deformable model whose tag identifier equals
* <tt>abs(tag_flag)</tt>
* <br><br>
* Returns a new load's tag number when successful.
* <br><br>
* <b>Errors:</b>
*<dl><tt>DM_parse_tag_flag()</tt> errors
*<dt><tt>DM_BAD_TAG_VALUE</tt></dt>
*<dd>The tag value must be <tt>-1</tt>, to have the system assign one, or <tt>2</tt> or greater.</dd>
*<dt><tt>DM_NO_ROOT_DMOD</tt></dt>
*<dd>The root deformable model cannot be <tt>NULL</tt> on input.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: member of target dmod hierarchy to search.
* @param tag_flag
* in: specify tgt dmod 1 = active dmod 2 = root dmod else = member dmod with tag=tag_flag.
* @param image_pt
* in: attractor's location or NULL sized:[image_dim] mem:[not-nested].
* @param power
* input measure of load's locality 0,1=global: =2,3,... more local.
* @param gain
* in: magnitude of load gain.
* @param tag
* in: when = -1 assign next tag number else use this tag number.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM int                // rtn: new tag id or negative err code
DM_add_attractor           // eff: adds a attractor to d2a_dmod
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   int tag_flag,           // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
   double *image_pt=NULL,  // in : attractor's location or NULL, [pre-alloced]
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
   int power=2,            // in : measure of load's locality
                           //      0,1=global: 2,3.. =more local
   double gain=0.0,        // in : magnitude of load gain
   int tag=-1,             // in : when == -1 assign next tag number
                           //      else use this tag number
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod

/**
* Adds a spring to a deformable model and returns a new tag identifier or an error.
* <br><br>
* <b>Role:</b> Builds and adds to the target deformable model a <tt>DS_spring</tt> with
* its domain point, image point, and gain specified by the input arguments. A
* spring is a force that acts on the surface at the point where the spring is
* attached, in a direction pointing to the spring's image point. gain is the
* spring's stiffness. The spring's image point can be specified in two ways:
* explicitly, by setting <tt>ipt_flag</tt> to <tt>1</tt> and giving an image point vector <i><i><i>x</i>,y</i>,z</i>, or
* by setting <tt>ipt_flag</tt> to <tt>0</tt> and having the system take the initial image point
* location from the deformable model's current location for the domain point's <tt>dpt</tt>
* position.
* <br><br>
* When <tt>domain_flag</tt> is set to <tt>1</tt>, <tt>dpt</tt> is given in the unit range and mapped to the
* <tt>pfunc</tt>'s actual domain range by this function. When <tt>domain_flag</tt> is set to <tt>0</tt>, <tt>dpt</tt>
* is given in the dmod's original domain range. When <tt>domain_flag</tt> is set to <tt>2</tt>, <tt>dpt</tt>
* is given in the dmod's <tt>pfunc</tt>'s internal domain range.
* <br><br>
* When tag equals <tt>-1</tt> the next available tag number is assigned to the newly
* created load. Otherwise, the input tag number is used.
* <br><br>
* <tt>tag_flag</tt> selects the target deformable model. When tag equals <tt>-1</tt> the next
* available tag number is assigned to the newly created load. Otherwise, the input
* tag number is used. Other valid values for the tag flag are:
* <br><br>
* <table border="0" width="100%">
* <tr>
* <td width="3%" align="left"><tt>1</tt></td>
* <td width="97%" align="left">active deformable model</td>
* </tr>
* <tr>
* <td width="3%" align="left"><tt>2</tt></td>
* <td width="97%" align="left"> root deformable model</td>
* </tr>
*</table><br>
* Otherwise, the target is the deformable model whose tag identifier equals
* <tt>abs(tag_flag)</tt>
* <br><br>
* Returns a new load's tag number when successful.
* <br><br>
* <b>Errors:</b>
*<dl><tt>DM_parse_tag_flag()</tt> errors
*<dt><tt>DM_BAD_IPT_FLAG_VALUE</tt></dt>
*<dd>The ipt_flag must be <tt>0</tt> or <tt>1</tt>.</dd>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The input ipt value cannot be <tt>NULL</tt> and the <tt>ipt_flag</tt> cannot be <tt>1</tt>.</dd>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The dpt input pointer cannot be <tt>NULL</tt>.</dd>
*<dt><tt>DM_BAD_DOMAIN_PT_RANGE</tt></dt>
*<dd>The domain point must be completely contained by the deformable model.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: member of target dmod hierarchy to search.
* @param tag_flag
* in: specify tgt dmod 1 = active dmod 2 = root dmod else = member dmod with tag=tag_flag.
* @param domain_flag
* orig_dmod_space, 1=dpt in unit_space, 2=dpt in internal_pfunc_space.
* @param dpt
* in: domain_pt loc of surf spring end sized:[domain_dim] ptr:[not-nested].
* @param ipt
* in: image_pt loc of free spring end sized:[image_dim] ptr:[not-nested].
* @param ipt_flag
* in: 0: let free_pt = image_loc of dpt 1: let free_pt = ipt.
* @param gain
* in: magnitude of load gain.
* @param tag
* in: when = -1 assign else use this tag number.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM int                // rtn: new tag id or negative err code
DM_add_spring              // eff: adds a spring to d2a_dmod
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   int tag_flag,           // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
   int domain_flag,        // in : 0=dpt in orig_dmod_space,
                           //      1=dpt in unit_space,
                           //      2=dpt in internal_dpt_space.
   double *dpt,            // in : domain_pt loc of surf spring end, [pre-alloced]
                           //      sized:[DM_get_domain_dim(rtn_err, dmod)]
   double *ipt=NULL,       // in : image_pt loc of free spring end, [pre-alloced]
                           //      or NULL, sized:[DM_get_image_dim(rtn_err, dmod)]
   int ipt_flag=0,         // in : 0: let free_pt = image_loc of dpt
                           //      1: let free_pt = ipt
   double gain=0.0,        // in : magnitude of load gain
   int tag=-1,             // in : when == -1 assign next tag number
                           //      else use this tag number
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod
/**
* Adds a set of springs to a deformable model and returns a new tag identifier or an error.
* <br><br>
* <b>Role:</b> Builds and adds to the target deformable model a <tt>DS_spring_set</tt>
* using <tt>domain_pts</tt> and <tt>free_pts</tt> and the gain input arguments. A <tt>DS_spring_set</tt> is a
* set of springs that all share a common gain. When <tt>free_pts</tt> is <tt>NULL</tt> each spring's
* <tt>free_pt</tt> position is calculated using the spring's <tt>domain_pt</tt> location and the
* current shape of the deformable model.
* <br><br>
* When <tt>domain_flag</tt> is set to <tt>1</tt>, every dpt is given in the unit range and mapped to
* the dmod's actual domain range by this function. When <tt>domain_flag</tt> is set to <tt>0</tt>,
* the <tt>dpts</tt> are given in the dmod's original domain range. When <tt>domain_flag</tt> is set
* to 2, the <tt>dpts</tt> are given in the dmod's <tt>pfunc</tt>'s internal domain range.
* <br><br>
* <tt>tag_flag</tt> selects the target deformable model. When tag equals <tt>-1</tt> the next
* available tag number is assigned to the newly created load. Otherwise, the input
* tag number is used. Other valid values for the tag flag are:
* <br><br>
* <table border="0" width="100%">
* <tr>
* <td width="3%" align="left"><tt>1</tt></td>
* <td width="97%" align="left">active deformable model</td>
* </tr>
* <tr>
* <td width="3%" align="left"><tt>2</tt></td>
* <td width="97%" align="left"> root deformable model</td>
* </tr>
*</table><br>
* Otherwise, the target is the deformable model whose tag identifier equals
* <tt>abs(tag_flag)</tt>
* <br><br>
* Returns a newly created <tt>DS_load</tt>'s tag number.
* <br><br>
* <b>Errors:</b>
*<dl><tt>DM_parse_tag_flag()</tt> errors
*<dt><tt>DM_scale_unit_dpt_to_pfunc()</tt></dt>
*<dd>errors</dd>
*<dt><tt>DM_BAD_TAG_VALUE</tt></dt>
*<dd>The input tag value must be <tt>-1</tt> or positive.</dd>
*<dt><tt>DM_NO_ROOT_DMOD</tt></dt>
*<dd>The the input deformable model cannot be <tt>NULL</tt>.</dd>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The dpt input pointer cannot be <tt>NULL</tt>.</dd>
*<dt><tt>DM_BAD_PT_COUNT_VALUE</tt></dt>
*<dd>The <tt>pt_count</tt> cannot be equal to or less than <tt>0</tt>.</dd>
*<dt><tt>DM_BAD_DOMAIN_PT_RANGE</tt></dt>
*<dd>The domain point must be completely contained by the deformable model.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: member of target dmod hierarchy to search.
* @param tag_flag
* in: specify tgt dmod 1 = active dmod 2 = root dmod else = member dmod with tag=tag_flag.
* @param domain_flag
* in: 0=domain_pts in orig_dmod_space, 1=domain_pts in unit_space, 2=domain_pts in internal_pfunc_space.
* @param pt_count
* in: number of springs in spring_set.
* @param domain_pts
* i/o: Spring surf end pts 1d=[u0,u1,...un], 2d=[uv0,...,uvN] Sized:[pt_count* domain_dim] ptr:[not-nested].
* @param free_pts
* in: Spring free end pts or NULL [xyz0,...,xyzN] Sized:[pt_count* image_dim] ptr:[not-nested].
* @param gain
* in: stiffness of all springs.
* @param tag
* next tag number else use this tag number.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM int                // rtn: new tag id or negative err code
DM_add_spring_set          // eff: add a set of springs to dmod
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   int tag_flag,           // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
   int domain_flag,        // in : 0=domain_pts in orig_dmod_space,
                           //      1=domain_pts in unit_space.
                           //      2=domain_pts in internal_pfunc_space.
   int pt_count,           // in : number of springs in spring_set
   double *domain_pts,     // i/o: Spring surf end pts, [pre-alloced]
                           //      1d=[u0,u1,..un], 2d=[uv0..,uvN]
                           //      sized:[pt_count*DM_get_domain_dim(rtn_err, dmod)]
   double *free_pts=NULL,  // in : Spring free end pts or NULL, [pre-alloced]
                           //       [xyz0..,xyzN]
                           //      sized:[pt_count*DM_get_image_dim(rtn_err, dmod)]
   double gain=0.0,        // in : stiffness of all springs
   int tag=-1,             // in : when == -1 assign next tag number
                           //      else use this tag number
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod
/**
* Adds a distributed curve spring and returns a new tag identifier or negative error code.
* <br><br>
* <b>Role:</b> Modifies the target deformable model.
* <br><br>
* <tt>DS_CSTRN_SRC</tt> <tt>src_type</tt> must be one of the following values:
* <br><br>
* <table border="0" width="100%">
* <tr>
* <td width="20%" align="left"><tt>ds_solid_cstrn</tt></td>
* <td width="80%" align="left">shared topo boundary</td>
* </tr>
* <tr>
* <td width="20%" align="left"><tt>ds_bound_cstrn</tt></td>
* <td width="80%" align="left">unshared topo boundary</td>
* </tr>
* <tr>
* <td width="20%" align="left"><tt>ds_user_cstrn</tt></td>
* <td width="80%" align="left">user created constraint</td>
* </tr>
* <tr>
* <td width="20%" align="left"><tt>ds_seam_cstrn</tt></td>
* <td width="80%" align="left">join two hierarchical faces</td>
* </tr>
*</table><br>
* Builds and adds a curve load to the target deformable model using <tt>src_C_pfunc</tt>,
* <tt>free_crv</tt>, and the gain input arguments. A curve load is a distributed spring
* which connects a set of points on the deformable model to a set of points in
* three dimensional space. The deformable model points are specified by a domain
* space curve, <tt>C(s) = [u(s), v(s)]</tt> where s is the curve's parameter. The domain
* space curve may be specified in either the loaded deformable model's original
* domain space <tt>(orig_dmod_space)</tt> or in the internal <tt>pfunc</tt>'s domain space
* <tt>(internal_pfunc_space)</tt>. Set <tt>domain_flag = 0</tt>, when <tt>src_C_pfunc</tt> is specified in
* orig_dmod_space and set <tt>domain_flag = 1</tt>, when <tt>src_C_pfunc</tt> is specified in
* <tt>internal_pfunc_space</tt>. When <tt>domain_flag == 0</tt>, the <tt>src_C_pfunc</tt> is scaled from the
* <tt>orig_dmod_space</tt> to the <tt>internal_pfunc_space</tt> and its pointer is stored internally.
* The <tt>src_C_Pfunc</tt> may not be described to this function in a <tt>unit_space</tt> domain
* range.
* <br><br>
* The free space points are specified by an image space curve,<tt> W(s) = [x(s), y(s),
* z(s)]</tt> where <i>s</i> is the same parameter used for the domain space curve. For every
* curve parameter value, s, there is a point-to-point spring between the domain
* curve point on the surface and the image curve point in free space. The net
* effect is a curve spring load or an elastic membrane stretched between the two
* curves. The two curves, <tt>Src_C_pfunc</tt> and <tt>free_crv</tt> should share a common domain
* range. When <tt>src_W_pfunc</tt>, is <tt>NULL</tt> on entry it will be generated by projecting the
* <tt>src_C_pfunc</tt> through the deformable model's shape into image space.
* <br><br>
* The image space of the <tt>Src_C_pfunc</tt> is the same as the <tt>domain_space</tt> of the loaded
* pfunc. This function does NOT scale the <tt>Src_C_pfunc</tt> from the unit square domain
* into the <tt>pfunc</tt>'s domain.
* <br><br>
* This function has two signatures. The first signature (with only eight
* arguments) is supplied for backward compatibility. The second signature allows
* the freedom in specifying target shape and properties to be loaded; the user can
* load position, tangent, or curvature. It is intentionally modeled on the
* signature of <tt>DM_add_crv_cstrn</tt>; see that routine's documentation for a discussion
* the argument list.
* <br><br>
* Even though multiple behaviors can be associated with one curve load, it is
* strongly recommended that a different curve load be added for each behavior to
* be controlled. Each behavior will then have a separate tag which can be used to
* separately tune how closely the curve is pulled to position tangency targets.
* <br><br>
* <tt>gain</tt> is the stiffness of the elastic membrane.
* <br><br>
* <tt>tag_flag</tt> selects the target deformable model. When tag equals <tt>-1</tt> the next
* available tag number is assigned to the newly created load. Otherwise, the input
* tag number is used. Other valid values for the tag flag are:
* <br><br>
* <table border="0" width="100%">
* <tr>
* <td width="3%" align="left"><tt>1</tt></td>
* <td width="97%" align="left">active deformable model</td>
* </tr>
* <tr>
* <td width="3%" align="left"><tt>2</tt></td>
* <td width="97%" align="left"> root deformable model</td>
* </tr>
*</table><br>
* Otherwise, the target is the deformable model whose tag identifier equals
* <tt>abs(tag_flag)</tt>
* <br><br>
* Returns a the tag number of the newly created load.
* <br><br>
* <b>Errors:</b>
*<dl><tt>DM_parse_tag_flag()</tt> errors
*<dt><tt>DM_BAD_TAG_VALUE</tt></dt>
*<dd>The input tag value must be <tt>-1</tt> or positive.</dd>
*<dt><tt>DM_NO_ROOT_DMOD</tt></dt>
*<dd>The the input deformable model cannot be <tt>NULL</tt>.</dd>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The input <tt>src_C_pfunc</tt> is <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_MIXED_CRV_LOAD_DIM</tt></dt>
*<dd>The <tt>src_C_pfunc</tt>'s <tt>image_dim</tt> must be equal to deformable model's <tt>domain_dim</tt>.</dd>
*<dt><tt>DM_MIXED_FREE_LOAD_DIM</tt></dt>
*<dd>The <tt>src_W_pfunc</tt>'s <tt>image_dim</tt> must be equal to the deformable model's <tt>image_dim</tt>.</dd>
*<dt><tt>DM_BAD_DOMAIN_PT_RANGE</tt></dt>
*<dd>The domain point must be completely contained by the deformable model.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: member of tgt dmod hierarchy to search.
* @param tag_flag
* in: specify tgt dmod 1 = active dmod 2 = root dmod else = member dmod with tag=tag_flag.
* @param domain_flag
* in: 0=src_C_pfunc in orig_dmod_space 2=src_C_pfunc in internal_pfunc_space.
* @param src_C_pfunc
* in: domain space curve connected to image curve over common s values. input in orig_dmod_space..
* @param src_W_pfunc
* in: the image space curve if NULL project src_C_pfunc (both curves domain_dim = 1).
* @param gain
* n: stiffness connecting 2 curves.
* @param tag
* in: when = -1 assign next tag number else use this tag number.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM int                // rtn: new tag id or negative err code
DM_add_crv_load            // eff: add a distributed curve spring
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   int tag_flag,           // in : specify tgt dmod
                           //      1   = active dmod
                           //      2   = root   dmod
                           //     else = member dmod with tag==tag_flag
   int domain_flag,        // in : 0=src_C_pfunc in orig_dmod_space
                           //      2=src_C_pfunc in internal_pfunc_space
   DS_pfunc *src_C_pfunc,  // in : domain space curve, [nested]
                           //      connects to image curve
                           //      at common s values.
                           //      input in orig_dmod_space.
   DS_pfunc *src_W_pfunc   // in : the image space curve, [nested]
      =NULL,               //      if NULL project src_C_pfunc
                           //      (both curves domain_dim == 1)
   double gain=0.0,        // in : stiffness connecting the 2 curves
   int tag=-1,             // in : when == -1 assign next tag number
                           //      else use this tag number
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod

//============================================================
// DM_find_load_by_tag()
//============================================================
/**
* Checks the entire hierarchy for a load with matching tag value.
* <br><br>
* <b>Role:</b> Searches every load in every deformable model in the hierarchy
* for a load with a matching tag. If found, this function returns the pointer to
* the load and updates the active deformable model pointer value.
* <br><br>
* The patch tag is set to a positive tag value for success.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model or alpha cannot be <tt>NULL</tt> on input.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
* <dd>The input tag cannot identify a deformable model, a load, a spring, a spring set,
*an attractor object, or a constraint in the model hierarchy.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: member of dmod hierarchy to search.
* @param tag
* in: unique tag identifier to match.
* @param patch_tag
* out: containing patch tag or err code.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM DS_load *          // rtn: load with matching tag value, [nested]
DM_find_load_by_tag        // eff: search entire hierarchy for load
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of hierarchy, [pre-alloced]
   int tag,                // in : unique tag identifier to match
   int &patch_tag,         // out: containing patch's tag or err code
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod

//============================================================
// DM_get pt_press, dist_press, spring, spring_set, curve_load
//============================================================

/**
* Gets the load data into return arguments and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies dpt, gain, <tt>negate_flag</tt>.
* <br><br>
* Looks for a load identified by the given tag in the entire patch hierarchy. When
* the tag specifies a pressure point load, this function fills all the output
* arguments appropriately and returns <tt>0</tt>. It changes the active deformable model to
* the one containing the load. Otherwise, it returns <tt>DM_TAG_OBJECT_NOT_FOUND</tt> and
* leaves output arguments unmodified.
* <br><br>
* When <tt>domain_flag</tt> is set to <tt>1</tt>, <tt>dpt</tt> is given in the unit range and mapped to the
* dmod's actual domain range by this function. When <tt>domain_flag</tt> is set to <tt>0</tt>, <tt>dpt</tt>
* is given in the dmod's original domain range. When <tt>domain_flag</tt> is set to <tt>2</tt>, <tt>dpt</tt>
* is given in the dmod's <tt>pfunc</tt>'s internal domain range.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_NON_NULL_OUTPUT_PTR</tt></dt>
*<dd>The domain point must be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>The input tag cannot identify a deformable model, a load, a spring, a spring set,
* an attractor object, or a constraint in the model hierarchy.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of target dmod hierarchy to search.
* @param tag
* in: load identifier.
* @param domain_flag
* in: 0=dpt in orig_dmod_space, 1=dpt in unit_space, 2=dpt in internal_pfunc_space.
* @param dpt
* out: load's domain loc, [u,v] sized:[Domain_dim].
* @param gain
* out: magnitude of load gain.
* @param negate_flag
* out: change normal dir (1=negate,0=do not).
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM void
DM_get_pt_press            // eff: place load data into rtn args
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   int tag,                // in : load identifier
   int domain_flag,        // in : 0=dpt in orig_dmod_space,
                           //      1=dpt in unit_space,
                           //      2=dpt in internal_pfunc_space.
   double *dpt,            // out: load's domain loc, [u,v], [pre-alloced]
                           //      sized:[DM_get_domain_dim(rtn_err, dmod)]
   double &gain,           // out: magnitude of load gain
   int &negate_flag,       // out: change normal dir (1=negate,0=dont)
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod
/**
* Gets and places the place load data into return arguments.
* <br><br>
* <b>Role:</b> Modifies <tt>domain_min</tt>, <tt>domain_max</tt>, <tt>gain</tt>, <tt>negate_flag</tt>.
* <br><br>
* Looks for a load identified by a given tag in the entire patch hierarchy. When
* the tag specifies a distributed pressure load, this function fills all the
* output arguments appropriately and changes the active deformable model to the
* one containing the load.
* <br><br>
* When <tt>domain_flag</tt> is set to <tt>1</tt>, domain points are given in the unit range and
* mapped to the dmod's actual domain range by this function. When <tt>domain_flag</tt> is
* set to <tt>0</tt>, domain points are given in the dmod's original domain range. When
* <tt>domain_flag</tt> is set to <tt>2</tt>, domain points are given in the dmod's <tt>pfunc</tt>'s internal
* domain range.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd>
* <dt><tt>DM_NULL_OUTPUT_PTR</tt></dt>
*<dd>The domain minimum or maximum is <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>The input tag cannot identify a deformable model, a load, a spring, a spring set,
* an attractor object, or a constraint in the model hierarchy.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of target dmod hierarchy to search.
* @param tag
* in: load identifier.
* @param domain_flag
* in: 0=domain_pts in orid_dmod_space, 1=domain_pts in unit_space. 2=domain_pts in internal_pfunc_space.
* @param domain_min
* out: low bound, sized:[domain_dim].
* @param domain_max
* out: top bound, sized:[domain_dim].
* @param gain
* out: magnitude of load gain.
* @param negate_flag
* out: change normal dir (1=negate,0=do not).
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM void
DM_get_dist_press          // eff: place load data into rtn args
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   int tag,                // in : load identifier
   int domain_flag,        // in : 0=domain_pts in orig_dmod_space,
                           //      1=domain_pts in unit_space,
                           //      2=domain_pts in internal_pfunc_space.
   double *domain_min,     // out: low bound, [pre-alloced]
                           //      sized:[DM_get_domain_dim(rtn_err, dmod)]
   double *domain_max,     // out: top bound, [pre-alloced]
                           //      sized:[DM_get_domain_dim(rtn_err, dmod)]
   double &gain,           // out: magnitude of load gain
   int &negate_flag,       // out: change normal dir (1=negate,0=dont)
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod
/**
* Gets the vector load data and places it into return arguments and returns 0 for success or an error.
* <br><br>
* <b>Role:</b> Modifies <tt>image_vec</tt>, and gain.
* <br><br>
* Looks for a vector load identified by tag in the entire patch hierarchy. When
* the tag specifies a <tt>DS_vector_load</tt>, fills all the output arguments appropriately
* and returns <tt>0</tt>. Changes the active deformable model to the one containing the
* load. Otherwise, returns the error <tt>DM_TAG_OBJECT_NOT_FOUND</tt> and leaves output
* arguments unmodified.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
* <dt><tt>DM_NULL_OUTPUT_PTR</tt></dt>
*<dd>The image vector is <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>The input tag cannot identify a deformable model, a load, a spring, a spring set,
* an attractor object, or a constraint in the model hierarchy.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of target dmod hierarchy to search.
* @param tag
* in: load identifier.
* @param image_vec
* out: image_vec for vector_load direction sized:[image_dim].
* @param gain
* out: magnitude of load gain.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM void
DM_get_vector_load         // eff: place load data into rtn args
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   int tag,                // in : load identifier
   double *image_vec,      // out: image_vec for vector_load direction, [pre-alloced]
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
   double &gain,           // out: magnitude of load gain
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod
/**
* Writes the place load data into return arguments and returns 0 or an error.
* <br><br>
* <b>Role:</b> This function searches for a load identified by specific tag in
* the entire patch hierarchy. When the tag specifies an attractor load, it fills
* all the output arguments appropriately and returns <tt>0</tt>. The active deformable
* model changes to the one containing the load. Otherwise, returns
* <tt>DM_TAG_OBJECT_NOT_FOUND</tt> and leaves output arguments unmodified.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model or beta cannot be <tt>NULL</tt> on input.</dd>
* <dt><tt>DM_NULL_OUTPUT_PTR</tt></dt>
*<dd>The image point is <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
* <dd>The input tag cannot identify a deformable model, a load, a spring, a spring set,
*an attractor object, or a constraint in the model hierarchy.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of target dmod hierarchy to search.
* @param tag
* in: load identifier.
* @param image_pt
* out: image_pt for attractor location sized:[image_dim].
* @param power
* out: measure of load's locality 0,1=global, 2,3,...=more local.
* @param gain
* out: magnitude of load gain.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM void
DM_get_attractor           // eff: place load data into rtn args
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   int tag,                // in : load identifier
   double *image_pt,       // out: image_pt for attractor location, [pre-alloced]
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
   int &power,             // out: measure of load's locality
                           //      0,1=global: 2,3.. =more local
   double &gain,           // out: magnitude of load gain
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod

/**
* Gets the load data and places it into return arguments and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies <tt>dpt</tt>, <tt>ipt</tt>, <tt>base_pt</tt>, gain.
* <br><br>
* Looks for a spring load in the entire patch hierarchy. When the tag specifies a
* spring load, this function fills all the output arguments appropriately and
* returns <tt>0</tt>. Changes the active deformable model to the one containing the load.
* Else returns <tt>DM_TAG_OBJECT_NOT_FOUND</tt> and leaves output arguments unmodified.
* <br><br>
* When <tt>domain_flag</tt> is set to <tt>1</tt>, <tt>dpt</tt> is given in the unit range and mapped to the
* <tt>pfunc</tt>'s actual domain range by this function. When <tt>domain_flag</tt> is set to <tt>0</tt>, <tt>dpt</tt>
* is given in the dmod's original domain range. When <tt>domain_flag</tt> is set to <tt>2</tt>, <tt>dpt</tt>
* is given in the dmod's <tt>pfunc</tt>'s internal domain range.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
* <dt><tt>DM_NULL_OUTPUT_PTR</tt></dt>
*<dd>The domain point and the image point are <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
* <dd>The input tag cannot identify a deformable model, a load, a spring, a spring set,
*an attractor object, or a constraint in the model hierarchy.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of target dmod hierarchy to search.
* @param tag
* in: load identifier.
* @param domain_flag
* in: 0=domain_pts in orig_dmod_space, 1=dpt in unit_space. 2=dpt in internal_pfunc_space.
* @param dpt
* out: domain_pt loc of surf spring end sized:[Domain_dim].
* @param free_pt
* out: image_pt loc of free spring end sized:[image_dim].
* @param base_pt
* out: image_pt loc of attached spring end sized:[image_dim].
* @param gain
* out: magnitude of load gain.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM void
DM_get_spring              // eff: place load data into rtn args
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   int tag,                // in : load identifier
   int domain_flag,        // in : 0=dpt in orig_dmod_space,
                           //      1=dpt in unit_space,
                           //      2=dpt in internal_pfunc_space.
   double *dpt,            // out: domain_pt loc of surf spring end, [pre-alloced]
                           //      sized:[DM_get_domain_dim(rtn_err, dmod)]
   double *free_pt,        // out: image_pt loc of free spring end, [pre-alloced]
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
   double *base_pt,        // out: image_pt loc of attached spring end, [pre-alloced]
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
   double &gain,           // out: magnitude of load gain
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod
/**
* Gets the spring set load data and places it into return arguments.
* <br><br>
* <b>Role:</b> Modifies <tt>pt_count</tt>, <tt>domain_pts</tt>, <tt>free_pts</tt>, gain.
* <br><br>
* Looks for a spring set load identified by the input tag in the entire patch
* hierarchy. When tag specifies a spring set load, this function fills all the
* output arguments appropriately and returns <tt>0</tt>. Changes the active deformable
* model to the one containing the load. Otherwise, returns <tt>DM_TAG_OBJECT_NOT_FOUND</tt>
* and leaves output arguments unmodified.
* <br><br>
* <tt>mallocs</tt> the memory used for the domain point output array so that the return
* domain point values can be scaled from the internal domain range to the unit
* square domain range. The calling program must free this memory.
* <br><br>
* When <tt>domain_flag</tt> is set to <tt>1</tt>, <tt>domain_pts</tt> are given in the unit range and mapped
* to the dmod's actual domain range by this function. When <tt>domain_flag</tt> is set to <tt>0</tt>,
* <tt>domain_pts</tt> are given in the dmod's original domain range. When <tt>domain_flag</tt> is
* set to <tt>2</tt>, <tt>domain_pts</tt> are given in the dmod's <tt>pfunc</tt>'s internal domain range.
* <br><br>
* This function updates the rest of the output pointer values to point to the
* load's nested data. Do not use these pointer values to free memory, nor after
* the pfunc has been deleted.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
* <dt><tt>DM_NULL_OUTPUT_PTR</tt></dt>
*<dd>The domain point and the free point are <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>The input tag cannot identify a deformable model, a load, a spring, a spring set,
* an attractor object, or a constraint in the model hierarchy.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of target dmod hierarchy to search.
* @param tag
* in: load identifier.
* @param domain_flag
* in: 0=domain_pts in orig_dmod_space 1=domain_pts in unit_space. 2=domain_pts in internal_pfunc_space.
* @param pt_count
* out: number of springs in spring_set.
* @param domain_pts
* out: Spring dmod end pts 1d=[u0, u1, ...un], 2d=[uv0..,uvN] Sized:[pt_count* domain_dim].
* @param free_pts
* ut: Spring free end pts or NULL [xyz0, ... xyzN] Sized:[pt_count* image_dim].
* @param base_pts
* ut: Spring attached end pts or NULL [xyz0, ... xyzN] Sized:[pt_count* image_dim].
* @param gain
* out: magnitude of load gain.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM void
DM_get_spring_set          // eff: place load data into rtn args
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   int tag,                // in : load identifier
   int domain_flag,        // in : 0=domain_pts in orig_dmod_space,
                           //      1=domain_pts in unit_space,
                           //      2=domain_pts in internal_pfunc_space.
   int &pt_count,          // out: number of springs in spring_set
   double *&domain_pts,    // out: Spring dmod end pts, [NULL-entry]
                           //      1d=[u0,u1,..un], 2d=[uv0..,uvN]
                           //      sized:[pt_count*DM_get_domain_dim(rtn_err, dmod)]
   double *&free_pts,      // out: Spring free end pts, [nested]
                           //       [xyz0..,xyzN]
                           //      sized:[pt_count*DM_get_image_dim(rtn_err, dmod)]
   double *&base_pts,      // out: Spring attached end pts, [nested]
                           //       [xyz0..,xyzN]
                           //      sized:[pt_count*DM_get_image_dim(rtn_err, dmod)]
   double &gain,           // out: magnitude of load gain
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // mallocs: caller must free domain_pts array
                           // note: sets active dmod

/**
* Gets the spring or spring set length and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies length.
* <br><br>
* Retrieves the tag object identified by the input tag value. When that object is
* a spring, this function then places the length of that spring into the output
* variable. The spring set then places the length of the longest spring in the set
* into the output object.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>The input tag cannot identify a deformable model, a load, a spring, a spring set,
* an attractor object, or a constraint in the model hierarchy.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of target dmod hierarchy to search.
* @param tag
* in: load identifier.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM double             // rtn: spring length (longest for spring_set)
DM_get_spring_length       // eff: get spring or spring_set length
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   int tag,                // in : load identifier
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod

/**
* Sets load data into return arguments and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies <tt>src_C_pfunc</tt>, <tt>src_W_pfunc</tt>, gain.
* <br><br>
* Looks for load identified by a given tag in the entire patch hierarchy. When the
* tag specifies a <tt>DS_curve_load</tt>, this function fills all the output arguments
* appropriately and returns 0. Changes the active deformable model to the one
* containing the load. Although the <tt>src_C_pfunc</tt> was constructed in the containing
* the load. <tt>orig_dmod_space</tt>, it is stored internally so that the <tt>src_C_pfunc</tt>'s
* image space is equal to the <tt>internal_pfunc_domain_space</tt> of the loaded deformable
* model.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
* <dd>The input tag cannot identify a deformable model, a load, a spring, a spring set,
* an attractor object, or a constraint in the model hierarchy.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of target dmod hierarchy to search.
* @param tag
* in: load identifier.
* @param src_C_pfunc
* out: domain space curve connected to image curve over common s values output in internal_pfunc_space.
* @param src_W_pfunc
* out: the image space curve.
* @param gain
* out: stiffness connecting the 2 curves note: the rtn src_C_pfunc ptrs and src_W_pfunc ptrs are still referenced within the load object - they are not copies Do not free or change the data within the structures without first copying them.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM void
DM_get_curve_load          // eff: place load data into rtn args
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   int tag,                // in : load identifier
   DS_pfunc *&src_C_pfunc, // out: domain space curve, [nested]
                           //      connects to image curve
                           //      at common s values,
                           //      output in internal_pfunc_space.
   DS_pfunc *&src_W_pfunc, // out: the image space curve, [nested]
   double &gain,           // out: stiffness connecting the 2 curves
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: the rtn src_C_pfunc ptrs
                           //       and src_W_pfunc ptrs are
                           //       still referenced within the
                           //       load object - they are not copies
                           //       Do not free or change the
                           //       data within the structures
                           //       without first copying them.
                           // note: sets active dmod

//============================================================
// DM_get_load_gain(), DM_set_load_gain(), DM_set_attractor_power()
//============================================================

/**
* Gets the load's gain and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies <tt>load_gain</tt>.
* <br><br>
* Gets the load's gain value, sets the containing deformable model as the active
* deformable model and returns <tt>0</tt> when the tag identifier recognizes a load object.
* Otherwise, returns <tt>DM_TAG_OBJECT_NOT_FOUND</tt> and leaves output arguments
* unmodified.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
* <dd>The input tag cannot identify a deformable model, a load, a spring, a spring set,
*an attractor object, or a constraint in the model hierarchy.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of target dmod hierarchy to search.
* @param tag
* in: load identifier.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM double             // rtn: load's gain value
DM_get_load_gain           // eff: return the load's load_gain
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   int tag,                // in : load identifier
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod

/**
* Sets the deformable model's <tt>load_gain</tt> and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies <tt>tag->DS_load->lds_gain</tt> and sets the active deformable
* model.
* <br><br>
* Sets the tag load's gain value, sets the containing deformable model as the
* active deformable model and returns 0 when the tag identifies a load object.
* Otherwise, returns <tt>DM_TAG_OBJECT_NOT_FOUND</tt> and leaves output arguments
* unmodified.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd>
*<dt><tt>DM_NO_ACTIVE_DMOD</tt></dt>
*<dd>The input deformable model's hierarchy has no currently active deformable model .</dd>
*<dt><tt>DM_BAD_PT_COUNT_VALUE</tt></dt>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>when the input tag value fails to identify a load or constraint</dd>
*<dt><tt>DM_BAD_GROUP_FLAG_VALUE</tt></dt>
*<dd>The <tt>group_flag</tt> must be <tt>0</tt> or <tt>1</tt>.</dd>
*<dt><tt>DM_BAD_INC_FLAG_VALUE</tt></dt>
*<dd>The <tt>inc_flag</tt> must be 0 or 1.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of target dmod hierarchy to search.
* @param type
* i/o: in: type of loads to modify (used when group_flag = 1) out: modified load or DS_tag_none.
* @param tag
* in: unique tag identifier to match.
* @param gain
* in: desired value to store.
* @param group_flag
* in: 0=set tag object only 1=set all active dmod loads of type.
* @param inc_flag
* in: 0:tag(gain) = gain 1:tag(gain)+= gain.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM void
DM_set_load_gain           // eff: set the dmod's load_gain
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   DS_TAGS &type,          // in : type of loads to modify
                           //      (used when group_flag == 1)
                           //      out= modified type or DS_tag_none
   int tag,                // in : load identifier'
                           //      (used when group_flag == 0)
   double gain,            // in : desired value to store
   int group_flag=0,       // in : 0=set tag object only
                           //      1=set all active dmod loads of type
   int inc_flag=0,         // in : 0:tag(gain) = gain
                           //      1:tag(gain)+= gain
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod

/**
* Sets an attractor's power and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Sets the tag load's power value when the input tag value
* identifies a tag object of type <tt>DS_attractor</tt>. Otherwise, returns
* <tt>DM_TAG_OBJECT_NOT_FOUND</tt>.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
* <dd>The input tag cannot identify a deformable model, a load, a spring, a spring set,
*an attractor object, or a constraint in the model hierarchy.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of target dmod hierarchy to search.
* @param tag
* in: unique load tag identifier to match.
* @param power
* in: measure of load's locality 0,1=global: 2,3.. =more local.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_set_attractor_power     // eff: set an attractor's power
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   int tag,                // in : load identifier
   int power=2,            // in : measure of load's locality
                           //      0,1=global: 2,3.. =more local
   SDM_options *sdmo=NULL);// in : SDM_options pointer


/**
* Gets type identifier for a load.
* <br><br>
* <b>Role:</b> Modifies <tt>type_id</tt>, <tt>src_type</tt>.
* <br><br>
* This function searches the deformable model hierarchy for a load with given
* input tag value and returns the type identifier of that load. The valid type
* identifier is one of the following:
* <br><br>
* <table border="0" width="100%">
* <tr>
* <td width="10%" align="left"><tt>DS_tag_pt_press</tt></td>
* <td width="90%" align="left">point pressure</td>
* </tr>
* <tr>
* <td width="10%" align="left"><tt>DS_tag_dist_press</tt></td>
* <td width="90%" align="left">distributed press</td>
* </tr>
* <tr>
* <td width="10%" align="left"><tt>DS_tag_dyn_load</tt></td>
* <td width="90%" align="left">dynamic load</td>
* </tr>
* <tr>
* <td width="10%" align="left"><tt>DS_tag_spring</tt></td>
* <td width="90%" align="left">spring load</td>
* </tr>
* <tr>
* <td width="10%" align="left"><tt>DS_tag_spring_se</tt></td>
* <td width="90%" align="left">spring set</td>
* </tr>
* <tr>
* <td width="10%" align="left"><tt>DS_tag_crv_load</tt></td>
* <td width="90%" align="left">curve load</td>
* </tr>
*</table>
*<br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
* <dd>The input tag cannot identify a deformable model, a load, a spring, a spring set,
*an attractor object, or a constraint in the model hierarchy.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of dmod hierarchy to search.
* @param tag
* in: load identifier.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM DS_TAGS            // rtn: DS_tag_pt_press  : point pressure
                           //      DS_tag_dist_press: distributed press
                           //      DS_tag_dyn_load  : dynamic load
                           //      DS_tag_spring    : spring load
                           //      DS_tag_spring_set: spring set
                           //      DS_tag_crv_load  : curve load
DM_get_load_type_id        // eff: get load's type id
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of target dmod hierarchy to search, [pre-alloced]
   int tag,                // in : load identifier
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod

//============================================================
// Load and Constraint Management
//============================================================

//====================================================
//  DM_convert_loads_and_cstrns
//====================================================
/**
* Converts curve objects into load objects and vice versa and returns a new tag identifier or an error.
* <br><br>
* <b>Role:</b> This function replaces <tt>DM_convert_cstrns_and_loads</tt>. The old
* functionality is now supported by calling this function with the non-default
* argument <tt>DM_forget_target</tt>. This was necessary due to a behavior change in
* converting between curve loads and curve constraints; the new function remembers
* target information.
* <br><br>
* This function modifies the tag object's containing deformable model. When
* allowed, it builds a load from the input constraint and then deletes the
* constraint (or vice versa). When operating on user-defined curve constraints it
* converts the constraint into a load, keeping the tag number the same.
* <br><br>
* The current shape of a curve object is defined as the projection of its domain
* curve into image space using the current deformable surface shape. The <tt>mem_flag</tt>
* argument specifies whether the new curve object will remember the old curve
* objects target shape. If <tt>DM_forget_target</tt> is specified, the new curve object
* will use its current shape as a target. If <tt>DM_remember_target</tt> is specified, it
* will use the old object's target shape.
* <br><br>
* For non-user curve objects, when the curve constraint can be disabled, it is
* disabled and the tag of the newly created curve load is returned. When the curve
* constraint cannot be deleted or disabled, no curve load is created and the value
* <tt>DM_UNSTOPABLE_CSTRN</tt> is returned. This will be the case for solid and seam
* constraints.
* <br><br>
* Note that link constraints and link loads cannot be converted, since link
* constraints are being deprecated.
* <br><br>
* If tag is a constraint that is:
* <br><br>
* <ul><li> not stoppable or the src_type is not <tt>ds_user_cstrn</tt>, then return is
* <tt>DM_NOT_USER_CSTR</tt>.</li>
* <li> a curve and the <tt>src_type</tt> is <tt>ds_boundary_cstrn</tt>, then add a curve load.
* The behaviors (including target curves) remain the same if <tt>DM_remember_target</tt> is
* passed in, otherwise behaviors remain the same but the target becomes the spline
* curve approximation to the current shape of the curve constraint.</li>
* <li> a curve and the <tt>src_type</tt> is <tt>ds_user_cstrn</tt>, then convert the curve
* constraint into a curve load. The behaviors (including target curves) remain the
* same if <tt>DM_remember_target</tt> is passed in, otherwise behaviors remain the same but
* the target becomes the spline curve approximation to the current shape (defined
* above) of the curve constraint. Note that a new tag object is not created; the
* returned tag <tt>ID</tt> will be the same as the tag argument passed in.</li>
* <li> a point, then add a spring.</li>
* <li> deletable (i.e. user constraints), then remove or delete the constraint.</li>
* <li> stoppable (i.e. boundary constraints), then turn it off. </li></ul>
*<br>
* If tag is a load that is:
* <br><br>
*<ul><li> a curve, then convert the curve load into a curve constraint. The
* behaviors (including target curves) remain the same if <tt>DM_remember_target</tt> is
* passed in, otherwise behaviors remain the same but the target becomes the
* current shape of the curve load.</li>
* <li> A pressure point or a spring, then add one point constraint and remove
* and delete the load.</li>
* <li>A spring_set, add one point for each spring and remove and delete the
* load and return the last tag added.</li>
* <li> Another kind of load, then return <tt>DM_LOAD_NOT_CONVERTIBLE</tt>.<br>
* Returns a newly created tag object's tag identifier for success.</li></ul>
* <br><br>
* <b>Errors:</b>
*<dl><dt><tt>DM_NO_ROOT_DMOD</tt></dt>
*<dd>The the input deformable model cannot be <tt>NULL</tt>.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>when the input tag value fails to identify a load or constraint.</dd>
*<dt><tt>DM_UNSTOPABLE_CSTRN</tt></dt>
*<dd>The identified constraint state is set to unstoppable.</dd>
*<dt><tt>DM_NOT_USER_CSTRN</tt></dt>
*<dd>The identified constraint type must be a <tt>ds_user_cstrn</tt> or a <tt>ds_bound_cstrn</tt></dd>
*<dt><tt>DM_LOAD_NOT_CONVERTIBLE</tt></dt>
*<dd>The identified load must be a <tt>DS_crv_load</tt>, a <tt>DS_spring</tt>, or a <tt>DS_spring_set</tt>.</dd>
*<dt><tt>DM_BAD_DOMAIN_PT_RANGE</tt></dt>
*<dd>The domain point must be completely contained by the deformable model's pfunc.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: member of target dmod hierarchy to search.
* @param tag
* in: load/cstrn identifier note.
* @param flag
* in: one of DM_remember_target  (default) DM_forget_target.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM int                // rtn: new tag id or neg err code
DM_convert_loads_and_cstrns// eff: convert crv_loads and springs
                           //      into crv and pt_cstrns and
                           //      vice versus.
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   int tag,                // in : load/cstrn identifier
   SPA_DM_target_memory flag   // in : oneof SPA_DM_remember_target, SPA_DM_forget_target
     = SPA_DM_remember_target,
   SDM_options *sdmo=NULL);// in : SDM_options pointer

                           // note: sets active dmod

//====================================================
//  DM_get/set_pt_uv(), DM_get/set_pt_xyz()
//====================================================
/**
* Gets the tag's current <i>uv</i> location and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies <tt>dpt</tt> for the active deformable model.
* <br><br>
* Searches for a constraint or load with matching tag. When found, this function
* loads the current <i>uv</i> position into dpt. This works for tags of type pressure
* point, spring, and point_constraints. For all other tag types it returns
* <tt>DM_NO_UV_PT_FOR_TAG_OBJ</tt>.
* <br><br>
* When <tt>domain_flag</tt> is set to <tt>1</tt>, dpt is given in the unit range and mapped to the
* <tt>pfunc</tt>'s actual domain range by this function. When <tt>domain_flag</tt> is set to 0, dpt
* is given in the dmod's original domain range. When <tt>domain_flag</tt> is set to 2, dpt
* is given in the dmod's <tt>pfunc</tt>'s internal domain range.
* <br><br>
* <b>Errors:</b><dl><dt><tt>DM_NO_ROOT_DMOD</tt></dt>
*<dd>The root deformable model cannot be <tt>NULL</tt> on input.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
* <dd>The input tag cannot identify a deformable model, a load, a spring, a spring set,
*an attractor object, or a constraint in the model hierarchy.</dd>
* <dt><tt>DM_NULL_OUTPUT_PTR</tt></dt>
*<dd>The domain point cannot be NULL on entry.</dd>
*<dt><tt>DM_NO_UV_PT_FOR_TAG_OBJ</tt></dt>
*<dd>The identified tag object has no distinct <i>uv</i> point to update <tt>dyn_loads</tt>,
* <tt>dist_press</tt>, <tt>crv_loads</tt> and <tt>crv_cstrns</tt>, or a deformable model.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of target dmod hierarchy to search.
* @param tag
* in: load/cstrn identifier.
* @param domain_flag
* in: 0=dpt in orig_dmod_space, 1=dpt in unit_space, 2=dpt in internal_pfunc_space.
* @param dpt
* out: tag's current <i>uv</i> location sized:[Domain_dim].
* @param type
* out: set to type of modified object or DS_tag_none.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM void
DM_get_pt_uv               // get: tag's current uv location
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   int tag,                // in : load/cstrn identifier
   int domain_flag,        // in : 0=return dpt in orig_dmod_space,
                           //      1=return dpt in unit_space,
                           //      2=return dpt in internal_pfunc_space.
   double *dpt,            // out: tag's current uv location, [pre-alloced]
                           //      sized:[DM_get_domain_dim(rtn_err, dmod)]
   DS_TAGS &type,          // out: set to type of modified object
                           //      or DS_tag_none
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod

/**
* Sets domain points in a load or and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies <tt>tag->Domain_pt()</tt>, active deformable model
* <br><br>
* Searches for a constraint or load with a matching tag. When found sets its
* domain point to the given input through the deformable model so that the
* deformable model is aware of the change. Works for tags of type
* <tt>DS_pressure_point</tt>, <tt>DS_spring</tt>, <tt>DS_point_constraint</tt>. For all other tag types
* returns <tt>DM_NO_UV_PT_FOR_TAG_OBJ</tt>.
* <br><br>
* <b>Errors:</b><dl><dt><tt>DM_NO_ROOT_DMOD</tt></dt>
*<dd>The root deformable model cannot be <tt>NULL</tt> on input.</dd>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>The input tag cannot identify a deformable model, a load, a spring, a spring set,
* an attractor object, or a constraint in the model hierarchy.</dd>
*<dt><tt>DM_NO_UV_PT_FOR_TAG_OBJ</tt></dt>
*<dd>The identified tag object has no distinct <i>uv</i> point to update <tt>dyn_loads</tt>,
* <tt>dist_press</tt>, <tt>crv_loads</tt> and <tt>crv_cstrns</tt>, or the deformable model, or when the input
* dpt is not contained by the target deformable model.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of target dmod hierarchy to search.
* @param tag
* in: unique tag identifier to match.
* @param domain_flag
* in: 0=dpt in orig_dmod_space, 1=dpt in unit_space 2=dpt in internal_pfunc_space.
* @param dpt
* in: pt's domain loc in unit square, sized:[Domain_dim].
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM DS_TAGS            // rtn: modified object's type or DS_tag_none
DM_set_pt_uv               // eff: sets dpt in a load or cstrn
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   int tag,                // in : load/cstrn identifier
   int domain_flag,        // in : 0=dpt in orig_dmod_space,
                           //      1=dpt in unit_space,
                           //      2=dpt in internal_pfunc_space.
   double *dpt,            // in : new dpt for the target, [pre-alloced]
                           //      sized:[DM_get_domain_dim(rtn_err, dmod)]
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod
/**
* Gets the tag's current <i>xyz</i> location and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies <i>p0</i> of the active deformable model.
* <br><br>
* Searches for a constraint, load, or control point with a matching tag. When
*<dd>found, this function loads its current xyz</dd> position into <i>p0</i> and sets the active
* deformable model. This works for any tag object which is associated with one or
* more point locations.
* <br><br>
* For <tt>vector_loads</tt>, when the <tt>pt_index</tt> equals <tt>DM_BASE_PT</tt>, this function returns the
* <tt>vector_load</tt>'s base point and when <tt>pt_index</tt> equals <tt>DM_END_LEG</tt> returns the load's
* <tt>end_pt</tt> (location of the tip of the <tt>tang_vector</tt> for that load vector).
* <br><br>
* For point constraints, returns the point location of the constraint's base point
* or one of its vector end points as selected by the <tt>pt_index</tt> value. Known
* pt_index values for point constraints include:
* <br><br>
* <tt>pt_index</tt> = <tt>DM_BASE_PT</tt> for the point's <tt>base_point</tt>.
* <br><br>
* <tt>pt_index</tt> = <tt>DM_TANG_LEG</tt> for a curve's tangent end point.
* <br><br>
* <tt>pt_index</tt> = <tt>DM_TANG1_LEG</tt> for a surface's tang1 end point.
* <br><br>
* <tt>pt_index</tt> = <tt>DM_TANG2_LEG</tt> for a surface's tang2 end point.
* <br><br>
* <tt>pt_index</tt> = <tt>DM_NORM_LEG</tt> for a curve or surface normal vector end point.
* <br><br>
* <tt>pt_index</tt> = <tt>DM_CURV_LEG</tt> for a curve's curvature end point.
* <br><br>
* <tt>pt_index</tt> = <tt>DM_CURV1_LEG</tt> for a surface's curv1 end point.
* <br><br>
* <tt>pt_index</tt> = <tt>DM_CURV2_LEG</tt> for a surface's curv2 end point.
* <br><br>
* <tt>pt_index</tt> = <tt>DM_BINORM_LEG</tt> for a curve's binormal end point.
* <br><br>
* For curve constraints this function returns the last pick point (that is the
* last location at which the functions <tt>DM_find_tag_by_image_line()</tt> or
* <tt>DM_find_pos_by_image_line()</tt> queried the deformable model).
* <br><br>
* For pressure point loads returns the load's image point. For springs returns the
* spring's <tt>free_pt</tt> location. For spring sets uses <tt>pt_index</tt> to select which spring
* <tt>free_pt</tt> to return. Locations for control points within the active deformable
* model can be queried by setting the tag value to less than or equal to <tt>-500</tt>. For
* all other tag types returns <tt>DM_NO_XYZ_PT_FOR_TAG_OBJ</tt>.
* <br><br>
* <b>Errors:</b><dl><dt><tt>DM_NO_ROOT_DMOD</tt></dt>
* <dd>The root deformable model cannot be NULL on input.</dd>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The value of P0 or p1 cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>The input tag cannot identify a deformable model, a load, a spring, a spring set,
* an attractor object, or a constraint in the model hierarchy.</dd>
*<dt><tt>DM_NO_XYZ_PT_FOR_TAG_OBJ</tt></dt>
* <dd>The identified tag object has no distinct xyz point to report <tt>dyn_loads</tt>,
*<tt>dist_press</tt>, <tt>crv_loads</tt> and <tt>crv_cstrns</tt>, or deformable model.</dd>
*<dt><tt>DM_BAD_PT_INDEX_VALUE</tt></dt>
*<dd>The <tt>spring_set</tt> and <tt>pt_index</tt> must be within the point count range.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of target dmod hierarchy to search.
* @param tag
* in: load/cstrn identifier.
* @param pt_index
* in: index of tag's image_pt to update one of DM_BASE_PT DM_END_LEG DM_TANG_LEG DM_TANG1_LEG DM_TANG2_LEG DM_NORM_LEG DM_CURV1_LEG DM_CURV2_LEG DM_BINORM_LEG or a control point tag or an id number in a spring set.
* @param p0
* ut: tag's current xyz location sized:[image_dim].
* @param type
* out: set to type of object queried or DS_tag_none.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM void
DM_get_pt_xyz              // get: tag's current xyz location
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   int tag,                // in : load/cstrn identifier
   int pt_index,           // in : index of tag's image_pt to update
                           //      for pt-cstrns and SPAvector-loads
                           //      one of DM_BASE_PT
                           //             DM_END_LEG
                           //             DM_TANG_LEG
                           //             DM_TANG1_LEG
                           //             DM_TANG2_LEG
                           //             DM_NORM_LEG
                           //             DM_CURV_LEG
                           //             DM_CURV1_LEG
                           //             DM_CURV2_LEG
                           //             DM_BINORM_LEG
                           //      or a control point tag index,
                           //      or an id number in a spring_set.
   double *p0,             // out: tag's current xyz location, [pre-alloced]
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
   DS_TAGS &type,          // out: set to type of object queried or
                           //      DS_tag_none
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod

/**
* Sets the image space point for a constraint or control point.
* <br><br>
* <b>Role:</b> This function sets the location for an <tt>image_point</tt> or a control
* point of the tag object identified by the input tag value.
* <br><br>
* When p1 is a reference to a <tt>NULL</tt> pointer, the tag's image point is set to the
* value p0. To help support mouse pick-and-drag, this subroutine can be used to
* compute the tag's new image point location from an image space line. The image
* space line or iline, is specified as the line which runs through <i>p0</i> and <i>p1</i>.
* Usually this line will be generated from the window system's pick-ray and
* pick-point for a user-generated pick event. Four different new point
* calculations are supported based on the <tt>dir_flag</tt>, <tt>pt_index</tt> and <tt>tag_type</tt> values.
* <br><br>
* When <tt>pt_index</tt> equals <tt>DM_BASE_PT</tt> and <tt>dir_flag</tt> equals <tt>1</tt>, the new image point is
* the intersection between the image line and the plane parallel to the xy axis
* that contains the current image point location. This is good for dragging a
* point in its <i>xy</i> plane.
* <br><br>
* When <tt>pt_index</tt> equals <tt>DM_BASE_PT</tt> and <tt>dir_flag</tt> equals <tt>2</tt>, the new image point is
* the point on a line parallel to the <i>z</i> axis containing the current image point
* location that is nearest to the input image line. This is good for dragging a
* point along its <i>z</i> axis.
* <br><br>
* For objects with vectors (such as tangent point constraints and vector loads)
* the input <tt>pt_index</tt> value specifies whether the vector's base point or one of the
* end points is moved. The object's vector runs from the vector's base point to
* its end point.
* <br><br>
* When <tt>pt_index</tt> equals <tt>DM_BASE_PT</tt>, the vector's base point and end point are both
* modified so that the vector's magnitude and direction remains constant while its
* position is modified. When <tt>pt_index</tt> equals any option that specifies an end
* point, e.g., <tt>DM_TANG1_LEG</tt>, the vector's end point is modified while the base
* point remains fixed. The vector's position remains constant while its magnitude
* and direction are modified.
* <br><br>
* When the <tt>pt_index</tt> corresponds to a vector end point and <tt>dir_flag</tt> equals <tt>1</tt>, the
* new image position of the end point is the intersection of the input image line
* and a hemisphere centered on the point constraint's base point with a radius
* equal to the current tangent vector's magnitude. This is good for rotating a
* vector by dragging.
* <br><br>
* When <tt>pt_index</tt> corresponds to one of the vector end points and <tt>dir_flag</tt> equals <tt>2</tt>,
* the new image position of the end point is the point on a line running through
* the vector's base point and end point closest to the input image line. This is
* good for changing the magnitude of a vector by dragging.
* <br><br>
* Tag numbers less than <tt>-500</tt> are used to change control point locations. When
* moving a control point location, the control point on the active deformable
* model is moved.
* <br><br>
* When <tt>pt_index</tt> equals <tt>DM_CURV1_LEG</tt> or <tt>DM_CURV2_LEG</tt> the vector is constrained to
* remain perpendicular to the point constraint's tangent vector. In this case the
* dir_flag equals 1 option will only rotate the vector in the plane perpendicular
* to the tangent vector.
* <br><br>
* When <tt>pt_index</tt> equals <tt>DM_TANG1_LEG</tt> or <tt>DM_TANG2_LEG</tt> and the object being
* manipulated is a pt_cstrn then the point constraint's <tt>DM_CURV1_LEG</tt> and
* <tt>DM_CURV2_LEG</tt> values are updated to keep the curvature vector perpendicular to
* the tangent and to keep the curvature value a constant. The osculating plane
* will be reoriented in an unspecified, but minimal, fashion. Modify the
* <tt>DM_CURV1_LEG</tt> or <tt>DM_CURV2_LEG</tt> plane as desired.
* <br><br>
* When <tt>dir_flag</tt> equals <tt>0</tt>, no constraints are applied to the input parameter and it
* is the responsibility of the calling program to make supply a valid position, e.g., the normal vector
* is perpendicular to the tangent and so on.
* <br><br>
* To summarize, for tag objects with vectors and a <tt>pt_index</tt> equal to <tt>1</tt>:
* <br><br>
* <table border="0" width="100%">
* <tr>
* <td width="33%" align="left">dir_flag == 1</td>
* <td width="67%" align="left">Sets the point closest to the pick ray on a base point centered
* hemisphere (for rotation).</td>
* </tr>
* <tr>
* <td width="33%" align="left"> dir_flag == 2</td>
* <td width="67%" align="left"> Sets the point closest to the pick ray on the base point, end
* point line <tt>(base_pt</tt>, <tt>end_pt</tt>) line (for scaling).</td>
* </tr>
*</table>
*<br><br>
* For cases where <tt>pt_index</tt> is equal <tt>DM_BASE_PT</tt>, or the index for a <tt>spring_set</tt> or a
* control point tag:
* <br><br>
* <table border="0" width="100%">
* <tr>
* <td width="33%" align="left">dir_flag == 1</td>
* <td width="67%" align="left">Sets the point at the intersection of the pick ray and the image
* point's xy plane (move in <i>xy</i> plane).</td>
* </tr>
* <tr>
* <td width="33%" align="left"> dir_flag == 2</td>
* <td width="67%" align="left"> Sets the point closest to the pick ray on the image point's
* <tt>z_vec</tt> line (move in <i>z</i> direction).</td>
* </tr>
*</table>
*<br><br>
* These four cases are intended to allow an application interface to move an xyz
* point so that it tracks a <tt>2D</tt> mouse location and moves in three dimensional space
* in a reasonable fashion.
* <br><br>
* <i><b>Note:</b></i>
* <br><br>
* - Use <tt>DM_END_LEG</tt> for the vector loads,
* <br><br>
* - Use <tt>DM_TANG_LEG</tt> for the pt_cstrn on a curve tangent <tt>end_pt</tt>,
* <br><br>
* - Use <tt>DM_TANG1_LEG</tt> for the pt_cstrn on a surface's first tangent <tt>end_pt</tt>,
* <br><br>
* - Use <tt>DM_TANG2_LEG</tt> for the pt_cstrn on a surface's second tangent <tt>end_pt</tt>,
* <br><br>
* - Use <tt>DM_NORM_LEG</tt> for the pt_cstrn on a surface's normal vector <tt>end_pt</tt>,
* <br><br>
* - Use <tt>DM_CURV_LEG</tt> for the pt_cstrn on a curve's curvature vector <tt>end_pt</tt>,
* <br><br>
* - Use <tt>DM_CURV1_LEG</tt> for the pt_cstrn on a surface's first curvature <tt>end_pt</tt>,
* <br><br>
* - Use <tt>DM_CURV2_LEG</tt> for the pt_cstrn on a surface's second curvature <tt>end_pt</tt>,
* <br><br>
*
* - Use <tt>DM_BINORM_LEG</tt> for the <tt>pt_cstrn</tt> on a curve's binormal vector <tt>end_pt</tt>.
* <br><br>
* Setting the curvature <tt>constraint_value</tt> from the <tt>image_pt</tt> when constraining a
* surface <tt>(domain_dim</tt> = 2) is ambiguous. There are two solutions for <i>k</i> and <i>n</i> which
* satisfy the relation,
* <pre> curv_pt = k * n and curv_pt = -k *-n </pre>
* The first solution, <i>k</i> positive, will have a surface concave in the direction of
* the surface normal <i>n</i>. The second solution, <i>k</i> negative, will have a surface
* convex in the direction of the surface normal n. For curves, <i>k</i> is always
* positive. When setting the <tt>curv_pt</tt> location directly <tt>(dir_flag</tt> = <tt>0</tt>) one must
* tell the library which case to use. Otherwise, it chooses to preserve the sign
* of the curvature when rotating the curvature vec <tt>(dir_flag</tt> = <tt>1</tt>), and chooses to
* preserve the direction of the surface <tt>norm_val</tt> when lengthening the vector
* <tt>(dir_flag</tt> = <tt>2</tt>).
* <br><br>
* Many of a <tt>pt_cstrn</tt>'s constraint values are related. This function can update the
* <tt>pt_cstrn</tt>'s constraint values one at a time, or as a convenience cascade a change
* to other cstrn values to make sure they are all in a valid state. When
* <tt>cascade_flag</tt> == <tt>0</tt>, only the <tt>image_pt</tt> and its related <tt>cstrn_value</tt> indicated by
* the <tt>pt_index</tt> value will be updated. In this case the application will have to
* ensure that all the <tt>pt_cstrn</tt> value relationships are satisfied prior to calling
* <tt>DM_solve()</tt> otherwise a conflicting constraint may occur.
* <br><br>
* When <tt>cascade_flag</tt> = <tt>1</tt>, all related <tt>cstrn_vals</tt> will be modified by a minimum
* amount to preserve the <tt>cstrn_val</tt> relationships. The angle between tangent
* vectors for point constraints on surfaces will be preserved. When <tt>cascade_flag</tt>
* == <tt>2</tt>, all related <tt>cstrn_vals</tt> will be modified by a minimum amount, but the angle
* between tangent vectors for constraint points on surfaces is allowed to vary.
* <br><br>
* The <tt>pt_cstrn</tt> rules include the following. Only those rules that use properties
* which are being constrained by the <tt>pt_cstrn</tt>'s behavior (see
* <tt>DM_set_cstrn_behavior()</tt>) need to be enforced. Unconstrained properties will
* satisfy these rules after the next call to <tt>DM_solve()</tt>.
* <br><br>
* Rules enforced on the constraint values for <tt>DS_pt_cstrns</tt>.
* <br><br><ol>
*<li> for curves in <tt>2d</tt></li>
*<ol type = a>
*<li><tt>tang1_val</tt> perp to <tt>norm_val</tt></li>
* <li><tt>curv1_val</tt> not negative</li>
* <li><tt>norm_val</tt> is an unit vector</li></ol>
* <li> for curves in <tt>3d</tt></li>
* <ol type = a>
* <li><tt>tang1_val</tt> perp to <tt>norm_val</tt></li>
* <li> <tt>norm_val</tt> perp to <tt>binorm_val</tt></li>
* <li><tt>binorm_val</tt> perp to <tt>tang1_val</tt></li>
* <li> <tt>curv1_val</tt> not negative</li>
* <li><tt>norm_val</tt> is an unit vector</li>
* <li><tt>binorm_val</tt> is an unit vector</li></ol>
* <li> for surfaces</li>
* <ol type = a><li> <tt>domain1_dir</tt> is an unit vector</li>
* <li><tt>domain2_dir</tt> is an unit vector</li>
* <li><tt>domain1_dir</tt> not parallel to <tt>domain2_dir</tt></li>
* <li> <tt>tang1_val</tt> perp to <tt>norm_val</tt></li>
* <li> <tt>tang2_val</tt> perp to <tt>norm_val</tt></li>
* <li><tt>tang1_val</tt>, <tt>tang2_val</tt>, and <tt>norm_val</tt> make a right hand coordinate system
* (note depends <tt>domain1_dir</tt> and <tt>domain2_dir</tt>)</li>
* <li><tt>norm_val</tt> is an unit vector</li>
* <li> no restrictions on <tt>curv1_val</tt> or <tt>curv2_val</tt></li></ol></ol>
* <br>
* <b>Errors:</b><dl><dt><tt>DM_NO_ROOT_DMOD</tt></dt>
*<dd>The root deformable model cannot be <tt>NULL</tt> on input.</dd>
*<dt><tt>DM_BAD_DIR_FLAG_VALUE</tt></dt>
*<dd>The value of dir must be 0, 1, or 2.</dd>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>When the dir is <tt>1</tt> or <tt>2</tt>, <i>P1</i> cannot be <tt>NULL</tt> on entry.</dd>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The value of <i>P0</i> or <i>p1</i> cannot be <tt>NULL</tt> on entry</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>The input tag cannot identify a deformable model, a load, a spring, a spring set,
* an attractor object, or a constraint in the model hierarchy.</dd>
*<dt><tt>DM_NO_XYZ_PT_FOR_TAG_OBJ</tt></dt>
* <dd>The identified tag object has no distinct xyz point to report <tt>dyn_loads</tt>,
*<tt>dist_press</tt>, <tt>crv_loads</tt> and <tt>crv_cstrns</tt>, or deformable model.</dd>
*<dt><tt>DM_BAD_PT_INDEX_VALUE</tt></dt>
*<dd>The <tt>spring_set</tt> and <tt>pt_index</tt> must be within the point count range.</dd>
*<dt><tt>DM_PARALLEL_PROJECT_DIR</tt></dt>
* <dd>The the projection direction (i.e., the <i>z</i> axis, the <i>xy</i> plane, or the direction
* of a tangent vector) cannot be parallel to the image line. If executed, the
*projection would move the <tt>image_pt</tt> out to infinity.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of target dmod hierarchy to search.
* @param tag
* in: unique tag identifier to match.
* @param pt_index
* in: index of tag's image_pt to update one of DM_BASE_PT  DM_END_LEG  DM_TANG_LEG  DM_TANG1_LEG  DM_TANG2_LEG  DM_NORM_LEG  DM_CURV1_LEG  DM_CURV2_LEG  DM_BINORM_LEG or a control point tag or an id number in a spring set.
* @param p0
* new ipt or p0 of iline=p0+u*(p1-p0) [not-nested] sized:[image_dim].
* @param p1
* in: NULL or p1 of iline=p0+u*(p1-p0) [not-nested] sized:[image_dim].
* @param dir_flag
* in: 0=let image_pt equal P0, for pt_index == DM_BASE_PT 1=move parallel to xy plane, 2=move parallel to z axis. else 1=rotate current vector, 2=change vector magnitude.
* @param cascade_flag
* in: 2=move tangent vecs independently, but update normal vecs. 1=update related pt_cstrn values and image_pts. 0=don't.
* @param curvature_sign
* in: used when dir_flag ==0 && domain_dim==2 && PT_index==PST_CURV1 or PST_CURV2. +1: use curv_pt =  k * n,  (k positive), -1: use curv_pt =  -k * -n, (k negative), to solve for k and n vals.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM DS_TAGS            // rtn: modified object type or DS_tag_none
DM_set_pt_xyz              // eff: sets ipt in a cstrn or cpt
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   int tag,                // in : load/cstrn identifier
   int pt_index,           // in : index of tag's image_pt to update
                           //      for pt-cstrns and SPAvector-loads
                           //      one of DM_BASE_PT
                           //             DM_END_LEG
                           //             DM_TANG_LEG
                           //             DM_TANG1_LEG
                           //             DM_TANG2_LEG
                           //             DM_NORM_LEG
                           //             DM_CURV_LEG
                           //             DM_CURV1_LEG
                           //             DM_CURV2_LEG
                           //             DM_BINORM_LEG
                           //      or a control point tag index,
                           //      or an id number in a spring_set.
   double *p0,             // in : new ipt or p0 of iline=p0+u*(p1-p0), [pre-alloced]
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
   double *p1,             // in : NULL    or p1 of iline=p0+u*(p1-p0), [pre-alloced]
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
   int dir_flag,           // in : oneof
                           //      0=let image_pt = P0
                           //      for pt_index == DM_BASE_PT
                           //        1=move parallel to xy plane,
                           //        2=move parallel to z axis
                           //      else
                           //        1=rotate current SPAvector
                           //        2=change SPAvector magnitude
  int cascade_flag=1,      // in : 2=move tangent vecs independently
                           //        but update normal vecs
                           //      1=update related pt_cstrn
                           //        values and image_pts.
                           //      0=don't.
  int curvature_sign=0,    // out: used when dir_flag  ==0 &&
                           //                domain_dim==2 &&
                           //      pt_index==PST_CURV1 or PST_CURV2
                           //   +1: use curv_pt =  k *  n (k positive)
                           //   -1: use curv_pt = -k * -n (k negative)
                           //      to solve for new values k and n
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod

/**
* Gets the value for a point constraint's constraint behavior.
* <br><br>
* <b>Role:</b> This function gets the value for a point constraint's constraint
* behavior. Constraint values are all associated with a display point which can
* often be used for updating the constraint values in a more intuitive fashion.
* Refer to <tt>DM_get_pt_xyz()</tt> and <tt>DM_set_pt_xyz()</tt>.
* <br><br>
* The input arguments dmod and tag identify which point constraint to query. The
* argument <tt>pt_index</tt> selects which of the point constraint's behavior values to
* return. Accepted values for <tt>pt_index</tt> include:
* <br><br>
* <tt>DM_BASE_PT</tt> = set base point position. <tt>cstrn_val_count</tt> = <tt>Image_dim</tt>
* <br><br>
* <tt>DM_TANG_LEG</tt> or
* <br><br>
* <tt>DM_TANG1_LEG</tt> = set tangent vector in the <tt>domain1_dir</tt> direction.
* <tt>cstrn_val_count</tt> = <tt>Image_dim</tt>
* <br><br>
* <tt>DM_TANG2_LEG</tt> = set tangent vector in the <tt>domain2_dir</tt> direction.
* <tt>cstrn_val_count</tt> = <tt>Image_dim</tt>
* <br><br>
* <tt>DM_CURV_LEG</tt> or
* <br><br>
* <tt>DM_CURV1_LEG</tt> = set curvature value in the <tt>domain1_dir</tt> direction.
* <tt>cstrn_val_count</tt> = <tt>1</tt>
* <br><br>
* <tt>DM_CURV2_LEG</tt> = set curvature value in the <tt>domain2_dir</tt> direction.
* <tt>cstrn_val_count</tt> = <tt>1</tt>
* <br><br>
* <tt>DM_NORM_LEG</tt> = set normal vector direction for a curve or surface.
* <tt>cstrn_val_count</tt> = <tt>Image_dim</tt>
* <br><br>
* <tt>DM_BINORM_LEG</tt> = Set binormal vector direction for a curve. <tt>cstrn_val_count</tt> =
* <tt>Image_dim</tt>
* <br><br>
* The desired value for the specified constraint behavior is returned in the
* pointer, <tt>cstrn_val</tt>, which is a <tt>cstrn_val_count</tt> length array. The constraint
* behavior value will be an image space position, vector, or scalar value
* depending on which constraint behavior is being updated as indicated above.
* <br><br>
* Use <tt>DM_set_cstrn_value</tt> or <tt>DM_set_pt_xyz</tt> to modifying a constraint behavior value.
* These functions will cause the constraint's associated display point to be
* repositioned.
* <br><br>
* <b>Errors:</b><dl><dt><tt>DM_NO_ROOT_DMOD</tt></dt>
*<dd>The root deformable model cannot be <tt>NULL</tt> on input.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>The input tag does not identify any tag object in the model hierarchy.</dd>
*<dt><tt>DM_NOT_A_PT_CSTRN</tt></dt>
*<dd>the input tag does not identify a point constraint in the model hierarchy.</dd>
*<dt><tt>DM_BAD_PT_INDEX_VALUE</tt></dt>
*<dd>When <tt>pt_index</tt> is not an appropriate value.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of target dmod hierarchy to search.
* @param tag
* in: pt_cstrn identifier.
* @param pt_index
* in: index of tag's value to query one of: DM_BASE_PT DM_TANG_LEG DM_TANG1_LEG DM_TANG2_LEG DM_NORM_LEG DM_CURV1_LEG DM_CURV2_LEG DM_BINORM_LEG.
* @param cstrn_val_count
* out: size of cstrn_val.
* @param cstrn_val
* out: tag's current cstrn_val sized: [cstrn_val_count].
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_get_cstrn_value         // get: tag's constraint behavior value
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   int tag,                // in : pt_cstrn identifier
   int pt_index,           // in : index of tag's value to query
                           //     one of DM_BASE_PT
                           //            DM_TANG_LEG
                           //            DM_TANG1_LEG
                           //            DM_TANG2_LEG
                           //            DM_NORM_LEG
                           //            DM_CURV1_LEG
                           //            DM_CURV2_LEG
                           //            DM_BINORM_LEG
   int &cstrn_val_count,   // out: size of cstrn_val
   double *&cstrn_val,     // out: tag's current
                           //      cstrn_val
                           // sized:[cstrn_val_count]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Sets the value for a point constraint's constraint behavior.
* <br><br>
* <b>Role:</b> This function sets the value for a point constraint's behavior
* and modifies its related display point location. Constraint values are all
* associated with a display point which can often be used for updating the
* constraint values in a more intuitive fashion. Refer to <tt>DM_get_pt_xyz()</tt> and
* <tt>DM_set_pt_xyz()</tt>.
* <br><br>
* The input arguments dmod and tag identify which point constraint to modify. The
* argument <tt>pt_index</tt> selects which of the point constraint's behavior values to
* update. Accepted values for <tt>pt_index</tt> include:
* <br><br>
* <tt>DM_BASE_PT</tt> = set base point position. <tt>cstrn_val_count</tt> = <tt>Image_dim</tt>
* <br><br>
* <tt>DM_TANG_LEG</tt> or
* <br><br>
* <tt>DM_TANG1_LEG</tt> = set tangent vector in the <tt>domain1_dir</tt> direction.
* <tt>cstrn_val_count</tt> = <tt>Image_dim</tt>
* <br><br>
* <tt>DM_TANG2_LEG</tt> = set tangent vector in the <tt>domain2_dir</tt> direction.
* cstrn_val_count = <tt>Image_dim</tt>
* <br><br>
* <tt>DM_CURV_LEG</tt> or
* <br><br>
* <tt>DM_CURV1_LEG</tt> = set curvature value in the <tt>domain1_dir</tt> direction.
* <tt>cstrn_val_count</tt> = <tt>1</tt>
* <br><br>
* <tt>DM_CURV2_LEG</tt> = set curvature value in the <tt>domain2_dir</tt> direction.
* <tt>cstrn_val_count</tt> = 1
* <br><br>
* <tt>DM_NORM_LEG</tt> = set normal vector direction for a curve or surface.
* <tt>cstrn_val_count</tt> = <tt>Image_dim</tt>
* <br><br>
* <tt>DM_BINORM_LEG</tt> = Set binormal vector direction for a curve. <tt>cstrn_val_count</tt> =
* <tt>Image_dim</tt>
* <br><br>
* The desired value for the selected constraint behavior is passed in through the
* pointer, <tt>cstrn_val</tt>, which is treated as a <tt>cstrn_val_count</tt> length array. The
* constraint behavior value will be an image space position, vector, or scalar
* value depending on which constraint behavior is being updated as indicated above.
* <br><br>
*
* Modifying a constraint behavior value will cause the associated constraint's
* display point to be repositioned.
* <br><br>
* This function is provided to simplify accessing point constraint internal values
* directly by an application. The deformable modeling library requires that
* certain relationships be maintained by the these internal values. It is the
* responsibility of the calling application to ensure that these relationships are
* satisfied prior to the next call to <tt>DM_solve()</tt>. Otherwise, conflicting
* constraints may be found. These requirements can be automatically met by using
* the function <tt>DM_set_pt_xyz()</tt> which can automatically change some constraint
* values to preserve these relationships.
* <br><br>
* The <tt>pt_cstrn</tt> constraint relationships include the following. Only those rules
* that use properties which are being constrained by the pt_cstrn's behavior (see
* <tt>DM_set_cstrn_behavior</tt>) need to be enforced. Unconstrained properties will
* satisfy these rules after the next call to <tt>DM_solve()</tt>.
* <br><br>
* Rules enforced on the constraint values for <tt>DS_pt_cstrns</tt> as follows:
* <br><br>
* <b>For curves in 2d:</b>
* <br><br>
* <li> <tt>tang1_val</tt> perp to <tt>norm_val</tt> <br></li>
* <li><tt>curv1_val</tt> not negative <br></li>
* <li> <tt>norm_val</tt> is an unit vector</li>
* <br><br>
* <b>For curves in <tt>3d</tt>:</b>
* <br><br>
* <li><tt>tang1_val</tt> perp to <tt>norm_val</tt>><br>
* <li><tt>norm_val</tt> perp to <tt>binorm_val</tt>><br>
* <li><tt>binorm_val</tt> perp to <tt>tang1_val</tt>><br>
* <li><tt>curv1_val</tt> not negative<br>
* <li><tt>norm_val</tt> is an unit vector<br>
* <li><tt>binorm_val</tt> is an unit vector
* <br><br>
* <b>For surfaces:</b>
* <br><br>
* <li><tt>domain1_dir</tt> is an unit vector<br></li>
* <li><tt>domain2_dir</tt> is an unit vector<br></li>
* <li><tt>domain1_dir</tt> not parallel to <tt>domain2_dir</tt><br></li>
* <li><tt>tang1_val</tt> perp to <tt>norm_val</tt><br></li>
* <li><tt>tang2_val</tt> perp to <tt>norm_val</tt><br></li>
* <li><tt>tang1_val</tt>, <tt>tang2_val</tt>, and <tt>norm_val</tt> make a right hand coordinate system
* (note depends <tt>domain1_dir</tt> and <tt>domain2_dir</tt>)<br></li>
* <li><tt>norm_val</tt> is unit vector<br></li>
* <li>no restrictions on <tt>curv1_val</tt> or <tt>curv2_val</tt></li>
* <br><br>
* <b>Errors:</b><dl><dt><tt>DM_NO_ROOT_DMOD</tt></dt>
*<dd>The root deformable model cannot be <tt>NULL</tt> on input.</dd>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>When <tt>cstrn_val</tt> is NULL on entry.</dd>
* <tt>DM_TAG_OBJECT_NOT_FOUND</tt>
* The input tag does not identify any tag object in the model hierarchy.
* <tt>DM_NOT_A_PT_CSTRN</tt>
* The input tag does not identify a point constraint in the model hierarchy.
*<dt><tt>DM_BAD_PT_INDEX_VALUE</tt></dt>
*<dd>When <tt>pt_index</tt> is not an appropriate value.</dd>
*<dt><tt>DM_BAD_CURVATURE_VALUE</tt></dt>
*<dd>When input curvature value for a curve is not zero or positive.</dd>
*<dt><tt>DM_BAD_CSTRN_VAL_SIZE</tt></dt>
*<dd>When input <tt>cstrn_val_count</tt> is not appropriate for the selected constraint
* behavior.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of target dmod hierarchy to search.
* @param tag
* in: unique tag identifier to match.
* @param pt_index
* in: index of tag's image_pt to update one of DM_BASE_PT  DM_TANG_LEG  DM_TANG1_LEG  DM_TANG2_LEG  DM_NORM_LEG  DM_CURV1_LEG  DM_CURV2_LEG  DM_BINORM_LEG.
* @param cstrn_val_count
* in: size of cstrn_val array.
* @param cstrn_val
* in: value to assign to constraint behavior. sized: [cstrn_val_count].
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM void
DM_set_cstrn_value         // eff: sets ipt in a cstrn or cpt
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   int tag,                // in : load/cstrn identifier
   int pt_index,           // in : index of tag's image_pt to update
                           //      for pt-cstrns and SPAvector-loads
                           //      one of DM_BASE_PT
                           //             DM_TANG_LEG
                           //             DM_TANG1_LEG
                           //             DM_TANG2_LEG
                           //             DM_NORM_LEG
                           //             DM_CURV_LEG
                           //             DM_CURV1_LEG
                           //             DM_CURV2_LEG
                           //             DM_BINORM_LEG
   int cstrn_val_count,    // in : size of cstrn_val array
   double *cstrn_val,      // in : value to assign to
                           //      selected constraint behavior
                           // sized:[cstrn_val_count]
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod

/**
* Forces a curve or link constraint to update its internally stored data after the constraint's source points have been modified.
* <br><br>
* <b>Role:</b> This function helps to enable curve tracking. It causes a curve
* constraint or link constraint to rebuild its constraint equations from the
* constraint's current source point definitions.
* <br><br>
* This function must be used for curve tracking. Create a curve or link constraint
* using a function to describe the target shape. This can be done by either
* building the constraint with a callback function, <tt>src_CW_func()</tt>, or with a
* combination of the <tt>DS_pfunc</tt> objects, src_W_pfunc, src_Wn_pfunc, and
* <tt>src_Wnn_pfunc</tt>. Modify the source shape. For example, the <tt>src_pfunc</tt> <tt>DS_pfunc</tt>
* objects can themselves be shaped as deformable curves. After the source shape is
* modified call this function to have the constraint update its internal equations
* so that the next call to <tt>DM_solve()</tt> will cause the constrained shape to deform
* to the new source shape.
* <br><br>
* This routine will set the constraint tight state on.
* <br><br>
* Refer to <tt>DM_add_crv_cstrn()</tt> and <tt>DM_add_link_cstrn()</tt>.
* <br><br>
* The input arguments <tt>dmod</tt> and <tt>tag</tt> identify which constraint to update.
* <br><br>
* The basic flavor for using a constraint is:
* <br><br><ul>
* <li>Create and apply the constraint to a deformable model</li>
* <tt>(DM_add_pt_cstrn()</tt>), (<tt>DM_add_crv_cstrn()</tt>), <tt>(DM_add_link_cstrn())</tt>,
* <tt>(DM_add_area_cstrn())</tt>.
* <br><br>
* <li>Select which geometry properties are being constrained,</li>
* <tt>(DM_set_cstrn_behavior())</tt>.
* <br><br>
* <li>For tracking point constraints, modify the geometry property values by
* moving the point constraint display points as described <tt>above,(DM_set_pt_xyz())</tt>.</li>
* <br><br>
* <li>For tracking curve constraints and link constraints, use the src_W_pts,
* src_Wn_pts, and/or src_Wnn_pts, update their shapes, and ask this deformable
* model to track the change with <tt>(DM_update_cstrn_src_pts())</tt>.</li>
* <br><br>
* <li>Call solve to see how the constraint modifications changed the
* deformable model shape, <tt>(DM_solve())</tt>.</li>
* <br><br>
* <li>Optionally, for point constraints, the point constraint's domain
* position and domain directions (for surfaces) may be modified with
* <tt>(DM_set_pt_uv())</tt> or <tt>(DM_set_cstrn_pttan_uv_dir())</tt>.</li>
* <br><br>
* <li>Optionally, modify the constraint rendering and image_pt locations to
* optimize viewing and/or interactions, <tt>DM_set_tan_display_gain()</tt></li>
* <tt>DM_set_comb_graphics()</tt>.</ul>
* <br>
* <b>Errors:</b><dl><dt><tt>DM_NO_ROOT_DMOD</tt></dt>
*<dd>The root deformable model cannot be <tt>NULL</tt> on input.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>The input tag does not identify any object in the model hierarchy.</dd>
*<dt><tt>DM_NOT_A_CRV_LINK_CSTRN</tt></dt>
* <dd>The input tag does not identify a crv_cstrn or link_cstrn constraint in the
*model hierarchy.</dd></dl>
* <br>
* <b>Effect:</b> System routine
* <br><br>
* @param rtn_err
* out: 0=success.
* @param dmod
* or negative err code.
* @param tag
* in: member of target.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM void
DM_update_cstrn_src_pts    // eff: refresh cstrn to track src_pts changes
  (int &rtn_err,           // out: 0=success
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   int tag,                // in : cstrn identifier
   SDM_options *sdmo=NULL);// in : SDM_options pointer

//============================================================
// Tag Management
//============================================================

//============================================================
// DM_get_tag_summary, DM_get_sibling_list,
// DM_rm_tag_object,   DM_assign_next_tag,
// DM_get_tag_count,   DM_set_tag_count,
// DM_classify_tag,    DM_parse_tag_flag
//============================================================

/**
* Gets the build and return tag summary list and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies the tag count. This function builds and returns a list
* containing the tag identifier and tag type for all tag objects in the target
* deformable model. It loads the <tt>tag_count</tt> with the number of tag objects on the
* deformable model. The tag summary list data is always ordered as:
* <br><br>
* <tt>[id0, type0] =</tt> target deformable model
* <tt>[id1, type1] = </tt>Parent deformable model
* <tt>[id2, type2] = </tt>Sibling deformable model
* <tt>[id3, type3] =</tt> Child deformable model
* <tt>[id4, type4] = </tt>ordered list of loads and constraints
* <tt>[..., ... ]  = </tt>tag identifiers and types
* <br><br>
* The first eight entries of the tag summary are always hierarchy data. When any
* of the hierarchy pointers are <tt>NULL</tt> their corresponding identifiers are set to <tt>-1</tt>,
* and the tag type is set to <tt>DS_tag_none</tt>. All tag types are defined in the
* <tt>DS_TAGS</tt> enumeration but are stored in the tag summary as ints.
* <br><br>
* Includes <tt>malloc</tt> the calling program must delete the tag summary array when it
* is finished.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_NON_NULL_OUTPUT_PTR</tt></dt>
*<dd>The tag summary must be <tt>NULL</tt>.</dd>
*<tt>DM_parse_tag_flag()</tt> errors</dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod to query.
* @param tag_count
* out: number of tag objects.
* @param tag_summary
* out: tag-object's (tag-id tag-type) [id0, type0, id1, type1, ...].
* @param sdmo
* in:SDM_options pointer  note: rtn_size: [2*tag_count] mallocs: caller must free tag_summary array note: sets active dmod.
**/
DECL_DM void
DM_get_tag_summary         // eff: build and return tag_sumamry list
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to query, [pre-alloced]
   int &tag_count,         // out: number of tag objects
   int *&tag_summary,      // out: tag-object's (tag-id tag-type), [NULL-entry]
                           //      [id0, type0, id1, type1,...]
                           //      sized:[2*tag_count] on exit
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // mallocs: caller must free
                           //          tag_summary array
                           // note: sets active dmod

/**
* Builds and returns a root sibling list.
* <br><br>
* <b>Role:</b> Builds and returns a list containing the tag identifier for all
* root siblings of the target deformable model's deformable modeling hierarchy. It
* sets the sibling count to agree with the number of root siblings.
* <br><br>
* The sibling list data is always ordered as:
* <br><br>
* <tt>[id0]</tt> = root dmod
* <br><br>
* <tt>[id1]</tt> = 1st sibling dmod
* <br><br>
* <tt>[id2]</tt> = 2nd sibling dmod
* <br><br>
* <tt>[...]</tt>
* <br><br>
* <tt>Mallocs</tt>: The calling program must delete the sibling list array when it is done
* with it.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on input.</dd>
*<dt><tt>DM_NON_NULL_OUTPUT_PTR</tt></dt>
*<dd>The sibling list cannot be NULL on entry.</dd>
*<tt>DM_parse_tag_flag()</tt> errors</dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: member of dmod hierarchy.
* @param sibling_count
* number of root level siblings.
* @param sibling_list
* sibling-id's (tag-id) [id0, id1, ...] sized:[sibing_count].
* @param sdmo
* in:SDM_options pointer  mallocs: caller must free sibling_list array note: sets active dmod.
**/
DECL_DM void
DM_get_sibling_list        // eff: build and return root's sibling_list
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   int &sibling_count,     // out: number of root level siblings
   int *&sibling_list,     // out: sibling_list (tag-id0 tag-id1), [NULL-entry]
                           //      sized:[sibling_count]
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // mallocs: caller must free
                           //          sibling_list array
                           // note: sets active dmod

/**
* Removes a deformable modeling tag object from a deformable modeling hierarchy.
* <br><br>
* <b>Role:</b> Finds, removes, and deletes the tag object, identified by the
* input tag value, from the input dmod's deformable modeling hierarchy. When a tag
* object is found, the function removes it from the deformable modeling hierarchy,
* deletes it, sets <tt>rtn_err</tt> value to <tt>0</tt>, and returns the type of the tag object that
* was removed. Otherwise, it sets <tt>rtn_err</tt> to a negative error code and returns a
* value of <tt>DS_tag_none</tt>.
* <br><br>
* This function removes constraints, loads, and descendant patches from a
* deformable modeling hierarchy. It can not remove a root sibling deformable model.
* An entire list of root siblings may be deleted by a call to <tt>DM_delete_dmod()</tt>.
* <br><br>
* A single root_sibling can be isolated and deleted by calling this function on
* every link_constraint that connects the root_sibling in question to its
* deformable modeling neighbors and then calling the <tt>DM_delete_dmod()</tt> function on
* the isolated <tt>root_sibling</tt>.
* <br><br>
* When deleting link constraints, it is possible to eliminate the last link
* between two sets of siblings. When this happens, all the internal sibling
* pointers are modified to separate the decoupled sets of siblings into two
* distinct deformable modeling hierarchies. The output argument <tt>detached_sibling</tt>
* is set to point to the root of the deformable modeling hierarchy which does not
* contain the input deformable model. The calling application must take care to
* manage the <tt>detached_sibling</tt> list since those objects are no longer a part of
* this hierarchy.
* <br><br>
* When deleting the specified tag object does not detach a sibling dmod, the
* output argument detached_sibling is set to <tt>NULL</tt>.
* <br><br>
* When <tt>deletable_flag</tt> == <tt>1</tt>, only those constraints which are deletable are deleted
* as determined by the call <tt>DM_get_cstrn_rights</tt>. When <tt>deletable_flag</tt> == <tt>0</tt>, any
* constraint will be deleted.
* <br><br>
* When any error is encountered, the <tt>rtn_err</tt> value is set to a negative error code
* and a value of <tt>DS_tag_none</tt> is returned.
* <br><br>
* <b>Errors:</b><dl><dt><tt>DM_NO_ROOT_DMOD</tt></dt>
*<dd>The root deformable model cannot be <tt>NULL</tt> on input.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>The input tag cannot identify a deformable model, a load, a spring, a spring set,
* an attractor object, or a constraint in the model hierarchy.</dd>
*<dt><tt>DM_CANT_RM_ROOT_DMOD</tt></dt>
* <dd>This function cannot be used to attempt to remove the root deformable model of a
*deformable model hierarchy.</dd>
*<dt><tt>DM_UNDELETABLE_CSTRN</tt></dt>
*<dd>The identified constraint cannot be set to undeletable.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of target dmod hierarchy to search.
* @param tag
* in: unique tag identifier to match.
* @param detached_dmod
* out: ptr to detached sibling or NULL.
* @param deletable_flag
* 1=only rm DM_DELETABLE cstrns, 0=remove any cstrn.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM DS_TAGS            // rtn: type of object deleted
DM_rm_tag_object           // eff: remove a dmod, load, or cstrn by tag
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   int tag,                // in : tag object identifier
   DS_dmod *&detached_dmod,// out: ptr to detached sibling or NULL, [malloc]
   int deletable_flag=1,   // in : 1=only rm DM_DELETABLE constraints
                           //      0=rm any constraint
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active_dmod

/**
* Increments and returns the tag count and the next available tag number or an error.
* <br><br>
* <b>Role:</b> Increments and returns the value of <tt>dmo_tag_count</tt>. Use this
* value as the tag value for the next tag object being created. If you always use
* this function to select the tag value for each tag object entered into a
* deformable model tag hierarchy, then every tag object in the hierarchy will have
* a unique value.
* <br><br>
* Returns a positive tag identifier number.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> System routine
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: dmod member of a hierarchy/ tree.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM int                // rtn: next tag number or neg err code
DM_assign_next_tag         // eff: incrment and rtn tag count
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod member of a hierarchy, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer
/**
* Gets the deformable model's <tt>dmo_tag_count</tt> value and returns the largest tag value currently assigned.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: dmod member of a hierarchy.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM int                // rtn: largest tag value currently assigned
                           //      or negative err code
DM_get_tag_count           // eff: return dmod's dmo_tag_count value
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod member of a hierarchy, [pre-alloced]
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Sets the deformable model's <tt>dmo_tag_count</tt> value and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies deformable <tt>model->root->dmo_tag_count</tt>
* <br><br>
* Sets the value of the largest currently assigned tag value for the deformable
* model hierarchy. Be sure that this number will generate unique tag values for
* new tag objects being added to the current hierarchy. The system assumes each
* tag identifier for every tag object is unique.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: target dmod to modify.
* @param tag_count
* largest tag value used by a tag object in the dmod hierarchy.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_set_tag_count           // eff: set dmod's dmo_tag_count value
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : target dmod, [pre-alloced]
   int tag_count,          // in : largest tag value used by a tag
                           //      object in the dmod hierarchy
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Checks the deformable model tree for a tag.
* <br><br>
* <b>Role:</b> Searches all the patch deformable models, constraints, loads,
* and control points for one that has the input tag value. When the tag value is
* found it returns the <tt>DS_tag_type</tt> enumeration value for its type and sets the
* active deformable model pointer to be the deformable model owner of the
* identified tag object. When not found it returns <tt>DS_tag_none</tt>
* <br><br>
* Returns the <tt>DS_tag_type</tt> for the tag or <tt>DS_tag_none</tt>. Type values are:
* <br><br>
* <tt>DS_tag_none</tt> = Value for no such tag object
* <br><br>
* <tt>DS_tag_control_pt</tt> = Surface control points
* <br><br>
* <tt>DS_tag_pt_press</tt> = Point pressures
* <br><br>
* <tt>DS_tag_dist_press</tt> = Distributed pressure
* <br><br>
* <tt>DS_tag_spring</tt> = Spring load
* <br><br>
* <tt>DS_tag_spring_set</tt> = Ordered point set load
* <br><br>
* <tt>DS_tag_crv_load</tt> = Curve spring load
* <br><br>
* <tt>DS_tag_dyn_load</tt> = Dynamic load
* <br><br>
* <tt>DS_tag_pt_cstrn</tt> = Point constraint
* <br><br>
* <tt>DS_tag_crv_seam [Base DS_cstrn]</tt> = Seam point constraint
* <br><br>
* <tt>DS_tag_crv_cstrn [Base DS_cstrn]</tt> = Curve constraint
* <br><br>
* <tt>DS_tag_crv_seam [Base DS_cstrn]</tt> = Seam curve constraint
* <br><br>
* <tt>DS_tag_srf_dmod [Base DS_dmod</tt>] = Surface deformable model
* <br><br>
* <tt>DS_tag_crv_dmod [Base DS_dmod</tt>] = Curve deformable model
* <br><br>
* <tt>DS_tag_vector_load [Base DS_load]</tt> = Vector load
* <br><br>
* <tt>DS_tag_attractor [Base DS_load]</tt> = Attractor load
* <br><br>
* <tt>DS_tag_link_cstrn [Base DS_cstrn]</tt> = Multi-surf link constraint
* <br><br>
* <tt>DS_tag_area_cstrn [Base DS_cstrn]</tt> = Isolated deformation region
* <br><br>
* <tt>DS_tag_link_load [Base DS_cstrn]</tt> = Multi-surf link load
* <br><br>
* <tt>DS_tag_track_pos_crv</tt>
* <br><br>
* <tt>[Base DS_dmod</tt>] = tracking position curve dmod
* <br><br>
* <tt>DS_tag_track_tan_crv</tt>
* <br><br>
* <tt>[Base DS_dmod</tt>] = tracking tangent curve dmod
* <br><br>
* <tt>DS_tag_track_curv_crv</tt>
* <br><br>
* <tt>[Base DS_dmod</tt>] = tracking curvature curve dmod
* <br><br>
* <b>Errors:</b><dl><dt><tt>DM_NON_NULL_OUTPUT_PTR</tt></dt>
*<dd>pfunc must be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param dmod
* in: member of target dmod hierarchy to search.
* @param tag
* in: src tag number.
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/
DECL_DM DS_TAGS            // rtn: DS_tag_type for tag or DS_tag_none
DM_classify_tag            // eff: search dmod tree for tag
  (int &rtn_err,           // out: 0=success or negative err code
   DS_dmod *dmod,          // in : member of dmod hierarchy, [pre-alloced]
   int tag,                // in : src tag number
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod
/**
* Converts a <tt>tag_flag</tt> identifier into a deformable model pointer and a <tt>walk_flag</tt> value.
* <br><br>
* <b>Role:</b> Modifies <tt>walk_flag</tt>, returns pointer to identified deformable
* model.
* <br><br>
* <tt>tag_flag</tt> identifies the target deformable model as follows. When <tt>tag_flag</tt> is
* equal to <tt>+1</tt> or <tt>-1</tt>, the target is the active deformable model. When <tt>tag_flag</tt> is
* equal to <tt>+2</tt> or <tt>-2</tt>, the target is the root deformable model. When <tt>tag_flag</tt> is
* equal to some other value, then the target is the deformable model that has the
* same tag identifier as the absolute value of the <tt>tag_flag</tt> value. When there is
* no deformable model whose tag identifier matches the absolute value of the
* <tt>tag_flag</tt> value, then there is no target and <tt>NULL</tt> is returned.
* <br><br>
* In summary:
* <br><br>
* <table border="0" width="100%">
* <tr>
* <td width="10%" align="left"><tt>+1 or -1</tt></td>
* <td width="90%" align="left">active deformable model</td>
* </tr>
* <tr>
* <td width="10%" align="left"><tt>+2 or -2</tt></td>
* <td width="90%" align="left">root deformable model</td>
* </tr>
* <tr>
* <td width="10%" align="left"><tt>+ or -num</tt></td>
* <td width="90%" align="left">deformable model with <tt>tag_id</tt> == <tt>|num|</tt><br></td>
* </tr>
*</table><br>
* The sign of the <tt>tag_flag</tt> value specifies the value of the <tt>walk_flag</tt>. <tt>walk_flag</tt>
* is a typical input required by all functions which are capable of walking the
* deformable modeling hierarchy recursively.
* Valid <tt>walk_flag</tt> values include:
* <br><br>
* <tt>0 =</tt> operate only on the target deformable model.<br>
* <tt>1 = </tt>operate only on the target deformable model and its offspring.<br>
* <tt>2 = </tt>operate on the target deformable model, its younger siblings, and all
* their offspring.
* <br><br>
* For multisurface models: When input <tt>tag_flag</tt> value is <tt>-2</tt> (root dmod and
* offspring) or when <tt>tag_flag</tt> is <tt>-1</tt> (the active deformable model and offspring)
* and the active deformable model is one of the root siblings, then the <tt>walk_flag</tt>
* value is set to <tt>2</tt> (target, siblings, and offspring), and the returned deformable
* model pointer is set to the oldest root sibling.
* <br><br>
* <b>Errors:</b> Returns <tt>NULL</tt> for any errors, and sets rtn_err to one of the following
* possible error values:
* <br><br>
*<dl><dt><tt>DM_DMOD_NOT_A_ROOT_DMOD</tt></dt>
*<dd>The deformable model must be the root of a hierarchy tree.</dd>
*<dt><tt>DM_BAD_TAG_FLAG_VALUE</tt></dt>
*<dd>The tag_flag cannot equal 0.</dd>
*<dt><tt>DM_NO_ACTIVE_DMOD</tt></dt>
*<dd>The input deformable model's hierarchy has no currently active deformable model.</dd>
*<dt><tt>DM_TAG_NOT_A_DMOD_MEMBER</tt></dt>
*<dd>The <tt>tag_flag</tt> does not identify a deformable model in the patch hierarchy.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of dmod hierarchy to search.
* @param tag_flag
* in: specify target dmod and how deep to go, 1=active, 2=root, else=member(tag= tag_flag) pos=tgt only, neg=tgt + offspring.
* @param walk_flag
* out: 0=tgt_only, 1=tgt and offspring, 2=tgt, siblings, and offspring.
* @param sdmo
* in:SDM_options pointer note: makes found tgt_dmod active.
**/
DECL_DM DS_dmod *          // rtn: ptr to target dmod, [pre-alloced]
DM_parse_tag_flag          // eff: turn tag_flag into dmod
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of hierarchy to search, [pre-alloced]
   int tag_flag,           // in : specify tgt dmod and how deep to go
                           //   1=active, 2=root, else=member(tag==tag_flag)
                           //   pos=tgt only, neg=tgt + offspring
   int &walk_flag,         // out: 0=tgt_only, 1=tgt and offspring
                           //      2=dmod,siblings,and offspring
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: makes found dmod active

// groupings of the DS_TAGS enum values

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM int                // rtn: 1=yes,0=no
DM_is_type_tracking_curve  // eff: check DS_TAGS enum type
  (int &rtn_err,           // out: 0=success or neg err code
   DS_TAGS ds_tags,        // in : DS_TAGS enum value to check
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM int                // rtn: 1=yes,0=no
DM_is_type_patch_dmod      // eff: check DS_TAGS enum type
  (int &rtn_err,           // out: 0=success or neg err code
   DS_TAGS ds_tags,        // in : DS_TAGS enum value to check
   SDM_options *sdmo=NULL);// in : SDM_options pointer

//====================================================
// interaction enabling functions
//====================================================

//====================================================
// DM_find_tag_by_image_line
//====================================================
/**
* Checks the entire hierarchy for the load, constraint, or control point closest to the input <tt>image_line</tt>.
* <br><br>
* <b>Role:</b> This function finds and returns the closest load, constraint, or
* control point tag object to an image space line.
* <br><br>
* The intent of this function is to enable mouse-based selection of tag objects.
* This works well when the image line (<i>p0</i> and <i>p1</i>) is defined by the mouse position
* and the viewing angle. Select p0 and p1 to be two points on the line which
* intersects the mouse cursor position and runs in the viewing direction. Make
* sure that <i>p0</i> is closer to the view than <i>p1</i>. The object selected by this routine
* will appear to be the one pointed to by the mouse.
* <br><br>
* Searches all the tag objects (pressure, constraints, control points, etc.) that
* are currently displayed (as recorded by deformable <tt>model->Draw_state()</tt>). The
* entire patch hierarchy is searched. The width of the search ray is given in
* image space units. Applications with access to window information will want to
* compute this distance from pixel size information. Usually five pixels is a good
* ray thickness. Only those tag objects within the search ray thickness are
* considered for selection.
* <br><br>
* Returns the tag of the tag object closest to the viewer (lowest image line <i>u</i>
* parameter value) within the bounded search volume. Loads and constraints have
* tag identifiers greater than <tt>0</tt>, and control points have tag identifiers less
* than or equal to <tt>-500</tt>.
* <br><br>
* <b>Errors:</b><dl><dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
* <dd>The input tag cannot identify a deformable model, a load, a spring, a spring set,
*an attractor object, or a constraint in the model hierarchy.</dd>
*<dt><tt>DM_NO_ROOT_DMOD</tt></dt>
*<dd>The root deformable model cannot be <tt>NULL</tt> on input.</dd>
* <dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The value of <i>P0</i> or <i>p1</i> cannot be <tt>NULL</tt> on entry</dd>
*<dt><tt>DM_BAD_MAX_DIST_VALUE</tt></dt>
*<dd>The <tt>max_dist</tt> must be larger than <tt>0.0</tt>.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: member of hierarchy to search.
* @param p0
* in: p0 of iline = p0 + u*(p1-p0) sized:[image_dim].
* @param p1
* in: p1 of iline = p0 + u*(p1-p0) sized:[image_dim].
* @param max_dist
* in: width of ray in which to search.
* @param pt_index
* out: image_pt index in tag for closest point.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM int                // rtn: tag id or negative err code
DM_find_tag_by_image_line  // eff: search hierarchy tree
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : member of hierarchy to search, [pre-alloced]
   double *p0,             // in : p0 of iline = p0 + u*(p1 - p0), [pre-alloced]
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
   double *p1,             // in : p1 of iline = p0 + u*(p1 - p0), [pre-alloced]
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
   double max_dist,        // in : width of ray in which to search
   int    &pt_index,       // out: image_pt index in tag for closest pt
   SDM_options *sdmo=NULL);// in : SDM_options pointer

//====================================================
//  DM_xsect_dmod_by_image_line
//====================================================

/**
* Finds <tt>uv_params</tt> for a ray deformable model intersection and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies <tt>domain_pt</tt>, <tt>image_pt</tt>, the active deformable model.
* <br><br>
* When the intersection is successful, this extension loads <tt>domain_pt</tt> and <tt>image_pt</tt>
* with nearest intersection point data, and sets the active deformable model to be
* the deformable model owner of the intersection, and return <tt>0</tt>.
* <br><br>
* <tt>max_dist</tt> defines the distance between two curves which is considered a near-miss.
* <br><br>
* <tt>domain_pt</tt> and <tt>image_pt</tt> are loaded with the position and the <tt>domain_pt</tt> of the
* intersection. The return is <tt>-1</tt> when no intersections are found.
* <br><br>
* When <tt>domain_flag</tt> is set to <tt>1</tt>, <tt>domain_pt</tt> is returned in the unit range. When
* <tt>domain_flag</tt> is set to <tt>0</tt>, <tt>domain_pt</tt> is returned in the dmod's original domain
* range. When <tt>domain_flag</tt> is set to <tt>2</tt>, <tt>domain_pt</tt> is returned in the dmod's <tt>pfunc</tt>'s
* internal domain range.
* <br><br>
* The input argument <tt>walk_flag</tt> controls the depth and breadth of the search for
* deformable models that intersect the input image line. A <tt>walk_flag</tt> value of <tt>0</tt>
* specifies that just the deformable model is modified, and a walk_flag value of 1
* specifies that the deformable model and all its offspring are modified and the
* greatest descendant is returned. When <tt>walk_flag</tt> value is <tt>2</tt> the target deformable
* model, its siblings, and all their descendants are searched for an intersection.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dmod
* in: target dmod.
* @param walk_flag
* in: specify how deep to go 0=dmod only, 1=dmod and offspring 2=dmod,siblings,and offspring.
* @param p0
* in: p0 of iline = p0 + u*(p1 - p0) sized:[image_dim].
* @param p1
* in: p1 of iline = p0 + u*(p1 - p0) sized:[image_dim].
* @param max_dist
* in: width of ray in which to search for crvs.
* @param image_line_u
* out: image_line u param value of xsect.
* @param domain_flag
* in: 0=rtn domain_pt in orig_dmod_space, 1=rtn domain_pt in unit_space, 2=rtn domain_pt in internal_pfunc_space.
* @param domain_pt
* out: shape uv param value of xsect sized:[domain_dim].
* @param image_pt
* out: image pt of xsect sized:[image_dim].
* @param sdmo
* in:SDM_options pointer note: sets active dmod.
**/

DECL_DM void
DM_xsect_dmod_by_image_line// eff: find uv_params for ray-dmod xsect
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : dmod to query, [pre-alloced]
   int walk_flag,          // in : specify how deep to go
                           //      0=dmod only,1=dmod and offspring
                           //      2=dmod,siblings,and offspring
   double *p0,             // in : p0 of iline = p0 + u*(p1 - p0), [pre-alloced]
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
   double *p1,             // in : p1 of iline = p0 + u*(p1 - p0), [pre-alloced]
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
   double max_dist,        // in : width of ray in which to search
                           //      for crvs
   double &image_line_u,   // out: image_line u param value of xsect
   int domain_flag,        // in : 0=return domain_pt in orig_dmod_space,
                           //      1=return domain_pt in unit_space.
                           //      2=return domain_pt in internal_pfunc_space.
   double *domain_pt,      // out: xsect uv param (0 to 1 range), [pre-alloced]
                           //      sized:[DM_get_domain_dim(rtn_err, dmod)]
   double *image_pt,       // out: image pt of xsect, [pre-alloced]
                           //      sized:[DM_get_image_dim(rtn_err, dmod)]
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: sets active dmod

//====================================================
// Library Management
//====================================================
//DO NOT DOC

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM void
DM_notify_icon_tagobj_changed// eff: send tagobj_changed message to icon
           (int &rtn_err,    // out: 0=success or neg err code
            DS_dmod *dmod,   // in : tag object identifier
            int tag,         // in : tag object identifier
     SDM_options *sdmo=NULL);// in : SDM_options pointer

//DO NOT DOC

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM void
DM_notify_icon_tagobj_changed// eff: send tagobj_changed message to icon
           (int &rtn_err,    // out: 0=success or neg err code
            DS_dmod *dmod,   // in : tag object identifier
            const int* tags, // in : tag array sized:[ntags]
            int ntags,       // in : number of tags
     SDM_options *sdmo=NULL);// in : SDM_options pointer



/**
* Sets the icon's owner.
* <br><br>
* <b>Role:</b> This <tt>API</tt> serves as notification to the icon that the owning tag
* object is complete, and the icon can be initialized. <tt>rtn_err</tt> returns 0 for
* success or a negative error code.
* <br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>Could not find tag object with the given tag.</dd></dl>
* <br>
* <b>Effect:</b> System routine
* <br><br>
* @param rtn_err
* error code.
* @param dmod
* model.
* @param tag
* tag object ID.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_set_icon_owner            // eff: set the icon's owner
           (int &rtn_err,    // out: 0=success or neg err code
            DS_dmod *dmod,   // in : tag object identifier
            int tag,         // in : tag object identifier
     SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Sets the icon's owner.
* <br><br>
* <b>Role:</b> This <tt>API</tt> serves as notification to the icon that the owning tag
* object is complete, and the icon can be initialized. <tt>rtn_err</tt> returns <tt>0</tt> for
* success or a negative error code.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>Could not find tag object with the given tag.</dd></dl>
* <br>
* <b>Effect:</b> System routine
* <br><br>
* @param rtn_err
* error code.
* @param dmod
* tag object ID.
* @param tags
* tag array.
* @param ntags
* number of tags.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_set_icon_owner            // eff: set the icon's owner
           (int &rtn_err,    // out: 0=success or neg err code
            DS_dmod *dmod,   // in : tag object identifier
            const int *tags, // in : tag array sized:[ntags]
            int ntags,       // in : number of tags
     SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Sets the icon for a tag object.
* <br><br>
* <b>Role:</b> This call should be followed with a call to <tt>DM_set_icon_owner</tt> to
* allow the icon to initialize itself. <tt>rtn_err</tt> returns <tt>0</tt> for success or a negative
* error code.
* <br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
* <dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>Could not find tag object with the given tag.</dd></dl>
* <br>
* <b>Effect:</b> System routine
* <br><br>
* @param rtn_err
* error code.
* @param dmod
* tag object ID.
* @param tag
* tag object ID.
* @param dmicon
* new icon.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_set_icon                      //eff: set tag obj icon; set icon's tag & DS_dmod ptrs
           (int& rtn_err,        // out: 0=success or neg err code
            DS_dmod *dmod,       // in : tag object identifier
            int tag,             // in : tag object identifier
            DM_icon *dmicon,     // in : the new icon
         SDM_options *sdmo=NULL);// in : SDM_options pointer


/**
* Gets the icon associated with a tag object and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>Could not find tag object with the given tag.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* error code.
* @param dmod
* model to check.
* @param tag
* tag object owning icon.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM DM_icon*                  // rtn: input's DM_icon ptr
DM_get_icon                       // eff: retrieve icon pointer
          (int &rtn_err,          // out: 0=success or neg err code
           DS_dmod *dmod,         // in : tag object identifier
           int tag,               // in : tag object identifier
          SDM_options *sdmo=NULL);// in : SDM_options pointer
/**
* Broadcasts <tt>draw</tt> method and it draws these icons using the data encapsulated by the <tt>draw_args</tt>.
* <br><br>
* <b>Role:</b> This function calls the icon's <tt>Draw</tt> method. The data
* encapsulated by the <tt>draw_args</tt> is passed through this method. <tt>rtn_err</tt> will return
* <tt>0</tt> for success, or a negative error code.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>Could not find tag object with the given tag.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* error code.
* @param args
* args passed.
* @param dmod
* model to query.
* @param tag
* tag object owning icon.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_draw_icon                     // eff: pass the command encapsulated by args to the icon's Draw method
           (int &rtn_err,        // out: 0=success or neg err code
            const DM_icon_draw_args &args,// in :  command passed through to the icons
            DS_dmod *dmod,       // in : tag object identifier
            int tag,             // in : tag object identifier
         SDM_options *sdmo=NULL);// in : SDM_options pointer
/**
* Broadcasts <tt>draw</tt> method and it draws these icons using the data encapsulated by the draw_args.
* <br><br>
* <b>Role:</b> This function calls the icon's <tt>Draw</tt> method. The data
* encapsulated by the <tt>draw_args</tt> is passed through this method. <tt>rtn_err</tt> will return
* <tt>0</tt> for success, or a negative error code.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>Could not find tag object with the given tag.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* error code.
* @param args
* args passed.
* @param dmod
* model to query.
* @param tags
* tag object owning query.
* @param ntags
* number of tags.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_draw_icon                          // eff: pass the command encapsulated by args to the icon's Draw method
            (int &rtn_err,            // out: 0=success or neg err code
             const DM_icon_draw_args &args,// in :  command passed through to the icons
             DS_dmod *dmod,           // in :  tag object identifier
             const int *tags,         // in :  tag object identifier, array of tags sized:[ntags]
             int ntags,              // in :  number of tags
             SDM_options *sdmo=NULL);// in : SDM_options pointer


/**
* Broadcasts the command encapsulated by the <tt>cmd_args</tt> to the icons.
* <br><br>
* <b>Role:</b> This function calls the icon's <tt>Setstate</tt> method. The data
* encapsulate by the <tt>cmd_args</tt> is passed through this method.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>Could not find tag object with the given tag.</dd></dl>
* <br>
* <b>Effect:</b> System routine
* <br><br>
* @param rtn_err
* error code.
* @param args
* args passed.
* @param dmod
* model to query.
* @param tag
* tag object owning icon.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_setstate_icon                  // eff: pass the command encapsulated by args to the icon's Draw method
           (int &rtn_err,         // out: 0=success or neg err code
            const DM_icon_cmd_args &args,// in :  command passed through to the icons
            DS_dmod *dmod,        // in : tag object identifier
            int tag,              // in : tag object identifier
          SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Broadcasts the command encapsulated by the <tt>cmd_args</tt> to the icons.
* <br><br>
* <b>Role:</b> This function calls the icon's <tt>Setstate</tt> method. The data
* encapsulate by the <tt>cmd_args</tt> is passed through this method.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>Could not find tag object with the given tag.</dd></dl>
* <br>
* <b>Effect:</b> System routine
* <br><br>
* @param rtn_err
* error code.
* @param args
* args passed.
* @param dmod
* model to check.
* @param tags
* tag object owning icon.
* @param ntags
* number of tag objects.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_setstate_icon                      // eff: pass the command encapsulated by args to the icon's Set_state method
           (int &rtn_err,        // out: 0=success or neg err code
            const DM_icon_cmd_args &args,// in :  command passed through to the icons
            DS_dmod *dmod,            // in :  tag object identifier
            const int *tags,          // in :  tag object identifier, array of tags sized:[ntags]
            int ntags,               // in :  number of tags
             SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Passes the command encapsulated by the <tt>query_args</tt> to the icon and to query the icon state.
* <br><br>
* <b>Role:</b> This function calls the icon's <tt>Query</tt> method. The data
* encapsulated by the <tt>query_args</tt> is passed through this method. <tt>rtn_err</tt> returns <tt>0</tt>
* for success or a negative error code.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>Could not find tag object with the given tag</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* error code.
* @param args
* args passed.
* @param dmod
* model to query.
* @param tag
* tag object owning icon.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_query_icon                         // eff: pass the query encapsulated by args to the icon's Query method
            (int& rtn_err,            // out: 0=success or neg err code
             DM_icon_query_args& args,// in : query passed through to the icons
             DS_dmod *dmod,           // in : tag object identifier
             int tag,                 // in : tag object identifier
              SDM_options *sdmo=NULL);// in : SDM_options pointer


/**
* Gets the deformable model's epsilon value and returns <tt>0</tt> or an error.
* <br><br>
* <b>Role:</b> Epsilon regulates a shape fairing (energy minimization) term
* that is used to dampen control point oscillations in high degree splines
* (degree>8).
* <br><br>
* Like the primary fairing terms, alpha and beta, epsilon should be <tt>0</tt> or positive.
* Epsilon is considered a supplement to the alpha and beta shape fairing terms,
* and should be relatively small compared to beta, the chief shape fairing term.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_NOT_A_DMOD_MEMBER</tt></dt>
*<dd>The <tt>tag_flag</tt> does not identify a deformable model in the patch hierarchy.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* return code 0=success, neg=error.
* @param dmod
* tag object identifier.
* @param tag
* tag object identifier.
* @param eps
* epsilon.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_get_epsilon             // eff: return the dmod's epsilon value values
  (int &rtn_err,           // out: 0=success or neg err code
   const DS_dmod *dmod,    // in : tag object identifier
   int tag,                // in : tag object identifier
   double& eps,            // out: epsilon
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Sets the deformable model's epsilon value and returns <tt>0</tt> or an error.
* <br><br>
* <b>Role:</b> Epsilon regulates a shape fairing (energy minimization) term
* that is used to dampen control point oscillations in high degree splines
* (degree>8).
* <br><br>
* Like the primary fairing terms, alpha and beta, epsilon should be <tt>0</tt> or positive.
* Epsilon is considered a supplement to the alpha and beta shape fairing terms,
* and should be relatively small compared to beta, the chief shape fairing term.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_NOT_A_DMOD_MEMBER</tt></dt>
*<dd>The <tt>tag_flag</tt> does not identify a deformable model in the patch hierarchy.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* return code 0=success, neg=error.
* @param dmod
* tag object identifier.
* @param tag
* tag object identifier.
* @param eps
* epsilon.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_set_epsilon             // eff: set the dmod's epsilon value values
  (int &rtn_err,           // out: 0=success or neg err code
   DS_dmod *dmod,          // in : tag object identifier
   int tag,                // in : tag object identifier
   double eps,             // in : epsilon
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the tags of all tag objects in a <tt>DS_dmod</tt> hierarchy.
* <br><br>
* <b>Role:</b> This function returns a <tt>DM_tag_array</tt> array containing the tags
* of all tag objects embedded in the <tt>DS_dmod</tt> hierarchy containing the input
* <tt>DS_dmod</tt>. The array is <tt>sized:[tags.Size()]</tt>. <tt>rtn_err</tt> returns <tt>0</tt> or a negative error
* code.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* error code.
* @param dmod
* model to check.
* @param ntags
* number of tags.
* @param tags
* array of tags.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_get_tags                      // eff: tag query for DS_dmod hierarchy
           (int &rtn_err,        // out: 0=success or neg err code
            DS_dmod *dmod,       // in : DS_dmod in hierarchy
            int &ntags,          // out: number of tags
            DM_tag_array &tags,  // out: array of tags found sized:[tags.Size()]
         SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the tags of all tag objects on a single <tt>DS_dmod</tt>.
* <br><br>
* <b>Role:</b> This function returns a <tt>DM_tag_array</tt> containing the tags of all
* tag objects embedded in the input <tt>DS_dmod</tt>. The array is sized:[tags.Size()].
* <tt>rtn_err</tt> returns <tt>0</tt> for success or a negative error code.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* error code.
* @param dmod
* model to check.
* @param ntags
* number of tags.
* @param tags
* array of tags.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_get_dmod_tags                 // eff: tag query for a single DS_dmod
           (int &rtn_err,        // out: 0=success or neg err code
            DS_dmod *dmod,       // in : the DS_dmod
            int &ntags,          // out: number of tags
            DM_tag_array &tags,  // out: array of tags found  sized:[tags.Size()]
         SDM_options *sdmo=NULL);// in : SDM_options pointer
/**
* Evaluate an embedded source curve for the given tag.
* <br><br>
* <b>Role:</b> This function evaluates an embedded source curve. It returns the
* embedding surface <i>uv</i> (domain) points corresponding to the input curve parameter
* points.See <tt>DM_eval_dmod</tt> for evaluating the embedding surface. <tt>rtn_err</tt> will
* return <tt>0</tt> for success, or a negative error code. The two arrays are sized: domain
* points, <tt>sized:[npts]</tt>; <i>uv</i> points, <tt>sized:[C.Size()]</tt>.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>Could not find tag object with the given tag.</dd>
*<dt><tt>DM_TAGOBJ_NO_SRC_PFUNC_OR_FUNC</tt></dt>
*<dd>Could not find <tt>src_C_pfunc</tt> or <tt>src_CW_func</tt> for given tag object (see
* <tt>DM_add_crv_cstrn</tt>, <tt>DM_add_crv_load</tt>, <tt>DM_add_link_cstrn</tt>).</dd>
*<dt><tt>DM_TAGOBJ_NOT_A_CURVE</tt></dt>
*<dd>Given tag object is not a curve.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* error code.
* @param dmod
* tag object ID.
* @param tag
* tag object ID.
* @param s
* array of domain points.
* @param npts
* number of eval points.
* @param C
* array of uv points.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_eval_crv_src_domain            // eff: eval src curve; return uv of embedding surface
           (int &rtn_err,         // out: 0=success or neg err code
            DS_dmod *dmod,        // in : tag object identifier
            int tag,              // in : tag object identifier
            const double *s,      // in : array of domain points, sized:[npts]
            int npts,             // in : number of eval points
            DM_dbl_array &C,      // out: array of image points, sized:[C.Size()]
          SDM_options *sdmo=NULL);// in : SDM_options pointer
/**
* Evaluate a target curve for the given tag.
* <br><br>
* <b>Role:</b> Returns the <i>xyz</i> points corresponding to the input curve parameter
* points.This function evaluates a target curve for the given tag.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>Could not find tag object with the given tag.</dd>
*<dt><tt>DM_TAGOBJ_NO_SRC_PFUNC_OR_FUNC</tt></dt>
*<dd>Could not find <tt>src_W_pfunc</tt>, <tt>src_CW_func</tt>, or <tt>src_C_pfunc</tt> for given tag object
* (see <tt>DM_add_crv_cstrn</tt>, <tt>DM_add_crv_load</tt>, <tt>DM_add_link_cstrn</tt>).</dd>
*<dt><tt>DM_TAGOBJ_NOT_A_CURVE</tt></dt>
*<dd>Given tag object is not a curve.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* error code.
* @param dmod
* tag object ID.
* @param tag
* tag object ID.
* @param s
* array of domain points.
* @param npts
* number of eval points.
* @param W
* array of uv points.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_eval_crv_tgt                   // eff: eval target curve SPAposition; return xyz
           (int &rtn_err,         // out: 0=success or neg err code
            DS_dmod *dmod,        // in : tag object identifier
            int tag,              // in : tag object identifier
            const double *s,      // in : array of domain points, sized:[npts]
            int npts,             // in : number of eval points
            DM_dbl_array &W,      // out: array of image points, sized:[W.Size()]
          SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the tag object minimum parameterization value.
* <br><br>
* <b>Role:</b> Many tag objects are parameterized by either a continuous (e.g.,
* curve load) or discrete parameter (e.g., point spring set). This function
* returns the lower corner of the parameterization bounding box. For example, for
* a curve load, this is a single value, and for an area constraint this a pair
* (<i>uv</i>) of values. <tt>rtn_err</tt> returns <tt>0</tt> for success or a negative error code.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be NULL on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>Could not find tag object with the given tag.</dd>
*<dt><tt>DM_TAGOBJ_HAS_NO_PARAM</tt></dt>
*<dd>Tag object is not a parameterized type.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* error code.
* @param dmod
* model to check.
* @param tag
* tag object ID.
* @param s_arr
* tag object ID.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_param_min                     // eff: min param value
           (int &rtn_err,         // out: 0=success or neg err code
            DS_dmod *dmod,        // in : tag object identifier
            int tag,              // in : tag object identifier
            DM_dbl_array &s_arr,  // out: param value; sized:[s_arr.Size()]
          SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Gets the tag object maximum parameterization value.
* <br><br>
* <b>Role:</b> Many tag objects are parameterized by either a continuous (e.g.,
* curve load) or discrete parameter (e.g., point spring set). This function
* returns the upper corner of the parameterization bounding box. For example, for
* a curve load, this is a single value, and for an area constraint this a pair
* (<i>uv</i>) of values. <tt>rtn_err</tt> returns <tt>0</tt> for success or a negative error code.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd>
*<dt><tt>DM_TAG_OBJECT_NOT_FOUND</tt></dt>
*<dd>Could not find tag object with the given tag.</dd>
*<dt><tt>DM_TAGOBJ_HAS_NO_PARAM</tt></dt>
*<dd>Tag object is not a parameterized type.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* error code.
* @param dmod
* model to check.
* @param tag
* tag object ID.
* @param s_arr
* tag object ID.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_param_max                      // eff: max param value
           (int &rtn_err,         // out: 0=success or neg err code
            DS_dmod *dmod,        // in : tag object identifier
            int tag,              // in : tag object identifier
            DM_dbl_array &s_arr,  // out: param value; sized:[s_arr.Size()]
          SDM_options *sdmo=NULL);// in : SDM_options pointer
/**
* Gets the tags of all <tt>DS_dmods</tt> in the hierarchy containing the input <tt>DS_dmod</tt>.
* <br><br>
* <b>Role:</b> This function returns a <tt>DM_tag_array</tt> containing the tags of all
* DS_dmods in the input <tt>DS_dmod</tt> hierarchy. The array is <tt>sized:[tags.Size()]</tt>.
* <tt>rtn_err</tt> returns <tt>0</tt> for success or a negative error code.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The deformable model cannot be <tt>NULL</tt> on entry.</dd></dl>
* <br>
* <b>Effect:</b> Read-only
* <br><br>
* @param rtn_err
* error code.
* @param dmod
* model to check.
* @param ntags
* number found.
* @param tags
* array of tags.
* @param sdmo
* in:SDM_options pointer.
**/

DECL_DM void
DM_get_dmods                     // eff: get array of dmod tags for the entire hierarchy
           (int& rtn_err,        // out: 0=success or neg err code
            DS_dmod *dmod,       // in : dmod in hierarchy
            int &ntags,          // out: number of dmods found
            DM_tag_array &tags,  // out: array of tags for found dmods - sized:[tags.Size()]
         SDM_options *sdmo=NULL);// in : SDM_options pointer


/**
* Sets the size of a <tt>DM_dbl_array</tt> and initializes the memory.
* <br><br>
* <b>Role:</b> The <tt>DM_dbl_array</tt> is a container class provided by deformable
* modeling. The <tt>DM_dbl_array</tt> can be used as growable array via the
* <tt>DM_set_array_size</tt> method. There are no user heap allocations or deletes
* associated with the <tt>DM_dbl_array</tt>.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* @param rtn_err
* error code.
* @param arr
* array to resize.
* @param new_size
* size to set.
* @param init_val
* value stored in returned memory.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_set_array_size                   // eff: set array size and initialize memory
            (int &rtn_err,          // out: 0=success or neg err code
             DM_dbl_array &arr,     // out: array to resize; sized:[arr.Size()]
             int new_size,          // in : size to set
             double init_val=0.0,   // in : value stored in returned memory
            SDM_options *sdmo=NULL);// in : SDM_options pointer

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM void
DM_get_area_load_minimal_corners // eff: query for an area load
  (int &rtn_err,                 // out: 0=success or neg err code
   DS_dmod *dmod,                // in : tag object identifier
   int tag,                      // in : tag object identifier
   DM_dbl_array &us,             // out: array of parameters sized:[us.Size()]
   DM_dbl_array &vs,             // out: array of parameters sized:[vs.Size()]
   SDM_options *sdmo=NULL);      // in : SDM_options pointer

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM void
DM_set_area_load_target        // eff: change the load target
  (int &rtn_err,               // out: 0=success or neg err code
   DS_dmod *dmod,              // in : tag object identifier
   int tag,                    // in : tag object identifier
   DS_pfunc*& pfunc,           // i/o: target pfunc (nulled on return)
   SDM_options *sdmo=NULL);    // in : SDM_options pointer

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM void
DM_initialize                  // eff: initialize deformable modeling
  (int &rtn_err,               // out: 0=success or neg err code
   SDM_options *sdmo=NULL);    // in : SDM_options pointer


/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM void
DM_initialize                   // eff: initialize deformable modeling
  (int &rtn_err,                // out: 0=success or neg err code
   const DS_init_sdmhusk_args &args, // in : options
   SDM_options *sdmo=NULL);     // in : SDM_options pointer


/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM void
DM_terminate                 // eff: terminate deformable modeling
  (int &rtn_err,             // out: 0=success or neg err code
   SDM_options *sdmo=NULL);  // in : SDM_options pointer

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM void
DM_terminate                    // eff: terminate deformable modeling
  (int &rtn_err,                // out: 0=success or neg err code
   const DS_init_sdmhusk_args &args, // in : options used for initialization
   SDM_options *sdmo=NULL);     // in : SDM_options pointer


/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM void
DM_find_link_total_gap     // use: Find total gap along link curve
 (int &rtn_err,            // out: 0=success or neg err code
  DS_dmod *dmod,           // in : member of hierarchy to modify
  int tag,                 // in : tag of curve to test
  int behavior,            // in : behavior to test; 1==>pos-link, 2==>tan-link, 4==>cur<i>v</i>-link
  double *total_gap,       // out: integrated (total) gap sized:[3]
   SDM_options *sdmo=NULL);// in : SDM_options pointer


/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM SDM_options *      // rtn: Pointer to an SDM_options object; caller must call DM_delete_SDM_options
DM_make_SDM_options        // use: Construct an SDM_options object
 (int &rtn_err,            // out: 0=success or neg err code
  int major,               // in : major version
  int minor,               // in : minor version
  int point);              // in : point version

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM void
DM_delete_SDM_options      // use: Delete an SDM_options object
(int &rtn_err,             // out: 0=success or neg err code
  SDM_options *&old_sdmo,  // i/o: in, options to delete, out, NULLed on exit
  SDM_options *sdmo=NULL); // in : SDM_options pointer

//DO NOT DOC
/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM DS_dmod*             // rtn: ptr to DS_dmod object
DM_get_dmod_from_jou_table   // eff: return the first DS_dmod from the journal table
           (int &rtn_err,    // out: 0=success or neg err code
   SDM_options *sdmo=NULL);// in : SDM_options pointer
//DO NOT DOC
/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM DS_dmod*             // rtn: ptr to DS_dmod object
DM_get_next_dmod_from_jou_table   // eff: return the first DS_dmod from the journal table
           (int &rtn_err,    // out: 0=success or neg err code
            int &istart,     // i/o: in, where to start, out, next table entry
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/*
// tbrv
*/
/**
 * @nodoc
 */

DECL_DM void
DM_disable_legacy_domain_scaling   // eff: turn off legacy domain scaling
            (int rtn_err,         // out: 0=success or neg err code
   SDM_options *sdmo=NULL);// in : SDM_options pointer

//====================================================
// DM_get_tolerance
//====================================================
/**
* Gets the tolerance limits.
* <br><br>
* <b>Role:</b> Modifies <tt>dist_tol</tt>, <tt>ang_tol</tt>. Gets the <tt>DS_tolerance</tt> and
* <tt>DS_angle_tol</tt> values. The <tt>DS_tolerance</tt> value is used to determine when the curve
* constraints are being held. Due to machine round-off this number should be no
* more than <tt>10</tt> orders of magnitude less than the range of the image space. So if
* you build your models in a <tt>10,000</tt> unit cube, dist_tol should be no smaller than
* <tt>1.0E-06</tt>. The angle tolerance is not currently used.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dist_tol
* out: min dist between two points.
* @param ang_tol
* out: min angle between two lines.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_get_tolerance           // eff: retrieve tolerance limits
  (int &rtn_err,           // out: 0=success or neg err code
   double &dist_tol,       // out: min dist between two points
   double &ang_tol,        // out: min angle between two lines
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Sets tolerance limits and returns <tt>0</tt> for success or an error.
* <br><br>
* <b>Role:</b> Modifies Library global values <tt>DS_tolerance</tt> and <tt>DS_angle_tol</tt>.
* <br><br>
* Sets the <tt>DS_tolerance</tt> and <tt>DS_angle_tol</tt> values.
* <br><br>
* The <tt>DS_tolerance</tt> value is used to determine when the curve constraints are being
* held. Due to machine round-off this number should be no more than 10 orders of
* magnitude less than the range of the image space. So if you build your models in
* a <tt>10,000</tt> unit cube, <tt>dist_tol</tt> should be no smaller than <tt>1.0E-06</tt>.
* <br><br>
* The angle tolerance is not currently used.
* <br><br>
* <b>Errors:</b><dl></dt><tt>DM_BAD_TOLERANCE_VALUE</tt>
*<dd>The <tt>dist_tol</tt> or <tt>ang_tol</tt> must be positive numbers.</dd></dl>
* <br>
* <b>Effect:</b> System routine
* <br><br>
* @param rtn_err
* out: 0=success or negative err code.
* @param dist_tol
* in: min dist between two points.
* @param ang_tol
* in: min angle between two lines.
* @param sdmo
* in:SDM_options pointer note: making these values too small will prevent the package from running.
**/
DECL_DM void
DM_set_tolerance           // eff: set tolerance limits
  (int &rtn_err,           // out: 0=success or neg err code
   double dist_tol,        // in : min dist between two points
   double ang_tol,         // in : min angle between two lines
   SDM_options *sdmo=NULL);// in : SDM_options pointer
                           // note: making these values too small
                           //       will prevent the package from
                           //       running.
/**
* Enables journaling and opens a file for write.
* <br><br>
* <b>Role:</b> This function modifies the library global values <tt>DM_journal_file</tt>,
* <tt>DM_journal</tt>, and <tt>DM_cascade</tt>.
* <br><br>
* A call to this function begins a <tt>DM API</tt> journal session. The call opens for
* write the file indicated by the input filename and sets the global variables
* <tt>DM_journal</tt> and <tt>DM_cascade</tt> to begin journaling all future <tt>DM API</tt> calls. <tt>DM API</tt>
* call journaling is a debugging tool for <tt>DM</tt> customers who do not run <tt>ACIS</tt>. When
* <tt>DM_journal</tt> is set to <tt>1</tt>, every call to a <tt>DM API</tt> routine generates an input report
* listing all the input variable values when the routine was entered. The <tt>DM API</tt>
* routines generate an output report listing all the output variable values just
* before the routine exits.
* <br><br>
* The cascade argument controls how much information gets written to the journal
* file as shown in the following table.
* <br><br>
* The cascade argument controls how much information gets written to the journal
* file. When the value of cascade is:
* <br><br>
* <tt>0 =</tt> all entry level <tt>DM</tt> calls are journaled.
* <br><br>
* <tt>1 = </tt>all entry level and cascaded <tt>DM</tt> calls are journaled.
* <br><br>
* <tt>2 = </tt>all entry level and intersection callback functions are journaled.
* <br><br>
* <tt>3 = </tt>all entry level <tt>DM</tt> calls, cascaded <tt>DM</tt> calls, and intersection callback
* functions are journaled.
* <br><br>
* When this function is called with journaling enabled, it closes the current
* journal file and opens a new one. Opening a journal file with the same name as
* the current journal file will cause an overwrite of the old journal file data.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The filename cannot be <tt>NULL</tt> on input.</dd>
*<dt><tt>DM_BAD_CASCADE_VALUE</tt></dt>
*<dd>The cascade value must be within the range <tt>0</tt> to <tt>3</tt>.</dd>
*<dt><tt>DM_FAILED_FILE_OPEN_WRITE</tt></dt>
*<dd>The designated file must be opened for write.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
*<br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param file_name
* file to write max sized:[256].
* @param cascade
* 0=only journal DM API entry calls 1=journal entry and cascading DM calls 2=journal entry and xsect callbacks 3=journal entry, cascading DM calls and xsect callbacks.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void
DM_journal_on              // eff: open and begin writing journal
  (int &rtn_err,           // out: 0=success or neg err code
   char *file_name,        // in : character name of file to write, [pre-alloced]
                           //      max sized:[256]
   int cascade=0,          // in : 0=only journal DM API entry calls
                           //      1=journal entry and cascading DM calls
                           //      2=journal entry and xsect callbacks
                           //      3=journal entry,cascading DM calls
                           //           and xsect callbacks
   SDM_options *sdmo=NULL);// in : SDM_options pointer

/**
* Replays a sequence of <tt>DM API </tt>calls from a <tt>DM API</tt> journal file for debugging purposes only.
* <br><br>
* <b>Role:</b> This function modifies the library global values <tt>DM_journal_file</tt>,
* <tt>DM_journal</tt>, and <tt>DM_cascade</tt>.
* <br><br>
* This function controls the opening and replaying of all the DM API calls held
* within a <tt>DM API</tt> call journal file. <tt>DM API</tt> call journaling is a debugging tool
* for <tt>DM</tt> customers who do not run run <tt>ACIS</tt>.
* <br><br>
* This function opens the indicated file and checks to see if it is a journal file.
* If so it replays all the recorded entries. If not, it returns an error code.
* When journaling is currently enabled, this function journal file before opening
* the new file. During replay, if all the returned arguments for every replayed DM
* <tt>API</tt> calls are the same as the journal entries then a <tt>0</tt> is returned; otherwise, a
* negative error code is returned. This replay feature can be used to compare the
* behavior of the <tt>DM API</tt> as integrated within two different applications and/or
* interfaced to two different geometric modelers.
* <br><br>
* <b>Errors:</b> <dl><dt><tt>DM_NULL_INPUT_PTR</tt></dt>
*<dd>The filename cannot be <tt>NULL</tt> on input.</dd>
*<dt><tt>DM_BAD_CASCADE_VALUE</tt></dt>
*<dd>The cascade value must be within the range <tt>0</tt> to <tt>3</tt>.</dd>
*<dt><tt>DM_FAILED_FILE_OPEN_READ</tt></dt>
*<dd>The designated file must be opened for read.</dd></dl>
* <br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param file_name
* name of file to read max sized:[256].
* @param sdmo
* in:SDM_options pointer.
**/

DECL_DM void
DM_journal_play            // eff: open and begin playing journal
  (int &rtn_err,           // out: 0=success or neg err code
   char *file_name,        // in : character name of file to play, [pre-alloced]
                           //      max sized:[256]
   SDM_options *sdmo=NULL);// in : SDM_options pointer
/**
* Closes a journal file and stops further journaling of <tt>DM API</tt> calls.
* <br><br>
* <b>Role:</b> This function modifies the library global values <tt>DM_journal_file</tt>,
* <tt>DM_journal</tt>, and <tt>DM_cascade</tt>.
* <br><br>
* This function ends the recording of a <tt>DM API</tt> journal. <tt>DM API</tt> call journaling is
* a debugging tool for customers who run the <tt>DM API</tt> without <tt>ACIS</tt>.
* <br><br>
* This function closes any currently open <tt>DM API</tt> journal file and sets the global
* variables <tt>DM_journal_file</tt>, <tt>DM_journal</tt> and <tt>DM_cascade</tt> to stop journaling all
* future <tt>DM API</tt> calls.
* <br><br>
* When journaling is currently disabled, this function takes no action.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param rtn_err
* out: 0=success or neg err code.
* @param sdmo
* in:SDM_options pointer.
**/
DECL_DM void               // eff: close journal file and stop
DM_journal_off             //      stop journaling.
  (int &rtn_err,           // out: 0=success or neg err code
   SDM_options *sdmo=NULL);// in : SDM_options pointer

//============================================================
// UNDOCUMENTED CONVENIENCE ROUTINES
//============================================================
/*
// tbrv
*/
/**
 * @nodoc
 */

DS_TAGS
DS_load_enum_2tag_enum     // rtn: DS_tag enum for input enum
  (DS_LDS type) ;          // in : a load type enum

/*
// tbrv
*/
/**
 * @nodoc
 */

DS_TAGS
DS_dmod_enum_2tag_enum     // rtn: DS_tag enum for input enum
  (DS_DMO type) ;          // in : a dmod type enum

/*
// tbrv
*/
/**
 * @nodoc
 */

DS_TAGS
DS_cstrn_enum_2tag_enum    // rtn: DS_tag enum for input enum
  (DS_CST type,            // in : a cstrn type enum
   DS_CSTRN_SRC src,       // in : a cstrn src_type enum
   DS_cstrn *cstrn) ;      // in : ptr to cstrn being classified

/*
// tbrv
*/
/**
 * @nodoc
 */

DS_DMO
DS_tag_enum_2dmod_enum     // rtn: DS_DMO enum for input DS_TAGS
  (DS_TAGS type) ;         // in : a dmod type enum
/*
DS_LDS
DS_tag_enum_2load_enum     // rtn: DS_LDS enum for input enum
  (DS_TAGS type) ;         // in : a load type enum

DS_CST
DS_tag_enum_2cstrn_enum    // rtn: DS_CST enum for input enum
  (DS_TAGS type) ;         // in : a DS_tag type enum
*/
/** @} */
#endif // end DMAPI_H

