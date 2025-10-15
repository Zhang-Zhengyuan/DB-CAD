/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// Header for Kernel api routines.
/*******************************************************************/
#if !defined( KERNAPI_HXX )
#define KERNAPI_HXX
#include "acis.hxx"
#include "api.hxx"
#include "bs3surf.hxx"
#include "dcl_kern.h"
#include "logical.h"
#include "pattern_enum.hxx"
#include "ptfcenum.hxx"
#include "bullsmal.hxx"
class AcisVersion;
class BODY;
class BULLETIN_BOARD;
class COEDGE;
class curve;
class DELTA_STATE;
class DELTA_STATE_LIST;
class EDGE;
class ENTITY;
class ENTITY_LIST;
class FACE;
class FileInterface;
class HISTORY_STREAM;
class HISTORY_STREAM_LIST;
class LOOP;
class outcome;
class SPAinterval;
class SPAposition;
class SPAtransf;
class SPAunit_vector;
class SPAvector;
class StreamFinder;
class surface;
class TCOEDGE;
class TEDGE;
class TRANSFORM;
class TVERTEX;
class VERTEX;
class WCS;
class SPApar_box;
class SPAboxing_options;
class SPACOLLECTION;
class tolerize_entity_opts;
class FileInfo;
/*******************************************************************/
/**
 * \defgroup KERNAPI Kernel Functionality
 *
 */

 /**
  * @file kernapi.hxx
  * @CAA2Level L1
  * @CAA2Usage U1
  * \addtogroup KERNAPI ACIS Kernel
  *
  * @{
  */

  /**
   * Identifier for a <tt>ENTITY</tt>.
   * <br>
   * <b>Role:</b> Unique identifier for a given <tt>ENTITY</tt>.
   */
typedef int tag_id_type;
/**
 * Identifier for a <tt>STATE</tt>.
 * <br>
 * <b>Role:</b> Unique identifier for a given <tt>STATE</tt>.
 */
typedef int STATE_ID;
/**
 * @nodoc
 */
typedef int ENTITY_ID;
/**
 * Initializes the kernel library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
**/
DECL_KERN outcome api_initialize_kernel();
/**
 * Terminates the kernel library.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
**/
DECL_KERN outcome api_terminate_kernel();

/** @} */

/**
 * \addtogroup ACISERRORMANAGEMENT
 * @{
 */

 /**
  * Sets the pointer argument checking for an API call to on or off.
  * <br><br>
  * <b>Role:</b> With argument checking on, pointer arguments to an API are
  * tested to determine whether they are <tt>NULL</tt>. If they are <tt>NULL</tt>, a message is
  * printed and the API returns an <tt>outcome</tt> with a nonzero error code.
  * <br><br>
  * Checks are also made on certain distances and angles supplied to APIs. Some APIs
  * make more extensive checks internally, but the effect is the same. When there is
  * an <tt>on</tt> error a message prints and the API returns an <tt>outcome</tt> with a nonzero error
  * code.
  * <br><br>
  * <b>Effect:</b> System routine
  * <br><br>
  * <b>Journal: </b> Not Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
  * <br><br>
 * @param on_off
  * <tt>TRUE</tt> for on.
  **/
DECL_KERN outcome api_checking(
    logical on_off);

/** @} */

/**
 * \addtogroup ACISHISTORY
 * @{
 */

 /**
  * Sets logging of entity modifications on or off for roll back purposes.
  * <br><br>
  * <b>Role:</b> <tt>TRUE</tt> enables logging of data structure changes.
  * <br><br>
  * Logging is turned on by default so applications can roll to
  * previous states.
  * <br><br>
  * If <tt>api_logging</tt> is set to <tt>FALSE</tt>, memory use is reduced as
  * bulletins are created only temporarily for roll of failed APIs.
  * At the end of a failed API, the bulletin board is rolled and
  * and deleted so the model is restored to it's previous non-corrupted
  * state.  If the API is successful and api logging is <tt>FALSE</tt>, the bulletin
  * board is simply deleted.  This reduces memory usage, but the
  * application will not be able to roll to previous states as all history
  * is lost.  This API sets the option "logging" to the appropriate value.
  * <br><br>
  * <b>Effect:</b> System routine
  * <br><br>
  * <b>Journal: </b> Not Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
  * <br><br>
 * @param on_off
  * <tt>TRUE</tt> for on.
  **/
DECL_KERN outcome api_logging(
    logical on_off
);

/**
 * Sets the @href HISTORY_STREAM specific API logging behavior.
 * <br><br>
 * <b>Role:</b> API logging, which controls the lifetime of logged entity modifications,
 * is normally established globally with @href api_logging.
 * This behavior can also be controlled for a particular history stream, which has
 * precedence over the global setting. Enabling API logging for a specific stream in other words,
 * enables the accumulation of logged entity modifications in <tt>DELTA_STATEs</tt> for that stream,
 * irrespective of the global setting.
 * The default @href HISTORY_STREAM API logging value is "unset", which causes the global setting to be used.
 * The default stream is chosen if no stream is specified.
 * Distributed history must be enabled for stream-specific logging to have any affect.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param sl
 * stream_logging value.
 * @param hs
 * <tt>HISTORY_STREAM</tt> to use, default is the current stream.
 * <br><br>
 * @see api_get_stream_logging, api_logging, stream_logging
 **/
DECL_KERN outcome api_set_stream_logging(stream_logging sl, HISTORY_STREAM* hs = NULL);

/**
 * Returns the @href HISTORY_STREAM specific API logging setting.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param sl
 * the current stream_logging value.
 * @param hs
 * HISTORY_STREAM to use, default is current stream.
 * <br><br>
 * @see api_set_stream_logging, api_logging, stream_logging
 **/
DECL_KERN outcome api_get_stream_logging(stream_logging& sl, HISTORY_STREAM* hs = NULL);
/** @} */

/**
 * \addtogroup KERNAPI
 * @{
 */

 /**
  * Starts the modeler.
  * <br><br>
  * <b>Role:</b> This API starts the modeler. This API must be called before calling any
  * other ACIS API, function, or method, with one exception: <tt>initialize_base</tt>. If you
  * need to configure the ACIS base component to meet your specific application needs, you
  * must call <tt>initialize_base</tt> prior to calling <tt>api_start_modeller</tt>. (Calling
  * <tt>initialize_base</tt> is optional.)
  * <br><br>
  * Care should be taken to call this API only once; this API is not use-counted and implicit
  * calls to internal initialization routines could adversely affect applications if called
  * more than once.
  * <br><br>
  * ACIS does not currently support stopping and restarting the modeler. However, it is
  * possible for applications to dynamically unload and reload the ACIS dll's, which would
  * subsequently require the app to call this API again.
  * <br><br>
  * The integer argument to this API is not used.
  * <br><br>
  * <b>Effect:</b> System routine
  * <br><br>
  * <b>Journal: </b> Not Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
  * <br><br>
 * @param n
  * not used.
  * <br><br>
  * @see api_stop_modeller, is_modeler_started, initialize_base, terminate_base
  */
DECL_KERN outcome api_start_modeller(
    int n = 0
);

/**
 * Stops the modeler.
 * <br><br>
 * <b>Role:</b> This API stops the modeler. After calling this API you should not call any
 * other ACIS API, function, or method, with one exception: <tt>terminate_base</tt>. You
 * would only need to call <tt>terminate_base</tt> after <tt>api_stop_modeller</tt> if you
 * had previously called <tt>initialize_base</tt> prior to calling <tt>api_start_modeller</tt>.
 * <br><br>
 * ACIS does not currently support stopping and restarting the modeler, except when done in a DLL.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @see api_start_modeller, is_modeler_started, initialize_base, terminate_base
 */
DECL_KERN outcome api_stop_modeller();

/**
 * Returns <tt>TRUE</tt> if the modeler is already started, otherwise <tt>FALSE</tt>.
 * <br><br>
 * @see api_start_modeller, api_stop_modeller
 */
DECL_KERN logical is_modeler_started();
/** @} */

/**
 * \addtogroup ACISOPTIONS
 *
 * @{
 */

 /**
  * Sets the value of the specified option to the given integer.
  * <br><br>
  * <b>Role:</b> This API sets the named option to the specified value. Refer to
  * the <tt>option:list</tt> Scheme extension at the toolkit for a list of the available options.
  * <br><br>
  * <b>Errors:</b> <tt>NULL</tt> or unknown option name specified.
  * <br><br>
  * <b>Effect:</b> System routine
   * <br><br>
  * <b>Journal: </b> Not Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
  * <br><br>
 * @param name
  * name of option.
  * @param value
  * integer or logical value to set.
  * <br><br>
  * @see option_header
  **/
DECL_KERN outcome api_set_int_option(
    char const* name,
    int			value
);

DECL_KERN outcome gme_api_set_int_option(
    char const* name,
    int			value
);

/**
 * Sets the	 value of the specified option to the given double.
 * <br><br>
 * <b>Role:</b> This API sets the named option to the specified value. Refer to
 * the <tt>option:list</tt> Scheme extension for a list of the available options.
 * <br><br>
 * <b>Errors:</b> <tt>NULL</tt> or unknown option name specified.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param name
 * name of option.
 * @param value
 * double value to set.
 **/
DECL_KERN outcome api_set_dbl_option(
    char const* name,
    double		value
);

DECL_KERN outcome gme_api_set_dbl_option(
    char const* name,
    double		value
);

/**
 * Sets the value of the specified option to the given string.
 * <br><br>
 * <b>Role:</b> This API sets the named option to the specified value. Refer to
 * the <tt>option:list</tt> scheme extension at the toolkit for a list of the available options.
 * <br><br>
 * <b>Errors:</b> <tt>NULL</tt> or unknown option name specified.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param name
 * name of option.
 * @param value
 * string value to set.
 * <br><br>
 * @see option_header
 **/
DECL_KERN outcome api_set_str_option(
    char const* name,
    char const* value
);

DECL_KERN outcome gme_api_set_str_option(
    char const* name,
    char const* value
);

/** @} */

/**
 * \addtogroup ACISHISTORY
 * @{
 */

 /**
  * Merges a <tt>DELTA_STATE</tt> instance into a <tt>HISTORY_STREAM</tt>.
  * <br><br>
  * <b>Role:</b> This API grafts a <tt>DELTA_STATE</tt> into a <tt>HISTORY_STREAM</tt>
  * following the active <tt>DELTA_STATE</tt> of the stream. This is used to in
  * conjunction with <tt>api_note_state</tt> and <tt>api_remove_state</tt> to build
  * multiple independent history streams. After noting a state, it can be moved
  * to an alternate stream by first removing it from the default stream with
  * <tt>api_remove_state</tt>, and then adding it to the stream it is to become a part of.
  * <br><br>
  * <b>Errors:</b> Either input pointer is <tt>NULL</tt>.
  * <br><br>
  * <b>Effect:</b> System routine
  * <br><br>
  * <b>Journal: </b> Not Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
  * <br><br>
 * @param ds
  * state to add.
  * @param hs
  * stream to add.
  * <br><br>
  * @see api_note_state, api_remove_state.
  **/
DECL_KERN outcome api_add_state(
    DELTA_STATE* ds,
    HISTORY_STREAM* hs
);
/**
 * Modifies the modeler state by applying a delta state.
 * <br><br>
 * <b>Role:</b> This API modifies the modeler's state to a different state using
 * the given delta state. For example, the delta state carries the modeler from
 * state <i>A</i> to state <i>B</i> and is applied only when the modeler is in state <i>A</i>.
 * <br><br>
 * <b>Errors:</b> The pointer to <tt>ds</tt> is <tt>NULL</tt>,
 * or the current state has not been noted.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param ds
 * delta state to be applied.
 **/
DECL_KERN outcome api_change_state(
    DELTA_STATE* ds
);

/**
 * Modifies a history stream by merging a range of delta states.
 * <br><br>
 * <b>Role:</b> This API merges the delta states contained in a range specified
 * by the user through the arguments <tt>ds1</tt> and <tt>ds2</tt>. If one of these arguments is
 * given as <tt>NULL</tt>, the specified state is merged with its next state. If both are
 * given as <tt>NULL</tt>, the active delta state is merged with its predecessor. The user
 * may specify the relevant history stream by furnishing the argument <tt>hs</tt>. Otherwise,
 * it is taken from <tt>ds1</tt> or <tt>ds2</tt>, if they are given, or set to the default stream,
 * if they are not. By default, the function fails if the range contains states
 * having partner states, but if the flag <tt>prune_partners</tt> is set to <tt>TRUE</tt>, the
 * function will prune the branches associated with these partners. If the
 * <tt>keep_both</tt> flag is <tt>TRUE</tt>, the merge happens between the given states so neither is
 * deleted. The <tt>keep_both</tt> flag has no effect unless two non-<tt>NULL</tt> states are given.
 * <br><br>
 * <b>Errors:</b> The delta states referenced by <tt>ds1</tt> and <tt>ds2</tt> do not belong to the same
 * stream, they do not belong to the specified stream, or partner states were
 * encountered with <tt>prune_partners</tt> set to <tt>FALSE</tt>.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param ds1
 * state defining one end of range to be merged.
 * @param ds2
 * other end of range to be merged.
 * @param hs
 * history stream  containing states.
 * @param prune_partners
 * flag to allow pruning of partner states.
 * @param keep_both
 * flag to indicate both states are to be kept.
 **/
DECL_KERN outcome api_merge_states(
    DELTA_STATE* ds1 = NULL,
    DELTA_STATE* ds2 = NULL,
    HISTORY_STREAM* hs = NULL,
    logical        prune_partners = FALSE,
    logical        keep_both = FALSE
);

/**
 * Deletes a delta state and dependent data.
 * <br><br>
 * <b>Role:</b> This API deletes a <tt>DELTA_STATE</tt>; i.e., the recorded information
 * that enables the modeler to change between two particular states.
 * <br><br>
 * <b>Errors:</b> <tt>NULL</tt> pointer to delta state.
 * <br><br>
 * <b>Limitations:</b> Delta states should be deleted starting with those furthest away
 * and working toward the current state to ensure that delete bulletins (and rolled
 * back create bulletins) are deleted last.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param ds
 * delta state to be deleted.
 **/
DECL_KERN outcome api_delete_ds(
    DELTA_STATE* ds
);

/**
 * Sets a check point for roll back and returns model differences since previous call to <tt>api_note_state</tt>.
 * <br><br>
 * <b>Role:</b> This API notes the current state of the model and returns a
 * pointer to a <tt>DELTA_STATE</tt> that contains differential model data covering the
 * period since the previous call to <tt>api_note_state</tt>.
 * <br><br>
 * If there have been no model changes since the last call to <tt>api_note_state</tt> the
 * returned <tt>DELTA_STATE*</tt> will be <tt>NULL</tt>.
 * <br><br>
 * The default <tt>HISTORY_STREAM</tt> is used, unless a different history stream is
 * supplied.
 * <br><br>
 * If the logical <tt>delete_if_empty</tt> is <tt>TRUE</tt>, an empty <tt>DELTA_STATE</tt> (i.e. one with no
 * bulletins) will be removed from the stream when noted.
 * <br><br>
 * To return the model to the previous state from the current state, call
 * <tt>api_change_state</tt> with the <tt>DELTA_STATE</tt> as an argument.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param ds
 * state change returned.
 * @param hs
 * history stream to check.
 * @param delete_if_empty
 * flag to delete ds if empty.
 * <br><br>
 * @see api_change_state, api_add_state, api_remove_state
 **/
DECL_KERN outcome api_note_state(
    DELTA_STATE*& ds,
    HISTORY_STREAM* hs = NULL,
    logical         delete_if_empty = FALSE
);
/** @} */

/**
 * \addtogroup ACISSAVERESTORE
 * @{
 */

 /**
  * Gets header info from the last restored file.
  * <br><br>
  * <b>Role:</b> The API fills in a <tt>FileInfo</tt> class with the header information
  * from the last restored file. It does not alter the model.
  * <br><br>
  * The @href FileInfo class contains the following information:
  * <ul>
  * <li><tt>product_id</tt> is a string indicating the product and version which produced the
  * save file.</li>
  * <li><tt>date</tt> is a string indicating the date the model was saved
  * (for example, "Fri Feb 9 16:49:43 MST 1996").</li>
  * <li><tt>units</tt> is a double indicating the modeling units. Modeling units are specified as millimeters per unit.</li>
  * <li><tt>acis_version</tt> is a string indicating the version of the ACIS libraries used in
  * the product which produced the save file.</li>
  * <li><tt>file_version</tt> is the ACIS save file version for which the model was saved
  * (for example, 200).</li>
  * <li><tt>SPAresabs</tt> is the distance tolerance in effect when the model was saved.</li>
  * <li><tt>SPAresnor</tt> is the normal tolerance in effect when the model was saved.</li>
  * </ul>
  * <br><br>
  * <b>Effect:</b> Read-only
  * <br><br>
  * <b>Journal: </b> Not Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
  * <br><br>
 * @param info
  * file information returned.
  **/
DECL_KERN outcome api_get_file_info(FileInfo& info);

/**
 * Sets required header info to be written to ACIS save files.
 * <br><br>
 * <b>Role:</b> The API sets the information to be written to the
 * header of later saved files. Does not alter the model. Beginning
 * with ACIS release 6.3, it is <b>required</b> that the product ID and units
 * to be populated for the file header
 * before you can save a SAT file.
 * <br><br>
 * The mask value indicates which values in the supplied <tt>FileInfo</tt>
 * structure are to be set. It is composed by "OR-ing" together mask
 * values as indicated below.
 * <br><br>
 * The <tt>FileInfo</tt> structure contains the following fields which can be
 * set:
 * <br><br>
 * <dl>
 * <dt><tt>product_id</tt></dt>
 * <dd>Mask = FileIdent<br>
 * String indicating the product and version that produced the save file.</dd>
 * <br><br>
 * <dt><tt>units</tt></dt>
 * <dd>Mask = FileUnits<br>
 * Modeling units specified as millimeters per unit.</dd>
 * <br><br>
 * <dt><tt>units</tt> values for common modeling units are:</dt>
 * <br>
 * <dd>
 * -1.0 = Units not specified
 * <br>
 * 1.0 = Millimeters
 * <br>
 * 10.0 = Centimeters
 * <br>
 * 1000.0 = Meters
 * <br>
 * 1000000.0 = Kilometers
 * <br>
 * 25.4 = Inches
 * <br>
 * 304.8 = Feet
 * <br>
 * 914.4 = Yards
 * <br>
 * 1609344.0 = Miles
 * </dd>
 * </dl><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param mask
 * mask indicating fields to set.
 * @param info
 * info to be set.
 **/
DECL_KERN outcome api_set_file_info(
    unsigned long mask,
    const FileInfo& info
);

/**
 * Restores entities from file.
 * <br><br>
 * <b>Role:</b> This function restores the entities from the file
 * stream specified by the file pointer argument to the supplied
 * <tt>ENTITY_LIST</tt>. The entities are expected to be in text format (SAT)
 * if the <tt>text_mode</tt> argument is <tt>TRUE</tt>; otherwise they are expected to
 * be in binary format (SAB). The file pointer should be positioned
 * at the point where the reading should begin. When the restore is
 * complete, the file pointer will be correctly positioned to the end
 * of the restored data. The file pointer is repositioned to the
 * original reading position in the event of an error.
 * <br><br>
 * A warning is given when custom entities that are unknown to the
 * system are encountered. These records are restored in entirety as
 * "unknown" entities, but are largely ignored by ACIS operations.
 * Unrecognized descendent entities of <tt>ATTRIB</tt>, <tt>SURFACE</tt>, or <tt>CURVE</tt>
 * generate a new record for their immediate owner class and references
 * to them become references to the new record. If a record for a
 * derived class of <tt>ATTRIB</tt> is not recognized, at the least an <tt>ATTRIB</tt>
 * record will result so that the chain of attributes for the entity
 * owning the unrecognized attribute remains connected.
 * <br><br>
 * A warning is also given if the version of the product receiving
 * the model is newer than the version that made the save file. It is
 * an error if the current product is older than that recorded in the file.
 * <br><br>
 * See the <i>[SAT Save and Restore](http://doc.spatial.com/articles/s/a/t/SAT_Save_and_Restore_981d.html)</i> Technical Article for more information.
 * <br><br>
 * <b>Errors:</b> Version number of this system is older than the version that
 * made the save file being restored. Malformed save file.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
 * @param file_ptr
 * open file descriptor.
 * @param text_mode
 * <tt>TRUE</tt> if file is text, <tt>FALSE</tt> if binary.
 * Note: the FILE* file_ptr must have been opened in binary mode if the file is binary - i.e., with the extension sab.
 * @param entities
 * returns restored top-level entities.
 * @param ao
 * ACIS options.
 * @see api_set_file_info, api_save_entity_list, api_save_entity_list_file,
 * api_save_entity_list_with_history, api_save_entity_list_with_history_file,
 * api_save_history, api_save_history_file, api_restore_entity_list,
 * api_restore_entity_list_file, api_restore_entity_list_with_history,
 * api_restore_entity_list_with_history_file, api_restore_history, api_restore_history_file
 **/
DECL_KERN outcome api_restore_entity_list(
    FILE* file_ptr,
    logical      text_mode,
    ENTITY_LIST& entities,
    AcisOptions* ao = NULL);

/**
 * Writes entities to a file in text or binary format.
 * <br><br>
 * <b>Role:</b> This function saves the entities contained in the
 * supplied <tt>ENTITY_LIST</tt> to the file stream specified by the file
 * pointer argument. The entities are stored in text format (SAT)
 * if the text mode argument is true, otherwise they are stored in
 * binary format (SAB). The file pointer should be an open file
 * positioned at the point where the writing should begin.
 * When the save is complete, the file pointer will be correctly
 * positioned to the end of the saved data. The file pointer is
 * repositioned to the original writing position in the event of an error.
 * <br><br>
 * See the <i>[SAT Save and Restore](http://doc.spatial.com/articles/s/a/t/SAT_Save_and_Restore_981d.html)</i> Technical Article for more information.
 * <br><br>
 * <b>Note:</b> Beginning with ACIS release 6.3, it is <b>required</b> that the product ID
 * and units to be populated for the file header (using class <tt>FileInfo</tt>)
 * before you can save a SAT file. Refer to the reference templates for
 * the class @href FileInfo and function @href api_set_file_info for more
 * information. The Technical Article <i>[Save and Restore Examples](http://doc.spatial.com/articles/s/a/v/Save_and_Restore_Examples_39be.html)</i> may also be useful.
 * <br><br>
 * <b>Errors:</b> Failed to save entities; e.g., unable to write disk file.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
 * @param file_ptr
 * open file descriptor.
 * @param text_mode
 * <tt>TRUE</tt> if file is text, <tt>FALSE</tt> if binary.
 * @param entity_list_save
 * list of entities to save.
 * @param ao
 * ACIS options.
 * <br><br>
 * @see api_set_file_info, api_save_entity_list, api_save_entity_list_file,
 * api_save_entity_list_with_history, api_save_entity_list_with_history_file,
 * api_save_history, api_save_history_file, api_restore_entity_list,
 * api_restore_entity_list_file, api_restore_entity_list_with_history,
 * api_restore_entity_list_with_history_file, api_restore_history, api_restore_history_file
 **/
DECL_KERN outcome api_save_entity_list(
    FILE* file_ptr,
    logical           text_mode,
    ENTITY_LIST const& entity_list_save,
    AcisOptions* ao = NULL);

/**
 * Restores entities from a custom input target.
 * <br><br>
 * <b>Role:</b> This function restores the entities from the custom
 * input target specified by the @href FileInterface argument to the supplied
 * <tt>ENTITY_LIST</tt>. The reading location in the input target should be
 * positioned at the point where the reading should begin. When the
 * restore is complete, the reading location will be correctly positioned
 * to the end of the restored data. The reading location is repositioned
 * to the original reading position in the event of an error.
 * <br><br>
 * A warning is given when custom entities that are unknown to the
 * system are encountered. These records are restored in entirety as
 * "unknown" entities, but are largely ignored by ACIS operations.
 * Unrecognized descendent entities of <tt>ATTRIB</tt>, <tt>SURFACE</tt>, or <tt>CURVE</tt>
 * generate a new record for their immediate owner class and references
 * to them become references to the new record. If a record for a
 * derived class of <tt>ATTRIB</tt> is not recognized, at the least an <tt>ATTRIB</tt>
 * record will result so that the chain of attributes for the entity
 * owning the unrecognized attribute remains connected.
 * <br><br>
 * A warning is also given if the version of the product receiving
 * the model is newer than the version that made the save file. It is
 * an error if the current product is older than that recorded in the file.
 * <br><br>
 * See the <i>[SAT Save and Restore](http://doc.spatial.com/articles/s/a/t/SAT_Save_and_Restore_981d.html)</i> Technical Article for more information.
 * <br><br>
 * <b>Errors:</b> Version number of this system is older than the version that
 * made the save file being restored. Malformed save file.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
* @param file_ptr
 * pointer to custom FileInterface object.
 * @param entities
 * returns restored top-level entities.
 * @param ao
 * ACIS options.
 * @see api_set_file_info, api_save_entity_list, api_save_entity_list_file,
 * api_save_entity_list_with_history, api_save_entity_list_with_history_file,
 * api_save_history, api_save_history_file, api_restore_entity_list,
 * api_restore_entity_list_file, api_restore_entity_list_with_history,
 * api_restore_entity_list_with_history_file, api_restore_history, api_restore_history_file
 **/
DECL_KERN outcome api_restore_entity_list_file(
    FileInterface* file_ptr,
    ENTITY_LIST& entities,
    AcisOptions* ao = NULL);

