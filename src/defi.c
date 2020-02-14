/* vim: set sw=4 ts=4 si et: */		

#include "defi.h"
#include <string.h>
#include <errno.h>

#define DEBUG 1

Defi_t * defi_create(Config_io_t * io, nodeindex_t size){
	Defi_t * defi = (Defi_t *) malloc (sizeof(Defi_t));

	defi->_io = io;
	defi->_size = size;
	defi->_list = connexcomponentlist_create();
	return defi;
}

void defi_destroy(Defi_t * defi){
	free (defi);
}

void defi_run(Defi_t * defi){
	printf("Run !!!\n");		

	char * buf = (char *) malloc (sizeof(char) * 100);
	char * buf2;
	nodeindex_t one, two;
	int re;

	gzrewind(defi->_io->input);
	gzclearerr(defi->_io->input);

	printf("Searching connex comFilling the Big Table...\n");
	for (;;){
		if (gzeof(defi->_io->input)){ break; }

		buf2 = gzgets(defi->_io->input, buf, 100);
		if (buf2 == Z_NULL){
			printf("READ OFF\n");
			break;
		} else {
			re = sscanf(buf, "%ld %ld", &one, &two);
			if (re < 0) {
			    printf("sscanf() failed: %s", strerror(errno));
			    exit(1);
			}
			pDEBUG("=== DEFI READING  %ld %ld ===\n", one, two);
			defi->_list = 
				connexcomponentlist_insert_edge(defi->_list, one, two);

			if (DEBUG) { connexcomponentlist_display(defi->_list); }
		}
	}
	connexcomponentlist_destroy(&(defi->_list));
	free(buf);
	printf("done\n");

}

#undef DEBUG
