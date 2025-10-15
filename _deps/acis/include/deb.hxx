/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

/*******************************************************************/
// dal  18Aug03 Minor change for compilation on aix.
/*******************************************************************/


//
//
// The debug module allows users to add and control debug output from
// their programs in two ways:
//
//    1. Interactive. This is the normal use - the programmer uses only
//       macros in the program, and controls the debug level at run-time. 
//
//    2. Programmable. The programmer can make calls directly to the
//       debug functions. This is more complicated to use and to control  
//       at run time, but can offer extra facilities. The programmable 
//       mode of use is not fully documented here; refer to the member
//       functions of the class DEB_ENV for details. 
//
// 
//    
//    MODULES
// 
// In either case, control of debug is done via MODULES. Typically, a
// module will be a set of related files from the users program,
// although each file could be a separate module, or the entire program
// could be a single module. Each module has a four letter name, in
// capitals. Each file must declare which module it belongs to, by
// including a declaration of the form:
// 
//    static char module[] = "MODX";
//
//
//
//    FUNCTIONS
//
// Any C++ function may contain debug statements. Each function that is
// to do so, should contain the macro DEB_enter as its first statement.
// This macro takes a single argument, which is a character string
// consisting of the function name. The function names are used to form
// a traceback of functions which is printed before each debug output 
// (they cannot be used to control debug). It is allowable to use DEB_enter
// even if a function has no debug output - this will cause the function
// name to be output in the traceback. Since the function name is only
// used for output, it is possible to use abbreviated names or symbols
// to represent the name. 
//
//
//
//    DEBUG OUTPUT
//
// To allow output of data, the DEB_out macro is used. This takes two
// arguments, an integer which indicates the level at which the data is
// to be output, and an expression which could validly follow the phrase
// "D3_cout << ", preceding the final ";". For example:
//
//    DEB_out( 2, "a = " << a );
// or
//    DEB_out( 4, "Singular case !!!\n\n  svec: " << svec << "\n" );
//
// The level should be a positive integer. 
// 
//
//
//    DEBUG CONTROL
// 
// Debug is controlled by certain strings that the debug functions recognise.
// The strings can be provided as arguments to a main program, or
// interactively in the interrupt handler. The following strings are
// recognised:
//
//    MODXn - module MODX wil be debugged at level n (all DEB_ out
//           statements, with levels of n or less, will be activated). 
//
//    ABCD-MODXn - module MODX will be debugged as above, but only when
//           called directly from module ABCD. 
//
//    ABCD-EFGH--MODXn - module MODX will be debugged when called indirectly
//           from module EFGH, which is called directly from module ABCD.
//    
//    ALLM is a general name that will match any module name. It can be
//           used in the following ways:
// 
//    ALLM0 to debug all modules at level zero - this may be useful
//           because it causes all function names to be included in the
//           function traceback (otherwise only functions in modules
//           that are 'on' are included).
//
//    MODX-ALLM1 to debug all modules called directly from MODX.
// 
// Note that if two or more strings cause a module to be debugged, then
// the level used is the highest from all the strings. Unlike Parasolid,
// later strings do not override earlier ones - for example, MODX4
// followed by MODX3, would debug MODX at level 4. 
//
//    MODXb - the handler will be called whenever a function in module MODX 
//           is entered. Note that this is slightly different from
//           Parasolid, which called the handler for every debug statement. 
//
//    ABCD-MODXb - conditional break. 
//
//    MODXr - reset module MODX. This will 'forget' all previous commands
//           of the form ...MODXn or ...MODXb. Note that a conditional
//           string cannot be reset (i.e. ABCD-MODXr is not valid). 
//
//    -<name> - switch debug output to a file "name". 
// 
//    +<name> - send debug output to the file "name' as well as stdout.
//   
// 
//
//    INTERRUPT HANDLER
//
// The interrupt handler can be entered either by ctrl\, or by a break
// command as described above. Once in the handler, the user can enter
// any of the commands described above, plus the following (in all
// cases, any shortened form of the command, including a single letter,
// is accepted): 
//
//    report - report on the current environment (i.e. state which
//                                                modules are 'on' )
//    traceback - print the current module traceback. 
// 
//    environment <name> - switch to a new environment. An environment
//           is  a set of controls which have been specified. The user
//           can choose to leave the current environment and start a new
//           one from scratch, or revert to one that was previously set up. 
//
//    continue - continue processing
//
//    quit - leave program
//
//    help - online help
//
//
//
//    INITIALISATION
//
// The macro DEB_ setup can be used to initialise the debug environments
// and set up the handler. This macro takes three arguments:
//
//    'name' - the name of the initial debug environment. 
//    'argc' - the number of strings following. 
//    'argv' - an array of character strings. 
//
// It is clear that the latter two arguments are of the form supplied to
// a main program, and in fact arguments supplied to main can be passed
// directly to the DEB_ setup macro. This will extract the arguments it
// recognises (removing them from the array of strings) and leave the
// rest. argc will typically be reduced by this macro. It is possible to
// supply 0 for the value of argc, if no strings are supplied (the value 
// 1 has the same effect).  
//
//
//
//	COUNTING
//
// The debug module can be used to count events over a complete program run, 
// or over a call to a function. The DEB_count macro defines an object of 
// class DEB_COUNT, which can be incremented by use of the DEB_increment 
// macro. When the destructor is called on DEB_COUNT it prints out its current 
// total. 
//
//
//
//    CONDITIONAL COMPILATION
//
// All debug code can be removed from the source before compilation by
// omitting the -DD3_DEBUG option to the CC compiler. 
//
//
//
//   SUMMARY
//
// To use the debug facilities:
//
//  1. Add DEB_setup( "init", argc, argv ) to the main program. 
//
//  2. Add static char module[] = "ABCD" to each file to be debugged. 
//
//  3. Call DEB_enter( "function name" ) at the start of each function.
//
//  4. Call DEB_out( 2, "data: " << a << " " << b << "\n" );  etc
//
//  5. a.out ABCD1 EFGH3 other arguments to run the program with debug
//
//  6. ctrl\ to enter the handler
//
//
//
//    TRAPS
//
//    1. Do not use the string DEB, even in comments - this is reserved
//       for the debug module. 
//
//    2. Do not set breaks in the debug module itself (DEBb) - it will 
//       cause recursion. 
//
//    3. When a macro needs a string, supply it in quotes (e.g. "func")
//       - strings are never copied, so using a variable to hold a
//       string, passing it to debug, then reusing the variable will 
//       change the string inside the debug module. 
//
//
//
// Note - the documentation above which relates to setting up handlers
// is out-of-date. DEB_setup no longer sets up the handler. Instead the
// user must add calls to signal himself. SR 13-May-1991.

