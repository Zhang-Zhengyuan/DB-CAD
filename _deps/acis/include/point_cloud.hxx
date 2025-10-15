/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/



#ifndef POINT_CLOUD_HXX
#define POINT_CLOUD_HXX

#include "dcl_kern.h"

#include "box.hxx"
#include "logical.h"
#include "position_list_stream.hxx"
#include "int_list_stream.hxx"


/**
 * @file point_cloud.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \defgroup ACISPOINTCLOUD Point Cloud
 * \ingroup  ACISKERN
 *
 *
 */
/**
 * \addtogroup ACISPOINTCLOUD
 *
 * @{
 */

class point_cloud_bulletin;
class point_cloud_iter_mgr;
class SPAtransf;


/**
* @nodoc
**/
enum SPApoint_cloud_sort_mode
{
	SPApoint_cloud_sort_always = 0,		// default
	SPApoint_cloud_sort_never,
	SPApoint_cloud_sort_as_needed
};

/**
* The <tt>SPApoint_cloud_copy_type</tt> enumeration is used to control the behavior of the <tt>make_object</tt> virtual
* function in the <tt>SPApoint_cloud</tt> and <tt>SPApoint_cloud_custom_data</tt> classes.
* <br><br>
* <b>Role:</b> Implementations of the <tt>make_object</tt> virtual function in classes derived from <tt>SPApoint_cloud</tt>
* or <tt>SPApoint_cloud_custom_data</tt> can customize the copy behavior using the <tt>SPApoint_cloud_copy_type</tt> enumeration.  
* This affects the <tt>copy</tt> (simple and deep copy), <tt>subset</tt>, <tt>backup</tt>, and <tt>roll</tt> methods.
* <br><br>
* <b>See also:</b>
* <br>@docwiki Point_Cloud Technical Article
* 
*/
enum SPApoint_cloud_copy_type 
{ 
	SPApoint_cloud_simple_copy = 0,
	SPApoint_cloud_deep_copy,
	SPApoint_cloud_subset_copy,
	SPApoint_cloud_backup_copy,
	SPApoint_cloud_roll_copy
};

/**
 * Implements a <tt>SPApoint_cloud_options</tt> class.
 * <br>
 * <b>Role:</b> The <tt>SPApoint_cloud_options</tt> class is used in the construction of the <tt>SPApoint_cloud</tt> object.
 * The <tt>SPApoint_cloud_options</tt> class defines the behavior of the <tt>SPApoint_cloud</tt> object.
 * The options defined by this class include the bounding box padding and the <tt>list_stream_options</tt> for the index list
 * and the position list.
 * <br><br>
 * <b>See also:</b>
 * <br>@docwiki Point_Cloud Technical Article
 */
class DECL_KERN SPApoint_cloud_options : public ACIS_OBJECT
{
	SPApoint_cloud_sort_mode _sort_mode;	// default is sort always and cannot currently be changed
	double _bounding_box_padding;
	list_stream_options _position_options;
	list_stream_options _index_options;
	

public:
	/**
	* C++ constructor requests memory for this object and populates it with the data supplied as arguments.
	* <br>
	* <b>Role:</b> Creates a <tt>SPApoint_cloud_options</tt> object using the supplied arguments.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	SPApoint_cloud_options(list_stream_options* position_options = NULL, list_stream_options* index_options = NULL);
	
	// sort mode
	//void set_sort_mode( SPApoint_cloud_sort_mode sort_mode );
	/**
	* @nodoc
	*/
	SPApoint_cloud_sort_mode get_sort_mode() const { return _sort_mode; }

