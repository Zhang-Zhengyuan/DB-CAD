/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( SWEEP_OPTIONS_CLASS )
#define SWEEP_OPTIONS_CLASS
/*******************************************************************/
// Include Files:
#include "acis.hxx"
#include "acis_options.hxx"
#include "dcl_swp.h"
#include "debugmsc.hxx"
#include "law.hxx"
#include "law_spl.hxx"
#include "logical.h"
#include "swp_enum.hxx"
#include "vers.hxx"
/*******************************************************************/
/**
 * @file swp_opts.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup SWPAPI
 *
 * @{
 */
class BODY;
class COEDGE;
class intcurve;
class SPAbox;
class SPApar_box;
class SPApar_pos;
class SPAposition;
class SPAtransf;
class SPAunit_vector;
class SPAvector;
class VOID_LIST;

/**
 * Type of merging operation.
 * For illustrations of the various merging options, see the <i>Merge Options</i> section
 * of the <i>Sweep Results</i> Technical Article.
 * @param merge_none
 * Does not merge any tangent edges.
 * @param merge_lateral
 * Merge tangent lateral edges if both surfaces are same.
 * @param merge_vertical
 * Merges tangent intermediate profile edges if both surfaces are same.
 * This occurs when draft angle or draft law is used.
 * @param merge_all
 * Merges all possible edges. The default value.
 * @param merge_unset
 * If <tt>merge_unset</tt> is specified, the global <tt>merge</tt> option will be used.<br>
 * If the global <tt>merge</tt> option is <tt>TRUE</tt>, then <tt>merge_all</tt> will be used.<br>
 * If the global <tt>merge</tt> option is <tt>FALSE</tt>, then <tt>merge_none</tt> will be used.
 */
enum merge_type
{
   merge_none,
   merge_lateral,
   merge_vertical,
   merge_all,
   merge_unset
};


/**
 * Type of mitering operation.
 * For illustrations of the various mitering options, see the <i>Miter Options</i> section
 * of the <i>Sweep Results</i> Technical Article.
 * @param default_miter
 * Default behavior (<tt>new_miter</tt>).
 * @param old_miter
 * Intersects the plane which is perpendicular to the path at the half angle
 * of the corner. The resulting profile on the plane is then swept continuing
 * along the path
 * @param new_miter
 * Reflects the profile to the other side of the corner up to the discontinuous
 * point. The two sides are then extended, intersected, and new edges formed as
 * necessary. The "ending" profile is the same as the starting profile.
 * @param crimp_miter
 * Reflects the profile to the other side of the corner up to the discontinuous
 * point. The portions of the two sides which are not intersecting are connected
 * using a smooth rotation about the discontinuous point
 * @param bend_miter
 * Makes a rounded miter.
 */
enum miter_type
{
    default_miter,
        old_miter,
        new_miter,
        crimp_miter,
        bend_miter		// Not currently implemented
};

/**
 * Specifies what and how to use a specific portion of the path in the sweeping operation.
 * @param TO_PROFILE
 * Sweep between start to profile location.
 * @param FROM_PROFILE
 * Sweep between profile location and path end.
 * @param ENTIRE_PATH
 * Sweep entire path.
 * @param BETWEEN_POINTS
 * Sweep between two points (projected to path).
 * @param SWEEP_TO
 * Sweep from profile to a point (projected to path).
 * @param AS_IS
 * Takes profile to the start of the path.
 */
enum sweep_portion
{
    TO_PROFILE,		   // sweep between start to profile location
        FROM_PROFILE,	   // sweep between profile location and path end
        ENTIRE_PATH,	   // sweep entire path
        BETWEEN_POINTS,	   // sweep between two points (projected to path)
        SWEEP_TO,		   // sweep from profile to a point (projected to path)
        AS_IS              // take profile as at the start of the path
};

/**
 * Gap type used in <tt>sweep_options</tt>
 */
#define EXTENDED 0

/**
 * Gap type used in <tt>sweep_options</tt>
 */
#define ROUNDED  1

/**
 * Gap type used in <tt>sweep_options</tt>
 */
#define NATURAL  2

/**
 * Used to specify unset values in <tt>sweep_options</tt>.
 */
#define UNSET_OPTION -1

/**
 * Type of draft repair.
 * @param NO_REPAIR
 * Sweep all the extension of the path without checking for degeneracies.
 * @param ERROR_ON_REPAIR
 * Stops at the first degeneracy and errors out.
 * @param DEGENERACY
 * Sweep will find degeneracies in the profile, generate a new profile
 * and continue, in an iterative way until the end of the path or a total degeneration of the profile.
 * @param FIRST_DEGENERACY
 * Stops at the first degeneracy.
 * @param SELF_INTERSECTION
 * Similar to <tt>DEGENERACY</tt>, but more costly because it looks for self-intersecting cases.
 * @param FIRST_SELF_INTERSECTION
 * Stops at the first self-intersection.
 */
enum draft_repair_level {
    NO_REPAIR,
        ERROR_ON_REPAIR,
        DEGENERACY,
        FIRST_DEGENERACY,
        SELF_INTERSECTION,
        FIRST_SELF_INTERSECTION
};

/**
 * Type of draft on holes (inner loops in faces)
 * @param NO_DRAFT
 * Holes are swept without draft.
 * @param WITH_PERIPHERY
 * Holes are swept in the same direction than the periphery loop.
 * @param AGAINST_PERIPHERY
 * Holes are swept in the opposite draft direction as the periphery loop.
 * @param BY_ANGLE
 * Specify an angle for the holes. Using the <tt>AGAINST_PERIPHERY</tt> behavior.
 */
