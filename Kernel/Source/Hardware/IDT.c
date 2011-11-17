extern "C" {
#include <types.h>
}
#include <PortIO.h>
#include <Hardware/IDT.h>
#include <Hardware/PIC.h>

#define IDTDESCRIPTOR_COUNT                         256

#define IDTDESCRIPTOR_SELECTOR_RPLRING0             0x00
#define IDTDESCRIPTOR_SELECTOR_RPLRING1             0x01
#define IDTDESCRIPTOR_SELECTOR_RPLRING2             0x02
#define IDTDESCRIPTOR_SELECTOR_RPLRING3             0x03
#define IDTDESCRIPTOR_SELECTOR_TABLEINDEX           0x04
#define IDTDESCRIPTOR_SELECTOR_DESCRIPTORINDEX      0x08

#define IDTDESCRIPTOR_TYPE_386TASKGATE32BIT         0x05
#define IDTDESCRIPTOR_TYPE_286INTERRUPTGATE16BIT    0x06
#define IDTDESCRIPTOR_TYPE_286TRAPGATE16BIT         0x07
#define IDTDESCRIPTOR_TYPE_386INTERRUPTGATE32BIT    0x0E
#define IDTDESCRIPTOR_TYPE_386TRAPGATE32BIT         0x0F
#define IDTDESCRIPTOR_TYPE_PRESENT                  0x80

typedef struct
{
    UINT16 Limit;
    UINT32 BaseAddress;
} IDTRegister;

typedef struct
{
    UINT16 BaseAddressLow;
    UINT16 Selector;
    BYTE Zero;
    BYTE TypeAndFlags;
    UINT16 BaseAddressHigh;
} IDTDescriptor;

IDTRegister gIDTRegister;
IDTDescriptor gIDTDescriptors[IDTDESCRIPTOR_COUNT];
BOOL gIDTWaiting[IDTDESCRIPTOR_COUNT];
IDTHandler gIDTHandlers[IDTDESCRIPTOR_COUNT];

void IDT_SetGate(BYTE pIndex, UINT32 pBaseAddress, UINT16 pSelector, BYTE pTypeAndFlags);

extern "C" {
extern void IDT_Update(IDTRegister* pRegister);
#include <Hardware/IDTExternalStubs.h> // Must remain here
void IDT_ISR_Handler(Registers pRegisters);
void IDT_IRQ_Handler(Registers pRegisters);
}

void IDT_InitGate(BYTE pIndex,
                  UINT32 pBaseAddress)
{ IDT_SetGate(pIndex, pBaseAddress, IDTDESCRIPTOR_SELECTOR_DESCRIPTORINDEX, IDTDESCRIPTOR_TYPE_386INTERRUPTGATE32BIT | IDTDESCRIPTOR_TYPE_PRESENT); }

void IDT_Initialize()
{
	gIDTRegister.Limit = (sizeof(IDTDescriptor) * IDTDESCRIPTOR_COUNT) - 1;
	gIDTRegister.BaseAddress = (UINT32)gIDTDescriptors;

#include <Hardware/IDTInitializeStubs.h> // Must remain here

	IDT_Update(&gIDTRegister);
}

void IDT_ScheduleInterrupt(UINT8 pInterrupt)
{ gIDTWaiting[pInterrupt] = TRUE; }

void IDT_WaitForInterrupt(UINT8 pInterrupt)
{ while (gIDTWaiting[pInterrupt]) IOWAIT(); }

void IDT_RegisterHandler(UINT8 pInterrupt,
                         IDTHandler pHandler)
{ gIDTHandlers[pInterrupt] = pHandler; }

void IDT_SetGate(BYTE pIndex,
                 UINT32 pBaseAddress,
                 UINT16 pSelector,
                 BYTE pTypeAndFlags)
{
    gIDTDescriptors[pIndex].BaseAddressLow = pBaseAddress & 0xFFFF;
    gIDTDescriptors[pIndex].BaseAddressHigh = (pBaseAddress >> 16) & 0xFFFF;
    gIDTDescriptors[pIndex].Selector = pSelector;
    gIDTDescriptors[pIndex].Zero = 0;
    gIDTDescriptors[pIndex].TypeAndFlags = pTypeAndFlags;
}

void IDT_ISR_Handler(Registers pRegisters)
{
    gIDTWaiting[pRegisters.int_no] = FALSE;
    if (gIDTHandlers[pRegisters.int_no] != NULL) gIDTHandlers[pRegisters.int_no](pRegisters);
}

void IDT_IRQ_Handler(Registers pRegisters)
{
    if (pRegisters.int_no >= 9) outb(PIC_IO_SLAVE_BASE + PIC_IO_COMMAND, PIC_IO_COMMAND_RESET);
    outb(PIC_IO_MASTER_BASE + PIC_IO_COMMAND, PIC_IO_COMMAND_RESET);

    gIDTWaiting[PIC_IRQ_MASTER_BASE + pRegisters.int_no] = FALSE;
    if (gIDTHandlers[PIC_IRQ_MASTER_BASE + pRegisters.int_no] != NULL) gIDTHandlers[PIC_IRQ_MASTER_BASE + pRegisters.int_no](pRegisters);
}
