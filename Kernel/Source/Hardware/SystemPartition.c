#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <PortIO.h>
#include <Hardware/SystemPartition.h>
#include <Hardware/IDT.h>
#include <Hardware/VGAText.h>
#include <Hardware/ATA/ATADevice.h>
#include <Hardware/ATA/ATAPIDevice.h>
#include <Utility/List.h>

typedef struct
{
    BYTE EntrySize;
    UINT32 LBA;
    UINT32 Length;
    BYTE Flags;
    PSTRING Identifier;
    List Entries;
} SystemPartitionCache;

BOOL SystemPartition_Detect();
BOOL SystemPartition_DetectATA(UINT16 pBaseAddress);
BOOL SystemPartition_DetectATAPIISO9660(ATADevice * pDevice);
BOOL SystemPartition_DetectATAPIISO9660SystemPartition(ATADevice * pDevice, UINT32 * pDescriptorSector);
void SystemPartition_Cache(SystemPartitionCache * pCache);

ATADevice * gSystemPartitionDevice = NULL;
UINT32 gSystemPartitionDescriptorSector = 0;
SystemPartitionCache gSystemPartitionCache;

void SystemPartition_Initialize()
{
    if(!SystemPartition_Detect()) return;

    PSTRING rootMount = "/SYSTEM";
    UINT32 rootMountLength = strlen(rootMount);

    PBYTE sectorBuffer = (PBYTE)malloc(ATAPIDEVICE_IO_SECTOR_SIZE);
    ATAPIDevice_ReadSectors(gSystemPartitionDevice, gSystemPartitionDescriptorSector, 1, sectorBuffer);

    gSystemPartitionCache.EntrySize = *(sectorBuffer + 156);
    gSystemPartitionCache.LBA = *(UINT32 *)(sectorBuffer + 156 + 2);
    gSystemPartitionCache.Length = *(UINT32 *)(sectorBuffer + 156 + 10);
    gSystemPartitionCache.Flags = *(sectorBuffer + 156 + 25);
    gSystemPartitionCache.Identifier = (PSTRING)malloc(rootMountLength + 1);
    strncpy(gSystemPartitionCache.Identifier, rootMount, rootMountLength);
    gSystemPartitionCache.Identifier[rootMountLength] = 0x00;
    free(sectorBuffer);

    SystemPartition_Cache(&gSystemPartitionCache);
}

BOOL SystemPartition_Detect()
{
    BOOL hasDisk = SystemPartition_DetectATA(ATADEVICE_IO_PRIMARY_BASE);
    if (hasDisk)
    {
        ATADevice * master = ATADevice_Initialize(ATADEVICE_IO_PRIMARY_BASE, ATADEVICE_IO_PRIMARY_CONTROL, ATADEVICE_IRQ_PRIMARY, FALSE);
        UINT32 descriptorSector = 0;
        if (master->Type == ATADEVICE_TYPE_PATAPI &&
            SystemPartition_DetectATAPIISO9660(master) &&
            SystemPartition_DetectATAPIISO9660SystemPartition(master, &descriptorSector))
        {
            gSystemPartitionDevice = master;
            gSystemPartitionDescriptorSector = descriptorSector;
            return TRUE;
        }
        free(master);

        ATADevice * slave = ATADevice_Initialize(ATADEVICE_IO_PRIMARY_BASE, ATADEVICE_IO_PRIMARY_CONTROL, ATADEVICE_IRQ_PRIMARY, TRUE);
        if (slave->Type == ATADEVICE_TYPE_PATAPI &&
            SystemPartition_DetectATAPIISO9660(slave) &&
            SystemPartition_DetectATAPIISO9660SystemPartition(slave, &descriptorSector))
        {
            gSystemPartitionDevice = slave;
            gSystemPartitionDescriptorSector = descriptorSector;
            return TRUE;
        }
        free(slave);
    }
    hasDisk = SystemPartition_DetectATA(ATADEVICE_IO_SECONDARY_BASE);
    if (hasDisk)
    {
        ATADevice * master = ATADevice_Initialize(ATADEVICE_IO_SECONDARY_BASE, ATADEVICE_IO_SECONDARY_CONTROL, ATADEVICE_IRQ_SECONDARY, FALSE);
        UINT32 descriptorSector = 0;
        if (master->Type == ATADEVICE_TYPE_PATAPI &&
            SystemPartition_DetectATAPIISO9660(master) &&
            SystemPartition_DetectATAPIISO9660SystemPartition(master, &descriptorSector))
        {
            gSystemPartitionDevice = master;
            gSystemPartitionDescriptorSector = descriptorSector;
            return TRUE;
        }
        free(master);

        ATADevice * slave = ATADevice_Initialize(ATADEVICE_IO_SECONDARY_BASE, ATADEVICE_IO_SECONDARY_CONTROL, ATADEVICE_IRQ_SECONDARY, TRUE);
        if (slave->Type == ATADEVICE_TYPE_PATAPI &&
            SystemPartition_DetectATAPIISO9660(slave) &&
            SystemPartition_DetectATAPIISO9660SystemPartition(slave, &descriptorSector))
        {
            gSystemPartitionDevice = slave;
            gSystemPartitionDescriptorSector = descriptorSector;
            return TRUE;
        }
        free(slave);
    }
    return FALSE;
}

