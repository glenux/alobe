/* vim: set sw=4 ts=4 si et: */		

#include <assert.h>
#include "distance.h"

#define DEBUG			0

/**************************************************************************
 * Constructeur 
 *
 * @return Pointeur sur l'objet Distance_t
 */

Distance_t * distance_create (Config_io_t * io, nodeindex_t size){
	Distance_t * distance = (Distance_t *) malloc (sizeof(Distance_t));

	distance->_io = io;
	distance->_size = size;
	distance->_mode = MODE_UNDEF;
	distance->_root = NODEINDEX_ROOT;
	//	distance->_fifo = fifo_create(size);
	distance->_store = store_create (io, size);
	store_set_output(distance->_store, false);

	store_fill_from_input_graph (distance->_store);
	return distance;
}


void distance_destroy(Distance_t * distance){
	Store_t * store = distance->_store;
	store_destroy (store);
	free(distance);
}

/**************************************************************************
 * Fixe le nombre d'itérations pour les calculs de distance
 *
 * @param distance 		Pointeur sur l'objet Distance_t
 * @param iterations 	Nombre d'itérations 
 */

void distance_set_iterations(Distance_t * distance, int iterations){

	pDEBUG("iterations %d\n", iterations);

	distance->_iterations = iterations;
}


/**************************************************************************
 * Fixe la racine pour le calcul des distances
 *
 * @param distance 		Pointeur sur l'objet Distance_t
 * @param rootidx		Index du noeud racine pour le calcul 
 */

void distance_set_root(Distance_t * distance, nodeindex_t rootidx){

	pDEBUG("root %ld\n", rootidx);

	distance->_root = rootidx;
}


/**************************************************************************
 * Fixe le mode d'affichage lors du calcul
 *
 * @param distance 		Pointeur sur l'objet Distance_t
 * @param mode			Mode selectionn&eacute;
 */

void distance_set_mode(Distance_t * distance, distance_mode_t mode){
	pDEBUG("mode %d\n", mode);

	distance->_mode = mode;
}


/**************************************************************************
 * Retourne tous les noeuds de la plus grande composante connexe
 *
  * ATTENTION: modifie STORE_EXTRA_VALUE
 *
 * ATTENTION: modifie STORE_EXTRA_VALUE2
 *
 * ATTENTION: modifie STORE_EXTRAPLACEles champs EXTRA des noeuds du distance->_store
 *
 * @return	Un pointeur sur une Fifo_t contenant les index des noeuds
 */

Fifo_t * distance_get_roots_from_max_cc(Distance_t * distance){
	pDEBUG("FIXME: NOT IMPLEMENTED\n");
#warning "FIXME: NOT IMPLEMENTED"
	return NULL;
}

/**************************************************************************
 * Retourne tous les noeuds d'une m&ecirc;me composante connexe
 *
 * ATTENTION: modifie STORE_EXTRA_VALUE
 *
 * ATTENTION: modifie STORE_EXTRA_VALUE2
 *
 * ATTENTION: modifie STORE_EXTRAPLACEles champs EXTRA des noeuds du distance->_store
 *
 * @return	Un pointeur sur une Fifo_t contenant les index des noeuds
 */

Fifo_t * distance_get_roots_from_random_cc(Distance_t * distance){
	Fifo_t * result;
	nodeindex_t selected_cc;
	/* on parcours les composantes connexes */
	Fifo_t * fifo_cc_list = fifo_create(distance->_size);

	pDEBUG("root not selected, preparing the cc list\n");

	store_connexity (distance->_store, fifo_cc_list);

	/* on choisi une index de composante au hasard*/
	selected_cc = fifo_get_random (fifo_cc_list);  
	printf("Composantes connexes disponibles : ");
	fifo_display(fifo_cc_list);

	pDEBUG("selected reference node %ld from the cc list\n", selected_cc);

	fifo_destroy (fifo_cc_list);

	pDEBUG("destroying the cc list\n");
	pDEBUG("reset of the store\n");

	store_reset (distance->_store, STORE_RESET_ALL ^ STORE_RESET_DEGREE);

	/* on crée un fifo fifo_roots avec les noeuds a parcourir... */
	result = fifo_create(distance->_size);
	/* on met tous les noeuds de la composante connexe dans le fifo fifo_roots */	
	store_fill_cc_from_node (distance->_store, selected_cc, result);	

	pDEBUG("display the cc of node\n");
	return result;
}


/**************************************************************************
 * Calcul de la distance
 *
 * @param distance 		Pointeur sur l'objet Distance_t
 */

