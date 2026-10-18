# API 参考 (API Reference)

> 本文件由 ACIS 帮助文档 PDF 整理而成，保留英文正文、API 名称、函数签名与代码示例（已去除中文译文），供 AI 检索与参考。内容按主题合并多个来源 PDF；每个二级标题对应一个原始文档，保留可追溯的来源标注。

## ACIS API 列表 · ACIS API List

> 来源：`api_list.pdf`

> 说明：ACIS API 函数索引，从 PDF 表格提取并去重，共 **995** 个 API，按名称首段分组，组内按出现顺序排列。

### api_abh_*  (2)

- `api_abh_chamfer_edges`
- `api_abh_vblend`

### api_abort_*  (1)

- `api_abort_state`

### api_accurate_*  (1)

- `api_accurate_bs3_approximation`

### api_add_*  (7)

- `api_add_generic_named_attribute`
- `api_add_guide_curve_si`
- `api_add_mapping_curve_sli`
- `api_add_state`
- `api_add_to_collection`
- `api_add_vertex_sli`
- `api_add_bodies_phlv5_computer`

### api_advanced_*  (1)

- `api_advanced_cover`

### api_align_*  (2)

- `api_align`
- `api_align_wires_sli`

### api_alternating_*  (2)

- `api_alternating_keep_pattern`
- `api_alternating_scale_pattern`

### api_apply_*  (1)

- `api_apply_transf`

### api_approx_*  (1)

- `api_approx`

### api_asm_*  (8)

- `api_asm_assembly_get_owning_model`
- `api_asm_component_add_property`
- `api_asm_component_has_property`
- `api_asm_entity_handle_get_ptr`
- `api_asm_model_find_components_with_property`
- `api_asm_model_get_entity_handle`
- `api_asm_model_get_owned_property_owners`
- `api_asm_model_phlv5_compute`

### api_bend_*  (1)

- `api_bend_to_curve_entity`

### api_blend_*  (8)

- `api_blend_edge`
- `api_blend_edges`
- `api_blend_edges_pos_rad`
- `api_blend_graph`
- `api_blend_holdline`
- `api_blend_holdline_face_face`
- `api_blend_seq`
- `api_blend_three_ent`

### api_body_*  (5)

- `api_body`
- `api_body_mass_props`
- `api_body_to_1d`
- `api_body_to_2d`
- `api_body_mass_pr`

### api_bool_*  (1)

- `api_bool_make_intersection_graph`

### api_boolean_*  (7)

- `api_boolean`
- `api_boolean_chop_body`
- `api_boolean_chop_complete`
- `api_boolean_complete`
- `api_boolean_glue`
- `api_boolean_start`
- `api_boolean_tube_body`

### api_breakup_*  (1)

- `api_breakup_wires_sli`

### api_brep_*  (1)

- `api_brep_health_report`

### api_build_*  (5)

- `api_build_body_sli`
- `api_build_edges_sli`
- `api_build_faces_sli`
- `api_build_wire`
- `api_build_phlv5_computer`

### api_calculate_*  (2)

- `api_calculate_edge_tolerance`
- `api_calculate_vertex_tolerance`

### api_chamfer_*  (5)

- `api_chamfer_edge`
- `api_chamfer_edges`
- `api_chamfer_vertex`
- `api_chamfer_vertices`
- `api_chamfer_wire_vertex`

### api_change_*  (3)

- `api_change_body_trans`
- `api_change_state`
- `api_change_to_state`

### api_check_*  (16)

- `api_check_and_fix_tedge`
- `api_check_body`
- `api_check_cur_smoothness`
- `api_check_edge`
- `api_check_entity`
- `api_check_entity_ff_ints`
- `api_check_face`
- `api_check_face_clearance`
- `api_check_face_loops`
- `api_check_histories`
- `api_check_list_ff_ints`
- `api_check_solid_clearance`
- `api_check_wire_self_inters`
- `api_check_wire_self_intersects`
- `api_check_edge_errors`
- `api_check_vertex_errors`

### api_checking_*  (1)

- `api_checking`

### api_clash_*  (2)

- `api_clash_bodies`
- `api_clash_faces`

### api_clean_*  (4)

- `api_clean_entity`
- `api_clean_list_of_entities`
- `api_clean_wire`
- `api_clean_body`

### api_cleanup_*  (1)

- `api_cleanup_mid_sheet_progenitor_info`

### api_clear_*  (4)

- `api_clear_annotations`
- `api_clear_guide_curves_sli`
- `api_clear_mapping_curves_sli`
- `api_clear_geometry_sharing_info`

### api_closed_*  (1)

- `api_closed_wire`

### api_collapse_*  (1)

- `api_collapse_wires_sli`

### api_combine_*  (2)

- `api_combine_body`
- `api_combine_edges`

### api_complete_*  (2)

- `api_complete_blends`
- `api_complete_intersection_graph`

### api_compute_*  (3)

- `api_compute_pcurve_tightness`
- `api_compute_points_segments`
- `api_compute_view_phlv5_computer`

### api_concl_*  (1)

- `api_concl_blend_ss`

### api_convert_*  (1)

- `api_convert_to_spline`

### api_copy_*  (4)

- `api_copy_body`
- `api_copy_entity`
- `api_copy_entity_list`
- `api_copy_entity_contents`

### api_cover_*  (7)

- `api_cover_circuits`
- `api_cover_planar_edges`
- `api_cover_planar_wires`
- `api_cover_sheet`
- `api_cover_wire`
- `api_cover_wire_loops`
- `api_cover_wires`

### api_create_*  (16)

- `api_create_boundary_field`
- `api_create_global_mesh`
- `api_create_global_meshes`
- `api_create_graph_from_cells`
- `api_create_graph_from_edges`
- `api_create_graph_from_faces`
- `api_create_hatch_lines`
- `api_create_history`
- `api_create_li`
- `api_create_point`
- `api_create_polyhedra_body`
- `api_create_polyhedra_mesh`
- `api_create_refinement`
- `api_create_si`
- `api_create_text`
- `api_create_vertex_template`

### api_crv_*  (1)

- `api_crv_self_inters`

### api_ct_*  (17)

- `api_ct_add_to_group`
- `api_ct_attach_cells`
- `api_ct_cell_area`
- `api_ct_cell_mass_pr`
- `api_ct_copy_cell`
- `api_ct_expand_cells`
- `api_ct_flatten_cells`
- `api_ct_get_all_cells`
- `api_ct_lose_cells`
- `api_ct_lose_group`
- `api_ct_make_group`
- `api_ct_point_in_cell`
- `api_ct_propagate_cface_attribs`
- `api_ct_remove_from_group`
- `api_ct_return_ents`
- `api_ct_return_groups`
- `api_ct_vacate_cell`

### api_curve_*  (15)

- `api_curve_arc`
- `api_curve_arc_3curve`
- `api_curve_arc_3pt`
- `api_curve_arc_center_edge`
- `api_curve_arc_diagonal`
- `api_curve_bezier`
- `api_curve_ellipse`
- `api_curve_fillet`
- `api_curve_law`
- `api_curve_line`
- `api_curve_line_tangent`
- `api_curve_spline`
- `api_curve_spline2`
- `api_curve_spline2_periodic`
- `api_curve_pattern`

### api_cylindrical_*  (1)

- `api_cylindrical_pattern`

### api_deep_*  (3)

- `api_deep_copy_entity`
- `api_deep_copy_entity_list`
- `api_deep_down_copy_entity`

### api_del_*  (2)

- `api_del_entity`
- `api_del_entity_list`

### api_delent_*  (1)

- `api_delent`

### api_delete_*  (9)

- `api_delete_all_AF_POINTs`
- `api_delete_blends`
- `api_delete_collection_entities`
- `api_delete_ds`
- `api_delete_entity_facets`
- `api_delete_exp_blends`
- `api_delete_history`
- `api_delete_sli`
- `api_delete_phlv5_computer`

### api_detect_*  (6)

