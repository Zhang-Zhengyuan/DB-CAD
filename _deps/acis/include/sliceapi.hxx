/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// ------------------------------------------------------------------------------------------------
// Interfaces to perform slice and query the output.
// ------------------------------------------------------------------------------------------------

#ifndef SLICEAPI_HXX
#define SLICEAPI_HXX

// ------------------------------------------------------------------------------------------------

/**
 * \defgroup BOOLSLICEAPI Slice
 * \ingroup BOOLADVAPI
 */

/**
 * @file sliceapi.hxx
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
class plane;
class SPAdouble_vector;

class slice_options;
class slice_output_query;
class slice_options_data;
struct slice_assoc_data;

// ------------------------------------------------------------------------------------------------

/*! \addtogroup BOOLSLICEAPI
 *  \brief Declared at <sliceapi.hxx>, SPAbool
 *  @{
 */

// ------------------------------------------------------------------------------------------------

/**
 * Performs a slice operation between sheet body and wire body or between two wire bodies.
 * <br><br>
 * <b>Role:</b> This API function computes the slice graph between two bodies.
 * It returns a handle to the slice graph, that can be further queried to get a wire-body.
 * Currently this API supports slice operation between a sheet body and a wire body and 
 * between two wire bodies.
 * <br><br>
 * <b><i>Note:</i></b> The handle to be slice output can not be directly consumed. To generate a 
 * explicit wire body suitable for subsequent general use, an object of @href slice_output_query
 * need to be instantiated and queried. The slice output handle should be released by calling
 * <tt>remove_ref()</tt> after usage.
 * <br><br>
 * The slice options argument, <tt>sopts</tt>, allows the caller to pass additional information 
 * to the slice operation. Refer to @href slice_options for more details.
 * <br><br>
 * <b>Errors:</b><br>
 * <tt>tool</tt> or <tt>blank</tt> is a NULL pointer or does not point to a @href BODY.<br>
 * <tt>tool</tt> or <tt>blank</tt> is not a wire body.<br>
 * <br><br>
 * <b>Effect:</b> Read-only on input. A new object representing the slice graph is created.
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param tool
 * (in) Tool, or slicing body.
 * @param blank
 * (in) Blank body to be sliced.
 * @param hso
 * (out) Handle to the output of slice operation.
 * @param sopts
 * (in) Slice options including versioning and journaling. Refer to @href slice_options for details.
 **/

DECL_BOOL outcome
api_slice( BODY *tool,
		   BODY *blank,
		   slice_output_handle *&hso,
		   slice_options const *sopts = NULL );

// ------------------------------------------------------------------------------------------------

/**
 * This class will provide user an interface to customize the behavior of slice operation.
 * Object of this class would be created by user on stack.
 */

class DECL_BOOL slice_options : public AcisOptions, public bool_base_options
{
public:

	/**
	 * Default constructor
	 */

	slice_options();

	/** 
	 * Constructor that accepts an AcisOptions object, if you already have one. 
	 * The argument ao should not be NULL.
	 */

	slice_options( AcisOptions const *ao );

	/**
	 * Destructor
	 */

	~slice_options();

	/**
	 * Sets the upper bound of the resolution to be used by slice algorithm to include interactions
	 * between tool and blank. Slice will compute all interactions where the gap is less than the
	 * supplied "fuzz". The default value of fuzz is -1, means that computation will adhere the 
	 * default resolution, i.e. SPAresabs.
	 */

	void set_fuzz( double = -1.0 );

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
	
	slice_options( slice_options const & );
	slice_options& operator=( slice_options const & );

	// Data members.

	slice_options_data *_data;

	// Friends.

	friend class slice_options_data;
};

// ------------------------------------------------------------------------------------------------

/**
 * This class allows a user to create an explicit wire body from slice output handle.
 * The user can get a customized copy of the slice output by querying the output with necessary
 * options. This interface also establishes the association between the slice and the input bodies
 * ( i.e. blank / tool ).
 */

class DECL_BOOL slice_output_query
{
public:

	/**
	 * Enumerates various preferences to geometry of the slice output.
	 * <br>
	 * <b>Role:</b> The @href geom_preference enum is used to specify any preferences to the 
	 * geometry of the wire-body created by @href slice_output_query.
	 * @param DEFAULT
	 * No preferences. The geometry of the slice output would be as per Booleans strategy.
	 * @param FROM_TOOL
	 * The geometry of slice output would adhere to the tool. i.e. the geometry of edges/vertices
	 * of resulting slice wire would be within SPAresabs (or the edge/vertex tolerance) from the geometry 
	 * of the tool wherever possible.
	 * @param FROM_BLANK
	 * The geometry of slice output would adhere to the blank. i.e. the geometry of edges/vertices
	 * of resulting slice wire would be within SPAresabs (or the edge/vertex tolerance) from the geometry 
	 * of the blank wherever possible.
	 */	
	
