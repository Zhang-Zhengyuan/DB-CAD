/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
// file     : dmerror.hxx
// created  : December 16, 1997
// author   : GW Celniker
// synopsis : Defines the error numbers that may be
//            returned by the DM programming interface
//            defined in "dmapi.hxx"

#ifndef DMERROR_H
#define DMERROR_H

// DM error numbers for system errors are listed in
// file dmexcept.hxx and range from -1 to -99

// error numbers returned by the DM api functions
#define DM_UNLABELLED_ERROR        -101 // routine signals an unlabled error - sorry 
#define DM_NO_ROOT_DMOD            -102 // input root_dmod argument was NULL
#define DM_NO_ACTIVE_DMOD          -103 // root dmod's current active dmod ptr == NULL
#define DM_DMOD_NOT_A_ROOT_DMOD    -104 // input dmod not the root_dmod
#define DM_UNCONTAINED_CHILD       -105 // input domain point not contained in targeted dmod
#define DM_PATCH_OCCLUDES_ROOT     -106 // input patch would occlude root dmod
#define DM_PATCH_OVERLAPS_SIBLING  -107 // input patch would overlap existing child
#define DM_DMOD_NOT_A_CURVE        -108 // input dmod not a curve as expected
#define DM_DMOD_NOT_A_SURFACE      -109 // input dmod not a surface as expected
#define DM_CANT_COPY_ROOT_PATCH    -110 // can't copy a root dmod as a patch
#define DM_CANT_RM_ONLY_ROOT_DMOD  -111 // can't remove the root dmod with DM_rm_patch() or DM_rm_tag_obj()
#define DM_CANT_RM_SRC_DMOD        -112 // Can't remove cstrn src_dmod's this way, use DM_set_cstrn_src_dmods()
#define DM_NOT_A_ROOT_SIBLING      -113 // identified target dmod is not a root's sibling
#define DM_DMOD_NOT_IN_A_DMESH     -114 // DS_dmod not part of a multi-surface dmesh structure
#define DM_TAG_NOT_A_DMOD_MEMBER   -115 // input tag value not a hierarchy dmod object
#define DM_NOT_BSPLINE_OR_NURB     -116 // dmod not a B-spline or nurb shape
#define DM_NOT_A_BSPLINE           -117 // input pfunc type not a B-spline as expected
#define DM_NOT_A_NURB              -118 // input pfunc type not a NURB as expected
#define DM_NOT_A_CIRC              -119 // input pfunc type not a circ as expected
#define DM_DMOD_MISSING_PFUNC      -120 // input dmod object does not contain a DS_pfunc object
#define DM_DELETE_A_USED_PFUNC     -121 // delete was called on a pfunc with a non-zero reference count
                                   
#define DM_TAG_OBJECT_NOT_FOUND    -122 // input tag number failed to find tag object of correct type
#define DM_TAG_NOT_CRV_CSTRN       -123 // input tag number failed to find a crv_cstrn tag object
                                   
#define DM_BAD_DOMAIN_DIM          -124 // input domain dimension not dmod's or not equal to 1 or 2
#define DM_BAD_DOMAIN_PT_RANGE     -125 // input domain point outside of the 0 to 1 range
#define DM_UNEVEN_LINK_SPEEDS      -126 // The speed between a link_cstrn's dcrvs must be linearly related
#define DM_NONMONOTONIC_LINK       -127 // link dcrvs must be monotonic, dC/ds can not change sign for valid values of s
#define DM_BAD_CIRC_SHAPE          -128 // input circ axis vectors are parallel turing a circ into a line
#define DM_BAD_SRC_CURVE_TYPE      -129 // src_type value not valid
#define DM_MISSING_SRC_DATA        -130 // wrong combination of inputs for curve descriptions for crv and link_cstrns
#define DM_MIXED_CRV_CSTRN_DIM     -131 // the image_dim of src_C_pfunc != to the dmod's domain_dim
#define DM_MIXED_AREA_CSTRN_DIM    -132 // the image_dim of the zone != to the domd's domain_dim
#define DM_MIXED_DCRV_DIM          -133 // dcrv's image_dim not equal to surface's domain_dim
#define DM_MIXED_CRV_LOAD_DIM      -134 // src_C_pfunc image_dim != dmod's domain_dim
#define DM_MIXED_FREE_LOAD_DIM     -135 // src_W_pfunc image_dim != dmod's image_dim
#define DM_BAD_SRC_PFUNC_MAPPING   -136 // a src_pfunc has an improper domain_dim or image_dim value
#define DM_CSTRN_HAS_NO_SRC_PFUNC  -137 // the crv_cstrn or link_cstrn is not using a src_W_pfunc
#define DM_BAD_CRV_CSTRN_DIM       -138 // the input src_C_pfunc is not a curve (i.e. domain_dim != 1)
#define DM_BAD_CRV_LOAD_DIM        -139 // the input src_C_pfunc is not a curve (i.e. domain_dim != 1)
#define DM_BAD_ELLIPSE_RANGE       -140 // requested ellipse does not fit in tgt_dmod
                                   
