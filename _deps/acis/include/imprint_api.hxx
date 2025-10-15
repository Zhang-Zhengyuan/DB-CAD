/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// ------------------------------------------------------------------------------------------------
// Interfaces to perform imprint operation and query the output.
// ------------------------------------------------------------------------------------------------

#ifndef IMPRINT_API_HXX
#define IMPRINT_API_HXX

// ------------------------------------------------------------------------------------------------

/**
 * @file imprint_api.hxx
 */

// ------------------------------------------------------------------------------------------------
// Includes:

#include "acis.hxx"
#include "dcl_bool.h"
#include "api.hxx"

#include "boolean_handles.hxx"
#include "bool_api_options.hxx"

#include <ostream>

// ------------------------------------------------------------------------------------------------
// Forward decls:

class BODY;
class ENTITY;
class imprint_input_handle;
class imprint_output_handle;
class imprint_options;
class imprint_options_data;

// ------------------------------------------------------------------------------------------------

/*! \addtogroup BOOLADVIMPRINT
 *  \brief Declared at <imprint_api.hxx>, SPAbool
 *  @{
 */

/**
 * Performs an imprint operation on the input specified.
 * <br><b>Technical Article:</b> <i>[Imprint](http://doc.spatial.com/articles/i/m/p/Imprint.html)</i>
 * <br><br>
 * <b>Role:</b> This API function makes an impression of one body on to another. It embeds the 
 * intersection graph on to the participating bodies. The API creates and returns an 
 * <tt>imprint_output_handle</tt> if user has asked for association between the imprinted entities
 * on tool and blank, otherwise returns a NULL output handle.
 * This API is optionally <i>Failsafe</i>. Under <i>Failsafe</i> mode, the API function attempts to
 * return as much of the result as possible. When the result is partial, then the API will still 
 * return success, but the returned <tt>outcome</tt> will return true if <tt>encountered_errors()</tt>
 * is called. The caller should examine the entities linked with the error information as part
 * of the errors and take any action required to compensate for the missing imprints.
 * <br><br>
 * <i>Failsafe</i> mode is enabled using the global option "bool_fs_imprint", as well as ensuring
 * that the "careful" option is set to <tt>FALSE</tt>. Setting the "careful" option to <tt>TRUE</tt>
 * will turn off Failsafe behavior. In non-failsafe mode the model is completely rolled back 
 * on the occurrence of the first unhandled error.
 * <br><br>
 * <b><i>Note:</i></b> The handle to the imprint output if returned can not be directly consumed. 
 * To query the association an object of @href imprint_output_query need to be instantiated. 
 * The imprint output handle should be released by calling <tt>remove_ref()</tt> after usage.
 * <br><br>
 * The imprint options argument, <tt>imopt</tt>, allows the caller to pass additional information 
 * to the imprint operation. Refer to @href imprint_options for more details.
 * <br><br>
 * <b><i>Note:</i></b> As a side effect of the imprint operation the tool body is transformed
 * into the coordinate system of the blank body.
 * <br><br>
 * <b>Errors:</b><br>
 * <tt>hii</tt> is a NULL pointer or constructed with invalid input bodies.<br>
 * <br><br>
 * <b>Effect:</b> Modifies both the participating bodies or as specified in imprint options.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param hii
 * (in) Handle to imprint input. Refer to @href imprint_input_handle for details.
 * @param hio
 * (out) Handle to the output of imprint operation. Refer to @href imprint_output_handle for details.
 * @param imopt
 * (in) Imprint options. Refer to @href imprint_options for details.
 * @param ao
 * (in) ACIS options such as versioning and journaling. Refer to @href AcisOptions for details.
 **/

DECL_BOOL outcome
api_imprint( imprint_input_handle *hii,
             imprint_output_handle *&hio,
			 imprint_options const *imopt = NULL,
			 AcisOptions const *ao = NULL );

// ------------------------------------------------------------------------------------------------

/**
 * Represents a handle to the inputs given to Imprint operation.
 * Once created and passed to an API, the handle is fully owned by the API.
 */

class DECL_BOOL imprint_input_handle : public boolean_input_handle
{
public:

	/**
	 * Destructor
	 */

	virtual ~imprint_input_handle();

protected:

	/**
	 * @nodoc
	 */
	// Default constructor. For internal use only!

	imprint_input_handle();
};
// ------------------------------------------------------------------------------------------------

/**
 * Represents an interface to supply inputs to an imprint operation.
 */

class DECL_BOOL imprint_input_maker
{
public:

	/**
	 * Constructs an object of <tt>imprint_input_maker</tt> with the given tool and blank
	 * bodies.
	 */

	imprint_input_maker( BODY *tool,
						 BODY *blank );

	/**
	 * Constructs an object of <tt>imprint_input_maker</tt> with slice output handle
	 * bodies.
	 */

