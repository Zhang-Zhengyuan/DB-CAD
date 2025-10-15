/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef PORTING_HXX
#define PORTING_HXX

#include <stdio.h>

#ifdef _MSC_VER

#define FSEEK64 _fseeki64	// int _fseeki64( FILE *stream, __int64_t offset, int origin );
#define FTELL64 _ftelli64	// __int64_t _ftelli64( FILE *stream );
#define UNLINK _unlink

#else 
							// off_t is 8-bytes
#define FSEEK64 fseeko		// int fseeko( FILE *stream, off_t offset, int origin );
#define FTELL64 ftello		// off_t ftello( FILE *stream );
#define UNLINK unlink

#endif

#endif
