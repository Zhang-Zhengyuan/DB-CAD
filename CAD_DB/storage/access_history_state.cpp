#include "access_internal.hxx"

void api_note_current_state(DELTA_STATE *&out_current_ds) {
    out_current_ds = nullptr;
    HISTORY_STREAM *hs = nullptr;
    api_get_default_history(hs);
    if (hs != nullptr) {
        out_current_ds = hs->get_active_ds();
    }
}

void api_compute_delta_since(IncrementalContext &ctx, CollabDelta &delta) {
    delta.created_or_updated.clear();
    delta.deleted.clear();

    DELTA_STATE *thissave_ds = nullptr;
    api_note_current_state(thissave_ds);
    if (thissave_ds == nullptr) {
        return;
    }

    if (ctx.lastsave_ds == nullptr) {
        HISTORY_STREAM *hs = nullptr;
        api_get_default_history(hs);
        if (hs != nullptr) {
            ctx.lastsave_ds = hs->get_root_ds();
        }
    }

    // 收集所有 changed entities（跨所有 bulletin board）
    std::unordered_set<ENTITY *> created_set, deleted_set;
    DELTA_STATE *this_ds = thissave_ds;
    while (this_ds != nullptr && this_ds != ctx.lastsave_ds) {
        BULLETIN_BOARD *bb = this_ds->bb();
        while (bb != nullptr) {
            BULLETIN *b = bb->start_bulletin();
            while (b != nullptr) {
                ENTITY *old_ent = b->old_entity_ptr();
                ENTITY *new_ent = b->new_entity_ptr();
                if (new_ent != nullptr) {
                    created_set.insert(new_ent);
                }
                if (old_ent != nullptr) {
                    deleted_set.insert(old_ent);
                }
                b = b->next();
            }
            bb = bb->next();
        }
        this_ds = this_ds->next();
    }

    // 过滤：移除"先创建后删除"的对冲项
    for (auto it = created_set.begin(); it != created_set.end();) {
        if (deleted_set.find(*it) != deleted_set.end()) {
            deleted_set.erase(*it);
            it = created_set.erase(it);
        } else {
            ++it;
        }
    }

    // 只保留 root body（BODY_ID），构建 deleted
    for (ENTITY *e : deleted_set) {
        if (e != nullptr && e->identity() == BODY_ID) {
            delta.deleted.push_back(e);
        }
    }

    // 构建 created_or_updated：只保留 root body，
    // 并把"同一 body 的子实体新增"排除（只取 BODY 本身）
    // 策略：如果一个 BODY 已在 created_set，就不再重复加它的子实体
    std::unordered_set<ENTITY *> root_bodies;
    for (ENTITY *e : created_set) {
        if (e != nullptr && e->identity() == BODY_ID) {
            root_bodies.insert(e);
        }
    }
    for (ENTITY *e : root_bodies) {
        delta.created_or_updated.push_back(e);
    }

    // 推进基准线
    ctx.lastsave_ds = thissave_ds;
}

void api_advance_delta_since(IncrementalContext &ctx) {
    DELTA_STATE *thissave_ds = nullptr;
    api_note_current_state(thissave_ds);
    if (thissave_ds != nullptr) {
        ctx.lastsave_ds = thissave_ds;
    }
}
