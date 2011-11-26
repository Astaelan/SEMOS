#pragma once

#include <array>

namespace SEMOS
{
    namespace Core
    {
        class IDT
        {
        public:
            static const uint16_t MaxEntries = 256;

            static struct Register
            {
                uint16_t Limit;
                uint32_t Address;
            } sRegister;

            struct Entry
            {
                uint16_t AddressLow;
                uint16_t Selector;
                uint8_t Zero;
                uint8_t TypeAndFlags;
                uint16_t AddressHigh;
            };

            struct Registers
            {
               uint32_t ds;
               uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
               uint32_t int_no, err_code;
               uint32_t eip, cs, eflags, useresp, ss;
            };

            typedef void (*IDTHandler)(Registers pRegisters);

        private:
            enum Selector
            {
                PriviledgeLevelRing0Selector = 0x0000,
                PriviledgeLevelRing1Selector = 0x0001,
                PriviledgeLevelRing2Selector = 0x0002,
                PriviledgeLevelRing3Selector = 0x0003,
                TableIndexSelector = 0x0004,
                DescriptorIndexSelector = 0x0008
            };

            enum Type
            {
                Task386Gate32BitType = 0x05,
                Interrupt286Gate16BitType = 0x06,
                Trap286Gate16BitType = 0x07,
                Interrupt386Gate32BitType = 0x0E,
                Trap386Gate32BitType = 0x0F,
                PresentType = 0x80
            };

            typedef std::array<Entry, MaxEntries> EntryArray;
            typedef std::array<bool, MaxEntries> ScheduledArray;
            typedef std::array<IDTHandler, MaxEntries> HandlersArray;

            static EntryArray sEntries;
            static ScheduledArray sScheduled;
            static HandlersArray sHandlers;

            static void SetGate(uint8_t pIndex,
                                uint32_t pAddress,
                                uint16_t pSelector,
                                uint8_t pTypeAndFlags);
            static void InitializeGate(uint8_t pIndex,
                                       uint32_t pAddress) { SetGate(pIndex, pAddress, Selector::DescriptorIndexSelector, Type::Interrupt386Gate32BitType | Type::PresentType); }

        public:

            static void Initialize();
            static void Schedule(uint8_t pInterrupt);
            static void Unschedule(uint8_t pInterrupt);
            static void WaitFor(uint8_t pInterrupt);
            static void RegisterHandler(uint8_t pInterrupt, IDTHandler pHandler);
            static IDTHandler GetHandler(uint8_t pInterrupt);
        };
    }
}