void distance_compute(Distance_t * distance){
	
	pDEBUG("begin\n");
	int cur_iteration;
	// nodeindex_t root;
	Fifo_t * fifo_roots = NULL;
	nodeindex_t limit_inferior = 0;
	nodeindex_t limit_superior = distance->_size;
	float average_average_distance = 0;
	nodeindex_t max_max_distance = 0;

	if (distance->_root < 0){
		if (distance->_root == NODEINDEX_RANDOM){
			fifo_roots = distance_get_roots_from_random_cc(distance);
		} else if (distance->_root == NODEINDEX_MAX_CC) {
			fifo_roots = distance_get_roots_from_max_cc(distance);
		} else {
			pDEBUG("C'est quoi ce choix de noeud ????");
			exit(4);
		}
	} else {
		store_reset (distance->_store, STORE_RESET_ALL ^ STORE_RESET_DEGREE);
		fifo_roots = fifo_create(distance->_size);
		store_fill_cc_from_node(distance->_store, distance->_root, fifo_roots);
		/* fifo_push (fifo_roots, distance->_root); */
	}
	printf("Noeuds utilisables comme racines : ");
	fifo_display(fifo_roots);

	for (cur_iteration = 0; cur_iteration < distance->_iterations; cur_iteration++){
		pDEBUG("iteration %d begin \n", cur_iteration);
		distance_computation_t * computation;

		nodeindex_t root_cc;
		if (distance->_mode == MODE_DEFI){
			// FIXME: selection optimisée des noeuds
			root_cc = fifo_get_random (fifo_roots);
		} else {
			if ((distance->_root >= 0) && (cur_iteration == 0)) {
					pDEBUG("first of roots %ld\n", distance->_root);
					root_cc = fifo_front (fifo_roots);
			} else {
				pDEBUG("random root\n");
				root_cc = fifo_get_random (fifo_roots);
			}
		}
		printf("Iteration %d -- choosing root %ld\n", cur_iteration, root_cc);

		computation = distance_all_from_one_node (distance, root_cc, cur_iteration);

		pDEBUG("Ordre de visite : ");
		if (DEBUG) fifo_display(computation->visited_nodes);

		max_max_distance = MAX(max_max_distance, computation->maximum);

		average_average_distance += computation->average;

		float approx_average_distance = (
				(float)average_average_distance 
				/ (float)(1 + cur_iteration));

		/* on sait que la longueur max peut servir de borne inférieure
		 * on choisit donc la limite inférieure maximale
		 */

		limit_inferior = MAX(computation->maximum, limit_inferior);

		if ((distance->_mode == MODE_SHOW_LIMIT_SUP)
				|| (distance->_mode == MODE_DEFI)) {
			/* tous les noeuds ont été visités, on calcule alors la distance
			 * maximale dans l'arbre (largeur) , et on garde a chaque fois le
			 * minimum */

			nodeindex_t width = distance_width (distance,
					computation->visited_nodes); 

			limit_superior = MIN(limit_superior, width);
		}

		switch(distance->_mode){
			case MODE_MAX_AND_AVERAGE: 
				{
					fprintf (distance->_io->output, 
							"Maximum distance : %ld\n", 
							computation->maximum);
					fprintf (distance->_io->output, 
							"Average distance : %f\n", 
							computation->average);
				}
				break;
			case MODE_MAX_DISTRIBUTION:
				{
					/* affichage déporté dans le calcul des distances */
				};
				break;
			case MODE_EVOLUTION:
				{
					fprintf(distance->_io->output, "%d %f\n", 
							cur_iteration, 
							approx_average_distance);
				}
				break;
			case MODE_SHOW_LIMIT_INF:
				{
					fprintf (distance->_io->output, "%d %ld\n", 
							cur_iteration,
							limit_inferior);
					printf ("             -- borne inférieure %ld\n", 
							limit_inferior);
				};
				break;
			case MODE_SHOW_LIMIT_SUP:
				{

					fprintf (distance->_io->output, "%d %ld\n", 
							cur_iteration,
							limit_superior);
					printf ("             -- borne supérieure %ld\n", 
							limit_superior);

				};
				break;
			case MODE_DEFI:
				{
					/* tous les noeuds ont été visités, on calcule alors la distance
					 * maximale dans l'arbre (largeur) , et on garde a chaque fois le
					 * minimum */

					fprintf (distance->_io->output, "inf %d %ld\n", 
							cur_iteration,
							limit_inferior);

					fprintf(distance->_io->output, "avg %d %f\n", 
							cur_iteration, 
							approx_average_distance);

					fprintf (distance->_io->output, "sup %d %ld\n", 
							cur_iteration,
							limit_superior);

				};
				break;
			default: 
				assert(1==0);
				break;
		}

		fifo_destroy (computation->visited_nodes);
		free (computation);
		pDEBUG("               -- end \n");
	}

	fifo_destroy(fifo_roots);

	average_average_distance = 
		(float)average_average_distance 
		/ (float)(distance->_iterations);

	if ((distance->_mode == MODE_MAX_AND_AVERAGE)
			&& (distance->_mode != MODE_MAX_DISTRIBUTION)) {
		printf ("Final maximum distance : %ld\n", max_max_distance);
		printf ("Final average distance : %f\n", average_average_distance);
	}

	pDEBUG("end\n");
}


