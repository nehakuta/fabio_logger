#include <stdlib.h>
#include <stddef.h>
#include "result.h"
#include "spi_hal.h"

/**
 * Write data to a SPI slave and discard incoming data
 *
 * \param handle a handle to the SPI bus
 * \param data a pointer to the data to write
 * \param num_bytes the number of bytes to write
 * \return a result code
 */
int spi_write(spi_handle_t *const handle, const void *data, size_t num_bytes, unsigned flags)
{
	// TODO
	return RES_OK;
}

/**
 * Send zero bytes and read data from a SPI slave
 *
 * \param handle a handle to the SPI bus
 * \param buf buffer where the bytes shall be written to
 * \param num_bytes number of bytes to read
 * \return a result code
 */
int spi_read(spi_handle_t *const handle, void *const buf, size_t num_bytes, unsigned flags)
{
	// TODO
	return RES_OK;
}

