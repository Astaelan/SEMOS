#pragma once

namespace SEMOS
{
    namespace Hardware
    {
        class PIT
        {
        private:
            static const uint16_t BaseAddress = 0x0040;
            static const uint16_t BaseIRQ = 0x00;

            enum Register
            {
                Counter0Register = 0x00,
                Counter1Register = 0x01,
                Counter2Register = 0x02,
                CommandRegister = 0x03
            };

            enum Command
            {
                BinaryCodedDecimalCommand = 0x01,

                Mode0Command = 0x00,
                Mode1Command = 0x02,
                Mode2Command = 0x04,
                Mode3Command = 0x06,
                Mode4Command = 0x08,
                Mode5Command = 0x0A,

                ReadWriteLatchCommand = 0x00,
                ReadWriteLSBOnlyCommand = 0x10,
                ReadWriteMSBOnlyCommand = 0x20,
                ReadWriteLSBThenMSBCommand = 0x30,

                SelectCounter0Command = 0x00,
                SelectCounter1Command = 0x40,
                SelectCounter2Command = 0x80,
                SelectReadBackCommand = 0xC0,

                Counter0InitializeCommand = Mode3Command | ReadWriteLSBThenMSBCommand | SelectCounter0Command
            };

        public:
            static const uint32_t MaxFrequency = 1193180;

            static void Initialize(uint32_t pFrequency);
        };
    }
}
