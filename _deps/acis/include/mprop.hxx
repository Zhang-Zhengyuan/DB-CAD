/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/


#if !defined( MPROP_CLASS )
#define MPROP_CLASS

#include "logical.h"

#include "acis.hxx"
#include "dcl_query.h"

#include "debugmsc.hxx"

#include "tensor.hxx"

#include "matrix.hxx"
#include "position.hxx"
#include "vector.hxx"


/**
* @file mprop.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 */

/** \addtogroup INTRAPIPHYSICALPROPS
 *  \brief Declared at <mprop.hxx>
 *  @{
 */

/**
 * Option enumerator for use with <tt>mass_props_options</tt>, specifying
 * the way in which voids and open, single-sided faces should be treated in
 * calculations of mass properties.
 * @param AS_ERROR
 * If body contains open, single-sided faces, or forms a closed void,
 * return volume as zero and issue a system error.
 * @param AS_2SIDED
 * Calculate volume as if any open, single-sided sheets in the body were
 * double-sided sheets.
 * @param AS_SOLID
 * Calculate volume as if any open, single-sided sheets in the body were
 * closed solids.
 * <br><br>
 * @see api_body_mass_props, mass_props_options
 */
enum one_sided_sheet_opt
{
    AS_ERROR,
    AS_2SIDED,
    AS_SOLID
};

/**
 * Enumerator for use with <tt>api_body_mass_props</tt>, specifying
 * the quantities returned by mass-property calculations.
 * @param NO_MASS_PROPS
 * No quantities will be (or have been) calculated.
 * @param VOLUME_ONLY
 * Only the volume will be (or has been) calculated.
 * @param VOLUME_AND_CENTROID
 * Only the volume and centroid will be (or have been) calculated.
 * @param VOLUME_CENTROID_AND_INERTIA
 * The volume, centroid, and inertia will be (or have been) calculated.
 * <br><br>
 * @see mass_props, mass_props_options
 */
enum mass_props_level
{
    NO_MASS_PROPS = -1,
    VOLUME_ONLY,
    VOLUME_AND_CENTROID,
    VOLUME_CENTROID_AND_INERTIA
};


/**
 * Defines a class for specifying options used in mass-property calculations.
 * <br>
 * <b>Role:</b> This class is used to configure mass-property calculations carried out
 * by <tt>api_body_mass_props</tt>.  It allows users to specify the quantities to be 
 * calculated, projection-plane information, and the desired accuracy of the results.  
 * In addition, it can be used to specify the way in which sheets are treated.  This
 * information is attached to a <tt>mass_props_options</tt> object by means of
 * class methods before use by <tt>api_body_mass_props</tt>.
 * <br><br>
 * @see api_body_mass_props, mass_props, mass_props_level, one_sided_sheet_opt
 */
