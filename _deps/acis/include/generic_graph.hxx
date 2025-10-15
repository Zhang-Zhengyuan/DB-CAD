// $Id: generic_graph.hxx,v 1.21 2001/08/22 16:10:18 ktatters Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef GENERIC_GRAPH_HXX
#define GENERIC_GRAPH_HXX

#include "dcl_kern.h"
#include "acis3dt.h"
#include "position.hxx"

/**
 * \defgroup ACISGRAPHTHEORY Graph Theory
 * \ingroup KERNAPI
 *
 */
 /**
 * @file generic_graph.hxx
 * @CAA2Level L2
 * @CAA2Usage U2
 * \addtogroup ACISGRAPHTHEORY
 *
 * @{
 */
///////////////////////////////////////////////////////////////////////////////
//
//  hash_sort:	A class for hash sorting lists of integers
//
//  To use this fast hash sorting, first make an object of type hash_sort.
//  The size is the maximum value that any of your integers might be.
//
//  Then add_item for each element in your original array.
//  The original_index is its location in your array, usually 0, 1, 2, etc.
//  The slot is where it sorts to, and must be 0 <= slot < size.
//  You can pass in several items with the same slot number,
//  and you need not use all the slots.
//  Items that share the same slot are not sorted amongst themselves.
//
//  When you are done adding items, call sort.
//  It returns an array of integers which are indexes into your original array.
//  For example, if you pass in 5 items, and the return array is 3 1 0 2 4,
//  Then your original array is sorted by accessing those elements, like this:
//  for (int i=0; i<5; i++)
//		printf("%i\n", my_array[ returned_array[i] ] );
//  The sort call also returns the elements in the returned array,
//  which should be the same as the number of items you added.
//  NOTE: You do NOT own the returned array - it dies when the class dies.
//
//  See generic_graph::get_entities_from_vertex in generic_graph.cxx
//  for a usage example.
//
///////////////////////////////////////////////////////////////////////////////
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN hash_item : public ACIS_OBJECT
{
public:
	int original_index;
	hash_item * next;
};
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN hash_sort : public ACIS_OBJECT
{
public:
	hash_sort(int temp_size);
	void add_item(int slot, int original_index);


	// You do NOT own the memory returned by this routine!
	// Use it BEFORE this class gets destroyed,
	// as it dies when this class dies.
	int * sort(int & how_many);

	~hash_sort();

protected:
	int size;		// the size of this integer hash table
	int count;		// number of entries currently added.
	hash_item **items;
	int *sort_items; // the result array
	int sort_count;	// the amount of memory currently in the result array
};

///////////////////////////////////////////////////////////////////////////////
//
//  A class for generic vertices of a graph
//
///////////////////////////////////////////////////////////////////////////////

class gedge;
class ENTITY;
/*
// tbrv
*/
/**
 * @nodoc
 */
int DECL_KERN new_graph_id();
/**
 * Creates an instance of a graph vertex for use in graph theory.
 * <br>
 * <b>Role:</b> The concepts of vertex, edge, and graph have been implemented as
 * the C++ classes <tt>gvertex</tt>, <tt>gedge</tt>, and <tt>generic_graph</tt>. (<tt>entity_gvertex</tt> is derived
 * from <tt>gvertex</tt> except that it contains a pointer to an entity in the model. Such
 * an entity could be a cell or a face.) A gvertex may be created with an optional
 * char <tt>*name</tt>. A gedge may be created with two gvertex pointers. An empty graph
 * may be created and edges and vertices may be added to it by calling its
 * <tt>add_vertex</tt> and <tt>add_edge</tt> methods. Once created, a graph may be interrogated,
 * ordered, or subsetted in a number of ways.
 * <br><br>
 * The C++ classes of <tt>gvertex</tt> and <tt>gedge</tt> are use counted in the same way that laws
 * are use counted. That is to say that the are copied by calling the <tt>add</tt> method
 * and deleted by calling the <tt>remove</tt> method.
 * <br><br>
 * To make a gvertex or gedge contain data, derive a class from the base classes
 * of <tt>gvertex</tt> and <tt>gedge</tt>. Use a technique similar to the <tt>entity_gvertex</tt> class which
 * enables it to contain an entity pointer.
 * <br><br>
 * @see gedge
 */

// Disabling warning because of variable "how_many" in gedge and gvertex
#pragma warning ( push )
#pragma warning( disable: 4251 )

class DECL_KERN gvertex : public ACIS_OBJECT
{
private:

	// The following three variables are NOT considered part of the gvertex
	// as far as the == operator is concerned, and methods like set_kind
	// can be declared as class const.
	int      use_count;
	logical  *kind_holder;
	int      kind_size;

protected:

