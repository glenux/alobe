/* vim: set sw=4 ts=4 si et: */		

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "units.h"
#include "store.h"

#include "degree.h"

#include "progressindicator.h"

#define DEBUG	0

/**
 * Constructeur de l'objet
 * @param io Configuration des flux d'entrée/sortie
 * @param size Nombre de noeuds
 * @return Pointeur sur la structure de l'objet
 */

Store_t * store_create(Config_io_t * io, nodeindex_t size)
{
	Store_t * store = (Store_t *) malloc (sizeof(Store_t));

	store->_io = io;
	store->_size = size;
	store->_show_output = true;

	store->_mod_degree = STORE_MODIF_UNDEF;
	store->_mod_value = STORE_MODIF_UNDEF;
	store->_mod_value2 = STORE_MODIF_UNDEF;
	store->_mod_ref = STORE_MODIF_UNDEF;

	return store;
}


/**
 * Destructeur de l'objet passé en paramètre.
 * @param store Pointeur sur la structure de l'objet
 */

void store_destroy(Store_t * store){
	free(store->_thetable);
	free(store->_refs);
	free(store);
}

/**
 * Fixe le mode d'affichage (silencieux ou pas)
 */

void store_set_output(Store_t * store, bool output){
	store->_show_output = output;
}

/**
 * Ré-initilaise les données EXTRA du Store_t
 *
 * @param store Pointeur sur l'objet Store_t manipulé
 */

void store_reset(Store_t * store, store_reset_t mode){
	nodeindex_t i;
	for (i = 0; i < store->_size; i++){
		if (mode & STORE_RESET_DEGREE) {
			store->_refs[i][STORE_EXTRA_DEGREE] = 0; // default degree
		}
		if (mode & STORE_RESET_VALUE) {
			store->_refs[i][STORE_EXTRA_VALUE] = NODEINDEX_UNDEF; // current seach
		}
		if (mode & STORE_RESET_VALUE2) {
			store->_refs[i][STORE_EXTRA_VALUE2] = NODEINDEX_UNDEF; // current seach
		}
		if (mode & STORE_RESET_REF) {
			store->_refs[i][STORE_EXTRA_REF] = NODEINDEX_UNDEF; // node reference in DFS
		}
	}
	if (mode & STORE_RESET_DEGREE){
		pDEBUG("reset STORE_RESET_DEGREE\n");
		store->_mod_degree = STORE_MODIF_RESET;
	}
	if (mode & STORE_RESET_VALUE){
		pDEBUG("reset STORE_RESET_VALUE\n");
		store->_mod_value = STORE_MODIF_RESET;
	}
	if (mode & STORE_RESET_VALUE2){
		pDEBUG("reset STORE_RESET_VALUE2\n");
		store->_mod_value2 = STORE_MODIF_RESET;
	}
	if (mode & STORE_RESET_REF){
		pDEBUG("reset STORE_RESET_REF\n");
		store->_mod_ref = STORE_MODIF_RESET;
	}
	
	return;
}

/**
 * Génère le modèle du Store_t
 *
 * Alloue la mémoire nécessaire en fonction des degrés des noeuds
 * et initialise les cases aux bonnes valeurs par défaut.
 * 
 * @param store Pointeur sur l'objet Store_t manipulé
 * @param degree Pointeur sur l'objet Degree_t fournissant les degrés.
 */

void store_generate_model(Store_t * store, Degree_t * degree){
	store->_size = degree->_size; 

	store->_refs = (nodeindex_t **) malloc (sizeof(nodeindex_t *) * store->_size);

	/* should be initialized to 1 instead of 0 else causes erroneous memory
	 * read/write, but why ?
	 * FIXME: find the bug... */
	nodeindex_t tablesize = 1;

	nodeindex_t i;

	for (i = 0; i < store->_size; i++){
		/* extra infos + index size (degree) */
		tablesize += degree_get (degree, i) + STORE_EXTRAPLACE; 
	}	

	store->_thetable = (nodeindex_t *) malloc (sizeof(nodeindex_t) * tablesize);


	nodeindex_t ref = 1;
	for (i = 0; i < store->_size; i++){
		store->_refs[i] = (nodeindex_t*) (store->_thetable + ref);
		ref += degree_get (degree, i) + STORE_EXTRAPLACE;	
	}

	store_reset (store, STORE_RESET_ALL);
	return;
}