/**
 * Writes entities to a custom output target.
 * <br><br>
 * <b>Role:</b> This function saves the entities contained in the
 * supplied <tt>ENTITY_LIST</tt> to the custom output target specified by
 * the FileInterface argument.  The writing location in the output
 * target should be positioned at the point where the writing should
 * begin. When the save is complete, the writing location will be
 * correctly positioned to the end of the saved data. The writing
 * location is repositioned to the original position in the event of an error.
 * <br><br>
 * See the <i>[SAT Save and Restore](http://doc.spatial.com/articles/s/a/t/SAT_Save_and_Restore_981d.html)</i> Technical Article for more information.
 * <br><br>
 * <b>Note:</b> Beginning with ACIS release 6.3, it is <b>required</b> that the product ID
 * and units to be populated for the file header (using class <tt>FileInfo</tt>)
 * before you can save a SAT file. Refer to the reference templates for
 * the class @href FileInfo and function @href api_set_file_info for more
 * information. The Technical Article <i>[Save and Restore Examples](http://doc.spatial.com/articles/s/a/v/Save_and_Restore_Examples_39be.html)</i> may also be useful.
 * <br><br>
 * <b>Errors:</b> Failed to save entities; e.g., unable to write disk file.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
 * @param file_ptr
 * pointer to custom FileInterface object.
 * @param entity_list_save
 * list of entities to save.
 * @param ao
 * ACIS options.
 * <br><br>
 * @see api_set_file_info, api_save_entity_list, api_save_entity_list_file,
 * api_save_entity_list_with_history, api_save_entity_list_with_history_file,
 * api_save_history, api_save_history_file, api_restore_entity_list,
 * api_restore_entity_list_file, api_restore_entity_list_with_history,
 * api_restore_entity_list_with_history_file, api_restore_history, api_restore_history_file
 **/
DECL_KERN outcome api_save_entity_list_file(
    FileInterface* file_ptr,
    ENTITY_LIST const& entity_list_save,
    AcisOptions* ao = NULL);
/**
 * Sets the save file format.
 * <br><br>
 * <b>Role:</b> This API sets the output file format. The system can output
 * data in a format that a previous version can read. This is only true for
 * objects that are compatible in the previous release.
 * <br><br>
 * The value for the save version is obtained by multiplying the major version
 * by 100 and adding the minor version. For example, version 2.1 is represented
 * as 201, version 3.0 is represented as 300, and release 19 is represented as 1900.
 * The save version number does not change with service packs. The default value is
 * the currently installed version of ACIS.
 * <br><br>
 * <b>Note:</b> <i>The terms "version" and "release" are synonymous and can be
 * used interchangeably. The term "release" and thus "Rn" (where "n" is the
 * major version number) began with R10.</i>
 * <br><br>
 * <b>Limitations:</b> New functionality or structures in the higher release
 * are not correctly handled by the modeler, and therefore, are not
 * supported.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param major_version
 * major version.<br><br>
 * Valid major and minor versions are:<br>
 * 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8<br>
 * 2.0, 2.1<br>
 * 3.0, 3.1<br>
 * 4.0, 5.0, 6.0, 7.0, 8.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0,
 * 18.0, 19.0, 20.0, 21.0, 22.0, 23.0, 24.0, 25.0, 26.0, 27.0, 28.0, 29.0, 30.0
 * @param minor_version
 * minor version.
 **/
DECL_KERN outcome api_save_version(
    int major_version,
    int minor_version
);
/**
 * Gets the current save file format version.
 * <br><br>
 * <b>Role:</b> This API gets the output file format version.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal:</b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param major_version
 * major version returned, for example, 21.
 * @param minor_version
 * minor version returned, for example, 0.
 **/
DECL_KERN outcome api_get_save_version(
    int& major_version,
    int& minor_version
);
/** @} */

/**
 * \addtogroup ACISHISTORY
 * @{
 */

 /**
  * Removes a <tt>DELTA_STATE</tt> instance from a <tt>HISTORY_STREAM</tt>.
  * <br><br>
  * <b>Role:</b> This API removes a <tt>DELTA_STATE</tt> from its owning
  * <tt>HISTORY_STREAM</tt> without deleting it. This is used to in conjunction
  * with <tt>api_note_state</tt> and <tt>api_add_state</tt> to build multiple
  * independent history streams. After noting a state, it can be moved
  * to an alternate stream by first removing it from the default stream and
  * then adding it (using <tt>api_add_state</tt>) to the stream it is to become a part
  * of.
  * <br><br>
  * <b>Effect:</b> Changes the history stream, but not the geometric model
  * <br><br>
  * <b>Journal: </b> Not Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
  * <br><br>
  * @param ds
  * state to remove.
  * <br><br>
  * @see api_note_state, api_add_state
  **/
DECL_KERN outcome api_remove_state(
    DELTA_STATE* ds
);
/**
 * Modifies modeler state by applying zero or more <tt>DELTA_STATE</tt>.
 * <br><br>
 * <b>Role:</b> This API modifies the modeler's state by rolling forward
 * or back the given number of times. When rolling forward past a
 * branch in the <tt>HISTORY_STREAM</tt> the branch taken is unspecified. To take
 * a particular <tt>DELTA_STATE</tt>, save a pointer to a state on the branch and
 * use <tt>api_change_to_state</tt>. Branches are created by rolling back and then
 * making additional changes to the model.
 * <br><br>
 * <b>Errors:</b> The pointer to <tt>hs</tt> is <tt>NULL</tt>,
 * or the current state has not been noted.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param hs
 * history stream to roll.
 * @param nRequest
 * number of states to roll; positive is forward, negative is backward.
 * @param nActual
 * returns number of delta states rolled.
 * <br><br>
 * @see api_change_to_state
 **/
DECL_KERN outcome api_roll_n_states(
    HISTORY_STREAM* hs,
    int             nRequest,
    int& nActual
);

/**
 * Modifies the modeler state by applying zero or more <tt>DELTA_STATEs</tt>.
 * <br><br>
 * <b>Role:</b> This API modifies the modeler's state to match that when
 * the given <tt>DELTA_STATE</tt> was first noted. The system finds the appropriate
 * path through the <tt>HISTORY_STREAM</tt> of which the <tt>DELTA_STATE</tt> is a member.
 * <br><br>
 * <b>Errors:</b> The pointer to <tt>hs</tt> or <tt>ds</tt> is <tt>NULL</tt>,
 * or the current state has not been noted.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param hs
 * history state to be applied.
 * @param ds
 * delta state to be applied.
 * @param n_actual
 * Number of delta states rolled returned.
 * <br><br>
 * @see api_note_state
 **/
DECL_KERN outcome api_change_to_state(
    HISTORY_STREAM* hs,
    DELTA_STATE* ds,
    int& n_actual
);

/**
 * Distributes <tt>BULLETIN</tt>s in a <tt>DELTA_STATE</tt> to one or more <tt>HISTORY_STREAM</tt>
 * as directed by a <tt>StreamFinder</tt>.
 * <br><br>
 * <b>Role:</b> This API distributes the given <tt>DELTA_STATE</tt> to one or more
 * <tt>HISTORY_STREAM</tt>s as directed by the given <tt>StreamFinder</tt>. In each stream
 * distributed to, a new <tt>DELTA_STATE</tt> will be created to hold the
 * <tt>BULLETIN</tt>. <tt>StreamFinder</tt> is an abstract base class with one pure virtual function,
 * <tt>findStream</tt>, which must return the <tt>HISTORY_STREAM</tt> associated with the
 * given <tt>ENTITY</tt>. The <tt>findStream</tt> function may be called more than once
 * for each <tt>ENTITY</tt>. In a topology based search, the <tt>StreamFinder</tt> can
 * cache data in an early pass that can be used in a later pass.
 * <br><br>
 * The base class <tt>StreamFinder</tt> provides functions for finding the stream
 * based on an attached <tt>ATTRIB_HISTORY</tt> and for maintaining a mapping of
 * entities to streams.
 * <br><br>
 * As an example, here is the <tt>StreamFinder</tt> used by the Part Management
 * Component.
 * <br><pre>
 * class StreamFinderPM : public StreamFinder {
 *      // A StreamFinder for the PM_HUSK.
 *      // Implements a nested approach to
 *      // distribution in which bulletins go to
 *      // the most specific stream available.
 *      // Part streams are more specific than
 *      // the default stream. Body streams are more
 *      // specific than part streams.
 * public:
 *      virtual HISTORY_STREAM* findStream( ENTITY* );
 * };
 * <br><br>
 * HISTORY_STREAM *
 * StreamFinderPM::findStream(
 *      ENTITY* pEntity
 * )
 * {
 *       HISTORY_STREAM* pStream = NULL;
 *       // Look for a ATTRIB_HISTORY. If found add
 *       // the entity and associated geometry to
 *       // the stream map.
 *       pStream = findStreamFromAttribute(pEntity);
 *
 *       if( !pStream ) {
 *              // Still no stream?.
 *              // Look for a stream on the part
 *              // the entity is in.
 *              PART* part = get_part(pEntity);
 *              if(part) {
 *                     pStream = part->history_stream();
 *              }
 *       }
 *
 *       if( pStream ) {
 *              addToStreamMap(pEntity, pStream);
 *       }
 *
 *       return pStream;
 * }</pre>
 * The <tt>clearDelta</tt> argument tells how to handle <tt>BULLETIN</tt> objects for which
 * a target stream could not be found. If <tt>TRUE</tt> they are deleted along
 * with the input <tt>DELTA_STATE</tt>. If <tt>FALSE</tt>, they are left in the input
 * state.
 * <br><br>
 * The <tt>hideStates</tt> argument tells whether or not to mark the resulting
 * states as hidden in the target streams. @href api_pm_roll_n_states does
 * not count hidden states. Hidden states are useful for operations
 * that should appear read only to the user. For example, a pick or
 * display operation may calculate boxes and create <tt>DELTA_STATE</tt>s.
 * One can hide these states so they are not apparent to the user.
 * <br><br>
 * <b>Errors:</b> The pointer to a <tt>DELTA_STATE</tt> is <tt>NULL</tt>.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param pState
 * delta state to be distributed.
 * @param pStreamFinder
 * used to direct distribution.
 * @param clearDelta
 * <tt>TRUE</tt> to delete undistributed bulletins.
 * @param hideStates
 * <tt>TRUE</tt> to mark new states as hidden.
 **/
DECL_KERN outcome api_distribute_state_to_streams(
    DELTA_STATE* pState,
    StreamFinder* pStreamFinder,
    logical      clearDelta,
    logical      hideStates
);

/**
 * Finds <tt>DELTA_STATE</tt> objects with the given name in the specified history
 * stream and adds them to the supplied <tt>DELTA_STATE_LIST</tt>.
 * <br><br>
 * <b>Role:</b> This API function finds <tt>DELTA_STATE</tt> objects with the given
 * name in the specified <tt>HISTORY_STREAM</tt> and adds them to the supplied
 * <tt>DELTA_STATE_LIST</tt>. The given name may contain the wildcard characters
 * '?' and '*'.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param name
 * name of the delta state.
 * @param hs
 * history stream to use.
 * @param dslist
 * states found and returned.
 **/
DECL_KERN outcome api_find_named_state(
    const char* name,
    HISTORY_STREAM* hs,
    DELTA_STATE_LIST& dslist
);

/**
 * Gives a string name to a <tt>DELTA_STATE</tt>.
 * <br><br>
 * <b>Role:</b> This API assigns a name to the given delta state. Use the
 * specified name in calls to <tt>api_find_named_state</tt> to find a state to pass
 * to <tt>api_change_to_state</tt>.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param name
 * name to give to current operation.
 * @param ds
 * delta state to name.
 * <br><br>
 * @see api_change_to_state
 **/
DECL_KERN outcome api_name_state(
    const char* name,
    DELTA_STATE* ds
);

/**
 * Removes a <tt>DELTA_STATE</tt> from a <tt>HISTORY_STREAM</tt>.
 * <br><br>
 * <b>Role:</b> Snips the graph of <tt>DELTA_STATE</tt>s just before the given
 * state and deletes the piece of the graph that does not include the
 * active state. Thus one can prune forward branches by passing a state
 * after the current state. One can prune past history by passing a
 * state prior to the current state. It is impossible to prune away
 * the active state.
 * <br><br>
 * <b>Errors:</b> The pointer to <tt>ds</tt> or <tt>hs</tt> is <tt>NULL</tt>.<br>
 * The given <tt>DELTA_STATE</tt> is not in the given history stream.
 * <br><br>
 * <b>Effect:</b> Changes the history stream, but not the geometric model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param hs
 * history stream to prune.
 * @param ds
 * delta state at boundary of pruning returned.
 **/
DECL_KERN outcome api_prune_history(
    HISTORY_STREAM* hs,
    DELTA_STATE* ds = NULL
);

/**
 * If necessary, adds an empty <tt>DELTA_STATE</tt> to the beginning of the <tt>HISTORY_STREAM</tt>
 * so that users can roll to a state with no entities.
 * <br><br>
 * <b>Role:</b> This routine examines the root <tt>DELTA_STATE</tt> of the
 * specified <tt>HISTORY_STREAM</tt>. If the root state is empty (no bulletin
 * boards), then it does nothing. If the root <tt>DELTA_STATE</tt> is non-empty,
 * then it adds a new, empty, root state immediately "before" the
 * original root state. In either case, it returns (through the <tt>root_state</tt>
 * argument) a pointer to the resulting empty root state.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param history
 * history stream to be modified.
 * @param root_state
 * pointer to the (empty) root delta state.
 **/
DECL_KERN outcome api_ensure_empty_root_state(
    HISTORY_STREAM* history,
    DELTA_STATE*& root_state
);

/**
 * Gets the size of the <tt>DELTA_STATE</tt> in the <tt>HISTORY_STREAM</tt>.
 * <br><br>
 * <b>Errors:</b> The pointer to the <tt>HISTORY_STREAM</tt> is <tt>NULL</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param hs
 * stream to use.
 * @param size
 * size.
 * @param start_ds
 * start.
 **/
DECL_KERN outcome api_get_history_size(
    HISTORY_STREAM* hs,
    int& size,
    DELTA_STATE* start_ds = NULL
);

/**
 * Removes forward <tt>DELTA_STATE</tt> objects from a <tt>HISTORY_STREAM</tt>.
 * <br><br>
 * <b>Role:</b> Prunes away all branches of the history stream
 * following the active state.
 * <br><br>
 * <b>Errors:</b> The pointer <tt>hs</tt> is <tt>NULL</tt>.
 * <br><br>
 * <b>Effect:</b> Changes the history stream, but not the geometric model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param hs
 * history stream.
 * <br><br>
 * @see api_prune_history
 **/
DECL_KERN outcome api_prune_following(
    HISTORY_STREAM* hs
);

/**
 * Finds faces that have been deleted, created, or modified since a particular <tt>DELTA_STATE</tt>.
 * <br><br>
 * <b>Role:</b> This routine is intended to find lists of faces that
 * have been created, deleted, or modified between the (input) start
 * <tt>DELTA_STATE</tt> and the current <tt>DELTA_STATE</tt> of the history stream that
 * contains that state.
 * <br><br>
 * For the purposes of this API, a face is not considered modified if
 * its associated attributes or bounding box changes, but it is
 * considered modified if one of its "contained" entities is modified.
 * These contained entities are its <tt>SURFACE</tt>s, <tt>LOOP</tt>s, <tt>COEDGE</tt>s, <tt>EDGE</tt>s
 * (and associated curves) and <tt>VERTEX</tt>es (and associated points).
 * <br><br>
 * The intended use of this API is to allow customers to avoid
 * refaceting faces which can be determined (by examining the
 * <tt>HISTORY_STREAM</tt>) to be unchanged since the start <tt>DELTA_STATE</tt>.  Because
 * of this, the algorithm to identify "modified" faces is conservative:
 * whenever it is unclear whether a change recorded in the history
 * stream actually affected a face in a manner which requires refaceting,
 * that face is included in the "modified" list.  This ensures that all
 * faces which require refaceting will be included at one of the lists,
 * at the expense of introducing occasional "false positives" into the
 * "modified" list.
 * <br><br>
 * This API clears the deleted, created, and modified lists before
 * writing to them (it overwrites them).
 * <br><br>
 * For additional information refer to the Technical Article <i>[Incremental Faceting](http://doc.spatial.com/articles/i/n/c/Incremental_Faceting_d3f3.html)</i>.
 * <br><br>
 * <b>Errors:</b> Roll back state not on history stream main branch.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param ds
 * start state.
 * @param deleted_faces
 * deleted since start.
 * @param created_faces
 * created since start.
 * @param modified_faces
 * modified since start.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_get_modified_faces(
    DELTA_STATE* ds,
    ENTITY_LIST& deleted_faces,
    ENTITY_LIST& created_faces,
    ENTITY_LIST& modified_faces,
    AcisOptions* ao = NULL);


/**
 * Finds Entities (Faces, Edges and Vertices) that have been created, deleted or modified since a particular <tt>DELTA_STATE</tt>.
 * <br><br>
 * <b>Role:</b> This routine is intended to find lists of faces, edges and vertices that
 * have been created, deleted, or modified between the (input) start
 * <tt>DELTA_STATE</tt> and the current <tt>DELTA_STATE</tt> of the history stream that
 * contains that state.
 * <br><br>
 * For the purposes of this API, a face is not considered modified if
 * its associated attributes or bounding box changes, but it is
 * considered modified if one of its "contained" entities is modified.
 * These contained entities are its <tt>SURFACE</tt>s, <tt>LOOP</tt>s, <tt>COEDGE</tt>s, <tt>EDGE</tt>s
 * (and associated curves) and <tt>VERTEX</tt>s (and associated points).
 * <br><br>
 * An edge is considered modified if one of its "contained" entities is modified.
 * These contained entities are its <tt>CURVE</tt>s, and <tt>VERTEX</tt>s (and associated points).
 * <br><br>
 * A vertex is considered modified if associated <tt>APOINT</tt> is modified.
 *
 * The intended use of this API is to allow customers to avoid
 * refaceting entities which can be determined (by examining the
 * <tt>HISTORY_STREAM</tt>) to be unchanged since the start <tt>DELTA_STATE</tt>.
 * Because of this, the algorithm to identify "modified" entities is conservative:
 * whenever it is unclear whether a change recorded in the history
 * stream actually affected an entity in a manner which requires refaceting,
 * that entity is included in the "modified" list.  This ensures that all
 * entities which require refaceting will be included at one of the lists,
 * at the expense of introducing occasional "false positives" into the
 * "modified" list.
 * <br><br>
 * This API clears the deleted, created, and modified lists before
 * writing to them (it overwrites them).
 * <br><br>
 * For additional information refer to the Technical Article <i>[Incremental Faceting](http://doc.spatial.com/articles/i/n/c/Incremental_Faceting_d3f3.html)</i>.
 * <br><br>
 * <b>Errors:</b> Roll back state not on history stream main branch.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param ds
 * start state.
 * @param deleted_entites
 * deleted since start.
 * @param created_entites
 * created since start.
 * @param modified_entites
 * modified since start.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_get_modified_entities(
    DELTA_STATE* ds,
    ENTITY_LIST& deleted_entites,
    ENTITY_LIST& created_entites,
    ENTITY_LIST& modified_entites,
    AcisOptions* ao = NULL);

/**
 * Finds faces that have been created, deleted, or modified to support incremental faceting.
 * <br><br>
 * <b>Role:</b> This routine is intended to find lists of faces that
 * have been created, deleted, or modified (on and) between the given
 * <tt>DELTA_STATE</tt> and the current state of the history stream
 * that contains the given <tt>DELTA_STATE</tt>.
 * <br><br>
 * For the purposes of this API, a face is not considered modified if
 * only its associated attributes or bounding box have been changed, but
 * it is considered modified if one of its "contained" entities is modified.
 * These contained entities are its <tt>SURFACE</tt>, <tt>LOOPs</tt>,
 * <tt>COEDGEs</tt>, <tt>EDGEs</tt> (and associated <tt>CURVEs</tt>) and
 * <tt>VERTEXes</tt> (and associated <tt>APOINTs</tt>).
 * <br><br>
 * The intended use of this API is to allow customers to avoid
 * refaceting faces which can be determined (by examining the
 * <tt>HISTORY_STREAM</tt>) to be unchanged since the given <tt>DELTA_STATE</tt>.
 * Because of this, the algorithm to identify "modified" faces is conservative:
 * whenever it is unclear whether a change recorded in the history
 * stream actually affected a face in a manner that requires refaceting,
 * that face is included in the "modified" list.  This ensures that all
 * faces that require refaceting will be included at one of the lists,
 * at the expense of introducing occasional "false positives" into the
 * "modified" list.
 * <br><br>
 * If a <tt>DELTA_STATE</tt> is not supplied, this function uses either the
 * current or active <tt>DELTA_STATE</tt> of the given <tt>HISTORY_STREAM</tt>.
 * If neither a <tt>DELTA_STATE</tt> nor a <tt>HISTORY_STREAM</tt> are supplied,
 * this function uses either the current or active <tt>DELTA_STATE</tt> of the
 * default <tt>HISTORY_STREAM</tt>. If a <tt>DELTA_STATE</tt> is supplied,
 * this function ignores the given <tt>HISTORY_STREAM</tt>. If a <tt>DELTA_STATE</tt>
 * is supplied, the function examines the <tt>BULLETINs</tt> on the <tt>BULLETIN_BOARDs</tt>
 * (on and) between the given <tt>DELTA_STATE</tt> and the current state of the model.
 * This function supports rolled <tt>DELTA_STATEs</tt> and branched <tt>HISTORY_STREAMs</tt>.
 * In addition, it can examine the most recent <tt>BULLETIN_BOARD</tt> when
 * application logging is turned off (set with @href api_logging). If application
 * logging is turned off and an application needs to examine the faces changed by
 * multiple API functions, all of the API functions must be contained within a single
 * <tt>API_BEGIN/API_END</tt> block. In addition, the application must use the
 * "bounding box based algorithm" described below.
 * <br><br>
 * This API function will use one of two algorithms. One algorithm, the "bounding box
 * based algorithm," depends upon bounding boxes on faces being reset whenever
 * a significant change is made to the shape of a face. This algorithm requires
 * the bounding boxes of faces to be calculated as part of any operation that
 * modifies the model. This may occur as a result of faceting the model at the end
 * of an operation, or may occur independent of faceting. The other algorithm,
 * which is invoked when a <tt>DELTA_STATE</tt> is supplied and the <tt>use_lower_topology</tt>
 * flag is set to <tt>TRUE</tt>, examines the <tt>BULLETINs</tt> of faces and
 * all constituent entities. This algorithm does not depend on bounding boxes being
 * set before each operation and reset by the operation to find modified faces.
 * The "bounding box based algorithm" is the more efficient of the two algorithms;
 * however, it does require the application to recalculate bounding boxes as a part
 * of each operation.
 * <br><br>
 * This API function does not clear the deleted, created, and modified lists before
 * writing to them (that is, it appends to the given lists).
 * <br><br>
 * For additional information refer to the Technical Article <i>[Incremental Faceting](http://doc.spatial.com/articles/i/n/c/Incremental_Faceting_d3f3.html)</i>.
 * <br><br>
 * <b>Errors:</b> Requires searching bulletins of lower topology but the
 * <tt>use_lower_topology</tt> flag is set to not search such bulletins.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param deleted_faces
 * faces deleted since the given state.
 * @param created_faces
 * faces created since the given state.
 * @param modified_faces
 * faces modified since the given state.
 * @param ds
 * the given state.
 * @param stream
 * the given history stream.
 * @param use_lower_topology
 * flag to examine bulletins for entities below the face level.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_get_refacet_faces(
    ENTITY_LIST& deleted_faces,
    ENTITY_LIST& created_faces,
    ENTITY_LIST& modified_faces,
    DELTA_STATE* ds = NULL,
    HISTORY_STREAM* stream = NULL,
    logical use_lower_topology = FALSE,
    AcisOptions* ao = NULL);

/**
 * Finds all active <tt>ENTITY</tt>s associated with a <tt>HISTORY_STREAM</tt>.
 * <br><br>
 * <b>Role:</b> This API finds all of the "active" entities associated
 * with a <tt>HISTORY_STREAM</tt> (i.e. created and not yet deleted in the main
 * line). These <tt>ENTITY</tt> objects are added to the <tt>ENTITY_LIST</tt>, <tt>ents</tt>.
 * Note that <tt>ents</tt> is not cleared by this routine, since this routine has
 * "append to" syntax.
 * <br><br>
 * A <tt>TRUE</tt> <tt>unowned_only</tt> flag indicates that the user is only interested
 * in a minimal set of highest level entities, typically a list of bodies.
 * It filters out any entities which are not top-level, as well any
 * points, curves, surfaces, transforms, annotations, or attributes found
 * by scanning the remaining entities with a <tt>SCAN_DISTRIBUTE</tt> flag. (It
 * ignores any entities that are owned by other entities.)
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param hs
 * stream to search.
 * @param ents
 * list into which active entities are placed.
 * @param unowned_only
 * filter flag.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_get_active_entities(
    HISTORY_STREAM const* hs,
    ENTITY_LIST& ents,
    logical               unowned_only = FALSE,
    AcisOptions* ao = NULL);

/**
 * Finds all attributes of a specified user type in a history stream.
 * <br><br>
 * <b>Role:</b> This routine is intended to allow users to find all of
 * "their" attributes in a <tt>HISTORY_STREAM</tt>. A user will typically pass
 * in his master attribute type code to obtain a list all attributes in
 * the <tt>HISTORY_STREAM</tt> specific to his company. Any such attributes found
 * are added to the <tt>attribs</tt> list.
 * <br><br>
 * This routine does not clear attributes since it has "append to"
 * syntax.
 * <br><br>
 * <tt>derivation_level</tt> specifies the number of levels of derivation of the
 * requested attribute type from <tt>ENTITY</tt>.
 * <br><br>
 * The <tt>active_only</tt> flag is intended for use immediately after restoring
 * the <tt>HISTORY_STREAM</tt> from a file. If <tt>active_only</tt> is <tt>FALSE</tt>, then backup
 * copies of the requested attribute type are also returned, allowing
 * the user to perform direct post-restore operations that may be
 * necessary to rebind these attributes to user data.
 * <br><br>
 * This flag setting violates the encapsulation of the roll mechanism;
 * it shall be used with great caution and alternative solutions (such
 * as using entity IDs) should be explored. This flag setting is only
 * intended to give users access to their own attributes. Passing in a
 * Spatial type code when <tt>active_only</tt> is <tt>FALSE</tt> results in undefined
 * behavior, possibly returning the error "access to non-user bulletin
 * board entities is not allowed".
 * <br><br>
 * <b>Errors:</b> Access to non-user bulletin board entities is not allowed
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param hs
 * stream to search.
 * @param derivation_level
 * number of levels.
 * @param attrib_type_code
 * id of attrib type.
 * @param attribs
 * list to add to.
 * @param active_only
 * backup-type flag.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_get_all_user_attributes(
    HISTORY_STREAM const* hs,
    int                   derivation_level,
    int                   attrib_type_code,
    ENTITY_LIST& attribs,
    logical               active_only = TRUE,
    AcisOptions* ao = NULL);

/**
 * Sets the root <tt>DELTA_STATE</tt>.
 * <br><br>
 * <b>Role:</b> For any given <tt>ENTITY</tt>, the history of that <tt>ENTITY</tt> must be
 * maintained on a single <tt>HISTORY_STREAM</tt>. <tt>api_distribute_state_to_streams</tt>
 * implements a consistency check to make sure this is the case. When
 * processing a change or delete <tt>BULLETIN</tt>, it first determines which
 * <tt>HISTORY_STREAM</tt> should get the <tt>BULLETIN</tt>. It then checks to see that
 * the given stream contains a create <tt>BULLETIN</tt> for the <tt>ENTITY</tt>. This
 * depends on cooperation from <tt>api_prune_history</tt>.
 * <br><br>
 * <tt>api_prune_history</tt> may delete a number of <tt>DELTA_STATE</tt>s, but it retains
 * the create <tt>BULLETIN</tt> for any active <tt>ENTITY</tt> in the root <tt>DELTA_STATE</tt>.
 * <br><br>
 * In addition to holding create <tt>BULLETIN</tt>s from pruned <tt>DELTA_STATE</tt>s, the
 * root <tt>DELTA_STATE</tt> cannot be rolled over by <tt>api_roll_n_states</tt>,
 * <tt>api_change_to_state</tt>, or other high level roll APIs. (The low level
 * <tt>api_change_state</tt> can roll over the root state, but we recommend against
 * it.) This makes it useful for holding <tt>BULLETIN</tt> objects created during
 * application initializations. For example, one does not typically want
 * to be able to roll over the create <tt>BULLETIN</tt>s from loading a SAT file
 * into a new part.
 * <br><br>
 * <tt>api_make_root_state</tt> prunes away all previous history, saving the create
 * <tt>BULLETIN</tt>s at the beginning of the given state, and makes the state
 * the root state.
 * <br><br>
 * <b>Errors:</b> The pointer to <tt>ds</tt> is <tt>NULL</tt>.
 * <br><br>
 * <b>Effect:</b> Changes the history stream, but not the geometric model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param ds
 * state to make the root of its stream.
 **/
