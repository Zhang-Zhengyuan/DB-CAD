/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#if !defined( ACIS_JOURNAL )
#define ACIS_JOURNAL
#include "base.hxx"
#include "dcl_base.h"
#include "dcl_kern.h"
/**
 * \defgroup ACISJOURNALING Journaling
 * \ingroup ACISAPIBEHAVIOR
 *
 */
/**
 * @file acis_journal.hxx
 * @CAA2Level L1
 * @CAA2Usage U1
 * \addtogroup ACISJOURNALING
 *
 * @{
 */

class ENTITY;
class ENTITY_LIST;
class BODY;
class SPAvector;
class SPAposition;
class SPApar_box;
class law;
class AcisOptions;
class outcome;
class SPAtransf;
class LAW; 

/**
 * Contains data and methods related to journaling operation.
 * <br>
 * <b>Role:</b> <tt>AcisJournal</tt> object provides methods to get or set scheme and SAT
 * filenames for journaling. It also provides methods to start, end, pause and
 * resume journaling operation. However the direct use of the methods is discouraged
 * and the user is advised to use the equivalent API calls like <tt>api_set_journal_name</tt>,
 * <tt>api_start_journal</tt> etc. instead.
 */
class DECL_KERN AcisJournal : public ACIS_OBJECT
{
public:
/**
 * Default constructor.
 * <br><br>
 * <b>Role:</b> Instantiates an <tt>AcisJournal</tt> object with SAT and scheme file names set to ACISJour.
 */
	AcisJournal();
/**
 * Destructor releases the memory allocated for this object when this object goes out of scope.
 */
	virtual ~AcisJournal();
/**
 * Copy constructor.
 * <br><br>
 * <b>Role:</b> Instantiates an <tt>AcisJournal</tt> object and copies the values from the <tt>AcisJournal</tt>
 * object passed as an argument.
 * <br><br>
 * @param aj
 * journal.
 */
	AcisJournal(const AcisJournal &aj);
/**
 * This method is used to retrieve the scheme and SAT file name stored in this object.
 */
	const char*		get_file_name() const;
/**
 * This method is used to set the filename for scheme and sat files.
 * <br><br>
 * <b>Role:</b> The user is advised to use <tt>api_set_journal_name</tt> instead.
 * <br><br>
 * @param name
 * journal file name.
 */
	void			set_file_name(const char* name);
/**
 * This method is used to set the status of the journaling operation to on or off.
 * <br><br>
 * <b>Role:</b> A value of FALSE is used to pause the journaling operation. A value of
 * TRUE is used to resume the journaling operation that was paused. The user is advised
 * to use <tt>api_pause_journal</tt> and <tt>api_resume_journal</tt> instead.
 * <br><br>
 * @param st
 * journaling on or off.
 */

	void			set_status(logical st);
/**
 * This method returns the status of the journaling operation.
 * <br><br>
 * <b>Role:</b> A value of TRUE indicates that the journaling operation is on.
 * A value of FALSE indicates that the journaling operation is off.
 */

