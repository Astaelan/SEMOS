extern "C" {
#include <stdlib.h>
#include <stdio.h>
}
#include <PortIO.h>
#include <Core/IDT.h>
#include <Hardware/ATA/ATAPIDevice.h>

using namespace SEMOS;
using namespace SEMOS::Core;

#define ATAPIDEVICE_IO_COMMAND_PACKET           0xA0
#define ATAPIDEVICE_IO_COMMAND_READSECTORS      0xA8


UINT32 ATAPIDevice_ReadSector(ATADevice * pDevice,
                              UINT32 pLBA28Address,
                              PBYTE pBuffer)
{
    /// Clean this up, ATAPI packet commands are 6 words (12 bytes)
    BYTE cmdRead[12];
    cmdRead[0] = ATAPIDEVICE_IO_COMMAND_READSECTORS;
    cmdRead[2] = (pLBA28Address >> 0x18) & 0xFF;
    cmdRead[3] = (pLBA28Address >> 0x10) & 0xFF;
    cmdRead[4] = (pLBA28Address >> 0x08) & 0xFF;
    cmdRead[5] = (pLBA28Address >> 0x00) & 0xFF;
    cmdRead[9] = 0x01;

    outb(pDevice->BaseRegister + ATADEVICE_IO_SELECT, pDevice->Drive & (1 << 4));
    outb(pDevice->BaseRegister + ATADEVICE_IO_FEATURES, 0x00);
    outb(pDevice->BaseRegister + ATADEVICE_IO_ADDRESS1, (ATAPIDEVICE_IO_SECTOR_SIZE & 0xFF));
    outb(pDevice->BaseRegister + ATADEVICE_IO_ADDRESS2, ((ATAPIDEVICE_IO_SECTOR_SIZE >> 8) & 0xFF));
    outb(pDevice->BaseRegister + ATADEVICE_IO_COMMAND, ATAPIDEVICE_IO_COMMAND_PACKET);
    while (inb(pDevice->BaseRegister + ATADEVICE_IO_COMMAND) & ATADEVICE_IO_STATUS_BUSY);

    BYTE status;
    while (!((status = inb(pDevice->BaseRegister + ATADEVICE_IO_COMMAND)) & (ATADEVICE_IO_STATUS_DATAREQUEST | ATADEVICE_IO_STATUS_ERROR)));
    if (status & ATADEVICE_IO_STATUS_ERROR) return 0;

    IDT::Schedule(pDevice->Interrupt);
    outw(pDevice->BaseRegister + ATADEVICE_IO_DATA, *(((UINT16 *)cmdRead) + 0));
    outw(pDevice->BaseRegister + ATADEVICE_IO_DATA, *(((UINT16 *)cmdRead) + 1));
    outw(pDevice->BaseRegister + ATADEVICE_IO_DATA, *(((UINT16 *)cmdRead) + 2));
    outw(pDevice->BaseRegister + ATADEVICE_IO_DATA, *(((UINT16 *)cmdRead) + 3));
    outw(pDevice->BaseRegister + ATADEVICE_IO_DATA, *(((UINT16 *)cmdRead) + 4));
    outw(pDevice->BaseRegister + ATADEVICE_IO_DATA, *(((UINT16 *)cmdRead) + 5));
    IDT::WaitFor(pDevice->Interrupt);

    UINT32 sizeBytes = (inb(pDevice->BaseRegister + ATADEVICE_IO_ADDRESS2) << 8) | inb(pDevice->BaseRegister + ATADEVICE_IO_ADDRESS1);
    UINT32 sizeWords = sizeBytes >> 1;
    for (UINT16 * buf = (UINT16 *)pBuffer; sizeWords; --sizeWords, ++buf) *buf = inw(pDevice->BaseRegister + ATADEVICE_IO_DATA);

    while (inb(pDevice->BaseRegister + ATADEVICE_IO_COMMAND) & (ATADEVICE_IO_STATUS_DATAREQUEST | ATADEVICE_IO_STATUS_BUSY));

    return sizeBytes;
}
