#ifndef _MBOOT_H
#define _MBOOT_H

extern "C" {
#include <types.h>
}

typedef struct
{
	UINT32 flags;
	UINT32 mem_lower;
	UINT32 mem_upper;
	UINT32 boot_device;
	UINT32 cmdline;
	UINT32 mods_count;
	UINT32 mods_addr;
	UINT32 sym_0;
	UINT32 sym_1;
	UINT32 sym_2;
	UINT32 sym_3;
	UINT32 mmap_length;
	UINT32 mmap_addr;
	UINT32 drives_length;
	UINT32 drives_addr;
	UINT32 config_table;
	UINT32 boot_loader_name;
	UINT32 apm_table;
	UINT32 vbe_control_info;
	UINT32 vbe_mode_info;
	UINT16 vbe_mode;
	UINT16 vbe_interface_seg;
	UINT16 vbe_interface_off;
	UINT16 vbe_interface_len;
} MBoot;

typedef struct
{
	UINT32 size;
	UINT32 addr0;
	UINT32 addr1;
	UINT32 length0;
	UINT32 length1;
	UINT32 type;
} MBootMMap;

typedef struct
{
	UINT32 Address;
	UINT32 Length;
	UINT32 Used;
} MBootMMapAvailable;

BOOL MBoot_Initialize(UINT32 pMBootMagic, PVOID pMBoot);

extern MBoot * gMBoot;
extern MBootMMapAvailable gMBootMMapAvailable[];
extern BYTE gMBootMMapAvailableCount;

#endif