/* ORIGINAL: acis2.1/spline/bs3_crv/fit.hxx */
// $Id: fit.hxx,v 1.28 2002/08/09 17:15:41 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Classes to support curve fitting. The basic class contains all the
// (virtual) methods required by the generic curve fitting routine,
// plus space for the object-space curves, and surface-related
// information, which come in separate classes. Derived
// classes will add type-specific information, and implement the
// methods suitably.

// At present the "methods" mentioned above are in fact two:
// one, "true_point", which takes a guess at a point on the required
// curve, and constructs a nearby accurate one, together with the
// corresponding curve direction, and surface SPAparameter values where
// appropriate, and "make_int_cur", which transfers fitted data to a
// new interpolated curve object, of a derived type corresponding to
// the derived type of curve_interp.

#if !defined( BS3_CURVE_FIT )
#define BS3_CURVE_FIT

#include "dcl_spl.h"
#include "logical.h"

#include "bs2curve.hxx"
#include "bs3curve.hxx"

#include "debugmsc.hxx"

#include "position.hxx"
#include "unitvec.hxx"
#include "param.hxx"

/**
* @file fit.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup KERNAPI
 *
 * @{
 */

class SPAbox;
class surface;
class interp_obj_data;
class interp_surf_data;
class point_data;
class int_cur;
class curve;

/**
 * Contains arrays to be interpolated and the information necessary for the interpolation.
 * <br>
 * <b>Role:</b> The main way of constructing a new intcurve (as opposed to a copy of an
 * existing one), and the only way to make one with an int_cur of a derived type, is
 * using an object of the <tt>curve_interp</tt> class, or a class derived from it. This class
 * contains arrays of points to be interpolated, and the information necessary for the
 * interpolation. Each derived class must supply two virtual functions, <tt>true_point</tt> specifies
 * how the interpolation information is used, and <tt>make_int_cur</tt> constructs an <tt>int_cur</tt> of the
 * appropriate derived class, which is usually defined at the same time as the derived
 * <tt>curve_interp</tt> class. This class can also take parameter values at the points that are
 * interpolated. These values can be given as an array of doubles in the data member param.
 * <br><br> The first task in constructing an intcurve is to generate a sequence of points along
 * the true curve in some context-dependent way, subject to certain conditions discussed later.
 * For each point there must be a position in object space, a curve direction, and possibly
 * one or more positions in the parameter space of any surfaces involved. All of this
 * information goes into the base class-any further information needed by the virtual functions
 * supplied by the application can be added to the derived class. The (derived) <tt>curve_interp</tt>
 * object is then passed to the intcurve constructor, together with an optional region of
 * interest, resulting in the interpolating curve of the correct type.
 * <br><br>
 * <b>RestrictionsonInputPointLists:</b>
 * <br>The current curve fitting algorithm takes points pair wise from the input point lists,
 * constructs a Hermite interpolant, cubic in object space, and quadratic in parameter space,
 * and tests its mid-point for a valid fit. On failure, it subdivides the interpolant into two
 * at the true mid point and tries again for each half. At each stage it tests the box containing
 * the span end points and the points of nearest approach of its two end tangents against a
 * supplied region of interest. If there is no overlap, it assumes that the true curve between
 * those end points lies entirely outside the region of interest, and does not attempt to fit
 * the spline any further. The application must ensure that the initial points supplied in the
 * <tt>curve_interp</tt> object are near enough so that all these operations and assumptions are valid.
 * Although there are efficiency/reliability trade-offs to be considered, it is unlikely that
 * a precise analysis of boxing would be justified.
 * <br><br>
 * <b>Hermite interpolation:</b>
 * <br>This involves determining the points of nearest approach of the end tangents in object
 * space, and determining the distance (measured algebraically along the tangent direction)
 * from the start point to point of nearest approach on the start tangent, and from the point
 * of nearest approach on the end tangent to the end point. For the interpolation to be
 * successful, both distances must be positive, and their ratio should not be too large or
 * small--for example, a factor of 10. Similarly, in each parameter space, the intersection
 * point of the end tangents should lie on the correct side of the end points to ensure that
 * the Bezier quadratic defined by these three control points has the correct initial and final
 * tangent directions.
 * <br><br>
 * <b>True point finding:</b>
 * <br> It is essential that the mid-point of the initial Hermite fit is close enough to the
 * true curve for the application-supplied <tt>true_point</tt> function to find a valid point. It will
 * normally need to be closer to the required curve than to any other curve satisfying the
 * interpolation conditions, and there should not be any high-frequency undulations in the
 * surfaces between the initial approximation and the true curve. This condition is impossible
 * to specify or test precisely, and so is subject to a series of heuristics. The main one
 * currently used is to reduce the allowable angle between the end tangent directions of a span
 * to roughly 30 degrees. This, together with the hard point order requirements for the initial
 * Hermite interpolation, has proved effective for surface-surface intersections and silhouette
 * lines in ACIS so far.
 * <br><br>
 * <b>Boxing:</b>
 * <br> This requires that the whole of the span of the true curve between the given end points
 * lies within the box containing those two end points and the points of nearest approach between
 * the end tangents. As a special case, it is permissible for the end points of a nonperiodic
 * curve to have zero-length direction vectors. This is for the case that the curve direction
 * is ill-defined by the normal procedure, and it is inconvenient to go to higher order.
 * With an intersection curve, for example, the surfaces may be tangent, so that the curve
 * direction can only be determined by a second-order method. In this case, the interpolation
 * process continually adjusts the end direction of the curve to give zero curvature there, then
 * calling <tt>true_point</tt> with that direction to adjust the direction to be valid if it only has one
 * degree of freedom. This appears to be effective and cheap.
 *<br><br>
 * @see point_data, point_obj_data, point_surf_data,SPAinterval
 */

