// $Id: cmdstack.hxx,v 1.6 2002/01/30 17:32:55 skenny Exp $
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
//    define command stack interface for Macintosh stdio interface.
//
//----------------------------------------------------------------------

#ifdef mac

#ifndef cmdstack_hxx
#define cmdstack_hxx

#include "logical.h"

//----------------------------------------------------------------------
// constants
//----------------------------------------------------------------------

const int BOS = -1;				// bottom of stack
const int default_size = 50;	// default stack size

//----------------------------------------------------------------------
// data structures
//----------------------------------------------------------------------

typedef struct cmdBuffer{
	unsigned len;
	char* command;
} cmdBuffer;

//----------------------------------------------------------------------
// class declaration
//----------------------------------------------------------------------

class cmdstack: public ACIS_OBJECT
{
  protected:
	int top;
	int size;
	int peekPtr;
	cmdBuffer *stack;
	
 public:
	// constructor
	cmdstack(int cmdSize = default_size);

	// copy constructor
    cmdstack(cmdstack *oStack );

	// destructor
    ~cmdstack();

	// push a command onto the stack and set the peekPtr to the new top of stack
	void push(const char *);

	// pop a command from the stack and set the peekPtr to new top of stack
	// WARNING - the character string that is returned will be destroyed by the next push
	char *pop(void);

	// put the peek pointer back at top of stack
	void reset_peek(void)  { peekPtr = top; }

	// rotate a command to the top of stack
	logical rot(int);

	// peek at a command on the stack and set peekPtr to that command
	const char *peek(int);

	// return the previous command on the stack and decrement peekPtr
	char *previous(void);

	// return the next command on the stack and increment peekPtr
	char *next(void);

	// return true if the stack is empty, false if not
	logical is_empty(void)	{ return top == BOS; }

	// return true if the stack is full, false if not
	logical is_full(void)	{ return top == size - 1; }
};

//======================================================================
#endif // mac
#endif // cmdstack_hxx
