// $Id: gen_render.hxx,v 1.22 2002/08/09 17:31:46 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef GENERIC_RENDERING_HXX
#define GENERIC_RENDERING_HXX

#include "acis.hxx"

class ENTITY;
class SPAtransf;
class pick_ray;
class SPAposition;
class SPAvector;
class asm_model;
class acis_pm_entity_mgr;

#include "api.hxx" // For outcome // For outcome
#include "efilter.hxx"
#include "imm_draw.hxx"

#include "lists.hxx"
#include "param.hxx"

#include "rgbcolor.hxx"
#include "draw_dt.hxx"

#include "gen_pick.hxx"
#include "scheme.hxx"

class PART;
class component_entity_handle;

logical InitializeRendering();
logical TerminateRendering();

outcome
TempRenderEntity(
	ENTITY* e,
	const SPAtransf *st,
	rgb_color col
	);

rgb_color FindColor(ENTITY* ent, rgb_color def);

outcome RefreshActive();

double UnitsPerPixel();

outcome HighlightEntity(
	ENTITY* e,
	logical on // TRUE to highlight, FALSE to remove highlight
	);

outcome HighlightComponentEntity(
	component_entity_handle* ce,
	logical on // TRUE to highlight, FALSE to remove highlight
	);

outcome GetHighlight(
    ENTITY*            ent,         // The ENTITY to test
    logical&           onoff        // TRUE if it is highlighted
    );

outcome SetHighlightColor(
    int                color        // the highlight color
    );

outcome SetHighlightColor(
    const rgb_color&   color        // the highlight color
    );

outcome GetHighlightColor(
	rgb_color&		   color		// the highlight color
	);

outcome SetActiveWCSColor(
    const rgb_color&   color        // the Active WCS color
    );

outcome GetActiveWCSColor(
	rgb_color&						// the Active WCS color
	);


outcome SetEntityColor(
    ENTITY*           ent,           // ENTITY to modify
    int               color          // new color index
    );

outcome EraseModel(
   asm_model*		model
   );

outcome EraseEntity(
    ENTITY*           ent           // The ENTITY to erase
    );

outcome DisplayEntity(
    ENTITY*           ent           // The ENTITY to display
    );

outcome SetEntityColor(
    ENTITY*           ent,           // ENTITY to modify
    rgb_color         color          // new color
    );

outcome GetEntityColor(
    ENTITY*           ent,          // ENTITY to look at
    rgb_color&        color         // color of entity
    );

outcome GetDefaultColor(
    rgb_color&        color         // default color
    );
outcome GetDefaultFaceColor(
    rgb_color&        color         // default color
    );
outcome GetDefaultEdgeColor(
    rgb_color&        color         // default color
    );
outcome GetDefaultVertexColor(
    rgb_color&        color         // default color
    );

outcome GetDefaultBackGroundColor(
    rgb_color&        color         // default color
    );

outcome SetDefaultColor(
	const rgb_color&	color		// default color
	);
outcome SetDefaultFaceColor(
	const rgb_color&	color		// default color
	);
outcome SetDefaultEdgeColor(
	const rgb_color&	color		// default color
	);
outcome SetDefaultVertexColor(
	const rgb_color&	color		// default color
	);
outcome SetDefaultBackGroundColor(
	const rgb_color&	color		// default color
	);
outcome IsDisplayed(
	const ENTITY*     ent,			// the ENTITY to test
	logical&          displayed		// return value
	);

point_style GetPointStyle();
void SetPointStyle( point_style style );

int GetPointSize();
void SetPointSize( int size );
int Get_HA_Map_Entries(void);
acis_pm_entity_mgr* CreateEntityManager(PART* part);

// For use with entity_test_filter
logical IsDisplayed(const ENTITY* ent);

void UpdateEntityDisplay(ENTITY*);

class CURVE;
class SchemeView;

void TempSketchCurve(
	CURVE *this_curve,
	SPAparameter start_param,
	SPAparameter end_param,
	const SPAtransf* sketch_transform,
	SchemeView* view
	);

void FlushGraphics();

ScmObject
DeleteRenderingContext(
	PART* part,
	const char* context
	);

ScmObject
PartViews(
	PART* part
	);

PART* PartFromView(SchemeView* v);
void RenderView(ENTITY_LIST& elist, SchemeView* v);

#ifdef NT
void mouse_event(
    MouseEventType type,
    int button,
    HWND hWnd,
    WPARAM wParam,
    LPARAM lParam
    );

void init_DISPLAY_instance(
	HINSTANCE hInstance
	);
#endif // NT


void DisplaySurfacePolys(logical on);
void DisplayFacets(logical on);
void DisplayParamLines(logical on);
void DisplaySurfacePolys(logical on);
void DisplayFaceNormals(logical on);
void DisplayFacetVertexNormals(logical on);

struct display_arrow_properties
{
	enum arrow_head_type
	{
		double_outside = -1,  // double arrows  -->->
		simple         =  0,  // a simple arrow ---->
		double_inside  =  1,  // double arrows  --<->
	} head_type;

	enum arrow_color
	{
		ARROW_YELLOW,
		ARROW_CYAN,
		ARROW_BLUE,
		ARROW_GREEN,
		ARROW_RED
	} color;
};


void DisplayArrow( 
			display_arrow_properties const& arrow_type,
			SPAposition const& arrow_base, 
			SPAvector const& arrow_vec 
			);

extern ScmObject create_scm_view(int argc, ScmObject *argv);

/*
void AddPolyline(int num_positions, SPAposition *array_of_positions, const char *c="polyline", rgb_color color=rgb_color(1.0,1.0,1.0));
void AddText(const SPAposition&, const char*, const char *c="text", rgb_color color=rgb_color(1.0,1.0,1.0));
void AddPoint(const SPAposition&,int size, const char *c="point", rgb_color color=rgb_color(1.0,1.0,1.0));
void RemoveDisplayType(const char*);
*/

class scm_immediate_draw : public IACISImmediateDraw
{
	void get_current_rgb(float &r,float &g,float &b);
	void set_current_rgb(float r,float g,float b);

	void draw_polyline_3d(
        int npts,               // Number of points
        const float pts[],      // 3 values (X, Y, Z) for each point
        logical fill = FALSE    // Fill polygon?
        );
    void draw_point_3d(
        const SPAposition &pos     // Coordinate at which to display point
        );
    void draw_text_3d(
        const SPAposition &pos,    // Position for start of text
        const char* text        // Text to be displayed
        );
    void draw_curve(
		curve *this_curve,
		SPAparameter start_param,
		SPAparameter end_param,
		const SPAtransf* sketch_transform
		);
    void draw_CURVE(
		CURVE *this_curve,
		SPAparameter start_param,
		SPAparameter end_param,
		const SPAtransf* sketch_transform
		);
	virtual void draw_EDGE(EDGE *);
};

#endif //  GENERIC_RENDERING_HXX
