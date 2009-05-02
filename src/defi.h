/* vim: set sw=4 ts=4 si et: */		

#ifndef _GYR_CONNEXITY_H
#define _GYR_CONNEXITY_H

#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>

#include "units.h"
#include "config.h"
#include "connexcomponent.h"

typedef struct {
		Config_io_t * _io;
		nodeindex_t _size;
		ConnexComponentList_t _list;
} Defi_t;


/*
 * nodemetaset * nodeset_list_create()
 * nodemetaset_insert_set(nodeset *);
 * nodemetaset_add_edge(nodeindex_t from, nodeindex_t to);
 */

Defi_t * defi_create(Config_io_t * io, nodeindex_t size);

void defi_destroy(Defi_t * defi);

void defi_append_edge(nodeindex_t from, nodeindex_t to);

void defi_run(Defi_t * store);


#endif
