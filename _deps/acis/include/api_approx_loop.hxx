/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef api_approx_loop_hxx
#define api_approx_loop_hxx

#include "bs2c_def.hxx"
#include "api.hxx"
#include "dcl_kern.h"
class LOOP;
class COEDGE;
class VERTEX;

class DECL_KERN SPA_LOOP_approx_options : public ACIS_OBJECT
{
public:
	/**
	* Fit tolerance is the max distance between
	* give bs2 curve and the composition of the edge
	* geometry and the closest point map.
	*
	* In the neighborhood of TVERTEXs this tolerance is not 
	* well defined.
	**/
	void set_fit_tolerance(double tol);
	/**
	* Gets the fit tolerance.  See <tt>set_fit_tolerance</tt> for explanation of fit tolerance.
	**/
	double get_fit_tolerance() const;

	/**
	* Vertex tolerance is the max distance in 3d between
	* the end of a bs2 curve in the loop and the start of the next.
	*
	* We interpret distances in 2d as distances in 3d usin surface derivatives.
	*
	* Default vertex tolerance is <tt>SPAresabs</tt>.
	**/
	void set_vertex_tolerance(double tol);
	/**
	* Gets the vertex tolerance.  See <tt>set_vertex_tolerance</tt> for explanation of fit tolerance.
	**/
	double get_vertex_tolerance() const;


	/**
	* The API may extend surfaces or make other simple modifications to the 
	* geometry being approximated.  This allows the API to give a better answer,
	* but in workflows where whole faces are being approximated, loop approx
	* should be used before querying the face for a bs3_surface.
	**/
	void set_allow_geometry_modification(logical do_geom_mod);
	logical get_allow_geometry_modification() const;


	SPA_LOOP_approx_options();
	SPA_LOOP_approx_options(SPA_LOOP_approx_options const&);
	SPA_LOOP_approx_options& operator=(SPA_LOOP_approx_options const&);
	~SPA_LOOP_approx_options();
	class Impl;
	Impl* get_impl();
	Impl const* get_impl() const;

private:
	Impl* impl;
};

class loop_approx_output_handle;
DECL_KERN void delete_loop_approx_handle(loop_approx_output_handle* h);


DECL_KERN outcome api_approx(LOOP* loop, loop_approx_output_handle*& handle, SPA_LOOP_approx_options* laopts = NULL, AcisOptions* ao = NULL);

class DECL_KERN loop_approx_queries : public ACIS_OBJECT
{
	loop_approx_output_handle* handle;
public:
	double tolerance_achieved() const;
	/**
	* A loop consists of some number of segments, each is associated with a bs2_curve.
	* The segments form a closed path in uv space.  Each segment is also associated with some
	* entities (usually COEDGE, but sometimes from a VERTEX a singularity
	**/
	int number_of_segments() const;
	/**
	* bs2 curve is ownded by the answer object.  If you want your own copy use bs2_curve_copy.
	**/
	bs2_curve get_segment_bs2( int ii) const;
	/**
	* Returns the loop this object is approximating
	**/
	LOOP* loop() const;
	/**
	* Returns the entity in this loop which corresponds to this entity (either a COEDGE or VERTEX
	* at a singularity
	**/
	ENTITY* get_segment_entity( int ii ) const;

	/**
	* Returns a 2d wire body made from the loop approximations.  This is for visualization purposes, not geometry construction.
	* LIMITATION: The algorithm used to make the wire body assumes there are no nonmanifold vertices/branches in the loop (except possibly at the seam).
	* If nonmanifold vertices occur that aren't on the seam, the output of this method will not check
	**/
	outcome wire_2d(BODY*& out_2d_wire);

	/**
	* Returns a 3d wire body made from the loop approximations.  It skips any segments that would produce degenerate edges.  The output
	* of this command is intended for visualization only.
	*
	* LIMITATION: If nonmanifold or branch points occur in the loop (even at a seam) the output body will not check well.
	**/
	outcome wire_3d(BODY*& out_3d_wire);

	/**
	* Returns the max gap between COEDGE/VERTEX geometry and the bs2_curve geometry
	* of the segments
	**/
	double tolerance_achieved( int ii ) const;

	/**
	* @nodoc
	**/
	~loop_approx_queries();
	/**
	* @nodoc
	**/
	loop_approx_queries(loop_approx_output_handle* ihandle);
	/**
	* @nodoc
	**/
	loop_approx_queries(loop_approx_queries const& other);
	/**
	* @nodoc
	**/
	loop_approx_queries& operator=(loop_approx_queries const& other);
};


#endif
