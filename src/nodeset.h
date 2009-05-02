/* vim: set sw=4 ts=4 si et: */		

#ifndef _GYR_EDGESET_H
#define _GYR_EDGESET_H

#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>

#include "units.h"
#include "config.h"

typedef struct _NodeSetItem_t NodeSetItem_t;
typedef struct _NodeSetItem_t * NodeSetList_t;

struct _NodeSetItem_t {
	nodeindex_t minimum;
	nodeindex_t maximum;

	NodeSetItem_t * next; 	
	NodeSetItem_t * prev; 	
} ;

/* List opts */
NodeSetList_t 	nodesetlist_create ();

void 			nodesetlist_destroy (NodeSetList_t * list);

bool 			nodesetlist_contains_node (NodeSetList_t list, nodeindex_t node_idx);

NodeSetList_t 	nodesetlist_insert_node (NodeSetList_t list, nodeindex_t node_idx);

NodeSetList_t 	nodesetlist_insert_item (NodeSetList_t root, 
										 NodeSetItem_t * item);

NodeSetList_t 	nodesetlist_merge(NodeSetList_t one, NodeSetList_t two);

void 			nodesetlist_display(NodeSetList_t list);


/* Item opts */
/* nodesetitem*/
NodeSetItem_t * 	nodesetitem_create ();

NodeSetItem_t * 	nodesetitem_copy (NodeSetItem_t * one);

NodeSetItem_t *		nodesetitem_create_from_node (nodeindex_t node_idx);

NodeSetItem_t * 	nodesetitem_create_from_bounds (NodeSetItem_t * one, 
													NodeSetItem_t * two);

void 				nodesetitem_destroy (NodeSetItem_t * node);

bool 				nodesetitem_is_snap (NodeSetItem_t * one, NodeSetItem_t * two);

bool 				nodesetitem_is_overlap (NodeSetItem_t * one, NodeSetItem_t * two);

bool 				nodesetitem_contains_node (NodeSetItem_t * item, nodeindex_t node_idx);
	
char * 				nodesetitem_tostr (NodeSetItem_t * root);

NodeSetItem_t * 	nodeset_insert_node (NodeSetItem_t * root, 
										 NodeSetItem_t * node);


/* edgeset */
NodeSetItem_t * 	edgeset_insert_edge(NodeSetItem_t * edgeset, nodeindex_t from, nodeindex_t to);


#endif
