#ifndef API_CONVEX_HULL_HXX
#define API_CONVEX_HULL_HXX

#include "api.hxx"
#include "dcl_hlc.h"

#include "body.hxx"
#include "position.hxx"
#include "lists.hxx"

#include "facet_options.hxx"


/**
* @file api_convex_hull.hxx
* \addtogroup hlc
* \addtogroup
* @{
*/

struct convex_hull_options_impl;

/**
* Options to be used in the generation of a convex hull. Used in the function @href <tt>api_convex_hull</tt>.
* If the surface and normal tolerances are set, the @href <tt>facet_options_precise</tt> will be used by the
* faceter to sample the input entities. Alternatively, the facet options can be given explicitly, as well.
* If neither are set by the user, the @href <tt>facet_options_visualization</tt> will be used with its default
* values.
*/
class DECL_HLC convex_hull_options : public ACIS_OBJECT
{
public:
	convex_hull_options();
	convex_hull_options(const convex_hull_options&);
	convex_hull_options(convex_hull_options&&);
	~convex_hull_options();
	convex_hull_options &operator=(const convex_hull_options&);
	convex_hull_options &operator=(convex_hull_options&&);

	/**
	* Set the surface and normal tolerances.
	* Internally the @href <tt>facet_options_precise</tt> will be used by the faceter to sample
	* the input entities, with the surface tolerance and normal tolerance set to the given values.
	* A value of zero means that the given tolerance is ignored.
	* The normal tolerance is given in degrees.
	* If <tt>set_facet_options</tt> was called previously, its effect will be cancelled.
	*/
	void set_surface_and_normal_tol(double surf_tol, double normal_tol);

	/**
	* Set the facet options. The faceter will use these options to generate a representative
	* set of points.
	* The facet options are deep copied and the copy is owned by <tt>convex_hull_options</tt>.
	* If <tt>set_surface_and_normal_tol</tt> was called previously, its effect will be cancelled.
	*/
	void set_facet_options(const facet_options &fo);

	/**
	* Enable the internal post-processing, which cleans up the geometry of the convex hull to be returned.
	* Default value is <tt>true</tt>.
	*/
	void enable_postprocessing(bool en);

	/**
	* Get the surface tolerance.
	*/
	double get_surface_tolerance() const;

	/**
	* Get the normal tolerance in degrees.
	*/
	double get_normal_tolerance() const;

	/**
	* Get the facet options. Can be NULL. The returned pointer is not owned by the caller and should not be deleted.
	*/
	facet_options *get_facet_options() const;

	/**
	* Returns whether post-processing is enabled. Post-processing cleans up the geometry of the convex hull to be returned.
	*/
	bool get_enable_postprocessing() const;

private:
	convex_hull_options_impl *impl;
};

/**
* Constructs the convex hull of a list of input entities.
* <br><br>
* <b>Role:</b> This API constructs the convex hull of a list of input entities.
* An entity in the list can be a <tt>BODY</tt> (open, solid, wire, mixed, multi-lump), <tt>LUMP</tt>, <tt>SHELL</tt>, <tt>FACE</tt>, <tt>WIRE</tt>, <tt>EDGE</tt> or <tt>VERTEX</tt>.
* <br><br>
* <b>Effect:</b> Changes model.
* <br><br>
* <b>Journal:</b> Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param[in] ents : list of entities
* @param[out] bdy : <tt>BODY</tt> representing the convex hull
* @param[in] cho : convex-hull options
* @param[in] ao : (optional) ACIS options
* @return The outcome object indicating success or failure.
*/
DECL_HLC
outcome api_convex_hull(ENTITY_LIST &ents, BODY *&bdy, const convex_hull_options &cho, AcisOptions *ao = NULL);

/**
* Constructs the convex hull of a set of input points.
* <br><br>
* <b>Role:</b> This API constructs the convex hull of a set of input points.
* <br><br>
* <b>Effect:</b> Changes model.
* <br><br>
* <b>Journal:</b> Unavailable
* <br><br>
* <b>Product(s):</b> 3D ACIS Modeler
* <br><br>
* @param[in] sz : number of input points
* @param[in] inp : array of input points
* @param[out] bdy : <tt>BODY</tt> representing the convex hull
* @param[in] enable_postprocessing : enable the internal post-processing (clean up the geometry of the convex hull to be returned)
* @param[in] ao : (optional) ACIS options
* @return The outcome object indicating success or failure.
*/
DECL_HLC
outcome api_convex_hull(unsigned sz, const SPAposition inp[], BODY *&bdy, bool enable_postprocessing = true, AcisOptions *ao = NULL);

/** @} */

#endif
