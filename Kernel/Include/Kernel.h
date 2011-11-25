#ifndef _KERNEL
#define _KERNEL

extern "C" {
#include <types.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
}

#include <iostream>

#include <Core/COMPortLogger.h>
#include <Core/Console.h>
#include <Core/MultiBoot.h>
#include <Core/FileSystem.h>
#include <Core/GDT.h>
#include <Core/IDT.h>
#include <Hardware/PIC.h>
#include <Hardware/PIT.h>
#include <Hardware/RTC.h>
#include <Hardware/SystemPartition.h>

using namespace SEMOS;
using namespace SEMOS::Core;
using namespace SEMOS::Hardware;

// SystemCalls

void Halt();
void Panic(const char * pMessage);

#endif