	/**
	* Sets the bounding box padding value on the <tt>SPApoint_cloud_options</tt> object.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	void set_bounding_box_padding( double padding );

	/**
	* Gets the current bounding box padding set on the <tt>SPApoint_cloud_options</tt> object.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	double get_bounding_box_padding() const { return _bounding_box_padding; }

	/**
	* Sets the <tt>list_stream_options</tt> used for the internal position list.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	void set_position_list_options(const list_stream_options& position_options);

	/**
	* Gets the <tt>list_stream_options</tt> used for the internal position list.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	list_stream_options& get_position_list_options() { return _position_options; }

	/**
	* Sets the <tt>list_stream_options</tt> used for the internal index list.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	void set_index_list_options(const list_stream_options& index_options);

	/**
	* Gets the <tt>list_stream_options</tt> used for the internal index list.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	list_stream_options& get_index_list_options() { return _index_options; }
};




/**
 * Implements a <tt>SPApoint_cloud_custom_data</tt> class.
 * <br>
 * <b>Role:</b> The <tt>SPApoint_cloud_custom_data</tt> class is provided as a base class that can
 * be extended to encapsulate user data which can be associated with a point cloud object.
 * To store application specific data with a point cloud, you can implement a derived class from 
 * <tt>SPApoint_cloud_custom_data</tt> and register an instantiation of the class with an instance of
 * the <tt>SPApoint_cloud</tt> class.
 * <br><br>
 * <b>See also:</b>
 * <br>@docwiki Point_Cloud Technical Article
 */
class SPApoint_cloud_custom_data : public ACIS_OBJECT
{
public:
	virtual ~SPApoint_cloud_custom_data() {};

	/**
	* The <tt>get_transform</tt> method returns the <tt>SPAtransf</tt> transform object associated with 
	* this <tt>SPApoint_cloud_custom_data</tt> object.
	* <br><br>
	* <b>Role:</b> The virtual <tt>get_transform</tt> method is provided as a way for you to communicate transformation
	* information to the <tt>SPApoint_cloud</tt> that maintains a reference to this <tt>SPApoint_cloud_custom_data</tt> object.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	virtual const SPAtransf* get_transform() const = 0;

	/**
	* You have to implement the <tt>make_object</tt> virtual method if you created a class derived
	* from <tt>SPApoint_cloud_custom_data</tt>. 
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	virtual SPApoint_cloud_custom_data* make_object( SPApoint_cloud_custom_data* target_custom_data,
		SPApoint_cloud_copy_type copy_type ) = 0;
};

/**
* This is the central class in ACIS Point Cloud Functionality.
* <br><br>
* <b>Role:</b> The <tt>SPApoint_cloud class</tt> is designed to hold large sets of points. This class provides functionality for the 
* efficient storage and manipulation of large sets of points.
* <br><br>
* <b>See also:</b>
* <br>@docwiki Point_Cloud Technical Article
*/
class DECL_KERN SPApoint_cloud : public ACIS_OBJECT
{
	position_list_stream* _pos_list;			// may be referred to by multiple point clouds, use-counted
	sortable_integer_ls* _index_list;			// owned by the point cloud, use count will be at most one

	point_cloud_bulletin* _backup;				// undo-redo data structures	

	SPApoint_cloud_custom_data* _custom_data;	// class to hold point cloud properties, transform

	SPAbox _bounding_box;						// cached bounding box
	logical _bound_needs_update;				// flag indicating that the cached box is out-of-date

	SPApoint_cloud_options _opts;				// internal storage of point cloud options

	int _current_pos;							// current index in the position list

	point_cloud_iter_mgr* _pci_mgr;				// internal manager of iterators associated with the point cloud

	// internal helper routines
	void calculate_bound();			
	void update_bounding_box( SPAposition const& pos );
	sortable_integer_ls* create_index_list();
	logical get_pos_list_index( int index, int& pos_index );
	point_cloud_iter_mgr* get_iterator_mgr();
	SPApoint_cloud* copy_internal( SPApoint_cloud* target_pc, SPApoint_cloud_options* pc_opts, SPApoint_cloud_copy_type copy_type );

	// access methods to support user implementations of save and restore
	friend logical save_point_cloud(void*);
	friend logical restore_point_cloud(void*);

	friend class point_cloud_bulletin;
	friend class SPApoint_cloud_iterator;

