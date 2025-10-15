/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef HH_GLOBALARC__HXX
#define HH_GLOBALARC__HXX
//--------------------------------------------------------------------

#include 	"acis.hxx"
#include 	"dcl_heal.h"
#include	"hh_arc.hxx"
#include	"glb_node.hxx"


//--------------------------------------------------------------------

#define HH_GlobalArc_LEVEL (HH_Arc_LEVEL + 1)
extern int DECL_HEAL HH_GlobalArc_TYPE;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(HH_GlobalArc, HEAL)
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

//--------------------------------------------------------------------
enum HH_GlobalArcType
{
	hh_arc_across_edge,
	hh_arc_across_vertex
};

enum HH_GlobalArcSucess
{
	hh_arc_unset,
	hh_arc_sucess,
	hh_arc_fail
};

class DECL_HEAL HH_GlobalArc 	: public HH_Arc
{
protected:
	HH_GlobalArcType 		 arcType;
	HH_GlobalArcSucess		 arcSucess;

public:
	HH_GlobalArc( HH_GlobalNode * , HH_GlobalNode *,
				  HH_ArcDir = hh_nodir ,HH_GlobalArcType =hh_arc_across_edge);

public:
	HH_GlobalArc();
		
	static HH_GlobalArc *
	create( HH_GlobalNode* , HH_GlobalNode*,
			HH_ArcDir = hh_nodir,HH_GlobalArcType =hh_arc_across_edge );

	void set_type(HH_GlobalArcType p);

	void set_arc_sucess(HH_GlobalArcSucess s);

	HH_GlobalArcType get_type();

	HH_GlobalArcSucess get_arc_sucess();

	virtual void cleanup();

	virtual void set_direction(HH_ArcDir d);

	/**
	 * @nodoc
	 */
	ENTITY_FUNCTIONS( HH_GlobalArc, HEAL)
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ENTITY_FUNCTIONS changes access to "public", do so
// explicitly here:
public:

};


//--------------------------------------------------------------------
#endif