/**
 * Enregistre une valeur (sémantique arbitraire) dans le noeud choisi
 *
 * @param store 		Pointeur sur l'objet Store_t manipulé
 * @param node_index 	Index du noeud choisi dans le Store_t
 * @param value 		Valeur a enregistrer
 */

void store_set_value(Store_t * store, nodeindex_t node_index, nodeindex_t value){
	assert(store->_mod_value == STORE_MODIF_BEGIN);
	pDEBUG("setting value-one %ld at %ld\n", value, node_index);
	store->_refs[node_index][STORE_EXTRA_VALUE] = value;
}

void store_set_value_2(Store_t * store, nodeindex_t node_index, nodeindex_t value){
	assert(store->_mod_value2 == STORE_MODIF_BEGIN);
	pDEBUG("setting value-two %ld at %ld\n", value, node_index);
	store->_refs[node_index][STORE_EXTRA_VALUE2] = value;
}

/**
 * Lit la valeur (sémantique arbitraire) dans le noeud choisi
 *
 * @param store 		Pointeur sur l'objet Store_t manipulé
 * @param node_index	Index du noeud choisi dans le Store_t
 */

nodeindex_t store_get_value(Store_t * store, nodeindex_t from){
	return store->_refs[from][STORE_EXTRA_VALUE];
}


nodeindex_t store_get_value_2(Store_t * store, nodeindex_t from){
	return store->_refs[from][STORE_EXTRA_VALUE2];
}

/** 
 * Enregistrer dans un noeud la référence à un autre noeud
 *
 * @param store 		Pointeur sur l'objet Store_t manipulé
 * @param node_index	Index du noeud choisi dans le Store_t
 * @param index_value	Index du noeud référencé dans le Store_t
 */

void store_set_ref(Store_t * store, nodeindex_t node_index, nodeindex_t index_value){
	assert(store->_mod_ref == STORE_MODIF_BEGIN);
	pDEBUG("index %ld / value %ld\n", node_index, index_value);
	store->_refs[node_index][STORE_EXTRA_REF] = index_value;
}


/**
 * Lit la référence contenue dans le noeud choisi
 *
 * @param store 		Pointeur sur l'objet Store_t manipulé
 * @param node_index	Index du noeud choisi dans le Store_t
 */

nodeindex_t store_get_ref(Store_t * store, nodeindex_t from){
	return store->_refs[from][STORE_EXTRA_REF];
}


/**
 * Ajoute un arc vers un autre noeud au noeud choisi
 *
 * @param store 		Pointeur sur l'objet Store_t manipulé
 * @param from			Index du noeud choisi dans le Store_t
 * @param to			Index du noeud destination (de l'arc) dans le Store_t
 */

#undef DEBUG
#define DEBUG 0
void store_add_adjacent(Store_t * store, nodeindex_t from, nodeindex_t to){
	pDEBUG("from = %ld  to =%ld\n", from, to);
	assert( from < store->_size);
	assert( from >= 0);
	assert( to < store->_size);
	assert( to >= 0);
	assert(store->_mod_degree == STORE_MODIF_BEGIN);
	
	nodeindex_t from_degree = store_get_degree (store, from);
	nodeindex_t redir = STORE_EXTRAPLACE + from_degree;
	store->_refs[from][redir] = to;
	store->_refs[from][STORE_EXTRA_DEGREE] = from_degree + 1; /* increment degree */
	
	return;
}


/**
 * Trouve l'index du noeud cherché dans la liste d'ajacence du noeud choisi
 */

#undef DEBUG
#define DEBUG 0
nodeindex_t store_find_adjacent (Store_t * store, nodeindex_t from, nodeindex_t to){
	nodeindex_t cur;

	nodeindex_t result = NODEINDEX_UNDEF;

	nodeindex_t from_degree = store_get_degree (store, from);
	nodeindex_t redir;

	pDEBUG("Degree [ %ld ] = %ld\n", from, from_degree);
	for (cur = 0; cur < from_degree; cur++){
		redir = STORE_EXTRAPLACE + cur;
		if (store->_refs[from][redir] == to){
			result = cur;
			break;
		} else {
		}
	}
	return result;
}

