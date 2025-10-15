/* ORIGINAL: acis2.1/kernutil/timer/timer.hxx */
/* $Id: timer.hxx,v 1.15 2002/06/03 15:22:07 ktatters Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// System timing. Declare a class which records the time (CPU and
// elapsed) when it was initialised, and provides suitable access
// functions, and one for time intervals.

#if !defined( TIMER_CLASS )
#define TIMER_CLASS
#ifdef __cplusplus

#if defined( _MSC_VER )
#define FTIME
#endif

#include <string.h>

// STIPORT TAC macintosh MWCWG5 carries typedef for time_t in time.h
#if defined (mac) && !defined(MacX)
#include <time.h>
#else
#include <sys/types.h>		// for time_t
#ifdef FTIME
#include <sys/timeb.h>		// for timeb
#endif
#endif

//#include "baseutil/base.hxx"
#include "dcl_base.h"
#include "debugmsc.hxx"
// ywoo 08Jan01: The following header files needed for this header file 
// to be stand alone.
#include "mmgr.hxx"
// ywoo:end

#include <time.h> // for clock_t & time()

class abs_time;
class delta_time;


// Base class to contain a time. For historical reasons this can be
// initialised to the current time, and two can be subtracted to
// give an elapsed period. The derived classes which follow should
// be used in the future.

class DECL_BASE time_val : public ACIS_OBJECT {
protected:
	time_t cpu;			// cpu time in 1/HZ units
	int millisec;		// milli-seconds left over
	time_t elapsed;		// elapsed time in seconds

	time_val( time_t c, time_t e );
	time_val( time_t c, int m, time_t e );
public:
	time_val();			// initialise to current system time

	double cpu_secs() const;		// return CPU time in seconds
	double elapsed_secs() const;	// return elapsed time in seconds
	double milli_secs() const;		// return elapsed milli-seconds left over

	time_val operator-( time_val const & ) const;

	void debug( FILE * = debug_file_ptr ) const;
};

// Class representing an "absolute" time as supplied by the system.

class DECL_BASE abs_time: public time_val 
{
	abs_time( time_t c, time_t e ): time_val( c, e ) {}
	abs_time( time_t c, int m, time_t e ): time_val( c, m, e ) {}
public:
	abs_time() {}		// initialise to current system time
	abs_time( abs_time const &old ): time_val( old.cpu, old.millisec,
											   old.elapsed ) {}

	delta_time operator-( abs_time const & ) const;

	abs_time operator+( delta_time const & ) const;
	abs_time const &operator+=( delta_time const & );
	abs_time operator-( delta_time const & ) const;
	abs_time const &operator-=( delta_time const & );

	friend class delta_time;
};

// Class representing a time period (between two abs_times).

class DECL_BASE delta_time: public time_val {
	delta_time( time_t c, time_t e ): time_val( c, e ) {}
	delta_time( time_t c, int m, time_t e ): time_val( c, m, e ) {}
public:
	delta_time(): time_val( 0, 0 ) {}		// initialise to zero
	delta_time( delta_time const &old ): time_val( old.cpu, old.millisec, 
												   old.elapsed ) {}

	delta_time operator+( delta_time const & ) const;
	delta_time const &operator+=( delta_time const & );
	delta_time operator-( delta_time const & ) const;
	delta_time const &operator-=( delta_time const & );

	abs_time operator+( abs_time const & ) const;

	friend class abs_time;
};

DECL_BASE double get_cpu_seconds();

#endif /* __cplusplus */
#endif /* TIMER_CLASS */

