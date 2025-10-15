/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/*
//	Main header for the Memory Manager.
//	This file has an immediate "C" section
//
//	Supported directives
//
//	1) MMGR_DISABLED
//		Only the C-Runtime memory allocation functions call the mmgr.
//
//	2) MMGR_FREELIST
//		Class based new and delete operators inherited from the MMGR_OBJECT base class
//		provide size information which is used for fast freelist allocations.
//
//	3) MMGR_ENABLED  * DEFAULT *
//		The C-Runtime memory allocation functions and
//		all new and delete operators call the mmgr.
//		Class based new and delete operators inherited from the MMGR_OBJECT base class
//		provide size information which is used for fast freelist allocations.
//
*/
#ifndef ACISMMGR_HXX_INCLUDED
#define ACISMMGR_HXX_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include "dcl_base.h"
#include "logical.h"
#include "static_types.hxx"

/**
 * \defgroup ACISMEMORYMANAGEMENT Memory Management
 * \ingroup ACISBASE
 *
 */
/**
 * @file mmgr.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISMEMORYMANAGEMENT
 *
 * @{
 */
/**
 * @nodoc
 */
#define NWARN(A) (A=A)
/**
 * Specifies the persistence of memory allocated via the ACIS memory manager.
 * @param eDefault
 * default memory persistence.
 * @param eSession
 * session memory persistence.
 * @param eDocument
 * document memory persistence.
 * @param eTemporary
 * temporary memory persistence.
 */
enum AcisMemType {

        eDefault = 1, eSession, eDocument, eTemporary
};
/**
 * Specifies the style of allocation.
 */
enum AcisMemCall {

	eNoSpec = 1,
	eMalloc, eCalloc, eRealloc, eStrdup, eFree,
	eGlob, eGlobDeco, eGlobArray, eGlobDecoArray,
	eClassStd, eClassArrayStd,
	eClass, eClassDeco, eClassArray, eClassDecoArray,
	eClassDecoEx, eClassDecoArrayEx,
	eClassSize, eClassSizeDeco, eClassSizeArray, eClassSizeDecoArray,
	eClassSizeDecoEx, eClassSizeDecoArrayEx,
	eClassDebugNew,eClassDebugArrayNew,
	eNewFreelist, eDelFreelist, eFreelistBlock,
	eFirstAlloc, eLastFree
};
/**
 * @nodoc
 */
