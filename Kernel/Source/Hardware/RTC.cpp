#include <PortIO.h>
#include <BCDHelper.h>
#include <Hardware/RTC.h>

#define RTC_IO_BASE     0x70

#define RTC_IO_ADDRESS  0x00
#define RTC_IO_DATA     0x01

BOOL gRTCBCDMode;
BOOL gRTC24HourMode;
BYTE gRTCStatusB;

static inline void RTC_WaitForReady()
{ do { outb(RTC_IO_BASE + RTC_IO_ADDRESS, 10); } while ((inb(RTC_IO_BASE + RTC_IO_DATA) & 0x80) != 0); }

void RTC_Initialize()
{
    RTC_WaitForReady();
    outb(RTC_IO_BASE + RTC_IO_ADDRESS, 0x0B);
    gRTCStatusB = inb(RTC_IO_BASE + RTC_IO_DATA);
    if ((gRTCStatusB & 0x02) == 0x02) gRTC24HourMode = TRUE;
    else gRTC24HourMode = FALSE;
    if ((gRTCStatusB & 0x04) == 0x04) gRTCBCDMode = FALSE;
    else gRTCBCDMode = TRUE;
}

BYTE RTC_GetSecond()
{
    RTC_WaitForReady();
    outb(RTC_IO_BASE + RTC_IO_ADDRESS, 0x00);
    if (gRTCBCDMode) return BCDHelper_FromBCD(inb(RTC_IO_BASE + RTC_IO_DATA));
    return inb(RTC_IO_BASE + RTC_IO_DATA);
}

BYTE RTC_GetMinute()
{
    RTC_WaitForReady();
    outb(RTC_IO_BASE + RTC_IO_ADDRESS, 0x02);
    if (gRTCBCDMode) return BCDHelper_FromBCD(inb(RTC_IO_BASE + RTC_IO_DATA));
    return inb(RTC_IO_BASE + RTC_IO_DATA);
}

BYTE RTC_GetHour()
{
    RTC_WaitForReady();
    outb(RTC_IO_BASE + RTC_IO_ADDRESS, 0x04);
    if (gRTCBCDMode)
    {
        if (gRTC24HourMode) return BCDHelper_FromBCD(inb(RTC_IO_BASE + RTC_IO_DATA));
        BYTE b = inb(RTC_IO_BASE + RTC_IO_DATA);
        if ((b & 0x80) == 0x80) return (BYTE)(BCDHelper_FromBCD(b) + 12);
        if (BCDHelper_FromBCD(b) == 12) return 0;
        return BCDHelper_FromBCD(b);
    }
    else
    {
        if (gRTC24HourMode) return inb(RTC_IO_BASE + RTC_IO_DATA);
        BYTE b = inb(RTC_IO_BASE + RTC_IO_DATA);
        if ((b & 0x80) == 0x80) return (BYTE)(b + 12);
        if (b == 12) return 0;
        return b;
    }
}

BYTE RTC_GetDayOfTheWeek()
{
    RTC_WaitForReady();
    outb(RTC_IO_BASE + RTC_IO_ADDRESS, 0x06);
    if (gRTCBCDMode) return BCDHelper_FromBCD(inb(RTC_IO_BASE + RTC_IO_DATA));
    return inb(RTC_IO_BASE + RTC_IO_DATA);
}

BYTE RTC_GetDayOfTheMonth()
{
    RTC_WaitForReady();
    outb(RTC_IO_BASE + RTC_IO_ADDRESS, 0x07);
    if (gRTCBCDMode) return BCDHelper_FromBCD(inb(RTC_IO_BASE + RTC_IO_DATA));
    return inb(RTC_IO_BASE + RTC_IO_DATA);
}

BYTE RTC_GetMonth()
{
    RTC_WaitForReady();
    outb(RTC_IO_BASE + RTC_IO_ADDRESS, 0x08);
    if (gRTCBCDMode) return BCDHelper_FromBCD(inb(RTC_IO_BASE + RTC_IO_DATA));
    return inb(RTC_IO_BASE + RTC_IO_DATA);
}

BYTE RTC_GetYear()
{
    RTC_WaitForReady();
    outb(RTC_IO_BASE + RTC_IO_ADDRESS, 0x09);
    if (gRTCBCDMode) return BCDHelper_FromBCD(inb(RTC_IO_BASE + RTC_IO_DATA));
    return inb(RTC_IO_BASE + RTC_IO_DATA);
}

BYTE RTC_GetCentury()
{
    RTC_WaitForReady();
    outb(RTC_IO_BASE + RTC_IO_ADDRESS, 0x32);
    if (gRTCBCDMode) return BCDHelper_FromBCD(inb(RTC_IO_BASE + RTC_IO_DATA));
    return inb(RTC_IO_BASE + RTC_IO_DATA);
}

#define RTC_MAX_MONTH_COUNT			12
BYTE gRTCDaysPerMonth[RTC_MAX_MONTH_COUNT + 1] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
BYTE gRTCLeapYearDaysPerMonth[RTC_MAX_MONTH_COUNT + 1] = { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };


UINT32 RTC_GetTotalDays(UINT16 pYear,
                        BYTE pMonth,
                        BYTE pDay)
{
    UINT32 totalDays = 0;
    BYTE month = 1;
    PBYTE daysPerMonth = gRTCDaysPerMonth;
    if (((pYear % 4 == 0) && (pYear % 100) != 0) || (pYear % 400) == 0) daysPerMonth = gRTCLeapYearDaysPerMonth;
    while (month < pMonth) totalDays += daysPerMonth[month++];
    return ((pDay - 1) + totalDays + (365 * (pYear - 1)) + ((pYear - 1) / 4) - ((pYear - 1) / 100) + ((pYear - 1) / 400));
}

UINT32 RTC_GetSecondsSinceEpoch()
{
    UINT32 nowDays = RTC_GetTotalDays((RTC_GetCentury() * 100) + RTC_GetYear(), RTC_GetMonth(), RTC_GetDayOfTheMonth());
    UINT32 epochDays = RTC_GetTotalDays(1970, 1, 1);
    return ((nowDays - epochDays) * 24 * 60 * 60) + (RTC_GetHour() * 60 * 60) + (RTC_GetMinute() * 60) + RTC_GetSecond();
}