DECL_KERN outcome api_make_root_state(
    DELTA_STATE* ds
);
/** @} */

/**
 * \addtogroup ACISSAVERESTORE
 * @{
 */

 /**
  * Restores an <tt>ENTITY_LIST</tt> from a file with history.
  * <br><br>
  * <b>Role:</b> The file pointer is an open file positioned at the point
  * where the reading should begin. When the restore is complete,
  * the file pointer will be correctly positioned at the end of the restored data.
  * This allows an application to restore multiple entities intermixed
  * with other application specific data in a single save file.
  * <br><br>
  * Establish in the calling routine whether the file is text or binary
  * type and set <tt>text_mode</tt> correctly.
  * Restoring a binary file is about twice as fast as restoring a text file.
  * <br><br>
  * When an <tt>ENTITY</tt> is restored from a file, any unrecognized main <tt>ENTITY</tt>
  * types (<tt>BODY</tt>, <tt>CURVE</tt>, etc.) are skipped and any references to those
  * entities are set to <tt>NULL</tt>.Unrecognized descendent entities of <tt>ATTRIB</tt>,
  * <tt>SURFACE</tt>, or <tt>CURVE</tt> generate a new record for their immediate owner
  * class and references to them become references to the new record. If a record for a
  * derived class of <tt>ATTRIB</tt> is not recognized, an <tt>ATTRIB</tt> record results
  * so that the chain of attributes for the entity owning the unrecognized
  * attribute remains connected.
  * <br><br>
  * It is possible to restore entities made by versions having different
  * sets of attribute classes. Attribute types common to the two versions
  * are restored, but attributes of types unknown to the receiving version
  * are ignored.
  * <br><br>
  * A warning is given if the version of the product receiving the model
  * differs from the version that made the save file. It is an error if
  * the current product is older than that recorded in the file.
  * <br><br>
  * Application data referring to <tt>DELTA_STATE</tt>s or <tt>HISTORY_STREAM</tt>s can be
  * restored using the concepts from the following pseudo code.
  * <pre>
  * class app_data {
  *       DELTA_STATE* ds;
  *       void save(DELTA_STATE_LIST& dslist) {
  *              write_int(dslist.lookup(ds));
  *       }
  *       void restore(DELTA_STATE_LIST& dslist) {
  *              ds = read_int();
  *       }
  *       void fix_pointers(DELTA_STATE_LIST& dslist) {
  *              if( (int) < 0 ) {
  *                     ds = NULL;
  *              } else {
  *               ds = dslist[i];
  *        }
  *       }
  * };
  * DELTA_STATE_LIST    dslist;
  * HISTORY_STREAM_LIST hslist;
  * ENTITY_LIST         elist;
  * api_restore_entity_list_with_history(file, TRUE, elist, hslist, dslist);
  *
  * foreach(app_data* ap) {
  *       ap->restore(dslist);
  * }
  * foreach(app_data* ap) {
  *       ap->fix_pointers(dslist);
  * }</pre>
  * A similar procedure can be used when restoring application data that
  * refers to <tt>HISTORY_STREAM</tt>s. See @href api_save_entity_list_with_history for
  * an example of how to save the above <tt>app_data</tt>.
  * <br><br>
  * <b>Errors:</b> <i>Warning:</i> Version number of this system differs from version
  * that made the save file being read.
  * <br><br>
  * <i>Warning:</i> Record for unrecognized entity is being skipped.
  * Unable to read file. Malformed save file.
  * <br><br>
  * <b>Effect:</b> Changes model
  * <br><br>
  * <b>Journal: </b> Not Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
  * <br><br>
 * @param file_ptr
  * open file descriptor.
  * @param text_mode
  * <tt>TRUE</tt> if file is text, <tt>FALSE</tt> if binary.
  * @param entities
  * returns entities made.
  * @param hslist
  * returns history streams made.
  * @param dslist
  * returns delta states made.
  * @param ao
  * ACIS options.
  * <br><br>
  * @see api_set_file_info, api_save_entity_list, api_save_entity_list_file, api_save_entity_list_with_history,
  * api_save_entity_list_with_history_file, api_save_history, api_save_history_file, api_restore_entity_list,
  * api_restore_entity_list_file, api_restore_entity_list_with_history, api_restore_entity_list_with_history_file,
  * api_restore_history, api_restore_history_file
  **/
DECL_KERN outcome api_restore_entity_list_with_history(
    FILE* file_ptr,
    logical             text_mode,
    ENTITY_LIST& entities,
    HISTORY_STREAM_LIST& hslist,
    DELTA_STATE_LIST& dslist,
    AcisOptions* ao = NULL);

/**
 * Restores an <tt>ENTITY_LIST</tt> from a file.
 * <br><br>
 * <b>Role:</b> The file pointer is an open file positioned at the point
 * where the reading should begin. When the restore is complete,
 * the file pointer will be correctly positioned at the end of the restored data.
 * This allows an application to restore multiple entities intermixed with
 * other application specific data in a single save file.
 * <br><br>
 * Establish in the calling routine whether the file is text or binary
 * mode. Set <tt>text_mode</tt> correctly.
 * Restoring a binary file is about twice as fast as restoring a text file;
 * <br><br>
 * When an <tt>ENTITY</tt> is restored from a file, any unrecognized main <tt>ENTITY</tt>
 * types (<tt>BODY</tt>, <tt>CURVE</tt>, etc.) are skipped and any references to those
 * entities are set to <tt>NULL</tt>. Unrecognized descendent entities of <tt>ATTRIB</tt>,
 * <tt>SURFACE</tt>, or <tt>CURVE</tt> generate a new record for their immediate owner class
 * and references to them become references to the new record. If a record
 * for a derived class of <tt>ATTRIB</tt> is not recognized, an <tt>ATTRIB</tt> record results
 * so that the chain of attributes for the entity owning the unrecognized
 * attribute remains connected.
 * <br><br>
 * It is possible to restore entities made by versions having different
 * sets of attribute classes. Attribute types common to the two versions
 * are restored, but attributes of types unknown to the receiving version
 * are ignored.
 * <br><br>
 * A warning is given if the version of the product receiving the model
 * differs from the version that made the save file. It is an error if
 * the current product is older than that recorded in the file.
 * <br><br>
 * Application data referring to <tt>DELTA_STATE</tt>s or <tt>HISTORY_STREAM</tt>s can be
 * restored as in the following pseudo code.
 * <pre>
 * class app_data {
 *       DELTA_STATE* ds;
 *       void save(DELTA_STATE_LIST& dslist) {
 *              write_int(dslist.lookup(ds));
 *       }
 *       void restore(DELTA_STATE_LIST& dslist) {
 *              ds = read_int();
 *       }
 *       void fix_pointers(DELTA_STATE_LIST& dslist) {
 *              if( (int) < 0 ) {
 *                     ds = NULL;
 *              } else {
 *                     ds = dslist[i];
 *              }
 *       }
 * };
 * DELTA_STATE_LIST    dslist;
 * HISTORY_STREAM_LIST hslist;
 * ENTITY_LIST         elist;
 * api_restore_entity_list_with_history(file, TRUE, elist, hslist, dslist);
 *
 * foreach(app_data* ap) {
 *	ap->restore(dslist);
 * }
 * foreach(app_data* ap) {
 *	ap->fix_pointers(dslist);
 * }</pre>
 * A similar procedure can be used when restoring application data that
 * refers to <tt>HISTORY_STREAM</tt>s. See @href api_save_entity_list_with_history_file for
 * an example of how to save the above <tt>app_data</tt>
 * <br><br>
 * <b>Errors:</b> <i>Warning:</i> Version number of this system differs from version
 * that made the save file being read.
 * <br><br>
 * <i>Warning:</i> Record for unrecognized entity is being skipped.
 * Unable to read file. Malformed save file.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
* @param file_ptr
 * open file descriptor.
 * @param entities
 * returns entities made.
 * @param hslist
 * returns history streams made.
 * @param dslist
 * returns delta states made.
 * @param ao
 * ACIS options.
 * <br><br>
 * @see api_set_file_info, api_save_entity_list, api_save_entity_list_file, api_save_entity_list_with_history,
 * api_save_entity_list_with_history_file, api_save_history, api_save_history_file, api_restore_entity_list,
 * api_restore_entity_list_file, api_restore_entity_list_with_history, api_restore_entity_list_with_history_file,
 * api_restore_history, api_restore_history_file
 **/
DECL_KERN outcome api_restore_entity_list_with_history_file(
    FileInterface* file_ptr,
    ENTITY_LIST& entities,
    HISTORY_STREAM_LIST& hslist,
    DELTA_STATE_LIST& dslist,
    AcisOptions* ao = NULL);

/**
 * Restores a <tt>HISTORY_STREAM</tt> and associated entities and <tt>ENTITY</tt> ID information from a file.
 * <br><br>
 * <b>Role:</b> This routine restores entities and any associated
 * history information from a file. If the file has no history (i.e.
 * it was created using <tt>api_save_entity_list</tt>) then the <tt>create_new_hs</tt>
 * flag is examined. If the flag is <tt>TRUE</tt>, a new history stream is
 * instantiated to hold the created entities. If the flag is <tt>FALSE</tt>, the
 * entities in the file are created in the default stream. All created
 * <tt>HITORY_STREAM</tt>s are added to the <tt>HISTORY_STREAM_LIST</tt> <tt>hlist</tt>; no change
 * in the <tt>HISTORY_STREAM_LIST</tt> indicates the file had no history and the
 * entities were placed in the default stream. <tt>api_get_active_entities</tt>
 * can be called on the streams in <tt>hlist</tt> to find the entities read in.
 * <br><br>
 * The file pointer is an open file positioned at the point where the reading
 * should begin. When the restore is complete, the file pointer
 * will be correctly positioned at the end of the restored data. This allows
 * an application to restore multiple entities intermixed with other
 * application-specific data in a single save file.
 * <br><br>
 * Establish in the calling routine whether the file is text or binary
 * type. Set <tt>text_mode</tt> correctly: <tt>TRUE</tt> if the file is text, <tt>FALSE</tt> if
 * binary. Restoring a binary file is about twice as fast as restoring a text file.
 * <br><br>
 * When an <tt>ENTITY</tt> is restored from a file, any unrecognized main <tt>ENTITY</tt>
 * types (<tt>BODY</tt>, <tt>CURVE</tt>, etc.) are skipped and any references to those
 * entities are set to <tt>NULL</tt>.
 * Unrecognized descendent entities of <tt>ATTRIB</tt>, <tt>SURFACE</tt>, or <tt>CURVE</tt>
 * generate a new record for their immediate owner class, and references
 * to them become references to the new record. If a record for a derived
 * class of <tt>ATTRIB</tt> is not recognized, an <tt>ATTRIB</tt> record results so that
 * the chain of attributes for the entity owning the unrecognized
 * attribute remains connected.
 * <br><br>
 * It is possible to restore entities made by versions having different
 * sets of attribute classes. Attribute types common to the two versions
 * are restored, but attributes of types unknown to the receiving version
 * are ignored.
 * <br><br>
 * A warning is given if the version of the product receiving the model
 * differs from the version that made the save file. It is an error if
 * the current product is older than that recorded in the file. <br><br>
 * <b>Errors:</b> <i>Warning:</i> Version number of this system differs from version
 * that made the save file being read.
 * <br><br>
 * <i>Warning:</i> Record for unrecognized entity is being skipped.
 * Unable to read file. Malformed save file.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param file_ptr
 * open file descriptor.
 * @param text_mode
 * <tt>TRUE</tt> if file is text, <tt>FALSE</tt> if binary.
 * @param hlist
 * restored history streams.
 * @param create_new_hs
 * flag for creating new history stream.
 * @param ao
 * ACIS options.
 * @see api_set_file_info, api_save_entity_list, api_save_entity_list_file, api_save_entity_list_with_history,
 * api_save_entity_list_with_history_file, api_save_history, api_save_history_file, api_restore_entity_list,
 * api_restore_entity_list_file, api_restore_entity_list_with_history, api_restore_entity_list_with_history_file,
 * api_restore_history, api_restore_history_file
 **/
DECL_KERN outcome api_restore_history(
    FILE* file_ptr,
    logical             text_mode,
    HISTORY_STREAM_LIST& hlist,
    logical             create_new_hs = FALSE,
    AcisOptions* ao = NULL);

/**
 * Restores a <tt>HISTORY_STREAM</tt> and associated entities and entity id information from a file.
 * <br><br>
 * <b>Role:</b> This routine is equivalent to @href api_restore_history, taking
 * a <tt>FileInterface*</tt> rather than a <tt>FILE*</tt>. Please refer to the description
 * of that routine, @href api_restore_history.
 * <br><br>
 * If <tt>create_new_hs</tt> is set <tt>TRUE</tt> and the restored file has no history,
 * this function will create a new <tt>HISTORY_STREAM</tt>.
 * <br><br>
 * <b>Errors:</b> <i>Warning:</i> Version number of this system differs from version that
 * made the save file being read.
 * <br><br>
 * <i>Warning:</i> Record for unrecognized entity is being skipped.
 * Unable to read file. Malformed save file.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param file_ptr
 * open file descriptor.
 * @param hlist
 * restored history streams.
 * @param create_new_hs
 * flag for creating new history stream.
 * @param ao
 * ACIS options.
 * <br><br>
 * @see api_set_file_info, api_save_entity_list, api_save_entity_list_file, api_save_entity_list_with_history,
 * api_save_entity_list_with_history_file, api_save_history, api_save_history_file, api_restore_entity_list,
 * api_restore_entity_list_file, api_restore_entity_list_with_history, api_restore_entity_list_with_history_file,
 * api_restore_history, api_restore_history_file
 **/
DECL_KERN outcome api_restore_history_file(
    FileInterface* file_ptr,
    HISTORY_STREAM_LIST& hlist,
    logical             create_new_hs = FALSE,
    AcisOptions* ao = NULL);

/**
 * Writes an <tt>ENTITY_LIST</tt> to a file as text or in binary mode.
 * <br><br>
 * <b>Role:</b> The file pointer argument should be an open file
 * positioned at the point where the writing should begin. When the save
 * is complete, the file pointer will be correctly positioned at the
 * end of the saved data; therefore, an application can save multiple
 * bodies intermixed with other application specific data in a single
 * save file.
 * <br><br>
 * The floating point precision for real numbers in text files is six
 * digits for single precision and 15 digits for double precision.
 * <br><br>
 * History data is saved after active entities in a form allowing
 * @href api_restore_entity_list to restore without history if desired.
 * <br><br>
 * The returned <tt>dslist</tt> can be used by the application to map <tt>DELTA_STATE</tt>
 * pointers to unique integers and back again during save and restore
 * to maintain an association between <tt>DELTA_STATE</tt>s and application data.
 * For example, one might use the following pseudo code:
 * <pre>
 * class app_data {
 *       DELTA_STATE* ds;
 *       void save(DELTA_STATE_LIST& dslist) {
 *              write_int(dslist.lookup(ds));
 *       }
 *       void restore(DELTA_STATE_LIST& dslist) {
 *              ds = read_int();
 *       }
 *       void fix_pointers(DELTA_STATE_LIST& dslist) {
 *              if( (int) < 0 ) {
 *                     ds = NULL;
 *              } else {
 *                     ds = dslist[i];
 *              }
 *       }
 * };
 *
 * DELTA_STATE_LIST    dslist;
 * HISTORY_STREAM_LIST hslist;
 * ENTITY_LIST         elist;
 * elist.add(entity_to_save);
 * api_save_entity_list_with_history(file, TRUE, elist, hslist, dslist);
 *
 * foreach(app_data* ap) {
 *       ap->save(dslist);
 * }</pre>
 * See @href api_restore_entity_list_with_history for an example of how to
 * restore the above <tt>app_data</tt>
 * <br><br>
 * Some entities may have <tt>HISTORY_STREAM</tt>s attached via an <tt>ATTRIB_HISTORY</tt>.
 * In this case the <tt>hslist</tt> would be larger on return than on entry. The
 * returned list can be used as with the dslist when saving application
 * data.
 * <br><br>
 * See the <i>[SAT Save and Restore](http://doc.spatial.com/articles/s/a/t/SAT_Save_and_Restore_981d.html)</i> Technical Article for more information.
 * <br><br>
 * <b>Note:</b> Beginning with ACIS release 6.3, it is <b>required</b> that the product ID
 * and units to be populated for the file header (using class <tt>FileInfo</tt>)
 * before you can save a SAT file. Refer to the reference templates for
 * the class @href FileInfo and function @href api_set_file_info for more
 * information. The Technical Article <i>[Save and Restore Examples](http://doc.spatial.com/articles/s/a/v/Save_and_Restore_Examples_39be.html)</i> may also be useful.
 * <br><br>
 * <b>Errors:</b> Failed to save entities; e.g., unable to write disk file.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param file_ptr
 * open file descriptor.
 * @param text_mode
 * <tt>TRUE</tt> if file is text, <tt>FALSE</tt> if binary.
 * @param const&entity_list
 * entities to save.
 * @param hslist
 * history streams to save.
 * @param dslist
 * returns delta states saved.
 * @param ao
 * ACIS options.
 * <br><br>
 * @see api_set_file_info, api_save_entity_list, api_save_entity_list_file, api_save_entity_list_with_history,
 * api_save_entity_list_with_history_file, api_save_history, api_save_history_file, api_restore_entity_list,
 * api_restore_entity_list_file, api_restore_entity_list_with_history, api_restore_entity_list_with_history_file,
 * api_restore_history, api_restore_history_file
 **/
DECL_KERN outcome api_save_entity_list_with_history(
    FILE* file_ptr,
    logical             text_mode,
    ENTITY_LIST const& entity_list,
    HISTORY_STREAM_LIST& hslist,
    DELTA_STATE_LIST& dslist,
    AcisOptions* ao = NULL);

/**
 * Writes an <tt>ENTITY_LIST</tt> to file as text or in binary mode.
 * <br><br>
 * <b>Role:</b> The file pointer argument should describe an open file
 * positioned at the point where the writing should begin. When the save
 * is complete, the file pointer will be correctly positioned at the
 * end of the saved data; therefore, an application can save multiple
 * bodies intermixed with other application specific data in a single
 * save file.
 * <br><br>
 * The floating point precision for real numbers in text files is six
 * digits for single precision and 15 digits for double precision.
 * <br><br>
 * History data is saved after active entities in a form allowing
 * @href api_restore_entity_list to restore without history if desired.
 * <br><br>
 * The returned dslist can be used by the application to map <tt>DELTA_STATE</tt>
 * pointers to unique integers and back again during save and restore to
 * maintain an association between <tt>DELTA_STATE</tt>s and application data. For
 * example, one might use the following pseudo code:
 * <pre>
 * class app_data {
 *       DELTA_STATE* ds;
 *       void save(DELTA_STATE_LIST& dslist) {
 *              write_int(dslist.lookup(ds));
 *       }
 *       void restore(DELTA_STATE_LIST& dslist) {
 *              ds = read_int();
 *       }
 *       void fix_pointers(DELTA_STATE_LIST& dslist) {
 *              if( (int) < 0 ) {
 *                  ds = NULL;
 *              } else {
 *                     ds = dslist[i];
 *              }
 *       }
 * };
 *
 * DELTA_STATE_LIST    dslist;
 * HISTORY_STREAM_LIST hslist;
 * ENTITY_LIST         elist;
 * elist.add(entity_to_save);
 * api_save_entity_list_with_history_file (fileInt, elist, hslist, dslist);
 *
 * foreach(app_data* ap) {
 *       ap->save(dslist);
 * }</pre>
 * See @href api_restore_entity_list_with_history_file for an example
 * of how to restore the above <tt>app_data</tt>.
 * <br><br>
 * Some entities may have <tt>HISTORY_STREAM</tt>s attached via an <tt>ATTRIB_HISTORY</tt>.
 * In this case the <tt>hslist</tt> would be larger on return than on entry. The
 * returned list can be used as with the <tt>dslist</tt> when saving application data.
 * <br><br>
 * See the <i>[SAT Save and Restore](http://doc.spatial.com/articles/s/a/t/SAT_Save_and_Restore_981d.html)</i> Technical Article for more information.
 * <br><br>
 * <b>Note:</b> Beginning with ACIS release 6.3, it is <b>required</b> that the product ID
 * and units to be populated for the file header (using class <tt>FileInfo</tt>)
 * before you can save a SAT file. Refer to the reference templates for
 * the class @href FileInfo and function @href api_set_file_info for more
 * information. The Technical Article <i>[Save and Restore Examples](http://doc.spatial.com/articles/s/a/v/Save_and_Restore_Examples_39be.html)</i> may also be useful.
 * <br><br>
 * <b>Errors:</b> Failed to save entities; e.g., unable to write disk file.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
 * @param file_ptr
 * open file descriptor.
 * @param entity_list
 * entities to save.
 * @param hslist
 * HISTORY_STREAMs to save.
 * @param dslist
 * returns saved DELTA_STATEs.
 * @param mainline_only
 * save only active DELTA_STATEs.
 * @param ao
 * ACIS options.
 * <br><br>
 * @see api_set_file_info, api_save_entity_list, api_save_entity_list_file, api_save_entity_list_with_history,
 * api_save_entity_list_with_history_file, api_save_history, api_save_history_file, api_restore_entity_list,
 * api_restore_entity_list_file, api_restore_entity_list_with_history, api_restore_entity_list_with_history_file,
 * api_restore_history, api_restore_history_file
 **/
DECL_KERN outcome api_save_entity_list_with_history_file(
    FileInterface* file_ptr,
    ENTITY_LIST const& entity_list,
    HISTORY_STREAM_LIST& hslist,
    DELTA_STATE_LIST& dslist,
    logical             mainline_only = FALSE,
    AcisOptions* ao = NULL);