SESSION_LOCAL_VAR int alloc_file_index;
#ifdef __cplusplus
    extern "C" {
#endif
/*	The "C" interface to the Memory Manager */
/**
 * ACIS wrapper for malloc.
 * @param alloc_size
 * number of bytes to allocate.
 * <br><br>
 * @see ACIS_FREE
 */
#define ACIS_MALLOC(alloc_size) acis_malloc(alloc_size,eDefault,__FILE__,__LINE__,&alloc_file_index);
#define GME_MALLOC(alloc_size) gme_malloc(alloc_size,eDefault,__FILE__,__LINE__,&alloc_file_index);
/**
 * ACIS wrapper for calloc.
 * @param alloc_num
 * number of elements to allocate.
 * @param alloc_size
 * number of bytes to allocate.
 * <br><br>
 * @see ACIS_FREE
 */
#define ACIS_CALLOC(alloc_num,alloc_size) acis_calloc(alloc_num,alloc_size,eDefault,__FILE__,__LINE__,&alloc_file_index);
#define GME_CALLOC(alloc_num,alloc_size) gme_calloc(alloc_num,alloc_size,eDefault,__FILE__,__LINE__,&alloc_file_index);
/**
 * ACIS wrapper for realloc.
 * @param memblock
 * pointer to original memory block.
 * @param old_size
 * size of original memory block.
 * @param new_size
 * number of bytes to allocate.
 * <br><br>
 * @see ACIS_REALLOC, ACIS_FREE
 */
#define ACIS_SAFE_REALLOC(memblock,old_size,new_size) acis_safe_realloc(memblock,old_size,new_size,eDefault,__FILE__,__LINE__,&alloc_file_index);
#define GME_SAFE_REALLOC(memblock,old_size,new_size) gme_safe_realloc(memblock,old_size,new_size,eDefault,__FILE__,__LINE__,&alloc_file_index);
/**
 * ACIS wrapper for realloc.
 * @param memblock
 * pointer to original memory block.
 * @param alloc_size
 * number of bytes to allocate.
 * <br><br>
 * @see ACIS_SAFE_REALLOC, ACIS_FREE
 */
#define ACIS_REALLOC(memblock,alloc_size) acis_realloc(memblock,alloc_size,eDefault,__FILE__,__LINE__,&alloc_file_index);
#define GME_REALLOC(memblock,alloc_size) gme_realloc(memblock,alloc_size,eDefault,__FILE__,__LINE__,&alloc_file_index);
/**
 * ACIS wrapper for strdup.
 * @param orgstring
 * string to copy.
 * <br><br>
 * @see ACIS_FREE
 */
#define ACIS_STRDUP(orgstring) acis_strdup(orgstring,eDefault,__FILE__,__LINE__,&alloc_file_index);
#define GME_STRDUP(orgstring) gme_strdup(orgstring,eDefault,__FILE__,__LINE__,&alloc_file_index);
/**
 * ACIS wrapper for free.
 * @param alloc_ptr
 * pointer to memory to be freed.
 * <br><br>
 * @see ACIS_MALLOC, ACIS_CALLOC, ACIS_SAFE_REALLOC, ACIS_STRDUP
 */
#define ACIS_FREE(alloc_ptr) acis_free(alloc_ptr);
#define GME_FREE(alloc_ptr) gme_free(alloc_ptr);

#ifdef _MSC_VER
    #include <malloc.h>
    /**
     * ACIS wrapper for alloca.
     * @param alloc_size
     * number of bytes to allocate from stack.
     */
    #define ACIS_ALLOCA(alloc_size) (((alloc_size) > 8) ? _alloca((alloc_size)) : _alloca(8))
    #define GME_ALLOCA(alloc_size) (((alloc_size) > 8) ? _alloca((alloc_size)) : _alloca(8))
#else
    #include <alloca.h>
    /**
     * ACIS wrapper for alloca.
     * @param alloc_size
     * number of bytes to allocate from stack.
     */
    #define ACIS_ALLOCA(alloc_size) (((alloc_size) > 8) ? alloca((alloc_size)) : alloca(8))
    #define GME_ALLOCA(alloc_size) (((alloc_size) > 8) ? alloca((alloc_size)) : alloca(8))
#endif

/**
 * @nodoc
 */

#define ACIS_MALLOCs(alloc_size) acis_malloc(alloc_size,eSession,__FILE__,__LINE__,&alloc_file_index);
#define GME_MALLOCs(alloc_size) gme_malloc(alloc_size,eSession,__FILE__,__LINE__,&alloc_file_index);
/**
 * @nodoc
 */

#define ACIS_MALLOCd(alloc_size) acis_malloc(alloc_size,eDocument,__FILE__,__LINE__,&alloc_file_index);
#define GME_MALLOCd(alloc_size) gme_malloc(alloc_size,eDocument,__FILE__,__LINE__,&alloc_file_index);
/**
 * @nodoc
 */

#define ACIS_MALLOCt(alloc_size) acis_malloc(alloc_size,eTemporary,__FILE__,__LINE__,&alloc_file_index);
#define GME_MALLOCt(alloc_size) gme_malloc(alloc_size,eTemporary,__FILE__,__LINE__,&alloc_file_index);
/**
 * @nodoc
 */

#define ACIS_CALLOCs(alloc_num,alloc_size) acis_calloc(alloc_num,alloc_size,eSession,__FILE__,__LINE__,&alloc_file_index);
#define GME_CALLOCs(alloc_num,alloc_size) gme_calloc(alloc_num,alloc_size,eSession,__FILE__,__LINE__,&alloc_file_index);
/**
 * @nodoc
 */

#define ACIS_CALLOCd(alloc_num,alloc_size) acis_calloc(alloc_num,alloc_size,eDocument,__FILE__,__LINE__,&alloc_file_index);
#define GME_CALLOCd(alloc_num,alloc_size) gme_calloc(alloc_num,alloc_size,eDocument,__FILE__,__LINE__,&alloc_file_index);
/**
 * @nodoc
 */

#define ACIS_CALLOCt(alloc_num,alloc_size) acis_calloc(alloc_num,alloc_size,eTemporary,__FILE__,__LINE__,&alloc_file_index);
#define GME_CALLOCt(alloc_num,alloc_size) gme_calloc(alloc_num,alloc_size,eTemporary,__FILE__,__LINE__,&alloc_file_index);
/**
 * @nodoc
 */

DECL_BASE void * acis_malloc( size_t alloc_size, enum AcisMemType alloc_type, const char *alloc_file, int alloc_line, int *alloc_file_index);
DECL_BASE void * gme_malloc( size_t alloc_size, enum AcisMemType alloc_type, const char *alloc_file, int alloc_line, int *alloc_file_index);
/**
 * @nodoc
 */

DECL_BASE void * acis_calloc( size_t alloc_num, size_t alloc_size, enum AcisMemType alloc_type, const char *alloc_file, int alloc_line, int *alloc_file_index);
DECL_BASE void * gme_calloc( size_t alloc_num, size_t alloc_size, enum AcisMemType alloc_type, const char *alloc_file, int alloc_line, int *alloc_file_index);

/**
 * @nodoc
 */
DECL_BASE void * acis_safe_realloc( void * memblock, size_t old_size, size_t new_size, enum AcisMemType alloc_type, const char *alloc_file, int alloc_line, int *alloc_file_index);
DECL_BASE void * gme_safe_realloc( void * memblock, size_t old_size, size_t new_size, enum AcisMemType alloc_type, const char *alloc_file, int alloc_line, int *alloc_file_index);

/**
 * @nodoc
 */
DECL_BASE void * acis_realloc( void * memblock, size_t alloc_size, enum AcisMemType alloc_type, const char *alloc_file, int alloc_line, int *alloc_file_index);
DECL_BASE void * gme_realloc( void * memblock, size_t alloc_size, enum AcisMemType alloc_type, const char *alloc_file, int alloc_line, int *alloc_file_index);

/**
 * @nodoc
 */
DECL_BASE char * acis_strdup( const char *orgstring, enum AcisMemType alloc_type, const char *alloc_file, int alloc_line, int *alloc_file_index);
DECL_BASE char * gme_strdup( const char *orgstring, enum AcisMemType alloc_type, const char *alloc_file, int alloc_line, int *alloc_file_index);

/**
 * @nodoc
 */
DECL_BASE void   acis_free( void * alloc_ptr);
DECL_BASE void   gme_free( void * alloc_ptr);

#ifdef __cplusplus
    }

    /*
    //	Second block of this file
    //	The "C++" interface of the Memory Manager
    */

    /* The complex allocator and destructor */
    /**
     * @nodoc
     */
    extern DECL_BASE void * (*acis_allocate)( size_t alloc_size, AcisMemType alloc_type, enum AcisMemCall alloc_call,
							    const char *alloc_file, int alloc_line, int *alloc_file_index);
    /**
     * @nodoc
     */
    DECL_BASE void * (*gme_allocate)( size_t alloc_size, AcisMemType alloc_type, enum AcisMemCall alloc_call,
                                const char* alloc_file, int alloc_line, int* alloc_file_index);
    /**
     * @nodoc
     */
    extern DECL_BASE void   (*acis_discard)( void * alloc_ptr, enum AcisMemCall alloc_call, size_t alloc_size);
    /**
     * @nodoc
     */
    DECL_BASE void   (*gme_discard)( void * alloc_ptr, enum AcisMemCall alloc_call, size_t alloc_size);


    /* The raw allocator and destructor */
    /**
     * @nodoc
     */
    extern void *(*acis_raw_malloc)(size_t);
    /**
     * @nodoc
     */
    DECL_BASE void* (*gme_raw_malloc)(size_t);
    /**
     * @nodoc
     */
    extern void (*acis_raw_free)(void*);
    /**
     * @nodoc
     */
    DECL_BASE void (*gme_raw_free)(void*);

    /**
     * @nodoc
     */
    extern logical acis_debug;
    DECL_BASE logical gme_debug;

    /**
     * @nodoc
     */
    extern logical acis_freelist;
    DECL_BASE logical gme_freelist;

    #ifdef MMGR_DISABLED
        /**
         * ACIS wrapper for C++ new - Complements ACIS_DELETE.
         * @see ACIS_DELETE
         */
        #define ACIS_NEW  new
        #define GME_NEW new
        /**
         * @nodoc
         */
        #define ACIS_NEWs new
        /**
         * @nodoc
         */
        #define ACIS_NEWd new
        /**
         * @nodoc
         */
        #define ACIS_NEWt new
        /**
         * ACIS wrapper for C++ delete - Complements <tt>ACIS_NEW</tt>.
         * @see ACIS_NEW, STD_CAST
         */
        #define ACIS_DELETE delete
        #define GME_DELETE delete

        #ifdef ACIS_BASE_CLASS
            #undef ACIS_BASE_CLASS
        #endif
        #ifdef MMGR_DEBUG
            #undef MMGR_DEBUG
        #endif
        #ifdef MMGR_AUDIT_LEAKS
            #undef MMGR_AUDIT_LEAKS
        #endif
        #ifdef MMGR_ENABLED
            #undef MMGR_ENABLED
        #endif
        #ifdef MMGR_POLY_FREELIST
            #undef MMGR_POLY_FREELIST
        #endif
        #ifdef MMGR_SUPPORT_THIS
            #undef MMGR_SUPPORT_THIS
        #endif
        #ifdef MMGR_FREELIST_THIS
            #undef MMGR_FREELIST_THIS
        #endif
        #define ACIS_BASE_CLASS
        #define MMGR_SUPPORT_THIS
        #define MMGR_FREELIST_THIS
        #define STD_TYPE(A) (A)
        #define STD_CAST
    #else /* MMGR_DISABLED */
        /*
        // Third block of this file
        // Either MMGR_FREELIST or MMGR_ENABLED defined
        */

        #include <new>
        		
        #if (_MSC_VER >= 1200) || defined(osf1) || defined(__KCC) || defined(mac)
            /**
             * @nodoc
             */
            #define NEED_EXCEPTION_DELETE
        #endif

        #if (_MSC_VER >= 1200)
            #pragma warning( disable: 4514 ) /* unreferenced inline function has been removed */
        #endif

        /* The initialization of the memory manager must come first */
        /**
         * @nodoc
         */
        DECL_BASE void initialize_mmgr_system();
        /**
         * @nodoc
         */
        DECL_BASE void gme_initialize_mmgr_system();
        /**
         * @nodoc
         */
        DECL_BASE void terminate_mmgr_system();
        /**
         * @nodoc
         */
        DECL_BASE void gme_terminate_mmgr_system();

        /**
         * @nodoc
         */
        class nifty_mmgr_counter {

         public:

             nifty_mmgr_counter() {
                    initialize_mmgr_system();
            }

             ~nifty_mmgr_counter() {
                    terminate_mmgr_system();
            }

        };

        #if defined(_MK_FWNAME_) || defined(BUILDING_ACIS)
            /**
             * @nodoc
             */
            SESSION_LOCAL_VAR nifty_mmgr_counter nifty;
        #endif
        /**
         * ACIS wrapper for C++ delete - Complements <tt>ACIS_NEW</tt>.
         * @see ACIS_NEW, STD_CAST
         */
        #define ACIS_DELETE delete

        #ifdef MMGR_AUDIT_LEAKS
            #ifndef MMGR_DEBUG
                /**
                 * @nodoc
                 */
                #define MMGR_DEBUG
            #endif
        #endif

        /**
         * @nodoc
         */
        #define MMGR_BASIC_FREELIST_SUPPORT \
            void * operator new( size_t alloc_size) { \
                    return acis_allocate( alloc_size, eDefault, eClassSize, NULL, 0, NULL); \
            } \
            void operator delete(void * alloc_ptr, size_t alloc_size ) { \
                    acis_discard( alloc_ptr, eClassSize, alloc_size ); \
            }

        /*
        // For Microsoft, we overload their <tt>DEBUG_NEW</tt> style signature.
        */
        #if defined(_MSC_VER)
            /**
             * @nodoc
             */
            #define MMGR_DEBUG_NEW
        #endif

        #ifdef MMGR_DEBUG_NEW
            /**
             * @nodoc
             */
            #define MMGR_DEBUG_NEW_SUPPORT \
                void * operator new( size_t alloc_size, const char *alloc_file, int alloc_line ) { \
                        return acis_allocate( alloc_size, eDefault, eClassDebugNew, alloc_file, alloc_line, NULL ); \
                } \
                void * operator new[]( size_t alloc_size, const char *alloc_file, int alloc_line ) { \
                        return acis_allocate( alloc_size, eDefault, eClassDebugArrayNew, alloc_file, alloc_line, NULL ); \
                } \
                void operator delete(void * alloc_ptr, const char *alloc_file, int alloc_line ) { \
                        alloc_file = alloc_file, alloc_line = alloc_line; acis_discard( alloc_ptr, eClassDebugNew, 0 ); \
                } \
                void operator delete[](void * alloc_ptr, const char *alloc_file, int alloc_line ) { \
                        alloc_file = alloc_file, alloc_line = alloc_line; acis_discard( alloc_ptr, eClassDebugArrayNew, 0 ); \
                }
        #else
            /**
             * @nodoc
             */
            #define MMGR_DEBUG_NEW_SUPPORT
        #endif /* MMGR_DEBUG_NEW */

        /*
        // Here we make the distinction between which platforms
        // support the new/delete signatures required by template
        // libraries, e.g., STL. The compilers that don't currently
        // support these signatures may support them with future
        // revisions, so this is marked REVISIT.
        */
        #if	defined(aix) || \
	        defined(sgi) || \
	        (defined(osf1) && !defined(osf1_64))
            /**
             * @nodoc
             */
            #define MMGR_STL_SUPPORT
        #else
            /**
             * @nodoc
             */
            #define MMGR_STL_SUPPORT \
                void * operator new( size_t, void *ptr ) { \
                        return ptr; \
                } \
                void operator delete( void *, void * ) { \
                        return; \
                }
        #endif
        #ifdef MMGR_FREELIST
            /**
             * ACIS wrapper for C++ new - Complements <tt>ACIS_DELETE</tt>.
             * @see ACIS_DELETE
             */
            #define ACIS_NEW  new

            /**
             * @nodoc
             */
            #define ACIS_NEWs new

            /**
             * @nodoc
             */
            #define ACIS_NEWd new

            /**
             * @nodoc
             */
            #define ACIS_NEWt new

            #ifdef MMGR_ENABLED
                #undef MMGR_ENABLED
            #endif
            /**
             * @nodoc
             */
            #define MMGR_SUPPORT_THIS
            /**
             * @nodoc
             */
            #define MMGR_EXTENDED_FREELIST_SUPPORT
            /**
             * @nodoc
             */
            #define MMGR_FREELIST_EXCEPTION_SUPPORT
            /**
             * @nodoc
             */
            #define STD_TYPE(A) (A)

            /**
             * @nodoc
             */
            #define STD_CAST

        #else    // 进入默认的 MMGR_ENABLED 情况
            #ifndef MMGR_ENABLED
            /**
             * @nodoc
             */
                #define MMGR_ENABLED
            #endif /* MMGR_FREELIST */

            /**
             * @nodoc
             */
            #define MMGR_FREELIST

            /**
             * ACIS wrapper for C++ new - Complements <tt>ACIS_DELETE</tt>.
             * @see ACIS_DELETE
             */
            #define ACIS_NEW  new(eDefault,__FILE__,__LINE__,&alloc_file_index)

            /**
             * @nodoc
             */
            #define ACIS_NEWs new(eSession,__FILE__,__LINE__,&alloc_file_index)

            /**
             * @nodoc
             */
            #define ACIS_NEWd new(eDocument,__FILE__,__LINE__,&alloc_file_index)

            /**
             * @nodoc
             */
            #define ACIS_NEWt new(eTemporary,__FILE__,__LINE__,&alloc_file_index)

            /**
             * @nodoc
             */
            class DECL_BASE ACIS_STD_TYPE_OBJECT {

            public:

                /**
                 * @nodoc
                 */
                void operator delete(void * alloc_ptr ) {
                        acis_discard( alloc_ptr, eClassStd, 0 );
                }
                /**
                 * @nodoc
                 */
                void operator delete [](void * alloc_ptr ) {
                        acis_discard( alloc_ptr, eClassArrayStd, 0 );
                }
            };

            /**
             * @nodoc
             */
            #define STD_TYPE(A) ((ACIS_STD_TYPE_OBJECT*)A)

            /**
             * Informs the ACIS memory manager that a simple data type is being deleted.
             * <br>
             * <b>Role:</b> When deleting a simple data type using <tt>ACIS_DELETE</tt>, you
             * must use the <tt>STD_CAST</tt> macro.
             * <br><br>
             * By using the <tt>STD_CAST</tt> macro, the ACIS memory management system can intercept
             * the call and we will successfully track the memory.
             * <br><br>
             * Do not use <tt>STD_CAST</tt> on anything but simple data types. Doing so may result
             * in your application crashing.
             * <br><br>
             * Here are some examples:
             * <pre>
             * double * pd = ACIS_NEW double;
             * ACIS_DELETE STD_CAST pd;<br>
             * double * dlist = ACIS_NEW double[14];
             * ACIS_DELETE [] STD_CAST dlist;<br>
             * SPAposition *ppos = ACIS_NEW SPAposition;
             * ACIS_DELETE ppos;	// STD_CAST not needed! Not a simple data type.<br>
             * SPAposition **pppos = ACIS_NEW SPAposition *;
             * ACIS_DELETE STD_CAST pppos;	// STD_CAST is needed - we're deleting a pointer.
             * </pre>
             */
            #define STD_CAST (ACIS_STD_TYPE_OBJECT*)

            #if defined(__KCC) || defined(solaris) || defined(osf1) || defined(mac)
                /**
                 * @nodoc
                 */
                #define MMGRTHROW throw()
                /**
                 * @nodoc
                 */
                #define MMGRTHROW_BAD_ALLOC throw(std::bad_alloc)
            #elif defined(hp700)
                /**
                 * @nodoc
                 */
                #define MMGRTHROW throw()
                /**
                 * @nodoc
                 */
                #define MMGRTHROW_BAD_ALLOC throw(bad_alloc)
            #else
                /**
                 * @nodoc
                 */
                #define MMGRTHROW
                /**
                 * @nodoc
                 */
                #define MMGRTHROW_BAD_ALLOC
            #endif

            #if defined(INTERNAL_DEBUG_CHECKS) && !defined(NO_MMGR_CATCH_ALL)

                // Temporarily suppress the new warning for VS 2015.
                // https://msdn.microsoft.com/en-us/library/mt723604.aspx
                #if _MSC_VER >= 1900
                                #pragma warning ( disable: 4595 )
                #endif
                    /* globally scoped new */
                    /**
                     * @nodoc
                     */
                    inline void * operator new( size_t alloc_size) MMGRTHROW_BAD_ALLOC {
                        return acis_allocate( alloc_size, eDefault, eGlob, NULL, 0, NULL);
                    }

                    /* globally scoped array new */
                    /**
                     * @nodoc
                     */
                    inline void * operator new[]( size_t alloc_size) MMGRTHROW_BAD_ALLOC {
                        return acis_allocate( alloc_size, eDefault, eGlobArray, NULL, 0, NULL);
                    }
                    /* globally scoped delete */
                    /**
                     * @nodoc
                     */
                    inline void operator delete( void * alloc_ptr) MMGRTHROW {
                        acis_discard( alloc_ptr, eGlob, 0 );
                    }

                    /* globally scoped array delete */
                    /**
                     * @nodoc
                     */
                    inline void operator delete [] ( void * alloc_ptr) MMGRTHROW {
                        acis_discard( alloc_ptr, eGlobArray, 0 );
                    }

            #endif /* INTERNAL_DEBUG_CHECKS */

            /* globally scoped decorated new */
            /**
             * @nodoc
             */
            inline void * operator new( size_t alloc_size, AcisMemType alloc_type, const char *alloc_file, int alloc_line, int *alloc_file_index) {
                return acis_allocate( alloc_size, alloc_type, eGlobDeco, alloc_file, alloc_line, alloc_file_index);
            }

            /* globally scoped decorated array new */
            /**
              * @nodoc
              */
            inline void * operator new []( size_t alloc_size, AcisMemType alloc_type, const char *alloc_file, int alloc_line, int *alloc_file_index) {
                return acis_allocate( alloc_size, alloc_type, eGlobDecoArray, alloc_file, alloc_line, alloc_file_index);
            }

            /**
             * @nodoc
             */
            #define MMGR_NO_FREELIST_SUPPORT \
                void * operator new( size_t alloc_size ) { \
                        return acis_allocate( alloc_size, eDefault, eClass, NULL, 0, NULL); \
                } \
                void operator delete(void * alloc_ptr ) { \
                        acis_discard( alloc_ptr, eClass, 0 ); \
                } \
                void * operator new []( size_t alloc_size ) { \
                        return acis_allocate( alloc_size, eDefault, eClassArray, NULL, 0, NULL); \
                } \
                void operator delete [](void * alloc_ptr ) { \
                        acis_discard( alloc_ptr, eClassArray, 0 ); \
                } \
                void * operator new( size_t alloc_size, AcisMemType alloc_type, const char *alloc_file, int alloc_line, int *alloc_file_index) { \
                        return acis_allocate( alloc_size, alloc_type, eClassDeco, alloc_file, alloc_line, alloc_file_index ); \
                } \
                void * operator new[]( size_t alloc_size, AcisMemType alloc_type, const char *alloc_file, int alloc_line, int *alloc_file_index) { \
                        return acis_allocate( alloc_size, alloc_type, eClassDecoArray, alloc_file, alloc_line, alloc_file_index ); \
                }
            /**
             * @nodoc
             */
            #define MMGR_EXTENDED_FREELIST_SUPPORT \
                void * operator new []( size_t alloc_size ) { \
                        return acis_allocate( alloc_size, eDefault, eClassSizeArray, NULL, 0, NULL); \
                } \
                void operator delete [](void * alloc_ptr ) { \
                        acis_discard( alloc_ptr, eClassSizeArray, 0 ); \
                } \
                void * operator new( size_t alloc_size, AcisMemType alloc_type, const char *alloc_file, int alloc_line, int *alloc_file_index) { \
                        return acis_allocate( alloc_size, alloc_type, eClassSizeDeco, alloc_file, alloc_line, alloc_file_index ); \
                } \
                void * operator new[]( size_t alloc_size, AcisMemType alloc_type, const char *alloc_file, int alloc_line, int *alloc_file_index) { \
                        return acis_allocate( alloc_size, alloc_type, eClassSizeDecoArray, alloc_file, alloc_line, alloc_file_index ); \
                }

            #ifdef NEED_EXCEPTION_DELETE
                /**
                 * @nodoc
                 */
                #define MMGR_NO_FREELIST_EXCEPTION_SUPPORT \
                    void operator delete(void * alloc_ptr, enum AcisMemType alloc_type, const char *alloc_file, int alloc_line, int *alloc_file_index) { \
                            alloc_file = alloc_file; alloc_file_index = alloc_file_index; alloc_line = alloc_line; alloc_type = alloc_type; acis_discard( alloc_ptr, eClassDecoEx, 0); \
                    } \
                    void operator delete [](void * alloc_ptr, enum AcisMemType alloc_type, const char *alloc_file, int alloc_line, int *alloc_file_index) { \
                            alloc_file = alloc_file; alloc_file_index = alloc_file_index; alloc_line = alloc_line; alloc_type = alloc_type; acis_discard( alloc_ptr, eClassDecoArrayEx, 0); \
                    }
                /**
                 * @nodoc
                 */
                #define MMGR_FREELIST_EXCEPTION_SUPPORT \
                    void operator delete(void * alloc_ptr, enum AcisMemType alloc_type, const char *alloc_file, int alloc_line, int *alloc_file_index) { \
                            alloc_file = alloc_file; alloc_file_index = alloc_file_index; alloc_line = alloc_line; alloc_type = alloc_type; acis_discard( alloc_ptr, eClassSizeDecoEx, 0); \
                    } \
                    void operator delete [](void * alloc_ptr, enum AcisMemType alloc_type, const char *alloc_file, int alloc_line, int *alloc_file_index) { \
                            alloc_file = alloc_file; alloc_file_index = alloc_file_index; alloc_line = alloc_line; alloc_type = alloc_type; acis_discard( alloc_ptr, eClassSizeDecoArrayEx, 0); \
                    }
            #else
                /**
                 * @nodoc
                 */
                #define MMGR_FREELIST_EXCEPTION_SUPPORT
                /**
                 * @nodoc
                 */
                #define MMGR_NO_FREELIST_EXCEPTION_SUPPORT
            #endif

            /**
             * Use this macro inside your class definition to enhance your class with ACIS memory management.
             * <br>
             * <b>Note:</b> does not provide support for ACIS freelisting.
             * @see MMGR_FREELIST_THIS, ACIS_OBJECT
             */
            #define MMGR_SUPPORT_THIS \
            MMGR_STL_SUPPORT \
            MMGR_DEBUG_NEW_SUPPORT \
            MMGR_NO_FREELIST_SUPPORT \
            MMGR_NO_FREELIST_EXCEPTION_SUPPORT

        /* MMGR_ENABLED */
        #endif

        /**
         * Use this macro inside your class definition to enhance your class with ACIS memory management and ACIS freelisting.
         * @see MMGR_SUPPORT_THIS, ACIS_OBJECT
         */
        #define MMGR_FREELIST_THIS \
        MMGR_STL_SUPPORT \
        MMGR_DEBUG_NEW_SUPPORT \
        MMGR_BASIC_FREELIST_SUPPORT \
        MMGR_EXTENDED_FREELIST_SUPPORT \
        MMGR_FREELIST_EXCEPTION_SUPPORT

        /**
         * @nodoc
         */
        #define ACIS_BASE_CLASS : public ACIS_OBJECT

        /**
         * Provides a base class for class level memory management of ACIS classes.
         * <br>
         * <b>Role:</b> The <tt>ACIS_OBJECT</tt> class provides a base class level interface to memory management
         * by overriding the standard <tt>new</tt> and <tt>delete</tt> operators. Deriving from <tt>ACIS_OBJECT</tt> provides
         * the same new/delete methods as placing <tt>MMGR_FREELIST_THIS</tt> inside your class.
         * <br><br>
         * A simple <tt>new</tt> operator is provided for older compilers that do not support overloading of <tt>new</tt>
         * and <tt>delete</tt>, and a decorated <tt>new</tt> operator that keeps track of the source file and line where
         * the <tt>new</tt> was issued is provided for newer compilers.
         * <br><br>
         * Additional versions of <tt>new</tt> are provided for allocating arrays of instances as well as single
         * instances.
         * @see MMGR_SUPPORT_THIS, MMGR_FREELIST_THIS
         */
        class DECL_BASE ACIS_OBJECT {

        public:
            /**
             * @nodoc
             */
            MMGR_FREELIST_THIS
        #if 0
        ; /* semicolon needed for mkman */
        #endif
        };
    #endif /*Either MMGR_FREELIST or MMGR_ENABLED */
#endif /* __cplusplus */

/**
 * @nodoc
 */
logical mmgr_running();

/**
 * @nodoc
 */
DECL_BASE logical gme_mmgr_running();

#if (defined(OS_Darwin) || defined(OS_Linux))  && defined(__cplusplus) 
/**
 * @nodoc
 */
// For internal use only
struct spa_mac_null
{
int i;
template<typename T>
operator T*()
{
	return (T*)i;
}
operator int()
{
	return i;
}
static spa_mac_null nl;
};

/**
 * @nodoc
 */
// For internal use only
void DECL_BASE mac_null_dummy();

#define SpaACIS 1
#if defined(_MK_CONTNAME_) && (1 == _MK_CONTNAME_)
#ifdef OS_Linux
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <iterator>
#endif	// OS_Linux
#undef NULL
#define NULL spa_mac_null::nl
#endif
#undef SpaACIS

#endif


/** @} */
#endif /* ACISMMGR_HXX_INCLUDED */

