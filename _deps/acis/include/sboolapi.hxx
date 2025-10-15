/* $Id: */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Description :
// Selective Boolean API Routines.
//
/*******************************************************************/
// History:
// Feb-05-03 jkf : Documentation check.
//
/*******************************************************************/
// Include Files :
#if !defined( SBOOLAPI_HXX )
#define SBOOLAPI_HXX

#include "base.hxx"
class BODY;
class ENTITY_LIST;
class SPAposition;
class SPAvector;
class SPAunit_vector;
class SPAtransf;

#include "dcl_bool.h"
#include "logical.h"
#include "api.hxx"
#include "swp_spl.hxx"
/*******************************************************************/

/**
 * \defgroup BOOLADVSBOOL Selective Booleans
 * \ingroup BOOLAPI
 *
 */

/**
* @file sboolapi.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

/*! \addtogroup BOOLADVSBOOL
 *  \brief Declared at <sboolapi.hxx>, SPAbool
 *  @{
 */


class generic_graph;

/**
 * Creates a graph for the first stage of selective Booleans from a tool body and a blank body.
 * <br><br>
 * <b>Role:</b> This API function creates a graph structure (e.g., graph theory) from the
 * <tt>blank_body</tt> entity and the <tt>tool_body</tt> entity. Using Cellular Topology, distinctive
 * cells of the <tt>blank_body</tt> and the <tt>tool_body</tt> become vertices of the graph.
 * Overlapping portions of the cells become edges in the graph. Once the graph has
 * been created further graph theory operations can be performed to obtain a more
 * desirable graph. This is then used as input to the second stage of selective
 * Booleans, @href api_selective_boolean_stage2.
 * <br><br>
 * For additional information on selective Boolean operations refer to the Technical Article
 * <i>Selective Booleans</i>.
 * <br><br>
 * <b>Errors:</b> Pointer to tool or blank body is <tt>NULL</tt> or not to a <tt>BODY</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param blank
 * blank body.
 * @param tool
 * tool body.
 * @param graph
 * cell adjacency graph.
 * @param ao
 * acis options.
 **/
DECL_BOOL outcome api_selective_boolean_stage1(
	BODY          *blank,
	BODY          *tool,
	generic_graph *&graph,
	AcisOptions *ao = NULL
	);

/**
 * Completes the selective Boolean process for the cells selected.
 * <br><br>
 * <b>Role:</b> This function modifies the entity based on a Cellular Topology cell list. 
 * Only the cell entities in the list should be kept. 
 * <br><br>
 * For additional information on selective Boolean operations refer to the Technical Article
 * <i>Selective Booleans</i>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param non_reg_unite_body
 * body to use.
 * @param cells_to_keep
 * cells to keep.
 * @param ao
 * acis options.
 **/
DECL_BOOL outcome api_selective_boolean_stage2(
	BODY          *non_reg_unite_body,
	ENTITY_LIST   &cells_to_keep,
	AcisOptions   *ao = NULL
	);

/**
 * Completes the selective Boolean process for the cells selected.
 * <br><br>
 * <b>Role:</b> This function modifies the entity based on the results of graph theory. The
 * graph maps to cells in the body and represents the cells to keep.
 * <br><br>
 * The mapping of cells in a graph to entities will not be the same from execution
 * to execution.
 * <br><br>
 * For additional information on selective Boolean operations refer to the Technical Article
 * <i>Selective Booleans</i>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param non_reg_unite_body
 * body to use.
 * @param graph_of_cells_to_keep
 * graph cells to keep.
 * @param ao
 * acis options.
 **/
DECL_BOOL outcome api_selective_boolean_stage2(
	BODY          *non_reg_unite_body,
	generic_graph *graph_of_cells_to_keep,
	AcisOptions *ao = NULL
	);

/**
 * Creates a graph (from cells) used in graph theory.
 * <br><br>
 * <b>Role:</b> This API function creates a graph that can be used for graph theory
 * operations and selective Booleans.
 * <br><br>
 * When creating a graph from cells, the list must contain cells from Cellular
 * Topology. The ACIS topology is analyzed to determine which cells are connected.
 * The connections between cells become edges (or dashes) of the graph.
 * <br><br>
 * For additional information on selective Boolean operations refer to the Technical Article
 * <i>Selective Booleans</i>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param cells
 * cells to use.
 * @param graph
 * A pointer to a generic_graph. The caller assumes ownership, and must call the release method at the end of its lifetime.
 * Note that the generic_graph DTOR is protected.
 * @param ao
 * acis options.
 **/
