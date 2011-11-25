#include <Kernel.h>

extern "C" {
void Kernel(uint32_t pMultiBootMagic, void * pMultiBootData);
}

void Kernel(uint32_t pMultiBootMagic,
            void * pMultiBootData)
{
	COMPortLogger::Initialize();
	COMPortLogger::WriteLine("See? It works!");

	Console::Clear(Console::CreateAttributes(Console::Color::LightWhite, Console::Color::DarkBlack));
    if (!MultiBoot::Initialize(pMultiBootMagic, pMultiBootData)) Panic("PANIC: Bootloader did not pass valid multiboot data");

    FileSystem::Initialize();

    printf("Booting SEMOS...\n");

	GDT::Initialize();
	IDT::Initialize();
	PIC::Initialize();
	PIT::Initialize(1000);
	RTC_Initialize();

    //SystemPartition_Initialize();

	time_t startupTime = time(NULL);
	printf("Startup @ %s\n", ctime(&startupTime));

    while (true) ;
}
