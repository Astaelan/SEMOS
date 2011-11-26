#include <Core/IDT.h>
#include <Hardware/PIC.h>

#include <PortIO.h>

using namespace SEMOS;
using namespace SEMOS::Core;
using namespace SEMOS::Hardware;

extern "C" {
extern void IDTUpdate(IDT::Register * pRegister);
#include <Core/IDTExternalStubs.h> // Must remain here
void IDTISRHandler(IDT::Registers pRegisters);
void IDTIRQHandler(IDT::Registers pRegisters);
}

IDT::Register IDT::sRegister;
IDT::EntryArray IDT::sEntries;
IDT::ScheduledArray IDT::sScheduled;
IDT::HandlersArray IDT::sHandlers;

void IDT::Initialize()
{
    sEntries.fill(Entry());
    sScheduled.fill(false);
    sScheduled.fill(nullptr);

	sRegister.Limit = (sizeof(Entry) * MaxEntries) - 1;
	sRegister.Address = (uint32_t)sEntries.data();

#include <Core/IDTInitializeStubs.h> // Must remain here

	IDTUpdate(&sRegister);
}

void IDT::Schedule(uint8_t pInterrupt)
{ sScheduled[pInterrupt] = true; }

void IDT::Unschedule(uint8_t pInterrupt)
{ sScheduled[pInterrupt] = false; }

void IDT::WaitFor(uint8_t pInterrupt)
{ while (sScheduled[pInterrupt]) IOWAIT(); }

void IDT::RegisterHandler(uint8_t pInterrupt, IDTHandler pHandler)
{ sHandlers[pInterrupt] = pHandler; }

IDT::IDTHandler IDT::GetHandler(uint8_t pInterrupt)
{ return sHandlers[pInterrupt]; }

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
    PIC::ResetIRQ(pRegisters.int_no >= 9);
    IDT::Unschedule(PIC::RemappedIRQOffset + pRegisters.int_no);
    IDT::IDTHandler handler = IDT::GetHandler(PIC::RemappedIRQOffset + pRegisters.int_no);
    if (handler) handler(pRegisters);
}