- `api_detect_blends`
- `api_detect_chamfers`
- `api_detect_match`
- `api_detect_mid_sheet_face_pairs`
- `api_detect_short_edges`
- `api_detect_sliver_faces`

### api_distribute_*  (1)

- `api_distribute_state_to_streams`

### api_dm_*  (3)

- `api_dm_get_attrib_dm2acis`
- `api_dm_rm_multi_face`
- `api_dm_add_link`

### api_do_*  (1)

- `api_do_one_blend_ss`

### api_down_*  (1)

- `api_down_copy_entity`

### api_ed_*  (2)

- `api_ed_inters_to_ents`
- `api_ed_self_inters`

### api_edent_*  (1)

- `api_edent_rel`

### api_edfa_*  (1)

- `api_edfa_int`

### api_edge_*  (16)

- `api_edge_approx_line_arc`
- `api_edge_arclength_metric`
- `api_edge_arclength_param`
- `api_edge_convexity_param`
- `api_edge_helix`
- `api_edge_law`
- `api_edge_plaw`
- `api_edge_spiral`
- `api_edge_spring`
- `api_edge_spring_law`
- `api_edge_spring_taper`
- `api_edge_tangent`
- `api_edge_taper_faces`
- `api_edge_to_spline`
- `api_edge`
- `api_edge_pattern`

### api_elliptical_*  (1)

- `api_elliptical_pattern`

### api_embed_*  (1)

- `api_embed_wire_in_faces`

### api_enclose_*  (1)

- `api_enclose_void`

### api_end_*  (1)

- `api_end_journal`

### api_ensure_*  (1)

- `api_ensure_empty_root_state`

### api_ent_*  (1)

- `api_ent_area`

### api_entity_*  (4)

- `api_entity_entity_distance`
- `api_entity_entity_touch`
- `api_entity_extrema`
- `api_entity_point_distance`

### api_estimate_*  (2)

- `api_estimate_min_rad_curvature_skin`
- `api_estimate_tangent_factor_scale_li`

### api_expand_*  (1)

- `api_expand_body`

### api_extend_*  (1)

- `api_extend_sheetbody`

### api_extract_*  (1)

- `api_extract_coed_info`

### api_face_*  (12)

- `api_face_conic`
- `api_face_cylinder_cone`
- `api_face_law`
- `api_face_nu_nv_isolines`
- `api_face_plane`
- `api_face_sphere`
- `api_face_spl_apprx`
- `api_face_spl_ctrlpts`
- `api_face_spl_intrp`
- `api_face_torus`
- `api_face_u_iso`
- `api_face_v_iso`

### api_facet_*  (6)

- `api_facet_area`
- `api_facet_bodies`
- `api_facet_curve`
- `api_facet_edge`
- `api_facet_entities`
- `api_facet_entity`

### api_faceted_*  (1)

- `api_faceted_face`

### api_fafa_*  (1)

- `api_fafa_int`

### api_fast_*  (1)

- `api_fast_find_face`

### api_fillet_*  (1)

- `api_fillet_vertex`

### api_find_*  (7)

- `api_find_and_remove_features`
- `api_find_annotations`
- `api_find_cls_ptto_face`
- `api_find_face`
- `api_find_named_attribute`
- `api_find_named_state`
- `api_find_vertex`

### api_fit_*  (2)

- `api_fit_line`
- `api_fit_plane`

### api_fix_*  (3)

- `api_fix_blends`
- `api_fix_check_problems`
- `api_fix_face_coedge_sense`

### api_fixup_*  (1)

- `api_fixup_intersection`

### api_flatten_*  (1)

- `api_flatten_body`

### api_get_*  (61)

- `api_get_acis_version`
- `api_get_active_entities`
- `api_get_active_state`
- `api_get_all_user_attributes`
- `api_get_coedges`
- `api_get_curve_ends`
- `api_get_default_cgm_container`
- `api_get_default_history`
- `api_get_default_refinement`
- `api_get_edges`
- `api_get_edges_from_all_entities`
- `api_get_ellipse_parameters`
- `api_get_entities`
- `api_get_entity_box`
- `api_get_entity_from_id`
- `api_get_entity_id`
- `api_get_entity_refinement`
- `api_get_ents`
- `api_get_faces`
- `api_get_faces_from_all_entities`
- `api_get_facet_edge_points`
- `api_get_facet_face_tri_points`
- `api_get_file_info`
- `api_get_history_from_entity`
- `api_get_history_size`
- `api_get_indexed_mesh`
- `api_get_journal`
- `api_get_loops`
- `api_get_lumps`
- `api_get_mesh_manager`
- `api_get_mid_sheet_progenitor_info`
- `api_get_modified_entities`
- `api_get_modified_faces`
- `api_get_owner`
- `api_get_owner_transf`
- `api_get_refacet_faces`
- `api_get_save_version`
- `api_get_sequential_mesh`
- `api_get_shell_facets`
- `api_get_shells`
- `api_get_state_from_id`
- `api_get_state_id`
- `api_get_stream_logging`
- `api_get_tangent_factors_li`
- `api_get_tcoedges`
- `api_get_tedges`
- `api_get_tvertices`
- `api_get_version_tag`
- `api_get_vertices`
- `api_get_vertices_from_all_entities`
- `api_get_wires`
- `api_get_body_facets`
- `api_get_default_vertex_template`
- `api_get_entity_from_spaentity_id`
- `api_get_entity_vertex_template`
- `api_get_face_facets`
- `api_get_lost_support`
- `api_get_lump_facets`
- `api_get_pattern_transfs`
- `api_get_spaentity_id`
- `api_get_version_tagint`

### api_heal_*  (1)

- `api_heal_edges_to_regions`

### api_hedgehog_*  (1)

- `api_hedgehog`

### api_hex_*  (2)

- `api_hex_cylindrical_pattern`
- `api_hex_pattern`

### api_hh_*  (58)

- `api_hh_analytic_analyze`
- `api_hh_analytic_auto`
- `api_hh_analytic_calc_fix`
- `api_hh_analyze_body`
- `api_hh_analyze_coedges`
- `api_hh_analyze_edges`
- `api_hh_analyze_faces`
- `api_hh_analyze_loops`
- `api_hh_analyze_lumps`
- `api_hh_analyze_shells`
- `api_hh_analyze_vertices`
- `api_hh_auto_heal`
- `api_hh_end_body_for_healing`
- `api_hh_force_simplify_to_cone`
- `api_hh_force_simplify_to_cylinder`
- `api_hh_force_simplify_to_plane`
- `api_hh_force_simplify_to_sphere`
- `api_hh_force_simplify_to_torus`
- `api_hh_gen_spline_analyze`
- `api_hh_gen_spline_auto`
- `api_hh_gen_spline_calc_fix`
- `api_hh_geombuild_analyze`
- `api_hh_geombuild_auto`
- `api_hh_geombuild_calc_fix`
- `api_hh_geombuild_check`
- `api_hh_geombuild_cleanup`
- `api_hh_get_bad_coedges`
- `api_hh_get_bad_edges`
- `api_hh_get_bad_faces`
- `api_hh_get_bad_loops`
- `api_hh_get_bad_lumps`
- `api_hh_get_bad_shells`
- `api_hh_get_bad_vertices`
- `api_hh_get_entity_details`
- `api_hh_init_body_for_healing`
- `api_hh_isospline_analyze`
- `api_hh_isospline_auto`
- `api_hh_isospline_calc_fix`
- `api_hh_make_tolerant`
- `api_hh_postprocess`
- `api_hh_preprocess`
- `api_hh_sharp_edge_analyze`
- `api_hh_sharp_edge_auto`
- `api_hh_sharp_edge_calc_fix`
- `api_hh_simplify_analyze`
- `api_hh_simplify_auto`
- `api_hh_simplify_calculate`
- `api_hh_simplify_cleanup`
- `api_hh_simplify_fix`
- `api_hh_stitch_analyze`
- `api_hh_stitch_auto`
- `api_hh_stitch_calculate`
- `api_hh_stitch_cleanup`
- `api_hh_stitch_fix`
- `api_hh_store_entity_details`
- `api_hh_wrapup_analyze`
- `api_hh_wrapup_auto`
- `api_hh_wrapup_calc_fix`

