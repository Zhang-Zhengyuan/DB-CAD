/* ORIGINAL: acis2.1/kerndata/savres/dispatch.hxx */
// $Id: dispatch.hxx,v 1.12 2002/08/09 17:15:19 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined(DISPATCH_HXX)
#define DISPATCH_HXX

// Define macros for restore dispatch tables
//
// DISPATCH_DECL is used in the .hxx file for the base class to declare the
// classes and functions needed to restore derived classes using a dispatch
// table.
//
// DISPATCH_DEF(, errno) is used in the .cxx file for the base class to
// define the functions and static variables needed to implement the dispatch
// table and restore functions. The argument errno is the error number to be
// reported if an unknown type identifier is read.
//
// The value of the macro THIS() (usually the name of the base class) is used
// by both of these macros to generate unique names.
//
// These macros define a class THIS()_restore_def which is used to add entries
// to the dispatch table. Its constructor takes a unique string to
// identify the subtype and pointers to functions for restoring the subtype.
//
// The macros also define operator>> and THIS()_restore functions which read
// an ID from the file and call the appropriate function to restore the
// indicated subtype.
//
// The class writer is responsible for writing operator<< and save functions
// which write the correct ID before writing the class data.
//
// In thing.hxx:
//
//#include "kerndata/savres/dispatch.hxx"
//
//class thing {
// ...
//public:
//	virtual void save() = 0;
//};
//
//#define THIS() thing
//DISPATCH_DECL
//
// In thing.cxx:
//
// ...
//DISPATCH_DEF(UNDEFINED_THING)
//
// In subthing.hxx:
//
//class subthing : public thing {
// ...
//public:
//	virtual void save();
//};
//
// In subthing.cxx:
//
//#define subthing_name "subthing"
//
//static thing *restore_subthing();
//
//static thing_restore_def(subthing_name, restore_subthing);
//
//void
//subthing::save()
//{
//	write_id(subthing_name);
// ...
//}
//
//thing *
//restore_subthing()
//{
//	subthing *ptr = new subthing();
// ...
//	return ptr;
//}

#include "acis.hxx"

// Set up macros for generating unique variable/function names

#ifdef THIS_FUNCTION
#undef THIS_FUNCTION
#endif
#ifdef DECL_FUNCTION
#undef DECL_FUNCTION
#endif

// STIPORT shd - make sure the following define is the same as entity.hxx
#if defined(__STDC__) || defined(hp700) || defined(sgi) || defined(mac) || defined (__ICL)
#if !defined(CONCAT)
#define CONCAT2(a,b) a## b
#define CONCAT(a,b) CONCAT2(a,b)
#endif

#define THIS_FUNCTION(name) CONCAT(THIS(), name)
#define DECL_FUNCTION(lib)  CONCAT(DECL_, lib)
#else
#if defined(sgi4)
#define THIS_FUNCTION(name) name2(THIS(), name)
#define DECL_FUNCTION(lib)  name2(DECL_, lib)
#else
#define THIS_FUNCTION(name) THIS()name
#define decl_() DECL_
#define DECL_FUNCTION(lib)  decl_()lib
#endif
#endif

#define MAX_ID 256

#define DISPATCH_DECL(lib) \
typedef THIS() *(*THIS_FUNCTION(_restore_func))(); \
\
class DECL_FUNCTION(lib) THIS_FUNCTION(_restore_def) { \
    char const *name; \
    THIS_FUNCTION(_restore_func) rest_fn; \
    THIS_FUNCTION(_restore_def) *next; \
public: \
    THIS_FUNCTION(_restore_def)( \
                char const *, \
                THIS_FUNCTION(_restore_func), \
				int & \
            ); \
    ~THIS_FUNCTION(_restore_def)(); \
\
    friend DECL_FUNCTION(lib) THIS() *THIS_FUNCTION(_restore)(); \
}; \
DECL_FUNCTION(lib) THIS() *THIS_FUNCTION(_restore)();

#define DISPATCH_DEF(errno) \
static THIS_FUNCTION(_restore_def) *THIS_FUNCTION(_restore_head); \
\
THIS_FUNCTION(_restore_def)::THIS_FUNCTION(_restore_def) ( \
	char const *nm, \
	THIS_FUNCTION(_restore_func) rfn, \
	int &id \
	) \
{ \
	LOCAL_VAR int next_id = 0; \
	next = THIS_FUNCTION(_restore_head); \
	name = nm; \
	rest_fn = rfn; \
	id = next_id++; \
	THIS_FUNCTION(_restore_head) = this; \
} \
\
THIS_FUNCTION(_restore_def)::~THIS_FUNCTION(_restore_def) () \
{ \
	THIS_FUNCTION(_restore_def) *this_def = THIS_FUNCTION(_restore_head); \
	THIS_FUNCTION(_restore_def) **prev_def = &THIS_FUNCTION(_restore_head); \
	while (this_def != NULL) { \
		if (this_def == this) { \
			*prev_def = this_def->next; \
			break; \
		} \
		prev_def = &this_def->next; \
		this_def = this_def->next; \
	} \
} \
\
THIS() * \
THIS_FUNCTION(_restore)() \
{ \
	char name[MAX_ID]; \
	read_id(name, MAX_ID); \
\
	for (THIS_FUNCTION(_restore_def) *this_def = THIS_FUNCTION(_restore_head); \
		 this_def != NULL; \
		 this_def = this_def->next) { \
		if (strcmp(name, this_def->name) == 0) \
			return (*this_def->rest_fn)(); \
	} \
\
	sys_error(errno); \
	return NULL; \
}
  
#endif
