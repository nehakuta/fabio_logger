
#include <stddef.h>
#include <stdlib.h>
#include "spiflash_funct.h"

int main(int argc, char **argv)
{
	// TODO: initialize SPI controller
	
	spi_handle_t *spi_dummy = NULL;	 // TODO
	
	/* Erase 64kB (SPIFLASH_ERASE_SIZE) starting from address 0 */
	spiflash_erase(spi_dummy, uint32_t start_adr, uint32_t num_bytes);

	/* Write message to flash address 0 */
	const char msg[] = "Hello World!\0";
	spiflash_write(spi_dummy, 0, msg, sizeof(msg));
	
	/* Read 16 bytes into buffer from address 0 */
	char buf[16];
	spiflash_read(spi_dummy, 0, buf, sizeof(buf));	
	/* buf now contains "Hello World!\x\xff\xff\xff" */
	return 0;
}

