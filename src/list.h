/* vim: set sw=4 ts=4 si et: */		

#ifndef _LIST_H
#define _LIST_H

#include "units.h"

typedef struct list_cell_t {
	void * data;	
	struct list_cell_t * prev;
	struct list_cell_t * next;
} List_cell_t;

typedef List_cell_t List_iterator_t;

typedef struct {
	List_cell_t * head;
	List_cell_t * tail;
} List_t ; 


List_t * list_create();

void list_init(List_t * list);

void list_destroy(List_t * list);

void list_destroy_with_data(List_t * list, void (* datadestroyfun)(void *) );


List_cell_t * list_cell_create();

void list_cell_destroy(List_cell_t * cell);

void list_cell_destroy_with_data(List_cell_t * cell, void (* datadestroyfun)(void *) );


bool list_is_empty(List_t * list);

void list_push_back(List_t * list, void * data);

void list_push_front(List_t * list, void * data);

// void list_merge(List_t * list_one, List_t * list_two); 

void * list_pop_front(List_t * list);

void * list_pop_back(List_t * lista);

void * list_front(List_t * list);

void * list_back(List_t * list);

// void * list_remove(List_t * list, void * data);

void * list_reverse(List_t * list);


void * list_iterator_value(List_cell_t *);

List_iterator_t * list_iterator(List_t * list);

List_iterator_t * list_iterator_next(List_cell_t * iterator);

bool list_iterator_is_end(List_cell_t * iterator);


#endif /* _LIST_H */
