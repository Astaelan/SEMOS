#ifndef _COMPORTLOGGER_H
#define _COMPORTLOGGER_H

extern "C" {
#include <types.h>
}

void COMPortLogger_Initialize();
void COMPortLogger_WriteData(BYTE b);
void COMPortLogger_WriteLine(const char * pString);
void COMPortLogger_WriteString(const char * pString);
int COMPortLogger_IsTransmitEmpty();

#endif