	// make the copy constructor and operator= private for now
	SPApoint_cloud(SPApoint_cloud const&) {};
	SPApoint_cloud& operator = (SPApoint_cloud const&) {return *((SPApoint_cloud*)NULL_REF);}

protected:
	/**
	* You have to implement the <tt>make_object</tt> virtual method if you derived a class from <tt>SPApoint_cloud</tt>. 
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	virtual SPApoint_cloud* make_object( SPApoint_cloud* target_pc, SPApoint_cloud_options* pc_opts,
		position_list_stream* pos_list, SPApoint_cloud_copy_type copy_type );

public: 
	/**
	 * C++ allocation constructor requests memory for this object and populates it.
	 * <br><br>
	 * <b>Role:</b> Constructs a <tt>SPApoint_cloud</tt> object using the optional input arguments.
	 * <tt>Note:</tt> The point cloud takes ownership of the <tt>pos_list</tt>, if one is passed in.
	 * <br><br>
	 * @param pc_opts
	 * optional <tt>SPApoint_cloud_options</tt> specifying the properties of the constructed <tt>SPApoint_cloud</tt> object.
	 * @param pos_list
	 * optional <tt>position_list_stream</tt> object containing initial points used in the <tt>SPApoint_cloud</tt> object.
	 * <br><br>
     * <b>See also:</b>
     * <br>@docwiki Point_Cloud Technical Article
	 */
	SPApoint_cloud( SPApoint_cloud_options* pc_opts = NULL, position_list_stream* pos_list = NULL );

