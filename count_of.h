#ifndef COUNT_OF_H
#define COUNT_OF_H

#include <stddef.h>
#define count_of(x) ((sizeof(x) / sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

#endif
