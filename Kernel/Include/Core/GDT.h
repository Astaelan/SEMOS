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
                AccessedAccess = 0x01,
                ReadWriteAccess = 0x02,
                DirectionConformingAccess = 0x04,
                ExecutableAccess = 0x08,
                OneAccess = 0x10,

                Ring0Access = 0x00,
                Ring1Access = 0x20,
                Ring2Access = 0x40,
                Ring3Access = 0x60,
                PresentAccess = 0x80,

                ReadWriteOnePresentAccess = ReadWriteAccess | OneAccess | PresentAccess
            };

            enum EntryFlags
            {
                Selector32BitFlag = 0x40,
                Granularity4KBFlag = 0x80,

                Selector32BitGranularity4KBFlags = Selector32BitFlag | Granularity4KBFlag
            };

        private:
            static Entry sEntries[];

            static void SetGate(uint8_t pIndex, uint32_t pAddress, uint32_t pLimit, uint8_t pAccess, uint8_t pFlags);

        public:
            static void Initialize();
        };
    }
}