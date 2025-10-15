/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Curve-curve intersection 

#ifndef INT_CURVE_CURVE
#define INT_CURVE_CURVE

#include "dcl_kern.h"
#include "debugmsc.hxx"
#include "param.hxx"
#include "position.hxx"

class curve;
class SPAbox;

/**
 * @file intcucu.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup INTRMAIN
 *
 * @{
 */
/**
 * Classify a curve-curve intersection.
 * <br>
 * <b>Role:</b> We do not have a sense of <i>inside</i> or <i>outside</i>, but can recognise
 * tangencies and regions of coincidence.
 * @param cur_cur_unknown
 * No comment on relationship.
 * @param cur_cur_normal
 * Normal angular intersection.
 * @param 	cur_cur_tangent
 * Curves are tangent (or antitangent), but not coincident.
 * @param 	cur_cur_coin
 * Curves are coincident over an extended region.
 */
enum curve_curve_rel
{
	cur_cur_unknown,
	cur_cur_normal,
	cur_cur_tangent,
	cur_cur_coin
};

enum gme_curve_curve_rel
{
	gme_cur_cur_unknown,
	gme_cur_cur_normal,
	gme_cur_cur_tangent,
	gme_cur_cur_coin
};

/**
 * Provides a hook for a client intersector to attach private information to an intersection.
 * <br>
 * <b>Role:</b> This base class is purely to provide a virtual
 * destructor to ensure that anything derived by the client gets destroyed
 * properly when the intersection object is destroyed.
 */
class DECL_KERN curve_curve_userdata {
public:
	virtual ~curve_curve_userdata() {}
};

#ifdef INTERNAL_DEBUG_CHECKS
#pragma warning ( push )
#pragma warning( disable: 4251 )
#endif

/**
 * Represents the intersection of a curve with another curve and returns the intersections as a list.
 * <br>
 * <b>Role:</b> This class represents the intersection of a curve (<tt>curve1</tt>) with
 * another curve (<tt>curve2</tt>). The intersections are returned as a list. The list can
 * be walked using the <tt>next</tt> member of the class. This class saves the
 * intersection point as <tt>int_point</tt>, and saves its corresponding parameter on
 * <tt>curve1</tt> as <tt>param1</tt>, and on <tt>curve2</tt> as <tt>param2</tt>.
 * The relation between <tt>curve1</tt> and <tt>curve2</tt> at this intersection
 * point are saved as <tt>low_rel</tt>, which is the relation on the lower parameter
 * side of the intersection and <tt>high_rel</tt>, which is higher parameter side
 * of the intersection with respect to <tt>curve1</tt>.
 * @see SPApar_pos, SPAposition
 */
class DECL_KERN curve_curve_int : public ACIS_OBJECT {
public:
#ifdef INTERNAL_DEBUG_CHECKS
	int _cci_index;
	#ifdef THREAD_SAFE_ACIS
		static safe_integral_type<int> _cci_cnt;
	#else
		static int _cci_cnt;
	#endif
#endif //INTERNAL_DEBUG_CHECKS
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * @param next
 * "next" list pointer.
 * @param pt
 * intersection point.
 * @param cur1
 * first curve parameter.
 * @param cur2
 * second curve parameter.
 * @param actual
 * actual param.
 */
	curve_curve_int(
				curve_curve_int*   next,
				SPAposition const& pt,
				double             cur1,
				double             cur2,
				SPApar_pos&        actual = *(class  SPApar_pos * ) NULL_REF
			);

	curve_curve_int(
				const char* gme,
				curve_curve_int*   next,
				SPAposition const& pt,
				double             cur1,
				double             cur2,
				SPApar_pos&        actual = *(class  SPApar_pos * ) NULL_REF
			);
/**
 * Copy constructor requests memory for this object and populates it 
 * with the data from the object supplied as an argument. Note, that userdata 
 * of new object is set to NULL and not assinged to other's userdata 
 * (in order to prevent accessing of deleted userdata).
 * <br><br>
 * @param other
 * curve-surf intersection.
 */
	curve_curve_int(curve_curve_int const& other);
	curve_curve_int(const char* gme, curve_curve_int const& other);
/**
 * Copy assignment operator. Clears the current data and replaces it with other's data. 
 * The copying behavior is same as that of the copy constructor (consider especially userdata).
 * <br><br>
 * @param other
 * curve-surf intersection.
 */
	curve_curve_int& operator=(curve_curve_int const& other);
	curve_curve_int& gme_operator_assign();
/**
 * Destructor, deleting a <tt>curve_curve_int</tt>, it explicitely calls the destructor of userdata.
 */
	~curve_curve_int();
/**
 * Equality operator
 */
    bool operator==( curve_curve_int const& rhs ) const;
	// 有容差的等于判断
	bool gme_operator_equal(curve_curve_int const& rhs) const;
/**
 * Writes debug information about <tt>curve_curve_int</tt> to standard output or to the specified file.
 * <br><br>
 * @param fp
 * file name.
 */
	void debug( FILE *fp = debug_file_ptr );
	void gme_debug( FILE *fp = debug_file_ptr );
/**
 * Relation @href curve_curve_rel of curves on the higher-parameter side of <tt>curve1</tt>.
 */
	curve_curve_rel high_rel;
/**
 * Intersection point.
 */
	SPAposition int_point;
/**
 * Relation @href curve_curve_rel of curves on the lower-parameter side of <tt>curve1</tt>.
 */
	curve_curve_rel low_rel;
/**
 * Pointer to allow linked lists of <tt>curve_curve_ints</tt>.
 */
	curve_curve_int *next;
/**
 * Intersection parameter on <tt>curve1</tt>.
 */
	double param1;
/**
 * Intersection parameter on <tt>curve2</tt>.
 */
	double param2;
/**
 * Pointer to an arbitrary object to store user data.
 * <br><br>
 * <b>Role:</b> If non-<tt>NULL</tt>, it is deleted when this object is deleted. It is the
 * responsibility of the user's class derived from this to ensure that the
 * destructor does what is necessary.
 */
	curve_curve_userdata *userdata;
/**
 * Surface parameters if the curves are known to lie on a surface.
 */
	SPApar_pos uv;
/**
 * <tt>TRUE</tt> if the surface parameters have been set -  <tt>FALSE</tt> by default.
 */
	logical uv_set;
};

#ifdef INTERNAL_DEBUG_CHECKS
#pragma warning ( pop )
#endif

/**
 * Deletes a linked list of <tt>curve_curve_int</tt> objects, given the head of the list.
 * <br><br>
 * <b>Role:</b> The head is always set to NULL on exit.
 * <br><br>
 * @param head
 * head.
 */
DECL_KERN void delete_curve_curve_ints( curve_curve_int*& head );
DECL_KERN void gme_delete_curve_curve_ints( curve_curve_int*& head );

/** @} */
#endif
