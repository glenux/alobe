/* vim: set sw=4 ts=4 si et: */		

#ifndef _GYR_CONFIG_H
#define _GYR_CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <zlib.h>

#include "units.h"

typedef enum {
	COMMAND_UNDEF,
	COMMAND_TP1_FILTER_EX2,
	COMMAND_TP1_DEGREE_EX3,
	COMMAND_TP1_STORE_GRAPH_EX4,
	COMMAND_TP1_DENSITY_AVGDEGREE_MAXDEGREE_EX5,
	COMMAND_TP1_CONNEXITY_EX6,
	COMMAND_TP1_CONNEXITY_DEFI,

	COMMAND_TP2_DISTANCE_ONE_TO_OTHER_EX1,
	COMMAND_TP2_PLOT_DISTRIB_DISTANCES_EX2,
	COMMAND_TP2_EVOL_OF_DISTS_FNC_NB_PARCOURS_EX3,
	COMMAND_TP2_LIMIT_INFERIOR_EX4,
	COMMAND_TP2_LIMIT_SUPERIOR_EX5,
	COMMAND_TP2_EVOLUTION_AND_LIMITS_DEFI,

	COMMAND_TP3_DISTRIB_DEG_EX1,
	COMMAND_TP3_EVOL_AVG_DEG_EX2,
	COMMAND_TP3_GENERATE_EX3, 
	COMMAND_TP3_DEGENERATE_EX5,  
	COMMAND_TP3_DEGENERATE_MAX_DEG_EX7
} cmd_t;

typedef struct {
		gzFile 		input;
		FILE * 		output;
		bool		verbose;
} Config_io_t;

typedef struct {
		char * 			_inputfile;
		char * 			_outputfile;

		Config_io_t  	io;
		int 			nodecount;
		int				iterations;
		nodeindex_t		root_index;
		nodeindex_t		filter_size;
		nodeindex_t		filter_offset;
		cmd_t 			command;
} Config_t;

void config_set(Config_t * config);

Config_t * config_create(int argc, char **argv);
bool config_is_valid(Config_t * config);
void config_usage(Config_t * config);
void config_destroy(Config_t * config);

#endif // _GYR_CONFIG_H