	/**
	* Virtual C++ destructor, deleting a <tt>SPApoint_cloud</tt> object.
	* <br><br>
	* <b>Role:</b> C++ destructor that is invoked on deletion of this object. Allocated memory is freed in this method.
	* This method is declared virtual so you implement a destructor for your derived class.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	virtual ~SPApoint_cloud();

	/**
	* Adds a position to the point cloud.
	* <br><br>
	* <b>Role:</b> This method enables addition of a single position to the point cloud.  It returns the point
	* cloud index at which the position is added.
	* @param pos
	* SPAposition to add
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	int add( SPAposition const& pos );

	/**
	* Adds a position to the point cloud.
	* <br><br>
	* <b>Role:</b> This method enables addition of a single position to the point cloud. It returns the point
	* cloud index at which the position is added.
	* @param x
	* x coordinate of the SPAposition to add
	* @param y
	* y coordinate of the SPAposition to add
	* @param z
	* z coordinate of the SPAposition to add
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	int add( double x, double y, double z );

	/**
	* Removes a point from the point cloud at the given point cloud index.
	* <br><br>
	* <b>Role:</b> Removes the point in the point cloud corresponding to the given point cloud index.
	* The method returns <tt>FALSE</tt> if the index is not valid.
	* @param index
	* The index of the point to be removed from the <tt>SPApoint_cloud</tt>.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	logical remove( int index );

	/**
	* Modifies an existing position in the point cloud at the given point cloud index (expert interface).
	* <br><br>
	* <b>Role:</b> Modifies the point in the point cloud corresponding to the given point cloud index.
	* The method returns <tt>FALSE</tt> if the index is invalid or the operation is otherwise unsuccessful.
	* <br>
	* <b>Note:</b> The modify call works only if the positions are not shared with any other <tt>SPApoint_cloud</tt> object
	* or by the history mechanism of the point cloud.
	* @param index
	* The point cloud index.
	* @param pos
	* The new value of the SPAposition at this index.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	logical modify( int index, SPAposition const& pos );

	/**
	* Modifies an existing position in the point cloud at the given point cloud index (expert interface).
	* <br><br>
	* <b>Role:</b> Modifies the point in the point cloud corresponding to the given point cloud index.
	* The method returns <tt>FALSE</tt> if the index is invalid or the operation is otherwise unsuccessful.
	* <br>
	* <b>Note:</b> The modify call works only if the positions are not shared with any other <tt>SPApoint_cloud</tt> object
	* or by the history mechanism of the point cloud.
	* @param index
	* The point cloud index.
	* @param x
	* x coordinate of the SPAposition to modify.
	* @param y
	* y coordinate of the SPAposition to modify.
	* @param z
	* z coordinate of the SPAposition to modify.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	logical modify( int index, double x, double y, double z );

	/**
	* Gets the position in a point cloud at the given point cloud index.
	* <br><br>
	* <b>Role: </b> The <tt>get</tt> method retrieves the <tt>SPAposition</tt> at the given point cloud index. The method
	* returns <tt>FALSE</tt> if the index is not valid.
	* <br>
	* <b>Note: </b> You should always check the return value of the call, because the retrieved position may not
	* be usable if the method returns <tt>FALSE</tt>.
	* @param index
	* The point cloud index of the desired position.
	* @param pos
	* The position retrieved from the point cloud.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	logical get( int index, SPAposition& pos );

	/**
	* Gets the position and the position list index at the given point cloud index (expert interface).
	* <br><br>
	* <b>Role: </b> The expert interface <tt>get</tt> method retrieves the <tt>SPAposition</tt> at the given point cloud given index  
	* as well as the index in the position list. The method returns <tt>FALSE</tt> if the point cloud index is not valid.
	* <br>
	* <b>Note:</b> You should always check the return value of the call, because the retrieved position may not
	* be usable if the method returns <tt>FALSE</tt>.
	* @param index
	* The point cloud index of the desired position.
	* @param pos
	* The position retrieved from the point cloud.
	* @param pos_list_index
	* The position list index associated with the retrieved position.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	logical get( int index, SPAposition& pos, int& pos_list_index );

	/**
	* Gets the maximum point cloud index.
	* <br><br>
	* <b>Role:</b> The <tt>max_index</tt> method returns the maximum point cloud index. If points have been deleted from the cloud, 
	* the returned value will be larger than the <tt>size</tt> of the point cloud. This method returns -1 if the point cloud is empty.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	int max_index();

	/**
	* Gets the size of the point cloud.
	* <br><br>
	* <b>Role:</b> The <tt>size</tt> method returns the number of active points contained in the point cloud. 
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	int size();

	/**
	* Returns a pointer to the <tt>SPAtransf</tt> transform object associated with the point cloud.
	* <br><br>
	* <b>Role: </b>
	* The <tt>get_transform</tt> method is used by ACIS functions to query for the transform associated with the point cloud.
	* This method is defined as a virtual method to allow you to define the transform for your class derived from <tt>SPApoint_cloud</tt>.
	* The default base class implementation of this method returns a pointer to a global identity transform.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
 	virtual const SPAtransf* get_transform() const;

	/**
	* Returns the bounding box of the point cloud.
	* <br><br>
	* <b>Role: </b>
	* The <tt>bound</tt> method returns the box bounding all points in the cloud.
	* The call to <tt>bound</tt> will trigger a re-calculation of the bounding box only if the bounding box is obsolete.
	* Optionally, you can force a recalculation of the bounding box.
	* @param force_recalc
	* Force a recalculation of the bounding box. The default value is <tt>FALSE</tt>.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	SPAbox bound( logical force_recalc = FALSE );

	/**
	* Creates a copy of the point cloud in either the simple copy or deep copy mode.
	* <br><br>
	* <b>Role:</b> This copy method makes a simple or deep copy of this point cloud depending on the value of the input.
	* The simple copy results in a point cloud which shares the positions with the original cloud, whereas the deep copy
	* leads to a point cloud which copies all the position data the new cloud.
	* <br>
	* <b>Note:</b> Deep copying a point cloud has implication for some operations such as the boolean operations.
	* @param deep_copy
	* Determines whether to make a simple or a deep copy. The default is <tt>FALSE</tt>.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	SPApoint_cloud* copy( logical deep_copy = FALSE );

	/**
	* Creates a subset of the point cloud.
	* <br><br>
	* <b>Role:</b> The <tt>subset</tt> method creates a new point cloud that contains a subset of points from the original
	* cloud. The new point cloud contains the points specified in terms of the point cloud indices of the original cloud.  
	* The subset cloud inherits the option settings from the original point cloud if the options are left unspecified.  
	* The subset method returns <tt>NULL</tt> if the point cloud indices in the input are invalid.
	* @param indices
	* The point cloud indices of points to include in the subset.
	* @param pc_opts
	* The point cloud options object.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	* 
	*/
	SPApoint_cloud* subset( integer_list_stream& indices, SPApoint_cloud_options* pc_opts = NULL );

