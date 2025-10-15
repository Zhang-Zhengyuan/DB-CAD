/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (BHL_SPLINE_UTL2)
#define BHL_SPLINE_UTL2

#include "sp3crtn.hxx"
#include "bs3cutil.hxx"
#include "sp2crtn.hxx"

#include "dcl_stitch.h"

class surface;
class COEDGE;
class curve;
class pcurve;
class SPAposition;

// Functional Class for Converting bs2_curve to bs3_curve
class DECL_STITCH bs3_from_bs2_curve
{

// Attributes
protected:
	const bs2_curve m_bs2;	// Input bs2_curve
	const surface * const m_surf;	// Surface for evaluation

// Exposed Functions
public:

	// Constructor
	bs3_from_bs2_curve(const bs2_curve& bs2, const surface& surf) : m_bs2(bs2),m_surf(&surf)
	{ /* do nothing here */	};

	// Destructor
	virtual ~bs3_from_bs2_curve(){/* do nothing here right now*/};

	// Main Function to get the bs3 from the bs2
	virtual logical get_bs3_from_bs2_curve(bs3_curve &);

// Overridables
protected:

	// Project point (surface eval)
	virtual SPAposition project_point(SPApar_pos);

}; //  end of class BS3_FROM_BS2


// Other spline utility functions
DECL_STITCH logical
bhl_get_bs2_from_bs3(bs3_curve& , const surface& , bs2_curve& );

DECL_STITCH logical
bhl_get_bs3_from_bs2(bs2_curve& , const surface& , bs3_curve&);

#endif

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
