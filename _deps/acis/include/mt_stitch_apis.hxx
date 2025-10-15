/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef MT_STITCH_APIS_HXX
#define MT_STITCH_APIS_HXX

#include "dcl_stitch.h"

class mt_stitch_options;
class stitch_input_handle;
class stitch_output_handle;

/**
* @file mt_stitch_apis.hxx
* \addtogroup STITCHAPI
*
* @{
*/

/**
* The entry API that creates the stitch input. 
* @par Technical Articles
* <i>[Creating the Stitching Input](http://doc.spatial.com/articles/m/u/l/Multithreaded_Stitching_8e62.html#Creating_the_Stitching_Input)</i>
* @par Role
* Given an ENTITY_LIST of BODYs and/or FACEs (which at this point can still be
* faces w/o owning BODY) a stitch_input_handle instance is created. 
* That serves as input argument for @href api_stitch_manage_coins, 
* @href api_stitch_preview, and @href api_stitch.
* Optionally, the behavior can be controlled through mt_stitch_options:
* relevant for this API is the max_stitch_tol_option.
* @par Journal
* Not available
* @par Errors
* <tt>INVALID_STITCH_HANDLE</tt>
* @par Effects
* Does not change model
* @par Products
* 3D InterOp, 3D ACIS Polyhedral 
* @param[in] bodies ENTITY_LIST of BODYs or FACEs
* @param[out] sih pointer to stitch_input_handle
* @param[in] sop pointer to mt_stitch_options (relevant is max_stitch_tol_option)
* @param[in] aop pointer to AcisOptions
* @return outcome provides information if the API succeeded
*/
DECL_STITCH
outcome api_stitch_make_input(
ENTITY_LIST& bodies,
stitch_input_handle* &sih,
mt_stitch_options* sop = NULL,
AcisOptions* aop = NULL);

/**
* \ingroup STITCH_CONTAINERS
* The container class for the stitch preview queries. 
* @par Technical Article
* <i>[Real Time Stitching Preview](http://doc.spatial.com/articles/m/u/l/Multithreaded_Stitching_8e62.html#Real_Time_Stitching_Preview)</i>
* @par Role
* It needs to be passed to @href api_stitch_preview to be created and filled 
* with data. None of its member functions throw exceptions. The class is
* designed as an <i>abstract base class</i> that cannot be
* instantiated, copied or assigned, it can only be deleted. This is done in
* order to hide implementation. Internally the data is accessed through public
* inheritance by overwrite the pure virtual member functions.
*/
class DECL_STITCH mt_stitch_preview_container
	: public ACIS_OBJECT
{
protected:
	mt_stitch_preview_container();
	mt_stitch_preview_container(const mt_stitch_preview_container&);
	mt_stitch_preview_container& operator=(const mt_stitch_preview_container&);
public:
	virtual ~mt_stitch_preview_container() = 0;
	/**
	* The public interface to get the amount of EDGE pairs.
	* @return the number of EDGE pairs
	*/
	virtual size_t size() const = 0;
	/**
	* The public interface to get the Hausdorff distance of the <tt>idx</tt>-th
	* EDGE pair. The EDGE pairs are sorted by their increasing Hausdorff 
	* distance.
	* @param[in] idx the index of the EDGE pair
	* @return the Hausdorff distance
	*/
	virtual double get_hausdorff_distance(const size_t idx) const = 0;
	/**
	* The public interface to get the <tt>idx</tt>-th EDGE pair. The EDGE pairs
	* are sorted by their increasing Hausdorff distance.
	* @param[in] idx the index of the EDGE pair
	* @param[out] edg0 a pointer to the first EDGE
	* @param[out] edg1 a pointer to the second EDGE
	*/
	virtual void get_edge_pair(const size_t idx, EDGE* &edg0, EDGE* &edg1) const = 0;
};

