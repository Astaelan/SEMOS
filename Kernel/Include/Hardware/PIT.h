#pragma once

namespace SEMOS
{
    namespace Hardware
    {
        class PIT
        {
        private:
            static const uint16_t BaseIOPort = 0x0040;
            static const uint16_t Counter0IOPort = BaseIOPort + 0x00;
            static const uint16_t Counter1IOPort = BaseIOPort + 0x01;
            static const uint16_t Counter2IOPort = BaseIOPort + 0x02;
            static const uint16_t CommandIOPort  = BaseIOPort + 0x03;
            static const uint16_t BaseIRQ = 0x00;

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
