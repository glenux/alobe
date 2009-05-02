/* vim: set sw=4 ts=4 si et: */		

#include "connexcomponent.h"


/**
 *
 * LIST OPERATIONS 
 *
 */

#define DEBUG_APPEND 0
#define DEBUG_INSERT 0
#define DEBUG	(DEBUG_APPEND | DEBUG_INSERT)

ConnexComponentList_t connexcomponentlist_create(){
	ConnexComponentList_t result = NULL;
	return result;
}

void connexcomponentlist_destroy(ConnexComponentList_t * list){
	ConnexComponentItem_t * ptr = *list;
	if (ptr != NULL){
		if (ptr->next != NULL){
			connexcomponentlist_destroy(&(ptr->next));
			ptr->next = NULL;
		}
		connexcomponentitem_destroy(ptr);
	}
}

ConnexComponentItem_t * connexcomponentlist_find_item_with_node(
		ConnexComponentList_t list, 
		nodeindex_t nodeidx)
{
	ConnexComponentItem_t * result = NULL;

	ConnexComponentItem_t * curptr = list;
	while(curptr != NULL){
		if (connexcomponentitem_contains_node(curptr, nodeidx)){
			result = curptr;
			break;
		}
		curptr = curptr -> next;
	}
	return result;
}

ConnexComponentList_t connexcomponentlist_insert_edge(
		ConnexComponentList_t list, 
		nodeindex_t node_one, 
		nodeindex_t node_two)
{
	bool one_created = false;
	bool two_created = false;
	ConnexComponentList_t result = list;
		
	ConnexComponentItem_t * cci_one = 
		connexcomponentlist_find_item_with_node(list, node_one);
	if (NULL == cci_one){
		one_created = true;
		/* on ajoute le premier noeud de l'edge 
		 * dans la liste des composantes connexes
		 */
		/* on fait pointer cci_one vers la composante en question */
		pDEBUG("Node %ld not found in existing component\n", node_one);
		cci_one = connexcomponentitem_create_from_node(node_one);
		if (DEBUG_INSERT) { 
			pDEBUG("Created component:\n"); 
			connexcomponentitem_display(cci_one); 
		}

//		result = connexcomponentlist_append_component(
//				result, 
//				cci_one);
	}

	ConnexComponentItem_t * cci_two = 
		connexcomponentlist_find_item_with_node(list, node_two);
	if (NULL == cci_two){
		two_created = true;
		/* on ajoute le premier noeud de l'edge 
		 * dans la liste des composantes connexes
		 */
		/* on fait pointer cci_one vers la composante en question */
		pDEBUG("Node %ld not found in existing component\n", node_two);
		cci_two = connexcomponentitem_create_from_node(node_two);
		if (DEBUG_INSERT) { 
			pDEBUG("Created component:\n"); 
			connexcomponentitem_display(cci_one); }

//		result = connexcomponentlist_append_component(
//				result, 
//				cci_two);
	}

	if (cci_one != cci_two)	{
		ConnexComponentItem_t * cci_merge = connexcomponentitem_create_from_merge(cci_one, cci_two);

		/* on efface les deux précédents */
		if (!one_created) { result = connexcomponentlist_remove_component(result, cci_one); }
		if (!two_created) { result = connexcomponentlist_remove_component(result, cci_two); }

		/* on ajoute le nouveau */
		result = connexcomponentlist_append_component(result, cci_merge);

		if (cci_one) { connexcomponentitem_destroy(cci_one); cci_one = NULL; }
		if (cci_two) { connexcomponentitem_destroy(cci_two); cci_two = NULL; }
	} else {
		/* les deux sont egales */
		if (cci_one) { connexcomponentitem_destroy(cci_one); cci_one = NULL; }
	}

	return result;
}

ConnexComponentList_t connexcomponentlist_append_component(
		ConnexComponentList_t list,
		ConnexComponentItem_t * item
		)
{
	ConnexComponentList_t head = list;

	if (DEBUG_APPEND) { 
		pDEBUG("PRE-append\n"); 
		connexcomponentlist_display(head);
	}

	item->next = NULL;
	item->prev = NULL;

	if (head == NULL){
		head = item;
	} else {
		item->next = head;
		head->prev = item;
		head = item;
	}

	if (DEBUG_APPEND){ 
		pDEBUG("POST-append\n");
		connexcomponentlist_display(head);
	}

	return head;
}

ConnexComponentList_t connexcomponentlist_remove_component(
		ConnexComponentList_t list,
		ConnexComponentItem_t * item
		)
{
	ConnexComponentList_t head = list;
	ConnexComponentItem_t * previtem = item->prev;
	ConnexComponentItem_t * nextitem = item->next;

	if (NULL != nextitem) { nextitem->prev = NULL; }
	if (NULL != previtem){ 	previtem->next = NULL; }

	if (NULL == previtem){
		/* si en tete de liste */
		/* on coupe les liens */
		/* on re-link la tete */
		head = nextitem;
	} else if (NULL == nextitem){
		/* sinon si en fin de liste */
		/* on coupe les liens */
	} else {
		/* sinon cas normal */
		previtem->next = NULL;
		nextitem->prev = NULL;
	}

	/* on coupe les liens */
	item->next = NULL;
	item->prev = NULL;

	return head;
}

void connexcomponentlist_display(ConnexComponentList_t list)
{
	ConnexComponentItem_t * curptr = list;

	printf("All connex components follow:\n");
	while (curptr != NULL){
		connexcomponentitem_display(curptr);
		curptr = curptr->next;				
	}
}

/**
 *
 * ITERM OPERATIONS 
 *
 */

ConnexComponentItem_t * connexcomponentitem_create()
{
	ConnexComponentItem_t * result = 
		(ConnexComponentItem_t *) malloc (sizeof(ConnexComponentItem_t));
	result->list = nodesetlist_create();
	result->prev = NULL;
	result->next = NULL;
	return result;
}

ConnexComponentItem_t * connexcomponentitem_create_from_node(
		nodeindex_t node_idx
		)
{
	ConnexComponentItem_t * result = connexcomponentitem_create();
	result->list = nodesetlist_create();
	result->list = nodesetlist_insert_node(result->list, node_idx);
	return result;
}

ConnexComponentItem_t * connexcomponentitem_create_from_merge(
		ConnexComponentItem_t * item_one,
		ConnexComponentItem_t * item_two
		)
{
	ConnexComponentItem_t * result = connexcomponentitem_create();

	// copy data from item_one
	result->list = nodesetlist_merge (result->list, item_one->list);
	// copy data from item_two
	result->list = nodesetlist_merge (result->list, item_two->list);

	/*
	 printf("connexcomponentitem_create_from_merge -- MERGING COMPONENT-ITEMS\n");
	connexcomponentitem_display(item_one);
	printf("connexcomponentitem_create_from_merge -- AND\n");
	connexcomponentitem_display(item_two);
	printf("connexcomponentitem_create_from_merge -- GIVES\n");
	connexcomponentitem_display(result);
    */

	return result;
}

void connexcomponentitem_destroy(ConnexComponentItem_t * item)
{
	nodesetlist_destroy(&(item->list));
	item->list = NULL;
	free(item);
}		

bool connexcomponentitem_contains_node(
		ConnexComponentItem_t * item, 
		nodeindex_t node_idx)
{
	return nodesetlist_contains_node (item->list, node_idx);		
}

void connexcomponentitem_display(ConnexComponentItem_t * item){
	nodesetlist_display (item->list);
}

#undef DEBUG_APPEND
#undef DEBUG_INSERT
#undef DEBUG