class DECL_QUERY mass_props_options : public ACIS_OBJECT {
    SPAposition         _plane_root;
    SPAvector           _plane_normal;
	struct {
		unsigned use_plane_info:1;
	} _flag;
    mass_props_level    _mp_level;
    double              _req_rel_accy;
    double              _sheet_thickness;
    one_sided_sheet_opt _osso;

public:
/**
 * C++ allocation constructor requests memory for this object and sets the default values
 * of its member data.
 * <br>
 * <b>Role:</b> Unless subsequently modified by another class method, use of an object
 * created by this constructor in calculating mass properties results in the following
 * default behavior:
 * <br><br>
 * 1) projection-plane information is selected by the API;
 * <br>
 * 2) volume, centroid, and inertia are all calculated;
 * <br>
 * 3) the requested relative accuracy is one-percent;
 * <br>
 * 4) sheets are treated as having zero thickness; and,
 * <br>
 * 5) the <tt>one_sided_sheet_opt</tt> setting is <tt>AS_ERROR</tt>.
 */
    mass_props_options();

/**
 * Returns a position and normal vector defining the projection plane to be used in 
 * calculating mass properties.
 */
    void get_plane_info(SPAposition& plane_root, SPAvector& plane_normal) const;

/**
 * Returns a flag indicating whether or not user-specified projection-plane information 
 * will be used in calculating the mass properties.
 */
    logical get_use_plane_info() const;

/**
 * Returns an indicator of the mass properties to be calculated.
 */
    mass_props_level get_level() const;

/**
 * Returns the desired relative accuracy for mass property calculations.
 */
    double get_req_rel_accy() const;

/**
 * Returns the thickness to be associated with sheets in mass property calculations.
 */
    double get_sheet_thickness() const;

/**
 * Returns an indicator of the way in which voids and open, one-sided sheets
 * will be treated.
 */
    one_sided_sheet_opt get_one_sided_sheet_opt() const;

/**
 * Sets the position and normal vector defining the projection plane to be used in 
 * calculating mass properties.
 * <br>
 * <b>Role:</b> The algorithm behind <tt>api_body_mass_props</tt> works by adding
 * the volumes obtained by sweeping the faces of a solid model to a reference plane.  
 * It is not necessary for the user to specify this plane, as the algorithm will 
 * attempt to make an optimal choice automatically.  This method is provided only for 
 * cases in which the user desires to override this choice.  Note that the values 
 * entered here will not be used unless the <tt>set_use_plane_info</tt> method is 
 * also called with an argument of <tt>TRUE</tt>.
 * <br><br>
 * When using this method, one should attempt to select a projection plane that 
 * will simplify the body as much as possible.  For instance, if the body were 
 * an extrusion, one would choose a plane perpendicular to the extrusion direction.  
 * This makes the numerical integration simpler.  In general, for speed, 
 * choose the plane so that as many faces as possible project onto it as lines; 
 * i.e., are edge-on to it. To improve the accuracy of the result, set
 * the plane to pass through a point within the body; generally, this is the
 * mid-point of its box.
 * <br><br>
 */
    void set_plane_info(const SPAposition& plane_root, const SPAvector& plane_normal);

/**
 * Sets a flag indicating whether or not user-specified projection-plane information 
 * will be used in calculating the mass properties.
 * <br>
 * <b>Role:</b> (See comments in the description of <tt>set_plane_info</tt>.)
 */
    void set_use_plane_info(logical use_plane_info); 

/**
 * Sets an indicator of the mass properties to be calculated. 
 * <br>
 * <b>Role:</b>  The choice of settings is as follows: 
 * <br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td width = 5%><tt>VOLUME_ONLY</tt></td>
 * <td>Only volume is calculated.</td>
 * </tr>
 * <tr>
 * <td><tt>VOLUME_AND_CENTROID</tt></td>
 * <td>Only volume and center of gravity are calculated.</td>
 * </tr>
 * <tr>
 * <td><tt>VOLUME_CENTROID_AND_INERTIA</tt></td>
 * <td>All properties (volume, center of gravity, and moments of inertia) are calculated.</td>
 * </tr>
 * </table>
 */
    void set_level(mass_props_level mp_level); 

/**
 * Sets the desired accuracy for mass property calculations to the specified relative error.
 * <br>
 * <b>Role:</b>  For example, entering 0.01 requests a maximum error of one percent.
 * <br><br>
 * Note that there is a limitation to the current algorithm in that it does have a 
 * bound on how precisely the algorithm can calculate mass properties, because of 
 * hard-coded convergence criteria in the functions.  When the returned accuracy exceeds
 * the requested value, the application may try tightening the latter and repeating
 * the calculation in order to achieve a more accurate result.  If the mass-properties
 * values nevertheless remain unchanged by this process, they are as close as can be 
 * achieved by the algorithm.
 */
    void set_req_rel_accy(double req_rel_accy);
    
/**
 * Sets the thickness to be associated with sheets in mass property calculations.
 * <br>
 * <b>Role:</b>  Generally, double-sided (sheet or embedded) faces are ignored when 
 * calculating mass properties - the assumption being that a sheet of zero thickness 
 * has a zero volume. This behavior can be altered by changing the value of the
 * <tt>sheet_thickness</tt> argument from its default of 0.0 to a positive value. This will
 * cause <tt>api_body_mass_props</tt> to treat double-sided sheets in the body as being
 * approximations to thin-shell bodies with the given thickness, and contributions
 * to the mass properties will be calculated for them also. Some caveats apply when
 * using this:
 * <br><br>
 * 1) The approximation is only valid for "small" values of the thickness. The range 
 * of thicknesses that can be considered "small" is application dependent.
 * <br>
 * 2) Double-sided faces that meet non-tangentially or intersect will
 * introduce errors as there is a double contribution to the mass properties from
 * those regions of the faces that are within the thickness value of each other.
 * The inside/outside flag for double-sided faces is taken into account, so a
 * double-sided face embedded in a solid will reduce the volume if it is given a
 * thickness value. Note that this means that an isolated "inside" double-sided
 * sheet will have a negative volume.
 */
    void set_sheet_thickness(double sheet_thickness);

/**
 * Sets an indicator of the way in which voids and open, one-sided sheets will be treated.  
 * <br>
 * <b>Role:</b> The choice of settings is as follows: 
 * <br>
 * <table width = 100%>
 * <tr width = 100%>
 * <td width = 5%><tt>AS_ERROR</tt></td>
 * </tr>
 * <tr>
 * <td><tt>AS_2SIDED</tt></td>
 * </tr>
 * <tr>
 * <td><tt>AS_SOLID</tt></td>
 * </tr>
 * </table>
 * The value <tt>AS_ERROR</tt> causes an error to be thrown when the body is found 
 * to have one or more open, single-sided sheets; it also produces an error when a 
 * closed solid is found to have a negative volume, i.e., when it forms a void.  This 
 * is the default setting produced by the <tt>mass_props_options</tt> constructor.  
 * The value <tt>AS_2SIDED</tt> causes <tt>api_body_mass_props</tt> to treat open, 
 * single-sided sheets as though they were double-sided, i.e., their volume is counted 
 * as zero unless a non-zero sheet-thickness option is specified.  The third alternative, 
 * the value <tt>AS_SOLID</tt>, should be used with caution.  With this choice, the 
 * "volume" of an open, single-sided sheet merely represents its projection on a plane 
 * (either a default plane, or the plane specified by means of the <tt>set_plane_info</tt> 
 * method).  It is therefore strongly dependent upon the choice of this plane and, of
 * course, not indicative of the volume of a true solid. 
 */
    void set_one_sided_sheet_opt(one_sided_sheet_opt osso);
};

