/* ORIGINAL: acis2.1/faceter/vu/af_par.hxx */
// $Id: af_par.hxx,v 1.9 2002/08/09 17:21:52 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//SC 01-13-04 Guy Stored UV values in LinkedMesh were only on SGI machines.
// On SGI, PAR_POS cannot be cast to SPApar_pos because PAR_POS was derived from ACIS_OBJECT,
// which made SGI compilers add more bytes to the start of the structure.

#ifndef _AF_PAR
#define _AF_PAR

#include <math.h>
#include "dcl_fct.h"
#include "af_intvl.hxx"

class PAR_POS;
class PAR_VEC;
class PAR_DIR;

PAR_VEC DECL_FCT operator-(const PAR_POS &uv0,const PAR_POS &uv1);
PAR_POS DECL_FCT operator-(const PAR_POS &uv0,const PAR_VEC &uv1);
PAR_POS DECL_FCT operator+(const PAR_POS &uv0,const PAR_VEC &uv1);
PAR_VEC DECL_FCT operator*(double a,const PAR_POS &uv1);
PAR_VEC DECL_FCT operator/(const PAR_POS &uv1,double a);

//DOC
// TOPIC(PAR_POS,Math classes;Faceter classes)
//
// An instance of the PAR_POS class represents a point in
// parametric (uv) space.
//
//END
class DECL_FCT PAR_POS {
	public:
	//MEMBER
	// The u and v fields of a PAR_POS are directly accessible by the
	//	application.
		double u,v;
	//END
	//MEMBER
	// The constructors for PAR_POS instances are:
		PAR_POS(){ u = v = 0; }
		PAR_POS(double u0,double v0){u = u0; v = v0;}
		PAR_POS(const PAR_POS &uv0){ u = uv0.u; v = uv0.v;}

		PAR_POS &operator+=(const PAR_VEC &uv1);
		PAR_POS &operator-=(const PAR_VEC &uv1);
	//END
	friend DECL_FCT PAR_VEC operator-(const PAR_POS &uv0,const PAR_POS &uv1);
	friend DECL_FCT PAR_POS operator-(const PAR_POS &uv0,const PAR_VEC &uv1);
	friend DECL_FCT PAR_POS operator+(const PAR_POS &uv0,const PAR_VEC &uv1);

	friend DECL_FCT PAR_VEC operator*(double a,const PAR_POS &uv1);
	friend DECL_FCT PAR_VEC operator/(const PAR_POS &uv1,double a);

	
	MMGR_FREELIST_THIS
};
logical operator==( PAR_POS const &uv1, PAR_POS const &uv2 );

double lensq(const PAR_POS& p1, const PAR_POS& p2);

DECL_FCT PAR_VEC operator-(const PAR_POS &uv0,const PAR_POS &uv1);
DECL_FCT PAR_POS operator-(const PAR_POS &uv0,const PAR_VEC &uv1);
DECL_FCT PAR_VEC operator-(const PAR_VEC &uv0,const PAR_VEC &uv1);
DECL_FCT PAR_POS operator+(const PAR_POS &uv0,const PAR_VEC &uv1);
DECL_FCT PAR_VEC operator+(const PAR_VEC &uv0,const PAR_VEC &uv1);
DECL_FCT PAR_VEC operator*(double a,const PAR_VEC &uv1);
DECL_FCT PAR_VEC operator/(const PAR_VEC &uv1,double a);
DECL_FCT double operator%(const PAR_VEC&uv0,const PAR_VEC &uv1);

class DECL_FCT PAR_VEC : public ACIS_OBJECT {
//DOC
// TOPIC(PAR_VEC,Math classes;Faceter classes)
//
// An instance of the PAR_VEC class represents a SPAvector in
// parametric (uv) space.
//
//END
	friend class PAR_DIR;
	public:
	//MEMBER
	// The u and v fields of a PAR_VEC are directly accessible by the
	//	application.
		double du,dv;
	//END
	//MEMBER
	// The constructors for PAR_VEC instances are:
		PAR_VEC(){ du = dv = 0; }
		PAR_VEC(double u0,double v0){du = u0; dv = v0;}
		PAR_VEC(const PAR_VEC &uv0){ du = uv0.du; dv = uv0.dv;}

		PAR_VEC &operator-=(const PAR_VEC &uv1);
		PAR_VEC &operator+=(const PAR_VEC &uv1);
		double len();
	//END

	friend DECL_FCT PAR_VEC operator-(const PAR_POS &uv0,const PAR_POS &uv1);
	friend DECL_FCT PAR_POS operator-(const PAR_POS &uv0,const PAR_VEC &uv1);
	friend DECL_FCT PAR_VEC operator-(const PAR_VEC &uv0,const PAR_VEC &uv1);
	friend DECL_FCT PAR_POS operator+(const PAR_POS &uv0,const PAR_VEC &uv1);
	friend DECL_FCT PAR_VEC operator+(const PAR_VEC &uv0,const PAR_VEC &uv1);
	friend DECL_FCT PAR_VEC operator*(double a,const PAR_VEC &uv1);
	friend DECL_FCT PAR_VEC operator/(const PAR_VEC &uv1,double a);
	friend DECL_FCT double operator%(const PAR_VEC&uv0,const PAR_VEC &uv1);
	PAR_VEC &operator=(const PAR_VEC &);
};

class PAR_BOX;
PAR_BOX operator|(const PAR_BOX &,const PAR_BOX&);

//DOC
// TOPIC(PAR_BOX,Math classes;Faceter classes)
//
// An instance of the PAR_POS class represents a SPAbox in
// 2D parametric (uv) space.
//
//END
class DECL_FCT PAR_BOX : public ACIS_OBJECT {
 private:
	int nullbox;
 public:
	PAR_POS uv0;
	PAR_POS uv1;
	PAR_BOX(){ nullbox = 1; uv0 = PAR_POS(1,1); uv1 = PAR_POS(0,0); }
	PAR_BOX(const PAR_BOX &box0)
		{ nullbox = box0.nullbox;
			uv0 = box0.uv0;
			uv1 = box0.uv1;
		}
	PAR_BOX(const PAR_POS &U)
		{ nullbox = 0;
			uv0 = uv1 = U;
		}
	PAR_BOX(const PAR_POS &U0, const PAR_POS &U1)
		{ nullbox = 0;
			uv0 = U0;
			uv1 = U1;
		}
	PAR_BOX &operator|=(const PAR_POS&);
	PAR_BOX &operator|=(const PAR_BOX&);
	friend PAR_BOX operator|(const PAR_BOX &,const PAR_BOX&);
	INTERVAL u_range()const;
	INTERVAL v_range()const;

	PAR_POS low() const{ return uv0; }
	PAR_POS high() const{ return uv1; }
};

class DECL_FCT PAR_DIR : public PAR_VEC {
 public:
	PAR_DIR(){}
	PAR_DIR(PAR_VEC const &V){
		double a;
		du = V.du;
		dv = V.dv;
		a = sqrt(du*du + dv*dv);
		if(a > 0){
			du /=a;
			dv /=a;
		}
	}
	PAR_DIR(double uu,double vv){
		double a = sqrt(uu*uu + vv*vv);
		if(a<=0)a = 1;
		du = uu/a;
		dv = vv/a;
	}
};

#endif