/**
 * Writes a <tt>HISTORY_STREAM</tt> and associated entities and entity ID information to a file.
 * <br><br>
 * <b>Role:</b> This API saves a complete <tt>HISTORY_STREAM</tt> to a file.
 * History data is saved after active entities in the same format as
 * @href api_save_entity_list_with_history, in a form that can be
 * restored without history by @href api_restore_entity_list, if desired.
 * If the history stream passed in is <tt>NULL</tt>, the default stream will be
 * saved.
 * <br><br>
 * The <tt>active_ents_only</tt> and <tt>mainline_only</tt> flags can be used to reduce
 * the size of the saved file. If <tt>mainline_only</tt> is <tt>TRUE</tt>, then only
 * <tt>DELTA_STATE</tt>s needed to get from the beginning of the <tt>HISTORY_STREAM</tt>
 * to the current state (the main line) are saved. This is equivalent
 * to ignoring all rolled delta states. If <tt>active_ents_only</tt> is <tt>TRUE</tt>, then
 * the active entities from hs are saved, along with <tt>HISTORY_STREAM</tt>
 * information containing only create bulletins for these entities.
 * <tt>active_ents_only</tt> is a more stringent condition than <tt>mainline_only</tt>;
 * the <tt>mainline_only</tt> flag has no effect when <tt>active_ents_only</tt> is <tt>TRUE</tt>.
 * <br><br>
 * Unhooked annotations are not considered active entities by this API;
 * rather they are stored with the history data. This means that they
 * will not be restored when using @href api_restore_entity_list and
 * will not be saved at all (even in the history data) when
 * <tt>active_ents_only</tt> is <tt>TRUE</tt>. If a user wants unhooked annotations to be
 * saved in the active entities section, he should find the unowned
 * active entities using @href api_get_active_entities (with
 * <tt>unowned_only</tt> = <tt>TRUE</tt>) and then call @href api_save_entity_list_with_history.
 * <br><br>
 * In addition to managing roll information, history streams also manage
 * entity id information; an entity id is unique to a history stream.
 * Entity IDs can only only be persisted by saving with history (using
 * @href api_save_history or @href api_save_entity_list_with_history).
 * Entity IDs are ignored when reading in with <tt>api_restore_entity_list</tt>;
 * they are only restored when using @href api_restore_entity_list_with_history
 * or @href api_restore_history. The main difference between using
 * @href api_save_entity_list and @href api_save_history
 * with <tt>active_ents_only</tt> = <tt>TRUE</tt> is that @href api_save_history maintains
 * the tag information.
 * <br><br>
 * The file pointer argument should be an open file positioned at the
 * point where the writing should begin. When the save is complete,
 * the file pointer will be correctly positioned at the end of the saved data.
 * An application can save multiple histories intermixed with other application
 * specific data in a single save file.
 * <br><br>
 * Errors can also occur if you use two different C runtime DLLs (for example,
 * one release and one debug) when using ACIS. Refer to the Technical Article
 * <i>Using Shared Libraries (DLLs)</i> in the Spatial Product Documentation
 * for more details.
 * <br><br>
 * Each entity record begins with a string identifier denoting its type.
 * When a file is restored, records of unrecognized derived classes will
 * be ignored.
 * <br><br>
 * The floating point precision for real numbers in text files is 6
 * digits for single precision and 15 digits for double precision.
 * <br><br>
 * <b>Errors:</b> Failed to save entities; for example, unable to write disk file.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param file_ptr
 * open file descriptor.
 * @param text_mode
 * <tt>TRUE</tt> if file is text, <tt>FALSE</tt> if binary.
 * @param hs
 * history stream to save.
 * @param active_ents_only
 * <tt>TRUE</tt> to ignore roll information.
 * @param mainline_only
 * <tt>TRUE</tt> to ignore rolled delta states.
 * @param ao
 * ACIS options.
 * @see api_set_file_info, api_save_entity_list, api_save_entity_list_file, api_save_entity_list_with_history,
 * api_save_entity_list_with_history_file, api_save_history, api_save_history_file, api_restore_entity_list,
 * api_restore_entity_list_file, api_restore_entity_list_with_history, api_restore_entity_list_with_history_file,
 * api_restore_history, api_restore_history_file
 **/
DECL_KERN outcome api_save_history(
    FILE* file_ptr,
    logical         text_mode,
    HISTORY_STREAM* hs = NULL,
    logical         active_ents_only = FALSE,
    logical         mainline_only = FALSE,
    AcisOptions* ao = NULL);

/**
 * Writes a <tt>HISTORY_STREAM</tt> and associated entities and entity id information to a file.
 * <br><br>
 * <b>Role:</b> This routine is equivalent to @href api_save_history,
 * taking a <tt>FileInterface*</tt> rather than a <tt>FILE*</tt>. Please refer to the
 * description of that routine.
 * <br><br>
 * <b>Errors:</b> Failed to save entities; e.g., unable to write disk file.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param file_ptr
 * open file descriptor.
 * @param hs
 * stream to save.
 * @param active_ents_only
 * <tt>TRUE</tt> to ignore roll information and unhooked <tt>ANNOTATION</tt>s.
 * @param mainline_only
 * <tt>TRUE</tt> to ignore rolled  delta states.
 * @param ao
 * ACIS options.
 * @see api_set_file_info, api_save_entity_list, api_save_entity_list_file, api_save_entity_list_with_history,
 * api_save_entity_list_with_history_file, api_save_history, api_save_history_file, api_restore_entity_list,
 * api_restore_entity_list_file, api_restore_entity_list_with_history, api_restore_entity_list_with_history_file,
 * api_restore_history, api_restore_history_file
 **/
DECL_KERN outcome api_save_history_file(
    FileInterface* file_ptr,
    HISTORY_STREAM* hs = NULL,
    logical         active_ents_only = FALSE,
    logical         mainline_only = FALSE,
    AcisOptions* ao = NULL);
/** @} */

/**
 * \addtogroup ACISTRANSFORM
 * @{
 */

 /**
  * Changes the transform entity attached to a body.
  * <b>Technical Article:</b> <i>[Entity Transformation](http://doc.spatial.com/articles/t/r/a/Transformation.html)</i>
  * <br><br>
  * <b>Role:</b> When transformations are applied to a body in ACIS, the
  * underlying geometries of all the subordinate entities are not changed.
  * This API simply attaches the transformation to the <tt>BODY</tt> entity and
  * indicates that calculations should pipe the geometry through the
  * transform. Each <tt>BODY</tt>'s transformation matrix gives the relationship
  * between its internal coordinate system and the working coordinate system.
  * <br><br>
  * If you want the transform actually applied to the geometry (called "fixing" the transform), use the
  * @href api_change_body_trans function. After applying the
  * transformation,
  * <pre>
  * BODY* my_body;
  * api_apply_transf(my_body, transf);</pre>
  * change the geometry of the object according to this transformation
  * by changing the <tt>BODY</tt>'s transform to an empty transformation:
  * <pre>
  * api_change_body_trans(my_body, NULL);</pre>
  * (This does increase the risk of introducing round-off errors to the geometry.)
  * <br><br>
  * Use transformations with caution. Scaling and translation effects can
  * combine to produce increasingly severe gaps in the geometry. Scaling
  * transforms not only scale up or down the geometry, but also scale up
  * or down gaps in the geometry. Furthermore, if you translate the geometry sufficiently, you can
  * move it far enough away from the origin that a gap is represented with
  * zero bits of resolution, and you cannot resolve it. Because <tt>SPAresabs</tt> does
  * not change, at some point geometric operations will fail.
  * <br><br>
  * Beyond these concerns, you should be aware that the application of a non-uniform scaling with
  * this API
  * <i>necessitates</i> that you fix the transform, in the manner shown above.  Such operations are handled by
  * Space Warping, which can create new topology by placing seam edges on periodic
  * %spline surfaces, for example, so that fixing the transform should be done immediately after the
  * transform is applied.
  * <br><br>
  * <b>Errors:</b> The <tt>entity</tt> argument is <tt>NULL</tt>.
  * <br><br>
  * <b>Effect:</b> Changes model
  * <br><br>
  * <b>Journal: </b> Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
  * <br><br>
 * @param entity
  * entity that recieves the new transform.
  * @param trans
  * new transform.
  * @param ao
  * ACIS options.
  **/
DECL_KERN outcome api_apply_transf(
    ENTITY* entity,
    SPAtransf const& trans,
    AcisOptions* ao = NULL);

/**
 * Substitutes the given transform for the existing transform of the <tt>BODY</tt>.
 * <b>Technical Article:</b> <i>[Entity Transformation](http://doc.spatial.com/articles/t/r/a/Transformation.html)</i>
 * <br><br>
 * <b>Role:</b> This API changes the geometry to leave the body unaltered in its
 * global coordinate space. Each <tt>BODY</tt> contains a transformation matrix that gives
 * the relationship between the internal coordinate system and that of the world
 * coordinate system. This API transforms the geometric definitions within the
 * object so that with the new transformation set in the body transformation, the
 * shape and position of the object are unchanged.
 * <br><br>
 * If <tt>negate</tt> is <tt>TRUE</tt>, this API negates the <tt>BODY</tt> by reflecting it about the origin
 * and reversing all directions.
 * <br><br>
 * Calling this API with a <tt>NULL</tt> transform pointer leaves the body with a <tt>NULL</tt>
 * transform, and any existing transforms are applied to the body geometry. For
 * example, to scale the <tt>BODY</tt>'s geometry, first call this API with the scaling
 * transform and then call it again with a <tt>NULL</tt> transform.
 * <br><br>
 * Use transformations with caution. Scaling and translation effects can combine to
 * produce increasingly severe gaps in the geometry. Scaling transforms not only
 * scale up or down the geometry, but also scale up or down gaps in the geometry.
 * If you translate the geometry, you can move it far enough away from the origin
 * that a gap is represented with 0 bits of resolution, and you cannot resolve it.
 * Because SPAresabs does not change, at some point geometric operations fail.
 * <br><br>
 * Call <tt>api_change_body_trans</tt> after @href api_transform_entity.
 * <br><br>
 * <b>Errors:</b> The pointer to a <tt>body</tt> is <tt>NULL</tt> or does not point to a <tt>BODY</tt>.
 * <br><br>
 * The pointer to a transform does not point to a <tt>TRANSFORM</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
* @param body
 * body to get new transform.
 * @param new_transform
 * new transform.
 * @param negate
 * negate the body.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_change_body_trans(
    BODY* body,
    TRANSFORM* new_transform,
    logical      negate = FALSE,
    AcisOptions* ao = NULL);

/**
 * Removes (discards) the transformation of a body.
 * <br><br>
 * <b>Role:</b> Each body contains a transformation matrix that gives the
 * relationship between its internal coordinate system and that of the world. This
 * API discards this transformation and places the body in the working coordinate
 * system.
 * <br><br>
 * <b>Errors:</b> Pointer to <tt>body</tt> is <tt>NULL</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
* @param entity
 * entity of interest.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_remove_transf(
    ENTITY* entity,
    AcisOptions* ao = NULL);
/** @} */

/**
 * \addtogroup ACISENTITYOPS
 * @{
 */

 /**
  * Creates a copy of a <tt>BODY</tt>.
  * <br><br>
  * <b>Role:</b> Given a <tt>BODY</tt> as input, copies the given <tt>BODY</tt> and all its
  * associated ("connected") entities, if any, using each <tt>ENTITY</tt>'s copy and fix-up
  * methods (e.g., <tt>copy_scan</tt>, <tt>copy_data</tt>, <tt>fix_pointers</tt>, etc.). This includes entities
  * that are <b>below</b> the given <tt>BODY</tt> in the topological hierarchy. For
  * example, copying an <tt>BODY</tt> copies the <tt>LUMPs</tt>, <tt>SHELLs</tt>, <tt>FACEs</tt>,
  * <tt>WIREs</tt>, <tt>LOOPs</tt>, <tt>COEDGEs</tt>, <tt>EDGEs</tt>, <tt>VERTEXes</tt>, etc.,
  * as well as all the associated curves, points, attributes, etc.
  * If there are no associated entities, only the given <tt>BODY</tt> is copied.
  * <br><br>
  * <b>Errors:</b> The pointer to an original <tt>BODY</tt> is <tt>NULL</tt> or does not point to a <tt>BODY</tt>.
  * <br><br>
  * <b>Effect:</b> Changes model
  * <br><br>
  * <b>Journal: </b> Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
  * <br><br>
 * @param body
  * body to be copied.
  * @param new_body
  * copy returned.
  * @param ao
  * ACIS options.
  **/
DECL_KERN outcome api_copy_body(
    BODY* body,
    BODY*& new_body,
    AcisOptions* ao = NULL);

/**
 * Creates a copy of an <tt>ENTITY</tt> and all its associated entities.
 * <br><br>
 * <b>Role:</b> This API copies the given <tt>ENTITY</tt> and all its associated
 * ("connected") entities, if any, using each <tt>ENTITY</tt>'s copy and fix-up methods (e.g.
 * <tt>copy_scan</tt>, <tt>copy_data</tt>, <tt>fix_pointers</tt>, etc.). This includes entities
 * that are <b>above</b> and <b>below</b> the given entity in the topological hierarchy. For example,
 * copying an <tt>EDGE</tt> copies the <tt>VERTEX</tt>, <tt>COEDGE</tt>s, <tt>LOOP</tt>s,
 * <tt>FACE</tt>s, <tt>SHELL</tt>s, etc., as well as all the associated curves, points,
 * attributes, etc. If there are no associated entities, only the given entity is copied.
 * <br><br>
 * <b>Note:</b> <tt>api_copy_entity</tt> copies all entities <b>above</b> and <b>below</b>
 * the input entity, while @href api_down_copy_entity only copies all entities <b>below</b>
 * the input entity. If <tt>api_copy_entity</tt> is incorrectly used where @href api_down_copy_entity
 * is intended, memory requirements could become unnecessarily and adversely large.
 * <br><br>
 * <b>Note:</b> For additional information on the variations of the entity copying process
 * refer to the Technical Article <i>[Copying Objects](http://doc.spatial.com/articles/c/o/p/Copying_Objects_1c3f.html)</i>.
 * <br><br>
 * <b>Errors:</b> Input entity is NULL.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param entity
 * entity to be copied.
 * @param new_entity
 * copy returned.
 * @param ao
 * ACIS options.
 * @see api_copy_entity, api_copy_entity_list, api_deep_copy_entity, api_deep_copy_entity_list,
 * api_down_copy
 **/
DECL_KERN outcome api_copy_entity(
    ENTITY* entity,
    ENTITY*& new_entity,
    AcisOptions* ao = NULL);

/**
 * Creates a copy of all entities in an <tt>ENTITY_LIST</tt> and all their associated entities.
 * <br><br>
 * <b>Role:</b> This API copies the entities in the given <tt>ENTITY_LIST</tt> and all
 * their associated ("connected") entities, if any, using each <tt>ENTITY</tt>'s copy and
 * fix-up methods (e.g. <tt>copy_scan</tt>, <tt>copy_data</tt>, <tt>fix_pointers</tt>, etc.). This includes
 * entities that are <b>above</b> and <b>below</b> the given entity in the topological
 * hierarchy. For example, copying an <tt>EDGE</tt> copies the <tt>VERTEX</tt>,
 * <tt>COEDGE</tt>s, <tt>LOOP</tt>s, <tt>FACE</tt>, <tt>SHELL</tt>s, etc., as well as
 * all the associated curves, points, attributes, etc. If there are no associated entities,
 * only the given entities are copied. The returned entity list's entities are in
 * the same order as the given <tt>ENTITY_LIST</tt>.
 * <br><br>
 * <b>Note:</b> For additional information on the variations of the entity copying process
 * refer to the Technical Article <i>[Copying Objects](http://doc.spatial.com/articles/c/o/p/Copying_Objects_1c3f.html)</i>.
 * <br><br>
 * <b>Errors:</b> Input list is empty.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param entity_list
 * list to copy.
 * @param copied_entity_list
 * copy returned.
 * @param ao
 * ACIS options.
 * @see api_copy_entity, api_copy_entity_list, api_deep_copy_entity, api_deep_copy_entity_list,
 * api_down_copy_entity
 **/
DECL_KERN outcome api_copy_entity_list(
    ENTITY_LIST& entity_list,
    ENTITY_LIST& copied_entity_list,
    AcisOptions* ao = NULL);

/**
 * Creates a deep copy of an <tt>ENTITY</tt> and all its associated entities.
 * <br><br>
 * <b>Role:</b> This API deep copies the given <tt>ENTITY</tt> and all its associated
 * (connected) entities, if any, using each <tt>ENTITY</tt>'s copy and fix-up methods (e.g.
 * <tt>copy_scan</tt>, <tt>copy_data</tt>, <tt>fix_pointers</tt>, etc.).
 * The difference between a deep copy and a regular copy is that a regular copy may
 * contain references to <i>shared</i> underlying associated entities, but a
 * deep copy will not. This includes entities that are <b>above</b> and <b>below</b> the
 * given entity in the topological hierarchy. For example, deep copying an <tt>EDGE</tt>
 * deep copies the <tt>VERTEX</tt>, <tt>COEDGE</tt>s, <tt>LOOP</tt>s, <tt>FACE</tt>s,
 * <tt>SHELL</tt>s, etc., as well as all the associated curves, points, attributes, etc.
 * If there are no associated entities, only the given entity is deep copied.
 * <br><br>
 * <b>Note:</b> For additional information on the variations of the entity copying process
 * refer to the Technical Article <i>[Copying Objects](http://doc.spatial.com/articles/c/o/p/Copying_Objects_1c3f.html)</i>.
 * <br><br>
 * <b>Errors:</b> Attempting to copy an entity that has associated entities that does not
 * support a deep copy routine. The <tt>NULL</tt> pointer is given to the <tt>entity</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
* @param entity
 * entity to copy.
 * @param new_entity
 * deep copy returned.
 * @param dpcpy_skip
 * flag to skip attributes not deep-copyable.
 * @param ao
 * ACIS options.
 * <br><br>
 * @see api_copy_entity, api_copy_entity_list, api_deep_copy_entity, api_deep_copy_entity_list,
 * api_down_copy_entity
 **/
DECL_KERN outcome api_deep_copy_entity(
    ENTITY* entity,
    ENTITY*& new_entity,
    logical      dpcpy_skip = FALSE,
    AcisOptions* ao = NULL);

/**
 * Creates a deep (complete) copy of an <tt>ENTITY</tt> and all entities below it in the topology
 * hierarchy.
 * <br><br>
 * <b>Role:</b> This API deep copies the given <tt>ENTITY</tt> and all its associated
 * ("connected") entities <b>below</b> it in the topology hierarchy. Like @href api_copy_entity,
 * it uses each <tt>ENTITY</tt>'s copy and fix-up methods (e.g. <tt>copy_scan</tt>,
 * <tt>copy_data</tt>, <tt>fix_pointers</tt>, etc.).
 * <br><br>
 * The difference between a deep copy and a regular copy is that a regular copy may
 * contain references to <i>shared</i> underlying associated entities, but a
 * deep copy will not.
 * <br><br>
 * The difference between a down copy and a regular copy is that a regular copy will
 * copy all entities connected both up and down the topological heirarchy, but a
 * down copy will only copy connected entities below the input entity.
 * <br><br>
 * For example, deep copying a <tt>FACE</tt> deep copies the <tt>LOOP</tt>s, <tt>COEDGE</tt>s,
 * <tt>EDGE</tt>s, and <tt>VERTEX</tt>s, as well as all the associated curves, points, attributes, etc.
 * If there are no associated entities, only the given entity is deep copied.
 * <br><br>
 * <b>Note:</b> Unlike @href api_copy_entity, attributes are not automatically copied (via their
 * <tt>copy_data</tt>). Instead, attributes are notified that their owner was copied using their
 * <tt>copy_owner</tt> method. This is a required and intentional difference of behavior;
 * attributes are not part of the ACIS topology and therefore do not know how to be
 * "down-copied". Granted, simple attributes <i>could</i> be automatically included in a
 * down-copy; it is complex attributes (attributes that point at entities other than
 * their owner) that are unable to down-copy.
 * <br><br>
 * <b>Note:</b> For additional information on the variations of the entity copying process
 * refer to the Technical Article <i>[Copying Objects](http://doc.spatial.com/articles/c/o/p/Copying_Objects_1c3f.html)</i>.
 * <br><br>
 * <b>Errors:</b> Attempting to copy an entity that has associated entities that do not
 * support a deep copy routine. The <tt>NULL</tt> pointer is given to the <tt>entity</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
* @param entity
 * entity to copy.
 * @param new_entity
 * deep copy returned.
 * @param dpcpy_skip
 * flag to skip attributes not deep-copyable.
 * @param ao
 * ACIS options.
 * <br><br>
 * @see api_copy_entity, api_copy_entity_list, api_deep_copy_entity, api_deep_copy_entity_list,
 * api_down_copy_entity
 **/
DECL_KERN outcome api_deep_down_copy_entity(
    ENTITY* entity,
    ENTITY*& new_entity,
    logical      dpcpy_skip = FALSE,
    AcisOptions* ao = NULL);

/**
 * Creates a deep copy of all entities in an <tt>ENTITY_LIST</tt> and all their associated entities.
 * <br><br>
 * <b>Role:</b> This API deep copies the entities in the given <tt>ENTITY_LIST</tt> and
 * all their associated (connected) entities, if any, using each <tt>ENTITY</tt>'s copy and
 * fix-up methods (e.g. <tt>copy_scan</tt>, <tt>copy_data</tt>, <tt>fix_pointers</tt>, etc.).
 * The difference between a deep copy and a regular copy is that a regular
 * copy may contain references to <i>shared</i> underlying associated entities, but
 * a deep copy will not. This includes entities that are <b>above</b> and <b>below</b> the given entity in the
 * topological hierarchy. For example, deep copying an <tt>EDGE</tt> deep copies the <tt>VERTEX</tt>,
 * <tt>COEDGE</tt>s, <tt>LOOP</tt>s, <tt>FACE</tt>s, <tt>SHELL</tt>s, etc. , as well as all the associated curves,
 * points, attributes, etc. If there are no associated entities, only the given
 * entities are deep copied. The returned <tt>ENTITY_LIST</tt>'s entities are in the same
 * order as the given <tt>ENTITY_LIST</tt>.
 * <br><br>
 * <b>Note:</b> For additional information on the variations of the entity copying process
 * refer to the Technical Article <i>[Copying Objects](http://doc.spatial.com/articles/c/o/p/Copying_Objects_1c3f.html)</i>.
 * <br><br>
 * <b>Errors:</b> Attempting to copy an entity that has associated entities that does not
 * support a deep copy routine.
 * <br><br>
 * The <tt>entity_list</tt> is empty.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
* @param entity_list
 * entities to copy.
 * @param new_entity_list
 * copies returned.
 * @param dpcpy_skip
 * flag to skip attributes not deep-copyable.
 * @param ao
 * ACIS options.
 * <br><br>
 * @see api_copy_entity, api_copy_entity_list, api_deep_copy_entity, api_deep_copy_entity_list,
 * api_down_copy_entity
 **/
DECL_KERN outcome api_deep_copy_entity_list(
    ENTITY_LIST& entity_list,
    ENTITY_LIST& new_entity_list,
    logical      dpcpy_skip = FALSE,
    AcisOptions* ao = NULL);

/**
 * Tests the deep copy functionality for improper sharing.
 * <br><br>
 * <b>Role:</b> This API deep copies the given list of entities. These entities
 * are saved to SAT files and restored, and then saved out again before and after a
 * deep copy. If this function is called in a <i>debug</i> build, the memory from the
 * original will be %pattern filled for additional checking of no sharing after a
 * deep copy. A comparison is done between the two SAT files created : <i> file1.sat
 * </i>(original) and <i>file2.sat</i> (deep copy).
 * <br><br>
 * This function is used primarily for internal testing. However, if derived
 * entities are used outside of ACIS, this function can be used to test their deep
 * copy capabilities.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param entity_list
 * list of entities to be deep copied.
 * @param numerical_tolerance
 * tolerance for real value comparisons.
 * @param report_all_errors
 * flag to skip  attributes not deep copyable.
 * @param file1
 * file of entities saved before deep-copy.
 * @param file2
 * file of entities saved after deep-copy.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_test_deep_copy(
    ENTITY_LIST const& entity_list,
    double             numerical_tolerance = SPAresnor,
    logical            report_all_errors = FALSE,
    char* file1 = NULL,
    char* file2 = NULL,
    AcisOptions* ao = NULL);

/**
 * Tests the deep down copy functionality for improper sharing.
 * <br><br>
 * <b>Role:</b> This API deep copies the given list of entities. These entities
 * are saved to SAT files and restored, and then saved out again before and after a
 * deep copy. If this function is called in an <i>ML</i> build, the memory from the
 * original will be %pattern filled for additional checking of no sharing after a
 * deep copy. A comparison is done between the two SAT files created : <i> file1.sat
 * </i>(original) and <i>file2.sat</i> (deep copy). The function also stores the deep
 * down copied entity list in the return_list variable so that the user can perform
 * further checking on it if desired.
 * <br><br>
 * This function is used primarily for internal testing. However, if derived
 * entities are used outside of ACIS, this function can be used to test their deep
 * copy capabilities.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param entity_list
 * list of entities to be deep copied.
 * @param return_list
 * copied entity list to return
 * @param numerical_tolerance
 * tolerance for real value comparisons.
 * @param report_all_errors
 * flag to skip  attributes not deep copyable.
 * @param file1
 * file of entities saved before deep-copy.
 * @param file2
 * file of entities saved after deep-copy.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_test_deep_down_copy(
    ENTITY_LIST const& entity_list,
    ENTITY_LIST& return_list,
    double             numerical_tolerance = SPAresnor,
    logical            report_all_errors = FALSE,
    char* file1 = NULL,
    char* file2 = NULL,
    AcisOptions* ao = NULL);


/**
 * Creates a copy of an <tt>ENTITY</tt> and all the entities below it in the topology
 * hierarchy.
 * <br><br>
 * <b>Role:</b> This API copies the given <tt>ENTITY</tt> and all its associated
 * ("connected") entities below it in the topology hierarchy. Like @href api_copy_entity,
 * it uses each <tt>ENTITY</tt>'s copy and fix-up methods (e.g. <tt>copy_scan</tt>,
 * <tt>copy_data</tt>, <tt>fix_pointers</tt>, etc.).
 * <br><br>
 * <b>Note:</b> Unlike @href api_copy_entity, attributes are not automatically copied (via their
 * <tt>copy_data</tt>). Instead, attributes are notified that their owner was copied using their
 * <tt>copy_owner</tt> method. This is a required and intentional difference of behavior;
 * attributes are not part of the ACIS topology and therefore do not know how to be
 * "down-copied". Granted, simple attributes <i>could</i> be automatically included in a
 * down-copy; it is complex attributes (attributes that point at entities other than
 * their owner) that are unable to down-copy.
 * <br><br>
 * <b>Note:</b> For additional information on the variations of the entity copying process
 * refer to the Technical Article <i>[Copying Objects](http://doc.spatial.com/articles/c/o/p/Copying_Objects_1c3f.html)</i>.
 * <br><br>
 * <b>Errors:</b> Input entity is NULL.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param in_ent
 * entity to be copied.
 * @param copy
 * copy returned.
 * @param ao
 * ACIS options.
 * @see api_copy_entity, api_copy_entity_list, api_deep_copy_entity, api_deep_copy_entity_list,
 * api_down_copy_entity
 **/
DECL_KERN outcome api_down_copy_entity(
    ENTITY* in_ent,
    ENTITY*& copy,
    AcisOptions* ao = NULL);