	/**
	* Creates a subset of the point cloud (expert interface).
	* <br><br>
	* <b>Role:</b> The <tt>subset</tt> method creates a new point cloud that contains a subset of points from the original cloud. 
	* The new point cloud contains the points specified in terms of indices of the position list in the original cloud. The subset
	* cloud inherits the option settings from the original point cloud if the options are left unspecified. The point cloud assumes
	* ownership of the <tt>pos_list_indices</tt> object.
	* <br>
	* <b>Note:</b> This interface does not check to ensure that the position list indices are valid.
	* @param indices
	* The indices of the points with respect to the position list.
	* @param pc_opts
	* The point cloud options object.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	* 
	*/
	SPApoint_cloud* subset( sortable_integer_ls* pos_list_indices, SPApoint_cloud_options* pc_opts = NULL );

	/**
	* Performs an in-place boolean unite operation on two point clouds. 
	* <br><br>
	* <b>Role:</b> The boolean <tt>unite</tt> method combines the points of two point clouds into one cloud. If the positions
	* are shared between the two clouds and the transforms are the same, then duplicate points are included only once in the result.
	* Otherwise, the points in the input cloud are all added as new points to the current cloud (without checking for duplicates
	* in 3D space). The result always replaces the current cloud. This method returns <tt>TRUE</tt> if the operation is successful.
	* <br>
	* <b>Note:</b> This operation will invalidate any iterators associated with the current cloud.
	* @param point_cloud
	* The point cloud whose points are to be united with the current cloud.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	logical unite( SPApoint_cloud& point_cloud );

	/**
	* Performs an in-place boolean subtraction of a point cloud from the current cloud. 
	* <br><br>
	* <b>Role:</b> The boolean <tt>subtract</tt> method removes the common points between the input and current point cloud from
	* the current cloud. The result replaces the current point cloud. This method works only if the input and current point clouds
	* share the positions and have the same transform. Otherwise, this method returns <tt>FALSE</tt>.
	* <br>
	* <b>Note:</b> This operation will invalidate any iterators associated with the current cloud.
	* @param point_cloud
	* The point cloud whose points are to be subtracted from the current cloud.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	logical subtract( SPApoint_cloud& point_cloud );

	/**
	* Performs an in-place boolean intersection between two point clouds. 
	* <br><br>
	* <b>Role:</b> The boolean <tt>intersection</tt> method retains the points common to both clouds. The results replaces
	* the current point cloud. This method works only if the input and the current point clouds share the positions and have
	* the same transform. Otherwise, this method returns <tt>FALSE</tt>.
	* @param point_cloud
	* <br>
	* <b>Note:</b> This operation will invalidate any iterators associated with the current cloud.
	* The point cloud whose points are to be intersected with the current cloud.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	* 
	*/
	logical intersect( SPApoint_cloud& point_cloud );

