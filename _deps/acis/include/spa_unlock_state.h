/*******************************************************************/
/*    Copyright (c) 2006-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef SPA_UNLOCK_STATE_H
#define SPA_UNLOCK_STATE_H
/**
 * \defgroup ACISLICENSE License
 * \ingroup ACISBASE
 * \brief Licensing
 */
/**
 * @file SL_spa_unlock_state.h
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISLICENSE
 * \brief Declared at <SL_spa_unlock_state.h>
 * @{
 */

/**
 * Possible outcomes of the validation process
 */
enum spa_unlock_state
{
	SPA_UNLOCK_PASS,
	SPA_UNLOCK_PASS_WARN,
	SPA_UNLOCK_FAIL
};

/** @} */
#endif