enum draft_hole_option {
    NO_DRAFT,
        WITH_PERIPHERY,
        AGAINST_PERIPHERY,
        BY_ANGLE
};

/**
 * 	Provides a data structure for sweeping operations to be used in the function @href api_sweep_with_options.
 * <br>
 * <b>Technical Article:</b> <i>[Sweeping with Options](http://doc.spatial.com/articles/s/w/e/Sweeping_with_Options_7371.html)</i>
 * <br><br>
 * <b>Role:</b> Provides a data structure for sweeping operations to be used in the function
 * <tt>api_sweep_with_options</tt>.
 * The options are described in the Technical Article <i>Sweeping with Options</i> and include:
 * <ul>
 * <li> <tt>bool_type</tt>: Specifies the Boolean operation to be used along with <tt>sweep_to_body</tt>
 *      or <tt>to_face</tt>. It is of type @href sweep_bool_type. Default value is <tt>LIMIT</tt>. </li>
 * 
 * <li> <tt>close_to_axis</tt>: If a revolve operation is specified and the profile is an open wire that is not touching
 *      the rotation axis, it will close the gap(s) between the profile and the axis. 
 *      Default is <tt>FALSE</tt>. </li>
 *
 * <li> <tt>cut_end_off</tt>: Cuts the end of the swept body squarely, with a perpendicular plane to the end of
 *      the profile. Default is <tt>FALSE</tt>. </li>
 * 
 * <li> <tt>draft_angle</tt>: Specifies the draft angle in radians. Default is 0.0. </li>
 * 
 * <li> <tt>draft_hole</tt>: Specifies the orientation of the draft angle in interior holes in faces being
 *      swept with draft. It is of type @href draft_hole_option. Default is <tt>AGAINST_PERIPHERY</tt>. </li>
 * 
 * <li> <tt>draft_hole_angle</tt>: The interior loops of a face can be drafted with a different angle. Default
 *      is 0.0. </li>
 * 
 * <li> <tt>draft_law</tt>: The draft angle can be defined in the form of a law. Default is <tt>NULL</tt>. </li>
 * 
 * <li> <tt>draft_repair</tt>: Specifies the type of repairs done internally to complete a draft operation.
 *      It is of type @href draft_repair_level. Default value is <tt>DEGENERACY</tt>. </li>
 * 
 * <li> <tt>end_draft_dist</tt>: Alternative way to set up a draft operation, by specifying the final
 *      offset distance. Default is 0.0. </li>
 * 
 * <li> <tt>gap_type</tt>: Specifies the type of face that will fill up the gaps made by draft operations.
 *      Its value may be: <tt>EXTENDED</tt>, <tt>ROUNDED</tt> or <tt>NATURAL</tt>. Default value is <tt>NATURAL</tt>. </li>
 * 
 * <li> <tt>keep_branches</tt>: Option to keep extra branches produced in <tt>sweep_to_body</tt> or <tt>to_face</tt> operations. 
 *      Default is <tt>FALSE</tt>. </li>
 * 
 * <li> <tt>keep_law</tt>: The resulting graph of the <tt>sweep_to_body</tt> or <tt>to_face</tt> operations can be defined
 *      using a customized law. Default is <tt>NULL</tt>. </li>
 * 
 * <li> <tt>keep_start_face</tt>: Specify if in a closed path operation or a full revolution revolve operation
 *      the original profile face should remain topologically in the same place, instead of getting
 *      absorbed by the new closed body. Default is <tt>FALSE</tt>.
 * <br><br>
 *      <b>Note:</b> ACIS throws an exception error after calling @href api_del_entity
 *      if the <tt>keep_start_face</tt> option was enabled during a sweep operation of
 *      more than 360 degrees. In other words, sweeping 360 degrees causes the original face
 *      to become the "top" or "end" face, and automatically deletes the face entity
 *      before the next API call. Thus, when <tt>api_del_entity</tt> attempts to access the original
 *      face entity, ACIS discovers that the face was deleted and not "kept" as specified.
 *      This conflict in behavior generates an exception error. </li>
 * 
 * <li> <tt>merge</tt>: The type of merge operation to be used. 
 *      It is of type @href merge_type. Default is <tt>merge_all</tt>. </li>
 * 
 * <li> <tt>miter</tt>: The type of mitering operation to be used. 
 *      It is of type @href miter_type. Default is <tt>new_miter</tt>. </li>
 * 
 * <li> <tt>miter_amount</tt>: Radius for <tt>bend_miter</tt> option. Default is 0.0. </li>
 * 
 * <li> <tt>portion</tt>: Specifies the portions of the path to be used. Default is <tt>ENTIRE_PATH</tt>. </li>
 * 
 * <li> <tt>portion_end</tt>: Position perpendicular to the path that will indicate the end of the
 *      sweep operation along the path. </li>
 * 
 * <li> <tt>portion_start</tt>: Position perpendicular to the path that indicates the beginning of the
 *      sweep operation along the path. </li>
 * 
 * <li> <tt>rail_laws</tt>: Laws that define the rails (for orientation) for the operation. Default is <tt>NULL</tt>. </li>
 * 
 * <li> <tt>rail_num</tt>: Number of rail laws used. </li>
 * 
 * <li> <tt>rigid</tt>: Specifies the orientation of the profile at miter planes or end of path. Default is <tt>FALSE</tt>. </li>
 * 
 * <li> <tt>scale_law</tt>: Scaling along the sweep path can be defined in the form of a vector law.
 *      Scale should be uniform in the profile plane <tt>xy</tt> and unit scale along path tangent<tt>z</tt>.
 *      Default is <tt>NULL</tt>. </li>
 *
 * <li> <tt>self_int_test</tt>: Specify if a self-intersecting surface check should be performed and if
 *      the body is allowed to complete even when a self-intersecting situation has been found. If set to 0, 
 *      the check will not be made and a body will be constructed; however, problems may appear during 
 *      subsequent modeling operations. If 2 is specified, the check will be performed, but it will notify 
 *      with a warning. Default is 1, which means an error will be thrown and a self-intersecting 
 *      body will not be constructed. </li>
 * 
 * <li> <tt>simplify</tt>: Simplify when possible surfaces into lighter (analytic) forms. Default is <tt>TRUE</tt>. </li>
 * 
 * <li> <tt>solid</tt>: The result body should be a closed solid body when possible. The default is <tt>TRUE</tt>, 
 *      meaning the result should be a closed solid, unless the profile is a non-closed wire or edge. 
 *      If the intended result is a open solid body, the <tt>solid</tt> and <tt>two_sided</tt> options 
 *      should be set to <tt>FALSE</tt>. If the intended result is a open sheet body, the <tt>solid</tt> option
 *      should be set to <tt>FALSE</tt> and the <tt>two_sided</tt> option should be set to <tt>TRUE</tt>. </li>
 * 
 * <li> <tt>start_draft_dist</tt>: Start offset distance used in draft operations. Default is 0.0. </li>
 * 
 * <li> <tt>steps</tt>: Number of linear divisions in revolve operations. Default is 0. 
 *      Converts a circular sweep path into the specified number of linear segments. 
 *      If the profile consists of linear segments, the results will be polygons.
 *      The intent is to create simpler surface geometry &ndash; possibly planar faces. </li>
 * 
 * <li> <tt>sweep_angle</tt>: Revolve angle, in radians. Default is <tt>2.0*M_PI</tt>. </li>
 * 
 * <li> <tt>sweep_to_body</tt>: Defines a body that will limit the sweep operation. Default is <tt>NULL</tt>.
 * <br><br>
 *      <b>Note:</b> The <tt>sweep_to_body</tt> option and it is related options are mutually exclusive with the <tt>to_face</tt>
 *      option. The <tt>to_face</tt> option is used for backward compatibility. This option uses the surface of
 *      the face to locate the end of the sweep. It does not work with <tt>bool_type, keep_law</tt> or
 *      <tt>keep_branches</tt> options. These three options only work with the <tt>sweep_to_body</tt> option. The
 *      <tt>sweep_to_body</tt> option uses the entire body instead of just one surface. Combined with the
 *      related options, different parts of the resulting sweep will remain. If the <tt>to_face</tt> option is
 *      used, sweeping will unset the <tt>bool_type</tt>, <tt>keep_law</tt> and <tt>keep_branches</tt> options. </li>
 * 
 * <li> <tt>to_face</tt>: Defines a surface that will limit the sweep operation. Default is <tt>NULL</tt>. </li>
 * 
 * <li> <tt>twist_angle</tt>: Defines an angle that the profile will twist by the end of the sweep operation,
 *      in radians. Default is 0.0. </li>
 * 
 * <li> <tt>twist_law</tt>: Twist angle can be defined with a law. Default is <tt>NULL</tt>. </li>
 * 
 * <li> <tt>two_sided</tt>: If a open body is created, specifies it should consist of single-sided or double-sided faces. 
 *      Default is <tt>TRUE</tt>, for double-sided faces. An open body consisting of single-sided faces is an open solid body;
 *      an open body consisting of double-sided faces is a sheet body. </li>
 * 
 * <li> <tt>which_side</tt>: Specifies which side of a "back-to-back face sheet" to sweep. This is old functionality, not
 *      meaningful anymore. </li>
 * </ul>
 * <b>Note:</b> In general, the <tt>get_</tt> methods do not increment use counts or return copies of objects; therefore,
 * what they return should be treated as read-only; however, the <tt>get_</tt> methods that return a law <i>do</i> increment
 * the use count of the law. 
 * <br><br>
 * <b>Limitations:</b> If the <tt>rail_law</tt> array were set up using <tt>api_make_rails</tt> and it included a
 * twist law, then <tt>rail_law</tt> option would be mutually exclusive with the <tt>twist_law</tt> option. In other words,
 * do not specify the <tt>twist_law</tt> option. 
 * @see SPAposition, BODY, surface, law
 */
