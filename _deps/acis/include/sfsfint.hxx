/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//             surface-surface intersection curve
/*******************************************************************/
#ifndef SFSFINT_HXX
#define SFSFINT_HXX
#include "logical.h"
#include "acis.hxx"
#include "dcl_kern.h"
#include "debugmsc.hxx"
#include "position.hxx"
#include "param.hxx"
class curve;
class pcurve;
class surface;
class surf_surf_term;
/*******************************************************************/
/**
 * @file sfsfint.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup INTRMAIN
 *
 * @{
 */
/**
 * Types of surface intersection.
 * <br><br>
 * <b>Role:</b> Describes the type of surface intersection in a 
 * curve produced by two intersecting surfaces.
 * @param	int_normal
 * Normal (usual) intersection.
 * @param int_tangent
 * Tangent or coincident intersection, with normals in the same direction.
 * @param int_antitangent
 * Tangent or coincident intersection, with normals in opposite direction.
 */
enum surf_int_type 
{
	int_normal,
	int_tangent,
	int_antitangent
};
/**
 * Types of surface-surface relation.
 * <br><br>
 * <b>Role:</b> Describes the type of relationship between two surfaces
 * used to produce a curve from two intersecting surfaces.
 * @param surf_unknown
 * Unknow relationship.
 * @param 	surf_inside
 * Surface is inside.
 * @param surf_outside
 * Surface is outside
 * @param surf_coincident
 * Coincidente surfaces.
 * @param surf_keep
 * Surface is kept.
 * @param surf_discard
 * Surface is discarded.
 */
enum surf_surf_rel 
{
	surf_unknown,
	surf_inside,
	surf_outside,
	surf_coincident,
	surf_keep,
	surf_discard
};

#ifdef INTERNAL_DEBUG_CHECKS
#pragma warning ( push )
#pragma warning( disable: 4251 )
#endif

/**
 * Represents the intersection of two face surfaces and returns zero or more curves.
 * <br><br>
 * <b>Role</b>: This class holds the details of the intersection of two FACE surfaces
 * and returns zero or more curves. Every EDGE of each FACE is assumed to have been
 * intersected with the other surface, so the intersection points may be used to
 * assist (for example, if the surfaces are parametric).
 * @see intcurve, curve, pcurve, surface
 */
