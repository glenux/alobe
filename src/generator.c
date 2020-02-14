/* vim: set sw=4 ts=4 si et: */		

#include <assert.h>
#include "generator.h"
#include "progressindicator.h"
#include "list.h"

#define DEBUG			0

nodeindex_t * nodeindex_create(nodeindex_t value){
	nodeindex_t * var = ((nodeindex_t *) malloc (sizeof(nodeindex_t)));
	*var = value;
	return var;
}

void nodeindex_destroy(void * daInt){
	nodeindex_t * truc = daInt;
	free(truc);
}

Generator_t * generator_create (Config_io_t * io, nodeindex_t n_count, nodeindex_t e_count){
	Generator_t * generator = (Generator_t *) malloc (sizeof(Generator_t));

	generator->_io = io;
	generator->_node_count = n_count;
	generator->_edge_count = e_count;

    // on crée le tableau et on initialise tout le monde à zéro.
    generator->_data = (List_t **) malloc (sizeof(List_t*) * n_count);

	nodeindex_t cur;
	for (cur = 0; cur < generator->_node_count; cur++){
		generator->_data[cur] = list_create();
	}
	
	return generator;
}

void generator_destroy (Generator_t * generator){
    free(generator->_data);
    free(generator);
    //FIXME: détruire les données
    //FIXME: détruire l'objet
}

bool generator_is_already_adjacent (Generator_t * generator, nodeindex_t src, nodeindex_t dst){
	nodeindex_t min, max;
	if (src < dst){
		min = src;
		max = dst;
	} else {
		min = dst;
		max = src;
	}

	bool found = false;
	List_cell_t * iterator = list_iterator(generator->_data[min]);
	while(!list_iterator_is_end(iterator)){

		nodeindex_t * val = list_iterator_value(iterator);
		if (*val == max) { found = true; break; }

		iterator = list_iterator_next(iterator);
	}
	
	return found;
}

bool generator_degree_is_null (Generator_t * generator, nodeindex_t idx){
	return list_is_empty(generator->_data[idx]);
}

void generator_add_edge(Generator_t * generator, nodeindex_t src, nodeindex_t dest){
	if (src < dest){
		list_push_back(generator->_data[src], nodeindex_create(dest));
	} else {
		list_push_back(generator->_data[dest], nodeindex_create(src));
	}
}

void generator_run(Generator_t * generator){

	nodeindex_t cur;
	if (generator->_node_count > generator->_edge_count){
		fprintf(stderr, "ATTENTION: il y aura des noeuds de degré zéro!\n");
	} else {
		nodeindex_t remaining = generator->_edge_count - generator->_node_count;

		for (cur = 0; cur < generator->_node_count; cur++){

			fprintf(stderr,"\r%s Selecting degrees... (%ld of %ld)", 
					progress_indicator(), 
					cur + 1, 
					generator->_edge_count);
			nodeindex_t selected_node;
			// si le degré est nul, alors on crée un lien
			// vers un autre noeud (rempli ou non)
			// (pour qu'aucun des noeuds ne soit seul)
			if (generator_degree_is_null (generator, cur)){
				bool is_bad;
				do {
					is_bad = false;
					selected_node = rand() % generator->_node_count;
					if (selected_node == cur){ is_bad = true; }
				} while(is_bad);
				generator_add_edge (generator, cur, selected_node);
			}
		}

		for (cur = 0; cur < remaining; cur++){
			nodeindex_t src_node;
			nodeindex_t dst_node;
			nodeindex_t deg;

			fprintf(stderr,"\r%s Selecting degreex... (%d of %d)", 
					progress_indicator(), 
					cur + 1 + generator->_node_count, 
					generator->_edge_count);

			bool is_bad;
			do {
				is_bad = false;

				src_node = rand() % generator->_node_count;
				dst_node = rand() % generator->_node_count;
				if (src_node == dst_node){ is_bad = true; }
				if (generator_is_already_adjacent (generator, src_node, dst_node)){ is_bad = true; }
			} while(is_bad);

			generator_add_edge (generator, src_node, dst_node);
		}
		fprintf(stderr, "\n");


		//FIXME: créer un deuxieme tableau de taille n, avec tab[i] = i;
		//utiliser ce tableau pour remplir les listes, afin d'éparpiller un peu moins les trucs en mémoire

	}

	//FIXME output sur stdout...
	for (cur = 0; cur < generator->_node_count; cur++){
		while(!generator_degree_is_null (generator, cur)){
			nodeindex_t * dest = list_pop_front (generator->_data[cur]);
			fprintf(generator->_io->output, "%ld %ld\n", cur, *dest);
			nodeindex_destroy(dest);
		}
	}

}
#undef DEBUG
