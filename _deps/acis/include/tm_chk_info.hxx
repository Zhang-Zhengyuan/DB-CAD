// $Id: tm_chk_info.hxx,v 1.4 2001/01/11 18:06:58 ywoo Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef TM_CHK_INFO_HEADER
#define TM_CHK_INFO_HEADER

// This file defines the objects returned by the tolerant modelling
// checking routines. A list of objects may be returned each
// representing an error that has been detected. The objects have been
// derived from the base error_info class, so that sys_errors can
// conveniently be signalled with extra information when this is
// appropriate. Note also the intermediate derivation from the
// error_list_info class, which means that instances of all these
// classes can be chained together via the next pointers.

class EDGE;
class COEDGE;
class check_status_list;

#include "logical.h"
#include "err_list.hxx"

#include "dcl_intr.h"
// ywoo 08Jan01: added the header file.
#include "dcl_kern.h"
// ywoo: end

extern DECL_KERN GLOBAL_CONST double SPAnull;


// This top-level one is purely a base class, no instances of this class
// should be made. It provides a number of "typically" useful fields,
// for recording an edge and coedge, and a SPAparameter on each. The edge
// and coedge pointers are initialised to NULL, and the doubles to the
// conventional value "null", so it is easy to see if any are set. When
// these errors are produced by calling functions that accept pure
// geometry as arguments, then the coedge_param (if set) will refer
// directly to those geometries. If produced by a function that took
// topology as arguments, then the coedge_param will be negated with
// respect to the edge if the coedge was reversed, thereby having the
// correct SPAparameter for the coedge itself.

// Many derived classes that represent actual errors will be happy with
// these fields, though some may want to provide some specific extras.

class DECL_INTR tm_chk_info : public error_list_info {
protected:			// (stop anyone making an instance of this class)
	tm_chk_info(
		EDGE *e = NULL,
		COEDGE *c = NULL,
		double e_param = SPAnull,
		double c_param = SPAnull
		);

public:
	virtual ~tm_chk_info();

	EDGE *edge;
	COEDGE *coedge;
	double edge_param, coedge_param;

	// These are just for debug or reporting errors. Each derived class
	// must implement its type_name member. The print method only needs
	// redefining when the derived class has extra parameters not
	// provided here in the base class that need to be output.

	virtual char const *type_name() const = 0;
	virtual void print( FILE * ) const;
};

// Utility function to print an entire list of tm_chk_infos.

void print_tm_chk_info_list(
					tm_chk_info *,
					FILE *,
					char const *leader = ""
					);


// Next all the real classes, instances of which can actually be made.
// First, we list them:

// Generic error due to bad input topology:
//
// tm_bad_topology: edge/coedges are not tolerant, or there are no
// coedges, or they have no geometry.
//     Extra info: edge and/or coedge that is bad.

// Errors associated only with the edge curve, irrespective of coedges:
//
// tedge_crv_non_G1: edge curve is not G1 in 3-space.
//     Extra info: the edge; edge SPAparameter of problem.
// tedge_bad_crv: edge curve failed generic curve checks.
//     Extra info: the edge; check_status_list from curve check.

// Errors associated only with coedges, unrelated to the edge curve:
//
// tcoedge_bs2_non_G1: bs2_curve is not G1 in SPAparameter-space.
//     Extra info: the coedge; coedge SPAparameter of problem.
// tcoedge_bs2_outside_sf: bs2_curve strays off surface.
//     Extra info: the coedge; coedge SPAparameter of problem.
// tcoedge_crv_non_G1: coedge 3D curve is not G1 in 3-space.
//     Extra info: the coedge; coedge SPAparameter of problem.
// tcoedge_bad_crv: coedge 3D curve failed generic curve checks.
//     Extra info: the coedge; check_status_list from 3d curve check.

// Errors coming about from a coedge curve in conjunction with the edge
// curve:
//
// tedge_tcoedge_ranges: could not compute common ranges of curves.
//     Extra info: edge and coedge; logical indicating start or end.
// tedge_tcoedge_bad_geom: edge and coedge geometries are
//        unsympathetic in some way. The curve directions may be
//        opposite in places, or one curve may have too high
//        curvature for the other to follow properly.
//     Extra info: edge and coedge; SPAparameter on each of problem.

// Errors involving the edge and edge tolerance (which obviously
// involves the coedges, but otherwise irrespective of them):
//
// tedge_local_self_int: edge has local self-intersection.
//     Extra info: the edge; edge SPAparameter of problem.
// tedge_remote_self_int: edge has remote self-intersection.
//     Extra info: the edge; 2 edge parameters of self-intersection.

