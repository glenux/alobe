/* vim: set sw=4 ts=4 si et: */		

#include "nodeset.h"

#define DEBUG_INSERT 0
#define DEBUG_MERGE 1

#define DEBUG	(DEBUG_INSERT | DEBUG_MERGE)

/**
 *
 * LIST OPERATIONS
 *
 */

NodeSetList_t nodesetlist_create(){
	return NULL;
}

void nodesetlist_destroy(NodeSetList_t * list){
	/* FIXME : delete all elements of the list with ptr */
	NodeSetItem_t * ptr = *list;
	if (ptr != NULL) {
		if (ptr->next != NULL){
			nodesetlist_destroy(&(ptr->next));
			ptr->next = NULL;
		} else {
			// on est le dernier, on se détruit
			nodesetitem_destroy(ptr); // DESTROY OK
		}
	}
}

bool nodesetlist_contains_node(NodeSetList_t list, nodeindex_t node_idx){
	NodeSetItem_t * curptr = list;
	bool answer = false;
	while (curptr != NULL){
		if (nodesetitem_contains_node(curptr, node_idx)) {
			answer = true;
			break;
		}
		curptr = curptr->next;
	}
	return answer;
}

NodeSetList_t nodesetlist_insert_node(NodeSetList_t list, nodeindex_t node_idx)
{
	NodeSetList_t head = list;
	/* on crée un élément pour le noeud */
	NodeSetItem_t * item = nodesetitem_create_from_node(node_idx);
	/*
	 * char * str = nodesetitem_tostr(item);
	 * printf("Created nodesetitem %s\n",str);
	 * free(str);
	 */
	head = nodesetlist_insert_item(head, item); 
	nodesetitem_destroy(item);
	return head;
}


NodeSetList_t nodesetlist_insert_item(NodeSetList_t root, 
		NodeSetItem_t * item)
{
	bool action_done = false;
	NodeSetList_t head = root;
	// on insere de façon triée...
	NodeSetItem_t * prevptr = NULL;	
	NodeSetItem_t * curptr = root;	
	char * curstr = NULL;
	char * itemstr = nodesetitem_tostr(item);

	while(curptr != NULL){

		curstr = nodesetitem_tostr(curptr);

		if (nodesetitem_is_overlap(curptr, item)){
			pDEBUG("OVERLAP %s vs %s\n", curstr, itemstr);
			/* on fusionne les deux... cf SNAP*/
			NodeSetItem_t * newptr = 
				nodesetitem_create_from_bounds(curptr, item);

			curptr->minimum = newptr->minimum;
			curptr->maximum = newptr->maximum;

			/* 
			 * char * str = nodesetitem_tostr(curptr);
			 * printf("=> %s\n",str);
			 * free(str);
			 */

			nodesetitem_destroy(newptr);

			action_done = true;
			break;
		} 
		else if (nodesetitem_is_snap(curptr, item))
		{
			pDEBUG("SNAP %s vs %s\n", curstr, itemstr);
			/* on fusionne les deux... cf OVERLAP*/
			NodeSetItem_t * newptr = 
				nodesetitem_create_from_bounds(curptr, item);

			curptr->minimum = newptr->minimum;
			curptr->maximum = newptr->maximum;

			/* 
			 * char * str = nodesetitem_tostr(curptr);
			 * printf("=> %s\n",str);
			 * free(str);
 			 */

			nodesetitem_destroy(newptr);

			action_done = true;
			break;
		} 
		else if (curptr->minimum > item->maximum) {
			/* alors on insère le noeud ici... */
			NodeSetItem_t * newptr = nodesetitem_copy(item);
			if (NULL == curptr->prev) 
			{ /* on est en tete de liste */
				newptr->next = head;
				if (head){ head->prev = newptr; }
				head = newptr;
			} 
			else 
			{ /* on est en milieu de liste */
				prevptr->next = newptr;
				newptr->prev = prevptr;
				newptr->next = curptr;
				curptr->prev = newptr;
			}

			action_done = true;
			break;
		} else {
			pDEBUG("Rien en commun : %s vs %s\n", curstr, itemstr);
			// on va ajouter à la fin ?
		}

		prevptr = curptr;	
		curptr = curptr->next;	

		free(curstr);
		curstr = NULL;
	}
	if (head == NULL){
		NodeSetItem_t * newptr = nodesetitem_copy(item);
		head = newptr;
		action_done = true;
	}

	if (!action_done){
		pDEBUG("On ajoute %s a la fin de la nodesetlist\n",itemstr);
		NodeSetItem_t * newptr = nodesetitem_copy(item);
		if (NULL != prevptr) {	prevptr->next = newptr; } // n'arrive jamais 
		newptr->prev = prevptr;
	}

	if (NULL != curstr) { free(curstr); }
	if (NULL != itemstr) { free(itemstr); }

	return head;
}

