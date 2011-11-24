import Core.Console;
import Core.Debug;
import Core.GDT;
import Core.IDT;
import Core.MultiBoot;
import Hardware.PIC;

extern (C) void Kernel(uint pMultiBootMagic,
                       void* pMultiBootData)
{
    Debug.Initialize();
    Console.Initialize();
    Console.WriteLine("Booting SEMOS...");

    MultiBoot.Initialize(pMultiBootMagic, pMultiBootData);

    GDT.Initialize();
    //IDT.Initialize();
    //PIC.Initialize();

    Console.WriteLine("Booted");
    while(true) { }
}
