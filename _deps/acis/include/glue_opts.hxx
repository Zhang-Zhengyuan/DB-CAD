/* $Id: glue_opts.hxx,v 1.6 2002/04/23 11:19:56 atan Exp $ */

/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( GLUE_OPTIONS_CLASS )
#define GLUE_OPTIONS_CLASS

/**
 * \defgroup BOOLGLUEOPTAPI Glue Options
 * \ingroup BOOLOPTIONSAPI
 *
 */

/**
* @file glue_opts.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

#include "acis.hxx"
#include "dcl_bool.h"
#include "acis_options.hxx"


/*! \addtogroup BOOLGLUEOPTAPI
 *
 *  @{
 */

class ENTITY;
/**
 * Class to hold information and options for a glue operation.
 * <br>
 * <b>Role:</b> A @href glue_options object is to be used in conjunction with two bodies
 * (blank and tool) whose intersection is known to lie along a set of coincident faces.
 * See documentation for @href api_boolean_glue for the definition of coincident faces.
 * <br><br>
 * This class contains two arrays of pointers to pairwise coincident faces of the blank
 * and tool respectively.  Therefore, the length of these arrays must be the same and
 * must be equal to <tt>num_coi_faces</tt>.  Use the method <tt>set_data</tt> to set this data.
 * <br><br>
 * This class also contains various flags which are all unset (-1) by default:<br>
 * <ul>
 * <li><tt>patch_and_face_cover</tt> </li>
 * <li><tt>blank_patches_strict_cover</tt> </li>
 * <li><tt>single_face_patch</tt> </li>
 * <li><tt>non_trivial</tt> </li>
 * </ul>
 * Certain combinations can
 * be set to improve performance.  It is important that the information provided is accurate,
 * as the glue operation will rely heavily on this information.
 * <br><br>
 * Given bodies b1 and b2, a <b><i>coincident patch</i></b> P1 in b1 is a maximal set 
 * of connected faces of b1 such that there exists a corresponding maximal set P2 of 
 * connected faces in b2 and a (well-defined onto) coincidence mapping from P1 to P2.
 * <br><br>
 * Face f1 <b><i>covers</i></b> face f2 if the point set of f2 is a subset of the point 
 * set of f1. Patch P1 <b><i>covers</i></b> patch P2 if the point set of P2 is a subset 
 * of the point set of P1. Given a pair of coincident patches P1 and P2, P1 is a 
 * <b><i>strict cover</i></b> of P2 if the point set of P2 is a subset of the interior 
 * point set of P1.
 * <br><br>
 * Setting <tt>patch_and_face_cover</tt> to <tt>TRUE</tt> will induce a performance enhancement. 
 * <tt>patch_and_face_cover</tt>
 * may be set to <tt>TRUE</tt> if the following conditions are met:
 * <ul>
 * <li>for every pair of coincident faces (to be specified in the glue operation), one face covers
 * the other face;</li>
 * <li>for every pair of coincident patches, one patch covers the other patch.</li>
 * </ul>
 * In addition to setting <tt>patch_and_face_cover</tt> to <tt>TRUE</tt>, setting <tt>blank_patches_strict_cover</tt> to
 * <tt>TRUE</tt> will induce another performance enhancement. <tt>blank_patches_strict_cover</tt> may be set to
 * <tt>TRUE</tt> if the following conditions are met:
 * <ul>
 * <li><tt>patch_and_face_cover</tt> is set to <tt>TRUE</tt>;</li>
 * <li>every patch in the blank (first) body is a strict cover of its corresponding patch in the
 * tool (second) body.</li>
 * </ul>
 * In addition to setting <tt>patch_and_face_cover</tt> to <tt>TRUE</tt>, setting <tt>single_face_patch</tt> to
 * <tt>TRUE</tt> will induce another performance enhancement. <tt>single_face_patch</tt> may be set to <tt>TRUE</tt> if
 * the following conditions are met:
 * <ul>
 * <li><tt>patch_and_face_cover</tt> is set to <tt>TRUE</tt>;</li>
 * <li>every patch consists of precisely <i>one</i> face;</li>
 * <li>for each patch, all edges are coincident and all vertices are coincident.  In other words,
 * the corresponding pair of coincident faces are geometrically and topologically identical.</li>
 * </ul>
 * Thus, <tt>blank_patches_strict_cover</tt> and <tt>single_face_patch</tt> cannot both be set to <tt>TRUE</tt>.
 * <br><br>
 * <tt>non_trivial</tt> may be set to <tt>TRUE</tt> if both bodies are solid bodies and it is guaranteed that the boolean operation will be
 * non-trivial. In the case of glue-unite, this is when the tool body lies outside the blank body.
 * In the case of glue-subtract, this is when the tool body is completely contained in the blank body.  This will induce another performance enhancement.  It is not dependent on the previous flags.  It is not applicable to sheet bodies.<br>
 * @see api_boolean_glue
 */

