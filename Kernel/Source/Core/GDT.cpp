#include <Core/GDT.h>

using namespace SEMOS;
using namespace SEMOS::Core;

extern "C" {
extern void GDTUpdate(GDT::Register * pRegister);
}

GDT::Register GDT::sRegister;
GDT::EntryArray GDT::sEntries;

void GDT::SetGate(uint8_t pIndex, uint32_t pAddress, uint32_t pLimit, uint8_t pAccess, uint8_t pFlags)
{
    sEntries[pIndex].AddressLow = pAddress & 0xFFFF;
    sEntries[pIndex].AddressMid = (pAddress >> 16) & 0xFF;
    sEntries[pIndex].AddressHigh = (pAddress >> 24) & 0xFF;
    sEntries[pIndex].LimitLow = pLimit & 0xFFFF;
    sEntries[pIndex].LimitHighAndFlags = ((pLimit >> 16) & 0x0F) | (pFlags & 0xF0);
    sEntries[pIndex].Access = pAccess;
}

void GDT::Initialize()
{
    sEntries.fill(Entry());

    sRegister.Limit = (sizeof(Entry) * MaxEntries) - 1;
    sRegister.Address = (uint32_t)sEntries.data();

    SetGate(0, 0x00000000, 0x00000000, 0x00, 0x00);
    SetGate(1, 0x00000000, 0xFFFFFFFF, EntryAccess::ReadWriteOnePresentAccess | EntryAccess::ExecutableAccess, EntryFlags::Selector32BitGranularity4KBFlags);
    SetGate(2, 0x00000000, 0xFFFFFFFF, EntryAccess::ReadWriteOnePresentAccess, EntryFlags::Selector32BitGranularity4KBFlags);
    SetGate(3, 0x00000000, 0xFFFFFFFF, EntryAccess::ReadWriteOnePresentAccess | EntryAccess::ExecutableAccess | EntryAccess::Ring3Access, EntryFlags::Selector32BitGranularity4KBFlags);
    SetGate(4, 0x00000000, 0xFFFFFFFF, EntryAccess::ReadWriteOnePresentAccess | EntryAccess::Ring3Access, EntryFlags::Selector32BitGranularity4KBFlags);

    GDTUpdate(&sRegister);
}

