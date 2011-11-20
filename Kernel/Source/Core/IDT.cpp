#include <Core/IDT.h>
#include <Hardware/PIC.h>

#include <PortIO.h>

using namespace SEMOS;
using namespace SEMOS::Core;

extern "C" {
extern void IDTUpdate(IDT::Register * pRegister);
#include <Core/IDTExternalStubs.h> // Must remain here
void IDTISRHandler(IDT::Registers pRegisters);
void IDTIRQHandler(IDT::Registers pRegisters);
}

IDT::Register IDT::sRegister;
IDT::Entry IDT::sEntries[MaxEntries];
bool IDT::sScheduled[MaxEntries];
IDT::IDTHandler IDT::sHandlers[MaxEntries];

void IDT::Initialize()
{
	sRegister.Limit = (sizeof(Entry) * MaxEntries) - 1;
	sRegister.Address = (uint32_t)sEntries;

#include <Core/IDTInitializeStubs.h> // Must remain here

	IDTUpdate(&sRegister);
}

void IDT::WaitFor(uint8_t pInterrupt)
{ while (sScheduled[pInterrupt]) IOWAIT(); }

void IDT::SetGate(uint8_t pIndex,
                  uint32_t pAddress,
                  uint16_t pSelector,
                  uint8_t pTypeAndFlags)
{
    sEntries[pIndex].AddressLow = pAddress & 0xFFFF;
    sEntries[pIndex].AddressHigh = (pAddress >> 16) & 0xFFFF;
    sEntries[pIndex].Selector = pSelector;
    sEntries[pIndex].Zero = 0;
    sEntries[pIndex].TypeAndFlags = pTypeAndFlags;
}

void IDTISRHandler(IDT::Registers pRegisters)
{
    IDT::Unschedule(pRegisters.int_no);
    IDT::IDTHandler handler = IDT::GetHandler(pRegisters.int_no);
    if (handler) handler(pRegisters);
}

void IDTIRQHandler(IDT::Registers pRegisters)
{
    if (pRegisters.int_no >= 9) outb(PIC_IO_SLAVE_BASE + PIC_IO_COMMAND, PIC_IO_COMMAND_RESET);
    outb(PIC_IO_MASTER_BASE + PIC_IO_COMMAND, PIC_IO_COMMAND_RESET);

    IDT::Unschedule(PIC_IRQ_MASTER_BASE + pRegisters.int_no);
    IDT::IDTHandler handler = IDT::GetHandler(PIC_IRQ_MASTER_BASE + pRegisters.int_no);
    if (handler) handler(pRegisters);
}
