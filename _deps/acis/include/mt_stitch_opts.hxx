/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef MT_STITCH_OPTS_HXX
#define MT_STITCH_OPTS_HXX

#include "stchapi.hxx"

/**
* @file mt_stitch_opts.hxx
* \addtogroup STITCHAPI
* \addtogroup STITCH_OPTIONS
*
* @{
*/

/**
* The maximum stitching tolerance option class. 
* @par Technical Article
* <i>[Maximum Stitching Tolerance Option](http://doc.spatial.com/articles/m/u/l/Multithreaded_Stitching_8e62.html#Maximum_Stitching_Tolerance)</i>
* @par Role
* This option specifies the maximum stitching tolerance in multithreaded
* stitching. This option is relevant for @href api_stitch_make_input and 
* @href api_stitch. This class serves as a public base class for 
* mt_stitch_options.
* @see 
*/
class DECL_STITCH max_stitch_tol_option
{
public:
	/**
	* The public member to set the maximum stitching tolerance.
	* @param[in] _max_stitch_tol a double value specifying the maximum stitching tolerance
	*/
	void set_max_stitch_tol(double _max_stitch_tol);
	/**
	* The public member to get the maximum stitching tolerance.
	* @return the maximum stitching tolerance
	*/
	double get_max_stitch_tol() const;
protected:
	max_stitch_tol_option();

private:
	double max_stitch_tol;
};

/**
* The managing coincident face pairs option class. 
* @par Technical Article
* <i>[Managing Coincident Faces Option](http://doc.spatial.com/articles/m/u/l/Multithreaded_Stitching_8e62.html#Managing_Coincident_Faces)</i>
* @par Role
* This option controls the management of coincident face pairs in multithreaded
* stitching. It is relevant only for @href api_stitch_manage_coins. This class
* serves as a public base class for mt_stitch_options.
*/
class DECL_STITCH manage_coins_option
{
public:
	/**
	* The enumerator that specifies all available options.
	*/
	enum MANAGE_COINS_TYPE{
		DETECT, /**< detect coincident face pairs. */
		EXCLUDE /**< do not stitch coincident face pairs (not implemented -- revert to DETECT if selected). */
	};
	const char* manage_coins_type_to_str() const;
	/**
	* The public member to set the option.
	* @param[in] _type a MANAGE_COINS_TYPE enum
	*/
	void set_manage_coins_type(MANAGE_COINS_TYPE _type);
	/**
	* The public member to get the option.
	* @return the set MANAGE_COINS_TYPE enum
	*/
	MANAGE_COINS_TYPE get_manage_coins_type() const;

protected:
	manage_coins_option();

private:
	MANAGE_COINS_TYPE type;
};

/**
* The body validation option class. 
* @par Technical Article
* <i>[Validation Option](http://doc.spatial.com/articles/m/u/l/Multithreaded_Stitching_8e62.html#Validation)</i>
* @par Role
* This option controls the validation of the stitched bodies in multithreaded
* stitching. It is relevant only for @href api_stitch. This class serves as a
* public base class for mt_stitch_options.
*/
class DECL_STITCH validation_option
{
public:
	/**
	* The enumerator that specifies all available options.
	*/
	enum VALIDATION_TYPE{
		SHEETS_AND_SOLIDS, /**< (default) make open bodies to double sided outside sheet bodies and closed bodies to solid bodies. */
		SHEETS_ONLY /**< make all bodies regardless if closed or not to double sided outside sheet bodies. */
	};
	const char* validation_type_to_str() const;
	/**
	* The public member to set the option.
	* @param[in] _type a VALIDATION_TYPE enum
	*/
	void set_validation_type(VALIDATION_TYPE _type);
	/**
	* The public member to get the option.
	* @return the set VALIDATION_TYPE enum
	*/
	VALIDATION_TYPE get_validation_type() const;

protected:
	validation_option();

private:
	VALIDATION_TYPE type;
};

/**
* The body healing option class. 
* @par Technical Article
* <i>[Healing Option](http://doc.spatial.com/articles/m/u/l/Multithreaded_Stitching_8e62.html#Healing)</i>
* @par Role
* This option controls the healing of the stitched bodies in multithreaded
* stitching. It is relevant only for @href api_stitch. This class serves as a
* public base class for mt_stitch_options.
*/
class DECL_STITCH heal_option
{
public:
	/**
	* The enumerator that specifies all available options.
	*/
	enum HEAL_TYPE{
		HEAL_ALL, /**< (default) apply healing operations to stitched bodies. */
		NO_HEAL /**< do not apply healing operations to stitched bodies. */
	};
	const char* heal_type_to_str() const;
	/**
	* The public member to set the option.
	* @param[in] _type a HEAL_TYPE enum
	*/
	void set_heal_type(HEAL_TYPE _type);
	/**
	* The public member to get the option.
	* @return the set HEAL_TYPE enum
	*/
	HEAL_TYPE get_heal_type() const;

protected:
	heal_option();

private:
	HEAL_TYPE type;
};

/**
* The compound option class for multithreaded stitching. 
* @par Technical Article
* <i>[Multithreaded Stitching Options](http://doc.spatial.com/articles/m/u/l/Multithreaded_Stitching_8e62.html#Compounded_Stitching_Options)</i>
* @par Role
* This class holds all options for multithreaded stitching and is used in 
* @href api_stitch_make_input, @href api_stitch_manage_coins, and 
* @href api_stitch. A pointer to mt_stitch_options is passed to the above
* mentioned APIs to control their behavior. The class mt_stitch_options has
* been derived from stitch_options, max_stitch_tol_option, manage_coins_option,
* validation_option, and heal_option.
*/
class DECL_STITCH mt_stitch_options : 
	public stitch_options, 
	public max_stitch_tol_option,
	public manage_coins_option,
	public validation_option,
	public heal_option
{
public:
	/**
	* @nodoc
	*/
	virtual STITCH_TYPE get_stitch_type() const
	{
		return MT_STITCH_TYPE;
	}
};

/** @} */

#endif // MT_STITCH_OPTS_HXX
