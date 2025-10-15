/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header defining data structures for checker options enums
/*******************************************************************/
#ifndef __CHK_OPTS_ENUMS_HXX__
#define __CHK_OPTS_ENUMS_HXX__
#include "insanity.err"

/**
*  Runtime difficulty level
*/
enum CHECK_RUNTIME_LEVEL_ENUM
{
    LEVEL_UNDEF = -1, 
    LEVEL00 = 0, 
    LEVEL05 = 5, 
    LEVEL10 = 10, 
    LEVEL20 = 20, 
    LEVEL30 = 30, 
    LEVEL40 = 40, 
    LEVEL50 = 50, 
    LEVEL60 = 60, 
    LEVEL70 = 70,
    LEVEL100 = 100
};

/**
* Global options
*/ 
enum CHECK_GLOBAL_OPTS_ENUM
{
    API_LOW_LEVEL_CHECKING,
    CHECK_ABORT,
    CHECK_AND_FIX,
    CHECK_DISCONT,
    CHECK_EDGE_GAPS,
    CHECK_EDGE_ON_FACE,
    CHECK_FF_INT,
    CHECK_LEVEL,
    CHECKER_LIMIT,
    CT_VALIDITY_CHECK,
    DEV_CHECKS,
    D3_CHECKS,
    D3_GEOM_CHECKS,
    D3_PCU_CHECKS,
    GET_AUX_MSG,
    INTEROP_INTERNAL1, 
    PERIODIC_NO_SEAM,
    QUICK_CHECK,
    R14_CHECKS,
    SHOW_WARNING_MSG,
    SHORT_EDGE_MAX_LENGTH,
    SLIVER_MAX_PERCENT,
    SPLIT_PERIODIC_SPLINES,
	IGNORE_LEVELS,
    CHECK_GLOBAL_OPTS_ENUM_END //!< Not in use
};

/**
*  Modes
*/
enum CHECK_MODES_ENUM
{
    INCLUDE_DEPENDANT_CHECKS, //!< Toggle: Run all additional checks a particular check depends on (ADVANCED). Default = TRUE
    SCAN_DOWN_ENTITY_CHECKS, //!< Toggle: Run checks on all the topological descendants. Default = FALSE
	STRICT_SELECTIVE_CHECKS, //!< Toggle: Runs strictly on selection mode. Do not honor levels or properties. Default = FALSE
    CHECK_MODES_ENUM_END //!< Not in use
};

/**
*  Tag cloud
*/
typedef int CHECK_SELECTION_ENUM;

#endif // __CHK_OPTS_ENUMS_HXX__ 
