/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//                EDM Offset Options
/*******************************************************************/

#if !defined(EDM_OFFSET_OPTIONS_H)
#define EDM_OFFSET_OPTIONS_H
#include "lop_opts.hxx"


/*******************************************************************/
/**
 * @nodoc
 * @file edm_offset_opts.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup LOPEDM
 *
 * @{
 */

/**
 * @nodoc
 * Direction of EDM offset relative to the face normals.
 * @param edm_offset_inward
 * Offset in the opposite direction of the face normal.
 * @param edm_offset_outward
 * Offset in the same direction as the face normal.
 */
enum edm_offset_dir
{
    edm_offset_inward,
	edm_offset_outward
};


/**
 * @nodoc
 * Options for the EDM offset operation.
 * <br>
 * <b>Role :</b> This class contains information used to control
 * the behavior of the EDM offset operation.
 * <br><br>
 * These options include: <br><br>
 * <table border=0 cellspacing=0 cellpadding=8 >
 * <tr>
 * <td valign = "top"><tt>_offset_dir</tt></td>
 * <td valign = "top">This is the direction of the EDM offset relative to the FACE normals (default is inward) </td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>_fail_if_not_manufacturable</tt></td>
 * <td valign = "top">This logical controls if the EDM offset operation fails when 
 * it encounters a geometric shape that cannot be manufactured using the specified 
 * tool motion (default is TRUE)
 * </td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>_lop_options</tt></td>
 * <td valign = "top">The lop options object that the edm_offset_options object holds</td>
 * </tr>
 * </table>
 */
class DECL_LOPT edm_offset_options : public ACIS_OBJECT
{
	edm_offset_dir _offset_dir;				    // default edm_offset_inward
    logical _fail_if_not_manufacturable;        // default TRUE
	lop_options _lop_options;
	/**
	 * Internal initialization.
	 */
	void init();

public:
	/**
	 * C++ allocation constructor requests memory for this object but does not populate it.
	 */
	edm_offset_options();

	/**
	 * C++ copy constructor.<br><br>
	 * @param rRhs
	 * edm_offset_options to copy.
	 */
	edm_offset_options(const edm_offset_options& rRhs);

	/**
	 * C++ default destructor.
	 */
	~edm_offset_options();

	/**
	 * Gets the EDM offset direction
	 */
	edm_offset_dir get_offset_dir() const;

	/**
	 * Gets the _fail_if_not_manufacturable logical
	 */
	logical get_fail_if_not_manufacturable() const;

	/**
     *  Gets the lop_options object.
     */
    lop_options& get_lop_options() const;
	
	/**
	 * Sets the EDM offset direction to the given value.
	 * <br><br>
	 * @param eVal
	 * New value.
	 */
	void set_offset_dir(edm_offset_dir eVal);

    /**
	 * Sets the _fail_if_not_manufacturable flag.
	 * <br><br>
	 * @param bVal
	 * New value.
	 */
	void set_fail_if_not_manufacturable(logical bVal);

	/**
	 * Sets the lop_options object in this edm_offset_options object
	 * <br><br>
	 * @param input_opts
	 * New value.
	 */
	void set_lop_options(lop_options& input_opts);


	/**
	 * Equals operator.
	 * <br><br>
	 * @param rRhs
	 * edm_offset_options to be compared.
	 */
	logical operator==(const edm_offset_options& rRhs);

	/**
	 * Non-equals operator.
	 * @param rRhs
	 * edm_offset_options to be compared.
	 */
	logical operator!=(const edm_offset_options& rRhs);
};

/**
 * @nodoc
 */
#define SET_EDM_OFFSET_OPTIONS(pEopts) \
    edm_offset_options dummy; if (!pEopts) pEopts = &dummy;
/** @} */
#endif