class DECL_SPLINE curve_interp : public ACIS_OBJECT {
public:
	// Input data
/**
 * The number of points to be interpolated.
 * <br>
 * <b>Role:</b> If the curve is periodic, this number is negative; i.e., the last point and
 * the direction are the same as the first point and direction. All arrays, both object-space
 * and parameter-space, are of this length.
 */
 	int npts;					// number of points to be interpolated,
								// negated if the curve is periodic
								// (i.e. the last point and direction
								// are the same as the first). All
								// arrays, both object-space and
								// SPAparameter-space, are of this length.
/**
 * The tolerance allowed on fitted splines.
 */
	double fitol;				// tolerance allowed on fitted splines
	double fitol_result;		// resulting 3D tolerance on fitted splines
	double partol_result;		// resulting 2D tolerance on fitted pcurve 1
/**
 *
 * The parameter values at the given points.
 * <br>
 * <b>Role:</b> This value is <tt>NULL</tt> if the fitting process chooses its own parameter values.
 */
	double const *param;		// the SPAparameter values at the given
								// points, or NULL if the fitting
								// process is to choose its own
								// SPAparameter values.

	// Object- and SPAparameter-space data objects
/**
 * The number of object-space curves being interpolated.
 */
 	int nobj;					// Number of object-space curves being
								// interpolated.
/**
 * The pointer to an array of object-space curve data records.
 */
 	interp_obj_data *objdata;	// pointer to an array of object-space
								// curve data records.
/**
 * The number of surface-related records.
 */
 	int nsurf;					// number of surface-related records
								// in use.
/**
 * The pointer to an array of objects describing surface-related information.
 */
 	interp_surf_data *sfdata;	// pointer to an array of objects
								// describing surface-related
								// information.

