/* vim: set sw=4 ts=4 si et: */		

#ifndef _GYR_CONNEX_COMPONENT_H
#define _GYR_CONNEX_COMPONENT_H

#include "nodeset.h"

typedef struct _ConnexComponentItem_t ConnexComponentItem_t;
typedef struct _ConnexComponentItem_t * ConnexComponentList_t;

struct _ConnexComponentItem_t {
    NodeSetList_t list;
    ConnexComponentItem_t * prev;
    ConnexComponentItem_t * next;
};

/* List opts */
ConnexComponentList_t connexcomponentlist_create();

void connexcomponentlist_destroy(ConnexComponentList_t * list);

ConnexComponentItem_t * connexcomponentlist_find_item_with_node(
								ConnexComponentList_t list, 
								nodeindex_t nodeidx);

ConnexComponentList_t connexcomponentlist_insert_edge(
								ConnexComponentList_t list, 
								nodeindex_t node_one, 
								nodeindex_t node_two);

ConnexComponentList_t connexcomponentlist_append_component(
								ConnexComponentList_t list,
								ConnexComponentItem_t * item
								);

ConnexComponentList_t connexcomponentlist_remove_component(
								ConnexComponentList_t list,
								ConnexComponentItem_t * item
								);

void 	connexcomponentlist_display(ConnexComponentList_t list);

/* Item opts */

ConnexComponentItem_t * connexcomponentitem_create();

ConnexComponentItem_t * connexcomponentitem_create_from_node(
								nodeindex_t node_idx
								);

ConnexComponentItem_t * connexcomponentitem_create_from_merge(
								ConnexComponentItem_t * item_one,
								ConnexComponentItem_t * item_two
								);

void connexcomponentitem_destroy(ConnexComponentItem_t * item);

void connexcomponentitem_display(ConnexComponentItem_t * item);

ConnexComponentList_t connexcomponentitem_insert_edge(
								ConnexComponentList_t list, 
								nodeindex_t node_one, 
								nodeindex_t node_two);

bool connexcomponentitem_contains_node(
								ConnexComponentItem_t * item, 
								nodeindex_t node_idx);

#endif
