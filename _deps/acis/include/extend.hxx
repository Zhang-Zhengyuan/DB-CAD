/* ORIGINAL: acis2.1/kerngeom/curve/extend.hxx */
// $Id: extend.hxx,v 1.12 2000/12/26 18:47:13 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Indent style: D3

// Modifications
//
// 11-Dec-02 bwc Removed flags added on 11-Nov-02.
// 11-Nov-02 bwc Added extend_in_u flags that specify whether surface1 and
//		surface2 should be extended in the u direction (for blend surfaces).

#if !defined extension_info_CLASS
#define extension_info_CLASS

#include "acis.hxx"
#include "dcl_kern.h"
#include "logical.h"
#include "discinfo.hxx"

// This class returns information about a curve or surface extension. 

class DECL_KERN extension_info  : public ACIS_OBJECT
    {
    discontinuity_info	_discontinuities;

public:
    logical hit_low_limit;
    logical hit_high_limit;

    extension_info() {}


    // Access function to the discontinuity_info. 

    discontinuity_info&	discontinuities() { return _discontinuities; }


    // Access function, returning a read-only array of
    // discontinuities of the given order only. 

    const double*	discontinuities( int& n_discont, int order ) const
	{ return _discontinuities.discontinuities( n_discont, order ); }


    // Access function to cached data, returning a read-only array of
    // discontinuities up to and including the given order. 

    const double*	all_discontinuities( int& n_discont, int order )
	{ return _discontinuities.all_discontinuities( n_discont, order ); }


    // State whether a particular SPAparameter value is a discontinuity, and if so,
    // the order of the discontinuity. 

    int		discontinuous_at( double t ) const
	{ return _discontinuities.discontinuous_at( t ); }
	

    // Add a discontinuity value to the list. 

    void	add_discontinuity( double value, int order )
	{ _discontinuities.add_discontinuity( value, order ); }


    // Remove a discontinuity value from the list. 

    void	remove_discontinuity( double value )
	{ _discontinuities.remove_discontinuity( value ); }
};

#endif