	// Output from the fit process. Only those portions of the first
	// object-space curve which are thought to be within the region
	// of interest are fitted - anything known to be outside is simply
	// left alone as soon as it is found to be outside. Subsidiary
	// object-space curves are fitted in the same SPAparameter intervals
	// as the first curve.
/**
 * The array of parameter intervals within which the fit is in tolerance.
 * <br>
 * <b>Role:</b> Portions outside these intervals are entirely outside the region
 * of interest, and so they may be well outside the tolerance. This is always kept
 * in numerical order and disjoint.
 */
 	SPAinterval *valid;			// array of SPAparameter intervals within
								// which the fit is (thought to be)
								// in tolerance. Portions outside
								// these intervals are entirely outside
								// the region of interest, and so may
								// be well outside tolerance. Always
								// kept in numerical order and disjoint
/**
 * The number of intervals in valid.
 */
 	int nvalid;					// the number of intervals in "valid"

	// Constructor for the simple case of a single object-space
	// curve. The point lists and tolerance are required for
	// all interpolations, so we require them for the base class
	// constructor to ensure that derived classes cannot forget them.
	// Everything else is initialised null, for cleanliness.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Creates a curve interpolation by accepting a list of positions and tangent
 * directions for the curve that is to be interpolated or fit, depending upon whether the
 * tolerance is 0.
 * <br><br>
 * @param arr_ent
 * number of array entries.
 * @param arr_pt
 * array of points.
 * @param arr_tan
 * array of tangents.
 * @param tol
 * fit tolerance.
 * @param surf
 * number of surface-related objects
 */
 	curve_interp(
				int arr_ent,		// the number of entries in the following
							// two arrays
				SPAposition const *arr_pt ,
							// array of points to be interpolated
				SPAvector const *arr_tan,
							// array of tangent directions and
							// magnitudes at the points to be
							// interpolated
				double tol,		// fit tolerance required
				int surf = 0		// number of surface-related objects to
							// allocate
			);

	// Constructor for the general case of a multiple object-space
	// curves and surfaces.
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param entry
 * number of array entries.
 * @param tol
 * fit tolerance.
 * @param cur
 * number of object-space curves
 * @param surf
 * number of surfaces.
 */
 	curve_interp(
				int entry,		// the number of entries in the point
							// arrays which will be set by the
							// caller
				double tol,		// fit tolerance required
				int cur = 0,	// the number of object-space curves to
							// be interpolated
				int surf = 0		// the number of surfaces (or non-surface
							// SPAparameter curves)
			);

	// Destructor to ensure that dependent data are removed adequately.

	virtual ~curve_interp();

	// Main user entry, to fit object-space splines and possible
	// SPAparameter-space splines to the given initial lists of points.
/**
 * Fits the object-space splines and possible parameter-space splines to the specified initial lists of points.
 * <br><br>
 * @param region
 * given precision region.
 */
 	void fit(
			SPAbox const &region		// region within which we require the
							// given precision
		);

	// Extract the nth object-space curve after fitting. The result
	// becomes the property of the caller, and subsequent calls return
	// NULL.
/**
 * Extracts the nth object-space curve after fitting.
 * <br><br>
 * <b>Role:</b> The result becomes the property of the caller, and subsequent calls return <tt>NULL</tt>.
 * <br><br>
 * @param cur
 * object-space curve.
 */
 	bs3_curve obj_bs( int cur = 0 );

	// Extract the actual fit tolearance achieved for the given object-
	// space curve.
/**
 * Extract the actual fit tolerance achieved for the given object-space curve.
 * <br><br>
 * @param tol
 * tolerance.
 */
 	double obj_fitol( int tol = 0 );

	// Extract the nth SPAparameter-space curve after fitting. The result
	// becomes the property of the caller, and subsequent calls return
	// NULL.
/**
 * Extracts the nth parameter-space curve after fitting.
 * <br><br>
 * <b>Role:</b> The result becomes the property of the caller, and subsequent calls return <tt>NULL</tt>.
 * <br><br>
 * @param cur
 * parameter-space curve.
 */
 	bs2_curve par_bs( int cur );

