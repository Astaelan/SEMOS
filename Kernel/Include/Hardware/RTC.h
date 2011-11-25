#pragma once

namespace SEMOS
{
    namespace Hardware
    {
        class RTC
        {
        private:
            static const uint16_t BaseIOPort    = 0x0070;
            static const uint16_t AddressIOPort = BaseIOPort + 0x00;
            static const uint16_t DataIOPort    = BaseIOPort + 0x01;

            static void WaitForReady();
            static uint8_t GetSecond();
            static uint8_t GetMinute();
            static uint8_t GetHour();
            static uint8_t GetDayOfWeek();
            static uint8_t GetDayOfMonth();
            static uint8_t GetMonth();
            static uint8_t GetYear();
            static uint8_t GetCentury();
            static uint32_t GetTotalDays(uint16_t pYear, uint8_t pMonth, uint8_t pDay);

            static bool sBinaryCodedDecimalMode;
            static bool sMilitaryMode;
            static uint8_t sStatus;

        public:
            static void Initialize();
            static uint32_t GetSecondsSinceEpoch();
        };
    }
}
