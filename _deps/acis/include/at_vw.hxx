/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/**********************************************************************/
/* Attaches IHL data and viewing parameters to bodies.                */
/**********************************************************************/
#if !defined AT_VW_INCLUDE
#define AT_VW_INCLUDE

#include "at_ihl.hxx"

class IHL_EDGE;
class IHL_CAMERA;

/**
 * @file at_vw.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup IHLAPI
 *  \brief Declared at <at_vw.hxx>
 *  @{
 */

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_IHL_VW, IHL )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

/**
 * @nodoc
 */
extern int ATTRIB_IHL_VW_TYPE;

/**
 * @nodoc
 */
#define ATTRIB_IHL_VW_LEVEL (ATTRIB_IHL_LEVEL + 1)


/**
 * Defines an attribute that attaches viewing information and hidden line data to an entity.
 * <br>
 * <b>Role:</b> An <tt>ATTRIB_IHL_VW</tt> contains a view token, which uniquely identifies a view,
 * an <tt>IHL_CAMERA</tt>, which defines the viewing parameters associated with the view, and an <tt>IHL_EDGE</tt> 
 * list, which contains the hidden line data.
 */
class DECL_IHL ATTRIB_IHL_VW : public ATTRIB_IHL
{
private:

    IHL_EDGE *last;        // last IHL_EDGE. Used by 'add_edge()'.

protected:

    IHL_EDGE   *pedglist;  // pointer to the start of the list of IHL_EDGEs.
    IHL_CAMERA *pcamera;   // pointer to a camera containing viewing information
    int        vw_token;   // identifies the view

    void set_edge_list(IHL_EDGE *edg_list);
    void set_camera(IHL_CAMERA *cam);
    void set_view(int tok);

public:

/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt> operator,
 * because this reserves the memory on the heap, a requirement to support roll back and history
 * management.
 * <br><br>
 * @param owner
 * the owning entity. Default is <tt>NULL</tt>.
 * @param cam
 * contains viewing parameters. Default is <tt>NULL</tt>.
 * @param edg_list
 * pointer to the first edge of the list. Default is <tt>NULL</tt>.
 * @param tok
 * unique identifier for the view. Default is <tt>0</tt>.
 */
    ATTRIB_IHL_VW(
        ENTITY     *owner = NULL,         // owning ENTITY
        IHL_CAMERA *cam = NULL,           // camera
        IHL_EDGE   *edg_list = NULL,      // edge list
        int        tok = 0);              // view token

	/**
	 * Returns <tt>TRUE</tt> if this is pattern compatible.
	 */
    virtual logical pattern_compatible() const;
 
	/**
     * Returns the pointer to the IHL_CAMERA.
	 */
    IHL_CAMERA * camera() const { return(pcamera); }

	/**
     * Returns a pointer to the list of IHL_EDGEs.
	 */
    IHL_EDGE * edge_list() const { return(pedglist); }

	/**
     * Returns the view token.
	 */
    int view() const { return(vw_token); }

	/**
     * Adds a new edge to the list.
	 * <br><br>
	 * @param edg
	 * the new edge.
	 */
    void add_edge(IHL_EDGE *edg);

    /**
     * @nodoc
     */
    ATTRIB_FUNCTIONS( ATTRIB_IHL_VW, IHL )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif

};

/**
 * Returns an <tt>ATTRIB_IHL_VW</tt> attribute with the given view token that is attached to the given entity,
 * if one exists.
 * <br><br>
 * @param owner
 * the entity to search.
 * @param token
 * given view token.
 */
DECL_IHL ATTRIB_IHL_VW* find_attrib_vw(
        ENTITY *owner,
        int    token );


/*! @} */
#endif