class DECL_SWP sweep_options : public ACIS_OBJECT
{
    sweep_bool_type     bool_type;
    logical             close_to_axis;
    int                 cut_end_off;
    sweep_options*      default_copy;
    double              draft_angle;
    draft_hole_option   draft_hole;
    double              draft_hole_angle;
    law*                draft_law;
    draft_repair_level  draft_repair;
    double              end_draft_dist;
    int                 gap_type;
    logical             keep_branches;
    law*                keep_law;
    logical             keep_start_face;
    int                 m_self_int_test;
    miter_type          miter;
    double              miter_amount;
    sweep_portion       portion;
    SPAposition         portion_end;
    SPAposition         portion_start;
    law**               rail_laws;
    int                 rail_num;
    logical             rigid;
    logical             rigid_twist_normal_from_profile;
    SPAunit_vector      rigid_twist_normal; //for rigid sweep with twist 
    law*                scale_law;
    logical             simplify;
    int                 solid;
    double              start_draft_dist;
    int                 steps;
    double              sweep_angle;
    BODY*               sweep_to_body;
    surface*            to_face;
    double              twist_angle;
    law*                twist_law;
    int                 two_sided;
    logical             which_side;
    merge_type          merge;
    logical             old_no_option;
    SPAunit_vector      *mAxis;
    void                init_variables();

public:

   /**
    * C++ allocation constructor requests memory for this object but does not populate it.
    */
    sweep_options();