	char     *internal_name;
	virtual ~gvertex();

public:

#ifdef THREAD_SAFE_ACIS
	static safe_integral_type<int> how_many;
#else
	static int how_many;
#endif
    /**
     * Creates an instance of a graph vertex and supplies it with a name.
     * <br><br>
     * @param name
     * name of vertex
     */
	gvertex(char const *name=NULL);
    /**
     * The C++ classes of <tt>gvertex</tt> and gedge are use counted in the same way that laws are use counted.
     * <br><br>
     * <b>Role:</b> That is to say that the are copied by calling the <tt>add</tt> method
     * and deleted by calling the <tt>remove</tt> method.
     */
	void       add() const;
	/**
	 * The C++ classes of <tt>gvertex</tt> and <tt>gedge</tt> are use counted in the same way that laws are use counted.
	 * <br><br>
	 * <b>Role:</b> That is to say that the are copied by calling the <tt>add</tt> method
	 * and deleted by calling the <tt>remove</tt> method.
	 */
	void       remove();
	/**
	 * Every graph vertices can be supplied a character string as a name.
	 * <br><br>
	 * <b>Role:</b> This method returns its name.
	 */
	char const *name() const;
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
	virtual logical isa(int t) const;
	/*
	// tbrv
	*/
	/**
	 * @nodoc
	 */
	static  int     id();
	/*
	// tbrv
	*/
	/**
	 * @nodoc
	 */
	virtual int     type() const;
	/**
	 * Assigns a user-defined <tt>kind</tt> to this graph edge. <tt>kind</tt> is actually a dynamic array.
	 * <br><br>
	 * <b>Role:</b> The value argument specifies whether or not this graph edge
	 * is of the <tt>kind</tt> number specified.
	 * <br><br>
	 * @param which
	 * kind of use.
	 * @param value
	 * turn on or off.
	 */
	void            set_kind(int which,logical value);
	/**
	 * Determines whether or not this graph vertex is of the specified <tt>kind</tt>.
	 * <br><br>
	 * @param which
	 * kind to test.
	 */
	logical         is_kind(int which) const;
	/**
	 * Returns the size of the <tt>kind</tt> array.
	 */
	int				get_kind_size() const;
	/**
	 * Sets the user-defined <tt>kind</tt> array for this graph item to <tt>NULL</tt>. <tt>kind</tt> is actually a dynamic array.
	 * <br><br>
	 * <b>Role:</b> The value argument specifies whether or not this graph edge
	 * is of the <tt>kind</tt> number specified.
	 */
	void            clear_kind();
    /**
     * Determines whether or not the supplied vertex is equal to this graph vertex.
     * <br><br>
     * @param in_vertex
     * supplied vertex.
     */
	logical operator==(gvertex const &in_vertex) const;
	/**
	 * Determines whether or not the supplied vertex is not equal to this graph vertex.
	 * <br><br>
	 * @param in_vertex
	 * gvertex.
	 */
	logical operator!=(gvertex const &in_vertex) const;
	/*
	// tbrv
	*/
	/**
	 * @nodoc
	 */
	virtual logical   same(gvertex const *) const;

	// Other optional methods
    /**
     * This returns a pointer to the entity that the graph vertex refers to.
     * <br><br>
     * <b>Role:</b> Initially, this can be a <tt>CELL</tt> or a <tt>FACE</tt>.
     */
	virtual ENTITY *get_entity() const;
};

// A gvertex class to hold entities
/**
 * Creates an instance of a graph vertex that holds an entity pointer as data.
 * <br><br>
 * <b>Role:</b> This is the same as the <tt>gvertex</tt> class, except that it holds a
 * pointer to an entity that the graph vertex refers to. Initially, this can be
 * a <tt>CELL</tt> or a <tt>FACE</tt>. This is a way for the graph theory tools to interface with
 * a geometric model.
 * <br><br>
 * To make a gvertex or gedge contain data, derive a class from the base classes
 * of <tt>gvertex</tt> and <tt>gedge</tt>. Use a technique similar to the <tt>entity_gvertex</tt> class which
 * enables it to contain an entity pointer.
 * <br><br>
 * @see ENTITY
 */
class DECL_KERN entity_gvertex: public gvertex
{
private:

	ENTITY *internal_entity;

protected:

	virtual ~entity_gvertex();

public:
    /**
     * Creates an instance of a graph vertex which also has a data pointer to an owning entity.
     * <br><br>
     * @param name
     * name of vertex.
     * @param ptr
     * pointer of ENTITY.
     */
	entity_gvertex(char const *name,ENTITY * ptr);
	/*
	// tbrv
	*/
	/**
	 * @nodoc
	 */
	virtual logical same(gvertex const *) const;
	/*
	// tbrv
	*/
	/**
	 * @nodoc
	 */
	logical isa(int t) const;
	/*
	// tbrv
	*/
	/**
	 * @nodoc
	 */
	static  int id();
	/*
	// tbrv
	*/
	/**
	 * @nodoc
	 */
	virtual int     type() const;
    /**
     * Returns a pointer to the entity stored as data on the graph vertex.
     * <br><br>
     * <b>Role:</b> Initially, the entity can be a <tt>CELL</tt> or a <tt>FACE</tt>.
     */
	ENTITY  *get_entity() const;
};

///////////////////////////////////////////////////////////////////////////////
//
//  A class for generic edges of a graph
//
///////////////////////////////////////////////////////////////////////////////
/**
 * Creates an instance of a graph edge for use in graph theory.
 * <br><br>
 * <b>Role:</b> The concepts of vertex, edge, and graph have been implemented as
 * the C++ classes <tt>gvertex</tt>, <tt>gedge</tt>, and <tt>generic_graph</tt>. <tt>(entity_gvertex</tt> is derived
 * from <tt>gvertex</tt> except that it contains a pointer to an entity in the model. Such
 * an entity could be a cell or a face.) A gvertex may be created with an optional
 * char <tt>*name</tt>. A gedge may be created with two gvertex pointers. An empty graph
 * may be created and edges and vertices may be added to it by calling its
 * <tt>add_vertex</tt> and <tt>add_edge</tt> methods. Once created, a graph may be interrogated,
 * ordered, or subsetted in a number of ways.
 * <br><br>
 * The C++ classes of <tt>gvertex</tt> and <tt>gedge</tt> are use counted in the same way that laws
 * are use counted. That is to say that the are copied by calling the add method
 * and deleted by calling the remove method.
 * <br><br>
 * To make a gvertex or gedge contain data, derive a class from the base classes
 * of <tt>gvertex</tt> and <tt>gedge</tt>. Use a technique similar to the <tt>entity_gvertex</tt> class which
 * enables it to contain an entity pointer.
 * <br><br>
 * @see gvertex
 */
