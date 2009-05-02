/* vim: set sw=4 ts=4 si et: */		

#ifndef _GYR_STORE_H
#define _GYR_STORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>


#include "units.h"
#include "config.h"
#include "degree.h"
#include "fifo.h"

typedef enum {
	STORE_RESET_DEGREE = 1,
	STORE_RESET_VALUE = 2,
	STORE_RESET_VALUE2 = 4,
	STORE_RESET_REF = 8,
	STORE_RESET_ALL = 15
} store_reset_t;

typedef enum {
	STORE_MODIF_UNDEF = 0,
	STORE_MODIF_RESET = 1,
	STORE_MODIF_BEGIN = 2,
	STORE_MODIF_END = 3
} store_modif_t;

typedef struct {
		Config_io_t *	_io;

		nodeindex_t **	_refs;
		nodeindex_t * 	_thetable;
		nodeindex_t 	_size;

		store_modif_t	_mod_degree;
		store_modif_t	_mod_value;
		store_modif_t	_mod_value2;
		store_modif_t	_mod_ref;
		
		bool			_show_output;
} Store_t;


Store_t * store_create(Config_io_t * io, nodeindex_t size);

void store_destroy(Store_t * store);

void store_fill(Store_t * store);

void store_set_output(Store_t * store, bool output);

void store_add_adjacent(Store_t * store, nodeindex_t from, nodeindex_t to);

nodeindex_t store_get_degree(Store_t * store, nodeindex_t from);

nodeindex_t store_get_adjacent(Store_t * store, nodeindex_t from, nodeindex_t index);

void store_set_value(Store_t * store, nodeindex_t from, nodeindex_t val);

void store_set_value_2(Store_t * store, nodeindex_t from, nodeindex_t val);

void store_init_from_degrees (Store_t * store, Degree_t * degree);
	
void store_fill_from_random_degree (Store_t * store, nodeindex_t edge_count);

nodeindex_t store_get_ref(Store_t * store, nodeindex_t from);

nodeindex_t store_get_value(Store_t * store, nodeindex_t from);

nodeindex_t store_get_value_2(Store_t * store, nodeindex_t from);

void store_del_node (Store_t * store, nodeindex_t from);

void store_set_ref(Store_t * store, nodeindex_t from, nodeindex_t val);

bool store_is_visit_done(Store_t * store, nodeindex_t index);

bool store_is_visit_undone(Store_t * store, nodeindex_t index);

void store_visit_node(Store_t * store, nodeindex_t index, nodeindex_t reference, Fifo_t * fifo_cc);

void store_connexity(Store_t * store, Fifo_t * fifo_indexes);
void store_reset(Store_t * store, store_reset_t mode);

void store_fill_cc_from_node(Store_t * store, nodeindex_t from, Fifo_t * fifo_cc );

#endif
