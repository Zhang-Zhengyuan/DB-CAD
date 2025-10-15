/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header for api routines.

// For details of how to call each routine, consult the documentation
// for individual routines.

#if !defined( BLEND_FACE_HXX )
#define BLEND_FACE_HXX



class SPACOLLECTION;
class SURFACE;
class COEDGE;
class FACE;

#include <float.h>
#include "api.hxx"
#include "dcl_blnd.h"
#include "logical_state.hxx"

class DECL_BLND SPA_blend_face_info : public ACIS_OBJECT
{
private:
	ENTITY_LIST _supports;
	logical_state _const_radius;
	double _radius;

public:
	SPA_blend_face_info();

	~SPA_blend_face_info();

	void set_supports(ENTITY_LIST &supports);
	void set_const_radius(logical const_radius);
	void set_radius(double radius);

	ENTITY_LIST get_supports();
	logical get_const_radius();
	double get_radius();

};
/**
 * @nodoc
 */
DECL_BLND outcome api_is_blend_face(FACE *face, logical &return_val, SPA_blend_face_info &bld_f_info = *(SPA_blend_face_info *)NULL_REF, AcisOptions *ao = NULL);

/**
 * @nodoc
 */
DECL_BLND outcome api_set_blend_face(FACE *face, SPA_blend_face_info &bld_f_info = *(SPA_blend_face_info *)NULL_REF, AcisOptions *ao = NULL);

/**
 * @nodoc
 */
DECL_BLND outcome api_get_lost_support(COEDGE *coedge, SURFACE *&lost_support, AcisOptions *ao = NULL);

/**
 * @nodoc
 */
DECL_BLND outcome api_set_lost_support(COEDGE *coedge, SURFACE *lost_support, AcisOptions *ao = NULL);


#endif //BLEND_FACE_HXX
