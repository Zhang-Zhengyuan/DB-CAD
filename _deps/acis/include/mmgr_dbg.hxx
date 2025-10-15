/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef MMGR_DBG_HXX
#define MMGR_DBG_HXX

#ifdef mac
#include <time.h>
#endif

#include <sys/timeb.h>
#include "base.hxx"
#include "mmgr.hxx"
#include "mutex.hxx"


struct mmgr_audit_block {
	
	mmgr_audit_block * next;
	void * alloc_ptr;
	const char * alloc_file;
	size_t alloc_size;
	size_t alloc_num;
	AcisMemType alloc_type;
	AcisMemCall alloc_call;
	unsigned int alloc_line;

#ifdef THREAD_SAFE_ACIS
	int alloc_thread;
	int delete_thread;
#endif
};


// 24 bits (4096 entries) are used to hash the allocated
// pointers when auditleaks in enabled.
LOCAL_CONST int hash_size = 4096;
#define HASH_FUNCTION(p)	(((intptr_t)p & 0x000fff00) >> 8)


struct mmgr_statistics {

	size_t high_bytes;     // 分配出的最多字节数
	size_t high_alloc_call;  // 之前的最多申请数         已申请的总次数 -1
	size_t alloc_bytes;   // 已申请过的总字节数
	size_t alloc_calls;   // 已申请的总次数
	size_t free_bytes;    // 已释放的总字节数
	size_t free_calls;    // 已释放的总次数
	size_t size_array[257];

	char ** alloc_file_names;  // 申请内存的文件名列表
	int alloc_file_count;      // 申请内存的文件数量

	char audit_file[256];   // mmgr.log

	size_t double_deletes;
	size_t mismatched_callers;
	size_t mismatched_sizes;
	size_t pre_alloc_calls;
	size_t global_new_calls;
	size_t global_delete_calls;
	size_t range_begin, range_end;
	struct timeb start_time;                    // 运行时间
	mmgr_audit_block * hash_array[hash_size];  // 对具体分配出去的对象建立索引
	FILE * mmgrfile;                           // 转储文件名

	mutex_resource mmgr_statistics_mutex;
};

DECL_BASE mmgr_statistics * mmgr_debug_stats();
DECL_BASE mmgr_statistics* gme_mmgr_debug_stats();
DECL_BASE void mmgr_dump_range( size_t range_begin, size_t range_end );
DECL_BASE void gme_mmgr_dump_range(size_t range_begin, size_t range_end);
DECL_BASE void mmgr_dump_info( void* alloc_ptr );
DECL_BASE void gme_mmgr_dump_info(void* alloc_ptr);

/**
 * @brief 格式化打印到Debug窗口
 * @param strOutputString 打印的数据
 * @return 空
 */
void OutputDebugPrintf(const char* str_output_string, ...);

#endif 
// MMGR_DBG_HXX
