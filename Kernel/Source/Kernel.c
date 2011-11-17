#include <Kernel.h>

extern "C" {
void Kernel(UINT32 pMBootMagic, PVOID pMBoot);
}

void Kernel(UINT32 pMBootMagic,
            PVOID pMBoot)
{
	SEMOS::Utility::COMPortLogger::Initialize();
	SEMOS::Utility::COMPortLogger::WriteLine("See? It works!");

	SEMOS::Hardware::VGAText::Clear(SEMOS::Hardware::VGAText::CreateAttributes(SEMOS::Hardware::VGAText::LightWhite, SEMOS::Hardware::VGAText::DarkBlack));
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
}
