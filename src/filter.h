/* vim: set sw=4 ts=4 si et: */		

#ifndef _GYR_FILTER_H
#define _GYR_FILTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#include "units.h"
#include "config.h"

typedef struct {
		Config_io_t * _io;
		int _size;
		int _offset;

} Filter_t;

Filter_t * filter_create(Config_io_t * io, nodeindex_t size, nodeindex_t offset);
void filter_run(Filter_t * filter);
void filter_destroy(Filter_t * filter);

#endif