class DECL_KERN gedge : public ACIS_OBJECT
{
private:

	int     use_count;
	logical *kind_holder;
	int     kind_size;
	double  weight;

protected:

	gvertex	*v1;
	gvertex	*v2;
	virtual ~gedge();

public:

#ifdef THREAD_SAFE_ACIS
	static safe_integral_type<int> how_many;
#else
	static int how_many;
#endif
    /**
     * Creates an instance of gedge between the two graph vertices supplied.
     * <br><br>
     * @param in_v1
     * vertex one.
     * @param in_v2
     * vertex two.
     * @param in_weight
     * weight of the edge.
     */
	gedge( gvertex const *in_v1,
		   gvertex const *in_v2,
		   double in_weight=0.0);
    /**
     * C++ allocation constructor requests memory for this object but does not populate it.
     */
	gedge();
    /**
     * Returns the first vertex associated with this graph edge.
     */
	gvertex const *vertex1() const;
	/**
	 * Returns the second vertex associated with this graph edge.
	 */
	gvertex const *vertex2() const;
    /**
     * The C++ classes of <tt>gvertex</tt> and <tt>gedge</tt> are use counted in the same way that laws are use counted.
     * <br><br>
     * <b>Role:</b> That is to say that they are copied by calling the <tt>add</tt> method
     * and deleted by calling the <tt>remove</tt> method.
     */
	void    add() const;
	/**
	 * The C++ classes of <tt>gvertex</tt> and <tt>gedge</tt> are use counted in the same way that laws are use counted.
	 * <br><br>
	 * <b>Role:</b> That is to say that they are copied by calling the <tt>add</tt> method
	 * and deleted by calling the <tt>remove</tt> method.
	 */
	void    remove();
    /*
    // tbrv
    */
    /**
     * @nodoc
     */
	virtual logical isa(int t) const;
	/*
	// tbrv
	*/
	/**
	 * @nodoc
	 */
	static  int     id();
	/*
	// tbrv
	*/
	/**
	 * @nodoc
	 */
	virtual int     type() const;
    /**
	 * Assigns a user-defined <tt>kind</tt> to this graph edge. <tt>kind</tt> is actually a dynamic array.
	 * <br><br>
	 * <b>Role:</b> The value argument specifies whether or not this graph edge
	 * is of the <tt>kind</tt> number specified.
	 * <br><br>
	 * @param which
	 * kind of use.
	 * @param value
	 * turn on or off.
	 */
	void            set_kind(int which,logical value);
    /**
	 * Determines whether or not this graph vertex is of the specified <tt>kind</tt>.
	 * <br><br>
	 * @param which
	 * kind to test.
	 */
	logical         is_kind(int which) const;
    /**
	 * Returns the size of the <tt>kind</tt> array.
	 */
	int				get_kind_size() const;
	/**
	 * Gets the weight of the gedge.
	 */
	double          get_weight() const;
	/**
	 * Sets the weight of the gedge.
	 * <br><br>
	 * @param in_weight
	 * weight.
	 */
	void            set_weight(double in_weight) const;
	/**
	 * Sets the user-defined <tt>kind</tt> array for this graph item to <tt>NULL</tt>,<tt>kind</tt> is actually a dynamic array.
	 * <br><br>
	 *<b>Role:</b> The value argument specifies whether or not this graph edge
	 * is of the <tt>kind</tt> number specified.
	 */
	void            clear_kind();
	/**
	 * Returns <tt>TRUE</tt> if the first vertex is the same as the last vertex, thus forming a loop.
	 */
	logical   is_loop() const { return (v1==v2); }
    /**
     * Determines whether or not the supplied graph edge is equal to this graph edge.
     * <br><br>
     * @param in_edge
     * test graph edge.
     */
	logical operator==(gedge const &in_edge) const;
	/**
	 * Determines whether or not the supplied graph edge is not equal to this graph edge.
	 * <br><br>
	 * @param in_edge
	 * test graph edge.
	 */
	logical operator!=(gedge const &in_edge) const;
	/*
	// tbrv
	*/

	/**
	 * @nodoc
	 */
	virtual logical   same(gedge const *) const;

	// Other optional methods
    /**
     * Returns <tt>NULL</tt>.
     */
	virtual ENTITY *get_entity() const;
};

#pragma warning ( pop )

// A gedge class to hold entities
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN entity_gedge: public gedge
{
private:

	ENTITY *internal_entity;

protected:

	virtual ~entity_gedge();

public:

	entity_gedge(gvertex const *in_v1, gvertex const *in_v2, ENTITY *);
	virtual logical same(gedge const *) const;
	logical isa(int t) const;
	static  int id();
	virtual int type() const;

	ENTITY  *get_entity() const;
};


///////////////////////////////////////////////////////////////////////////////
//
//  A generic graph class
//
///////////////////////////////////////////////////////////////////////////////