/**************************************************************************
 * Calcule de toutes les distances à partir d'un unique noeud 
 *
 * ATTENTION: utilise STORE_EXTRA_DEGREE
 */

distance_computation_t * distance_all_from_one_node(
		Distance_t * distance, 
		nodeindex_t cc_root,
		int iteration)
{
	/** oblige a avoir initilisé les degrés */
	assert(distance->_store->_mod_degree == STORE_MODIF_END);

	distance_computation_t * result;
	result = (distance_computation_t *) malloc (sizeof(distance_computation_t));
	result->average = 0;
	result->cc_size = 0;
	result->maximum = 0;
	result->visited_nodes = fifo_create(distance->_size);

	/* choisir aléatoirement une valeur dans le Fifo_t */		
	nodeindex_t root = cc_root; 

	/* on reset le store afin de pouvoir faire des calculs dessus */
	store_reset (distance->_store, STORE_RESET_ALL ^ STORE_RESET_DEGREE);

	/* on indique au store qu'on s'apprete a faire des modifications sur
	 * les références */
	distance->_store->_mod_ref = STORE_MODIF_BEGIN;
	distance->_store->_mod_value = STORE_MODIF_BEGIN;

	nodeindex_t dist_nodecounter = 0; /* compteur de noeuds pour une distance donnée */
	nodeindex_t dist_curdist = 0; /* distance comptée actuelle */
	nodeindex_t root_distance;
	nodeindex_t root_oldvalue = root;

	Fifo_t * fifo_remaining_nodes = fifo_create(distance->_size);
	pDEBUG("choosing %ld as the root\n", root);
	fifo_push (fifo_remaining_nodes, root);

	store_set_ref (distance->_store, root, NODEINDEX_ROOT);
	store_set_value (distance->_store, root, 0); /* distance 0 */

	while (!fifo_is_empty (fifo_remaining_nodes)) {
		result->cc_size += 1;
		root_oldvalue = root;
		root = fifo_pop (fifo_remaining_nodes);
		fifo_push (result->visited_nodes, root);

		root_distance = store_get_value (distance->_store, root);

		if (distance->_mode == MODE_MAX_DISTRIBUTION){
			if ((root_distance > dist_curdist) || (root_distance > result->maximum)){
				fprintf(distance->_io->output, "%ld %ld\n", dist_curdist, dist_nodecounter);
				dist_curdist = root_distance;
				dist_nodecounter = 0;
			}
			dist_nodecounter += 1;
		}

		if (root_distance > result->maximum) {
			result->maximum = root_distance;
		}
		result->average += root_distance;

		nodeindex_t cur;
		nodeindex_t degree = store_get_degree (distance->_store, root);

		/* pour tous les noeuds adjacents, ajouter a la fifo */
		for (cur = 0; cur < degree; cur++){
			nodeindex_t node = store_get_adjacent (distance->_store, root, cur);
			/* printf("Adjacent node %ld\n", node); */

			store_modif_t ref_status = distance->_store->_mod_ref;
			distance->_store->_mod_ref = STORE_MODIF_END;

			bool is_undone = store_is_visit_undone (distance->_store, node);
			distance->_store->_mod_ref = ref_status;

			if (is_undone){
				nodeindex_t cur_distance = root_distance + 1;

				store_set_ref (distance->_store, node, root);
				store_set_value (distance->_store, node, cur_distance);
				fifo_push (fifo_remaining_nodes, node);
			}
		}

		/* parcourir tous les noeuds de la composante connexe, si on les connait pas */

	} /* end while fifo_is_empty*/

	/* on a fini de faire des modifications sur les références */
	distance->_store->_mod_value = STORE_MODIF_END;
	distance->_store->_mod_ref = STORE_MODIF_END;

	pDEBUG("done\n");
	result->average = ((float)(result->average) / (float)(result->cc_size));

	return result;
}


