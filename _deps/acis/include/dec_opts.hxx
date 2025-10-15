/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// ------------------------------------------------------------------------------------------------
//  Options object for spatial decompositions used by subshell decomposition.
// ------------------------------------------------------------------------------------------------

#ifndef DECOMP_OPTIONS_HXX
#define DECOMP_OPTIONS_HXX

// ------------------------------------------------------------------------------------------------

/**
 * @nodoc
 * @file dec_opts.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup EULERAPI
 *
 * @{
 */

// ------------------------------------------------------------------------------------------------
// Includes:

#include "acis.hxx"
#include "logical.h"
#include "dcl_eulr.h"
#include "acis_options.hxx"
#include "box.hxx"

// ------------------------------------------------------------------------------------------------
// Forward decls:
class SUBSHELL;
class FACE;
class decomp_options;

// ------------------------------------------------------------------------------------------------

/**
 * @nodoc
 * Type of subshell decomposition.
 * @param bin_tree3D
 * Decomposes each shell using a 3D binary tree decomposition. The default.
 * @param oct_tree3D
 * Decomposes each shell using a 3D octree decomposition. 
 * @param z3_bin_tree2D
 * Decomposes each shell by first decomposing into 3 subshells in the z-direction,
 * then using a 2D binary tree in the x and y directions.
 * @param z3_quad_tree2D
 * Decomposes each shell by first decomposing into 3 subshells in the z-direction,
 * then using a 2D quad-tree in the x and y directions.
 * @param z3_xn_bin_tree2D
 * Decomposes each shell by first decomposing into 3 subshells in the z-direction,
 * then decomposing at an array of coordinate values in the x-direction,
 * then using a 2D binary tree in the x and y directions.
 * @param z3_yn_bin_tree2D
 * Decomposes each shell by first decomposing into 3 subshells in the z-direction,
 * then decomposing at an array of coordinate values in the y-direction,
 * then using a 2D binary tree in the x and y directions.
 * @param zn_bin_tree2D
 * Decomposes each shell by first decomposing the shell at an array of coordinate 
 * values in the z-direction, then using a 2D binary tree in the x and y directions.
 * @param zn_quad_tree2D
 * Decomposes each shell by first decomposing the shell at an array of coordinate 
 * values in the z-direction, then using a 2D quad-tree in the x and y directions.
*/
enum decomp_type
{
	bin_tree3D,
	octree3D,
	z3_bin_tree2D,
	z3_quad_tree2D,
	z3_xn_bin_tree2D,
	z3_yn_bin_tree2D,
	zn_bin_tree2D,
	zn_quad_tree2D
};

// Pointer to a subshell decomposition function.
/**
 * @nodoc
 */
typedef SUBSHELL* (*decomp_func) ( FACE*&, SPAbox const&, int, decomp_options const* );


/**
 * @nodoc
 * This class provides an interface to customize the behavior of 
 * subshell decompositions.
 * Object of this class can be created by the user on the stack.
 */

class DECL_EULR decomp_options : public AcisOptions
{
public:

	/**
	 * Default constructor - initializes values to default values.
	 */

	decomp_options();

	/**
	 * Destructor - deallocates coordinate array, if it exists.
	 */

	~decomp_options();

	/**
	 * Returns <tt>TRUE</tt> if the input object is same as this object 
	 * or <tt>FALSE</tt> otherwise.
	 * <br><br>
	 * @param in_decomp_opt
	 * object to compare.
	 */
	logical operator== (decomp_options const& in_decomp_opt) const;

	/**
	 * Gets the type of subshell decomposition to perform.
	 */

	decomp_type get_decomp_type( ) const { return my_type; }

	/**
	 * Gets the minimum size for subdividing a subshell.
	 */

	int get_min_num_faces( ) const { return min_num_faces; }

	/**
	 * Gets the maximum depth for subdividing a shell,
	 * where 0 is the first subshell level.
	 */

	int get_max_depth( ) const { return max_depth; }

	/**
	 * Gets the coordinate array and size. 
	 * Coordinate values must be in increasing order.
	 */

	int get_num_coords( ) const { return num_coords; }
	double* get_coords( ) const { return coords; }

	/**
	 * Gets the flag to use a bounding box to use in place of the shell's bounding box.
	 */

	logical get_use_box( ) { return use_box;}

	/**
	 * Gets the bounding box to use in place of the shell's bounding box.
	 */

	SPAbox& get_box( ) { return my_box;}

	// Gets the subshell decomposition function. For internal use only.
	/**
	 * @nodoc
	 */
	decomp_func get_decomp_func( ) const { return my_func; }

	/**
	 * Sets the type of subshell decomposition to perform.
	 * @param dt
	 * type of subshell decomposition.
	 */

	void set_decomp_type( decomp_type dt );

	/**
	 * Sets the minimum size for subdividing a subshell.
	 * @param mnf
	 * minumum number of faces.
	 */

	void set_min_num_faces( int mnf );

	/**
	 * Sets the maximum depth for subdividing a shell,
	 * where 0 is the first subshell level.
	 * @param depth
	 * maximum depth.
	 */

	void set_max_depth( int depth );

	/**
	 * Sets the coordinate array and size.
	 * Coordinate values must be in increasing order.
	 * @param num_coords
	 * size of coordinate array.
	 * @param coords
	 * array of coordinate values.
	 */

	void set_coords( int num_coords, const double* coords );

	/**
	 * Sets the flag to use a bounding box in place of the shell's bounding box.
	 * @param ub
	 * flag to use alternate bounding box.
	 */

	void set_use_box( logical ub );

	/**
	 * Sets the bounding box to use in place of the shell's bounding box.
	 * @param box
	 * alternate bounding box.
	 */

	void set_box( const SPAbox& box );

	// Sets the subshell decomposition function. For internal use only.
	/**
	 * @nodoc
	 */
	void set_decomp_func( decomp_func fptr );

private:
	
	// Copy constructor and Assignment operators - to avoid duplication of 
	// objects of this class. 
	
	decomp_options( decomp_options const & );
	decomp_options& operator=( decomp_options const & );

	// Initialize data members.

	void init();

	// Data members.

	decomp_type my_type;  // Type of subshell decomposition to perform.
	decomp_func my_func;  // Subshell decomposition function pointer.
	int min_num_faces;    // The minimum size for subdividing a subshell.
	int max_depth;        // Where 0 is the first subshell level.
	int num_coords;       // The size of the coordinate array.
	double* coords;       // Array of coordinate values at which to subdivide.
	logical use_box;      // Flag for using bounding box.
	SPAbox my_box;        // Replacement for shell box, if given.

};

/** @} */
#endif // DECOMP_OPTIONS_HXX
