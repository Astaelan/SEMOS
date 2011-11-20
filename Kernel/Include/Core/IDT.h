#pragma once

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
                PriviledgeLevelRing0 = 0x0000,
                PriviledgeLevelRing1 = 0x0001,
                PriviledgeLevelRing2 = 0x0002,
                PriviledgeLevelRing3 = 0x0003,
                TableIndex = 0x0004,
                DescriptorIndex = 0x0008
            };

            enum Type
            {
                Task386Gate32Bit = 0x05,
                Interrupt286Gate16Bit = 0x06,
                Trap286Gate16Bit = 0x07,
                Interrupt386Gate32Bit = 0x0E,
                Trap386Gate32Bit = 0x0F,
                Present = 0x80
            };

            static Entry sEntries[];
            static bool sScheduled[];
            static IDTHandler sHandlers[];

            static void SetGate(uint8_t pIndex,
                                uint32_t pAddress,
                                uint16_t pSelector,
                                uint8_t pTypeAndFlags);
            static void InitializeGate(uint8_t pIndex,
                                       uint32_t pAddress) { SetGate(pIndex, pAddress, Selector::DescriptorIndex, Type::Interrupt386Gate32Bit | Type::Present); }

        public:

            static void Initialize();
            static void Schedule(uint8_t pInterrupt) { sScheduled[pInterrupt] = true; }
            static void Unschedule(uint8_t pInterrupt) { sScheduled[pInterrupt] = false; }
            static void WaitFor(uint8_t pInterrupt);
            static void RegisterHandler(uint8_t pInterrupt, IDTHandler pHandler) { sHandlers[pInterrupt] = pHandler; }
            static IDTHandler GetHandler(uint8_t pInterrupt) { return sHandlers[pInterrupt]; }
        };
    }
}
