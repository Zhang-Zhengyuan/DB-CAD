/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
/*******************************************************************/
//  Modification history
//  DM  28May08   Added grouping for documentation
//  DM  05Dec07   Created file
/*******************************************************************/
// User silhouette options class definition. This object is required by
// api_silhouette_edges and sg_make_silhouette_edges

#ifndef SIL_OPTS_HFILE
#define SIL_OPTS_HFILE

#include "acis.hxx"
#include "dcl_intr.h"

/**
* @file sil_opts.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */


/** \addtogroup INTRAPISILHOUETTE
 *  \brief Declared at <sil_opts.hxx>, SPAintr
 *  @{
 */

/**
 * Represents common data and functionality for silhouette options.
 * <br>
 * <b>Role:</b> <tt>silhouette_options</tt> is the generic class to be used
 * in <tt>api_silhouette_edges</tt>.
 * <br><br>
 * @see api_silhouette_edges
 */
class DECL_INTR silhouette_options : public ACIS_OBJECT
{
protected:

	double m_sil_tolerance;

public:

    /**
	 * Constructor.
	 * <br><br>
	 * <b>Role:</b> Creates a silhouette_options object.
	 * <br><br>
	 */
	silhouette_options();

    /**
	 * Returns the value of the silhouette tolerance.
	 */
	double get_sil_tolerance() const;

    /**
	 * Sets the silhouette tolerance.
	 */
	void set_sil_tolerance(double tol);
};

/** @} */

#endif // SIL_OPTS_HFILE
