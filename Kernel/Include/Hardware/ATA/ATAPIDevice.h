#ifndef _ATAPIDEVICE_H
#define _ATAPIDEVICE_H

#include <types.h>
#include <Hardware/ATA/ATADevice.h>

#define ATAPIDEVICE_IO_SECTOR_SIZE                 2048

UINT32 ATAPIDevice_ReadSector(ATADevice * pDevice, UINT32 pLBA28Address, PBYTE pBuffer);

#endif