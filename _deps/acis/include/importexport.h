/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef IMPORTEXPORT_H
#define IMPORTEXPORT_H

#if defined(_MSC_VER)
#define IMPORT_EXPORT_SYMBOLS
#define EXPORT_SYMBOL __declspec(dllexport)
#define IMPORT_SYMBOL __declspec(dllimport)
#elif defined(_LINUX_SOURCE)
#define IMPORT_EXPORT_SYMBOLS
#define EXPORT_SYMBOL __attribute__((visibility("default")))
#define IMPORT_SYMBOL __attribute__((visibility("default")))
#else
#define IMPORT_EXPORT_SYMBOLS
#define EXPORT_SYMBOL
#define IMPORT_SYMBOL
#endif

#endif


