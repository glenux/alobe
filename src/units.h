#ifndef _ALOBE_UNITS_H
#define _ALOBE_UNITS_H

#define bool short
#define true 1
#define false 0

typedef long nodeindex_t;

/* place 0 = degree
 * place 1 = current subnode visit index
 * place 2 = node reference
 */

#define COLOR_RED "\x1B[31m"
#define COLOR_GREEN "\x1B[32m"
#define COLOR_YELLOW "\x1B[33m"
#define COLOR_NORMAL "\x1B[0m"


/**************************************************************************
 * Quelques macros
 */

#define MAX(x, y)	  ( (x) > (y) ? (x) : (y) )

#define MIN(x, y)	  ( (x) > (y) ? (y) : (x) )
										   
#define pDEBUG(...)   if (DEBUG){ \
						fprintf(stderr, "%s%s%s at %s:%d -- %s",\
								COLOR_GREEN,\
								__func__,\
								COLOR_NORMAL,\
								__FILE__,\
								__LINE__,\
								COLOR_YELLOW); \
						fprintf(stderr,__VA_ARGS__); \
						fprintf(stderr,"%s",COLOR_NORMAL); fflush(stderr); }

#define pnDEBUG(...)   if (DEBUG){ \
						fprintf(stderr,"%s",COLOR_YELLOW); \
						fprintf(stderr,__VA_ARGS__); \
						fprintf(stderr,"%s",COLOR_NORMAL); \
						fflush(stderr); }

#define STORE_EXTRAPLACE 4
#define STORE_EXTRA_DEGREE 0
#define STORE_EXTRA_VALUE 1
#define STORE_EXTRA_VALUE2 2
#define STORE_EXTRA_REF 3

#define NODEINDEX_UNDEF -1
#define NODEINDEX_ROOT -2
#define NODEINDEX_RANDOM -2
#define NODEINDEX_MAX_CC -3


#endif