// Support for longjumps - DEB_save_stack, DEB_restore_stack.
// Use DEB_save_stack before the call to setjmp, to save the current state 
// of the debug traceback stack. If a longjump occurs, use DEB_restore_stack
// to restore it.

#ifndef MSCDEBH
#define MSCDEBH


#include "dcl_kern.h"
#include "logical.h"

// These macros provide the 'basic' facilities that are sufficient 
// for most uses. 

#ifdef D3_DEBUG
#include "stream.hxx"
#include "deb_md.hxx"

#define DEB_module( name )	LOCAL_CONST char module[] = name;

#define DEB_setup( name, argc, argv )                 \
    DEB_ENV *DEB_env = ACIS_NEW DEB_ENV( name ); argc = DEB_env->setup( argc, argv );

#define DEB_enter( fn ) DEBUG_FN DEB( fn, module );		\
                        LOCAL_VAR int deb_enter_count = 0;	\
			deb_enter_count++

#define DEB_out( level, stream )		  		\
    if ( DEB.permission() >= level )                  		\
	{                                             		\
	DEB.call( deb_enter_count );                   		\
	DEB_current_env->os << stream;                		\
	if ( DEB_current_env->copout ) D3_cout << stream;	\
	}

#define DEB_info( stream )		       			\
    {                                             		\
    if ( DEB.permission() >= 0 )				\
	DEB.call( deb_enter_count );                            \
    DEB_current_env->os << "\nDebug information from " << 	\
	DEB.filename() << ":   " << stream;                	\
    if ( DEB_current_env->copout ) 				\
	D3_cout << "\nDebug information from " << 		\
	    DEB.filename() << ":   " << stream;			\
    }

#define DEB_save_stack() DEB_MD_LINK *DEB_stack_top = DEB__module_traceback

#define DEB_restore_stack() DEB__module_traceback = DEB_stack_top