	/**
	* Compacts the point cloud (expert interface).
	* <br><br>
	* <b>Role:</b> The <tt>compact</tt> method does multiple things to the point cloud that cannot be undone, so you must use
	* this functionality with care. This method will delete all the backups of prior states of the point cloud and will invalidate all iterators
	* associated with the cloud. In the default case that the flag <tt>deep_compact</tt> is <tt>FALSE</tt>, the point cloud is re-indexed to
	* eliminate any indices for points that have been removed from the cloud. If the flag is set to <tt>TRUE</tt>, the point cloud positions are
	* unshared from all other point clouds, and only the positions relevant to the current cloud are retained.
	* <br>
	* <b>Note:</b> The results of point cloud compaction are permanent, and compacting the position list has ramfications for
	* other operations such as the boolean operations.
	* @param deep_compact
	* Optional argument that causes the positions to be unshared from other point clouds and irrelevant points to be permanently deleted from the cloud.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/	
	logical compact( logical deep_compact = FALSE );

	//
	// Point Cloud undo-redo
	//
	/**
	* Records the state of the <tt>SPApoint_cloud</tt> object and returns the state identifier.
	* <br><br>
	* <b>Role:</b> This method creates a backup record that contains the current state of the point cloud and adds it to the list of known states.
	* All forward states (that is, redo states) are removed and the new state becomes the last known state.
	* Backup accepts and stores a pointer to a string that can be used as another means to identify a state. 
	* ACIS does not copy, modify, or assume ownership of this string in any way. It is up to you to create and destroy it appropriately.
	* The return value is the identifier assigned to the new state. The identifier is a sequental number 
	* that is the quasi index of the state in the linked list of states. When assigned, it is the value of the earlier state plus one.
	* @param name
	* A custom name for the state.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	int backup( const char* name = NULL); 

	/**
	* Reactivates the state reached by rolling the specified number of steps through the known states.
	* <br><br>
	* <b>Role:</b>
	* This is the primary means for undoing and reapplying backup records by "rolling" either forwards or backwards through the known states.
	* Negative step values rusult in roll back, which is undo, and positive values result in roll forward, which is redo.
	* The return value is the number of steps successfully rolled.
	* @param steps
	* The number of steps to roll through the states.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	int roll( int steps = -1);

	/**
	* Reactivates the state specified by the identifier returned from backup.
	* <br><br>
	* <b>Role:</b>
	* This method will roll through backup records, either forwards or backwards, until it either reaches the specificed state
	* or the state with the lowest state ID that is not less than the one specified. Rolling to state ID zero for example, 
	* will always result in rolling to the very first state.
	* The return value is the number of steps successfully rolled.
	* @param state_id
	* The identifier of the state to reactivate.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	int roll_to( int state_id);

	/**
	* Deletes all states owned by the <tt>SPApoint_cloud</tt> object.
	* <br><br>
	* <b>Role:</b>
	* Use this method method to permanently delete all backup records. 
	* Returns <tt>FALSE</tt> if no states exist.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	logical delete_all_states();

	/**
	* Deletes the state specified by the identifier.
	* <br><br>
	* <b>Role:</b>
	* Use this method to permanently delete a specific state. 
	* Returns <tt>FALSE</tt> if the specified state does not exist.
	* @param state_id
	* The identifier of the state to delete.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	logical prune( int state_id);

	/**
	* Invalidates all forward states owned by the <tt>SPApoint_cloud</tt> object.
	* <br><br>
	* <b>Role:</b>
	* Use this method to invalidate all backup records that have been undone. 
	* The main purpose for this is to assure that forward states cannot be 
	* reinstated after the <tt>SPApoint_cloud</tt> object has been modified. 
	* Otherwise it would be possible to roll to an earlier state, then modify the point cloud,
	* then roll to a later state (forward), which will result in unexpected behavior.
	* This method is automatically called by all base class methods that modify the point cloud.
	* Call this method in derived class methods as needed.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	void invalidate_forward_states();

	/**
	* Returns the state ID of current state of the <tt>SPApoint_cloud</tt> object.
	* <br><br>
	* <b>Note:</b> The return value is -1 if no states exist.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	int backup_id() const;

	/**
	* Returns the name of current state of the <tt>SPApoint_cloud</tt> object.
	* <br><br>
	* <b>Note:</b> The return value is NULL if no states exist.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	const char* backup_name() const;

	/**
	* Returns TRUE if the current state has been rolled back, otherwise FALSE.
	* <br><br>
	* <b>Note:</b> The return value has no meaning if no states exist.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	logical backup_state() const;

	/**
	* Returns TRUE if the current state is invalid, otherwise FALSE.
	* <br><br>
	* <b>Note:</b> The return value has no meaning if no states exist.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	logical backup_invalid() const;

	//
	// Point Cloud custom data
	//
	/**
	* Method that sets the <tt>SPApoint_cloud_custom_data</tt> on this <tt>SPApoint_cloud</tt> object.
	* <br>
	* The <tt>SPApoint_cloud</tt> class maintains a reference to an instance of the 
    * <tt>SPApoint_cloud_custom_data</tt> class. If you want to store custom information with the point-cloud,
	* you can choose to derive your application-specific class from <tt>SPApoint_cloud_custom_data</tt>.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	void set_custom_data( SPApoint_cloud_custom_data* custom_data );

	/**
	* Method that retrieves the <tt>SPApoint_cloud_custom_data</tt> object that this <tt>SPApoint_cloud</tt> object refers to.
	* <br>
	* The <tt>SPApoint_cloud</tt> class maintains a reference to an instance of the 
    * <tt>SPApoint_cloud_custom_data</tt> class. If you want to store custom information with the point-cloud,
	* you can choose to derive your application-specific class from <tt>SPApoint_cloud_custom_data</tt>.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	const SPApoint_cloud_custom_data* get_custom_data() const;
};

/**
 * Implements a <tt>SPApoint_cloud_iterator</tt> class.
 * <br>
 * <b>Role:</b> The <tt>SPApoint_cloud_iterator</tt> class is provided to iterate through the points in an instance of
 * <tt>SPApoint_cloud</tt> object. An instance of this iterator class is automatically invalidated after certain operations
 * on the associated point cloud such as boolean operations, compaction, or destruction.
 * <br><br>
 * <b>See also:</b>
 * <br>@docwiki Point_Cloud Technical Article
 */
class DECL_KERN SPApoint_cloud_iterator : public ACIS_OBJECT
{
	int _iter;
	int _iter_pos;
	SPApoint_cloud* _pc;

