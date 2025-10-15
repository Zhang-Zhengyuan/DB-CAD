/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header file defining the vertex blend attribute.
#if !defined( VBLEND_ATTRIB )
#define VBLEND_ATTRIB
class ENTITY;
class VERTEX;
class segend;
class surface;
class vblend_geom;
class vsegend;
#include "acis.hxx"
#include "dcl_blnd.h"
#include "bl_att.hxx"
#include "at_sys.hxx"
#include "vbl_enum.hxx"
/**
 * @file vbl_att.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup BLNDAPI
 *
 * @{
 */

/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE( ATTRIB_VBLEND, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif


/**
 * @nodoc
 */
extern DECL_BLND int ATTRIB_VBLEND_TYPE;
#define ATTRIB_VBLEND_LEVEL (ATTRIB_BLEND_LEVEL + 1)

/**
 * Defines the vertex blend attribute.
 */
class DECL_BLND ATTRIB_VBLEND: public ATTRIB_BLEND {
private:

double			bulge_data;	// only used for spline blend surfs
	bl_continuity	continuity_data;

	// Record special properties of blends on vertices (see vbl_enum.hxx).

	bl_v_property bl_v_prop_data;

	// Record method if any to be used for auto setback computation.

	bl_v_auto_property bl_v_auto_data;

	// The list of supporting vertex blend entities.

	support_entity *_support;

	// Set all the supports of this blend

	logical
	set_supports(
				 int n,			// size of array
				 ENTITY *ents[]	// pointers to entities
				 );

public:

	// Two flags for the new vtx blnd code
/**
 * Flag indicating whether the vertex blend has been processed.
 */
	logical vtx_processed ;
/**
 * Flag indicating whether to use new vertex blend processing or old.
 */
	logical	vtx_old_way;

/**
 * Flag indicating whether we are doing concurrent blending or not.
 */
	logical concurrent_blending;

    // Simple read function for the data members.
/**
 * Returns the bulge factor.
 */
	double bulge() const { return bulge_data; }
/**
 * Returns properties of blends on vertices.
 */
	bl_v_property bl_v_prop() const { return bl_v_prop_data; }
/**
 * Returns the property, if any, to be used for autosetback computation.
 * <br><br>
 * <b>Role:</b> This signifies whether setbacks at a vertex blend are to be calculated automatically
 * or not (a choice of different properties could be offered in the future). Valid values are defined
 * in the enumeration <tt>bl_v_auto_property</tt>.
 */
	bl_v_auto_property bl_v_auto() const { return bl_v_auto_data; }

	// (continuity read by virtual function below)
/**
 * Returns TRUE if the given blend is an <tt>bi_blend</tt>, otherwise, it returns FALSE.
 * <br><br>
 * <b>Role:</b> A <tt>bi_blend</tt> is a blend that is sometimes attached to smooth vertices during blending
 * to indicate that they also need blending.
 */
	logical is_bi_blend() const { return bl_v_prop_data==bl_v_bi_blend; }
/**
 * Returns TRUE if the given entity is a <tt>cuspate</tt>, otherwise, it returns FALSE.
 */
	logical is_cuspate() const { return bl_v_prop_data==bl_v_cusp; }

	// This is the official function to discover what entities the
	// spring curves of a blend face run along, i.e. its "supports".
	// For a vertex blend we add all the faces incident at the
	// vertex and return how many there were. For bi-blends we currently
	// add nothing and return 0 as this functionality is not currently
	// supported for them.
/**
 * @nodoc
 */
	virtual int supports( ENTITY **& entity) const;

	// Member setting functions. These ensure that the attribute
	// is safely backed up before making the change.
/**
 * Sets the bulge.
 * <br><br>
 * @param set_bulge
 * bulge.
 */
	void set_bulge( double set_bulge);
/**
 * Marks the blend as having at least the given continuity at its boundaries.
 * <br><br>
 * @param continuity
 * continuity.
 */
	void set_continuity( bl_continuity  continuity);
/**
 * Sets the <tt>bi_blend</tt> property value.
 * <br><br>
 * @param blnd
 * bi_blend property.
 */
	void set_bi_blend( bl_v_property blnd = bl_v_bi_blend );
/**
 * Sets special properties of blends on vertices.
 * <br><br>
 * @param blnd
 * vertex blend property.
 */
	void set_bl_v_prop( bl_v_property blnd );
/**
 * Sets the property to be used for autosetback computation.
 * <br><br>
 * <b>Role:</b> This signifies whether setbacks at a vertex blend are to be calculated automatically
 * or not (a choice of different properties could be offered in the future). Valid values are defined
 * in the enumeration <tt>bl_v_auto_property</tt>.
 * <br><br>
 * @param auto_blnd
 * autosetback property.
 */
	void set_bl_v_auto( bl_v_auto_property auto_blnd);

	// Default for bulge factor (see also ccode/cc_blend.cxx).
#define BULGE_DEFAULT 1
/**
 * C++ initialize constructor requests memory for this object and populates it with the data supplied as arguments.
 * <br><br>
 * <b>Role:</b> Applications should call this constructor only with the overloaded <tt>new</tt> operator
 * inherited from the <tt>ENTITY</tt> class (for example, x=<tt>new ATTRIB_VBLEND(...)</tt>), because this reserves
 * the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param v_owner
 * vertex owner.
 * @param bulge1
 * bulge.
 * @param continuity1
 * continuity.
 * @param v_prop
 * vertex blend property.
 * @param auto_prop
 * vertex auto property.
 */
	ATTRIB_VBLEND(
				VERTEX * v_owner= NULL,			// owner
				double bulge1 = BULGE_DEFAULT,
				bl_continuity continuity1 = slope_continuous,
				bl_v_property  v_prop= bl_v_unset,
				bl_v_auto_property auto_prop = bl_v_auto_unset
			);

	// Make a copy of a blend (setting implicit blend data only and
	// without attaching the copy to an entity).
/**
 * C++ constructor, creating an <tt>ATTRIB_VBLEND</tt> by copying one.
 * <br><br>
 * <b>Role:</b> C++ copy constructor requests memory for this object and populates it with the data
 * from the object supplied as an argument. Applications should call this constructor only with the
 * overloaded <tt>new</tt> operator inherited from the <tt>ENTITY</tt> class (for example, x=<tt>new ATTRIB_VBLEND(...)</tt>),
 * because this reserves the memory on the heap, a requirement to support roll back and history management.
 * <br><br>
 * @param blnd
 * existing blend.
 */
	ATTRIB_VBLEND( ATTRIB_VBLEND const *blnd);
/**
 * @nodoc
 */
	virtual ATTRIB_BLEND* copy() const;

	// Functions called to aid attribute migration during modelling
	// operations.
/**
 * @nodoc
 */
	virtual void split_owner( ENTITY * entity);
									// the owner of this attribute is
									// about to be split in two - the
									// argument is the new piece.
/**
 * @nodoc
 */
	virtual void merge_owner( ENTITY * entity, logical del_owner );
									// the owner of this attribute is
									// about to be merged with the
									// given entity. The logical
									// argument is TRUE if the owner is
									// to be deleted in the merge.

	// Test two blends for equality.
/**
 * @nodoc
 */
	virtual logical operator==( ATTRIB_BLEND const& blnd ) const;

	// Return a measure of continuity at blend boundary.
/**
 * @nodoc
 */
	virtual bl_continuity continuity( int sup_index ) const;

	// Find details of blend geometry.

/**
 * Finds the details of the blend geometry.
 * <br><br>
 * @param blnd_geo
 * blend geometry.
 * @param coedge1
 * rb_vbl_spr_coedge.
 * @param coedge2
 * rb_vbl_sh_coedge.
 */
	vblend_geom *find_vblend_geometry(
		vblend_geom *blnd_geo,
		COEDGE *coedge1 = NULL,	// rb_vbl_spr_coedge
		COEDGE * coedge2= NULL,		// rb_vbl_sh_coedge
        ENTITY_LIST const &ents
          = * ( ENTITY_LIST * ) NULL_REF
	  );

	// Find details of blend boundary and return it as a chain of
	// vsegends.

/**
 * Finds the details of the blend boundary and returns it as a chain of <tt>vsegends</tt>.
 * <br><br>
 * <b>Role:</b> A <tt>vsegend</tt> holds information about the vertex blend sheet segment/boundaries.
 * <br><br>
 * @param blnd_geo
 * blend geometry.
 */
	vblend_geom *find_vblend_curves(vblend_geom *blnd_geo);

	// Given blend boundary details in a vsegend chain, find the
	// blend surface and return it and pcurves if any in a vblend_geom.

/**
 * Finds the blend surface and returns it.
 * <br><br>
 * <b>Role:</b> Returns blend surface and pcurves if there are any in a <tt>vblend_geom</tt>,
 * given the blend boundary details in a <tt>vsegend</tt> chain. A <tt>vsegend</tt> holds information
 * about the vertex blend sheet segment/boundaries.
 * <br><br>
 * @param blnd_geo
 * blend geometry.
 * @param coedge1
 * rb_vbl_spr_coedge.
 * @param coedge2
 * rb_vbl_sh_coedge.
 */
	vblend_geom *find_vblend_surface(
        vblend_geom *blnd_geo,
        COEDGE * coedge1 = NULL,	// rb_vbl_spr_coedge
        COEDGE * coedge2 = NULL,	// rb_vbl_sh_coedge
        ENTITY_LIST const &ents
           = * ( ENTITY_LIST * ) NULL_REF
        );

	// This is the entry point to the blend stage 1 processing that
	// makes a set of sheet faces for the blend attribute. For vertex
	// blends this method calls the existing function
	// make_sheet_faces_for_vertex.
	//
	// Its return values are:
	//   1: ok.
	//   0: the blend didn't work, but it might later, e.g., after
	//      surrounding geometry / topology is created.
	//  -1: this entire blend is never going to work.
/**
 * @nodoc
 */
	virtual int make_sheet_faces( blend1_data & arr_blnd, BODY *sheet_body );

/**
 * @nodoc
 */
	virtual blend_debug_info* enquire(FILE*fp);

/**
 * @nodoc
 */
	support_entity *
	get_support_entity( ENTITY *ent );

	/**
	 * @nodoc
	 */
	ATTRIB_FUNCTIONS( ATTRIB_VBLEND, BLND )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

};

// Very handy little function to find the (first) ATTRIB_VBLEND on any
// given entity.

/**
 * @nodoc
 */
DECL_BLND ATTRIB_VBLEND *find_vblend_attrib( ENTITY *owner );


// Set a blend attribute on a vertex whose edges have already been
// given blend attributes and apply the given setback (>= 0) to each
// of its blended edges.
// If the given setback is negative, leave existing setbacks on the
// edges unchanged.

/**
 * @nodoc
 */
DECL_BLND void set_vblend(
	VERTEX *,
	double = BULGE_DEFAULT,	// optional bulge factor b (0 <= b <= 2)
	double = -1.0,			// optional setback
	bl_continuity = slope_continuous,	// optional continuity
	bl_v_property = bl_v_unset
);

// Set a blend attribute on a vertex and compute and apply a setback
// (>= 0) to each (blended) edge of the vertex.  If an edge has a
// blend of zero size, leave its setback unchanged.  Setbacks are
// found by assuming the edges are straight and faces are planar at
// the vertex, and that circular spring curves of radius equal to the
// average edge blend radius at the vertex, are desired.

/**
 * @nodoc
 */
DECL_BLND void set_vblend_auto(
	VERTEX *,
	double = BULGE_DEFAULT,	// optional bulge factor b (0 <= b <= 2)
	bl_continuity = slope_continuous,	// optional continuity
	bl_v_property = bl_v_unset,
	bl_v_auto_property = bl_v_auto_unset
);

// As the above, but with the crucial difference that any vertex blends
// marked with this routine will use rolling ball vertex blends whenever
// possible (overriding any automatically computed setbacks). If a
// rolling ball vertex blend is not possible, will do the vertex blend
// with autosetbacks.

/**
 * @nodoc
 */
DECL_BLND void set_vblend_autoblend(
	VERTEX *,
	double = BULGE_DEFAULT,	// optional bulge factor b (0 <= b <= 2)
	bl_continuity = slope_continuous,	// optional continuity
	bl_v_property = bl_v_unset
);

#undef BULGE_DEFAULT


// Make spring curves to complete a vertex blend boundary.  Updates the
// chain of vsegends in vblend_geom.


/**
 * @nodoc
 */
DECL_BLND vblend_geom *make_vbl_curves
(
	ATTRIB_VBLEND *,
	vblend_geom *
);

// Make the surface for a blended vertex - a sphere, torus, rolling-ball
// spline or a free-form surface, and leave it in the vblend_geom supplied.
// The two coedges refer to the blend body and sheet faces on which a
// rolling-ball vertex blend surface will be made.


/**
 * @nodoc
 */
DECL_BLND vblend_geom *make_vbl_surface
(
	ATTRIB_VBLEND *,
	vblend_geom *,		// chain of vsegends giving boundary
	COEDGE * = NULL,	// coedge of blend body face on which spring curve lies
	COEDGE * = NULL,    // coedge of adjacent sheet face
    ENTITY_LIST const &ents =
        * ( ENTITY_LIST * ) NULL_REF
);

/** @} */
#endif