	// Extract the actual fit tolearance achieved for the given parameter-
	// space curve.

	double par_obj_fitol( int = 0 );

	// Extract the nth valid SPAinterval from the object (n from 0 to
	// nvalid - 1). Return NULL for n outside that range.
/**
 * Extracts the nth valid interval from the object, where n ranges from 0 to nvalid - 1.
 * <br><br>
 * <b>Role:</b> This method returns <tt>NULL</tt> if n is outside the range.
 * <br><br>
 * @param interval
 * valid interval.
 */
 	SPAinterval const *valid_range( int interval  );

	// *** Virtual functions to be provided by the user in his
	// *** derived classes.

	// Function which given an initial guess at a point on the
	// required curve finds an accurate near point, together with the
	// curve direction. Does this in object space and the SPAparameter
	// space of every relevant parametric surface. This must be
	// supplied for every class derived from this one, and constitutes
	// the only information about the true curve required by the
	// fitting process.
/**
 * Finds the true-point in 3D for a given parameter value.
 * <br><br>
 * <b>Role:</b> The input position, direction, and parameter values are approximate;
 * the exact values are provided as output.
 * <br><br>
 * @param tol
 * tolerance.
 * @param data
 * point data.
 */
 	virtual void true_point(
			double tol,				// allowable tolerance on the point
			point_data &data		// initial guess updated by routine
		) const = 0;

	// Function to construct an int_cur (or a derived object) to
	// represent the fitted curve. Used by the intcurve constructor
	// for fitting curves to point lists. This function must be
	// provided for every class derived from this one, and will
	// construct the appropriate object to represent that type of
	// curve, normally derived from the base class int_cur.
/**
 * Constructs an <tt>int_cur</tt> to represent the fitted curve.
 * <br><br>
 * <b>Role:</b> This is used by the intcurve constructor for fitting curves to
 * the point lists. This method must be provided for every class derived from this one,
 * and it constructs the appropriate object to represent that type of curve, normally
 * derived from the base class, <tt>int_cur</tt>.
 */
 	virtual int_cur *make_int_cur() = 0;

    // Returns NULL unless overridden in a derived class (i.e., proj_int_interp)
/*
// tbrv
*/
/**
 * @nodoc
 */
    virtual curve const *proj_cur() const
        { return NULL; }


private:
	// The following functions are used internally by "fit".

	// Initialise a new current segment from give start and end
	// points.

	void init_seg(
			point_data &,
			point_data &
		);

	// Evaluate the current segment at a SPAparameter value

	void eval_seg( double, point_data & ) const;

	// Test a span of the current segment, and refine it if it is not
	// a good enough fit.

	void refine_seg(
			point_data const &,	// point and direction information for
								// start point of this span
			point_data const &,	// same for end of span
			double,				// required fit tolerance for curve
			SPAbox const &			// we are only interested in portions
								// of this curve within the SPAbox
		);

	// Split the current segment at a given mid-point, splitting
	// any spline which needs it, and returning a flag to say if any
	// was split.

	logical split_seg(
			point_data const &,
			point_data const &,
			point_data const &
		);

	// Set the min-max boxes on the current segment splines.

	void box_seg();

	// Merge a new segment into the main spline, including all
	// relevant SPAparameter-space splines.

	void merge_seg();

	// Adjust the parametrisation of each segment to ensure
	// continuity of first derivative (if necessary), and clean
	// up the whole curve if possible.

	void cleanup_results();

	// Mark the splines as periodic, if the request is for a periodic
	// curve.

	void mark_periodic();

	// Print out the initial points

	void debug_input( FILE * = debug_file_ptr ) const;

	// Print out the final splines.

	void debug_result( FILE * = debug_file_ptr ) const;

protected:

	// To check if the middle uv lies outside the 2d boundary of
	// the surface. It also fixes by making a linear interpolation
	// between the start and the end

