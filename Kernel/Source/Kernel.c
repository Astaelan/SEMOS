#include <Kernel.h>

void Kernel(UINT32 pMBootMagic,
            PVOID pMBoot)
{
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

	COMPortLogger_Initialize();
	COMPortLogger_WriteLine("See? It works!");

	SystemPartition_Initialize();

    //FILE* fp = fopen("Test", "r");
    //if (fp) { }
	while (TRUE);
}