	logical			get_status() const;

/*
// tbrv
*/

/**
 * @nodoc
 */
	AcisJournal& operator=(const AcisJournal &);
/**
 * This method is used to start journaling operation.
 * <br><br>
 * <b>Role:</b> It opens the SAT and scheme files and adds the header information.
 * It stores the file handler for subsequent journaling by other APIs. The user is advised
 * to use <tt>api_start_journal</tt> instead.
 */
	void		 start_journaling();
/**
 * This method is used to end the journaling operation that was started using <tt>start_journaling</tt> method.
 * <br><br>
 * <b>Role:</b> It adds the scheme file footer information and closes the SAT and scheme files.
 * The user is advised to use <tt>api_end_journal</tt> instead.
 */
	void		 end_journaling();
/*
// tbrv
*/
/**
 * @nodoc
 */
	void		 start_api_journal(char* function_name, logical need_to_load_sat = TRUE);
/*
// tbrv
*/
/**
 * @nodoc
 */
	void		 resume_api_journal();
/*
// tbrv
*/
/**
 * @nodoc
 */
	void		 end_api_journal();

/**
 * @nodoc
 */
	const char* get_scm_file_name() const;

protected:
	int			get_scm_file_index();
	int			get_sat_file_index();
	const char* get_sat_file_name() const;
	FILE*		get_scm_file(){return m_scm_file;};
	FILE*		get_sat_file(){return m_sat_file;};
	void		set_sat_file_name();
	void		set_scm_file_name();
	void		set_scm_file_index();
	void		set_sat_file_index();
	void		set_save_number(int num);
	int			get_save_number();

//  Writes down the option_header options which are not set to the default value.
	void write_header_options();

// *****************************************************************************************************
//  Writes down a transform by defining the different translation components and composing the final
//  result.
//  Format:
//  (define name (transform:compose t1 t2))
//  (define name (transform:compose name t3))
// *****************************************************************************************************
	void write_transform(char* name, SPAtransf);

// *****************************************************************************************************
//  Writes down the definition of an acis_options
//  Format: (is version is different than latest)
//  (define ao (acis_options:set 'version (versiontag x y z)))
// *****************************************************************************************************
	void write_acis_options(AcisOptions*);

// *****************************************************************************************************
//  Writes down an acis_options definition in the scheme command context
//  Format: (is version is different than latest)
//  (acis_options:set 'version (versiontag x y z))
// *****************************************************************************************************
	char* write_acis_options_nd(AcisOptions*);

// Functions that save entities to SAT files ---------------------------------------------------------
//	void write_ENTITY_array_to_sat(int num_entities,ENTITY** entity_array,ENTITY_LIST &ent_list);

// *****************************************************************************************************
// Writes  down to a SAT file an ENTITY without increasing the saved entity counter and
	void write_ENTITY_to_sat (ENTITY* the_entity);

// *****************************************************************************************************
// Writes down an ENTITY_LIST to a SAT file without incrementing the saved entity counter
	void write_ENTITY_LIST_to_sat (const ENTITY_LIST& entity_list);

// *****************************************************************************************************
// Writes down a law to a SAT file. It does not increment the saved entity counter
	void write_LAW_to_sat(law* the_law);

// Function that save entities to SCM files ------------------------------------------------------------
// *****************************************************************************************************
//   Writes down an integer to the SCM file in the format
//   (define name xxx)
// *****************************************************************************************************
	void write_int_to_scm(char* name, int num);


// *****************************************************************************************************
//  Writes down a floating point number to the SCM file with the format
//  (define name XXX.XXXXXXXX)
// *****************************************************************************************************
	void write_float_to_scm(char* name, double num);

// *****************************************************************************************************
//  Writes down a SPAvector array to the SCM file in the form of a list
//  Format:
//  (define listname (list (gvector x1 y1 z1) (gvector x2 y2 y2) ... (gvector xn yx zn)))
// *****************************************************************************************************
	void write_vector_array_to_scm(int num_vectors, SPAvector* vectors, char* name);

// *****************************************************************************************************
//  Writes down a floating point number array to the SCM file in the form of a list
//  Format:
//  (define listname (list num1 num2 ... numN))
// *****************************************************************************************************
	void write_float_array_to_scm(int num_magnitudes, double* magnitudes, char* name);

// *****************************************************************************************************
//  Writes down the definition of a list of entities taken from a SAT file and increases the saved
//  entity counter.
//  Format:
//  (define listname (list-ref (part:entities) x) (list-ref (part:entities) y)... (list-ref (part:entities) n))
// *****************************************************************************************************
	void write_ENTITY_LIST_to_scm(const ENTITY_LIST& ent_list, char* name);

// *****************************************************************************************************
//  Writes down the definition or handling of an entity taken from a SAT file and increases the saved
//  entity counter.
//  Format:
//  (define name (list-ref (part:entities) x))
//  or
//  (list-ref (part:entities) x)
// *****************************************************************************************************
	void write_ENTITY_to_scm(char* name, logical define = TRUE);

// *****************************************************************************************************
//  Writes down a logical value to the SCM file
//  Format:
//  (define name #t) or (define name #f)
// *****************************************************************************************************
	void write_logical_to_scm(char* name, logical state);

// *****************************************************************************************************
//  Writes down a SPAvector to the SCM file
//  Format:
//  (define name (gvector x y z))
// *****************************************************************************************************
	void write_vector_to_scm(char* name, const SPAvector& the_vector);

// *****************************************************************************************************
//  Writes down a SPAposition to the SCM file
//  Format:
//  (define name (SPAposition x y z))
// *****************************************************************************************************
	void write_position_to_scm(char* name, const SPAposition& the_pos);

// *****************************************************************************************************
//  Writes down a SPAposition list to the SCM file
//  Format:
//  (define name (list (SPAposition x y z)....))
// *****************************************************************************************************
	void write_position_array_to_scm(char* name, int nop, SPAposition* the_pos_array);

// *****************************************************************************************************
//  Writes down a SPAposition list to the SCM file as a list of reals
//  Format:
//  (define name (list x1 y1 z1 x2 y2 z2 ....))
// *****************************************************************************************************
    void write_positions_as_float_array_to_scm(char* name, int nop, SPAposition* the_pos_array);

// *****************************************************************************************************
//  Writes down a par-box to the SCM file with the format
//  (define name (cons (par u-low u-high) (par v-low v-high) ) )
// *****************************************************************************************************
	void write_par_box_to_scm( char const * name, SPApar_box const& pbx );

// *****************************************************************************************************
// Writes down an Entity to the Scm file. The ENTITY can be of any kind, Since the owner of an ENTITY
// is what is saved to a SAT file (if is a FACE which belongs to a BODY, the BODY is the one saved),
// it is needed to define it from its owner.
// The owner is saved and the saved entity counter is increased.
// Format:
// (define entityOwner (list-ref (part:entities) x))
// (define name (list-ref (entity:(faces,vertices,...) y)
//
// if Entity is a Body
// (define name (list-ref (part:entities) x))
//
	void write_ENTITY(char* name, ENTITY* theEntity);

// *****************************************************************************************************
//  Writes down an ENTITY_LIST to the journal file and saves those entities to the SAT file, the
//  saved counter is increased.
//  If noclasify is set to TRUE it saves without caring of repeated or type entities.
//  Format:
//  Format:
//  (define listname (list-ref (part:entities) x) (list-ref (part:entities) y)... (list-ref (part:entities) n))
//
//  If clasify is on, checks for common owners and definition from them.
//  Format:
//  (define listname (list-ref (list
//                               (list-ref (entity:type (list-ref (part:entities) x) y)
//								 ....
//								 (list-ref (entity:type (list-ref (part:entities) x) y)  )
// *****************************************************************************************************
	void write_ENTITY_LIST(char* name, const ENTITY_LIST& entity_list, logical noclasify=FALSE);


// *****************************************************************************************************
//  Writes down an ENTITY_LIST to the journal file and saves those entities to the SAT file, the
//  saved counter is increased.
//  If noclasify is set to TRUE it saves without caring of repeated or type entities.
//	If checkforduplicates is set to TRUE, it removes duplicates from the Original Entity List
//  Format:
//  Format:
//  (define listname (list-ref (part:entities) x) (list-ref (part:entities) y)... (list-ref (part:entities) n))
//
//  If clasify is on, checks for common owners and definition from them.
//  Format:
//  (define listname (list-ref (list
//                               (list-ref (entity:type (list-ref (part:entities) x) y)
//								 ....
//								 (list-ref (entity:type (list-ref (part:entities) x) y)  )
// *****************************************************************************************************
	void write_ENTITY_LIST(char* name, const ENTITY_LIST& ORIG_entity_list, logical noclasify, logical checkforduplicates);

// *****************************************************************************************************
//  Writes down a law to the journal file and saves it to the SAT file, the
//  saved counter is increased.
//  Format:
//  (define name (list-ref (part:entities) x))
//  ; String definition 						<NOTE that the string evaluation may not be correct
//  or
//  (list-ref (part:entities x))
//  ; String definition 						<NOTE that the string evaluation may not be correct
	void write_LAW(char* name, law* theLaw, logical define = TRUE);

// *****************************************************************************************************
//  Writes down an ENTITY array to the journal file. Saves the entities to the SAT file and the
//  saved counter is increased.
//  Format:
//  (define listname (list-ref (part:entities) x) (list-ref (part:entities) y)... (list-ref (part:entities) n))
	void write_ENTITY_array(char* name, int num_entities,ENTITY** entity_array,ENTITY_LIST &ent_list);

// *****************************************************************************************************
//  Writes down a LAW array to the journal file. Saves the laws to the SAT file and the
//  saved counter is increased.
//  Format:
//  (define listname (list-ref (part:entities) x) (list-ref (part:entities) y)... (list-ref (part:entities) n))
	void write_LAW_array(char* name, int num_laws,LAW** law_array,ENTITY_LIST &law_list);

// *****************************************************************************************************
//  Writes down the API Header. Writes down the name specified (should be the api being called)
//  and loads the SAT file which contains the entities to be input into the Scheme extension that
//  will be defined below.
//
//	knt 05Nov03.	Added optional argument, need_to_load_sat, because cstr api's like api_solid_cylinder_cone
//					simply create an analytic from scratch - there is no need to create a sat file in these
//					api's, hence, no need to load it.
// *****************************************************************************************************
	void write_api_header(char* name, logical need_to_load_sat = TRUE);
	void write_api_footer();

// *****************************************************************************************************
//  Writes down the Journal Header. Sets up an OpenGL window and convenient options for
//  better visualization
// *****************************************************************************************************
	void write_journal_header();
	void write_journal_footer();
	void generate_scm_file_name();
	void generate_sat_file_name();