class gedge_link;
class gvertex_link;
class ENTITY_LIST;
class law;
/**
 * Creates an instance of a graph for the graph theory mathematical operations.
 * <br>
 *<b>Role:</b> The concepts of vertex, edge, and graph have been implemented as
 * the C++ classes <tt>gvertex</tt>, <tt>gedge</tt>, and <tt>generic_graph</tt>. <tt>(entity_gvertex</tt> is derived
 * from <tt>gvertex</tt> except that it contains a pointer to an entity in the model. Such
 * an entity could be a cell or a face.) A gvertex may be created with an optional
 * char <tt>*name</tt>. A gedge may be created with two gvertex pointers. An empty graph
 * may be created and edges and vertices may be added to it by calling its <tt>add_vertex</tt>
 * and <tt>add_edge</tt> methods. Once created, a graph may be interrogated, ordered, or
 * subsetted in a number of ways.
 * <br><br>
 * The <tt>generic_graph</tt> class has methods to tell if a graph is connected, a tree,
 * linear, or a cycle. It also has methods to tell how many components the graph
 * has and to return each of the components as a subgraph. Moreover, the components
 * may be identified by giving an edge or vertex in them.
 */
class DECL_KERN generic_graph : public ACIS_OBJECT
{
private:
	int          use_count;
	gedge_link   *root_edge;
	gvertex_link *root_vertex;
	int          vertex_num;
	int          edge_num;
	int          index_state[4];
	int          number_of_components;
	logical      connected;
	logical      cyclic_order_flag;

	void set_component(gvertex_link *) const;
	void clear_vertex_index(int which) const;
	gedge_link *find_edge_link(gedge const *) const;
	gvertex_link *find_vertex_link(gvertex const *) const;
	gedge_link *find_edge_link(gedge_link const *) const;
	gvertex_link *find_vertex_link(gvertex_link const *) const;
	logical is_cut_vertex(gvertex_link *) const;
	logical is_cut_edge(gedge_link *) const;
	logical is_cycle_vertex(gvertex_link *) const;
	void order_from_link(gvertex_link **,int size);
	void weight_from_link(gvertex_link *);
	void order_cyclic_link(gvertex_link *,gvertex_link *);
	void save_vertex_index(int) const;
	void restore_vertex_index(int) const;
	generic_graph *find_shortest_path(gvertex_link *start,
		                              gvertex_link *end,
									  logical weighted=0.0) const;

protected:

	virtual ~generic_graph();

public:

	// Construction and destruction
	/**
	 * Increments the use count of how many references there are to this <tt>generic_graph</tt> instance.
	 * <br><br>
	 *<b>Role:</b> The object will not be destroyed until all references to it
	 * have been removed.
	 */
	void    add() const;
	/**
	 * Decrements the use count for the generic graph, and destroys the object when the use count reaches zero.
	 */
	void    remove();
	/**
	 * Creates a graph with the given name.
	 * <br><br>
	 * @param in_str
	 * name of the graph.
	 */
	generic_graph(char const *in_str=NULL);
	/**
	 * Adds a vertex to the graph structure by specifying its name.
	 * <br><br>
	 * @param vertex
	 * name of the vertex.
	 */
	void add_vertex(gvertex const * vertex);
	/**
	 * Adds a vertex to the graph structure by specifying a pointer to the graph vertex.
	 * <br><br>
	 * @param ptr
	 * pointer to vertex.
	 */
	void add_vertex(char const * ptr);

	/**
	 * Adds the specified graph edge to the graph structure using its pointer.
	 * <br><br>
	 * @param edge
	 * pointer to edge.
	 */
	void add_edge(gedge const * edge);
	/**
	 * Adds a graph edge to the graph structure by specifying pointers to its vertices.
	 * <br><br>
	 *<b>Role:</b> Optionally an entity can be associated with the graph edge,
	 * to, for example, tie the graph to features of a geometric model.
	 * <br><br>
	 * @param first_vertex
	 * first vertex of the edge.
	 * @param sec_vertex
	 * second vertex of edge.
	 * @param weight
	 * weight.
	 */
	void add_edge(gvertex const * first_vertex,
		          gvertex const * sec_vertex,
				  double weight=0.0);
	/**
	 * Adds a graph edge to the graph structure by specifying pointers to its vertices.
	 * <br><br>
	 * @param first_vertex
	 * first vertex of edge.
	 * @param sec_vertex
	 * second vertex of edge.
	 * @param in_ent
	 * optional entity to associate with edge.
	 */
	void add_edge(gvertex const * first_vertex,
		          gvertex const * sec_vertex,
				  ENTITY *in_ent=NULL);
	/**
	 * Adds the named graph edge to a graph structure.
	 * <br><br>
	 * @param name
	 * name of edge.
	 */
	void add_edge(char const * name);

