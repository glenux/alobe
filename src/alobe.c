/* vim: set sw=4 ts=4 si et: */		
#include <time.h>

#include "config.h"
#include "filter.h"
#include "degree.h"
#include "store.h"
#include "defi.h"
#include "distance.h"
#include "generator.h"
#include "degenerator.h"

#include "units.h"

#define DEBUG 1

int main(int argc, char ** argv){
	srand(time(NULL));

	Config_t * config = config_create (argc, argv);
	if (!config_is_valid (config)) { config_usage (config); exit(-1); }
    pDEBUG("Got config from command line\n");

	config_set (config);
    pDEBUG("Config set\n");

	// en fonction de la commande 
	switch (config->command){
		case COMMAND_TP1_FILTER_EX2:
			{
				Filter_t * filter = filter_create(&(config->io), 
						config->filter_size,
						config->filter_offset);
				filter_run (filter);
				filter_destroy (filter);
			}
			break;

		case COMMAND_TP1_DEGREE_EX3:
			{	
				Degree_t * degree = degree_create (&(config->io),	
						config->nodecount,						
						MODE_DEGREE);
				degree_set_output (degree, true);
				degree_fill_from_input_graph (degree);
				degree_destroy (degree);
			}
			break;

		case COMMAND_TP1_STORE_GRAPH_EX4:
			{
				Store_t * store = store_create(&(config->io), 
						config->nodecount);
				store_fill_from_input_graph (store);
				store_destroy (store);
			}
			break;

		case COMMAND_TP1_DENSITY_AVGDEGREE_MAXDEGREE_EX5:
			{
				Degree_t * degree = degree_create (&(config->io),	
						config->nodecount,						
						MODE_DEGSTATS);
				degree_set_output (degree, true);
				degree_fill_from_input_graph (degree);
				degree_destroy (degree);
			}
			break;

		case COMMAND_TP1_CONNEXITY_EX6:
			{
				Store_t * store = store_create(&(config->io), 
						config->nodecount);
				store_fill_from_input_graph (store);
				store_connexity (store, NULL);
				store_destroy (store);
			}
			break;

		case COMMAND_TP1_CONNEXITY_DEFI:
			{
				Defi_t * defi = defi_create(&(config->io),
						config->nodecount);
				defi_run (defi);
				defi_destroy (defi);
			}
			break;

		case COMMAND_TP2_DISTANCE_ONE_TO_OTHER_EX1:
			{
				fprintf(stderr,"Exercice 1 - Ecrire un  programme qui,  étant  donné un\n");
				fprintf(stderr," graphe et un de ses noeud,  calcule la distance  de ce\n");
				fprintf(stderr," noeud  a tous  les  autres,  ainsi que  la moyenne  de\n");
				fprintf(stderr," toutes ces distances.\n\n");

				Distance_t * distance = distance_create((&config->io),
						config->nodecount);
				distance_set_mode (distance, MODE_MAX_AND_AVERAGE);

				distance_set_root (distance, config->root_index);
				distance_set_iterations (distance, 1);

				distance_compute (distance);
				distance_destroy (distance);
			}
			break;

		case COMMAND_TP2_PLOT_DISTRIB_DISTANCES_EX2:
			{
				fprintf(stderr,"Exercice 2 - Tracer la  distribution des  distances à un\n");
				fprintf(stderr," noeud donné, c'est à dire pour chaque entier  i le nom-\n");
				fprintf(stderr," bre de noeuds à distance i du noeud donné.\n\n");
				
				Distance_t * distance = distance_create((&config->io),
						config->nodecount);
				distance_set_mode (distance, MODE_MAX_DISTRIBUTION);

				distance_set_root (distance, config->root_index);
				distance_set_iterations (distance, 1);

				distance_compute (distance);
				distance_destroy (distance);
			}
			break;
		case COMMAND_TP2_EVOL_OF_DISTS_FNC_NB_PARCOURS_EX3:
			{
				fprintf(stderr, "Exercice 3 - Ecrire un  programme  qui permet  de tracer\n");
				fprintf(stderr, " l'évolution de  l'estimation de  la distance moyenne en\n");
				fprintf(stderr, " fonction du nombre de parcours effectués.\n");
				fprintf(stderr, "A chaque étape, le programme calcule la distance moyenne\n");
				fprintf(stderr, " d'un noeud à tous les autres, puis fait la moyenne avec\n");
				fprintf(stderr, " toutes les valeurs précédentes.\n\n");
						
				Distance_t * distance = distance_create((&config->io),
						config->nodecount);
				distance_set_mode (distance, MODE_EVOLUTION);

				distance_set_root (distance, config->root_index);
				distance_set_iterations (distance, config->iterations);

				distance_compute (distance);
				distance_destroy (distance);
			}
			break;
		case COMMAND_TP2_LIMIT_INFERIOR_EX4:
			{
				fprintf(stderr, "Exercice 4 - Ecrire un  programme qui  calcule une borne\n");
				fprintf(stderr, " inférieure du diamètre, en prenant la distance maximale\n");
				fprintf(stderr, " d'un noeud donné à tous les autres.\n\n");

				Distance_t * distance = distance_create((&config->io),
						config->nodecount);
				distance_set_mode (distance, MODE_SHOW_LIMIT_INF);

				distance_set_root (distance, config->root_index);
				distance_set_iterations (distance, config->iterations);

				distance_compute (distance);
				distance_destroy (distance);
			}
			break;
		case COMMAND_TP2_LIMIT_SUPERIOR_EX5:
			{
				fprintf(stderr, "Exercice 5 - Ecrire un programme  qui calcule  une borne\n");
				fprintf(stderr, " supérieure du diamètre, en prenant la distance maximale\n");
				fprintf(stderr, " dans l'arbre du parcours en largeur.\n\n");

				Distance_t * distance = distance_create((&config->io),
						config->nodecount);
				distance_set_mode (distance, MODE_SHOW_LIMIT_SUP);

				distance_set_root (distance, config->root_index);
				distance_set_iterations (distance, config->iterations);

				distance_compute (distance);
				distance_destroy (distance);
			}
			break;
		case COMMAND_TP2_EVOLUTION_AND_LIMITS_DEFI:
			{
				fprintf(stderr, "Défi - Ecrire un  programme qui  donne un  encadrement de\n");
				fprintf(stderr, " plus en plus fin  du diamètre d'un graphe.  Le programme\n");
				fprintf(stderr, " effectue à  chaque étape  un parcours  en largeur et  en\n");
				fprintf(stderr, " déduit une borne inférieure et une borne supérieure pour\n");
				fprintf(stderr, " le diamètre.  Si elle  améliorent le résultat  elle sont\n");
				fprintf(stderr, " conservées  à la  place des meilleures  valeurs trouvées\n");
				fprintf(stderr, " lors des étapes précédentes.\n\n");
				
				Distance_t * distance = distance_create((&config->io),
						config->nodecount);
				distance_set_mode (distance, MODE_DEFI);

				distance_set_root (distance, config->root_index);
				distance_set_iterations (distance, config->iterations);

				distance_compute (distance);
				distance_destroy (distance);
			}
			break;

		case COMMAND_TP3_DISTRIB_DEG_EX1:
			{
				fprintf(stderr, "Exercice 1 - Ecrire un programme qui calcule la distribu-\n");
				fprintf(stderr, " tion des degrés d'un graphe.\n");

				Degree_t * degree = degree_create (&(config->io),	
						config->nodecount,						
						MODE_DEGREE);
				degree_set_output (degree, false);
				degree_fill_from_input_graph (degree);

				degree_output_distribution (degree);
				degree_destroy (degree);

			}
			break;

		case COMMAND_TP3_EVOL_AVG_DEG_EX2:
			{
				Store_t * store = store_create(&(config->io), 
						config->nodecount);
				store_fill_from_input_graph (store);

				store_compute_avg_degree(store, config->iterations);

				store_destroy (store);

			}
			break;

		case COMMAND_TP3_GENERATE_EX3:
			{

				fprintf(stderr, "Generating a random graph\n");

				Generator_t * generator = generator_create(&(config->io),
						config->nodecount,
						config->filter_size);

				generator_run (generator);
				generator_destroy (generator); 
			}
			break;
			
		case COMMAND_TP3_DEGENERATE_EX5:
			{
				fprintf(stderr, "Degenerating input graph\n");
				Degenerator_t * degen = degenerator_create (&(config->io),
						config->nodecount, config->iterations);						
				degenerator_set_select_mode (degen, DEGENERATOR_SELECT_RANDOM);

				degenerator_compute (degen);
				degenerator_destroy (degen);
			}
			break;
	
		case COMMAND_TP3_DEGENERATE_MAX_DEG_EX7:
			{
				fprintf(stderr, "Degenerating input graph with max degrees first\n");
				Degenerator_t * degen = degenerator_create (&(config->io),
						config->nodecount, config->iterations);						
				degenerator_set_select_mode (degen, DEGENERATOR_SELECT_MAX_DEG);

				degenerator_compute (degen);
				degenerator_destroy (degen);
			}
			break;

		default:
			{
				printf("Undefined mode\n");
			}
			break;
	}

	config_destroy (config);

	return EXIT_SUCCESS;
}
