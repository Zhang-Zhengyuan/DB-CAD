#ifndef __TAG_SYSTEM_H__
#define __TAG_SYSTEM_H__

/*
 * The in interface in this file is private. The public tag management
 * functions are in cad.h and mesh.h.
 */

struct meshgems_tag_system_t_;
typedef struct meshgems_tag_system_t_ meshgems_tag_system_t;

MESHGEMS_METHOD meshgems_integer meshgems_tag_system_get_buffer_size(meshgems_tag_system_t *ts);
MESHGEMS_METHOD void meshgems_tag_system_delete(meshgems_tag_system_t *ts);
MESHGEMS_METHOD meshgems_tag_system_t *meshgems_tag_system_new(meshgems_integer nt);
MESHGEMS_METHOD meshgems_tag_system_t *meshgems_tag_system_clone(meshgems_tag_system_t *tso);
MESHGEMS_METHOD meshgems_status_t meshgems_tag_system_add_base_tag(meshgems_tag_system_t *ts, meshgems_integer btag);
MESHGEMS_METHOD meshgems_status_t meshgems_tag_system_create_base_tag(meshgems_tag_system_t *ts, meshgems_integer *btag);
MESHGEMS_METHOD meshgems_status_t meshgems_tag_system_get_composite_tag(meshgems_tag_system_t *ts, meshgems_integer *tag,
								     meshgems_integer ntag, meshgems_integer *pctag);
MESHGEMS_METHOD meshgems_status_t meshgems_tag_system_get_composite_tag_list(meshgems_tag_system_t *ts,
									  meshgems_integer *nctag, meshgems_integer *ctag);
MESHGEMS_METHOD meshgems_status_t meshgems_tag_system_get_composite_tag_definition(meshgems_tag_system_t *ts, meshgems_integer ctag,
										meshgems_integer *n_basic_tag, meshgems_integer *basic_tag);

MESHGEMS_METHOD meshgems_status_t meshgems_tag_system_get_tag_count(meshgems_tag_system_t *ts, meshgems_integer *nbtags);
MESHGEMS_METHOD meshgems_status_t meshgems_tag_system_get_tag_tag(meshgems_tag_system_t *ts, meshgems_integer itag, meshgems_integer *tagtag);
MESHGEMS_METHOD meshgems_status_t meshgems_tag_system_is_tag_present(meshgems_tag_system_t *ts, meshgems_integer tagtag);
MESHGEMS_METHOD meshgems_status_t meshgems_tag_system_is_base_tag_present(meshgems_tag_system_t *ts, meshgems_integer btag);

#endif