	// Questions about the vertices
	/**
	 * Returns an array of graph vertices that make up the graph structure.
	 * <br><br>
	 *<b>Role:</b> User must supply a pointer to a variable which represents the
	 * size of the array.
	 * <br><br>
	 * @param size
	 * number of vertices.
	 */
	gvertex **get_vertices(int &size) const;
	/**
	 * Gets a list of all the gvertexes with exactly one gedge (leaves of the tree).
	 * <br><br>
	 * @param size
	 * size of returned array.
	 */
	gvertex **get_leaves(int &size) const;
	/*
	// tbrv
	*/
	/**
	 * @nodoc
	 */
	gvertex *get_root() const;
	/**
	 * Returns the number of graph vertices in the graph structure.
	 */
	int     number_of_vertices() const;
	/**
	 * Determines whether or not the specified graph vertex is a cut vertex.
	 * <br><br>
	 * @param test
	 * test vertex.
	 */
	logical is_cut_vertex(gvertex const * test) const;
	/**
	 * Returns the degree of the specified graph vertex.
	 * <br><br>
	 * @param ptr
	 * pointer to vertex.
	 */
	int     degree(gvertex const * ptr) const;
	/**
	 * Specifies the number of components that part of the graph structure.
	 * <br><br>
	 * @param no
	 * number of components.
	 */
	int     component(gvertex const * no) const;
	/**
	 * Returns an array of graph edges that are adjacent to the specified vertex.
	 * <br><br>
	 *<b>Role:</b> User must supply a pointer to a variable representing the
	 * size of the array.
	 * <br><br>
	 * @param test
	 * test vertex.
	 * @param size
	 * number of edges.
	 */
	gedge   **get_adjacent_edges(gvertex const * test,int &size) const;
	/**
	 * Returns an array of graph vertices that are adjacent to the specified vertex.
	 * <br><br>
	 *<b>Role:</b> User must supply a pointer to a variable representing the
	 * size of the array.
	 * <br><br>
	 * @param test
	 * test vertex.
	 * @param size
	 * number of vertices.
	 */
	gvertex **get_adjacent_vertices(gvertex const * test,int &size) const;
	/**
	 * Returns a pointer to the named graph vertex.
	 * <br><br>
	 * @param name
	 * name to search.
	 */
	gvertex const *find_vertex_by_name(char const *name) const;
	/**
	 * Returns a pointer to the graph vertex by following its model entity.
	 * <br><br>
	 * @param ent
	 * entity to search.
	 */
	gvertex const *find_vertex_by_entity(ENTITY *ent) const;
	/**
	 * Determines whether or not the specified graph vertex is a cycle vertex.
	 * <br><br>
	 * @param test
	 * test vertex.
	 */
	logical is_cycle_vertex(gvertex const * test) const;
	/**
	 * Returns <tt>TRUE</tt> if the given gvertex exists in the graph.
	 * <br><br>
	 * @param in_vertex
	 * gvertex.
	 */
	logical vertex_exists(gvertex const * in_vertex);
	/**
	 * Determines if the two specified gvertexes share a common gedge.
	 * <br><br>
	 * @param first_vertex
	 * first vertex.
	 * @param sec_vertex
	 * second vertex.
	 */
	logical adjacent(gvertex const * first_vertex,gvertex const * sec_vertex) const;

	// Questions about the edges
	/**
	 * Returns an array of graph edges that are part of the graph structure.
	 * <br><br>
	 *<b>Role:</b> User must supply a pointer to a variable representing the
	 * size of the array.
	 * <br><br>
	 * @param size
	 * number of edges.
	 */
	gedge **get_edges(int &size) const;
	/**
	 * Returns the number of graph edges in the graph structure.
	 */
	int     number_of_edges() const;
	/**
	 * Specifies the number of components that part of the graph structure.
	 * <br><br>
	 * @param no
	 * number of components.
	 */
	int     component(gedge const * no) const;
	/**
	 * Locates a graph edge in the graph structure by its specified name.
	 * <br><br>
	 * @param v1
	 * name to search.
	 */
	gedge   const *find_edge_by_name(char const *v1) const;
	/**
	 * Uses the two given entities to find two gvertex's, then uses these two gvertex's to find a gedge that is defined by them.
	 * <br><br>
	 *<b>Role:</b> Returns <tt>NULL</tt> if such gedge does not exist.
	 * <br><br>
	 * @param ent1
	 * first entity.
	 * @param ent2
	 * second entity.
	 */
	gedge	const *find_edge_by_entities(ENTITY *ent1, ENTITY *ent2) const;
	/**
	 * Locates a graph edge in the graph structure by its bounding vertices.
	 * <br><br>
	 * @param first_vertex
	 * first vertex.
	 * @param sec_vertex
	 * second vertex.
	 * @param ref_ent
	 * optionally return only gedges associated with a particular entity.
	 */
	gedge   const *find_edge_by_vertex(gvertex const * first_vertex,
		                               gvertex const * sec_vertex,
									   ENTITY  const *ref_ent = NULL) const;
	/**
	 * Uses the two given gvertexes to find all gedges that connects the gvertexes.
	 * <br><br>
	 *<b>Role:</b> This method returns the number of gedges found. <tt>ge_list</tt> and
	 * target are optional. <tt>ge_list</tt> returns the gedges found. The caller may specify
	 * how many gedges are required by setting a target number. The default target
	 * is <tt>0</tt>, which gets all gedges.
	 * <br><br>
	 * @param first_vertex
	 * first gvertex.
	 * @param sec_vertex
	 * second gvertex.
	 * @param out
	 * ge_list.
	 * @param no
	 * target.
	 */
	int     find_all_edges_by_vertex(gvertex const * first_vertex,
		                             gvertex const * sec_vertex,
								     gedge **&out = *(gedge ***) NULL_REF,
									 int no = 0) const;
	/**
	 * Determines whether or not the specified graph edge is a cut edge.
	 * <br><br>
	 * @param test
	 * test edge.
	 */
	logical is_cut_edge(gedge const * test) const;
	/**
	 * Returns <tt>TRUE</tt> if the graph has no multiple edges.
	 * <br><br>
	 * @param edge
	 * gedge.
	 */
	logical is_simple(gedge const * edge) const;
	// multiple edges are a group (>1) of edges that connect two vertices
	/**
	 * Returns <tt>TRUE</tt> if there is more than one gedge spanning this gedge's vertices.
	 * <br><br>
	 * @param edge
	 * gedge
	 */
	logical is_multiple_edge(gedge const * edge) const;
 	// Questions about the graph
	/**
	 * Determines whether or not the graph structure is a tree.
	 */
	logical is_tree() const;
	/**
	 * Determines whether or not the graph structure is linear.
	 */
	logical is_linear() const;
	/**
	 * Determines whether or not the graph structure is cyclic.
	 */
	logical is_cycle() const;
	/**
	 * Determines whether or not the graph is connected.
	 */
	logical is_connected() const;
	/**
	 * Returns <tt>TRUE</tt> if <tt>in_graph</tt> is a subset of the <tt>THIS</tt> graph.
	 * <br><br>
	 * @param graph
	 * graph that might be a subset of the THIS graph
	 */
	logical is_subset(generic_graph const * graph) const;
	/**
	 * Returns the number of components in the graph structure.
	 */
	int     components() const;
	/**
	 * Lists all entities associated with all gedges and gvertexes of the graph.
	 * <br><br>
	 * @param enty
	 * pointer to entities.
	 * @param use_ordering
	 * ordering on or off.
	 */
	void    get_entities(ENTITY_LIST & enty, logical use_ordering=FALSE) const;
	/**
	 * Returns the sum of the weights of all the gedges in the graph.
	 */
	double  total_weight() const;
	/**
	 * Lists all entities associated with all gvertexes of the graph
	 * <br><br>
	 * @param enty
	 * list of entities.
	 * @param use_ordering
	 * ordering on or off.
	 */
	void get_entities_from_vertex(ENTITY_LIST & enty, logical use_ordering=FALSE) const;
	/**
	 * Lists all entities associated with all gedges of the graph
	 * <br><br>
	 * @param enty
	 * list of entities.
	 */
	void  get_entities_from_edge(ENTITY_LIST & enty) const;

