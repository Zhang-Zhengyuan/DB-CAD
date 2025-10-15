// $Id: gen_pick.hxx,v 1.5 2002/06/12 15:31:34 jhauswir Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef GENERIC_PICKING_HXX
#define GENERIC_PICKING_HXX

#include "api.hxx" // For outcome // For outcome
#include "pick_ray.hxx"
#include "efilter.hxx"
#include "edge.hxx"

#include "wndo_dt.hxx"

class asm_model_list;
class component_entity_handle_list;


//======================================================================
// define masks for setting the button state

/* Mouse event types */
typedef enum {
    G_MOUSE_MOVE,
    G_MOUSE_BUTTON_DOWN,
    G_MOUSE_BUTTON_UP,
    G_MOUSE_DBL_CLICK,
    G_MOUSE_ENTER_WINDOW,
    G_MOUSE_LEAVE_WINDOW
} MouseEventType;

#ifdef X_WINDOWS
#include <X11/X.h>

#define PE_LEFT_BUTTON Button1
#define PE_MIDDLE_BUTTON Button2
#define PE_RIGHT_BUTTON Button3
#define PE_BUTTON_4 Button4
#define PE_BUTTON_5 Button5
#define PE_LEFT_BUTTON_MASK Button1Mask
#define PE_MIDDLE_BUTTON_MASK Button2Mask
#define PE_RIGHT_BUTTON_MASK Button3Mask
#define PE_SHIFT_MASK ShiftMask
#define PE_CONTROL_MASK ControlMask
#define PE_ALT_MASK Mod1Mask

#endif

#ifdef NT
#include "windows.h"

#define PE_LEFT_BUTTON 1
#define PE_MIDDLE_BUTTON 2
#define PE_RIGHT_BUTTON 3
#define PE_BUTTON_4 4
#define PE_BUTTON_5 5
#define PE_LEFT_BUTTON_MASK MK_LBUTTON
#define PE_MIDDLE_BUTTON_MASK MK_MBUTTON
#define PE_RIGHT_BUTTON_MASK MK_RBUTTON
#define PE_SHIFT_MASK MK_SHIFT
#define PE_CONTROL_MASK MK_CONTROL
#define PE_ALT_MASK 0

#endif

#if defined( mac ) && !defined( macos_a64 ) && !defined( macos_a64_debug )
#include "carbonheader.h"

#define PE_LEFT_BUTTON 1
#define PE_MIDDLE_BUTTON 2
#define PE_RIGHT_BUTTON 3
#define PE_BUTTON_4 4
#define PE_BUTTON_5 5
#define PE_LEFT_BUTTON_MASK btnState
#define PE_MIDDLE_BUTTON_MASK 0
#define PE_RIGHT_BUTTON_MASK 0
#define PE_SHIFT_MASK shiftKey
#define PE_CONTROL_MASK controlKey
#define PE_ALT_MASK optionKey
#define PE_CMD_MASK cmdKey

#endif

#if defined( OS_Darwin ) //&& ( defined( macos_a64 ) || defined( macos_a64_debug) )
#define PE_LEFT_BUTTON 1
#define PE_MIDDLE_BUTTON 4
#define PE_RIGHT_BUTTON 3
#define PE_BUTTON_4 4
#define PE_BUTTON_5 5
#define PE_LEFT_BUTTON_MASK 0
#define PE_MIDDLE_BUTTON_MASK 0
#define PE_RIGHT_BUTTON_MASK 0
#define PE_SHIFT_MASK 0x02000000 
#define PE_CONTROL_MASK 0x04000000
#define PE_ALT_MASK 0x08000000
#define PE_CMD_MASK 0x10000000
#endif

//======================================================================

class SchemeView;

class PickEvent: public ACIS_OBJECT
{

  private:
    int Pick_X;
    int Pick_Y;
    int Pick_Button;
    window_handle Pick_View;
    unsigned int Button_State;

  public:
    PickEvent();
	PickEvent(int x, int y, int button, window_handle win);
	PickEvent(int x, int y, int button, window_handle win, unsigned int state);

    int x() const {return Pick_X;}
    int y() const {return Pick_Y;}
    int button() const {return Pick_Button;}
	window_handle window() const;
	SchemeView *view() const;
	logical activate_view() const;
    unsigned int state() const {return Button_State;}