/**
 * Deletes an <tt>ENTITY</tt> and its subentities.
 * <br><br>
 * <b>Role:</b> This API deletes an <tt>ENTITY</tt> and all entities below it in the
 * topological hierarchy.
 * <br><br>
 * <b>Errors:</b> Pointer to the entity is <tt>NULL</tt> or not to topology (<tt>BODY</tt>,
 * <tt>LUMP</tt>, <tt>WIRE</tt>, etc.).
 * <br><br>
 * <b>Limitations:</b> Pointers above the deleted entity in the topological hierarchy
 * are not guaranteed to be set to <tt>NULL</tt>. For example, when a lump is deleted, the
 * body pointer to the lump may or may not be set to <tt>NULL</tt>. This function loses the
 * given topological entity, all lower-level topological entities comprising the
 * given entity, and reduces the use count. It could possibly remove any associated
 * geometry. It does not affect any pointers that were pointing to any of the
 * objects. When using this API, pointers that used to point to the entity need to
 * be fixed, or the item could be unhooked and then deleted.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param given_entity
 * entity to be deleted.
 * @param ao
 * ACIS options.
 * <br><br>
 * @see api_del_entity
 **/
DECL_KERN outcome api_delent(
    ENTITY* given_entity,
    AcisOptions* ao = NULL);

/**
 * Deletes the given <tt>ENTITY</tt> and all its associated entities.
 * <br><br>
 * <b>Role:</b> This API deletes an <tt>ENTITY</tt> and all its associated entities. This
 * loses all entities that are connected to the given entity. It will lose multiple
 * <tt>BODY</tt>s if they are connected by attributes. It does not affect any pointers that
 * were pointing to any of the objects. This allows you to delete an entire entity
 * from anywhere in the entity's topological hierarchy without having to traverse
 * to the top of the topology chain.
 * <br><br>
 * <b>Note:</b> Deletes entities above as well as below the specified entity in
 * the hierarchy.
 * <br><br>
 * <b>Errors:</b> Pointer to the entity is <tt>NULL</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
* @param given_entity
 * entity to be deleted.
 * @param ao
 * ACIS options.
 * <br><br>
 * @see api_delent
 **/
DECL_KERN outcome api_del_entity(
    ENTITY* given_entity,
    AcisOptions* ao = NULL);

/**
 * Deletes the entities contained in the given <tt>ENTITY_LIST</tt>.
 * <br><br>
 * <b>Role:</b> This API deletes all the entities in an <tt>ENTITY_LIST</tt> and all
 * their associated entities. This loses all entities that are connected to the
 * given entities. It will lose multiple bodies if they are connected by attributes.
 * It does not affect any pointers that were pointing to any of the objects. This
 * allows deletion of an entire entity from anywhere in the entity's topological
 * hierarchy without having to traverse to the top of the topology chain.
 * <br><br>
 * Use this API instead of @href api_del_entity when you need to delete more than one
 * entity at a time, since calling <tt>api_del_entity</tt> repeatedly could be dangerous as
 * the user has to keep track of what in the list has already been deleted.
 * <br><br>
 * <b>Note:</b> Deletes entities above as well as below the specified entities
 * in the hierarchy.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param given_list
 * entities to be deleted.
 * @param ao
 * ACIS options.
 * <br><br>
 * @see api_del_entity
 **/
DECL_KERN outcome api_del_entity_list(
    ENTITY_LIST& given_list,
    AcisOptions* ao = NULL);
/** @} */

/**
 * \addtogroup ACISWCS
 *
 * @{
 */

 /**
  * Creates a new working coordinate system.
  * <br><br>
  * <b>Role:</b> This API creates a new working coordinate system (<tt>new_wcs</tt>) with its origin
  * placed at <tt>origin</tt>, its <i>x</i>-axis pointing toward <tt>xpt</tt>, and its <i>y</i>-axis pointing
  * toward <tt>ypt</tt>.
  * <br><br>
  * <b>Effect:</b> Changes model
  * <br><br>
  * <b>Journal: </b> Not Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
  * <br><br>
 * @param origin
  * origin of WCS.
  * @param xpt
  * position on x-axis.
  * @param ypt
  * position in positive xy-plane.
  * @param new_wcs
  * returns created WCS.
  * @param ao
  * ACIS options.
  **/
DECL_KERN outcome api_wcs_create(
    const SPAposition& origin,
    const SPAposition& xpt,
    const SPAposition& ypt,
    WCS*& new_wcs,
    AcisOptions* ao = NULL);

/**
 * Sets a specified working coordinate system to be active.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param new_active
 * WCS to make active or NULL (model space).
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_wcs_set_active(
    WCS* new_active,
    AcisOptions* ao = NULL);

/**
 * Gets the active working coordinate system.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param active_wcs
 * returns active WCS or NULL.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_wcs_get_active(
    WCS*& active_wcs,
    AcisOptions* ao = NULL);
/** @} */

/**
 * \addtogroup ACISQUERIES
 * @{
 */

 /**
  * Gets the top level owner of an <tt>ENTITY</tt>.
  * <br><br>
  * <b>Role:</b> This API finds the top level <tt>ENTITY</tt> that "owns" the given
  * <tt>ENTITY</tt> (<tt>ent</tt>). For the purpose of this API, an <tt>ENTITY</tt> owns another <tt>ENTITY</tt> if the
  * second <tt>ENTITY</tt> is part of the definition of the first.
  * <br><br>
  * For example, if an <tt>EDGE</tt> is created with one of the curve-creation API functions,
  * that <tt>EDGE</tt> is not owned by any other <tt>ENTITY</tt>. If a solid is created, the <tt>EDGE</tt>s of
  * that solid are owned by the solid. For an <tt>EDGE</tt>, <tt>FACE</tt>, <tt>VERTEX</tt>, etc., that is a
  * part of a solid, this API returns the <tt>BODY</tt> pointer of the solid. If the <tt>ENTITY</tt>
  * is not owned by another <tt>ENTITY</tt>, then the pointer returns <i>itself</i>.
  * <br><br>
  * An <tt>ENTITY</tt> is top level when making a call to <tt>api_get_owner</tt> returns itself. Also,
  * every <tt>ENTITY</tt> contains an <tt>owner</tt> method. This method returns the next higher
  * <tt>ENTITY</tt>. If that object is the top level <tt>ENTITY</tt>, then the <tt>this</tt> pointer is returned.
  * This means that if a <tt>FACE</tt> does not point to an owning <tt>SHELL</tt>, this <tt>FACE</tt> is top
  * level for that model. A <tt>BODY</tt> is normally top level, but in some cases, there are
  * others that are the top level <tt>ENTITY</tt>.
  * <br><br>
  * <b>Effect:</b> Read-only
  * <br><br>
  * <b>Journal: </b> Not Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
  * <br><br>
 * @param ent
  * entity to determine owner.
  * @param owner
  * top level owner of entity returned.
  * @param ao
  * ACIS options.
  **/
DECL_KERN outcome api_get_owner(
    ENTITY* ent,
    ENTITY*& owner,
    AcisOptions* ao = NULL);

/**
 * Gets the end points of an edge.
 * <br><br>
 * <b>Role:</b> This API returns the end points of an edge. If the edge is part of a body
 * that has an attached transformation, the returned positions will be transformed using the
 * transformation of the body. This API function requires the edge to have an underlying %curve.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param crv
 * the given edge.
 * @param pt1
 * start position returned.
 * @param pt2
 * end position returned.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_get_curve_ends(
    EDGE* crv,
    SPAposition& pt1,
    SPAposition& pt2,
    AcisOptions* ao = NULL);

/**
 * Gets the geometry data that defines a circle or an %ellipse underlying an edge.
 * <br><br>
 * <b>Role:</b> This API returns the data defining a circular or elliptical edge.
 * The start and end parameters are for the edge.
 * If the sense of the edge is <tt>FORWARD</tt> these will be the same as the underlying
 * curve's parameter values at the start and end of the edge.
 * If the sense of the edge is <tt>REVERSED</tt> these will be negation of the underlying
 * curve's parameter values at the start and end of the edge.
 * <br><br>
 * <b>Errors:</b> The %curve under the <tt>EDGE</tt> is not an elliptical %curve.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param ell
 * elliptical or circular edge.
 * @param center
 * center returned.
 * @param normal
 * normal to %plane of %ellipse returned.
 * @param major_axis
 * major axis returned (length equals major radius).
 * @param radius_ratio
 * ratio of major radius to minor radius returned.
 * @param start_param
 * edge start parameter returned.
 * @param end_param
 * edge end parameter returned.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_get_ellipse_parameters(
    EDGE* ell,
    SPAposition& center,
    SPAunit_vector& normal,
    SPAvector& major_axis,
    double& radius_ratio,
    double& start_param,
    double& end_param,
    AcisOptions* ao = NULL);

//---------------------------------------------------------

/**
 * Gets all lumps related to an entity.
 * <br><br>
 * <b>Technical Articles:</b> <i>[Topology](http://doc.spatial.com/articles/t/o/p/Topology.html)</i>,
 * <i>[Lump](http://doc.spatial.com/articles/l/u/m/Lump.html)</i>
 * <br><br>
 * <b>Role:</b> If the input entity (<tt>ent</tt>) is a <tt>BODY</tt>, <tt>SHELL</tt>, <tt>FACE</tt>,
 * <tt>EDGE</tt>, <tt>LOOP</tt>, or <tt>VERTEX</tt>, this API returns all <tt>LUMP</tt>s of that entity.
 * <br><br>
 * By default, patterned objects are included in the list of entities. In general,
 * however, the parameter <tt>include_pat</tt> determines how this function deals with such
 * objects. The user may specify one of the following actions through this argument:
 * <br><br>
 * <dl>
 *    <dt><tt>PAT_CAN_CREATE</tt></dt><dd> - patterned objects are created if they do not already exist, and
 *                     are included in the list.</dd>
 *    <dt><tt>PAT_NO_CREATE</tt></dt><dd> -  only those patterned objects that have already been created are
 *                     included in the list.</dd>
 *    <dt><tt>PAT_IGNORE</tt></dt><dd> - no patterned objects besides seed %pattern objects are included in
 *                     the list.</dd>
 * </dl><br>
 * <b>Effect:</b> Usually Read-only; however, if <tt>include_pat == PAT_CAN_CREATE</tt> and unexpanded patterns exist,
 * then this function can create topological entities.
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param ent
 * entity to examine.
 * @param lump_list
 * returned list of lumps related to the input entity.
 * @param include_pat
 * how to treat patterned lumps.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_get_lumps(
    ENTITY* ent,
    ENTITY_LIST& lump_list,
    PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE,
    AcisOptions* ao = NULL);
DECL_KERN outcome gme_api_get_lumps(
    ENTITY* ent,
    ENTITY_LIST& lump_list,
    PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE,
    AcisOptions* ao = NULL);

// Get all SHELLS from an ENTITY

/**
 * Gets all shells related to an entity.
 * <br><br>
 * <b>Technical Articles:</b> <i>[Topology](http://doc.spatial.com/articles/t/o/p/Topology.html)</i>,
 * <i>[Shell](http://doc.spatial.com/articles/s/h/e/Shell.html)</i>
 * <br><br>
 * <b>Role:</b> If the input entity (<tt>ent</tt>) is a <tt>BODY</tt>, <tt>LUMP</tt>, <tt>FACE</tt>,
 * <tt>EDGE</tt>, <tt>LOOP</tt>, or <tt>VERTEX</tt>, this API returns all <tt>SHELL</tt>s of that entity.
 * <br><br>
 * By default, patterned objects are included in the list of entities. In general,
 * however, the parameter <tt>include_pat</tt> determines how this function deals with such
 * objects. The user may specify one of the following actions through this argument:
 * <br><br>
 * <dl>
 *    <dt><tt>PAT_CAN_CREATE</tt></dt><dd> - patterned objects are created if they do not already exist, and
 *                     are included in the list.</dd>
 *    <dt><tt>PAT_NO_CREATE</tt></dt><dd> -  only those patterned objects that have already been created are
 *                     included in the list.</dd>
 *    <dt><tt>PAT_IGNORE</tt></dt><dd> -     no patterned objects besides seed %pattern objects are included in
 *                     the list.</dd>
 * </dl><br>
 * <b>Effect:</b> Usually Read-only; however, if <tt>include_pat == PAT_CAN_CREATE</tt> and unexpanded patterns exist,
 * then this function can create topological entities.
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param ent
 * entity to examine.
 * @param shell_list
 * returned list of shells related to the input entity.
 * @param include_pat
 * how to treat patterned shells.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_get_shells(
    ENTITY* ent,
    ENTITY_LIST& shell_list,
    PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE,
    AcisOptions* ao = NULL);

DECL_KERN outcome gme_api_get_shells(
    ENTITY* ent,
    ENTITY_LIST& shell_list,
    PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE,
    AcisOptions* ao = NULL);
// Get all FACES of an ENTITY

/**
 * Gets all faces related to an entity.
 * <br><br>
 * <b>Technical Articles:</b> <i>[Topology](http://doc.spatial.com/articles/t/o/p/Topology.html)</i>,
 * <i>[Face](http://doc.spatial.com/articles/f/a/c/Face.html)</i>
 * <br><br>
 * <b>Role:</b> If the input entity (<tt>ent</tt>) is a <tt>BODY</tt>, <tt>LUMP</tt>, or
 * <tt>SHELL</tt>, this API  returns all <tt>FACE</tt>s of that entity. If the input entity is an
 * <tt>EDGE</tt>, <tt>LOOP</tt>, or <tt>VERTEX</tt>, this API returns all <tt>FACE</tt>s that
 * share the <tt>EDGE</tt>, <tt>LOOP</tt>, or <tt>VERTEX</tt>.
 * <br><br>
 * By default, patterned objects are included in the list of entities. In general,
 * however, the parameter <tt>include_pat</tt> determines how this function deals with such
 * objects. The user may specify one of the following actions through this argument:
 * <br><br>
 * <dl>
 *    <dt><tt>PAT_CAN_CREATE</tt></dt><dd> - patterned objects are created if they do not already exist, and
 *                     are included in the list.</dd>
 *    <dt><tt>PAT_NO_CREATE</tt></dt><dd> -  only those patterned objects that have already been created are
 *                     included in the list.</dd>
 *    <dt><tt>PAT_IGNORE</tt></dt><dd> -     no patterned objects besides seed %pattern objects are included in
 *                     the list.</dd>
 * </dl><br>
 * <b>Effect:</b> Usually Read-only; however, if <tt>include_pat == PAT_CAN_CREATE</tt> and unexpanded patterns exist,
 * then this function can create topological entities.
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param ent
 * entity to examine.
 * @param face_list
 * returned list of faces related to the input entity.
 * @param include_pat
 * how to treat patterned faces.
 * @param ao
 * ACIS options.
 **/

DECL_KERN outcome api_get_faces(
    ENTITY* ent,
    ENTITY_LIST& face_list,
    PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE,
    AcisOptions* ao = NULL);
DECL_KERN outcome gme_api_get_faces(
    ENTITY* ent,
    ENTITY_LIST& face_list,
    PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE,
    AcisOptions* ao = NULL);

// Get all LOOPS of an ENTITY

/**
 * Gets all loops related to an entity.
 * <br><br>
 * <b>Technical Articles:</b> <i>[Topology](http://doc.spatial.com/articles/t/o/p/Topology.html)</i>,
 * <i>[Loop](http://doc.spatial.com/articles/l/o/o/Loop.html)</i>
 * <br><br>
 * <b>Role:</b> If the input entity (<tt>ent</tt>) is a <tt>BODY</tt>, <tt>LUMP</tt>, <tt>FACE</tt>,
 * <tt>EDGE</tt>, <tt>SHELL</tt>, or <tt>VERTEX</tt>, this API returns all <tt>LOOP</tt>s of that entity.
 * <br><br>
 * By default, patterned objects are included in the list of entities. In general,
 * however, the parameter <tt>include_pat</tt> determines how this function deals with such
 * objects. The user may specify one of the following actions through this argument:
 * <br><br>
 * <dl>
 *    <dt><tt>PAT_CAN_CREATE</tt></dt><dd> - patterned objects are created if they do not already exist, and
 *                     are included in the list.</dd>
 *    <dt><tt>PAT_NO_CREATE</tt></dt><dd> -  only those patterned objects that have already been created are
 *                     included in the list.</dd>
 *    <dt><tt>PAT_IGNORE</tt></dt><dd> -     no patterned objects besides seed %pattern objects are included in
 *                     the list.</dd>
 * </dl><br>
 * <b>Effect:</b> Usually Read-only; however, if <tt>include_pat == PAT_CAN_CREATE</tt> and unexpanded patterns exist,
 * then this function can create topological entities.
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param ent
 * entity to examine.
 * @param loop_list
 * returned list of loops related to the input entity.
 * @param include_pat
 * how to treat patterned loops.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_get_loops(
    ENTITY* ent,
    ENTITY_LIST& loop_list,
    PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE,
    AcisOptions* ao = NULL);
DECL_KERN outcome gme_api_get_loops(
    ENTITY* ent,
    ENTITY_LIST& loop_list,
    PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE,
    AcisOptions* ao = NULL);

// Get all the WIREs from an ENTITY.

/**
 * Gets all the wires related to an entity.
 * <br><br>
 * <b>Technical Articles:</b> <i>[Topology](http://doc.spatial.com/articles/t/o/p/Topology.html)</i>,
 * <i>[Wire](http://doc.spatial.com/articles/w/i/r/Wire.html)</i>
 * <br><br>
 * <b>Role:</b> If the input entity (<tt>ent</tt>) has <tt>WIRE</tt>s (i.e., <tt>BODY</tt>, <tt>LUMP</tt>, etc.), this
 * API returns all the <tt>WIRE</tt>s of the entity.
 * <br><br>
 * By default, patterned objects are included in the list of entities. In general,
 * however, the parameter <tt>include_pat</tt> determines how this function deals with such
 * objects. The user may specify one of the following actions through this argument:
 * <br><br>
 * <dl>
 *    <dt><tt>PAT_CAN_CREATE</tt></dt><dd> - patterned objects are created if they do not already exist, and
 *                     are included in the list.</dd>
 *    <dt><tt>PAT_NO_CREATE</tt></dt><dd> -  only those patterned objects that have already been created are
 *                     included in the list.</dd>
 *    <dt><tt>PAT_IGNORE</tt></dt><dd> -     no patterned objects besides seed %pattern objects are included in
 *                     the list.</dd>
 * </dl><br>
 * <b>Effect:</b> Usually Read-only; however, if <tt>include_pat == PAT_CAN_CREATE</tt> and unexpanded patterns exist,
 * then this function can create topological entities.
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param ent
 * entity to examine.
 * @param out_list
 * returned list of wires related to the input entity.
 * @param include_pat
 * how to treat patterned wires.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_get_wires(
    ENTITY* ent,
    ENTITY_LIST& out_list,
    PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE,
    AcisOptions* ao = NULL);
DECL_KERN outcome gme_api_get_wires(
    ENTITY* ent,
    ENTITY_LIST& out_list,
    PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE,
    AcisOptions* ao = NULL);

// Get all the EDGES from an ENTITY.

/**
 * Gets all the edges related to an entity.
 * <br><br>
 * <b>Technical Articles:</b> <i>[Topology](http://doc.spatial.com/articles/t/o/p/Topology.html)</i>,
 * <i>[Edge](http://doc.spatial.com/articles/e/d/g/Edge.html)</i>
 * <br><br>
 * <b>Role:</b> If the input entity (<tt>ent</tt>) has <tt>EDGE</tt>s (i.e., <tt>BODY</tt>, <tt>LUMP</tt>, <tt>FACE</tt>, etc.),
 * this API returns all <tt>EDGE</tt>s of the entity. The input entity can also be a
 * <tt>VERTEX</tt>, in which case this API returns all <tt>EDGE</tt>s that share the common <tt>VERTEX</tt>.
 * <br><br>
 * By default, patterned objects are included in the list of entities. In general,
 * however, the parameter <tt>include_pat</tt> determines how this function deals with such
 * objects. The user may specify one of the following actions through this argument:
 * <br><br>
 * <dl>
 *    <dt><tt>PAT_CAN_CREATE</tt></dt><dd> - patterned objects are created if they do not already exist, and
 *                     are included in the list.</dd>
 *    <dt><tt>PAT_NO_CREATE</tt></dt><dd> -  only those patterned objects that have already been created are
 *                     included in the list.</dd>
 *    <dt><tt>PAT_IGNORE</tt></dt><dd> -     no patterned objects besides seed %pattern objects are included in
 *                     the list.</dd>
 * </dl><br>
 * <b>Effect:</b> Usually Read-only; however, if <tt>include_pat == PAT_CAN_CREATE</tt> and unexpanded patterns exist,
 * then this function can create topological entities.
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param ent
 * entity to examine.
 * @param edge_list
 * returned list of edges related to the input entity.
 * @param include_pat
 * how to treat patterned edges.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_get_edges(
    ENTITY* ent,
    ENTITY_LIST& edge_list,
    PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE,
    AcisOptions* ao = NULL);
DECL_KERN outcome gme_api_get_edges(
    ENTITY* ent,
    ENTITY_LIST& edge_list,
    PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE,
    AcisOptions* ao = NULL);

// Get all the COEDGES from an ENTITY.

/**
 * Gets all the coedges related to an entity.
 * <br><br>
 * <b>Technical Articles:</b> <i>[Topology](http://doc.spatial.com/articles/t/o/p/Topology.html)</i>,
 * <i>[Coedge](http://doc.spatial.com/articles/c/o/e/Coedge.html)</i>
 * <br><br>
 * <b>Role:</b> If the input entity (<tt>ent</tt>) has <tt>COEDGE</tt>s (i.e., <tt>BODY</tt>, <tt>LUMP</tt>, <tt>FACE</tt>,
 * etc.), this API returns all <tt>COEDGE</tt>s of the entity.
 * <br><br>
 * By default, patterned objects are included in the list of entities. In general,
 * however, the parameter <tt>include_pat</tt> determines how this function deals with such
 * objects. The user may specify one of the following actions through this argument:
 * <br><br>
 * <dl>
 *    <dt><tt>PAT_CAN_CREATE</tt></dt><dd> - patterned objects are created if they do not already exist, and
 *                     are included in the list.</dd>
 *    <dt><tt>PAT_NO_CREATE</tt></dt><dd> -  only those patterned objects that have already been created are
 *                     included in the list.</dd>
 *    <dt><tt>PAT_IGNORE</tt></dt><dd> -     no patterned objects besides seed %pattern objects are included in
 *                     the list.</dd>
 * </dl><br>
 * <b>Effect:</b> Usually Read-only; however, if <tt>include_pat == PAT_CAN_CREATE</tt> and unexpanded patterns exist,
 * then this function can create topological entities.
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param ent
 * entity to examine.
 * @param coedge_list
 * returned list of coedges related to the input entity.
 * @param include_pat
 * how to treat patterned coedges.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_get_coedges(
    ENTITY* ent,
    ENTITY_LIST& coedge_list,
    PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE,
    AcisOptions* ao = NULL);
DECL_KERN outcome gme_api_get_coedges(
    ENTITY* ent,
    ENTITY_LIST& coedge_list,
    PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE,
    AcisOptions* ao = NULL);

// Get all VERTICES of an ENTITY

/**
 * Gets all vertices related to an entity.
 * <br><br>
 * <b>Technical Articles:</b> <i>[Topology](http://doc.spatial.com/articles/t/o/p/Topology.html)</i>,
 * <i>[Vertex](http://doc.spatial.com/articles/v/e/r/Vertex.html)</i>
 * <br><br>
 * <b>Role:</b> If the specified entity (<tt>ent</tt>) is a <tt>BODY</tt>, <tt>LUMP</tt>, <tt>SHELL</tt>, <tt>FACE</tt>,
 * <tt>EDGE</tt>, or <tt>LOOP</tt>, this API returns all <tt>VERTEX</tt>es of that entity.
 * <br><br>
 * By default, patterned objects are included in the list of entities. In general,
 * however, the parameter <tt>include_pat</tt> determines how this function deals with such
 * objects. The user may specify one of the following actions through this argument:
 * <br><br>
 * <dl>
 *    <dt><tt>PAT_CAN_CREATE</tt></dt><dd> - patterned objects are created if they do not already exist, and
 *                     are included in the list.</dd>
 *    <dt><tt>PAT_NO_CREATE</tt></dt><dd> -  only those patterned objects that have already been created are
 *                     included in the list.</dd>
 *    <dt><tt>PAT_IGNORE</tt></dt><dd> -     no patterned objects besides seed %pattern objects are included in
 *                     the list.</dd>
 * </dl><br>
 * <b>Effect:</b> Usually Read-only; however, if <tt>include_pat == PAT_CAN_CREATE</tt> and unexpanded patterns exist,
 * then this function can create topological entities.
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param ent
 * entity to examine.
 * @param vertex_list
 * returned list of vertices related to the input entity.
 * @param include_pat
 * how to treat patterned vertices.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_get_vertices(
    ENTITY* ent,
    ENTITY_LIST& vertex_list,
    PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE,
    AcisOptions* ao = NULL);
DECL_KERN outcome gme_api_get_vertices(
    ENTITY* ent,
    ENTITY_LIST& vertex_list,
    PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE,
    AcisOptions* ao = NULL);

// Get all the TEDGES from an ENTITY.

/**
 * Gets all the tedges related to an entity.
 * <br><br>
 * <b>Role:</b> If the input entity (<tt>ent</tt>) has <tt>TEDGE</tt>s (i.e., <tt>BODY</tt>, <tt>LUMP</tt>, <tt>FACE</tt>, etc.),
 * this function returns all <tt>TEDGE</tt>s of the entity. The input entity can also be a
 * <tt>VERTEX</tt>, in which case this function returns all <tt>TEDGE</tt>s that share the common
 * <tt>VERTEX</tt>.
 * <br><br>
 * By default, patterned objects are included in the list of entities. In general,
 * however, the parameter <tt>include_pat</tt> determines how this function deals with such
 * objects. The user may specify one of the following actions through this argument:
 * <br><br>
 * <dl>
 *    <dt><tt>PAT_CAN_CREATE</tt></dt><dd> - patterned objects are created if they do not already exist, and
 *                     are included in the list.</dd>
 *    <dt><tt>PAT_NO_CREATE</tt></dt><dd> -  only those patterned objects that have already been created are
 *                     included in the list.</dd>
 *    <dt><tt>PAT_IGNORE</tt></dt><dd> -     no patterned objects besides seed %pattern objects are included in
 *                     the list.</dd>
 * </dl><br>
 * <b>Effect:</b> Usually Read-only; however, if <tt>include_pat == PAT_CAN_CREATE</tt> and unexpanded patterns exist,
 * then this function can create topological entities.
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param ent
 * entity to examine.
 * @param tedge_list
 * returned list of tedges related to the input entity.
 * @param include_pat
 * how to treat patterned tedges.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_get_tedges(
    ENTITY* ent,
    ENTITY_LIST& tedge_list,
    PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE,
    AcisOptions* ao = NULL);
DECL_KERN outcome gme_api_get_tedges(
    ENTITY* ent,
    ENTITY_LIST& tedge_list,
    PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE,
    AcisOptions* ao = NULL);
// Get all the TCOEDGES from an ENTITY.

/**
 * Gets all the tcoedges related to an entity.
 * <br><br>
 * <b>Role:</b> If the input entity (<tt>ent</tt>) has <tt>TCOEDGE</tt>s (i.e., <tt>BODY</tt>, <tt>LUMP</tt>, <tt>FACE</tt>, etc.),
 * this function returns all <tt>TCOEDGE</tt>s of the entity.
 * <br><br>
 * By default, patterned objects are included in the list of entities. In general,
 * however, the parameter <tt>include_pat</tt> determines how this function deals with such
 * objects. The user may specify one of the following actions through this argument:
 * <br><br>
 * <dl>
 *    <dt><tt>PAT_CAN_CREATE</tt></dt><dd> - patterned objects are created if they do not already exist, and
 *                     are included in the list.</dd>
 *    <dt><tt>PAT_NO_CREATE</tt></dt><dd> -  only those patterned objects that have already been created are
 *                     included in the list.</dd>
 *    <dt><tt>PAT_IGNORE</tt></dt><dd> -     no patterned objects besides seed %pattern objects are included in
 *                     the list.</dd>
 * </dl><br>
 * <b>Effect:</b> Usually Read-only; however, if <tt>include_pat == PAT_CAN_CREATE</tt> and unexpanded patterns exist,
 * then this function can create topological entities.
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param ent
 * entity to examine.
 * @param tcoedge_list
 * returned list of tcoedges related to the input entity.
 * @param include_pat
 * how to treat patterned tcoedges.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_get_tcoedges(
    ENTITY* ent,
    ENTITY_LIST& tcoedge_list,
    PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE,
    AcisOptions* ao = NULL);
DECL_KERN outcome gme_api_get_tcoedges(
    ENTITY* ent,
    ENTITY_LIST& tcoedge_list,
    PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE,
    AcisOptions* ao = NULL);
// Get all TVERTICES of an ENTITY

/**
 * Gets all <tt>TVERTEX</tt>es related to an entity.
 * <br><br>
 * <b>Role:</b> If the specified entity (<tt>ent</tt>) is a <tt>BODY</tt>, <tt>LUMP</tt>, <tt>SHELL</tt>, <tt>FACE</tt>, <tt>EDGE</tt>, or
 * <tt>LOOP</tt>, this function returns all <tt>TVERTEX</tt>es of that entity.
 * <br><br>
 * By default, patterned objects are included in the list of entities. In general,
 * however, the parameter <tt>include_pat</tt> determines how this function deals with such
 * objects. The user may specify one of the following actions through this argument:
 * <br><br>
 * <dl>
 *    <dt><tt>PAT_CAN_CREATE</tt></dt><dd> - patterned objects are created if they do not already exist, and
 *                     are included in the list.</dd>
 *    <dt><tt>PAT_NO_CREATE</tt></dt><dd> -  only those patterned objects that have already been created are
 *                     included in the list.</dd>
 *    <dt><tt>PAT_IGNORE</tt></dt><dd> -     no patterned objects besides seed %pattern objects are included in
 *                     the list.</dd>
 * </dl><br>
 * <b>Effect:</b> Usually Read-only; however, if <tt>include_pat == PAT_CAN_CREATE</tt> and unexpanded patterns exist,
 * then this function can create topological entities.
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param ent
 * entity to examine.
 * @param tvertex_list
 * returned list of tvertices related to the input entity.
 * @param include_pat
 * how to treat patterned tvertices.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_get_tvertices(
    ENTITY* ent,
    ENTITY_LIST& tvertex_list,
    PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE,
    AcisOptions* ao = NULL);
DECL_KERN outcome gme_api_get_tvertices(
    ENTITY* ent,
    ENTITY_LIST& tvertex_list,
    PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE,
    AcisOptions* ao = NULL);

// Get all the specified entities from the input ent

/**
 * Gets all specified entities related to an entity.
 * <br><br>
 * <b>Role:</b> This comprehensive API behaves analogous to the collection of
 * like API's that return the related entities of a specific entity, such as
 * @href api_get_faces and @href api_get_edges. This API however allows the specification of
 * multiple entity types to be returned from a single pass of the traversal
 * algorithm. The entity selection is made by passing a bit mask of topological IDs,
 * and optionally geometrical IDs, to the API. The bit masks are created by
 * "or-ing" the respective IDs together, as the following example demonstrates:
 * <pre>
 *    ENTITY_ID topo_bits = FACE_ID | EDGE_ID;
 *    ENTITY_ID geom._bits = SURFACE_ID | CURVE_ID; </pre>
 * The <tt>ENTITY_ID</tt> bit masks are categorized into topology IDs and geometry IDs, and
 * cannot be mixed. They are however, mutually exclusive within their respective
 * groups.
 * <br><br>
 * The following topological IDs are available:
 * <br><br>
 * <tt>BODY_ID</tt>, <tt>LUMP_ID</tt>, <tt>SHELL_ID</tt>, <tt>SUBSHELL_ID</tt>, <tt>WIRE_ID</tt>, <tt>FACE_ID</tt>, <tt>LOOP_ID</tt>, <tt>COEDGE_ID</tt>,
 * <tt>EDGE_ID</tt>, <tt>VERTEX_ID</tt>
 * <br><br>
 * The following geometrical IDs are available:
 * <br><br>
 * <tt>TRANSFORM_ID</tt>, <tt>APOINT_ID</tt>, <tt>PCURVE_ID</tt>, <tt>SURFACE_ID</tt>, <tt>CURVE_ID</tt>
 * <br><br>
 * Given a set of topological ID selections, the traversal algorithm searches for
 * the selected entities from the level of the input entity within the topological
 * hierarchy, and works its way down, selecting all that are lower in the
 * hierarchical order. When the selection set contains entity IDs that are higher
 * in the topological hierarchy than the input entity, then the higher-level
 * entities that share the input entities are also selected.
 * <br><br>
 * Given a <tt>FACE</tt> input entity with <tt>LUMP_ID</tt> and <tt>LOOP_ID</tt> selection IDs, for example,
 * the algorithm would select the owning <tt>LUMP</tt>, ignoring others, and would halt the
 * traversal after selecting all loops of the face since the <tt>LOOP_ID</tt> is the lowest
 * selection ID.
 * <br><br>
 * The algorithm does not traverse laterally and will simply select the input
 * entity in this case. For example, given a <tt>FACE</tt> input entity and a <tt>FACE_ID</tt>
 * selection, the input face would be returned.
 * <br><br>
 * The geometrical ID selections drive the algorithm in the same manner and assume
 * the same level in the hierarchy as their topological owners.
 * <br><br>
 * By default, patterned objects are included in the list of entities. In general,
 * however, the parameter <tt>include_pat</tt> determines how this function deals with such
 * objects. The user may specify one of the following actions through this argument:
 * <br><br>
 * <dl>
 *    <dt><tt>PAT_CAN_CREATE</tt></dt><dd> - patterned objects are created if they do not already exist, and
 *                     are included in the list.</dd>
 *    <dt><tt>PAT_NO_CREATE</tt></dt><dd> -  only those patterned objects that have already been created are
 *                     included in the list.</dd>
 *    <dt><tt>PAT_IGNORE</tt></dt><dd> -     no patterned objects besides seed %pattern objects are included in
 *                     the list.</dd>
 * </dl><br>
 * <b>Effect:</b> Usually Read-only; however, if <tt>include_pat == PAT_CAN_CREATE</tt> and unexpanded patterns exist,
 * then this function can create topological entities.
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param ent
 * entity to examine.
 * @param ent_list
 * returned list of related entities.
 * @param topology_ids
 * topological selection.
 * @param geometry_ids
 * geometrical selection.
 * @param include_pat
 * how to treat patterned faces.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_get_entities(
    ENTITY* ent,
    ENTITY_LIST& ent_list,
    ENTITY_ID topology_ids,
    ENTITY_ID geometry_ids,
    PAT_NEXT_TYPE include_pat = PAT_CAN_CREATE,
    AcisOptions* ao = NULL);

/** @} */

