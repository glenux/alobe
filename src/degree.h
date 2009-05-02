/* vim: set sw=4 ts=4 si et: */		

#ifndef _GYR_DEGREE_H
#define _GYR_DEGREE_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <zlib.h>

#include "units.h"
#include "config.h"

typedef enum {
	MODE_DEGREE,
	MODE_DEGSTATS
} Degree_mode_t;

typedef struct {
		Config_io_t *	_io;
		
		bool 			_show_output;
		Degree_mode_t 	_mode;
		
		nodeindex_t 	_size;
		nodeindex_t 	_minIdx;
		nodeindex_t 	_maxIdx;

		nodeindex_t 	arcs;
		float 			average;
		nodeindex_t 	maximum;
		float 			density;

		nodeindex_t * 	_data;
} Degree_t;

Degree_t * degree_create(Config_io_t * io, nodeindex_t size, Degree_mode_t mode);
void degree_destroy(Degree_t * degree);
void degree_display(Degree_t * degree);

void degree_set_output(Degree_t * degree, bool out);

void degree_set(Degree_t * degree, nodeindex_t nodeidx, nodeindex_t value);

nodeindex_t degree_get(Degree_t * degree, nodeindex_t nodeidx);

nodeindex_t degree_begin(Degree_t * degree);
nodeindex_t degree_end(Degree_t * degree);
nodeindex_t degree_size(Degree_t * degree);
void degree_run(Degree_t * degree);

#endif
