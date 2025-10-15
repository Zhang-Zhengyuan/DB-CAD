/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef _spa_utf8_hxx_
#define _spa_utf8_hxx_

#include <stddef.h>
#include <stdlib.h>
#include "base.hxx"

/**
 * \defgroup ACISSTRINGS Strings
 * \ingroup ACISBASE
 *
 */
/**
 * @file spautf8.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISSTRINGS
 *
 * @{
 */

/**
	 * Converts a wide-character string to a UTF-8 string.
	 * <br><br>
	 * <b>Role:</b> This method should be used for converting a wide-character string to a UTF-8 (8-bit Unicode Transformation Format) string, which is a lossless, 8-bit, variable-length character encoding for Unicode.
	 * The variable length nature of the UTF-8 format means that the length of the resultant UTF-8 string has to be calculated.
	 * This method returns the size of the required buffer to hold the resultant UTF8 string if the output string is not passed in.
	 * The size which is returned includes space for the null terminator.
	 * If an output string is passed in, it is populated with the resultant UTF-8 string.
	 * <br><br>
	 * @param pwcsInput
	 * Input wide-character string
	 * @param pstrUTF8Output
	 * Ouput UTF-8 string
*/
DECL_BASE size_t getUTF8String (const SPAWCHAR_T* pwcsInput, char* pstrUTF8Output);

    /**
	 * Converts a UTF-8 string to a wide-character string.
	 * <br><br>
	 * <b>Role:</b> This method should be used for converting a UTF-8 (8-bit Unicode Transformation Format)  string to a wide-character string
	 * The variable length nature of the UTF-8 format means that the length of the resultant wide-character string has to be calculated.
	 * This method returns the size of the required buffer to hold the resultant wide-character string if the output string is not passed in.
	 * The size which is returned includes space for the null terminator.
	 * If an output string is passed in, it is populated with the resultant wide-character string.
	 * <br><br>
	 * @param pstrUTF8
	 * Input UTF-8 string
	 * @param pwcsWideString
	 * Ouput wide-character string
     */
DECL_BASE size_t getWideString (const char* pstrUTF8, int iStringLength, SPAWCHAR_T* pwcsWideString);

/** @} */

#endif // _spa_utf8_hxx_
