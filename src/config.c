/* vim: set sw=4 ts=4 si et: */		

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

#define	DEBUG	1

/**************************************************************************
 * Constructeur 
 *
 * @param argc Nombre de parametres
 * @param argv Tableau vers les chaines de caracteres contenant les 
 *             parametres
 */

Config_t * config_create(int argc, char **argv) {
	Config_t * config = (Config_t*)malloc(sizeof(Config_t));

	config->command = COMMAND_UNDEF;
	config->filter_size = -1;
	config->root_index = NODEINDEX_RANDOM;
	config->filter_offset = -1;
	config->_inputfile = strdup("-");
	config->_outputfile = strdup("-");
	config->nodecount = -1;
	config->io.verbose = false;

	while (1) {
		static struct option long_options[] = {
			//{"compress1", no_argument, 0, 'c'},

			/* TP1 */
			{"tp1filter", no_argument, 0, 'F'},
			{"tp1degree", no_argument, 0, 'D'},
			{"tp1store", no_argument, 0, 'S'},
			{"tp1average", no_argument, 0, 'A'},
			{"tp1connexity", no_argument, 0, 'C'},
			{"tp1defi", no_argument, 0, 'E'},
			
			/* TP2 */
			{"tp2distance", no_argument, 0, 'I'},
			{"tp2distanceplot", no_argument, 0, 'J'},
			{"tp2distevolution", no_argument, 0, 'L'},
			{"tp2limitinf", no_argument, 0, 'M'},
			{"tp2limitsup", no_argument, 0, 'N'},
			{"tp2defi", no_argument, 0, 'O'},

			/* TP3 */
			{"tp3distribdeg", no_argument, 0, 'H'},
			{"tp3evolavgdeg", no_argument, 0, 'K'},
			{"tp3generate", no_argument, 0, 'G'},
			{"tp3degenerate", no_argument, 0, 'B'},
			{"tp3degenmaxdeg", no_argument, 0, 'P'},

			{"verbose", no_argument, 0, 'v'},
			
			{"input", required_argument, 0, 'i'},
			{"count", required_argument, 0, 'c'},
			{"output", required_argument, 0, 'o'},
			{"iterations", required_argument, 0, 'n'},

			{"root", required_argument, 0, 'r'},
			{"size", required_argument, 0, 's'},
			{"offset", required_argument, 0, 't'},

			{0, 0, 0, 0}
		};

		int option_index = 0;

		int c = getopt_long(argc, argv, "vABCDEFGHIJKLMNOPSc:i:o:s:t:r:n:",
				long_options, &option_index);

		/* detect the end of options */
		if (c == -1) {
			break;
		}

		switch (c) {
			case 0:
				pDEBUG("case NULL\n");
				break;

			case 'r':
				pDEBUG("Parameter -> Root index = %s\n",optarg);
				if (strcmp(optarg,"rand") == 0){
					config->root_index = NODEINDEX_RANDOM;
				} else if (strcmp(optarg,"max") == 0){
					config->root_index = NODEINDEX_MAX_CC;
				} else {
					sscanf(optarg, "%ld", &(config->root_index));
				}
				break;

			case 'n':
				pDEBUG("Parameter -> Iterations = %s\n",optarg);
				sscanf(optarg, "%d", &(config->iterations));
				break;

			case 't':
				pDEBUG("Parameter -> Filter offset = %s\n",optarg);
				sscanf(optarg, "%ld", &(config->filter_offset));
				break;

			case 's':
				pDEBUG("Parameter -> Edge count = %s\n",optarg);
				sscanf(optarg, "%ld", &(config->filter_size));
				break;
				
			case 'o':
				pDEBUG("Parameter -> Output file = %s\n",optarg);
				free(config->_outputfile);
				config->_outputfile = strdup(optarg);
				break;

			case 'i':
				pDEBUG("Parameter -> Input file -> %s\n",optarg);
				free(config->_inputfile);
				config->_inputfile = strdup(optarg);
				break;

			case 'c':
				pDEBUG("Parameter -> Node count -> %s\n",optarg);
				sscanf(optarg, "%d", &(config->nodecount));
				break;

			case 'A':
				pDEBUG("Command -> TP1 AVERAGE\n");
				config->command = COMMAND_TP1_DENSITY_AVGDEGREE_MAXDEGREE_EX5;
				break;
				
			case 'B':
				pDEBUG("Command -> TP3 DEGENERATE\n");
				config->command = COMMAND_TP3_DEGENERATE_EX5;
				break;

			case 'C':
				pDEBUG("Command -> TP1 CONNEXITY\n");
				config->command = COMMAND_TP1_CONNEXITY_EX6;
				break;

			case 'D':
				pDEBUG("Command -> TP1 DEGREE\n");
				config->command = COMMAND_TP1_DEGREE_EX3;
				break;

			case 'E':
				pDEBUG("Command -> TP1 DEFI\n");
				config->command = COMMAND_TP1_CONNEXITY_DEFI;
				break;

			case 'F':
				pDEBUG("Command -> TP1 FILTER\n");
				config->command = COMMAND_TP1_FILTER_EX2;
				break;

			case 'G':
				pDEBUG("Command -> TP3 GENERATE\n");
				config->command = COMMAND_TP3_GENERATE_EX3;
				break;

			case 'H':
				pDEBUG("Command -> TP3 DISTRIB DEGREEE\n");
				config->command = COMMAND_TP3_DISTRIB_DEG_EX1;
				break;

			case 'I':
				pDEBUG("Command -> TP2 DISTANCE\n");
				config->command = COMMAND_TP2_DISTANCE_ONE_TO_OTHER_EX1;
				break;

			case 'J':
				pDEBUG("Command -> TP2 PLOT DISTRIB DIST\n");
				config->command = COMMAND_TP2_PLOT_DISTRIB_DISTANCES_EX2;
				break;

			case 'K':
				pDEBUG("Command -> TP3 EVOL AVG DEGREEE\n");
				config->command = COMMAND_TP3_EVOL_AVG_DEG_EX2;
				break;

			case 'L':
				pDEBUG("Command -> TP2 DISTANCES EVOLUTION OF DISTS / ITERATIONS\n");
				config->command = COMMAND_TP2_EVOL_OF_DISTS_FNC_NB_PARCOURS_EX3;
				break;

			case 'M':
				pDEBUG("Command -> TP2 DISTANCES LIMIT INFERIOR\n");
				config->command = COMMAND_TP2_LIMIT_INFERIOR_EX4;
				break;

			case 'N':
				pDEBUG("Command -> TP2 DISTANCES LIMIT SUPERIOR\n");
				config->command = COMMAND_TP2_LIMIT_SUPERIOR_EX5;
				break;

			case 'O':
				pDEBUG("Command -> TP2 DISTANCES EVOLUTION AND LIMITS DEFI\n");
				config->command = COMMAND_TP2_EVOLUTION_AND_LIMITS_DEFI;
				break;
	
			case 'P':
				pDEBUG("Command -> TP3 DEGENERATE MAX DEG FIRST\n");
				config->command = COMMAND_TP3_DEGENERATE_MAX_DEG_EX7;
				break;

			case 'S':
				pDEBUG("Command -> TP1 STOREGRAPH\n");
				config->command = COMMAND_TP1_STORE_GRAPH_EX4;
				break;
				
			case 'v':
				pDEBUG("Option -> Verbose mode\n");
				config->io.verbose = true;
				break;

			case '?':
				pDEBUG("Unknow option -> '?'\n");
				exit(-1);
				break;

			default:
				pDEBUG("Unknow option -> Default \n");
				exit(-1);
		}
	}
	return config;
}


