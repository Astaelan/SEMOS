#include <MBoot.h>

#define MBOOT_MAGIC									0x2BADB002

#define MBOOT_FLAGS_MEMORY							(1 << 0)
#define MBOOT_FLAGS_BOOT_DEVICE						(1 << 1)
#define MBOOT_FLAGS_CMDLINE							(1 << 2)
#define MBOOT_FLAGS_MODULES							(1 << 3)
#define MBOOT_FLAGS_AOUT							(1 << 4)
#define MBOOT_FLAGS_ELF								(1 << 5)
#define MBOOT_FLAGS_MMAP							(1 << 6)
#define MBOOT_FLAGS_DRIVES							(1 << 7)
#define MBOOT_FLAGS_CONFIG_TABLE					(1 << 8)
#define MBOOT_FLAGS_BOOT_LOADER_NAME				(1 << 9)
#define MBOOT_FLAGS_APM_TABLE						(1 << 10)
#define MBOOT_FLAGS_VBE								(1 << 11)

#define MBOOT_MMAP_TYPE_AVAILABLE					1

#define MBOOT_MMAP_AVAILABLE_MAX					16

#define MBOOT_MMAP_CONVENTIONAL						(1024 * 640)


MBoot * gMBoot = NULL;
MBootMMapAvailable gMBootMMapAvailable[MBOOT_MMAP_AVAILABLE_MAX];
BYTE gMBootMMapAvailableCount = 0;

BOOL MBoot_Initialize(UINT32 pMBootMagic,
                      PVOID pMBoot)
{
	if (pMBootMagic != MBOOT_MAGIC) return FALSE;
	gMBoot = (MBoot *)pMBoot;

    extern CHAR __BOF;
    extern CHAR __EOF;
	UINT32 kernelHead = (UINT32)&__BOF;
	UINT32 kernelTail = (UINT32)&__EOF;
	UINT32 kernelSize = kernelTail - kernelHead;
	MBootMMap * mmapBase = (MBootMMap *)gMBoot->mmap_addr;
    UINT32 mmapSize = mmapBase->size + 4;
    UINT32 mmapCount = gMBoot->mmap_length / mmapSize;

	for (UINT32 mmapIndex = 0; mmapIndex < mmapCount; ++mmapIndex, mmapBase = (MBootMMap *)(gMBoot->mmap_addr + (mmapIndex * mmapSize)))
	{
		if (mmapBase->type == MBOOT_MMAP_TYPE_AVAILABLE)
		{
			UINT32 address = mmapBase->addr0;
			UINT32 length = mmapBase->length0;
			if (address + length <= MBOOT_MMAP_CONVENTIONAL) continue;
			if (kernelHead >= address && kernelTail <= (address + length))
			{
				address += kernelSize;
				length -= kernelSize;
			}
			gMBootMMapAvailable[gMBootMMapAvailableCount].Address = address;
			gMBootMMapAvailable[gMBootMMapAvailableCount].Length = length;
			gMBootMMapAvailable[gMBootMMapAvailableCount].Used = 0;
			++gMBootMMapAvailableCount;
		}
	}
	return TRUE;
}
