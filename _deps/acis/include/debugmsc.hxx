/* ORIGINAL: acis2.1/kernutil/debug/debugmsc.hxx */
/* $Id: debugmsc.hxx,v 1.5 2001/07/19 16:44:27 ywoo Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Simple i/o routines used mainly in debug code.

#if !defined( DEBUG_MISC )
#define DEBUG_MISC

#include <stdio.h>
#include "dcl_base.h"
#include "option.hxx"

DECL_BASE void acis_assert( const char* check_str, int check_val, const char* check_file, int check_line);

extern DECL_BASE option_header idc;
#if defined(INTERNAL_DEBUG_CHECKS) 
# define IDC_ASSERT(_CHECK_) acis_assert(#_CHECK_,!!(_CHECK_),__FILE__,__LINE__)
#else
# define IDC_ASSERT(_CHECK_)
#endif

#ifdef INTERNAL_DEBUG_CHECKS
#define IDC_BEGIN if( idc.on() ) {
#else
#define IDC_BEGIN if(0) {
#endif
#define IDC_END }

extern DECL_BASE option_header d3_debug;

//#ifdef INTERNL_DEBUG_CHECKS
#ifdef D3_DEBUG
#define D3_DEBUG_BEGIN if( d3_debug.on() ) {
#define NOT_D3_DEBUG_BEGIN if( ! d3_debug.on() ) {
#define D3_DEBUG_END }
#define NOT_D3_DEBUG_END }
#else
#define NOT_D3_DEBUG_BEGIN
#define NOT_D3_DEBUG_END
#endif



/**
* @file debugmsc.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISDEBUG
 *
 * @{
 */
/**
 * Pointer to default debug file.
 */
extern DECL_BASE FILE *debug_file_ptr;
/**
 * Prints a double representing a signed distance.
 * <br><br>
 * <b>Role:</b> The distance  is considered to be zero if its magnitude is less
 * than <tt>SPAresabs</tt>.
 * <br><br>
 * @param d
 * value to print.
 * @param fp
 * file pointer.
 */
DECL_BASE void debug_dist( double d, FILE *fp = debug_file_ptr );

/**
 * Prints a real representing a normalized, dimensionless quantity.
 * <br><br>
 * <b>Role:</b> The quantity is considered to be zero if its magnitude is less than
 * <tt>SPAresnor</tt>.
 * <br><br>
 * @param d
 * double to print.
 * @param fp
 * file pointer.
 */
DECL_BASE void debug_norm( double d, FILE *fp = debug_file_ptr );

/**
 * Prints a real number (double) with appropriate precision.
 * <br><br>
 * @param d
 * double to print.
 * @param fp
 * file pointer.
 */
DECL_BASE void debug_real( double d, FILE *fp = debug_file_ptr );

/**
 * 以合适精度打印实数到文件中
 * @param d 待打印实数
 * @param fp 文件指针
 */
DECL_BASE void gme_debug_real( double d, FILE *fp = debug_file_ptr );

/**
 * Prints a real number (double) with appropriate precision to a string.
 * <br><br>
 * @param d
 * double to print.
 * @param str
 * character pointer (string).
 */
DECL_BASE void debug_real_str( double d, char *str);

/**
 * 以合适精度将实数打印至字符串中
 * @param d 待打印的实数
 * @param str 字符串指针
 */
DECL_BASE void gme_debug_real_str( double d, char *str);

/**
 * Prints a pointer.
 * <br><br>
 * <b>Role:</b> By default, prints the pointer as a relative address. If the option
 * <tt>debug_absolute_addresses</tt> is on, an absolute address is used.
 * <br><br>
 * @param pv
 * pointer to print.
 * @param fp
 * file pointer.
 */
DECL_BASE void debug_pointer( void const *pv, FILE *fp = debug_file_ptr );

/**
 * Prints a pointer as a string.
 * <br><br>
 * <b>Role:</b> By default, prints the pointer as a relative address.
 * If the option <tt>debug_absolute_addresses</tt> is on, an absolute address is used.
 * <br><br>
 * @param pv
 * pointer to print.
 * @param str
 * character pointer (string).
 */
DECL_BASE void debug_pointer_str( void const *pv, char *str, logical use_abs_opt = FALSE );

/**
* Scans a pointer from a string.
* <br><br>
* <b>Role:</b> By default, prints the pointer as a relative address.
* If the option <tt>debug_absolute_addresses</tt> is on, an absolute address is used.
* <br><br>
* @param str
* character pointer (string).
* @param pv
* pointer to scan.
* @param use_abs_opt
* whether to use 'debug_absolute_addresses' option.
*/
DECL_BASE void debug_str_to_pointer( char const *str, void ** pv );

/**
 * Resets the internal timer for timing operations using <tt>debug_time</tt>.
 * <br>
 * @see debug_time
 */
DECL_BASE void debug_time_init();

/**
 * @brief 重置为gme_debug_time()函数提供服务的内部计时器
 * @see gme_debug_time
 */
DECL_BASE void gme_debug_time_init();

/**
 * Displays elapsed time since the last call to <tt>debug_time_init</tt> or <tt>debug_time</tt>, also resets the internal timer.
 * <br><br>
 * @param fp
 * file pointer.
 * @see debug_time_init
 */
DECL_BASE void debug_time( FILE *fp = debug_file_ptr );

/**
 * @brief 显示自上次调用gme_debug_time()或gme_debug_time_init()以来的时间，并重置计时器
 * @param fp 文件指针
 * @see gme_debug_time_init
 */
DECL_BASE void gme_debug_time(FILE* fp = debug_file_ptr);

/**
 * Prints a newline character.
 * <br><br>
 * @param fp
 * file pointer.
 */
DECL_BASE void debug_newline( FILE *fp = debug_file_ptr );

/**
 * @brief 打印换行符
 * @param fp 文件指针
 */
DECL_BASE void gme_debug_newline(FILE* fp = debug_file_ptr);

/** @} */
#endif

