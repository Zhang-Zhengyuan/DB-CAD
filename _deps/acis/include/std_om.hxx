/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/**********************************************************************/
/* Defines standard IHL output manager.                               */
/**********************************************************************/
#if ! defined ( STD_OM_INCLUDE )
#define  STD_OM_INCLUDE

#include "ihloutp.hxx"
#include "lists.hxx"

class ENTITY_LIST;
class IHL_CAMERA;
class IHL_SEGMENT;
class ATTRIB_IHL_VW;

/**
 * @file std_om.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 *! \addtogroup IHLAPI
 *  \brief Declared at <std_om.hxx>
 *  @{
 */

/**********************************************************************/

/**
 * Defines an output manager for interactive hidden line (IHL) data that is used by the Scheme AIDE.
 * <br>
 * <b>Role:</b> This output manager serves as an example for customer-derived output managers,
 * derived from <tt>IHL_OUTPUT_MANAGER</tt>. An IHL output manager is conceptually similar to a mesh manager
 * for the Faceter. An IHL output manager controls the output of hidden line data for use by
 * an application. 
 */
class DECL_IHL IHL_STDOUT_MANAGER : public IHL_OUTPUT_MANAGER
{
private :

    const ENTITY_LIST *bodlst;       // body list
    IHL_CAMERA        *camera;       // camera
    ENTITY_LIST       *seglst;       // IHL_SEGMENTs
    int               view;          // view token
    logical           fNeedIntrSeg,  // TRUE if interior segments needed
                      fNeedHidSeg,   // TRUE if hidden segments needed
                      fNeedNoHid;    // STI mka TRUE if no hidden segments calc needed
    IHL_SEGMENT       *cur_seg;
    ATTRIB_IHL_VW     *att_vw;       // current view attribute

public :

	/**
	 * C++ Constructor.
	 * <br>
	 * @param bodies
	 * list of bodies to generate IHL views for.
	 * @param cam
	 * contains viewing parameters.
	 * @param segments
	 * list of IHL_SEGMENTs.
	 * @param token
	 * unique view token.
	 * @param fIntrSeg
	 * <tt>TRUE</tt> if hidden line segments are to be generated for edges interior to a face.
	 * @param fHidSeg
	 * <tt>TRUE</tt> if hidden line segments are to be output.
	 * @param fNoHidCalc
	 * <tt>TRUE</tt> if no hidden line calculations are to be performed.
	 */

    IHL_STDOUT_MANAGER(
        const ENTITY_LIST& bodies,
        IHL_CAMERA         *cam,
        ENTITY_LIST&       segments,
        int                token,
        logical            fIntrSeg,
        logical            fHidSeg,
        logical            fNoHidCalc );

	/**
	 * C++ Destructor.
	 */
	~IHL_STDOUT_MANAGER();

	/**
     * Specifies whether 2D or 3D data is to be generated.
	 * <br>
	 * The <tt>IHL_STDOUT_MANAGER</tt> always generates 2D output.
	 */
    logical need_3D_coordinates() { return(FALSE); }

	/**
     * Output invisible line segments?
	 */
    logical need_hidden_segments() { return(fNeedHidSeg); }

	/**
     * Output line segments interior to a face?
	 */
    logical need_interior_segments() { return(fNeedIntrSeg); }

	/**
     * Needs hidden line segment calculation?
	 */
    logical need_no_hidden_calc() { return(fNeedNoHid); }

	/**
     * Gets the current segment.
	 */
    virtual IHL_SEGMENT * get_cur_seg() { return(cur_seg); }

	/**
     * Sets current segment to the given segment.
	 * <br>
	 * @param seg
	 * new, current segment.
	 */
    virtual void  set_cur_seg(IHL_SEGMENT * seg) { cur_seg = seg; }

	/**
     * Adds the given segment to seglst.
	 * <br>
	 * @param seg
	 * segment to add to the list
	 */
    virtual void  add_seg_to_list(IHL_SEGMENT * seg) { seglst->add(seg); }

	/**
     * Initializes the IHL data structure for the next entity to be processed.
	 * <br>
	 * Attaches ATTRIB_IHL_VW attributes to the body.
	 */
    virtual void announce_next_object();

	/**
     * Processes a 2D segment.
	 * <br>
	 * @param lseg
	 * 2D coordinates of the start/end of the segment, packed in an array of 4 doubles <tt>(x1,y1,x2,y2)</tt>.
	 * @param l3seg
	 * 3D coordinates of the start/end of the segment, packed in an array of 6 doubles <tt>(x1,y1,z1,x2,y2,z2)</tt>.
	 * @param lseguv
	 * 2D parameter-space coordinates of the start/end of the segment, packed in an array of 4 doubles <tt>(u1,v1,u2,v2)</tt>.
	 * @param lsegt
	 * start/end parameters of the segment (if on a model edge), packed in an array of 2 doubles <tt>(t_s,t_e)</tt>.
	 * @param tag
	 * <tt>void*</tt> from the corresponding <tt>PE_EDGE</tt>.
	 * @param fVisible
	 * <tt>TRUE</tt> if the announced segment is visible.
	 * @param tpar
	 * parametric length from start to end of segment (if on a silhouette curve).
	 * @param onsil
	 * <tt>TRUE</tt> if the segment is on a silhouette curve.
	 * @param join
	 * join as previous or next to the current segment.
	 */
    virtual void announce_2D_segment(
            double  *lseg,           // array of 4 doubles with start/end
                                     // 2D-coordinates
            double  *l3seg,          // array of 6 doubles with start/end
                                     // 3D-coordinates
		    double  *lseguv,         // array of 4 doubles with start/end
                                     // 2D-coordinates
            double  *lsegt,          // array of 2 doubles with start/end
                                     // tpars
            void    *tag,            // void* from the corresponding PE_EDGE
            logical fVisible,        // TRUE if segment is visible
            double	tpar,            // tpar from begin point to end point of segment
            logical onsil,           // seg on silhouette
            IHL_SEGMENT_JOIN_TYPE join);  // join as prev or next to cur_seg

	/**
     * Processes a 2D segment.
	 * <br>
	 * @param lseg
	 * 3D-coordinates of the start/end of the segment, packed in an array of 6 doubles <tt>(x1,y1,z1,x2,y2,z2)</tt>.
	 * @param tag
	 * <tt>void*</tt> from the corresponding <tt>PE_EDGE</tt>.
	 * @param fVisible
	 * <tt>TRUE</tt> if the announced segment is visible.
	 */
    virtual void announce_3D_segment(
            double  *lseg,           // array of 6 doubles with start/end
                                     // 3D-coordinates
            void    *tag,            // void* from the corresponding PE_EDGE
            logical fVisible );      // TRUE if segment is visible
};

/*! @} */
#endif