BOOL SystemPartition_DetectATA(UINT16 pBaseAddress)
{
    outb(pBaseAddress + ATADEVICE_IO_SECTORS, 0xDE);
    outb(pBaseAddress + ATADEVICE_IO_ADDRESS0, 0xAD);
    return inb(pBaseAddress + ATADEVICE_IO_SECTORS) == 0xDE && inb(pBaseAddress + ATADEVICE_IO_ADDRESS0) == 0xAD;
}

BOOL SystemPartition_DetectATAPIISO9660(ATADevice * pDevice)
{
    BYTE sectorBuffer[ATAPIDEVICE_IO_SECTOR_SIZE];
    UINT32 bytesRead = ATAPIDevice_ReadSectors(pDevice, 0x10, 1, sectorBuffer);
    if (bytesRead < 6) return FALSE;
    return sectorBuffer[1] == 'C' && sectorBuffer[2] == 'D' && sectorBuffer[3] == '0' && sectorBuffer[4] == '0' && sectorBuffer[5] == '1';
}

BOOL SystemPartition_DetectATAPIISO9660SystemPartition(ATADevice * pDevice,
                                                       UINT32 * pDescriptorSector)
{
    UINT32 sector = 0x10;
    BYTE sectorBuffer[ATAPIDEVICE_IO_SECTOR_SIZE];
    while (ATAPIDevice_ReadSectors(pDevice, sector, 1, sectorBuffer) > 0)
    {
        ++sector;
        if (sectorBuffer[0] == 0xFF) break;
        if (sectorBuffer[0] != 0x01) continue;
        if (sectorBuffer[40] == 'S' &&
            sectorBuffer[41] == 'E' &&
            sectorBuffer[42] == 'M' &&
            sectorBuffer[43] == 'O' &&
            sectorBuffer[44] == 'S') { *pDescriptorSector = sector - 1; return TRUE; }
    }
    return FALSE;
}

void SystemPartition_Cache(SystemPartitionCache * pCache)
{
    PBYTE sectorBuffer = (PBYTE)malloc(ATAPIDEVICE_IO_SECTOR_SIZE);
    PBYTE sectorBuf = sectorBuffer;

    ATAPIDevice_ReadSectors(gSystemPartitionDevice, pCache->LBA, 1, sectorBuffer);

    BYTE entrySize = *sectorBuf;
    sectorBuf += entrySize;
    entrySize = *sectorBuf;
    sectorBuf += entrySize;

    BYTE parentIdentifierLength = strlen(pCache->Identifier);
    entrySize = *sectorBuf;
    while (entrySize != 0)
    {
        SystemPartitionCache * cache = (SystemPartitionCache *)malloc(sizeof(SystemPartitionCache));
        cache->EntrySize = entrySize;
        cache->LBA = *(UINT32 *)(sectorBuf + 2);
        cache->Length = *(UINT32 *)(sectorBuf + 10);
        cache->Flags = *(sectorBuf + 25);
        BYTE identifierLength = *(sectorBuf + 32);
        if (!(cache->Flags & 0x02)) identifierLength -= 2;
        cache->Identifier = (PSTRING)malloc(parentIdentifierLength + 1 + identifierLength + 1);
        strncpy(cache->Identifier, pCache->Identifier, parentIdentifierLength);
        cache->Identifier[parentIdentifierLength] = '/';
        strncpy(cache->Identifier + parentIdentifierLength + 1, (PSTRING)(sectorBuf + 33), identifierLength);
        cache->Identifier[parentIdentifierLength + 1 + identifierLength] = 0x00;
        cache->Entries.Head = NULL;
        cache->Entries.Tail = NULL;
        cache->Entries.Count = 0;

        List_Add(&pCache->Entries, cache);

        printf("%s, LBA = 0x%x, Length = 0x%x, Flags = 0x%x\n", cache->Identifier, cache->LBA, cache->Length, cache->Flags);

        if (cache->Flags & 0x02) SystemPartition_Cache(cache);

        sectorBuf += entrySize;
        entrySize = *sectorBuf;
    }
    free(sectorBuffer);
}
