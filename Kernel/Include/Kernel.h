#ifndef _KERNEL
#define _KERNEL

extern "C" {
#include <types.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
}

#include <iostream>

#include <Core/MultiBoot.h>
#include <Core/FileSystem.h>
#include <Core/GDT.h>
#include <Core/IDT.h>
#include <Hardware/PIC.h>
#include <Hardware/PIT.h>
#include <Hardware/RTC.h>
#include <Hardware/VGAText.h>
#include <Hardware/SystemPartition.h>
#include <Utility/COMPortLogger.h>

using namespace SEMOS;
using namespace SEMOS::Core;
using namespace SEMOS::Hardware;
using namespace SEMOS::Utility;

// SystemCalls

void Halt();
void Panic(const char * pMessage);

#endif