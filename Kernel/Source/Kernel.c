#include <Kernel.h>

#include "Compat.h"
#include "Sys.h"

#include "CLIFile.h"
#include "MetaData.h"
#include "Type.h"
#include "Heap.h"
#include "Finalizer.h"
#include "System.Net.Sockets.Socket.h"
#include "MethodState.h"

void Kernel(UINT32 pMBootMagic,
            PVOID pMBoot)
{
	COMPortLogger_Initialize();
	COMPortLogger_WriteLine("See? It works!");

	VGAText_Clear(VGATEXT_ATTRIBUTES(VGATEXT_ATTRIBUTE_LIGHT_WHITE, VGATEXT_ATTRIBUTE_DARK_BLACK));
	if (!MBoot_Initialize(pMBootMagic, pMBoot)) Panic("PANIC: Bootloader did not pass valid multiboot data");

    FileSystem_Initialize();

    printf("Booting SEMOS...\n");

	GDT_Initialize();
	IDT_Initialize();
	PIC_Initialize();
	PIT_Initialize(1000);
	RTC_Initialize();

	time_t startupTime = time(NULL);
	printf("Startup @ %s\n", ctime(&startupTime));


	SystemPartition_Initialize();

    printf("CLR Runtime Initializing\n");
	logLevel = 0;

    JIT_Execute_Init();
	MetaData_Init();
	Type_Init();
	Heap_Init();
	Finalizer_Init();
    printf("CLR Runtime Initialized\n");

	tCLIFile *cliFile = CLIFile_Load("/SYSTEM/KERNEL.EXE");
    if (cliFile) printf("Loaded Kernel.exe\n");
	INT32 retValue = CLIFile_Execute(cliFile, 0, NULL);
    printf("Executed Kernel.exe: %d\n", retValue);
    if (retValue) { }
    while (TRUE);
}
