/* $Id: tim.hxx,v 1.16 2002/01/30 17:29:08 skenny Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef TIMH
#define TIMH

#ifdef D3_TIMES

//             THE D-CUBED PERFORMANCE TIMING MECHANISM
//
//
// INTRODUCTION
//
// This is available by compiling all the sources with D3_TIMES defined.
// You do NOT need to define D3_STANDALONE, D3_DEBUG or anything else.
// There is no extra baggage involved in using D3_TIMES, for example,
// you do not pick up any D3 stream or debug code.
//
// The system is based on the internal clock of the computer, so there's
// no point trying to analyse things which are typically much less than
// a few clock ticks. So in practice this probably means "several
// hundredths of a second".
//
// The system will record time spent within each instrumented function
// or block of code and output the time when it exits that function or
// block of code. It will also maintain a cumulative total across all
// the calls to that code which it will output when the image exits, or
// when test harness "time" command is invoked (or the Scheme function
// d3:time is invoked). When the "time" command is issued, output is
// only produced for those functions which have been called since the
// last "time" command, and the cumulative time recorded is also that
// since the last "time" command.
//
// This system is obviously designed not to impact performance too much
// in its own right, but each timing call does typically involve a couple
// of heap allocations. However, so long as only "expensive" functions are
// instrumented the information it provides should be useful and quite
// accurate.
//
//
// SIMPLE USAGE
//
// The system can be used very simply, just like the old D-Cubed timing
// system of which it is an extension. When using the macros it is NOT
// necessary to wrap them in an "#ifdef D3_TIMES". No special action is
// necessary for recursive functions, they are handled correctly.
//
// Functions or blocks of code to be timed may be instrumented with one
// of:
//
// TIM_init( "function_name" );
//
// which causes a timing statement for that single call to be issued
// upon exit, and a cumulative total to be maintained which is outputted
// at image exit or invocation of the test harness "time" command (or
// the Scheme function d3:time invoked).
//
// TIM_init_no_acc( "function_name" );
//
// a timing statement is outputted when this function or block of code
// is exited. No running total is accumulated, however.
//
// TIM_init_output( "function_name", 0 );
//
// here NO timing statement is issued when this call is exited, however,
// a cumulative total IS maintained as usual. (Replacing the 0 with a 1
// is identical to the basic "TIM_init").
//
// The only real difference from the old system is that "TIM_out()" is
// not necessary, as output happens when the TIM_init goes out of scope.
// TIM_out() may be used to cause the output to happen early. Every
// timer will only issue one timing statement, successive calls to
// TIM_out() have no effect.
//
//
// RUNTIME CONTROL OF THE TIMING MECHANISM
//
// The option "d3_times" can be used to control the behaviour of the
// mechanism at runtime. If set to 0, the timing mechanism is silenced
// completely. If set to 1, only the summary statistics are output,
// through use of the test harness time command, the Scheme function
// d3:time, or at image exit. If set to 2, the timings for every
// instrumented call are output as they happen.
//
// The default setting of the option is 0, unless other D-Cubed defines
// are included (D3_DEBUG or D3_STANDALONE) when the default is 2.
//
//
// MAKING TIMERS AVAILABLE TO OTHERS
//
// If the algorithm you are instrumenting is relatively low level, and
// lots of other algorithms use it, it may be helpful to make the timers
// publicly available, so that people can measure how long they are
// spending in *your* algorithm on behalf of *their* algorithm. For
// example, you might wish to know how much time SSI is spending in CSI
// as distinct from how much time you are spending in CSI as a whole
// during a boolean operation.
//
// In this case you will have to make a "handle" for your timer. The
// "handle" - an instance of the TIMER_HANDLE class - is the actual
// thing that you export to other algorithms by making it a global
// variable. This is perhaps not ideal, but there aren't too many ways
// of getting hold of it elsewhere without potentially significant
// performance overheads - hardly what we want! So you might add to your
// file (and here you need the ifdef) something like:
//
// #ifdef D3_TIMES
// TIMER_HANDLE my_function_TIMER_HANDLE;
// #endif
//
// then, instead of using the TIM_init macro, use the following:
//
// TIM_init_handle( "my_function", &my_function_TIMER_HANDLE );
//
// and it should all be business as usual. You will need to place a
// declaration of your handle into a header file somewhere to enable 
// others easy access, look at intersct/kernint/timers.hxx for an
// example. For an example of this being done you could look in
// intersct/kernint/d3_ssi/ssi_intr.cxx . How someone else would use
// your timer is covered later.
//
//
// MORE ADVANCED USAGE - EXPLICIT USE OF TIMERS
//
// You can explicitly make your own timers - instances of the
// TIMING_DATA class - and use them in your code. You will need to
// "#ifdef D3_TIMES" such code appropriately. For example, you might
// like to set a timer running and at various intervals obtain the
// elapsed time from it, maybe resetting it occasionally as you go:
//
// TIMING_DATA timer( "func", 0 );	// now it's running
// ...do somthing...
// double elapsed_time = timer.elapsed_seconds();
// // You could call timer.stop() here - it's optional
// timer.start();	// resets the timer to zero
// ...do some more...
// double more_elapsed_time = timer.elapsed_seconds();
// etc.
//
// Note the initial "0" passed to the timer constructor, which indicates
// that no timing statement is to be output when the "stop()" function
// is called, or the timer goes out of scope.
//
// RULE 1: TIMING_DATA objects should have local or function scope.
//
// Because a TIMING_DATA object always has local scope, it cannot
// recognise recursion. So to do this correctly we have a "handle" class
// - the TIMER_HANDLE - which is a global or static variable which is
// hung onto the timer itself (this is what TIM_init_no_acc does):
//
// static TIMER_HANDLE handle;
// TIMING_DATA timer( "func", 1, &handle );
// ...now recursion will be recognised correctly...
// 
// RULE 2: the TIMER_HANDLE object should always have static or global
// scope.
//
// If you know that code is non-recursive then there's no need to
// provide a "handle", otherwise best policy would probably be always to
// supply one (as the TIM macros do). If you forget, the consequences
// are not dire - timing statements for recursive calls will be output
// no different from top-level ones, so you won't be able to tell them
// apart.
//
// Note, however, that the examples above making explicit TIMING_DATAs
// do NOT accumulate any times. For that we need to know about
// "buckets".
//
//
// MORE ADVANCED USAGE - BUCKETS
//
// To accumulate times from a timer, we have to attach a "bucket" (an
// instance of the TIMER_BUCKET class) to it, so that the timer can put
// its results in. Again, because the TIMING_DATA only ever has local
// scope we actually attach buckets to the timer's handle.
//
// A bucket outputs its accumulated times when it is destroyed. In usual
// circumstances this means:
//
// RULE 3: TIMER_BUCKETs should have static or global scope.
//
// (Actually, if you know what you are doing you may wish to control
// their scope explicity - for example, you might wish to accumulate
// times within one function for calls to another, and always output an
// accumulated time when the first function exits.)
//
// The same bucket can be added to several different timers. This means
// you can, for example, create a "miscellaneous" bucket for function
// calls you are not interested in seeing separately. To do this, you
// would create a bucket with, say, global scope (or file scope if all
// the references will be in the same file):
//
// TIMER_BUCKET misc_bucket( "miscellaneous_functions" );
//
// and then you could instrument the start of each function with:
//
// TIM_init_bucket_output( "func", &misc_bucket, 0 );
//
// This will accumulate all the times in the same bucket, which will be
// output when the image quits (or the "time" command used), but will
// issue NO per-call timing statements (that's what the "0" means). Note
// that there's no problem if your functions call one another, the
// bucket will not count any time spent more than once.
// 
//
// EVEN MORE ADVANCED - ADDING EXTRA BUCKETS
//
// In the most simple circumstance - and this is what TIM_init does - we
// make a bucket along with the timer, and hang the bucket on the timer.
// Below is more-or-less what the TIM_init macro does:
//
// static TIMER_BUCKET bucket( "bucket" );	// the name the bucket prints
// static TIMER_HANDLE handle;
// TIMER_ADD_BUCKET add_bucket( &handle, &bucket );	// hang bucket on timer
// TIMING_DATA timer( "func", 1, &handle );
// ...now off we go and do something...
//
// Buckets are attached to timers by the TIMER_ADD_BUCKET class. This is
// actually creating an object with local scope which performs the job.
// The reason for doing it this way is that the bucket gets unattached
// when the TIMER_ADD_BUCKET goes out of scope.
//
// RULE 4: TIMER_ADD_BUCKET should always have local scope. Also, you
// should make it BEFORE you make the TIMING_DATA itself, or the results
// will be incorrect.
//
// More usefully, here's how we attach an additional bucket to someone
// else's timer. Suppose we have either declared, or included a header 
// file with this declaration:
//
// extern TIMER_HANDLE someone_else_TIMER_HANDLE;
//
// then, within an "#ifdef D3_TIMES" we could write:
//
// static TIMER_BUCKET my_bucket( "my_calls_to_someone_else" );
// TIMER_ADD_BUCKET my_add_bucket( &someone_else_TIMER_HANDLE, &my_bucket );
// ...stuff which calls "someone_else"...
//
// and we will get, in addition to anything else, a cumulative total
// time for all the calls to "someone_else" when called from within this
// block of code.
//
// Any number of buckets can be attached to pretty much any number of
// timers. Buckets are sufficiently smart that time will never be
// counted twice (or more). For example, if you rig up a bucket to
// accumulate time spent in CSI and SSI, time spent in CSI on behalf of
// SSI will not be counted twice. It is only the first timer that is set
// running with a particular bucket attached that will be allowed to add
// its time in at the end.
//
// For an example, look in blend/kernbool/capping/capping.cxx .
//
//
// FINAL WORDS AND CAUTIONS
//
// As usual there's plenty of rope here to hang yourself. But if used as
// intended it seems to work well, and if programmed carefully into your
// code can yield useful statistics for performance analysis just by
// executing the program.
//
// If anyone has any requests for fixes, modifications, enhancements etc.
// please submit them!
//
// DAP 23-Oct-97.


//VPL 18June2001: moved the implementation classes out of D3_TIMES macro definition.

// The basic macros for "simple use". We can add more if the need
// arises.

// Automatically define a timer and bucket to go togther. The timer
// issues timing statements if output is non-zero. The bucket will
// output a cumulative time at image exit, or when
// TIM_output_timer_buckets is invoked (e.g. by the test harness "time"
// command.

#define TIM_init_output( fn, output ) \
LOCAL_VAR TIMER_BUCKET _BUCKET( fn ); \
LOCAL_VAR TIMER_HANDLE _HANDLE; \
TIMER_ADD_BUCKET _ADD_BUCKET( &_HANDLE, &_BUCKET ); \
TIMING_DATA _DATA( fn, output, &_HANDLE )

// Automatically define a timer and bucket, both of which output in the
// normal manner.

#define TIM_init( fn ) TIM_init_output( fn, 1 )

// Automatically define a timer but which does no accumulation.

#define TIM_init_no_acc( fn ) \
LOCAL_VAR TIMER_HANDLE _HANDLE; \
TIMING_DATA _DATA( fn, 1, &_HANDLE )

// Automatically define a bucket and timer to accumulate times for the
// given handle. You would use this if you have defined the handle
// externally so that other algorithms can reference it.

#define TIM_init_handle_output( fn, handle_ptr, output ) \
LOCAL_VAR TIMER_BUCKET _BUCKET( fn ); \
TIMER_ADD_BUCKET _ADD_BUCKET( handle_ptr, &_BUCKET ); \
TIMING_DATA _DATA( fn, output, handle_ptr )

// Automatically define a timer which uses the given handle which
// outputs in the normal manner.

#define TIM_init_handle( fn, handle_ptr ) \
TIM_init_handle_output( fn, handle_ptr, 1 )

// Automatically define a timer to put its results in this bucket. You
// would use this if you want several different functions all to
// accumulate a combined total. The timer will issue timing statements
// per-call if output is non-zero.

#define TIM_init_bucket_output( fn, bucket_ptr, output ) \
LOCAL_VAR TIMER_HANDLE _HANDLE; \
TIMER_ADD_BUCKET _ADD_BUCKET( &_HANDLE, bucket_ptr ); \
TIMING_DATA _DATA( fn, output, &_HANDLE )

// Automatically create a timer to dump its results in the given bucket,
// and *do* produce timing statements for each call.

#define TIM_init_bucket( fn, ptr ) TIM_init_bucket_output( fn, ptr, 1 )

#define TIM_out( ) { _DATA.stop(); }


#else //D3_TIMES

#define TIM_init( fn )

#define TIM_init_output( fn, output )

#define TIM_init_no_acc( fn )

#define TIM_init_handle( fn, handle_ptr )

#define TIM_init_handle_output( fn, handle_ptr, output )

#define TIM_init_bucket( fn, bucket_ptr )

#define TIM_init_bucket_output( fn, bucket_ptr, output )

#define TIM_out( )

#endif //D3_TIMES

//VPL 18June2001: moved the implementation classes out of D3_TIMES macro definition.


#include <stdlib.h>

#include <string.h>
#include "dcl_kern.h"
#include "base.hxx" // to make sure we have ACIS_OBJECT base class
#include "debugmsc.hxx" // for D3 macros
#ifndef _WINDOWS

// STIPORT TAC macintosh doesn't have these headers
#if !defined ( mac ) || defined (MacX)
#include <sys/types.h>
#include <sys/times.h>
#ifndef MacX
#include <malloc.h>
#endif
#else
#include <extras.h> // for strdup
#endif

#else
#include <time.h>
#endif 

// Now the implementation of the classes themselves.

// First we need to be able to create arbitrary lists of buckets, as
// pretty much any bucket can be attached to any timer.

class TIMER_BUCKET;

class DECL_KERN TIMER_BUCKET_LIST : public ACIS_OBJECT
    {
public:
    TIMER_BUCKET_LIST ( TIMER_BUCKET *b, TIMER_BUCKET_LIST *n = NULL )
	: bucket( b ), next( n ) {}
    TIMER_BUCKET *bucket;
    TIMER_BUCKET_LIST *next;
    };

// This class maintains accumulated times for all calls to any timers
// (TIMING_DATAs) to which it has been attached. We also maintain a
// global list of all the existing buckets so that
// TIM_output_timer_buckets() can zip through them all and print the
// statistics.

class DECL_KERN TIMER_BUCKET : public ACIS_OBJECT
    {
    // The name which will be printed ahead of "accumulated times".

    char *_name;

    // User time and system time accumulate here.

    double _user_time, _sys_time;

    // Number of calls to the timer which have been made. If this is 0
    // we won't bother outputting anything.

    int _ncalls;

    // The number of running timers which want to put their time into
    // this bucket will be kept here. That's because only the first
    // of those timers started will actually be allowed to do so, to
    // stop us counting double (or worse). This gets incremented each
    // time a timer with an interest in this bucket starts, and
    // decremented when it stops.

    int _active;

    // To main the global list. Doubly-linked so that we can easily
    // extract ourselves when we are destroyed.

    TIMER_BUCKET *_prev, *_next;

public:

    // Constructor, given the name.

    TIMER_BUCKET( char* n );

    ~TIMER_BUCKET();

    // Next in the global list of all existing buckets.

    TIMER_BUCKET *next() const { return _next; }

    char *name() { return _name; }

//VPL 19June2001: added functions to access user time & system time.
	double user_time() const { return _user_time; }
	double system_time() const { return _sys_time; }

    // Print the accumulated time.

    void output();

    friend class TIMING_DATA;
    };

// The "handle" class. Handles are made for TIMING_DATAs, and any
// buckets can then be hung on the handle. The handle also allows
// recursion to be handled correctly as it should not have local scope.

class DECL_KERN TIMER_HANDLE : public ACIS_OBJECT
    {
    // The depth of any recursive call to a timer.

    int _depth;

    // The list of buckets the timer is interested in.

    TIMER_BUCKET_LIST *_buckets;

public:
    
    TIMER_HANDLE() : _depth( 0 ), _buckets( NULL ) {}

    ~TIMER_HANDLE() {}

    friend class TIMER_ADD_BUCKET;
    friend class TIMING_DATA;
    };

// This is the TIMING_DATA class which represents a "timer". When it is
// constructed it starts running. It may be explicitly stopped, and even
// re-started. When it is stopped it looks for any attached buckets into
// which it will dump its times so as to accumulate running totals.

class DECL_KERN TIMING_DATA : public ACIS_OBJECT
    {
    // The name to be printed ahead of "timings".

    char* _name;

    // Whether the timer is running.

    int _running;

    // Whether timing statements are to be issued by this timer.

    int _output;

    // The moment at which we started.

#ifndef  _WINDOWS
#if defined ( mac ) && !defined (MacX)
	unsigned long initial_time;
#else
    struct tms initial_times;
#endif
#else
    clock_t initial_time;
#endif

    // The "handle". This is the static or global data structure where
    // buckets actually get attached. Being static/global, it also
    // allows us to handle recursion properly.

    TIMER_HANDLE *_handle;

public:

    // Constructor, given the name, whether we want this timer to
    // generate per-call timings or not, and an associated handle.

    TIMING_DATA( char* n, int oput = 1, TIMER_HANDLE *h = NULL );

    ~TIMING_DATA();

    char *name() { return _name; }

    // Change the timer's name. Not usually needed, except for
    // marginally nefarious purposes.

    void set_name( char const *new_name );

    // Re-start the timer. If not running (i.e. already been stopped) we
    // are restarted. If already running the time is re-zeroed, so any
    // time between the original start and re-start is "lost".

    void start();

    // Stop the timer. Also outputs a timing statement if output is
    // requested.

    void stop();

    // Return the number of elapsed seconds since the timer was started.

    double elapsed_seconds();
    };


// A class of which we make an instance in order to tell the given timer
// that it should be accumulating its time in the given bucket. We do
// this with a class object (instead of an obvious function call) so as
// to undo the effect automatically when we go out of scope.

class DECL_KERN TIMER_ADD_BUCKET : public ACIS_OBJECT
    {
    // The timer (referenced via its handle) to which we have added a bucket.

    TIMER_HANDLE *_handle;

public:

    // Add bucket to timer (actually its handle). A timer can have any
    // number of buckets. Even if you accidentally repeat some it
    // shouldn't matter.
	//VPL 19June2001: added level SPAparameter. this controls whether or not the bucket will be added to timer,
	//based on the current setting of the d3_times option. default value of -1 will always add the timer.

    TIMER_ADD_BUCKET( TIMER_HANDLE *handle, TIMER_BUCKET *bucket, int iLevel=-1 );

    // Destructor which automatically unravels what we did.

    ~TIMER_ADD_BUCKET();
    };


// Function to output and reset all the timing buckets in existence.
// This is what the test harness "time" command calls, though in
// principle anyone else could too.
//this function is implemented only in the D3_TIMES version.

void DECL_KERN TIM_output_timer_buckets();

//VPL 19June2001: new function for timing data output to file, in customizable format.
//Function to ouput all the timing data from current bucket list. the data will be
//written out into a file (with specified name). the format for the data is also taken as
//argument. this format controls which buckets will be accessed (filter like behaviour).

void DECL_KERN TIM_log_data(const char* filename, const char* format);

#endif
