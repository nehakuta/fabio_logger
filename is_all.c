#include "is_all.h"

/** Return true if all data has given value */
bool is_all(const void *data, uint8_t value, size_t num_bytes)
{
	size_t i;
	const uint8_t *p = (uint8_t*)data;
	
	for (i = 0; i < num_bytes; i++) {
		if (p[i] != value) {
			return false;
		}
	}
	
	return true;
}
