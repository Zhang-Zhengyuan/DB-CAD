/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//               Offset Options
/*******************************************************************/
#if !defined(OFFSET_OPTIONS_H)
#define OFFSET_OPTIONS_H
#include "acis.hxx"
#include "dcl_lopt.h"
class option_header;
class OfstJournal;
class LoptJournal;
/*******************************************************************/
/**
* @file offset_opts.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup OFSTAPI
 *  @{
 */
/**
 * Offset options values.
 * <br><br>
 * @param simplify_on
 * simplifies the offset spline surface into analytical surface, if possible.
 * @param simplify_off
 * does not simplify offset spline surface into analytical surface.
 * @param simplify_default
 * when simplify_default value is specified, the option_header <tt>lop_simplify</tt>'s
 * value is used for defining the offset surface behavior.
 */
enum ofst_simplify_state
{
    simplify_on,
    simplify_off,
    simplify_default
};

/**
 * Offset side values for api_offset_edges_on_faces.
 * <br><br>
 * @param SPA_OFST_ON_LEFT_FACE
 * the edges are offset on the face(s) associated with the input.
 * @param SPA_OFST_ON_RIGHT_FACE
 * the edges are offset on face(s) adjacent to that associated with the input
 */
enum offset_on_faces {                 
	SPA_OFST_ON_LEFT_FACE,
	SPA_OFST_ON_RIGHT_FACE 
	};

/**
 * Options for offset operations.
 * <br>
 * <b>Role:</b> This class allows you to control
 * the behavior of the offset faces created.
 * <br><br>
 * <tt>simplify</tt>: Specifies whether to simplify the offset of a spline
 * surface to analytic surface (if possible) or not. "simplify" is an enum 
 * data type having these values:
 * <br><br>
 * <i><b>simplify_on </b></i> specifies to simplify the offset<br>
 * <i><b>simplify_off </b></i> specifies not to simplify the offset<br>
 * <i><b>simplify_default </b> specifies that the option_header <tt>lop_simplify</tt>'s
 * value is to be used for defining the offset surface behavior.
 * <br><br>
 * This option replaces the functionality of the option_header <tt>lop_simplify</tt>.</i> 
 */
class DECL_LOPT offset_options : public ACIS_OBJECT
{

    friend class OfstJournal;
    friend class LoptJournal;

	ofst_simplify_state _mSimplify;

    logical _mTrimBadGeom;

	/**
	 * Internal initialization.
	 */
	void init();

public:

	/**
	* Default constructor.
	*/
	offset_options();
	/**
	 * Copy constructor.<br><br>
	 * @param rRhs
	 * offset_options to copy.
	 */
	offset_options(const offset_options& rRhs);
	/**
	 * Destructor.
	 */
	~offset_options();
	 /**
     * Gets the status of the simplify flag which controls whether the offset
     * surface is to be simplified.
	 */
	logical get_simplify() const;
 	/**
	 * Sets value of simplify flag (which controls whether the offset surface is to be simplified)
     * to the given value.
	 * <br><br>
	 * @param bVal
	 * New value.
	 */
	void    set_simplify(ofst_simplify_state bVal);
    /**
	 * @nodoc
	 * verify version flag
	 */
    void set_trim_bad_geom( logical trim_bad ) { _mTrimBadGeom = trim_bad; }
    /**
	 * @nodoc
	 * verify version flag
	 */
    logical get_trim_bad_geom() { return _mTrimBadGeom; }
	/**
	 *  Equals Operator.
	 * <br><br>
	 * @param rRhs
	 * offset_options to be compared.
	 */
	logical operator==(const offset_options& rRhs);
	/**
	 * Non-equals operator.
	 * @param rRhs
	 * offset_options to be compared.
	 */
	logical operator!=(const offset_options& rRhs);
    /**
	 * @nodoc
	 * verify version flag
	 */
	void verify_version();

};
/*! @} */
#endif

