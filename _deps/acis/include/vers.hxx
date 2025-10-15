/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#ifndef ACIS_VERSION_HXX
#define ACIS_VERSION_HXX

#include <stddef.h>
#include "logical.h"
#include "base.hxx"
#include "dcl_base.h"

/**
* @file vers.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISVERSIONING
 *
 * @{
 */


/**
 * Returns the major version number of the ACIS executable.
 * <br><br>
 * <b>Role:</b> The ACIS product binaries contain information to identify its version.
 * This version information consists of 3 numbers: a major, a minor, and a point number.
 * All ACIS releases are versioned and can be identified by these numbers.
 * The ACIS 8.0 release, for example, has a major number of 8,
 * a minor number of 0, and a point number of 0. For simplicity, these numbers
 * are depicted with dot separation (e.g. 8.0.0).
 * <br><br>
 * The first service pack to the ACIS 8.0 release would consequently be identified as 8.0.1.
 * The major number obviously describes the major release version number,
 * the minor number describes the minor release version number, and the point version
 * describes the service pack level of the product.
 * <br><br>
 * The purpose of the 3-part ACIS versioning system is not only to identify the significance
 * of the changes and enhancements of the product but also to provide expectations
 * of the impact these changes can pose to the customer. Major releases signify
 * significant changes to the product that could require changes in customer code
 * in order to integrate the release. These changes, which require rebuilding and re-linking,
 * are simplified by the use of the update tool, which is delivered with the ACIS product.
 * Minor releases signify minor changes to the product that do not require changes in customer code,
 * but could require a complete rebuild and re-link of the customer application in order to
 * integrate the release. Point releases (service packs) signify minimal changes to the product
 * that do not require any code changes, rebuilds, or re-links, in order to integrate the release.
 * <br><br>
 * The current version of the ACIS product can be queried at runtime by calling
 * the <tt>get_major_version()</tt>, <tt>get_minor_version()</tt>, and <tt>get_point_version()</tt> functions,
 * which return the major, minor, and point version numbers respectively.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @see get_minor_version, get_point_version
 **/
DECL_BASE int get_major_version();

/**
 * Returns the minor version number of the ACIS executable.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @see get_major_version, get_point_version
 **/
DECL_BASE int get_minor_version();

/**
 * Returns the point version number of the ACIS executable.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * @see get_major_version, get_minor_version
 */
DECL_BASE int get_point_version();


/**
 * @nodoc
 */
class AcisVersion;


/**
 * Compares two <tt>AcisVersion</tt> objects for equality.
 * <br><br>
 * @param avL
 * left AcisVersion.
 * @param avR
 * right AcisVersion.
 */
DECL_BASE logical operator==(AcisVersion const& avL, AcisVersion const& avR);

/**
 * Compares two <tt>AcisVersion</tt> objects for inequality.
 * <br><br>
 * @param avL
 * left AcisVersion.
 * @param avR
 * right AcisVersion.
 */
DECL_BASE logical operator!=(AcisVersion const& avL, AcisVersion const& avR);

/**
 * Compares two <tt>AcisVersion</tt> objects for greater than or equal to.
 * <br><br>
 * @param avL
 * left AcisVersion.
 * @param avR
 * right AcisVersion.
 */
DECL_BASE logical operator>=(AcisVersion const& avL, AcisVersion const& avR);

/**
 * Compares two <tt>AcisVersion</tt> objects for less than or equal to.
 * <br><br>
 * @param avL
 * left AcisVersion.
 * @param avR
 * right AcisVersion.
 */
DECL_BASE logical operator<=(AcisVersion const& avL, AcisVersion const& avR);

/**
 * Compares two <tt>AcisVersion</tt> objects for less than.
 * <br><br>
 * @param avL
 * left AcisVersion.
 * @param avR
 * right AcisVersion.
 */
DECL_BASE logical operator<(AcisVersion const& avL, AcisVersion const& avR);

/**
 * Compares two <tt>AcisVersion</tt> objects for greater than.
 * <br><br>
 * @param avL
 * left AcisVersion.
 * @param avR
 * right AcisVersion.
 */
DECL_BASE logical operator>(AcisVersion const& avL, AcisVersion const& avR);

/**
 * Creates an <tt>AcisVersion</tt> object.
 * <br>
 * <b>Role:</b> This creates an <tt>AcisVersion</tt> object. When used with the <tt>AcisOptions</tt>
 * object, it allows the user to specify which algorithmic version of ACIS to
 * execute.  Algorithmic versioning allows you to guarantee the exact regeneration
 * of geometric and topological construction by calling APIs with specific version
 * information.
 * @see ALGORITHMIC_VERSION_BLOCK, GET_ALGORITHMIC_VERSION, AcisOptions
 */
