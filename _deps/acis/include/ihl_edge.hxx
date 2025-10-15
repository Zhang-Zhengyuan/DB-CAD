/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/**********************************************************************/
/* Defines a class for storing standard output of IHL.                */
/**********************************************************************/
#if !defined ( IHL_EDGE_INCLUDE )
#define IHL_EDGE_INCLUDE

#include "en_ihl.hxx"

class IHL_SEGMENT;

/**
 * @file ihl_edge.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup IHLAPI
 *  \brief Declared at <ihl_edge.hxx>
 *  @{
 */

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(IHL_EDGE, IHL )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

/**
 * @nodoc
 */
extern DECL_IHL int IHL_EDGE_TYPE;
           
/**
 * Identifier that gives number of levels of derivation of the IHL_EDGE class from ENTITY.
 */
#define IHL_EDGE_LEVEL (ENTITY_LEVEL + 2)

/**
 * Defines an edge in the hidden line data structure.
 * <br>
 * <b>Role:</b> The hidden line data structure contains a doubly-linked list of edges. These edges represent 
 * either model edges or silhouette edges. Each edge consists of a number of segments. Each segment is categorized
 * as being visible or hidden. That is, an edge may contain multiple visible and hidden segments.
 * <br><br>
 * For more information refer to the Technical Article <i>Interactive Hidden Line</i>.
 */
class DECL_IHL IHL_EDGE : public ENTITY_IHL
{
private :

    IHL_EDGE    *pPrevious,
                *pNext;
    IHL_SEGMENT *pSegmt;

public :

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt> operator,
 * because this reserves the memory on the heap, a requirement to support roll back and history
 * management.
 * <br><br>
 * @param prev
 * previous IHL_EDGE in the doubly-linked list.
 * @param next
 * next IHL_EDGE in the doubly-linked list.
 * @param lseg
 * pointer to first IHL_SEGMENT of this edge.
 */
    IHL_EDGE(
        IHL_EDGE    *prev = NULL,
        IHL_EDGE    *next = NULL,
        IHL_SEGMENT *lseg = NULL );

    // Inquiry functions

    /**
     * Returns the previous IHL_EDGE in the linked list.
     */
    IHL_EDGE * previous() { return(pPrevious); }

    /**
    * Returns the next IHL_EDGE in the linked list.
    */
    IHL_EDGE * next() { return(pNext); }

    /**
     * Returns the IHL_SEGMENT pointer.
     */
    IHL_SEGMENT * segment() { return(pSegmt); }

    // Modification functions. Used internally by IHL.

    /**
     * Sets the previous IHL_EDGE pointer.
     */
    void set_previous(IHL_EDGE *prev) { pPrevious = prev; }

    /**
     * Sets the next IHL_EDGE pointer.
     */
    void set_next(IHL_EDGE *next) { pNext = next; }

    // Include the standard member functions for all entities.
    /**
     * @nodoc
     */
    ENTITY_FUNCTIONS( IHL_EDGE, IHL )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

};

/* @} */
#endif

