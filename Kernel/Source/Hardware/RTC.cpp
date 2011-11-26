#include <Hardware/RTC.h>

#include <PortIO.h>

using namespace SEMOS;
using namespace SEMOS::Hardware;

bool RTC::sBinaryCodedDecimalMode = false;
bool RTC::sMilitaryMode = false;

inline void RTC::WaitForReady()
{ do { outb(AddressIOPort, 10); } while ((inb(DataIOPort) & 0x80) != 0); }

void RTC::Initialize()
{
    WaitForReady();
    outb(AddressIOPort, 0x0B);
    uint8_t status = inb(DataIOPort);
    if ((status & 0x02) == 0x02) sMilitaryMode = true;
    else sMilitaryMode = false;
    if ((status & 0x04) == 0x04) sBinaryCodedDecimalMode = false;
    else sBinaryCodedDecimalMode = true;
}

uint8_t RTC::GetSecond()
{
    WaitForReady();
    outb(AddressIOPort, 0x00);
    if (sBinaryCodedDecimalMode) return FromBCD(inb(DataIOPort));
    return inb(DataIOPort);
}

uint8_t RTC::GetMinute()
{
    WaitForReady();
    outb(AddressIOPort, 0x02);
    if (sBinaryCodedDecimalMode) return FromBCD(inb(DataIOPort));
    return inb(DataIOPort);
}

uint8_t RTC::GetHour()
{
    WaitForReady();
    outb(AddressIOPort, 0x04);
    if (sBinaryCodedDecimalMode)
    {
        if (sMilitaryMode) return FromBCD(inb(DataIOPort));
        uint8_t b = inb(DataIOPort);
        if ((b & 0x80) == 0x80) return FromBCD(b) + 12;
        if (FromBCD(b) == 12) return 0;
        return FromBCD(b);
    }
    else
    {
        if (sMilitaryMode) return inb(DataIOPort);
        uint8_t b = inb(DataIOPort);
        if ((b & 0x80) == 0x80) return b + 12;
        if (b == 12) return 0;
        return b;
    }
}

uint8_t RTC::GetDayOfWeek()
{
    WaitForReady();
    outb(AddressIOPort, 0x06);
    if (sBinaryCodedDecimalMode) return FromBCD(inb(DataIOPort));
    return inb(DataIOPort);
}

uint8_t RTC::GetDayOfMonth()
{
    WaitForReady();
    outb(AddressIOPort, 0x07);
    if (sBinaryCodedDecimalMode) return FromBCD(inb(DataIOPort));
    return inb(DataIOPort);
}

uint8_t RTC::GetMonth()
{
    WaitForReady();
    outb(AddressIOPort, 0x08);
    if (sBinaryCodedDecimalMode) return FromBCD(inb(DataIOPort));
    return inb(DataIOPort);
}

uint8_t RTC::GetYear()
{
    WaitForReady();
    outb(AddressIOPort, 0x09);
    if (sBinaryCodedDecimalMode) return FromBCD(inb(DataIOPort));
    return inb(DataIOPort);
}

uint8_t RTC::GetCentury()
{
    WaitForReady();
    outb(AddressIOPort, 0x32);
    if (sBinaryCodedDecimalMode) return FromBCD(inb(DataIOPort));
    return inb(DataIOPort);
}

#define MAX_MONTH_COUNT			12
uint8_t gDaysPerMonth[MAX_MONTH_COUNT + 1] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
uint8_t gLeapYearDaysPerMonth[MAX_MONTH_COUNT + 1] = { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

uint32_t RTC::GetTotalDays(uint16_t pYear,
                           uint8_t pMonth,
                           uint8_t pDay)
{
    uint32_t totalDays = 0;
    uint8_t month = 1;
    uint8_t* daysPerMonth = gDaysPerMonth;
    if (((pYear % 4 == 0) && (pYear % 100) != 0) || (pYear % 400) == 0) daysPerMonth = gLeapYearDaysPerMonth;
    while (month < pMonth) totalDays += daysPerMonth[month++];
    return ((pDay - 1) + totalDays + (365 * (pYear - 1)) + ((pYear - 1) / 4) - ((pYear - 1) / 100) + ((pYear - 1) / 400));
}

uint32_t RTC::GetSecondsSinceEpoch()
{
    uint32_t nowDays = GetTotalDays((GetCentury() * 100) + GetYear(), GetMonth(), GetDayOfMonth());
    uint32_t epochDays = GetTotalDays(1970, 1, 1);
    return ((nowDays - epochDays) * 24 * 60 * 60) + (GetHour() * 60 * 60) + (GetMinute() * 60) + GetSecond();
}
