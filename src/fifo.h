/* vim: set sw=4 ts=4 si et: */		

#ifndef _FIFO_H
#define _FIFO_H

#include "config.h"

typedef struct {
	nodeindex_t *	_data;

	nodeindex_t 	_fill;
	nodeindex_t 	_size;
	nodeindex_t  	_head;
	nodeindex_t 	_tail;
} Fifo_t;


Fifo_t * 	fifo_create(nodeindex_t size);

void	fifo_push(Fifo_t * fifo, nodeindex_t node);

Fifo_t * fifo_copy(Fifo_t * fifo);

void fifo_reverse(Fifo_t * fifo);

bool fifo_is_empty(Fifo_t * fifo);

nodeindex_t fifo_front(Fifo_t * fifo);

nodeindex_t fifo_pop(Fifo_t * fifo);

nodeindex_t fifo_get_size(Fifo_t * fifo);

void fifo_display(Fifo_t * fifo);

void	fifo_destroy(Fifo_t * fifo);

nodeindex_t fifo_get_random(Fifo_t * fifo);

#endif // _FIFO_H
