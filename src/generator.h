/* vim: set sw=4 ts=4 si et: */		

#ifndef _GENERATOR_H
#define _GENERATOR_H

#include "config.h"
#include "list.h"

typedef struct {
	Config_io_t *	_io;

	int 			_iterations;
	nodeindex_t     _node_count;
	nodeindex_t     _edge_count;

    List_t **   _data;
} Generator_t ;


Generator_t * generator_create (Config_io_t * io, nodeindex_t n_count, nodeindex_t e_count);

void generator_destroy (Generator_t * distance);

#endif
