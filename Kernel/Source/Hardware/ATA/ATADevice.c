#include <stdlib.h>
#include <PortIO.h>
#include <Hardware/PIC.h>
#include <Hardware/ATA/ATADevice.h>

#define ATADEVICE_SIGNATURE_PATA		    0x0000
#define ATADEVICE_SIGNATURE_PATAPI		    0xEB14
#define ATADEVICE_SIGNATURE_SATA		    0xC33C
#define ATADEVICE_SIGNATURE_SATAPI		    0x9669

#define ATADEVICE_IO_CONTROL_SOFTRESET      0x04


ATADevice * ATADevice_Initialize(UINT16 pBaseRegister,
                                 UINT16 pControlRegister,
                                 BYTE pInterrupt,
                                 BOOL pSlave)
{
    ATADevice * device = (ATADevice *)malloc(sizeof(ATADevice));
    device->BaseRegister = pBaseRegister;
    device->ControlRegister = pControlRegister;
    device->Interrupt = PIC_IRQ_MASTER_BASE + pInterrupt;
    device->Drive = 0xA0 | (pSlave << 4);

    ATADevice_Reset(device);
    UINT16 signature = inb(device->BaseRegister + ATADEVICE_IO_ADDRESS1) | (inb(device->BaseRegister + ATADEVICE_IO_ADDRESS2) << 8);
    switch (signature)
    {
        case ATADEVICE_SIGNATURE_PATA: device->Type = ATADEVICE_TYPE_PATA; break;
        case ATADEVICE_SIGNATURE_PATAPI: device->Type = ATADEVICE_TYPE_PATAPI; break;
        case ATADEVICE_SIGNATURE_SATA: device->Type = ATADEVICE_TYPE_SATA; break;
        case ATADEVICE_SIGNATURE_SATAPI: device->Type = ATADEVICE_TYPE_SATAPI; break;
        default: device->Type = ATADEVICE_TYPE_NONE; break;
    }
    outb(device->BaseRegister + ATADEVICE_IO_FEATURES, 0x00);
    return device;
}

void ATADevice_Reset(ATADevice * pDevice)
{
    outb(pDevice->ControlRegister, ATADEVICE_IO_CONTROL_SOFTRESET);
    for (int index = 0; index < 4; ++index) inb(pDevice->ControlRegister);
    outb(pDevice->ControlRegister, 0);
    for (int index = 0; index < 4; ++index) inb(pDevice->ControlRegister);
    ATADevice_SelectDrive(pDevice);
}

void ATADevice_SelectDrive(ATADevice * pDevice)
{
    outb(pDevice->BaseRegister + ATADEVICE_IO_SELECT, pDevice->Drive & (1 << 4));
    for (int index = 0; index < 4; ++index) inb(pDevice->ControlRegister);
}

PSTRING ATADevice_GetTypeName(ATADevice* pDevice)
{
    switch (pDevice->Type)
    {
        case ATADEVICE_TYPE_PATA: return "PATA";
        case ATADEVICE_TYPE_PATAPI: return "PATAPI";
        case ATADEVICE_TYPE_SATA: return "SATA";
        case ATADEVICE_TYPE_SATAPI: return "SATAPI";
        default: return "NONE";
    }
}