void store_display_adjacent (Store_t * store, nodeindex_t from){
	nodeindex_t cur;

	nodeindex_t from_degree = store_get_degree (store, from);
	printf("%ld -> [ ", from);
	for (cur = 0; cur < from_degree; cur++){
		nodeindex_t redir = STORE_EXTRAPLACE + cur;

		printf("%ld ",store->_refs[from][redir]);
	}
	printf("]\n");
}
/**
 * Retire l'arc vers un autre noeud, à partir du noeud choisi
 *
 */

void store_del_adjacent(Store_t * store, nodeindex_t from, nodeindex_t to){
	nodeindex_t idx = store_find_adjacent (store, from, to);
	
	if (idx == NODEINDEX_UNDEF){
		printf("Adjacent %ld non trouvé dans la liste de %ld!\n", to, from);
		store_display_adjacent(store, from);
		exit(-1);
	}

	// on décale tous les suivants de 1, et on décrémente le degré
	nodeindex_t from_degree = store_get_degree (store, from);
	nodeindex_t cur;
	for (cur = idx; cur < (from_degree - 1); cur++){
		nodeindex_t redir = STORE_EXTRAPLACE + cur;
		store->_refs[from][redir] = store->_refs[from][redir + 1];
		store->_refs[from][redir + 1] = NODEINDEX_UNDEF;
	}
	store->_refs[from][STORE_EXTRA_DEGREE] = from_degree - 1; /* increment degree */
}

/** 
 * Supprime le noeud choisi
 */

void store_del_node (Store_t * store, nodeindex_t from){
	// on visite tous les adjacents, 
	// et on leur dit de supprimer le noeud courant de leur liste...
	
	pDEBUG("Deleting node %ld\n", from);
	nodeindex_t from_degree = store_get_degree (store, from);

	nodeindex_t cur;
	store_display_adjacent(store, from);
	for (cur = 0; cur < from_degree; cur++){
		nodeindex_t redir = STORE_EXTRAPLACE + cur;
		nodeindex_t adj = store->_refs[from][redir];
		store->_refs[from][redir] = NODEINDEX_UNDEF;

		pDEBUG("  - Removing adjacent %ld from %ld\n", from, adj);
		store_display_adjacent(store, adj);
		// supprimer le noeud courrant de la liste d'adjacence de ses voisins
		store_del_adjacent(store, adj, from);
		store_display_adjacent(store, adj);
		
	}
	// on anihile le noeud...
	store->_refs[from][STORE_EXTRA_DEGREE] = 0;
}

/**
 */
nodeindex_t store_get_adjacent(Store_t * store, nodeindex_t from, nodeindex_t index){
	nodeindex_t result;

	assert(index < store->_refs[from][STORE_EXTRA_DEGREE]);
	result = store->_refs[from][STORE_EXTRAPLACE + index];

	return result;
}


/**
 * Lit le degré du noeud choisi
 *
 * @param store 		Pointeur sur l'objet Store_t manipulé
 * @param node_index	Index du noeud choisi dans le Store_t
 */

nodeindex_t store_get_degree (Store_t * store, nodeindex_t from){
	nodeindex_t result;

	result = store->_refs[from][STORE_EXTRA_DEGREE];
	return result;
}

/**************************************************************************
 *
 */

double store_compute_avg_degree(Store_t * store, int iterations){
	double average_sum;
	double average_avg;

	nodeindex_t cur;
	nodeindex_t rand_node = (nodeindex_t)(random() % store->_size);
	for(cur = 0; cur < iterations; cur++){
		nodeindex_t rand_node = (nodeindex_t)(random()) % store->_size;
		fprintf(stderr, "Selecting node %ld\n", rand_node);
		
		average_sum += store_get_degree(store, rand_node);
		average_avg = average_sum / (cur + 1);
		fprintf(store->_io->output, "%ld %lf\n", cur, average_avg);
	}
}

void store_init_from_degrees (Store_t * store, Degree_t * degree){
	store->_mod_value = STORE_MODIF_BEGIN;
	nodeindex_t cur_node;
	for (cur_node = 0; cur_node < store->_size; cur_node++){
        nodeindex_t cur_deg = degree_get (degree, cur_node);
		printf("\r%s Degree = %ld for node %ld... ", progress_indicator(), cur_deg, cur_node);
        store_set_value(store, cur_node, cur_deg);
	}
	store->_mod_value = STORE_MODIF_END;
}

