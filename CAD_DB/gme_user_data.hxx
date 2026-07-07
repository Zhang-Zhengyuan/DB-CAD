#pragma once

#include <string>
#include <vector>

#include "acis/include/bulletin.hxx"
#include "window.h"

struct ENTITY_TREE_ITEM;

class GME_DELTA_STATE_user_data : public DELTA_STATE_user_data {
    std::vector<ENTITY_TREE_ITEM> m_tree_items;

public:
    GME_DELTA_STATE_user_data() {};
    void add_tree_item(ENTITY_TREE_ITEM* item) { m_tree_items.push_back(*item); }
    std::vector<ENTITY_TREE_ITEM>* get_tree_items() { return &m_tree_items; }
};
