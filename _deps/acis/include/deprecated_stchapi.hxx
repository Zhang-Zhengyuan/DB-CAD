/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#ifndef deprecated_stchapi_hxx
#define deprecated_stchapi_hxx

#include "dcl_stitch.h"
#include "api.hxx"
#include "acis_options.hxx"

class outcome;
class BODY;
class stitch_options;
class ENTITY_LIST;


/**
 * @nodoc
 * <b>Obsolete:</b> Instead, use outcome api_stitch( const ENTITY_LIST& to_be_stitched,
 * ENTITY_LIST& output_bodies, ENTITY_LIST& new_bodies, stitch_options* sopts,
 * AcisOptions* aopts = NULL ).
 * <br><br>
 *
 * Attempts to stitch a list of faces and bodies into a single body.
 * <br><br>
 * <b>Role:</b> This API attempts to stitch all the entities in the entity list
 * with the given body. The entities can be faces or bodies. The stitched body can
 * either be a single lump or multiple lumps. Each lump can be a sheet lump or a
 * solid lump. The body passed to the API should not be <tt>NULL</tt>, but it can
 * be a body without any lumps. When the API returns, all the entities which were
 * considered for stitching will be contained inside the given body.
 * Any faces which failed to stitch or only partially stitched will be contained
 * in the body as lumps containing isolated faces or sheets, so <tt>api_separate_body()</tt>
 * should be call on the body after the API returns.<br><br>
 *
 * The API can stitch the entities in three modes, exact, tolerant, and restricted tolerant.<br><br>
 *
 * If an object of type <tt>exact_stitch_options</tt> is passed through the
 * <tt>sopts</tt> pointer, then the API stitches faces only if their edges are
 * close enough to be within the ACIS tolerance <tt>SPAresabs</tt>. In this
 * mode(exact stitching), the API behavior is exactly the same as the behavior
 * of @href api_stitch found in Boolean Component.<br><br>
 *
 * If an object of type <tt>tolerant_stitch_options</tt> is passed through the
 * <tt>sopts</tt> pointer, then the API is in tolerant mode. It attempts to stitch
 * faces even if the edges are not close enough to be within the ACIS tolerance
 * <tt>SPAresabs</tt>, but are "reasonably" close. In this case, the API may
 * convert some edges to tolerant edges during stitching. In this mode, the API will
 * also remove zero-area faces lying in the regions that are likely to be stitched.
 * <br><br>
 * In R10 and later versions, if coincident faces are detected during the tolerant
 * stitching process, <tt>api_stitch</tt> will fail with the error <tt>COINCIDENT_FACE</tt>.
 * For pre-R10 versions of the call to <tt>api_stitch</tt>, passing in coincident
 * faces will lead to undefined behavior.
 * <br><br>
 * In R11 and later versions, if coincident faces are detected during the tolerant
 * stitching process (including both <tt>TOLERANT_STITCH</tt> or <tt>EDGE_TOLERANT_STITCH</tt>
 * modes), then <tt>api_stitch</tt>, depending upon the mode set in
 * <tt>tolerant_stitch_options</tt> object passed to <tt>api_stitch</tt>, will
 * behave in the following ways:
 * <br><br>
 * <tt>SPASTITCH_COIN_SKIP</tt> mode: Under this option, if <tt>api_stitch</tt>
 * comes across a pair of coincident faces, then the API will not stitch the two
 * faces together along the edge, and stitching continues. The coincident face pair
 * is recorded internally and the stitcher throws a warning of <tt>COINCIDENT_FACE</tt>.
 * The API optionally provides the caller of the API with a list of one or more
 * face-clusters each of which contains a set of coincident faces detected during
 * stitching. For example, if face A is coincident with face B, and face A coincident
 * with face C, then faces A, B, C form one cluster. The exact methods to obtain
 * the clusters of coincident faces can be found in the description of
 * <tt>tolerant_stitch_options</tt> class.<br><br>
 * <tt>SPASTITCH_COIN_STITCH</tt> mode: Under this option, if <tt>api_stitch</tt>
 * comes across a pair of coincident faces, then <tt>api_stitch</tt> will stitch
 * the two faces together along the edge and stitching continues. The coincident
 * face pair is recorded internally and the stitcher throws a warning of
 * <tt>COINCIDENT_FACE</tt>. The API optionally provides the caller of the API
 * with a list of one or more face-clusters each of which contains a set of coincident
 * faces detected during stitching. For example, if face A is coincident with face B,
 * and face A coincident with face C, then faces A, B, C form one cluster. The
 * exact methods to obtain the clusters of coincident faces can be found in the
 * description of <tt>tolerant_stitch_options</tt> class. Since this produces
 * back-to-back faces, it is likely downstream ACIS operations will fail.
 * So this mode is strongly discouraged. <br><br>
 * <tt>SPASTITCH_COIN_ERROR</tt> mode: Under this option, if <tt>api_stitch</tt>
 * comes across a pair of coincident faces, then it will immediately return with
 * the error <tt>COINCIDENT_FACE</tt>. The state of the model is rolled back
 * to the unstitched state.
 * <br><br>
 * If an object of type <tt>edge_tolstitch_options</tt> is passed through the
 * <tt>sopts</tt> pointer, then the API is in restricted tolerant mode.  This is
 * the same as tolerant mode, except that the <tt>to_be_stitched</tt> list must
 * contain a list of <tt>EDGEs</tt> (rather than <tt>FACE</tt> or <tt>BODYs</tt>)
 * and only the <tt>EDGEs</tt> appearing in the list are candidates for stitching.
 * In this case, the incoming model must not have any stitch attributes attached;
 * users can request the api to search for and remove these incoming attributes
 * by setting a flag in the <tt>edge_tolstitch_options</tt> object.  Another flag
 * controls whether <tt>api_stitch</tt> checks for inverted (i.e. void) bodies at
 * the end of stitching; by default the face normals are adjusted so that the
 * stitched model will not be a void. If users are confident that all the face normals on the
 * incoming faces are already correct, then this functionality can be switched off.
 * <br><br>
 *
 * In R12 and later versions, this API has a <b>fail-safe</b> behavior (i.e. it attempts to do
 * as much as possible and not fail, even in cases when it encounters geometry and topology
 * related errors) under <tt>TOLERANT_STITCH</tt> and <tt>EDGE_TOLERANT_STITCH</tt> modes.
 * On an event of a recoverable error this API will undo the current atomic transaction that
 * failed due to this error, raise a sys_warning with the same error message, and proceed further.
 * The fail-safe behavior of this API does not apply to "irrecoverable" errors, such as
 * <tt>NOT_BODY</tt>, <tt>INVALID_STITCH_MAX_TOL</tt>, <tt>COINCIDENT_FACES</tt>
 * (in <tt>SPASTITCH_COIN_ERROR</tt> mode),
 * <tt>INPUT_NOT_AN_EDGE</tt>, <tt>EDGE_ALREADY_STITCHED</tt>, and <tt>EDGE_HAS_NO_FACE</tt>.
 * These errors tend to indicate a programmatic mistake in the parameters being passed into the API,
 * rather than geometric or topological errors in the bodies being stitched.
 * Note that the fail-safe behavior can be switched OFF by pushing a value of
 * <tt>TRUE</tt> onto the global option <tt>careful</tt> before calling <tt>api_stitch</tt>
 * (and popping it after the call). When the fail-safe behavior is switched off,
 * this API will fail and roll back to its initial state when the first
 * error is encountered.<br><br>
 *
 * In R12 and later versions, this API will roll when coincident faces are
 * detected if <tt>careful</tt> global option is set to <tt>TRUE</tt>. When careful is <tt>TRUE</tt>,
 * the behavior of this API in <tt>SPASTITCH_COIN_ERROR</tt> mode will be to roll and exit
 * immediately, while the behavior of this API in <tt>SPASTITCH_COIN_SKIP</tt> and
 * <tt>SPASTITCH_COIN_STITCH</tt> modes will be to record the coincident face
 * information and continue stitching and roll the model back to its initial
 * state before exiting. So, when the <tt>careful</tt> option is set to <tt>TRUE</tt> and the
 * coincident face handling mode is set to <tt>SPASTITCH_COIN_SKIP</tt> or
 * <tt>SPASTITCH_COIN_STITCH</tt>, this API keeps the model unstitched, but provides
 * the coincident face information. When the <tt>careful</tt> option is set to <tt>FALSE</tt>,
 * this API will roll the model back to its initial state only in
 * <tt>SPASTITCH_COIN_ERROR</tt> mode.<br><br>
 *
 * The API will put into <tt>ENTITY_LIST& to_be_stitched</tt>, all the entities that were not
 * considered for stitching for topological reasons (including wire bodies). Note that if a body
 * in the incoming list <tt>ENTITY_LIST& to_be_stitched</tt> or the given <tt>BODY* body</tt>
 * has multiple lumps and one of those lumps is found unacceptable for stitching,
 * then the API will separate the unacceptable lump into a separate body which will be
 * placed into <tt>ENTITY_LIST& to_be_stitched</tt>. Normally the
 * <tt>ENTITY_LIST& to_be_stitched</tt> will be empty upon exit of the API. If it is not empty,
 * then the contents of this list will have to be managed by the caller of the API. In particular,
 * the caller is responsible for calling <tt>api_delent</tt>() on these entities when they are no longer needed.
 * <br><br>
 *
 * A user can determine the result of this API using the following:<br>
 * <dl><dd>
 *  a.	If <tt>outcome::encountered_errors</tt> returns <tt>FALSE</tt>,
 *  then the API has fully succeeded. i.e. errors were not encountered.<br>
 *  b.	If <tt>outcome::encountered_errors</tt> returns <tt>TRUE</tt>,
 *  and <tt>outcome::ok</tt> returns <tt>TRUE</tt>, then it means that the API has
 *  encountered error(s), yet it proceeded further successfully. In this case,
 *  the out coming <tt>BODY *body</tt> is not guaranteed to be usable.<br>
 *  c.	If <tt>outcome::ok</tt> returns <tt>FALSE</tt>, then the API has failed and
 *  rolled the model back to the state before the API was called.
 * </dd></dl>
 * <br>
 *
 * This API puts into the <tt>outcome</tt> returned, the information about any
 * error or problem that it encounters during its operation.
 * For more details, refer to the documentation of <tt>outcome</tt> and the technical article.
 * <br><br>
 *
 * <b>Errors:</b> Pointer to the body is <tt>NULL</tt> or not to a <tt>BODY</tt>.<br>
 * None of the coedges or vertices contain identical geometry.<br>
 * Incompatible coedges encountered.<br>
 *
 * The possible errors due to topological reasons are <tt>INVALID_TOPOLOGY</tt> and
 * <tt>UNSUPPORTED_TOPOLOGY</tt>.
 * <tt>INVALID_TOPOLOGY</tt> is used to report topology that is invalid in ACIS.
 * <tt>UNSUPPORTED_TOPOLOGY</tt> is used to report topology that is valid in ACIS but is not
 * supported by this API.
 *
 * <br><br>
 * <b>Limitations:</b>
 * <br><br>
 * Tolerant stitching and Edge Tolerant stitching can not be used
 * to create non-manifold (that is, more than two coedges per edge) bodies. Coincident
 * faces cannot be stitched.<br><br>
 * All incoming entities must check.<br><br>
 * For restricted tolerant stitching, all entities in the to_be_stitched list must
 * be unshared <tt>EDGEs</tt> belonging (at top-level) to a <tt>FACE</tt> or a <tt>BODY</tt>;
 * The body passed to the API should not be <tt>NULL</tt>. But it can be a body without any
 * lumps. Also it can be a body, which owns some or all the edges passed in the
 * list.<br><br>
 * Tolerant stitching may result into a single lump or a set of lumps each of which
 * may be a solid lump or a sheet lump. The API will make sure that all these lumps
 * become a part of the given body.
 * <br><br>
 * If a user has specified the <tt>max_stitch_tol</tt> parameter in the
 * <tt>tolerant_stitch_options</tt> object passed and has registered a callback
 * function with <tt>hh_set_stitch_opt_callback()</tt>, then the value of the
 * <tt>max_stitch_tol</tt> parameter set by a callback function will override
 * the value set by <tt>tolerant_stitch_options</tt> object. The user is
 * recommended to use <tt>tolerant_stitch_options</tt> object for setting the
 * <tt>max_stitch_tol</tt> parameter and not register any callback function with
 * <tt>hh_set_stitch_opt_callback()</tt> before calling the <tt>api_stitch</tt>.<br><br>
 *
 * <b>Effect:</b> Changes model. This API may remove edges smaller than maximum stitch
 * tolerance and faces with zero area. This API may also split incoming edges in the process
 * of stitching.
 * <br><br>
* <b>Journal: </b> Available
* <br><br>
 * <b>Product(s):</b> 3D InterOp
 * <br><br>
 * @param body
 * body to be stitched with.
 * @param to_be_stitched
 * list of entities to be stitched.
 * @param sopts
 * stitch options.
 * @param aopts
 * options, version,  journal.
 * <br><br>
 * @see stitch_options, edge_tolstitch_options, exact_stitch_options, tolerant_stitch_options
 **/
[[deprecated ("Deprecated API")]] 
DECL_STITCH outcome api_stitch(
	BODY* body,					  // body that other stuff will be stitched to.
	ENTITY_LIST & to_be_stitched, // list of entities to stitch into body.
	stitch_options* sopts,        // configuration information.
	AcisOptions* aopts = NULL		  // required by ACIS
);



#endif

