/* vim: set sw=4 ts=4 si et: */		

#include "progressindicator.h"

char * progress_indicator(){
	char * result;
	static int prog = 0;
    int max = 1200;
	prog = prog % max;
	if (prog < (max/4)){
		result = progress_indicator_table[0];
	} else if (prog < (max/2)) {
		result = progress_indicator_table[1];
	} else if (prog < ((3 * max)/4)) {
		result = progress_indicator_table[2];
	} else {
		result = progress_indicator_table[3];
	}
	prog += 1;
	return result;
}
