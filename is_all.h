#ifndef IS_ALL_H
#define IS_ALL_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

bool is_all(const void *data, uint8_t value, size_t num_bytes);

#endif