/**
 * \addtogroup ACISKERNLAWS
 * @{
 */

 //============================================================
 // Law api functions

class law;
class law_data;

/**
 * Creates a law from a string and an optional array of law data.
 * <br><br>
 * <b>Role:</b> This API parses a character string (<tt>str</tt>), generates the
 * associated law classes, and returns a pointer to the top-level law that was
 * created (<tt>answer</tt>). Deriving the <tt>law</tt> class and all associated classes individually
 * is possible. However, it is more likely that <tt>api_str_to_law</tt> and law string
 * parsing will be employed, because it is easier and more straightforward to
 * implement.
 * <br><br>
 * The valid syntax for the character strings (<tt>str</tt>) in the law mathematical
 * functions are given in the law symbol templates. The law mathematical functions
 * support nesting of law symbols. Once the character string (<tt>str</tt>) has been created,
 * it is passed to <tt>api_str_to_law</tt> along with a pointer to an output law (<tt>answer</tt>),
 * an array of law data (<tt>data</tt>), and the size of the law data array (<tt>size</tt>).
 * <br><br>
 * The <tt>unary_law</tt>, <tt>binary_law</tt>, and <tt>multiple_law</tt> classes are used if the application
 * is passing only laws into a law class, in which case it becomes a pointer to a
 * law or an array of pointers to laws, respectively. Numbers, positions,
 * parametric positions, vectors, and vector fields, in addition to the law symbols,
 * are passed as input to the <tt>api_str_to_law</tt> and become laws for these purposes.
 * <br><br>
 * On the other hand, the <tt>unary_data_law</tt> and <tt>multiple_data_law</tt> classes are used if
 * the application is passing more complicated structures into a law class. These
 * could be curves, wires, surfaces, transforms, or even laws. Instead of having a
 * pointer to a law or an array of pointers to laws, the <tt>unary_data_law</tt> and
 * <tt>multiple_data_law</tt> classes have a pointer to a <tt>law_data</tt> class or an array of
 * pointers to <tt>law_data</tt> classes, respectively.
 * <br><br>
 * <b>Limitations:</b> Only ASCII characters are supported in law-names.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param str
 * string of the law to be created.
 * @param answer
 * returns the created law.
 * @param data
 * array of supporting data used in the law creation.
 * @param size
 * size of the law data array.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_str_to_law(
    const char* str,
    law** answer,
    law_data** data = NULL,
    int            size = 0,
    AcisOptions* ao = NULL);
DECL_KERN outcome gme_api_str_to_law(
    const char* str,
    law** answer,
    law_data** data = NULL,
    int            size = 0,
    AcisOptions* ao = NULL);

/**
 * Integrates a law over a given domain to a given tolerance.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param input_law
 * the law to be integrated.
 * @param start
 * start of the domain of integration.
 * @param end
 * end of the domain of integration.
 * @param answer
 * result of the integration.
 * @param tolerance
 * (optional) tolerance for the accuracy of the result.
 * @param min_level
 * (optional) minimum Romberg Table rows.
 * @param used_level
 * (optional) number of Romberg rows returned.
 **/
DECL_KERN outcome api_integrate_law(
    law* input_law,
    double start,
    double end,
    double& answer,
    double tolerance = 1E-12,
    int    min_level = 2,
    int* used_level = NULL
);
DECL_KERN outcome gme_api_integrate_law(
    law* input_law,
    double start,
    double end,
    double& answer,
    double tolerance = 1E-12,
    int    min_level = 2,
    int* used_level = NULL
);


/**
 * Integrates a law over a given domain to a given tolerance, with respect to a given variable.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param input_law
 * the law to be integrated.
 * @param start
 * start of the domain of integration.
 * @param end
 * end of the domain of integration.
 * @param wrt
 * variable over which to integrate.
 * @param along
 * an array (the size of the take dim of the law) that gives the values for all
 * variables other than the integration variable.
 * @param answer
 * result of the integration.
 * @param tolerance
 * (optional) tolerance for the accuracy of the result.
 * @param min_level
 * (optional) minimum Romberg Table rows.
 * @param used_level
 * (optional) number of Romberg rows returned.
 **/
DECL_KERN outcome api_integrate_law_wrt(
    law* input_law,
    double start,
    double end,
    int    wrt,
    double* along,
    double& answer,
    double tolerance = 1E-12,
    int    min_level = 2,
    int* used_level = NULL
);

/**
 * Integrates a law over a given domain to a given tolerance, with respect to a given variable and an array of points used to split the domain.
 * <br><br>
 * <b>Role:</b> During the integration the function will take into account an array of
 * points into which to split the domain. This function should be used if the domain contains
 * known singularities.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param input_law
 * the law to be integrated.
 * @param start
 * start of the domain of integration.
 * @param end
 * end of the domain of integration.
 * @param wrt
 * variable over which to integrate.
 * @param along
 * an array (the size of the take dim of the law) that gives the values for all
 * variables other than the integration variable.
 * @param answer
 * result of the integration.
 * @param number_of_splits
 * (optional) number of singularities.
 * @param splits
 * (optional) number of splits.
 * @param tolerance
 * (optional) tolerance for the accuracy of the result.
 * @param min_level
 * (optional) minimum Romberg Table rows.
 * @param used_level
 * (optional) number of Romberg rows returned.
 **/
DECL_KERN outcome api_integrate_law_wrt_and_splits(
    law* input_law,
    double start,
    double end,
    int    wrt,
    double* along,
    double& answer,
    int    number_of_splits = 0,
    double* splits = NULL,
    double tolerance = 1E-12,
    int    min_level = 2,
    int* used_level = NULL
);

/**
 * Numerically differentiates a law at a given point with respect to a given variable a given number of times.
 * <br><br>
 * <b>Role:</b> The derivative may be taken from both sides or just from the
 * left or right.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param input_law
 * the law to differentiate.
 * @param where
 * where to take the derivative.
 * @param which_dim
 * which variable to take the derivative with respect to.
 * @param answer
 * the result of the differentiation.
 * @param type
 * 0 = normal, 1 = from the left, 2 = from the right.
 * @param times
 * how many times to take the derivative.
 **/
DECL_KERN outcome api_ndifferentiate_law(
    law* input_law,
    double* where,
    int    which_dim,
    double* answer,
    int    type = 0,	  // 0 for normal, 1 for left, 2 for right
    int    times = 1
);
DECL_KERN outcome gme_api_ndifferentiate_law(
    law* input_law,
    double* where,
    int    which_dim,
    double* answer,
    int    type = 0,	  // 0 for normal, 1 for left, 2 for right
    int    times = 1
);

/**
 * Returns all the roots of the given law over the given domain.
 * <br><br>
 * <b>Role:</b> Memory allocation for the returned roots (in <tt>answer</tt>)
 * is done by the API itself.  You should initialize a NULL pointer for <tt>answer</tt>
 * prior to calling the API, and free the memory following the call at an appropriate
 * stage.  For example:
 * <pre>
 * double* answer = NULL;
 * int numroots = 0;
 * outcome rc = api_nroots_of_law(my_law, -10, 10, &numroots, &answer);
 * // Process the result
 * ...
 * // Clean up
 * ACIS_DELETE [] STD_CAST answer;
 * </pre>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param input_law
 * law whose roots are desired.
 * @param start
 * start of the domain.
 * @param end
 * end of the domain.
 * @param size
 * returns the number of roots that were found.
 * @param answer
 * returns the roots.
 **/
DECL_KERN outcome api_nroots_of_law(
    law* input_law,
    double start,
    double end,
    int* size,
    double** answer
);

DECL_KERN outcome gme_api_nroots_of_law(law* input_law, double start, double end, int* size, double** answer);

/**
 * Gets the maximum value of a given law over the given domain.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param input_law
 * law whose maximum is desired.
 * @param start
 * start of the domain.
 * @param end
 * end of the domain.
 * @param answer
 * returns the location of maximum value within the domain.
 **/
DECL_KERN outcome api_nmax_of_law(
    law* input_law,
    double start,
    double end,
    double* answer
);

DECL_KERN outcome gme_api_nmax_of_law(
    law* input_law,
    double start,
    double end,
    double* answer
);
/**
 * Gets the minimum value of a given law over the given domain.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param input_law
 * law whose minimum is desired.
 * @param start
 * start of the domain.
 * @param end
 * end of the domain.
 * @param answer
 * returns the location of the minimum value within the domain.
 **/
DECL_KERN outcome api_nmin_of_law(
    law* input_law,
    double start,
    double end,
    double* answer
);

DECL_KERN outcome gme_api_nmin_of_law(
    law* input_law,
    double start,
    double end,
    double* answer
);
/**
 * Determines where two given laws are equal within a given domain.
 * <br><br>
 * <b>Role:</b> Memory allocation for the returned points (in <tt>answer</tt>)
 * is done by the API itself.  You should initialize a NULL pointer for <tt>answer</tt>
 * prior to calling the API, and free the memory following the call at an appropriate
 * stage.  For example:
 * <pre>
 * double* answer = NULL;
 * int numpoints = 0;
 * outcome rc = api_nsolve_laws(my_law1, my_law2, -10, 10, &numpoints, &answer);
 * // Process the result
 * ...
 * // Clean up
 * ACIS_DELETE [] STD_CAST answer;
 * </pre>
 * <b>Limitations:</b> The number of points at which the two laws are equal must be finite.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param input_law1
 * first law.
 * @param input_law2
 * second law.
 * @param start
 * start of the domain.
 * @param end
 * end of the domain.
 * @param size
 * returns the number of points at which the two laws are equal.
 * @param answer
 * returns the set of points at which the two laws are equal.
 **/
DECL_KERN outcome api_nsolve_laws(
    law* input_law1,
    law* input_law2,
    double start,
    double end,
    int* size,
    double** answer
);
DECL_KERN outcome gme_api_nsolve_laws(
    law* input_law1,
    law* input_law2,
    double start,
    double end,
    int* size,
    double** answer
);
/**
 * Creates the default rail laws for sweeping along a wire.
 * <br><br>
 * <b>Role:</b> This produces an array of rail laws that can be used by sweeping
 * in the sweep options. A single rail law is produced if <tt>path</tt> is a single edge
 * or a wire with a single underlying edge. Otherwise, it creates multiple rail
 * laws, one for each underlying edge in <tt>path</tt>.
 * <br><br>
 * The only required argument is <tt>path</tt>. If no other arguments are supplied, then
 * default rails are created. The default for the creation of rails is:
 * <br><br>
 * <ul>
 * <li>If the path is planar, the rail law is the planar normal. A constant
 * vector law is returned.</li>
 * <li>If the path is a helix, the rail law points towards the axis. The Frenet
 * law is returned.</li>
 * <li>If all edges in the wire are planar, then an array of rail laws is
 * created, whereby each law in the array corresponds to an edge in the wire. The
 * rail laws correspond to the planar normal of edges.</li>
 * <li>If faces are provided, then surface normal laws are returned. In this case, the path should be a wire, with edges on the supplied faces.</li>
 * <li>If the path isn't one of the above cases, the rail uses minimum rotation.</li>
 * </ul><br>
 * If the input <tt>path</tt> is composed of multiple pieces, such as a wire with more than
 * one underlying edge, then array arguments must supply the same number of
 * elements as the number of <tt>path</tt> elements. They may pad their array with <tt>NULL</tt>
 * arguments.
 * <br><br>
 * The <tt>axis</tt> argument is used for path segments that have an implied center axis. An
 * example of this might be a helix, an expanding helix, or the coil of a telephone
 * handset cable. The <tt>axis</tt> argument is the derivative of the implied center axis,
 * which tells the implied axis direction. When the <tt>axis</tt> is supplied, then its
 * cross product with the path is returned. The <tt>axis</tt> array can be padded with <tt>NULL</tt>
 * for sections of the <tt>path</tt> that do not have an implied axis.
 * <br><br>
 * The <tt>face</tt> argument is used when a portion of the path segments borders a
 * non-analytic face. The coedge of the wire provided as <tt>path</tt> must actually belong
 * to the face entity supplied. The face must be non-analytic. The resulting rail
 * is oriented to the face normal. The <tt>face</tt> array can be padded with <tt>NULL</tt> for
 * sections of the path that do not have such a face.
 * <br><br>
 * The <tt>user_rails</tt> argument permits any default rail for a given section of the path
 * to be overridden by the user-supplied law in the array. The <tt>user_rails</tt> array can
 * be padded with <tt>NULL</tt> for sections of the path that are to use the default.
 * <br><br>
 * The <tt>twist</tt> argument works on the whole rail array. After the other rail
 * parameters have been input and calculated, the law provided by <tt>twist</tt> operates on
 * the whole set of rails. This takes in an angle of twist per distance along the
 * path.
 * <br><br>
 * <b>Limitations:</b> When faces are supplied, the path must be a wire, with each edge lying in the corresponding face.
 * The edges must not have non-G1 discontinuities in this case.
 * <br><br>
 * <b>Effect:</b> Read-only.
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param path
 * a WIRE or EDGE.
 * @param rails
 * array of rail laws returned.
 * @param number_of_rails
 * number of rail laws returned.
 * @param axis
 * optional axis in an array.
 * @param faces
 * optional faces in an array.
 * @param user_rails
 * optional user defined rails.
 * @param twist_law
 * optional twist law.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_make_rails(
    ENTITY* path,
    law**& rails,
    int& number_of_rails,
    law** axis = NULL,
    FACE** faces = NULL,
    law** user_rails = NULL,
    law* twist_law = NULL,
    AcisOptions* ao = NULL,
    SPAunit_vector const& in_rigid_tangent = *(const class SPAunit_vector*)NULL_REF);

/**
 * Converts a law mathematic function into an entity for the purposes of saving to and restoring from a SAT file.
 * <br><br>
 * <b>Role:</b> Law mathematic functions that are used for the analysis of a
 * design are not normally saved to the SAT file. Typically, only laws that are
 * attached to model entities through geometry definitions are saved to the SAT
 * file. In order to make laws more persistent and to share them from session to
 * session, they can be turned into <tt>LAW</tt> instances, which being derived from <tt>ENTITY</tt>
 * can be saved and restored.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param input_law
 * the law function to be converted.
 * @param out_ent
 * the resulted entity into which the law is converted.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_law_to_entity(
    law* input_law,
    ENTITY*& out_ent,
    AcisOptions* ao = NULL);

DECL_KERN outcome gme_api_law_to_entity(
    law* input_law,
    ENTITY*& out_ent,
    AcisOptions* ao = NULL);
/** @} */

/**
 * \addtogroup ACISTRANSFORM
 * @{
 */

class transform_entity_options;
/**
 * Applies a transformation to an entity.
 * <b>Technical Article:</b> <i>[Entity Transformation](http://doc.spatial.com/articles/t/r/a/Transformation.html)</i>
 * <br><br>
 * <b>Role:</b> If the entity is a <tt>BODY</tt>, this API concatenates the transform
 * with any transform that is already applied. If the entity is not a body, this
 * API transforms the geometry.
 * <br><br>
 * If the Warping Component is linked into the executable, one can do non-uniform
 * scaling with this function. If the given transform contains a shear component,
 * the options object can be used to specify whether or not geometric simplification
 * is done.
 * <br><br>
 * <b>Errors:</b> A <tt>NULL</tt> pointer to an entity is specified.
 * <br><br>
 * An attempt is made to transform an entity that belongs to another entity.
 * <br><br>
 * <b>Effect:</b> Changes model.
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler, 3D ACIS Polyhedral
 * <br><br>
 * @param ent
 * entity to transform.
 * @param tform
 * transform to apply.
 * @param te_opts
 * transform options object.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_transform_entity(
    ENTITY* ent,                // the ENTITY to transform
    const SPAtransf& tform,     // the transform to apply
    transform_entity_options* te_opts = NULL,
    AcisOptions* ao = NULL);
/** @} */

/**
 * \addtogroup ACISQUERIES
 * @{
 */

 /**
  * Computes the given number of equidistant points in the parametric space of the underlying curve for the <tt>COEDGE</tt>.
  * <br><br>
  * <b>Role:</b> This API returns %surface tangents perpendicular to the %curve.
  * Sense flag <tt>forward</tt> controls the direction for ordering of points. Sense flag
  * <tt>outward</tt> indicates whether the face tangent points away from or into the face.
  * This function is useful for interpolating surfaces to join with the face.
  * <br><br>
  * <b>Errors:</b> <tt>NULL</tt> <tt>ENTITY</tt> or not a <tt>COEDGE</tt>.
  * <br><br>
  * <b>Effect:</b> Read-only
  * <br><br>
  * <b>Journal: </b> Not Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
  * <br><br>
 * @param coedge
  * coedge of face.
  * @param forward
  * forward direction of evaluation.
  * @param outward
  * tangents point off of face.
  * @param num_pts
  * size of arrays/number of points where to evaluate.
  * @param pts
  * points along edge returned (user allocates arrays).
  * @param tans
  * %surface tangents along edge at positions returned.
  **/
DECL_KERN outcome api_extract_coed_info(
    COEDGE* coedge,
    logical         forward,
    logical         outward,
    int             num_pts,
    SPAposition* pts,
    SPAunit_vector* tans
);
/** @} */

