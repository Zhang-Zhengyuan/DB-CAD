/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header file for system options.
// First there is an option-definition class which records a
// logical value, which denotes whether the relevant option is
// on or off.
// The class has a constructor which links an instance of it into a
// global chain which can then be inspected for reporting or
// changing the value.
// Presently the option chain is set up at initialisation.  New
// options can easily be added by making further static declarations
// of this class.
#ifndef OPTION_HXX
#define OPTION_HXX
#include "dcl_base.h"
#include "logical.h"
#include "mmgr.hxx"
#include "safe.hxx"
/**
 * \defgroup ACISOPTIONS Options
 * \ingroup ACISAPIBEHAVIOR
 *
 */
/**
 * @file option.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISOPTIONS
 *
 * @{
 */

/**
 * When creating an <tt>option_header</tt> object, it implicitly becomes one of these types.
 * @see option_header
 */
enum option_type {
	logical_option,
	int_option,
	double_option,
	string_option,
	unknown_option
};
/**
 * @nodoc
 * Class for stacking option values, for well-behaved test harness
 * scripts.
 * The value is a union of the possible types - as usual, since
 * "logical" is in fact an integer, we only have three options.
 */
union opt_value_t 
{
	int int_value;
	double double_value;
	char *string_value;
};
/**
 * @nodoc
 */
class DECL_BASE option_value
{
public:
	option_value *next;

	logical default_option_value;
	option_type opt_type;

	opt_value_t opt_value;

	option_value( int, option_value * = NULL, logical default_init = FALSE );
	option_value( char const * gme, int, option_value * = NULL, logical default_init = FALSE );
	option_value( double, option_value * = NULL, logical default_init = FALSE );
	option_value( char const * gme, double, option_value * = NULL, logical default_init = FALSE );
	option_value( char const *, option_value * = NULL, logical default_init = FALSE );
	option_value( char const * gme, char const *, option_value * = NULL, logical default_init = FALSE );

	~option_value();

	logical on() const { return opt_type == int_option || opt_type == logical_option ?
							(opt_value.int_value != 0) : FALSE; }
	int count() const { return opt_type == int_option || opt_type == logical_option ?
							opt_value.int_value : 0; }
	double value() const { return opt_type == double_option ?
							opt_value.double_value : 0.0; }
	char const *string() const { return opt_type == string_option ?
							opt_value.string_value : NULL; }

	// Data printing routine
	void display( FILE * ) const;
	void gme_display( FILE * ) const;

	MMGR_SUPPORT_THIS
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
};
/**
 * Records a value that denotes whether the option is <i>on</i>, <i>off</i>, or set to a given value.
 * <br>
 * <b>Role:</b> The class has a constructor that links an instance of it into a global
 * chain that can then be inspected for reporting or changing the value. It maintains
 * the default option value, as well as a stack of option values. Presently, the option
 * chain is set up at initialization. New options can easily be added by making further
 * static declarations of this class.
 */
class option_header 
{
	/**
	 * pointer for linking option chain
	 */
	option_header *next_ptr;	
	/**
	 * The name
	 */
	const char *option_name;
	/**
	 * The value
	 */
	option_value def_value;

#ifdef THREAD_SAFE_ACIS
	safe_object_pointer<option_value> value_stack;
#else
	option_value *value_stack;
#endif

	void enter_in_list( char const * );
	void gme_enter_in_list(char const *);
public:

	/**
	 * C++ initialize constructor requests memory for this object and  populates it with the data supplied as arguments.
	 * <br><br>
	 * <b>Role:</b> Creates an <tt>option_header</tt> with the specified name and
	 * initial integer value and links it into the option list.
	 * For tidiness, we link options in alphabetic order.
	 * <br><br>
	 * @param name
	 * name.
	 * @param init
	 * initial value.
	 */
	DECL_BASE option_header( char const * name, int init);
	DECL_BASE option_header(char const *gme, char const *name, int init);

	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * <b>Role:</b> Creates an <tt>option_header</tt> with the specified name and
	 * initial double value and links it into the option list.
	 * For tidiness, we link options in alphabetic order.
	 * <br><br>
	 * @param name
	 * name.
	 * @param init
	 * initial value.
	 */
	DECL_BASE option_header( char const * name, double init);
	DECL_BASE option_header(char const *gme, char const *name, double init);

	/**
	 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
	 * <br><br>
	 * <b>Role:</b> Creates an <tt>option_header</tt> with the specified name and
	 * initial string value and links it into the option list.
	 * For tidiness, we link options in alphabetic order.
	 * <br><br>
	 * @param name
	 * name.
	 * @param init
	 * initial string.
	 */
	DECL_BASE option_header( char const * name, char const * init);
	DECL_BASE option_header(char const *gme, char const *name, char const *init);

	/**
	 * C++ destructor, deleting an <tt>option_header</tt>.
	 * <br><br>
	 * <b>Role:</b> This destructor returns the option to free storage,
	 * cleans up stacked values, and removes the header from the list.
	 */
	DECL_BASE  ~option_header();

