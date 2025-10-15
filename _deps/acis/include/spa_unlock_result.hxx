/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef UNLOCK_RESULT_HEADER
#define UNLOCK_RESULT_HEADER

#include "dcl_base.h"
#include "mmgr.hxx"
#include "spa_unlock_state.h"


/**
 * @file spa_unlock_result.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISLICENSE
 *
 * @{
 */

namespace SL {
	class spa_unlock_result;
};

/**
* Validation process warning and error messages.
* @param SPA_UNLOCK_NO_MSG
* No error.
* @param 		SPA_UNLOCK_ERROR
* Third party component license validation has failed.
* @param SPA_UNLOCK_INVALID_KEY
* Validation of third party component license has failed.
* @param SPA_UNLOCK_KEY_EXPIRED
* License for third party components has expired.
* @param SPA_UNLOCK_KEY_WARN_EXPIRE
* License for third party components will expire soon.
* @param SPA_UNLOCK_INVALID_VERSION
* Software version invalid for third party component license.
* @param SPA_UNLOCK_NOT_REGISTERED
* Third party component has not been registered.
* @param SPA_UNLOCK_INVALID_REGISTRATION
* Third party component has not been registered properly. Please reregister.
* @param SPA_UNLOCK_REGISTRATION_EXPIRED
* Third party component registration has expired. Please reregister.
* @param SPA_UNLOCK_REGISTRATION_WARN_EXPIRE
* Third party component registration will expire soon. Please reregister.
* @param SPA_UNLOCK_KEY_REGISTRATION_MISMATCH
* Third party component has been registered using the wrong license.
**/
enum spa_unlock_errmsg
{
	SPA_UNLOCK_NO_MSG,						
	SPA_UNLOCK_ERROR,						
	SPA_UNLOCK_INVALID_KEY,				
	SPA_UNLOCK_KEY_EXPIRED,					
	SPA_UNLOCK_KEY_WARN_EXPIRE,				
	SPA_UNLOCK_INVALID_VERSION,				
	SPA_UNLOCK_NOT_REGISTERED,				
	SPA_UNLOCK_INVALID_REGISTRATION,		
	SPA_UNLOCK_REGISTRATION_EXPIRED,		
	SPA_UNLOCK_REGISTRATION_WARN_EXPIRE,	
	SPA_UNLOCK_KEY_REGISTRATION_MISMATCH
};



/**
 * Stores the outcome of the license validation process.
 * <br><br>
 * <b>Role:</b> This class stores the outcome of the call to the api <tt>spa_unlock_products</tt>.
 * The class can be queried to determine if the validation was successful.  If the products could
 * not be unlocked, queries are supplied to determine the cause of the failure.  It is also
 * possible to retrieve logged info to send to Spatial technical support for debugging.
 * <br>
 @see spa_unlock_products
 */
class DECL_BASE spa_unlock_result: public ACIS_OBJECT
{
public:
	/**
	 * C++ allocation constructor requests memory for this object and populates it with default values.
	*/
	spa_unlock_result();

	/**
	 * C++ allocation constructor requests memory for this object and populates it by copying
	 * the contents of the argument object.
	 * @param res
	 * res spa_unlock_result
	*/
	spa_unlock_result( const spa_unlock_result& res );

	/**
	* @nodoc
	*/
	spa_unlock_result( const SL::spa_unlock_result& internal_res );

	/**
	 * C++ assignment operator destroys the existing contents of the object and repopulates it
	 * by copying the contents of the argument object.
	 * @param res
	 * res spa_unlock_result
	*/
	spa_unlock_result operator=( const spa_unlock_result& res );

	/**
	 * C++ destructor frees the memory used by this object.
	*/
	~spa_unlock_result();

	/**
	* Gets the logged info for debugging purposes.
	* <br><br>
	* <b>Role:</b> If the state indicates an error occurred during validation, then the
	* the string returned by this method can be saved to a file for debugging purposes.
	* The file can then be sent to Spatial technical support for processing.
	*/
	const char* get_log_info();

	/**
	* Gets the state of the object.
	* <br><br>
	@see spa_unlock_state
	*/
	spa_unlock_state get_state() const;

	/**
	* Gets the error or warning message number.
	* <br><br>
	@see spa_unlock_errmsg
	*/
	spa_unlock_errmsg get_message_no() const;

	/**
	* Gets a text message associated with the error or warning message number.
	* <br><br>
	@see spa_unlock_errmsg
	*/
	const char* get_message_text() const;

	/**
	* Gets the number of days until expiration.
	* <br><br>
	* <b>Role:</b> If the reason that validation caused an error or warning is due
	* to the expiration date of a license, the number of days until expiration may
	* be retrieved using this method.  If the license has expired, the returned value
	* is negative.
	@see spa_unlock_errmsg
	*/
	int get_num_days() const;

	/**
	* Gets the number of products and list of the products unlocked.
	* <br><br>
	* <b>Role:</b> If the state indicates an error occurred during validation, then the
	* the number of products returned is 0. The class ownes the memory associcated with 
	* product_names. Caller should not call delete on it.	
	*/
	void get_unlocked_products(char** &product_names , int &num_products);

private:
	/**
	* @nodoc
	*/
	SL::spa_unlock_result * m_unlock_result;
};


/** @} */
#endif