    void set_x(int x) {Pick_X = x;}
    void set_y(int y) {Pick_Y = y;}
    void set_button(int button) {Pick_Button = button;}
	void set_window(window_handle win);

    void set_state(unsigned int state) {Button_State = state;}
    unsigned int add_state(unsigned int state);
    logical has_state(unsigned int state) const;

	// Test it two gen_pick_events are the same
	int operator==(const PickEvent&) const;

	// See if the shift key was down
	logical shift_state() const;

	// See if the control key was down
	logical control_state() const;

	// See if the alt key was down
	logical alt_state() const;

	// Test for specific mouse buttons
	logical left_button() const;
	logical right_button() const;
	logical middle_button() const;
};

outcome PickScreenPosition( const PickEvent& pe,           // the pick_event
							const SPAposition*   origin,       // Plane origin or NULL
							const SPAvector*     pnl_normal,   // Plane normal or NULL
							SPAposition& pt                    // point in model space coordinates
							);

// Pick a single ENTITY.  A top level ENTITY will be picked
outcome PickEntity(
    const PickEvent&  pe,          // a pick event
    entity_filter*     filter,      // a pick filter or NULL
	int                depth,       // pick depth
	ENTITY_LIST const* pick_from,   // list of ENTIITYs to pick from or
                                    // null for all ENTITY's in display list
    ENTITY*&           ent          // the entity picked
    );

SPAposition PickClosestPointOnEntity(
	pick_ray const&    pray,
	entity_filter*     filter,
    ENTITY_LIST const* pick_from
	);

pick_ray
GetPickRay(
	PickEvent const&,
	logical at_eye = FALSE
	);

EDGE* PickEdge(
    const PickEvent&  pe,
	entity_filter*,
	ENTITY_LIST const* = 0);

void PickEdges(
    const pick_ray &,
	ENTITY_LIST const& ,
	entity_filter*,
	ENTITY_LIST&);

outcome PickFaces(
    const pick_ray&    pray,        // a pick_ray to do the pick
    entity_filter*     filter,      // a pick filter or NULL
    ENTITY_LIST const* body_list,   // A list of BODIES to search
    ENTITY_LIST&       face_list    // A list of FACES hit by the ray
                                    // sorted by the order that they are hit.
    );

outcome PickAsmEntities(
    const pick_ray&    pray,       
    entity_filter*     filter,      // a pick filter or NULL
	int                /*depth*/,       // pick depth
	asm_model_list const& pick_from,
                                    // null for all ENTITY's in display list
	component_entity_handle_list& ent_list
    );

outcome PickAsmFaces(
    const pick_ray&    pray,       
    entity_filter*     filter,     
	asm_model_list const& pick_from,
	component_entity_handle_list& face_list
    );

outcome PickAsmEdges(
    const pick_ray&    pray,       
    entity_filter*     filter,     
	asm_model_list const& pick_from,
	component_entity_handle_list& edge_list
    );

outcome PickAsmVertices(
    const pick_ray&    pray,       
    entity_filter*     filter,     
	asm_model_list const& pick_from,
	component_entity_handle_list& vertex_list
    );

VERTEX* PickVertex(
    const pick_ray &,
	entity_filter*, 
	ENTITY_LIST const*);

void PickInRegion(
	const PickEvent&,
	const PickEvent&,
	entity_filter*,
	ENTITY_LIST&);

void SetPickAperture(int dx,int dy);
void GetPickAperture(int &dx,int &dy);

SPAposition IntersectRayWithPlane(
    const pick_ray& pe,
    const SPAposition* origin,
    const SPAvector* pln_normal
    );

PickEvent
GetPickEvent();

outcome PickEntityFromRay(
    pick_ray &pray,      // the pick ray
	ENTITY_LIST const& pick_from,    // pick_from (not used yet)
    entity_filter *filter, // optional entity filter
	ENTITY_LIST& entity_list // returned entities
    );


int PickChain(
	EDGE const*, 
	EDGE const*, 
	ENTITY_LIST const*,
	ENTITY_LIST&);

#endif // GENERIC_PICKING_HXX
