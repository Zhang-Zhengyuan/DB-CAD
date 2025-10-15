// $Id: acis_options_internal.hxx,v 1.4 2002/01/28 16:29:45 rocon Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( ACIS_OPTIONS_INTERNAL )
#define ACIS_OPTIONS_INTERNAL

#include <stddef.h>
#include "logical.h"
#include "base.hxx"
#include "dcl_base.h"
#include "dcl_kern.h"
#include "acis_journal.hxx"
#include "vers.hxx"

class DECL_KERN AcisOptionsInternal : public ACIS_OBJECT
{
#ifdef linux
	// Jeff 11.07.03 (bug 73232)
	// suppress gcc warning: class AcisOptionsInternal only defines a private destructor and has no friends
	// This is suppodedly fixed in the latest gcc therefore REVISIT
	friend class suppress_gcc_compiler_warning;
#endif

public:
	AcisOptionsInternal();
	AcisOptionsInternal(AcisJournal&);
	AcisOptionsInternal(AcisJournal &journal, AcisVersion const &version);
	AcisOptionsInternal(AcisVersion const &version);

	void		      set_version(AcisVersion const &version);
	AcisVersion&      get_version();

	void              set_journal(AcisJournal &journalInfo);
	AcisJournal&      get_journal();

	void			  add();
	void			  remove();

private:
	virtual ~AcisOptionsInternal();
	AcisVersion m_acis_version;
	AcisJournal m_acis_journal;
	int         m_use_count;
};
#endif

