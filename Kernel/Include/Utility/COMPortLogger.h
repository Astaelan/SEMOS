#ifndef _COMPORTLOGGER_H
#define _COMPORTLOGGER_H

#include <types.h>

void COMPortLogger_Initialize();
void COMPortLogger_WriteData(BYTE b);
void COMPortLogger_WriteLine(const PSTRING pString);
void COMPortLogger_WriteString(const PSTRING pString);
int COMPortLogger_IsTransmitEmpty();

#endif