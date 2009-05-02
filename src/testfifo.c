/* vim: set sw=4 ts=4 si et: */		

#include "fifo.h"

int main(){
	Fifo_t * fifo = fifo_create(20);
	Fifo_t * fifo2;

	nodeindex_t i;
	nodeindex_t before, after;
	printf("\n************************************************************************\n");
	printf("                                PHASE 1 (fill with 0..19)\n");
	printf("************************************************************************\n");

	for (i=0; i<20; i++){
		before = fifo_get_size(fifo);
		fifo_push (fifo, i);
		after = fifo_get_size(fifo);
		printf("Pushed %ld\n", i);
		printf("Fifo size %ld -> %ld\n", before, after);
	}

	fifo_display (fifo);

	printf("\n************************************************************************\n");
	printf("                                PHASE 2 (move 5 to back)\n");
	printf("************************************************************************\n");

	for (i=0; i<5; i++){
		nodeindex_t tmp;
		tmp = fifo_pop(fifo);
		fifo_push(fifo, tmp);
	}

	fifo_display (fifo);

	printf("\n************************************************************************\n");
	printf("                                PHASE 3 (reverse)\n");
	printf("************************************************************************\n");

	fifo_reverse (fifo);
	fifo_display (fifo);
	

	printf("\n************************************************************************\n");
	printf("                                PHASE 4 (copy)\n");
	printf("************************************************************************\n");

	fifo2 = fifo_copy (fifo);
	fifo_destroy (fifo);
	fifo = fifo2;
	fifo2 = NULL;

	fifo_display (fifo);

	printf("\n************************************************************************\n");
	printf("                                PHASE 5 (pop everything)\n");
	printf("************************************************************************\n");
	while (!fifo_is_empty(fifo)){
		before = fifo_get_size(fifo);
		nodeindex_t val = fifo_pop (fifo);
		after = fifo_get_size(fifo);
		printf("Popped %ld\n", val);
		printf("Fifo size %ld -> %ld\n", before, after);
	}

	fifo_display (fifo);

	fifo_destroy (fifo);

	return EXIT_SUCCESS;
}
