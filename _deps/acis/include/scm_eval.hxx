/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//----------------------------------------------------------------------
// purpose---
//   Declare procedures for evaluating scheme expressions from C++
//----------------------------------------------------------------------

#ifndef SCM_EVAL_H
#define SCM_EVAL_H

#include "scheme.hxx"
#include "parm_str.hxx"

/**
 * @file scm_eval.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup SCMAPI
 * \Declared at <scm_eval.hxx>
 * @{
 */

class ENTITY;

/**
 * Evaluates Scheme expressions given as a character string.
 * <br><br>
 * <b>Role:</b> Evaluates incomplete or multiple Scheme expressions. Use this
 * function when the expression to be evaluated may not be complete. It looks for
 * matching sets of parentheses and evaluates the expression when the parentheses
 * balance. A string that contains either a partial expression or multiple
 * expressions can be passed in.
 * <br><br>
 * If the command string contains more than one expression, the function evaluates
 * the first expression found, sets the <tt>scheme_result</tt> argument, and returns the 
 * part of the command that has not been evaluated yet. When there is a partial expression,
 * the <tt>nest_level</tt> argument sets to the number of right parentheses required to
 * balance the expression.
 * <br><br>
 * The function returns <tt>NULL</tt> when nothing remains in the command string to be
 * evaluated. It retains partial expressions and uses them as the start of an
 * expression the next time <tt>SchemeCommand</tt> is called.
 * <br><br>
 * The following example illustrates the use of <tt>SchemeCommand</tt>:
 * <pre>
 *    int nestlevel = 0;
 *    param_string result;
 *    const char* cmdstr = command_string;
 * 
 *    while(*cmdstr) {
 *        cmdstr = SchemeCommand(cmdstr, result, nestlevel);
 *        const char* result_string = result;
 *        if (nestlevel == 0 && *result_string) {
 *            printf("%s\n", result);
 *        }
 *    }
 * </pre>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param inpLine
 * Scheme expression.
 * @param scheme_result
 * returns result of first expression evaluation.
 * @param nest_level
 * returns level of parentheses nesting.
 */
const char* SchemeCommand(const char* inpLine, param_string& scheme_result, int& nest_level);

// These procedures return 0 if the evaluation was successful.  Otherwise
// they return an error code.

/**
 * Evaluates a string or Scheme object.
 * <br><br>
 * <b>Role:</b> This function is overloaded.
 * <br><br>
 * It evaluates a Scheme expression that is already a Scheme object. This
 * version of <tt>SchemeEvaluate</tt> accepts a Scheme expression that has already been
 * parsed into a Scheme object. Because it does not have to go through the Scheme
 * reader to convert a string into an object, it is faster to evaluate the same
 * expression many times. The expression to be evaluated must be protected from
 * garbage collection if the repeated evaluation is not done within the scope of a
 * single C++ procedure. The function returns 0 if the procedure successfully
 * evaluates; otherwise, it returns an exception code.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param expr
 * Scheme expression.
 * @param result
 * returns result.
 **/

int SchemeEvaluate(ScmObject expr, ScmObject&result);

/**
 * Evaluates a string or Scheme object.
 * <br><br>
 * <b>Role:</b> This function is overloaded.
 * <br><br>
 * It evaluates a Scheme expression for its result, returned as a Scheme
 * object. This version of <tt>SchemeEvaluate</tt> evaluates an expression that is given as
 * a character string, and returns the result as a Scheme object. This has the
 * benefit of not having to convert the result to a <tt>char*</tt> and back with the
 * resulting potential for loss of precision. The <tt>is_Scm_(type)</tt> and <tt>get_Scm_(type)</tt>
 * functions are called to check the result and convert it into a C++ object. The
 * function returns 0 if the procedure successfully evaluates; otherwise, it
 * returns an exception code.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param str
 * command string.
 * @param result
 * returns result.
 **/

int SchemeEvaluate(const char* str, ScmObject &result);

/**
 * Evaluates a string or Scheme object.
 * <br><br>
 * <b>Role:</b> This function is overloaded.
 * <br><br>
 * It evaluates an expression given by a character string, and returns the
 * result as a <tt>param_string</tt>. The <tt>param_string</tt> is cast to a <tt>char*</tt>.
 * The function returns 0 if the procedure successfully evaluates; otherwise, it returns an
 * exception code.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param str
 * command string.
 * @param result_string
 * returns result.
 **/

int SchemeEvaluate(const char* str, param_string &result_string);

/**
 * Evaluates a string or Scheme object.
 * <br><br>
 * <b>Role:</b> This function is overloaded.
 * <br><br>
 * It evaluates an expression given as a character string. This is useful
 * for evaluating the expression for its side effects. The function does not return
 * the result of evaluating the expression. The function returns 0 if the procedure
 * successfully evaluates; otherwise, it returns an exception code.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param str
 * command string.
 **/

int SchemeEvaluate(const char *str);

//----------------------------------------------------------------------
// The following procedure returns the last result (ScmObject)
// and status (int)

/*
// tbrv
*/
/**
 * @nodoc
 */
int SchemeResult(ScmObject&);

/**
 * Evaluates an object that is a Scheme procedure against a list of arguments.
 * <br><br>
 * <b>Role:</b> Evaluates a Scheme object that is a procedure with arguments.
 * Use this specialized evaluator when writing hook functions to be called from C++.
 * <br><br>
 * The first argument is a Scheme procedure; the second argument is a list of
 * Scheme objects containing the arguments to be passed to the procedure.
 * <br><br>
 * The function passes back a <tt>ScmObject</tt> containing the result of the operation. The
 * function returns 0 if the procedure is successful; otherwise, it returns an exception code.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param proc
 * Scheme procedure.
 * @param args
 * arguments to the procedure.
 * @param result
 * returns result of evaluation.
 */
