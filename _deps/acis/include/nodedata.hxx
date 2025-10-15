/* ORIGINAL: acis2.1/faceter/attribs/nodedata.hxx */
// $Id: nodedata.hxx,v 1.11 2002/08/09 17:21:51 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _NODEDATA
#define _NODEDATA



#include "dcl_fct.h"
// ywoo 08Jan01: added the header file.
#include "mmgr.hxx"
// ywoo: end

#ifndef NODE_DATA_CELL_TYPE
#define NODE_DATA_CELL_TYPE double
#endif

#define NODE_DATA_CELL_NULL_VALUE 0.0
class SPAunit_vector;
class SPAposition;
//**************************************************
// Lookup table interface for 'token types' in mesh nodes.
//
// A node_mapping is a mapping from a set of token types
//	to positions in an array of NODE_DATA_CELL_TYPE cells.
//
// A node_instance is an array of NODE_DATA_CELL_TYPE cells.
//
// IMPLEMENTATION:
//
// 1) Assume that node instances far outnumber node_mappings, and
//		are accessed repeatedly.  Therefore speed of access to the
//		node_mapping is far more important than flexibility of the mapping
//		structure.
// 2) The POSITION_TOKEN is gets special treatment -- it is explicitly
//		allocated within the node instance.  Other token types are
//		reached via the pointer to an array of cells.
// 3) The af_node_mapping is a pure data class -- no pointers go out,
//		and no use count tracking.  The expected 'use' is that some ENTITY
//		will define an af_node_mapping as a member item, and the entity takes
//		responsibility for those issues.
//
//****************************************************
#ifndef PARAMETER_TOKENS
#define PARAMETER_TOKENS

// STIPORT TAC gotta be long on 64-bit machines
#if defined( osf1 ) || defined( __alpha )
typedef long parameter_token;
#else
typedef int parameter_token ;
#endif

#define INVALID_TOKEN       -1

#define POSITION_TOKEN       0
#define NORMAL_TOKEN         1
#define COLOUR_TOKEN         2
#define COLOR_TOKEN          2
#define TRANSPARENCY_TOKEN   3
#define TEXTURE_COORDS_TOKEN 4
#define UV_TOKEN             5
#define UV_DERIVS_TOKEN      6
#define UV_CHANGE_TOKEN      7
#define POINTER_TOKEN        8

// STIPORT athena had to remove , after VECTOR_TOKEN_TYPE to compile on aix,osf and ultrix
enum token_type
     {
        FLOAT_TOKEN_TYPE = 0,
        POINT_TOKEN_TYPE,
        UNIT_VECTOR_TOKEN_TYPE,
        COLOUR_TOKEN_TYPE,
        VECTOR_TOKEN_TYPE,
		POINTER_TOKEN_TYPE
     };
#endif

#define PREDECLARED_TOKEN_TYPES 9

/*
// tbrv
*/
/**
 * @nodoc
 */
typedef struct {
	parameter_token id;
	token_type type;		// From the enumeration above.
	int ninstance;			//	Number of instances of the token_type
	int ncell;				//	Number of cells
	int offset;				//	Offset within data array
 }TOKEN_DEF;


/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_FCT af_node_mapping : public ACIS_OBJECT {
	int allocated_cells;
	TOKEN_DEF mapping[	PREDECLARED_TOKEN_TYPES ];
 public:
	af_node_mapping();
	af_node_mapping(int ntoken, parameter_token token[]);
	~af_node_mapping();

	// STI dgh int extra_cells();
	int extra_cells() const;

	int same(af_node_mapping *that);
	void copy(af_node_mapping *from);

	void update(int ntoken,parameter_token token_type[]);
	int defined(parameter_token id);
	int locate(parameter_token id,int &offset,int &count) const;

	void save();
	void restore();
	void merge( af_node_mapping *other );
	void print(FILE *to);
};

class DECL_FCT af_node_instance : public ACIS_OBJECT
{
	private:
		int use_count;
		int ncell;
		NODE_DATA_CELL_TYPE X[3];
		NODE_DATA_CELL_TYPE *data;
	public:
		// STI dgh af_node_instance(af_node_mapping *mapping = (af_node_mapping*)NULL);
		af_node_instance(const af_node_mapping *mapping = (af_node_mapping*)NULL);
		af_node_instance(af_node_instance const &);
		~af_node_instance();

		void use();
		void lose();

		int count() { return use_count; }

	// Queries. Each returns FALSE if the data is not available.
		int normal(const af_node_mapping *,SPAunit_vector &);
		int point(SPAposition &);
		int get_parameter_data(const af_node_mapping *T,parameter_token,
			NODE_DATA_CELL_TYPE *values);

	// Data installation.  Each is a noop if the template does not have this
	//	data item.
		void set_normal(const af_node_mapping *,const SPAunit_vector &);
		void set_point(const SPAposition &);
		void set_parameter_data(const af_node_mapping *T,parameter_token,
			const NODE_DATA_CELL_TYPE *values);

};

//



#endif