DECL_BOOL outcome api_create_graph_from_cells(
	ENTITY_LIST   &cells,
    generic_graph *&graph,
	AcisOptions   *ao = NULL
	);

/**
 * Creates a graph (from faces) used in graph theory.
 * <br><br>
 * <b>Role:</b> This API function creates a graph that can be used for graph theory
 * operations and selective Booleans.
 * <br><br>
 * For additional information on selective Boolean operations refer to the Technical Article
 * <i>Selective Booleans</i>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param faces
 * faces to use.
 * @param graph
 * A pointer to a generic_graph. The caller assumes ownership, and must call the release method at the end of its lifetime.
 * Note that the generic_graph DTOR is protected.
 * @param ao
 * acis options.
 **/
DECL_BOOL outcome api_create_graph_from_faces(
	ENTITY_LIST   &faces,
    generic_graph *&graph,
	AcisOptions   *ao = NULL
	);

/**
 * Creates a graph (from edges) used in graph theory.
 * <br><br>
 * <b>Role:</b> This API function creates a graph used for graph theory operations and
 * selective Booleans.
 * <br><br>
 * For additional information on selective Boolean operations refer to the Technical Article
 * <i>Selective Booleans</i>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param edges
 * edges to use.
 * @param graph
 * A pointer to a generic_graph. The caller assumes ownership, and must call the release method at the end of its lifetime.
 * Note that the generic_graph DTOR is protected.
 * @param ao
 * acis options.
 **/
DECL_BOOL outcome api_create_graph_from_edges(
	ENTITY_LIST    &edges,
    generic_graph *&graph,
	AcisOptions   *ao = NULL
	);

// The API boolean_tube_body takes in two bodies and calles the selective
// booleans stage1 once and stage2 the number_of_options times.  Each time
// automaticaly setting up the sub-graph from a set of options and start and
// end conditions.
/**
 * Creates a data structure to facilitate generalizing selective Booleans.
 * <br>
 * <b>Role:</b> @href api_boolean_tube_body uses the <tt>tube_options</tt> data structure.
 * It takes two bodies (<tt>blank</tt> and <tt>tube</tt>) as input and calls the selective Booleans
 * stage 1 once and stage 2 the <tt>number_of_options</tt> times. Each time these selective
 * Booleans are called this function automatically sets up the subgraph from a
 * set of options and start and end conditions.
 * <br><br>
 * <tt>tube_options</tt> contain six options:
 * <ul>
 * <li><b><tt>bool_type</tt>:</b> Specifies the type of Boolean operation. 
 * The four possible value are: <tt>UNITE</tt>, <tt>LIMIT</tt>, <tt>INTERSECT</tt>,
 * and <tt>SUBTRACT</tt>. The default value is <tt>LIMIT</tt></li>
 * <li><b><tt>connected</tt>:</b> Unused option.</li>
 * <li><b><tt>keep_branches</tt>:</b> Specifies whether or not to retain branch cells. 
 * (That is, cells that are not on a "shortest path" between a start cell and end cell.)
 * The default value is <tt>FALSE.</tt></li>
 * <li><b><tt>keep_law</tt>:</b> A law that specifies which cells to keep. 
 * The default value is <tt>NULL.</tt></li>
 * <li><b><tt>merge</tt>:</b> Unused option.</li>
 * <li><b><tt>only_from</tt>:</b> Specifies if cells are "only from" the tube or blank. 
 * <tt>0</tt> means either. <tt>1</tt> means the tube. <tt>2</tt> means the blank.
 * The default value is <tt>0</tt>.</li>
 * </ul>
 * <br>
 * For additional information on selective Boolean operations refer to the Technical Article
 * <i>Selective Booleans</i>.
 * <br>
 * @see law
 */
class DECL_BOOL tube_options : public ACIS_OBJECT
{
private:
	law             *keep_law;
	logical         keep_branches;
	sweep_bool_type bool_type;
	int             only_from;  // 0 either,
	                            // 1 only from the blank,
	                            // 2 only from the tube
	logical         connected;
	logical         merge;
public:
 /**
  * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
  * <br><br>
  * @param keep_law
  * law for keep elements.
  * @param keep_branches
  * keep graph branches.
  * @param bool_type
  * type of Boolean.
  * @param only_from
  * from blank or tool or either.
  * @param connected
  * not used.
  * @param merge
  * not used.
  */

