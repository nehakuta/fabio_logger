/**
 * Functions for standard SPI flashes
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>
#include "result.h"
#include "my_time.h"
#include "spi_hal.h"
#include "spiflash_defs.h"
#include "spiflash_funct.h"
#include "minmax.h"
#include "is_all.h"

#ifndef SPIFLASH_SKIP_UNNEEDED_WRITES
/* Don't implement this feature by default */
	#define SPIFLASH_SKIP_UNNEEDED_WRITES	false
#endif

#ifndef SPIFLASH_FAST_ERASE
/* Don't use fast erase by default */
	#define SPIFLASH_FAST_ERASE 		false
#endif

#ifndef SPIFLASH_PAGE_SIZE
	#define SPIFLASH_PAGE_SIZE			256
#endif

#ifndef SPIFLASH_WR_TIMEOUT_US
	#define SPIFLASH_WR_TIMEOUT_US		10000
#endif

#ifndef SPIFLASH_ERASE_SIZE
	#define SPIFLASH_ERASE_SIZE			0x10000
#endif

#ifndef SPIFLASH_ERASE_CMD
	#define SPIFLASH_ERASE_CMD			SFL_CMD_ERASE_D8
#endif

/**
 * Read the status register of the SPI FLASH
 *
 * \return the status register value
 */
int spiflash_read_status(spi_handle_t *const spi)
{
	assert(spi != NULL);
    uint8_t status, cmd = SFL_CMD_RDSR;
    spi_write(spi, &cmd, 1, CS_SET);  
    spi_read(spi, &status, 1, CS_CLEAR_AFTER); 

    assert(status != 0xff);
    return status;
}

/**
 * Wait until write in progress bit is cleared
 *
 * \param handle a pointer to the SPI device
 * \param timeout_ms timeout in ms
 *
 *  \return Status
 *  		- #RES_OK
 *  		- #RES_ERROR
 *  		- #RES_TIMEOUT
 */
int spiflash_wait_complete(spi_handle_t *const spi, unsigned timeout_us)
{
	int status;
	assert(spi != NULL);
	struct timeval timer;

	timer_set_timeout_us(&timer, timeout_us);

	while (1) {
		status = spiflash_read_status(spi);

		if (status < 0) {
			return RES_ERROR;
		}

		/* wait until write in progress bit (WIP) is false */
		if (!(status & SFL_ST_WIP)) {
			break;
		}

		if (timer_expired(&timer)) {
			return RES_TIMEOUT;
		}
	}

	return RES_OK;
}

/**
 * SPI Flash write function
 */
int spiflash_write(spi_handle_t *const spi, uint32_t start_adr, const void *data, size_t bytes2write)
{
	assert(spi != NULL);
	assert(data != NULL);

	const uint8_t *d = (const uint8_t*)data;
	uint8_t cmd[4];

	/* Set command */
	cmd[0] = SFL_CMD_WRITE3;

	/* Don't write over a page boundary! */
	uint32_t cur_adr = start_adr;
	const uint32_t page_size = SPIFLASH_PAGE_SIZE;
	const uint32_t page_msk = page_size - 1;	
	size_t bytes_written = 0, bytes_remaining = bytes2write;
	
	while (bytes_remaining) {
		const uint32_t bytes2end_of_page = page_size - (cur_adr & page_msk);
		size_t write_bytes = min(bytes2end_of_page, bytes_remaining);

		/* Set address bytes */
		cmd[1] = (cur_adr >> 16) & 0xff;
		cmd[2] = (cur_adr >> 8) & 0xff;
		cmd[3] = (cur_adr >> 0) & 0xff;

		if (SPIFLASH_SKIP_UNNEEDED_WRITES && is_all(&d[bytes_written], 0xff, write_bytes)) {
			/* Writing 0xff has no effect, so it can be skipped to speed up writing */
			continue;
		}
		
		/* Send request */
		spiflash_write_enable(spi);
		spi_write(spi, cmd, sizeof(cmd), CS_SET);
		spi_write(spi, &d[bytes_written], write_bytes, CS_CLEAR_AFTER);

		bytes_written += write_bytes;
		bytes_remaining -= write_bytes;
		cur_adr += write_bytes;
		
		/* Wait until write is completed */
		int res = spiflash_wait_complete(spi, SPIFLASH_WR_TIMEOUT_US);
		if (res != RES_OK) {
			return res;
		}
	}
	
	return RES_OK;
}

/**
 * SPI Flash read function 
 */
int spiflash_read(spi_handle_t *const spi, uint32_t start_adr, void *const data, size_t bytes2read)
{
	assert(spi != NULL);
	assert(data != NULL);
	
	uint8_t cmd[5];
	
	/* Set command */
	cmd[0] = SFL_CMD_FAST_READ3;
	
	/* Set address bytes */
	cmd[1] = (start_adr >> 16) & 0xff;
	cmd[2] = (start_adr >> 8) & 0xff;
	cmd[3] = (start_adr >> 0) & 0xff;

	/* Set dummy byte */
	cmd[4] = 0x00;

	/* Send request */
	spi_write(spi, cmd, sizeof(cmd), CS_SET);
	spi_read(spi, data, bytes2read, CS_CLEAR_AFTER);
	return RES_OK;
}

