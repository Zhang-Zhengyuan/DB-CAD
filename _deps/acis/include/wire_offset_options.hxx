/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( WIRE_OFFSET_OPTIONS_CLASS )
#define WIRE_OFFSET_OPTIONS_CLASS

#include "acis.hxx"
#include "dcl_ofst.h"
#include "off_wire.hxx"
#include "unitvec.hxx"
#include "vers.hxx"

/*******************************************************************/
/**
* @file wire_offset_options.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup OFSTAPI
 *  @{
 */
/**
 * Used to specify unset values in <tt>wire_offset_options</tt>.
 */
#if !defined(UNSET_OPTION)
	#define UNSET_OPTION -1
#endif

class law;

/**
 * Provides a data structure for wire offsetting operations to be used in the function @href api_offset_planar_wire.
 * <br>
 * <b>Role:</b> This class holds various options for the wire offset operation. The following
 * lists the options:
 * <ul>
 * <li><tt>distance</tt>: This option specifies the offset distance. It is represented by a law.
 * The default value is a NULL law.</li>
 * <li><tt>gap_type</tt>: This option specifies the type of gap to be placed when
 * the offsets of adjacent, nontangential wire edges do not touch at a common vertex.
 * The type can be:
 * <ul>
 * <li><tt>"extended"</tt> which linearly extends the offset curves along their
 * tangents until they intersect (that is, creates a corner),</li> 
 * <li><tt>"rounded"</tt> which connects the endpoints with a tangential arc, or</li> 
 * <li><tt>"natural"</tt> which extends the curves using curvature extension rather than linear extension.</li>
 * </ul>
 * Presently, the gap type <tt>"natural"</tt> might be done with a linear extension if curvature 
 * extension cannot be achieved. It might also be done with a rounded extension if the
 * extensions do not intersect. A warning is issued if the <tt>gap_type</tt> is changed.
 * The default value is <tt>"natural"</tt>.</li>
 * <li><tt>keep_minimum_topology</tt>: Wire offset might create additional edges for gap fill geometry.
 * These additional edges will not be created if <i>keep_minimum_topology</i> is <tt>TRUE</tt>. This is done by
 * extending the offset edges. In cases the offset edge cannot be extended, the wire offset operation
 * will fail and an error message is issued. The default value is 0, <tt>FALSE</tt>.</li>
 * <li><tt>overlap</tt>: This option specifies whether overlapping segments are to be removed.
 * The default value is 0, <tt>FALSE</tt>.</li>
 * <li><tt>plane_normal</tt>: This option specifies the normal vector of the wire plane, which 
 * is the plane where the offset wire body lies. The default value is a NULL unit vector.</li>
 * <li><tt>trim</tt>: This option specifies whether self-intersections are to be removed.
 * The default value is 1, <tt>TRUE</tt>.</li>
 * <li><tt>twist_law</tt>: This option specifies the twist law. The default value is a NULL law,
 * implying no twist.</li>
 * <li><tt>zero_length</tt>: If the option is not set to zero, then the offset will return a zero-length 
 * wire when offset to a point. The default value is 0, <tt>FALSE</tt>.</li>
 * </ul>
 * Note: All the options settings are optional, except distance, which must be specified for a successful 
 * wire offsetting operation. If an option is not specified, the default value is used. 
 */
class DECL_OFST wire_offset_options : public ACIS_OBJECT
{
private:
	// -1 unset 0 false 1 true
	sg_gap_type    mGapType;
	int            mKeepMiniTopology;
   int            mTrim;
   int            mOverlap;
   SPAunit_vector mPlaneNormal;

   law            *mDistLaw;
   law            *mTwistLaw;
   int			  mZeroLength;
   int            mAddAttribs;

public:
/**
 * Default constructor.
 */
	wire_offset_options();
/**
 * Default destructor.
 */

	~wire_offset_options();


// Set functions.

/**
 * Sets the gap filling type.
 * <br><br>
 * @param iGapType
 * Gap type can be extended, rounded, or natural.
 */
	void set_gap_type (sg_gap_type iGapType);
/**
 * Sets the value of the <i>keep_minimum_topology</i> option.
 * <br><br>
 * @param iStatus
 * new <i>keep_minimum_topology</i> option value.
 */
	void set_keep_minimum_topology (int iStatus);
/**
 * Sets the value of the <i>trim</i> option.
 * <br><br>
 * @param iStatus
 * new <i>trim</i> option value.
 */
	void set_trim (int iStatus);
/**
 * Sets the value of the <i>overlap</i> option.
 * <br><br>
 * @param iStatus
 * new <i>overlap</i> option value.
 */
	void set_overlap (int iStatus);
/**
 * Sets the value of the <i>plane_normal</i> option.
 * <br><br>
 * @param iNormal
 * new <i>plane_normal</i> option value.
 */
	void set_plane_normal (const SPAunit_vector &iNormal);
/**
 * Sets the value of the <i>zero_length</i> option.
 * <br><br>
 * @param iStatus
 * new <i>zero_length</i> option value.
 */
	void set_zero_length (int iStatus);
/**
 * @nodoc
 * Internal use only.
 */
	void set_add_attribs (int iStatus);

/**
 * Sets the <i>distance</i> option.
 * <br><br>
 * @param iDistLaw
 * new <i>distance</i> option value.
 */
   void set_distance (law* iDistLaw);
/**
 * Sets the <i>distance</i> option.
 * <br><br>
 * <b>Role:</b> This signature of <tt>set_distance</tt> takes a double and 
 * converts it into a constant law to represent the offset distance.
 * <br><br>
 * @param iDistance
 * new <i>distance</i> option value.
 */
   void set_distance (double iDistance);

 /**
 * Sets the <i>twist_law</i> option.
 * <br><br>
 * @param iTwistLaw
 * new <i>twist_law</i> option value.
 */
   void set_twist_law(law* iTwistLaw);

/**
 * Returns the current value of the <i>gap_type</i> option.
 */
	sg_gap_type get_gap_type () const;
/**
 * Returns the current value of the <i>keep_minimum_topology</i> option.
 */
	int get_keep_minimum_topology () const;
/**
 * Returns the current value of the <i>trim</i> option.
 */
   int get_trim () const;
/**
 * Returns the current value of the <i>overlap</i> option.
 */
	int get_overlap () const;
/**
 * Returns the current value of the <i>plane_normal</i> option.
 */
   SPAunit_vector get_plane_normal () const;
/**
 * Returns the current value of the <i>zero_length</i> option.
 */
   int get_zero_length () const;
/**
 * @nodoc
 * Internal use only.
 */
   int get_add_attribs () const;
/**
 * Returns the <i>distance</i> option.
 */
   law *get_distance_law() const;

/**
 * Returns the <i>twist</i> option.
 */
   law *get_twist_law() const;


/**
 * Verifies whether this option instance is equal to the given offset option.
 * <br><br>
 * @param iSrcOpt
 * source offset options to test.
 */
	int operator== (const wire_offset_options& iSrcOpt);
/**
 * Verifies whether this option instance is not equal to the given offset option.
 * <br><br>
 * @param iSrcOpt
 * source options to test.
 */
	int operator!= (const wire_offset_options& iSrcOpt);

};
/*! @} */
#endif

