#ifndef _RTC_H
#define _RTC_H

#include "types.h"

void RTC_Initialize();
BYTE RTC_GetSecond();
BYTE RTC_GetMinute();
BYTE RTC_GetHour();
BYTE RTC_GetDayOfTheWeek();
BYTE RTC_GetDayOfTheMonth();
BYTE RTC_GetMonth();
BYTE RTC_GetYear();
BYTE RTC_GetCentury();
UINT32 RTC_GetSecondsSinceEpoch();

#endif