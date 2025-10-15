/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// $Id:
//

#ifndef kern_netspl_corner_hxx
#define kern_netspl_corner_hxx

#include "dcl_kern.h"
#include "logical.h"
#include "vector.hxx"
#include "position.hxx"
#include "unitvec.hxx"

// Undocumented class : For INTERNAL USE ONLY

/**
 * @nodoc
 */
class DECL_KERN netspl_corner : public ACIS_OBJECT {
/**
 * @nodoc
 */
	double t_par;
/**
 * @nodoc
 */
	double s_par;
/**
 * @nodoc
 */
	double dt_par;
/**
 * @nodoc
 */
	double ds_par;
/**
 * @nodoc
 */
	SPAposition pos;
/**
 * @nodoc
 */
	SPAvector dv;
/**
 * @nodoc
 */
	SPAvector ddv;
/**
 * @nodoc
 */
	SPAvector dddv;
/**
 * @nodoc
 */
	SPAvector du;
/**
 * @nodoc
 */
	SPAvector ddu;
/**
 * @nodoc
 */
	SPAvector dddu;
/**
 * @nodoc
 */
	SPAvector twist;
/**
 * @nodoc
 */
	SPAunit_vector mNormal;
/**
 * @nodoc
 */
	double         mDuLength;
/**
 * @nodoc
 */
	double         mDvLength;
/**
 * @nodoc
 */
	double         mAdjustAngle;
/**
 * @nodoc
 */
	void comp_cbd_data();

public:
/**
 * @nodoc
 */
	netspl_corner ();
/**
 * @nodoc
 */
	~netspl_corner();
	// Copy constructor
/**
 * @nodoc
 */
	netspl_corner (const netspl_corner& old);
/**
 * @nodoc
 */
	netspl_corner
		( 
		double      t_param,
		double      s_param,
		const SPAposition &pos_value,
		const SPAvector   &dv_value,
		const SPAvector   &du_value
		);

/**
 * @nodoc
 */
	double get_s_par () const;
/**
 * @nodoc
 */
	double get_t_par () const;
/**
 * @nodoc
 */
	double get_ds_par () const;
/**
 * @nodoc
 */
	double get_dt_par () const;

/**
 * @nodoc
 */
	SPAposition get_pos () const;
/**
 * @nodoc
 */
	SPAvector get_pos_vec () const;
/**
 * @nodoc
 */
	SPAvector get_du () const;
/**
 * @nodoc
 */
	SPAvector get_ddu () const;
/**
 * @nodoc
 */
	SPAvector get_dddu () const;
/**
 * @nodoc
 */
	SPAvector get_dv () const;
/**
 * @nodoc
 */
	SPAvector get_ddv () const;
/**
 * @nodoc
 */
	SPAvector get_dddv () const;
/**
 * @nodoc
 */
	SPAvector get_twist () const;
/**
 * @nodoc
 */
	SPAvector get_ucbd_adjust () const;
/**
 * @nodoc
 */
	SPAvector get_vcbd_adjust () const;
/**
 * @nodoc
 */
	SPAvector get_normal () const;
/**
 * @nodoc
 */
	void get_tangent_adjusts 
		(
		double   &oDuLength,
		double   &oDvLength, 
		double   &oAdjustAngle
		) const;
/**
 * @nodoc
 */
	void get_degeneracy
		(
		logical  &oUDeg,
		logical  &oVDeg
		);
/**
 * @nodoc
 */
	void set_s_par  (double par);
/**
 * @nodoc
 */
	void set_t_par  (double par);
/**
 * @nodoc
 */
	void set_ds_par (double par);
/**
 * @nodoc
 */
	void set_dt_par (double par);
/**
 * @nodoc
 */
	void set_pos  (const SPAposition &pos_value);
/**
 * @nodoc
 */
	void set_du   (const SPAvector &vec);
/**
 * @nodoc
 */
	void set_ddu  (const SPAvector &vec);
/**
 * @nodoc
 */
	void set_dddu (const SPAvector &vec );
/**
 * @nodoc
 */
	void set_dv   (const SPAvector &vec);
/**
 * @nodoc
 */
	void set_ddv  (const SPAvector &vec);
/**
 * @nodoc
 */
	void set_dddv (const SPAvector &vec );
/**
 * @nodoc
 */
	void set_twist (const SPAvector &vec);
/**
 * @nodoc
 */
	void set_curve_data 
		( 
		const SPAposition    &pos_value, 
		const SPAvector      &dv_vec, 
		const SPAvector      &ddv_vec,
		const SPAvector      &dddv_vec,
		const SPAvector      &du_vec,
		const SPAvector      &ddu_vec,
		const SPAvector      &dddu_vec
		);
/**
 * @nodoc
 */
	void set_normal (const SPAunit_vector &iNormal); 
/**
 * @nodoc
 */
	void debug_data
		(
		char const 	*leader,
		logical 	/*brief*/,
		FILE 		*fp
		) const;
};


#endif
