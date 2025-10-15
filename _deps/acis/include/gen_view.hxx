// $Id: gen_view.hxx,v 1.8 2002/08/09 17:31:46 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//-----------------------------------------------------------------------------
// An abstract view class for general use within Scheme extensions.  Each 
// renderer should derive a concrete class to implement these methods
//-----------------------------------------------------------------------------
#ifndef GENERIC_VIEW_HXX
#define GENERIC_VIEW_HXX

#include "base.hxx"
#include "transf.hxx"
#include "draw_dt.hxx"
#include "wndo_dt.hxx"
#include "rgbcolor.hxx"
#include "curdef.hxx"
#include "curve.hxx"
#include "gen_render.hxx"
#include "timer.hxx"
#include "rndr_env.hxx"

class SPAposition;
class SPAunit_vector;
class SPAvector;
class RenderingObject;

class ENTITY;

class SchemeView : public view_base
{
public:
	virtual ~SchemeView() {}

	virtual long AddRef() = 0;
	virtual long Release() = 0;

	// Create a RenderingObject matched to this derived view type and add it.
	virtual RenderingObject* CreateRenderingObject() = 0;
	virtual RenderingObject* CreateRenderingObject(RenderingObject*) = 0;
	virtual void AddRenderingObject(RenderingObject* o) = 0;
	virtual void RemoveRenderingObject(RenderingObject* o) = 0;
	virtual void GetRenderingObjectList( VOID_LIST & list ) = 0;

	virtual SPAposition get_eye() = 0;
	virtual void set_eye(SPAposition const&) = 0;

	virtual SPAposition get_target() = 0;
	virtual void set_target(SPAposition const&) = 0;

	virtual SPAunit_vector get_up() = 0;
	virtual void set_up(SPAvector const&) = 0;

	virtual SPAunit_vector get_right() = 0;
	virtual SPAunit_vector get_out() = 0;

	virtual logical get_perspective() = 0;
	virtual void set_perspective(logical ) = 0;

	virtual double get_width() = 0;
	virtual double get_height() = 0;
	virtual void   set_size(double w, double h) = 0;

	virtual double get_hither() = 0;
	virtual void set_hither(double hither) = 0;
	virtual double get_yon() = 0;
	virtual void set_yon(double yon) = 0;

	virtual rgb_color get_foreground_rgb() = 0;
	virtual void set_foreground_rgb(const rgb_color& foreground) = 0;
	virtual rgb_color get_background_rgb() = 0;
	virtual void set_background_rgb(const rgb_color& background) = 0;

	virtual void set_line_style( line_style style ) = 0;
	virtual line_style get_line_style() = 0;
	virtual void set_point_style( point_style style ) = 0;
	virtual point_style get_point_style() = 0;
	virtual void set_point_size( int size ) = 0;
	virtual int get_point_size() = 0;
	virtual void set_line_width( int width ) = 0;
	virtual int get_line_width() = 0;
    virtual logical set_text_font(const char* font_name) = 0;
	virtual void set_text_size(int size) = 0;
	virtual void set_drawing_mode( drawing_mode mode ) = 0;
	virtual void set_rubberband_mode( logical mode ) = 0;

    // Set the size of the viewport (default is the size of the display window)
    virtual void set_viewport(
        int x,          // Position of the viewport in the display window
        int y,
        int width,      // Size of the viewport (in pixels)
        int height) = 0;

    virtual void get_viewport(
        int &x,         // Position of the viewport in the display window
        int &y,
        int &width,     // Size of the viewport (in pixels)
        int &height ) = 0;

	virtual void recompute_viewport() = 0;

	virtual void * get_window_handle() = 0;

	virtual logical drawable_to_world( int x, int y, SPAposition &pt ) = 0;
    virtual logical world_to_drawable( const SPAposition &pt, float &x, float &y ) = 0;