	tube_options(law             *keep_law=NULL,
		         logical         keep_branches=FALSE,
			     sweep_bool_type bool_type=LIMIT,
			     int             only_from=0,
			     logical         connected=FALSE,
				 logical         merge=FALSE);
	/**
	 * C++ destructor.
	 */
	~tube_options();
    /**
     * Determines if two sets of options are equal.
     * <br><br>
     * @param in_tube_opt
     * pointer to the tube option.
     */
	int operator== (tube_options& in_tube_opt);
	/**
     * Tests to see whether or not two sets of options are unequal.
     * <br><br>
	 * @param in_tube_opt
	 * comparison item.
     */
	int operator!= (tube_options& in_tube_opt);
    /**
     * Sets the current <tt>keep_law</tt> option values.
     * <br><br>
     * @param in_keep_law
     * option values.
     */
	void set_keep_law(law *in_keep_law);
	/**
	 * Sets logical to keep or discard branch cells.
	 * <br><br>
	 * <b>Role:</b> Cells that are not on any "shortest path" are considered "branch cells".
	 * This option sets the flag to see if these cells can be either kept or discarded (all of them) flag. Option defaults to <tt>FALSE</tt>.
	 * <br><br>
	 * @param in_keep_branches
	 * keep or not.
	 */
	void set_keep_branches(logical in_keep_branches);
	/**
	 * Sets the <tt>bool_type</tt> for the operation.
	 * <br><br>
	 * <b>Role:</b> The four possible <tt>bool_types</tt> are <tt>UNITE</tt>, <tt>LIMIT</tt>, <tt>INTERSECT</tt>,
	 * and <tt>SUBTRACT</tt>.
	 * <br><br>
	 * @param in_bool_type
	 * Boolean type.
	 */
	void set_bool_type(sweep_bool_type in_bool_type);
	/**
	 * Sets the option to use cells from either (0), only from the tube (1), or only from the blank (2).
	 * <br><br>
	 * @param in_only_from
	 * option value.
	 */
	void set_only_from(int in_only_from);
	/**
	 * Sets whether or not to check for connections.
	 * <br><br>
	 * <b>Role:</b> When creating a graph from cells the list must contain cells
	 * from Cellular Topology. The ACIS topology is analyzed to determine which
	 * cells are connected. The connections between cells become edges (or dashes)
	 * of the graph.
	 * <br><br>
	 * @param in_connected
	 * option flag.
	 */
	void set_connected(logical in_connected);
	/**
	 * Sets the merge option value.
	 * <br><br>
	 * @param in_merge
	 * merge or not.
	 */
	void set_merge(logical in_merge);
    /**
     * Returns the current <tt>keep_law</tt> option value.
     */
	law*			get_keep_law();
	/**
	 * Returns the current <tt>keep_branches</tt> option value.
	 */
	logical			get_keep_branches();
	/**
	 * Returns the current <tt>bool_type</tt>.
	 */
	sweep_bool_type get_bool_type();
	/**
	 * Returns the current <tt>only_from</tt> option value.
	 */
	logical			get_only_from();
	/**
	 * Returns the current <tt>connected</tt> option value.
	 */
	logical			get_connected();
	/**
	 * Returns the current <tt>merge</tt> option value.
	 */
	logical			get_merge();
};