	imprint_input_maker( slice_output_handle *hso );

	/**
	 * Destructor
	 */

	~imprint_input_maker();

	/**
	 * Returns a pointer of imprint input handle that can be used to pass into imprint API. 
	 * This call transfers the ownership of handle to the caller.
	 */

	imprint_input_handle* unhook_handle();

private:
	
	// For internal use only!

	imprint_input_handle *_h;
};
// ------------------------------------------------------------------------------------------------


/**
 * This class will provide user an interface to customize the behavior of imprint operation.
 * Object of this class would be created by user on stack.
 */

class DECL_BOOL imprint_options : public bool_base_options
{
public:

	/**
	 * Default constructor
	 */

	imprint_options();

	/**
	 * Destructor
	 */

	~imprint_options();

	/** Enumerates the types of modification of a particular input body i.e. blank or tool.
	 * Values:
	 *			MOD_SKIP		- Skips the specified body from modification. Currently tool can be 
	 *								  skipped but not the blank.
	 *			MOD_IN_PLACE	- Performs an imprint on the specified body in-place
	 * The default mode for both tool and blank is MOD_IN_PLACE.
	*/

	enum modification_type
	{
		MOD_IN_PLACE,
		MOD_SKIP
		// Proposed: , MOD_COPY
	};

	/**
	 * Sets the modification type for either blank or tool as specified.
	 */

	void set_modification_type( bool_body_type btype, modification_type m );

	/**
	 * Sets the upper bound of the resolution to be used by imprint algorithm to include interactions
	 * between tool and blank. The default value is -1, means that computation will adhere the 
	 * default resolution, i.e. SPAresabs.
	 */

	void set_fuzz( double = -1.0 );

	/**
	 * Enumerates the different atomicity levels for the imprint operation when it runs on
	 * failsafe mode.
	 * Values:
	 *			FA_DEFAULT		- Default granularity
	 *			FS_ENTITY_PAIR	- Granularity is a pair of entities from tool and blank.
	 *							  for solid/sheet the entity is face and for a wire the entity is edge.
	 *			FS_SLICE_EDGE	- Granularity is edge of slice.
	 *			FS_BODY_ENTITY	- Granularity is the entity of input body, say tool face.
	 *							  for solid/sheet the entity is face and for a wire the entity is edge.
	 */

	enum fs_atomicity
	{
		FA_DEFAULT,
		FA_ENTITY_PAIR,
		FA_SLICE_EDGE,
		FA_BODY_ENTITY
	};

	/**
	 * Sets the failsafe atomicity of the operation. If the atomicity is not set then the default
	 * value is <tt>FA_DEFAULT</tt>.
	 */

	void set_fs_atomicity( fs_atomicity fa );

	/**
	 * Sets the need to get association through <tt>imprint_output_handle</tt>. If this function is called
	 * with true then the association between imprinted entities on tool and blank is made 
	 * available through <tt>imprint_output_handle</tt>. 
	 */

	void set_need_association( bool need_assoc );

	/**
	* @nodoc
	*/

	void output(std::ostream& os) const;

	/**
	* @nodoc
	*/

	int type() const override
	{
		return id();
	}

	/**
	* @nodoc
	*/

	static int id();

private:
	
	// Copy constructor and Assignment operators - to avoid duplication of objects of this class.
	
	imprint_options( imprint_options const & );
	imprint_options& operator=( imprint_options const & );

	// Data members.

	imprint_options_data *_data;

	// Friends.

	friend class imprint_options_data;
};
// ------------------------------------------------------------------------------------------------


/**
 * This class allows a user to query the output of an imprint operation. It can provide the
 * association between the edges/vertices imprinted on both blank and tool bodies.
 */

class DECL_BOOL imprint_output_query
{
public:

	/**
	 * Constructor to create an object of this class given an imprint_output_handle.
	 */

	imprint_output_query( imprint_output_handle const *h );

	/**
	 * Returns NULL unless the imprint operation is expected to make a new tool body.
	 */

	BODY* tool() const;

	/**
	 * Returns NULL unless the imprint operation is expected to make a new blank body.
	 */

	BODY* blank() const;

	/**
	 * Returns the imprinted entities for the given bool body type.
	 */

	void imprinted_entities( bool_body_type body_typ, ENTITY_LIST &imprinted_ents ) const;

	/**
	 * Finds associated entities given an imprinted entity. For example, if an imprinted edge on blank
	 * is given then the corresponding imprinted edge on tool is returned.
	 */

	void associated_entities( ENTITY *imprinted_ent, ENTITY_LIST &assoc_ents ) const;

private:

	// For internal use only!

	imprint_output_handle const *_hio;	
};
/** @} */
// ------------------------------------------------------------------------------------------------

#endif // IMPRINT_API_HXX