	// Subgraphs
	/**
	 * Returns a graph structure that represents the shortest path between the two specified graph vertices.
	 * <br><br>
	 *<b>Role:</b> If weighted is <tt>FALSE</tt>, the method will return the shortest path
	 * (fewest number of gvertexes in it.) If weighted is <tt>TRUE</tt>, the method will
	 * return the lightest path (where the sum of all the weights applied to gvertexes
	 * and gedges is lowest.)
	 * <br><br>
	 * @param start_vertex
	 * starting vertex.
	 * @param end_vertex
	 * ending vertex.
	 * @param weighted
	 * shortest(false) or lightest(true).
	 */
	generic_graph *find_shortest_path(gvertex const * start_vertex,gvertex const * end_vertex,logical weighted=FALSE) const;
	/**
	 * Returns a graph structure which represents the shortest cycle that contains the given graph vertex.
	 * <br><br>
	 * @param start_vertex
	 * starting vertex.
	 */
	generic_graph *find_shortest_cycle(gvertex const * start_vertex) const;
	/**
	 * The subset method with two integers takes a and b and returns a subgraph in one of two ways.
	 * <br><br>
	 * <b>Role:</b> If a&lt;b, then the set of all vertices with orders between a
	 * and b is returned along with all edges that have both of their adjacent
	 * vertices in this set.
	 * <br>
	 * If b&lt;a, then the set of all vertices with orders not between a and b is
	 * returned along with all edges that have both of their adjacent vertices
	 * in this set.
	 * <br><br>
	 * @param a
	 * integer a.
	 * @param b
	 * integer b.
	 */
	generic_graph *subset(int a,int b) const;
	/**
	 * The subset method with a law returns the set of all vertices such that their order evaluates as true along with the all edges that have both of their adjacent vertices evaluating as true orders.
	 * <br><br>
	 * @param eval_law
	 * law for evaluation.
	 */
	generic_graph *subset(law * eval_law) const;
	/**
	 * Specifies the number of components that part of the graph structure.
	 * <br><br>
	 * @param no
	 * number of components.
	 */
	generic_graph *component(int no) const;
	/**
	 * Unites this graph with the specified graph. Graph edges and vertices only appear once.
	 * <br><br>
	 * @param graph
	 * graph to add.
	 */
	generic_graph *unite(generic_graph * graph) const;
	/**
	 * Removes the specified graph from this graph structure.
	 * <br><br>
	 * @param graph
	 * graph to remove.
	 * @param keep
	 * flag for keep.
	 */
	generic_graph *subtract(generic_graph * graph,logical keep) const;
	/**
	 * Subtracts the gedges of the input graph from the full graph.
	 * <br><br>
	 * @param graph
	 * input graph.
	 */
	generic_graph *subtract_edges(generic_graph *graph) const;
	/**
	 * Returns a graph structure that represents the intersection of this graph structure with the specified test graph structure.
	 * <br><br>
	 * @param graph
	 * test graph.
	 */
	generic_graph *intersect(generic_graph * graph) const;
	/**
	 * This returns a new graph structure containing all of the graph vertices that are considered cut vertices.
	 * <br><br>
	 * <b>Role:</b> Cut vertices are defined as those vertices whose removal
	 * results in more graph components than were originally present.
	 */
	generic_graph *cut_vertices() const;
	/**
	 * This returns a new graph structure containing all of the graph vertices that are considered cut vertices.
	 * <br><br>
	 * <b>Role:</b> Cut vertices are defined as those vertices whose removal results
	 * in more graph components than were originally present.
	 */
	generic_graph *cut_edges() const;
	/**
	 * This returns a new graph structure containing all of the graph edges that are considered cycle edges.
	 * <br><br>
	 * <b>Role:</b> Cycle edges are defined as the shortest path through a graph
	 * structure resulting in a closed loop.
	 */
	generic_graph *cycle_edges() const;
	/**
	 * Copies the graph structure into another graph structure.
	 */
	generic_graph *copy() const;
	/**
	 * This assigns a user-defined <tt>kind</tt> and its on/off status to the graph structure.
	 * <br><br>
	 * @param which
	 * kind to test.
	 * @param value
	 * on or off.
	 */
	generic_graph *kind(int which,logical value=TRUE) const;
	/**
	 * Returns a graph of branches off of a specified portion of the given trunk.
	 * <br><br>
	 * @param trunk
	 * linear trunk portion of the graph.
	 * @param order
	 * nth gvertex on trunk.
	 * @param keep_trunk
	 * if true include segments of the branch, if false include branches.
	 */
	generic_graph *branch(generic_graph *trunk,int order,logical keep_trunk) const;
	/**
	 * Returns a graph of branches off of a specified portion of the given trunk.
	 * <br><br>
	 * @param trunk
	 * linear trunk portion of the graph.
	 * @param which
	 * sub-portion of trunk from which to collect branches.
	 * @param keep_trunk
	 * if true include segments of the branch, if false include branches.
	 */
	generic_graph *branch(generic_graph *trunk,generic_graph *which,logical keep_trunk) const;
	/*
	// tbrv
	*/
	/**
	 * @nodoc
	 */
	void           mark_branches(generic_graph *mark_graph=NULL);
	/**
	 * Finds all branches in the graph and return a set of subgraphs that do not have a branch.
	 * <br><br>
	 * @param out_graphs
	 * subgraph list.
	 */
	int  split_branches(generic_graph **&out_graphs);

