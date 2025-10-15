/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef SPAPOSITION_CLOUD_HXX
#define SPAPOSITION_CLOUD_HXX

/**
* @file SPAposition_cloud.hxx
* @CAA2Level L1
* @CAA2Usage U1
*! \addtogroup FCTAPI
*  \brief Declared at <SPAposition_cloud.hxx>
*  @{
*/


#include "base.hxx"
#include "position.hxx"
#include "box.hxx"
#include "dcl_base.h"
#include "stdio.h"
#include "transf.hxx"
#include "container_utils.hxx"
class SPAposition_cloud_impl;
class SPAposition_subcloud_impl;
class SPAposition_cloud;
class SPAposition_ptr_const_alias_array; //to be removed.

/**
* SPAposition_cloud_iterator allows you to get the points from a SPAposition_cloud or SPAposition_subcloud object.
*
* Unlike many iterators, it iterates over the points in batches: this is a performance optimization.  There are
* layers of function calls between SPAposition_cloud and the buffer of output points.  By copying points in larger batches,
* the impact of the intervening function calls amortized over all calls is minimized.
*
**/
// Typical code for iterating over this is
//
// SPAposition_cloud_iterator itr=cloud.get_iterator();
// SPAposition buffer[16];
// int num_pts =0;
// for( itr.init(); num_pts=itr.next_points( 16, buffer ); )
//{
//	// do something with this batch of points
//}
//
// It is ok (but not especially useful for the user to change the buffer size between calls to the function, 
// or alternate between calls to next_points and next_coordinates.
//

class DECL_BASE SPAposition_cloud_iterator: public ACIS_OBJECT
{
public:
	/**
	* set iterator to point at the first SPAposition in the cloud.
	**/
	void init();

	/**
	* Return value indicates number of points returned
	*
	* Writes at most max_num_points SPApositions into the array out_points;
	* the client is responsible for making sure the array out_points is big enough to
	* hold the requested number of points.
	**/
	int next_points( int max_num_points, SPAposition* out_points );

	/**
	* Return value indicates number of points returned (num points returned = 3*(num coords returned) )
	*
	* Write at most 3*max_num_points floats into the array out_coords;
	* the client is responsible for making sure the array out_coords is big enough to
	* hold the requested number of points. (i.e., out_coords must have at least 3*max_num_points.)
	**/
	int next_coordinates( int max_num_points, float* out_coords );

	/**
	* Default constructor.
	**/
	SPAposition_cloud_iterator();

	/**
	* copy constructor.
	**/
	SPAposition_cloud_iterator( const SPAposition_cloud_iterator&);

	/**
	* assignment operator.
	**/
	SPAposition_cloud_iterator& operator=( const SPAposition_cloud_iterator&);

	/**
	* @nodoc
	**/
	SPAposition_subcloud_impl const* get_impl();

	/**
	*@nodoc
	**/
	SPAposition_cloud_iterator( SPAposition_subcloud_impl const* );

	/**
	* @nodoc
	**/
	~SPAposition_cloud_iterator();

private:
	SPAposition_subcloud_impl const* m_impl;
	int m_idx;
};

/**
* SPAposition_subcloud represents a subset of a SPAposition_cloud.  Each such subset knows its
* box and can access its points.  
*
* The copy constructor and assignment operator for subclouds are private, because your code should
* not copy subcloud objects.  Customer code should only refer to subclouds by reference within
* the method examine(SPAposition_subcloud const& subcloud) of a derived class of SPAposition_cloud_selector.
**/

class DECL_BASE SPAposition_subcloud:public ACIS_OBJECT
{
public:

	/**
	* Gets the box for this subcloud
	**/
	SPAbox get_box() const;

	/**
	* Gets an iterator for this subcloud.
	**/
	SPAposition_cloud_iterator get_iterator() const;

	int size() const;

	/**
	* @nodoc
	**/
	SPAposition_subcloud( SPAposition_subcloud_impl*);
	/**
	* @nodoc
	**/
	SPAposition_subcloud_impl* get_impl();
private:
	SPAposition_subcloud( SPAposition_subcloud const&); // do not copy
	SPAposition_subcloud& operator=(SPAposition_subcloud const&); // do not copy
	SPAposition_subcloud_impl* m_impl;
};

/**
* SPAposition_cloud objects are organized into subclouds.  The SPAposition_cloud_selector allows 
* customer code to write customized searches over SPAposition_clouds.  
*
* This interface includes one method to override: examine. 
* The value you return from examine tells the algorithm whether to break the subcloud into smaller parts
* for closer examination( return value=TRUE means do break the subcloud into further parts).
**/
//*
//* A typical implementor of this interface will have an examine function which looks roughly like the following
//*
//* const int small_num_pts=16;//should use some small number of points, 16 is arbitrary but a reasonable value.
//* logical examine( SPAposition_subcloud const& subcloud )
//*{
//*	if( subcloud.size() > small_num_pts ) 
//*	{
//*		SPAbox box = subcloud.get_box();
//*		// use box and region of space you are trying to select to determine whether you need to look
//*		// at smaller subsets of this subcloud, return answer from here
//*	}
//*	else
//*	{
//*		SPAposition buffer[16];
//*		SPAposition_cloud_iterator itr=subcloud.get_iterator();
//*		int num_pts=itr.next_points( small_num_pts, buffer);
//*
//*		for( int ii=0; ii<num_pts; ++ii )
//*		{
//*			// do brute force selection algorithm on this small subset.
//*		}
//*
//*		return FALSE;
//*	}
//*}
class DECL_BASE SPAposition_cloud_selector: public ACIS_OBJECT
{
public:
	/**
	* This virtual method is called repeatly on parts of the SPAposition_cloud by the SPAposition_cloud::select method.
	*
	* The return value indicates whether this class would like to examine the constituent pieces of the given cloud.
	**/
	virtual logical examine( SPAposition_subcloud const& cloud)=0;