/**************************************************************************
 * Organise les fichiers d'entrée/sortie dans l'objet Config_t.
 *
 * ATTENTION: Etape obligatoire apres la création du Config_t.
 *
 * @param config	Pointeur sur l'objet Config_t
 */

void config_set(Config_t * config){
	// on utilise STDIN si le parametre est "-" et un fichier d'entrée (gz?) sinon
	if (0 == strcmp (config->_inputfile, "-")) { 
		config->io.input = gzdopen (fileno (stdin), "rb"); 
	} else { 
		config->io.input = gzopen (config->_inputfile, "rb"); 
	}
	if (config->io.input == NULL) {
		fprintf (stderr, "%s at %s:%d -- Err: can't gzopen %s\n", 
				__func__, __FILE__, __LINE__, config->_inputfile);
		exit (1);
	}

	// on utilise STDOUT si le parametre est "-" est un fichier de sortie sinon
	if (0 == strcmp(config->_outputfile, "-")){ 
		config->io.output = stdout;
	} else { 
		config->io.output = fopen(config->_outputfile, "wb"); 
	}
	if (config->io.output == NULL){
		perror(config->_outputfile);
		exit (1);
	}
}


/**************************************************************************
 * Destructeur
 */

void config_destroy(Config_t * config){
	gzclose(config->io.input);
	fclose(config->io.output);

	free(config->_inputfile);
	free(config->_outputfile);
	free(config);
}