### api_hook_*  (1)

- `api_hook_annotations`

### api_ihl_*  (10)

- `api_ihl_clean`
- `api_ihl_compute`
- `api_ihl_compute_entity_silhouette`
- `api_ihl_compute_from_meshes`
- `api_ihl_facet`
- `api_ihl_get_mesh`
- `api_ihl_get_output_manager`
- `api_ihl_occlusion`
- `api_ihl_retrieve`
- `api_ihl_set_output_manager`

### api_imprint_*  (5)

- `api_imprint`
- `api_imprint_complete`
- `api_imprint_shadow_edges`
- `api_imprint_stitch`
- `api_imprint_stitch_complete`

### api_incr_*  (1)

- `api_incr_bool_prepare`

### api_init_*  (1)

- `api_init_blend_ss`

### api_initialize_*  (35)

- `api_initialize_admgi_control`
- `api_initialize_admgi_draweng`
- `api_initialize_admhoops`
- `api_initialize_admicon`
- `api_initialize_advanced_blending`
- `api_initialize_blending`
- `api_initialize_booleans`
- `api_initialize_cellular_topology`
- `api_initialize_clearance`
- `api_initialize_constructors`
- `api_initialize_covering`
- `api_initialize_defeature`
- `api_initialize_euler_ops`
- `api_initialize_face_removal`
- `api_initialize_faceter`
- `api_initialize_generic_attributes`
- `api_initialize_healing`
- `api_initialize_hidden_line_removal`
- `api_initialize_hlr`
- `api_initialize_interactive_hidden_line`
- `api_initialize_intersectors`
- `api_initialize_kernel`
- `api_initialize_local_ops`
- `api_initialize_offsetting`
- `api_initialize_part_manager`
- `api_initialize_polyhedra`
- `api_initialize_query`
- `api_initialize_rbi`
- `api_initialize_shelling`
- `api_initialize_skinning`
- `api_initialize_spline`
- `api_initialize_stitching`
- `api_initialize_sweeping`
- `api_initialize_hlc`
- `api_initialize_persistent_id`

### api_integrate_*  (3)

- `api_integrate_law`
- `api_integrate_law_wrt`
- `api_integrate_law_wrt_and_splits`

### api_inter_*  (1)

- `api_inter_ed_ed`

### api_intersect_*  (2)

- `api_intersect`
- `api_intersect_curves`

### api_is_*  (1)

- `api_is_blend_face`

### api_join_*  (1)

- `api_join_edges`

### api_law_*  (1)

- `api_law_to_entity`

### api_linear_*  (2)

- `api_linear_pattern`
- `api_linear_scale_pattern`

### api_load_*  (1)

- `api_load_state`

### api_local_*  (1)

- `api_local_remove`

### api_loft_*  (2)

- `api_loft_coedges`
- `api_loft_faces`

### api_logging_*  (1)

- `api_logging`

### api_loop_*  (2)

- `api_loop_external`
- `api_loop_type`

### api_lose_*  (1)

- `api_lose_surface_conditions_li`

### api_make_*  (50)

- `api_make_VBL_output_surfaces`
- `api_make_approx_curve`
- `api_make_approx_surface`
- `api_make_blend_cross_curve`
- `api_make_blend_sheet`
- `api_make_blend_wire`
- `api_make_cnface`
- `api_make_cubic`
- `api_make_cuboid`
- `api_make_edge_from_curve`
- `api_make_ewire`
- `api_make_ewires`
- `api_make_face_from_surface`
- `api_make_face_spline`
- `api_make_flat_on_faces`
- `api_make_frustum`
- `api_make_kwire`
- `api_make_linear`
- `api_make_mapping_curves_sli`
- `api_make_mid_sheet_body`
- `api_make_planar_disk`
- `api_make_plface`
- `api_make_polygon`
- `api_make_polynomial_law`
- `api_make_prism`
- `api_make_pyramid`
- `api_make_quintic`
- `api_make_radius_constant`
- `api_make_radius_fixed_width`
- `api_make_radius_holdline`
- `api_make_radius_param_rads`
- `api_make_radius_param_rads_tan`
- `api_make_radius_pos_rads`
- `api_make_radius_rnd_chamfer`
- `api_make_radius_rot_ellipse`
- `api_make_radius_spline_rad`
- `api_make_radius_two_ends`
- `api_make_rails`
- `api_make_root_state`
- `api_make_shadow`
- `api_make_solid_from_faces`
- `api_make_spface`
- `api_make_sphere`
- `api_make_spline`
- `api_make_torus`
- `api_make_trface`
- `api_make_version_object`
- `api_make_wire`
- `api_make_wire_from_points_segments`
- `api_make_wires_sli`

### api_manifold_*  (1)

- `api_manifold_class`

### api_mark_*  (1)

- `api_mark_faceted_faces`

### api_merge_*  (3)

- `api_merge_faces`
- `api_merge_seam_edges`
- `api_merge_states`

### api_minimize_*  (2)

- `api_minimize_entities`
- `api_minimize_twist_wires_sli`

### api_mk_*  (10)

- `api_mk_by_faces`
- `api_mk_ed_bs3_curve`
- `api_mk_ed_conic`
- `api_mk_ed_cubic`
- `api_mk_ed_ellipse`
- `api_mk_ed_int_ctrlpts`
- `api_mk_ed_line`
- `api_mk_fa_spl_ctrlpts`
- `api_mk_fa_spl_fit`
- `api_mk_fa_spl_intp`

### api_model_*  (1)

- `api_model_comparison`

### api_modify_*  (4)

- `api_modify_ellipse`
- `api_modify_line`
- `api_modify_wire_sli`
- `api_modify_vertex_template`

### api_move_*  (3)

- `api_move_edge`
- `api_move_faces`
- `api_move_vertex_sli`

### api_n_*  (2)

- `api_n_body_clash`
- `api_n_body_unite_2d`

### api_name_*  (1)

- `api_name_state`

### api_ndifferentiate_*  (1)

- `api_ndifferentiate_law`

### api_net_*  (2)

- `api_net_sections`
- `api_net_wires`

### api_nmax_*  (1)

- `api_nmax_of_law`

### api_nmin_*  (1)

- `api_nmin_of_law`

### api_note_*  (1)

- `api_note_state`

### api_nroots_*  (1)

- `api_nroots_of_law`

### api_nsolve_*  (1)

- `api_nsolve_laws`

### api_offset_*  (8)

- `api_offset_edges_on_faces`
- `api_offset_face`
- `api_offset_face_edge`
- `api_offset_face_loops`
- `api_offset_faces`
- `api_offset_faces_specific`
- `api_offset_planar_face_loops`
- `api_offset_planar_wire`

### api_optimize_*  (1)

- `api_optimize_tvertex_tolerance`

### api_orient_*  (1)

- `api_orient_wire`

### api_part_*  (17)

- `api_part_add_entity`
- `api_part_create`
- `api_part_delete`
- `api_part_delete_all_states`
- `api_part_entities`
- `api_part_entity_id`
- `api_part_load`
- `api_part_lookup_entity`
- `api_part_name_state`
- `api_part_note_state`
- `api_part_remove_entity`
- `api_part_roll_n_states`
- `api_part_roll_to_state`
- `api_part_save`
- `api_part_start_state`
- `api_part_get_distribution_modelogical`
- `api_part_set_distribution_modelogical`

### api_pattern_*  (48)

