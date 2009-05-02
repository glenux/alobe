/* vim: set sw=4 ts=4 si et: */		

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"

int * int_create(int value){
	int * var = ((int*) malloc (sizeof(int)));
	*var = value;
	return var;
}

void int_destroy(void * daInt){
	int * truc = daInt;
	free(truc);
}

int main(){
	List_t * list = list_create();
	int * value, * final;
	int z;

	printf("************************************************************************\n");
	printf("                  PHASE 1 - Creation and base test (push_back)\n");
	printf("************************************************************************\n");

	assert(list_is_empty(list) == true);

	value = int_create(1);
	printf ("Test push_back value %d\n",*value);
	list_push_back (list, value);

	final = list_back(list);
	printf ("Test back (%d)\n",*final);
	assert ((*final) == 1);

	final = list_pop_back(list);
	printf ("Test pop_back (%d)\n",*final);
	assert ((*final) == 1);
	int_destroy (final);

	final = list_pop_back(list);
	printf ("Test pop_back on empty\n");
	assert (final == NULL);

	printf("************************************************************************\n");
	printf("                 PHASE 2 - Multiple (non-equal) push and pop\n");
	printf("************************************************************************\n");

	for (z=0; z<100; z++){
		value = int_create(z);
		list_push_back (list, value);
	}
	for (z=0; z<100; z++){
		final = list_pop_back(list);
		int_destroy (final);
	}

	final = list_pop_back(list);
	printf ("Test pop_back on empty\n");
	assert (final == NULL);

	printf("************************************************************************\n");
	printf("                          PHASE 3 - Empty destroy test\n");
	printf("************************************************************************\n");
	printf("Test destroy with data\n");
	list_destroy_with_data(list, int_destroy);

	printf("************************************************************************\n");
	printf("                  PHASE 4 - Creation and base test (push_front)\n");
	printf("************************************************************************\n");
	list = list_create();
	value = int_create(1);
	printf ("Test push_front value %d\n",*value);
	list_push_front (list, value);
	value = int_create(2);
	list_push_front (list, value);

	final = list_front(list);
	printf ("Test front (%d)\n",*final);
	assert ((*final) == 2);

	final = list_back(list);
	printf ("Test back (%d)\n",*final);
	assert ((*final) == 1);

	final = list_pop_front(list);
	printf ("Test pop_front (%d)\n",*final);
	assert ((*final) == 2);
	int_destroy (final);

	// FIXME: on ne devrait pouvoir dire rien d'intéressant ici, le null n'est pas 
	// obligatoire...
	//final = list_pop_front(list);
	//printf ("Test pop_front on empty\n");
	//assert (final == NULL);

	printf("************************************************************************\n");
	printf("PHASE 5 - ...\n");
	printf("************************************************************************\n");

	for (z=0; z<100; z++){
		value = int_create(z);
		list_push_front (list, value);
	}
	for (z=0; z<100; z++){
		final = list_pop_front(list);
		int_destroy (final);
	}

	/*
	printf ("Test pop_front on empty\n");
	final = list_pop_front(list);
	assert (final == NULL);
	*/

	printf("Test destroy with data\n");
	list_destroy_with_data(list, int_destroy);


	return EXIT_SUCCESS;
}
