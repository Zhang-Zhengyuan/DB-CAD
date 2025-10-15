/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( ADM_OPTIONS_CLASS )
#define ADM_OPTIONS_CLASS


#include "acis.hxx"
#include "dcl_adm.h"

// class adm_options
// Purpose:  adm_options override AcisOptions algorithmic versioning for specific adm behaviors:
//           the use of boundary loads, and automatic surface trimming.
//
// Description: An adm_options object is optionally passed to adm api's; adm api's have names of the form api_dm_XXX.
//              adm_option member functions are used to set and query the option values.

/**
* @file adm_options.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISDM
 *
 * @{
 */

/**
 * Class to override the <tt>AcisOptions</tt> algorithmic versioning for specific adm behaviors - use of boundary loads, and automatic surface trimming.
 * <br><br>
 * <b>Role:</b> adm apis have names of the form <tt>api_dm_XXX</tt>. An <tt>adm_options</tt> object
 * is optionally passed to adm apis. The member functions of this class are used
 * to set and query the option values.
 */

class DECL_ADM adm_options : public ACIS_OBJECT
{


protected:
    int trim_faces;
    int use_bdy_loads;
public:
/**
 * Constructs an <tt>adm_options</tt> object.
 */
    adm_options();
/**
 * Destructor.
 */
    ~adm_options();

// set_get functions
/**
 * Sets the value of the <tt>trim_faces</tt> member to the specified value.
 * <br><br>
 * <b>Role: </b>A value of 0 indicates not to trim, and a value of 1 indicates trim.
 * <br><br>
 * @param value
 * value to set
 */
    void set_trim_faces(int value);      // 0 for don't trim, 1 for trim
/**
 * Returns the current value of the <tt>trim_faces</tt> member.
 */
    int get_trim_faces() const;    // query options setting for trim faces
/**
 * Sets the value of the <tt>use_bdy_loads</tt> member to the specified value.
 * <br><br>
 * <b>Role: </b>A value of 0 indicates not to use loads on FACE boundaries
 * (use constraints instead), and a value of 1 indicates use loads.
 * <br><br>
 * @param value
 * value to set
 */
    void set_use_bdy_loads(int value);   // 0 for don't use loads on FACE boundaries (use constraints instead), 1 for use loads
/**
 * Returns the current value of the <tt>use_bdy_loads</tt> member.
 */
    int get_use_bdy_loads() const; // query options setting for boundary loads

};

 /** @} */
#endif