- `api_pattern_apply_to_entity`
- `api_pattern_apply_to_faces`
- `api_pattern_copy`
- `api_pattern_create_cylindrical`
- `api_pattern_create_elliptical`
- `api_pattern_create_from_laws`
- `api_pattern_create_from_list`
- `api_pattern_create_hex`
- `api_pattern_create_hex_cylindrical`
- `api_pattern_create_linear`
- `api_pattern_create_on_curve`
- `api_pattern_create_on_edge`
- `api_pattern_create_on_surface`
- `api_pattern_create_polar_grid`
- `api_pattern_create_radial`
- `api_pattern_create_random`
- `api_pattern_create_spherical`
- `api_pattern_delete`
- `api_pattern_find_bump`
- `api_pattern_modify_compose`
- `api_pattern_modify_concatenate`
- `api_pattern_modify_element_remove`
- `api_pattern_modify_element_restore`
- `api_pattern_modify_element_scale`
- `api_pattern_modify_element_transform`
- `api_pattern_modify_filter`
- `api_pattern_modify_filter_alternating`
- `api_pattern_modify_filter_periodic`
- `api_pattern_modify_filter_random`
- `api_pattern_modify_mirror`
- `api_pattern_modify_offset_random`
- `api_pattern_modify_orient_random`
- `api_pattern_modify_reflect`
- `api_pattern_modify_root_transformation`
- `api_pattern_modify_scale`
- `api_pattern_modify_scale_alternating`
- `api_pattern_modify_scale_linear`
- `api_pattern_modify_scale_periodic`
- `api_pattern_modify_scale_random`
- `api_pattern_modify_transform`
- `api_pattern_query_coordinate_dimension`
- `api_pattern_query_coordinates`
- `api_pattern_query_element_indices`
- `api_pattern_query_root_transformation`
- `api_pattern_query_size`
- `api_pattern_query_transformations`
- `api_pattern_to_entity`
- `api_pattern_update_entities`

### api_pause_*  (1)

- `api_pause_journal`

### api_periodic_*  (2)

- `api_periodic_keep_pattern`
- `api_periodic_scale_pattern`

### api_phlv5_*  (3)

- `api_phlv5_clean`
- `api_phlv5_compute`
- `api_phlv5_retrieve`

### api_pidget_*  (1)

- `api_pidget`

### api_pidrem_*  (1)

- `api_pidrem`

### api_pidset_*  (1)

- `api_pidset`

### api_pierce_*  (1)

- `api_pierce_sheet`

### api_planar_*  (2)

- `api_planar_face_pr`
- `api_planar_slice`

### api_point_*  (2)

- `api_point_in_body`
- `api_point_in_face`

### api_polar_*  (1)

- `api_polar_grid_pattern`

### api_polyhedra_*  (1)

- `api_polyhedra_get_mesh`

### api_preview_*  (1)

- `api_preview_blends`

### api_process_*  (1)

- `api_process_mtwork_packet`

### api_project_*  (4)

- `api_project_curve_to_surface`
- `api_project_edge_to_face`
- `api_project_wire`
- `api_project_wire_to_body`

### api_prune_*  (2)

- `api_prune_following`
- `api_prune_history`

### api_ptent_*  (1)

- `api_ptent_rel`

### api_q_*  (1)

- `api_q_edges_around_vertex`

### api_query_*  (1)

- `api_query_state_validity`

### api_radial_*  (1)

- `api_radial_pattern`

### api_random_*  (5)

- `api_random_keep_pattern`
- `api_random_offset_pattern`
- `api_random_orient_pattern`
- `api_random_pattern`
- `api_random_scale_pattern`

### api_ray_*  (3)

- `api_ray_fire`
- `api_ray_test_body`
- `api_ray_test_ents`

### api_raytest_*  (2)

- `api_raytest_body`
- `api_raytest_ents`

### api_reenter_*  (1)

- `api_reenter_coedges_li`

### api_regularise_*  (1)

- `api_regularise_entity`

### api_remove_*  (17)

- `api_remove_and_repair_body`
- `api_remove_edges`
- `api_remove_face`
- `api_remove_faces`
- `api_remove_from_collection`
- `api_remove_generic_named_attribute`
- `api_remove_lop_attribs`
- `api_remove_mapping_curve_sli`
- `api_remove_no_merge_attrib`
- `api_remove_open_gap`
- `api_remove_state`
- `api_remove_transf`
- `api_remove_vertex_sli`
- `api_remove_wire_edge`
- `api_remove_wire_edges`
- `api_remove_zero_length_edge`
- `api_remove_pattern`

### api_repair_*  (1)

- `api_repair_body_self_ints`

### api_replace_*  (6)

- `api_replace_edge_with_tedge`
- `api_replace_edge_with_tvertex`
- `api_replace_face_with_tedge`
- `api_replace_tedge_with_edge`
- `api_replace_tvertex_with_vertex`
- `api_replace_vertex_with_tvertex`

### api_reset_*  (3)

- `api_reset_boxes`
- `api_reset_bs3_approximation`
- `api_reset_entity_refinement`

### api_restore_*  (6)

- `api_restore_entity_list`
- `api_restore_entity_list_file`
- `api_restore_entity_list_with_history`
- `api_restore_entity_list_with_history_file`
- `api_restore_history`
- `api_restore_history_file`

### api_resume_*  (1)

- `api_resume_journal`

### api_return_*  (2)

- `api_return_collection_ents`
- `api_return_collections`

### api_reverse_*  (3)

- `api_reverse_body`
- `api_reverse_face`
- `api_reverse_wire`

### api_rh_*  (92)

- `api_rh_convert_image_end`
- `api_rh_convert_image_start`
- `api_rh_convert_rgb_scanline`
- `api_rh_copy_light`
- `api_rh_copy_material`
- `api_rh_copy_texture_space`
- `api_rh_create_cube_environment`
- `api_rh_create_foreground`
- `api_rh_create_light_shadow`
- `api_rh_create_material`
- `api_rh_create_texture_space`
- `api_rh_delete_light`
- `api_rh_delete_light_shadow`
- `api_rh_delete_material`
- `api_rh_get_background`
- `api_rh_get_background_args`
- `api_rh_get_clipping`
- `api_rh_get_color_comp`
- `api_rh_get_default_background_rgb`
- `api_rh_get_default_edge_rgb`
- `api_rh_get_default_face_rgb`
- `api_rh_get_default_vertex_rgb`
- `api_rh_get_displace_comp`
- `api_rh_get_displace_comp_list`
- `api_rh_get_displace_status`
- `api_rh_get_environment_map`
- `api_rh_get_foreground`
- `api_rh_get_foreground_args`
- `api_rh_get_foreground_types`
- `api_rh_get_highlight_rgb`
- `api_rh_get_light_args`
- `api_rh_get_light_list`
- `api_rh_get_light_state`
- `api_rh_get_light_types`
- `api_rh_get_material`
- `api_rh_get_material_color`
- `api_rh_get_material_texture`
- `api_rh_get_material_transp`
- `api_rh_get_output_mode`
- `api_rh_get_reflect_comp`
- `api_rh_get_reflect_comp_list`
- `api_rh_get_resolution`
- `api_rh_get_sidedness`
- `api_rh_get_sub_image`
- `api_rh_get_texture_space`
- `api_rh_get_texture_space_args`
- `api_rh_get_texture_space_types`
- `api_rh_get_transp_comp`
- `api_rh_get_transp_status`
- `api_rh_initialise_supl_shaders`
- `api_rh_read_lightworks_image`
- `api_rh_read_lightworks_image_size`
- `api_rh_render_cube_environment`
- `api_rh_render_entities`
- `api_rh_set_background`
- `api_rh_set_background_arg`
- `api_rh_set_clipping`
- `api_rh_set_default_background_rgb`
- `api_rh_set_default_edge_rgb`
- `api_rh_set_default_face_rgb`
- `api_rh_set_default_rgb`
- `api_rh_set_default_vertex_rgb`
- `api_rh_set_displace_arg`
- `api_rh_set_displace_comp`
- `api_rh_set_displace_status`
- `api_rh_set_entity_rgb`
- `api_rh_set_environment_map`
- `api_rh_set_foreground`
- `api_rh_set_foreground_arg`
- `api_rh_set_highlight_rgb`
- `api_rh_set_light_arg`
- `api_rh_set_light_state`
- `api_rh_set_material_color`
- `api_rh_set_material_reflection`
- `api_rh_set_material_texture`
- `api_rh_set_material_transp`
- `api_rh_set_reflect_arg`
- `api_rh_set_reflect_status`
- `api_rh_set_resolution`
- `api_rh_set_sidedness`
- `api_rh_set_sub_image`
- `api_rh_set_texture_space`
- `api_rh_set_texture_space_arg`
- `api_rh_set_transp_arg`
- `api_rh_set_view`
- `api_rh_get_std_background`
- `api_rh_get_std_light`
- `api_rh_get_std_material`
- `api_rh_render_std_entities`
- `api_rh_set_std_background`
- `api_rh_set_std_light`
- `api_rh_set_std_material`