	// this is a base class so we need a virtual destructor.
	/**
	* @nodoc
	**/
	virtual ~SPAposition_cloud_selector();
};


/**
* SPAposition_cloud is a class for geometrically querying large sets of points.  The querying 
* is handled through the select methods.  The cloud is organized into subclouds: each subcloud
* knows its box and how to get to its points.  The select methods provide an interface for querying
* through the subclouds.
*
* The SPAposition_cloud_iterator class allows access to batches of points from this cloud.
**/
class DECL_BASE SPAposition_cloud :public ACIS_OBJECT
{
public:
	/**
	* Constructs point cloud from user point array.  Creates its own copy of the points array.
	**/
	SPAposition_cloud( int npts, SPAposition const* pts);

	//SPAposition_cloud( int npts, SPAposition const* const* ptr_array );
	//SPAposition_cloud( const char* csv_filename );

	/**
	* Select points from this cloud using a selector.  Please see the documentation class for
	* more details regarding how to write a selector.
	**/
	void select( SPAposition_cloud_selector& selector) const;

	/**
	* Get iterator to iterate over points in the cloud.
	**/
	SPAposition_cloud_iterator get_iterator() const;

	/**
	* returns number of points in cloud
	**/
	int size() const;

	/**
	* Applies a transform to the cloud and reorganizes it.
	**/
	void apply_transform( SPAtransf const& trans );

	/**
	* Filters duplicates out of the cloud.  Points considered duplicate if they are within resolution of each other.
	**/
	void remove_duplicates( double resolution );

	/**
	* Remove specified points from cloud
	**/
	void remove( SPAposition_vector const& points_to_remove, double point_coincidence_tol );

	/**
	* Returns a box for the cloud.  This box is not guaranteed to be the tightest possible box for the points.
	**/
	SPAbox get_box() const;

	/**
	* Default constructor.  Creates uninitialized empty cloud.
	**/
	SPAposition_cloud();

	/**
	* Copy constructor.  Copies are light, aliasing the points rather than copying them.
	**/
	SPAposition_cloud( SPAposition_cloud const& other );

	/**
	* Assignment operator.  Copies are light, aliasing the points rather than copying them.
	**/
	SPAposition_cloud& operator=(SPAposition_cloud const& other );

	/**
	* Destructor.
	**/
	~SPAposition_cloud();

	/**
	* @nodoc
	**/
	SPAposition_cloud_impl* get_impl() const;

	/**
	* @nodoc
	**/
	SPAposition_cloud( SPAposition_cloud_impl* this_impl );

	/**
	* Returns a requested number of points into the out_buffer.  If the cloud is smaller
	* than the requested number of points fewer will be returned.  Return value is the actual number
	* of points filled.
	**/
	int guess_nearby_points	(
		SPAposition const& test_point,
		int num_requested,
		SPAposition* out_buffer	) const;

private:
	SPAposition_cloud_impl* m_impl;
};


/**
* Utility for reading csv file or xyz file into a point cloud.
* The csv file or xyz file must be a text file for this function to read it.
* Each row in the input file must have the format 
* 
* double double double
* or
* double,double,double
**/

DECL_BASE SPAposition_cloud read_position_cloud_from_file(FILE* fp);

/**
* Utility for reading csv file or xyz file into a point cloud.
* The csv file or xyz file must be a text file for this function to read it.
* Each row in the input file must have the format 
* 
* double double double
* or
* double,double,double
**/
DECL_BASE SPAposition_cloud read_position_cloud_from_file(const char* file_name);

/**
* Writes an xyz file from a SPAposition_cloud.  Note that the round trip of loading a cloud using
* read_position_cloud_from_file and this function will reorder the points.
**/
DECL_BASE void write_position_cloud_to_file(const SPAposition_cloud& cloud, FILE* fp);

/**
* Writes an xyz file from a SPAposition_cloud.  Note that the round trip of loading a cloud using
* read_position_cloud_from_file and this function will reorder the points.
**/
DECL_BASE void write_position_cloud_to_file(const SPAposition_cloud& cloud, const char* file_name);

/**
* Reads the points from an STL file (either binary or text) into a SPAposition_cloud.
* It also removes any duplicate vertices (where vertices are duplicated if one is within duplicate_resolution
* of the other ).  The exact cloud output depends on the order the files are read in, but is guaranteed to 
* have no points within duplicate_resolution of each other.
**/
DECL_BASE SPAposition_cloud read_position_cloud_from_stl_file( const char* file_name, double duplicate_resolution );

/* @} */
#endif