   /**
    * C++ destructor, deletes a <tt>sweep_options</tt> object.
    */
    ~sweep_options();

   /**
    * Tests to see whether this <tt>sweep_options</tt> instance is equal to the given <tt>sweep_options</tt>.
    * @param in_sw_opt
    * <tt>sweep_options</tt> object to test.
    */
    int operator== (sweep_options& in_sw_opt);

   /**
    * Tests to see whether this <tt>sweep_options</tt> instance is not equal to the given <tt>sweep_options</tt>.
    * @param in_sw_opt
    * <tt>sweep_options</tt> object to test.
    */
    int operator!= (sweep_options& in_sw_opt);
    
   /**
    * Makes a copy of this <tt>sweep_options</tt> object and returns a pointer to it.
    */
    sweep_options *copy();

   /**
    * Makes a deep copy of this <tt>sweep_options</tt> object and all of its data. 
	* A new instance of a <tt>sweep_options</tt> object is
    * created that is the owner of all the internal rails data and responsible for its deletion.
    * The <tt>to_face</tt> and <tt>sweep_to_body</tt> are copied by reference.
    */
    sweep_options* deep_copy();

    /*
    // tbrv
    */
   /**
    * @nodoc
    */
    void fix_up( law *pathlaw);

    /*
    // tbrv
    */
   /**
    * @nodoc
    */
    void fix_up( COEDGE *profile,
                COEDGE *path,
                SPAtransf *to_profile=NULL);

   /**
    * Returns the current <tt>bool_type</tt> option values.
    */
    sweep_bool_type    get_bool_type() const;

   /**
    * Gets the current value of the <tt>close_to_axis</tt> option.
	* <b>Role:</b> The <tt>close_to_axis</tt> sweep option allows a solid body to be 
	* created from a rotationally swept, open wire body.
    * When <tt>TRUE</tt>, the ends of the wire body will be extended to the axis. 
	* Care should be taken so that all of the profile is on one side of the axis, 
	* to avoid a self-intersecting body. The default is <tt>FALSE</tt>.
    */
    logical            get_close_to_axis() const;

   /**
    * Gets whether the end is cut off or not.
    */
    int                get_cut_end_off() const;

   /**
    * Gets the draft angle.
    */
    double             get_draft_angle() const;

   /**
    * Gets the draft hole option.
    */
    draft_hole_option  get_draft_hole() const;

   /**
    * Gets the draft hole angle.
    */
    double             get_draft_hole_angle() const;

   /**
    * Gets the draft law.
    */
    law*               get_draft_law() const;

   /**
    * Gets the draft repair level.
    */
    draft_repair_level get_draft_repair() const;

   /**
    * Gets the end draft distance.
    */
    double             get_end_draft_dist() const;

   /**
    * Gets the gap type.
    */
    int                get_gap_type() const;

   /**
    * Gets whether or not branches are kept.
    */
    logical            get_keep_branches() const;

   /**
    * Gets the keep law.
    */
    law*                get_keep_law() const;

   /**
    * Gets whether the face used in the sweep is kept available after the sweep or not.
    */
    logical            get_keep_start_face() const;

   /**
    * Gets the miter amount.
	* This is only meaningful when using the @href miter_type <tt>bend_miter</tt>
    */
    double             get_miter_amount() const;

   /**
    * Gets the miter type.
    */
    miter_type         get_miter_type() const;

   /**
    * Gets the end portion of the curve used for sweeping.
    */
    SPAposition        get_portion_end() const;

   /**
    * Gets the start portion of the curve used for sweeping.
    */
    SPAposition        get_portion_start() const;

   /**
    * Gets a specific rail law.
    * @param which
    * array index of rail law.
    */
    law*               get_rail_law(int which) const;

   /**
    * Gets the size of the rail laws array.
    */
    int                get_rail_num() const;

   /**
    * Gets whether this is a rigid sweep or not.
    */
    logical            get_rigid() const;

   /**
    * Gets which twist direction will be used from profile normal or path
    * start direction.
    *
    */
    logical is_rigid_twist_normal_from_profile () const;

   /**
    * Gets the rigid twist normal vector
    *
    */
    SPAunit_vector get_rigid_twist_normal () const;

   /**
    * Gets the type of self-intersection test.
    */
    int               get_self_int_test() const;

   
   /**
	* Gets the scale law.
    */
    law*               get_scale_law() const;

	/**
    * Gets whether or not simplification is done.
    */
    logical            get_simplify() const;

   /**
    * Gets whether or not a solid is requested to be created.
    * <b>Role:</b> Gets whether a closed solid body should be created.
    * The return value will be -1 if the option has not yet been set, 0 if the option has
    * been set to <tt>FALSE</tt>, and 1 if the option has been set to <tt>TRUE</tt>.
    */
    int                get_solid() const;

   /**
    * Gets the starting draft distance.
    */
    double             get_start_draft_dist() const;

   /**
    * Gets the number of steps requested for a revolve operation.
    */
    int                get_steps() const;

   /**
    * Gets the sweep angle.
    */
    double             get_sweep_angle() const;

   /**
    * Gets the sweep portion.
    */
    sweep_portion      get_sweep_portion() const;

   /**
    * Gets the body swept to.
    */
    BODY*               get_sweep_to_body() const;

   /**
    * Gets the face swept to.
    */
    surface*            get_to_face() const;

   /**
    * Gets the twist angle.
    */
    double             get_twist_angle() const;

   /**
    * Gets the twist law.
    */
    law*                get_twist_law() const;

