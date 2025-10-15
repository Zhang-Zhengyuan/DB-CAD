/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef BODY_MESH_HPP
#define BODY_MESH_HPP

#include <stdio.h>

#include "acis.hxx"
#include "api.hxx"
#include "kernapi.hxx"
#include "body.hxx"
#include "lump.hxx"
#include "shell.hxx"
#include "face.hxx"
#include "facet.err"
#include "af_api.hxx"
#include "gmeshmg.hxx"
#include "fct_utl.hxx"

class BodyMeshPolygon;

class BodyMeshNode: public ACIS_OBJECT
{
public:
	SPAposition			m_Pos;
	SPApar_pos			m_UV;
	double				m_TParam;
	ENTITY*				m_Entity;
	BodyMeshPolygon*	m_FirstSharingPolygon;
	int					m_FirstSharingPolynodeIndex;
	int 				m_FacetLineCount;
	int 				m_FacetLineOnEDGECount;

	BodyMeshNode()
	{
		m_TParam = 0;
		m_Entity = 0;
		m_FirstSharingPolygon = 0;
		m_FirstSharingPolynodeIndex = 0;
		m_FacetLineCount = 0;
		m_FacetLineOnEDGECount = 0;
	}
	~BodyMeshNode() {}
};

class BodyMeshPolynode: public ACIS_OBJECT
{
public:
	BodyMeshNode*		m_Node;
	BodyMeshPolygon*	m_NextSharingPolygon; // some day, make this m_PolygonMate
	int					m_NextSharingPolynodeIndex;

	BodyMeshPolynode()
	{
		m_Node = 0;
		m_NextSharingPolygon = 0;
		m_NextSharingPolynodeIndex = 0;
	}
};

class BodyMeshPolygon: public ACIS_OBJECT
{
public:
	BodyMeshPolygon() {}
	~BodyMeshPolygon() {}
	int					m_PolynodeCount;
	BodyMeshPolynode*	m_Polynodes;
};

class DECL_FCT BODY_MESH: public ACIS_OBJECT
{
private:
	int					m_PolygonTotal;
	int					m_PolynodeTotal;
	int					m_NodeTotal;
	int					m_PolygonCount;
	int					m_PolynodeCount;
	int					m_NodeCount;
	BodyMeshPolygon*	m_Polygons;
	BodyMeshPolynode*	m_Polynodes;
	BodyMeshNode*		m_Nodes;
	int					m_RefCount;
public:
	BODY_MESH();
	~BODY_MESH();
	void addRef() { m_RefCount++; }
	void release();
	void
	initialize(
				int inPolygonTotal,
				int inPolynodeTotal,
				int inNodeTotal);
	int 
	add_polygon(
				int inPolygonIndex,
				int inPolynodeCount,
				int inShare);
	int 
	set_polygon_node(
				int	inPolygonIndex,
				int	inPolynodeIndex,
				void*	inID);
	BodyMeshNode* 
	add_node(
				int					inNodeIndex,
				ENTITY*				inEntity,
				const SPAposition&	inPos,
				const SPApar_pos*	inUV,
				double				inTparam);
	int get_num_node() { return m_NodeCount; }
	int get_num_polygon() { return m_PolygonCount; }
	int get_num_polynode() { return m_PolynodeCount; }
	int get_num_polynode(int inPolygonIndex);
	logical	get_node(int inPolygonIndex, int inPolynodeIndex, BodyMeshNode*& outNode);
	logical	get_node(int inNodeIndex, BodyMeshNode*& outNode);
};

#endif