/**
 * Generic flash erase function
 * 
 * Note: This function doesn't wait for completion of the erase!
 */
int spiflash_erase_start(spi_handle_t *const spi, uint8_t erase_cmd, uint32_t start_adr)
{
	assert(spi != NULL);

	uint8_t cmd[4];
	
	/* Set command */
	cmd[0] = erase_cmd;
	
	/* Set address bytes */
	cmd[1] = (start_adr >> 16) & 0xff;
	cmd[2] = (start_adr >> 8) & 0xff;
	cmd[3] = (start_adr >> 0) & 0xff;
	
	/* Send request */
	spiflash_write_enable(spi);
	spi_write(spi, cmd, sizeof(cmd), CS_SET | CS_CLEAR_AFTER);
	return RES_OK;
}

/**
 * Return true if all data bytes within the given address range are 0xff
 */
bool spiflash_is_all_0xff(spi_handle_t *const spi, uint32_t start_adr, uint32_t num_bytes)
{
	assert(spi != NULL);

	uint8_t buf[128];
	uint32_t cur_adr;

	assert((num_bytes % sizeof(buf)) == 0);
		
	for (cur_adr = start_adr; cur_adr < start_adr + num_bytes; cur_adr += sizeof(buf)) {
		int res = spiflash_read(spi, cur_adr, buf, sizeof(buf));
		assert(res == RES_OK);
		
		if (!is_all(buf, 0xff, sizeof(buf))) {
			return false;
		}
	}
	
	/* No zero bit found */
	return true;
}

/**
 * 
 * 
 * \param estate Erase state machine state
 * \param start_adr the start address of the memory area which shall be erased
 * \param bytes2erase the number of bytes to erase
 * \return a result code
 */
int spiflash_start_erase_task(sfl_erase_state_t *const estate, uint32_t start_adr, uint32_t bytes2erase)
{
	assert(estate != NULL);
	assert(estate->state == ES_IDLE);
	
	estate->cur_adr = start_adr;
	estate->start_adr = start_adr;
	estate->end_adr = start_adr + bytes2erase;
	
#if SPIFLASH_FAST_ERASE
	estate->state = ES_BLANK_CHECK;
#else
	estate->state = ES_ERASE;
#endif
	return RES_OK;
}

/**
 * Erase given memory range (non-blocking version)
 * 
 * \param handle the SPI flash handle
 * \param start_adr the start address of the memory area which shall be erased
 * \param bytes2erase the number of bytes to erase
 * \return a result code
 */
void spiflash_erase_task(spi_handle_t *const spi, sfl_erase_state_t *const estate)
{
	assert(spi != NULL);
	assert(estate != NULL);
		
	assert(estate->state < _ES_MAX_STATE);

	if (estate->state == ES_IDLE || estate->state == ES_ERASE_DONE) {
		return; /* nothing to do */
	}

	if (SPIFLASH_FAST_ERASE && estate->state == ES_BLANK_CHECK) {
		if (spiflash_is_all_0xff(spi, estate->cur_adr, SPIFLASH_ERASE_SIZE)) {
			/* Erase not needed because all bytes are 0xff */
			estate->state = ES_WAIT_DONE;
		} else {
			estate->state = ES_ERASE;
		}
		return; /* Return to avoid long block */
	}
	
	/* 
	 * Send the erase command to the flash 
	 */
	if (estate->state == ES_ERASE) {
		spiflash_erase_start(spi, SPIFLASH_ERASE_CMD, estate->cur_adr);
		estate->state = ES_WAIT;
		return;
	}

	/* 
	 * Wait while write in progress bit (WIP) is set
	 */
	if (estate->state == ES_WAIT) {
		// TODO: with timeout!
		uint8_t status = spiflash_read_status(spi);

		if (!(status & SFL_ST_WIP)) {
			estate->state = ES_WAIT_DONE;
		}
	}
	
	/* 
	 * Increment address and check if we are finished
	 */
	if (estate->state == ES_WAIT_DONE) {
		estate->cur_adr += SPIFLASH_ERASE_SIZE;
		
		if (estate->cur_adr >= estate->end_adr) {
			estate->state = ES_ERASE_DONE;
		} else if (SPIFLASH_FAST_ERASE) {
			estate->state = ES_BLANK_CHECK;
		} else {
			estate->state = ES_ERASE;
		}
	}

	// TODO: common timeout handling here!
	return;
}

/**
 * Generic flash erase function
 * 
 * Note: This function doesn't wait for completion of the erase!
 */
int spiflash_erase(spi_handle_t *const spi, uint32_t start_adr, uint32_t bytes2erase)
{
	assert(spi != NULL);

	sfl_erase_state_t estate = {
		.state = ES_IDLE
	};

	spiflash_start_erase_task(&estate, start_adr, bytes2erase);
	
	while (estate.state != ES_ERASE_DONE) {
		spiflash_erase_task(spi, &estate);
	}
	
	return RES_OK;
}