/**
 * \addtogroup BSGEOMETRY
 * @{
 */

 /**
  * Splits the approximating surface for a <tt>VBL_SURF</tt> into <tt>n</tt> four-sided <tt>bs3_surface</tt> patches.
  * <br><br>
  * <b>Role:</b> This API approximates an <i>n</i>-sided @href VBL_SURF by n four-sided
  * <tt>bs3_surfaces</tt>, which enables ACIS to output vertex blend surfaces in a form that
  * can be read and used by most other packages.
  * <br><br>
  * In ACIS, the <tt>VBL_SURF</tt> is approximated by a <tt>bs3_surface</tt> whose parameterization is
  * defined over an <i>n</i>-sided regular polygon. This API makes up <i>n</i> four-sided patches
  * that are separated from one another in the original approximating surface
  * parameter space by straight lines radiating from the center of the <i>n</i>-sided
  * polygon to the midpoint of each of the sides. The new approximating surfaces are
  * parameterized over the unit square. Adjacent <tt>bs3_surface</tt> patches are made up to
  * have the same number of knot points along common boundaries, and so the surfaces
  * are C0 continuous across the boundaries. However, the parameter lines do not run
  * smoothly across the boundaries, and so the approximation is not C1 or G1
  * continuous across the patch boundaries.
  * <br><br>
  * The API function receives a pointer to a surface, which has an underlying
  * <tt>VBL_SURF</tt>. The caller should also supply two doubles, which specify the requested
  * fit tolerances of the approximating surfaces, both on the interior of the
  * <tt>VBL_SURF</tt>, and on its boundary. These will return the fit tolerances that the
  * approximating surfaces have achieved, which may be larger than the requested fit
  * tolerances, if these are particularly small, or if the surface is particularly
  * complex. One might, for example, request an internal fit tolerance of .001 and a
  * boundary fit tolerance that is 10 times smaller than this. Note that the
  * interior fit tolerance (but NOT the boundary one) may be passed as exactly -1.0
  * to mean "do not measure the interior fit", in which case no particular interior
  * fit is guaranteed, except that which comes about naturally by having fit the
  * boundary correctly, and no meaningful value is returned for the achieved
  * interior fit. The function can operate more quickly if no specific interior fit
  * is requested. The internal joins between the patch boundaries are unaffected by
  * this.
  * <br><br>
  * The caller must also supply the function with a pointer to a <tt>bs3_surface</tt> and a
  * reference to an integer. These will be used to return an array of the
  * approximating <tt>bs3_surface</tt>s and the length of this array, respectively.
  * Additionally, an <tt>approx_error</tt> flag can be supplied which specifies whether the
  * errors returned need to be precise or merely a (close) upper bound. If passed
  * <tt>TRUE</tt>, the error is bounded approximately but quite closely, and the function
  * will be able to work more quickly.
  * <br><br>
  * If the supplied surface is of type spline and the underlying <tt>spl_sur</tt> is a
  * <tt>VBL_SURF</tt>, this API function makes up <i>n</i> four-sided approximating <tt>bs3_surfaces</tt>. A
  * pointer to an array of these <tt>bs3_surfaces</tt> is then returned, along with the
  * number of <tt>bs3_surfaces</tt> in the array, and the maximum internal and boundary fit
  * tolerances of the <tt>bs3_surfaces</tt>. If the supplied surface is not a spline, or if
  * it is a <tt>spline</tt> and the underlying <tt>spl_sur</tt> is not a VBL_SURF, this API function
  * returns a <tt>NULL</tt> <tt>bs3_surface</tt> pointer and sets the number of approximating
  * <tt>bs3_surfaces</tt> to zero.
  * <br><br>
  * <b>Errors:</b> A <tt>NULL</tt> pointer to a surface is specified. The <tt>spl_sur</tt> underlying the
  * surface is not a <tt>VBL_SURF</tt>.
  * <br><br>
  * <b>Effect:</b> Read-only
  * <br><br>
  * <b>Journal: </b> Not Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
  * <br><br>
 * @param vbl_sf
  * surface with underlying VBL_SURF.
  * @param interior_fit
  * achieved interior fit tolerance.
  * @param boundary_fit
  * achieved boundary fit tolerance.
  * @param bs3_array
  * array of returned bs3_surfaces.
  * @param n
  * number of returned bs3_surfaces.
  * @param approx_error
  * if <tt>TRUE</tt>, approximate the error.
  **/
DECL_KERN outcome api_make_VBL_output_surfaces(
    const surface* vbl_sf,
    double& interior_fit,
    double& boundary_fit,
    bs3_surface*& bs3_array,
    int& n,
    logical			approx_error = TRUE
);
/** @} */

/**
 * \addtogroup ACISMEMORYMANAGEMENT
 *
 * @{
 */

 /**
  * Sets the limit in bytes of how much stack ACIS may use.
  * <br><br>
  * <b>Role:</b> ACIS can monitor the size of the stack. This function sets the
  * limit in bytes of how much stack ACIS may use. If the limit is exceeded, ACIS
  * will trap, returning <tt>EXCESSIVE_RECURSION</tt>. Passing 0 results in no stack
  * monitoring.
  * <br><br>
  * <b>Effect:</b> Read-only
  * <br><br>
  * <b>Journal: </b> Not Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
  * <br><br>
 * @param limit
  * bytes of stack memory.
  **/
DECL_KERN outcome api_stackmon_limit(
    size_t limit
);
/** @} */

/**
 * \addtogroup ACISCHECKING
 *
 * @{
 */

 /**
  * Checks a face to see that it contains valid loops.
  * <br><br>
  * <b>Role:</b> This function checks that the direction of a <tt>FACE</tt>'s <tt>LOOP</tt>s are
  * correct. It eliminates the need to calculate the area of a face to determine the
  * validity of the face. (If the area calculation for a <tt>FACE</tt> was negative, it was
  * indicative of a problem usually in the direction of <tt>LOOP</tt>s.)
  * <br><br>
  * This API returns outcome to indicate if the input <tt>FACE</tt> contains invalid <tt>LOOP</tt>s.
  * An error message is contained in the outcome.
  * <br><br>
  * ai_info[0]: number of periphery loops<br>
  * ai_info[1]: number of holes<br>
  * ai_info[2]: number of u separation loops<br>
  * ai_info[3]: number of v separation loops<br>
  * ai_info[4]: number of unknown loops
  * <br><br>
  * <b>Effect:</b> System routine
  * <br><br>
  * <b>Journal: </b> Not Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
  * <br><br>
 * @param in_face
  * face to test.
  * @param ai_info
  * where test results stored.
  * @param ao
  * ACIS options.
  **/
DECL_KERN outcome api_check_face_loops(
    FACE* in_face,
    int          ai_info[5] = NULL,
    AcisOptions* ao = NULL);
/** @} */

/**
 * \addtogroup ACISQUERIES
 *
 * @{
 */

 /**
  * Determines the <tt>loop_type</tt> of a given loop.
  * <br><br>
  * <b>Role:</b> This API returns the type of a given loop. Types include:
  * <br><br>
  * <ul>
  * <li><tt>loop_unknown</tt></li>
  * <li><tt>loop_periphery</tt></li>
  * <li><tt>loop_hole</tt></li>
  * <li><tt>loop_separation</tt> (not used)</li>
  * <li><tt>loop_u_separation</tt></li>
  * <li><tt>loop_v_separation</tt></li>
  * <li><tt>loop_uv_separation</tt></li>
  * </ul><br>
  * For more information on the loop classification refer to the <i>Classification of Loops</i>
  * section of the <i>[Loop](http://doc.spatial.com/articles/l/o/o/Loop.html)</i> Technical Article.
  * <br><br>
  * <b>Effect:</b> Read-only
  * <br><br>
  * <b>Journal: </b> Not Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
  * <br><br>
 * @param in_loop
  * Loop to test.
  * @param type
  * Type of loop. Refer to @href loop_type for more details about loop types.
  * @param info
  * Array holding information about the loop. <br>
  * If type is <tt>u</tt> or <tt>v</tt> separation loop, then
  * <tt>info[0]</tt> is the number of seam crossing and
  * <tt>info[1]</tt> denotes if it is the upper or lower parameteric bound.<br>
  * If type is <tt>u</tt> separation loop,
  * <tt>info[1] == 1</tt> indicates the loop is the lower u bound of the face,
  * <tt>info[1] == -1</tt> indicates the loop is the upper u bound of the face,
  * <tt>info[1] == 0</tt> indicates the value has not been set.<br>
  * If type is <tt>v</tt> separation loop,
  * <tt>info[1] == -1</tt> indicates the loop is the lower v bound of the face,
  * <tt>info[1] == 1</tt> indicates the loop is the upper v bound of the face,
  * <tt>info[1] == 0</tt> indicates the value has not been set. <br>
  * If type is <tt>u_v</tt> separation loop, then <tt>info[0]</tt> is the number of
  * <tt>v</tt> seam crossings, and <tt>info[1]</tt> is the number of
  * <tt>u</tt> seam crossing.
  **/
DECL_KERN outcome api_loop_type(
    LOOP* in_loop,
    loop_type& type,
    int       info[2] = NULL
);
/** @} */

/**
 * \defgroup ACISTOLERANT Tolerant Geometry
 * \ingroup KERNAPI
 *
 */

 /**
  * Calculates the tolerance of an edge.
  * <br><br>
  * <b>Role:</b> This function calculates the tolerance of an <tt>EDGE</tt> or a <tt>TEDGE</tt> and
  * returns a tolerance value. It <i>does not</i> use the tolerance value on the <tt>TEDGE</tt>.
  * <br><br>
  * <b>Effect:</b> System routine
  * <br><br>
  * <b>Journal: </b> Not Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
  * <br><br>
 * @param edge
  * edge to test.
  * @param tol
  * resulting tolerance.
  * @param ao
  * ACIS options.
  **/
DECL_KERN outcome api_calculate_edge_tolerance(
    EDGE* edge,
    double& tol,
    AcisOptions* ao = NULL);

/**
 * Calculates the tolerance of a vertex.
 * <br><br>
 * <b>Role:</b> This function calculates the tolerance of a <tt>VERTEX</tt> or a <tt>TVERTEX</tt>
 * and returns a tolerance value. It <i>does not</i> use the tolerance value on the
 * <tt>TVERTEX</tt>.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param vertex
 * input vertex / tvertex.
 * @param tol
 * resulting tolerance.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_calculate_vertex_tolerance(
    VERTEX* vertex,
    double& tol,
    AcisOptions* ao = NULL);

/**
 * Updates the tolerance on an <tt>ENTITY</tt>.
 * <br><br>
 * <b>Role:</b> This function calculates and updates the tolerance of
 * the given entity and its children.
 * <br><br>
 * <b>Effect:</b> Changes model's derived data
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param this_entity
 * entity with tolerance.
 * @param updated
 * result <tt>TRUE</tt> is a tolerant entity updated.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_update_tolerance(
    ENTITY* this_entity,
    logical& updated,
    AcisOptions* ao = NULL);


/** @} */

/**
 * \addtogroup KERNAPI
 * @{
 */

 /**
  * A function pointer type used in various search routines.
  */
typedef logical(*is_fun)(const ENTITY*);
/** @} */

/**
 * \addtogroup ACISQUERIES
 * @{
 */

 /**
  * Determines if an <tt>ENTITY</tt> is an <tt>ANNOTATION</tt>.
  * <br><br>
  * @param ent
  * ENTITY to be queried.
  */
DECL_KERN logical is_ANNOTATION(const ENTITY* ent);

DECL_KERN logical is_ATTRIB_TAG(const ENTITY* ent);
DECL_KERN logical is_EE_LIST(const ENTITY* ent);
/** @} */

/**
 * \addtogroup ACISANNOTATIONS
 * @{
 */

 /**
  * Finds all annotation entities of a given type.
  * <br><br>
  * <b>Role:</b> When the annotations option is turned on, certain modeling
  * operations like blending or sweeping produce <tt>ANNOTATION</tt> class instances in
  * internal <tt>ENTITY_LIST</tt>. The <tt>api_find_annotations</tt> function can be used to acquire a
  * list of those annotations for user-defined processing. Generally, annotations
  * are cleared manually from the list by using <tt>api_clear_annotations</tt> before the
  * next modeling operation.
  * <br><br>
  * The flag for <tt>is_fun</tt> defaults to <tt>is_ANNOTATION</tt>. However, any <tt>is_</tt> function for a
  * class can be used. So, for example, to get the top vertex annotations from a
  * sweep operation, this function can be passed <tt>is_SWEEP_ANNO_VERTEX_TOP</tt> as an
  * argument.
  * <br><br>
  * <b>Effect:</b> Read-only
  * <br><br>
  * <b>Journal: </b> Not Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
  * <br><br>
 * @param list_annotation
  * list of annotation entities.
  * @param annno
  * test for specific type of annotation.
  * @param bb
  * obsolete, ignored.
  * @param ao
  * ACIS options.
  **/
DECL_KERN outcome api_find_annotations(
    ENTITY_LIST& list_annotation,
    is_fun          anno = is_ANNOTATION,
    BULLETIN_BOARD* bb = NULL,
    AcisOptions* ao = NULL);

/**
 * Traverses the active list of annotations and adds <tt>ATTRIB_ANNOTATION</tt>s to hook them to the annotated entities.
 * <br><br>
 * <b>Role:</b> The flag for <tt>is_fun</tt> defaults to <tt>is_ANNOTATION</tt>. However, any <tt>is</tt>
 * function for a class can be used. So, for example, to get the top vertex
 * annotations from a sweep operation, this function can be passed
 * <tt>is_SWEEP_ANNO_VERTEX_TOP</tt> as an argument.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param anno
 * type of annotation.
 * @param bb
 * obsolete, ignored.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_hook_annotations(
    is_fun          anno = is_ANNOTATION,
    BULLETIN_BOARD* bb = NULL,
    AcisOptions* ao = NULL);

/**
 * Traverses the active list of annotations and removes associated <tt>ATTRIB_ANNOTATION</tt>s.
 * <br><br>
 * <b>Role:</b> The function for <tt>is_fun</tt> defaults to <tt>is_ANNOTATION</tt>. However, any
 * <tt>is</tt> function for a class can be used. So, for example, to get the top vertex
 * annotations from a sweep operation, this function can be passed
 * <tt>is_SWEEP_ANNO_VERTEX_TOP</tt> as an argument.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
 * @param anno
 * Function pointer to the type of annotation.
 * @param bb
 * obsolete, ignored acis options.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_unhook_annotations(
    is_fun          anno = is_ANNOTATION,
    BULLETIN_BOARD* bb = NULL,
    AcisOptions* ao = NULL);

/**
 * Clears annotation entities from the currently active bulletin board.
 * <br><br>
 * <b>Role:</b> Searches the global list of annotations and loses them. This
 * should be called at some point after a modeling operation, such as sweeping or
 * blending. Once the annotation information has been handled, it must be cleared
 * from the active bulletin board using <tt>api_clear_annotations</tt> before the next
 * modeling operation. Ideally, the operation to be annotated should be wrapped in
 * an <tt>API_BEGIN</tt>/<tt>END</tt> block so the call to <tt>api_clear_annotations</tt> will restore the
 * bulletin board to a state as if annotations had never been created.
 * <br><br>
 * Not calling <tt>api_clear_annotations</tt> can lead to a bloated bulletin board as well
 * as incorrect links between separate modeling operations when option
 * <tt>unhook_annotations</tt> is <tt>FALSE</tt>.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_clear_annotations(
    AcisOptions* ao = NULL);

/** @} */

/**
 * \addtogroup ACISKERNLAWS
 * @{
 */

 /**
  * Creates a linear law given {a,b,f(a),f(b)}.
  * <br><br>
  * <b>Role:</b> Produces a linear polynomial with given boundary conditions for
  * both its output. The user supplies the boundary values <i>a</i> and <i>b</i>
  * and the desired output of the law at <i>a</i> and <i>b</i> (e.g., <tt>f_a</tt> and <tt>f_b</tt>).
  * The result is a linear polynomial meeting these boundary conditions.
  * <br><br>
  * <b>Effect:</b> Changes model
  * <br><br>
  * <b>Journal: </b> Not Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Modeler
  * <br><br>
 * @param aval
  * a value.
  * @param bval
  * b value.
  * @param faval
  * f at a.
  * @param fbval
  * f at b.
  * @param answer
  * ptr to law.
  * <br><br>
  * @see api_make_cubic, api_make_quintic
  **/
DECL_KERN outcome api_make_linear(double aval,
    double bval,
    double faval,
    double fbval,
    law*& answer);

DECL_KERN outcome gme_api_make_linear(double aval,
    double bval,
    double faval,
    double fbval,
    law*& answer);
/**
 * Creates a cubic law given {a,b,f(a),f(b),f'(a),f'(b)}.
 * <br><br>
 * <b>Role:</b> Produces a cubic polynomial with given boundary conditions for
 * both it and its first derivative. The user supplies the boundary values <i>a</i>
 * and <i>b</i>, the desired output of the law at <i>a</i> and <i>b</i>
 * (e.g., <tt>f_a</tt> and <tt>f_b</tt>), and the desired output of the first derivative at <i>a</i>
 * and <i>b</i> (e.g., <tt>df_a</tt> and <tt>df_b</tt>). The result is a cubic polynomial meeting
 * these boundary conditions.
 * <pre>
 * F(a)  = fa.
 * F(b)  = fb.
 * F'(a) = ffa.
 * F'(b) = ffb.</pre>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param aval
 * a value.
 * @param bval
 * b value.
 * @param faval
 * f at a.
 * @param fbval
 * f at b.
 * @param ffaval
 * deriv of f at a.
 * @param ffbval
 * deriv of f at b.
 * @param answer
 * ptr to law.
 * <br><br>
 * @see api_make_linear, api_make_quintic
 **/
DECL_KERN outcome api_make_cubic(double aval,
    double bval,
    double faval,
    double fbval,
    double ffaval,
    double ffbval,
    law*& answer);

DECL_KERN outcome gme_api_make_cubic(double aval,
    double bval,
    double faval,
    double fbval,
    double ffaval,
    double ffbval,
    law*& answer);
/**
 * Creates a quintic law given {a,b,f(a),f(b),f'(a),f'(b) f''(a) f''(b)}.
 * <br><br>
 * <b>Role:</b> Produces a quintic polynomial with given boundary conditions for
 * it, its first derivative, and its second derivative. The user supplies the
 * boundary values <i>a</i> and <i>b</i>, the desired output of the law at
 * <i>a</i> and <i>b</i> (e.g., <tt>f_a</tt> and <tt>f_b</tt>), the desired output of the first
 * derivative at <i>a</i> and <i>b</i> (e.g., <tt>df_a</tt> and <tt>df_b</tt>), and the desired
 * output of the second derivative at <i>a</i> and <i>b</i> (e.g., <tt>ddf_a</tt> and <tt>ddf_b</tt>).
 * The result is a quintic polynomial meeting these boundary conditions.
 * <pre>
 * F(a)   = fa.
 * F(b)   = fb.
 * F'(a)  = ffa.
 * F'(b)  = ffb.
 * F''(a) = fffa.
 * F''(b) = fffb.</pre>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param aval
 * a value.
 * @param bval
 * b value.
 * @param faval
 * f at a.
 * @param fbval
 * f at b.
 * @param ffaval
 * 1st deriv of f at a.
 * @param ffbval
 * 1st deriv of f at b.
 * @param fffaval
 * 2nd deriv of f at a.
 * @param fffbval
 * 2nd deriv of f at b.
 * @param answer
 * ptr to law.
 * <br><br>
 * @see api_make_cubic, api_make_linear
 **/
DECL_KERN outcome api_make_quintic(double aval,
    double bval,
    double faval,
    double fbval,
    double ffaval,
    double ffbval,
    double fffaval,
    double fffbval,
    law*& answer);
DECL_KERN outcome gme_api_make_quintic(double aval,
    double bval,
    double faval,
    double fbval,
    double ffaval,
    double ffbval,
    double fffaval,
    double fffbval,
    law*& answer);
/**
 * Creates a polynomial law.
 * <br><br>
 * <b>Role:</b> Given an array of coefficients and the maximum degree for the
 * polynomial, this creates a law that represents the associated polynomial.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Modeler
 * <br><br>
* @param coeff
 * array of coefficients.
 * @param degree
 * maximum degree of polynomial.
 * @param answer
 * ptr to law.
 **/
DECL_KERN outcome api_make_polynomial_law(double* coeff,
    int     degree,
    law*& answer);

DECL_KERN outcome gme_api_make_polynomial_law(double* coeff,
    int     degree,
    law*& answer);
/** @} */

/**
 * \addtogroup ACISBOXING
 *
 * @{
 */

 /**
  * Removes any cached bounding boxes from the selected <tt>BODY</tt> and its subparts (or just the selected entity if it is not a <tt>BODY</tt>).
  * When a subsequent operation queries the bounding box of the entity, either directly via <tt>api_get_entity_box</tt> or indirectly as
  * part of a more involved operation, the necessary bounding boxes will be recomputed and cached again.
  * <br><br>
  * <b>Note:</b> This operation has no effect on <tt>ASM_ASSEMBLY</tt> and <tt>ASM_MODEL_REF</tt> entities.
  * <br><br>
  * <b>Effect:</b> Changes model's derived data
  * <br><br>
  * <b>Journal: </b> Not Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
  * <br><br>
 * @param ent
  * entity of interest.
  * @param ao
  * ACIS options.
  **/
DECL_KERN outcome api_reset_boxes(ENTITY* ent, AcisOptions* ao = NULL);

/** @} */

/**
 * \addtogroup ACISENTITYOPS
 *
 * @{
 */

 /**
  * Creates a copy of an <tt>ENTITY</tt> all its associated subentities.
  * <br><br>
  * <b>Role:</b> This API copies the given <tt>ENTITY</tt> and all its associated
  * subentities, if any. Subentities are those that are <i>below</i> the given
  * <tt>ENTITY</tt> in the topological hierarchy. It <i>does not</i> copy entities
  * that are above the given <tt>ENTITY</tt>. The optional transformation is applied
  * to the copied entity, if applicable.
  * <br><br>
  * <b>Note:</b> This special-case function only operates on <tt>VERTEX</tt>, <tt>EDGE</tt>, <tt>COEDGE</tt>, <tt>WIRE</tt>,
  * <tt>LOOP</tt>, <tt>FACE</tt>, <tt>SHELL</tt>, and <tt>LUMP</tt> entities; for all other entity types, it calls
  * <tt>api_copy_entity</tt>.
  * <br><br>
  * <b>Note:</b> @href api_copy_entity copies all entities <b>above</b> and <b>below</b>
  * the input entity, while <tt>api_copy_entity_contents</tt> only copies all entities <b>below</b>
  * the input entity. If @href api_copy_entity is incorrectly used where <tt>api_copy_entity_contents</tt>
  * is intended, memory requirements could become unnecessarily and  adversely large.
  * <br><br>
  * <b>Important: This function was deprecated in R16. The API has been replaced by @href api_down_copy_entity.</b>
  * <br><br>
  * <b>Errors:</b> The pointer to an original entity is <tt>NULL</tt>.
  * <br><br>
  * <b>Effect:</b> Changes model
  * <br><br>
  * <b>Journal: </b> Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
  * <br><br>
 * @param in_ent
  * entity to be copied.
  * @param copy
  * copy returned.
  * @param tr
  * optional transformation.
  * @param ao
  * ACIS options.
  * <br><br>
  * @see api_copy_entity, api_deep_copy_entity
  **/
DECL_KERN outcome api_copy_entity_contents(
    ENTITY* in_ent,
    ENTITY*& copy,
    const SPAtransf& tr = *(const SPAtransf*)NULL_REF,
    AcisOptions* ao = NULL);

/** @} */

/**
 * \addtogroup INTRAPIPROJECT
 *
 * @{
 */

 /**
  * Projects a curve onto a surface.
  * <br><br>
  * <b>Role:</b> This API projects <tt>in_curve</tt> onto <tt>in_surface</tt>, returning the result
  * in <tt>curves_on_surface</tt> and <tt>num_curves</tt>.
  * <tt>in_range</tt> is the parameter range over which <tt>in_curve</tt> is
  * to be projected.
  * <br><br>
  * The input curve and surface should both be in working coordinates. They might not
  * be if their owning body contains a transformation, and may need to be converted.
  * See the Scheme command, <tt>curve:project-to-surface</tt>, for an example of how to do this.
  * <br><br>
  * Only those parts of the curve for which a perpendicular projection onto the
  * surface exists will be projected. Parts which can only be projected to the
  * boundaries of the surface will be excluded. Note that it is entirely possible
  * that no perpendicular projection exists for any part of the curve. In these
  * situations, the function will return a successful outcome, but <tt>curves_on_surface</tt>
  * will be null. This allows an application to distinguish between a failed projection
  * and a non-existent projection.
  * <br><br>
  * <b>Errors:</b> Curve or surface not in working coordinates.
  * <br><br>
  * <b>Limitations:</b>
  * <br><br>
  * <ol>
  * <li>This function does <b>not</b> guarantee to return legal ACIS curves in all
  * circumstances - for instance, the curves may have discontinuities, be
  * degenerate or may self-intersect.</li>
  * </ol><br>
  * <b>Effect:</b> Read-only
  * <br><br>
  * <b>Journal: </b> Not Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
  * <br><br>
 * @param in_curve
  * curve to project.
  * @param in_range
  * parameter range over which to project <tt>in_curve</tt>.
  * @param in_surface
  * surface to project curve on to.
  * @param curves_on_surface
  * array of projected curves, containing resulting curve projected onto surface.
  * @param num_curves
  * number of curves in result.
  * @param ao
  * ACIS options.
  **/
DECL_KERN outcome api_project_curve_to_surface(const curve& in_curve,
    const SPAinterval& in_range,
    const surface& in_surface,
    curve**& curves_on_surface,
    int& num_curves,
    AcisOptions* ao = NULL);

/**
 * @nodoc
 * Projects a curve onto a surface.
 * <br><br>
 * <b>Role:</b> This API projects <tt>in_curve</tt> onto <tt>in_surface</tt>, returning the result
 * in <tt>curve_on_surface</tt>. <tt>in_range</tt> is the parameter range over which <tt>in_curve</tt> is
 * to be projected.
 * <br><br>
 * This API is deprecated. Please use the alternative version of the API
 * which can return multiple curves.
 * <b>Errors:</b> Curve or surface not in working coordinates.
 * <br><br>
 * <b>Limitations:</b>
 * <br><br>
 * <ol>
 * <li>If the curve has a perpendicular projection onto the surface
 * over more than one distinct interval, the function will fail, as it can only
 * return a single output curve.</li>
 * <li>This function does <b>not</b> guarantee to return a legal ACIS curve in all
 * circumstances - for instance, the curve may have discontinuities, be
 * degenerate or may self-intersect.</li>
 * </ol><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param in_curve
 * curve to project.
 * @param in_range
 * parameter range over which to project in_curve.
 * @param in_surface
 * surface to project curve on to.
 * @param curve_on_surface
 * resulting curve projected onto surface.
 * @param ao
 * ACIS options.
 */