/**
 * Defines a class for returning quantities obtained by mass-property calculations.
 * <br>
 * <b>Role:</b> This class is used to encapsulate the results of mass-property 
 * calculations carried out by <tt>api_body_mass_props</tt>.  It includes methods
 * for querying volume and its accuracy, center of gravity (centroid), inertia,
 * principal moments, and principal axes.  Please note that any of these quanties
 * not specifically requested of <tt>api_body_mass_props</tt> will remain set to a
 * value associated with a body of zero volume.
 * The user should use the method <tt>get_level_used</tt> to determine which of 
 * these quantities has actually been calculated.
 * <br><br>
 * @see api_body_mass_props, mass_props_options, mass_props_level, one_sided_sheet_opt
 */
class DECL_QUERY mass_props : public ACIS_OBJECT {
    
    class moments * _moments;

    friend DECL_QUERY void mp_set_moments(
        mass_props&         mp, 
        const moments&      mom);

public:
/**
 * C++ allocation constructor requests memory for this object and populates it with
 * data associated with an object of zero volume.
 */
	mass_props();	

/**
 * C++ allocation constructor requests memory for this object and populates it with
 * data associated with an original object.
 */
    mass_props( mass_props const& orig );

/**
 * Assignment operator, copies over the data associated with right side object.
 */
    mass_props &operator=( mass_props const& rhs );    

/**
 * Destructor
 */
    ~mass_props();

/**
 * Returns the level used in the object's calculation.
 * <br>
 * <b>Role:</b> This method returns <tt>NO_MASS_PROPS</tt> if no calulations have
 * been performed.
 */
    mass_props_level get_level_used() const;
/**
 * Returns the relative accuracy of the calculated volume.
 */
    double get_rel_accy_vol_achieved() const;
/**
 * Returns the volume.
 * <br>
 * <b>Role:</b> This method returns zero if the volume has not been calculated.
 */
    double get_volume() const;
/**
 * Returns the centroid (center of gravity).
 * <br>
 * <b>Role:</b> The center of gravity is calculated with respect to
 * the working coordinate system, and will therefore change if the body
 * is transformed.  This method returns (0, 0, 0) if the centroid has
 * not been calculated.
 */
    SPAposition get_centroid() const;
/**
 * Returns the inertia tensor.
 * <br>
 * <b>Role:</b> The inertia tensor is calculated with respect to the world 
 * coordinate system, and will therefore change if the body is transformed.
 * The major diagonal of the inertia tensor returned contains values for the
 * inertias about each axis. For the inertia about the <i>x</i>-axis:
 * <pre>(integral)((y*y + z*z)dV)</pre>
 * the off-diagonal terms are for the products of inertia; for example:
 * <pre>(integral)((x*y)dV)</pre>
 * This method returns the zero tensor if the inertia has not been calculated.
 */
    tensor get_inertia() const;
/**
 * Returns the principal moments.
 * <br>
 * <b>Role:</b> The principal moments of inertia are with respect to the 
 * coordinate system defined by the center of gravity and the principal axes. 
 * (These three values should be relatively constant under translational and 
 * rotational transformations, although their order may change depending on 
 * the principal axes.)  This method returns zeroes if the principal moments 
 * have not been calculated.
 */
    void get_p_moments(double p_moments[3]) const;
/**
 * Returns the principal axes.
 * <br>
 * <b>Role:</b> The principal axes are calculated with respect to the world 
 * coordinate system, and will therefore change if the body is transformed.
 * Each axis should be orthogonal to the other two axes.  This method returns 
 * the x-, y-, and z-axes if the principal axes have not been calculated.
 */
    void get_p_axes(SPAunit_vector p_axes[3]) const;
/**
 * @nodoc
 */
	const moments& get_moments() const;
};

/** @} */

#endif