	friend class point_cloud_iter_mgr;

public:
	/**
	 * C++ allocation constructor requests memory for this object and populates it.
	 * <br><br>
	 * <b>Role:</b> Constructs a <tt>SPApoint_cloud_iterator</tt> object and associates it with the specified point cloud.
	 * <br><br>
	 * @param pc
	 * Point cloud through which to iterate.
	 * <br><br>
     * <b>See also:</b>
     * <br>@docwiki Point_Cloud Technical Article
	 */
	SPApoint_cloud_iterator( SPApoint_cloud& pc );

	/**
	* Virtual C++ destructor, deleting a <tt>SPApoint_cloud_iterator</tt> object.
	*/
	~SPApoint_cloud_iterator();

	/**
	* Initializes the <tt>SPApoint_cloud_iterator</tt> object to the beginning of the list.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	void init_start();

	/**
	* Initializes the <tt>SPApoint_cloud_iterator</tt> object to the end of the list.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	void init_end();

	/**
	* Retrieves the next (undeleted) position in the point cloud associated with this iterator.
	* <br><br>
	*<b>Role:</b> This method returns the next existing <tt>SPAposition</tt> in the associated point cloud. 
	* The method returns <tt>FALSE</tt> when it reaches the end of list, in which case the retrieved
	* position may be unusable.
	* <br>
	* @param pos
	* The retrieved position.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	logical next( SPAposition& pos );

	/**
	* Retrieves the previous (undeleted) position in the point cloud associated with this iterator.
	* <br><br>
	* <b>Role:</b> This method returns the previous existing <tt>SPAposition</tt> in the associated point cloud. 
	* The method returns <tt>FALSE</tt> when it reaches the beginning of list, in which case the retrieved
	* position may be unusable.
	* <br>
	* @param pos
	* The retireved position.
	* <br><br>
    * <b>See also:</b>
    * <br>@docwiki Point_Cloud Technical Article
	*/
	logical previous( SPAposition& pos );
};

/** @} */

#endif // POINT_CLOUD_HXX