#define DM_CONFLICTING_CONSTRAINT  -141 // solve stopped due to a conflicting constraint
#define DM_SOLVE_NEVER_CONVERGED   -142 // solve iterated DS_SOLVE_MAX_ITER_COUNT times before reaching steady state
#define DM_BAD_CSTRN_CHANGE        -143 // requested cstrn state change prohibited for this cstrn
#define DM_BAD_SRC_TYPE_PARENT     -144 // src_type choice incompatible with parent_dmod value
#define DM_BAD_WUV_WVV_VALUES      -145 // Wuv_vec and Wvv_vec must be NULL for pt_cstrns on curves
#define DM_UNSTOPABLE_CSTRN        -146 // can't convert cstrn - it is unstopable
#define DM_UNDELETABLE_CSTRN       -147 // can't remove cstrn - it is undeletable
#define DM_NOT_USER_CSTRN          -148 // can't convert cstrn - it is not a type ds_user_cstrn cstrn
#define DM_LOAD_NOT_CONVERTIBLE    -149 // can't convert load - not a crv_load, spring, or spring_set
#define DM_CSTRN_NOT_CONVERTIBLE   -150 // can't convert cstrn - not a crv_cstrn or a pt_cstrn
#define DM_NO_UV_PT_FOR_TAG_OBJ    -151 // no uv pt for target tag object, or given dpt is uncontained
#define DM_NO_XYZ_PT_FOR_TAG_OBJ   -152 // can't get xyz pt for this kind of tag object
#define DM_PARALLEL_PROJECT_DIR    -153 // set_pt_xyz project dir is parallel to target axis
#define DM_NO_INTERSECTIONS        -154 // input image_line does not intersect dmod shape
#define DM_NOT_A_PT_CSTRN          -155 // identified constraint is not a point constraint
#define DM_NOT_A_CRV_LINK_CSTRN    -156 // identified constraint is not a curve or link constraint
#define DM_UNINITIALIZED_TAN_DIR   -157 // Tang direction is uninitialized - see DM_set_cstrn_pttan_uv_dir()
#define DM_UNINIT_DOMAIN_DIR       -158 // Domain direction is uninitialized - see DM_set_cstrn_pttan_uv_dir()
#define DM_ZERO_LENGTH_DOMAIN_DIR  -159 // Domain_dir SPAvector can not be zero length
#define DM_PARALLEL_DOMAIN_DIRS    -160 // Domain_dir directions must not be parallel
#define DM_NEG_CURVATURE_VALUE     -161 // Curvature value for curves must be positive or NULL for undefined
                                   