/**
 * Performs a selective Boolean operation on two bodies.
 * <br><br>
 * <b>Role:</b> The API function <tt>boolean_tube_body</tt> takes two bodies (<tt>blank</tt> and <tt>tube</tt>) as
 * input and calls the selective Booleans stage1 once and stage2 the
 * <tt>number_of_options</tt> times. Each time these selective Booleans are called this
 * function automatically sets up the subgraph from a set of options and start and
 * end conditions.
 * <br><br>
 * If you pass multiple options into <tt>api_boolean_tube_body</tt>, the results will be
 * found many times faster than calling it separately for each option. The number of bodies
 * returned in <tt>bodies</tt> equals the number of options. 
 * <br><br>
 * <b>Defining the type of operation (i.e., specifying the <tt>bool_type</tt>) using tube_options:</b>
 * <br><br>
 * There are 3 types of cells: "only from the blank", "only from the tool", and "from both".
 * <br><br>
 * There are 4 possible <tt>bool_types</tt>, <tt>UNITE</tt>, <tt>LIMIT</tt>, <tt>INTERSECT</tt>, and <tt>SUBTRACT</tt>:
 * <br><br>
 * <ul>
 * <li>A <tt>UNITE</tt> operation deals with the cells "only from the tool", adding the
 * specified ones to the blank. </li>
 * <li>A <tt>LIMIT</tt> operation is the same as <tt>UNITE</tt> except that all cells from the
 * blank are discarded instead of kept.</li>
 * <li>An <tt>INTERSECT</tt> operation deals with cells "from both". Only the specified
 * cells are kept.</li>
 * <li>A <tt>SUBTRACT</tt> operation deals with cells "from both". Start with the blank
 * and subtract the specified cells.</li>
 * </ul>
 * <br>
 * For example, given a variable <tt>opts</tt> of type <tt>tube_options</tt>, call: 
 * <tt>opts->set_bool_type(UNITE)</tt>.
 * <br><br>
 * <b>Defining Laws using tube_options:</b>
 * <br><br>
 * Laws are used to specify which subset of the possible cells your operation
 * should use. Laws are string expressions consisting of the main variable that
 * loops over all possible cell numbers (x1), some constants (X2, X3, X4, X5, X6,
 * and possibly more), and some operators and conditionals (=, !=, >, or, etc). For
 * shorthand, you can use x for x1, y for x2, and z for x3.
 * <br><br>
 * <ul>
 * <li>x1 (x) = order of this cell, or the independent variable.</li>
 * <li>x2 (y) = largest order in the component cell x belongs to.</li>
 * <li>x3 (z) = <tt>TRUE</tt> if from tool.</li>
 * <li>x4 = <tt>TRUE</tt> if from blank.</li>
 * <li>x5 = <tt>TRUE</tt> if start cell.</li>
 * <li>x6 = <tt>TRUE</tt> if end cell.</li>
 * </ul>
 * <br>
 * <b>Example Laws:</b>
 * <br><br>
 * Imagine a vertical tube cutting through the three legs of an extruded letter 'E',
 * where the tube also extends above the top leg and below the bottom leg. Let 'E'
 * be the blank, and the tube be the tool. Specify the top cell to be a start cell,
 * and the bottom cell to be the end cell, using the ordering techniques explained
 * below. There are now 4 possible cells that could be included in the unite
 * operation, numbered from top to bottom 0, 1, 2, and 3.
 * <br><br>
 * <ul>
 * <li>x1 iterates over the 4 cells, testing each one to see whether it should
 * be kept. In this case, it takes on numbers 0, 1, 2, and 3.</li>
 * <li>x2 is always 3, since 3 is the biggest possible number.</li>
 * <li>x3 is always <tt>TRUE</tt>, since we are doing a unite, and only cells from the
 * tool are considered.</li>
 * <li>x4 is always <tt>FALSE</tt>, since we are doing a unite, and no cells from the
 * blank are considered.</li>
 * <li>x5 is <tt>TRUE</tt> if x1 is 0.</li>
 * <li>x6 is <tt>TRUE</tt> if x1 is 3.</li>
 * <li>Thus, to specify all but the last cell, use:<br>
 * <tt>law* keep_law = NULL;</tt><br>
 * <tt>api_str_to_law("x!=x2", &keep_law)</tt><br>
 * <tt>opts->set_keep_law( keep_law );</tt><br>
 * <tt>keep_law->remove();</tt></li>
 * <li>To specify all start cells, substitute the second line with:<br>
 * <tt>api_str_to_law("x5", &keep_law)</tt></li>
 * <li>To keep the second and last cells, substitute the second line with:<br>
 * <tt>api_str_to_law("x=1 or x6", &keep_law)</tt></li>
 * </ul>
 * <br>
 * <b>Defining Cell Numbers, or Ordering:</b>
 * <br><br>
 * To determine cell numbers, first pick start cells and end cells. This is done by
 * picking start positions and end positions, or start faces and end faces, which are
 * on the cells you want to pick. Start cells are all numbered zero. The shortest
 * paths from start cells to end cells are automatically found. Depending on the
 * operation type, non-relevant cells are discarded and the rest are given an
 * integral number based on the steps they are away from the start cell. Note that
 * multiple cells could share the same number.
 * <br><br>
 * <b>Keep Branches:</b>
 * <br><br>
 * Cells that are not on any "shortest path" are considered "branch cells." You can
 * either keep them all or discard them all with the <tt>keep_branches</tt> flag, which
 * defaults to <tt>FALSE</tt>. To keep branch cells use: <tt>opts->set_keep_branches(TRUE)</tt>.
 * <br><br>
 * For additional information on selective Boolean operations refer to the Technical Article
 * <i>Selective Booleans</i>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param blank
 * blank body.
 * @param tube
 * tube body.
 * @param start_faces
 * list of start faces.
 * @param end_faces
 * list of end faces.
 * @param start_pos
 * array of start positions.
 * @param number_of_starts
 * number of start positions.
 * @param end_pos
 * array of end positions.
 * @param number_of_ends
 * number of end positions.
 * @param start_dir
 * start direction should be (0,0,0) if not cyclic.
 * @param opts
 * options for tube.
 * @param number_of_options
 * number of options.
 * @param bodies
 * output bodies.
 * @param ao
 * acis options.
 **/
