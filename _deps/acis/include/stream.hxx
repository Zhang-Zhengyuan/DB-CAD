/* ORIGINAL: acis2.1/kernutil/d3_misc/stream.h */
/* $Id: stream.hxx,v 1.8 2002/02/13 16:55:06 aclode Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//stream.h   

// Header for stream functions and operators. 
// These provide facilities externally similar to C++ streams but
// greatly simplified (ICB 17-1-90)

#if defined D3_STANDALONE || defined D3_DEBUG 

#ifndef STREAMH
#define STREAMH

#include <stdio.h>
#include "dcl_kern.h"
#include "logical.h"
#include "mmgr.hxx" // ACIS_OBJECT
#include "debugmsc.hxx" // for base D3 macros
#include "option.hxx"

extern DECL_KERN option_header d3_suppress_output;
class D3_ostream;
class D3_istream;
DECL_KERN D3_ostream& operator<<(D3_ostream&, const int );
DECL_KERN D3_ostream& operator<<(D3_ostream&, const long );
DECL_KERN D3_ostream& operator<<(D3_ostream&, const double );
DECL_KERN D3_ostream& operator<<(D3_ostream&, const void* );
DECL_KERN D3_ostream& operator<<(D3_ostream&, const char* );
DECL_KERN D3_ostream& operator<<(D3_ostream&, const char );
DECL_KERN D3_istream& operator>>(D3_istream&, int& );
DECL_KERN D3_istream& operator>>(D3_istream&, long& );
DECL_KERN D3_istream& operator>>(D3_istream&, double& );
DECL_KERN D3_istream& operator>>(D3_istream&, char* );
DECL_KERN D3_istream& operator>>(D3_istream&, char& );
DECL_KERN D3_istream& operator>>(D3_istream&, void*& );


class DECL_KERN D3_ostream  : public ACIS_OBJECT
    {
private:
    char*	tab;

	char* get_tab();
public:
    FILE* 	fp;
    logical 	opened;

    logical	open( char* );
    void	close();
    		D3_ostream( FILE* );
    		D3_ostream();
    		~D3_ostream();

    void	begin_indent();
    void	end_indent();

    friend 	DECL_KERN D3_ostream& 	operator<<( D3_ostream&, const int );
    friend 	DECL_KERN D3_ostream& 	operator<<( D3_ostream&, const long );
    friend 	DECL_KERN D3_ostream& 	operator<<( D3_ostream&, const double );
    friend 	DECL_KERN D3_ostream& 	operator<<( D3_ostream&, const void* );
    friend 	DECL_KERN D3_ostream& 	operator<<( D3_ostream&, const char* );
    friend 	DECL_KERN D3_ostream& 	operator<<( D3_ostream&, const char );
    };




class DECL_KERN D3_istream  : public ACIS_OBJECT
    {
    int		n_read;
    logical	skip_newline;
    logical	check_for_eol();
public:
    FILE* 	fp;			// File used by D3_istream
    logical 	opened;	

    logical 	open( char* );		// Open named file for D3_istream

    void 	close();		// Close associated file

    		D3_istream( FILE* );
    		D3_istream();
		~D3_istream();

    logical	good();			// Indicates that the previous 
					// input operation worked as expected

    char	get();			// Get next character (dont skip WS)

    void	putback( int );		// Move stream pointer back the
    					// given number of positions

    void	dont_skip_newline();	// For the next input operation only, 
					// don't treat a newline as whitespace

    long	mark();			// Return the current SPAposition in the
					// file
    void	rollback( long );	// Reposition the file pointer


    // The following operators skip leading whitespace

    friend	DECL_KERN D3_istream& 	operator>>( D3_istream&, int& );
    friend	DECL_KERN D3_istream& 	operator>>( D3_istream&, long& );
    friend	DECL_KERN D3_istream& 	operator>>( D3_istream&, double& );
    friend	DECL_KERN D3_istream& 	operator>>( D3_istream&, char* );
    friend	DECL_KERN D3_istream& 	operator>>( D3_istream&, char& );
    friend	DECL_KERN D3_istream& 	operator>>( D3_istream&, void*& );
    };



extern SESSION_GLOBAL_VAR DECL_KERN D3_istream D3_cin;
extern SESSION_GLOBAL_VAR DECL_KERN D3_ostream D3_cout;
extern SESSION_GLOBAL_VAR DECL_KERN D3_ostream D3_cerr;

// STI rr (12/15/99) begin: Declared allow_break here to get the DLLD version
// to compile

// STL apc 30 Jan 02 Extend allow_break so we can write listener classes to be
// informed when a break happens
enum break_type {
Warning_break = 10,
Assert_break = 20,
Fatal_break = 30,
Null_break = 100
};

DECL_KERN void allow_break(break_type type);


class DECL_KERN BreakListener
    {
private:
    friend DECL_KERN void allow_break(break_type type);
    BreakListener* _previous;
    BreakListener* _next;
protected:
    virtual void break_occurred(break_type type) = 0;
    BreakListener* next() { return _next; }
public:
    BreakListener();
    virtual ~BreakListener();
    };

// STI rr : end

#endif
#endif
