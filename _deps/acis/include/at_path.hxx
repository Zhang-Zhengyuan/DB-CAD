/* ORIGINAL: acis2.1/sg_husk/sweep/at_path.hxx */
// $Id: at_path.hxx,v 1.7 2002/08/09 17:28:45 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined ( ATTRIB_STI_NOR_ATTR_CLASS )
#define ATTRIB_STI_NOR_ATTR_CLASS


#include "dcl_swp.h"
#include "logical.h"
#include "entity.hxx"
#include "at_sg.hxx"

#include "bs3curve.hxx"

//STI HDR_INDEP: nvp
#include "unitvec.hxx"
#include "interval.hxx"
class curve;

class SPAunit_vector;
class surface;
class COEDGE;
class VERTEX;
class law;
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_STI_NOR_ATTR, SWP )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_STI_PSPLIT_ATTR, SWP )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_STI_SECT_ATTR, SWP )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif



extern DECL_SWP int ATTRIB_STI_NOR_ATTR_TYPE;

#define ATTRIB_STI_NOR_ATTR_LEVEL ( ATTRIB_SG_LEVEL + 1 )

class DECL_SWP ATTRIB_STI_NOR_ATTR : public ATTRIB_SG
{

protected:

	SPAunit_vector path_normal;		// Path normal
	SPAunit_vector prof_normal;		// Profile normal
	SPAunit_vector mitre_normal;		// mitre plane normal

	//bs3_curve	rail_spline;		// rail bspline
	bs3_curve	path_spline;		// path bspline
	surface 	*lateral_sur;		// lateral surface

	int			path_t;				// path_type 
	double		tol;				// path curve fit tolerance

	curve		*ext_curve;
	SPAinterval	ext_range;
public:

	ATTRIB_STI_NOR_ATTR( 
		    ENTITY* = NULL,
			const SPAunit_vector & = *(const class SPAunit_vector *)NULL_REF,	// path nor
			const SPAunit_vector & = *(const class SPAunit_vector *)NULL_REF,	// prof nor
			const SPAunit_vector & = *(const class SPAunit_vector *)NULL_REF,	// mitre nor
			bs3_curve = NULL,								    // path bs
			surface * = (surface *)NULL,					    // lateral sur
			int = 1, 										    // path type
			double = 0.0									    // path tolerance
	);

    virtual logical pattern_compatible() const;
                                    // returns TRUE 
	// Query functions

	// Return various normals
	SPAunit_vector path_nor() { return path_normal; }
	SPAunit_vector prof_nor() { return prof_normal; }
	SPAunit_vector mitre_nor() { return mitre_normal; }

	// Return type type( -1 : linear, 1: planar , 0 : non-planar )
	int path_type() { return path_t; }
	double path_tol() { return tol; }

	// Return the data, used
	bs3_curve path_bs() { return path_spline; }
	//bs3_curve rail_bs() { return rail_spline; }
	surface *lat_surf() { return lateral_sur; }
	curve *extension() { return ext_curve; }
	SPAinterval &extension_range() { return ext_range; }


	// Set functions
	void set_path( bs3_curve in_cur ) { path_spline = in_cur; }
	// void set_rail( bs3_curve in_cur ) { rail_spline = in_cur; }
	void set_surf( surface *in_sur ) { lateral_sur = in_sur; }
	void set_ext_curve( curve *cur ) { ext_curve = cur; }
	void set_ext_range( SPAinterval &range ) { ext_range = range; }

	// Functions called to aid attribute migration during 
	// modelling operations.
	// We do not need any of these, as this is a temporary attribute 
	// used for passing data between the various  functions in sweep.
	// The default implemetation is ok!

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_STI_NOR_ATTR, SWP )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

extern DECL_SWP int ATTRIB_STI_PSPLIT_ATTR_TYPE;

#define ATTRIB_STI_PSPLIT_ATTR_LEVEL ( ATTRIB_SG_LEVEL + 1 )

class DECL_SWP ATTRIB_STI_PSPLIT_ATTR : public ATTRIB_SG
{
	int			no_pts;
	double		*params;

	public :

		ATTRIB_STI_PSPLIT_ATTR(
					ENTITY*	= NULL );

    virtual logical pattern_compatible() const;
                                    // returns TRUE 
		inline void set_no_pts( int no ) { no_pts = no; }
		inline void set_params( double *inp ) { params = inp; }
		inline int no_of_pts() { return no_pts; }
		inline double *parameters() { return params; }

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_STI_PSPLIT_ATTR, SWP )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

extern DECL_SWP int ATTRIB_STI_SECT_ATTR_TYPE;

#define ATTRIB_STI_SECT_ATTR_LEVEL ( ATTRIB_SG_LEVEL + 1 )

class DECL_SWP ATTRIB_STI_SECT_ATTR : public ATTRIB_SG
{
	curve	*section;
	public :

		ATTRIB_STI_SECT_ATTR(
					ENTITY*	= NULL,
					curve * = NULL );
		
    virtual logical pattern_compatible() const;
                                    // returns TRUE 
		curve *section_curve() { return section; }

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_STI_SECT_ATTR, SWP )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

#endif