NodeSetList_t nodesetlist_merge(NodeSetList_t one, NodeSetList_t two){
	NodeSetList_t result = nodesetlist_create();

	NodeSetItem_t * curptr;

	if (DEBUG_MERGE){
		pDEBUG("MERGING NODESETLIST\n");
		nodesetlist_display(one);
	}

	curptr = one;
	while (curptr != NULL){
		/* 
		 * str = nodesetitem_tostr(curptr);
		 * printf("Copying... %s\n", str);
		 * free(str);
		 */
		result = nodesetlist_insert_item (result, curptr);
		curptr = curptr->next;
	}

	if (DEBUG_MERGE){
		pDEBUG("AND\n");
		nodesetlist_display(two);
	}

	curptr = two;
	while (curptr != NULL){
		/* 
		 * str = nodesetitem_tostr(curptr);
		 * printf("Copying... %s\n", str);
		 * free(str);
		 */
		result = nodesetlist_insert_item (result, curptr);
		curptr = curptr->next;
	}

	if (DEBUG_MERGE){
		pDEBUG("GIVES\n");
		nodesetlist_display(result);
	}
	return result;
}

void nodesetlist_display(NodeSetList_t list)
{

	printf("NodeSetList = {\n");
	NodeSetItem_t * curptr = list;	
	while(curptr != NULL){
		char * set = nodesetitem_tostr(curptr);
		printf("\t%s, \n",set);
		free(set);
		curptr = curptr->next;
	}
	printf("}\n");
}

/** 
 * 
 * ITEM OPERATIONS
 *
 */


NodeSetItem_t * nodesetitem_create(){
	NodeSetItem_t * node = (NodeSetItem_t *) malloc (sizeof(NodeSetItem_t));
	node->next = NULL;
	node->prev = NULL;
	node->minimum = -1;
	node->maximum = -1;
	return node;
}

NodeSetItem_t * nodesetitem_create_from_node(nodeindex_t node_idx){
	NodeSetItem_t * node = nodesetitem_create();
	node->minimum = node_idx;
	node->maximum = node_idx;
	return node;
}

NodeSetItem_t * nodesetitem_copy(NodeSetItem_t * original){
	NodeSetItem_t * result = nodesetitem_create();
	result->minimum = original->minimum;
	result->maximum = original->maximum;
	return result;
}

void nodesetitem_destroy(NodeSetItem_t * root){
	root->prev = NULL;
	root->next = NULL;
	free(root);
	root = NULL;
}


bool nodesetitem_contains_node(NodeSetItem_t * item, nodeindex_t node_idx){
	if ((item->minimum <= node_idx) && (item->maximum >= node_idx)){
		return true;
	} else {
		return false;
	}
}

bool nodesetitem_is_overlap(NodeSetItem_t * one, NodeSetItem_t * two){	
	bool answer = false;
	if ( 
			(
			 (one->minimum <= two->minimum)	// om <= tm <= oM
			 && (one->maximum >= two->minimum)
			) 
			|| (
				(one->minimum <= two->maximum)	// om <= tM <= oM
				&& (one->maximum >= two->maximum)
			   )
			|| (
				(two->minimum <= one->minimum)	// tm <= om <= tM
				&& (two->maximum >= one->minimum)
			   )
			|| (
				(two->minimum <= one->maximum)	// tm <= oM <= tM
				&& (two->maximum >= one->maximum)
			   )
	   )
	{
		answer = true;

	} 
	return answer;
}

bool nodesetitem_is_snap(NodeSetItem_t * one, NodeSetItem_t * two){
	bool answer = false;
	if ((one->maximum + 1) == two->minimum){
		answer = true;
	} else if ((two->maximum +1) == one->minimum){
		answer = true;
	}
	return answer;
}

char * nodesetitem_tostr(NodeSetItem_t * root){
	char * str = (char *) malloc (sizeof(char) * 100);
	sprintf(str,"[ %ld .. %ld ]", root->minimum, root->maximum);
	return str;
}

NodeSetItem_t * nodesetitem_create_from_bounds(NodeSetItem_t * one, 
		NodeSetItem_t * two){
	NodeSetItem_t * result;
	result = nodesetitem_create();
	result->minimum = 
		(one->minimum < two->minimum) ? (one->minimum) : (two->minimum);
	result->maximum = 
		(one->maximum > two->maximum) ? (one->maximum) : (two->maximum);

	/* 
	 * char * str= nodesetitem_tostr(result);
	 * printf("CREATING FROM BOUNDS -> %s\n", str);
	 * free(str);
	 */
	return result;
}



NodeSetList_t nodesetlist_insert_edge(NodeSetList_t list, 
		nodeindex_t from, 
		nodeindex_t to)
{
	NodeSetList_t head = list;
	NodeSetItem_t * edgeset_from;
	NodeSetItem_t * edgeset_to;

	printf("INSERTING EDGE (%ld - %ld)\n",from, to);

	if (NULL == list){ 
		printf("Empty set");
		edgeset_from = nodesetitem_create_from_node(from);
		head = nodesetlist_insert_item(head, edgeset_from); /* edgeset_from may have been deleted */

		edgeset_to = nodesetitem_create_from_node (to);
		head = nodesetlist_insert_item(head, edgeset_to); /* edgeset_to may have been deleted */
	} else {
#warning "NODESETLIST_INSERT_EDGE NOT IMPLEMENTED FOR LIST!=NULL"
	}
	return head;
}

#undef DEBUG_INSERT
#undef DEBUG_MERGE
#undef DEBUG