class DECL_KERN surf_surf_int : public ACIS_OBJECT 
{
private:
	double _tol;

public:
#ifdef INTERNAL_DEBUG_CHECKS
	int _ssi_index;
	#ifdef THREAD_SAFE_ACIS
		static safe_integral_type<int> _ssi_cnt;
	#else
		static int _ssi_cnt;
	#endif
#endif //INTERNAL_DEBUG_CHECKS
/**
 * C++ initialize constructor requests memory for this object and populates 
 * it with the data supplied as arguments.
 * <br><br>
 * <b>Role</b>: The default type is normal, and the relationships are suitable
 * for the conventional curve direction being the cross products of the surface
 * normals in the given order.
 * @param cur
 * curve
 * @param inter
 * surf-surf intersection
 * @param term1
 * surf-surf termination
 * @param term2
 * surf-surf termination
 */
	surf_surf_int(
				curve * cur,
				surf_surf_int * inter = NULL,
				surf_surf_term * term1 = NULL,
				surf_surf_term * term2 = NULL
			);
	surf_surf_int(
				surf_surf_int * inter,
				curve * cur
			);
	void init(
				surf_surf_term *start,
				surf_surf_term *end
			);
	void gme_init(
				surf_surf_term *start,
				surf_surf_term *end
			);
/**
 * C++ initialize constructor requests memory for this object and populates it
 * with the data supplied as arguments.
 * <br>
 * <b>Role</b>: Constructor for an intersection curve representing an isolated point.
 * The default type is normal and the relationships are set to unknown.
 * @param pos
 * position
 * @param nextint
 * nest surf-surf intersection
 */
	surf_surf_int(
				SPAposition const &pos,
				surf_surf_int *nextint = NULL
			);
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param intsec
 * surf-surf intersection
 */
 	surf_surf_int( surf_surf_int const &intsec );
/**
 * Copies a surf_surf_int object. The behavior is same as that of the copy constructor.
 * <br><br>
 * @param intsec
 * surf-surf intersection
 */
	surf_surf_int& operator= ( surf_surf_int const &intsec );
	void gme_operator_assign(surf_surf_int const& intsec);
/**
 * Compares 2 surf_surf_int objects and returns a boolean.
 * <br><br>
 * @param ssi
 * surf-surf intersection
 */
	bool operator==( const surf_surf_int *ssi );
	bool gme_operator_equal(const surf_surf_int* ssi);
/**
 * C++ destructor, deleting a surf_surf_int.
 */
	~surf_surf_int();
/**
 * For each of the surfaces, it specifies the relationship @href surf_surf_rel
 * on the left side of the intersection curve to the auxiliary surface.
 * <br><br>
 * <b>Role</b>: Because this is always a clean "inside" or "outside," the right
 * relationship is always the converse, so it does not need to be recorded.
 */
	surf_surf_rel aux_left_rel[2];
/**
 * Normally <tt>NULL</tt>, this points to a surface containing the intersection curve.
 * <br><br>
 * <b>Role</b>: It is roughly perpendicular to the subject surfaces if they are
 * tangential or near tangential, which allows for clean intersections with the
 * edges of the faces.
 */
	surface *aux_surf;
/**
 * Intersecting curve from the face-face coincidence, and it may be <tt>NULL</tt>.
 * <br><br>
 * <b>Role</b>: In this case, all face-body relationships are either surf_symmetric
 * or surf_antisymmetric, and this is the only CURVE_LIST record in the list.
 */
	curve *cur;
/**
 * Writes debug information about surf_surf_int to the printer or to the specified file.
 * @param fp
 * file name
 */
	void debug( FILE *fp = debug_file_ptr ) const;
	void gme_debug( FILE *fp = debug_file_ptr ) const;
/**
 * The parameter value of end_point, which is meaningless if the end_point is <tt>NULL</tt>.
 */
	double end_param;
/**
 * The terminator point at the end of the curve.
 * <br><br>
 * <b>Role</b>: It is <tt>NULL</tt> if the curve is not bounded at the end.
 */
	surf_surf_term *end_term;
/**
 * The classification of the intersection type @href surf_int_type .
 */
	surf_int_type int_type;

#ifndef NO_MESH_CLASSES
/**
 * Intersection type @href surf_int_type with respect to other face
 * of the portions of each face to the left of the intersection curve.
 * <br><br>
 * <b>Role</b>: Only used for mesh surface intersections (otherwise the single
 * int_type above is sufficient). Set to int_unknown if not used.
 */
	surf_int_type left_int_type[2];
#endif

/**
 * The relationships @href surf_surf_rel with respect to the other face of the
 * portions of each face to the left of the intersection curve. 
 */
	surf_surf_rel left_surf_rel[2];
/**
 * Next surface-surface intersection.
 */
	surf_surf_int *next;
/**
 * The number of values in the array <tt>split_param</tt>.
 * <br><br>
 * <b>Role</b>: This is 0 if <tt>split_param</tt> is <tt>NULL</tt>.
 */
	int nsplit;


#ifdef INTERNAL_DEBUG_CHECKS
private:
#endif
/**
 * The first pcurve, it provides the parametric-space intersection curve with
 * respect to the intersection surfaces, if they are parametric.
 * <br><br>
 * <b>Role</b>: It may be <tt>NULL</tt> even if <tt>cur</tt> is not <tt>NULL</tt>.
 */
	pcurve *pcur1;

/**
 * The second pcurve, it provides the parametric-space intersection curve with 
 * respect to the intersection surfaces, if they are parametric.
 * <br><br>
 * <b>Role</b>: It may be <tt>NULL</tt> even if <tt>cur</tt> is not <tt>NULL</tt>.
 */
	pcurve *pcur2;

public:
	pcurve * get_pcur(int index, bool valid_cur_pcur = false) const;

	void set_pcur( pcurve * , int );

#ifndef NO_MESH_CLASSES
/**
 * Intersection type @href surf_int_type with respect to other face
 * of the portions of each face to the left of the intersection curve.
 * <br><br>
 * <b>Role</b>: Only used for mesh surface intersections (otherwise the single
 * int_type above is sufficient). Set to int_unknown if not used.
 */
	surf_int_type right_int_type[2];
#endif

/**
 * The relationships @href surf_surf_rel with respect to the other face of the
 * portions of each face to the left of the intersection curve. 
 */
	surf_surf_rel right_surf_rel[2];
/**
 * The terminator point at the start of the curve.
 * <br><br>
 * <b>Role</b>: It is <tt>NULL</tt> if the curve is not bounded at the start.
 * An array of parameter values flagging bounded regions of the curve where it
 * lies outside the region of interest. Each value is a typical parameter value
 * within the portion outside the SPAbox. If no SPAbox was specified, or the
 * intersection curve lies wholly within the SPAbox, or it is unbounded but only
 * enters the SPAbox in one SPAinterval, then this pointer is <tt>NULL</tt>. Otherwise it must
 * point to an array on the heap.
 */
	double *split_param;
/**
 * The parameter value of start_point, which is meaningless if the start_point is <tt>NULL</tt>.
 */
	double start_param;
/**
 * The terminator point at the start of the curve.
 * <br><br>
 * <b>Role</b>: It is <tt>NULL</tt> if the curve is not bounded at the start.
*/
	surf_surf_term *start_term;

/**
 * @nodoc
 */
	double tolerance() const { return _tol; }

/**
 * @nodoc
 */
	void set_tolerance(double new_t);
	void gme_set_tolerance(double new_t);

/**
 * Ignore pcurves stored in curve
 */
	bool ignore_cur_pcurve;
};

