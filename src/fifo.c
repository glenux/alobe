/* vim: set sw=4 ts=4 si et: */		

#include <assert.h>
#include <string.h>
#include "fifo.h"

#define	DEBUG 	0

/**
 * Constructeur de la file (d'index de noeuds) Fifo_t
 *
 * @param size 	Longueur maximum de la Fifo_t
 * @return Un pointeur sur la Fifo_t créée
 */

Fifo_t * fifo_create(nodeindex_t size){
	Fifo_t * fifo = (Fifo_t *) malloc (sizeof(Fifo_t));	


	fifo->_fill = 0;
	
	pDEBUG("Fifo size : %ld\n", size);
	fifo->_size = size;
	fifo->_head = -1;
	fifo->_tail = -1;

	fifo->_data = (nodeindex_t *) malloc (sizeof(nodeindex_t) * size);

	return fifo;
}

Fifo_t * fifo_copy(Fifo_t * fifo){
	pDEBUG("Fifo size : %ld\n", fifo->_size);
	Fifo_t * result = fifo_create(fifo->_size);
	result->_fill = fifo->_fill;
	result->_head = fifo->_head;
	result->_tail = fifo->_tail;
	memcpy(result->_data, fifo->_data, (sizeof(nodeindex_t) * fifo->_size));
	
	return result;
}

void fifo_reverse(Fifo_t * fifo){
	pDEBUG("Fifo size : %ld\n", fifo->_size);
		
	// pour chaque element du fifo on inverse sa position
	nodeindex_t tail = fifo->_tail;
	nodeindex_t head = fifo->_head;
	nodeindex_t fillcounter = 0;
	if (fifo->_fill > 0){
		while( (2 * fillcounter) < fifo->_fill ){
			pDEBUG("head %ld tail %ld swap %ld and %ld\n", 
					head, tail, fifo->_data[head],
					fifo->_data[tail]);
			nodeindex_t tmp = fifo->_data[head];
			fifo->_data[head] = fifo->_data[tail];
			fifo->_data[tail] = tmp;
			tail += -1;
			head += 1;
			if (tail < 0) { tail = fifo->_size - 1; }
			if (head >= fifo->_size) { head = 0; }
			fillcounter += 1;
		}
	}
}


/**
 * Destructeur de la file Fifo_t
 * 
 * @param	Un pointeur sur la Fifo_t
 */

void fifo_destroy(Fifo_t * fifo){
	free(fifo->_data);
	free(fifo);	
}


/**
 * Indique si la file Fifo_t est vide
 *
 * @param	Un pointeur sur la Fifo_t
 * @return 	TRUE si la Fifo_t est vide, FALSE sinon
 */

bool fifo_is_empty(Fifo_t * fifo){
	bool result;
	if ((fifo->_head == -1) && (fifo->_tail == -1)){
		result = true;
	} else {
		result = false;
	}
	return result;
}


/**
 * Ajoute un élément en fin de liste Fifo_t
 *
 * @param	Un pointeur sur la Fifo_t
 * @param	Un index de noeud a enregistrer dans la Fifo_t
 */

void fifo_push(Fifo_t * fifo, nodeindex_t node){
	pDEBUG("value %ld\n", node);
	pnDEBUG("tail %ld ->", fifo->_tail);

	if (fifo->_tail == -1){
		pnDEBUG("(undef) ");
		assert(fifo->_head == -1);

		fifo->_tail = 0;
		fifo->_head = 0;
	} else {
		fifo->_tail += 1;
		if (fifo->_tail >= fifo->_size){
			fifo->_tail = 0;
			pnDEBUG("(loop) ");
		}
	}
	fifo->_data[fifo->_tail] = node;
	fifo->_fill += 1;
	pnDEBUG("%ld\n", fifo->_tail);
	assert(fifo->_fill <= fifo->_size);
}

/**
 * Lit sans retirer le premier élément de la Fifo_t
 *
 * @param	Un pointeur sur la Fifo_t
 * @return  Le premier élément de la Fifo_t
 */

nodeindex_t fifo_front(Fifo_t * fifo){
	nodeindex_t result ;
	result = fifo->_data[fifo->_head];
	pDEBUG("value %ld\n", result);
	pDEBUG("head at %ld\n", fifo->_head);

	return result;
}

/**
 * Lit et retire le premier élément de la Fifo_t
 *
 * @param	Un pointeur sur la Fifo_t
 * @return  Le premier élément de la Fifo_t
 */

nodeindex_t fifo_pop(Fifo_t * fifo){
	nodeindex_t result ;
	result = fifo->_data[fifo->_head];
	pDEBUG("value %ld\n", result);
	pnDEBUG("head %ld -> ", fifo->_head);

	fifo->_head += 1;
	if (fifo->_head >= fifo->_size){
		fifo->_head = 0;
	}

	fifo->_fill -= 1;
	if (fifo->_fill == 0){ 
		pnDEBUG("(empty) ");
		fifo->_head = -1;
		fifo->_tail = -1;
	}
	assert(fifo->_fill >= 0);
	pnDEBUG("%ld\n", fifo->_head);

	return result;
}



/**
 * Lit un élément au hasard dans la file Fifo_t
 *
 * @param	Un pointeur sur la Fifo_t
 */

nodeindex_t fifo_get_random(Fifo_t * fifo){
	nodeindex_t idx = rand() % fifo_get_size(fifo);
	nodeindex_t result;
	pDEBUG("between %ld and %ld (size %ld)\n", 
			(long)fifo->_head, 
			(long)fifo->_tail, 
			fifo->_size);
	idx += fifo->_head;
	if (idx >= fifo->_size){
		idx = idx % (fifo->_size);	
	}
	result = fifo->_data[idx];
	pDEBUG("gives %ld (value = %ld)\n",idx, result);
	return result;	
}


/**
 * Indique la taille de la file Fifo_t passée en paramètre
 *
 * @param	Un pointeur sur la Fifo_t
 */

nodeindex_t fifo_get_size(Fifo_t * fifo){
	return fifo->_fill;
}




/**
 * Affiche le contenu de la file Fifo_t sur la sortie
 * standard
 */

void fifo_display(Fifo_t * fifo){
	int idx = fifo->_head;
 	bool done = false;
	pDEBUG(" ");
	printf("[");
	while (!done){
		if (idx == -1) { break; }
		if (idx == fifo->_tail) { done = true; }
		printf("%ld", fifo->_data[idx]);
		
		if (!done){ printf(", "); }

		idx += 1;
		if (idx >= fifo->_size){ idx = 0; }
	}
	printf(" ]\n");
}

#undef DEBUG