#define DM_NON_NULL_OUTPUT_PTR     -162 // reference to output array ptrs are not NULL on entry
#define DM_NULL_OUTPUT_PTR         -163 // required output ptr array is NULL on entry
#define DM_NULL_INPUT_PTR          -164 // input ptr to an array was NULL on input
#define DM_BAD_TAG_FLAG_VALUE      -165 // input tag_flag value was 0
#define DM_BAD_REFINEMENT_VALUE    -166 // input patch refinement must be 1 or greated
#define DM_BAD_CONTINUITY_VALUE    -167 // bad continuity flag - must be 0=C0, 1=C1, 2=C2, or -1 to toggle C0,C1,C2
#define DM_BAD_WALK_FLAG_VALUE     -168 // walk_flag not 0 (dmod only), 1 (dmod and offspring), or 2 (dmod, siblings, all offspring)
#define DM_BAD_DMOD_FLAG_VALUE     -169 // dmod_flag value must be 0, 1, or 2
#define DM_BAD_CONT_FLAG_VALUE     -170 // input cont_flag must be 1 (elevate cont) or 0 (preserve cont)
#define DM_BAD_DT_VALUE            -171 // input dt value has to be larger than 0.0    
#define DM_BAD_MASS_VALUE          -172 // input mass value has to be 0.0 or larger
#define DM_BAD_DAMP_VALUE          -173 // input damp value has to be 0.0 or larger
#define DM_BAD_TAG_VALUE           -174 // bad tag value, must be -1 (system assigns value) or 2 or larger
#define DM_BAD_END_COND_VALUE      -175 // input end_cond value not open, closed, or periodic
#define DM_BAD_SINGULAR_VALUE      -176 // input singular value not none, low, high, or both
#define DM_BAD_NEGATE_FLAG_VALUE   -177 // input negate_flag must be 1=negate or 0=don't
#define DM_BAD_PT_COUNT_VALUE      -178 // input point count must be greater than 0
#define DM_BAD_PT_INDEX_VALUE      -179 // input pt_index value not valid for found tag object
#define DM_BAD_CURVATURE_VALUE     -180 // input curvature value must be zero or positive
#define DM_BAD_CSTRN_VAL_SIZE      -181 // input cstrn_val_size is not correct for the selected constraint behavior
#define DM_BAD_DIR_FLAG_VALUE      -182 // input dir flag value must be 0, 1, or 2.
#define DM_BAD_MAX_DIST_VALUE      -183 // input max_dist value must be greater than 0.0
#define DM_BAD_ALPHA_VALUE         -184 // deformable alpha value (resist stretch) must 0 or greater
#define DM_BAD_BETA_VALUE          -185 // deformable beta value (resist bending) must be 0 or greater
#define DM_BAD_DELTA_VALUE         -186 // deformable delta value (resist displacement) must be 0 or greater
#define DM_ZERO_ALPHA_AND_BETA     -187 // Cannot have both alpha and beta equal to zero
#define DM_ZERO_AREA_PATCH         -188 // Cannot add a zero area patch to the hierarchy
#define DM_BAD_NTGRL_DEGREE_VALUE  -189 // requested Gauss integration degree not in supported range (1 to 19)
#define DM_BAD_IMAGE_DIM_VALUE     -190 // requested image dim must be larger than 0
#define DM_BAD_DEGREE_VALUE        -200 // requested spline degree must be 0 or greater
#define DM_BAD_KNOT_COUNT_VALUE    -201 // spline knot count must be larger than 0
#define DM_BAD_KNOT_TO_CPT_COUNT   -202 // last knot index value not equal to dof_count + degree - 2 
#define DM_BAD_ELEM_COUNT_VALUE    -203 // circ elem counts must be 5 or greater
#define DM_BAD_TOLERANCE_VALUE     -204 // tolerance values must be larger than 0
#define DM_BAD_SHAPE_FLAG_VALUE    -205 // Bad default shape flag - must 0=zero,1=save
#define DM_BAD_INTER_STATE_VALUE   -206 // Bad interior state value - must be 0=allow C0, or 1=enforce C1
#define DM_BAD_IPT_FLAG_VALUE      -207 // Bad ipt_flag - not 0=0 length spring or 1=use given end pt
#define DM_BAD_STATE_FLAG_VALUE    -208 // Bad state flag - 0=disable cstrn, 1=enable cstrn, -1=toggle cstrn
#define DM_BAD_INC_FLAG_VALUE      -209 // Bad inc_flag - 0=set value absoluetly, 1=set value incrementally
#define DM_BAD_GROUP_FLAG_VALUE    -210 // Bad group_flag - 0=set tag object only,1=set all loads of type
#define DM_BAD_SEAM_NUMBER_VALUE   -211 // Bad seam number - must be in range 0 to seam_count - 1
#define DM_BAD_DISP_GAIN_VALUE     -212 // Bad tan_display_gain - must not be 0, may be positive or negative
#define DM_BAD_DRAW_MESH_VALUE     -213 // Bad draw mesh values - must be greater than 0
#define DM_BAD_CASCADE_VALUE       -214 // Bad cascade value - must be 0 to 3
#define DM_BAD_CASCADE_FLAG_VALUE  -215 // Bad cascade_flag value - must be 0, 1, or 2
#define DM_FAILED_FILE_OPEN_WRITE  -216 // Designated file failed to open for write
#define DM_FAILED_FILE_OPEN_READ   -217 // Designated file failed to open for read
#define DM_NOT_A_JOURNAL_FILE      -218 // Specified file is not a journal file
#define DM_DIVERGENT_JOURNAL_RUN   -219 // Journal file value differs from journal run value - stoping play
#define DM_FILLET_RADIUS_TOO_BIG   -220 // Fillet Radius is too large to fit in shape
#define DM_FILLET_RADIUS_NOT_POS   -221 // Fillet Radius has to be positive to fit in the shape
#define DM_BAD_SRC_DATA_TGT_VALUE  -222 // tgt value for Src_data calls must be the value 1 or 2
#define DM_BAD_CIRC_PARAM_VALUE    -223 // circ param values must be from 0 to 2*DS_PI
#define DM_BAD_SCALE_PARAM_VALUE   -224 // Scale SPAparameter must be a non-zero number
#define DM_BAD_DOMAIN_FLAG_VALUE   -225 // domain_flag values: 0=orig_dmod_space, 1=unit-space, 2=internal_pfunc_space
#define DM_BAD_SHIFT_FLAG_VALUE    -226 // tag_shift_flag vals: 0=don't shift tags, 1=do shift when needed
#define DM_BAD_ZONE_FLAG_VALUE     -227 // zone flag value must be 0=zone area moves or 1=zone area fixed
#define DM_BAD_CSTRN_BEHAVIOR      -228 // Incorrect behavior setting: use DM_NORM to specify surface normal constraint
#define DM_MULTI_SURF_EXTRAPOLATE  -229 // Can't extrapolate a deformable model within a multi-surface mesh                              
#define DM_BAD_WHICH_DIR_VALUE     -230 // input which_dir val not a legal value
#define DM_BAD_TYPE_ID_VALUE       -231 // input DS_TAGS type_id value not a valid DS_dmod type
                                   