// Errors involving the edge, edge tolerance and a specific coedge:
//
// tedge_tcoedge_bad_tol: tolerance calculation is incorrect.
//     Extra info: edge and coedge; SPAparameter on each of problem.

// Here are the actual declarations:


// First, the generic error for signalling bad input:

class DECL_INTR tm_bad_topology : public tm_chk_info {
public:
	tm_bad_topology( EDGE *e, COEDGE *c );
	~tm_bad_topology();
	char const *type_name() const;
	int type() const;
	LOCAL_PROC int id();
};


// Those associated only with the edge:

class DECL_INTR tedge_crv_non_G1 : public tm_chk_info {
public:
	tedge_crv_non_G1( EDGE *e, double e_param );
	~tedge_crv_non_G1();
	char const *type_name() const;
	int type() const;
	LOCAL_PROC int id();
};

class DECL_INTR tedge_bad_crv : public tm_chk_info {
public:
	// we take over ownership of the passed check_status_list
	tedge_bad_crv( EDGE *e, check_status_list *clist );
	~tedge_bad_crv();
	check_status_list *csl;
	char const *type_name() const;
	void print( FILE * ) const;
	int type() const;
	LOCAL_PROC int id();
};
		

// Next, those associated only with the coedge:

class DECL_INTR tcoedge_bs2_non_G1 : public tm_chk_info {
public:
	tcoedge_bs2_non_G1( COEDGE *c, double c_param );
	~tcoedge_bs2_non_G1();
	char const *type_name() const;
	int type() const;
	LOCAL_PROC int id();
};

class DECL_INTR tcoedge_bs2_outside_sf : public tm_chk_info {
public:
	tcoedge_bs2_outside_sf( COEDGE *c, double c_param );
	~tcoedge_bs2_outside_sf();
	char const *type_name() const;
	int type() const;
	LOCAL_PROC int id();
};

class DECL_INTR tcoedge_crv_non_G1 : public tm_chk_info {
public:
	tcoedge_crv_non_G1( COEDGE *c, double c_param );
	~tcoedge_crv_non_G1();
	char const *type_name() const;
	int type() const;
	LOCAL_PROC int id();
};

class DECL_INTR tcoedge_bad_crv : public tm_chk_info {
public:
	// we take over ownership of the passed check_status_list
	tcoedge_bad_crv( COEDGE *c, check_status_list *clist );
	~tcoedge_bad_crv();
	check_status_list *csl;
	char const *type_name() const;
	void print( FILE * ) const;
	int type() const;
	LOCAL_PROC int id();
};


// Now, those that involve a coedge in conjunction with an edge:

class DECL_INTR tedge_tcoedge_ranges : public tm_chk_info {
public:
	tedge_tcoedge_ranges( EDGE *e, COEDGE *c, logical at_start );
	~tedge_tcoedge_ranges();
	logical start;
	char const *type_name() const;
	int type() const;
	LOCAL_PROC int id();
};

class DECL_INTR tedge_tcoedge_bad_geom : public tm_chk_info {
public:
	tedge_tcoedge_bad_geom(
			  EDGE *e, COEDGE *c, double e_param, double c_param
			  );
	~tedge_tcoedge_bad_geom();
	char const *type_name() const;
	int type() const;
	LOCAL_PROC int id();
};


// Those involving the edge and tolerance, but not otherwise any
// coedges:

class DECL_INTR tedge_local_self_int : public tm_chk_info {
public:
	tedge_local_self_int( EDGE *e, double e_param );
	~tedge_local_self_int();
	char const *type_name() const;
	int type() const;
	LOCAL_PROC int id();
};

class DECL_INTR tedge_remote_self_int : public tm_chk_info {
public:
	tedge_remote_self_int(
				  EDGE *e, double e_param, double other_e_param
				  );
	~tedge_remote_self_int();
	double other_edge_param;
	char const *type_name() const;
	void print( FILE * ) const;
	int type() const;
	LOCAL_PROC int id();
};


// And finally, for tolerance errors:

class DECL_INTR tedge_tcoedge_bad_tol : public tm_chk_info {
public:
	tedge_tcoedge_bad_tol(
			  EDGE *e, COEDGE *c, double e_param, double c_param
			  );
	~tedge_tcoedge_bad_tol();
	char const *type_name() const;
	int type() const;
	LOCAL_PROC int id();
};


#endif