	// Methods to order a graph
	/**
	 * The order-from method of ordering a graph works well for trees and linear graphs.
	 * <br><br>
	 * <b>Role:</b> The two graphs show in the Ordering Graphs figure have been
	 * ordered by distance from vertex A.
	 * <br><br>
	 * For images illustrating this method, look under Technical Articles of the Kernel Component at <tt>"Classes with Images"</tt>.
	 * <br><br>
	 * @param vertex
	 * starting vertex.
	 */
	void order_from(gvertex const * vertex);
    /**
	 * The order-from method of ordering a graph works well for trees and linear graphs.
	 * <br><br>
	 * <b>Role:</b> The two graphs show in the Ordering Graphs figure have been
	 * ordered by distance from vertex A.
	 * <br><br>
	 * For images illustrating this method, look under Technical Articles of the Kernel Component at <tt>"Classes with Images"</tt>.
	 * <br><br>
	 * @param graph
	 * graph.
	 */
	void order_from(generic_graph * graph);
	/**
	 * If a graph is cyclic, then it may be ordered by the <tt>order_cyclic</tt> method.
	 * <br><br>
	 * <b>Role:</b> This sets a given vertex's order to zero and the other vertices
	 * in a cyclic order as shown in the following figure.
	 * <br><br>
	 * For images illustrating this method, look under Technical Articles of the Kernel Component at <tt>"Classes with Images"</tt>.
	 * <br><br>
	 * @param first_vertex
	 * first vertex.
	 * @param last_vertex
	 * last vertex.
	 */
	void order_cyclic(gvertex const * first_vertex,gvertex const * last_vertex);
	/**
	 * Another way to order a graph G is to order it with respect to an ordered graph H such that G is a subgraph of H.
	 * <br><br>
	 * <b>Role:</b> The <tt>order_with</tt> method imposes the order of H onto G and rescales
	 * the ordering on G to remove gap. The type of ordering (i.e. cyclic or not)
	 * is inherited from the ordered graph H. If the compress option is turned on,
	 * the resulting gvertexes are numbered sequentially. In the example below,
	 * gvertexes in the uncompressed result would be numbered 124, but in the
	 * compressed result would be numbered 012.
	 * <br><br>
	 * For images illustrating this method, look under Technical Articles of the Kernel Component at <tt>"Classes with Images"</tt>.
	 * <br><br>
	 * @param graph
	 * other graph.
	 * @param compress
	 * remove gaps in ordering if True.
	 */
	void order_with(generic_graph * graph,logical compress=TRUE);
	/**
	 * Once a graph has been ordered, its ordering may be negated with this method.
	 * <br><br>
	 * <b>Role:</b> Negation is a special operation and returns different results
	 * for cycles and trees depending upon the start vertex. In the following
	 * figure, the graph vertex A was initially 0 in the ordering before the negation
	 * operation.
	 * <br><br>
	 * For images illustrating this method, look under Technical Articles of the Kernel Component at <tt>"Classes with Images"</tt>.
	 */
	void negate();
	/**
	 * Once a graph has been ordered, the order of a vertex may be found by calling the <tt>get_order</tt> method.
	 * <br><br>
	 * @param vertex
	 * pointer to vertex.
	 */
	int  get_order(gvertex const * vertex) const;
	/**
	 * Manually assigns an order to a gvertex in a graph.
	 * <br><br>
	 * @param vertex
	 * gvertex.
	 * @param order
	 * order.
	 */
	void set_order(gvertex const * vertex, int order);
	/**
	 * Once a graph has been ordered,the maximum order in the graph may be found by calling the <tt>max_order</tt> method.
	 */
	int  max_order() const;
	/**
	 * Once a graph has been ordered, the minimum order in the graph may be found by calling the <tt>min_order</tt> method.
	 */
	int  min_order() const;

