/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//                Detect Match Options
/*******************************************************************/

#ifndef DETECT_MATCH_OPTS_HXX
#define DETECT_MATCH_OPTS_HXX

/*******************************************************************/
// Include files
#include "dcl_query.h"	// DECL_QUERY
#include "acis.hxx"
#include <ostream>

/*******************************************************************/

class detect_match_options_data;
class match_filter_group_data;
class detect_match_body_criteria_impl;
/**
 * \defgroup DETECTFEATURE Detect Feature
 */

/**
 * @file detect_match_opts.hxx
 */

/** 
 * \addtogroup DETECTFEATURE
 * \brief Declared at <detect_match_opts.hxx>
 * @{
 */



/**
 * Options for match detection.
 * <br>
 * <b>Role:</b> This class contains information to be used to control
 * the behaviour of match detection done in @href api_detect_match.
 * <br>
 * <table border=0 cellspacing=0 cellpadding=8 >
 * <tr>
 * <td valign = "top"><tt>distance_allowance</tt></td>
 * <td valign = "top"> Distance allowance. This is a positive double value that is used as the threshold for the distance comparison. 
 *  In case of coincidence filter, this value is used as upper bound of the gap between the candidate entity and the surface 
 *  of the seed entity. In order to use the ACIS default setting, the value of this
 *  option needs to be set to -1.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>angle_allowance</tt></td>
 * <td valign = "top"> Angle allowance. This is used as the threshold for the angular gap comparison.
 * The angle input is in radians and it should be a positive double value. In case of coincidence filter, this value is used as upper bound of the angular gap between the surfaces 
 *  of the seed and candidate entities. For example, during comparison of two planar faces, <tt>angle_allowance</tt>
 *  serves as the maximum angle between the two plane-normals. It is recommended to maintain this option 
 * value of the order of <tt>SPAresnor</tt>. In order to use the ACIS default setting, the value of this
 *  option needs to be set to -1.</td>
 * </tr>
 * <tr>
 * <td valign = "top"><tt>honor_face_normals</tt></td>
 * <td valign = "top"> Honor face normals. Flag to decide whether face sense is to be taken into consideration during match detection.
 * When this flag is set to true, only holes are considered comparable to holes and not to bosses. Otherwise, a candidate face of a hole
 * or pocket feature can be considered matching with a seed face of a boss or protrusion feature. By default, this flag is set to false.</td>
 * </tr>
 * </table>
*/

class DECL_QUERY detect_match_options : public ACIS_OBJECT
{

public: 
	/**
	* Default constructor.
	*/
	detect_match_options(double distance_allowance = -1.0, 
						 double angle_allowance = -1.0, 
						 bool honor_face_normals = false);

	/**
	* Destructor.
	*/
	virtual ~detect_match_options();

	void output(std::ostream& os) const;

private : 

	detect_match_options_data* _data;

	friend class detect_match_options_data;

};


/**
 * Provides ability to specify granularity with which matching is performed.
 * <br>
 * <b>Role:</b> The constructor of this class takes in a integer specifying the granularity of matching.
 * 1 represents coarse matching criteria while 10 represents a finer match.
 */

class DECL_QUERY detect_match_body_criteria : public ACIS_OBJECT
{
private:
	detect_match_body_criteria_impl* _data;
	friend class detect_match_body_criteria_impl;
public:
	/**
	* Default constructor.
	*/
	detect_match_body_criteria();
	/**
	* Constructor takes an integer in range [1-10] signifying the granularity of the match to be attempted. 1 signifies coarse, 10 more fine.
	*/
	detect_match_body_criteria(int level );
	virtual ~detect_match_body_criteria();
	/**
	* Provides the ability to change the match level criteria.
	*/
	void set_match_level(int);
};


/**
 * Match Detection filters.
 * <br>
 * <b>Role:</b> This class contains information about the criteria to be used during the 
 * match detection done in @href api_detect_match. Objects of this class need to be created using the
 * static functions of @href match_filter class.
 * <br>
*/

class DECL_QUERY match_filter_group : public ACIS_OBJECT 
{

public: 
	/**
	* Copy constructor.
	*/
	match_filter_group( match_filter_group const & );

	match_filter_group& operator= (match_filter_group const&);
	/**
	* Destructor.
	*/
	virtual ~match_filter_group();

private:
	match_filter_group( match_filter_group_data *data );

	match_filter_group_data *_data;

	friend class match_filter;
};

/**
 * Match filter.
 * <br>
 * <b>Role:</b> This class provides the static functions for creating @href match_filter_group object.
 * <br>
*/

class DECL_QUERY match_filter: public ACIS_OBJECT
{
public:
	/**
	* Function to create coincidence filter.
	*/
	static match_filter_group create_coincident();
};

/** @} */

#endif  // DETECT_MATCH_OPTS_HXX