DECL_KERN outcome api_project_curve_to_surface(const curve& in_curve,
    const SPAinterval& in_range,
    const surface& in_surface,
    curve*& curve_on_surface,
    AcisOptions* ao = NULL);

/** @} */

/**
 * \addtogroup ACISSAVERESTORE
 * @{
 */

 /**
  * Saves the current state of global variables into a text file.
  * <br><br>
  * <b>Role:</b> This API saves the current states of global variables such as
  * global options and global tolerances to a text file. You may use
  * this function to save a specific state of your application to a file and load
  * the state through the file into Scheme AIDE to compare the behaviors between
  * your application and the test applications.
  * <br><br>
  * <b>Effect:</b> Read-only
  * <br><br>
  * <b>Journal: </b> Not Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
  * <br><br>
 * @param file_ptr
  * file descriptor.
  * @param ao
  * ACIS options.
  * <br><br>
  * @see api_load_state
  **/
DECL_KERN outcome api_save_state(FILE* file_ptr, AcisOptions* ao = NULL);

/**
 * Loads the state of global variables from a given text file.
 * <br><br>
 * <b>Role:</b> This API loads the states of global variables such as global options
 * and global tolerances from a given text file. You may use
 * this function with @href api_save_state to compare the behaviors between your
 * application and Scheme AIDE.
 * <br><br>
 * <b>Effect:</b> Changes the state of the modeler, but not the model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param file_ptr
 * file descriptor.
 * @param ao
 * ACIS options.
 * <br><br>
 * @see api_save_state
 **/
DECL_KERN outcome api_load_state(FILE* file_ptr, AcisOptions* ao = NULL);

/** @} */

/**
 * \addtogroup ACISHISTORY
 * @{
 */

 /**
  * Returns a newly created <tt>HISTORY_STREAM</tt> on the heap.
  * <br><br>
  * <b>Effect:</b> System routine
  * <br><br>
  * <b>Journal: </b> Not Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
  * <br><br>
 * @param hs
  * created history stream.
  **/
DECL_KERN outcome api_create_history(HISTORY_STREAM*& hs);

/**
 * Sets the input <tt>HISTORY_STREAM</tt> to be the default the history stream.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param hs
 * input history stream.
 **/
DECL_KERN outcome api_set_default_history(HISTORY_STREAM* hs);

/**
 * Returns the default <tt>HISTORY_STREAM</tt>.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param default_hs
 * default history stream.
 **/
DECL_KERN outcome api_get_default_history(HISTORY_STREAM*& default_hs);

/**
 * Deletes the <tt>HISTORY_STREAM</tt> and all <tt>ENTITY</tt>s in the stream.
 * <br><br>
 * <b>Role:</b> Deletes the <tt>HISTORY_STREAM</tt> and all <tt>ENTITY</tt>s associated with
 * <tt>BULLETIN</tt> on the <tt>HISTORY_STREAM</tt>. Therefore, no <tt>ENTITY</tt>s will be deleted when
 * logging is off. Uses the default <tt>HISTORY_STREAM</tt> if none is supplied.
 * <br><br>
 * <b>Errors:</b> Fails when unable to remove all <tt>ENTITY</tt>s referred to in the stream.
 * <br><br>
 * <b>Limitations:</b> Logging must be used.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param hs
 * input history stream.
 **/
DECL_KERN outcome api_delete_history(HISTORY_STREAM* hs = NULL);

/**
 * Returns the <tt>HISTORY_STREAM</tt> in which the <tt>ENTITY</tt> lives.
 * <br><br>
 * <b>Limitations:</b> Logging must be used.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param ent
 * input entity.
 * @param hs
 * returned history stream.
 **/
DECL_KERN outcome api_get_history_from_entity(
    ENTITY* ent,
    HISTORY_STREAM*& hs
);

/**
 * Returns the active <tt>DELTA_STATE</tt> in the given <tt>HISTORY_STREAM</tt>.
 * <br><br>
 * <b>Role:</b> The active <tt>DELTA_STATE</tt> is either the most recently closed state in the stream,
 * made by calling <tt>note_state</tt>, or the state just rolled to. If no <tt>HISTORY_STREAM</tt> is
 * supplied, the default stream is used.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param active_ds
 * returned delta state.
 * @param hs
 * input history stream.
 **/
DECL_KERN outcome api_get_active_state(
    DELTA_STATE*& active_ds,
    HISTORY_STREAM* hs = NULL
);

/**
 * Returns <tt>TRUE</tt> if the given <tt>DELTA_STATE</tt> is in the <tt>HISTORY_STREAM</tt>.
 * <br><br>
 * <b>Role:</b> If no <tt>HISTORY_STREAM</tt> is supplied, the default stream is used.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param ds
 * input to test.
 * @param change_state_possible
 * <tt>TRUE</tt> if state is valid.
 * @param hs
 * input history stream.
 **/
DECL_KERN outcome api_query_state_validity(
    DELTA_STATE* ds,
    logical& change_state_possible,
    HISTORY_STREAM* hs = NULL
);

/**
 * Returns a <tt>DELTA_STATE</tt> identified by the given id.
 * <br><br>
 * <b>Role:</b> Returns the pointer to the <tt>DELTA_STATE</tt> identified by <tt>id</tt>. If no
 * <tt>HISTORY_STREAM</tt> is specified, the default stream is used. If the <tt>DELTA_STATE</tt>
 * corresponding to the <tt>id</tt> is not in the stream, a <tt>NULL</tt> pointer is returned.
 * <br><br>
 * <b>Errors:</b> <tt>id</tt> is not valid in the given stream.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param id
 * ID of the <tt>DELTA_STATE</tt>.
 * @param returned_ds
 * returned <tt>DELTA_STATE</tt>.
 * @param hs
 * history where the ENTITY lives.
 **/
DECL_KERN outcome api_get_state_from_id(
    STATE_ID        id,
    DELTA_STATE*& returned_ds,
    HISTORY_STREAM* hs = NULL
);

/**
 * Returns a unique integer identifier, in a particular <tt>HISTORY_STREAM</tt>, for a given <tt>DELTA_STATE</tt>.
 * <br><br>
 * <b>Role:</b> This ID number (tag) is saved with the <tt>HISTORY_STREAM</tt> and
 * does not change on restore.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param ds
 * <tt>DELTA_STATE</tt> for which id tag is requested.
 * @param id
 * returned ID.
 **/
DECL_KERN outcome api_get_state_id(
    DELTA_STATE* ds,
    STATE_ID& id
);

/**
 * Returns the pointer to the <tt>ENTITY</tt> identified by the given <tt>id</tt>.
 * <br><br>
 * <b>Role:</b> If no <tt>HISTORY_STREAM</tt> is specified, the default stream is used.
 * If the <tt>ENTITY</tt> corresponding to the <tt>id</tt> is not alive, a <tt>NULL</tt> pointer
 * is returned.
 * <br><br>
 * <b>Errors:</b> <tt>id</tt> is not valid in the given stream.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param id
 * ID of the <tt>ENTITY</tt>.
 * @param returned_ent
 * returned <tt>ENTITY</tt>.
 * @param hs
 * history where the <tt>ENTITY</tt> lives.
 **/
DECL_KERN outcome api_get_entity_from_id(
    tag_id_type     id,
    ENTITY*& returned_ent,
    HISTORY_STREAM* hs = NULL
);

/**
 * Returns a unique integer identifier (ID number) for a given <tt>ENTITY</tt>.
 * <br><br>
 * <b>Role:</b> Returns a unique integer, in a particular <tt>HISTORY_STREAM</tt>, for a
 * given <tt>ENTITY</tt>. This ID number (tag) is evaluated lazily but, once requested, is
 * saved with the <tt>HISTORY_STREAM</tt> and does not change on restore.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param ent
 * <tt>ENTITY</tt> for which id tag is requested.
 * @param id
 * returned ID.
 **/
DECL_KERN outcome api_get_entity_id(
    ENTITY* ent,
    tag_id_type& id
);

/**
 * Checks all <tt>HISTORY_STREAM</tt>s for problems.
 * <br><br>
 * <b>Role:</b> Checks all <tt>HISTORY_STREAM</tt>s for mixing and improper entity IDs.
 * Problems are reported to <tt>fptr</tt>, standard output by default, and <tt>HISTORY_STREAM</tt>s
 * with errors are returned in the <tt>insane_list</tt>, if non-<tt>NULL</tt>.
 * <br><br>
 * <b>Effect:</b> System routine
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param insane_list
 * list of questionable streams.
 * @param fptr
 * file for check output.
 **/
DECL_KERN outcome api_check_histories(
    HISTORY_STREAM_LIST* insane_list = NULL,
    FILE* fptr = stdout
);

/**
 * Deletes the current <tt>DELTA_STATE</tt> and rolls model to the state before the current state.
 * <br><br>
 * <b>Role:</b> Deletes the current <tt>DELTA_STATE</tt> rolling the model to the state
 * before construction of the current state was started.
 * <br><br>
 * If no stream is supplied, the default stream is used.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param hs
 * use default stream if NULL.
 **/
DECL_KERN outcome api_abort_state(HISTORY_STREAM* hs = NULL);

/** @} */

/**
 * \defgroup ACISVERSIONING Versioning
 * \ingroup ACISAPIBEHAVIOR
 *
 */

 /**
  * \defgroup ACISVERSIONING
  *
  * @{
  */

  /**
   * Gets the version tag from an <tt>AcisVersion</tt>.
   * <br><br>
   * <b>Role:</b> Returns the requested version tag.
   * <br><br>
   * <b>Effect:</b> Read-only
   * <br><br>
   * <b>Journal: </b> Not Available
   * <br><br>
   * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
   * <br><br>
  * @param av
   * ACIS version object.
   * @param tag
   * tag of ACIS version object.
   **/
DECL_KERN outcome api_get_version_tag(
    AcisVersion* av,
    int& tag);

/**
 * Gets the version tag from the individual point information.
 * <br><br>
 * <b>Role:</b> Returns the requested version tag.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param major
 * major version number.
 * @param minor
 * minor version number.
 * @param point
 * point version number.
 * @param tag
 * tag of this version.
 **/
DECL_KERN outcome api_get_version_tag(
    int major,
    int minor,
    int point,
    int& tag);

/**
 * Gets the version tag from the current <tt>AcisVersion</tt>.
 * <br><br>
 * <b>Role:</b> Returns the requested version tag.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param tag
 * tag of current ACIS executable.
 **/
DECL_KERN outcome api_get_version_tag(int& tag);

/**
 * Makes an <tt>AcisVersion</tt> object from a version tag integer.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param av
 * ACIS version object from following input.
 * @param tag
 * input tag.
 **/
DECL_KERN outcome api_make_version_object(
    AcisVersion*& av,
    int tag);

/**
 * Makes an <tt>AcisVersion</tt> object from individual point integers.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param av
 * ACIS version object from following input.
 * @param major
 * major version number.
 * @param minor
 * minor version number.
 * @param point
 * point version number.
 **/
DECL_KERN outcome api_make_version_object(
    AcisVersion*& av,
    int major,
    int minor,
    int point);

/**
 * Makes an current <tt>AcisVersion</tt> object.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param av
 * ACIS version object of current executable.
 **/
DECL_KERN outcome api_make_version_object(
    AcisVersion*& av);

/** @} */

/**
 * \addtogroup ACISMEMORYMANAGEMENT
 * @{
 */

 /**
  * Supplies the specific minimization actions to <tt>api_minimize_entities</tt>.
  * <br><br>
  * <b>Role:</b> Use this class to specify custom memory minimizations options to  @href api_minimize_entities.
  * In general, setting the members to zero disables the specific action,
  * setting them to one enables the action. The <tt>approx_action</tt> flag has
  * precedence over the <tt>geometry_action</tt> flag. Enabling the <tt>approx_action</tt> flag causes approximations
  * that can be recalculated (lazy geometry) to be deleted instead of paged. Non-lazy geometry can only be
  * paged and therefore only evaluates the <tt>geometry_action</tt> flag.
  * The <tt>history_action</tt> flag, which enables minimization of surfaces and curves when they transition into history,
  * and the <tt>restore_action</tt> flag, which enables minimization of surfaces and curves when they are restored from an
  * ACIS save file, are only evaluated in the default <tt>minimize_options</tt> object. They work in conjuction with the
  * the <tt>geometry_action</tt> and <tt>approx_action</tt> flags, and page geometry according to the values of those flags. For example,
  * if the <tt>geometry_action</tt> flag is 0, <tt>history_action</tt> and <tt>restore_action</tt> have no effect.
  * @see api_minimize_entities, api_set_default_minimize_options
  */
class DECL_KERN minimize_options : public ACIS_OBJECT {
public:
    /**
     * Setting <tt>geometry_action</tt> to 1 causes geometry to be paged to disk.
     * This is enabled by default.
     */
    int geometry_action;
    /**
     * Setting <tt>approx_action</tt> to 1 causes approximate geometry to be deleted instead of paged.
     * This is disabled by default.
     */
    int approx_action;
    /**
     * Setting <tt>history_action</tt> to 1 enables minimization to the level specified by <tt>geometry_action</tt> and <tt>approx_action</tt>
     * when surfaces and curves transition into history.
     * This is disabled by default.
     */
    int history_action;
    /**
     * Setting restore_action to 1 enables minimization to the level specified by <tt>geometry_action</tt> and <tt>approx_action</tt>
     * when surfaces and curves are restored.
     * This is disabled by default.
     */
    int restore_action;

public:

    /**
     * Constructs a default <tt>minimize_options</tt> object and initializes its member data to default values.
     */
    minimize_options();
};

/**
 * Attempts to minimize the working memory footprint of the input entities.
 * <br><br>
 * <b>Role:</b> Applies the default minimization actions, or the specific actions of the optional
 * @href minimize_options object, on the geometry of the input entities.
 * This API allows you to page geometric data to disk and to delete lazy approximations,
 * which can drastically reduce the working memory footprint of the model.
 * The paged geometric data is automatically unpaged and the lazy data is automatically recalculated when needed.
 * The input entities are scanned downward, which allows you to be very specific as to which geometry is to be affected.
 * When the input entity is a face, for example, then only the geometry of that face's edges and coedges are minimized.
 * In contrast, given a body input entity, all the geometry is to be minimized.
 * The location and name of the page file can be set with the @href initialize_page_system function.
 * The effectiveness of the paging system can be evaluated with the @href get_page_statistics function.
 * <br><br>
 * <b>Note:</b> The default actions enable paging.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param list
 * input entities
 * @param mo
 * minimization options
 * @param ao
 * ACIS options
 * @see minimize_options, initialize_page_system, get_page_statistics
 **/
DECL_KERN outcome api_minimize_entities(
    ENTITY_LIST* list,
    minimize_options* mo = NULL,
    AcisOptions* ao = NULL);

/**
 * Set the default minimization actions.
 * <br><br>
 * <b>Role:</b> The <tt>minimize_options</tt> object supplied to api_minimize_entities is optional.
 * The default actions are used when not supplied to the API, when surfaces and curves transition into history, and when
 * curves and surfaces are restored from an ACIS save file.
 * When a surface is lost, for example, its geometric data will be minimized based upon the default minimize options actions.
 * The default minimize options enable the paging of all geometry, including lazy approximations.
 * Specifying default actions with this API will enable automatic history minimization if the page system is initialized.
 * <br><br>
 * <b>Note:</b> The default actions enable paging.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param mo
 * minimization options
 * @see api_minimize_entities, minimize_options, initialize_page_system, get_page_statistics
 **/
DECL_KERN outcome api_set_default_minimize_options(minimize_options* mo);

/** @} */

/**
 * \addtogroup CSTRMAKEFACEAPI
 *
 * @{
 */

 /**
  * Create a <tt>FACE</tt> that spans an entire surface.
  * <br><br>
  * <b>Role:</b> The <tt>surface</tt> object is not a topological entity and, therefore, cannot be saved.  Constructing
  * a <tt>FACE</tt> based upon <tt>surface</tt> the allows the geometry to be saved for later use/access.  Because the
  * only input to this function is lower case geometry, no journaling is available.
  * <br><br>
  * <b>Effect:</b> Changes model
  * <br><br>
  * <b>Journal: </b> Not Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
  * <br><br>
 * @param from_surface
  * pointer to a surface. The caller retains ownership of <tt>from_surface</tt>, and must ensure it's destruction.
  * @param face_from_surface
  * returned pointer to newly constructed <tt>FACE</tt> that contains a (deep) copy of <tt>from_surface</tt>.
  * @param ao
  * ACIS options
  * @see minimize_options, initialize_page_system, get_page_statistics
  **/
DECL_KERN outcome api_make_face_from_surface(surface* from_surface,
    FACE*& face_from_surface,
    AcisOptions* ao = NULL);

/**
 * Constructs a new <tt>FACE</tt> from a spline surface.
 * <br><br>
 * <b>Role:</b> Constructs a single face which is the whole of a given %spline
 * %surface. This handles %spline surfaces closed in one or both directions.
 * For periodic surfaces seam edges are created at the boundaries, if the global
 * option <tt>split_periodic_splines</tt> is <tt>TRUE</tt>. They will be suppressed
 * if <tt>split_periodic_splines</tt> is <tt>FALSE</tt>.
 * <br><br>
 * In all cases, the resulting face has one loop. The number of coedges is 4 minus
 * the number of singularities on the %surface.
 * <br><br>
 * In the open/open case there are four edges and four vertices, in the open/closed
 * and closed/open cases there are three edges and two vertices, and in the
 * closed/closed case there are two edges and one vertex.
 * <br><br>
 * The face normal always follows the sense of the %spline %surface.
 * <br><br>
 * The API function @href api_make_face_from_surface is the preferred interface for
 * creating a face from any %surface; however, this direct interface function may
 * provide advantages if the boundary curves are known. This function also allows
 * a portion of the %surface to used by specifying a par_box.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * @param this_surface
 * The given %surface.
 * @param lv
 * The low v %curve.
 * @param hv
 * The high v %curve.
 * @param lu
 * The low u %curve.
 * @param hu
 * The high u %curve.
 * @param pb
 * Par_box to use.
 **/
DECL_KERN FACE* make_face_spline(
    surface const& this_surface,		// spline geometry supporting face
    curve const& lv = *(curve const*)NULL_REF,		// low v curve
    curve const& hv = *(curve const*)NULL_REF,		// high v curve
    curve const& lu = *(curve const*)NULL_REF,		// low u curve
    curve const& hu = *(curve const*)NULL_REF,		// high u curve
    SPApar_box const& pb = *(SPApar_box const*)NULL_REF  // uv bound to use
);

/** @} */

/**
 * \addtogroup ACISLISTS
 *
 * @{
 */

 /**
  * Gets a list of all entities in a collection.
  * <br><br>
  * <b>Errors:</b> Pointer to collection is <tt>NULL</tt> or not to a <tt>SPACOLLECTION</tt>.
  * <br><br>
  * <b>Effect:</b> Read-only
  * <br><br>
  * <b>Journal: </b> Not Available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
  * <br><br>
 * @param col
  * collection to be examined.
  * @param list
  * returned entities in the collection.
  * @param ao
  * ACIS options.
  **/
DECL_KERN outcome api_return_collection_ents(
    SPACOLLECTION* col,
    ENTITY_LIST& list,
    AcisOptions* ao = NULL);

/**
 * Gets a list of all collections in which an entity participates.
 * <br><br>
 * <b>Role:</b> This API retrieves a list of all collections that an entity is in.
 * <br><br>
 * <b>Errors:</b> <tt>NULL</tt> entity.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal: </b> Not Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param ent
 * entity of interest.
 * @param list
 * returned collections containing the entity.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_return_collections(
    ENTITY* ent,
    ENTITY_LIST& list,
    AcisOptions* ao = NULL);

/**
 * Adds a list of entities to a collection.
 * <br><br>
 * <b>Role:</b> This API adds the list of entities to the collection's list of entities
 * If the entity is already in collection, it does nothing.
 * <br><br>
 * <b>Errors:</b> Pointer to collection is <tt>NULL</tt> or not to a <tt>SPACOLLECTION</tt>.
 * Any <tt>ENTITY</tt> in ent_list is <tt>NULL</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param ent_list
 * entities to be added.
 * @param col
 * collection containing entities.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_add_to_collection(
    ENTITY_LIST& ent_list,
    SPACOLLECTION* col,
    AcisOptions* ao = NULL);


/**
 * Removes a list of entities from a collection.
 * <br><br>
 * <b>Role:</b> This API removes the list of entities from the collection's list of entities
 * If the entity is not in collection, it does nothing.
 * <br><br>
 * <b>Errors:</b> Any <tt>ENTITY</tt> in ent_list is <tt>NULL</tt>.
 * Pointer to collection is <tt>NULL</tt> or not to a <tt>SPACOLLECTION</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal: </b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param ent_list
 * entities to be removed.
 * @param col
 * collection containing entities.
 * @param ao
 * ACIS options.
 **/
DECL_KERN outcome api_remove_from_collection(
    ENTITY_LIST& ent_list,
    SPACOLLECTION* col,
    AcisOptions* ao = NULL);

/**
 * Deletes the member entities within the collection and then delete the collection itself.
 * <br><br>
 * <b>Role:</b> This API deletes the (deletable) member entities within the collection and then
 * deletes the collection itself.
 * <br><br>
 * If you want to un-collect the entities (delete the collection but not its members), use
 * api_del_entity on the collection.
 * <br><br>
 * <b>Errors:</b> Pointer to collection is <tt>NULL</tt> or not to a <tt>SPACOLLECTION</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal:</b> Available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param col
 * collection containing entities
 * @param ao
 * ACIS options
 **/

DECL_KERN outcome api_delete_collection_entities(SPACOLLECTION* col, AcisOptions* ao = NULL);

/** @} */

/**
 * \addtogroup ACISGEOMETRY
 *
 * @{
 */

 /**
  * @nodoc
  * Causes the geometry on one or more spline surfaces to be shared with that on another.
  * <br><br>
  * <b>Role:</b> This API is intended to allow you to restore geometry sharing among
  * spline surfaces restored from multiple files.  When it is invoked, all surfaces that share
  * geometry with the surface <tt>to_be_replaced</tt> have their geometries replaced by that
  * on the surface <tt>existing</tt>.  <i>It is up to you to ensure that the two geometries
  * are the same, so that a valid model is the result.</i>
  * <br><br>
  * This API may be used as follows: (1) Read the first file; (2) set the option
  * <tt>keep_geometry_sharing_info</tt> to <tt>TRUE</tt>; (3) read the second file; (4)
  * invoke the API on surfaces known to be the same; and (5) call
  * @href api_clear_geometry_sharing_info to free the sharing information from memory.
  * <br><br>
  * <b>Errors:</b> The option <tt>keep_geometry_sharing_info</tt> is set to <tt>FALSE</tt>.
  * <br><br>
  * <b>Effect:</b> Changes model
  * <br><br>
  * <b>Journal:</b> Not available
  * <br><br>
  * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
  * <br><br>
 * @param to_be_replaced
  * surface whose geometry it to be replaced
  * @param existing
  * surface whose geometry is to be the replacement
  * @param ao
  * ACIS options
  **/
DECL_KERN outcome api_share_geometry(surface* to_be_replaced, surface* existing, AcisOptions* ao = NULL);
/**
 * @nodoc
 * Causes the geometry on one or more spline curves to be shared with that on another.
 * <br><br>
 * <b>Role:</b> This API is intended to allow the user to restore geometry sharing among
 * spline curves restored from multiple files.  When it is invoked, all curves that share
 * geometry with the curve <tt>to_be_replaced</tt> have their geometries replaced by that
 * on the curve <tt>existing</tt>.  <i>It is up to the user to ensure that the two geometries
 * are the same, so that a valid model is the result.</i>
 * <br><br>
 * This API may be used as follows: (1) Read the first file; (2) set the option
 * <tt>keep_geometry_sharing_info</tt> to <tt>TRUE</tt>; (3) read the second file; (4)
 * invoke the API on curves known to be the same; and (5) call
 * @href api_clear_geometry_sharing_info to free the sharing information from memory.
 * <br><br>
 * <b>Errors:</b> The option <tt>keep_geometry_sharing_info</tt> is set to <tt>FALSE</tt>.
 * <br><br>
 * <b>Effect:</b> Changes model
 * <br><br>
 * <b>Journal:</b> Not available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param to_be_replaced
 * curve whose geometry it to be replaced
 * @param existing
 * curve whose geometry is to be the replacement
 * @param ao
 * ACIS options
 **/
DECL_KERN outcome api_share_geometry(curve* to_be_replaced, curve* existing, AcisOptions* ao = NULL);
/**
 * @nodoc
 * Clears geometry-sharing information from memory.
 * <br><br>
 * <b>Role:</b> Setting the option <tt>keep_geometry_sharing_info</tt> to <tt>TRUE</tt>
 * allows ACIS to collect geometry-sharing information when a file is read in, for use
 * by the APIs @href api_share_geometry.  Call this function to free the memory
 * holding this information when it is no longer needed.
 * <br><br>
 * <b>Errors:</b> None.
 * <br><br>
 * <b>Effect:</b> Read-only
 * <br><br>
 * <b>Journal:</b> Not available
 * <br><br>
 * <b>Product(s):</b> 3D ACIS Exchange, 3D Viz Exchange, 3D ACIS Modeler
 * <br><br>
* @param ao
 * ACIS options
 **/
DECL_KERN outcome api_clear_geometry_sharing_info(AcisOptions* ao = NULL);

/**
* @nodoc
*/
DECL_KERN outcome api_get_spaentity_id(
    ENTITY* ent,
    uint64_t& id,
    AcisOptions* ao = NULL
);

/**
* @nodoc
*/
DECL_KERN outcome api_get_entity_from_spaentity_id(
    uint64_t     id,
    ENTITY*& returned_ent,
    AcisOptions* ao = NULL
);


/** @} */

#endif

