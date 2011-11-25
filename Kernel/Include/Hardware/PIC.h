#pragma once

namespace SEMOS
{
    namespace Hardware
    {
        class PIC
        {
        private:
            static const uint8_t MasterAddress = 0x20;
            static const uint8_t SlaveAddress = 0xA0;
            
            enum Register
            {
                CommandRegister = 0x00,
                DataRegister = 0x01
            };

            enum Command
            {
                ConfigModeCommand = 0x01,
                SinglePICCommand = 0x02,
                Interval4Command = 0x04,
                LevelModeCommand = 0x08,
                InitializeCommand = 0x10,
                ResetCommand = 0x20
            };

            enum Line
            {
                MasterToSlaveLine = 0x04,
                SlaveToMasterLine = 0x02
            };

            enum Mode
            {
                Microprocessor8086Mode = 0x01,
                AutoEndOfInterruptMode = 0x02,
                MasterMode = 0x04,
                BufferedMode = 0x08,
                SpecialFullyNestedMode = 0x10
            };

        public:
            static const uint8_t RemappedIRQOffset = 0x20;

            static void Initialize();
            static void ResetIRQ(bool pSlave);
        };
    }
}