### api_roll_*  (1)

- `api_roll_n_states`

### api_rsw_*  (4)

- `api_rsw_face_path`
- `api_rsw_face_vec`
- `api_rsw_wire_path`
- `api_rsw_wire_vec`

### api_sample_*  (1)

- `api_sample_faces`

### api_save_*  (8)

- `api_save_entity_list`
- `api_save_entity_list_file`
- `api_save_entity_list_with_history`
- `api_save_entity_list_with_history_file`
- `api_save_history`
- `api_save_history_file`
- `api_save_state`
- `api_save_version`

### api_scribe_*  (1)

- `api_scribe`

### api_selective_*  (3)

- `api_selective_boolean_stage1`
- `api_selective_boolean_stage2`
- `api_selective_unite`

### api_selectively_*  (2)

- `api_selectively_imprint`
- `api_selectively_intersect`

### api_separate_*  (1)

- `api_separate_body`

### api_set_*  (43)

- `api_set_abh_blends`
- `api_set_acis_options`
- `api_set_cc_blend`
- `api_set_const_chamfers`
- `api_set_const_rounds`
- `api_set_dbl_option`
- `api_set_default_history`
- `api_set_default_minimize_options`
- `api_set_default_refinement`
- `api_set_ee_cr_blend`
- `api_set_ee_vr_blend`
- `api_set_entity_refinement`
- `api_set_exp_co_ro_fbl_att`
- `api_set_exp_co_ro_ffbl_att`
- `api_set_exp_const_chamfer`
- `api_set_exp_const_round`
- `api_set_file_info`
- `api_set_inst_blend`
- `api_set_int_option`
- `api_set_journal`
- `api_set_journal_name`
- `api_set_mesh_manager`
- `api_set_mesh_manager_to_default`
- `api_set_no_merge_attrib`
- `api_set_options_li`
- `api_set_options_si`
- `api_set_pcurve_tightness`
- `api_set_str_option`
- `api_set_stream_logging`
- `api_set_tangent_factors_li`
- `api_set_var_blends`
- `api_set_vblend`
- `api_set_vblend_auto`
- `api_set_vblend_autoblend`
- `api_set_version`
- `api_set_auto_clipping`
- `api_set_blend_face`
- `api_set_coedge_tightness`
- `api_set_default_vertex_template`
- `api_set_edge_tightness`
- `api_set_entity_vertex_template`
- `api_set_lost_support`
- `api_set_view_phlv5_computer`

### api_shadow_*  (1)

- `api_shadow_taper_faces`

### api_share_*  (1)

- `api_share_geometry`

### api_sheet_*  (2)

- `api_sheet_from_ff`
- `api_sheet_loop`

### api_shell_*  (1)

- `api_shell_external`

### api_show_*  (1)

- `api_show_guides_si`

### api_silhouette_*  (1)

- `api_silhouette_edges`

### api_simplify_*  (4)

- `api_simplify_wires_sli`
- `api_simplify_body`
- `api_simplify_face`
- `api_simplify_pcurve`

### api_skin_*  (2)

- `api_skin_faces`
- `api_skin_wires`

### api_slice_*  (3)

- `api_slice`
- `api_slice_complete`
- `api_slice_of_model`

### api_sliver_*  (1)

- `api_sliver_entities`

### api_smooth_*  (2)

- `api_smooth_edge_seq`
- `api_smooth_edges_to_curve`

### api_solid_*  (4)

- `api_solid_block`
- `api_solid_cylinder_cone`
- `api_solid_sphere`
- `api_solid_torus`

### api_spherical_*  (1)

- `api_spherical_pattern`

### api_split_*  (7)

- `api_split_curve`
- `api_split_edge_at_disc`
- `api_split_edges_at_poles`
- `api_split_face`
- `api_split_face_at_disc`
- `api_split_periodic_faces`
- `api_split_wire_edges`

### api_spring_*  (1)

- `api_spring_back`

### api_stackmon_*  (1)

- `api_stackmon_limit`

### api_start_*  (3)

- `api_start_journal`
- `api_start_modeller`
- `api_start_vertex_sli`

### api_stitch_*  (2)

- `api_stitch`
- `api_stitch_nonmanifold`

### api_stop_*  (1)

- `api_stop_modeller`

### api_str_*  (1)

- `api_str_to_law`

### api_subgraph_*  (2)

- `api_subgraph_2dcell`
- `api_subgraph_3dcell`

### api_subset_*  (1)

- `api_subset_graph_with_plane`

### api_subtract_*  (1)

- `api_subtract`

### api_surface_*  (1)

- `api_surface_pattern`

### api_sw_*  (12)

- `api_sw_chain_axis`
- `api_sw_chain_path`
- `api_sw_chain_path_options`
- `api_sw_chain_surface`
- `api_sw_chain_vec`
- `api_sw_chain_wire`
- `api_sw_face_axis`
- `api_sw_face_norm`
- `api_sw_face_surface`
- `api_sw_face_vec`
- `api_sw_face_wire`
- `api_sw_wire_axis_sol`

### api_sweep_*  (2)

- `api_sweep_more`
- `api_sweep_with_options`

### api_taper_*  (1)

- `api_taper_faces`

### api_terminate_*  (36)

- `api_terminate_admgi_control`
- `api_terminate_admgi_draweng`
- `api_terminate_admhoops`
- `api_terminate_admicon`
- `api_terminate_advanced_blending`
- `api_terminate_blending`
- `api_terminate_booleans`
- `api_terminate_cellular_topology`
- `api_terminate_clearance`
- `api_terminate_constructors`
- `api_terminate_covering`
- `api_terminate_defeature`
- `api_terminate_euler_ops`
- `api_terminate_face_removal`
- `api_terminate_faceter`
- `api_terminate_generic_attributes`
- `api_terminate_healing`
- `api_terminate_hidden_line_removal`
- `api_terminate_hlr`
- `api_terminate_interactive_hidden_line`
- `api_terminate_intersectors`
- `api_terminate_kernel`
- `api_terminate_local_ops`
- `api_terminate_lopt_ops`
- `api_terminate_offsetting`
- `api_terminate_part_manager`
- `api_terminate_polyhedra`
- `api_terminate_query`
- `api_terminate_rbi`
- `api_terminate_shelling`
- `api_terminate_skinning`
- `api_terminate_spline`
- `api_terminate_stitching`
- `api_terminate_sweeping`
- `api_terminate_hlc`
- `api_terminate_persistent_id`

### api_test_*  (2)

- `api_test_deep_copy`
- `api_test_deep_down_copy`

### api_tighten_*  (1)

- `api_tighten_gaps`

### api_tolerize_*  (1)

- `api_tolerize_entity`

### api_trans_*  (1)

- `api_trans_edge`

### api_transform_*  (1)

- `api_transform_entity`

### api_trim_*  (4)

- `api_trim_2curves`
- `api_trim_chain`
- `api_trim_curve`
- `api_trim_middle`

### api_tweak_*  (8)

- `api_tweak_extend_faces`
- `api_tweak_faces`
- `api_tweak_faces_init`
- `api_tweak_fix_vertex`
- `api_tweak_open_circuits`
- `api_tweak_pick_edge_solution`
- `api_tweak_query_edge_solutions`
- `api_tweak_to_body`

### api_uncover_*  (1)

- `api_uncover_face`

### api_unfold_*  (1)

- `api_unfold`

### api_unhook_*  (4)

- `api_unhook_annotations`
- `api_unhook_face`
- `api_unhook_faces`
- `api_unhook_wire_edge`

### api_unite_*  (3)

