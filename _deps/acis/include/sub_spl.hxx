/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//                    subset spline surface
/*******************************************************************/
#if !defined( sub_spl_sur_CLASS )
#define sub_spl_sur_CLASS
#include <stdio.h>
#include "dcl_kern.h"
#include "interval.hxx"
#include "spldef.hxx"
#include "pipe_spl.hxx"
#include "position.hxx"
#include "unitvec.hxx"
class SizeAccumulator;
/*******************************************************************/
/**
* @file sub_spl.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISSURFACES
 *
 * @{
 */

/**
 * @nodoc
 * Internal use. Restore mechanism.
 */
DECL_KERN subtype_object *restore_sub_spl_sur();
#if defined D3_STANDALONE || defined D3_DEBUG
/**
 * @nodoc
 */

DECL_KERN D3_istream& operator>>( D3_istream &, spl_sur *& );
/**
 * @nodoc
 */
DECL_KERN D3_ostream& operator<<( D3_ostream &, spl_sur const & );
#endif
/**
 * Subsetted spline surface.<br>
 * <b>Role:</b> Class to represent the geometry of a spline surface
 * which is a subsetted region of another <tt>spl_sur</tt>.  The <tt>subset</tt>
 * <i>uv</i> range may be smaller or larger than the range of the progenitor surface,
 * and may overlap it in any way. Subset surfaces should be created by using the
 * <tt>spline::subset()</tt> method which will correctly extend andtrim a
 * <tt>sub_spl_sur</tt> as its surface definition.  The <tt>spline::subset()</tt>
 * method is smart about handling periodic surfaces when the desired
 * subset crosses a periodic seam.
 */
class DECL_KERN sub_spl_sur: public spl_sur
{
public:
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Creates a new spline using a progenitor surface and a given
 * parameter range. The parameter range, defaults to full range
 * of progenitor surface.
 * <br><br>
 * @param spl
 * Progenitor surface.
 * @param range
 * Parameter range that the new surface will be limited to.
 */

	sub_spl_sur(
				spline const     &spl,
				SPApar_box const &range = *(const class SPApar_box *)NULL_REF
			);
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param sur
 * sub_spl_sur.
 */
	sub_spl_sur( const sub_spl_sur& old );
/**
 * Creates a copy of an item that does not share any data with the original.
 * <br><br>
 * <b>Role:</b> Allocates new storage for all member data and any pointers.
 * Returns a pointer to the copied item.
 * <br><br>
 * @param pm
 * list of items within the entity that are already  deep copied
 */
   virtual spl_sur *deep_copy(pointer_map * pm = NULL) const;
/**
 * Returns the underlying spline surface.
 */
	const spline &def_spline() const
	    { return def_spl; }
/**
 * @nodoc
 * Internal use.
 */
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
/**
 * @nodoc
 */
	virtual void minimize( minimize_helper*);
/**
 * Returns the ID for the <tt>sub_spl_sur</tt> list.
 */
	static int id();
/**
 * @nodoc
 */
	virtual void save() const;
/**
 * Saves the information for a <tt>sub_spl_sur</tt> to a save file.
 */
	virtual void save_data() const;
/**
 * @nodoc
  */
	virtual logical need_save_as_approx(int save_to_version, logical check_progenitors) const;
/**
 * Returns the integer representing the type of this <tt>spl_sur</tt>.
 */
	virtual int type() const;
/**
 * Returns the type of <tt>subsur</tt>.
 */
	virtual char const *type_name() const;

private:
	/**
	 * Defining spline.<br><br>
	 * <b>Role:</b>
	 * Record the underlying surface defining this one. In fact it is
	 * just the spl_sur which is relevant, but we wrap it up in a
	 * spline to handle sharing.
	*/
	spline def_spl;
	/**
	 * Spline approximmation.
	 */
	bs3_surface extended_sur_data;
	/**
	 * Default contructor.
	 */
	sub_spl_sur();
	/**
	 * Default destructor.
	 */
	virtual ~sub_spl_sur();
	/**
     * Function to determine if this spl_sur contains a pipe surface.
	 */

    virtual logical contains_pipe() const
	    { return def_spl.get_spl_sur().type() == pipe_spl_sur::id(); }

	/**
	 * Duplication. Can't be done by constructor, as we want it
	 * to be virtual.
	 */
	virtual subtrans_object *copy() const;

