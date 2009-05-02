/* vim: set sw=4 ts=4 si et: */		

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"

#define DEBUG	0

/**************************************************************************
 *
 */

List_t * list_create(){
	pDEBUG("\n");
	List_t * list = (List_t *) malloc (sizeof(List_t));
	
	list_init(list);

	return list;
}

/**************************************************************************
 *
 */

void list_init(List_t * list){
	list->head = NULL;
	list->tail = NULL;
}

/**************************************************************************
 *
 */

void list_destroy(List_t * list){
	pDEBUG("\n");
	// on détruit tous les noeuds de la liste, 
	// sans détruire les données dedans ?	
	while(!list_is_empty(list)){
		pDEBUG("destroying front cell\n");
		void * data = list_pop_front(list);
		pDEBUG("set data to NULL\n");
		data = NULL;
	}
	free(list);
}


/**************************************************************************
 *
 */

void list_destroy_with_data(List_t * list, void (* data_destroy_fun) (void *)){
	// on détruit tous les noeuds de la liste, 
	// sans détruire les données dedans ?	
	while(!list_is_empty(list)){
		pDEBUG("destroying front cell and data\n");
		void * data = list_pop_front(list);
		(* data_destroy_fun)(data);		
	}
	free(list);
}


/**************************************************************************
 *
 */

List_cell_t * list_cell_create(){
	List_cell_t * list_cell = 
		(List_cell_t *) malloc (sizeof(List_cell_t));
	pDEBUG("set data to NULL\n");
	list_cell->data = NULL;
	list_cell->next = NULL;
	list_cell->prev = NULL;
	return list_cell;
}


/**************************************************************************
 *
 */

void list_cell_destroy(List_cell_t * list_cell){
	assert(list_cell->data == NULL);
	free(list_cell);	
}


/**************************************************************************
 *
 */

void list_cell_destroy_with_data(List_cell_t * list_cell, 
		void (* data_destroy_fun) (void *))
{
	data_destroy_fun(list_cell->data);

	pDEBUG("set data to NULL\n");
	list_cell->data = NULL;
	list_cell_destroy(list_cell);
}


/**************************************************************************
 *
 */

bool list_is_empty(List_t * list){
	bool result = false;
	if ((list->head == list->tail)
			&& (list->head == NULL)){
		result = true;
	}
	return result;
}


/**************************************************************************
 *
 */

void list_push_back(List_t * list, void * data){
	// si la liste est vide ?
	if (list_is_empty(list)){
		List_cell_t * cell = list_cell_create();
		cell->data = data;

		list->head = cell;
		list->tail = cell;
	} else {
		List_cell_t * old_tail = list->tail;
		List_cell_t * new_tail = list_cell_create();
		
		new_tail->data = data;		

		old_tail->next = new_tail;
		new_tail->prev = old_tail;
	
		list->tail = new_tail;
	}
}


/**************************************************************************
 *
 */

void list_push_front(List_t * list, void * data){
	// si la liste est vide ?
	if (list_is_empty(list)){
		List_cell_t * cell = list_cell_create();
		cell->data = data;

		list->head = cell;
		list->tail = cell;
	} else {
		List_cell_t * old_head = list->head;
		List_cell_t * new_head = list_cell_create();
		
		new_head->data = data;		

		old_head->prev = new_head;
		new_head->next = old_head;
	
		list->head = new_head;
	}

}


/**************************************************************************
 *
 */

void * list_pop_front(List_t * list){
	void * celldata = NULL;
	if (!list_is_empty(list)){
		pDEBUG("\n");
		celldata = list_front(list);
		List_cell_t * cell = list->head;

		list->head = cell->next;
		if (list->head == NULL){ list->tail = NULL; }

		pDEBUG("set data to NULL\n");
		cell->data = NULL;
		list_cell_destroy(cell);
	}
	return celldata;
}


/**************************************************************************
 *
 */

void * list_pop_back(List_t * list){
	void * celldata = NULL;
	if (!list_is_empty(list)){
		pDEBUG("\n");
		celldata = list_back(list);

		List_cell_t * cell = list->tail;
		list->tail = cell->prev;
		if (list->tail == NULL){ list->head = NULL; }

		pDEBUG("set data to NULL\n");
		cell->data = NULL;
		list_cell_destroy(cell);
	}
	return celldata;
}


/**************************************************************************
 *
 */

void * list_front(List_t * list){
	List_cell_t * cell = list->head;

	void * celldata;
	if (cell != NULL){
		celldata = cell->data;
	} else {
		celldata = NULL;
		//assert(0==1); //FIXME! trouver un moyen d'indiquer que la liste est vide...
	}

	return celldata;
}


/**************************************************************************
 *
 */


void * list_back(List_t * list){
	List_cell_t * cell = list->tail;

	void * celldata;
	if (cell != NULL){
		celldata = cell->data;
	} else {
		celldata = NULL;
		// assert(0==1); //FIXME! trouver un moyen d'indiquer que la liste est vide...
	}
	return celldata;
}

/**************************************************************************
 *
 */

void * list_iterator_value(List_iterator_t * iterator){
	return iterator->data;
}

List_iterator_t * list_iterator(List_t * list){
	return list->head;
}

List_iterator_t * list_iterator_next(List_iterator_t * iterator){
	return iterator->next;
}

bool list_iterator_is_end(List_cell_t * iterator){
	return (iterator == NULL);
}