- `api_unite`
- `api_unite_edges`
- `api_unite_wires`

### api_unstitch_*  (1)

- `api_unstitch_nonmani`

### api_update_*  (2)

- `api_update_intersection`
- `api_update_tolerance`

### api_va_*  (1)

- `api_va_delete_meshva_mesh`

### api_valid_*  (1)

- `api_valid_start_vertices_sli`

### api_vha_*  (2)

- `api_vha_initialize`
- `api_vha_terminate`

### api_void_*  (1)

- `api_void_volume`

### api_warp_*  (1)

- `api_warp_entity_slice`

### api_wcs_*  (3)

- `api_wcs_create`
- `api_wcs_get_active`
- `api_wcs_set_active`

### api_wiggle_*  (1)

- `api_wiggle`

### api_wire_*  (3)

- `api_wire_area`
- `api_wire_len`
- `api_wire_to_chain`

### api_wrap_*  (2)

- `api_wrap`
- `api_wrap_wire`



## API 宏 · API Macros

> 来源：`api_macros.pdf`

The body of every API function is enclosed within a pair of macros. The pair of macros used depends on the intended
behavior of the API function. These macros set up error handlers and affect the behavior of the history mechanism.

### Contents

1 Overview 1 1.1 Introduction to the History Mechanism 1.2 Introduction to Exception Handling 2.1 Example 1 2.1 1 2.2 Example 2 2.2 2 3.1 Example 3.1 4.1 Example 1 4.1 1 4.2 Example 2 4.2 2 6 See Also 6 There are four pairs of macros in the API_BEGIN/API_END family of macros. Used to control the creation of bulletin boards in the history mechanism and provide rollback behavior in the event of an exception. Similar to API_BEGIN/API_END but always rolls back the (potentially API_TRIAL_BEGIN / Similar to API_NOP_BEGIN/API_NOP_END but rolls back the nested API_TRIAL_END block only in the event of of an error.

Used by the ACIS system functions that manipulate the history-related data
structures. These macros are not intended to be used by applications.
Important: These macros must always be used in matched pairs, creating a block of code.
Important: These blocks should never contain a return, goto, break, or other statement that would
prematurely transfer control out of the block.
return goto break

### Introduction to the History Mechanism

ACIS records sets of ENTITY changes on bulletin boards. A bulletin board is the lowest level construct that can be rolled
backward or forward. When a bulletin board is rolled backward, all the ENTITY changes recorded on it are undone. When
a previously rolled back bulletin board is rolled forward, the ENTITY changes are once again part of the model.
During an operation, ACIS may temporarily create a stacked bulletin board. That is, a bulletin board within another bulletin
board. Sometimes these are referred to as nested bulletin boards. When the macro denoting the end of a stacked bulletin
board is reached, the ENTITY changes recorded on the stacked (inner) bulletin board will either be merged with the next
higher level bulletin board or immediately rolled back. Stacked bulletin boards do not remain in the persistent model.
For additional information on the ACIS history mechanism, refer to History and Roll. For information specifically about
bulletin boards (which are created using API_BEGIN/API_END blocks), refer to Bulletin Board.

### Introduction to Exception Handling

ACIS provides a set of exception handling macros to facilitate handling exceptions. The API_BEGIN/API_END family of
macros are implemented using these macros; therefore, the API_BEGIN/API_END family of macros contain exception
handling functionality. If an error occurs inside a API_BEGIN/API_END block, it will be handled, any model changes
that occurred in the block may be undone, and execution will continue immediately following the block. The application
may at that point issue a message to the user, delay the message to a later time, attempt an alternative action, or take

whatever action is appropriate.
Important: Exceptions are not automatically propagated by the API_BEGIN/API_END family of macros.
The application is responsible for properly propagating exceptions when nested macro blocks exist.
For additional information on exception handling in ACIS, refer to Error Handling.

### API_BEGIN and API_END Macros

Control the creation of bulletin boards in the history mechanism.
Protect the contained logic block with ACIS exception handling.
Provide rollback behavior in the event a modeling operation fails.
The API_BEGIN macro does several things for you. If it is the outermost API_BEGIN, it will create and open a bulletin
board for you. Each API_BEGIN starts an exception handling block for you. In addition, all API_BEGIN statements
declare an outcome called result for you. The outcome object will record the success or failure of the block of code
and contain information about the nature of any failure. (In addition to containing an error_info objects that contain
additional information about the problem. For additional information about processing errors, refer to Error Return
Mechanisms.)
result outcome
Because all ENTITY changes must be on a bulletin board, all ENTITY changes must be within an
API_BEGIN/API_END block. A common ACIS programming mistake is to call a direct interface function that changes an
ENTITY outside of an API_BEGIN/API_END block.
Because API_BEGIN declares result for you, you should not redeclare result inside the block. Redeclaring result
inside the API_BEGIN/API_END block will hide the one declared in the API_BEGIN macro.

Each API_END macro closes its exception handling block, but it does not resignal an exception. The outermost API_END
records the value of result in the bulletin board, indicating success or failure of the API_BEGIN/API_END block.
The processing of the bulletin board depends on the option bb_immediate_close . If bb_immediate_close has a
value of TRUE, then the bulletin board is processed when the API_END is reached. If bb_immediate_close has a
value of FALSE, then the bulletin board is not processed until the next API_BEGIN statement. Processing a bulletin
board implies rolling back the changes if the operation failed, or closing the bulletin board and possibly merging it with
other bulletin boards if the operation succeeded. The default behavior of ACIS is to delay the processing of bulletin boards
until the next API_BEGIN is encountered. This provides a measure of safety if you were to accidentally modify an
ENTITY before the next API_BEGIN.
Important: The outermost API_BEGIN/API_END block defines the scope of the bulletin board.
API_BEGIN/API_END blocks within the outermost API_BEGIN/API_END block provide exception
handling, but they do not affect the bulletin board.
There are several global options and functions that affect the behavior of an API_BEGIN/API_END block. Some of
these are described below.
bb_immediate_close This option controls whether bulletin boards are closed at the API_END statement or the
next API_BEGIN statement, as described above. The default value for this option is FALSE.
compress_bb If the value of this option is TRUE, at the end of each successful block the bulletins in the bulletin
board created for that block are merged with those from the previous bulletin board, so they appear as though the
operations occurred in the same block. The default value for this option is TRUE.
logging If the value of this option is FALSE, each bulletin board is deleted as soon as the next is opened and
application functions to get bulletin boards or delta states always return NULL. At the end of a failed API function, the
bulletin board is rolled and and deleted so the model is restored to its previous non-corrupted state. If the API function
is successful and API logging is FALSE, the bulletin board is simply deleted. This reduces memory usage, but the
application will not be able to roll to previous states because no history exists. The default value for this option is

api_logging(logical on_off) This API function sets the option logging to the specified value.

```cpp
api_logging(logical on_off) logging
```

set_logging(logical on_off) This global function specifies whether bulletins are created in an application.
This is different from api_logging, which keeps bulletins and bulletin boards hidden from the application level, but
still creates them to support error recovery. You should not call this function.

```cpp
set_logging(logical on_off) api_logging
```

### Example 1

The example code in Creating a New API Function to create a new API function illustrates how an API_BEGIN/API_END
block can be used to surround direct interface calls. A portion of the source code for api_make_cuboid is reproduced
below for your convenience.
double depth,
double height,

```cpp
{
body = make_parallelepiped( width, depth, height );
result = outcome( body == NULL ? API_FAILED : 0 );
return result;
}
```

The function make_parallelepiped consists of direct interface calls that modify the persistent model. (It constructs a
new body.) api_make_cuboid surrounds make_parallelepiped with an API_BEGIN/API_END block. This
provides:
A bulletin board to record the changes to the model,

Exception handling in case an exception occurs inside the API_BEGIN/API_END block, and
Rollback behavior in case the model changes need to be rolled back.
If make_parallelepiped successfully constructed a new body, result is set to 0. If it did not, result is set to
API_FAILED, which is an ACIS error code indicating the operation failed. When the API_END statement is reached the
value of result is examined and recorded in the bulletin board. If result is non-zero, the bulletin board will be rolled
back, causing all the model changes on the bulletin board to be undone. To be more precise, if the
API_BEGIN/API_END block in api_make_cuboid is the outermost API_BEGIN/API_END block, the above
actions will occur. If this API_BEGIN/API_END block is logically contained within another API_BEGIN/API_END
block, then this API_BEGIN/API_END block will serve as an exception handling block.
result
api_make_cuboid API_BEGIN/API_END API_BEGIN/API_END
API_BEGIN/API_END API_BEGIN/API_END API_BEGIN/API_END
Important: The outermost API_BEGIN/API_END block controls the scope of the bulletin board.
Important: All changes to the persistent model (that is, ENTITYs) MUST be contained on a bulletin board.

### Example 2

The code snippet below illustrates how an application can call ACIS API functions to create and delete a simple model. It
creates a prism, creates a cone, intersects the two bodies, then deletes the resulting body. Each ACIS API function contains
an API_BEGIN/API_END block. Because there are no API_BEGIN/API_END blocks surrounding the API functions,
each API function creates its own bulletin board. (In this example foo1 will create four bulletin boards: one for
api_make_prism, one for api_make_frustum, one for api_intersect, and one for api_del_entity.) In
addition, each API function performs its own exception handling.

```cpp
void foo1( void )
{
BODY * my_body = NULL;
foo2( my_body );
```

// Delete the returned body.

```cpp
if ( my_body )
api_del_entity( my_body );
```

```cpp
}
void foo2( BODY *& retbody )
{
BODY *prism = NULL;
BODY *cone = NULL;
```

// Construct a prism.

```cpp
outcome result = api_make_prism( ..., prism );
```

// If the prism was successfully constructed, construct a cone.

```cpp
if ( result.ok() )
{
result = api_make_frustum( ..., cone );
}
```

// If both the prism and cone were constructed, intersect them.
// prism is the tool, cone is the blank.
// The result of this operation is the blank (cone).
// The tool (prism) is deleted by the Boolean operation.

```cpp
if ( result.ok() )
{
result = api_intersect( prism, cone );
}
```

// Setup our return value.

```cpp
if ( result.ok() )
{
retbody = cone;
}
}
```

The code snippet above works well so long as no errors occur. If an error were to occur in one of the API functions, there is
the possibility that the persistent model would not be properly cleaned up. (For instance, if the intersection operation
were to fail, the prism and cone would still exist.) The code snippet below illustrates how an API_BEGIN/API_END
block can be added to f002 such that it surrounds the three construction and modeling API functions, thereby combining
them on a single bulletin board. If an error occurs inside the API_BEGIN/API_END block, all of the model changes will
be rolled back. The bulletin board contains the creation of the prism, the construction of the cone, and the intersection of
the two bodies. foo2 has essentially been converted into an API function. It even returns an outcome to inform the
calling function of any potential problem.
f002
foo2 outcome

```cpp
void foo1( void )
{
BODY * my_body = NULL;
outcome result = foo2( my_body );
```

// Delete the returned body.

```cpp
if ( result.ok() )
api_del_entity( my_body );
}
```

```cpp
outcome foo2( BODY *& retbody )
{
BODY *prism = NULL;
BODY *cone = NULL;
```

// Construct a prism.

```cpp
result = api_make_prism( ..., prism );
```

// If the prism was successfully constructed, construct a cone.

```cpp
if ( result.ok() )
{
result = api_make_frustum( ..., cone );
}
```

// If both the prism and cone were constructed, intersect them.
// prism is the tool, cone is the blank.
// The result of this operation is the blank (cone).
// The tool (prism) is deleted by the Boolean operation.

```cpp
if ( result.ok() )
{
result = api_intersect( prism, cone );
}
// If a NULL body is returned, then there was no
```

// overlap - we consider this to be a failure.
// Force a rollback by setting result to a failing value.

```cpp
if ( cone == NULL )
result = outcome( API_FAILED );
```

// Setup our return values.

```cpp
if ( result.ok() )
{
retbody = cone;
}
return result;
}
```

There are some significant points to note about the code snippet above:
The variable result in foo2 is used to determine the success or failure of the entire API_BEGIN/API_END block.
It is even set to API_FAILED if the prism and cone do not intersect. By doing this, the API_END will examine
result and, if it is non-zero, it will roll back all changes (assuming bb_immediate_close has a value of TRUE).
In the event of any type of failure, no model cleanup is needed. The rollback will perform all model cleanup.

If the operation is successful, a single bulletin board will be created by the history mechanism to represent the
modeling operations in foo2. This bulletin board will contain the creation of the prism, the creation of the cone, and
the Boolean intersection operation. A separate bulletin board will be created for the deletion operation in foo1.
foo2
foo1
There are no returns or gotos inside the API_BEGIN/API_END block. This is very important, because once an
API_BEGIN/API_END block is entered, it must be exited only through its API_END macro.
To take this example one step further, we could add a function to call foo1 and add an API_BEGIN/API_END block to
foo1. foo2 is unchanged.
foo2

```cpp
void foo0( void )
{
BODY * my_body = NULL;
outcome result = foo1( my_body );
```

// Delete the returned body.

```cpp
if ( result.ok() )
api_del_entity( my_body );
}
outcome foo1( BODY *& retbody )
{
BODY * my_body = NULL;
result = foo2( my_body );
```

// perform additional operations on my_body.
....
// Setup our return values.

```cpp
if ( result.ok() )
{
retbody = my_body;
}
return result;
}
outcome foo2( BODY *& retbody )
{
BODY *prism = NULL;
BODY *cone = NULL;
```

// Construct a prism.

```cpp
result = api_make_prism( ..., prism );
```

// If the prism was successfully constructed, construct a cone.

```cpp
if ( result.ok() )
{
result = api_make_frustum( ..., cone );
}
```

// If both the prism and cone were constructed, intersect them.
// prism is the tool, cone is the blank.
// The result of this operation is the blank (cone).
// The tool (prism) is deleted by the Boolean operation.

```cpp
if ( result.ok() )
{
result = api_intersect( prism, cone );
}
// If a NULL body is returned, then there was no
```

// overlap - we consider this to be a failure.
// Force a rollback by setting result to a failing value.

```cpp
if ( cone == NULL )
result = outcome( API_FAILED );
```

// Setup our return values.

```cpp
if ( result.ok() )
{
retbody = cone;
}
return result;
}
```

This illustrates that nesting API_BEGIN/API_END blocks is perfectly acceptable. However, there are important
behavioral differences:
foo1 has now essentially been converted into an API function.
foo1
If a failure occurs within foo2, its API_END will NOT rollback the changes. The rollback will occur at the outermost
API_END, which is now the API_END in foo1.
It is important for foo2 to propagate the knowledge of success or failure using its return value. This allows foo1() to
set its result appropriately. The knowledge of success or failure must be propagated upward through
API_BEGIN/API_END blocks to ensure that the rollback occurs at the outermost API_END.

```cpp
foo2 foo1() result
```

### API_NOP_BEGIN and API_NOP_END Macros

When one or more ENTITYs are changed within an API_NOP_BEGIN/API_NOP_END block, those changes are
undone upon exiting API_NOP_END. This is useful when you want to evaluate the model without changing it. Changes
are allowed to happen as part of the operation, but they are then undone by API_NOP_END.
API_NOP_BEGIN may create a stacked bulletin board. A stacked bulletin board is logically nested inside an already
existing bulletin board, if one exists. Pairing of the associated macros must be strictly maintained.
Because API_NOP_END makes use of the bulletin facilities, the effect is lost if bulletin board logging is turned off.
Therefore, bulletin board logging is temporarily turned on in API_NOP_BEGIN and reset in API_NOP_END.
API_NOP_END rolls over the bulletin board opened by API_NOP_BEGIN. The bulletin pointers of the entities in the
previous bulletin are restored to their original state, leaving the bulletin board in the state it was before the

### Example

This example demonstrates how an API_NOP_BEGIN/API_NOP_END block can be used to "const-ify" a function. That
is, it rolls back all unwanted model changes. In this example an API_NOP_BEGIN/API_NOP_END block is used to roll
back unwanted model changes that occur while determining if a position lies on a given edge. In this case, the algorithm
creates temporary APOINT and point_entity_rel objects and we do not want them to exist after the end of the
function. The APOINT object is derived from ENTITY and will be rolled back at the API_NOP_END statement. The
point_entity_rel object is not derived from ENTITY; therefore, we must delete it manually. It is destructed after
the API_NOP_BEGIN/API_NOP_END block so that it will always be destructed, even if an exception would occur
within the API_NOP_BEGIN/API_NOP_END block. (In this case, because the point_entity_rel is created by an
API function and API functions should not throw exceptions, we do not really need to put the destruction of the
point_entity_rel after the API_NOP_BEGIN/API_NOP_END block, but we do so to demonstrate how the block
can be used for exception handling.)
API_NOP_BEGIN/API_NOP_END API_NOP_BEGIN/API_NOP_END
logical is_on_the_edge(SPAposition const & my_loc, EDGE * my_edge)

```cpp
{
```

```cpp
logical on_edge = FALSE;
point_entity_rel * my_rel = NULL;
```

// Create a temporary APOINT based upon the given location.

```cpp
APOINT * my_point = ACIS_NEW APOINT(my_loc);
// Determine if the location is on the interior of the edge
```

// or on either of its vertices.

```cpp
result = api_ptent_rel(my_point, my_edge, my_rel);
```

if (result.ok() && my_rel != NULL &&
(my_rel.rel_type == point_in_entity || my_rel.rel_type == point_on_entity))

```cpp
on_edge = TRUE;
if (!result.ok())
on_edge = FALSE;
if (my_rel != NULL)
ACIS_DELETE my_rel;
return on_edge;
}
```

In this example is_on_the_edge treats the input EDGE as a const. Although this may be the desired behavior of the
function, it is not the most efficient behavior. Because api_ptent_rel will use the bounding box of the EDGE and will
calculate one if one does not exist, and because API_NOP_END will remove the bounding box from the EDGE if it did not
exist before the API_NOP_BEGIN, this function may calculate and throw away the EDGE's bounding box. If this function
were to be called many times on the same EDGE, this would be very inefficient. To be more efficient, you should call
get_edge_box or api_get_entity_box before calling is_on_the_edge. (Because get_edge_box is a direct
interface function that can modify an entity, it must always be surrounded by a API_BEGIN/API_END block.)

### API_TRIAL_BEGIN and API_TRIAL_END Macros

Macros API_TRIAL_BEGIN and API_TRIAL_END can be used to bracket code that may or may not have results you
want to keep. Like API_NOP_BEGIN, API_TRIAL_BEGIN creates a stacked bulletin board. In API_TRIAL_END, the
outcome result is checked. If the result is good, the model edits are retained and the stacked bulletin board is
merged with the main bulletin board. If the outcome is bad, the results are rolled away as if API_NOP_BEGIN and
API_NOP_END had been used. These trial macros only work if logging is turned on. Therefore, bulletin board logging is

temporarily turned on in API_TRIAL_BEGIN and reset in API_TRIAL_END.
result outcome
API_TRIAL_BEGIN may create a stacked bulletin board. A stacked bulletin board is logically nested inside an already
existing bulletin board, if one exists. Pairing of the associated macros must be strictly maintained.
The main difference between these trial macros and the normal API_BEGIN/API_END macros is in the timing and in
what gets rolled back. The trial macros create a stacked bulletin board so the rolling section is exactly what is between the
two macros, regardless of how they may be nested inside other sets of macros. In the trial case, the roll back occurs in
API_TRIAL_END, whereas in the normal case, the roll back is delayed until the next bulletin board is opened. In the
normal case, the roll back is also dependent on the error eventually getting propagated to the outermost API_END. The
trial case always rolls back on failure without having to propagate to the outermost level. In other words, the behavior of
an API_TRIAL_BEGIN/API_TRIAL_END block is exactly the same, regardless of its nesting level.

### Example 1

In this example, we demonstrate how API_TRIAL_BEGIN/API_TRIAL_END blocks may be used to perform an
operation using three alternative algorithms. If the algorithm in the API_TRIAL_BEGIN/API_TRIAL_END block fails,
any model changes in the block will immediately be rolled back at the API_TRIAL_END statement. If the algorithm is
successful, the model changes on the nested bulletin board will be immediately merged with next higher level bulletin
board.
It is assumed that do_something is contained within a larger algorithm that contains an API_BEGIN/API_END block.
Otherwise it would not make sense to use API_TRIAL_BEGIN/API_TRIAL_END blocks: API_BEGIN/API_END
blocks would suffice.
API_TRIAL_BEGIN/API_TRIAL_END API_BEGIN/API_END

```cpp
logical do_something(BODY *& my_body)
{
logical success = algorithm1(my_body);
if (!success)
success = algorithm2(my_body);
if (!success)
```

```cpp
success = algorithm3(my_body);
return success;
}
logical algorithm1(BODY *& my_body)
{
volatile logical success = TRUE;
// Implement algorithm 1
if (!result.ok())
success = FALSE;
return success;
}
logical algorithm2(BODY *& my_body)
{
volatile logical success = TRUE;
// Implement algorithm 2
if (!result.ok())
success = FALSE;
return success;
}
logical algorithm3(BODY *& my_body)
{
volatile logical success = TRUE;
// Implement algorithm 3
if (!result.ok())
success = FALSE;
return success;
}
```

### Example 2

API_TRIAL_BEGIN/API_TRIAL_END blocks are also commonly used to provide failsafe behavior. The function in the
following code snippet illustrates how this can be achieved. do_something performs a failsafe operation on a list of
faces. The API_BEGIN/API_END block provides a single bulletin board for the entire operation. The
API_TRIAL_BEGIN/API_TRIAL_END block provides a stacked bulletin board for each attempted face operation. If
an operation on a face fails, any model changes that were made during the face operation are rolled back immediately. If
an operation on a face fails with an unexpected error (for instance, if an access violation were to occur), iteration ceases
and the error is propagated to the API_BEGIN/API_END block. This causes the entire operation to fail. If a face
operation completes successfully, the changes made to the model during the face operation are added to the bulletin
board. (More precisely, the bulletins on the stacked bulletin board become part of the main bulletin board and the stacked

bulletin board is deleted.)
There are two outcome objects named result in do_something. One is declared by the API_BEGIN macro. The
other is declared by the API_TRIAL_BEGIN macro. The scope of the second one is limited to the while-loop.

```cpp
outcome do_something(ENTITY_LIST & face_list)
{
face_list.init();
FACE* this_face = NULL;
while((this_face = (FACE* )face_list.next()) != NULL)
{
```

// Perform the operation on this face.

```cpp
logical ok = face_operation(this_face);
result = (ok) ? outcome(0) : outcome(API_FAILED);
```

// If the error was not an expected error, propagate it.

```cpp
if (!result.ok() && result.error_number() != API_FAILED)
{
sys_error(result.error_number() );
}
}
return result;
}
```

### API_SYS_BEGIN and API_SYS_END Macros

ACIS uses API_SYS_BEGIN and API_SYS_END for internal system routines. System routines manipulate bulletin
boards and roll back, and therefore do not return bulletin boards, or change the model. We recommend that applications
do not use these macros.

### See Also

Creating a New API Function
Bulletin Board

Categories: ACIS Extending ACIS ACIS History and Roll Error Management
As part of our commitment to prioritizing the security and integrity of your advanced engineering software
systems built upon SPATIAL technology, our team actively identifies and mitigates potential vulnerabilities using
industry-standard tools, databases, and composition analysis software for identifying CVEs. This approach
ensures confidence and resilience for you and your end users.
ACIS, 3DScript, and SAT are registered trademarks of SPATIAL Corp. Privacy Policy | Terms of Use
Send questions or comments to docfeedback_spatial@3ds.com