class DECL_BASE AcisVersion : public ACIS_OBJECT
{
	friend DECL_BASE logical operator==(AcisVersion const& vt1, AcisVersion const& vt2);
	friend DECL_BASE logical operator!=(AcisVersion const& vt1, AcisVersion const& vt2);
	friend DECL_BASE logical operator>=(AcisVersion const& vt1, AcisVersion const& vt2);
	friend DECL_BASE logical operator<=(AcisVersion const& vt1, AcisVersion const& vt2);
	friend DECL_BASE logical operator<(AcisVersion const& vt1, AcisVersion const& vt2);
	friend DECL_BASE logical operator>(AcisVersion const& vt1, AcisVersion const& vt2);

	int	m_version_tag;

public:
	/**
	 * Default constructor.
	 */
	AcisVersion();

	/**
	 * Creates an <tt>AcisVersion</tt> object from the given input.
	 * <br><br>
	 * @param major
	 * major version.
	 * @param minor
	 * minor version.
	 * @param point
	 * point version.
	 */
	AcisVersion(int major, int minor, int point);

	/**
	 * Constructs an <tt>AcisVersion</tt> object from the given tag.
	 * <br><br>
	 * <b>Role:</b> The ACIS version tag is an integral value which meets the following
	 * criteria: for ACIS 6.3.1, the tag would equal 6*10000 + 3*100 + 1 = 60301.
	 * So if you provided the integral value 60301 to this constructor, you
	 * would be creating an <tt>AcisVersion</tt> object with the version set to 6.3.1.
	 * <br><br>
	 * @param tag
	 * version tag.
	 */
	AcisVersion(int tag);

	/**
	 * Returns the major version number.
	 */
	int acis_major();

	/**
	 * Returns the minor version number.
	 */
	int acis_minor();

	/**
	 * Returns the point version number.
	 */
	int acis_point();

	/**
	 * Returns the version tag.
	 * <br><br>
	 * <b>Role:</b> The ACIS version tag is an integral value which meets the following
	 * criteria: for ACIS 6.3.1, the tag would equal 6*10000 + 3*100 + 1 = 60301.
	 * So if you've constructed an <tt>AcisVersion</tt> object for ACIS 6.3.1, this method
	 * will return the value 60301.
	 */
	int tag();

	/**
	 * Operator equal.
	 * <br><br>
	 * <b>Role:</b> Use to assign one <tt>AcisVersion</tt> object to another.
	 * <br><br>
	 * @param in
	 * AcisVersion to copy.
	 */
	AcisVersion& operator=(const AcisVersion& in);
};


/**
 * Returns the <tt>AcisVersion</tt> for the current scope.
 * <br><br>
 * <b>Role:</b> This function can be used to obtain the AcisVersion for the
 * current scope. Since it is possible to use <tt>ALGORITHMIC_VERSION_BLOCK</tt> and
 * <tt>API_VERS_BEGIN</tt> to set the ACIS algorithmic version for large spans and
 * possibly even nested blocks of code, this can be used to test the ACIS version
 * for the current scope of execution.
 * <br><br>
 * For example:
 * <pre>
 * if ( GET_ALGORITHMIC_VERSION() &gt;= AcisVersion(22,0,0) )
 * //  execute code for ACIS R22 and later
 * else
 * //  execute pre-R22 code
 * </pre>
 */
DECL_BASE AcisVersion GET_ALGORITHMIC_VERSION(void);



// ********************************************************************
// Class:
//    acis_version_span
//
// Purpose:
//		A simple object that leverages ctor/dtor functionality to
//		give an exact timespan (lifetime) for setting a particular
//		Acis version.
// ********************************************************************
/**
 * @nodoc
 */
class DECL_BASE acis_version_span : public ACIS_OBJECT
{
	AcisVersion old_version;

public:
	acis_version_span();
	acis_version_span(const AcisVersion *av);
	~acis_version_span();
};


/**
 * Defines a block of code to be executed as a particular ACIS version.
 * <br>
 * <b>Role:</b> If the input is NULL, this macro has no effect.
 * @param vers
 * Pointer to an AcisVersion object which is set to the ACIS version to be executed.
 */
#define ALGORITHMIC_VERSION_BLOCK(vers) acis_version_span _avs(vers)


/**
 * @nodoc
 */
DECL_BASE logical is_valid_acis_save_version( int major, int minor );


/** @} */
#endif