	enum geom_preference
	{
		DEFAULT = 0,
		FROM_TOOL,
		FROM_BLANK
	};

	/**
	 * @nodoc
	 */

	class association;

	/**
	 * Constructor to create an object of this class given a slice_output_handle.
	 */

	slice_output_query( slice_output_handle const *h );

	/**
	 * Creates a wire-body that represents the slice output. The user can request to provide the 
	 * association between slice and its progenitors i.e. tool/blank.
	 */

	BODY* make_wire( association &assoc_map = *( association* )NULL_REF ) const;

	/**
	 * Creates a wire-body that represents the slice output. The user can request to provide the 
	 * association between slice and its progenitors i.e. tool/blank. Moreover, the geometry of the
	 * resulting wire-body can be fine-tuned through geom_preference argument.
	 */
	
	BODY* make_wire( association &assoc_map,
					 geom_preference pref ) const;

	/**
	 * Destructor.
	 */

	~slice_output_query();

private:

	// For internal use only!

	slice_output_handle const *_hs;	
};

// ------------------------------------------------------------------------------------------------

/*
 * Represents a container to store the association between edge or vertex of slice graph with its
 * progenitor i.e. tool/blank entity. Allows user to iterate through each association and also to
 * query the progenitor given a specific edge/vertex of slice.
 */

class DECL_BOOL slice_output_query::association
{
public:

	/**
	 * Default Constructor
	 */

	association();

	/**
	 * Destructor
	 */

	~association();

	/** 
	 * Initializes the iterator to the beginning. This function is meant to be used along
	 * with next() in order to iterate through each association.
	 */

	void init() const;

	/** 
	 * Returns the next association. If returned slice_ent is NULL, then it means the iterator
	 * has reached the end.
	 */

	bool next( ENTITY *&slice_ent,
			   ENTITY_LIST &tool_ents,
			   ENTITY_LIST &blank_ents ) const;

	/**
	 * Returns the progenitor entities from tool and blank, given an entity (either edge or vertex)
	 * of the slice. 
	 */

	void progenitors( ENTITY *slice_ent,
					  ENTITY_LIST &tool_ents,
					  ENTITY_LIST &blank_ents ) const;					

private:
	
	// Data members. For internal use only!

	slice_assoc_data *_data;

	friend class slice_output_query;
};
// ------------------------------------------------------------------------------------------------


/**
* Performs a slice operation between a given body and a set of multiple planes.
* <br><br>
* <b>Role:</b> This API function computes the slice graph between the given body and the set of
* multiple planes. It returns a handle to the slice graph, that can be further queried to get
* the wire-body that represents the slice graph.
* <b><i>Note:</i></b> The handle to be slice output can not be directly consumed. To generate a
* explicit wire body suitable for subsequent general use, an object of @href slice_output_legacy
* need to be instantiated and queried. The slice output handle should be released by calling
* <tt>remove_ref()</tt> after usage.
* <br><br>
* The slice options argument, <tt>sopts</tt>, allows the caller to pass additional information
* to the slice operation. Refer to @href slice_options for more details.
* <br><br>
* <b>Errors:</b><br>
* <tt>blank</tt> is a NULL pointer or does not point to a @href BODY.<br>
* <br><br>
* <b>Effect:</b> Read-only on input. A new object representing the slice graph is created.
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler, 3D ACIS Polyhedral
* <br><br>
* @param blank
* (in) Blank body to be sliced.
* @param datum
* (in) Reference plane for the set of planes to be used as tool
* @param offsets
* (in) Offset values to represent the tool planes. The datum will be used only there is an offset 0.0 supplied.
* @param hso
* (out) Handle to the output of slice operation.
* @param sopts
* (in) Slice options including versioning and journaling. Refer to @href slice_options for details.
*/

DECL_BOOL outcome
api_planar_slice( BODY* blank, 
				  plane const& datum,
				  SPAdouble_vector const &offsets,
				  slice_output_handle *&hso,
				  slice_options const *sopts );



/**
* Represents a light weight query operator that can extract the output from slice_output_handle.
* This class doesn't provide any information on association between the slice output and
* the input bodies.
*/

class DECL_BOOL slice_output_legacy
{
public:

	/**
	* Constructor to create an object of this class given a slice_output_handle.
	*/

	slice_output_legacy( slice_output_handle const *h );

	/**
	* Unhooks and returns the wire-body that represents the slice output. The ownership of the wire-body returned
	* is transferred to the caller. The object of this class doesn't retain any information about the slice output
	* once this function is called.
	*/

	BODY* unhook_wire();

	/**
	* Destructor.
	*/

	~slice_output_legacy();

private:

	// For internal use only!

	slice_output_handle const *_hs;
};



/** @} */

#endif // SLICEAPI_HXX