void store_fill_from_random_graph (Store_t * store, nodeindex_t edge_count){
	assert(store->_mod_degree == STORE_MODIF_RESET);

	store->_mod_degree = STORE_MODIF_BEGIN;
	store->_mod_value = STORE_MODIF_BEGIN;
	
    nodeindex_t cur_node;
    nodeindex_t cur_deg;
	
	printf("Filling 'remaining degrees' %ld...\n", store->_size);

	/*
	
    // on remplit les "remaining degree" dans les VALUES du store
	for (cur_node = 0; cur_node < store->_size; cur_node++){
        nodeindex_t cur_deg = store_get_degree(store, cur_node);
        store_set_value(store, cur_node, cur_deg);
	}
	*/

	nodeindex_t nb_edges = 0;

	printf("Filling random nodes %ld...\n", store->_size);
    for (cur_node = 0; cur_node < store->_size; cur_node++){
        // on lit le degré du noeud
        nodeindex_t src_deg = store_get_value(store, cur_node);
        
        while(src_deg > 0){
			printf("r_degree[ %ld ] => %ld = ?\n", cur_node, src_deg);
            // choisir un autre noeud au pif
            bool choix_is_bad;
            nodeindex_t dest_node;
            do {
                dest_node = rand() % store->_size;
                choix_is_bad = false;
                
                // on vérifie que le noeud dest est différent du noeud source
                if (dest_node == cur_node){ choix_is_bad = true; }
               
                // on vérifie qu'il reste des places disponibles dans le noeud
                // visé...
                nodeindex_t dest_remaining_deg = store_get_value(store, dest_node); 
                if (dest_remaining_deg <= 0){ choix_is_bad = true; }

                // vérifier que le noeud n'est pas _deja_ dans les adjacents...
                nodeindex_t dest_deg = store_get_degree(store, dest_node);
				nodeindex_t inf_deg;

                for (inf_deg = 0; inf_deg < dest_deg; inf_deg++){
					printf("Checking adjacent %ld of node %ld\n", inf_deg, dest_node);
                    // lire les numéros des noeuds afin de vérifier
                    // si les adjacents ne contiennent pas le noeud source
                    if (store_get_adjacent(store, dest_node, inf_deg) == cur_node){
                        choix_is_bad = true;
                        break;
                    }
                }
            } while(choix_is_bad);

            // on ajoute un lien vers le noeud à partir de ce noeud, 
            // puis l'inverse...
            store_add_adjacent(store, cur_node, dest_node);
			store_add_adjacent(store, dest_node, cur_node);
			nb_edges += 1;

			printf("Adding edge from %ld to %ld\n", cur_node, dest_node);

			nodeindex_t dest_deg; 
            // décrémenter le nombre de dispo sur la destination
			dest_deg = store_get_value (store, dest_node);
			assert(dest_deg > 0);
			store_set_value (store, dest_node, dest_deg - 1);
			printf("r_degree[ %ld ] => %ld = ?\n", dest_node, dest_deg - 1 );
 

			// décrementer le nombre de dispo sur la source
			src_deg = src_deg - 1;
			store_set_value (store, cur_node, src_deg);
        }

    }
}


/**
 * Initialise la structure du store à partir d'une premiere
 * lecture des degrés des noeuds dans le fichier
 *
 * ATTENTION: modifie STORE_EXTRA_DEGREE (ok)
 * 
 * @param store 		Pointeur sur l'objet Store_t manipulé
 */

void store_init_from_input_graph(Store_t * store){
	/* on interdit de re-remplir un tableau qui aurait déja été
	 * rempli. L'utilisateur n'avait qu'a faire attention a ses 
	 * données... */
	assert(store->_mod_degree == STORE_MODIF_UNDEF);

	Degree_t * degree = degree_create (store->_io, 
			store->_size, 
			MODE_DEGREE);
	degree_set_output (degree, false);
	degree_fill_from_input_graph (degree);

	// on allocate la mémoire du tableau
	store_generate_model(store, degree);
	degree_destroy(degree);
}

/**
 * Remplit le store à partir des informations d'adjacence contenues
 * dans le fichier.
 *
 * ATTENTION: modifie STORE_EXTRA_DEGREE (ok)
 * FIXME: penser a découper la premiere partie dans une fonction à part
 *
 * @param store 		Pointeur sur l'objet Store_t manipulé
 */

