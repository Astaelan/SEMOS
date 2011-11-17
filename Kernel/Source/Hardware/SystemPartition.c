extern "C" {
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
}
#include <PortIO.h>
#include <FileSystem.h>
#include <Hardware/SystemPartition.h>
#include <Hardware/IDT.h>
#include <Hardware/VGAText.h>
#include <Hardware/ATA/ATADevice.h>
#include <Hardware/ATA/ATAPIDevice.h>
#include <Utility/List.h>

#undef errno
extern int  errno;

char * strdup(const char *);
int	strcasecmp(const char *, const char *);

typedef struct
{
    BYTE EntrySize;
    UINT32 LBA;
    UINT32 Length;
    BYTE Flags;
    char * Identifier;
    List Entries;
} SystemPartitionCache;

BOOL SystemPartition_Detect();
BOOL SystemPartition_DetectATA(UINT16 pBaseAddress);
BOOL SystemPartition_DetectATAPIISO9660(ATADevice * pDevice);
BOOL SystemPartition_DetectATAPIISO9660SystemPartition(ATADevice * pDevice, UINT32 * pDescriptorSector);
void SystemPartition_Cache(SystemPartitionCache * pCache);

INT32 SystemPartition_OpenHandler(FileDescriptor * pDescriptor, const char * pPath, INT32 pFlags, mode_t pMode);
INT32 SystemPartition_CloseHandler(FileDescriptor * pDescriptor);
off_t SystemPartition_LSeekHandler(FileDescriptor * pDescriptor, off_t pOffset, INT32 pWhence);
INT32 SystemPartition_WriteHandler(FileDescriptor * pDescriptor, const void * pData, size_t pLength);
INT32 SystemPartition_ReadHandler(FileDescriptor * pDescriptor, void * pData, size_t pLength);


ATADevice * gSystemPartitionDevice = NULL;
UINT32 gSystemPartitionDescriptorSector = 0;
SystemPartitionCache gSystemPartitionCache;

void SystemPartition_Initialize()
{
    if(!SystemPartition_Detect()) return;

    const char * rootMount = "/SYSTEM";
    UINT32 rootMountLength = strlen(rootMount);

    PBYTE sectorBuffer = (PBYTE)malloc(ATAPIDEVICE_IO_SECTOR_SIZE);
    ATAPIDevice_ReadSector(gSystemPartitionDevice, gSystemPartitionDescriptorSector, sectorBuffer);

    gSystemPartitionCache.EntrySize = *(sectorBuffer + 156);
    gSystemPartitionCache.LBA = *(UINT32 *)(sectorBuffer + 156 + 2);
    gSystemPartitionCache.Length = *(UINT32 *)(sectorBuffer + 156 + 10);
    gSystemPartitionCache.Flags = *(sectorBuffer + 156 + 25);
    gSystemPartitionCache.Identifier = (char *)malloc(rootMountLength + 1);
    strncpy(gSystemPartitionCache.Identifier, rootMount, rootMountLength);
    gSystemPartitionCache.Identifier[rootMountLength] = 0x00;
    free(sectorBuffer);

    SystemPartition_Cache(&gSystemPartitionCache);

    FileSystem_Register(rootMount, &SystemPartition_OpenHandler);
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
    UINT32 bytesRead = ATAPIDevice_ReadSector(pDevice, 0x10, sectorBuffer);
    if (bytesRead < 6) return FALSE;
    return sectorBuffer[1] == 'C' && sectorBuffer[2] == 'D' && sectorBuffer[3] == '0' && sectorBuffer[4] == '0' && sectorBuffer[5] == '1';
}

BOOL SystemPartition_DetectATAPIISO9660SystemPartition(ATADevice * pDevice,
                                                       UINT32 * pDescriptorSector)
{
    UINT32 sector = 0x10;
    BYTE sectorBuffer[ATAPIDEVICE_IO_SECTOR_SIZE];
    while (ATAPIDevice_ReadSector(pDevice, sector, sectorBuffer) > 0)
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

    ATAPIDevice_ReadSector(gSystemPartitionDevice, pCache->LBA, sectorBuffer);

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
        cache->Identifier = (char *)malloc(parentIdentifierLength + 1 + identifierLength + 1);
        strncpy(cache->Identifier, pCache->Identifier, parentIdentifierLength);
        cache->Identifier[parentIdentifierLength] = '/';
        strncpy(cache->Identifier + parentIdentifierLength + 1, (const char *)(sectorBuf + 33), identifierLength);
        cache->Identifier[parentIdentifierLength + 1 + identifierLength] = 0x00;
        cache->Entries.Head = NULL;
        cache->Entries.Tail = NULL;
        cache->Entries.Count = 0;

        List_Add(&pCache->Entries, cache);

        //printf("%s, LBA = 0x%x, Length = 0x%x, Flags = 0x%x\n", cache->Identifier, cache->LBA, cache->Length, cache->Flags);

        if (cache->Flags & 0x02) SystemPartition_Cache(cache);

        sectorBuf += entrySize;
        entrySize = *sectorBuf;
    }
    free(sectorBuffer);
}

