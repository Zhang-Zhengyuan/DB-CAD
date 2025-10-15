/* ORIGINAL: acis2.1/spline/sg_bs3c/bs3cseg.hxx */
/* $Id: bs3cseg.hxx,v 1.7 2001/01/11 18:08:36 ywoo Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (BS3_CURVE_OFFSET_SEG_CLASS)
#define BS3_CURVE_OFFSET_SEG_CLASS

#include "dcl_spl.h"
// ywoo 08Jan01: added the header file.
#include "mmgr.hxx"
#include "bs3curve.hxx"
// ywoo: end

// THIS CLASS IS TEMPORARY. DO NOT DOCUMENT.
// It has been introduced to support STRATA 
// for the 2.0 release. It will be removed 
// in future releases.


class DECL_SPLINE bs3_offset_seg : public ACIS_OBJECT
{
  public:

	// offset spline
    bs3_curve bs;

	// SPAparameter range of definition
	double t0;
	double t1;

	// curve type ( >0 forward, <0 reversed)
	int type;

	// next in the linked list  ( == NULL => end)
    bs3_offset_seg* next;


	// constructor
    bs3_offset_seg(bs3_curve bs,
				   double t0,
				   double t1,
				   int    type,
                   bs3_offset_seg *list = NULL);
};

// this function will delete a list of offset curve objects
DECL_SPLINE void delete_offset_list(bs3_offset_seg*);

#endif
