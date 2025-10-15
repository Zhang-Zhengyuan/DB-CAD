// $Id: vers_cb.hxx,v 1.6 2000/12/26 18:48:32 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef __vers_cb_hxx
#define __vers_cb_hxx

#include "acis.hxx"
#include "dcl_kern.h"

#include "logical.h"
#include "tlkit_cb.hxx"

class DECL_KERN version_callback : public toolkit_callback
{
	friend class version_callback_list;

protected:
	virtual ~version_callback();

public:
	virtual char const *get_date_stamp() const;
	virtual char const *get_version_qualifier() const;
	virtual char const * const *get_version_specs() const;
};

class DECL_KERN version_callback_list : public toolkit_callback_list
{
public:
	void add(version_callback *cb) { toolkit_callback_list::add(cb); }
	void append(version_callback *cb) { toolkit_callback_list::append(cb); }

	virtual char const *get_date_stamp() const;
	virtual char const *get_version_qualifier() const;
	virtual char const * const *get_version_specs() const;
};

DECL_KERN void add_acis_version_cb(version_callback *cb);
DECL_KERN void remove_acis_version_cb(version_callback *cb);

#endif
