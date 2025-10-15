/*******************************************************************/
/*    Copyright (c) 2016-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef incr_bool_api_hxx
#define incr_bool_api_hxx

#include "api.hxx"
#include "dcl_hlc.h"
#include "entity.hxx"
#include <vector>

class bool_base_options;

/**
* \defgroup INCR Incremental Workflows
* Incremental workflows attempt to increase the robustness of their regular counterparts
* by iteratively preparing of the inputs based on the failure feedback and reattempting the operation.
* @{
* \defgroup INCR_BOOL Incremental Boolean Workflow
* Incremental workflow specific to Boolean operations
* @}
*/

/**
* @file incr_bool_api.hxx
* \addtogroup INCR
* \addtogroup INCR_BOOL
* @{
*/

/**
* The options class for the incremental Boolean workflow.
* @par Technical Article
* <i>[Incremental Boolean Workflow](http://doc.spatial.com/articles/i/n/c/Incremental_Boolean_Workflow_f6d8.html)</i>
* @par Role
* Currently, it serves as a placeholder until the need for specific options arises.
*/
class incr_bool_options;

/**
* The context handle class for the incremental Boolean workflow.
* @par Technical Article
* <i>[Incremental Boolean Workflow](http://doc.spatial.com/articles/i/n/c/Incremental_Boolean_Workflow_f6d8.html)</i>
* @par Role
* This class implements the handle that preserves the context for the entire 
* incremental Boolean workflow. It is created with @href incr_bool_handle::make before 
* starting the incremental workflow and updated via @href incr_bool_handle::update 
* with the @href outcome of the previous Boolean operation.
* A context handle is used by @href api_incr_bool_prepare to appropriately modify the tool and blank.
* The class is designed as an <i>abstract base class</i> that cannot be 
* instantiated, copied or assigned; it can only be deleted. This is done
* in order to hide the implementation. Internally the data is accessed through 
* public inheritance. The user takes ownership of instances.
*/
class DECL_HLC incr_bool_handle
	: public ACIS_OBJECT
{
protected:
	incr_bool_handle();
	incr_bool_handle(const incr_bool_handle&);
	incr_bool_handle& operator=(const incr_bool_handle&);

public:
	/**
	* The desctructor is public, indicating the user takes ownership of instances.
	*/
	virtual ~incr_bool_handle() = 0;
	/**
	 * Make a context handle for the incremental Boolean workflow.
	 * @param[in] tool The tool body for the Boolean operation.
	 * @param[in] blank The blank body for the Boolean operation.
	 * @param[in] bo A pointer to an instance of one of the derived variants of @href bool_base_options can be provided. If not nullptr, the internal settings of bo will be changed as needed and <tt>bo</tt> must be used for the Boolean operations in the current IBW context.
	 * @param[in] ibo Always NULL. For future use.
	 * @param[in] ao ACIS options such as versioning and journaling. For journaling to work properly the same AcisOptions object must be used by incr_bool_handle::make, @href api_incr_bool_prepare, and the Boolean operation.
	 */
	static incr_bool_handle* make(
		BODY* tool, 
		BODY* blank, 
		bool_base_options* bo = NULL,
		const incr_bool_options* ibo = NULL,
		const AcisOptions* ao = NULL);

	/**
	* The returned @href incr_bool_options are the ones that must be used for the Boolean operations in the current IBW context. They can be static-casted back to the original type.
	* @return Same @href incr_bool_options pointer as passed to @href incr_bool_handle::make is returned.
	*/
	virtual bool_base_options* get_bool_options() const = 0;

	/**
	 * Update the context handle for the incremental Boolean workflow 
	 * with the outcome returned by the previous Boolean operation. 
	 * The return value signifies whether to continue the incremental Boolean workflow or not.
	 * @param[in] res The @href outcome returned by the previous Boolean operation.
	 * @return True to continue the incremental Boolean workflow, otherwise false.
	 */
	virtual bool update(const outcome& res) = 0;

	/**
	* Resets all the context handles for the incremental Boolean workflow
	* with their original values.
	* @return None
	*/
	virtual void terminate() = 0;

	virtual  const  std::vector<SPAbox> & getProblematicRegions() const = 0;
};

/**
* The core API function of the incremental Boolean workflow.
* @par Technical Article
* <i>[Incremental Boolean Workflow](http://doc.spatial.com/articles/i/n/c/Incremental_Boolean_Workflow_f6d8.html)</i>
* @par Role
* This API function prepares the tool and blank bodies for a next iteration in the incremental Boolean workflow.
* Given a pointer to an @href incr_bool_handle object this API function prepares the tool and blank bodies 
* for the next iteration step by removing the complexitities identified by the previous Boolean operation. 
* @par Journal
* Available
* @par Errors
* <tt>INCR_FAILED</tt> \n
* <tt>INCR_NO_COMPLEXITIES</tt> \n
* <tt>INCR_NO_PROGRESS</tt> \n
* <tt>INCR_UNKNOWN_COMPLEXITY</tt>
* @par Effect
* Changes model
* @par Products
* 3D ACIS Modeler
* @param[in,out] ibh The context handle for the incremental Boolean workflow.
* @param[in] ao The ACIS option, used for versioning and journaling. For journaling to work properly the same AcisOptions object must be used by @href incr_bool_handle::make, <tt>api_incr_bool_prepare</tt>, and the Boolean operation.
* @return The @href outcome object indicating success or failure.
*/
DECL_HLC outcome api_incr_bool_prepare(
	incr_bool_handle* ibh,
	const AcisOptions* ao = NULL);

/** @} */

#endif // incr_bool_api_hxx
