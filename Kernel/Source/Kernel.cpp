#include <Kernel.h>

extern "C" {
void Kernel(uint32_t pMultiBootMagic, void * pMultiBootData);
}

void Kernel(uint32_t pMultiBootMagic,
            void * pMultiBootData)
{
    if (!MultiBoot::Initialize(pMultiBootMagic, pMultiBootData)) return;

    DeviceManager::Initialize();

    COMPortLogger* comPortLogger = new COMPortLogger();
    if (!DeviceManager::RegisterCOMPortLogger(comPortLogger))
    {
        delete comPortLogger;
        return;
    }
	DeviceManager::COMPortLoggersWriteLine("See? It works!");

    Console* console = new Console();
    if (!DeviceManager::RegisterConsole(console))
    {
        delete console;
        return;
    }
	DeviceManager::GetConsole().Clear(Console::CreateAttributes(Console::Color::LightWhite, Console::Color::DarkBlack));
    DeviceManager::GetConsole().WriteLine("Booting SEMOS...");

    FileSystem::Initialize();
	GDT::Initialize();
	IDT::Initialize();

	PIC::Initialize();
	PIT::Initialize(1000);
	RTC::Initialize();

    //SystemPartition_Initialize();

	time_t startupTime = time(NULL);
	printf("Startup @ %s\n", ctime(&startupTime));

    while (true) ;
}