void store_fill_from_input_graph(Store_t * store){
    store_init_from_input_graph(store);

	/* on interdit de remplit autre chose qu'un tableau propre
	 * au niveau des degrés */
	assert(store->_mod_degree == STORE_MODIF_RESET);
	
	store->_mod_degree = STORE_MODIF_BEGIN;

	//int len;
	char * buf = (char *) malloc (sizeof(char) * 100);
	char * buf2;
	nodeindex_t counter = 0;
	nodeindex_t one, two;
	int re;

	gzrewind(store->_io->input); 
	gzclearerr(store->_io->input);

	fprintf(stderr,"\r    Filling the big table from file data...");

	for (;;){
		//	fprintf(stderr,"\r%s", progress_indicator());
		if (gzeof(store->_io->input)){ break; }

		buf2 = gzgets(store->_io->input, buf, 100);
		if (buf2 == Z_NULL){
			pDEBUG("READ OFF\n");
			break;
		} else {
			counter++;
			re = sscanf(buf, "%ld %ld", &one, &two);
			//printf("Add from %d to %d\n", one, two);
			store_add_adjacent(store, one, two);
			store_add_adjacent(store, two, one);

			}

	}
	fprintf(stderr,"\n");

	store->_mod_degree = STORE_MODIF_END;

	free (buf);
	return;
}


/**
 * Indique si la visite du noeud est terminée 
 *
 * ATTENTION: utilise STORE_EXTRA_REF (ok)
 *
 * @param store 		Pointeur sur l'objet Store_t manipulé
 * @return bool			TRUE si le noeud a été visité
 */

bool store_is_visit_done(Store_t * store, nodeindex_t index){
	assert((store->_mod_ref == STORE_MODIF_RESET)
			|| (store->_mod_ref == STORE_MODIF_END));

	return (store->_refs[index][STORE_EXTRA_REF] != NODEINDEX_UNDEF);
}


/**
 * Indique si la visite du noeud n'a pas été faite
 * 
 * ATTENTION: utilise STORE_EXTRA_REF (ok)
 *
 * @param store 		Pointeur sur l'objet Store_t manipulé
 * @return bool			TRUE si le noeud a été visité
*/

bool store_is_visit_undone(Store_t * store, nodeindex_t index){
	assert((store->_mod_ref == STORE_MODIF_RESET)
			|| (store->_mod_ref == STORE_MODIF_END));

	return (store->_refs[index][STORE_EXTRA_REF] == NODEINDEX_UNDEF);
}


/**
 * Visite récursivement le noeud pour un parcours en profondeur
 * 
 * ATTENTION: utilise STORE_EXTRA_DEGREE (ok)
 * 
 * ATTENTION: utilise STORE_EXTRA_REF (ok)
 * ATTENTION: modifie STORE_EXTRA_REF (ok)
 *
 * @param store 		Pointeur sur l'objet Store_t manipulé
 * @param cur_idx		Index du noeud par lequel commencer
 * @param reference		Index du noeud père
 */

void store_visit_node(Store_t * store, 
		nodeindex_t cur_idx, 
		nodeindex_t reference, 
		Fifo_t * fifo_cc )
{
	/* on oblige l'initialisation des degrés */
	assert(store->_mod_degree == STORE_MODIF_END);
	
	/* on oblige l'utilisateur a indiquer qu'il est entrain de faire
	 * des modifications sur STORE_EXTRA_REF */
	assert(store->_mod_ref == STORE_MODIF_BEGIN);

	pDEBUG("store_visit_node -- %ld \n",cur_idx); 
	if (fifo_cc){ fifo_push (fifo_cc, cur_idx); }
	store->_refs[cur_idx][STORE_EXTRA_REF] = reference;
	/* pour chacun des noeuds visitables, on visite... */
	nodeindex_t adj_pos = 0;

	for (adj_pos = 0; adj_pos < store_get_degree (store, cur_idx); adj_pos++){
		nodeindex_t adj_idx = store->_refs[cur_idx][STORE_EXTRAPLACE + adj_pos];

		store_modif_t mod_ref_status = store->_mod_ref;
		store->_mod_ref = STORE_MODIF_END;
		bool is_undone = store_is_visit_undone(store, adj_idx);
		pDEBUG("visit of %ld %s\n", adj_idx, (is_undone?"undone":"done"));
		store->_mod_ref = mod_ref_status;

		if (is_undone){
			/* alors on visite... */
			store_visit_node(store, adj_idx, cur_idx, fifo_cc);
		} 
	}

	return; 
}