int SchemeFuncall(ScmObject proc, ScmObject args, ScmObject& result);

/**
 * Loads a Scheme file into memory.
 * <br><br>
 * <b>Role:</b> The function returns 0 if the loaded procedure successfully
 * evaluates; otherwise, it returns an exception code.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param filename
 * file to load.
 **/

int SchemeLoad(const char* filename);

/**
 * Loads a Scheme file into memory via <tt>journal:load</tt>.
 * <br><br>
 * <b>Role:</b> The function returns 0 if the loaded procedure successfully
 * evaluates; otherwise, it returns an exception code.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param filename
 * file to load.
 **/
int SchemeLoad(const char* filename, logical journal);


/**
 * Converts a Scheme object to a string.
 * <br><br>
 * <b>Role:</b> Returns 0 if the operation is successful.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param obj
 * Scheme object.
 * @param result_string
 * returns string.
 */
int SchemeObjectToString(ScmObject obj, param_string& result_string);


/**
 * Reads a string into a Scheme object.
 * <br><br>
 * <b>Role:</b> Returns 0 if the operation is successful.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param str
 * input string.
 * @param result
 * returns Scheme object.
 */
int StringToSchemeObject(const char* str, ScmObject& result);

//----------------------------------------------------------------------
// The following procedures return TRUE if the expression was evaluated
// OK and returned an object of the right type.  Otherwise, they
// return FALSE


/**
 * Gets the value of a Scheme expression that returns an <tt>int</tt>.
 * <br><br>
 * <b>Role:</b> Gets the value of a specific data type returned by a Scheme
 * expression when the data type returned by the expression is already known. This
 * function return <tt>TRUE</tt> if the expression successfully evaluates and the result
 * returns; otherwise, it returns <tt>FALSE</tt>. <tt>SchemeEvaluate</tt> can also be used.
 * <br><br>
 * This function is overloaded.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param expression
 * string to evaluate.
 * @param value
 * returns integer value.
 */
logical SchemeGetValue(const char* expression, int& value);

/**
 * Gets the value of a Scheme expression that returns a <tt>double</tt>.
 * <br><br>
 * <b>Role:</b> Gets the value of a specific data type returned by a Scheme
 * expression when the data type returned by the expression is already known. This
 * function return <tt>TRUE</tt> if the expression successfully evaluates and the result
 * returns; otherwise, it returns <tt>FALSE</tt>. <tt>SchemeEvaluate</tt> can also be used.
 * <br><br>
 * This function is overloaded.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param expression
 * string to evaluate.
 * @param value
 * returns <tt>double</tt> value.
 */
logical SchemeGetValue(const char* expression, double& value);

/**
 * Gets the value of a Scheme expression that returns an <tt>ENTITY*</tt>.
 * <br><br>
 * <b>Role:</b> Gets the value of a specific data type returned by a Scheme
 * expression when the data type returned by the expression is already known. This
 * function return <tt>TRUE</tt> if the expression successfully evaluates and the result
 * returns; otherwise, it returns <tt>FALSE</tt>. <tt>SchemeEvaluate</tt> can also be used.
 * <br><br>
 * This function is overloaded.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param expression
 * string to evaluate.
 * @param value
 * returns <tt>ENTITY*</tt> value.
 */
logical SchemeGetValue(const char* expression, ENTITY*& value);

// The following procedures set the value of a Scheme variable to
// a given value.  They use DEFINE, so a new variable with the given
// name will be created if it doesn't already exist.


/**
 * Sets the value of a Scheme variable.
 * <br><br>
 * <b>Role:</b> Sets the value of a Scheme variable of a specific data type when
 * the data type of that variable is already known. If the variable does not yet
 * exist a new variable with the given name is created using <tt>define</tt>.
 * <br><br>
 * This function is overloaded.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param variable_name
 * name of variable to set.
 * @param value
 * value to set.
 */
logical SchemeSetVariable(const char* variable_name, int value);

/**
 * Sets the value of a Scheme variable.
 * <br><br>
 * <b>Role:</b> Sets the value of a Scheme variable of a specific data type when
 * the data type of that variable is already known. If the variable does not yet
 * exist a new variable with the given name is created using <tt>define</tt>.
 * <br><br>
 * This function is overloaded.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param variable_name
 * name of variable to set.
 * @param value
 * value to set.
 */
logical SchemeSetVariable(const char* variable_name, double value);

/**
 * Sets the value of a Scheme variable.
 * <br><br>
 * <b>Role:</b> Sets the value of a Scheme variable of a specific data type when
 * the data type of that variable is already known. If the variable does not yet
 * exist a new variable with the given name is created using <tt>define</tt>.
 * <br><br>
 * This function is overloaded.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param variable_name
 * name of variable to set.
 * @param value
 * value to set.
 */
logical SchemeSetVariable(const char* variable_name, ENTITY* value);

/**
 * Sets the value of a Scheme variable.
 * <br><br>
 * <b>Role:</b> Sets the value of a Scheme variable of a specific data type when
 * the data type of that variable is already known. If the variable does not yet
 * exist a new variable with the given name is created using <tt>define</tt>.
 * <br><br>
 * This function is overloaded.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * @param variable_name
 * name of variable to set.
 * @param value
 * value to set.
 */
logical SchemeSetVariable(const char* variable_name, const char* value);

/** @} */
#endif