   /**
    * Gets whether a double-sided sheet body should be created.
    * <b>Role:</b> Gets whether a double-sided sheet body should be created.
    * The return value will be -1 if the option has not yet been set, 0 if the option has
    * been set to <tt>FALSE</tt>, and 1 if the option has been set to <tt>TRUE</tt>.
    */
    int				   get_two_sided() const;
    
    //	const AcisVersion  get_version() const;
    
   /**
    * Gets which side of the profile should be swept.
    */
    logical            get_which_side() const;
    
   /**
    * Returns if this is an old sweep type: a sweep without a <tt>sweep_option</tt> object.
    */
    logical is_old_no_option_sweep () const;
    
   /**
    * Returns the axis vector when sweeping along a constant axis; 
    * returns <tt>NULL</tt> otherwise.
    */
    SPAunit_vector *get_axis ();
    
   /**
    * Refreshes (resets) all option values to their defaults.
    */
    void refresh();

   /**
    * Internal function used to set the default copy rail law to <tt>NULL</tt>.
    * <b>Role:</b> Because the rail law is particular to the path used it must be set each time by
    * the user, or the default rails will be used.
    */
    void reset_default_copy_rail_laws();

    /*
    // tbrv
    */
   /**
    * @nodoc
    */
    void reset_default_to_face(surface*, sweep_bool_type, law*, logical);

   /**
    * Sets the option for the Boolean operation type with the <tt>to_body</tt>.
    * <b>Role:</b> The sweep option <tt>bool_type</tt> specifies what should be done with the pieces when
    * sweeping to or through a fixed body. This is used as part of Selective Booleans. The <tt>bool_type</tt>
    * also controls the numbering on the cells of the tool body and the blank body. The number of
    * cells can affect the results of a defined <tt>keep_law</tt>. Default value is <tt>LIMIT</tt>. 
    * @param b
    * type of Boolean operation: <tt>UNITE</tt>, <tt>INTERSECT</tt>, <tt>SUBTRACT</tt>, <tt>KEEP_BOTH</tt>, or <tt>LIMIT</tt>.
    */
    void set_bool_type(sweep_bool_type b);

   /**
    * The sweep option <tt>close_to_axis</tt> allows a solid body to be created from a rotationally swept, open wire body.
    * <b>Role:</b> This method sets open wires to be swept around an axis to create a solid body.
    * When <tt>TRUE</tt>, the ends of the wire body will be extended to the axis. Care should be taken so that all of the
    * profile is on one side of the axis to avoid a self-intersecting body. The default is <tt>FALSE</tt>.
    * <br><br>
    * The option <tt>close_to_axis</tt> may be used only if the path is specified as a position and an axis of
    * revolution.
    * @param c
    * <tt>TRUE</tt> make a solid.
    */
    void set_close_to_axis(logical c);

   /**
    * The sweep option <tt>cut_end_off</tt> determines whether or not the end of the swept surface should be cut off squarely.
    * <b>Role:</b> When <tt>TRUE</tt>, the end of sweeping is planar and perpendicular to the path. When
    * <tt>FALSE</tt>, the sweeping operation is faster, and a nonplanar profile is not cut off at the end of
    * the sweep path. The default is <tt>FALSE</tt>.
    * @param c
    * <tt>TRUE</tt> to cut perpendicular to sweep path.
    */
    void set_cut_end_off(int c);

   /**
    * Sets the draft angle.
    * <b>Role:</b> Sets the angle (in radians) with which the swept profile is to draft out (positive) or in (negative) while sweeping.
    * <tt>set_draft_angle()</tt>, <tt>set_draft_law()</tt>, <tt>set_start_draft_dist()</tt> and <tt>set_end_draft_dist()</tt> are 
	* mutually exclusive, i.e. setting one will unset the others.
    * @param angle
    * draft angle.
    */
    void set_draft_angle(double angle);

   /**
    * Controls the offset behavior of internal loops (or holes) of a profile face when used to sweep with draft.
    * <b>Role:</b> The default is <tt>AGAINST_PERIPHERY</tt>, in which holes are swept with the opposite draft
    * direction as the periphery loop.
    * @param in_draft_hole
    * draft hole option to set:  <tt>AGAINST_PERIPHERY</tt>, <tt>ANGLE</tt> <i>value</i>, <tt>NO_DRAFT</tt>, or <tt>WITH_PERIPHERY</tt>.
    * @param angle
    * draft hole angle (radians).
    */
    void set_draft_hole(draft_hole_option in_draft_hole, double angle=0);

   /**
    * Sets the law to be used to calculate the draft angle behavior.
    * <b>Role:</b> Sets a law pointer that is the offset draft distance as a function of the "pseudo-length" along the path.
    * The <tt>pseudo-length</tt> is the parameter fraction along the path times the true length of the path.
    * <br><br>
    * <tt>set_draft_angle()</tt>, <tt>set_draft_law()</tt>, <tt>set_start_draft_dist()</tt> and <tt>set_end_draft_dist()</tt> are mutually exclusive, 
	* i.e. setting one will unset the others.
    * @param law_draft
    * law for drafting.
    */
    void set_draft_law(law* law_draft);

   /**
    * Sets the level of detection for degeneracy and self-intersection during sweeping with draft.
    * <b>Role:</b> Sets the level of detection for degeneracy and self-intersection during sweeping with draft.
    * The default is <tt>DEGENERACY</tt>.
    * <br><br>
    * In ACIS, the sweep-with-draft surfaces are created by continuously offsetting the original
    * profile along the path. If the offset amount is too large, an edge of the profile could be
    * completely trimmed by adjacent edges. The sweep option <tt>draft_repair</tt> handles this problem. The
    * option also handles the problem when sweeping a profile causes self-intersections.
    * @param l
    * level of detection: <tt>NO_REPAIR</tt>, <tt>ERROR_ON_REPAIR</tt>, <tt>DEGENERACY</tt>, 
	* <tt>FIRST_DEGENERACY</tt>, <tt>SELF_INTERSECTION</tt>, or <tt>FIRST_SELF_INTERSECTION</tt>.
    */
    void set_draft_repair_level(draft_repair_level l);

