#pragma once

namespace SEMOS
{
    namespace Core
    {
        class GDT
        {
        public:
            static const uint8_t MaxEntries = 5;

            static struct Register
            {
                uint16_t Limit;
                uint32_t Address;
            } sRegister;

            struct Entry
            {
                uint16_t LimitLow;
                uint16_t AddressLow;
                uint8_t AddressMid;
                uint8_t Access;
                uint8_t LimitHighAndFlags;
                uint8_t AddressHigh;
            };

            enum EntryAccess
            {
                Accessed = 0x01,
                ReadWrite = 0x02,
                DirectionConforming = 0x04,
                Executable = 0x08,
                One = 0x10,

                Ring0 = 0x00,
                Ring1 = 0x20,
                Ring2 = 0x40,
                Ring3 = 0x60,
                Present = 0x80,

                ReadWriteOnePresent = ReadWrite | One | Present
            };

            enum EntryFlags
            {
                Selector32Bit = 0x40,
                Granularity4KB = 0x80,

                Selector32BitGranularity4KB = Selector32Bit | Granularity4KB
            };

        private:
            static Entry sEntries[];

            static void SetGate(uint8_t pIndex, uint32_t pAddress, uint32_t pLimit, uint8_t pAccess, uint8_t pFlags);

        public:
            static void Initialize();
        };
    }
}