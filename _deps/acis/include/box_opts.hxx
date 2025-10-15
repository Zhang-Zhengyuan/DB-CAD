/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// User boxing options class definition. This object is required by
// api_get_entity_box, whenever more precision is needed.

#ifndef BOX_OPTS_HFILE
#define BOX_OPTS_HFILE

#include "acis.hxx"
#include "api.hxx"
#include "dcl_kern.h"
class WCS;
/**
 * \defgroup ACISBOXING Entity Bounding Box
 * brief Determine an axis-aligned box that bounds an ENTITY
 * \ingroup MODELANALYSIS
 *
 */

/**
 * @file box_opts.hxx
 * @CAA2Level L1
 * @CAA2Usage U2
 * \addtogroup ACISBOXING
 *
 * @{
 */

// This enumerated type encodes the boxing mode.
/**
 * Specifies mode of boxing behavior.
 * <br>
 * <b>Role:</b> <tt>SPAboxing_mode</tt> is an enumerated type used by @href SPAboxing_options to specify 
 * the size and mode of the box returned by @href api_get_entity_box.
 * <br><br>
 * By default the only guarantee in the size of the box for an entity returned 
 * from @href api_get_entity_box is that it will enclose the input entities. How tightly the box 
 * fits depends on the algorithm used to calculate the box and this also affects the time 
 * taken to do the calculation.
 * <br><br>
 * Entity boxes are cached in ACIS to optimize performance. However, there is no guarantee 
 * about which algorithm is used to calculate a cached box, only that the box encloses the 
 * entity. This criterion is enough to support internal ACIS functionality.
 * <br><br>
 * In order to allow applications to have some more control over which algorithm is used in 
 * the box calculation for @href api_get_entity_box, the @href SPAboxing_mode enumerator is provided.  
 * The "reproducible" modes are ones where the value returned will always be the same, 
 * that is, the mode chooses the algorithm to calculate the box. The "non-reproducible" modes 
 * allow ACIS to use internal optimizations so whilst the value is guaranteed to contain the 
 * entity, it may be different depending on what is in the cache or other circumstances.
 * <br>
 * @param SPAboxing_tight
 * Computes a reproducible tight box (spline, sphere surfaces). No caching. 
 * @param SPAboxing_loose
 * Computes a reproducible loose box (spline, sphere surfaces). No caching.
 * @param SPAboxing_quick
 * Computes a non-reproducible quick box (returns any box available). No caching.
 * @param SPAboxing_default. 
 * Computes and caches a non-reproducible box (tight for torus, sphere by default).
 * @param SPAboxing_default_pre_R12
 * Computes a box as it used to in ACIS prior to release 12. No caching.
 * @param SPAboxing_exact
 * Computes a reproducible exact box. In other words, this box type shrink-wraps the body; 
 * each face of the box touches the body. No caching.
 * <br><br>
 * @see SPAboxing_options, api_get_entity_box
 **/

enum SPAboxing_mode 
{
    SPAboxing_tight,
    SPAboxing_loose,
    SPAboxing_quick, 
    SPAboxing_default, 
    SPAboxing_default_pre_R12,
    SPAboxing_exact
};

/**
 * Represents common data and functionality for all boxing options.
 * <br>
 * <b>Role:</b> <tt>SPAboxing_options</tt> is the generic class to be used
 * in <tt>api_get_entity_box</tt>. It carries information regarding the coordinate
 * system of the model, and the available boxing modes. These modes are specified 
 * using one of the enumerated types defined in <tt>SPAboxing_mode</tt>.
 * <br><br>
 * @see api_get_entity_box, SPAboxing_mode
 */

class DECL_KERN SPAboxing_options : public ACIS_OBJECT
{
private:
    SPAboxing_mode  m_Current_boxing_mode;
    WCS*            m_pCurrent_WCS;
public:
     /**
	 * Constructor.
	 * <br><br>
	 * <b>Role:</b> Creates a SPAboxing_options object.
	 * <br><br>
	 * @param wcs
	 * Coordinate system of the model.
     * @param box_mode
	 * Enumerated boxing mode.
	 */
    SPAboxing_options(WCS *wcs = NULL, SPAboxing_mode box_mode = SPAboxing_default);
    SPAboxing_options(const SPAboxing_options &box_opts);
    ~SPAboxing_options(void);
/**
 * @nodoc
 */
	SPAboxing_options& operator=(SPAboxing_options const& old);

     /**
	 * Retrieves the current boxing mode.
	 * <br><br>
	 * <b>Role:</b> Meant only for ACIS internal usage mainly.
	 */
    SPAboxing_mode get_mode(void) const { return m_Current_boxing_mode; }
     /**
	 * Establishing a boxing mode.
	 * <br><br>
	 * <b>Role:</b> Sets boxing mode to specified value.
	 * <br><br>
	 * @param box_mode
	 * Enumerated boxing mode.
	 */
    void set_mode(SPAboxing_mode box_mode);
     /**
	 * Establishing a boxing mode.
	 * <br><br>
	 * <b>Role:</b> Sets mode to default value (this is better than a default argument, as it can be versioned)
	 * <br><br>
	 * @param box_mode
	 * Enumerated boxing mode.
	 */
    void set_mode(void); 
      /**
	 * Returns a pointer to the current coordinate system, set for boxing.
	 * <br><br>
	 * <b>Role:</b> Returns the coordiante system set by the user for boxing
	 */
    WCS* get_wcs(void) const { return m_pCurrent_WCS; }
     /**
	 * Sets the coordinate system for boxing.
	 * <br><br>
	 * <b>Role:</b> Sets option object to refer to wcs. Boxing options
     * does NOT own a wcs. It is the client's responsability
     * to ensure that the code uses a valid wcs pointer,
     * whenever an object of boxing_options is used.
	 * <br><br>
	 * @param box_mode
	 * Enumerated boxing mode.
	 */
    void set_wcs(WCS* wcs = NULL);  
};
/** @} */
#endif   // BOX_OPTS_HFILE