/**
* The API for a real time stitching preview. 
* @par Technical Articles
* <i>[Real Time Stitching Preview](http://doc.spatial.com/articles/m/u/l/Multithreaded_Stitching_8e62.html#Real_Time_Stitching_Preview)</i>
* @par Role
* Given a pointer to a stitch_input_handle fill the provided 
* mt_stitch_preview_container object, which subsequently can be used for
* querying preview information.
* @par Journal
* Available
* @par Errors
* <tt>INVALID_STITCH_HANDLE</tt>
* @par Effects
* ENTITYs given to the stitch_input_handle that are FACEs without owning 
* BODYs become BODYs. If any of the BODYs have a transform it is applied
* to all underlying geometries.
* @par Products
* 3D InterOp, 3D ACIS Polyhedral 
* @param[in] sih pointer to stitch_input_handle
* @param[out] cntnr pointer to mt_stitch_preview_container
* @param[in] aop pointer to AcisOptions
* @return the outcome which provides information if the API succeeded
*/
DECL_STITCH
outcome api_stitch_preview(
stitch_input_handle* sih,
mt_stitch_preview_container* &cntnr,
AcisOptions* aop = NULL);

/**
* \ingroup STITCH_CONTAINERS
* The container class for coincident face cluster queries.
* @par Technical Article
* <i>[Managing Coincident Faces](http://doc.spatial.com/articles/m/u/l/Multithreaded_Stitching_8e62.html#Managing_Coincident_Faces)</i>
* @par Role
* It needs to be passed to @href api_stitch_manage_coins to be created and 
* filled with data. None of its member functions throw exceptions.
* The class is designed as an <i>abstract base class</i> that cannot be
* instantiated, copied or assigned, it can only be deleted. This is done in
* order to hide implementation. Internally the data is accessed through public
* inheritance by overwrite the pure virtual member functions.
*/
class DECL_STITCH mt_stitch_manage_coins_container
	: public ACIS_OBJECT
{
protected:
	mt_stitch_manage_coins_container();
	mt_stitch_manage_coins_container(const mt_stitch_manage_coins_container&);
	mt_stitch_manage_coins_container& operator=(const mt_stitch_manage_coins_container&);

public:
	virtual ~mt_stitch_manage_coins_container() = 0;

	/**
	* The public interface to get the amount of coincident FACE pairs.
	* @return the number of coincident FACE pairs
	*/
	virtual size_t size() const = 0;

	/**
	* The public interface to get the <tt>idx</tt>-th coincident FACE pair.
	* @param[in] idx the index of the coincident FACE pair
	* @param[out] fac0 a pointer to the first FACE
	* @param[out] fac1 a pointer to the second FACE
	*/
	virtual void get_coincident_face_pair(const size_t idx, FACE* &fac0, FACE* &fac1) const = 0;
};

/**
* The API that manages coincident faces. 
* @par Technical Articles
* <i>[Managing Coincident Faces](http://doc.spatial.com/articles/m/u/l/Multithreaded_Stitching_8e62.html#Managing_Coincident_Faces)</i>
* @par Role
* Given a pointer to a stitch_input_handle manage coincident face clusters. 
* Optionally, the behavior can be controlled through mt_stitch_options:
* relevant for this API is the manage_coins_option.
* @par Journal
* Available
* @par Errors
* <tt>INVALID_STITCH_HANDLE</tt> \n
* <tt>IMPROPER_STITCH_OPTION</tt>
* @par Effects
* ENTITYs given to the stitch_input_handle that are FACEs without owning
* BODYs become BODYs. If any of the BODYs have a transform it is applied
* to all underlying geometries.
* @par Products
* 3D InterOp, 3D ACIS Polyhedral 
* @param[in] sih pointer to stitch_input_handle
* @param[out] cntnr pointer to mt_stitch_manage_coins_container
* @param[in] sop pointer to mt_stitch_options (relevant is manage_coins_option)
* @param[in] aop pointer to AcisOptions
* @return the outcome which provides information if the API succeeded
*/
DECL_STITCH
outcome api_stitch_manage_coins(
stitch_input_handle* sih,
mt_stitch_manage_coins_container* &cntnr,
mt_stitch_options* sop = NULL,
AcisOptions* aop = NULL);

