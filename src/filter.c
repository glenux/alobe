/* vim: set sw=4 ts=4 si et: */		

#include "filter.h"

Filter_t * filter_create(Config_io_t * io, nodeindex_t size, nodeindex_t offset)
{
	Filter_t * filter = (Filter_t*) malloc (sizeof(Filter_t));
	
	filter->_io = io;
	filter->_size = size;
	filter->_offset = offset;

	return filter;
}

void filter_destroy(Filter_t * filter){
	free(filter);
	filter = NULL;
}

void filter_run(Filter_t * filter){

	printf("filter_run -- Filtering between [ %d .. %d ]...\n", 
			filter->_offset, 
			filter->_size + filter->_offset);

	char * buf = (char *) malloc (sizeof(char) * 100);
	char * buf2;
	for (;;){
		if (gzeof(filter->_io->input)){ break; }

		buf2 = gzgets (filter->_io->input, buf, 100);
		if (buf2 == Z_NULL){
			printf ("filter_run -- READ OFF\n");
			break;
		} else {
			nodeindex_t one, two;
			sscanf(buf,"%ld %ld",&one,&two);
			if ((one >= filter->_offset) 
					&& (one < (filter->_offset + filter->_size))
					&& (two >= filter->_offset)
					&& (two < (filter->_offset + filter->_size))
					){
				fwrite (buf2, sizeof(char), strlen(buf2), filter->_io->output);
			}
		}
	}
	printf("filter_run -- done\n");
}