	virtual int draw_polyline_3d(
        int npts,               // Number of points
        const float pts[],      // 3 values (X, Y, Z) for each point
        logical fill = FALSE    // Fill polygon?
        ) = 0;
	virtual int draw_polyline_3d(
        int npts,               // Number of points
        const SPAposition pts[],
        logical fill = FALSE    // Fill polygon?
        ) = 0;
    virtual int draw_point_3d(
        const SPAposition &pos     // Coordinate at which to display point
        ) = 0;
    virtual int draw_text_3d(
        const SPAposition &pos,    // Position for start of text
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

	virtual void make_current() = 0;
	virtual void clear() = 0;
	virtual void flush( int flags = 0 ) = 0;
	virtual void destroy() = 0;
	virtual void refresh(logical update_sils=FALSE) = 0;
	virtual void refresh_immediate() = 0;
	virtual logical is_valid() = 0;

	virtual void update_extrema() = 0;

	virtual void name_window(const char* n) = 0;

	virtual unsigned char * get_image() = 0;
	virtual logical is_shaded() = 0;

	virtual logical get_DisplayFacets() = 0;
	virtual void set_DisplayFacets(logical l) = 0;

	virtual logical get_DisplayParamLines() = 0;
	virtual void set_DisplayParamLines(logical l) = 0;

	virtual logical get_DisplaySurfacePolys() = 0;
	virtual void set_DisplaySurfacePolys(logical l) = 0;

	virtual logical get_DisplayFaceNormals() = 0;
	virtual void set_DisplayFaceNormals(logical l) = 0;

	virtual logical get_DisplayFacetVertexNormals() = 0;
	virtual void set_DisplayFacetVertexNormals(logical l) = 0;

	virtual void rotate(
		SPAunit_vector axis,   // Axis direction in model space
		SPAposition pt,        // Axis SPAposition in model space
		double angle        // Rotation angle in radians
		)
	{	// get the view parameters
		SPAposition target = get_target();
		SPAposition eye = get_eye();
		SPAunit_vector up = get_up();
		// build the SPAtransf
		// first translate the rotation origin to (0,0,0)
		SPAposition origin( 0, 0, 0 );
		SPAvector delta = origin - pt;
		SPAtransf tform = translate_transf( delta );
		// now apply the rotation
		tform *= rotate_transf( angle, axis );
		// finally, move it back to where it started
		tform *= translate_transf( ( -delta ) );
		// apply the transform
		target *= tform;
		eye *= tform;
		up *= tform;
		// now update the view parameters
		set_target( target );
		set_eye( eye );
		set_up( up );
	}
	
	virtual double get_time_elapsed() { return get_cpu_seconds();  }

	virtual const char *	get_engine_full_name() = 0;
	virtual const char *	get_engine_abbrev() = 0;
	virtual int				get_engine_version_major() = 0;
	virtual int				get_engine_version_minor() = 0;
	virtual int				get_engine_version_point() = 0;	
};

// A class to be used by those who don't want to explicitly code Release()
class SchemeViewPtr: public ACIS_OBJECT
{
	SchemeView* m_pView;
public:
	SchemeViewPtr(SchemeView* v = NULL) : m_pView(v) {if(m_pView != (SchemeView*)NULL_REF) m_pView->AddRef();}
	~SchemeViewPtr() { if(m_pView != (SchemeView*)NULL_REF) m_pView->Release(); }
	SchemeView* operator->() { return m_pView; }
	operator SchemeView*() { return m_pView; }
	SchemeView* operator=(SchemeView* v) {
		if(m_pView != v) {
			if(m_pView != (SchemeView*)NULL_REF) m_pView->Release();
			m_pView = v;
			//if(v) v->AddRef();
			if (m_pView != (SchemeView*)NULL_REF)
				m_pView->AddRef();
		}
		return m_pView;
	}
};


SchemeView* GetActiveView();
void SetActiveView(SchemeView* v);
SchemeView* FindView(window_handle w);

SchemeView* GetPrinterView(SchemeView* orgview);
SchemeView* GetMetafileView(SchemeView* orgview, const char* name, int type, int placeable);
SchemeView* GetPostscriptView(SchemeView* orgview, const char* name, logical color, double width, double height);
SchemeView* GetInterleafView(SchemeView* orgview, const char* name, logical color, double width, double height);
SchemeView* GetPNMView(SchemeView* orgview, const char* name, logical color, logical binary);
SchemeView* GetTargaView(SchemeView* orgview, const char* name, logical color, logical compress);

SPAtransf ViewToModelTransf(SchemeView* view );

#endif // GENERIC_VIEW_HXX
