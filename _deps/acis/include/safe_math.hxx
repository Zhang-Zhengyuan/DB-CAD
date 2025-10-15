/* $Id: safe_math.hxx,v 1.9 2002/08/09 17:10:26 jeff Exp $ */
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// This header includes all rounding safe math

#if !defined( SAFE_MATH_HXX )
#define SAFE_MATH_HXX
#include <math.h>
#include <float.h>
#include "base.hxx"
#include "errorbase.err"
#include "errorbase.hxx"

inline double safe_acos( double angle )
{ 
	return angle >= 1.0 ? 0.0  :  angle <= -1.0 ? M_PI  :  acos(angle); 
}

inline double safe_asin( double angle )
{ 
	return angle >= 1.0 ? M_PI/2.0  :  angle <= -1.0 ? -M_PI/2.0  :  asin(angle); 
}

inline double safe_atan2(double x, double y)
{
	if(x == 0.0 && y == 0.0)
    {
		sys_error(MATH_DOMAIN);
        return 0.0;
    }
    else
	    return atan2(x,y);
}

inline double safe_sqrt(double a)
{
    if (a >= 0.0)
        return sqrt(a);
    else if (a < 0.0 && a > -SPAresmch)
        return 0.0;
    else
    {
		sys_error(MATH_DOMAIN);
        return 0.0;
    }
}

inline double safe_fabs(double d)
{
	return (d < 0.0) ? -d : d;
}

inline double safe_pow(double x, double y)
{
	if (y == 2.0)
		return x*x;

	if (x == 1.0) 
		return 1.0;

	if (x == 0.0)
	{
		if (y < 0.0)
			return DBL_MAX;
		else 
			return 0.0;
	}
	return pow(x,y);
}

#endif