/**************************************************************************
 * Vérifie la validité des options de configuration
 */

bool config_is_valid(Config_t * config) {
	int score = 0;
	int valid = 0;

	if (strlen(config->_inputfile) > 0) {
		score++;
	}
	valid++;

	if (config->nodecount > 0) {
		score++;
	}
	valid++;

	if (strlen(config->_outputfile) > 0) {
		score++;
	}
	valid++;

	return (valid == score);
}


/**************************************************************************
 * Affiche sur la sortie standard les paramètres acceptés
 */

void config_usage(Config_t * config) {
	printf("Usage: alobe [options]\n");
	printf("\n");
	printf("Mandatory options:\n");
	printf("-i, -input-data <string>   Input filename\n");
	printf("-c, -input-count <string>  Node count\n");
	printf("-o, -output <string>       Input filename\n");
	printf("-n, -iterations <int>      Number of iterations\n");
	printf("-t, -offset <int>          Filter offset\n");
	printf("-s, -size <int>            Edge count\n");
	printf("-r, -root <value>          Root node. Value in : <integer> | 'max' | 'rand' (default 'rand')\n");
	printf("\n");
	printf("-F, -tp1filter             Cmd: TP1 Filter\n");
	printf("-D, -tp1degree             Cmd: TP1 Compute degree\n");
	printf("-S, -tp1store              Cmd: TP1 Fill the store\n");
	printf("-A, -tp1average            Cmd: TP1 Compute the average degree\n");
	printf("-C, -tp1connexity          Cmd: TP1 Find connex components\n");
	printf("-E, -tp1defi               Cmd: TP1 Find connex components (defi mode)\n");
	printf("\n");
	printf("-I, -tp2distance           Cmd: TP2 Compute the distances for one node\n");
	printf("-J, -tp2distanceplot       Cmd: TP2 Plot the distances for one node\n");
	printf("-L, -tp2distevolution      Cmd: TP2 Plot evolution of average dist\n");
	printf("-M, -tp2limitinf           Cmd: TP2 Borne inf\n");
	printf("-N, -tp2limitsup           Cmd: TP2 Borne sup\n");
	printf("-O, -tp2defi               Cmd: TP2 Defi\n");
	printf("\n");
	printf("-H, -tp3distribdeg         Cmd: TP3 Plot the distribution of degrees\n");
	printf("-K, -tp3evolavgdeg         Cmd: TP3 Evolution of average degree\n");
	printf("-G, -tp3generate           Cmd: TP3 Generate a random graph\n");
	printf("-B, -tp3degenerate         Cmd: TP3 Degenerate input graph (random)\n");
	printf("-P, -tp3degenmaxdeg        Cmd: TP3 Degenerate input graph (max degree first)\n");

}

