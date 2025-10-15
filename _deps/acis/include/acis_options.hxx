/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( ACIS_OPTIONS )
#define ACIS_OPTIONS

#include <stddef.h>
#include "logical.h"
#include "base.hxx"
#include "dcl_base.h"
#include "dcl_kern.h"
#include "acis_journal.hxx"
#include "acis_options_internal.hxx"
/**
 * \defgroup ACISAPIBEHAVIOR API Behavior
 * \ingroup KERNAPI
 *
 */
/**
 * @file acis_options.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISAPIBEHAVIOR
 *
 * @{
 */
class AcisVersion;
class outcome;

/**
 * Implements an <tt>AcisOptions</tt> class.
 * <br>
 * <b>Role:</b> The <tt>AcisOptions</tt> class is designed to act as a simple class that
 * holds options which are to be passed to every API function. The options include
 * information like Version and Journal.
 */

class DECL_KERN AcisOptions : public ACIS_OBJECT
{
public:

/**
 * C++ allocation constructor requests memory for this object but does not populate it.
 */

	AcisOptions();
/**
 * C++ copy constructor requests memory for this object and populates it with the data from the object supplied as an argument.
 * <br><br>
 * @param ao
 * Acis Options.
 */

	AcisOptions(const AcisOptions* ao);
/**
 * C++ allocation constructor requests memory for this object and populates the value of <tt>AcisJournal</tt> alone with the value of the passed argument.
 * <br><br>
 * @param journal
 * journal.
 */
AcisOptions(const AcisOptions& ao);



	AcisOptions(AcisJournal & journal);
/**
 * C++ allocation constructor requests memory for this object and populates the value of <tt>AcisJournal</tt> as well as <tt>AcisVersion</tt> with the corresponding values in the passed arguments.
 * <br><br>
 * @param journal
 * journal.
 * @param version
 * version.
 */

	AcisOptions(AcisJournal &journal, AcisVersion const &version);
/**
 * C++ allocation constructor requests memory for this object and populates the value of <tt>AcisVersion</tt> alone with the value of the passed argument.
 * <br><br>
 * @param version
 * version.
 */

	AcisOptions(AcisVersion const &version);
/**
 * C++ destructor for <tt>AcisOptions</tt> which deallocates memory.
 */

	virtual ~AcisOptions();
/**
 * This method sets the value of <tt>AcisJournal</tt> to the value of the passed <tt>AcisJournal</tt> object.
 * <br><br>
 * @param journalInfo
 * journal.
 */

	virtual void			set_journal(AcisJournal &journalInfo);
/**
 * This method returns the <tt>AcisJournal</tt> value.
 */

	virtual AcisJournal&	get_journal();
/**
 * This method sets the value of <tt>AcisVersion</tt> of the <tt>AcisOptions</tt> object to the value of the passed <tt>AcisVersion</tt> object.
 * <br><br>
 * @param version
 * version.
 */

	virtual void			set_version(AcisVersion const &version);
/**
 * This method returns the <tt>AcisVersion</tt> value.
 */

	virtual AcisVersion&	get_version();
/**
 * This method verifies if the existing <tt>AcisOptions</tt> object is the same as the object passed as the argument.
 * <br><br>
 * <b>Role:</b> In case, both of them are same, it returns the existing <tt>AcisOptions</tt> object.
 * If they are not the same, it replaces the existing <tt>AcisOptions</tt> object with the passed
 * object and returns the same.
 * <br><br>
 * @param ao
 * acis options.
 */

	virtual AcisOptions& operator=(const AcisOptions &ao );
/**
 * This method returns <tt>TRUE</tt> if the journal is enabled and the specified component is set to be journaled,  Otherwise it returns <tt>FALSE</tt>.
 */

	logical					journal_on();

private:
	AcisOptionsInternal* m_acis_options_internal;
};
// *********************************************************************************************
// Sets the Journal or Version data members to the AcisOptions Oject.
// These objects are true copied into the AcisOption Object
/**
 * Copies the <tt>AcisJournal</tt> and <tt>AcisVersion</tt> Objects from the arguments to the data members inside <tt>AcisOptions</tt>.
 * <br><br>
 * <b>Role:</b> Takes the arguments and copies them into the data members
 * contained in the AcisOptionsInternal data member.
 * <br><br>
 * <b>Limitations:</b> The version and journal objects are true copied, they are
 * independent from the ones contained in the <tt>AcisOptions</tt> object.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param ao
 * acis options.
 * @param aj
 * acis journal to be copied.
 * @param av
 * acis version to be copied.
 **/
DECL_KERN outcome api_set_acis_options(
									   AcisOptions* ao,
									   AcisJournal& aj = *(AcisJournal*)NULL_REF,
									   AcisVersion& av = *(AcisVersion*)NULL_REF);

/** @} */
/**
 * \addtogroup ACISVERSIONING
 * @{
 */
// *********************************************************************************************
//  Sets the Version Object into the AcisOption Object. A true is perfomed
/**
 * Copies the version object into the acis option.
 * <br><br>
 * <b>Role:</b> Makes a true copy of the version object into the <tt>AcisOptions</tt>
 * object.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
* <b>Journal: </b> Not Available
* <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param ao
 * acis options.
 * @param av
 * acis version to set.
 **/
DECL_KERN outcome api_set_version(AcisOptions* ao,	AcisVersion& av);

/** @} */
#endif