#ifdef INTERNAL_DEBUG_CHECKS
#pragma warning ( pop )
#endif
/**
 * @nodoc
 * Terminator point. Just one exists for each special (e.g. surface
 * tangency) point, and is pointed to by every surf_surf_int record
 * for a curve which start or ends at the point. Therefore we have a
 * use count to handle the multiple references.
 * First the base class for terminator information. Derived classes
 * will be used by clients to store useful information. The main
 * purpose is to ensure that an appropriate destructor gets called.
 */
class DECL_KERN surf_surf_term_data : public ACIS_OBJECT {
public:
	/**
	 * Default constructor
	 */
	surf_surf_term_data() {}
	/**
	 * C++ destructor
	 */
	virtual ~surf_surf_term_data();
};
/**
 * @nodoc
 * Describes the surface surface intersection terminator relationships.
 * <br><br>
 * <b>Role:</b>  Enumeration to classify a terminator relative to the 
 * SPAbox used in the surface-surface intersection. In particular it 
 * indicates when a curve has left the SPAbox, and so does not need 
 * to be extended.
 * @param	surf_surf_term_unknown_box
 * Unknown relationship
 * @param	surf_surf_term_inside_box
 * Inside box
 * @param	surf_surf_term_outside_box
 * Ouside box
 */
enum surf_surf_term_cont 
{
	surf_surf_term_unknown_box,
	surf_surf_term_inside_box,
	surf_surf_term_outside_box
};
/**
 * @nodoc
 * Terminator description class
 */
class DECL_KERN surf_surf_term : public ACIS_OBJECT 
{
private:

	/**
	 * Number of current references to this record.
	 */
	int use_count;

	double _tol;

public:
	/**
	* Location in object space of the terminator point.
	*/
	SPAposition term_pos;
	/**
	* Location in the parameter space of the first surface.
	*/
	SPApar_pos term_uv1;
	/**
	* Location in the parameter space of the second surface.
	*/
	SPApar_pos term_uv2;
	/**
	* Set to <tt>TRUE</tt> if the term_uv? values have been set correctly. This is 
	* a temporary measure until all terminators get these values recorded
	* at source.
	*/
	logical params_set;
	/**
	* Specify whether the terminator is inside or outside the region 
	* of interest, or unknown.
	*/
	surf_surf_term_cont cont;
	/**
	* Pointer for client's use, for additional data associated with the terminator.
	* Always set <tt>NULL</tt> by the surface-surface intersector.
	*/
	surf_surf_term_data *term_data;
	/**
	 * C++ Constructor
	 * @param pos
	 */
	surf_surf_term( SPAposition const &pos );
	surf_surf_term(const char *gme, SPAposition const &pos);
	/**
	 * C++ Constructor
	 * @param pos
	 * Location in object space of the terminator point.
	 * @param parp1
	 * Location in the parameter space of the first surface.
	 * @param parp2
	 * Location in the parameter space of the second surface.
	 */
	surf_surf_term( SPAposition const & pos, 
		            SPApar_pos const & parp1, 
					SPApar_pos const & parp2);
	surf_surf_term( const char *gme,
		            SPAposition const & pos, 
		            SPApar_pos const & parp1, 
					SPApar_pos const & parp2);
	/**
	 * C++ Copy constructor
	 */
    surf_surf_term( const surf_surf_term & sst );
	surf_surf_term(const char *gme, const surf_surf_term & sst);
	/**
	 * Default destructor.
	 */
	~surf_surf_term() 
	{
		if (term_data != NULL)
			ACIS_DELETE term_data;
	}
	/**
	 * Increase use count.
	 */
	void add() { ++use_count; }
	/**
     * Decrease use count and self deletion.
	 */
	void remove() {
		if (--use_count <= 0)
			ACIS_DELETE this;
	}
	/**
	 * Debug this object to a file
	 */
	void debug( FILE * = debug_file_ptr ) const;

/**
 * @nodoc
 */
	double tolerance() const { return _tol; }

/**
 * @nodoc
 */
	void raise_tolerance(double new_tol);
};

/**
 * @nodoc
 * Delete a linked list of surf_surf_ints, given the head of the list.
 * The head is always set to zero on exit.
 * @param head 
 * The initial surf_surf_int.
 */
DECL_KERN	void	delete_surf_surf_ints( surf_surf_int*& head );

/**
 * @nodoc
 * Shallow copies a linked list of surf_surf_ints, given the original list.
 * @param orig_list (input), copy_list (output)
*/

DECL_KERN	void	copy_surf_surf_int_list( surf_surf_int* const orig_list, surf_surf_int*& copy_list);

/** @} */
#endif