	virtual logical check_and_fix_middle_uv(const point_data &start,
										  const point_data &end,
										  point_data &middle,
										  logical only_for_singular_surf);

	// Make point_data a friend, so that it may use private member
	// functions. In fact all it actually needs is eval_seg() for
	// one of its constructors.

	friend class point_data;
};

// Subsidiary data for each object-space curve being fitted.
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_SPLINE interp_obj_data : public ACIS_OBJECT {
public:
	// The input data for this curve - an array of points and one of
	// directions.

	SPAposition const *obj_pt;		// pointer to array of points to be
								// interpolated.
	SPAvector const *obj_tan;		// pointer to array of derivatives at
								// the interpolation points.

private:
	// Results of the interpolation

	bs3_curve obj_bs;			// object-space curve constructed.

	double obj_fitol;			// actual fit achieved.

	double actual_fitol;		// Non fudged actual fit achieved.

	// Workspace during interpolation

	bs3_curve obj_seg;			// current segment of object-space
								// curve.

public:
	interp_obj_data();
	~interp_obj_data();

	// Extract the spline from the object. Note that this can only be
	// done once for each "fit" operation, NULL being returned after
	// the first call.

	bs3_curve bs();

	// Extract the fit tolerance achieved.

	double fitol() { return obj_fitol; }

	// Extract the Non fudged fit tolerance achieved.

	double afitol() { return actual_fitol; }

	// Let the containing class get at our stuff.

	friend class curve_interp;
};

// Subsidiary data for each surface relevant to the fitting process.
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_SPLINE interp_surf_data : public ACIS_OBJECT {
public:
	// Input data. A surface, plus SPAposition and direction arrays
	// in its SPAparameter space, or NULL if it is not parametric.

	surface const *sur;			// surface of interest. NULL if the
								// SPAparameter values relate to something
								// other than a surface.

	SPApar_pos const *par_pt;		// pointer to array of SPAparameter-space
								// points to interpolate, NULL if the
								// surface is not parametric.

	SPApar_vec const *par_tan;		// pointer to array of SPAparameter-space
								// directions corresponding to the
								// positions in par_pt. NULL if the
								// surface is not parametric, or if
								// the directions can be deduced from
								// the surface and the object-space
								// direction. May not be NULL if the
								// surface is NULL.

private:
	// The result of interpolation

	bs2_curve par_bs;			// SPAparameter-space curve constructed.

	double obj_fitol;			// actual fit achieved.

	double actual_fitol;		// Non fudged actual fit achieved.

	// Workspace used during interpolation

	bs2_curve par_seg;			// current segment of SPAparameter-space
								// curve.

public:
	interp_surf_data();
	~interp_surf_data();

	// Extract the spline from the object. Note that this can only be
	// done once for each "fit" operation, NULL being returned after
	// the first call.

	bs2_curve bs();

	// Extract the fit tolerance achieved.

	double fitol() { return obj_fitol; }

	// Extract the Non fudged fit tolerance achieved.

	double afitol() { return actual_fitol; }

	// Let the containing class get at our stuff.

	friend class curve_interp;
};

// Class for passing round points in object and SPAparameter spaces.
// This is used mostly internally to the curve-fitting functions of
// the curve_interp class, but is the medium by which point data are
// transferred into and out of the application-supplied true_point
// function.

class point_obj_data;
class point_surf_data;
struct ag_cnode;
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_SPLINE point_data : public ACIS_OBJECT {
public:
	double param;		// curve SPAparameter for this point, the same
						// for all curves.

	int nobj;			// number of object-space curves

	point_obj_data *objdata;
						// Pointer to array of object-space curve data

	int nsurf;			// number of surface-related objects

	point_surf_data *sfdata;
						// pointer to array of surface-related objects

	logical free;		// TRUE to indicate that this point is known to be
						// a special point (e.g. a surface tangency for an
						// intersection curve), which requires special
						// treatment by curve_interp::true_point() because it
						// does not have a well-defined curve direction.
	// ywoo 13Apr01: added a flag.
	logical refit;

	// Construct a copy of an existing data point.

	point_data(
			point_data const &
		);

	// Construct an object representing the mid-point of two given
	// points.

	point_data(
			point_data const &,
			point_data const &,
			curve_interp const &
		);

	// Construct an initial data point.

	point_data(
			curve_interp const &,
			int
		);

	// Destroy a point and subsidiary data.

	~point_data();

	friend class curve_interp;
};