	//  Data Members
	char*         m_file_name;
	logical       m_status;					 //  Journaling on/off
	int			  m_sat_file_index;
	int			  m_scm_file_index;
	int			  m_saved_entity_number;
	FILE*	      m_scm_file;
	FILE*		  m_sat_file;
	char*         m_sat_file_name;
	char*         m_scm_file_name;
private:
	void		init(const AcisJournal &rhs);
	char        m_avstr[200];

};
// *****************************************************************************************************
// * Gets a reference to the journal inside AcisOptions
// *****************************************************************************************************
/**
* Gets a reference to the <tt>AcisJournal</tt> contained in <tt>AcisOptions</tt>.
* <br><br>
* <b>Role:</b> Gets a reference to the <tt>AcisJournal</tt> object contained in the
* AcisOptionsInternal data member.
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
* output reference to acis journal.
**/

DECL_KERN outcome api_get_journal( AcisOptions* ao,  AcisJournal* &aj );

// *****************************************************************************************************
// * Starts the Journaling. Sets the status flag on, generates a scheme script file name and writes down
// * the header.
// *****************************************************************************************************
/**
* Sets the status flag for journalizing to on and initializes journal.
* <br><br>
* <b>Role:</b> Sets the status flag on to enable journalizing. It creates the
* journal file and writes down the header. It also generates the SAT file name and
* sets the file name counters properly.
* <br><br>
* <b>Errors:</b> Starting again an already enabled journal may cause erasing previously
* saved files or leaving incomplete journaled files.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param ao
* acis options such as journal, version.
**/
DECL_KERN outcome api_start_journal(AcisOptions* ao);

