/* ORIGINAL: acis18 */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef tolerance_param_hxx
#define tolerance_param_hxx
#include "mmgr.hxx"
#include "dcl_kern.h"

/**
 * Options object part containing tolerance information.
 * <br>
 * <b>Role:</b> 
 * Objects of this class hold a tolerance.  The user may set the tolerance
 * to anything they like: the get_tol method will return whatever the user set
 * the get_tol_internal method will return the larger of their value and
 * SPAresabs.
 *
 **/
class  DECL_KERN tolerance_param: public ACIS_OBJECT
{
private:
	/***
	* @nodoc
	**/
	double _tol;	
	logical use_default_tol;

public:
	tolerance_param();
	/***
	* set_tol called with no arguments returns the object to default behavior
	**/
	void set_tol();
	/***
	* set_tol sets the tolerance 
	* <br><br>
	* @param tolerance
	**/
	void set_tol(double tolerance);
	/***
	* get_tol retrieves the tolerance Do not use get_tol to modify
	* tolerance.
	**/
	double get_tol() const;
	/***
	*@nodoc
	**/
	// for internal use only
	// Contains logic for sorting out bad user provided values of tolerance
	// if tolerance provided is less than current SPAresabs, it returns SPAresabs.
	double get_tol_internal() const;
};
#endif 
