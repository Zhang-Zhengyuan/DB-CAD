/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined (BHL_SNAP_TRANS)
#define BHL_SNAP_TRANS

#include "acis.hxx"
#include "transf.hxx"
#include "dcl_heal.h"
class HH_Trans;
DECL_HEAL HH_Trans operator*(HH_Trans const &, HH_Trans const & );
DECL_HEAL HH_Trans operator*( HH_Trans const &t1, HH_Trans const *t2 ) ;

class DECL_HEAL HH_Trans : public  SPAtransf
{
private:
	double minor_scale;//scale factor for minor radius of torus
	double major_scale;//scale factor for major radius of torus

public:

	HH_Trans(); // creates the identity transformation

	HH_Trans( SPAtransf const &,double = 1,double =1 );	
	
	HH_Trans( HH_Trans const & ); //Copy	

	// Return the inverse transformation (must be no shear in the
	// given transformation).

	HH_Trans inverse() const;
	double get_minor_scale() const {return minor_scale;}
	double get_major_scale() const {return major_scale;}
	void set_major_scale( double );
	void set_minor_scale(double);
	friend DECL_HEAL HH_Trans operator*( HH_Trans const &, HH_Trans const & );
	friend DECL_HEAL HH_Trans operator*(
						HH_Trans const &t1,
						HH_Trans const *t2
					)
	{
		return t1 * (*t2);
	}

};



#endif


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------


