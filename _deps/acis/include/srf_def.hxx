/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

// Header to interface between Acis and AGlib (or in principle any
// other spline package). This simply provides a definition for a
// spline curve type for the major part of Acis to use.

#if !defined( BS3_SURF_DEF )
#define BS3_SURF_DEF

// Haul in the typedef for bs3_surface - it will always be wanted by
// anyone who uses this file.

#include "dcl_kern.h"
#include "mmgr.hxx"
#include "bs3surf.hxx"
#include "dcl_spl.h"
#include "base.hxx"

struct ag_surface;
struct ag_snode;


class DECL_SPLINE bs3_surf_def : public ACIS_OBJECT {

private:

	ag_surface *sur;		// the surface itself

	bs3_surf_form formu;	// Acis' opinion of the end conditions in each
	bs3_surf_form formv;	// SPAparameter direction - open, closed or "periodic".
	size_t state;				// the paged state
    int m_initial_u_seam_mult; //  knot multiplicity at creation when periodic data is input 
    int m_start_u_mult;
    int m_initial_v_seam_mult;
    int m_start_v_mult;

#ifdef GEOMPAGE_DEBUG
public:
	bs3_surf_def *prev;
	bs3_surf_def *next;
#endif

public:

	bs3_surf_def( ag_surface *, 
                  bs3_surf_form = bs3_surf_unknown_ends,
                  bs3_surf_form = bs3_surf_unknown_ends,
                  int = -1,
                  int = 0,
                  int = -1,
                  int = 0
                  );

	~bs3_surf_def();

	void trash();

	ag_surface * get_sur();
	ag_surface ** get_sur_ref();
	void set_sur( ag_surface *c);

	bs3_surf_form get_formu();
	void set_formu( bs3_surf_form fu);

	bs3_surf_form get_formv();
	void set_formv( bs3_surf_form fu);

	void page();
	void unpage();
	int paged() { return state != 0; }

    void set_initial_seam_multiplicity(int u_seam_mult, int u_start_mult, int v_seam_mult, int v_start_mult);

    int  initial_u_seam_multiplicity(int &start_u_mult = *(int*)NULL_REF);
    int  initial_v_seam_multiplicity(int &start_v_mult = *(int*)NULL_REF);
};

#endif