/**************************************************************************
 * Trouve la plus grande distance dans l'arbre du parcours en largeur
 * FIXME: probablement une erreur de calcul dans la largeur pour les
 * arbres plats a nombreux voisins
 *
 * ATTENTION: utilise STORE_EXTRA_REF
 *
 * ATTENTION: modifie STORE_EXTRA_VALUE 
 *
 * ATTENTION: modifie STORE_EXTRA_VALUE2
 *
 * VALUE indique la hauteur et 
 * VALUE2 indique la largeur max
 */

nodeindex_t distance_width (Distance_t * distance, Fifo_t * fifo_visit){
	Fifo_t * fifo_inverse_visit = fifo_copy (fifo_visit);	
	fifo_reverse (fifo_inverse_visit);

	assert(distance->_store->_mod_ref == STORE_MODIF_END);

	store_reset (distance->_store, STORE_RESET_VALUE | STORE_RESET_VALUE2);
	distance->_store->_mod_value = STORE_MODIF_BEGIN;
	distance->_store->_mod_value2 = STORE_MODIF_BEGIN;

	nodeindex_t last_node = NODEINDEX_UNDEF;

	while(!fifo_is_empty(fifo_inverse_visit)){
		nodeindex_t cur_node = fifo_pop (fifo_inverse_visit);
		last_node = cur_node;
		pDEBUG("visiting %ld\n", cur_node);	

		nodeindex_t father_node = store_get_ref (distance->_store, cur_node);

		nodeindex_t cur_height = store_get_value (distance->_store, cur_node);
		nodeindex_t cur_width = store_get_value_2 (distance->_store, cur_node);


		if (cur_height == NODEINDEX_UNDEF){
			/* hauteur zéro pour le noeud si jamais visité, car
			 * il s'agit d'une feuille */
			cur_height = 0;
		}
		if (cur_width == NODEINDEX_UNDEF){
			/* largeur zéro pour le noeud si jamais visité, car
			 * il s'agit d'une feuille */
			cur_width = 0;
		}

		store_set_value (distance->_store, cur_node, cur_height);
		store_set_value_2 (distance->_store, cur_node, cur_width);

		pDEBUG("   - %ld height = %ld\n", cur_node, cur_height);
		pDEBUG("   - %ld width = %ld\n", cur_node, cur_width);

		if (father_node != NODEINDEX_ROOT){
			nodeindex_t father_height = store_get_value (distance->_store, father_node);
			nodeindex_t father_width = store_get_value_2 (distance->_store, father_node);

			if (father_height == NODEINDEX_UNDEF){
				/* hauteur zéro pour le noeud si jamais visité */
				father_height = 0;
			}
			if (father_width == NODEINDEX_UNDEF){
				/* largeur zéro pour le noeud si jamais visité */
				father_width = 0;
			}

			nodeindex_t old_father_height = father_height;

			father_height = 
				MAX( 
						cur_height + 1, 
						father_height
				   );

			/* marche pas 
			father_width = MAX(
					cur_width + 1, // passe par cur_node
					MAX(
						//father_height + cur_height, 
						father_height + cur_height + 1, //passe par cur_node et father_node
						father_width // pas dans le sous arbre de cur_node
					   )
			   );
			*/
			/* marche */

			/*
			 * printf("cur plus grand        : %ld\n", cur_width);
			 * printf("plus grand déja passé : %ld\n", father_width);
			 * printf("rejoint les plus longs: %ld\n", old_father_height + cur_height + 1);
			 */

			father_width = 
				MAX(
						MAX(
							// si cur est le plus grand
							cur_width, 
							// si le plus grand est déja passé par là avant
							father_width
							), 
						old_father_height + cur_height  + 1// on rejoint deux plus longs chemins
				   );
			/**/
			/*
			   father_width = 
			   MAX(
			   cur_width + 1, 
			   MAX(
			   father_height + cur_height, 
			   father_width
			   )
			   );
			   */
			store_set_value (distance->_store, father_node, father_height);
			store_set_value_2 (distance->_store, father_node, father_width);

			pDEBUG("   - father %ld\n", father_node);
			pDEBUG("       - height = %ld\n", father_height);
			pDEBUG("       - width = %ld\n", father_width);
		}

	}

	nodeindex_t max_height = store_get_value (distance->_store, last_node);
	nodeindex_t max_width = store_get_value_2 (distance->_store, last_node);

	distance->_store->_mod_value = STORE_MODIF_END;
	distance->_store->_mod_value2 = STORE_MODIF_END;

	pDEBUG("Maximum height = %ld\n", max_height);
	pDEBUG("Maximum width = %ld\n", max_width);

	fifo_destroy (fifo_inverse_visit);
	return max_width;
}

#undef DEBUG