class DECL_BOOL glue_options : public AcisOptions
{
public:
	/**
	* C++ constructor requests memory for this object and populates it with the data supplied as arguments.
	* <br><br>
	* @param num_faces
	* number of coincident faces.
	* @param tool_faces
	* tool faces.
	* @param blank_faces
	* blank faces.
	*/
	glue_options(int num_faces = 0, ENTITY** tool_faces = NULL, ENTITY** blank_faces = NULL);
	/**
	* Destructor.
	*/
	virtual ~glue_options(){};
	/**
	* Returns <tt>TRUE</tt> if the input object is same as this object or <tt>FALSE</tt> otherwise.
	* <br><br>
	* @param in_glue_opt
	* object to compare.
	*/
	logical operator== (glue_options const& in_glue_opt) const;
	/**
	* Returns <tt>TRUE</tt> if the input object is not same as this object or <tt>FALSE</tt> otherwise.
	* <br><br>
	* @param in_glue_opt
	* object to compare.
	*/
	logical operator!= (glue_options const& in_glue_opt) const;
	/**
	* Returns a copy of this object.
	*/
	glue_options *copy() const;
	/**
	* Copies the flag values from the input object to this object.
	* <br><br>
	* @param in_obj
	* input object.
	*/
	void copy_flags(const glue_options* in_obj);
	// Set functions
	/**
	* Copies the input values to this object.
	* <br><br>
	* @param num_faces
	* number of coincident faces.
	* @param tool_faces
	* tool faces.
	* @param blank_faces
	* blank faces.
	*/
	void set_data(int num_faces, ENTITY** tool_faces, ENTITY** blank_faces);
	/**
	* Copies the flag values to this object.
	* <br><br>
	* @param face_pair_cov
	* face_pair_cover.
	* @param blnk_pat
	* blank patches strict cover.
	* @param non_triv
	* non trivial.
	* @param edge_iden
	* edges identical. 
	*/
	void set_flags(int face_pair_cov, int blnk_pat, int non_triv, int edge_iden = -1);
	/**
	* Copies the input value to <tt>patch_and_face_cover</tt> flag.
	* <br><br>
	* @param val
	* value to set.
	*/
	void set_patch_and_face_cover(int val);
	/**
	* Copies input value to <tt>blank_patches_strict_cover</tt> flag.
	* <br><br>
	* @param val
	* value to set.
	*/
	void set_blank_patches_strict_cover(int val);
	/**
	* Copies the input value to <tt>non_trivial</tt> flag.
	* <br><br>
	* @param val
	* value to set.
	*/
	void set_non_trivial(int val);
	/**
	* Copies input value to <tt>single_face_patch</tt> flag.
	* <br><br>
	* @param val
	* value to set.
	*/
	void set_single_face_patch(int val);

	// Get functions
	/**
	* Returns the number of coincident faces.
	*/
	int get_num_coi_faces() const;
	/**
	* Returns the tool faces.
	*/
	ENTITY **get_tfaces() const;
	/**
	* Returns the blank faces.
	*/
	ENTITY **get_bfaces() const;
	/**
	* Returns the current value of <tt>patch_and_face_cover</tt> flag.
	*/
	int get_patch_and_face_cover() const;
	/**
	* Returns the current value of <tt>blank_patches_strict_cover</tt> flag.
	*/
	int get_blank_patches_strict_cover() const;
	/**
	* Returns the current value of <tt>non_trivial</tt> flag.
	*/
	int get_non_trivial() const;
	/**
	* Returns the current value of <tt>single_face_patch</tt> flag.
	*/
	int get_single_face_patch() const;

	// Sanity check
	/**
	* Performs a sanity check on the object.
	*/
	logical check() const;

	/*
	 *@nodoc
	 */
	// For internal use only.

	enum coin_detection_mode
	{
		DETECT_NONE,
		DETECT_ALL,
		DETECT_REMAINING
	};

	/*
	 *@nodoc
	 */
	// For internal use only.

	void set_coin_detect_action( coin_detection_mode cdmode, double = -1.0 /* for future expansion*/ );

	/*
	 *@nodoc
	 */
	// For internal use only.

	double near_coincidence_fuzz() const;

private:

	int num_coi_faces;
	ENTITY **tfaces;
	ENTITY **bfaces;

	// -1 unset 0 false 1 true

	int patch_and_face_cover;
	int blank_patches_strict_cover;
	int non_trivial;
	int single_face_patch;

	double _coi_fuzz;
};

/*! @} */

#endif
