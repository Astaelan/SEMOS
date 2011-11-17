extern "C" {
#include <types.h>
}
#include <Hardware/GDT.h>

#define GDTDESCRIPTOR_COUNT                         5

#define GDTDESCRIPTOR_ACCESS_ACCESSED               0x01
#define GDTDESCRIPTOR_ACCESS_READWRITE              0x02
#define GDTDESCRIPTOR_ACCESS_DIRECTIONCONFORMING    0x04
#define GDTDESCRIPTOR_ACCESS_EXECUTABLE             0x08
#define GDTDESCRIPTOR_ACCESS_ONE                    0x10
#define GDTDESCRIPTOR_ACCESS_RING0                  0x00
#define GDTDESCRIPTOR_ACCESS_RING1                  0x20
#define GDTDESCRIPTOR_ACCESS_RING2                  0x40
#define GDTDESCRIPTOR_ACCESS_RING3                  0x60
#define GDTDESCRIPTOR_ACCESS_PRESENT                0x80

#define GDTDESCRIPTOR_FLAGS_32BITSELECTOR           0x40
#define GDTDESCRIPTOR_FLAGS_4KBGRANULARITY          0x80

typedef struct
{
    UINT16 Limit;
    UINT32 BaseAddress;
} GDTRegister;

typedef struct
{
    UINT16 LimitLow;
    UINT16 BaseAddressLow;
    BYTE BaseAddressMid;
    BYTE Access;
    BYTE LimitHighAndFlags;
    BYTE BaseAddressHigh;
} GDTDescriptor;

GDTRegister gGDTRegister;
GDTDescriptor gGDTDescriptors[GDTDESCRIPTOR_COUNT];

void GDT_SetGate(BYTE pIndex, UINT32 pBaseAddress, UINT32 pLimit, BYTE pAccess, BYTE pFlags);

extern "C" {
extern void GDT_Update(GDTRegister * pRegister);
}

void GDT_Initialize()
{
    gGDTRegister.Limit = (sizeof(GDTDescriptor) * GDTDESCRIPTOR_COUNT) - 1;
    gGDTRegister.BaseAddress = (UINT32)gGDTDescriptors;

    GDT_SetGate(0, 0x00000000, 0x00000000, 0x00, 0x00);
    GDT_SetGate(1, 0x00000000, 0xFFFFFFFF, GDTDESCRIPTOR_ACCESS_READWRITE | GDTDESCRIPTOR_ACCESS_EXECUTABLE | GDTDESCRIPTOR_ACCESS_ONE | GDTDESCRIPTOR_ACCESS_PRESENT, GDTDESCRIPTOR_FLAGS_32BITSELECTOR | GDTDESCRIPTOR_FLAGS_4KBGRANULARITY);
    GDT_SetGate(2, 0x00000000, 0xFFFFFFFF, GDTDESCRIPTOR_ACCESS_READWRITE | GDTDESCRIPTOR_ACCESS_ONE | GDTDESCRIPTOR_ACCESS_PRESENT, GDTDESCRIPTOR_FLAGS_32BITSELECTOR | GDTDESCRIPTOR_FLAGS_4KBGRANULARITY);
    GDT_SetGate(3, 0x00000000, 0xFFFFFFFF, GDTDESCRIPTOR_ACCESS_READWRITE | GDTDESCRIPTOR_ACCESS_EXECUTABLE | GDTDESCRIPTOR_ACCESS_ONE | GDTDESCRIPTOR_ACCESS_RING3 | GDTDESCRIPTOR_ACCESS_PRESENT, GDTDESCRIPTOR_FLAGS_32BITSELECTOR | GDTDESCRIPTOR_FLAGS_4KBGRANULARITY);
    GDT_SetGate(4, 0x00000000, 0xFFFFFFFF, GDTDESCRIPTOR_ACCESS_READWRITE | GDTDESCRIPTOR_ACCESS_ONE | GDTDESCRIPTOR_ACCESS_RING3 | GDTDESCRIPTOR_ACCESS_PRESENT, GDTDESCRIPTOR_FLAGS_32BITSELECTOR | GDTDESCRIPTOR_FLAGS_4KBGRANULARITY);
    GDT_Update(&gGDTRegister);
}

void GDT_SetGate(BYTE pIndex,
                 UINT32 pBaseAddress,
                 UINT32 pLimit,
                 BYTE pAccess,
                 BYTE pFlags)
{
	gGDTDescriptors[pIndex].BaseAddressLow = pBaseAddress & 0xFFFF;
	gGDTDescriptors[pIndex].BaseAddressMid = (pBaseAddress >> 16) & 0xFF;
	gGDTDescriptors[pIndex].BaseAddressHigh = (pBaseAddress >> 24) & 0xFF;
	gGDTDescriptors[pIndex].LimitLow = pLimit & 0xFFFF;
	gGDTDescriptors[pIndex].LimitHighAndFlags = ((pLimit >> 16) & 0x0F) | (pFlags & 0xF0);
	gGDTDescriptors[pIndex].Access = pAccess;
}
