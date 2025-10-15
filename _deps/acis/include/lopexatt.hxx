/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header file defining SURFACE attributes for local ops.
/*******************************************************************/

#if !defined( LOP_EXT_ATTRIB )
#define LOP_EXT_ATTRIB

#include "acis.hxx"
#include "dcl_lop.h"
#include "tophdr.hxx"
#include "coedge.hxx"
#include "edge.hxx"
#include "param.hxx"
#include "attrib.hxx"
#include "at_sys.hxx"
#include "surface.hxx"
#include "lists.hxx"
#include "face.hxx"
#include "box.hxx"

class TWEAK;

#if defined D3_STANDALONE || defined D3_DEBUG
class   D3_ostream;
class   D3_istream;
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_LOP_SURFACE_EXT, LOP )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_LOP_CURVE_EXT, LOP )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

// The ATTRIB_LOP_SURFACE_EXT class is declared here.

// The base class contains ....

extern DECL_LOP int ATTRIB_LOP_SURFACE_EXT_TYPE;
#define ATTRIB_LOP_SURFACE_EXT_LEVEL (ATTRIB_SYS_LEVEL + 1)

class DECL_LOP ATTRIB_LOP_SURFACE_EXT: public ATTRIB_SYS {
private:
    // data
	TWEAK   *_tweak;         // tweak
	SPAbox  	_orig_mbox;      // original model space SPAbox
	SPAbox 	_mbox;           // current model space SPAbox
	SPApar_box _orig_pbox;      // original SPAparameter SPAbox
	SPApar_box _pbox;           // current SPAparameter SPAbox
	ENTITY_LIST _face_list;  // list of faces using surface
	SURFACE *_sf;            // surface ( may not be owner if copied )

protected:

	// Limit the u extension request for certain cases when
	// extending variable radius blend surfaces. 
	void limit_extension_var_rad(void);

public:
	// Constructors
	ATTRIB_LOP_SURFACE_EXT();

	ATTRIB_LOP_SURFACE_EXT(
			ENTITY        	*own,			 // owner
			TWEAK           *twk, 
			logical       	&status	        // valid construction occurred
			);

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_LOP_SURFACE_EXT, LOP )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

    virtual logical pattern_compatible() const;
                                    // returns TRUE 
	// data
	TWEAK *tweak() const { return _tweak; }
	SURFACE *surf() const { return _sf; }
	logical set_surf( SURFACE *sf ) 
	{
		// let go of old surface
		if ( surf() ) surf()->remove();
		_sf = sf; 
		// set stake in new
		surf()->add();
		return TRUE; 
	}
	SPAbox const &orig_mbox() const { return _orig_mbox; }
	SPAbox &mbox() { return _mbox; }
	SPApar_box const &orig_pbox() const { return _orig_pbox; }
	SPApar_box &pbox() { return _pbox; }

	void face_add( FACE *fa ) { _face_list.add( fa ); }
	int face_count() const { return _face_list.count(); }
	FACE *face( int n ) const { return ( FACE * ) _face_list[ n ]; }

	// to estimate local surface extension
	logical estimate( const COEDGE *coed );

	// to extend surface
	logical extend();

	// to trim surface
	logical trim() const;

	// STI rr (02/26/2001) begin: Make this attribute not 
	// savable and remove sys_error from the macros.
	logical savable() const { return FALSE; }
	// STI rr end.

#if defined D3_STANDALONE || defined D3_DEBUG

    void        print( D3_ostream& os, int level )      const;

#endif

};


#if defined D3_STANDALONE || defined D3_DEBUG

extern DECL_LOP D3_ostream& operator<<( D3_ostream& os, const ATTRIB_LOP_SURFACE_EXT& );

#endif

// The ATTRIB_LOP_CURVE_EXT class is declared here.

// The base class contains ....

extern DECL_LOP int ATTRIB_LOP_CURVE_EXT_TYPE;
#define ATTRIB_LOP_CURVE_EXT_LEVEL (ATTRIB_SYS_LEVEL + 1)

class DECL_LOP ATTRIB_LOP_CURVE_EXT: public ATTRIB_SYS {
private:
    // data
	TWEAK   *_tweak;          // tweak
	SPAbox  	 _orig_mbox;      // original model space SPAbox
	SPAbox 	 _mbox;           // current model space SPAbox
	SPAinterval _orig_tbox;      // original param range
	SPAinterval _tbox;           // current required param range
	ENTITY_LIST _edge_list;   // list of edges using surface

protected:

public:
	// Constructors
	ATTRIB_LOP_CURVE_EXT();

	ATTRIB_LOP_CURVE_EXT(
			ENTITY        	*own,			 // owner
			TWEAK           *twk,            // tweak
			logical       	&status	         // valid construction occurred
			);

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_LOP_CURVE_EXT, LOP )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

    virtual logical pattern_compatible() const;
                                    // returns TRUE 
	// data
	TWEAK *tweak() const { return _tweak; }
	CURVE *curv() const { return ( CURVE * ) owner(); }
	SPAbox const &orig_mbox() const { return _orig_mbox; }
	SPAbox &mbox() { return _mbox; }
	SPAinterval const &orig_tbox() const { return _orig_tbox; }
	SPAinterval &tbox() { return _tbox; }

	void edge_add( EDGE *ed ) { _edge_list.add( ed ); }
	int edge_count() const { return _edge_list.count(); }
	EDGE *edge( int n ) const { return ( EDGE * ) _edge_list[ n ]; }

	// to estimate local curve extension
	logical estimate( const COEDGE *coed );

	// to extend curve
	logical extend();

	// to trim curve
	logical trim() const;
	
	// STI rr (02/26/2001) begin: Make this attribute not 
	// savable and remove sys_error from the macros.
	logical savable() const { return FALSE; }
	// STI rr end.

#if defined D3_STANDALONE || defined D3_DEBUG

    void        print( D3_ostream& os, int level )      const;

#endif

};


#if defined D3_STANDALONE || defined D3_DEBUG

extern DECL_LOP D3_ostream& operator<<( D3_ostream& os, const ATTRIB_LOP_CURVE_EXT& );

#endif

DECL_LOP SPApar_box face_param_range( FACE *face, TWEAK *tw );

#endif