   /**
    * Specifies the offset distance at the end of the sweep operation.
    * <b>Role:</b> Specifies an offset distance in the plane of the profile where the swept
    * surface is to end. 
	* <br>
	* <tt>set_start_draft_dist()</tt> and <tt>set_end_draft_dist()</tt> are generally used together.
    * @param end_dist
    * end offset distance.
    */
    void set_end_draft_dist(double end_dist);

   /**
    * Sets the desired method for filling in gaps created by offsetting during a draft operation.
    * @param g
    * gap type: valid values are <tt>NATURAL</tt> (2), <tt>ROUNDED</tt> (1), and <tt>EXTENDED</tt> (0).
    */
    void set_gap_type(int g);

   /**
    * Sets whether or not to keep branches from <tt>sweep_to body</tt> and <tt>to_face</tt> operations.
    * <b>Role:</b> The <tt>keep_branches</tt> option specifies whether or not graph branches should be kept.
    * The <tt>keep_law</tt> option specifies in an ordered graph which items to keep. In this case, <tt>TRUE</tt>
    * means that all branches are kept. The <tt>sweep_to_body</tt> option must be specified when using
    * <tt>keep_branches</tt>.
    * <br><br>
    * The <tt>keep_branches</tt> and <tt>keep_law</tt> options work together.
    * @param k
    * <tt>TRUE</tt> to keep all graph branches.
    */
    void set_keep_branches(logical k);

   /**
    * Specifies which portions of a graph are to be kept.
    * <b>Role:</b> Specifies which portions of a graph are to be kept.
    * Any law, including conditional tests, can be used to specify what is used for the
    * sweep operation. When <tt>keep_law</tt> is <tt>TRUE</tt>, all graph pieces are saved. (For more information on
    * graph theory, refer to the <i>Graph Theory</i> Technical Article. For more information on using branches and 
    * keep laws, refer to the <i>Branches</i> and <i>Using the Keep Law</i> sections of the <i>Sweep To</i> Technical Article.)
    * @param law_keep
    * law to keep.
    */
    void set_keep_law(law* law_keep);

   /**
    * Specifies that the face used to sweep remains available after the sweep operation.
    * <b>Role:</b> This is useful when the sweep path is a closed loop, or when selective Booleans
    * will be used after the sweeping process.
    * <br><br>
    * For example, a circular profile can be swept around a circle, resulting in a torus. By default,
    * once the %torus is created, the start face is removed, along with information about where the
    * toroidal %surface starts and ends. When <tt>keep_start_face</tt> is turned on, the start face is left as
    * part of the resulting model. This acts as a "handle" for the start and end location. If the
    * swept %torus were to be used in a Selective Boolean process, the handle would be necessary. For
    * example, the API function @href api_boolean_tube_body makes use of the start face.
	* <br><br>
    * This face may not be the same object that was passed into the API function. 
    * <br><br>
    * The <tt>sweep_to_body</tt> option should not be used with <tt>keep_start_face</tt>.
    * @param k
    * <tt>TRUE</tt> to keep the start face.
    */
    void set_keep_start_face(logical k);

   /**
    * Specifies how mitering is performed.
    * <b>Role:</b> Specifies how mitering is performed. The default is the enumeration value <tt>default_miter</tt>, which takes the system
    * default. Other types include <tt>new_miter</tt>, <tt>old_miter</tt>, <tt>crimp_miter</tt> and <tt>bend_miter</tt>.
    * @param m
    * type of mitering:	<tt>default_miter</tt>, <tt>new_miter</tt>, <tt>old_miter</tt>, <tt>crimp_miter</tt> or <tt>bend_miter</tt>.
    */
    void set_miter(miter_type m);

   /**
    * Sets the minimum radius for using the @href miter_type <tt>bend_miter</tt>.
    * @param m
    * miter radius.
    */
    void set_miter_amount(double m);

   /**
    * Sets the end of the portion of the curve used for sweeping.
    * @param p
    * given end position.
    */
    void set_portion_end(const SPAposition &p );

   /**
    * Sets the start of the portion of the curve used for sweeping.
    * @param p
    * given start position.
    */
    void set_portion_start(const  SPAposition &p );

   /**
    * Specifies the orientation of the profile as it is swept.
    * <b>Role:</b> Specifies the orientation of the profile as it is swept.
    * A rail law is simply a vector field along the sweep path. The field is made of
    * unit vectors that are perpendicular to the path.
    * <br>
    * The default rail law for a non-planar path is minimum rotation,
    * (e.g., the minor law function). For a planar path, the default rail law is a constant vector
    * equal to the planar normal.
    * @param rail_law
    * given rail law.
    * @see set_rail_laws()
    */
    void set_rail_law(law* rail_law);

   /**
    * Sets an array of rail laws.
    * <b>Role:</b> Sets an array of rail laws. A rail law is simply a vector field along the sweep path. The field is made of
    * unit vectors that are perpendicular to the path. An array of rail laws supplied to the sweep
    * operation dictates how the profile is to be oriented on the path. The size of the rail law
    * array is determined by the number of segments in the path. Each path segment has its own
    * associated rail law.
    * @param rail_law
    * array of rail laws.
    * @param size
    * size of array.
    */
    void set_rail_laws(law** rail_law, int size);

