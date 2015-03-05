#ifndef MINMAX_H
#define MINMAX_H

/* It's not Standard C, but if min() and max() are defined, then here: */
#include <stdlib.h>

/* Still not defined: defined it now */
#ifndef min
	#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

#ifndef max
	#define max(a,b) ((a) > (b) ? (a) : (b))
#endif

#endif
