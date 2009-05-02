/* vim: set sw=4 ts=4 si et: */		

#ifndef _DEGENERATOR_H
#define _DEGENERATOR_H

#include "config.h"
#include "store.h"
#include "list.h"

typedef enum {
	DEGENERATOR_SELECT_RANDOM = 0,
	DEGENERATOR_SELECT_MAX_DEG = 1
} Degenerator_select_mode_t;



typedef struct _Degenerator_t {
	Config_io_t *	_io;

	int 			_iterations;
	int 			_iterations_offset;

	Store_t *      _store;

	nodeindex_t    _size;

	Degenerator_select_mode_t _select_mode;

} Degenerator_t ;


typedef nodeindex_t (* Degenerator_select_fun_t)(Degenerator_t * degenerator);


Degenerator_t * degenerator_create (Config_io_t * io, nodeindex_t n_count, int iterations);

void degenerator_destroy (Degenerator_t * distance);


nodeindex_t degenerator_select_random (Degenerator_t * degenerator);
nodeindex_t degenerator_select_max_deg (Degenerator_t * degenerator);


static Degenerator_select_fun_t degenerator_select_mode_table[] = {
	degenerator_select_random,
	degenerator_select_max_deg
};

void degenerator_set_select_mode (Degenerator_t * degenerator, Degenerator_select_mode_t mode);

#endif
