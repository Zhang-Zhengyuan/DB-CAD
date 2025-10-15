/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//               Local Operations Smoke Tests
/*******************************************************************/
#if !defined( LOP_SMOKE_TEST_HXX )
#define LOP_SMOKE_TEST_HXX
#include "logical.h"
#include "scheme.hxx"
#include "test_utl.hxx" 
class lop_options;
/*******************************************************************/
void lop_test_internal1( acis_test_unit &test,
							int argc, 
							ScmObject *argv, 
							lop_options* pLopts,
							logical AutoScale= 1, 
							logical dojournal=0
							) ;
void lop_test_internal2( acis_test_unit &test,
							int argc, 
							ScmObject *argv, 
							lop_options* pLopts,
							logical AutoScale= 1,
							logical dojournal=0
							) ;


#endif