/**
* The core API responsible for the actual stitching.
* @par Technical Articles
* <i>[Core Stitching API](http://doc.spatial.com/articles/m/u/l/Multithreaded_Stitching_8e62.html#Core_API)</i>
* @par Role 
* Given an instance of an pointer to a stitch_input_handle stitch whatever can be stitched.
* Optionally, the behavior can be controlled through mt_stitch_options:
* relevant for this API are the max_stitch_tol_option, the validation_option, 
* and the heal_option. The API returns an ENTITY_LIST of bodies and a pointer
* to a stitch_output_handle. That serves as input for @href api_stitch_query.
* The API supports <i>[failsafe behavior](http://doc.spatial.com/articles/m/u/l/Multithreaded_Stitching_8e62.html#Failsafe_Stitching)</i>.
* @par Journal
* Available
* @par Errors
* <tt>IMPROPER_STITCH_OPTION</tt> \n
* <tt>INVALID_STITCH_HANDLE</tt> \n
* <tt>NOTHING_TO_STITCH</tt> \n
* <tt>INVALID_STITCH_MAX_TOL</tt>
* @par Effects
* Changes topology and geometry of the model
* @par Products
* 3D InterOp, 3D ACIS Polyhedral 
* @param[in,out] sih pointer to stitch_input_handle
* @param[out] soh pointer to stitch_output_handle
* @param[out] bodies ENTITY_LIST containing stitched bodies
* @param[in] sop pointer to mt_stitch_options (relevant are max_stitch_tol_option, validation_option, and heal_option)
* @param[in] aop pointer to AcisOptions
* @return the outcome which provides information if the API succeeded
*/
DECL_STITCH
outcome api_stitch(
stitch_input_handle* sih,
stitch_output_handle* &soh,
ENTITY_LIST& bodies,
mt_stitch_options* sop = NULL,
AcisOptions* aop = NULL);

/**
* \ingroup STITCH_CONTAINERS
* The container class for quering data from stiched bodies.
* @par Technical Article
* <i>[Result Queries](http://doc.spatial.com/articles/m/u/l/Multithreaded_Stitching_8e62.html#Result_Queries)</i>
* @par Role
* It needs to be passed to @href api_stitch_query to be created and filled with
* data. None of its member functions throw exceptions.
* The class is designed as an <i>abstract base class</i> that cannot be
* instantiated, copied or assigned, it can only be deleted. This is done in
* order to hide implementation. Internally the data is accessed through public
* inheritance by overwrite the pure virtual member functions.
*/

class DECL_STITCH mt_stitch_query_container
	: public ACIS_OBJECT
{
protected:
	mt_stitch_query_container();
	mt_stitch_query_container(const mt_stitch_query_container&);
	mt_stitch_query_container& operator=(const mt_stitch_query_container&);

public:
	virtual ~mt_stitch_query_container() = 0;
	/**
	* The public interface to get the maximum stitched tolerance, i.e., Hausdorff 
	* distance of the farthest apart but still stitched EDGE pair.
	* @return the maximum stitched tolerance
	*/
	virtual double get_max_stitch_tol() const = 0;
	/**
	* The public interface to get all remaining free/unstitched EDGE s.
	* @param[out] el_free an ENTITY_LIST containing all remaining free EDGE s
	*/
	virtual void get_free_edges(ENTITY_LIST& el_free) const = 0;
	/**
	* The public interface to get all stitched EDGE s.
	* @param[out] el_stch an ENTITY_LIST containing all stitched EDGE s
	*/
	virtual void get_stitched_edges(ENTITY_LIST& el_stch) const = 0;
	/**
	* The public interface to get all newly created short (shorter than the
	* provided maximum stitching tolerance) EDGE s.
	* @param[out] el_shrt an ENTITY_LIST containing all newly create short EDGE s
	*/
	virtual void get_new_short_edges(ENTITY_LIST& el_shrt) const = 0;
};

/**
* The API for querying data from stitched bodies. 
* @par Technical Article
* <i>[Result Queries](http://doc.spatial.com/articles/m/u/l/Multithreaded_Stitching_8e62.html#Result_Queries)</i>
* @par Role
* It allocates a pointer to mt_stitch_query_container and fills it with data.
* @par Journal
* Not available
* @par Errors
* <tt>INVALID_STITCH_HANDLE</tt>
* @par Effects
* Does not change model
* @par Products
* 3D InterOp, 3D ACIS Polyhedral 
* @param[in] soh pointer to stitch_output_handle
* @param[out] cntnr pointer to mt_stitch_query_container
* @param[in] aop pointer to AcisOptions
* @return the outcome which provides information if the API succeeded
*/
DECL_STITCH
outcome api_stitch_query(
stitch_output_handle* soh,
mt_stitch_query_container* &cntnr,
AcisOptions* aop = NULL);

/** @} */

#endif // MT_STITCH_APIS_HXX
