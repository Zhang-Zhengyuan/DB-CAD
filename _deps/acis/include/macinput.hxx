// $Id: macinput.hxx,v 1.4 2001/09/19 21:12:43 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// This entire file is only used by Macintosh systems
#ifdef mac

#ifndef mac_input_hxx
#define mac_input_hxx

#include "acinput.hxx"

class mac_input_cb : public input_callback
{
public:
	mac_input_cb(FILE *in_fp) : input_callback(in_fp) {}

	virtual int do_getc();
	virtual int do_ungetc(int);
};

#endif
#endif

