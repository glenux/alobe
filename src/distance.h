/* vim: set sw=4 ts=4 si et: */		

#ifndef _DISTANCE_H
#define _DISTANCE_H

#include "config.h"
#include "store.h"
#include "fifo.h"

typedef struct {
	float average;
	nodeindex_t cc_size;
	nodeindex_t maximum;
	Fifo_t *  visited_nodes;
} distance_computation_t;

typedef enum {
	MODE_UNDEF,
	MODE_MAX_AND_AVERAGE,
	MODE_MAX_DISTRIBUTION,
	MODE_EVOLUTION,
	MODE_SHOW_LIMIT_INF,
	MODE_SHOW_LIMIT_SUP,
	MODE_DEFI
} distance_mode_t ;

typedef struct {
	Config_io_t *	_io;

	distance_mode_t _mode;
	Store_t * 		_store;
	int 			_iterations;
	nodeindex_t 	_size;
	nodeindex_t		_root;
	//				Fifo_t * 		_fifo;
} Distance_t ;


Distance_t * distance_create (Config_io_t * io, nodeindex_t size);

void distance_compute(Distance_t * distance);

void distance_destroy (Distance_t * distance);

distance_computation_t * distance_all_from_one_node(
		Distance_t * distance, 
		nodeindex_t cc_root,
		int iteration);

void distance_set_root(Distance_t * distance, nodeindex_t rootidx);

void distance_set_mode(Distance_t * distance, distance_mode_t mode);

nodeindex_t distance_width (Distance_t * distance, Fifo_t * fifo_visit);

void distance_set_iterations(Distance_t * distance, int iterations);


#endif