	// Methods for <tt>kinds</tt> info
	/**
	 * Returns the largest number of <tt>kinds</tt> used to mark any gvertex or gedge.
	 * <br><br>
	 * <b>Role:</b> This is useful for determining the number of the next unused <tt>kind</tt>.
	 */
	int  max_kind() const;
	/**
	 * Sets the user-defined <tt>kind</tt> array for this graph item to <tt>NULL</tt>.
	 * <br><br>
	 * <b>Role:</b> <tt>kind</tt> is actually a dynamic array that can be used to assign
	 * arbitrary flags to gedges and gvertexes on the graph.
	 */
	void clear_kind();
	/**
	 * Turn the given <tt>kind</tt> on or off for all gvertexes and gedges in the reference graph.
	 * <br><br>
	 * <b>Role:</b> The reference graph is a subset of the full graph.
	 * <br><br>
	 * @param graph
	 * reference graph.
	 * @param which
	 * which kind.
	 * @param value
	 * turn kind on if True or off if False.
	 */
	void set_kind(generic_graph * graph, int which, logical value=TRUE);

	/**
	 * @nodoc
	 */
	bool subtract_edge( gedge * edge );
	/**
	 * @nodoc
	 */
	bool subtract_isolated_vertex( gvertex * vertex );
};

/*
graph
graph:add-edge
graph:add-vertex
graph:is-tree
graph:is-linear
graph:is-cycle
graph:is-connected
graph:components
graph:shortest-path
graph:cut-vertices
graph:cut-edges
graph:is-cut-edge
graph:is-cut-vertex
graph:subset
graph:copy
graph:which-component
graph:component
graph:unite
graph:subtract
graph:intersect
graph:shortest-cycle
graph:is-cycle-vertex
graph:get-order
graph:order-cylic
graph:order-with
graph:order-from
graph:show-order
graph:negate
graph:kind
graph:entities
graph:set_kind
graph:kind?
graph:kinds?
*/

///////////////////////////////////////////////////////////////////////////////
//
// Other classes not to be used by anyone other than the methods of
// generic_graph
//
///////////////////////////////////////////////////////////////////////////////
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN gedge_link : public ACIS_OBJECT
{
private:

	gedge        *internal_data;
	gedge_link   *internal_next;
	gvertex_link *internal_v1;
	gvertex_link *internal_v2;

	// Note changing the data in the index array is not to be considered
	// changing the same as changing the graph.  Hence fucntions that do
	// this may still be const functions.

	int          internal_index[1];   // 0 used to temp remove 0 means removed

public:

	gedge_link(gedge const *in_data,gedge_link *in_next);
	~gedge_link();

	gedge        const *data() const;
	gedge_link   *next() const;
	void         set_next(gedge_link *); // needed so vertex can set new first edge in its edge list
	gvertex_link *vertex() const;
	gvertex_link *other(gvertex_link *) const;
	void         add_vertex(gvertex_link *) const;
	int          index(int which) const;
	void         set_index(int which,int what) const;
};
/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_KERN gvertex_link : public ACIS_OBJECT
{
private:

	gvertex      *internal_data;
	gvertex_link *internal_next;
	gedge_link   *internal_edges;
	int          edge_num;

	// Note changing the data in the index array is not to be considered
	// the same as changing the graph.  Hence fucntions that do so may
	// still be const functions.

	int          internal_index[4];   //  0 used for ordering
	                                  //  1 used for component number
									  //  2 used to temp remove 0 means removed
									  //    or for marking
									  //  3 used to save an index
	double       internal_weight;

public:

	gvertex_link(gvertex const *in_data,gvertex_link *in_next);
	~gvertex_link();

	void         add_edge(gedge_link *);
	gvertex      const *data() const;
	gvertex_link *next() const;
	gedge_link   *edges() const;
	void         set_first_edge(gedge_link *target_edge); // does nothing if target_edge is not in list
	int          index(int which) const;
	void         set_index(int which,int what) const;
	int          degree() const;
	void         set_weight(double in_weight) const;
	double       weight() const;
	bool		 subtract_edge( gedge* edge );
	void		 set_next( gvertex_link *);
};

// Other functions
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN generic_graph *create_graph_from_faces(ENTITY_LIST &face_list);
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN generic_graph *create_graph_from_edges(ENTITY_LIST &edge_list);

// Creates a graph from a list of FACES and a list of EDGES.
// The vertices of the graph consist FACE entities and EDGE entities.
// The edges of the graph consist of:
// 1) FACES that are adjacent.
// 2) EDGES that are adjacent.
// 3) A FACE/EDGE pair that is adjacent.
// NOTE: The edge_list should only consist of isolated edges from wires,
// not edges that are part of the given faces.
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_KERN generic_graph *create_graph_from_faces_and_edges(ENTITY_LIST &face_list, ENTITY_LIST &edge_list);
/** @} */
#endif