#define DEB_filename()	DEB.filename()

#define DEB_count( name, message ) DEB_COUNT name( module, message );

#define DEB_increment( name )		name.increment();

// ACIS timers Build
#elif defined ENABLE_ACIS_TIMERS
#include "timer.hxx"

#define DEB_module( name )	

#define DEB_setup( name, argc, argv )

#define DEB_enter( fn ) acis_timer azx( fn );

#define DEB_out( level,stream )

#define DEB_info( stream )

#define DEB_save_stack()

#define DEB_restore_stack()

#define DEB_filename()	" "

#define DEB_count( name, message ) 

#define DEB_increment( name )

// Internal debug checks build
#elif defined INTERNAL_DEBUG_CHECKS
#include "errorbase.hxx"

#define DEB_module( name )	

#define DEB_setup( name, argc, argv )

#define DEB_enter( fn ) LOCAL_VAR int deb_enter_count = 0; STKLIM_ENTER 

#define DEB_out( level,stream ) { if(deb_enter_count){int a = 0;}}

#define DEB_info( stream )

#define DEB_save_stack()

#define DEB_restore_stack()

#define DEB_filename()	" "

#define DEB_count( name, message ) 

#define DEB_increment( name )

// Regular builds
#else

#define DEB_module( name )	

#define DEB_setup( name, argc, argv )

#define DEB_enter( fn )

#define DEB_out( level,stream )

#define DEB_info( stream )

#define DEB_save_stack()

#define DEB_restore_stack()

#define DEB_filename()	" "

#define DEB_count( name, message ) 

#define DEB_increment( name )

#endif

// Classes used only in D3_DEBUG

#ifdef D3_DEBUG

// This class is used to store data for the current debug function. An
// entity of the class lives as long as the function does. 

class DECL_KERN DEBUG_FN : public ACIS_OBJECT
    {
    int id;
    int level;
    DEB_MD_LINK module_traceback_head;    
    const char* name;
public:
    DEBUG_FN( const char*, const char* );
    ~DEBUG_FN();
    const char* filename()	{ return name; }
    int permission() 	{ return level; }
    void call( int count );
    };


// This class defines a debug environment. 

class DEB_CH_LINK;
class FN_LINK;
DECL_KERN void INT_handler(int );
DECL_KERN void DEB_change_environment(char* );
class DEB_ENV;
DECL_KERN D3_ostream& operator<<(D3_ostream&, DEB_ENV* );

class DECL_KERN DEB_ENV : public ACIS_OBJECT
    {
    friend class FN_LINK;
    friend class DEBUG_FN;

    DEB_CH_LINK* module_chains;
    FILE 	file;
    char* 	name;
    DEB_ENV* 	next;
    void	add( char*, char );
    void	remove();
    int 	current_id;
    int 	previous_id;
    FN_LINK*	last;
    friend DECL_KERN void INT_handler( int );
    friend DECL_KERN void DEB_change_environment( char* );
    friend DECL_KERN D3_ostream& operator<<( D3_ostream&, DEB_ENV* );

public:
    DEB_ENV( char* name );	// set up a new debug environment called 'name'
    ~DEB_ENV();
    D3_ostream 	os;             // debug output stream

    logical 	copout;         // set if debug output is to go to stdout 
    void control( char* ); 	// processes a debug control string of the form
				// MODXn, MODXb, ABCD-MODXn, ABCD-MODXb, etc.
    int level( logical* );  	// returns the level of the current module 
				// and a flag to say whether 'break' is on
    void open_file( char* );   	// switch debug to a named file
    void reset( char* );	// reset a module
    void select();		// make 'this' the current environment
    void report();		// report environment to stdout 
    int setup( int, char** );   // process a set of control strings of the 
				// form argc, argv given to main
    };


// This class counts events. 

class DECL_KERN DEB_COUNT : public ACIS_OBJECT
    {
    int 	_total;
    const char*	_message;
    const char*	_module;

public:
	DEB_COUNT( const char* module, const char* message )	
		: 	_total( 0 ), _module( module ),	_message( message ) {}
	~DEB_COUNT();
    
    void	increment()	{ _total++; }
    };

extern DECL_KERN DEB_ENV* DEB_current_env;

extern DECL_KERN void DEB_traceback();

extern DECL_KERN void DEB_change_environment( char* );

#endif
#endif
