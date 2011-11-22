import Core.MultiBoot;
import Core.VGAText;

extern (C) void Kernel(uint pMultiBootMagic,
                       void* pMultiBootData)
{
    Console.Initialize();
    Console.WriteLine("Booting SEMOS...");

    MultiBoot.Initialize(pMultiBootMagic, pMultiBootData);

    while(true) { }
}