   /**
    * Specifies whether or not to make the sweep cross-sections parallel to one another.
    * <b>Role:</b> Specifies whether or not to make the cross-sections of a sweep parallel to one another.
    * The default is <tt>FALSE</tt>, which means that the cross-sections are perpendicular to the sweep path.
    * @param r
    * flag specifies whether a rigid sweep is to be performed. 
    */
    void set_rigid(logical r);

   /**
    * Specifies which twist direction will be used from profile or path.
    * @param iFromProfile
    *    <tt>TRUE</tt>:  The profile normal will be used.<br>
    *    <tt>FALSE</tt>: The path start tangent direction will be used. (Default behavior.)
    */
    void set_rigid_twist_normal_from_profile (const logical &iFromProfile);

   /**
    * Specifies the normal direction in a rigid twist sweep.
    * <b>Role:</b> Specifies the normal direction in a rigid twist sweep. Nomally this is the start
    * tangent direction of the path.
    * @param iNorm
    * the direction for the rigid twist.
    */
    void set_rigid_twist_normal (const SPAunit_vector &iNorm);

	/**
	 * Specifies the amount of scaling in the <tt>xyz</tt> direction in the form of a vector law.
	 * <b>Role:</b> Scaled sweep along the sweep path can be performed by specifying the amount of scale in the form of vector law.
	 * The <tt>x</tt> direction is in the direction of the rail; the <tt>z</tt> direction is in the
	 * direction of the tangent of the path; and the <tt>y</tt> direction is in the direction <tt>z</tt> cross <tt>x</tt>.
	 * <br><br>
	 * The scale provided should be uniform in the profile plane i.e. in the <tt>xy</tt> direction and
	 * should be unit scale along the path tangent i.e. in the <tt>z</tt> direction.
	 * The scale law can take one of the two forms:
	 * <ul>
	 * <li> <b>Constant scale law:</b> Specifies the amount of scale to be achieved at the end of the sweep path. Vector law takes
	 *      the form (a,a,1), where "a" is a constant. For <tt>a</tt> greater than one, we have upscaling and for <tt>a</tt> less than one,
	 *      we have downscaling. When <tt>a</tt> equals to one, scale is not considered. </li>
	 * <li> <b>Non-constant scale law:</b> Specifies the scale law as a function of the "pseudo-length" along the path.
	 *      The <tt>pseudo-length</tt> is the parameter fraction along the path times the true length of the path.
	 *      Vector law takes the form (a*x,a*x,1) or (1+a*x,1+a*x,1), where "a" is a constant and "x" is the <tt>pseudo-length</tt>.
	 *      The scale law input (a*x,a*x,1) is implicitly converted to (1+a*x,1+a*x,1), as the scale value at the start of the path,
	 *      should always be a unit scale. </li>
	 * </ul>
	 * <br><br>
	 * <b>Note:</b> For constant scale law, scale should not be set to (0,0,1). For non-constant scale law, scale should not take the value (0,0,1)
	 * along the sweep path. Sweep with scale option is mutually exclusive with the sweep with draft options.
	 * @param scale_law
	 * law for scaling.
	 */
    void set_scale_law(law* scale_law);

   /**
    * Sets the type of self-intersection testing.
    * <b>Role:</b> Set the type of self-intersection testing. This test is performed at the moment of
    * the creation of surfaces. Valid values are: 0 (No checking), 1 check and throw error, 2 check and throw warning (body will contain
    * self-intersecting surfaces). Default is 1.
    * @param type
    * type of check (0, 1, or 2).
    */
    void set_self_int_test(int type);

   /**
    * Simplify swept surfaces to analytic surfaces whenever possible.
    * <b>Role:</b> Simplify swept surfaces to analytic surfaces whenever possible.
    * Default is <tt>TRUE</tt>. This method is primarily intended for internal use. Performance 
	* of subsequent operations may be reduced if this option is set to <tt>FALSE</tt>.
    * @param s
    * simplify surface when <tt>TRUE</tt>.
    */
    void set_simplify(logical s);

   /**
    * Specifies whether or not the result of sweeping a closed wire profile is intended to be a solid or a sheet body.
    * <b>Role:</b> Specifies whether or not the result of sweeping is intended to be a solid or a sheet body.
    * If <tt>solid</tt> is set to <tt>FALSE</tt> and <tt>two_sided</tt> is set to <tt>TRUE</tt>, then the sweep operation
    * creates a double-sided sheet body. If the intended result is a single-sided sheet body, then both
    * the <tt>solid</tt> and <tt>two_sided</tt> options should be set to <tt>FALSE</tt>.
    * @param s
    * flag for creating a solid body.
    */
    void set_solid(int s);

   /**
    * Specifies the offset distance at the start of the sweep operation.
    * <b>Role:</b> Specifies an offset distance in the plane of the profile where the swept
    * surface is to start.
    * <br>
	* <tt>set_start_draft_dist()</tt> and <tt>set_end_draft_dist()</tt> are generally used together.
    * @param st_dist
    * start offset distance.
    */
    void set_start_draft_dist(double st_dist);

   /**
    * Specifies to convert a circular sweep path into the specified number of linear segments.
    * <b>Role:</b> Specifies to convert a circular sweep path into the specified number of linear segments.
    * The results are polygons, and the intent is to create simpler geometry by keeping
    * faces planar. The default is 0.
    * <br><br>
    * This option may only be used when specifying the path as a position and axis of revolution.
    * @param s
    * number of linear segments.
    */
    void set_steps(int s);

   /**
    * Specifies the angle to sweep around an axis, in radians.
    * <b>Role:</b> Specifies the angle to sweep around an axis, in radians. The default is <tt>2.0*M_PI</tt>.
	* <br><br>
    * If the specified angle is positive, then sweep occurs in a counter-clockwise direction.
    * If the specified angle is negative, then the sweep occurs in a clockwise direction.
    * @param s
    * angle in radians.
    * <br><br>
    */
    void set_sweep_angle(double s);