#define DM_BSPLN_CONSTRUCT_FAILED  -232 // Constructor for B-spline object failed
#define DM_PFUNC_CONSTRUCT_FAILED  -233 // Constructor for DS_pfunc object failed
#define DM_DMOD_CONSTRUCT_FAILED   -234 // Constructor for DS_dmod object failed
#define DM_CG_UNCONVERGED          -235 // Congrad solve failed to converge
#define DM_BAD_SPLINE_DEGREE       -236 // Can't set degree lower than 3
#define DM_HAS_PARENT              -237 // Can't set degree when a parent dmod is present
#define DM_HAS_CHILD               -238 // Can't set degree when a child dmod is present
#define DM_NULL_DMOD_PTR           -239 // Input dmod pointer was null

#define DM_ARRAY_OUT_OF_BOUNDS     -240 // DM array out of bounds
#define DM_TAGOBJ_HAS_NO_PARAM     -241 // DM tag object has no parameterization
#define DM_TAGOBJ_NOT_A_CURVE      -242 // DM tag object not a curve
#define DM_TAGOBJ_NO_SRC_PFUNC_OR_FUNC -243 // DM tag object has no src pfunc or func                                  
#define DM_SEGMENTER_FAILURE       -244 // Segmenter returned no segments or NULL pointer
#define DM_ADMICON_ACIS_ERROR      -245 // admicon returned acis error
#define DM_CODE_NOT_COMPLETE       -246 // Sorry, this function not complete in this Release
#define DM_INTERNAL_ERROR          -247 // Unexpected internal error in DM
#define DM_BAD_POLYGON             -248 // Input points do not specify a valid polygon
#define DM_NON_CONVEX_POLYGON      -249 // Input points do not specify a convex polygon
#define DM_BAD_GEOM_CONDITION      -250 // Bad surface or pcurve condition - 0 pivot in solver
#define DM_INITIALIZE_FAILURE      -251 // DM initialization failed
#define DM_TERMINATE_FAILURE       -252 // DM termination failed
#define DM_PARAMETER_OFF_SURFACE   -253 // Input pcurve evaluates off DS_dmod surface
#define DM_SEG_CALLBACK_NOT_INIT   -254 // DM segmenter callback was not initialized
#define DM_BAD_SURFACE_CORNER_TANGENTS -255 // Surface tangents at corner parallel

#define DM_UNAUTHORIZED_FUNCTIONALITY -256 // Not authorized to use this functionality

#define DM_CUSTOMER_ERROR       -999999 // DM customer defined error

#endif  // DMERROR_H