	/**
	 * @nodoc
	 */
	MMGR_SUPPORT_THIS
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
	/**
	 * Returns the next <tt>option_header</tt>.
	 */
	option_header *next() const { return next_ptr; }

	/**
	 * Returns the name of the <tt>option_header</tt>.
	 */
	char const *name() const { return option_name; }

	/**
	 * Returns the type of <tt>option_header</tt>.
	 * <br><br>
	 * <b>Role:</b> The option types are <tt>logical_option</tt>, <tt>int_option</tt>, <tt>double_option</tt>,
	 * <tt>string_option</tt>, or <tt>unknown_option</tt>.
	 */
	option_type type() const {
		return value_stack == NULL ? unknown_option : value_stack->opt_type;
	}
	/**
	 * Returns whether the <tt>option_header</tt> is on or off if the option type is <tt>logical_option</tt>.
	 */
	logical on() const {
		return value_stack == NULL ? FALSE : value_stack->on();
	}
	/**
	 * Returns the value of the <tt>option_header</tt> if the option type is <tt>int_option</tt> or <tt>logical_option</tt>.
	 */
	int count() const {
		return value_stack == NULL ? 0 : value_stack->count();
	}

	/**
	 * Returns the value of the <tt>option_header</tt> if the option type is <tt>double_option</tt>.
	 */
	double value() const {
		return value_stack == NULL ? 0.0 : value_stack->value();
	}
	/**
	 * Returns the string value of the <tt>option_header</tt> if the option type is <tt>string_option</tt>.
	 */
	char const *string() const {
		return value_stack == NULL ? NULL : value_stack->string();
	}
	/**
	 * Sets the value of the <tt>option_header</tt> if the option type is an <tt>integer_option</tt> type or <tt>logical_option</tt> type.
	 * <br><br>
	 * @param count
	 * count.
	 */
	DECL_BASE void set( int count);
	DECL_BASE void gme_set(int count);

	/**
	 * Sets the value of the <tt>option_header</tt> if the option type is a <tt>double_option</tt> type.
	 * <br><br>
	 * @param value
	 * double value.
	 */
	DECL_BASE void set( double value);
	DECL_BASE void gme_set(double value);

	/**
	 * Sets the value of the <tt>option_header</tt> if the type is <tt>string_option</tt>.
	 * <br><br>
	 * @param opt
	 * option name.
	 */
	DECL_BASE void set( char const * opt);
	DECL_BASE void gme_set(char const *opt);

	/**
	 * @nodoc
	 */
	DECL_BASE void set( option_value const & optval);
	DECL_BASE void gme_set( option_value const & optval);

	/**
	 * Resets the option to its default value, leaving the stack intact.
	 */
	DECL_BASE void set_to_default();
	DECL_BASE void gme_set_to_default();

	// Data stacking routines
	/**
	 * Pushes a new integral value onto the value stack.
	 * <br><br>
	 * @param val
	 * integer value.
	 */
	DECL_BASE void push( int val);
	DECL_BASE void gme_push(int val);

	/**
	 * Pushes a new double value onto the value stack.
	 * <br><br>
	 * @param val
	 * double value.
	 */
	DECL_BASE void push( double val);
	DECL_BASE void gme_push(double val);

	/**
	 * Pushes a new string value onto the value stack.
	 * <br><br>
	 * @param newval
	 * new value.
	 */
	DECL_BASE void push( char const * newval);
	DECL_BASE void gme_push(char const *newval);

	/**
	 * @nodoc
	 */
	DECL_BASE void push( option_value const & optval);
	DECL_BASE void gme_push( option_value const & optval);

	/**
	 * Pops the stack to the previous value.
	 */
	DECL_BASE void pop();
	DECL_BASE void gme_pop();

	/**
	 * Resets the <tt>option_header</tt> to the default value.
	 */
	DECL_BASE void reset();
	DECL_BASE void gme_reset();

	/**
	 * Prints the data contained in the <tt>option_header</tt> to the specified file.
	 * <br><br>
	 * @param fp
	 * file name.
	 */
	void display( FILE *fp ) const { value_stack->display( fp ); }

	/**
	 * Checks the current value against the default value.
	 * <br><br>
	 * <b>Role:</b> Returns <tt>TRUE</tt> if they are same or <tt>FALSE</tt> otherwise.
	 */
	DECL_BASE logical is_default();
	DECL_BASE logical gme_is_default();
};
/**
 * Returns pointer to the head of the option list.
 * <br><br>
 * <b>Effect:</b> Read-only
 */
DECL_BASE option_header *get_option_list();
DECL_BASE option_header *gme_get_option_list();
/**
 * Gets the given option in the list.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @param name
 * option name.
 **/
DECL_BASE option_header *find_option( char const * name );
DECL_BASE option_header *gme_find_option( char const * name );
/**
 * @nodoc
 */
DECL_BASE void disable_option_callback();
/**
 * @nodoc
 */
DECL_BASE void enable_option_callback();

/** @} */
#endif

