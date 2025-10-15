/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

//----------------------------------------------------------------------------

#if !defined	ATTRIB_HH_ENT_GEOMBUILD_PCURVE_CLASS
#define			ATTRIB_HH_ENT_GEOMBUILD_PCURVE_CLASS
//----------------------------------------------------------------------------

#include	"dcl_heal.h"
#include	"entgmbld.hxx"

class ATTRIB_HH_AGGR_GEOMBUILD_BASE;
class PCURVE;
//----------------------------------------------------------------------------
/**
 * @nodoc
 */
ENTITY_IS_PROTOTYPE(ATTRIB_HH_ENT_GEOMBUILD_PCURVE, HEAL )
#if 0
; // semicolon needed for mkman (doc tool) parsing
#endif

DECL_HEAL extern int ATTRIB_HH_ENT_GEOMBUILD_PCURVE_TYPE;
/**
 * @nodoc
 */
#define ATTRIB_HH_ENT_GEOMBUILD_PCURVE_LEVEL (ATTRIB_HH_ENT_GEOMBUILD_BASE_LEVEL + 1 )

//----------------------------------------------------------------------------

class DECL_HEAL ATTRIB_HH_ENT_GEOMBUILD_PCURVE : public ATTRIB_HH_ENT_GEOMBUILD_BASE {
	protected:
	int continuity;			// = 1 -> curve not continuous
	int degeneracy;			// = 1 -> curve degenerate
	int selfint;			// = 1 -> curve self-intersects
	int closure;			// = 1 -> curve not closed
	int approx_fit;			// = 1 -> approximate fit
	int within_domain;		// = 1 -> curve SPAparameter not within surface SPAparameter range
// STI cpp begin
		logical save_sw;
// STI cpp end



	public:
		ATTRIB_HH_ENT_GEOMBUILD_PCURVE( PCURVE *e=NULL);
		/**
		 * @nodoc
		 */
		ATTRIB_FUNCTIONS( ATTRIB_HH_ENT_GEOMBUILD_PCURVE, HEAL )
#if 0
; // semicolon needed for mkman (doc tool) parsing)
#endif
// Because mkman isn't aware that ATTRIB_FUNCTIONS changes access to "public", we do so
// explicitly here:
public:

    virtual logical pattern_compatible() const;
                                    // returns TRUE
	// Set functions
	void set_continuity (int q) {continuity = q;}
	void set_degeneracy (int q) {degeneracy = q;}
	void set_selfint (int q) {selfint = q;}
	void set_closure (int c) {closure = c;}
	void set_approx_fit (int q) {approx_fit = q;}
	void set_within_domain (int q) {within_domain = q;}

	// Get functions
	int get_continuity () const {return continuity;}
	int get_degeneracy () const {return degeneracy;}
	int get_selfint () const {return selfint;}
	int get_closure () const {return closure;}
	int get_approx_fit () const {return approx_fit;}
	int get_within_domain () const {return within_domain;}
	int get_quality () const;

	// Reset attribute
	void reset() ;
	int adv_check();
};




#endif  //ATTRIB_HH_ENT_GEOMBUILD_PCURVE_CLASS