	/**
	 * Make sure that the progenitor is only referenced by "this"
	 */
	void make_single_ref();
	/**
	 * Generate an extended bs3_surface.
	 */
	bs3_surface make_sub_approx(

			SPApar_box const &
#ifdef IF_WE_EVER_DO_SPLITTING_AT_THIS_LEVEL
			,
			int
#endif
		);

#ifdef IF_WE_EVER_DO_SPLITTING_AT_THIS_LEVEL
	/**
	 * @nodoc
	 */
	bs3_surface chop(
		bs3_surface &old_surf,
		SPAinterval const &new_range,
		SPAinterval const &other_new_range,
		int axis,
		int end,
		SPAinterval const &this_old_range,
		SPAinterval const &other_old_range);
#endif

/**
 * Tes for equality.<br><br>
 * <b>Role:</b> Test for equality. This does not guarantee that all
 * effectively equal surfaces are determined to be equal, but
 * does guarantee that different surfaces are correctly
 * identified as such.
*/
	logical operator==( subtype_object const & ) const;
/**
 * Transformation.<br><br>
 * <b>Role:</b> The base class transforms the spline and fit
 * tolerance: we must handle the curve and sweep vector.
 */
	virtual void operator*=( SPAtransf const & );

/**
 * Parameter shift: adjust the spline surface to have a parameter
 * range increased by the argument value (which may be negative).
 * This is only used to move portions of a periodic surface by
 * integral multiples of the period, so "shift_v" will never be
 * used, and so we can leave it undefined here.
 */
	virtual void shift_u( double );
	virtual void shift_v( double );

/**
*  Divide a surface into two pieces at a given parameter value.
*  If the split is at the end of the parameter range, the spl_sur
*  is just returned as the appropriate half (in increasing
*  parameter order), and the other is <tt>NULL</tt>. Otherwise a new spl_sur
*  is used for one part, and the old one is modified for the other.
*/
	void split(
				double,
				spl_sur *[ 2 ],
				int
			);
	virtual void split_u(
				double,
				spl_sur *[ 2 ]
			);
	virtual void split_v(
				double,
				spl_sur *[ 2 ]
			);
/**
*  Concatenate the contents of two surfaces into one. This is only
*  called after splitting a periodic surface, to rejoin the pieces
*  the other way round.
*/
	virtual void append_u( spl_sur & );
	virtual void append_v( spl_sur & );

/**
*  Find the parameter values of a point on the surface.
*/

	virtual SPApar_pos param(
				SPAposition const &,
				SPApar_pos const & = *(SPApar_pos *)NULL_REF
			) const;
/**
 * Find the position and first and second derivatives of the
 * surface at a given point.
 */

	virtual void eval(
				SPApar_pos const &uv,
				SPAposition &pos,
				SPAvector *dpos,	// first derivatives - array of
								// length 2 in order xu, xv
				SPAvector *ddpos	// second derivatives - array of
								// length 3 in order xuu, xuv, xvv
			) const;

/**
 * @nodoc
 */
	friend DECL_KERN subtype_object *restore_sub_spl_sur();
/**
 * Restore spline from file.
 */
	void restore_data();

#if defined D3_STANDALONE || defined D3_DEBUG

	friend DECL_KERN D3_istream& operator>>(
				D3_istream &,
				spl_sur *&
			);

	friend DECL_KERN D3_ostream& operator<<(
				D3_ostream &,
				spl_sur const &
			);

	virtual void input(
				D3_istream &
			);

	virtual void print(
				D3_ostream &
			) const;

#endif

/**
 * Debug printout. As for save and restore we split the operation
 * into two parts: the virtual function "kernutil/debug" prints a class-
 * specific identifying line, then calls the ordinary function
 * "kernutil/debug_data" to put out the details. It is done this way so that
 * a derived class' debug_data can call its parent's version first,
 * to put out the common data. Indeed, if the derived class has no
 * additional data it need not define its own version of debug_data
 * and use its parent's instead. A string argument provides the
 * introduction to each displayed line after the first, and a
 * logical sets "brief" output (normally removing detailed
 * subsidiary curve and surface definitions).
 */
	virtual void debug(
				char const *,
				logical,
				FILE *
			) const;
	void debug_data(
				char const *,
				logical,
				FILE *
			) const;

	virtual void process( geometry_definition_processor& p ) const;
};


/**
* @nodoc
* <b>Obsolete:</b> This extension has been deprecated.
* <br><br>
* Creates an extension to a given spline using the supplied distance.
* <br><br>
* <b>Role:</b> Functions to extend splines, which use this <tt>spl_sur</tt> definition.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param spl
* input surface.
* @param dist
* distance to extend.
**/

DECL_KERN spline *extend_spline(
			spline const &spl,
			double       dist
		);

/**
* @nodoc
* <b>Obsolete:</b> This extension has been deprecated.
* <br><br>
* Creates an extension to a given spline using the supplied distance.
* <br><br>
* <b>Role:</b> Functions to extend splines, which use this <tt>spl_sur</tt> definition.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param spl
* input surface.
* @param dist
* distance to extend.
**/

DECL_KERN spline *extend_spline(
			spline const &spl,
			double       dist[2][2]
		);

/**
* @nodoc
* <b>Obsolete:</b> This extension has been deprecated.
* <br><br>
* Creates an extension to a given spline using the supplied distance.
* <br><br>
* <b>Role:</b> Functions to extend splines, which use this <tt>spl_sur</tt> definition.
* <br><br>
* <b>Effect:</b> Changes model
* <br><br>
* @param spl
* input surface.
* @param range
* change in range.
**/

DECL_KERN spline *extend_spline(
			spline const     &spl,
			SPApar_box const &range
		);
/** @} */
#endif
