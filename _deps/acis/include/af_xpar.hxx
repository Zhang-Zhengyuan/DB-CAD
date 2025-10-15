/* ORIGINAL: acis2.1/faceter/vu/af_xpar.hxx */
// $Id: af_xpar.hxx,v 1.9 2000/12/26 18:35:45 products Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _AF_XPAR
#define _AF_XPAR




#include "dcl_fct.h"
#include "af_par.hxx"

class DECL_FCT AF_PAR_TRANS : public ACIS_OBJECT {
 public:
	virtual PAR_POS xform(const PAR_POS &uv) const;
	virtual PAR_POS invert(const PAR_POS &uv) const;
	AF_PAR_TRANS();		// Create identity transform.
	virtual ~AF_PAR_TRANS() {}
	virtual int identity() const;	// Test if the transform is an identity.
};

// AF_PAR_TRANS_FLIP_X flips the sign of the x coordinate.
class DECL_FCT AF_PAR_TRANS_FLIP_X : public AF_PAR_TRANS
//  Typical application: Obtain a working coordinate system
//		that changes CW orientations to CCW.
{
 public:
	virtual PAR_POS xform(const PAR_POS &uv) const;
	virtual PAR_POS invert(const PAR_POS &uv) const;
	AF_PAR_TRANS_FLIP_X();
	virtual int identity() const;	// Test if the transform is an identity.
};

// AF_PAR_TRANS_CLOCKWISE_90 rotates 90 degrees clockwise.
//  Typical application: Obtain a working coordinate system
//		where the usual u direction is vertical
//
class DECL_FCT AF_PAR_TRANS_CLOCKWISE_90 : public AF_PAR_TRANS
{
 public:
	virtual PAR_POS xform(const PAR_POS &uv) const;
	virtual PAR_POS invert(const PAR_POS &uv) const;
	AF_PAR_TRANS_CLOCKWISE_90();
	virtual int identity() const;	// Test if the transform is an identity.
};

// AF_PAR_TRANS_CLOCKWISE_180 rotates 180 degrees clockwise.
//  Typical application: Obtain a working coordinate system
//		with u or v direction flipped backwards but maintaining
//		orientation.
//
class DECL_FCT AF_PAR_TRANS_CLOCKWISE_180 : public AF_PAR_TRANS
{
 public:
	virtual PAR_POS xform(const PAR_POS &uv) const;
	virtual PAR_POS invert(const PAR_POS &uv) const;
	AF_PAR_TRANS_CLOCKWISE_180();
	virtual int identity() const;	// Test if the transform is an identity.
};

class DECL_FCT AF_PAR_TRANS_COUNTERCLOCKWISE_90 : public AF_PAR_TRANS
{
 public:
	virtual PAR_POS xform(const PAR_POS &uv) const;
	virtual PAR_POS invert(const PAR_POS &uv) const;
	AF_PAR_TRANS_COUNTERCLOCKWISE_90();
	virtual int identity() const;	// Test if the transform is an identity.
};

class DECL_FCT AF_PAR_TRANS_SCALED_CCW_45 : public AF_PAR_TRANS
{
 public:
	virtual PAR_POS xform(const PAR_POS &uv) const;
	virtual PAR_POS invert(const PAR_POS &uv) const;
	AF_PAR_TRANS_SCALED_CCW_45();
	virtual int identity() const;	// Test if the transform is an identity.
};

class DECL_FCT AF_PAR_TRANS_SCALE : public AF_PAR_TRANS
{
 private:
	double su,sv;		// scale factors.
 public:
	virtual PAR_POS xform(const PAR_POS &uv) const;
	virtual PAR_POS invert(const PAR_POS &uv) const;
	AF_PAR_TRANS_SCALE(double su,double sv);
	virtual int identity() const;	// Test if the transform is an identity.
};



#endif