/**
 * Trouve l'index du premier noeud non visité (en partant du noeud indiqué)
 *
 * ATTENTION: utilise STORE_EXTRA_REF (ok)
 *
 * @param store 		Pointeur sur l'objet Store_t manipulé
 * @param index			Indique le noeud de départ dans la recherche		
 */

nodeindex_t store_find_undone(Store_t * store, nodeindex_t index){
	assert((store->_mod_ref == STORE_MODIF_RESET)
			|| (store->_mod_ref == STORE_MODIF_END));
	nodeindex_t first_idx;
	for (first_idx = index; first_idx < store->_size; first_idx++){
		if (store_is_visit_undone(store, first_idx)){ break; }
	}
	if (first_idx == store->_size){ first_idx = NODEINDEX_UNDEF; }
	return first_idx;
}


/**
 * Trouve les composantes connexes dans le graphe représenté par le Store_t
 *
 * ATTENTION: modifie STORE_EXTRA_REF (ok)
 *
 * @param store 		Pointeur sur l'objet Store_t manipulé
 * @param fifo_indexes  File contenant les noeuds de référence des composantes
 * 						connexes
 */

void store_connexity(Store_t * store, Fifo_t * fifo_indexes){
	nodeindex_t first_idx = 0;
	nodeindex_t counter = 0;

	assert(store->_mod_ref == STORE_MODIF_RESET);


	while(first_idx != NODEINDEX_UNDEF){
		first_idx = store_find_undone (store, first_idx);
		
		store->_mod_ref = STORE_MODIF_BEGIN;

		if (first_idx != NODEINDEX_UNDEF){
			if (fifo_indexes) { fifo_push(fifo_indexes, first_idx); }
			counter++;
			if (store->_show_output){
				pDEBUG(" ");
				fprintf (store->_io->output, "Found connex component at %ld\n", first_idx);
			}
			if (store->_io->verbose){ printf ("store_connexity -- = { \n"); }

			// visit with no ancestors
			store_visit_node (store, first_idx, NODEINDEX_ROOT, NULL); 
			
			if (store->_io->verbose){ printf ("store_connexity -- }\n"); }
		}
		store->_mod_ref = STORE_MODIF_END;
	}


	if (store->_show_output){
		pDEBUG(" ");
		fprintf (store->_io->output, "Found %ld connex components\n", counter);
	}

	return;
}


/**
 * Remplit la file passée en parametre avec les noeuds de la composante
 * connexe contenant le noeud de référence.
 *
 * ATTENTION: utilise STORE_EXTRA_DEGREE (ok)
 *
 * ATTENTION: modifie STORE_EXTRA_REF (ok)
 *
 * @param store 		Pointeur sur l'objet Store_t manipulé
 * @param from			Index du noeud de référence
 * @param fifo_cc		Pointeur sur la file a remplir
 */

void store_fill_cc_from_node(Store_t * store, nodeindex_t from, Fifo_t * fifo_cc ){
	/* on oblige a avoir défini les degrés */
	assert(store->_mod_degree == STORE_MODIF_END);

	// on ré-initialise le store
	store_reset (store, STORE_RESET_REF);

	store->_mod_ref = STORE_MODIF_BEGIN;

	// on parcourt en profondeur le store a partir du noeud en question		
	/* on cherche les noeuds visités et on les ajoute a la liste */
	store_visit_node (store, from, from, fifo_cc);
	
	store->_mod_ref = STORE_MODIF_END;

	return;
}


/**
 * Trouve la composante connexe a laquelle appartient le noeud donné.
 *
 * ATTENTION: utilise STORE_EXTRA_REF (ok)
 *
 * @param store 		Pointeur sur l'objet Store_t manipulé
 * @param node			Noeud dont on cherche la composante connexe
 * @return L'index du noeud de référence de la composante connexe
 */

nodeindex_t store_find_cc_of_node(Store_t * store, nodeindex_t node){
	/* on oblige a avoir défini les références */
	assert(store->_mod_ref == STORE_MODIF_END);

	// on remonte de référence en référence...
	pDEBUG("find cc of node %ld\n", node);
	while(node != store_get_ref(store, node)){
		node = store_get_ref(store, node);	
	}
	pDEBUG("cc is %ld\n", node);
	return node;
}

#undef DEBUG
