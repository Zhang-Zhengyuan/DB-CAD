/* ORIGINAL: acis2.1/faceter/vu/af_intvl.hxx */
// $Id: af_intvl.hxx,v 1.9 2001/01/11 18:02:48 ywoo Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef _AF_INTVL
#define _AF_INTVL

#include "dcl_fct.h"
// ywoo 08Jan01: added the header files.
#include "mmgr.hxx"
// ywoo: end

//DOC
// TOPIC(INTERVAL,Math classes;Faceter classes)
//
//	An instance of the INTERVAL class represents a
//	finite SPAinterval of real (1D) space.
//
//END
class DECL_FCT INTERVAL : public ACIS_OBJECT {
	private:
		int nullrange;
		double low,high;
	public:
		INTERVAL(){ nullrange = 1;}
		INTERVAL(double a){
			 nullrange = 0;
			 low  = a;
			 high = a;
			}
		INTERVAL(double a,double b){
				if( b < a){
					nullrange = 1;
				}else{
					nullrange = 0;
					low = a;
					high = b;
				}
			}
		double start_pt()const{
			return (nullrange ? 1 : low);
		}
		double end_pt()const{
			return (nullrange ? 0 : high);
		}

		INTERVAL &operator|=(const INTERVAL &J);

		INTERVAL &operator|=(double a){
			if(nullrange){
				nullrange = 0;
				low = high = a;
			}else{
				if(a < low)low = a;
				if(a > high)high = a;
			}
			return *this;
		}
};

#endif
