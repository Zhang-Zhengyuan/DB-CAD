/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef SPA_SAMPLE_EDGES_HXX
#define SPA_SAMPLE_EDGES_HXX


#include "dcl_kern.h"
#include "logical.h"
#include "mmgr.hxx"

class EDGE;
class SPAposition;
class AcisOptions;
class outcome;
class ENTITY_LIST;
class SPAposition_vector;
/**
 * @file sample_edges.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISQUERIES
 *
 * @{
 */



/**
* Compute equal equal turning samples from a list of <tt>EDGE</tt>s. The turning is computed in magnitude - the direction is not considered. 
* Non-G1 discontinuities in or between the edges can introduce inaccuracies, which are bounded by the size of the non-G1 angular jumps.
* The sample spacing should be larger than the largest non-G1 angular jump - visually sharp edge connections should not be included.
* The input edges must not all be geometrically linar (flat).
* <br><br>
* <b>Effect:</b> Read-only.
* <br><br>
* @param edges
* input EDGEs,
* @param num_samp
* input number of samples wanted; must be >= 2.
* @param points
* returned SPApositions. Note: before calling, points must be allocated of size num_samp. The caller owns points, and must delete at the end of its lifetime.
* @param ao
* ACIS options such as versioning and journaling.
**/

DECL_KERN outcome 
api_equal_turning_samples_from_edges(ENTITY_LIST const& edges, int num_samp, SPAposition* points, AcisOptions* ao=NULL);


class SPA_sample_edges_options_impl;

/**
 * 	Provides a data structure for the sampling of contiguous, unbranched edges. It is to be used in the function @href api_sample_edges.
 * For more details, see the Technical Article <i>[Sampling Edges](http://doc.spatial.com/articles/s/a/m/Sampling_Edges_e577.html)</i>
 */

class DECL_KERN SPA_sample_edges_options : public ACIS_OBJECT
{
public:
/**
 * Type of sampling operation.
  * @param numsamp_arc_len
  * User-specified number of samples, with equal arc length between samples.
  * @param numsamp_equal_turning
  * User-specified number of samples, with equal turning between samples.
  * @param max_len
  * User-specified arc length between samples, except the last two samples may have less.
  * @param max_turning
  * User-specified turning between samples, except the last two samples may have less. Turning is in units of radians.
  * @param max_len_and_turning
  * User-specified turning and arc length between samples - samples will be spaced with max_len arc length, or max_turning turning between; except the last two samples may have less. Turning is in units of radians.
*/
	enum method { numsamp_arc_len = 0,
					numsamp_equal_turning = 1,
					max_len = 2,
					max_turning = 3,
					max_len_and_turning = 4 };
private:
	SPA_sample_edges_options_impl* m_impl;
public:
	SPA_sample_edges_options();
	~SPA_sample_edges_options();
	SPA_sample_edges_options(SPA_sample_edges_options const& other);
	SPA_sample_edges_options& operator=(SPA_sample_edges_options const& other);
   /**
    * Sets the options for sampling with the method <tt>numsamp_arc_len</tt>
    * <b>Role:</b> Sets the SPA_edges_sample_options to sample such that there is equal arc length of the curves between samples, with the
	* user-specified total number of samples.
    * @param nSamp
    * The total number of samples returned.
    */
	void set_as_num_samp_arc_len(int nSamp);
   /**
    * Sets the options for sampling with the method <tt>numsamp_equal_turning</tt>
    * <b>Role:</b> Sets the SPA_edges_sample_options to sample such that there is equal turning of the curves between samples, with the
	* user-specified total number of samples.
    * @param nSamp
    * The total number of samples returned.
    */
	void set_as_num_samp_equal_turning(int nSamp);
   /**
    * Sets the options for sampling with the method <tt>max_len</tt>
    * <b>Role:</b> Sets the SPA_edges_sample_options to sample such that there is user-specified arc length of the curves between samples.
    * @param maxLen
    * Between samples, the curves will be have equal arc length - except the arc length may be shorter between the last two samples.
    */
	void set_as_max_len(double maxLen);
   /**
    * Sets the options for sampling with the method <tt>max_turning</tt>
    * <b>Role:</b> Sets the SPA_edges_sample_options to sample such that there is users-specified equal turning of the curves between samples.
    * @param maxTurning
    * Radians; between samples, the curves will be have equal turning - except the turning may turn less between the last two samples.
    */
	void set_as_max_turning(double maxTurning);
   /**
    * Sets the options for sampling with the method <tt>max_len_and_turning</tt>
    * <b>Role:</b> Sets the SPA_edges_sample_options to sample such that between samples, either the arc length is user-specified, or the turning is user-specified, and neither is greater than the user-specifications.
	* Except the last two samples may have both less arc length and less turning.
    * @param maxLen
    * Upper bound for the arc length between samples - except the curves may be shorter between the last two samples.
    * @param maxTurning
    * Radians; upper bound for the turning between samples - except the curves may turn less between the last two samples.
    */
	void set_as_max_len_and_turning(double maxLen, double maxTurning);
   /**
    * Gets the method to be used for sampling, returning an enum <tt>SPA_edges_sample_options::method</tt>.
    */
	method get_method() const;
   /**
    * Gets the number of samples to get; this is meaningful only if using the methods <tt>numsamp_arc_len</tt> or <tt>numsamp_equal_turning</tt>.
    */
	int get_num_samp() const;
   /**
    * Gets the max arc length between samples; this is meaningful only if using the methods <tt>max_len</tt> or <tt>max_len_and_turning</tt>.
    */	
	double get_maxLen() const;
   /**
    * Gets the max turning between samples; this is meaningful only if using the methods <tt>max_turning</tt> or <tt>max_len_and_turning</tt>.
    */	
	double get_maxTurning() const;

	/**
	* @nodoc
	**/
	SPA_sample_edges_options_impl* get_impl();
};

/**
* Compute samples from a list of <tt>EDGE</tt>s with user-specified maximum distance and turning angle. 
* The turning is computed in magnitude - the direction is not considered. 
* Non-G1 discontinuities in or between the edges will be skipped in the turning computation.
* For more details, see the Technical Article <i>[Sampling Edges](http://doc.spatial.com/articles/s/a/m/Sampling_Edges_e577.html)</i>
* <br><br>
* <b>Effect:</b> Read-only.
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* @param edges
* input EDGEs,
* @param points
* returned SPAposition_vector. 
* @param eso
* SPA_sample_edges_options to set parameters such as max length and max turning angle
* @param ao
* ACIS options such as versioning and journaling.
**/

DECL_KERN outcome 
api_sample_edges(ENTITY_LIST const& edges, SPAposition_vector& points, SPA_sample_edges_options* eso=NULL, AcisOptions* ao=NULL);

/** @} */
#endif
