/* vim: set sw=4 ts=4 si et: */		

#include <assert.h>
#include "degree.h"

#define	DEBUG	1

Degree_t * degree_create(Config_io_t * io, nodeindex_t size, Degree_mode_t mode )
{
	nodeindex_t i;
	
	Degree_t * degree = (Degree_t*) malloc (sizeof (Degree_t));
	degree->_io = io;
	degree->_show_output = false;
	degree->_size = size;
	degree->_mode = mode;

	degree->arcs = 0;
	degree->average = 0;
	degree->maximum = 0;
	degree->density = 0;

	degree->_maxIdx = 0; /* set to the minimum */
	degree->_minIdx = size; /* set to the maximum */

	degree->_data = (nodeindex_t*) malloc (sizeof (nodeindex_t) * size);
	
	for (i = 0; i < size; i++){ degree_set (degree, i, 0); }

	return degree;
}

void degree_destroy(Degree_t * degree){
	free(degree->_data);
	free(degree);
	degree = NULL;
}

void degree_set_output(Degree_t * degree, bool out){
	degree->_show_output = out;
}

nodeindex_t degree_begin(Degree_t * degree){
	return degree->_minIdx;
}

nodeindex_t degree_end(Degree_t * degree){
	return degree->_maxIdx;
}

nodeindex_t degree_size(Degree_t * degree){
	return (1 + degree->_maxIdx - degree->_minIdx);
}

nodeindex_t degree_get(Degree_t * degree, nodeindex_t nodeidx){
	return degree->_data[nodeidx];
}

void degree_set(Degree_t * degree, nodeindex_t nodeidx, nodeindex_t value){
    assert(nodeidx < degree->_size);
	degree->_data[nodeidx] = value;
}

void degree_fill_from_random(Degree_t * degree, nodeindex_t edge_count){

    if (degree->_size > edge_count){
        printf("ATTENTION: il y aura des noeuds de degré zéro!\n");
    } else {
        nodeindex_t cur;
        nodeindex_t remaining = edge_count - degree->_size;

        for (cur = 0; cur < degree->_size; cur++){
			nodeindex_t selected_node;
			// si le degré est nul, alors on crée un lien
			// vers un autre noeud (rempli ou non)
			// (pour qu'aucun des noeuds ne soit seul)
			if (degree_get (degree, cur) == 0){
				degree_set(degree, cur, 1);

				selected_node = rand() % degree->_size;
				nodeindex_t deg = degree_get(degree, selected_node);
				degree_set(degree, selected_node, deg + 1); 
			}
		}

		for (cur = 0; cur < remaining; cur++){
			nodeindex_t src_node;
			nodeindex_t dst_node;
			nodeindex_t deg;

			printf("\r%s Selecting degrees... (%ld of %ld)", 
					progress_indicator(), 
					cur+1, 
					remaining);

			bool is_bad;
			do {
				is_bad = false;
				
				src_node = rand() % degree->_size;
				dst_node = rand() % degree->_size;
				if (src_node == dst_node){ is_bad = true; }
			} while(is_bad);

			deg = degree_get(degree, src_node);
			degree_set(degree, src_node, deg + 1); //  
			
			deg = degree_get(degree, dst_node);
			degree_set(degree, dst_node, deg + 1); //  
		}
		printf("\n");
	}
	degree_display(degree);
}

void degree_display(Degree_t * degree){
	nodeindex_t cur_node;
	for (cur_node = 0; cur_node < degree->_size; cur_node++){
		printf("degree[ %ld ] = %ld\n", cur_node, degree_get (degree, cur_node));
	}
}

void degree_output_distribution(Degree_t * degree){
	nodeindex_t max = degree->maximum + 1;
	nodeindex_t * distrib = (nodeindex_t *) malloc (sizeof(nodeindex_t) * max);

	nodeindex_t cur;
	for (cur = 0; cur < max; cur++){
		distrib[cur] = 0;
	}

	for (cur = 0; cur < degree->_size; cur++){
		nodeindex_t deg = degree_get(degree, cur);
		distrib[deg] = distrib[deg] + 1;
	}
	for (cur = 0; cur < max; cur++){
		fprintf(degree->_io->output, "%ld %ld\n", cur, distrib[cur]);
	}
}

void degree_fill_from_input_graph(Degree_t * degree){

	pDEBUG("Computing degree of each node...\n");

	char * buf = (char*) malloc (sizeof(char) * 100);
	char * buf2;
	int rep;
	nodeindex_t one, two;

	gzseek(degree->_io->input, 0, SEEK_SET);

	for (;;){
		if (gzeof(degree->_io->input)){ break; }

		buf2 = gzgets(degree->_io->input, buf, 100);
		if (buf2 == Z_NULL){
			pDEBUG("READ END\n");
			break;
		} else {


			rep = sscanf(buf,"%ld %ld",&one,&two);

			if (one < degree->_minIdx) { degree->_minIdx = one; }
			else if (one > degree->_maxIdx) { degree->_maxIdx = one; }

			if (two < degree->_minIdx) { degree->_minIdx = two; }
			else if (two > degree->_maxIdx) { degree->_maxIdx = two; }

			nodeindex_t one_deg, two_deg;
			one_deg = degree_get (degree, one) + 1;
			two_deg = degree_get (degree, two) + 1;
			degree_set (degree, one, one_deg);
			degree_set (degree, two, two_deg);

			/* on fait les STATS tout le temps... */
			degree->arcs = degree->arcs + 2;
			if (two_deg > degree->maximum){ 
				degree->maximum = two_deg;
			}
			if (one_deg > degree->maximum){ 
				degree->maximum = one_deg;
			}

		} /* end else */

	} /* end for */


	if (degree->_mode == MODE_DEGSTATS){
		/* arcs_reel = compute_arcs/ 2;
		 * arcs_possible = density->size * (density->_size - 1) / 2
		 * compute_density = arcs_reel / arcs_possible
		 */

		degree->density = (degree->arcs / 
				(degree->_size * (degree->_size - 1)));
		degree->average = (float)degree->arcs / (float)degree->_size;
	}

	pDEBUG("done\n");

	if (degree->_show_output){
		// on affiche les résultats
		if (degree->_mode == MODE_DEGSTATS){
			// calculs statistiques
			fprintf (degree->_io->output, 
					"degree_run -- Degree average: %f\n", 
					degree->average);
			fprintf (degree->_io->output, 
					"degree_run -- Degree maximum: %ld\n", 
					degree->maximum);
			fprintf (degree->_io->output, 
					"degree_run -- Density : %f\n", 
					degree->density);
		} else {
			// degrés bruts
			nodeindex_t idx; 

			for (idx = 0; idx < degree->_size; idx++){
				fprintf (degree->_io->output, 
						"%ld %ld\n", 
						idx, 
						degree_get (degree, idx));
			}
		}
	}

	free (buf);
	return;
}

#undef DEBUG
