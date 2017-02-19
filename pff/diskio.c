/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for Petit FatFs (C)ChaN, 2014      */
/*-----------------------------------------------------------------------*/

#include "diskio.h"
#include "../sd.h"
#include "../pins.h"
#include <avr/io.h>

DSTATUS disk_initialize (void)
{
	DSTATUS stat;

    if(initSD(&SD_CS_PORT, &SD_CS_DDR, SD_CS_BIT) == 0){
        // Failed:
        stat = RES_ERROR;
    } else {
        // Worked:
        stat = RES_OK;
    }

	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Partial Sector                                                   */
/*-----------------------------------------------------------------------*/

DRESULT disk_readp (
                    BYTE* buff,		/* Pointer to the destination object */
                    DWORD sector,	/* Sector number (LBA) */
                    UINT offset,	/* Offset in the sector */
                    UINT count		/* Byte count (bit15:destination) */
                    )
{
	DRESULT res;

    if(readSD(buff, sector, offset, count) == 0){
        // Failed
        res = RES_ERROR;
    } else {
        // Worked
        res = RES_OK;
    }

	return res;
}



/*-----------------------------------------------------------------------*/
/* Write Partial Sector                                                  */
/*-----------------------------------------------------------------------*/

DRESULT disk_writep (
                     const BYTE* buff,		/* Pointer to the data to be written, NULL:Initiate/Finalize write operation */
                     DWORD sc		/* Sector number (LBA) or Number of bytes to send */
                     )
{
	DRESULT res;


	if (!buff) {
		if (sc) {

			// Initiate write process

		} else {

			// Finalize write process

		}
	} else {

		// Send data to the disk

	}

	return res;
}