   /**
    * Sets the portion of the path to be used in the sweeping operation.
    * <b>Role:</b> The sweep option <tt>portion</tt> permits localizing how much of the path is used for sweeping the profile.
    * When positions are specified to localize the region for sweeping, the positions
    * do not have to lie on the path. The closest point on the path from the specified position is
    * used as the sweep limit.
    * <br><br>
    * The default is <tt>ENTIRE_PATH</tt>. Other options include <tt>BETWEEN_POINTS</tt>, <tt>FROM_PROFILE</tt>, <tt>TO_PROFILE</tt>, 
    * <tt>SWEEP_TO</tt>, and <tt>AS_IS</tt>. <tt>AS_IS</tt> is provided for backward compatibility.
    * @param por_opt
    * portion option: <tt>ENTIRE_PATH</tt>, <tt>BETWEEN_POINTS</tt>, <tt>FROM_PROFILE</tt>, <tt>TO_PROFILE</tt>, 
    * <tt>SWEEP_TO</tt>, and <tt>AS_IS</tt>.
    * @param in_st
    * start position.
    * @param in_end
    * end position.
    */
    void set_sweep_portion( sweep_portion por_opt,
        SPAposition &in_st= *(SPAposition *) NULL_REF,
        SPAposition &in_end= *(SPAposition *) NULL_REF);
    
   /**
    * Specifies a pointer to the body where sweeping is to finish.
    * <b>Role:</b> Specifies a pointer to the body where sweeping is to finish.
    * The <tt>sweep_to_body</tt> option is similar to the <tt>to_face</tt> option except that multiple faces on a body can be
    * used to clip the sweep result properly. Equivalent results would be obtained if a profile were
    * swept through a body, and if then the two bodies were Boolean united together. The default is <tt>NULL</tt>.
    * <br><br>
    * The <tt>sweep_to_body</tt> option is mutually exclusive with <tt>to_face</tt>. The <tt>sweep_to_body</tt> option should
    * not be used with <tt>keep_start_face</tt>. The <tt>sweep_to_body</tt> option must be specified when using <tt>keep_law</tt> or
    * <tt>keep_branches</tt>.
    * <br><br> 
	* Boolean operations are used to get this result. The swept body can be contained in the <tt>to_body</tt> object.
	* <br><br> 
	* For additional information on the <tt>sweep_to_body</tt> option see the Technical Article <i>Sweep To</i>.
    * @param b
    * pointer to termination body object.
    */
    void set_sweep_to_body(BODY* b);

   /**
    * Sets a surface to which the swept body will be clipped.
    * <b>Role:</b> Sets a surface to which the swept body will be clipped. 
    * The <tt>to_face</tt> option must be explicitly specified. The default is <tt>NULL</tt>. The <tt>to_face</tt>
    * option is mutually exclusive with the <tt>sweep_to_body</tt> option.
    * @param surf
    * surface for clipping.
    */
    void set_to_face(surface* surf);

   /**
    * Sets the angle that the profile twists in total as the profile is swept along the path, regardless of the length of the path.
    * @param angle
    * law for angle.
    */
    void set_twist_angle(double angle);

   /**
    * Sets the angle that the profile will be rotated along the path, as a function of the path length.
    *<b>Role:</b> Sets the angle that the profile will be rotated along the path, as a function of the path length.
    * The specified law should return the angle of twist in radians as a function of the length of the wire, 
	* starting at wire length equal to zero for the beginning of the wire.
    * @param twist_law
    * law for twist.
    */
    void set_twist_law(law* twist_law);

   /**
    * Sets whether or not the resulting sheets will be double-sided or not.
    * <b>Role:</b> Sets whether or not the resulting sheets will be double-sided or not. 
    * If <tt>solid</tt> is set to <tt>FALSE</tt> and <tt>two_sided</tt> is set to <tt>TRUE</tt>, then the sweep operation
    * creates a double-sided sheet body. If the intended result is a single-sided sheet body, then both
    * the <tt>solid</tt> and <tt>two_sided</tt> options should be set to <tt>FALSE</tt>.
    * @param t
    * <tt>TRUE</tt> for two-sided sheet body.
    */
    void set_two_sided(logical t);

   /**
    * Establishes which side of the profile should be swept. (Old functionality.)
    * @param w
    * flag for which side.
    */
    void set_which_side(logical w);

   /**
    * Gets the merge type.
    */
    merge_type         get_merge_type() const;

   /**
    * Sets the merge type.
    * <b>Role:</b> Sets the type merging to be performed. 
	* Valid values are: <tt>merge_none</tt>, <tt>merge_lateral</tt>, <tt>merge_vertical</tt>, 
	* <tt>merge_all</tt>, and <tt>merge_unset</tt>. Default is <tt>merge_all</tt>.
    * @param merge
    * flag for merging.
    */
    void         set_merge_type(merge_type merge);

   /**
    * Sets the old "no option" sweep type.
    * @param iOldWay
    *    <tt>TRUE</tt>:  The sweep is performed the old way, which initially didn't have a <tt>sweep_options</tt> object.<br>
    *    <tt>FALSE</tt>: New way, which has a <tt>sweep_options</tt> object. (Default behavior.)
    */
    void set_old_no_option_sweep (const logical iOldWay);

   /**
    * Sets the axis vector when sweeping along a constant axis.
    * @param iAxis
    * axis vector
    */
    void set_axis (const SPAunit_vector &iAxis);
    
};
/** @} */
#endif
