#ifndef _ATADEVICE_H
#define _ATADEVICE_H

#include <types.h>

#define ATADEVICE_IO_PRIMARY_BASE           0x01F0
#define ATADEVICE_IO_SECONDARY_BASE         0x0170
#define ATADEVICE_IO_PRIMARY_CONTROL        0x03F6
#define ATADEVICE_IO_SECONDARY_CONTROL      0x0376
#define ATADEVICE_IRQ_PRIMARY               0x0E
#define ATADEVICE_IRQ_SECONDARY             0x0F

#define ATADEVICE_IO_DATA                   0x00
#define ATADEVICE_IO_FEATURES               0x01
#define ATADEVICE_IO_SECTORS                0x02
#define ATADEVICE_IO_ADDRESS0               0x03
#define ATADEVICE_IO_ADDRESS1               0x04
#define ATADEVICE_IO_ADDRESS2               0x05
#define ATADEVICE_IO_SELECT                 0x06
#define ATADEVICE_IO_COMMAND                0x07

#define ATADEVICE_IO_STATUS_ERROR           0x01
#define ATADEVICE_IO_STATUS_DATAREQUEST     0x08
#define ATADEVICE_IO_STATUS_SERVICE         0x10
#define ATADEVICE_IO_STATUS_DRIVEFAULT      0x20
#define ATADEVICE_IO_STATUS_READY           0x40
#define ATADEVICE_IO_STATUS_BUSY            0x80

#define ATADEVICE_TYPE_NONE				    0x00
#define ATADEVICE_TYPE_PATA				    0x01
#define ATADEVICE_TYPE_PATAPI			    0x02
#define ATADEVICE_TYPE_SATA				    0x03
#define ATADEVICE_TYPE_SATAPI			    0x04

typedef struct
{
	UINT16 BaseRegister;
	UINT16 ControlRegister;
	BYTE Interrupt;
	BYTE Drive;
	BYTE Type;
} ATADevice;

ATADevice * ATADevice_Initialize(UINT16 pBaseRegister, UINT16 pControlRegister, BYTE pInterrupt, BOOL pSlave);
void ATADevice_Reset(ATADevice * pDevice);
void ATADevice_SelectDrive(ATADevice * pDevice);
PSTRING ATADevice_GetTypeName(ATADevice * pDevice);

#endif