// Class for object-space curve data for a point object.
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_SPLINE point_obj_data : public ACIS_OBJECT {
public:
	// Initial values supplied to true_point, and refined by it.

	SPAposition obj_pt;	// object-space point

	SPAvector obj_tan;		// object-space tangent direction

	// Data set by true_point.

	// This is a misnomer, from the time when this algorithm was only
	// used for surface-surface intersections. The length of the
	// cross product of the surface normals was a useful measure of
	// possible high curvature of the curve, and so was used to tighten
	// up tolerances locally. Now it should be used by true_point for
	// this latter purpose, however derived. It is only used if the more
	// direct "curve_tol" is not set. There is an overloaded meaning as
	// well. If this value is set negative, it indicates that the SPAvector
	// obj_tan is a true curve derivative, which should be matched.
	// Otherwise the direction only of obj_tan is used, the magnitude
	// of the derivative being calculated by the fitting algorithm.

	double obj_tan_len;

	// Local tolerance conditions returned by true_point. They are
	// set negative on entry to true_point, and ignored if still
	// negative on exit.

	double point_tol;	// the actual error between the evaluated true
						// point and the exact one. By default the requested
						// tolerance is used.
	double curve_tol;	// the local fit tolerance required to suit the
						// defining surfaces, etc. By default obj_tan_len
						// is multiplied by the requested fit tolerance.

private:
	// The following data members and constructors are used by the
	// curve fitting functions of curve_interp.

	// STI dgh *** TEMPORARY KLUDE; REMOVE FOR 2.0 RELEASE ***
	//
	// define both Applied Geometry and Intergraph structures for
	// fitting a curve; only one will be valid depending on which
	// spline implementation library is linked in!

	ag_cnode *seg_node;	// start of the Bezier segment of the object-
						// space curve segment which contains this
						// point

    int pl_indx;        // start of the Bezier segment of the object-
                        // space curve segment which contains this
                        // point

	logical seg_ok;		// TRUE if the current curve segment is deemed
						// to be accurate enough.

public:
    point_obj_data();
    ~point_obj_data();	// Needed to avoid bug in some compilers

	friend class point_data;
	friend class curve_interp;
};

// Surface-related data for a point object.
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_SPLINE point_surf_data : public ACIS_OBJECT {
public:
	// Initial values supplied to true_point, and refined by it.

	SPApar_pos par_pt;		// SPAparameter-space point

	SPApar_vec par_tan;	// SPAparameter-space direction and rate

private:
	// The following data members are used by the
	// curve fitting functions of curve_interp.

	// STI dgh *** TEMPORARY KLUDE; REMOVE FOR 2.0 RELEASE ***
	//
	// define both Applied Geometry and Intergraph structures for
	// fitting a curve; only one will be valid depending on which
	// spline implementation library is linked in!

	ag_cnode *seg_node;	// start of the Bezier segment of the
						// SPAparameter-space curve segment which
						// contains this point

    int pl_indx;        // start of the Bezier segment of the object-
                        // space curve segment which contains this
                        // point

	logical seg_ok;		// TRUE if the current segment is deemed to be
						// accurate enough, or if the corresponding
						// surface is not parametric.

public:
    point_surf_data();
    ~point_surf_data();	// Needed to avoid bug in some compilers

	friend class point_data;
	friend class curve_interp;
};

/*! @} */
#endif

