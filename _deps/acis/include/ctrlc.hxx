/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#include "errorbase.hxx"
#if !defined( CTRLC_HXX )
#define CTRLC_HXX
/**
 * @file ctrlc.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISCALLBACKS
 *
 * @{
 */

/**
 * Defined callback function type for <tt>set_acis_interrupt_checker</tt>.
 */
typedef void (*ACIS_Interrupt_Checker)(const char*);

/**
 * Installs an interrupt checker that can be used to interrupt ACIS.
 * <br><br>
 * <b>Role:</b> Installing an interrupt checker with <tt>set_acis_interrupt_checker</tt>  
 * allows the application to gracefully interrupt the current ACIS operation 
 * and regain control of the process. This is a useful feature when applications 
 * want to be responsive to end-user input, such as responding to Control-C input 
 * during lengthy operations. This function allows one to install a callback function
 * that is frequently called from various places within ACIS code so that 
 * application side events can be evaluated. The callback function receives 
 * an informative string, which describes the current function in ACIS, 
 * as a passed in argument. The application can simply return from the callback, 
 * which allows the operation to continue; call the <tt>interrupt_acis</tt> function, 
 * which will interrupt the process in a synchronous manner; or
 * call the <tt>sys_error</tt> function with the <tt>SIGINT_FAULT</tt> argument, 
 * which will interrupt the operation asynchronously (immediately).
 * <br><br>
 * @param interrupt_checker
 * Customer callback function.
 **/
DECL_BASE ACIS_Interrupt_Checker set_acis_interrupt_checker(ACIS_Interrupt_Checker interrupt_checker );

/**
 * @nodoc
 * A class to save and restore the acis interrupt checker using an
 * automatic variable. This makes it easy to temporarily override it
 * or turn it off during critical operations. 
 */
class save_acis_interrupt_checker : public ACIS_OBJECT {
	ACIS_Interrupt_Checker checker;
public:
	save_acis_interrupt_checker(ACIS_Interrupt_Checker c) {
		checker = set_acis_interrupt_checker(c);
	}
	~save_acis_interrupt_checker() { 
		set_acis_interrupt_checker(checker);
	}
};

/** @} */
#endif // CTRLC_HXX
