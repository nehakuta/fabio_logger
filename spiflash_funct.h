/** 
 * \defgroup SPIFLASH_FUNCT_H SPI Flash Functions
 *
 * \see SPIFLASH_DEFS
 * \{
 */

#ifndef SPIFLASH_FUNCT_H
#define SPIFLASH_FUNCT_H

#include <stdint.h>
#include <stdbool.h>
#include "spi_hal.h"
#include "spiflash_defs.h"

enum sfl_erase_sm_state {
	ES_IDLE,
	ES_BLANK_CHECK,
	ES_ERASE,
	ES_WAIT,
	ES_WAIT_DONE,
	ES_ERASE_DONE,
	_ES_MAX_STATE
};

typedef struct sfl_erase_state_t {
	uint8_t state;
	uint32_t cur_adr;
	uint32_t start_adr;
	uint32_t end_adr;
} sfl_erase_state_t;

/**
 * Sends the Write-Enable Command to the Flash which is required before every write operation actions
 */
static inline void spiflash_write_enable(spi_handle_t *const spi)
{
	const uint8_t cmd = SFL_CMD_WREN;
	spi_write(spi, &cmd, 1, CS_SET | CS_CLEAR_AFTER);
}

/**
 * Write status register
 */
static inline int spiflash_write_sreg(spi_handle_t *const spi, uint8_t stat_reg)
{
	const uint8_t cmd[2] = {SFL_CMD_WRSR, stat_reg};
	return spi_write(spi, cmd, 2, CS_SET | CS_CLEAR_AFTER);
}


/**
 * Sends the Write-Enable Command to the Flash which is required before every write operation actions
 */
static inline void spiflash_write_disable(spi_handle_t *const spi)
{
	const uint8_t cmd = SFL_CMD_WRDIS;
    spi_write(spi, &cmd, 1, CS_SET | CS_CLEAR_AFTER);
}

int spiflash_erase_start(spi_handle_t *const spi, uint8_t erase_cmd, uint32_t start_adr);
int spiflash_read(spi_handle_t *const spi, uint32_t start_adr, void *const data, size_t bytes2read);
int spiflash_write(spi_handle_t *const spi, uint32_t start_adr, const void *data, size_t bytes2write);

/* Low level functions */
void spiflash_write_enable(spi_handle_t *const handle);
int spiflash_read_status(spi_handle_t *const handle);
int spiflash_wait_complete(spi_handle_t *const handle, unsigned timeout_us);
bool spiflash_is_all_0xff(spi_handle_t *const spi, uint32_t start_adr, uint32_t num_bytes);
void spiflash_erase_task(spi_handle_t *const spi, sfl_erase_state_t *const estate);
int spiflash_start_erase_task(sfl_erase_state_t *const estate, uint32_t start_adr, uint32_t bytes2erase);
int spiflash_erase(spi_handle_t *const spi, uint32_t start_adr, uint32_t num_bytes);

#endif

/** \} */
