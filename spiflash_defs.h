/** 
 * \defgroup SPIFLASH_DEFS Definitions for standard SPI flashes
 *
 * \{
 */
#ifndef SPIFLASH_DEFS_H
#define SPIFLASH_DEFS_H

#define SFL_PAGE_SIZE				256		/**< Usually, not guaranteed */

/* SPI Flash Commands */
#define SFL_CMD_WREN	  		 	0x06	/**< Write Enable */
#define SFL_CMD_WRDIS	  			0x04	/**< Write Disable */
#define SFL_CMD_RDSR				0x05	/**< Read Status register */
#define SFL_CMD_WRSR				0x01	/**< Write Status register */
#define SFL_CMD_READ3				0x03	/**< Read Memory with 3 address bytes */
#define SFL_CMD_FAST_READ3			0x0B	/**< Read Memory at higher clock speed with 3 address bytes */
#define SFL_CMD_WRITE3				0x02	/**< Byte Program */
#define SFL_CMD_AAI_WRITE			0xAD	/**< Address Auto Increment (AAI) programming */

#define SFL_CMD_CHIP_ERASE_60	 	0x60	/**< Erase Full Memory Array */
#define SFL_CMD_CHIP_ERASE_C7	 	0xC7	/**< Erase Full Memory Array */
#define SFL_CMD_CHIP_ERASE_C4	 	0xC4	/**< Erase Full Memory Array */

#define SFL_CMD_ERASE_20		  	0x20	/**< Erase Memory (usually 4kByte block) */
#define SFL_CMD_ERASE_52		  	0x52	/**< Erase Memory (usually 32kByte block) */
#define SFL_CMD_ERASE_D8		  	0xD8	/**< Erase Memory (usually 64kByte block) */
	
#define SFL_CMD_ERASE4_20			0x20	/**< Erase Memory with 4 address bytes */
#define SFL_CMD_ERASE4_21			0x21	/**< Erase Memory with 4 address bytes */
#define SFL_CMD_ERASE4_DC			0xDC	/**< Erase Memory with 4 address bytes, erase size is usually same as 0xD8 */

#define SFL_CMD_RDID_90				0x90	/**< Read Device ID, vendor specific, output many dummy bytes */
#define SFL_CMD_RDID_AB				0xAB	/**< Read Device ID, vendor specific, output many dummy bytes  */

#define SFL_CMD_RD_JEDEC_ID_9E 		0x9E	/**< JEDEC ID Read */
#define SFL_CMD_RD_JEDEC_ID_9F 		0x9F	/**< JEDEC ID Read */

#define SFL_CMD_RD_SFDP				0x5A	/**< Read Serial Flash Discoverable Parameters (SFDP) @see sfdp_hdr_t */

#define SFL_CMD_READ4				0x13	/**< Read Memory with 4 address bytes */
#define SFL_CMD_FAST_READ4			0x0C	/**< Read Memory at higher clock speed with 4 address bytes */
#define SFL_CMD_WRITE4				0x12	/**< Write Memory with 4 address bytes */

	
/* Common SPI Flash status register bits */
#define SFL_ST_WIP			(1 << 0)	/**< Write In Progress */
#define SFL_ST_WEL			(1 << 1)	/**< Write Enable Latch */
#define SFL_ST_BP0			(1 << 2)	/**< Block Protect 0 */
#define SFL_ST_BP1			(1 << 3)	/**< Block Protect 1 */
#define SFL_ST_BP2			(1 << 4)	/**< Block Protect 2 */

#endif
/** \} */

