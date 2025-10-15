/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef __immediate_draw_hxx__
#define __immediate_draw_hxx__

#include "dcl_kern.h"
#include "logical.h"
#include "position.hxx"

class curve;
class CURVE;
class EDGE;
class SPAparameter;
class SPAtransf;

class IACISImmediateDraw: public ACIS_OBJECT
{
public:

	virtual void get_current_rgb(float &r,float &g,float &b) = 0;
	virtual void set_current_rgb(float r,float g,float b) = 0;

	virtual void draw_polyline_3d(
        int npts,               // Number of points
        const float pts[],      // 3 values (X, Y, Z) for each point
        logical fill = FALSE    // Fill polygon?
        ) = 0;
    virtual void draw_point_3d(
        const SPAposition&pos     // Coordinate at which to display point
        ) = 0;
    virtual void draw_text_3d(
        const SPAposition&pos,    // Position for start of text
        const char* text        // Text to be displayed
        ) = 0;
    virtual void draw_curve(
		curve *this_curve,
		SPAparameter start_param,
		SPAparameter end_param,
		const SPAtransf* sketch_transform
		)=0;
    virtual void draw_CURVE(
		CURVE *this_curve,
		SPAparameter start_param,
		SPAparameter end_param,
		const SPAtransf* sketch_transform
		)=0;
	virtual void draw_EDGE(EDGE *)=0;
};

DECL_KERN void set_IACISImmediateDraw(IACISImmediateDraw *draw);
DECL_KERN IACISImmediateDraw *get_IACISImmediateDraw();

DECL_KERN void imm_draw_polyline_3d(
        int npts,               // Number of points
        const float pts[],      // 3 values (X, Y, Z) for each point
        logical fill = FALSE    // Fill polygon?
        );

DECL_KERN void imm_draw_point_3d(
        const SPAposition&pos     // Coordinate at which to display point
        );

DECL_KERN void imm_draw_text_3d(
        const SPAposition&pos,    // Position for start of text
        const char* text        // Text to be displayed
        );

DECL_KERN void imm_get_current_rgb(float &r,float &g,float &b);
DECL_KERN void imm_set_current_rgb(float r,float g,float b);

DECL_KERN void imm_draw_curve(
	curve *this_curve,
	const SPAparameter& start_param,
	const SPAparameter& end_param,
	const SPAtransf* sketch_transform
	);
DECL_KERN void imm_draw_CURVE(
	CURVE *this_curve,
	const SPAparameter& start_param,
	const SPAparameter& end_param,
	const SPAtransf* sketch_transform
	);

DECL_KERN void imm_draw_EDGE(EDGE *this_EDGE);

#endif //__immediate_draw_hxx__