DECL_BOOL outcome api_boolean_tube_body(
	BODY         *blank,
	BODY         *tube,
	ENTITY_LIST  &start_faces,
	ENTITY_LIST  &end_faces,
	SPAposition     *start_pos,
	int          number_of_starts,
	SPAposition     *end_pos,
	int          number_of_ends,
	SPAvector       start_dir,			// Should be (0,0,0) if not cyclic
	tube_options **opts,
	int          number_of_options,
	ENTITY_LIST  &bodies,
	AcisOptions *ao = NULL);


/**
 * Finds the subset of a graph on one side of a %plane.
 * <br><br>
 * <b>Role:</b> The graph is first split at the specified %plane. Then a copy of the portion
 * of the graph on one side of the plane is returned.
 * It is assumed that the graph is made of entities that are either faces or cells.
 * <br><br>
 * For additional information on selective Boolean operations refer to the Technical Article
 * <i>Selective Booleans</i>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param whole_graph
 * input graph.
 * @param plane_origin
 * origin.
 * @param plane_normal
 * %plane normal.
 * @param partial_graph
 * output graph.
 * @param ao
 * acis options.
 **/
DECL_BOOL outcome api_subset_graph_with_plane (
	const generic_graph *whole_graph,
	const SPAposition &plane_origin,
	const SPAunit_vector & plane_normal,
	generic_graph *&partial_graph,
	AcisOptions *ao = NULL
	);

/**
 * Returns a subgraph containing only the vertices that are 2D cells.
 * <br><br>
 * For additional information on selective Boolean operations refer to the Technical Article
 * <i>Selective Booleans</i>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param whole_graph
 * input graph.
 * @param partial_graph
 * output graph with 2d cells.
 * @param ao
 * acis options.
 **/
DECL_BOOL outcome api_subgraph_2dcell (
	const generic_graph *whole_graph,
	generic_graph *&partial_graph,
	AcisOptions *ao = NULL
	);

/**
 * Returns a subgraph containing only the vertices that are 3D cells.
 * <br><br>
 * For additional information on selective Boolean operations refer to the Technical Article
 * <i>Selective Booleans</i>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param whole_graph
 * input graph.
 * @param partial_graph
 * output graph with 3d cells.
 * @param ao
 * acis options.
 **/
DECL_BOOL outcome api_subgraph_3dcell (
	const generic_graph *whole_graph,
	generic_graph *&partial_graph,
	AcisOptions *ao = NULL
	);

/**
 * Unites two bodies with the given positions.
 * <br><br>
 * <b>Role:</b> This selective union operation takes two bodies and two arrays of
 * positions; one array corresponding to each body. The arrays of positions are used 
 * to specify the portions of the bodies to keep after the selective Boolean operation. 
 * If the number of positions in an array is zero the operation will retain the entire body.
 * Each position should be chosen such that it uniquely specifies a cell, which is used as 
 * input to the second stage of the selective Boolean process.
 * <br><br>
 * For additional information on selective Boolean operations refer to the Technical Article
 * <i>Selective Booleans</i>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
 * @param tool
 * first body.
 * @param blank
 * second body.
 * @param tnum
 * number of position on tool to keep, 0 means keep all.
 * @param tpos
 * positions on tool body.
 * @param bnum
 * number of position on blank to keep, 0 means keep all.
 * @param bpos
 * positions on blank body.
 * @param ao
 * acis options.
 **/
DECL_BOOL outcome api_selective_unite (
	BODY     *tool,
	BODY     *blank,
	int       tnum,
	SPAposition *tpos,
	int       bnum=0,
	SPAposition *bpos=NULL,
	AcisOptions *ao = NULL
	);

/*! @} */

#endif