SystemPartitionCache * SystemPartition_FindCached(SystemPartitionCache * pCache, const char * pPath)
{
    if (!strcasecmp(pCache->Identifier, pPath)) return pCache;
    for (Node * node = pCache->Entries.Head; node; node = node->Next)
    {
        SystemPartitionCache * cache = (SystemPartitionCache *)node->Data;
        cache = SystemPartition_FindCached(cache, pPath);
        if (cache) return cache;
    }
    return NULL;
}

INT32 SystemPartition_OpenHandler(FileDescriptor * pDescriptor, const char * pPath, INT32 pFlags, mode_t pMode)
{
    SystemPartitionCache * cache = SystemPartition_FindCached(&gSystemPartitionCache, pPath);
    if (!cache)
    {
        errno = EACCES;
        return -1;
    }
    printf("Opening %s, %d bytes...\n", cache->Identifier, cache->Length);
    if (pFlags && pMode) { }
    pDescriptor->Active = TRUE;
    pDescriptor->BlockStart = cache->LBA;
    pDescriptor->Mode = S_IFBLK | (S_IREAD >> 3) | (S_IREAD >> 6);
    pDescriptor->TotalSize = cache->Length;
    pDescriptor->BlockSize = ATAPIDEVICE_IO_SECTOR_SIZE;
    pDescriptor->BlockCount = pDescriptor->TotalSize / pDescriptor->BlockSize;
    if ((pDescriptor->TotalSize % pDescriptor->BlockSize) != 0) ++pDescriptor->BlockCount;
    pDescriptor->Path = strdup(cache->Identifier);
    pDescriptor->CloseHandler = &SystemPartition_CloseHandler;
    pDescriptor->LSeekHandler = &SystemPartition_LSeekHandler;
    pDescriptor->WriteHandler = &SystemPartition_WriteHandler;
    pDescriptor->ReadHandler = &SystemPartition_ReadHandler;
    return pDescriptor->Index;
}

INT32 SystemPartition_CloseHandler(FileDescriptor * pDescriptor)
{
    pDescriptor->Active = FALSE;
    pDescriptor->Device = 0;
    pDescriptor->BlockStart = 0;
    pDescriptor->Mode = 0;
    pDescriptor->TotalSize = 0;
    pDescriptor->BlockSize = 0;
    pDescriptor->BlockCount = 0;
    pDescriptor->TerminalStream = FALSE;
    pDescriptor->Offset = 0;
    free(pDescriptor->Path);
    pDescriptor->Path = NULL;
    pDescriptor->CloseHandler = NULL;
    pDescriptor->LSeekHandler = NULL;
    pDescriptor->WriteHandler = NULL;
    pDescriptor->ReadHandler = NULL;
    return 0;
}

off_t SystemPartition_LSeekHandler(FileDescriptor * pDescriptor, off_t pOffset, INT32 pWhence)
{
    off_t tempOffset = pDescriptor->Offset;
    switch (pWhence)
    {
    case SEEK_SET: tempOffset = pOffset; break;
    case SEEK_CUR: tempOffset += pOffset; break;
    case SEEK_END: tempOffset = pDescriptor->TotalSize; break;
    default:
        errno = EINVAL;
        return -1;
    }
    if (tempOffset <= 0) pDescriptor->Offset = 0;
    else if ((UINT32)tempOffset >= pDescriptor->TotalSize) pDescriptor->Offset = pDescriptor->TotalSize;
    else pDescriptor->Offset = tempOffset;
    //printf("SystemPartition LSeekHandler: %i, %i = %u\n", (INT32)pOffset, pWhence, pDescriptor->Offset);
    return (off_t)pDescriptor->Offset;
}

INT32 SystemPartition_WriteHandler(FileDescriptor * pDescriptor, const void * pData, size_t pLength)
{
    if (pDescriptor && pData && pLength) { }
    errno = EBADF;
    return -1;
}

INT32 SystemPartition_ReadHandler(FileDescriptor * pDescriptor, void * pData, size_t pLength)
{
    PBYTE sectorBuffer = (PBYTE)malloc(ATAPIDEVICE_IO_SECTOR_SIZE);
    UINT32 dataOffset = 0;
    UINT32 dataRemaining = pLength;
    UINT32 currentBlock = pDescriptor->BlockStart + (pDescriptor->Offset / ATAPIDEVICE_IO_SECTOR_SIZE);
    while (pDescriptor->Offset < pDescriptor->TotalSize && dataRemaining > 0)
    {
        UINT32 sectorLength = ATAPIDevice_ReadSector(gSystemPartitionDevice, currentBlock, sectorBuffer);
        UINT32 sectorOffset = pDescriptor->Offset % ATAPIDEVICE_IO_SECTOR_SIZE;
        UINT32 sectorUsable = sectorLength - sectorOffset;
        UINT32 sectorConsumed = dataRemaining;
        if (sectorConsumed > sectorUsable) sectorConsumed = sectorUsable;
        if ((pDescriptor->Offset + sectorConsumed) > pDescriptor->TotalSize) sectorConsumed = pDescriptor->TotalSize - pDescriptor->Offset;
        memcpy((PBYTE)pData + dataOffset, sectorBuffer + sectorOffset, sectorConsumed);
        pDescriptor->Offset += sectorConsumed;
        dataOffset += sectorConsumed;
        dataRemaining -= sectorConsumed;
        if (pDescriptor->Offset < pDescriptor->TotalSize && dataRemaining > 0) ++currentBlock;
    }
    free(sectorBuffer);
    return dataOffset;
}
