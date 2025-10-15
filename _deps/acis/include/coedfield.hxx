// $Id: coedfield.hxx,v 1.6 2002/08/09 17:19:06 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
//
// This file processes the boundary condition of a coedge and generates a law to describe
// the cross boundary SPAvector field.
//
#if !defined( COEDFIELD_HXX )
#define COEDFIELD_HXX

#include "acis.hxx"
#include "dcl_intr.h"
#include "logical.h"

#include "vector.hxx"
#include "position.hxx"
#include "interval.hxx"
#include "unitvec.hxx"

/**
* @file coedfield.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

class COEDGE;
class FACE;
class law;
class PositionVector;
class SPAtransf;
class WIRE;
class EDGE;
// ywoo 08Jan01: added the class.
class ENTITY_LIST;
// ywoo: end

/** \addtogroup INTRAPIBOUNDARYFIELD
 *  \brief Declared at <coedfield.hxx>, SPAintr
 *  @{
 */

/**
 * Specifies the type of vector field built as a boundary field.
 * @param EMPTY_FIELD
 * no base face.
 * @param UNIFORM_VEC_FIELD
 * defined by a central vector.
 * @param FACE_NORMAL_FIELD
 * vector field generated from the face normal vector.
 * @param FACE_TANGENT_FIELD
 * vector field generated from the face normal vector.
 * @param FACE_SIDE_FIELD
 * vector field generated from the side face cross boundary vector.
 * @param DRAFT_NORMAL_FIELD
 * vector field generated from an angle to a base vector(field).
 * @param DRAFT_SIDE_FIELD
 * vector field generated from an angle to side faces of a base face.
 **/
enum fieldtype {
	EMPTY_FIELD,		// no base face
	UNIFORM_VEC_FIELD,	// defined by a central SPAvector
	FACE_NORMAL_FIELD,	// SPAvector field generated from the face normal SPAvector
	FACE_TANGENT_FIELD,	// SPAvector field generated from the face normal SPAvector
	FACE_SIDE_FIELD,	// SPAvector field generated from the side face cross boundary SPAvector
	DRAFT_NORMAL_FIELD,	// SPAvector field generated from an angle to a base SPAvector (field)
	DRAFT_SIDE_FIELD	// SPAvector field generated from an angle to side faces of a base face
};

/** @} */

/*
// tbrv
*/
/**
 * @nodoc
 */

enum pvec_status { COND_INVALID, COND_FREE, COND_VALID };

/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_INTR PositionVector : public ACIS_OBJECT
{
public:
	PositionVector();

	SPAunit_vector	vec;	// constraint
	SPAposition    pos;
	double      param;
	pvec_status status;

	PositionVector &operator=(PositionVector &);
	void		set(SPAunit_vector &, SPAposition &, double);

#ifdef SPA_DEBUG
	void debug(FILE *, SPAtransf &);
#endif
};

/*
// tbrv
*/
/**
 * @nodoc
 */
class DECL_INTR CoedgeField : public ACIS_OBJECT
{
	fieldtype        type;
	COEDGE	 		*base_coed;
	SPAinterval         base_range;

	FACE            *base_face;
	double           base_draft;

	law				*normal_field;
	law             *tangent_field;
	law             *cross_field;

	law				*unit_normal_field;
	law             *unit_tangent_field;
	law             *unit_cross_field;

	law             *result_field;

	PositionVector   st_cond;
	PositionVector   nd_cond;
	int				 n_conds;
	int				 n_alloc;
	PositionVector  *interior_conds;

	logical          rev;
	logical			 m_forward;

	law				*get_normal(logical fnorm=FALSE);
	law             *get_tangent(logical fnorm=FALSE);
	law             *get_cross(logical fnorm=FALSE);

	int				 m_mark;			// internal use

public:

	CoedgeField(COEDGE *,				// base coedge
				fieldtype,				// mark how the resulting SPAvector field relates to the field face
		        law *,					// normal law
				int fwd=TRUE,			// coedge direction to use
				int rev=FALSE,			// reverse flag
				double draft=0,			// draft angle
				SPAinterval &range=*(class SPAinterval *) NULL_REF);

	CoedgeField(COEDGE *,				// base coedge
				fieldtype,				// mark how the resulting SPAvector field relates to the field face
		        FACE *,					// face associated with the coedge
				int fwd=TRUE,			// coedge direction to use
				int rev=FALSE,			// reverse flag
				double draft=0,			// draft angle
				SPAinterval &range=*(class SPAinterval *) NULL_REF);

	~CoedgeField();
	law *field(PositionVector *st=NULL, PositionVector *nd=NULL);

	int		     set_next(CoedgeField *);
	int			 validate();
	logical      compatible_vector(SPAunit_vector &, double, SPAunit_vector &mod= *(SPAunit_vector *) NULL_REF);
	int			 make_complete();
	void		 align_next();
	void		 align_prev();

	COEDGE		*coedge()	{ return base_coed; }
	SPAinterval	 range()	{ return base_range; }
	logical      forward()	{ return m_forward; }
	void         reverse();

	SPAunit_vector eval_normal(double);
	SPAunit_vector eval_tangent(double);
	SPAunit_vector eval_cross(double);
	SPAposition    eval_position(double);

	int			add_edge_constrain(EDGE *); // return -1: invalid, 0: not intersect, 1: valid and added

	void		set_mark(int in_mark) { m_mark=in_mark; }
	int			mark() { return m_mark; }

	CoedgeField *next;
	CoedgeField *prev;

#ifdef SPA_DEBUG
	void debug(FILE *);
#endif
};
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_INTR law ** create_boundary_field(
	FACE	    *in_fas,
	fieldtype    ftype,
	int		     rev=FALSE,
	double       draft=0,
	SPAvector      &uniform_vec= *(SPAvector *) NULL_REF,
	ENTITY_LIST &cons_eds= *(ENTITY_LIST *) NULL_REF,
	int          global=FALSE);
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_INTR law ** create_boundary_field(
	WIRE	    *in_wire,
	fieldtype    ftype,
	int		     rev=FALSE,
	double       draft=0,
	SPAvector      &uniform_vec= *(SPAvector *) NULL_REF,
	ENTITY_LIST &cons_eds= *(ENTITY_LIST *) NULL_REF,
	int          global=FALSE);
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_INTR law ** create_boundary_field(
	ENTITY_LIST &coeds,
	FACE       **ref_faces,
	fieldtype    ftype=FACE_SIDE_FIELD,
	int		     rev=FALSE,
	double       draft=0,
	SPAvector      &uniform_vec= *(SPAvector *) NULL_REF,
	ENTITY_LIST &cons_eds= *(ENTITY_LIST *) NULL_REF,
	int          global=FALSE);
/*
// tbrv
*/
/**
 * @nodoc
 */
DECL_INTR law ** create_boundary_field(
	ENTITY_LIST &coeds,
	law        **field_laws,
	double       draft=0,
	ENTITY_LIST &cons_eds= *(ENTITY_LIST *) NULL_REF,
	int		     global=FALSE);

#endif // COEDFIELD_HXX


