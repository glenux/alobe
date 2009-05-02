/* vim: set sw=4 ts=4 si et: */		

#include <assert.h>
#include "degenerator.h"
#include "progressindicator.h"
#include "store.h"
#include "fifo.h"

#define DEBUG			1

Degenerator_t * degenerator_create (Config_io_t * io, nodeindex_t n_count, int iterations){
	Degenerator_t * degenerator = (Degenerator_t *) malloc (sizeof(Degenerator_t));

	degenerator->_io = io;

	degenerator->_store = store_create(io, n_count);
	store_set_output(degenerator->_store, false);
	store_fill_from_input_graph (degenerator->_store);

	degenerator->_size = n_count;
	degenerator->_iterations = iterations;
	degenerator->_iterations_offset = 10;
	

	degenerator_set_select_mode (degenerator, DEGENERATOR_SELECT_RANDOM);

	printf("create ok\n");
	return degenerator;
}

void degenerator_set_select_mode (Degenerator_t * degenerator, Degenerator_select_mode_t mode){
	printf("select %d\n", mode);
	degenerator->_select_mode = mode;
}

void degenerator_destroy (Degenerator_t * degenerator){
	store_destroy(degenerator->_store);
    free(degenerator);
}

nodeindex_t degenerator_select_max_deg (Degenerator_t * degenerator){
	nodeindex_t deg, max_deg;
	nodeindex_t cur_node, max_node;
	
	max_deg = 0;
	max_node = 0;
	for (cur_node = 0; cur_node < degenerator->_size; cur_node++){
		deg = store_get_degree(degenerator->_store, cur_node);
		if (deg > max_deg){ 
			max_deg = deg; 
			max_node = cur_node;
		}
	}
	return max_node;
}

nodeindex_t degenerator_select_random (Degenerator_t * degenerator){
	// Choisir un noeud de degré > 0
	bool is_bad;
	nodeindex_t selected_node;
	do {
		is_bad = false;
		selected_node = random() % degenerator->_size;

		// ne pas virer un noeud déja supprimé...
		if (store_get_degree(degenerator->_store, selected_node) <= 0) { is_bad = true; }
	} while (is_bad);
	return selected_node;
}

void degenerator_compute (Degenerator_t * degenerator){
	int cur_iter;
	

	pDEBUG("Iterating %d times\n", degenerator->_iterations);
	for (cur_iter = 0; cur_iter < degenerator->_iterations; cur_iter++){
		if ((cur_iter % degenerator->_iterations_offset) == 0){
			// faire des calculs (nombre de composantes connexes)
			store_reset (degenerator->_store, STORE_RESET_ALL ^ STORE_RESET_DEGREE);

			Fifo_t * fifo_idx = fifo_create(degenerator->_size);
			store_connexity (degenerator->_store, fifo_idx);

			fprintf(degenerator->_io->output, "%d %ld\n", cur_iter, fifo_get_size (fifo_idx));
			fifo_destroy(fifo_idx);
			// afficher la courbe...
		}
		// Choisir un noeud selon une certaine méthode...
		nodeindex_t selected_node = (degenerator_select_mode_table[degenerator->_select_mode])(degenerator);
		printf("Node %ld\n", selected_node);

		store_del_node(degenerator->_store, selected_node);
	}
}

#undef DEBUG
