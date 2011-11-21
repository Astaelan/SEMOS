#pragma once

namespace SEMOS
{
    namespace Core
    {
        class COMPortLogger
        {
        private:
            static const uint16_t BaseAddress = 0x02F8;

            enum Register
            {
                DataRegister = 0x00,
                InterruptRegister = 0x01,
                FIFORegister = 0x02,
                LineDataRegister = 0x03,
                ModemDataRegister = 0x04,
                LineStatusRegister = 0x05,
                ModemStatusRegister = 0x06,
                ScratchRegister = 0x07
            };

            static const uint32_t WriteAttempts = 1000;

            static bool IsTransmitEmpty();

        public:
            static void Initialize();
            static void WriteByte(uint8_t pByte);
            static void WriteString(const char * pString);
            static void WriteLine(const char * pLine);
        };
    }
}
