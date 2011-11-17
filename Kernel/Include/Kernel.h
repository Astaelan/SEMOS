#ifndef _KERNEL
#define _KERNEL

extern "C" {
#include <types.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
}

#include <MBoot.h>
#include <FileSystem.h>
#include <Hardware/GDT.h>
#include <Hardware/IDT.h>
#include <Hardware/PIC.h>
#include <Hardware/PIT.h>
#include <Hardware/RTC.h>
#include <Hardware/VGAText.h>
#include <Hardware/SystemPartition.h>
#include <Utility/COMPortLogger.h>

// SystemCalls

void Halt();
void Panic(const char * pMessage);

#endif