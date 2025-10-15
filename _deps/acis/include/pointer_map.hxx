// $Id: pointer_map.hxx,v 1.12 2000/12/26 18:48:27 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ACIS_POINTER_MAP_HXX
#define ACIS_POINTER_MAP_HXX

/********************************************************************************/

#include "base_pointer_map.hxx"
#include "acis.hxx"
#include "dcl_kern.h"

/**
* @file pointer_map.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup KERNAPI
 *
 * @{
 */


class surface;
class curve;
class pcurve;
class law;
class law_data;
class spl_sur;
class int_cur;
class par_cur;
#ifndef NO_MESH_CLASSES
class msh_sur;
class com_cur;
#endif

/**
 * This class tracks reference counting of sub-objects used in making a deep copy.
 * <br>
 * <b>Role:</b> The pointer_map class is used in conjunction with deep_copy. A regular 
 * copy attempts to optimize time and space by reference counting an object, or pieces 
 * of the object, instead of copying the whole thing. The result of a deep copy does 
 * not share any data with the original object. However, if the original object had 
 * reference counted sub-objects, it is desirable that the deep copied object also 
 * share its corresponding sub-objects. The  pointer_map class is used to keep track 
 * of this kind of information. Thus, if the original object were a car with four 
 * wheels, all of which referenced one wheel object, the deep copied object would also 
 * contain four wheels which reference only one wheel object.
 * @see api_deep_copy_entity
 */
class DECL_KERN pointer_map: public base_pointer_map
{
public:
	/**
	 * Default constructor.
	 */
	pointer_map();

	/**
	 * Copies a surface, returning an unreferenced pointer.
	 * @param s
	 * original surface.
	 */
	surface *	get_surface(const surface * s);

	/**
	 * Copies a curve, returning an unreferenced pointer. 
	 * @param c
	 * original curve.
	 */
	curve *		get_curve(const curve * c);

	/**
	 * Copies a pcurve, returning an unreferenced pointer.
	 * @param pc
	 * original pcurve.
	 */
	pcurve *	get_pcurve(const pcurve * pc);

	/**
	 * Copies a spl_sur, returning an unreferenced pointer.
	 * @param s
	 * original spl_sur.
	 */
	spl_sur *	get_spl_sur(const spl_sur * s);

	/**
	 * Copies an int_cur, returning an unreferenced pointer.
	 * @param ic
	 * original int_cur.
	 */
	int_cur *	get_int_cur(const int_cur * ic);

	/**
	 * Copies a par_cur, returning an unreferenced pointer.
	 * @param pc 
	 * original par_cur.
	 */
	par_cur *	get_par_cur(const par_cur * pc);

#ifndef NO_MESH_CLASSES
	/**
	 * Copies a msh_sur, returning an unreferenced pointer.
	 * @param ms
	 * original msh_sur.
	 */
	msh_sur *	get_msh_sur(const msh_sur * ms);

	/**
	 * Copies a com_cur, returning an unreferenced pointer.
	 * @param cc
	 * original com_cur.
	 */
	com_cur *	get_com_cur(const com_cur * cc);
#endif

	/**
	 * Default destructor.
	 */
	virtual ~pointer_map();

protected:
};
/********************************************************************************/

/*! @} */
#endif

