// $Id: sw_curve_law.hxx,v 1.8 2002/08/09 17:13:30 jeff Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef SW_CURVE_LAW_HXX
#define SW_CURVE_LAW_HXX

#include "dcl_law.h"
#include "law_base.hxx"
#include "law_data.hxx"
#include "sw_curve.hxx"

/////////////////////////////////////////////////////////////
//
// The sw_curve_law_data class
//
/////////////////////////////////////////////////////////////
class DECL_LAW sw_curve_law_data: public base_curve_law_data
{
protected:
	sw_curve      *scurve;		
public:				  
	~sw_curve_law_data();
	sw_curve_law_data();
	sw_curve_law_data(sw_curve const &in_scurve,
				   double in_start=0,
				   double in_end=0);
	void  save();
	law_data *restore();
	char  const *symbol(law_symbol_type type);
	SPAvector eval(double para,int deriv, int side=0);
	double curvature(double para);
	double length_param(double base,double length);
	double length(double start,double end);
	law *law_form();
	sw_curve *curve_data();
	double point_perp(SPAposition in_point);
	double point_perp(SPAposition in_point,
		              double   in_t); 
	law_data *deep_copy(base_pointer_map * pm = NULL) const;
	law_data *set_domain(SPAinterval *new_domain);
	int  date();
	// STI ROLL
	virtual void full_size(SizeAccumulator&, logical = TRUE) const;
	// STI ROLL
};


#endif