// *****************************************************************************************************
// * Ends the Journaling. Sets the status flag off, writes down the footer.
// *****************************************************************************************************
/**
* Sets the status flag off journalizing and finishes the snapshot journaling mechanism.
* <br><br>
* <b>Role:</b> Sets the status flag to off and writes down the script footer.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param ao
* acis options such as version, journal.
**/
DECL_KERN outcome api_end_journal(AcisOptions* ao);
/**
* Sets the status flag for journalizing to off, disabling the snapshot journal mechanism.
* <br><br>
* <b>Role:</b> Sets the status flag to off to disable journalizing.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param ao
* acis options.
**/
DECL_KERN outcome api_pause_journal(AcisOptions* ao);
/**
* Sets the status flag for journalizing to on, enabling the snapshot journal mechanism.
* <br><br>
* <b>Role:</b> Sets the status flag to on to enable journalizing.
* <br><br>
* <b>Errors:</b> If this is used before start. The header will not be written and some
* functions will fail to journalize.
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param ao
* acis options.
**/
DECL_KERN outcome api_resume_journal(AcisOptions* ao);

// *****************************************************************************************************
// * Makes a true copy of the journal object to the acisoptions object journal member
// *****************************************************************************************************
/**
* Copies the <tt>AcisJournal</tt> object to <tt>AcisOptions</tt>.
* <br><br>
* <b>Role:</b> Takes the <tt>AcisJournal</tt> object and copies it into the data member
* contained in the AcisOptionsInternal data member.
* <br><br>
* <b>Limitations:</b> The journals object is true copied, it is independent from the
* one contained in the <tt>AcisOptions</tt> object.
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
**/
DECL_KERN outcome api_set_journal( AcisOptions* ao,  AcisJournal &aj);

// *****************************************************************************************************
// * Sets the journal file name
// *****************************************************************************************************
/**
* Sets the snapshot journal file name.
* <br><br>
* <b>Role:</b> Sets the file name for the snapshot journal. The extension name
* is not needed.
* <br><br>
* <b>Limitations:</b> If a name other than <tt>AcisJour</tt> is set, then the new name would be
* used always.  In this case, instead of serializing the output name (AcisJour_x,
* scm, x = 0...n), it would create always the same file (e.g. My_name.scm). This
* can be really useful because it will not create a large number of files if the
* journal is implemented in a function that is called many times and it is desired
* to keep only the last call (e.g. when an error occurs).
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param ao
* acis options.
* @param name
* journal file name.
**/
DECL_KERN outcome api_set_journal_name(AcisOptions* ao, const char* name);
/**
* Sets the snapshot journal file name.
* <br><br>
* <b>Role:</b> Sets the file name for the snapshot journal. The extension name
* is not needed.
* <br><br>
* <b>Limitations:</b> If a name other than <tt>AcisJour</tt> is set, then the new name would be
* used always.  In this case, instead of serializing the output name (AcisJour_x,
* scm, x = 0...n), it would create always the same file (e.g. My_name.scm). This
* can be really useful because it will not create a large number of files if the
* journal is implemented in a function that is called many times and it is desired
* to keep only the last call (e.g. when an error occurs).
* <br><br>
* <b>Effect:</b> System routine
* <br><br>
* <b>Journal: </b> Not Available
* <br><br>
* <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param aj
* acis journal.
* @param name
* journal file name.
**/
DECL_KERN outcome api_set_journal_name(AcisJournal* aj, const char* name);


/** @} */
#endif

